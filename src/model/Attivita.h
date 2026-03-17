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
    Attivita(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i);
    virtual ~Attivita();

    //metodi virtuali per logica e comportamento
    virtual bool isUrgent() const =0; //restituisce true o false con condizioni diverse nelle classi derivate
    virtual void performAction()= 0; // fa un'azione specifica in base al tipo di attività

    //metodi per visualizzazione e GUI
    virtual QString getIconPath() const = 0; //per trovare il path dell'icona specifica per ogni classe
    virtual QString getType() const = 0; //restituisce il tipo di attività
    virtual QString getSummary() const = 0; // restituisce le informazioni base in base al tipo di Attività


    QString getTitle() const;
    QString getDescr() const;
    QString getCateg() const;
    QDate getDate() const;
    unsigned int getId() const;

    void setTitle(const QString& newtitle);
    void setDescr(const QString& newdescr);
    void setCateg(const QString& newcateg);
    void setDate(const QDate& newdate);

};

#endif // ATTIVITA_H
