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

    QSqlRecord rec = query.record();
    int nameCol;

    //qDebug() << Q_FUNC_INFO << ":  Entering the While..." ;

    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << Q_FUNC_INFO << ":  Start of While" ;
        if (query.isValid())
        {
            nameCol = rec.indexOf("id");
            qsos.append((query.value(nameCol)).toInt());
            writeQuery(query, out, _em, false, false, _logN); // JustMarked = false, onlyRequested = false

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
    while ( (query.next()) && (!noMoreQso) )
    {
       //qDebug() << "FileManager::adifLogExportToFile -  Just in the While";
        if (query.isValid())
        {
            //qDebug() << "FileManager::adifLogExportToFile -  Query is Valid";
             writeQuery(query, out, ModeADIF, false, false, -1);
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
    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << Q_FUNC_INFO << " -  Just in the While";
        if (query.isValid())
        {
            //qDebug() << Q_FUNC_INFO << " -  Query is Valid";
            writeQuery(query, out, ModeADIF, false, false, -1);
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

    QSqlRecord rec = query.record();
    QDateTime date;
    //qDebug() << Q_FUNC_INFO << " - 50";

    while ( (query.next()) && (!noMoreQso) )
    {
        //qDebug() << Q_FUNC_INFO << " - 50.1";

        if (query.isValid())
        {
            writeQuery(query, out, ModeADIF, exportJustMarkedQSO, exportOnlyQSLRequested, _logN);
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
    progress.setWindowModality(Qt::WindowModal);
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

QList<int> FileManager::adifLoTWReadLog(const QString& tfileName, const int logN)
{
   //qDebug() << Q_FUNC_INFO << tfileName;
    QString fileName = tfileName;
    QList<int> readed;
    //QElapsedTimer time1 = QTime::currentTime();

    readed.clear();
    if (!dataProxy->doesThisLogExist(logN))
    {
       //qDebug() << Q_FUNC_INFO << " - ERROR: The log does not exist: " << QString::number(logN);
        return readed;
    }
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

   //qDebug() << Q_FUNC_INFO << " - QSOs found: " << QString::number(numberOfQsos);
   //qDebug() << Q_FUNC_INFO << " - STEP: " << QString::number(step);

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

   //qDebug() << Q_FUNC_INFO << " - Going to read the HEADER";
    //Read HEADER
    line = file.readLine().trimmed().toUpper();
   //qDebug() << Q_FUNC_INFO << line;

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
   //qDebug() << Q_FUNC_INFO << " - QSO data reading started..." ;
    QDate _tdate;
    noMoreQso = false;
    //time1.start();
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
           //qDebug() << Q_FUNC_INFO << " -line:" << line;
            fields << line.split("<", QT_SKIP);


            qsToPass.clear();
            auxString.clear();
            foreach (aux, fields)
            {
                aux = aux.trimmed();
                if ( (aux.contains('>')) && (auxString.length() > 0) )
                {
                    qsToPass.last() = qsToPass.last() + auxString;
                       //qDebug() << Q_FUNC_INFO << " - Modified in qsToPass: " << qsToPass.last();
                    //qsToPass << aux.trimmed();
                   //qDebug() << Q_FUNC_INFO << " - Added to qsToPass: " << aux.trimmed();
                    auxString.clear();
                }
                else if (( aux.contains('>')) && (auxString.length() <= 0) )
                {
                    qsToPass << aux.trimmed();
                }
                else
                {
                    auxString = auxString + "-" + aux.trimmed();
                   //qDebug() << Q_FUNC_INFO << " - auxString: " << auxString;
                }
               //qDebug() << Q_FUNC_INFO << " - fields: " << aux;
            }

            if (auxString.length()>0)
            {
                   //qDebug() << Q_FUNC_INFO << " - auxString2: " << auxString;
                qsToPass.last() = qsToPass.last() + auxString.trimmed();
            }
             //qDebug() << Q_FUNC_INFO << " - END fields";
               //qDebug() << Q_FUNC_INFO << " - Mod: " << qsToPass.join("/");
             //qDebug() << Q_FUNC_INFO << " - END2 fields";

            fields = qsToPass;

            if (fields.contains("EOR>")) // We are going to add a QSO to the log... prepare the Query!
            {
               //qDebug() << Q_FUNC_INFO << " - START of QSO adding"<< QT_ENDL;
                //dataProxy->isThisQSODuplicated()

                //int getDuplicatedQSOId(const QString &_qrz, const QString &_date, const QString &_time, const int _band, const int _mode);
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
                  //qDebug() << Q_FUNC_INFO << " - clearAdif length: " << QString::number(clearAdif.length());
                    if (clearAdif.length()==2)
                    {
                        validQSO = true;
                        field = clearAdif.at(0);
                        data = clearAdif.at(1);

                      //qDebug() << Q_FUNC_INFO << " - field: " << field;
                      //qDebug() << Q_FUNC_INFO << " - data: " << data;
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
                           //qDebug() << Q_FUNC_INFO << " - field: " << field;
                           //qDebug() << Q_FUNC_INFO << " - data: " << data;

                             _tdate = util->getDateFromADIFDateString(data);
                             if (_tdate.isValid())
                             {
                                 _dateTime.setDate(_tdate);
                             }
                        }
                        else if (field == "TIME_ON")
                        {
                            //qDebug() << Q_FUNC_INFO << " - field: " << field;
                            //qDebug() << Q_FUNC_INFO << " - data: " << data;
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
                       //qDebug() << Q_FUNC_INFO << " - NOT VALID ADIF RECEIVED: " << "<" + str;
                    }
                }
               //qDebug() << Q_FUNC_INFO << " - If QSO is valid, we will call the addQSOToList" << "<" + str;
              //qDebug() << Q_FUNC_INFO << " - New Add String query: ";
               //dataProxy->addQSO(&qso);
                if (validQSO)
                {
                    modifiedQSO = -2; //
                    if (qsl_rcvd)
                    {
                         modifiedQSO = dataProxy->lotwUpdateQSLReception (_call, _dateTime, _band, _mode, _qslrdate);
                    }
                   //qDebug() << Q_FUNC_INFO << " - QSO Modified:  " << QString::number(modifiedQSO);
                    if (modifiedQSO>0)
                    {
                      //qDebug() << Q_FUNC_INFO << " - QSO Modified:  " << _call;
                       readed.append(modifiedQSO);
                       modifiedQSOList << _call << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << _band << _mode;// << util->getDateSQLiteStringFromDate(_qslrdate);
                       emit addQSOToList(modifiedQSOList);
                       modifiedQSOList.clear();
                    }
                    else if ((modifiedQSO == -1 ) && (!qsl_rcvd))
                    {
                        bool ignoreQSO = false;
                      //qDebug() << Q_FUNC_INFO << " - QSO NOT found but confirmed by LoTW - Is it an error or should I add it to the log? " << _call;
                        if (alwaysAdd)
                        {
                           //qDebug() << Q_FUNC_INFO << " - ADD THE QSO AUTOMATICALLY!!!" ;
                            modifiedQSO = dataProxy->addQSOFromLoTW(_call, _dateTime, _mode, _band, _freq, _qslrdate, stationCallsign, logN);
                            if (modifiedQSO>0)
                            {
                                readed.append(modifiedQSO);
                              //qDebug() << Q_FUNC_INFO << " - QSO ADDED readed: " << QString::number(readed.length());
                                modifiedQSOList << _call << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << _band << _mode;// << util->getDateSQLiteStringFromDate(_qslrdate);
                                emit addQSOToList(modifiedQSOList);
                                modifiedQSOList.clear();
                              //qDebug() << Q_FUNC_INFO << " - QSO ADDED: " << QString::number(modifiedQSO);
                            }
                            else
                            {
                              //qDebug() << Q_FUNC_INFO << " - QSO NOT ADDED: " << QString::number(modifiedQSO);
                            }
                        }
                        else if (alwaysIgnore)
                        {
                          //qDebug() << Q_FUNC_INFO << " - IGNORE THE QSO AUTOMATICALLY!!!" ;
                        }
                        else
                        {
                            //if (askUserToAddThisQSOToLog(_call, _dateTime, _mode, _band, _freq, _qslrdate))
                            if (askUserToAddThisQSOToLog(_call, _dateTime, _mode, _band, _freq))
                            {
                              //qDebug() << Q_FUNC_INFO << " - ADD THE QSO !!!" ;
                                modifiedQSO = dataProxy->addQSOFromLoTW(_call, _dateTime, _mode, _band, _freq, _qslrdate, stationCallsign, logN);
                                if (modifiedQSO>0)
                                {
                                    readed.append(modifiedQSO);
                                  //qDebug() << Q_FUNC_INFO << " - QSO ADDED-2 readed: " << QString::number(readed.length());
                                    modifiedQSOList << _call << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << _band << _mode;// << util->getDateSQLiteStringFromDate(_qslrdate);
                                    emit addQSOToList(modifiedQSOList);
                                    modifiedQSOList.clear();
                                  //qDebug() << Q_FUNC_INFO << " - QSO ADDED-2: " << QString::number(modifiedQSO);
                                }
                                else
                                {
                                  //qDebug() << Q_FUNC_INFO << " - QSO NOT ADDED-2: " << QString::number(modifiedQSO);
                                }
                            }
                            else
                            {
                              //qDebug() << Q_FUNC_INFO << " - IGNORE THE QSO !!!" ;
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
                     //qDebug() << Q_FUNC_INFO << " - QSO NOT Modified:  Error: " << QString::number(modifiedQSO) << " - " << _call;
                    }
                }
                _call.clear();
                _date.clear();
                _time.clear();
                _band.clear();
                _mode.clear();
                _qslrdate = QDate::currentDate();

              //qDebug() << Q_FUNC_INFO << " - END of QSO "<< QT_ENDL;
                fields.clear();
                i++;
            } // END of  if (fields.contains("EOR>")) // We are going to add a QSO to the log... !
            else
            {
                if (file.atEnd())
                {
                    noMoreQso = true;
                }
                 //qDebug() << Q_FUNC_INFO << " - fields DOES NOT contains EOR>";
            }
        if (( (i % step ) == 0) )
        { // To update the speed I will only show the progress once each X QSOs
            //qDebug() << Q_FUNC_INFO << " - MOD 0 - i = " << QString::number(i) ;
            aux = tr("Importing LoTW ADIF file...") + "\n" + tr(" QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);
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
  //qDebug() << Q_FUNC_INFO << " - - END: " << QString::number(readed.length());
    return readed;
}

bool FileManager::adifReadLog2(const QString& tfileName, const int logN)
{
    qDebug() << Q_FUNC_INFO << " - Start: " << tfileName << "/" << QString::number(logN);
    QFile file( tfileName );
    if (!file.exists ())
    {
        qDebug() << Q_FUNC_INFO << " - END: file does not exist";
        return false;
    }
    int qsos = howManyQSOsInFile (file);
    qDebug() << Q_FUNC_INFO << " - QSOs: " << QString::number(qsos);
    qint64 pos = passHeader (file); // Position in the file to calculate where the header ends
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        qDebug() << Q_FUNC_INFO << "  File not found" ;
        return false;
    }

    file.seek (pos); // QSO Data starts here

    QSO qso;
    QStringList fields; // fields keeps the running array,

    fields.clear ();
    QString line = QString();
    //line.clear();
    qso.clear ();

    qDebug() << Q_FUNC_INFO << ": Progress defined" ;
    QProgressDialog progress(tr("Writing ADIF file..."), tr("Abort writing"), 0, qsos, this);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    //progress.setWindowTitle(tr("Import"));
    progress.setAutoClose(true);
    int step = util->getProgresStepForDialog(qsos);
    int i = 0;
    bool noMoreQSO = false;
    qDebug() << Q_FUNC_INFO << ": We start the while" ;
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
        qDebug() << Q_FUNC_INFO << ": Reading the line: " << line ;
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
                qDebug() << Q_FUNC_INFO << QString(": Progress Cancelled") ;
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
        qDebug() << Q_FUNC_INFO << QString(": List of fields is empty!") ;
    }
    qDebug() << Q_FUNC_INFO << QString(": End of File or no more QSOs") ;
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

bool FileManager::adifReadLog(const QString& tfileName, const int logN)
{
    //qDebug() << Q_FUNC_INFO << " " << tfileName;
    //QElapsedTimer time1;

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
    //int errorCode = -1;
    //int qsosInTransaction = 0;
    bool ignoreErrorCode19 = false;

    QFile file( fileName );

    //bool moreThanOneLog = adifCheckMoreThanOneLog(file);
    int howManyLogs = howManyLogsInFile(file);
    fillHashLog(file);
    //I am creating several logs when importing a file
    //We need to fill the hashLog to process then in processLog

    //bool keepLogsInFile = false;

      //qDebug() << Q_FUNC_INFO << " Logs: " << QString::number(howManyLogs);

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
                   //qDebug() << Q_FUNC_INFO << " clicked YES";
                //keepLogsInFile = true;
              break;
          case QMessageBox::No:
                // No Save was clicked
                   //qDebug() << Q_FUNC_INFO << " clicked NO";
                 //keepLogsInFile = false;
                 return false;
          default:
                // should never be reached
                //keepLogsInFile = false;
                return false;
                   //qDebug() << Q_FUNC_INFO << " default";
        }
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
          //qDebug() << Q_FUNC_INFO << "  File not found" << fileName;
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
       //qDebug() << Q_FUNC_INFO << "  QSOs found: " << QString::number(numberOfQsos);

    QProgressDialog progress(tr("Reading ADIF file..."), tr("Abort reading"), 0, numberOfQsos, this);
    /*progress.setWindowModality(Qt::WindowModal);*/
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setVisible(true);
    progress.setValue(0);
    progress.setMaximum(numberOfQsos);

    step = util->getProgresStepForDialog(numberOfQsos);

    //step = getProgresStepForDialog(numberOfQsos);
       //qDebug() << Q_FUNC_INFO << "  (STEP)/Number: " << QString::number(step) << "/" << QString::number(numberOfQsos);
       //qDebug() << Q_FUNC_INFO << "  (number & step: " << QString::number(numberOfQsos % step);

    file.seek(pos);

    /*
    Optional header information may be included before the actual data in the file.
    To include optional header info, the first character of the file must be something other than <.
    Any amount of header info of any value except <eoh> may be included. The header info must be
    terminated with <eoh>. Any number of characters of any value except < may follow <eoh>.
    The first < after <eoh> is the start of the first field of the first data record in the file.
    */

       //qDebug() << Q_FUNC_INFO << " Going to read the HEADER";
    //Read HEADER
    line = file.readLine().trimmed().toUpper();
       //qDebug() << Q_FUNC_INFO << " " << line;

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
      //qDebug() << Q_FUNC_INFO << " QSO data reading started..." ;

    preparedQuery.prepare( "INSERT INTO log (call, qso_date, bandid, modeid, srx, stx, srx_string, stx_string, qso_date_off, band_rx, rst_sent, rst_rcvd, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, eq_call, email, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, my_gridsquare, iota, iota_island_id, my_iota, my_iota_island_id, k_index, lat, lon, my_lat, my_lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, clublog_qso_upload_date, clublog_qso_upload_status, max_bursts, ms_shower, my_antenna, my_city, my_cnty, my_country, my_cq_zone, my_name, name, operator, station_callsign, owner_callsign, my_rig, my_sig, my_sig_info, my_sota_ref, my_state, state, my_street, my_vucc_grids, notes, nr_bursts, nr_pings, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, tx_pwr, sat_mode, sat_name, sfi, sig, sota_ref, swl, ten_ten, vucc_grids, web, lognumber) VALUES (:call, :qso_date, :bandid, :modeid, :srx, :stx, :srx_string, :stx_string, :qso_date_off, :band_rx, :rst_sent, :rst_rcvd, :cqz, :ituz, :dxcc, :address, :age, :cnty, :comment, :a_index, :ant_az, :ant_el, :ant_path, :arrl_sect, :checkcontest, :class, :contacted_op, :contest_id, :country, :credit_submitted, :credit_granted, :distance, :eq_call, :email, :eqsl_qslrdate, :eqsl_qslsdate, :eqsl_qsl_rcvd, :eqsl_qsl_sent, :force_init, :freq, :freq_rx, :gridsquare, :my_gridsquare, :iota, :iota_island_id, :my_iota, :my_iota_island_id, :k_index, :lat, :lon, :my_lat, :my_lon, :lotw_qslrdate, :lotw_qslsdate, :lotw_qsl_rcvd, :lotw_qsl_sent, :clublog_qso_upload_date, :clublog_qso_upload_status, :max_bursts, :ms_shower, :my_antenna, :my_city, :my_cnty, :my_country, :my_cq_zone, :my_name, :name, :operator, :station_callsign, :owner_callsign, :my_rig, :my_sig, :my_sig_info, :my_sota_ref, :my_state, :state, :my_street, :my_vucc_grids, :notes, :nr_bursts, :nr_pings, :pfx, :precedence, :prop_mode, :public_key, :qslmsg, :qslrdate, :qslsdate, :qsl_rcvd, :qsl_sent, :qsl_rcvd_via, :qsl_sent_via, :qsl_via, :qso_complete, :qso_random, :qth, :rx_pwr, :tx_pwr, :sat_mode, :sat_name, :sfi, :sig, :sota_ref, :swl, :ten_ten, :vucc_grids, :web, :lognumber)" );

    //file.seek(pos);
    fields.clear();
   // while ( (!file.atEnd() ) && (!noMoreQso) && (sqlOK))
    //time1.start();
    while ((!noMoreQso) && (sqlOK))
    {
        if (!file.atEnd())
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            //line.append(file.readLine().toUpper()); // TODO: Check if we should remove extra spaces,tabs and so on...
            //qDebug() << Q_FUNC_INFO << " -line:" << line;
            //fields.clear(); //TODO: Check if I should clear fields... I think I should not because I could loose data if a line contains data after an <EOR>
             fields << line.split("<", QT_SKIP);
        }

        //TODO: Check what happens

        //qDebug() << Q_FUNC_INFO << "  START fields";
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
                  //qDebug() << Q_FUNC_INFO << "  Modified in qsToPass: " << qsToPass.last();
                qsToPass << aux.trimmed();
                  //qDebug() << Q_FUNC_INFO << "  Added to qsToPass: " << aux.trimmed();
                auxString.clear();
            }
            else if (( aux.contains('>')) && (auxString.length() <= 0) )
            {
                qsToPass << aux.trimmed();
            }
            else
            {
                auxString = auxString + "-" + aux.trimmed();
                  //qDebug() << Q_FUNC_INFO << "  auxString: " << auxString;
            }
              //qDebug() << Q_FUNC_INFO << "  fields: " << aux;
        }

          //qDebug() << Q_FUNC_INFO << " -W-1";

        if (auxString.length()>0)
        {
              //qDebug() << Q_FUNC_INFO << "  auxString2: " << auxString;
            qsToPass.last() = qsToPass.last() + auxString.trimmed();
        }

           //qDebug() << Q_FUNC_INFO << "  END fields";
           //qDebug() << Q_FUNC_INFO << "  Mod: " << qsToPass.join("/");
           //qDebug() << Q_FUNC_INFO << "  END2 fields";

        fields = qsToPass;

        if (fields.contains("EOR>")) // We are going to add a QSO to the log... prepare the Query!
        {
               //qDebug() << Q_FUNC_INFO << " fields contains EOR>";
            preparedQuery.bindValue( ":lognumber", logN);

            while ( (!EOR) && (!fields.isEmpty()) )
            {
                   //qDebug() << Q_FUNC_INFO << " -W-2";
                fieldToAnalyze = (fields.takeFirst()).trimmed();

                if ( fieldToAnalyze.contains("EOR>") )
                {
                        //qDebug() << Q_FUNC_INFO << " -W-2.1";
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
                           //qDebug() << Q_FUNC_INFO << " isDupeQSO = true" ;
                    }
                }
                else
                {
                       //qDebug() << Q_FUNC_INFO << " Not contains EOR" ;

                    if ((!fieldToAnalyze.contains('>')) && (currentQSOfields.length()>0))
                    {
                          //qDebug() << Q_FUNC_INFO << " Contains > & currentsQSOfields.length>0" ;
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
                //qDebug() << Q_FUNC_INFO << " DUPE QSO, not adding it";
            }

            //qDebug() << Q_FUNC_INFO << " executedQuery1: " << preparedQuery.executedQuery() ;
            //qDebug() << Q_FUNC_INFO << " executedQuery2: " << preparedQuery.executedQuery() ;
            //qDebug() << Q_FUNC_INFO << " LastQuery2: " << preparedQuery.lastQuery() ;


            if (( (i % step ) == 0) )
            { // To update the speed I will only show the progress once each X QSOs
                  //qDebug() << Q_FUNC_INFO << " MOD 0 - i = " << QString::number(i) ;
                //aux = QString(tr("Importing LoTW ADIF file...\n QSO: %1/%2\nImporting speed: 0 QSOs/sec")).arg(i).arg(numberOfQsos);

                aux = tr("Importing ADIF file...") + "\n" + tr(" QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);

               progress.setLabelText(aux);
               progress.setValue(i);
            }
            else
            {
                  //qDebug() << Q_FUNC_INFO << " Mod: "<< QString::number(i) << " mod " << QString::number(step) << " = " << QString::number(i % step);
            }

            if (sqlOK)
            {
                //qDebug() << Q_FUNC_INFO << " (1) in While sqlOK (QSO added) = TRUE" ;
            }
            else
            {
                //errorCode = preparedQuery.lastError().nativeErrorCode();
                //qDebug() << Q_FUNC_INFO << " QSO DUPE";

                //qDebug() << Q_FUNC_INFO << " (1) LastQuery: " << preparedQuery.lastQuery() ;
                //qDebug() << Q_FUNC_INFO << " (1) LastError-data: " << preparedQuery.lastError().databaseText() ;
                //qDebug() << Q_FUNC_INFO << " (1) LastError-driver: " << preparedQuery.lastError().driverText() ;
                //qDebug() << Q_FUNC_INFO << " (1) LastError-n: " << QString::number(preparedQuery.lastError().nativeErrorCode() );
                if (((preparedQuery.lastError().nativeErrorCode()).toInt() == 19) && (!ignoreErrorCode19))
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
                               //qDebug() << Q_FUNC_INFO << " (1) clicked YES";
                            sqlOK = true;
                          break;
                    case QMessageBox::YesToAll:
                        // Yes was clicked
                               //qDebug() << Q_FUNC_INFO << " (1) clicked YES to ALL";
                            ignoreErrorCode19 = true;
                            sqlOK = true;
                        break;
                      case QMessageBox::No:
                          // No Save was clicked
                               //qDebug() << Q_FUNC_INFO << " (1) clicked NO";
                             sqlOK = false;
                          break;
                      default:
                          // should never be reached
                            sqlOK = true;
                               //qDebug() << Q_FUNC_INFO << " (1) default";
                          break;
                    }
                    //;
                }
                else if(((preparedQuery.lastError().nativeErrorCode()).toInt() == 19) && (ignoreErrorCode19))
                {
                  sqlOK = true;
                    //qDebug() << Q_FUNC_INFO << " errorCode=19 && ignoreErrorCode19";
                }
                else
                {
                       //qDebug() << Q_FUNC_INFO << " (2) LastQuery: " << preparedQuery.lastQuery() ;
                       //qDebug() << Q_FUNC_INFO << " (2) LastError-data: " << preparedQuery.lastError().databaseText() ;
                       //qDebug() << Q_FUNC_INFO << " (2) LastError-driver: " << preparedQuery.lastError().driverText() ;
                       //qDebug() << Q_FUNC_INFO << " (2) LastError-n: " << QString::number(preparedQuery.lastError().nativeErrorCode() );

                    emit queryError( Q_FUNC_INFO, preparedQuery.lastError().databaseText(), preparedQuery.lastError().nativeErrorCode(), preparedQuery.lastQuery());
                    noMoreQso = true;
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
               //qDebug() << Q_FUNC_INFO << " fields DOES NOT contains EOR>";
        }
        //TODO: Check how to stop the importing process
    } // END OF WHILE

    if (noMoreQso)
    {
           //qDebug() << Q_FUNC_INFO << " noMoreQso = true";
        progress.setValue(numberOfQsos);
    }
    else
    {
           //qDebug() << Q_FUNC_INFO << " noMoreQso = false";
    }
    if (sqlOK)
    {
           //qDebug() << Q_FUNC_INFO << " sqlOK = true";
    }
    else
    {
           //qDebug() << Q_FUNC_INFO << " sqlOK = false";
    }

    if (sqlOK)
    {
        /*
        if (db.commit())
        {
               //qDebug() << Q_FUNC_INFO << " Last commit OK" ;
        }
        else
        {
            errorCode = preparedQuery.lastError().nativeErrorCode();
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
                       //qDebug() << Q_FUNC_INFO << " I have just set sqlOK=False (5)";
                  break;
              default:
                  // should never be reached
                    sqlOK = false;
                       //qDebug() << Q_FUNC_INFO << " I have just set sqlOK=False (6)";
                  break;
            }

            if (db.rollback())
            {}
            else
            {
              //TODO: Check the error if db.rollback returns false
            }
        }
        */
    }
    else
    {
           //qDebug() << Q_FUNC_INFO << " sqlOK = NOK" ;
    }
    progress.setValue(numberOfQsos);
       //qDebug() << Q_FUNC_INFO << "  END " ;
    return true;
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

bool FileManager::processQsoReadingADIF(const QStringList &_line, const int logNumber) //, const bool _keepLogsInFile)
//bool FileManager::processQsoReadingADIF(const QStringList _line, const int logNumber, const bool _keepLogsInFile, QHash<int, int> &_logs)
{
       //qDebug() << "FileManager::processQsoReadingADIF: log: " << QString::number(logNumber);
       //qDebug() << "FileManager::processQsoReadingADIF: log: " << _line.at(0);
       //qDebug() << "FileManager::processQsoReadingADIF: " << _line.join("/");

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

    if (SwitchHash.empty()) initializeSwitchHash();

    //confirmed = 0; // 0 means worked, 1 means confirmed

    //QString queryString, stringFields, stringData;

    QString aux; // Aux string
    QString aux2;
    QString qrzCall = QString();
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
    //QString wrongCall = QString();
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

       //qDebug() << "FileManager::processQsoReadingADIF" << QString::number(qs.size()) << "/" << QString::number(logNumber);
    //TODO: To remove the next line, it was just to measure the time it takes.
    ignoreUnknownAlways = true;
    QString str;
    //preparedQuery.bindValue( ":confirmed", '0' );

       //qDebug() << "FileManager::processQsoReadingADIF: Entering the foreach";
    foreach (str, qs)
    {
           //qDebug() << "FileManager::processQsoReadingADIF: " << str;
        if ( !( (str.contains(":")) && (str.contains(">")) ) )
        {
               //qDebug() << "FileManager::processQsoReadingADIF: NOT (contains : and >): " << str;
        }
        else
        {
               //qDebug() << "FileManager::processQsoReadingADIF: (contains : and >): " << str;

            oneField = str.split(">", QT_SKIP);

           //qDebug() << "FileManager::processQsoReadingADIF: (oneField)" << oneField;
            if (checkADIFValidFormat(oneField))
            {
                i = (qs.at(0)).count(":");
                field = (oneField.at(0)).trimmed(); // Needs to be cleared FIELD:4:D
                data = (oneField.at(1)).trimmed();
                data = util->checkAndFixASCIIinADIF(data);
                //qDebug() << "FileManager::processQsoReadingADIF: field/data" << field << "/" << data;

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
                    //qDebug() << "FileManager::checkADIFValidFormat-1 ";
                    //return false;
                }

                //field = oneField.at(0).trimmed();
                //data = oneField.at(1).trimmed();

                //length = field.indexOf(":");
                //field = field.left(length);
                //qDebug() << "FileManager::processQsoReadingADIF (field/data): " << field << "/" << data;
                if (SwitchHash.find(field) != SwitchHash.end()) {
                    switch (SwitchHash[field]) {
                    case(1):
                        //qDebug() << "FileManager::processQsoReadingADIF-CALL:" << data;
                        qrzCall = data;
                        haveCall = util->isValidCall(qrzCall);
                        if (haveCall)
                        {
                            //qDebug() << "FileManager::processQsoReadingADIF-CALL: Have CALL!!" ;
                            preparedQuery.bindValue(":call", qrzCall);
                        }
                        //qDebug() << "FileManager::processQsoReadingADIF-CALL-END:" << data;
                        break;
                    case(2):
                        //qDebug() << "FileManager::processQsoReadingADIF-QSO_DATE:" << data;
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            dateTime.setDate(dateT);
                            haveDate = true;
                        }
                        else {
                            //qDebug() << "FileManager::processQsoReadingADIF QSO_DATE is NOT VALID: " << data;
                        }
                        break;
                    case(3):

                        //preparedQuery.bindValue( ":bandid", data );
                        i = dataProxy->getIdFromBandName(data);
                        //i = db->getBandIDFromName2(data);
                        if (i >= 0)
                        {
                            preparedQuery.bindValue(":bandid", QString::number(i));
                            haveBand = true;
                            bandi = i;

                            //qDebug() << "FileManager::processQsoReadingADIF-Band: " << data << "/"  << QString::number(i);
                        }
                        else
                        {
                            //qDebug() << "FileManager::processQsoReadingADIF-Band - Wrong band: " << data << "/"  << QString::number(i);
                        }
                        /*
                         queryString = QString("SELECT id FROM band WHERE name ='%1'").arg(data);
                         query.exec(queryString);
                         query.next();
                         if (query.isValid())
                         {
                             preparedQuery.bindValue( ":bandid", query.value(0).toInt() );

                                //qDebug() << "FileManager::bprocessQsoReadingADIF-Band: " << data;
                         }
                         */
                        break;
                    case(4):
                        modei = dataProxy->getSubModeIdFromSubMode(data);
                        // get modeid
                        if (modei >= 0)
                        {
                            {
                                if (!haveSubMode)
                                {
                                    preparedQuery.bindValue(":modeid", QString::number(modei));
                                    haveMode = true;
                                    haveSubMode = true;
                                    submode = dataProxy->getSubModeFromId(modei);
                                }
                            }
                        }
                        break;
                    case(5):
                        modei = dataProxy->getSubModeIdFromSubMode(data);
                        if (modei >= 0)
                        {
                            preparedQuery.bindValue(":modeid", QString::number(modei));
                            preparedQuery.bindValue(":submode", QString::number(modei));
                            haveSubMode = true;
                            haveMode = true;
                            submode = data;
                            //submode = data;
                        }
                        break;
                    case(6):
                        preparedQuery.bindValue(":srx", data);

                        //qDebug() << "FileManager::bprocessQsoReadingADIF-srx: " << data;
                        break;
                    case(7):
                        preparedQuery.bindValue(":stx", data);

                        //qDebug() << "FileManager::bprocessQsoReadingADIF-stx: " << data;
                        break;
                    case(8):
                        time = util->getTimeFromADIFTimeString(data);

                        if (time.isValid())
                        {
                            dateTime.setTime(time);
                            haveTime = true;
                        }
                        break;
                    case(9):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            dateTimeOFF.setDate(dateT);
                            haveDateOff = true;
                        }
                        break;
                    case(10):
                        i = dataProxy->getIdFromBandName(data);

                        //i = db->getBandIDFromName2(data);
                        if (i >= 0)
                        {
                            preparedQuery.bindValue(":band_rx", QString::number(i));
                            bandRXDef = true;
                            bandrxi = i;
                        }
                        break;
                    case(11):
                        time = util->getTimeFromADIFTimeString(data);
                        if (time.isValid())
                        {
                            dateTimeOFF.setTime(time);
                            haveTimeOff = true;
                        }
                        break;
                    case(12):
                        preparedQuery.bindValue(":rst_sent", data);

                        //qDebug() << "FileManager::bprocessQsoReadingADIF-rst_rsent: " << data;
                        rstTXr = true;
                        break;
                    case(13):

                        //qDebug() << "FileManager::bprocessQsoReadingADIF-rst_rcvd: " << data;
                        preparedQuery.bindValue(":rst_rcvd", data);
                        rstRXr = true;
                        break;
                    case(14):
                        preparedQuery.bindValue(":srx_string", data);
                        break;
                    case(15):
                        preparedQuery.bindValue(":stx_string", data);
                        break;
                    case(16):
                        preparedQuery.bindValue(":cqz", data);

                        //cqz = data.toInt();
                        break;
                    case(17):
                        preparedQuery.bindValue(":ituz", data);

                        //ituz = data.toInt();
                        break;
                    case(18):

                        //dxcc = data.toInt();
                        preparedQuery.bindValue(":dxcc", data);
                        break;
                    case(19):
                        preparedQuery.bindValue(":address", data);
                        break;
                    case(20):
                        preparedQuery.bindValue(":age", data);
                        break;
                    case(21):
                        preparedQuery.bindValue(":cnty", data);
                        break;
                    case(22):
                        preparedQuery.bindValue(":comment", data);
                        break;
                    case(23):
                        preparedQuery.bindValue(":a_index", data);
                        break;
                    case(24):
                        preparedQuery.bindValue(":ant_az", data);
                        break;
                    case(25):
                        preparedQuery.bindValue(":ant_el", data);
                        break;
                    case(26):
                        preparedQuery.bindValue(":ant_path", data);
                        break;
                    case(27):
                        preparedQuery.bindValue(":arrl_sect", data);
                        break;
                    case(28):
                        preparedQuery.bindValue(":award_granted", data);
                        break;
                    case(29):
                        preparedQuery.bindValue(":award_submitted", data);
                        break;
                    case(30):
                        preparedQuery.bindValue(":checkcontest", data);
                        break;
                    case(31):
                        preparedQuery.bindValue(":class", data);
                        break;
                    case(32):
                        preparedQuery.bindValue(":cont", data);
                        break;
                    case(33):
                        preparedQuery.bindValue(":contacted_op", data);
                        break;
                    case(34):
                        preparedQuery.bindValue(":contest_id", data);
                        break;
                    case(35):
                        preparedQuery.bindValue(":country", data);
                        break;
                    case(36):
                        preparedQuery.bindValue(":credit_submitted", data);
                        break;
                    case(37):
                        preparedQuery.bindValue(":credit_granted", data);
                        break;
                    case(38):
                        preparedQuery.bindValue(":distance", data);
                        break;
                    case(39):
                        preparedQuery.bindValue(":darc_dok", data);
                        break;
                    case(40):
                        preparedQuery.bindValue(":eq_call", data);
                        break;
                    case(41):
                        if (data.contains("@") && (data.contains(".")))
                        {
                            preparedQuery.bindValue(":email", data);
                        }
                        break;
                    case(42):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":eqsl_qslrdate", util->getDateSQLiteStringFromDate(dateT));
                        }
                        break;
                    case(43):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":eqsl_qslsdate", util->getDateSQLiteStringFromDate(dateT));
                        }
                        break;
                    case(44):
                        preparedQuery.bindValue(":eqsl_qsl_rcvd", data);
                        break;
                    case(45):
                        preparedQuery.bindValue(":eqsl_qsl_sent", data);
                        hasEqslQslSent = true;
                        break;
                    case(46):
                        preparedQuery.bindValue(":fists", data);
                        break;
                    case(47):
                        preparedQuery.bindValue(":fists_cc", data);
                        break;
                    case(48):
                        preparedQuery.bindValue(":force_init", data);
                        break;
                    case(49):

                        //qDebug() << "FileManager::processQsoReadingADIF  -FREQ: " << QString::number(data.toDouble());
                        if (haveBand)
                        {
                            if (dataProxy->getBandIdFromFreq(data.toDouble()) == bandi)

                                //if (db->isThisFreqInBand(db->getBandNameFromNumber(bandi), data))
                            {
                                preparedQuery.bindValue(":freq", data);
                                haveFreqTX = true;
                                freqTX = data;
                            }
                            else
                            {

                                // IF band is defined but not the same than freq, Band wins
                            }
                        }
                        else
                        {
                            preparedQuery.bindValue(":freq", data);
                            haveFreqTX = true;
                            freqTX = data;
                            i = dataProxy->getBandIdFromFreq(data.toDouble());

                            if (i >= 0)
                            {
                                preparedQuery.bindValue(":bandid", QString::number(i));
                                haveBand = true;

                                //qDebug() << "FileManager::processQsoReadingADIF-Band: " << data << "/"  << QString::number(i);
                            }
                        }
                        break;
                    case(50):
                        if (bandRXDef)
                        {
                            if (dataProxy->getBandIdFromFreq(data.toDouble()) == bandrxi)
                            {
                                preparedQuery.bindValue(":freq_rx", data);
                                haveFreqRX = true;
                            }
                            else
                            {

                                // IF band is defined but not the same than freq, Band wins
                            }
                        }
                        else
                        {
                            preparedQuery.bindValue(":freq_rx", data);
                            haveFreqRX = true;
                            i = dataProxy->getBandIdFromFreq(data.toDouble());

                            if (i >= 0)
                            {
                                preparedQuery.bindValue(":band_rx", QString::number(i));
                                bandRXDef = true;

                                //qDebug() << "FileManager::processQsoReadingADIF-Band: " << data << "/"  << QString::number(i);
                            }
                        }
                        break;
                    case(51):
                        preparedQuery.bindValue(":gridsquare", data);
                        break;
                    case(53):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":hrd_qso_upload_date", util->getDateSQLiteStringFromDate(dateT));
                        }
                        break;
                    case(54):
                        preparedQuery.bindValue(":hrd_qso_upload_status", data);
                        break;
                    case(55):
                        //TODO: if data is NULL or empty, ask for my_gridsquare
                        preparedQuery.bindValue(":my_gridsquare", data);
                        break;
                    case(56):

                        //qDebug() << ": MY_ANTENNA: " << data;
                        preparedQuery.bindValue(":my_antenna", data);
                        break;
                    case(57):

                        //qDebug() << "FileManager::processQsoReadingADIF (IOTA): " << data;
                        data = awards->checkIfValidIOTA(data);
                        /*
                        if (data.length()==4)
                        //EU-1
                        {
                            data.insert(3, "00");
                        }
                        else if (data.length()==5)
                        //EU-01
                        {
                            data.insert(3, "0");
                        }
                        */
                        if (data.length() == 6)
                        {
                            preparedQuery.bindValue(":iota", data);
                        }
                        break;
                    case(58):
                        preparedQuery.bindValue(":iota_island_id", data);
                        break;
                    case(59):
                        /*
                        if (data.length()==4)
                        //EU-1
                        {
                            data.insert(3, "00");
                        }
                        else if (data.length()==5)
                        //EU-01
                        {
                            data.insert(3, "0");
                        }
                        */
                        data = awards->checkIfValidIOTA(data);
                        if (data.length() == 6)
                        {
                            preparedQuery.bindValue(":my_iota", data);
                        }
                        break;
                    case(60):
                        preparedQuery.bindValue(":my_dxcc", data);
                        break;
                    case(61):
                        preparedQuery.bindValue(":my_fists", data);
                        break;
                    case(62):
                        preparedQuery.bindValue(":my_iota_island_id", data);
                        break;
                    case(63):
                        preparedQuery.bindValue(":k_index", data);
                        break;
                    case(64):
                        preparedQuery.bindValue(":lat", data);
                        break;
                    case(65):
                        preparedQuery.bindValue(":lon", data);
                        break;
                    case(66):
                        preparedQuery.bindValue(":my_lat", data);
                        break;
                    case(67):
                        preparedQuery.bindValue(":my_lon", data);
                        break;
                    case(68):
                        preparedQuery.bindValue(":my_itu_zone", data);
                        break;
                    case(69):
                        preparedQuery.bindValue(":my_postal_code", data);
                        break;
                    case(70):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":lotw_qslrdate", util->getDateSQLiteStringFromDate(dateT));
                        }
                        break;
                    case(71):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":lotw_qslsdate", util->getDateSQLiteStringFromDate(dateT));
                        }
                        break;
                    case(72):
                        preparedQuery.bindValue(":lotw_qsl_rcvd", data);
                        break;
                    case(73):
                        preparedQuery.bindValue(":lotw_qsl_sent", data);
                        hasLotwQslSent = true;
                        break;
                    case(74):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":clublog_qso_upload_date", util->getDateSQLiteStringFromDate(dateT));
                            hasClublogQslSent = true;
                        }
                        break;
                    case(75):
                        preparedQuery.bindValue(":clublog_qso_upload_status", data);
                        break;
                    case(76):
                        preparedQuery.bindValue(":max_bursts", data);
                        break;
                    case(77):
                        preparedQuery.bindValue(":ms_shower", data);
                        break;
                    case(78):
                        preparedQuery.bindValue(":my_city", data);
                        break;
                    case(79):
                        preparedQuery.bindValue(":my_cnty", data);
                        break;
                    case(80):
                        preparedQuery.bindValue(":my_country", data);
                        break;
                    case(81):
                        preparedQuery.bindValue(":my_cq_zone", data);
                        break;
                    case(82):
                        preparedQuery.bindValue(":my_name", data);
                        break;
                    case(83):
                        preparedQuery.bindValue(":name", data);
                        break;
                    case(84):
                        if (util->isValidCall(data))
                        {
                            preparedQuery.bindValue(":operator", data);
                        }
                        break;
                    case(85):
                        if (util->isValidCall(data))
                        {
                            hasStationCall = true;
                            preparedQuery.bindValue(":station_callsign", data);
                        }
                        break;
                    case(86):
                        if (util->isValidCall(data))
                        {
                            preparedQuery.bindValue(":owner_callsign", data);
                        }
                        break;
                    case(87):
                        preparedQuery.bindValue(":my_rig", data);
                        break;
                    case(88):
                        preparedQuery.bindValue(":my_sig", data);
                        break;
                    case(89):
                        preparedQuery.bindValue(":my_sig_info", data);
                        break;
                    case(90):
                        preparedQuery.bindValue(":my_sota_ref", data);
                        break;
                    case(91):
                        preparedQuery.bindValue(":my_state", data);
                        break;
                    case(92):
                        preparedQuery.bindValue(":state", data);
                        break;
                    case(93):
                        preparedQuery.bindValue(":my_street", data);
                        break;
                    case(94):
                        preparedQuery.bindValue(":my_usaca_counties", data);
                        break;
                    case(95):
                        preparedQuery.bindValue(":my_vucc_grids", data);
                        break;
                    case(96):
                        preparedQuery.bindValue(":notes", data);
                        break;
                    case(97):
                        preparedQuery.bindValue(":nr_bursts", data);
                        break;
                    case(98):
                        preparedQuery.bindValue(":nr_pings", data);
                        break;
                    case(99):
                        preparedQuery.bindValue(":pfx", data);
                        break;
                    case(100):
                        preparedQuery.bindValue(":precedence", data);
                        break;
                    case(101):
                        preparedQuery.bindValue(":prop_mode", data);
                        break;
                    case(102):
                        preparedQuery.bindValue(":public_key", data);
                        break;
                    case(103):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":qrzcom_qso_upload_date", util->getDateSQLiteStringFromDate(dateT));
                            hasQrzQslSent = true;
                        }
                        break;
                    case(104):
                        preparedQuery.bindValue(":qrzcom_qso_upload_status", data);
                        break;
                    case(105):
                        preparedQuery.bindValue(":qslmsg", data);
                        break;
                    case(106):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":qslrdate", util->getDateSQLiteStringFromDate(dateT));
                        }
                        break;
                    case(107):
                        dateT = util->getDateFromADIFDateString(data);
                        if (dateT.isValid())
                        {
                            preparedQuery.bindValue(":qslsdate", util->getDateSQLiteStringFromDate(dateT));
                        }
                        break;
                    case(108):
                        preparedQuery.bindValue(":qsl_rcvd", data);

                        //preparedQuery.bindValue( ":confirmed", '1' );
                        break;
                    case(109):
                        preparedQuery.bindValue(":qsl_sent", data);
                        break;
                    case(110):
                        preparedQuery.bindValue(":qsl_rcvd_via", data);
                        break;
                    case(111):
                        preparedQuery.bindValue(":qsl_sent_via", data);
                        break;
                    case(112):

                        //qDebug() << "FileManager::bprocessQsoReadingADIF-QSL_VIA: " << data;
                        if (data == "BUREAU")
                            // This comprobation is to "correct" old logs, mainly from KLog

                                // comming from older ADIF files
                        {
                            preparedQuery.bindValue(":qsl_sent_via", "B");
                        }
                        else if ((data == "B") || (data == "D") || (data == "E"))
                            // M Deprecated value from ADIF 304
                        {
                            preparedQuery.bindValue(":qsl_via", data);
                        }
                        else
                        {

                            // If values are not valid, are not imported.

                            //TODO: Send an error to the user to show that there was an invalid field
                        }
                        break;
                    case(113):
                        preparedQuery.bindValue(":qso_complete", data);
                        break;
                    case(114):
                        preparedQuery.bindValue(":qso_random", data);
                        break;
                    case(115):
                        preparedQuery.bindValue(":qth", data);
                        break;
                    case(116):
                        preparedQuery.bindValue(":region", data);
                        break;
                    case(117):
                        preparedQuery.bindValue(":rig", data);
                        break;
                    case(118):

                        //qDebug() << "FileManager::bprocessQsoReadingADIF-rx_pwr: " << data;
                        preparedQuery.bindValue(":rx_pwr", data);
                        break;
                    case(119):

                        //qDebug() << "FileManager::bprocessQsoReadingADIF-tx_pwr: " << data;
                        preparedQuery.bindValue(":tx_pwr", data);
                        break;
                    case(120):
                        preparedQuery.bindValue(":sat_mode", data);
                        break;
                    case(121):
                        preparedQuery.bindValue(":sat_name", data);
                        break;
                    case(122):
                        preparedQuery.bindValue(":sfi", data);
                        break;
                    case(123):
                        preparedQuery.bindValue(":sig", data);
                        break;
                    case(124):
                        preparedQuery.bindValue(":sig_info", data);
                        break;
                    case(125):
                        preparedQuery.bindValue(":silent_key", data);
                        break;
                    case(126):
                        preparedQuery.bindValue(":skcc", data);
                        break;
                    case(127):
                        preparedQuery.bindValue(":sota_ref", data);
                        break;
                    case(128):
                        preparedQuery.bindValue(":swl", data);
                        break;
                    case(129):
                        preparedQuery.bindValue(":ten_ten", data);
                        break;
                    case(130):
                        preparedQuery.bindValue(":uksmg", data);
                        break;
                    case(131):
                        preparedQuery.bindValue(":usaca_counties", data);
                        break;
                    case(132):
                        preparedQuery.bindValue(":ve_prov", data);
                        break;
                    case(133):
                        preparedQuery.bindValue(":vucc_grids", data);
                        break;
                    case(134):
                        preparedQuery.bindValue(":web", data);
                        break;
                    case(138): //Lognumber in a multiple-log file

                        //TODO: Think about how to import a file with different logs

                        //isThisQSODuplicated(const QString &_qrz, const QString &_date, const QString &_time, const int _band, const int _mode)
                        break;
                    case(139): //Importing from N1MM
                        preparedQuery.bindValue(":points", data);
                        break;
                    }
                }
            }
            else
            {
                   //qDebug() << "FileManager::processQsoReadingADIF (field) CheckAdif FALSE: " << field;
            }
        }
    }

    if (sendEQSLByDefault)
    {
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
          //qDebug() << "FileManager::processQsoReadingADIF (Don't have all mandatory fields): ";
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
        //qDebug() << "FileManager::processQsoReadingADIF - Missing fields: " << aux2;

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
           //qDebug() << "FileManager::processQsoReadingADIF defaultStationCallsign: " << defaultStationCallsign;

        if ((hasStationCall) || (util->isValidCall(defaultStationCallsign)))
        {
            preparedQuery.bindValue( ":station_callsign", defaultStationCallsign );
        }
    }

    preparedQuery.bindValue( ":lognumber", QString::number(logNumber));
    return isDupeQSO;
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

