#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <connection.h>
#include <QDebug>
#include <sign.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    if (!c.createconnect()) {
        qDebug() << "Failed to connect to database";
        return -1;
    }
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "testing_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    sign signWindow;  // ✅ Changed from login to sign
    signWindow.show();
    return a.exec();
}
