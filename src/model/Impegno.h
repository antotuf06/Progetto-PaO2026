#ifndef IMPEGNO_H
#define IMPEGNO_H
#include "Attivita.h"


class Impegno: public Attivita {
private:
    QTime start;
    QTime finish;  //orari di inizio e fine

public:

    //costruttori e distruttore
    Impegno();
    Impegno(const QString& t, const QString& d, const QString& c, const QDate& da, unsigned int i, const periodicityTipe& per, const QDate& end, const QTime& s, const QTime& f);
    virtual ~Impegno() = default;

    //metodi overridati
    Attivita* clone() const override;
    bool isUrgent() const  override;
    QString getIconPath() const override;
    QString getType() const override;
    QString getSummary() const override;
    void performAction() override;
    QDate calcolaProssimaData() const override;

    //getter e setter
    QTime getStart() const;
    QTime getFinish() const;

    void setStart(const QTime& newstart);
    void setFinish(const QTime& newfinish);

    //salvataggio in json
    QJsonObject toJson() const;

};
#endif // IMPEGNO_H
