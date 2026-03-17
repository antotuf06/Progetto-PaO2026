#include <iostream>
#include <QDate>
#include <QTime>
#include <QDebug>
#include "Scadenza.h"

int main() {
    // 1. Creazione di un appuntamento per OGGI
    QDate domani = QDate::currentDate().addDays(1);
    QTime ora = QTime::currentTime().addSecs(3600);

    Scadenza* scad = new Scadenza(
        "Consegna Progetto",
        "Consegna progetto PaO",
        "Scadenza",
        domani,
        1,
        ora
        );

    // 2. Test dei Getter
    qDebug() << "Titolo:" << scad->getTitle();
    qDebug() << "Data:" << scad->getDate();
    qDebug() << "Ora:" << scad->getHour().toString();

    // 3. Test Polimorfismo (isUrgent)
    if (scad->isUrgent()) {
        qDebug() << "TEST SUPERATO: La scadenza è urgente.";
    } else {
        qDebug() << "TEST FALLITO: Dovrebbe essere urgente.";
    }

    // 4. Test getSummary
    qDebug() << "Summary:" << scad->getSummary();

    delete scad;
    return 0;
}