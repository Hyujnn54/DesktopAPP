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
               "$$PWD/managers/resources" \
               "$$PWD/ui/mainwindow" \
               "$$PWD/ui/chartwindow" \
               "$$PWD/dialog/updateclientdialog" \
               "$$PWD/dialog/updatetrainingdialog" \
               "$$PWD/dialog/updatemeeting" \
               "$$PWD/dialog/updateemployee" \
               "$$PWD/dialog/updateresourcesdialog" \
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
    managers/resources/resource.h \
    managers/resources/resourcemanager.h \
    ui/chartwindow/chartwindow.h \
    ui/mainwindow/mainwindow.h \
    dialog/updateclientdialog/updateclientdialog.h \
    dialog/updatetrainingdialog/updatetrainingdialog.h \
    dialog/updatemeeting/updatemeeting.h \
    dialog/updateemployee/updateemployeedialog.h \
    dialog/updateresourcesdialog/updateresourcesdialog.h \
    lib/qrcodegen/qrcodegen.hpp \
    sign.h

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
    managers/resources/resource.cpp \
    managers/resources/resourcemanager.cpp \
    ui/chartwindow/chartwindow.cpp \
    ui/mainwindow/mainwindow.cpp \
    dialog/updateclientdialog/updateclientdialog.cpp \
    dialog/updatetrainingdialog/updatetrainingdialog.cpp \
    dialog/updatemeeting/updatemeeting.cpp \
    dialog/updateemployee/updateemployeedialog.cpp \
    dialog/updateresourcesdialog/updateresourcesdialog.cpp \
    lib/qrcodegen/qrcodegen.cpp \
    sign.cpp

FORMS += \
    ui/chartwindow/ChartWindow.ui \
    ui/mainwindow/mainwindow.ui \
    dialog/updateclientdialog/updateclientdialog.ui \
    dialog/updatetrainingdialog/updatetrainingdialog.ui \
    dialog/updatemeeting/updatemeeting.ui \
    dialog/updateemployee/UpdateEmployeeDialog.ui \
    dialog/updateresourcesdialog/updateresourcesdialog.ui \
    sign.ui

# Commented out OpenCV includes and libraries
# INCLUDEPATH += C:/opencv/build/install/include
# 
# LIBS += -LC:/opencv/build/lib \
#            -lopencv_core480 \
#            -lopencv_imgproc480 \
#            -lopencv_imgcodecs480 \
#            -lopencv_highgui480 \
#            -lopencv_videoio480 \
#            -lopencv_objdetect480 \
#            -lopencv_face480 \

RESOURCES += img.qrc

DISTFILES += \
    assets/background.jpg \
    client.files

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
