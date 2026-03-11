#ifndef SCADENZA_H
#define SCADENZA_H

#include "Attivita.h"

class Scadenza: public Attivita {
private:
    bool done;
public:
    //big 3
    Scadenza();
    Scadenza(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int& i, bool f=false);
    ~Scadenza() = default;

    //metodo overridato
    bool isUrgent() const override;
    QJsonObject toJson() const override;

    //getter e setter specifici
    bool isDone() const;
    void setDone(bool status);
};

#endif // SCADENZA_H
