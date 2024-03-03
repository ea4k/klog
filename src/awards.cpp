#include "awards.h"
/***************************************************************************
                          awards.cpp  -  description
                             -------------------
    begin                : nov 2011
    copyright            : (C) 2011 by Jaime Robles
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

//#include <QDebug>

Awards::Awards(DataProxy_SQLite *dp, const QString &_parentFunction)
{
    Q_UNUSED(_parentFunction);
    //qDebug() << "Awards::Awards- from: " << _parentFunction;
    dataProxy = dp;
    //QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    world = new World(dataProxy, Q_FUNC_INFO);

       //qDebug() << "Awards::Awards - Before DXMarathon" ;
    dxMarathon = new DXMarathon(dataProxy);
       //qDebug() << "Awards::Awards - After DXMarathon" ;
    util = new Utilities(Q_FUNC_INFO);
    //world->create();
/*
    newOneColor.setNamedColor("#ff0000");
    neededColor.setNamedColor("#ff8c00");
    workedColor.setNamedColor("#ffd700");
    confirmedColor.setNamedColor("#32cd32");
    defaultColor.setNamedColor("#00bfff");
*/
    //"Awards::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default;
    //Awards::setColors:  "#ff0000" / "#ff8c00" / "#ffd700" / "#32cd32" / "#00bfff"

    newOneColor = Qt::black;
    neededColor = Qt::black;
    workedColor = Qt::black;
    confirmedColor = Qt::black;
    defaultColor = Qt::black;

    dxccWorked.clear();
    dxccConfirmed.clear();
    wazWorked.clear();
    wazConfirmed.clear();
    manageModes = false;
       //qDebug() << "Awards::Awards - END" ;
}

Awards::~Awards()
{
    delete(world);
    delete(dxMarathon);
    delete(util);
}

void Awards::setAwardDXCC(const int _qsoId)
{
       //qDebug() << "Awards::setAwardDXCC: _qsoId: " << QString::number(_qsoId);
    dataProxy->setDXCCAwardStatus(_qsoId);
}

void Awards::setAwardWAZ(const int _qsoId)
{
       //qDebug() << "Awards::setAwardWAZ: _qsoId: " << QString::number(_qsoId);
    dataProxy->setWAZAwardStatus(_qsoId);
}

QString Awards::getQSOofAward (const int _enti, const int _bandid, const int _log, const bool _confirmed)
{// Returns the QRZ that granted that status in the DXCC
       //qDebug() << "Awards::getQSOofAward: " << QString::number(_enti) << "/" << QString::number(_bandid);
    QSqlQuery query;
    QString stringQuery = QString();
    bool sqlOK = false;
    //QString answer = QString();

    if (_confirmed)
    {
        stringQuery = QString("SELECT call, qso_date from log where dxcc='%1' AND bandid='%2' AND lognumber='%3' AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y')").arg(_enti).arg(_bandid).arg(_log);
    }
    else
    {
       stringQuery = QString("SELECT call, qso_date from log where dxcc='%1' AND bandid='%2' AND lognumber='%3'").arg(_enti).arg(_bandid).arg(_log);
    }


    sqlOK = query.exec(stringQuery);
       //qDebug() << "Awards::getQSOofAward: stringQuery: " << stringQuery;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            QString answer = query.value(0).toString() + " / " + query.value(1).toString() ;

            query.finish();
               //qDebug() << "Awards::getQSOofAward: answer: " << answer;
            return answer;
        }
        else
        {
            query.finish();
               //qDebug() << "Awards::getQSOofAward: 0";
            return QString();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
           //qDebug() << "Awards::getQSOofAward: Query error";
        return QString();
    }
}

int Awards::getQSOIdofAward (const int _enti, const int _bandid, const int _log, const bool _confirmed)
{// Returns the QSOid that granted that status in the DXCC
       //qDebug() << "Awards::getQSOIdofAward: " << QString::number(_enti) << "/" << QString::number(_bandid);
    QSqlQuery query;
    QString stringQuery = QString();
    bool sqlOK = false;
    int answer = -1;

    //stringQuery = QString("SELECT qsoid from awarddxcc where dxcc='%1' and band='%2'").arg(_enti).arg(_bandid);


    if (_confirmed)
    {
        stringQuery = QString("SELECT id from log where dxcc='%1' AND bandid='%2' AND lognumber='%3' AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y')").arg(_enti).arg(_bandid).arg(_log);
    }
    else
    {
       stringQuery = QString("SELECT id from log where dxcc='%1' AND bandid='%2' AND lognumber='%3'").arg(_enti).arg(_bandid).arg(_log);
    }

    sqlOK = query.exec(stringQuery);
       //qDebug() << "Awards::getQSOIdofAward: stringQuery: " << stringQuery;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            answer = query.value(0).toInt();
               //qDebug() << "Awards::getQSOIdofAward: answer: " << QString::number(answer);
            query.finish();
            if (answer>=1)
            {
                return answer;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            query.finish();
               //qDebug() << "Awards::getQSOIdofAward: 0";
            return -2;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
           //qDebug() << "Awards::getQSOIdofAward: Query error";
        return -3;
    }
}

int Awards::getDXCCWorked(const int _logNumber)
{
    //qDebug() << "Awards::getDXCCWorked (logNumber): " << QString::number(_logNumber);
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;

    if (dataProxy->doesThisLogExist(_logNumber))
    {
        stringQuery = QString("SELECT COUNT (DISTINCT dxcc) FROM log WHERE dxcc>'0' AND dxcc < '1000' AND lognumber='%1'").arg(_logNumber);
    }
    else
    {
        stringQuery = QString("SELECT COUNT (DISTINCT dxcc) FROM log WHERE dxcc>'0' AND dxcc < '1000' ");
    }


    sqlOK = query.exec(stringQuery);
       //qDebug() << "Awards::getDXCCWorked: stringQuery: " << stringQuery;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v = (query.value(0)).toInt();
            query.finish();
               //qDebug() << "Awards::getDXCCWorked: " << QString::number((query.value(0)).toInt());
            return v;
        }
        else
        {
               //qDebug() << "Awards::getDXCCWorked: 0";
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
           //qDebug() << "Awards::getDXCCWorked: Query error";
        return 0;
    }
}

int Awards::getDXCCConfirmed(const int _logNumber)
{
     //qDebug() << "Awards::getDXCCConfirmed (logNumber): " << QString::number(_logNumber);
    return dataProxy->getFieldInBand (DXCC, "ALL", true, "ALL", _logNumber);
}

