#ifndef ATTIVITAVISITOR_H
#define ATTIVITAVISITOR_H

class Scadenza;
class Impegno;
class Appuntamento;

// interfaccia del pattern Visitor: permette di eseguire un'operazione specifica
// per ogni tipo concreto di Attivita' senza controllare il tipo a runtime
// (il doppio dispatch e' garantito da Attivita::accept, overridato in ogni classe derivata)
class AttivitaVisitor {
    public:
        virtual ~AttivitaVisitor() = default;
        virtual void visit(Scadenza& s) = 0;
        virtual void visit(Impegno& i) = 0;
        virtual void visit(Appuntamento& a) = 0;
};

#endif // ATTIVITAVISITOR_H
