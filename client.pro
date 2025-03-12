QT += core gui sql printsupport network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# Debug output to verify flags
message("C++ flags: $$QMAKE_CXXFLAGS")

SOURCES += \
    chartwindow.cpp \
    client.cpp \
    connection.cpp \
    emailsender.cpp \
    main.cpp \
    mainwindow.cpp \
    updateclientdialog.cpp

HEADERS += \
    chartwindow.h \
    client.h \
    connection.h \
    emailsender.h \
    mainwindow.h \
    updateclientdialog.h

FORMS += \
    ChartWindow.ui \
    mainwindow.ui \
    updateclientdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
