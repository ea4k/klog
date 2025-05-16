/***************************************************************************
                          dataproxy_sqlite.cpp  -  description
                             -------------------
    begin                : sept 2014
    copyright            : (C) 2014 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "dataproxy_sqlite.h"
#include "callsign.h"

//#include <QDebug>

DataProxy_SQLite::DataProxy_SQLite(const QString &_parentFunction, const QString &_softVersion)
{
    #ifdef QT_DEBUG
      //qDebug() << Q_FUNC_INFO << _softVersion << _parentFunction;
    #else
      //qDebug() << Q_FUNC_INFO << "Running a release build";
    #endif
    (void)_parentFunction;
    //qDebug() << Q_FUNC_INFO << ": " << _softVersion << _parentFunction;
    logLevel = None;
    //qDebug() << Q_FUNC_INFO << " - 45";
    util = new Utilities(Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << " - 46";
    util->setVersion(_softVersion);
    //qDebug() << Q_FUNC_INFO << " - 48: " << util->getKLogDBFile();
    db = new DataBase(Q_FUNC_INFO, _softVersion, util->getKLogDBFile());
    //qDebug() << Q_FUNC_INFO << " - 49";

    dbCreated = db->createConnection(Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << " - 50";

     //qDebug() << Q_FUNC_INFO << " - 51";
    //util->setSpecialCalls(getSpecialCallsigns());
     //qDebug() << Q_FUNC_INFO << " - 52";
    qso = new QSO;
    //qDebug() << Q_FUNC_INFO << " - 53";

    searching = false;
    executionN = 0;
    connect(db, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    //qDebug() << Q_FUNC_INFO << " - END";
    logEvent (Q_FUNC_INFO, "END", Debug);
}

DataProxy_SQLite::~DataProxy_SQLite()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    delete(util);
    delete(qso);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

int DataProxy_SQLite::getHowManyQSOPerPropMode(const QString &_p, const int _logn)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QSqlQuery query;
    QString queryString;
    bool sqlOK;

    if (_logn < 0)
    {
        queryString = "SELECT COUNT(DISTINCT id) FROM log WHERE prop_mode= :prop_mode";
        if (!query.prepare(queryString))
        {return 0;}
        query.bindValue(":prop_mode", _p);
    }
    else
    {
        queryString = "SELECT COUNT(DISTINCT id) FROM log WHERE lognumber= :lognumber AND prop_mode= :prop_mode";
        if (!query.prepare(queryString))
        {return 0;}
        query.bindValue(":lognumber", _logn);
        query.bindValue(":prop_mode", _p);
    }

    sqlOK = query.exec();

    if (sqlOK && query.next() && query.isValid())
    {
        int v = query.value(0).toInt();
        query.finish();
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return v;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return 0;
    }
}

QString DataProxy_SQLite::getSoftVersion()
{ //SELECT MAX (softversion) FROM softwarecontrolç
    logEvent (Q_FUNC_INFO, "Start", Debug);
    return util->getVersion();

    QSqlQuery query;
    QString stQuery = QString("SELECT MAX (softversion) FROM softwarecontrol");
    if (query.exec(stQuery))
    {
        query.next();
        if (query.isValid())
        {
            QString v = (query.value(0)).toString();
            query.finish();
               //qDebug() << Q_FUNC_INFO << " - DATA: " << v;
            if (v.length()<1)
            {
                //The following is not a query error but if the softwareversion value is lower than 0 or empty
                queryError(Q_FUNC_INFO, tr("Software version in DB is null"), "-1", tr("Query didn't failed")); // To alert about any failed query execution
            }
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return v;
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
    //logEvent (Q_FUNC_INFO, "END", Debug);
}

QString DataProxy_SQLite::getDBVersion()
{ //SELECT MAX (dbversion) FROM softwarecontrol
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QSqlQuery query;
    QString stQuery = QString("SELECT MAX (dbversion) FROM softwarecontrol");
    if (query.exec(stQuery))
    {
        query.next();
        if (query.isValid())
        {
            QString v = (query.value(0)).toString();
            query.finish();
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return v;
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
}

bool DataProxy_SQLite::reconnectDB()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return db->reConnect(util->getKLogDBFile());
}

void DataProxy_SQLite::createLogModel()
{
    logEvent (Q_FUNC_INFO, "Start-END **** EMPTY FUNCTION", Debug);
}

void DataProxy_SQLite::createLogPanel(){
    logEvent (Q_FUNC_INFO, "Start-END **** EMPTY FUNCTION", Debug);
}

int DataProxy_SQLite::getIdFromModeName(const QString& _modeName)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_modeName.length()<2)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return -4;
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return db->getModeIdFromSubMode(_modeName);
}

int DataProxy_SQLite::getSubModeIdFromSubMode(const QString &_subModeName)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);

    if (_subModeName.length()<2)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return -3;
    }
    QSqlQuery query;
    QString stQuery = QString("SELECT id FROM mode WHERE submode='%1'").arg(_subModeName.toUpper());
    if (query.exec(stQuery))
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return v;
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-4", Debug);
        return -2;
    }
    logEvent (Q_FUNC_INFO, "END-3", Debug);
    return -3;
}

int DataProxy_SQLite::getModeIdFromSubModeId(const int _sm)
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return getIdFromModeName(getNameFromSubMode(getSubModeFromId(_sm)));
}

bool DataProxy_SQLite::isModeDeprecated (const QString &_sm)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_sm.length()<2)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return -3;
    }
    QSqlQuery query;
    QString stQuery = QString("SELECT deprecated FROM mode WHERE submode='%1'").arg(_sm);
    if (query.exec(stQuery))
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(0)).toInt() == 1 )
            {
                query.finish();
                logEvent (Q_FUNC_INFO, "END-2", Debug);
                return true;
            }
            else
            {
                query.finish();
                logEvent (Q_FUNC_INFO, "END-3", Debug);
                return false;
            }
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-4", Debug);
            return false; // In case we can't check, we don't state it as deprecated
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-5", Debug);
        return false;   // In case we can't check, we don't state it as deprecated
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return false;
}

int DataProxy_SQLite::getIdFromBandName(const QString& _bandName)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_bandName.length()<1)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return -4;
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return db->getBandIdFromName(_bandName);
}

QString DataProxy_SQLite::getNameFromBandId (const int _id)
{
    logEvent (Q_FUNC_INFO, "Start-END", Debug);
    return db->getBandNameFromNumber(_id);
}

QString DataProxy_SQLite::getNameFromModeId (const int _id)
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return db->getModeNameFromNumber(_id);
}

QString DataProxy_SQLite::getNameFromSubModeId (const int _id)
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return db->getSubModeNameFromNumber(_id);
}

QString DataProxy_SQLite::getSubModeFromId (const int _id)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QSqlQuery query;
    QString queryString = QString("SELECT submode FROM mode WHERE id='%1'").arg(_id);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            QString v = (query.value(0)).toString();
            query.finish();
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return v;
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
    //logEvent (Q_FUNC_INFO, "END", Debug);
}

QString DataProxy_SQLite::getNameFromSubMode (const QString &_sm)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QSqlQuery query;
    QString queryString = QString("SELECT name FROM mode WHERE submode='%1'").arg(_sm.toUpper());
    //QString queryString = QString("SELECT name, deprecated FROM mode WHERE submode='%1'").arg(_sm.toUpper());
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            QString v = (query.value(0)).toString();
            query.finish();
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return v;
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
}

Frequency DataProxy_SQLite::getFreqFromBandId(const int _id)
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return db->getFreqFromBandId(_id);
}

int DataProxy_SQLite::getBandIdFromFreq(const double _n)
{
    //Freq should be in MHz
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_n);
    bool sqlOk = false;
    QString queryString = QString("SELECT id FROM band WHERE lower <= :freq and upper >= :freq");

    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":freq", _n);
    sqlOk = query.exec();

     //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();
    if (sqlOk)
    {
        //qDebug() << Q_FUNC_INFO << " - Query OK";
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return v;
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return -1;
        }
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - Query NOK";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return -2;
    }
}

QString DataProxy_SQLite::getBandNameFromFreq(const double _n)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    bool sqlOk = false;
    QString queryString = QString("SELECT name FROM band WHERE lower <= :freq and upper >= :freq");

    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":freq", _n);

    sqlOk = query.exec();

    if (sqlOk)
    {
        query.next();
        if (query.isValid())
        {
            QString b = (query.value(0)).toString();
            query.finish();
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return b;
        }
        else
        {
            query.finish();
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
}

double DataProxy_SQLite::getLowLimitBandFromBandName(const QString &_sm)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_sm.length ()<2)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return -1.0;
    }

    QString queryString = QString("SELECT lower FROM band WHERE name= :sm OR name= :name");
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":sm", _sm);
    query.bindValue(":name", _sm.toUpper());

    bool sqlOK = query.exec();

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            double fr = (query.value(0)).toDouble();
            query.finish();
            if ( fr < 0 )
            {
                logEvent (Q_FUNC_INFO, "END-2", Debug);
                return -1.0;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << " -(value found): " << QString::number(fr);
                logEvent (Q_FUNC_INFO, "END-2", Debug);
                return fr;
            }
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - -1.0-2";
            query.finish();
            logEvent (Q_FUNC_INFO, "END-3", Debug);
            return -1.0;
        }
        //qDebug() << Q_FUNC_INFO << " - -1.0-3";
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - SQL Error";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-4", Debug);
        return -1.0;
    }
}

double DataProxy_SQLite::getLowLimitBandFromBandId(const int _sm)
{
         //qDebug() << Q_FUNC_INFO << " - Start";
    QString queryString = QString("SELECT lower FROM band WHERE id= :sm");
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":sm", _sm);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(1)).toDouble()<0 )
            {
                     //qDebug() << Q_FUNC_INFO << " -1.0-1";
                query.finish();
                return -1.0;
            }
            else
            {
                     //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toDouble());
                double v = (query.value(0)).toDouble();
                query.finish();
                return v;
            }
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << "  -1.0-2";
            query.finish();
            return -1.0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1.0;
    }
}

double DataProxy_SQLite::getUpperLimitBandFromBandName(const QString &_sm)
{
    //qDebug() << Q_FUNC_INFO << ": " << _sm;
    if (_sm.length ()<2)
    {
        return -1.0;
    }
    QSqlQuery query;
    QString queryString = QString("SELECT upper FROM band WHERE name='%1' OR name='%2'").arg(_sm).arg(_sm.toUpper());
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(0)).toDouble()<0 )
            {
                //qDebug() << Q_FUNC_INFO << ": -1.0-1";
                query.finish();
                return -1.0;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": (else): " << QString::number((query.value(0)).toDouble());
                double v = (query.value(0)).toDouble();
                query.finish();
                return v;
            }
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << ": -1.0-2";
            query.finish();
            return -1.0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1.0;
    }
}

bool DataProxy_SQLite::isThisFreqInBand(const QString &_band, const QString &_fr)
{
    return db->isThisFreqInBand(_band, _fr);
}

QStringList DataProxy_SQLite::getFields()
{
    //qDebug() << Q_FUNC_INFO;
    QStringList fields;
    fields.clear();
    QSqlQuery q;
    QSqlRecord localRecord = q.driver()->record("log");
    for (int var = 0; var < localRecord.count(); ++var) {
        QString fieldName = localRecord.fieldName(var);
        fields << fieldName;
        //qDebug() << fieldName;
    }
    return fields;
}

QStringList DataProxy_SQLite::getBandNames()
{
    //qDebug() << Q_FUNC_INFO;
    QStringList bands = QStringList();
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    queryString = QString("SELECT DISTINCT name FROM band");
    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                queryString = (query.value(0)).toString();
                //qDebug() << Q_FUNC_INFO << " : " << queryString;
                bands.append(queryString);
            }
            else
            {
                query.finish();
                return QStringList();
            }
        }
        query.finish();
        return sortBandNamesBottonUp(bands);
        //return bands;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }
}

QStringList DataProxy_SQLite::getModes()
{
    QStringList modes = QStringList();

    QSqlQuery query("SELECT submode FROM mode ORDER BY submode");

    while (query.next()) {
        if (query.isValid()){
            modes << query.value(0).toString();
        }
    }
    query.finish();
    modes.sort();
    return modes;
}

QStringList DataProxy_SQLite::sortBandNamesBottonUp(const QStringList _qs)
{
    //Receives a list of band names, sorts it from the lower band to the upper band and returns
      //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qs.length());
    if (_qs.length()<2)
    {
        return _qs;
    }

    QMap<double, QString> map;
    map.clear();
    QStringList qs;
    qs.clear();
      //qDebug() << Q_FUNC_INFO << " : 00";

    for (int j=0; j<_qs.count(); j++)
    {
        map.insert(getLowLimitBandFromBandName(_qs.at(j)), _qs.at(j));
    }
      //qDebug() << Q_FUNC_INFO << " : 10";
    QMap<double, QString>::const_iterator i = map.constBegin();
      //qDebug() << Q_FUNC_INFO << " : 20";
    while (i != map.constEnd()) {
        qs << i.value();
        ++i;
    }

      //qDebug() << Q_FUNC_INFO << " :  END -lengh = " << QString::number(qs.length());
    qs.removeDuplicates();
    return qs;
}

QStringList DataProxy_SQLite::getBandIDs()
{
    //qDebug() << Q_FUNC_INFO;
    QStringList bands = QStringList();
    QSqlQuery query("SELECT id FROM band");

    while (query.next()) {
        if (query.isValid()){
            bands << query.value(0).toString();
        }
    }
    query.finish();
    return sortBandIdBottonUp(bands);
}

QStringList DataProxy_SQLite::sortBandIdBottonUp(const QStringList _qs)
{
    //Receives a list of band id, sorts it from the lower band to the upper band and returns
    QMap<double, QString> map;
    QStringList qs;
    qs.clear();


    for (int j=0; j<_qs.count(); j++)
    {
        map.insert(getLowLimitBandFromBandId(_qs.at(j).toInt()), _qs.at(j));
    }

    QMap<double, QString>::const_iterator i = map.constBegin();

    while (i != map.constEnd()) {
        qs << i.value();
        ++i;
    }

    return qs;
}

QStringList DataProxy_SQLite::getModesIDs()
{
    QStringList modes = QStringList();
    QSqlQuery query("SELECT id FROM mode");

    while (query.next()) {
        if (query.isValid()){
            modes << query.value(0).toString();
        }
    }
    query.finish();
    return modes;
}

QStringList DataProxy_SQLite::getBandsInLog(const int _log)
{
         //qDebug() << Q_FUNC_INFO ;
    QStringList bands = QStringList();
    QString queryString = QString();
    if (_log <= 0)
    {
        queryString = QString("SELECT DISTINCT band.name FROM log, band WHERE band.id = log.bandid ORDER BY band.id DESC");
    }
    else
    {
        queryString = QString("SELECT DISTINCT band.name FROM log, band WHERE band.id = log.bandid AND log.lognumber='%1' ORDER BY band.id DESC").arg(_log);
    }

    QSqlQuery query(queryString);

    while (query.next()) {
        if (query.isValid()){
            bands << query.value(0).toString();
        }
    }
    query.finish();
    return sortBandNamesBottonUp(bands);
    //return bands;
}

QStringList DataProxy_SQLite::getModesInLog(const int _log)
{
       //qDebug() << Q_FUNC_INFO << " - Start ";
    QStringList modes = QStringList();
    QString queryString = QString();
    if (_log <=0 )
    {
        queryString = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid GROUP BY mode.submode  ORDER BY count (mode.submode) DESC");
    }
    else
    {
        queryString = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid AND log.lognumber='%1' GROUP BY mode.submode  ORDER BY count (mode.submode) DESC").arg(_log);
    }

    QSqlQuery query(queryString);

    while (query.next()) {
        if (query.isValid()){
            modes << query.value(1).toString();
        }
    }
    query.finish();
        //qDebug() << Q_FUNC_INFO << " - " << modes.join(" - ");
    modes.sort();
    return modes;
}

int DataProxy_SQLite::getMostUsedBand(const int _log)
{
       //qDebug() << Q_FUNC_INFO << " - Start ";

    QString queryString = QString();
    if (_log <=0 )
    {
        queryString = QString("SELECT band.id, band.name, COUNT (band.name) FROM log, band WHERE band.id = log.bandid GROUP BY band.id  ORDER BY count (band.id) DESC LIMIT 1");
    }
    else
    {
        queryString = QString("SELECT band.id, band.name, COUNT (band.name) FROM log, band WHERE band.id = log.bandid AND log.lognumber='%1' GROUP BY band.id  ORDER BY count (band.id) DESC LIMIT 1").arg(_log);
    }
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                int v = query.value(0).toInt();
                query.finish();
                   //qDebug() << Q_FUNC_INFO << " -OK: " << QString::number(v)<< QT_ENDL;
                return v;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
           //qDebug() << Q_FUNC_INFO << " --ERROR-1: ";
        return -1;
    }
       //qDebug() << Q_FUNC_INFO << " --ERROR-2: ";
    return -2;
}

int DataProxy_SQLite::getMostUsedMode(const int _log)
{
      //qDebug() << Q_FUNC_INFO << " - Start ";

    QString queryString = QString();
    if (_log <=0 )
    {
        queryString = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid GROUP BY mode.submode  ORDER BY count (mode.submode) DESC LIMIT 1");
    }
    else
    {
        queryString = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid AND log.lognumber='%1' GROUP BY mode.submode  ORDER BY count (mode.submode) DESC LIMIT 1").arg(_log);
    }
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                int v = query.value(0).toInt();
                query.finish();
                return v;
            }
        }
        query.finish();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
    return -2;
}

int DataProxy_SQLite::getLastQSOid()
{
         //qDebug() << Q_FUNC_INFO << " - Start";
    QSqlQuery query;
    bool sqlOK = query.exec("SELECT MAX(id) from log");

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = query.value(0).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
}

QDate DataProxy_SQLite::getFirstQSODateFromCall (const QString &_call)
{
    //qDebug() << Q_FUNC_INFO << ": " << _call;

    QSqlQuery query;
    QString stringQuery;
    QDate _date;
    Callsign callsign(_call);
    if (callsign.isValid())
    {
        stringQuery = QString("SELECT qso_date from log where station_callsign='%1' ORDER BY qso_date ASC LIMIT 1").arg(_call);
    }
    else if (_call == "NOT")
    {
        stringQuery = QString("SELECT qso_date from log where ((station_callsign IS NULL) OR (station_callsign='')) ORDER BY qso_date ASC LIMIT 1");
    }
    else
    { // ALL
        stringQuery = QString("SELECT qso_date from log where 1 ORDER BY qso_date ASC LIMIT 1");
    }

    bool sqlOK = query.exec(stringQuery);
    //qDebug() << Q_FUNC_INFO << ": " << query.lastQuery();

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            _date = util->getDateFromSQliteString((query.value(0)).toString());
            //_date = QDate::fromString((query.value(0)).toString(), "yyyy-MM-dd");
            //stringQuery = query.value(0).toString();
            query.finish();
            if (_date.isValid())
            {
                //qDebug() << Q_FUNC_INFO << ": END OK" ;
                return _date;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": END-1 " ;
                return QDate();
            }
        }
        else
        {
            query.finish();
            //qDebug() << Q_FUNC_INFO << ": END-2" ;
            return QDate();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        //qDebug() << Q_FUNC_INFO << ": END-3" ;
        return QDate();
    }
}

QDate DataProxy_SQLite::getLastQSODateFromCall (const QString &_call)
{
    //qDebug() << Q_FUNC_INFO << ": " << _call;
    QSqlQuery query;
    QString stringQuery;
    QDate _date;
    Callsign callsign(_call);
    if (callsign.isValid())
    {
        stringQuery = QString("SELECT qso_date from log where station_callsign='%1' ORDER BY qso_date DESC LIMIT 1").arg(_call);
    }
    else if (_call == "NOT")
    {
        stringQuery = QString("SELECT qso_date from log where ((station_callsign IS NULL) OR (station_callsign='')) ORDER BY qso_date DESC LIMIT 1");
    }
    else
    { // ALL
        stringQuery = QString("SELECT qso_date from log where 1 ORDER BY qso_date DESC LIMIT 1");
    }
    bool sqlOK = query.exec(stringQuery);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            _date = util->getDateFromSQliteString((query.value(0)).toString());
            //_date = QDate::fromString((query.value(0)).toString(), "yyyy-MM-dd");
            //stringQuery = query.value(0).toString();
            query.finish();
            if (_date.isValid())
            {
                //qDebug() << Q_FUNC_INFO << ": OK";
                return _date;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": END-1";
                return QDate();
            }
        }
        else
        {
            query.finish();
            //qDebug() << Q_FUNC_INFO << ": END-2";
            return QDate();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        //qDebug() << Q_FUNC_INFO << ": END-3";
        return QDate();
    }
}

bool DataProxy_SQLite::clearLog()
{
         //qDebug() << Q_FUNC_INFO << " - Start";
    //int errorCode = 0;
    QSqlQuery query;
    bool sqlOK = query.exec("DELETE FROM log");

    if (sqlOK)
    {
             //qDebug() << Q_FUNC_INFO << " - Log deleted!";
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() <<Q_FUNC_INFO << " - Log deleted FAILED";
        //errorCode = query.lastError().text();

             //qDebug() <<Q_FUNC_INFO << " - LastQuery: " << query.lastQuery() ;
             //qDebug() <<Q_FUNC_INFO << " - LastError-data: " << query.lastError().databaseText() ;
             //qDebug() <<Q_FUNC_INFO << " - LastError-driver: " << query.lastError().driverText() ;
             //qDebug() <<Q_FUNC_INFO << " - LastError-n: " << QString::number(query.lastError().text() );
    }
    query.finish();

    if (query.isActive())
    {
             //qDebug() <<Q_FUNC_INFO << " - Query Active!";
        query.finish();
        return false;
    }
    else
    {
        query.prepare("VACUUM;");

             //qDebug() <<Q_FUNC_INFO << " - Query Not Active!";
        if (query.exec())
        {
                 //qDebug() <<Q_FUNC_INFO << " - VACUUM OK!";
            query.finish();
            return true;
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                 //qDebug() <<Q_FUNC_INFO << " - VACUUM FAILED";
            //errorCode = query.lastError().text();
                 //qDebug() << Q_FUNC_INFO << " - - query error: " << QString::number(query.lastError().text());
                 //qDebug() <<Q_FUNC_INFO << " - LastQuery: " << query.lastQuery() ;
                 //qDebug() <<Q_FUNC_INFO << " - LastError-data: " << query.lastError().databaseText() ;
                 //qDebug() <<Q_FUNC_INFO << " - LastError-driver: " << query.lastError().driverText() ;
                 //qDebug() <<Q_FUNC_INFO << " - LastError-n: " << QString::number(query.lastError().text() );
        }
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslSentViaDirect(const int _qsoId, const QDate &_updateDate)
{
         //qDebug() <<Q_FUNC_INFO << " -";
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'D', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() <<Q_FUNC_INFO << " -" << queryString;
    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    return false;
}

bool DataProxy_SQLite::qslSentViaBureau(const int _qsoId, const QDate &_updateDate)
{
    //qDebug() << Q_FUNC_INFO << " -";

    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'B', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);

    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " - - OK";
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        //qDebug() << Q_FUNC_INFO << " - - NOK";
    }
    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QDate &_updateDate)
{
         //qDebug() <<Q_FUNC_INFO << " - " << QString::number (_qsoId) << "/" << _updateDate;
    QSqlQuery query;
    QString queryString;


    queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
             //qDebug() << Q_FUNC_INFO << " -: TRUE";
        //setWAZAwardStatus(_qsoId);
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
         //qDebug() << Q_FUNC_INFO << " - FALSE";
    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QDate &_updateDate, const bool _queueSentQSL)
{
         //qDebug() << Q_FUNC_INFO << " - " << _updateDate;
    QSqlQuery query;
    QString queryString;
    //bool requestQSL = false;
    bool sqlOK;

    if  (_queueSentQSL)
    {
        queryString = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);

        sqlOK = query.exec(queryString);

        if (sqlOK)
        {
            query.next();
            if (query.isValid())
            {
                queryString = (query.value(0)).toString();
                if ((queryString == "Y") || (queryString == "R"))
                {
                    // NO ACTION REQUIRED, QSL IS ALREADY SENT
                          //qDebug() << Q_FUNC_INFO << " - QSL already requested";
                     //requestQSL = false;
                     queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
                else
                {
                         //qDebug() << Q_FUNC_INFO << " - Request QSL-1";
                    //requestQSL = true;
                    queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
            }
            else
            {
                     //qDebug() << Q_FUNC_INFO << " -Request QSL-2";
                queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                //requestQSL = true;
            }
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << " -Request QSL-3";
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
            //requestQSL = true;
        }
    }
    else
    {
        //requestQSL = false;
        queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
    }
    query.finish();
    sqlOK = query.exec(queryString);

            //queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
    if (sqlOK)
    {
             //qDebug() << Q_FUNC_INFO << " - TRUE";
        query.finish();
        //setDXCCAwardStatus(_qsoId);
        //setWAZAwardStatus(_qsoId);

        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }

         //qDebug() << Q_FUNC_INFO << " - FALSE";
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QDate &_updateDate)
{
         //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.finish();
        //setDXCCAwardStatus(_qsoId);
        //setWAZAwardStatus(_qsoId);
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QDate &_updateDate, const bool _queueSentQSL)
{
         //qDebug() << Q_FUNC_INFO << " - " << _updateDate;
    QSqlQuery query;
    QString queryString;
    bool sqlOK;

    if  (_queueSentQSL)
    {
        queryString = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
        sqlOK = query.exec(queryString);

        if(sqlOK)
        {
            query.next();
            if (query.isValid())
            {
                queryString = (query.value(0)).toString();
                if ((queryString == "Y") || (queryString == "R"))
                {
                    // NO ACTION REQUIRED, QSL IS ALREADY SENT
                          //qDebug() << Q_FUNC_INFO << " - QSL already requested";
                     queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
                else
                {
                         //qDebug() << Q_FUNC_INFO << " - Request QSL-1";
                    queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
            }
            else
            {
                     //qDebug() << Q_FUNC_INFO << " - Request QSL-2";
                queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                 //qDebug() << Q_FUNC_INFO << " - Request QSL-3";
            queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
        }
    }
    else
    {
        queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
    }
    query.finish();
    sqlOK = query.exec(queryString);


    if (sqlOK)
    {
             //qDebug() << Q_FUNC_INFO << " - TRUE";
        query.finish();
        //setDXCCAwardStatus(_qsoId);
        //setWAZAwardStatus(_qsoId);
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }

         //qDebug() << Q_FUNC_INFO << " - FALSE";
    query.finish();
    return false;
}


bool DataProxy_SQLite::qslSentAsRequested(const int _qsoId, const QDate &_updateDate)
{
    //TODO: Add some protection to the data before modifying
         //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_sent = 'R', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << Q_FUNC_INFO << " - " << queryString;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
             //qDebug() << Q_FUNC_INFO << " -";
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslRecAsRequested(const int _qsoId, const QDate &_updateDate)
{
//TODO: Add some protection to the data before modifying
    //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_rcvd = 'R', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << Q_FUNC_INFO << " - " << queryString;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " - OK";
        query.finish();
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - NOK";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::setClubLogSent(const int _qsoId, const QString &_st, const QDate &_updateDate)
{ // Updates the QSO with the ClubLog status & date
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET clublog_qso_upload_status = '%1', clublog_qso_upload_date = '%2' WHERE id = '%3'").arg(_st).arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << Q_FUNC_INFO << " - " << queryString;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
             //qDebug() << Q_FUNC_INFO << " - - TRUE";
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
         //qDebug() << Q_FUNC_INFO << " - - FALSE";
    query.finish();
    return false;
}

bool DataProxy_SQLite::setLoTWQSLRec (const int _qsoId, const QString &_st, const QDate &_updateDate)
{
    QSqlQuery query;
    QString queryString;
    if (!util->isValidQSL_Rcvd(_st))
    {
        return false;
    }
    if (!_updateDate.isValid())
    {
        return false;
    }
    queryString = QString("UPDATE log SET lotw_qsl_rcvd = '%1', lotw_qslrdate = '%2' WHERE id = '%3'").arg(_st).arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << Q_FUNC_INFO << " -: " << queryString;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " - - TRUE";
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
         //qDebug() << Q_FUNC_INFO << " - - FALSE";
    query.finish();
    return false;
}

bool DataProxy_SQLite::isQSOConfirmed(const int _qsoId, const bool _checkPaper, const bool _checkLoTW)
{ // Returns true if the QSO has been confirmed via paper and/or LoTW
    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE id = '%1'").arg(_qsoId);

    if (!query.exec(queryString))
    {
        return false;
    }
    if (!query.next ())
    {
        return false;
    }
    if (!query.isValid ())
    {
        return false;
    }

    QString paperQSL = (query.value(0)).toString();
    QString lotwQSL = (query.value(1)).toString();
    query.finish();

    if (_checkPaper && _checkLoTW)
    {
        return ((paperQSL == "Y") || (lotwQSL == "Y"));
    }
    else if (_checkPaper && !_checkLoTW)
    {
        return (paperQSL == "Y");
    }
    else if (!_checkPaper && _checkLoTW)
    {
        return (lotwQSL == "Y");
    }
    else
    {
        return false;
    }
}

bool DataProxy_SQLite::isQSLReceived(const int _qsoId)
{
         //qDebug() << Q_FUNC_INFO << " -" << QString::number(_qsoId);
    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT qsl_rcvd FROM log WHERE id = '%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            queryString = (query.value(0)).toString();
            query.finish();
            if (queryString == "Y")
            {
                     //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qsoId) << "QSL Received";
                return true;
            }
            else
            {
                     //qDebug() << Q_FUNC_INFO << " -: " << QString::number(_qsoId) << "QSL NOT Received-1";
                return false;
            }
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << " -: " << QString::number(_qsoId) << "QSL NOT Received-2";
            query.finish();
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    return false;
}

bool DataProxy_SQLite::isQSLSent(const int _qsoId)
{
         //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qsoId);
    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            queryString = (query.value(0)).toString();
            query.finish();
            if (queryString == "Y")
            {
                     //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qsoId) << "QSL Sent";
                return true;
            }
            else
            {
                     //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qsoId) << "QSL NOT Sent-1";
                return false;
            }
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qsoId) << "QSL NOT Sent-2";
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    //return false;
}

int DataProxy_SQLite::getBandFromId(const int _qsoId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT bandid FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    return -1;
}

int DataProxy_SQLite::getModeFromId(const int _qsoId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT modeid FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    return -1;
}

int DataProxy_SQLite::getDXCCFromId(const int _qsoId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT dxcc FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    return -1;
}

int DataProxy_SQLite::getCQZFromId(const int _qsoId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT cqz FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    return -1;
}

QList<int> DataProxy_SQLite::getBandModeDXCCCQZlogIDFromId(const int _qsoId)
{
    QList<int> result;
    result.clear();


    QSqlQuery query;
    query.prepare("SELECT bandid, modeid, dxcc, cqz, lognumber FROM log WHERE id = :id");
    query.bindValue(":id", _qsoId);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish ();
        return result;
    }

    if (!query.next() || !query.isValid()) {
          query.finish();
          return result;
    }

    QSqlRecord rec = query.record();
    int nameCol = rec.indexOf("bandid");

    int v = (query.value(nameCol)).toInt();
    if (v<1)
    {
        return result;
    }
    result.append (v);

    nameCol = rec.indexOf("modeid");
    v = (query.value(nameCol)).toInt();
    if (v<1)
    {
        result.clear();
        return result;
    }
    result.append (v);

    nameCol = rec.indexOf("dxcc");
    v = (query.value(nameCol)).toInt();

    if (v<1)
    {
        result.clear();
        return result;
    }
    result.append (v);

    nameCol = rec.indexOf("cqz");
    v = (query.value(nameCol)).toInt();

    if (v<1)
    {
        result.clear();
        return result;
    }
    result.append (v);

    nameCol = rec.indexOf("lognumber");
    v = (query.value(nameCol)).toInt();

    if (v<1)
    {
        result.clear();
        return result;
    }
    result.append (v);

    query.finish ();
    return result;
}

QString DataProxy_SQLite::getCallFromId(const int _qsoId)
{
         //qDebug() << Q_FUNC_INFO << " -;
    QSqlQuery query;
    QString queryString = QString("SELECT call FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            QString v = (query.value(0)).toString();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    return QString();
}


QStringList DataProxy_SQLite::getClubLogRealTimeFromId(const int _qsoId)
{
    //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qsoId);
/* Return a QStringList with 16 fields with these data:

QSO_DATE, TIME_ON, QSLRDATE, QSLSDATE, CALL, OPERATOR, MODE, BAND, BAND_RX, FREQ, QSL_RCVD,
LOTW_QSL_RCVD, QSL_SENT, DXCC, PROP_MODE, CREDIT_GRANTED

*/
    QSqlQuery query;
    int nameCol = -1;
    QStringList dataC = QStringList();
    //QString aux1 = QString();
    //QString aux2 = QString();
    //QString call = QString();

