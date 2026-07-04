#include "Manager.h"
#include "Appuntamento.h"
#include "Impegno.h"
#include "Scadenza.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

// --- COSTRUTTORE E DISTRUTTORE ---

Manager::Manager(const QString& file) : fileName(file) {
}

Manager::~Manager() {
    clear(); //libera la memoria prima di distruggere l'oggetto
}

//gestione

void Manager::addAttivita(Attivita* a) {
    if (a) {
        listaAttivita.append(a);
    }
}

void Manager::addPeriodicity(Attivita* primo, bool salva) {
    if (!primo) return;

    //aggiungo sempre il primo elemento
    addAttivita(primo);

    //controllo se ha una ricorrenza attiva e una data di fine valida
    if (primo->getPeriodicity() != Nessuna && primo->getEndPeriod().isValid()) {

        QDate prossima = primo->calcolaProssimaData();   //chiamata polimorfa
        Attivita* ultimoCreato = primo;

        while (prossima.isValid() && prossima <= primo->getEndPeriod()) {  //crea una copia dell'ultima attività aggiunta e le assegna la data successiva, finchè non supera la data di fine periodo

            Attivita* copia = ultimoCreato->clone();   //chiamata polimorfa
            copia->setDate(prossima);

            addAttivita(copia);

            //preparo per il ciclo successivo
            ultimoCreato = copia;
            prossima = ultimoCreato->calcolaProssimaData();
        }
    }

    if (salva) salvaFile();
}

void Manager::removeAttivita(int index) {
    if (index >= 0 && index < listaAttivita.size()) {
        delete listaAttivita[index];  // Libera la memoria RAM
        listaAttivita.removeAt(index); // Rimuove il puntatore dal vettore
        salvaFile();                   // Aggiorna il file JSON
    }
}

void Manager::updateAttivita(int index, Attivita* nuova) {
    if (index >= 0 && index < listaAttivita.size() && nuova != nullptr) {
        delete listaAttivita[index];
        listaAttivita[index] = nuova;
        salvaFile();
    }
}

void Manager::clear() {
    qDeleteAll(listaAttivita);
    listaAttivita.clear();     //svuota il vettore
}


//accesso ai dati

const QVector<Attivita*>& Manager::getLista() const {
    return listaAttivita;
}

Attivita* Manager::getAttivita(int index) const {
    if (index >= 0 && index < listaAttivita.size()) {
        return listaAttivita[index];
    }
    return nullptr;
}

int Manager::getSize() const {
    return listaAttivita.size();
}


//persistenza

void Manager::salvaFile() const {
    QJsonArray jsonArray;

    for (Attivita* a : listaAttivita) {
        jsonArray.append(a->toJson());    //chiamata polimorfa
    }

    QJsonDocument doc(jsonArray);
    QFile file(fileName);

    //scrive su file sovrascrivendo il contenuto precedente
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "Errore: impossibile aprire il file per il salvataggio.";
    }
}

void Manager::caricaFile() {
    //svuota la lista in memoria per non duplicare i dati se carichiamo 2 volte
    clear();
    caricaDaFile(fileName);
}

void Manager::importaFile(const QString& file) {
    //non svuota la lista: le attivita' importate si aggiungono a quelle gia' create in sessione
    caricaDaFile(file);
}

void Manager::caricaDaFile(const QString& file) {
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "File non trovato o impossibile da leggere.";
        return;
    }

    //legge tutto il file JSON e lo trasforma in un Array
    QByteArray data = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    const QJsonArray jsonArray = doc.array();

    //ricostruisce tutti gli oggetti
    for (const auto& val : jsonArray) {
        QJsonObject obj = val.toObject();
        QString tipo = obj["tipo"].toString();

        //dati comuni
        QString t = obj["titolo"].toString();
        QString d = obj["descrizione"].toString();
        QString c = obj["categoria"].toString();
        QDate da = QDate::fromString(obj["data"].toString(), Qt::ISODate);
        unsigned int i = static_cast<unsigned int>(obj["id"].toInt());

        periodicityTipe per = static_cast<periodicityTipe>(obj["ricorrenza"].toInt());

        QDate end;
        QString endStr = obj["fineRicorrenza"].toString();
        if (!endStr.isEmpty()) {
            end = QDate::fromString(endStr, Qt::ISODate);
        }

        //dati specifici
        if (tipo == "Appuntamento") {
            QString p = obj["luogo"].toString();
            QTime h = QTime::fromString(obj["ora"].toString(), Qt::ISODate);

            addAttivita(new Appuntamento(t, d, c, da, i, per, end, p, h));

        } else if (tipo == "Impegno") {
            QTime s = QTime::fromString(obj["start"].toString(), Qt::ISODate);
            QTime f2 = QTime::fromString(obj["finish"].toString(), Qt::ISODate);

            addAttivita(new Impegno(t, d, c, da, i, per, end, s, f2));

        } else if (tipo == "Scadenza") {
            bool done = obj["done"].toBool();
            QTime h = QTime::fromString(obj["ora"].toString(), Qt::ISODate);

            addAttivita(new Scadenza(t, d, c, da, i, per, end, h, done));
        }
    }
}