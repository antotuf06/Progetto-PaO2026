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

    // aggiunge un'attività e le sue ricorrenze; se salva e' false non scrive subito su file
    // (usato dalla GUI per rimandare il salvataggio al tasto "Salva")
    void addPeriodicity(Attivita* primo, bool salva = true);

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

    // carica dal file JSON e popola il vettore (sostituendo il contenuto attuale)
    void caricaFile();

    // importa le attivita' lette da un file esterno aggiungendole a quelle gia' presenti
    // in memoria, senza toccare il file di salvataggio predefinito
    void importaFile(const QString& file);

private:
    // legge un file JSON e aggiunge le attivita' trovate alla lista in memoria
    void caricaDaFile(const QString& file);
};

#endif // MANAGER_H