int Awards::getWAZWorked(const int _logNumber)
{
       //qDebug() << "Awards::getWAZWorked (logNumber): " << QString::number(_logNumber);
    QSqlQuery query;
    QString stringQuery;
    if (dataProxy->doesThisLogExist(_logNumber))
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41' AND lognumber='%1')").arg(_logNumber);
    }
    else
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41')");
    }

    bool sqlOK = query.exec(stringQuery);
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
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return 0;
    }
}

int Awards::getWAZConfirmed(const int _logNumber)
{
       //qDebug() << "Awards::getWAZConfirmed (logNumber): " << QString::number(_logNumber);

    QSqlQuery query;
    QString stringQuery;
    //Usar la siguiente para el confirmed
    if (dataProxy->doesThisLogExist(_logNumber))
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41' AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y') AND lognumber='%1')").arg(_logNumber);
    }
    else
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41' AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y'))");
    }

    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            int v =(query.value(0)).toInt();
            query.finish();
            return v;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return 0;
    }
}

bool Awards::isThisSpotConfirmed(const QStringList &_qs)
{
    return (getDXStatus(_qs) == 13);
}

int Awards::getDXStatus (const QStringList &_qs)
{
    //qDebug() << "Awards::getDXStatus: Entity: " << _qs.at(0) << "/ Band: " << _qs.at(1) << "/ Mode: " << _qs.at(2)  << "/ Log: " << _qs.at(3)  <<  QT_ENDL;
    // Receives:  QStringList _qs;
    //_qs << Entity << BandId << << ModeId << lognumber;

/*                                                                                                                  Not mode
    -1 - Error.                                                                                     - ERROR     - ERROR
 0  -   New one                                                                                 - New One       - New One       -   0
 1  -   Worked but not in this band nor this mode                                               - Needed One    - Needed One    -   1
 2  -   Worked in this band, not this mode                                                      - Needed One    - Worked One    -   3
 3  -   Worked in this band and in this mode                                                    - Worked One    - Worked One    -   3
 4  -   Worked in this mode, not this band                                                      - Needed One    - Needed One    -   1

 5  -   Confirmed in another band/mode but not worked in this band nor this mode                - Needed One    - Needed One    -   1
 6  -   Confirmed in another band/mode but just worked in this band and not in this mode        - Needed One    - Worked One    -   3
 7  -   Confirmed in another band/mode but just worked in this mode and not in this band        - Needed One    - Needed One    -   1
 8  -   Confirmed in another band/mode but just worked in this band and mode                    - Worked One    - Worked One    -   3
 9  -   Confirmed in this mode, but not worked this band                                        - Needed One    - Needed One    -   1
10  -   Confirmed in this mode, but worked this band                                            - Worked One    - Worked One    -   3
11  -   Confirmed in this band but not worked in this mode                                      - Needed One    - Confirmed One -   13
12  -   Confirmed in this band but worked in this mode                                          - Worked One    - Confirmed One -   13
13  -   Confirmed in this band and mode                                                         - Confirmed One - Confirmed One -   13


14  -   New CQ Zone
15  -   New CQ Zone on band
16  -   New ITU Zone
17  -   New ITU Zone on band
18  -   New Grid
19  -   New Grid on band

*/

/*
0   -   New One     -   Never worked before                         -   RED
1   -   Needed      -   New one in this band                        -   ORANGE
2   -   Worked      -   Worked in this band but not confirmed       -   YELLOW
3   -   Confirmed   -   Confirmed in this band                      -   GREEN
*/
    if (_qs.length() != 4  )
    {
        //qDebug() << "Awards::getDXStatus: Length != 4, return -1";
        return -1;
    }

    //int errorCode = 0;

    int _band = _qs.at(1).toInt();
    int _mode = _qs.at(2).toInt();
    int _logNumber = _qs.at(3).toInt();
    int dxccEntity = (_qs.at(0)).toInt();

    //qDebug() << "Awards::getDXStatus: dxccEntity: " << QString::number(dxccEntity);
    if (dxccEntity<=0)
    {
        //qDebug() << "Awards::getDXStatus: dxccEntity <= 0, return -1";
        return -1;
    }

    bool checkingMode = true;
    if ( (_mode==-1) || (manageModes==false))
    {
        checkingMode = false;
        //qDebug() << "Awards::getDXStatus: checkingMode = FALSE";
    }

    int wb = dxccStatusBand(dxccEntity, _band, _logNumber); //-1 error / 0 Not worked / 1 worked / 2 confirmed
    int wm = -1;
    if (checkingMode)
    {
        wm = dxccStatusMode(dxccEntity, _mode, _logNumber); //-1 error / 0 Not worked / 1 worked / 2 confirmed
    }
   // int wm = dxccStatusMode(dxccEntity, _mode, _logNumber); //-1 error / 0 Not worked / 1 worked / 2 confirmed

    if (wm==-1)
    {
        checkingMode = false;
    }

    //qDebug() << "Awards::getDXStatus: wb=" << QString::number(wb) << " - wm=" << QString::number(wm);
    //qDebug() << "Awards::getDXStatus: dxccStatus: " << QString::number(dxccStatus(dxccEntity, _logNumber));

    switch(dxccStatus(dxccEntity, _logNumber))
    {
    case 0:
        //qDebug() << "Awards::getDXStatus: return 0";
        return 0;                   // ATNO
        //break;
    case 1:                         // Worked, not confirmed
        switch (wb)
        {
        case 0:                     //  Not worked in this band but in another band
            if (checkingMode)
            {
                if (wm==1)
                {
                    //qDebug() << "Awards::getDXStatus: return 4";
                    return 4;
                }
                else
                {
                    //qDebug() << "Awards::getDXStatus: return 1";
                    return 1;
                }
            }
            else
            {
                //qDebug() << "Awards::getDXStatus: return 4 nc";
                return 4;
            }
        case 1:                     // Worked in this band
            if (checkingMode)
            {
                if (wm==1)
                {
                    //qDebug() << "Awards::getDXStatus: return 3";
                    return 3;
                }
                else
                {
                    //qDebug() << "Awards::getDXStatus: return 2";
                    return 2;
                }
            }
            else
            {
                //qDebug() << "Awards::getDXStatus: return 3 nc";
                return 3;
            }

        default:                    // ERROR
               //qDebug() << "Awards::getDXStatus: default return -1 - 1";
            return -1;
        }
    case 2:         // Confirmed
        if (wb==2)
        {
            if (checkingMode)
            {
                if (wm==2)
                {
                    //qDebug() << "Awards::getDXStatus: return 13";
                    return 13;
                }
                else if (wm==1)
                {
                    //qDebug() << "Awards::getDXStatus: return 12";
                    return 12;
                }
                else
                {
                    //qDebug() << "Awards::getDXStatus: return 11";
                    return 11;
                }
            }
            else
            {
                //qDebug() << "Awards::getDXStatus: return 13 nc";
                return 13;
            }
        }
        else if ((wb ==1) || (wb == 0))
        {
            if (checkingMode)
            {
                if (wb==1)
                {
                    if (wm==1)
                    {
                        //qDebug() << "Awards::getDXStatus: return 8";
                        return 8;
                    }
                    else
                    {
                        //qDebug() << "Awards::getDXStatus: return 6";
                        return 6;
                    }
                }
                else
                {
                    if (wm==1)
                    {
                        //qDebug() << "Awards::getDXStatus: return 7";
                        return 7;
                    }
                    else
                    {
                        //qDebug() << "Awards::getDXStatus: return 5";
                        return 5;
                    }
                }
            }
            else
            {
                if (wb==0)
                {
                    //qDebug() << "Awards::getDXStatus: return 7 nc";
                    return 7;
                }
                else
                {
                    //qDebug() << "Awards::getDXStatus: return 8 nc";
                    return 8;
                }
            }
        }
        else
        {
            //qDebug() << "Awards::getDXStatus: return -1 - 2";
            return -1;
        }
    default:
        //qDebug() << "Awards::getDXStatus: return -1 default2";
        return -1;
    }   // END OF SWITCH
    //qDebug() << "Awards::getDXStatus: END, return -1";
    //return -1;
}

