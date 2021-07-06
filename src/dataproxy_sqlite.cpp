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
#include "utilities.h"
//#include <QDebug>

DataProxy_SQLite::DataProxy_SQLite(const QString &_parentFunction, const QString &_softVersion)
{
      //qDebug() << "DataProxy_SQLite::DataProxy_SQLite" << _softVersion << _parentFunction << endl;

       //qDebug() << "DataProxy_SQLite::DataProxy_SQLite 1" << endl;
    util = new Utilities();
    util->setVersion(_softVersion);
    qso = new QSO;

    db = new DataBase(Q_FUNC_INFO, _softVersion, util->getKLogDBFile());
    dbCreated = db->createConnection(Q_FUNC_INFO);
    //dbCreated = db->createBandModeMaps();
       //qDebug() << "DataProxy_SQLite::DataProxy_SQLite - END" << endl;
    searching = false;
    executionN = 0;

    //preparedQuery = new QSqlQuery;
    //db = new DataBase(0);
    //DataProxy_SQLite = new DataProxy_SQLite();
      //qDebug() << "DataProxy_SQLite::DataProxy_SQLite  END" << endl;

    //connect(db, SIGNAL(debugLog(QString, QString, int)), this, SLOT(slotCaptureDebugLogs(QString, QString, int)) );

}
DataProxy_SQLite::~DataProxy_SQLite(){
         //qDebug() << "DataProxy_SQLite::~DataProxy_SQLite" << endl;

}

int DataProxy_SQLite::getHowManyQSOPerPropMode(const QString &_p, const int _logn)
{
    //qDebug() << "DataProxy_SQLite::getHowManyQSOPerPropMode: " << _p << "/" << QString::number(_logn) << endl;

    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    if (_logn < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log WHERE prop_mode='%1'").arg(_p);
    }
    else
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND prop_mode='%2'").arg(_logn).arg(_p);
    }

    sqlOK = query.exec(queryString);

    //qDebug() << "DataProxy_SQLite::getHowManyQSOPerPropMode: queryString: " << queryString << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            //qDebug() << "DataProxy_SQLite::getHowManyQSOPerPropMode: " << QString::number((query.value(0)).toInt()) << endl;
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::getHowManyQSOPerPropModer: 0" << endl;
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << "DataProxy_SQLite::getHowManyQSOPerPropMode: Query error" << endl;
        query.finish();
        return 0;
    }
}

QString DataProxy_SQLite::getSoftVersion()
{ //SELECT MAX (softversion) FROM softwarecontrol

    QSqlQuery query;
    QString stQuery = QString("SELECT MAX (softversion) FROM softwarecontrol");
    if (query.exec(stQuery))
    {
        query.next();
        if (query.isValid())
        {
            QString v = (query.value(0)).toString();
            query.finish();
               //qDebug() << "DataProxy_SQLite::getSoftVersion: DATA: " << v << endl;
            if (v.length()<1)
            {
                //The following is not a query error but if the softwareversion value is lower than 0 or empty
                queryError(Q_FUNC_INFO, tr("Software version in DB is null"), -1, tr("Query didn't failed")); // To alert about any failed query execution
            }
            return v;
        }
        else
        {
            query.finish();
               //qDebug() << "DataProxy_SQLite::getSoftVersion: version empty-1"  << endl;
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
           //qDebug() << "DataProxy_SQLite::getSoftVersion: version empty-1 - ERROR"  << endl;
        return QString();
    }
}

QString DataProxy_SQLite::getDBVersion()
{ //SELECT MAX (dbversion) FROM softwarecontrol

    QSqlQuery query;
    QString stQuery = QString("SELECT MAX (dbversion) FROM softwarecontrol");
    if (query.exec(stQuery))
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
}

bool DataProxy_SQLite::reconnectDB()
{
    return db->reConnect(util->getKLogDBFile());
}

void DataProxy_SQLite::createLogModel(){
         //qDebug() << "DataProxy_SQLite::createLogModel" << endl;
}

void DataProxy_SQLite::createLogPanel(){
         //qDebug() << "DataProxy_SQLite::createLogPanel" << endl;
}

int DataProxy_SQLite::getIdFromModeName(const QString& _modeName)
{
      //qDebug() << "DataProxy_SQLite::getIdFromModeName: " << _modeName << "/" << QString::number(db->getModeIDFromName2(_modeName)) << endl;
    if (_modeName.length()<2)
    {
        return -4;
    }
    return db->getModeIDFromName2(_modeName);

}

int DataProxy_SQLite::getSubModeIdFromSubMode(const QString &_subModeName)
{
      //qDebug() << "DataProxy_SQLite::getSubModeIdFromSubMode: " << _subModeName << endl;

    if (_subModeName.length()<2)
    {
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -2;
    }
}

int DataProxy_SQLite::getModeIdFromSubModeId(const int _sm)
{
    return getIdFromModeName(getNameFromSubMode(getSubModeFromId(_sm)));

}


bool DataProxy_SQLite::isModeDeprecated (const QString &_sm)
{
    if (_sm.length()<2)
    {
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
            return false; // In case we can't check, we don't state it as deprecated
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;   // In case we can't check, we don't state it as deprecated
    }
}

int DataProxy_SQLite::getIdFromBandName(const QString& _bandName)
{
    //qDebug() << "DataProxy_SQLite::getIdFromBandName: " << _bandName  << "/" << QString::number(db->getBandIDFromName2(_bandName))<< endl;
    if (_bandName.length()<1)
    {
            //qDebug() << "DataProxy_SQLite::getIdFromBandName:-4: " << _bandName  << "/" << QString::number(db->getBandIDFromName2(_bandName))<< endl;
        return -4;
    }

    return db->getBandIDFromName2(_bandName);
}

QString DataProxy_SQLite::getNameFromBandId (const int _id)
{
    //qDebug() << "DataProxy_SQLite::getNameFromBandId: " << QString::number(_id) << endl;
    return db->getBandNameFromID2(_id);
}

QString DataProxy_SQLite::getNameFromModeId (const int _id)
{
         //qDebug() << "DataProxy_SQLite::getNameFromModeId" << endl;
    //return db->getModeNameFromID2(_id);

    return db->getModeNameFromNumber(_id);
}

QString DataProxy_SQLite::getNameFromSubModeId (const int _id)
{
        //qDebug() << "DataProxy_SQLite::getNameFromSubModeId: " << QString::number(_id) << "DB: " << db->getModeNameFromID2(_id) << endl;
    return db->getSubModeNameFromID2(_id);

/*
    QSqlQuery query;
    QString queryString = QString("SELECT submode, name, deprecated FROM mode WHERE id='%1'").arg(_id);
    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(2)).toInt()<0 )
            { // DEPRECATED VALUE, return the MODE
                return (query.value(1)).toString();
            }
            else
            {
                return (query.value(0)).toString();
            }
        }
        else
        {
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }
*/
}

QString DataProxy_SQLite::getSubModeFromId (const int _id)
{
         //qDebug() << "DataProxy_SQLite::getSubModeFromId: " << QString::number(_id) << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }


    //return QString();
}

QString DataProxy_SQLite::getNameFromSubMode (const QString &_sm)
{
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
            return v;

        }
        else
        {
            query.finish();
            return QString();
        }
        //query.finish();
        //return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
}

QString DataProxy_SQLite::getFreqFromBandId(const int _id)
{
    //qDebug() << "DataProxy_SQLite::getFreqFromBandId: " << QString::number(_id) << endl;
    return db->getFreqFromBandId(_id);
}

int DataProxy_SQLite::getBandIdFromFreq(const double _n)
{
          //qDebug() << "DataProxy_SQLite::getBandIdFromFreq: " << QString::number(_n) << endl;
    //Freq should be in MHz
     bool sqlOk = false;
    QString queryString = QString("SELECT id FROM band WHERE lower <= '%1' and upper >= '%2'").arg(_n).arg(_n);

    QSqlQuery query(queryString);
    sqlOk = query.exec();

         //qDebug() << "DataProxy_SQLite::getBandIdFromFreq: Query: " << query.lastQuery() << endl;
    if (sqlOk)
    {
             //qDebug() << "DataProxy_SQLite::getBandIdFromFreq: Query OK" << endl;
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
             //qDebug() << "DataProxy_SQLite::getBandIdFromFreq: Query NOK" << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -2;
    }
    //return -3;
}

QString DataProxy_SQLite::getBandNameFromFreq(const double _n)
{
       //qDebug() << "DataProxy_SQLite::getBandNameFromFreq: " << QString::number(_n) << endl;
    return getNameFromBandId(getBandIdFromFreq(_n));
}

double DataProxy_SQLite::getLowLimitBandFromBandName(const QString &_sm)
{
         //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: " << _sm << endl;
    if (_sm.length ()<2)
    {
        return -1.0;
    }
    QSqlQuery query;
    QString queryString = QString("SELECT lower FROM band WHERE name='%1' OR name='%2'").arg(_sm).arg(_sm.toUpper());
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(0)).toDouble()<0 )
            {
                     //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: -1.0-1" << endl;
                query.finish();
                return -1.0;
            }
            else
            {
                     //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName(else): " << QString::number((query.value(0)).toDouble()) << endl;
                double v = (query.value(0)).toDouble();
                query.finish();
                return v;
            }
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: -1.0-2" << endl;
            query.finish();
            return -1.0;
        }

             //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: -1.0-3" << endl;
        //query.finish();
        //return -1.0;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1.0;
    }
    //return -1.0;
}


double DataProxy_SQLite::getLowLimitBandFromBandId(const int _sm)
{
         //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId" << endl;

    QSqlQuery query;
    QString queryString = QString("SELECT lower FROM band WHERE id='%1'").arg(_sm);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(1)).toDouble()<0 )
            {
                     //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId: -1.0-1" << endl;
                query.finish();
                return -1.0;
            }
            else
            {
                     //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId: " << QString::number((query.value(0)).toDouble()) << endl;
                double v = (query.value(0)).toDouble();
                query.finish();
                return v;
            }
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId: -1.0-2" << endl;
            query.finish();
            return -1.0;
        }


        //query.finish();
        //return -1.0;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1.0;
    }
    //return -1.0;

}

double DataProxy_SQLite::getUpperLimitBandFromBandName(const QString &_sm)
{
    //qDebug() << Q_FUNC_INFO << ": " << _sm << endl;
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
                //qDebug() << Q_FUNC_INFO << ": -1.0-1" << endl;
                query.finish();
                return -1.0;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": (else): " << QString::number((query.value(0)).toDouble()) << endl;
                double v = (query.value(0)).toDouble();
                query.finish();
                return v;
            }
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << ": -1.0-2" << endl;
            query.finish();
            return -1.0;
        }

        //qDebug() << Q_FUNC_INFO << ": -1.0-3" << endl;
        //query.finish();
        //return -1.0;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1.0;
    }
    //return -1.0;
}

bool DataProxy_SQLite::isThisFreqInBand(const QString &_band, const QString &_fr)
{
    return db->isThisFreqInBand(_band, _fr);
}

QStringList DataProxy_SQLite::getBands()
{
       //qDebug() << "DataProxy_SQLite::getBands - DEPRECATED please use getBandNames - TODO: Remove this function and change the calls" << endl;
    return getBandNames();


/*
   QStringList bands = QStringList();
    QSqlQuery query("SELECT name FROM band");
    while (query.next()) {
        if (query.isValid()){
            bands << query.value(0).toString();
        }
    }
    //return bands;
    return sortBandNamesBottonUp(bands);
*/
}

QStringList DataProxy_SQLite::getBandNames()
{
        //qDebug() << "DataProxy_SQLite::getBandNames" << endl;
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
                     //qDebug() << "DataProxy_SQLite::getBandNames: " << queryString << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
      //qDebug() << "DataProxy_SQLite::sortBandNamesBottonUp: " << QString::number(_qs.length()) << endl;
    if (_qs.length()<2)
    {
        return _qs;
    }

    QMap<double, QString> map;
    map.clear();
    QStringList qs;
    qs.clear();
      //qDebug() << "DataProxy_SQLite::sortBandNamesBottonUp-00" << endl;

    for (int j=0; j<_qs.count(); j++)
    {
        map.insert(getLowLimitBandFromBandName(_qs.at(j)), _qs.at(j));
    }
      //qDebug() << "DataProxy_SQLite::sortBandNamesBottonUp-10" << endl;
    QMap<double, QString>::const_iterator i = map.constBegin();
      //qDebug() << "DataProxy_SQLite::sortBandNamesBottonUp-20" << endl;
    while (i != map.constEnd()) {
        qs << i.value();
        ++i;
    }

      //qDebug() << "DataProxy_SQLite::sortBandNamesBottonUp - END -lengh = " << QString::number(qs.length()) << endl;
    qs.removeDuplicates();
    return qs;
}


QStringList DataProxy_SQLite::getBandIDs()
{
         //qDebug() << "DataProxy_SQLite::getBandIDs" << endl;
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
         //qDebug() << "DataProxy_SQLite::getBandsInLog: " << endl;
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
       //qDebug() << "DataProxy_SQLite::getModesInLog: " << endl;
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
        //qDebug() << "DataProxy_SQLite::getModesInLog: " << modes.join(" - ") << endl;
    modes.sort();
    return modes;
}

int DataProxy_SQLite::getMostUsedBand(const int _log)
{
       //qDebug() << "DataProxy_SQLite::getMostUsedBand: " << endl;

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
                   //qDebug() << "DataProxy_SQLite::getMostUsedBand-OK: " << QString::number(v)<< endl;
                return v;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
           //qDebug() << "DataProxy_SQLite::getMostUsedBand-ERROR-1: " << endl;
        return -1;
    }
       //qDebug() << "DataProxy_SQLite::getMostUsedBand-ERROR-2: " << endl;
    return -2;
}

int DataProxy_SQLite::getMostUsedMode(const int _log)
{
      //qDebug() << "DataProxy_SQLite::getMostUsedMode: " << endl;

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1;
    }
    return -2;

}


int DataProxy_SQLite::getLastQSOid()
{
         //qDebug() << "DataProxy_SQLite::getLastQSOid" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1;
    }

}

QDate DataProxy_SQLite::getFirstQSODateFromCall (const QString &_call)
{
    //qDebug() << "DataProxy_SQLite::getFirstQSODateFromCall: " << _call << endl;

    QSqlQuery query;
    QString stringQuery;
    QDate _date;
    if (util->isValidCall(_call))
    {
        stringQuery = QString("SELECT qso_date from log where station_callsign='%1' ORDER BY qso_date ASC LIMIT 1").arg(_call);
    }
    else
    {
        stringQuery = QString("SELECT qso_date from log where 1 ORDER BY qso_date ASC LIMIT 1");
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
                //qDebug() << "DataProxy_SQLite::getFirstQSODateFromCall: END OK"  << endl;
                return _date;
            }
            else
            {
                //qDebug() << "DataProxy_SQLite::getFirstQSODateFromCall: END-1 "  << endl;
                return QDate();
            }

        }
        else
        {
            query.finish();
            //qDebug() << "DataProxy_SQLite::getFirstQSODateFromCall: END-2"  << endl;
            return QDate();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        //qDebug() << "DataProxy_SQLite::getFirstQSODateFromCall: END-3"  << endl;
        return QDate();
    }
}

QDate DataProxy_SQLite::getLastQSODateFromCall (const QString &_call)
{
    //qDebug() << "DataProxy_SQLite::getLastQSODateFromCall: " << _call << endl;
    QSqlQuery query;
    QString stringQuery;
    QDate _date;
    if (util->isValidCall(_call))
    {
        stringQuery = QString("SELECT qso_date from log where station_callsign='%1' ORDER BY qso_date DESC LIMIT 1").arg(_call);
    }
    else
    {
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
                //qDebug() << "DataProxy_SQLite::getLastQSODateFromCall: OK" << endl;
                return _date;
            }
            else
            {
                //qDebug() << "DataProxy_SQLite::getLastQSODateFromCall: END-1" << endl;
                return QDate();
            }

        }
        else
        {
            query.finish();
            //qDebug() << "DataProxy_SQLite::getLastQSODateFromCall: END-2" << endl;
            return QDate();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        //qDebug() << "DataProxy_SQLite::getLastQSODateFromCall: END-3" << endl;
        return QDate();
    }
}

