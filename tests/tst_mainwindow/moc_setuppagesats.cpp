/****************************************************************************
** Meta object code from reading C++ file 'setuppagesats.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setuppages/setuppagesats.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setuppagesats.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupPageSats_t {
    QByteArrayData data[19];
    char stringdata0[283];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupPageSats_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupPageSats_t qt_meta_stringdata_SetupPageSats = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SetupPageSats"
QT_MOC_LITERAL(1, 14, 10), // "newSatData"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "_qs"
QT_MOC_LITERAL(4, 30, 10), // "queryError"
QT_MOC_LITERAL(5, 41, 14), // "functionFailed"
QT_MOC_LITERAL(6, 56, 10), // "errorCodeS"
QT_MOC_LITERAL(7, 67, 11), // "nativeError"
QT_MOC_LITERAL(8, 79, 11), // "failedQuery"
QT_MOC_LITERAL(9, 91, 23), // "slotImportButtonClicked"
QT_MOC_LITERAL(10, 115, 23), // "slotExportButtonClicked"
QT_MOC_LITERAL(11, 139, 20), // "slotNewButtonClicked"
QT_MOC_LITERAL(12, 160, 21), // "slotEditButtonClicked"
QT_MOC_LITERAL(13, 182, 23), // "slotRemoveButtonClicked"
QT_MOC_LITERAL(14, 206, 21), // "slotAnalyzeNewSatData"
QT_MOC_LITERAL(15, 228, 15), // "slotSatselected"
QT_MOC_LITERAL(16, 244, 11), // "QModelIndex"
QT_MOC_LITERAL(17, 256, 5), // "index"
QT_MOC_LITERAL(18, 262, 20) // "slotSatDoubleClicked"

    },
    "SetupPageSats\0newSatData\0\0_qs\0queryError\0"
    "functionFailed\0errorCodeS\0nativeError\0"
    "failedQuery\0slotImportButtonClicked\0"
    "slotExportButtonClicked\0slotNewButtonClicked\0"
    "slotEditButtonClicked\0slotRemoveButtonClicked\0"
    "slotAnalyzeNewSatData\0slotSatselected\0"
    "QModelIndex\0index\0slotSatDoubleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupPageSats[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    4,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x08 /* Private */,
      11,    0,   78,    2, 0x08 /* Private */,
      12,    0,   79,    2, 0x08 /* Private */,
      13,    0,   80,    2, 0x08 /* Private */,
      14,    1,   81,    2, 0x08 /* Private */,
      15,    1,   84,    2, 0x08 /* Private */,
      18,    1,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    6,    7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void SetupPageSats::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupPageSats *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newSatData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->queryError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 2: _t->slotImportButtonClicked(); break;
        case 3: _t->slotExportButtonClicked(); break;
        case 4: _t->slotNewButtonClicked(); break;
        case 5: _t->slotEditButtonClicked(); break;
        case 6: _t->slotRemoveButtonClicked(); break;
        case 7: _t->slotAnalyzeNewSatData((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 8: _t->slotSatselected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->slotSatDoubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetupPageSats::*)(const QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageSats::newSatData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SetupPageSats::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupPageSats::queryError)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetupPageSats::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SetupPageSats.data,
    qt_meta_data_SetupPageSats,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupPageSats::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupPageSats::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupPageSats.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetupPageSats::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SetupPageSats::newSatData(const QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SetupPageSats::queryError(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
