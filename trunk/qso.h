#ifndef QSO_H
#define QSO_H
/***************************************************************************
                          qso.h  -  description
                             -------------------
    begin                : dec 2014
    copyright            : (C) 2014 by Jaime Robles
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
// The class QSO shall include ALL the ADIF fields
// Current ADIF version http://www.adif.org/304/ADIF_304.htm
#include <QString>

enum AntPath_t {G, O, S, L};
enum ARRLSec_t {AL, AK, AB, AR, AZ, BC, CO, CT, DE, EB, EMA, ENY, EPA, EWA, GA, ID, IL, IN, IA,
                KS, LAX, LA, ME, MB, MAR, MDC, MI, MN, MS, MO, MT, NE, NV, NH, NM, NLI, NL, NC, ND,
                NTX, NFL, NNJ, NNY, NT, NWT, OH, OK, ON, ORG, OR, PAC, PR, QC, RI, SV, SDG, SF,
                SJV, SB, SCV, SK, SC, SD, STX, SFL, SNJ, TN, VI, UT, VT, VA, WCF, WTX, WV, WMA, WNY,
                WPA, WWA, WI, WY};

class QSO
{


public:
    QSO();
    ~QSO();
private:
    //Strings
    QString address, address_int, call, check, contestClass, comment, comment_intl, contacted_op,
            contest_id, email, eq_call, fists, my_fists, guest_op, ms_shower, my_city,
            my_city_intl, name, name_intl, my_name, my_name_intl, my_postal_code, my_postal_code_intl,
            my_rig, my_rig_intl, my_sig, my_sig_intl, my_sig_info, my_sig_info_intl, my_street,
            my_street_intl, notes, notes_intl, operator_call, owner_callsign, pxf, precedence,
            public_key, qslmsg, qslmsg_intl, qsl_via, qth, qth_intl, rig, rig_intl, rst_rcvd, rst_sent,
            sat_mode, sat_name, sig, sig_intl, sig_info, sig_info_intl, skcc, srx_string, stx_string,
            station_callsign, web;

    //BOOLEAN
    QChar   force_init, qso_random, SWL;

    //Numbers
    QString age, a_index, ant_az, ant_el, cqz, distance, freq, freq_rx, ituz, k_index, max_bursts,
            my_cq_zone, nr_bursts, nr_pings, rx_pwr, sfi, srx, stx, ten_ten, tx_pwr;

    //Dates
    QString qso_date, qso_date_off, clublog_qso_upload_date, eqsl_qslrdate, eqsl_qslsdate, hrdlog_qso_upload_date,
            lotw_qslrdate, lotw_qslsdate, qrzcom_qso_upload_date, qslrdate, qslsdate;

    //TIME
    QString time_off, time_on;
    //QSL
    QString eqsl_qsl_rcvd;
    QString eqsl_qsl_sent;

    //Enums
    QString mode, submode;
    QString qso_complete;
    QString ant_path;
    QString arrl_sect;
    QString award_submitted;
    QString award_granted;
    QString band, band_rx;
    QString cnty, my_cnty;
    QString my_usaca_counties, usaca_couties;
    QString cont;
    QString credit_submitted;
    QString credit_granted;
    QString dxcc, my_dxcc;
    QString my_vucc_grids, vucc_grids;
    QString prop_mode;



   //GRIDSQUARE
    QString gridsquare, my_gridsquare;

    //Location
    QString lat, lon;

    //QSO UPLOAD STATUS
    QString clublog_qso_upload_status, hrdlog_qso_upload_status, qrzcom_qso_upload_status;

    //IOTA
    QString iota, iota_island_id, my_iota, my_iota_island_id;

    //QSL_RCVD
    QString lotw_qsl_rcvd, qsl_rcvd;

    //QSL_SENT
    QString lotw_qsl_sent, qsl_sent;

    //QSL-VIA
    QString qsl_rcvd_via, qsl_sent_via;

    //SOTAREF
    QString my_sota_ref, sota_ref;

    //COUNTRY
    QString my_country, my_country_intl, country, country_intl;
    QString my_state, state;


   // AntPath_t ant_path;
   // ARRLSec_t arrl_sect;



};

#endif // QSO_H
