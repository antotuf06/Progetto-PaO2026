#ifndef DETTAGLIATTIVITA_H
#define DETTAGLIATTIVITA_H

#include <QWidget>
#include "Attivita.h"

namespace Ui {
class DettagliAttivita;
}

class DettagliAttivita : public QWidget
{
    Q_OBJECT

public:
    explicit DettagliAttivita(Attivita* a, QWidget *parent = nullptr);
    ~DettagliAttivita();

signals:
    void richiestaModifica(Attivita* a);
    void richiestaEliminazione(Attivita* a);

private:
    Ui::DettagliAttivita *ui;
    Attivita* attivita;

    void aggiungiRiga(const QString& etichetta, const QString& valore);
    QString periodicitaTesto(const Attivita* a) const;
};

#endif // DETTAGLIATTIVITA_H
