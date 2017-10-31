/***************************************************************************
                          dataproxy_sqlite.cpp  -  description
                             -------------------
    begin                : sept 2014
    copyright            : (C) 2014 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify         *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "dataproxy_sqlite.h"
//#include <QDebug>

DataProxy_SQLite::DataProxy_SQLite()
{
    //qDebug() << "DataProxy_SQLite::DataProxy_SQLite" << endl;
    db = new DataBase(0);
    //qDebug() << "DataProxy_SQLite::DataProxy_SQLite 1" << endl;
    dbCreated = db->createConnection();
    //dbCreated = db->createBandModeMaps();
    //qDebug() << "DataProxy_SQLite::DataProxy_SQLite - END" << endl;
    searching = false;
    executionN = 0;


}
DataProxy_SQLite::~DataProxy_SQLite(){
      //qDebug() << "DataProxy_SQLite::~DataProxy_SQLite" << endl;
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
        return -3;
    }
    return db->getModeIDFromName2(_modeName);

}

int DataProxy_SQLite::getSubModeIdFromSubMode(const QString _subModeName)
{
    //qDebug() << "DataProxy_SQLite::getSubModeIdFromSubMode: " << _subModeName << endl;

    if (_subModeName.length()<2)
    {
        return -3;
    }
    QSqlQuery query;
    QString stQuery = QString("SELECT id FROM mode WHERE submode='%1'").arg(_subModeName);
    if (query.exec(stQuery))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -2;
    }
}

int DataProxy_SQLite::getModeIdFromSubModeId(const int _sm)
{
    return getIdFromModeName(getNameFromSubMode(getSubModeFromId(_sm)));

}


bool DataProxy_SQLite::isModeDeprecated (const QString _sm)
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
               return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false; // In case we can't check, we don't state it as deprecated
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;   // In case we can't check, we don't state it as deprecated
    }
}

int DataProxy_SQLite::getIdFromBandName(const QString& _bandName)
{
     //qDebug() << "DataProxy_SQLite::getIdFromBandName: " << _bandName  << "/" << QString::number(db->getBandIDFromName2(_bandName))<< endl;
    if (_bandName.length()<1)
    {
        return -3;
    }
    return db->getBandIDFromName2(_bandName);
}

QString DataProxy_SQLite::getNameFromBandId (const int _id)
{
      //qDebug() << "DataProxy_SQLite::getNameFromBandId " << endl;
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
    QString stringQuery = QString("SELECT submode, name, deprecated FROM mode WHERE id='%1'").arg(_id);
    bool sqlOK = query.exec(stringQuery);
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
    QString stringQuery = QString("SELECT submode FROM mode WHERE id='%1'").arg(_id);
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toString();
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


    return QString();
}

QString DataProxy_SQLite::getNameFromSubMode (const QString _sm)
{
    QSqlQuery query;
    QString stringQuery = QString("SELECT name FROM mode WHERE submode='%1'").arg(_sm.toUpper());
    //QString stringQuery = QString("SELECT name, deprecated FROM mode WHERE submode='%1'").arg(_sm.toUpper());
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toString();

        }
        else
        {
            return QString();
        }
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }
}

QString DataProxy_SQLite::getFreqFromBandId(const int _id)
{
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
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
          //qDebug() << "DataProxy_SQLite::getBandIdFromFreq: Query NOK" << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
       return -1;
    }
    return -1;
}

QString DataProxy_SQLite::getBandNameFromFreq(const double _n)
{
    return getNameFromBandId(getBandIdFromFreq(_n));
}

double DataProxy_SQLite::getLowLimitBandFromBandName(const QString _sm)
{
      //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: " << _sm << endl;
    QSqlQuery query;
    QString stringQuery = QString("SELECT lower FROM band WHERE name='%1' or name='%2'").arg(_sm).arg(_sm.toUpper());
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(0)).toDouble()<0 )
            {
                  //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: -1.0-1" << endl;
                return -1.0;
            }
            else
            {
                  //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName(else): " << QString::number((query.value(0)).toDouble()) << endl;
                return (query.value(0)).toDouble();
            }
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: -1.0-2" << endl;
            return -1.0;
        }

          //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandName: -1.0-3" << endl;
        return -1.0;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1.0;
    }
    return -1.0;
}


double DataProxy_SQLite::getLowLimitBandFromBandId(const QString _sm)
{
      //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId" << endl;
    QSqlQuery query;
    QString stringQuery = QString("SELECT lower FROM band WHERE id='%1'").arg(_sm.toUpper());
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(1)).toDouble()<0 )
            {
                  //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId: -1.0-1" << endl;
                return -1.0;
            }
            else
            {
                  //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId: " << QString::number((query.value(0)).toDouble()) << endl;
                return (query.value(0)).toDouble();
            }
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId: -1.0-2" << endl;
            return -1.0;
        }

          //qDebug() << "DataProxy_SQLite::getLowLimitBandFromBandId: -1.0-3" << endl;
        return -1.0;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1.0;
    }
    return -1.0;

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
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT DISTINCT name from band");
    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                  //qDebug() << "DataProxy_SQLite::getBandNames: " << stringQuery << endl;
                bands.append(stringQuery);
            }
            else
            {
                return QStringList();
            }

        }
        return sortBandNamesBottonUp(bands);
        //return bands;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
    return modes;
}

QStringList DataProxy_SQLite::sortBandNamesBottonUp(const QStringList _qs)
{
    //Receives a list of band names, sorts it from the lower band to the upper band and returns
    //qDebug() << "DataProxy_SQLite::sortBandNamesBottonUp" << endl;

   //Next lines to be commented out
   for (int i=0; i<_qs.length();i++)
   {
         //qDebug() << "DataProxy_SQLite::sortBandNamesBottonUp - band: " << _qs.at(i) << endl;
   }
   //Previous lines to be commented out

    QMap<double, QString> map;
    QStringList qs;
    qs.clear();


    for (int j=0; j<_qs.count(); j++)
    {
        map.insert(getLowLimitBandFromBandName(_qs.at(j)), _qs.at(j));
    }

    QMap<double, QString>::const_iterator i = map.constBegin();

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
        map.insert(getLowLimitBandFromBandId(_qs.at(j)), _qs.at(j));
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
    return modes;
}


QStringList DataProxy_SQLite::getBandsInLog(const int _log)
{
      //qDebug() << "DataProxy_SQLite::getBandsInLog: " << endl;
    QStringList bands = QStringList();
    QString stringQuery = QString();
    if (_log <= 0)
    {
        stringQuery = QString("SELECT DISTINCT band.name FROM log, band WHERE band.id = log.bandid ORDER BY band.id DESC");
    }
    else
    {
        stringQuery = QString("SELECT DISTINCT band.name FROM log, band WHERE band.id = log.bandid AND log.lognumber='%1' ORDER BY band.id DESC").arg(_log);
    }

    QSqlQuery query(stringQuery);
    while (query.next()) {
        if (query.isValid()){
            bands << query.value(0).toString();
        }
    }
    return sortBandNamesBottonUp(bands);
    //return bands;
}

QStringList DataProxy_SQLite::getModesInLog(const int _log)
{
    //qDebug() << "DataProxy_SQLite::getModesInLog: " << endl;
    QStringList modes = QStringList();
    QString stringQuery = QString();
    if (_log <=0 )
    {
        stringQuery = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid GROUP BY mode.submode  ORDER BY count (mode.submode) DESC");
    }
    else
    {

        stringQuery = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid AND log.lognumber='%1' GROUP BY mode.submode  ORDER BY count (mode.submode) DESC").arg(_log);
    }

    QSqlQuery query(stringQuery);
    while (query.next()) {
        if (query.isValid()){
            modes << query.value(1).toString();
        }
    }
     //qDebug() << "DataProxy_SQLite::getModesInLog: " << modes.join(" - ") << endl;
    return modes;
}

int DataProxy_SQLite::getMostUsedBand(const int _log)
{
      //qDebug() << "DataProxy_SQLite::getMostUsedBand: " << endl;

    QString stringQuery = QString();
    if (_log <=0 )
    {
        stringQuery = QString("SELECT band.id, band.name, COUNT (band.name) FROM log, band WHERE band.id = log.bandid GROUP BY band.id  ORDER BY count (band.id) DESC LIMIT 1");
    }
    else
    {
        stringQuery = QString("SELECT band.id, band.name, COUNT (band.name) FROM log, band WHERE band.id = log.bandid AND log.lognumber='%1' GROUP BY band.id  ORDER BY count (band.id) DESC LIMIT 1").arg(_log);
    }
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                return query.value(0).toInt();
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1;
    }
    return -1;
}

int DataProxy_SQLite::getMostUsedMode(const int _log)
{
      //qDebug() << "DataProxy_SQLite::getMostUsedMode: " << endl;

    QString stringQuery = QString();
    if (_log <=0 )
    {
        stringQuery = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid GROUP BY mode.submode  ORDER BY count (mode.submode) DESC LIMIT 1");
    }
    else
    {
        stringQuery = QString("SELECT mode.id, mode.submode, COUNT (mode.submode) FROM log, mode WHERE mode.id = log.modeid AND log.lognumber='%1' GROUP BY mode.submode  ORDER BY count (mode.submode) DESC LIMIT 1").arg(_log);
    }
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                return query.value(0).toInt();
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1;
    }
    return -1;

}


int DataProxy_SQLite::getLastQSOid()
{
      //qDebug() << "DataProxy_SQLite::getLastQSOid" << endl;
    QSqlQuery query;
    if (query.exec("SELECT MAX(id) from log"))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1;
    }

}

bool DataProxy_SQLite::clearLog()
{
      //qDebug() << "DataProxy_SQLite::clearLog" << endl;
    int errorCode = 0;
    QSqlQuery query;
    if (query.exec("DELETE FROM log"))
    {
          //qDebug() << "DataProxy_SQLite::clearLog: Log deleted!" << endl;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::clearLog: Log deleted FAILED" << endl;
        //errorCode = query.lastError().number();
          //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }

    if (query.exec("DELETE FROM awarddxcc"))
    {
          //qDebug() << "DataProxy_SQLite::clearLog: Awarddxcc deleted!" << endl;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::clearLog: Awarddxcc deletedFAILED" << endl;
        //errorCode = query.lastError().number();
          //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
          //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }

    if (query.exec("DELETE FROM awardwaz"))
    {
          //qDebug() << "DataProxy_SQLite::clearLog: Awardwaz deleted!" << endl;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::clearLog: Awardwaz deleted FAILED" << endl;
        //errorCode = query.lastError().number();
          //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
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
        return false;
    }
    else
    {
        query.prepare("VACUUM;");

          //qDebug() << "DataProxy_SQLite::clearLog: Query Not Active!" << endl;
        if (query.exec())
        {
              //qDebug() << "DataProxy_SQLite::clearLog: VACUUM OK!" << endl;
            return true;
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
              //qDebug() << "DataProxy_SQLite::clearLog: VACUUM FAILED" << endl;
            //errorCode = query.lastError().number();
              //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
              //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
              //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
              //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
              //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
        }
    }
    return false;
}

bool DataProxy_SQLite::qslSentViaDirect(const int _qsoId, const QString _updateDate)
{
      //qDebug() << "DataProxy_SQLite::qslSentViaDirect" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'D', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
      //qDebug() << "DataProxy_SQLite::qslSentViaDirect: " << stringQuery << endl;
    if (query.exec(stringQuery))
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::qslSentViaBureau(const int _qsoId, const QString _updateDate)
{
      //qDebug() << "DataProxy_SQLite::qslSentViaBureau" << endl;

    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'B', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);

    if (query.exec(stringQuery))
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QString _updateDate)
{
      //qDebug() << "DataProxy_SQLite::" << QString::number (_qsoId) << "/" << _updateDate << endl;
    QSqlQuery query;
    QString stringQuery;


    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    if (query.exec(stringQuery))
    {
          //qDebug() << "DataProxy_SQLite:: TRUE" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

      //qDebug() << "DataProxy_SQLite:: FALSE" << endl;
    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QString _updateDate, const bool _queueSentQSL)
{
      //qDebug() << "DataProxy_SQLite::qslRecViaBureau: " << _updateDate << endl;
    QSqlQuery query;
    QString stringQuery;
    bool requestQSL = false;


    if  (_queueSentQSL)
    {
        stringQuery = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
        if(query.exec(stringQuery))
        {
            query.next();
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                if ((stringQuery == "Y") || (stringQuery == "R"))
                {
                    // NO ACTION REQUIRED, QSL IS ALREADY SENT
                       //qDebug() << "DataProxy_SQLite::qslRecViaBureau: QSL already requested" << endl;
                     requestQSL = false;
                     stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
                else
                {
                      //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-1" << endl;
                    requestQSL = true;
                    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
            }
            else
            {
                  //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-2" << endl;
                stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                requestQSL = true;
            }
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-3" << endl;
            stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
            requestQSL = true;
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        requestQSL = false;
        stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    }


    //stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    if (query.exec(stringQuery))
    {
          //qDebug() << "DataProxy_SQLite::qslRecViaBureau TRUE" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

      //qDebug() << "DataProxy_SQLite::qslRecViaBureau FALSE" << endl;
    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QString _updateDate)
{
      //qDebug() << "DataProxy_SQLite::qslRecViaDirect" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);

    if (query.exec(stringQuery))
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QString _updateDate, const bool _queueSentQSL)
{
      //qDebug() << "DataProxy_SQLite::qslRecViaDirect: " << _updateDate << endl;
    QSqlQuery query;
    QString stringQuery;

    if  (_queueSentQSL)
    {
        stringQuery = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
        if(query.exec(stringQuery))
        {
            query.next();
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                if ((stringQuery == "Y") || (stringQuery == "R"))
                {
                    // NO ACTION REQUIRED, QSL IS ALREADY SENT
                       //qDebug() << "DataProxy_SQLite::qslRecViaDirect: QSL already requested" << endl;
                     stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
                else
                {
                      //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-1" << endl;
                    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
            }
            else
            {
                  //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-2" << endl;
                stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
              //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-3" << endl;
            stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
        }

    }
    else
    {
        stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    }



    if (query.exec(stringQuery))
    {
          //qDebug() << "DataProxy_SQLite::qslRecViaDirect TRUE" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

      //qDebug() << "DataProxy_SQLite::qslRecViaDirect FALSE" << endl;
    return false;
}


bool DataProxy_SQLite::qslSentAsRequested(const int _qsoId, const QString _updateDate)
{
    //TODO: Add some protection to the data before modifying
      //qDebug() << "DataProxy_SQLite::qslSentAsRequested" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_sent = 'R', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
      //qDebug() << "DataProxy_SQLite::qslSentAsRequested: " << stringQuery << endl;

    if (query.exec(stringQuery))
    {
          //qDebug() << "DataProxy_SQLite::qslSentAsRequested" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::qslRecAsRequested(const int _qsoId, const QString _updateDate)
{
//TODO: Add some protection to the data before modifying
      //qDebug() << "DataProxy_SQLite::qslRecAsRequested" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_rcvd = 'R', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
      //qDebug() << "DataProxy_SQLite::qslRecAsRequested: " << stringQuery << endl;

    if (query.exec(stringQuery))
    {
          //qDebug() << "DataProxy_SQLite::qslRecAsRequested" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    return false;
}

bool DataProxy_SQLite::setClubLogSent(const int _qsoId, const QString _st, const QString _updateDate)
{ // Updates the QSO with the ClubLog status & date

    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET clublog_qso_upload_status = '%1', clublog_qso_upload_date = '%2' WHERE id = '%3'").arg(_st).arg(_updateDate).arg(_qsoId);
      //qDebug() << "DataProxy_SQLite::setClubLogSent: " << stringQuery << endl;

    if (query.exec(stringQuery))
    {
          //qDebug() << "DataProxy_SQLite::setClubLogSent - TRUE" << endl;
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
      //qDebug() << "DataProxy_SQLite::setClubLogSent - FALSE" << endl;

    return false;
}


bool DataProxy_SQLite::isQSLReceived(const int _qsoId)
{

      //qDebug() << "DataProxy_SQLite::isQSLReceived" << QString::number(_qsoId) << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("SELECT qsl_rcvd FROM log WHERE id = '%1'").arg(_qsoId);
    bool sqlOK = query.exec(stringQuery);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            stringQuery = (query.value(0)).toString();
            if (stringQuery == "Y")
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
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    return false;

}

bool DataProxy_SQLite::isQSLSent(const int _qsoId)
{

      //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {

        query.next();
        if (query.isValid())
        {
            stringQuery = (query.value(0)).toString();
            if (stringQuery == "Y")
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
        return false;
    }
    return false;
}

QString DataProxy_SQLite::getCallFromId(const int _qsoId)
{
      //qDebug() << "DataProxy_SQLite::getCallFromId" << endl;
    QSqlQuery query;
    QString stringQuery = QString("SELECT call FROM log WHERE id='%1'").arg(_qsoId);
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toString();
        }
        else
        {
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
    QString aux1 = QString();
    QString aux2 = QString();
    QString call = QString();

// IMPORTANT: band_rx is not always present, and if it is not present, the query with INNER JOIN will fail.
// To fix that we will do two queries, one to check if I have all the data and if not another one with a reduced scope.

    QString stringQuery = QString("SELECT qso_date, time_on, qslrdate, qslsdate, call, station_callsign, operator, M.name, B.name, R.name, freq, qsl_rcvd, lotw_qsl_rcvd, qsl_sent, dxcc, prop_mode, credit_granted FROM log INNER JOIN band as B ON bandid = B.id INNER JOIN band as R ON band_rx = R.id INNER JOIN mode as M ON modeid = M.id WHERE log.id='%1'").arg(_qsoId);

    bool sqlOk = query.exec(stringQuery);

    dataC << QString::number(_qsoId);

    QSqlRecord rec = query.record();
    if (sqlOk)
    {
       //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId sqlOK" << endl;
        if (query.next())
        //if (1)
        {
            if (query.isValid())
            {
                nameCol = rec.indexOf("qso_date");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("time_on");


                //aux = (query.value(nameCol)).toString(); aux1 = checkAndFixASCIIinADIF(aux1);
                //aux1 = checkAndFixASCIIinADIF(aux1);
                  //qDebug() << "FileManager::adifLogExportToFile-time_on: "  << aux1 << endl;

                aux1 = (query.value(nameCol)).toString();
                 //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId time: " << aux1 << endl;
                if ( ((aux1.length()) == 5) || ((aux1.length()) == 8) ){
                    aux1.remove(QChar(':'), Qt::CaseInsensitive);
                    dataC << aux1;
                     //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId time1.5: " << aux1 << endl;

                }
                 //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId time2: " << aux1 << endl;




                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("qslrdate");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("qslsdate");
                dataC << (query.value(nameCol)).toString();

                nameCol = rec.indexOf("call");
                call = (query.value(nameCol)).toString();
                dataC << call;

                nameCol = rec.indexOf("operator");
                dataC << (query.value(nameCol)).toString();

                //nameCol = rec.indexOf("M.name");                 //TODO: Fix this to get the proper column
                dataC << (query.value(7)).toString();

                //nameCol = rec.indexOf("B.name");
                dataC << (query.value(8)).toString();               //TODO: Fix this to get the proper column

                //nameCol = rec.indexOf("R.name");                 //TODO: Fix this to get the proper column (use an index instead of a number)
                dataC << (query.value(9)).toString();

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
                aux2 = (query.value(nameCol)).toString();
                if (aux2.length()>2)
                {
                    dataC <<  aux2;
                }
                else
                {
                    dataC << call;
                }


                //dataC << (query.value(nameCol)).toString();
                 //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: RETURNING ... OK" << endl;
                return dataC;
            }
            else
            {
                //NO VALID
                 //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NO VALID NOT OK" << endl;
                return QStringList();
            }
        }
        else
        {
            QString stringQuery = QString("SELECT qso_date, time_on, qslrdate, qslsdate, call, station_callsign, operator, M.name, B.name, freq, qsl_rcvd, lotw_qsl_rcvd, qsl_sent, dxcc, prop_mode, credit_granted FROM log INNER JOIN band as B ON bandid = B.id INNER JOIN mode as M ON modeid = M.id WHERE log.id='%1'").arg(_qsoId);
            //QString stringQuery = QString("SELECT qso_date, time_on, qslrdate, qslsdate, call, operator, M.name, B.name, freq, qsl_rcvd, lotw_qsl_rcvd, qsl_sent, dxcc, prop_mode, credit_granted FROM log INNER JOIN band as B ON bandid = B.id INNER JOIN mode as M ON modeid = M.id WHERE log.id='%1'").arg(_qsoId);
              //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NO NEXT NOT OK" << endl;
            call = QString();

            sqlOk = query.exec(stringQuery);
            rec = query.record();
            if (sqlOk)
            {
                 //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId OK2" << endl;
                if (query.next())
                {
                     //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NEXT OK2" << endl;
                    if (query.isValid())
                    {
                        nameCol = rec.indexOf("qso_date");
                        dataC << (query.value(nameCol)).toString();
                        nameCol = rec.indexOf("time_on");

                        aux1 = (query.value(nameCol)).toString();
                         //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId time2-1: " << aux1 << endl;
                        if ( ((aux1.length()) == 5) || ((aux1.length()) == 8) ){
                            aux1.remove(QChar(':'), Qt::CaseInsensitive);
                            dataC << aux1;
                             //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId time2-1.5: " << aux1 << endl;

                        }
                         //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId time2-2: " << aux1 << endl;


                        //dataC << (query.value(nameCol)).toString();
                        nameCol = rec.indexOf("qslrdate");
                        dataC << (query.value(nameCol)).toString();
                        nameCol = rec.indexOf("qslsdate");
                        dataC << (query.value(nameCol)).toString();
                        nameCol = rec.indexOf("call");                        
                        call = (query.value(nameCol)).toString();
                        dataC << call;
                        nameCol = rec.indexOf("operator");
                        dataC << (query.value(nameCol)).toString();

                        nameCol = rec.indexOf("M.name");                 //TODO: Fix this to get the proper column
                        dataC << (query.value(7)).toString();

                        nameCol = rec.indexOf("B.name");
                        dataC << (query.value(8)).toString();               //TODO: Fix this to get the proper column

                        //nameCol = rec.indexOf("band_rx");                 //TODO: Fix this to get the proper column (use an index instead of a number)
                        dataC << "";

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
                        aux2 = QString();
                        nameCol = rec.indexOf("station_callsign");
                        aux2 = (query.value(nameCol)).toString();
                        if (aux2.length()>2)
                        {
                            dataC <<  aux2;
                        }
                        else
                        {
                            dataC << call;
                        }


                         //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId: RETURNING ... OK" << endl;
                        return dataC;

                    }
                    else
                    {
                         //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NO VALID NOT OK2" << endl;
                        return QStringList();
                    }
                }
                else
                {
                     //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NO NEXT NOT OK2" << endl;
                    return QStringList();
                }



            }
            else
            {
                 //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId NOT OK2" << endl;
                return QStringList();
            }
            return QStringList();
            // NO NEXT
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
        return QStringList();
    }
     //qDebug() << "DataProxy_SQLite::getClubLogRealTimeFromId END NOT OK" << endl;
    return QStringList();
}

QString DataProxy_SQLite::getNameFromQRZ(const QString _call)
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
                    return (query.value(0)).toString();
                }
            }
        }
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }
}

QString DataProxy_SQLite::getQTHFromQRZ(const QString _call)
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
                    return (query.value(0)).toString();
                }
            }
        }
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }

}

QString DataProxy_SQLite::getLocatorFromQRZ(const QString _call)
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
                    return (query.value(0)).toString();
                }
            }
        }
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }

}


QString DataProxy_SQLite::getIOTAFromQRZ(const QString _call)
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
                    return (query.value(0)).toString();
                }
            }
        }
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }

}


QString DataProxy_SQLite::getQSLViaFromQRZ(const QString _call)
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
                    return (query.value(0)).toString();
                }
            }
        }
        return QString();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }
}


bool DataProxy_SQLite::deleteQSO(const int _qsoId)
{
      //qDebug() << "DataProxy_SQLite::deleteQSO" << endl;
    QSqlQuery query;
    QString stringQuery = QString("DELETE FROM log WHERE id='%1'").arg(_qsoId);
    if (query.exec(stringQuery))
    {
        return true;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;
    }
}

int DataProxy_SQLite::isWorkedB4(const QString _qrz, const int _currentLog)
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
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1;
    }

}

bool DataProxy_SQLite::isThisQSODuplicated(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode)
{
     //qDebug() << "DataProxy_SQLite::isThisQSODuplicated" << endl;
    QSqlQuery query;
    QString queryString;

    queryString = QString("SELECT id FROM log WHERE call='%1' AND qso_date='%2' AND time_on='%3' AND bandid='%4' AND modeid='%5'").arg(_qrz).arg(_date).arg(_time).arg(_band).arg(_mode);

    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ((query.value(0)).toInt()>0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;
    }

    return false;
}


int DataProxy_SQLite::getDuplicatedQSOId(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode)
{
      //qDebug() << "DataProxy_SQLite::isThisQSODuplicated" << endl;
     QSqlQuery query;
     QString queryString;
     int qsoId = -1;

     queryString = QString("SELECT id FROM log WHERE call='%1' AND qso_date='%2' AND time_on='%3' AND bandid='%4' AND modeid='%5'").arg(_qrz).arg(_date).arg(_time).arg(_band).arg(_mode);

     bool sqlOK = query.exec(queryString);
     if (sqlOK)
     {
         query.next();
         if (query.isValid())
         {
             qsoId = (query.value(0)).toInt();
             if (qsoId)
             {
                 return qsoId;
             }
             else
             {
                 return -1;
             }
         }
         else
         {
             return -1;
         }
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         return -1;
     }

    return -1;
}

bool DataProxy_SQLite::isDXCCConfirmed(const int _dxcc, const int _currentLog)
{
     //qDebug() << "DataProxy_SQLite::isDXCCConfirmed: " << QString::number(_dxcc) << "/" << QString::number(_currentLog) << endl;
    QString queryString = QString("SELECT confirmed from awarddxcc WHERE dxcc='%1' AND lognumber='%2'").arg(_dxcc).arg(_currentLog);
    QSqlQuery query;

    if (query.exec (queryString))
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(0)).toInt() == 1)
            {
                 //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: TRUE" << endl;
                return true;
            }
            else
            {
                 //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE1" << endl;
                return false;
            }
        }
        else
        {
             //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE2" << endl;
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE3" << endl;
        return false;
    }
     //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE4" << endl;
    return false;
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
    QSqlQuery query;

    QString queryString = QString("SELECT DISTINCT (substr (qso_date, 0, 5)) FROM log WHERE lognumber='%0' ORDER BY 'qso_date'").arg(_currentLog);
    QString year = QString();

    bool sqlOk = query.exec(queryString);
    if (sqlOk)
    {
          //qDebug() << "DataProxy_SQLite::getYearsOperating: sqlOk = true" << endl;
        while (query.next())
        {
            if (query.isValid())
            {
                year = (query.value(0)).toString();
                years << year;
                year.clear();
            }
        }
          //qDebug() << "DataProxy_SQLite::getYearsOperating: END OK - " << QString::number(years.size())<< endl;
        return years;
        //return years.sort();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::getYearsOperating: sqlOk = false" << endl;
        return years;
    }

}

void DataProxy_SQLite::compressDB()
{
    db->compress();
}

bool DataProxy_SQLite::unMarkAllQSO()
{
    return db->unMarkAllQSO();
}

int DataProxy_SQLite::getQSOonYear(const int _year, const int _logNumber)
{
      //qDebug() << "DataProxy_SQLite::getQSOonYear: " << QString::number(_year) << "/" << QString::number(_logNumber) << endl;

    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT count (id) from (SELECT DISTINCT id FROM log WHERE lognumber='%1' AND qso_date LIKE '%2%')").arg(_logNumber).arg(_year);

    sqlOK = query.exec(stringQuery);
      //qDebug() << "DataProxy_SQLite::getQSOonYear: stringQuery: " << stringQuery << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
              //qDebug() << "DataProxy_SQLite::getQSOonYear: " << QString::number((query.value(0)).toInt()) << endl;
            return (query.value(0)).toInt();
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getQSOonYear: 0" << endl;
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::getDXCConYear: Query error" << endl;
        return 0;
    }
}

int DataProxy_SQLite::getDXCConYear(const int _year, const int _logNumber)
{
      //qDebug() << "DataProxy_SQLite::getDXCConYear: " << QString::number(_year) << "/" << QString::number(_logNumber) << endl;

    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT count (dxcc) from  (SELECT DISTINCT dxcc FROM log WHERE lognumber='%0' AND qso_date LIKE '%%1%' AND dxcc <>'')").arg(_logNumber).arg(_year);

    sqlOK = query.exec(stringQuery);
      //qDebug() << "DataProxy_SQLite::getDXCConYear: stringQuery: " << stringQuery << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
              //qDebug() << "DataProxy_SQLite::getDXCConYear: " << QString::number((query.value(0)).toInt()) << endl;
            return (query.value(0)).toInt();
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getDXCConYear: 0" << endl;
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::getDXCConYear: Query error" << endl;
        return 0;
    }
}

int DataProxy_SQLite::getCQzonYear(const int _year, const int _logNumber)
{
      //qDebug() << "DataProxy_SQLite::getCQzonYear: " << QString::number(_year) << endl;
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT count (cqz) from  (SELECT DISTINCT cqz FROM log WHERE lognumber='%0' AND qso_date LIKE '%%1%' AND cqz <>'')").arg(_logNumber).arg(_year);

    sqlOK = query.exec(stringQuery);
      //qDebug() << "DataProxy_SQLite::getCQzonYear: stringQuery: " << stringQuery << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
              //qDebug() << "DataProxy_SQLite::getCQzonYear: " << QString::number((query.value(0)).toInt()) << endl;
            return (query.value(0)).toInt();
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getCQzonYear: 0" << endl;
            return 0;
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::getCQzonYear: Query error" << endl;
        return 0;
    }
}

bool DataProxy_SQLite::newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber)
{
     //qDebug() << "DataProxy_SQLite::newDXMarathon" << endl;
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    bool existingDXCC = false;
    bool existingCQz = false;

    stringQuery = QString("SELECT dxcc, cqz FROM log WHERE (lognumber='%1' AND qso_date  LIKE'%%2%') AND (dxcc ='%3' OR cqz ='%4')").arg(_logNumber).arg(_year).arg(_dxcc).arg(_cq);

    sqlOK = query.exec(stringQuery);
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
            return false;
        }
        else
        {
             //qDebug() << "DataProxy_SQLite::newDXMarathon - TRUE1" << endl;
            return true;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         //qDebug() << "DataProxy_SQLite::newDXMarathon - TRUE2" << endl;
         return true;   // It is an error inthe query but Work First Worry Later, let us work that QSO.
    }
     //qDebug() << "DataProxy_SQLite::newDXMarathon - TRUE3" << endl;
    return true;
}

QStringList DataProxy_SQLite::getContestNames()
{
      //qDebug() << "DataProxy_SQLite::getContestNames() "  << endl;
    QStringList contests = QStringList();
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT DISTINCT name from supportedcontests ORDER BY id ASC");

    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                  //qDebug() << "DataProxy_SQLite::getContestNames: " << stringQuery  << endl;
                contests.append(stringQuery);
            }
            else
            {
                return QStringList();
            }

        }

        return contests;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QStringList();
    }

    return QStringList();

}

QStringList DataProxy_SQLite::getContestCat(const int _catn)
{
    QStringList contests = QStringList();
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;

    switch (_catn) {
        case 1:
            stringQuery = QString("SELECT DISTINCT name from contestcatoperator ORDER BY id ASC");
        break;
        case 2:
            stringQuery = QString("SELECT DISTINCT name from contestcatassisted ORDER BY id ASC");
        break;
        case 3:
            stringQuery = QString("SELECT DISTINCT name from contestcatpower ORDER BY id ASC");
        break;
        case 4:
            stringQuery = QString("SELECT DISTINCT name from contestcatband ORDER BY id ASC");
        break;
        case 5:
            stringQuery = QString("SELECT DISTINCT name from contestcatoverlay ORDER BY id ASC");
        break;
        case 6:
            stringQuery = QString("SELECT DISTINCT name from contestcatmode ORDER BY id ASC");
        break;
        default:
            return QStringList();
        break;

        }

    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                contests.append(stringQuery);
            }
            else
            {
                return QStringList();
            }
        }
        return contests;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QStringList();
    }
    return QStringList();

}


QStringList DataProxy_SQLite::getContestOverlays()
{
      //qDebug() << "DataProxy_SQLite::getContestOverlays: "<< endl;

    QStringList contests = QStringList();
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT DISTINCT name from contestcatoverlay ORDER BY id ASC");

    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                  //qDebug() << "DataProxy_SQLite::getContestOverlays: " << stringQuery  << endl;
                contests.append(stringQuery);
            }
            else
            {
                return QStringList();
            }

        }

        return contests;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QStringList();
    }

    return QStringList();
}

QStringList DataProxy_SQLite::getPropModeList()
{
     //qDebug()  << "DataProxy_SQLite::getPropModeLists"  << endl;
    QString aux = QString();
    QStringList qs;
    qs.clear();

    QString stringQuery = QString("SELECT id, shortname, name FROM prop_mode_enumeration ORDER BY name");
    QSqlQuery query;

    if (query.exec(stringQuery))
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(0)).toString() + " - " + (query.value(1)).toString() + " - " + (query.value(2)).toString();
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
        return QStringList();
    }
    return qs;
}

QStringList DataProxy_SQLite::getSatellitesList()
{
      //qDebug()  << "DataProxy_SQLite::getSatellitesList"  << endl;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString stringQuery = QString("SELECT satarrlid, satname FROM satellites");
     QSqlQuery query;

     if (query.exec(stringQuery))
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
         return QStringList();
     }
     return qs;
}


QString DataProxy_SQLite::getSatelliteUplink(const QString _sat)
{
      //qDebug()  << "DataProxy_SQLite::getSatelliteUplink: " << _sat << endl;
    QString aux = QString();
    QString aux2 = QString();
    double fr1, fr2, fr;
    QString stringQuery = QString("SELECT uplink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    if (query.exec(stringQuery))
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
            return QString();
        }
    }
    else
    {
          //qDebug()  << "DataProxy_SQLite::getSatelliteUplink:  query failed: " << query.lastQuery()  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }

      //qDebug()  << "DataProxy_SQLite::getSatelliteUplink: final: " << aux << endl;
    return aux;
}


QString DataProxy_SQLite::getSatelliteDownlink(const QString _sat)
{
      //qDebug()  << "DataProxy_SQLite::getSatelliteDownlink: " << _sat << endl;
    QString aux = QString();
    QString aux2 = QString();
    double fr1, fr2, fr;
    QString stringQuery = QString("SELECT downlink FROM satellites WHERE satarrlid='%1'").arg(_sat);
    QSqlQuery query;

    if (query.exec(stringQuery))
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
            return QString();
        }
    }
    else
    {
          //qDebug()  << "DataProxy_SQLite::getSatelliteDownlink:  query failed: " << query.lastQuery()  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QString();
    }

      //qDebug()  << "DataProxy_SQLite::getSatelliteDownlink: final: " << aux << endl;
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
     QString stringQuery = QString("SELECT shortname, name FROM qsl_rec_status");
     QSqlQuery query;

     if (query.exec(stringQuery))
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
         return QStringList();
     }
     return qs;
}

QStringList DataProxy_SQLite::getQSLSentList()
{
      //qDebug()  << "DataProxy_SQLite::getQSLSentList"  << endl;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString stringQuery = QString("SELECT shortname, name FROM qsl_sent_status");
     QSqlQuery query;

     if (query.exec(stringQuery))
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
         return QStringList();
     }
     return qs;
}


QStringList DataProxy_SQLite::getClubLogStatusList()
{
      //qDebug()  << "DataProxy_SQLite::getClubLogStatusList"  << endl;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString stringQuery = QString("SELECT shortname, name FROM clublog_status");
     QSqlQuery query;

     if (query.exec(stringQuery))
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
         return QStringList();
     }
     return qs;
}

QStringList DataProxy_SQLite::getQSLViaList()
{
      //qDebug()  << "DataProxy_SQLite::getQSLViaList"  << endl;
     QString aux = QString();
     QStringList qs;
     qs.clear();
     QString stringQuery = QString("SELECT shortname, name FROM qsl_via");
     QSqlQuery query;

     if (query.exec(stringQuery))
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
         return QStringList();
     }
     return qs;
}

QStringList DataProxy_SQLite::getValidCatOptions(const int _currentCat, const int _lowerCat)
{
      //qDebug() << "DataProxy_SQLite::getContestNames: " << QString::number(_currentCat) <<"/" << QString::number(_lowerCat) << endl;
    QStringList contests = QStringList();
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;

    switch (_currentCat) {

        case 0:
            stringQuery = QString("SELECT DISTINCT contestcatoperator.name FROM contestcatoperator JOIN contest ON contest.catoperator=contestcatoperator.id WHERE contest.contest='%1' ORDER BY contestcatoperator.id").arg(_lowerCat);

        break;
        case 1:
            stringQuery = QString("SELECT DISTINCT contestcatassisted.name FROM contestcatassisted JOIN contest ON contest.catassisted=contestcatassisted.id WHERE contest.catoperator='1' ORDER BY contestcatassisted.id").arg(_lowerCat);
        break;
        case 2:
            stringQuery = QString("SELECT DISTINCT contestcatpower.name FROM contestcatpower JOIN contest ON contest.catpower=contestcatpower.id WHERE contest.catoperator='1' ORDER BY contestcatpower.id").arg(_lowerCat);
        break;
        case 3:
            stringQuery = QString("SELECT DISTINCT contestcatband.name FROM contestcatband JOIN contest ON contest.catband=contestcatband.id WHERE contest.catoperator='1' ORDER BY contestcatband.id").arg(_lowerCat);
        break;
        case 4:
            stringQuery = QString("SELECT DISTINCT contestcatoverlay.name FROM contestcatoverlay JOIN contest ON contest.catoverlay=contestcatoverlay.id WHERE contest.catoperator='1' ORDER BY contestcatoverlay.id").arg(_lowerCat);
        break;
        case 5:
            stringQuery = QString("SELECT DISTINCT contestcatmode.name FROM contestcatmode JOIN contest ON contest.catmode=contestcatmode.id WHERE contest.catoperator='1' ORDER BY contestcatmode.id").arg(_lowerCat);
        break;
        default:
            return QStringList();
        break;

        }

    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        while(query.next())
        {
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                contests.append(stringQuery);
            }
            else
            {
                return QStringList();
            }

        }

        return contests;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return QStringList();
    }
}

bool DataProxy_SQLite::haveAtLeastOneLog()
{
      //qDebug() << "DataProxy_SQLite::haveAtLeastOneLog()" << endl;
    QSqlQuery query;

    if (query.exec("SELECT COUNT(id) from logs"))
    {
        query.next();
        if (query.isValid())
        {
            if((query.value(0)).toInt()>0)
            {
                   return true;
            }
            else
            {
                return false;
            }

        }
        else
        {
            return false;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;
    }
    return false;
}

int DataProxy_SQLite::getNumberOfManagedLogs()
{
      //qDebug() << "DataProxy_SQLite::getNumberOfManagedLogs" << endl;
    QSqlQuery query;
    if (query.exec("SELECT COUNT (*) from logs"))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1;
    }
    return -1;
}

int DataProxy_SQLite::getMaxLogNumber()
{
    QSqlQuery query;
    QString st = QString("SELECT MAX(id) FROM logs");
    if (query.exec(st))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -1;
    }
    return -1;
}


QStringList DataProxy_SQLite::getListOfManagedLogs()
{
    //This function returns the list of log IDs that are being managed
      //qDebug() << "DataProxy_SQLite::getListOfManagedLogs" << endl;
    QSqlQuery query;
    QStringList qs;
    qs.clear();
    if (query.exec("SELECT id FROM logs"))
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
    return qs;
}

QString DataProxy_SQLite::getStationCallSignFromLog(const int _log)
{
      //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: " << QString::number(_log)<< endl;
    QSqlQuery query;
    QString st = QString("SELECT stationcall FROM logs WHERE id='%1'").arg(_log);
    if (query.exec(st))
    {
        query.next();
        if (query.isValid())
        {
              //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: " <<  (query.value(0)).toString() << endl;
            return (query.value(0)).toString();
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: Not valid" << endl;
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: query failed" << endl;
        return QString();
    }

      //qDebug() << "DataProxy_SQLite::getStationCallSignFromLog: END" << endl;
    return QString();

}

QString DataProxy_SQLite::getOperatorsFromLog(const int _log)
{
    //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: " << QString::number(_log)<< endl;
  QSqlQuery query;
  QString st = QString("SELECT operators FROM logs WHERE id='%1'").arg(_log);
  if (query.exec(st))
  {
      query.next();
      if (query.isValid())
      {
            //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: " <<  (query.value(0)).toString() << endl;
          return (query.value(0)).toString();
      }
      else
      {
            //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: Not valid" << endl;
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: query failed" << endl;
      return QString();
  }

    //qDebug() << "DataProxy_SQLite::getOperatorsFromLog: END" << endl;
  return QString();
}

QString DataProxy_SQLite::getCommentsFromLog(const int _log)
{
    //qDebug() << "DataProxy_SQLite::getLogDateFromLog: " << QString::number(_log)<< endl;
  QSqlQuery query;
  QString st = QString("SELECT comment FROM logs WHERE id='%1'").arg(_log);
  if (query.exec(st))
  {
      query.next();
      if (query.isValid())
      {
            //qDebug() << "DataProxy_SQLite::getLogDateFromLog: " <<  (query.value(0)).toString() << endl;
          return (query.value(0)).toString();
      }
      else
      {
            //qDebug() << "DataProxy_SQLite::getLogDateFromLog: Not valid" << endl;
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << "DataProxy_SQLite::getLogDateFromLog: query failed" << endl;
      return QString();
  }

    //qDebug() << "DataProxy_SQLite::getCommentsFromLog: END" << endl;
  return QString();
}

QString DataProxy_SQLite::getLogDateFromLog(const int _log)
{
    //qDebug() << "DataProxy_SQLite::getLogDateFromLog: " << QString::number(_log)<< endl;
  QSqlQuery query;
  QString st = QString("SELECT logdate FROM logs WHERE id='%1'").arg(_log);
  if (query.exec(st))
  {
      query.next();
      if (query.isValid())
      {
            //qDebug() << "DataProxy_SQLite::getLogDateFromLog: " <<  (query.value(0)).toString() << endl;
          return (query.value(0)).toString();
      }
      else
      {
            //qDebug() << "DataProxy_SQLite::getLogDateFromLog: Not valid" << endl;
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << "DataProxy_SQLite::getLogDateFromLog: query failed" << endl;
      return QString();
  }

    //qDebug() << "DataProxy_SQLite::getLogDateFromLog: END" << endl;
  return QString();
}

QString DataProxy_SQLite::getLogTypeNFromLog(const int _log)
{
    //qDebug() << "DataProxy_SQLite::getLogTypeNFromLog: " << QString::number(_log)<< endl;
  QSqlQuery query;
  QString st = QString("SELECT logtypen FROM logs WHERE id='%1'").arg(_log);
  if (query.exec(st))
  {
      query.next();
      if (query.isValid())
      {
            //qDebug() << "DataProxy_SQLite::getLogTypeNFromLog: " <<  (query.value(0)).toString() << endl;
          return (query.value(0)).toString();
      }
      else
      {
            //qDebug() << "DataProxy_SQLite::getLogTypeNFromLog: Not valid" << endl;
          return QString();
      }
  }
  else
  {
      emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        //qDebug() << "DataProxy_SQLite::getLogTypeNFromLog: query failed" << endl;
      return QString();
  }

    //qDebug() << "DataProxy_SQLite::getLogTypeNFromLog: END" << endl;
  return QString();
}

int DataProxy_SQLite::getContestTypeN(const int _co, const int _catop, const int _catas, const int _catpo, const int _catba, const int _catov, const int _catmo)
{//typeContestSelected, contestCatOperators, contestCatAssisted, contestCatPower, contestCatBands, contestCatOverlay, contestCatMode
     //qDebug() << "DataProxy_SQLite::getContestTypeN: " << endl;
    QSqlQuery query;
    QString st = QString("SELECT id FROM contest WHERE contest='%1' AND catoperator='%2' AND catassisted='%3' AND catpower='%4' AND catoverlay='%5' AND catmode='%6' AND catband='%7'").arg(_co).arg(_catop).arg(_catas).arg(_catpo).arg(_catov).arg(_catmo).arg(_catba);

     //qDebug() << "DataProxy_SQLite::getContestTypeN: " << st << endl;

    if (query.exec(st))
    {
         //qDebug() << "DataProxy_SQLite::getContestTypeN: (OK) LastQuery: " << query.lastQuery()  << endl;
        query.next();
        if (query.isValid())
        {
             //qDebug() << "DataProxy_SQLite::getContestTypeN: " <<  (query.value(0)).toString() << endl;
            return (query.value(0)).toInt();
        }
        else
        {
             //qDebug() << "DataProxy_SQLite::getContestTypeN: Not valid (-1)" << endl;
            return -1;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         //qDebug() << "DataProxy_SQLite::getContestTypeN: (ERROR) LastQuery: " << query.lastQuery()  << endl;
         //qDebug() << "DataProxy_SQLite::getContestTypeN: query failed (-1)" << endl;
        return -1;
    }

     //qDebug() << "DataProxy_SQLite::getContestTypeN: END (-1)" << endl;
    return -1;

}

QStringList DataProxy_SQLite::getDataFromContestType(const int _n)
{
      //qDebug() << "DataProxy_SQLite::getDataFromContestType - n: " << QString::number(_n) << endl;
    QStringList dataC = QStringList();
    QSqlQuery query, query1;
    QString stringQuery;
    int nameCol = -1;
    bool sqlOK;
    stringQuery = QString("SELECT supportedcontests.name, contest.contest, contest.catoperator, contest.catassisted, contest.catpower, contest.catoverlay, contest.catmode, contest.catband FROM supportedcontests JOIN contest ON contest.contest=supportedcontests.id WHERE contest.id='%1'").arg(_n);
    sqlOK = query.exec(stringQuery);
      //qDebug() << "DataProxy_SQLite::getDataFromContestType: LastQuery: " << query.lastQuery()  << endl;
    QSqlRecord rec = query.record();
    if (sqlOK)
    {
          //qDebug() << "DataProxy_SQLite::getDataFromContestType: Query OK" << endl;
        if(query.next())
        {
              //qDebug() << "DataProxy_SQLite::getDataFromContestType: Query Next" << endl;
            if (query.isValid())
            {
                  //qDebug() << "DataProxy_SQLite::getDataFromContestType: Query Valid" << endl;

                  //qDebug() << "DataProxy_SQLite::getDataFromContestType:-1 " << endl;
                nameCol = rec.indexOf("contest");
                dataC << (query.value(nameCol)).toString();
                  //qDebug() << "DataProxy_SQLite::getDataFromContestType: -2" << endl;
                nameCol = rec.indexOf("catoperator");
                dataC << (query.value(nameCol)).toString();
                  //qDebug() << "DataProxy_SQLite::getDataFromContestType: -3" << endl;
                nameCol = rec.indexOf("catassisted");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("catpower");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("catoverlay");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("catmode");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("name");
                dataC << (query.value(nameCol)).toString();
                nameCol = rec.indexOf("catband");
                dataC << (query.value(nameCol)).toString();

                return dataC;

            }
            else
            {
                  //qDebug() << "DataProxy_SQLite::getDataFromContestType: Query value no valid" << endl;
                return QStringList();
            }

        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getDataFromContestType: No Next" << endl;
            return QStringList();
        }

        return dataC;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::getDataFromContestType: Query not OK" << endl;
         return QStringList();
    }
}

int DataProxy_SQLite::getLogTypeNumber(const QString _logType)
{
    return db->getLogTypeNumber(_logType);
}

QString DataProxy_SQLite::getLogTypeName(const int _logType)
{
    return db->getLogTypeName(_logType);
}

QString DataProxy_SQLite::getLogTypeOfUserLog(const int _logN)
{
/*
 *  Returns the type of log (DX, CQ-WW-SSB, ...) or DX as default if nothing found
*/
      //qDebug() << "DataProxy_SQLite::getLogTypeOfUserLog: " << QString::number(_logN) << endl;
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT logtype FROM logs WHERE id='%1'").arg(_logN);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
              //qDebug() << "DataProxy_SQLite::getLogTypeOfUserLog: found: " << (query.value(0)).toString() << endl;
            return (query.value(0)).toString();
        }
        else
        {
              //qDebug() << "DataProxy_SQLite::getLogTypeOfUserLog: NOT found: returning DX"  << endl;
            return "DX";
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataProxy_SQLite::getLogTypeOfUserLog: Query error: returning DX"  << endl;
        return "DX";
    }
}