bool DataProxy_SQLite::clearLog()
{
         //qDebug() << "DataProxy_SQLite::clearLog" << endl;
    //int errorCode = 0;
    QSqlQuery query;
    bool sqlOK = query.exec("DELETE FROM log");

    if (sqlOK)
    {
             //qDebug() << "DataProxy_SQLite::clearLog: Log deleted!" << endl;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::clearLog: Log deleted FAILED" << endl;
        //errorCode = query.lastError().number();

             //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }
    query.finish();
    sqlOK = query.exec("DELETE FROM awarddxcc");


    if (sqlOK)
    {
             //qDebug() << "DataProxy_SQLite::clearLog: Awarddxcc deleted!" << endl;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::clearLog: Awarddxcc deletedFAILED" << endl;

             //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(query.lastError().number()) << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }
    query.finish();
    if (query.exec("DELETE FROM awardwaz"))
    {
             //qDebug() << "DataProxy_SQLite::clearLog: Awardwaz deleted!" << endl;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::clearLog: Awardwaz deleted FAILED" << endl;
        //errorCode = query.lastError().number();
             //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(query.lastError().number()) << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
             //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }

    query.finish();
    //query.clear();

    if (query.isActive())
    {
             //qDebug() << "DataProxy_SQLite::clearLog: Query Active!" << endl;
        query.finish();
        return false;
    }
    else
    {
        query.prepare("VACUUM;");

             //qDebug() << "DataProxy_SQLite::clearLog: Query Not Active!" << endl;
        if (query.exec())
        {
                 //qDebug() << "DataProxy_SQLite::clearLog: VACUUM OK!" << endl;
            query.finish();
            return true;
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                 //qDebug() << "DataProxy_SQLite::clearLog: VACUUM FAILED" << endl;
            //errorCode = query.lastError().number();
                 //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(query.lastError().number()) << endl;
                 //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
                 //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
                 //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
                 //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
        }
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslSentViaDirect(const int _qsoId, const QDate &_updateDate)
{
         //qDebug() << "DataProxy_SQLite::qslSentViaDirect" << endl;
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'D', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << "DataProxy_SQLite::qslSentViaDirect: " << queryString << endl;
    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    return false;
}

bool DataProxy_SQLite::qslSentViaBureau(const int _qsoId, const QDate &_updateDate)
{
    //qDebug() << "DataProxy_SQLite::qslSentViaBureau" << endl;

    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'B', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);

    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
        //qDebug() << "DataProxy_SQLite::qslSentViaBureau - OK" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << "DataProxy_SQLite::qslSentViaBureau - NOK" << endl;
    }

    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QDate &_updateDate)
{
         //qDebug() << "DataProxy_SQLite::" << QString::number (_qsoId) << "/" << _updateDate << endl;
    QSqlQuery query;
    QString queryString;


    queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
    bool sqlOK = query.exec(queryString);
    query.finish();
    if (sqlOK)
    {
             //qDebug() << "DataProxy_SQLite:: TRUE" << endl;
        //setDXCCAwardStatus(_qsoId);
        //setWAZAwardStatus(_qsoId);
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

         //qDebug() << "DataProxy_SQLite:: FALSE" << endl;
    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QDate &_updateDate, const bool _queueSentQSL)
{
         //qDebug() << "DataProxy_SQLite::qslRecViaBureau: " << _updateDate << endl;
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
                          //qDebug() << "DataProxy_SQLite::qslRecViaBureau: QSL already requested" << endl;
                     //requestQSL = false;
                     queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
                else
                {
                         //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-1" << endl;
                    //requestQSL = true;
                    queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
            }
            else
            {
                     //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-2" << endl;
                queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                //requestQSL = true;
            }
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-3" << endl;
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
             //qDebug() << "DataProxy_SQLite::qslRecViaBureau TRUE" << endl;
        query.finish();
        //setDXCCAwardStatus(_qsoId);
        //setWAZAwardStatus(_qsoId);

        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

         //qDebug() << "DataProxy_SQLite::qslRecViaBureau FALSE" << endl;
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QDate &_updateDate)
{
         //qDebug() << "DataProxy_SQLite::qslRecViaDirect" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QDate &_updateDate, const bool _queueSentQSL)
{
         //qDebug() << "DataProxy_SQLite::qslRecViaDirect: " << _updateDate << endl;
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
                          //qDebug() << "DataProxy_SQLite::qslRecViaDirect: QSL already requested" << endl;
                     queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
                else
                {
                         //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-1" << endl;
                    queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
                }
            }
            else
            {
                     //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-2" << endl;
                queryString = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                 //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-3" << endl;
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
             //qDebug() << "DataProxy_SQLite::qslRecViaDirect TRUE" << endl;
        query.finish();
        //setDXCCAwardStatus(_qsoId);
        //setWAZAwardStatus(_qsoId);
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

         //qDebug() << "DataProxy_SQLite::qslRecViaDirect FALSE" << endl;
    query.finish();
    return false;
}


bool DataProxy_SQLite::qslSentAsRequested(const int _qsoId, const QDate &_updateDate)
{
    //TODO: Add some protection to the data before modifying
         //qDebug() << "DataProxy_SQLite::qslSentAsRequested" << endl;
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_sent = 'R', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << "DataProxy_SQLite::qslSentAsRequested: " << queryString << endl;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
             //qDebug() << "DataProxy_SQLite::qslSentAsRequested" << endl;
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::qslRecAsRequested(const int _qsoId, const QDate &_updateDate)
{
//TODO: Add some protection to the data before modifying
    //qDebug() << "DataProxy_SQLite::qslRecAsRequested" << endl;
    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET qsl_rcvd = 'R', qslsdate = '%1' WHERE id = '%2'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << "DataProxy_SQLite::qslRecAsRequested: " << queryString << endl;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        //qDebug() << "DataProxy_SQLite::qslRecAsRequested OK" << endl;
        query.finish();
        return true;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::qslRecAsRequested NOK" << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.finish();
    return false;
}

bool DataProxy_SQLite::setClubLogSent(const int _qsoId, const QString &_st, const QDate &_updateDate)
{ // Updates the QSO with the ClubLog status & date

    QSqlQuery query;
    QString queryString;
    queryString = QString("UPDATE log SET clublog_qso_upload_status = '%1', clublog_qso_upload_date = '%2' WHERE id = '%3'").arg(_st).arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_qsoId);
         //qDebug() << "DataProxy_SQLite::setClubLogSent: " << queryString << endl;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
             //qDebug() << "DataProxy_SQLite::setClubLogSent - TRUE" << endl;
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
         //qDebug() << "DataProxy_SQLite::setClubLogSent - FALSE" << endl;
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
         //qDebug() << "DataProxy_SQLite::setLoTWQSLRec: " << queryString << endl;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        //qDebug() << "DataProxy_SQLite::setLoTWQSLRec - TRUE" << endl;
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
         //qDebug() << "DataProxy_SQLite::setLoTWQSLRec - FALSE" << endl;
    query.finish();
    return false;
}

bool DataProxy_SQLite::isQSLReceived(const int _qsoId)
{

         //qDebug() << "DataProxy_SQLite::isQSLReceived" << QString::number(_qsoId) << endl;
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
                     //qDebug() << "DataProxy_SQLitew::isQSLReceived: " << QString::number(_qsoId) << "QSL Received" << endl;
                return true;
            }
            else
            {
                     //qDebug() << "DataProxy_SQLite::isQSLReceived: " << QString::number(_qsoId) << "QSL NOT Received-1" << endl;
                return false;
            }
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::isQSLReceived: " << QString::number(_qsoId) << "QSL NOT Received-2" << endl;
            query.finish();
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
    }
    return false;

}

bool DataProxy_SQLite::isQSLSent(const int _qsoId)
{

         //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << endl;
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
                     //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << "QSL Sent" << endl;
                return true;
            }
            else
            {
                     //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << "QSL NOT Sent-1" << endl;
                return false;
            }
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << "QSL NOT Sent-2" << endl;
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
    }
    return -1;
}

QString DataProxy_SQLite::getCallFromId(const int _qsoId)
{
         //qDebug() << "DataProxy_SQLite::getCallFromId" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
    }
    return QString();
}


QStringList DataProxy_SQLite::getClubLogRealTimeFromId(const int _qsoId)
{
    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: " << QString::number(_qsoId) << endl;
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
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: sqlOK -1" << endl;
        if (query.next())
        {
            //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: qsl next -1" << endl;
            if (query.isValid())
            {
                //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: sql valid -1" << endl;
                QSqlRecord rec = query.record();
                nameCol = rec.indexOf("band_rx");
                QString aux = (query.value(nameCol)).toString();
                if (aux.length()<1)
                { // We DON'T have a band_rx available
                    query.finish();
                    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: without BAND-RX" << endl;
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
                //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: NO VALID" << endl;
                return QStringList();
            }
        }
        else
        {
            query.finish();
            //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-1" << endl;
            return QStringList();
        }
    }
    else
    {
        query.finish();
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-2" << endl;
        return QStringList();
    }


    sqlOk = query.exec(queryString);
    dataC << QString::number(_qsoId);
    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: lastQuery: " << query.lastQuery() << endl;

    if (sqlOk)
    {
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId sqlOK-1" << endl;
        if (query.next())
        {
            //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId query next = OK" << endl;
            if (query.isValid())
            {
                QSqlRecord rec = query.record();
                //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId query valid = OK" << endl;
                QString aux;
                nameCol = rec.indexOf("qso_date");
                aux = util->getADIFDateFromQDateTime(util->getDateTimeFromSQLiteString((query.value(nameCol)).toString()));
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-3: " << aux << endl;
                    return QStringList();
                }
                dataC << aux;
                aux = util->getADIFTimeFromQDateTime(util->getDateTimeFromSQLiteString((query.value(nameCol)).toString()));
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-4: " << aux<< endl;
                    return QStringList();
                }
               //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId Time_on: " << aux << endl;
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
                    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-5: " << aux << endl;
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
                    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-6: " << aux<< endl;
                    return QStringList();
                }
                dataC << aux;

                //nameCol = rec.indexOf("B.name"); // BAND
                aux = (query.value(7)).toString();
                if (aux.length()<1)
                {
                    query.finish();
                    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-7: " << aux << endl;
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

                if (util->isValidCall(aux2))
                {
                    dataC <<  aux2;
                }
                else
                {
                    dataC << call;
                }

                //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: RETURNING ... OK" << endl;
                query.finish();
                return dataC;
            }
            else
            {
                //NO VALID
                //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NO VALID NOT OK" << endl;
                query.finish();
                return QStringList();
            }
        }
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NOT sqlOK" << endl;
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: 2 LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: 2 LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: 2 LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: 2 LastError-n: " << QString::number(query.lastError().number() ) << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QStringList();
    }
    //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: ERROR-10" << endl;
    return QStringList();
}

QString DataProxy_SQLite::getNameFromQRZ(const QString &_call)
{
    if (_call.length() <= 0)
    {
        //qDebug() << "DataProxy_SQLite::getNameFromQRZ return 0" << endl;
        return QString();
    }

    QSqlQuery query;
    QString queryString = QString("SELECT name FROM log WHERE call='%0'").arg(_call);

    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                if (((query.value(0)).toString()).length()>0)
                {
                    //qDebug() << "DataProxy_SQLite::getNameFromQRZ: " <<  (query.value(0)).toString() << endl;
                    QString v = (query.value(0)).toString();
                    query.finish();
                    return v;
                }
            }
        }
        query.finish();
        //qDebug() << "DataProxy_SQLite::getNameFromQRZ: NO NAME"  << endl;
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
}

QString DataProxy_SQLite::getQTHFromQRZ(const QString &_call)
{
    if (_call.length() <= 0)
    {
        return QString();
    }
    QSqlQuery query;
    QString queryString = QString("SELECT qth FROM log WHERE call='%0'").arg(_call);

    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                if (((query.value(0)).toString()).length()>0)
                {
                    QString v = (query.value(0)).toString();
                    query.finish();
                    return v;
                }
            }
        }
        query.finish();
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }

}

QString DataProxy_SQLite::getLocatorFromQRZ(const QString &_call)
{
    if (_call.length() <= 0)
    {
        return QString();
    }
    QSqlQuery query;
    QString queryString = QString("SELECT gridsquare FROM log WHERE call='%0'").arg(_call);

    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                if (((query.value(0)).toString()).length()>0)
                {
                    QString v = (query.value(0)).toString();
                    query.finish();
                    return v;
                }
            }
        }
        query.finish();
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }

}


QString DataProxy_SQLite::getIOTAFromQRZ(const QString &_call)
{
    if (_call.length() <= 0)
    {
        return QString();
    }
    QSqlQuery query;
    QString queryString = QString("SELECT iota FROM log WHERE call='%0'").arg(_call);

    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                if (((query.value(0)).toString()).length()>0)
                {
                    QString v = (query.value(0)).toString();
                    query.finish();
                    return v;
                }
            }
        }
        query.finish();
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
}


QString DataProxy_SQLite::getQSLViaFromQRZ(const QString &_call)
{
    if (_call.length() <= 0)
    {
        return QString();
    }
    QSqlQuery query;
    QString queryString = QString("SELECT DISTINCT qsl_via FROM log WHERE call='%0'").arg(_call);

    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                if (((query.value(0)).toString()).length()>0)
                {
                    QString v = (query.value(0)).toString();
                    query.finish();
                    return v;
                }
            }
        }
        query.finish();
        return QString();;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
}

bool DataProxy_SQLite::updateAwardDXCC()
{
       //qDebug() << "DataProxy_SQLite::updateAwardDXCC" << endl;
    fillEmptyDXCCInTheLog();
    return db->updateAwardDXCCTable();
       //qDebug() << "DataProxy_SQLite::updateAwardDXCC-END" << endl;
}

bool DataProxy_SQLite::updateAwardWAZ()
{
       //qDebug() << "DataProxy_SQLite::updateAwardWAZ" << endl;
    return db->updateAwardWAZTable();
}

bool DataProxy_SQLite::QRZCOMModifyFullLog(const int _currentLog)
{
    //qDebug() << " DataProxy_SQLite::QRZCOMModifyFullLog" << QString::number(_currentLog) << endl;

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
       //qDebug() << " DataProxy_SQLite::QRZCOMModifyFullLog: END TRUE" << endl;
       return true;
   }
   else
   {
       emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
       //qDebug() << " DataProxy_SQLite::QRZCOMModifyFullLog: END FALSE: " << query.lastError().databaseText()  << endl;
       return false;
   }

    //qDebug() << " DataProxy_SQLite::QRZCOMModifyFullLog: END TRUE"  << endl;
   //return false;
}

bool DataProxy_SQLite::QRZCOMSentQSOs(const QList<int> &_qsos)
{
    //qDebug() << " DataProxy_SQLite::QRZCOMSentQSOs" << QString::number(_qsos.count()) << endl;
    if (_qsos.count() < 1)
    {
        return true;
    }
    QString queryString;
    bool sqlOK;
    QSqlQuery query;

    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << " DataProxy_SQLite::QRZCOMLSentQSOs: updating QSO: " << QString::number(_qsos.at(i)) << endl;


         queryString = QString("UPDATE log SET qrzcom_qso_upload_status = 'Y', qrzcom_qso_upload_date = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
         sqlOK = query.exec(queryString);
         query.finish();
         if (sqlOK)
         {
              //qDebug() << " DataProxy_SQLite::QRZCOMSentQSOs: exec: " << query.lastQuery() << endl;
         }
         else
         {
             emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
               //qDebug() << " DataProxy_SQLite::QRZCOMSentQSOs: END FALSE"  << endl;
             return false;
         }
    }
      //qDebug() << " DataProxy_SQLite::QRZCOMSentQSOs: END TRUE"  << endl;
    return true;
}


bool DataProxy_SQLite::clublogSentQSOs(const QList<int> &_qsos)
{
    //qDebug() << " DataProxy_SQLite::clublogSentQSOs" << QString::number(_qsos.count()) << endl;
    if (_qsos.count() < 1)
    {
        return true;
    }
    QString queryString;
    bool sqlOK;
    QSqlQuery query;

    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << " DataProxy_SQLite::lotwSentQSOs: updating QSO: " << QString::number(_qsos.at(i)) << endl;


         queryString = QString("UPDATE log SET clublog_qso_upload_status = 'Y', clublog_qso_upload_date = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
         sqlOK = query.exec(queryString);
         query.finish();
         if (sqlOK)
         {
              //qDebug() << " DataProxy_SQLite::clublogSentQSOs: exec: " << query.lastQuery() << endl;
         }
         else
         {
             emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
               //qDebug() << " DataProxy_SQLite::clublogSentQSOs: END FALSE"  << endl;
             return false;
         }
    }
      //qDebug() << " DataProxy_SQLite::clublogSentQSOs: END TRUE"  << endl;
    return true;
}

bool DataProxy_SQLite::clublogModifyFullLog(const int _currentLog)
 {
     //qDebug() << " DataProxy_SQLite::clublogModifyFullLog" << QString::number(_currentLog) << endl;

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << " DataProxy_SQLite::clublogModifyFullLog: END FALSE"  << endl;
        return false;
    }

       //qDebug() << " DataProxy_SQLite::clublogModifyFullLog: END TRUE"  << endl;
    //return false;
 }

 bool DataProxy_SQLite::eQSLModifyFullLog(const int _currentLog)
 {
     //qDebug() << " DataProxy_SQLite::eQSLModifyFullLog" << QString::number(_currentLog) << endl;

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
        //qDebug() << " DataProxy_SQLite::eQSLModifyFullLog: END TRUE" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << " DataProxy_SQLite::eQSLModifyFullLog: END FALSE: " << query.lastError().databaseText()  << endl;
        return false;
    }

       //qDebug() << " DataProxy_SQLite::eQSLModifyFullLog: END TRUE"  << endl;

 }

 bool DataProxy_SQLite::eQSLSentQSOs(const QList<int> &_qsos)
 {
     //qDebug() << " DataProxy_SQLite::eQSLSentQSOs" << QString::number(_qsos.count()) << endl;
     if (_qsos.count() < 1)
     {
         return true;
     }
     QString queryString;
     bool sqlOK;
     QSqlQuery query;

     for (int i = 0; i< _qsos.count(); i++)
     {
           //qDebug() << " DataProxy_SQLite::eQSLSentQSOs: updating QSO: " << QString::number(_qsos.at(i)) << endl;


          queryString = QString("UPDATE log SET eqsl_qsl_sent = 'Y', eqsl_qslsdate = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
          sqlOK = query.exec(queryString);
          query.finish();
          if (sqlOK)
          {
               //qDebug() << " DataProxy_SQLite::eQSLSentQSOs: exec: " << query.lastQuery() << endl;
          }
          else
          {
              emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                //qDebug() << " DataProxy_SQLite::eQSLSentQSOs: END FALSE"  << endl;
              return false;
          }
     }
       //qDebug() << " DataProxy_SQLite::eQSLSentQSOs: END TRUE"  << endl;
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
    //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: " << _dxcall << endl;

    //void MainWindow::slotWSJTXloggedQSO(const int _type, const QString &_dxcall, const quint64 _freq, const QString &_mode,
    //                                              const QString &_dx_grid, const QString &_time_off, const QString &_report_sent, const QString &_report_rec,
    //                                              const QString &_tx_power, const QString &_comments, const QString &_name, const QString &_time_on)

    //_qso format: Date/TimeOn/call/bandid/modeid/freq/dxgrid/timeOff/rsttx/rstrx/txpower/comments/name
/*
    Mandatory data:
             "qso_date VARCHAR(10) NOT NULL, "
             "call VARCHAR(40) NOT NULL, "
             "bandid INTEGER NOT NULL, "
             "modeid INTEGER NOT NULL, "
*/

    QString stringFields  = QString();
    QString stringData = QString();
    QString stringQuery = QString();

    QSqlQuery query;

    if (util->isValidCall(_dxcall))
    {
        stringFields   = "call, ";
        stringData =  "'" + _dxcall + "', ";

    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: Error: call" << endl;
        return false;
    }

    if (_datetime.isValid())
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: time-on: " <<  _datetime << endl;
        stringFields  = stringFields  + "qso_date, ";
        QDateTime _dateTime;
        //_dateTime.setDate(QDate::currentDate());
        //_dateTime.setTime(QTime::fromString(_time_on, "yyyyMMddhhmmss"));
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: time-on: " << _datetime << endl;
        //stringData =  stringData + "'" + QDateTime::fromString(_time_on, "yyyyMMddhhmmss").toString("yyyy-MM-dd") + "', '" + QDateTime::fromString(_time_on, "yyyyMMddhhmmss").toString("hh:mm:ss") + "', ";
        stringData =  stringData + "'" + util->getDateTimeSQLiteStringFromDateTime(_datetime) + "', ";
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: time-on: " << stringData << endl;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: Error: time-on_ " << _datetime << endl;
        return false;
    }

    QString band = getBandNameFromFreq(_freq);
    if (band != _band)
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: Error: FREQ / BAND inconsistency " << _band << "/" << QString::number(_freq) << endl;
    }

    if (band.length()>0)
    {
            stringFields  = stringFields  + "bandid, " ;
            stringData =  stringData + "'" + QString::number(getBandIdFromFreq(_freq)) + "', ";

            stringFields  = stringFields  + "freq, " ;
            stringData =  stringData + "'" + QString::number(_freq) + "', ";
            // EA5WA fix to add the freq RX into the log
            stringFields  = stringFields  + "freq_rx, " ;
            stringData =  stringData + "'" + QString::number(_freq) + "', ";
    }
    else
    {
           //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: Error: band" << endl;


        emit queryError(Q_FUNC_INFO, "Incorrect band: " + _band, -1000 , "No query error");
        return false;
    }

    int _modeid = getSubModeIdFromSubMode(_mode);
       //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: mode: " << _mode << endl;
       //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: modeid: " << QString::number(_modeid) << endl;
    if (util->isValidModeId(_modeid))
    {
        stringFields  = stringFields  + "modeid, ";
        stringData =  stringData + "'" +  QString::number(_modeid) + "', ";
    }
    else
    {
           //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: Error: mode" << endl;
        emit queryError(Q_FUNC_INFO, "Incorrect mode: " + _mode, -1000, "No query error");
        return false;
    }

    if (_datetime_off.isValid())
    {
        stringFields  = stringFields  + "qso_date_off, ";
        stringData =  stringData + "'" + util->getDateTimeSQLiteStringFromDateTime(_datetime_off) + "', ";
    }

    if (util->isValidRST(_rstTX))
    {
        stringFields  = stringFields  + "rst_sent, ";
        stringData =  stringData + "'" + _rstTX + "', ";
    }

    if (util->isValidRST(_rstRX))
    {
        stringFields   = stringFields   + "rst_rcvd, ";
        stringData =  stringData + "'" + _rstRX + "', ";
    }

    if (_sTX.length()>0)
    {
        stringFields  = stringFields  + "stx_string, ";
        stringData =  stringData + "'" + _sTX + "', ";
    }

    if (_sRX.length()>0)
    {
        stringFields  = stringFields  + "srx_string, ";
        stringData =  stringData + "'" + _sRX + "', ";
    }

    if (_comment.length()>0)
    {
        stringFields   = stringFields   + "comment, ";
        stringData =  stringData + "'" + _comment + "', ";
    }

    if (_name.length()>0)
    {
        stringFields   = stringFields   + "name, ";
        stringData =  stringData + "'" + _name + "', ";
    }

    if (util->isValidGrid(_dxgrid))
    {
        stringFields   = stringFields   + "gridsquare, ";
        stringData =  stringData + "'" + _dxgrid + "', ";
    }

    if (util->isValidGrid(_mygrid))
    {
        stringFields   = stringFields   + "my_gridsquare, ";
        stringData =  stringData + "'" + _mygrid + "', ";
    }


    if (util->isValidPower(QString::number(txpower)))
    {
        stringFields  = stringFields  + "tx_pwr, ";
        stringData =  stringData + "'" + QString::number(txpower) + "', ";
    }

    if (util->isValidCall(_operator))
    {
        stringFields  = stringFields  + "operator, ";
        stringData =  stringData + "'" + _operator + "', ";
    }

    if (util->isValidCall(_stationcallsign))
    {
            stringFields  = stringFields  + "station_callsign, ";
            stringData =  stringData + "'" + _stationcallsign + "', ";
    }
    if (_dxcc>0)
    {
        stringFields  = stringFields  + "dxcc, ";
        stringData =  stringData + "'" + QString::number(_dxcc) + "', ";

        int _cqz, _ituz;
        _cqz = getCQzFromEntity(_dxcc);
        _ituz = getITUzFromEntity(_dxcc);
        if (_cqz >0)
        {
            stringFields  = stringFields  + "cqz, ";
            stringData =  stringData + "'" + QString::number(_cqz) + "', ";

        }
        if (_ituz >0)
        {
            stringFields  = stringFields  + "ituz, ";
            stringData =  stringData + "'" + QString::number(_ituz) + "', ";
        }
    }

    if (_sendQSL)
    {
    stringFields  = stringFields  + "qsl_via, ";
    stringData =  stringData + "'B', ";

    stringFields  = stringFields  + "lotw_qsl_sent, ";
    stringData =  stringData + "'Q', ";

    stringFields  = stringFields  + "eqsl_qsl_sent, ";
    stringData =  stringData + "'Q', ";

    stringFields  = stringFields  + "hrdlog_qso_upload_status, ";
    stringData =  stringData + "'M', ";

    stringFields  = stringFields  + "clublog_qso_upload_status, ";
    stringData =  stringData + "'M', ";

    stringFields  = stringFields  + "qrzcom_qso_upload_status, ";
    stringData =  stringData + "'M', ";
    }

    stringFields  = stringFields  + "lognumber";
    stringData =  stringData + "'" + QString::number(_logNumber) + "'";
