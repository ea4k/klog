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
#include "QtSql/qsqlerror.h"
#include "qsqlrecord.h"
#include "callsign.h"

QSO::QSO(QObject *parent)
    : QObject(parent)
{
    //startT = QTime::currentTime();
   //qDebug() << Q_FUNC_INFO << " - " << startT.msec();
    logLevel = None;
    qsoId = -1;
    util = new Utilities(Q_FUNC_INFO);

    //db = new DataBase(Q_FUNC_INFO, "1", util->getKLogDBFile());
    //db = new DataBase(Q_FUNC_INFO, klogVersion, util->getKLogDBFile());
}

QSO::QSO(const QSO &other)
    : QObject(other.parent())
{
    //startT = QTime::currentTime();
  //qDebug() << Q_FUNC_INFO << " - " << startT.msec();
   //qDebug() << Q_FUNC_INFO << " (2): " << other.callsign;
    util = new Utilities(Q_FUNC_INFO);
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
   //qDebug() << Q_FUNC_INFO;
    delete(util);
}

void QSO::operator=(QSO const &_other)
{
   //qDebug() << Q_FUNC_INFO;
   //qDebug() << Q_FUNC_INFO << " - " << startT.msec();
    callsign        = _other.callsign;
    qsoId           = _other.qsoId;
    logId           = _other.logId;
    dxcc            = _other.dxcc;
    a_index         = _other.a_index;
    k_index         = _other.k_index;
    cqz             = _other.cqz;
    fists           = _other.fists;
    fists_cc        = _other.fists_cc;
    my_fists        = _other.my_fists;
    iota_ID         = _other.iota_ID;
    itu_zone        = _other.itu_zone;
    nr_bursts       = _other.nr_bursts;
    max_bursts      = _other.max_bursts;
    nr_pings        = _other.nr_pings;
    my_cqz          = _other.my_cqz;
    my_itu_zone     = _other.my_itu_zone;
    my_dxcc         = _other.my_dxcc;
    my_iota_ID      = _other.my_iota_ID;
    srx             = _other.srx;
    stx             = _other.stx;
    uksmg           = _other.uksmg;
    ten_ten         = _other.ten_ten;
    sfi             = _other.sfi;
    freq_tx         = _other.freq_tx;
    freq_rx         = _other.freq_rx;
    pwr_rx          = _other.pwr_rx;
    pwr_tx          = _other.pwr_tx;
    age             = _other.age;
    ant_el          = _other.ant_el;
    ant_az          = _other.ant_az;
    distance        = _other.distance;
    altitude        = _other.altitude;
    my_altitude     = _other.my_altitude;

    satName         = _other.satName;
    satMode         = _other.satMode;
    stationCallsign = _other.stationCallsign;
    operatorCall    = _other.operatorCall;
    propMode        = _other.propMode;
    band            = _other.band;
    band_rx         = _other.band_rx;
    mode            = _other.mode;
    gridsquare      = _other.gridsquare;
    my_gridsquare   = _other.my_gridsquare;
    gridsquare_ext  = _other.gridsquare_ext;
    my_gridsquare_ext   = _other.my_gridsquare_ext;
    qth             = _other.qth;
    name            = _other.name;
    RST_tx          = _other.RST_tx;
    RST_rx          = _other.RST_rx;

    qsl_rcvd        = _other.qsl_rcvd;
    qsl_sent        = _other.qsl_sent;
    qslSenVia       = _other.qslSenVia;
    qslRecVia       = _other.qslRecVia;
    qslVia          = _other.qslVia;
    check           = _other.check;
    clase           = _other.clase;

    lotw_qsl_sent   = _other.lotw_qsl_sent;
    lotw_qsl_rcvd   = _other.lotw_qsl_rcvd;
    sota_ref        = _other.sota_ref;
    my_sota_ref     = _other.my_sota_ref;
    my_rig          = _other.my_rig;
    my_antenna      = _other.my_antenna;
    my_arrl_sect    = _other.my_arrl_sect;
    vucc_grids      = _other.vucc_grids;
    my_vucc_grids   = _other.my_vucc_grids;

    pota_ref        = _other.pota_ref;
    my_pota_ref     = _other.my_pota_ref;
    clublog_status  = _other.clublog_status;
    hrdlog_status   = _other.hrdlog_status;
    QRZCom_status   = _other.QRZCom_status;
    hamlogeu_status = _other.hamlogeu_status;
    hamqth_status   = _other.hamqth_status;

    eqsl_qsl_sent   = _other.eqsl_qsl_sent;
    eqsl_qsl_rcvd   = _other.eqsl_qsl_sent;
    comment         = _other.comment;
    address         = _other.address;
    ant_path        = _other.ant_path;
    arrl_sect       = _other.arrl_sect;
    continent       = _other.continent;
    rig             = _other.rig;
    country         = _other.country;
    award_granted   = _other.award_granted;
    award_submitted = _other.award_submitted;
    county          = _other.county;
    contacted_op    = _other.contacted_op;
    contacted_owner = _other.contacted_owner;
    contest_id      = _other.contest_id;
    credit_granted  = _other.credit_granted;
    credit_submitted    = _other.credit_submitted;
    darc_dok        = _other.darc_dok;
    email           = _other.email;
    qso_complete    = _other.qso_complete;
    usaca_counties  = _other.usaca_counties;
    ve_prov         = _other.ve_prov;
    web             = _other.web;
    wwff_ref        = _other.wwff_ref;
    iota            = _other.iota;
    ownerCall       = _other.ownerCall;
    latitude        = _other.latitude;
    longitude       = _other.longitude;
    ms_shower       = _other.ms_shower;
    notes           = _other.notes;
    prefix          = _other.prefix;
    precedence      = _other.precedence;
    public_key      = _other.public_key;
    qslmsg          = _other.qslmsg;
    region          = _other.region;
    sig             = _other.sig;
    sig_info        = _other.sig_info;
    skcc            = _other.skcc;
    srx_string      = _other.srx_string;
    stx_string      = _other.stx_string;
    state           = _other.state;
    submode         = _other.submode;
    my_city         = _other.my_city;
    my_county       = _other.my_county;
    my_country      = _other.my_country ;
    my_iota         = _other.my_iota ;
    my_latitude     = _other.my_latitude;
    my_longitude    = _other.my_longitude;
    my_name         = _other.my_name;
    my_postal_code  = _other.my_postal_code;;
    my_sig          = _other.my_sig;
    my_sig_info     = _other.my_sig_info;
    my_state        = _other.my_state;
    my_street       = _other.my_street;
    my_usaca_counties   = _other.my_usaca_counties;
    my_wwff_ref     = _other.my_wwff_ref;

    qso_time_off    = _other.qso_time_off;
    qso_dateTime    = _other.qso_dateTime;

    QSLRDate        = _other.QSLRDate;
    QSLSDate        = _other.QSLSDate;
    QSLLoTWRDate    = _other.QSLLoTWRDate;
    QSLLoTWSDate    = _other.QSLLoTWSDate;
    qso_date_off    = _other.qso_date_off;
    eQSLRDate       = _other.eQSLRDate;
    eQSLSDate       = _other.eQSLSDate;
    clublogQSOUpdateDate    = _other.clublogQSOUpdateDate;
    hrdlogUploadDate    = _other.hrdlogUploadDate;
    hamlogeuUpdateDate  = _other.hamlogeuUpdateDate;
    hamqthUpdateDate    = _other.hamqthUpdateDate;

    QRZComDate      = _other.QRZComDate;
    forceInit       = _other.forceInit;
    qso_random      = _other.qso_random;
    swl             = _other.swl;

    // The following variables are not part of ADIF

    backup          = _other.backup;
    lotwUpdating    = _other.lotwUpdating;
    realTime        = _other.realTime;
    manualMode      = _other.manualMode;
    silent_key      = _other.silent_key;
    keepComment     = _other.keepComment;
    keepOther       = _other.keepOther;
    keepMyData      = _other.keepMyData;
    keepSat         = _other.keepSat;
    modifying       = _other.modifying;
    isValidDistance = _other.isValidDistance;

    haveBand        = _other.haveBand;
    haveMode        = _other.haveMode;
    haveSubMode     = _other.haveSubMode;
    haveDateTime    = _other.haveDateTime;
    haveCall        = _other.haveCall;
}

