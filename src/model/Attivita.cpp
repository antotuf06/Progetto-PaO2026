#include "Attivita.h"
//big 3
Attivita::Attivita() : title("Nuova Attività"), descr(""), categ("Generale"), date(QDate::currentDate()), id(0) {}

Attivita::Attivita(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i) : title(t), descr(d), categ(c), date(da), id(i) {}

Attivita::~Attivita(){};

//getter
QString Attivita::getTitle() const {return title;}
QString Attivita::getDescr() const {return descr;}
QString Attivita::getCateg() const {return categ;}
QDate Attivita::getDate() const {return date;}
unsigned int Attivita::getId() const {return id;}

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