// IMPORTANT: band_rx is not always present, and if it is not present, the query with INNER JOIN will fail.
// To fix that we will do two queries, one to check if I have all the data and if not another one with a reduced scope.
    QString queryString = QString("SELECT band_rx FROM log WHERE id='%1'").arg(_qsoId);

    bool sqlOk = query.exec(queryString);
    bool haveBandRX = false;
    if (sqlOk)
    {
        //qDebug() << Q_FUNC_INFO << " - sqlOK -1";
        if (query.next())
        {
            //qDebug() << Q_FUNC_INFO << " - qsl next -1";
            if (query.isValid())
            {
                //qDebug() << Q_FUNC_INFO << " - sql valid -1";
                QSqlRecord rec = query.record();
                nameCol = rec.indexOf("band_rx");
                QString aux = (query.value(nameCol)).toString();
                if (aux.length()<1)
                { // We DON'T have a band_rx available
                    query.finish();
                    //qDebug() << Q_FUNC_INFO << " - without BAND-RX";
                    queryString = QString("SELECT qso_date, qslrdate, qslsdate, call, station_callsign, operator, M.name, B.name, freq, qsl_rcvd, lotw_qsl_rcvd, qsl_sent, dxcc, prop_mode, credit_granted FROM log INNER JOIN band as B ON bandid = B.id INNER JOIN mode as M ON modeid = M.id WHERE log.id='%1'").arg(_qsoId);
                }
                else {
                    haveBandRX = true;
                    queryString = QString("SELECT qso_date, qslrdate, qslsdate, call, station_callsign, operator, M.name, B.name, R.name, freq, qsl_rcvd, lotw_qsl_rcvd, qsl_sent, dxcc, prop_mode, credit_granted FROM log INNER JOIN band as B ON bandid = B.id INNER JOIN band as R ON band_rx = R.id INNER JOIN mode as M ON modeid = M.id WHERE log.id='%1'").arg(_qsoId);
                }
            }
            else
            { // We have a band_rx available
                query.finish();
                //qDebug() << Q_FUNC_INFO << " - NO VALID";
                return QStringList();
            }
        }
        else
        {
            query.finish();
            //qDebug() << Q_FUNC_INFO << " - ERROR-1";
            return QStringList();
        }
    }
    else
    {
        query.finish();
        //qDebug() << Q_FUNC_INFO << " - ERROR-2";
        return QStringList();
    }

    sqlOk = query.exec(queryString);
    dataC << QString::number(_qsoId);
    //qDebug() << Q_FUNC_INFO << " - lastQuery: " << query.lastQuery();

    if (sqlOk)
    {
        //qDebug() << Q_FUNC_INFO << " - sqlOK-1";
        if (query.next())
        {
            //qDebug() << Q_FUNC_INFO << " - query next = OK";
            if (query.isValid())
            {
                QSqlRecord rec = query.record();
                //qDebug() << Q_FUNC_INFO << " - query valid = OK";
                QString aux;
                nameCol = rec.indexOf("qso_date");
                aux = util->getADIFDateFromQDateTime(util->getDateTimeFromSQLiteString((query.value(nameCol)).toString()));
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << Q_FUNC_INFO << " - ERROR-3: " << aux;
                    return QStringList();
                }
                dataC << aux;
                aux = util->getADIFTimeFromQDateTime(util->getDateTimeFromSQLiteString((query.value(nameCol)).toString()));
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << Q_FUNC_INFO << " - ERROR-4: " << aux;
                    return QStringList();
                }
               //qDebug() << Q_FUNC_INFO << " - Time_on: " << aux;
                dataC << aux;
                //dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("qslrdate");
                dataC << util->getADIFDateFromQDate( util->getDateFromSQliteString((query.value(nameCol)).toString()) );
                //dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("qslsdate");
                dataC << util->getADIFDateFromQDate( util->getDateFromSQliteString((query.value(nameCol)).toString()) );
                //dataC << (query.value(nameCol)).toString();

                nameCol = rec.indexOf("call");
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << Q_FUNC_INFO << " - ERROR-5: " << aux;
                    return QStringList();
                }
                QString call = (query.value(nameCol)).toString();
                dataC << call;

                nameCol = rec.indexOf("operator");
                dataC << (query.value(nameCol)).toString();

                //nameCol = rec.indexOf("M.name");   //MODE       //TODO: Fix this to get the proper column
                aux = (query.value(6)).toString();
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << Q_FUNC_INFO << " - ERROR-6: " << aux;
                    return QStringList();
                }
                dataC << aux;

                //nameCol = rec.indexOf("B.name"); // BAND
                aux = (query.value(7)).toString();
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << Q_FUNC_INFO << " - ERROR-7: " << aux;
                    return QStringList();
                }
                dataC << aux;

                if (haveBandRX)
                {
                    dataC << (query.value(8)).toString();
                }
                else
                {
                    dataC << "";
                }
                //nameCol = rec.indexOf("R.name"); //BAND_RX    //TODO: Fix this to get the proper column (use an index instead of a number)

                nameCol = rec.indexOf("freq");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("qsl_rcvd");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("lotw_qsl_rcvd");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("qsl_sent");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("dxcc");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("prop_mode");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("credit_granted");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("station_callsign");
                QString aux2 = (query.value(nameCol)).toString();

                Callsign callsign(aux2);
                if (callsign.isValid())
                {
                    dataC <<  aux2;
                }
                else
                {
                    dataC << call;
                }

                //qDebug() << Q_FUNC_INFO << " - RETURNING ... OK";
                query.finish();
                return dataC;
            }
            else
            {
                //NO VALID
                //qDebug() << Q_FUNC_INFO << " - NO VALID NOT OK";
                query.finish();
                return QStringList();
            }
        }
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - NOT sqlOK";
        //qDebug() << Q_FUNC_INFO << " - 2 LastQuery: " << query.lastQuery() ;
        //qDebug() << Q_FUNC_INFO << " - 2 LastError-data: " << query.lastError().databaseText() ;
        //qDebug() << Q_FUNC_INFO << " - 2 LastError-driver: " << query.lastError().driverText() ;
        //qDebug() << Q_FUNC_INFO << " - 2 LastError-n: " << QString::number(query.lastError().text() );
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }
    //qDebug() << Q_FUNC_INFO << " - ERROR-10";
    return QStringList();
}

QStringList DataProxy_SQLite::getFilteredLocators(const QString &_band, const QString &_mode, const QString &_prop, const QString &_sat, bool _confirmed)
{
    //qDebug() << Q_FUNC_INFO << ": " << _band;
    //qDebug() << Q_FUNC_INFO << ": " << _mode;
    //qDebug() << Q_FUNC_INFO << ": " << _prop;
    //qDebug() << Q_FUNC_INFO << ": " << _sat;

    QStringList grids = QStringList();
    QSqlQuery query;
    QString queryString;

    QString bandString = QString();
    int bandId = getIdFromBandName(_band);

    if (util->isValidBandId(bandId))
    {
        bandString = QString("bandid = '%1'").arg(bandId);
    }
    else
    {
        bandString = QString("bandid <> ''");
    }

    QString modeString = QString();
    int modeId = getIdFromModeName(_mode);
    if (util->isValidModeId(modeId))
    {
        modeString = QString("AND modeid = '%1'").arg(modeId);
    }
    else
    {
        modeString = QString("AND modeid <> '' ");
    }

    QString propString = QString();
    QString satsString = QString();

    if (isValidPropMode(_prop))
    {
        propString = QString("AND prop_mode = '%1'").arg(_prop);
        if (_prop == "SAT")
        {
            //qDebug() << Q_FUNC_INFO  << ": SAT: " << _sat;
            if (getDBSatId(_sat)>0)
            {
                satsString = QString("AND sat_name = '%1'").arg(_sat);
            }
            else
            {
                satsString = QString("AND sat_name <> 'x'");
            }
        }
        else
        {
            satsString = QString("AND sat_name = ''");
        }
    }
    else
    {
        propString = QString("AND prop_mode <> 'x'");
        satsString = QString("AND sat_name = ''");
    }

    QString confirmedString = QString();
    if (_confirmed)
    {
        confirmedString = QString("AND ((qsl_rcvd = 'Y') OR (lotw_qsl_rcvd = 'Y'))");
    }
    else
    {
        confirmedString = QString("AND qsl_rcvd <> 'x' ");
    }

    queryString = QString("SELECT DISTINCT gridsquare from log WHERE %1 %2 %3 %4 %5 ORDER BY id ASC").arg(bandString).arg(modeString).arg(propString).arg(satsString).arg(confirmedString);

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << queryString ;
        while(query.next())
        {
            if (query.isValid())
            {
                queryString = (query.value(0)).toString();
                grids.append(queryString);
                if (grids.contains ("IN99"))
                {
                    //qDebug() << Q_FUNC_INFO << ": " << queryString ;
                }
            }
            else
            {
                query.finish();
                return QStringList();
            }
        }
        query.finish();
        grids.sort();
        return grids;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }
}