bool QSO::copy(const QSO& other)
{   //Copies the data of another QSO into this one
   //qDebug() << Q_FUNC_INFO;
    clear();
    setLogId(other.logId);
    setStationCallsign(other.stationCallsign);

    // VARIABLES for ADIF //////////
    setAddress(other.address);
    setAge(other.age);
    setAltitude(other.altitude);
    setA_Index(other.a_index);
    setAnt_az(other.ant_az);
    setAnt_el(other.ant_el);
    setAnt_Path(other.ant_path);
    setARRL_Sect(other.arrl_sect);
    setAwardSubmitted(other.award_submitted);
    setAwardGranted(other.award_granted);
    setBand(other.band);
    setBandRX(other.band_rx);
    setCall(other.callsign);
    setCheck(other.check);
    setClass(other.clase);
    setClubLogDate(other.clublogQSOUpdateDate);
    setClubLogStatus(other.clublog_status);

    setCounty(other.county);
    setComment(other.comment);
    setContinent(other.continent);
    setContactedOperator(other.contacted_op);
    setContestID(other.contest_id);
    setCountry(other.country);
    setCQZone(other.cqz);
    setCreditGranted(other.credit_granted);
    setCreditSubmitted(other.credit_submitted);

    setDarcDok(other.darc_dok);
    setDistance(other.distance);
    setDXCC(other.dxcc);
    setEmail(other.email);
    setOwnerCallsign(other.ownerCall);
    setContactedOperator(other.contacted_op);
    setEQSLQSLRDate(other.eQSLRDate);
    //qDebug() << Q_FUNC_INFO;
    setEQSLQSLSDate(other.eQSLSDate);

    setEQSLQSL_RCVD(other.eqsl_qsl_rcvd);
    setEQSLQSL_SENT(other.eqsl_qsl_sent);
    setFists(other.fists);
    setFistsCC(other.fists_cc);
    setForceInit(other.forceInit);
    setFreq(other.freq_tx);
    setFreqRX(other.freq_rx);
    setGridSquare(other.gridsquare);
    setGridSquare_ext(other.gridsquare_ext);
    setOperatorCallsign(other.operatorCall);

    setHRDUpdateDate(other.hrdlogUploadDate);
    setHRDLogStatus(other.hrdlog_status);
    setHamLogEUUpdateDate(other.hamlogeuUpdateDate);
    setHamLogEUStatus(other.hrdlog_status);
    setHamQTHUpdateDate(other.hamqthUpdateDate);
    setHamQTHStatus(other.hamqth_status);

    setIOTA(other.iota);
    setIotaID(other.iota_ID);
    setItuZone(other.itu_zone);
    setK_Index(other.k_index);
    setLatitude(other.latitude);
    setLongitude(other.longitude);
    setLoTWQSLRDate(other.QSLLoTWRDate);
    setLoTWQSLSDate(other.QSLLoTWSDate);
    setLoTWQSL_RCVD(other.lotw_qsl_rcvd);
    setLoTWQSL_SENT(other.lotw_qsl_sent);

    setMaxBursts(other.max_bursts);
    //qDebug() <<Q_FUNC_INFO << " - 010 " ;
    setMode(other.mode);
    //qDebug() <<Q_FUNC_INFO << " - 011" ;
    //qDebug() <<Q_FUNC_INFO << ": " << getMode();
    //qDebug() <<Q_FUNC_INFO << " - 012 " ;
    setMsShower(other.ms_shower);
    setMyAltitude(other.my_altitude);
    setMyAntenna(other.my_antenna);
    setMyARRL_Sect(other.my_arrl_sect);
    setMyCity(other.my_city);
    setMyCounty(other.my_county);
    setMyCountry(other.my_country);
    setMyCQZone(other.my_cqz);

    setMyDXCC(other.my_dxcc);
    setMyFists(other.my_fists);
    setMyGridSquare(other.my_gridsquare);
    setMyGridSquare_ext(other.my_gridsquare_ext);
    setMyIOTA(other.my_iota);
    setMyIotaID(other.my_iota_ID);
    setMyITUZone(other.my_itu_zone);
    setMyLatitude(other.my_latitude);
    setMyLongitude(other.my_longitude);
    setMyName(other.my_name);
    setMyPOTA_Ref(other.my_pota_ref);
    setMyPostalCode(other.my_postal_code);

    setMyRig(other.my_rig);
    setMySig(other.my_sig);
    setMySigInfo(other.my_sig_info);
    setMySOTA_REF(other.my_sota_ref);
    setMyState(other.my_state);
    setMyStreet(other.my_street);
    setMyUsacaCounties(other.my_usaca_counties);
    setMyVUCCGrids(other.my_vucc_grids);
    setMyWWFF_Ref(other.my_wwff_ref);

    setName(other.name);
    setNotes(other.notes);
    setNrBursts(other.nr_bursts);
    setNrPings(other.nr_pings);
    setOperatorCallsign(other.operatorCall);
    setOwnerCallsign(other.ownerCall);
    setEQ_Call(other.contacted_owner);
    setPrefix(other.prefix);
    setPOTA_Ref(other.pota_ref);
    setPrecedence(other.precedence);
    setPropMode(other.propMode);
    setPublicKey(other.public_key);
    setQRZCOMDate(other.QRZComDate);
    setQRZCOMStatus(other.QRZCom_status);
    setQSLMsg(other.qslmsg);
    setQSLRDate(other.QSLRDate);
    setQSLSDate(other.QSLSDate);
    setQSL_RCVD(other.qsl_rcvd);
    setQSL_SENT(other.qsl_sent);

    setQSLSenVia(other.qslSenVia);
    setQSLRecVia(other.qslRecVia);
    setQSLVia(other.qslVia);
    setQSOComplete(other.qso_complete);
    setDateTimeOn(other.qso_dateTime);
    setDateOff(other.qso_date_off);
    setQSORandom(other.qso_random);
    setQTH(other.qth);
    setRegion(other.region);
    setRig(other.rig);
    setRSTRX(other.RST_rx);
    setRSTTX(other.RST_tx);
    setRXPwr(other.pwr_rx);
    setSatMode(other.satMode);
    setSatName(other.satName);

    setSFI(other.sfi);
    setSIG(other.sig);
    setSIG_INFO(other.sig_info);
    setSilentKey(other.silent_key);
    setSkcc(other.skcc);
    setSOTA_REF(other.sota_ref);
    setSrx(other.srx);
    setSrxString(other.srx_string);

    setState(other.state);
    setStx(other.stx);
    setStxString(other.stx_string);
    setSubmode(other.submode);
    setSwl(other.swl);
    setTenTen(other.ten_ten);
    setTimeOff(other.qso_time_off);

    setTXPwr(other.pwr_tx);
    setUksmg(other.uksmg);
    setUsacaCounties(other.usaca_counties);
    setVeProv(other.ve_prov);
    setVUCCGrids(other.vucc_grids);
    setWeb(other.web);
    setWWFF_Ref(other.wwff_ref);
    return true;
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

bool QSO::isComplete() const
{
    if (!haveCall)
    {
        return false;
    }

    if (!haveDateTime)
    {
        return false;
    }

    if (!haveBand)
        return false;
    return (haveMode || haveSubMode);

    //return (haveBand && (haveMode || haveSubMode) && haveDateTime && haveCall);
}

void QSO::clear()
{   // When we clear a QSO, we put data that is not valid for a QSO, if possible.
    // so no data that has not been saved by the user is "populated automatically" by KLog
    // without the user's knowledge or intention
   //qDebug() << Q_FUNC_INFO << " - " << startT.msec();
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
    age = -1;
    altitude = -10000;
    a_index = -1;
    ant_az = -91.0;
    ant_el = -91.0;
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
    distance = -1;
    dxcc = 0;
    email = QString();
    ownerCall = QString();
    contacted_owner = QString();
    eQSLRDate = QDate();
    eQSLSDate = QDate();
    eqsl_qsl_rcvd = QString();
    eqsl_qsl_sent = QString();
    fists = -1;
    fists_cc = -1;
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
    iota_ID = -1;
    itu_zone = 0;
    k_index = -1;
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
    my_fists = -1;
    my_gridsquare = QString();
    my_gridsquare_ext = QString();
    my_iota = QString();
    my_iota_ID = -1;
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
    sfi = -1;
    sig = QString();
    sig_info = QString();
    silent_key = false;
    skcc = QString();
    sota_ref = QString();
    srx = -1;
    srx_string = QString();
    state = QString();
    stx = -1;
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

bool QSO::getBackup() const
{
    return backup;
}

void QSO::setModifying(const bool _mod)
{
    modifying = _mod;
}

bool QSO::getModifying() const
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

int QSO::getQSOid() const
{
    if (qsoId>0)
        return qsoId;
    return -1;
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

int QSO::getLogId() const
{
    if (logId>0)
        return logId;
    return -1;
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
    if (freq_tx>=0)
        return freq_tx;
    return -1.0;
}

double QSO::getFreqRX()
{
    if (freq_rx>=0)
        return freq_rx;
    return -1.0;
}

bool QSO::isValid()
{// Add more controls: Call, Date, Time, Band, Mode?
    logEvent (Q_FUNC_INFO, "Start", Debug);
    return isComplete();
    //if ( (callsign.length()>0))
    //{
    //   logEvent (Q_FUNC_INFO, "END-true", Debug);
    //    return true;
    //}
    //else
    //{
    //   logEvent (Q_FUNC_INFO, "END-false", Debug);
    //    return false;
    //}

}

bool QSO::setCall(const QString &_c)
{
    logEvent (Q_FUNC_INFO, QString("Start: %1").arg(_c), Debug);
    Callsign call(_c);
   //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (call.isValid())
    {
       //qDebug() << Q_FUNC_INFO << ": valid Call";
        logEvent (Q_FUNC_INFO, QString("END - true"), Debug);
        callsign = _c;
        haveCall = true;
        return true;
    }
    else
    {
        logEvent (Q_FUNC_INFO, QString("END - false-2"), Debug);
       //qDebug() << Q_FUNC_INFO << ": NOT valid Call";
        return false;
    }
}

QString QSO::getCall() const
{
   //qDebug() << Q_FUNC_INFO << ": " << callsign;
   //qDebug() << Q_FUNC_INFO  << " - " << startT.msec();
    if (callsign.length()>2)
        return callsign;
    return QString();
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

QString QSO::getBand() const
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

QString QSO::getBandRX() const
{
    return band_rx;
}

bool QSO::setMode(const QString &_c)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    QString aux = _c;

    if (aux.isNull())
    {
        //qDebug() << Q_FUNC_INFO << ": NULL";
        mode = QString();
        haveMode = false;
        logEvent (Q_FUNC_INFO, "END - False 1", Debug);
        return false;
    }
    if (aux.length()>0)
    {
        //qDebug() << Q_FUNC_INFO << ": mode = " << _c;
        mode = aux;
        if (!haveSubMode)
            setSubmode(mode);
        logEvent (Q_FUNC_INFO, "END - True", Debug);
        haveMode = true;
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": FALSE 2";
        mode = QString();
        haveMode = false;
        logEvent (Q_FUNC_INFO, "END - False 2", Debug);
        return false;
    }
}

QString QSO::getMode() const
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

QDate QSO::getDate() const
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

QDate QSO::getDateOff() const
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

QTime QSO::getTimeOff() const
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

QTime QSO::getTimeOn() const
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

QDateTime QSO::getDateTimeOn() const
{
    return qso_dateTime;
}

void QSO::setRealTime(const bool _rt)
{
    realTime = _rt;
}

bool QSO::getRealTime() const
{
    return realTime;
}

void QSO::setManualMode(const bool _rt)
{
    manualMode = _rt;
}

bool QSO::getManualMode() const
{
    return manualMode;
}

// eQSL Tab

bool QSO::setClubLogStatus(const QString &_c)
{
   //qDebug() << Q_FUNC_INFO << " - " << _c;

    if (util->isValidUpload_Status (_c))
    {
       //qDebug() << Q_FUNC_INFO << " - Valid";
        clublog_status = _c;
        return true;
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - NOT Valid";
        return false;
    }
}

QString QSO::getClubLogStatus() const
{
    return clublog_status;
}

bool QSO::setClubLogDate(const QDate &_c)
{
   //qDebug() << Q_FUNC_INFO << " - " << _c.toString("yyyy-MM-dd");
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

QDate QSO::getClubLogDate() const
{
    return clublogQSOUpdateDate;
}

bool QSO::setQRZCOMStatus(const QString &_c)
{
   //qDebug() << Q_FUNC_INFO << " - " << _c;
    if (util->isValidUpload_Status (_c))
    {
       //qDebug() << Q_FUNC_INFO << " - VALID";
        QRZCom_status = _c;
        return true;
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - NOT VALID";
      return false;
    }
}

QString QSO::getQRZCOMStatus() const
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

QDate QSO::getQRZCOMDate() const
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

QString QSO::getEQSLQSL_RCVD() const
{
    return eqsl_qsl_rcvd;
}

bool QSO::setEQSLQSL_SENT(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << " : " << _c;
    if (util->isValidQSL_Sent(_c))
    {
        eqsl_qsl_sent = _c;
        return true;
    }
    else {
        return false;
    }
}

QString QSO::getEQSLQSL_SENT() const
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

QDate QSO::getEQSLQSLRDate() const
{
    return eQSLRDate;
}

bool QSO::setEQSLQSLSDate(const QDate &_c)
{
    //QDate dat = _c;
    //qDebug() << Q_FUNC_INFO << " - " << dat.toString("yyyy-MM-dd");
    if (_c.isValid())
    {
        eQSLSDate = _c;

        //qDebug() << Q_FUNC_INFO << " -  eQSLSDate: " << eQSLSDate.toString("yyyy-MM-dd");
        return true;
    }
    else
    {
        eQSLSDate = QDate();
        return false;
    }
}

QDate QSO::getEQSLQSLSDate() const
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

QString QSO::getLoTWQSL_SENT() const
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

QDate QSO::getLoTWQSLSDate() const
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

QString QSO::getLoTWQSL_RCVD() const
{
    return lotw_qsl_rcvd;
}

bool QSO::setLoTWQSLRDate(const QDate &_c)
{
    //QDate dat = _c;
   //qDebug() << Q_FUNC_INFO << " - " << dat.toString("yyyy-MM-dd");
    //qDebug() << Q_FUNC_INFO << ": " << _c.toString("yyyy-MM-dd");
    if (_c.isValid())
    {
       //qDebug() << Q_FUNC_INFO << ":  TRUE";
        QSLLoTWRDate = _c;
       //qDebug() << Q_FUNC_INFO << " - " << QSLLo.toString("yyyy-MM-dd");
        return true;
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << ": FALSE";
        QSLLoTWRDate = QDate();
        return false;
    }
}

QDate QSO::getLoTWQSLRDate() const
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

QString QSO::getQSL_SENT() const
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

QDate QSO::getQSLSDate() const
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

QString QSO::getQSL_RCVD() const
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

QString QSO::getQSLSentVia() const
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

QString QSO::getQSLRecVia() const
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

QString QSO::getQSLVia() const
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

QDate QSO::getQSLRDate() const
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

QString QSO::getQSLMsg() const
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
        RST_rx =  util->getDefaultRST(getMode());
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
        RST_tx =  util->getDefaultRST(getMode());
        return false;
    }
}

QString QSO::getRSTTX() const
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

double QSO::getRXPwr() const
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

QString QSO::getComment() const
{
    return comment;
}

bool QSO::setKeepComment(bool _k)
{
    keepComment = _k;
    return true;
}

bool QSO::getKeepComment() const
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

int QSO::getDXCC() const
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

QString QSO::getPropMode() const
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

QString QSO::getSOTA_REF() const
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

QString QSO::getMySOTA_REF() const
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

QString QSO::getMyVUCCGrids() const
{
    return my_vucc_grids;
}

QString QSO::getMyRig() const
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

QString QSO::getMyAntenna() const
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

QString QSO::getMyARRL_Sect() const
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
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if ((0 >= _c) && (_c <= 120))
    {
        age = _c;
        return true;
    }
    age = -1;
    return false;
}

double QSO::getAge() const
{
    return age;
}

bool QSO::setAltitude(const double _c)
{
    if (_c>=0)
    {
        altitude = _c;
        return true;
    }
    altitude = 0.0;
    return false;

}

double QSO::getAltitude() const
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
        pwr_tx = 0.0;
        logEvent (Q_FUNC_INFO, "END - False", Debug);
        return false;
    }
}

