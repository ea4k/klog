/****************************************************************************
** Meta object code from reading C++ file 'eqslutilities.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/eqslutilities.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eqslutilities.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_eQSLUtilities_t {
    QByteArrayData data[23];
    char stringdata0[282];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_eQSLUtilities_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_eQSLUtilities_t qt_meta_stringdata_eQSLUtilities = {
    {
QT_MOC_LITERAL(0, 0, 13), // "eQSLUtilities"
QT_MOC_LITERAL(1, 14, 20), // "actionReturnDownload"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 2), // "_i"
QT_MOC_LITERAL(4, 39, 6), // "_qsoId"
QT_MOC_LITERAL(5, 46, 4), // "done"
QT_MOC_LITERAL(6, 51, 17), // "actionShowProgres"
QT_MOC_LITERAL(7, 69, 8), // "received"
QT_MOC_LITERAL(8, 78, 5), // "total"
QT_MOC_LITERAL(9, 84, 11), // "actionError"
QT_MOC_LITERAL(10, 96, 11), // "showMessage"
QT_MOC_LITERAL(11, 108, 2), // "_t"
QT_MOC_LITERAL(12, 111, 17), // "disableeQSLAction"
QT_MOC_LITERAL(13, 129, 2), // "_b"
QT_MOC_LITERAL(14, 132, 18), // "signalFileUploaded"
QT_MOC_LITERAL(15, 151, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(16, 179, 10), // "QList<int>"
QT_MOC_LITERAL(17, 190, 21), // "slotQsoUploadFinished"
QT_MOC_LITERAL(18, 212, 14), // "QNetworkReply*"
QT_MOC_LITERAL(19, 227, 4), // "data"
QT_MOC_LITERAL(20, 232, 16), // "downloadProgress"
QT_MOC_LITERAL(21, 249, 19), // "slotErrorManagement"
QT_MOC_LITERAL(22, 269, 12) // "networkError"

    },
    "eQSLUtilities\0actionReturnDownload\0\0"
    "_i\0_qsoId\0done\0actionShowProgres\0"
    "received\0total\0actionError\0showMessage\0"
    "_t\0disableeQSLAction\0_b\0signalFileUploaded\0"
    "QNetworkReply::NetworkError\0QList<int>\0"
    "slotQsoUploadFinished\0QNetworkReply*\0"
    "data\0downloadProgress\0slotErrorManagement\0"
    "networkError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_eQSLUtilities[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       5,    0,   69,    2, 0x06 /* Public */,
       6,    2,   70,    2, 0x06 /* Public */,
       9,    1,   75,    2, 0x06 /* Public */,
      10,    1,   78,    2, 0x06 /* Public */,
      12,    1,   81,    2, 0x06 /* Public */,
      14,    2,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,   89,    2, 0x08 /* Private */,
      20,    2,   92,    2, 0x08 /* Private */,
      21,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    7,    8,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 16,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    7,    8,
    QMetaType::Void, 0x80000000 | 15,   22,

       0        // eod
};

void eQSLUtilities::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<eQSLUtilities *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->actionReturnDownload((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 1: _t->done(); break;
        case 2: _t->actionShowProgres((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 3: _t->actionError((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: _t->showMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->disableeQSLAction((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 6: _t->signalFileUploaded((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1])),(*reinterpret_cast< QList<int>(*)>(_a[2]))); break;
        case 7: _t->slotQsoUploadFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 8: _t->downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 9: _t->slotErrorManagement((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
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
        case 7:
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
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (eQSLUtilities::*)(const int , const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eQSLUtilities::actionReturnDownload)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (eQSLUtilities::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eQSLUtilities::done)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (eQSLUtilities::*)(qint64 , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eQSLUtilities::actionShowProgres)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (eQSLUtilities::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eQSLUtilities::actionError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (eQSLUtilities::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eQSLUtilities::showMessage)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (eQSLUtilities::*)(const bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eQSLUtilities::disableeQSLAction)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (eQSLUtilities::*)(QNetworkReply::NetworkError , QList<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eQSLUtilities::signalFileUploaded)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject eQSLUtilities::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_eQSLUtilities.data,
    qt_meta_data_eQSLUtilities,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *eQSLUtilities::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *eQSLUtilities::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_eQSLUtilities.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int eQSLUtilities::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void eQSLUtilities::actionReturnDownload(const int _t1, const int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void eQSLUtilities::done()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void eQSLUtilities::actionShowProgres(qint64 _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void eQSLUtilities::actionError(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void eQSLUtilities::showMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void eQSLUtilities::disableeQSLAction(const bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void eQSLUtilities::signalFileUploaded(QNetworkReply::NetworkError _t1, QList<int> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