bool DataProxy_SQLite::QRZCOMModifyFullLog(const int _currentLog)
{
    //qDebug() << Q_FUNC_INFO << " -" << QString::number(_currentLog);

    if (!doesThisLogExist(_currentLog))
    {
        return false;
    }
    QString queryString;
    bool sqlOK;
    QSqlQuery query;

   queryString = QString("UPDATE log SET qrzcom_qso_upload_status = 'M', qrzcom_qso_upload_date = '%1'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate()));
   sqlOK = query.exec(queryString);
   query.finish();
   if (sqlOK)
   {
       //qDebug() << " DataProxy_SQLite::QRZCOMModifyFullLog: END TRUE";
       return true;
   }
   else
   {
       emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
       //qDebug() << Q_FUNC_INFO << " - END FALSE: " << query.lastError().databaseText() ;
       return false;
   }

    //qDebug() << Q_FUNC_INFO << " - END TRUE" ;
   //return false;
}

bool DataProxy_SQLite::QRZCOMSentQSOs(const QList<int> &_qsos)
{
    //qDebug() << Q_FUNC_INFO << " -" << QString::number(_qsos.count());
    if (_qsos.count() < 1)
    {
        return true;
    }
    QString queryString;
    bool sqlOK;
    QSqlQuery query;

    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << Q_FUNC_INFO << " - updating QSO: " << QString::number(_qsos.at(i));


         queryString = QString("UPDATE log SET qrzcom_qso_upload_status = 'Y', qrzcom_qso_upload_date = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
         sqlOK = query.exec(queryString);
         query.finish();
         if (sqlOK)
         {
              //qDebug() << Q_FUNC_INFO << " - exec: " << query.lastQuery();
         }
         else
         {
             emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
               //qDebug() << Q_FUNC_INFO << " - END FALSE" ;
             return false;
         }
    }
      //qDebug() << Q_FUNC_INFO << " - END TRUE" ;
    return true;
}


bool DataProxy_SQLite::clublogSentQSOs(const QList<int> &_qsos)
{
    //qDebug() << Q_FUNC_INFO << " -" << QString::number(_qsos.count());
    if (_qsos.count() < 1)
    {
        return true;
    }
    QString queryString;
    bool sqlOK;
    QSqlQuery query;

    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << Q_FUNC_INFO << " - updating QSO: " << QString::number(_qsos.at(i));


         queryString = QString("UPDATE log SET clublog_qso_upload_status = 'Y', clublog_qso_upload_date = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
         sqlOK = query.exec(queryString);
         query.finish();
         if (sqlOK)
         {
              //qDebug() << Q_FUNC_INFO << " -: exec: " << query.lastQuery();
         }
         else
         {
             emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
               //qDebug() << Q_FUNC_INFO << " -: END FALSE" ;
             return false;
         }
    }
      //qDebug() << Q_FUNC_INFO << " -: END TRUE" ;
    return true;
}

bool DataProxy_SQLite::clublogModifyFullLog(const int _currentLog)
 {
     //qDebug() << Q_FUNC_INFO << " -" << QString::number(_currentLog);

     if (!doesThisLogExist(_currentLog))
     {
         return false;
     }
     QString queryString;
     bool sqlOK;
     QSqlQuery query;

    queryString = QString("UPDATE log SET clublog_qso_upload_status = 'M', clublog_qso_upload_date = '%1'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate()));
    sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        //qDebug() << Q_FUNC_INFO << " -: END FALSE" ;
        return false;
    }

       //qDebug() << Q_FUNC_INFO << " -: END TRUE" ;
    //return false;
 }

 bool DataProxy_SQLite::eQSLModifyFullLog(const int _currentLog)
 {
     //qDebug() << Q_FUNC_INFO << " -" << QString::number(_currentLog);

     if (!doesThisLogExist(_currentLog))
     {
         return false;
     }
     QString queryString;
     bool sqlOK;
     QSqlQuery query;

    queryString = QString("UPDATE log SET eqsl_qsl_sent = 'Q', eqsl_qslsdate = '%1'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate()));
    sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " -: END TRUE";
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        //qDebug() << Q_FUNC_INFO << " -: END FALSE: " << query.lastError().databaseText() ;
        return false;
    }
       //qDebug() << Q_FUNC_INFO << " -: END TRUE" ;
 }

 bool DataProxy_SQLite::eQSLSentQSOs(const QList<int> &_qsos)
 {
     //qDebug() << Q_FUNC_INFO << " -" << QString::number(_qsos.count());
     if (_qsos.count() < 1)
     {
         return true;
     }
     QString queryString;
     bool sqlOK;
     QSqlQuery query;

     for (int i = 0; i< _qsos.count(); i++)
     {
           //qDebug() << Q_FUNC_INFO << " -: updating QSO: " << QString::number(_qsos.at(i));


          queryString = QString("UPDATE log SET eqsl_qsl_sent = 'Y', eqsl_qslsdate = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
          sqlOK = query.exec(queryString);
          query.finish();
          if (sqlOK)
          {
               //qDebug() << Q_FUNC_INFO << " -: exec: " << query.lastQuery();
          }
          else
          {
              emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                //qDebug() << Q_FUNC_INFO << " -: END FALSE" ;
              return false;
          }
     }
       //qDebug() << Q_FUNC_INFO << " -: END TRUE" ;
     return true;
 }



bool DataProxy_SQLite::addQSOFromWSJTX (const QString &_dxcall, const QString &_mode, const QString &_band, const double _freq,
                      const QString &_mygrid, const QString &_dxgrid,
                      const QString &_rstTX, const QString &_rstRX, const QString &_sRX, const QString &_sTX,
                      const QString &_comment,
                      const QString &_stationcallsign, const QString &_name, const QString &_operator,
                      const QDateTime &_datetime, const QDateTime &_datetime_off, const double txpower,
                      const int _dxcc, const int _logNumber, bool _sendQSL)
{
    //qDebug() << Q_FUNC_INFO << _dxcall;

    //_qso format: Date/TimeOn/call/bandid/modeid/freq/dxgrid/timeOff/rsttx/rstrx/txpower/comments/name
    QSO qso;
    qso.setCall(_dxcall);
    qso.setDateTimeOn(_datetime);
    qso.setBand(_band);
    qso.setFreq(_freq);
    qso.setMode(_mode);
    qso.setDateOff(_datetime_off.date());
    qso.setTimeOff(_datetime_off.time());
    qso.setRSTTX(_rstTX);
    qso.setRSTRX(_rstRX);
    qso.setStxString(_sTX);
    qso.setSrxString(_sRX);
    qso.setComment(_comment);
    qso.setName(_name);
    qso.setGridSquare(_dxgrid);
    qso.setMyGridSquare(_mygrid);
    qso.setTXPwr(txpower);
    qso.setOperatorCallsign(_operator);
    qso.setStationCallsign(_stationcallsign);
    qso.setDXCC(_dxcc);

    if (_sendQSL)
    {
        qso.setQSLVia("B");
        qso.setLoTWQSL_SENT("Q");
        qso.setEQSLQSL_SENT("Q");
        qso.setHRDLogStatus("M");
        qso.setClubLogStatus("M");
        qso.setQRZCOMStatus("M");
    }
    qso.setLogId(_logNumber);
    return (qso.toDB()>0);
}

bool DataProxy_SQLite::deleteQSO(const int _qsoId)
{
         //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;
    QString queryString = QString("DELETE FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
}

int DataProxy_SQLite::isWorkedB4(const QString &_qrz, const int _currentLog)
{
         //qDebug() << Q_FUNC_INFO << " -";
    //Returns the QSO id
    QSqlQuery query;
    QString queryString;
    if (_currentLog < 0)
    {
        queryString = QString("SELECT id FROM log WHERE call='%1'").arg(_qrz);
    }
    else
    {
        queryString = QString("SELECT id FROM log WHERE call='%1' AND lognumber='%2'").arg(_qrz).arg(_currentLog);
    }

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
}

QList<int> DataProxy_SQLite::isThisQSODuplicated (const QSO &_qso, const int _secs)
{
   //qDebug() << Q_FUNC_INFO << " - 000";
    QList<int> dupeQsos;
   //qDebug() << Q_FUNC_INFO << " - 001";
    dupeQsos.clear();
   //qDebug() << Q_FUNC_INFO << " - 002";
    QSO q(_qso);
   //qDebug() << Q_FUNC_INFO << " - 003";
    //qDebug() << Q_FUNC_INFO << " - Call: " << q.getCall();

    if (!q.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - END Empty 3" ;
        return dupeQsos;
    }
   //qDebug() << Q_FUNC_INFO << " - 005";
    int validityPeriod = _secs * 60;
    QString initTime = util->getDateTimeSQLiteStringFromDateTime(q.getDateTimeOn().addSecs(-validityPeriod));
    QString endTime = util->getDateTimeSQLiteStringFromDateTime(q.getDateTimeOn().addSecs(validityPeriod));

    //yyyy-MM-dd hh:mm:ss
    // We will match +-15min
   //qDebug() << Q_FUNC_INFO << " - 010";
    QString queryString = QString("SELECT id, qso_date FROM log WHERE call = ':call' AND bandid = ':bandid' AND modeid= ':modeid' AND qso_date BETWEEN ':initTime' AND ':endTime'");
    //QString queryString = QString("SELECT id, qso_date FROM log WHERE call= :call AND bandid = :bandid AND modeid= :modeid AND qso_date >= ':initTime' AND qso_date <= ':endTime'");
          //queryString = QString("SELECT id, qso_date FROM log WHERE call='%1' AND bandid='%2' AND modeid='%3' AND qso_date>='%4' AND qso_date<='%5'").arg(_qrz).arg(_band).arg(_mode).arg(initTime).arg(endTime);
    QSqlQuery query;
    if (!query.prepare(queryString))
    {
        //qDebug() << Q_FUNC_INFO << ": ERROR preparing the Query";
        return dupeQsos;
    }
   //qDebug() << Q_FUNC_INFO << " - 020";
    query.bindValue(":call", q.getCall());   
    query.bindValue(":bandid", getIdFromBandName(q.getBand()));   
    query.bindValue(":modeid", getIdFromModeName(q.getMode()));   
    query.bindValue(":initTime", initTime);   
    query.bindValue(":endTime", endTime);
   //qDebug() << Q_FUNC_INFO << " - 030";
    //qDebug() << Q_FUNC_INFO << " QUERY:   " << q.getCall();
    //qDebug() << Q_FUNC_INFO << " QUERY:   " << getIdFromBandName(q.getBand());
    //qDebug() << Q_FUNC_INFO << " QUERY:   " << getIdFromModeName(q.getMode());
    //qDebug() << Q_FUNC_INFO << " QUERY:   " << initTime;
    //qDebug() << Q_FUNC_INFO << " QUERY:   " << endTime;

    bool sqlOK = query.exec(queryString);
   //qDebug() << Q_FUNC_INFO << " - 050";
    if (sqlOK)
    {
       //qDebug() << Q_FUNC_INFO << " - 051";
        while (query.next())
        {
            if (query.isValid())
            {
                int dupeQSO = (query.value(0)).toInt();
                if (dupeQSO>0)
                {
                    dupeQsos.append(dupeQSO);
                }
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << " - END Empty 2" ;
            }
        }
       //qDebug() << Q_FUNC_INFO << " - END - ok" ;
        return dupeQsos;
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - 052";
        //qDebug() << Q_FUNC_INFO << " : Native Error1 " << query.lastError().text();
        //qDebug() << Q_FUNC_INFO << " : Native Error2 " << query.lastError();
        //qDebug() << Q_FUNC_INFO << " : Native Error3 " << query.lastError().text();

        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
       //qDebug() << Q_FUNC_INFO << " - END Empty 1" ;
        return dupeQsos;        
    }
}

int DataProxy_SQLite::getDuplicatedQSOId(const QString &_qrz, const QDateTime &_datetime, const int _band, const int _mode)
{
    //qDebug() << Q_FUNC_INFO;
    QSqlQuery query;
    QString datetime = util->getDateTimeSQLiteStringFromDateTime(_datetime);

    // Use parameterized query to avoid SQL injection and improve performance
    query.prepare("SELECT id FROM log WHERE call = :call AND qso_date = :qso_date AND bandid = :bandid AND modeid = :modeid");
    query.bindValue(":call", _qrz);
    query.bindValue(":qso_date", datetime);
    query.bindValue(":bandid", _band);
    query.bindValue(":modeid", _mode);

    // Execute the query
    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return -1;
    }

    // Check the result
    if (query.next() && query.isValid())
    {
        int qsoId = query.value(0).toInt();
        return qsoId > 0 ? qsoId : -2;
    }

    return -3;
}

bool DataProxy_SQLite::isHF(const int _band)
{// 160M is considered as HF
    if ( (_band>=getIdFromBandName("10M")) && (_band<=getIdFromBandName("160M")) )
    {
            //qDebug() << Q_FUNC_INFO << " - TRUE";
        return true;
    }
    else
    {
            //qDebug() << Q_FUNC_INFO << " - FALSE";
        return false;
    }
}

bool DataProxy_SQLite::isWARC(const int _band)
{
    if ( (_band==getIdFromBandName("12M")) || (_band==getIdFromBandName("17M")) || ((_band==getIdFromBandName("30M")) ) )
    {
             //qDebug() << Q_FUNC_INFO << " - tRUE";
        return true;
    }
    else
    {
             //qDebug() << Q_FUNC_INFO << " - FALSE";
        return false;
    }
}

bool DataProxy_SQLite::isVHF(const int _band)
{
    if (_band<=getIdFromBandName("6M"))
    {
            //qDebug() << Q_FUNC_INFO << " - TRUE";
        return true;
    }
    else
    {
            //qDebug() << Q_FUNC_INFO << " - FALSE";
        return false;
    }
}

bool DataProxy_SQLite::isUHF(const int _band)
{
    if (_band<=getIdFromBandName("70CM"))
    {
            //qDebug() << Q_FUNC_INFO << " - TRUE";
        return true;
    }
    else
    {
            //qDebug() << Q_FUNC_INFO << " - FALSE";
        return false;
    }
}

QStringList DataProxy_SQLite::getOperatingYears(const int _currentLog)
{
       //qDebug() << Q_FUNC_INFO << " - " << QString::number(_currentLog);
    QStringList years = QStringList();
    //QStringList yearsSorted = QStringList();
    QSqlQuery query;
    QString queryString;
    if (_currentLog<0)
    {
        queryString = QString("SELECT DISTINCT (substr (qso_date, 0, 5)) FROM log ORDER BY 'qso_date'");
    }
    else
    {
        queryString = QString("SELECT DISTINCT (substr (qso_date, 0, 5)) FROM log WHERE lognumber='%0' ORDER BY 'qso_date'").arg(_currentLog);
    }

    //QString year = QString();
       //qDebug() << Q_FUNC_INFO << " - -1";
    bool sqlOk = query.exec(queryString);

    if (!sqlOk)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return years;
    }

    //qDebug() << Q_FUNC_INFO << " - sqlOk = true";
    while (query.next())
    {
        if (query.isValid())
        {
           //QString year = (query.value(0)).toString();
        //qDebug() << Q_FUNC_INFO << " - year=" << year;
            years << (query.value(0)).toString();
            //year.clear();
        }
    }
           //qDebug() << Q_FUNC_INFO << " - END OK - " << QString::number(years.size())<< QT_ENDL;
    query.finish();
    if (years.length()>0)
    {
        years.sort();
    }
    return years;
}

void DataProxy_SQLite::compressDB()
{
    db->compress();
}

bool DataProxy_SQLite::unMarkAllQSO()
{
    return db->unMarkAllQSO();
}

bool DataProxy_SQLite::lotwSentQueue(const QDate &_updateDate, const int _currentLog)
{// Mark LOTW QSL SENT as Q (Queued)
    // If currentLog <0 ALL the QSO of the log will be queued

       //qDebug() << Q_FUNC_INFO << " - Date:" << _updateDate << " /" << QString::number(_currentLog);
    QString queryString;

    if (_currentLog<1)
    {
        queryString = QString("UPDATE log SET lotw_qsl_sent = 'Q', lotw_qslsdate = '%1' WHERE lotw_qsl_sent != 'Y' AND lotw_qsl_sent != 'N' AND lotw_qsl_sent != 'R' AND lotw_qsl_sent != 'I' AND lotw_qsl_sent != 'Q' OR lotw_qsl_sent IS NULL").arg(util->getDateSQLiteStringFromDate(_updateDate));
    }
    else
    {
        queryString = QString("UPDATE log SET lotw_qsl_sent = 'Q', lotw_qslsdate = '%1' WHERE lognumber = '%2' AND lotw_qsl_sent != 'Y' AND lotw_qsl_sent != 'N' AND lotw_qsl_sent != 'R' AND lotw_qsl_sent != 'I' AND lotw_qsl_sent != 'Q' OR lotw_qsl_sent IS NULL").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_currentLog);
    }

    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::lotwSentYes(const QDate &_updateDate, const int _currentLog, const QString &_station)
{// Mark LOTW QSL SENT as Q (Queued)
    // If currentLog <0 ALL the QSO of the log will be queued

       //qDebug() << Q_FUNC_INFO << " - " << QString::number(_currentLog);
    QString queryString;


    if (_currentLog<1)
    {
        if (_station == "ALL")
        {
            queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE lotw_qsl_sent == 'Q'");
        }
        else
        {
            queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE lotw_qsl_sent == 'Q' AND station_callsign='%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_station);
        }
    }
    else
    {
        if (_station == "ALL")
        {
            queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE lognumber = '%2' AND lotw_qsl_sent == 'Q'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_currentLog);
        }
        else
        {
            queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE lognumber = '%2' AND lotw_qsl_sent == 'Q' AND station_callsign='%3'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_currentLog).arg(_station);
        }
    }

    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::lotwSentQSOs(const QList<int> &_qsos)
{
      //qDebug() << Q_FUNC_INFO << " -" << QString::number(_qsos.count());
    if (_qsos.count() < 1)
    {
        return true;
    }
    QString queryString;
    bool sqlOK;
    QSqlQuery query;

    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << Q_FUNC_INFO << " -: updating QSO: " << QString::number(_qsos.at(i));

         //queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE id='%2'").arg((QDate::currentDate()).toString("yyyy-MM-dd")).arg(QString::number(_qsos.at(i)));
         queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
         sqlOK = query.exec(queryString);
         query.finish();
         if (sqlOK)
         {
              //qDebug() << Q_FUNC_INFO << " -: exec: " << query.lastQuery();
         }
         else
         {
             emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
               //qDebug() << Q_FUNC_INFO << " -: END FALSE" ;
             return false;
         }
    }
      //qDebug() << Q_FUNC_INFO << " -: END TRUE" ;
    return true;
}

int DataProxy_SQLite::lotwUpdateQSLReception (const QString &_call, const QDateTime &_dateTime, const QString &_band, const QString &_mode, const QDate &_qslrdate)
{ //Returns the QSO id updated or -1 if none was updated.
   //qDebug() << Q_FUNC_INFO << " - " << _call << "/" << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << "/" <<_band <<"/"<<_mode << "/" << util->getADIFDateFromQDate(_qslrdate)  << endl ;
    int bandid = getIdFromBandName(_band);
    int modeid = getIdFromModeName(_mode);

    QString qso_date;
    qso_date = util->getDateTimeSQLiteStringFromDateTime(_dateTime);

    QString queryString;
    //queryString = QString("SELECT id, lotw_qsl_rcvd FROM log WHERE call='%1' AND qso_date='%2' AND bandid='%4' AND modeid='%5'").arg(_call).arg(qso_date).arg(bandid).arg(modeid);
    queryString = QString("SELECT id, lotw_qsl_rcvd FROM log WHERE call='%1' AND qso_date='%2' AND bandid='%4' AND modeid='%5'").arg(_call).arg(qso_date).arg(bandid).arg(modeid);

    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int id = (query.value(0)).toInt();
            QString _lotwQSLReceived = (query.value(1).toString());

            if (( id > 0) && (_lotwQSLReceived!="Y"))
            {
                query.finish();

                //QString qslsdate = (QDate::fromString(_qslsdate, "yyyyMMdd")).toString("yyyy-MM-dd");
                //QString qslrdate = (QDate::fromString(_qslrdate, "yyyyMMdd")).toString("yyyy-MM-dd");
                queryString = QString("UPDATE log SET lotw_qsl_rcvd = 'Y', lotw_qslrdate = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(_qslrdate)).arg(QString::number(id));

                sqlOK = query.exec(queryString);
                query.finish();
                if (sqlOK)
                {
                   //qDebug() << Q_FUNC_INFO << " - Modified Id: " << QString::number(id);
                    return id;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                   //qDebug() << Q_FUNC_INFO << " - SQL ERROR";
                    return -4;
                }
            }
            else
            {
               //qDebug() << Q_FUNC_INFO << " - ID Not found";
                query.finish();
                return -5;
            }
        }
        else if ((query.lastError().text()).toInt() == -1)
        {
           //qDebug() << Q_FUNC_INFO << " - QSO not found " << query.lastQuery();
            return -1;
        }
        else
        {
           //qDebug() << Q_FUNC_INFO << " - Unknown error " << query.lastQuery();
           //qDebug() << Q_FUNC_INFO << " - Error: " << query.lastError().databaseText();
           //qDebug() << Q_FUNC_INFO << " - Error: " << QString::number(query.lastError().text());
            query.finish();
            return -3;
        }
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - Query error: " << query.lastQuery();
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return -100;
}
QString DataProxy_SQLite::getStringQueryLogNumber (const int _a)
{
    // Returns the string to build a query.
    // This is used by:
    // DataProxy_SQLite::getQSOsListLoTWToSend
    // DataProxy_SQLite::getQSOsListClubLogToSent
    // DataProxy_SQLite::getQSOsListEQSLToSent
    // DataProxy_SQLite::getQSOsListQRZCOMToSent
    // DataProxy_SQLite::getQSOsListToBeExported

    if (doesThisLogExist (_a))
    {
        return QString(" AND lognumber='%1'").arg(_a);
    }
    else
    {
        return QString();
    }
}

