/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
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
    "handleAddButtonClick",
    "",
    "handleDeleteButtonClick",
    "handleUpdateButtonClick",
    "updateInputFields",
    "handleSearchButtonClick",
    "handleSearchTextChanged",
    "text",
    "handleSortCriteriaChanged",
    "index",
    "handleStatisticsButtonClick",
    "toggleTheme",
    "toggleSidebar",
    "handleGenerateQRCodeButtonClick",
    "on_chatSendButton_clicked",
    "on_chatClearButton_clicked",
    "processUserInput",
    "input",
    "handleRefreshStatsButtonClick",
    "on_toggleLegendCheckBox_stateChanged",
    "state",
    "onAIResponseReceived",
    "QNetworkReply*",
    "reply",
    "handleExportPdfButtonClick",
    "handleTabChanged"
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
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  128,    2, 0x08,    1 /* Private */,
       3,    0,  129,    2, 0x08,    2 /* Private */,
       4,    0,  130,    2, 0x08,    3 /* Private */,
       5,    0,  131,    2, 0x08,    4 /* Private */,
       6,    0,  132,    2, 0x08,    5 /* Private */,
       7,    1,  133,    2, 0x08,    6 /* Private */,
       9,    1,  136,    2, 0x08,    8 /* Private */,
      11,    0,  139,    2, 0x08,   10 /* Private */,
      12,    0,  140,    2, 0x08,   11 /* Private */,
      13,    0,  141,    2, 0x08,   12 /* Private */,
      14,    0,  142,    2, 0x08,   13 /* Private */,
      15,    0,  143,    2, 0x08,   14 /* Private */,
      16,    0,  144,    2, 0x08,   15 /* Private */,
      17,    1,  145,    2, 0x08,   16 /* Private */,
      19,    0,  148,    2, 0x08,   18 /* Private */,
      20,    1,  149,    2, 0x08,   19 /* Private */,
      22,    1,  152,    2, 0x08,   21 /* Private */,
      25,    0,  155,    2, 0x08,   23 /* Private */,
      26,    1,  156,    2, 0x08,   24 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,

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
        // method 'handleAddButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleDeleteButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleUpdateButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateInputFields'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSearchButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleSortCriteriaChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleStatisticsButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleTheme'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleSidebar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleGenerateQRCodeButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chatSendButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_chatClearButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processUserInput'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleRefreshStatsButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_toggleLegendCheckBox_stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onAIResponseReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'handleExportPdfButtonClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleTabChanged'
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
        case 0: _t->handleAddButtonClick(); break;
        case 1: _t->handleDeleteButtonClick(); break;
        case 2: _t->handleUpdateButtonClick(); break;
        case 3: _t->updateInputFields(); break;
        case 4: _t->handleSearchButtonClick(); break;
        case 5: _t->handleSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->handleSortCriteriaChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->handleStatisticsButtonClick(); break;
        case 8: _t->toggleTheme(); break;
        case 9: _t->toggleSidebar(); break;
        case 10: _t->handleGenerateQRCodeButtonClick(); break;
        case 11: _t->on_chatSendButton_clicked(); break;
        case 12: _t->on_chatClearButton_clicked(); break;
        case 13: _t->processUserInput((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->handleRefreshStatsButtonClick(); break;
        case 15: _t->on_toggleLegendCheckBox_stateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->onAIResponseReceived((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 17: _t->handleExportPdfButtonClick(); break;
        case 18: _t->handleTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP
