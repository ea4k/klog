#ifndef KLOG_QSO_H
#define KLOG_QSO_H
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
#include "utilities.h"
#include "klogdefinitions.h"
#include "adif.h"
#include "frequency.h"
#include "qsodatacache.h"

class QSO : public QObject
{
    Q_OBJECT
    friend class tst_QSO;

public:
    QSO(QObject *parent = nullptr);
    QSO(const QSO& other);
    ~QSO();
    void operator=(QSO const &_other);

    bool copy(const QSO& other);
    void setLogLevel (const DebugLogLevel _b);
    void setRealTime(const bool _rt);   // Not QSO info but KLog status
    bool getRealTime() const;
    void setManualMode(const bool _rt); // Not QSO info but KLog status
    bool getManualMode() const;

    void setBackup(const bool _rt);
    bool getBackup() const;

    void setModifying(const bool _mod);
    bool getModifying() const;

    bool setData(const QString &_adifPair, bool _lotw = false);

    void clear();
    bool isValid();
    void setLoTWUpdating(bool _lotw);
    bool getLoTWUpdating();

    bool setCall(const QString &_c);
    QString getCall() const;
    bool setBand(const QString &_c);
    QString getBand() const;
    bool setMode(const QString &_c);
    QString getMode() const;

    bool setDateTimeOn(const QDateTime &_c);
    QDateTime getDateTimeOn() const;
    bool setDate(const QDate &_c);
    QDate getDate() const;
    bool setTimeOn(const QTime &_c);
    QTime getTimeOn() const;

    bool setQSOid(const int _i);
    int getQSOid() const;

    bool setLogId(const int _i);
    int getLogId() const;

    bool setBandRX(const QString &_c);
    QString getBandRX() const;

    bool setRSTTX(const QString &_c);
    QString getRSTTX() const;
    bool setRSTRX(const QString &_c);
    QString getRSTRX();
    bool setFreq(const double _f);
    bool setFreqRX(const double _f);
    double getFreqTX();
    double getFreqRX();
    bool setQTH(const QString &_c);
    QString getQTH();
    bool setGridSquare(const QString &_c);
    QString getGridSquare();
    bool setGridSquare_ext(const QString &_c);
    QString getGridSquare_ext();
    bool setName(const QString &_c);
    QString getName();
    bool setRXPwr(const double _f);
    double getRXPwr() const;

    // QSL TAB
    bool setQSL_SENT(const QString &_c);
    QString getQSL_SENT() const;
    bool setQSLSDate(const QDate &_c);
    QDate getQSLSDate() const;
    bool setQSLSenVia(const QString &_qs);
    QString getQSLSentVia() const;

    bool setQSL_RCVD(const QString &_c);
    QString getQSL_RCVD() const;
    bool setQSLRDate(const QDate &_c);
    QDate getQSLRDate() const;
    bool setQSLRecVia(const QString &_qs);
    QString getQSLRecVia() const;

    bool setQSLVia(const QString &_qs);
    QString getQSLVia() const;
    bool setQSLMsg(const QString &_qs);
    QString getQSLMsg() const;

    // eQSL tab
    bool setClubLogStatus(const QString &_c);
    QString getClubLogStatus() const;

    bool setClubLogDate(const QDate &_c);
    bool setClubLogDate(const QString& data);

    QDate getClubLogDate() const;

    bool setEQSLQSL_RCVD(const QString &_c);
    QString getEQSLQSL_RCVD() const;
    bool setEQSLQSL_SENT(const QString &_c);
    QString getEQSLQSL_SENT() const;
    bool setEQSLQSLRDate(const QDate &_c);
    QDate getEQSLQSLRDate() const;
    bool setEQSLQSLSDate(const QDate &_c);
    QDate getEQSLQSLSDate() const;

    bool setLoTWQSL_RCVD(const QString &_c);
    QString getLoTWQSL_RCVD() const;
    bool setLoTWQSL_SENT(const QString &_c);
    QString getLoTWQSL_SENT() const;
    bool setLoTWQSLRDate(const QDate &_c);
    QDate getLoTWQSLRDate() const;
    bool setLoTWQSLSDate(const QDate &_c);
    QDate getLoTWQSLSDate() const;

