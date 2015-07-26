#include "awards.h"


/***************************************************************************
                          awards.cpp  -  description
                             -------------------
    begin                : nov 2011
    copyright            : (C) 2011 by Jaime Robles
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

//#include <QDebug>

Awards::Awards()
{
    //qDebug() << "Awards::Awards"  << endl;
    world = new World();
    dataProxy = new DataProxy_SQLite();
    dxMarathon = new DXMarathon();
    //world->create();

    //newOneColor = Qt::green;
    //neededColor = Qt::darkGreen;
    //workedColor = Qt::magenta;
    //confirmedColor = Qt::red;
    //defaultColor = Qt::gray;

    newOneColor = Qt::black;
    neededColor = Qt::black;
    workedColor = Qt::black;
    confirmedColor = Qt::black;
    defaultColor = Qt::black;

    dxccWorked.clear();
    dxccConfirmed.clear();
    wazWorked.clear();
    wazConfirmed.clear();
}

Awards::~Awards()
{

}

void Awards::setAwardDXCC(const int _qsoId)
{
    //qDebug() << "Awards::setAwardDXCC: _qsoId: " << QString::number(_qsoId) << endl;
    QString stringQuery;
    QSqlQuery query;
    int nameCol;
    QString _ln;
    bool sqlOK;
    //bool __confirmed = dataProxy->isQSLReceived(_qsoId);


    QString  _band, _mode, _confirmed, _qsoid, _refid, _refcon, _dxcc;
    _qsoid = QString::number(_qsoId);

    stringQuery = QString("SELECT dxcc, bandid, modeid, qsl_rcvd, lognumber FROM log WHERE id='%1'").arg(_qsoid);
    //qDebug() << "Awards::setAwardDXCC: stringQuery: " << stringQuery << endl;

    sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        //qDebug() << "Awards::setAwardDXCC: 1 exec NOK: " << stringQuery << endl;
    }

    query.next();
    QSqlRecord rec = query.record();

}

void Awards::setAwardWAZ(const int _qsoId)
{
    //qDebug() << "Awards::setAwardWAZ: _qsoId: " << QString::number(_qsoId) << endl;
    QString stringQuery;
    QSqlQuery query;
    int nameCol;
    QString _ln;

    QString _cqz, _band, _mode, _confirmed, _qsoid, _refid, _refcon;
    _qsoid = QString::number(_qsoId);

    stringQuery = QString("SELECT cqz, bandid, modeid, qsl_rcvd, lognumber FROM log WHERE id='%1'").arg(_qsoid);
    //qDebug() << "Awards::setAwardWAZ: stringQuery-0: " << stringQuery << endl;

    query.exec(stringQuery);
    query.next();
    QSqlRecord rec = query.record();


    if ( query.isValid()  )
    {
        nameCol = rec.indexOf("cqz");
        _cqz = (query.value(nameCol)).toString();
        nameCol = rec.indexOf("bandid");
        _band = (query.value(nameCol)).toString();
        nameCol = rec.indexOf("modeid");
        _mode = (query.value(nameCol)).toString();
        nameCol = rec.indexOf("qsl_rcvd");
        _confirmed = (query.value(nameCol)).toString();
        nameCol = rec.indexOf("lognumber");
        _ln = (query.value(nameCol)).toString();

        if (_confirmed == "Y")
        {
            _confirmed = "1" ;
        }
        else
        {
            _confirmed = "0";
        }
    }
   //qDebug() << "Awards::setAwardWAZ: " << _cqz << "/" << _band << "/" << _mode  << "/" << _confirmed << endl;

    stringQuery = QString("SELECT id, confirmed FROM awardwaz WHERE cqz='%1' AND band='%2' AND mode='%3' AND lognumber='%4'").arg(_cqz).arg(_band).arg(_mode).arg(_ln);
    //qDebug() << "Awards::setAwardWAZ: stringQuery:-1 " << stringQuery << endl;
    query.exec(stringQuery);
    query.next();
    rec = query.record();

    if (query.isValid())
    {
        nameCol = rec.indexOf("id");
        _refid = query.value(nameCol).toString();
        nameCol = rec.indexOf("confirmed");
        _refcon = (query.value(nameCol)).toString();

        if ( _refcon == "1")
        {
            // NOP -> Everything is already saved
        }
        else
        { // TODO: We need to modify the registry to save the confirmed status of the pair.
            if (_confirmed=="1")
            {
                stringQuery = QString("UPDATE awardwaz SET cqz='%1', band='%2', mode='%3', confirmed='%4', qsoid='%5' WHERE id='%6'").arg(_cqz).arg(_band).arg(_mode).arg(_confirmed).arg(_qsoid).arg(_refid );
                //qDebug() << "Awards::setAwardWAZ: stringQuery:-2 " << stringQuery << endl;
                query.exec(stringQuery);
            }
            else
            {
            }
        }
    }
    else
    {
        stringQuery = QString("INSERT INTO awardwaz (cqz, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_cqz).arg(_band).arg(_mode).arg(_confirmed).arg(_ln).arg(_qsoid);
        //qDebug() << "Awards::setAwardWAZ: stringQuery:-3 " << stringQuery << endl;
        query.exec(stringQuery);
    }
}

int Awards::getDXCCWorked(const int _logNumber)
{
    //qDebug() << "Awards::getDXCCWorked (logNumber): " << QString::number(_logNumber) << endl;
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT count (dxcc) from  (SELECT DISTINCT dxcc FROM awarddxcc WHERE lognumber='%1' AND dxcc <>'')").arg(_logNumber);

    sqlOK = query.exec(stringQuery);
    //qDebug() << "Awards::getDXCCWorked: stringQuery: " << stringQuery << endl;
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            //qDebug() << "Awards::getDXCCWorked: " << QString::number((query.value(0)).toInt()) << endl;
            return (query.value(0)).toInt();
        }
        else
        {
            //qDebug() << "Awards::getDXCCWorked: 0" << endl;
            return 0;
        }

    }
    else
    {
        //qDebug() << "Awards::getDXCCWorked: Query error" << endl;
        return 0;
    }

}

int Awards::getDXCCConfirmed(const int _logNumber)
{
    //qDebug() << "Awards::getDXCCConfirmed (logNumber): " << QString::number(_logNumber) << endl;
    QSqlQuery query;
    QString stringQuery;
    bool sqlOK;
    stringQuery = QString("SELECT COUNT (dxcc) FROM (SELECT DISTINCT dxcc FROM awarddxcc WHERE confirmed='1' AND lognumber='%1' AND dxcc <>'')").arg(_logNumber);
    sqlOK = query.exec(stringQuery);
    //qDebug() << "Awards::getDXCCWorked: stringQuery: " << stringQuery << endl;

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            //qDebug() << "Awards::getDXCCConfirmed: " << QString::number((query.value(0)).toInt()) << endl;
            return (query.value(0)).toInt();
        }
        else
        {
             //qDebug() << "Awards::getDXCCConfirmed: 0" << endl;
            return 0;
        }

    }
    else
    {
        //qDebug() << "Awards::getDXCCConfirmed: query error" << endl;
      return 0;
    }


}

int Awards::getWAZWorked(const int _logNumber)
{
    //qDebug() << "Awards::getWAZWorked (logNumber): " << QString::number(_logNumber) << endl;
    QSqlQuery query;
    QString stringQuery;
    //stringQuery = QString("SELECT count (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND qsl_rcvd='Y' AND lognumber='%1')").arg(_logNumber);
    stringQuery = QString("SELECT count (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND lognumber='%1')").arg(_logNumber);
    //stringQuery = QString("SELECT count (cqz) from  (SELECT DISTINCT cqz FROM awardwaz WHERE lognumber='%1' AND cqz <> '')").arg(_logNumber);
    query.exec(stringQuery);

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

int Awards::getWAZConfirmed(const int _logNumber)
{
    //qDebug() << "Awards::getWAZConfirmed (logNumber): " << QString::number(_logNumber) << endl;

    QSqlQuery query;
    QString stringQuery;
    //Usar la siguiente para el confirmed
    stringQuery = QString("SELECT count (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND qsl_rcvd='Y' AND lognumber='%1')").arg(_logNumber);
    //stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM awardwaz WHERE lognumber='%1' AND confirmed='1' AND cqz <> '')").arg(_logNumber);
    query.exec(stringQuery);

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


int Awards::getDXStatus (const QStringList _qs)
{
    //estoy aqui tratando de que salga el color adecuado

    //qDebug() << "Awards::getDXStatus: Call: " << _qs.at(0) << "/ Band: " << _qs.at(1) << "/ Mode: " << _qs.at(2)  << "/ Log: " << _qs.at(3)  <<  endl;
// Receives:  QStringList _qs;
//_qs << QRZ << BandId << << ModeId << lognumber;

/*
    -1 - Error.
    0 - New one.                                                        - newOneColor       ( mode=-1=> 0/newOneColor       )
    1 - worked in this mode, not this band                              - neededColor       ( mode=-1=> 1/neededColor       )
    2 - worked in this band, not this mode                              - workedColor       ( mode=-1=> 3/workedColor       )
    3 - worked in this band and mode                                    - workedColor       ( mode=-1=> 3/workedColor       )
    4 - confirmed in this mode, not worked in this band                 - neededColor       ( mode=-1=> 1/neededColor       )
    5 - confirmed in this band, not worked in this mode                 - neededColor       ( mode=-1=> 8/confirmedColor    )
    6 - confirmed in this mode, not confirmed, but worked in this band  - workedColor       ( mode=-1=> 3/workedColor       )
    7 - confirmed in this band, not confirmed, but worked in this mode  - workedColor       ( mode=-1=> 8/confirmedColor    )
    8 - confirmed in this band and mode                                 - confirmedColor    ( mode=-1=> 8/confirmedColor    )
*/

