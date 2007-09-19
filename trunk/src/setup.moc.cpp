/****************************************************************************
** Setup meta object code from reading C++ file 'setup.h'
**
** Created: Thu Sep 13 18:01:10 2007
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "setup.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Setup::className() const
{
    return "Setup";
}

QMetaObject *Setup::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Setup( "Setup", &Setup::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Setup::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Setup", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Setup::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Setup", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Setup::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = setupui::staticMetaObject();
    static const QUMethod slot_0 = {"setRig", 0, 0 };
    static const QUMethod slot_1 = {"slotOkBtnClicked", 0, 0 };
    static const QUMethod slot_2 = {"slotFileNameClicked", 0, 0 };
    static const QUMethod slot_3 = {"slotOpenFileButtonClicked", 0, 0 };
    static const QUMethod slot_4 = {"slotSearchAwardFilekPushButtonClicked", 0, 0 };
    static const QUMethod slot_5 = {"slotAddButtonClicked", 0, 0 };
    static const QUMethod slot_6 = {"slotAwardClearButtonClicked", 0, 0 };
    static const QUMethod slot_7 = {"slotAwardRemoveButtonClicked", 0, 0 };
    static const QUMethod slot_8 = {"slotAddDxClusterPushButtonClicked", 0, 0 };
    static const QUMethod slot_9 = {"slotClearDxClusterPushButtonClicked", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_10 = {"slotAwardRightButtonFromList", 2, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_11 = {"slotAwardSelected", 1, param_slot_11 };
    static const QUMethod slot_12 = {"slotAwardDelete", 0, 0 };
    static const QUParameter param_slot_13[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_13 = {"showMenuRightButton", 2, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out }
    };
    static const QUMethod slot_14 = {"getLastAwardNumber", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "tdxcluster", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"checkIfValidDXCluster", 2, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "tdxcluster", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"checkIfNewDXCluster", 2, param_slot_16 };
    static const QMetaData slot_tbl[] = {
	{ "setRig()", &slot_0, QMetaData::Public },
	{ "slotOkBtnClicked()", &slot_1, QMetaData::Private },
	{ "slotFileNameClicked()", &slot_2, QMetaData::Private },
	{ "slotOpenFileButtonClicked()", &slot_3, QMetaData::Private },
	{ "slotSearchAwardFilekPushButtonClicked()", &slot_4, QMetaData::Private },
	{ "slotAddButtonClicked()", &slot_5, QMetaData::Private },
	{ "slotAwardClearButtonClicked()", &slot_6, QMetaData::Private },
	{ "slotAwardRemoveButtonClicked()", &slot_7, QMetaData::Private },
	{ "slotAddDxClusterPushButtonClicked()", &slot_8, QMetaData::Private },
	{ "slotClearDxClusterPushButtonClicked()", &slot_9, QMetaData::Private },
	{ "slotAwardRightButtonFromList(QListViewItem*,const QPoint&)", &slot_10, QMetaData::Private },
	{ "slotAwardSelected(QListViewItem*)", &slot_11, QMetaData::Private },
	{ "slotAwardDelete()", &slot_12, QMetaData::Private },
	{ "showMenuRightButton(QListViewItem*,const QPoint&)", &slot_13, QMetaData::Private },
	{ "getLastAwardNumber()", &slot_14, QMetaData::Private },
	{ "checkIfValidDXCluster(const QString&)", &slot_15, QMetaData::Private },
	{ "checkIfNewDXCluster(const QString&)", &slot_16, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Setup", parentObject,
	slot_tbl, 17,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Setup.setMetaObject( metaObj );
    return metaObj;
}

void* Setup::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Setup" ) )
	return this;
    return setupui::qt_cast( clname );
}

bool Setup::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setRig(); break;
    case 1: slotOkBtnClicked(); break;
    case 2: slotFileNameClicked(); break;
    case 3: slotOpenFileButtonClicked(); break;
    case 4: slotSearchAwardFilekPushButtonClicked(); break;
    case 5: slotAddButtonClicked(); break;
    case 6: slotAwardClearButtonClicked(); break;
    case 7: slotAwardRemoveButtonClicked(); break;
    case 8: slotAddDxClusterPushButtonClicked(); break;
    case 9: slotClearDxClusterPushButtonClicked(); break;
    case 10: slotAwardRightButtonFromList((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 11: slotAwardSelected((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 12: slotAwardDelete(); break;
    case 13: showMenuRightButton((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 14: static_QUType_int.set(_o,getLastAwardNumber()); break;
    case 15: static_QUType_bool.set(_o,checkIfValidDXCluster((const QString&)static_QUType_QString.get(_o+1))); break;
    case 16: static_QUType_bool.set(_o,checkIfNewDXCluster((const QString&)static_QUType_QString.get(_o+1))); break;
    default:
	return setupui::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Setup::qt_emit( int _id, QUObject* _o )
{
    return setupui::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Setup::qt_property( int id, int f, QVariant* v)
{
    return setupui::qt_property( id, f, v);
}

bool Setup::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
