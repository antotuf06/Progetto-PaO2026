#include "formappuntamento.h"
#include "ui_formappuntamento.h"

FormAppuntamento::FormAppuntamento(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormAppuntamento)
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

    connect(ui->btnAnnulla, &QPushButton::clicked, this, &FormAppuntamento::annulla);
    connect(ui->btnSalva, &QPushButton::clicked, this, &FormAppuntamento::salva);
}

FormAppuntamento::~FormAppuntamento()
{
    delete ui;
}

Appuntamento* FormAppuntamento::creaAttivita(unsigned int id) const
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

    return new Appuntamento(ui->editTitolo->text(), ui->editDescr->toPlainText(), ui->editCateg->text(),
                             ui->dateEdit->date(), id, periodicita, fineRicorrenza, ui->editLuogo->text(), ui->timeEdit->time());
}
