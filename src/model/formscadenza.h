#ifndef FORMSCADENZA_H
#define FORMSCADENZA_H

#include <QWidget>
#include "Scadenza.h"

namespace Ui {
class FormScadenza;
}

class FormScadenza : public QWidget
{
    Q_OBJECT

public:
    explicit FormScadenza(QWidget *parent = nullptr);
    ~FormScadenza();

    // costruisce una Scadenza con i dati correnti del form
    Scadenza* creaAttivita(unsigned int id) const;

signals:
    void annulla();
    void salva();

private:
    Ui::FormScadenza *ui;
};

#endif // FORMSCADENZA_H
