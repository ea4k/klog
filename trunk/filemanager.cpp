/***************************************************************************
                          filemanager.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include "filemanager.h"
//#include <QDebug>




FileManager::FileManager(DataProxy *dp)
{
    //qDebug() << "FileManager::FileManager()-1" << endl;

    dataProxy = dp;

    db = new DataBase(0);
    util = new Utilities;
    ignoreUnknownAlways = false;
    world = new World(dataProxy);
    awards = new Awards(dataProxy);


    klogVersion = util->getVersion();
    //dataProxyPrepared = new DataProxy_SQLite(klogVersion);

    noMoreQso = false;

    util = new Utilities();
    hashLogs.clear();

}

/*
FileManager::FileManager(DataProxy *dp, const QString _klogDir)
{
    //qDebug() << "FileManager::FileManager()-2: Dir" << _klogDir << endl;
    dataProxy = dp;
    db = new DataBase(0);

    util = new Utilities;
    ignoreUnknownAlways = false;
    world = new World(dataProxy, klogDir);
    awards = new Awards(dataProxy);

    klogVersion = util->getVersion();
    dataProxyPrepared = new DataProxy_SQLite(klogVersion);

    noMoreQso = false;
    hashLogs.clear();
    klogDir = util->getHomeDir();

}
*/
FileManager::FileManager(DataProxy *dp, const QString _klogDir, const QString _softVersion)
//FileManager::FileManager(const QString _klogDir, const QString _softVersion, DataBase _db)
{
    //qDebug() << "FileManager::FileManager()-3: Dir(2)" << _klogDir << endl;
    dataProxy = dp;
    db = new DataBase(0);

    klogVersion = _softVersion;
    //dataProxyPrepared = new DataProxy_SQLite(klogVersion);
    util = new Utilities;
    util->setVersion(klogVersion);

    klogDir = _klogDir;
    ignoreUnknownAlways = false;
    world = new World(dataProxy, klogDir);
    awards = new Awards(dataProxy);

    noMoreQso = false;  
    hashLogs.clear();

}

FileManager::~FileManager()
{

}

bool FileManager::checkADIFValidFormat(const QStringList _qs)
{
    QStringList qs = _qs;


    if (qs.size()!= 2)
    {
        //qDebug() << "FileManager::checkADIFValidFormat-0 (not two) " << endl;
        return false;
    }

    QString q0 = qs.at(0);
    QString q1 = qs.at(1);

   //qDebug() << "FileManager::checkADIFValidFormat: " << qs.at(0) << endl;
   //qDebug() << "FileManager::checkADIFValidFormat: " << qs.at(1) << endl;

    int len = 0;
    int i = (qs.at(0)).count(":");       

    if (i == 2)
    { // DATE:8:D / 20141020
        len = (q0.section(':', 1, 1)).toInt();
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        len = (q0.section(':', 1, 1)).toInt();
    }
    else
    {
        //qDebug() << "FileManager::checkADIFValidFormat-1 " << endl;
        return false;
    }

    //i = (qs.at(0)).indexOf(":");
    //i = (qs.at(0)).length() - i -1;

    //qDebug() << "i = " << QString::number(i) << "/" << qs.at(0) << endl;


    //len = ( (qs.at(0)).right(i)).toInt();
    //qDebug() << "len = " << QString::number(len) << endl;

    if ( (q1).length() != len )
    {
        //qDebug() << "FileManager::checkADIFValidFormat-2: " << (qs.at(1)) << " - " << QString::number((qs.at(1)).length()) << "/" << QString::number(len) << endl;
        return false;
    }

    if (len <= 0)
    {
        //qDebug() << "FileManager::checkADIFValidFormat-3 " << endl;
        return false;
    }

    //qDebug() << "FileManager::checkADIFValidFormat-4 (true)" << endl;

    return true;


}

bool FileManager::adifLogExport(const QString& _fileName, const int _logN)
{
  //qDebug() << "FileManager::adifLogExport" << endl;


    return adifLogExportToFile(_fileName, _logN, false, false, false);

}

int FileManager::adifLoTWLogExport(const QString& _fileName, const int _logN)
{
    //qDebug() << "FileManager::adifLoTWLogExport: " << _fileName << endl;

    if (!dataProxy->doesThisLogExist(_logN))
    {
        //qDebug() << "FileManager::adifLoTWLogExport - The log does not exist" << endl;
        return -1;
    }

    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return -2;

    QString stationCallToUse = QString();
    QStringList stationCallSigns;
    stationCallSigns.clear();
    stationCallSigns << tr("NONE");
    stationCallSigns << dataProxy->getStationCallSignsFromLog(_logN);
    bool callsignTyped = false;

    if (stationCallSigns.length()>1)
    {
        QString msg = QString(tr("The log that you have selected contains more than just one station callsign.") + "\n\n" + tr("Please select the station callsing you want to export the log from:"));

        bool ok;
        stationCallToUse = QInputDialog::getItem(this, tr("Station Callsign:"),
                                             msg, stationCallSigns, 0, false, &ok);

        if (ok && !stationCallToUse.isEmpty())
        {
            //qDebug() << "FileManager::adifLoTWLogExport: StatioNCallsign: " << stationCallToUse << endl;
        }
        else
        {

            //getText(QWidget * parent,
            //        const QString & title, const QString & label, QLineEdit::EchoMode mode = QLineEdit::Normal,
            //        const QString & text = QString(), bool * ok = 0, Qt::WindowFlags flags = 0,
            //        Qt::InputMethodHints inputMethodHints = Qt::ImhNone) [static]


            stationCallToUse = (QInputDialog::getText(this, tr("Define Station Callsign"),
                                                     tr("You have selected no callsign. KLog will export QSOs without a station callsign defined and those with the call you are entering here.") + "\n\n" + tr("Enter the station callsign to use for this log or leave it empty for QSO without station callsign defined:"), QLineEdit::Normal,
                                                     "", &ok)).toUpper();


             if (ok)
             {
                callsignTyped = true;

             }
             else
             {
                 QMessageBox msgBox;
                 msgBox.setIcon(QMessageBox::Warning);

                 QString aux = QString(tr("No station callsign has been selected and threfore no log will be exported") );
                 msgBox.setText(aux);
                 msgBox.setStandardButtons(QMessageBox::Ok);
                 int ret = msgBox.exec();
                 switch (ret) {
                   case QMessageBox::Ok:
                       // Ok was clicked
                         return -3;
                       break;
                   default:
                         // should never be reached
                       break;
                 }

             }
        }
    }



    QTextStream out(&file);
    int numberOfQsos = dataProxy->getHowManyQSOInLog(_logN);
    int step = util->getProgresStepForDialog(numberOfQsos);
    int i = 0;

    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::ApplicationModal);

    out << "ADIF v2.2.7 Export from KLog\nhttp://www.klog.xyz/klog\n<PROGRAMVERSION:" << QString::number(klogVersion.length()) << ">" << klogVersion << "\n<PROGRAMID:7>KLOG" << endl;
    out << "<APP_KLOG_QSOS:" << QString::number((QString::number(numberOfQsos)).length()) << ">" << QString::number(numberOfQsos) << endl;

    QDateTime dateTime = (QDateTime::currentDateTime()).toUTC();

    out << "<APP_KLOG_LOG_DATE_EXPORT:" << QString::number((dateTime.toString("yyyyMMdd-hhmm")).length()) << ">" << dateTime.toString("yyyyMMdd-hhmm") << endl;

    out << "<EOH>" << endl;

    QString queryString = QString("SELECT call, freq, bandid, band_rx, freq_rx, modeid, qso_date, time_on, prop_mode, sat_name, lotw_qsl_sent, station_callsign FROM log WHERE lognumber='%1'").arg(_logN);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    //qDebug() << "FileManager::adifLoTWLogExport: " << query.lastQuery() << endl;
    if (!sqlOK)
    {
        //qDebug() << "FileManager::adifLoTWLogExport: Query Error"  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return -4;
    }

    QSqlRecord rec = query.record();
    int nameCol;
    QString aux, aux2;
    QString bandst, bandrxst;
    bool propsat;
    while ( (query.next()))
    {
        //qDebug() << "FileManager::adifLoTWLogExport: Start of While"  << endl;
        if (query.isValid())
        {
            nameCol = rec.indexOf("station_callsign");
            aux = (query.value(nameCol)).toString();
            if ( ( (stationCallToUse == "NONE") && (aux.length() <3) ) || (aux == stationCallToUse)  )
            //if ( (aux.length()>2) || ((aux == stationCallToUse) && callsignTyped)  )
            { // We are only exporting the QSO from the appropriate station callsign or with empty stationcallsigns but we will add the one entered by the user.
                nameCol = rec.indexOf("lotw_qsl_sent");
                aux = (query.value(nameCol)).toString();
                if (aux == "Q")
                {
                    //qDebug() << "FileManager::adifLoTWLogExport: Start of isValid"  << endl;
                    propsat = false;    // Reset the QSO in case it is a Satellite QSO

                    nameCol = rec.indexOf("call");
                    aux = (query.value(nameCol)).toString();
                    aux = util->checkAndFixASCIIinADIF(aux);
                    //qDebug() << "FileManager::adifLoTWLogExport: " << QString::number(nameCol) << "/" << aux1 << endl;
                    if (aux.length()>0)
                    {
                        out << "<CALL:" << QString::number(aux.length()) << ">" << aux<< " ";
                    }
                    //qDebug() << "FileManager::adifLoTWLogExport: CALL"  << endl;

                    nameCol = rec.indexOf("qso_date");
                    aux = (query.value(nameCol)).toString();
                    aux = util->checkAndFixASCIIinADIF(aux);
                    if ((aux.length()) == 10){
                        aux.remove(QChar('-'), Qt::CaseInsensitive);
                        aux.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<QSO_DATE:" << QString::number(aux.length()) << ">" << aux  << " ";
                        }
                    }

                    nameCol = rec.indexOf("time_on");
                    aux = (query.value(nameCol)).toString();
                    aux = util->checkAndFixASCIIinADIF(aux);
                    //qDebug() << "FileManager::adifLoTWLogExportToFile-time_on: "  << aux1 << endl;
                    if ( ((aux.length()) == 5) || ((aux.length()) == 8) ){
                        aux.remove(QChar(':'), Qt::CaseInsensitive);

                        out << "<TIME_ON:" << QString::number(aux.length()) << ">" << aux  << " ";
                    }

                    nameCol = rec.indexOf("bandid");
                    aux = (query.value(nameCol)).toString();
                    //qDebug() << "FileManager::adifLoTWLogExportToFile-Band-1: "  << aux << endl;
                    aux = util->checkAndFixASCIIinADIF(aux);
                    //qDebug() << "FileManager::adifLoTWLogExportToFile-Band-2: "  << aux << endl;
                    //aux = db->getBandNameFromID2(aux.toInt());
                    aux = dataProxy->getNameFromBandId(aux.toInt());
                    //qDebug() << "FileManager::adifLoTWLogExportToFile-Band-3: "  << aux << endl;


                    if (dataProxy->getIdFromBandName(aux)>=0)
                    {
                        out << "<BAND:" << QString::number(aux.length()) << ">" << aux  << " ";
                        bandst = aux;
                    }

                    nameCol = rec.indexOf("band_rx");
                    aux = (query.value(nameCol)).toString();
                    aux = util->checkAndFixASCIIinADIF(aux);
                    aux = dataProxy->getNameFromBandId(aux.toInt());

                    if ( dataProxy->getIdFromBandName(aux)>=0)
                    {
                        out << "<BAND_RX:" << QString::number(aux.length()) << ">" << aux  << " ";
                        QString bandrxst = aux;
                    }
                    //qDebug() << "FileManager::adifLoTWLogExport: BAND_RX"  << endl;

                    nameCol = rec.indexOf("modeid");
                    aux = (query.value(nameCol)).toString();
                    aux = util->checkAndFixASCIIinADIF(aux);
                    // get SubModeId to check if it is the same or not from modeid
                    aux2 = dataProxy->getSubModeFromId(aux.toInt());
                    //aux = db->getModeNameFromID2(aux.toInt());
                    aux = dataProxy->getNameFromSubMode(aux2);

                    //qDebug() << "FileManager::adifLoTWLogExportToFile - MODE aux2:  " << aux2 << endl;
                    //qDebug() << "FileManager::adifLoTWLogExportToFile - MODE aux:  " << aux << endl;

                    if ((aux.length()>1) && (dataProxy->getIdFromModeName(aux)>=0))
                    {
                        //haveMode = true;
                        out << "<MODE:" << QString::number(aux.length()) << ">" << aux  << " ";
                    }

                    if ((aux != aux2) && (aux.length()>1) && (dataProxy->getSubModeIdFromSubMode(aux2)>=0) )
                    {
                        //haveMode = true;
                        out << "<SUBMODE:" << QString::number(aux2.length()) << ">" << aux2  << " ";
                    }

                    //qDebug() << "FileManager::adifLoTWLogExport: SUBMODE: " << aux2  << endl;

                    nameCol = rec.indexOf("freq");
                    aux = (query.value(nameCol)).toString();
                    //qDebug() << "FileManager::adifLoTWLogExportToFile FREQ1: "  << aux1 << endl;
                    aux = util->checkAndFixASCIIinADIF(aux);

                    if ((aux.length())>0){
                        //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
                        if (dataProxy->getBandIdFromFreq(aux.toDouble()) == dataProxy->getIdFromBandName(bandst))
                        //if (db->isThisFreqInBand(bandst, aux))
                        {
                           out << "<FREQ:" << QString::number(aux.length()) << ">" << aux  << " ";
                        }

                    }
                    //qDebug() << "FileManager::adifLoTWLogExport: FREQ"  << endl;
                    nameCol = rec.indexOf("freq_rx");
                    aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
                    if ((aux.length())>0){
                        //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
                        if (dataProxy->getBandIdFromFreq(aux.toDouble()) == dataProxy->getIdFromBandName(bandrxst))
                        //if (db->isThisFreqInBand(bandrxst, aux))
                        {
                           out << "<FREQ_RX:" << QString::number(aux.length()) << ">" << aux  << " ";
                        }
                    }
                    //qDebug() << "FileManager::adifLoTWLogExport: FREQ_RX"  << endl;

                    nameCol = rec.indexOf("prop_mode");
                    aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
                    if ((aux.length())>1){
                        out << "<PROP_MODE:" << QString::number(aux.length()) << ">" << aux  << " ";
                        if (aux == "SAT")
                        {
                            propsat = true;
                        }

                    }
                    //qDebug() << "FileManager::adifLoTWLogExport: PROP_MODE"  << endl;

                    nameCol = rec.indexOf("sat_name");
                    aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
                    if ((aux.length())>0){
                        out << "<SAT_NAME:" << QString::number(aux.length()) << ">" << aux  << " ";

                        if (!propsat)
                        {
                             out << "<PROP_MODE:3>SAT ";
                        }
                    }
                    //qDebug() << "FileManager::adifLoTWLogExport: SAT_NAME"  << endl;

                    out << "<EOR> " << endl;

                    i++;
                    if (( (i % step ) == 0) )
                    { // To update the speed I will only show the progress once each X QSOs
                       //qDebug() << "FileManager::adifLoTWLogExport: MOD 0 - i = " << QString::number(i)  << endl;

                        aux = tr("Exporting LoTW ADIF file...") + "\n" + tr(" QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);

                       progress.setLabelText(aux);
                       progress.setValue(i);

                    }
                    else
                    {
                       //qDebug() << "FileManager::adifLoTWLogExport: Mod: "<< QString::number(i) << " mod " << QString::number(step) << " = " << QString::number(i % step) << endl;

                    }
                }
            }
        }
        //qDebug() << "FileManager::adifLoTWLogExport: End Of Valid"  << endl;
    }
    //qDebug() << "FileManager::adifLoTWLogExport: End of While " << endl;




    return i;
}

