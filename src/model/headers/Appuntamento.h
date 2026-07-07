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
    Appuntamento(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const periodicityTipe& per, const QDate& end, const QString& p, const QTime& h);
    virtual ~Appuntamento() =default;
    Attivita* clone() const override;

    //getter e setter
    QString getPlace() const;
    QTime getHour() const;

    void setPlace(const QString& newplace);
    void setHour(const QTime& newhour);

    //metodi overridati
    bool isUrgent() const override;
    QString getIconPath() const override;
    QString getType() const override;
    QString getSummary() const override;
    void performAction() override;
    QDate calcolaProssimaData() const override;

    //salvataggio json
    QJsonObject toJson() const override;
};

#endif // APPUNTAMENTO_H
