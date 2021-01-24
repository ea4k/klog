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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
#include "filemanager.h"
//#include <QDebug>


FileManager::FileManager(DataProxy_SQLite *dp)
{
       //qDebug() << "FileManager::FileManager()-1" << endl;
    constrid = 1;
    dataProxy = dp;
    dbCreated = false;
    rstTXDefault  = false;
    rstRXDefault = false;
    ignoreUnknownAlways = false;
    noMoreQso = false;
    defaultStationCallsign = QString();
    duplicatedQSOSlotInSecs = 0;

    util = new Utilities;
    //qso = new QSO;
    klogVersion = util->getVersion();
    db = new DataBase(Q_FUNC_INFO, klogVersion, util->getKLogDBFile());

    usePreviousStationCallsignAnswerAlways = false;
    world = new World(dataProxy, Q_FUNC_INFO);
    awards = new Awards(dataProxy, Q_FUNC_INFO);
    hashLogs.clear();
    //qDebug() << "FileManager::FileManager()-1  - END" << endl;

}


FileManager::FileManager(DataProxy_SQLite *dp, const QString &_klogDir, const QString &_softVersion)
//FileManager::FileManager(const QString _klogDir, const QString _softVersion, DataBase _db)
{
       //qDebug() << "FileManager::FileManager()-3: Dir(2)" << _klogDir << endl;
    constrid = 2;
    dataProxy = dp;
    util = new Utilities;
    util->setVersion(klogVersion);
    defaultStationCallsign = QString();
    dbCreated = false;
    rstTXDefault  = false;
    rstRXDefault = false;
    duplicatedQSOSlotInSecs = 0;
    db = new DataBase(Q_FUNC_INFO, klogVersion, util->getKLogDBFile());

    klogVersion = _softVersion;
    //dataProxyPrepared = new DataProxy_SQLite(klogVersion);

    klogDir = _klogDir;
    ignoreUnknownAlways = false;
    usePreviousStationCallsignAnswerAlways = false;
    world = new World(dataProxy, klogDir, Q_FUNC_INFO);
    awards = new Awards(dataProxy, Q_FUNC_INFO);

    noMoreQso = false;
    hashLogs.clear();
       //qDebug() << "FileManager::FileManager()-3: Dir(2) - END"  << endl;
}

FileManager::~FileManager()
{

}

void FileManager::setDuplicatedQSOSlot (const int _secs)
{
    if (_secs >= 0)
    {
        duplicatedQSOSlotInSecs = _secs;
    }
}

bool FileManager::checkADIFValidFormat(const QStringList &_qs)
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
    //qDebug() << "FileManager::adifLogExport" << _fileName << endl;

    return adifLogExportToFile(_fileName, _logN, false, false, false);

}

void FileManager::showError (const QString &_txt)
{
    //qDebug() << Q_FUNC_INFO << ": "  << _txt << endl;

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - Error"));
    msgBox.setIcon(QMessageBox::Warning);

    QString aux = QString(_txt );
    msgBox.setText(aux);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

}

