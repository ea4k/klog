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
#include "awards.h"

Awards::Awards(DataProxy_SQLite *dp, World *injectedWorld, const QString &_parentFunction)
{
    Q_UNUSED(_parentFunction);
  //qDebug() << "Awards::Awards- from: " << _parentFunction;
    dataProxy = dp;
    // qSqlDatabase db = QSqlDatabase::database("QSQLITE");
    //world = new World(dataProxy, Q_FUNC_INFO);
    world = injectedWorld;

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

    newOneColor   = KLOG_COLOR_NEW_ONE;
    neededColor   = KLOG_COLOR_NEEDED;
    workedColor   = KLOG_COLOR_WORKED;
    confirmedColor = KLOG_COLOR_CONFIRMED;
    defaultColor  = KLOG_COLOR_DEFAULT;

    dxccWorked.clear();
    dxccConfirmed.clear();
    wazWorked.clear();
    wazConfirmed.clear();
    manageModes = false;
  //qDebug() << "Awards::Awards - END" ;
}

Awards::~Awards()
{
    //delete(world);
    delete(dxMarathon);
}

QString Awards::getQSOofAward(const int _enti, const int _bandid, const int _modeid, const int _log, const bool _confirmed)
{// Returns the QRZ that granted that status in the DXCC
    QSqlQuery query;

    QString stringQuery = "SELECT call, qso_date FROM log "
                          "WHERE dxcc=:dxcc AND bandid=:bandid AND lognumber=:log";

    if (_modeid >= 0)
        stringQuery += " AND modeid=:modeid";

    if (_confirmed)
        stringQuery += " AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y')";

    query.prepare(stringQuery);
    query.bindValue(":dxcc",   _enti);
    query.bindValue(":bandid", _bandid);
    query.bindValue(":log",    _log);

    if (_modeid >= 0)
        query.bindValue(":modeid", _modeid);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(),
                        query.lastError().text(), query.lastQuery());
        return QString();
    }

    if (query.next() && query.isValid())
        return query.value(0).toString() + " / " + query.value(1).toString();

    return QString();
}



int Awards::getQSOIdofAward(const int _enti, const int _bandid, const int _modeid, const int _log, const bool _confirmed)
{
    QSqlQuery query;

    QString stringQuery = "SELECT id FROM log "
                          "WHERE dxcc=:dxcc AND bandid=:bandid AND lognumber=:log";

    if (_modeid >= 0)
        stringQuery += " AND modeid=:modeid";

    if (_confirmed)
        stringQuery += " AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y')";

    query.prepare(stringQuery);
    query.bindValue(":dxcc",   _enti);
    query.bindValue(":bandid", _bandid);
    query.bindValue(":log",    _log);

    if (_modeid >= 0)
        query.bindValue(":modeid", _modeid);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(),
                        query.lastError().text(), query.lastQuery());
        return -3;
    }

    if (!query.next() || !query.isValid())
        return -2;

    const int answer = query.value(0).toInt();
    return (answer >= 1) ? answer : -1;
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

int Awards::getWAZWorked(const int _logNumber, const QList<int> &modeIds)
{
       //qDebug() << "Awards::getWAZWorked (logNumber): " << QString::number(_logNumber);
    QString modeFilter;
    if (!modeIds.isEmpty())
    {
        QStringList parts;
        for (int id : modeIds) parts << QString::number(id);
        modeFilter = QString(" AND modeid IN (%1)").arg(parts.join(QLatin1Char(',')));
    }
    QSqlQuery query;
    QString stringQuery;
    if (dataProxy->doesThisLogExist(_logNumber))
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41' AND lognumber='%1'%2)").arg(_logNumber).arg(modeFilter);
    }
    else
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41'%1)").arg(modeFilter);
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

