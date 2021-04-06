/***************************************************************************
                          qso.cpp  -  description
                             -------------------
    begin                : may 2020
    copyright            : (C) 2020 by Jaime Robles
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
#include "qso.h"

QSO::QSO()
{
    clear();
}

QSO::~QSO()
{
    clear();
}
void QSO::clear()
{
    qsoId = -1;
    logId = -1;
    satName.clear();
    callsign.clear();
    stationCallsign.clear();
    band = QString();
    mode = QString();
    lotw_qsl_sent = QString();
    lotw_qsl_rcvd = QString();
    gridsquare = QString();
    freq = -1.0;
    freq_rx = -1.0;
    dxcc = -1;
    propMode.clear();
    qsl_rcvd = QChar();
    QSLRDate = QDate();
    QSLLoTWRDate = QDate();
    QSLLoTWSDate = QDate();
    qso_dateTime = QDateTime();
    lotwUpdating = false;
}

bool QSO::setQSOid(const int _i)
{
    if (_i >0)
    {
        qsoId = _i;
        return true;
    }
    else
    {
        return false;
    }
}
int QSO::getQSOid()
{
    return qsoId;
}

bool QSO::setLogId(const int _i)
{
    if (_i >0)
    {
        logId = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getLogId()
{
    return logId;
}

bool QSO::isValid()
{
   //qDebug() << "QSO::isValid: " << callsign << endl;
    if ( (callsign.length()>0))
    {
       //qDebug() << "QSO::isValid: TRUE" << endl;
        return true;
    }
    else
    {
       //qDebug() << "QSO::isValid: FALSE" << endl;
        return false;
    }

}

bool QSO::setCall(const QString &_c)
{
   //qDebug() << "QSO::setCall: " << _c << endl;
    if (util->isValidCall(_c))
    {
       //qDebug() << "QSO::setCall: Valid Call"  << endl;
        callsign = _c;
        return true;
    }
    else {
       //qDebug() << "QSO::setCall: NOT valid Call"  << endl;
        return false;
    }
}

QString QSO::getCall()
{
    return callsign;
}

bool QSO::setDXCC(const int _i)
{
    if (util->isValidDXCC(_i))
    {
        dxcc = _i;
        return true;
    }
    return false;

}
int QSO::getDXCC()
{
    return dxcc;
}

bool QSO::setStationCallsign(const QString &_c)
{
    if (util->isValidCall(_c))
    {
       stationCallsign = _c;
        return true;
    }
    else {
        return false;
    }
}

QString QSO::getStationCallsign()
{
    return stationCallsign;
}

bool QSO::setBand(const QString &_c)
{
   //qDebug() << "QSO::setBand: "<< _c << endl;
    if (_c.length()>0)
    {
        band = _c;
        return true;
    }
    else
    {
        band = QString();
        return false;
    }
}

QString QSO::getBand()
{
    return band;
}

bool QSO::setBandRX(const QString &_c)
{
   //qDebug() << "QSO::setBandRX: "<< _c << endl;
    if (_c.length()>0)
    {
        band = _c;
        return true;
    }
    else
    {
        band = QString();
        return false;
    }
}

QString QSO::getBandRX()
{
    return band;
}

bool QSO::setMode(const QString &_c)
{
    //int i = dataProxy->getIdFromModeName(_c);
    if (_c.length()>0)
    {
        mode = _c;
        return true;
    }
    else
    {
        mode = QString();
        return false;
    }
}

QString QSO::getMode()
{
    return mode;
}

bool QSO::setDate(const QDate &_c)
{
   //qDebug() << "QSO::setDate: " << util->getDateSQLiteStringFromDate(_c) << endl;
    if (_c.isValid())
    {
       //qDebug() << "QSO::setDate: VALID" << endl;
        qso_dateTime.setDate(_c);
        return true;
    }
    else
    {
       //qDebug() << "QSO::setDate: NOT VALID" << endl;
        qso_dateTime.setDate(QDate());
        return false;
    }
}

QDate QSO::getDate()
{
    return qso_dateTime.date();
}

bool QSO::setTimeOn(const QTime &_c)
{
   //qDebug() << "QSO::setQSLTime: " << _c.toString("mmhhss") << endl;
    if (_c.isValid())
    {
       //qDebug() << "QSO::setQSLTime: VALID" << endl;
        qso_dateTime.setTime(_c);
        return true;
    }
    else
    {
       //qDebug() << "QSO::setQSLTime: NOT VALID" << endl;
        qso_dateTime.setTime(QTime());
        return false;
    }
}

QTime QSO::getTimeOn()
{
    return qso_dateTime.time();
}

QDateTime QSO::getDateTimeOn()
{
    return qso_dateTime;
}

bool QSO::setDateTime(const QDateTime &_c)
{
    if (_c.isValid())
    {
        qso_dateTime = _c;
        return true;
    }
    else
    {
        return false;
    }
}

bool QSO::setFreq(const float _f)
{
    if (_f>0)
    {
        freq = _f;
        return true;
    }
    else {
        return false;
    }
}

bool QSO::setFreqRX(const float _f)
{
    if (_f>0)
    {
        freq_rx = _f;
        return true;
    }
    else {
        return false;
    }
}
float QSO::getFreq()
{
    return freq;
}
float QSO::getFreqRX()
{
    return freq_rx;
}

bool QSO::setQSL_RCVD(const QString &_c)
{
    if (util->isValidQSL_Rcvd(_c))
    {
        qsl_rcvd = _c;
        return true;
    }
    else {
        return false;
    }
}

QString QSO::getQSL_RCVD()
{
    return qsl_rcvd;
}

bool QSO::setLoTWQSL_RCVD(const QString &_c)
{
    //qDebug() << "QSO::setLoTWQSL_RCVD: " << _c  << endl;
    if (util->isValidQSL_Rcvd(_c))
    {
        //qDebug() << "QSO::setLoTWQSL_RCVD: VALID " << endl;
        lotw_qsl_rcvd = _c;
        return true;
    }
    else
    {
        //qDebug() << "QSO::setLoTWQSL_RCVD: NOT valid " << endl;
        return false;
    }
}
QString QSO::getLoTWQSL_RCVD()
{
    return lotw_qsl_rcvd;
}
bool QSO::setLoTWQSL_SENT(const QString &_c)
{
    if (util->isValidQSL_Sent(_c))
    {
        lotw_qsl_sent = _c;
        return true;
    }
    else {
        return false;
    }
}
QString QSO::getLoTWQSL_SENT()

{
    return lotw_qsl_sent;
}

bool QSO::setLoTWQSLRDate(const QDate &_c)
{
    //qDebug() << "QSO::setLoTWQSLRDate: " << _c.toString("yyyy-MM-dd") << endl;
    if (_c.isValid())
    {
        //qDebug() << "QSO::setLoTWQSLRDate: TRUE" << endl;
        QSLLoTWRDate = _c;
        return true;
    }
    else
    {
        //qDebug() << "QSO::setLoTWQSLRDate: FALSE" << endl;
        QSLLoTWRDate = QDate();
        return false;
    }
}

QDate QSO::getLoTWQSLRDate()
{
    return QSLLoTWRDate;
}

bool QSO::setLoTWQSLSDate(const QDate &_c)
{
    if (_c.isValid())
    {
        QSLLoTWSDate = _c;
        return true;
    }
    else
    {
        QSLLoTWSDate = QDate();
        return false;
    }
}

QDate QSO::getLoTWQSLSDate()
{
    return QSLRDate;
}

bool QSO::setQSLRDate(const QDate &_c)
{
    if (_c.isValid())
    {
        QSLRDate = _c;
        return true;
    }
    else
    {
        QSLRDate = QDate();
        return false;
    }
}

QDate QSO::getQSLRDate()
{
    return QSLRDate;
}

bool QSO::setSatName(const QString &_c)
{
    if (_c.length()>0)
    {
        satName = _c;
        return true;
    }
    else
    {
        satName = QString();
        return false;
    }
}

QString QSO::getSatName()
{
    return satName;
}

bool QSO::setPropMode(const QString &_c)
{
    if (_c.length()>0)
    {
        propMode = _c;
        return true;
    }
    else
    {
        propMode = QString();
        return false;
    }
}

QString QSO::getPropMode()
{
    return propMode;
}
void QSO::setLoTWUpdating(bool _lotw)
{
    lotwUpdating = _lotw;
}

bool QSO::setGridSquare(const QString &_c)
{
    if (util->isValidGrid(_c))
    {
        gridsquare = _c;
        return false;
    }
    else
    {
        return false;
    }
}

QString QSO::getGridSquare()
{
    return gridsquare;
}

bool QSO::setData(const QString &_adifPair)
{
    //qDebug() << "QSO::setData: " << _adifPair << endl;
    QStringList d;
    d.clear();
    d << util->getValidADIFFieldAndData(_adifPair);
    if (d.length()!=2)
    {
        //qDebug() << "QSO::setData: NOT VALID ADIF: " << _adifPair << endl;
        return false;
    }
   //qDebug() << "QSO::setData: " << d.at(0) << "/" << d.at(1) << endl;

    QString field = d.at(0).toUpper();
    QString data = d.at(1);

    if (field == "CALL")
    {
        setCall(data);
    }
    else if (field == "BAND")
    {
        setBand(data);
    }
    else if (field == "MODE")
    {
        setMode(data);
    }
    else if (field == "TIME_ON")
    {
        setTimeOn(util->getTimeFromADIFTimeString(data));
    }
    else if (field == "QSO_DATE")
    {
       //qDebug() << "QSO::setData: QSO_DATE: " << data  << endl;
        setDate(util->getDateFromADIFDateString(data));
    }
    else if (field == "FREQ")
    {
        setFreq(data.toFloat());
    }
    else if (field == "FREQ_RX")
    {
        setFreqRX(data.toFloat());
    }
    else if (field == "QSLRDATE")
    {
        setQSLRDate(util->getDateFromADIFDateString(data));
    }
    else if (field == "QSL_RCVD")
    {
        setQSL_RCVD(data);
    }
    else if (field == "SAT_NAME")
    {
        setSatName(data);
    }
    else if (field == "PROP_MODE")
    {
        setPropMode(data);
    }
    else if (field == "LOTW_QSL_RCVD")
    {
       setLoTWQSL_RCVD(data);
    }
    else if (field == "LOTW_QSL_SENT")
    {
       setLoTWQSL_SENT(data);
    }
    else if (field == "STATION_CALLSIGN")
    {
       setStationCallsign(data);
    }
    else if (field == "GRIDSQUARE")
    {
        setGridSquare(data);
    }
    else if (field == "APP_LOTW_RXQSL")
    {
        //qDebug() << "QSO::setData: APP_LOTW_RXQSL: " << data  << endl;
        setLoTWQSL_RCVD("Y");
        setLoTWQSLRDate(util->getDateFromLoTWQSLDateString(data));
    }

    return true;
}
