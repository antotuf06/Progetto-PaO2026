#include "dettagliattivita.h"
#include "ui_dettagliattivita.h"
#include "Scadenza.h"
#include "Impegno.h"
#include "Appuntamento.h"
#include <QLabel>
#include <QHBoxLayout>

DettagliAttivita::DettagliAttivita(Attivita* a, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DettagliAttivita)
    , attivita(a)
{
    ui->setupUi(this);

    if (!attivita) return;

    ui->titleLabel->setText(attivita->getTitle());
    ui->iconLabel->setPixmap(QPixmap(attivita->getIconPath()));
    ui->urgentBadge->setVisible(attivita->isUrgent());

    // getType() qui mostra solo una semplice stringa, uso consentito dalle specifiche
    aggiungiRiga("Tipo", attivita->getType());
    if (!attivita->getCateg().isEmpty()) aggiungiRiga("Categoria", attivita->getCateg());
    aggiungiRiga("Data", attivita->getDate().toString("dd/MM/yyyy"));

    // dispatch polimorfico (pattern Visitor): ogni tipo aggiunge le proprie righe di dettaglio
    attivita->accept(*this);

    aggiungiRiga("Ricorrenza", periodicitaTesto(attivita));

    ui->descrizioneLabel->setText(attivita->getDescr());
    ui->labelDescrTitolo->setVisible(!attivita->getDescr().isEmpty());
    ui->descrizioneLabel->setVisible(!attivita->getDescr().isEmpty());

    connect(ui->btnModifica, &QPushButton::clicked, this, [this]() { emit richiestaModifica(attivita); });
    connect(ui->btnElimina, &QPushButton::clicked, this, [this]() { emit richiestaEliminazione(attivita); });
}

DettagliAttivita::~DettagliAttivita()
{
    delete ui;
}

void DettagliAttivita::aggiungiRiga(const QString& etichetta, const QString& valore)
{
    QWidget* riga = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(riga);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    QLabel* labelEtichetta = new QLabel(etichetta + ":");
    labelEtichetta->setStyleSheet("font-weight: bold; color: #2b2b2b;");
    labelEtichetta->setMinimumWidth(90);

    QLabel* labelValore = new QLabel(valore);
    labelValore->setStyleSheet("color: #5f5f5f;");
    labelValore->setWordWrap(true);

    layout->addWidget(labelEtichetta, 0, Qt::AlignTop);
    layout->addWidget(labelValore, 1);

    ui->campiLayout->addWidget(riga);
}

QString DettagliAttivita::periodicitaTesto(const Attivita* a) const
{
    switch (a->getPeriodicity()) {
    case Giornaliera: return QString("Giornaliera fino al %1").arg(a->getEndPeriod().toString("dd/MM/yyyy"));
    case Settimanale: return QString("Settimanale fino al %1").arg(a->getEndPeriod().toString("dd/MM/yyyy"));
    case Mensile: return QString("Mensile fino al %1").arg(a->getEndPeriod().toString("dd/MM/yyyy"));
    default: return "Nessuna";
    }
}

void DettagliAttivita::visit(Scadenza& s)
{
    aggiungiRiga("Ora", s.getHour().toString("HH:mm"));
    aggiungiRiga("Stato", s.isDone() ? "Completata" : "Da fare");
}

void DettagliAttivita::visit(Impegno& i)
{
    aggiungiRiga("Orario", QString("%1 - %2").arg(i.getStart().toString("HH:mm"), i.getFinish().toString("HH:mm")));
}

void DettagliAttivita::visit(Appuntamento& a)
{
    aggiungiRiga("Ora", a.getHour().toString("HH:mm"));
    if (!a.getPlace().isEmpty()) aggiungiRiga("Luogo", a.getPlace());
}
