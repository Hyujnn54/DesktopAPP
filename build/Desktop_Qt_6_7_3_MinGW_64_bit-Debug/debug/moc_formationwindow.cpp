/****************************************************************************
** Meta object code from reading C++ file 'formationwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../formationwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formationwindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSFormationWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSFormationWindowENDCLASS = QtMocHelpers::stringData(
    "FormationWindow",
    "on_addButtonclicked",
    "",
    "toggleTheme",
    "toggleSidebar",
    "on_deleteButtonClicked",
    "on_updateButtonClicked",
    "on_searchInput_textChanged",
    "text",
    "on_resetSearchButton_clicked",
    "exportToPdf",
    "onNotificationLabelClicked",
    "on_refreshStatsButton_clicked",
    "onSmsRequestFinished",
    "QNetworkReply*",
    "reply",
    "on_pushButton_2_clicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSFormationWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x08,    1 /* Private */,
       3,    0,   87,    2, 0x08,    2 /* Private */,
       4,    0,   88,    2, 0x08,    3 /* Private */,
       5,    0,   89,    2, 0x08,    4 /* Private */,
       6,    0,   90,    2, 0x08,    5 /* Private */,
       7,    1,   91,    2, 0x08,    6 /* Private */,
       9,    0,   94,    2, 0x08,    8 /* Private */,
      10,    0,   95,    2, 0x08,    9 /* Private */,
      11,    0,   96,    2, 0x08,   10 /* Private */,
      12,    0,   97,    2, 0x08,   11 /* Private */,
      13,    1,   98,    2, 0x08,   12 /* Private */,
      16,    0,  101,    2, 0x08,   14 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject FormationWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSFormationWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSFormationWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSFormationWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FormationWindow, std::true_type>,
        // method 'on_addButtonclicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleTheme'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggleSidebar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_deleteButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_updateButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_searchInput_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_resetSearchButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportToPdf'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNotificationLabelClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_refreshStatsButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSmsRequestFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'on_pushButton_2_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void FormationWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FormationWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_addButtonclicked(); break;
        case 1: _t->toggleTheme(); break;
        case 2: _t->toggleSidebar(); break;
        case 3: _t->on_deleteButtonClicked(); break;
        case 4: _t->on_updateButtonClicked(); break;
        case 5: _t->on_searchInput_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->on_resetSearchButton_clicked(); break;
        case 7: _t->exportToPdf(); break;
        case 8: _t->onNotificationLabelClicked(); break;
        case 9: _t->on_refreshStatsButton_clicked(); break;
        case 10: _t->onSmsRequestFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 11: _t->on_pushButton_2_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *FormationWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FormationWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSFormationWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FormationWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
