#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardattivita.h"
#include "dettagliattivita.h"
#include "formscadenza.h"
#include "formimpegno.h"
#include "formappuntamento.h"
#include "Scadenza.h"
#include "Impegno.h"
#include "Appuntamento.h"
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
    colonneCorrenti = 2;
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


    menuCreaAttivita->installEventFilter(this);

    connect(ui->dateMin2, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(-7)); });
    connect(ui->dateMin1, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(-1)); });
    connect(ui->datePlus1, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(1)); });
    connect(ui->datePlus2, &QToolButton::clicked, this, [this]() { ui->dateEdit->setDate(ui->dateEdit->date().addDays(7)); });
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, [this]() { popolaGrigliaAttivita(); });

    connect(ui->SaveButton, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getSaveFileName(this, "Salva attività", QString(), "File JSON (*.json)");
        if (!file.isEmpty()) {
            manager->salvaFile(file);
        }
    });

    connect(ui->ImportButton, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, "Importa attività", QString(), "File JSON (*.json)");
        if (!file.isEmpty()) {
            manager->importaFile(file);
            popolaGrigliaAttivita();
        }
    });

    ui->fltrImp->setCheckable(true);
    ui->fltrApp->setCheckable(true);
    ui->fltrScad->setCheckable(true);
    ui->fltrUrg->setCheckable(true);

    connect(ui->fltrImp, &QPushButton::toggled, this, [this]() { popolaGrigliaAttivita(); });
    connect(ui->fltrApp, &QPushButton::toggled, this, [this]() { popolaGrigliaAttivita(); });
    connect(ui->fltrScad, &QPushButton::toggled, this, [this]() { popolaGrigliaAttivita(); });
    connect(ui->fltrUrg, &QPushButton::toggled, this, [this]() { popolaGrigliaAttivita(); });

    connect(ui->fltrHome, &QPushButton::clicked, this, [this]() {
        ui->fltrImp->setChecked(false);
        ui->fltrApp->setChecked(false);
        ui->fltrScad->setChecked(false);
        ui->fltrUrg->setChecked(false);
        ui->SearchBar->clear();
        popolaGrigliaAttivita();
    });

    connect(ui->SearchBar, &QLineEdit::textChanged, this, [this]() { popolaGrigliaAttivita(); });
}

MainWindow::~MainWindow()
{
    delete manager;
    delete ui;
}

