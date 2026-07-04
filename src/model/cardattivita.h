#ifndef CARDATTIVITA_H
#define CARDATTIVITA_H

#include <QFrame>
#include "Attivita.h"

namespace Ui {
class CardAttivita;
}

class CardAttivita : public QFrame
{
    Q_OBJECT

public:
    explicit CardAttivita(Attivita* a, QWidget *parent = nullptr);
    ~CardAttivita();

    Attivita* getAttivita() const;
    void aggiornaVisualizzazione();

signals:
    void attivitaModificata(Attivita* a);

private slots:
    void onBtnAzioneClicked();
    void onChkCompletataToggled(bool checked);

private:
    Ui::CardAttivita *ui;
    Attivita* attivita;
};

#endif // CARDATTIVITA_H
