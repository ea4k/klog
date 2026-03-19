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
#include "adif.h"
#include "locator.h"

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
    //util = new Utilities(Q_FUNC_INFO);
        //Utilities util(Q_FUNC_INFO);
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
    //qso = new QSO;
    //qDebug() << Q_FUNC_INFO << " - 53";
    createHashes();
    //loadBandLimits();
    searching = false;
    executionN = 0;
    connect(db, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    //qDebug() << Q_FUNC_INFO << " - END";
    logEvent (Q_FUNC_INFO, "END", Debug);
}

DataProxy_SQLite::~DataProxy_SQLite()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //delete(util);
    //delete(qso);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool DataProxy_SQLite::createHashes()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    loadBandDataCache();
    loadModeDataCache();
    if (!m_cache.isBandListOK())
        return false;
    if (!m_cache.isModeListOK())
        return false;

    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

void DataProxy_SQLite::loadBandDataCache()
{
    QSqlQuery query("SELECT id, name, lower, upper FROM band");
    Frequency fmin;
    Frequency fmax;

    while (query.next()) {
        fmin.fromDouble(query.value(2).toDouble(), MHz);
        fmax.fromDouble(query.value(3).toDouble(), MHz);
        m_cache.addBand(
            query.value(0).toInt(),    // The existing BandID
            query.value(1).toString(), // Name
            fmin, // Min
            fmax  // Max
        );
    }
}

void DataProxy_SQLite::loadModeDataCache()
{
    QSqlQuery query("SELECT id, submode, name, cabrillo, deprecated FROM mode");
    while (query.next()) {
        m_cache.addMode(
            query.value(0).toInt(),     // id
            query.value(1).toString(),  // submode
            query.value(2).toString(),  // mode
            query.value(3).toString(),  // cabrillo
            query.value(4).toBool()     // deprecated
        );
    }
}

void DataProxy_SQLite::setPKGVersion(const QString &_pkgVersion)
{
    pkgVersion = _pkgVersion;
}

QString DataProxy_SQLite::getPKGVersion()
{
    return pkgVersion;
}

int DataProxy_SQLite::getHowManyQSOPerPropMode(const QString &_p, const int _logn)
{
    logEvent(Q_FUNC_INFO, "Start", Devel);
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

    logEvent(Q_FUNC_INFO, "Start-End", Debug);
    m_queriesPrepared = false;   // Invalidate pre-prepared
    m_insertQuery = QSqlQuery(); // Free the queries
    m_updateQuery = QSqlQuery();
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
    if (_modeName.length() < 2) return -4;
        return m_cache.getModeIdFromSubmode(_modeName);
    //return db->getModeIdFromSubMode(_modeName);
}

bool DataProxy_SQLite::isValidMode(const QString& _modeName)
{
    return ( getIdFromModeName(_modeName) >=0 );
}

bool DataProxy_SQLite::isValidBand(const QString& _bandName)
{
    return ( getIdFromBandName(_bandName) >=0 );
}

bool DataProxy_SQLite::isModeDeprecated (const QString &_sm)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_sm.length() < 2) return false;
    return m_cache.isModeDeprecated(_sm);
}

int DataProxy_SQLite::getIdFromBandName(const QString& _bandName)
{
    //logEvent (Q_FUNC_INFO, "Start", Debug);
    return m_cache.getBandFromName(_bandName).id;
}

QString DataProxy_SQLite::getNameFromBandId (const int _id)
{ //TODO: Use the hash
    //logEvent(Q_FUNC_INFO, "Start", Devel);
    return m_cache.getBandFromId(_id).name;
}

QString DataProxy_SQLite::getNameFromModeId (const int _id)
{ //TODO: Use the hash
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return m_cache.getModeFromId(_id).mode;
    //return db->getModeNameFromNumber(_id);
}

QString DataProxy_SQLite::getSubModeFromId (const int _id)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    return m_cache.getModeFromId(_id).submode;
}

QString DataProxy_SQLite::getNameFromSubMode (const QString &_sm)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    return m_cache.getModeNameFromSubmode(_sm);
}

//Frequency DataProxy_SQLite::getFreqFromBandId(const int _id)
//{//getLowLimitBandFromBandId
//    logEvent (Q_FUNC_INFO, "Start-End", Debug);

//    return db->getFreqFromBandId(_id);
//}

int DataProxy_SQLite::getBandIdFromFreq(const Frequency _n)
{
    // Replaced the heavy SQL query with a fast in-memory lookup.

    logEvent (Q_FUNC_INFO, "Start", Debug);
    return m_cache.getBandFromFreq (_n).id;
}



QString DataProxy_SQLite::getBandNameFromFreq(const Frequency _n)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
        return m_cache.getBandFromFreq(_n).name;
}

Frequency DataProxy_SQLite::getLowLimitBandFromBandName(const QString &_sm)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    return m_cache.getBandFromName(_sm).minFreq;
}

Frequency DataProxy_SQLite::getLowLimitBandFromBandId(const int _sm)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    return m_cache.getBandFromId(_sm).minFreq;
}

Frequency DataProxy_SQLite::getUpperLimitBandFromBandName(const QString &_sm)
{
    //qDebug() << Q_FUNC_INFO << ": " << _sm;
        return m_cache.getBandFromName(_sm).maxFreq;
}


/*
bool DataProxy_SQLite::loadBandLimits()
{
    logEvent(Q_FUNC_INFO, "Start", Devel);
    m_bandLimits.clear();

    // Query ID, lower limit, and upper limit from the 'band' table
    QString queryString = "SELECT id, lower, upper FROM band";
    QSqlQuery query;
    //query.setForwardOnly(true); // Optimization for reading data once

    if (!query.exec(queryString))
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        logEvent(Q_FUNC_INFO, "END-FAIL-1 - Query failed", Debug);
        return false;
    }

    while (query.next())
    {
        if (query.isValid())
        {
            BandLimits limits;
            // Retrieve values using native C++ types for best speed and precision
            limits.id = query.value(0).toInt();
            limits.lower = query.value(1).toDouble();
            limits.upper = query.value(2).toDouble();

            // Store only valid bands (ID > 0 and a valid range)
            if (limits.id > 0 && limits.lower < limits.upper) {
                 m_bandLimits.append(limits);
            }
        }
    }

    query.finish();
    logEvent(Q_FUNC_INFO, QString("END - Loaded %1 bands").arg(m_bandLimits.count()), Debug);
    return true;
}
*/

bool DataProxy_SQLite::isThisFreqInBand(const QString &_band, const Frequency _fr)
{
    //qDebug() << Q_FUNC_INFO << " - Band: " << _band;
    //qDebug() << Q_FUNC_INFO << " - BandC: " << m_cache.getBandFromFreq(_fr).name;
    return (m_cache.getBandFromFreq(_fr).name == _band );
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
    //query.setForwardOnly(true);
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
    static Adif adif(Q_FUNC_INFO);
    QStringList result;
    for (const AdifMode &g : adif.getModeList())
        result << g.mode;
    return result;
}

QStringList DataProxy_SQLite::getSubModes()
{
    static Adif adif(Q_FUNC_INFO);
    QStringList result;
    for (const AdifMode &g : adif.getModeList())
        result << g.submodes;
    return result;
}

QStringList DataProxy_SQLite::getModesAndSubmodes()
{
    static Adif adif(Q_FUNC_INFO);
    QStringList result;
    for (const AdifMode &g : adif.getModeList())
        result << g.mode << g.submodes;
    return result;
}

