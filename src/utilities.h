#ifndef KLOG_UTILITIES_H
#define KLOG_UTILITIES_H
/***************************************************************************
                          utilities.h  -  description
                             -------------------
    begin                : jun 2015
    copyright            : (C) 2015 by Jaime Robles
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
    This class implements general use utilities that can be used from any other class
    Anything that is not directly related with a class itself and could be used from different
    classes should be here.

    This classs hould not need to query the DB neither the DataProxy Class

*/
#include <iostream>
#include <QObject>
#include <QtGlobal>
#include <QString>
#include <QtWidgets>
#include <QPalette>
#include <QtDebug>
#include "locator.h"
#include "klogdefinitions.h"
#include "adif.h"
//#include "dataproxy_sqlite.h"

class Utilities : public QObject {
    Q_OBJECT
    friend class tst_Utilities;

public:
    Utilities(const QString &_parentName);
    ~Utilities();
    //void setDarkMode(const QString &_dm);
    //bool isDarkMode();
    void setCallValidation(const bool _b);
    void setLongPrefixes (const QStringList &_p);
    void setSpecialCalls (const QStringList &_p);

    int getProgresStepForDialog(int totalSteps);
    bool trueOrFalse(const QString &_s); // reads a String and return true if s.upper()== TRUE :-)
    QChar boolToCharToSQLite(const bool _b);
    QString boolToQString(const bool _b);
    QString checkAndFixASCIIinADIF(const QString &_data);
    QString getAgent(const QString &_klogversion);
    QString getGlobalAgent(const QString &_klogversion);
    //QOperatingSystemVersion getOS();
    //Devel or debug functions - Not adding any feature to the user
    void printQString(const QStringList &_qs);
    QString getKLogDBFile();
    QString getKLogDBBackupFile();
    bool QStringToBool(const QString &_s);

    //QString getKLogDatabaseFile(const QString &_file);
    bool fileExists(const QString &_fileName);

    QString getTQSLsFileName();
    QString getTQSLsPath();   // Depending on the OS where are usually installed the executables
    QString getHomeDir();
    QString getCfgFile();
    QString getCTYFile();
    QString getDebugLogFile();
    QString getSaveSpotsLogFile();
    QString getBackupADIFile();
    QString getClubLogFile();
    QString getEQSLFile();
    QString getLoTWAdifFile();
    QString getClearSQLi(QString _s);
    void setVersion(const QString &_v);
    QString getVersion();
    double getVersionDouble();

    QDate getDefaultDate();
    QString getDefaultRST(const QString &_m);
    QStringList getDefaultLogFields();

    int getNormalizedDXCCValue(const int _dxcc);

    // Validations
    bool isValidDate(const QDate _d);
    bool isValidDateTime(const QString &_d);
    bool isValidCall(const QString &_c, bool _force=false);

    bool isSameFreq(const double fr1, const double fr2);
    bool isValidBandId(const int _b);
    bool isValidModeId(const int _m);
    bool isValidFreq(const QString &_b);
    bool isValidGrid(const QString &_b);
    bool isValidVUCCGrids(const QString &_b);
    bool isValidRST(const QString &_b);
    bool isValidPower(const QString &_b);
    bool isValidComment(const QString &_b);
    bool isValidName(const QString &_b);
    bool isValidADIFField(const QString &_b);
    bool isValidQSL_Rcvd(const QString &c);
    bool isValidQSL_Sent(const QString &c);
    bool isValidUpload_Status(const QString &c);
    //bool isValidFISTS(const QString &c);

    bool isValidTimeFromString(const QString &_s);
    bool isValidDateFromString(const QString &_s);
    bool isValidDateTimeFromString(const QString &_s);
    bool isValidDXCC(const int _d);
    bool isValidAntPath(const QString &_s);
    bool isValidARRLSect(const QString &_s);
    bool isValidContinent(const QString &_s);
    bool isValidPropMode(const QString &_s);
    bool isValidEmail(const QString &_s);
    QStringList getValidADIFFieldAndData(const QString &_b);
    //QString getADIFField(const QString &_fieldName, const QString &_data);

