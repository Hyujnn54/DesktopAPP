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
    clientmanager.cpp \
    connection.cpp \
    customsqlquerymodel.cpp \
    emailsender.cpp \
    formations.cpp \
    main.cpp \
    mainwindow.cpp \
    trainingmanager.cpp \
    updateclientdialog.cpp \
    updatetrainingdialog.cpp

HEADERS += \
    ClickableLabel.h \
    chartwindow.h \
    client.h \
    clientmanager.h \
    connection.h \
    customsqlquerymodel.h \
    emailsender.h \
    formations.h \
    mainwindow.h \
    trainingmanager.h \
    updateclientdialog.h \
    updatetrainingdialog.h

FORMS += \
    ChartWindow.ui \
    mainwindow.ui \
    updateclientdialog.ui \
    updatetrainingdialog.ui

RESOURCES += \
    img.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    background.jpg
