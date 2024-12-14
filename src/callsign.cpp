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
    fullCall(callsign.toUpper()), homeAreaNumber(0), valid(false), prefValid(false)
{
    //qDebug() << Q_FUNC_INFO << ": " << callsign;
    hostAreaNumberExist = false;
    homeAreaNumberExist = false;

    QRegularExpression callsignRE = callsignRegEx();
    QRegularExpressionMatch match = callsignRE.match(fullCall);

    QRegularExpression prefnRE = prefixRegEx();
    QRegularExpressionMatch matchPrefix = prefnRE.match(fullCall);
    //bool hasHost = false;
    clear();
    if ( match.hasMatch() )
    {
        //qDebug() << Q_FUNC_INFO << " - 10";
        //it is a valid callsign
        valid       = true;
        prefValid   = true;
        fullCall    = match.captured("fullcall");
        //qDebug() << Q_FUNC_INFO << " - fullcall: " << fullCall;
        if (match.captured("hostprefix").length()>0)
        {
            //qDebug() << Q_FUNC_INFO << " - 20";
            hostFullPref    = match.captured("hostprefix") + match.captured("hostareanumber");
            hostPref        = match.captured("hostprefix");
            hostAreaNumber  = match.captured("hostareanumber").toInt(&hostAreaNumberExist);
            //qDebug() << Q_FUNC_INFO << " - hostFullPref   : " << hostFullPref;
            //qDebug() << Q_FUNC_INFO << " - hostPref       : " << hostPref;
            //qDebug() << Q_FUNC_INFO << " - hostAreaNumner : " << QString::number(hostAreaNumber);
            //hasHost = true;
        }
        else if (match.captured("").length()>0)
        {
            //qDebug() << Q_FUNC_INFO << " - 30";
            hostFullPref    = match.captured("suffixprefix") + match.captured("suffixareanumber");
            hostPref        = match.captured("suffixprefix");
            hostAreaNumber  = match.captured("suffixareanumber").toInt(&hostAreaNumberExist);

            //qDebug() << Q_FUNC_INFO << " - hostFullPref   : " << hostFullPref;
            //qDebug() << Q_FUNC_INFO << " - hostPref       : " << hostPref;
            //qDebug() << Q_FUNC_INFO << " - hostAreaNumner : " << QString::number(hostAreaNumber);

        }
        //qDebug() << Q_FUNC_INFO << " - 40";
        homeFullCall        = match.captured("homeprefix")+match.captured("homeareanumber")+match.captured("homesuffix");
        homeFullPref        = match.captured("homeprefix")+match.captured("homeareanumber");
        homePref            = match.captured("homeprefix");
        homeAreaNumber      = match.captured("homeareanumber").toInt(&homeAreaNumberExist);
        homeSuffix          = match.captured("homesuffix");
        //if (!hasHost)
        //{
        //    hostPref        = homePref;
        //    hostFullPref    = homeFullPref;
        //    hostAreaNumber  = homeAreaNumber;
        //}

        additionalSuffix        = match.captured("suffix");

        //qDebug() << Q_FUNC_INFO << " - homeFullCall     : " << homeFullCall;
        //qDebug() << Q_FUNC_INFO << " - homeFullPref     : " << homeFullPref;
        //qDebug() << Q_FUNC_INFO << " - homePref         : " << homePref;
        //qDebug() << Q_FUNC_INFO << " - homeAreaNumner   : " << QString::number(homeAreaNumber);
        //qDebug() << Q_FUNC_INFO << " - homeSuffix       : " << homeSuffix;
        //qDebug() << Q_FUNC_INFO << " - AdditionalSuffix : " << additionalSuffix;

    }
    else if (matchPrefix.hasMatch())
    {
        //qDebug() << Q_FUNC_INFO << " - 50";
        prefValid = true;
        hostPref                    = matchPrefix.captured("homeprefix");
        hostFullPref                = matchPrefix.captured("homefullprefix");      // The prefix without the area number
        homePref                    = hostPref;
        homeFullPref                = hostFullPref;
        homeAreaNumber              = matchPrefix.captured("homeareanumber").toInt(&homeAreaNumberExist);      // Just the area number (optional)

        //qDebug() << Q_FUNC_INFO << " - hostFullPref     : " << hostFullPref;
        //qDebug() << Q_FUNC_INFO << " - hostPref         : " << hostPref;
        //qDebug() << Q_FUNC_INFO << " - homeFullPref     : " << homeFullPref;
        //qDebug() << Q_FUNC_INFO << " - homePref         : " << homePref;
        //qDebug() << Q_FUNC_INFO << " - homeAreaNumner   : " << QString::number(homeAreaNumber);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - 60";
        //it is an invalid callsign
        fullCall = QString();
    }

    //qDebug() << Q_FUNC_INFO << " - 100";
}

