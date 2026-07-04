#ifndef FORMIMPEGNO_H
#define FORMIMPEGNO_H

#include <QWidget>
#include "Impegno.h"

namespace Ui {
class FormImpegno;
}

class FormImpegno : public QWidget
{
    Q_OBJECT

public:
    explicit FormImpegno(QWidget *parent = nullptr);
    ~FormImpegno();

    // costruisce un Impegno con i dati correnti del form
    Impegno* creaAttivita(unsigned int id) const;

signals:
    void annulla();
    void salva();

private:
    Ui::FormImpegno *ui;
};

#endif // FORMIMPEGNO_H
