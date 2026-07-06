#include "cardattivita.h"
#include "ui_cardattivita.h"
#include "Scadenza.h"
#include <QMouseEvent>

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

    QString tipo = attivita->getType();

    // colore della barra a sinistra in base al tipo di attivita'
    QString coloreTipo = "#a855f7"; // viola: Appuntamento (default)
    if (tipo == "Scadenza") coloreTipo = "#ec4899";      // rosa
    else if (tipo == "Impegno") coloreTipo = "#3b82f6";  // blu

    // il bordo colorato e' parte del bordo stesso della card (non un widget separato)
    // cosi' segue esattamente la curvatura del border-radius senza uscire dai bordi
    QString bg = urgente ? "#fff5f5" : "#ffffff";
    QString bordo = urgente ? "#f87171" : "#b0b0b0";
    setStyleSheet(QString(
        "#CardAttivita {"
        "    background-color: %1;"
        "    border: 2px solid %2;"
        "    border-left: 6px solid %3;"
        "    border-radius: 10px;"
        "}"
    ).arg(bg, bordo, coloreTipo));

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

void CardAttivita::mousePressEvent(QMouseEvent* event)
{
    // il click su checkbox/bottone non arriva qui (i widget figli lo consumano prima),
    // quindi questo intercetta solo i click sulle parti "passive" della card
    if (event->button() == Qt::LeftButton && attivita) {
        emit cliccata(attivita);
    }
    QFrame::mousePressEvent(event);
}