    QString getMainCallFromComplexCall(const QString &_complexCall); // F from F/EA4K/p, EA4K from EA4K/p or EA4K from EA4K
    QString getAValidCall (const QString &_wrongCall);
    QString getPrefixFromCall(const QString &_c, bool withAreaNumber = false);
    bool isAOneLetterPrefix(const QChar &_c);

    // Write DATE/TIME to DB
    QString getDateTimeSQLiteStringFromDateTime(const QDateTime &_d);
    QString getDateSQLiteStringFromDate(const QDate &_d);

    // Read from DB
    QDateTime getDateTimeFromSQLiteString(const QString &_s);
    QTime getTimeFromSQLiteString(const QString &_s);
    QDate getDateFromSQliteString(const QString &_s);

    // Translate the LOG table fields into human readable
    QString getLogColumnName(const QString &_column);


    // Creates the ADIF DATE & TIME formats
    QString getADIFDateFromQDateTime(const QDateTime &_d);  // Will produce the ADIF DATE format: "YYYYMMDD"
    QString getADIFDateFromQDate(const QDate &_d);          // Will produce the ADIF DATE format: "YYYYMMDD"
    QString getADIFTimeFromQDateTime(const QDateTime &_d);  // Will produce the ADIF TIME format: "HHMMSS"
    QString getADIFTimeFromQTime(const QTime &_d);      // Will produce the ADIF TIME format: "HHMMSS"

    // Parse Date & Time from ADIF
    QDate getDateFromADIFDateString(const QString &_s);     // Expects an ADIF DATE format string: "YYYYMMDD"
    QTime getTimeFromADIFTimeString(const QString &_s);     // Expects and ADIF TIME format String "HHMMSS" or "HHMM"

    // Parse date fromLoTW
    QDate getDateFromLoTWQSLDateString(const QString &_s);

    // Creates the Cabrillo DATE & TIME (http://wwrof.org/cabrillo/)
    QString getCabrilloDateFromQDate(const QDate &_d);          // Will produce the Cabrillo DATE format: "YYYY-MM-DD"
    QString getCabrilloTimeFromQDateTime(const QDateTime &_d);  // Will produce the Cabrillo TIME format: "HHMM"

    // Parse QSO_COMPLETE from ADIF
    QString getQSO_CompleteFromADIF(const QString &_s);     // Expect a string and returns 1char string for DB
    QString getADIFQSO_CompleteFromDB(const QString &_s);   // Returns the ADIF QSO_COMPLETE


    QString getOnlineServiceName(OnLineProvider _service);

    bool isValidDistance(const double _d);
    bool isValidSponsor(const QString &_s);

    QString debugLevelToString(DebugLogLevel _l);
    DebugLogLevel stringToDebugLevel(const QString &_s);
    bool isValidLogLevel(const QString &_s);
    QStringList getDebugLevels();
    void setLogLevel(DebugLogLevel _l);
    void openQrzcom(const QString _call);
    void printCommandHelp();
    //QPalette getPalete(bool _ok);
signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);

private:
    void init();
    void InitializeHash();
    void setLogColumnNames(); // Creates the map of column Names (should be called from init() )
    bool processConfigLine(const QString &_line);
    QString getKLogDefaultDatabaseFile();
    //bool isCountrySuffix (const QString &_c);
    bool isAValidOperatingSuffix (const QString &_c);
    bool isAPrefix(const QString &_c); // Returns true if it is the country prefix or it includes the area number
    bool isAKnownPrefix(const QString &_c);
    bool isAKnownCall(const QString &_c);
    bool isValidSimpleCall(const QString &_c);
    //QPair<QString, QString> getCallParts(const QString &_c);

    void setARRLSect();
    void setContinent();
    void setSponsorsList();
    QString getCheckedComplexCall(const QString &_c);
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    //bool darkMode;
    QString dbPath;
    QString softwareVersion;
    QString parentName;     // The class that instanciated this object.

    QStringList ARRL_sects, continent, sponsorsList, logLevels;
    DebugLogLevel logLevel;
    QStringList longPrefixes, specialCalls;
    QMap<QString, QString> columnNames;
    bool validateCalls;
    QHash<QString, QString> ADIFHash; // Name, type

};

#endif // UTILITIES_H


