#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "mainwindow.h"

// Global connection object to ensure it persists throughout application lifetime
static Connection g_connection;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    bool test = g_connection.createconnect();
    if (test)
    {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                 QObject::tr("Connection successful.\n"
                                             "Click OK to continue."), QMessageBox::Ok);
    }
    else
    {
        QString errorMsg = "Connection failed.\nDetails: " + g_connection.getDatabase().lastError().text();
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr(qPrintable(errorMsg)), QMessageBox::Cancel);
        return 1; // Exit with error
    }

    return a.exec();
}
