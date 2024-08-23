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

    static QStringList secondarySpecialSuffixes;

    QString getCallsign();
    QString getHostPrefix();                // The complete prefix (simple + area number if exists)
    QString getHostPrefixWithDelimiter();
    QString getBase();
    QString getBasePrefix();
    QString getBasePrefixNumber();
    QString getSuffix();
    QString getSuffixWithDelimiter();
    QString getWPXPrefix();
    QString getSimplePrefix();              // The prefix without the area number
    int getAreaNumber();                    // Just the prefix area number

    bool isValid();                         // True if it is a full callsign
    bool isValidPrefix();                   // True if it is a prefix, but not a call

private:
    static QString callsignRegExString();
    static QRegularExpression callsignRegEx();
    static QString prefixRegExString();
    static QRegularExpression prefixRegEx();

    QString fullCallsign;
    QString hostPrefix;
    QString hostPrefixWithoutAreaNumber;
    QString hostPrefixWithDelimiter;
    QString base;
    QString basePrefix;
    QString basePrefixNumber;
    QString suffix;
    QString suffixWithDelimiter;
    int areaNumber;
    bool valid;         // The entered strig is a correct callsign
    bool prefValid;     // The entered strig is a correct prefix
};

#endif // CALLSIGN_H
