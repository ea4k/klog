#ifndef KLOG_CALLSIGN_H
#define KLOG_CALLSIGN_H
/***************************************************************************
                          callsign.h  -  description
                             -------------------
    begin                : ago 2024
    copyright            : (C) 2024 by Jaime Robles
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
    This class implements the object callsign to centralize all about callsigns
    This code is mainly coming from QLog: https://github.com/foldynl/QLog/blob/master/core/Callsign.h
    Thank you Lada, OK1MLG.

    Important: https://cqwpx.com/rules.htm
    This classs hould not need to query the DB neither the DataProxy Class
*/
#include <QDebug>
#include <QObject>
#include <QRegularExpression>

class Callsign : public QObject
{
    Q_OBJECT
public:
    Callsign(const QString &callsign,
                      QObject *parent = nullptr);
    ~Callsign();
    static QStringList secondarySpecialSuffixes;


    // Needed fields for a call are:
    // - hostFullPrefix (QString)
    // - hostPrefix     (QString)
    // - hostAreaNumber (int)
    // - homeCallsign
    // - homeFullPrefix (QString)   EA4
    // - homePrefix     (QString)   EA
    // - homeAreaNumber (int)       4
    // - homeSuffix     (QString)
    // - suffix         (QString)

    QString getCallsign();                  // Returns the FULL callsign                                            (fullCall)
    QString getHostFullPrefix();            // The complete host prefix (simple + area number if exists)            (hostFullPrefix)
    QString getHostPrefix();                // The host prefix (simple without area number if exists)               (hostPrefix)
    int getHostAreaNumber();                // Get host area number                                                 (hostAreaNumber)


    QString getHomeCallsign();              // Returns the base / home callsign like EA4K in K1/EA4K, or EA4K/QRP   (fullCall)
    QString getHomeFullPrefix();            // The complete home prefix (simple + area number if exists)            (homeFullPrefix)
    QString getHomePrefix();                // The Home prefix without area number                                  (homePrefix)
    int getHomeAreaNumber();                // Get the home area number                                             (homeAreaNumber)
    QString getHomeSuffix();                // The suffix of the home call                                          (homeSuffix)
    QString getSuffix();                    // Additional suffixes like /P, /QRP, /MM, ...                          (generalSuffix)

    bool isValid();                         // True if it is a full callsign
    bool isValidPrefix();                   // True if it is a prefix, but not a call
    void clear();

private:
    static QString callsignRegExString();
    static QRegularExpression callsignRegEx();
    static QString prefixRegExString();
    static QRegularExpression prefixRegEx(); 

    QString fullCall;



    QString homeCallsign;       // EA4K in EA4K or KB1/EA4K/QRP
    QString homePrefix;         // EA4  in EA4K

    QString homeSuffix;         // K    in EA4K
    int homeAreaNumber;         // 4    in EA4K



    QString generalSuffix;      // QRP in EA4K/QRP


    bool valid;         // The entered string is a correct callsign
    bool prefValid;     // The entered string is a correct prefix

    // Needed fields for a call are:
    // - hostFullPrefix (QString)
    // - hostPrefix     (QString)
    // - hostAreaNumber (int)
    // - homeCallsign
    // - homeFullPrefix (QString)
    // - homePrefix     (QString)
    // - homeAreaNumber (int)
    // - homeSuffix     (QString)
    // - suffix         (QString)

    QString hostFullPrefix;
    QString hostPrefix;
    int hostAreaNumber;
    QString homeFullPrefix;
};

#endif // CALLSIGN_H
