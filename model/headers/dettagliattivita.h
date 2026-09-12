#ifndef DETTAGLIATTIVITA_H
#define DETTAGLIATTIVITA_H

#include <QWidget>
#include "Attivita.h"
#include "AttivitaVisitor.h"

namespace Ui {
class DettagliAttivita;
}

// DettagliAttivita implementa AttivitaVisitor per aggiungere le righe di
// dettaglio specifiche di ogni tipo tramite double dispatch, senza
// controllare esplicitamente il tipo dell'attivita'
class DettagliAttivita : public QWidget, public AttivitaVisitor
{
    Q_OBJECT

public:
    explicit DettagliAttivita(Attivita* a, QWidget *parent = nullptr);
    ~DettagliAttivita();

    void visit(Scadenza& s) override;
    void visit(Impegno& i) override;
    void visit(Appuntamento& a) override;

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
