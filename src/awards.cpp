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
}

//void Awards::setAwardDXCC(const int _qsoId)
//{
       //qDebug() << "Awards::setAwardDXCC: _qsoId: " << QString::number(_qsoId);
//    dataProxy->setDXCCAwardStatus(_qsoId);
//}

//void Awards::setAwardWAZ(const int _qsoId)
//{
       //qDebug() << "Awards::setAwardWAZ: _qsoId: " << QString::number(_qsoId);
//    dataProxy->setWAZAwardStatus(_qsoId);
//}

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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
            query.finish();
            return 0;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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

bool Awards::isThisSpotConfirmed(EntityStatus _entityStatus)
{
    return (_entityStatus.status == confirmed);
}

int Awards::getDXStatus (EntityStatus _entityStatus)
{
    //qDebug() << Q_FUNC_INFO<< ": Entity: " << _qs.at(0) << "/ Band: " << _qs.at(1) << "/ Mode: " << _qs.at(2)  << "/ Log: " << _qs.at(3)  <<  QT_ENDL;
    // Receives:  QStringList _qs;
    //_qs << Entity << BandId << << ModeId << lognumber;

    // TODO: Maybe a status per band/mode... check how WSJTX is doing it
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



  //qDebug() << Q_FUNC_INFO<< ":  dxccEntity: " << QString::number(_entityStatus.dxcc);
    if (_entityStatus.dxcc<=0)
    {
      //qDebug() << Q_FUNC_INFO<< ":  dxccEntity <= 0, return -1";
        return -1;
    }

    bool checkingMode = true;
    if ( (_entityStatus.modeId==-1) || (manageModes==false))
    {
        checkingMode = false;
      //qDebug() << Q_FUNC_INFO<< ":  checkingMode = FALSE";
    }

    int wb = dxccStatusBand(_entityStatus.dxcc, _entityStatus.bandId, _entityStatus.logId); //-1 error / 0 Not worked / 1 worked / 2 confirmed
    int wm = -1;
    if (checkingMode)
    {
        wm = dxccStatusMode(_entityStatus.dxcc, _entityStatus.modeId, _entityStatus.logId); //-1 error / 0 Not worked / 1 worked / 2 confirmed
    }
   // int wm = dxccStatusMode(dxccEntity, _mode, _logNumber); //-1 error / 0 Not worked / 1 worked / 2 confirmed

    if (wm==-1)
    {
        checkingMode = false;
    }

    //qDebug() << Q_FUNC_INFO<< ":  wb=" << QString::number(wb) << " - wm=" << QString::number(wm);
    //qDebug() << Q_FUNC_INFO<< ":  dxccStatus: " << QString::number(dxccStatus(dxccEntity, _logNumber));

    switch(dxccStatus(_entityStatus.dxcc, _entityStatus.logId))
    {
    case 0:
        //qDebug() << Q_FUNC_INFO<< ":  return 0";
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
                    //qDebug() << Q_FUNC_INFO<< ":  return 4";
                    return 4;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO<< ":  return 1";
                    return 1;
                }
            }
            else
            {
                //qDebug() << Q_FUNC_INFO<< ":  return 4 nc";
                return 4;
            }
        case 1:                     // Worked in this band
            if (checkingMode)
            {
                if (wm==1)
                {
                    //qDebug() << Q_FUNC_INFO<< ":  return 3";
                    return 3;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO<< ":  return 2";
                    return 2;
                }
            }
            else
            {
                //qDebug() << Q_FUNC_INFO<< ":  return 3 nc";
                return 3;
            }

        default:                    // ERROR
               //qDebug() << Q_FUNC_INFO<< ":  default return -1 - 1";
            return -1;
        }
    case 2:         // Confirmed
        if (wb==2)
        {
            if (checkingMode)
            {
                if (wm==2)
                {
                    //qDebug() << Q_FUNC_INFO<< ":  return 13";
                    return 13;
                }
                else if (wm==1)
                {
                    //qDebug() << Q_FUNC_INFO<< ":  return 12";
                    return 12;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO<< ":  return 11";
                    return 11;
                }
            }
            else
            {
                //qDebug() << Q_FUNC_INFO<< ": return 13 nc";
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
                        //qDebug() << Q_FUNC_INFO<< ": return 8";
                        return 8;
                    }
                    else
                    {
                        //qDebug() << Q_FUNC_INFO<< ": return 6";
                        return 6;
                    }
                }
                else
                {
                    if (wm==1)
                    {
                        //qDebug() << Q_FUNC_INFO<< ": return 7";
                        return 7;
                    }
                    else
                    {
                        //qDebug() << Q_FUNC_INFO<< ": return 5";
                        return 5;
                    }
                }
            }
            else
            {
                if (wb==0)
                {
                    //qDebug() << Q_FUNC_INFO<< ": return 7 nc";
                    return 7;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO<< ": return 8 nc";
                    return 8;
                }
            }
        }
        else
        {
            //qDebug() << Q_FUNC_INFO<< ": return -1 - 2";
            return -1;
        }
    default:
        //qDebug() << Q_FUNC_INFO<< ": return -1 default2";
        return -1;
    }   // END OF SWITCH
    //qDebug() << Q_FUNC_INFO<< ": END, return -1";
    //return -1;
}

