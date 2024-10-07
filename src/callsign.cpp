/***************************************************************************
                          callsign.cpp  -  description
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
#include "callsign.h"

Callsign::Callsign(const QString &callsign, QObject *parent) : QObject{parent},
    fullCallsign(callsign.toUpper()), areaNumber(0), valid(false), prefValid(false)
{
    //qDebug() << Q_FUNC_INFO << ": " << callsign;

    QRegularExpression callsignRE = callsignRegEx();
    QRegularExpressionMatch match = callsignRE.match(fullCallsign);

    QRegularExpression prefnRE = prefixRegEx();
    QRegularExpressionMatch matchPrefix = prefnRE.match(fullCallsign);


    if ( match.hasMatch() )
    {
        //it is a valid callsign
        valid = true;
        hostPrefixWithDelimiter = match.captured(1);
        hostPrefix              = match.captured(2);
        base                    = match.captured(3);
        basePrefix              = match.captured(4);
        basePrefixNumber        = match.captured(5);
        suffixWithDelimiter     = match.captured(7);
        suffix                  = match.captured(8);
    }
    else if (matchPrefix.hasMatch())
    {
        prefValid = true;

        //qDebug() << Q_FUNC_INFO << " -  match1: " << matchPrefix.captured(1);
        //qDebug() << Q_FUNC_INFO << " -  match2: " << matchPrefix.captured(2);
        //qDebug() << Q_FUNC_INFO << " -  match3: " << matchPrefix.captured(3);
        //qDebug() << Q_FUNC_INFO << " -  match4: " << matchPrefix.captured(4);
        //qDebug() << Q_FUNC_INFO << " -  match5: " << matchPrefix.captured(5);
        //qDebug() << Q_FUNC_INFO << " -  match6: " << matchPrefix.captured(6);
        //qDebug() << Q_FUNC_INFO << " -  match7: " << matchPrefix.captured(7);
        hostPrefix                  = matchPrefix.captured(1);      // Full prefix
        hostPrefixWithoutAreaNumber = matchPrefix.captured(4);      // The prefix without the area number
        areaNumber                  = matchPrefix.captured(7).toInt();      // Just the area number (optional)
    }
    else
    {
        //it is an invalid callsign
        fullCallsign = QString();
    }
}

Callsign::~Callsign(){}

QRegularExpression Callsign::callsignRegEx()
{
    return QRegularExpression(callsignRegExString(), QRegularExpression::CaseInsensitiveOption);
}

;QRegularExpression Callsign::prefixRegEx()
{
    return QRegularExpression(prefixRegExString(), QRegularExpression::CaseInsensitiveOption);
}

;QString Callsign::callsignRegExString()
{
    return QString("^(([A-Z0-9]+)[\\/])?(([A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])([0-9]|[0-9]+)([A-Z]+))([\\/]([A-Z0-9]+))?");
}

;QString Callsign::prefixRegExString()
{
    //qDebug() << Q_FUNC_INFO;
    // Matches prefix: "^([A-Z0-9]+[\/])?([A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])([0-9]|[0-9]+)([A-Z]+)([\/][A-Z0-9]+)?"
    // E73 prefix is not correctly matched, it is matched as EA400 so simple prefix is not OK

    return QString("^((([A-Z])|([A-Z]{1,2})|([0-9][A-Z])|([A-Z][0-9]))([0-9]*))$");
}

;QString Callsign::getCallsign()
{
    return fullCallsign;
}

;QString Callsign::getHostPrefix()
{
    return hostPrefix;
}

int Callsign::getAreaNumber()
{
    if (prefValid)
        return areaNumber;
    else
        return -1;
}

QString Callsign::getHostPrefixWithoutNumber()
{
    if (prefValid)
        return hostPrefixWithoutAreaNumber;
    else
        return QString();
}

;QString Callsign::getHostPrefixWithDelimiter()
{
    return hostPrefixWithDelimiter;
}

;QString Callsign::getBase()
{
    return base;
}

;QString Callsign::getBasePrefix()
{
    return basePrefix;
}

;QString Callsign::getBasePrefixNumber()
{
    return basePrefixNumber;
}

;QString Callsign::getSuffix()
{
    return suffix;
}

;QString Callsign::getSuffixWithDelimiter()
{
    return suffixWithDelimiter;
}

;QString Callsign::getWPXPrefix()
{
    if ( !isValid() )
        return QString();

    // defined here
    // https://www.cqwpx.com/rules.htm

    // inspired here
    // https://git.fkurz.net/dj1yfk/yfklog/src/branch/develop/yfksubs.pl#L605


    /*********************
     * ONLY BASE CALLSIGN
     *********************/
    if ( getBase() != QString()
        && getHostPrefix() == QString()
        && getSuffix() == QString() )
    {
        // only callsign
        // return callsign prefix + prefix number
        // OL80ABC -> OL80
        // OK1ABC -> OK1
        return getBasePrefix() + getBasePrefixNumber();
    }

    /*********************
     * HOST PREFIX PRESENT
     *********************/
    if ( getHostPrefix() != QString() )
    {
        // callsign has a Host prefix SP/OK1XXX
        // we do not look at the suffix and assign automatically HostPrefix + '0'

        if ( getHostPrefix().back().isDigit() )
            return getHostPrefix();

        return getHostPrefix() + QString("0");
    }

    /****************
     * SUFFIX PRESENT
     ****************/
    if ( getSuffix().length() == 1) // some countries add single numbers as suffix to designate a call area, e.g. /4
    {
        bool isNumber = false;
        (void)suffix.toInt(&isNumber);
        if ( isNumber )
        {
            // callsign suffix is a number
            // VE7ABC/2 -> VE2
            return getBasePrefix() + getSuffix();
        }

        // callsign suffix is not a number
        // OK1ABC/P -> OK1
        return getBasePrefix() + getBasePrefixNumber();
    }

    /***************************
     * SUFFIX PRESENT LENGTH > 1
     ***************************/
    if ( secondarySpecialSuffixes.contains(getSuffix()) )
    {
        // QRP, MM etc.
        // OK1ABC/AM -> OK1
        return getBasePrefix() + getBasePrefixNumber();
    }

    // valid prefix should contain a number in the last position - check it
    // and prefix is not just a number
    // N8ABC/KH9 -> KH9

    bool isNumber = false;
    (void)getSuffix().toInt(&isNumber);

    if ( isNumber )
    {
        // suffix contains 2 and more numbers - ignore it
        return getBasePrefix() + getBasePrefixNumber();
    }

    // suffix is combination letters and digits and last position is a number
    if ( getSuffix().back().isDigit() )
        return getSuffix();

    // prefix does not contain a number - add "0"
    return getSuffix() + QString("0");
}

