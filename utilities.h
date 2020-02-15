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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
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
#include "locator.h"
#include <QtDebug>

class Utilities
{
public:
    Utilities();
    ~Utilities();
    int getProgresStepForDialog(int totalSteps);
    bool trueOrFalse(const QString &_s); // reads a String and return true if s.upper()== TRUE :-)
    QString checkAndFixASCIIinADIF(const QString &_data);
    QString getAgent(const QString &_klogversion);
    //QOperatingSystemVersion getOS();
    //Devel or debug functions - Not adding any feature to the user
    void printQString(const QStringList &_qs);
    QString getKLogDBFile();

    //QString getKLogDatabaseFile(const QString &_file);
    bool isDBFileExisting();
    bool isDBFileExisting(const QString &_file);

    QString getHomeDir();
    QString getCfgFile();
    QString getCTYFile();
    QString getDebugLogFile();
    QString getSaveSpotsLogFile();

    void setVersion(const QString &_v);
    QString getVersion();
    double getVersionDouble();

    QDate getDefaultDate();    
    int getNormalizedDXCCValue(const int _dxcc);

    // Validations
    bool isValidDate(const QDate _d);
    bool isValidDateTime(const QString &_d);
    bool isValidCall(const QString &_c);
    bool isValidTime(const QString &_t);
    bool isValidBandId(const int _b);
    bool isValidModeId(const int _m);
    bool isValidFreq(const QString &_b);
    bool isValidGrid(const QString &_b);
    bool isValidRST(const QString &_b);
    bool isValidPower(const QString &_b);
    bool isValidComment(const QString &_b);
    bool isValidName(const QString &_b);
    bool isValidADIFField(const QString &_b);
    QStringList getValidADIFFieldAndData(const QString &_b);


private:
    bool processConfigLine(const QString &_line);
    QString getKLogDefaultDatabaseFile();
    QString dbPath;
    QString softwareVersion;

};

#endif // UTILITIES_H

