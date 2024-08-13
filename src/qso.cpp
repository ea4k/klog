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
    logLevel = None;
    qsoId = -1;
    util = new Utilities(Q_FUNC_INFO);
    util->setCallValidation(false);
    //db = new DataBase(Q_FUNC_INFO, "1", util->getKLogDBFile());
    //db = new DataBase(Q_FUNC_INFO, klogVersion, util->getKLogDBFile());
}

QSO::QSO(const QSO& other)
{
    util = new Utilities(Q_FUNC_INFO);
    util->setCallValidation(false);
    logLevel = other.logLevel;
    haveBand = other.haveBand;
    haveMode = other.haveMode;
    haveSubMode = other.haveSubMode;
    haveDateTime = other.haveDateTime;
    haveCall = other.haveCall;

    qsoId = other.qsoId;
    logId = other.logId;
    backup = other.backup;
    stationCallsign = other.stationCallsign;
    lotwUpdating = other.lotwUpdating;
    realTime = other.realTime;
    manualMode = other.manualMode;
    keepComment = other.keepComment;
    keepMyData = other.keepMyData;
    keepOther = other.keepOther;
    keepSat = other.keepSat;
    modifying = other.modifying;

    // VARIABLES for ADIF //////////
    address = other.address;
    age = other.age;
    altitude = other.altitude;
    a_index = other.a_index;
    ant_az = other.ant_az;
    ant_el = other.ant_el;
    ant_path = other.ant_path;
    arrl_sect = other.arrl_sect;
    award_submitted = other.award_submitted;
    award_granted = other.award_granted;
    band = other.band;
    band_rx = other.band_rx;
    callsign = other.callsign;
    check = other.check;
    clase = other.clase;
    clublogQSOUpdateDate = other.clublogQSOUpdateDate;
    clublog_status = other.clublog_status;
    county = other.county;
    comment = other.comment;
    continent = other.continent;
    contacted_op = other.contacted_op;
    contest_id = other.contest_id;
    country = other.country;
    cqz = other.cqz;
    credit_granted = other.credit_granted;
    credit_submitted = other.credit_submitted;
    darc_dok = other.darc_dok;
    distance = other.distance;
    dxcc = other.dxcc;
    email = other.email;
    ownerCall = other.ownerCall;
    contacted_owner = other.contacted_owner;
    eQSLRDate = other.eQSLRDate;
    eQSLSDate = other.eQSLSDate;
    eqsl_qsl_rcvd = other.eqsl_qsl_rcvd;
    eqsl_qsl_sent = other.eqsl_qsl_sent;
    fists = other.fists;
    fists_cc = other.fists_cc;
    forceInit = other.forceInit;
    freq_tx = other.freq_tx;
    freq_rx = other.freq_rx;
    gridsquare = other.gridsquare;
    gridsquare_ext = other.gridsquare_ext;
    operatorCall = other.operatorCall;
    hrdlogUploadDate = other.hrdlogUploadDate;
    hrdlog_status = other.hrdlog_status;
    hamlogeu_status = other.hamlogeu_status;
    hamlogeuUpdateDate = other.hamlogeuUpdateDate;
    hamqth_status = other.hamqth_status;
    hamqthUpdateDate = other.hamqthUpdateDate;
    iota = other.iota;
    iota_ID = other.iota_ID;
    itu_zone = other.itu_zone;
    k_index = other.k_index;
    latitude = other.latitude;
    longitude = other.longitude;
    QSLLoTWRDate = other.QSLLoTWRDate;
    QSLLoTWSDate = other.QSLLoTWSDate;
    lotw_qsl_rcvd = other.lotw_qsl_rcvd;
    lotw_qsl_sent = other.lotw_qsl_sent;
    max_bursts = other.max_bursts;
    mode = other.mode;
    ms_shower = other.ms_shower;
    my_altitude = other.my_altitude;
    my_antenna = other.my_antenna;
    my_arrl_sect = other.my_arrl_sect;
    my_city = other.my_city;
    my_county = other.my_county;
    my_country = other.my_country;
    my_cqz = other.my_cqz;
    my_dxcc = other.my_dxcc;
    my_fists = other.my_fists;
    my_gridsquare = other.my_gridsquare;
    my_gridsquare_ext = other.my_gridsquare_ext;
    my_iota = other.my_iota;
    my_iota_ID = other.my_iota_ID;
    my_itu_zone = other.my_itu_zone;
    my_latitude = other.my_latitude;
    my_longitude = other.my_longitude;
    my_name = other.my_name;
    my_pota_ref = other.my_pota_ref;
    my_postal_code = other.my_postal_code;
    my_rig = other.my_rig;
    my_sig = other.my_sig;
    my_sig_info = other.my_sig_info;
    my_sota_ref = other.my_sota_ref;
    my_state = other.my_state;
    my_street = other.my_street;
    my_usaca_counties = other.my_usaca_counties;
    my_vucc_grids= other.my_vucc_grids;
    my_wwff_ref = other.my_wwff_ref;
    name = other.name;
    notes = other.notes;
    nr_bursts = other.nr_bursts;
    nr_pings = other.nr_pings;
    operatorCall = other.operatorCall;
    ownerCall = other.ownerCall;
    contacted_owner = other.contacted_owner;
    prefix = other.prefix;
    pota_ref = other.pota_ref;
    precedence = other.precedence;
    propMode = other.propMode;
    public_key = other.public_key;
    QRZComDate = other.QRZComDate;
    QRZCom_status = other.QRZCom_status;
    qslmsg = other.qslmsg;
    QSLRDate = other.QSLRDate;
    QSLSDate = other.QSLSDate;
    qsl_rcvd = other.qsl_rcvd;
    qsl_sent = other.qsl_sent;
    qslSenVia = other.qslSenVia;
    qslRecVia = other.qslRecVia;
    qslVia = other.qslVia;
    qso_complete = other.qso_complete;
    qso_dateTime = other.qso_dateTime;
    qso_date_off = other.qso_date_off;
    qso_random = other.qso_random;
    qth = other.qth;
    region = other.region;
    rig = other.rig;
    RST_rx = other.RST_rx;
    RST_tx = other.RST_tx;
    pwr_rx = other.pwr_rx;
    satMode = other.satMode;
    satName = other.satName;
    sfi =other.sfi;
    sig = other.sig;
    sig_info = other.sig_info;
    silent_key = other.silent_key;
    skcc = other.skcc;
    sota_ref = other.sota_ref;
    srx = other.srx;
    srx_string = other.srx_string;
    state = other.state;
    stx = other.stx;
    stx_string = other.stx_string;
    submode = other.submode;
    swl = other.swl;
    ten_ten = other.ten_ten;
    qso_time_off = other.qso_time_off;
    pwr_tx = other.pwr_tx;
    uksmg = other.uksmg;
    usaca_counties = other.usaca_counties;
    ve_prov = other.ve_prov;
    vucc_grids = other.vucc_grids;
    web = other.web;
    wwff_ref = other.wwff_ref;
}

QSO::~QSO()
{
    delete(util);
}

void QSO::setLogLevel (const DebugLogLevel _b)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    logLevel = _b;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void QSO::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    if (logLevel<=_level)
        emit debugLog (_func, _msg, _level);
}

bool QSO::isComplete()
{
    return (haveBand && (haveMode || haveSubMode) && haveDateTime && haveCall);
}

