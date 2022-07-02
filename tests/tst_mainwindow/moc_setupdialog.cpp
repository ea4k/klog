/****************************************************************************
** Meta object code from reading C++ file 'setupdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setupdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setupdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupDialog_t {
    QByteArrayData data[31];
    char stringdata0[376];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupDialog_t qt_meta_stringdata_SetupDialog = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SetupDialog"
QT_MOC_LITERAL(1, 12, 10), // "exitSignal"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "status"
QT_MOC_LITERAL(4, 31, 10), // "queryError"
QT_MOC_LITERAL(5, 42, 14), // "functionFailed"
QT_MOC_LITERAL(6, 57, 10), // "errorCodeS"
QT_MOC_LITERAL(7, 68, 11), // "nativeError"
QT_MOC_LITERAL(8, 80, 11), // "failedQuery"
QT_MOC_LITERAL(9, 92, 8), // "debugLog"
QT_MOC_LITERAL(10, 101, 5), // "_func"
QT_MOC_LITERAL(11, 107, 4), // "_msg"
QT_MOC_LITERAL(12, 112, 13), // "DebugLogLevel"
QT_MOC_LITERAL(13, 126, 6), // "_level"
QT_MOC_LITERAL(14, 133, 10), // "qrzcomAuto"
QT_MOC_LITERAL(15, 144, 10), // "changePage"
QT_MOC_LITERAL(16, 155, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(17, 172, 7), // "current"
QT_MOC_LITERAL(18, 180, 8), // "previous"
QT_MOC_LITERAL(19, 189, 18), // "slotReadConfigData"
QT_MOC_LITERAL(20, 208, 19), // "slotOkButtonClicked"
QT_MOC_LITERAL(21, 228, 23), // "slotCancelButtonClicked"
QT_MOC_LITERAL(22, 252, 21), // "slotAnalyzeNewLogData"
QT_MOC_LITERAL(23, 274, 3), // "_qs"
QT_MOC_LITERAL(24, 278, 22), // "slotSetStationCallSign"
QT_MOC_LITERAL(25, 301, 2), // "_p"
QT_MOC_LITERAL(26, 304, 16), // "slotSetOperators"
QT_MOC_LITERAL(27, 321, 24), // "slotQueryErrorManagement"
QT_MOC_LITERAL(28, 346, 11), // "slotFocusOK"
QT_MOC_LITERAL(29, 358, 14), // "slotQRZCOMAuto"
QT_MOC_LITERAL(30, 373, 2) // "_b"

    },
    "SetupDialog\0exitSignal\0\0status\0"
    "queryError\0functionFailed\0errorCodeS\0"
    "nativeError\0failedQuery\0debugLog\0_func\0"
    "_msg\0DebugLogLevel\0_level\0qrzcomAuto\0"
    "changePage\0QListWidgetItem*\0current\0"
    "previous\0slotReadConfigData\0"
    "slotOkButtonClicked\0slotCancelButtonClicked\0"
    "slotAnalyzeNewLogData\0_qs\0"
    "slotSetStationCallSign\0_p\0slotSetOperators\0"
    "slotQueryErrorManagement\0slotFocusOK\0"
    "slotQRZCOMAuto\0_b"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    4,   87,    2, 0x06 /* Public */,
       9,    3,   96,    2, 0x06 /* Public */,
      14,    1,  103,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    2,  106,    2, 0x08 /* Private */,
      19,    0,  111,    2, 0x08 /* Private */,
      20,    0,  112,    2, 0x08 /* Private */,
      21,    0,  113,    2, 0x08 /* Private */,
      22,    1,  114,    2, 0x08 /* Private */,
      24,    1,  117,    2, 0x08 /* Private */,
      26,    1,  120,    2, 0x08 /* Private */,
      27,    4,  123,    2, 0x08 /* Private */,
      28,    0,  132,    2, 0x08 /* Private */,
      29,    1,  133,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 12,   10,   11,   13,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 16,   17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   23,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   30,

       0        // eod
};

void SetupDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->exitSignal((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->queryError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: _t->debugLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< DebugLogLevel(*)>(_a[3]))); break;
        case 3: _t->qrzcomAuto((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->changePage((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 5: _t->slotReadConfigData(); break;
        case 6: _t->slotOkButtonClicked(); break;
        case 7: _t->slotCancelButtonClicked(); break;
        case 8: _t->slotAnalyzeNewLogData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 9: _t->slotSetStationCallSign((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->slotSetOperators((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->slotQueryErrorManagement((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 12: _t->slotFocusOK(); break;
        case 13: _t->slotQRZCOMAuto((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetupDialog::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupDialog::exitSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SetupDialog::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupDialog::queryError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SetupDialog::*)(QString , QString , DebugLogLevel );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupDialog::debugLog)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SetupDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupDialog::qrzcomAuto)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetupDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SetupDialog.data,
    qt_meta_data_SetupDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SetupDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void SetupDialog::exitSignal(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SetupDialog::queryError(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SetupDialog::debugLog(QString _t1, QString _t2, DebugLogLevel _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SetupDialog::qrzcomAuto(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
