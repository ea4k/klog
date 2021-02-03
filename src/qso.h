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

    bool setData(const QString &_adifPair);
    void clear();
    bool isValid();
    void setLoTWUpdating(bool _lotw);

    bool setDate(const QDate &_c);
    QDate getDate();

    bool setTimeOn(const QTime &_c);
    QTime getTimeOn();

    QDateTime getDateTimeOn();
    bool setDateTime(const QDateTime &_c);

    bool setQSOid(const int _i);
    int getQSOid();

    bool setLogId(const int _i);
    int getLogId();

    bool setBand(const QString &_c);
    QString getBand();
    bool setBandRX(const QString &_c);
    QString getBandRX();

    bool setMode(const QString &_c);
    QString getMode();

    bool setCall(const QString &_c);
    QString getCall();

    bool setDXCC(const int _i);
    int getDXCC();

    bool setFreq(const float _f);
    bool setFreqRX(const float _f);
    float getFreq();
    float getFreqRX();

    bool setQSL_RCVD(const QString &_c);
    QString getQSL_RCVD();

    bool setQSLRDate(const QDate &_c);
    QDate getQSLRDate();

    bool setLoTWQSL_RCVD(const QString &_c);
    QString getLoTWQSL_RCVD();
    bool setLoTWQSL_SENT(const QString &_c);
    QString getLoTWQSL_SENT();
    bool setLoTWQSLRDate(const QDate &_c);
    QDate getLoTWQSLRDate();
    bool setLoTWQSLSDate(const QDate &_c);
    QDate getLoTWQSLSDate();


    bool setPropMode(const QString &_c);
    QString getPropMode();

    bool setSatName(const QString &_c);
    QString getSatName();

    bool setStationCallsign(const QString &_c);
    QString getStationCallsign();

    bool setGridSquare(const QString &_c);
    QString getGridSquare();

    bool add();


private:



    QString satName, callsign, stationCallsign, propMode, band, mode, gridsquare;
    int qsoId, logId, dxcc;
    QString qsl_rcvd;
    QDate QSLRDate, QSLLoTWRDate, QSLLoTWSDate;
    QDateTime qso_dateTime;    
    float freq, freq_rx;
    QString lotw_qsl_sent, lotw_qsl_rcvd;

    Utilities *util;

    bool lotwUpdating;
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