bool Callsign::isValid()
{
    return valid;
}

bool Callsign::isValidPrefix()
{
    return prefValid;
}

// Based on wiki information
// https://en.wikipedia.org/wiki/Amateur_radio_call_signs
;QStringList Callsign::secondarySpecialSuffixes =
    {
        "A",   // operator at a secondary location registered with the licensing authorities
        "AM",  // aeronautical mobile
        "M",   // mobile operation
        "MM",  // marine mobile
        "P",   // portable operation
        "QRP",  // QRP - unofficial
        "R",    // repeaters
        "B",    // beacon
        "LGT"   // 'LIGHTHOUSE' or 'LIGHTSHIP'  - unofficial
};

// https:// cqwpx.com/rules.htm
/*
1. A PREFIX is the letter/numeral combination which forms the first part of the amateur call.
Examples: N8, W8, WD8, HG1, HG19, KC2, OE2, OE25, LY1000, etc. Any difference in the numbering, lettering,
or order of same shall count as a separate prefix.

A station operating from a DXCC entity different from that indicated by its call sign is required to sign portable.
The portable prefix must be an authorized prefix of the country/call area of operation.
In cases of portable operation, the portable designator will then become
the prefix. Example: N8BJQ operating from Wake Island would sign N8BJQ/KH9 or N8BJQ/NH9.
KH6XXX operating from Ohio must use an authorized prefix for the U.S. 8th district (/W8, /AD8, etc.).

Portable designators without numbers will be assigned a zero (Ø) after the second letter of the portable
designator to form the prefix.
Example: PA/N8BJQ would become PAØ. All calls without numbers will be assigned a zero (Ø) after the first
two letters to form the prefix. Example: XEFTJW would count as XEØ. Maritime mobile, mobile, /A, /E, /J, /P,
or other license class identifiers do not count as prefixes.
*/
