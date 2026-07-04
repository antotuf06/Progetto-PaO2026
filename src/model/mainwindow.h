#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "Manager.h"
#include "Attivita.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_page0_customContextMenuRequested(const QPoint &pos);
    void onAttivitaModificata(Attivita* a);
    void onCreaImpegno();
    void onCreaAppuntamento();
    void onCreaScadenza();
    void onCreazioneAnnullata();

private:
    Ui::MainWindow *ui;
    Manager* manager;
    QMenu* menuCreaAttivita;
    int colonneCorrenti;
    bool modalitaCompatta;

    void popolaGrigliaAttivita();
    void mostraFormCreazione(QWidget* form);
    void adattaLayout();
    int calcolaColonne() const;
    unsigned int prossimoId() const;
};
#endif // MAINWINDOW_H
