#include "Scadenza.h"

//costruttori
Scadenza::Scadenza(): Attivita::Attivita(), done(false), hour(QTime::currentTime()) {};
Scadenza::Scadenza(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const periodicityTipe& per, const QDate& end, const QTime& h,bool f) : Attivita::Attivita(t, d, c, da, i, per, end), done(f), hour(h) {};

//metodi overridati
bool Scadenza::isUrgent() const {           //è urgente se mancano meno di 24 ore o se la scadenza è già passata
    QDateTime adesso = QDateTime::currentDateTime();
    QDateTime scadenza(getDate(), getHour());
    qint64 secondiMancanti = adesso.secsTo(scadenza);

    if (secondiMancanti <= 3600 * 24) {
        return true;
    }

    return false;

};

//getter e setter

QString Scadenza::getIconPath() const {
    return ":/icons/IconaScadenzaSVG.svg";
};

QString Scadenza::getType() const {
    return "Scadenza";
};

QString Scadenza::getSummary() const {     //restituisce "Finire entro: data ora" o "Attività completata"
    if(!done){
        QDateTime istante(getDate(), getHour());
        QString show = istante.toString("dd/MM/yyyy hh:mm");
        return "Finire entro: "+show;
    } else {
        return "Attività completata";
    }
};
void Scadenza::performAction() {      //inverte lo stato della scadenza (fatto o non fatto)
    done=!done;
};

//getter e setter
bool Scadenza::isDone() const {
    return done;
};

void Scadenza::setDone(bool status){
    done=status;
};

QTime Scadenza::getHour() const{
    return hour;
};

void Scadenza::setHour(const QTime& newhour){
    if (newhour.isNull()) return;
    hour=newhour;
};

Attivita* Scadenza::clone() const {
    return new Scadenza(*this);
}

QDate Scadenza::calcolaProssimaData() const {  //restituisce un QDate nullo, per evidenziare che non c'è ricorrenza per le scadenze
    return QDate();
}

//salvataggio in json
QJsonObject Scadenza::toJson() const {
    QJsonObject json = baseJson(); // campi comuni
    json["tipo"] = "Scadenza";   // Identificatore
    json["done"] = done;
    json["ora"] = hour.toString(Qt::ISODate);
    return json;
}