double QSO::getTXPwr() const
{
    return pwr_tx;
}

bool QSO::setOperatorCallsign(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << "Start: " << _c;
    //logEvent(Q_FUNC_INFO, "Start", Debug);
    Callsign call(_c);
    if (call.isValid())
    {
        operatorCall = _c;
        //qDebug() << Q_FUNC_INFO << "END - true";
        logEvent(Q_FUNC_INFO, "END-true", Debug);
        return true;
    }
    else {
        //qDebug() << Q_FUNC_INFO << "End - false";
        operatorCall = QString();
        logEvent(Q_FUNC_INFO, "END-false", Debug);
        return false;
    }
}

QString QSO::getOperatorCallsign() const
{
    return operatorCall;
}

bool QSO::setStationCallsign(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << "Start: " << _c;

    Callsign call(_c);
    if (call.isValid())
    //qDebug() << Q_FUNC_INFO << " - 010";
    {
        //qDebug() << Q_FUNC_INFO << " - True";
        stationCallsign = _c;
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - False";
        stationCallsign = QString();
        return false;
    }
}

QString QSO::getStationCallsign() const
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
        my_gridsquare = QString();
        return false;
    }
}

QString QSO::getMyGridSquare() const
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
        my_gridsquare_ext = QString();
        return false;
    }
}

