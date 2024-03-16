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
//FileManager::FileManager(const QString &_klogDir, const QString &_softVersion, DataBase _db)
{
     //qDebug() << "FileManager::FileManager()-3: Dir(2)" << _klogDir;
    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    util->setCallValidation(false);
    db = new DataBase(Q_FUNC_INFO, klogVersion, util->getKLogDBFile());
    world = new World(dataProxy, Q_FUNC_INFO);
    awards = new Awards(dataProxy, Q_FUNC_INFO);
    //qDebug() << "FileManager::FileManager()-3: Dir(2) - END" ;
}

FileManager::~FileManager()
{
    delete(db);
    delete(awards);
    delete(world);
}

void FileManager::init()
{
    //util->setLongPrefixes(dataProxy->getLongPrefixes());
    //util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    klogVersion = dataProxy->getSoftVersion();
    klogDir = util->getHomeDir();
    defaultStationCallsign = QString();
    //constrid = 1;
    //constrid = 2;
    ignoreUnknownAlways = false;
    usePreviousStationCallsignAnswerAlways = false;
    duplicatedQSOSlotInSecs = 15;
    sendEQSLByDefault = false;
    dbCreated = false;
    rstTXDefault  = false;
    rstRXDefault = false;


    noMoreQso = false;
    //hashLogs.clear();
    util->setVersion(klogVersion);
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
           //qDebug() << "FileManager::checkADIFValidFormat-0 (not two) ";
        return false;
    }

    QString q0 = qs.at(0);
    QString q1 = qs.at(1);

      //qDebug() << "FileManager::checkADIFValidFormat: " << qs.at(0);
      //qDebug() << "FileManager::checkADIFValidFormat: " << qs.at(1);

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
           //qDebug() << "FileManager::checkADIFValidFormat-1 ";
        return false;
    }

    //i = (qs.at(0)).indexOf(":");
    //i = (qs.at(0)).length() - i -1;

       //qDebug() << "i = " << QString::number(i) << "/" << qs.at(0);

    //len = ( (qs.at(0)).right(i)).toInt();
       //qDebug() << "len = " << QString::number(len);

    if ( (q1).length() != len )
    {
           //qDebug() << "FileManager::checkADIFValidFormat-2: " << (qs.at(1)) << " - " << QString::number((qs.at(1)).length()) << "/" << QString::number(len);
        return false;
    }

    if (len <= 0)
    {
           //qDebug() << "FileManager::checkADIFValidFormat-3 ";
        return false;
    }

       //qDebug() << "FileManager::checkADIFValidFormat-4 (true)";

    return true;
}

bool FileManager::adifLogExport(const QString& _fileName, const int _logN)
{
    //qDebug() << "FileManager::adifLogExport" << _fileName;

    return adifLogExportToFile(_fileName, _logN, false, false, false);
}