int DataProxy_SQLite::getLogNumberFromQSOId(const int _qsoId)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT lognumber FROM log WHERE id='%1'").arg(_qsoId);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }

    }
    else
    {
        return -1;
    }
}


int DataProxy_SQLite::getHowManyQSOInLog(const int _log)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(_log);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return 0;
        }

    }
    else
    {
        return 0;
    }
}

int DataProxy_SQLite::getHowManyConfirmedQSLInLog(const int _log)
{
    if (!doesThisLogExist(_log))
    {
       return 0;
    }

    QSqlQuery query;
    QString aux;
    aux = QString("SELECT count(id) FROM log WHERE qsl_rcvd='Y' AND lognumber='%1'").arg(_log);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


bool DataProxy_SQLite::addNewLog (const QStringList _qs)
{
     //qDebug() << "SetupPageLogs::addNewLog: " << _qs.at(2) << "/" << _qs.at(5) << "/" << _qs.at(6) << endl;

    //_qs << dateString << stationCallsign << _qs.at(4) << comment << _qs.at(12);
      //qDebug() << "SetupPageLogs::slotAnalyzeNewLogData:  " << _qs.at(4) << "/" << _qs.at(12) << endl;
    // Date/Call/Operators/"DX"/comment/"1"

    if (_qs.size()!=8)
    {
         //qDebug() << "SetupPageLogs::addNewLog: != 8"  << endl;
        return false;
    }
     //qDebug() << "SetupPageLogs::addNewLog: Has the appropriate length"  << endl;

    QString aux = QString();
    //int nameCol = -1;

    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);    
    QString _operators = _qs.at(2);

    //_operators.clear();
    //_operators << (_qs.at(2)).split(',', QString::SkipEmptyParts);
    QString _typeContest = _qs.at(3);
    QString _comment = _qs.at(4);
    QString _typeContestN = _qs.at(5);
    QString id = _qs.at(6);
    QString editing = _qs.at(7);

    QString queryString;
    QSqlQuery query;
    bool sqlOK;

    if (editing == "1")
    { // We are editing
         //qDebug() << "SetupPageLogs::addNewLog: We are editing!" << endl;
        queryString = QString("UPDATE logs SET logdate = '%1', stationcall = '%2', operators = '%3', comment = '%4',  logtype = '%5', logtypen = '%6' WHERE id = '%7'").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(_typeContest).arg(_typeContestN).arg(id);
        sqlOK = query.exec(queryString);
        if (sqlOK)
        {
             //qDebug() << "SetupPageLogs::addNewLog: Editing OK!" << endl;
            return true;
        }
        else
        {
             //qDebug() << "SetupPageLogs::addNewLog: Editing NOK!" << endl;
            return false;
        }
        return false;
    }

     //qDebug() << "SetupPageLogs::addNewLog: We are adding a new log" << endl;

    // First we check if the log is already there
    queryString = QString("SELECT id FROM logs WHERE logdate='%1' AND stationcall='%2' AND logtype='%3' AND logtypen='%4'").arg(_dateString).arg(_stationCallsign).arg(_typeContest).arg(_typeContestN);
    //"logs"
    //"id, logdate, stationcall, comment, logtype"
      //qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << endl;

    sqlOK = query.exec(queryString);

    QSqlRecord rec = query.record(); // Number of columns

    if ( (query.next()) && (query.isValid()) )
    {
        //nameCol = rec.indexOf("id");
        //aux = (query.value(nameCol)).toString();
          //qDebug() << "SetupPageLogs::addNewLog: id = " << aux << endl;
        return false;
    }

    //Now we add the new log

    queryString = QString("INSERT INTO logs (logdate, stationcall, operators, comment, logtype, logtypen) values('%1','%2','%3','%4', '%5', '%6')").arg(_dateString).arg(_stationCallsign).arg(_operators).arg(_comment).arg(_typeContest).arg(_typeContestN);
      //qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << endl;
    sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}



