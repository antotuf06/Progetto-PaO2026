#include "Impegno.h"


//costruttori
Impegno::Impegno() : Attivita::Attivita(), start(QTime::currentTime()), finish(QTime::currentTime().addSecs(3600)) {};
Impegno::Impegno(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const periodicityTipe& per, const QDate& end, const QTime& s, const QTime& f) : Attivita::Attivita(t, d, c, da, i, per, end), start(s), finish(f) {};



//metodi overridati

bool Impegno::isUrgent() const{                    //restituisce true se mancano meno di 2 ore all'ora di inizio, o se l'impegno è in corso
    QDateTime adesso = QDateTime::currentDateTime();
    QDateTime inizio(getDate(), getStart());
    QDateTime fine(getDate(), getFinish());

    if (adesso >= fine) {           //l'impegno è già terminato: non è più urgente
        return false;
    }

    qint64 secondiMancanti = adesso.secsTo(inizio);

    if (secondiMancanti <= 3600 * 2) {
        return true;
    }

    return false;
};


QString Impegno::getIconPath() const {
    return ":/icons/IconaImpegnoSVG.svg";
};

QString Impegno::getType() const {return "Impegno";};

QString Impegno::getSummary() const {           //ritorna stringa del tipo "dataInizio oraInizio-oraFine"
    QDateTime inizio(getDate(), getStart());
    QTime fine = getFinish();
    QString show1 = inizio.toString("dd/MM/yyyy hh:mm");
    QString show2 = fine.toString("hh:mm");
    return  show1+"-"+show2 ;
};


void Impegno::performAction() {          // Aggiunge 30 minuti a entrambi gli orari
    setStart(getStart().addMSecs(30 * 60 * 1000));
    setFinish(getFinish().addMSecs(30 * 60 * 1000));

    qDebug() << "Impegno posticipato di 30 minuti.";
}

//getter e setter

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

Attivita* Impegno::clone() const {
    return new Impegno(*this);
}

QDate Impegno::calcolaProssimaData() const {
    if (getPeriodicity() == Nessuna) return QDate(); // Data non valida

    QDate nuovaData = getDate();
    if (getPeriodicity() == Giornaliera) return nuovaData.addDays(1);
    if (getPeriodicity() == Settimanale) return nuovaData.addDays(7);
    if (getPeriodicity() == Mensile) return nuovaData.addMonths(1);

    return QDate();
}

//salvataggio in json
QJsonObject Impegno::toJson() const{
    QJsonObject json = baseJson(); // campi comuni
    json["tipo"] = "Impegno";   // Identificatore
    json["start"] = start.toString(Qt::ISODate);      //campi specifici
    json["finish"] = finish.toString(Qt::ISODate);
    return json;
}