QString DataProxy_SQLite::getStringQueryMyGrid (const QString &_a)
{
    // Returns the string to build a query.
    // This is used by:
    // DataProxy_SQLite::getQSOsListToBeExported
    if (util->isValidGrid (_a))
    {
        return QString("my_gridsquare='%1'").arg(_a);
    }
    else if (_a == "ALL")
    {
        return QString("((my_gridsquare!='ALL') OR (my_gridsquare IS NULL) OR (my_gridsquare=''))");
    }
    else
    { // NO GRID
        return QString("(my_gridsquare='' OR my_gridsquare IS NULL)");
    }
}

QString DataProxy_SQLite::getStringQueryStationCallSign (const QString &_a)
{
    // Returns the string to build a query.
    // This is used by:
    // DataProxy_SQLite::getQSOsListLoTWToSend
    // DataProxy_SQLite::getGridsToBeSent
    // DataProxy_SQLite::getQSOsListClubLogToSent
    // DataProxy_SQLite::getQSOsListEQSLToSent
    // DataProxy_SQLite::getQSOsListQRZCOMToSent
    // DataProxy_SQLite::getQSOsListToBeExported
    // DataProxy_SQLite::getQSOsListeQSLNotSent
    Callsign callsign(_a);
    if (callsign.isValid())
    {
        return QString("station_callsign='%1'").arg(_a);
    }
    else if (_a == "ALL")
    {
        return QString("((station_callsign!='ALL') OR (station_callsign IS NULL) OR (station_callsign=''))");
    }
    else
    {
        return QString("((station_callsign='') OR (station_callsign IS NULL))");
    }
}

QList<int> DataProxy_SQLite::getQSOsListLoTWToSend(const QString &_stationCallsign, const QString &_myGrid, const QDate &_startDate, const QDate &_endDate, bool _justQueued, int _logN)
{
    //qDebug() << Q_FUNC_INFO << " - Call/Start/end: " << _stationCallsign << _myGrid << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd");


    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

    QString _queryGrid_string = getStringQueryMyGrid (_myGrid);


    QString _query_justQueued;
    if (_justQueued)
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued TRUE";
        _query_justQueued = QString("lotw_qsl_sent='Q'");
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued FALSE";
        _query_justQueued = QString("lotw_qsl_sent!='1'");
    }

    QString _query_logNumber = getStringQueryLogNumber(_logN);

    QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
    QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

    queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 AND %3 %4 AND %5 AND %6").arg(_queryST_string).arg(_queryGrid_string).arg(_query_justQueued).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);


    QList <int> qsoList;
    qsoList.clear();

    QSqlQuery query;
    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery() ;
    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << Q_FUNC_INFO << " - QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd");
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;
}

QStringList DataProxy_SQLite::getGridsToBeSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, const ExportMode _em, bool _justModified, int _logN)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    QStringList grids;
    grids.clear ();

    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);
    QString _queryST_logNumber = getStringQueryLogNumber (_logN);

    QString _query_justQueued;
    if ((_justModified) && (_em == ModeLotW))
    {
        _query_justQueued = QString("lotw_qsl_sent='Q'");
    }
    else
    {
        _query_justQueued = QString("((lotw_qsl_sent!='1') OR (lotw_qsl_sent IS NULL))");
    }

    queryString = QString("SELECT DISTINCT my_gridsquare FROM log WHERE %1 AND ((my_gridsquare<>'') OR (my_gridsquare IS NOT NULL)) AND qso_date>='%2' AND qso_date<='%3' AND %4 %5").arg(_queryST_string).arg(util->getDateSQLiteStringFromDate(_startDate)).arg(util->getDateSQLiteStringFromDate(_endDate.addDays (1))).arg(_query_justQueued).arg(_queryST_logNumber);

    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
   //qDebug() << Q_FUNC_INFO << ": " << query.lastQuery ();

    if (sqlOK)
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(0)).toString();
                if (aux.length()>1)
                {
                    grids.append ((query.value(0)).toString());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        grids.sort ();
       //qDebug() << Q_FUNC_INFO << " - END-1";
        return grids;
    }
    query.finish();
    grids.sort();
   //qDebug() << Q_FUNC_INFO << " - END";
    return grids;
}

QList<int> DataProxy_SQLite::getQSOsListClubLogToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified, int _logN)
{
    //qDebug() << Q_FUNC_INFO << " - Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd");

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

    QString _query_justModified;
    if (_justModified)
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued TRUE";
        _query_justModified = QString("clublog_qso_upload_status='M'");
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued FALSE";
        _query_justModified = QString("clublog_qso_upload_status!='M'");
    }
    //qDebug() << Q_FUNC_INFO << " - logN: " << QString::number(_logN);

    QString _query_logNumber = getStringQueryLogNumber(_logN);


    QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
    QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

    queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5").arg(_queryST_string).arg(_query_justModified).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << Q_FUNC_INFO << " - QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd");
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    //qDebug() << Q_FUNC_INFO << " - Adding: "  << QString::number((query.value(0)).toInt());
                    qsoList.append((query.value(0)).toInt());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    //qDebug() << Q_FUNC_INFO << " - Returning: #"  << QString::number(qsoList.length());
    return qsoList;
}

QList<int> DataProxy_SQLite::getQSOsListEQSLToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified, int _logN)
{
   //qDebug() << Q_FUNC_INFO << " -Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd");

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);


    QString _query_justModified;
    if (_justModified)
    {
        //qDebug() << Q_FUNC_INFO << " -justQueued TRUE";
        _query_justModified = QString("eqsl_qsl_sent='Q'");
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " -justQueued FALSE";
        _query_justModified = QString("eqsl_qsl_sent!='M'");
    }

    QString _query_logNumber = getStringQueryLogNumber(_logN);

    QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
    QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

    queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5").arg(_queryST_string).arg(_query_justModified).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " -Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " -Query: " << query.lastQuery();

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << Q_FUNC_INFO << " -QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd");
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;
}

QList<int> DataProxy_SQLite::getQSOsListQRZCOMToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified, int _logN)
{
    //qDebug() << Q_FUNC_INFO << " - Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd");

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

    QString _query_justModified;
    if (_justModified)
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued TRUE";
        _query_justModified = QString("qrzcom_qso_upload_status='M'");
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued FALSE";
        _query_justModified = QString("qrzcom_qso_upload_status!='-'");
    }

    QString _query_logNumber = getStringQueryLogNumber(_logN);

    QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
    QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

    queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5").arg(_queryST_string).arg(_query_justModified).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << Q_FUNC_INFO << " - QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd");
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;
}


QList<int> DataProxy_SQLite::getQSOsListToBeExported(const QString &_stationCallsign, const QString &_grid, const QDate &_startDate, const QDate &_endDate, int _logN)
{
    //qDebug() << Q_FUNC_INFO << QString("Call: %1, Grid: %2, StartDate: %3, EndDate: %4").arg(_stationCallsign).arg(_grid).arg(_startDate.toString("yyyyMMdd")).arg(_endDate.toString("yyyyMMdd"));

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

    QString _queryGrid_String = getStringQueryMyGrid (_grid);

    QString _query_logNumber = getStringQueryLogNumber(_logN);

    QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
    QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

    queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5 ").arg(_queryST_string).arg(_queryGrid_String).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);;

    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << ": Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << ": Query: " << query.lastQuery();

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << Q_FUNC_INFO << ": QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd");
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;
}

QList<int> DataProxy_SQLite::getQSOsAll()
{
    QString queryString = QString("SELECT id FROM log") ;
    QSqlQuery query;
    QList <int> qsoList;
    qsoList.clear();

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << ": Query: " << query.lastQuery();

    if (sqlOK)
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                qsoList.append((query.value(0)).toInt());
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    return qsoList;
}

QList<int> DataProxy_SQLite::getQSOsListeQSLNotSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justQueued)
{
    //qDebug() << Q_FUNC_INFO << " - Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd");

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

    QString _query_justQueued;
    if (_justQueued)
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued TRUE";
        _query_justQueued = QString("eqsl_qsl_sent='Q'");
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - justQueued FALSE";
        _query_justQueued = QString("eqsl_qsl_sent!='1'");
    }
    /* Modify accordingly to add log number support
        QString _query_logNumber;
        if (doesThisLogExist (_logN))
        {
            _query_logNumber = QString(" AND lognumber='%1'").arg(_logN);
        }
        else
        {
            _query_logNumber.clear ();
        }
        queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 '%3'").arg(_queryST_string).arg(_query_justQueued).arg(_logN);

    */
    queryString = QString("SELECT id, qso_date FROM log WHERE ") + _queryST_string + " AND " + _query_justQueued;

    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << "DataProxy_SQLite: QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd");
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;
}



QStringList DataProxy_SQLite::getQSODetailsForLoTWDownload(const int _id)
{ //Returns QRZ << date+time << Band (txt) << mode (txt)
    //qDebug() << Q_FUNC_INFO << " -" << QString::number(_id);
    QStringList result;
    result.clear();
    //getNameFromBandId
    QSqlQuery query;
    //QString queryString = QString("SELECT call, qso_date, my_gridsquare, bandid, modeid FROM log WHERE id='%0'").arg(_id);
    QString queryString = QString("SELECT call, qso_date, my_gridsquare, band.name, mode.name FROM log JOIN band ON log.bandid=band.id JOIN mode on log.modeid=mode.id WHERE log.id='%0'").arg(_id);


    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                QString call = query.value(0).toString();
                QString date = query.value(1).toString();
                //QString date = util->getDateTimeFromSQLiteString(query.value(1).toString());

                QString myGrid = query.value(2).toString();
                QString bandid = query.value(3).toString();
                QString modeid = query.value(4).toString();
                query.finish();
                //qDebug() << Q_FUNC_INFO << " - - date: " << date;
                //qDebug() << Q_FUNC_INFO << " - - time: " << time;

                //getDateTimeSQLiteStringFromDateTime
                //QString dateTime = (QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss")).toString("yyyy-MM-dd hh:mm");
                QString dateTime = (util->getDateTimeFromSQLiteString(date)).toString("yyyy-MM-dd hh:mm");

                //bandid = getNameFromBandId(bandid.toInt());
                //modeid = getNameFromModeId(modeid.toInt());

                result.append(call);
                result.append(dateTime);
                result.append (myGrid);
                result.append(bandid);
                result.append(modeid);
                //qDebug() << Q_FUNC_INFO << " - - END: call: " << call;
                return result;
            }
            //qDebug() << Q_FUNC_INFO << " - - END: no valid " ;
        }
        query.finish();
        //qDebug() << Q_FUNC_INFO << " - - END: no next " ;
        return result;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - - END: SQL NOK " ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return result;
    }
}

int DataProxy_SQLite::getQSOonYear(const int _year, const int _logNumber)
{
       //qDebug() << Q_FUNC_INFO << " - " << QString::number(_year) << "/" << QString::number(_logNumber);

    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    if (_logNumber < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log WHERE qso_date LIKE '%1%'").arg(_year);
    }
    else
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND qso_date LIKE '%2%'").arg(_logNumber).arg(_year);
    }

    sqlOK = query.exec(queryString);

         //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << " - 0";
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() << Q_FUNC_INFO << " - Query error";
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getDXCConYear(const int _year, const int _logNumber)
{
         //qDebug() << Q_FUNC_INFO << " - " << QString::number(_year) << "/" << QString::number(_logNumber);

    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    if (_logNumber < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT dxcc) FROM log WHERE dxcc>'0' AND qso_date LIKE '%1%'").arg(_year);
    }
    else
    {
        queryString = QString("SELECT COUNT (DISTINCT dxcc) FROM log WHERE dxcc>'0' AND lognumber='%1' AND qso_date LIKE '%2%'").arg(_logNumber).arg(_year);
    }

    sqlOK = query.exec(queryString);

         //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << " - 0";
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() << Q_FUNC_INFO << " - Query error";
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getCQzonYear(const int _year, const int _logNumber)
{
         //qDebug() << Q_FUNC_INFO << " - " << QString::number(_year);
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    if (_logNumber < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT cqz) FROM log where qso_date LIKE '%1%' AND cqz>'0' AND cqz < '41'").arg(_year);
    }
    else
    {
        queryString = QString("SELECT COUNT (DISTINCT cqz) FROM log where lognumber='%1' AND cqz>'0' AND cqz<'41' AND qso_date LIKE '%2%'").arg(_logNumber).arg(_year);
    }

    sqlOK = query.exec(queryString);

         //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - 0";
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() << Q_FUNC_INFO << " - Query error";
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getQSOsWithDXCC(const int _dxcc, const int _logNumber)
{
     //qDebug() << Q_FUNC_INFO << " - " << QString::number(_dxcc);
  QSqlQuery query;
  QString queryString;
  bool sqlOK;
  if (_logNumber < 0)
  {
      //queryString = QString("SELECT COUNT (DISTINCT id) FROM log where dxcc LIKE '%1'").arg(_dxcc);
      queryString = QString("SELECT COUNT (DISTINCT id) FROM log where dxcc = '%1'").arg(_dxcc);
  }
  else
  {
      //queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND dxcc LIKE '%2'").arg(_logNumber).arg(_dxcc);
      queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND dxcc = '%2'").arg(_logNumber).arg(_dxcc);
  }

  sqlOK = query.exec(queryString);

       //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
               //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
          int v = (query.value(0)).toInt();
          query.finish();
          return v;
      }
      else
      {
               //qDebug() << Q_FUNC_INFO << " - 0";
          query.finish();
          return 0;
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
           //qDebug() << Q_FUNC_INFO << " - Query error";
      query.finish();
      return 0;
  }
}

int DataProxy_SQLite::getQSOsAtHour(const int _hour, const int _log)
{
     //qDebug() << Q_FUNC_INFO << " - " << QString::number(_hour);
  QSqlQuery query;
  QString queryString;
  bool sqlOK;
  QString aux = QString();
  if (_hour < 10)
  {
      aux = "0" + QString::number(_hour);
  }
  else
  {
      aux = QString::number(_hour);
  }

  if (_log < 0)
  {
      queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE qso_date LIKE '% %1:%'").arg(aux);
  }
  else
  {
      queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE lognumber='%1' AND qso_date LIKE '% %2:%'").arg(_log).arg(aux);
  }

  sqlOK = query.exec(queryString);

       //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
               //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
          int v = (query.value(0)).toInt();
          query.finish();
          return v;
      }
      else
      {
               //qDebug() << Q_FUNC_INFO << " - 0";
          query.finish();
          return 0;
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
           //qDebug() << Q_FUNC_INFO << " - Query error";
      query.finish();
      return 0;
  }
}


int DataProxy_SQLite::getQSOsAtHourOnBand(const int _hour, const int _band, const int _log)
{
      //qDebug() << Q_FUNC_INFO << " - " << QString::number(_hour);
   QSqlQuery query;
   QString queryString;
   bool sqlOK;
   QString aux = QString();

   if (_hour < 10)
   {
       aux = "0" + QString::number(_hour);
   }
   else
   {
       aux = QString::number(_hour);
   }

   if (_log < 0)
   {
       queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE bandid='%1' AND qso_date LIKE '% %2:%'").arg(_band).arg(aux);
   }
   else
   {
       queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE lognumber='%1' AND bandid='%2' AND qso_date LIKE '% %3:%' ").arg(_log).arg(_band).arg(aux);
   }

   sqlOK = query.exec(queryString);

        //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
   if (sqlOK)
   {
       query.next();
       if (query.isValid())
       {
                //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
           int v = (query.value(0)).toInt();
           query.finish();
           return v;
       }
       else
       {
                //qDebug() << Q_FUNC_INFO << " - 0";
           query.finish();
           return 0;
       }
   }
   else
   {
       emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug() << Q_FUNC_INFO << " - Query error";
       query.finish();
       return 0;
   }
}

int DataProxy_SQLite::getQSOsOnMonth(const int _month, const int _log)
{
       //qDebug() << Q_FUNC_INFO << " - " << QString::number(_month);
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    QString aux = QString();
    if (_month < 10)
    {
        aux = "0" + QString::number(_month);
    }
    else
    {
        aux = QString::number(_month);
    }

    if (_log < 0)
    {
        queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE qso_date LIKE '%-%1-%'").arg(aux);
    }
    else
    {
        queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE lognumber='%1' AND qso_date LIKE '%-%2-%'").arg(_log).arg(aux);
    }

    sqlOK = query.exec(queryString);

         //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
               //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
               //qDebug() << Q_FUNC_INFO << " - 0";
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() << Q_FUNC_INFO << " - Query error";
        query.finish();
        return 0;
    }
}

bool DataProxy_SQLite::updateQSONumberPerLog()
{
    //qDebug() << Q_FUNC_INFO;
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    QList<int> _logsInLogs;
    _logsInLogs.clear();

    queryString = QString("SELECT id FROM logs");

    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                if ( (query.value(0)).toInt() >= 1)
                {
                    _logsInLogs.append((query.value(0)).toInt());
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug() << Q_FUNC_INFO << " - TRUE2";
        query.finish();
        return false;
    }
    query.finish();
    foreach(int i, _logsInLogs)
    {
        int _qsos = getHowManyQSOInLog(i);
        if (_qsos>0)
        {
            queryString = QString("UPDATE logs set logtypen = '%1' WHERE id = '%2'").arg(_qsos).arg(i);
            sqlOK = query.exec(queryString);
            if (!sqlOK)
            {
                query.finish();
                return false;
            }
            query.finish();
        }
    }
    return true;
}

bool DataProxy_SQLite::newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber)
{
        //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    bool existingDXCC = false;
    bool existingCQz = false;

    queryString = QString("SELECT dxcc, cqz FROM log WHERE (lognumber='%1' AND qso_date  LIKE'%%2%') AND (dxcc ='%3' OR cqz ='%4')").arg(_logNumber).arg(_year).arg(_dxcc).arg(_cq);

    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                if ( (query.value(0)).toInt() == _dxcc)
                {
                         //qDebug() << Q_FUNC_INFO << " - - Existing DXCC";
                    existingDXCC = true;
                }
                if ( (query.value(1)).toInt() == _cq)
                {
                        //qDebug() << Q_FUNC_INFO << " - - Existing CQz";
                    existingCQz = true;
                }
            }
        }
        if (existingDXCC && existingCQz)
        {
                //qDebug() << Q_FUNC_INFO << " - - FALSE";
            query.finish();
            return false;
        }
        else
        {
                //qDebug() << Q_FUNC_INFO << " - - TRUE1";
            query.finish();
            return true;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug() << Q_FUNC_INFO << " - - TRUE2";
        query.finish();
         return true;   // It is an error inthe query but Work First Worry Later, let us work that QSO.
    }
}

QStringList DataProxy_SQLite::getContestNames()
{
         //qDebug() << Q_FUNC_INFO << " -" ;
    QStringList contests = QStringList();
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    queryString = QString("SELECT DISTINCT name from supportedcontests ORDER BY id ASC");

    sqlOK = query.exec(queryString);


    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                queryString = (query.value(0)).toString();
                     //qDebug() << Q_FUNC_INFO << " - " << queryString ;
                contests.append(queryString);
            }
            else
            {
                query.finish();
                return QStringList();
            }
        }
        query.finish();
        contests.sort();
        return contests;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }

    //return QStringList();
}