QList<int> FileManager::adifLogExportReturnList(const QString& _fileName, const QString &_callsign, const QDate &_startDate, const QDate &_endDate, const int _logN, const ExportMode _em)
//QList<int> FileManager::adifLogExportReturnList(const QString& _fileName, const QString &_callsign, const QDate &_startDate, const QDate &_endDate, const int _logN, const bool LoTWOnly)
{
    //qDebug() << Q_FUNC_INFO << ": Start)" << _fileName << "/" << _callsign << endl;
    QList<int> qsos;
    qsos.clear();
    noMoreQso = false;
    if (_logN != -1)
    { // We will export data from ALL logs.
        if (!dataProxy->doesThisLogExist(_logN))
        {
            //qDebug() << Q_FUNC_INFO << ": The log does not exist" << endl;
            showError(tr("The selected log does not exist, please check it again."));
            return qsos;
        }
    }

    QString queryStringCount;
    QString queryString;
    QString _queryStation;

    if (util->isValidCall(_callsign))
    {
         _queryStation = QString(" station_callsign ='%1'").arg(_callsign);
    }
    else if (_callsign == "ALL")
    {
        _queryStation = QString(" station_callsign !='ALL'");
    }
    else
    {
         _queryStation = QString(" station_callsign =''");
    }

    QString _queryDateFrom;

    if (_startDate.isValid())
    {        
         _queryDateFrom = QString(" AND qso_date >= '%1'").arg(util->getDateSQLiteStringFromDate(_startDate));
    }
    else
    {
         _queryDateFrom = QString(" AND qso_date != '1'");
    }

    QString _queryDateTo;
    if (_endDate.isValid())
    {
         _queryDateTo = QString(" AND qso_date <= '%1'").arg(util->getDateSQLiteStringFromDate(_endDate.addDays(1)));
    }
    else
    {
         _queryDateTo = QString();
    }

    QString _queryLog;
    if (_logN == -1)
    {
         _queryLog = QString(" AND logNumber = '%1'").arg(_logN);
    }
    else
    {
         _queryLog = QString();
    }

    if ((!util->isValidCall(_callsign)) && (_callsign != "ALL") && (_callsign !="NOT"))
    {
         showError(tr("The selected callsign (%1) is not valid, please check it again to export the log.").arg(_callsign));
         return qsos;
    }

    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
         showError(tr("The file %1 can't be opened.").arg(_fileName));
         return qsos;
    }

    QSqlQuery query;

    if (_em == ModeLotW)
    {
       //qDebug() << "FileManager::adifLogExportReturnList: Exporting for LoTW" << endl;
        // LoTW Required fields: call sign, UTC Date, UTC time, Mode, Band
        // LoTW Optional fields: RX band, Frecuency TX, frecuency RX, Propagation mode, Satellite

        queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation + QString(" AND lotw_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
        queryString = QString("SELECT id, call, freq, bandid, band_rx, freq_rx, modeid, gridsquare, my_gridsquare, qso_date, prop_mode, sat_name, station_callsign FROM log WHERE") + _queryStation + QString(" AND lotw_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
    }
    else if (_em == ModeClubLog)
    {
        //qDebug() << "FileManager::adifLogExportReturnList: Exporting for ClubLog" << endl;
        queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation + QString(" AND clublog_qso_upload_status='M'") + _queryDateFrom + _queryDateTo;
        queryString = QString("SELECT id, call, rst_sent, rst_rcvd, freq, bandid, band_rx, modeid, qso_date, qsl_rcvd, qslrdate, qslsdate, prop_mode, operator, station_callsign, dxcc, qsl_sent, lotw_qsl_rcvd, credit_granted, notes, qso_date_off FROM log WHERE") + _queryStation + QString(" AND clublog_qso_upload_status='M'") + _queryDateFrom + _queryDateTo;
    }
    else if (_em == ModeEQSL)
    {
        //qDebug() << "FileManager::adifLogExportReturnList: Exporting for eQSL" << endl;
        queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation + QString(" AND eqsl_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
        queryString = QString("SELECT id, call, rst_sent, freq, bandid, modeid, qso_date, prop_mode, operator, station_callsign, sat_name, my_cnty, my_gridsquare, my_lat, my_lon FROM log WHERE") + _queryStation + QString(" AND eqsl_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
    }
    else
    {
        //qDebug() << "FileManager::adifLogExportReturnList: Exporting normal ADIF" << endl;
        if (_callsign == "ALL")
        {
            queryStringCount = QString("SELECT COUNT (id) FROM log");
            queryString = QString("SELECT * FROM log");
        }
        else
        {
            queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation  + _queryDateFrom + _queryDateTo + _queryLog;
            queryString = QString("SELECT * FROM log WHERE") + _queryStation + _queryDateFrom + _queryDateTo + _queryLog;
        }
    }
    //qDebug() << "FileManager::adifLogExportReturnList: Query Count: " << queryStringCount << endl;
    //qDebug() << "FileManager::adifLogExportReturnList: Query data: " << queryString << endl;
    int numberOfQsos = dataProxy->getHowManyQSOInLog(_logN);
    int i = 0;

    bool sqlOK = query.exec(queryStringCount);
   //qDebug() << "FileManager::adifLogExportReturnList: " << query.lastQuery() << endl;
    if (!sqlOK)
    {
        //qDebug() << "FileManager::adifLogExportReturnList: Query Error"  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return qsos;
    }
    else
    {
        query.next();
        if (query.isValid())
        {
            i = (query.value(0)).toInt();
            if (i>0)
            {
                numberOfQsos = i;
                query.finish();
            }
            else if (i == 0)
            {
                showError(tr("There are no QSOs pending to be exported with that station callsign."));
                return qsos;
            }
        }
    }

    QTextStream out(&file);

    int step = util->getProgresStepForDialog(numberOfQsos);
   //qDebug() << "FileManager::adifLogExportReturnList: " << QString::number(step) << endl;

    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.setWindowTitle(tr("Export"));
    progress.setAutoClose(true);


    writeADIFHeader(out, _em, numberOfQsos);

    i = 0;

    sqlOK = query.exec(queryString);
    //qDebug() << "FileManager::adifLogExportReturnList: " << query.lastQuery() << endl;
    if (!sqlOK)
    {
        //qDebug() << "FileManager::adifLogExportReturnList: Query Error"  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return qsos;
    }

    QSqlRecord rec = query.record();
    int nameCol;

   //qDebug() << "FileManager::adifLogExportReturnList: Entering the While..."  << endl;

    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << "FileManager::adifLogExportReturnList: Start of While"  << endl;
        if (query.isValid())
        {
            nameCol = rec.indexOf("id");
            qsos.append((query.value(nameCol)).toInt());
            writeQuery(query, out, _em, false, false, _logN); // JustMarked = false, onlyRequested = false

            i++;
            //qDebug() << "FileManager::adifLogExportReturnList: Start of isValid"  << endl;

        } // END of if (query.isValid())
        else
        {
            //qDebug() << "FileManager::adifLogExportReturnList: Querty NOT isValid"  << endl;
        }

        //qDebug() << "FileManager::adifLogExportReturnList: Checking if cancelled"  << endl;

        if (( (i % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << "FileManager::adifLogExportReturnList: ********************************   UPDATING THE MESSAGE! " << QString::number(i)  << endl;
            QString aux = tr("Exporting ADIF file...\n QSO: %1 / %2 ").arg(i).arg(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);
            //qDebug() << "FileManager::adifLogExportReturnList: ********************************   UPDATING THE MESSAGE: " << aux  << endl;
        }

        if ( progress.wasCanceled() )
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - User cancelled"));
            QString aux = QString(tr("You have canceled the file export. The file will be removed and no data will be exported.") + "\n" + tr("Do you still want to cancel?"));
            msgBox.setText(aux);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            switch (ret)
            {
                case QMessageBox::Yes:
            // Yes was clicked
                noMoreQso = true;
                qsos.clear();
                progress.setValue(numberOfQsos);
                return qsos;
                case QMessageBox::No:
                // No Save was clicked
                break;
                default:
                // should never be reached
                break;
            }
        }

    } // END OF WHILE

    //qDebug() << "FileManager::adifLogExportReturnList: End: " << QString::number(qsos.count()) << endl;
    progress.setValue(numberOfQsos);
    return qsos;
}
bool FileManager::adifQSOsExport(const QString& _fileName, QList<int> _qsos)
{
   //qDebug() << "FileManager::adifQSOsExport: " << _fileName << endl;
    int numberOfQSOs = _qsos.length();
    if (numberOfQSOs<1)
    {
        //TODO: Warn the user NO QSOS TO EXPORT
       //qDebug() << "FileManager::adifQSOsExport: No QSOs received to be exported" << endl;
    }
    noMoreQso = false;
   //qDebug() << "FileManager::adifQSOsExport - 01" << endl;
    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
   //qDebug() << "FileManager::adifQSOsExport - 02" << endl;
    QTextStream out(&file);

    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT * FROM log WHERE id IN (");
    //qDebug() << "FileManager::adifQSOsExport - 10" << endl;
    QString numbers = QString();
    foreach (int i, _qsos)
    {
        //qDebug() << "FileManager::adifQSOsExport - query: " << QString("id = '%1'").arg(i)  << endl;
        numbers = numbers + QString::number(i) ;
        if (i != _qsos.last())
        {
           numbers = numbers + ", ";
        }
    }
    queryString = queryString + numbers + ")";
    //qDebug() << "FileManager::adifQSOsExport: writing the header" << endl;
    writeADIFHeader(out, ModeADIF, _qsos.length());
   //qDebug() << "FileManager::adifQSOsExport: writing the body" << endl;

    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
    //qDebug() << "FileManager::adifQSOsExport: query error: " << query.lastQuery() << endl;
     emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
   //qDebug() << "FileManager::adifQSOsExport: query: " << query.lastQuery() << endl;
    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQSOs, this);
    progress.setMaximum(numberOfQSOs);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Export progress"));
    int currentQso = 0;
    int step = util->getProgresStepForDialog(numberOfQSOs);
    while ( (query.next()) && (!noMoreQso) )
    {
       //qDebug() << "FileManager::adifLogExportToFile -  Just in the While" << endl;
        if (query.isValid())
        {
            //qDebug() << "FileManager::adifLogExportToFile -  Query is Valid" << endl;
             writeQuery(query, out, ModeADIF, false, false, -1);
        } // Closes the isValid
        else {
           //qDebug() << "FileManager::adifLogExportToFile -  Query is NOT Valid" << endl;
        }

       //qDebug() << "FileManager::adifLogExportToFile -  before showing progress: " << QString::number(currentQso) << endl;
        currentQso++;

        if (( (currentQso % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
           //qDebug() << "FileManager::adifLogExportToFile -  Showing progress: "  << endl;
            QString aux1 = tr("Writing ADIF file...\n QSO: ")  + QString::number(currentQso) + "/" + QString::number(numberOfQSOs);
            progress.setLabelText(aux1);
            progress.setValue(currentQso);
        }
       //qDebug() << "FileManager::adifLogExportToFile -  after showing progress (current%Step): " << QString::number(currentQso%step) << endl;

        if ( progress.wasCanceled() )
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - User cancelled"));
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

    }



   //qDebug() << "FileManager::adifQSOsExport - END" << endl;
    return true;
}



bool FileManager::adifLogExportToFile(const QString& _fileName, const int _logN, bool justMarked, bool _qslRequested , bool _lotw)
{
    //adifLogExportToFile(const QString& _fileName, const int _logN=0, bool justMarked = false, bool _qslRequested = false, bool _lotw=false);
    // If _logN = 0, then we will export ALL the logs.
    //qDebug() << "FileManager::adifLogExportToFile: " << _fileName << endl;
    bool exportJustMarkedQSO = justMarked;
    //bool marked = false;
    bool exportOnlyQSLRequested = _qslRequested;
    //bool haveMode = false;
    noMoreQso = false;

    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    QString aux1, aux2, queryString, bandst, bandrxst; // bandst & bandrxst are to check if the band is properly defined
    //int nameCol = 0;
    QSqlQuery query1;
    int numberOfQsos = 0;
    int currentQso = 0;
    noMoreQso = false;
    int step = 1;
    //bool propsat=false; // Just to check if we have added the prop_mode needed by LOTW when SAT QSO
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
            queryString = QString("SELECT COUNT(id) marked FROM log");
        }
        else
        {
            queryString = QString("SELECT COUNT(id) FROM log WHERE lognumber='%1'").arg(_logN);
        }

        bool sqlOK = query.exec(queryString);
        if (!sqlOK)
        {            
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        }

        if (query.next())
        {
            if (query.isValid())
            {
                numberOfQsos = query.value(0).toInt();
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

    //qDebug() << "FileManager::adifLogExportToFile - end get numberOfQsos = " << QString::number(numberOfQsos) << endl;

    step = util->getProgresStepForDialog(numberOfQsos);
    //qDebug() << "FileManager::adifLogExportToFile END -  Steps = " << QString::number(step) << endl;
    //step = getProgresStepForDialog(numberOfQsos);

    writeADIFHeader(out, ModeADIF, numberOfQsos);
    /*
    out << "ADIF v3.1.1 Export from KLog\nhttps://www.klog.xyz/klog\n<PROGRAMVERSION:" << QString::number(klogVersion.length()) << ">" << klogVersion << "\n<PROGRAMID:4>KLOG " << endl;
    out << "<APP_KLOG_QSOS:" << QString::number((QString::number(numberOfQsos)).length()) << ">" << QString::number(numberOfQsos) << endl;

    QDateTime dateTime = (QDateTime::currentDateTime()).toUTC();

        out << "<APP_KLOG_LOG_DATE_EXPORT:" << QString::number((dateTime.toString("yyyyMMdd-hhmm")).length()) << ">" << dateTime.toString("yyyyMMdd-hhmm") << endl;



    out << "<EOH>" << endl;
    */

    if (exportAll)
    {
        if (justMarked)
        {
            queryString = QString("SELECT * FROM log WHERE marked='X'");
        }
        else
        {
            queryString = QString("SELECT * FROM log");
        }
    }
    else
    {
        if (justMarked)
        {
            queryString = QString("SELECT * FROM log WHERE marked='X' AND lognumber='%1'").arg(_logN);
        }
        else
        {
            queryString = QString("SELECT * FROM log WHERE lognumber='%1'").arg(_logN);
        }

    }

    QSqlQuery query;
    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Export progress"));

    QSqlRecord rec = query.record();
    QDateTime date;

    //qDebug() << "FileManager::adifLogExportToFile -  before the While" << endl;

    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << "FileManager::adifLogExportToFile -  Just in the While" << endl;


        if (query.isValid())
        {
            writeQuery(query, out, ModeADIF, exportJustMarkedQSO, exportOnlyQSLRequested, _logN);

        } // Closes the isValid

        //qDebug() << "FileManager::adifLogExportToFile -  before showing progress: " << QString::number(currentQso) << endl;
        currentQso++;
        if (( (currentQso % step )== 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << "FileManager::adifLogExportToFile -  Showing progress: "  << endl;
            aux1 = tr("Writing ADIF file...\n QSO: ")  + QString::number(currentQso) + "/" + QString::number(numberOfQsos);
            progress.setLabelText(aux1);
            progress.setValue(currentQso);
        }
        //qDebug() << "FileManager::adifLogExportToFile -  after showing progress (current%Step): " << QString::number(currentQso%step) << endl;

        if ( progress.wasCanceled() )
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - User cancelled"));
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

    } //Closes the While

      //qDebug() << "FileManager::adifLogExportToFile -  after the While" << endl;
    progress.setValue(numberOfQsos);

    if (noMoreQso)
    {
    //TODO: Remove the file (_fileName)
          //qDebug() << "FileManager::adifLogExportToFile -  noMoreQSO = true" << endl;
        file.remove();
        return false;
    }
    else
    {
          //qDebug() << "FileManager::adifLogExportToFile -  noMoreQSO = false" << endl;
        return writeBackupDate();
    }    
}

bool FileManager::printQs(const QStringList &_line)
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
    //qDebug() << "FileManager::adifLogExportMarked:" << _fileName << endl;
    return adifLogExportToFile(_fileName, 0, true, false, false);
}

QList<int> FileManager::adifLoTWReadLog2(const QString& fileName, const int logN)
{
  //qDebug() << "FileManager::adifLoTWReadLog2" << fileName << endl;
   //QSO qso(Q_FUNC_INFO);
   QString stationCallSign;
   stationCallSign.clear();
   bool addNewQSOs = false;
   bool askedToAddNewQSOs = false;
   QList<int> _qsos;
   _qsos.clear();
   if (!dataProxy->doesThisLogExist(logN))
   {
       //qDebug() << "FileManager::adifLoTWReadLog2: ERROR: The log does not exist: " << QString::number(logN) << endl;
       return _qsos;
   }
   //qDebug() << "FileManager::adifLoTWReadLog2 - 10"  << endl;
   //QString fn = "blotw.adi";
   QFile file( fileName );
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       //qDebug() << "FileManager::adifLoTWReadLog File not found" << fn << endl;
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Warning);
       msgBox.setWindowTitle(tr("KLog - File not opened"));
       QString aux = QString(tr("It was not possible to open the file %1 for reading.") ).arg(fileName);
       msgBox.setText(tr("KLog was not able to read the LoTW file"));
       msgBox.setInformativeText(aux);
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.exec();
       return _qsos;
   }
   //qDebug() << "FileManager::adifLoTWReadLog2 - 20"  << endl;
    int numberOfQsos = 0;
    QString line = QString();
    bool hasEOH = false;
    //qDebug() << "FileManager::adifLoTWReadLog2 - 30"  << endl;
    qint64 pos = file.pos();
    //while ( !file.atEnd() && !hasEOH )
    while ( !file.atEnd() )
    {
        line = (file.readLine()).toUpper();
        if (line.contains("OWNCALL:"))
        {
            stationCallSign = (line.section(": ", 1, 1)).simplified();
            if (!util->isValidCall(stationCallSign))
            {
                stationCallSign = QString();
            }
        }
        //qDebug() << "FileManager::adifLoTWReadLog2 - line-1: " << line  << endl;
        numberOfQsos = numberOfQsos + line.count("EOR>");
        if ((line.count("<EOH>")>0) && (!hasEOH))
        {
            //qDebug() << "FileManager::adifLoTWReadLog2 - line-1: EOH FOUND!"   << endl;
            hasEOH = true;
            pos = file.pos();
        }
    }
    file.seek(pos);
    int step = util->getProgresStepForDialog(numberOfQsos);
    QProgressDialog progress(tr("Processing LoTW ADIF file..."), tr("Abort processing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.setWindowTitle(tr("LoTW reading"));
    progress.setAutoClose(true);

   //qDebug() << "FileManager::adifLoTWReadLog2 - After header while"  << endl;
    noMoreQso = false;
    QStringList fields;
    QSO qso;
    qso.clear();
    int i = 0;
    while (!file.atEnd() && !noMoreQso)
    {
        fields.clear();
        line = (file.readLine()).toUpper();
        if (line.contains("<APP_LOTW_EOF>"))
        {
            noMoreQso = true;
        }
        fields << line.split("<", QString::SkipEmptyParts);

        foreach(QString a, fields)
        {
            //qDebug() << "FileManager::adifLoTWReadLog2 - Fields: " << a  << endl;

            QString fullField = "<" + a.trimmed();
            if (fullField.contains("<EOR>"))
            {
                if (qso.isValid())
                {
                   //qDebug() << "FileManager::adifLoTWReadLog2 VALID QSO: " << endl;
                    qso.setLogId(logN);
                    if (util->isValidCall(stationCallSign))
                    {
                        QString aux = QString("<STATION_CALLSIGN:%1>%2").arg(QString::number(stationCallSign.length())).arg(stationCallSign);
                        qso.setData(aux);
                    }

                    QList<int> dupeQsos;
                    dupeQsos.clear();
                    dupeQsos << dataProxy->isThisQSODuplicated(Q_FUNC_INFO, qso.getCall(), qso.getDateTimeOn(), dataProxy->getIdFromBandName(qso.getBand()), dataProxy->getIdFromModeName(qso.getMode()), duplicatedQSOSlotInSecs);

                    if ((dupeQsos.length()<1) && (!askedToAddNewQSOs) )
                    {
                        askedToAddNewQSOs = true;
                        QMessageBox msgBox;
                        msgBox.setWindowTitle(tr("KLog - Add new QSOs?"));
                        QString aux = QString(tr("Do you want to add non existing QSOs to your local log?"));
                        msgBox.setText(aux);
                        msgBox.setDetailedText(tr("There are some QSOs in the LoTW log that are not in your local log."));
                        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                        msgBox.setDefaultButton(QMessageBox::No);
                        int ret = msgBox.exec();
                        switch (ret)
                        {
                            case QMessageBox::Yes:
                            addNewQSOs = true;
                            break;
                            case QMessageBox::No:
                            addNewQSOs = false;
                            break;
                            default:
                            // should never be reached
                            break;
                        }
                    }

                    if ((dupeQsos.length()<1) && (addNewQSOs))
                    {
                        //qDebug() << "FileManager::adifLoTWReadLog2 -  New QSO ... adding ..."   << endl;
                        int lastId = dataProxy->addQSO(qso);
                        if (lastId>0)
                        {
                            _qsos.append(lastId);
                            //qDebug() << "FileManager::adifLoTWReadLog2 -  New QSO ... added ..."   << endl;
                        }
                        else
                        {
                            //qDebug() << "FileManager::adifLoTWReadLog2 -  New QSO ... adding ... FAILED TO ADD"   << endl;
                        }
                    }
                    else
                    {
                        //qDebug() << "FileManager::adifLoTWReadLog2 -  EXisting QSO or not adding, Updating LoTW QSL status to: " <<  qso.getLoTWQSL_RCVD()  << endl;
                        if (dataProxy->setLoTWQSLRec (dupeQsos.at(0), qso.getLoTWQSL_RCVD(), qso.getLoTWQSLRDate()))
                        {
                            _qsos.append(dupeQsos.at(0));
                            //qDebug() << "FileManager::adifLoTWReadLog2: Modified QSO: " << QString::number(dupeQsos.at(0)) << endl;
                        }
                    }

                    i++;
                    qso.clear();
                }
                else
                {
                   //qDebug() << "FileManager::adifLoTWReadLog2 NOT VALID QSO: " << endl;
                }
            }
            else
            {
                qso.setData(fullField);
            }
        }

        if (( (i % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << "FileManager::adifLoTWReadLog2: ********************************   UPDATING THE MESSAGE! " << QString::number(i)  << endl;
            QString aux = tr("Processing LoTW ADIF file......\n QSO: %1 / %2 ").arg(i).arg(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);
            //qDebug() << "FileManager::adifLoTWReadLog2: ********************************   UPDATING THE MESSAGE: " << aux  << endl;
        }

        if ( progress.wasCanceled() )
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - User cancelled"));
            QString aux = QString(tr("You have canceled the LoTW processing. The process will be stopped and your log may not be completely updated.") + "\n" + tr("Do you still want to cancel?"));
            msgBox.setText(aux);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            switch (ret)
            {
                case QMessageBox::Yes:
            // Yes was clicked
                noMoreQso = true;
                _qsos.clear();
                progress.setValue(numberOfQsos);
                return _qsos;
                //return qsos;
                case QMessageBox::No:
                // No Save was clicked
                break;
                default:
                // should never be reached
                break;
            }
        }

    }

    // Start reading the file

  //qDebug() << "FileManager::adifLoTWReadLog2 - END"  << endl;
   return _qsos;
}

QList<int> FileManager::adifLoTWReadLog(const QString& tfileName, const int logN)
{
   //qDebug() << "FileManager::adifLoTWReadLog: " << tfileName << endl;
    QString fileName = tfileName;
    QList<int> readed;
    QTime time1 = QTime::currentTime();

    readed.clear();
    if (!dataProxy->doesThisLogExist(logN))
    {
       //qDebug() << "FileManager::adifLoTWReadLog: ERROR: The log does not exist: " << QString::number(logN) << endl;
        return readed;
    }
    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       //qDebug() << "FileManager::adifLoTWReadLog File not found" << fileName << endl;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - File not opened"));
        QString aux = QString(tr("It was not possible to open the file %1 for reading.") ).arg(fileName);
        msgBox.setText(tr("KLog was not able to read the LoTW file"));
        msgBox.setInformativeText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return readed;
    }

    //qso->clear();

    QStringList fields, qsToPass;
    QStringList modifiedQSOList;    // This is to emit it so after reading the full file, it is possible to show a qtable showing the QSOs that have been modified.
    modifiedQSOList.clear();
    bool hasEOH = false;
    int numberOfQsos = 0;
    int i = 0;
    //int numberOfQsosLoWTHeader = 0;
    QString line = QString();
    QString auxString = QString();
    QString aux = QString();
    qint64 pos; //Position in the file
    bool inHeader = true;
    pos = file.pos();
    fields.clear();
    qsToPass.clear();
    QDateTime _dateTime;
    bool alwaysAdd = false;
    bool alwaysIgnore = false;
    QString stationCallsign = QString();

    //int step = 1;

    while ( !file.atEnd()   )
    {
        line = (file.readLine()).toUpper();
        numberOfQsos = numberOfQsos + line.count("EOR>");
        if ((line.count("<EOH>")>0) && (!hasEOH))
        {
            hasEOH = true;
        }
    }


    int step = util->getProgresStepForDialog(numberOfQsos);

    //<APP_LOTW_NUMREC:3>847

   //qDebug() << "FileManager::adifLoTWReadLog QSOs found: " << QString::number(numberOfQsos) << endl;
   //qDebug() << "FileManager::adifLoTWReadLog STEP: " << QString::number(step) << endl;

    QProgressDialog progress(tr("Reading LoTW file..."), tr("Abort reading"), 0, numberOfQsos, this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setVisible(true);
    progress.setValue(0);
    progress.setMaximum(numberOfQsos);

    //step = util->getProgresStepForDialog(numberOfQsos);

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
            if (line.contains("OWNCALL:"))
            {
                stationCallsign = line.section(": ", 1, 1);
                if (!util->isValidCall(stationCallsign))
                {
                    stationCallsign = QString();
                }
            }
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
    QDate _tdate;
    noMoreQso = false;
    time1.start();
    while (!noMoreQso )
    {
        if (!file.atEnd())
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            if (line.contains("<APP_LOTW_EOF>"))
            {
                noMoreQso = true;
            }
            //line.append(file.readLine().toUpper()); // TODO: Check if we should remove extra spaces,tabs and so on...
           //qDebug() << "FileManager::adifLoTWReadLog-line:" << line << endl;
            fields << line.split("<", QString::SkipEmptyParts);


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
               //qDebug() << "FileManager::adifLoTWReadLog: START of QSO adding"<< endl;
                //dataProxy->isThisQSODuplicated()

                //int getDuplicatedQSOId(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode);
                QString str, _call, _date, _time, _band, _mode;
                double _freq = 0.0;
                bool haveBand = false;
                QDate _qslrdate = QDate::currentDate();
                QString _satName = QString();

                bool qsl_rcvd = false;
                QString field, data;
                QStringList clearAdif;
                bool validQSO = false;
                int modifiedQSO = -1;
                foreach (str, fields)
                {
                    //field = readAdifField("<"+str).at(0);
                    clearAdif.clear();
                    clearAdif << readAdifField("<"+str);
                  //qDebug() << "FileManager::adifLoTWReadLog: clearAdif length: " << QString::number(clearAdif.length()) << endl;
                    if (clearAdif.length()==2)
                    {
                        validQSO = true;
                        field = clearAdif.at(0);
                        data = clearAdif.at(1);

                      //qDebug() << "FileManager::adifLoTWReadLog: field: " << field << endl;
                      //qDebug() << "FileManager::adifLoTWReadLog: data: " << data << endl;
                        if (field == "CALL")
                        {
                            if (util->isValidCall(data))
                            {
                                _call = data;

                            }
                            else
                            {
                                _call = util->getAValidCall(data);

                            }
                        }
                        else if (field == "QSO_DATE")
                        {
                           //qDebug() << "FileManager::adifLoTWReadLog: field: " << field << endl;
                           //qDebug() << "FileManager::adifLoTWReadLog: data: " << data << endl;

                             _tdate = util->getDateFromADIFDateString(data);
                             if (_tdate.isValid())
                             {
                                 _dateTime.setDate(_tdate);

                             }
                        }
                        else if (field == "TIME_ON")
                        {
                            //qDebug() << "FileManager::adifLoTWReadLog: field: " << field << endl;
                            //qDebug() << "FileManager::adifLoTWReadLog: data: " << data << endl;
                            _time = data;

                            if (util->getTimeFromADIFTimeString(data).isValid())
                            {
                                _dateTime.setTime(util->getTimeFromADIFTimeString(data));

                            }
                        }
                        else if (field == "BAND")
                        {
                            _band  = data;

                            haveBand = true;
                        }
                        else if (field == "MODE")
                        {
                            _mode = data;

                        }
                        else if (field == "FREQ") // In MHz with 5 decimal places
                        {        
                            if (haveBand)
                            {
                                int bandi = dataProxy->getIdFromBandName(_band);
                                if (dataProxy->getBandIdFromFreq(data.toDouble()) == bandi)
                                {
                                    _freq = data.toDouble();

                                }
                                else
                                {
                                    _freq = dataProxy->getLowLimitBandFromBandId(bandi);

                                    // IF band is defined but not the same than freq, Band wins
                                }
                            }
                            else
                            {
                                _freq = data.toDouble();

                                int bandi = dataProxy->getBandIdFromFreq(data.toDouble());
                                if (bandi>=0)
                                {
                                    _band = dataProxy->getBandNameFromFreq(_freq);
                                    haveBand = true;

                                }
                             }


                        }
                        else if (field == "APP_LOTW_RXQSO")
                        {
                            //_qslsdate = data;
                        }
                        else if (field == "QSL_RCVD")
                        {                           
                            if (data == "Y")
                            {
                                qsl_rcvd = true;
                            }
                        }
                        else if (field == "QSLRDATE")
                        {
                            if ((util->getDateFromADIFDateString(data)).isValid())
                            {
                                _qslrdate = util->getDateFromADIFDateString(data);
                            }

                        }
                        else if (field == "SAT_NAME")
                        {
                            if (dataProxy->getSatelliteName(data).length()>0)
                            {
                                _satName = data;
                            }
                        }
                        else
                        {
                            //Unknown ADIF received.
                        }
                    }
                    else
                    {
                       //qDebug() << "FileManager::adifLoTWReadLog: NOT VALID ADIF RECEIVED: " << "<" + str << endl;
                    }

                }
               //qDebug() << "FileManager::adifLoTWReadLog: If QSO is valid, we will call the addQSOToList" << "<" + str << endl;
              //qDebug() << "FileManager::adifLoTWReadLog: New Add String query: " << endl;
               //dataProxy->addQSO(&qso);
                if (validQSO)
                {
                    modifiedQSO = -2; //
                    if (qsl_rcvd)
                    {
                         modifiedQSO = dataProxy->lotwUpdateQSLReception (_call, _dateTime, _band, _mode, _qslrdate);
                    }
                   //qDebug() << "FileManager::adifLoTWReadLog: QSO Modified:  " << QString::number(modifiedQSO) << endl;
                    if (modifiedQSO>0)
                    {
                      //qDebug() << "FileManager::adifLoTWReadLog: QSO Modified:  " << _call << endl;
                       readed.append(modifiedQSO);

                       modifiedQSOList << _call << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << _band << _mode;// << util->getDateSQLiteStringFromDate(_qslrdate);
                       emit addQSOToList(modifiedQSOList);
                       modifiedQSOList.clear();
                    }
                    else if ((modifiedQSO == -1 ) && (!qsl_rcvd))
                    {
                        bool ignoreQSO = false;
                      //qDebug() << "FileManager::adifLoTWReadLog: QSO NOT found but confirmed by LoTW - Is it an error or should I add it to the log? " << _call << endl;
                        if (alwaysAdd)
                        {
                           //qDebug() << "FileManager::adifLoTWReadLog: ADD THE QSO AUTOMATICALLY!!!"  << endl;
                            modifiedQSO = dataProxy->addQSOFromLoTW(_call, _dateTime, _mode, _band, _freq, _qslrdate, stationCallsign, logN);
                            if (modifiedQSO>0)
                            {
                                readed.append(modifiedQSO);
                              //qDebug() << "FileManager::adifLoTWReadLog QSO ADDED readed: " << QString::number(readed.length()) << endl;
                                modifiedQSOList << _call << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << _band << _mode;// << util->getDateSQLiteStringFromDate(_qslrdate);
                                emit addQSOToList(modifiedQSOList);
                                modifiedQSOList.clear();
                              //qDebug() << "FileManager::adifLoTWReadLog: QSO ADDED: " << QString::number(modifiedQSO) << endl;

                            }
                            else
                            {
                              //qDebug() << "FileManager::adifLoTWReadLog: QSO NOT ADDED: " << QString::number(modifiedQSO) << endl;
                            }
                        }
                        else if (alwaysIgnore)
                        {
                          //qDebug() << "FileManager::adifLoTWReadLog: IGNORE THE QSO AUTOMATICALLY!!!"  << endl;
                        }
                        else
                        {

                            if (askUserToAddThisQSOToLog(_call, _dateTime, _mode, _band, _freq, _qslrdate))
                            {
                              //qDebug() << "FileManager::adifLoTWReadLog: ADD THE QSO !!!"  << endl;
                                modifiedQSO = dataProxy->addQSOFromLoTW(_call, _dateTime, _mode, _band, _freq, _qslrdate, stationCallsign, logN);
                                if (modifiedQSO>0)
                                {
                                    readed.append(modifiedQSO);
                                  //qDebug() << "FileManager::adifLoTWReadLog QSO ADDED-2 readed: " << QString::number(readed.length()) << endl;
                                    modifiedQSOList << _call << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << _band << _mode;// << util->getDateSQLiteStringFromDate(_qslrdate);
                                    emit addQSOToList(modifiedQSOList);
                                    modifiedQSOList.clear();
                                  //qDebug() << "FileManager::adifLoTWReadLog: QSO ADDED-2: " << QString::number(modifiedQSO) << endl;
                                }
                                else
                                {
                                  //qDebug() << "FileManager::adifLoTWReadLog: QSO NOT ADDED-2: " << QString::number(modifiedQSO) << endl;
                                }
                            }
                            else
                            {
                              //qDebug() << "FileManager::adifLoTWReadLog: IGNORE THE QSO !!!"  << endl;
                            }
                            if (askUserToUseAlwaysSameAnswer())
                            {
                                if (!ignoreQSO)
                                {
                                    alwaysAdd = true;
                                }
                                else
                                {
                                    alwaysIgnore = true;
                                }

                            }
                            else
                            {
                                alwaysAdd = false;
                                alwaysIgnore = false;
                            }

                        }

                    }
                    else
                    {
                     //qDebug() << "FileManager::adifLoTWReadLog: QSO NOT Modified:  Error: " << QString::number(modifiedQSO) << " - " << _call << endl;
                    }
                }

                _call.clear();
                _date.clear();
                _time.clear();
                _band.clear();
                _mode.clear();
                _qslrdate = QDate::currentDate();

              //qDebug() << "FileManager::adifLoTWReadLog: END of QSO "<< endl;
                fields.clear();
                i++;

            } // END of  if (fields.contains("EOR>")) // We are going to add a QSO to the log... !
            else
            {
                if (file.atEnd())
                {
                    noMoreQso = true;
                }
                 //qDebug() << "FileManager::adifLoTWReadLog: fields DOES NOT contains EOR>" << endl;

            }



        if (( (i % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << "FileManager::adifLoTWReadLog: MOD 0 - i = " << QString::number(i)  << endl;
            aux = tr("Importing LoTW ADIF file...") + "\n" + tr(" QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);
            //aux = QString(tr("Importing LoTW ADIF file...\n QSO: %1/%2\nImporting speed: %3 QSOs/sec")).arg(i).arg(numberOfQsos).arg(step / (time1.elapsed()/1000));

            time1.restart();

            progress.setLabelText(aux);
            progress.setValue(i);
        }

        if (i>=numberOfQsos)
        {
            noMoreQso = true;
        }

        if ( progress.wasCanceled() )
        {

            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - User cancelled"));
            aux = QString(tr("You have canceled the file import. The file will be removed and no data will be imported.") + "\n" + tr("Do you still want to cancel?"));
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
        }
    }
    progress.setValue(numberOfQsos);

  //qDebug() << "FileManager::adifLoTWReadLog - END: " << QString::number(readed.length()) << endl;

    return readed;
}

bool FileManager::adifReadLog(const QString& tfileName, const int logN)
{
    //qDebug() << "FileManager::adifReadLog:" << tfileName << endl;
    QTime time1;

    //int n = 0;
    //QSqlDatabase db = QSqlDatabase::database();
    //int maxLogs = dataProxy->getNumberOfManagedLogs(); // To manage several logs
    usePreviousStationCallsignAnswerAlways = false; // This is to ensure that the StationCallsign is used in the process Line function
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
    bool isDupeQSO = false;
    bool askedToAddDupeQSOs = false;
    bool addDupeQSOs = false;
    qint64 pos; //Position in the file
    int i = 0; //Aunxiliar variable
    int numberOfQsos = 0;
    int step = 1;
    int errorCode = -1;
    //int qsosInTransaction = 0;
    bool ignoreErrorCode19 = false;

    QFile file( fileName );

    //bool moreThanOneLog = adifCheckMoreThanOneLog(file);
    int howManyLogs = howManyLogsInFile(file);
    fillHashLog(file);
    //I am creating several logs when importing a file
    //We need to fill the hashLog to process then in processLog


    //bool keepLogsInFile = false;

      //qDebug() << "FileManager::adifReadLog: Logs: " << QString::number(howManyLogs) << endl;

    if (howManyLogs>1)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - Log selection"));
        aux = QString(tr("There is more than one log in this logfile.") + "\n" + tr("All logs will be imported into the current log.") + "\n" + tr("Do you want to continue?"));
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Yes:
              // Yes was clicked
                   //qDebug() << "FileManager::adifReadLog: clicked YES" << endl;
                //keepLogsInFile = true;
              break;
          case QMessageBox::No:
                // No Save was clicked
                   //qDebug() << "FileManager::adifReadLog: clicked NO" << endl;
                 //keepLogsInFile = false;
                 return false;
          default:
                // should never be reached
                //keepLogsInFile = false;
                return false;
                   //qDebug() << "FileManager::adifReadLog: default" << endl;
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

    preparedQuery.prepare( "INSERT INTO log (call, qso_date, bandid, modeid, srx, stx, srx_string, stx_string, qso_date_off, band_rx, rst_sent, rst_rcvd, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, eq_call, email, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, my_gridsquare, iota, iota_island_id, my_iota, my_iota_island_id, k_index, lat, lon, my_lat, my_lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, clublog_qso_upload_date, clublog_qso_upload_status, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_name, name, operator, station_callsign, owner_callsign, my_rig, my_sig, my_sig_info, my_state, state, my_street, notes, nr_bursts, nr_pings, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, tx_pwr, sat_mode, sat_name, sfi, sig, swl, ten_ten, web, points, multiplier, lognumber) VALUES (:call, :qso_date, :bandid, :modeid, :srx, :stx, :srx_string, :stx_string, :qso_date_off, :band_rx, :rst_sent, :rst_rcvd, :cqz, :ituz, :dxcc, :address, :age, :cnty, :comment, :a_index, :ant_az, :ant_el, :ant_path, :arrl_sect, :checkcontest, :class, :contacted_op, :contest_id, :country, :credit_submitted, :credit_granted, :distance, :eq_call, :email, :eqsl_qslrdate, :eqsl_qslsdate, :eqsl_qsl_rcvd, :eqsl_qsl_sent, :force_init, :freq, :freq_rx, :gridsquare, :my_gridsquare, :iota, :iota_island_id, :my_iota, :my_iota_island_id, :k_index, :lat, :lon, :my_lat, :my_lon, :lotw_qslrdate, :lotw_qslsdate, :lotw_qsl_rcvd, :lotw_qsl_sent, :clublog_qso_upload_date, :clublog_qso_upload_status, :max_bursts, :ms_shower, :my_city, :my_cnty, :my_country, :my_cq_zone, :my_name, :name, :operator, :station_callsign, :owner_callsign, :my_rig, :my_sig, :my_sig_info, :my_state, :state, :my_street, :notes, :nr_bursts, :nr_pings, :pfx, :precedence, :prop_mode, :public_key, :qslmsg, :qslrdate, :qslsdate, :qsl_rcvd, :qsl_sent, :qsl_rcvd_via, :qsl_sent_via, :qsl_via, :qso_complete, :qso_random, :qth, :rx_pwr, :tx_pwr, :sat_mode, :sat_name, :sfi, :sig, :swl, :ten_ten, :web, :points, :multiplier, :lognumber)" );
/*
    if (db.transaction())
    {
           //qDebug() << "FileManager::adifReadLog: Transaction Opened"  << endl;
    }
    else
    {
           //qDebug() << "FileManager::adifReadLog: Transaction NOT Opened"  << endl;
    }
*/
    //file.seek(pos);
    fields.clear();
   // while ( (!file.atEnd() ) && (!noMoreQso) && (sqlOK))
    time1.start();
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
            QString fieldToCheck;
            fieldToCheck = "<" + aux;
            //QStringList _TEST;
            //_TEST.clear();
            //_TEST << util->getValidADIFFieldAndData('<'+fields.at(0));
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
            preparedQuery.bindValue( ":lognumber", logN);

            while ( (!EOR) && (!fields.isEmpty()) )
            {

                   //qDebug() << "FileManager::adifReadLog-W-2" << endl;


                fieldToAnalyze = (fields.takeFirst()).trimmed();

                if ( fieldToAnalyze.contains("EOR>") )
                {
                        //qDebug() << "FileManager::adifReadLog-W-2.1" << endl;
                    currentQSOfields << fieldToAnalyze;
                    isDupeQSO = processQsoReadingADIF(currentQSOfields, logN);
                    if (isDupeQSO && (!askedToAddDupeQSOs))
                    {
                        askedToAddDupeQSOs = true;
                        QMessageBox msgBox;
                        msgBox.setWindowTitle(tr("KLog - Add new QSOs?"));
                        QString aux = QString(tr("Do you want to add dupe QSOs to your local log?"));
                        msgBox.setText(aux);
                        msgBox.setDetailedText(tr("There are some QSOs in this logfile that may be dupes as they have same call, band & mode and a very close date."));
                        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                        msgBox.setDefaultButton(QMessageBox::No);
                        int ret = msgBox.exec();
                        switch (ret)
                        {
                            case QMessageBox::Yes:
                            addDupeQSOs = true;
                            break;
                            case QMessageBox::No:
                            addDupeQSOs = false;
                            break;
                            default:
                            // should never be reached
                            break;
                        }



                           //qDebug() << "FileManager::adifReadLog: isDupeQSO = true"  << endl;

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
            if ((!isDupeQSO) || (addDupeQSOs))
            {
                sqlOK = preparedQuery.exec();
            }
            else
            {
                //qDebug() << "FileManager::adifReadLog: DUPE QSO, not adding it" << endl;
            }

            //qDebug() << "FileManager::adifReadLog: executedQuery1: " << preparedQuery.executedQuery()  << endl;
            //qDebug() << "FileManager::adifReadLog: executedQuery2: " << preparedQuery.executedQuery()  << endl;
            //qDebug() << "FileManager::adifReadLog: LastQuery2: " << preparedQuery.lastQuery()  << endl;


            if (( (i % step ) == 0) )
            { // To update the speed I will only show the progress once each X QSOs
                  //qDebug() << "FileManager::adifReadLog: MOD 0 - i = " << QString::number(i)  << endl;
                //aux = QString(tr("Importing LoTW ADIF file...\n QSO: %1/%2\nImporting speed: 0 QSOs/sec")).arg(i).arg(numberOfQsos);
                /*
                 *qDebug() << "FileManager::adifReadLog: " << QString::number(step) << endl;
                qDebug() << "FileManager::adifReadLog: " << QString::number(time1.elapsed()) << endl;
                qDebug() << "FileManager::adifReadLog: " << QString::number(time1.elapsed()/1000) << endl;
                qDebug() << "FileManager::adifReadLog: " << QString::number(step / (time1.elapsed()/1000)) << endl;

                if (time1.elapsed()/1000 != 0)
                {
                    aux = QString(tr("Importing LoTW ADIF file...\n QSO: %1/%2\nImporting speed: %3 QSOs/sec")).arg(i).arg(numberOfQsos).arg(step / (time1.elapsed()/1000));
                }
                else
                {
                    aux = QString(tr("Importing LoTW ADIF file...\n QSO: %1/%2\nImporting speed: 0 QSOs/sec")).arg(i).arg(numberOfQsos);
                }


                time1.restart();
                */
                aux = tr("Importing ADIF file...") + "\n" + tr(" QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);

               progress.setLabelText(aux);
               progress.setValue(i);

            }
            else
            {
                  //qDebug() << "FileManager::adifReadLog: Mod: "<< QString::number(i) << " mod " << QString::number(step) << " = " << QString::number(i % step) << endl;
            }

            if (sqlOK)
            {
                //qDebug() << "FileManager::adifReadLog: (1) in While sqlOK (QSO added) = TRUE"  << endl;
            }
            else
            {
                errorCode = preparedQuery.lastError().number();
                //qDebug() << "FileManager::adifReadLog: QSO DUPE" << endl;

                //qDebug() << "FileManager::adifReadLog: (1) LastQuery: " << preparedQuery.lastQuery()  << endl;
                //qDebug() << "FileManager::adifReadLog: (1) LastError-data: " << preparedQuery.lastError().databaseText()  << endl;
                //qDebug() << "FileManager::adifReadLog: (1) LastError-driver: " << preparedQuery.lastError().driverText()  << endl;
                //qDebug() << "FileManager::adifReadLog: (1) LastError-n: " << QString::number(preparedQuery.lastError().number() ) << endl;
                if ((errorCode == 19) && (!ignoreErrorCode19))
                { // There are some repeated QSO
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("KLog - Duplicated QSOs"));
                    aux = tr("It seems that there are some duplicated QSOs in the ADIF file you are importing. Do you want to continue? (Duped QSOs will not be imported)");
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

                    emit queryError( Q_FUNC_INFO, preparedQuery.lastError().databaseText(), preparedQuery.lastError().number(), preparedQuery.lastQuery());
                    noMoreQso = true;
                    /*
                    QMessageBox msgBox;
                    aux = tr("An unexpected error ocurred while importing. Please send this code to the developer for analysis: ");
                    msgBox.setText(aux + "FM-1 #" + QString::number(errorCode) );
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Ok:
                          // Yes was clicked
                            //sqlOK = false;
                               //qDebug() << "FileManager::adifReadLog: (2) I have just set sqlOK=False (1)" << endl;
                            noMoreQso = true;
                            return;
                          break;
                      default:
                          // should never be reached
                            //sqlOK = false;
                               //qDebug() << "FileManager::adifReadLog: (2) I have just set sqlOK=False (2)" << endl;
                          break;
                    }
                    */
                }
            }

            if ( progress.wasCanceled() )
            {

                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("KLog - User cancelled"));
                aux = QString(tr("You have canceled the file import. The file will be removed and no data will be imported.") + "\n" + tr("Do you still want to cancel?"));
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

            currentQSOfields.clear();
            queryPreparation(logN); // to clear Values
            //progress.setValue(i);
            i++;
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
        /*
        if (db.commit())
        {
               //qDebug() << "FileManager::adifReadLog: Last commit OK"  << endl;
        }
        else
        {
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

            if (db.rollback())
            {

            }
            else
            {
              //TODO: Check the error if db.rollback returns false
            }

        }
        */

    }
    else
    {
           //qDebug() << "FileManager::adifReadLog: sqlOK = NOK"  << endl;

    }


    progress.setValue(numberOfQsos);



       //qDebug() << "FileManager::adifReadLog END "  << endl;
    return true;

}

bool FileManager::processQsoReadingADIF(const QStringList &_line, const int logNumber) //, const bool _keepLogsInFile)
//bool FileManager::processQsoReadingADIF(const QStringList _line, const int logNumber, const bool _keepLogsInFile, QHash<int, int> &_logs)
{

       //qDebug() << "FileManager::processQsoReadingADIF: log: " << QString::number(logNumber) << endl;
       //qDebug() << "FileManager::processQsoReadingADIF: log: " << _line.at(0) << endl;
       //qDebug() << "FileManager::processQsoReadingADIF: " << _line.join("/") << endl;

    //QHash<int, int> &hashLogs = _logs;

    //QFile &file = _f;

    //bool keepLogsInF;// = _keepLogsInFile; //TODO: Check if needed or remove it completely. This line is just to remove a warning
    int i = -1;
    QDate date, dateT;
    date = QDate();
    dateT = QDate();
    QTime time;
    QDateTime dateTime, dateTimeOFF;
    dateTime = QDateTime();
    QStringList qs = _line;
    QStringList oneField;
    QString field, data;
    QSqlQuery query;

    //confirmed = 0; // 0 means worked, 1 means confirmed

    QString queryString, stringFields, stringData;
    //int lastId;
    //int dxcc = 0;
    //int cqz = 0;
    //int ituz = 0;
    //int i; // Aux value
    QString aux; // Aux string
    QString aux2, aux3;
    QString qrzCall = "";
    QString submode = QString();
    QString defaultRSTTX = util->getDefaultRST(QString());
    QString defaultRSTRX = util->getDefaultRST(QString());

    bool isDupeQSO = false;
    int modei = -1;
    int bandi = -1;
    int bandrxi = -1;
    bool rstRXr = false;
    bool rstTXr = false;

    //KLog does not understand (and will not import) a QSO without these fields
    bool haveCall = false;
    QString wrongCall = QString();
    bool haveBand = false;
    bool bandRXDef = false;
    bool haveMode = false;
    bool haveSubMode = false;
    bool haveTime = false;
    bool haveDate = false;
    bool haveTimeOff = false;
    bool haveDateOff = false;

    bool haveFreqTX = false;
    bool haveFreqRX = false;
    bool hasStationCall = false;
	bool hasLotwQslSent = false;
    bool hasEqslQslSent = false;
    bool hasQrzQslSent = false;
    bool hasClublogQslSent = false;
    QString freqTX = QString();

    //bool ret;
    //int length = 0;
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
               //qDebug() << "FileManager::processQsoReadingADIF: field/data" << field << "/" << data << endl;

                if (i == 2)
                { // DATE:8:D / 20141020
                    //length = (field.section(':', 1, 1)).toInt();
                    field = field.section(':', 0, 0);
                }
                else if (i == 1)
                { // DATE:8 / 20141020
                    //length = (field.section(':', 1, 1)).toInt();
                    field = field.section(':', 0, 0);
                }
                else
                {
                       //qDebug() << "FileManager::checkADIFValidFormat-1 " << endl;
                    //return false;
                }

                //field = oneField.at(0).trimmed();
                //data = oneField.at(1).trimmed();

                //length = field.indexOf(":");
                //field = field.left(length);
                //qDebug() << "FileManager::processQsoReadingADIF (field/data): " << field << "/" << data << endl;

                if (field == "CALL")
                {
                   //qDebug() << "FileManager::processQsoReadingADIF-CALL:" << data << endl;
                    qrzCall = data;                    
                    haveCall = util->isValidCall(qrzCall);
                    if (haveCall)
                    {
                        //qDebug() << "FileManager::processQsoReadingADIF-CALL: Have CALL!!"  << endl;
                        preparedQuery.bindValue( ":call", qrzCall );
                    }
                   //qDebug() << "FileManager::processQsoReadingADIF-CALL-END:" << data << endl;
                }
                else if (field == "QSO_DATE")
                {
                    //qDebug() << "FileManager::processQsoReadingADIF-QSO_DATE:" << data << endl;
                    dateT = util->getDateFromADIFDateString(data);

                    if (dateT.isValid())
                    {
                        dateTime.setDate(dateT);
                        haveDate = true;
                    }
                    else {
                       //qDebug() << "FileManager::processQsoReadingADIF QSO_DATE is NOT VALID: " << data << endl;

                    }
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
                    modei = dataProxy->getSubModeIdFromSubMode(data); // get modeid
                    if (modei>=0)
                    {
                        {
                            if (!haveSubMode)
                            {
                                preparedQuery.bindValue( ":modeid", QString::number(modei) );
                                haveMode = true;
                                haveSubMode = true;
                                submode = dataProxy->getSubModeFromId(modei);
                            }
                        }
                    }
                }
                else if (field == "SUBMODE")
                {                    
                    modei = dataProxy->getSubModeIdFromSubMode(data);
                    if (modei>=0)
                    {                        

                        preparedQuery.bindValue( ":modeid", QString::number(modei) );
                        preparedQuery.bindValue( ":submode", QString::number(modei) );
                        haveSubMode = true;
                        haveMode=true;
                        submode = data;
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
                    time = util->getTimeFromADIFTimeString(data);

                    if (time.isValid())
                    {
                        dateTime.setTime(time);
                        haveTime = true;
                    }

                }
                else if (field == "QSO_DATE_OFF")
                {
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        dateTimeOFF.setDate(dateT);
                        haveDateOff = true;
                    }
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
                }

                else if (field == "TIME_OFF")
                {
                    time = util->getTimeFromADIFTimeString(data);
                    if (time.isValid())
                    {
                        dateTimeOFF.setTime(time);
                        haveTimeOff = true;
                    }
                }

                else if (field == "RST_SENT")
                {
                    preparedQuery.bindValue( ":rst_sent", data );
                   //qDebug() << "FileManager::bprocessQsoReadingADIF-rst_rsent: " << data << endl;
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
                    //cqz = data.toInt();

                }
                else if (field == "ITUZ")
                {
                    preparedQuery.bindValue( ":ituz", data );
                    //ituz = data.toInt();
                }
                else if (field == "DXCC")
                {
                    //dxcc = data.toInt();
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
                else if (field == "DARC_DOK")
                {
                    preparedQuery.bindValue( ":darc_dok", data );
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
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                       preparedQuery.bindValue( ":eqsl_qslrdate", util->getDateSQLiteStringFromDate(dateT) );
                    }
                }
                else if (field == "EQSL_QSLSDATE")
                {
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":eqsl_qslsdate", util->getDateSQLiteStringFromDate(dateT) );
                    }

                }
                else if (field == "EQSL_QSL_RCVD")
                {
                    preparedQuery.bindValue( ":eqsl_qsl_rcvd", data );
                }
                else if (field == "EQSL_QSL_SENT")
                {
                    preparedQuery.bindValue( ":eqsl_qsl_sent", data );
                    hasEqslQslSent = true;
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

                    if (haveBand)
                    {
                        if (dataProxy->getBandIdFromFreq(data.toDouble()) == bandi)
                        //if (db->isThisFreqInBand(db->getBandNameFromNumber(bandi), data))
                        {
                            preparedQuery.bindValue( ":freq", data);
                            haveFreqTX =true;
                            freqTX = data;
                        }
                        else
                        {
                            // IF band is defined but not the same than freq, Band wins
                        }
                    }
                    else
                    {
                        preparedQuery.bindValue( ":freq", data);
                        haveFreqTX =true;
                        freqTX = data;
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
                    if (bandRXDef)
                    {
                        if (dataProxy->getBandIdFromFreq(data.toDouble()) == bandrxi)                        
                        {
                            preparedQuery.bindValue( ":freq_rx", data);
                            haveFreqRX =true;
                        }
                        else
                        {
                            // IF band is defined but not the same than freq, Band wins
                        }
                    }
                    else
                    {                        
                        preparedQuery.bindValue( ":freq_rx", data);
                        haveFreqRX = true;
						i = dataProxy->getBandIdFromFreq(data.toDouble());

                        if (i>=0)
                        {
                            preparedQuery.bindValue( ":band_rx", QString::number(i) );
                            bandRXDef = true;
                               //qDebug() << "FileManager::processQsoReadingADIF-Band: " << data << "/"  << QString::number(i) << endl;
                        }
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
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":hrd_qso_upload_date", util->getDateSQLiteStringFromDate(dateT) );
                    }
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
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":lotw_qslrdate", util->getDateSQLiteStringFromDate(dateT) );
                    }
                }
                else if (field == "LOTW_QSLSDATE")
                {
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":lotw_qslsdate", util->getDateSQLiteStringFromDate(dateT) );
                    }
                }
                else if (field == "LOTW_QSL_RCVD")
                {
                    preparedQuery.bindValue( ":lotw_qsl_rcvd", data );
                }
                else if (field == "LOTW_QSL_SENT")
                {
                    preparedQuery.bindValue( ":lotw_qsl_sent", data );
					hasLotwQslSent = true;
                }
                else if (field == "CLUBLOG_QSO_UPLOAD_DATE")
                {
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":clublog_qso_upload_date", util->getDateSQLiteStringFromDate(dateT) );
						hasClublogQslSent = true;
                    }
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
                    if (util->isValidCall(data))
                    {
                        preparedQuery.bindValue( ":operator", data );
                    }
                }
                else if (field == "STATION_CALLSIGN")
                {
                    if (util->isValidCall(data))
                    {
                        hasStationCall = true;
                        preparedQuery.bindValue( ":station_callsign", data );
                    }
                }
                else if (field == "OWNER_CALLSIGN")
                {
                    if (util->isValidCall(data))
                    {
                        preparedQuery.bindValue( ":owner_callsign", data );
                    }
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
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":qrzcom_qso_upload_date", util->getDateSQLiteStringFromDate(dateT));
						hasQrzQslSent = true;
                    }
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
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":qslrdate", util->getDateSQLiteStringFromDate(dateT));
                    }
                }
                else if (field == "QSLSDATE")
                {
                    dateT = util->getDateFromADIFDateString(data);
                    if (dateT.isValid())
                    {
                        preparedQuery.bindValue( ":qslsdate", util->getDateSQLiteStringFromDate(dateT) );
                    }
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

    if ( haveCall && haveDate && haveTime && haveBand && haveMode)
    {
        QList<int> _dupeQSOs;
        _dupeQSOs.clear();
        _dupeQSOs << dataProxy->isThisQSODuplicated(Q_FUNC_INFO, qrzCall, dateTime, bandi, modei, duplicatedQSOSlotInSecs);
        if (_dupeQSOs.length()>0)
        {
            isDupeQSO = true;
        }
        //QList<int> DataProxy_SQLite::isThisQSODuplicated(const QString &_qrz, const QDateTime &_dateTime, const int _band, const int _mode, const int _secs)
    }

    if ( (haveDate) && (haveTime))
    {       
        preparedQuery.bindValue( ":qso_date", util->getDateTimeSQLiteStringFromDateTime(dateTime));
        //preparedQuery.bindValue( ":time_on", dateTime.toString("hhmmss"));
    }


    if ( (haveDateOff) && (haveTimeOff))
    {
        preparedQuery.bindValue( ":qso_date_off", util->getDateTimeSQLiteStringFromDateTime(dateTimeOFF));
    }


    if ((haveBand) && (!haveFreqTX))
    {
        preparedQuery.bindValue( ":freq",  dataProxy->getFreqFromBandId(bandi));
    }
    if ((bandRXDef) && (!haveFreqRX))
    {
        preparedQuery.bindValue( ":freq_rx",  dataProxy->getFreqFromBandId(bandrxi));
    }
    if ((haveFreqTX) && (!haveFreqRX))
    {
        preparedQuery.bindValue( ":freq_rx",  freqTX);
    } 
	
	if (!haveCall)
    {        
        QString text = util->getAValidCall(qrzCall);
        if (!(util->isValidCall(text)))
        {
            haveCall = true;
            qrzCall = text;
        }
        else
        {
            qrzCall = QString();
            haveCall = false;
        }
    }

    if (!(haveBand  && haveMode && haveTime && haveDate ))
    {
          //qDebug() << "FileManager::processQsoReadingADIF (Don't have all mandatory fields): " << endl;
        aux2 = tr ("This QSO is not including the minimum data to consider a QSO as valid!") + "\n\n\n" + tr("Please edit the ADIF file and make sure that it include at least:") + "\n\nCALL, QSO_DATE, TIME_ON, BAND "+ tr("and") +" MODE.\n\n" + tr("This QSO had:") + "\n";

        if (!haveBand)
        {
            aux2 = aux2 + tr(" - The band missing and the following call: ") + qrzCall + ".\n";
        }

        if (!haveMode)
        {
            aux2 = aux2 + tr(" - The mode missing and the following call: ") + qrzCall + ".\n";
        }
        if (!haveDate)
        {
            aux2 = aux2 + tr(" - The date missing and the following call: ") + qrzCall + ".\n";
        }
        if (!haveTime)
        {
            aux2 = aux2 + tr(" - The time missing and the following call: ") + qrzCall + ".\n";
        }


        aux2 = aux2 + "\n\n" + tr("Do you want to continue with the current file?");
        //qDebug() << "FileManager::processQsoReadingADIF - Missing fields: " << aux2 << endl;

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog: Not all required data found!"));
        msgBox.setText(aux2);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        switch (ret)
        {
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

        //submode = dataProxy->getSubModeFromId(i);
        defaultRSTTX = util->getDefaultRST(submode);
        QMessageBox msgBox;
        aux = tr("Some QSOs of this log, (i.e.: %1) seems to lack RST-TX information.").arg(qrzCall)  + "\n\n" + tr("Click on Yes to add a default %1 for mode %2 to all QSOs with a similar problem.").arg(defaultRSTTX).arg(submode) + "\n\n" + tr("If you select NO, maybe the QSO will not be imported.");
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
        defaultRSTRX = util->getDefaultRST(submode);

        QMessageBox msgBox;
        //aux = tr("Some QSOs of this log, (i.e.: %1) seems to lack RST-RX information.").arg(qrzCall) + "\n\n" + tr("Click on Yes to add a default 59 to all QSO with a similar problem.") + "\n\n" + tr("If you select NO, the QSO may not be imported.");
        aux = tr("Some QSOs of this log, (i.e.: %1) seems to lack RST-RX information.").arg(qrzCall)  + "\n\n" + tr("Click on Yes to add a default %1 for mode %2 to all QSOs with a similar problem.").arg(defaultRSTRX).arg(submode) + "\n\n" + tr("If you select NO, maybe the QSO will not be imported.");
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
        preparedQuery.bindValue( ":rst_sent", defaultRSTTX );

    }

    if ((!rstRXr) && (rstRXDefault))
    {
        preparedQuery.bindValue( ":rst_rcvd", defaultRSTRX );
    }

    if ((!hasStationCall)  )
    {

        if (!usePreviousStationCallsignAnswerAlways)
        {
            QMessageBox msgBox;
            if(getStationCallsignFromUser(qrzCall, dateTime.date()))
            {
                hasStationCall = true;
            }
            else
            {

                msgBox.setWindowTitle(tr("KLog - No Station callsign entered."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("KLog - No Station callsign entered.");
                msgBox.setInformativeText("Not entering a valid Station Callsign may create problems in some ocasions, like when uploading data to LoTW.\n\n Do you want to try again?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::Yes);
                int ret = msgBox.exec();
                if (ret == QMessageBox::Yes)
                {
                    if (!getStationCallsignFromUser(qrzCall, dateTime.date()))
                    {
                        msgBox.setWindowTitle(tr("KLog - No Station callsign entered."));
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText("KLog - No Station callsign entered.");
                        msgBox.setInformativeText("No valid station callsign to add to the QSO.");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        msgBox.exec();

                    }
                    else
                    {
                        hasStationCall = true;
                    }
                }
            }

            msgBox.setWindowTitle(tr("KLog - Apply to all QSOs in this log?"));
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setText("KLog - Do you want to use the same answer for all the QSOs in this log?");
            msgBox.setInformativeText("KLog can use the same answer that you provided to all the QSOs in this logfile when importing it.");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes)
            {
                usePreviousStationCallsignAnswerAlways = true;
            }
            else
            {
                usePreviousStationCallsignAnswerAlways = false;
            }
        }
           //qDebug() << "FileManager::processQsoReadingADIF defaultStationCallsign: " << defaultStationCallsign << endl;

        if ((hasStationCall) || (util->isValidCall(defaultStationCallsign)))
        {
            preparedQuery.bindValue( ":station_callsign", defaultStationCallsign );
        }
		if (!hasLotwQslSent)
		{
			preparedQuery.bindValue( ":lotw_qsl_sent","Q");
		}

		if (!hasEqslQslSent)
		{
			preparedQuery.bindValue( ":eqsl_qsl_sent","Q");
		}

		if (!hasClublogQslSent)
		{
			preparedQuery.bindValue( ":clublog_qso_upload_status","M");
		}

		if (!hasQrzQslSent)
		{
			preparedQuery.bindValue( ":qrzcom_qso_upload_status","M");
		}
	}	

    preparedQuery.bindValue( ":lognumber", QString::number(logNumber));

    return isDupeQSO;

}

bool FileManager::getStationCallsignFromUser(const QString &_qrzDX, const QDate &_dt)
{
    //qDebug() << "FileManager::getStationCallsignFromUser: " << _qrzDX << "/" << util->getDateSQLiteStringFromDate(_dt) << "-" << endl;
    bool ok;
    QString text;
    QString aux;
    QString _date = QString();
    if (_dt.isValid())
    {
        _date = ", on "+  util->getDateSQLiteStringFromDate(_dt) ;
    }

    if (util->isValidCall(_qrzDX))
    {
        aux = tr("KLog has found one QSO without the Station Callsign defined.\n\nEnter the Station Callsign that was used to do this QSO with %1 on %2:").arg(_qrzDX).arg(_date);
        text = QInputDialog::getText(this, tr("KLog - QSO without Station Callsign"),
                                                   aux, QLineEdit::Normal, "", &ok);
    }
    else
    {
        text = QInputDialog::getText(this, tr("KLog - QSO without Station Callsign"),
                                                   tr("KLog has found one QSO without the Station Callsign defined.\n\nEnter the Station Callsign that was used to do this QSO on %1:").arg(_date), QLineEdit::Normal,
                                                   "", &ok);
    }

    if (ok && util->isValidCall(text))
    {
        defaultStationCallsign = text.toUpper();
        return true;
    }
    else
    {
        return false;
    }
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
    //qDebug() << "FileManager::adifReqQSLExport" << _fileName << endl;
    return adifLogExportToFile(_fileName, 0, false, true, false);
}

bool FileManager::modifySetupFile(const QString& _filename, const QString &_field, const QString &_value)
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
    bool modified = false;

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
            modified = true;
        }
        else
        {
            in << line << endl;
        }
    }
    if (!modified)
    {// If the data is not found, we will add it to the end.
        in << _field << "=" << _value << ";" << endl;
    }

    return true;
}

