/****************************************************************************
** Meta object code from reading C++ file 'downloadcty.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/downloadcty.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'downloadcty.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DownLoadCTY_t {
    QByteArrayData data[18];
    char stringdata0[236];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DownLoadCTY_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DownLoadCTY_t qt_meta_stringdata_DownLoadCTY = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DownLoadCTY"
QT_MOC_LITERAL(1, 12, 20), // "actionReturnDownload"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 2), // "_i"
QT_MOC_LITERAL(4, 37, 4), // "done"
QT_MOC_LITERAL(5, 42, 8), // "_success"
QT_MOC_LITERAL(6, 51, 17), // "actionShowProgres"
QT_MOC_LITERAL(7, 69, 8), // "received"
QT_MOC_LITERAL(8, 78, 5), // "total"
QT_MOC_LITERAL(9, 84, 11), // "actionError"
QT_MOC_LITERAL(10, 96, 15), // "downloadStopped"
QT_MOC_LITERAL(11, 112, 20), // "slotDownloadFinished"
QT_MOC_LITERAL(12, 133, 14), // "QNetworkReply*"
QT_MOC_LITERAL(13, 148, 5), // "reply"
QT_MOC_LITERAL(14, 154, 20), // "slotDownloadProgress"
QT_MOC_LITERAL(15, 175, 19), // "slotErrorManagement"
QT_MOC_LITERAL(16, 195, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(17, 223, 12) // "networkError"

    },
    "DownLoadCTY\0actionReturnDownload\0\0_i\0"
    "done\0_success\0actionShowProgres\0"
    "received\0total\0actionError\0downloadStopped\0"
    "slotDownloadFinished\0QNetworkReply*\0"
    "reply\0slotDownloadProgress\0"
    "slotErrorManagement\0QNetworkReply::NetworkError\0"
    "networkError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DownLoadCTY[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    2,   60,    2, 0x06 /* Public */,
       9,    1,   65,    2, 0x06 /* Public */,
      10,    0,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   69,    2, 0x08 /* Private */,
      14,    2,   72,    2, 0x08 /* Private */,
      15,    1,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    7,    8,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    7,    8,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void DownLoadCTY::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DownLoadCTY *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->actionReturnDownload((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->done((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->actionShowProgres((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 3: _t->actionError((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: _t->downloadStopped(); break;
        case 5: _t->slotDownloadFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 6: _t->slotDownloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 7: _t->slotErrorManagement((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 7:
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
            using _t = void (DownLoadCTY::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownLoadCTY::actionReturnDownload)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DownLoadCTY::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownLoadCTY::done)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DownLoadCTY::*)(qint64 , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownLoadCTY::actionShowProgres)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DownLoadCTY::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownLoadCTY::actionError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DownLoadCTY::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownLoadCTY::downloadStopped)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DownLoadCTY::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_DownLoadCTY.data,
    qt_meta_data_DownLoadCTY,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DownLoadCTY::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownLoadCTY::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DownLoadCTY.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DownLoadCTY::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void DownLoadCTY::actionReturnDownload(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DownLoadCTY::done(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DownLoadCTY::actionShowProgres(qint64 _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DownLoadCTY::actionError(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DownLoadCTY::downloadStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
