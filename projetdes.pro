QT += core gui sql printsupport charts network serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Atelier_Connexion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++17  # Updated to C++17 (optional, can keep c++11 if preferred)
# CONFIG += console  # Comment out or remove unless you need console output

SOURCES += \
    arduino.cpp \
    connection.cpp \
    customsqlquerymodel.cpp \
    formations.cpp \
    main.cpp \
    formationwindow.cpp \
    updatetrainingdialog.cpp

HEADERS += \
    arduino.h \
    connection.h \
    customsqlquerymodel.h \
    formations.h \
    formationwindow.h \
    updatetrainingdialog.h

FORMS += \
    formationwindow.ui

RESOURCES += \
    img.qrc
