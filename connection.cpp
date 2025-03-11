// connection.cpp
#include "connection.h"
#include <QDebug>

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
    // If already connected, return true
    if (isConnected && db.isOpen()) {
        return true;
    }

    // If there's an existing connection, remove it
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    // Create new connection
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_projet");
    db.setUserName("ahmed");
    db.setPassword("2004");

    isConnected = db.open();

    if (isConnected) {
        qDebug() << "Database connection established successfully";
    } else {
        qDebug() << "Database connection failed: " << db.lastError().text();
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
