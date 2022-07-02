/****************************************************************************
** Meta object code from reading C++ file 'setuppagelogsnew.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setuppages/setuppagelogsnew.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setuppagelogsnew.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupPageLogsNew_t {
    QByteArrayData data[10];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupPageLogsNew_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupPageLogsNew_t qt_meta_stringdata_SetupPageLogsNew = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SetupPageLogsNew"
QT_MOC_LITERAL(1, 17, 10), // "newLogData"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "_qs"
QT_MOC_LITERAL(4, 33, 9), // "cancelled"
QT_MOC_LITERAL(5, 43, 2), // "_c"
QT_MOC_LITERAL(6, 46, 19), // "slotOKButtonClicked"
QT_MOC_LITERAL(7, 66, 23), // "slotCancelButtonClicked"
QT_MOC_LITERAL(8, 90, 30), // "slotStationCallSignTextChanged"
QT_MOC_LITERAL(9, 121, 24) // "slotOperatorsTextChanged"

    },
    "SetupPageLogsNew\0newLogData\0\0_qs\0"
    "cancelled\0_c\0slotOKButtonClicked\0"
    "slotCancelButtonClicked\0"
    "slotStationCallSignTextChanged\0"
    "slotOperatorsTextChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupPageLogsNew[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   50,    2, 0x08 /* Private */,
       7,    0,   51,    2, 0x08 /* Private */,
       8,    0,   52,    2, 0x08 /* Private */,
       9,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::Bool,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetupPageLogsNew::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupPageLogsNew *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newLogData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->cancelled((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 2: _t->slotOKButtonClicked(); break;
        case 3: _t->slotCancelButtonClicked(); break;
        case 4: _t->slotStationCallSignTextChanged(); break;
        case 5: _t->slotOperatorsTextChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetupPageLogsNew::*)(const QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageLogsNew::newLogData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SetupPageLogsNew::*)(const bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageLogsNew::cancelled)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetupPageLogsNew::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SetupPageLogsNew.data,
    qt_meta_data_SetupPageLogsNew,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupPageLogsNew::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupPageLogsNew::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupPageLogsNew.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SetupPageLogsNew::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SetupPageLogsNew::newLogData(const QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SetupPageLogsNew::cancelled(const bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
