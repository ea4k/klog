/****************************************************************************
** Meta object code from reading C++ file 'setuppagecolors.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setuppages/setuppagecolors.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setuppagecolors.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupPageColors_t {
    QByteArrayData data[10];
    char stringdata0[227];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupPageColors_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupPageColors_t qt_meta_stringdata_SetupPageColors = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SetupPageColors"
QT_MOC_LITERAL(1, 16, 28), // "slotNewOneColorButtonClicked"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 31), // "slotConfirmedColorButtonClicked"
QT_MOC_LITERAL(4, 78, 28), // "slotWorkedColorButtonClicked"
QT_MOC_LITERAL(5, 107, 28), // "slotNeededColorButtonClicked"
QT_MOC_LITERAL(6, 136, 29), // "slotDefaultColorButtonClicked"
QT_MOC_LITERAL(7, 166, 22), // "slotWSJTXButtonClicked"
QT_MOC_LITERAL(8, 189, 21), // "slotKLogButtonClicked"
QT_MOC_LITERAL(9, 211, 15) // "slotSetDarkMode"

    },
    "SetupPageColors\0slotNewOneColorButtonClicked\0"
    "\0slotConfirmedColorButtonClicked\0"
    "slotWorkedColorButtonClicked\0"
    "slotNeededColorButtonClicked\0"
    "slotDefaultColorButtonClicked\0"
    "slotWSJTXButtonClicked\0slotKLogButtonClicked\0"
    "slotSetDarkMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupPageColors[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetupPageColors::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupPageColors *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotNewOneColorButtonClicked(); break;
        case 1: _t->slotConfirmedColorButtonClicked(); break;
        case 2: _t->slotWorkedColorButtonClicked(); break;
        case 3: _t->slotNeededColorButtonClicked(); break;
        case 4: _t->slotDefaultColorButtonClicked(); break;
        case 5: _t->slotWSJTXButtonClicked(); break;
        case 6: _t->slotKLogButtonClicked(); break;
        case 7: _t->slotSetDarkMode(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SetupPageColors::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SetupPageColors.data,
    qt_meta_data_SetupPageColors,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupPageColors::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupPageColors::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupPageColors.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetupPageColors::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
