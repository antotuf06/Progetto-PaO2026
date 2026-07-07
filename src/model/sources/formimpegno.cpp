#include "formimpegno.h"
#include "ui_formimpegno.h"

FormImpegno::FormImpegno(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormImpegno)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateRicor->setDate(QDate::currentDate());

    auto aggiornaVisibilitaRicorrenza = [this](bool checked) {
        ui->dateRicor->setVisible(checked);
        ui->TipoRicor->setVisible(checked);
        ui->comboBox->setVisible(checked);
    };

    connect(ui->Ricorrenza, &QCheckBox::toggled, this, aggiornaVisibilitaRicorrenza);
    aggiornaVisibilitaRicorrenza(ui->Ricorrenza->isChecked());

    connect(ui->btnAnnulla, &QPushButton::clicked, this, &FormImpegno::annulla);
    connect(ui->btnSalva, &QPushButton::clicked, this, &FormImpegno::salva);
}

FormImpegno::~FormImpegno()
{
    delete ui;
}

Impegno* FormImpegno::creaAttivita(unsigned int id) const
{
    periodicityTipe periodicita = Nessuna;
    QDate fineRicorrenza;
    if (ui->Ricorrenza->isChecked()) {
        QString tipo = ui->comboBox->currentText();
        if (tipo == "Giornaliera") periodicita = Giornaliera;
        else if (tipo == "Settimanale") periodicita = Settimanale;
        else if (tipo == "Mensile") periodicita = Mensile;
        fineRicorrenza = ui->dateRicor->date();
    }

    return new Impegno(ui->editTitolo->text(), ui->editDescr->toPlainText(), ui->editCateg->text(),
                        ui->dateEdit->date(), id, periodicita, fineRicorrenza, ui->timeStart->time(), ui->timeFinish->time());
}

void FormImpegno::caricaDati(Impegno* i)
{
    if (!i) return;

    ui->editTitolo->setText(i->getTitle());
    ui->editCateg->setText(i->getCateg());
    ui->editDescr->setPlainText(i->getDescr());
    ui->dateEdit->setDate(i->getDate());
    ui->timeStart->setTime(i->getStart());
    ui->timeFinish->setTime(i->getFinish());

    bool ricorrente = i->getPeriodicity() != Nessuna;
    ui->Ricorrenza->setChecked(ricorrente);
    if (ricorrente) {
        QString tipo;
        if (i->getPeriodicity() == Giornaliera) tipo = "Giornaliera";
        else if (i->getPeriodicity() == Settimanale) tipo = "Settimanale";
        else if (i->getPeriodicity() == Mensile) tipo = "Mensile";
        ui->comboBox->setCurrentText(tipo);
        ui->dateRicor->setDate(i->getEndPeriod());
    }
}