bool FileManager::adifLogExportToFile(const QString& _fileName, const int _logN, bool justMarked, bool _qslRequested , bool _lotw)
{
    // If _logN = 0, then we will export ALL the logs.
  //qDebug() << "FileManager::adifLogExportToFile: " << _fileName << endl;

    bool exportJustMarkedQSO = justMarked;
    bool marked = false;
    bool exportOnlyQSLRequested = _qslRequested;
    //bool haveMode = false;
    noMoreQso = false;

    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    QString aux1, aux2, queryString, bandst, bandrxst; // bandst & bandrxst are to check if the band is properly defined
    int nameCol = 0;
    QSqlQuery query1;
    int numberOfQsos = 0;
    int currentQso = 0;
    //bool noMoreQso = false;
    int step = 1;
    bool propsat=false; // Just to check if we have added the prop_mode needed by LOTW when SAT QSO
    //bool bandOK = false; // Just to check if the band is properly defined
    bool exportAll = false;
    if (_logN <=0)
    {
        exportAll = true;
    }
    else
    {
        exportAll = false;
    }

    bandst = QString();
    bandrxst = QString();

    if (exportJustMarkedQSO)
    {
        //TODO: Count the marked QSO and adjust the numberOfQsos
        numberOfQsos = 0;
        QSqlQuery query;
        if (exportAll)
        {
            queryString = QString("SELECT marked FROM log");
        }
        else
        {
            queryString = QString("SELECT marked FROM log WHERE lognumber='%1'").arg(_logN);
        }

        bool sqlOK = query.exec(queryString);
        if (!sqlOK)
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        }
        QSqlRecord rec = query.record();

        while ( (query.next())) {
            if (query.isValid())
            {
                nameCol = rec.indexOf("marked");
                aux1 = (query.value(nameCol)).toString();
                if (aux1 == "X")
                {
                    numberOfQsos++;
                }
                else
                {
                }
            }
            else
            {
            }
        }
        //qDebug() << "FileManager::adifLogExportToFile -  numberOfQsos = " << QString::number(numberOfQsos)<< endl;
    }
    else
    {
        if (exportAll)
        {
            aux1 = "SELECT count(id) FROM log";
        }
        else
        {
            aux1 = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(_logN);
        }

        bool sqlOK = query1.exec(aux1);
        if (!sqlOK)
        {
            emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
        }
        query1.next();

        if (query1.isValid())
        {
            numberOfQsos = (query1.value(0)).toInt();
        }
        //qDebug() << "FileManager::adifLogExportToFile -  numberOfQsos = " << QString::number(numberOfQsos)<< endl;
    }

    //qDebug() << "FileManager::adifLogExportToFile END -  numberOfQsos = " << QString::number(numberOfQsos) << endl;

    step = util->getProgresStepForDialog(numberOfQsos);
    //step = getProgresStepForDialog(numberOfQsos);
    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::ApplicationModal);

    out << "ADIF v2.2.7 Export from KLog\nhttp://www.klog.xyz/klog\n<PROGRAMVERSION:" << QString::number(klogVersion.length()) << ">" << klogVersion << "\n<PROGRAMID:7>KLOG" << endl;
    out << "<APP_KLOG_QSOS:" << QString::number((QString::number(numberOfQsos)).length()) << ">" << QString::number(numberOfQsos) << endl;

    QDateTime dateTime = (QDateTime::currentDateTime()).toUTC();

    out << "<APP_KLOG_LOG_DATE_EXPORT:" << QString::number((dateTime.toString("yyyyMMdd-hhmm")).length()) << ">" << dateTime.toString("yyyyMMdd-hhmm") << endl;

    out << "<EOH>" << endl;


    if (exportAll)
    {
        queryString = QString("SELECT * FROM log");
    }
    else
    {
        queryString = QString("SELECT * FROM log WHERE lognumber='%1'").arg(_logN);
    }

    QSqlQuery query;
    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    //QSqlQuery query("SELECT * FROM log");
    QSqlRecord rec = query.record();


   //qDebug() << "FileManager::adifLogExportToFile -  before the While" << endl;
    while ( (query.next()) && (!noMoreQso) )
    {
        marked = false;

        if (query.isValid())
        {
            if (exportJustMarkedQSO == true)
            {
                nameCol = rec.indexOf("marked");
                aux1 = (query.value(nameCol)).toString();
                if (aux1 == "X")
                {
                    //qDebug() << "FileManager::adifLogExportToFile: MARKED: " << aux1 << endl;

                    currentQso++;

                    nameCol = rec.indexOf("call");
                    aux1 = (query.value(nameCol)).toString();

                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    //qDebug() << "FileManager::adifLogExportToFile: " << QString::number(nameCol) << "/" << aux1 << endl;
                    if (aux1.length()>0)
                    {
                        out << "<CALL:" << QString::number(aux1.length()) << ">" << aux1<< " ";
                    }

                    nameCol = rec.indexOf("qso_date");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<QSO_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("qso_date_off");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");
                        if (util->isValidDate(date))
                        {
                            out << "<QSO_DATE_OFF:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("time_on");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    //qDebug() << "FileManager::adifLogExportToFile-time_on: "  << aux1 << endl;
                    if ( ((aux1.length()) == 5) || ((aux1.length()) == 8) ){
                        aux1.remove(QChar(':'), Qt::CaseInsensitive);

                        out << "<TIME_ON:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }


                    nameCol = rec.indexOf("time_off");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    //qDebug() << "FileManager::adifLogExportToFile: time_off-682" << QString::number(nameCol) << "/" << aux1 << endl;
                    if ( ((aux1.length()) == 5) || ((aux1.length()) == 8) ){
                        aux1.remove(QChar(':'), Qt::CaseInsensitive);
                        out << "<TIME_OFF:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        //qDebug() << "FileManager::adifLogExportToFile: time_off exported-682" << endl;
                    }

                    nameCol = rec.indexOf("bandid");
                    aux1 = (query.value(nameCol)).toString();
                   //qDebug() << "FileManager::adifLogExportToFile: bandid1: " << aux1 << endl;

                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                   //qDebug() << "FileManager::adifLogExportToFile: bandid2: " << aux1 << endl;
                    aux1 = dataProxy->getNameFromBandId(aux1.toInt());
                    //aux1 = db->getBandNameFromID2(aux1.toInt());
                   //qDebug() << "FileManager::adifLogExportToFile: bandid3: " << aux1 << endl;

                    if (dataProxy->getIdFromBandName(aux1)>=0)
                    {
                        out << "<BAND:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        bandst = aux1;
                    }


                    nameCol = rec.indexOf("band_rx");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);

                    if ( (0 < aux1.toInt()) && (aux1.toInt() < 30) && (aux1.length()>0) && (dataProxy->getIdFromBandName(aux1)>=0))
                    {
                        aux1 = dataProxy->getNameFromBandId(aux1.toInt());
                        //aux1 = db->getBandNameFromID2(aux1.toInt());
                        out << "<BAND_RX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        bandrxst = aux1;
                    }

                    nameCol = rec.indexOf("modeid");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    // get SubModeId to check if it is the same or not from modeid
                    aux2 = dataProxy->getSubModeFromId(aux1.toInt());
                    aux1 = dataProxy->getNameFromModeId(aux1.toInt());
                    //aux1 = db->getModeNameFromID2(aux1.toInt());

                    //qDebug() << "FileManager::adifLogExportToFile - MODE aux2:  " << aux2 << endl;
                    //qDebug() << "FileManager::adifLogExportToFile - MODE aux1:  " << aux1 << endl;

                    if ((aux1.length()>1) && (dataProxy->getIdFromModeName(aux1)>=0))
                    {
                        //haveMode = true;
                        out << "<MODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    if ((aux1 != aux2) && (aux2.length()>1) && (dataProxy->getSubModeIdFromSubMode(aux2)>=0) )
                    {
                        //haveMode = true;
                        out << "<SUBMODE:" << QString::number(aux2.length()) << ">" << aux2  << " ";
                    }


                    nameCol = rec.indexOf("srx");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0)
                    {
                        out << "<SRX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    nameCol = rec.indexOf("srx_string");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SRX_STRING:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    nameCol = rec.indexOf("stx");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<STX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("stx_string");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<STX_STRING:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("cqz");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())>0) && (0 < aux1.toInt()) && (aux1.toInt() < CQZones+1) ){
                        out << "<CQZ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ituz");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())>0) && (0 < aux1.toInt()) && (aux1.toInt() < ITUZones+1) ){
                        out << "<ITUZ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    //qDebug() << "FileManager::adifLogExportToFile: DXCC - Now..."  << endl;

                    nameCol = rec.indexOf("dxcc");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<DXCC:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        //qDebug() << "FileManager::adifLogExportToFile: DXCC " << aux1 << endl;
                    }
                    //qDebug() << "FileManager::adifLogExportToFile: DXCC - Exported!"  << endl;

                    nameCol = rec.indexOf("address");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<ADDRESS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("age");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<AGE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("cnty");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CNTY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("comment");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<COMMENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("a_index");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<A_INDEX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ant_az");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<ANT_AZ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ant_el");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<ANT_EL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ant_path");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<ANT_PATH:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("arrl_sect");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<ARRL_SECT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("checkcontest");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CHECKCONTEST:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("class");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CLASS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("cont");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CONT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("contacted_op");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CONTACTED_OP:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("contest_id");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CONTEST_ID:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("points");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<APP_KLOG_POINTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("multiplier");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<APP_KLOG_MULTIPLIER:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("transmitterid");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<APP_KLOG_TRX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("country");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<COUNTRY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("credit_submitted");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CREDIT_SUBMITTED:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("credit_granted");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<CREDIT_GRANTED:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("distance");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<DISTANCE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("dark_doc");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<DARK_DOK:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("eq_call");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<EQ_CALL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("email");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        if (aux1.contains("@") && (aux1.contains(".")))
                        {
                           out << "<EMAIL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("eqsl_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<EQSL_QSLRDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("eqsl_qslsdate");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<EQSL_QSLSDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("eqsl_qsl_rcvd");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if (  ((aux1.length())==1) && (aux1!="N") ) {
                        out << "<EQSL_QSL_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    nameCol = rec.indexOf("eqsl_qsl_sent");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if (  ((aux1.length())==1) && (aux1!="N") ){
                        out << "<EQSL_QSL_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("fists");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<FISTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("fists_cc");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<FISTS_CC:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("force_init");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<FORCE_INIT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("freq");
                    aux1 = (query.value(nameCol)).toString();
                    //qDebug() << "FileManager::adifLogExportToFile FREQ1: "  << aux1 << endl;
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    //qDebug() << "FileManager::adifLogExportToFile FREQ1.1: "  << aux1 << endl;
                    if ((aux1.length())>0){
                        //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
                        if (dataProxy->getBandIdFromFreq(aux1.toDouble()) == dataProxy->getIdFromBandName(bandst))
                        //if (db->isThisFreqInBand(bandst, aux1))
                        {
                           out << "<FREQ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }

                    }
                    nameCol = rec.indexOf("freq_rx");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
                        if (dataProxy->getBandIdFromFreq(aux1.toDouble()) == dataProxy->getIdFromBandName(bandrxst))
                        //if (db->isThisFreqInBand(bandrxst, aux1))
                        {
                           out << "<FREQ_RX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("guest_op");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<GUEST_OP:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("hrdlog_qso_upload_date");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<HRDLOG_QSO_UPLOAD_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("hrdlog_qso_upload_status");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())==1)  && ((aux1!="Y") || (aux1!="N") || (aux1!="M")) ){
                        out << "<HRDLOG_QSO_UPLOAD_STATUS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("gridsquare");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<GRIDSQUARE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_gridsquare");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_GRIDSQUARE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_antenna");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_ANTENNA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_dxcc");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_DXCC:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_fists");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_FISTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("iota");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    //qDebug() << "FileManager::adifLogExportToFile (IOTA): " << aux1 << endl;

                    if (((aux1.length())>=4) && ((aux1.length())<=6)){
                    //if ((aux1.length())==6){
                        out << "<IOTA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("iota_island_id");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    //qDebug() << "FileManager::adifLogExportToFile (IOTA_ID): " << aux1 << endl;

                    if ((aux1.length())>0){
                        out << "<IOTA_ISLAND_ID:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_iota");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if (((aux1.length())>=4) && ((aux1.length())<=6)){
                    //if ((aux1.length())==6){
                        out << "<MY_IOTA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    nameCol = rec.indexOf("my_iota_island_id");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_IOTA_ISLAND_ID:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }


                    nameCol = rec.indexOf("k_index");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<K_INDEX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_itu_zone");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_ITU_ZONE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("lat");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<LAT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("lon");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<LON:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_lat");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_LAT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_lon");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_LON:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("lotw_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<LOTW_QSLRDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("lotw_qslsdate");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<LOTW_QSLSDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }


                    nameCol = rec.indexOf("lotw_qsl_rcvd");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())==1) && (aux1!="N") ) {
                        out << "<LOTW_QSL_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    nameCol = rec.indexOf("lotw_qsl_sent");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())==1)  && (aux1!="N") ){
                        out << "<LOTW_QSL_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("clublog_qso_upload_date");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<CLUBLOG_QSO_UPLOAD_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("clublog_qso_upload_status");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())==1)  && ((aux1!="Y") || (aux1!="N") || (aux1!="M")) ){
                        out << "<CLUBLOG_QSO_UPLOAD_STATUS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("qrzcom_qso_upload_date");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<QRZCOM_QSO_UPLOAD_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("qrzcom_qso_upload_status");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())==1)  && ((aux1!="Y") || (aux1!="N") || (aux1!="M")) ){
                        out << "<QRZCOM_QSO_UPLOAD_STATUS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("max_bursts");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MAX_BURSTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ms_shower");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MS_SHOWER:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_city");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_CITY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_cnty");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_CNTY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_country");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_COUNTRY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_cq_zone");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_CQ_ZONE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_name");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_NAME:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("name");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<NAME:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("operator");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<OPERATOR:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("station_callsign");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<STATION_CALLSIGN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    nameCol = rec.indexOf("owner_callsign");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<OWNER_CALLSIGN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_postal_code");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_POSTAL_CODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_rig");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_RIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_sig");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_SIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_sota_ref");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_SOTA_REF:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_postal_code");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_POSTAL_CODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_state");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_STATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("state");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<STATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_street");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_STREET:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("notes");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        aux1.replace("\n", "---");
                        out << "<NOTES:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("nr_bursts");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<NR_BURSTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("nr_pings");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<NR_PINGS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("pfx");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<PFX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    nameCol = rec.indexOf("precedence");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<PRECEDENCE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }


                    nameCol = rec.indexOf("public_key");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<PUBLIC_KEY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("qslmsg");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<QSLMSG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<QSLRDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("qslsdate");
                    aux1 = (query.value(nameCol)).toString();
                    aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length()) == 10){
                        aux1.remove(QChar('-'), Qt::CaseInsensitive);
                        aux1.remove(QChar('/'), Qt::CaseInsensitive);
                        QDate date = QDate::fromString(aux1, "yyyyMMdd");

                        if (util->isValidDate(date))
                        {
                            out << "<QSLSDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("qsl_rcvd");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if (((aux1.length())==1) && (aux1!="N") ){
                        out << "<QSL_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";

                        nameCol = rec.indexOf("qsl_rcvd_via");
                        aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                        if ((aux1.length())==1){
                            out << "<QSL_RCVD_VIA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    nameCol = rec.indexOf("qsl_sent");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if (((aux1.length())==1) && (aux1!="N") ){
                        out << "<QSL_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        nameCol = rec.indexOf("qsl_sent_via");
                        aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                        if ((aux1.length())==1)  {
                            out << "<QSL_SENT_VIA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }

                    }

                    nameCol = rec.indexOf("qsl_via");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<QSL_VIA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("qso_complete");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<QSO_COMPLETE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("qso_random");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<QSO_RANDOM:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("qth");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<QTH:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("rst_sent");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<RST_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("rst_rcvd");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<RST_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("region");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<REGION:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("rig");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<RIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("rx_pwr");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if (((aux1.length())>0) && (aux1.toDouble()>0) ){
                        out << "<RX_PWR:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("tx_pwr");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( ((aux1.length())>0) && (aux1.toDouble()>0)) {
                        out << "<TX_PWR:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("prop_mode");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>1){
                        out << "<PROP_MODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        propsat = true;
                    }


                    nameCol = rec.indexOf("sat_mode");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SAT_MODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("sat_name");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SAT_NAME:" << QString::number(aux1.length()) << ">" << aux1  << " ";

                        if (!propsat)
                        {
                             out << "<PROP_MODE:3>SAT ";
                        }
                    }

                    nameCol = rec.indexOf("sfi");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SFI:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("sig");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("sig_info");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SIG_INFO:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("silent_key");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SILENT_KEY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("skcc");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SKCC:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("sota_ref");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SOTA_REF:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("state");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<STATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("swl");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<SWL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ten_ten");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<TEN_TEN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ten_ten");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<TEN_TEN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("uksmg");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<UKSMG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("ve_prov");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<VE_PROV:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_usaca_counties");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_USACA_COUNTIES:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("usaca_counties");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<USACA_COUNTIES:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("vucc_grids");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<VUCC_GRIDS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("my_vucc_grids");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<MY_VUCC_GRIDS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                    nameCol = rec.indexOf("web");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<WEB:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    if (exportAll)
                    {
                        nameCol = rec.indexOf("lognumber");
                        aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                        if ((aux1.length())>0){
                            out << "<APP_KLOG_LOGN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                        }
                    }

                    out << "<EOR> " << endl;
                }
                else
                {
                    //qDebug() << "FileManager::adifLogExportToFile: NOT MARKED" << endl;
                    //marked = false;
                    // Find something that bypass the while without breaking it all

                }
            }
            else
            {
                if(exportOnlyQSLRequested)
                {

                    nameCol = rec.indexOf("qsl_sent");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ( aux1 !="R" )
                    {
                        goto EndOfWhile;
                    }
                }

                currentQso++;

                nameCol = rec.indexOf("call");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                //qDebug() << "FileManager::adifLogExportToFile: " << QString::number(nameCol) << "/" << aux1 << endl;
                if (aux1.length()>0)
                {
                    out << "<CALL:" << QString::number(aux1.length()) << ">" << aux1 << " ";
                }
                //qDebug() << "FileManager::adifLogExportToFile before 30 " << endl;
                nameCol = rec.indexOf("qso_date");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<QSO_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("qso_date_off");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<QSO_DATE_OFF" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }
qDebug() << "FileManager::adifLogExportToFile before 50 " << endl;
                nameCol = rec.indexOf("time_on");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                //qDebug() << "FileManager::adifLogExportToFile-time_on: "  << aux1 << endl;
                if ( ((aux1.length()) == 5) || ((aux1.length()) == 8) ){
                    aux1.remove(QChar(':'), Qt::CaseInsensitive);
                    out << "<TIME_ON:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("time_off");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                //qDebug() << "FileManager::adifLogExportToFile: time_off-1489" << QString::number(nameCol) << "/" << aux1 << endl;
                if ( ((aux1.length()) == 5) || ((aux1.length()) == 8) ){
                    aux1.remove(QChar(':'), Qt::CaseInsensitive);
                    out << "<TIME_OFF:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    //qDebug() << "FileManager::adifLogExportToFile: time_off-1489-exported"  << endl;
                }

                nameCol = rec.indexOf("bandid");
                aux1 = (query.value(nameCol)).toString();
               //qDebug() << "FileManager::adifLogExportToFile: bandid21: " << aux1 << endl;
                aux1 = util->checkAndFixASCIIinADIF(aux1);
               //qDebug() << "FileManager::adifLogExportToFile: bandid22: " << aux1 << endl;

                //aux1 = db->getBandNameFromID2(aux1.toInt());
                aux1 = dataProxy->getNameFromBandId(aux1.toInt());
               //qDebug() << "FileManager::adifLogExportToFile: bandid23: " << aux1 << endl;

                //TODO: If the DB has some blank fields (bandid or whatever, they will be exported as "-1" so not valid.
                // Check how to avoid exporting wrong data.

                if (dataProxy->getIdFromBandName(aux1)>=0)
                {
                    out << "<BAND:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    bandst = aux1;
                }


                nameCol = rec.indexOf("band_rx");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);

qDebug() << "FileManager::adifLogExportToFile before 60 " << endl;
                if ( (0 < aux1.toInt()) && (aux1.toInt() < 30) && (aux1.length()>0) && (dataProxy->getIdFromBandName(aux1)>=0) )  {
                    //aux1 = db->getBandNameFromID2(aux1.toInt());
                    aux1 = dataProxy->getNameFromBandId(aux1.toInt());

                    out << "<BAND_RX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    bandrxst = aux1;
                    /*queryString = QString("SELECT name FROM band WHERE id='%1'").arg(aux1);
                    query1.exec(queryString);
                    query1.next();
                    if (query1.isValid())
                    {
                        aux1 = (query1.value(0)).toString();
                        out << "<BAND_RX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                    */
                }

                nameCol = rec.indexOf("modeid");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);

                // get SubModeId to check if it is the same or not from modeid
                aux2 = dataProxy->getSubModeFromId(aux1.toInt());
                aux1 = dataProxy->getNameFromSubMode(aux2);
                //aux1 = db->getModeNameFromID2(aux1.toInt());

                //qDebug() << "FileManager::adifLogExportToFile - MODE2 aux2:  " << aux2 << endl;
                //qDebug() << "FileManager::adifLogExportToFile - MODE2 aux1:  " << aux1 << endl;

                if ((aux1.length()>1) && (dataProxy->getIdFromModeName(aux1)>=0))
                {
                    out << "<MODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    //haveMode = true;
                }


                if ((aux1 != aux2) && (aux2.length()>1) && (dataProxy->getSubModeIdFromSubMode(aux2)>=0))
                {
                    //haveMode = true;
                    out << "<SUBMODE:" << QString::number(aux2.length()) << ">" << aux2  << " ";
                }

