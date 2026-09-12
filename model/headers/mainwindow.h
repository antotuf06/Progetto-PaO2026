#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "Manager.h"
#include "Attivita.h"
#include "AttivitaVisitor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// MainWindow implementa AttivitaVisitor per aprire la schermata di modifica corretta
// (pattern Visitor: il dispatch al metodo visit() giusto avviene per polimorfismo
// tramite Attivita::accept, senza controllare esplicitamente il tipo dell'attivita')
class MainWindow : public QMainWindow, public AttivitaVisitor
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void visit(Scadenza& s) override;
    void visit(Impegno& i) override;
    void visit(Appuntamento& a) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onAttivitaModificata(Attivita* a);
    void onCreaImpegno();
    void onCreaAppuntamento();
    void onCreaScadenza();
    void onCreazioneAnnullata();
    void mostraDettagli(Attivita* a);
    void onModificaAttivita(Attivita* a);
    void onEliminaAttivita(Attivita* a);

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

    // apre il form di modifica per un'attivita' esistente, gestendo salvataggio e refresh della GUI;
    // e' un template perche' la logica e' identica per ogni coppia Form/tipo di Attivita' concreta
    template<typename FormT, typename ModelT>
    void apriFormModifica(ModelT* modello);
};
#endif // MAINWINDOW_H