Callsign::~Callsign(){}

QRegularExpression Callsign::callsignRegEx()
{
    return QRegularExpression(callsignRegExString(), QRegularExpression::CaseInsensitiveOption);
}

QRegularExpression Callsign::prefixRegEx()
{
    // Returns a REGEX that matches a hamradio prefix
    return QRegularExpression(prefixRegExString(), QRegularExpression::CaseInsensitiveOption);
}

QString Callsign::callsignRegExString()
{   // This suffix regex matches QRP, MM... but also prefixes for remote operations.
    // Suffix: ((?<suffix>QRP|MM)|((?<prefix>[A-Z0-9]{1,2})(?<areanumber>[0-9]+)?))

    // Base callsign
    // ^(?<basecall>(?<baseprefixwithnumber>(?<baseprefix>[A-Z][0-9]|[A-Z]|[A-Z]{1,2}|[0-9][A-Z]|E)(?<areanumber>[0-9]+))(?<basesuffix>[A-Z]+))
    // REGEX created with Gemini:
    //return QString("^(?<fullcall>(?<hostprefix>(?:[FGKNRWUGMI]|[A-Z]{2}|\\d[A-Z]|[A-Z]\\d))?\\/?(?<hostareanumber>\\d+)?\\/?(?<homeprefix>(?:[FGKNRWUGMI]|[A-Z]{2}|\\d[A-Z]|[A-Z]\\d))(?<homeareanumber>\\d+)(?<homesuffix>[A-Z]{0,3}))(?:\\/?(?:(?<suffixprefix>(?:[FGKNRWUGMI]|[A-Z]{2}|\\d[A-Z]|[A-Z]\\d))(?<suffixareanumber>\\d+)?|(?<suffix>(?!(?:[FGKNRWUGMI]|[A-Z]{2}|\\d[A-Z]|[A-Z]\\d)\\d)[^\\/\n]+)))$");
    // Host prefix /must end in /
    //^(?<fullcall>(?<hostfullcall>((?<hostpref>[A-Z0-9]{1,2}(?<hostareanumber>[0-9]|[0-9]+)?)\/)?(?<homefullpref>(?<homepref>[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]|[0-9]+))(?<homesuffix>[A-Z]+))(\/(?<additionalsuffix>[A-Z0-9]+))?)
    return QString("^(?<fullcall>((?<hostfullcall>(?<hostfullprefix>(?<hostprefix>[A-Z0-9]{1,2})(?<hostareanumber>[0-9]|[0-9]+)?)\\/)?(?<homefullcall>(?<homefullprefix>(?<homeprefix>[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]|[0-9]+))(?<homesuffix>[A-Z]+)))(\\/(?<additionalsuffix>[A-Z0-9]+))?)");
}

QString Callsign::prefixRegExString()
{
    //qDebug() << Q_FUNC_INFO;

    // Matches prefix: ^(?<homefullprefix>(?<homeprefix>[A-Z]{1,2}|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[0-9][A-Z]|)(?<homeareanumber>[0-9]+)?)

    return QString("^(?<homefullprefix>(?<homeprefix>[A-Z]{1,2}|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[0-9][A-Z]|)(?<homeareanumber>[0-9]+)?)");
}

QString Callsign::getCallsign(){return fullCall;}

QString Callsign::getHostFullPrefix(){return hostFullPref;}
QString Callsign::getHostPrefix(){return hostPref;}
int Callsign::getHostAreaNumber(){return hostAreaNumber;}


QString Callsign::getHomeFullPrefix(){return homeFullPref;}
QString Callsign::getHomePrefix(){return homePref;}
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

    homeFullCall.clear();       // EA4K
    homeFullPref.clear();       // EA4
    homePref.clear();           // EA
    homeAreaNumber      = -1;         // 4
    homeAreaNumberExist = false;
    homeSuffix.clear();

    additionalSuffix.clear();

    valid       = false;         // The entered string is a correct callsign
    prefValid   = false;     // The entered string is a correct prefix
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
