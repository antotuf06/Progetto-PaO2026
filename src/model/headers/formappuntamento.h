#ifndef FORMAPPUNTAMENTO_H
#define FORMAPPUNTAMENTO_H

#include <QWidget>
#include "Appuntamento.h"

namespace Ui {
class FormAppuntamento;
}

class FormAppuntamento : public QWidget
{
    Q_OBJECT

public:
    explicit FormAppuntamento(QWidget *parent = nullptr);
    ~FormAppuntamento();

    // costruisce un Appuntamento con i dati correnti del form
    Appuntamento* creaAttivita(unsigned int id) const;

    // precompila il form con i dati di un Appuntamento esistente (modalita' modifica)
    void caricaDati(Appuntamento* a);

signals:
    void annulla();
    void salva();

private:
    Ui::FormAppuntamento *ui;
};

#endif // FORMAPPUNTAMENTO_H
