/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ui/mainwindow/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qabstractbarseries.h>
#include <QtCharts/qvbarmodelmapper.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCharts/qcandlestickseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_clientSectionButton_clicked",
    "",
    "on_trainingSectionButton_clicked",
    "on_meetingSectionButton_clicked",
    "on_employeeSectionButton_clicked",
    "on_statisticsButton_clicked",
    "toggleSidebar",
    "toggleTheme",
    "handleNotificationLabelClicked",
    "updateNotificationLabel",
    "count",
    "on_chatSendButton_clicked",
    "on_chatClearButton_clicked",
    "processUserInput",
    "input",
    "onAIResponseReceived",
    "QNetworkReply*",
    "reply",
    "on_employeeSearchChanged",
    "text",
    "on_resetSearchButton_clicked",
    "on_addButton_clicked",
    "on_deleteBtn_clicked",
    "on_modifyBtn_clicked",
    "on_downloadBtn_clicked",
    "on_selectImageButton_clicked",
    "on_generateQRCodeBtn_clicked",
    "validateEmployeeInput",
    "validateCIN",
    "validateName",
    "validatePhone",
    "validateEmail",
    "validateSalary",
    "on_clientChartRefreshButton_clicked",
    "on_trainingChartRefreshButton_clicked",
    "on_meetingChartRefreshButton_clicked",
    "on_clientChartTypeComboBox_currentIndexChanged",
    "index",
    "on_trainingChartTypeComboBox_currentIndexChanged",
    "on_meetingChartTypeComboBox_currentIndexChanged",
    "on_clientChartFilterComboBox_currentIndexChanged",
    "on_trainingChartFilterComboBox_currentIndexChanged",
    "on_meetingChartFilterComboBox_currentIndexChanged"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  230,    2, 0x08,    1 /* Private */,
       3,    0,  231,    2, 0x08,    2 /* Private */,
       4,    0,  232,    2, 0x08,    3 /* Private */,
       5,    0,  233,    2, 0x08,    4 /* Private */,
       6,    0,  234,    2, 0x08,    5 /* Private */,
       7,    0,  235,    2, 0x08,    6 /* Private */,
       8,    0,  236,    2, 0x08,    7 /* Private */,
       9,    0,  237,    2, 0x08,    8 /* Private */,
      10,    1,  238,    2, 0x08,    9 /* Private */,
      12,    0,  241,    2, 0x08,   11 /* Private */,
      13,    0,  242,    2, 0x08,   12 /* Private */,
      14,    1,  243,    2, 0x08,   13 /* Private */,
      16,    1,  246,    2, 0x08,   15 /* Private */,
      19,    1,  249,    2, 0x08,   17 /* Private */,
      21,    0,  252,    2, 0x08,   19 /* Private */,
      22,    0,  253,    2, 0x08,   20 /* Private */,
      23,    0,  254,    2, 0x08,   21 /* Private */,
      24,    0,  255,    2, 0x08,   22 /* Private */,
      25,    0,  256,    2, 0x08,   23 /* Private */,
      26,    0,  257,    2, 0x08,   24 /* Private */,
      27,    0,  258,    2, 0x08,   25 /* Private */,
      28,    0,  259,    2, 0x08,   26 /* Private */,
      29,    1,  260,    2, 0x08,   27 /* Private */,
      30,    1,  263,    2, 0x08,   29 /* Private */,
      31,    1,  266,    2, 0x08,   31 /* Private */,
      32,    1,  269,    2, 0x08,   33 /* Private */,
      33,    1,  272,    2, 0x08,   35 /* Private */,
      34,    0,  275,    2, 0x08,   37 /* Private */,
      35,    0,  276,    2, 0x08,   38 /* Private */,
      36,    0,  277,    2, 0x08,   39 /* Private */,
      37,    1,  278,    2, 0x08,   40 /* Private */,
      39,    1,  281,    2, 0x08,   42 /* Private */,
      40,    1,  284,    2, 0x08,   44 /* Private */,
      41,    1,  287,    2, 0x08,   46 /* Private */,
      42,    1,  290,    2, 0x08,   48 /* Private */,
      43,    1,  293,    2, 0x08,   50 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   38,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_clientSectionButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingSectionButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_meetingSectionButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_employeeSectionButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_statisticsButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleSidebar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleTheme'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleNotificationLabelClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateNotificationLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_chatSendButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chatClearButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processUserInput'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onAIResponseReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'on_employeeSearchChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_resetSearchButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_addButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_deleteBtn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_modifyBtn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_downloadBtn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_selectImageButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_generateQRCodeBtn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'validateEmployeeInput'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'validateCIN'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'validateName'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'validatePhone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'validateEmail'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'validateSalary'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_clientChartRefreshButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingChartRefreshButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_meetingChartRefreshButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientChartTypeComboBox_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_trainingChartTypeComboBox_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_meetingChartTypeComboBox_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_clientChartFilterComboBox_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_trainingChartFilterComboBox_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_meetingChartFilterComboBox_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_clientSectionButton_clicked(); break;
        case 1: _t->on_trainingSectionButton_clicked(); break;
        case 2: _t->on_meetingSectionButton_clicked(); break;
        case 3: _t->on_employeeSectionButton_clicked(); break;
        case 4: _t->on_statisticsButton_clicked(); break;
        case 5: _t->toggleSidebar(); break;
        case 6: _t->toggleTheme(); break;
        case 7: _t->handleNotificationLabelClicked(); break;
        case 8: _t->updateNotificationLabel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->on_chatSendButton_clicked(); break;
        case 10: _t->on_chatClearButton_clicked(); break;
        case 11: _t->processUserInput((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->onAIResponseReceived((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 13: _t->on_employeeSearchChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->on_resetSearchButton_clicked(); break;
        case 15: _t->on_addButton_clicked(); break;
        case 16: _t->on_deleteBtn_clicked(); break;
        case 17: _t->on_modifyBtn_clicked(); break;
        case 18: _t->on_downloadBtn_clicked(); break;
        case 19: _t->on_selectImageButton_clicked(); break;
        case 20: _t->on_generateQRCodeBtn_clicked(); break;
        case 21: { bool _r = _t->validateEmployeeInput();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 22: _t->validateCIN((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->validateName((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->validatePhone((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->validateEmail((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 26: _t->validateSalary((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 27: _t->on_clientChartRefreshButton_clicked(); break;
        case 28: _t->on_trainingChartRefreshButton_clicked(); break;
        case 29: _t->on_meetingChartRefreshButton_clicked(); break;
        case 30: _t->on_clientChartTypeComboBox_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 31: _t->on_trainingChartTypeComboBox_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->on_meetingChartTypeComboBox_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->on_clientChartFilterComboBox_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->on_trainingChartFilterComboBox_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->on_meetingChartFilterComboBox_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 36)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 36;
    }
    return _id;
}
QT_WARNING_POP
