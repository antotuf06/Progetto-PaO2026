#ifndef APPUNTAMENTO_H
#define APPUNTAMENTO_H
#include "Attivita.h"

class Appuntamento: public Attivita {

private:
    QString place;
    QTime hour;

public:

    //costruttori e distruttore
    Appuntamento();
    Appuntamento(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int& i, const QString& p, const QTime& h);
    virtual ~Appuntamento() =default;

    //getter e setter
    QString getPlace() const;
    QTime getHour() const;

    void setPlace(const QString& newplace);
    void setHour(const QTime& newhour);

    //metodi overridati
    bool isUrgent() const override;
    QJsonObject toJson() const override;
};

#endif // APPUNTAMENTO_H
