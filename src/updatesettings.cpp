/***************************************************************************
                          updatesettings.h  -  description
                             -------------------
    begin                : mar 2023
    copyright            : (C) 2023 by Jaime Robles
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

#include "updatesettings.h"
#include "global.h"

UpdateSettings::UpdateSettings()
{

}
UpdateSettings::~UpdateSettings()
{

}

bool UpdateSettings::updateFile()
{
    qDebug() << Q_FUNC_INFO ;
    Utilities util(Q_FUNC_INFO);
    QString _oldFile = util.getCfgFile ();
    if (!QFile::exists(_oldFile))
    {
        return false;
    }
    else
    {
        QFile file(_oldFile);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  /* Flawfinder: ignore */
        {
            return false;
        }
        while (!file.atEnd()){
            QByteArray line = file.readLine();
            processConfigLine(line);
        }
        //return file.remove();
    }
    return true;
    //return true;
}

bool UpdateSettings::processConfigLine(const QString &_line)
{
    //qDebug() << Q_FUNC_INFO << _line;
    Utilities util(Q_FUNC_INFO);
    QString line = _line.simplified();

    QStringList values = line.split("=", QT_SKIP);

    if (line.startsWith('#'))
    {
        return true;
    }
    if (!( (line.contains('=')) && (line.contains(';')))){
        return false;
    }

    QString value = values.at(1);
    QString tab = (values.at(0)).toUpper();

    int endValue = value.indexOf(';');
    if (endValue>-1)
    {
        value = value.left(value.length() - (value.length() - endValue));
    }
    QSettings settings(util.getSetFile (), QSettings::IniFormat);

    if (tab == "CALLSIGN")
    {
        settings.beginGroup ("UserData");
        settings.setValue ("Callsign", value);
        settings.endGroup ();
    }else if(tab =="SELECTEDLOG"){
        settings.setValue ("SelectedLog", value.toInt ());
    }else if(tab =="VERSION"){
        settings.setValue ("Version", value);
    }else if (tab == "OPERATORS"){
        settings.beginGroup ("UserData");
        settings.setValue ("Operators", value);
        settings.endGroup ();
    }else if (tab=="CQZ"){
        settings.beginGroup ("UserData");
        settings.setValue ("CQz", (value).toInt());
        settings.endGroup ();
    }else if (tab=="ITUZ"){
        settings.beginGroup ("UserData");
        settings.setValue ("ITUz", (value).toInt());
        settings.endGroup ();
    }else if (tab=="MODES"){
        settings.beginGroup ("BandMode");
        settings.setValue ("Modes", value.split (", ", QT_SKIP));
        settings.endGroup ();
    }else if (tab=="BANDS"){
        //qDebug() << Q_FUNC_INFO << ": " << value;
        settings.beginGroup ("BandMode");
        settings.setValue ("Bands", value.split (", ", QT_SKIP));
        settings.endGroup ();
    }else if (tab=="LOGVIEWFIELDS"){
        settings.setValue ("LogViewFields", value);
    }else if (tab=="REALTIME"){
        settings.beginGroup ("Misc");
        settings.setValue ("RealTime", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="SHOWSECONDS"){
        settings.beginGroup ("Misc");
        settings.setValue ("ShowSeconds", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="UTCTIME"){
        settings.beginGroup ("Misc");
        settings.setValue ("UTCTime", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="ALWAYSADIF"){
        settings.beginGroup ("Misc");
        settings.setValue ("AlwaysADIF", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="USEDEFAULTNAME"){
        settings.beginGroup ("Misc");
        settings.setValue ("UseDefaultName", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="DBPATH"){
        settings.beginGroup ("Misc");
        settings.setValue ("DBPath", value);
        settings.endGroup ();
    }else if (tab=="DEFAULTADIFFILE"){
        settings.beginGroup ("Misc");
        settings.setValue ("DefaultADIFFile", value);
        settings.endGroup ();
    }else if (tab=="IMPERIALSYSTEM"){
        settings.beginGroup ("Misc");
        settings.setValue ("ImperialSystem", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="COMPLETEWITHPREVIOUS"){
        settings.beginGroup ("Misc");
        settings.setValue ("CompleteWithPreviou", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="SENDQSLWHENREC"){
        settings.beginGroup ("Misc");
        settings.setValue ("SendQSLWhenRec", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="MANAGEDXMARATHON"){
        settings.beginGroup ("Misc");
        settings.setValue ("ManageDXMarathon", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="DEBUGLOG"){
        settings.beginGroup ("Misc");
        settings.setValue ("DebugLog", value);
        settings.endGroup ();
    }else if (tab=="SHOWCALLSIGNINSEARCH"){
        settings.beginGroup ("Misc");
        settings.setValue ("ShowCallsignInSearch", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="CHECKNEWVERSIONS"){
        settings.beginGroup ("Misc");
        settings.setValue ("CheckNewVersions", util.trueOrFalse (value));
        settings.endGroup ();
    //}else if (tab=="PROVIDEINFO"){
        //settings.setValue ("ProvideInfo", util.trueOrFalse (value));
    }else if (tab=="SENDEQSLBYDEFAULT"){
        settings.beginGroup ("Misc");
        settings.setValue ("SendEQSLByDefault", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="DUPLICATEDQSOSLOT"){
        if (value.toInt()>=0)
        {
            settings.beginGroup ("Misc");
            settings.setValue ("DuplicatedQSOSlot", value.toInt());
            settings.endGroup ();
        }
    }else if (tab == "CHECKVALIDCALLS"){
        settings.beginGroup ("Misc");
        settings.setValue ("CheckValidCalls", util.trueOrFalse (value));
       settings.endGroup ();
    }else if (tab=="UDPSERVER"){
        settings.beginGroup ("UDPServer");
        settings.setValue ("UDPServer", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="UDPNETWORKINTERFACE"){
        settings.beginGroup ("UDPServer");
        settings.setValue ("UDPNetworkInterface", value);
        settings.endGroup ();
    }else if (tab=="UDPSERVERPORT"){
        settings.beginGroup ("UDPServer");
        settings.setValue ("UDPServerPort", value.toInt());
        settings.endGroup ();
    }else if (tab=="LOGFROMWSJTX"){
        settings.beginGroup ("UDPServer");
        settings.setValue ("LogFromWSJTX", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="LOGAUTOFROMWSJTX"){
        settings.beginGroup ("UDPServer");
        settings.setValue ("LogAutoFromWSJTX", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="REALTIMEFROMWSJTX"){
        settings.beginGroup ("UDPServer");
        settings.setValue ("RealTimeFromWSJTX", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab=="INFOTIMEOUT"){
        settings.beginGroup ("UDPServer");
        settings.setValue ("InfoTimeOut", value.toInt ());
        settings.endGroup ();
    }else if (tab =="NAME"){
        settings.beginGroup ("UserData");
        settings.setValue ("Name", value);
        settings.endGroup ();
    }else if (tab =="ADDRESS1"){
        settings.beginGroup ("UserData");
        settings.setValue ("Address1", value);
        settings.endGroup ();
    }else if (tab =="ADDRESS2"){
        settings.beginGroup ("UserData");
        settings.setValue ("Address2", value);
        settings.endGroup ();
    }else if (tab =="ADDRESS3"){
        settings.beginGroup ("UserData");
        settings.setValue ("Address3", value);
        settings.endGroup ();
    }else if (tab =="ADDRESS4"){
        settings.beginGroup ("UserData");
        settings.setValue ("Address4", value);
        settings.endGroup ();
    }else if (tab =="CITY"){
        settings.beginGroup ("UserData");
        settings.setValue ("City", value);
        settings.endGroup ();
    }else if (tab =="ZIPCODE"){
        settings.beginGroup ("UserData");
        settings.setValue ("ZipCode", value);
        settings.endGroup ();
    }else if (tab =="PROVINCESTATE"){
        settings.beginGroup ("UserData");
        settings.setValue ("ProvinceState", value);
        settings.endGroup ();
    }else if (tab =="COUNTRY"){
        settings.beginGroup ("UserData");
        settings.setValue ("Country", value);
        settings.endGroup ();
    }else if (tab =="POWER"){
        settings.beginGroup ("UserData");
        settings.setValue ("Power", value.toInt ());
        settings.endGroup ();
    }else if (tab =="RIG1"){
        settings.beginGroup ("UserData");
        settings.setValue ("Rig1", value);
        settings.endGroup ();
    }else if (tab =="RIG2"){
        settings.beginGroup ("UserData");
        settings.setValue ("Rig2", value);
        settings.endGroup ();
    }else if (tab =="RIG3"){
        settings.beginGroup ("UserData");
        settings.setValue ("Rig3", value);
        settings.endGroup ();
    }else if (tab =="ANTENNA1"){
        settings.beginGroup ("UserData");
        settings.setValue ("Antenna1", value);
        settings.endGroup ();
    }else if (tab =="ANTENNA2"){
        settings.beginGroup ("UserData");
        settings.setValue ("Antenna2", value);
        settings.endGroup ();
    }else if (tab =="ANTENNA3"){
        settings.beginGroup ("UserData");
        settings.setValue ("Antenna3", value);
        settings.endGroup ();
    }else if (tab =="STATIONLOCATOR"){
        Locator locator;
        if ( locator.isValidLocator(value) )
        {
            settings.beginGroup ("UserData");
            settings.setValue ("StationLocator", value);
            settings.endGroup ();
        }
    }else if (tab =="DXCLUSTERSHOWHF"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowHF", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab =="DXCLUSTERSHOWVHF"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowVHF", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab =="DXCLUSTERSHOWWARC"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowWARC", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab =="DXCLUSTERSHOWWORKED"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowWorked", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab =="DXCLUSTERSHOWCONFIRMED"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowConfirmed", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab =="DXCLUSTERSHOWANN"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowANN", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab =="DXCLUSTERSHOWWWV"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowWWV", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab =="DXCLUSTERSHOWWCY"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterShowWCY", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab  =="DXCLUSTERSERVERTOUSE"){
        settings.beginGroup ("DXCluster");
        //qDebug() << Q_FUNC_INFO << "DXClusterServerToUse: " << value;
        settings.setValue ("DXClusterServerToUse", value);
        settings.endGroup ();
    }else if (tab  =="DXCLUSTERSERVERPORT"){

        QList<QString> clusters;
        clusters.clear ();
        settings.beginGroup ("DXCluster");
        int size = settings.beginReadArray("DXClusterServers");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            clusters.append (settings.value("Server").toString());
            //qDebug() << Q_FUNC_INFO << " - Reading Servers: " << settings.value("Server").toString();
        }
        settings.endArray();
        settings.endGroup ();

        clusters.append (value);
        //qDebug() << Q_FUNC_INFO << " - AddedServer: " << value;

        settings.beginGroup ("DXCluster");
        settings.beginWriteArray("DXClusterServers");
         for (int i = 0; i < clusters.size(); ++i) {
             settings.setArrayIndex(i);
             settings.setValue("Server", clusters.at(i));
             //qDebug() << Q_FUNC_INFO << " - Writting Servers: " << clusters.at(i);
         }
        settings.endArray();
        settings.endGroup ();

    }else if (tab  =="DXCLUSTERSAVE"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterSave", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab  =="DXCLUSTERSENDTOMAP"){
        settings.beginGroup ("DXCluster");
        settings.setValue ("DXClusterSendToMap", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="NEWONECOLOR"){
        settings.beginGroup ("Colors");
        settings.setValue ("NewOneColor", value);
        settings.endGroup ();
    }else if(tab =="NEEDEDCOLOR"){
        settings.beginGroup ("Colors");
        settings.setValue ("NeededColor", value);
        settings.endGroup ();
    }else if(tab =="WORKEDCOLOR"){
        settings.beginGroup ("Colors");
        settings.setValue ("WorkedColor", value);
        settings.endGroup ();
    }else if(tab =="CONFIRMEDCOLOR"){
        settings.beginGroup ("Colors");
        settings.setValue ("ConfirmedColor", value);
        settings.endGroup ();
    }else if(tab =="DEFAULTCOLOR"){
        settings.beginGroup ("Colors");
        settings.setValue ("DefaultColor", value);
        settings.endGroup ();
    }else if(tab =="DARKMODE"){
        settings.beginGroup ("Colors");
        settings.setValue ("DarkMode", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="HAMLIBRIGTYPE"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamLibRigType", value.toInt ());
        settings.endGroup ();
    }else if(tab =="HAMLIBSERIALPORT"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamlibSerialPort", value);
        settings.endGroup ();
    }else if(tab =="HAMLIBSERIALBAUDS"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamlibSerialBauds", value.toInt ());
        settings.endGroup ();
    }else if(tab =="HAMLIB"){
        settings.beginGroup ("HamLib");
        settings.setValue ("Hamlib", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab=="HAMLIBREADONLY"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamlibReadOnly", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="HAMLIBSERIALDATABITS"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamLibSerialDataBits", value.toInt ());
        settings.endGroup ();
    }else if(tab =="HAMLIBSERIALSTOPBITS"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamLibSerialStopBit", value);
        settings.endGroup ();
    }else if(tab =="HAMLIBSERIALFLOWCONTROL"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamLibSerialFlowControl", value);
        settings.endGroup ();
    }else if(tab =="HAMLIBSERIALPARITY"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamLibSerialParity", value);
        settings.endGroup ();
    }else if (tab == "HAMLIBRIGPOLLRATE"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamlibRigPollRate", value.toInt ());
        settings.endGroup ();
    }else if (tab == "HAMLIBNETADDRESS"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamlibNetAddress", value);
        settings.endGroup ();
    }else if (tab == "HAMLIBNETPORT"){
        settings.beginGroup ("HamLib");
        settings.setValue ("HamlibNetPort", value.toInt ());
        settings.endGroup ();
    }else if(tab =="CLUBLOGACTIVE"){
        settings.beginGroup ("ClubLog");
        settings.setValue ("ClubLogActive", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="CLUBLOGREALTIME"){
        settings.beginGroup ("ClubLog");
        settings.setValue ("ClubLogRealTime", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="CLUBLOGPASS"){
        settings.beginGroup ("ClubLog");
        settings.setValue ("ClubLogPass", value);
        settings.endGroup ();
    }else if(tab =="CLUBLOGEMAIL"){
        settings.beginGroup ("ClubLog");
        settings.setValue ("ClubLogEmail", value);
        settings.endGroup ();
    }else if(tab =="EQSLACTIVE"){
        settings.beginGroup ("eQSL");
        settings.setValue ("eQSLActive", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="EQSLCALL"){
        settings.beginGroup ("eQSL");
        settings.setValue ("eQSLCall", value);
        settings.endGroup ();
    }else if(tab =="EQSLPASS"){
        settings.beginGroup ("eQSL");
        settings.setValue ("eQSLPass", value);
        settings.endGroup ();
    }else if(tab =="QRZCOMACTIVE"){
        settings.beginGroup ("QRZcom");
        settings.setValue ("QRZcomActive", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="QRZCOMSUBSCRIBER"){
        settings.beginGroup ("QRZcom");
        settings.setValue ("QRZcomSubscriber", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="QRZCOMUSER"){
        settings.beginGroup ("QRZcom");
        settings.setValue ("QRZcomUser", value);
        settings.endGroup ();
    }else if(tab =="QRZCOMAUTO"){
        settings.beginGroup ("QRZcom");
        settings.setValue ("QRZcomAuto", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="QRZCOMPASS"){
        settings.beginGroup ("QRZcom");
        settings.setValue ("QRZcomPass", value);
        settings.endGroup ();
    }else if(tab =="QRZCOMLOGBOOKKEY"){
        settings.beginGroup ("QRZcom");
        settings.setValue ("QRZcomLogBookKey", value);
        settings.endGroup ();
    }else if(tab =="LOTWACTIVE"){
        settings.beginGroup ("LoTW");
        settings.setValue ("LoTWActive", util.trueOrFalse (value));
        settings.endGroup ();
    }else if(tab =="LOTWPATH"){
        settings.beginGroup ("LoTW");
        settings.setValue ("LoTWPath", value);
        settings.endGroup ();
    }else if(tab =="LOTWUSER"){
        settings.beginGroup ("LoTW");
        settings.setValue ("LoTWUser", value);
        settings.endGroup ();
    }else if(tab =="LOTWPASS"){
        settings.beginGroup ("LoTW");
        settings.setValue ("LoTWPass", value);
        settings.endGroup ();
    }else if(tab =="LATESTBACKUP"){
        QDateTime dtime = QDateTime::fromString(value, "yyyyMMdd-hhmmss");

        settings.setValue ("LatestBackup", dtime.toString(Qt::ISODate));
    }else if(tab =="MAINWINDOWSIZE"){
        QSize windowSize;
        QStringList values;
        values.clear();
        values << value.split("x");
        if ((values.at(0).toInt()>0) && (values.at(1).toInt()>0))
        {
            windowSize.setWidth(values.at(0).toInt());
            windowSize.setHeight(values.at(1).toInt());
        }
        settings.setValue ("MainWindowSize", windowSize);
    }else if(tab =="DELETEALWAYSADIFILE"){
        settings.beginGroup ("Misc");
        settings.setValue ("DeleteAlwaysAdiFile", util.trueOrFalse (value));
        settings.endGroup ();
    }else if (tab == "LATESTBACKUP"){
        settings.setValue ("LatestBackup", value);
    }else{
           //qDebug() << "SetupDialog::processConfigLine: NONE: " << QT_ENDL;
    }
    return true;
}
