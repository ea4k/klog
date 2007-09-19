/****************************************************************************
** Klog meta object code from reading C++ file 'klog.h'
**
** Created: Thu Sep 13 18:01:14 2007
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "klog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Klog::className() const
{
    return "Klog";
}

QMetaObject *Klog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Klog( "Klog", &Klog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Klog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Klog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Klog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Klog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Klog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = klogui::staticMetaObject();
    static const QUMethod slot_0 = {"slotUpdateTime", 0, 0 };
    static const QUMethod slot_1 = {"slotQSLRec", 0, 0 };
    static const QUMethod slot_2 = {"slotQSLSent", 0, 0 };
    static const QUMethod slot_3 = {"slotQSLRecSent", 0, 0 };
    static const QUMethod slot_4 = {"slotQsoDelete", 0, 0 };
    static const QUMethod slot_5 = {"slotCancelSearchButton", 0, 0 };
    static const QUMethod slot_6 = {"slotSearchButton", 0, 0 };
    static const QUMethod slot_7 = {"slotQrzChanged", 0, 0 };
    static const QUMethod slot_8 = {"slotOkBtn", 0, 0 };
    static const QUMethod slot_9 = {"slotClearBtn", 0, 0 };
    static const QUMethod slot_10 = {"clearEntityBox", 0, 0 };
    static const QUMethod slot_11 = {"slotQslSentBoxChanged", 0, 0 };
    static const QUMethod slot_12 = {"slotQslRecvBoxChanged", 0, 0 };
    static const QUMethod slot_13 = {"slotQSLcomboBoxChanged", 0, 0 };
    static const QUMethod slot_14 = {"slotSearchQSO2QSL", 0, 0 };
    static const QUMethod slot_15 = {"slotPreferences", 0, 0 };
    static const QUMethod slot_16 = {"fileNew", 0, 0 };
    static const QUMethod slot_17 = {"fileSave", 0, 0 };
    static const QUMethod slot_18 = {"fileSaveAs", 0, 0 };
    static const QUMethod slot_19 = {"filePrint", 0, 0 };
    static const QUMethod slot_20 = {"slothamlibUpdateFrequency", 0, 0 };
    static const QUParameter param_slot_21[] = {
	{ "fn", &static_QUType_QString, 0, QUParameter::In },
	{ "lb", &static_QUType_ptr, "LogBook", QUParameter::In },
	{ "manualSave", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"adifTempFileSave", 3, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "tfileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"adifReadLog", 1, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out },
	{ "totalSteps", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"getProgresStepForDialog", 2, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ "tfileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_24 = {"tlfReadLog", 1, param_slot_24 };
    static const QUMethod slot_25 = {"fileOpen", 0, 0 };
    static const QUMethod slot_26 = {"fileExit", 0, 0 };
    static const QUMethod slot_27 = {"helpAbout", 0, 0 };
    static const QUMethod slot_28 = {"readQso", 0, 0 };
    static const QUMethod slot_29 = {"modifyQso", 0, 0 };
    static const QUParameter param_slot_30[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_30 = {"slotQsoSelectedForEdit", 1, param_slot_30 };
    static const QUParameter param_slot_31[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_31 = {"slotQsoSearchSelectedForEdit", 1, param_slot_31 };
    static const QUParameter param_slot_32[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_32 = {"slotQsoSelected", 1, param_slot_32 };
    static const QUParameter param_slot_33[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_33 = {"slotQsoRightButtonFromLog", 2, param_slot_33 };
    static const QUParameter param_slot_34[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_34 = {"slotQsoRightButtonFromSearch", 2, param_slot_34 };
    static const QUMethod slot_35 = {"slotModifyBtn", 0, 0 };
    static const QUMethod slot_36 = {"showLogList", 0, 0 };
    static const QUParameter param_slot_37[] = {
	{ "qqso", &static_QUType_int, 0, QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_37 = {"showMenuRightButton", 2, param_slot_37 };
    static const QUParameter param_slot_38[] = {
	{ "tqrz", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_38 = {"getByCall", 1, param_slot_38 };
    static const QUParameter param_slot_39[] = {
	{ "n", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_39 = {"getByNumber", 1, param_slot_39 };
    static const QUMethod slot_40 = {"showQso", 0, 0 };
    static const QUMethod slot_41 = {"toEditQso", 0, 0 };
    static const QUMethod slot_42 = {"accept", 0, 0 };
    static const QUMethod slot_43 = {"slotLocatorChanged", 0, 0 };
    static const QUMethod slot_44 = {"slotMyLocatorChanged", 0, 0 };
    static const QUMethod slot_45 = {"slotClusterCloseConnection", 0, 0 };
    static const QUMethod slot_46 = {"slotClusterSendToServer", 0, 0 };
    static const QUMethod slot_47 = {"slotClusterSocketReadyRead", 0, 0 };
    static const QUMethod slot_48 = {"slotClusterSocketConnected", 0, 0 };
    static const QUMethod slot_49 = {"slotClusterSocketConnectionClosed", 0, 0 };
    static const QUMethod slot_50 = {"slotClusterSocketClosed", 0, 0 };
    static const QUParameter param_slot_51[] = {
	{ "e", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_51 = {"slotClusterSocketError", 1, param_slot_51 };
    static const QUMethod slot_52 = {"slotClusterConnect", 0, 0 };
    static const QUMethod slot_53 = {"slotClusterClearInputLine", 0, 0 };
    static const QUParameter param_slot_54[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_54 = {"slotClusterSpotToLog", 1, param_slot_54 };
    static const QUParameter param_slot_55[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_55 = {"slotClusterSpotCheck", 1, param_slot_55 };
    static const QUMethod slot_56 = {"slotKlogSetup", 0, 0 };
    static const QUMethod slot_57 = {"slotBugReport", 0, 0 };
    static const QUParameter param_slot_58[] = {
	{ "i", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_58 = {"slotModeChanged", 1, param_slot_58 };
    static const QUMethod slot_59 = {"slotBandChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "slotUpdateTime()", &slot_0, QMetaData::Public },
	{ "slotQSLRec()", &slot_1, QMetaData::Public },
	{ "slotQSLSent()", &slot_2, QMetaData::Public },
	{ "slotQSLRecSent()", &slot_3, QMetaData::Public },
	{ "slotQsoDelete()", &slot_4, QMetaData::Public },
	{ "slotCancelSearchButton()", &slot_5, QMetaData::Public },
	{ "slotSearchButton()", &slot_6, QMetaData::Public },
	{ "slotQrzChanged()", &slot_7, QMetaData::Public },
	{ "slotOkBtn()", &slot_8, QMetaData::Public },
	{ "slotClearBtn()", &slot_9, QMetaData::Public },
	{ "clearEntityBox()", &slot_10, QMetaData::Public },
	{ "slotQslSentBoxChanged()", &slot_11, QMetaData::Public },
	{ "slotQslRecvBoxChanged()", &slot_12, QMetaData::Public },
	{ "slotQSLcomboBoxChanged()", &slot_13, QMetaData::Public },
	{ "slotSearchQSO2QSL()", &slot_14, QMetaData::Public },
	{ "slotPreferences()", &slot_15, QMetaData::Public },
	{ "fileNew()", &slot_16, QMetaData::Public },
	{ "fileSave()", &slot_17, QMetaData::Public },
	{ "fileSaveAs()", &slot_18, QMetaData::Public },
	{ "filePrint()", &slot_19, QMetaData::Public },
	{ "slothamlibUpdateFrequency()", &slot_20, QMetaData::Public },
	{ "adifTempFileSave(const QString&,LogBook,bool)", &slot_21, QMetaData::Public },
	{ "adifReadLog(const QString&)", &slot_22, QMetaData::Public },
	{ "getProgresStepForDialog(int)", &slot_23, QMetaData::Public },
	{ "tlfReadLog(const QString&)", &slot_24, QMetaData::Public },
	{ "fileOpen()", &slot_25, QMetaData::Public },
	{ "fileExit()", &slot_26, QMetaData::Public },
	{ "helpAbout()", &slot_27, QMetaData::Public },
	{ "readQso()", &slot_28, QMetaData::Public },
	{ "modifyQso()", &slot_29, QMetaData::Public },
	{ "slotQsoSelectedForEdit(QListViewItem*)", &slot_30, QMetaData::Public },
	{ "slotQsoSearchSelectedForEdit(QListViewItem*)", &slot_31, QMetaData::Public },
	{ "slotQsoSelected(QListViewItem*)", &slot_32, QMetaData::Public },
	{ "slotQsoRightButtonFromLog(QListViewItem*,const QPoint&)", &slot_33, QMetaData::Public },
	{ "slotQsoRightButtonFromSearch(QListViewItem*,const QPoint&)", &slot_34, QMetaData::Public },
	{ "slotModifyBtn()", &slot_35, QMetaData::Public },
	{ "showLogList()", &slot_36, QMetaData::Public },
	{ "showMenuRightButton(int,const QPoint&)", &slot_37, QMetaData::Public },
	{ "getByCall(const QString&)", &slot_38, QMetaData::Public },
	{ "getByNumber(const int)", &slot_39, QMetaData::Public },
	{ "showQso()", &slot_40, QMetaData::Public },
	{ "toEditQso()", &slot_41, QMetaData::Public },
	{ "accept()", &slot_42, QMetaData::Public },
	{ "slotLocatorChanged()", &slot_43, QMetaData::Private },
	{ "slotMyLocatorChanged()", &slot_44, QMetaData::Private },
	{ "slotClusterCloseConnection()", &slot_45, QMetaData::Private },
	{ "slotClusterSendToServer()", &slot_46, QMetaData::Private },
	{ "slotClusterSocketReadyRead()", &slot_47, QMetaData::Private },
	{ "slotClusterSocketConnected()", &slot_48, QMetaData::Private },
	{ "slotClusterSocketConnectionClosed()", &slot_49, QMetaData::Private },
	{ "slotClusterSocketClosed()", &slot_50, QMetaData::Private },
	{ "slotClusterSocketError(int)", &slot_51, QMetaData::Private },
	{ "slotClusterConnect()", &slot_52, QMetaData::Private },
	{ "slotClusterClearInputLine()", &slot_53, QMetaData::Private },
	{ "slotClusterSpotToLog(QListViewItem*)", &slot_54, QMetaData::Private },
	{ "slotClusterSpotCheck(QListViewItem*)", &slot_55, QMetaData::Private },
	{ "slotKlogSetup()", &slot_56, QMetaData::Private },
	{ "slotBugReport()", &slot_57, QMetaData::Private },
	{ "slotModeChanged(int)", &slot_58, QMetaData::Private },
	{ "slotBandChanged()", &slot_59, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Klog", parentObject,
	slot_tbl, 60,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Klog.setMetaObject( metaObj );
    return metaObj;
}

void* Klog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Klog" ) )
	return this;
    return klogui::qt_cast( clname );
}

bool Klog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotUpdateTime(); break;
    case 1: slotQSLRec(); break;
    case 2: slotQSLSent(); break;
    case 3: slotQSLRecSent(); break;
    case 4: slotQsoDelete(); break;
    case 5: slotCancelSearchButton(); break;
    case 6: slotSearchButton(); break;
    case 7: slotQrzChanged(); break;
    case 8: slotOkBtn(); break;
    case 9: slotClearBtn(); break;
    case 10: clearEntityBox(); break;
    case 11: slotQslSentBoxChanged(); break;
    case 12: slotQslRecvBoxChanged(); break;
    case 13: slotQSLcomboBoxChanged(); break;
    case 14: slotSearchQSO2QSL(); break;
    case 15: slotPreferences(); break;
    case 16: fileNew(); break;
    case 17: fileSave(); break;
    case 18: fileSaveAs(); break;
    case 19: filePrint(); break;
    case 20: slothamlibUpdateFrequency(); break;
    case 21: adifTempFileSave((const QString&)static_QUType_QString.get(_o+1),(LogBook)(*((LogBook*)static_QUType_ptr.get(_o+2))),(bool)static_QUType_bool.get(_o+3)); break;
    case 22: adifReadLog((const QString&)static_QUType_QString.get(_o+1)); break;
    case 23: static_QUType_int.set(_o,getProgresStepForDialog((int)static_QUType_int.get(_o+1))); break;
    case 24: tlfReadLog((const QString&)static_QUType_QString.get(_o+1)); break;
    case 25: fileOpen(); break;
    case 26: fileExit(); break;
    case 27: helpAbout(); break;
    case 28: readQso(); break;
    case 29: modifyQso(); break;
    case 30: slotQsoSelectedForEdit((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 31: slotQsoSearchSelectedForEdit((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 32: slotQsoSelected((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 33: slotQsoRightButtonFromLog((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 34: slotQsoRightButtonFromSearch((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 35: slotModifyBtn(); break;
    case 36: showLogList(); break;
    case 37: showMenuRightButton((int)static_QUType_int.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 38: getByCall((const QString&)static_QUType_QString.get(_o+1)); break;
    case 39: getByNumber((const int)static_QUType_int.get(_o+1)); break;
    case 40: showQso(); break;
    case 41: toEditQso(); break;
    case 42: accept(); break;
    case 43: slotLocatorChanged(); break;
    case 44: slotMyLocatorChanged(); break;
    case 45: slotClusterCloseConnection(); break;
    case 46: slotClusterSendToServer(); break;
    case 47: slotClusterSocketReadyRead(); break;
    case 48: slotClusterSocketConnected(); break;
    case 49: slotClusterSocketConnectionClosed(); break;
    case 50: slotClusterSocketClosed(); break;
    case 51: slotClusterSocketError((int)static_QUType_int.get(_o+1)); break;
    case 52: slotClusterConnect(); break;
    case 53: slotClusterClearInputLine(); break;
    case 54: slotClusterSpotToLog((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 55: slotClusterSpotCheck((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 56: slotKlogSetup(); break;
    case 57: slotBugReport(); break;
    case 58: slotModeChanged((int)static_QUType_int.get(_o+1)); break;
    case 59: slotBandChanged(); break;
    default:
	return klogui::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Klog::qt_emit( int _id, QUObject* _o )
{
    return klogui::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Klog::qt_property( int id, int f, QVariant* v)
{
    return klogui::qt_property( id, f, v);
}

bool Klog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
