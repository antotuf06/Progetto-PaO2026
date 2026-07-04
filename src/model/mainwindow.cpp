#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardattivita.h"
#include "formscadenza.h"
#include "formimpegno.h"
#include "formappuntamento.h"
#include <QScrollArea>
#include <QResizeEvent>
#include <QFileDialog>

static const int LARGHEZZA_CARD = 270;
static const int SPAZIATURA_GRID = 10;
static const int SOGLIA_COMPATTA = 700;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(new Manager())
    , colonneCorrenti(-1)
    , modalitaCompatta(false)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    ui->centralArea->setStretchFactor(ui->cxColumn, 3);
    ui->centralArea->setStretchFactor(ui->rxColumn, 1);

    // l'app parte sempre senza attivita': compaiono solo importando un file o creandole
    colonneCorrenti = 2; // valore iniziale ragionevole; verra' corretto al primo resize reale
    popolaGrigliaAttivita();

    menuCreaAttivita = new QMenu(this);
    menuCreaAttivita->setStyleSheet(
        "QMenu {"
        "   background-color: #ffffff;"
        "   border: 1px solid #16a34a;"
        "   border-radius: 8px;"
        "   padding: 4px;"
        "}"
        "QMenu::item {"
        "   padding: 8px 20px;"
        "   color: #14532d;"
        "   border-radius: 6px;"
        "}"
        "QMenu::item:selected {"
        "   background-color: #22c55e;"
        "   color: #ffffff;"
        "}"
    );
    menuCreaAttivita->addAction("Impegno", this, &MainWindow::onCreaImpegno);
    menuCreaAttivita->addAction("Appuntamento", this, &MainWindow::onCreaAppuntamento);
    menuCreaAttivita->addAction("Scadenza", this, &MainWindow::onCreaScadenza);

    ui->btnCreaAttivita->setMenu(menuCreaAttivita);

    connect(ui->dateMin2, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(-7)); });
    connect(ui->dateMin1, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(-1)); });
    connect(ui->datePlus1, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(1)); });
    connect(ui->datePlus2, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(7)); });
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, [this]() { popolaGrigliaAttivita(); });

    connect(ui->SaveButton, &QPushButton::clicked, this, [this]() { manager->salvaFile(); });

    connect(ui->ImportButton, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, "Importa attività", QString(), "File JSON (*.json)");
        if (!file.isEmpty()) {
            manager->importaFile(file);
            popolaGrigliaAttivita();
        }
    });
}

MainWindow::~MainWindow()
{
    delete manager;
    delete ui;
}

void MainWindow::on_page0_customContextMenuRequested(const QPoint &pos)
{

}

void MainWindow::popolaGrigliaAttivita()
{
    QLayoutItem* item;
    while ((item = ui->gridAttivita->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int colonne = qMax(1, colonneCorrenti);

    // mostra solo le attivita' della data selezionata nel frameData; le istanze di
    // un'attivita' ricorrente sono gia' oggetti separati (uno per occorrenza, generati
    // da Manager::addPeriodicity), quindi questo filtro le mostra automaticamente
    // tutte nei rispettivi giorni
    QDate dataSelezionata = ui->dateEdit->date();
    QVector<Attivita*> lista;
    for (Attivita* a : manager->getLista()) {
        if (a->getDate() == dataSelezionata) {
            lista.append(a);
        }
    }

    for (int idx = 0; idx < lista.size(); ++idx) {
        CardAttivita* card = new CardAttivita(lista[idx]);
        connect(card, &CardAttivita::attivitaModificata, this, &MainWindow::onAttivitaModificata);
        ui->gridAttivita->addWidget(card, idx / colonne, idx % colonne, Qt::AlignTop | Qt::AlignLeft);
    }

    // spinge tutto lo spazio in eccesso oltre l'ultima riga/colonna usata,
    // cosi' le card restano impacchettate senza spaziature extra tra loro;
    // azzera prima gli stretch di eventuali configurazioni precedenti (numero
    // di colonne diverso) cosi' non restano stretch "fantasma" su righe/colonne vecchie
    int righe = lista.isEmpty() ? 0 : (lista.size() + colonne - 1) / colonne;
    for (int i = 0; i < 8; ++i) {
        ui->gridAttivita->setColumnStretch(i, 0);
        ui->gridAttivita->setRowStretch(i, 0);
    }
    ui->gridAttivita->setColumnStretch(colonne, 1);
    ui->gridAttivita->setRowStretch(righe, 1);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    adattaLayout();
}

void MainWindow::adattaLayout()
{
    bool compatta = width() < SOGLIA_COMPATTA;
    if (compatta != modalitaCompatta) {
        modalitaCompatta = compatta;
        ui->lxColumn->setVisible(!modalitaCompatta);
        ui->rxColumn->setVisible(!modalitaCompatta);
    }

    int colonne = calcolaColonne();
    if (colonne != colonneCorrenti) {
        colonneCorrenti = colonne;
        popolaGrigliaAttivita();
    }
}

int MainWindow::calcolaColonne() const
{
    if (modalitaCompatta) return 1;

    int disponibile = ui->cxColumn->width() - 10; // margini sinistro/destro del grid
    int colonne = (disponibile + SPAZIATURA_GRID) / (LARGHEZZA_CARD + SPAZIATURA_GRID);
    return qBound(1, colonne, 4);
}

void MainWindow::onAttivitaModificata(Attivita* a)
{
    Q_UNUSED(a);
    // il salvataggio su file avviene solo quando l'utente preme il tasto "Salva" in alto
}

void MainWindow::mostraFormCreazione(QWidget* form)
{
    QLayout* layout = ui->editcreatePage->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(form);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("background-color: transparent;");

    layout->addWidget(scrollArea);
    connect(form, SIGNAL(annulla()), this, SLOT(onCreazioneAnnullata()));
    ui->rxColumn->setCurrentWidget(ui->editcreatePage);
}

void MainWindow::onCreaImpegno()
{
    FormImpegno* form = new FormImpegno();
    connect(form, &FormImpegno::salva, this, [this, form]() {
        manager->addPeriodicity(form->creaAttivita(prossimoId()), false); // non salva subito su file
        popolaGrigliaAttivita();
        onCreazioneAnnullata();
    });
    mostraFormCreazione(form);
}

void MainWindow::onCreaAppuntamento()
{
    FormAppuntamento* form = new FormAppuntamento();
    connect(form, &FormAppuntamento::salva, this, [this, form]() {
        manager->addPeriodicity(form->creaAttivita(prossimoId()), false); // non salva subito su file
        popolaGrigliaAttivita();
        onCreazioneAnnullata();
    });
    mostraFormCreazione(form);
}

void MainWindow::onCreaScadenza()
{
    FormScadenza* form = new FormScadenza();
    connect(form, &FormScadenza::salva, this, [this, form]() {
        manager->addPeriodicity(form->creaAttivita(prossimoId()), false); // non salva subito su file
        popolaGrigliaAttivita();
        onCreazioneAnnullata();
    });
    mostraFormCreazione(form);
}

void MainWindow::onCreazioneAnnullata()
{
    ui->rxColumn->setCurrentWidget(ui->idlePage);
}

unsigned int MainWindow::prossimoId() const
{
    unsigned int maxId = 0;
    for (Attivita* a : manager->getLista()) {
        maxId = qMax(maxId, a->getId());
    }
    return maxId + 1;
}