int Awards::dxccStatusBandMode(const int _ent, const int _band, const int _mode, const int _logNumber, bool _checkingMode)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_ent) << "/" << QString::number(_band) << "/" << QString::number(_mode);
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();

    if (_checkingMode)
    {
           //qDebug() << Q_FUNC_INFO << ": Checking Mode TRUE";
        queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2' AND modeid='%3' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_mode)).arg(QString::number(_logNumber));
    }
    else
    {
           //qDebug() << Q_FUNC_INFO << ": Checking Mode FALSE";
        queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2' AND lognumber='%3'").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_logNumber));
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
                       //qDebug() << Q_FUNC_INFO << ": return - 2";
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
           //qDebug() << Q_FUNC_INFO << ": return - -1";
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
           //qDebug() << Q_FUNC_INFO << ": return - 0-4";
    return status;
}

int Awards::dxccStatus(const int _ent, const int _logNumber)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_ent);
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();
    int worked = 0;

    queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND lognumber='%2' ").arg(QString::number(_ent)).arg(QString::number(_logNumber));

    if (query.exec(queryString))
    {
           //qDebug() << Q_FUNC_INFO << ": query exec OK: " << query.lastQuery();
        while (query.next())
        {
               //qDebug() << Q_FUNC_INFO << ": query VALUE: " << (query.value(0)).toString();
            if ( query.isValid() )
            {
                   //qDebug() << Q_FUNC_INFO << ": query valid OK";
                if(((query.value(0)).toString() == "Y") || ((query.value(1)).toString() == "Y"))
                {
                       //qDebug() << Q_FUNC_INFO << ": value = 1 - return 2";
                    query.finish();
                    return 2;
                }
                worked = 1;
            }
        }
           //qDebug() << Q_FUNC_INFO << ": return "<< QString::number(worked) ;
        query.finish();
           //qDebug() << Q_FUNC_INFO << ": END: " << QString::number(worked);
        return worked;
    }
    else
    { // The query fails...
           //qDebug() << Q_FUNC_INFO << ": return -1" ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return -1;
    }
       //qDebug() << Q_FUNC_INFO << ": return 0" ;
    //return worked;
}

QColor Awards::getQRZDXStatusColor(EntityStatus _entitystatus)
{

  //qDebug() << Q_FUNC_INFO << " - Start: " ;
  //qDebug() << Q_FUNC_INFO << " - Entityd: " << _entitystatus.dxcc;
  //qDebug() << Q_FUNC_INFO << " - BandId:  " << _entitystatus.bandId;
  //qDebug() << Q_FUNC_INFO << " - ModeId:  " << _entitystatus.modeId;
  //qDebug() << Q_FUNC_INFO << " - Log:     " << _entitystatus.logId;


    /*
    0 - New One
    1 - Needed
    2 - Worked
    3 - Confirmed
    */

    QColor returnedColor;

    int status = getDXStatus(_entitystatus);


  //qDebug() << Q_FUNC_INFO<< ":  status: " << QString::number(status) << "/" << getDXStatusString(status);
  //qDebug() << Q_FUNC_INFO<< ":  status: " << QString::number(status);

    switch (status) {
        case 0:
            //qDebug() << Q_FUNC_INFO<< ":  returning newOneColor";
            returnedColor = newOneColor;
        break;
        case 1:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 2:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 3:
            //qDebug() << Q_FUNC_INFO<< ":  returning workedColor";
            returnedColor =  workedColor;
        break;
        case 4:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 5:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 6:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 7:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 8:
            //qDebug() << Q_FUNC_INFO<< ":  returning workedColor";
            returnedColor =  workedColor;
        break;
        case 9:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 10:
            //qDebug() << Q_FUNC_INFO<< ":  returning workedColor";
            returnedColor =  workedColor;
        break;
        case 11:
            //qDebug() << Q_FUNC_INFO<< ":  returning neededColor";
            returnedColor =  neededColor;
        break;
        case 12:
            //qDebug() << Q_FUNC_INFO<< ":  returning workedColor";
            returnedColor =  workedColor;
        break;
        case 13:
            //qDebug() << Q_FUNC_INFO<< ":  returning confirmedColor";
            returnedColor =  confirmedColor;
        break;
        //break;
        default:
            //qDebug() << Q_FUNC_INFO<< ":  returning defaultColor";
            returnedColor =  defaultColor;
        break;
    }
    return returnedColor;
}

