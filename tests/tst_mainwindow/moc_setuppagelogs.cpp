/****************************************************************************
** Meta object code from reading C++ file 'setuppagelogs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setuppages/setuppagelogs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setuppagelogs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupPageLogs_t {
    QByteArrayData data[18];
    char stringdata0[243];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupPageLogs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupPageLogs_t qt_meta_stringdata_SetupPageLogs = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SetupPageLogs"
QT_MOC_LITERAL(1, 14, 10), // "newLogData"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "_qs"
QT_MOC_LITERAL(4, 30, 10), // "queryError"
QT_MOC_LITERAL(5, 41, 14), // "functionFailed"
QT_MOC_LITERAL(6, 56, 10), // "errorCodeS"
QT_MOC_LITERAL(7, 67, 11), // "nativeError"
QT_MOC_LITERAL(8, 79, 11), // "failedQuery"
QT_MOC_LITERAL(9, 91, 7), // "focusOK"
QT_MOC_LITERAL(10, 99, 20), // "slotNewButtonClicked"
QT_MOC_LITERAL(11, 120, 21), // "slotEditButtonClicked"
QT_MOC_LITERAL(12, 142, 23), // "slotRemoveButtonClicked"
QT_MOC_LITERAL(13, 166, 21), // "slotAnalyzeNewLogData"
QT_MOC_LITERAL(14, 188, 15), // "slotLogSelected"
QT_MOC_LITERAL(15, 204, 11), // "QModelIndex"
QT_MOC_LITERAL(16, 216, 5), // "index"
QT_MOC_LITERAL(17, 222, 20) // "slotLogDoubleClicked"

    },
    "SetupPageLogs\0newLogData\0\0_qs\0queryError\0"
    "functionFailed\0errorCodeS\0nativeError\0"
    "failedQuery\0focusOK\0slotNewButtonClicked\0"
    "slotEditButtonClicked\0slotRemoveButtonClicked\0"
    "slotAnalyzeNewLogData\0slotLogSelected\0"
    "QModelIndex\0index\0slotLogDoubleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupPageLogs[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    4,   62,    2, 0x06 /* Public */,
       9,    0,   71,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,
      12,    0,   74,    2, 0x08 /* Private */,
      13,    1,   75,    2, 0x08 /* Private */,
      14,    1,   78,    2, 0x08 /* Private */,
      17,    1,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 15,   16,

       0        // eod
};

void SetupPageLogs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupPageLogs *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newLogData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->queryError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: _t->focusOK(); break;
        case 3: _t->slotNewButtonClicked(); break;
        case 4: _t->slotEditButtonClicked(); break;
        case 5: _t->slotRemoveButtonClicked(); break;
        case 6: _t->slotAnalyzeNewLogData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 7: _t->slotLogSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 8: _t->slotLogDoubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetupPageLogs::*)(const QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageLogs::newLogData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SetupPageLogs::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageLogs::queryError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SetupPageLogs::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageLogs::focusOK)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetupPageLogs::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SetupPageLogs.data,
    qt_meta_data_SetupPageLogs,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupPageLogs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupPageLogs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupPageLogs.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetupPageLogs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SetupPageLogs::newLogData(const QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SetupPageLogs::queryError(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SetupPageLogs::focusOK()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
