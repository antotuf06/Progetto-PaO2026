#ifndef ATTIVITA_H
#define ATTIVITA_H
#include <QString>
#include <QDate>
#include <QTime>
#include <iostream>
class Attivita {
private:
    QString title;
    QString descr;
    QString categ;
    QDate date;
    unsigned int id; //per salvare in json
public:
    Attivita();
    virtual ~Attivita();

    virtual bool isUrgent() const =0;

    QString getTitle();
    QString getDescr();
    QString getCateg();
    QDate getDate();
    unsigned int getId();

    void setTitle(const QString& newtitle);
    void setDescr(const QString& newdescr);
    void setCateg(const QString& newcateg);
    void setDate(const QDate& newdate);

    virtual QJsonObject toJson() const = 0;
};

#endif // ATTIVITA_H
