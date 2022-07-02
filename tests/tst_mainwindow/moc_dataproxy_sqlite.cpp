/****************************************************************************
** Meta object code from reading C++ file 'dataproxy_sqlite.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/dataproxy_sqlite.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataproxy_sqlite.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataProxy_SQLite_t {
    QByteArrayData data[17];
    char stringdata0[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataProxy_SQLite_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataProxy_SQLite_t qt_meta_stringdata_DataProxy_SQLite = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DataProxy_SQLite"
QT_MOC_LITERAL(1, 17, 8), // "qsoFound"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 3), // "_qs"
QT_MOC_LITERAL(4, 31, 10), // "queryError"
QT_MOC_LITERAL(5, 42, 14), // "functionFailed"
QT_MOC_LITERAL(6, 57, 10), // "errorCodeS"
QT_MOC_LITERAL(7, 68, 11), // "nativeError"
QT_MOC_LITERAL(8, 80, 11), // "failedQuery"
QT_MOC_LITERAL(9, 92, 8), // "debugLog"
QT_MOC_LITERAL(10, 101, 9), // "errorCode"
QT_MOC_LITERAL(11, 111, 13), // "DebugLogLevel"
QT_MOC_LITERAL(12, 125, 5), // "level"
QT_MOC_LITERAL(13, 131, 20), // "slotCaptureDebugLogs"
QT_MOC_LITERAL(14, 152, 5), // "_func"
QT_MOC_LITERAL(15, 158, 4), // "_msg"
QT_MOC_LITERAL(16, 163, 6) // "_level"

    },
    "DataProxy_SQLite\0qsoFound\0\0_qs\0"
    "queryError\0functionFailed\0errorCodeS\0"
    "nativeError\0failedQuery\0debugLog\0"
    "errorCode\0DebugLogLevel\0level\0"
    "slotCaptureDebugLogs\0_func\0_msg\0_level"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataProxy_SQLite[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    4,   42,    2, 0x06 /* Public */,
       9,    3,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    3,   58,    2, 0x08 /* Private */,
      13,    2,   65,    2, 0x28 /* Private | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 11,    5,   10,   12,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 11,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   14,   15,

       0        // eod
};

void DataProxy_SQLite::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataProxy_SQLite *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->qsoFound((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->queryError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: _t->debugLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< DebugLogLevel(*)>(_a[3]))); break;
        case 3: _t->slotCaptureDebugLogs((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const DebugLogLevel(*)>(_a[3]))); break;
        case 4: _t->slotCaptureDebugLogs((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataProxy_SQLite::*)(const QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataProxy_SQLite::qsoFound)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataProxy_SQLite::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataProxy_SQLite::queryError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataProxy_SQLite::*)(QString , QString , DebugLogLevel );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataProxy_SQLite::debugLog)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataProxy_SQLite::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_DataProxy_SQLite.data,
    qt_meta_data_DataProxy_SQLite,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataProxy_SQLite::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataProxy_SQLite::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataProxy_SQLite.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataProxy_SQLite::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DataProxy_SQLite::qsoFound(const QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataProxy_SQLite::queryError(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataProxy_SQLite::debugLog(QString _t1, QString _t2, DebugLogLevel _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