/*
    if ( stringFields.endsWith(", ") )
    {
        stringFields.chop(2);
    }

    if ( stringData.endsWith(", ") )
    {
        stringData.chop(2);
    }
*/
    stringQuery = "INSERT INTO log (" + stringFields  + ") values (" + stringData +")" ;
    //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: Query: " << stringQuery << endl;

    bool sqlOK = query.exec(stringQuery);

    //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: LastQuery: " << query.lastQuery() << endl;

    if (sqlOK)
    {
        query.finish();
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: SQL OK" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        //qDebug() << "DataProxy_SQLite::addQSOFromWSJTX: Error: SQL " << endl;
        return false;
    }

}

int DataProxy_SQLite::addQSOFromLoTW(const QString &_call, const QDateTime _datetime, const QString &_mode, const QString &_band, const double _freq, const QDate _qslrdate, const QString &_stationcallsign, const int _logn)
{
    //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: " << _call << endl;


/*
    Mandatory data:
             "qso_date VARCHAR(10) NOT NULL, "
             "call VARCHAR(40) NOT NULL, "
             "bandid INTEGER NOT NULL, "
             "modeid INTEGER NOT NULL, "
             "lognumber"
*/

    QString stringFields  = QString();
    QString stringData = QString();
    QString stringQuery = QString();

    //getITUzFromEntity()

    QSqlQuery query;

    if (util->isValidCall(_call))
    {
        stringFields   = "call, ";
        stringData =  "'" + _call + "', ";

    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: Error: call" << endl;
        return -1;
    }

    if (_datetime.isValid())
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: time-on: " <<  _datetime << endl;
        stringFields  = stringFields  + "qso_date, ";
        QDateTime _dateTime;
        stringData =  stringData + "'" + util->getDateTimeSQLiteStringFromDateTime(_datetime) + "', ";
        //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: time-on: " << stringData << endl;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: Error: time-on_ " << _datetime << endl;
        return -2;
    }

    QString band = getBandNameFromFreq(_freq);
    if (band != _band)
    {
        //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: Error: FREQ / BAND inconsistency " << _band << "/" << QString::number(_freq) << endl;
    }

    if (band.length()>0)
    {
            stringFields  = stringFields  + "bandid, " ;
            stringData =  stringData + "'" + QString::number(getBandIdFromFreq(_freq)) + "', ";

            stringFields  = stringFields  + "freq, " ;
            stringData =  stringData + "'" + QString::number(_freq) + "', ";
    }
    else
    {
           //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: Error: band" << endl;


        emit queryError(Q_FUNC_INFO, "Incorrect band: " + _band, -1000 , "No query error");
        return -3;
    }

    int _modeid = getSubModeIdFromSubMode(_mode);
    if (util->isValidModeId(_modeid))
    {
        stringFields  = stringFields  + "modeid, ";
        stringData =  stringData + "'" +  QString::number(_modeid) + "', ";

        QString rst =  util->getDefaultRST(_mode);
        if (rst.length()>0)
        {
            stringFields  = stringFields  + "rst_sent, ";
            stringData =  stringData + "'" + rst + "', ";

            stringFields  = stringFields  + "rst_rcvd, ";
            stringData =  stringData + "'" +  rst + "', ";
        }
    }
    else
    {
           //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: Error: mode" << endl;
        emit queryError(Q_FUNC_INFO, "Incorrect mode: " + _mode, -1000, "No query error");
        return -4;
    }

    if (_qslrdate.isValid())
    {
        stringFields  = stringFields  + "lotw_qslrdate, ";
        stringData =  stringData + "'" +  util->getDateSQLiteStringFromDate(_qslrdate) + "', ";

        stringFields  = stringFields  + "lotw_qsl_rcvd, ";
        stringData =  stringData + "'Y', ";

        stringFields  = stringFields  + "lotw_qsl_sent, ";
        stringData =  stringData + "'Q', ";
    }


    if (util->isValidCall(_stationcallsign))
    {
            stringFields  = stringFields  + "station_callsign, ";
            stringData =  stringData + "'" + _stationcallsign + "', ";
    }
    int _dxcc = getDXCCFromPrefix(_call);
    if (_dxcc>0)
    {
        stringFields  = stringFields  + "dxcc, ";
        stringData =  stringData + "'" + QString::number(_dxcc) + "', ";

        int _cqz, _ituz;
        _cqz = getCQzFromEntity(_dxcc);
        _ituz = getITUzFromEntity(_dxcc);
        if (_cqz >0)
        {
            stringFields  = stringFields  + "cqz, ";
            stringData =  stringData + "'" + QString::number(_cqz) + "', ";

        }
        if (_ituz >0)
        {
            stringFields  = stringFields  + "ituz, ";
            stringData =  stringData + "'" + QString::number(_ituz) + "', ";

        }

    }

    stringFields  = stringFields  + "lognumber";
    stringData =  stringData + "'" + QString::number(_logn) + "'";

    stringQuery = "INSERT INTO log (" + stringFields  + ") values (" + stringData +")" ;
    //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: Query: " << stringQuery << endl;

    bool sqlOK = query.exec(stringQuery);

    //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: LastQuery: " << query.lastQuery() << endl;

    if (sqlOK)
    {
        //query.finish();
        if (query.exec("SELECT MAX(id) FROM log"))
        {
            if (query.next())
            {
                if (query.isValid())
                {
                    int v = (query.value(0)).toInt();
                    query.finish();
                    return v;
                }
                else
                {
                    //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: NOT VALID" << endl;
                }
            }
            else
            {
                //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: QUERY EXEC NOT NEXT" << endl;

            }
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: NOT OK: " << query.lastError().databaseText() << endl;
            //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: NOT OK: " << QString::number(query.lastError().number()) << endl;
        }
        //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: SQL OK" << endl;
        return -5;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        //qDebug() << "DataProxy_SQLite::addQSOFromLoTW: Error: SQL " << endl;
        return -100;
    }

}

bool DataProxy_SQLite::deleteQSO(const int _qsoId)
{
         //qDebug() << "DataProxy_SQLite::deleteQSO" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
}



int DataProxy_SQLite::isWorkedB4(const QString &_qrz, const int _currentLog)
{
         //qDebug() << "DataProxy_SQLite::isWorkedB4" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1;
    }

}

QList<int> DataProxy_SQLite::isThisQSODuplicated(const QString &_callingFunc, const QString &_qrz, const QDateTime &_dateTime, const int _band, const int _mode, const int _secs)
//QList<int> DataProxy_SQLite::isThisQSODuplicated(const QString &_qrz, const QDateTime &_dateTime, const int _band, const int _mode)
{
        //qDebug() << "DataProxy_SQLite::isThisQSODuplicated" << endl;
    QSqlQuery query;
    QString queryString;
    QList<int> dupeQsos;
    dupeQsos.clear();
    int validityPeriod = 15 * 60;
    QString initTime = util->getDateTimeSQLiteStringFromDateTime(_dateTime.addSecs(-validityPeriod));
    QString endTime = util->getDateTimeSQLiteStringFromDateTime(_dateTime.addSecs(validityPeriod));

    //yyyy-MM-dd hh:mm:ss
    // We will match +-15min


    queryString = QString("SELECT id, qso_date FROM log WHERE call='%1' AND bandid='%2' AND modeid='%3' AND qso_date>'%4' AND qso_date<'%5'").arg(_qrz).arg(_band).arg(_mode).arg(initTime).arg(endTime);

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
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
                   //qDebug() << "DataProxy_SQLite::isThisQSODuplicated - not valid"  << endl;
            }
        }
        return dupeQsos;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return dupeQsos;
    }
}


int DataProxy_SQLite::getDuplicatedQSOId(const QString &_qrz, const QDateTime &_datetime, const int _band, const int _mode)
{
         //qDebug() << "DataProxy_SQLite::isThisQSODuplicated" << endl;
     QSqlQuery query;
     QString queryString;
    QString datetime = util->getDateTimeSQLiteStringFromDateTime(_datetime);

     queryString = QString("SELECT id FROM log WHERE call='%1' AND qso_date='%2' AND bandid='%4' AND modeid='%5'").arg(_qrz).arg(datetime).arg(_band).arg(_mode);

     bool sqlOK = query.exec(queryString);

     if (sqlOK)
     {
         query.next();
         if (query.isValid())
         {
             int qsoId = (query.value(0)).toInt();
             if (qsoId)
             {
                 query.finish();
                 return qsoId;
             }
             else
             {
                 query.finish();
                 return -1;
             }
         }
         else
         {
             query.finish();
             return -1;
         }
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
         return -1;
     }

    //return -1;
}

bool DataProxy_SQLite::isDXCCConfirmed(const int _dxcc, const int _currentLog)
{
        //qDebug() << "DataProxy_SQLite::isDXCCConfirmed: " << QString::number(_dxcc) << "/" << QString::number(_currentLog) << endl;
    QString queryString = QString("SELECT confirmed from awarddxcc WHERE dxcc='%1' AND lognumber='%2'").arg(_dxcc).arg(_currentLog);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(0)).toInt() == 1)
            {
                    //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: TRUE" << endl;
                query.finish();
                return true;
            }
            else
            {
                    //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE1" << endl;
                query.finish();
                return false;
            }
        }
        else
        {
                //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE2" << endl;
            query.finish();
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE3" << endl;
        query.finish();
        return false;
    }
}

bool DataProxy_SQLite::isHF(const int _band)
{// 160M is considered as HF
    if ( (_band>=getIdFromBandName("10M")) && (_band<=getIdFromBandName("160M")) )
    {
            //qDebug() << "DataProxy_SQLite::isHF: TRUE" << endl;
        return true;
    }
    else
    {
            //qDebug() << "DataProxy_SQLite::isHF: FALSE" << endl;
        return false;
    }
}

bool DataProxy_SQLite::isWARC(const int _band)
{
    if ( (_band==getIdFromBandName("12M")) || (_band==getIdFromBandName("17M")) || ((_band==getIdFromBandName("30M")) ) )
    {
             //qDebug() << "DataProxy_SQLite::isWARC: tRUE" << endl;
        return true;
    }
    else
    {
             //qDebug() << "DataProxy_SQLite::isWARC: FALSE" << endl;
        return false;
    }
}

bool DataProxy_SQLite::isVHF(const int _band)
{
    if (_band<=getIdFromBandName("6M"))
    {
            //qDebug() << "DataProxy_SQLite::isVHF: TRUE" << endl;
        return true;
    }
    else
    {
            //qDebug() << "DataProxy_SQLite::isVHF: FALSE" << endl;
        return false;
    }
}

bool DataProxy_SQLite::isUHF(const int _band)
{
    if (_band<=getIdFromBandName("70CM"))
    {
            //qDebug() << "DataProxy_SQLite::isUHF: TRUE" << endl;
        return true;
    }
    else
    {
            //qDebug() << "DataProxy_SQLite::isUHF: FALSE" << endl;
        return false;
    }
}

QStringList DataProxy_SQLite::getOperatingYears(const int _currentLog)
{
       //qDebug() << "DataProxy_SQLite::getYearsOperating: " << QString::number(_currentLog) << endl;
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


    QString year = QString();
       //qDebug() << "DataProxy_SQLite::getYearsOperating: -1" << endl;
    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
           //qDebug() << "DataProxy_SQLite::getYearsOperating: sqlOk = true" << endl;
        while (query.next())
        {
            if (query.isValid())
            {
                year = (query.value(0)).toString();
                   //qDebug() << "DataProxy_SQLite::getYearsOperating: year=" << year << endl;
                years << year;
                year.clear();
            }
            else
            {
                   //qDebug() << "DataProxy_SQLite::getYearsOperating: NOT VALID"  << endl;
            }
        }
           //qDebug() << "DataProxy_SQLite::getYearsOperating: END OK - " << QString::number(years.size())<< endl;
        query.finish();
        //return years;
        if (years.length()>0)
        {
            years.sort();
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::getYearsOperating: sqlOk = false" << endl;
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

       //qDebug() << "DataProxy_SQLite::lotwSentQueue: Date:" << _updateDate << " /" << QString::number(_currentLog) << endl;
    QString queryString;

    if (_currentLog<1)
    {
        queryString = QString("UPDATE log SET lotw_qsl_sent = 'Q', lotw_qslsdate = '%1' WHERE lotw_qsl_sent != 'Y' AND lotw_qsl_sent != 'N' AND lotw_qsl_sent != 'R' AND lotw_qsl_sent != 'I' AND lotw_qsl_sent != 'Q'").arg(util->getDateSQLiteStringFromDate(_updateDate));
    }
    else
    {
        queryString = QString("UPDATE log SET lotw_qsl_sent = 'Q', lotw_qslsdate = '%1' WHERE lognumber = '%2' AND lotw_qsl_sent != 'Y' AND lotw_qsl_sent != 'N' AND lotw_qsl_sent != 'R' AND lotw_qsl_sent != 'I' AND lotw_qsl_sent != 'Q'").arg(util->getDateSQLiteStringFromDate(_updateDate)).arg(_currentLog);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::lotwSentYes(const QDate &_updateDate, const int _currentLog, const QString &_station)
{// Mark LOTW QSL SENT as Q (Queued)
    // If currentLog <0 ALL the QSO of the log will be queued

       //qDebug() << "DataProxy_SQLite::lotwSentQueue: " << QString::number(_currentLog) << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::lotwSentQSOs(const QList<int> &_qsos)
{
      //qDebug() << " DataProxy_SQLite::lotwSentQSOs" << QString::number(_qsos.count()) << endl;
    if (_qsos.count() < 1)
    {
        return true;
    }
    QString queryString;
    bool sqlOK;
    QSqlQuery query;

    for (int i = 0; i< _qsos.count(); i++)
    {
          //qDebug() << " DataProxy_SQLite::lotwSentQSOs: updating QSO: " << QString::number(_qsos.at(i)) << endl;

         //queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE id='%2'").arg((QDate::currentDate()).toString("yyyy-MM-dd")).arg(QString::number(_qsos.at(i)));
         queryString = QString("UPDATE log SET lotw_qsl_sent = 'Y', lotw_qslsdate = '%1' WHERE id='%2'").arg(util->getDateSQLiteStringFromDate(QDate::currentDate())).arg(QString::number(_qsos.at(i)));
         sqlOK = query.exec(queryString);
         query.finish();
         if (sqlOK)
         {
              //qDebug() << " DataProxy_SQLite::lotwSentQSOs: exec: " << query.lastQuery() << endl;
         }
         else
         {
             emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
               //qDebug() << " DataProxy_SQLite::lotwSentQSOs: END FALSE"  << endl;
             return false;
         }
    }
      //qDebug() << " DataProxy_SQLite::lotwSentQSOs: END TRUE"  << endl;
    return true;
}

int DataProxy_SQLite::lotwUpdateQSLReception (const QString &_call, const QDateTime &_dateTime, const QString &_band, const QString &_mode, const QDate &_qslrdate)
{ //Returns the QSO id updated or -1 if none was updated.
   //qDebug() << " DataProxy_SQLite::lotwUpdateQSLReception: " << _call << "/" << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << "/" <<_band <<"/"<<_mode << "/" << util->getADIFDateFromQDate(_qslrdate)  << endl ;
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
                   //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception: Modified Id: " << QString::number(id) << endl;
                    return id;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                   //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception: SQL ERROR" << endl;
                    return -4;
                }
            }
            else
            {
               //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception ID Not found" << endl;
                query.finish();
                return -5;
            }
        }
        else if (query.lastError().number() == -1)
        {
           //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception QSO not found " << query.lastQuery() << endl;
            return -1;
        }
        else
        {
           //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception Unknown error " << query.lastQuery() << endl;
           //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception Error: " << query.lastError().databaseText() << endl;
           //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception Error: " << QString::number(query.lastError().number()) << endl;
            query.finish();
            return -3;
        }
    }
    else
    {
       //qDebug() << "DataProxy_SQLite::lotwUpdateQSLReception Query error: " << query.lastQuery() << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.finish();
    return -100;
}

QList<int> DataProxy_SQLite::getQSOsListLoTWNotSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justQueued)
{
    //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd") << endl;

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string;
    if (util->isValidCall(_stationCallsign))
    {
        _queryST_string = QString("station_callsign='%1'").arg(_stationCallsign);
    }
    else if (_stationCallsign == "ALL")
    {
        _queryST_string = QString("station_callsign!='ALL'");
    }
    else
    {
        _queryST_string = QString("station_callsign=''");
    }

    QString _query_justQueued;
    if (_justQueued)
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent justQueued TRUE" << endl;
        _query_justQueued = QString("lotw_qsl_sent='Q'");
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent justQueued FALSE" << endl;
        _query_justQueued = QString("lotw_qsl_sent!='1'");
    }

    queryString = QString("SELECT id, qso_date FROM log WHERE ") + _queryST_string + " AND " + _query_justQueued;

    /*
    if (justQueued)
    {
        queryString = QString("SELECT id, qso_date FROM log WHERE station_callsign='%1' AND lotw_qsl_sent='Q'").arg(_stationCallsign);
    }
    else
    {
        queryString = QString("SELECT id, qso_date FROM log WHERE station_callsign='%1' ").arg(_stationCallsign);
    }
    */


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent Query: " << query.lastQuery() << endl;

    if (sqlOK)
    {
       // //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent Query: " << query.lastQuery() << endl;

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd") << endl;
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
            else
            {
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;

}


QStringList DataProxy_SQLite::getQSOsListLoTWNotSent2(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justQueued)
{
    //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent2 Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd") << endl;
    QStringList list;
    list.clear();




    QList <int> qsoList;
    qsoList.clear();
    //QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string;
    if (util->isValidCall(_stationCallsign))
    {
        _queryST_string = QString("station_callsign='%1'").arg(_stationCallsign);
    }
    else if (_stationCallsign == "ALL")
    {
        _queryST_string = QString("station_callsign!='ALL'");
    }
    else
    {
        _queryST_string = QString("station_callsign=''");
    }

    QString _query_justQueued;
    if (_justQueued)
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent justQueued TRUE" << endl;
        _query_justQueued = QString("lotw_qsl_sent='Q'");
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent justQueued FALSE" << endl;
        _query_justQueued = QString("lotw_qsl_sent!='1'");
    }

    queryString = QString("SELECT call, qso_date, bandid, modeid FROM log WHERE  ") + _queryST_string + " AND " + _query_justQueued;
    //queryString = QString("SELECT id, qso_date FROM log WHERE ") + _queryST_string + " AND " + _query_justQueued;


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent Query: " << query.lastQuery() << endl;

    if (sqlOK)
    {
       // //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent Query: " << query.lastQuery() << endl;
        QStringList result;
        while ( (query.next())) {
            if (query.isValid())
            {
                result.clear();

                QString call = query.value(0).toString();
                QString date = query.value(1).toString();
                //QString date = util->getDateTimeFromSQLiteString(query.value(1).toString());

                //QString time = query.value(2).toString();
                QString bandid = query.value(3).toString();
                QString modeid = query.value(4).toString();
                //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent2 - date: " << date << endl;
                //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent2 - time: " << time << endl;

                //getDateTimeSQLiteStringFromDateTime
                //QString dateTime = (QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss")).toString("yyyy-MM-dd hh:mm");
                QString dateTime = (util->getDateTimeFromSQLiteString(date)).toString("yyyy-MM-dd hh:mm");

                bandid = getNameFromBandId(bandid.toInt());
                modeid = getNameFromModeId(modeid.toInt());

                result.append(call);
                result.append(dateTime);
                result.append(bandid);
                result.append(modeid);
                list.append(result);



            }
            else
            {
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        list.clear();
        return list;
    }
    query.finish();
//  qs.sort();
    return list;

}

QList<int> DataProxy_SQLite::getQSOsListClubLogToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified)
{
    //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd") << endl;

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string;
    if (util->isValidCall(_stationCallsign))
    {
        _queryST_string = QString("station_callsign='%1'").arg(_stationCallsign);
    }
    else if (_stationCallsign == "ALL")
    {
        _queryST_string = QString("station_callsign!='ALL'");
    }
    else
    {
        _queryST_string = QString("station_callsign=''");
    }

    QString _query_justModified;
    if (_justModified)
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent justQueued TRUE" << endl;
        _query_justModified = QString("clublog_qso_upload_status='M'");
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent justQueued FALSE" << endl;
        _query_justModified = QString("clublog_qso_upload_status!='M'");
    }

    queryString = QString("SELECT id, qso_date FROM log WHERE ") + _queryST_string + " AND " + _query_justModified;


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent Query: " << query.lastQuery() << endl;

    if (sqlOK)
    {
       // //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent Query: " << query.lastQuery() << endl;

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd") << endl;
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent Adding: "  << QString::number((query.value(0)).toInt()) << endl;
                    qsoList.append((query.value(0)).toInt());
                }
            }
            else
            {
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    //qDebug() << "DataProxy_SQLite::getQSOsListClubLogToSent Returning: #"  << QString::number(qsoList.length()) << endl;
    return qsoList;
}

