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
    //qDebug() << "DataProxy_SQLite::DataProxy_SQLite " << endl;

    db = new DataBase(0);
    dbCreated = db->createBandModeMaps();

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
    return db->getModeNameFromID2(_id);
}

double DataProxy_SQLite::getFreqFromBandId(const int _id)
{
    return db->getFreqFromBandId(_id);
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
        //qDebug() << "DataProxy_SQLite::clearLog: Log deleted FAILED" << endl;
        errorCode = query.lastError().number();
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
        //qDebug() << "DataProxy_SQLite::clearLog: Awarddxcc deletedFAILED" << endl;
        errorCode = query.lastError().number();
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
        //qDebug() << "DataProxy_SQLite::clearLog: Awardwaz deleted FAILED" << endl;
        errorCode = query.lastError().number();
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
            //qDebug() << "DataProxy_SQLite::clearLog: VACUUM FAILED" << endl;
            errorCode = query.lastError().number();
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
    //QDebug() << "DataProxy_SQLite::qslSentViaDirect: " << stringQuery << endl;
    if (query.exec(stringQuery))
    {
        return true;
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
        requestQSL = false;
        stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    }


    //stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    if (query.exec(stringQuery))
    {
        //qDebug() << "DataProxy_SQLite::qslRecViaBureau TRUE" << endl;
        return true;
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

    return false;
}

bool DataProxy_SQLite::isQSLReceived(const int _qsoId)
{

    //qDebug() << "DataProxy_SQLite::isQSLReceived" << QString::number(_qsoId) << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("SELECT qsl_rcvd FROM log WHERE id = '%1'").arg(_qsoId);
    query.exec(stringQuery);
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

bool DataProxy_SQLite::isQSLSent(const int _qsoId)
{

    //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
    query.exec(stringQuery);
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

QString DataProxy_SQLite::getCallFromId(const int _qsoId)
{
    //qDebug() << "DataProxy_SQLite::getCallFromId" << endl;
    QSqlQuery query;
    QString stringQuery = QString("SELECT call FROM log WHERE id='%1'").arg(_qsoId);
    query.exec(stringQuery);
    query.next();
    if (query.isValid())
    {
        return (query.value(0)).toString();
    }
    else
    {
        return "";
    }
}

bool DataProxy_SQLite::deleteQSO(const int _qsoId)
{
    //qDebug() << "DataProxy_SQLite::deleteQSO" << endl;
    QSqlQuery query;
    QString stringQuery = QString("DELETE FROM log WHERE id='%1'").arg(_qsoId);
    return query.exec(stringQuery);
}

int DataProxy_SQLite::isWorkedB4(const QString _qrz, const int _currentLog)
{
    //qDebug() << "DataProxy_SQLite::isWorkedB4" << endl;
    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT id FROM log WHERE call='%1' AND lognumber='%2'").arg(_qrz).arg(_currentLog);
    //queryString = "SELECT id FROM log WHERE call='" + _qrz +"'";
    //qDebug() << "World::checkIfWorkedB4: " << queryString << endl;
    query.exec(queryString);
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
       //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE3" << endl;
        return false;
    }
   //qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE4" << endl;
    return false;
}

bool DataProxy_SQLite::isHF(const int _band)
{
    if ((_band>=18) && (_band<=27))
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
    if ((_band == 19) || (_band==21) ||(_band==23) )
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
    if (_band<=17)
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

QStringList DataProxy_SQLite::getOperatingYears(const int _currentLog)
{
    //qDebug() << "DataProxy_SQLite::getYearsOperating: " << QString::number(_currentLog) << endl;
    QStringList years = QStringList();
    QSqlQuery query;
    QString queryString = QString("SELECT DISTINCT (substr (qso_date, 0, 5)) FROM log WHERE lognumber='%0'").arg(_currentLog);
    QString year = QString();

    bool sqlOk = query.exec(queryString);
    if (sqlOk)
    {
        while (query.next())
        {
            if (query.isValid())
            {
                year = (query.value(0)).toString();
                years << year;
                year.clear();
            }
        }
        return years;
        //return years.sort();
    }
    else
    {
        return years;
    }

}

void DataProxy_SQLite::compressDB()
{
    db->compress();
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
        //qDebug() << "DataProxy_SQLite::getCQzonYear: Query error" << endl;
        return 0;
    }
}

QStringList DataProxy_SQLite::getContestNames()
{
    qDebug() << "DataProxy_SQLite::getContestNames() "  << endl;
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
                qDebug() << "DataProxy_SQLite::getContestNames: " << stringQuery  << endl;
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
         return QStringList();
    }

    return QStringList();

}

QStringList DataProxy_SQLite::getBandNames()
{

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
                bands.append(stringQuery);
            }
            else
            {
                return QStringList();
            }

        }

        return bands;
    }
    else
    {
         return QStringList();
    }
}

QStringList DataProxy_SQLite::getValidCatOptions(const int _currentCat, const int _lowerCat)
{
    qDebug() << "DataProxy_SQLite::getContestNames: " << QString::number(_currentCat) <<"/" << QString::number(_lowerCat) << endl;
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
         return QStringList();
    }
}

bool DataProxy_SQLite::haveAtLeastOneLog()
{
    qDebug() << "DataProxy_SQLite::haveAtLeastOneLog()" << endl;
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
        return -1;
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
        return -1;
    }


    return -1;
}