    bool setQRZCOMStatus(const QString &_c);
    QString getQRZCOMStatus() const;
    bool setQRZCOMDate(const QDate &_c);
    QDate getQRZCOMDate() const;

    void setDefaultEQSLSentServices(const bool _send);

    // Comment Tab
    bool setComment(const QString &_c);
    QString getComment() const;
    bool setKeepComment(bool _k);
    bool getKeepComment() const;

    // Others Tab
    bool setDXCC(const int _i);
    int getDXCC() const;
    bool setPropMode(const QString &_c);
    QString getPropMode() const;
    bool setSOTA_REF(const QString &_c);
    QString getSOTA_REF() const;
    bool setAge(const double _c);
    double getAge() const;
    bool setIOTA(const QString &_c);
    QString getIOTA();
    bool setKeepOthers(bool _k);
    bool getKeepOthers();
    bool setVUCCGrids(const QString &_c);
    QString getVUCCGrids();

    bool setPOTA_Ref(const QString &_c);
    QString getPOTA_Ref();
    bool setWWFF_Ref(const QString &_c);
    QString getWWFF_Ref() const;
    bool setSIG(const QString &_c);
    QString getSIG() const;
    bool setSIG_INFO(const QString &_c);
    QString getSIG_INFO() const;

    // My data Tab
    bool setTXPwr(double _f);
    double getTXPwr() const;
    bool setOperatorCallsign(const QString &_c);
    QString getOperatorCallsign() const;
    bool setStationCallsign(const QString &_c);
    QString getStationCallsign() const;
    bool setMyGridSquare(const QString &_c);
    QString getMyGridSquare() const;
    bool setMyGridSquare_ext(const QString &_c);
    QString getMyGridSquare_ext() const;
    bool setKeepMyData(bool _k);
    bool getKeepMyData() const;

    bool setMySOTA_REF(const QString &_c);
    QString getMySOTA_REF() const;
    bool setMyRig(const QString &_c);
    QString getMyRig() const;
    bool setMyAntenna(const QString &_c);
    QString getMyAntenna() const;
    bool setMyVUCCGrids(const QString &_c);
    QString getMyVUCCGrids() const;

    bool setMyPOTA_Ref(const QString &_c);
    QString getMyPOTA_Ref() const;
    bool setMyWWFF_Ref(const QString &_c);
    QString getMyWWFF_Ref() const;



    // Satellite Tab
    bool setSatName(const QString &_c);
    QString getSatName() const;
    bool setSatMode(const QString &_c);
    QString getSatMode() const;
    bool setKeepSatTab(bool _k);
    bool getKeepSatTab() const;

    bool setAddress(const QString &_c);
    QString getAddress() const;
    bool setAltitude(const double _c);
    double getAltitude() const;
    bool setA_Index(const double _i);
    double getA_Index() const;
    bool setAnt_az(const double _c);
    double getAnt_az() const;
    bool setAnt_el(const double _c);
    double getAnt_el() const;
    bool setAnt_Path(const QString &_c);
    QString getAnt_Path() const;
    bool setARRL_Sect(const QString &_c);
    QString getARRL_Sect() const;
    bool setCheck(const QString &_c);
    QString getCheck() const;
    bool setClass(const QString &_c);
    QString getClass() const;
    bool setContinent(const QString &_c);
    QString getContinent() const;
    bool setDistance(const double _i);          // In Km
    double getDistance() const;                 // In Km
    bool setOwnerCallsign(const QString &_c);
    QString getOwnerCallsign() const;
    bool setEQ_Call(const QString &_c); // Contacted station owner
    QString getEQ_Call() const;               // Contacted station owner
    bool setHRDUpdateDate(const QDate &_c);
    QDate getHRDUpdateDate() const;
    bool setHRDLogStatus(const QString &_c);
    QString getHRDLogStatus() const;

