#include <QCoreApplication>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QDir>
#include <QFileInfo>

// Includiamo il Manager e le classi derivate
#include "Manager.h"
#include "Appuntamento.h"
#include "Impegno.h"
#include "Scadenza.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Diamo un nome a un file di test per non sporcare il database reale
    QString fileTest = "test_database.json";
    qDebug() << "-> La cartella di lavoro attuale è:" << QDir::currentPath();
    qDebug() << "-> Il percorso esatto del file sarà:" << QFileInfo(fileTest).absoluteFilePath();

    qDebug() << "=========================================";
    qDebug() << "  FASE 1: CREAZIONE E SALVATAGGIO";
    qDebug() << "=========================================";

    // Creiamo il primo manager che simula l'utente che inserisce i dati
    Manager managerScrittura(fileTest);
    managerScrittura.clear(); // Partiamo da una situazione pulita

    QDate oggi = QDate::currentDate();
    QDate fineMese = oggi.addMonths(1);
    QDate fraTreGiorni = oggi.addDays(2);

    // 1. Scadenza (Singola)
    Scadenza* bolletta = new Scadenza(
        "Bolletta Luce", "Scadenza", "Utenze", oggi, 1, Nessuna, QDate(), QTime(12, 0), false
        );
    managerScrittura.addPeriodicity(bolletta);

    // 2. Appuntamento (Settimanale)
    Appuntamento* calcetto = new Appuntamento(
        "Partita Calcetto", "Solito gruppo", "Sport", oggi, 2, Settimanale, fineMese, "Campetti", QTime(19, 0)
        );
    managerScrittura.addPeriodicity(calcetto);

    // 3. Impegno (Giornaliero)
    Impegno* medicina = new Impegno(
        "Antibiotico", "Dopo i pasti", "Salute", oggi, 3, Giornaliera, fraTreGiorni, QTime(8, 0), QTime(8, 5)
        );
    managerScrittura.addPeriodicity(medicina);

    qDebug() << "-> Inserite attività iniziali.";
    qDebug() << "-> Totale oggetti in memoria dopo la generazione delle ricorrenze:" << managerScrittura.getSize();

    // Il salvataggio avviene in automatico dentro aggiungiConRicorrenza(),
    // ma lo richiamiamo per sicurezza.
    managerScrittura.salvaFile();
    qDebug() << "-> Dati salvati in" << fileTest << "\n";



    qDebug() << "=========================================";
    qDebug() << "  FASE 2: RICARICAMENTO DAL FILE JSON";
    qDebug() << "=========================================";

    // Creiamo un SECONDO manager totalmente slegato dal primo.
    // Questo simula l'utente che ha chiuso e riaperto l'applicazione.
    Manager managerLettura(fileTest);
    managerLettura.caricaFile();

    qDebug() << "-> Oggetti ricaricati con successo:" << managerLettura.getSize();

    // Verifichiamo se il polimorfismo e i dati si sono conservati
    for (int i = 0; i < managerLettura.getSize(); ++i) {
        Attivita* att = managerLettura.getAttivita(i);

        qDebug().noquote() << "\n[" << att->getType() << "]" << att->getDate().toString("dd/MM/yyyy") << "-" << att->getTitle();
        qDebug().noquote() << "   Dettagli:" << att->getSummary();
    }

    qDebug() << "\n=========================================";
    qDebug() << "  TEST COMPLETATO";
    qDebug() << "=========================================";

    return 0;
}