bool DataProxy_SQLite::doesThisLogExist(const int _log)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT count(id) FROM logs WHERE id='%1'").arg(_log);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            if ( query.value(0).toInt()>0)
            {
                return true;
            }

        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }
    return false;
}

int DataProxy_SQLite::getContinentIdFromContinentShortName(const QString _n)
{

    if (_n.length()!=2)
    {
        return -3;
    }
    QSqlQuery query;
    QString stQuery = QString("SELECT id FROM continent WHERE shortname=='%1'").arg(_n);
    if (query.exec(stQuery))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -2;
    }
}

QString DataProxy_SQLite::getContinentShortNameFromEntity(const int _n)
{
    QSqlQuery query;
    QString aux;
    //aux = QString("SELECT continent.shortname FROM entity JOIN continent ON entity.continent=continent.shortname WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_n);
    aux = QString("SELECT continent.shortname FROM entity JOIN continent ON entity.continent=continent.shortname WHERE dxcc='%1'").arg(_n);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toString();
        }
        else
        {
            return QString();
        }

    }
    else
    {
        return  QString();
    }
}

int DataProxy_SQLite::getContinentIdFromEntity(const int _n)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT continent.id FROM entity JOIN continent ON entity.continent=continent.shortname WHERE dxcc='%1'").arg(_n);
    //aux = QString("SELECT continent.id FROM entity JOIN continent ON entity.continent=continent.shortname WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_n);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }

    }
    else
    {
        return -2;
    }
}