QString QSO::getMyGridSquare_ext() const
{
    //qDebug() << Q_FUNC_INFO << ": " << gridsquare;
    return my_gridsquare_ext;
}

bool QSO::setKeepMyData(bool _k)
{
    keepMyData = _k;
    return true;
}

bool QSO::getKeepMyData() const
{
    return keepMyData;
}

// Satellite Tab
bool QSO::setKeepSatTab(bool _k){keepSat = _k; return true;}

bool QSO::getKeepSatTab() const{return keepSat;}

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

QString QSO::getSatName() const
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

QString QSO::getSatMode() const
{
    return satMode;
}

bool QSO::setAddress(const QString &_c)
{
    if (_c.length()>0)
    {
        address = _c;
        return true;
    }
    address = QString();
    return false;
}

QString QSO::getAddress() const
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
    a_index = 0;
    return false;
}

int QSO::getA_Index() const
{
    return a_index;
}


bool QSO::setAnt_az(const double _c)
{ //TODO: Adjust number: http://www.adif.org/312/ADIF_312.htm#QSO_Field_ANT_AZ
    ant_az = fmod(_c, 360.0);
    if (ant_az>0.0)
        return true;
    ant_az = 0.0;
    return false;
}
double QSO::getAnt_az() const
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
    ant_el = 0.0;
    return false;
}

double QSO::getAnt_el() const
{
    return ant_el;
}

bool QSO::setAnt_Path(const QString &_c)
{
    if (!adif->isValidAntPath (_c))
    {
        ant_path = QString();
        return false;
    }

    ant_path = _c;
    return true;
}

QString QSO::getAnt_Path() const
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

QString QSO::getARRL_Sect() const
{
    return arrl_sect;
}

bool QSO::setCheck(const QString &_c)
{
    check = _c;
    return true;
}

QString QSO::getCheck() const
{
    return check;
}

bool QSO::setClass(const QString &_c)
{
    clase = _c;
    return true;
}

QString QSO::getClass() const
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

QString QSO::getContinent() const
{
    return continent;
}

bool QSO::setDistance(const double _i)
{
    if (adif->isValidDistance(QString::number(_i)))
    {
        distance = _i;
        return true;
    }
    distance = 0.0;
    return false;
}

double QSO::getDistance() const
{
    return distance;
}

bool QSO::setOwnerCallsign(const QString &_c)
{
    Callsign call(_c);
    if (call.isValid())
    {
        ownerCall = _c;
        return true;
    }
    else {
        return false;
    }
}

QString QSO::getOwnerCallsign() const
{
    return ownerCall;
}

bool QSO::setEQ_Call(const QString &_c)
{
    Callsign call(_c);
    if (call.isValid())
    {
        contacted_owner = _c;
        return true;
    }
    else {
        return false;
    }
}

QString QSO::getEQ_Call() const
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

QDate QSO::getHRDUpdateDate() const
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

QString QSO::getHRDLogStatus() const
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

QString QSO::getHamLogEUStatus() const
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

QDate QSO::getHamLogEUUpdateDate() const
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

QString QSO::getHamQTHStatus() const
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

QDate QSO::getHamQTHUpdateDate() const
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

int QSO::getK_Index() const
{
    return k_index;
}

bool QSO::setRig(const QString &_c)
{
    rig = _c;
    return true;
}

QString QSO::getRig() const
{
    return rig;
}

bool QSO::setCountry(const QString &_c)
{
    country = _c;
    return true;
}

QString QSO::getCountry() const
{
    return country;
}

bool QSO::setAwardGranted(const QString &_c)
{
    award_granted = _c;
    return true;
}

QString QSO::getAwardGranted() const
{
    return award_granted;
}

