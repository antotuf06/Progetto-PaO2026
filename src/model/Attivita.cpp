#include "Attivita.h"
//big 3
Attivita::Attivita() : title("Nuova Attività"), descr(""), categ("Generale"), date(QDate::currentDate()), id(0), periodicity(Nessuna), endPeriod(QDate::currentDate()) {}

Attivita::Attivita(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const periodicityTipe& per, const QDate& end) : title(t), descr(d), categ(c), date(da), id(i), periodicity(per), endPeriod(end) {}

Attivita::~Attivita(){};

//getter
QString Attivita::getTitle() const {return title;}
QString Attivita::getDescr() const {return descr;}
QString Attivita::getCateg() const {return categ;}
QDate Attivita::getDate() const {return date;}
unsigned int Attivita::getId() const {return id;}
periodicityTipe Attivita::getPeriodicity() const {return periodicity;}
QDate Attivita::getEndPeriod() const {return endPeriod;}

//setter
void Attivita::setTitle(const QString& newtitle) {
    if (newtitle.isEmpty()) return;
    title=newtitle;
}

void Attivita::setDescr(const QString& newdescr) {
    descr=newdescr;
}

void Attivita::setCateg(const QString& newcateg) {
    if (newcateg.isEmpty()) return;
    categ=newcateg;
}

void Attivita::setDate(const QDate& newdate) {
    if(newdate.isNull()) return;
    date=newdate;
}

void Attivita::setPeriodicity(const periodicityTipe& newper) {
    periodicity=newper;
}

void Attivita::setEndPeriod(const QDate& newend){
    if(newend.isNull()) return;
    endPeriod=newend;
}

QJsonObject Attivita::baseJson() const {
    QJsonObject json;
    json["titolo"] = title;
    json["descrizione"] = descr;
    json["categoria"] = categ;
    json["data"] = date.toString(Qt::ISODate); //formato YYYY-MM-DD
    json["id"] = static_cast<int>(id);
    json["ricorrenza"] = static_cast<int>(periodicity);
    json["fineRicorrenza"] = endPeriod.isValid() ? endPeriod.toString(Qt::ISODate) : "";
    return json;
}