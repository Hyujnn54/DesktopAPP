QT += core gui sql printsupport charts network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Atelier_Connexion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11 console

SOURCES += \
    connection.cpp \
    customsqlquerymodel.cpp \
    formations.cpp \
    main.cpp \
    formationwindow.cpp \
    updatetrainingdialog.cpp

HEADERS += \
    connection.h \
    customsqlquerymodel.h \
    formations.h \
    formationwindow.h \
    updatetrainingdialog.h

FORMS += \
    formationwindow.ui

RESOURCES += \
    img.qrc