bool QSO::setAwardSubmitted(const QString &_c)
{
    award_submitted = _c;
    return true;
}

QString QSO::getAwardSubmitted() const
{
    return award_submitted;
}

bool QSO::setCounty(const QString &_c)
{
    county = _c;
    return true;
}

QString QSO::getCounty() const
{
    return county;
}

bool QSO::setContactedOperator(const QString &_c)
{
    Callsign call(_c);
    if (call.isValid())
    {
        contacted_op = _c;
        return true;
    }
    else {
        return false;
    }
}

QString QSO::getContactedOperator() const
{
    return contacted_op;
}

bool QSO::setContestID(const QString &_c)
{
    contest_id = _c;
    return true;
}

QString QSO::getContestID() const
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
        cqz = 0;
        return false;
    }
}

int QSO::getCQZone() const
{
    return cqz;
}

bool QSO::setCreditGranted(const QString &_c)
{
    credit_granted = _c;
    return true;
}

QString QSO::getCreditGranted() const
{
    return credit_granted;
}

bool QSO::setCreditSubmitted(const QString &_c)
{
    credit_submitted = _c;
    return true;
}

QString QSO::getCreditSubmitted() const
{
    return credit_submitted;
}

bool QSO::setDarcDok(const QString &_c)
{
    darc_dok = _c;
    return true;
}

QString QSO::getDarcDok() const
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

QString QSO::getEmail() const
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

int QSO::getFists() const
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

int QSO::getFistsCC() const
{
    return fists_cc;
}

bool QSO::setForceInit(bool _k)
{
    forceInit = _k;
    return true;
}

bool QSO::getForceInit() const
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

int QSO::getIotaID() const
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
        itu_zone = 0;
        return false;
    }
}

int QSO::getItuZone() const
{
    return itu_zone;
}

bool QSO::setLatitude(const QString &_c)
{
    latitude = _c;
    return true;
}

QString QSO::getLatitude() const
{
    return latitude;
}

bool QSO::setLongitude(const QString &_c)
{
    longitude = _c;
    return true;
}

QString QSO::getLongitude() const
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

int QSO::getNrBursts() const
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

int QSO::getMaxBursts() const
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

int QSO::getNrPings() const
{
    return nr_pings;
}

bool QSO::setMsShower(const QString &_c)
{
    ms_shower = _c;
    return true;
}

QString QSO::getMsShower() const
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

QString QSO::getQSOComplete() const
{
    return qso_complete;
}

bool QSO::setQSORandom(bool _k)
{
    qso_random = _k;
    return true;
}

bool QSO::getQSORandom() const
{
    return qso_random;
}

bool QSO::setMyAltitude(const double _c)
{
    my_altitude = _c;
    return true;
}

double QSO::getMyAltitude() const
{
    return my_altitude;
}

bool QSO::setMyCity(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << " - " << _c;
    my_city = _c;
    return true;
}

QString QSO::getMyCity() const
{
    return my_city;
}

bool QSO::setMyCounty(const QString &_c)
{
    my_county = _c;
    return true;
}

QString QSO::getMyCounty() const
{
    return my_county;
}

bool QSO::setMyCountry(const QString &_c)
{
    my_country = _c;
    return true;
}

QString QSO::getMyCountry() const
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
        my_cqz = 0;
        return false;
    }
}

int QSO::getMyCQZone() const
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

int QSO::getMyDXCC() const
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

int QSO::getMyFists() const
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

QString QSO::getMyIOTA() const
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

int QSO::getMyIotaID() const
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
        my_itu_zone = 0;
        return false;
    }
}

int QSO::getMyITUZone() const
{
    return my_itu_zone;
}

bool QSO::setMyLatitude(const QString &_c)
{
    my_latitude = _c;
    return true;
}

QString QSO::getMyLatitude() const
{
    return my_latitude;
}

bool QSO::setMyLongitude(const QString &_c)
{
    my_longitude = _c;
    return true;
}

QString QSO::getMyLongitude() const
{
    return my_longitude;
}

bool QSO::setMyName(const QString &_c)
{
    my_name = _c;
    return true;
}

QString QSO::getMyName() const
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

QString QSO::getMyPOTA_Ref() const
{
    return my_pota_ref;
}

bool QSO::setMyPostalCode(const QString &_c)
{
    my_postal_code = _c;
    return true;
}

QString QSO::getMyPostalCode() const
{
    return my_postal_code;
}

bool QSO::setMySig(const QString &_c)
{
    my_sig = _c;
    return true;
}

QString QSO::getMySig() const
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

QString QSO::getMyState() const
{
    return my_state;
}

bool QSO::setMyStreet(const QString &_c)
{
    my_street = _c;
    return true;
}

QString QSO::getMyStreet() const
{
    return my_street;
}

bool QSO::setMyUsacaCounties(const QString &_c)
{
    my_usaca_counties = _c;
    return true;
}

QString QSO::getMyUsacaCounties() const
{
    return my_usaca_counties;
}

bool QSO::setNotes(const QString &_c)
{
    notes = _c;
    return true;
}

QString QSO::getNotes() const
{
    return notes;
}

bool QSO::setPrefix(const QString &_c)
{
    prefix = _c;
    return true;
}

QString QSO::getPrefix() const
{
    return prefix;
}

bool QSO::setPrecedence(const QString &_c)
{
    precedence = _c;
    return true;
}

QString QSO::getPrecedence() const
{
    return precedence;
}

bool QSO::setPublicKey(const QString &_c)
{
    public_key = _c;
    return true;
}

QString QSO::getPublicKey() const
{
    return public_key;
}

bool QSO::setRegion(const QString &_c)
{
    region = _c;
    return true;
}

QString QSO::getRegion() const
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

int QSO::getTenTen() const
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

int QSO::getSFI() const
{
    return sfi;
}

bool QSO::setSIG(const QString &_c)
{
    sig = _c;
    return true;
}

QString QSO::getSIG() const
{
    return sig;
}

bool QSO::setSIG_INFO(const QString &_c)
{
    sig_info = _c;
    return true;
}

QString QSO::getSIG_INFO() const
{
    return sig_info;
}

bool QSO::setSilentKey(bool _k)
{
    silent_key = _k;
    return true;
}

bool QSO::getSilentKey() const
{
    return silent_key;
}

bool QSO::setSkcc(const QString &_c)
{
    skcc = _c;
    return true;
}

QString QSO::getSkcc() const
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

int QSO::getSrx() const
{
    return srx;
}

bool QSO::setSrxString(const QString &_c)
{
    srx_string = _c;
    return true;
}

QString QSO::getSrxString() const
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

int QSO::getStx() const
{
    return stx;
}

bool QSO::setStxString(const QString &_c)
{
    stx_string = _c;
    return true;
}

QString QSO::getStxString() const
{
    return stx_string;
}

bool QSO::setState(const QString &_c)
{
    state = _c;
    return true;
}

QString QSO::getState() const
{
    //qDebug() << Q_FUNC_INFO << ": " << state;
    return state;
}

bool QSO::setSubmode(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (_c.length()>0)
    {
        //qDebug() << Q_FUNC_INFO << ": submode: " << _c;
        submode = _c;
        haveSubMode = true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": NULL";
        submode = QString();
        haveSubMode = false;
    }


    //if (requestMode){
    //    emit getModeSignal(submode);
    //}
    return true;
}

QString QSO::getSubmode() const
{
    return submode;
}

bool QSO::setSwl(bool _k)
{
    swl = _k;
    return true;
}

