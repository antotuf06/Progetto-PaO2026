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
    // explicit per evitare conversioni implicite da QString a Manager
    explicit Manager(const QString& file = "database.json");

    ~Manager();


    //add o remove dal vettore
    // aggiunge un'attività
    void addAttivita(Attivita* a);

    // aggiunge un'attività e le sue ricorrenze
    void addPeriodicity(Attivita* primo);

    // rimuove l'attività all'indice specificato
    void removeAttivita(int index);

    // sostituisce un'attività all'indice dato con una modificata
    void updateAttivita(int index, Attivita* nuova);

    // svuota completamente la lista
    void clear();


    //getter
    // ritorna una reference alla lista completa (solo lettura)
    const QVector<Attivita*>& getLista() const;

    // restituisce una singola attività
    Attivita* getAttivita(int index) const;

    // ritorna il numero di attività presenti
    int getSize() const;


    //persistenza
    // salva il vettore attuale nel file specificato dal costruttore
    void salvaFile() const;

    // carica dal file JSON e popola il vettore
    void caricaFile();
};

#endif // MANAGER_H