int Awards::dxccStatusBandMode(const int _ent, const int _band, const int _mode, const int _logNumber, bool _checkingMode)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << "Awards::dxccStatusBandMode: " << QString::number(_ent) << "/" << QString::number(_band) << "/" << QString::number(_mode);
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();

    if (_checkingMode)
    {
           //qDebug() << "Awards::dxccStatusBandMode: Checking Mode TRUE";
        queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2' AND modeid='%3' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_mode)).arg(QString::number(_logNumber));
    }
    else
    {
           //qDebug() << "Awards::dxccStatusBandMode: Checking Mode FALSE";
        queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2' AND lognumber='%3' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_logNumber));
    }

    int status = 0;

    if (query.exec(queryString))
    {
        while (query.next())
        {
            if ( query.isValid() )
            {
                if((query.value(0).toString() == "Y") || (query.value(1).toString() == "Y"))
                {
                       //qDebug() << "Awards::dxccStatusBandMode: return - 2";
                    query.finish();
                    return 2;
                }
                status = 1;
            }
        }
        query.finish();
    }
    else
    { // The query fails...
           //qDebug() << "Awards::dxccStatusBandMode: return - -1";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return -1;
    }
           //qDebug() << "Awards::dxccStatusBandMode: return - 0-4";
    return status;
}

/*
int Awards::dxccStatusBandMode(const int _ent, const int _band, const int _mode, const int _logNumber, bool _checkingMode)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << "Awards::dxccStatusBandMode: " << QString::number(_ent) << "/" << QString::number(_band) << "/" << QString::number(_mode);
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();

    if (_checkingMode)
    {
           //qDebug() << "Awards::dxccStatusBandMode: Checking Mode TRUE";
        queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND band='%2' AND mode='%3' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_mode)).arg(QString::number(_logNumber));
    }
    else
    {
           //qDebug() << "Awards::dxccStatusBandMode: Checking Mode FALSE";
        queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND band='%2' AND lognumber='%3' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_logNumber));
    }

    int status = 0;

    if (query.exec(queryString))
    {
        while (query.next())
        {
            if ( query.isValid() )
            {
                if(query.value(0).toString() == "1")
                {
                       //qDebug() << "Awards::dxccStatusBandMode: return - 2";
                    query.finish();
                    return 2;
                }
                else if(query.value(0).toString() == "0")
                {
                       //qDebug() << "Awards::dxccStatusBandMode: return - 1";
                   if (status < 1)
                   {
                       status = 1;
                   }
                }
            }
        }
        query.finish();

    }
    else
    { // The query fails...
           //qDebug() << "Awards::dxccStatusBandMode: return - -1";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return -1;
    }
           //qDebug() << "Awards::dxccStatusBandMode: return - 0-4";
    return status;
}

*/

int Awards::dxccStatus(const int _ent, const int _logNumber)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << "Awards::dxccStatus: " << QString::number(_ent);
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();
    int worked = 0;

    queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND lognumber='%2' ").arg(QString::number(_ent)).arg(QString::number(_logNumber));

    if (query.exec(queryString))
    {
           //qDebug() << "Awards::dxccStatus: query exec OK: " << query.lastQuery();
        while (query.next())
        {
               //qDebug() << "Awards::dxccStatus: query VALUE: " << (query.value(0)).toString();
            if ( query.isValid() )
            {
                   //qDebug() << "Awards::dxccStatus: query valid OK";
                if(((query.value(0)).toString() == "Y") || ((query.value(1)).toString() == "Y"))
                {

                       //qDebug() << "Awards::dxccStatus: value = 1 - return 2";
                    query.finish();
                    return 2;
                }
                worked = 1;
            }
        }
           //qDebug() << "Awards::dxccStatus: return "<< QString::number(worked) ;
        query.finish();
           //qDebug() << "Awards::dxccStatus: END: " << QString::number(worked);
        return worked;
    }
    else
    { // The query fails...
           //qDebug() << "Awards::dxccStatus: return -1" ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return -1;
    }
       //qDebug() << "Awards::dxccStatus: return 0" ;
    //return worked;
}

/*
int Awards::dxccStatus(const int _ent, const int _logNumber)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << "Awards::dxccStatus: " << QString::number(_ent);
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();
    int worked = 0;

    queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND lognumber='%2' ").arg(QString::number(_ent)).arg(QString::number(_logNumber));

    if (query.exec(queryString))
    {
           //qDebug() << "Awards::dxccStatus: query exec OK: " << query.lastQuery();
        while (query.next())
        {
               //qDebug() << "Awards::dxccStatus: query VALUE: " << (query.value(0)).toString();
            if ( query.isValid() )
            {
                   //qDebug() << "Awards::dxccStatus: query valid OK";
                if((query.value(0)).toString() == "1")
                {

                       //qDebug() << "Awards::dxccStatus: value = 1 - return 2";
                    query.finish();
                    return 2;
                }
                else if((query.value(0)).toString() == "0")
                {

                       //qDebug() << "Awards::dxccStatus: value = 0 - worked 1";
                    if (worked <1)
                    {
                        worked = 1;
                    }
                }
            }
        }
           //qDebug() << "Awards::dxccStatus: return "<< QString::number(worked) ;
        query.finish();
           //qDebug() << "Awards::dxccStatus: END: " << QString::number(worked);
        return worked;

    }
    else
    { // The query fails...
           //qDebug() << "Awards::dxccStatus: return -1" ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return -1;
    }
       //qDebug() << "Awards::dxccStatus: return 0" ;
    //return worked;
}
*/