int Awards::getWAZConfirmed(const int _logNumber, const QList<int> &modeIds)
{
       //qDebug() << "Awards::getWAZConfirmed (logNumber): " << QString::number(_logNumber);

    QString modeFilter;
    if (!modeIds.isEmpty())
    {
        QStringList parts;
        for (int id : modeIds) parts << QString::number(id);
        modeFilter = QString(" AND modeid IN (%1)").arg(parts.join(QLatin1Char(',')));
    }
    QSqlQuery query;
    QString stringQuery;
    //Usar la siguiente para el confirmed
    if (dataProxy->doesThisLogExist(_logNumber))
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41' AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y') AND lognumber='%1'%2)").arg(_logNumber).arg(modeFilter);
    }
    else
    {
        stringQuery = QString("SELECT COUNT (cqz) FROM (SELECT DISTINCT cqz FROM log WHERE cqz!='' AND cqz>'0' AND cqz<'41' AND (qsl_rcvd='Y' OR lotw_qsl_rcvd='Y')%1)").arg(modeFilter);
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

QColor Awards::getEntityStatusColor(EntityStatus _entitystatus)
{

  //qDebug() << Q_FUNC_INFO << " - Start: " ;
  //qDebug() << Q_FUNC_INFO << " - Entityd: " << _entitystatus.dxcc;
  //qDebug() << Q_FUNC_INFO << " - BandId:  " << _entitystatus.bandId;
  //qDebug() << Q_FUNC_INFO << " - ModeId:  " << _entitystatus.modeId;
  //qDebug() << Q_FUNC_INFO << " - Status:  " << _entitystatus.status;
  //qDebug() << Q_FUNC_INFO << " - Log:     " << _entitystatus.logId;

    switch (_entitystatus.status)
    {
    case needed:
        return neededColor;
        break;
    case worked:
        return workedColor;
        break;
    case confirmed:
        return confirmedColor;
        break;
    default:
        return defaultColor;
        break;
    }
}



QSOStatus Awards::getQSOStatus(const int _dxcc, const int _band, const int _mode)
{
    if (_dxcc <= 0 || _band <= 0)
        return unknown;

    bool hasDxcc  = false;
    bool isWorked = false;

    for (const EntityStatus &es : std::as_const(dxccStatusList))
    {
        if (es.dxcc != _dxcc)
            continue;

        hasDxcc = true;

        if (es.bandId != _band)
            continue;

        // _mode < 0 means we are not managing modes
        const bool modeMatches = (_mode < 0) || (es.modeId == _mode);
        if (!modeMatches)
            continue;

        // Matches entity+band+mode
        if (es.status == confirmed)
            return confirmed;

        if (es.status == worked)
            isWorked = true;
    }

    if (!hasDxcc)  return ATNO;
    if (isWorked)  return worked;
    return needed;
}


QString Awards::checkIfValidIOTA(const QString &_tiota)
{  //TODO: Move it to ADIF
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
}

void Awards::setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default)
{
   //qDebug() << Q_FUNC_INFO <<  "Confirmed: " << _newOne.name(QColor::HexRgb) << " /  Needed: " << _needed.name(QColor::HexRgb)
   // << " / Worked: " << _worked.name(QColor::HexRgb) << " / Confirmed: " << _confirmed.name(QColor::HexRgb)
   // << " / Default: " << _default.name(QColor::HexRgb);

    defaultColor    = _default;
    neededColor     = _needed;
    workedColor     = _worked;
    confirmedColor  = _confirmed;
    newOneColor     = _newOne;
}

QColor Awards::getDefaultColor(){return defaultColor;}

QColor Awards::getColorFromStatus(const QSOStatus &_status)
{
    switch (_status) {
    case ATNO:      return newOneColor; break;
        case needed:    return neededColor; break;
        case worked:    return workedColor; break;
        case confirmed: return confirmedColor; break;
        default:        return defaultColor; break;
    }
}

