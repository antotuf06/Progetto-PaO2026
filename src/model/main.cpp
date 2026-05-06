#include <QCoreApplication>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QVector>

// Includi i tuoi file (assicurati che i nomi coincidano)
#include "Appuntamento.h"
#include "Impegno.h"
#include "Scadenza.h"

// --- FUNZIONE DI SUPPORTO (Simula il Manager) ---
// Questa è la logica che in futuro sposterai in Manager::aggiungiConRicorrenza
void generaRicorrenze(QVector<Attivita*>& lista, Attivita* primo) {
    // 1. Aggiungiamo il primo elemento
    lista.append(primo);

    // 2. Generiamo le copie se l'attività è ricorrente e ha una scadenza
    if (primo->getPeriodicity() != Nessuna && primo->getEndPeriod().isValid()) {
        QDate prossima = primo->calcolaProssimaData();
        Attivita* ultimoCreato = primo;

        while (prossima.isValid() && prossima <= primo->getEndPeriod()) {
            Attivita* copia = ultimoCreato->clone();
            copia->setDate(prossima);

            lista.append(copia);

            ultimoCreato = copia;
            prossima = ultimoCreato->calcolaProssimaData();
        }
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qDebug() << "--- INIZIO TEST POLIMORFISMO E RICORRENZE ---";

    // Questo vettore sostituisce temporaneamente il Manager
    QVector<Attivita*> listaAttivita;

    // Date di test
    QDate oggi = QDate::currentDate();
    QDate fineMese = oggi.addMonths(1);
    QDate fraTreGiorni = oggi.addDays(2);

    // --- TEST 1: SCADENZA (Non ricorrente) ---
    qDebug() << "\nCreazione Scadenza (Singola)...";
    // N.B: Sostituisci il numero 1 con l'enum della tua Priority se necessario
    Scadenza* bolletta = new Scadenza("Bolletta Luce", "Pagamento bimestrale",
                                      "Utenze", oggi, 1, false);
    generaRicorrenze(listaAttivita, bolletta);


    // --- TEST 2: APPUNTAMENTO (Settimanale) ---
    qDebug() << "Creazione Appuntamento (Settimanale fino a fine mese)...";
    Appuntamento* calcetto = new Appuntamento("Partita Calcetto", "Solito gruppo",
                                              "Sport", oggi, 2,
                                              "Campetti", QTime(19, 0),
                                              Settimanale, fineMese);
    generaRicorrenze(listaAttivita, calcetto);


    // --- TEST 3: IMPEGNO (Giornaliero) ---
    qDebug() << "Creazione Impegno (Giornaliero per 3 giorni)...";
    Impegno* medicina = new Impegno("Prendere Antibiotico", "Dopo i pasti",
                                    "Salute", oggi, 3,
                                    QTime(8, 0), QTime(8, 5),
                                    Giornaliera, fraTreGiorni);
    generaRicorrenze(listaAttivita, medicina);


    // --- STAMPA DEI RISULTATI ---
    qDebug() << "\n--- LISTA COMPLETA DELLE ATTIVITA' ---";
    qDebug() << "Totale oggetti in memoria:" << listaAttivita.size();

    for (int i = 0; i < listaAttivita.size(); ++i) {
        Attivita* att = listaAttivita[i];

        // Verifica con i getter effettivi che hai definito in Attivita
        qDebug() << "ID:" << i
                 << "| Data:" << att->getDate().toString("dd/MM/yyyy")
                 << "| Titolo:" << att->getTitle();
    }

    // --- PULIZIA DELLA MEMORIA ---
    // Dato che non c'è il distruttore del Manager, dobbiamo liberare la RAM a mano qui
    qDebug() << "\nPulizia memoria in corso...";
    qDeleteAll(listaAttivita);
    listaAttivita.clear();

    qDebug() << "Test completato.";

    return 0;
}