void MainWindow::popolaGrigliaAttivita()
{
    QLayoutItem* item;
    while ((item = ui->gridAttivita->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int colonne = qMax(1, colonneCorrenti);

    // mostra solo le attivita' della data selezionata nel frameData
    QDate dataSelezionata = ui->dateEdit->date();
    bool soloImpegno = ui->fltrImp->isChecked();
    bool soloAppuntamento = ui->fltrApp->isChecked();
    bool soloScadenza = ui->fltrScad->isChecked();
    bool soloUrgenti = ui->fltrUrg->isChecked();
    bool nessunFiltroTipo = !soloImpegno && !soloAppuntamento && !soloScadenza;
    QString testoRicerca = ui->SearchBar->text().trimmed();

    QVector<Attivita*> lista;
    for (Attivita* a : manager->getLista()) {
        if (a->getDate() != dataSelezionata) continue;

        if (!nessunFiltroTipo) {
            QString tipo = a->getType();
            bool tipoOk = (soloImpegno && tipo == "Impegno")
                       || (soloAppuntamento && tipo == "Appuntamento")
                       || (soloScadenza && tipo == "Scadenza");
            if (!tipoOk) continue;
        }

        if (soloUrgenti && !a->isUrgent()) continue;

        if (!testoRicerca.isEmpty()) {
            bool trovato = a->getTitle().contains(testoRicerca, Qt::CaseInsensitive)
                        || a->getCateg().contains(testoRicerca, Qt::CaseInsensitive)
                        || a->getDescr().contains(testoRicerca, Qt::CaseInsensitive);
            if (!trovato) continue;
        }

        lista.append(a);
    }

    for (int idx = 0; idx < lista.size(); ++idx) {
        CardAttivita* card = new CardAttivita(lista[idx]);
        connect(card, &CardAttivita::attivitaModificata, this, &MainWindow::onAttivitaModificata);
        connect(card, &CardAttivita::cliccata, this, &MainWindow::mostraDettagli);
        ui->gridAttivita->addWidget(card, idx / colonne, idx % colonne, Qt::AlignTop | Qt::AlignLeft);
    }

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

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == menuCreaAttivita && event->type() == QEvent::Show) {
        const QSize menuSize = menuCreaAttivita->size();
        const QPoint sottoBottone = ui->btnCreaAttivita->mapToGlobal(QPoint(0, ui->btnCreaAttivita->height()));
        const QPoint sopraBottone = ui->btnCreaAttivita->mapToGlobal(QPoint(0, 0)) - QPoint(0, menuSize.height());
        const int finestraFondo = this->mapToGlobal(QPoint(0, this->height())).y();

        if (sottoBottone.y() + menuSize.height() > finestraFondo) {
            menuCreaAttivita->move(sopraBottone);
        } else {
            menuCreaAttivita->move(sottoBottone);
        }
    }
    return QMainWindow::eventFilter(watched, event);
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

void MainWindow::mostraDettagli(Attivita* a)
{
    QLayout* layout = ui->detailsPage->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    DettagliAttivita* dettagli = new DettagliAttivita(a);
    connect(dettagli, &DettagliAttivita::richiestaModifica, this, &MainWindow::onModificaAttivita);
    connect(dettagli, &DettagliAttivita::richiestaEliminazione, this, &MainWindow::onEliminaAttivita);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(dettagli);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("background-color: transparent;");

    layout->addWidget(scrollArea);
    ui->rxColumn->setCurrentWidget(ui->detailsPage);
}

void MainWindow::onModificaAttivita(Attivita* a)
{
    if (!a) return;

    QString tipo = a->getType();
    if (tipo == "Scadenza") {
        FormScadenza* form = new FormScadenza();
        form->caricaDati(static_cast<Scadenza*>(a));
        connect(form, &FormScadenza::salva, this, [this, form, a]() {
            int index = manager->getLista().indexOf(a);
            if (index >= 0) {
                manager->updateAttivita(index, form->creaAttivita(a->getId()), false);
                popolaGrigliaAttivita();
                mostraDettagli(manager->getAttivita(index));
            } else {
                popolaGrigliaAttivita();
                onCreazioneAnnullata();
            }
        });
        mostraFormCreazione(form);
    } else if (tipo == "Impegno") {
        FormImpegno* form = new FormImpegno();
        form->caricaDati(static_cast<Impegno*>(a));
        connect(form, &FormImpegno::salva, this, [this, form, a]() {
            int index = manager->getLista().indexOf(a);
            if (index >= 0) {
                manager->updateAttivita(index, form->creaAttivita(a->getId()), false);
                popolaGrigliaAttivita();
                mostraDettagli(manager->getAttivita(index));
            } else {
                popolaGrigliaAttivita();
                onCreazioneAnnullata();
            }
        });
        mostraFormCreazione(form);
    } else if (tipo == "Appuntamento") {
        FormAppuntamento* form = new FormAppuntamento();
        form->caricaDati(static_cast<Appuntamento*>(a));
        connect(form, &FormAppuntamento::salva, this, [this, form, a]() {
            int index = manager->getLista().indexOf(a);
            if (index >= 0) {
                manager->updateAttivita(index, form->creaAttivita(a->getId()), false);
                popolaGrigliaAttivita();
                mostraDettagli(manager->getAttivita(index));
            } else {
                popolaGrigliaAttivita();
                onCreazioneAnnullata();
            }
        });
        mostraFormCreazione(form);
    }
}

void MainWindow::onEliminaAttivita(Attivita* a)
{
    if (!a) return;

    int index = manager->getLista().indexOf(a);
    if (index >= 0) {
        manager->removeAttivita(index, false); // non salva subito su file
    }
    popolaGrigliaAttivita();
    onCreazioneAnnullata();
}
