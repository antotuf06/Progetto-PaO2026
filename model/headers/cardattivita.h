#ifndef CARDATTIVITA_H
#define CARDATTIVITA_H

#include <QFrame>
#include "Attivita.h"
#include "AttivitaVisitor.h"

class QMouseEvent;

namespace Ui {
class CardAttivita;
}

// CardAttivita implementa AttivitaVisitor per configurare gli elementi grafici
// specifici di ogni tipo (colore accento, checkbox/pulsante azione) tramite
// double dispatch, senza chiedere al modello il proprio tipo
class CardAttivita : public QFrame, public AttivitaVisitor
{
    Q_OBJECT

public:
    explicit CardAttivita(Attivita* a, QWidget *parent = nullptr);
    ~CardAttivita();

    Attivita* getAttivita() const;
    void aggiornaVisualizzazione();

    void visit(Scadenza& s) override;
    void visit(Impegno& i) override;
    void visit(Appuntamento& a) override;

signals:
    void attivitaModificata(Attivita* a);
    void cliccata(Attivita* a);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onBtnAzioneClicked();
    void onChkCompletataToggled(bool checked);

private:
    Ui::CardAttivita *ui;
    Attivita* attivita;

    void impostaStileBordo(const QString& coloreAccento);
};

#endif // CARDATTIVITA_H