bool QSO::getSwl() const
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

int QSO::getUksmg() const
{
    return uksmg;
}

bool QSO::setUsacaCounties(const QString &_c)
{
    usaca_counties = _c;
    return true;
}

QString QSO::getUsacaCounties() const
{
    return usaca_counties;
}

bool QSO::setVeProv(const QString &_c)
{
    ve_prov = _c;
    return true;
}

QString QSO::getVeProv() const
{
    return ve_prov;
}

bool QSO::setWeb(const QString &_c)
{
    web = _c;
    return true;
}

QString QSO::getWeb() const
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

QString QSO::getWWFF_Ref() const
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

QString QSO::getMyWWFF_Ref() const
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
    //setLoTWQSL_RCVD("Y");
    return setLoTWQSLRDate(util->getDateFromLoTWQSLDateString(data));
}
bool QSO::setLoTWQSLSDate1(const QString& data) {
    //setLoTWQSL_SENT("Y");
    return setLoTWQSLSDate(util->getDateFromLoTWQSLDateString(data));
}
bool QSO::setLoTWQSLSDate2(const QString& data) {
    //setLoTWQSL_SENT("Y");
    return setLoTWQSLSDate(util->getDateFromLoTWQSLDateString(data));
}

QHash<QString, decltype(std::mem_fn(&QSO::decltype_function))> QSO::SetDataHash;

void QSO::InitializeHash() {
    //qDebug() << Q_FUNC_INFO;
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
bool QSO::setData(const QString &_adifPair, bool _lotw)
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

    if (_lotw)
    {
        if (field == "QSL_RCVD")
            field = "LOTW_QSL_RCVD";
    }


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
    //qDebug() << Q_FUNC_INFO << " - Start: qsoId: " << QString::number(_qsoId);
    if (!isComplete ())
    {
        //qDebug() << Q_FUNC_INFO << " - QSO NOT COMPLETE";
        return -1;
    }

    //qDebug() << Q_FUNC_INFO << "Mode: " << getMode();
    //qDebug() << Q_FUNC_INFO << "Submode: " << getSubmode();
    //qDebug() << Q_FUNC_INFO << " - QSO Complete... adding";
    QString queryString;
    queryString.clear();
    if (_qsoId<=0)
    {
        //qDebug() << Q_FUNC_INFO << " - qsoID <=0";
        queryString = getAddQueryString();
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - qsoID>0";
        queryString = getModifyQueryString();
    }
    //qDebug() << Q_FUNC_INFO << " Query: " << queryString;;

    QSqlQuery query = getPreparedQuery(queryString);
    //qDebug() << Q_FUNC_INFO << " qsoId: " << QString::number(_qsoId);
    if (_qsoId>0)
    {
        //qDebug() << Q_FUNC_INFO << " - binding ID";
        query.bindValue (":id", _qsoId);
    }
    //qDebug() << Q_FUNC_INFO << " - executing query";
    if (query.exec())
    {
        //qDebug() << Q_FUNC_INFO << QString(": QSO ADDED/Modified: %1 - %2").arg(callsign).arg(getDateTimeOn().toString("yyyyMMdd-hhmm"));
        //qDebug() << Q_FUNC_INFO << ": QSO ADDED/Modified: " << query.lastQuery ();
        if (_qsoId>0)
            return _qsoId;

        return getLastInsertedQSO();
    }
    else
    {

            //qDebug() << Q_FUNC_INFO << QString(": QSO NOT ADDED/Modified: %1 - %2").arg(callsign).arg(_qsoId);
            //qDebug() << Q_FUNC_INFO << ": QSO NOT ADDED/Modified: " << query.lastQuery ();
            //qDebug() << Q_FUNC_INFO << ": Error: databaseText: " << query.lastError().databaseText();
            //qDebug() << Q_FUNC_INFO << ": Error: text: " << query.lastError().text();
            //qDebug() << Q_FUNC_INFO << ": Error: driverText: " << query.lastError().driverText();
            //qDebug() << Q_FUNC_INFO << ": Error: nativeErrorCode: " << query.lastError().nativeErrorCode();
        //void MainWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString queryFailed)
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return -2;
    }
    query.finish();
    //qDebug() << Q_FUNC_INFO << " - END";
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
    bool ok = query.prepare ("SELECT band.id FROM band WHERE band.name= :bandname");
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return -1;
    }
    return -1;
}

QString QSO::getBandNameFromBandId(int bandId)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(bandId);
    QSqlQuery query;
    //qDebug() << Q_FUNC_INFO << "Band: " << getBand();
    bool ok = query.prepare ("SELECT name FROM band WHERE id= :id");
    if (!ok)
    {
        return QString();
        //qDebug() << Q_FUNC_INFO << " - Query NOT prepared-3167";
    }
    query.bindValue (":id", bandId);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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
    bool ok = query.prepare ("SELECT mode.id FROM mode WHERE mode.submode= :submode");
    if (!ok)
    {
        //qDebug() << Q_FUNC_INFO << " - Failed to prepare";
        return -1;
    }
    //qDebug() << Q_FUNC_INFO << " - Binding mode" << getMode();
    query.bindValue (":submode", getSubmode());

    //query.bindValue(":idQSO", _qsoId);
    //if (haveSubMode)
    //{
    //    query.bindValue (":submode", getSubmode ());
    //}
    //else if (haveMode)
    //{
    //    query.bindValue (":submode", getMode ());
    //}
    //else
    //{
    //    return -1;
    //}
    //EA4K: falla al crear un qso que llega desde wsjtx... devuelve 5
    if (query.exec ())
    {
        if (query.next())
        {
            if (query.isValid ())
            {
               //qDebug() << Q_FUNC_INFO << ": " << query.value (0).toString();
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return -2;
    }
    return -3;
}

QString QSO::getModeNameFromModeId(int _modeId, bool _submode)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_modeId);
    QSqlQuery query;
    bool ok;
    if (_submode)
    {
        ok = query.prepare ("SELECT submode FROM mode WHERE id= :id");
    }
    else
    {
        ok = query.prepare ("SELECT name FROM mode WHERE id= :id");
    }

    if (!ok)
    {
        //qDebug() << Q_FUNC_INFO << " - Query NOT prepared-3255";
        return QString();
    }
    query.bindValue (":id", _modeId);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return QString();
    }

    if (!query.next())
        return QString();
    if (!query.isValid())
        return QString();

    //qDebug() << Q_FUNC_INFO << ": " << (query.value(0)).toString();
    return (query.value(0)).toString();
}