QList<int> DataProxy_SQLite::getQSOsListEQSLToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified)
{
    //qDebug() << "DataProxy_SQLite::getQSOsListEQSLToSent Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd") << endl;

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string;
    if (util->isValidCall(_stationCallsign))
    {
        _queryST_string = QString("station_callsign='%1'").arg(_stationCallsign);
    }
    else if (_stationCallsign == "ALL")
    {
        _queryST_string = QString("station_callsign!='ALL'");
    }
    else
    {
        _queryST_string = QString("station_callsign=''");
    }

    QString _query_justModified;
    if (_justModified)
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListEQSLToSent justQueued TRUE" << endl;
        _query_justModified = QString("eqsl_qsl_sent='Q'");
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListEQSLToSent justQueued FALSE" << endl;
        _query_justModified = QString("eqsl_qsl_sent!='M'");
    }

    queryString = QString("SELECT id, qso_date FROM log WHERE ") + _queryST_string + " AND " + _query_justModified;


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << "DataProxy_SQLite::getQSOsListEQSLToSent Query: " << query.lastQuery() << endl;

    if (sqlOK)
    {
       // //qDebug() << "DataProxy_SQLite::getQSOsListEQSLToSent Query: " << query.lastQuery() << endl;

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << "DataProxy_SQLite::getQSOsListEQSLToSent QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd") << endl;
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
            else
            {
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;
}

QList<int> DataProxy_SQLite::getQSOsListQRZCOMToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified)
{
    //qDebug() << "DataProxy_SQLite::QList<int> getQSOsListQRZCOMToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified=true); Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd") << endl;

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string;
    if (util->isValidCall(_stationCallsign))
    {
        _queryST_string = QString("station_callsign='%1'").arg(_stationCallsign);
    }
    else if (_stationCallsign == "ALL")
    {
        _queryST_string = QString("station_callsign!='ALL'");
    }
    else
    {
        _queryST_string = QString("station_callsign=''");
    }

    QString _query_justModified;
    if (_justModified)
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListQRZCOMToSent justQueued TRUE" << endl;
        _query_justModified = QString("qrzcom_qso_upload_status='M'");
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListQRZCOMToSent justQueued FALSE" << endl;
        _query_justModified = QString("qrzcom_qso_upload_status!='-'");
    }

    queryString = QString("SELECT id, qso_date FROM log WHERE ") + _queryST_string + " AND " + _query_justModified;


    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << "DataProxy_SQLite::getQSOsListQRZCOMToSent Query: " << query.lastQuery() << endl;

    if (sqlOK)
    {
       // //qDebug() << "DataProxy_SQLite::getQSOsListQRZCOMToSent Query: " << query.lastQuery() << endl;

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << "DataProxy_SQLite::getQSOsListQRZCOMToSent QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd") << endl;
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
            else
            {
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        qsoList.clear();
        return qsoList;
    }
    query.finish();
    qs.sort();
    return qsoList;
}


QList<int> DataProxy_SQLite::getQSOsListeQSLNotSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justQueued)
{
    //qDebug() << "DataProxy_SQLite::getQSOsListeQSLNotSent Call/Start/end: " << _stationCallsign << _startDate.toString("yyyyMMdd") << "/" << _endDate.toString("yyyyMMdd") << endl;

    QList <int> qsoList;
    qsoList.clear();
    QDate tmpDate;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString;

    QString _queryST_string;
    if (util->isValidCall(_stationCallsign))
    {
        _queryST_string = QString("station_callsign='%1'").arg(_stationCallsign);
    }
    else if (_stationCallsign == "ALL")
    {
        _queryST_string = QString("station_callsign!='ALL'");
    }
    else
    {
        _queryST_string = QString("station_callsign=''");
    }

    QString _query_justQueued;
    if (_justQueued)
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListeQSLNotSent justQueued TRUE" << endl;
        _query_justQueued = QString("eqsl_qsl_sent='Q'");
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getQSOsListeQSLNotSent justQueued FALSE" << endl;
        _query_justQueued = QString("eqsl_qsl_sent!='1'");
    }

    queryString = QString("SELECT id, qso_date FROM log WHERE ") + _queryST_string + " AND " + _query_justQueued;

    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << "DataProxy_SQLite::getQSOsListeQSLNotSent Query: " << query.lastQuery() << endl;

    if (sqlOK)
    {
       // //qDebug() << "DataProxy_SQLite::getQSOsListeQSLNotSent Query: " << query.lastQuery() << endl;

        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(1)).toString() ;
                tmpDate = util->getDateFromSQliteString(aux);
                //qDebug() << "DataProxy_SQLite::getQSOsListLoTWNotSent QSO Date: " << aux << "/" << tmpDate.toString("yyyy-MM-dd") << endl;
                //tmpDate = QDate::fromString(aux, "yyyy-MM-dd");
                if ((_startDate<=tmpDate) && _endDate>=tmpDate)
                {
                    qsoList.append((query.value(0)).toInt());
                }
            }
            else
            {
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
    //qDebug() << "DataProxy_SQLite::getQSODetailsForLoTWDownload" << QString::number(_id) << endl;
    QStringList result;
    result.clear();
    //getNameFromBandId
    QSqlQuery query;
    QString queryString = QString("SELECT call, qso_date, bandid, modeid FROM log WHERE id='%0'").arg(_id);

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

                //QString time = query.value(2).toString();
                QString bandid = query.value(2).toString();
                QString modeid = query.value(3).toString();
                query.finish();
                //qDebug() << "DataProxy_SQLite::getQSODetailsForLoTWDownload - date: " << date << endl;
                //qDebug() << "DataProxy_SQLite::getQSODetailsForLoTWDownload - time: " << time << endl;

                //getDateTimeSQLiteStringFromDateTime
                //QString dateTime = (QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss")).toString("yyyy-MM-dd hh:mm");
                QString dateTime = (util->getDateTimeFromSQLiteString(date)).toString("yyyy-MM-dd hh:mm");

                bandid = getNameFromBandId(bandid.toInt());
                modeid = getNameFromModeId(modeid.toInt());

                result.append(call);
                result.append(dateTime);
                result.append(bandid);
                result.append(modeid);
                //qDebug() << "DataProxy_SQLite::getQSODetailsForLoTWDownload - END: call: " << call << endl;
                return result;
            }
            //qDebug() << "DataProxy_SQLite::getQSODetailsForLoTWDownload - END: no valid "  << endl;
        }
        query.finish();
        //qDebug() << "DataProxy_SQLite::getQSODetailsForLoTWDownload - END: no next "  << endl;
        return result;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getQSODetailsForLoTWDownload - END: SQL NOK "  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return result;
    }
}

int DataProxy_SQLite::getQSOonYear(const int _year, const int _logNumber)
{
       //qDebug() << "DataProxy_SQLite::getQSOonYear: " << QString::number(_year) << "/" << QString::number(_logNumber) << endl;

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

         //qDebug() << "DataProxy_SQLite::getQSOonYear: queryString: " << queryString << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << "DataProxy_SQLite::getQSOonYear: " << QString::number((query.value(0)).toInt()) << endl;
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::getQSOonYear: 0" << endl;
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::getDXCConYear: Query error" << endl;
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getDXCConYear(const int _year, const int _logNumber)
{
         //qDebug() << "DataProxy_SQLite::getDXCConYear: " << QString::number(_year) << "/" << QString::number(_logNumber) << endl;

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

         //qDebug() << "DataProxy_SQLite::getDXCConYear: queryString: " << queryString << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << "DataProxy_SQLite::getDXCConYear: " << QString::number((query.value(0)).toInt()) << endl;
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::getDXCConYear: 0" << endl;
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::getDXCConYear: Query error" << endl;
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getCQzonYear(const int _year, const int _logNumber)
{
         //qDebug() << "DataProxy_SQLite::getCQzonYear: " << QString::number(_year) << endl;
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

         //qDebug() << "DataProxy_SQLite::getCQzonYear: queryString: " << queryString << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << "DataProxy_SQLite::getCQzonYear: " << QString::number((query.value(0)).toInt()) << endl;
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::getCQzonYear: 0" << endl;
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::getCQzonYear: Query error" << endl;
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getQSOsWithDXCC(const int _dxcc, const int _logNumber)
{

     //qDebug() << "DataProxy_SQLite::getQSOsWithDXCC: " << QString::number(_dxcc) << endl;
  QSqlQuery query;
  QString queryString;
  bool sqlOK;
  if (_logNumber < 0)
  {
      queryString = QString("SELECT COUNT (DISTINCT id) FROM log where dxcc LIKE '%1'").arg(_dxcc);
  }
  else
  {
      queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND dxcc LIKE '%2'").arg(_logNumber).arg(_dxcc);
  }


  sqlOK = query.exec(queryString);

       //qDebug() << "DataProxy_SQLite::getQSOsWithDXCC: queryString: " << queryString << endl;
  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
               //qDebug() << "DataProxy_SQLite::getQSOsWithDXCC: " << QString::number((query.value(0)).toInt()) << endl;
          int v = (query.value(0)).toInt();
          query.finish();
          return v;
      }
      else
      {
               //qDebug() << "DataProxy_SQLite::getQSOsWithDXCC: 0" << endl;
          query.finish();
          return 0;
      }

  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
           //qDebug() << "DataProxy_SQLite::getQSOsWithDXCC: Query error" << endl;
      query.finish();
      return 0;
  }
}

int DataProxy_SQLite::getQSOsAtHour(const int _hour, const int _log)
{
     //qDebug() << "DataProxy_SQLite::getQSOsAtHour: " << QString::number(_hour) << endl;
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

       //qDebug() << "DataProxy_SQLite::getQSOsAtHour: queryString: " << queryString << endl;
  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
               //qDebug() << "DataProxy_SQLite::getQSOsAtHour: " << QString::number((query.value(0)).toInt()) << endl;
          int v = (query.value(0)).toInt();
          query.finish();
          return v;
      }
      else
      {
               //qDebug() << "DataProxy_SQLite::getQSOsAtHour: 0" << endl;
          query.finish();
          return 0;
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
           //qDebug() << "DataProxy_SQLite::getQSOsAtHour: Query error" << endl;
      query.finish();
      return 0;
  }
}


int DataProxy_SQLite::getQSOsAtHourOnBand(const int _hour, const int _band, const int _log)
{
      //qDebug() << "DataProxy_SQLite::getQSOsAtHourOnBand: " << QString::number(_hour) << endl;
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
       queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE qso_date LIKE '% %1:%' AND bandid='%2'").arg(aux).arg(_band);
   }
   else
   {
       queryString = QString("SELECT COUNT(DISTINCT id) FROM log WHERE lognumber='%1' AND qso_date LIKE '% %2:%' AND bandid='%3'").arg(_log).arg(aux).arg(_band);
   }

   sqlOK = query.exec(queryString);

        //qDebug() << "DataProxy_SQLite::getQSOsAtHourOnBand: queryString: " << queryString << endl;
   if (sqlOK)
   {
       query.next();
       if (query.isValid())
       {
                //qDebug() << "DataProxy_SQLite::getQSOsAtHourOnBand: " << QString::number((query.value(0)).toInt()) << endl;
           int v = (query.value(0)).toInt();
           query.finish();
           return v;
       }
       else
       {
                //qDebug() << "DataProxy_SQLite::getQSOsAtHourOnBand: 0" << endl;
           query.finish();
           return 0;
       }
   }
   else
   {
       emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            //qDebug() << "DataProxy_SQLite::getQSOsAtHourOnBandç: Query error" << endl;
       query.finish();
       return 0;
   }
}

int DataProxy_SQLite::getQSOsOnMonth(const int _month, const int _log)
{
       //qDebug() << "DataProxy_SQLite::getgetQSOsOnMonth: " << QString::number(_month) << endl;
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

         //qDebug() << "DataProxy_SQLite::getQSOsAtHour: queryString: " << queryString << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
               //qDebug() << "DataProxy_SQLite::getQSOsAtHour: " << QString::number((query.value(0)).toInt()) << endl;
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
               //qDebug() << "DataProxy_SQLite::getQSOsAtHour: 0" << endl;
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::getQSOsAtHour: Query error" << endl;
        query.finish();
        return 0;
    }
}

bool DataProxy_SQLite::updateQSONumberPerLog()
{
    //qDebug() << Q_FUNC_INFO << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            //qDebug() << Q_FUNC_INFO << " - TRUE2" << endl;
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
        //qDebug() << "DataProxy_SQLite::newDXMarathon" << endl;
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
                         //qDebug() << "DataProxy_SQLite::newDXMarathon - Existing DXCC" << endl;
                    existingDXCC = true;
                }
                if ( (query.value(1)).toInt() == _cq)
                {
                        //qDebug() << "DataProxy_SQLite::newDXMarathon - Existing CQz" << endl;
                    existingCQz = true;
                }

            }
        }

        if (existingDXCC && existingCQz)
        {
                //qDebug() << "DataProxy_SQLite::newDXMarathon - FALSE" << endl;
            query.finish();
            return false;
        }
        else
        {
                //qDebug() << "DataProxy_SQLite::newDXMarathon - TRUE1" << endl;
            query.finish();
            return true;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            //qDebug() << "DataProxy_SQLite::newDXMarathon - TRUE2" << endl;
        query.finish();
         return true;   // It is an error inthe query but Work First Worry Later, let us work that QSO.
    }
}

QStringList DataProxy_SQLite::getContestNames()
{
         //qDebug() << "DataProxy_SQLite::getContestNames() "  << endl;
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
                     //qDebug() << "DataProxy_SQLite::getContestNames: " << queryString  << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QStringList();
    }
}


QStringList DataProxy_SQLite::getContestOverlays()
{
         //qDebug() << "DataProxy_SQLite::getContestOverlays: "<< endl;

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
                     //qDebug() << "DataProxy_SQLite::getContestOverlays: " << queryString  << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QStringList();
    }
}

QStringList DataProxy_SQLite::getPropModeList()
{
        //qDebug()  << "DataProxy_SQLite::getPropModeLists"  << endl;
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
            else
            {
            }
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
}

bool DataProxy_SQLite::addSatellite(const QString &_arrlId, const QString &_name, const QString &_downLink, const QString &_upLink, const QString &_mode, int id)
{
      //qDebug()  << "DataProxy_SQLite::addSatellite: " << QString::number(id)  << endl;
    QSqlQuery query;
    QString queryString;

    if (id>0)
    {
        queryString = QString("UPDATE satellites set satarrlid = '%1', satname = '%2', uplink = '%3', downlink = '%4', satmode = '%5' WHERE id = '%6'").arg(_arrlId).arg(_name).arg(_upLink).arg(_downLink).arg(_mode).arg(id);

    }
    else
    {
        queryString = QString("INSERT INTO satellites (satarrlid, satname, uplink, downlink, satmode) VALUES ('%1', '%2', '%3', '%4', '%5')").arg(_arrlId).arg(_name).arg(_upLink).arg(_downLink).arg(_mode);
    }

    bool sqlOK = query.exec(queryString);
      //qDebug()  << "DataProxy_SQLite::addSatellite - query: " <<  query.lastQuery() << endl;

    if (sqlOK)
    {
          //qDebug()  << "DataProxy_SQLite::addSatellite - TRUE"  << endl;
          //qDebug()  << "DataProxy_SQLite::addSatellite - TRUE - ERROR: " <<  QString::number(query.lastError().number()) << endl;
        query.finish();
        return true;
    }
    else
    {
          //qDebug()  << "DataProxy_SQLite::addSatellite - FALSE"  << endl;
        //if (query.lastError().number() == 19)
        //{ // Duplicate Satellite
        //    QMessageBox msgBox;
        //    msgBox.setIcon(QMessageBox::Warning);
        //    msgBox.setText(tr("A duplicated satellite has been detected in the file and will not be imported."));
        //    msgBox.setInformativeText(tr("Please check the satellite information file and ensure it is properly populated.") + "\n" + tr("Now you will see a more detailed error that can be used for debugging..."));
        //    msgBox.exec();
        //}

        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }

}

