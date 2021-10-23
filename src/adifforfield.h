#ifndef ADIFFORFIELD_H
#define ADIFFORFIELD_H

/***************************************************************************
                          adifforfield.h  -  description
                             -------------------
    begin                : oct 2021
    copyright            : (C) 2021 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/
#include <QObject>
#include <QString>
#include "klogdefinitions.h"
#include "utilities.h"

class ADIFForField: public QObject {
    Q_OBJECT
    // TODO:
    //      Add checks for all the data (maybe adding the dataProxy to check helps
    //      but may slow down the process as queries will be executed.
public:
    ADIFForField();

    QString getADIFForQSODate(const QString &_data, ExportMode _em = ModeADIF);
    QString getADIFForCall(const QString &_data);
    QString getADIFForRSTSent(const QString &_data);
    QString getADIFForRSTRcvd(const QString &_data);
    QString getADIFForBand(const QString &_data);
    QString getADIFForMode(const QString &_data);
    QString getADIFForSubMode(const QString &_data);
    QString getADIFForCQz(const QString &_data);
    QString getADIFForITUz(const QString &_data);
    QString getADIFForDXCC(const QString &_data);
    QString getADIFForAddress(const QString &_data);
    QString getADIFForAge(const QString &_data);
    QString getADIFForCNTY(const QString &_data);
    QString getADIFForComment(const QString &_data);
    QString getADIFForA_Index(const QString &_data);
    QString getADIFForAnt_az(const QString &_data);
    QString getADIFForAnt_el(const QString &_data);
    QString getADIFForAnt_path(const QString &_data);
    QString getADIFForARRL_sect(const QString &_data);
    QString getADIFForGridSquare(const QString &_data);
    QString getADIFForMyGridSquare(const QString &_data);
    QString getADIFForQSODateOff(const QString &_data, ExportMode _em = ModeADIF);
    QString getADIFForFreq(const QString &_data);
    QString getADIFForStationCallsign(const QString &_data);

    QString getADIFForBandRX(const QString &_data);
    QString getADIFForFreq_rx(const QString &_data);

    QString getADIFForQSLRDate(const QString &_data);
    QString getADIFForQSLSDate(const QString &_data);
    QString getADIFForQSLRcvd(const QString &_data);
    QString getADIFForQSLSent(const QString &_data);

    QString getADIFForLoTWRDate(const QString &_data);
    QString getADIFForLoTWSDate(const QString &_data);
    QString getADIFForLoTWQSLRcvd(const QString &_data);
    QString getADIFForLoTWQSLSent(const QString &_data);

    QString getADIFForClubLogQSOUploadDate(const QString &_data);
    QString getADIFForClubLogQSOUploadStatus(const QString &_data);


 /*

    QString getADIFForHRDLogQSOUploadStatus(const QString &_data);
    QString getADIFForHRDLogQSOUploadDate(const QString &_data);
    QString getADIFForHRDLogQSOUploadStatus(const QString &_data);
    QString getADIFForQRZCOMQSOUploadDate(const QString &_data);
    QString getADIFForQRZCOMQSOUploadStatus(const QString &_data);
    QString getADIFForAward_Submitted(const QString &_data);
    QString getADIFForAward_Granted(const QString &_data);

    QString getADIFForCheckContest(const QString &_data);
    QString getADIFForClass(const QString &_data);

    QString getADIFForCont(const QString &_data);
    QString getADIFForContactedOp(const QString &_data);
    QString getADIFForContestId(const QString &_data);
    QString getADIFForCountry(const QString &_data);
    QString getADIFForCreditSubmitted(const QString &_data);
    QString getADIFForCreditGranted(const QString &_data);
    QString getADIFForDark_doc(const QString &_data);
    QString getADIFForDistance(const QString &_data);
    QString getADIFForEmail(const QString &_data);
    QString getADIFForEQ_Call(const QString &_data);
    QString getADIFForEQSL_QSLRDate(const QString &_data);
    QString getADIFForEQSL_QSLSDate(const QString &_data);
    QString getADIFForEQSL_QSLRCVD(const QString &_data);
    QString getADIFForEQSL_QSLSent(const QString &_data);
    QString getADIFForFists(const QString &_data);
    QString getADIFForFist_cc(const QString &_data);
    QString getADIFForForce_init(const QString &_data);

    QString getADIFForGuest_op(const QString &_data);
    QString getADIFForIOTA(const QString &_data);
    QString getADIFForIOTAIslandId(const QString &_data);
    QString getADIFForMy_IOTA(const QString &_data);
    QString getADIFForMy_IOTAIslandId(const QString &_data);
    QString getADIFForK_Index(const QString &_data);
    QString getADIFForLat(const QString &_data);
    QString getADIFForLon(const QString &_data);
    QString getADIFForMaxBurst(const QString &_data);
    QString getADIFForMultiplier(const QString &_data);
    QString getADIFForMS_Shower(const QString &_data);
    QString getADIFForMy_Antenna(const QString &_data);
    QString getADIFForMy_City(const QString &_data);
    QString getADIFForMy_Cnty(const QString &_data);
    QString getADIFForMy_Country(const QString &_data);
    QString getADIFForMy_CQz(const QString &_data);
    QString getADIFForMy_DXCC(const QString &_data);
    QString getADIFForMy_Fists(const QString &_data);
    QString getADIFForMy_Ituz(const QString &_data);
    QString getADIFForMy_Lat(const QString &_data);
    QString getADIFForMy_Lon(const QString &_data);
    QString getADIFForMy_Name(const QString &_data);
    QString getADIFForMy_PostalCode(const QString &_data);
    QString getADIFForMy_Rig(const QString &_data);
    QString getADIFForSig(const QString &_data);
    QString getADIFForSig_info(const QString &_data);
    QString getADIFForMy_Sig(const QString &_data);
    QString getADIFForMy_Sig_info(const QString &_data);
    QString getADIFForMy_SotaRef(const QString &_data);
    QString getADIFForSotaRef(const QString &_data);
    QString getADIFForMy_State(const QString &_data);
    QString getADIFForMy_Street(const QString &_data);
    QString getADIFForMy_USACA_Counties(const QString &_data);
    QString getADIFForUSACA_Counties(const QString &_data);
    QString getADIFForMy_VUCC_Grids(const QString &_data);
    QString getADIFForVUCC_Grids(const QString &_data);
    QString getADIFForName(const QString &_data);
    QString getADIFForNotes(const QString &_data);
    QString getADIFForNr_Bursts(const QString &_data);
    QString getADIFForNr_pings(const QString &_data);
    QString getADIFForOperator(const QString &_data);
    QString getADIFForOwner_Callsign(const QString &_data);
    QString getADIFForPfx(const QString &_data);
    QString getADIFForPoints(const QString &_data);
    QString getADIFForPrecedence(const QString &_data);
    QString getADIFForProp_Mode(const QString &_data);
    QString getADIFForPublic_key(const QString &_data);
    QString getADIFForQSLMsg(const QString &_data);

    QString getADIFForQSLSentVia(const QString &_data);
    QString getADIFForQSLRecVia(const QString &_data);
    QString getADIFForQSLVia(const QString &_data);
    QString getADIFForQSO_Complete(const QString &_data);
    QString getADIFForQSO_Random(const QString &_data);
    QString getADIFForQTH(const QString &_data);
    QString getADIFForRegion(const QString &_data);
    QString getADIFForRig(const QString &_data);
    QString getADIFForRX_Pwr(const QString &_data);
    QString getADIFForTX_Pwr(const QString &_data);
    QString getADIFForSat_Mode(const QString &_data);
    QString getADIFForSfi(const QString &_data);

    QString getADIFForSilentKey(const QString &_data);
    QString getADIFForSKCC(const QString &_data);


    QString getADIFForSRx_String(const QString &_data);
    QString getADIFForSTx_String(const QString &_data);

    QString getADIFForSRx(const QString &_data);
    QString getADIFForSTx(const QString &_data);
    QString getADIFForState(const QString &_data);

    QString getADIFForSWL(const QString &_data);
    QString getADIFForUKSMG(const QString &_data);
    QString getADIFForVE_Prov(const QString &_data);
    QString getADIFForTenTen(const QString &_data);
    QString getADIFForWeb(const QString &_data);
    QString getADIFForTransmiterId(const QString &_data);
    QString getADIFForLogNumber(const QString &_data);

  */


private:
    bool showInvalidCallMessage(const QString &_call);
    QString getADIFPair(const QString &_field, const QString &_data);
    Utilities *util;

};

#endif // ADIFFORFIELD_H
