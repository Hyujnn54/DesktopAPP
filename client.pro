# TrainingManagement.pro

QT += core gui sql printsupport network charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrainingManagement
TEMPLATE = app

CONFIG += c++17

# Debug output to verify flags
message("C++ flags: $$QMAKE_CXXFLAGS")

SOURCES += \
    chartwindow.cpp \
    client.cpp \
    connection.cpp \
    customsqlquerymodel.cpp \
    emailsender.cpp \
    formations.cpp \
    main.cpp \
    mainwindow.cpp \
    formationwindow.cpp \
    updateclientdialog.cpp \
    updatetrainingdialog.cpp

HEADERS += \
    chartwindow.h \
    client.h \
    connection.h \
    customsqlquerymodel.h \
    emailsender.h \
    formations.h \
    mainwindow.h \
    formationwindow.h \
    updateclientdialog.h \
    updatetrainingdialog.h

FORMS += \
    ChartWindow.ui \
    mainwindow.ui \
    formationwindow.ui \
    updateclientdialog.ui \

RESOURCES += \
    img.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
