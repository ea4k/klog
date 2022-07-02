/****************************************************************************
** Meta object code from reading C++ file 'elogqrzlog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/elogqrzlog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'elogqrzlog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_eLogQrzLog_t {
    QByteArrayData data[27];
    char stringdata0[326];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_eLogQrzLog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_eLogQrzLog_t qt_meta_stringdata_eLogQrzLog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "eLogQrzLog"
QT_MOC_LITERAL(1, 11, 20), // "actionReturnDownload"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 2), // "_i"
QT_MOC_LITERAL(4, 36, 6), // "_qsoId"
QT_MOC_LITERAL(5, 43, 4), // "done"
QT_MOC_LITERAL(6, 48, 17), // "actionShowProgres"
QT_MOC_LITERAL(7, 66, 8), // "received"
QT_MOC_LITERAL(8, 75, 5), // "total"
QT_MOC_LITERAL(9, 81, 11), // "actionError"
QT_MOC_LITERAL(10, 93, 11), // "showMessage"
QT_MOC_LITERAL(11, 105, 2), // "_t"
QT_MOC_LITERAL(12, 108, 16), // "disableQRZAction"
QT_MOC_LITERAL(13, 125, 2), // "_b"
QT_MOC_LITERAL(14, 128, 17), // "signalLogUploaded"
QT_MOC_LITERAL(15, 146, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(16, 174, 10), // "QList<int>"
QT_MOC_LITERAL(17, 185, 15), // "dataFoundSignal"
QT_MOC_LITERAL(18, 201, 5), // "_type"
QT_MOC_LITERAL(19, 207, 5), // "_data"
QT_MOC_LITERAL(20, 213, 19), // "slotManagerFinished"
QT_MOC_LITERAL(21, 233, 14), // "QNetworkReply*"
QT_MOC_LITERAL(22, 248, 4), // "data"
QT_MOC_LITERAL(23, 253, 22), // "slotManagerLogFinished"
QT_MOC_LITERAL(24, 276, 16), // "downloadProgress"
QT_MOC_LITERAL(25, 293, 19), // "slotErrorManagement"
QT_MOC_LITERAL(26, 313, 12) // "networkError"

    },
    "eLogQrzLog\0actionReturnDownload\0\0_i\0"
    "_qsoId\0done\0actionShowProgres\0received\0"
    "total\0actionError\0showMessage\0_t\0"
    "disableQRZAction\0_b\0signalLogUploaded\0"
    "QNetworkReply::NetworkError\0QList<int>\0"
    "dataFoundSignal\0_type\0_data\0"
    "slotManagerFinished\0QNetworkReply*\0"
    "data\0slotManagerLogFinished\0"
    "downloadProgress\0slotErrorManagement\0"
    "networkError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_eLogQrzLog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       5,    0,   79,    2, 0x06 /* Public */,
       6,    2,   80,    2, 0x06 /* Public */,
       9,    1,   85,    2, 0x06 /* Public */,
      10,    1,   88,    2, 0x06 /* Public */,
      12,    1,   91,    2, 0x06 /* Public */,
      14,    2,   94,    2, 0x06 /* Public */,
      17,    2,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    1,  104,    2, 0x08 /* Private */,
      23,    1,  107,    2, 0x08 /* Private */,
      24,    2,  110,    2, 0x08 /* Private */,
      25,    1,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    7,    8,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 16,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   18,   19,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    7,    8,
    QMetaType::Void, 0x80000000 | 15,   26,

       0        // eod
};

void eLogQrzLog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<eLogQrzLog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->actionReturnDownload((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 1: _t->done(); break;
        case 2: _t->actionShowProgres((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 3: _t->actionError((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: _t->showMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->disableQRZAction((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 6: _t->signalLogUploaded((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1])),(*reinterpret_cast< QList<int>(*)>(_a[2]))); break;
        case 7: _t->dataFoundSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->slotManagerFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 9: _t->slotManagerLogFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 10: _t->downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 11: _t->slotErrorManagement((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (eLogQrzLog::*)(const int , const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::actionReturnDownload)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (eLogQrzLog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::done)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (eLogQrzLog::*)(qint64 , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::actionShowProgres)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (eLogQrzLog::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::actionError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (eLogQrzLog::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::showMessage)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (eLogQrzLog::*)(const bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::disableQRZAction)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (eLogQrzLog::*)(QNetworkReply::NetworkError , QList<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::signalLogUploaded)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (eLogQrzLog::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eLogQrzLog::dataFoundSignal)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject eLogQrzLog::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_eLogQrzLog.data,
    qt_meta_data_eLogQrzLog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *eLogQrzLog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *eLogQrzLog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_eLogQrzLog.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int eLogQrzLog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void eLogQrzLog::actionReturnDownload(const int _t1, const int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void eLogQrzLog::done()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void eLogQrzLog::actionShowProgres(qint64 _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void eLogQrzLog::actionError(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void eLogQrzLog::showMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void eLogQrzLog::disableQRZAction(const bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void eLogQrzLog::signalLogUploaded(QNetworkReply::NetworkError _t1, QList<int> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void eLogQrzLog::dataFoundSignal(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
