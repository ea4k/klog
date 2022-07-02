/****************************************************************************
** Meta object code from reading C++ file 'awardswidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/awardswidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'awardswidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AwardsWidget_t {
    QByteArrayData data[16];
    char stringdata0[233];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwardsWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwardsWidget_t qt_meta_stringdata_AwardsWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwardsWidget"
QT_MOC_LITERAL(1, 13, 8), // "debugLog"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 5), // "_func"
QT_MOC_LITERAL(4, 29, 4), // "_msg"
QT_MOC_LITERAL(5, 34, 13), // "DebugLogLevel"
QT_MOC_LITERAL(6, 48, 6), // "_level"
QT_MOC_LITERAL(7, 55, 10), // "queryError"
QT_MOC_LITERAL(8, 66, 14), // "functionFailed"
QT_MOC_LITERAL(9, 81, 10), // "errorCodeS"
QT_MOC_LITERAL(10, 92, 11), // "nativeError"
QT_MOC_LITERAL(11, 104, 11), // "failedQuery"
QT_MOC_LITERAL(12, 116, 23), // "requireCurrentLogSignal"
QT_MOC_LITERAL(13, 140, 24), // "requireCurrentYearSignal"
QT_MOC_LITERAL(14, 165, 34), // "slotRecalculateAwardsButtonCl..."
QT_MOC_LITERAL(15, 200, 32) // "slotOperatingYearComboBoxChanged"

    },
    "AwardsWidget\0debugLog\0\0_func\0_msg\0"
    "DebugLogLevel\0_level\0queryError\0"
    "functionFailed\0errorCodeS\0nativeError\0"
    "failedQuery\0requireCurrentLogSignal\0"
    "requireCurrentYearSignal\0"
    "slotRecalculateAwardsButtonClicked\0"
    "slotOperatingYearComboBoxChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwardsWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x06 /* Public */,
       7,    4,   51,    2, 0x06 /* Public */,
      12,    0,   60,    2, 0x06 /* Public */,
      13,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   62,    2, 0x08 /* Private */,
      15,    0,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwardsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AwardsWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->debugLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< DebugLogLevel(*)>(_a[3]))); break;
        case 1: _t->queryError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: _t->requireCurrentLogSignal(); break;
        case 3: _t->requireCurrentYearSignal(); break;
        case 4: _t->slotRecalculateAwardsButtonClicked(); break;
        case 5: _t->slotOperatingYearComboBoxChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AwardsWidget::*)(QString , QString , DebugLogLevel );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AwardsWidget::debugLog)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AwardsWidget::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AwardsWidget::queryError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AwardsWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AwardsWidget::requireCurrentLogSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (AwardsWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AwardsWidget::requireCurrentYearSignal)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AwardsWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_AwardsWidget.data,
    qt_meta_data_AwardsWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AwardsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwardsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AwardsWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AwardsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AwardsWidget::debugLog(QString _t1, QString _t2, DebugLogLevel _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwardsWidget::queryError(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwardsWidget::requireCurrentLogSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AwardsWidget::requireCurrentYearSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