QSOStatus Awards::getQSOStatus(const int &_status)
{
    switch (_status) {
    case 0:
        return ATNO;
        break;
    case 1:
        return needed;
        break;
    case 2:
        return needed;
        break;
    case 3:
        return worked;
        break;
    case 4:
        return needed;
        break;
    case 5:
        return needed;
        break;
    case 6:
        return needed;
        break;
    case 7:
        return needed;
        break;
    case 8:
        return worked;
        break;
    case 9:
        return needed;
        break;
    case 10:
        return worked;
        break;
    case 11:
        return needed;
        break;
    case 12:
        return worked;
        break;
    case 13:
        return confirmed;
        break;
    //break;
    default:
        return unknown;
        break;
    }
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

    EntityStatus aux;

    foreach (aux, dxccStatusList)
    {
         //qDebug() << Q_FUNC_INFO << " DXCC: " << QString::number(aux.dxcc);
         //qDebug() << Q_FUNC_INFO << " Band: " << QString::number(aux.bandid);
         if (aux.status == confirmed)
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
            if (aux.bandId == _band)
            {
                //qDebug() << Q_FUNC_INFO << " Band found: " << QString::number(_band);
                if (aux.status == confirmed )
                {
                   //qDebug() << Q_FUNC_INFO << " Confirmed " ;
                    return "C";
                }
                else if (aux.status == worked)
                {
                   //qDebug() << Q_FUNC_INFO << " Worked " ;
                    return "W";
                }
                else {return "-";}
                if (aux.status == confirmed)
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return "-";
    }
    query.finish();
    return status;
}

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

void Awards::setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default)
{
    //qDebug() << Q_FUNC_INFO << ": " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default;

    defaultColor = _default;
    neededColor = _needed;
    workedColor = _worked;
    confirmedColor = _confirmed;
    newOneColor = _newOne;
}

QColor Awards::getDefaultColor(){return defaultColor;}


void Awards::recalculateAwards()
{
/*
  TODO: I need to optimize this function.
    The select & insert of setAwardDXCC are too slow)
*/
    //qDebug() << Q_FUNC_INFO;
    dataProxy->updateAwardDXCC();
    emit awardDXCCUpdated();
    dataProxy->updateAwardWAZ();
    //qDebug() << Q_FUNC_INFO << " - END";
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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

void Awards::setAwards(const int _qsoId)
{
       //qDebug() << "Awards::setAwards: _qsoId: " << QString::number(_qsoId);
    dataProxy->setDXCCAwardStatus(_qsoId);
    dataProxy->setWAZAwardStatus(_qsoId);
}

int Awards::setDXCCToQSO(const int _dxcc, const int _qsoid) // Defines the DXCC in a QSO
{
       //qDebug() << Q_FUNC_INFO << ": " << QString::number(_dxcc) << "/" << QString::number(_qsoid);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
           //qDebug() << Q_FUNC_INFO << ": DXCC Updated in Log but failed....";
       int errorCode = query.lastError().text().toInt();
        query.finish();
           //qDebug() << Q_FUNC_INFO << ": LastQuery: " << query.lastQuery() ;
           //qDebug() << Q_FUNC_INFO << ": LastError-data: " << query.lastError().databaseText() ;
           //qDebug() << Q_FUNC_INFO << ": LastError-driver: " << query.lastError().driverText() ;
           //qDebug() << Q_FUNC_INFO << ": LastError-n: " << QString::number(query.lastError().text() );
        return errorCode;
    }
}

int Awards::setCQToQSO(const int _cqz, const int _qsoid) // Defines the CQ in a QSO
{
       //qDebug() << Q_FUNC_INFO << ": " << QString::number(_cqz) << "/" << QString::number(_qsoid);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
           //qDebug() << Q_FUNC_INFO << ": DXCC Updated in Log but failed....";
        int errorCode = query.lastError().text().toInt();
        query.finish();
           //qDebug() << Q_FUNC_INFO << ": LastQuery: " << query.lastQuery() ;
           //qDebug() << Q_FUNC_INFO << ": LastError-data: " << query.lastError().databaseText() ;
           //qDebug() << Q_FUNC_INFO << ": LastError-driver: " << query.lastError().driverText() ;
           //qDebug() << Q_FUNC_INFO << ": LastError-n: " << QString::number(query.lastError().text() );
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
       //qDebug() << Q_FUNC_INFO << ":  " << QString::number(_year);
    return dxMarathon->getDXMarathonCQ(_year, _logNumber);
}