QColor Awards::getQRZDXStatusColor(const QStringList &_qs)
{
    //qs << Entity << spotBand << "-1" << QString::number(currentLog) ;
    //qDebug() << "Awards::getQRZDXStatusColor qs.length: " << QString::number(_qs.length());

    //From Search QSO to QSL: q << _call << bandid << _mode << QString::number(currentLog);
    //qDebug() << "Awards::getQRZDXStatusColor: " << _qs.at(0) << "/" << _qs.at(1) << "/" << _qs.at(2) << _qs.at(3);
    // Receives:  QStringList _qs;
    //_qs << Entity << BandID << ModeId << lognumber;

    /*
    0 - New One
    1 - Needed
    2 - Worked
    3 - Confirmed
    */

    QColor returnedColor;

    int status = getDXStatus(_qs);

    //qDebug() << "Awards::getQRZDXStatusColor: status: " << QString::number(status) << "/" << getDXStatusString(status);
    //qDebug() << "Awards::getQRZDXStatusColor: status: " << QString::number(status);

    switch (status) {
        case 0:
            returnedColor = newOneColor;
        break;
        case 1:
            returnedColor =  neededColor;
        break;
        case 2:
            returnedColor =  neededColor;
        break;
        case 3:
            returnedColor =  workedColor;
        break;
        case 4:
            returnedColor =  neededColor;
        break;
        case 5:
            returnedColor =  neededColor;
        break;
        case 6:
            returnedColor =  neededColor;
        break;
        case 7:
            returnedColor =  neededColor;
        break;
        case 8:
            returnedColor =  workedColor;
        break;
        case 9:
            returnedColor =  neededColor;
        break;
        case 10:
            returnedColor =  workedColor;
        break;
        case 11:
            returnedColor =  neededColor;
        break;
        case 12:
            returnedColor =  workedColor;
        break;
        case 13:
            returnedColor =  confirmedColor;
        break;
        //break;
        default:
            returnedColor =  defaultColor;
        break;
    }
    return returnedColor;
}

QString Awards::getDXStatusString (const int &_status)
{
       //qDebug() << "Awards::getDXStatusString: " << QString::number(_status);

    QString message = QString();

    switch (_status) {

    case 0:
        message = QObject::tr("New One, work it!");
        //message = QObject::tr("0-new One");
    break;
    case 1:
        message = QObject::tr("Needed, work it!");
        //message = QObject::tr("1-Needed, work it!");
    break;
    case 2:
        message = QObject::tr("Needed, work it!");
    break;
    case 3:
        message = QObject::tr("Worked but not confirmed");
    break;
    case 4:
        message = QObject::tr("Needed, work it!");
   break;
    case 5:
        message = QObject::tr("Needed, work it!");
   break;
    case 6:
        message = QObject::tr("Needed, work it!");
   break;
    case 7:
        message = QObject::tr("Needed, work it!");
   break;
    case 8:
        message = QObject::tr("Worked but not confirmed");
   break;
    case 9:
    message = QObject::tr("Needed, work it!");
   break;
    case 10:
        message = QObject::tr("Worked but not confirmed");
   break;
    case 11:
    message = QObject::tr("Needed, work it!");
   break;
    case 12:
        message = QObject::tr("Worked but not confirmed");
   break;
    case 13:
    message = QObject::tr("Confirmed");
   break;
    //break;
    default:
        message = QObject::tr("Not identified");
    break;
    }
    return message;
}

QString Awards::getDXCCStatusBand(const int _dxcc, const int _band)
{
    // Returns -, W or C (Not worked, worked, Confirmed)
    //qDebug() << Q_FUNC_INFO << "DXCC/Band: " << QString::number(_dxcc) << "/" << QString::number(_band);
    //qDebug() << Q_FUNC_INFO << "dxccStatusList: " << QString::number(dxccStatusList.length ());

    EntityBandStatus aux;

    foreach (aux, dxccStatusList)
    {
         //qDebug() << Q_FUNC_INFO << " DXCC: " << QString::number(aux.dxcc);
         //qDebug() << Q_FUNC_INFO << " Band: " << QString::number(aux.bandid);
         if (aux.confirmed)
         {
             //qDebug() << Q_FUNC_INFO << "Confirmed";
         }
         else
         {
          //qDebug() << Q_FUNC_INFO << "Not confirmed";
         }
        if (aux.dxcc == _dxcc)
        {
            //qDebug() << Q_FUNC_INFO << " DXCC found: " << QString::number(_dxcc);
            if (aux.bandid == _band)
            {
                //qDebug() << Q_FUNC_INFO << " Band found: " << QString::number(_band);
                if (aux.confirmed)
                {
                    //qDebug() << Q_FUNC_INFO << " Confirmed " ;
                    return "C";
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO << " Worked " ;
                    return "W";
                }
            }
        }
    }
    //qDebug() << Q_FUNC_INFO << " Returnin -" ;
    return "-";
}

