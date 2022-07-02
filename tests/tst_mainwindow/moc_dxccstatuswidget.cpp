/****************************************************************************
** Meta object code from reading C++ file 'dxccstatuswidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/dxccstatuswidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dxccstatuswidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DXCCStatusWidget_t {
    QByteArrayData data[16];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DXCCStatusWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DXCCStatusWidget_t qt_meta_stringdata_DXCCStatusWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DXCCStatusWidget"
QT_MOC_LITERAL(1, 17, 8), // "showQsos"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "QList<int>"
QT_MOC_LITERAL(4, 38, 5), // "_qsos"
QT_MOC_LITERAL(5, 44, 15), // "fillInQSOSignal"
QT_MOC_LITERAL(6, 60, 8), // "debugLog"
QT_MOC_LITERAL(7, 69, 5), // "_func"
QT_MOC_LITERAL(8, 75, 4), // "_msg"
QT_MOC_LITERAL(9, 80, 13), // "DebugLogLevel"
QT_MOC_LITERAL(10, 94, 6), // "_level"
QT_MOC_LITERAL(11, 101, 24), // "slotRefreshButtonClicked"
QT_MOC_LITERAL(12, 126, 15), // "slotItemEntered"
QT_MOC_LITERAL(13, 142, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(14, 160, 4), // "item"
QT_MOC_LITERAL(15, 165, 21) // "slotItemDoubleClicked"

    },
    "DXCCStatusWidget\0showQsos\0\0QList<int>\0"
    "_qsos\0fillInQSOSignal\0debugLog\0_func\0"
    "_msg\0DebugLogLevel\0_level\0"
    "slotRefreshButtonClicked\0slotItemEntered\0"
    "QTableWidgetItem*\0item\0slotItemDoubleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DXCCStatusWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    0,   47,    2, 0x06 /* Public */,
       6,    3,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   55,    2, 0x0a /* Public */,
      12,    1,   56,    2, 0x08 /* Private */,
      15,    1,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 9,    7,    8,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void DXCCStatusWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DXCCStatusWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showQsos((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 1: _t->fillInQSOSignal(); break;
        case 2: _t->debugLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< DebugLogLevel(*)>(_a[3]))); break;
        case 3: _t->slotRefreshButtonClicked(); break;
        case 4: _t->slotItemEntered((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->slotItemDoubleClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DXCCStatusWidget::*)(QList<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DXCCStatusWidget::showQsos)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DXCCStatusWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DXCCStatusWidget::fillInQSOSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DXCCStatusWidget::*)(QString , QString , DebugLogLevel );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DXCCStatusWidget::debugLog)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DXCCStatusWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DXCCStatusWidget.data,
    qt_meta_data_DXCCStatusWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DXCCStatusWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DXCCStatusWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DXCCStatusWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DXCCStatusWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DXCCStatusWidget::showQsos(QList<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DXCCStatusWidget::fillInQSOSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DXCCStatusWidget::debugLog(QString _t1, QString _t2, DebugLogLevel _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