QStringList DataProxy_SQLite::getContestCat(const int _catn)
{
    QStringList contests = QStringList();
    QSqlQuery query;
    QString queryString;
    bool sqlOK;

    switch (_catn) {
        case 1:
            queryString = QString("SELECT DISTINCT name from contestcatoperator ORDER BY id ASC");
        break;
        case 2:
            queryString = QString("SELECT DISTINCT name from contestcatassisted ORDER BY id ASC");
        break;
        case 3:
            queryString = QString("SELECT DISTINCT name from contestcatpower ORDER BY id ASC");
        break;
        case 4:
            queryString = QString("SELECT DISTINCT name from contestcatband ORDER BY id ASC");
        break;
        case 5:
            queryString = QString("SELECT DISTINCT name from contestcatoverlay ORDER BY id ASC");
        break;
        case 6:
            queryString = QString("SELECT DISTINCT name from contestcatmode ORDER BY id ASC");
        break;
        default:
            return QStringList();
        //break;
        }

    sqlOK = query.exec(queryString);


    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                queryString = (query.value(0)).toString();
                contests.append(queryString);
            }
            else
            {
                query.finish();
                return QStringList();
            }
        }
        query.finish();
        return contests;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }
}


QStringList DataProxy_SQLite::getContestOverlays()
{
         //qDebug() << Q_FUNC_INFO << " - "<< QT_ENDL;

    QStringList contests = QStringList();
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    queryString = QString("SELECT DISTINCT name from contestcatoverlay ORDER BY id ASC");

    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                queryString = (query.value(0)).toString();
                     //qDebug() << Q_FUNC_INFO << " - " << queryString ;
                contests.append(queryString);
            }
            else
            {
                query.finish();
                return QStringList();
            }
        }
        query.finish();
        return contests;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }
}

bool DataProxy_SQLite::isValidPropMode(const QString &_prop)
{
    QSqlQuery query;
    QString queryString = QString("SELECT shortname FROM prop_mode_enumeration WHERE shortname='%1'").arg(_prop);
    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            query.finish();
            return true;
        }
    }
    return false;
}

