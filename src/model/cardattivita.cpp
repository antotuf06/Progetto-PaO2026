#include "cardattivita.h"
#include "ui_cardattivita.h"
#include "Scadenza.h"

CardAttivita::CardAttivita(Attivita* a, QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::CardAttivita)
    , attivita(a)
{
    ui->setupUi(this);

    connect(ui->btnAzione, &QPushButton::clicked, this, &CardAttivita::onBtnAzioneClicked);
    connect(ui->chkCompletata, &QCheckBox::toggled, this, &CardAttivita::onChkCompletataToggled);

    aggiornaVisualizzazione();
}

CardAttivita::~CardAttivita()
{
    delete ui;
}

Attivita* CardAttivita::getAttivita() const
{
    return attivita;
}

void CardAttivita::aggiornaVisualizzazione()
{
    if (!attivita) return;

    ui->iconLabel->setPixmap(QPixmap(attivita->getIconPath()));
    ui->titleLabel->setText(attivita->getTitle());
    ui->summaryLabel->setText(attivita->getSummary());

    bool urgente = attivita->isUrgent();
    ui->urgentBadge->setVisible(urgente);
    setStyleSheet(urgente
        ? "#CardAttivita { background-color: #fff5f5; border: 2px solid #f87171; border-radius: 10px; }"
        : "#CardAttivita { background-color: #ffffff; border: 2px solid #b0b0b0; border-radius: 10px; }");

    QString tipo = attivita->getType();
    bool isScadenza = (tipo == "Scadenza");
    ui->chkCompletata->setVisible(isScadenza);
    ui->btnAzione->setVisible(!isScadenza);

    if (isScadenza) {
        Scadenza* s = static_cast<Scadenza*>(attivita);
        ui->chkCompletata->blockSignals(true);
        ui->chkCompletata->setChecked(s->isDone());
        ui->chkCompletata->blockSignals(false);
    } else if (tipo == "Impegno") {
        ui->btnAzione->setText("+30min");
    } else if (tipo == "Appuntamento") {
        ui->btnAzione->setText("+1h");
    }
}

void CardAttivita::onBtnAzioneClicked()
{
    if (!attivita) return;
    attivita->performAction();
    aggiornaVisualizzazione();
    emit attivitaModificata(attivita);
}

void CardAttivita::onChkCompletataToggled(bool checked)
{
    Q_UNUSED(checked);
    if (!attivita) return;
    attivita->performAction(); // Scadenza::performAction inverte lo stato "done"
    aggiornaVisualizzazione();
    emit attivitaModificata(attivita);
}