int DataProxy_SQLite::getDBSatId(const QString &_arrlId)
{
      //qDebug()  << "DataProxy_SQLite::getDBSatId: " << _arrlId << endl;
 int aux = -1;
 QString queryString = QString("SELECT id FROM satellites WHERE satarrlid='%1'").arg(_arrlId);
 QSqlQuery query;

 bool sqlOK = query.exec(queryString);

 if (sqlOK)
 {
     query.next();
     if (query.isValid())
     {
         aux = query.value(0).toInt();
     }
     else
     {
              //qDebug()  << "DataProxy_SQLite::getDBSatId:  query not valid"  << endl;
         query.finish();
     }
 }
 else
 {
          //qDebug()  << "DataProxy_SQLite::getSatelliteUplink:  query failed: " << query.lastQuery()  << endl;
     emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
     query.finish();
 }

      //qDebug()  << "DataProxy_SQLite::getSatelliteUplink: final: " << aux << endl;
 query.finish();
 return aux;
}

QStringList DataProxy_SQLite::getSatellitesList()
{
         //qDebug()  << "DataProxy_SQLite::getSatellitesList"  << endl;
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
             else
             {
             }
         }

     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     qs.sort();
     return qs;
}


QString DataProxy_SQLite::getSatelliteUplink(const QString &_sat)
{
         //qDebug()  << "DataProxy_SQLite::getSatelliteUplink: " << _sat << endl;
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
            aux = QString::number(getFreqFromRange(aux));
        }
        else
        {
                 //qDebug()  << "DataProxy_SQLite::getSatelliteUplink:  query not valid"  << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << "DataProxy_SQLite::getSatelliteUplink:  query failed: " << query.lastQuery()  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << "DataProxy_SQLite::getSatelliteUplink: final: " << aux << endl;
    query.finish();
    return aux;
}


QString DataProxy_SQLite::getSatelliteDownlink(const QString &_sat)
{
         //qDebug()  << "DataProxy_SQLite::getSatelliteDownlink: " << _sat << endl;
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
            aux = QString::number(getFreqFromRange(aux));
        }
        else
        {
                 //qDebug()  << "DataProxy_SQLite::getSatelliteDownlink:  query not valid"  << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << "DataProxy_SQLite::getSatelliteDownlink:  query failed: " << query.lastQuery()  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << "DataProxy_SQLite::getSatelliteDownlink: final: " << aux << endl;
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
                 //qDebug()  << "DataProxy_SQLite::getSatelliteMode:  query not valid"  << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << "DataProxy_SQLite::getSatelliteMode:  query failed: " << query.lastQuery()  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << "DataProxy_SQLite::getSatelliteMode: final: " << aux << endl;
    return aux;
}

QString DataProxy_SQLite::getSatelliteFullUplink(const QString &_sat)
{
      //qDebug()  << "DataProxy_SQLite::getSatelliteFullUplink: " << _sat << endl;
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
     }
     else
     {
              //qDebug()  << "DataProxy_SQLite::getSatelliteFullUplink:  query not valid"  << endl;
         query.finish();
         return QString();
     }
 }
 else
 {
          //qDebug()  << "DataProxy_SQLite::getSatelliteFullUplink:  query failed: " << query.lastQuery()  << endl;
     emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
     query.finish();
     return QString();
 }

      //qDebug()  << "DataProxy_SQLite::getSatelliteFullUplink: final: " << aux << endl;
 query.finish();
 return aux;
}

QString DataProxy_SQLite::getSatelliteFullDownlink(const QString &_sat)
{
      //qDebug()  << "DataProxy_SQLite::getSatelliteFullDownlink: " << _sat << endl;
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
              //qDebug()  << "DataProxy_SQLite::getSatelliteFullDownlink:  query not valid"  << endl;
         query.finish();
         return QString();
     }
 }
 else
 {
          //qDebug()  << "DataProxy_SQLite::getSatelliteFullDownlink:  query failed: " << query.lastQuery()  << endl;
     emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
     query.finish();
     return QString();
 }

      //qDebug()  << "DataProxy_SQLite::getSatelliteFullDownlink: final: " << aux << endl;
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
                 //qDebug()  << "DataProxy_SQLite::getSatelliteMode:  query not valid"  << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << "DataProxy_SQLite::getSatelliteMode:  query failed: " << query.lastQuery()  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << "DataProxy_SQLite::getSatelliteMode: final: " << aux << endl;
    return aux;
}

QString DataProxy_SQLite::getSatelliteName(const QString &_sat)
{
      //qDebug()  << "DataProxy_SQLite::getSatelliteName: " << _sat << endl;
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
              //qDebug()  << "DataProxy_SQLite::getSatelliteName:  query not valid"  << endl;
         query.finish();
         return QString();
     }
 }
 else
 {
          //qDebug()  << "DataProxy_SQLite::getSatelliteName:  query failed: " << query.lastQuery()  << endl;
     emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
     query.finish();
     return QString();
 }

      //qDebug()  << "DataProxy_SQLite::getSatelliteName: final: " << aux << endl;
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
                 //qDebug()  << "DataProxy_SQLite::getSateliteArrlIdFromId:  query not valid"  << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
             //qDebug()  << "DataProxy_SQLite::getSateliteArrlIdFromId:  query failed: " << query.lastQuery()  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }

         //qDebug()  << "DataProxy_SQLite::getSateliteArrlIdFromId: final: " << aux << endl;
    return aux;

}

double DataProxy_SQLite::getFreqFromRange(QString _fr)
{ //May even receive: 145.900-146.00 and should return the mid in the range (145.950)
         //qDebug()  << "DataProxy_SQLite::getFreqFromRange: " << _fr << endl;

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
        // We select the  first range

             //qDebug()  << "DataProxy_SQLite::getFreqFromRange: has several freqs: " << aux << endl;
        aux = aux.section(',', 0, 0);   // We select the first package

    }
    if (aux.contains('-'))          // Potentially somethink like: 435.030-435.456
    {
             //qDebug()  << "DataProxy_SQLite::getFreqFromRange: has several freqs: " << aux << endl;
        fr2 = aux.section('-', 1, 1);   // We select the second freq
        fr1 = aux.section('-', 0, 0);   // We select the first freq

             //qDebug()  << "DataProxy_SQLite::getFreqFromRange: fr1: " << fr1 << endl;
             //qDebug()  << "DataProxy_SQLite::getFreqFromRange: fr2: " << fr2 << endl;
        f1 = fr1.toDouble();
        f2 = fr2.toDouble();
             //qDebug()  << "DataProxy_SQLite::getFreqFromRange: f1: " << QString::number(f1) << endl;
             //qDebug()  << "DataProxy_SQLite::getFreqFromRange: f2: " << QString::number(f2) << endl;

        f1 = (f2 + f1)/2;

             //qDebug()  << "DataProxy_SQLite::getFreqFromRange: f1 after calc: " << QString::number(f1) << endl;
    }
    else
    {   // It is only one freq 145.950 so this is what must be returned
        f1 = aux.toDouble();
    }

         //qDebug()  << "DataProxy_SQLite::getFreqFromRange: Return: " << QString::number(f1) << endl;
    return f1;
}

QStringList DataProxy_SQLite::getQSLRcvdList()
{
         //qDebug()  << "DataProxy_SQLite::getQSLRcvdList"  << endl;
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
             else
             {
             }
         }

     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}

QStringList DataProxy_SQLite::getQSLSentList()
{
         //qDebug()  << "DataProxy_SQLite::getQSLSentList"  << endl;
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
             else
             {
             }
         }

     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}


QStringList DataProxy_SQLite::getClubLogStatusList()
{
         //qDebug()  << "DataProxy_SQLite::getClubLogStatusList"  << endl;
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
             else
             {
             }
         }

     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}

QStringList DataProxy_SQLite::getQSLViaList()
{
         //qDebug()  << "DataProxy_SQLite::getQSLViaList"  << endl;
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
             else
             {
             }
         }

     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
         return QStringList();
     }
     query.finish();
     return qs;
}

/*
QStringList DataProxy_SQLite::getValidCatOptions(const int _currentCat, const int _lowerCat)
{
         //qDebug() << "DataProxy_SQLite::getContestNames: " << QString::number(_currentCat) <<"/" << QString::number(_lowerCat) << endl;
    QStringList contests = QStringList();
    QSqlQuery query;
    QString queryString;
    bool sqlOK;

    switch (_currentCat) {

        case 0:
            queryString = QString("SELECT DISTINCT contestcatoperator.name FROM contestcatoperator JOIN contest ON contest.catoperator=contestcatoperator.id WHERE contest.contest='%1' ORDER BY contestcatoperator.id").arg(_lowerCat);
        break;
        case 1:
            queryString = QString("SELECT DISTINCT contestcatassisted.name FROM contestcatassisted JOIN contest ON contest.catassisted=contestcatassisted.id WHERE contest.catoperator='1' ORDER BY contestcatassisted.id").arg(_lowerCat);
        break;
        case 2:
            queryString = QString("SELECT DISTINCT contestcatpower.name FROM contestcatpower JOIN contest ON contest.catpower=contestcatpower.id WHERE contest.catoperator='1' ORDER BY contestcatpower.id").arg(_lowerCat);
        break;
        case 3:
            queryString = QString("SELECT DISTINCT contestcatband.name FROM contestcatband JOIN contest ON contest.catband=contestcatband.id WHERE contest.catoperator='1' ORDER BY contestcatband.id").arg(_lowerCat);
        break;
        case 4:
            queryString = QString("SELECT DISTINCT contestcatoverlay.name FROM contestcatoverlay JOIN contest ON contest.catoverlay=contestcatoverlay.id WHERE contest.catoperator='1' ORDER BY contestcatoverlay.id").arg(_lowerCat);
        break;
        case 5:
            queryString = QString("SELECT DISTINCT contestcatmode.name FROM contestcatmode JOIN contest ON contest.catmode=contestcatmode.id WHERE contest.catoperator='1' ORDER BY contestcatmode.id").arg(_lowerCat);
        break;
        default:
            return QStringList();
        break;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QStringList();
    }
}
*/

bool DataProxy_SQLite::haveAtLeastOneLog()
{
         //qDebug() << "DataProxy_SQLite::haveAtLeastOneLog()" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    //return false;
}

QStringList DataProxy_SQLite::getColumnNamesFromTableLog()
{
       //qDebug() << "DataProxy_SQLite::getColumnNamesFromTableLog" << endl;
    return getColumnNamesFromTable("log");
}

QStringList DataProxy_SQLite::getColumnNamesFromTable(const QString &_tableName)
{
       //qDebug() << "DataProxy_SQLite::getColumnNamesFromTable" << endl;
    return db->getColumnNamesFromTable(_tableName);
}

bool DataProxy_SQLite::setDXCCAwardStatus(const int _qsoId)
{
    // If the band/mode/log is already confirmed: Return true
    // If the band/mode/log is already worked and status worked: Return true
    // If the band/mode/log is already worked and status confirmed: Update and Return true
    // If not worked: Add and Return true

      //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: " << QString::number(_qsoId) << endl;
    if (_qsoId <= 0)
    {
           //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: QSOid <=0 " << QString::number(_qsoId) << endl;
        return false;
    }

    int _dxcc = getDXCCFromId(_qsoId);
    if (_dxcc <= 0)
    {
           //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: DXCC <= 0: DXCC: " << QString::number(_dxcc) << " - "  << QString::number(_qsoId) << endl;
        return false;
    }

    int _band = getBandFromId(_qsoId);
    if (_band <= 0)
    {
          //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: Band <= 0: BAND: " << QString::number(_band) << " - "  << QString::number(_qsoId) << endl;
        return false;
    }

    int _mode = getModeFromId(_qsoId);
    if (_mode <= 0)
    {
          //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: Mode <= 0: Mode: " << QString::number(_mode) << " - "  << QString::number(_qsoId) << endl;
        return false;
    }

    int _log = getLogNumberFromQSOId(_qsoId);
    if (_log <= 0)
    {
          //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: Log <= 0: Log: " << QString::number(_log) << " - "  << QString::number(_qsoId) << endl;
        return false;
    }

    // If the band/mode/log is already confirmed: Return true
    QSqlQuery query;

    // awarddxcc id dxcc band mode confirmed qsoid lognumber
    // If the band/mode/log is already confirmed: Return true
    // If the band/mode/log is already worked and status worked: Return true
    // If the band/mode/log is already worked and status confirmed: Update and Return true
    // If not worked: Add and Return true

    QString queryString = QString("SELECT id, confirmed, qsoid FROM awarddxcc WHERE band='%1' AND mode='%2' AND dxcc='%3'").arg(_band).arg(_mode).arg(_dxcc);

    bool sqlOK = query.exec(queryString);
      //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: Queryexec-1: " << queryString << endl;
    queryString.clear();

    if (sqlOK)
    {
        QSqlRecord rec = query.record();
        query.next();
        //int nameCol = -1;
        if (query.isValid())
        {
            int nameCol = rec.indexOf("id");
            int __id = (query.value(nameCol)).toInt();

            nameCol = rec.indexOf("confirmed");
            QString __confirmed = (query.value(nameCol)).toString();
            if (__confirmed == "1")
            {   // #1 - If the band/mode/log is already confirmed: Return true
                query.finish();
                return true;
            }
            else if (__confirmed == "0")
            {
                if (!isQSLReceived((_qsoId)))
                {// #2 - If the band/mode/log is already worked and status worked: Return true
                    query.finish();
                    return true;
                }
                else
                { // #3 - If the band/mode/log is already worked and status confirmed: Update and Return true
                    nameCol = rec.indexOf("qsoid");
                    //int __qsoid = (query.value(nameCol)).toInt();
                    queryString = QString("UPDATE awarddxcc SET confirmed = '1', qsoid = '%1' WHERE id = '%2'").arg(_qsoId).arg(__id);
                }
            }
            else
            {   // This case should not happen?
                query.finish();
                return true;
            }

            query.finish();

            // #1 - If the band/mode/log is already confirmed: Return true
            // #2 - If the band/mode/log is already worked and status worked: Return true
            // #3 - If the band/mode/log is already worked and status confirmed: Update and Return true
            // #4 - If not worked: Add and Return true
        }
        else
        {
            //#4 - If not worked: Add and Return true
            query.finish();
            // awarddxcc id dxcc band mode confirmed qsoid lognumber
            queryString = QString("INSERT INTO awarddxcc (dxcc, band, mode, confirmed, qsoid, lognumber) values('%1','%2','%3','0', '%4', '%5')").arg(_dxcc).arg(_band).arg(_mode).arg(_qsoId).arg(_log);
        }

        if (queryString.length()>5)
        {
            if (query.exec(queryString))
            {
                  //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: Queryexec-2: " << queryString << endl;
                query.finish();
                return true;
            }

            else
            {
                if(query.lastError().number()==19)
                {}
                else
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                    query.finish();
                    return false;

                }


            }
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    query.finish();
    return true;

}


bool DataProxy_SQLite::setWAZAwardStatus(const int _qsoId)
{
    // If the band/mode/log is already confirmed: Return true
    // If the band/mode/log is already worked and status worked: Return true
    // If the band/mode/log is already worked and status confirmed: Update and Return true
    // If not worked: Add and Return true

       //qDebug() << "DataProxy_SQLite::setDXCCAwardStatus: " << QString::number(_qsoId) << endl;
    if (_qsoId <= 0)
    {
        return false;
    }

    int _cqz = getCQZFromId(_qsoId);
    if (_cqz <= 0)
    {
        return false;
    }

    int _band = getBandFromId(_qsoId);
    if (_band <= 0)
    {
        return false;
    }

    int _mode = getModeFromId(_qsoId);
    if (_mode <= 0)
    {
        return false;
    }

    int _log = getLogNumberFromQSOId(_qsoId);
    if (_log <= 0)
    {
        return false;
    }

    // If the band/mode/log is already confirmed: Return true
    QSqlQuery query;

    // awarddxcc id dxcc band mode confirmed qsoid lognumber
    // If the band/mode/log is already confirmed: Return true
    // If the band/mode/log is already worked and status worked: Return true
    // If the band/mode/log is already worked and status confirmed: Update and Return true
    // If not worked: Add and Return true

    QString queryString = QString("SELECT id, confirmed, qsoid FROM awardwaz WHERE band='%1' AND mode='%2' AND cqz='%3'").arg(_band).arg(_mode).arg(_cqz);

    bool sqlOK = query.exec(queryString);
    queryString.clear();

    if (sqlOK)
    {
        QSqlRecord rec = query.record();
        query.next();
        int nameCol = -1;
        if (query.isValid())
        {
            nameCol = rec.indexOf("id");
            int __id = (query.value(nameCol)).toInt();

            nameCol = rec.indexOf("confirmed");
            QString __confirmed = (query.value(nameCol)).toString();
            if (__confirmed == "1")
            {   // #1 - If the band/mode/log is already confirmed: Return true
                query.finish();
                return true;
            }
            else if (__confirmed == "0")
            {
                if (!isQSLReceived((_qsoId)))
                {// #2 - If the band/mode/log is already worked and status worked: Return true
                    query.finish();
                    return true;
                }
                else
                { // #3 - If the band/mode/log is already worked and status confirmed: Update and Return true
                    nameCol = rec.indexOf("qsoid");
                    //int __qsoid = (query.value(nameCol)).toInt();
                    queryString = QString("UPDATE awardwaz SET confirmed = '1', qsoid = '%1' WHERE id = '%2'").arg(_qsoId).arg(__id);
                }
            }
            else
            {   // This case should not happen?
                query.finish();
                return true;
            }

            query.finish();

            // #1 - If the band/mode/log is already confirmed: Return true
            // #2 - If the band/mode/log is already worked and status worked: Return true
            // #3 - If the band/mode/log is already worked and status confirmed: Update and Return true
            // #4 - If not worked: Add and Return true
        }
        else
        {
            //#4 - If not worked: Add and Return true
            query.finish();
            // awarddxcc id dxcc band mode confirmed qsoid lognumber
            queryString = QString("INSERT INTO awardwaz (cqz, band, mode, confirmed, qsoid, lognumber) values('%1','%2','%3','0', '%4', '%5')").arg(_cqz).arg(_band).arg(_mode).arg(_qsoId).arg(_log);
        }

        if (queryString.length()>5)
        {
            if (query.exec(queryString))
            {
                query.finish();
                return true;
            }
            else
            {
                if(query.lastError().number()==19)
                {}
                else
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                    query.finish();
                    return false;

                }
            }
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    query.finish();
    return true;

}
bool DataProxy_SQLite::addDXCCEntitySubdivision(const QString &_name, const QString &_short, const QString &_pref,
                                                const QString &_group, const int _regId,
                                                const int _dxcc, const int _cq, const int _itu,
                                                const QDate &_startDate, const QDate &_endDate,
                                                const bool _deleted)
{
    //qDebug() << "DataProxy_SQLite::addDXCCEntitySubdivision: length: " << _name << endl;
    // id / name / shortname / prefix / regionalgroup / regionalid / dxcc / cqz / ituz / start_date / end_date / deleted
    QString queryString;
    QSqlQuery query;
    bool sqlOK = false;

    //qDebug() << "DataProxy_SQLite::addRegionalAward: Importing: " << _regionalAward.getRegionalAwardRefName(i)  << endl;
    queryString = QString("INSERT INTO primary_subdivisions (name, shortname, prefix, regionalgroup, "
                          "regionalid, dxcc, cqz, ituz, start_date, end_date, deleted) "
                              "values ('%1','%2','%3', '%4','%5','%6', '%7','%8', '%9', '%10','%11')")
            .arg(_name).arg(_short).arg(_pref).arg(_group).arg(_regId).arg(_dxcc)
            .arg(_cq).arg(_itu).arg(util->getDateSQLiteStringFromDate(_startDate))
            .arg(util->getDateSQLiteStringFromDate(_endDate)).arg(util->boolToCharToSQLite(_deleted));

    sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        //qDebug() << "DataProxy_SQLite::addRegionalAward: " << query.lastQuery() << endl;
        query.finish();
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addRegionalAward: ERROR " << endl;
        //qDebug() << "DataProxy_SQLite::addRegionalAward: - query error: " << QString::number(query.lastError().number()) << endl;
        //qDebug() << "DataProxy_SQLite::addRegionalAward: LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "DataProxy_SQLite::addRegionalAward: LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "DataProxy_SQLite::addRegionalAward: LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "DataProxy_SQLite::addRegionalAward: LastError-n: " << QString::number(query.lastError().number() ) << endl;

        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    //qDebug() << "DataProxy_SQLite::addDXCCEntitySubdivision: END"  << endl;
    return true;
}



int DataProxy_SQLite::getNumberOfManagedLogs()
{
         //qDebug() << "DataProxy_SQLite::getNumberOfManagedLogs" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1;
    }
    //return -1;
}


QStringList DataProxy_SQLite::getListOfManagedLogs()
{
    //This function returns the list of log IDs that are being managed
         //qDebug() << "DataProxy_SQLite::getListOfManagedLogs" << endl;
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
                     //qDebug() << "DataProxy_SQLite::getListOfManagedLogs: " << (query.value(0)).toString()  << endl;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        qs.clear();
    }
    query.finish();
    return qs;
}

