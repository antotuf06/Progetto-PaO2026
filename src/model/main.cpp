#include <iostream>
#include <QDate>
#include <QTime>
#include <QDebug>
#include "Impegno.h"

int main() {
    // 1. Creazione di un appuntamento per OGGI
    QDate oggi = QDate::currentDate();
    QTime ora = QTime::currentTime().addSecs(3600); // Tra un'ora

    Impegno* imp = new Impegno(
        "Riunione Progetto",
        "Discussione architettura",
        "Università",
        oggi,
        1,
        ora,
        ora.addSecs(3600)
        );

    // 2. Test dei Getter
    qDebug() << "Titolo:" << imp->getTitle();
    qDebug() << "Inizio:" << imp->getStart().toString();
    qDebug() << "Fine:" << imp->getFinish().toString();

    // 3. Test Polimorfismo (isUrgent)
    if (imp->isUrgent()) {
        qDebug() << "TEST SUPERATO: L'appuntamento di oggi è urgente.";
    } else {
        qDebug() << "TEST FALLITO: Dovrebbe essere urgente.";
    }

    // 4. Test getSummary
    qDebug() << "Summary:" << imp->getSummary();

    imp->performAction();
    qDebug() << "Titolo:" << imp->getTitle();
    qDebug() << "Inizio:" << imp->getStart().toString();
    qDebug() << "Fine:" << imp->getFinish().toString();

    qDebug() << "Summary:" << imp->getSummary();



    delete imp;
    return 0;
}