int Awards::getDXMarathonScore(const int _year, const int _logNumber)
{
       //qDebug() << Q_FUNC_INFO << ":  " << QString::number(_year);
    return dxMarathon->getDXMarathonScore(_year, _logNumber);
}

bool Awards::isDXMarathonNeed(const int _dxcc, const int _cq, const int _year, const int _logNumber)
{
    return dxMarathon->neededForDXMarathon(_dxcc, _cq, _year, _logNumber);
}

int Awards::dxccStatusBand(const int _ent, const int _band, const int _logNumber) //-1 error / 0 Not worked / 1 worked / 2 confirmed
{
    //-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_ent) << "/" << QString::number(_band);

        QSqlQuery query = QSqlQuery();
        QString queryString = QString("SELECT DISTINCT qsl_rcvd, lotw_qsl_rcvd FROM log WHERE dxcc='%1' AND bandid='%2' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_logNumber));
        int status = 0;
 //qDebug() << Q_FUNC_INFO << ": " << queryString;
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
              emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
              query.finish();
              return -1;
        }
       //qDebug() << Q_FUNC_INFO << ": return - 0.3";
        return status;                                       // if arrives to here decision => not worked
}


int Awards::dxccStatusMode(const int _ent, const int _mode, const int _logNumber) //-1 error / 0 Not worked / 1 worked / 2 confirmed
{
    //-1 error / 0 Not worked / 1 worked / 2 confirmed
         //qDebug() << Q_FUNC_INFO << ": " << QString::number(_ent) << "/" << QString::number(_mode);

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
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            query.finish();
          //TODO: Manage the query error
            return -1;
        }
        return status;                                       // if arrives to here decision => not worked
}

void Awards::setManageModes(const bool _manageModes)
{
    manageModes = _manageModes;
}

bool Awards::updateDXCCBandsStatus(const int _logNumber)
{
   //qDebug() << Q_FUNC_INFO << ": " << QString::number(_logNumber);
    QSqlQuery query;
    QString stringQuery = QString();

    if (_logNumber>0)
    {
        stringQuery = QString("SELECT DISTINCT dxcc, bandid, modeid, qsl_rcvd, lotw_qsl_rcvd, id FROM log WHERE lognumber= :lognumber ORDER BY dxcc");
    }
    else
    {
        stringQuery = QString("SELECT DISTINCT dxcc, bandid, modeid, qsl_rcvd, lotw_qsl_rcvd, id FROM log ORDER BY dxcc");
    }
    if (_logNumber>0)
    {
        if (!query.prepare(stringQuery))
            return false;
        query.bindValue(":lognumber", _logNumber);
    }


    if (!executeQuery(query, stringQuery))
    {
            return false;
    }

    dxccStatusList.clear();


    int qsos = processQueryResults(query);
    populateDXCCStatusMap(); // Populate the multi-hash map

    query.finish();
    return !(dxccStatusList.isEmpty() && qsos > 0);
}

void Awards::populateDXCCStatusMap()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    dxccStatusMap.clear(); // Clear the multi-hash map
    for (const EntityStatus &status : dxccStatusList) {
        dxccStatusMap.insert(status.dxcc, status);
    }
}

QList<EntityStatus> Awards::findEntityStatusByDXCC(int dxcc) const
{
    return dxccStatusMap.values(dxcc); // Return all EntityStatus instances for the given dxcc
}

QSOStatus Awards::getStatus(const QSqlQuery &query, const QSqlRecord &rec)
{
    if ((query.value(rec.indexOf("qsl_rcvd")).toString() == "Y") ||
        (query.value(rec.indexOf("lotw_qsl_rcvd")).toString() == "Y"))
    {
        return QSOStatus::confirmed;
    } else
    {
        return QSOStatus::worked;
    }
}

int Awards::processQueryResults(QSqlQuery &query)
{
    int qsos = 0;

    while (query.next()) {
        if (!query.isValid()) {
            emit queryError(Q_FUNC_INFO, "Invalid query result", "", query.lastQuery());
            return qsos;
        }

        qsos++;
        EntityStatus ent;
        QSqlRecord rec = query.record();

        ent.dxcc = query.value(rec.indexOf("dxcc")).toInt();
        ent.bandId = query.value(rec.indexOf("bandid")).toInt();
        ent.modeId = query.value(rec.indexOf("modeid")).toInt();
        ent.status = getStatus(query, rec);
        ent.qsoId = query.value(rec.indexOf("id")).toInt();

        dxccStatusList.append(ent);
    }

    return qsos;
}

bool Awards::executeQuery(QSqlQuery &query, const QString &stringQuery)
{
    if (!query.exec(stringQuery)) {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    return true;
}
