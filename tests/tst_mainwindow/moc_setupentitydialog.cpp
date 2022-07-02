/****************************************************************************
** Meta object code from reading C++ file 'setupentitydialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/setuppages/setupentitydialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setupentitydialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetupEntityDialog_t {
    QByteArrayData data[18];
    char stringdata0[283];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupEntityDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupEntityDialog_t qt_meta_stringdata_SetupEntityDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SetupEntityDialog"
QT_MOC_LITERAL(1, 18, 11), // "entityAdded"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "_qs"
QT_MOC_LITERAL(4, 35, 19), // "slotOkButtonClicked"
QT_MOC_LITERAL(5, 55, 23), // "slotCancelButtonClicked"
QT_MOC_LITERAL(6, 79, 15), // "slotCheckEntity"
QT_MOC_LITERAL(7, 95, 19), // "slotCheckMainprefix"
QT_MOC_LITERAL(8, 115, 12), // "slotCheckCQz"
QT_MOC_LITERAL(9, 128, 13), // "slotCheckITUz"
QT_MOC_LITERAL(10, 142, 18), // "slotCheckContinent"
QT_MOC_LITERAL(11, 161, 17), // "slotCheckLatitude"
QT_MOC_LITERAL(12, 179, 18), // "slotCheckLongitude"
QT_MOC_LITERAL(13, 198, 12), // "slotCheckUTC"
QT_MOC_LITERAL(14, 211, 15), // "slotCheckARRLid"
QT_MOC_LITERAL(15, 227, 16), // "slotCheckDeleted"
QT_MOC_LITERAL(16, 244, 20), // "slotCheckDeletedDate"
QT_MOC_LITERAL(17, 265, 17) // "slotCheckPrefixes"

    },
    "SetupEntityDialog\0entityAdded\0\0_qs\0"
    "slotOkButtonClicked\0slotCancelButtonClicked\0"
    "slotCheckEntity\0slotCheckMainprefix\0"
    "slotCheckCQz\0slotCheckITUz\0"
    "slotCheckContinent\0slotCheckLatitude\0"
    "slotCheckLongitude\0slotCheckUTC\0"
    "slotCheckARRLid\0slotCheckDeleted\0"
    "slotCheckDeletedDate\0slotCheckPrefixes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupEntityDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   92,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    0,  102,    2, 0x08 /* Private */,
      15,    0,  103,    2, 0x08 /* Private */,
      16,    0,  104,    2, 0x08 /* Private */,
      17,    0,  105,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetupEntityDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SetupEntityDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->entityAdded((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->slotOkButtonClicked(); break;
        case 2: _t->slotCancelButtonClicked(); break;
        case 3: _t->slotCheckEntity(); break;
        case 4: _t->slotCheckMainprefix(); break;
        case 5: _t->slotCheckCQz(); break;
        case 6: _t->slotCheckITUz(); break;
        case 7: _t->slotCheckContinent(); break;
        case 8: _t->slotCheckLatitude(); break;
        case 9: _t->slotCheckLongitude(); break;
        case 10: _t->slotCheckUTC(); break;
        case 11: _t->slotCheckARRLid(); break;
        case 12: _t->slotCheckDeleted(); break;
        case 13: _t->slotCheckDeletedDate(); break;
        case 14: _t->slotCheckPrefixes(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SetupEntityDialog::*)(const QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SetupEntityDialog::entityAdded)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SetupEntityDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SetupEntityDialog.data,
    qt_meta_data_SetupEntityDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SetupEntityDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupEntityDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetupEntityDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SetupEntityDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void SetupEntityDialog::entityAdded(const QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