int FileManager::howManyLogsInFile(QFile& _f)
{
      //qDebug() << "FileManager::howManyLogsInFile:" << endl;
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
        return false;
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
    //bool atLeastOneLog = false;

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
            //atLeastOneLog = true;
        }
        else
        {

        }
    }

    file.close();

    return logs;
}

void FileManager::setVersion(const QString &_version)
{
    util->setVersion(_version);
}

QStringList FileManager::readAdifField(const QString &_field)
{
    // This function receives a QString with an ADIF field and returns a QStringList with the following format:
    // ADIF-tag, value
    // If the QString is not an ADIF tag, it returns a clear QStringList.
    // We are expecting the ADIF format:
    // <F:L:T>D
    // <Field:Length:Type>Data

    //qDebug() << "FileManager::readAdifField: " << _field << endl;
    QStringList result;
    result.clear();

    if (_field == "<EOR>")
    {
        //qDebug() << "FileManager::readAdifField: EOR found!!" << endl;
        result << "EOR" << "EOR";
        return result;
    }

    if (!((_field.startsWith("<")) && (_field.contains(":")) && (_field.contains(">"))))
    {
        //qDebug() << "FileManager::readAdifField: NOT (contains : and >): " << _field << endl;
        return QStringList();
    }


    // Now we have the data in the result[1]
    result = _field.split(">", QString::SkipEmptyParts); // Remove the first '<' and split in field & data
    QString data = result.at(1);
    QStringList fieldList;
    fieldList.clear();
    fieldList << (result.at(0)).split(':'); // we may have 1 or 2 depending on the format of the ADIF field.
    int iAux = fieldList.length();
      //qDebug() << "FileManager::readAdifField: iAux: " << QString::number(iAux) << endl;
    int dataLength;
    QString field;
    QString fieldType;
      //qDebug() << "FileManager::readAdifField: analyzing..."  << endl;

    if (iAux == 2)
    {
        field = fieldList.at(0);
        dataLength = (fieldList.at(1)).toInt();
        fieldType = QString();
    }
    else if (iAux == 3)
    {
        field = fieldList.at(0);
        dataLength = (fieldList.at(1)).toInt();
        fieldType = fieldList.at(2);
    }
    else
    { // Not valid ADIF
         //qDebug() << "FileManager::readAdifField: iAux != 1, 2" << endl;
        return QStringList();
    }
    field.remove('<');
    data = data.left(dataLength);
      //qDebug() << "FileManager::readAdifField: field: " << field << endl;
      //qDebug() << "FileManager::readAdifField: dataLength: " << QString::number(dataLength) << endl;
      //qDebug() << "FileManager::readAdifField: data: " << data << endl;
      //qDebug() << "FileManager::readAdifField: fieldType: " << fieldType << endl;

    if (data.length() != dataLength)
    {
        //qDebug() << "FileManager::readAdifField: data.length != dataLength: " << QString::number(data.length()) << "/" << QString::number(dataLength) << endl;
        return QStringList();
    }
    result.clear();
    result << field << data;
    //qDebug() << "FileManager::readAdifField: OK: " << field << "/" << data << endl;
    return result;

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

QDateTime FileManager::getDateTimeOfLastBackup()
{
      //qDebug() << "FileManager::getDateTimeOfLastBackup: " << (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmss")<< endl;
      //qDebug() << "FileManager::getDateTimeOfLastBackup: " << util->getCfgFile() << endl;
    QFile file (util->getCfgFile());
    QString line;
    QStringList fields;
    fields.clear();
    QDateTime _dataTime = QDateTime();

    if (file.open (QIODevice::ReadOnly))
    {
        while ( !file.atEnd()   )
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            if (line.contains("LATESTBACKUP"))
            {
                fields << line.split("=", QString::SkipEmptyParts);
                line = fields.at(1);
                line.truncate(15);
                return _dataTime.fromString(line, "yyyyMMdd-hhmmss");
            }
        }
    }
    return QDateTime();
}

