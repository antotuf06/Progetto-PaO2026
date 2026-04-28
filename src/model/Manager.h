#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>
#include <QString>
#include "Attivita.h"

class Manager {
private:
    QVector<Attivita*> listaAttivita;
    QString fileName;

public:
    // Costruttore: riceve il percorso del file JSON
    Manager(const QString& file = "database.json");

    // Distruttore: per evitare memory leaks
    ~Manager();


    void addAttivita(Attivita* a);

    // Rimuove l'attività all'indice specificato
    void removeAttivita(int index);

    // Permette di modificare un'attività esistente, che viene sostituita con una nuova attività ma con i dati modificati
    void updateAttivita(int index, Attivita* nuova);

    // Ritorna la lista per visualizzazione nella GUI
    const QVector<Attivita*>& getLista() const;

    // Ritorna il numero di attività presenti
    int getSize() const;


    //Metodi per la persistenza
    void salvaFile() const;
    void caricaFile();
};

#endif // MANAGER_H