QStringList DataProxy_SQLite::getContinentShortNames()
{

    QSqlQuery query;
    QString aux;
    QStringList continents;
    continents.clear();
    aux = QString("SELECT shortname FROM continent");
    if (query.exec(aux))
    {
        while (query.next())
        {
            if (query.isValid())
            {
                continents << query.value(0).toString();
            }
        }
        return continents;
    }
    else
    {
        return QStringList();
    }
    return QStringList();
}

bool DataProxy_SQLite::isValidContinentShortName(const QString _n)
{
    QString stringQuery = QString("SELECT id FROM continent WHERE shortname ='%1'").arg(_n);
    QSqlQuery query;
    if (query.exec(stringQuery))
    {
        if (query.next())
        {
            if (query.isValid())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }
    return false;
}

int DataProxy_SQLite::getITUzFromPrefix(const QString _p)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT ituz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -2;
    }

}

int DataProxy_SQLite::getCQzFromPrefix(const QString _p)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT cqz FROM prefixesofentity WHERE prefix LIKE '%1'").arg(_p);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -2;
    }
}

int DataProxy_SQLite::getCQzFromEntity(const int _n)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT cqz FROM entity WHERE dxcc='%1'").arg(_n);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -2;
    }
}

int DataProxy_SQLite::getITUzFromEntity(const int _n)
{
    QSqlQuery query;
    QString aux;
    aux = QString("SELECT ituz FROM entity WHERE dxcc='%1'").arg(_n);
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -2;
    }
}