QString DataProxy_SQLite::getStationCallSignFromLog(const int _log)
{
         //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: " << QString::number(_log)<< endl;
    QSqlQuery query;
    QString queryString = QString("SELECT stationcall FROM logs WHERE id='%1'").arg(_log);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: " <<  (query.value(0)).toString() << endl;
            QString v = (query.value(0)).toString();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: Not valid" << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: query failed" << endl;
        query.finish();
        return QString();
    }

         //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: END" << endl;
    //return QString();

}

QStringList DataProxy_SQLite::getStationCallSignsFromLog(const int _log)
{
    //qDebug() << "DataProxy_SQLite::getStationCallSignsFromLog" << endl;
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
                    //qDebug() << "DataProxy_SQLite::getStationCallSignsFromLog: " << queryString << endl;

           }
           else
           {
               query.finish();
               //qDebug() << "DataProxy_SQLite::getStationCallSignsFromLog-END-1 - fail" << endl;
               return QStringList();
           }

       }
       query.finish();
       calls.removeDuplicates();
   }
   else
   {
       emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
       query.finish();
       //qDebug() << "DataProxy_SQLite::getStationCallSignsFromLog-END-2 - fail" << endl;
       return QStringList();
   }
   calls.sort();
   //qDebug() << "DataProxy_SQLite::getStationCallSignsFromLog-END" << endl;
   return calls;
}



QString DataProxy_SQLite::getOperatorsFromLog(const int _log)
{
    //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: " << QString::number(_log)<< endl;
    QSqlQuery query;
    QString queryString = QString("SELECT operators FROM logs WHERE id='%1'").arg(_log);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
               //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: " <<  (query.value(0)).toString() << endl;
            QString v = (query.value(0)).toString();
            query.finish();
            return v;
        }
        else
        {
               //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: Not valid" << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
           //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: query failed" << endl;
        query.finish();
        return QString();
    }

  //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: END" << endl;
  //return QString();
}

QString DataProxy_SQLite::getCommentsFromLog(const int _log)
{
       //qDebug() << "DataProxy_SQLite::getLogDateFromLog: " << QString::number(_log)<< endl;
  QSqlQuery query;
  QString queryString = QString("SELECT comment FROM logs WHERE id='%1'").arg(_log);
  bool sqlOK = query.exec(queryString);

  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
               //qDebug() << "DataProxy_SQLite::getCommentsFromLog: " <<  (query.value(0)).toString() << endl;
          QString v = (query.value(0)).toString();
          query.finish();
          return v;
      }
      else
      {
               //qDebug() << "DataProxy_SQLite::getCommentsFromLog: Not valid" << endl;
          query.finish();
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
           //qDebug() << "DataProxy_SQLite::getLogDateFromLog: query failed" << endl;
      query.finish();
      return QString();
  }

       //qDebug() << "DataProxy_SQLite::getCommentsFromLog: END" << endl;
  //return QString();
}

QString DataProxy_SQLite::getLogDateFromLog(const int _log)
{
       //qDebug() << "DataProxy_SQLite::getLogDateFromLog: " << QString::number(_log)<< endl;
  QSqlQuery query;
  QString queryString = QString("SELECT logdate FROM logs WHERE id='%1'").arg(_log);
  bool sqlOK = query.exec(queryString);

  if (sqlOK)
  {
      query.next();
      if (query.isValid())
      {
               //qDebug() << "DataProxy_SQLite::getLogDateFromLog: " <<  (query.value(0)).toString() << endl;
          QString v = (query.value(0)).toString();
          query.finish();
          return v;
      }
      else
      {
               //qDebug() << "DataProxy_SQLite::getLogDateFromLog: Not valid" << endl;
          query.finish();
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
           //qDebug() << "DataProxy_SQLite::getLogDateFromLog: query failed" << endl;
      query.finish();
      return QString();
  }

       //qDebug() << "DataProxy_SQLite::getLogDateFromLog: END" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1;
    }
}

bool DataProxy_SQLite::fillEmptyDXCCInTheLog()
{
    //qDebug() << "DataProxy_SQLite::fillEmptyDXCCInTheLog"  << endl;

    QSqlQuery query;
    QSqlQuery query2;

    QString queryString = QString("SELECT COUNT (id) FROM log WHERE dxcc IS NULL OR dxcc<'1' OR cont IS NULL");

    bool sqlOK = query.exec(queryString);
    int qsos = -1;

    if (sqlOK)
    {
        //QSqlDatabase::database().commit();
        query.next();
        qsos = (query.value(0)).toInt();
        query.finish();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    if (qsos < 1)
    {
        return true;
    }
    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

    queryString = QString("SELECT id, call FROM log WHERE dxcc IS NULL OR dxcc<1 OR cont IS NULL");
    sqlOK = query.exec(queryString);

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
                   //qDebug() << "DataBase::fillEmptyDXCCInTheLog: DXCC: " << _dxcc << endl;
                   //qDebug() << "DataBase::fillEmptyDXCCInTheLog: Cont: " << _continent << endl;
                // UPDATE THE ID WITH THE DXCC
                queryString = QString("UPDATE log SET dxcc = '%1', cont = '%2' WHERE id = '%3'").arg(_dxcc).arg(_continent).arg(_id);
                sqlOK = query2.exec(queryString);
                if (!sqlOK)
                {
                    emit queryError(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().number(), query2.lastQuery());
                    query2.finish();
                    return false;
                }
                query2.finish();

                if (( (j % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    _aux = QObject::tr("Updating DXCC and Continent information...") + "\n" + QObject::tr("QSO: ")  + QString::number(j) + "/" + QString::number(qsos);
                    //_aux = "Updating ...";
                    progress.setLabelText(_aux);
                    progress.setValue(j);
                }
                if ( progress.wasCanceled() )
                {
                       //qDebug() << "DataBase::fillEmptyDXCCInTheLog: progress canceled" << endl;
                    query.finish();
                    return true;
                }
                j++;
            }

        }
        progress.setValue(qsos);
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog DXCC"));
        msgBox.setText(tr("All QSOs have been updated with a DXCC and the Continent.") );
        msgBox.exec();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }

    query.finish();
    return true;
}


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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int DataProxy_SQLite::getQSOsWithContinent(const QString &_cont, const int _logNumber)
{
       //qDebug() << "DataProxy_SQLite::getQSOsWithContinent: " << _cont << endl;
    QSqlQuery query;
    QString queryString;
    bool sqlOK;
    if (_logNumber < 0)
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log where cont LIKE '%1'").arg(_cont);
    }
    else
    {
        queryString = QString("SELECT COUNT (DISTINCT id) FROM log where lognumber='%1' AND cont LIKE '%2'").arg(_logNumber).arg(_cont);
    }


    sqlOK = query.exec(queryString);

         //qDebug() << "DataProxy_SQLite::getQSOsWithContinent: queryString: " << queryString << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                 //qDebug() << "DataProxy_SQLite::getQSOsWithContinent: " << QString::number((query.value(0)).toInt()) << endl;
            int v = (query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
                 //qDebug() << "DataProxy_SQLite::getQSOsWithContinent: 0" << endl;
            query.finish();
            return 0;
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
             //qDebug() << "DataProxy_SQLite::getQSOsWithContinent: Query error" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return 0;
    }
}

QList<QList<int>> DataProxy_SQLite::getTop10QSOPerDXCC(const int _log)
{
    //qDebug() << Q_FUNC_INFO << " : " << QString::number(_log) << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return result;
    }


    //qDebug() << Q_FUNC_INFO << " - END"  << endl;
    //return result;



}

bool DataProxy_SQLite::addNewLog (const QStringList _qs)
{
        //qDebug() << "DataProxy_SQLite::addNewLog: " << _qs.at(2) << "/" << _qs.at(5) << "/" << _qs.at(6) << endl;
      //qDebug() << "DataProxy_SQLite::addNewLog: Size: " << QString::number(_qs.size()) << endl;
    // newLogq << dateString << stationCallsign << operators << comment << QString::number(selectedLog) << _qs.at(4) ; (last field is 1 or 0 editing)

    if (_qs.size()!=6)
    {
          //qDebug() << "DataProxy_SQLite::addNewLog: != 6"  << endl;
        return false;
    }
      //qDebug() << "DataProxy_SQLite::addNewLog: Has the appropriate length"  << endl;

    QString aux = QString();
    //int nameCol = -1;

    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);
    QString _operators = _qs.at(2);

    //_operators.clear();
    //_operators << (_qs.at(2)).split(',', QString::SkipEmptyParts);
    //QString _typeContest = _qs.at(3);
    QString _comment = _qs.at(3);
    //QString _typeContestN = _qs.at(5);

    QString id = _qs.at(4);
    QString editing = _qs.at(5);

    QString queryString;
    QSqlQuery query;
    bool sqlOK;

    if (editing == "1")
    { // We are editing
            //qDebug() << "DataProxy_SQLite::addNewLog: We are editing!" << endl;
        //queryString = QString("UPDATE logs SET logdate = '%1', stationcall = '%2', operators = '%3', comment = '%4',  logtype = '%5', logtypen = '%6' WHERE id = '%7'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(_typeContest).arg(_typeContestN).arg(id);
        queryString = QString("UPDATE logs SET logdate = '%1', stationcall = '%2', operators = '%3', comment = '%4' WHERE id = '%5'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(id);
        sqlOK = query.exec(queryString);

        if (sqlOK)
        {
                //qDebug() << "DataProxy_SQLite::addNewLog: Editing OK!" << endl;
            query.finish();
            return true;
        }
        else
        {
                //qDebug() << "DataProxy_SQLite::addNewLog: Editing NOK!" << endl;
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            query.finish();
            return false;
        }
        //return false;
    }

        //qDebug() << "DataProxy_SQLite::addNewLog: We are adding a new log" << endl;

    // First we check if the log is already there
    //queryString = QString("SELECT id FROM logs WHERE logdate='%1' AND stationcall='%2' AND logtype='%3' AND logtypen='%4'").arg(_dateString).arg(_stationCallsign).arg(_typeContest).arg(_typeContestN);
    queryString = QString("SELECT id FROM logs WHERE logdate='%1' AND stationcall='%2' AND operators = '%3' AND comment = '%4'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment);
    //"logs"
    //"id, logdate, stationcall, comment, logtype"
    //qDebug() << "DataProxy_SQLite::addNewLog query1: " << queryString << endl;

    sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            //qDebug() << "DataProxy_SQLite::addNewLog query error: " << queryString << endl;
            // It seems that the log is already existing!
            return false;
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }




    //Now we add the new log

    //queryString = QString("INSERT INTO logs (logdate, stationcall, operators, comment, logtype, logtypen) values('%1','%2','%3','%4', '%5', '%6')").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(_typeContest).arg(_typeContestN);
    queryString = QString("INSERT INTO logs (logdate, stationcall, operators, comment) values('%1','%2','%3','%4')").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment);
         //qDebug() << "DataProxy_SQLite::addNewLog query1: " << queryString << endl;
    sqlOK = query.exec(queryString);


    if (sqlOK)
    {
        query.finish();
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    //return false;
}



bool DataProxy_SQLite::doesThisLogExist(const int _log)
{
       //qDebug() << "DataProxy_SQLite::doesThisLogExist: " << QString::number(_log) << endl;
       //qDebug() << "DataProxy_SQLite::doesThisLogExist - Name:" << db->getDBName() << endl;
    QSqlQuery query;

    QString queryString = QString("SELECT id FROM logs WHERE id='%1'").arg(_log);
    bool sqlOK = query.exec(queryString);

       //qDebug() << "DataProxy_SQLite::doesThisLogExist: query: " << query.lastQuery()  << endl;

    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                query.finish();
                   //qDebug() << "DataProxy_SQLite::doesThisLogExist: END TRUE"  << endl;
                return true;
            }
            else
            {
                query.finish();
                   //qDebug() << "DataProxy_SQLite::doesThisLogExist: END FALSE 1"  << endl;
                return false;
            }
        }
        else
        {
            query.finish();
               //qDebug() << "DataProxy_SQLite::doesThisLogExist: END FALSE 2"  << endl;
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
           //qDebug() << "DataProxy_SQLite::doesThisLogExist: END FALSE 3"  << endl;
        return false;
    }
       //qDebug() << "DataProxy_SQLite::doesThisLogExist: END FALSE 4"  << endl;
    //return false;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -2;
    }
}

QString DataProxy_SQLite::getContinentShortNameFromEntity(const int _n)
{
    QSqlQuery query;

    QString queryString= QString("SELECT continent FROM entity WHERE dxcc='%1'").arg(_n);
    //QString queryString= QString("SELECT continent.shortname FROM entity JOIN continent ON entity.continent=continent.shortname WHERE dxcc='%1'").arg(_n);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return  QString();
    }
}

int DataProxy_SQLite::getContinentIdFromEntity(const int _n)
{
    QSqlQuery query;
    QString queryString = QString("SELECT continent.id FROM entity JOIN continent ON entity.continent=continent.shortname WHERE dxcc='%1'").arg(_n);
    bool sqlOK = query.exec(queryString);

    //aux = QString("SELECT continent.id FROM entity JOIN continent ON entity.continent=continent.shortname WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_n);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
}

int DataProxy_SQLite::getITUzFromPrefix(const QString &_p)
{
    QSqlQuery query;
    QString queryString = QString("SELECT ituz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -2;
    }

}

int DataProxy_SQLite::getCQzFromPrefix(const QString &_p)
{
    QSqlQuery query;
    QString queryString = QString("SELECT cqz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -2;
    }
}

QString DataProxy_SQLite::getEntityNameFromId(const int _n)
{
    //qDebug() << "DataProxy_SQLite::getEntityNameFromId: " << QString::number(_n) << endl;

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
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
    //return QString();
}

int DataProxy_SQLite::getEntityIdFromName(const QString &_e)
{
    //qDebug() << "DataProxy_SQLite::getEntityIdFromName:" << _e << endl;

    int id = -1;
  QString queryString;
  QSqlQuery query;

  //queryString = QString("SELECT mainprefix FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_entityN);
  queryString = QString("SELECT dxcc FROM entity WHERE name='%1'").arg(_e);
  //queryString = "SELECT prefix FROM prefixesofentity WHERE dxcc=='" + QString::number(i) +"'";
  bool sqlOK = query.exec(queryString);

  if (!sqlOK)
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
QStringList DataProxy_SQLite::getEntiNameAndPrefixFromId(const int _dxcc)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_dxcc);

    if (_dxcc <= 0 )
    {
        return QStringList();
    }
    bool sqlOK;
    QString queryString;
    QSqlQuery query;
    QString motherEntName = QString();
    if (_dxcc > 1000)
    {
        QString aux = (QString::number(_dxcc)).right(3);
        QString queryString2 = QString("SELECT name FROM entity WHERE dxcc='%1'").arg(aux);
        sqlOK = query.exec(queryString2);

        if (sqlOK)
        {
            if (query.next())
            {
                if (query.isValid())
                {
                    motherEntName = (query.value(0)).toString();
                    if (motherEntName.length ()<1)
                    {
                        return QStringList();
                    }
                }
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            query.finish ();
            return QStringList();
        }
    }
    query.finish ();

    QStringList result;
    result.clear();
    queryString = QString("SELECT mainprefix, name FROM entity WHERE dxcc='%1'").arg(_dxcc);

    sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QStringList();
    }
    else
    {
        query.next();

        if (query.isValid())
        {
            queryString = (query.value(0)).toString();
            QString prefix = (query.value(0)).toString();
            QString name  = (query.value(1)).toString();
            query.finish();
            if (prefix.length ()<1)
            {
                return QStringList();
            }
            if (name.length ()<1)
            {
                return QStringList();
            }
            if ((_dxcc>1000) && (motherEntName.length ()>2))
            {
                name = name + "(" + motherEntName + ")";
            }

            result << prefix << name;
            return result;
        }
        else
        {
            query.finish();
            return QStringList();
        }
    }
}

QString DataProxy_SQLite::getEntityMainPrefix(const int _entityN)
{
        //qDebug() << "DataProxy_SQLite::getEntityMainPrefix:" << QString::number(_entityN) << endl;

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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

      //qDebug() << "DataProxy_SQLite::getEntityIdFromMainPrefix:" << _e << endl;

    int id = -1;
    QString queryString;
    QSqlQuery query;
    queryString = QString("SELECT dxcc FROM entity WHERE mainprefix='%1'").arg(_e);
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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

int DataProxy_SQLite::getDXCCFromPrefix(const QString &_p)
{
      //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix - " << Q_FUNC_INFO << "-" << _p << "-" << endl;

    QSqlQuery query;
    QString queryString = QString("SELECT dxcc FROM prefixesofentity WHERE prefix='%1'").arg(_p);
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
          //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix: query OK: query: " << queryString << endl;
        if (query.next())
        {
            if (query.isValid())
            {
                int v = (query.value(0)).toInt();
                query.finish();
                  //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix: return 0: " << QString::number(v)  << endl;
                return v;
            }
            else
            {
                query.finish();
                  //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix: return -1: " << endl;
                return -1;
            }
        }
        else
        {
            query.finish();
              //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix: return -2: " << endl;
            return -2;
        }
    }
    else
    {
          //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix: query NOK: query: " << queryString << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
          //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix: return -3: " << endl;
        return -3;
    }
    //return -4;
}


bool DataProxy_SQLite::isNewCQz(int _c)
{
    QSqlQuery query;
    QString queryString = QString("SELECT id FROM log WHERE cqz='%1'").arg(_c);
    //queryString = "SELECT id FROM log WHERE cqz=='" + QString::number(_cqz) +"'";
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
            return v;
        }

    }
    //return 0.0;
}

