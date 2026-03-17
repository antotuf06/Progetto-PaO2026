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
    Scadenza(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const QTime& h,bool f=false);
    ~Scadenza() = default;

    //metodi overridati
    bool isUrgent() const override;
    QString getIconPath() const override;
    QString getType() const override;
    QString getSummary() const override;
    void performAction() override;

    //getter e setter specifici
    bool isDone() const;
    void setDone(bool status);
    QTime getHour() const;
    void setHour(const QTime& newhour);
};

#endif // SCADENZA_H
