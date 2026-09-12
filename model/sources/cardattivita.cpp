#include "cardattivita.h"
#include "ui_cardattivita.h"
#include "Scadenza.h"
#include "Impegno.h"
#include "Appuntamento.h"
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

    // dispatch polimorfico (pattern Visitor): ogni tipo configura a modo suo
    // colore, checkbox/pulsante azione, senza controllare esplicitamente il tipo
    attivita->accept(*this);
}

void CardAttivita::impostaStileBordo(const QString& coloreAccento)
{
    // il bordo colorato e' parte del bordo stesso della card (non un widget separato)
    // cosi' segue esattamente la curvatura del border-radius senza uscire dai bordi
    bool urgente = attivita->isUrgent();
    QString bg = urgente ? "#fff5f5" : "#ffffff";
    QString bordo = urgente ? "#f87171" : "#b0b0b0";
    setStyleSheet(QString(
        "#CardAttivita {"
        "    background-color: %1;"
        "    border: 2px solid %2;"
        "    border-left: 6px solid %3;"
        "    border-radius: 10px;"
        "}"
    ).arg(bg, bordo, coloreAccento));
}

void CardAttivita::visit(Scadenza& s)
{
    impostaStileBordo("#ec4899"); // rosa
    ui->chkCompletata->setVisible(true);
    ui->chkCompletata->blockSignals(true);
    ui->chkCompletata->setChecked(s.isDone());
    ui->chkCompletata->blockSignals(false);
    ui->btnAzione->setVisible(false);
}

void CardAttivita::visit(Impegno& i)
{
    Q_UNUSED(i);
    impostaStileBordo("#3b82f6"); // blu
    ui->chkCompletata->setVisible(false);
    ui->btnAzione->setVisible(true);
    ui->btnAzione->setText("+30min");
}

void CardAttivita::visit(Appuntamento& a)
{
    Q_UNUSED(a);
    impostaStileBordo("#a855f7"); // viola
    ui->chkCompletata->setVisible(false);
    ui->btnAzione->setVisible(true);
    ui->btnAzione->setText("+1h");
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
