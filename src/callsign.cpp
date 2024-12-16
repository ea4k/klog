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
#include "qforeach.h"

Callsign::Callsign(const QString &callsign, QObject *parent) : QObject{parent},
    fullCall(callsign.toUpper()), homeAreaNumber(0), valid(false), prefValid(false)
{
    qDebug() << Q_FUNC_INFO << ": " << callsign;
    hostAreaNumberExist = false;
    homeAreaNumberExist = false;
    QString string2test = fullCall;

    QRegularExpression callsignRE = callsignRegEx();
    QRegularExpressionMatch match = callsignRE.match(string2test);

    QRegularExpression prefnRE = prefixRegEx();
    QRegularExpressionMatch matchPrefix = prefnRE.match(string2test);
    /*
    if (matchPrefix.isValid())
    {
        qDebug() << Q_FUNC_INFO << ": matchPrefix IS VALID";
        //qDebug() << Q_FUNC_INFO << ": " << (matchPrefix.regularExpression()).pattern();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << ": matchPrefix IS NOT VALID";
    }
    if (matchPrefix.hasPartialMatch())
    {
        qDebug() << Q_FUNC_INFO << ": matchPrefix HAS A PARTIAL MATCH ";
    }
    else
    {
        qDebug() << Q_FUNC_INFO << ": matchPrefix HAS NOT A PARTIAL MATCH ";
    }
    */
    //bool hasHost = false;
    clear();
    if ( match.hasMatch() )
    {
        qDebug() << Q_FUNC_INFO << " - 10";
        //it is a valid callsign
        valid       = true;
        prefValid   = true;
        fullCall    = match.captured("fullcall");
        //qDebug() << Q_FUNC_INFO << " - fullcall: " << fullCall;
        hostFullPrefix  = match.captured("hostfullprefix");
        hostPrefix      = match.captured("hostprefix");
        hostAreaNumber  = match.captured("hostareanumber").toInt(&hostAreaNumberExist);
        if (!hostAreaNumberExist)
            hostAreaNumber = -1;

        qDebug() << Q_FUNC_INFO << " - 40";
        homeFullCall        = match.captured("homefullcall");
        homeFullPrefix      = match.captured("homefullprefix");
        homeSpecialPrefix   = match.captured("homespecialprefix");
        homePrefix          = match.captured("homeprefix");
        homeAreaNumber      = match.captured("homeareanumber").toInt(&homeAreaNumberExist);
        if (!homeAreaNumberExist)
            homeAreaNumber = -1;
        homeSuffix          = match.captured("homesuffix");

        additionalSuffix    = match.captured("suffix");
    }
    else if ( matchPrefix.hasMatch() )
    {
        qDebug() << Q_FUNC_INFO << " - 50";

        prefValid = true;
        homeFullPrefix              = matchPrefix.captured("homefullprefix");
        homePrefix                  = matchPrefix.captured("homeprefix");     // The prefix without the area number

        homeAreaNumber              = matchPrefix.captured("homeareanumber").toInt(&homeAreaNumberExist);      // Just the area number (optional)
        if (!homeAreaNumberExist)
            homeAreaNumber = -1;


        qDebug() << Q_FUNC_INFO << " - @ homeFullPref     : " << homeFullPrefix;
        qDebug() << Q_FUNC_INFO << " - @ homePref         : " << homePrefix;
        qDebug() << Q_FUNC_INFO << " - @ homeAreaNumbner   : " << QString::number(homeAreaNumber);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " - 60";
        //it is an invalid callsign
        fullCall = QString();
    }

    qDebug() << Q_FUNC_INFO << " - ************************************************* ";

    qDebug() << Q_FUNC_INFO << " - fullCall         : " << fullCall;
    if (valid)
        qDebug() << Q_FUNC_INFO << " - valid            : TRUE";
    else
        qDebug() << Q_FUNC_INFO << " - valid            : FALSE";

    if (prefValid)
        qDebug() << Q_FUNC_INFO << " - prefValid        : TRUE";
    else
        qDebug() << Q_FUNC_INFO << " - prefValid        : FALSE";

    qDebug() << Q_FUNC_INFO << " - hostFullPrefix     : " << hostFullPrefix;
    qDebug() << Q_FUNC_INFO << " - hostPrefix         : " << hostPrefix;
    qDebug() << Q_FUNC_INFO << " - hostPrefix         : " << hostPrefix;
    qDebug() << Q_FUNC_INFO << " - hostAreaNumner   : " << QString::number(hostAreaNumber);


    qDebug() << Q_FUNC_INFO << " - homeFullCall     : " << homeFullCall;
    qDebug() << Q_FUNC_INFO << " - homeFullPrefix   : " << homeFullPrefix;
    qDebug() << Q_FUNC_INFO << " - homeSpecialPrefix: " << homeFullPrefix;
    qDebug() << Q_FUNC_INFO << " - homePrefix       : " << homePrefix;
    qDebug() << Q_FUNC_INFO << " - homeAreaNumber   : " << QString::number(homeAreaNumber);
    qDebug() << Q_FUNC_INFO << " - homeSuffix       : " << homeSuffix;

    qDebug() << Q_FUNC_INFO << " - AdditionalSuffix : " << additionalSuffix;
    qDebug() << Q_FUNC_INFO << " - ##################################################";
    //qDebug() << Q_FUNC_INFO << " - 100";
}

