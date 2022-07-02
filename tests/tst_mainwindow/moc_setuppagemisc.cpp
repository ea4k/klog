/****************************************************************************
** Meta object code from reading C++ file 'setuppagemisc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setuppages/setuppagemisc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setuppagemisc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupPageMisc_t {
    QByteArrayData data[10];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupPageMisc_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupPageMisc_t qt_meta_stringdata_SetupPageMisc = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SetupPageMisc"
QT_MOC_LITERAL(1, 14, 25), // "slotOpenFileButtonClicked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 32), // "slotUseDefaultButtonStateChanged"
QT_MOC_LITERAL(4, 74, 5), // "state"
QT_MOC_LITERAL(5, 80, 34), // "slotDefaultFileNameLineEditCh..."
QT_MOC_LITERAL(6, 115, 34), // "slotcheckNewVersionCheckBoxCl..."
QT_MOC_LITERAL(7, 150, 19), // "slotDBButtonClicked"
QT_MOC_LITERAL(8, 170, 21), // "slotDBLineEditChanged"
QT_MOC_LITERAL(9, 192, 23) // "slotMoveDBButtonClicked"

    },
    "SetupPageMisc\0slotOpenFileButtonClicked\0"
    "\0slotUseDefaultButtonStateChanged\0"
    "state\0slotDefaultFileNameLineEditChanged\0"
    "slotcheckNewVersionCheckBoxClicked\0"
    "slotDBButtonClicked\0slotDBLineEditChanged\0"
    "slotMoveDBButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupPageMisc[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    1,   50,    2, 0x08 /* Private */,
       5,    0,   53,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,
       8,    0,   56,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetupPageMisc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupPageMisc *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotOpenFileButtonClicked(); break;
        case 1: _t->slotUseDefaultButtonStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotDefaultFileNameLineEditChanged(); break;
        case 3: _t->slotcheckNewVersionCheckBoxClicked(); break;
        case 4: _t->slotDBButtonClicked(); break;
        case 5: _t->slotDBLineEditChanged(); break;
        case 6: _t->slotMoveDBButtonClicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetupPageMisc::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SetupPageMisc.data,
    qt_meta_data_SetupPageMisc,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupPageMisc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupPageMisc::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupPageMisc.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetupPageMisc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