/*
0   -   New One     -   Never worked before                         -   RED
1   -   Needed      -   Needed in this band                         -   ORANGE
3   -   Worked      -   Worked in this band but not confirmed       -   YELLOW
8   -   Confirmed   -   Confirmed in this band                      -   GREEN
*/


    if (_qs.length() != 4  )
    {
        return -1;
    }
    int errorCode = 0;

    int _band = _qs.at(1).toInt();
    int _mode = _qs.at(2).toInt();
    int _logNumber = _qs.at(3).toInt();
    int dxccEntity = world->getQRZARRLId(_qs.at(0) );

   //qDebug() << "Awards::getDXStatus: dxccEntity: " << QString::number(dxccEntity) << endl;


    bool checkingMode = true;
    if (_mode==-1)
    {
        checkingMode = false;
    }

    switch(dxccStatus(dxccEntity, _logNumber))
    { //-1 error / 0 Not worked / 1 worked / 2 confirmed
        case 0: // Not worked in any band                       => NEW ONE!
       //qDebug() << "Awards::getDXStatus: dxccStatus returned: 0" << endl;
            return 0;
        break;
        case 1: // Worked in another band, but not confirmed    => Could be new one, but it has been worked
       //qDebug() << "Awards::getDXStatus: dxccStatus returned 1: " << endl;
            switch (dxccStatusBandMode (dxccEntity, _band, _mode, _logNumber, checkingMode))
            {// Status in this band & Mode? //-1 error / 0 Not worked / 1 worked / 2 confirmed

                case 0: //
                    qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned 0(not worked): " << endl;
                    return 1;
                break;
                case 1: //
                    qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned 1(worked): " << endl;
                    return 2;
                break;
                case 2: //
                    qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned 2(confirmed): " << endl;
                    return 2;
                break;
                default:
                    qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned default: " << endl;
                    return -1;
                break;
            }
        break;
        case 2: // Confirmed
       //qDebug() << "Awards::getDXStatus: dxccStatus returned 2: " << endl;
            switch (dxccStatusBandMode (dxccEntity, _band, _mode, _logNumber, checkingMode))
            {// Status in this band & Mode? //-1 error / 0 Not worked / 1 worked / 2 confirmed
                case 0: // Not worked in this band&Mode
               //qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned 0 but I translate into 1: " << endl;
                    return 1;
                break;
                case 1: // Worked in this band&Mode
               //qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned 1 but I translate into 2: " << endl;
                    return 2;
                break;
                case 2: // Confirmed in this band&Mode
               //qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned 2 but I translate into 3: " << endl;
                    return 3;
                break;
                default:
               //qDebug() << "Awards::getDXStatus: dxccStatusBandMode returned default2: " << endl;
                    return -1;
                break;
            }
        break;
        default:
       //qDebug() << "Awards::getDXStatus: dxccStatus returned default: " << endl;
            return -1;
        break;
    }
    return -1;
}
int Awards::dxccStatusBandMode(const int _ent, const int _band, const int _mode, const int _logNumber, bool _checkingMode)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
   //qDebug() << "Awards::dxccStatusBandMode: " << QString::number(_ent) << "/" << QString::number(_band) << "/" << QString::number(_mode) << endl;
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();

    if (_checkingMode)
    {
       //qDebug() << "Awards::dxccStatusBandMode: Checking Mode TRUE" << endl;
        queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND band='%2' AND mode='%3' AND lognumber='%4' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_mode)).arg(QString::number(_logNumber));
    }
    else
    {
       //qDebug() << "Awards::dxccStatusBandMode: Checking Mode FALSE" << endl;
        queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND band='%2' AND lognumber='%3' ").arg(QString::number(_ent)).arg(QString::number(_band)).arg(QString::number(_logNumber));
    }


    if (query.exec(queryString))
    {
        if (query.next())
        {
            if ( query.isValid() )
            {
                if(query.value(0).toInt() == 1)
                {
                    return 2;
                }
                else if(query.value(0).toInt() == 0)
                {
                    return 1;
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
        else
        { // No value => Not Worked
            return 0;
        }

    }
    else
    { // The query fails...
      //TODO: Manage the query error
        return -1;
    }

    return 0;
}


int Awards::dxccStatus(const int _ent, const int _logNumber)
{//-1 error / 0 Not worked / 1 worked / 2 confirmed
    //qDebug() << "Awards::dxccStatus: " << QString::number(_ent) << endl;
    QSqlQuery query = QSqlQuery();
    QString queryString = QString();
    int worked = 0;

    queryString = QString("SELECT confirmed FROM awarddxcc WHERE dxcc='%1' AND lognumber='%2' ").arg(QString::number(_ent)).arg(QString::number(_logNumber));

    if (query.exec(queryString))
    {
        while (query.next())
        {
            if ( query.isValid() )
            {
                if(query.value(0).toInt() == 1)
                {
                    return 2;
                }
                else if(query.value(0).toInt() == 0)
                {
                    worked = 1;
                }
            }
        }
        return worked;

    }
    else
    { // The query fails...
      //TODO: Manage the query error
        return -1;
    }
    return 0;
}

QColor Awards::getQRZDXStatusColor(const QStringList _qs)
{
    qDebug() << "Awards::getQRZDXStatusColor qs.length: " << QString::number(_qs.length()) << endl;

    //From Search QSO to QSL: q << _call << bandid << _mode << QString::number(currentLog);


    //qDebug() << "Awards::getQRZDXStatusColor: " << _qs.at(0) << "/" << _qs.at(1) << "/" << _qs.at(2) << _qs.at(3) << endl;
    // Receives:  QStringList _qs;
    //_qs << QRZ << BandID << ModeId << lognumber;

/*
 -1 - Error.
  0 - New one.                                                        - newOneColor       ( mode=-1=> 0/newOneColor       )
  1 - worked in this mode, not this band                              - neededColor       ( mode=-1=> 1/neededColor       )
  2 - worked in this band, not this mode                              - workedColor       ( mode=-1=> 3/workedColor       )
  3 - worked in this band and mode                                    - workedColor       ( mode=-1=> 3/workedColor       )
  4 - confirmed in this mode, not worked in this band                 - neededColor       ( mode=-1=> 1/neededColor       )
  5 - confirmed in this band, not worked in this mode                 - neededColor       ( mode=-1=> 8/confirmedColor    )
  6 - confirmed in this mode, not confirmed, but worked in this band  - workedColor       ( mode=-1=> 3/workedColor       )
  7 - confirmed in this band, not confirmed, but worked in this mode  - workedColor       ( mode=-1=> 8/confirmedColor    )
  8 - confirmed in this band and mode                                 - confirmedColor    ( mode=-1=> 8/confirmedColor    )
*/

    QColor returnedColor;
    int status = getDXStatus(_qs);
    qDebug() << "Awards::getQRZDXStatusColor: status: " << QString::number(status) << endl;
    switch (status) {

        case 0:
            returnedColor = newOneColor;
            qDebug() << "Awards::getQRZDXStatusColor: 0-new: " << returnedColor.name() << endl;

        break;
        case 1:
            returnedColor =  neededColor;
            qDebug() << "Awards::getQRZDXStatusColor: 1-needed: " << returnedColor.name() << endl;
        break;
        case 2:
            returnedColor =  workedColor;
            qDebug() << "Awards::getQRZDXStatusColor: 2-worked: " << returnedColor.name() << endl;
        break;
        case 3:
            returnedColor =  workedColor;
            qDebug() << "Awards::getQRZDXStatusColor: 3-worked: " << returnedColor.name() << endl;
        break;
        case 4:
            returnedColor =  neededColor;
            qDebug() << "Awards::getQRZDXStatusColor: 4-needed: " << returnedColor.name() << endl;
        break;
        case 5:
            returnedColor =  neededColor;
            qDebug() << "Awards::getQRZDXStatusColor: 5-needed: " << returnedColor.name() << endl;
        break;
        case 6:
            returnedColor =  workedColor;
            qDebug() << "Awards::getQRZDXStatusColor: 6-worked: " << returnedColor.name() << endl;
        break;
        case 7:
            returnedColor =  workedColor;
            qDebug() << "Awards::getQRZDXStatusColor: 7-worked: " << returnedColor.name() << endl;
        break;
        case 8:
            returnedColor =  confirmedColor;
            qDebug() << "Awards::getQRZDXStatusColor: 8-confirmed: " << returnedColor.name() << endl;
        break;
        default:
            returnedColor =  defaultColor;
            qDebug() << "Awards::getQRZDXStatusColor: Def: " << returnedColor.name() << endl;
        break;

    }
    return returnedColor;
}




QString Awards::checkIfValidIOTA(const QString _tiota)
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
    //qDebug() << "Awards::checkIfValidIOTA: " << _tiota << endl;
    bool _valid = false;
    QString _continent;
    QString _number;

    if (_tiota.count("-") == 1)
    {
        QStringList _values = _tiota.split("-", QString::SkipEmptyParts);
        _continent = _values.at(0);
        _number = _values.at(1);
    }
    else
    {
        return "";
    }

    //qDebug() << "Awards::checkIfValidIOTA (cont) " << _continent << endl;
    //qDebug() << "Awards::checkIfValidIOTA (numb): " << _number << endl;

    // Check if continent is valid
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
}


void Awards::setColors (const QString _newOne, const QString _needed, const QString _worked, const QString _confirmed, const QString _default)
//void Awards::setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default)
{
    //qDebug() << "Awards::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default << endl;

    defaultColor = QColor(_default.toUpper());
    neededColor = QColor(_needed.toUpper());
    workedColor = QColor(_worked.toUpper());
    confirmedColor = QColor(_confirmed.toUpper());
    newOneColor = QColor(_newOne.toUpper());

    if (defaultColor.isValid())
    {
        defaultColor.setNamedColor(_default.toUpper());
    }
    else
    {
    }

    if (neededColor.isValid())
    {
        neededColor.setNamedColor(_needed.toUpper());
    }
    else
    {
    }


    if (confirmedColor.isValid())
    {
        confirmedColor.setNamedColor(_confirmed.toUpper());
    }
    else
    {
    }

    if (newOneColor.isValid())
    {
        newOneColor.setNamedColor(_newOne.toUpper());
    }
    else
    {     
    }

    if (workedColor.isValid())
    {
        workedColor.setNamedColor(_worked.toUpper());
    }
    else
    {
        //qDebug() << "Awards::setColors: Worked NOT VALID" << endl;
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

    //qDebug() << "Awards::recalculateAwards" << endl;
    //select distinct dxcc,  band, mode from awarddxcc
    QList<QString> awardsQList;
    awardsQList.clear();

    QSqlDatabase db = QSqlDatabase::database();

    //1.- Delete ALL the data in awarddxcc and awardwaz
    QSqlQuery query, queryInsert, queryCQZ, queryUpdate;
    QString queryString, aux, _call, _qslStatus;
    bool cancelPressed = false;
    bool CQZknown = false;
    bool CQZknownBefore = false;
    bool DXCCknown = false;
    bool DXCCknownBefore = false;
    bool queryOk = false;
    bool queryOkcq = false;
    int errorCode = -1;
    int nameCol, _id, _dxcc, _band, _mode, _cqz, _logNumber, _confirmed;


    int i = 0;
    int step = 1;
    int numberOfQsos = 0;
    if (query.exec("SELECT MAX(id) FROM log"))
    {
        query.next();
        numberOfQsos = query.value(0).toInt();
        //qDebug() << "Awards::recalculateAwards: MAX = " << QString::number(numberOfQsos) << endl;
        //step = getProgresStepForDialog(numberOfQsos);
        step = util.getProgresStepForDialog(numberOfQsos);

    }
    else
    {
        //qDebug() << "Awards::recalculateAwards: Query FAILED so MAX = " << QString::number(numberOfQsos) << endl;
    }


    query.exec("DELETE FROM awarddxcc");
    query.exec("DELETE FROM awardwaz");

    QProgressDialog progress("Updating the award status... " + QString::number(numberOfQsos) + " QSOs", "Abort Update", 0, numberOfQsos);
    progress.setWindowModality(Qt::WindowModal);

    queryString = QString("SELECT dxcc, call, bandid, modeid, qsl_rcvd, lognumber, cqz, id FROM log");
    if (query.exec(queryString))
    {
        QSqlRecord rec = query.record();

        db.transaction();

        while ( (query.next()) && (!cancelPressed))
        {
            i++;
            DXCCknownBefore = false;
            CQZknownBefore = false;
            nameCol = rec.indexOf("call");
            _call = (query.value(nameCol)).toString();


            nameCol = rec.indexOf("id");
            _id = (query.value(nameCol)).toInt();


            nameCol = rec.indexOf("dxcc");
            _dxcc = (query.value(nameCol)).toInt();
            //qDebug() << "Awards::recalculateAwards: CALL/ID/DXCC: " << _call << "/"<< _id << "/" << QString::number(_dxcc) << endl;

            if (_dxcc<=0)
            {
                //qDebug() << "Awards::recalculateAwards: DXCC not known for id: " << QString::number(_id) << "Searching..." << endl;
                _dxcc = world->getQRZARRLId(_call);
                if (_dxcc<=0)
                {
                    //qDebug() << "Awards::recalculateAwards: DXCC not known for id: " << QString::number(_id) << endl;
                    DXCCknown = false;

                }
                else
                {
                    DXCCknown = true;
                    //qDebug() << "Awards::recalculateAwards: DXCC found " << QString::number(_id) << "/" << QString::number(_dxcc)<< endl;
                }
            }
            else
            {
                DXCCknown = true;
                DXCCknownBefore = true;
                //qDebug() << "Awards::recalculateAwards: DXCC already known for id: " << QString::number(_id)  << " = " << QString::number(_dxcc) << endl;
            }

            //qDebug() << "Awards::recalculateAwards: CALL/ID/DXCC-2: " << _call << "/"<< _id << "/" << QString::number(_dxcc) << endl;

            nameCol = rec.indexOf("bandid");
            _band = (query.value(nameCol)).toInt();

            nameCol = rec.indexOf("modeid");
            _mode = (query.value(nameCol)).toInt();

            nameCol = rec.indexOf("qsl_rcvd");
            _qslStatus = (query.value(nameCol)).toString();
            if (_qslStatus == "Y")
            {
                _confirmed = 1 ;
            }
            else
            {
                _confirmed = 0;
            }

            //qDebug() << "Awards::recalculateAwards: Recalculating CQZ " << endl;

            nameCol = rec.indexOf("cqz");
            _cqz = (query.value(nameCol)).toInt();
            //awardsQList << QString::number(_cqz);
            if ((_cqz<=0) && (_dxcc>0))
            {
                //qDebug() << "Awards::recalculateAwards: CQZ not known for id: " << QString::number(_id) << "Searching..." << endl;
                _cqz = world->getEntityCqz(_dxcc);
                if (_cqz<=0)
                {
                //qDebug() << "Awards::recalculateAwards: CQZ not known for id: " << QString::number(_id) << endl;
                    CQZknown = false;
                }
                else
                {
                    CQZknown = true;
                //qDebug() << "Awards::recalculateAwards: CQZ found " << QString::number(_id) << "/" << QString::number(_cqz)<< endl;
                }
            }
            else if ((_cqz<=0) && (_dxcc<=0))
            {
                //qDebug() << "Awards::recalculateAwards: CQZ nor DXCC not known for id: " << QString::number(_id) << endl;
                CQZknown = false;
                DXCCknown = false;
            }
            else
            {
                CQZknown = true;
                CQZknownBefore = true;
                //qDebug() << "Awards::recalculateAwards: CQZ already known for id: " << QString::number(_id) << " = " << QString::number(_cqz) << endl;
            }

            nameCol = rec.indexOf("lognumber");
            _logNumber = (query.value(nameCol)).toInt();

            if (_dxcc>0)
            {
                awardsQList << QString::number(_dxcc);
                if (_cqz>0)
                {
                    awardsQList << QString::number(_cqz);
                }
                else
                {
                    awardsQList << QString::number(-1);
                }
                awardsQList << QString::number(_band);
                awardsQList << QString::number(_mode);
                awardsQList << QString::number(_confirmed);
                awardsQList << QString::number(_id);
                awardsQList << QString::number(_logNumber);
            }
// awardsQList << _dxcc / _cqz / _band / _mode / confirmed / id / logNumber
            if ((DXCCknown) && (!DXCCknownBefore))
            { //I should update the DXCC in the log as it was not known before and now it is!
                errorCode = setDXCCToQSO(_dxcc, _id);
            }
            if ((CQZknown) && (!CQZknownBefore))
            { //I should update the DXCC in the log as it was not known before and now it is!
                errorCode = setCQToQSO(_cqz, _id);
            }

            if (( (i % step ) == 0) )
            { // To update the speed I will only show the progress once each X QSOs

                aux = "Updating the award status...  "  + QString::number(i) + "/" + QString::number(numberOfQsos);
                progress.setLabelText(aux);
                progress.setValue(i);

            }
            else
            {

            }


            if (progress.wasCanceled())
            {
                cancelPressed = true;
            }

        } // END of While
        db.commit();

        query.prepare( "INSERT INTO awarddxcc (dxcc, band, mode, confirmed, lognumber, qsoid) VALUES (:dxcc, :band, :mode, :confirmed, :lognumber, :qsoid)" );
        queryCQZ.prepare( "INSERT INTO awardwaz (cqz, band, mode, confirmed, lognumber, qsoid) VALUES (:cqz, :band, :mode, :confirmed, :lognumber, :qsoid)" );

        db.transaction();
        //queryString = QString("SELECT dxcc, call, bandid, modeid, qsl_rcvd, lognumber, cqz, id FROM log");
        // awardsQList << _dxcc / _cqz / _band / _mode / confirmed / id / logNumber
        int _cqzt = -1;
        for (int i = 0; i < awardsQList.size(); i++) {

                query.bindValue( ":dxcc", awardsQList.at(i));
                _dxcc = (awardsQList.at(i)).toInt();

                i++;
                _cqzt = (awardsQList.at(i)).toInt(); // We read the CQz
                if (_cqzt>0) // CQ
                {
                    queryCQZ.bindValue( ":cqz", awardsQList.at(i));

                }

                i++;
                query.bindValue( ":band", awardsQList.at(i));
                _band = (awardsQList.at(i)).toInt();

                if (_cqzt>0) // CQ
                {
                    queryCQZ.bindValue( ":band", awardsQList.at(i));
                }

                i++;
                query.bindValue( ":mode", awardsQList.at(i));
                _mode = (awardsQList.at(i)).toInt();
                if (_cqzt>0) // CQ
                {
                    queryCQZ.bindValue( ":mode", awardsQList.at(i));
                }

                i++;
                query.bindValue( ":confirmed", awardsQList.at(i));
                _confirmed = (awardsQList.at(i)).toInt();
                //qDebug() << "Awards::recalculateAwards: Confirmed=" << QString::number(_confirmed) << endl;
                if (_cqzt>0) // CQ
                {
                    queryCQZ.bindValue( ":confirmed", awardsQList.at(i));
                }

                i++;                
                query.bindValue( ":qsoid", awardsQList.at(i));
                _id =(awardsQList.at(i)).toInt();
                if (_cqzt>0) // CQ
                {
                    queryCQZ.bindValue( ":qsoid", awardsQList.at(i));
                }

                i++;
                query.bindValue( ":lognumber", awardsQList.at(i));
                if (_cqzt>0) // CQ
                {
                    queryCQZ.bindValue( ":lognumber", awardsQList.at(i));
                }

            queryOk = query.exec(); // Add a DXCC


            if (queryOk)
            {// New worked QSO inserted in AwardDXCC
                //qDebug() << "Awards::recalculateAwards: queryOK!" << endl;
            }
            else
            {// Something happened while adding the new worked DXCC (may be already worked/confirmed or simply something went wrong)
                //qDebug() << "Awards::recalculateAwards: queryOK = False" << endl;
                errorCode = query.lastError().number();
                if (errorCode == 19)
                { // More than probably the SET band/mode/DXCC is already in the DB

                    //qDebug() << "Awards::recalculateAwards: queryOK = False - errorCode 19" << endl;
                    if (_confirmed == 1)
                    {
                        //qDebug() << "Awards::recalculateAwards: queryOK = False - errorCode 19 & _confirmed = true. Updating: " << _id << "..." << endl;
                        errorCode = setAwardDXCCConfirmed(_band, _mode, _dxcc, _id) ;
                        //qDebug() << "Awards::recalculateAwards: queryOK = False - errorCode 19 & _confirmed = true. Updated: " << errorCode << "..." << endl;
                    }
                    else
                    {
                        //qDebug() << "Awards::recalculateAwards: queryOK = False - errorCode 19 & _confirmed = false, no need to update anything: " << _id << endl;
                        //No consigo que se mantenga cuando se ha confirmado un pais
                    }


                }
                else
                {
                    //qDebug() << "Awards::recalculateAwards: LastQuery-2: " << query.lastQuery()  << endl;
                    //qDebug() << "Awards::recalculateAwards: LastError-data-2: " << query.lastError().databaseText()  << endl;
                    //qDebug() << "Awards::recalculateAwards: LastError-driver-2: " << query.lastError().driverText()  << endl;
                    //qDebug() << "Awards::recalculateAwards: LastError-n-2: " << QString::number(query.lastError().number() ) << endl;
                }

            }

            //queryOkcq = queryCQZ.exec(); // Add a CQz

        }

        db.commit();

    }
    else
    {
        return;
    }



    //qDebug() << "Awards::recalculateAwards - END" << endl;
}

int Awards::getQSOsInLog(const int _logNumber)
{
    //qDebug() << "Awards::getQSOsInLog: " << QString::number(_logNumber) << endl;
    QSqlQuery query;
    QString queryString;

    queryString = QString("SELECT COUNT (id) FROM log WHERE lognumber='%1'").arg(_logNumber) ;

    if( !query.exec(queryString) )
    {
        return -1;
    }
    else
    {
        query.next();
        if (query.isValid())
        {
            return query.value(0).toInt();
        }
        else
        {
            return -2;
        }
    }
}


void Awards::setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId)
{
    //qDebug() << "Awards::setAwards: " << QString::number(_dxcc) << endl;
/*
_workedOrConfirmed = -1     Remove this pair
_workedOrConfirmed = 0     Set as Worked
_workedOrConfirmed = 1     Set as Confirmed
*/
    int d = _dxcc;
    int b = _band;
    int m = _mode;
    int w = _workedOrConfirmed;
    int l = _logNumber;
    int i = _qsoId;
    //setAwardDXCC(_dxcc,  _band, _mode, _workedOrConfirmed, _logNumber);
    //
    //bool Awards::setAwardDXCC(const int _dxcc, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId)

    setAwardDXCC(d, b, m, w, l, i);
    setAwardWAZ(d, b, m, w, l, i);
    //setAwardWAZ(_waz,  _band,  _mode,  _workedOrConfirmed, _logNumber);

}


void Awards::setAwards(const int _qsoId)
{
    //qDebug() << "Awards::setAwards: _qsoId: " << QString::number(_qsoId) << endl;

    QString stringQuery, _confirmed, _call;
    QSqlQuery query;
    int nameCol;
    bool DXCCknown=false;
    bool CQZknown=false;
    int _dxcc, _cqz, _band, _mode, _logNumber, _alreadyConfirmed;
    bool _confirmedB = false;

    stringQuery = QString("SELECT dxcc, call, bandid, modeid, qsl_rcvd, lognumber, cqz FROM log WHERE id='%1'").arg(_qsoId);

    if (query.exec(stringQuery))
    {
        query.next();
        QSqlRecord rec = query.record();

        nameCol = rec.indexOf("call");
        _call = (query.value(nameCol)).toString();

        nameCol = rec.indexOf("dxcc");
        _dxcc = (query.value(nameCol)).toInt();

        nameCol = rec.indexOf("cqz");
        _cqz = (query.value(nameCol)).toInt();

        nameCol = rec.indexOf("bandid");
        _band = (query.value(nameCol)).toInt();

        nameCol = rec.indexOf("modeid");
        _mode = (query.value(nameCol)).toInt();

        nameCol = rec.indexOf("qsl_rcvd");
        _confirmed = (query.value(nameCol)).toString();

        nameCol = rec.indexOf("lognumber");
        _logNumber = (query.value(nameCol)).toInt();

        if (_dxcc <= 0)
        {
            _dxcc = world->getQRZARRLId(_call);
            if (_dxcc > 0)
            {
                DXCCknown = true;
            }
            else
            {
                DXCCknown = false;
            }
        }
        else
        {
            DXCCknown = true;
        }

        if ( (_cqz <= 0) && (DXCCknown) )
        {
            _cqz = world->getEntityCqz(_dxcc);
            if (_cqz > 0)
            {
                CQZknown = true;
            }
            else
            {
                CQZknown = false;
            }
        }
        else if (_cqz > 0)
        {
            CQZknown = true;
        }
        else
        {
            CQZknown = false;
        }


        if (_confirmed == "Y")
        {
            //qDebug() << "Awards::setAwards: QSO is CONFIRMED: " << QString::number(_qsoId)  << " = " << QString::number(_dxcc) << endl;
            _confirmed = "1" ;
            _confirmedB = true;
        }
        else
        {
            //qDebug() << "Awards::setAwards: QSO is only WORKED: " << QString::number(_qsoId)  << " = " << QString::number(_dxcc) << endl;
            _confirmed = "0";
            _confirmedB = false;
        }


    }

    else
    {
        //qDebug() << "Awards::setAwards: ERROR IN First Select: " << QString::number(_qsoId) << endl;
        //qDebug() << "Awards::setAwardDXCC: ERROR IFS: " << QString::number(_qsoId) << endl;
        //qDebug() << "Awards::setAwardDXCC: ERROR IFS Driver-error: " <<query.lastError().driverText() << endl;
        //qDebug() << "Awards::setAwardDXCC: ERROR IFS DB-error: " <<query.lastError().databaseText() << endl;
        //qDebug() << "Awards::setAwardDXCC: ERROR IFS lastQuery: " <<query.lastQuery() << endl;
        return;
    }

    if (DXCCknown)
    {
        //int setAwardDXCCst(const int _dxcc, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId);
        // int _dxcc, _cqz, _band, _mode, _logNumber,
        nameCol = setAwardDXCCst(_dxcc, _band,  _mode, _confirmedB, _logNumber,  _qsoId);
        //qDebug() << "Awards::setAwards: OutPutDXCC: " << QString::number(_qsoId) << " = " << QString::number(nameCol) << endl;
    }

    if (CQZknown)
    {
        nameCol = setAwardWAZst(_cqz, _band,  _mode, _confirmedB, _logNumber,  _qsoId);
        //qDebug() << "Awards::setAwards: OutPut (WAZ): " << QString::number(_qsoId) << " = " << QString::number(nameCol) << endl;
    }

}



bool Awards::setAwardDXCC(const int _dxcc, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId)
{
   //qDebug() << "Awards::setAwardDXCC(DXCC/BAND/MODE/WORKED/log/qsoid): " << QString::number(_dxcc) << "/" << QString::number(_band) << "/" << QString::number(_mode) << "/"<<  QString::number(_workedOrConfirmed) << "/" << QString::number(_logNumber) << "/" << QString::number(_qsoId) << endl;
   bool isOK1 = false;
   bool isOK2 = false;
   QString stringQuery;
   if (_dxcc<=0)
   {
       //qDebug() << "Awards::setAwardDXCC: DXCC < 0" << endl;
       return false;
   }

/*

     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
     "dxcc INTEGER NOT NULL,"
      "band INTEGER NOT NULL, "
     "mode INTEGER NOT NULL, "
       "confirmed INTEGER, "
     "qsoid INTEGER NOT NULL, "
      "lognumber INTEGER NOT NULL, "
*/
    QSqlQuery query;
    if (_dxcc>0)
    {
        stringQuery = QString("INSERT INTO awarddxcc (dxcc, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_dxcc).arg(_band).arg(_mode).arg(_workedOrConfirmed).arg(_logNumber).arg(_qsoId);
        if (query.exec(stringQuery) )
        {
           //qDebug() << "Awards::setAwardDXCC: OK DXCC _qsoId: " << QString::number(_qsoId) << endl;
           isOK1 = true;

        }
        else
        {
            //qDebug() << "Awards::setAwardDXCC: NOK DXCC _qsoId: " << QString::number(_qsoId) << endl;
            //qDebug() << "Awards::setAwardDXCC: NOK DXCC Driver-error: " <<query.lastError().driverText() << endl;
            //qDebug() << "Awards::setAwardDXCC: NOK DXCC DB-error: " <<query.lastError().databaseText() << endl;
            //qDebug() << "Awards::setAwardDXCC: NOK DXCC lastQuery: " <<query.lastQuery() << endl;

            return false;
        }

    }
    else
    {
        return false;
    }


// Second phase, the memory storage.
//TODO: Choose whether I use a DB or memory but both may not be optimal.
    int value = (QString::number(_band) + QString::number(_mode)).toInt();

    if (_workedOrConfirmed == -1)
    {
        dxccWorked.remove(_dxcc, value);
        dxccConfirmed.remove(_dxcc, value);
        isOK2 = true;
    }

    if ( dxccConfirmed.contains(_dxcc, value) )
    { // Already confirmed, nothing to do
        isOK2 = true;
    }
    else
    {
        if (dxccWorked.contains(_dxcc, value))
        { //It is worked
            if (_workedOrConfirmed == 1)
            {
                dxccConfirmed.insert(_dxcc, value);
                isOK2 = true;
            }
            else
            {
            }
        }
        else
        { // It is not worked!
            dxccWorked.insert(_dxcc, value);
            isOK2 = true;
        }
    }
    return (isOK1 && isOK2);
}



bool Awards::setAwardWAZ(const int _cqz, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId)
{
    //qDebug() << "Awards::setAwardWAZ: " << QString::number(_band) + QString::number(_mode) << endl;
   bool isOK1 = false;
    bool isOK2 = false;
/*

     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
     "dxcc INTEGER NOT NULL,"
      "band INTEGER NOT NULL, "
     "mode INTEGER NOT NULL, "
       "confirmed INTEGER, "
     "qsoid INTEGER NOT NULL, "
      "lognumber INTEGER NOT NULL, "
*/
    QSqlQuery query;
    if (_cqz>0)
    {
        QString stringQuery = QString("INSERT INTO awardwaz (cqz, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_cqz).arg(_band).arg(_mode).arg(_workedOrConfirmed).arg(_logNumber).arg(_qsoId);
        if (query.exec(stringQuery) )
        {
           //qDebug() << "Awards::setAwardWAZ: OK CQZ _qsoId: " << QString::number(_qsoId) << endl;
           isOK1 = true;

        }
        else
        {
           //qDebug() << "Awards::setAwardWAZ: NOK CQZ _qsoId: " << QString::number(_qsoId) << endl;
            return false;
        }

    }
    else
    {
        return false;
    }


// Second phase, the memory storage.
//TODO: Choose whether I use a DB or memory but both may not be optimal.
    int value = (QString::number(_band) + QString::number(_mode)).toInt();

    if (_workedOrConfirmed == -1)
    {
        wazWorked.remove(_cqz, value);
        wazConfirmed.remove(_cqz, value);
        isOK2 = true;
    }

    if ( wazConfirmed.contains(_cqz, value) )
    { // Already confirmed, nothing to do
        isOK2 = true;
    }
    else
    {
        if (wazWorked.contains(_cqz, value))
        { //It is worked
            if (_workedOrConfirmed == 1)
            {
                wazConfirmed.insert(_cqz, value);
                isOK2 = true;
            }
            else
            {
            }
        }
        else
        { // It is not worked!
            wazWorked.insert(_cqz, value);
            isOK2 = true;
        }
    }
    return (isOK1 && isOK2);
}


int Awards::setAwardDXCCst(const int _dxcc, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId)
{
   //qDebug() << "Awards::setAwardDXCCst-0: " << QString::number(_dxcc) << "/" << QString::number(_band) << "/" << QString::number(_mode) << "/" << QString::number(_logNumber) << "/" << QString::number(_qsoId) << endl;
    int nameCol=-1;
    QString _refid = QString();
    /*
    _confirmedQSO == false QSO is just worked
    _confirmedQSO == true QSO is confirmed

    */
    //TODO: Fix the way we check for data validity for this function
    if (!( (_dxcc>=0) && (_band >=0) && (_mode>=0) && (_logNumber>=0) && (_qsoId >=0) ))
    {
         //qDebug() << "Awards::setAwardDXCCst: Not valid data received!" << endl;
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

    //qDebug() << "Awards::setAwardDXCCst: _qsoId: " << QString::number(_qsoId) << "/" << QString::number(_iconfirmed) << endl;
    QString stringQuery = QString();
    QSqlQuery query;
    bool sqlOK = false;
    int errorCode = -1;

    stringQuery = QString("SELECT id, confirmed FROM awarddxcc where dxcc='%1' AND band='%2' AND mode='%3' AND lognumber='%4'").arg(_dxcc).arg(_band).arg(_mode).arg(_logNumber);

    //stringQuery = QString("INSERT INTO awarddxcc (dxcc, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_dxcc).arg(_band).arg(_mode).arg(_iconfirmed).arg(_logNumber).arg(_qsoId);

    sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        //qDebug() << "Awards::setAwardDXCCst-1:" << endl;

        query.next();
        QSqlRecord rec = query.record();

        if (query.isValid())
        {// We have some data, we need to UPDATE - We are only confirming!
            //qDebug() << "Awards::setAwardDXCCst: We have some data, we neer to update" << endl;
            nameCol = rec.indexOf("id");
            _refid = query.value(nameCol).toString();
            stringQuery = QString("UPDATE awarddxcc SET confirmed='1', qsoid='%1' WHERE id='%2'").arg(_qsoId).arg(_refid);
            //qDebug() << "Awards::setAwardDXCCst: (UPDATE): " << stringQuery << endl;
            if (sqlOK)
            { // Set of data updated
                //qDebug() << "Awards::setAwardDXCCst: Data updated!" << endl;
                return 1;
            }
            else
            { // Something failed. Trace it!
                errorCode = query.lastError().number();
                //qDebug() << "Awards::setAwardDXCCst(UPDATE): LastQuery: " << query.lastQuery()  << endl;
                //qDebug() << "Awards::setAwardDXCCst(UPDATE): LastError-data: " << query.lastError().databaseText()  << endl;
                //qDebug() << "Awards::setAwardDXCCst(UPDATE): LastError-driver: " << query.lastError().driverText()  << endl;
                //qDebug() << "Awards::setAwardDXCCst(UPDATE): LastError-n: " << QString::number(query.lastError().number() ) << endl;
                return -1;

            }

        }
        else
        { // We don't have this set, we need to INSERT
            stringQuery = QString("INSERT INTO awarddxcc (dxcc, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_dxcc).arg(_band).arg(_mode).arg(_iconfirmed).arg(_logNumber).arg(_qsoId);
            sqlOK = query.exec(stringQuery);
            //qDebug() << "Awards::setAwardDXCCst: We don't have data... so we INSERT" << endl;
            //qDebug() << "Awards::setAwardDXCCst: (INSERT): " << stringQuery << endl;
            if (sqlOK)
            { // Set of data included

                //qDebug() << "Awards::setAwardDXCCst: Data inserted!" << endl;
            }
            else
            { // Something failed. Trace it!
                errorCode = query.lastError().number();
                //qDebug() << "Awards::setAwardDXCCst(INSERT): LastQuery: " << query.lastQuery()  << endl;
                //qDebug() << "Awards::setAwardDXCCst(INSERT): LastError-data: " << query.lastError().databaseText()  << endl;
                //qDebug() << "Awards::setAwardDXCCst(INSERT): LastError-driver: " << query.lastError().driverText()  << endl;
                //qDebug() << "Awards::setAwardDXCCst(INSERT): LastError-n: " << QString::number(query.lastError().number() ) << endl;
                return -1;

            }

        }

    }
    else
    { // Trave the error... what may be happening???
        errorCode = query.lastError().number();
        //qDebug() << "Awards::setAwardDXCCst(SELECT): LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "Awards::setAwardDXCCst(SELECT): LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "Awards::setAwardDXCCst(SELECT): LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "Awards::setAwardDXCCst(SELECT): LastError-n: " << QString::number(query.lastError().number() ) << endl;
        return -1;

    }

    return -1;

}

int Awards::setAwardWAZst(const int _cqz, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId)
{
   //qDebug() << "Awards::setAwardWAZst(CQZ/BAND/MODE/WORKED/log/qsoid): " << QString::number(_cqz) << "/" << QString::number(_band) << "/" << QString::number(_mode) << "/" << QString::number(_logNumber) << "/" << QString::number(_qsoId) << endl;

/*
    _confirmed == false QSO is just worked
    _confirmed == true QSO is confirmed

*/

   //TODO: Fix the way we check for data validity for this function
    if (!( (_cqz>=0) && (_band >=0) && (_mode>=0) && (_logNumber>=0) && (_qsoId >=0) ))
    {
        //qDebug() << "Awards::setAwardWAZst: some data was NOK" << endl;
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
        int errorCode = -1;

        stringQuery = QString("INSERT INTO awardwaz (cqz, band, mode, confirmed, lognumber, qsoid) values('%1','%2','%3','%4','%5','%6')").arg(_cqz).arg(_band).arg(_mode).arg(_iconfirmed).arg(_logNumber).arg(_qsoId);

        sqlOK = query.exec(stringQuery);

        if ((sqlOK) && (!_confirmed)) // First time a DXCC/Band/mode is worked
        {
            //qDebug() << "Awards::setAwardWAZst: _qsoId: " << QString::number(_qsoId) << "- 1"  << endl;
            return 1;
        }
        else if ((sqlOK) && (_confirmed)) // First time a CQZ/Band/Mode is confirmed
        {
            //qDebug() << "Awards::setAwardWAZst: _qsoId: " << QString::number(_qsoId) << "- 2"  << endl;
            return 2;
        }
        else
        {
            //qDebug() << "Awards::setAwardWAZst: _qsoId: " << QString::number(_qsoId) << "- sqlOK ERROR: "  << endl;
            errorCode = query.lastError().number();
            //qDebug() << "Awards::setAwardWAZst: LastQuery: " << query.lastQuery()  << endl;
            //qDebug() << "Awards::setAwardWAZst: LastError-data: " << query.lastError().databaseText()  << endl;
            //qDebug() << "Awards::setAwardWAZst: LastError-driver: " << query.lastError().driverText()  << endl;
            //qDebug() << "Awards::setAwardWAZst: LastError-n: " << QString::number(query.lastError().number() ) << endl;


        }
        return -1;
}

int Awards::setAwardDXCCConfirmed(const int _band, const int _mode, const int _dxcc, const int _newQSOid) // Changes the status of a DXCC from worked to confirmed
{
    //qDebug() << "Awards::setAwardDXCCConfirmed: " << QString::number(_band) << "/" << QString::number(_mode) << "/" << QString::number(_dxcc)<< "/" << QString::number(_newQSOid)<< endl;

/*
 * Check if the DXCC is worked
 * If worked, change the qsoid and set is confirmed
 * If not worked (or _prev=0), add the DXCC as confimed
 *
 */

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
                stringQuery = QString("UPDATE awarddxcc SET confirmed='1', qsoid='%1' WHERE qsoid='%2'").arg(_newQSOid).arg(aux);
                sqlOK = query.exec(stringQuery);
                if (sqlOK)
                {
                    return _newQSOid;
                }
                else
                { // UPDATE failed
                    errorCode = query.lastError().number();
                    //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastQuery: " << query.lastQuery()  << endl;
                    //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastError-data: " << query.lastError().databaseText()  << endl;
                    //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastError-driver: " << query.lastError().driverText()  << endl;
                    //qDebug() << "Awards::setAwardDXCCConfirmed-Update: LastError-n: " << QString::number(query.lastError().number() ) << endl;
                    return errorCode;
                }

            }
            else
            {
                // Not valid record
                //qDebug() << "Awards::setAwardDXCCConfirmed: Not valid record" << endl;
                return -3;
            }
        }
        else
        {
            // Not next record
            //qDebug() << "Awards::setAwardDXCCConfirmed: Not next record" << endl;
            return -2;
        }
    }
    else
    { //
        errorCode = query.lastError().number();
        //qDebug() << "Awards::setAwardDXCCConfirmed: LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "Awards::setAwardDXCCConfirmed: LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "Awards::setAwardDXCCConfirmed: LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "Awards::setAwardDXCCConfirmed: LastError-n: " << QString::number(query.lastError().number() ) << endl;
        return errorCode;
    }

    return 1;
}


int Awards::setDXCCToQSO(const int _dxcc, const int _qsoid) // Defines the DXCC in a QSO
{
    //qDebug() << "Awards::setDXCCToQSO: " << QString::number(_dxcc) << "/" << QString::number(_qsoid) << endl;
    int errorCode = -1;
    QString queryString = QString("UPDATE log SET dxcc='%1' WHERE id='%2'").arg(_dxcc).arg(_qsoid);
    QSqlQuery query = QSqlQuery();
    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        return 1;
    }
    else
    {
        //qDebug() << "Awards::setDXCCToQSO: DXCC Updated in Log but failed...." << endl;
        errorCode = query.lastError().number();
        //qDebug() << "Awards::setDXCCToQSO: LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "Awards::setDXCCToQSO: LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "Awards::setDXCCToQSO: LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "Awards::setDXCCToQSO: LastError-n: " << QString::number(query.lastError().number() ) << endl;
        return errorCode;
    }
}

int Awards::setCQToQSO(const int _cqz, const int _qsoid) // Defines the CQ in a QSO
{
    //qDebug() << "Awards::setCQToQSO: " << QString::number(_cqz) << "/" << QString::number(_qsoid) << endl;
    int errorCode = -1;
    QString queryString = QString("UPDATE log SET cqz='%1' WHERE id='%2'").arg(_cqz).arg(_qsoid);
    QSqlQuery query = QSqlQuery();
    bool sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        return 1;
    }
    else
    {
        //qDebug() << "Awards::setCQToQSO: DXCC Updated in Log but failed...." << endl;
        errorCode = query.lastError().number();
        //qDebug() << "Awards::setCQToQSO: LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "Awards::setCQToQSO: LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "Awards::setCQToQSO: LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "Awards::setCQToQSO: LastError-n: " << QString::number(query.lastError().number() ) << endl;
        return errorCode;
    }
}

bool Awards::getIsDXCCConfirmed(const int _dxcc, const int _logNumber)
{
  //  isDXCCConfirmed(const int _dxcc, const int _currentLog);
    return dataProxy->isDXCCConfirmed(_dxcc, _logNumber);

}

int Awards::getDXMarathonDXCC(const int _year, const int _logNumber)
{
    //qDebug() << "Awards::getDXMarathonDXCC: " << QString::number(_year) << endl;

    return dxMarathon->getDXMarathonDXCC(_year, _logNumber);
}

int Awards::getDXMarathonCQ(const int _year, const int _logNumber)
{
    //qDebug() << "Awards::getDXMarathonCQ: " << QString::number(_year) << endl;
    return dxMarathon->getDXMarathonCQ(_year, _logNumber);
}

int Awards::getDXMarathonScore(const int _year, const int _logNumber)
{
    //qDebug() << "Awards::getDXMarathonScore: " << QString::number(_year) << endl;

    return dxMarathon->getDXMarathonScore(_year, _logNumber);

}
