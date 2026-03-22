#include <iostream>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <vector>

#include "Scadenza.h"
#include "Appuntamento.h"
#include "Impegno.h"

void testAttivita(Attivita* a) {
    qDebug() << "\n--- TEST CLASSE:" << a->getType() << "---";

    // 1. Test Costruttore e Restituzione dati iniziali
    qDebug() << "[Dati Iniziali]";
    qDebug() << "ID:" << a->getId();
    qDebug() << "Titolo:" << a->getTitle();
    qDebug() << "Descrizione:" << a->getDescr();
    qDebug() << "Categoria:" << a->getCateg();
    qDebug() << "Data:" << a->getDate().toString("dd/MM/yyyy");

    // 2. Test getIconPath
    QString path = a->getIconPath();
    if (QFile::exists(path)) {
        qDebug() << "Icona trovata:" << path;
    } else {
        qDebug() << "Errore: Icona NON trovata in:" << path;
    }

    // 3. Test getSummary
    qDebug() << "Summary pre-azione:" << a->getSummary();

    // 4. Test performAction
    qDebug() << "Eseguo performAction()...";
    a->performAction();

    // 5. Restituzione dati dopo la modifica
    qDebug() << "[Dati Post-Modifica]";
    qDebug() << "Summary post-azione:" << a->getSummary();

    // Verifica specifica per Scadenza (se è stata segnata come 'done')
    Scadenza* s = dynamic_cast<Scadenza*>(a);
    if (s) {
        qDebug() << "Stato completamento:" << (s->isDone() ? "FATTO" : "DA FARE");
    }

    qDebug() << "---------------------------------------";
}

int main() {
    QDate oggi = QDate::currentDate();
    QTime ora = QTime::currentTime();

    // Creazione istanze delle tre classi derivate
    Scadenza* scadenza = new Scadenza(
        "Comprare latte",
        "Prendere quello intero",
        "Spesa",
        oggi,
        101,
        QTime(20, 0),
        false
        );

    Appuntamento* appuntamento = new Appuntamento(
        "Dentista",
        "Pulizia annuale",
        "Salute",
        oggi.addDays(1),
        102,
        "Via Roma 15",
        QTime(15, 30)
        );

    Impegno* impegno = new Impegno(
        "Studio PAO",
        "Preparazione test main",
        "Università",
        oggi,
        103,
        ora,
        ora.addSecs(3600)
        );

    // Esecuzione dei test tramite polimorfismo
    testAttivita(scadenza);
    testAttivita(appuntamento);
    testAttivita(impegno);

    // Pulizia
    delete scadenza;
    delete appuntamento;
    delete impegno;

    return 0;
}