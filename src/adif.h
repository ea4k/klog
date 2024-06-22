#ifndef KLOG_ADIF_H
#define KLOG_ADIF_H
/***************************************************************************
                          adif.h  -  description
                             -------------------
    begin                : ago 2023
    copyright            : (C) 2023 by Jaime Robles
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
/*
    This class implements general controls on ADIF standard
    Refer to https://www.adif.org/

    This classs hould not need to query the DB neither the DataProxy Class

*/
#include <QObject>
#include <QtGlobal>
#include <QString>
#include <QtDebug>
#include <QStringList>
//#include "locator.h"
#include "klogdefinitions.h"


class Adif : public QObject {
    Q_OBJECT
    //friend class tst_Adif;

public:
    Adif(const QString &_parentName);
    ~Adif();
    void init();
    bool isValidFreq(const QString &_b);            //>0
    bool isValidPower(const QString &_b);           //>0
    bool isValidK_Index(const QString &_b);         //0-9
    bool isValidCQz(const QString &_b);             //1-40
    bool isValidITUz(const QString &_b);            // 1-90
    bool isValidDXCC(const QString &_b);            // 0-522
    bool isValidAge(const QString &_b);             // 0-120
    bool isValidDistance(const QString &_b);        //>0.0
    bool isValidAnt_EL(const QString &_b);          //>=0-360
    bool isValidAnt_AZ(const QString &_b);          //>=-90-90
    bool isValidA_Index(const QString &_b);         //0-400
    bool isValidIOTA_islandID(const QString &_b);   //1-99999999
    bool isValidNRBursts(const QString &_b);        //>0
    bool isValidPings(const QString &_b);            //>0
    bool isValidSFI(const QString &_b);            //0-300
    bool isValidSTX(const QString &_b);            //>=0
    bool isValidSRX(const QString &_b);            //>=0
    bool isValidFISTS(const QString &_b);            //>0
    bool isValidUKSMG(const QString &_b);            //>0
    bool isValidTenTen(const QString &_b);            //>0
    bool isValidLogId(const QString &_b);            //>0
    bool isValidAntPath(const QString &_s);
    bool isValidQSO_COMPLETE(const QString &_s);    // "Y", "N", "NIL", "?"
    bool isValidPOTA(const QString &_s);            // TODO
    bool isValidWWFF_Ref(const QString &_s);        // TODO

    void setLogLevel(DebugLogLevel _l);
    QString getADIFField(const QString &_fieldName, const QString &_data);
    QString getADIFBoolFromBool(const bool _b);             // Will produce the ADIF format if a bool is received

signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);

private:
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    void InitializeHash();
    void setARRLSect();
    void setContinents();
    void setSponsorsList();

    bool isValidCall(const QString &_c);

    QString parentName;
    DebugLogLevel logLevel;
    QHash<QString, QString> ADIFHash; // Name, type

    QStringList ARRL_sects, continents, sponsorsList;
};

#endif // ADIF_H


