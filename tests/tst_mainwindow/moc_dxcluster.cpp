/****************************************************************************
** Meta object code from reading C++ file 'dxcluster.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/dxcluster.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dxcluster.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DXClusterWidget_t {
    QByteArrayData data[22];
    char stringdata0[437];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DXClusterWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DXClusterWidget_t qt_meta_stringdata_DXClusterWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "DXClusterWidget"
QT_MOC_LITERAL(1, 16, 13), // "dxspotclicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "_qs"
QT_MOC_LITERAL(4, 35, 13), // "dxspotArrived"
QT_MOC_LITERAL(5, 49, 5), // "_call"
QT_MOC_LITERAL(6, 55, 5), // "_text"
QT_MOC_LITERAL(7, 61, 5), // "_freq"
QT_MOC_LITERAL(8, 67, 23), // "slotClusterDisplayError"
QT_MOC_LITERAL(9, 91, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(10, 120, 11), // "socketError"
QT_MOC_LITERAL(11, 132, 22), // "slotClusterDataArrived"
QT_MOC_LITERAL(12, 155, 26), // "slotClusterSocketConnected"
QT_MOC_LITERAL(13, 182, 33), // "slotClusterSocketConnectionCl..."
QT_MOC_LITERAL(14, 216, 23), // "slotClusterSendToServer"
QT_MOC_LITERAL(15, 240, 25), // "slotClusterClearLineInput"
QT_MOC_LITERAL(16, 266, 27), // "slotClusterInputTextChanged"
QT_MOC_LITERAL(17, 294, 43), // "slotClusterDXClusterWidgetIte..."
QT_MOC_LITERAL(18, 338, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(19, 355, 4), // "item"
QT_MOC_LITERAL(20, 360, 37), // "slotClusterDXClusterWidgetIte..."
QT_MOC_LITERAL(21, 398, 38) // "slotClusterDXClusterWidgetIte..."

    },
    "DXClusterWidget\0dxspotclicked\0\0_qs\0"
    "dxspotArrived\0_call\0_text\0_freq\0"
    "slotClusterDisplayError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "slotClusterDataArrived\0"
    "slotClusterSocketConnected\0"
    "slotClusterSocketConnectionClosed\0"
    "slotClusterSendToServer\0"
    "slotClusterClearLineInput\0"
    "slotClusterInputTextChanged\0"
    "slotClusterDXClusterWidgetItemDoubleClicked\0"
    "QListWidgetItem*\0item\0"
    "slotClusterDXClusterWidgetItemEntered\0"
    "slotClusterDXClusterWidgetItemSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DXClusterWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    3,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   84,    2, 0x08 /* Private */,
      11,    0,   87,    2, 0x08 /* Private */,
      12,    0,   88,    2, 0x08 /* Private */,
      13,    0,   89,    2, 0x08 /* Private */,
      14,    0,   90,    2, 0x08 /* Private */,
      15,    0,   91,    2, 0x08 /* Private */,
      16,    0,   92,    2, 0x08 /* Private */,
      17,    1,   93,    2, 0x08 /* Private */,
      20,    1,   96,    2, 0x08 /* Private */,
      21,    0,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Double,    5,    6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void,

       0        // eod
};

void DXClusterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DXClusterWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dxspotclicked((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->dxspotArrived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 2: _t->slotClusterDisplayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: _t->slotClusterDataArrived(); break;
        case 4: _t->slotClusterSocketConnected(); break;
        case 5: _t->slotClusterSocketConnectionClosed(); break;
        case 6: _t->slotClusterSendToServer(); break;
        case 7: _t->slotClusterClearLineInput(); break;
        case 8: _t->slotClusterInputTextChanged(); break;
        case 9: _t->slotClusterDXClusterWidgetItemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 10: _t->slotClusterDXClusterWidgetItemEntered((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->slotClusterDXClusterWidgetItemSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DXClusterWidget::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DXClusterWidget::dxspotclicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DXClusterWidget::*)(const QString & , const QString & , const double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DXClusterWidget::dxspotArrived)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DXClusterWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DXClusterWidget.data,
    qt_meta_data_DXClusterWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DXClusterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DXClusterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DXClusterWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DXClusterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void DXClusterWidget::dxspotclicked(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DXClusterWidget::dxspotArrived(const QString & _t1, const QString & _t2, const double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
