#include "Impegno.h"


//costruttori
Impegno::Impegno() : Attivita::Attivita(), start(QTime::currentTime()), finish(QTime::currentTime().addSecs(3600)) {};
Impegno::Impegno(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const QTime& s, const QTime& f) : Attivita::Attivita(t, d, c, da, i), start(s), finish(f) {};



//metodi overridati

bool Impegno::isUrgent() const{                    //restituisce true se mancano meno di 2 ore all'ora di inizio
    QDateTime adesso = QDateTime::currentDateTime();
    QDateTime inizio(getDate(), getStart());
    qint64 secondiMancanti = adesso.secsTo(inizio);

    if (secondiMancanti <= 3600 * 2) {
        return true;
    }

    return false;
};


QString Impegno::getIconPath() const {};

QString Impegno::getType() const {return "Impegno";};

QString Impegno::getSummary() const {           //ritorna stringa del tipo "dataInizio oraInizio-oraFine"
    QDateTime inizio(getDate(), getStart());
    QTime fine = getFinish();
    QString show1 = inizio.toString("dd/MM/yyyy hh:mm");
    QString show2 = fine. toString("hh:mm");
    return  show1+"-"+show2 ;
};


void Impegno::performAction() {
    // Aggiunge 30 minuti a entrambi gli orari
    setStart(getStart().addMSecs(30 * 60 * 1000));
    setFinish(getFinish().addMSecs(30 * 60 * 1000));

    qDebug() << "Impegno posticipato di 30 minuti.";
}

//getter

QTime Impegno::getStart() const {return start;};

QTime Impegno::getFinish() const {return finish;};

void Impegno::setStart(const QTime& newstart) {
    if (newstart.isNull()) return;
    start=newstart;
};

void Impegno::setFinish(const QTime& newfinish) {
    if(newfinish.isNull()) return;
    finish=newfinish;
};