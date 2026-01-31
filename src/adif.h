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
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtDebug>
#include <QtGlobal>
#include "klogdefinitions.h"

class Adif : public QObject {
    Q_OBJECT
    //friend class tst_Adif;

public:
    Adif(const QString &_parentName);
    ~Adif();
    void init();
    ADIFField setPair(const QString &_pair);

    bool isValidFreq(const QString &_b);            //>0
    bool isValidPower(const double _b);           //>0
    bool isValidK_Index(const int _b);         //0-9
    bool isValidCQz(const int _b);              //1-40
    bool isValidITUz(const int _b);            // 1-90
    bool isValidDXCC(const int _b);            // 0-522
    bool isValidAge(const double _b);             // 0-120
    bool isValidDistance(const double _b);        //>0.0
    bool isValidAnt_EL(const double _b);          //>=0-360
    bool isValidAnt_AZ(const double _b);          //>=-90-90
    bool isValidA_Index(const double _b);              //0-400
    bool isValidIOTA_islandID(const int _b);   //1-99999999
    bool isValidNRBursts(const int _b);        //>0
    bool isValidPings(const int _b);            //>0
    bool isValidSFI(const int _b);            //0-300
    bool isValidSTX(const int _b);            //>=0
    bool isValidSRX(const int _b);            //>=0
    bool isValidFISTS(const int _b);            //>0
    bool isValidUKSMG(const int _b);            //>0
    bool isValidTenTen(const int _b);            //>0
    bool isValidLogId(const int _b);            //>0
    //bool isValidAltitude (const double _s);       // > 10000 (10000 is a default value in KLog)
    bool isValidAntPath(const QString &_s);
    bool isValidMode (const QString &_s);
    bool isValidQSO_COMPLETE(const QString &_s);        // "Y", "N", "NIL", "?"
    QString getQSO_COMPLETEFromDB(const QString &_s);   // Translates the DB value into an ADIF value
    int setQSO_COMPLETEToDB(const QString &_s);         // Translates the ADIF to a DB value

    bool isValidPOTA(const QString &_s);            // TODO
    bool isValidWWFF_Ref(const QString &_s);        // TODO
    bool isValidQSLRCVD(const QString &_s, bool _importing=true);         // Y, I, or V) (V on import only)
    bool isValidQSLSENT(const QString &_s);         // Y, Q, or I

    void setLogLevel(DebugLogLevel _l);
    QString getADIFField(const QString &_fieldName, const QString &_data);
    QString getADIFBoolFromBool(const bool _b);             // Will produce the ADIF format if a bool is received
    void setModes(const QStringList &_modes);            //TODO: Do not depend on external source to fill

    QStringList getQSOUploadStatus (bool _fullName = false);    // Returns the ADIF enumeration; fullname= true -> Yes-Upload
    QStringList getQSLSentStatus (bool _fullName = false);      // Returns the ADIF enumeration; fullname= true -> Yes-Sent
    QStringList getQSLRecStatus (bool _fullName = false);       // Returns the ADIF enumeration; fullname= true -> Yes-Sent

    QString getADIFDateStringFromLoTWDateTime(const QString &_lotwdatetime);      // Returns the date when importing LoTW ADIF

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
    QHash<QString, QString> ADIFHash;   // Name, type
    QStringList notZeroFields;          // Numeral ADIF fields where having a Zero makes no sense

    QStringList ARRL_sects, continents, sponsorsList, modes;
};

#endif // ADIF_H

