/****************************************************************************
** Meta object code from reading C++ file 'udpserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/udpserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udpserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UDPServer_t {
    QByteArrayData data[30];
    char stringdata0[271];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UDPServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UDPServer_t qt_meta_stringdata_UDPServer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UDPServer"
QT_MOC_LITERAL(1, 10, 13), // "status_update"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 5), // "_type"
QT_MOC_LITERAL(4, 31, 7), // "_dxcall"
QT_MOC_LITERAL(5, 39, 5), // "_freq"
QT_MOC_LITERAL(6, 45, 5), // "_mode"
QT_MOC_LITERAL(7, 51, 6), // "report"
QT_MOC_LITERAL(8, 58, 7), // "de_call"
QT_MOC_LITERAL(9, 66, 7), // "de_grid"
QT_MOC_LITERAL(10, 74, 7), // "dx_grid"
QT_MOC_LITERAL(11, 82, 8), // "sub_mode"
QT_MOC_LITERAL(12, 91, 10), // "logged_qso"
QT_MOC_LITERAL(13, 102, 5), // "_band"
QT_MOC_LITERAL(14, 108, 7), // "_mygrid"
QT_MOC_LITERAL(15, 116, 7), // "_dxgrid"
QT_MOC_LITERAL(16, 124, 5), // "rstTX"
QT_MOC_LITERAL(17, 130, 5), // "rstRX"
QT_MOC_LITERAL(18, 136, 7), // "comment"
QT_MOC_LITERAL(19, 144, 15), // "stationcallsign"
QT_MOC_LITERAL(20, 160, 4), // "name"
QT_MOC_LITERAL(21, 165, 9), // "_operator"
QT_MOC_LITERAL(22, 175, 8), // "datetime"
QT_MOC_LITERAL(23, 184, 12), // "datetime_off"
QT_MOC_LITERAL(24, 197, 11), // "_exchangeTX"
QT_MOC_LITERAL(25, 209, 11), // "_exchangeRX"
QT_MOC_LITERAL(26, 221, 6), // "_txpwr"
QT_MOC_LITERAL(27, 228, 11), // "clearSignal"
QT_MOC_LITERAL(28, 240, 5), // "_func"
QT_MOC_LITERAL(29, 246, 24) // "slotReadPendingDatagrams"

    },
    "UDPServer\0status_update\0\0_type\0_dxcall\0"
    "_freq\0_mode\0report\0de_call\0de_grid\0"
    "dx_grid\0sub_mode\0logged_qso\0_band\0"
    "_mygrid\0_dxgrid\0rstTX\0rstRX\0comment\0"
    "stationcallsign\0name\0_operator\0datetime\0"
    "datetime_off\0_exchangeTX\0_exchangeRX\0"
    "_txpwr\0clearSignal\0_func\0"
    "slotReadPendingDatagrams"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UDPServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    9,   34,    2, 0x06 /* Public */,
      12,   17,   53,    2, 0x06 /* Public */,
      27,    1,   88,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      29,    0,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Double, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,    7,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Double, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QDateTime, QMetaType::QDateTime, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    6,   13,    5,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
    QMetaType::Void, QMetaType::QString,   28,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void UDPServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UDPServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->status_update((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const QString(*)>(_a[8])),(*reinterpret_cast< const QString(*)>(_a[9]))); break;
        case 1: _t->logged_qso((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const QString(*)>(_a[8])),(*reinterpret_cast< const QString(*)>(_a[9])),(*reinterpret_cast< const QString(*)>(_a[10])),(*reinterpret_cast< const QString(*)>(_a[11])),(*reinterpret_cast< const QString(*)>(_a[12])),(*reinterpret_cast< const QDateTime(*)>(_a[13])),(*reinterpret_cast< const QDateTime(*)>(_a[14])),(*reinterpret_cast< const QString(*)>(_a[15])),(*reinterpret_cast< const QString(*)>(_a[16])),(*reinterpret_cast< const QString(*)>(_a[17]))); break;
        case 2: _t->clearSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->slotReadPendingDatagrams(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UDPServer::*)(const int , const QString & , const double , const QString & , const QString , const QString , const QString , const QString , const QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UDPServer::status_update)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UDPServer::*)(const QString & , const QString & , const QString & , const double , const QString & , const QString & , const QString & , const QString & , const QString & , const QString & , const QString & , const QString & , const QDateTime , const QDateTime , const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UDPServer::logged_qso)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UDPServer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UDPServer::clearSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UDPServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UDPServer.data,
    qt_meta_data_UDPServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UDPServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UDPServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UDPServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UDPServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void UDPServer::status_update(const int _t1, const QString & _t2, const double _t3, const QString & _t4, const QString _t5, const QString _t6, const QString _t7, const QString _t8, const QString _t9)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t8))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t9))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UDPServer::logged_qso(const QString & _t1, const QString & _t2, const QString & _t3, const double _t4, const QString & _t5, const QString & _t6, const QString & _t7, const QString & _t8, const QString & _t9, const QString & _t10, const QString & _t11, const QString & _t12, const QDateTime _t13, const QDateTime _t14, const QString & _t15, const QString & _t16, const QString & _t17)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t8))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t9))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t10))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t11))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t12))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t13))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t14))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t15))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t16))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t17))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UDPServer::clearSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