void FileManager::showError (const QString &_txt)
{
    //qDebug() << Q_FUNC_INFO << ": "  << _txt;

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - Error"));
    msgBox.setIcon(QMessageBox::Warning);

    QString aux = QString(_txt );
    msgBox.setText(aux);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void FileManager::setSendQSLByDefault (const bool _send)
{
    sendEQSLByDefault = _send;
}

QList<int> FileManager::adifLogExportReturnList(const QString& _fileName, const QString &_callsign, const QString &_grid, const QDate &_startDate, const QDate &_endDate, const int _logN, const ExportMode _em)
{
    //qDebug() << Q_FUNC_INFO << ": Start)" << _fileName << "/" << _callsign << "/ " << _grid;
    QList<int> qsos;
    qsos.clear();
    if ((!util->isValidCall(_callsign)) && (_callsign != "ALL") && (_callsign !="NOT"))
    {
         showError(tr("The selected callsign (%1) is not valid, please check it again to export the log.").arg(_callsign));
         return qsos;
    }
    noMoreQso = false;
    if (_logN != -1)
    { // We will export data from ALL logs.
        if (!dataProxy->doesThisLogExist(_logN))
        {
            //qDebug() << Q_FUNC_INFO << ": The log does not exist";
            showError(tr("The selected log does not exist, please check it again."));
            return qsos;
        }
    }

    QString queryStringCount;
    QString queryString;
    QString _queryStation;
    QString _queryGrid;

    if (_callsign == "NOT")
    {
         //qDebug() << Q_FUNC_INFO << ": else: " << _callsign;
         _queryStation = QString(" ((station_callsign ='') OR (station_callsign IS NULL))");
    }
    else if (_callsign == "ALL")
    {
         //qDebug() << Q_FUNC_INFO << ": ALL: " << _callsign;
        _queryStation = QString(" station_callsign !='ALL'");
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << ": ValidCall: " << _callsign;
        _queryStation = QString(" station_callsign ='%1'").arg(_callsign);
    }
    //qDebug() << Q_FUNC_INFO << ": _queryStation: " << _queryStation;
    if (util->isValidGrid(_grid))
    {
        _queryGrid = QString(" AND my_gridsquare = '%1'").arg(_grid);
    }
    else if (_grid == "NOT")
    {
        _queryGrid = QString(" AND (my_gridsquare = '' OR my_gridsquare IS NULL)");
    }
    else
    {
        _queryGrid = QString();
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

    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
         showError(tr("The file %1 can't be opened.").arg(_fileName));
         return qsos;
    }

    QSqlQuery query;

    if (_em == ModeLotW)
    {
         //qDebug() << Q_FUNC_INFO << ":  Exporting for LoTW";
        // LoTW Required fields: call sign, UTC Date, UTC time, Mode, Band
        // LoTW Optional fields: RX band, Frecuency TX, frecuency RX, Propagation mode, Satellite

        queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation + _queryGrid + QString(" AND lotw_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
        queryString = QString("SELECT id, call, freq, bandid, band_rx, freq_rx, modeid, gridsquare, my_gridsquare, qso_date, prop_mode, sat_name, station_callsign FROM log WHERE") + _queryStation + QString(" AND lotw_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
    }
    else if (_em == ModeClubLog)
    {
         //qDebug() << Q_FUNC_INFO << ": Exporting for ClubLog";
        queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation + QString(" AND clublog_qso_upload_status='M'") + _queryDateFrom + _queryDateTo;
        queryString = QString("SELECT id, call, rst_sent, rst_rcvd, freq, bandid, band_rx, modeid, qso_date, qsl_rcvd, qslrdate, qslsdate, prop_mode, operator, station_callsign, dxcc, qsl_sent, lotw_qsl_rcvd, credit_granted, notes, qso_date_off FROM log WHERE") + _queryStation + QString(" AND clublog_qso_upload_status='M'") + _queryDateFrom + _queryDateTo;
    }
    else if (_em == ModeEQSL)
    {
         //qDebug() << Q_FUNC_INFO << ":  Exporting for eQSL";
        queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation + QString(" AND eqsl_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
        queryString = QString("SELECT id, call, rst_sent, freq, bandid, modeid, qso_date, prop_mode, operator, station_callsign, sat_name, my_cnty, my_gridsquare, my_lat, my_lon FROM log WHERE") + _queryStation + QString(" AND eqsl_qsl_sent='Q'") + _queryDateFrom + _queryDateTo;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": Exporting normal ADIF";
        if (_callsign == "ALL")
        {
            queryStringCount = QString("SELECT COUNT (id) FROM log");
            queryString = QString("SELECT * FROM log");
        }
        else if (_callsign == "NOT")
        {
            queryStringCount = QString("SELECT COUNT (id) FROM log WHERE ") + _queryStation + _queryDateFrom + _queryDateTo + _queryLog;
            queryString = QString("SELECT * FROM log WHERE ") + _queryStation + _queryDateFrom + _queryDateTo + _queryLog;
        }
        else
        {
            queryStringCount = QString("SELECT COUNT (id) FROM log WHERE") + _queryStation  + _queryDateFrom + _queryDateTo + _queryLog;
            queryString = QString("SELECT * FROM log WHERE") + _queryStation + _queryDateFrom + _queryDateTo + _queryLog;
        }
    }
    //qDebug() << Q_FUNC_INFO << ":  Query Count: " << queryStringCount;
    //qDebug() << Q_FUNC_INFO << ":  Query data: " << queryString;
    int numberOfQsos = dataProxy->getHowManyQSOInLog(_logN);
    int i = 0;

    bool sqlOK = query.exec(queryStringCount);
    //qDebug() << Q_FUNC_INFO << ": " << query.lastQuery();
    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << ":  Query Error" ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
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
    //qDebug() << Q_FUNC_INFO << ":  " << QString::number(step);

    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.setWindowTitle(tr("Export"));
    progress.setAutoClose(true);


    writeADIFHeader(out, _em, numberOfQsos);

    i = 0;

    sqlOK = query.exec(queryString);
     //qDebug() << Q_FUNC_INFO << ":  " << query.lastQuery();
    if (!sqlOK)
    {
         //qDebug() << Q_FUNC_INFO << ":  Query Error" ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return qsos;
    }


    int nameCol;

    //qDebug() << Q_FUNC_INFO << ":  Entering the While..." ;
    QSqlRecord rec;
    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << Q_FUNC_INFO << ":  Start of While" ;
        if (query.isValid())
        {
            rec = query.record();
            nameCol = rec.indexOf("id");
            qsos.append((query.value(nameCol)).toInt());
            writeQuery(rec, out, _em, false, false, _logN); // JustMarked = false, onlyRequested = false
            //Use std::move(query) instead of query?
            // Copy queries is deprecated

            i++;
            //qDebug() << Q_FUNC_INFO << ":  Start of isValid" ;
        } // END of if (query.isValid())
        else
        {
            //qDebug() << Q_FUNC_INFO << ":  Querty NOT isValid" ;
        }

        //qDebug() << Q_FUNC_INFO << ":  Checking if cancelled" ;

        if (( (i % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << Q_FUNC_INFO << ":  ********************************   UPDATING THE MESSAGE! " << QString::number(i) ;
            QString aux = tr("Exporting ADIF file...\n QSO: %1 / %2 ").arg(i).arg(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);
            //qDebug() << Q_FUNC_INFO << ":  ********************************   UPDATING THE MESSAGE: " << aux ;
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

    //qDebug() << Q_FUNC_INFO << ":  End: " << QString::number(qsos.count());
    progress.setValue(numberOfQsos);
    return qsos;
}

QList<int> FileManager::adifLogExportReturnList2(const QString& _fileName, const QString &_callsign, QList<int> _qsos, const ExportMode _em, const int _logN)
{
    Q_UNUSED(_logN);
    //qDebug() << Q_FUNC_INFO << " - Start";
    //qDebug() << Q_FUNC_INFO << " - QSOs: " << QString::number(_qsos.length ());
    //qDebug() << Q_FUNC_INFO << ": Start)" << _fileName << "/" << _callsign << "/ " << _grid;
    QList<int> qsos;
    qsos.clear();
    if ((!util->isValidCall(_callsign)) && (_callsign != "ALL") && (_callsign !="NOT"))
    {
         showError(tr("The selected callsign (%1) is not valid, please check it again to export the log.").arg(_callsign));
         return qsos;
    }

    QString queryString;
    if (_em == ModeLotW)
    {
        queryString = QString("call, freq, bandid, band_rx, freq_rx, modeid, gridsquare, my_gridsquare, qso_date, prop_mode, sat_name, station_callsign");

    }
    else if (_em == ModeClubLog)
    {
         queryString = QString("call, rst_sent, rst_rcvd, freq, bandid, band_rx, modeid, qso_date, qsl_rcvd, qslrdate, qslsdate, prop_mode, operator, station_callsign, dxcc, qsl_sent, lotw_qsl_rcvd, credit_granted, notes, qso_date_off");
    }
    else if (_em == ModeEQSL)
    {
        queryString = QString("call, rst_sent, freq, bandid, modeid, submode, qso_date, prop_mode, operator, station_callsign, sat_name, my_cnty, my_gridsquare, my_lat, my_lon, qslmsg, sat_mode, sat_name");
    }
    else
    { // THis is ModeADIF and ModeQRZ we will upload ALL the ADIF files
        queryString = QString("*");
    }

    if (adifQSOsExport2(_fileName, queryString, _qsos, _em))
    {
        return _qsos;
    }
    else
    {
        return qsos;
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    return qsos;
}

bool FileManager::adifQSOsExport(const QString& _fileName, QList<int> _qsos)
{
   //qDebug() << "FileManager::adifQSOsExport: " << _fileName;
    int numberOfQSOs = _qsos.length();
    if (numberOfQSOs<1)
    {
        //TODO: Warn the user NO QSOS TO EXPORT
       //qDebug() << "FileManager::adifQSOsExport: No QSOs received to be exported";
    }
    noMoreQso = false;
   //qDebug() << "FileManager::adifQSOsExport - 01";
    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
        return false;
   //qDebug() << "FileManager::adifQSOsExport - 02";
    QTextStream out(&file);

    QSqlQuery query;
    //qDebug() << "FileManager::adifQSOsExport - 10";
    QString numbers = QString();
    foreach (int i, _qsos)
    {
        //qDebug() << "FileManager::adifQSOsExport - query: " << QString("id = '%1'").arg(i) ;
        numbers = numbers + QString::number(i) ;
        if (i != _qsos.last())
        {
           numbers = numbers + ", ";
        }
    }

    QString queryString = QString("SELECT * FROM log WHERE id IN (%1)").arg(numbers);
    //qDebug() << "FileManager::adifQSOsExport: writing the header";
    writeADIFHeader(out, ModeADIF, _qsos.length());
   //qDebug() << "FileManager::adifQSOsExport: writing the body";

    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
    //qDebug() << "FileManager::adifQSOsExport: query error: " << query.lastQuery();
     emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
   //qDebug() << "FileManager::adifQSOsExport: query: " << query.lastQuery();
    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQSOs, this);
    progress.setMaximum(numberOfQSOs);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Export progress"));
    int currentQso = 0;
    int step = util->getProgresStepForDialog(numberOfQSOs);
    QSqlRecord rec;
    while ( (query.next()) && (!noMoreQso) )
    {
       //qDebug() << "FileManager::adifLogExportToFile -  Just in the While";
        if (query.isValid())
        {
            rec = query.record();
            //qDebug() << "FileManager::adifLogExportToFile -  Query is Valid";
             writeQuery(rec, out, ModeADIF, false, false, -1);
        } // Closes the isValid
        else {
           //qDebug() << "FileManager::adifLogExportToFile -  Query is NOT Valid";
        }

       //qDebug() << "FileManager::adifLogExportToFile -  before showing progress: " << QString::number(currentQso);
        currentQso++;

        if (( (currentQso % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
           //qDebug() << "FileManager::adifLogExportToFile -  Showing progress: " ;
            QString aux1 = tr("Writing ADIF file...\n QSO: ")  + QString::number(currentQso) + "/" + QString::number(numberOfQSOs);
            progress.setLabelText(aux1);
            progress.setValue(currentQso);
        }
       //qDebug() << "FileManager::adifLogExportToFile -  after showing progress (current%Step): " << QString::number(currentQso%step);
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
   //qDebug() << "FileManager::adifQSOsExport - END";
    return true;
}

bool FileManager::adifQSOsExport2(const QString& _fileName, const QString& _fields, QList<int> _qsos, ExportMode _em)
{ // The fields are the database fields that are to be selected in the query
    //qDebug() << Q_FUNC_INFO << " - Start";
    int numberOfQSOs = _qsos.length();
    if (numberOfQSOs<1)
    {
        //TODO: Warn the user NO QSOS TO EXPORT
       //qDebug() << Q_FUNC_INFO << " - No QSOs received to be exported";
    }
    QString fields = _fields;
    if (_fields.length ()<1)
    {
        fields = "*";
    }
    bool exportAll = false;
    if ((_qsos.at(0) == -1) && (_qsos.size() == 1))
    {
        numberOfQSOs = dataProxy->getHowManyQSOInLog(-1);
        exportAll = true;
    }


    noMoreQso = false;
   //qDebug() << Q_FUNC_INFO << " - 01";
    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
        return false;
   //qDebug() << Q_FUNC_INFO << " - 02";
    QTextStream out(&file);

    QSqlQuery query;
    //qDebug() << Q_FUNC_INFO << " - 10";
    QString numbers = QString();
    foreach (int i, _qsos)
    {
        //qDebug() << Q_FUNC_INFO << " - query: " << QString("id = '%1'").arg(i) ;
        numbers = numbers + QString::number(i) ;
        if (i != _qsos.last())
        {
           numbers = numbers + ", ";
        }
    }

    QString queryString;

    if (exportAll)
    {
        queryString = QString("SELECT %1 FROM log").arg(fields);
    }
    else
    {
        queryString = QString("SELECT %1 FROM log WHERE id IN (%2)").arg(fields).arg(numbers);
    }
    //qDebug() << Q_FUNC_INFO << " - writing the header";
    writeADIFHeader(out, _em, numberOfQSOs);
   //qDebug() << Q_FUNC_INFO << " - writing the body";

    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " -query error: " << query.lastQuery();
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    //qDebug() << Q_FUNC_INFO << " - query: " << query.lastQuery();
    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQSOs, this);
    progress.setMaximum(numberOfQSOs);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Export progress"));
    int currentQso = 0;
    int step = util->getProgresStepForDialog(numberOfQSOs);
    QSqlRecord rec;
    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << Q_FUNC_INFO << " -  Just in the While";
        if (query.isValid())
        {
            rec = query.record();
            //qDebug() << Q_FUNC_INFO << " -  Query is Valid";
            writeQuery(rec, out, ModeADIF, false, false, -1);
        } // Closes the isValid
        else {
            //qDebug() << Q_FUNC_INFO << " -  Query is NOT Valid";
        }

        //qDebug() << Q_FUNC_INFO << " -  before showing progress: " << QString::number(currentQso);
        currentQso++;

        if (( (currentQso % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << Q_FUNC_INFO << " -  Showing progress: " ;
            QString aux1 = tr("Writing ADIF file...\n QSO: ")  + QString::number(currentQso) + "/" + QString::number(numberOfQSOs);
            progress.setLabelText(aux1);
            progress.setValue(currentQso);
        }
        //qDebug() << Q_FUNC_INFO << " -  after showing progress (current%Step): " << QString::number(currentQso%step);
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
    //qDebug() << Q_FUNC_INFO << " - END";
    return true;
}


void FileManager::setCallValidation (const bool _b)
{
    //util->setCallValidation(_b);
    util->setCallValidation(_b);
    dataProxy->setCallValidation(_b);
}

bool FileManager::adifLogExportToFile(const QString& _fileName, const int _logN, bool justMarked, bool _qslRequested , bool _lotw)
{
    //adifLogExportToFile(const QString& _fileName, const int _logN=0, bool justMarked = false, bool _qslRequested = false, bool _lotw=false);
    // If _logN = 0, then we will export ALL the logs.
    //qDebug() << Q_FUNC_INFO << _fileName;
    Q_UNUSED(_lotw);
    bool exportJustMarkedQSO = justMarked;
    //bool marked = false;
    bool exportOnlyQSLRequested = _qslRequested;
    //bool haveMode = false;
    noMoreQso = false;
    //qDebug() << Q_FUNC_INFO << " - 10";
    QFile file(_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
        return false;
    //qDebug() << Q_FUNC_INFO << " - 11";

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
    //qDebug() << Q_FUNC_INFO << " - 20";
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
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        }

        if (query.next())
        {
            if (query.isValid())
            {
                numberOfQsos = query.value(0).toInt();
            }
        }
           //qDebug() << "FileManager::adifLogExportToFile -  numberOfQsos = " << QString::number(numberOfQsos)<< QT_ENDL;
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
            emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().nativeErrorCode(), query1.lastQuery());
        }
        query1.next();

        if (query1.isValid())
        {
            numberOfQsos = (query1.value(0)).toInt();
        }
          //qDebug() << "FileManager::adifLogExportToFile -  numberOfQsos = " << QString::number(numberOfQsos)<< QT_ENDL;
    }
    //qDebug() << Q_FUNC_INFO << " - 30";
    //qDebug() << "FileManager::adifLogExportToFile - end get numberOfQsos = " << QString::number(numberOfQsos);

    step = util->getProgresStepForDialog(numberOfQsos);
    //qDebug() << Q_FUNC_INFO << " - 31";
    //qDebug() << "FileManager::adifLogExportToFile END -  Steps = " << QString::number(step);
    //step = getProgresStepForDialog(numberOfQsos);

    writeADIFHeader(out, ModeADIF, numberOfQsos);
    //qDebug() << Q_FUNC_INFO << " - 32";
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
    //qDebug() << Q_FUNC_INFO << " - 40";
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    //qDebug() << Q_FUNC_INFO << " - 41";
    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Export progress"));

    QDateTime date;
    //qDebug() << Q_FUNC_INFO << " - 50";
    QSqlRecord rec;
    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << Q_FUNC_INFO << " - 50.1";

        if (query.isValid())
        {
            rec = query.record();
            writeQuery(rec, out, ModeADIF, exportJustMarkedQSO, exportOnlyQSLRequested, _logN);
            //qDebug() << Q_FUNC_INFO << " - 50.2";
        } // Closes the isValid

        //qDebug() << "FileManager::adifLogExportToFile -  before showing progress: " << QString::number(currentQso);
        currentQso++;
        if (( (currentQso % step )== 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << "FileManager::adifLogExportToFile -  Showing progress: " ;
            aux1 = tr("Writing ADIF file...\n QSO: ")  + QString::number(currentQso) + "/" + QString::number(numberOfQsos);
            progress.setLabelText(aux1);
            progress.setValue(currentQso);
            //qDebug() << Q_FUNC_INFO << " - 50.3";
        }
        //qDebug() << "FileManager::adifLogExportToFile -  after showing progress (current%Step): " << QString::number(currentQso%step);

        if ( progress.wasCanceled() )
        {
            //qDebug() << Q_FUNC_INFO << " - 50.7";
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
    //qDebug() << Q_FUNC_INFO << " - 60";
      //qDebug() << "FileManager::adifLogExportToFile -  after the While";
    progress.setValue(numberOfQsos);
    //qDebug() << Q_FUNC_INFO << " - 70";
    if (noMoreQso)
    {
    //TODO: Remove the file (_fileName)
          //qDebug() << "FileManager::adifLogExportToFile -  noMoreQSO = true";
        file.remove();
        //qDebug() << Q_FUNC_INFO << " - 70.1";
        return false;
    }
    else
    {
          //qDebug() << "FileManager::adifLogExportToFile -  noMoreQSO = false";
        //qDebug() << Q_FUNC_INFO << " - 70.2";
        return writeBackupDate();
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

bool FileManager::printQs(const QStringList &_line)
//bool FileManager::printQs(const QString &_q, const QStringList _line)
{
    QStringList qs = _line;

    for (int i = 0; i<qs.size(); i++){
           //qDebug() << qs.at(i) << " ";
    }

    return true;
}

bool FileManager::adifLogExportMarked(const QString& _fileName)
{
    //qDebug() << "FileManager::adifLogExportMarked:" << _fileName;
    return adifLogExportToFile(_fileName, 0, true, false, false);
}

QList<int> FileManager::adifLoTWReadLog2(const QString& fileName, const int logN)
{
  //qDebug() << Q_FUNC_INFO << " - " << fileName;
   //QSO qso(Q_FUNC_INFO);
   QString stationCallSign;
   stationCallSign.clear();
   bool addNewQSOs = false;
   bool askedToAddNewQSOs = false;
   QList<int> _qsos;
   _qsos.clear();
   if (!dataProxy->doesThisLogExist(logN))
   {
       //qDebug() << Q_FUNC_INFO << " - : ERROR: The log does not exist: " << QString::number(logN);
       return _qsos;
   }
   //qDebug() << Q_FUNC_INFO << " -  - 10" ;
   //QString fn = "blotw.adi";
   QFile file( fileName );
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
   {
       //qDebug() << Q_FUNC_INFO << " - File not found" << fileName;
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
   //qDebug() << Q_FUNC_INFO << " -  - 20" ;
    int numberOfQsos = 0;
    QString line = QString();
    bool hasEOH = false;
    //qDebug() << Q_FUNC_INFO << " -  - 30" ;
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
        //qDebug() << Q_FUNC_INFO << " -  - line-1: " << line ;
        numberOfQsos = numberOfQsos + line.count("EOR>");
        if ((line.count("<EOH>")>0) && (!hasEOH))
        {
            //qDebug() << Q_FUNC_INFO << " -  - line-1: EOH FOUND!"  ;
            hasEOH = true;
            pos = file.pos();
        }
    }
    file.seek(pos);
    int step = util->getProgresStepForDialog(numberOfQsos);

    QProgressDialog progress(tr("Processing LoTW ADIF file..."), tr("Abort processing"), 0, numberOfQsos, this);
    progress.setMaximum(numberOfQsos);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setValue(0);
    progress.setWindowTitle(tr("LoTW reading"));
    progress.setAutoClose(true);
    //qDebug() << Q_FUNC_INFO << " -  - After header while" ;
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
        fields << line.split("<", QT_SKIP);

        foreach(QString a, fields)
        {
            //qDebug() << Q_FUNC_INFO << " -  - Fields: " << a ;
            QString fullField = "<" + a.trimmed();
            if (fullField.contains("<EOR>"))
            {
                if (qso.isValid())
                {
                   //qDebug() << Q_FUNC_INFO << " -  VALID QSO: ";
                    qso.setLogId(logN);
                    if ((util->isValidCall(stationCallSign)) && (stationCallSign.length ()>0))
                    {
                        QString aux = QString("<STATION_CALLSIGN:%1>%2").arg(QString::number(stationCallSign.length())).arg(stationCallSign);
                        qso.setData(aux);
                    }
                    QList<int> dupeQsos;
                    dupeQsos.clear();
                    int mode;
                    if (qso.getSubmode().isEmpty()) {
                        mode = dataProxy->getIdFromModeName(qso.getMode());
                    } else {
                        mode = dataProxy->getSubModeIdFromSubMode(qso.getSubmode());
                    }
                    dupeQsos << dataProxy->isThisQSODuplicated(Q_FUNC_INFO, qso.getCall(), qso.getDateTimeOn(), dataProxy->getIdFromBandName(qso.getBand()), mode, duplicatedQSOSlotInSecs);

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
                        //qDebug() << Q_FUNC_INFO << " -  -  New QSO ... adding ..."  ;
                        qso.setDefaultEQSLSentServices (sendEQSLByDefault);
                        int lastId = dataProxy->addQSO(qso);
                        if (lastId>0)
                        {
                            _qsos.append(lastId);
                            //qDebug() << Q_FUNC_INFO << " -  -  New QSO ... added ..."  ;
                        }
                    }
                    else
                    {
                        //qDebug() << Q_FUNC_INFO << " -  -  EXisting QSO or not adding, Updating LoTW QSL status to: " <<  qso.getLoTWQSL_RCVD() ;
                        if (dataProxy->setLoTWQSLRec (dupeQsos.at(0), qso.getLoTWQSL_RCVD(), qso.getLoTWQSLRDate()))
                        {
                            _qsos.append(dupeQsos.at(0));
                            //qDebug() << Q_FUNC_INFO << " - : Modified QSO: " << QString::number(dupeQsos.at(0));
                        }
                    }
                    i++;
                    qso.clear();
                }
            }
            else
            {
                qso.setData(fullField);
                //qDebug() << Q_FUNC_INFO << ": - " << fullField;
                if (fullField.contains ("<CALL:"))
                {
                    //qDebug() << Q_FUNC_INFO << ": Getting the DXCC for a call.";
                    int _dxcc = dataProxy->getDXCCFromPrefix (qso.getCall ());
                    //qDebug() << Q_FUNC_INFO << ": DXCC: "  << QString::number(_dxcc);
                    if (util->isValidDXCC (_dxcc))
                    {
                        //qDebug() << Q_FUNC_INFO << ": - Adding a DXCC" ;
                        qso.setDXCC (_dxcc);
                    }
                }
            }
        }
        if (( (i % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << Q_FUNC_INFO << " - : ********************************   UPDATING THE MESSAGE! " << QString::number(i) ;
            QString aux = tr("Processing LoTW ADIF file......\n QSO: %1 / %2 ").arg(i).arg(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);
            //qDebug() << Q_FUNC_INFO << " - : ********************************   UPDATING THE MESSAGE: " << aux ;
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

   //qDebug() << Q_FUNC_INFO << " -  - END: " << QString::number(_qsos.length ());
   return _qsos;
}

bool FileManager::adifReadLog2(const QString& tfileName, const int logN)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << tfileName << "/" << QString::number(logN);
    QFile file( tfileName );
    if (!file.exists ())
    {
        //qDebug() << Q_FUNC_INFO << " - END: file does not exist";
        return false;
    }
    int qsos = howManyQSOsInFile (file);
    //qDebug() << Q_FUNC_INFO << " - QSOs: " << QString::number(qsos);
    qint64 pos = passHeader (file); // Position in the file to calculate where the header ends
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //qDebug() << Q_FUNC_INFO << "  File not found" ;
        return false;
    }

    file.seek (pos); // QSO Data starts here

    QSO qso;
    QStringList fields; // fields keeps the running array,

    fields.clear ();
    QString line = QString();
    //line.clear();
    qso.clear ();

    //qDebug() << Q_FUNC_INFO << ": Progress defined" ;
    QProgressDialog progress(tr("Reading ADIF file..."), tr("Abort reading"), 0, qsos, this);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setValue(0);
    //progress.setWindowTitle(tr("Import"));
    progress.setAutoClose(true);
    int step = util->getProgresStepForDialog(qsos);
    int i = 0;
    bool noMoreQSO = false;
    //qDebug() << Q_FUNC_INFO << ": We start the while" ;
    while ((!file.atEnd()) && (!noMoreQSO))
    {
        // One line is read and splitted into the list of fields
        // Fields are analyzed one by one and extracted from the list of fields
        // Fields are added to the QSO
        // If the field is <EOR> the QSO is completed and added to the log.
        // Once the QSO is added to the log, QSO is cleared and process continues
        // Once the list of fields is empty, we read another file and start again
        // until we reach the end of file
        line.clear();
        line.append(file.readLine().trimmed().toUpper());
        //qDebug() << Q_FUNC_INFO << ": Reading the line: " << line ;
        fields << line.split("<", QT_SKIP);
        while (!fields.isEmpty())
        {
            //qDebug() << Q_FUNC_INFO << QString(": Fields still has %1 items").arg(fields.count()) ;
            QString fieldToAnalyze = "<" + (fields.takeFirst()).trimmed();
            //qDebug() << Q_FUNC_INFO << QString(": Extracted: %1").arg(fieldToAnalyze) ;
            if (fieldToAnalyze.contains ("<EOR>"))
            {
                //qDebug() << Q_FUNC_INFO << QString(": EOR detected, QSO added");
                qso.setLogId (logN);
                qso.toDB ();
                qso.clear ();
                i++;
                if (( (i % step ) == 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    //qDebug() << Q_FUNC_INFO << " MOD 0 - i = " << QString::number(i) ;
                    QString aux = tr("Importing ADIF file...") + "\n" + tr(" QSO: ")  + QString::number(i) + "/" + QString::number(qsos);
                    progress.setLabelText(aux);
                    progress.setValue(i);
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO << " Mod: "<< QString::number(i) << " mod " << QString::number(step) << " = " << QString::number(i % step);
                }
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << QString(": Adding this to the QSO: %1").arg(fieldToAnalyze) ;
                //fieldToAnalyze must be an ADIF record: <Field:length:Data type>Data
                qso.setData (fieldToAnalyze);
            }
            if ( progress.wasCanceled() )
            {
                //qDebug() << Q_FUNC_INFO << QString(": Progress Cancelled") ;
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("KLog - User cancelled"));
                QString aux = QString(tr("You have canceled the file import. The file will be removed and no data will be imported.") + "\n" + tr("Do you still want to cancel?"));
                msgBox.setText(aux);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);
                int ret = msgBox.exec();
                switch (ret) {
                case QMessageBox::Yes:
                    // Yes was clicked
                    noMoreQSO = true;
                    break;
                case QMessageBox::No:
                    // No Save was clicked
                    break;
                default:
                    // should never be reached
                    break;
                }
            }
            //qDebug() << Q_FUNC_INFO << QString(": Field process finished: ").arg(fieldToAnalyze) ;
        }
        //qDebug() << Q_FUNC_INFO << QString(": List of fields is empty!") ;
    }
    //qDebug() << Q_FUNC_INFO << QString(": End of File or no more QSOs") ;
    file.close ();
    progress.setValue(qsos);    // Closes the progressDialog

    //qDebug() << Q_FUNC_INFO << " - END";
    return true;
}

qint64 FileManager::passHeader(QFile & _f)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << _f.fileName ();
    QFile &file = _f;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //qDebug() << Q_FUNC_INFO << "  File not found" ;
        return false;
    }
    //bool hasEOH = false;
    qint64 pos = file.pos();
    QString line;
    while ( !file.atEnd()   )
    {
        line = file.readLine ().toUpper ();
        //qDebug() << Q_FUNC_INFO << " - " << line;
        if (line.count ("<EOH>")>0)
        {
            break;
        }
    }
    pos = file.pos();
    file.close ();
    //qDebug() << Q_FUNC_INFO << " - END (" << QString::number(pos) << ")";
    return pos;
}

QHash<QString, int> FileManager::SwitchHash;

void FileManager::initializeSwitchHash() {
    SwitchHash = {
    {"CALL", 1},
    {"QSO_DATE", 2},
    {"BAND", 3},
    {"MODE", 4},
    {"SUBMODE", 5},
    {"SRX", 6},
    {"STX", 7},
    {"TIME_ON", 8},
    {"QSO_DATE_OFF", 9},
    {"BAND_RX", 10},
    {"TIME_OFF", 11},
    {"RST_SENT", 12},
    {"RST_RCVD", 13},
    {"SRX_STRING", 14},
    {"STX_STRING", 15},
    {"CQZ", 16},
    {"ITUZ", 17},
    {"DXCC", 18},
    {"ADDRESS", 19},
    {"AGE", 20},
    {"CNTY", 21},
    {"COMMENT", 22},
    {"A_INDEX", 23},
    {"ANT_AZ", 24},
    {"ANT_EL", 25},
    {"ANT_PATH", 26},
    {"ARRL_SECT", 27},
    {"AWARD_GRANTED", 28},
    {"AWARD_SUBMITTED", 29},
    {"CHECKCONTEST", 30},
    {"CLASS", 31},
    {"CONT", 32},
    {"CONTACTED_OP", 33},
    {"CONTEST_ID", 34},
    {"COUNTRY", 35},
    {"CREDIT_SUBMITTED", 36},
    {"CREDIT_GRANTED", 37},
    {"DISTANCE", 38},
    {"DARC_DOK", 39},
    {"EQ_CALL", 40},
    {"EMAIL", 41},
    {"EQSL_QSLRDATE", 42},
    {"EQSL_QSLSDATE", 43},
    {"EQSL_QSL_RCVD", 44},
    {"EQSL_QSL_SENT", 45},
    {"FISTS", 46},
    {"FISTS_CC", 47},
    {"FORCE_INIT", 48},
    {"FREQ", 49},
    {"FREQ_RX", 50},
    {"GRIDSQUARE", 51},
    {"HRDLOG_QSO_UPLOAD_DATE", 53},
    {"HRDLOG_QSO_UPLOAD_STATUS", 54},
    {"MY_GRIDSQUARE", 55},
    {"MY_ANTENNA", 56},
    {"IOTA", 57},
    {"IOTA_ISLAND_ID", 58},
    {"MY_IOTA", 59},
    {"MY_DXCC", 60},
    {"MY_FISTS", 61},
    {"MY_IOTA_ISLAND_ID", 62},
    {"K_INDEX", 63},
    {"LAT", 64},
    {"LON", 65},
    {"MY_LAT", 66},
    {"MY_LON", 67},
    {"MY_ITU_ZONE", 68},
    {"MY_POSTAL_CODE", 69},
    {"LOTW_QSLRDATE", 70},
    {"LOTW_QSLSDATE", 71},
    {"LOTW_QSL_RCVD", 72},
    {"LOTW_QSL_SENT", 73},
    {"CLUBLOG_QSO_UPLOAD_DATE", 74},
    {"CLUBLOG_QSO_UPLOAD_STATUS", 75},
    {"MAX_BURSTS", 76},
    {"MS_SHOWER", 77},
    {"MY_CITY", 78},
    {"MY_CNTY", 79},
    {"MY_COUNTRY", 80},
    {"MY_CQ_ZONE", 81},
    {"MY_NAME", 82},
    {"NAME", 83},
    {"OPERATOR", 84},
    {"STATION_CALLSIGN", 85},
    {"OWNER_CALLSIGN", 86},
    {"MY_RIG", 87},
    {"MY_SIG", 88},
    {"MY_SIG_INFO", 89},
    {"MY_SOTA_REF", 90},
    {"MY_STATE", 91},
    {"STATE", 92},
    {"MY_STREET", 93},
    {"MY_USACA_COUNTIES", 94},
    {"MY_VUCC_GRIDS", 95},
    {"NOTES", 96},
    {"NR_BURSTS", 97},
    {"NR_PINGS", 98},
    {"PFX", 99},
    {"PRECEDENCE", 100},
    {"PROP_MODE", 101},
    {"PUBLIC_KEY", 102},
    {"QRZCOM_QSO_UPLOAD_DATE", 103},
    {"QRZCOM_QSO_UPLOAD_STATUS", 104},
    {"QSLMSG", 105},
    {"QSLRDATE", 106},
    {"QSLSDATE", 107},
    {"QSL_RCVD", 108},
    {"QSL_SENT", 109},
    {"QSL_RCVD_VIA", 110},
    {"QSL_SENT_VIA", 111},
    {"QSL_VIA", 112},
    {"QSO_COMPLETE", 113},
    {"QSO_RANDOM", 114},
    {"QTH", 115},
    {"REGION", 116},
    {"RIG", 117},
    {"RX_PWR", 118},
    {"TX_PWR", 119},
    {"SAT_MODE", 120},
    {"SAT_NAME", 121},
    {"SFI", 122},
    {"SIG", 123},
    {"SIG_INFO", 124},
    {"SILENT_KEY", 125},
    {"SKCC", 126},
    {"SOTA_REF", 127},
    {"SWL", 128},
    {"TEN_TEN", 129},
    {"UKSMG", 130},
    {"USACA_COUNTIES", 131},
    {"VE_PROV", 132},
    {"VUCC_GRIDS", 133},
    {"WEB", 134},
    {"APP_KLOG_TRX", 137},
    {"APP_KLOG_LOGN", 138},
    {"APP_N1MM_POINTS", 139}
    };
}

bool FileManager::getStationCallsignFromUser(const QString &_qrzDX, const QDate &_dt)
{
    //qDebug() << "FileManager::getStationCallsignFromUser: " << _qrzDX << "/" << util->getDateSQLiteStringFromDate(_dt) << "-";
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
       //qDebug() << "FileManager::queryPreparation log: " << QString::number(_logN);
    //Prepares the query, ALL fields to default except lognumber, as may change...

    QStringList columns;
    columns.clear();

    columns << dataProxy->getColumnNamesFromTableLog();
    if (columns.size()<2)
    {
           //qDebug() << "FileManager::queryPreparation: <2 " ;
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
   //qDebug() << "FileManager::adifReqQSLExport" << _fileName;
    return adifLogExportToFile(_fileName, 0, false, true, false);
}
int FileManager::howManyQSOsInFile (QFile & _f)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << _f.fileName ();
    QFile &file = _f;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //qDebug() << "FileManager::howManyLogsInFile File not found";
        return -1;
    }
    QString line;
    int qsos = 0;
    while ( !file.atEnd()   )
    {
        line.clear();
        line.append(file.readLine().trimmed().toUpper());

        if (line.contains("APP_KLOG_QSOS"))
        {
            QStringList fields;
            fields.clear();
            fields << line.split("<", QT_SKIP);
            QString aux = QString();
            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if (aux.contains("APP_KLOG_QSOS"))
                {
                    QStringList data = QStringList();
                    data << aux.split('>');
                    file.close ();
                    //qDebug() << Q_FUNC_INFO << " - END-1";
                    return ((data.at(1)).toInt());
                }
            }
        }
        else if (line.contains("EOR"))
        {
            qsos++;
        }
    }

    file.close();
    //qDebug() << Q_FUNC_INFO << " - END";
    return qsos;
}

int FileManager::howManyLogsInFile(QFile& _f)
{
      //qDebug() << "FileManager::howManyLogsInFile:";
    QFile &file = _f;
    //qint64 pos; //Position in the file
    QString line = QString();
    QStringList fields = QStringList();
    QStringList logs = QStringList();
    QString aux = QString();
    QStringList data = QStringList();
    bool atLeastOneLog = false;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
           //qDebug() << "FileManager::howManyLogsInFile File not found";
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
            fields << line.split("<", QT_SKIP);

            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if (aux.contains("APP_KLOG_LOGN"))
                {
                    data.clear();
                    data << aux.split('>');
                      //qDebug() << "FileManager::howManyLogsInFile: data.0: " << data.at(0);
                      //qDebug() << "FileManager::howManyLogsInFile: data.1: " << data.at(1);
                    if (!logs.contains(data.at(1)))
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
    }
       //qDebug() << "FileManager::howManyLogsInFile: JUST ONE!" << aux;
    file.close();

    if ( (logs.size()<2) && atLeastOneLog )
    {
        return 1;
    }
    return logs.size();
}


bool FileManager::fillHashLog(QFile &_f)
{
       //qDebug() << Q_FUNC_INFO << " - Start" ;
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
           //qDebug() <<  log: " << QString::number(qs.at(i).toInt());
        hashLogs.insert(i+1, (qs.at(i)).toInt() );
    }

    for (i = 0; i < hashLogs.size(); ++i)
    {
           //qDebug() << Q_FUNC_INFO << " -  hashLogs: " << QString::number(hashLogs[i]);
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
           //qDebug() << Q_FUNC_INFO << QString::number(hashLogs.value(i)) << "/" << QString::number(i) << ");
    }
    return true;
}


QStringList FileManager::getListOfLogsInFile(QFile& _f)
{
    // Returns the logs of the file + 10.000 (So, log 1 is returned as 10001, 2 as 10.002 and so on)
       //qDebug() << "FileManager::getListOfLogsInFile:";
    QFile &file = _f;
    //qint64 pos; //Position in the file
    QString line = QString();
    QStringList fields = QStringList();
    QStringList logs = QStringList();
    QString aux = QString();
    QStringList data = QStringList();
    //bool atLeastOneLog = false;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
           //qDebug() << "FileManager::howManyLogsInFile File not found";
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
            fields << line.split("<", QT_SKIP);

            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if (aux.contains("APP_KLOG_LOGN"))
                {
                    data.clear();
                    data << aux.split('>');
                       //qDebug() << "FileManager::howManyLogsInFile: data.0: " << data.at(0);
                       //qDebug() << "FileManager::howManyLogsInFile: data.1: " << data.at(1);
                    if (!logs.contains(QString::number(data.at(1).toInt()+10000)))
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

    //qDebug() << "FileManager::readAdifField: " << _field;
    QStringList result;
    result.clear();

    if (_field == "<EOR>")
    {
        //qDebug() << "FileManager::readAdifField: EOR found!!";
        result << "EOR" << "EOR";
        return result;
    }

    if (!((_field.startsWith("<")) && (_field.contains(":")) && (_field.contains(">"))))
    {
        //qDebug() << "FileManager::readAdifField: NOT (contains : and >): " << _field;
        return QStringList();
    }

    // Now we have the data in the result[1]
    result = _field.split(">", QT_SKIP); // Remove the first '<' and split in field & data
    QString data = result.at(1);
    QStringList fieldList;
    fieldList.clear();
    fieldList << (result.at(0)).split(':'); // we may have 1 or 2 depending on the format of the ADIF field.
    int iAux = fieldList.length();
      //qDebug() << "FileManager::readAdifField: iAux: " << QString::number(iAux);
    int dataLength;
    QString field;
    QString fieldType;
      //qDebug() << "FileManager::readAdifField: analyzing..." ;

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
         //qDebug() << "FileManager::readAdifField: iAux != 1, 2";
        return QStringList();
    }
    field.remove('<');
    data = data.left(dataLength);
      //qDebug() << "FileManager::readAdifField: field: " << field;
      //qDebug() << "FileManager::readAdifField: dataLength: " << QString::number(dataLength);
      //qDebug() << "FileManager::readAdifField: data: " << data;
      //qDebug() << "FileManager::readAdifField: fieldType: " << fieldType;

    if (data.length() != dataLength)
    {
        //qDebug() << "FileManager::readAdifField: data.length != dataLength: " << QString::number(data.length()) << "/" << QString::number(dataLength);
        return QStringList();
    }
    result.clear();
    result << field << data;
    //qDebug() << "FileManager::readAdifField: OK: " << field << "/" << data;
    return result;
}

QDateTime FileManager::getDateTimeOfLastBackup()
{
      //qDebug() << "FileManager::getDateTimeOfLastBackup: " << (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmss")<< QT_ENDL;
      //qDebug() << "FileManager::getDateTimeOfLastBackup: " << util->getCfgFile();
    QFile file (util->getCfgFile());
    QString line;
    QStringList fields;
    fields.clear();
    QDateTime _dataTime = QDateTime();

    if (file.open (QIODevice::ReadOnly)) /* Flawfinder: ignore */
    {
        while ( !file.atEnd()   )
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            if (line.contains("LATESTBACKUP"))
            {
                fields << line.split("=", QT_SKIP);
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
      //qDebug() << "FileManager::writeBackupDate: current: " << (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmss");
      //qDebug() << "FileManager::writeBackupDate: current: " << (getDateTimeOfLastBackup()).toString("yyyyMMdd-hhmmss");

    QFile file (util->getCfgFile());
    QString line, lineTemp;
    //QStringList fields;
    //fields.clear();
    QDateTime _dataTime = QDateTime();

    QStringList completeFile;
    completeFile.clear();

    if(file.open(QIODevice::ReadWrite | QIODevice::Text)) /* Flawfinder: ignore */
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
      //qDebug() << "FileManager::setStationCallSign: " << _st;
    if (util->isValidCall(_st))
    {
          //qDebug() << "FileManager::setStationCallSign: True";
        defaultStationCallsign = _st;
          //qDebug() << "FileManager::setStationCallSign: " << defaultStationCallsign;
    }
    else
    {
          //qDebug() << "FileManager::setStationCallSign: FALSE";
    }

   //qDebug() << "FileManager::setStationCallSign: -" << defaultStationCallsign << "-END";
}


bool FileManager::askUserToUseAlwaysSameAnswer()
{
    //qDebug() << "FileManager::askUserToUseAlwaysSameAnswer: " ;

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

    //qDebug() << "FileManager::askUserToUseAlwaysSameAnswer: - END";
}

bool FileManager::askUserToAddThisQSOToLog(const QString &_call, const QDateTime _datetime, const QString &_mode, const QString &_band, const double _freq)
{
    Q_UNUSED(_freq);
    //qDebug() << "FileManager::askUserToAddThisQSOToLog: " << _call;
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

    //qDebug() << "FileManager::askUserToAddThisQSOToLog: - END";
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
        out << "ADIF v3.1.0 Export from KLog\n<PROGRAMID:4>KLog ";
    }
    else
    {
        out << "ADIF v3.1.0 Export from KLog\nhttps://www.klog.xyz/klog\n<PROGRAMVERSION:" << QString::number(klogVersion.length()) << ">" << klogVersion << "\n<PROGRAMID:4>KLOG ";
        out << "<APP_KLOG_QSOS:" << QString::number((QString::number(_numberOfQsos)).length()) << ">" << QString::number(_numberOfQsos);
        out << "<APP_KLOG_LOG_DATE_EXPORT:" << QString::number((QDateTime::currentDateTime().toString("yyyyMMdd-hhmm")).length()) << ">" << QDateTime::currentDateTime().toString("yyyyMMdd-hhmm");
    }
    out << "<EOH>\n";
}

void FileManager::writeQuery(QSqlRecord &rec, QTextStream &out, const ExportMode _em, const bool _justMarked, const bool _onlyRequested, const int _logN )
{
    out << dataProxy->getADIFFromQSOQuery(rec, _em, _justMarked, _onlyRequested, _logN);
}