bool FileManager::writeBackupDate()
{
      //qDebug() << "FileManager::writeBackupDate: current: " << (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmss") << endl;
      //qDebug() << "FileManager::writeBackupDate: current: " << (getDateTimeOfLastBackup()).toString("yyyyMMdd-hhmmss") << endl;

    QFile file (util->getCfgFile());
    QString line, lineTemp;
    //QStringList fields;
    //fields.clear();
    QDateTime _dataTime = QDateTime();

    QStringList completeFile;
    completeFile.clear();

    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&file);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if ( !(line.toUpper()).contains("LATESTBACKUP")  )
            {
                s.append(line + "\n");
            }
        }
        s.append("LatestBackup=" + (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmss") + ";\n" );
        file.resize(0);
        t << s;
        file.close();
    }

    return true;
}



void FileManager::setStationCallSign(const QString& _st)
{
      //qDebug() << "FileManager::setStationCallSign: " << _st << endl;
    if (util->isValidCall(_st))
    {        
          //qDebug() << "FileManager::setStationCallSign: True" << endl;
        defaultStationCallsign = _st;
          //qDebug() << "FileManager::setStationCallSign: " << defaultStationCallsign << endl;
    }
    else
    {
          //qDebug() << "FileManager::setStationCallSign: FALSE" << endl;
    }

   //qDebug() << "FileManager::setStationCallSign: -" << defaultStationCallsign << "-END" << endl;
}


