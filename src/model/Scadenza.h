#ifndef SCADENZA_H
#define SCADENZA_H

#include "Attivita.h"
class Scadenza: public Attivita {
private:
    bool done;
    QTime hour;
public:
    //big 3
    Scadenza();
    Scadenza(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const periodicityTipe& per, const QDate& end, const QTime& h,bool f=false);
    ~Scadenza() = default;

    //metodi overridati
    Attivita* clone() const override;
    bool isUrgent() const override;
    QString getIconPath() const override;
    QString getType() const override;
    QString getSummary() const override;
    void performAction() override;
    QDate calcolaProssimaData() const override;

    //getter e setter specifici
    bool isDone() const;
    void setDone(bool status);
    QTime getHour() const;
    void setHour(const QTime& newhour);

    //salvataggio in json
    QJsonObject toJson() const override;
};

#endif // SCADENZA_H
