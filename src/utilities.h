#ifndef UTILITIES_H
#define UTILITIES_H
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
    classes should be here

*/
#include <QtGlobal>
#include <QString>
#include <QtWidgets>
#include <QPalette>
#include "locator.h"
#include <QtDebug>
#include "klogdefinitions.h"

class Utilities
{
public:
    Utilities();
    ~Utilities();
    void setDarkMode(const QString &_dm);
    bool isDarkMode();
    void setCallValidation(const bool _b);

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

    //QString getKLogDatabaseFile(const QString &_file);
    bool isDBFileExisting();
    bool isDBFileExisting(const QString &_file);
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
    bool isValidCall(const QString &_c);

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
    bool isValidQSL_Rcvd(const QString &c, bool rcvd=true); // If receiving is true, If creating the ADIF is false
    bool isValidQSL_Sent(const QString &c);
    bool isValidUpload_Status(const QString &c);

    bool isValidTimeFromString(const QString &_s);
    bool isValidDateFromString(const QString &_s);
    bool isValidDateTimeFromString(const QString &_s);
    bool isValidDXCC(const int _d);
    QStringList getValidADIFFieldAndData(const QString &_b);
    QString getAValidCall (const QString &_wrongCall);
    QString getPrefixFromCall(const QString &_c);
    //QString getPrefixFromFullCall(const QString &_c);

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

    // Parse Date & Time from ADIF
    QDate getDateFromADIFDateString(const QString &_s);     // Expects an ADIF DATE format string: "YYYYMMDD"
    QTime getTimeFromADIFTimeString(const QString &_s);     // Expects and ADIF TIME format String "HHMMSS" or "HHMM"

    // Parse date fromLoTW
    QDate getDateFromLoTWQSLDateString(const QString &_s);

    // Creates the Cabrillo DATE & TIME (http://wwrof.org/cabrillo/)
    QString getCabrilloDateFromQDate(const QDate &_d);          // Will produce the Cabrillo DATE format: "YYYY-MM-DD"
    QString getCabrilloTimeFromQDateTime(const QDateTime &_d);  // Will produce the Cabrillo TIME format: "HHMM"

    QString getOnlineServiceName(OnLineProvider _service);
    //QPalette getPalete(bool _ok);
private:
    void init();
    bool processConfigLine(const QString &_line);
    QString getKLogDefaultDatabaseFile();
    int isAPrefix(const QString &_c);
    bool isValidSubCall(const QString &_c);
    //QPair<QString, QString> getCallParts(const QString &_c);

    bool darkMode, callValidation;
    QString dbPath;
    QString softwareVersion;
    //QPalette palRed, palBlack;

};

#endif // UTILITIES_H


