/****************************************************************************
** Meta object code from reading C++ file 'tipsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/tipsdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tipsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TipsDialog_t {
    QByteArrayData data[20];
    char stringdata0[319];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TipsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TipsDialog_t qt_meta_stringdata_TipsDialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TipsDialog"
QT_MOC_LITERAL(1, 11, 8), // "debugLog"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 5), // "_func"
QT_MOC_LITERAL(4, 27, 4), // "_msg"
QT_MOC_LITERAL(5, 32, 13), // "DebugLogLevel"
QT_MOC_LITERAL(6, 46, 6), // "_level"
QT_MOC_LITERAL(7, 53, 15), // "fillInQSOSignal"
QT_MOC_LITERAL(8, 69, 16), // "fillInDXCCSignal"
QT_MOC_LITERAL(9, 86, 17), // "findQSL2QSOSignal"
QT_MOC_LITERAL(10, 104, 23), // "fileExportToPrintSignal"
QT_MOC_LITERAL(11, 128, 24), // "fileOpenKLogFolderSignal"
QT_MOC_LITERAL(12, 153, 24), // "toolSendPendingQSLSignal"
QT_MOC_LITERAL(13, 178, 23), // "toolRecPendingQSLSignal"
QT_MOC_LITERAL(14, 202, 26), // "toolRecRecPendingQSLSignal"
QT_MOC_LITERAL(15, 229, 21), // "toolsUploadLoTWSignal"
QT_MOC_LITERAL(16, 251, 21), // "slotPrevButtonClicked"
QT_MOC_LITERAL(17, 273, 21), // "slotNextButtonClicked"
QT_MOC_LITERAL(18, 295, 17), // "slotLinkActivated"
QT_MOC_LITERAL(19, 313, 5) // "_link"

    },
    "TipsDialog\0debugLog\0\0_func\0_msg\0"
    "DebugLogLevel\0_level\0fillInQSOSignal\0"
    "fillInDXCCSignal\0findQSL2QSOSignal\0"
    "fileExportToPrintSignal\0"
    "fileOpenKLogFolderSignal\0"
    "toolSendPendingQSLSignal\0"
    "toolRecPendingQSLSignal\0"
    "toolRecRecPendingQSLSignal\0"
    "toolsUploadLoTWSignal\0slotPrevButtonClicked\0"
    "slotNextButtonClicked\0slotLinkActivated\0"
    "_link"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TipsDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   79,    2, 0x06 /* Public */,
       7,    0,   86,    2, 0x06 /* Public */,
       8,    0,   87,    2, 0x06 /* Public */,
       9,    0,   88,    2, 0x06 /* Public */,
      10,    0,   89,    2, 0x06 /* Public */,
      11,    0,   90,    2, 0x06 /* Public */,
      12,    0,   91,    2, 0x06 /* Public */,
      13,    0,   92,    2, 0x06 /* Public */,
      14,    0,   93,    2, 0x06 /* Public */,
      15,    0,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    0,   95,    2, 0x08 /* Private */,
      17,    0,   96,    2, 0x08 /* Private */,
      18,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   19,

       0        // eod
};

void TipsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TipsDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->debugLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< DebugLogLevel(*)>(_a[3]))); break;
        case 1: _t->fillInQSOSignal(); break;
        case 2: _t->fillInDXCCSignal(); break;
        case 3: _t->findQSL2QSOSignal(); break;
        case 4: _t->fileExportToPrintSignal(); break;
        case 5: _t->fileOpenKLogFolderSignal(); break;
        case 6: _t->toolSendPendingQSLSignal(); break;
        case 7: _t->toolRecPendingQSLSignal(); break;
        case 8: _t->toolRecRecPendingQSLSignal(); break;
        case 9: _t->toolsUploadLoTWSignal(); break;
        case 10: _t->slotPrevButtonClicked(); break;
        case 11: _t->slotNextButtonClicked(); break;
        case 12: _t->slotLinkActivated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TipsDialog::*)(QString , QString , DebugLogLevel );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::debugLog)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::fillInQSOSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::fillInDXCCSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::findQSL2QSOSignal)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::fileExportToPrintSignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::fileOpenKLogFolderSignal)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::toolSendPendingQSLSignal)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::toolRecPendingQSLSignal)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::toolRecRecPendingQSLSignal)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TipsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TipsDialog::toolsUploadLoTWSignal)) {
                *result = 9;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TipsDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_TipsDialog.data,
    qt_meta_data_TipsDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TipsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TipsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TipsDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int TipsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void TipsDialog::debugLog(QString _t1, QString _t2, DebugLogLevel _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TipsDialog::fillInQSOSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TipsDialog::fillInDXCCSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TipsDialog::findQSL2QSOSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TipsDialog::fileExportToPrintSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TipsDialog::fileOpenKLogFolderSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TipsDialog::toolSendPendingQSLSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TipsDialog::toolRecPendingQSLSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TipsDialog::toolRecRecPendingQSLSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TipsDialog::toolsUploadLoTWSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