double DataProxy_SQLite::getLatitudeFromEntity(const int _e)
{

    QString queryString = QString("SELECT latitude FROM entity WHERE dxcc='%1'").arg(_e);
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
            else
            {
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

    //return QString();
}

QStringList DataProxy_SQLite::getEntitiesNames()
{
         //qDebug()  << "DataProxy_SQLite::getEntitiesNames"  << endl;
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
            else
            {
            }
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.finish();
    qs.sort();
    return qs;
}


/*
 * The following code was showing in the Entities prefixes and entities like Sicily, African Italy... and they are not officially an entity in the DXCC
QStringList DataProxy_SQLite::getEntitiesNames()
{
        //qDebug()  << "DataProxy_SQLite::getEntitiesNames"  << endl;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString queryString = QString("SELECT mainprefix, name, dxcc FROM entity");
    QSqlQuery query,query2;
    int dxcc = -1;
    QString aux2 = QString();

    if (query.exec(queryString))
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                dxcc = query.value(2).toInt();
                    //qDebug()  << "DataProxy_SQLite::getEntitiesNames - DXCC: " <<  QString::number(dxcc) << endl;
                if (dxcc > 1000)
                {
                        //qDebug()  << "DataProxy_SQLite::getEntitiesNames - DXCC>1000 going in details: " << endl;

                    aux2 = QString::number(dxcc);
                    aux2 = aux2.right(3);
                        //qDebug()  << "DataProxy_SQLite::getEntitiesNames - aux2: " <<  aux2 << endl;

                    queryString = QString("SELECT mainprefix, name FROM entity WHERE dxcc ='%1'").arg(aux2);
                    if (query2.exec(queryString))
                    {
                        if (query2.next())
                        {
                            if (query2.isValid())
                            {
                                //aux2 = (query2.value(1)).toString();

                                aux.clear();
                                aux = (query.value(0)).toString() + "-" + (query.value(1)).toString()+" (" + (query2.value(0)).toString() + "-" + (query2.value(1)).toString() + " - " + aux2 + ")" ;

                            }
                        }

                    }
                    else
                    {
                        //TODO: Manage the error
                    }

                }
                else
                {
                        //qDebug()  << "DataProxy_SQLite::getEntitiesNames - DXCC<1000 quick! " << endl;
                    aux.clear();
                    aux = (query.value(0)).toString() + "-" + (query.value(1)).toString()+" ("+QString::number(dxcc)+")";

                }

                        //qDebug()  << "DataProxy_SQLite::getEntitiesNames - AUX: " << aux << endl;
                //result = result + ", " + (query.value(0)).toString();
                qs << aux;
            }
            else
            {
                //TODO: Manage the error
            }
        }
    }
    else
    {
                //TODO: Manage the error
    }
    return qs;
}
*/

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return -1;
    }
}

bool DataProxy_SQLite::updateISONames()
{
       //qDebug()  << "DataProxy_SQLite::updateISONames"  << endl;
    bool result;
    result = db->updateTheEntityTableISONames();

    return result;
}

QString DataProxy_SQLite::getISOName(const int _n)
{
    //qDebug()  << "DataProxy_SQLite::getISONames: " << QString::number(_n)  << endl;
    if (_n <= 0 )
    {
             //qDebug()  << "DataProxy_SQLite::getISONames: NOT KNOWN - UN" << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            //qDebug()  << "DataProxy_SQLite::getISOName: Query error - UN"  << endl;
        query.finish();
        return "un"; // When no flag is known, we return the UN flag
    }
    else
    {
        query.next();

        if (query.isValid()){
            //qDebug()  << "DataProxy_SQLite::getISOName: " << "N: " << QString::number(_n) << "- ISO Name: " << (query.value(0)).toString() << endl;
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
                  //qDebug()  << "DataProxy_SQLite::getISOName: NO ISO Name: "  << endl;
            query.finish();
            return "un"; // When no flag is known, we return the UN flag
        }
    }
         //qDebug()  << "DataProxy_SQLite::getISOName: NO ISO Name: default"  << endl;
    //return "un"; // When no flag is known, we return the UN flag
}

void DataProxy_SQLite::getFoundInLog(const QString &_txt, const int _log)
{
         //qDebug()  << "DataProxy_SQLite::getFoundInLog: " << _txt << "/" << QString::number(_log)  << endl;
/*
    searching = true;
    executionN++;

    QSqlQuery query;
    QString queryString, aux;
    aux.clear();
    QStringList qsoDataFound;
    qsoDataFound.clear();
    if (_txt.length()<2)
    {
             //qDebug()  << "DataProxy_SQLite::getFoundInLog: lengh shorter than 2"  << endl;
        searching = false;
        return;
    }

         //qDebug()  << "DataProxy_SQLite::getFoundInLog: let's go searching!"  << endl;
    if (_log <= 0)
    {
        queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call LIKE '%%1%'").arg(_txt);
    }
    else
    {
        queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call LIKE '%%1%' AND lognumber='%2'").arg(_txt).arg(_log);
    }

    bool sqlOk = query.exec(queryString);
    if (sqlOk)
    {
             //qDebug()  << "DataProxy_SQLite::getFoundInLog: Query OK"  << endl;
        QSqlRecord rec = query.record();
        int nameCol = -1;
        int counter = 0;
        while ((query.next()) && (searching))
        {
            if (query.isValid())
            {
                counter++;
                     //qDebug()  << "DataProxy_SQLite::getFoundInLog: Data found: - "  << QString::number(counter) << endl;
                qsoDataFound.clear();

                nameCol = rec.indexOf("call");
                qsoDataFound << "call:" + (query.value(nameCol)).toString();
                     //qDebug()  << "DataProxy_SQLite::getFoundInLog: Data found: " << (query.value(nameCol)).toString() << endl;

                nameCol = rec.indexOf("qso_date");
                qsoDataFound << "qso_date:" + (query.value(nameCol)).toString();

                nameCol = rec.indexOf("time_on");
                qsoDataFound << "time_on:" + (query.value(nameCol)).toString();

                nameCol = rec.indexOf("bandid");
                qsoDataFound << "bandid:" + (query.value(nameCol)).toString();

                nameCol = rec.indexOf("modeid");
                qsoDataFound << "modeid:" + (query.value(nameCol)).toString();

                nameCol = rec.indexOf("qsl_rcvd");
                aux = (query.value(nameCol)).toString();
                if (aux.length()<1)
                {
                    aux = "N";
                }
                qsoDataFound << "qsl_rcvd:" + aux;

                nameCol = rec.indexOf("qsl_sent");
                aux = (query.value(nameCol)).toString();
                if (aux.length()<1)
                {
                    aux = "N";
                }
                qsoDataFound << "qsl_sent:" + aux;

                nameCol = rec.indexOf("station_callsign");
                qsoDataFound << "station_callsign:" + (query.value(nameCol)).toString();

                nameCol = rec.indexOf("id");
                qsoDataFound << "id:" + (query.value(nameCol)).toString();
                if (qsoDataFound.length()>1)
                {
                 emit qsoFound(qsoDataFound);
                      //qDebug()  << "DataProxy_SQLite::getFoundInLog: Emitting qsoDataFound... (exec: "<< QString::number(executionN) << ") Call-id: " << qsoDataFound.at(0)  << "-" << qsoDataFound.at(8) << endl;
                }
                else
                {
                         //qDebug()  << "DataProxy_SQLite::getFoundInLog: NOT emitting qsoDataFound... (exec: "<< QString::number(executionN) << ")" << endl;
                         //qDebug()  << "DataProxy_SQLite::getFoundInLog: NOT Emitting qsoDataFound..."  << endl;
                }

            } // Closes if next.isValid
        } // Closes While
             //qDebug()  << "DataProxy_SQLite::getFoundInLog: No more data!"  << endl;
        return;
    }
    else
    {
             //qDebug()  << "DataProxy_SQLite::getFoundInLog: Error 1: Query NOK"  << endl;
        searching = false;
      return;
    }
         //qDebug()  << "DataProxy_SQLite::getFoundInLog: Error 2"  << endl;
    searching = false;
    return;
    */
}

/*
bool DataProxy_SQLite::queryPrepare(const QString &_query)
{
       //qDebug()  << "DataProxy_SQLite::queryPrepare: " << _query << endl;

    return true;
}

bool DataProxy_SQLite::queryBind(const QString &_field, const QString &value)
{
       //qDebug()  << "DataProxy_SQLite::queryBind: " << _field << "/" << value << endl;
    //preparedQuery.bindValue( _field, value );
    //db->queryBind(_field, value);


    return true;
}


bool DataProxy_SQLite::queryExec()
{
       //qDebug()  << "DataProxy_SQLite::queryExec  "  << endl;
    bool sqlOK = db->queryExec();


    if (!sqlOK)
    {
        //emit queryError(Q_FUNC_INFO, preparedQuery.lastError().databaseText(), preparedQuery.lastError().number(), preparedQuery.lastQuery());
           //qDebug()  << "DataProxy_SQLite::queryExec - FAILED execution "   << endl;
    }
    else
    {
           //qDebug()  << "DataProxy_SQLite::queryExec - executed "  << endl;
    }
    return sqlOK;
}
*/

int DataProxy_SQLite::getPrefixId(const QString &_qrz)
{
       //qDebug() << "DataProxy_SQLite::getPrefixId: -" << _qrz <<"-" << endl;
    //TODO: Instead of going from long to short, identify prefixes from the begining:
    // character(may be number) + number

    if (_qrz.length() < 1)
    {
        return -1;
    }
    int entityID = 0;

    QString aux = changeSlashAndFindPrefix((_qrz).toUpper());

    while ((entityID <= 0) && (aux.length()>=1) )
    {
        entityID = getDXCCFromPrefix(aux);

            //qDebug() << "DataProxy_SQLite::getPrefixId: in the while" << aux << " = " <<  QString::number(entityID) << endl;
         if (entityID<=0)
         {
             aux.chop(1);
         }

    }
        //qDebug() << "DataProxy_SQLite::getPrefixId: " <<  _qrz << QString::number(entityID) << endl;
    return entityID;
}

QString DataProxy_SQLite::changeSlashAndFindPrefix(const QString &_qrz)
{
       //qDebug() << "DataProxy_SQLite::changeSlashAndFindPrefix: -"  << _qrz <<"-" << endl;
    int iaux1, iaux2;

    QString aux = _qrz.toUpper();

    if ((aux).count('\\')) // Replaces \ by / to ease operation.
    {
        aux.replace(QChar('\\'), QChar('/'));
    }
    else
    {
        return aux;
    }

    if (aux.count('/')) // / found! Checking different options
    {

        if (aux.endsWith("/") )
        { // We look for calls ending in slash "/" or "\"
            aux.remove(aux.length()-1,1);
        }
        iaux1 = aux.indexOf('/');
            //qDebug() << "DataProxy_SQLite::changeSlashAndFindPrefix: Slash found at: "  << QString::number(iaux1) << endl;

        iaux2 = (aux.length())- iaux1; // iaux2 is the length of the second part
        if (iaux2 < 0){
            iaux2 = -iaux2;
        }

        if ( iaux1 < iaux2 ) { //Like in F/EA0K, we can simply take the first part as the prefix
            aux = aux.left(iaux1);
        }
        else
        {
            aux = aux.right(iaux2 -1);
        }
    }
    return aux;
}

void DataProxy_SQLite::slotCaptureDebugLogs(const QString &_func, const QString &_msg, const DebugLogLevel _level)
{
   emit debugLog(_func, _msg, _level);
}

