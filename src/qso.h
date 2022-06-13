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
#include <QDate>
#include <QTime>
#include <QDebug>
#include "utilities.h"

class QSO : public QObject
{
    Q_OBJECT

public:
    QSO();
    ~QSO();

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
    bool setDate(const QDate &_c);
    QDate getDate();

    bool setTimeOn(const QTime &_c);
    QTime getTimeOn();
    QDateTime getDateTimeOn();
    bool setDateTimeOn(const QDateTime &_c);

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
    QString getSentVia();

    bool setQSL_RCVD(const QString &_c);
    QString getQSL_RCVD();
    bool setQSLRDate(const QDate &_c);
    QDate getQSLRDate();
    bool setQSLRecVia(const QString &_qs);
    QString getRecVia();

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
    bool setTXPwr(const double _f);
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

    bool setClase(const QString &_c);
    QString getClase();

    bool setClublogQSOUpdateDate(const QDate &_c);
    QDate getClublogQSOUpdateDate();

    bool setContinent(const QString &_c);
    QString getContinent();
    bool setDistance(const int _i);
    int getDistance();
    bool setOwnerCallsign(const QString &_c);
    QString getOwnerCallsign();
    bool setHRDUpdateDate(const QDate &_c);
    QDate getHRDUpdateDate();
    bool setHRDLogStatus(const QString &_c);
    QString getHRDLogStatus();
    bool setFreq(const double _f);
    double getFreq();
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





    bool add();


private:

    QString satName, satMode, callsign, stationCallsign, operatorCall, propMode, band, mode, gridsquare, myGridsquare, qth, name, RST_tx, RST_rx;
    int qsoId, logId, dxcc, a_index, k_index, distance, cqz, fists, fists_cc;
    QString qsl_rcvd, qsl_sent, qslSenVia, qslRecVia, qslVia, qslMsg, check, clase;
    QDate QSLRDate, QSLSDate, QSLLoTWRDate, QSLLoTWSDate, qso_date_off;
    QTime qso_time_off;
    QDateTime qso_dateTime;
    double freq_tx, freq_rx, pwr_rx, pwr_tx, age, ant_el, ant_az, freq;
    QString lotw_qsl_sent, lotw_qsl_rcvd, sota_ref, my_sota_ref, my_rig, my_antenna, vucc_grids, my_vucc_grids;

    QString clublog_status, hrdlog_status, QRZCom_status;
    QDate clublogDate,clublogQSOUpdateDate, hrdlogUploadDate;
    QString eqsl_qsl_sent, eqsl_qsl_rcvd;
    QDate eQSLRDate, eQSLSDate;
    QDate QRZComDate;
    QString comment, address, ant_path, arrl_sect, continent, rig, country, award_granted, award_submitted, county, contacted_op, contest_id;
    QString credit_granted, credit_submitted,darc_dok, email;
    bool keepComment, keepOther, keepMyData, keepSat, modifying, isValidDistance;

    QString iota, ownerCall;

    Utilities *util;

    bool lotwUpdating, realTime, manualMode;
    bool backup;
   // DataProxy_SQLite *dataProxy;

   /*
   FORCE_INIT,
   GUEST_OP,
   IOTA_ISLAND_ID, ITUZ,
   LAT,  LON,
   MAX_BURSTS, MODE, MS_SHOWER, MY_ANTENNA, MY_CITY,  MY_CNTY,  MY_COUNTRY,  MY_CQ_ZONE,  MY_DXCC,  MY_FISTS,  MY_GRIDSQUARE,  MY_IOTA,  MY_IOTA_ISLAND_ID,  MY_ITU_ZONE,  MY_LAT,  MY_LON,  MY_NAME,  MY_POSTAL_CODE,  MY_RIG, MY_SIG,  MY_SIG_INFO,  MY_SOTA_REF,  MY_STATE,  MY_STREET,  MY_USACA_COUNTIES,  MY_VUCC_GRIDS,
   NOTES,  NR_BURSTS,  NR_PINGS,
   OPERATOR,  OWNER_CALLSIGN,
   PFX,  PRECEDENCE,  PROP_MODE,  PUBLIC_KEY,
   QSLMSG,  QSO_COMPLETE,  QSO_RANDOM,
   REGION,
   SFI,  SIG, SIG_INFO,  SILENT_KEY,  SKCC,  SOTA_REF,  SRX,  SRX_STRING,  STATE,  STATION_CALLSIGN,  STX,  STX_STRING,  SUBMODE,  SWL,
   TEN_TEN,
   UKSMG,  USACA_COUNTIES,
   VE_PROV,
   WEB
   */

};

#endif // QSO_H
