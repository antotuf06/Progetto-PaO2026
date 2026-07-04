#include "formscadenza.h"
#include "ui_formscadenza.h"

FormScadenza::FormScadenza(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormScadenza)
{
    ui->setupUi(this);

    ui->dateTimeEdit->setDate(QDate::currentDate());
    ui->dateRicor->setDate(QDate::currentDate());

    auto aggiornaVisibilitaRicorrenza = [this](bool checked) {
        ui->dateRicor->setVisible(checked);
        ui->TipoRicor->setVisible(checked);
        ui->comboBox->setVisible(checked);
    };

    connect(ui->Ricorrenza, &QCheckBox::toggled, this, aggiornaVisibilitaRicorrenza);
    aggiornaVisibilitaRicorrenza(ui->Ricorrenza->isChecked());

    connect(ui->btnAnnulla, &QPushButton::clicked, this, &FormScadenza::annulla);
    connect(ui->btnSalva, &QPushButton::clicked, this, &FormScadenza::salva);
}

FormScadenza::~FormScadenza()
{
    delete ui;
}

Scadenza* FormScadenza::creaAttivita(unsigned int id) const
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

    return new Scadenza(ui->editTitolo->text(), ui->editDescr->toPlainText(), ui->editCateg->text(),
                         ui->dateTimeEdit->date(), id, periodicita, fineRicorrenza, ui->dateTimeEdit->time(), false);
}