void Awards::recalculateAwards()
{
    //qDebug() << Q_FUNC_INFO;
    if (updateDXCCStatus())
        emit awardDXCCUpdated();
    //dataProxy->updateAwardWAZ();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void Awards::setAwards()
{
    //qDebug() << Q_FUNC_INFO << " - " << QString::number(_qsoId);
    recalculateAwards();
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
    //qDebug() << Q_FUNC_INFO << " - " << _dxcc;

    //for (const auto& indexEntityStatus : dxccStatusList)
    for (const auto& indexEntityStatus : std::as_const(dxccStatusList))
    {
        if (indexEntityStatus.dxcc == _dxcc &&
            indexEntityStatus.status == QSOStatus::confirmed &&
            (_logNumber <= 0 || indexEntityStatus.logId == _logNumber))
        {
            return true;
        }
    }
    return false;
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

void Awards::setManageModes(const bool _manageModes)
{
    manageModes = _manageModes;
}

bool Awards::updateDXCCStatus(const int _logNumber)
{
   //qDebug() << Q_FUNC_INFO << ": " << QString::number(_logNumber);
    QSqlQuery query;
    QString stringQuery = QString();

    if (_logNumber>=0)
    {
        stringQuery = QString("SELECT dxcc, bandid, modeid, qsl_rcvd, lotw_qsl_rcvd, lognumber, MIN(id) as id FROM log WHERE lognumber = :lognumber GROUP BY dxcc, bandid, modeid, qsl_rcvd, lotw_qsl_rcvd ORDER BY dxcc;");
    }
    else
    {
        stringQuery = QString("SELECT dxcc, bandid, modeid, qsl_rcvd, lotw_qsl_rcvd, lognumber, MIN(id) as id FROM log GROUP BY dxcc, bandid, modeid, qsl_rcvd, lotw_qsl_rcvd ORDER BY dxcc;");
    }
    if (_logNumber>=0)
    {
        if (!query.prepare(stringQuery))
            return false;
        query.bindValue(":lognumber", _logNumber);
        if (!query.exec())
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            query.finish();
            return false;
        }
    }
    else if (!executeQuery(query, stringQuery))
    {
        return false;
    }

    int qsos = processQueryResults(query);
   //qDebug() << Q_FUNC_INFO << "- DXCC :" << qsos;
    populateDXCCStatusMap(); // Populate the multi-hash map

    query.finish();

    return !(dxccStatusList.isEmpty() && (qsos > 0));
}

void Awards::populateDXCCStatusMap()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    dxccStatusMap.clear(); // Clear the multi-hash map

    for (const EntityStatus &status : std::as_const(dxccStatusList))
    {
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
    }
    else
    {
        return QSOStatus::worked;
    }
}

int Awards::processQueryResults(QSqlQuery &query)
{
   //qDebug() << Q_FUNC_INFO << " - Start ";
    int qsos = 0;
    dxccStatusList.clear();

    while (query.next()) {
        if (!query.isValid()) {
            emit queryError(Q_FUNC_INFO, "Invalid query result", "", query.lastQuery());
            return qsos;
        }

        qsos++;
        EntityStatus ent = extractEntityStatus(query);

        if (updateOrAddEntityStatus(ent)) {
            dxccStatusList.append(ent);
        }
        //printEntityStatus(Q_FUNC_INFO, ent);
    }
    return qsos;
}

EntityStatus Awards::extractEntityStatus(QSqlQuery &query) {
    QSqlRecord rec = query.record();

    EntityStatus ent;
    ent.dxcc = query.value(rec.indexOf("dxcc")).toInt();
    ent.bandId = query.value(rec.indexOf("bandid")).toInt();
    ent.modeId = query.value(rec.indexOf("modeid")).toInt();
    ent.status = getStatus(query, rec);
    ent.qsoId = query.value(rec.indexOf("id")).toInt();
    ent.logId = query.value(rec.indexOf("lognumber")).toInt();

    return ent;
}