qDebug() << "FileManager::adifLogExportToFile before 70 " << endl;
                nameCol = rec.indexOf("srx");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SRX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("srx_string");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SRX_STRING:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("stx");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<STX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("stx_string");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<STX_STRING:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("cqz");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())>0) && (0 < aux1.toInt()) && (aux1.toInt() < CQZones+1) ){
                    out << "<CQZ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("ituz");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())>0) && (0 < aux1.toInt()) && (aux1.toInt() < ITUZones+1) ){
                    out << "<ITUZ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                //qDebug() << "FileManager::adifLogExportToFile - DXCC to be exported:  " << endl;
                nameCol = rec.indexOf("dxcc");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                //qDebug() << "FileManager::adifLogExportToFile - DXCC: " << aux1 << endl;
                //if ((aux1.length())>0){
                if ( ((aux1.length())>0) && (0 < aux1.toInt()) && (aux1.toInt() < DXCCEntities + 5) ){
                    out << "<DXCC:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    //qDebug() << "FileManager::adifLogExportToFile - DXCC  in the if"  << endl;
                }
                //qDebug() << "FileManager::adifLogExportToFile - DXCC alreadyexported:  "  << endl;

                nameCol = rec.indexOf("address");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<ADDRESS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
qDebug() << "FileManager::adifLogExportToFile before 80 " << endl;
                nameCol = rec.indexOf("age");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<AGE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("cnty");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CNTY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("comment");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<COMMENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("a_index");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<A_INDEX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("ant_az");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<ANT_AZ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("ant_el");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<ANT_EL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("ant_path");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<ANT_PATH:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
qDebug() << "FileManager::adifLogExportToFile before 90 " << endl;
                nameCol = rec.indexOf("arrl_sect");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<ARRL_SECT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("checkcontest");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CHECKCONTEST:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("class");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CLASS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("cont");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CONT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("contacted_op");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CONTACTED_OP:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("contest_id");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CONTEST_ID:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("points");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<APP_KLOG_POINTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("multiplier");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<APP_KLOG_MULTIPLIER:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("transmitterid");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<APP_KLOG_TRX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

            //qDebug() << "FileManager::adifLogExportToFile before 100 " << endl;
                nameCol = rec.indexOf("country");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<COUNTRY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("credit_submitted");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CREDIT_SUBMITTED:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("credit_granted");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<CREDIT_GRANTED:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("distance");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<DISTANCE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("dark_doc");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<DARK_DOK:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("eq_call");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<EQ_CALL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("email");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0)
                {
                    if (aux1.contains("@") && (aux1.contains(".")))
                    {
                        out << "<EMAIL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                }

                nameCol = rec.indexOf("eqsl_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<EQSL_QSLRDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("eqsl_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<EQSL_QSLSDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }
                //qDebug() << "FileManager::adifLogExportToFile before eqsl_qsl_rcvd: " << endl;
                nameCol = rec.indexOf("eqsl_qsl_rcvd");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if (  ((aux1.length())==1) && (aux1!="N") ) {
                    out << "<EQSL_QSL_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                //qDebug() << "FileManager::adifLogExportToFile before eqsl_qsl_sent: " << endl;
                nameCol = rec.indexOf("eqsl_qsl_sent");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if (  ((aux1.length())==1) && (aux1!="N") ){
                    out << "<EQSL_QSL_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("fists");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<FISTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("fists_cc");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<FISTS_CC:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                //qDebug() << "FileManager::adifLogExportToFile before force_init: " << endl;
                nameCol = rec.indexOf("force_init");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<FORCE_INIT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                //qDebug() << "FileManager::adifLogExportToFile before freq: " << endl;
                nameCol = rec.indexOf("freq");
                //qDebug() << "FileManager::adifLogExportToFile before freq: nameCol: " << QString::number(nameCol) << endl;
                aux1 = (query.value(nameCol)).toString();
                //qDebug() << "FileManager::adifLogExportToFile FREQ2: "  << aux1 << endl;
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                //qDebug() << "FileManager::adifLogExportToFile FREQ2.1: "  << aux1 << endl;
                if ((aux1.length())>0){
                    if (dataProxy->getBandIdFromFreq(aux1.toDouble()) == dataProxy->getIdFromBandName(bandst))
                    //if (db->isThisFreqInBand(bandst, aux1))
                    {
                        out << "<FREQ:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                }

                nameCol = rec.indexOf("freq_rx");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    if (dataProxy->getBandIdFromFreq(aux1.toDouble()) == dataProxy->getIdFromBandName(bandst))
                    //if (db->isThisFreqInBand(bandst, aux1))
                    {
                        out << "<FREQ_RX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                }

                nameCol = rec.indexOf("guest_op");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<GUEST_OP:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("hrdlog_qso_upload_date");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<HRDLOG_QSO_UPLOAD_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("hrdlog_qso_upload_status");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())==1)  && ((aux1!="Y") || (aux1!="N") || (aux1!="M")) ){
                    out << "<HRDLOG_QSO_UPLOAD_STATUS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("gridsquare");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<GRIDSQUARE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_gridsquare");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_GRIDSQUARE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_antenna");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_ANTENNA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("iota");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                //qDebug() << "FileManager::adifLogExportToFile (IOTA2): " << aux1 << endl;
                if (((aux1.length())>=4) && ((aux1.length())<=6)){
                //if ((aux1.length())==6){
                    out << "<IOTA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("iota_island_id");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                //qDebug() << "FileManager::adifLogExportToFile (IOTA_ID2): " << aux1 << endl;

                if ((aux1.length())>0){
                    out << "<IOTA_ISLAND_ID:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_iota");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if (((aux1.length())>=4) && ((aux1.length())<=6)){
                //if ((aux1.length())==6){
                    out << "<MY_IOTA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("my_iota_island_id");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_IOTA_ISLAND_ID:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }


                nameCol = rec.indexOf("k_index");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<K_INDEX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_itu_zone");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_ITU_ZONE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("lat");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<LAT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("lon");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<LON:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_lat");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_LAT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_lon");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_LON:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("lotw_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<LOTW_QSLRDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("lotw_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<LOTW_QSLSDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("lotw_qsl_rcvd");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())==1) && (aux1!="N") ) {
                    out << "<LOTW_QSL_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("lotw_qsl_sent");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())==1)  && (aux1!="N") ){
                    out << "<LOTW_QSL_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }                

                nameCol = rec.indexOf("clublog_qso_upload_date");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<CLUBLOG_QSO_UPLOAD_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("clublog_qso_upload_status");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())==1)  && ((aux1!="Y") || (aux1!="N") || (aux1!="M")) ){
                    out << "<CLUBLOG_QSO_UPLOAD_STATUS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("qrzcom_qso_upload_status");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<QRZCOM_QSO_UPLOAD_DATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("qrzcom_qso_upload_status");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())==1)  && ((aux1!="Y") || (aux1!="N") || (aux1!="M")) ){
                    out << "<QRZCOM_QSO_UPLOAD_STATUS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("max_bursts");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MAX_BURSTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("ms_shower");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MS_SHOWER:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_city");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_CITY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_cnty");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_CNTY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_country");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_COUNTRY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_cq_zone");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_CQ_ZONE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_name");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_NAME:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("name");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<NAME:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("operator");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<OPERATOR:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("station_callsign");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<STATION_CALLSIGN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("owner_callsign");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<OWNER_CALLSIGN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_postal_code");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_POSTAL_CODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_rig");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_RIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_sig");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_SIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_sig_info");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_SIG_INFO:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_state");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_STATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("state");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<STATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_street");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_STREET:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("notes");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<NOTES:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("nr_bursts");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<NR_BURSTS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("nr_pings");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<NR_PINGS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("pfx");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<PFX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }
                nameCol = rec.indexOf("precedence");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<PRECEDENCE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("prop_mode");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>1){
                    out << "<PROP_MODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("public_key");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<PUBLIC_KEY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("qslmsg");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<QSLMSG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("qslrdate");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<QSLRDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("qslsdate");
                aux1 = (query.value(nameCol)).toString();
                aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length()) == 10){
                    aux1.remove(QChar('-'), Qt::CaseInsensitive);
                    aux1.remove(QChar('/'), Qt::CaseInsensitive);
                    QDate date = QDate::fromString(aux1, "yyyyMMdd");

                    if (util->isValidDate(date))
                    {
                        out << "<QSLSDATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                nameCol = rec.indexOf("qsl_rcvd");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if (((aux1.length())==1) && (aux1!="N") ){
                    out << "<QSL_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";

                    nameCol = rec.indexOf("qsl_rcvd_via");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())==1){
                        out << "<QSL_RCVD_VIA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }


                nameCol = rec.indexOf("qsl_sent");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if (((aux1.length())==1) && (aux1!="N") ){
                    out << "<QSL_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    nameCol = rec.indexOf("qsl_sent_via");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())==1)  {
                        out << "<QSL_SENT_VIA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }

                }

                nameCol = rec.indexOf("qsl_via");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<QSL_VIA:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("qso_complete");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<QSO_COMPLETE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("qso_random");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<QSO_RANDOM:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("qth");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<QTH:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("rst_sent");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<RST_SENT:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("rst_rcvd");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<RST_RCVD:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("region");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<REGION:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("rig");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<RIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("rx_pwr");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if (((aux1.length())>0) && (aux1.toDouble()>0)){
                    out << "<RX_PWR:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("tx_pwr");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ( ((aux1.length())>0) && (aux1.toDouble()>0)) {
                    out << "<TX_PWR:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("sat_mode");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SAT_MODE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("sat_name");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SAT_NAME:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("sfi");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SFI:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("sig");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SIG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("sig_info");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SIG_INFO:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("silent_key");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SILENT_KEY:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("skcc");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SKCC:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("sota_ref");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SOTA_REF:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("state");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<STATE:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("swl");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<SWL:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("ten_ten");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<TEN_TEN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("uksmg");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<UKSMG:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("ve_prov");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<VE_PROV:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_usaca_counties");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_USACA_COUNTIES:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("usaca_counties");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<USACA_COUNTIES:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("vucc_grids");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<VUCC_GRIDS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("my_vucc_grids");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<MY_VUCC_GRIDS:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                nameCol = rec.indexOf("web");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                if ((aux1.length())>0){
                    out << "<WEB:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                }

                if (exportAll)
                {
                    nameCol = rec.indexOf("lognumber");
                    aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                    if ((aux1.length())>0){
                        out << "<APP_KLOG_LOGN:" << QString::number(aux1.length()) << ">" << aux1  << " ";
                    }
                }

                out << "<EOR> " << endl;
                using namespace std;
                EndOfWhile:
                ;
            }

            if (( (currentQso % step )== 0) )
            { // To update the speed I will only show the progress once each X QSOs
                aux1 = tr("Writing ADIF file...\n QSO: ")  + QString::number(currentQso) + "/" + QString::number(numberOfQsos);
                progress.setLabelText(aux1);
                progress.setValue(currentQso);
            }

        if ( progress.wasCanceled() )
        {
            QMessageBox msgBox;
            QString aux = QString(tr("You have canceled the file export. The file will be removed and no data will be exported.") + "\n" + tr("Do you still want to cancel?"));
            msgBox.setText(aux);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            switch (ret) {
              case QMessageBox::Yes:
                  // Yes was clicked
                    noMoreQso = true;
                  break;
              case QMessageBox::No:
                    // No Save was clicked
                  break;
              default:
                    // should never be reached
                  break;
            }

        }
        else
        {}
    } // Closes the isValid
        else
        {}
    } //Closes the While

   //qDebug() << "FileManager::adifLogExportToFile -  after the While" << endl;
    progress.setValue(numberOfQsos);

    if (noMoreQso)
    {
    //TODO: Remove the file (_fileName)
        file.remove();

        return true;
    }
    else
    {
        return false;
    }
    return true;
}

bool FileManager::cabrilloLogExport(const QString& _fileName, const QString _contestType, const int logNconst)
{
    //qDebug() << "FileManager::cabrilloLogExport" << endl;

    return cabrilloLogExportToFile(_fileName, logNconst);

}

