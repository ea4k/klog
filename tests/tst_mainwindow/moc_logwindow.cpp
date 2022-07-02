/****************************************************************************
** Meta object code from reading C++ file 'logwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/logwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogWindow_t {
    QByteArrayData data[43];
    char stringdata0[838];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogWindow_t qt_meta_stringdata_LogWindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LogWindow"
QT_MOC_LITERAL(1, 10, 22), // "actionQSODoubleClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 6), // "_qsoid"
QT_MOC_LITERAL(4, 41, 15), // "actionDeleteQSO"
QT_MOC_LITERAL(5, 57, 12), // "updateAwards"
QT_MOC_LITERAL(6, 70, 16), // "updateSearchText"
QT_MOC_LITERAL(7, 87, 10), // "queryError"
QT_MOC_LITERAL(8, 98, 14), // "functionFailed"
QT_MOC_LITERAL(9, 113, 10), // "errorCodeS"
QT_MOC_LITERAL(10, 124, 11), // "nativeError"
QT_MOC_LITERAL(11, 136, 11), // "failedQuery"
QT_MOC_LITERAL(12, 148, 15), // "deleteTheseQSOs"
QT_MOC_LITERAL(13, 164, 10), // "QList<int>"
QT_MOC_LITERAL(14, 175, 5), // "_qsos"
QT_MOC_LITERAL(15, 181, 21), // "exportToADIFTheseQSOs"
QT_MOC_LITERAL(16, 203, 23), // "uploadToQRZcomTheseQSOs"
QT_MOC_LITERAL(17, 227, 18), // "slotDoubleClickLog"
QT_MOC_LITERAL(18, 246, 11), // "QModelIndex"
QT_MOC_LITERAL(19, 258, 5), // "index"
QT_MOC_LITERAL(20, 264, 21), // "slotRighButtonFromLog"
QT_MOC_LITERAL(21, 286, 3), // "pos"
QT_MOC_LITERAL(22, 290, 27), // "slotQSLSentViaBureauFromLog"
QT_MOC_LITERAL(23, 318, 27), // "slotQSLSentViaDirectFromLog"
QT_MOC_LITERAL(24, 346, 26), // "slotQSLRecViaDirectFromLog"
QT_MOC_LITERAL(25, 373, 26), // "slotQSLRecViaBureauFromLog"
QT_MOC_LITERAL(26, 400, 20), // "slotQsoDeleteFromLog"
QT_MOC_LITERAL(27, 421, 20), // "slotQSOToEditFromLog"
QT_MOC_LITERAL(28, 442, 24), // "slotQueryErrorManagement"
QT_MOC_LITERAL(29, 467, 15), // "slotCheckQRZCom"
QT_MOC_LITERAL(30, 483, 18), // "slotCheckDXHeatCom"
QT_MOC_LITERAL(31, 502, 19), // "slotQSOsDeselectAll"
QT_MOC_LITERAL(32, 522, 17), // "slotQSOsSelectAll"
QT_MOC_LITERAL(33, 540, 21), // "slotQSOsDeleteFromLog"
QT_MOC_LITERAL(34, 562, 21), // "slotQSOsExportFromLog"
QT_MOC_LITERAL(35, 584, 27), // "slotQSOsUploadToLoTWFromLog"
QT_MOC_LITERAL(36, 612, 30), // "slotQSOsUploadToClubLogFromLog"
QT_MOC_LITERAL(37, 643, 27), // "slotQSOsUploadToEQSLFromLog"
QT_MOC_LITERAL(38, 671, 35), // "slotMultipleQSLSentViaBureauF..."
QT_MOC_LITERAL(39, 707, 35), // "slotMultipleQSLSentViaDirectF..."
QT_MOC_LITERAL(40, 743, 34), // "slotMultipleQSLRecViaBureauFr..."
QT_MOC_LITERAL(41, 778, 34), // "slotMultipleQSLRecViaDirectFr..."
QT_MOC_LITERAL(42, 813, 24) // "slotQSOsQRZUploadFromLog"

    },
    "LogWindow\0actionQSODoubleClicked\0\0"
    "_qsoid\0actionDeleteQSO\0updateAwards\0"
    "updateSearchText\0queryError\0functionFailed\0"
    "errorCodeS\0nativeError\0failedQuery\0"
    "deleteTheseQSOs\0QList<int>\0_qsos\0"
    "exportToADIFTheseQSOs\0uploadToQRZcomTheseQSOs\0"
    "slotDoubleClickLog\0QModelIndex\0index\0"
    "slotRighButtonFromLog\0pos\0"
    "slotQSLSentViaBureauFromLog\0"
    "slotQSLSentViaDirectFromLog\0"
    "slotQSLRecViaDirectFromLog\0"
    "slotQSLRecViaBureauFromLog\0"
    "slotQsoDeleteFromLog\0slotQSOToEditFromLog\0"
    "slotQueryErrorManagement\0slotCheckQRZCom\0"
    "slotCheckDXHeatCom\0slotQSOsDeselectAll\0"
    "slotQSOsSelectAll\0slotQSOsDeleteFromLog\0"
    "slotQSOsExportFromLog\0slotQSOsUploadToLoTWFromLog\0"
    "slotQSOsUploadToClubLogFromLog\0"
    "slotQSOsUploadToEQSLFromLog\0"
    "slotMultipleQSLSentViaBureauFromLog\0"
    "slotMultipleQSLSentViaDirectFromLog\0"
    "slotMultipleQSLRecViaBureauFromLog\0"
    "slotMultipleQSLRecViaDirectFromLog\0"
    "slotQSOsQRZUploadFromLog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  169,    2, 0x06 /* Public */,
       4,    1,  172,    2, 0x06 /* Public */,
       5,    0,  175,    2, 0x06 /* Public */,
       6,    0,  176,    2, 0x06 /* Public */,
       7,    4,  177,    2, 0x06 /* Public */,
      12,    1,  186,    2, 0x06 /* Public */,
      15,    1,  189,    2, 0x06 /* Public */,
      16,    1,  192,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  195,    2, 0x08 /* Private */,
      20,    1,  198,    2, 0x08 /* Private */,
      22,    0,  201,    2, 0x08 /* Private */,
      23,    0,  202,    2, 0x08 /* Private */,
      24,    0,  203,    2, 0x08 /* Private */,
      25,    0,  204,    2, 0x08 /* Private */,
      26,    0,  205,    2, 0x08 /* Private */,
      27,    0,  206,    2, 0x08 /* Private */,
      28,    4,  207,    2, 0x08 /* Private */,
      29,    0,  216,    2, 0x08 /* Private */,
      30,    0,  217,    2, 0x08 /* Private */,
      31,    0,  218,    2, 0x08 /* Private */,
      32,    0,  219,    2, 0x08 /* Private */,
      33,    0,  220,    2, 0x08 /* Private */,
      34,    0,  221,    2, 0x08 /* Private */,
      35,    0,  222,    2, 0x08 /* Private */,
      36,    0,  223,    2, 0x08 /* Private */,
      37,    0,  224,    2, 0x08 /* Private */,
      38,    0,  225,    2, 0x08 /* Private */,
      39,    0,  226,    2, 0x08 /* Private */,
      40,    0,  227,    2, 0x08 /* Private */,
      41,    0,  228,    2, 0x08 /* Private */,
      42,    0,  229,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 13,   14,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::QPoint,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,   11,
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

void LogWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LogWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->actionQSODoubleClicked((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->actionDeleteQSO((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: _t->updateAwards(); break;
        case 3: _t->updateSearchText(); break;
        case 4: _t->queryError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 5: _t->deleteTheseQSOs((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 6: _t->exportToADIFTheseQSOs((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 7: _t->uploadToQRZcomTheseQSOs((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 8: _t->slotDoubleClickLog((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->slotRighButtonFromLog((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 10: _t->slotQSLSentViaBureauFromLog(); break;
        case 11: _t->slotQSLSentViaDirectFromLog(); break;
        case 12: _t->slotQSLRecViaDirectFromLog(); break;
        case 13: _t->slotQSLRecViaBureauFromLog(); break;
        case 14: _t->slotQsoDeleteFromLog(); break;
        case 15: _t->slotQSOToEditFromLog(); break;
        case 16: _t->slotQueryErrorManagement((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 17: _t->slotCheckQRZCom(); break;
        case 18: _t->slotCheckDXHeatCom(); break;
        case 19: _t->slotQSOsDeselectAll(); break;
        case 20: _t->slotQSOsSelectAll(); break;
        case 21: _t->slotQSOsDeleteFromLog(); break;
        case 22: _t->slotQSOsExportFromLog(); break;
        case 23: _t->slotQSOsUploadToLoTWFromLog(); break;
        case 24: _t->slotQSOsUploadToClubLogFromLog(); break;
        case 25: _t->slotQSOsUploadToEQSLFromLog(); break;
        case 26: _t->slotMultipleQSLSentViaBureauFromLog(); break;
        case 27: _t->slotMultipleQSLSentViaDirectFromLog(); break;
        case 28: _t->slotMultipleQSLRecViaBureauFromLog(); break;
        case 29: _t->slotMultipleQSLRecViaDirectFromLog(); break;
        case 30: _t->slotQSOsQRZUploadFromLog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        case 7:
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
            using _t = void (LogWindow::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::actionQSODoubleClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LogWindow::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::actionDeleteQSO)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LogWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::updateAwards)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LogWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::updateSearchText)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (LogWindow::*)(QString , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::queryError)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (LogWindow::*)(QList<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::deleteTheseQSOs)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (LogWindow::*)(QList<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::exportToADIFTheseQSOs)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (LogWindow::*)(QList<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogWindow::uploadToQRZcomTheseQSOs)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LogWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LogWindow.data,
    qt_meta_data_LogWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LogWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LogWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void LogWindow::actionQSODoubleClicked(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LogWindow::actionDeleteQSO(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LogWindow::updateAwards()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LogWindow::updateSearchText()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void LogWindow::queryError(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void LogWindow::deleteTheseQSOs(QList<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void LogWindow::exportToADIFTheseQSOs(QList<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void LogWindow::uploadToQRZcomTheseQSOs(QList<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