void FileManager::writeQuery(QSqlQuery query, QTextStream &out, const ExportMode _em, const bool _justMarked, const bool _onlyRequested, const int _logN )
{
    //qDebug() << Q_FUNC_INFO << ": " <<  query.lastQuery();
    int nameCol;
    QString aux;
    //bool propsat = false;    // Reset the QSO in case it is a Satellite QSO
    QSqlRecord rec = query.record();
    QSO qso;
    qso.clear();

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
        aux = (query.value(nameCol)).toString();
        qso.setCall(aux);
    }

    nameCol = rec.indexOf("qso_date");
    QDateTime tDateTime;
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); 
        tDateTime = util->getDateTimeFromSQLiteString(aux);
        qso.setDateTimeOn(tDateTime);
    }

    nameCol = rec.indexOf("bandid");
    QString bandst = QString();
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();

        //qDebug() << Q_FUNC_INFO << ": -Band-1: "  << aux;
        aux = util->checkAndFixASCIIinADIF(aux);
        //qDebug() << Q_FUNC_INFO << ": -Band-2: "  << aux;
        aux = dataProxy->getNameFromBandId(aux.toInt());
        qso.setBand(aux);
    }

    nameCol = rec.indexOf("freq");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        qDebug() << Q_FUNC_INFO << ":  FREQ_TX-1: "  << aux;
        aux = util->checkAndFixASCIIinADIF(aux);
        qDebug() << Q_FUNC_INFO << ":  FREQ_TX-2: "  << aux;
        double freqTX = aux.toDouble();
        qso.setFreqTX(freqTX);
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
            qso.setBandRX(aux);
        }
    }

    nameCol = rec.indexOf("freq_rx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        double freqRX = aux.toDouble();
        qso.setFreqRX(freqRX);
    }

    nameCol = rec.indexOf("modeid");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        // get SubModeId to check if it is the same or not from modeid
        QString aux2 = dataProxy->getSubModeFromId(aux.toInt());
        aux = dataProxy->getNameFromSubMode(aux2);
        qso.setMode(aux);
        qso.setSubmode(aux2);
    }

    nameCol = rec.indexOf("prop_mode");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setPropMode(aux);
        //qDebug() << Q_FUNC_INFO << ":  PROP_MODE" ;
    }

    nameCol = rec.indexOf("sat_name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSatName(aux);
    //qDebug() << Q_FUNC_INFO << ":  SAT_NAME" ;
    }

    nameCol = rec.indexOf("gridsquare");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setGridSquare(aux);
    }

    nameCol = rec.indexOf("my_gridsquare");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyGridSquare(aux);
    }

    nameCol = rec.indexOf("station_callsign");
    if ((nameCol>=0) && (_em != ModeEQSL))
    {
        aux = (query.value(nameCol)).toString();
        qso.setStationCallsign(aux);
    }

    nameCol = rec.indexOf("qso_date_off");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        tDateTime = util->getDateTimeFromSQLiteString(aux);
        qso.setDateOff(tDateTime.date());
        qso.setTimeOff(tDateTime.time());
    }

    nameCol = rec.indexOf("srx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSrx(aux.toInt ());
    }

    nameCol = rec.indexOf("srx_string");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSrxString(aux);
    }

    nameCol = rec.indexOf("stx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setStx(aux.toInt ());
    }

    nameCol = rec.indexOf("stx_string");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setStxString(aux);
    }

    //qDebug() << Q_FUNC_INFO << ":  - 100";
    nameCol = rec.indexOf("cqz");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setCQZone(aux.toInt());
    }

    nameCol = rec.indexOf("ituz");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setItuZone(aux.toInt());

    }
    //qDebug() << Q_FUNC_INFO << ":  DXCC - Now..." ;
    nameCol = rec.indexOf("dxcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setDXCC(aux.toInt());
    }

    nameCol = rec.indexOf("address");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setAddress(aux);
    }

    nameCol = rec.indexOf("age");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setAge(aux.toInt());
    }

    nameCol = rec.indexOf("cnty");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setCounty(aux);
    }

    nameCol = rec.indexOf("comment");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setComment(aux);
    }

    nameCol = rec.indexOf("a_index");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setA_Index(aux.toInt());
    }

    nameCol = rec.indexOf("ant_az");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setAnt_az(aux.toDouble());
    }

    nameCol = rec.indexOf("ant_el");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setAnt_el(aux.toDouble());
    }

    nameCol = rec.indexOf("ant_path");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setAnt_Path(aux);
    }

    nameCol = rec.indexOf("arrl_sect");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setARRL_Sect(aux);
    }

    nameCol = rec.indexOf("checkcontest");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setCheck(aux);
    }

    //qDebug() << Q_FUNC_INFO << ":  - 30";
    nameCol = rec.indexOf("class");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setClass(aux);
    }

    nameCol = rec.indexOf("cont");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setContinent(aux);
    }

    nameCol = rec.indexOf("contacted_op");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setContactedOperator(aux);
    }

    nameCol = rec.indexOf("contest_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setContestID(aux);
    }

    nameCol = rec.indexOf("country");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setCountry(aux);
    }

    nameCol = rec.indexOf("credit_submitted");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setCreditSubmitted(aux);
    }

    nameCol = rec.indexOf("credit_granted");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setCreditGranted(aux);
    }

    nameCol = rec.indexOf("distance");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setDistance(aux.toDouble ());
    }

    nameCol = rec.indexOf("darc_dok");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setDarcDok(aux);
    }

    nameCol = rec.indexOf("eq_call");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setEQ_Call(aux);
    }

    nameCol = rec.indexOf("email");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setEmail(aux);
    }

    nameCol = rec.indexOf("eqsl_qslrdate");
    QDate date;
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setEQSLQSLRDate(date);
    }

    nameCol = rec.indexOf("eqsl_qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setEQSLQSLSDate(date);
    }

    nameCol = rec.indexOf("eqsl_qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setEQSLQSL_RCVD(aux);
    }

    nameCol = rec.indexOf("eqsl_qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setEQSLQSL_SENT(aux);
    }

    nameCol = rec.indexOf("fists");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setFists(aux.toInt());
    }

    nameCol = rec.indexOf("fists_cc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setFistsCC(aux.toInt());
    }

    nameCol = rec.indexOf("force_init");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setForceInit(util->QStringToBool(aux));
    }

    nameCol = rec.indexOf("hrdlog_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setHRDUpdateDate(date);
    }

    nameCol = rec.indexOf("hrdlog_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        //TODO: Add a isValidUploadStatus
        qso.setHRDLogStatus(aux);
    }

    nameCol = rec.indexOf("my_antenna");
    //qDebug() << Q_FUNC_INFO << ": my_antenna-0";
    if (nameCol>=0)
    {
        //qDebug() << Q_FUNC_INFO << ": my_antenna-1";
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyAntenna(aux);
    }
    //qDebug() << Q_FUNC_INFO << ": my_antenna-99";
    nameCol = rec.indexOf("my_dxcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyDXCC(aux.toInt());
    }

    nameCol = rec.indexOf("my_fists");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyFists(aux.toInt());
    }

    nameCol = rec.indexOf("iota");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setIOTA(aux);
    }

    nameCol = rec.indexOf("iota_island_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setIotaID(aux.toInt());
    }

    nameCol = rec.indexOf("my_iota");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyIOTA(aux);
    }
    nameCol = rec.indexOf("my_iota_island_id");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyIotaID(aux.toInt());
    }

    nameCol = rec.indexOf("k_index");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setK_Index(aux.toInt());
    }

    nameCol = rec.indexOf("my_itu_zone");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyITUZone(aux.toInt());
    }

    nameCol = rec.indexOf("lat");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setLatitude(aux);
    }

    nameCol = rec.indexOf("lon");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setLongitude(aux);
    }

    nameCol = rec.indexOf("my_lat");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyLatitude(aux);
    }

    nameCol = rec.indexOf("my_lon");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyLongitude(aux);
    }

    nameCol = rec.indexOf("lotw_qslrdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setLoTWQSLRDate(date);
    }

    nameCol = rec.indexOf("lotw_qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setLoTWQSLSDate(date);
    }

    nameCol = rec.indexOf("lotw_qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setLoTWQSL_RCVD(aux);
    }

    nameCol = rec.indexOf("lotw_qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setLoTWQSL_SENT(aux);
    }

    nameCol = rec.indexOf("clublog_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setClublogQSOUpdateDate(date);
    }

    nameCol = rec.indexOf("clublog_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setClubLogStatus(aux);
    }

    nameCol = rec.indexOf("qrzcom_qso_upload_date");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setQRZCOMDate(date);
    }
    nameCol = rec.indexOf("qrzcom_qso_upload_status");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQRZCOMStatus(aux);
    }
    nameCol = rec.indexOf("max_bursts");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMaxBursts(aux.toInt());
    }
    nameCol = rec.indexOf("ms_shower");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMsShower(aux);
    }
    nameCol = rec.indexOf("my_city");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyCity(aux);
    }

    nameCol = rec.indexOf("my_cnty");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyCounty(aux);
    }
    nameCol = rec.indexOf("my_country");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyCountry(aux);
    }
    nameCol = rec.indexOf("my_cq_zone");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyCQZone(aux.toInt());
    }
    nameCol = rec.indexOf("my_name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyName(aux);
    }
    nameCol = rec.indexOf("name");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setName(aux);
    }
    nameCol = rec.indexOf("operator");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setOperatorCallsign(aux);
    }
    nameCol = rec.indexOf("owner_callsign");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setOwnerCallsign(aux);
    }
    nameCol = rec.indexOf("my_postal_code");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyPostalCode(aux);
    }
    nameCol = rec.indexOf("my_rig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyRig(aux);
    }
    nameCol = rec.indexOf("my_sig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMySig(aux);
    }
    nameCol = rec.indexOf("my_sota_ref");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMySOTA_REF(aux);
    }

    nameCol = rec.indexOf("my_state");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyState(aux);
    }
    nameCol = rec.indexOf("my_street");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyStreet(aux);
    }
    nameCol = rec.indexOf("notes");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setNotes(aux);
    }
    nameCol = rec.indexOf("nr_bursts");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setNrBursts(aux.toInt());
    }
    nameCol = rec.indexOf("nr_pings");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setNrPings(aux.toInt());
    }
    nameCol = rec.indexOf("pfx");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setPrefix(aux);
    }

    nameCol = rec.indexOf("precedence");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setPrecedence(aux);
    }
    nameCol = rec.indexOf("public_key");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setPublicKey(aux);
    }
    nameCol = rec.indexOf("qslmsg");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQSLMsg(aux);
    }
    nameCol = rec.indexOf("qslrdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setQSLRDate(date);
    }
    nameCol = rec.indexOf("qslsdate");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString();
        date = util->getDateFromSQliteString(aux);
        qso.setQSLSDate(date);
    }
    nameCol = rec.indexOf("qsl_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQSL_RCVD(aux);
    }

    nameCol = rec.indexOf("qsl_rcvd_via");
    if (nameCol>=0)
    {
       aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
       qso.setQSLRecVia(aux);
    }

    nameCol = rec.indexOf("qsl_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQSL_SENT(aux);
    }
    nameCol = rec.indexOf("qsl_sent_via");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQSLSenVia(aux);
    }
    nameCol = rec.indexOf("qsl_via");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQSLVia(aux);
    }
    nameCol = rec.indexOf("qso_complete");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQSOComplete(aux);
    }
    nameCol = rec.indexOf("qso_random");
    if (nameCol>=0)
    {   //TODO: Check wether it makes sense to use this field for ALL QSOs or just when it is not random.
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQSORandom(util->QStringToBool(aux));
    }
    nameCol = rec.indexOf("qth");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setQTH(aux);
    }
    nameCol = rec.indexOf("rst_sent");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setRSTTX(aux);
    }
    nameCol = rec.indexOf("rst_rcvd");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setRSTRX(aux);
    }
    nameCol = rec.indexOf("region");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setRegion(aux);
    }
    nameCol = rec.indexOf("rig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setRig(aux);
    }
    nameCol = rec.indexOf("rx_pwr");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setRXPwr(aux.toDouble());
    }
    nameCol = rec.indexOf("tx_pwr");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setTXPwr(aux.toDouble());
    }
    nameCol = rec.indexOf("sat_mode");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSatMode(aux);
    }
    nameCol = rec.indexOf("sfi");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSFI(aux.toInt());
    }
    nameCol = rec.indexOf("sig");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSig(aux);
    }
    nameCol = rec.indexOf("sig_info");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSigInfo(aux);
    }
    nameCol = rec.indexOf("silent_key");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSilentKey(util->QStringToBool(aux));
    }
    nameCol = rec.indexOf("skcc");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSkcc(aux);
    }
    nameCol = rec.indexOf("sota_ref");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSOTA_REF(aux);
    }
    nameCol = rec.indexOf("state");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setState(aux);
    }
    nameCol = rec.indexOf("swl");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setSwl(util->QStringToBool(aux));
    }
    nameCol = rec.indexOf("ten_ten");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setTenTen(aux.toInt());
    }

    nameCol = rec.indexOf("uksmg");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setUksmg(aux.toInt());
    }
    nameCol = rec.indexOf("ve_prov");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setVeProv(aux);
    }
    nameCol = rec.indexOf("my_usaca_counties");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyUsacaCounties(aux);
    }
    nameCol = rec.indexOf("usaca_counties");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setUsacaCounties(aux);
    }
    nameCol = rec.indexOf("vucc_grids");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setVUCCGrids(aux);
    }
    nameCol = rec.indexOf("my_vucc_grids");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setMyVUCCGrids(aux);
    }
    nameCol = rec.indexOf("web");
    if (nameCol>=0)
    {
        aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
        qso.setWeb(aux);
    }
    if (_logN == -1)
    {
        nameCol = rec.indexOf("lognumber");
        if (nameCol>=0)
        {
            aux = (query.value(nameCol)).toString(); aux = util->checkAndFixASCIIinADIF(aux);
            qso.setLogId(aux.toInt());
        }
    }
   // out << "<EOR>\n";
    out << qso.getADIF() << "<EOR>\n";
}
