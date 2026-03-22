#include "Scadenza.h"

//costruttori
Scadenza::Scadenza(): Attivita::Attivita(), done(false), hour(QTime::currentTime()) {};
Scadenza::Scadenza(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const QTime& h,bool f) : Attivita::Attivita(t, d, c, da, i), done(f), hour(h) {};

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

QString Scadenza::getIconPath() const {
    return ":/icons/icons/IconaScadenzaSVG.svg";
};

QString Scadenza::getType() const {
    return "Scadenza";
};

QString Scadenza::getSummary() const {     //restituisce "Finire entro: data ora"
    QDateTime istante(getDate(), getHour());
    QString show = istante.toString("dd/MM/yyyy hh:mm");
    return "Finire entro: "+show;
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