Callsign::~Callsign(){}

QRegularExpression Callsign::callsignRegEx()
{
    // Returns a REGEX that matches a hamradio callsign
    return QRegularExpression(callsignRegExString(), QRegularExpression::CaseInsensitiveOption);
}

QRegularExpression Callsign::prefixRegEx()
{
    // Returns a REGEX that matches a hamradio prefix
    return QRegularExpression(prefixRegExString(), QRegularExpression::CaseInsensitiveOption);
}

QString Callsign::callsignRegExString()
{
    // This suffix regex matches QRP, MM... but also prefixes for remote operations.
    // Returns a REGEX string that matches a hamradio callsign
    //return QString("^(?<fullcall>((?<hostfullcall>(?<hostfullprefix>(?<hostprefix>[A-Z0-9]{1,2})(?<hostareanumber>[0-9]|[0-9]+)?)\\/)?(?<homefullcall>(?<homefullprefix>(?<homeprefix>3D2|3D6|3D2C|3D2R|SV2A|[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]|[0-9]+))(?<homesuffix>[A-Z]+)))(\\/(?<additionalsuffix>[A-Z0-9]+))?)");
    return QString("^(?<fullcall>((?<hostfullcall>(?<hostfullprefix>(?<hostprefix>[A-Z]{2}|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[0-9][A-Z])(?<hostareanumber>[0-9]+)?)\\/)?(?<homefullcall>(?<homefullprefix>(?<homespecialprefix>3D2|3D6|3D2C|3D2R|SV2A|(?<homeprefix>[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]+)))(?<homesuffix>[A-Z]+)))(\\/(?<additionalsuffix>[A-Z0-9]+))?)");

}

QString Callsign::prefixRegExString()
{
    //qDebug() << Q_FUNC_INFO;
    // Returns a REGEX string that matches a hamradio prefix
    return QString("^(?<homefullprefix>(?<homeprefix>[A-Z]{2}|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[0-9][A-Z])(?<homeareanumber>[0-9]+)?)");
}

QString Callsign::getCallsign(){return fullCall;}

QString Callsign::getHostFullPrefix(){return hostFullPrefix;}
QString Callsign::getHostPrefix(){return hostPrefix;}
int Callsign::getHostAreaNumber(){return hostAreaNumber;}


QString Callsign::getHomeFullPrefix(){return homeFullPrefix;}
QString Callsign::getHomePrefix(){if (homeSpecialPrefix.length()>0) return homeSpecialPrefix; return homePrefix;}
QString Callsign::getHomeSuffix(){return homeSuffix;}
int Callsign::getHomeAreaNumber(){return homeAreaNumber;}

QString Callsign::getAdditionalSuffix(){ return additionalSuffix;}
QString Callsign::getHomeCallsign(){return homeFullCall;}

int Callsign::getAreaNuber()
{
    if (hostAreaNumberExist)
        return hostAreaNumber;
    if (homeAreaNumberExist)
        return homeAreaNumber;
    else
        return -1;
}



bool Callsign::isValid()
{
    return valid;
}

bool Callsign::isValidPrefix()
{
    return prefValid;
}

void Callsign::clear()
{
    fullCall.clear();
    hostFullPref.clear();
    hostPref.clear();
    hostAreaNumber = -1;
    hostAreaNumberExist = false;

    homeFullCall.clear();           // EA4K
    homeFullPref.clear();           // EA4
    homePref.clear();               // EA
    homeAreaNumber      = -1;       // 4
    homeAreaNumberExist = false;
    homeSuffix.clear();

    additionalSuffix.clear();

    valid       = false;            // The entered string is a correct callsign
    prefValid   = false;            // The entered string is a correct prefix
}

// Based on wiki information
// https://en.wikipedia.org/wiki/Amateur_radio_call_signs
QStringList Callsign::secondarySpecialSuffixes =
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
