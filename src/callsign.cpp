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

    bool hostAreaNumberExist1       = false;   // Helper to check if the value exists or not
    bool hostSpecialNumberExist1    = false;   // Helper to check if the value exists or not
    bool homeAreaNumberExist        = false;   // Helper to check if the value exists or not
    bool hostAreaNumberExist2       = false;   // Helper to check if the value exists or not
    bool hostSpecialNumberExist2    = false;   // Helper to check if the value exists or not
    bool homeSpecialAreaNumberExist = false;   // Helper to check if the value exists or not

    // KB1/EA4K/QRP
    QString hostFullPrefix1;                    // KB1
    QString hostNormalPrefix1;                  // KB
    //int     hostAreaNumber1;                    // 1
    QString hostSpecialPrefix1;                 // 3D2 (in 3D2/EA4K)
    int     hostSpecialNumber1;                 // 0 (in 3D20/EA4K)(not sure if this usecase will exist!)

    QString hostFullPrefix2;                    // VU2 in EA4K/VU2
    QString hostNormalPrefix2;                  // VU in EA4K/VU2
    //int     hostAreaNumber2;                    // 2 in EA4K/VU2
    QString hostSpecialPrefix2;                 // 3D2 (in EA4K/3D2)
    //int     hostSpecialNumber2;                 // 0 (in EA4K/3D20)(not sure if this usecase will exist!)
    QString homeNormalPrefix;                   // EA   in EA4K
    QString homeSpecialPrefix;                  // 3D2  in 3D2A
    // int     homeSpecialAreaNumber;              // 0    in 3D20A
    QString suffix;                             // String containing Suffixes like QRP in EA4K/QRP but also 3D2 in EA4K/3D2

    if (fullCall.contains('\\'))                // Lets normalize complx calls
         fullCall.replace('\\', '/');

    QString string2test = fullCall;

    QRegularExpression      callsignRE  = callsignRegEx();
    QRegularExpression      prefnRE     = prefixRegEx();
    QRegularExpressionMatch match       = callsignRE.match(string2test);
    QRegularExpressionMatch matchPrefix = prefnRE.match(string2test);
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
    if ( match.hasMatch() )
    {
        //qDebug() << Q_FUNC_INFO << " - 10";
        //it is a valid callsign
        valid                       = true;
        prefValid                   = true;

        hostFullPrefix1             = match.captured("hostfullprefix1");
        hostNormalPrefix1           = match.captured("hostnormalprefix1");
        int hostAreaNumber1         = match.captured("hostareanumber1").toInt(&hostAreaNumberExist1);
        hostSpecialPrefix1          = match.captured("hostspecialprefix1");
        hostSpecialNumber1          = match.captured("hostspecialareanumber1").toInt(&hostSpecialNumberExist1);

      //qDebug() << Q_FUNC_INFO << " - 40";
        homeCallsign                = match.captured("homecall");
        homeFullPrefix              = match.captured("homefullprefix");
      //qDebug() << Q_FUNC_INFO << " homeFullPrefix: " << homeFullPrefix;
        homeNormalPrefix            = match.captured("homenormalprefix");
        homeSpecialPrefix           = match.captured("homespecialprefix");

        int homeSpecialAreaNumber   = match.captured("homespecialareanumber").toInt(&homeSpecialAreaNumberExist);
        homeAreaNumber              = match.captured("homeareanumber").toInt(&homeAreaNumberExist);


        homeSuffix                  = match.captured("homesuffix");

        suffix                      = match.captured("suffix");

        hostFullPrefix2             = match.captured("hostfullprefix2");
        hostNormalPrefix2           = match.captured("hostnormalprefix2");
        int hostAreaNumber2         = match.captured("hostareanumber2").toInt(&hostAreaNumberExist2);
        hostSpecialPrefix2          = match.captured("hostspecialprefix2");
        int hostSpecialNumber2      = match.captured("hostspecialareanumber2").toInt(&hostSpecialNumberExist2);

        generalSuffix               = match.captured("generalsuffix");

        // Now we have all raw data, let's select what is valid

        if (hostFullPrefix1.length()>0)
        {
            //qDebug() << Q_FUNC_INFO << " - 100";
            hostFullPrefix = hostFullPrefix1;
            if (hostNormalPrefix1.length()>0)
            {
                //qDebug() << Q_FUNC_INFO << " - 110";
                hostPrefix = hostNormalPrefix1;
                if (hostAreaNumberExist1)
                {
                    //qDebug() << Q_FUNC_INFO << " - 120";
                    hostAreaNumber = hostAreaNumber1;
                }
                else
                {
                    hostAreaNumber = -1;
                }
            }
            else if (hostSpecialPrefix1.length()>0)
            {
                //qDebug() << Q_FUNC_INFO << " - 130";
                hostPrefix = hostSpecialPrefix1;
                if (hostSpecialNumberExist1)
                {
                    //qDebug() << Q_FUNC_INFO << " - 140";
                    hostAreaNumber = hostSpecialNumber1;
                }
                else
                {
                    hostAreaNumber = -1;
                }
            }
        }
        else if (hostFullPrefix2.length()>0)
        {
            //qDebug() << Q_FUNC_INFO << " - 200";
            hostFullPrefix = hostFullPrefix2;
            if (hostNormalPrefix2.length()>0)
            {
                //qDebug() << Q_FUNC_INFO << " - 210";
                hostPrefix = hostNormalPrefix2;
                if (hostAreaNumberExist2)
                {
                    //qDebug() << Q_FUNC_INFO << " - 220";
                    hostAreaNumber = hostAreaNumber2;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO << " - 230";
                    hostAreaNumber = -1;
                }
            }
            else if (hostSpecialPrefix2.length()>0)
            {
                //qDebug() << Q_FUNC_INFO << " - 240";
                hostPrefix = hostSpecialPrefix2;
                if (hostSpecialNumberExist2)
                {
                    //qDebug() << Q_FUNC_INFO << " - 250";
                    hostAreaNumber = hostSpecialNumber2;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO << " - 260";
                    hostAreaNumber = -1;
                }
            }
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - 250";
            hostAreaNumber = -1;
        }


        if (homeSpecialPrefix.length()<=0)          // There is no special prefix
        {
            homePrefix = homeNormalPrefix;
        }
        else
        {
            homePrefix = homeSpecialPrefix;
            homeAreaNumber = -1;
            if (homeSpecialAreaNumberExist)         // Not all Special prefixes have area number // 3D2A vs 3D20A
                homeAreaNumber = homeSpecialAreaNumber;
        }

        //qDebug() << Q_FUNC_INFO << " - 300";

        if (generalSuffix.length()>0)               // looking for cases like EA4K/6 where the right call should be EA6/EA4K
        {
            //qDebug() << Q_FUNC_INFO << " - 301";
            bool generalSuffixIsANumber = false;
            int areaN = generalSuffix.toInt(&generalSuffixIsANumber);
            if (generalSuffixIsANumber)
            {
                //qDebug() << Q_FUNC_INFO << " - 320";
                hostPrefix = homePrefix;
                hostAreaNumber = areaN;
                hostFullPrefix = hostPrefix + generalSuffix;
                generalSuffix.clear();
            }
            //qDebug() << Q_FUNC_INFO << " - 399";
        }

      //qDebug() << Q_FUNC_INFO << " - @ hostFullPrefix     : " << hostFullPrefix;
      //qDebug() << Q_FUNC_INFO << " - @ hostPrefix         : " << hostPrefix;
      //qDebug() << Q_FUNC_INFO << " - @ hostAreaNumber     : " << QString::number(hostAreaNumber);
      //qDebug() << Q_FUNC_INFO << " - @ homeCallsign       : " << homeCallsign;
      //qDebug() << Q_FUNC_INFO << " - @ homeFullPrefix     : " << homeFullPrefix;
      //qDebug() << Q_FUNC_INFO << " - @ homePrefix         : " << homePrefix;
      //qDebug() << Q_FUNC_INFO << " - @ homeNormalPrefix   : " << homeNormalPrefix;
      //qDebug() << Q_FUNC_INFO << " - @ homeAreaNumber     : " << QString::number(homeAreaNumber);
      //qDebug() << Q_FUNC_INFO << " - @ homeSuffix         : " << homeSuffix;
      //qDebug() << Q_FUNC_INFO << " - @ suffix             : " << generalSuffix;

    }
    else if ( matchPrefix.hasMatch() )
    {
        //qDebug() << Q_FUNC_INFO << " - 50";

        prefValid = true;
        homeFullPrefix      = matchPrefix.captured("prefix");
        homeSpecialPrefix   = matchPrefix.captured("specialprefix");
        hostSpecialNumber1  = match.captured("specialareanumber").toInt(&hostSpecialNumberExist1);
        homeNormalPrefix    = match.captured("normalprefix");
        homeAreaNumber      = match.captured("areanumber").toInt(&homeAreaNumberExist);

        // Now we have the data, let's select the good one

        if (hostSpecialNumberExist1)
        {
            homePrefix = homeSpecialPrefix;
            homeAreaNumber = hostSpecialNumber1;
        }
        else if (!homeAreaNumberExist)
        {
            homeAreaNumber = 0;
        }


        qDebug() << Q_FUNC_INFO << " - @ homeFullPrefix     : " << homeFullPrefix;
        qDebug() << Q_FUNC_INFO << " - @ homePrefix         : " << homePrefix;
        qDebug() << Q_FUNC_INFO << " - @ homeAreaNumber     : " << QString::number(homeAreaNumber);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - 60";
        //it is an invalid callsign
        clear();
    }
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
    // This REGEX matches full callsigns including prefixes and suffixes
    return QString("^((?<hostfullprefix1>((?<hostspecialprefix1>3D2|3D6|3D2C|3D2R|SV2A)(?<hostspecialareanumber1>[0-9]*))|(?<hostnormalprefix1>B|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[A-Z]{2}|[0-9][A-Z])(?<hostareanumber1>[0-9]*))\\/)?(?<homecall>(?<homefullprefix>(?<homespecialprefix>3D2|3D6|3D2C|3D2R|SV2A)(?<homespecialareanumber>[0-9]*)|(?<homenormalprefix>B|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]+))(?<homesuffix>[A-Z]+))(\\/(?<suffix>(?<hostfullprefix2>(?<hostspecialprefix2>3D2|3D6|3D2C|3D2R|SV2A)(?<hostspecialareanumber2>[0-9]*)|(?<hostnormalprefix2>B|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[A-Z]{2}|[0-9][A-Z])(?<hostareanumber2>[0-9]*))|(?<generalsuffix>[A-Z0-9]*)))?$");
}