QString Awards::getDXCCStatusBand2(const int _dxcc, const int _band, const int _logNumber)
{
    // Returns -, W or C (Not worked, worked, Confirmed)
       //qDebug() << "Awards::getDXCCStatusBand: log received: " << QString::number(_logNumber);
    QString stringQuery;
    if (_logNumber<0)
    {
        stringQuery = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2'").arg(_dxcc).arg(_band);
    }
    else
    {
        stringQuery = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2' AND lognumber='%3'").arg(_dxcc).arg(_band).arg(_logNumber);
    }
    QString status = "-";

    QSqlQuery query;
    bool sqlOk = query.exec(stringQuery);
    if (sqlOk)
    {
        while (query.next()) {
            if (query.isValid())
            {
                if (((query.value(0)).toString() == "Y") || ((query.value(1)).toString() == "Y"))
                {
                    query.finish();
                    return "C";
                }
                status = "W";
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return "-";
    }
    query.finish();
    return status;
}

/*
  QString Awards::getDXCCStatusBand(const int _dxcc, const int _band, const int _logNumber)
{
    // Returns -, W or C (Not worked, worked, Confirmed)
       //qDebug() << "Awards::getDXCCStatusBand: log received: " << QString::number(_logNumber);
    QString stringQuery;
    if (_logNumber<0)
    {
        stringQuery = QString("SELECT confirmed from awarddxcc WHERE dxcc='%1' AND band='%2'").arg(_dxcc).arg(_band);
    }
    else
    {
        stringQuery = QString("SELECT confirmed from awarddxcc WHERE dxcc='%1' AND band='%2' AND lognumber='%3'").arg(_dxcc).arg(_band).arg(_logNumber);
    }
    QString status = "-";

    QSqlQuery query;
    bool sqlOk = query.exec(stringQuery);
    if (sqlOk)
    {
        while (query.next()) {
            if (query.isValid())
            {
                if ((query.value(0)).toString() == "1")
                {
                    query.finish();
                    return "C";
                }
                else if ((query.value(0)).toString() == "0")
                {
                    if (status == "-")
                    {
                        status = "W";
                    }
                }
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return "-";
    }
    query.finish();
    return status;
}

*/

QString Awards::checkIfValidIOTA(const QString &_tiota)
{
/**********************************
  IOTA should be always with this format: CC-NNN
  being:
     - CC the shortname of the continent
     - NNN Number of the reference.
       NNN has ALWAYS to include THREE(3) characters.
       ADIF Specs says:
            CC is the continent designator {NA, SA, EU , AF, OC, AS, AN}
            XXX is the island designator, where 0 <= XXX ,<= 999 [use leading zeroes]

Returns a valid format IOTA if possible and "" in other cases.

************************************/
       //qDebug() << "Awards::checkIfValidIOTA: " << _tiota;
    //bool _valid = false;
    QString _continent;
    QString _number;

    if (_tiota.count("-") == 1)
    {
        QStringList _values = _tiota.split("-", QT_SKIP);
        if (_values.size() != 2)
        {
            return "";
        }

        if (dataProxy->isValidContinentShortName(_values.at(0)))
        {
            _continent = _values.at(0);
        }
        else
        {
            return "";
        }
        if ((_values.at(1)).toInt() > 0)
        {
            _number = _values.at(1);
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }

       //qDebug() << "Awards::checkIfValidIOTA (cont) " << _continent;
       //qDebug() << "Awards::checkIfValidIOTA (numb): " << _number;

    // Check if continent is valid

    if (dataProxy->isValidContinentShortName(_continent))
    {
        if ( (_number.toInt() >0 ) && ((_number.toInt()) < 1000 ))
        {
            if ((_number.length()) == 3)
            {
                return _continent + "-" + _number ;
            }
            else if ((_number.length()) == 2)
            {
                return _continent + "-0" + QString::number((_number).toInt());
            }
            else if ((_number.length()) == 1)
            {
                return _continent + "-00" + QString::number((_number).toInt());
            }
            else
            {
                return "";
            }
        }
        else
        {
            return "";
        }
    }
    else
    {
        return QString();
    }
    //return QString();

 /*
    QString stringQuery = QString("SELECT id FROM continent WHERE shortname ='%1'").arg(_continent);

    QSqlQuery query;
    query.exec(stringQuery);
    while (query.next()) {
        if (query.isValid())
        {
            _valid = true;

            if ( (_number.toInt() >0 ) && ((_number.toInt()) < 1000 ))
            {
                if ((_number.length()) == 3)
                {
                    return _continent + "-" + _number ;
                }
                else if ((_number.length()) == 2)
                {
                    return _continent + "-0" + QString::number((_number).toInt());
                }
                else if ((_number.length()) == 1)
                {
                    return _continent + "-00" + QString::number((_number).toInt());
                }
                else
                {
                    return "";
                }
            }
            else
            {
                return "";
            }
        }
    }
    return "";
*/
}

void Awards::setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default)
{
       //qDebug() << "Awards::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default;

    defaultColor = QColor(_default.toUpper());
    neededColor = QColor(_needed.toUpper());
    workedColor = QColor(_worked.toUpper());
    confirmedColor = QColor(_confirmed.toUpper());
    newOneColor = QColor(_newOne.toUpper());

    if (defaultColor.isValid())
    {
        defaultColor.setNamedColor(_default.toUpper());
    }

    if (neededColor.isValid())
    {
        neededColor.setNamedColor(_needed.toUpper());
    }

    if (confirmedColor.isValid())
    {
        confirmedColor.setNamedColor(_confirmed.toUpper());
    }

    if (newOneColor.isValid())
    {
        newOneColor.setNamedColor(_newOne.toUpper());
    }

    if (workedColor.isValid())
    {
        workedColor.setNamedColor(_worked.toUpper());
    }
}

QColor Awards::getDefaultColor()
{
    return defaultColor;
}

void Awards::recalculateAwards()
{
/*
  TODO: I need to optimize this function.
    The select & insert of setAwardDXCC are too slow)
    Should go in a transaction
*/

    qDebug() << Q_FUNC_INFO;
    dataProxy->updateAwardDXCC();
    emit awardDXCCUpdated();
    dataProxy->updateAwardWAZ();
    qDebug() << Q_FUNC_INFO << " - END";
}

int Awards::getQSOsInLog(const int _logNumber)
{
       //qDebug() << "Awards::getQSOsInLog: " << QString::number(_logNumber);
    QSqlQuery query;
    QString queryString;
    int v;

    queryString = QString("SELECT COUNT (id) FROM log WHERE lognumber='%1'").arg(_logNumber) ;

    if( !query.exec(queryString) )
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return -1;
    }
    else
    {
        query.next();
        if (query.isValid())
        {
            v = query.value(0).toInt();
            query.finish();
            return v;
        }
        else
        {
            query.finish();
            return -2;
        }
    }
}

/*
void Awards::setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId)
{
       //qDebug() << "Awards::setAwards: " << QString::number(_dxcc);

    //_workedOrConfirmed = -1     Remove this pair
    //_workedOrConfirmed = 0     Set as Worked
    //_workedOrConfirmed = 1     Set as Confirmed

    int d = _dxcc;
    int b = _band;
    int m = _mode;
    int w = _workedOrConfirmed;
    int l = _logNumber;
    int i = _qsoId;
    int z = _waz;

    setAwardDXCC(d, b, m, w, l, i);
    setAwardWAZ(z, b, m, w, l, i);
    //setAwardWAZ(_waz,  _band,  _mode,  _workedOrConfirmed, _logNumber);

}

*/

void Awards::setAwards(const int _qsoId)
{
       //qDebug() << "Awards::setAwards: _qsoId: " << QString::number(_qsoId);
    dataProxy->setDXCCAwardStatus(_qsoId);
    dataProxy->setWAZAwardStatus(_qsoId);
}
/*
int Awards::setAwardDXCCst(const int _dxcc, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId)
{
       //qDebug() << "Awards::setAwardDXCCst-0: " << QString::number(_dxcc) << "/" << QString::number(_band) << "/" << QString::number(_mode) << "/" << QString::number(_logNumber) << "/" << QString::number(_qsoId);
    int nameCol=-1;
    QString _refid = QString();

    // _confirmedQSO == false QSO is just worked
    // _confirmedQSO == true QSO is confirmed

    //TODO: Fix the way we check for data validity for this function
    if (!( (_dxcc>=0) && (_band >=0) && (_mode>=0) && (_logNumber>=0) && (_qsoId >=0) ))
    {
            //qDebug() << "Awards::setAwardDXCCst: Not valid data received!";
        return -1;
    }
    int _iconfirmed;

    if (_confirmed)
    {
        _iconfirmed = 1;
    }
    else
    {
        _iconfirmed = 0;
    }

       //qDebug() << "Awards::setAwardDXCCst: _qsoId: " << QString::number(_qsoId) << "/" << QString::number(_iconfirmed);
    QString stringQuery = QString();
    QSqlQuery query;
    bool sqlOK = false;
    int errorCode = -1;

    stringQuery = QString("SELECT id, confirmed FROM awarddxcc where dxcc='%1' AND band='%2' AND mode='%3' AND lognumber='%4'").arg(_dxcc).arg(_band).arg(_mode).arg(_logNumber);


    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
           //qDebug() << "Awards::setAwardDXCCst-1:";

        query.next();
        QSqlRecord rec = query.record();

        if (query.isValid())
        {// We have some data, we need to UPDATE - We are only confirming!
               //qDebug() << "Awards::setAwardDXCCst: We have some data, we neer to update";
            nameCol = rec.indexOf("id");
            _refid = query.value(nameCol).toString();
            stringQuery = QString("UPDATE awarddxcc SET confirmed='1', qsoid='%1' WHERE id='%2'").arg(_qsoId).arg(_refid);
               //qDebug() << "Awards::setAwardDXCCst: (UPDATE): " << stringQuery;
            if (sqlOK)
            { // Set of data updated
                   //qDebug() << "Awards::setAwardDXCCst: Data updated!";
                query.finish();
                return 1;
            }
            else
            { // Something failed. Trace it!
                errorCode = query.lastError().nativeErrorCode();
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                query.finish();
                return -1;

            }

        }
        else
        { // We don't have this set, we need to INSERT
            stringQuery = QString("INSERT INTO awarddxcc (dxcc, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_dxcc).arg(_band).arg(_mode).arg(_iconfirmed).arg(_logNumber).arg(_qsoId);
            sqlOK = query.exec(stringQuery);
               //qDebug() << "Awards::setAwardDXCCst: We don't have data... so we INSERT";
               //qDebug() << "Awards::setAwardDXCCst: (INSERT): " << stringQuery;
            if (sqlOK)
            { // Set of data included
                query.finish();
                   //qDebug() << "Awards::setAwardDXCCst: Data inserted!";
            }
            else
            { // Something failed. Trace it!
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                errorCode = query.lastError().nativeErrorCode();
                query.finish();
                return -1;

            }

        }

    }
    else
    { // Trace the error... what may be happening???
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        errorCode = query.lastError().nativeErrorCode();
        query.finish();
        return -1;

    }

    return -1;

}

*/

/*
int Awards::setAwardWAZst(const int _cqz, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId)
{
      //qDebug() << "Awards::setAwardWAZst(CQZ/BAND/MODE/WORKED/log/qsoid): " << QString::number(_cqz) << "/" << QString::number(_band) << "/" << QString::number(_mode) << "/" << QString::number(_logNumber) << "/" << QString::number(_qsoId);


    // _confirmed == false QSO is just worked
    // _confirmed == true QSO is confirmed



   //TODO: Fix the way we check for data validity for this function
    if (!( (_cqz>=0) && (_band >=0) && (_mode>=0) && (_logNumber>=0) && (_qsoId >=0) ))
    {
           //qDebug() << "Awards::setAwardWAZst: some data was NOK";
        return -1;
    }
        int _iconfirmed;

        if (_confirmed)
        {
            _iconfirmed = 1;

        }
        else
        {
            _iconfirmed = 0;
        }


        QString stringQuery;
        QSqlQuery query;
        bool sqlOK;
        //int errorCode = -1;

        stringQuery = QString("INSERT INTO awardwaz (cqz, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_cqz).arg(_band).arg(_mode).arg(_iconfirmed).arg(_logNumber).arg(_qsoId);

        sqlOK = query.exec(stringQuery);

        if ((sqlOK) && (!_confirmed)) // First time a DXCC/Band/mode is worked
        {
               //qDebug() << "Awards::setAwardWAZst: _qsoId: " << QString::number(_qsoId) << "- 1" ;
            query.finish();
            return 1;
        }
        else if ((sqlOK) && (_confirmed)) // First time a CQZ/Band/Mode is confirmed
        {
               //qDebug() << "Awards::setAwardWAZst: _qsoId: " << QString::number(_qsoId) << "- 2" ;
            query.finish();
            return 2;
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
               //qDebug() << "Awards::setAwardWAZst: _qsoId: " << QString::number(_qsoId) << "- sqlOK ERROR: " ;
            //errorCode = query.lastError().nativeErrorCode();
            query.finish();
               //qDebug() << "Awards::setAwardWAZst: LastQuery: " << query.lastQuery() ;
               //qDebug() << "Awards::setAwardWAZst: LastError-data: " << query.lastError().databaseText() ;
               //qDebug() << "Awards::setAwardWAZst: LastError-driver: " << query.lastError().driverText() ;
               //qDebug() << "Awards::setAwardWAZst: LastError-n: " << QString::number(query.lastError().nativeErrorCode() );


        }
        return -1;
}

*/

/*
int Awards::setAwardDXCCConfirmed(const int _band, const int _mode, const int _dxcc, const int _newQSOid) // Changes the status of a DXCC from worked to confirmed
{
       //qDebug() << "Awards::setAwardDXCCConfirmed: " << QString::number(_band) << "/" << QString::number(_mode) << "/" << QString::number(_dxcc)<< "/" << QString::number(_newQSOid)<< QT_ENDL;


    QString stringQuery;
    QSqlQuery query = QSqlQuery();
    bool sqlOK = false;
    int errorCode = -1;
    int nameCol = -1;
    QString aux = QString();

    stringQuery = QString("SELECT qsoid FROM awarddxcc WHERE band='%1' AND mode='%2' AND dxcc='%3'").arg(_band).arg(_mode).arg(_dxcc);
    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        QSqlRecord rec = query.record();
        if (query.next())
        {
            if (query.isValid())
            {
                nameCol = rec.indexOf("qsoid");
                aux = (query.value(nameCol)).toString();
                query.finish();
                stringQuery = QString("UPDATE awarddxcc SET confirmed='1', qsoid='%1' WHERE qsoid='%2'").arg(_newQSOid).arg(aux);
                sqlOK = query.exec(stringQuery);
                if (sqlOK)
                {
                    query.finish();
                    return _newQSOid;
                }
                else
                { // UPDATE failed
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                    errorCode = query.lastError().nativeErrorCode();
                    query.finish();
                       //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastQuery: " << query.lastQuery() ;
                       //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastError-data: " << query.lastError().databaseText() ;
                       //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastError-driver: " << query.lastError().driverText() ;
                       //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastError-n: " << QString::number(query.lastError().nativeErrorCode() );
                    return errorCode;
                }

            }
            else
            {
                // Not valid record
                   //qDebug() << "Awards::setAwardDXCCConfirmed: Not valid record";
                return -3;
            }
        }
        else
        {
            // Not next record
               //qDebug() << "Awards::setAwardDXCCConfirmed: Not next record";
            return -2;
        }
    }
    else
    { //
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        errorCode = query.lastError().nativeErrorCode();
        query.finish();
           //qDebug() << "Awards::setAwardDXCCConfirmed: LastQuery: " << query.lastQuery() ;
           //qDebug() << "Awards::setAwardDXCCConfirmed: LastError-data: " << query.lastError().databaseText() ;
           //qDebug() << "Awards::setAwardDXCCConfirmed: LastError-driver: " << query.lastError().driverText() ;
           //qDebug() << "Awards::setAwardDXCCConfirmed: LastError-n: " << QString::number(query.lastError().nativeErrorCode() );
        return errorCode;
    }

    return 1;
}

*/

int Awards::setDXCCToQSO(const int _dxcc, const int _qsoid) // Defines the DXCC in a QSO
{
       //qDebug() << "Awards::setDXCCToQSO: " << QString::number(_dxcc) << "/" << QString::number(_qsoid);
    //int errorCode = -1;
    QString queryString = QString("UPDATE log SET dxcc='%1' WHERE id='%2'").arg(_dxcc).arg(_qsoid);
    QSqlQuery query = QSqlQuery();
    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.finish();
        return 1;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
           //qDebug() << "Awards::setDXCCToQSO: DXCC Updated in Log but failed....";
       int errorCode = query.lastError().nativeErrorCode().toInt();
        query.finish();
           //qDebug() << "Awards::setDXCCToQSO: LastQuery: " << query.lastQuery() ;
           //qDebug() << "Awards::setDXCCToQSO: LastError-data: " << query.lastError().databaseText() ;
           //qDebug() << "Awards::setDXCCToQSO: LastError-driver: " << query.lastError().driverText() ;
           //qDebug() << "Awards::setDXCCToQSO: LastError-n: " << QString::number(query.lastError().nativeErrorCode() );
        return errorCode;
    }
}