QString DataProxy_SQLite::getEntityNameFromId(const int _n)
{
      //qDebug() << "DataProxy_SQLite::getEntityNameFromId: " << QString::number(_n) << endl;

    QSqlQuery query;
    //QString stQuery = QString("SELECT name FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_n);
    QString stQuery = QString("SELECT name FROM entity WHERE dxcc='%1'").arg(_n);
    QString motherEntName = QString();

    if (_n > 1000)
    {
        QString aux = (QString::number(_n)).right(3);
        QString stQuery2 = QString("SELECT name FROM entity WHERE dxcc='%1'").arg(aux);
        if (query.exec(stQuery2))
        {
            if (query.next())
            {
                if (query.isValid())
                {
                    motherEntName = (query.value(0)).toString();
                }
            }
        }
    }

    if (query.exec(stQuery))
    {
        if (query.next())
        {
            if (query.isValid())
            {
                if (_n>1000)
                {
                    return (query.value(0)).toString() + " (" + motherEntName + ")";
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
            return QString();
        }
    }
    else
    {
        return QString();
    }
    return QString();
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

    //queryString = QString("SELECT mainprefix FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_entityN);
    queryString = QString("SELECT mainprefix FROM entity WHERE dxcc='%1'").arg(_entityN);
    //queryString = "SELECT prefix FROM prefixesofentity WHERE dxcc=='" + QString::number(i) +"'";
    if (!query.exec(queryString))
    {
        return QString();
    }
    else
    {
        query.next();

        if (query.isValid()){
            return (query.value(0)).toString();
        }else
        {
            return QString();
        }
    }
    return QString();
}

int DataProxy_SQLite::getDXCCFromPrefix(const QString _p)
{
      //qDebug() << "DataProxy_SQLite::getDXCCFromPrefix(: -" << _p << "-" << endl;

    QSqlQuery query;
    QString stQuery = QString("SELECT dxcc FROM prefixesofentity WHERE prefix='%1'").arg(_p);
    if (query.exec(stQuery))
    {
        if (query.next())
        {
            if (query.isValid())
            {
                return (query.value(0)).toInt();
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -2;
        }
    }
    else
    {
        return -3;
    }
    return -4;
}


bool DataProxy_SQLite::isNewCQz(int _c)
{
    QSqlQuery query;
    QString queryString = QString("SELECT id FROM log WHERE cqz='%1'").arg(_c);
    //queryString = "SELECT id FROM log WHERE cqz=='" + QString::number(_cqz) +"'";

    if (!query.exec(queryString))
    {
        return false;
    }
    else
    {
        query.next();

        if (query.isValid()){
            return true;
        }else{
            return false;
        }

    }

    return false;
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

    if (!query.exec(queryString))
    {
        return false;
    }
    else
    {
        query.next();

        if (query.isValid()){
            return true;
        }else{
            return false;
        }
    }
    return false;
}

double DataProxy_SQLite::getLongitudeFromEntity(const int _e)
{

    QString queryString = QString("SELECT longitude FROM entity WHERE dxcc='%1'").arg(_e);
    QSqlQuery query;

    if (!query.exec(queryString))
    {
        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) ) {
            return 0.0;
        }else{
            return (query.value(0)).toDouble();
        }

    }
    return 0.0;
}

double DataProxy_SQLite::getLatitudeFromEntity(const int _e)
{

    QString queryString = QString("SELECT latitude FROM entity WHERE dxcc='%1'").arg(_e);
    QSqlQuery query;

    if (!query.exec(queryString))
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) ) {
            return 0.0;
        }else{
            return (query.value(0)).toDouble();
        }

    }
    return 0.0;
}

