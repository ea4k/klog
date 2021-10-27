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
#include "adifforfield.h"
#include "utilities.h"

class QSO : public QObject
{
    Q_OBJECT

public:
    QSO();
    ~QSO();

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

    void setRealTime(const bool _rt);
    bool getRealTime();

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
    double setAge(const double _c);
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

    bool setCQz(const int _i);
    int getCQz();
    bool setMyCQz(const int _i);
    int getMyCQz();

    bool setITUz(const int _i);
    int getITUz();
    bool setMyITUz(const int _i);
    int getMyITUz();

    bool setAddress(const QString &_c);
    QString getAddress();

    bool setContestId(const QString &_c);
    QString getContestId();
    bool setPfx(const QString &_c);
    QString getPfx();
    bool setCont(const QString &_c);
    QString getCont();
    bool setSRx(const int _c);
    int getSRx();
    bool setSTx(const int _c);
    int getSTx();
    bool setSRx_string(const QString &_c);
    QString getSRx_string();
    bool setSTx_string(const QString &_c);
    QString getSTx_string();
    bool setPoints(const int _c);
    int getPoints();
    bool setPrecedence(const QString &_c);
    QString getPrecedence();

    QString getADIF();


private:

    QString contestId, pfx, continent, srx_string, stx_string, precedence;
    int srx, stx, points;


    QString satName, satMode, callsign, stationCallsign, operatorCall, propMode, band, mode, gridsquare, myGridsquare, qth, name, RST_tx, RST_rx;
    int qsoId, logId, dxcc;
    int cqz, myCQz, ituz, myITUz;
    QString qsl_rcvd, qsl_sent, qslSenVia, qslRecVia, qslVia, qslMsg;
    QDate QSLRDate, QSLSDate, QSLLoTWRDate, QSLLoTWSDate;
    QDateTime qso_dateTime;
    double freq_tx, freq_rx, pwr_rx, pwr_tx, age;
    QString lotw_qsl_sent, lotw_qsl_rcvd, sota_ref, my_sota_ref, my_rig, my_antenna, vucc_grids, my_vucc_grids;

    QString clublog_status;
    QDate clublogDate;
    QString eqsl_qsl_sent, eqsl_qsl_rcvd;
    QDate eQSLRDate, eQSLSDate;
    QString QRZCom_status;
    QDate QRZComDate;
    QString comment, address;
    bool keepComment, keepOther, keepMyData, keepSat, modifying;


    QString iota;

    Utilities *util;

    bool lotwUpdating, realTime;
    bool backup;
   // DataProxy_SQLite *dataProxy;


   /*
   ADDRESS,  ADDRESS_INTL,  AGE,  A_INDEX,  ANT_AZ,  ANT_EL,  ANT_PATH,  ARRL_SECT,  AWARD_GRANTED,  AWARD_SUBMITTED,
   BAND,  BAND_RX,
   CALL,  CHECK,  CLASS,  CLUBLOG_QSO_UPLOAD_DATE,  CLUBLOG_QSO_UPLOAD_STATUS,  CNTY,  COMMENT,  COMMENT_INTL,  CONT,  CONTACTED_OP,  CONTEST_ID,  COUNTRY,  COUNTRY_INTL,  CQZ,  CREDIT_SUBMITTED,  CREDIT_GRANTED,
   DARC_DOK,  DISTANCE,  DXCC,
   EMAIL,  EQ_CALL,  EQSL_QSLRDATE,  EQSL_QSLSDATE,  EQSL_QSL_RCVD,  EQSL_QSL_SENT,
   FISTS,  FISTS_CC,  FORCE_INIT,  FREQ,  FREQ_RX,
   GRIDSQUARE,  GUEST_OP,
   HRDLOG_QSO_UPLOAD_DATE,  HRDLOG_QSO_UPLOAD_STATUS,
   IOTA,  IOTA_ISLAND_ID,  ITUZ,
   K_INDEX,
   LAT,  LON,  LOTW_QSLRDATE,  LOTW_QSLSDATE,  LOTW_QSL_RCVD,  LOTW_QSL_SENT,
   MAX_BURSTS,  MODE,  MS_SHOWER,  MY_ANTENNA,  MY_ANTENNA_INTL,  MY_CITY,  MY_CITY_INTL,  MY_CNTY,  MY_COUNTRY,  MY_COUNTRY_INTL,  MY_CQ_ZONE,  MY_DXCC,  MY_FISTS,  MY_GRIDSQUARE,  MY_IOTA,  MY_IOTA_ISLAND_ID,  MY_ITU_ZONE,  MY_LAT,  MY_LON,  MY_NAME,  MY_NAME_INTL,  MY_POSTAL_CODE,  MY_POSTAL_CODE_INTL,  MY_RIG,  MY_RIG_INTL,  MY_SIG,  MY_SIG_INTL,  MY_SIG_INFO,  MY_SIG_INFO_INTL,  MY_SOTA_REF,  MY_STATE,  MY_STREET,  MY_STREET_INTL,  MY_USACA_COUNTIES,  MY_VUCC_GRIDS,
   NAME,  NAME_INTL,  NOTES,  NOTES_INTL,  NR_BURSTS,  NR_PINGS,
   OPERATOR,  OWNER_CALLSIGN,
   PFX,  PRECEDENCE,  PROP_MODE,  PUBLIC_KEY,
   QRZCOM_QSO_UPLOAD_DATE,  QRZCOM_QSO_UPLOAD_STATUS,  QSLMSG,  QSLMSG_INTL,  QSLRDATE,  QSLSDATE,  QSL_RCVD,  QSL_RCVD_VIA,  QSL_SENT,  QSL_SENT_VIA,  QSL_VIA,  QSO_COMPLETE,  QSO_DATE,  QSO_DATE_OFF,  QSO_RANDOM,  QTH,  QTH_INTL,
   REGION,  RIG,  RIG_INTL,  RST_RCVD,  RST_SENT,  RX_PWR,
   SAT_MODE,  SAT_NAME,  SFI,  SIG,  SIG_INTL,  SIG_INFO,  SIG_INFO_INTL,  SILENT_KEY,  SKCC,  SOTA_REF,  SRX,  SRX_STRING,  STATE,  STATION_CALLSIGN,  STX,  STX_STRING,  SUBMODE,  SWL,
   TEN_TEN,  TIME_OFF,  TIME_ON,  TX_PWR,
   UKSMG,  USACA_COUNTIES,
   VE_PROV,  VUCC_GRIDS,
   WEB
   */

};

#endif // QSO_H