int Awards::setCQToQSO(const int _cqz, const int _qsoid) // Defines the CQ in a QSO
{
       //qDebug() << "Awards::setCQToQSO: " << QString::number(_cqz) << "/" << QString::number(_qsoid);
    //int errorCode = -1;
    QString queryString = QString("UPDATE log SET cqz='%1' WHERE id='%2'").arg(_cqz).arg(_qsoid);
    QSqlQuery query = QSqlQuery();
    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        query.finish();
        return 1;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
           //qDebug() << "Awards::setCQToQSO: DXCC Updated in Log but failed....";
        int errorCode = query.lastError().nativeErrorCode().toInt();
        query.finish();
           //qDebug() << "Awards::setCQToQSO: LastQuery: " << query.lastQuery() ;
           //qDebug() << "Awards::setCQToQSO: LastError-data: " << query.lastError().databaseText() ;
           //qDebug() << "Awards::setCQToQSO: LastError-driver: " << query.lastError().driverText() ;
           //qDebug() << "Awards::setCQToQSO: LastError-n: " << QString::number(query.lastError().nativeErrorCode() );
        return errorCode;
    }
}

bool Awards::getIsDXCCConfirmed(const int _dxcc, const int _logNumber)
{
  //  isDXCCConfirmed(const int _dxcc, const int _currentLog);
    return dataProxy->isDXCCConfirmed(_dxcc, _logNumber);

}

