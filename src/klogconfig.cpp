/***************************************************************************
                          klogconfig.cpp  -  description
                             -------------------
    begin                : Jun 2021
    copyright            : (C) 2021 by Ladislav Foldyna
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
#include <QSettings>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QSize>
#include "klogconfig.h"
#include "utilities.h"

KlogConfig::KlogConfig()
   : QSettings(Utilities::getHomeDir().append("/klog.ini"), QSettings::IniFormat)
{
}

KlogConfig::~KlogConfig()
{
}

bool KlogConfig::old2newMigrationSuccess()
{
    qDebug() << "KlogConfig::old2newMigrationSuccess - START" << endl;

    QString oldconfig = Utilities::getHomeDir() +
#if defined(Q_OS_WIN)
                ("/klogrc.cfg");
#else
                ("/klogrc");
#endif

    if (!QFile::exists(oldconfig))
    {
        qDebug() << "KlogConfig::old2newMigrationSuccess - " << oldconfig << " does not exists - END" << endl;
        //old config does not exists
        return false;
    }

    QFile file(oldconfig);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "KlogConfig::old2newMigrationSuccess - " << oldconfig << " cannot open - END" << endl;
        //cannot open file
        return false;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line;
        line.clear();
        line  = in.readLine();
        processOldConfigLine(line);
    }

    file.close();

    qDebug() << "KlogConfig::old2newMigrationSuccess - END" << endl;
    return true;
}

void KlogConfig::processOldConfigLine(const QString &_line)
{
    qDebug() << "KlogConfig::ProcessOldConfigLine - ["<< _line << "] - START" << endl;

    QString line = _line.simplified();

    if (!((line.contains('=')) && (line.contains(';'))))
    {
        return; 
    }

    if (line.startsWith('#'))
    {
        return;
    }

    QStringList config_values;
    config_values.clear();
    config_values << line.split("=", QString::SkipEmptyParts);

    if (config_values.length() != 2)
    {
        return;
    }

    QString field = (config_values.at(0)).toUpper();
    QString config_value = config_values.at(1);
    config_value = config_value.remove(";");

    qDebug() << "KlogConfig::ProcessOldConfigLine [1:" << field << ", 2:" << config_value << "]" << endl;

    if (field == "CALLSIGN")  setValue("userdata/callsign", config_value.toUpper());
    else if (field == "OPERATORS") setValue("userdata/operators", config_value);
    else if (field == "CQZ") setValue("userdata/cqz", config_value);
    else if (field == "ITUZ") setValue("userdata/ituz", config_value);
    else if (field == "STATIONLOCATOR") setValue("userdata/stationlocator", config_value);
    else if (field == "NAME") setValue("userdata/name", config_value);
    else if (field == "ADDRESS1") setValue("userdata/address1", config_value);
    else if (field == "ADDRESS2") setValue("userdata/address2", config_value);
    else if (field == "ADDRESS3") setValue("userdata/address3", config_value);
    else if (field == "ADDRESS4") setValue("userdata/address4", config_value);
    else if (field == "ADDRESS4") setValue("userdata/address4", config_value);
    else if (field == "CITY") setValue("userdata/city", config_value);
    else if (field == "ZIPCODE") setValue("userdata/zipcode", config_value);
    else if (field == "PROVINCESTATE") setValue("userdata/provincestate", config_value);
    else if (field == "COUNTRY") setValue("userdata/country", config_value);
    else if (field == "POWER") setValue("userdata/power", config_value.toFloat());
    else if (field == "RIG1") setValue("userdata/rig1", config_value);
    else if (field == "RIG2") setValue("userdata/rig2", config_value);
    else if (field == "RIG3") setValue("userdata/rig3", config_value);
    else if (field == "ANTENNA1") setValue("userdata/antenna1", config_value);
    else if (field == "ANTENNA2") setValue("userdata/antenna2", config_value);
    else if (field == "ANTENNA3") setValue("userdata/antenna3", config_value);
    else if (field == "MODES") setValue("userdata/modes", config_value.split(", ", QString::SkipEmptyParts));
    else if (field == "BANDS") setValue("userdata/bands", config_value.split(", ", QString::SkipEmptyParts));
    else if (field == "REALTIME"  // generic false / true mapping
             || field == "USEDEFAULTNAME"
             || field == "IMPERIALSYSTEM"
             || field == "SENDQSLWHENREC"
             || field == "MANAGEDXMARATHON"
             || field == "SHOWCALLSIGNINSEARCH"
             || field == "CHECKNEWVERSIONS"
             || field == "PROVIDEINFO"
             || field == "ALWAYSADIF"
             || field == "DEBUGLOG"
             || field == "UTCTIME"
             || field == "SENDEQSLBYDEFAULT"
             || field == "COMPLETEWITHPREVIOUS"
             || field == "DELETEALWAYSADIFILE"
             || field == "LATESTBACKUP" ) setValue(field.toLower(), config_value.toLower());
    else if (field == "DXCLUSTERSERVERTOUSE") setValue("DXC/servertouse", config_value);
    else if (field == "DXCLUSTERSAVE") setValue("DXC/save", config_value.toLower());
    else if (field == "DXCLUSTERSHOWHF") setValue("DXC/showhf", config_value.toLower());
    else if (field == "DXCLUSTERSHOWVHF") setValue("DXC/showvhf", config_value.toLower());
    else if (field == "DXCLUSTERSHOWWARC") setValue("DXC/showwarc", config_value.toLower());
    else if (field == "DXCLUSTERSHOWWORKED") setValue("DXC/showworked", config_value.toLower());
    else if (field == "DXCLUSTERSHOWCONFIRMED") setValue("DXC/showconfirmed", config_value.toLower());
    else if (field == "DXCLUSTERSHOWANN") setValue("DXC/showann", config_value.toLower());
    else if (field == "DXCLUSTERSHOWWWV") setValue("DXC/showwwn", config_value.toLower());
    else if (field == "DXCLUSTERSHOWWCY") setValue("DXC/showwcy", config_value.toLower());
    else if (field == "DEFAULTADIFFILE") setValue("defaultadiffile", config_value.toLower());
    else if (field == "STATIONLOCATOR") setValue("stationlocator", config_value);
    else if (field == "POWER") setValue("power", (config_value.toFloat()>0.0f) ? config_value.toFloat(): 0.0);
    else if (field == "DUPLICATEDQSOSLOT") setValue("duplicatedqsoslot", (config_value.toInt()>0) ? config_value.toInt(): 0);
    else if (field == "NEWONECOLOR") setValue("colors/newone", config_value);
    else if (field == "NEEDEDCOLOR") setValue("colors/needed", config_value);
    else if (field == "WORKEDCOLOR") setValue("colors/worked", config_value);
    else if (field == "CONFIRMEDCOLOR") setValue("colors/confirmed", config_value);
    else if (field == "DEFAULTCOLOR") setValue("colors/default", config_value);
    else if (field == "UDPSERVER") setValue("udpserver/active", config_value.toLower());
    else if (field == "UDPNETWORKINTERFACE") setValue("udpserver/interface", config_value);
    else if (field == "UDPSERVERPORT") setValue("udpserver/port", (config_value.toInt()>0 && config_value.toInt()<65536) ? config_value.toInt() : 1234);
    else if (field == "INFOTIMEOUT") setValue("infotimeout", (config_value.toInt()>0 && config_value.toInt()<65536) ? config_value.toInt() : 0);
    else if (field == "LOGFROMWSJTX") setValue("wsjtx/logfromwsjtx", config_value.toLower());
    else if (field == "LOGAUTOFROMWSJTX") setValue("wsjtx/logautofromwsjtx", config_value.toLower());
    else if (field == "REALTIMEFROMWSJTX") setValue("wsjtx/realtimefromwsjtx", config_value.toLower());
    else if (field == "HAMLIBRIGTYPE") setValue("hamlib/rigtype", config_value.toInt());
    else if (field == "HAMLIBSERIALPORT") setValue("hamlib/serialport", config_value);
    else if (field == "HAMLIBSERIALBAUDS") setValue("hamlib/serialbauds", config_value);
    else if (field == "HAMLIBSERIALDATABITS") setValue("hamlib/serialdatabits", config_value);
    else if (field == "HAMLIBSERIALSTOPBITS") setValue("hamlib/serialstopbits", config_value);
    else if (field == "HAMLIBSERIALFLOWCONTROL") setValue("hamlib/serialflowcontrol", config_value);
    else if (field == "HAMLIBSERIALPARITY") setValue("hamlib/serialparity", config_value);
    else if (field == "HAMLIBRIGPOLLRATE") setValue("hamlib/pollrate", config_value.toInt());
    else if (field == "HAMLIB") setValue("hamlib/active", config_value.toLower());
    else if (field == "HAMLIBREADONLY") setValue("hamlib/readonly", config_value.toLower());
    else if (field == "HAMLIBNETADDRESS") setValue("hamlib/netaddress", config_value);
    else if (field == "HAMLIBNETPORT") setValue("hamlib/netport", (config_value.toInt()>0 && config_value.toInt()<65536) ? config_value.toInt() : 1234);
    else if (field == "SELECTEDLOG") setValue("selectedlog", config_value.toInt());
    else if (field == "CLUBLOGACTIVE") setValue("clublog/active", config_value.toLower());
    else if (field == "CLUBLOGREALTIME") setValue("clublog/realtime", config_value.toLower());
    else if (field == "CLUBLOGPASS") setValue("clublog/pass", config_value);
    else if (field == "CLUBLOGEMAIL") setValue("clublog/email", config_value);
    else if (field == "QRZCOMACTIVE") setValue("qrzcom/active", config_value.toInt());
    else if (field == "QRZCOMAUTO") setValue("qrzcom/auto", config_value.toInt());
    else if (field == "QRZCOMPASS") setValue("qrzcom/pass", config_value);
    else if (field == "QRZCOMUSER") setValue("qrzcom/user", config_value);
    else if (field == "QRZCOMLOGBOOKKEY") setValue("qrzcom/logbookkey", config_value);
    else if (field == "EQSLACTIVE") setValue("eqsl/active", config_value.toLower());
    else if (field == "EQSLREALTIME") setValue("eqsl/realtime", config_value.toLower());
    else if (field == "EQSLCALL") setValue("eqsl/call", config_value);
    else if (field == "EQSLPASS") setValue("eqsl/pass", config_value);
    else if (field == "EQSLUSESTATIONCALLSIGN") setValue("eqsl/usestationcallsign", config_value.toLower());
    else if (field == "LOTWACTIVE") setValue("lotw/active", config_value.toLower());
    else if (field == "LOTWPATH") setValue("lotw/path", config_value);
    else if (field == "LOTWUSER") setValue("lotw/user", config_value);
    else if (field == "LOTWPASS") setValue("lotw/pass", config_value);
    else if (field == "MAINWINDOWSIZE") 
    {
        QStringList tmp;
        tmp.clear();
        tmp << config_value.split("x");
        setValue("window/size", QSize(tmp.at(0).toInt(), tmp.at(1).toInt()));
    }

    else
    {
        // the rest of fields "as-in" into generic
        setValue(field.toLower(), config_value);
    }

    qDebug() << "KlogConfig::ProcessOldConfigLine - END" << endl;

    return;
}