    bool setHamLogEUStatus(const QString &_c);
    QString getHamLogEUStatus() const;
    bool setHamLogEUUpdateDate(const QDate &_c);
    QDate getHamLogEUUpdateDate() const;
    bool setHamQTHStatus(const QString &_c);
    QString getHamQTHStatus()  const;
    bool setHamQTHUpdateDate(const QDate &_c);
    QDate getHamQTHUpdateDate() const;


    bool setK_Index(const int _i);
    int getK_Index() const;

    bool setDateTimeOff(const QDateTime &_c);
    QDateTime getDateTimeOff() const;
    bool setDateOff(const QDate &_c);
    QDate getDateOff() const;
    bool setTimeOff(const QTime &_c);
    QTime getTimeOff() const;




    bool setRig(const QString &_c);
    QString getRig() const;
    bool setCountry(const QString &_c);
    QString getCountry() const;
    bool setAwardGranted(const QString &_c);
    QString getAwardGranted() const;
    bool setAwardSubmitted(const QString &_c);
    QString getAwardSubmitted() const;
    bool setCounty(const QString &_c);
    QString getCounty() const;
    bool setContactedOperator(const QString &_c);
    QString getContactedOperator() const;
    bool setContestID(const QString &_c);
    QString getContestID() const;
    bool setCQZone(const int _i);
    int getCQZone() const;
    bool setCreditGranted(const QString &_c);
    QString getCreditGranted() const;
    bool setCreditSubmitted(const QString &_c);
    QString getCreditSubmitted() const;
    bool setDarcDok(const QString &_c);
    QString getDarcDok() const;
    bool setEmail(const QString &_c);
    QString getEmail() const;
    bool setFists(const int _i);
    int getFists() const;
    bool setFistsCC(const int _i);
    int getFistsCC() const;
    bool setForceInit(bool _k);
    bool getForceInit() const;
    bool setIotaID(const int _i);
    int getIotaID() const;
    bool setItuZone(const int _i);
    int getItuZone() const;
    bool setLatitude(const QString &_c);
    QString getLatitude() const;
    bool setLongitude(const QString &_c);
    QString getLongitude() const;
    bool setQSOComplete(const QString &_c);         // Receives valid ADIF data (Y/N/NIL/?)
    QString getQSOComplete() const;
    bool setNrBursts(const int _i);
    int getNrBursts() const;
    bool setMaxBursts(const int _i);
    int getMaxBursts() const;
    bool setNrPings(const int _i);
    int getNrPings() const;
    bool setMsShower(const QString &_c);
    QString getMsShower() const;
    bool setQSORandom(bool _k);
    bool getQSORandom() const;
    bool setMyAltitude(const double _c);
    double getMyAltitude() const;
    bool setMyARRL_Sect(const QString &_c);
    QString getMyARRL_Sect() const;
    bool setMyCity(const QString &_c);
    QString getMyCity() const;
    bool setMyCounty(const QString &_c);
    QString getMyCounty() const;
    bool setMyCountry(const QString &_c);
    QString getMyCountry() const;
    bool setMyCQZone(const int _i);
    int getMyCQZone() const;
    bool setMyDXCC(const int _i);
    int getMyDXCC() const;
    bool setMyFists(const int _c);
    int getMyFists() const;
    bool setMyIOTA(const QString &_c);
    QString getMyIOTA() const;
    bool setMyIotaID(const int _i);
    int getMyIotaID() const;
    bool setMyITUZone(const int _i);
    int getMyITUZone() const;
    bool setMyLatitude(const QString &_c);
    QString getMyLatitude() const;
    bool setMyLongitude(const QString &_c);
    QString getMyLongitude() const;
    bool setMyName(const QString &_c);
    QString getMyName() const;
    bool setMyPostalCode(const QString &_c);
    QString getMyPostalCode() const;
    bool setMySig(const QString &_c);
    QString getMySig() const;
    bool setMySigInfo(const QString &_c);
    QString getMySigInfo();
    bool setMyState(const QString &_c);
    QString getMyState() const;
    bool setMyStreet(const QString &_c);
    QString getMyStreet() const;
    bool setMyUsacaCounties(const QString &_c);
    QString getMyUsacaCounties() const;
    bool setNotes(const QString &_c);
    QString getNotes() const;
    bool setPrefix(const QString &_c);          // ADIF - PFX   - WPX Prefix
    QString getPrefix() const;
    bool setPrecedence(const QString &_c);
    QString getPrecedence() const;
    bool setPublicKey(const QString &_c);
    QString getPublicKey() const;
    bool setRegion(const QString &_c);
    QString getRegion() const;
    bool setTenTen(const int _i);
    int getTenTen() const;
    bool setSFI(const int _i);
    int getSFI() const;

