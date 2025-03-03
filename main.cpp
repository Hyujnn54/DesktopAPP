#include "connection.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::addLibraryPath("D:/qt/6.7.3/mingw_64/plugins");

    Connection c; // Keep connection in scope
    if (!c.createconnect()) {
        qDebug() << "Failed to connect!";
        return -1; // Exit if connection fails
    }

    MainWindow w;
    w.show();
    return a.exec(); // Keep the Qt event loop running
}
