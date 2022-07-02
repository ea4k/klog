/****************************************************************************
** Meta object code from reading C++ file 'mainwindowmydatatab.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/inputwidgets/mainwindowmydatatab.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindowmydatatab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindowMyDataTab_t {
    QByteArrayData data[11];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindowMyDataTab_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindowMyDataTab_t qt_meta_stringdata_MainWindowMyDataTab = {
    {
QT_MOC_LITERAL(0, 0, 19), // "MainWindowMyDataTab"
QT_MOC_LITERAL(1, 20, 18), // "myLocChangedSignal"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 2), // "_q"
QT_MOC_LITERAL(4, 43, 13), // "returnPressed"
QT_MOC_LITERAL(5, 57, 24), // "slotMyLocatorTextChanged"
QT_MOC_LITERAL(6, 82, 23), // "slotOperatorTextChanged"
QT_MOC_LITERAL(7, 106, 30), // "slotStationCallSignTextChanged"
QT_MOC_LITERAL(8, 137, 17), // "slotReturnPressed"
QT_MOC_LITERAL(9, 155, 29), // "slotMyUserADIFComboBoxChanged"
QT_MOC_LITERAL(10, 185, 24) // "slotSetCurrentMyUSerData"

    },
    "MainWindowMyDataTab\0myLocChangedSignal\0"
    "\0_q\0returnPressed\0slotMyLocatorTextChanged\0"
    "slotOperatorTextChanged\0"
    "slotStationCallSignTextChanged\0"
    "slotReturnPressed\0slotMyUserADIFComboBoxChanged\0"
    "slotSetCurrentMyUSerData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindowMyDataTab[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    0,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   58,    2, 0x08 /* Private */,
       6,    0,   59,    2, 0x08 /* Private */,
       7,    0,   60,    2, 0x08 /* Private */,
       8,    0,   61,    2, 0x08 /* Private */,
       9,    0,   62,    2, 0x08 /* Private */,
      10,    0,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindowMyDataTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindowMyDataTab *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->myLocChangedSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->returnPressed(); break;
        case 2: _t->slotMyLocatorTextChanged(); break;
        case 3: _t->slotOperatorTextChanged(); break;
        case 4: _t->slotStationCallSignTextChanged(); break;
        case 5: _t->slotReturnPressed(); break;
        case 6: _t->slotMyUserADIFComboBoxChanged(); break;
        case 7: _t->slotSetCurrentMyUSerData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindowMyDataTab::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindowMyDataTab::myLocChangedSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindowMyDataTab::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindowMyDataTab::returnPressed)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindowMyDataTab::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainWindowMyDataTab.data,
    qt_meta_data_MainWindowMyDataTab,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindowMyDataTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindowMyDataTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindowMyDataTab.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWindowMyDataTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MainWindowMyDataTab::myLocChangedSignal(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindowMyDataTab::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
