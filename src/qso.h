#ifndef QSO_H
#define QSO_H
/***************************************************************************
                          qso.h  -  description
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

#include <QString>
#include <QSqlQuery>
#include <QDate>
#include <QTime>
#include <QObject>
//#include <QDebug>
#include "utilities.h"
#include "klogdefinitions.h"
#include "database.h"
//#include <functional>

class QSO : public QObject
{
    Q_OBJECT
    friend class tst_QSO;

public:
    QSO();
    ~QSO();
    void setLogLevel (const DebugLogLevel _b);
    void setRealTime(const bool _rt);   // Not QSO info but KLog status
    bool getRealTime();
    void setManualMode(const bool _rt); // Not QSO info but KLog status
    bool getManualMode();

    void setBackup(const bool _rt);
    bool getBackup();

    void setModifying(const bool _mod);
    bool getModifying();

    bool setData(const QString &_adifPair);
    void clear();
    bool isValid();
    void setLoTWUpdating(bool _lotw);

    bool setCall(const QString &_c);
    QString getCall();
    bool setBand(const QString &_c);
    QString getBand();
    bool setMode(const QString &_c);
    QString getMode();

    bool setDateTimeOn(const QDateTime &_c);
    QDateTime getDateTimeOn();
    bool setDate(const QDate &_c);
    QDate getDate();
    bool setTimeOn(const QTime &_c);
    QTime getTimeOn();

    bool setQSOid(const int _i);
    int getQSOid();

    bool setLogId(const int _i);
    int getLogId();


    bool setBandRX(const QString &_c);
    QString getBandRX();


    bool setRSTTX(const QString &_c);
    QString getRSTTX();
    bool setRSTRX(const QString &_c);
    QString getRSTRX();
    double setFreqTX(const double _f);
    bool setFreqRX(const double _f);
    double getFreqTX();
    double getFreqRX();
    bool setQTH(const QString &_c);
    QString getQTH();
    bool setGridSquare(const QString &_c);
    QString getGridSquare();
    bool setName(const QString &_c);
    QString getName();
    bool setRXPwr(const double _f);
    double getRXPwr();

    // QSL TAB
    bool setQSL_SENT(const QString &_c);
    QString getQSL_SENT();
    bool setQSLSDate(const QDate &_c);
    QDate getQSLSDate();
    bool setQSLSenVia(const QString &_qs);
    QString getQSLSentVia();

    bool setQSL_RCVD(const QString &_c);
    QString getQSL_RCVD();
    bool setQSLRDate(const QDate &_c);
    QDate getQSLRDate();
    bool setQSLRecVia(const QString &_qs);
    QString getQSLRecVia();

    bool setQSLVia(const QString &_qs);
    QString getQSLVia();
    bool setQSLMsg(const QString &_qs);
    QString getQSLMsg();


    // eQSL tab
    bool setClubLogStatus(const QString &_c);
    QString getClubLogStatus();
    bool setClubLogDate(const QDate &_c);
    QDate getClubLogDate();

    bool setEQSLQSL_RCVD(const QString &_c);
    QString getEQSLQSL_RCVD();
    bool setEQSLQSL_SENT(const QString &_c);
    QString getEQSLQSL_SENT();
    bool setEQSLQSLRDate(const QDate &_c);
    QDate getEQSLQSLRDate();
    bool setEQSLQSLSDate(const QDate &_c);
    QDate getEQSLQSLSDate();

    bool setLoTWQSL_RCVD(const QString &_c);
    QString getLoTWQSL_RCVD();
    bool setLoTWQSL_SENT(const QString &_c);
    QString getLoTWQSL_SENT();
    bool setLoTWQSLRDate(const QDate &_c);
    QDate getLoTWQSLRDate();
    bool setLoTWQSLSDate(const QDate &_c);
    QDate getLoTWQSLSDate();

    bool setQRZCOMStatus(const QString &_c);
    QString getQRZCOMStatus();
    bool setQRZCOMDate(const QDate &_c);
    QDate getQRZCOMDate();

    void setDefaultEQSLSentServices(const bool _send);

    // Comment Tab
    bool setComment(const QString &_c);
    QString getComment();
    bool setKeepComment(bool _k);
    bool getKeepComment();

    // Others Tab
    bool setDXCC(const int _i);
    int getDXCC();
    bool setPropMode(const QString &_c);
    QString getPropMode();
    bool setSOTA_REF(const QString &_c);
    QString getSOTA_REF();
    bool setAge(const double _c);
    double getAge();
    bool setIOTA(const QString &_c);
    QString getIOTA();
    bool setKeepOthers(bool _k);
    bool getKeepOthers();
    bool setVUCCGrids(const QString &_c);
    QString getVUCCGrids();


    // My data Tab
    bool setTXPwr(double _f);
    double getTXPwr();
    bool setOperatorCallsign(const QString &_c);
    QString getOperatorCallsign();
    bool setStationCallsign(const QString &_c);
    QString getStationCallsign();
    bool setMyGridSquare(const QString &_c);
    QString getMyGridSquare();
    bool setKeepMyData(bool _k);
    bool getKeepMyData();
    bool setMySOTA_REF(const QString &_c);
    QString getMySOTA_REF();
    bool setMyRig(const QString &_c);
    QString getMyRig();
    bool setMyAntenna(const QString &_c);
    QString getMyAntenna();
    bool setMyArrlSect(const QString &_c);
    QString getMyArrlSect();
    bool setMyVUCCGrids(const QString &_c);
    QString getMyVUCCGrids();
    // Satellite Tab
    bool setSatName(const QString &_c);
    QString getSatName();
    bool setSatMode(const QString &_c);
    QString getSatMode();
    bool setKeepSatTab(bool _k);
    bool getKeepSatTab();

    bool setAddress(const QString &_c);
    QString getAddress();
    bool setA_Index(const int _i);
    int getA_Index();
    bool setAnt_az(const double _c);
    double getAnt_az();
    bool setAnt_el(const double _c);
    double getAnt_el();
    bool setAnt_Path(const QString &_c);
    QString getAnt_Path();
    bool setARRL_Sect(const QString &_c);
    QString getARRL_Sect();
    bool setCheck(const QString &_c);
    QString getCheck();
    bool setClass(const QString &_c);
    QString getClass();
    bool setClublogQSOUpdateDate(const QDate &_c);
    QDate getClublogQSOUpdateDate();
    bool setContinent(const QString &_c);
    QString getContinent();
    bool setDistance(const double _i);
    double getDistance();
    bool setOwnerCallsign(const QString &_c);
    QString getOwnerCallsign();
    bool setEQ_Call(const QString &_c); // Contacted station owner
    QString getEQ_Call();               // Contacted station owner
    bool setHRDUpdateDate(const QDate &_c);
    QDate getHRDUpdateDate();
    bool setHRDLogStatus(const QString &_c);
    QString getHRDLogStatus();
    bool setK_Index(const int _i);
    int getK_Index();
    bool setDateOff(const QDate &_c);
    QDate getDateOff();
    bool setTimeOff(const QTime &_c);
    QTime getTimeOff();
    bool setRig(const QString &_c);
    QString getRig();
    bool setCountry(const QString &_c);
    QString getCountry();
    bool setAwardGranted(const QString &_c);
    QString getAwardGranted();
    bool setAwardSubmitted(const QString &_c);
    QString getAwardSubmitted();
    bool setCounty(const QString &_c);
    QString getCounty();
    bool setContactedOperator(const QString &_c);
    QString getContactedOperator();
    bool setContestID(const QString &_c);
    QString getContestID();
    bool setCQZone(const int _i);
    int getCQZone();
    bool setCreditGranted(const QString &_c);
    QString getCreditGranted();
    bool setCreditSubmitted(const QString &_c);
    QString getCreditSubmitted();
    bool setDarcDok(const QString &_c);
    QString getDarcDok();
    bool setEmail(const QString &_c);
    QString getEmail();
    bool setFists(const int _i);
    int getFists();
    bool setFistsCC(const int _i);
    int getFistsCC();
    bool setForceInit(bool _k);
    bool getForceInit();
    bool setIotaID(const int _i);
    int getIotaID();
    bool setItuZone(const int _i);
    int getItuZone();
    bool setLatitude(const QString &_c);
    QString getLatitude();
    bool setLongitude(const QString &_c);
    QString getLongitude();
    bool setQSOComplete(const QString &_c);
    QString getQSOComplete();
    bool setNrBursts(const int _i);
    int getNrBursts();
    bool setMaxBursts(const int _i);
    int getMaxBursts();
    bool setNrPings(const int _i);
    int getNrPings();
    bool setMsShower(const QString &_c);
    QString getMsShower();
    bool setQSORandom(bool _k);
    bool getQSORandom();
    bool setMyCity(const QString &_c);
    QString getMyCity();
    bool setMyCounty(const QString &_c);
    QString getMyCounty();
    bool setMyCountry(const QString &_c);
    QString getMyCountry();
    bool setMyCQZone(const int _i);
    int getMyCQZone();
    bool setMyDXCC(const int _i);
    int getMyDXCC();
    bool setMyFists(const QString &_c);
    QString getMyFists();
    bool setMyIOTA(const QString &_c);
    QString getMyIOTA();
    bool setMyIotaID(const int _i);
    int getMyIotaID();
    bool setMyITUZone(const int _i);
    int getMyITUZone();
    bool setMyLatitude(const QString &_c);
    QString getMyLatitude();
    bool setMyLongitude(const QString &_c);
    QString getMyLongitude();
    bool setMyName(const QString &_c);
    QString getMyName();
    bool setMyPostalCode(const QString &_c);
    QString getMyPostalCode();
    bool setMySig(const QString &_c);
    QString getMySig();
    bool setMySigInfo(const QString &_c);
    QString getMySigInfo();
    bool setMyState(const QString &_c);
    QString getMyState();
    bool setMyStreet(const QString &_c);
    QString getMyStreet();
    bool setMyUsacaCounties(const QString &_c);
    QString getMyUsacaCounties();
    bool setNotes(const QString &_c);
    QString getNotes();
    bool setPrefix(const QString &_c);
    QString getPrefix();
    bool setPrecedence(const QString &_c);
    QString getPrecedence();
    bool setPublicKey(const QString &_c);
    QString getPublicKey();
    bool setRegion(const QString &_c);
    QString getRegion();
    bool setTenTen(const int _i);
    int getTenTen();
    bool setSFI(const int _i);
    int getSFI();
    bool setSig(const QString &_c);
    QString getSig();
    bool setSigInfo(const QString &_c);
    QString getSigInfo();
    bool setSilentKey(bool _k);
    bool getSilentKey();
    bool setSkcc(const QString &_c);
    QString getSkcc();
    bool setSrx(const int _i);
    int getSrx();
    bool setSrxString(const QString &_c);
    QString getSrxString();
    bool setState(const QString &_c);
    QString getState();
    bool setStx(const int _i);
    int getStx();
    bool setStxString(const QString &_c);
    QString getStxString();
    //bool setSubmode(const QString &_c, bool requestMode = false);
    bool setSubmode(const QString &_c);
    QString getSubmode();
    bool setSwl(bool _k);
    bool getSwl();
    bool setUksmg(const int _i);
    int getUksmg();
    bool setUsacaCounties(const QString &_c);
    QString getUsacaCounties();
    bool setVeProv(const QString &_c);
    QString getVeProv();
    bool setWeb(const QString &_c);
    QString getWeb();
    bool setWwffRef(const QString &_c);
    QString getWwffRef();
    bool setMyWwffRef(const QString &_c);
    QString getMyWwffRef();
    bool add();
    bool modify(const int _qsoId);
    bool isComplete();


signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    void getBandSignal (double fr);
    void getModeSignal (QString submode);

private:
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);

    int qsoId, logId, dxcc, a_index, k_index, cqz, fists, fists_cc, iota_ID, itu_zone, nr_bursts, max_bursts, nr_pings, my_cqz, my_itu_zone, my_dxcc, my_iota_ID, srx, stx, uksmg;
    int ten_ten, sfi;
    double freq_tx, freq_rx, pwr_rx, pwr_tx, age, ant_el, ant_az, freq, distance;

    QString satName, satMode, callsign, stationCallsign, operatorCall, propMode, band, band_rx, mode, gridsquare, my_gridsquare, qth, name, RST_tx, RST_rx;
    QString qsl_rcvd, qsl_sent, qslSenVia, qslRecVia, qslVia, check, clase;
    QString lotw_qsl_sent, lotw_qsl_rcvd, sota_ref, my_sota_ref, my_rig, my_antenna, my_arrl_sect, vucc_grids, my_vucc_grids;
    QString clublog_status, hrdlog_status, QRZCom_status;
    QString eqsl_qsl_sent, eqsl_qsl_rcvd;
    QString comment, address, ant_path, arrl_sect, continent, rig, country, award_granted, award_submitted, county, contacted_op, contacted_owner, contest_id;
    QString credit_granted, credit_submitted,darc_dok, email, qso_complete, usaca_counties, ve_prov, web, wwff_ref;
    QString iota, ownerCall, latitude, longitude, ms_shower, notes, prefix, precedence, public_key, qslmsg, region, sig, sig_info, skcc, srx_string, stx_string, state, submode;
    QString my_city, my_county, my_country, my_fists, my_iota, my_latitude, my_longitude, my_name, my_postal_code, my_sig, my_sig_info, my_state, my_street, my_usaca_counties, my_wwff_ref;

    QTime qso_time_off;
    QDateTime qso_dateTime;

    QDate QSLRDate, QSLSDate, QSLLoTWRDate, QSLLoTWSDate, qso_date_off;
    QDate eQSLRDate, eQSLSDate, clublogDate,clublogQSOUpdateDate, hrdlogUploadDate;;
    QDate QRZComDate;

    bool backup, lotwUpdating, realTime, manualMode, silent_key;
    bool keepComment, keepOther, keepMyData, keepSat, modifying, isValidDistance, forceInit, qso_random, swl;
    bool haveBand, haveMode, haveDateTime, haveCall;

    Utilities *util;
    DebugLogLevel logLevel;

   // DataProxy_SQLite *dataProxy;
    bool decltype_function(const QString& _c); //empty function to find correct typenames for mem_fn, DO NOT RENAME
    static QHash<QString, decltype(std::mem_fn(&QSO::decltype_function))> SetDataHash;
    void InitializeHash();

    //Overloaded helper functions to accept string data for nonstring functions
    bool setAge(const QString &data);
    bool setA_Index(const QString &data);
    bool setAnt_az(const QString &data);
    bool setAnt_el(const QString &data);
    bool setCQZone(const QString &data);
    bool setDistance(const QString &data);
    bool setDXCC(const QString &data);
    bool setFists(const QString &data);
    bool setFistsCC(const QString &data);
    bool setIotaID(const QString &data);
    bool setItuZone(const QString &data);
    bool setK_Index(const QString &data);
    bool setMaxBursts(const QString &data);
    bool setMyCQZone(const QString &data);
    bool setMyDXCC(const QString &data);
    bool setMyIotaID(const QString &data);
    bool setMyITUZone(const QString &data);
    bool setNrBursts(const QString &data);
    bool setNrPings(const QString &data);
    bool setSFI(const QString &data);
    bool setSrx(const QString &data);
    bool setStx(const QString &data);
    bool setTenTen(const QString &data);
    bool setUksmg(const QString &data);

    bool setFreqTX(const QString& data);
    bool setFreqRX(const QString& data);
    bool setRXPwr(const QString& data);
    bool setTXPwr(const QString& data);

    bool setClublogQSOUpdateDate(const QString& data);
    bool setEQSLQSLRDate(const QString& data);
    bool setEQSLQSLSDate(const QString& data);
    bool setForceInit(const QString& data);
    bool setHRDUpdateDate(const QString& data);
    bool setLoTWQSLRDate(const QString& data);
    bool setLoTWQSLSDate(const QString& data);
    bool setQRZCOMDate(const QString& data);
    bool setQSLRDate(const QString& data);
    bool setQSLSDate(const QString& data);
    bool setDate(const QString& data);
    bool setDateOff(const QString& data);
    bool setQSORandom(const QString& data);
    bool setSilentKey(const QString& data);
    bool setSwl(const QString& data);
    bool setTimeOff(const QString& data);
    bool setTimeOn(const QString& data);

    bool setLoTWQSLRDate2(const QString& data);
    bool setLoTWQSLSDate1(const QString& data);
    bool setLoTWQSLSDate2(const QString& data);


};

#endif // QSO_H
