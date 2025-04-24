#include "formations.h"
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

// Default constructor
formations::formations()
    : idfor(0),
      formation(""),
      description(""),
      trainer(""),
      datef(QDate::currentDate()),
      time(0),
      prix(0.0) // Initialize as double
{
}


// Parameterized constructor
formations::formations(int idfor, QString &formation, QString &description, QString &trainer, QDate &datef, int &time, double &prix)
    : idfor(idfor),
      formation(formation),
      description(description),
      trainer(trainer),
      datef(datef),
      time(time),
      prix(prix)
{
}

// Getters
int formations::getIdfor() const
{
    return idfor;
}

QString formations::getFormation() const
{
    return formation;
}

QString formations::getDescription() const
{
    return description;
}

QString formations::getTrainer() const
{
    return trainer;
}

QDate formations::getDatef() const
{
    return datef;
}

int formations::getTime() const
{
    return time;
}

double formations::getPrix() const
{ // Change return type to double
    return prix;
}

// Setters
void formations::setIdfor(int idfor)
{
    this->idfor = idfor;
}

void formations::setFormation(const QString &formation)
{
    this->formation = formation;
}

void formations::setDescription(const QString &description)
{
    this->description = description;
}

void formations::setTrainer(const QString &trainer)
{
    this->trainer = trainer;
}

void formations::setDatef(const QDate &datef)
{
    this->datef = datef;
}

void formations::setTime(int time)
{
    this->time = time;
}

void formations::setPrix(double prix)
{ // Change parameter type to double
    this->prix = prix;
}

bool formations::ajoutforma()
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX) "
                  "VALUES (FORMATIONS_SEQ.NEXTVAL, :formation, :description, :trainer, :datef, :time, :prix)");

    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", datef);
    query.bindValue(":time", time);
    query.bindValue(":prix", prix);

    if (!query.exec())
    {
        qDebug() << "SQL Error:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel *formations::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        qDebug() << "Database not open!";
        model->setQuery(QString());
        return model;
    }
    model->setQuery("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM formations");
    if (model->lastError().isValid())
    {
        qDebug() << "SQL Error:" << model->lastError().text();
    }
    qDebug() << "Rows fetched:" << model->rowCount() << "Columns:" << model->columnCount();
    for (int row = 0; row < model->rowCount(); ++row)
    {
        qDebug() << "Row" << row << ":"
                 << model->index(row, 0).data().toString() << ","
                 << model->index(row, 1).data().toString() << ","
                 << model->index(row, 2).data().toString() << ","
                 << model->index(row, 3).data().toString() << ","
                 << model->index(row, 4).data().toString() << ","
                 << model->index(row, 5).data().toString() << ","
                 << model->index(row, 6).data().toString();
    }
    return model;
}
bool formations::deleteFormation(int idfor)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FORMATIONS WHERE IDFORM = :id");
    query.bindValue(":id", idfor);

    if (!query.exec())
    {
        qDebug() << "Delete failed:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0)
    {
        qDebug() << "No row found with ID:" << idfor;
        return false;
    }

    return true;
}

bool formations::updateFormation(int idfor, const QString &newFormation, const QString &newDescription,
                                 const QString &newTrainer, const QDate &newDatef, int newTime, double newPrix)
{
    QSqlQuery query;
    query.prepare("UPDATE formations SET formation = :formation, description = :description, "
                  "trainer = :trainer, datef = :datef, time = :time, prix = :prix "
                  "WHERE IDFORM = :id");

    query.bindValue(":formation", newFormation);
    query.bindValue(":description", newDescription);
    query.bindValue(":trainer", newTrainer);
    query.bindValue(":datef", newDatef);
    query.bindValue(":time", newTime);
    query.bindValue(":prix", newPrix); // Bind as double
    query.bindValue(":id", idfor);

    if (!query.exec())
    {
        qDebug() << "Failed to update formation:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0)
    {
        qDebug() << "No row found to update with ID:" << idfor;
        return false;
    }

    return true;
}

bool formations::exists(int idfor)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM formations WHERE idform = :idform");
    query.bindValue(":idform", idfor);

    if (query.exec())
    {
        if (query.next())
        {
            int count = query.value(0).toInt();
            qDebug() << "ID check for" << idfor << ": count =" << count;
            return count > 0;
        }
    }
    else
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }
    return false;
}
