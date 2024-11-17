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
/*

 QRegularExpression re("^(?<date>\\d\\d)/(?<month>\\d\\d)/(?<year>\\d\\d\\d\\d)$");
 QRegularExpressionMatch match = re.match("08/12/1985");
 if (match.hasMatch()) {
     QString date = match.captured("date"); // date == "08"
     QString month = match.captured("month"); // month == "12"
     QString year = match.captured("year"); // year == 1985
 }

*/
//QTest::newRow("fullcall") << "fullcall" << "hostfullcall" << "hostfullpref" << "hostpref" << "hostareanumber" << "homefullcall" << "homefullpref" << "homepref" << "homeareanumber" << "homesuffix" << "additionalsuffix" << true;
/*
    QString fullCall;           // KB1/EA4K/QRP
    QString hostFullCall;       // KB1/EA4K
    QString hostFullPref;       // KB1
    QString hostPref;           // KB
    int hostAreaNumber;         // 1

    QString homeFullCall;       // EA4K
    QString homeFullPref;       // EA4
    QString homePref;           // EA
    int homeAreaNumber;         // 4
    QString homeSuffix;         // K

    QString additionalSuffix;   // QRP

    bool valid;         // The entered strig is a correct callsign
*/
    if ( match.hasMatch() )
    {
        //it is a valid callsign
        valid = true;
        fullCall                = match.captured ("fullcall");
        hostFullCall            = match.captured("hostfullcall");
        hostFullPref            = match.captured("hostfullpref");
        hostPref                = match.captured("hostpref");
        hostAreaNumber          = match.captured("hostareanumber").toInt(&hostAreaNumberExist);

        homeFullCall            = match.captured("homefullcall");
        homeFullPref            = match.captured("homefullpref");
        homePref                = match.captured("homepref");
        homeAreaNumber          = match.captured("homeareanumber").toInt(&homeAreaNumberExist);
        homeSuffix              = match.captured("homesuffix");

        additionalSuffix        = match.captured("additionalsuffix");
    }
    else if (matchPrefix.hasMatch())
    {
        prefValid = true;
        hostPref                    = matchPrefix.captured("homeprefix");
        hostFullPref                = matchPrefix.captured("homefullprefix");      // The prefix without the area number
        homeAreaNumber              = matchPrefix.captured("homeareanumber").toInt(&homeAreaNumberExist);      // Just the area number (optional)
    }
    else
    {
        //it is an invalid callsign
        fullCall = QString();
    }
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

    // Host prefix /must end in /
    //^(?<fullcall>(?<hostfullcall>((?<hostpref>[A-Z0-9]{1,2}(?<hostareanumber>[0-9]|[0-9]+)?)\/)?(?<fullhomepref>(?<homepref>[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]|[0-9]+))(?<basesuffix>[A-Z]+))(\/(?<additionalsuffix>[A-Z0-9]+))?)
    return QString("^(?<fullcall>((?<hostfullcall>(?<hostfullpref>(?<hostpref>[A-Z0-9]{1,2})(?<hostareanumber>[0-9]|[0-9]+)?)\\/)?(?<homefullcall>(?<homefullpref>(?<homepref>[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]|[0-9]+))(?<homesuffix>[A-Z]+)))(\\/(?<additionalsuffix>[A-Z0-9]+))?)");
}

QString Callsign::prefixRegExString()
{
    //qDebug() << Q_FUNC_INFO;

    // Matches prefix: ^(?<homefullpref>(?<homepref>[A-Z]{1,2}|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[0-9][A-Z]|)(?<homeareanumber>[0-9]+)?)

    return QString("^(?<homefullpref>(?<homepref>[A-Z]{1,2}|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[0-9][A-Z]|)(?<homeareanumber>[0-9]+)?)");
}

QString Callsign::getCallsign()
{
    return fullCall;
}


QString Callsign::getHostFullPrefix(){return hostFullPref;}
int Callsign::getHostAreaNuber(){return hostAreaNumber;}

QString Callsign::getHomePrefix(){return homePref;}
QString Callsign::getHomeFullPrefix(){return homeFullPref;}
QString Callsign::getHomeSuffix(){return homeSuffix;}
int Callsign::getHomeAreaNuber(){return homeAreaNumber;}

QString Callsign::getAdditionalSuffix(){ return additionalSuffix;}
QString Callsign::getHostPrefix(){return hostPref;}
{
    return hostPref;
}

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