bool FileManager::cabrilloLogExportCQWWToFile(const QString& _fileName, const int logNconst)
{
    //qDebug() << "FileManager::cabrilloLogExportCQWWToFile" << _fileName, endl;
/*
START-OF-LOG:
ARRL-SECTION:
CALLSIGN: N6TW
CATEGORY: SINGLE-OP ALL LOW
CLAIMED-SCORE:
CLUB:
CONTEST: CQ-WW-SSB
NAME:
ADDRESS:
ADDRESS:
OPERATORS: [required for multi-op stations]
SOAPBOX: [add lines if needed]
SOAPBOX:
QSO:  3799 PH 2000-11-26 0711 N6TW          59  03     K9QZO         59  04     0
QSO: 14256 PH 2000-11-26 0711 N6TW          59  03     P29AS         59  28     0
QSO: 21250 PH 2000-11-26 0711 N6TW          59  03     4S7TWG        59  22     0
QSO: 28530 PH 2000-11-26 0711 N6TW          59  03     JT1FAX        59  23     0
QSO:  7250 PH 2000-11-26 0711 N6TW          59  03     WA6MIC        59  03     0
END-OF-LOG:

This is a template for the QSO portion of the log.
                          --------info sent------- -------info rcvd--------
QSO: freq  mo date       time call          rst exch   call          rst exch   t
QSO: ***** ** yyyy-mm-dd nnnn ************* nnn ****** ************* nnn ****** n
QSO:  3799 PH 2000-11-26 0711 N6TW          59  03     JT1Z          59  23     0

000000000111111111122222222223333333333444444444455555555556666666666777777777788
123456789012345678901234567890123456789012345678901234567890123456789012345678901

Official Categories

This is the list of categories the robot will accept. This will assure we get
your category correct and avoid the errors of previous years

CATEGORY: SINGLE-OP ALL HIGH
CATEGORY: SINGLE-OP 160M HIGH
CATEGORY: SINGLE-OP 80M HIGH
CATEGORY: SINGLE-OP 40M HIGH
CATEGORY: SINGLE-OP 20M HIGH
CATEGORY: SINGLE-OP 15M HIGH
CATEGORY: SINGLE-OP 10M HIGH
CATEGORY: SINGLE-OP ALL LOW
CATEGORY: SINGLE-OP 160M LOW
CATEGORY: SINGLE-OP 80M LOW
CATEGORY: SINGLE-OP 40M LOW
CATEGORY: SINGLE-OP 20M LOW
CATEGORY: SINGLE-OP 15M LOW
CATEGORY: SINGLE-OP 10M LOW
CATEGORY: SINGLE-OP ALL QRP
CATEGORY: SINGLE-OP 160M QRP
CATEGORY: SINGLE-OP 80M QRP
CATEGORY: SINGLE-OP 40M QRP
CATEGORY: SINGLE-OP 20M QRP
CATEGORY: SINGLE-OP 15M QRP
CATEGORY: SINGLE-OP 10M QRP
CATEGORY: SINGLE-OP-ASSISTED ALL
CATEGORY: SINGLE-OP-ASSISTED 160M
CATEGORY: SINGLE-OP-ASSISTED 80M
CATEGORY: SINGLE-OP-ASSISTED 40M
CATEGORY: SINGLE-OP-ASSISTED 20M
CATEGORY: SINGLE-OP-ASSISTED 15M
CATEGORY: SINGLE-OP-ASSISTED 10M
CATEGORY: MULTI-ONE
CATEGORY: MULTI-TWO
CATEGORY: MULTI-MULTI
CATEGORY: CHECKLOG
*/

QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    QString aux1, queryString;
    int nameCol = 0;
    QSqlQuery query, query1;
    int numberOfQsos = 0;
    int currentQso = 0;
    int currentLog = logNconst;

    noMoreQso = false;

    queryString = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(currentLog);
    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.next();
    if (query.isValid())
    {
        numberOfQsos = (query.value(0)).toInt();
    }

    QProgressDialog progress(tr("Writing Cabrillo file..."), tr("Abort writing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::WindowModal);

    out << "START-OF-LOG:" << endl;
    out << "ARRL-SECTION: " << endl;
    out << "CALLSIGN:" << endl;
    out << "CATEGORY:" << endl;
    out << "CLAIMED-SCORE:" << endl;
    out << "CLUB: " << endl;
    out << "CONTEST:" << endl;
    out << "NAME: " << endl;
    out << "ADDRESS:" << endl;
    out << "ADDRESS:" << endl;
    out << "ADDRESS:" << endl;
    out << "ADDRESS:" << endl;
    out << "OPERATORS:" << endl;
    out << "SOAPBOX:" << endl;
    out << "SOAPBOX:" << endl;
    out << "SOAPBOX:" << endl;
    out << "SOAPBOX:" << endl;


    //TODO: When the software supports several log, this should be taken into account in the following query.
    queryString = "SELECT freq, bandid, modeid, qso_date, time_on, station_callsign, rst_sent, rst_rcvd, call, stx FROM log";
    sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    QSqlRecord rec = query.record();


    while ( (query.next()) && (!noMoreQso) ) {

        if (query.isValid())
        {
/*          This is a template for the QSO portion of the log.
                                      --------info sent------- -------info rcvd--------
            QSO: freq  mo date       time call          rst exch   call          rst exch   t
            QSO: ***** ** yyyy-mm-dd nnnn ************* nnn ****** ************* nnn ****** n
            QSO:  3799 PH 2000-11-26 0711 N6TW          59  03     JT1Z          59  23     0
            000000000111111111122222222223333333333444444444455555555556666666666777777777788
            123456789012345678901234567890123456789012345678901234567890123456789012345678901
*/
            currentQso++;
            out << "QSO: " << endl;

            nameCol = rec.indexOf("freq");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
            if ((aux1.length())>0){
                out << aux1.rightJustified(5, ' ', true) << " ";
            }
            else // if we dont have the freq but the band
            {
                nameCol = rec.indexOf("bandid");
                aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
                queryString = QString("SELECT cabrillo FROM band WHERE id='%1'").arg(aux1);
                sqlOK = query1.exec(queryString);
                if (!sqlOK)
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
                }
                query1.next();
                if (query1.isValid())
                {
                    aux1 = (query1.value(0)).toString();
                    out << aux1.rightJustified(5, ' ', true) << " ";
                }
                else
                { // if we dont even have the band, we add the freq "00000"
                    out << "00000" << " ";
                }
            }

            nameCol = rec.indexOf("modeid");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
            queryString = QString("SELECT cabrillo FROM mode WHERE id='%1'").arg(aux1);
            sqlOK = query1.exec(queryString);
            if (!sqlOK)
            {
                emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
            }
            query1.next();
            if (query1.isValid())
            {
                aux1 = (query1.value(0)).toString();
                out << aux1  << " ";
            }
            else
            {
                out << "NO"  << " ";
            }

            nameCol = rec.indexOf("qso_date");
            aux1 = (query.value(nameCol)).toString();
            aux1 = util->checkAndFixASCIIinADIF(aux1);
            if ((aux1.length()) == 10){
                aux1.remove(QChar('-'), Qt::CaseInsensitive);
                aux1.remove(QChar('/'), Qt::CaseInsensitive);
                QDate date = QDate::fromString(aux1, "yyyyMMdd");

                if (util->isValidDate(date))
                {
                    out << aux1  << " ";
                }
                else
                {
                    out << "0000-00-00"  << " ";
                }
            }

            nameCol = rec.indexOf("time_on");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
            //qDebug() << "FileManager::adifLogExportToFile-time_on: "  << aux1 << endl;
            if ( ((aux1.length()) == 5) || ((aux1.length()) == 8) ){
                aux1.remove(QChar(':'), Qt::CaseInsensitive);
                //out << aux1.resize(2) << " ";
            }
            else
            {
                out << "0000" << " ";
            }

            nameCol = rec.indexOf("station_callsign");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
            if ( (0 <= aux1.length()) && (aux1.length() <= 14) )
            {
                out << aux1.rightJustified(13, ' ', true) << " ";
            }
            else
            {
                out << QString("NOCAL").rightJustified(13, ' ', true) << " ";
            }

            nameCol = rec.indexOf("rst_sent");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
            if ((aux1.length())>0){
                out << aux1.rightJustified(3, ' ', true) << " ";
            }

            nameCol = rec.indexOf("call");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);

            if ((0 <= aux1.length()) && (aux1.length() <= 14) )
            {
                out << aux1.rightJustified(13, ' ', true) << " ";
            }
            else
            {
                out << QString("NOCAL").rightJustified(13, ' ', true) << " ";
            }

            nameCol = rec.indexOf("rst_rcvd");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
            if ((aux1.length())>0){
                out << aux1.rightJustified(3, ' ', true) << " ";
            }

            nameCol = rec.indexOf("stx");
            aux1 = (query.value(nameCol)).toString(); aux1 = util->checkAndFixASCIIinADIF(aux1);
            if ((aux1.length())>0){
                out << "<STX:" << QString::number(aux1.length()) << ">" << aux1  << " ";
            }
        }
        else
        {

        }
    } // END OF WHILE
    out << "END-OF-LOG:" << endl;
    file.close();
    return true;

}

bool FileManager::cabrilloLogExportToFile(const QString& _fileName, const int logNconst)
{
    //qDebug() << "FileManager::cabrilloLogExportToFile" << endl;

    QString fn = _fileName;

    //TODO: Message "You must select a proper file format
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog: Cabrillo Log Export not implemented"));
    msgBox.setText(tr("I am sorry but the Cabrillo Export To File feature has still not been implemented."));
    msgBox.exec();
    return false;
}

bool FileManager::printQs(const QStringList _line)
//bool FileManager::printQs(const QString _q, const QStringList _line)
{
    QStringList qs = _line;

    for (int i = 0; i<qs.size(); i++){
        //qDebug() << qs.at(i) << " ";
    }

    return true;
}


bool FileManager::adifLogExportMarked(const QString& _fileName)
{    
    return adifLogExportToFile(_fileName, 0, true, false);
}

/*
bool FileManager::adifCheckMoreThanOneLog(QFile& _f)
{
    //qDebug() << "FileManager::adifCheckMoreThanOneLog:" << endl;
    QFile &file = _f;
    qint64 pos; //Position in the file
    QString line = QString();
    QStringList fields;
    QString aux = QString();
    QString lastAux = QString();
    QStringList data = QStringList();
    int log1 = -1;
    int log2 = -2;
    bool firstLog = true;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "FileManager::adifCheckMoreThanOneLog File not found" << endl;
        return false;
    }

    pos = file.pos();

    while ( !file.atEnd()   )
    {
        //line = file.readLine();

        line.clear();
        line.append(file.readLine().trimmed().toUpper());

        if (line.contains("APP_KLOG_LOGN"))
        {   fields.clear();
            fields << line.split("<", QString::SkipEmptyParts);

            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if (aux.contains("APP_KLOG_LOGN"))
                {
                    data.clear();
                    data << aux.split('>');
                    //qDebug() << "FileManager::adifCheckMoreThanOneLog: data.0: " << data.at(0) << endl;
                    //qDebug() << "FileManager::adifCheckMoreThanOneLog: data.1: " << data.at(1) << endl;
                    if (firstLog)
                    {
                        log1 = (data.at(1)).toInt();
                        firstLog = false;
                    }
                    else
                    {
                        if (log1 == (data.at(1)).toInt())
                        {}
                        else
                        {
                            file.close();
                            return true;
                        }
                    }
                }
            }
        }
    }
    //qDebug() << "FileManager::adifCheckMoreThanOneLog: JUST ONE!" << aux << endl;
    file.close();
    return false;
}
*/

bool FileManager::adifLoTWReadLog(const QString& tfileName)
{
    //qDebug() << "FileManager::adifLoTWReadLog: " << tfileName << endl;
    QString fileName = tfileName;

    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "FileManager::adifLoTWReadLog File not found" << fileName << endl;
        return false;
    }

    QStringList fields, qsToPass;
    bool hasEOH = false;
    int numberOfQsos = 0;
    int numberOfQsosLoWTHeader = 0;
    QString line = QString();
    QString auxString = QString();
    QString aux = QString();
    qint64 pos; //Position in the file
    bool inHeader = true;
    pos = file.pos();
    fields.clear();
    qsToPass.clear();
    int step = 1;


    while ( !file.atEnd()   )
    {
        line = (file.readLine()).toUpper();
        numberOfQsos = numberOfQsos + line.count("EOR>");
        if ((line.count("<EOH>")>0) && (!hasEOH))
        {
            hasEOH = true;
        }
        numberOfQsosLoWTHeader = numberOfQsosLoWTHeader + line.count("");
    }
    //<APP_LoTW_NUMREC:3>847

    //qDebug() << "FileManager::adifLoTWReadLog QSOs found: " << QString::number(numberOfQsos) << endl;

    QProgressDialog progress(tr("Reading LoTW file..."), tr("Abort reading"), 0, numberOfQsos, this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setVisible(true);
    progress.setValue(0);
    progress.setMaximum(numberOfQsos);

    step = util->getProgresStepForDialog(numberOfQsos);

    //qDebug() << "FileManager::adifLoTWReadLog (STEP)/Number: " << QString::number(step) << "/" << QString::number(numberOfQsos) << endl;
    //qDebug() << "FileManager::adifLoTWReadLog (number & step: " << QString::number(numberOfQsos % step) << endl;

    file.seek(pos);

    /*
       Optional header information may be included before the actual data in the file.
       To include optional header info, the first character of the file must be something other than <.
       Any amount of header info of any value except <eoh> may be included. The header info must be
       terminated with <eoh>. Any number of characters of any value except < may follow <eoh>.
       The first < after <eoh> is the start of the first field of the first data record in the file.
    */

    //qDebug() << "FileManager::adifLoTWReadLog: Going to read the HEADER" << endl;
    //Read HEADER
    line = file.readLine().trimmed().toUpper();
    //qDebug() << "FileManager::adifLoTWReadLog: " << line << endl;

    if ( (!(line.startsWith('<'))) && (inHeader) )
    { // The file has a header
        if (line.contains("<EOH>")) // To check if the first line contains the EOR but not alone in the line.
        {
            inHeader = false;
        }

        line.clear(); // We should finish the if with real data in "line" or a clear one.
        while ( inHeader && hasEOH)
        {
            line = file.readLine().trimmed().toUpper();
            if (line.contains("<EOH>"))
            {
                inHeader = false;
                line.clear(); //TODO: Maybe this clearing deletes a line that may have data...
            }
        }
        pos = file.pos();
    }
    else if (!(line.startsWith('<')))
    {   // The file does not have any header.
        // Reading the first QSO...
        /*
            Cases:
               1.- One big line with several QSO
               2.- One QSO uses several lines.                      - THIS IS USUALLY THE CASE FOR LOTW
               3.- Last line of one QSO includes data of the next one
        */
        inHeader = true;
        while (inHeader)
        {
            pos = file.pos();
            line = file.readLine().trimmed().toUpper();
            if (  (line.startsWith('<'))  )
            {
                inHeader = false;
                line.clear(); //TODO: Maybe this clearing deletes a line that may have data...
            }
        }
    }

    file.seek(pos);
    //START reading QSO data...
    //qDebug() << "FileManager::adifLoTWReadLog: QSO data reading started..."  << endl;

    while ((!noMoreQso) )
    {
        if (!file.atEnd())
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            if (line.contains("<APP_LoTW_EOF>"))
            {
                noMoreQso = true;
            }
            //line.append(file.readLine().toUpper()); // TODO: Check if we should remove extra spaces,tabs and so on...
            //qDebug() << "FileManager::adifLoTWReadLog-line:" << line << endl;
            fields << line.split("<", QString::SkipEmptyParts);
/*
<CALL:6>DL4ZAA
<BAND:3>15M
<FREQ:8>21.31700
<MODE:3>SSB
<APP_LoTW_MODEGROUP:5>PHONE
<QSO_DATE:8>20150329
<TIME_ON:6>112900
<QSL_RCVD:1>Y
<QSLRDATE:8>20170910
<eor>
*/
            qsToPass.clear();
            auxString.clear();
            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if ( (aux.contains('>')) && (auxString.length() > 0) )
                {
                    qsToPass.last() = qsToPass.last() + auxString;
                    //qDebug() << "FileManager::adifLoTWReadLog Modified in qsToPass: " << qsToPass.last() << endl;
                    //qsToPass << aux.trimmed();
                    //qDebug() << "FileManager::adifLoTWReadLog Added to qsToPass: " << aux.trimmed() << endl;
                    auxString.clear();
                }
                else if (( aux.contains('>')) && (auxString.length() <= 0) )
                {
                    qsToPass << aux.trimmed();
                }
                else
                {
                    auxString = auxString + "-" + aux.trimmed();
                    //qDebug() << "FileManager::adifLoTWReadLog auxString: " << auxString << endl;
                }
               //qDebug() << "FileManager::adifLoTWReadLog fields: " << aux << endl;
            }

            if (auxString.length()>0)
            {
                //qDebug() << "FileManager::adifLoTWReadLog auxString2: " << auxString << endl;
                qsToPass.last() = qsToPass.last() + auxString.trimmed();
            }
            //qDebug() << "FileManager::adifLoTWReadLog END fields" << endl;
            //qDebug() << "FileManager::adifLoTWReadLog Mod: " << qsToPass.join("/") << endl;
            //qDebug() << "FileManager::adifLoTWReadLog END2 fields" << endl;

            fields = qsToPass;


            if (fields.contains("EOR>")) // We are going to add a QSO to the log... prepare the Query!
            {
                //qDebug() << "FileManager::adifLoTWReadLog: START of QSO "<< endl;
                //dataProxy->isThisQSODuplicated()

                //int getDuplicatedQSOId(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode);
                QString str, _call, _date, _time, _band, _mode;
                QString field;
                foreach (str, fields)
                {
                    field = readAdifField(str).at(0);

                    if (field == "CALL")
                    {

                    }
                    else if (field == "QSO_DATE")
                    {

                    }
                    else if (field == "TIME_ON")
                    {}
                    else if (field == "BAND")
                    {}
                    else if (field == "MODE")
                    {}
                    else
                    {}
                    //qDebug() << "FileManager::adifLoTWReadLog: " << str << endl;
                }
                //qDebug() << "FileManager::adifLoTWReadLog: END of QSO "<< endl;
                fields.clear();

            } // END of  if (fields.contains("EOR>")) // We are going to add a QSO to the log... !
            else
            {
                if (file.atEnd())
                {
                    noMoreQso = true;
                }
                //qDebug() << "FileManager::adifLoTWReadLog: fields DOES NOT contains EOR>" << endl;

            }

        }
    }

    return true;
}