QSqlQuery QSO::getPreparedQuery(const QString &_s)
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
    query.bindValue(":qso_date", util->getDateTimeSQLiteStringFromDateTime (getDateTimeOn ()));
    query.bindValue(":call", getCall());
    query.bindValue(":rst_sent", getRSTTX());
    query.bindValue(":rst_rcvd", getRSTRX());
    query.bindValue(":bandid", getBandIdFromBandName (false));
    query.bindValue(":modeid", getModeIdFromModeName ());
    query.bindValue(":cqz", getCQZone());
    query.bindValue(":ituz", getItuZone());
    query.bindValue(":dxcc", getDXCC());
    query.bindValue(":address", getAddress());
    if (adif->isValidAge(QString::number(getAge())))
        query.bindValue(":age", getAge());
    if (adif->isValidAltitude(QString::number(getAltitude())))
        query.bindValue(":altitude", getAltitude());
    if (adif->isValidA_Index(QString::number(getA_Index())))
        query.bindValue(":a_index", getA_Index());
    if (adif->isValidAnt_AZ(QString::number(getAnt_az())))
        query.bindValue(":ant_az", getAnt_az());
    if (adif->isValidAnt_EL(QString::number(getAnt_el())))
        query.bindValue(":ant_el", getAnt_el());

    query.bindValue(":ant_path", getAnt_Path());
    query.bindValue(":arrl_sect", getARRL_Sect());
    query.bindValue(":award_submitted", getAwardSubmitted ());
    query.bindValue(":award_granted", getAwardGranted ());
    query.bindValue(":band_rx", getBandIdFromBandName(true));
    query.bindValue(":checkcontest", getCheck());
    query.bindValue(":class", getClass());

    query.bindValue(":cnty", getCounty());
    query.bindValue(":comment", getComment());
    query.bindValue(":cont", getContinent ());
    query.bindValue(":contacted_op", getContactedOperator());
    query.bindValue(":contest_id", getContestID());
    query.bindValue(":country", getCountry());
    query.bindValue(":credit_submitted", getCreditSubmitted());
    query.bindValue(":credit_granted,", getCreditGranted());
    query.bindValue(":darc_dok", getDarcDok ());
    if (adif->isValidDistance(QString::number(getDistance())))
        query.bindValue(":distance", getDistance());
    query.bindValue(":email", getEmail());
    query.bindValue(":eq_call", getEQ_Call());


    if (adif->isValidFISTS(QString::number(getFists())))
        query.bindValue(":fists", getFists ());
    if (adif->isValidFISTS(QString::number(getFistsCC())))
        query.bindValue(":fists_cc", getFistsCC ());

    query.bindValue(":force_init", util->boolToCharToSQLite (getForceInit()));
    query.bindValue(":freq_tx", getFreqTX());
    query.bindValue(":freq_rx", getFreqRX());
    query.bindValue(":gridsquare", getGridSquare());
    query.bindValue(":gridsquare_ext", getGridSquare_ext());

    query.bindValue(":iota", getIOTA());
    if (adif->isValidIOTA_islandID(QString::number(getIotaID())))
        query.bindValue(":iota_island_id", getIotaID());
    if (adif->isValidK_Index(QString::number(getK_Index())))
        query.bindValue(":k_index", getK_Index());
    query.bindValue(":lat", getLatitude());
    query.bindValue(":lon", getLongitude());

    query.bindValue(":clublog_qso_upload_date", util->getDateSQLiteStringFromDate(getClubLogDate()));
    query.bindValue(":clublog_qso_upload_status", getClubLogStatus());

    query.bindValue(":hrdlog_qso_upload_date", getHRDUpdateDate ());
    query.bindValue(":hrdlog_qso_upload_status", getHRDLogStatus ());

    query.bindValue(":hamlogeu_qso_upload_date", getHamLogEUUpdateDate());
    query.bindValue(":hamlogeu_qso_upload_status", getHamLogEUStatus());
    query.bindValue(":hamqth_qso_upload_date", getHamQTHUpdateDate());
    query.bindValue(":hamqth_qso_upload_status", getHamQTHStatus());


    query.bindValue(":eqsl_qsl_rcvd", getEQSLQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(getEQSLQSL_RCVD()))
        query.bindValue(":eqsl_qslrdate", util->getDateSQLiteStringFromDate(getEQSLQSLRDate()));

    qDebug() << Q_FUNC_INFO << "- eqsl_qsl_sent: " << getEQSLQSL_SENT();
    query.bindValue(":eqsl_qsl_sent", getEQSLQSL_SENT());

    if (qsl_sent_dates_set.contains(getEQSLQSL_SENT()))
    {
        qDebug() << Q_FUNC_INFO << "- Saving date " << getEQSLQSL_SENT();
        query.bindValue(":eqsl_qslsdate", util->getDateSQLiteStringFromDate(getEQSLQSLSDate()));
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "- NOT Saving date " << getEQSLQSL_SENT();
    }


    query.bindValue(":qsl_rcvd", getQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(getQSL_RCVD()))
        query.bindValue(":qslrdate", util->getDateSQLiteStringFromDate(getQSLRDate()));

    query.bindValue(":qsl_sent", getQSL_SENT());
    if (qsl_sent_dates_set.contains(getQSL_SENT()))
        query.bindValue(":qslsdate", util->getDateSQLiteStringFromDate(getQSLSDate()));

    query.bindValue(":lotw_qsl_rcvd", getLoTWQSL_RCVD());
    if (qsl_rcvd_dates_set.contains(getLoTWQSL_RCVD()))
        query.bindValue(":lotw_qslrdate", util->getDateSQLiteStringFromDate(getLoTWQSLRDate()));

    query.bindValue(":lotw_qsl_sent", getLoTWQSL_SENT());
    if (qsl_sent_dates_set.contains(getLoTWQSL_SENT()))
        query.bindValue(":lotw_qslsdate", util->getDateSQLiteStringFromDate(getLoTWQSLSDate()));


    query.bindValue(":qrzcom_qso_upload_date", util->getDateSQLiteStringFromDate(getQRZCOMDate ()));
    query.bindValue(":qrzcom_qso_upload_status", getQRZCOMStatus ());

    if (adif->isValidNRBursts(QString::number(getMaxBursts())))
        query.bindValue(":max_bursts", getMaxBursts());
    query.bindValue(":ms_shower", getMsShower());
    if (adif->isValidAltitude(QString::number(getMyAltitude())))
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
    if (adif->isValidIOTA_islandID(QString::number(getMyIotaID())))
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
    if (adif->isValidNRBursts(QString::number(getNrBursts())))
        query.bindValue(":nr_bursts", getNrBursts());
    if (adif->isValidPings(QString::number(getNrPings())))
        query.bindValue(":nr_pings", getNrPings());
    query.bindValue(":operator", getOperatorCallsign());
    query.bindValue(":owner_callsign", getOwnerCallsign());
    query.bindValue(":pfx", getPrefix());

    query.bindValue(":pota_ref", getPOTA_Ref());
    query.bindValue(":precedence", getPrecedence());
    query.bindValue(":prop_mode", getPropMode());
    query.bindValue(":public_key", getPublicKey());


    query.bindValue(":qslmsg", getQSLMsg());


    query.bindValue(":qsl_rcvd_via", getQSLRecVia());
    query.bindValue(":qsl_sent_via", getQSLSentVia());
    query.bindValue(":qsl_via", getQSLVia());
    query.bindValue(":qso_complete", getQSOComplete());
    query.bindValue(":qso_random", util->boolToCharToSQLite (getQSORandom()));
    query.bindValue(":qth", getQTH());
    query.bindValue(":region", getRegion ());
    query.bindValue(":rig", getRig ());
    if (adif->isValidPower(QString::number(getRXPwr())))
        query.bindValue(":rx_pwr", getRXPwr());
    query.bindValue(":sat_mode", getSatMode());
    query.bindValue(":sat_name",getSatName());
    if (adif->isValidSFI(QString::number(getSFI())))
        query.bindValue(":sfi", getSFI());
    query.bindValue(":sig", getSIG());
    query.bindValue(":sig_info", getSIG_INFO ());
    query.bindValue(":silent_key", util->boolToCharToSQLite (getSilentKey ()));
    query.bindValue(":skcc", getSkcc ());

    query.bindValue(":sota_ref", getSOTA_REF());
    query.bindValue(":srx_string", getSrxString());
    if (adif->isValidSRX(QString::number(getSrx())))
        query.bindValue(":srx", getSrx());
    query.bindValue(":stx_string", getStxString());
    if (adif->isValidSTX(QString::number(getStx())))
        query.bindValue(":stx", getStx());
    query.bindValue(":state", getState());
    query.bindValue(":station_callsign", getStationCallsign());
    //query.bindValue(":submode", getModeIdFromModeName ());

    query.bindValue(":swl", util->boolToCharToSQLite (getSwl()));
    if (adif->isValidUKSMG(QString::number(getUksmg())))
        query.bindValue(":uksmg", getUksmg ());
    query.bindValue(":usaca_counties", getUsacaCounties ());
    query.bindValue(":ve_prov", getVeProv ());
    query.bindValue(":wwff_ref", getWWFF_Ref());
    query.bindValue(":vucc_grids", getVUCCGrids());
    if (adif->isValidTenTen(QString::number(getTenTen())))
        query.bindValue(":ten_ten", getTenTen());
    if (adif->isValidPower(QString::number(getTXPwr())))
        query.bindValue(":tx_pwr", getTXPwr());
    query.bindValue(":web", getWeb());
    query.bindValue(":qso_date_off", util->getDateSQLiteStringFromDate(getDateOff()));
    query.bindValue(":lognumber", getLogId());

    //QVariantList list = query.boundValues();
    //for (int i = 0; i < list.size(); ++i)
    //qDebug() << Q_FUNC_INFO << QString(": %1").arg(i) << "/ " << list.at(i).toString().toUtf8().data() << "\n";

    //qDebug() << Q_FUNC_INFO << " - END";
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
    if (adif->isValidAltitude(QString::number(getAltitude())))
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

    adifStr.append(adif->getADIFField ("dark_dok", darc_dok ));
    if (distance>0)
        adifStr.append(adif->getADIFField ("distance", QString::number(distance) ));
    adifStr.append(adif->getADIFField ("email",  email));
    adifStr.append(adif->getADIFField ("eq_call",  getEQ_Call()));

    adifStr.append(adif->getADIFField ("eqsl_qsl_rcvd", eqsl_qsl_rcvd));
    if ((eQSLRDate.isValid()) && (adif->isValidQSLRCVD(eqsl_qsl_rcvd)))
        adifStr.append(adif->getADIFField ("eqsl_qslrdate", util->getADIFDateFromQDate(eQSLRDate) ));

    adifStr.append(adif->getADIFField ("eqsl_qsl_sent", eqsl_qsl_sent));
    if ((eQSLSDate.isValid()) && (adif->isValidQSLSENT(eqsl_qsl_sent)))
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

    adifStr.append(adif->getADIFField ("lotw_qsl_sent", lotw_qsl_sent));
    if ((QSLLoTWSDate.isValid()) && ( adif->isValidQSLSENT(lotw_qsl_sent) ))
        adifStr.append(adif->getADIFField ("lotw_qslsdate", util->getADIFDateFromQDate(QSLLoTWSDate) ));

    adifStr.append(adif->getADIFField ("lotw_qsl_rcvd", lotw_qsl_rcvd));
    if ((QSLLoTWRDate.isValid()) && ( adif->isValidQSLRCVD(lotw_qsl_rcvd) ))
        adifStr.append(adif->getADIFField ("lotw_qslrdate", util->getADIFDateFromQDate(QSLLoTWRDate) ));


    if (adif->isValidNRBursts(QString::number(getMaxBursts())))
        adifStr.append(adif->getADIFField ("max_bursts", QString::number(getMaxBursts()) ));

    adifStr.append(adif->getADIFField ("ms_shower",  ms_shower));
    if (adif->isValidAltitude(QString::number(getMyAltitude())))
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

    if ((QSLRDate.isValid()) && ( adif->isValidQSLRCVD(qsl_rcvd)))
        adifStr.append(adif->getADIFField ("qslrdate", util->getADIFDateFromQDate(QSLRDate) ));

    adifStr.append(adif->getADIFField ("qsl_sent", getQSL_SENT()));
    if ((QSLSDate.isValid()) && ( adif->isValidQSLSENT(qsl_sent)))
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
    if (adif->isValidSRX(QString::number(srx)))
        adifStr.append(adif->getADIFField ("srx", QString::number(srx)));
    adifStr.append(adif->getADIFField ("stx_string", stx_string));
    if (adif->isValidSTX(QString::number(stx)))
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
    return adifStr + "<EOR>\n";
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
}