void QSO::clear()
{   // When we clear a QSO, we put data that is not valid for a QSO, if possible.
    // so no data that has not been saved by the user is "populated automatically" by KLog
    // without the user's knowledge or intention

    logEvent (Q_FUNC_INFO, "Start", Debug);
    haveBand = false;
    haveMode = false;
    haveSubMode = false;
    haveDateTime = false;
    haveCall = false;

    qsoId = -1;
    logId = -1;
    backup = false;
    stationCallsign = QString();
    lotwUpdating = false;
    realTime = false;
    manualMode = false;
    keepComment = false;
    keepMyData = false;
    keepOther = false;
    keepSat = false;
    modifying = false;

    // VARIABLES for ADIF //////////
    address = QString();
    age = 0;
    altitude = 0.0;
    a_index = 0;
    ant_az = 0.0;
    ant_el = 0.0;
    ant_path = QString();
    arrl_sect = QString();
    award_submitted = QString();
    award_granted = QString();
    band = QString();
    band_rx = QString();
    callsign = QString();
    check = QString();
    clase = QString();
    clublogQSOUpdateDate = QDate();
    clublog_status = QString();
    county = QString();
    comment = QString();
    continent = QString();
    contacted_op = QString();
    contest_id = QString();
    country = QString();
    cqz = 0;
    credit_granted = QString();
    credit_submitted = QString();
    darc_dok = QString();
    distance = 0;
    dxcc = 0;
    email = QString();
    ownerCall = QString();
    contacted_owner = QString();
    eQSLRDate = QDate();
    eQSLSDate = QDate();
    eqsl_qsl_rcvd = QString();
    eqsl_qsl_sent = QString();
    fists = 0;
    fists_cc = 0;
    forceInit = false;
    freq_tx = 0;
    freq_rx = 0;
    gridsquare = QString();
    gridsquare_ext = QString();
    operatorCall = QString();
    hrdlogUploadDate = QDate();
    hrdlog_status = QString();
    hamlogeu_status = QString();
    hamlogeuUpdateDate = QDate();
    hamqth_status = QString();
    hamqthUpdateDate = QDate();
    iota = QString();
    iota_ID = 0;
    itu_zone = 0;
    k_index = 0;
    latitude = QString();
    longitude = QString();
    QSLLoTWRDate = QDate();
    QSLLoTWSDate = QDate();
    lotw_qsl_rcvd = QString();
    lotw_qsl_sent = QString();
    max_bursts = 0;
    mode = QString();
    ms_shower = QString();
    my_altitude = 0.0;
    my_antenna = QString();
    my_arrl_sect = QString();
    my_city = QString();
    my_county = QString();
    my_country = QString();
    my_cqz = 0;
    my_dxcc = 0;
    my_fists = 0;
    my_gridsquare = QString();
    my_gridsquare_ext = QString();
    my_iota = QString();
    my_iota_ID = 0;
    my_itu_zone = 0;
    my_latitude = QString();
    my_longitude = QString();
    my_name = QString();
    my_pota_ref = QString();
    my_postal_code = QString();
    my_rig = QString();
    my_sig = QString();
    my_sig_info = QString();
    my_sota_ref = QString();
    my_state = QString();
    my_street = QString();
    my_usaca_counties = QString();
    my_vucc_grids= QString();
    my_wwff_ref = QString();
    name = QString();
    notes = QString();
    nr_bursts = 0;
    nr_pings = 0;
    operatorCall = QString();
    ownerCall = QString();
    contacted_owner = QString();
    prefix = QString();
    pota_ref = QString();
    precedence = QString();
    propMode = QString();
    public_key = QString();
    QRZComDate = QDate();
    QRZCom_status = QString();
    qslmsg = QString();
    QSLRDate = QDate();
    QSLSDate = QDate();
    qsl_rcvd = QString();
    qsl_sent = QString();
    qslSenVia = QString();
    qslRecVia = QString();
    qslVia = QString();
    qso_complete = qso_complete = util->getQSO_CompleteFromADIF("Y");
    qso_dateTime = QDateTime();
    qso_date_off = QDate();
    qso_random = true;
    qth = QString();
    region = QString();
    rig = QString();
    RST_rx = QString();
    RST_tx = QString();
    pwr_rx = 0.0;
    satMode = QString();
    satName = QString();
    sfi = 0;
    sig = QString();
    sig_info = QString();
    silent_key = false;
    skcc = QString();
    sota_ref = QString();
    srx = 0;
    srx_string = QString();
    state = QString();
    stx = 0;
    stx_string = QString();
    submode = QString();
    swl = false;
    ten_ten = 0;
    qso_time_off = QTime();
    pwr_tx = 0.0;
    uksmg = 0;
    usaca_counties = QString();
    ve_prov = QString();
    vucc_grids = QString();
    web = QString();
    wwff_ref = QString();
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void QSO::setBackup(const bool _rt)
{
    backup    = _rt;
}

bool QSO::getBackup()
{
    return backup;
}

void QSO::setModifying(const bool _mod)
{
    modifying = _mod;
}

bool QSO::getModifying()
{
    return modifying;
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

bool QSO::setFreq(const double _f)
{
   //qDebug() << Q_FUNC_INFO << ": " << QString::number(_f);
    if (_f>0)
    {
        freq_tx = _f;
       //qDebug() << Q_FUNC_INFO << ":-2 " << QString::number(freq_tx);
        setBandFromFreq(freq_tx);
        if (freq_rx<=0)
            setFreqRX(freq_tx);
        return true;
    }
    else {
        return false;
    }
}

bool QSO::setFreqRX(const double _f)
{
    if (_f>0)
    {
        freq_rx = _f;
        setBandFromFreq(_f, false);
        return true;
    }
    else {
        return false;
    }
}

double QSO::getFreqTX()
{
    return freq_tx;
}

double QSO::getFreqRX()
{
    return freq_rx;
}

bool QSO::isValid()
{// Add more controls: Call, Date, Time, Band, Mode?
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ( (callsign.length()>0))
    {
       logEvent (Q_FUNC_INFO, "END-true", Debug);
        return true;
    }
    else
    {
       logEvent (Q_FUNC_INFO, "END-false", Debug);
        return false;
    }
}

bool QSO::setCall(const QString &_c)
{
    logEvent (Q_FUNC_INFO, QString("Start: %1").arg(_c), Debug);
    QString aux;
    aux = _c;
    if (aux.isNull())
    {
        logEvent(Q_FUNC_INFO, "END - False-1", Debug);
        return false;
    }
    if (util->isValidCall(aux))
    {
        logEvent (Q_FUNC_INFO, QString("END - true"), Debug);
        callsign = aux;
        haveCall = true;
        return true;
    }
    else
    {
        logEvent (Q_FUNC_INFO, QString("END - false-2"), Debug);
        return false;
    }
}

QString QSO::getCall()
{
    //qDebug() << Q_FUNC_INFO << ": " << callsign;
    return callsign;
}

void QSO::setBandFromFreq(const double _fr, bool TX)
{
    if (TX)
    {
        setBand (getBandNameFromFreq (_fr));
    }
    else
    {
       setBandRX (getBandNameFromFreq (_fr));
    }
}

bool QSO::setBand(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (_c.length()>0)
    {
        band = _c;
        haveBand = true;
        if (band_rx.isNull())
            band_rx = band;
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
   //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (_c.length()>0)
    {
        band_rx = _c;
        return true;
    }
    else
    {
        band_rx = QString();
        return false;
    }
}

QString QSO::getBandRX()
{
    return band_rx;
}

bool QSO::setMode(const QString &_c)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QString aux = _c;
    if (aux.isNull())
    {
        mode = QString();
        logEvent (Q_FUNC_INFO, "END - False 1", Debug);
        return false;
    }
    if (aux.length()>0)
    {
        mode = aux;
        logEvent (Q_FUNC_INFO, "END - True", Debug);
        haveMode = true;
        return true;
    }
    else
    {
        mode = QString();
        logEvent (Q_FUNC_INFO, "END - False 2", Debug);
        return false;
    }
}

QString QSO::getMode()
{
    return mode;
}

bool QSO::setDate(const QDate &_c)
{
    if (_c.isValid())
    {
        qso_dateTime.setDate(_c);
        if(qso_dateTime.time().isValid ())
            haveDateTime = true;
        return true;
    }
    else
    {
        return false;
    }
}

QDate QSO::getDate()
{
    return qso_dateTime.date();
}

bool QSO::setDateOff(const QDate &_c)
{
    if (_c.isValid())
    {
        qso_date_off = _c;
        return true;
    }
    else
    {
        qso_date_off = QDate();
        return false;
    }
}

QDate QSO::getDateOff()
{
    return qso_date_off;
}

bool QSO::setTimeOff(const QTime &_c)
{
    if (_c.isValid())
    {
        qso_time_off = _c;
        return true;
    }
    else
    {
        qso_time_off = QTime();
        return false;
    }
}

QTime QSO::getTimeOff()
{
    return qso_time_off;
}

bool QSO::setTimeOn(const QTime &_c)
{
   //qDebug() << Q_FUNC_INFO << ": " << _c.toString("mmhhss");
    if (_c.isValid())
    {
       //qDebug() << Q_FUNC_INFO << ": VALID";
        qso_dateTime.setTime(_c);
        if (qso_dateTime.date().isValid ())
            haveDateTime = true;
        return true;
    }
    else
    {
        return false;
    }
}

QTime QSO::getTimeOn()
{
    return qso_dateTime.time();
}

bool QSO::setDateTimeOn(const QDateTime &_c)
{
    if (_c.isValid())
    {
        qso_dateTime = _c;
        haveDateTime = true;
        return true;
    }
    else
    {
        return false;
    }
}

QDateTime QSO::getDateTimeOn()
{
    return qso_dateTime;
}

void QSO::setRealTime(const bool _rt)
{
    realTime = _rt;
}

bool QSO::getRealTime()
{
    return realTime;
}

void QSO::setManualMode(const bool _rt)
{
    manualMode = _rt;
}

bool QSO::getManualMode()
{
    return manualMode;
}

// eQSL Tab

bool QSO::setClubLogStatus(const QString &_c)
{
    if (util->isValidUpload_Status (_c))
    {
        clublog_status = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getClubLogStatus()
{
    return clublog_status;
}

bool QSO::setClubLogDate(const QDate &_c)
{
    if (_c.isValid())
    {
        clublogQSOUpdateDate = _c;
        return true;
    }
    else
    {
        clublogQSOUpdateDate  = QDate();
        return false;
    }
}

QDate QSO::getClubLogDate()
{
    return clublogQSOUpdateDate;
}

bool QSO::setQRZCOMStatus(const QString &_c)
{
    {
        if (util->isValidUpload_Status (_c))
        {
            QRZCom_status = _c;
            return true;
        }
        else
        {
            return false;
        }
    }
}

QString QSO::getQRZCOMStatus()
{
    return QRZCom_status;
}

bool QSO::setQRZCOMDate(const QDate &_c)
{
    if (_c.isValid())
    {
        QRZComDate = _c;
        return true;
    }
    else
    {
        QRZComDate = QDate();
        return false;
    }
}

QDate QSO::getQRZCOMDate()
{
    return QRZComDate;
}

bool QSO::setEQSLQSL_RCVD(const QString &_c)
{
    if (util->isValidQSL_Rcvd(_c))
    {
        eqsl_qsl_rcvd = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getEQSLQSL_RCVD()
{
    return eqsl_qsl_rcvd;
}

bool QSO::setEQSLQSL_SENT(const QString &_c)
{
    if (util->isValidQSL_Sent(_c))
    {
        eqsl_qsl_sent = _c;
        return true;
    }
    else {
        return false;
    }
}

QString QSO::getEQSLQSL_SENT()
{
    return eqsl_qsl_sent;
}

bool QSO::setEQSLQSLRDate(const QDate &_c)
{
    if (_c.isValid())
    {
        eQSLRDate = _c;
        return true;
    }
    else
    {
        eQSLRDate = QDate();
        return false;
    }
}

QDate QSO::getEQSLQSLRDate()
{
    return eQSLRDate;
}

bool QSO::setEQSLQSLSDate(const QDate &_c)
{
    if (_c.isValid())
    {
        eQSLSDate = _c;
        return true;
    }
    else
    {
        eQSLSDate = QDate();
        return false;
    }
}

QDate QSO::getEQSLQSLSDate()
{
    return eQSLSDate;
}

bool QSO::setLoTWQSL_SENT(const QString &_c)
{
    if (util->isValidQSL_Sent(_c))
    {
        lotw_qsl_sent = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getLoTWQSL_SENT()
{
    return lotw_qsl_sent;
}

bool QSO::setLoTWQSLSDate(const QDate &_c)
{
    //qDebug() << Q_FUNC_INFO;
    if (_c.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - valid";
        QSLLoTWSDate = _c;
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - NOT valid";
        QSLLoTWSDate = QDate();
        return false;
    }
}

QDate QSO::getLoTWQSLSDate()
{
    return QSLLoTWSDate;
}

bool QSO::setLoTWQSL_RCVD(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (util->isValidQSL_Rcvd(_c))
    {
        //qDebug() << Q_FUNC_INFO << ": VALID ";
        lotw_qsl_rcvd = _c;
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": NOT valid ";
        return false;
    }
}

QString QSO::getLoTWQSL_RCVD()
{
    return lotw_qsl_rcvd;
}

bool QSO::setLoTWQSLRDate(const QDate &_c)
{
    //qDebug() << Q_FUNC_INFO << ":  << _c.toString("yyyy-MM-dd");
    if (_c.isValid())
    {
        //qDebug() << Q_FUNC_INFO << ":  TRUE";
        QSLLoTWRDate = _c;
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": FALSE";
        QSLLoTWRDate = QDate();
        return false;
    }
}

QDate QSO::getLoTWQSLRDate()
{
    return QSLLoTWRDate;
}

// QSL TAB
bool QSO::setQSL_SENT(const QString &_c)
{
    if (util->isValidQSL_Sent (_c))
    {
        qsl_sent = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getQSL_SENT()
{
    return qsl_sent;
}

bool QSO::setQSLSDate(const QDate &_c)
{
    if (_c.isValid())
    {
        QSLSDate = _c;
        return true;
    }
    else
    {
        QSLSDate = QDate();
        return false;
    }
}

QDate QSO::getQSLSDate()
{
    return QSLSDate;
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

bool QSO::setQSLSenVia(const QString &_qs)
{
    if (!_qs.isEmpty ())
    {
        qslSenVia = _qs;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getQSLSentVia()
{
    return qslSenVia;
}

bool QSO::setQSLRecVia(const QString &_qs)
{
    if (!_qs.isEmpty ())
    {
        qslRecVia = _qs;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getQSLRecVia()
{
    return qslRecVia;
}

bool QSO::setQSLVia(const QString &_qs)
{
    if (!_qs.isEmpty ())
    {
        qslVia = _qs;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getQSLVia()
{
    return qslVia;
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

bool QSO::setQSLMsg(const QString &_qs)
{
    if (!_qs.isEmpty ())
    {
        qslmsg = _qs;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getQSLMsg()
{
    return qslmsg;
}

void QSO::setLoTWUpdating(bool _lotw)
{
    lotwUpdating = _lotw;
}

void QSO::setDefaultEQSLSentServices(const bool _send)
{
    if (_send)
    {
        if ((getLoTWQSL_SENT()).isEmpty())
        {
            setLoTWQSL_SENT ("Q");
        }
        if ((getClubLogStatus ()).isEmpty())
        {
            setClubLogStatus ("M");
        }
        //if (((getEQSLQSL_SENT ()).isEmpty()) || (getEQSLQSL_SENT ().isNull ()))
        if ((getEQSLQSL_SENT ()).isEmpty())
        {
            setEQSLQSL_SENT ("Q");
        }
        if ((getQRZCOMStatus ()).isEmpty())
        {
            setQRZCOMStatus ("M");
        }
    }
}

bool QSO::setGridSquare(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (util->isValidGrid(_c))
    {
        gridsquare = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getGridSquare()
{
    //qDebug() << Q_FUNC_INFO << ": " << gridsquare;
    return gridsquare;
}

bool QSO::setGridSquare_ext(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (util->isValidGrid_ext(_c))
    {
        gridsquare_ext = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getGridSquare_ext()
{
    //qDebug() << Q_FUNC_INFO << ": " << gridsquare;
    return gridsquare_ext;
}

bool QSO::setQTH(const QString &_c)
{
    if (!_c.isEmpty ())
    {
        qth = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getQTH()
{
    return qth;
}

bool QSO::setName(const QString &_c)
{
    if (!_c.isEmpty ())
    {
        name = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getName()
{
    return name;
}

bool QSO::setRSTRX(const QString &_c)
{
    if (!_c.isEmpty ())
    {
        RST_rx = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getRSTRX()
{
    return RST_rx;
}

bool QSO::setRSTTX(const QString &_c)
{
    if (!_c.isEmpty ())
    {
        RST_tx = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getRSTTX()
{
    return RST_tx;
}

bool QSO::setRXPwr(const double _f)
{
    if (_f>0)
    {
        pwr_rx = _f;
        return true;
    }
    else {
        return false;
    }
}

double QSO::getRXPwr()
{
    return pwr_rx;
}

// Comment Tab
bool QSO::setComment(const QString &_c)
{
    if (_c.isNull ())
        return false;
    comment = _c;
    return true;
}

QString QSO::getComment()
{
    return comment;
}

bool QSO::setKeepComment(bool _k)
{
    keepComment = _k;
    return true;
}

bool QSO::getKeepComment()
{
    return keepComment;
}

// Other Tab
bool QSO::setDXCC(const int _i)
{
    if (!util->isValidDXCC(_i))
        return false;
    dxcc = _i;
    return true;
}

int QSO::getDXCC()
{
    return dxcc;
}

bool QSO::setPropMode(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << _c;
    if (!util->isValidPropMode (_c))
    {
        //qDebug() << Q_FUNC_INFO << " - Not valid!";
        propMode = QString();
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - OK END";
    propMode = _c;
    return true;
}

QString QSO::getPropMode()
{
    //qDebug() << Q_FUNC_INFO << ": " << propMode;
    return propMode;
}

bool QSO::setSOTA_REF(const QString &_c)
{// TODO: Check if the SOTA REF is Valid
    if (_c.length ()<=1)
        return false;
    sota_ref = _c;
    return true;
}

QString QSO::getSOTA_REF()
{
    return sota_ref;
}

bool QSO::setMySOTA_REF(const QString &_c)
{
    if (_c.length ()<=1)
        return false;
    my_sota_ref = _c;
    return true;
}

QString QSO::getMySOTA_REF()
{
    return my_sota_ref;
}

bool QSO::setMyRig(const QString &_c)
{
    if (_c.length ()<!1)
        return false;
    my_rig = _c;
    return true;
}

bool QSO::setVUCCGrids(const QString &_c)
{
    if (!util->isValidVUCCGrids (_c))
        return false;
    vucc_grids = _c;
    return true;
}

QString QSO::getVUCCGrids()
{
    return vucc_grids;
}

bool QSO::setMyVUCCGrids(const QString &_c)
{
    if (!util->isValidVUCCGrids (_c))
        return false;
    my_vucc_grids = _c;
    return true;
}

QString QSO::getMyVUCCGrids()
{
    return my_vucc_grids;
}

QString QSO::getMyRig()
{
    return my_rig;
}

bool QSO::setMyAntenna(const QString &_c)
{
    if (_c.length ()<1)
        return false;
    my_antenna = _c;
    return true;
}

QString QSO::getMyAntenna()
{
    return my_antenna;
}

bool QSO::setMyARRL_Sect(const QString &_c)
{
    if (!util->isValidARRLSect (_c))
    {
        return false;
    }
    else
    {
        my_arrl_sect = _c;
        return true;
    }
}

QString QSO::getMyARRL_Sect()
{
    return my_arrl_sect;
}

bool QSO::setPOTA_Ref(const QString &_c)
{
    if (!adif->isValidPOTA(_c))
        return false;
    pota_ref = _c;
    return true;
}

QString QSO::getPOTA_Ref()
{
    return pota_ref;
}

bool QSO::setAge(const double _c)
{
    if ((0 <= _c) && (_c <= 120))
    {
        age = _c;
        return true;
    }
    return false;
}

double QSO::getAge()
{
    return age;
}

bool QSO::setAltitude(const double _c)
{
    altitude = _c;
    return true;
}

double QSO::getAltitude()
{
    return altitude;
}

bool QSO::setIOTA(const QString &_c)
{
    if (_c.length()>0)
    {
        iota = _c;
        return true;
    }
    else
    {
        iota = QString();
        return false;
    }
}

QString QSO::getIOTA()
{
    return iota;
}

bool QSO::setKeepOthers(bool _k)
{
    keepOther = _k;
    return true;
}

bool QSO::getKeepOthers()
{
    return keepOther;
}

// My Data
bool QSO::setTXPwr(double _f)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_f>0)
    {
        pwr_tx = _f;
        logEvent (Q_FUNC_INFO, "END - True", Debug);
        return true;
    }
    else {
        logEvent (Q_FUNC_INFO, "END - False", Debug);
        return false;
    }
}

double QSO::getTXPwr()
{
    return pwr_tx;
}

bool QSO::setOperatorCallsign(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << "Start: " << _c;
    //logEvent(Q_FUNC_INFO, "Start", Debug);
    QString aux = _c;
    if (aux.length()<3)
        return false;


    if (util->isValidCall(aux))
    {
       operatorCall = aux;
       //qDebug() << Q_FUNC_INFO << "END - true";
       logEvent(Q_FUNC_INFO, "END-true", Debug);
       return true;
    }
    else {
        //qDebug() << Q_FUNC_INFO << "End - false";
        logEvent(Q_FUNC_INFO, "END-false", Debug);
        return false;
    }
}

QString QSO::getOperatorCallsign()
{
    return operatorCall;
}

bool QSO::setStationCallsign(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << "Start: " << _c;
    QString aux = _c;
    if ((aux.length()<3) || (aux.isNull()))
    {
        //qDebug() << Q_FUNC_INFO << " - 009";
        return false;
    }

    //qDebug() << Q_FUNC_INFO << " - 010";
    if (util->isValidCall(aux))
    {
        //qDebug() << Q_FUNC_INFO << " - True";
        stationCallsign = aux;
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - False";
       return false;
    }
}

QString QSO::getStationCallsign()
{
    return stationCallsign;
}

bool QSO::setMyGridSquare(const QString &_c)
{
    if (util->isValidGrid(_c))
    {
        my_gridsquare = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getMyGridSquare()
{
    return my_gridsquare;
}

bool QSO::setMyGridSquare_ext(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (util->isValidGrid_ext(_c))
    {
        my_gridsquare_ext = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getMyGridSquare_ext()
{
    //qDebug() << Q_FUNC_INFO << ": " << gridsquare;
    return my_gridsquare_ext;
}

bool QSO::setKeepMyData(bool _k)
{
    keepMyData = _k;
    return true;
}

bool QSO::getKeepMyData()
{
    return keepMyData;
}

// Satellite Tab
bool QSO::setKeepSatTab(bool _k){keepSat = _k; return true;}

bool QSO::getKeepSatTab(){return keepSat;}

bool QSO::setSatName(const QString &_c)
{
    if (_c.length()>0)
    {
        satName = _c;
        return setPropMode("SAT");
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

bool QSO::setSatMode(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (_c.length()>0)
    {
        satMode = _c;
        return true;
    }
    else
    {
        satMode = QString();
        return false;
    }
}

QString QSO::getSatMode()
{
    return satMode;
}

bool QSO::setAddress(const QString &_c)
{
   address = _c;
   return true;
}

QString QSO::getAddress()
{
    return address;
}

bool QSO::setA_Index(const int _i)
{
    if ((_i>=0) && (_i<=400))
    {
        a_index = _i;
        return true;
    }
    return false;
}

int QSO::getA_Index()
{
    return a_index;
}


bool QSO::setAnt_az(const double _c)
{ //TODO: Adjust number: http://www.adif.org/312/ADIF_312.htm#QSO_Field_ANT_AZ
    ant_az = fmod(_c, 360.0);
    return true;
    /*
    if ((0 <= _c) && (_c <= 360))
    {
        //ant_az = _c;

        return true;
    }
    else
    {

    }
    return false;*/
}
double QSO::getAnt_az()
{
    return ant_az;
}

bool QSO::setAnt_el(const double _c)
{ //TODO: Adjust number: http://www.adif.org/312/ADIF_312.htm#QSO_Field_ANT_EL
    if ((-90 <= _c) && (_c <= 90))
    {
        ant_el = _c;
        return true;
    }
    return false;
}

double QSO::getAnt_el()
{
    return ant_el;
}

bool QSO::setAnt_Path(const QString &_c)
{
    if (!adif->isValidAntPath (_c))
        return false;
    ant_path = _c;
    return true;
}

QString QSO::getAnt_Path()
{
    return ant_path;
}

bool QSO::setARRL_Sect(const QString &_c)
{
    if (!util->isValidARRLSect (_c))
        return false;
    arrl_sect = _c;
    return true;
}

QString QSO::getARRL_Sect()
{
    return arrl_sect;
}

bool QSO::setCheck(const QString &_c)
{
    check = _c;
    return true;
}

QString QSO::getCheck()
{
    return check;
}

bool QSO::setClass(const QString &_c)
{
    clase = _c;
    return true;
}

QString QSO::getClass()
{
    return clase;
}

bool QSO::setContinent(const QString &_c)
{
    if (!util->isValidContinent (_c))
        return false;
    continent = _c;
    return true;
}

QString QSO::getContinent()
{
    return continent;
}

bool QSO::setDistance(const double _i)
{
    if (util->isValidDistance(_i))
    {
        distance = _i;
        return true;
    }
    return false;
}

double QSO::getDistance()
{
    return distance;
}

bool QSO::setOwnerCallsign(const QString &_c)
{
    if (util->isValidCall(_c))
    {
       ownerCall = _c;
       return true;
    }
    else {
       return false;
    }
}

QString QSO::getOwnerCallsign()
{
    return ownerCall;
}

bool QSO::setEQ_Call(const QString &_c)
{
    if (util->isValidCall(_c))
    {
        contacted_owner = _c;
        return true;
    }
    else {
       return false;
    }
}

QString QSO::getEQ_Call()
{
    return contacted_owner;
}
// Contacted station owner

bool QSO::setHRDUpdateDate(const QDate &_c)
{
    if (_c.isValid())
    {
        hrdlogUploadDate = _c;
        return true;
    }
    else
    {
        hrdlogUploadDate = QDate();
        return false;
    }
}

QDate QSO::getHRDUpdateDate()
{
    return hrdlogUploadDate;
}


bool QSO::setHRDLogStatus(const QString &_c)
{
    if (util->isValidUpload_Status (_c))
    {
        hrdlog_status = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getHRDLogStatus()
{
    return hrdlog_status;
}

bool QSO::setHamLogEUStatus(const QString &_c)
{
    if (util->isValidUpload_Status (_c))
    {
        hamlogeu_status = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getHamLogEUStatus()
{
    return hamlogeu_status;
}

bool QSO::setHamLogEUUpdateDate(const QDate &_c)
{
    if (_c.isValid())
    {
        hamlogeuUpdateDate = _c;
        return true;
    }
    else
    {
        hamlogeuUpdateDate = QDate();
        return false;
    }
}

QDate QSO::getHamLogEUUpdateDate()
{
    return hamlogeuUpdateDate;
}

bool QSO::setHamQTHStatus(const QString &_c)
{
    if (util->isValidUpload_Status (_c))
    {
        hamqth_status = _c;
        return true;
    }
    else
    {
        return false;
    }
}

QString QSO::getHamQTHStatus()
{
    return hamqth_status;
}

bool QSO::setHamQTHUpdateDate(const QDate &_c)
{
    if (_c.isValid())
    {
        hamqthUpdateDate = _c;
        return true;
    }
    else
    {
        hamqthUpdateDate = QDate();
        return false;
    }
}

QDate QSO::getHamQTHUpdateDate()
{
    return hamqthUpdateDate;
}

bool QSO::setK_Index(const int _i)
{
    if ((_i>=0) && (_i<=400))
    {
        k_index = _i;
        return true;
    }
    return false;
}

int QSO::getK_Index()
{
    return k_index;
}

bool QSO::setRig(const QString &_c)
{
    rig = _c;
    return true;
}

QString QSO::getRig()
{
    return rig;
}

bool QSO::setCountry(const QString &_c)
{
    country = _c;
    return true;
}

QString QSO::getCountry()
{
    return country;
}

bool QSO::setAwardGranted(const QString &_c)
{
    award_granted = _c;
    return true;
}

QString QSO::getAwardGranted()
{
    return award_granted;
}

bool QSO::setAwardSubmitted(const QString &_c)
{
    award_submitted = _c;
    return true;
}

QString QSO::getAwardSubmitted()
{
    return award_submitted;
}

bool QSO::setCounty(const QString &_c)
{
    county = _c;
    return true;
}

QString QSO::getCounty()
{
    return county;
}

bool QSO::setContactedOperator(const QString &_c)
{
    if (util->isValidCall(_c))
    {
       contacted_op = _c;
       return true;
    }
    else {
       return false;
    }
}

QString QSO::getContactedOperator()
{
    return contacted_op;
}

bool QSO::setContestID(const QString &_c)
{
    contest_id = _c;
    return true;
}

QString QSO::getContestID()
{
    return contest_id;
}

bool QSO::setCQZone(const int _i)
{
    if ((_i>0) && (_i<=40))
    {
        cqz = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getCQZone()
{
    return cqz;
}

bool QSO::setCreditGranted(const QString &_c)
{
    credit_granted = _c;
    return true;
}

QString QSO::getCreditGranted()
{
    return credit_granted;
}

bool QSO::setCreditSubmitted(const QString &_c)
{
    credit_submitted = _c;
    return true;
}

QString QSO::getCreditSubmitted()
{
    return credit_submitted;
}

bool QSO::setDarcDok(const QString &_c)
{
    darc_dok = _c;
    return true;
}

QString QSO::getDarcDok()
{
    return darc_dok;
}

bool QSO::setEmail(const QString &_c)
{
    if (util->isValidEmail(_c))
    {
        email = _c;
        return true;
    }
    return false;
}

QString QSO::getEmail()
{
    return email;
}

bool QSO::setFists(const int _i)
{
    if (adif->isValidFISTS(QString::number(_i)))
    {
        fists = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getFists()
{
    return fists;
}

bool QSO::setFistsCC(const int _i)
{
    if (_i>=0)
    {
        fists_cc = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getFistsCC()
{
    return fists_cc;
}

bool QSO::setForceInit(bool _k)
{
    forceInit = _k;
    return true;
}

bool QSO::getForceInit()
{
    return forceInit;
}

bool QSO::setIotaID(const int _i)
{
    if (_i>=0)
    {
        iota_ID = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getIotaID()
{
    return iota_ID;
}

bool QSO::setItuZone(const int _i)
{
    if (_i>0 && (_i<=90))
    {
        itu_zone = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getItuZone()
{
    return itu_zone;
}

bool QSO::setLatitude(const QString &_c)
{
    latitude = _c;
    return true;
}

QString QSO::getLatitude()
{
    return latitude;
}

bool QSO::setLongitude(const QString &_c)
{
    longitude = _c;
    return true;
}

QString QSO::getLongitude()
{
    return longitude;
}

bool QSO::setNrBursts(const int _i)
{
    if (_i>=0)
    {
        nr_bursts = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getNrBursts()
{
    return nr_bursts;
}

bool QSO::setMaxBursts(const int _i)
{
    if (_i>=0)
    {
        max_bursts = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getMaxBursts()
{
    return max_bursts;
}

bool QSO::setNrPings(const int _i)
{
    if (_i>=0)
    {
        nr_pings = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getNrPings()
{
    return nr_pings;
}

bool QSO::setMsShower(const QString &_c)
{
    ms_shower = _c;
    return true;
}

QString QSO::getMsShower()
{
    return ms_shower;
}

bool QSO::setQSOComplete(const QString &_c)
{ // Y, N, I, ? are the valid chars.
  // Here we store the short version just for the DB
  // If we need to export it to ADIF, we need to call util->getADIFQSO_CompleteFromDB()
    Adif adif(Q_FUNC_INFO);
    qso_complete = adif.getQSO_COMPLETEFromDB(_c);
    //qso_complete = util->getQSO_CompleteFromADIF(_c);
    //qDebug() << Q_FUNC_INFO << ": " << qso_complete;
    return true;
}

QString QSO::getQSOComplete()
{
    return qso_complete;
}

bool QSO::setQSORandom(bool _k)
{
    qso_random = _k;
    return true;
}

bool QSO::getQSORandom()
{
    return qso_random;
}

bool QSO::setMyAltitude(const double _c)
{
    my_altitude = _c;
    return true;
}

double QSO::getMyAltitude()
{
    return my_altitude;
}

bool QSO::setMyCity(const QString &_c)
{
    my_city = _c;
    return true;
}

QString QSO::getMyCity()
{
    return my_city;
}

bool QSO::setMyCounty(const QString &_c)
{
    my_county = _c;
    return true;
}

QString QSO::getMyCounty()
{
    return my_county;
}

bool QSO::setMyCountry(const QString &_c)
{
    my_country = _c;
    return true;
}

QString QSO::getMyCountry()
{
    return my_country;
}

bool QSO::setMyCQZone(const int _i)
{
    if ((_i>=0) && (_i<=40))
    {
        my_cqz = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getMyCQZone()
{
    return my_cqz;
}

bool QSO::setMyDXCC(const int _i)
{
    if (util->isValidDXCC(_i))
    {
        my_dxcc = _i;
        return true;
    }
    return false;
}

int QSO::getMyDXCC()
{
    return my_dxcc;
}

bool QSO::setMyFists(const int _i)
{
    if (_i>0)
    {
        my_fists = _i;
        return true;
    }
    return false;
}

int QSO::getMyFists()
{
    return my_fists;
}

bool QSO::setMyIOTA(const QString &_c)
{
    if (_c.length()>0)
    {
        my_iota = _c;
        return true;
    }
    else
    {
        my_iota = QString();
        return false;
    }
}

QString QSO::getMyIOTA()
{
    return my_iota;
}

bool QSO::setMyIotaID(const int _i)
{
    if (_i>=0)
    {
        my_iota_ID = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getMyIotaID()
{
    return my_iota_ID;
}

bool QSO::setMyITUZone(const int _i)
{
    if (_i>=0 && (_i<=90))
    {
        my_itu_zone = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getMyITUZone()
{
    return my_itu_zone;
}

bool QSO::setMyLatitude(const QString &_c)
{
    my_latitude = _c;
    return true;
}

QString QSO::getMyLatitude()
{
    return my_latitude;
}

bool QSO::setMyLongitude(const QString &_c)
{
    my_longitude = _c;
    return true;
}

QString QSO::getMyLongitude()
{
    return my_longitude;
}

bool QSO::setMyName(const QString &_c)
{
    my_name = _c;
    return true;
}

QString QSO::getMyName()
{
    return my_name;
}

bool QSO::setMyPOTA_Ref(const QString &_c)
{
    if (!adif->isValidPOTA(_c))
        return false;
    my_pota_ref = _c;
    return true;
}

QString QSO::getMyPOTA_Ref()
{
    return my_pota_ref;
}

bool QSO::setMyPostalCode(const QString &_c)
{
    my_postal_code = _c;
    return true;
}

QString QSO::getMyPostalCode()
{
    return my_postal_code;
}

bool QSO::setMySig(const QString &_c)
{
    my_sig = _c;
    return true;
}

QString QSO::getMySig()
{
    return my_sig;
}

bool QSO::setMySigInfo(const QString &_c)
{
    my_sig_info = _c;
    return true;
}

QString QSO::getMySigInfo()
{
    return my_sig_info;
}

bool QSO::setMyState(const QString &_c)
{
    my_state = _c;
    return true;
}

QString QSO::getMyState()
{
    return my_state;
}

bool QSO::setMyStreet(const QString &_c)
{
    my_street = _c;
    return true;
}

QString QSO::getMyStreet()
{
    return my_street;
}

bool QSO::setMyUsacaCounties(const QString &_c)
{
    my_usaca_counties = _c;
    return true;
}

QString QSO::getMyUsacaCounties()
{
    return my_usaca_counties;
}

bool QSO::setNotes(const QString &_c)
{
    notes = _c;
    return true;
}

QString QSO::getNotes()
{
    return notes;
}

bool QSO::setPrefix(const QString &_c)
{
    prefix = _c;
    return true;
}

QString QSO::getPrefix()
{
    return prefix;
}

bool QSO::setPrecedence(const QString &_c)
{
    precedence = _c;
    return true;
}

QString QSO::getPrecedence()
{
    return precedence;
}

bool QSO::setPublicKey(const QString &_c)
{
    public_key = _c;
    return true;
}

QString QSO::getPublicKey()
{
    return public_key;
}

bool QSO::setRegion(const QString &_c)
{
    region = _c;
    return true;
}

QString QSO::getRegion()
{
    return region;
}

bool QSO::setTenTen(const int _i)
{
    if (_i>=0)
    {
        ten_ten = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getTenTen()
{
    return ten_ten;
}

bool QSO::setSFI(const int _i)
{
    if (_i>=0 && (_i<=300))
    {
        sfi = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getSFI()
{
    return sfi;
}

bool QSO::setSIG(const QString &_c)
{
    sig = _c;
    return true;
}

QString QSO::getSIG()
{
    return sig;
}

bool QSO::setSIG_INFO(const QString &_c)
{
    sig_info = _c;
    return true;
}

QString QSO::getSIG_INFO()
{
    return sig_info;
}

bool QSO::setSilentKey(bool _k)
{
    silent_key = _k;
    return true;
}

bool QSO::getSilentKey()
{
    return silent_key;
}

bool QSO::setSkcc(const QString &_c)
{
    skcc = _c;
    return true;
}

QString QSO::getSkcc()
{
    return skcc;
}

bool QSO::setSrx(const int _i)
{
    if (_i>=0)
    {
        srx = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getSrx()
{
    return srx;
}

bool QSO::setSrxString(const QString &_c)
{
    srx_string = _c;
    return true;
}

QString QSO::getSrxString()
{
    return srx_string;
}

bool QSO::setStx(const int _i)
{
    if (_i>=0)
    {
        stx = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getStx()
{
    return stx;
}

bool QSO::setStxString(const QString &_c)
{
    stx_string = _c;
    return true;
}

QString QSO::getStxString()
{
    return stx_string;
}

bool QSO::setState(const QString &_c)
{
    state = _c;
    return true;
}

QString QSO::getState()
{
    qDebug() << Q_FUNC_INFO << ": " << state;
    return state;
}

bool QSO::setSubmode(const QString &_c)
{
    submode = _c;
    haveSubMode = true;

    //if (requestMode){
    //    emit getModeSignal(submode);
    //}
    return true;
}

QString QSO::getSubmode()
{
    return submode;
}

bool QSO::setSwl(bool _k)
{
    swl = _k;
    return true;
}

bool QSO::getSwl()
{
    return swl;
}

bool QSO::setUksmg(const int _i)
{
    if (_i>=0)
    {
        uksmg = _i;
        return true;
    }
    else
    {
        return false;
    }
}

int QSO::getUksmg()
{
    return uksmg;
}

bool QSO::setUsacaCounties(const QString &_c)
{
    usaca_counties = _c;
    return true;
}

QString QSO::getUsacaCounties()
{
    return usaca_counties;
}

bool QSO::setVeProv(const QString &_c)
{
    ve_prov = _c;
    return true;
}

QString QSO::getVeProv()
{
    return ve_prov;
}

bool QSO::setWeb(const QString &_c)
{
    web = _c;
    return true;
}

QString QSO::getWeb()
{
    return web;
}

bool QSO::setWWFF_Ref(const QString &_c)
{
    if (!adif->isValidWWFF_Ref(_c))
        return false;
    wwff_ref = _c;
    return true;
}

QString QSO::getWWFF_Ref()
{
    return wwff_ref;
}

bool QSO::setMyWWFF_Ref(const QString &_c)
{
    if (!adif->isValidWWFF_Ref(_c))
        return false;
    my_wwff_ref = _c;
    return true;
}

QString QSO::getMyWWFF_Ref()
{
    return my_wwff_ref;
}


// helper functions for hash, returns original function but takes string data as imput
bool QSO::setAge(const QString &data) { return setAge(data.toDouble()); }
bool QSO::setAltitude(const QString &data) { return setAltitude(data.toDouble()); };
bool QSO::setA_Index(const QString& data) { return setA_Index(data.toInt()); }
bool QSO::setAnt_az(const QString& data) { return setAnt_az(data.toDouble()); }
bool QSO::setAnt_el(const QString& data) { return setAnt_el(data.toDouble()); }
bool QSO::setCQZone(const QString& data) { return setCQZone(data.toInt()); }
bool QSO::setDistance(const QString& data) { return setDistance(data.toDouble()); }
bool QSO::setDXCC(const QString& data) { return setDXCC(data.toInt()); }
bool QSO::setFists(const QString& data) { return setFists(data.toInt()); }
bool QSO::setFistsCC(const QString& data) { return setFistsCC(data.toInt()); }
bool QSO::setMyFists(const QString& data) { return setMyFists(data.toInt()); }
bool QSO::setIotaID(const QString& data) { return setIotaID(data.toInt()); }
bool QSO::setItuZone(const QString& data) { return setItuZone(data.toInt()); }
bool QSO::setK_Index(const QString& data) { return setK_Index(data.toInt()); }
bool QSO::setMaxBursts(const QString& data) { return setMaxBursts(data.toInt()); }
bool QSO::setMyAltitude(const QString &data) { return setMyAltitude(data.toDouble()); };
bool QSO::setMyCQZone(const QString& data) { return setMyCQZone(data.toInt()); }
bool QSO::setMyDXCC(const QString& data) { return setMyDXCC(data.toInt()); }
bool QSO::setMyIotaID(const QString& data) { return setMyIotaID(data.toInt()); }
bool QSO::setMyITUZone(const QString& data) { return setMyITUZone(data.toInt()); }
bool QSO::setNrBursts(const QString& data) { return setNrBursts(data.toInt()); }
bool QSO::setNrPings(const QString& data) { return setNrPings(data.toInt()); }
bool QSO::setSFI(const QString& data) { return setSFI(data.toInt()); }
bool QSO::setSrx(const QString& data) { return setSrx(data.toInt()); }
bool QSO::setStx(const QString& data) { return setStx(data.toInt()); }
bool QSO::setTenTen(const QString& data) { return setTenTen(data.toInt()); }
bool QSO::setUksmg(const QString& data) { return setUksmg(data.toInt()); }

bool QSO::setFreq(const QString& data) { return setFreq(data.toDouble()); }
bool QSO::setFreqRX(const QString& data) { return setFreqRX(data.toDouble()); }
bool QSO::setRXPwr(const QString& data){ return setRXPwr(data.toDouble()); }
bool QSO::setTXPwr(const QString& data){ return setTXPwr(data.toDouble()); }

bool QSO::setClubLogDate(const QString& data) { return setClubLogDate(util->getDateFromADIFDateString(data)); }
bool QSO::setEQSLQSLRDate(const QString& data) { return setEQSLQSLRDate(util->getDateFromADIFDateString(data)); }
bool QSO::setEQSLQSLSDate(const QString& data) { return setEQSLQSLSDate(util->getDateFromADIFDateString(data)); }
bool QSO::setForceInit(const QString& data) { return setForceInit(util->QStringToBool(data)); }
bool QSO::setHRDUpdateDate(const QString& data) { return setHRDUpdateDate(util->getDateFromADIFDateString(data)); }
bool QSO::setHamLogEUUpdateDate(const QString& data) { return setHamLogEUUpdateDate(util->getDateFromADIFDateString(data)); }
bool QSO::setHamQTHUpdateDate(const QString& data) { return setHamQTHUpdateDate(util->getDateFromADIFDateString(data)); }
bool QSO::setLoTWQSLRDate(const QString& data) { return setLoTWQSLRDate(util->getDateFromADIFDateString(data)); }
bool QSO::setLoTWQSLSDate(const QString& data) { return setLoTWQSLSDate(util->getDateFromADIFDateString(data)); }
bool QSO::setQRZCOMDate(const QString& data) { return setQRZCOMDate(util->getDateFromADIFDateString(data)); }
bool QSO::setQSLRDate(const QString& data) { return setQSLRDate(util->getDateFromADIFDateString(data)); }
bool QSO::setQSLSDate(const QString& data) { return setQSLSDate(util->getDateFromADIFDateString(data)); }
bool QSO::setDate(const QString& data) { return setDate(util->getDateFromADIFDateString(data)); }
bool QSO::setDateOff(const QString& data) { return setDateOff(util->getDateFromADIFDateString(data)); }
bool QSO::setQSORandom(const QString& data) {  return setQSORandom(util->QStringToBool(data)); }
bool QSO::setSilentKey(const QString& data) { return setSilentKey(util->QStringToBool(data)); }
bool QSO::setSwl(const QString& data) { return setSwl(util->QStringToBool(data)); }
bool QSO::setTimeOff(const QString& data) { return setTimeOff(util->getTimeFromADIFTimeString(data)); }
bool QSO::setTimeOn(const QString& data) { return setTimeOn(util->getTimeFromADIFTimeString(data)); }

bool QSO::setLoTWQSLRDate2(const QString& data) {
    setLoTWQSL_RCVD("Y");
    return setLoTWQSLRDate(util->getDateFromLoTWQSLDateString(data));
}
bool QSO::setLoTWQSLSDate1(const QString& data) {
    setLoTWQSL_SENT("Y");
    return setLoTWQSLSDate(util->getDateFromLoTWQSLDateString(data));
}
bool QSO::setLoTWQSLSDate2(const QString& data) {
    setLoTWQSL_SENT("Y");
    return setLoTWQSLSDate(util->getDateFromLoTWQSLDateString(data));
}

QHash<QString, decltype(std::mem_fn(&QSO::decltype_function))> QSO::SetDataHash;

void QSO::InitializeHash() {
    SetDataHash = {
        {"ADDRESS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAddress)},
        {"AGE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAge)},
        {"ALTITUDE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAltitude)},
        {"A_INDEX", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setA_Index)},
        {"ANT_AZ", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAnt_az)},
        {"ANT_EL", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAnt_el)},
        {"ANT_PATH", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAnt_Path)},
        {"ARRL_SECT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setARRL_Sect)},
        {"AWARD_SUBMITTED", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAwardSubmitted)},
        {"AWARD_GRANTED", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setAwardGranted)},
        {"BAND", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setBand)},
        {"BAND_RX", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setBandRX)},
        {"CALL", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setCall)},
        {"CHECK", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setCheck)},
        {"CLASS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setClass)},
        {"CLUBLOG_QSO_UPLOAD_DATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setClubLogDate)},
        {"CLUBLOG_QSO_UPLOAD_STATUS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setClubLogStatus)},
        {"CNTY", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setCounty)},
        {"COMMENT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setComment)},
        {"CONT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setContinent)},
        {"CONTACTED_OP", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setContactedOperator)},
        {"CONTEST_ID", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setContestID)},
        {"COUNTRY", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setCountry)},
        {"CQZ", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setCQZone)},
        {"CREDIT_SUBMITTED", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setCreditSubmitted)},
        {"CREDIT_GRANTED", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setCreditGranted)},
        {"DARC_DOK", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setDarcDok)},
        {"DISTANCE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setDistance)},
        {"DXCC", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setDXCC)},
        {"EMAIL", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setEmail)},
        {"EQ_CALL", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setOwnerCallsign)},
        {"EQSL_QSLRDATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setEQSLQSLRDate)},
        {"EQSL_QSLSDATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setEQSLQSLSDate)},
        {"EQSL_QSL_RCVD", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setEQSLQSL_RCVD)},
        {"EQSL_QSL_SENT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setEQSLQSL_SENT)},
        {"FISTS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setFists)},
        {"FISTS_CC", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setFistsCC)},
        {"FORCE_INIT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setForceInit)},
        {"FREQ", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setFreq)},
        {"FREQ_RX", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setFreqRX)},
        {"GRIDSQUARE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setGridSquare)},
        {"GRIDSQUARE_EXT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setGridSquare_ext)},
        {"HRDLOG_QSO_UPLOAD_DATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setHRDUpdateDate)},
        {"HRDLOG_QSO_UPLOAD_STATUS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setHRDLogStatus)},
        {"HAMLOGEU_QSO_UPLOAD_DATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setHamLogEUUpdateDate)},
        {"HAMQTH_QSO_UPLOAD_STATUS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setHamQTHStatus)},
        {"HAMLOGEU_QSO_UPLOAD_STATUS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setHamLogEUStatus)},
        {"IOTA_ISLAND_ID", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setIotaID)},
        {"ITUZ", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setItuZone)},
        {"K_INDEX", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setK_Index)},
        {"LAT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLatitude)},
        {"LON", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLongitude)},
        {"LOTW_QSLRDATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLoTWQSLRDate)},
        {"LOTW_QSLSDATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLoTWQSLSDate)},
        {"LOTW_QSL_RCVD", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLoTWQSL_RCVD)},
        {"LOTW_QSL_SENT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setEQSLQSL_SENT)},
        {"MAX_BURSTS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMaxBursts)},
        {"MODE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMode)},
        {"MS_SHOWER", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMsShower)},
        {"MY_ALTITUDE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyAltitude)},
        {"MY_ANTENNA", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyAntenna)},
        {"MY_ARRL_SECT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyARRL_Sect)},
        {"MY_CITY", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyCity)},
        {"MY_CNTY", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyCounty)},
        {"MY_COUNTRY", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyCountry)},
        {"MY_CQ_ZONE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyCQZone)},
        {"MY_DXCC", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyDXCC)},
        {"MY_FISTS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyFists)},
        {"MY_GRIDSQUARE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyGridSquare)},
        {"MY_GRIDSQUARE_EXT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyGridSquare_ext)},
        {"MY_IOTA", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyIOTA)},
        {"MY_IOTA_ISLAND_ID", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyIotaID)},
        {"MY_ITU_ZONE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyITUZone)},
        {"MY_LAT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyLatitude)},
        {"MY_LON", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyLongitude)},
        {"MY_NAME", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyName)},
        {"MY_POSTAL_CODE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyPostalCode)},
        {"MY_POTA_REF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyPOTA_Ref)},
        {"MY_RIG", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyRig)},
        {"MY_SIG", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMySig)},
        {"MY_SIG_INFO", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMySigInfo)},
        {"MY_SOTA_REF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMySOTA_REF)},
        {"MY_STATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyState)},
        {"MY_STREET", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyStreet)},
        {"MY_USACA_COUNTIES", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyUsacaCounties)},
        {"MY_VUCC_GRIDS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyVUCCGrids)},
        {"MY_WWFF_REF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setMyWWFF_Ref)},
        {"NAME", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setName)},
        {"NOTES", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setNotes)},
        {"NR_BURSTS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setNrBursts)},
        {"NR_PINGS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setNrPings)},
        {"OPERATOR", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setOperatorCallsign)},
        {"OWNER_CALLSIGN", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setOwnerCallsign)},
        {"PFX", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setPrefix)},
        {"POTA_REF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setPOTA_Ref)},
        {"PRECEDENCE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setPrecedence)},
        {"PROP_MODE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setPropMode)},
        {"PUBLIC_KEY", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setPublicKey)},
        {"QRZCOM_QSO_UPLOAD_DATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQRZCOMDate)},
        {"QRZCOM_QSO_UPLOAD_STATUS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQRZCOMStatus)},
        {"QSLMSG", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSLMsg)},
        {"QSLRDATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSLRDate)},
        {"QSLSDATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSLSDate)},
        {"QSL_RCVD", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSL_RCVD)},
        {"QSL_RCVD_VIA", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSLRecVia)},
        {"QSL_SENT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSL_SENT)},
        {"QSL_SENT_VIA", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSLSenVia)},
        {"QSL_VIA", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSLVia)},
        {"QSO_COMPLETE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSOComplete)},
        {"QSO_DATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setDate)},
        {"QSO_DATE_OFF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setDateOff)},
        {"QSO_RANDOM", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQSORandom)},
        {"QTH", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setQTH)},
        {"REGION", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setRegion)},
        {"RIG", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setRig)},
        {"RST_RCVD", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setRSTRX)},
        {"RST_SENT", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setRSTTX)},
        {"RX_PWR", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setRXPwr)},
        {"SAT_MODE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSatMode)},
        {"SAT_NAME", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSatName)},
        {"SFI", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSFI)},
        {"SIG", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSIG)},
        {"SIG_INFO", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSIG_INFO)},
        {"SILENT_KEY", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSilentKey)},
        {"SKCC", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSkcc)},
        {"SOTA_REF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSOTA_REF)},
        {"SRX", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSrx)},
        {"SRX_STRING", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSrxString)},
        {"STATE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setState)},
        {"STATION_CALLSIGN", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setStationCallsign)},
        {"STX", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setStx)},
        {"STX_STRING", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setStxString)},
        {"SUBMODE", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSubmode)},
        {"SWL", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setSwl)},
        {"TEN_TEN", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setTenTen)},
        {"TIME_OFF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setTimeOff)},
        {"TIME_ON", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setTimeOn)},
        {"TX_PWR", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setTXPwr)},
        {"UKSMG", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setUksmg)},
        {"USACA_COUNTIES", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setUsacaCounties)},
        {"VE_PROV", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setVeProv)},
        {"VUCC_GRIDS", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setVUCCGrids)},
        {"WEB", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setWeb)},
        {"WWFF_REF", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setWWFF_Ref)},
        {"APP_LOTW_RXQSL", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLoTWQSLRDate2)},
        {"APP_LOTW_RXQSO", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLoTWQSLSDate1)},
        {"APP_LOTW_QSO_TIMESTAMP", decltype(std::mem_fn(&QSO::decltype_function))(&QSO::setLoTWQSLSDate2)}
    };
    return;
}



// SET DATA ----------------------------------------------------------------------------------
bool QSO::setData(const QString &_adifPair)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
   //qDebug() << Q_FUNC_INFO << ": " << _adifPair;
    QStringList d;
    d.clear();
    d << util->getValidADIFFieldAndData(_adifPair);
    if (d.length()!=2)
    {
        logEvent (Q_FUNC_INFO, "END - ADIF not valid", Debug);
        return false;
    }
   //qDebug() << Q_FUNC_INFO << ": " << d.at(0) << "/" << d.at(1);

    QString field = d.at(0).toUpper();
    QString data = d.at(1);

    if (SetDataHash.empty()) {
        InitializeHash();
    }
    if (SetDataHash.contains(field)) {
        (*SetDataHash.find(field))(this,data);
    }

    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

int QSO::toDB(int _qsoId)
{ // This function will add or modify a QSO in the DB depending on the _qsoID.
  // if _qsoID is >0 it should be an existing QSO in the DB.
    qDebug() << Q_FUNC_INFO << " - Start: qsoId: " << QString::number(_qsoId);
    if (!isComplete ())
    {
        qDebug() << Q_FUNC_INFO << " - QSO NOT COMPLETE";
        return -1;
    }
    qDebug() << Q_FUNC_INFO << " - QSO Complete... adding";
    QString queryString;
    queryString.clear();
    if (_qsoId<=0)
    {
        qDebug() << Q_FUNC_INFO << " - qsoID <=0";
        queryString = getAddQueryString();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " - qsoID>0";
        queryString = getModifyQueryString();
    }
    qDebug() << Q_FUNC_INFO << " Query: " << queryString;;
    QSqlQuery query = getPreparedQuery(queryString);
    if (_qsoId>0)
    {
        qDebug() << Q_FUNC_INFO << " - binding ID";
        query.bindValue (":id", _qsoId);
    }
    qDebug() << Q_FUNC_INFO << " - executing query";
    if (query.exec())
    {
        qDebug() << Q_FUNC_INFO << QString(": QSO ADDED/Modified: %1 - %2").arg(callsign).arg(getDateTimeOn().toString("yyyyMMdd-hhmm"));
        qDebug() << Q_FUNC_INFO << ": QSO ADDED/Modified: " << query.lastQuery ();
        if (_qsoId>0)
            return _qsoId;
        return 1;//db->getLastInsertedQSO();
    }
    else
    {
         qDebug() << Q_FUNC_INFO << QString(": QSO NOT ADDED/Modified: %1 - %2").arg(callsign).arg(_qsoId);
         qDebug() << Q_FUNC_INFO << ": QSO NOT ADDED/Modified: " << query.lastQuery ();
         qDebug() << Q_FUNC_INFO << ": Error: " << query.lastError().databaseText();
         qDebug() << Q_FUNC_INFO << ": Error: " << query.lastError().text();
         qDebug() << Q_FUNC_INFO << ": Error: " << query.lastError().nativeErrorCode();
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return -2;
    }
    query.finish();
    qDebug() << Q_FUNC_INFO << " - END";
    return 1;
}

QString QSO::getAddQueryString()
{   // submode is not used, keep it empty.
    // mode field is populated with the submode
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
        "station_callsign, swl, uksmg, usaca_counties, ve_prov, wwff_ref, vucc_grids, ten_ten, tx_pwr, web, qso_date_off, marked, lognumber) "
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
        ":station_callsign, :swl, :uksmg, :usaca_counties, :ve_prov, :wwff_ref, :vucc_grids, :ten_ten, :tx_pwr, :web, :qso_date_off, "
        ":marked, :lognumber)" );
}

QString QSO::getModifyQueryString()
{ // submode is not used, keep it empty.
  // mode field is populated with the submode
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
                   "state = :state, station_callsign = :station_callsign, swl = :swl, uksmg = :uksmg, "
                   "usaca_counties = :usaca_counties, ve_prov = :ve_prov, wwff_ref = :wwff_ref, vucc_grids = :vucc_grids, ten_ten = :ten_ten, "
                   "tx_pwr = :tx_pwr, web = :web, qso_date_off = :qso_date_off, marked = :marked, lognumber = :lognumber "
                   "WHERE id = :id");
}

int QSO::getBandIdFromBandName(bool _rxBand)
{
    QSqlQuery query;
    //qDebug() << Q_FUNC_INFO << "Band: " << getBand();
    bool ok = query.prepare ("SELECT band.id FROM band WHERE band.name=:bandname");
    if (!ok)
    {
        //qDebug() << Q_FUNC_INFO << " - Query NOT prepared-3122";
    }
    if (_rxBand){
        query.bindValue (":bandname", getBandRX());
    }
    else
    {
        query.bindValue (":bandname", getBand ());
    }

    if (query.exec ())
    {
        if (query.next())
        {
            if (query.isValid ())
            {
                return query.value (0).toInt ();
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << " - Query NOT valid";
            }
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - Query NO Next";
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return -1;
    }
    return -1;
}

QString QSO::getBandNameFromBandId(int bandId)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(bandId);
    QSqlQuery query;
    //qDebug() << Q_FUNC_INFO << "Band: " << getBand();
    bool ok = query.prepare ("SELECT name FROM band WHERE id=:id");
    if (!ok)
    {
        return QString();
        //qDebug() << Q_FUNC_INFO << " - Query NOT prepared-3167";
    }
    query.bindValue (":id", bandId);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return QString();
    }

    if (!query.next())
        return QString();
    if (!query.isValid())
        return QString();

    //qDebug() << Q_FUNC_INFO << ": " << (query.value(0)).toString();
    return (query.value(0)).toString();
}

int QSO::getModeIdFromModeName()
{
    // We need to save always the submode id
    // If submode is no present, then we will store the mode id
   //qDebug() << Q_FUNC_INFO;
    QSqlQuery query;
    //
    // SELECT mode.id FROM mode WHERE mode.submode="FT4"
    // SELECT mode.id FROM mode WHERE mode.name="MFSK"
    bool ok = query.prepare ("SELECT mode.id FROM mode WHERE mode.submode=:submode");
    if (!ok)
    {
        return -1;
    }
    if (haveSubMode)
    {
        query.bindValue (":submode", getSubmode ());
    }
    else if (haveMode)
    {
        query.bindValue (":submode", getMode ());
    }
    else
    {
        return -1;
    }

    if (query.exec ())
    {
        if (query.next())
        {
            if (query.isValid ())
            {
                return query.value (0).toInt ();
            }
            else
            {
                return -4;
            }
        }
        else
        {
            return -5;
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return -2;
    }
    return -3;
}

QString QSO::getModeNameFromModeId(int _modeId, bool _submode)
{
    qDebug() << Q_FUNC_INFO << ": " << QString::number(_modeId);
    QSqlQuery query;
    bool ok;
    if (_submode)
    {
        ok = query.prepare ("SELECT submode FROM mode WHERE id=:id");
    }
    else
    {
        ok = query.prepare ("SELECT name FROM mode WHERE id=:id");
    }

    if (!ok)
    {
        qDebug() << Q_FUNC_INFO << " - Query NOT prepared-3255";
        return QString();
    }
    query.bindValue (":id", _modeId);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return QString();
    }

    if (!query.next())
        return QString();
    if (!query.isValid())
        return QString();

    qDebug() << Q_FUNC_INFO << ": " << (query.value(0)).toString();
    return (query.value(0)).toString();
}

QSqlQuery QSO::getPreparedQuery(const QString &_s)
{
    QSqlQuery query;

    qDebug() << Q_FUNC_INFO << " - Start ";
    qDebug() << Q_FUNC_INFO << " - queryString: " << _s;

    query.clear ();
    if (!query.prepare (_s))
    {
       qDebug() << Q_FUNC_INFO << " - Query not prepared-3285";
       query.clear ();
       return query;
    }
    qDebug() << Q_FUNC_INFO << " - Starting to bind values...";
    query.bindValue(":qso_date", util->getDateTimeSQLiteStringFromDateTime (getDateTimeOn ()));
    query.bindValue(":call", getCall());
    query.bindValue(":rst_sent", getRSTTX());
    query.bindValue(":rst_rcvd", getRSTRX());
    query.bindValue(":bandid", getBandIdFromBandName ());
    query.bindValue(":modeid", getModeIdFromModeName ());
    query.bindValue(":cqz", getCQZone());
    query.bindValue(":ituz", getItuZone());
    query.bindValue(":dxcc", getDXCC());
    query.bindValue(":address", getAddress());
    query.bindValue(":age", getAge());
    query.bindValue(":altitude", getAltitude());

    query.bindValue(":a_index", getA_Index());
    query.bindValue(":ant_az", getAnt_az());
    query.bindValue(":ant_el", getAnt_el());

    query.bindValue(":ant_path", getAnt_Path());
    query.bindValue(":arrl_sect", getARRL_Sect());
    query.bindValue(":award_submitted", getAwardSubmitted ());
    query.bindValue(":award_granted", getAwardGranted ());
    query.bindValue(":band_rx", getBandIdFromBandName(true));
    query.bindValue(":checkcontest", getCheck());
    query.bindValue(":class", getClass());
    query.bindValue(":clublog_qso_upload_date", util->getDateSQLiteStringFromDate(getClubLogDate()));
    query.bindValue(":clublog_qso_upload_status", getClubLogStatus());
    query.bindValue(":cnty", getCounty());
    query.bindValue(":comment", getComment());
    query.bindValue(":cont", getContinent ());
    query.bindValue(":contacted_op", getContactedOperator());
    query.bindValue(":contest_id", getContestID());
    query.bindValue(":country", getCountry());
    query.bindValue(":credit_submitted", getCreditSubmitted());
    query.bindValue(":credit_granted,", getCreditGranted());
    query.bindValue(":darc_dok", getDarcDok ());
    query.bindValue(":distance", getDistance());
    query.bindValue(":email", getEmail());
    query.bindValue(":eq_call", getEQ_Call());
    query.bindValue(":eqsl_qslrdate", util->getDateSQLiteStringFromDate(getEQSLQSLRDate()));
    query.bindValue(":eqsl_qslsdate", util->getDateSQLiteStringFromDate(getEQSLQSLSDate()));

    query.bindValue(":eqsl_qsl_rcvd", getEQSLQSL_RCVD());
    query.bindValue(":eqsl_qsl_sent", getEQSLQSL_SENT());
    query.bindValue(":fists", getFists ());
    query.bindValue(":fists_cc", getFistsCC ());

    query.bindValue(":force_init", util->boolToCharToSQLite (getForceInit()));
    query.bindValue(":freq_tx", getFreqTX());
    query.bindValue(":freq_rx", getFreqRX());
    query.bindValue(":gridsquare", getGridSquare());
    query.bindValue(":gridsquare_ext", getGridSquare_ext());
    query.bindValue(":hrdlog_qso_upload_date", getHRDUpdateDate ());
    query.bindValue(":hrdlog_qso_upload_status", getHRDLogStatus ());

    query.bindValue(":hamlogeu_qso_upload_date", getHamLogEUUpdateDate());
    query.bindValue(":hamlogeu_qso_upload_status", getHamLogEUStatus());
    query.bindValue(":hamqth_qso_upload_date", getHamQTHUpdateDate());
    query.bindValue(":hamqth_qso_upload_status", getHamQTHStatus());


    query.bindValue(":iota", getIOTA());
    query.bindValue(":iota_island_id", getIotaID());
    query.bindValue(":k_index", getK_Index());
    query.bindValue(":lat", getLatitude());
    query.bindValue(":lon", getLongitude());
    query.bindValue(":lotw_qslrdate", util->getDateSQLiteStringFromDate(getLoTWQSLRDate()));
    query.bindValue(":lotw_qslsdate", util->getDateSQLiteStringFromDate(getLoTWQSLSDate()));
    query.bindValue(":lotw_qsl_rcvd", getLoTWQSL_RCVD());
    query.bindValue(":lotw_qsl_sent", getLoTWQSL_SENT());
    query.bindValue(":max_bursts", getMaxBursts());
    query.bindValue(":ms_shower", getMsShower());
    query.bindValue(":my_altitude", getMyAltitude());
    query.bindValue(":my_antenna", getMyAntenna());
    query.bindValue(":my_arrl_sect", getMyARRL_Sect());
    query.bindValue(":my_city", getMyCity());

    query.bindValue(":my_cnty", getMyCounty());
    query.bindValue(":my_country", getMyCountry());
    query.bindValue(":my_cq_zone", getMyCQZone());
    query.bindValue(":my_dxcc", getMyDXCC ());
    query.bindValue(":my_fists", getMyFists ());
    query.bindValue(":my_gridsquare", getMyGridSquare());
    query.bindValue(":my_gridsquare_ext", getMyGridSquare_ext());
    query.bindValue(":my_iota", getMyIOTA());
    query.bindValue(":my_iota_island_id", getMyIotaID());
    query.bindValue(":my_itu_zone", getMyITUZone ());
    query.bindValue(":my_lat", getMyLatitude());
    query.bindValue(":my_lon", getMyLongitude());
    query.bindValue(":my_name", getMyName());
    query.bindValue(":my_pota_ref", getMyPOTA_Ref());

    query.bindValue(":my_postal_code", getMyPostalCode ());
    query.bindValue(":my_rig", getMyRig());

    query.bindValue(":my_sig", getMySig());
    query.bindValue(":my_sig_info", getMySigInfo());
    query.bindValue(":my_sota_ref", getMySOTA_REF());
    query.bindValue(":my_state", getMyState());
    query.bindValue(":my_street", getMyStreet());
    query.bindValue(":my_usaca_counties", getMyUsacaCounties ());
    query.bindValue(":my_wwff_ref", getMyWWFF_Ref());
    query.bindValue(":my_vucc_grids", getMyVUCCGrids());
    query.bindValue(":name", getName());
    query.bindValue(":notes", getNotes());
    query.bindValue(":nr_bursts", getNrBursts());
    query.bindValue(":nr_pings", getNrPings());
    query.bindValue(":operator", getOperatorCallsign());
    query.bindValue(":owner_callsign", getOwnerCallsign());
    query.bindValue(":pfx", getPrefix());

    query.bindValue(":pota_ref", getPOTA_Ref());
    query.bindValue(":precedence", getPrecedence());
    query.bindValue(":prop_mode", getPropMode());
    query.bindValue(":public_key", getPublicKey());
    query.bindValue(":qrzcom_qso_upload_date", util->getDateSQLiteStringFromDate(getQRZCOMDate ()));
    query.bindValue(":qrzcom_qso_upload_status", getQRZCOMStatus ());

    query.bindValue(":qslmsg", getQSLMsg());
    query.bindValue(":qslrdate", util->getDateSQLiteStringFromDate(getQSLRDate()));
    query.bindValue(":qslsdate", util->getDateSQLiteStringFromDate(getQSLSDate()));
    query.bindValue(":qsl_rcvd", getQSL_RCVD());
    query.bindValue(":qsl_sent", getQSL_SENT());
    query.bindValue(":qsl_rcvd_via", getQSLRecVia());

    query.bindValue(":qsl_sent_via", getQSLSentVia());
    query.bindValue(":qsl_via", getQSLVia());
    query.bindValue(":qso_complete", getQSOComplete());
    query.bindValue(":qso_random", util->boolToCharToSQLite (getQSORandom()));
    query.bindValue(":qth", getQTH());
    query.bindValue(":region", getRegion ());
    query.bindValue(":rig", getRig ());

    query.bindValue(":rx_pwr", getRXPwr());
    query.bindValue(":sat_mode", getSatMode());
    query.bindValue(":sat_name",getSatName());
    query.bindValue(":sfi", getSFI());
    query.bindValue(":sig", getSIG());
    query.bindValue(":sig_info", getSIG_INFO ());
    query.bindValue(":silent_key", util->boolToCharToSQLite (getSilentKey ()));
    query.bindValue(":skcc", getSkcc ());

    query.bindValue(":sota_ref", getSOTA_REF());
    query.bindValue(":srx_string", getSrxString());

    query.bindValue(":srx", getSrx());
    query.bindValue(":stx_string", getStxString());
    query.bindValue(":stx", getStx());
    query.bindValue(":state", getState());
    query.bindValue(":station_callsign", getStationCallsign());
    //query.bindValue(":submode", getModeIdFromModeName ());

    query.bindValue(":swl", util->boolToCharToSQLite (getSwl()));
    query.bindValue(":uksmg", getUksmg ());
    query.bindValue(":usaca_counties", getUsacaCounties ());
    query.bindValue(":ve_prov", getVeProv ());
    query.bindValue(":wwff_ref", getWWFF_Ref());
    query.bindValue(":vucc_grids", getVUCCGrids());
    query.bindValue(":ten_ten", getTenTen());
    query.bindValue(":tx_pwr", getTXPwr());
    query.bindValue(":web", getWeb());
    query.bindValue(":qso_date_off", util->getDateSQLiteStringFromDate(getDateOff()));
    query.bindValue(":lognumber", getLogId());

    QVariantList list = query.boundValues();
    for (int i = 0; i < list.size(); ++i)
        qDebug() << Q_FUNC_INFO << QString(": %1").arg(i) << "/ " << list.at(i).toString().toUtf8().data() << "\n";

    qDebug() << Q_FUNC_INFO << " - END";
    return query;
}

QString QSO::getADIF()
{
    if (!isComplete())
        return QString();
    adif = new Adif(Q_FUNC_INFO);

    QString adifStr = QString();
    adifStr.append(adif->getADIFField ("CALL", callsign));
    if (!qso_dateTime.isValid())
        return QString();
    adifStr.append(adif->getADIFField ("QSO_DATE",  util->getADIFDateFromQDateTime(qso_dateTime)));
    adifStr.append(adif->getADIFField ("QSO_DATE_OFF",  util->getADIFDateFromQDate(qso_date_off)));
    adifStr.append(adif->getADIFField ("TIME_ON",  util->getADIFTimeFromQDateTime(qso_dateTime)));
    adifStr.append(adif->getADIFField ("TIME_OFF",  util->getADIFTimeFromQTime(qso_time_off)));
    adifStr.append(adif->getADIFField ("RST_RCVD", RST_rx));
    adifStr.append(adif->getADIFField ("RST_SENT",  RST_tx));
    adifStr.append(adif->getADIFField ("BAND",  band));
    if (QString::compare(band, band_rx) != 0)
        adifStr.append(adif->getADIFField ("BAND_RX",  band_rx));
    adifStr.append(adif->getADIFField ("MODE",  mode));
    if (QString::compare(mode, submode) != 0)
    adifStr.append(adif->getADIFField ("SUBMODE", submode ));

    if (adif->isValidCQz(QString::number(cqz)))
        adifStr.append(adif->getADIFField ("CQZ",  QString::number(cqz) ));

    if (adif->isValidITUz(QString::number(itu_zone)))
        adifStr.append(adif->getADIFField ("ITUZ", QString::number(itu_zone) ));

    if (adif->isValidDXCC(QString::number(dxcc)) && (dxcc>0))
        adifStr.append(adif->getADIFField ("DXCC",  QString::number(dxcc)));
    adifStr.append(adif->getADIFField ("ADDRESS",  address));
    if (age>0.0)  //Only relevant if Age >0
    adifStr.append(adif->getADIFField ("AGE",  QString::number(age)));
    adifStr.append(adif->getADIFField ("ALTITUDE",  QString::number(getAltitude())));
    adifStr.append(adif->getADIFField ("CNTY",  county));
    adifStr.append(adif->getADIFField ("COMMENT",  comment));
    if ((adif->isValidA_Index(QString::number(a_index))) && (a_index>0))
        adifStr.append(adif->getADIFField ("A_INDEX",  QString::number(a_index)));
    if ((adif->isValidAnt_AZ(QString::number(ant_az))) && (ant_az!=0))
        adifStr.append(adif->getADIFField ("ANT_AZ",  QString::number(ant_az)));
    if ((adif->isValidAnt_EL(QString::number(ant_el))) && (ant_el!=0.0) )
        adifStr.append(adif->getADIFField ("ANT_EL",  QString::number(ant_el)));
    adifStr.append(adif->getADIFField ("ANT_PATH", ant_path));
    adifStr.append(adif->getADIFField ("ARRL_SECT", arrl_sect ));
    adifStr.append(adif->getADIFField ("AWARD_SUBMITTED",  award_submitted));
    adifStr.append(adif->getADIFField ("AWARD_GRANTED",  award_granted));

    adifStr.append(adif->getADIFField ("checkcontest",  check));
    adifStr.append(adif->getADIFField ("class",  clase));
    adifStr.append(adif->getADIFField ("clublog_qso_upload_status", clublog_status ));

    if ((clublogQSOUpdateDate .isValid()) && ((clublog_status =="Y") || (clublog_status =="N")))
        adifStr.append(adif->getADIFField ("clublog_qso_upload_date",  util->getADIFDateFromQDate(clublogQSOUpdateDate) ));

    adifStr.append(adif->getADIFField ("cont",  continent));
    adifStr.append(adif->getADIFField ("contacted_op", contacted_op));
    adifStr.append(adif->getADIFField ("contest_id",  contest_id));
    adifStr.append(adif->getADIFField ("country",  country));
    adifStr.append(adif->getADIFField ("credit_submitted",  credit_submitted));
    adifStr.append(adif->getADIFField ("credit_granted", credit_granted ));

    if (distance>0)
        adifStr.append(adif->getADIFField ("darc_dok", QString::number(distance) ));
    adifStr.append(adif->getADIFField ("email",  email));
    adifStr.append(adif->getADIFField ("eq_call",  getEQ_Call()));

    adifStr.append(adif->getADIFField ("eqsl_qsl_rcvd", eqsl_qsl_rcvd));
    if (eQSLRDate.isValid())
        adifStr.append(adif->getADIFField ("eqsl_qslrdate", util->getADIFDateFromQDate(eQSLRDate) ));

    adifStr.append(adif->getADIFField ("eqsl_qsl_sent", eqsl_qsl_sent));
    if (eQSLSDate.isValid())
        adifStr.append(adif->getADIFField ("eqsl_qslsdate", util->getADIFDateFromQDate(eQSLSDate) ));

    if (fists>0)
        adifStr.append(adif->getADIFField ("fists", QString::number(fists)));

    if (fists_cc>0)
        adifStr.append(adif->getADIFField ("fists_cc", QString::number(fists_cc)));
    if (forceInit)      // Only relevant if true
        adifStr.append(adif->getADIFField ("force_init", adif->getADIFBoolFromBool(getForceInit()) ));

    //qDebug() << Q_FUNC_INFO << ": Printing FREQ: " << QString::number(freq_tx);
    if (adif->isValidFreq(QString::number(freq_tx)))
        adifStr.append(adif->getADIFField ("freq",  QString::number(freq_tx)));

    //qDebug() << Q_FUNC_INFO << ": Printing FREQ_RX";
    if ((adif->isValidFreq(QString::number(freq_rx))) && (freq_tx != freq_rx))
        adifStr.append(adif->getADIFField ("freq_rx", QString::number(freq_rx) ));
    if (util->isValidGrid_ext(gridsquare))
        adifStr.append(adif->getADIFField ("gridsquare",  gridsquare));
    if (util->isValidGrid_ext(gridsquare_ext))
        adifStr.append(adif->getADIFField ("gridsquare_ext",  gridsquare_ext));
    adifStr.append(adif->getADIFField ("hrdlog_qso_upload_date",  util->getADIFDateFromQDate(hrdlogUploadDate)));
    adifStr.append(adif->getADIFField ("hrdlog_qso_upload_status", hrdlog_status ));
    adifStr.append(adif->getADIFField ("hamlogeu_qso_upload_status", hamlogeu_status ));
    adifStr.append(adif->getADIFField ("hamlogeu_qso_upload_date",  util->getADIFDateFromQDate(hamlogeuUpdateDate)));
    adifStr.append(adif->getADIFField ("hamqth_qso_upload_status", hamqth_status ));
    adifStr.append(adif->getADIFField ("hamqth_qso_upload_date",  util->getADIFDateFromQDate(hamqthUpdateDate)));


    adifStr.append(adif->getADIFField ("iota", iota));

    if (iota_ID>0)
        adifStr.append(adif->getADIFField ("iota_island_id", QString::number(iota_ID)));

    if ((adif->isValidK_Index(QString::number(k_index))) && (k_index>0))
        adifStr.append(adif->getADIFField ("k_index", QString::number(k_index)));
    adifStr.append(adif->getADIFField ("lat", latitude));
    adifStr.append(adif->getADIFField ("lon", longitude));
    adifStr.append(adif->getADIFField ("lotw_qslrdate", util->getADIFDateFromQDate(QSLLoTWRDate)));
    adifStr.append(adif->getADIFField ("lotw_qslsdate", util->getADIFDateFromQDate(QSLLoTWSDate)));
    adifStr.append(adif->getADIFField ("lotw_qsl_rcvd", lotw_qsl_rcvd));
    adifStr.append(adif->getADIFField ("lotw_qsl_sent", lotw_qsl_sent));

    if (adif->isValidNRBursts(QString::number(getMaxBursts())))
        adifStr.append(adif->getADIFField ("max_bursts", QString::number(getMaxBursts()) ));

    adifStr.append(adif->getADIFField ("ms_shower",  ms_shower));
    adifStr.append(adif->getADIFField ("my_altitude",  QString::number(getMyAltitude())));
    adifStr.append(adif->getADIFField ("my_antenna", my_antenna));
    adifStr.append(adif->getADIFField ("my_arrl_sect", my_arrl_sect ));
    adifStr.append(adif->getADIFField ("my_city", my_city));

    adifStr.append(adif->getADIFField ("my_cnty", my_county));
    adifStr.append(adif->getADIFField ("my_country", my_country));

    if (adif->isValidCQz(QString::number(my_cqz)))
        adifStr.append(adif->getADIFField ("my_cq_zone", QString::number(my_cqz)));

    if (adif->isValidDXCC(QString::number(my_dxcc)) && (my_dxcc>0))
        adifStr.append(adif->getADIFField ("my_dxcc", QString::number(my_dxcc) ));

    if (adif->isValidFISTS(QString::number(my_fists)))
        adifStr.append(adif->getADIFField ("my_fists", QString::number(my_fists) ));
    if (util->isValidGrid_ext(my_gridsquare))
        adifStr.append(adif->getADIFField ("my_gridsquare", my_gridsquare ));
    if (util->isValidGrid_ext(my_gridsquare_ext))
        adifStr.append(adif->getADIFField ("my_gridsquare_ext", my_gridsquare_ext));
    adifStr.append(adif->getADIFField ("my_iota", my_iota));
    if (adif->isValidIOTA_islandID(QString::number(getMyIotaID())))
        adifStr.append(adif->getADIFField ("my_iota_island_id", QString::number(getMyIotaID())));

    if (adif->isValidITUz(QString::number(my_itu_zone)))
        adifStr.append(adif->getADIFField ("my_itu_zone", QString::number(my_itu_zone)));

    adifStr.append(adif->getADIFField ("my_lat", my_latitude));
    adifStr.append(adif->getADIFField ("my_lon", my_longitude));
    adifStr.append(adif->getADIFField ("my_name", my_name));
    adifStr.append(adif->getADIFField ("my_pota_ref", my_pota_ref));
    adifStr.append(adif->getADIFField ("my_postal_code", my_postal_code));
    adifStr.append(adif->getADIFField ("my_rig", my_rig));
    adifStr.append(adif->getADIFField ("my_sig", my_sig));
    adifStr.append(adif->getADIFField ("my_sig_info", my_sig_info));

    adifStr.append(adif->getADIFField ("my_sota_ref", my_sota_ref));
    adifStr.append(adif->getADIFField ("my_state", my_state));
    adifStr.append(adif->getADIFField ("my_street", my_street));
    adifStr.append(adif->getADIFField ("my_usaca_counties", my_usaca_counties));
    adifStr.append(adif->getADIFField ("my_wwff_ref", my_wwff_ref));
    adifStr.append(adif->getADIFField ("my_vucc_grids", my_vucc_grids));
    adifStr.append(adif->getADIFField ("name", name));
    adifStr.append(adif->getADIFField ("notes", notes));
    if (adif->isValidNRBursts(QString::number(nr_bursts)))
        adifStr.append(adif->getADIFField ("nr_bursts", QString::number(nr_bursts)));
    if (adif->isValidPings(QString::number(nr_pings)))
        adifStr.append(adif->getADIFField ("nr_pings", QString::number(nr_pings)));
    adifStr.append(adif->getADIFField ("operator", operatorCall));
    adifStr.append(adif->getADIFField ("owner_callsign", ownerCall));
    adifStr.append(adif->getADIFField ("pfx", prefix));
    adifStr.append(adif->getADIFField ("pota_ref", pota_ref));
    adifStr.append(adif->getADIFField ("precedence", precedence));
    adifStr.append(adif->getADIFField ("prop_mode", propMode));
    adifStr.append(adif->getADIFField ("public_key", public_key));
    adifStr.append(adif->getADIFField ("qrzcom_qso_upload_date",  util->getADIFDateFromQDate(QRZComDate) ));
    adifStr.append(adif->getADIFField ("qrzcom_qso_upload_status", QRZCom_status ));

    adifStr.append(adif->getADIFField ("qslmsg", qslmsg));
    adifStr.append(adif->getADIFField ("qsl_rcvd", getQSL_RCVD()));

    if ((QSLRDate.isValid()) && ( (qsl_rcvd=="Y") || (qsl_rcvd=='I') || (qsl_rcvd=='R')))
        adifStr.append(adif->getADIFField ("qslrdate", util->getADIFDateFromQDate(QSLRDate) ));

    adifStr.append(adif->getADIFField ("qsl_sent", getQSL_SENT()));
    if ((QSLSDate.isValid()) && ( (qsl_sent=="Y") || (qsl_sent=='Q') || (qsl_sent=='I')))
        adifStr.append(adif->getADIFField ("qslsdate", util->getADIFDateFromQDate(QSLSDate)));


    if ((getQSL_RCVD()=="Y") or (getQSL_RCVD()=="V")) // Valid cases to use qslrcvdVia
        adifStr.append(adif->getADIFField ("qsl_rcvd_via", qslRecVia));

     if (getQSL_SENT()=="Y")                // Valid case to use qslsentVia
        adifStr.append(adif->getADIFField ("qsl_sent_via", qslSenVia));
    adifStr.append(adif->getADIFField ("qsl_via", qslVia));
    adifStr.append(adif->getADIFField ("qso_complete", util->getADIFQSO_CompleteFromDB(getQSOComplete())));

    //TODO: Check wether it makes sense to use this field for ALL QSOs or just when it is not random.
    if (getQSORandom())
        adifStr.append(adif->getADIFField ("qso_random", adif->getADIFBoolFromBool(getQSORandom())));

    adifStr.append(adif->getADIFField ("qth", qth));
    adifStr.append(adif->getADIFField ("region", region));
    adifStr.append(adif->getADIFField ("rig", rig));

    if (adif->isValidPower(QString::number(pwr_rx)))
        adifStr.append(adif->getADIFField ("rx_pwr", QString::number(pwr_rx)));
    adifStr.append(adif->getADIFField ("sat_mode", getSatMode()));
    adifStr.append(adif->getADIFField ("sat_name", getSatName()));

    if ((adif->isValidSFI(QString::number(sfi))) && (sfi>0))
        adifStr.append(adif->getADIFField ("sfi", QString::number(sfi)));
    adifStr.append(adif->getADIFField ("sig", sig));
    adifStr.append(adif->getADIFField ("sig_info", sig_info));
    if (silent_key) //We only save if SK
        adifStr.append(adif->getADIFField ("silent_key", adif->getADIFBoolFromBool(silent_key)));
    adifStr.append(adif->getADIFField ("skcc", skcc));

    adifStr.append(adif->getADIFField ("sota_ref", sota_ref));
    adifStr.append(adif->getADIFField ("srx_string", srx_string));
    if (adif->isValidSRX(QString::number(nr_bursts)))
        adifStr.append(adif->getADIFField ("srx", QString::number(srx)));
    adifStr.append(adif->getADIFField ("stx_string", stx_string));
    if (adif->isValidSTX(QString::number(nr_bursts)))
        adifStr.append(adif->getADIFField ("stx", QString::number(stx)));

    adifStr.append(adif->getADIFField ("state", state));
    adifStr.append(adif->getADIFField ("station_callsign", stationCallsign));
    if (swl)    // We only print it if it is a SWL QSO
        adifStr.append(adif->getADIFField ("swl", adif->getADIFBoolFromBool(swl)));
    if (adif->isValidUKSMG(QString::number(uksmg)))
        adifStr.append(adif->getADIFField ("uksmg", QString::number(uksmg)));

    adifStr.append(adif->getADIFField ("usaca_counties", usaca_counties));
    adifStr.append(adif->getADIFField ("ve_prov", ve_prov));
    adifStr.append(adif->getADIFField ("wwff_ref", wwff_ref));
    adifStr.append(adif->getADIFField ("vucc_grids", vucc_grids));

    if (adif->isValidTenTen(QString::number(ten_ten)))
        adifStr.append(adif->getADIFField ("ten_ten", QString::number(ten_ten)));
    if (adif->isValidPower(QString::number(pwr_tx)))
        adifStr.append(adif->getADIFField ("tx_pwr", QString::number(pwr_tx)));
    adifStr.append(adif->getADIFField ("web", web));
    if (adif->isValidLogId(QString::number(getLogId())))
        adifStr.append(adif->getADIFField ("APP_KLOG_LOGN", QString::number(getLogId())));
    //return  qso.getADIF() + "<EOR>\n";
    return adifStr + "<EOR>\n";;
}

QString QSO::getBandNameFromFreq(const double _n)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    bool sqlOk = false;
    QString queryString = QString("SELECT name FROM band WHERE lower <= '%1' and upper >= '%1'").arg(_n);

    QSqlQuery query(queryString);
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
}

bool QSO::fromDB(int _qsoId)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QString queryString = "SELECT * FROM log WHERE id=:idQSO";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":idQSO", _qsoId);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return false;
    }
    if (!query.next())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return false;
    }
    clear();
    QSqlRecord rec = query.record();

    //qDebug() << Q_FUNC_INFO << "  - 20";
    QString data = (query.value(rec.indexOf("qso_date"))).toString();
    setDateTimeOn(util->getDateTimeFromSQLiteString(data));

    data = (query.value(rec.indexOf("call"))).toString();
    setCall(data);

    data = (query.value(rec.indexOf("rst_sent"))).toString();
    setRSTTX(data);

    data = (query.value(rec.indexOf("rst_rcvd"))).toString();
    setRSTRX(data);

    data = (query.value(rec.indexOf("bandid"))).toString();
    data = getBandNameFromBandId(data.toInt());
    setBand(data);

    data = (query.value(rec.indexOf("band_rx"))).toString();
    data = getBandNameFromBandId(data.toInt());
    setBandRX(data);

    data = (query.value(rec.indexOf("modeid"))).toString();
    setMode(getModeNameFromModeId(data.toInt(), false));
    setSubmode(getModeNameFromModeId(data.toInt(), true));
    //qDebug() << Q_FUNC_INFO << "  - 30";
    setCQZone((query.value(rec.indexOf("cqz"))).toInt());
    setItuZone((query.value(rec.indexOf("ituz"))).toInt());
    setDXCC((query.value(rec.indexOf("dxcc"))).toInt());

    setAddress((query.value(rec.indexOf("address"))).toString());
    setAge((query.value(rec.indexOf("age"))).toDouble());
    setAltitude((query.value(rec.indexOf("altitude"))).toDouble());
    setCounty((query.value(rec.indexOf("cnty"))).toString());

    setA_Index((query.value(rec.indexOf("a_index"))).toInt());
    setAnt_az((query.value(rec.indexOf("ant_az"))).toDouble());
    setAnt_el((query.value(rec.indexOf("ant_el"))).toDouble());
    setAnt_Path((query.value(rec.indexOf("ant_path"))).toString());

    setARRL_Sect((query.value(rec.indexOf("arrl_sect"))).toString());
    setAwardSubmitted((query.value(rec.indexOf("award_submitted"))).toString());
    setAwardGranted((query.value(rec.indexOf("award_granted"))).toString());

    setCheck((query.value(rec.indexOf("checkcontest"))).toString());
    setClass((query.value(rec.indexOf("class"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 40";
    data = (query.value(rec.indexOf("clublog_qso_upload_date"))).toString();
    setClubLogDate(util->getDateTimeFromSQLiteString(data).date());
    setClubLogStatus((query.value(rec.indexOf("clublog_qso_upload_status"))).toString());

    setContinent((query.value(rec.indexOf("cont"))).toString());
    setContactedOperator((query.value(rec.indexOf("contacted_op"))).toString());
    setContestID((query.value(rec.indexOf("contest_id"))).toString());

    setCountry((query.value(rec.indexOf("country"))).toString());
    setCreditSubmitted((query.value(rec.indexOf("credit_submitted"))).toString());
    setCreditGranted((query.value(rec.indexOf("credit_granted"))).toString());

    setDarcDok((query.value(rec.indexOf("darc_dok"))).toString());
    setDistance((query.value(rec.indexOf("distance"))).toDouble());

    setEmail((query.value(rec.indexOf("email"))).toString());
    setEQ_Call((query.value(rec.indexOf("eq_call"))).toString());

    data = (query.value(rec.indexOf("eqsl_qslrdate"))).toString();
    setEQSLQSLRDate(util->getDateTimeFromSQLiteString(data).date());

    data = (query.value(rec.indexOf("eqsl_qslsdate"))).toString();
    setEQSLQSLSDate(util->getDateTimeFromSQLiteString(data).date());
    //qDebug() << Q_FUNC_INFO << "  - 50";
    setEQSLQSL_RCVD((query.value(rec.indexOf("eqsl_qsl_rcvd"))).toString());
    setEQSLQSL_SENT((query.value(rec.indexOf("eqsl_qsl_sent"))).toString());
    setFists((query.value(rec.indexOf("fists"))).toInt());
    setFistsCC((query.value(rec.indexOf("fists_cc"))).toInt());
    setForceInit(util->QStringToBool((query.value(rec.indexOf("force_init"))).toString()));
    setFreq((query.value(rec.indexOf("freq"))).toDouble());
    setFreqRX((query.value(rec.indexOf("freq_rx"))).toDouble());
    setGridSquare((query.value(rec.indexOf("gridsquare"))).toString());
    setGridSquare_ext((query.value(rec.indexOf("gridsquare_ext"))).toString());
    data = (query.value(rec.indexOf("hrdlog_qso_upload_date"))).toString();
    setHRDUpdateDate(util->getDateTimeFromSQLiteString(data).date());

    //qDebug() << Q_FUNC_INFO << "  - 60";
    setHRDLogStatus((query.value(rec.indexOf("hrdlog_qso_upload_status"))).toString());

    data = (query.value(rec.indexOf("hamlogeu_qso_upload_date"))).toString();
    setHamLogEUUpdateDate(util->getDateTimeFromSQLiteString(data).date());
    setHamLogEUStatus((query.value(rec.indexOf("hamlogeu_qso_upload_status"))).toString());

    data = (query.value(rec.indexOf("hamqth_qso_upload_date"))).toString();
    setHamQTHUpdateDate(util->getDateTimeFromSQLiteString(data).date());
    setHamQTHStatus((query.value(rec.indexOf("hamqth_qso_upload_status"))).toString());


    //qDebug() << Q_FUNC_INFO << "  - 61";
    setIOTA((query.value(rec.indexOf("iota"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 62";
    setIotaID((query.value(rec.indexOf("iota_island_id"))).toInt());
    //qDebug() << Q_FUNC_INFO << "  - 63";
    setK_Index((query.value(rec.indexOf("k_index"))).toInt());
    //qDebug() << Q_FUNC_INFO << "  - 64";
    setLatitude((query.value(rec.indexOf("lat"))).toString());
    setLongitude((query.value(rec.indexOf("lon"))).toString());

    data = (query.value(rec.indexOf("lotw_qslrdate"))).toString();
    setLoTWQSLRDate(util->getDateTimeFromSQLiteString(data).date());
    data = (query.value(rec.indexOf("lotw_qslsdate"))).toString();
    setLoTWQSLSDate(util->getDateTimeFromSQLiteString(data).date());

    setLoTWQSL_RCVD((query.value(rec.indexOf("lotw_qsl_rcvd"))).toString());
    setLoTWQSL_SENT((query.value(rec.indexOf("lotw_qsl_sent"))).toString());

    setMaxBursts((query.value(rec.indexOf("max_bursts"))).toInt());
    setMsShower((query.value(rec.indexOf("ms_shower"))).toString());
    setMyAltitude((query.value(rec.indexOf("my_altitude"))).toDouble());
    setMyAntenna((query.value(rec.indexOf("my_antenna"))).toString());
    setMyARRL_Sect((query.value(rec.indexOf("my_arrl_sect"))).toString());
    setMyCity((query.value(rec.indexOf("my_city"))).toString());
    setMyCounty((query.value(rec.indexOf("my_cnty"))).toString());
    setMyCountry((query.value(rec.indexOf("my_country"))).toString());
    setMyCQZone((query.value(rec.indexOf("my_cq_zone"))).toInt());
    setMyITUZone((query.value(rec.indexOf("my_itu_zone"))).toInt());
    setMyDXCC((query.value(rec.indexOf("my_dxcc"))).toInt());
    setMyFists((query.value(rec.indexOf("my_fists"))).toInt());
    setMyGridSquare((query.value(rec.indexOf("my_gridsquare"))).toString());
    setMyGridSquare_ext((query.value(rec.indexOf("my_gridsquare_ext"))).toString());
    setMyIOTA((query.value(rec.indexOf("my_iota"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 80";
    setMyLatitude((query.value(rec.indexOf("my_lat"))).toString());
    setMyLongitude((query.value(rec.indexOf("my_lon"))).toString());

    setMyName((query.value(rec.indexOf("my_name"))).toString());
    setMyPOTA_Ref((query.value(rec.indexOf("my_pota_ref"))).toString());
    setMyPostalCode((query.value(rec.indexOf("my_postal_code"))).toString());
    setMyRig((query.value(rec.indexOf("my_rig"))).toString());
    setMySig((query.value(rec.indexOf("my_sig"))).toString());
    setMySigInfo((query.value(rec.indexOf("my_sig_info"))).toString());
    setMySOTA_REF((query.value(rec.indexOf("my_sota_ref"))).toString());
    setMyState((query.value(rec.indexOf("my_state"))).toString());
    setMyStreet((query.value(rec.indexOf("my_street"))).toString());
    setMyUsacaCounties((query.value(rec.indexOf("my_usaca_counties"))).toString());
    setMyWWFF_Ref((query.value(rec.indexOf("my_wwff_ref"))).toString());
    setMyVUCCGrids((query.value(rec.indexOf("my_vucc_grids"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 90";
    setName((query.value(rec.indexOf("name"))).toString());
    setNotes((query.value(rec.indexOf("notes"))).toString());

    setNrBursts((query.value(rec.indexOf("nr_bursts"))).toInt());
    setNrPings((query.value(rec.indexOf("nr_pings"))).toInt());
    setOperatorCallsign((query.value(rec.indexOf("operator"))).toString());
    setOwnerCallsign((query.value(rec.indexOf("owner_callsign"))).toString());
    setPrefix((query.value(rec.indexOf("pfx"))).toString());
    setPOTA_Ref((query.value(rec.indexOf("pota_ref"))).toString());
    setPrecedence((query.value(rec.indexOf("precedence"))).toString());
    setPropMode((query.value(rec.indexOf("prop_mode"))).toString());
    setPublicKey((query.value(rec.indexOf("public_key"))).toString());

    data = (query.value(rec.indexOf("qrzcom_qso_upload_date"))).toString();
    setQRZCOMDate(util->getDateTimeFromSQLiteString(data).date());
    setQRZCOMStatus((query.value(rec.indexOf("qrzcom_qso_upload_status"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 100";
    setQSLMsg((query.value(rec.indexOf("qslmsg"))).toString());
    data = (query.value(rec.indexOf("qslrdate"))).toString();
    setQSLRDate(util->getDateTimeFromSQLiteString(data).date());
    data = (query.value(rec.indexOf("qslsdate"))).toString();
    setQSLSDate(util->getDateTimeFromSQLiteString(data).date());

    setQSL_RCVD((query.value(rec.indexOf("qsl_rcvd"))).toString());
    setQSL_SENT((query.value(rec.indexOf("qsl_sent"))).toString());
    setQSLRecVia((query.value(rec.indexOf("qsl_rcvd_via"))).toString());
    setQSLSenVia((query.value(rec.indexOf("qsl_sent_via"))).toString());

    setQSLVia((query.value(rec.indexOf("qsl_via"))).toString());
    setQSOComplete((query.value(rec.indexOf("qso_complete"))).toString());
    setQSORandom(util->QStringToBool((query.value(rec.indexOf("qso_random"))).toString()));
    //qDebug() << Q_FUNC_INFO << "  - 120";
    setQTH((query.value(rec.indexOf("qth"))).toString());
    setRegion((query.value(rec.indexOf("region"))).toString());
    setRig((query.value(rec.indexOf("rig"))).toString());
    setRXPwr((query.value(rec.indexOf("rig"))).toDouble());

    setSatName((query.value(rec.indexOf("sat_name"))).toString());
    setSatMode((query.value(rec.indexOf("sat_mode"))).toString());

    setSFI((query.value(rec.indexOf("sfi"))).toInt());
    setSIG((query.value(rec.indexOf("sig"))).toString());
    setSIG_INFO((query.value(rec.indexOf("sig_info"))).toString());

    setSilentKey(util->QStringToBool((query.value(rec.indexOf("silent_key"))).toString()));
    setSkcc((query.value(rec.indexOf("skcc"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 130";
    setSOTA_REF((query.value(rec.indexOf("sota_ref"))).toString());
    setSrxString((query.value(rec.indexOf("srx_string"))).toString());
    setSrx((query.value(rec.indexOf("srx"))).toInt());
    setStxString((query.value(rec.indexOf("stx_string"))).toString());
    setStx((query.value(rec.indexOf("stx"))).toInt());

    setState((query.value(rec.indexOf("state"))).toString());
    setStationCallsign((query.value(rec.indexOf("station_callsign"))).toString());

    setSwl(util->QStringToBool((query.value(rec.indexOf("swl"))).toString()));
    setUksmg((query.value(rec.indexOf("uksmg"))).toString());
    setUsacaCounties((query.value(rec.indexOf("usaca_counties"))).toString());
    setVeProv((query.value(rec.indexOf("ve_prov"))).toString());
    setVUCCGrids((query.value(rec.indexOf("vucc_grids"))).toString());
    setWWFF_Ref((query.value(rec.indexOf("wwff_ref"))).toString());
    setTenTen((query.value(rec.indexOf("ten_ten"))).toInt());
    setTXPwr((query.value(rec.indexOf("tx_pwr"))).toDouble());
    //qDebug() << Q_FUNC_INFO << "  - 140";
    setWeb((query.value(rec.indexOf("web"))).toString());
    data = (query.value(rec.indexOf("qso_date_off"))).toString();
    setDateOff(util->getDateTimeFromSQLiteString(data).date());
    setLogId((query.value(rec.indexOf("lognumber"))).toInt());
    //qDebug() << Q_FUNC_INFO << "  - 150";
    logEvent (Q_FUNC_INFO, "END", Debug);
    return false;
}