QStringList DataProxy_SQLite::getPropModeList()
{
        //qDebug()  << Q_FUNC_INFO << " -" ;
    QString aux = QString();
    QStringList qs;
    qs.clear();

    QString queryString = QString("SELECT id, shortname, name FROM prop_mode_enumeration ORDER BY name");
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    QString num;

    if (sqlOK)
    {
        QString translatedValue = QString();
        QString valueToTranslate = QString();
        while ( (query.next())) {
            if (query.isValid())
            {
                valueToTranslate = (query.value(2)).toString();
                if (valueToTranslate == "Aircraft Scatter")
                {
                    translatedValue = tr("Aircraft Scatter", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "Aurora")
                {
                    translatedValue = tr("Aurora");
                }
                else if(valueToTranslate == "Aurora-E")
                {
                    translatedValue = tr("Aurora-E");
                }
                else if(valueToTranslate == "Back scatter")
                {
                    translatedValue = tr("Back scatter", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "Earth-Moon-Earth")
                {
                    translatedValue = tr("Earth-Moon-Earth");
                }
                else if(valueToTranslate == "Sporadic E")
                {
                    translatedValue = tr("Sporadic E");
                }
                else if(valueToTranslate == "Field Aligned Irregularities")
                {
                    translatedValue = tr("Field Aligned Irregularities", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "F2 Reflection")
                {
                    translatedValue = tr("F2 Reflection", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "Internet-assisted")
                {
                    translatedValue = tr("Internet-assisted");
                }
                else if(valueToTranslate == "Ionoscatter")
                {
                    translatedValue = tr("Ionoscatter", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "Meteor scatter")
                {
                    translatedValue = tr("Meteor scatter", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "Terrestrial or atmospheric repeater or transponder")
                {
                    translatedValue = tr("Terrestrial or atmospheric repeater or transponder");
                }
                else if(valueToTranslate == "Rain scatter")
                {
                    translatedValue = tr("Rain scatter", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "Satellite")
                {
                    translatedValue = tr("Satellite");
                }
                else if(valueToTranslate == "Trans-equatorial")
                {
                    translatedValue = tr("Trans-equatorial", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "Tropospheric ducting")
                {
                    translatedValue = tr("Tropospheric ducting", "Common term in hamradio, do not translate if not sure");
                }
                else if(valueToTranslate == "")
                {
                    translatedValue = tr("");
                }
                else
                {
                   translatedValue = valueToTranslate;
                }

                aux.clear();
                num = (query.value(0)).toString();
                if (num.toInt() <= 9)
                {
                    num = "0" + num;
                }
                aux = num + " - " + (query.value(1)).toString() + " - " + translatedValue;
                qs << aux;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }
    query.finish();
    qs.sort();
    return qs;
}

bool DataProxy_SQLite::clearSatList()
{
    QSqlQuery query;
    bool sqlOK = query.exec("DELETE FROM satellites");

    if (sqlOK)
    {
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
}

bool DataProxy_SQLite::addSatellite(const QString &_arrlId, const QString &_name, const QString &_downLink, const QString &_upLink, const QString &_mode, int id)
{
    QSqlQuery query;
    QString queryString;

    if (id > 0)
    {
        queryString = QString(
            "UPDATE satellites SET satarrlid = :arrlId, satname = :name, uplink = :upLink, downlink = :downLink, satmode = :mode WHERE id = :id"
        );
        query.prepare(queryString);
        query.bindValue(":id", id);
    }
    else
    {
        queryString = QString(
            "INSERT INTO satellites (satarrlid, satname, uplink, downlink, satmode) VALUES (:arrlId, :name, :upLink, :downLink, :mode)"
        );
        query.prepare(queryString);
    }

    query.bindValue(":arrlId", _arrlId);
    query.bindValue(":name", _name);
    query.bindValue(":upLink", _upLink);
    query.bindValue(":downLink", _downLink);
    query.bindValue(":mode", _mode);

    if (query.exec())
    {
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
}

int DataProxy_SQLite::getDBSatId(const QString &_arrlId)
{
    int aux = -1;
    QSqlQuery query;
    QString queryString = "SELECT id FROM satellites WHERE satarrlid= :arrlId";

    query.prepare(queryString);
    query.bindValue(":arrlId", _arrlId);

    if (query.exec())
    {
        if (query.next())
        {
            aux = query.value(0).toInt();
        }
        else
        {
            query.finish();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }

    query.finish();
    return aux;
}

QStringList DataProxy_SQLite::getSatellitesList()
{
         //qDebug()  << Q_FUNC_INFO << " -" ;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString queryString = QString("SELECT satarrlid, satname FROM satellites");
     QSqlQuery query;

     bool sqlOK = query.exec(queryString);


     if (sqlOK)
     {
         while ( (query.next())) {
             if (query.isValid())
             {
                 aux.clear();
                 aux = (query.value(0)).toString() + " - " + (query.value(1)).toString();
                 qs << aux;
             }
         }
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     qs.sort();
     return qs;
}


QString DataProxy_SQLite::getSatelliteUplink(const QString &_sat, int _pair)
{
    //qDebug()  << Q_FUNC_INFO << " - " << _sat;
    QString aux = QString();
    //QString aux2 = QString();
    //double fr1, fr2, fr;
    QString queryString = QString("SELECT uplink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
            aux = QString::number(getFreqFromRange(aux, _pair));
        }
        else
        {
                 //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    query.finish();
    return aux;
}


QString DataProxy_SQLite::getSatelliteDownlink(const QString &_sat, int _pair)
{
         //qDebug()  << Q_FUNC_INFO << " - " << _sat;
    QString aux = QString();
    //QString aux2 = QString();
    //double fr1, fr2, fr;
    QString queryString = QString("SELECT downlink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);


    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
            aux = QString::number(getFreqFromRange(aux,_pair));
        }
        else
        {
                 //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    query.finish();
    return aux;
}

QString DataProxy_SQLite::getSatelliteMode(const QString &_sat)
{
    QString aux = QString();
    QString queryString = QString("SELECT satmode FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
            query.finish();
            if (aux.contains(','))
            {   // Potentially somethink like: SSB,CW
                // We select the first one
                aux = aux.section(',', 0, 0);   // We select the first package
            }
        }
        else
        {
                 //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    return aux;
}

QString DataProxy_SQLite::getSatelliteFullUplink(const QString &_sat)
{
    //qDebug()  << Q_FUNC_INFO << " - " << _sat;
    QString aux = QString();
    QString queryString = QString("SELECT uplink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
    query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
        }
        else
        {
            //qDebug()  << Q_FUNC_INFO << " -  query    not valid" ;
            query.finish();
            return QString();
        }
    }
    else
    {
        //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }
    //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    query.finish();
    return aux;
}

QString DataProxy_SQLite::getSatelliteFullDownlink(const QString &_sat)
{
      //qDebug()  << Q_FUNC_INFO << " - " << _sat;
    QString aux = QString();
    //QString aux2 = QString();
    //double fr1, fr2, fr;
    QString queryString = QString("SELECT downlink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
        }
        else
        {
            //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return QString();
        }
    }
    else
    {
        //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }
      //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    query.finish();
    return aux;
}

QString DataProxy_SQLite::getSatelliteFullMode(const QString &_sat)
{
    QString aux = QString();
    QString queryString = QString("SELECT satmode FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
            query.finish();
        }
        else
        {
                 //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }
         //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    return aux;
}

QString DataProxy_SQLite::getSatelliteName(const QString &_sat)
{
      //qDebug()  << Q_FUNC_INFO << " - " << _sat;
 QString aux = QString();

 QString queryString = QString("SELECT satname FROM satellites WHERE satarrlid='%1'").arg(_sat);
 QSqlQuery query;

 bool sqlOK = query.exec(queryString);


 if (sqlOK)
 {
     query.next();
     if (query.isValid())
     {
         aux = query.value(0).toString();
     }
     else
     {
              //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
         query.finish();
         return QString();
     }
 }
 else
 {
          //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
     emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
     query.finish();
     return QString();
 }

      //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
 query.finish();
 return aux;
}

QString DataProxy_SQLite::getSateliteArrlIdFromId(const int _id)
{
    QString aux = QString();
    QString queryString = QString("SELECT satarrlid FROM satellites WHERE id='%1'").arg(_id);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);


    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
            query.finish();
        }
        else
        {
                 //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }
         //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    return aux;
}

double DataProxy_SQLite::getFreqFromRange(QString _fr, int _pair)
{ //May even receive: 145.900-146.00 and should return the mid in the range (145.950)
         //qDebug()  << Q_FUNC_INFO << " - " << _fr;
    QString fr1, fr2, aux;
    double f1, f2;
    fr1.clear();
    fr2.clear();
    f1 = 0.0;
    f2 = 0.0;

    aux.clear();
    aux = _fr;

    if (aux.contains(','))
    {   // Potentially somethink like: 435.030-435.456,146.180
        if((_pair<0) || (_pair>1))
        {
            _pair = 0;
        }
             //qDebug()  << Q_FUNC_INFO << " - has several freqs: " << aux;
        aux = aux.section(',', _pair, _pair);   // We select the selected package
    }
    if (aux.contains('-'))          // Potentially somethink like: 435.030-435.456
    {
             //qDebug()  << Q_FUNC_INFO << " - has several freqs: " << aux;
        fr2 = aux.section('-', 1, 1);   // We select the second freq
        fr1 = aux.section('-', 0, 0);   // We select the first freq

             //qDebug()  << Q_FUNC_INFO << " - fr1: " << fr1;
             //qDebug()  << Q_FUNC_INFO << " - fr2: " << fr2;
        f1 = fr1.toDouble();
        f2 = fr2.toDouble();
             //qDebug()  << Q_FUNC_INFO << " - f1: " << QString::number(f1);
             //qDebug()  << Q_FUNC_INFO << " - f2: " << QString::number(f2);

        f1 = (f2 + f1)/2;

             //qDebug()  << Q_FUNC_INFO << " - f1 after calc: " << QString::number(f1);
    }
    else
    {   // It is only one freq 145.950 so this is what must be returned
        f1 = aux.toDouble();
    }

         //qDebug()  << Q_FUNC_INFO << " - Return: " << QString::number(f1);
    return f1;
}

QStringList DataProxy_SQLite::getQSLRcvdList()
{
         //qDebug()  << Q_FUNC_INFO << " -" ;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString queryString = QString("SELECT shortname, name FROM qsl_rec_status");
     QSqlQuery query;

     bool sqlOK = query.exec(queryString);

     if (sqlOK)
     {
         QString translatedValue = QString();
         QString valueToTranslate = QString();

         while ( (query.next())) {
             if (query.isValid())
             {
                 valueToTranslate = (query.value(1)).toString();
                 if (valueToTranslate == "Yes")
                 {
                     translatedValue = tr("Yes");
                 }
                 else if(valueToTranslate == "No")
                 {
                     translatedValue = tr("No");
                 }
                 else if(valueToTranslate == "Requested")
                 {
                     translatedValue = tr("Requested");
                 }
                 else if(valueToTranslate == "Ignore/Invalid")
                 {
                     translatedValue = tr("Ignore/Invalid");
                 }
                 else if(valueToTranslate == "Validated")
                 {
                     translatedValue = tr("Validated");
                 }
                 else
                 {
                    translatedValue = valueToTranslate;
                 }
                 aux.clear();
                 aux = (query.value(0)).toString() + " - " + translatedValue;
                 qs << aux;
             }
         }
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}

QStringList DataProxy_SQLite::getQSLSentList()
{
         //qDebug()  << Q_FUNC_INFO << " -" ;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString queryString = QString("SELECT shortname, name FROM qsl_sent_status");
     QSqlQuery query;
     bool sqlOK = query.exec(queryString);

     if (sqlOK)
     {
         QString translatedValue = QString();
         QString valueToTranslate = QString();

         while ( (query.next()))
         {
             if (query.isValid())
             {
                 valueToTranslate = (query.value(1)).toString();
                 if (valueToTranslate == "Yes")
                 {
                     translatedValue = tr("Yes");
                 }
                 else if(valueToTranslate == "No")
                 {
                     translatedValue = tr("No");
                 }
                 else if(valueToTranslate == "Requested")
                 {
                     translatedValue = tr("Requested");
                 }
                 else if(valueToTranslate == "Queued")
                 {
                     translatedValue = tr("Queued");
                 }
                 else if(valueToTranslate == "Ignore/Invalid")
                 {
                     translatedValue = tr("Ignore/Invalid");
                 }
                 else
                 {
                    translatedValue = valueToTranslate;
                 }
                 aux.clear();
                 aux = (query.value(0)).toString() + " - " + translatedValue;
                 qs << aux;
             }
         }
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}


QStringList DataProxy_SQLite::getClubLogStatusList()
{
         //qDebug()  << Q_FUNC_INFO << " -" ;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString queryString = QString("SELECT shortname, name FROM clublog_status");
     QSqlQuery query;
     bool sqlOK = query.exec(queryString);

    QString translatedValue = QString();
    QString valueToTranslate = QString();

     if (sqlOK)
     {
         while ( (query.next())) {
             if (query.isValid())
             {
                 valueToTranslate = (query.value(1)).toString();
                 if (valueToTranslate == "Uploaded")
                 {
                     translatedValue = tr("Uploaded");
                 }
                 else if(valueToTranslate == "Do not upload")
                 {
                     translatedValue = tr("Do not upload");
                 }
                 else if(valueToTranslate == "Modified")
                 {
                     translatedValue = tr("Modified");
                 }
                 else
                 {
                    translatedValue = valueToTranslate;
                 }
                 aux.clear();
                 aux = (query.value(0)).toString() + " - " + translatedValue;
                 qs << aux;
             }
         }
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}

QStringList DataProxy_SQLite::getQSLViaList()
{
         //qDebug()  << Q_FUNC_INFO << " -" ;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString queryString = QString("SELECT shortname, name FROM qsl_via_enumeration");
     QSqlQuery query;

     bool sqlOK = query.exec(queryString);

     if (sqlOK)
     {
         QString translatedValue = QString();
         QString valueToTranslate = QString();

         while ( (query.next())) {
             if (query.isValid())
             {
                 valueToTranslate = (query.value(1)).toString();

                 if (valueToTranslate == "Bureau")
                 {
                     translatedValue = tr("Bureau", "Common term in hamradio, do not translate if not sure");
                 }
                 else if(valueToTranslate == "Direct")
                 {
                     translatedValue = tr("Direct");
                 }
                 else if(valueToTranslate == "Electronic")
                 {
                     translatedValue = tr("Electronic");
                 }
                 else if(valueToTranslate == "Manager")
                 {
                     translatedValue = tr("Manager", "Common term in hamradio, do not translate if not sure");
                 }
                 else
                 {
                    translatedValue = valueToTranslate;
                 }

                 aux.clear();
                 aux = (query.value(0)).toString() + " - " + translatedValue;
                 qs << aux;
             }
         }
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}

bool DataProxy_SQLite::haveAtLeastOneLog()
{
         //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;

    bool sqlOK = query.exec("SELECT COUNT(id) from logs");

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if((query.value(0)).toInt()>0)
            {
                query.finish();
                return true;
            }
            else
            {
                query.finish();
                return false;
            }
        }
        else
        {
            query.finish();
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    //return false;
}

QStringList DataProxy_SQLite::getColumnNamesFromTableLog()
{
       //qDebug() << Q_FUNC_INFO << " -";
    return getColumnNamesFromTable("log");
}

QStringList DataProxy_SQLite::getColumnNamesFromTable(const QString &_tableName)
{
       //qDebug() << Q_FUNC_INFO << " -;
    return db->getColumnNamesFromTable(_tableName);
}

bool DataProxy_SQLite::addDXCCEntitySubdivision(const QString &_name, const QString &_short, const QString &_pref,
                                                const QString &_group, const int _regId,
                                                const int _dxcc, const int _cq, const int _itu,
                                                const QDate &_startDate, const QDate &_endDate,
                                                const bool _deleted)
{
    //qDebug() << Q_FUNC_INFO << " - length: " << _name;
    // id / name / shortname / prefix / regionalgroup / regionalid / dxcc / cqz / ituz / start_date / end_date / deleted
    QString queryString;
    QSqlQuery query;
    bool sqlOK = false;

    //qDebug() << Q_FUNC_INFO << " - Importing: " << _regionalAward.getRegionalAwardRefName(i) ;
    queryString = QString("INSERT INTO primary_subdivisions (name, shortname, prefix, regionalgroup, "
                          "regionalid, dxcc, cqz, ituz, start_date, end_date, deleted) "
                              "values ('%1','%2','%3', '%4','%5','%6', '%7','%8', '%9', '%10','%11')")
            .arg(_name).arg(_short).arg(_pref).arg(_group).arg(_regId).arg(_dxcc)
            .arg(_cq).arg(_itu).arg(util->getDateSQLiteStringFromDate(_startDate))
            .arg(util->getDateSQLiteStringFromDate(_endDate)).arg(util->boolToCharToSQLite(_deleted));

    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " - " << query.lastQuery();
        query.finish();
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - ERROR ";
        //qDebug() << Q_FUNC_INFO << " - - query error: " << QString::number(query.lastError().text());
        //qDebug() << Q_FUNC_INFO << " - LastQuery: " << query.lastQuery() ;
        //qDebug() << Q_FUNC_INFO << " - LastError-data: " << query.lastError().databaseText() ;
        //qDebug() << Q_FUNC_INFO << " - LastError-driver: " << query.lastError().driverText() ;
        //qDebug() << Q_FUNC_INFO << " - LastError-n: " << QString::number(query.lastError().text() );

        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - END" ;
    return true;
}

QList<PrimarySubdivision> DataProxy_SQLite::getPrimarySubDivisions(const int _entity, const QString &_pref)
{ // Returns the Primary Subdivision for an Entity
  // If _pref is empty, and entity >=0 we look for all the subdivisions of the Entity,
  // If _pref is not empty, we look for the subdivisions with that entity, if none, we look for the number.
   //qDebug() << Q_FUNC_INFO << " - Start: " << QString::number(_entity) << "/" << _pref;
    QList<PrimarySubdivision> list;
    list.clear();

    QSqlQuery query;
    QString queryString;
    if ((_pref.length()>0))
    {
       //qDebug() << Q_FUNC_INFO << " - Running for no pref, delivering ALL for the entity";
        queryString = QString("SELECT dxcc, prefix, name, shortname, cqz, ituz FROM primary_subdivisions WHERE prefix = :prefix ORDER BY shortname");
        query.prepare(queryString);
        query.bindValue(":prefix", _pref);
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - Running with a pref, delivering just for the prefix";
       //qDebug() << Q_FUNC_INFO << QString("If the entity is <=0 The list will be empty. Entity: %1").arg(_entity);
        if (_entity<=0)
        {
           //qDebug() << Q_FUNC_INFO << " - END: entity <= 0";
            return list;
        }
        queryString = QString("SELECT dxcc, prefix, name, shortname, cqz, ituz FROM primary_subdivisions WHERE dxcc = :dxcc ORDER BY shortname");
        query.prepare(queryString);
        query.bindValue(":dxcc", _entity);
    }

    bool sqlOK = query.exec();

    if (sqlOK)
    {
       //qDebug() << Q_FUNC_INFO << ": sqlOK true";

        while (query.next())
        {
            if (query.isValid())
            {
                PrimarySubdivision ps;
                ps.dxcc = (query.value(0)).toInt();
                ps.prefix = (query.value(1)).toString();
                ps.name = (query.value(2)).toString();
                ps.shortName = (query.value(3)).toString();
                ps.cqz = (query.value(4)).toInt();
                ps.ituz = (query.value(5)).toInt();

                list.append(ps);
               //qDebug() << Q_FUNC_INFO << " : " << ps.name ;
            }
            else
            {
               //qDebug() << Q_FUNC_INFO << ": query not valid";
            }
        }
       //qDebug() << Q_FUNC_INFO << ": query not next";
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << ": sqlOK FALSE";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        list.clear();
    }
    query.finish();

    //qDebug() << Q_FUNC_INFO << " - END - " << QString::number(list.count()) ;
    return list;
}


int DataProxy_SQLite::getNumberOfManagedLogs()
{
         //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;

    bool sqlOK = query.exec("SELECT COUNT (*) from logs");

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
    //return -1;
}

int DataProxy_SQLite::getMaxLogNumber()
{
    QSqlQuery query;
    QString queryString = QString("SELECT MAX(id) FROM logs");
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
    //return -1;
}

QStringList DataProxy_SQLite::getListOfManagedLogs()
{
    //This function returns the list of log IDs that are being managed
         //qDebug() << Q_FUNC_INFO << " -";
    QSqlQuery query;
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT id FROM logs");
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                qs << (query.value(0)).toString();
                     //qDebug() << Q_FUNC_INFO << " -: " << (query.value(0)).toString() ;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        qs.clear();
    }
    query.finish();
    return qs;
}

QString DataProxy_SQLite::getStationCallSignFromLog(const int _log)
{
         //qDebug() << Q_FUNC_INFO << " - " << QString::number(_log)<< QT_ENDL;
    QSqlQuery query;
    QString queryString = QString("SELECT stationcall FROM logs WHERE id='%1'").arg(_log);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << Q_FUNC_INFO << " - " <<  (query.value(0)).toString();
            QString v = (query.value(0)).toString();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << " - Not valid";
            query.finish();
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() << Q_FUNC_INFO << " - query failed";
        query.finish();
        return QString();
    }
         //qDebug() << Q_FUNC_INFO << " - END";
    //return QString();
}

QStringList DataProxy_SQLite::getStationCallSignsFromLog(const int _log)
{
   //qDebug() << Q_FUNC_INFO << " -";
   QStringList calls = QStringList();
   QSqlQuery query;
   QString queryString;
   bool sqlOK;
   if (doesThisLogExist(_log))
   {
       queryString = QString("SELECT DISTINCT station_callsign FROM log WHERE lognumber='%1'").arg(_log);
   }
   else
   {
       queryString = QString("SELECT DISTINCT station_callsign FROM log");
   }

   sqlOK = query.exec(queryString);

   if (sqlOK)
   {
       while(query.next())
       {
           if (query.isValid())
           {
               queryString = (query.value(0)).toString();
               if (queryString.length()>2)
               {
                   calls.append(queryString);
               }
                    //qDebug() << Q_FUNC_INFO << " -: " << queryString;
           }
           else
           {
               query.finish();
               //qDebug() << Q_FUNC_INFO << " --END-1 - fail";
               return QStringList();
           }
       }
       query.finish();
       calls.removeDuplicates();
   }
   else
   {
       emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
       query.finish();
       //qDebug() << Q_FUNC_INFO << " --END-2 - fail";
       return QStringList();
   }
   calls.sort();
   //qDebug() << Q_FUNC_INFO << " --END";
   return calls;
}

QStringList DataProxy_SQLite::getStationCallSignsFromLogWithLoTWPendingToSend(const int _log)
{
  //qDebug() << Q_FUNC_INFO << ": logNumber: " << _log;

    // Check if the log exists; return an empty list if it doesn't
    if (!doesThisLogExist(_log)) {
        return QStringList();
    }

    // Prepare the SQL query with placeholders
    QString queryString = "SELECT DISTINCT station_callsign FROM log WHERE lotw_qsl_sent = 'Q' AND lognumber = :logNumber";
    QSqlQuery query;

    // Bind parameters to the prepared query
    query.prepare(queryString);
    //query.bindValue(":lotwStatus", "Q");
    query.bindValue(":logNumber", _log);

    // Execute the query
    if (!query.exec()) {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.executedQuery());
        return QStringList();
    }

    // Collect valid call signs
    QStringList calls;
    while (query.next()) {
        QString callSign = query.value(0).toString().trimmed();
        if (callSign.length() > 2) { // Ensure valid call signs
            calls.append(callSign);
        }
    }

    // Remove duplicates and sort the results
    calls.removeDuplicates();
    calls.sort();

    return calls;
}

QString DataProxy_SQLite::getOperatorsFromLog(const int _log)
{
    //qDebug() << Q_FUNC_INFO << " - " << QString::number(_log)<< QT_ENDL;
    QSqlQuery query;
    QString queryString = QString("SELECT operators FROM logs WHERE id='%1'").arg(_log);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
               //qDebug() << Q_FUNC_INFO << " - " <<  (query.value(0)).toString();
            QString v = (query.value(0)).toString();
            query.finish();
            return v;
        }
        else
        {
               //qDebug() << Q_FUNC_INFO << " - Not valid";
            query.finish();
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
           //qDebug() << Q_FUNC_INFO << " - query failed";
        query.finish();
        return QString();
    }

  //qDebug() << Q_FUNC_INFO << " - END";
  //return QString();
}

QString DataProxy_SQLite::getCommentsFromLog(const int _log)
{
 //qDebug() << Q_FUNC_INFO << ": " << QString::number(_log);
  QSqlQuery query;
  QString queryString = QString("SELECT comment FROM logs WHERE id='%1'").arg(_log);
  bool sqlOK = query.exec(queryString);

  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
       //qDebug() << Q_FUNC_INFO << ": " << (query.value(0)).toString();
        QString v = (query.value(0)).toString();
        query.finish();
        return v;
      }
      else
      {
           //qDebug() << Q_FUNC_INFO << ": Not valid";
          query.finish();
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
       //qDebug() << Q_FUNC_INFO << ":  query failed";
      query.finish();
      return QString();
  }
}

QString DataProxy_SQLite::getLogDateFromLog(const int _log)
{
       //qDebug() << Q_FUNC_INFO << " - " << QString::number(_log)<< QT_ENDL;
  QSqlQuery query;
  QString queryString = QString("SELECT logdate FROM logs WHERE id='%1'").arg(_log);
  bool sqlOK = query.exec(queryString);

  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
               //qDebug() << Q_FUNC_INFO << " - " <<  (query.value(0)).toString();
          QString v = (query.value(0)).toString();
          query.finish();
          return v;
      }
      else
      {
               //qDebug() << Q_FUNC_INFO << " - Not valid";
          query.finish();
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
           //qDebug() << Q_FUNC_INFO << " - query failed";
      query.finish();
      return QString();
  }
       //qDebug() << Q_FUNC_INFO << " - END";
  //return QString();
}

int DataProxy_SQLite::getLogNumberFromQSOId(const int _qsoId)
{
    QSqlQuery query;
    QString queryString = QString("SELECT lognumber FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
}


/*
bool DataProxy_SQLite::fillEmptyDXCCInTheLog()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    int qsos = getHowManyEmptyDXCCorCont();
    if (qsos < 1)
    {
        return true;
    }
    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

    QString queryString = QString("SELECT id, call FROM log WHERE dxcc IS NULL OR dxcc<1 OR cont IS NULL");
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        int nameCol = -1;

        QSqlRecord rec = query.record();
        QString _call = QString();
        QString _id = QString();
        QString _dxcc = QString();
        QString _aux = QString();
        QString _continent = QString();
        int j = 0;

        while (query.next())
        {
            if (query.isValid())
            {
                nameCol = rec.indexOf("id");
                _id = (query.value(nameCol)).toString();
                nameCol = rec.indexOf("call");
                _call = (query.value(nameCol)).toString();
                _dxcc = QString::number(getPrefixId(_call));
                _continent = getContinentShortNameFromEntity(_dxcc.toInt());
                   //qDebug() << Q_FUNC_INFO << ":   DXCC: " << _dxcc;
                   //qDebug() << Q_FUNC_INFO << ":   Cont: " << _continent;
                // UPDATE THE ID WITH THE DXCC
                sqlOK = updateDXCCAndContinent(_id.toInt(), _dxcc.toInt(), _continent);
                if (!sqlOK)
                {
                    query.finish();
                    return false;
                }
                if (( (j % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    _aux = QObject::tr("Updating DXCC and Continent information...") + "\n" + QObject::tr("QSO: ")  + QString::number(j) + "/" + QString::number(qsos);
                    //_aux = "Updating ...";
                    progress.setLabelText(_aux);
                    progress.setValue(j);
                }
                if ( progress.wasCanceled() )
                {
                    //qDebug() << Q_FUNC_INFO << ":   progress canceled";
                    query.finish();
                    return true;
                }
                j++;
            }
        }
        query.finish();
        progress.setValue(qsos);
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog DXCC"));
        msgBox.setText(tr("All QSOs have been updated with a DXCC and the Continent.") );
        msgBox.exec();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    query.finish();
    return true;
}

*/


int DataProxy_SQLite::getHowManyQSOInLog(const int _log)
{
    QString queryString = QString();
    QSqlQuery query = QSqlQuery();
    if (_log < 0)
    {
        queryString = QString("SELECT count(id) FROM log");
    }
    else
    {
        queryString = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(_log);
    }

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getHowManyConfirmedQSLInLog(const int _log)
{
    if (!doesThisLogExist(_log) && (_log>0))
    {
       return 0;
    }
    QString queryString = QString();
    QSqlQuery query;
    if (_log < 0)
    {
        queryString = QString("SELECT count(id) FROM log WHERE (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y')");
    }
    else
    {
        queryString = QString("SELECT count(id) FROM log WHERE (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y') AND lognumber='%1'").arg(_log);
    }

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getHowManyQSLSentInLog(const int _log)
{
    if (!doesThisLogExist(_log) && (_log>0))
    {
       return 0;
    }
    QString queryString = QString();
    QSqlQuery query;
    if (_log < 0)
    {
        queryString = QString("SELECT count(id) FROM log WHERE qsl_sent='Y'");
    }
    else
    {
        queryString = QString("SELECT count(id) FROM log WHERE qsl_sent='Y' AND lognumber='%1'").arg(_log);
    }

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getQSOsWithContinent(const QString &_cont, const int _logNumber)
{
       //qDebug() << Q_FUNC_INFO << " - " << _cont;
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    if (_logNumber < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log where continent = '%1'").arg(_cont);
    }
    else
    {
        //queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND cont LIKE '%2'").arg(_logNumber).arg(_cont);
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND cont='%2'").arg(_logNumber).arg(_cont);
    }

    sqlOK = query.exec(queryString);

         //qDebug() << Q_FUNC_INFO << " - queryString: " << queryString;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << Q_FUNC_INFO << " - " << QString::number((query.value(0)).toInt());
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << " - 0";
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() << Q_FUNC_INFO << " - Query error";
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getQSOsInBand(const QString &_band, const int _log)
{
    if (!doesThisLogExist(_log) && (_log>0))
    {
       return 0;
    }
    int bandId = getIdFromBandName(_band);
    if ( bandId < 0)
    {
        return 0;
    }

    QString queryString = QString();
    QSqlQuery query;
    if (_log < 0)
    {
        queryString = QString("SELECT count(id) FROM log WHERE bandid='%1'").arg(bandId);
    }
    else
    {
        queryString = QString("SELECT count(id) FROM log WHERE bandid='%1' AND lognumber='%2'").arg(bandId).arg(_log);
    }

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getQSOsInMode(const QString &_mode, const int _log)
{
    if (!doesThisLogExist(_log) && (_log>0))
    {
       return 0;
    }
    int modeId = getIdFromModeName(_mode);
    if ( modeId < 0)
    {
        return 0;
    }

    QString queryString = QString();
    QSqlQuery query;
    if (_log < 0)
    {
        queryString = QString("SELECT count(id) FROM log WHERE modeid='%1'").arg(modeId);
    }
    else
    {
        queryString = QString("SELECT count(id) FROM log WHERE modeid='%1' AND lognumber='%2'").arg(modeId).arg(_log);
    }

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getDXCCInBand(const int _bandid, const bool _confirmed, const int _log)
{ //(SELECT COUNT (DISTINCT dxcc) from log inner join band on log.bandid = band.id where band.name="80M")
    Q_UNUSED(_bandid);
    if (!doesThisLogExist(_log) && (_log>0))
    {
       return 0;
    }

    QChar _confirmedChar = 'N';
    if (_confirmed)
    {
        _confirmedChar = 'Y';
    }

    QString queryString;
    QSqlQuery query;
    if (_log < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT dxcc) from log where band.id= :bandid AND (qsl_rcvd= :confirmed OR lotw_qsl_rcvd= :confirmed)");
    }
    else
    {
        queryString = QString("SELECT COUNT (DISTINCT dxcc) from log where band.id= :bandid AND log.id= :log AND (qsl_rcvd= :confirmed OR lotw_qsl_rcvd= :confirmed)");
    }
    if (!query.prepare (queryString))
    {
        return 0;
    }
    query.bindValue (":confirmed", _confirmedChar);
    query.bindValue (":log", _log);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
}

QList<QList<int>> DataProxy_SQLite::getTop10QSOPerDXCC(const int _log)
{
    //qDebug() << Q_FUNC_INFO << " : " << QString::number(_log);
    QList<QList<int>> result;
    result.clear();
    QSqlQuery query;
    QString queryString;
    if (_log <0)
    {
        queryString = "SELECT log.dxcc, COUNT(log.dxcc) FROM log GROUP BY log.dxcc ORDER BY COUNT(log.dxcc) DESC LIMIT 10";
    }
    else
    {
        queryString = QString("select log.dxcc, count(log.dxcc) from log WHERE log.lognumber = '%1' group by log.dxcc ORDER BY count(log.dxcc) DESC limit 10").arg(_log);
    }
    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        QList<int> temp;
        while (query.next())
        {
            if (query.isValid())
            {
                temp.clear();
                temp.append(query.value(0).toInt());
                temp.append(query.value(1).toInt());
                result.append(temp);
                //_id = (query.value(nameCol)).toString();
            }
        }
        return result;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return result;
    }
    //qDebug() << Q_FUNC_INFO << " - END" ;
    //return result;
}

bool DataProxy_SQLite::addNewLog (const QStringList _qs)
{
        //qDebug() << Q_FUNC_INFO << " - " << _qs.at(2) << "/" << _qs.at(5) << "/" << _qs.at(6);
      //qDebug() << Q_FUNC_INFO << " - Size: " << QString::number(_qs.size());
    // newLogq << dateString << stationCallsign << operators << comment << QString::number(selectedLog) << _qs.at(4) ; (last field is 1 or 0 editing)

    if (_qs.size()!=6)
    {
          //qDebug() << Q_FUNC_INFO << " - != 6" ;
        return false;
    }
      //qDebug() << Q_FUNC_INFO << " - Has the appropriate length" ;

    QString aux = QString();

    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);
    QString _operators = _qs.at(2);
    QString _comment = _qs.at(3);

    QString id = _qs.at(4);
    QString editing = _qs.at(5);

    QString queryString;
    QSqlQuery query;
    bool sqlOK;

    if (editing == "1")
    { // We are editing
            //qDebug() << Q_FUNC_INFO << " - We are editing!";
        //queryString = QString("UPDATE logs SET logdate = '%1', stationcall = '%2', operators = '%3', comment = '%4',  logtype = '%5', logtypen = '%6' WHERE id = '%7'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(_typeContest).arg(_typeContestN).arg(id);
        queryString = QString("UPDATE logs SET logdate = '%1', stationcall = '%2', operators = '%3', comment = '%4' WHERE id = '%5'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(id);
        sqlOK = query.exec(queryString);

        if (sqlOK)
        {
            //qDebug() << Q_FUNC_INFO << " - Editing OK!";
            query.finish();
            return true;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - Editing NOK!";
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            query.finish();
            return false;
        }
        //return false;
    }

    //qDebug() << Q_FUNC_INFO << " - We are adding a new log";

    // First we check if the log is already there

    queryString = QString("SELECT id FROM logs WHERE logdate='%1' AND stationcall='%2' AND operators = '%3' AND comment = '%4'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment);
    //"logs"
    //"id, logdate, stationcall, comment, logtype"
    //qDebug() << Q_FUNC_INFO << " - query1: " << queryString;

    sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            //qDebug() << Q_FUNC_INFO << " - query error: " << queryString;
            // It seems that the log is already existing!
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }

    //Now we add the new log
    queryString = QString("INSERT INTO logs (logdate, stationcall, operators, comment) values('%1','%2','%3','%4')").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment);
     //qDebug() << Q_FUNC_INFO << " - query1: " << queryString;
    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    //return false;
}

bool DataProxy_SQLite::doesThisLogExist(const int _log)
{
    //qDebug() << Q_FUNC_INFO << " - " << QString::number(_log);
    //qDebug() << Q_FUNC_INFO << " - - Name:" << db->getDBName();
    QSqlQuery query;
    //SELECT COUNT (*) FROM log WHERE lognumber='3'
    //SELECT COUNT (*) FROM logs WHERE id='1'

    QString queryString = QString("SELECT COUNT (*) FROM logs WHERE id='%1'").arg(_log);
    bool sqlOK = query.exec(queryString);

    //qDebug() << Q_FUNC_INFO << " query: " << query.lastQuery() ;

    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " - END False 1";
        return false;
    }

    if (!query.next())
    {
        //qDebug() << Q_FUNC_INFO << " - END False 2";
        return false;
    }
    if (!query.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - END False 3";
        return false;
    }
    return (query.value(0).toInt() >0);

    /*
    //int i = query.value(0).toInt();
    //qDebug() << Q_FUNC_INFO << " - Value: " << QString::number(i);
    //qDebug() << Q_FUNC_INFO << " - END TRUE";
    return true;

    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                query.finish();
                //qDebug() << Q_FUNC_INFO << " - END TRUE" ;
                return true;
            }
            else
            {
                query.finish();
                //qDebug() << Q_FUNC_INFO << " - END FALSE 1" ;
                return false;
            }
        }
        else
        {
            query.finish();
             //qDebug() << Q_FUNC_INFO << " - END FALSE 2" ;
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        //qDebug() << Q_FUNC_INFO << " - END FALSE 3" ;
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - END FALSE 4" ;
    return false;
    */
}

int DataProxy_SQLite::getContinentIdFromContinentShortName(const QString &_n)
{
    if (_n.length()!=2)
    {
        return -3;
    }
    QSqlQuery query;
    QString queryString = QString("SELECT id FROM continent WHERE shortname=='%1'").arg(_n);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -2;
    }
}

QString DataProxy_SQLite::getContinentShortNameFromEntity(const int _n)
{
    // SELECT DISTINCT dxcc, bandid, modeid, id, lognumber from log WHERE qsl_rcvd='Y' OR lotw_qsl_rcvd='Y' GROUP BY dxcc, bandid, modeid
    QSqlQuery query;

    //QString queryString= QString("SELECT continent FROM entity WHERE dxcc='%1'").arg(_n);
    QString queryString= QString("SELECT continent.shortname FROM entity JOIN continent ON entity.continent=continent.id WHERE entity.dxcc='%1'").arg(_n);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            queryString = (query.value(0)).toString();
            query.finish();
            return queryString;
        }
        else
        {
            query.finish();
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return  QString();
    }
}

int DataProxy_SQLite::getContinentIdFromEntity(const int _n)
{
    QSqlQuery query;
    QString queryString = QString("SELECT continent.id FROM entity JOIN continent ON entity.continent=continent.shortname WHERE dxcc='%1'").arg(_n);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -2;
    }
}

QStringList DataProxy_SQLite::getContinentShortNames()
{
    QSqlQuery query;
    QStringList continents;
    continents.clear();
    QString queryString = QString("SELECT shortname FROM continent");
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                continents << query.value(0).toString();
            }
        }
        query.finish();
        continents.sort();
        return continents;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QStringList();
    }
    //return QStringList();
}

bool DataProxy_SQLite::isValidContinentShortName(const QString &_n)
{
    QString queryString = QString("SELECT id FROM continent WHERE shortname ='%1'").arg(_n);
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                query.finish();
                return true;
            }
            else
            {
                query.finish();
                return false;
            }
        }
        else
        {
            query.finish();
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
}

bool DataProxy_SQLite::isValidDXCC(const int _e)
{
    QString queryString = QString("SELECT id FROM entity WHERE dxcc ='%1'").arg(_e);
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                if (query.value(0).toInt()==1)
                query.finish();
                return true;
            }
            else
            {
                query.finish();
                return false;
            }
        }
        else
        {
            query.finish();
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
}

QStringList DataProxy_SQLite::filterValidFields(const QStringList &_fields)
{
    const QStringList validFields = getFields();
    QStringList returningFields;

    for (QString field : _fields)
    {
        field = field.trimmed();
        if (field.endsWith(';'))
        {
            field.chop(1);
        }
        if (validFields.contains(field))
        {
            returningFields << field;
        }
    }

    // If no valid fields are found, return default fields
    if (returningFields.isEmpty())
    {
        returningFields = util->getDefaultLogFields();
    }

    return returningFields;
}

int DataProxy_SQLite::getITUzFromPrefix(const QString &_p)
{
    QSqlQuery query;
    //QString queryString = QString("SELECT ituz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
    QString queryString = QString("SELECT ituz FROM prefixesofentity WHERE prefix = '%1'").arg(_p);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -2;
    }
}

int DataProxy_SQLite::getCQzFromPrefix(const QString &_p)
{
    QSqlQuery query;
    //QString queryString = QString("SELECT cqz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
    QString queryString = QString("SELECT cqz FROM prefixesofentity WHERE prefix = '%1'").arg(_p);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -2;
    }
}

int DataProxy_SQLite::getCQzFromEntity(const int _n)
{
    QSqlQuery query;
    QString queryString = QString("SELECT cqz FROM entity WHERE dxcc='%1'").arg(_n);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -2;
    }
}

int DataProxy_SQLite::getITUzFromEntity(const int _n)
{
    QSqlQuery query;
    QString queryString = QString("SELECT ituz FROM entity WHERE dxcc='%1'").arg(_n);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -2;
    }
}

QString DataProxy_SQLite::getEntityNameFromId(const int _n)
{
  //qDebug() << Q_FUNC_INFO << " - " << QString::number(_n);

    QSqlQuery query;
    QString queryString = QString("SELECT name FROM entity WHERE dxcc='%1'").arg(_n);
    QString motherEntName = QString();
    bool sqlOK;

    if (_n > 1000)
    {
        QString aux = (QString::number(_n)).right(3);
        QString queryString2 = QString("SELECT name FROM entity WHERE dxcc='%1'").arg(aux);
        sqlOK = query.exec(queryString2);

        if (sqlOK)
        {
            if (query.next())
            {
                if (query.isValid())
                {
                    motherEntName = (query.value(0)).toString();
                }
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        }
    }
    query.finish();
    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                if (_n>1000)
                {
                    motherEntName = (query.value(0)).toString() + " (" + motherEntName + ")";
                }
                else
                {
                    motherEntName = (query.value(0)).toString();
                }
                query.finish();
                return motherEntName;
            }
            else
            {
                query.finish();
                return QString();
            }
        }
        else
        {
            query.finish();
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }
    //return QString();
}

int DataProxy_SQLite::getEntityIdFromName(const QString &_e)
{
    //qDebug() << Q_FUNC_INFO << " -" << _e;

    int id = -1;
  QString queryString;
  QSqlQuery query;

  queryString = QString("SELECT dxcc FROM entity WHERE name='%1'").arg(_e);
  //queryString = "SELECT prefix FROM prefixesofentity WHERE dxcc=='" + QString::number(i) +"'";
  bool sqlOK = query.exec(queryString);

  if (!sqlOK)
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
      query.finish();
      return id;
  }
  else
  {
    query.next();
    if (query.isValid())
    {
        id = (query.value(0)).toInt();
        query.finish();
        return id;
    }
      else
      {
        query.finish();
          return -1;
      }
  }
}

QMap<EntityData, int> DataProxy_SQLite::getAllEntiNameISOAndPrefix()
{
    QMap<EntityData, int> entities;
    entities.clear();

    QString queryString("SELECT dxcc, name, isoname, mainprefix FROM entity");
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
      query.finish ();
      return entities;
    }
    while (query.next())
    {
      if (!query.isValid())
          continue;
      int dxcc = (query.value(0)).toInt();
      EntityData entity;
      entity.dxcc       = dxcc;
      entity.name       = (query.value(1)).toString();
      entity.isoname    = (query.value(2)).toString();
      entity.mainprefix = (query.value(3)).toString();
      entities.insert(entity, dxcc);
    }
    return entities;
}

QString DataProxy_SQLite::getEntityMainPrefix(const int _entityN)
{
    //qDebug() << Q_FUNC_INFO << " -" << QString::number(_entityN);

    if (_entityN <= 0 )
    {
        return QString();
    }

    QString queryString;
    QSqlQuery query;

    queryString = QString("SELECT mainprefix FROM entity WHERE dxcc='%1'").arg(_entityN);

    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }
    else
    {
        query.next();

        if (query.isValid())
        {
            queryString = (query.value(0)).toString();
            query.finish();
            return queryString;
        }
        else
        {
            query.finish();
            return QString();
        }
    }
    //return QString();
}

int DataProxy_SQLite::getEntityIdFromMainPrefix(const QString &_e)
{
      //qDebug() << Q_FUNC_INFO << " -" << _e;
    int id = -1;
    QString queryString;
    QSqlQuery query;
    queryString = QString("SELECT dxcc FROM entity WHERE mainprefix='%1'").arg(_e);
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return id;
    }
    else
    {
        query.next();
        if (query.isValid())
        {
            id = (query.value(0)).toInt();
            query.finish();
            return id;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
}

/*
int DataProxy_SQLite::getDXCCFromPrefix(const QString &_p)
{
    //qDebug() << Q_FUNC_INFO << " - " << _p << "-";

    QSqlQuery query;
    QString queryString = QString("SELECT dxcc FROM prefixesofentity WHERE prefix='%1'").arg(_p);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
          //qDebug() << Q_FUNC_INFO << ": query OK: query: " << queryString;
        if (query.next())
        {
            if (query.isValid())
            {
                int v = (query.value(0)).toInt();
                query.finish();
                  //qDebug() << Q_FUNC_INFO << ": return 0: " << QString::number(v) ;
                return v;
            }
            else
            {
                query.finish();
                  //qDebug() << Q_FUNC_INFO << ": return -1: ";
                return -1;
            }
        }
        else
        {
            query.finish();
              //qDebug() << Q_FUNC_INFO << ": return -2: ";
            return -2;
        }
    }
    else
    {
          //qDebug() << Q_FUNC_INFO << ": query NOK: query: " << queryString;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
          //qDebug() << Q_FUNC_INFO << ": return -3: ";
        return -3;
    }
}
*/

bool DataProxy_SQLite::isNewCQz(int _c)
{
    QSqlQuery query;
    QString queryString = QString("SELECT id FROM log WHERE cqz='%1'").arg(_c);
    //queryString = "SELECT id FROM log WHERE cqz=='" + QString::number(_cqz) +"'";
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    else
    {
        query.next();
        return query.isValid();
    }
}

bool DataProxy_SQLite::isNewEntity(int _e)
{
    if (_e <= 0)
    {
        return false;
    }

    //QString queryString;
    QSqlQuery query;
    QString queryString = QString("SELECT id FROM log WHERE dxcc='%1'").arg(_e);
    //queryString = "SELECT id FROM log WHERE dxcc=='" + QString::number(_entityN) +"'";

    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    else
    {
        query.next();

        if (query.isValid())
        {
            query.finish();
            return true;
        }
        else
        {
            query.finish();
            return false;
        }
    }
    //return false;
}

double DataProxy_SQLite::getLongitudeFromEntity(const int _e)
{
    QString queryString = QString("SELECT longitude FROM entity WHERE dxcc='%1'").arg(_e);
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) )
        {
            query.finish();
            return 0.0;
        }
        else
        {   double v = (query.value(0)).toDouble();
            query.finish();
            return -v;
        }
    }
}