QString DataProxy_SQLite::getEntityPrefixes(const int _enti)
{
    if (_enti<=0)
    {
        return QString();

    }
    QString result;
    result = "";
    QString queryString;
    QSqlQuery query;
    int i = _enti;
    queryString = "SELECT prefix FROM prefixesofentity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
            return result;
        }
    }

    return QString();
}

QStringList DataProxy_SQLite::getEntitiesNames()
{
      //qDebug()  << "DataProxy_SQLite::getEntitiesNames"  << endl;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString stringQuery = QString("SELECT mainprefix, name, dxcc FROM entity");
    QSqlQuery query;

    if (query.exec(stringQuery))
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
    QString stringQuery = QString("SELECT mainprefix, name, dxcc FROM entity");
    QSqlQuery query,query2;
    int dxcc = -1;
    QString aux2 = QString();

    if (query.exec(stringQuery))
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

                    stringQuery = QString("SELECT mainprefix, name FROM entity WHERE dxcc ='%1'").arg(aux2);
                    if (query2.exec(stringQuery))
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
    QString aux;
    aux = QString("SELECT count(id) FROM entity");
    if (query.exec(aux))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return 0;
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return 0;
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

    QString queryString, aux;
    QSqlQuery query;
    aux.clear();
    queryString = QString("SELECT isoname FROM entity WHERE dxcc='%1'").arg(_n);
    if (!query.exec(queryString))
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         //qDebug()  << "DataProxy_SQLite::getISOName: Query error - UN"  << endl;
        return "nu"; // When no flag is known, we return the UN flag
    }
    else
    {
        query.next();

        if (query.isValid()){
              //qDebug()  << "DataProxy_SQLite::getISOName: ISO Name: " << (query.value(0)).toString() << endl;
            aux = (query.value(0)).toString();
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
            return "un"; // When no flag is known, we return the UN flag
        }
    }
      //qDebug()  << "DataProxy_SQLite::getISOName: NO ISO Name: default"  << endl;
    return "un"; // When no flag is known, we return the UN flag
}

void DataProxy_SQLite::getFoundInLog(const QString _txt, const int _log)
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

