#ifndef FORMSCADENZA_H
#define FORMSCADENZA_H

#include <QWidget>

namespace Ui {
class FormScadenza;
}

class FormScadenza : public QWidget
{
    Q_OBJECT

public:
    explicit FormScadenza(QWidget *parent = nullptr);
    ~FormScadenza();

private:
    Ui::FormScadenza *ui;
};

#endif // FORMSCADENZA_H