double DataProxy_SQLite::getLatitudeFromEntity(const int _e)
{
    QString queryString = QString("SELECT latitude FROM entity WHERE dxcc='%1'").arg(_e);
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();

        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) )
        {
            query.finish();
            return 0.0;
        }
        else
        {
            double v = (query.value(0)).toDouble();
            query.finish();
            return v;
        }
    }
}

QString DataProxy_SQLite::getEntityPrefixes(const int _enti)
{
    if (_enti<=0)
    {
        return QString();
    }
    QString result;
    result = QString();
    QString queryString;
    QSqlQuery query;
    int i = _enti;
    queryString = "SELECT prefix FROM prefixesofentity WHERE dxcc=='" + QString::number(i) +"'";
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return QString();
    }
    else
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                result = result + ", " + (query.value(0)).toString();
            }
        }
        if (result.length() < 1)
        {
            return result;
        }
        else
        {
            result = result.remove(0,2);
            query.finish();
            return result;
        }
    }
}

QStringList DataProxy_SQLite::getSpecialCallsigns()
{
    //qDebug() << Q_FUNC_INFO;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT prefix from prefixesofentity WHERE prefix like '=%'");
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                //aux.clear();
                aux = (query.value(0)).toString();
                aux = aux.remove(0,1);
                qs << aux;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    qs.sort();
    //qDebug() << Q_FUNC_INFO << ": count: " << QString::number(qs.count());
    return qs;
}

QHash<QString, int> DataProxy_SQLite::getWorldData()
{
    //qDebug() << Q_FUNC_INFO << "Start";
    QHash<QString, int> world;
    world.clear();

    QString queryString;
    QSqlQuery query;
    QString pref;

    queryString = "SELECT prefix, dxcc FROM prefixesofentity";
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
       //qDebug() << Q_FUNC_INFO << "END-FAIL-1 - !sqlOK";
        return world;
    }
    else
    {
        while ( (query.next()))
        {
            if (query.isValid())
            {
               //qDebug() << Q_FUNC_INFO << QString("Pref/Ent = %1/%2").arg((query.value(0)).toString()).arg((query.value(1)).toInt());
                pref = (query.value(0)).toString();
                if (pref.startsWith('='))
                {
                    pref.remove(0,1);
                }
                world.insert(pref, (query.value(1)).toInt());
            }
            else
            {
                query.finish();
                world.clear();
               //qDebug() << Q_FUNC_INFO << "END-FAIL - Query not valid";
                return world;
            }
        }
    }
    query.finish();
    //qDebug() << Q_FUNC_INFO << "END";
   //qDebug() << Q_FUNC_INFO << ": count: " << QString::number(world.count());
    return world;
}

QStringList DataProxy_SQLite::getLongPrefixes()
{//select prefix FROM prefixesofentity WHERE (length(prefix)>2) AND (length(prefix)<6)  AND (prefix NOT LIKE '%/%')
    //qDebug() << Q_FUNC_INFO;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT prefix from prefixesofentity WHERE prefix NOT like '=%'");
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(0)).toString();
                qs << aux;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    qs.sort();
    //foreach(aux, qs)
    //{
    //    //qDebug() << aux;
    //}
    return qs;
}

QStringList DataProxy_SQLite::getEntitiesNames()
{
         //qDebug()  << Q_FUNC_INFO << " -" ;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT mainprefix, name, dxcc FROM entity");
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                if (query.value(2).toInt()<1000)
                {
                    aux.clear();
                    aux = (query.value(0)).toString() + "-" + (query.value(1)).toString()+" ("+(query.value(2)).toString()+")";
                    //result = result + ", " + (query.value(0)).toString();
                    qs << aux;
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    qs.sort();
    return qs;
}

QStringList DataProxy_SQLite::getEntitiesIds()
{
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT dxcc FROM entity");
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                if (query.value(0).toInt()>0)
                {
                    aux.clear();
                    aux = (query.value(0)).toString();
                    qs << aux;
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    qs.sort();
    return qs;
}

int DataProxy_SQLite::getHowManyEntities()
{
    QSqlQuery query;
    QString queryString = QString("SELECT count(id) FROM entity");
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getMaxEntityID(bool limit)
{
    //SELECT MAX (dxcc) FROM entity WHERE dxcc<1000
    QSqlQuery query;
    QString  queryString;
    if (limit)
    {
        queryString = QString("SELECT MAX (dxcc) FROM entity WHERE dxcc<1000");
    }
    else
    {
        queryString = QString("SELECT MAX (dxcc) FROM entity");
    }

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
}

QList<int> DataProxy_SQLite::getListOfDXCCIds()
{
    QSqlQuery query;
    QString  queryString =  QString("SELECT dxcc FROM entity");
    QList<int> entities;
    entities.clear();

    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return entities;
    }
    while(query.next())
    {
        if (query.isValid())
        {
            entities.append((query.value(0)).toInt());
        }
    }
    query.finish();
    //int i;
    //foreach (i, entities) {
    //   //qDebug() << Q_FUNC_INFO << " - " << QString::number(i);
    //}
    return entities;
}

bool DataProxy_SQLite::updateISONames()
{
    //qDebug()  << Q_FUNC_INFO << " -" ;
    return db->updateTheEntityTableISONames();
}

QString DataProxy_SQLite::getISOName(const int _n)
{
    //qDebug()  << Q_FUNC_INFO << " - " << QString::number(_n) ;
    if (_n <= 0 )
    {
             //qDebug()  << Q_FUNC_INFO << " - NOT KNOWN - UN";
        return "un"; // When no flag is known, we return the UN flag
    }
    int n = _n;
    if (_n<1000)
    {
        n = _n;
    }
    else if (_n>=2000)
    {
        n = _n - 2000;
    }
    else
    {
        n = _n - 1000;
    }
    QString queryString, aux;
    QSqlQuery query;
    aux.clear();
    queryString = QString("SELECT isoname FROM entity WHERE dxcc='%1'").arg(n);
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug()  << Q_FUNC_INFO << " - Query error - UN" ;
        query.finish();
        return "un"; // When no flag is known, we return the UN flag
    }
    else
    {
        query.next();

        if (query.isValid()){
            //qDebug()  << Q_FUNC_INFO << " - " << "N: " << QString::number(_n) << "- ISO Name: " << (query.value(0)).toString();
            aux = (query.value(0)).toString();
            query.finish();
            if (aux.length()>1)
            {
                return aux;
            }
            else
            {
                return "un"; // When no flag is known, we return the UN flag
            }
        }
        else
        {
            //qDebug()  << Q_FUNC_INFO << " - NO ISO Name: " ;
            query.finish();
            return "un"; // When no flag is known, we return the UN flag
        }
    }
}

 bool DataProxy_SQLite::addPrimarySubdivisions()
 {
    //qDebug() << Q_FUNC_INFO;
     return db->populateTablePrimarySubdivisions();
 }

 /*
int DataProxy_SQLite::getPrefixId(const QString &_qrz)
{
    //qDebug() << Q_FUNC_INFO << ": -" << _qrz <<"-";
    //TODO: Instead of going from long to short, identify prefixes from the begining:
    // character(may be number) + number

    Callsign callsign(_qrz);
    if (!callsign.isValid())
        return -1;
    if (!callsign.isValidPrefix())
        return -2;

    QString aux = callsign.getHostFullPrefix();
    int entityID = 0;

    while ((entityID <= 0) && (aux.length()>=1) )
    {
        entityID = getDXCCFromPrefix(aux);

            //qDebug() << Q_FUNC_INFO << ": in the while" << aux << " = " <<  QString::number(entityID);
         if (entityID<=0)
         {
             aux.chop(1);
         }
    }
    //qDebug() << Q_FUNC_INFO << ": " <<  _qrz << QString::number(entityID);
    return entityID;
}
*/

void DataProxy_SQLite::setLogLevel (const DebugLogLevel _l)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    logLevel = _l;
    db->setLogLevel(logLevel);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void DataProxy_SQLite::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    if (logLevel<=_level)
        emit debugLog (_func, _msg, _level);
}
/*
QString DataProxy_SQLite::getADIFQSO(const int _qsoId, ExportMode _em)
{
    // Called from eLogQrzLog::sendQSO just for the QRZ.com QSO send

    //qDebug() << Q_FUNC_INFO << ": " <<  QString::number(_qsoId);
    QSqlQuery query;
    QString queryString = QString("SELECT * FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOk = query.exec(queryString);

    if (!sqlOk)
        return QString();
    if (!query.next())
        return QString();
    if (!query.isValid())
        return QString();
    QSqlRecord rec = query.record();

    return getADIFFromQSOQuery(rec, _em, false, false, -1);
}
*/

QString DataProxy_SQLite::getADIFValueFromRec(QSqlRecord _rec, const QString &_fieldName)
{// To refactor the getADIFFromQSOQuery function
    int nameCol = _rec.indexOf(_fieldName);
    if (nameCol>=0)
    {
        QSqlField field = _rec.field(nameCol);
        return (field.value()).toString();
    }
    return QString();
}


