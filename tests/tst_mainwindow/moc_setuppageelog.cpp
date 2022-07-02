/****************************************************************************
** Meta object code from reading C++ file 'setuppageelog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setuppages/setuppageelog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setuppageelog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupPageELog_t {
    QByteArrayData data[18];
    char stringdata0[287];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupPageELog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupPageELog_t qt_meta_stringdata_SetupPageELog = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SetupPageELog"
QT_MOC_LITERAL(1, 14, 8), // "enterKey"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 10), // "qrzcomAuto"
QT_MOC_LITERAL(4, 35, 2), // "_s"
QT_MOC_LITERAL(5, 38, 17), // "slotClubLogActive"
QT_MOC_LITERAL(6, 56, 14), // "slotEQSLActive"
QT_MOC_LITERAL(7, 71, 14), // "slotTQSLActive"
QT_MOC_LITERAL(8, 86, 16), // "slotQRZCOMActive"
QT_MOC_LITERAL(9, 103, 14), // "slotQRZCOMAuto"
QT_MOC_LITERAL(10, 118, 22), // "slotQRZCallTextChanged"
QT_MOC_LITERAL(11, 141, 21), // "slotSelectTQSLClicked"
QT_MOC_LITERAL(12, 163, 23), // "slotPathLineEditChanged"
QT_MOC_LITERAL(13, 187, 2), // "_q"
QT_MOC_LITERAL(14, 190, 19), // "slotEnterKeyPressed"
QT_MOC_LITERAL(15, 210, 27), // "slotClubLogEmailDefineColor"
QT_MOC_LITERAL(16, 238, 24), // "slotLoTWEmailDefineColor"
QT_MOC_LITERAL(17, 263, 23) // "sloteQSLCallTextChanged"

    },
    "SetupPageELog\0enterKey\0\0qrzcomAuto\0"
    "_s\0slotClubLogActive\0slotEQSLActive\0"
    "slotTQSLActive\0slotQRZCOMActive\0"
    "slotQRZCOMAuto\0slotQRZCallTextChanged\0"
    "slotSelectTQSLClicked\0slotPathLineEditChanged\0"
    "_q\0slotEnterKeyPressed\0"
    "slotClubLogEmailDefineColor\0"
    "slotLoTWEmailDefineColor\0"
    "sloteQSLCallTextChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupPageELog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    1,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   88,    2, 0x08 /* Private */,
       6,    1,   91,    2, 0x08 /* Private */,
       7,    1,   94,    2, 0x08 /* Private */,
       8,    1,   97,    2, 0x08 /* Private */,
       9,    1,  100,    2, 0x08 /* Private */,
      10,    0,  103,    2, 0x08 /* Private */,
      11,    0,  104,    2, 0x08 /* Private */,
      12,    1,  105,    2, 0x08 /* Private */,
      14,    0,  108,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetupPageELog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupPageELog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enterKey(); break;
        case 1: _t->qrzcomAuto((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slotClubLogActive((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 3: _t->slotEQSLActive((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 4: _t->slotTQSLActive((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 5: _t->slotQRZCOMActive((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 6: _t->slotQRZCOMAuto((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 7: _t->slotQRZCallTextChanged(); break;
        case 8: _t->slotSelectTQSLClicked(); break;
        case 9: _t->slotPathLineEditChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->slotEnterKeyPressed(); break;
        case 11: _t->slotClubLogEmailDefineColor(); break;
        case 12: _t->slotLoTWEmailDefineColor(); break;
        case 13: _t->sloteQSLCallTextChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetupPageELog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageELog::enterKey)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SetupPageELog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageELog::qrzcomAuto)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetupPageELog::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SetupPageELog.data,
    qt_meta_data_SetupPageELog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupPageELog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupPageELog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupPageELog.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetupPageELog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void SetupPageELog::enterKey()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SetupPageELog::qrzcomAuto(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