bool FileManager::adifReadLog(const QString& tfileName, const int logN)
{
    //qDebug() << "FileManager::adifReadLog:" << tfileName << endl;

    //int n = 0;
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    //int maxLogs = dataProxy->getNumberOfManagedLogs(); // To manage several logs


    bool sqlOK = true;
    QStringList queries = QStringList();
    queries.clear();
    QSqlQuery query;

    QStringList fields, qsToPass, qsAux;
    QStringList currentQSOfields = QStringList();
    fields.clear();
    qsToPass.clear();
    qsAux.clear();

    QString fieldToAnalyze = QString();
    QString fileName = tfileName;
    QString line = QString();
    QString aux = QString();
    QString auxString = QString();
    QString _band = QString();

    bool inHeader = true;
    bool EOR = false;
    noMoreQso = false;
    bool preparedQBool = false;
    qint64 pos; //Position in the file
    int i = 0; //Aunxiliar variable
    int numberOfQsos = 0;
    int step = 1;
    int errorCode = -1;
    int qsosInTransaction = 0;
    bool ignoreErrorCode19 = false;

    QFile file( fileName );

    //bool moreThanOneLog = adifCheckMoreThanOneLog(file);
    int howManyLogs = howManyLogsInFile(file);
    fillHashLog(file);
    //I am creating several logs when importing a file
    //We need to fill the hashLog to process then in processLog


    bool keepLogsInFile = false;

   //qDebug() << "FileManager::adifReadLog: Logs: " << QString::number(howManyLogs) << endl;

    if (howManyLogs>1)
    {

        QMessageBox msgBox;
        aux = QString(tr("There are more than one log in this logfile.\nAll logs will be imported in the current log.\nDo you want to continue?"));
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Yes:
              // Yes was clicked
                //qDebug() << "FileManager::adifReadLog: clicked YES" << endl;
                keepLogsInFile = true;
              break;
          case QMessageBox::No:
                // No Save was clicked
                //qDebug() << "FileManager::adifReadLog: clicked NO" << endl;
                 keepLogsInFile = false;
                 return false;
              break;
          default:
                // should never be reached
                keepLogsInFile = false;
                return false;
                //qDebug() << "FileManager::adifReadLog: default" << endl;
              break;
        }
    }


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       //qDebug() << "FileManager::adifReadLog File not found" << fileName << endl;
        return false;
    }

    bool hasEOH = false;
    pos = file.pos();

    while ( !file.atEnd()   )
    {
        line = (file.readLine()).toUpper();
        numberOfQsos = numberOfQsos + line.count("EOR>");
        if ((line.count("<EOH>")>0) && (!hasEOH))
        {
            hasEOH = true;
        }
    }

    //qDebug() << "FileManager::adifReadLog QSOs found: " << QString::number(numberOfQsos) << endl;

    QProgressDialog progress(tr("Reading ADIF file..."), tr("Abort reading"), 0, numberOfQsos, this);
    /*progress.setWindowModality(Qt::WindowModal);*/
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setVisible(true);
    progress.setValue(0);
    progress.setMaximum(numberOfQsos);

    step = util->getProgresStepForDialog(numberOfQsos);

    //step = getProgresStepForDialog(numberOfQsos);
    //qDebug() << "FileManager::adifReadLog (STEP)/Number: " << QString::number(step) << "/" << QString::number(numberOfQsos) << endl;
    //qDebug() << "FileManager::adifReadLog (number & step: " << QString::number(numberOfQsos % step) << endl;

    file.seek(pos);

    /*
    Optional header information may be included before the actual data in the file.
    To include optional header info, the first character of the file must be something other than <.
    Any amount of header info of any value except <eoh> may be included. The header info must be
    terminated with <eoh>. Any number of characters of any value except < may follow <eoh>.
    The first < after <eoh> is the start of the first field of the first data record in the file.
    */

    //qDebug() << "FileManager::adifReadLog: Going to read the HEADER" << endl;
    //Read HEADER
    line = file.readLine().trimmed().toUpper();
    //qDebug() << "FileManager::adifReadLog: " << line << endl;

    if ( (!(line.startsWith('<'))) && (inHeader) )
    { // The file has a header

        if (line.contains("<EOH>")) // To check if the first line contains the EOR but not alone in the line.
        {
            inHeader = false;
        }

        line.clear(); // We should finish the if with real data in "line" or a clear one.
        while ( inHeader && hasEOH)
        {
            line = file.readLine().trimmed().toUpper();
            if (line.contains("<EOH>"))
            {
                inHeader = false;
                line.clear(); //TODO: Maybe this clearing deletes a line that may have data...
            }
        }
        pos = file.pos();
    }
    else if (!(line.startsWith('<')))
    { // The file does not have any header.
      // Reading the first QSO...
        /*
          Cases:
            1.- One big line with several QSO
            2.- One QSO uses several lines.
            3.- Last line of one QSO includes data of the next one
        */
        inHeader = true;
        while (inHeader)
        {
            pos = file.pos();
            line = file.readLine().trimmed().toUpper();
            if (  (line.startsWith('<'))  )
            {
                inHeader = false;
                line.clear(); //TODO: Maybe this clearing deletes a line that may have data...
            }
        }
    }

    file.seek(pos);

    // START reading QSO data...
   //qDebug() << "FileManager::adifReadLog: QSO data reading started..."  << endl;
   preparedQuery.prepare(prepareStringLog());


/*
    bool prepBool = dataProxy->queryPrepare("INSERT INTO log (call, qso_date, bandid, modeid, time_on, time_off, srx, stx, srx_string, stx_string, qso_date_off, band_rx, rst_sent, rst_rcvd, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, eq_call, email, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, my_gridsquare, iota, iota_island_id, my_iota, my_iota_island_id, k_index, lat, lon, my_lat, my_lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, clublog_qso_upload_date, clublog_qso_upload_status, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_name, name, operator, station_callsign, owner_callsign, my_rig, my_sig, my_sig_info, my_state, state, my_street, notes, nr_bursts, nr_pings, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, tx_pwr, sat_mode, sat_name, sfi, sig, swl, ten_ten, web, points, multiplier, lognumber) VALUES (:call, :qso_date, :bandid, :modeid, :time_on, :time_off, :srx, :stx, :srx_string, :stx_string, :qso_date_off, :band_rx, :rst_sent, :rst_rcvd, :cqz, :ituz, :dxcc, :address, :age, :cnty, :comment, :a_index, :ant_az, :ant_el, :ant_path, :arrl_sect, :checkcontest, :class, :contacted_op, :contest_id, :country, :credit_submitted, :credit_granted, :distance, :eq_call, :email, :eqsl_qslrdate, :eqsl_qslsdate, :eqsl_qsl_rcvd, :eqsl_qsl_sent, :force_init, :freq, :freq_rx, :gridsquare, :my_gridsquare, :iota, :iota_island_id, :my_iota, :my_iota_island_id, :k_index, :lat, :lon, :my_lat, :my_lon, :lotw_qslrdate, :lotw_qslsdate, :lotw_qsl_rcvd, :lotw_qsl_sent, :clublog_qso_upload_date, :clublog_qso_upload_status, :max_bursts, :ms_shower, :my_city, :my_cnty, :my_country, :my_cq_zone, :my_name, :name, :operator, :station_callsign, :owner_callsign, :my_rig, :my_sig, :my_sig_info, :my_state, :state, :my_street, :notes, :nr_bursts, :nr_pings, :pfx, :precedence, :prop_mode, :public_key, :qslmsg, :qslrdate, :qslsdate, :qsl_rcvd, :qsl_sent, :qsl_rcvd_via, :qsl_sent_via, :qsl_via, :qso_complete, :qso_random, :qth, :rx_pwr, :tx_pwr, :sat_mode, :sat_name, :sfi, :sig, :swl, :ten_ten, :web, :points, :multiplier, :lognumber)" );
    if (prepBool)
    {
        //qDebug() << "FileManager::adifReadLog: Query prepared succesfully"  << endl;
    }
    else
    {
        //qDebug() << "FileManager::adifReadLog: Query NOT prepared succesfully"  << endl;
    }
*/



    if (db.transaction())
    {
        //qDebug() << "FileManager::adifReadLog: Transaction Opened"  << endl;
    }
    else
    {
        //qDebug() << "FileManager::adifReadLog: Transaction NOT Opened"  << endl;
    }

    //file.seek(pos);
    fields.clear();
   // while ( (!file.atEnd() ) && (!noMoreQso) && (sqlOK))
    while ((!noMoreQso) && (sqlOK))
    {

        if (!file.atEnd())
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            //line.append(file.readLine().toUpper()); // TODO: Check if we should remove extra spaces,tabs and so on...
            //qDebug() << "FileManager::adifReadLog-line:" << line << endl;
            //fields.clear(); //TODO: Check if I should clear fields... I think I should not because I could loose data if a line contains data after an <EOR>

             fields << line.split("<", QString::SkipEmptyParts);
        }

        //TODO: Check what happens

        //qDebug() << "FileManager::adifReadLog START fields" << endl;
        qsToPass.clear();
        auxString.clear();
        foreach (aux, fields)
        {
            aux = aux.trimmed();
            if ( (aux.contains('>')) && (auxString.length() > 0) )
            {
                //qsToPass << auxString + aux;
                qsToPass.last() = qsToPass.last() + auxString;
                //qDebug() << "FileManager::adifReadLog Modified in qsToPass: " << qsToPass.last() << endl;
                qsToPass << aux.trimmed();
                //qDebug() << "FileManager::adifReadLog Added to qsToPass: " << aux.trimmed() << endl;
                auxString.clear();
            }
            else if (( aux.contains('>')) && (auxString.length() <= 0) )
            {
                qsToPass << aux.trimmed();
            }
            else
            {
                auxString = auxString + "-" + aux.trimmed();
               //qDebug() << "FileManager::adifReadLog auxString: " << auxString << endl;
            }

           //qDebug() << "FileManager::adifReadLog fields: " << aux << endl;
        }

       //qDebug() << "FileManager::adifReadLog-W-1" << endl;

        if (auxString.length()>0)
        {
           //qDebug() << "FileManager::adifReadLog auxString2: " << auxString << endl;
            qsToPass.last() = qsToPass.last() + auxString.trimmed();
        }

        //qDebug() << "FileManager::adifReadLog END fields" << endl;
        //qDebug() << "FileManager::adifReadLog Mod: " << qsToPass.join("/") << endl;
        //qDebug() << "FileManager::adifReadLog END2 fields" << endl;

        fields = qsToPass;

        if (fields.contains("EOR>")) // We are going to add a QSO to the log... prepare the Query!
        {
            //qDebug() << "FileManager::adifReadLog: fields contains EOR>" << endl;

            preparedQuery.bindValue( QString(":lognumber"), QString::number(logN));



            while ( (!EOR) && (!fields.isEmpty()) )
            {

                //qDebug() << "FileManager::adifReadLog-W-2" << endl;


                fieldToAnalyze = (fields.takeFirst()).trimmed();

                if ( fieldToAnalyze.contains("EOR>") )
                {
                     //qDebug() << "FileManager::adifReadLog-W-2.1" << endl;
                    currentQSOfields << fieldToAnalyze;
                    //preparedQBool = processQsoReadingADIF(currentQSOfields, logN, keepLogsInFile, hashLogs);
                    preparedQBool = processQsoReadingADIF(currentQSOfields, logN, keepLogsInFile);
                    if (preparedQBool)
                    {
                        //qDebug() << "FileManager::adifReadLog: preparedQBool = true"  << endl;

                    }
                    else
                    {
                        //qDebug() << "FileManager::adifReadLog: preparedQBool = false"  << endl;
                    }

                }
                else
                {
                    //qDebug() << "FileManager::adifReadLog: Not contains EOR"  << endl;

                    if ((!fieldToAnalyze.contains('>')) && (currentQSOfields.length()>0))
                    {
                       //qDebug() << "FileManager::adifReadLog: Contains > & currentsQSOfields.length>0"  << endl;
                        auxString = currentQSOfields.at(currentQSOfields.length()-1);
                        auxString = auxString + "\n" + fieldToAnalyze;
                        //currentQSOfields.at(currentQSOfields.length()) = auxString;
                        fieldToAnalyze = auxString;

                    }
                    currentQSOfields << fieldToAnalyze;
                    EOR = false;
                }
            }
            //sqlOK = dataProxy->queryExec();
            sqlOK = preparedQuery.exec();
            //qDebug() << "FileManager::adifReadLog: executedQuery1: " << preparedQuery.executedQuery()  << endl;

            if (!sqlOK)
            {

               //qDebug() << "FileManager::adifReadLog: (0) LastQuery: " << preparedQuery.lastQuery()  << endl;
               //qDebug() << "FileManager::adifReadLog: (0) LastError-data: " << preparedQuery.lastError().databaseText()  << endl;
               //qDebug() << "FileManager::adifReadLog: (0) LastError-driver: " << preparedQuery.lastError().driverText()  << endl;
               //qDebug() << "FileManager::adifReadLog: (0) LastError-n: " << QString::number(preparedQuery.lastError().number() ) << endl;
            }
            else
            {
                //qDebug() << "FileManager::adifReadLog: preparedQuery executed with NO ERROR "  << endl;
            }

            queryPreparation(logN); // to clear Values

            //qDebug() << "FileManager::adifReadLog: executedQuery2: " << preparedQuery.executedQuery()  << endl;
            //qDebug() << "FileManager::adifReadLog: LastQuery2: " << preparedQuery.lastQuery()  << endl;


            if (( (i % step ) == 0) )
            { // To update the speed I will only show the progress once each X QSOs
               //qDebug() << "FileManager::adifReadLog: MOD 0 - i = " << QString::number(i)  << endl;

                aux = tr("Importing ADIF file...\n QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);

               progress.setLabelText(aux);
               progress.setValue(i);

            }
            else
            {
               //qDebug() << "FileManager::adifReadLog: Mod: "<< QString::number(i) << " mod " << QString::number(step) << " = " << QString::number(i % step) << endl;

            }

            if ( progress.wasCanceled() )
            {

                QMessageBox msgBox;
                aux = QString(tr("You have cancelled the file export. The file will be removed and no data will be exported.\nDo you still want to cancel?"));
                msgBox.setText(aux);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);
                int ret = msgBox.exec();
                switch (ret) {
                  case QMessageBox::Yes:
                      // Yes was clicked
                        noMoreQso = true;
                      break;
                  case QMessageBox::No:
                        // No Save was clicked
                      break;
                  default:
                        // should never be reached
                      break;
                }
            }
            else
            {}


            progress.setValue(i);

            i++;

            if (sqlOK)
            {
                currentQSOfields.clear();
                //qDebug() << "FileManager::adifReadLog: (1) in While sqlOK (QSO added) = TRUE"  << endl;
            }
            else
            {
                /*
                //errorCode = preparedQuery.lastError().number();
                //qDebug() << "FileManager::adifReadLog: QSO DUPE" << endl;

                //qDebug() << "FileManager::adifReadLog: (1) LastQuery: " << preparedQuery.lastQuery()  << endl;
                //qDebug() << "FileManager::adifReadLog: (1) LastError-data: " << preparedQuery.lastError().databaseText()  << endl;
                //qDebug() << "FileManager::adifReadLog: (1) LastError-driver: " << preparedQuery.lastError().driverText()  << endl;
                //qDebug() << "FileManager::adifReadLog: (1) LastError-n: " << QString::number(preparedQuery.lastError().number() ) << endl;
                if ((errorCode == 19) && (!ignoreErrorCode19))
                { // There are some repeated QSO
                    QMessageBox msgBox;
                    aux = tr("It seems that there are some QSO duplicated in the ADIF file you are importing. Do you want to continue? (Duped QSO will not be imported)");
                    msgBox.setText(aux);
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Yes:
                          // Yes was clicked
                            //qDebug() << "FileManager::adifReadLog: (1) clicked YES" << endl;
                            sqlOK = true;
                          break;
                    case QMessageBox::YesToAll:
                        // Yes was clicked
                            //qDebug() << "FileManager::adifReadLog: (1) clicked YES to ALL" << endl;
                            ignoreErrorCode19 = true;
                            sqlOK = true;
                        break;
                      case QMessageBox::No:
                          // No Save was clicked
                            //qDebug() << "FileManager::adifReadLog: (1) clicked NO" << endl;
                             sqlOK = false;
                          break;
                      default:
                          // should never be reached
                            sqlOK = true;
                            //qDebug() << "FileManager::adifReadLog: (1) default" << endl;
                          break;
                    }
                    //;

                }
                else if((errorCode == 19) && (ignoreErrorCode19))
                {
                  sqlOK = true;
                 //qDebug() << "FileManager::adifReadLog: errorCode=19 && ignoreErrorCode19" << endl;

                }
                else
                {
                    //qDebug() << "FileManager::adifReadLog: (2) LastQuery: " << preparedQuery.lastQuery()  << endl;
                    //qDebug() << "FileManager::adifReadLog: (2) LastError-data: " << preparedQuery.lastError().databaseText()  << endl;
                    //qDebug() << "FileManager::adifReadLog: (2) LastError-driver: " << preparedQuery.lastError().driverText()  << endl;
                    //qDebug() << "FileManager::adifReadLog: (2) LastError-n: " << QString::number(preparedQuery.lastError().number() ) << endl;

                    QMessageBox msgBox;
                    aux = tr("An unexpected error ocurred while importing. Please send this code to the developer for analysis: ");
                    msgBox.setText(aux + "FM-1 #" + QString::number(errorCode) );
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Ok:
                          // Yes was clicked
                            sqlOK = false;
                            //qDebug() << "FileManager::adifReadLog: (2) I have just set sqlOK=False (1)" << endl;
                          break;
                      default:
                          // should never be reached
                            sqlOK = false;
                            //qDebug() << "FileManager::adifReadLog: (2) I have just set sqlOK=False (2)" << endl;
                          break;
                    }
                }
            */
            }

            //qDebug() << "FileManager::adifReadLog: qsosInTransaction: " <<  QString::number(qsosInTransaction)   << endl;
            if ((qsosInTransaction>=step*10) && (qsosInTransaction>200) )
            {
                qsosInTransaction = 0;
                if (db.commit())
                {
                    //qDebug() << "FileManager::adifReadLog: MIDcommit OK: " <<  QString::number(i) << endl;

                    if (db.transaction())
                    {
                        //qDebug() << "FileManager::adifReadLog: MIDTransaction Opened" << endl;
                    }
                    else
                    {
                        //qDebug() << "FileManager::adifReadLog: MIDTransaction NOT Opened"  << endl;
                    }
                }
                else
                {
                    //qDebug() << "FileManager::adifReadLog: MIDcommit NOK: " <<  QString::number(i) << endl;
                    errorCode = preparedQuery.lastError().number();

                    QMessageBox msgBox;
                    aux = tr("An error ocurred while importing. No data will be imported. Please send this code to the developer for analysis: ");
                    msgBox.setText(aux + "FM-2 #" + QString::number(errorCode));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    int ret = msgBox.exec();

                    switch (ret) {
                      case QMessageBox::Ok:
                          // Yes was clicked
                            sqlOK = false;
                            noMoreQso = true;
                            //qDebug() << "FileManager::adifReadLog: I have just set sqlOK=False (3)" << endl;
                          break;
                      default:
                          // should never be reached
                            sqlOK = false;
                            //qDebug() << "FileManager::adifReadLog: I have just set sqlOK=False (4)" << endl;
                          break;
                    }

                    if (db.rollback())
                    {
                        //qDebug() << "FileManager::adifReadLog: MIDcommit NOK: Rolledback"  << endl;

                    }
                    else
                    {
                      //TODO: Check the error if db.rollback returns false
                      //qDebug() << "FileManager::adifReadLog: MIDcommit NOK: Roleback returned FALSE¿?"  << endl;
                    }

                }
            }
            else
            {
                qsosInTransaction++;
            }


        } // END of  if (fields.contains("EOR>")) // We are going to add a QSO to the log... !
        else
        {
            if (file.atEnd())
            {
                noMoreQso = true;
            }
            //qDebug() << "FileManager::adifReadLog: fields DOES NOT contains EOR>" << endl;

        }


        //TODO: Check how to stop the importing process

    } // END OF WHILE

    if (noMoreQso)
    {
        //qDebug() << "FileManager::adifReadLog: noMoreQso = true" << endl;
        progress.setValue(numberOfQsos);

    }
    else
    {
        //qDebug() << "FileManager::adifReadLog: noMoreQso = false" << endl;
    }
    if (sqlOK)
    {
        //qDebug() << "FileManager::adifReadLog: sqlOK = true" << endl;
    }
    else
    {
        //qDebug() << "FileManager::adifReadLog: sqlOK = false" << endl;
    }

    if (sqlOK)
    {
        if (db.commit())
        {
            //qDebug() << "FileManager::adifReadLog: Last commit OK"  << endl;
        }
        else
        {
            //emit queryError(Q_FUNC_INFO, preparedQuery.lastError().databaseText(), preparedQuery.lastError().number(), preparedQuery.lastQuery());

            //qDebug() << "FileManager::adifReadLog: Commit error DBText: " << preparedQuery.lastError().databaseText() << endl;
            //qDebug() << "FileManager::adifReadLog: Commit error Last Error T: " << QString::number(preparedQuery.lastError().number()) << endl;
            //qDebug() << "FileManager::adifReadLog: Commit error Last Query: " << preparedQuery.lastQuery() << endl;
            /*
            errorCode = preparedQuery.lastError().number();
            QMessageBox msgBox;
            aux = tr("An error ocurred while importing. No data will be imported. Please send this code to the developer for analysis: ");
            msgBox.setText(aux + "FM-3 #" + QString::number(errorCode));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);

            int ret = msgBox.exec();
            switch (ret) {
              case QMessageBox::Ok:
                  // Yes was clicked
                    sqlOK = false;
                    //qDebug() << "FileManager::adifReadLog: I have just set sqlOK=False (5)" << endl;
                  break;
              default:
                  // should never be reached
                    sqlOK = false;
                    //qDebug() << "FileManager::adifReadLog: I have just set sqlOK=False (6)" << endl;
                  break;

            }
            */
            if (db.rollback())
            {

            }
            else
            {
              //TODO: Check the error if db.rollback returns false
            }

        }

    }
    else
    {
        //qDebug() << "FileManager::adifReadLog: sqlOK = NOK"  << endl;

    }


    progress.setValue(numberOfQsos);


    //qDebug() << "FileManager::adifReadLog END "  << endl;
    return true;

}