QString DataProxy_SQLite::getADIFFromQSOQuery(QSqlRecord rec, ExportMode _em, bool _justMarked, bool _onlyRequested, const int _logN )
{   //qDebug() << Q_FUNC_INFO << ": " <<  query.lastQuery();
    //qDebug() << Q_FUNC_INFO << ": START";
    int nameCol;
    QString aux;

    QSO qso;
    qso.clear();
    QSqlField field;

    if (_justMarked)
    {
        nameCol = rec.indexOf("marked");
        field = rec.field(nameCol);
        aux = (field.value()).toString();
        if (aux != "X")
        {
            return QString();
        }
    }

    if (_onlyRequested)
    {
        nameCol = rec.indexOf("qsl_sent");
        field = rec.field(nameCol);
        aux = (field.value()).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        if ( aux !="R" )
        {
            return QString();
        }
    }

    qso.setCall(getADIFValueFromRec(rec, "call"));

    QDateTime tDateTime;
    aux = getADIFValueFromRec(rec, "qso_date");
    tDateTime = util->getDateTimeFromSQLiteString(aux);
    qso.setDateTimeOn(tDateTime);

    aux = getADIFValueFromRec(rec, "bandid");
    qso.setBand(getNameFromBandId(aux.toInt()));

    aux = getADIFValueFromRec(rec, "freq");
    qso.setFreq(aux.toDouble());

    aux = getADIFValueFromRec(rec, "band_rx");
    aux = getNameFromBandId(aux.toInt());
    qso.setBandRX(aux);

    aux = getADIFValueFromRec(rec, "freq_rx");
    qso.setFreqRX((aux.toDouble()));

    aux = getADIFValueFromRec(rec, "modeid");
    QString aux2 = getSubModeFromId(aux.toInt());
    aux = getNameFromSubMode(aux2);
    qso.setMode(aux);
    qso.setSubmode(aux2);

    qso.setPropMode(getADIFValueFromRec(rec, "prop_mode"));
    qso.setSatName(getADIFValueFromRec(rec, "sat_name"));

    qso.setGridSquare(getADIFValueFromRec(rec, "gridsquare"));
    qso.setMyGridSquare(getADIFValueFromRec(rec, "my_gridsquare"));

    if (_em != ModeEQSL)
        qso.setStationCallsign(getADIFValueFromRec(rec, "station_callsign"));

    aux = getADIFValueFromRec(rec, "qso_date_off");
    tDateTime = util->getDateTimeFromSQLiteString(aux);
    qso.setDateOff(tDateTime.date());
    qso.setTimeOff(tDateTime.time());

    qso.setSrx((getADIFValueFromRec(rec, "srx")).toInt());
    qso.setSrxString(getADIFValueFromRec(rec, "srx_string"));

    qso.setStx((getADIFValueFromRec(rec, "stx")).toInt());
    qso.setStxString(getADIFValueFromRec(rec, "stx_string"));

    qso.setCQZone((getADIFValueFromRec(rec, "cqz")).toInt());
    qso.setItuZone((getADIFValueFromRec(rec, "ituz")).toInt());
    qso.setDXCC((getADIFValueFromRec(rec, "dxcc")).toInt());
    //qDebug() << Q_FUNC_INFO << ":  - 100";

    qso.setAddress(getADIFValueFromRec(rec, "address"));
    qso.setAge((getADIFValueFromRec(rec, "age")).toDouble());

    qso.setCounty(getADIFValueFromRec(rec, "cnty"));
    qso.setComment(getADIFValueFromRec(rec, "comment"));

    qso.setA_Index((getADIFValueFromRec(rec, "a_index")).toInt());

    qso.setAnt_az((getADIFValueFromRec(rec, "ant_az")).toDouble());
    qso.setAnt_el((getADIFValueFromRec(rec, "ant_al")).toDouble());
    qso.setAnt_Path(getADIFValueFromRec(rec, "ant_path"));

    qso.setARRL_Sect(getADIFValueFromRec(rec, "arrl_sect"));
    qso.setCheck(getADIFValueFromRec(rec, "checkcontest"));
    qso.setClass(getADIFValueFromRec(rec, "class"));

    //qDebug() << Q_FUNC_INFO << ":  - 30";
    qso.setContinent(getADIFValueFromRec(rec, "cont"));
    qso.setContactedOperator(getADIFValueFromRec(rec, "contacted_op"));
    qso.setContestID(getADIFValueFromRec(rec, "contest_id"));

    qso.setCountry(getADIFValueFromRec(rec, "country"));

    qso.setCreditSubmitted(getADIFValueFromRec(rec, "credit_submitted"));
    qso.setCreditGranted(getADIFValueFromRec(rec, "credit_granted"));

    qso.setDistance(getADIFValueFromRec(rec, "distance").toDouble());
    qso.setDarcDok(getADIFValueFromRec(rec, "darc_dok"));


    qso.setEQ_Call(getADIFValueFromRec(rec, "eq_call"));
    qso.setEmail(getADIFValueFromRec(rec, "email"));

    aux = getADIFValueFromRec(rec, "eqsl_qslrdate");
    qso.setEQSLQSLRDate(util->getDateFromSQliteString(aux));

    aux = getADIFValueFromRec(rec, "eqsl_qslsdate");
    qso.setEQSLQSLSDate(util->getDateFromSQliteString(aux));


    qso.setEQSLQSL_RCVD(getADIFValueFromRec(rec, "eqsl_qsl_rcvd"));
    qso.setEQSLQSL_SENT(getADIFValueFromRec(rec, "eqsl_qsl_sent"));

    qso.setFists(getADIFValueFromRec(rec, "fists").toInt());
    qso.setFistsCC(getADIFValueFromRec(rec, "fists_cc").toInt());
    qso.setForceInit(util->QStringToBool(getADIFValueFromRec(rec, "force_init")));

    aux = getADIFValueFromRec(rec, "hrdlog_qso_upload_date");
    qso.setHRDUpdateDate(util->getDateFromSQliteString(aux));
    qso.setHRDLogStatus(getADIFValueFromRec(rec, "hrdlog_qso_upload_status"));

    qso.setMyAntenna(getADIFValueFromRec(rec, "my_antenna"));

    //qDebug() << Q_FUNC_INFO << ": my_antenna-99";
    qso.setMyDXCC(getADIFValueFromRec(rec, "my_dxcc").toInt());


    qso.setMyFists(getADIFValueFromRec(rec, "my_fists").toInt());
    qso.setIOTA(getADIFValueFromRec(rec, "iota"));
    qso.setIotaID(getADIFValueFromRec(rec, "iota_island_id").toInt());

    qso.setMyIOTA(getADIFValueFromRec(rec, "my_iota"));
    qso.setMyIotaID(getADIFValueFromRec(rec, "my_iota_island_id").toInt());
    qso.setK_Index(getADIFValueFromRec(rec, "k_index").toInt());
    qso.setMyITUZone(getADIFValueFromRec(rec, "my_itu_zone").toInt());

    qso.setLatitude(getADIFValueFromRec(rec, "lat"));
    qso.setLongitude(getADIFValueFromRec(rec, "lon"));

    qso.setMyLatitude(getADIFValueFromRec(rec, "my_lat"));
    qso.setMyLongitude(getADIFValueFromRec(rec, "my_lon"));

    aux = getADIFValueFromRec(rec, "lotw_qslrdate");
    qso.setLoTWQSLRDate(util->getDateFromSQliteString(aux));

    aux = getADIFValueFromRec(rec, "lotw_qslsdate");
    qso.setLoTWQSLSDate(util->getDateFromSQliteString(aux));

    qso.setLoTWQSL_RCVD(getADIFValueFromRec(rec, "lotw_qsl_rcvd"));
    qso.setLoTWQSL_SENT(getADIFValueFromRec(rec, "lotw_qsl_sent"));

    aux = getADIFValueFromRec(rec, "clublog_qso_upload_date");
    qso.setClubLogDate(util->getDateFromSQliteString(aux));

    qso.setClubLogStatus(getADIFValueFromRec(rec, "clublog_qso_upload_status"));

    aux = getADIFValueFromRec(rec, "qrzcom_qso_upload_date");
    qso.setQRZCOMDate(util->getDateFromSQliteString(aux));

    qso.setQRZCOMStatus(getADIFValueFromRec(rec, "qrzcom_qso_upload_status"));

    qso.setMaxBursts(getADIFValueFromRec(rec, "max_bursts").toInt());
    qso.setMsShower(getADIFValueFromRec(rec, "ms_shower"));

    qso.setMyCity(getADIFValueFromRec(rec, "my_city"));
    qso.setMyCounty(getADIFValueFromRec(rec, "my_cnty"));
    qso.setMyCountry(getADIFValueFromRec(rec, "my_country"));
    qso.setMyCQZone(getADIFValueFromRec(rec, "my_cq_zone").toInt());
    qso.setMyName(getADIFValueFromRec(rec, "my_name"));

    qso.setName(getADIFValueFromRec(rec, "name"));
    qso.setOperatorCallsign(getADIFValueFromRec(rec, "operator"));

    qso.setOwnerCallsign(getADIFValueFromRec(rec, "owner_callsign"));
    qso.setMyPostalCode(getADIFValueFromRec(rec, "my_postal_code"));
    qso.setMyRig(getADIFValueFromRec(rec, "my_rig"));
    qso.setMySig(getADIFValueFromRec(rec, "my_sig"));


    qso.setMySOTA_REF(getADIFValueFromRec(rec, "my_sota_ref"));
    qso.setMyState(getADIFValueFromRec(rec, "my_state"));
    qso.setMyStreet(getADIFValueFromRec(rec, "my_street"));

    qso.setNotes(getADIFValueFromRec(rec, "notes"));
    qso.setNrBursts(getADIFValueFromRec(rec, "nr_bursts").toInt());
    qso.setNrPings(getADIFValueFromRec(rec, "nr_pings").toInt());
    qso.setPrefix(getADIFValueFromRec(rec, "pfx"));

    qso.setPrecedence(getADIFValueFromRec(rec, "precedence"));
    qso.setPublicKey(getADIFValueFromRec(rec, "public_key"));
    qso.setQSLMsg(getADIFValueFromRec(rec, "qslmsg"));

    aux = getADIFValueFromRec(rec, "qslrdate");
    qso.setQSLRDate(util->getDateFromSQliteString(aux));

    aux = getADIFValueFromRec(rec, "qslsdate");
    qso.setQSLSDate(util->getDateFromSQliteString(aux));
    qso.setQSL_RCVD(getADIFValueFromRec(rec, "qsl_rcvd"));
    qso.setQSLRecVia(getADIFValueFromRec(rec, "qsl_rcvd_via"));

    qso.setQSL_SENT(getADIFValueFromRec(rec, "qsl_sent"));
    qso.setQSLSenVia(getADIFValueFromRec(rec, "qsl_sent_via"));
    qso.setQSLVia(getADIFValueFromRec(rec, "qsl_via"));

    qso.setQSOComplete(util->getADIFQSO_CompleteFromDB(getADIFValueFromRec(rec, "qso_complete")));
    qso.setQSORandom(util->QStringToBool(getADIFValueFromRec(rec, "qso_random")));

    qso.setQTH(getADIFValueFromRec(rec, "qth"));
    qso.setRSTTX(getADIFValueFromRec(rec, "rst_sent"));

    qso.setRSTRX(getADIFValueFromRec(rec, "rst_rcvd"));
    qso.setRegion(getADIFValueFromRec(rec, "region"));
    qso.setRig(getADIFValueFromRec(rec, "rig"));

    qso.setRXPwr(getADIFValueFromRec(rec, "rx_pwr").toDouble());
    qso.setTXPwr(getADIFValueFromRec(rec, "tx_pwr").toDouble());

    qso.setSatMode(getADIFValueFromRec(rec, "sat_mode"));

    qso.setSFI(getADIFValueFromRec(rec, "sfi").toInt());
    qso.setSIG(getADIFValueFromRec(rec, "sig"));

    qso.setSIG_INFO(getADIFValueFromRec(rec, "sig_info"));
    qso.setSilentKey(util->QStringToBool(getADIFValueFromRec(rec, "silent_key")));

    qso.setSkcc(getADIFValueFromRec(rec, "skcc"));
    qso.setSOTA_REF(getADIFValueFromRec(rec, "sota_ref"));

    qso.setState(getADIFValueFromRec(rec, "state"));
    qso.setSwl(util->QStringToBool(getADIFValueFromRec(rec, "swl")));
    qso.setTenTen(getADIFValueFromRec(rec, "ten_ten").toInt());
    qso.setUksmg(getADIFValueFromRec(rec, "uksmg").toInt());

    qso.setVeProv(getADIFValueFromRec(rec, "ve_prov"));
    qso.setMyUsacaCounties(getADIFValueFromRec(rec, "my_usaca_counties"));
    qso.setUsacaCounties(getADIFValueFromRec(rec, "usaca_counties"));

    qso.setVUCCGrids(getADIFValueFromRec(rec, "vucc_grids"));
    qso.setMyVUCCGrids(getADIFValueFromRec(rec, "my_vucc_grids"));

    qso.setWeb(getADIFValueFromRec(rec, "web"));
    if (_logN == -1)
        qso.setLogId(getADIFValueFromRec(rec, "lognumber").toInt());

   return  qso.getADIF();
}

bool DataProxy_SQLite::showInvalidCallMessage(const QString &_call){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - Invalid call detected"));

    QString aux = _call;

    if (aux.length()<1)
    {
        aux = QString(tr("An empty callsign has been detected. Do you want to export this QSO anyway (click on Yes) or remove the field from the exported ADIF record?"));
    }
    else
    {
        aux = QString(tr("An invalid callsign has been detected %1. Do you want to export this callsign anyway (click on Yes) or remove the call from the exported log?").arg(aux));
    }

    msgBox.setText(aux);
    msgBox.setInformativeText(tr("Exporting wrong calls may create problems in the applications you are potentially importing this logfile to. It may, however, be a good callsign that is wrongly identified by KLog as not valid."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
          // Yes was clicked
           return true;
    case QMessageBox::No:
            // No Save was clicked
        return false;
    default:
            // should never be reached
        return false;
    }
}

QList<QSO*> DataProxy_SQLite::getSatDXCCStats(int _log)
{
    //qDebug() << Q_FUNC_INFO << ": log = " << QString::number(_log);

    QList<QSO*> _qsos;
    QString stringQuery;
    if (doesThisLogExist(_log))
    {
        //qDebug() << Q_FUNC_INFO << ": log exists " ;

        stringQuery = QString("SELECT call, qso_date, band.name, mode.name, entity.name, log.dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log, entity, band, mode where log.dxcc <>''  AND sat_name <>'' AND log.dxcc=entity.dxcc AND log.bandid=band.id AND log.modeid=mode.id AND lognumber='%1' ORDER BY entity.name").arg(_log);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": log does not exist " ;
        stringQuery = QString("SELECT call, qso_date, band.name, mode.submode, entity.name, log.dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log, entity, band, mode where log.dxcc <>''  AND sat_name <>'' AND log.dxcc=entity.dxcc AND log.bandid=band.id AND log.modeid=mode.id ORDER BY entity.name");
    }

    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << ":  Query NOK";
        //qDebug() << Q_FUNC_INFO << ":  " << query.lastError().databaseText();
        //qDebug() << Q_FUNC_INFO << ":  " << query.lastQuery();
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return _qsos;
    }
    else
    {
        while(query.next())
        {
            if (query.isValid())
            {
                int nameCol;
                QSO *_qso = new QSO;
                _qso->clear();

                QSqlRecord rec = query.record();
                nameCol = rec.indexOf("call");
                _qso->setCall((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qso_date");

                _qso->setDateTimeOn (util->getDateTimeFromSQLiteString((query.value(nameCol)).toString()));

                nameCol = rec.indexOf("bandid");
                //qDebug() << Q_FUNC_INFO << " - bandid" << QString::number((query.value(nameCol)).toInt());
                _qso->setBand(query.value(nameCol).toString());

                //nameCol = rec.indexOf("modeid");
                //qDebug() << Q_FUNC_INFO << ": modeid" << QString::number((query.value(nameCol)).toInt());
                _qso->setMode(getNameFromSubMode (query.value(3).toString()));
                _qso->setSubmode(query.value(3).toString());
                nameCol = rec.indexOf("sat_name");
                _qso->setSatName((query.value(nameCol)).toString());

                nameCol = rec.indexOf("log.dxcc");
                _qso->setDXCC((query.value(nameCol)).toInt());

                nameCol = rec.indexOf("lotw_qsl_rcvd");
                _qso->setLoTWQSL_RCVD((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qsl_rcvd");
                _qso->setQSL_RCVD((query.value(nameCol)).toString());

                _qsos.append(_qso);
            }
            else
            {
                _qsos.clear();
                query.finish();
                return _qsos;
            }
        }
    }
    //qDebug() << Q_FUNC_INFO << "- END";
    return _qsos;
}

QList<QSO *> DataProxy_SQLite::getGridStats(int _log)
{
    //qDebug() << Q_FUNC_INFO <<  ": log = " << QString::number(_log);
    QList<QSO*> _qsos;
    QString stringQuery;
    if (doesThisLogExist(_log))
    {
        //qDebug() << Q_FUNC_INFO <<  ":: log exists " ;
        stringQuery = QString("SELECT call, substr(gridsquare, 1, 4), bandid, modeid, lotw_qsl_rcvd, qsl_rcvd from log where gridsquare <>'' AND lognumber='%1'").arg(_log);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO <<  ": log does not exist " ;
        stringQuery = QString("SELECT call, substr(gridsquare, 1, 4), bandid, modeid, lotw_qsl_rcvd, qsl_rcvd from log where gridsquare <>''");
    }

    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO <<  ":  Query NOK";
        //qDebug() << Q_FUNC_INFO <<  ":  " << query.lastError().databaseText();
        //qDebug() << Q_FUNC_INFO <<  ":  " << query.lastQuery();
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return _qsos;
    }
    else
    {
        while(query.next())
        {
            if (query.isValid())
            {// call, substr(gridsquare, 1, 4), bandid, modeid, lotw_qsl_rcvd, qsl_rcvd
                int nameCol;
                QSO *_qso = new QSO;
                _qso->clear();

                QSqlRecord rec = query.record();
                nameCol = rec.indexOf("call");
                _qso->setCall((query.value(nameCol)).toString());

                nameCol = rec.indexOf("substr(gridsquare, 1, 4)");
                _qso->setGridSquare((query.value(nameCol)).toString());

                nameCol = rec.indexOf("bandid");
                //qDebug() << Q_FUNC_INFO <<  ": bandid" << QString::number((query.value(nameCol)).toInt());
                _qso->setBand(getNameFromBandId((query.value(nameCol)).toInt()));

                nameCol = rec.indexOf("modeid");
                //qDebug() << Q_FUNC_INFO <<  ": modeid" << QString::number((query.value(nameCol)).toInt());
                _qso->setMode(getNameFromModeId((query.value(nameCol)).toInt()));
                _qso->setSubmode (getSubModeFromId ((query.value(nameCol)).toInt()));

                nameCol = rec.indexOf("lotw_qsl_rcvd");
                _qso->setLoTWQSL_RCVD((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qsl_rcvd");
                _qso->setQSL_RCVD((query.value(nameCol)).toString());

                _qsos.append(_qso);
                //qDebug() << Q_FUNC_INFO <<  ": call: " << _call;
                //qDebug() << Q_FUNC_INFO <<  ": band: " << _band;
                //qDebug() << Q_FUNC_INFO <<  ": mode: " << _mode;
            }
            else
            {
                _qsos.clear();
                query.finish();
                return _qsos;
            }
        }
        //qDebug() << Q_FUNC_INFO <<  ":    Query OK";
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    return _qsos;
}

QList<QSO *> DataProxy_SQLite::getSatGridStats(int _log)
{
    //qDebug() << Q_FUNC_INFO << " - log = " << QString::number(_log);

    QList<QSO*> _qsos;
    QString stringQuery;
    if (doesThisLogExist(_log))
    {
        //qDebug() << Q_FUNC_INFO << " - log exists " ;
        stringQuery = QString("SELECT call, qso_date, bandid, modeid, substr(gridsquare, 1, 4), lotw_qsl_rcvd, qsl_rcvd, sat_name from log where gridsquare <>''  AND sat_name <>'' AND lognumber='%1'").arg(_log);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - log does not exist " ;
        stringQuery = QString("SELECT call, qso_date, bandid, modeid, substr(gridsquare, 1, 4), lotw_qsl_rcvd, qsl_rcvd, sat_name from log where gridsquare <>''  AND sat_name <>''");
    }


    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " -  Query NOK";
        //qDebug() << Q_FUNC_INFO << " -  " << query.lastError().databaseText();
        //qDebug() << Q_FUNC_INFO << " -  " << query.lastQuery();
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return _qsos;
    }
    else
    {
        while(query.next())
        {
            if (query.isValid())
            {
                int nameCol;
                QSO *_qso = new QSO;
                _qso->clear();

                QSqlRecord rec = query.record();
                nameCol = rec.indexOf("call");
                _qso->setCall((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qso_date");
                _qso->setDateTimeOn (util->getDateTimeFromSQLiteString((query.value(nameCol)).toString()));

                nameCol = rec.indexOf("bandid");
                //qDebug() << Q_FUNC_INFO << " - bandid" << QString::number((query.value(nameCol)).toInt());
                _qso->setBand(getNameFromBandId((query.value(nameCol)).toInt()));

                nameCol = rec.indexOf("modeid");
                //qDebug() << Q_FUNC_INFO << " - modeid" << QString::number((query.value(nameCol)).toInt());
                _qso->setMode(getNameFromModeId((query.value(nameCol)).toInt()));
                _qso->setSubmode(getSubModeFromId((query.value(nameCol)).toInt()));

                nameCol = rec.indexOf("sat_name");
                _qso->setSatName((query.value(nameCol)).toString());

                nameCol = rec.indexOf("substr(gridsquare, 1, 4)");
                _qso->setGridSquare((query.value(nameCol)).toString());

                nameCol = rec.indexOf("lotw_qsl_rcvd");
                _qso->setLoTWQSL_RCVD((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qsl_rcvd");
                _qso->setQSL_RCVD((query.value(nameCol)).toString());

                _qsos.append(_qso);
                //qDebug() << Q_FUNC_INFO << " - call: " << _call;
                //qDebug() << Q_FUNC_INFO << " - band: " << _band;
                //qDebug() << Q_FUNC_INFO << " - mode: " << _mode;
            }
            else
            {
                _qsos.clear();
                query.finish();
                return _qsos;
            }
        }
        //qDebug() << Q_FUNC_INFO <<  ":    Query OK";
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    return _qsos;
}

int DataProxy_SQLite::getFieldInBand(ValidFieldsForStats _field, const QString &_band, bool confirmedOnly, QString _mode, int _log)
{
        //qDebug() << Q_FUNC_INFO << ": " << _band << "/" << _mode << "/" << QString::number(_log) ;

    if ((!doesThisLogExist(_log)) && !(_log == -1))
    {
        //qDebug() << Q_FUNC_INFO << ": Exit no log";
        return 0;
    }
    int bandId = getIdFromBandName (_band);
    QString bandString = QString();

    if (!(_band.toUpper () == "ALL"))
    {
        if (bandId<1)
        {
            //qDebug() << Q_FUNC_INFO << ": Exit band";
            return 0;
        }
        bandString = QString(" AND bandid='%1'").arg(bandId);
    }

    QString field = QString();
    QString specialField = QString();

    switch (_field)
    {
        case DXCC:
            field = "dxcc";
            specialField = field + " <>'' AND dxcc<1000 AND dxcc>0";

        break;
        case GridSquare:
            field = "gridsquare";
            specialField = field +  " <>''";

        break;
    }

    QString stringQuery;
    QSqlQuery query;
    QString modeString = QString();


   int modeId = getIdFromModeName(_mode);
   if (_mode.toUpper() == "ALL")
   {
       //qDebug() << Q_FUNC_INFO << ": ALL Modes" ;
   }
   else if (util->isValidModeId(modeId))
   {
       //qDebug() << Q_FUNC_INFO << ": Valid Mode" ;
       modeString = QString(" AND modeid='%1' ").arg(modeId);
   }
   else
   {
       //qDebug() << Q_FUNC_INFO << ": Mode not valid!" ;
       return 0;
   }

    QString logString = QString();
    if (!(_log == -1))
    {
        logString = QString(" AND lognumber='%1'").arg(_log);
    }
    QString confirmedString = QString();
    if (confirmedOnly)
    {
        confirmedString = QString(" AND (lotw_qsl_rcvd='Y' OR qsl_rcvd='Y')");
    }

    stringQuery = QString("SELECT COUNT (DISTINCT %1) from log WHERE %2 %3 %4 %5 %6").arg(field).arg(specialField).arg(modeString).arg(bandString).arg(confirmedString).arg(logString);

    //qDebug() << Q_FUNC_INFO << " :  Query: " << stringQuery;

    bool sqlOK = query.exec(stringQuery);

    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " :  Query NOK";
        //qDebug() << Q_FUNC_INFO << " :  " << query.lastError().databaseText();
        //qDebug() << Q_FUNC_INFO << " :  " << query.lastQuery();
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return 0;
    }
    else
    {
        query.next();
        if (query.isValid())
        {
            //qDebug() << Q_FUNC_INFO << " : " << QString::number((query.value(0)).toInt());
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
    }
    query.finish();
    return 0;
}

void DataProxy_SQLite::slotCaptureDebugLogs(const QString &_func, const QString &_msg, DebugLogLevel _l)
{
    logEvent(_func, _msg, _l);
}
