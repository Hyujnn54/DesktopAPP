
#include "connection.h"
#include "formationwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::addLibraryPath("D:/qt/6.7.3/mingw_64/plugins");

    Connection c; // Keep connection in scope
    bool test = c.createconnect();
    FormationWindow w; // Note: Replace MainWindow with FormationWindow if needed
    if (test) {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                                 QObject::tr("connection successful.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);

    } else {
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connection failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
        qDebug() << "Failed to connect!";
        return -1; // Exit if connection fails
    }

    return a.exec(); // Keep the Qt event loop running
}