QString Callsign::prefixRegExString()
{
    //qDebug() << Q_FUNC_INFO;
    // Returns a REGEX string that matches a hamradio prefix like F in F/EA4K/QRP or EA6 in EA6/EA4K
    return QString("^(?<prefix>(?<specialprefix>3D2|3D6|3D2C|3D2R|SV2A)(?<specialareanumber>[0-9]*)|(?<normalprefix>B|F|G|I|K|M|N|R|U|W|[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<areanumber>[0-9]*))$");
}

    // if getHomeIfEmpty is true and no hostprefix is identified, it will return homePrefix
QString Callsign::getCallsign()                             {return fullCall;}
QString Callsign::getHostFullPrefix(bool getHomeIfEmpty)    {if (getHomeIfEmpty && (hostFullPrefix.length()<=0)) return homeFullPrefix; else return hostFullPrefix;}
QString Callsign::getHostPrefix(bool getHomeIfEmpty)        {if (getHomeIfEmpty && (hostPrefix.length()<=0)) return homePrefix; else return hostPrefix;}
int Callsign::getHostAreaNumber(bool getHomeIfEmpty)        {if (getHomeIfEmpty && (hostAreaNumber<0)) return homeAreaNumber; else return hostAreaNumber;}

QString Callsign::getHomeCallsign()     {return homeCallsign;}
QString Callsign::getHomeFullPrefix()   {return homeFullPrefix;}
QString Callsign::getHomePrefix()       {return homePrefix;}
int Callsign::getHomeAreaNumber()       {return homeAreaNumber;}

QString Callsign::getHomeSuffix()       {return homeSuffix;}
QString Callsign::getSuffix()           {return generalSuffix;}



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
    hostFullPrefix.clear();
    hostPrefix.clear();
    homeSuffix.clear();
    generalSuffix.clear();
    homeCallsign.clear();           // EA4K
    homeFullPrefix.clear();         // EA4
    homePrefix.clear();             // EA

    hostAreaNumber          = -1;
    homeAreaNumber          = -1;       // 4
    valid                   = false;            // The entered string is a correct callsign
    prefValid               = false;            // The entered string is a correct prefix
}

// Based on wiki information
// https://en.wikipedia.org/wiki/Amateur_radio_call_signs
/*
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
*/
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
