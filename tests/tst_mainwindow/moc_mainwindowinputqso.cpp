/****************************************************************************
** Meta object code from reading C++ file 'mainwindowinputqso.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/inputwidgets/mainwindowinputqso.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindowinputqso.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindowInputQSO_t {
    QByteArrayData data[14];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindowInputQSO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindowInputQSO_t qt_meta_stringdata_MainWindowInputQSO = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MainWindowInputQSO"
QT_MOC_LITERAL(1, 19, 13), // "returnPressed"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "dxLocatorChanged"
QT_MOC_LITERAL(4, 51, 4), // "_loc"
QT_MOC_LITERAL(5, 56, 13), // "rxFreqChanged"
QT_MOC_LITERAL(6, 70, 2), // "_f"
QT_MOC_LITERAL(7, 73, 13), // "txFreqChanged"
QT_MOC_LITERAL(8, 87, 19), // "handOverFocusSignal"
QT_MOC_LITERAL(9, 107, 17), // "slotReturnPressed"
QT_MOC_LITERAL(10, 125, 22), // "slotLocatorTextChanged"
QT_MOC_LITERAL(11, 148, 17), // "slotFreqTXChanged"
QT_MOC_LITERAL(12, 166, 17), // "slotFreqRXChanged"
QT_MOC_LITERAL(13, 184, 16) // "slotSplitClicked"

    },
    "MainWindowInputQSO\0returnPressed\0\0"
    "dxLocatorChanged\0_loc\0rxFreqChanged\0"
    "_f\0txFreqChanged\0handOverFocusSignal\0"
    "slotReturnPressed\0slotLocatorTextChanged\0"
    "slotFreqTXChanged\0slotFreqRXChanged\0"
    "slotSplitClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindowInputQSO[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,
       5,    1,   68,    2, 0x06 /* Public */,
       7,    1,   71,    2, 0x06 /* Public */,
       8,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   75,    2, 0x08 /* Private */,
      10,    0,   76,    2, 0x08 /* Private */,
      11,    1,   77,    2, 0x08 /* Private */,
      12,    1,   80,    2, 0x08 /* Private */,
      13,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,

       0        // eod
};

void MainWindowInputQSO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindowInputQSO *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->returnPressed(); break;
        case 1: _t->dxLocatorChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->rxFreqChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->txFreqChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->handOverFocusSignal(); break;
        case 5: _t->slotReturnPressed(); break;
        case 6: _t->slotLocatorTextChanged(); break;
        case 7: _t->slotFreqTXChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->slotFreqRXChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->slotSplitClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindowInputQSO::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindowInputQSO::returnPressed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindowInputQSO::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindowInputQSO::dxLocatorChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindowInputQSO::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindowInputQSO::rxFreqChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindowInputQSO::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindowInputQSO::txFreqChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindowInputQSO::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindowInputQSO::handOverFocusSignal)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindowInputQSO::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainWindowInputQSO.data,
    qt_meta_data_MainWindowInputQSO,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindowInputQSO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindowInputQSO::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindowInputQSO.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWindowInputQSO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MainWindowInputQSO::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindowInputQSO::dxLocatorChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindowInputQSO::rxFreqChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindowInputQSO::txFreqChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindowInputQSO::handOverFocusSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
