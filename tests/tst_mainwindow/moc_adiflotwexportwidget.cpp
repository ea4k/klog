/****************************************************************************
** Meta object code from reading C++ file 'adiflotwexportwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/widgets/adiflotwexportwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'adiflotwexportwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AdifLoTWExportWidget_t {
    QByteArrayData data[12];
    char stringdata0[168];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AdifLoTWExportWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AdifLoTWExportWidget_t qt_meta_stringdata_AdifLoTWExportWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "AdifLoTWExportWidget"
QT_MOC_LITERAL(1, 21, 9), // "selection"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 3), // "_st"
QT_MOC_LITERAL(4, 36, 7), // "_startD"
QT_MOC_LITERAL(5, 44, 5), // "_endD"
QT_MOC_LITERAL(6, 50, 10), // "ExportMode"
QT_MOC_LITERAL(7, 61, 11), // "_exportMode"
QT_MOC_LITERAL(8, 73, 23), // "slotOKPushButtonClicked"
QT_MOC_LITERAL(9, 97, 27), // "slotCancelPushButtonClicked"
QT_MOC_LITERAL(10, 125, 26), // "slotStationCallsignChanged"
QT_MOC_LITERAL(11, 152, 15) // "slotDateChanged"

    },
    "AdifLoTWExportWidget\0selection\0\0_st\0"
    "_startD\0_endD\0ExportMode\0_exportMode\0"
    "slotOKPushButtonClicked\0"
    "slotCancelPushButtonClicked\0"
    "slotStationCallsignChanged\0slotDateChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AdifLoTWExportWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   48,    2, 0x08 /* Private */,
       9,    0,   49,    2, 0x08 /* Private */,
      10,    0,   50,    2, 0x08 /* Private */,
      11,    0,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QDate, QMetaType::QDate, 0x80000000 | 6,    3,    4,    5,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AdifLoTWExportWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AdifLoTWExportWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selection((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QDate(*)>(_a[2])),(*reinterpret_cast< QDate(*)>(_a[3])),(*reinterpret_cast< ExportMode(*)>(_a[4]))); break;
        case 1: _t->slotOKPushButtonClicked(); break;
        case 2: _t->slotCancelPushButtonClicked(); break;
        case 3: _t->slotStationCallsignChanged(); break;
        case 4: _t->slotDateChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AdifLoTWExportWidget::*)(QString , QDate , QDate , ExportMode );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AdifLoTWExportWidget::selection)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AdifLoTWExportWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_AdifLoTWExportWidget.data,
    qt_meta_data_AdifLoTWExportWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AdifLoTWExportWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdifLoTWExportWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AdifLoTWExportWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AdifLoTWExportWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AdifLoTWExportWidget::selection(QString _t1, QDate _t2, QDate _t3, ExportMode _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