QString DataProxy_SQLite::getADIFQSO(const int _qsoId)
{
    QString ADIFqso;
    ADIFqso.clear();

    //qDebug() << "DataProxy_SQLite::getADIFQSO: " <<  QString::number(_qsoId) << endl;
    int nameCol;
    QString aux;
    bool propsat = false;    // Reset the QSO in case it is a Satellite QSO

    QSqlQuery query;
    QString queryString = QString("SELECT * FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOk = query.exec(queryString);

    if (sqlOk)
    {
        if (query.next())
        {
            if (query.isValid())
            {
              //qDebug() << "DataProxy_SQLite::getADIFQSO: Query OK: " << query.lastQuery() << endl;
            }
            else
            {
                //qDebug() << "DataProxy_SQLite::getADIFQSO: Query isValid FAILED: " << query.lastQuery() << endl;
                query.finish();
                return QString();
            }

        }
        else
        {
            //qDebug() << "DataProxy_SQLite::getADIFQSO: Query NEXT FAILED: " << query.lastQuery() << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getADIFQSO: Query FAILED: " << query.lastQuery() << endl;
        query.finish();
        return QString();
    }
    QSqlRecord rec = query.record();



    nameCol = rec.indexOf("call");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        //qDebug() << "DataProxy_SQLite::getADIFQSO: " << QString::number(nameCol) << "/" << aux1 << endl;
        if (util->isValidCall(aux))
        {
            ADIFqso.append("<CALL:" + QString::number(aux.length()) + ">" + aux + " ");
        }
        else
        {
            if (showInvalidCallMessage(aux))
            {
                 ADIFqso.append("<CALL:" + QString::number(aux.length()) + ">" + aux + " ");
            }
        }
    }
    nameCol = rec.indexOf("qso_date");
    QDateTime tDateTime;
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        tDateTime = util->getDateTimeFromSQLiteString(aux);
        if (tDateTime.isValid())
        {
            aux = util->getADIFDateFromQDateTime(tDateTime);
            ADIFqso.append("<QSO_DATE:" + QString::number(aux.length()) + ">" + aux + " ");
            aux = util->getADIFTimeFromQDateTime(tDateTime);
            //aux.chop(2);
            ADIFqso.append("<TIME_ON:" + QString::number(aux.length()) + ">" + aux + " ");
         }
    }

    nameCol = rec.indexOf("bandid");
    QString bandst = QString();
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        aux = getNameFromBandId(aux.toInt());

        if (getIdFromBandName(aux)>=0)
        {
          bandst = aux;
        }
    }
    nameCol = rec.indexOf("freq");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);

        double freqTX = aux.toDouble();
        if (freqTX > 0.0)
        {
            //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
            if (getBandIdFromFreq(freqTX) != getIdFromBandName(bandst))
            {
                bandst = getBandNameFromFreq(freqTX);
            }
            ADIFqso.append("<FREQ:" + QString::number(aux.length()) + ">" + aux  + " ");


        }
        if (getIdFromBandName(bandst)>0)
        {
            ADIFqso.append("<BAND:" + QString::number(bandst.length()) + ">" + bandst  + " ");
        }
    }
    // Now the BAND RX
    nameCol = rec.indexOf("band_rx");
    QString bandrxst = QString();
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        aux = getNameFromBandId(aux.toInt());

        if (getIdFromBandName(aux)>=0)
        {
            bandrxst = aux;
        }
    }
    nameCol = rec.indexOf("freq_rx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        double freqRX = aux.toDouble();
        if (freqRX > 0.0)
        {
        //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
            if (getBandIdFromFreq(freqRX) != getIdFromBandName(bandrxst))
            {
                bandrxst = getBandNameFromFreq(freqRX);
            }
            ADIFqso.append("<FREQ_RX:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
        if (getIdFromBandName(bandrxst)>0)
        {
            ADIFqso.append("<BAND_RX:" + QString::number(bandrxst.length()) + ">" + bandrxst  + " ");
        }
        // END of Band RX
    }
    nameCol = rec.indexOf("modeid");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        // get SubModeId to check if it is the same or not from modeid
        QString aux2 = getSubModeFromId(aux.toInt());
        //aux = db->getModeNameFromID2(aux.toInt());
        aux = getNameFromSubMode(aux2);
        if ((aux.length()>1) && (getIdFromModeName(aux)>=0))
        {
            ADIFqso.append("<MODE:" + QString::number(aux.length()) + ">" + aux + " ");
        }
        if ((aux != aux2) && (aux.length()>1) && (getSubModeIdFromSubMode(aux2)>=0) )
        {
            ADIFqso.append("<SUBMODE:" + QString::number(aux2.length()) + ">" + aux2  +  " ");
        }
    }
    nameCol = rec.indexOf("prop_mode");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>1)
        {
            ADIFqso.append("<PROP_MODE:" + QString::number(aux.length()) + ">" + aux  + " ");
            if (aux == "SAT")
            {
                propsat = true;
            }
        }
        //qDebug() << "DataProxy_SQLite::getADIFQSO: PROP_MODE"  << endl;
    }
    nameCol = rec.indexOf("sat_name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<SAT_NAME:" + QString::number(aux.length()) + ">" + aux  + " ");
            if (!propsat)
            {
                ADIFqso.append("<PROP_MODE:3>SAT ");
                propsat = false;
            }
        }
    //qDebug() << "DataProxy_SQLite::getADIFQSO: SAT_NAME"  << endl;
    }
    nameCol = rec.indexOf("gridsquare");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<GRIDSQUARE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_gridsquare");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_GRIDSQUARE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("station_callsign");
    if ((nameCol>=0) )
    {
        aux = (query.value(nameCol)).toString();
        //qDebug() << "DataProxy_SQLite::getADIFQSO: StationCallSign: " << aux  << endl;
        if ((util->isValidCall(aux)))
        { // User selected one station callsign from the log
            ADIFqso.append("<STATION_CALLSIGN:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qso_date_off");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        tDateTime = util->getDateTimeFromSQLiteString(aux);

        if (tDateTime.isValid())
        {
            aux = util->getADIFDateFromQDateTime(tDateTime);
            ADIFqso.append("<QSO_DATE_OFF:" + QString::number(aux.length()) + ">" + aux  + " ");
            aux = util->getADIFTimeFromQDateTime(tDateTime);
            ADIFqso.append("<TIME_OFF:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("srx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<SRX:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }

    nameCol = rec.indexOf("srx_string");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<SRX_STRING:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("stx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<STX:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("stx_string");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<STX_STRING:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    //qDebug() << "DataProxy_SQLite::getADIFQSO - 100" << endl;
    nameCol = rec.indexOf("cqz");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())>0) && (0 < aux.toInt()) && (aux.toInt() < CQZones+1) )
        {
            ADIFqso.append("<CQZ:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("ituz");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())>0) && (0 < aux.toInt()) && (aux.toInt() < ITUZones+1) )
        {
            ADIFqso.append("<ITUZ:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
        //qDebug() << "DataProxy_SQLite::getADIFQSO: DXCC - Now..."  << endl;
    }
    nameCol = rec.indexOf("dxcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<DXCC:" + QString::number(aux.length()) + ">" + aux  + " ");
            //qDebug() << "DataProxy_SQLite::getADIFQSO: DXCC " << aux << endl;
        }
        //qDebug() << "DataProxy_SQLite::getADIFQSO: DXCC - Exported!"  << endl;
    }
    nameCol = rec.indexOf("address");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<ADDRESS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("age");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<AGE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("cnty");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<CNTY:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("comment");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<COMMENT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
        //qDebug() << "DataProxy_SQLite::getADIFQSO - 200" << endl;
    }
    nameCol = rec.indexOf("a_index");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<A_INDEX:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("ant_az");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<ANT_AZ:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("ant_el");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<ANT_EL:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("ant_path");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<ANT_PATH:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("arrl_sect");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<ARRL_SECT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("checkcontest");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<CHECKCONTEST:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    //qDebug() << "DataProxy_SQLite::getADIFQSO - 30" << endl;
    nameCol = rec.indexOf("class");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<CLASS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("cont");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<CONT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("contacted_op");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            ADIFqso.append("<CONTACTED_OP:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("contest_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<CONTEST_ID:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("points");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<APP_KLOG_POINTS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("multiplier");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<APP_KLOG_MULTIPLIER:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
        //qDebug() << "DataProxy_SQLite::getADIFQSO - 40" << endl;
    }
    nameCol = rec.indexOf("transmiterid");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<APP_KLOG_TRX:" + QString::number(aux.length()) + ">" + aux  + " ");
        }

    }
    nameCol = rec.indexOf("country");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<COUNTRY:" + QString::number(aux.length()) + ">" + aux  + " ");
        }

    }
    nameCol = rec.indexOf("credit_submitted");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<CREDIT_SUBMITTED:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("credit_granted");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<CREDIT_GRANTED:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("distance");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<DISTANCE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("darc_dok");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<DARC_DOK:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("eq_call");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            ADIFqso.append("<EQ_CALL:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("email");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            if (aux.contains("@") && (aux.contains(".")))
            {
                ADIFqso.append("<EMAIL:" + QString::number(aux.length()) + ">" + aux  + " ");
            }
        }
    }
    nameCol = rec.indexOf("eqsl_qslrdate");
    QDate date;
    if (nameCol>=0)
    {
            aux = (query.value(nameCol)).toString();
            date = util->getDateFromSQliteString(aux);
            if (date.isValid())
            {
                aux = util->getADIFDateFromQDate(date);
                ADIFqso.append("<EQSL_QSLRDATE:" + QString::number(aux.length()) + ">" + aux  + " ");
            }
    }
    nameCol = rec.indexOf("eqsl_qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<EQSL_QSLSDATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("eqsl_qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (  ((aux.length())==1) && (aux!="N") )
        {
            ADIFqso.append("<EQSL_QSL_RCVD:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("eqsl_qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (  ((aux.length())==1) && (aux!="N") )
        {
            ADIFqso.append("<EQSL_QSL_SENT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("fists");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<FISTS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("fists_cc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<FISTS_CC:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("force_init");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<FORCE_INIT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("guest_op");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<GUEST_OP:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("hrdlog_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<HRDLOG_QSO_UPLOAD_DATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("hrdlog_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && ((aux!="Y") || (aux!="N") || (aux!="M")) )
        {
            ADIFqso.append("<HRDLOG_QSO_UPLOAD_STATUS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_antenna");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_ANTENNA:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_dxcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_DXCC:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_fists");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_FISTS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("iota");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        //qDebug() << "DataProxy_SQLite::getADIFQSO (IOTA): " << aux << endl;
        if (((aux.length())>=4) && ((aux.length())<=6))
        {
            ADIFqso.append("<IOTA:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("iota_island_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        //qDebug() << "DataProxy_SQLite::getADIFQSO (IOTA_ID): " << aux << endl;
        if ((aux.length())>0)
        {
            ADIFqso.append("<IOTA_ISLAND_ID:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_iota");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())>=4) && ((aux.length())<=6))
        {
            ADIFqso.append("<MY_IOTA:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }

    nameCol = rec.indexOf("my_iota_island_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_IOTA_ISLAND_ID:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("k_index");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<K_INDEX:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_itu_zone");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_ITU_ZONE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("lat");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<LAT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("lon");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<LON:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
     }
    nameCol = rec.indexOf("my_lat");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_LAT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
     }
    nameCol = rec.indexOf("my_lon");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_LON:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
     }
    nameCol = rec.indexOf("lotw_qslrdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<LOTW_QSLRDATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("lotw_qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<LOTW_QSLSDATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }

    }
    nameCol = rec.indexOf("lotw_qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1) && (aux!="N") )
        {
            ADIFqso.append("<LOTW_QSL_RCVD:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("lotw_qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && (aux!="N") )
        {
            ADIFqso.append("<LOTW_QSL_SENT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }

    }
    nameCol = rec.indexOf("clublog_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<CLUBLOG_QSO_UPLOAD_DATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("clublog_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && ((aux!="Y") || (aux!="N") || (aux!="M")) )
        {
            ADIFqso.append("<CLUBLOG_QSO_UPLOAD_STATUS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qrzcom_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<QRZCOM_QSO_UPLOAD_DATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qrzcom_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && ((aux!="Y") || (aux!="N") || (aux!="M")) )
        {
            ADIFqso.append("<QRZCOM_QSO_UPLOAD_STATUS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("max_bursts");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MAX_BURSTS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("ms_shower");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MS_SHOWER:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_city");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_CITY:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_cnty");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_CNTY:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_country");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_COUNTRY:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_cq_zone");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.toInt()>0) && (aux.toInt()<41))
        {
            ADIFqso.append("<MY_CQ_ZONE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_NAME:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<NAME:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("operator");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            ADIFqso.append("<OPERATOR:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("owner_callsign");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            ADIFqso.append("<OWNER_CALLSIGN:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_postal_code");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_POSTAL_CODE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_rig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_RIG:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_sig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_SIG:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_sota_ref");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_SOTA_REF:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_postal_code");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_POSTAL_CODE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_state");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_STATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_street");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<MY_STREET:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("notes");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            aux.replace("\n", "---");
            ADIFqso.append("<NOTES:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("nr_bursts");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<NR_BURSTS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("nr_pings");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<NR_PINGS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("pfx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<PFX:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("precedence");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<PRECEDENCE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("public_key");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<PUBLIC_KEY:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qslmsg");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<QSLMSG:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qslrdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<QSLRDATE:" + QString::number(aux.length()) + ">" + aux + " ");
        }
    }
    nameCol = rec.indexOf("qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            ADIFqso.append("<QSLSDATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())==1) && (aux!="N") )
        {
            ADIFqso.append("<QSL_RCVD:" + QString::number(aux.length()) + ">" + aux  + " ");

            nameCol = rec.indexOf("qsl_rcvd_via");
            aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
            if ((aux.length())==1)
            {
                ADIFqso.append("<QSL_RCVD_VIA:" + QString::number(aux.length()) + ">" + aux  + " ");
            }
        }
    }
    nameCol = rec.indexOf("qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())==1) && (aux!="N") )
        {
            ADIFqso.append("<QSL_SENT:" + QString::number(aux.length()) + ">" + aux  + " ");
            nameCol = rec.indexOf("qsl_sent_via");
            aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
            if ((aux.length())==1)
            {
                ADIFqso.append("<QSL_SENT_VIA:" + QString::number(aux.length()) + ">" + aux  + " ");
            }
        }
    }
    nameCol = rec.indexOf("qsl_via");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<QSL_VIA:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qso_complete");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<QSO_COMPLETE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qso_random");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<QSO_RANDOM:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("qth");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<QTH:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("rst_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<RST_SENT:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("rst_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<RST_RCVD:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("region");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<REGION:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("rig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<RIG:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("rx_pwr");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())>0) && (aux.toDouble()>0) )
        {
            ADIFqso.append("<RX_PWR:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("tx_pwr");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())>0) && (aux.toDouble()>0))
        {
            ADIFqso.append("<TX_PWR:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("sat_mode");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            ADIFqso.append("<SAT_MODE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("sfi");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<SFI:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("sig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<SIG:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("sig_info");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<SIG_INFO:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("silent_key");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<SILENT_KEY:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("skcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<SKCC:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("sota_ref");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<SOTA_REF:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("state");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<STATE:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("swl");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<SWL:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("ten_ten");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<TEN_TEN:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("ten_ten");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<TEN_TEN:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("uksmg");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<UKSMG:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
        }
    nameCol = rec.indexOf("ve_prov");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<VE_PROV:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_usaca_counties");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<MY_USACA_COUNTIES:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("usaca_counties");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<USACA_COUNTIES:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("vucc_grids");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<VUCC_GRIDS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("my_vucc_grids");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<MY_VUCC_GRIDS:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }
    nameCol = rec.indexOf("web");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            ADIFqso.append("<WEB:" + QString::number(aux.length()) + ">" + aux  + " ");
        }
    }

        nameCol = rec.indexOf("lognumber");
        if (nameCol>=0)
        {
            aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
            if ((aux.length())>0)
            {
                ADIFqso.append("<APP_KLOG_LOGN:" + QString::number(aux.length()) + ">" + aux  + " ");
            }
        }

    ADIFqso.append("<EOR>");



    return ADIFqso;
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
    //qDebug() << Q_FUNC_INFO << ": log = " << QString::number(_log) << endl;

    QList<QSO*> _qsos;
    QString stringQuery;
    if (doesThisLogExist(_log))
    {
        //qDebug() << Q_FUNC_INFO << ": log exists "  << endl;
        //stringQuery = QString("SELECT call, qso_date, bandid, modeid, dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log where dxcc <>''  AND sat_name <>'' AND lognumber='%1' group by dxcc").arg(_log);
       //stringQuery = QString("SELECT call, qso_date, bandid, modeid, dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log where dxcc <>''  AND sat_name <>'' AND lognumber='%1'").arg(_log);
        stringQuery = QString("SELECT call, qso_date, band.name, mode.name, entity.name, log.dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log, entity, band, mode where log.dxcc <>''  AND sat_name <>'' AND log.dxcc=entity.dxcc AND log.bandid=band.id AND log.modeid=mode.id AND lognumber='%1' ORDER BY entity.name").arg(_log);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": log does not exist "  << endl;
        stringQuery = QString("SELECT call, qso_date, band.name, mode.name, entity.name, log.dxcc, lotw_qsl_rcvd, qsl_rcvd, sat_name from log, entity, band, mode where log.dxcc <>''  AND sat_name <>'' AND log.dxcc=entity.dxcc AND log.bandid=band.id AND log.modeid=mode.id ORDER BY entity.name");
    }

    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << ":  Query NOK" << endl;
        //qDebug() << Q_FUNC_INFO << ":  " << query.lastError().databaseText() << endl;
        //qDebug() << Q_FUNC_INFO << ":  " << query.lastQuery() << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
                //qDebug() << "DataProxy_SQLite::getGridStats: bandid" << QString::number((query.value(nameCol)).toInt()) << endl;
                _qso->setBand(query.value(2).toString());

                //nameCol = rec.indexOf("modeid");
                //qDebug() << Q_FUNC_INFO << ": modeid" << QString::number((query.value(nameCol)).toInt()) << endl;
                _qso->setMode(query.value(3).toString());

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

    //qDebug() << Q_FUNC_INFO << "- END" << endl;
    return _qsos;
}

QList<QSO *> DataProxy_SQLite::getSatGridStats(int _log)
{
    //qDebug() << "DataProxy_SQLite::getGridStats: log = " << QString::number(_log) << endl;

    QList<QSO*> _qsos;
    QString stringQuery;
    if (doesThisLogExist(_log))
    {
        //qDebug() << "DataProxy_SQLite::getGridStats: log exists "  << endl;
        stringQuery = QString("SELECT call, qso_date, bandid, modeid, substr(gridsquare, 1, 4), lotw_qsl_rcvd, qsl_rcvd, sat_name from log where gridsquare <>''  AND sat_name <>'' AND lognumber='%1'").arg(_log);
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::getGridStats: log does not exist "  << endl;
        stringQuery = QString("SELECT call, qso_date, bandid, modeid, substr(gridsquare, 1, 4), lotw_qsl_rcvd, qsl_rcvd, sat_name from log where gridsquare <>''  AND sat_name <>''");
    }


    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        //qDebug() << "DataProxy_SQLite::getGridStats:  Query NOK" << endl;
        //qDebug() << "DataProxy_SQLite::getGridStats:  " << query.lastError().databaseText() << endl;
        //qDebug() << "DataProxy_SQLite::getGridStats:  " << query.lastQuery() << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
                //qDebug() << "DataProxy_SQLite::getGridStats: bandid" << QString::number((query.value(nameCol)).toInt()) << endl;
                _qso->setBand(getNameFromBandId((query.value(nameCol)).toInt()));

                nameCol = rec.indexOf("modeid");
                //qDebug() << "DataProxy_SQLite::getGridStats: modeid" << QString::number((query.value(nameCol)).toInt()) << endl;
                _qso->setMode(getNameFromModeId((query.value(nameCol)).toInt()));

                nameCol = rec.indexOf("sat_name");
                _qso->setSatName((query.value(nameCol)).toString());

                nameCol = rec.indexOf("substr(gridsquare, 1, 4)");
                _qso->setGridSquare((query.value(nameCol)).toString());

                nameCol = rec.indexOf("lotw_qsl_rcvd");
                _qso->setLoTWQSL_RCVD((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qsl_rcvd");
                _qso->setQSL_RCVD((query.value(nameCol)).toString());

                _qsos.append(_qso);
                //qDebug() << "DataProxy_SQLite::getGridStats: call: " << _call << endl;
                //qDebug() << "DataProxy_SQLite::getGridStats: band: " << _band << endl;
                //qDebug() << "DataProxy_SQLite::getGridStats: mode: " << _mode << endl;

            }
            else
            {
                _qsos.clear();
                query.finish();
                return _qsos;
            }
        }
        //qDebug() << "DataProxy_SQLite::addQSO:  Query OK" << endl;
    }

    //qDebug() << "DataProxy_SQLite::getGridStats - END" << endl;
    return _qsos;
}

int DataProxy_SQLite::addQSO(QSO &_qso)
{
   //qDebug() << "DataProxy_SQLite::addQSO: " << _qso.getCall() << endl;
    qso = &_qso;
   //qDebug() << "DataProxy_SQLite::addQSO: " << qso->getCall() << endl;
    //qso = _qso;

    bool havePropMode = false;
    bool haveRSTRX = false;
    bool haveRSTTX = false;
    QString stringQuery;
    QString stringFields, stringData;
    stringFields.clear();
    stringData.clear();


   //qDebug() << "DataProxy_SQLite::addQSO: QDateTime: " << util->getDateTimeSQLiteStringFromDateTime(qso->getDateTimeOn())  << endl;
    if (qso->getDateTimeOn().isValid())
    {
        stringFields.append("qso_date, ");
        stringData = stringData + "'" + util->getDateTimeSQLiteStringFromDateTime(qso->getDateTimeOn()) + "', ";
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSO:  END-1" << endl;
        return -1;
    }

    if (util->isValidCall(qso->getCall()))
    {
        stringFields.append("call, ");
        stringData = stringData + "'" + qso->getCall() + "', ";
    }
    else
    {
       //qDebug() << "DataProxy_SQLite::addQSO:  END-2" << endl;
        return -2;
    }
   //qDebug() << "DataProxy_SQLite::addQSO: Band: " << qso->getBand() << endl;
    if (util->isValidBandId(getIdFromBandName(qso->getBand())))
    {
        stringFields.append("bandid, ");
        stringData = stringData + "'" + QString::number(getIdFromBandName(qso->getBand())) + "', ";
    }
    else
    {
       //qDebug() << "DataProxy_SQLite::addQSO:  END-3" << endl;
        return -3;
    }
    if (util->isValidModeId(getIdFromModeName(qso->getMode())))
    {
        stringFields.append("modeid, ");
        stringData = stringData + "'" + QString::number(getIdFromModeName(qso->getMode())) + "', ";
    }
    else
    {
       //qDebug() << "DataProxy_SQLite::addQSO:  END-4" << endl;
        return -4;
    }
    if (util->isValidBandId(getIdFromBandName(qso->getBandRX())))
    {
        stringFields.append("band_rx, ");
        stringData = stringData + "'" + QString::number(getIdFromBandName(qso->getBandRX())) + "', ";
    }
    if (qso->getFreqTX () > 0)
    {
        stringFields.append("freq, ");
        stringData = stringData + "'" + QString::number(qso->getFreqTX()) + "', ";
    }
    if (qso->getFreqRX() > 0)
    {
        stringFields.append("freq_rx, ");
        stringData = stringData + "'" + QString::number(qso->getFreqRX()) + "', ";
    }
    if (util->isValidCall(qso->getStationCallsign()))
    {
        stringFields.append("station_callsign, ");
        stringData = stringData + "'" + qso->getStationCallsign() + "', ";
    }
    if (qso->getQSLRDate().isValid())
    {
        stringFields.append("qslrdate, ");
        stringData = stringData + "'" + util->getDateSQLiteStringFromDate(qso->getQSLRDate()) + "', ";
    }
    //qDebug() << "DataProxy_SQLite::addQSO:  getQSL_RCVD: " << qso->getQSL_RCVD() << endl;
    if (util->isValidQSL_Rcvd(qso->getQSL_RCVD()))
    {
        //qDebug() << "DataProxy_SQLite::addQSO:  getQSL_RCVD: VALID "  << endl;
        stringFields.append("qsl_rcvd, ");
        stringData = stringData + "'" + qso->getQSL_RCVD() + "', ";
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSO:  getQSL_RCVD: NOT valid"  << endl;
    }
    //qDebug() << "DataProxy_SQLite::addQSO:  LOTW_QSL_RCVD: " << qso->getLoTWQSL_RCVD()  << endl;
    if (util->isValidQSL_Rcvd(qso->getLoTWQSL_RCVD()))
    {
        //qDebug() << "DataProxy_SQLite::addQSO:  LOTW_QSL_RCVD - VALID"  << endl;
        stringFields.append("lotw_qsl_rcvd, ");
        stringData = stringData + "'" + qso->getLoTWQSL_RCVD() + "', ";
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSO:  LOTW_QSL_RCVD - NOT valid"  << endl;
    }
    //qDebug() << "DataProxy_SQLite::addQSO:  LOTW_QSL_RCVD - END"  << endl;

    if (qso->getLoTWQSLRDate().isValid())
    {
        stringFields.append("lotw_qslrdate, ");
        stringData = stringData + "'" + util->getDateSQLiteStringFromDate(qso->getLoTWQSLRDate()) + "', ";
    }
    if (qso->getLoTWQSLSDate().isValid())
    {
        stringFields.append("lotw_qslsdate, ");
        stringData = stringData + "'" + util->getDateSQLiteStringFromDate(qso->getLoTWQSLSDate()) + "', ";
    }
    if ( getSatelliteName(qso->getSatName()).length()>0 )
    {
        stringFields.append("sat_name, ");
        stringData = stringData + "'" + qso->getSatName() + "', ";
        if (!havePropMode)
        {
            havePropMode = true;
            stringFields.append("prop_mode, ");
            stringData = stringData + "'SAT', ";
        }
    }
    if ( !havePropMode && (qso->getPropMode().length()>0) )
    {
        stringFields.append("prop_mode, ");
        stringData = stringData + "'" + qso->getPropMode() + "', ";
        havePropMode = true;
    }
    if (!haveRSTRX)
    {
        stringFields.append("rst_rcvd, ");
        stringData = stringData + "'" + util->getDefaultRST(qso->getMode())+ "', ";
    }
    if (!haveRSTTX)
    {
        stringFields.append("rst_sent, ");
        stringData = stringData + "'" + util->getDefaultRST(qso->getMode())+ "', ";
    }

    if (qso->getLogId()>0)
    {
        stringFields.append("lognumber");
        stringData = stringData + QString::number(qso->getLogId());
    }
    else
    {
       //qDebug() << "DataProxy_SQLite::addQSO: No LogNumber" << endl;
        return -2;
    }

    stringQuery = "INSERT INTO log (" + stringFields  + ") values (" + stringData +")" ;
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
       //qDebug() << "DataProxy_SQLite::addQSO:  Query NOK" << endl;
       //qDebug() << "DataProxy_SQLite::addQSO:  " << query.lastError().databaseText() << endl;
       //qDebug() << "DataProxy_SQLite::addQSO:  " << query.lastQuery() << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::addQSO:  Query OK" << endl;
    }
   //qDebug() << "DataProxy_SQLite::addQSO: " << stringQuery << endl;
    return getLastQSOid();


    //return 1;
}

