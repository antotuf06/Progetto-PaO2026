#include "formscadenza.h"
#include "ui_formscadenza.h"

FormScadenza::FormScadenza(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormScadenza)
{
    ui->setupUi(this);
}

FormScadenza::~FormScadenza()
{
    delete ui;
}