bool QSO::fromDB(int _qsoId)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << " - Start";
    QString queryString = "SELECT * FROM log WHERE id= :idQSO";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":idQSO", _qsoId);

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return false;
    }
    if (!query.next())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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
    setClubLogDate(util->getDateFromSQliteString(data));
    setClubLogStatus((query.value(rec.indexOf("clublog_qso_upload_status"))).toString());
    setComment((query.value(rec.indexOf("comment"))).toString());
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
    setEQSLQSLRDate(util->getDateFromSQliteString(data));
    //qDebug() << Q_FUNC_INFO;
    data = (query.value(rec.indexOf("eqsl_qslsdate"))).toString();
    //qDebug() << Q_FUNC_INFO << "  - 49: " << data;

    setEQSLQSLSDate(util->getDateFromSQliteString(data));
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
    setHRDUpdateDate(util->getDateFromSQliteString(data));

    //qDebug() << Q_FUNC_INFO << "  - 60";
    setHRDLogStatus((query.value(rec.indexOf("hrdlog_qso_upload_status"))).toString());

    data = (query.value(rec.indexOf("hamlogeu_qso_upload_date"))).toString();
    setHamLogEUUpdateDate(util->getDateFromSQliteString(data));
    setHamLogEUStatus((query.value(rec.indexOf("hamlogeu_qso_upload_status"))).toString());

    data = (query.value(rec.indexOf("hamqth_qso_upload_date"))).toString();
    setHamQTHUpdateDate(util->getDateFromSQliteString(data));
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
   //qDebug() << Q_FUNC_INFO << " - lotq_qslrdate - DB_ " << data;
    setLoTWQSLRDate(util->getDateFromSQliteString(data));

    data = (query.value(rec.indexOf("lotw_qslsdate"))).toString();
    setLoTWQSLSDate(util->getDateFromSQliteString(data));

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
    setQRZCOMDate(util->getDateFromSQliteString(data));
    setQRZCOMStatus((query.value(rec.indexOf("qrzcom_qso_upload_status"))).toString());
    //qDebug() << Q_FUNC_INFO << "  - 100";
    setQSLMsg((query.value(rec.indexOf("qslmsg"))).toString());
    data = (query.value(rec.indexOf("qslrdate"))).toString();
    setQSLRDate(util->getDateFromSQliteString(data));
    data = (query.value(rec.indexOf("qslsdate"))).toString();
    setQSLSDate(util->getDateFromSQliteString(data));

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
    setRXPwr((query.value(rec.indexOf("rx_pwr"))).toDouble());
    //qDebug() << Q_FUNC_INFO << "  - 140";
    setWeb((query.value(rec.indexOf("web"))).toString());
    data = (query.value(rec.indexOf("qso_date_off"))).toString();
    setDateOff(util->getDateFromSQliteString(data));
    setLogId((query.value(rec.indexOf("lognumber"))).toInt());
    //qDebug() << Q_FUNC_INFO << "  - 150";
    logEvent (Q_FUNC_INFO, "END", Debug);
    return false;
}

int QSO::getLastInsertedQSO()
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
        //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return id;
    //qDebug() << Q_FUNC_INFO << " - END";
}

