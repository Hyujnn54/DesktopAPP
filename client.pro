# client.pro
QT += core gui sql printsupport network charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrainingManagement
TEMPLATE = app

CONFIG += c++17

# Quote paths to handle spaces
INCLUDEPATH += "$$PWD/core" \
               "$$PWD/managers/client" \
               "$$PWD/managers/training" \
               "$$PWD/managers/meeting" \
               "$$PWD/managers/employee" \
               "$$PWD/ui/mainwindow" \
               "$$PWD/ui/chartwindow" \
               "$$PWD/dialog/updateclientdialog" \
               "$$PWD/dialog/updatetrainingdialog" \
               "$$PWD/dialog/updatemeeting" \
               "$$PWD/dialog/updateemployee" \
               "$$PWD/lib/qrcodegen"

HEADERS += \
    core/connection.h \
    core/customsqlquerymodel.h \
    core/emailsender.h \
    core/notificationmanager.h \
    managers/client/client.h \
    managers/client/clientmanager.h \
    managers/training/formations.h \
    managers/training/trainingmanager.h \
    managers/meeting/meeting.h \
    managers/meeting/meetingmanager.h \
    managers/employee/employee.h \
    managers/employee/employeemanager.h \
    ui/chartwindow/chartwindow.h \
    ui/mainwindow/mainwindow.h \
    dialog/updateclientdialog/updateclientdialog.h \
    dialog/updatetrainingdialog/updatetrainingdialog.h \
    dialog/updatemeeting/updatemeeting.h \
    dialog/updateemployee/updateemployeedialog.h \
    lib/qrcodegen/qrcodegen.hpp

SOURCES += \
    core/connection.cpp \
    core/customsqlquerymodel.cpp \
    core/emailsender.cpp \
    core/main.cpp \
    core/notificationmanager.cpp \
    managers/client/client.cpp \
    managers/client/clientmanager.cpp \
    managers/training/formations.cpp \
    managers/training/trainingmanager.cpp \
    managers/meeting/meeting.cpp \
    managers/meeting/meetingmanager.cpp \
    managers/employee/employee.cpp \
    managers/employee/employeemanager.cpp \
    ui/chartwindow/chartwindow.cpp \
    ui/mainwindow/mainwindow.cpp \
    dialog/updateclientdialog/updateclientdialog.cpp \
    dialog/updatetrainingdialog/updatetrainingdialog.cpp \
    dialog/updatemeeting/updatemeeting.cpp \
    dialog/updateemployee/updateemployeedialog.cpp \
    lib/qrcodegen/qrcodegen.cpp

FORMS += \
    ui/chartwindow/ChartWindow.ui \
    ui/mainwindow/mainwindow.ui \
    dialog/updateclientdialog/updateclientdialog.ui \
    dialog/updatetrainingdialog/updatetrainingdialog.ui \
    dialog/updatemeeting/updatemeeting.ui \
    dialog/updateemployee/UpdateEmployeeDialog.ui

RESOURCES += img.qrc

DISTFILES += \
    assets/background.jpg \
    client.files

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