bool FileManager::askUserToUseAlwaysSameAnswer()
{
    //qDebug() << "FileManager::askUserToUseAlwaysSameAnswer: "  << endl;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - Don't ask again"));

    msgBox.setText(tr("Do you want to reuse your answer?"));
    msgBox.setInformativeText(tr("KLog will use automatically your previous answer for any other similar ocurrence, if any, without asking you again."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
          // Yes was clicked
           return true;
    case QMessageBox::No:
            // No Save was clicked
        return false;
    default:
            // should never be reached
        return false;
    }

    //qDebug() << "FileManager::askUserToUseAlwaysSameAnswer: - END" << endl;
}

bool FileManager::askUserToAddThisQSOToLog(const QString &_call, const QDateTime _datetime, const QString &_mode, const QString &_band, const double _freq, const QDate _qslrdate)
{
    //qDebug() << "FileManager::askUserToAddThisQSOToLog: " << _call << endl;
    QString qsoData = QString(tr("<ul><li>Date/Time:</i> %1</li><li>Callsign: %2</li><li>Band: %3</li><li>Mode: %4</li></ul>")).arg(util->getDateTimeSQLiteStringFromDateTime(_datetime)).arg(_call).arg(_band).arg(_mode);
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - QSO not found"));

    msgBox.setText(tr("Do you want to add this QSO to the log?:\n\n") + qsoData);
    msgBox.setInformativeText(tr("We have found a QSO coming from LoTW that is not in your local log.\n\nDo you want KLog to add this QSO to the log?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
          // Yes was clicked
           return true;
    case QMessageBox::No:
            // No Save was clicked
        return false;
    default:
            // should never be reached
        return false;
    }

    //qDebug() << "FileManager::askUserToAddThisQSOToLog: - END" << endl;

}

bool FileManager::showInvalidCallMessage(const QString &_call){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - Invalid call detected"));

    QString aux = _call;

    if (aux.length()<1)
    {
        aux = QString(tr("An empty callsign has been detected. Do you want to export this QSO anyway (click on Yes) or remove the field from the exported log file?"));

    }
    else
    {
        aux = QString(tr("An invalid callsign has been detected %1. Do you want to export this callsign anyway (click on Yes) or remove the call from the exported log file?").arg(aux));
    }

    msgBox.setText(aux);
    msgBox.setInformativeText(tr("Exporting wrong calls may create problems in the applications you are potentially importing this logfile to. It may, however, be a good callsign that is wrongly identified by KLog as not valid. You can, however, edit the ADIF file once the export process is finished."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
          // Yes was clicked
           return true;
    case QMessageBox::No:
            // No Save was clicked
        return false;
    default:
            // should never be reached
        return false;
    }
}

void FileManager::writeADIFHeader(QTextStream &out, const ExportMode _em, const int _numberOfQsos)
{
    if (_em == ModeEQSL)
    {
        out << "ADIF v3.1.0 Export from KLog\n<PROGRAMID:4>KLog " << endl;
    }
    else
    {
        out << "ADIF v3.1.0 Export from KLog\nhttps://www.klog.xyz/klog\n<PROGRAMVERSION:" << QString::number(klogVersion.length()) << ">" << klogVersion << "\n<PROGRAMID:4>KLOG " << endl;
        out << "<APP_KLOG_QSOS:" << QString::number((QString::number(_numberOfQsos)).length()) << ">" << QString::number(_numberOfQsos) << endl;
        out << "<APP_KLOG_LOG_DATE_EXPORT:" << QString::number((QDateTime::currentDateTime().toString("yyyyMMdd-hhmm")).length()) << ">" << QDateTime::currentDateTime().toString("yyyyMMdd-hhmm") << endl;
    }
    out << "<EOH>" << endl;
}

void FileManager::writeQuery(QSqlQuery query, QTextStream &out, const ExportMode _em, const bool _justMarked, const bool _onlyRequested, const int _logN )
{
   //qDebug() << "FileManager::writeQuery: " <<  query.lastQuery() << endl;
    int nameCol;
    QString aux;
    bool propsat = false;    // Reset the QSO in case it is a Satellite QSO
    QSqlRecord rec = query.record();

    if (_justMarked)
    {
        nameCol = rec.indexOf("marked");
        aux = (query.value(nameCol)).toString();
        if (aux != "X")
        {
            return;
        }
    }

    if (_onlyRequested)
    {
        nameCol = rec.indexOf("qsl_sent");
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( aux !="R" )
        {
            return;
        }
    }

    nameCol = rec.indexOf("call");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
       //qDebug() << "FileManager::writeQuery: " << QString::number(nameCol) << "/" << aux << endl;
        if (util->isValidCall(aux))
        {
            out << "<CALL:" << QString::number(aux.length()) << ">" << aux << " ";
        }
        else
        {
            if (showInvalidCallMessage(aux))
            {
                 out << "<CALL:" << QString::number(aux.length()) << ">" << aux << " ";
            }
        }
    }
    nameCol = rec.indexOf("qso_date");
    QDateTime tDateTime;
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        tDateTime = util->getDateTimeFromSQLiteString(aux);
        if (tDateTime.isValid())
        {
            aux = util->getADIFDateFromQDateTime(tDateTime);
            out << "<QSO_DATE:" << aux.length() << ">" << aux  << " ";
            aux = util->getADIFTimeFromQDateTime(tDateTime);
            if (_em == ModeEQSL)
            {
                aux.chop(2);
                out << "<TIME_ON:" << aux.length() << ">" << aux  << " ";
            }
            else
            {
                out << "<TIME_ON:" << aux.length() << ">" << aux  << " ";
            }
        }
    }

    nameCol = rec.indexOf("bandid");
    QString bandst = QString();
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        //qDebug() << "FileManager::writeQuery-Band-1: "  << aux << endl;
        aux = util->checkAndFixASCIIinADIF(aux);
        //qDebug() << "FileManager::writeQuery-Band-2: "  << aux << endl;
        aux = dataProxy->getNameFromBandId(aux.toInt());

        if (dataProxy->getIdFromBandName(aux)>=0)
        {
         //out << "<BAND:" << QString::number(aux.length()) << ">" << aux  << " ";
         bandst = aux;
        }
    }
    nameCol = rec.indexOf("freq");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        //qDebug() << "FileManager::adifLogExportReturnList FREQ1: "  << aux << endl;
        aux = util->checkAndFixASCIIinADIF(aux);

        double freqTX = aux.toDouble();
        if (freqTX > 0.0)
        {
            //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
            if (dataProxy->getBandIdFromFreq(freqTX) != dataProxy->getIdFromBandName(bandst))
            {
                bandst = dataProxy->getBandNameFromFreq(freqTX);
            }
            if (_em != ModeEQSL)
            {
                out << "<FREQ:" << QString::number(aux.length()) << ">" << aux  << " ";
            }

        }
        if (dataProxy->getIdFromBandName(bandst)>0)
        {
            out << "<BAND:" << QString::number(bandst.length()) << ">" << bandst  << " ";
        }
    }
    // Now the BAND RX
    nameCol = rec.indexOf("band_rx");
    QString bandrxst = QString();
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        aux = dataProxy->getNameFromBandId(aux.toInt());

        if (dataProxy->getIdFromBandName(aux)>=0)
        {
            bandrxst = aux;
        }
    }
    nameCol = rec.indexOf("freq_rx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        double freqRX = aux.toDouble();
        if (freqRX > 0.0)
        {
        //TODO: Check if the Band is correctly defined. BAND Wins and freq is lost if not correct
            if (dataProxy->getBandIdFromFreq(freqRX) != dataProxy->getIdFromBandName(bandrxst))
            {
                bandrxst = dataProxy->getBandNameFromFreq(freqRX);
            }
            out << "<FREQ_RX:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
        if (dataProxy->getIdFromBandName(bandrxst)>0)
        {
            out << "<BAND_RX:" << QString::number(bandrxst.length()) << ">" << bandrxst  << " ";
        }
        // END of Band RX
    }
    nameCol = rec.indexOf("modeid");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        // get SubModeId to check if it is the same or not from modeid
        QString aux2 = dataProxy->getSubModeFromId(aux.toInt());
        //aux = db->getModeNameFromID2(aux.toInt());
        aux = dataProxy->getNameFromSubMode(aux2);
        if ((aux.length()>1) && (dataProxy->getIdFromModeName(aux)>=0))
        {
            out << "<MODE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
        if ((aux != aux2) && (aux.length()>1) && (dataProxy->getSubModeIdFromSubMode(aux2)>=0) )
        {
            out << "<SUBMODE:" << QString::number(aux2.length()) << ">" << aux2  << " ";
        }
    }
    nameCol = rec.indexOf("prop_mode");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>1)
        {
            out << "<PROP_MODE:" << QString::number(aux.length()) << ">" << aux  << " ";
            if (aux == "SAT")
            {
                propsat = true;
            }
        }
        //qDebug() << "FileManager::writeQuery: PROP_MODE"  << endl;
    }
    nameCol = rec.indexOf("sat_name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<SAT_NAME:" << QString::number(aux.length()) << ">" << aux  << " ";
            if (!propsat && (_em == ModeLotW))
            {
                out << "<PROP_MODE:3>SAT ";
                propsat = false;
            }
        }
    //qDebug() << "FileManager::writeQuery: SAT_NAME"  << endl;
    }
    nameCol = rec.indexOf("gridsquare");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<GRIDSQUARE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_gridsquare");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_GRIDSQUARE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("station_callsign");
    if ((nameCol>=0) && (_em != ModeEQSL))
    {
        aux = (query.value(nameCol)).toString();
        //qDebug() << "FileManager::writeQuery: StationCallSign: " << aux  << endl;
        if ((util->isValidCall(aux)))
        { // User selected one station callsign from the log
            out << "<STATION_CALLSIGN:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qso_date_off");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        tDateTime = util->getDateTimeFromSQLiteString(aux);

        if (tDateTime.isValid())
        {
            aux = util->getADIFDateFromQDateTime(tDateTime);
            out << "<QSO_DATE_OFF:" << QString::number(aux.length()) << ">" << aux  << " ";
            aux = util->getADIFTimeFromQDateTime(tDateTime);
            out << "<TIME_OFF:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("srx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<SRX:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }

    nameCol = rec.indexOf("srx_string");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<SRX_STRING:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("stx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<STX:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("stx_string");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<STX_STRING:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    //qDebug() << "FileManager::writeQuery - 100" << endl;
    nameCol = rec.indexOf("cqz");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())>0) && (0 < aux.toInt()) && (aux.toInt() < CQZones+1) )
        {
            out << "<CQZ:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("ituz");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())>0) && (0 < aux.toInt()) && (aux.toInt() < ITUZones+1) )
        {
            out << "<ITUZ:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
        //qDebug() << "FileManager::writeQuery: DXCC - Now..."  << endl;
    }
    nameCol = rec.indexOf("dxcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<DXCC:" << QString::number(aux.length()) << ">" << aux  << " ";
            //qDebug() << "FileManager::writeQuery: DXCC " << aux << endl;
        }
        //qDebug() << "FileManager::writeQuery: DXCC - Exported!"  << endl;
    }
    nameCol = rec.indexOf("address");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<ADDRESS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("age");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<AGE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("cnty");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<CNTY:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("comment");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<COMMENT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
        //qDebug() << "FileManager::writeQuery - 200" << endl;
    }
    nameCol = rec.indexOf("a_index");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<A_INDEX:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("ant_az");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<ANT_AZ:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("ant_el");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<ANT_EL:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("ant_path");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<ANT_PATH:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("arrl_sect");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<ARRL_SECT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("checkcontest");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<CHECKCONTEST:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    //qDebug() << "FileManager::writeQuery - 30" << endl;
    nameCol = rec.indexOf("class");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<CLASS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("cont");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<CONT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("contacted_op");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            out << "<CONTACTED_OP:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("contest_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<CONTEST_ID:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("points");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<APP_KLOG_POINTS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("multiplier");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<APP_KLOG_MULTIPLIER:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
        //qDebug() << "FileManager::writeQuery - 40" << endl;
    }
    nameCol = rec.indexOf("transmiterid");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<APP_KLOG_TRX:" << QString::number(aux.length()) << ">" << aux  << " ";
        }

    }
    nameCol = rec.indexOf("country");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<COUNTRY:" << QString::number(aux.length()) << ">" << aux  << " ";
        }

    }
    nameCol = rec.indexOf("credit_submitted");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<CREDIT_SUBMITTED:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("credit_granted");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<CREDIT_GRANTED:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("distance");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<DISTANCE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("darc_dok");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<DARC_DOK:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("eq_call");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            out << "<EQ_CALL:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("email");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            if (aux.contains("@") && (aux.contains(".")))
            {
                out << "<EMAIL:" << QString::number(aux.length()) << ">" << aux  << " ";
            }
        }
    }
    nameCol = rec.indexOf("eqsl_qslrdate");
    QDate date;
    if (nameCol>=0)
    {
            aux = (query.value(nameCol)).toString();
            date = util->getDateFromSQliteString(aux);
            if (date.isValid())
            {
                aux = util->getADIFDateFromQDate(date);
                out << "<EQSL_QSLRDATE:" << QString::number(aux.length()) << ">" << aux  << " ";
            }
    }
    nameCol = rec.indexOf("eqsl_qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<EQSL_QSLSDATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("eqsl_qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (  ((aux.length())==1) && (aux!="N") )
        {
            out << "<EQSL_QSL_RCVD:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("eqsl_qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (  ((aux.length())==1) && (aux!="N") )
        {
            out << "<EQSL_QSL_SENT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("fists");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<FISTS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("fists_cc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<FISTS_CC:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("force_init");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<FORCE_INIT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("guest_op");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<GUEST_OP:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("hrdlog_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<HRDLOG_QSO_UPLOAD_DATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("hrdlog_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && ((aux!="Y") || (aux!="N") || (aux!="M")) )
        {
            out << "<HRDLOG_QSO_UPLOAD_STATUS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_antenna");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_ANTENNA:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_dxcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_DXCC:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_fists");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_FISTS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("iota");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        //qDebug() << "FileManager::writeQuery (IOTA): " << aux << endl;
        if (((aux.length())>=4) && ((aux.length())<=6))
        {
            out << "<IOTA:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("iota_island_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        //qDebug() << "FileManager::writeQuery (IOTA_ID): " << aux << endl;
        if ((aux.length())>0)
        {
            out << "<IOTA_ISLAND_ID:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_iota");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())>=4) && ((aux.length())<=6))
        {
            out << "<MY_IOTA:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }

    nameCol = rec.indexOf("my_iota_island_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_IOTA_ISLAND_ID:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("k_index");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<K_INDEX:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_itu_zone");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_ITU_ZONE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("lat");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<LAT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("lon");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<LON:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
     }
    nameCol = rec.indexOf("my_lat");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_LAT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
     }
    nameCol = rec.indexOf("my_lon");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_LON:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
     }
    nameCol = rec.indexOf("lotw_qslrdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<LOTW_QSLRDATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("lotw_qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<LOTW_QSLSDATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }

    }
    nameCol = rec.indexOf("lotw_qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1) && (aux!="N") )
        {
            out << "<LOTW_QSL_RCVD:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("lotw_qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && (aux!="N") )
        {
            out << "<LOTW_QSL_SENT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }

    }
    nameCol = rec.indexOf("clublog_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<CLUBLOG_QSO_UPLOAD_DATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("clublog_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && ((aux!="Y") || (aux!="N") || (aux!="M")) )
        {
            out << "<CLUBLOG_QSO_UPLOAD_STATUS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qrzcom_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<QRZCOM_QSO_UPLOAD_DATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qrzcom_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())==1)  && ((aux!="Y") || (aux!="N") || (aux!="M")) )
        {
            out << "<QRZCOM_QSO_UPLOAD_STATUS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("max_bursts");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MAX_BURSTS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("ms_shower");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MS_SHOWER:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_city");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_CITY:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_cnty");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_CNTY:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_country");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_COUNTRY:" << QString::number(aux.length()) << ">" << aux  << " ";
        }        
    }
    nameCol = rec.indexOf("my_cq_zone");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.toInt()>0) && (aux.toInt()<41))
        {
            out << "<MY_CQ_ZONE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_NAME:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<NAME:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("operator");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            out << "<OPERATOR:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("owner_callsign");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (util->isValidCall(aux))
        {
            out << "<OWNER_CALLSIGN:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_postal_code");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_POSTAL_CODE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_rig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_RIG:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_sig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_SIG:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_sota_ref");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_SOTA_REF:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_postal_code");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_POSTAL_CODE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_state");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_STATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_street");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<MY_STREET:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("notes");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            aux.replace("\n", "---");
            out << "<NOTES:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("nr_bursts");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<NR_BURSTS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("nr_pings");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<NR_PINGS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("pfx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<PFX:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("precedence");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<PRECEDENCE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("public_key");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<PUBLIC_KEY:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qslmsg");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<QSLMSG:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qslrdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<QSLRDATE:" << QString::number(aux.length()) << ">" << aux << " ";
        }
    }
    nameCol = rec.indexOf("qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        if (date.isValid())
        {
            aux = util->getADIFDateFromQDate(date);
            out << "<QSLSDATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())==1) && (aux!="N") )
        {
            out << "<QSL_RCVD:" << QString::number(aux.length()) << ">" << aux  << " ";

            nameCol = rec.indexOf("qsl_rcvd_via");
            aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
            if ((aux.length())==1)
            {
                out << "<QSL_RCVD_VIA:" << QString::number(aux.length()) << ">" << aux  << " ";
            }
        }
    }
    nameCol = rec.indexOf("qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())==1) && (aux!="N") )
        {
            out << "<QSL_SENT:" << QString::number(aux.length()) << ">" << aux  << " ";
            nameCol = rec.indexOf("qsl_sent_via");
            aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
            if ((aux.length())==1)
            {
                out << "<QSL_SENT_VIA:" << QString::number(aux.length()) << ">" << aux  << " ";
            }
        }

    }
    nameCol = rec.indexOf("qsl_via");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<QSL_VIA:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qso_complete");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<QSO_COMPLETE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qso_random");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<QSO_RANDOM:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("qth");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<QTH:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("rst_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<RST_SENT:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("rst_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<RST_RCVD:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("region");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<REGION:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("rig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<RIG:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("rx_pwr");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if (((aux.length())>0) && (aux.toDouble()>0) )
        {
            out << "<RX_PWR:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("tx_pwr");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ( ((aux.length())>0) && (aux.toDouble()>0))
        {
            out << "<TX_PWR:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("sat_mode");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0)
        {
            out << "<SAT_MODE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("sfi");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<SFI:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("sig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<SIG:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("sig_info");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<SIG_INFO:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("silent_key");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<SILENT_KEY:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("skcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<SKCC:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("sota_ref");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<SOTA_REF:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("state");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<STATE:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("swl");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<SWL:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("ten_ten");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<TEN_TEN:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("ten_ten");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<TEN_TEN:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("uksmg");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<UKSMG:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
        }
    nameCol = rec.indexOf("ve_prov");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<VE_PROV:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_usaca_counties");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<MY_USACA_COUNTIES:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("usaca_counties");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<USACA_COUNTIES:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("vucc_grids");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<VUCC_GRIDS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("my_vucc_grids");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<MY_VUCC_GRIDS:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }
    nameCol = rec.indexOf("web");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        if ((aux.length())>0){
            out << "<WEB:" << QString::number(aux.length()) << ">" << aux  << " ";
        }
    }

    if (_logN == -1)
    {
        nameCol = rec.indexOf("lognumber");
        if (nameCol>=0)
        {
            aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
            if ((aux.length())>0)
            {
                out << "<APP_KLOG_LOGN:" << QString::number(aux.length()) << ">" << aux  << " ";
            }
        }
    }

    out << "<EOR>" << endl;
}