    bool setSilentKey(bool _k);
    bool getSilentKey() const;
    bool setSkcc(const QString &_c);
    QString getSkcc() const;
    bool setSrx(const int _i);
    int getSrx() const;
    bool setSrxString(const QString &_c);
    QString getSrxString() const;
    bool setState(const QString &_c);       // ADIF - STATE - Primary subdivision
    QString getState() const;
    bool setStx(const int _i);
    int getStx() const;
    bool setStxString(const QString &_c);
    QString getStxString() const;
    bool setSubmode(const QString &_c);
    QString getSubmode() const;
    bool setSwl(bool _k);
    bool getSwl() const;
    bool setUksmg(const int _i);
    int getUksmg() const;
    bool setUsacaCounties(const QString &_c);
    QString getUsacaCounties() const;
    bool setVeProv(const QString &_c);
    QString getVeProv() const;
    bool setWeb(const QString &_c);
    QString getWeb() const;



    int toDB(int _qsoId = 0);
    bool fromDB(int _qsoId);
    //bool add();
    //bool modify(const int _qsoId);
    bool isComplete() const;
    QString getADIF(ExportMode _em = ModeADIF);
    QString getBandNameFromFreq(const double _n); // Should be push out of this class
    bool updateFromLoTW(const int _qsoId);        // It checks if the QSO is already in the log and updates just the LoTW fields, if needed.
    void printQSO();        //TODO:  Complete: Prints all QSO fields - DEBUG ONLY

signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    void  getModeSignal (QString submode);   // Request the mode to be filled for a given submode
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution

private:
    //QTime startT;
    QString getADIFStandard();
    QString getADIFLoTW();
    QString getADIFClubLog();
    QString getADIFeQSL();


    bool isValidCall() const;
    bool isValidDateTime() const;

    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    QString getAddQueryString();
    QString getModifyQueryString();
    QSqlQuery getPreparedQuery(const QString &_s);
    int getBandIdFromBandName(bool _rxBand=false);   // if rxBand = true, it will chec the bandRX
    QString getBandNameFromBandId(int bandId);
    int getModeIdFromModeName();                // It really returns submode
    QString getModeNameFromModeId(int _modeId, bool _submode=true);
    void setBandFromFreq(const double _fr, bool TX = true);
    //enum QSOStatus {unknown, ATNO, needed, worked, confirmed, dupe};
    //DataBase *db;
    void cleanMode(); // Cleans mode & submode
    // Functions to update a QSO after downloading LoTW

    int findIdFromQSO(const QString &_qrz, const QDateTime &_datetime, const int _band, const int _mode);


    int qsoId, logId, dxcc, k_index, cqz, fists, fists_cc, my_fists, iota_ID, itu_zone, nr_bursts, max_bursts, nr_pings, my_cqz, my_itu_zone, my_dxcc, my_iota_ID, srx, stx, uksmg;
    int ten_ten, sfi;
    double pwr_rx, pwr_tx, age, ant_el, ant_az, distance, altitude, my_altitude, a_index;
    Frequency freq_tx, freq_rx;

    QString satName, satMode, callsign, stationCallsign, operatorCall, propMode, band, band_rx, mode, gridsquare, my_gridsquare, gridsquare_ext, my_gridsquare_ext, qth, name, RST_tx, RST_rx;
    QString qsl_rcvd, qsl_sent, qslSenVia, qslRecVia, qslVia, check, clase;
    QString lotw_qsl_sent, lotw_qsl_rcvd, sota_ref, my_sota_ref, my_rig, my_antenna, my_arrl_sect, vucc_grids, my_vucc_grids;
    QString pota_ref,  my_pota_ref;
    QString clublog_status, hrdlog_status, QRZCom_status;
    QString hamlogeu_status, hamqth_status;