int Awards::getDXMarathonQSO(const int _year, const int _logNumber)
{
       //qDebug() << "Awards::getDXMarathonQSO: " << QString::number(_year);

    return dxMarathon->getDXMarathonQSO(_year, _logNumber);
}

int Awards::getDXMarathonDXCC(const int _year, const int _logNumber)
{
       //qDebug() << "Awards::getDXMarathonDXCC: " << QString::number(_year);

    return dxMarathon->getDXMarathonDXCC(_year, _logNumber);
}

int Awards::getDXMarathonCQ(const int _year, const int _logNumber)
{
       //qDebug() << "Awards::getDXMarathonCQ: " << QString::number(_year);
    return dxMarathon->getDXMarathonCQ(_year, _logNumber);
}

int Awards::getDXMarathonScore(const int _year, const int _logNumber)
{
       //qDebug() << "Awards::getDXMarathonScore: " << QString::number(_year);
    return dxMarathon->getDXMarathonScore(_year, _logNumber);
}

bool Awards::isDXMarathonNeed(const int _dxcc, const int _cq, const int _year, const int _logNumber)
{
    return dxMarathon->neededForDXMarathon(_dxcc, _cq, _year, _logNumber);
}

int Awards::dxccStatusBand(const int _ent, const int _band, const int _logNumber) //-1 error / 0 Not worked / 1 worked / 2 confirmed
{
    //-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << "Awards::dxccStatusBand: " << QString::number(_ent) << "/" << QString::number(_band);

        QSqlQuery query = QSqlQuery();
        QString queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_logNumber));
        int status = 0;
 //qDebug() << "Awards::dxccStatusBand: " << queryString;
        if (query.exec(queryString))
        {
            while (query.next())
            {
                if ( query.isValid() )
                {
                    if((query.value(0).toString() == "Y") || (query.value(1).toString() == "Y"))         // Confirmed
                    {
                        query.finish();
                        return 2;
                    }
                    status = 1;
                }                                           // Not present => Not worked
            }
            query.finish();
        }
        else
        {
            //TODO: Manage the query error
              emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
              query.finish();
              return -1;
        }

       //qDebug() << "Awards::dxccStatusBand: return - 0.3";
        return status;                                       // if arrives to here decision => not worked
}

