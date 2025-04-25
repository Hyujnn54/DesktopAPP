// connection.h
#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{
public:
    Connection();
    ~Connection();
    bool createconnect();
    static QSqlDatabase getDatabase();
    static void closeConnection(); // Add method to properly close connection

private:
    static QSqlDatabase db;
    static bool isConnected;
};

#endif // CONNECTION_H