bool Awards::updateOrAddEntityStatus(const EntityStatus &ent) {


    for (qsizetype i = 0; i < dxccStatusList.count(); ++i) {
        if (dxccStatusList[i].dxcc == ent.dxcc &&
            dxccStatusList[i].bandId == ent.bandId &&
            dxccStatusList[i].modeId == ent.modeId) {
            if (ent.status == confirmed && dxccStatusList[i].status != confirmed) {
                dxccStatusList[i].status = ent.status;
                dxccStatusList[i].qsoId = ent.qsoId;
                dxccStatusList[i].logId = ent.logId;
                return false; // Entity was updated
            }
            return false; // Entity was not added or updated
        }
    }
    return true; // Entity is new and will be added
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

QString Awards::status2String(const QSOStatus &_status, bool shortString)
{
    switch (_status)
    {
    case QSOStatus::ATNO:
        if (shortString)
            return tr("A", "Do not translate");
        return tr("ATNO", "All Time New One, Do not translate");
    case QSOStatus::needed:
        if (shortString)
            return tr("N", "Needed, please try to keep the N");
        return tr("Needed");
    case QSOStatus::worked:
        if (shortString)
            return tr("W", "Worked, please try to keep the W");
        return tr("Worked");
    case QSOStatus::confirmed:
        if (shortString)
            return tr("C", "Confirmed, please try to keep the C");
        return tr("Confirmed");
    default:
        if (shortString)
            if (shortString)
                return tr("U", "Unknown");
        return tr("Unknown");
    }   
}

QString Awards::status2Message(const QSOStatus &_status)
{

    //enum QSOStatus {unknown, ATNO, needed, worked, confirmed};
    QString msg;

    switch (_status) {
    case ATNO:
       //qDebug() << Q_FUNC_INFO << " - ATNO";
           msg = tr("New One, work it!");
        break;
    case needed:
       //qDebug() << Q_FUNC_INFO << " - Needed";
        if (manageModes)
            msg = tr("Needed (band/mode), work it!");
        else
            msg = tr("Needed, work it!");
        break;
    case worked:
       //qDebug() << Q_FUNC_INFO << " - Worked";
        if (manageModes)
            msg = tr("Worked (band/mode) but not confirmed");
        else
            msg = tr("Worked but not confirmed");
        break;
    case confirmed:
       //qDebug() << Q_FUNC_INFO << " - Confirmed";
        return QObject::tr("Confirmed");
       if (manageModes)
           msg = tr("Confirmed (band/mode)");
       else
           msg = tr("Confirmed");
        break;
    case unknown:
        //qDebug() << Q_FUNC_INFO << " - Unknown";
        msg = tr("Unknown!");
        break;
    //break;
    default:
        msg = tr("Not identified");
        break;
    }
    return msg;
}


void Awards::printEntityStatus(const QString &_callingFunction, const EntityStatus &ent)
{   // This is a debug function
   //Q_UNUSED(_callingFunction);
   // Q_UNUSED(ent);

 //qDebug() << Q_FUNC_INFO << " ------------------------------------------------------------------------------------";
 //qDebug() << Q_FUNC_INFO << " - " <<  _callingFunction << " - DXCC   : " << ent.dxcc;
 //qDebug() << Q_FUNC_INFO << " - " <<  _callingFunction << " - Band   : " << ent.bandId;
 //qDebug() << Q_FUNC_INFO << " - " <<  _callingFunction << " - Mode   : " << ent.modeId;
 //qDebug() << Q_FUNC_INFO << " - " <<  _callingFunction << " - QSOid  : " << ent.qsoId;
 //qDebug() << Q_FUNC_INFO << " - " <<  _callingFunction << " - LogId  : " << ent.logId;
 //qDebug() << Q_FUNC_INFO << " - " <<  _callingFunction << " - Status : " << status2String(ent.status);
//qDebug() << Q_FUNC_INFO << " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
}

void Awards::printColors(const QString &_callingFunction)
{
   //qDebug() << Q_FUNC_INFO << " - " << _callingFunction;
   //qDebug() << Q_FUNC_INFO << " - newOneColor:    " << newOneColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - neededColor:    " << neededColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - workedColor:    " << workedColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - confirmedColor: " << confirmedColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - defaultColor:   " << defaultColor.name(QColor::HexRgb);
}