QStringList DataProxy_SQLite::sortBandNamesBottonUp(const QStringList _qs)
{//TODO: Check if is working well or can be optimized
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
                map.insert(getLowLimitBandFromBandName(_qs.at(j)).toDouble(), _qs.at(j));
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
    //query.setForwardOnly(true);

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
                map.insert(getLowLimitBandFromBandId(_qs.at(j).toInt()).toDouble(), _qs.at(j));
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
    //query.setForwardOnly(true);

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
    //query.setForwardOnly(true);

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

    QSqlQuery query;
    //query.setForwardOnly(true);
    query.exec(queryString);

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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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

int DataProxy_SQLite::getLastInsertedQSO()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QString stringQuery = QString("SELECT last_insert_rowid()");

    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    int id = -1;

    if (sqlOK)
    {
        query.next();
        id = (query.value(0)).toInt();
    }
    else
    {
        // queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return id;
    //qDebug() << Q_FUNC_INFO << " - END";
}


QDate DataProxy_SQLite::getFirstQSODateFromCall (const QString &_call)
{
    //qDebug() << Q_FUNC_INFO << ": " << _call;

    QSqlQuery query;
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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

                        // queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
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
    //query.setForwardOnly(true);
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
        //Utilities util(Q_FUNC_INFO);
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
        //Utilities util(Q_FUNC_INFO);
        queryString = QString("UPDATE log SET clublog_qso_upload_status = '%1', clublog_qso_upload_date = '%2' WHERE id = '%3'")
                .arg(_st, util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
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
        //Utilities util(Q_FUNC_INFO);
        if (!util->isValidQSL_Rcvd(_st))
    {
        return false;
    }
    if (!_updateDate.isValid())
    {
        return false;
    }
        queryString = QString("UPDATE log SET lotw_qsl_rcvd = '%1', lotw_qslrdate = '%2' WHERE id = '%3'")
                .arg(_st, util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);
    int nameCol = -1;
    QStringList dataC = QStringList();
    // qString aux1 = QString();
    // qString aux2 = QString();
    // qString call = QString();

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
                                //Utilities util(Q_FUNC_INFO);
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
    //qDebug() << Q_FUNC_INFO << " - band: " << _band;
    //qDebug() << Q_FUNC_INFO << " - mode: " << _mode;
    //qDebug() << Q_FUNC_INFO << " - prop: " << _prop;
    //qDebug() << Q_FUNC_INFO << " -  sat: " << _sat;

    QStringList grids;
    QStringList where;
    where << "gridsquare IS NOT NULL" << "gridsquare <> ''";

    // Resolve band/mode ids (only add predicates when they restrict results)
    const int bandId = getIdFromBandName(_band);
    if (bandId > 0) {
        where << "bandid = :bandid";
    }

    const int modeId = getIdFromModeName(_mode);
    if (modeId > 0) {
        where << "modeid = :modeid";
    }
    // Propagation + satellite handling
    const bool propValid = isValidPropMode(_prop);
    const bool isSat = (propValid && _prop == "SAT");
    int satDbId = -1;
    if (isSat) {
        satDbId = getDBSatId(_sat); // > 0 means a known sat was chosen
    }

    if (propValid) {
        //qDebug() << Q_FUNC_INFO << " - PROP VALID";
        where << "prop_mode = :prop";
        if (isSat)
        {
            where << "prop_mode = 'SAT'";
            if (satDbId > 0) {
                where << "sat_name = :satname";
            } else {
                // Keep SAT QSOs except the sentinel 'x'
                //where << "COALESCE(sat_name,'') <> 'x'";
            }
        } else {
            // Not SAT propagation: exclude SAT-tagged QSOs
            //where << "COALESCE(sat_name,'') = ''";
        }
    } else {
        // No specific prop filter: exclude sentinel 'x' and any SAT-tagged QSOs
        //qDebug() << Q_FUNC_INFO << " - PROP NOT VALID";
        //where << "COALESCE(prop_mode,'') <> 'x'";
        //where << "COALESCE(sat_name,'') = ''";
    }

    // Confirmation filtering
    if (_confirmed) {
        where << "((qsl_rcvd = 'Y') OR (lotw_qsl_rcvd = 'Y'))";
    } else {
        // Keep anything that is not the sentinel 'x' (include NULL)
        where << "COALESCE(qsl_rcvd,'') <> 'x'";
    }

    QString sql = "SELECT DISTINCT gridsquare FROM log";
    if (!where.isEmpty()) {
        sql += " WHERE " + where.join(" AND ");
    }
    // No ORDER BY here; DISTINCT+ORDER BY can be expensive. We sort in-memory below.

    //qDebug() << Q_FUNC_INFO << ": " << sql;
    QSqlQuery query;
    //query.setForwardOnly(true);
    query.prepare(sql);

    if (bandId > 0) query.bindValue(":bandid", bandId);
    if (modeId > 0) query.bindValue(":modeid", modeId);
    if (propValid)  query.bindValue(":prop", _prop);
    if (isSat && satDbId > 0) query.bindValue(":satname", _sat);

    if (!query.exec()) {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(),
                        query.lastError().text(), query.executedQuery());
        return QStringList();
    }

    while (query.next()) {
        if (!query.isValid()) continue;
        const QString grid = query.value(0).toString().trimmed();
        if (!grid.isEmpty()) {
            grids.append(grid);
        }
    }
    query.finish();

    grids.sort();          // natural sort for caller expectations
    grids.removeDuplicates(); // defensive; DISTINCT already dedupes

    return grids;
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
        //Utilities util(Q_FUNC_INFO);
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
        //Utilities util(Q_FUNC_INFO);
    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << Q_FUNC_INFO << " - updating QSO: " << QString::number(_qsos.at(i));
                 queryString = QString("UPDATE log SET qrzcom_qso_upload_status = 'Y', qrzcom_qso_upload_date = '%1' WHERE id='%2'")
                         .arg(util->getDateSQLiteStringFromDate(QDate::currentDate()), QString::number(_qsos.at(i)));
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
        //Utilities util(Q_FUNC_INFO);
    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << Q_FUNC_INFO << " - updating QSO: " << QString::number(_qsos.at(i));

                 queryString = QString("UPDATE log SET clublog_qso_upload_status = 'Y', clublog_qso_upload_date = '%1' WHERE id='%2'")
                         .arg(util->getDateSQLiteStringFromDate(QDate::currentDate()), QString::number(_qsos.at(i)));
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
        //Utilities util(Q_FUNC_INFO);
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
        //Utilities util(Q_FUNC_INFO);
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
        //Utilities util(Q_FUNC_INFO);
     for (int i = 0; i< _qsos.count(); i++)
     {
           //qDebug() << Q_FUNC_INFO << " -: updating QSO: " << QString::number(_qsos.at(i));


          queryString = QString("UPDATE log SET eqsl_qsl_sent = 'Y', eqsl_qslsdate = '%1' WHERE id='%2'")
                  .arg(util->getDateSQLiteStringFromDate(QDate::currentDate()), QString::number(_qsos.at(i)));
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
    //return (qso.toDB()>0);
    return (addQSO(qso)>0);
}

int DataProxy_SQLite::addQSO(QSO &_qso)
{
    if (!_qso.isComplete())
        return -1;
    _qso.clearQSLDateIfNeeded();

    prepareStaticQueries();
    if (!m_queriesPrepared)
    {
        emit queryError(Q_FUNC_INFO,
                        "Queries could not be prepared",
                        "prepareStaticQueries() failed before addQSO",
                        QString());
        return -2;
    }

    const bool isInsert = (_qso.getQSOid() <= 0);
    QSqlQuery &query = isInsert ? m_insertQuery : m_updateQuery;

    bindQSOValues(query, _qso);
    if (!isInsert)
        query.bindValue(":id", _qso.getQSOid());

    if (query.exec())
    {
        if (isInsert)
        {
            const int newId = query.lastInsertId().toInt();
            return (newId > 0) ? newId : getLastInsertedQSO();
        }
        return _qso.getQSOid();
    }

    // Duplicate detection: UNIQUE constraint (code 19) is not a real error,
    // the duplicate cache should have caught it first, but handle it silently here as fallback.
    const QSqlError sqlError = query.lastError();
    if (sqlError.nativeErrorCode() == QLatin1String("19") ||
        sqlError.databaseText().contains(QLatin1String("UNIQUE constraint failed"), Qt::CaseInsensitive))
    {
        return -2;  // Silent duplicate — no queryError emitted
    }

    emit queryError(Q_FUNC_INFO, sqlError.databaseText(),
                    sqlError.text(), query.lastQuery());
    return -2;
}

QString DataProxy_SQLite::getAddQueryString()
{
    return QString( "INSERT INTO log ("
                   "qso_date, call, rst_sent, rst_rcvd, bandid, modeid, cqz, ituz, dxcc, address, age, altitude, cnty, comment, a_index, ant_az, ant_el, "
                   "ant_path, arrl_sect, award_submitted, award_granted, band_rx, checkcontest, class, clublog_qso_upload_date, "
                   "clublog_qso_upload_status, cont, contacted_op, contest_id, country, credit_submitted, credit_granted, darc_dok, "
                   "distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, fists, fists_cc, "
                   "force_init, freq, freq_rx, gridsquare, gridsquare_ext, hrdlog_qso_upload_date, hrdlog_qso_upload_status, "
                   "hamlogeu_qso_upload_date, hamlogeu_qso_upload_status, hamqth_qso_upload_date, hamqth_qso_upload_status, "
                   "iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, "
                   "my_antenna, my_altitude, my_arrl_sect, my_city, my_cnty, my_country, my_cq_zone, my_dxcc, my_fists, my_gridsquare, my_gridsquare_ext, my_iota, my_iota_island_id, "
                   "my_itu_zone, my_lat, "
                   "my_lon, my_name, my_pota_ref, my_postal_code, my_rig, my_sig, my_sig_info, my_sota_ref, my_state, my_street, "
                   "my_usaca_counties, my_wwff_ref, my_vucc_grids, name, "
                   "notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, pota_ref, precedence, prop_mode, public_key, qrzcom_qso_upload_date, "
                   "qrzcom_qso_upload_status, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, "
                   "qth, region, rig, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, silent_key, skcc, sota_ref, srx_string, srx, stx_string, stx, state, "
                   "station_callsign, submode, swl, uksmg, usaca_counties, ve_prov, wwff_ref, vucc_grids, ten_ten, tx_pwr, web, qso_date_off, marked, lognumber) "
                   "VALUES ("
                   ":qso_date, :call, :rst_sent, :rst_rcvd, :bandid, :modeid, :cqz, :ituz, :dxcc, :address, :age, :altitude, :cnty, :comment, :a_index, :ant_az, :ant_el, "
                   ":ant_path, :arrl_sect, :award_submitted, :award_granted, :band_rx, :checkcontest, :class, :clublog_qso_upload_date, :clublog_qso_upload_status, :cont, "
                   ":contacted_op, :contest_id, :country, :credit_submitted, :credit_granted, :darc_dok, :distance, :email, :eq_call, :eqsl_qslrdate, :eqsl_qslsdate, "
                   ":eqsl_qsl_rcvd, :eqsl_qsl_sent, :fists, :fists_cc, :force_init, :freq_tx, :freq_rx, :gridsquare, :gridsquare_ext, :hrdlog_qso_upload_date, "
                   ":hrdlog_qso_upload_status, :hamlogeu_qso_upload_date, :hamlogeu_qso_upload_status, :hamqth_qso_upload_date, :hamqth_qso_upload_status, "
                   ":iota, :iota_island_id, :k_index, :lat, :lon, :lotw_qslrdate, :lotw_qslsdate, :lotw_qsl_rcvd, :lotw_qsl_sent, :max_bursts, :ms_shower, "
                   ":my_antenna, :my_altitude, :my_arrl_sect, :my_city, :my_cnty, :my_country, :my_cq_zone, :my_dxcc, :my_fists, :my_gridsquare, :my_gridsquare_ext, :my_iota, :my_iota_island_id, :my_itu_zone, :my_lat, "
                   ":my_lon, :my_name, :my_pota_ref, :my_postal_code, :my_rig, :my_sig, :my_sig_info, :my_sota_ref, :my_state, :my_street, :my_usaca_counties, :my_wwff_ref, :my_vucc_grids, :name, "
                   ":notes, :nr_bursts, :nr_pings, :operator, :owner_callsign, :pfx, :pota_ref, :precedence, :prop_mode, :public_key, :qrzcom_qso_upload_date, "
                   ":qrzcom_qso_upload_status, :qslmsg, :qslrdate, :qslsdate, :qsl_rcvd, :qsl_sent, :qsl_rcvd_via, :qsl_sent_via, :qsl_via, :qso_complete, :qso_random, "
                   ":qth, :region, :rig, :rx_pwr, :sat_mode, :sat_name, :sfi, :sig, :sig_info, :silent_key, :skcc, :sota_ref, :srx_string, :srx, :stx_string, :stx, :state, "
                   ":station_callsign, :submode, :swl, :uksmg, :usaca_counties, :ve_prov, :wwff_ref, :vucc_grids, :ten_ten, :tx_pwr, :web, :qso_date_off, "
                   ":marked, :lognumber)" );
}

QString DataProxy_SQLite::getModifyQueryString()
{
    return QString("UPDATE log SET call = :call, qso_date = :qso_date, rst_sent = :rst_sent, rst_rcvd = :rst_rcvd, "
                   "bandid = :bandid, modeid = :modeid, cqz = :cqz, ituz = :ituz, dxcc = :dxcc, address = :address, "
                   "age = :age, altitude = :altitude, cnty = :cnty, comment = :comment, a_index = :a_index, ant_az = :ant_az, ant_el = :ant_el, "
                   "ant_path = :ant_path, arrl_sect = :arrl_sect, award_submitted = :award_submitted, "
                   "award_granted = :award_granted, band_rx = :band_rx, checkcontest = :checkcontest, class = :class, "
                   "clublog_qso_upload_date = :clublog_qso_upload_date, clublog_qso_upload_status = :clublog_qso_upload_status, "
                   "cont = :cont, contacted_op = :contacted_op, contest_id = :contest_id, country = :country, "
                   "credit_submitted = :credit_submitted, credit_granted = :credit_granted, darc_dok = :darc_dok, "
                   "distance = :distance, email = :email, eq_call = :eq_call, eqsl_qslrdate = :eqsl_qslrdate, "
                   "eqsl_qslsdate = :eqsl_qslsdate, eqsl_qsl_rcvd = :eqsl_qsl_rcvd, eqsl_qsl_sent = :eqsl_qsl_sent, "
                   "fists = :fists, fists_cc = :fists_cc, force_init = :force_init, freq = :freq_tx, freq_rx = :freq_rx, "
                   "gridsquare = :gridsquare, gridsquare_ext = :gridsquare_ext, "
                   "hrdlog_qso_upload_date = :hrdlog_qso_upload_date, hrdlog_qso_upload_status = :hrdlog_qso_upload_status, "
                   "hamlogeu_qso_upload_date = :hamlogeu_qso_upload_date, hamlogeu_qso_upload_status = :hamlogeu_qso_upload_status, "
                   "hamqth_qso_upload_date = :hamqth_qso_upload_date, hamqth_qso_upload_status = :hamqth_qso_upload_status, "
                   "iota = :iota, iota_island_id = :iota_island_id, "
                   "k_index = :k_index, lat = :lat, lon = :lon, lotw_qslrdate = :lotw_qslrdate, lotw_qslsdate = :lotw_qslsdate, "
                   "lotw_qsl_rcvd = :lotw_qsl_rcvd, lotw_qsl_sent = :lotw_qsl_sent, max_bursts = :max_bursts, "
                   "ms_shower = :ms_shower, my_antenna = :my_antenna, my_altitude = :my_altitude, my_arrl_sect = :my_arrl_sect, my_city = :my_city, my_cnty = :my_cnty, "
                   "my_country = :my_country, my_cq_zone = :my_cq_zone, my_dxcc = :my_dxcc, my_fists = :my_fists, "
                   "my_gridsquare = :my_gridsquare, my_gridsquare_ext = :my_gridsquare_ext, my_iota = :my_iota, my_iota_island_id = :my_iota_island_id, "
                   "my_itu_zone = :my_itu_zone, my_lat = :my_lat, my_lon = :my_lon, my_name = :my_name, "
                   "my_pota_ref = :my_pota_ref, my_postal_code = :my_postal_code, my_rig = :my_rig, my_sig = :my_sig, my_sig_info = :my_sig_info, "
                   "my_sota_ref = :my_sota_ref, my_state = :my_state, my_street = :my_street, "
                   "my_usaca_counties = :my_usaca_counties, my_wwff_ref = :my_wwff_ref, my_vucc_grids = :my_vucc_grids, name = :name, notes = :notes, "
                   "nr_bursts = :nr_bursts, nr_pings = :nr_pings, operator = :operator, owner_callsign = :owner_callsign, "
                   "pfx = :pfx, pota_ref = :pota_ref, precedence = :precedence, prop_mode = :prop_mode, "
                   "public_key = :public_key, qrzcom_qso_upload_date = :qrzcom_qso_upload_date, "
                   "qrzcom_qso_upload_status = :qrzcom_qso_upload_status, qslmsg = :qslmsg, qslrdate = :qslrdate, "
                   "qslsdate = :qslsdate, qsl_rcvd = :qsl_rcvd, qsl_sent = :qsl_sent, qsl_rcvd_via = :qsl_rcvd_via, "
                   "qsl_sent_via = :qsl_sent_via, qsl_via = :qsl_via, qso_complete = :qso_complete, qso_random = :qso_random, "
                   "qth = :qth, region = :region, rig = :rig, rx_pwr = :rx_pwr, sat_mode = :sat_mode, sat_name = :sat_name, "
                   "sfi = :sfi, sig = :sig, sig_info = :sig_info, silent_key = :silent_key, skcc = :skcc, "
                   "sota_ref = :sota_ref, srx_string = :srx_string, srx = :srx, stx_string = :stx_string, stx = :stx, "
                   "state = :state, station_callsign = :station_callsign, submode = :submode, swl = :swl, uksmg = :uksmg, "
                   "usaca_counties = :usaca_counties, ve_prov = :ve_prov, wwff_ref = :wwff_ref, vucc_grids = :vucc_grids, ten_ten = :ten_ten, "
                   "tx_pwr = :tx_pwr, web = :web, qso_date_off = :qso_date_off, marked = :marked, lognumber = :lognumber "
                   "WHERE id = :id");
}

QSqlQuery DataProxy_SQLite::getPreparedQuery(const QString &_s, const QSO &_qso)
{
    QSet<QString> qsl_rcvd_dates_set;
    qsl_rcvd_dates_set.insert("Y");
    qsl_rcvd_dates_set.insert("I");
    qsl_rcvd_dates_set.insert("V");

    QSet<QString> qsl_sent_dates_set;
    qsl_sent_dates_set.insert("Y");
    qsl_sent_dates_set.insert("Q");
    qsl_sent_dates_set.insert("I");

    QSqlQuery query;

    //qDebug() << Q_FUNC_INFO << " - Start ";
    //qDebug() << Q_FUNC_INFO << " - queryString: " << _s;

    query.clear ();
    if (!query.prepare (_s))
    {
        //qDebug() << Q_FUNC_INFO << " - Query not prepared-3285";
        query.clear ();
        return query;
    }
    //qDebug() << Q_FUNC_INFO << " - Starting to bind values...";
    Utilities util(Q_FUNC_INFO);
   //qDebug() << Q_FUNC_INFO << " - QSO Date: " << util.getDateTimeSQLiteStringFromDateTime (_qso.getDateTimeOn());
    query.bindValue(":qso_date", util.getDateTimeSQLiteStringFromDateTime (_qso.getDateTimeOn()));
    query.bindValue(":call", _qso.getCall());
    query.bindValue(":rst_sent", _qso.getRSTTX());
    query.bindValue(":rst_rcvd", _qso.getRSTRX());
    //query.bindValue(":bandid", _qso.getBandIdFromBandName(false));
    query.bindValue(":bandid", getIdFromBandName(_qso.getBand()));
    //query.bindValue(":modeid", _qso.getModeIdFromModeName());
    query.bindValue(":modeid", getIdFromModeName(_qso.getMode()));
    query.bindValue(":cqz", _qso.getCQZone());
    query.bindValue(":ituz", _qso.getItuZone());
    query.bindValue(":dxcc", _qso.getDXCC());
    query.bindValue(":address", _qso.getAddress());
    Adif adif(Q_FUNC_INFO);
    if (adif.isValidAge(_qso.getAge()))
        query.bindValue(":age", _qso.getAge());
    query.bindValue(":altitude", _qso.getAltitude());
    if (adif.isValidA_Index(_qso.getA_Index()))
        query.bindValue(":a_index", _qso.getA_Index());
    if (adif.isValidAnt_AZ(_qso.getAnt_az()))
        query.bindValue(":ant_az", _qso.getAnt_az());
    if (adif.isValidAnt_EL((_qso.getAnt_el())))
        query.bindValue(":ant_el", _qso.getAnt_el());

    query.bindValue(":ant_path", _qso.getAnt_Path());
    query.bindValue(":arrl_sect", _qso.getARRL_Sect());
    query.bindValue(":award_submitted", _qso.getAwardSubmitted ());
    query.bindValue(":award_granted", _qso.getAwardGranted ());
    //query.bindValue(":band_rx", _qso.getBandIdFromBandName(true));
    query.bindValue(":band_rx", getIdFromBandName(_qso.getBandRX()));

    query.bindValue(":checkcontest", _qso.getCheck());
    query.bindValue(":class", _qso.getClass());

    query.bindValue(":cnty", _qso.getCounty());
    query.bindValue(":comment", _qso.getComment());
    query.bindValue(":cont", _qso.getContinent ());
    query.bindValue(":contacted_op", _qso.getContactedOperator());
    query.bindValue(":contest_id", _qso.getContestID());
    query.bindValue(":country", _qso.getCountry());
    query.bindValue(":credit_submitted", _qso.getCreditSubmitted());
    query.bindValue(":credit_granted,", _qso.getCreditGranted());
    query.bindValue(":darc_dok", _qso.getDarcDok ());
    if (adif.isValidDistance(_qso.getDistance()))
        query.bindValue(":distance", _qso.getDistance());
    query.bindValue(":email", _qso.getEmail());
    query.bindValue(":eq_call", _qso.getEQ_Call());


    if (adif.isValidFISTS(_qso.getFists()))
        query.bindValue(":fists", _qso.getFists ());
    if (adif.isValidFISTS(_qso.getFistsCC()))
        query.bindValue(":fists_cc", _qso.getFistsCC ());

    query.bindValue(":force_init", util.boolToCharToSQLite (_qso.getForceInit()));
    query.bindValue(":freq_tx", _qso.getFreqTX());
    query.bindValue(":freq_rx", _qso.getFreqRX());
    query.bindValue(":gridsquare", _qso.getGridSquare());
    query.bindValue(":gridsquare_ext", _qso.getGridSquare_ext());

    query.bindValue(":iota", _qso.getIOTA());
    if (adif.isValidIOTA_islandID(_qso.getIotaID()))
        query.bindValue(":iota_island_id", _qso.getIotaID());
    if (adif.isValidK_Index(_qso.getK_Index()))
        query.bindValue(":k_index", _qso.getK_Index());
    query.bindValue(":lat", _qso.getLatitude());
    query.bindValue(":lon", _qso.getLongitude());

    query.bindValue(":clublog_qso_upload_date", util.getDateSQLiteStringFromDate(_qso.getClubLogDate()));
    query.bindValue(":clublog_qso_upload_status", _qso.getClubLogStatus());

    query.bindValue(":hrdlog_qso_upload_date", _qso.getHRDUpdateDate ());
    query.bindValue(":hrdlog_qso_upload_status", _qso.getHRDLogStatus ());

    query.bindValue(":hamlogeu_qso_upload_date", _qso.getHamLogEUUpdateDate());
    query.bindValue(":hamlogeu_qso_upload_status", _qso.getHamLogEUStatus());
    query.bindValue(":hamqth_qso_upload_date", _qso.getHamQTHUpdateDate());
    query.bindValue(":hamqth_qso_upload_status", _qso.getHamQTHStatus());


    query.bindValue(":eqsl_qsl_rcvd", _qso.getEQSLQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(_qso.getEQSLQSL_RCVD()))
        query.bindValue(":eqsl_qslrdate", util.getDateSQLiteStringFromDate(_qso.getEQSLQSLRDate()));

    //qDebug() << Q_FUNC_INFO << "- eqsl_qsl_sent: " << getEQSLQSL_SENT();
    query.bindValue(":eqsl_qsl_sent", _qso.getEQSLQSL_SENT());

    if (qsl_sent_dates_set.contains(_qso.getEQSLQSL_SENT()))
        query.bindValue(":eqsl_qslsdate", util.getDateSQLiteStringFromDate(_qso.getEQSLQSLSDate()));

    query.bindValue(":qsl_rcvd", _qso.getQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(_qso.getQSL_RCVD()))
        query.bindValue(":qslrdate", util.getDateSQLiteStringFromDate(_qso.getQSLRDate()));

    query.bindValue(":qsl_sent", _qso.getQSL_SENT());
    if (qsl_sent_dates_set.contains(_qso.getQSL_SENT()))
        query.bindValue(":qslsdate", util.getDateSQLiteStringFromDate(_qso.getQSLSDate()));

    query.bindValue(":lotw_qsl_rcvd", _qso.getLoTWQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(_qso.getLoTWQSL_RCVD()))
        query.bindValue(":lotw_qslrdate", util.getDateSQLiteStringFromDate(_qso.getLoTWQSLRDate()));

    query.bindValue(":lotw_qsl_sent", _qso.getLoTWQSL_SENT());
    if (qsl_sent_dates_set.contains(_qso.getLoTWQSL_SENT()))
        query.bindValue(":lotw_qslsdate", util.getDateSQLiteStringFromDate(_qso.getLoTWQSLSDate()));


    query.bindValue(":qrzcom_qso_upload_date", util.getDateSQLiteStringFromDate(_qso.getQRZCOMDate ()));
    query.bindValue(":qrzcom_qso_upload_status", _qso.getQRZCOMStatus ());

    if (adif.isValidNRBursts(_qso.getMaxBursts()))
        query.bindValue(":max_bursts", _qso.getMaxBursts());
    query.bindValue(":ms_shower", _qso.getMsShower());
    query.bindValue(":my_altitude", _qso.getMyAltitude());
    query.bindValue(":my_antenna", _qso.getMyAntenna());
    query.bindValue(":my_arrl_sect", _qso.getMyARRL_Sect());
    query.bindValue(":my_city", _qso.getMyCity());

    query.bindValue(":my_cnty", _qso.getMyCounty());
    query.bindValue(":my_country", _qso.getMyCountry());
    query.bindValue(":my_cq_zone", _qso.getMyCQZone());
    query.bindValue(":my_dxcc", _qso.getMyDXCC ());
    query.bindValue(":my_fists", _qso.getMyFists ());
    query.bindValue(":my_gridsquare", _qso.getMyGridSquare());
    query.bindValue(":my_gridsquare_ext", _qso.getMyGridSquare_ext());
    query.bindValue(":my_iota", _qso.getMyIOTA());
    if (adif.isValidIOTA_islandID(_qso.getMyIotaID()))
        query.bindValue(":my_iota_island_id", _qso.getMyIotaID());
    query.bindValue(":my_itu_zone", _qso.getMyITUZone ());
    query.bindValue(":my_lat", _qso.getMyLatitude());
    query.bindValue(":my_lon", _qso.getMyLongitude());
    query.bindValue(":my_name", _qso.getMyName());
    query.bindValue(":my_pota_ref", _qso.getMyPOTA_Ref());

    query.bindValue(":my_postal_code", _qso.getMyPostalCode ());
    query.bindValue(":my_rig", _qso.getMyRig());

    query.bindValue(":my_sig", _qso.getMySig());
    query.bindValue(":my_sig_info", _qso.getMySigInfo());
    query.bindValue(":my_sota_ref", _qso.getMySOTA_REF());
    query.bindValue(":my_state", _qso.getMyState());
    query.bindValue(":my_street", _qso.getMyStreet());
    query.bindValue(":my_usaca_counties", _qso.getMyUsacaCounties ());
    query.bindValue(":my_wwff_ref", _qso.getMyWWFF_Ref());
    query.bindValue(":my_vucc_grids", _qso.getMyVUCCGrids());
    query.bindValue(":name", _qso.getName());
    query.bindValue(":notes", _qso.getNotes());
    if (adif.isValidNRBursts(_qso.getNrBursts()))
        query.bindValue(":nr_bursts", _qso.getNrBursts());
    if (adif.isValidPings(_qso.getNrPings()))
        query.bindValue(":nr_pings", _qso.getNrPings());
    query.bindValue(":operator", _qso.getOperatorCallsign());
    query.bindValue(":owner_callsign", _qso.getOwnerCallsign());
    query.bindValue(":pfx", _qso.getPrefix());

    query.bindValue(":pota_ref", _qso.getPOTA_Ref());
    query.bindValue(":precedence", _qso.getPrecedence());
    query.bindValue(":prop_mode", _qso.getPropMode());
    query.bindValue(":public_key", _qso.getPublicKey());


    query.bindValue(":qslmsg", _qso.getQSLMsg());


    query.bindValue(":qsl_rcvd_via", _qso.getQSLRecVia());
    query.bindValue(":qsl_sent_via", _qso.getQSLSentVia());
    query.bindValue(":qsl_via", _qso.getQSLVia());
    if (adif.isValidQSO_COMPLETE(_qso.getQSOComplete()))
        query.bindValue(":qso_complete", adif.setQSO_COMPLETEToDB(_qso.getQSOComplete()));
    query.bindValue(":qso_random", util.boolToCharToSQLite (_qso.getQSORandom()));
    query.bindValue(":qth", _qso.getQTH());
    query.bindValue(":region", _qso.getRegion ());
    query.bindValue(":rig", _qso.getRig ());
    if (adif.isValidPower(_qso.getRXPwr()))
        query.bindValue(":rx_pwr", _qso.getRXPwr());
    if (_qso.getPropMode() == "SAT")
    {
        query.bindValue(":sat_mode", _qso.getSatMode());
        query.bindValue(":sat_name", _qso.getSatName());
    }
    if (adif.isValidSFI(_qso.getSFI()))
        query.bindValue(":sfi", _qso.getSFI());
    query.bindValue(":sig", _qso.getSIG());
    query.bindValue(":sig_info", _qso.getSIG_INFO ());
    query.bindValue(":silent_key", util.boolToCharToSQLite (_qso.getSilentKey ()));
    query.bindValue(":skcc", _qso.getSkcc ());

    query.bindValue(":sota_ref", _qso.getSOTA_REF());
    query.bindValue(":srx_string", _qso.getSrxString());
    if (adif.isValidSRX(_qso.getSrx()))
        query.bindValue(":srx", _qso.getSrx());
    query.bindValue(":stx_string", _qso.getStxString());
    if (adif.isValidSTX(_qso.getStx()))
        query.bindValue(":stx", _qso.getStx());
    query.bindValue(":state", _qso.getState());
    query.bindValue(":station_callsign", _qso.getStationCallsign());
    // query.bindValue(":submode", _qso.getModeIdFromModeName());

    query.bindValue(":swl", util.boolToCharToSQLite (_qso.getSwl()));
    if (adif.isValidUKSMG(_qso.getUksmg()))
        query.bindValue(":uksmg", _qso.getUksmg ());
    query.bindValue(":usaca_counties", _qso.getUsacaCounties ());
    query.bindValue(":ve_prov", _qso.getVeProv ());
    query.bindValue(":wwff_ref", _qso.getWWFF_Ref());
    query.bindValue(":vucc_grids", _qso.getVUCCGrids());
    if (adif.isValidTenTen(_qso.getTenTen()))
        query.bindValue(":ten_ten", _qso.getTenTen());
    if (adif.isValidPower(_qso.getTXPwr()))
        query.bindValue(":tx_pwr", _qso.getTXPwr());
    query.bindValue(":web", _qso.getWeb());
    query.bindValue(":qso_date_off", util.getDateTimeSQLiteStringFromDateTime(_qso.getDateTimeOff()));
    query.bindValue(":lognumber", _qso.getLogId());

    // qVariantList list = query.boundValues();
    //for (int i = 0; i < list.size(); ++i)
    //qDebug() << Q_FUNC_INFO << QString(": %1").arg(i) << "/ " << list.at(i).toString().toUtf8().data() << "\n";

    //qDebug() << Q_FUNC_INFO << " - END";
    return query;
}

void DataProxy_SQLite::bindQSOValues(QSqlQuery &query, const QSO &_qso)
{
    static const QSet<QString> qsl_rcvd_dates_set = {"Y", "I", "V"};
    static const QSet<QString> qsl_sent_dates_set = {"Y", "Q", "I"};
    static Utilities util(Q_FUNC_INFO);
    static Adif adif(Q_FUNC_INFO);


    //QSqlQuery query;

    //qDebug() << Q_FUNC_INFO << " - Start ";
    //qDebug() << Q_FUNC_INFO << " - queryString: " << _s;

    //query.clear ();

    //qDebug() << Q_FUNC_INFO << " - Starting to bind values...";
    //Utilities util(Q_FUNC_INFO);
   //qDebug() << Q_FUNC_INFO << " - QSO Date: " << util.getDateTimeSQLiteStringFromDateTime (_qso.getDateTimeOn());
    query.bindValue(":qso_date", util.getDateTimeSQLiteStringFromDateTime (_qso.getDateTimeOn()));
    query.bindValue(":call", _qso.getCall());
    query.bindValue(":rst_sent", _qso.getRSTTX());
    query.bindValue(":rst_rcvd", _qso.getRSTRX());
    //query.bindValue(":bandid", _qso.getBandIdFromBandName(false));
    query.bindValue(":bandid", getIdFromBandName(_qso.getBand()));
    //query.bindValue(":modeid", _qso.getModeIdFromModeName());
    query.bindValue(":modeid", getIdFromModeName(_qso.getMode()));
    query.bindValue(":cqz", _qso.getCQZone());
    query.bindValue(":ituz", _qso.getItuZone());
    query.bindValue(":dxcc", _qso.getDXCC());
    query.bindValue(":address", _qso.getAddress());
    //Adif adif(Q_FUNC_INFO);
    if (adif.isValidAge(_qso.getAge()))
        query.bindValue(":age", _qso.getAge());
    query.bindValue(":altitude", _qso.getAltitude());
    if (adif.isValidA_Index(_qso.getA_Index()))
        query.bindValue(":a_index", _qso.getA_Index());
    if (adif.isValidAnt_AZ(_qso.getAnt_az()))
        query.bindValue(":ant_az", _qso.getAnt_az());
    if (adif.isValidAnt_EL((_qso.getAnt_el())))
        query.bindValue(":ant_el", _qso.getAnt_el());

    query.bindValue(":ant_path", _qso.getAnt_Path());
    query.bindValue(":arrl_sect", _qso.getARRL_Sect());
    query.bindValue(":award_submitted", _qso.getAwardSubmitted ());
    query.bindValue(":award_granted", _qso.getAwardGranted ());
    //query.bindValue(":band_rx", _qso.getBandIdFromBandName(true));
    query.bindValue(":band_rx", getIdFromBandName(_qso.getBandRX()));

    query.bindValue(":checkcontest", _qso.getCheck());
    query.bindValue(":class", _qso.getClass());

    query.bindValue(":cnty", _qso.getCounty());
    query.bindValue(":comment", _qso.getComment());
    query.bindValue(":cont", _qso.getContinent ());
    query.bindValue(":contacted_op", _qso.getContactedOperator());
    query.bindValue(":contest_id", _qso.getContestID());
    query.bindValue(":country", _qso.getCountry());
    query.bindValue(":credit_submitted", _qso.getCreditSubmitted());
    query.bindValue(":credit_granted,", _qso.getCreditGranted());
    query.bindValue(":darc_dok", _qso.getDarcDok ());
    if (adif.isValidDistance(_qso.getDistance()))
        query.bindValue(":distance", _qso.getDistance());
    query.bindValue(":email", _qso.getEmail());
    query.bindValue(":eq_call", _qso.getEQ_Call());


    if (adif.isValidFISTS(_qso.getFists()))
        query.bindValue(":fists", _qso.getFists ());
    if (adif.isValidFISTS(_qso.getFistsCC()))
        query.bindValue(":fists_cc", _qso.getFistsCC ());

    query.bindValue(":force_init", util.boolToCharToSQLite (_qso.getForceInit()));
    query.bindValue(":freq_tx", _qso.getFreqTX());
    query.bindValue(":freq_rx", _qso.getFreqRX());
    query.bindValue(":gridsquare", _qso.getGridSquare());
    query.bindValue(":gridsquare_ext", _qso.getGridSquare_ext());

    query.bindValue(":iota", _qso.getIOTA());
    if (adif.isValidIOTA_islandID(_qso.getIotaID()))
        query.bindValue(":iota_island_id", _qso.getIotaID());
    if (adif.isValidK_Index(_qso.getK_Index()))
        query.bindValue(":k_index", _qso.getK_Index());
    query.bindValue(":lat", _qso.getLatitude());
    query.bindValue(":lon", _qso.getLongitude());

    query.bindValue(":clublog_qso_upload_date", util.getDateSQLiteStringFromDate(_qso.getClubLogDate()));
    query.bindValue(":clublog_qso_upload_status", _qso.getClubLogStatus());

    query.bindValue(":hrdlog_qso_upload_date", _qso.getHRDUpdateDate ());
    query.bindValue(":hrdlog_qso_upload_status", _qso.getHRDLogStatus ());

    query.bindValue(":hamlogeu_qso_upload_date", _qso.getHamLogEUUpdateDate());
    query.bindValue(":hamlogeu_qso_upload_status", _qso.getHamLogEUStatus());
    query.bindValue(":hamqth_qso_upload_date", _qso.getHamQTHUpdateDate());
    query.bindValue(":hamqth_qso_upload_status", _qso.getHamQTHStatus());


    query.bindValue(":eqsl_qsl_rcvd", _qso.getEQSLQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(_qso.getEQSLQSL_RCVD()))
        query.bindValue(":eqsl_qslrdate", util.getDateSQLiteStringFromDate(_qso.getEQSLQSLRDate()));

    //qDebug() << Q_FUNC_INFO << "- eqsl_qsl_sent: " << getEQSLQSL_SENT();
    query.bindValue(":eqsl_qsl_sent", _qso.getEQSLQSL_SENT());

    if (qsl_sent_dates_set.contains(_qso.getEQSLQSL_SENT()))
        query.bindValue(":eqsl_qslsdate", util.getDateSQLiteStringFromDate(_qso.getEQSLQSLSDate()));

    query.bindValue(":qsl_rcvd", _qso.getQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(_qso.getQSL_RCVD()))
        query.bindValue(":qslrdate", util.getDateSQLiteStringFromDate(_qso.getQSLRDate()));

    query.bindValue(":qsl_sent", _qso.getQSL_SENT());
    if (qsl_sent_dates_set.contains(_qso.getQSL_SENT()))
        query.bindValue(":qslsdate", util.getDateSQLiteStringFromDate(_qso.getQSLSDate()));

    query.bindValue(":lotw_qsl_rcvd", _qso.getLoTWQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(_qso.getLoTWQSL_RCVD()))
        query.bindValue(":lotw_qslrdate", util.getDateSQLiteStringFromDate(_qso.getLoTWQSLRDate()));

    query.bindValue(":lotw_qsl_sent", _qso.getLoTWQSL_SENT());
    if (qsl_sent_dates_set.contains(_qso.getLoTWQSL_SENT()))
        query.bindValue(":lotw_qslsdate", util.getDateSQLiteStringFromDate(_qso.getLoTWQSLSDate()));


    query.bindValue(":qrzcom_qso_upload_date", util.getDateSQLiteStringFromDate(_qso.getQRZCOMDate ()));
    query.bindValue(":qrzcom_qso_upload_status", _qso.getQRZCOMStatus ());

    if (adif.isValidNRBursts(_qso.getMaxBursts()))
        query.bindValue(":max_bursts", _qso.getMaxBursts());
    query.bindValue(":ms_shower", _qso.getMsShower());
    query.bindValue(":my_altitude", _qso.getMyAltitude());
    query.bindValue(":my_antenna", _qso.getMyAntenna());
    query.bindValue(":my_arrl_sect", _qso.getMyARRL_Sect());
    query.bindValue(":my_city", _qso.getMyCity());

    query.bindValue(":my_cnty", _qso.getMyCounty());
    query.bindValue(":my_country", _qso.getMyCountry());
    query.bindValue(":my_cq_zone", _qso.getMyCQZone());
    query.bindValue(":my_dxcc", _qso.getMyDXCC ());
    query.bindValue(":my_fists", _qso.getMyFists ());
    query.bindValue(":my_gridsquare", _qso.getMyGridSquare());
    query.bindValue(":my_gridsquare_ext", _qso.getMyGridSquare_ext());
    query.bindValue(":my_iota", _qso.getMyIOTA());
    if (adif.isValidIOTA_islandID(_qso.getMyIotaID()))
        query.bindValue(":my_iota_island_id", _qso.getMyIotaID());
    query.bindValue(":my_itu_zone", _qso.getMyITUZone ());
    query.bindValue(":my_lat", _qso.getMyLatitude());
    query.bindValue(":my_lon", _qso.getMyLongitude());
    query.bindValue(":my_name", _qso.getMyName());
    query.bindValue(":my_pota_ref", _qso.getMyPOTA_Ref());

    query.bindValue(":my_postal_code", _qso.getMyPostalCode ());
    query.bindValue(":my_rig", _qso.getMyRig());

    query.bindValue(":my_sig", _qso.getMySig());
    query.bindValue(":my_sig_info", _qso.getMySigInfo());
    query.bindValue(":my_sota_ref", _qso.getMySOTA_REF());
    query.bindValue(":my_state", _qso.getMyState());
    query.bindValue(":my_street", _qso.getMyStreet());
    query.bindValue(":my_usaca_counties", _qso.getMyUsacaCounties ());
    query.bindValue(":my_wwff_ref", _qso.getMyWWFF_Ref());
    query.bindValue(":my_vucc_grids", _qso.getMyVUCCGrids());
    query.bindValue(":name", _qso.getName());
    query.bindValue(":notes", _qso.getNotes());
    if (adif.isValidNRBursts(_qso.getNrBursts()))
        query.bindValue(":nr_bursts", _qso.getNrBursts());
    if (adif.isValidPings(_qso.getNrPings()))
        query.bindValue(":nr_pings", _qso.getNrPings());
    query.bindValue(":operator", _qso.getOperatorCallsign());
    query.bindValue(":owner_callsign", _qso.getOwnerCallsign());
    query.bindValue(":pfx", _qso.getPrefix());

    query.bindValue(":pota_ref", _qso.getPOTA_Ref());
    query.bindValue(":precedence", _qso.getPrecedence());
    query.bindValue(":prop_mode", _qso.getPropMode());
    query.bindValue(":public_key", _qso.getPublicKey());


    query.bindValue(":qslmsg", _qso.getQSLMsg());


    query.bindValue(":qsl_rcvd_via", _qso.getQSLRecVia());
    query.bindValue(":qsl_sent_via", _qso.getQSLSentVia());
    query.bindValue(":qsl_via", _qso.getQSLVia());
    if (adif.isValidQSO_COMPLETE(_qso.getQSOComplete()))
        query.bindValue(":qso_complete", adif.setQSO_COMPLETEToDB(_qso.getQSOComplete()));
    query.bindValue(":qso_random", util.boolToCharToSQLite (_qso.getQSORandom()));
    query.bindValue(":qth", _qso.getQTH());
    query.bindValue(":region", _qso.getRegion ());
    query.bindValue(":rig", _qso.getRig ());
    if (adif.isValidPower(_qso.getRXPwr()))
        query.bindValue(":rx_pwr", _qso.getRXPwr());
    if (_qso.getPropMode() == "SAT")
    {
        query.bindValue(":sat_mode", _qso.getSatMode());
        query.bindValue(":sat_name", _qso.getSatName());
    }
    if (adif.isValidSFI(_qso.getSFI()))
        query.bindValue(":sfi", _qso.getSFI());
    query.bindValue(":sig", _qso.getSIG());
    query.bindValue(":sig_info", _qso.getSIG_INFO ());
    query.bindValue(":silent_key", util.boolToCharToSQLite (_qso.getSilentKey ()));
    query.bindValue(":skcc", _qso.getSkcc ());

    query.bindValue(":sota_ref", _qso.getSOTA_REF());
    query.bindValue(":srx_string", _qso.getSrxString());
    if (adif.isValidSRX(_qso.getSrx()))
        query.bindValue(":srx", _qso.getSrx());
    query.bindValue(":stx_string", _qso.getStxString());
    if (adif.isValidSTX(_qso.getStx()))
        query.bindValue(":stx", _qso.getStx());
    query.bindValue(":state", _qso.getState());
    query.bindValue(":station_callsign", _qso.getStationCallsign());
    // query.bindValue(":submode", _qso.getModeIdFromModeName());

    query.bindValue(":swl", util.boolToCharToSQLite (_qso.getSwl()));
    if (adif.isValidUKSMG(_qso.getUksmg()))
        query.bindValue(":uksmg", _qso.getUksmg ());
    query.bindValue(":usaca_counties", _qso.getUsacaCounties ());
    query.bindValue(":ve_prov", _qso.getVeProv ());
    query.bindValue(":wwff_ref", _qso.getWWFF_Ref());
    query.bindValue(":vucc_grids", _qso.getVUCCGrids());
    if (adif.isValidTenTen(_qso.getTenTen()))
        query.bindValue(":ten_ten", _qso.getTenTen());
    if (adif.isValidPower(_qso.getTXPwr()))
        query.bindValue(":tx_pwr", _qso.getTXPwr());
    query.bindValue(":web", _qso.getWeb());
    query.bindValue(":qso_date_off", util.getDateTimeSQLiteStringFromDateTime(_qso.getDateTimeOff()));
    query.bindValue(":lognumber", _qso.getLogId());

    // qVariantList list = query.boundValues();
    //for (int i = 0; i < list.size(); ++i)
    //qDebug() << Q_FUNC_INFO << QString(": %1").arg(i) << "/ " << list.at(i).toString().toUtf8().data() << "\n";

    //qDebug() << Q_FUNC_INFO << " - END";
    //return query;
}


QSO DataProxy_SQLite::fromDB(const int _qsoId)
{

    // TODO: Be aware that the function QString DataProxy_SQLite::getADIFFromQSOQuery
    // has a similar function
    // Make sure that all fields are included inbot functions until consolidated
    logEvent (Q_FUNC_INFO, "Start", Debug);
   //qDebug() << Q_FUNC_INFO << " - Start: " << _qsoId;

    if (_qsoId<1)
        return QSO();

    QString queryString = "SELECT log.*, "
                          "t_band.name AS band_name, "
                          "t_band_rx.name AS bandrx_name, "
                          "t_mode.name AS mode_name, "
                          "t_mode.submode AS submode_name "
                          "FROM log "
                          "LEFT JOIN band AS t_band ON log.bandid = t_band.id "
                          "LEFT JOIN band AS t_band_rx ON log.band_rx = t_band_rx.id "
                          "LEFT JOIN mode AS t_mode ON log.modeid = t_mode.id "
                          "WHERE log.id = :idQSO";


    QSqlQuery query;
    query.prepare(queryString);

   //qDebug() << Q_FUNC_INFO << " - query = " << queryString;
    query.bindValue(":idQSO", _qsoId);
    QSO qso;
    qso.clear();

    if (!query.exec() )
    {
       //qDebug() << Q_FUNC_INFO << " - ERROR in exec ";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return qso;
    }
    if (!query.next())
    {
       //qDebug() << Q_FUNC_INFO << " - ERROR in next ";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return qso;
    }

    QSqlRecord rec = query.record();

   //qDebug() << Q_FUNC_INFO << "  - 20";
    Utilities util(Q_FUNC_INFO);
    QString data = (query.value(rec.indexOf("qso_date"))).toString();

    qso.setDateTimeOn(util.getDateTimeFromSQLiteString(data));

    data = (query.value(rec.indexOf("call"))).toString();
    qso.setCall(data);

    data = (query.value(rec.indexOf("rst_sent"))).toString();
    qso.setRSTTX(data);

    data = (query.value(rec.indexOf("rst_rcvd"))).toString();
    qso.setRSTRX(data);

    data = (query.value(rec.indexOf("mode_name"))).toString();
    qso.setMode(data);
    data = (query.value(rec.indexOf("submode_name"))).toString();
    qso.setSubmode(data);
    //qDebug() << Q_FUNC_INFO << "  - 30";
    qso.setCQZone((query.value(rec.indexOf("cqz"))).toInt());
    qso.setItuZone((query.value(rec.indexOf("ituz"))).toInt());
    qso.setDXCC((query.value(rec.indexOf("dxcc"))).toInt());

    qso.setAddress((query.value(rec.indexOf("address"))).toString());
    qso.setAge((query.value(rec.indexOf("age"))).toDouble());
    qso.setAltitude((query.value(rec.indexOf("altitude"))).toDouble());
    qso.setCounty((query.value(rec.indexOf("cnty"))).toString());

    qso.setA_Index((query.value(rec.indexOf("a_index"))).toDouble());
    qso.setAnt_az((query.value(rec.indexOf("ant_az"))).toDouble());
    qso.setAnt_el((query.value(rec.indexOf("ant_el"))).toDouble());
    qso.setAnt_Path((query.value(rec.indexOf("ant_path"))).toString());

    qso.setARRL_Sect((query.value(rec.indexOf("arrl_sect"))).toString());
    qso.setAwardSubmitted((query.value(rec.indexOf("award_submitted"))).toString());
    qso.setAwardGranted((query.value(rec.indexOf("award_granted"))).toString());

    qso.setCheck((query.value(rec.indexOf("checkcontest"))).toString());
    qso.setClass((query.value(rec.indexOf("class"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 40";

    data = (query.value(rec.indexOf("clublog_qso_upload_date"))).toString();
    qso.setClubLogDate(util.getDateFromSQliteString(data));
    qso.setClubLogStatus((query.value(rec.indexOf("clublog_qso_upload_status"))).toString());
    qso.setComment((query.value(rec.indexOf("comment"))).toString());
    qso.setContinent((query.value(rec.indexOf("cont"))).toString());
    qso.setContactedOperator((query.value(rec.indexOf("contacted_op"))).toString());
    qso.setContestID((query.value(rec.indexOf("contest_id"))).toString());

    qso.setCountry((query.value(rec.indexOf("country"))).toString());
    qso.setCreditSubmitted((query.value(rec.indexOf("credit_submitted"))).toString());
    qso.setCreditGranted((query.value(rec.indexOf("credit_granted"))).toString());

    qso.setDarcDok((query.value(rec.indexOf("darc_dok"))).toString());
    qso.setDistance((query.value(rec.indexOf("distance"))).toDouble());

    qso.setEmail((query.value(rec.indexOf("email"))).toString());
    qso.setEQ_Call((query.value(rec.indexOf("eq_call"))).toString());

    data = (query.value(rec.indexOf("eqsl_qslrdate"))).toString();
    qso.setEQSLQSLRDate(util.getDateFromSQliteString(data));
    //qDebug() << Q_FUNC_INFO;
    data = (query.value(rec.indexOf("eqsl_qslsdate"))).toString();
    //qDebug() << Q_FUNC_INFO << "  - 49: " << data;

    qso.setEQSLQSLSDate(util.getDateFromSQliteString(data));
    //qDebug() << Q_FUNC_INFO << "  - 50";
    qso.setEQSLQSL_RCVD((query.value(rec.indexOf("eqsl_qsl_rcvd"))).toString());
    qso.setEQSLQSL_SENT((query.value(rec.indexOf("eqsl_qsl_sent"))).toString());
    qso.setFists((query.value(rec.indexOf("fists"))).toInt());
    qso.setFistsCC((query.value(rec.indexOf("fists_cc"))).toInt());
    qso.setForceInit(util.QStringToBool((query.value(rec.indexOf("force_init"))).toString()));

    qso.setFreq((query.value(rec.indexOf("freq"))).toDouble());
    data = (query.value(rec.indexOf("band_name"))).toString();
    qso.setBand(data);
    if (!isValidBand(qso.getBand()))
        qso.setBandRX(getBandNameFromFreq(qso.getFreqTX()));

    qso.setFreqRX((query.value(rec.indexOf("freq_rx"))).toDouble());
    data = (query.value(rec.indexOf("bandrx_name"))).toString();
    qso.setBandRX(data);
    if (!isValidBand(qso.getBandRX()))
        qso.setBandRX(getBandNameFromFreq(qso.getFreqRX()));

    qso.setGridSquare((query.value(rec.indexOf("gridsquare"))).toString());
    qso.setGridSquare_ext((query.value(rec.indexOf("gridsquare_ext"))).toString());
    data = (query.value(rec.indexOf("hrdlog_qso_upload_date"))).toString();
    qso.setHRDUpdateDate(util.getDateFromSQliteString(data));

    //qDebug() << Q_FUNC_INFO << "  - 60";
    qso.setHRDLogStatus((query.value(rec.indexOf("hrdlog_qso_upload_status"))).toString());

    data = (query.value(rec.indexOf("hamlogeu_qso_upload_date"))).toString();
    qso.setHamLogEUUpdateDate(util.getDateFromSQliteString(data));
    qso.setHamLogEUStatus((query.value(rec.indexOf("hamlogeu_qso_upload_status"))).toString());

    data = (query.value(rec.indexOf("hamqth_qso_upload_date"))).toString();
    qso.setHamQTHUpdateDate(util.getDateFromSQliteString(data));
    qso.setHamQTHStatus((query.value(rec.indexOf("hamqth_qso_upload_status"))).toString());


    //qDebug() << Q_FUNC_INFO << "  - 61";
    qso.setIOTA((query.value(rec.indexOf("iota"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 62";
    qso.setIotaID((query.value(rec.indexOf("iota_island_id"))).toInt());
    //qDebug() << Q_FUNC_INFO << "  - 63";
    qso.setK_Index((query.value(rec.indexOf("k_index"))).toInt());
    //qDebug() << Q_FUNC_INFO << "  - 64";
    qso.setLatitude((query.value(rec.indexOf("lat"))).toString());
    qso.setLongitude((query.value(rec.indexOf("lon"))).toString());

    data = (query.value(rec.indexOf("lotw_qslrdate"))).toString();
   //qDebug() << Q_FUNC_INFO << " - lotq_qslrdate - DB_ " << data;
    qso.setLoTWQSLRDate(util.getDateFromSQliteString(data));

    data = (query.value(rec.indexOf("lotw_qslsdate"))).toString();
    qso.setLoTWQSLSDate(util.getDateFromSQliteString(data));

    qso.setLoTWQSL_RCVD((query.value(rec.indexOf("lotw_qsl_rcvd"))).toString());
    qso.setLoTWQSL_SENT((query.value(rec.indexOf("lotw_qsl_sent"))).toString());

    qso.setMaxBursts((query.value(rec.indexOf("max_bursts"))).toInt());
    qso.setMsShower((query.value(rec.indexOf("ms_shower"))).toString());
    qso.setMyAltitude((query.value(rec.indexOf("my_altitude"))).toDouble());
    qso.setMyAntenna((query.value(rec.indexOf("my_antenna"))).toString());
    qso.setMyARRL_Sect((query.value(rec.indexOf("my_arrl_sect"))).toString());
    qso.setMyCity((query.value(rec.indexOf("my_city"))).toString());
    qso.setMyCounty((query.value(rec.indexOf("my_cnty"))).toString());
    qso.setMyCountry((query.value(rec.indexOf("my_country"))).toString());
    qso.setMyCQZone((query.value(rec.indexOf("my_cq_zone"))).toInt());
    qso.setMyITUZone((query.value(rec.indexOf("my_itu_zone"))).toInt());
    qso.setMyDXCC((query.value(rec.indexOf("my_dxcc"))).toInt());
    qso.setMyFists((query.value(rec.indexOf("my_fists"))).toInt());
    qso.setMyGridSquare((query.value(rec.indexOf("my_gridsquare"))).toString());
    qso.setMyGridSquare_ext((query.value(rec.indexOf("my_gridsquare_ext"))).toString());
    qso.setMyIOTA((query.value(rec.indexOf("my_iota"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 80";
    qso.setMyLatitude((query.value(rec.indexOf("my_lat"))).toString());
    qso.setMyLongitude((query.value(rec.indexOf("my_lon"))).toString());

    qso.setMyName((query.value(rec.indexOf("my_name"))).toString());
    //qDebug() << Q_FUNC_INFO << " - MY_POTA_REF: " << (query.value(rec.indexOf("my_pota_ref"))).toString();
    qso.setMyPOTA_Ref((query.value(rec.indexOf("my_pota_ref"))).toString());
    qso.setMyPostalCode((query.value(rec.indexOf("my_postal_code"))).toString());
    qso.setMyRig((query.value(rec.indexOf("my_rig"))).toString());
    qso.setMySig((query.value(rec.indexOf("my_sig"))).toString());
    //qDebug() << Q_FUNC_INFO << " - MY_SIG_INFO: " << (query.value(rec.indexOf("my_sig_info"))).toString();
    qso.setMySigInfo((query.value(rec.indexOf("my_sig_info"))).toString());
    qso.setMySOTA_REF((query.value(rec.indexOf("my_sota_ref"))).toString());
    qso.setMyState((query.value(rec.indexOf("my_state"))).toString());
    qso.setMyStreet((query.value(rec.indexOf("my_street"))).toString());
    qso.setMyUsacaCounties((query.value(rec.indexOf("my_usaca_counties"))).toString());
    qso.setMyWWFF_Ref((query.value(rec.indexOf("my_wwff_ref"))).toString());
    qso.setMyVUCCGrids((query.value(rec.indexOf("my_vucc_grids"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 90";
    qso.setName((query.value(rec.indexOf("name"))).toString());
    qso.setNotes((query.value(rec.indexOf("notes"))).toString());

    qso.setNrBursts((query.value(rec.indexOf("nr_bursts"))).toInt());
    qso.setNrPings((query.value(rec.indexOf("nr_pings"))).toInt());
    qso.setOperatorCallsign((query.value(rec.indexOf("operator"))).toString());
    qso.setOwnerCallsign((query.value(rec.indexOf("owner_callsign"))).toString());
    qso.setPrefix((query.value(rec.indexOf("pfx"))).toString());
    qso.setPOTA_Ref((query.value(rec.indexOf("pota_ref"))).toString());
    qso.setPrecedence((query.value(rec.indexOf("precedence"))).toString());
    qso.setPropMode((query.value(rec.indexOf("prop_mode"))).toString());
    qso.setPublicKey((query.value(rec.indexOf("public_key"))).toString());

    data = (query.value(rec.indexOf("qrzcom_qso_upload_date"))).toString();
    qso.setQRZCOMDate(util.getDateFromSQliteString(data));
    qso.setQRZCOMStatus((query.value(rec.indexOf("qrzcom_qso_upload_status"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 100";
    qso.setQSLMsg((query.value(rec.indexOf("qslmsg"))).toString());
    data = (query.value(rec.indexOf("qslrdate"))).toString();
    qso.setQSLRDate(util.getDateFromSQliteString(data));
    data = (query.value(rec.indexOf("qslsdate"))).toString();
    qso.setQSLSDate(util.getDateFromSQliteString(data));

    qso.setQSL_RCVD((query.value(rec.indexOf("qsl_rcvd"))).toString());
    qso.setQSL_SENT((query.value(rec.indexOf("qsl_sent"))).toString());
    qso.setQSLRecVia((query.value(rec.indexOf("qsl_rcvd_via"))).toString());
    qso.setQSLSenVia((query.value(rec.indexOf("qsl_sent_via"))).toString());

    qso.setQSLVia((query.value(rec.indexOf("qsl_via"))).toString());
    Adif adif(Q_FUNC_INFO);
    qso.setQSOComplete(adif.getQSO_COMPLETEFromDB((query.value(rec.indexOf("qso_complete"))).toString()));
    qso.setQSORandom(util.QStringToBool((query.value(rec.indexOf("qso_random"))).toString()));
    //qDebug() << Q_FUNC_INFO << "  - 120";
    qso.setQTH((query.value(rec.indexOf("qth"))).toString());
    qso.setRegion((query.value(rec.indexOf("region"))).toString());
    qso.setRig((query.value(rec.indexOf("rig"))).toString());
    qso.setRXPwr((query.value(rec.indexOf("rig"))).toDouble());

    qso.setSatName((query.value(rec.indexOf("sat_name"))).toString());
    qso.setSatMode((query.value(rec.indexOf("sat_mode"))).toString());

    qso.setSFI((query.value(rec.indexOf("sfi"))).toInt());
    qso.setSIG((query.value(rec.indexOf("sig"))).toString());
    qso.setSIG_INFO((query.value(rec.indexOf("sig_info"))).toString());

    qso.setSilentKey(util.QStringToBool((query.value(rec.indexOf("silent_key"))).toString()));
    qso.setSkcc((query.value(rec.indexOf("skcc"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 130";
    qso.setSOTA_REF((query.value(rec.indexOf("sota_ref"))).toString());
    qso.setSrxString((query.value(rec.indexOf("srx_string"))).toString());
    qso.setSrx((query.value(rec.indexOf("srx"))).toInt());
    qso.setStxString((query.value(rec.indexOf("stx_string"))).toString());
    qso.setStx((query.value(rec.indexOf("stx"))).toInt());

    qso.setState((query.value(rec.indexOf("state"))).toString());
    qso.setStationCallsign((query.value(rec.indexOf("station_callsign"))).toString());

    qso.setSwl(util.QStringToBool((query.value(rec.indexOf("swl"))).toString()));
    qso.setUksmg((query.value(rec.indexOf("uksmg"))).toString());
    qso.setUsacaCounties((query.value(rec.indexOf("usaca_counties"))).toString());
    qso.setVeProv((query.value(rec.indexOf("ve_prov"))).toString());
    qso.setVUCCGrids((query.value(rec.indexOf("vucc_grids"))).toString());
    qso.setWWFF_Ref((query.value(rec.indexOf("wwff_ref"))).toString());
    qso.setTenTen((query.value(rec.indexOf("ten_ten"))).toInt());
    qso.setTXPwr((query.value(rec.indexOf("tx_pwr"))).toDouble());
    qso.setRXPwr((query.value(rec.indexOf("rx_pwr"))).toDouble());
    //qDebug() << Q_FUNC_INFO << "  - 140";
    qso.setWeb((query.value(rec.indexOf("web"))).toString());

    data = (query.value(rec.indexOf("qso_date_off"))).toString();
    qso.setDateTimeOff(util.getDateTimeFromSQLiteString(data));

    qso.setLogId((query.value(rec.indexOf("lognumber"))).toInt());
    //qDebug() << Q_FUNC_INFO << "  - 150";
    logEvent (Q_FUNC_INFO, "END", Debug);

    return qso;
}

void DataProxy_SQLite::prepareStaticQueries()
{
    if (m_queriesPrepared)
            return;

        m_insertQuery = QSqlQuery();
        if (!m_insertQuery.prepare(getAddQueryString()))
        {
            emit queryError(Q_FUNC_INFO,
                            m_insertQuery.lastError().databaseText(),
                            m_insertQuery.lastError().text(),
                            QStringLiteral("prepareStaticQueries: INSERT failed"));
            return; // NO marcar como preparado → reintentará en el siguiente addQSO
        }

        m_updateQuery = QSqlQuery();
        if (!m_updateQuery.prepare(getModifyQueryString()))
        {
            emit queryError(Q_FUNC_INFO,
                            m_updateQuery.lastError().databaseText(),
                            m_updateQuery.lastError().text(),
                            QStringLiteral("prepareStaticQueries: UPDATE failed"));
            return; // NO marcar como preparado
        }

        m_queriesPrepared = true;
}

bool DataProxy_SQLite::updateQSOFromLoTW(const QSO &_lotwQso, const int _qsoId)
{
    if (_qsoId <= 0)
        return false;

    QSO qso = fromDB(_qsoId);   // Existing QSO
    if (!qso.isValid())
        return false;

    if (!qso.mergeLoTWData(_lotwQso))  // Modifies the QSO with LoTW
        return false;

    return (addQSO(qso) > 0);   // Save the QSO
}

bool DataProxy_SQLite::applyLoTWFieldsToQSO(const QSO &_lotwQso, const int _qsoId)
{
    if (_qsoId <= 0)
        return false;

    QStringList setClauses;
    Utilities util(Q_FUNC_INFO);
    Adif adif(Q_FUNC_INFO);

    // Solo añadir al UPDATE los campos que LoTW trae informados
    if (_lotwQso.getLoTWQSL_RCVD() == "Y")
    {
        setClauses << "lotw_qsl_rcvd = 'Y'";
        if (_lotwQso.getLoTWQSLRDate().isValid())
            setClauses << QString("lotw_qslrdate = '%1'")
                              .arg(util.getDateSQLiteStringFromDate(_lotwQso.getLoTWQSLRDate()));
    }

    if (_lotwQso.getLoTWQSL_SENT() == "Y")
    {
        setClauses << "lotw_qsl_sent = CASE WHEN lotw_qsl_sent != 'Y' THEN 'Y' ELSE lotw_qsl_sent END";
        if (_lotwQso.getLoTWQSLSDate().isValid())
            setClauses << QString("lotw_qslsdate = '%1'")
                              .arg(util.getDateSQLiteStringFromDate(_lotwQso.getLoTWQSLSDate()));
    }

    if (!_lotwQso.getCreditGranted().isEmpty())
        setClauses << QString("credit_granted = '%1'").arg(_lotwQso.getCreditGranted());

    if (!_lotwQso.getCreditSubmitted().isEmpty())
        setClauses << QString("credit_submitted = '%1'").arg(_lotwQso.getCreditSubmitted());

    if (!_lotwQso.getCounty().isEmpty())
        setClauses << QString("cnty = '%1'").arg(_lotwQso.getCounty());

    if (adif.isValidContinent(_lotwQso.getContinent()))
        setClauses << QString("cont = '%1'").arg(_lotwQso.getContinent());

    if (adif.isValidCQz(_lotwQso.getCQZone()))
        setClauses << QString("cqz = '%1'").arg(_lotwQso.getCQZone());

    if (adif.isValidITUz(_lotwQso.getItuZone()))
        setClauses << QString("ituz = '%1'").arg(_lotwQso.getItuZone());

    Locator locator;
    if (locator.isValidLocator(_lotwQso.getGridSquare()))
        setClauses << QString("gridsquare = '%1'").arg(_lotwQso.getGridSquare());

    if (!_lotwQso.getState().isEmpty())
        setClauses << QString("state = '%1'").arg(_lotwQso.getState());

    if (setClauses.isEmpty())
        return true;    // LoTW no trajo nada útil, no hay nada que hacer

    QString queryString = QString("UPDATE log SET %1 WHERE id = '%2'")
                              .arg(setClauses.join(", "))
                              .arg(_qsoId);

    QSqlQuery query;
    if (!query.exec(queryString))
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(),
                        query.lastError().text(), query.lastQuery());
        return false;
    }
    return true;
}

bool DataProxy_SQLite::deleteQSO(const int _qsoId)
{
   //qDebug() << Q_FUNC_INFO << " - Start: Deleting QSO from log: " << _qsoId;
    QSqlQuery query;
    QString queryString = QString("DELETE FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.finish();
        removeDuplicateCache(_qsoId);
        emit qsoDeleted(_qsoId);  // Deleted QSO specificc QSO
        emit logChanged();        // generic signal to trigger slotRefreshYearsComboBox
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
}

void DataProxy_SQLite::removeDuplicateCache(int _qsoId)
{
   //qDebug() << Q_FUNC_INFO << " - Start: Deleting QSO from cache: " << _qsoId;
   //qDebug() << Q_FUNC_INFO << " - Start - " << QString("Cached %1 entries").arg(m_qsoCache.count()) ;
    auto it = m_qsoCache.begin();

    while (it != m_qsoCache.end())
    {
        // Again, assuming .first is the ID in your QsoInfo
        if (it.value().first == _qsoId)
        {
            it = m_qsoCache.erase(it); // Safely remove it
            break;                     // ID found and removed, we can stop
        }
        else
        {
            ++it;
        }
    }
   //qDebug() << Q_FUNC_INFO << " - END - " << QString("End - Cached %1 entries").arg(m_qsoCache.count()) ;
}

int DataProxy_SQLite::isWorkedB4(const QString &_qrz, const int _currentLog)
{
         //qDebug() << Q_FUNC_INFO << " -";
    //Returns the QSO id
    QSqlQuery query;
    //query.setForwardOnly(true);
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



int DataProxy_SQLite::isThisQSODuplicated (const QSO &_qso, const int _secs)
{
   //qDebug() << Q_FUNC_INFO << " - 000";
    int bandId = getIdFromBandName(_qso.getBand());
    int modeId = getIdFromModeName(_qso.getMode());
    return findDuplicateId(_qso.getCall(), _qso.getDateTimeOn(), bandId, modeId, _secs );
}

bool DataProxy_SQLite::isHF(const int _band)
{// 160M is considered as HF
    const BandEntry b = m_cache.getBandFromId(_band);

    if (!b.isValid()) return false;
      return b.minFreq >= Frequency(1.8, MHz) && b.maxFreq <= Frequency(30.0, MHz);
}

bool DataProxy_SQLite::isWARC(const int _band)
{
    const QString name = m_cache.getBandFromId(_band).name;
    return name == "30M" || name == "17M" || name == "12M";
}

bool DataProxy_SQLite::isVHF(const int _band)
{
    const BandEntry b = m_cache.getBandFromId(_band);
    if (!b.isValid()) return false;
    return b.minFreq >= Frequency(40.0, MHz) && b.maxFreq <= Frequency(300.0, MHz);
}

bool DataProxy_SQLite::isUHF(const int _band)
{
    const BandEntry b = m_cache.getBandFromId(_band);
    if (!b.isValid()) return false;
    return b.minFreq >= Frequency(300.0, MHz) && b.maxFreq <= Frequency(3000.0, MHz);
}

QStringList DataProxy_SQLite::getOperatingYears(const int _currentLog)
{
       //qDebug() << Q_FUNC_INFO << " - " << QString::number(_currentLog);
    QStringList years = QStringList();
    // qStringList yearsSorted = QStringList();
    QSqlQuery query;
    //query.setForwardOnly(true);
    QString queryString;
    if (_currentLog<0)
    {
        queryString = QString("SELECT DISTINCT (substr (qso_date, 0, 5)) FROM log ORDER BY 'qso_date'");
    }
    else
    {
        queryString = QString("SELECT DISTINCT (substr (qso_date, 0, 5)) FROM log WHERE lognumber='%0' ORDER BY 'qso_date'").arg(_currentLog);
    }

    // qString year = QString();
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
           // qString year = (query.value(0)).toString();
        //qDebug() << Q_FUNC_INFO << " - year=" << year;
            years << (query.value(0)).toString();
            //year.clear();
        }
    }
           //qDebug() << Q_FUNC_INFO << " - END OK - " << QString::number(years.size())<< QT_ENDL;
    query.finish();
    if (!years.isEmpty())
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
        //Utilities util(Q_FUNC_INFO);
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

        //Utilities util(Q_FUNC_INFO);
    if (_currentLog<1)
    {
        if (_station == "ALL")
        {
            queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE lotw_qsl_sent == 'Q'");
        }
        else
        {
                        queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE lotw_qsl_sent == 'Q' AND station_callsign='%2'")
                                .arg(util->getDateSQLiteStringFromDate(_updateDate), _station);
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
        //Utilities util(Q_FUNC_INFO);
    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << Q_FUNC_INFO << " -: updating QSO: " << QString::number(_qsos.at(i));

         // queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE id='%2'").arg((QDate::currentDate()).toString("yyyy-MM-dd")).arg(QString::number(_qsos.at(i)));
                 queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE id='%2'")
                         .arg(util->getDateSQLiteStringFromDate(QDate::currentDate()), QString::number(_qsos.at(i)));
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
        //Utilities util(Q_FUNC_INFO);
     //qDebug() << Q_FUNC_INFO << " - " << _call << "/" << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << "/" <<_band <<"/"<<_mode << "/" << util->getADIFDateFromQDate(_qslrdate)  << endl ;
    int bandid = getIdFromBandName(_band);
    int modeid = getIdFromModeName(_mode);

    QString qso_date;
    qso_date = util->getDateTimeSQLiteStringFromDateTime(_dateTime);

    QString queryString;
    // queryString = QString("SELECT id, lotw_qsl_rcvd FROM log WHERE call='%1' AND qso_date='%2' AND bandid='%4' AND modeid='%5'").arg(_call).arg(qso_date).arg(bandid).arg(modeid);
    queryString = QString("SELECT id, lotw_qsl_rcvd FROM log WHERE call='%1' AND qso_date='%2' AND bandid='%4' AND modeid='%5'")
            .arg(_call, qso_date).arg(bandid, modeid);

    QSqlQuery query;
    //query.setForwardOnly(true);
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

                // qString qslsdate = (QDate::fromString(_qslsdate, "yyyyMMdd")).toString("yyyy-MM-dd");
                // qString qslrdate = (QDate::fromString(_qslrdate, "yyyyMMdd")).toString("yyyy-MM-dd");
                queryString = QString("UPDATE log SET lotw_qsl_rcvd = 'Y', lotw_qslrdate = '%1' WHERE id='%2'")
                    .arg(util->getDateSQLiteStringFromDate(_qslrdate), QString::number(id));

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
        //Utilities util(Q_FUNC_INFO);
    Locator locator;
    if (locator.isValidLocator(_a))
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
        //QStringList qs;
        //qs.clear();
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

    queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 AND %3 %4 AND %5 AND %6")
            .arg(_queryST_string, _queryGrid_string).arg(_query_justQueued, _query_logNumber, _queryDateFrom, _queryDateTo);


    QList <int> qsoList;
    qsoList.clear();

    QSqlQuery query; //query.setForwardOnly(true);
    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery() ;
    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();
                //Utilities util(Q_FUNC_INFO);
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
        //qs.sort();
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
        //Utilities util(Q_FUNC_INFO);
        queryString = QString("SELECT DISTINCT my_gridsquare FROM log WHERE %1 AND ((my_gridsquare<>'') OR (my_gridsquare IS NOT NULL)) AND qso_date>='%2' AND qso_date<='%3' AND %4 %5").arg(_queryST_string).arg(util->getDateSQLiteStringFromDate(_startDate)).arg(util->getDateSQLiteStringFromDate(_endDate.addDays (1))).arg(_query_justQueued).arg(_queryST_logNumber);

    QSqlQuery query; //query.setForwardOnly(true);

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

        //QStringList qs;
        //qs.clear();
        //QString queryString;

        //QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

        //QString _query_justModified;
        //if (_justModified)
        //{
        //qDebug() << Q_FUNC_INFO << " - justQueued TRUE";
        //    _query_justModified = QString("clublog_qso_upload_status='M'");
        //}
        //else
        //{
        //qDebug() << Q_FUNC_INFO << " - justQueued FALSE";
        //    _query_justModified = QString("clublog_qso_upload_status!='M'");
        //}
    //qDebug() << Q_FUNC_INFO << " - logN: " << QString::number(_logN);

        //QString _query_logNumber = getStringQueryLogNumber(_logN);


        //QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
        //QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

        //queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5").arg(_queryST_string).arg(_query_justModified).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);

    QString queryString = getStringQueryForQSOsLists(_stationCallsign, ClubLog, _justModified,_startDate, _endDate, _logN);
    //QString queryString = getStringQueryForQSOsLists(_stationCallsign, _query_justModified, _startDate, _endDate, _logN);

    QSqlQuery query; //query.setForwardOnly(true);

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();
                //Utilities util(Q_FUNC_INFO);
                QDate tmpDate;
                QString aux = QString();
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
        //qs.sort();
    //qDebug() << Q_FUNC_INFO << " - Returning: #"  << QString::number(qsoList.length());
    return qsoList;
}
QString DataProxy_SQLite::getStringQueryForQSOsLists(
        const QString& _stationCallsign,
        const OnLineProvider& _provider,
        const bool _justModified,
        const QDate &_startDate,
        const QDate &_endDate,
        const int _logN)
{
    QString _justModifiedS = getQueryJustModifiedString(_provider, _justModified);
    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);
    QString _query_logNumber = getStringQueryLogNumber(_logN);
    QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
    QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

    return QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5")
            .arg(_queryST_string, _justModifiedS, _query_logNumber, _queryDateFrom, _queryDateTo);
}

// EA4K optimizando
QString DataProxy_SQLite::getQueryJustModifiedString(const OnLineProvider& _provider, const bool _justModified)
{//enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ}; //, HamQTH, HRDLog
    switch (_provider) {
            case eQSL:
                    if (_justModified)
                        return QString("eqsl_qsl_sent='Q'");
                    return QString("eqsl_qsl_sent!='M'");
            break;
        case QRZ:
                if (_justModified)
                    return QString("qrzcom_qso_upload_status='M'");
                return QString("qrzcom_qso_upload_status!='-'");
        break;
        case ClubLog:
                if (_justModified)
                    return QString("clublog_qso_upload_status='M'");
                return QString("clublog_qso_upload_status!='M'");
        break;
        default:
            break;
        }
    return QString();
}


QList<int> DataProxy_SQLite::getQSOsListEQSLToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified, int _logN)
{
   //qDebug() << Q_FUNC_INFO << " -Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd");

    QList <int> qsoList;
    qsoList.clear();

        //QStringList qs;
        //qs.clear();
        //QString queryString;
        //QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

        //QString _query_justModified;
        //if (_justModified)
        //{
        //qDebug() << Q_FUNC_INFO << " -justQueued TRUE";
        //    _query_justModified = QString("eqsl_qsl_sent='Q'");
        //}
        //else
        //{
        //qDebug() << Q_FUNC_INFO << " -justQueued FALSE";
        //    _query_justModified = QString("eqsl_qsl_sent!='M'");
        //}

        //QString _query_logNumber = getStringQueryLogNumber(_logN);
        //QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
        //QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

        //queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5").arg(_queryST_string).arg(_query_justModified).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);



        QString queryString = getStringQueryForQSOsLists(_stationCallsign, eQSL, _justModified,_startDate, _endDate, _logN);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " -Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " -Query: " << query.lastQuery();
                //Utilities util(Q_FUNC_INFO);
                QDate tmpDate;
                QString aux = QString();
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
        //qs.sort();
    return qsoList;
}


QList<int> DataProxy_SQLite::getQSOsListQRZCOMToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified, int _logN)
{
    //qDebug() << Q_FUNC_INFO << " - Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd");

    QList <int> qsoList;
    qsoList.clear();


        //QStringList qs;
        //qs.clear();
        //QString queryString;

        //QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);

        //QString _query_justModified;
        //if (_justModified)
        //{
        //qDebug() << Q_FUNC_INFO << " - justQueued TRUE";
        //    _query_justModified = QString("qrzcom_qso_upload_status='M'");
        //}
        //else
        //{
        //qDebug() << Q_FUNC_INFO << " - justQueued FALSE";
        //    _query_justModified = QString("qrzcom_qso_upload_status!='-'");
        //}

        //QString _query_logNumber = getStringQueryLogNumber(_logN);
        //QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
        //QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

        //queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5").arg(_queryST_string).arg(_query_justModified).arg(_query_logNumber).arg(_queryDateFrom).arg(_queryDateTo);

        QString queryString = getStringQueryForQSOsLists(_stationCallsign, QRZ, _justModified,_startDate, _endDate, _logN);

    QSqlQuery query; //query.setForwardOnly(true);

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();
                //Utilities util(Q_FUNC_INFO);
                QString aux = QString();
                QDate tmpDate;
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
        //qs.sort();
    return qsoList;
}


QList<int> DataProxy_SQLite::getQSOsListToBeExported(const QString &_stationCallsign, const QString &_grid, const QDate &_startDate, const QDate &_endDate, int _logN)
{
    //qDebug() << Q_FUNC_INFO << QString("Call: %1, Grid: %2, StartDate: %3, EndDate: %4").arg(_stationCallsign).arg(_grid).arg(_startDate.toString("yyyyMMdd")).arg(_endDate.toString("yyyyMMdd"));

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
        //QStringList qs;
        //qs.clear();
    QString queryString;

    QString _queryST_string = getStringQueryStationCallSign(_stationCallsign);
    QString _queryGrid_String = getStringQueryMyGrid (_grid);
    QString _query_logNumber = getStringQueryLogNumber(_logN);
    QString _queryDateFrom = QString(" date(qso_date)>=date('%1')").arg(_startDate.toString ("yyyy-MM-dd"));
    QString _queryDateTo = QString(" date(qso_date)<=date('%1')").arg(_endDate.toString ("yyyy-MM-dd"));

    queryString = QString("SELECT id, qso_date FROM log WHERE %1 AND %2 %3 AND %4 AND %5 ")
            .arg(_queryST_string, _queryGrid_String, _query_logNumber, _queryDateFrom, _queryDateTo);;

    QSqlQuery query; //query.setForwardOnly(true);

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << ": Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << ": Query: " << query.lastQuery();
                //Utilities util(Q_FUNC_INFO);
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
        //qs.sort();
    return qsoList;
}

QList<int> DataProxy_SQLite::getQSOsAll()
{
    QString queryString = QString("SELECT id FROM log") ;
    QSqlQuery query; //query.setForwardOnly(true);
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
        //QStringList qs;
        //qs.clear();
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
    //query.setForwardOnly(true);
    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();

    if (sqlOK)
    {
       // //qDebug() << Q_FUNC_INFO << " - Query: " << query.lastQuery();
                //Utilities util(Q_FUNC_INFO);
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
        //qs.sort();
    return qsoList;
}



QStringList DataProxy_SQLite::getQSODetailsForLoTWDownload(const int _id)
{ //Returns QRZ << date+time << Band (txt) << mode (txt)
    //qDebug() << Q_FUNC_INFO << " -" << QString::number(_id);
    QStringList result;
    result.clear();
    //getNameFromBandId
    QSqlQuery query; //query.setForwardOnly(true);
    // qString queryString = QString("SELECT call, qso_date, my_gridsquare, bandid, modeid FROM log WHERE id='%0'").arg(_id);
    QString queryString = QString("SELECT call, qso_date, my_gridsquare, band.name, mode.name FROM log JOIN band ON log.bandid=band.id JOIN mode on log.modeid=mode.id WHERE log.id='%0'").arg(_id);


    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                                //Utilities util(Q_FUNC_INFO);
                QString call = query.value(0).toString();
                QString date = query.value(1).toString();
                                // qString date = util->getDateTimeFromSQLiteString(query.value(1).toString());

                QString myGrid = query.value(2).toString();
                QString bandid = query.value(3).toString();
                QString modeid = query.value(4).toString();
                query.finish();
                //qDebug() << Q_FUNC_INFO << " - - date: " << date;
                //qDebug() << Q_FUNC_INFO << " - - time: " << time;

                //getDateTimeSQLiteStringFromDateTime
                // qString dateTime = (QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss")).toString("yyyy-MM-dd hh:mm");
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

    QSqlQuery query; //query.setForwardOnly(true);
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

    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
  QSqlQuery query; //query.setForwardOnly(true);
  QString queryString;
  bool sqlOK;
  if (_logNumber < 0)
  {
      // queryString = QString("SELECT COUNT (DISTINCT id) FROM log where dxcc LIKE '%1'").arg(_dxcc);
      queryString = QString("SELECT COUNT (DISTINCT id) FROM log where dxcc = '%1'").arg(_dxcc);
  }
  else
  {
      // queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND dxcc LIKE '%2'").arg(_logNumber).arg(_dxcc);
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
  QSqlQuery query; //query.setForwardOnly(true);
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
   QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query; //query.setForwardOnly(true);
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
     QSqlQuery query; //query.setForwardOnly(true);

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


Frequency DataProxy_SQLite::getSatelliteUplink(const QString &_sat, int _pair)
{
    //qDebug()  << Q_FUNC_INFO << " - " << _sat;
    QString aux = QString();
    // qString aux2 = QString();
    //double fr1, fr2, fr;
    QString queryString = QString("SELECT uplink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query; //query.setForwardOnly(true);
    Frequency freq;
    freq.clear();
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
            freq = getFreqFromRange(aux, _pair, MHz);
            //aux = freq.toQString();
        }
        else
        {
                 //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return freq;
        }
    }
    else
    {
             //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return freq;
    }

         //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    query.finish();
    return freq;
}


Frequency DataProxy_SQLite::getSatelliteDownlink(const QString &_sat, int _pair)
{
         //qDebug()  << Q_FUNC_INFO << " - " << _sat;
    QString aux = QString();
    // qString aux2 = QString();
    //double fr1, fr2, fr;
    QString queryString = QString("SELECT downlink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query; //query.setForwardOnly(true);
    Frequency freq;
    freq.clear();

    bool sqlOK = query.exec(queryString);


    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            aux = query.value(0).toString();
            freq = getFreqFromRange(aux,_pair, MHz);
            //aux = freq.toQString();
        }
        else
        {
                 //qDebug()  << Q_FUNC_INFO << " -  query not valid" ;
            query.finish();
            return freq;
        }
    }
    else
    {
             //qDebug()  << Q_FUNC_INFO << " -  query failed: " << query.lastQuery() ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return freq;
    }

         //qDebug()  << Q_FUNC_INFO << " - final: " << aux;
    query.finish();
    return freq;
}

QString DataProxy_SQLite::getSatelliteMode(const QString &_sat)
{
    QString aux = QString();
    QString queryString = QString("SELECT satmode FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query; //query.setForwardOnly(true);

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
    // qString aux2 = QString();
    //double fr1, fr2, fr;
    QString queryString = QString("SELECT downlink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query; //query.setForwardOnly(true);

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
 QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query; //query.setForwardOnly(true);

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

Frequency DataProxy_SQLite::getFreqFromRange(const QString &_fr, int _pair, FreqUnits freqUnits)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    Frequency freq;

    if (_fr.trimmed().isEmpty())
        return freq;

    QString aux = _fr.trimmed();

    // Step 1: Split the pairs if more than one
    if (aux.contains(','))
    {
        const QStringList pairs = aux.split(',', Qt::SkipEmptyParts);
        const int clampedPair = qBound(0, _pair, pairs.size() - 1);
        aux = pairs.at(clampedPair).trimmed();
    }

    // Step 2: Return the mid frequency from the pair
    if (aux.contains('-'))
    {
        const QString fr1str = aux.section('-', 0, 0).trimmed();
        const QString fr2str = aux.section('-', 1, 1).trimmed();

        Frequency f1, f2;
        if (!f1.fromQString(fr1str, freqUnits) || !f2.fromQString(fr2str, freqUnits))
            return freq;

        const double mid = qAbs(f1.toDouble(MHz) + f2.toDouble(MHz)) / 2.0;
        freq.fromDouble(mid, MHz);
    }
    else
    {
        // Only one freq, ensure the value is positive
        bool ok;
        const double val = qAbs(aux.toDouble(&ok));
        if (ok && val > 0.0)
            freq.fromDouble(val, freqUnits);
    }

    return freq;
}

/*
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
*/

QStringList DataProxy_SQLite::getQSLViaList()
{
         //qDebug()  << Q_FUNC_INFO << " -" ;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString queryString = QString("SELECT shortname, name FROM qsl_via_enumeration");
     QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query;
    bool sqlOK = false;
        //Utilities util(Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << " - Importing: " << _regionalAward.getRegionalAwardRefName(i) ;
    // Use a prepared statement to safely handle subdivision names that contain
    // apostrophes (e.g. "L'Hospitalet de Llobregat"), preventing SQL injection.
    query.prepare("INSERT INTO primary_subdivisions (name, shortname, prefix, regionalgroup, "
                  "regionalid, dxcc, cqz, ituz, start_date, end_date, deleted) "
                  "VALUES (:name, :shortname, :prefix, :regionalgroup, "
                  ":regionalid, :dxcc, :cqz, :ituz, :start_date, :end_date, :deleted)");
    query.bindValue(":name",         _name);
    query.bindValue(":shortname",    _short);
    query.bindValue(":prefix",       _pref);
    query.bindValue(":regionalgroup",_group);
    query.bindValue(":regionalid",   _regId);
    query.bindValue(":dxcc",         _dxcc);
    query.bindValue(":cqz",          _cq);
    query.bindValue(":ituz",         _itu);
    query.bindValue(":start_date",   util->getDateSQLiteStringFromDate(_startDate));
    query.bindValue(":end_date",     util->getDateSQLiteStringFromDate(_endDate));
    query.bindValue(":deleted",      util->boolToCharToSQLite(_deleted));
    sqlOK = query.exec();

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
    int normalizedInt = _entity % 1000;

    QSqlQuery query; //query.setForwardOnly(true);
    QString queryString;
    if ((!_pref.isEmpty()))
    {
      //qDebug() << Q_FUNC_INFO << " - Running for no pref, delivering ALL for the entity";
        queryString = QString("SELECT dxcc, prefix, name, shortname, cqz, ituz FROM primary_subdivisions WHERE prefix = :prefix ORDER BY shortname");
        query.prepare(queryString);
        query.bindValue(":prefix", _pref);
    }
    else
    {
      //qDebug() << Q_FUNC_INFO << " - Running without a pref, delivering just for the prefix";
      //qDebug() << Q_FUNC_INFO << QString("If the entity is <=0 The list will be empty. Entity: %1").arg(normalizedInt);
        if (normalizedInt<=0)
        {
          //qDebug() << Q_FUNC_INFO << " - END: entity <= 0";
            return list;
        }
        queryString = QString("SELECT dxcc, prefix, name, shortname, cqz, ituz FROM primary_subdivisions WHERE dxcc = :dxcc ORDER BY shortname");
        query.prepare(queryString);
        query.bindValue(":dxcc", normalizedInt);
    }

    bool sqlOK = query.exec();

    if (sqlOK)
    {
     //qDebug() << Q_FUNC_INFO << ": sqlOK true";

        while (query.next())
        {
            bool dupe = false;
            if (query.isValid())
            {
                PrimarySubdivision ps;
                //ps.dxcc = (query.value(0)).toInt();
                ps.dxcc = _entity;
                ps.prefix = (query.value(1)).toString();
                ps.name = (query.value(2)).toString();
                ps.shortName = (query.value(3)).toString();
                ps.cqz = (query.value(4)).toInt();
                ps.ituz = (query.value(5)).toInt();

                PrimarySubdivision aux;
                foreach (aux, list)
                {
                    if (aux.name == ps.name)
                        dupe = true;
                }
                if (!dupe)
                    list.append(ps);

             //qDebug() << Q_FUNC_INFO << " : " << ps.name ;
            }
            else
            {
             //qDebug() << Q_FUNC_INFO << ": query not valid";
            }
        }
     //qDebug() << Q_FUNC_INFO << ": Afterthe while, query no more next";
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
    QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
   QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);

    // Bind parameters to the prepared query
    query.prepare(queryString);
    // query.bindValue(":lotwStatus", "Q");
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
    QSqlQuery query; //query.setForwardOnly(true);
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
  QSqlQuery query; //query.setForwardOnly(true);
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
  QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query;
    //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
    QString queryString;
    bool sqlOK;
    if (_logNumber < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log where continent = '%1'").arg(_cont);
    }
    else
    {
        // queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND cont LIKE '%2'").arg(_logNumber).arg(_cont);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
    if (_log < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT dxcc) from log where band.id= :bandid AND (qsl_rcvd= :confirmed OR lotw_qsl_rcvd= :confirmed)");
    }
    else
    {
        queryString = QString("SELECT COUNT (DISTINCT dxcc) from log where band.id= :bandid AND log.id= :log AND (qsl_rcvd= :confirmed OR _qsl_rcvd= :confirmed)");
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
    bool sqlOK;

    if (editing == "1")
    { // We are editing
            //qDebug() << Q_FUNC_INFO << " - We are editing!";
        // queryString = QString("UPDATE logs SET logdate = '%1', stationcall = '%2', operators = '%3', comment = '%4',  logtype = '%5', logtypen = '%6' WHERE id = '%7'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(_typeContest).arg(_typeContestN).arg(id);
        queryString = QString("UPDATE logs SET logdate = '%1', stationcall = '%2', operators = '%3', comment = '%4' WHERE id = '%5'")
                .arg(_dateString, _stationCallsign, _operators, _comment).arg(id);
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

    queryString = QString("SELECT id FROM logs WHERE logdate='%1' AND stationcall='%2' AND operators = '%3' AND comment = '%4'")
            .arg(_dateString, _stationCallsign, _operators, _comment);
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
    queryString = QString("INSERT INTO logs (logdate, stationcall, operators, comment) values('%1','%2','%3','%4')")
            .arg(_dateString).arg(_stationCallsign, _operators, _comment);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);

    // qString queryString= QString("SELECT continent FROM entity WHERE dxcc='%1'").arg(_n);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    Adif adif(Q_FUNC_INFO);
    return adif.getContinents();
}

bool DataProxy_SQLite::isValidContinentShortName(const QString &_n)
{
    Adif adif(Q_FUNC_INFO);
    return adif.isValidContinent(_n);
}

bool DataProxy_SQLite::isValidDXCC(const int _e)
{
    QString queryString = QString("SELECT id FROM entity WHERE dxcc ='%1'").arg(_e);
    QSqlQuery query; //query.setForwardOnly(true);
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
                //Utilities util(Q_FUNC_INFO);
                returningFields = util->getDefaultLogFields();
    }

    return returningFields;
}

int DataProxy_SQLite::getITUzFromPrefix(const QString &_p)
{
    QSqlQuery query; //query.setForwardOnly(true);
    // qString queryString = QString("SELECT ituz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
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
    QSqlQuery query; //query.setForwardOnly(true);
    // qString queryString = QString("SELECT cqz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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

    QSqlQuery query; //query.setForwardOnly(true);
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
  QSqlQuery query; //query.setForwardOnly(true);

  queryString = QString("SELECT dxcc FROM entity WHERE name='%1'").arg(_e);
  // queryString = "SELECT prefix FROM prefixesofentity WHERE dxcc=='" + QString::number(i) +"'";
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);

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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
    QString queryString = QString("SELECT id FROM log WHERE cqz='%1'").arg(_c);
    // queryString = "SELECT id FROM log WHERE cqz=='" + QString::number(_cqz) +"'";
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

    // qString queryString;
    QSqlQuery query; //query.setForwardOnly(true);
    QString queryString = QString("SELECT id FROM log WHERE dxcc='%1'").arg(_e);
    // queryString = "SELECT id FROM log WHERE dxcc=='" + QString::number(_entityN) +"'";

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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);
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
    QSqlQuery query; //query.setForwardOnly(true);

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
                //qDebug() << Q_FUNC_INFO << ": " << aux;
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

/*
bool DataProxy_SQLite::getFreqHashData()
{
    // 1. Efficiency: Set forward-only flag for quicker sequential reads
    QSqlQuery query;
    //query.setForwardOnly(true);

    freqBandIDHash.clear();

    QString queryString = "SELECT id, lower FROM band";

    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return false;
    }
    else
    {
        while (query.next())
        {
            if (query.isValid())
            {
                // 2. Data Integrity: Retrieve data using native double type for better precision/speed
                int id = query.value(0).toInt();
                double lowerFreqValue = query.value(1).toDouble();

                Frequency freq;
                freq.fromDouble(lowerFreqValue);

                if (freq.isValid())
                {
                    freqBandIDHash.insert(id, freq);
                }
                // TODO: Log an error if a row has a valid ID but invalid frequency object
            }
        }
    }
    query.finish();
    return true;
}
*/

QHash<QString, int> DataProxy_SQLite::getWorldData()
{
    //qDebug() << Q_FUNC_INFO << "Start";
    QHash<QString, int> world;
    world.clear();

    QString queryString;
    QSqlQuery query;
    //query.setForwardOnly(true);
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
    //query.setForwardOnly(true);

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

QStringList DataProxy_SQLite::getEntitiesNames(bool _dxccOnly)
{
    //qDebug()  << Q_FUNC_INFO << " -" ;

    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT mainprefix, name, dxcc FROM entity");
    QSqlQuery query;
    //query.setForwardOnly(true);

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                if (_dxccOnly)
                {
                    if (query.value(2).toInt()<1000)
                    {
                        aux = (query.value(0)).toString() + "-" + (query.value(1)).toString()+" ("+(query.value(2)).toString()+")";
                    }
                }
                else
                {
                    aux = (query.value(0)).toString() + "-" + (query.value(1)).toString()+" ("+(query.value(2)).toString()+")";
                }
               //qDebug() << Q_FUNC_INFO << ": Adding: " << aux;
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
    return qs;
}

QStringList DataProxy_SQLite::getEntitiesIds()
{
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT dxcc FROM entity");
    QSqlQuery query;
    //query.setForwardOnly(true);

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
    //query.setForwardOnly(true);
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

QString DataProxy_SQLite::getADIFValueFromRec(const QSqlRecord& _rec, const QString &_fieldName)
{// To refactor the getADIFFromQSOQuery function
    return _rec.value (_fieldName).toString ();

    //int nameCol = _rec.indexOf(_fieldName);
    //if (nameCol>=0)
    //{
    //    QSqlField field = _rec.field(nameCol);
    //    return (field.value()).toString();
    //}
    //return QString();
}


QString DataProxy_SQLite::getADIFFromQSOQuery(QSqlRecord rec, ExportMode _em, bool _justMarked, bool _onlyRequested, const int _logN )
{   //qDebug() << Q_FUNC_INFO << ": " <<  query.lastQuery();
    //qDebug() << Q_FUNC_INFO << ": START";
        //TODO:
        // Be aware that the function bool QSO::fromDB(int _qsoId)
        // has a similar function
        // Make sure that all fields are included inbot functions until consolidated
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
        //Utilities util(Q_FUNC_INFO);
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

    qso.setAltitude((getADIFValueFromRec(rec, "altitude")).toDouble());
    qso.setAwardSubmitted(getADIFValueFromRec(rec, "award_submitted"));
    qso.setAwardGranted(getADIFValueFromRec(rec, "award_granted"));
    qso.setGridSquare_ext(getADIFValueFromRec(rec, "gridsquare_ext"));

    qso.setHamLogEUStatus(getADIFValueFromRec(rec, "hamlogeu_qso_upload_status"));
    QDateTime tDateTime;
    aux = getADIFValueFromRec(rec, "hamlogeu_qso_upload_date");
        tDateTime = util->getDateTimeFromSQLiteString(aux);
    qso.setHamLogEUUpdateDate(tDateTime.date());

    aux = getADIFValueFromRec(rec, "hamqth_qso_upload_date");
        tDateTime = util->getDateTimeFromSQLiteString(aux);
    qso.setHamQTHUpdateDate(tDateTime.date());
    // qso.setHamQTHUpdateDate(getADIFValueFromRec(rec, "hamqth_qso_upload_date"));
    qso.setHamQTHStatus(getADIFValueFromRec(rec, "hamqth_qso_upload_status"));
    qso.setMyARRL_Sect(getADIFValueFromRec(rec, "my_arrl_sect"));
    qso.setMyGridSquare_ext(getADIFValueFromRec(rec, "my_gridsquare_ext"));
    qso.setMyPOTA_Ref(getADIFValueFromRec(rec, "my_pota_ref"));
    qso.setMySigInfo(getADIFValueFromRec(rec, "my_sig_info"));
    qso.setMyWWFF_Ref(getADIFValueFromRec(rec, "my_wwff_ref"));
    qso.setPOTA_Ref(getADIFValueFromRec(rec, "pota_ref"));
    qso.setWWFF_Ref(getADIFValueFromRec(rec, "wwff_ref"));

    qso.setCall(getADIFValueFromRec(rec, "call"));


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
    // qString aux2 = getSubModeFromId(aux.toInt());

    qso.setMode(getSubModeFromId(aux.toInt()));

    aux = getADIFValueFromRec(rec, "submode");
    qso.setSubmode(getSubModeFromId(aux.toInt()));

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
    Adif adif(Q_FUNC_INFO);
    adif.getQSO_COMPLETEFromDB(getADIFValueFromRec(rec, "qso_complete"));
        // qso.setQSOComplete(util->getADIFQSO_CompleteFromDB(getADIFValueFromRec(rec, "qso_complete")));
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
        stringQuery = QString("SELECT call, qso_date, band.name, mode.submode, entity.name, log.dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log, entity, band, mode where log.dxcc <>''  AND sat_name <>'' AND log.dxcc=entity.dxcc AND log.bandid=band.id AND log.modeid=mode.id AND lognumber='%1' ORDER BY entity.name").arg(_log);
        //stringQuery = QString("SELECT call, qso_date, band.name, mode.name, entity.name, log.dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log, entity, band, mode where log.dxcc <>''  AND sat_name <>'' AND log.dxcc=entity.dxcc AND log.bandid=band.id AND log.modeid=mode.id AND lognumber='%1' ORDER BY entity.name").arg(_log);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": log does not exist " ;
        stringQuery = QString("SELECT call, qso_date, band.name, mode.submode, entity.name, log.dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log, entity, band, mode where log.dxcc <>''  AND sat_name <>'' AND log.dxcc=entity.dxcc AND log.bandid=band.id AND log.modeid=mode.id ORDER BY entity.name");
    }

    QSqlQuery query;
    //query.setForwardOnly(true);
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
                //Utilities util(Q_FUNC_INFO);
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
                _qso->setMode(query.value(3).toString());
                //_qso->setSubmode(query.value(3).toString());  // SetMode sets also submode
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
    //query.setForwardOnly(true);
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
                //_qso->setMode(getNameFromModeId((query.value(nameCol)).toInt()));     // SetMode sets also submode
                _qso->setMode (getSubModeFromId ((query.value(nameCol)).toInt()));   // SetMode sets also submode

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
    //query.setForwardOnly(true);
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
                //Utilities util(Q_FUNC_INFO);
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
                //_qso->setMode(getNameFromModeId((query.value(nameCol)).toInt())); // SetMode sets also submode
                _qso->setSubmode(getSubModeFromId((query.value(nameCol)).toInt())); // SetMode sets also submode

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
    //query.setForwardOnly(true);
    QString modeString = QString();


   int modeId = getIdFromModeName(_mode);
   if (_mode.toUpper() == "ALL")
   {
       //qDebug() << Q_FUNC_INFO << ": ALL Modes" ;
   }
   else if (modeId > 0)
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

    stringQuery = QString("SELECT COUNT (DISTINCT %1) from log WHERE %2 %3 %4 %5 %6")
            .arg(field, specialField, modeString, bandString, confirmedString, logString);

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

QString DataProxy_SQLite::generateGroupingKey(const QString &call, int bandId, int modeId)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    return QString("%1|%2|%3")
            .arg(call.toUpper())
            .arg(bandId)
            .arg(modeId);
}

int DataProxy_SQLite::findDuplicateId(const QString &call, const QDateTime &newTime,
                                       int bandId, int modeId, int marginSeconds)
{
    QString key = generateGroupingKey(call, bandId, modeId);
    QList<QsoInfo> potentialDupes = m_qsoCache.values(key);
    const QDateTime normalizedNew = normalizeForCache(newTime);  // ← normalizar

    for (const QsoInfo &info : std::as_const(potentialDupes))
    {
        qint64 diff = qAbs(info.second.secsTo(normalizedNew));
        if (diff <= marginSeconds)
            return info.first;
    }
    return -1;
}

void DataProxy_SQLite::loadDuplicateCache(int logId)
{
    logEvent(Q_FUNC_INFO, "Start", Devel);
   //qDebug() << Q_FUNC_INFO << " - Start";
    clearDuplicateCache();

    // Select only necessary fields.
    // Optimization: query.value(index) is faster with explicit column names.
    QString queryString = "SELECT id, call, qso_date, bandid, modeid FROM log";
    if (logId > 0)
    {
        queryString += QString(" WHERE lognumber=%1").arg(logId);
    }

    QSqlQuery query;
    query.setForwardOnly(true);

    if (query.exec(queryString))
    {
                //Utilities util(Q_FUNC_INFO);
        while (query.next())
        {
            int id = query.value(0).toInt();
            QString call = query.value(1).toString();
            // Convert SQLite string to QDateTime
            //QDateTime datetime = util->getDateTimeFromSQLiteString(query.value(2).toString());
            int band = query.value(3).toInt();
            int mode = query.value(4).toInt();

            QDateTime datetime = normalizeForCache(
            util->getDateTimeFromSQLiteString(query.value(2).toString()));
            if (datetime.isValid()) {
                QString key = generateGroupingKey(call, band, mode);
                QsoInfo qsoInfo(id, datetime);

                // QMultiHash::insert does NOT overwrite.
                // It appends the value to the list of values for this key.
                m_qsoCache.insert(key, qsoInfo);
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
   //qDebug() << Q_FUNC_INFO << " - END - " << QString("End - Cached %1 entries").arg(m_qsoCache.count()) ;
    logEvent(Q_FUNC_INFO, QString("End - Cached %1 entries").arg(m_qsoCache.count()), Debug);
}

void DataProxy_SQLite::addDuplicateCache (int _qsoId, const QSO &qso, const int _bandId, const int _modeId)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    //int bandId = getIdFromBandName(qso.getBand());
    //int modeId = getIdFromModeName(qso.getMode());
        // qString date = util->getDateTimeSQLiteStringFromDateTime(qso.getDateTimeOn());
    QString key = generateGroupingKey(qso.getCall(), _bandId, _modeId);
    QsoInfo qsoInfo(_qsoId, normalizeForCache(qso.getDateTimeOn()));  // ← normalizar
    m_qsoCache.insert(key, qsoInfo);

   //qDebug() << Q_FUNC_INFO << " - END - " << QString("End - Cached %1 entries").arg(m_qsoCache.count()) ;
    //qDebug() << Q_FUNC_INFO << "Key: " << key;
}

void DataProxy_SQLite::clearDuplicateCache()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    m_qsoCache.clear();
   //qDebug() << Q_FUNC_INFO << " - END - " << QString("End - Cached %1 entries").arg(m_qsoCache.count()) ;
}

bool DataProxy_SQLite::beginTransaction()
{
    prepareStaticQueries();
    //QSqlDatabase::database().transaction();
    return db->beginTransaction();
}

bool DataProxy_SQLite::commitTransaction()
{
    return db->commitTransaction();
}

bool DataProxy_SQLite::rollbackTransaction()
{
    return db->rollbackTransaction();
}

void DataProxy_SQLite::slotCaptureDebugLogs(const QString &_func, const QString &_msg, DebugLogLevel _l)
{
    logEvent(_func, _msg, _l);
}
