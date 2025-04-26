#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Check available drivers
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();

    // Create an instance of Connection and attempt to establish the database connection
    Connection conn;
    bool dbConnected = conn.createconnect();
    if (!dbConnected) {
        QMessageBox::warning(nullptr, "Database Warning",
                             "Failed to connect to the database. Some features may be unavailable.");
        // Continue running the application instead of exiting
    }

    MainWindow w(dbConnected); // Pass the connection status to MainWindow
    w.show();
    return a.exec();
}