bool FileManager::processQsoReadingADIF(const QStringList _line, const int logNumber, const bool _keepLogsInFile)
//bool FileManager::processQsoReadingADIF(const QStringList _line, const int logNumber, const bool _keepLogsInFile, QHash<int, int> &_logs)
{

    //qDebug() << "FileManager::processQsoReadingADIF: log: " << QString::number(logNumber) << endl;
   //qDebug() << "FileManager::processQsoReadingADIF: log: " << _line.at(0) << endl;
    //qDebug() << "FileManager::processQsoReadingADIF: " << _line.join("/") << endl;

    //QHash<int, int> &hashLogs = _logs;

    //QFile &file = _f;

    //bool keepLogsInF;// = _keepLogsInFile; //TODO: Check if needed or remove it completely. This line is just to remove a warning
    int i = -1;
    QDate date;
    QTime time;
    QStringList qs = _line;
    QStringList oneField;
    QString field, data;
    QSqlQuery query;
    //confirmed = 0; // 0 means worked, 1 means confirmed

    QString queryString, stringFields, stringData;
    //int lastId;
    int dxcc = 0;
    int cqz = 0;
    int ituz = 0;
    //int i; // Aux value
    QString aux; // Aux string
    QString aux2, aux3;
    QString qrzCall = "";
    QString submode = QString();
    bool bandRXDef = false;
    int bandi = -1;
    int bandrxi = -1;
    bool rstRXr = false;
    bool rstTXr = false;

    //KLog does not understand (and will not import) a QSO without these fields
    bool haveCall = false;
    bool haveBand = false;
    bool haveMode = false;
    bool haveSubMode = false;
    bool haveTime = false;
    bool haveDate = false;

    //bool ret;
    int lenght = 0;
    //int currentLog = logNumber;

    //qDebug() << "FileManager::processQsoReadingADIF" << QString::number(qs.size()) << "/" << QString::number(logNumber) << endl;
    //TODO: To remove the next line, it was just to measure the time it takes.
    ignoreUnknownAlways = true;
    QString str;
    //preparedQuery.bindValue( ":confirmed", '0' );

    //qDebug() << "FileManager::processQsoReadingADIF: Entering the foreach" << endl;
    foreach (str, qs)
    {
        //qDebug() << "FileManager::processQsoReadingADIF: " << str << endl;
        if ( !( (str.contains(":")) && (str.contains(">")) ) )
        {
            //qDebug() << "FileManager::processQsoReadingADIF: NOT (contains : and >): " << str << endl;
        }
        else
        {
            //qDebug() << "FileManager::processQsoReadingADIF: (contains : and >): " << str << endl;

            oneField = str.split(">", QString::SkipEmptyParts);

            //qDebug() << "FileManager::processQsoReadingADIF: (oneField)" << oneField << endl;
            if (checkADIFValidFormat(oneField))
            {
                i = (qs.at(0)).count(":");
                field = (oneField.at(0)).trimmed(); // Needs to be cleared FIELD:4:D
                data = (oneField.at(1)).trimmed();
                data = util->checkAndFixASCIIinADIF(data);


                if (i == 2)
                { // DATE:8:D / 20141020
                    lenght = (field.section(':', 1, 1)).toInt();
                    field = field.section(':', 0, 0);
                }
                else if (i == 1)
                { // DATE:8 / 20141020
                    lenght = (field.section(':', 1, 1)).toInt();
                    field = field.section(':', 0, 0);
                }
                else
                {
                    //qDebug() << "FileManager::checkADIFValidFormat-1 " << endl;
                    //return false;
                }

                //field = oneField.at(0).trimmed();
                //data = oneField.at(1).trimmed();

                //lenght = field.indexOf(":");
                //field = field.left(lenght);
                //qDebug() << "FileManager::processQsoReadingADIF (field/data): " << field << "/" << data << endl;

                if (field == "CALL")
                {
                    qrzCall = data;

                    preparedQuery.bindValue( ":call", qrzCall );
                    haveCall = true;

                    //qDebug() << "FileManager::processQsoReadingADIF-CALL:" << data << endl;
                }
                else if (field == "QSO_DATE")
                {
                    //qDebug() << "FileManager::processQsoReadingADIF-QSO_DATE:" << data << endl;
                    preparedQuery.bindValue( ":qso_date", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                    haveDate = true;
                }
                else if (field == "BAND")
                {
                    //preparedQuery.bindValue( ":bandid", data );
                    i = dataProxy->getIdFromBandName(data);
                    //i = db->getBandIDFromName2(data);
                    if (i>=0)
                    {
                        preparedQuery.bindValue( ":bandid", QString::number(i) );
                        haveBand = true;
                        bandi = i;

                        //qDebug() << "FileManager::processQsoReadingADIF-Band: " << data << "/"  << QString::number(i) << endl;
                    }
                    else
                    {
                     //qDebug() << "FileManager::processQsoReadingADIF-Band - Wrong band: " << data << "/"  << QString::number(i) << endl;
                    }
                   /*
                    queryString = QString("SELECT id FROM band WHERE name ='%1'").arg(data);
                    query.exec(queryString);
                    query.next();
                    if (query.isValid())
                    {
                        preparedQuery.bindValue( ":bandid", query.value(0).toInt() );
                        //qDebug() << "FileManager::bprocessQsoReadingADIF-Band: " << data << endl;
                    }
                    */
                }

                else if (field == "MODE")
                {
                    i = dataProxy->getSubModeIdFromSubMode(data); // get modeid
                    if (i>=0)
                    {
                        {
                            if (!haveSubMode)
                            {
                                preparedQuery.bindValue( ":modeid", QString::number(i) );
                                haveMode = true;
                                haveSubMode = true;
                            }
                        }
                    }
                }

                else if (field == "SUBMODE")
                {                    
                    i = dataProxy->getSubModeIdFromSubMode(data);
                    if (i>=0)
                    {                        
                        preparedQuery.bindValue( ":modeid", QString::number(i) );
                        preparedQuery.bindValue( ":submode", QString::number(i) );
                        haveSubMode = true;
                        haveMode=true;
                        //submode = data;
                    }
                }

                else if (field == "SRX")
                {
                    preparedQuery.bindValue( ":srx", data );
                    //qDebug() << "FileManager::bprocessQsoReadingADIF-srx: " << data << endl;
                }
                else if (field == "STX")
                {
                    preparedQuery.bindValue( ":stx", data );
                    //qDebug() << "FileManager::bprocessQsoReadingADIF-stx: " << data << endl;
                }

                else if (field == "TIME_ON")
                {
                    if (data.length() == 4)
                    {
                        data = data + "00";
                    }

                    preparedQuery.bindValue( ":time_on", (time.fromString(data,"hhmmss")).toString("hh:mm:ss") );
                    haveTime = true;
                    aux3 = (time.fromString(data,"hhmmss")).toString("hh:mm:ss");

                    //qDebug() << "FileManager::bprocessQsoReadingADIF-time_on: " << (time.fromString(data,"hhmmss")).toString("hh:mm:ss") << endl;
                }
                else if (field == "QSO_DATE_OFF")
                {

                    preparedQuery.bindValue( ":qso_date_off", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }

                else if (field == "BAND_RX")
                {
                    i = dataProxy->getIdFromBandName(data);
                    //i = db->getBandIDFromName2(data);
                    if (i>=0)
                    {
                        preparedQuery.bindValue( ":band_rx", QString::number(i) );
                        bandRXDef = true;
                        bandrxi = i;
                    }
                    /*
                    queryString = QString("SELECT id FROM band WHERE name ='%1'").arg(data);
                    query.exec(queryString);
                    query.next();
                    if (query.isValid())
                    {

                        preparedQuery.bindValue( ":band_rx", query.value(0).toInt() );
                    }
                    */
                }

                else if (field == "TIME_OFF")
                {
                    if (data.length() == 4)
                    {
                        data = data + "00";
                    }

                    preparedQuery.bindValue( ":time_off", (time.fromString(data,"hhmmss")).toString("hh:mm:ss") );
                }

                else if (field == "RST_SENT")
                {
                    preparedQuery.bindValue( ":rst_sent", data );
                    rstTXr = true;
                }

                else if (field == "RST_RCVD")
                {
                    //qDebug() << "FileManager::bprocessQsoReadingADIF-rst_rcvd: " << data << endl;
                    preparedQuery.bindValue( ":rst_rcvd", data );
                    rstRXr = true;
                }

                else if (field == "SRX_STRING")
                {
                    preparedQuery.bindValue( ":srx_string", data );
                }
                else if (field == "STX_STRING")
                {
                    preparedQuery.bindValue( ":stx_string", data );
                }
                else if (field == "CQZ")
                {
                    preparedQuery.bindValue( ":cqz", data );
                    cqz = data.toInt();

                }
                else if (field == "ITUZ")
                {
                    preparedQuery.bindValue( ":ituz", data );
                    ituz = data.toInt();
                }
                else if (field == "DXCC")
                {
                    dxcc = data.toInt();
                    preparedQuery.bindValue( ":dxcc", data );

                }
                else if (field == "ADDRESS")
                {
                    preparedQuery.bindValue( ":address", data );
                }
                else if (field == "AGE")
                {
                    preparedQuery.bindValue( ":age", data );
                }
                else if (field == "CNTY")
                {
                    preparedQuery.bindValue( ":cnty", data );
                }
                else if (field == "COMMENT")
                {
                    preparedQuery.bindValue( ":comment", data );
                }
                else if (field == "A_INDEX")
                {
                    preparedQuery.bindValue( ":a_index", data );
                }
                else if (field == "ANT_AZ")
                {
                    preparedQuery.bindValue( ":ant_az", data );
                }
                else if (field == "ANT_EL")
                {
                    preparedQuery.bindValue( ":ant_el", data );
                }
                else if (field == "ANT_PATH")
                {
                    preparedQuery.bindValue( ":ant_path", data );
                }
                else if (field == "ARRL_SECT")
                {
                    preparedQuery.bindValue( ":arrl_sect", data );
                }
                else if (field == "AWARD_GRANTED")
                {
                    preparedQuery.bindValue( ":award_granted", data );
                }
                else if (field == "AWARD_SUBMITTED")
                {
                    preparedQuery.bindValue( ":award_submitted", data );
                }
                else if (field == "CHECKCONTEST")
                {
                    preparedQuery.bindValue( ":checkcontest", data );
                }
                else if (field == "CLASS")
                {
                    preparedQuery.bindValue( ":class", data );
                }
                else if (field == "CONT")
                {
                    preparedQuery.bindValue( ":cont", data );
                }
                else if (field == "CONTACTED_OP")
                {
                    preparedQuery.bindValue( ":contacted_op", data );
                }
                else if (field == "CONTEST_ID")
                {
                    preparedQuery.bindValue( ":contest_id", data );
                }
                else if (field == "COUNTRY")
                {
                    preparedQuery.bindValue( ":country", data );
                }
                else if (field == "CREDIT_SUBMITTED")
                {
                    preparedQuery.bindValue( ":credit_submitted", data );
                }
                else if (field == "CREDIT_GRANTED")
                {
                    preparedQuery.bindValue( ":credit_granted", data );

                }
                else if (field == "DISTANCE")
                {
                    preparedQuery.bindValue( ":distance", data );
                }
                else if (field == "DARK_DOK")
                {
                    preparedQuery.bindValue( ":dark_dok", data );
                }
                else if (field == "EQ_CALL")
                {
                    preparedQuery.bindValue( ":eq_call", data );
                }
                else if (field == "EMAIL")
                {
                    if (data.contains("@") && (data.contains(".")))
                    {
                       preparedQuery.bindValue( ":email", data );
                    }

                }
                else if (field == "EQSL_QSLRDATE")
                {
                    preparedQuery.bindValue( ":eqsl_qslrdate", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }
                else if (field == "EQSL_QSLSDATE")
                {
                    preparedQuery.bindValue( ":eqsl_qslsdate", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }
                else if (field == "EQSL_QSL_RCVD")
                {
                    preparedQuery.bindValue( ":eqsl_qsl_rcvd", data );
                }
                else if (field == "EQSL_QSL_SENT")
                {
                    preparedQuery.bindValue( ":eqsl_qsl_sent", data );
                }
                else if (field == "FISTS")
                {
                    preparedQuery.bindValue( ":fists", data );
                }
                else if (field == "FISTS_CC")
                {
                    preparedQuery.bindValue( ":fists_cc", data );
                }
                else if (field == "FORCE_INIT")
                {
                    preparedQuery.bindValue( ":force_init", data );
                }
                else if (field == "FREQ")
                {
                    //qDebug() << "FileManager::processQsoReadingADIF  -FREQ: " << QString::number(data.toDouble()) << endl;
                    preparedQuery.bindValue( ":freq", data);
                    if (haveBand)
                    {
                        if (dataProxy->getBandIdFromFreq(data.toDouble()) == bandi)
                        //if (db->isThisFreqInBand(db->getBandNameFromNumber(bandi), data))
                        {
                            preparedQuery.bindValue( ":freq", data);
                        }
                        else
                        {
                            // IF band is defined but not the same than freq, Band wins
                        }
                    }
                    else
                    {
                        preparedQuery.bindValue( ":freq", data);
                        i = dataProxy->getBandIdFromFreq(data.toDouble());

                        if (i>=0)
                        {
                            preparedQuery.bindValue( ":bandid", QString::number(i) );
                            haveBand = true;
                            //qDebug() << "FileManager::processQsoReadingADIF-Band: " << data << "/"  << QString::number(i) << endl;
                        }
                    }
                }
                else if (field == "FREQ_RX")
                {
                    //haveBand = true;
                    //bandi = i;
                    //i = db->getBandIDFromName2(data);
                    if (bandRXDef)
                    {
                        if (dataProxy->getBandIdFromFreq(data.toDouble()) == bandrxi)
                        //if (db->isThisFreqInBand(db->getBandNameFromNumber(bandrxi), data))
                        {
                            preparedQuery.bindValue( ":freq_rx", data);
                        }
                        else
                        {
                            // IF band is defined but not the same than freq, Band wins
                        }
                    }
                    else
                    {
                        preparedQuery.bindValue( ":freq_rx", data);
                    }

                }
                else if (field == "GRIDSQUARE")
                {
                    preparedQuery.bindValue( ":gridsquare", data );
                }
                else if (field == "GUEST_OP")
                {
                    preparedQuery.bindValue( ":guest_op", data );
                }
                else if (field == "HRDLOG_QSO_UPLOAD_DATE")
                {
                    preparedQuery.bindValue( ":hrd_qso_upload_date", data );
                }
                else if (field == "HRDLOG_QSO_UPLOAD_STATUS")
                {
                    preparedQuery.bindValue( ":hrd_qso_upload_status", data );
                }
                else if (field == "MY_GRIDSQUARE")
                {
                    preparedQuery.bindValue( ":my_gridsquare", data );
                }
                else if (field == "MY_ANTENNA")
                {
                    preparedQuery.bindValue( ":my_antenna", data );
                }
                else if (field == "IOTA")
                {
                    //qDebug() << "FileManager::processQsoReadingADIF (IOTA): " << data << endl;
                    data = awards->checkIfValidIOTA(data);
                    /*
                    if (data.length()==4) //EU-1
                    {
                        data.insert(3, "00");
                    }
                    else if (data.length()==5) //EU-01
                    {
                        data.insert(3, "0");
                    }
                    */
                    if (data.length() == 6)
                    {
                        preparedQuery.bindValue( ":iota", data );
                    }
                }
                else if (field == "IOTA_ISLAND_ID")
                {
                    preparedQuery.bindValue( ":iota_island_id", data );
                }
                else if (field == "MY_IOTA")
                {
                    /*
                    if (data.length()==4) //EU-1
                    {
                        data.insert(3, "00");
                    }
                    else if (data.length()==5) //EU-01
                    {
                        data.insert(3, "0");
                    }
                    */
                    data = awards->checkIfValidIOTA(data);
                    if (data.length() == 6)
                    {
                        preparedQuery.bindValue( ":my_iota", data );
                    }
                }
                else if (field == "MY_DXCC")
                {
                    preparedQuery.bindValue( ":my_dxcc", data );
                }
                else if (field == "MY_FISTS")
                {
                    preparedQuery.bindValue( ":my_fists", data );
                }
                else if (field == "MY_IOTA_ISLAND_ID")
                {
                    preparedQuery.bindValue( ":my_iota_island_id", data );
                }
                else if (field == "K_INDEX")
                {
                    preparedQuery.bindValue( ":k_index", data );
                }
                else if (field == "LAT")
                {
                    preparedQuery.bindValue( ":lat", data );
                }
                else if (field == "LON")
                {
                    preparedQuery.bindValue( ":lon", data );
                }
                else if (field == "MY_LAT")
                {
                    preparedQuery.bindValue( ":my_lat", data );
                }
                else if (field == "MY_LON")
                {
                    preparedQuery.bindValue( ":my_lon", data );
                }
                else if (field == "MY_ITU_ZONE")
                {
                    preparedQuery.bindValue( ":my_itu_zone", data );
                }
                else if (field == "MY_POSTAL_CODE")
                {
                    preparedQuery.bindValue( ":my_postal_code", data );
                }
                else if (field == "LOTW_QSLRDATE")
                {
                    preparedQuery.bindValue( ":lotw_qslrdate", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }
                else if (field == "LOTW_QSLSDATE")
                {
                    preparedQuery.bindValue( ":lotw_qslsdate", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }
                else if (field == "LOTW_QSL_RCVD")
                {
                    preparedQuery.bindValue( ":lotw_qsl_rcvd", data );
                }
                else if (field == "LOTW_QSL_SENT")
                {
                    preparedQuery.bindValue( ":lotw_qsl_sent", data );
                }
                else if (field == "CLUBLOG_QSO_UPLOAD_DATE")
                {
                    preparedQuery.bindValue( ":clublog_qso_upload_date", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }
                else if (field == "CLUBLOG_QSO_UPLOAD_STATUS")
                {
                    preparedQuery.bindValue( ":clublog_qso_upload_status", data );
                }
                else if (field == "MAX_BURSTS")
                {
                    preparedQuery.bindValue( ":max_bursts", data );
                }
                else if (field == "MS_SHOWER")
                {
                    preparedQuery.bindValue( ":ms_shower", data );
                }
                else if (field == "MY_CITY")
                {
                    preparedQuery.bindValue( ":my_city", data );
                }
                else if (field == "MY_CNTY")
                {
                    preparedQuery.bindValue( ":my_cnty", data );
                }
                else if (field == "MY_COUNTRY")
                {
                    preparedQuery.bindValue( ":my_country", data );
                }
                else if (field == "MY_CQ_ZONE")
                {
                    preparedQuery.bindValue( ":my_cq_zone", data );
                }
                else if (field == "MY_NAME")
                {
                    preparedQuery.bindValue( ":my_name", data );
                }
                else if (field == "NAME")
                {
                    preparedQuery.bindValue( ":name", data );
                }
                else if (field == "OPERATOR")
                {
                    preparedQuery.bindValue( ":operator", data );
                }
                else if (field == "STATION_CALLSIGN")
                {
                    preparedQuery.bindValue( ":station_callsign", data );
                }
                else if (field == "OWNER_CALLSIGN")
                {
                    preparedQuery.bindValue( ":owner_callsign", data );
                }
                else if (field == "MY_RIG")
                {
                    preparedQuery.bindValue( ":my_rig", data );
                }
                else if (field == "MY_SIG")
                {
                    preparedQuery.bindValue( ":my_sig", data );
                }
                else if (field == "MY_SIG_INFO")
                {
                    preparedQuery.bindValue( ":my_sig_info", data );
                }
                else if (field == "MY_SOTA_REF")
                {
                    preparedQuery.bindValue( ":my_sota_ref", data );
                }
                else if (field == "MY_STATE")
                {
                    preparedQuery.bindValue( ":my_state", data );
                }
                else if (field == "STATE")
                {
                    preparedQuery.bindValue( ":state", data );
                }
                else if (field == "MY_STREET")
                {
                    preparedQuery.bindValue( ":my_street", data );
                }
                else if (field == "MY_USACA_COUNTIES")
                {
                    preparedQuery.bindValue( ":my_usaca_counties", data );
                }
                else if (field == "MY_VUCC_GRIDS")
                {
                    preparedQuery.bindValue( ":my_vucc_grids", data );
                }
                else if (field == "NOTES")
                {
                    preparedQuery.bindValue( ":notes", data );
                }
                else if (field == "NR_BURSTS")
                {
                    preparedQuery.bindValue( ":nr_bursts", data );
                }
                else if (field == "NR_PINGS")
                {
                    preparedQuery.bindValue( ":nr_pings", data );
                }
                else if (field == "PFX")
                {
                    preparedQuery.bindValue( ":pfx", data );
                }
                else if (field == "PRECEDENCE")
                {
                    preparedQuery.bindValue( ":precedence", data );
                }
                else if (field == "PROP_MODE")
                {
                    preparedQuery.bindValue( ":prop_mode", data );
                }
                else if (field == "PUBLIC_KEY")
                {
                    preparedQuery.bindValue( ":public_key", data );
                }
                else if (field == "QRZCOM_QSO_UPLOAD_DATE")
                {
                    preparedQuery.bindValue( ":qrzcom_qso_upload_date", data );
                }
                else if (field == "QRZCOM_QSO_UPLOAD_STATUS")
                {
                    preparedQuery.bindValue( ":qrzcom_qso_upload_status", data );
                }
                else if (field == "QSLMSG")
                {
                    preparedQuery.bindValue( ":qslmsg", data );
                }
                else if (field == "QSLRDATE")
                {
                    preparedQuery.bindValue( ":qslrdate", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }
                else if (field == "QSLSDATE")
                {
                    preparedQuery.bindValue( ":qslsdate", (date.fromString(data, "yyyyMMdd")).toString("yyyy/MM/dd") );
                }
                else if (field == "QSL_RCVD")
                {
                    preparedQuery.bindValue( ":qsl_rcvd", data );
                    //preparedQuery.bindValue( ":confirmed", '1' );
                }
                else if (field == "QSL_SENT")
                {
                    preparedQuery.bindValue( ":qsl_sent", data );
                }
                else if (field == "QSL_RCVD_VIA")
                {
                    preparedQuery.bindValue( ":qsl_rcvd_via", data );
                }
                else if (field == "QSL_SENT_VIA")
                {
                    preparedQuery.bindValue( ":qsl_sent_via", data );
                }
                else if (field == "QSL_VIA")
                {

                    //qDebug() << "FileManager::bprocessQsoReadingADIF-QSL_VIA: " << data << endl;

                    if (data == "BUREAU") // This comprobation is to "correct" old logs, mainly from KLog
                        // comming from older ADIF files
                    {
                        preparedQuery.bindValue( ":qsl_sent_via", "B" );
                    }
                    else if ( (data == "B") || (data == "D") || (data == "E") )// M Deprecated value from ADIF 304
                    {
                       preparedQuery.bindValue( ":qsl_via", data );
                    }
                    else
                    {
                        // If values are not valid, are not imported.
                        //TODO: Send an error to the user to show that there was an invalid field
                    }
                }
                else if (field == "QSO_COMPLETE")
                {
                    preparedQuery.bindValue( ":qso_complete", data );
                }
                else if (field == "QSO_RANDOM")
                {
                    preparedQuery.bindValue( ":qso_random", data );
                }
                else if (field == "QTH")
                {
                    preparedQuery.bindValue( ":qth", data );
                }
                else if (field == "REGION")
                {
                    preparedQuery.bindValue( ":region", data );
                }
                else if (field == "RIG")
                {
                    preparedQuery.bindValue( ":rig", data );
                }
                else if (field == "RX_PWR")
                {
                    //qDebug() << "FileManager::bprocessQsoReadingADIF-rx_pwr: " << data << endl;
                    preparedQuery.bindValue( ":rx_pwr", data);
                }
                else if (field == "TX_PWR")
                {
                    //qDebug() << "FileManager::bprocessQsoReadingADIF-tx_pwr: " << data << endl;
                    preparedQuery.bindValue( ":tx_pwr", data);
                }
                else if (field == "SAT_MODE")
                {
                    preparedQuery.bindValue( ":sat_mode", data );
                }
                else if (field == "SAT_NAME")
                {
                    preparedQuery.bindValue( ":sat_name", data );
                }
                else if (field == "SFI")
                {
                    preparedQuery.bindValue( ":sfi", data );
                }
                else if (field == "SIG")
                {
                    preparedQuery.bindValue( ":sig", data );
                }
                else if (field == "SIG_INFO")
                {
                    preparedQuery.bindValue( ":sig_info", data );
                }
                else if (field == "SILENT_KEY")
                {
                    preparedQuery.bindValue( ":silent_key", data );
                }
                else if (field == "SKCC")
                {
                    preparedQuery.bindValue( ":skcc", data );
                }
                else if (field == "SOTA_REF")
                {
                    preparedQuery.bindValue( ":sota_ref", data );
                }
                else if (field == "STATE")
                {
                    preparedQuery.bindValue( ":state", data );
                }
                else if (field == "SUBMODE")
                {
                    preparedQuery.bindValue( ":submode", data );
                }
                else if (field == "SWL")
                {
                    preparedQuery.bindValue( ":swl", data );
                }
                else if (field == "TEN_TEN")
                {
                    preparedQuery.bindValue( ":ten_ten", data );
                }
                else if (field == "UKSMG")
                {
                    preparedQuery.bindValue( ":uksmg", data );
                }
                else if (field == "USACA_COUNTIES")
                {
                    preparedQuery.bindValue( ":usaca_counties", data );
                }
                else if (field == "VE_PROV")
                {
                    preparedQuery.bindValue( ":ve_prov", data );
                }
                else if (field == "VUCC_GRIDS")
                {
                    preparedQuery.bindValue( ":vucc_grids", data );
                }
                else if (field == "WEB")
                {
                    preparedQuery.bindValue( ":web", data );
                }
                else if (field == "APP_KLOG_POINTS") //Importing own ADIF fields
                {
                    preparedQuery.bindValue( ":points", data );
                }
                else if (field == "APP_KLOG_MULTIPLIER") //Importing own ADIF fields
                {
                    preparedQuery.bindValue( ":multiplier", data );
                }
                else if (field == "APP_KLOG_TRX") //Importing own ADIF fields
                {
                    preparedQuery.bindValue( ":transmiterid", data );
                }
                else if (field == "APP_KLOG_LOGN") //Lognumber in a multiple-log file
                {
                    //TODO: Think about how to import a file with different logs
                    //isThisQSODuplicated(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode)
                }
                else if (field == "APP_N1MM_POINTS") //Importing from N1MM
                {
                    preparedQuery.bindValue( ":points", data );
                }
                else
                {
                }
            }
            else
            {
                //qDebug() << "FileManager::processQsoReadingADIF (field) CheckAdif FALSE: " << field << endl;
            }
        }
    }
    //preparedQuery.bindValue( ":lognumber", i);
   //qDebug() << "FileManager::processQsoReadingADIF: logNumber: " << QString::number(logNumber) << endl;

   // if ((haveSubMode) && (!haveMode))
   // { // We can guess the mode from a submode!

   //     preparedQuery.bindValue( ":mode", dataProxy->getIdFromModeName(dataProxy->getModeFromSubMode(submode)) );
   //     haveMode  = true;
   // }

    if (!(haveBand && haveCall && haveMode && haveTime && haveDate ))
    {
        aux2 = tr ("This QSO is not including the minimum data to consider a QSO as valid!.\n\n\nPlease edit the ADIF file and make sure that it include at least:\n\nCALL, QSO_DATE, TIME_ON, BAND and MODE.\n\nThis QSO had:\n");

        if (!haveBand)
        {
            aux2 = aux2 + tr(" - The band missing and the following call: ") + qrzCall + ".\n";
            //aux2 = "Band missing " + qrzCall + " ";
        }
        if (!haveCall)
        {
            aux2 = aux2 + tr(" - The call missing but was done at this time: ") + aux3 + ".\n";
            //aux2 = "Call missing " + aux3 + " ";
        }
        if (!haveMode)
        {
            aux2 = aux2 + tr(" - The mode missing and the following call: ") + qrzCall + ".\n";
            //aux2 = "Mode missing " + qrzCall + " ";
        }
        if (!haveDate)
        {
            aux2 = aux2 + tr(" - The date missing and the following call: ") + qrzCall + ".\n";
            //aux2 = "Date missing " + qrzCall + " ";
        }
        if (!haveTime)
        {
            aux2 = aux2 + tr(" - The time missing and the following call: ") + qrzCall + ".\n";
            //aux2 = "Time missing " + qrzCall + " ";
        }


        aux2 = aux2 + tr("\n\nDo you want to continue with the current file?");
       //qDebug() << "FileManager::processQsoReadingADIF - Missing fields: " << aux2 << endl;

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog: Not all required data found!"));
        msgBox.setText(aux2);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Yes:
              // Yes was clicked

              break;
        case QMessageBox::Cancel:
            noMoreQso = true;
            break;
          default:
              // should never be reached
              break;
        }

    }

    if ((!rstTXr) && (!rstTXDefault))
    {
        QMessageBox msgBox;
        aux = tr("This log seems to lack of RST-TX information.\n\nClick on Yes to add a default 59 to all QSO with a similar problem.\n\nIf you select NO, the QSO may not be imported.");
        msgBox.setWindowTitle(tr("KLog: No RST TX found!"));
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Yes:
              // Yes was clicked
                rstTXDefault = true;
              break;
          default:
              // should never be reached
                rstTXDefault = false;
              break;
        }
    }

    if ((!rstRXr) && (!rstRXDefault))
    {
        QMessageBox msgBox;
        aux = tr("This log seems to lack of RST-RX information.\n\nClick on Yes to add a default 59 to all QSO with a similar problem.\n\nIf you select NO, the QSO may not be imported.");
        msgBox.setText(aux);
        msgBox.setWindowTitle(tr("KLog: No RST RX found!"));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Yes:
              // Yes was clicked
                rstRXDefault = true;
              break;
          default:
              // should never be reached
                rstRXDefault = false;
              break;
        }
    }

    if ((!rstTXr) && (rstTXDefault))
    {
        preparedQuery.bindValue( ":rst_sent", "59" );

    }

    if ((!rstRXr) && (rstRXDefault))
    {

        preparedQuery.bindValue( ":rst_rcvd", "59" );
    }


    preparedQuery.bindValue( ":lognumber", QString::number(logNumber));


    return true;

}


void FileManager::queryPreparation(const int _logN)
{
    //qDebug() << "FileManager::queryPreparation log: " << QString::number(_logN) << endl;
    //Prepares the query, ALL fields to default except lognumber, as may change...

    QStringList columns;
    columns.clear();

    columns << dataProxy->getColumnNamesFromTableLog();
    if (columns.size()<2)
    {
        //qDebug() << "FileManager::queryPreparation: <2 "  << endl;
        return;
    }


    for (int i=0;i<columns.size();i++)
    {
        if (columns.at(i) == "lognumber")
        {
            preparedQuery.bindValue( ":lognumber", QString::number(_logN));
        }
        else if (columns.at(i) == "rx_pwr")
        {
            preparedQuery.bindValue( ":rx_pwr", QString::number(0.0));
        }
        else if (columns.at(i) == "tx_pwr")
        {
            preparedQuery.bindValue( ":tx_pwr", QString::number(0.0));
        }

        else
        {
            preparedQuery.bindValue(":" + columns.at(i), "");
        }
    }
}

bool FileManager::adifReqQSLExport(const QString& _fileName)
{
    return adifLogExportToFile(_fileName, 0, false, true);
}


bool FileManager::modifySetupFile(const QString& _filename, QString _field, const QString _value)
{
   //qDebug() << "FileManager::modifySetupFile" << endl;


    QFile file(_filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){
       //qDebug() << "FileManager::modifySetupFile File not found" << _filename << endl;
        return false;
    }

    QTemporaryFile tmp;
    if (!tmp.open()) {
          //qDebug() << "FileManager::modifySetupFile- Temp file not opened" << endl;
           return false;
    }

    QString line = QString();
    QTextStream in(&file);
    QTextStream out(&tmp);
    qint64 pos1 = in.pos();
    qint64 pos2 = out.pos();

    out << in.readAll();

    in.seek(pos1);
    out.seek(pos2);


    while (!out.atEnd())
    {
        line = out.readLine();
        //qDebug() << "FileManager::modifySetupFile- Temp file: " << line << endl;
        if (line.startsWith(_field))
        {
            in << _field << "=" << _value << ";" << endl;
        }
        else
        {
            in << line << endl;
        }
    }
    return true;
}

int FileManager::howManyLogsInFile(QFile& _f)
{
   //qDebug() << "FileManager::howManyLogsInFile:" << endl;
    QFile &file = _f;
    qint64 pos; //Position in the file
    QString line = QString();
    QStringList fields = QStringList();
    QStringList logs = QStringList();
    QString aux = QString();
    QStringList data = QStringList();
    bool atLeastOneLog = false;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "FileManager::howManyLogsInFile File not found" << endl;
        return false;
    }

    pos = file.pos();

    while ( !file.atEnd()   )
    {
        //line = file.readLine();

        line.clear();
        line.append(file.readLine().trimmed().toUpper());

        if (line.contains("APP_KLOG_LOGN"))
        {   fields.clear();
            fields << line.split("<", QString::SkipEmptyParts);

            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if (aux.contains("APP_KLOG_LOGN"))
                {
                    data.clear();
                    data << aux.split('>');
                   //qDebug() << "FileManager::howManyLogsInFile: data.0: " << data.at(0) << endl;
                   //qDebug() << "FileManager::howManyLogsInFile: data.1: " << data.at(1) << endl;
                    if (logs.contains(data.at(1)))
                    {

                    }
                    else
                    {
                        logs.append(data.at(1));
                    }
                }
            }
        }
        else if (line.contains("EOR"))
        {
            atLeastOneLog = true;
        }
        else
        {

        }
    }
    //qDebug() << "FileManager::howManyLogsInFile: JUST ONE!" << aux << endl;
    file.close();

    if ( (logs.size()<2) && atLeastOneLog )
    {
        return 1;
    }
    return logs.size();
}


bool FileManager::fillHashLog(QFile &_f)
{
    //qDebug() << "FileManager::fillHashLog:" << endl;
   //hashLogs
    //1.- Run the log and fill hashLogs
    //2.- Run the file and continue the log series to be able to translate the files log into our own log
    hashLogs.clear();
    QStringList qs;
    qs.clear();
    qs << dataProxy->getListOfManagedLogs();
    int i = 0;
    for (i=0;i<qs.size();i++)
    {
        //qDebug() << "FileManager::fillHashLog: - log: " << QString::number(qs.at(i).toInt()) << endl;
        hashLogs.insert(i+1, (qs.at(i)).toInt() );
    }

    for (i = 0; i < hashLogs.size(); ++i)
    {
        //qDebug() << "FileManager::fillHashLog: - hashLogs: " << QString::number(hashLogs[i]) << endl;
    }

    qs.clear();
    qs << getListOfLogsInFile(_f);


    int j = i;

    for (int i=0; i<qs.size();i++)
    {
        hashLogs.insert(j+i, (qs.at(i)).toInt());
    }

    for (i=0; i<hashLogs.size();i++)
    {
        //qDebug() << "FileManager::fillHashLog: (" << QString::number(hashLogs.value(i)) << "/" << QString::number(i) << ")" << endl;
    }
    return true;
}

QStringList FileManager::getListOfLogsInFile(QFile& _f)
{
    // Returns the logs of the file + 10.000 (So, log 1 is returned as 10001, 2 as 10.002 and so on)
    //qDebug() << "FileManager::getListOfLogsInFile:" << endl;
    QFile &file = _f;
    //qint64 pos; //Position in the file
    QString line = QString();
    QStringList fields = QStringList();
    QStringList logs = QStringList();
    QString aux = QString();
    QStringList data = QStringList();
    bool atLeastOneLog = false;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "FileManager::howManyLogsInFile File not found" << endl;
        return QStringList();
    }

    //pos = file.pos();

    while ( !file.atEnd()   )
    {
        //line = file.readLine();

        line.clear();
        line.append(file.readLine().trimmed().toUpper());

        if (line.contains("APP_KLOG_LOGN"))
        {   fields.clear();
            fields << line.split("<", QString::SkipEmptyParts);

            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if (aux.contains("APP_KLOG_LOGN"))
                {
                    data.clear();
                    data << aux.split('>');
                    //qDebug() << "FileManager::howManyLogsInFile: data.0: " << data.at(0) << endl;
                    //qDebug() << "FileManager::howManyLogsInFile: data.1: " << data.at(1) << endl;
                    if (logs.contains(QString::number(data.at(1).toInt()+10000)))
                    {

                    }
                    else
                    {
                        logs.append(QString::number(data.at(1).toInt()+10000));
                    }
                }
            }
        }
        else if (line.contains("EOR"))
        {
            atLeastOneLog = true;
        }
        else
        {

        }
    }

    file.close();

    return logs;
}

void FileManager::setVersion(const QString _version)
{
    util->setVersion(_version);
}

QStringList FileManager::readAdifField (const QString _field)
{
    // This function receives a QString with an ADIF field and returns a QString with the following format:
    // ADIF-tag, value
    // If the QString is not an ADIF tag, it returns a clear QStringList.
    // We are expecting the ADIF format:
    // <F:L:T>D
    // <Field:Length:Type>Data
/*
    //qDebug() << "FileManager::readAdifField: " << _field << endl;
    QStringList result;
    result.clear();

    if (!((_field.startsWith("<")) && (_field.contains(":")) && (_field.contains(">"))))
    {
        //qDebug() << "FileManager::readAdifField: NOT (contains : and >): " << str << endl;
        return QStringList();
    }

    // Now we have the data in the result[1]
    result = _field.split(">", QString::SkipEmptyParts);
    QString aux = result.at(0);
    QString data = result.at(1);
    QString type = QString();
    int length = -1;
    QString fname = QString();

    if ( !((aux.endsWith(">")) && !(aux.contains(":"))) )
    {
        return QStringList();
    }

    int i = aux.count(":");

    if (i == 1)
    {
        lenght = (aux.section(':', 1, 1)).toInt();
        fname = aux.section(':', 0, 0);
    }
    else if (i == 2)
    {
        lenght = (aux.section(':', 1, 1)).toInt();
        fname = aux.section(':', 0, 0);
        type = aux.section(':', 2, 2);
    }
    else
    {
        //qDebug() << "FileManager::readAdifField: NO proper format(1): " << str << endl;
        return QStringList();
    }

    if (length<1)
    {
        //qDebug() << "FileManager::readAdifField: NO proper format(2): " << str << endl;
        return QStringList();
    }



    // Now data is splitted in the appropriate variables. We start checking format!
    result.at(0) = fname;

    if (fname == "QSO_DATE")
    {
        QDate date;
        date.fromString(data, "yyyyMMdd");
        if (date.isValid())
        {
            result.at(1) = data;
            return result;
        }
        else
        {
            //qDebug() << "FileManager::readAdifField: NO proper format(date): " << str << endl;
            return QStringList();
        }
    }
    else if (fname == "TIME_ON")
    {
        QTime time;

        if (data.length() == 4)
        {
            data = data + "00";
        }
        time.fromString(data,"hhmmss");
        if (time.isValid())
        {
            result.at(1) = data;
            return result;
        }
        else
        {
            //qDebug() << "FileManager::readAdifField: NO proper format(time): " << str << endl;
            return QStringList();
        }

    }
    else if (fname == "BAND")
    {
        if (dataProxy->getIdFromBandName(data)>0)
        {
            result.at(1) = data;
            return result;
        }
        else
        {
            //qDebug() << "FileManager::readAdifField: NO proper data(BAND): " << str << endl;
            return QStringList();
        }
    }
    else if (fname == "MODE")
    {
        if (dataProxy->getSubModeIdFromSubMode(data)>0)
        {
            result.at(1) = data;
            return result;
        }
        else
        {
            //qDebug() << "FileManager::readAdifField: NO proper data(BAND): " << str << endl;
            return QStringList();
        }
    }
    else if (fname == "QSL_RCVD")
    {

    }
    else if (fname == "QSLRDATE")
    {
        QDate date;
        date.fromString(data, "yyyyMMdd");
        if (date.isValid())
        {
            result.at(1) = data;
            return result;
        }
        else
        {
            //qDebug() << "FileManager::readAdifField: NO proper format(qslrdate): " << str << endl;
            return QStringList();
        }
    }


    else
    {
        //qDebug() << "FileManager::readAdifField: NO Field found: " << str << endl;
        return QStringList();
    }
    //qDebug() << "FileManager::readAdifField: NO Field found-2: " << str << endl;
      */
    return QStringList();

}

QString FileManager::prepareStringLog()
{
    //qDebug() << "FileManager::prepareStringLog: " << endl;
    QStringList columns;
    columns.clear();

    columns << dataProxy->getColumnNamesFromTableLog();
    if (columns.size()<2)
    {
        //qDebug() << "FileManager::prepareStringLog: <2 "  << endl;
        return QString();
    }

    QString queryFields, queryValues;
    queryFields.clear();
    queryValues.clear();

    for (int i=0;i<columns.size();i++)
    {
        queryFields = queryFields + columns.at(i) + ", ";
        queryValues = queryValues + ":" + columns.at(i) + ", ";
    }
    queryFields = queryFields.left(queryFields.size() -2);
    queryValues = queryValues.left(queryValues.size() -2);

    QString string = "INSERT INTO log ("  + queryFields + ") VALUES (" + queryValues + ")";
    //qDebug() << "FileManager::prepareStringLog: " << string << endl;
    return string;

}


