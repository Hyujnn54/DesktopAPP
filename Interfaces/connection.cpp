// connection.cpp
#include "connection.h"
#include <QDebug>
#include <QSqlDriver>

QSqlDatabase Connection::db = QSqlDatabase();
bool Connection::isConnected = false;

Connection::Connection()
{
}

Connection::~Connection()
{
    // Don't close the connection in destructor
    // We'll handle it explicitly
}

bool Connection::createconnect()
{
    if (isConnected && db.isOpen()) {
        qDebug() << "Connection already open, returning true";
        return true;
    }

    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_projet");
    db.setUserName("ahmed");
    db.setPassword("2004");

    isConnected = db.open();
    if (isConnected) {
        qDebug() << "Database connection established successfully";
        qDebug() << "Driver Name:" << db.driverName();
        qDebug() << "Database Name:" << db.databaseName();
        qDebug() << "Connected to:" << db.connectionName();
    } else {
        qDebug() << "Database connection failed:" << db.lastError().text();
        qDebug() << "Driver Error:" << db.driver()->lastError().text();
    }

    return isConnected;
}

QSqlDatabase Connection::getDatabase()
{
    return db;
}

void Connection::closeConnection()
{
    if (isConnected) {
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        isConnected = false;
        qDebug() << "Database connection closed";
    }
}
