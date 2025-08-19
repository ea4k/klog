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
    The inspiration and part of the code is coming from QLog: https://github.com/foldynl/QLog/blob/master/core/Callsign.h
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
    Callsign(const QString &callsign, QObject *parent = nullptr);
    ~Callsign();
    void operator()(const QString &newCallsign);

    //static QStringList secondarySpecialSuffixes;

    // K1/EA4K/QRP
    // Needed fields for a call are:
    // - hostFullPrefix (QString)   K1
    // - hostPrefix     (QString)   K
    // - hostAreaNumber (int)       1
    // - homeCallsign               EA4K
    // - homeFullPrefix (QString)   EA4
    // - homePrefix     (QString)   EA
    // - homeAreaNumber (int)       4
    // - homeSuffix     (QString)   K
    // - suffix         (QString)   QRP

    QString getCallsign();                                  // Returns the FULL callsign                                                                        (fullCall)
    QString getHostFullPrefix();    // The complete host prefix (simple + area number if exists)  , if only prefix it should be false   (hostFullPrefix)
    QString getHostPrefix();        // The host prefix (simple without area number if exists), if only prefix it should be false        (hostPrefix)
    int     getHostAreaNumber();    // Get host area number, if only prefix it should be false                                          (hostAreaNumber)
                                                            // if getHomeIfEmpty is true and no hostprefix is identified, it will return homePrefix


    QString getHomeCallsign();              // Returns the base / home callsign like EA4K in K1/EA4K, or EA4K/QRP   (fullCall)
    QString getHomeFullPrefix();            // The complete home prefix (simple + area number if exists)            (homeFullPrefix)
    QString getHomePrefix();                // The Home prefix without area number                                  (homePrefix)
    int     getHomeAreaNumber();            // Get the home area number                                             (homeAreaNumber)
    QString getHomeSuffix();                // The suffix of the home call                                          (homeSuffix)
    QString getSuffix();                    // Additional suffixes like /P, /QRP, /MM, ...                          (generalSuffix)

    bool isValid();                             // True if it is a full callsign
    bool isValidPrefix();                       // True if it is a prefix, but not a call
    bool isSimple();                            // True if it has no / nor \ characters, no prefix nor suffix
    bool isAOneLetterHostPrefix();   // True if is the prefix starts by B|F|G|I|K|M|N|R|U|W and is valid
    void clear();

private:
    static QString              getSpecialPrefixes();
    static QString              callsignRegExString();
    static QRegularExpression   callsignRegEx();
    static QString              prefixRegExString();
    static QRegularExpression   prefixRegEx();

    void    initialize(const QString &callsign);    // A helper to perform the initialization and prevent code duplication
                       //Returns the list of special prefixes for the REGEX

    QString fullCall;           // K1/EA4K/QRP
    QString hostFullPrefix;     // K1
    QString hostPrefix;         // K
    int     hostAreaNumber;     // 1
    QString homeCallsign;       // EA4K
    QString homeFullPrefix;     // EA4
    QString homePrefix;         // EA
    QString homeSuffix;         // K
    int     homeAreaNumber;     // 4
    QString generalSuffix;      // QRP


    bool valid;         // The entered string is a correct callsign
    bool prefValid;     // The entered string is a correct prefix
};

#endif // CALLSIGN_H
