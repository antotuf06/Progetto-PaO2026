#include "Appuntamento.h"

//coatruttori
Appuntamento::Appuntamento() : Attivita::Attivita(), place(""), hour(QTime::currentTime()) {};
Appuntamento::Appuntamento(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const QString& p, const QTime& h): Attivita::Attivita(t, d, c, da, i), place(p), hour(h) {};

//getter e setter
QTime Appuntamento::getHour() const {return hour;}
QString Appuntamento::getPlace() const {return place;}
QString Appuntamento::getType() const {return "Appuntamento";}

void Appuntamento::setPlace(const QString& newplace){
    place=newplace;
}

void Appuntamento::setHour(const QTime& newhour){
    if (newhour.isNull()) return;
    hour=newhour;
}

bool Appuntamento::isUrgent() const {              //L'appuntamento è urgente se è nel giorno corrente, mancano meno di due ore o se la data è già passata
    QDate oggi=QDate::currentDate();
    QTime ora=QTime::currentTime();
    if (getDate() == oggi || ora.secsTo(getHour())<=7200 ) return true;
    if (getDate()<oggi) return true;
    return false;
};

QString Appuntamento::getIconPath() const {
    return ":/icons/icons/IconaAppuntamentoSVG.svg";
};


QString Appuntamento::getSummary() const  {         //restituisce un piccolo riassunto delle info dell'attività per facilitare la visualizzazione (es. "12/07/2027 11:30, via Roma 27")
    QDateTime istante(getDate(), getHour());
    QString show = istante.toString("dd/MM/yyyy hh:mm");
    return  show +", "+ getPlace();
};

void Appuntamento::performAction() {
    setHour(getHour().addSecs(3600));
    qDebug() << "Appuntamento posticipato di un'ora";
} ;

QJsonObject Appuntamento::toJson() const {
    QJsonObject json = baseJson(); // campi comuni
    json["tipo"] = "Appuntamento";   // tipo attività
    json["luogo"] = place;           // dati specifici
    json["ora"] = hour.toString(Qt::ISODate);
    return json;
}