/*
int Awards::dxccStatusBand(const int _ent, const int _band, const int _logNumber) //-1 error / 0 Not worked / 1 worked / 2 confirmed
{
    //-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << "Awards::dxccStatusBand: " << QString::number(_ent) << "/" << QString::number(_band);

        QSqlQuery query = QSqlQuery();
        QString queryString = QString();

        queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND band='%2' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_logNumber));
        int status = 0;

        if (query.exec(queryString))
        {
            while (query.next())
            {
                if ( query.isValid() )
                {
                    if(query.value(0).toString() == "1")         // Confirmed
                    {
                        query.finish();
                        return 2;
                    }
                    else if(query.value(0).toString() == "0")    // Worked
                    {
                        if (status < 1)
                        {
                            status = 1;
                        }
                    }
                }                                           // Not present => Not worked
            }
            query.finish();
        }
        else
        {
            //TODO: Manage the query error
              emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
              query.finish();
              return -1;
        }

       //qDebug() << "Awards::dxccStatusBand: return - 0.3";
        return status;                                       // if arrives to here decision => not worked
}

*/

int Awards::dxccStatusMode(const int _ent, const int _mode, const int _logNumber) //-1 error / 0 Not worked / 1 worked / 2 confirmed
{
    //-1 error / 0 Not worked / 1 worked / 2 confirmed
         //qDebug() << "Awards::dxccStatusMode: " << QString::number(_ent) << "/" << QString::number(_mode);

        QSqlQuery query = QSqlQuery();
        QString queryString = QString();
        if (_mode == -1)
        {
            return -1;
        }
        int status = 0;
        queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND modeid='%2' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_mode)).arg(QString::number(_logNumber));


        if (query.exec(queryString))
        {
            while (query.next())
            {
                if ( query.isValid() )
                {
                    if((query.value(0).toString() == "Y") || (query.value(1).toString() == "Y"))         // Confirmed
                    {
                        query.finish();
                        return 2;
                    }
                    status = 1;
                }                                           // Not present => Not worked
            }
            query.finish();
        }
        else
        { // The query fails...
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            query.finish();
          //TODO: Manage the query error
            return -1;
        }
        return status;                                       // if arrives to here decision => not worked
}

/*
int Awards::dxccStatusMode(const int _ent, const int _mode, const int _logNumber) //-1 error / 0 Not worked / 1 worked / 2 confirmed
{
    //-1 error / 0 Not worked / 1 worked / 2 confirmed
         //qDebug() << "Awards::dxccStatusMode: " << QString::number(_ent) << "/" << QString::number(_mode);

        QSqlQuery query = QSqlQuery();
        QString queryString = QString();
        if (_mode == -1)
        {
            return -1;
        }
        int status = 0;
        queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND mode='%2' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_mode)).arg(QString::number(_logNumber));


        if (query.exec(queryString))
        {
            while (query.next())
            {
                if ( query.isValid() )
                {
                    if(query.value(0).toString() == "1")         // Confirmed
                    {
                        query.finish();
                        return 2;
                    }
                    else if(query.value(0).toString() == "0")    // Worked
                    {
                        if (status < 1)
                        {
                            status = 1;
                        }
                    }
                }                                           // Not present => Not worked
            }
            query.finish();
        }
        else
        { // The query fails...
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            query.finish();
          //TODO: Manage the query error
            return -1;
        }
        return status;                                       // if arrives to here decision => not worked
}
*/
void Awards::setManageModes(const bool _manageModes)
{
    manageModes = _manageModes;
}

bool Awards::updateDXCCBandsStatus(const int _logNumber)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_logNumber);
    //QList<EntityBandStatus> dxccStatus;
    QSqlQuery query;
    QString stringQuery = QString();
    bool sqlOK = false;
    //QString answer;
    if (_logNumber>0)
    {
        //stringQuery = QString("SELECT DISTINCT dxcc, bandid, qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc>0 AND lognumber='%1'ORDER BY dxcc").arg(_logNumber);
        stringQuery = QString("SELECT DISTINCT dxcc, bandid, qsl_rcvd, lotw_qsl_rcvd, id FROM log lognumber='%1'ORDER BY dxcc").arg(_logNumber);
    }
    else
    {
        stringQuery = QString("SELECT DISTINCT dxcc, bandid, qsl_rcvd, lotw_qsl_rcvd, id FROM log ORDER BY dxcc");
    }

    int qsos = 0;
    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " - exec query: " << query.lastQuery ();

        dxccStatusList.clear();
        while(query.next())
        {
            if (query.isValid())
            {
                qsos++;
                EntityBandStatus ent;
                if (query.value(0).toInt()==0)
                {
                    //qDebug() << Q_FUNC_INFO << " - Returning false for: QSOid" << QString::number(query.value(4).toInt());
                    return false;
                }
                ent.dxcc = query.value(0).toInt();

                ent.bandid = query.value(1).toInt();
                if ((query.value(2).toString () == "Y") || (query.value(3).toString () == "Y"))
                {
                    ent.confirmed = true;
                }
                else
                {
                    ent.confirmed = false;
                }
                dxccStatusList.append (ent);
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": query NOK: " << query.lastQuery ();
                query.finish();
                return false;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        return false;
    }
    query.finish();

    if ((dxccStatusList.length ()<1) && (qsos>0))
    {
        //qDebug() << Q_FUNC_INFO << " RETURN FALSE" ;
        // It may be the case that there are not dxcc
        return false;
    }
    else if (dxccStatusList.length ()<1)
    {
        //qDebug() << Q_FUNC_INFO << ": dxccStatusList length <1 ";
    }
    else if(qsos>0)
    {
                    //qDebug() << Q_FUNC_INFO << ": qsos>0 ";
    }
    else
    {
                    //qDebug() << Q_FUNC_INFO << ": ELSE";
    }

    //qDebug() << Q_FUNC_INFO << ": dxccStatusList length: " << QString::number(dxccStatusList.length ()) ;
    //qDebug() << Q_FUNC_INFO << ": QSOs: " << QString::number(qsos) ;
    return true;
}
