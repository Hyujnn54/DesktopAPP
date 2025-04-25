/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
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
    "on_clientAddButton_clicked",
    "",
    "on_clientDeleteButton_clicked",
    "on_clientUpdateButtonClicked",
    "on_clientSearchInput_textChanged",
    "on_clientSearchCriteriaComboBox_currentIndexChanged",
    "on_clientResetSearchButton_clicked",
    "on_clientTableViewHeaderClicked",
    "logicalIndex",
    "on_clientConsultationCalendar_selectionChanged",
    "on_clientConsultationCalendar_activated",
    "date",
    "on_clientExportPdfButton_clicked",
    "on_trainingAddButtonClicked",
    "on_trainingDeleteButtonClicked",
    "on_trainingUpdateButtonClicked",
    "on_trainingSearchInput_textChanged",
    "text",
    "on_trainingResetSearchButton_clicked",
    "on_trainingExportButtonClicked",
    "on_trainingSmsRequestFinished",
    "QNetworkReply*",
    "reply",
    "handleTrainingNotificationLabelClick",
    "toggleSidebar",
    "toggleTheme",
    "sendConsultationReminders",
    "on_statisticsButton_clicked",
    "on_clientSectionButton_clicked",
    "on_trainingSectionButton_clicked"
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
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  158,    2, 0x08,    1 /* Private */,
       3,    0,  159,    2, 0x08,    2 /* Private */,
       4,    0,  160,    2, 0x08,    3 /* Private */,
       5,    0,  161,    2, 0x08,    4 /* Private */,
       6,    0,  162,    2, 0x08,    5 /* Private */,
       7,    0,  163,    2, 0x08,    6 /* Private */,
       8,    1,  164,    2, 0x08,    7 /* Private */,
      10,    0,  167,    2, 0x08,    9 /* Private */,
      11,    1,  168,    2, 0x08,   10 /* Private */,
      13,    0,  171,    2, 0x08,   12 /* Private */,
      14,    0,  172,    2, 0x08,   13 /* Private */,
      15,    0,  173,    2, 0x08,   14 /* Private */,
      16,    0,  174,    2, 0x08,   15 /* Private */,
      17,    1,  175,    2, 0x08,   16 /* Private */,
      19,    0,  178,    2, 0x08,   18 /* Private */,
      20,    0,  179,    2, 0x08,   19 /* Private */,
      21,    1,  180,    2, 0x08,   20 /* Private */,
      24,    0,  183,    2, 0x08,   22 /* Private */,
      25,    0,  184,    2, 0x08,   23 /* Private */,
      26,    0,  185,    2, 0x08,   24 /* Private */,
      27,    0,  186,    2, 0x08,   25 /* Private */,
      28,    0,  187,    2, 0x08,   26 /* Private */,
      29,    0,  188,    2, 0x08,   27 /* Private */,
      30,    0,  189,    2, 0x08,   28 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

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
        // method 'on_clientAddButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientDeleteButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientUpdateButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientSearchInput_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientSearchCriteriaComboBox_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientResetSearchButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientTableViewHeaderClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_clientConsultationCalendar_selectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientConsultationCalendar_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'on_clientExportPdfButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingAddButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingDeleteButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingUpdateButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingSearchInput_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_trainingResetSearchButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingExportButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingSmsRequestFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'handleTrainingNotificationLabelClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleSidebar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleTheme'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendConsultationReminders'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_statisticsButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clientSectionButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_trainingSectionButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_clientAddButton_clicked(); break;
        case 1: _t->on_clientDeleteButton_clicked(); break;
        case 2: _t->on_clientUpdateButtonClicked(); break;
        case 3: _t->on_clientSearchInput_textChanged(); break;
        case 4: _t->on_clientSearchCriteriaComboBox_currentIndexChanged(); break;
        case 5: _t->on_clientResetSearchButton_clicked(); break;
        case 6: _t->on_clientTableViewHeaderClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->on_clientConsultationCalendar_selectionChanged(); break;
        case 8: _t->on_clientConsultationCalendar_activated((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 9: _t->on_clientExportPdfButton_clicked(); break;
        case 10: _t->on_trainingAddButtonClicked(); break;
        case 11: _t->on_trainingDeleteButtonClicked(); break;
        case 12: _t->on_trainingUpdateButtonClicked(); break;
        case 13: _t->on_trainingSearchInput_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->on_trainingResetSearchButton_clicked(); break;
        case 15: _t->on_trainingExportButtonClicked(); break;
        case 16: _t->on_trainingSmsRequestFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 17: _t->handleTrainingNotificationLabelClick(); break;
        case 18: _t->toggleSidebar(); break;
        case 19: _t->toggleTheme(); break;
        case 20: _t->sendConsultationReminders(); break;
        case 21: _t->on_statisticsButton_clicked(); break;
        case 22: _t->on_clientSectionButton_clicked(); break;
        case 23: _t->on_trainingSectionButton_clicked(); break;
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
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    return _id;
}
QT_WARNING_POP