    QString eqsl_qsl_sent, eqsl_qsl_rcvd;
    QString comment, address, ant_path, arrl_sect, continent, rig, country, award_granted, award_submitted, county, contacted_op, contacted_owner, contest_id;
    QString credit_granted, credit_submitted,darc_dok, email, qso_complete, usaca_counties, ve_prov, web, wwff_ref;
    QString iota, ownerCall, latitude, longitude, ms_shower, notes, prefix, precedence, public_key, qslmsg, region, sig, sig_info, skcc, srx_string, stx_string, state, submode;
    QString my_city, my_county, my_country, my_iota, my_latitude, my_longitude, my_name, my_postal_code, my_sig, my_sig_info, my_state, my_street, my_usaca_counties, my_wwff_ref;

    //QTime qso_time_off;
    QDateTime qso_dateTime;
    QDateTime qso_dateTime_off;

    QDate QSLRDate, QSLSDate, QSLLoTWRDate, QSLLoTWSDate;//, qso_date_off;
    QDate eQSLRDate, eQSLSDate, clublogQSOUpdateDate, hrdlogUploadDate;
    QDate hamlogeuUpdateDate, hamqthUpdateDate;

    QDate QRZComDate;

    bool backup, lotwUpdating, realTime, manualMode, silent_key;
    bool keepComment, keepOther, keepMyData, keepSat, modifying, isValidDistance, forceInit, qso_random, swl;
    bool haveBand, haveMode, haveSubMode, haveDateTime, haveCall;

    QSODataCache *qdata;
    //Utilities *util;
    //Adif *adif;
    DebugLogLevel logLevel;


   // DataProxy_SQLite *dataProxy;
    bool decltype_function(const QString& _c); //empty function to find correct typenames for mem_fn, DO NOT RENAME
    static QHash<QString, decltype(std::mem_fn(&QSO::decltype_function))> SetDataHash;
    void InitializeHash();

    //Overloaded helper functions to accept string data for nonstring functions
    bool setAge(const QString &data);
    bool setAltitude(const QString &data);
    bool setA_Index(const QString &data);
    bool setAnt_az(const QString &data);
    bool setAnt_el(const QString &data);
    bool setCQZone(const QString &data);
    bool setDistance(const QString &data);      // In Km
    bool setDXCC(const QString &data);
    bool setFists(const QString &data);
    bool setFistsCC(const QString &data);
    bool setMyFists(const QString &data);
    bool setIotaID(const QString &data);
    bool setItuZone(const QString &data);
    bool setK_Index(const QString &data);
    bool setMaxBursts(const QString &data);
    bool setMyAltitude(const QString &data);
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

    bool setFreq(const QString& data);
    bool setFreqRX(const QString& data);
    bool setRXPwr(const QString& data);
    bool setTXPwr(const QString& data);

    bool setEQSLQSLRDate(const QString& data);
    bool setEQSLQSLSDate(const QString& data);
    bool setForceInit(const QString& data);
    bool setHRDUpdateDate(const QString& data);
    bool setHamLogEUUpdateDate(const QString& data);
    bool setHamQTHUpdateDate(const QString& data);
    bool setLoTWQSLRDate(const QString& data);
    bool setLoTWQSLSDate(const QString& data);
    //bool setLoTWRXQSO(const QString& string);     // Receives the string date/time in LOTW format (APP_LoTW_RXQSO)
    bool setLoTWRXQSO(const QString& data);
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

    void clearQSLDateIfNeeded();            // Prevents a date when not ADIF compatible

    //KLOG_DEPRECATED bool setLoTWQSLRDate2(const QString& data);
    //KLOG_DEPRECATED bool setLoTWQSLSDate1(const QString& data);
    //KLOG_DEPRECATED bool setLoTWQSLSDate2(const QString& data);

    int getLastInsertedQSO();   // just a query to get the latest inserted QSO
};

#endif // QSO_H
