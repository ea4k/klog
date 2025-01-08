/***************************************************************************
                          lotwutilities.cpp  -  description
                             -------------------
    begin                : apr 2020
    copyright            : (C) 2020 by Jaime Robles
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

#include "lotwutilities.h"
#include "callsign.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

LoTWUtilities::LoTWUtilities(const QString &_klogDir, const QString &_klogVersion, const QString &_parentFunction, DataProxy_SQLite *dp)
{
    Q_UNUSED(_parentFunction);
#ifdef QT_DEBUG
  //qDebug() << Q_FUNC_INFO << ": "  << _klogDir << " - " << _parentFunction;
#else
#endif
    dataProxy = dp;
    calendar = new QCalendarWidget;

    manager = new QNetworkAccessManager(this);

    reply = nullptr;
    file = new QFile;
    //url = new QUrl;
    klogDir = _klogDir;
    klogVersion = _klogVersion;
    downloadAborted = false;
    stationCallsign.clear();
    startDate.clear();
    lotwQuery.clear();
    lotwUser.clear();
    lotwPassword.clear();
    fileName = "lotwDownload.adi";

    pDialog = new QProgressDialog(nullptr);
    pDialog->cancel();
    firstDate = QDate::currentDate();
    calendar->setToolTip(tr("Double click on the date that you want to use as the start date for downloading QSOs."));

    connect(calendar, SIGNAL(activated(QDate)), this, SLOT(slotCalendarDateSelected(QDate)));

   //qDebug() << Q_FUNC_INFO << ":  - END" ;
}

LoTWUtilities::~LoTWUtilities()
{
    delete(dataProxy);
    //delete(util);
    delete(file);
    delete(manager);
      //qDebug() << Q_FUNC_INFO << " - END" ;
}

void LoTWUtilities::slotCalendarDateSelected(const QDate _d)
{
   //qDebug() << Q_FUNC_INFO << ": " << _d.toString("yyyyMMdd") ;
    firstDate = _d;
    startThefullDownload();
}

void LoTWUtilities::setFileName(const QString &_fn)
{
    //qDebug() << Q_FUNC_INFO << ":  " << _fn ;
    if (_fn.length()>0)
    {
        fileName = _fn;
    }
    //qDebug() << Q_FUNC_INFO << ":  - END"  ;
}

QString LoTWUtilities::getFileName()
{
    //qDebug() Q_FUNC_INFO << ":  " << fileName  ;
    return fileName;
}

bool LoTWUtilities::selectQuery(const int _queryId)
{
     //qDebug() << Q_FUNC_INFO << ":  - Start: " << QString::number(_queryId);
    bool savePassword = true;
    if (lotwPassword.length()<1)
    {
        savePassword = false;

        bool ok;
        lotwPassword = QInputDialog::getText(nullptr, tr("KLog - LoTW password needed"),
                                                   tr("Please enter your LoTW password: "), QLineEdit::Password, "", &ok);
        if (!ok)
        {
             //qDebug() << Q_FUNC_INFO << ":  - END 1" <<  QT_ENDL;
            return false;
        }
    }
    switch (_queryId)
    {
    case 1: // Normal query
        lotwQuery = QString("https://lotw.arrl.org/lotwuser/lotwreport.adi?login=%1&password=%2&qso_query=1&qso_qsl=no&qso_owncall=%3&qso_startdate=%4").arg(lotwUser).arg(lotwPassword).arg(stationCallsign).arg(startDate);
        break;
    case 2:
        lotwQuery = QString("https://lotw.arrl.org/lotwuser/lotwreport.adi?login=%1&password=%2&qso_query=1&qso_qsl=no&qso_owncall=%3&qso_startdate=%4").arg(lotwUser).arg(lotwPassword).arg(stationCallsign).arg(firstDate.toString("yyyyMMdd"));
        break;
    default:
    {
        lotwQuery = QString("https://lotw.arrl.org/lotwuser/lotwreport.adi?login=%1&password=%2&qso_query=1&qso_qsl=no&qso_owncall=%3&qso_startdate=%4").arg(lotwUser).arg(lotwPassword).arg(stationCallsign).arg(startDate);
    }
    }

    if (!savePassword)
    {// We delete the password as soon as possible if the user is not willing to save it
        lotwPassword = QString();
    }
    url = QUrl(lotwQuery);

     //qDebug() << Q_FUNC_INFO << ":  - END" <<  QT_ENDL;

    return true;
}

bool LoTWUtilities::setStationCallSign(const QString &_call)
{
    //qDebug() << Q_FUNC_INFO << ":  " << _call;
    Callsign callsign(_call);
    if (!callsign.isValid())
    {
        //qDebug() << Q_FUNC_INFO << ":  FALSE 1" ;
        return false;
    }
    if (((dataProxy->getStationCallSignsFromLog(-1)).contains(_call)))
    {
        //qDebug() << Q_FUNC_INFO << ":  TRUE" ;
        stationCallsign = _call;
        QDate date = dataProxy->getFirstQSODateFromCall(stationCallsign);
        //qDebug() << Q_FUNC_INFO << ":  Date: " << startDate ;
        if (date.isValid())
        {
            startDate = date.toString("yyyyMMdd");
             //qDebug() << Q_FUNC_INFO << ":  StartDate" << startDate ;
        }
        else
        {
            startDate.clear();
             //qDebug() << Q_FUNC_INFO << ":  StartDate not valid Date";
             //qDebug() << Q_FUNC_INFO << "False 2 - END";
            return false;
        }

        //qDebug() << Q_FUNC_INFO << ":  startDate: " << startDate ;
        //qDebug() << Q_FUNC_INFO << "True 1 - END";
        return true;
    }
    else if (dataProxy->getHowManyQSOInLog(-1) <1)
    {
        //qDebug() << Q_FUNC_INFO << ": TRUE Empty log" ;
        stationCallsign = _call;
        //qDebug() << Q_FUNC_INFO << "True 2 - END";
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ":  Not a single QSO in the log with that StationCallsign: " << stationCallsign ;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - LoTW Station callsign"));
        QString aux = QString(tr("There is not a single QSO in the log with that station callsign.") );
        msgBox.setText(tr("Are you sure that you want to use that station callsign (%1)?").arg(_call));
        msgBox.setDetailedText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            stationCallsign = _call;
            //qDebug() << Q_FUNC_INFO << "True 3 - END";
            return true;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << ":  FALSE 2" ;
            //qDebug() << Q_FUNC_INFO << "FALSE 3 - END";
            return false;
        }
    }
}

void LoTWUtilities::startRequest(QUrl url)
{
    //qDebug() << Q_FUNC_INFO << ":  " << url.toString() ;
    QByteArray agent = QString("KLog-" + klogVersion).toUtf8();
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, agent);
    //request.setRawHeader("User-Agent", agent);
    //reply = manager->get(QNetworkRequest(url));
    reply = manager->get(request);
    //qDebug() << Q_FUNC_INFO << ":  - 10" ;
    // Whenever more data is received from the network,
    // this readyRead() signal is emitted
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    // Also, downloadProgress() signal is emitted when data is received
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64)));
    // This signal is emitted when the reply has finished processing.
    // After this signal is emitted,
    // there will be no more updates to the reply's data or metadata.
    connect(reply, SIGNAL(finished()), this, SLOT(slotFinished()));
    //qDebug() << Q_FUNC_INFO << ":  - END";
}

int LoTWUtilities::download()
{
    //qDebug() << Q_FUNC_INFO << ": - Start";
    if (!selectQuery(-1))
    {
        //qDebug() << Q_FUNC_INFO << ":  - END-1";
        return -1;
    }
    //qDebug() << Q_FUNC_INFO << ":  - 10";
    QFileInfo fileInfo(url.path());

    //qDebug() << Q_FUNC_INFO << ":  - 11";
    if (QFile::exists(fileName))
    {
        //qDebug() << Q_FUNC_INFO << ":  - 12";
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - LoTW File already exists"));
        QString aux = QString(tr("There is a file already existing with the name that will be used.") );
        msgBox.setText(tr("The file %1 already exist. Do you want to overwrite?").arg(fileName));
        msgBox.setDetailedText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        //qDebug() << Q_FUNC_INFO << ":  - 13";
        if (ret == QMessageBox::No)
        {
             //qDebug() <<Q_FUNC_INFO << ":  - END- 2";
            return -2;
        }
      }
        //qDebug() << Q_FUNC_INFO << ":  - 20 Filename: -" << fileName << "-";
        if (!file->isOpen())
        {
            //qDebug() << Q_FUNC_INFO << ":  - 21 Filename: -";
            file->setFileName(fileName);
            //qDebug() << Q_FUNC_INFO << ":  - 22 Filename: -";
        }
        //qDebug() << Q_FUNC_INFO << ":  - 23 Filename: -";

      if (!file->open(QIODevice::WriteOnly)) /* Flawfinder: ignore */
      {
          QMessageBox msgBox;
          msgBox.setIcon(QMessageBox::Warning);
          msgBox.setWindowTitle(tr("KLog - LoTW Can't write the file"));
          QString aux = QString(tr("KLog was not able to save the file %1.\nError returned: %2") ).arg(fileName).arg(file->errorString());
          msgBox.setText(tr("The file %1 already exists.").arg(fileName));
          msgBox.setDetailedText(aux);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          //file->close();
          //delete file;
          //file = nullptr;
           //qDebug() << Q_FUNC_INFO << ":  - END - 3";
          return -3;
      }
    //qDebug() << Q_FUNC_INFO << ":  - 30";
      // used for progressDialog
      // This will be set true when canceled from progress dialog
    downloadAborted = false;
    //qDebug() << Q_FUNC_INFO << ":  - 31";
    //progressDialog = new QProgressDialog(nullptr);
    //qDebug() << Q_FUNC_INFO << ":  - 40";
    pDialog->setLabelText(tr("Downloading data to file: %1.").arg(fileName));
    //qDebug() << Q_FUNC_INFO << ":  - 41";
    pDialog->setWindowTitle(tr("KLog - LoTW download"));
    //qDebug() << Q_FUNC_INFO << ":  - 42";
    pDialog->setWindowModality(Qt::WindowModal);
    //qDebug() << Q_FUNC_INFO << ":  - 43";
    pDialog->reset();
    //qDebug() << Q_FUNC_INFO << ":  - 44";
    pDialog->setRange(0, 0);
    //qDebug() << Q_FUNC_INFO << ":  - 45";
    pDialog->setMinimumDuration(0);
    //qDebug() << Q_FUNC_INFO << ":  - 46";
    pDialog->show();
    //qDebug() << Q_FUNC_INFO << ":  - 47";

    connect(pDialog, SIGNAL(canceled()), this, SLOT(slotCancelDownload()));
    //qDebug() << Q_FUNC_INFO << ":  - 50";

    startRequest(url);
     //qDebug() << "LoTWUtilities::download - END";
    return 1;
}

int LoTWUtilities::fullDownload()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    QDate date = dataProxy->getFirstQSODateFromCall(stationCallsign);
   //qDebug() << Q_FUNC_INFO << ":  Date: " << startDate ;
    if (date.isValid())
    {
        startDate = date.toString("yyyyMMdd");
        //qDebug() << Q_FUNC_INFO << ":  StartDate" << startDate ;
    }
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("KLog - LoTW Start date selection"));
    QString aux = QString(tr("This is the first date of a QSO with the callsign %1 in this log If you think that in LoTW you may have previous QSOs, answer No.").arg(stationCallsign) );
    msgBox.setText(tr("Do you want to use this date (%1) as start date?").arg(startDate));
    msgBox.setDetailedText(aux);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes)
    {
         //qDebug() << Q_FUNC_INFO << ":  - Yes";
          firstDate = date;
          startThefullDownload();
          return 1;
    }

    calendar->setSelectedDate(firstDate);
    calendar->setGridVisible(true);
    calendar->setMaximumDate(QDate::currentDate());
    calendar->show();
   //qDebug() << Q_FUNC_INFO << ":  - END";
    return 2;
}

int LoTWUtilities::startThefullDownload()
{
   //qDebug() << Q_FUNC_INFO << ":  - Start";
    if (calendar->isVisible())
    {
        calendar->close();
    }

    if (!selectQuery(2))
    {
       //qDebug() << Q_FUNC_INFO << ":  - END-1";
        return -1;
    }
   //qDebug() << Q_FUNC_INFO << ": : - 10";
    QFileInfo fileInfo(url.path());

   //qDebug() << Q_FUNC_INFO << ": : - 11";
   if (QFile::exists(fileName))
   {
       //qDebug() << Q_FUNC_INFO << ": : - 12";
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Question);
       msgBox.setWindowTitle(tr("KLog - LoTW File already exists"));
       QString aux = QString(tr("There is a file already existing with the name that will be used.") );
       msgBox.setText(tr("The file %1 already exist. Do you want to overwrite?").arg(fileName));
       msgBox.setDetailedText(aux);
       msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
       msgBox.setDefaultButton(QMessageBox::Yes);
       int ret = msgBox.exec();
       //qDebug() << Q_FUNC_INFO << ": : - 13";
       if (ret == QMessageBox::No)
       {
            //qDebug() << Q_FUNC_INFO << ":  - END- 2";
           return -2;
       }
     }
       //qDebug() << Q_FUNC_INFO << ": : - 20 Filename: -" << fileName << "-";
       if (!file->isOpen())
       {
           //qDebug() << Q_FUNC_INFO << ": : - 21 Filename: -";
           file->setFileName(fileName);
           //qDebug() << Q_FUNC_INFO << ": : - 22 Filename: -";
       }
       //qDebug() << Q_FUNC_INFO << ": : - 23 Filename: -";

     if (!file->open(QIODevice::WriteOnly)) /* Flawfinder: ignore */
     {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setWindowTitle(tr("KLog - LoTW Can't write the file"));
         QString aux = QString(tr("KLog was not able to save the file %1.\nError returned: %2") ).arg(fileName).arg(file->errorString());
         msgBox.setText(tr("The file %1 already exists.").arg(fileName));
         msgBox.setDetailedText(aux);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         //file->close();
         //delete file;
         //file = nullptr;
          //qDebug() << Q_FUNC_INFO << ":  - END - 3";
         return -3;
     }
   //qDebug() << Q_FUNC_INFO << ": : - 30";
     // used for progressDialog
     // This will be set true when canceled from progress dialog
   downloadAborted = false;
   //qDebug() << Q_FUNC_INFO << ": : - 31";
   //progressDialog = new QProgressDialog(nullptr);
   //qDebug() << Q_FUNC_INFO << ": : - 40";
   pDialog->setLabelText(tr("Downloading data to file: %1.").arg(fileName));
   //qDebug() << Q_FUNC_INFO << ": : - 41";
   pDialog->setWindowTitle(tr("KLog - LoTW download"));
   //qDebug() << Q_FUNC_INFO << ": : - 42";
   pDialog->setWindowModality(Qt::WindowModal);
   //qDebug() << Q_FUNC_INFO << ": : - 43";
   pDialog->reset();
   //qDebug() << Q_FUNC_INFO << ": : - 44";
   pDialog->setRange(0, 0);
   //qDebug() << Q_FUNC_INFO << ": : - 45";
   pDialog->setMinimumDuration(0);
   //qDebug() << Q_FUNC_INFO << ": : - 46";
   pDialog->show();
   //qDebug() << Q_FUNC_INFO << ": : - 47";

   connect(pDialog, SIGNAL(canceled()), this, SLOT(slotCancelDownload()));
   //qDebug() << Q_FUNC_INFO << ": : - 50";

   startRequest(url);
    //qDebug() << Q_FUNC_INFO << ":  - END";

    return 1;
}

void LoTWUtilities::slotDownloadProgress(qint64 bytesRead) {
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(bytesRead);
    if (downloadAborted)
    {
         //qDebug() << Q_FUNC_INFO << ":  CANCELLED";
        return;
    }

    pDialog->setValue(bytesRead);
     //qDebug() << Q_FUNC_INFO << ": - END ";
}

void LoTWUtilities::slotReadyRead()
{
    //qDebug() << Q_FUNC_INFO << ":  " << reply->readLine();
    if (file)
    {
        file->write(reply->readAll());
    }
    //qDebug() << Q_FUNC_INFO << ":  - END";
}

void LoTWUtilities::slotFinished()
{
    //qDebug() << Q_FUNC_INFO << ":  - Started";
    // when canceled
     if (downloadAborted)
     {
         if (file)
         {
             file->close();
             file->remove();
             //delete file;
             //file = nullptr;
         }
         //reply->deleteLater();
         pDialog->cancel();
         reply->close();
         //qDebug() << Q_FUNC_INFO << ":  - END Canceled";
         return;
     }
    //qDebug() << Q_FUNC_INFO << ":  - 10";
     // download finished normally
    pDialog->cancel();
    //qDebug() << Q_FUNC_INFO << ":  - 11";
    file->flush();
    //qDebug() << Q_FUNC_INFO << ":  - 12";
    file->close();
    //qDebug() << Q_FUNC_INFO << ":  - 13";

     // get redirection url
     QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
     //qDebug() << Q_FUNC_INFO << ":  - 14";
     if (reply->error())
     {
         file->remove();
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setWindowTitle(tr("KLog - LoTW Download error"));
         QString aux;
         msgBox.setText(tr("There was an error (%1) while downloading the file from LoTW.").arg(QString::number(reply->error())));
         aux = QString(tr("The downloading error details are: %1") ).arg(reply->errorString());
         msgBox.setDetailedText(aux);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
    }
    else if (!redirectionTarget.isNull())
    {
         //qDebug() << Q_FUNC_INFO << ":  - Redirection";
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - LoTW Redirection found"));
        QString aux = QString(tr("The remote server redirected our connection to %1") ).arg(newUrl.toString());
        msgBox.setText(tr("Do you want to follow the redirection?"));
        msgBox.setDetailedText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes)
        {
            url = newUrl;
            //reply->deleteLater();
            file->open(QIODevice::WriteOnly); /* Flawfinder: ignore */
            file->resize(0);
            startRequest(url);
             //qDebug() << Q_FUNC_INFO << ":  - END-1";
            return;
        }
     }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": :  " ;
    }
    //qDebug() << "LoTWUtilities::slotReadyRead - Going to parse ...";
    parseDownloadedFile(file->fileName());
    //qDebug() << "LoTWUtilities::slotReadyRead - END";
}

void LoTWUtilities::slotCancelDownload()
{
     //qDebug() << Q_FUNC_INFO << ":  - Start";
    downloadAborted = true;
    reply->abort();
     //qDebug() << Q_FUNC_INFO << ":  - END";
}

void LoTWUtilities::setUser(const QString &_call)
{
     //qDebug() << Q_FUNC_INFO << ":  " << _call;
    lotwUser = _call;
     //qDebug() << Q_FUNC_INFO << ":  END";
}

void LoTWUtilities::setPass(const QString &_pass)
{
     //qDebug() << Q_FUNC_INFO << ":  " << _pass;
    lotwPassword = _pass;
     //qDebug() << Q_FUNC_INFO << ": END";
}

bool LoTWUtilities::getIsReady()
{
     //qDebug() << Q_FUNC_INFO << ":  user/station: -" << lotwUser <<"/" << stationCallsign << "-";
    if ((lotwUser.length()>1) && (stationCallsign.length()>1))
    {
         //qDebug() << Q_FUNC_INFO << ":  true";
        return true;
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << ":  false";
      return false;
    }
}

void LoTWUtilities::parseDownloadedFile(const QString &_fn)
{
    //qDebug() << Q_FUNC_INFO << ":  " << _fn;
    QString _fileName = _fn;
    QMessageBox msgBox;
    QString aux;

    QFile file( _fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //qDebug() << Q_FUNC_INFO << ":  File not found" << _fileName;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - LoTW File not found"));
        msgBox.setText(tr("KLog can't find the downloaded file."));
        aux = QString(tr("It was not possible for find the file %1 that has been just downloaded.") ).arg(_fn);

        msgBox.setDetailedText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        //qDebug() <<Q_FUNC_INFO << ":  END-1";
        return ;
    }
    else
    {
        //qint64 startOfFile = file.pos();
        // Look for a Header
        bool hasHeader = false;
        int numQSO = 0;
        QString Lotw_owncall = QString("OWNCALL: %1").arg(stationCallsign.toUpper());
        //bool hasOwnCall = false;
        //bool hasProgramID = false;
        bool userPasswordError = false;
        while ((!file.atEnd()) && (!hasHeader))
        {
            QByteArray line = file.readLine();
            QString lineU = line.toUpper();
            //qDebug() << Q_FUNC_INFO << ":  lineU: " << lineU;
            if (lineU.contains("<EOH>"))
            {
                 //qDebug() << Q_FUNC_INFO << ":  EOH found";
                hasHeader = true;
            }
            //else if (lineU.contains("<PROGRAMID:4>LOTW"))
            //{
            //     //qDebug() << Q_FUNC_INFO << ":  ProgramID found";
            //    hasProgramID = true;
            //}
            //else if (lineU.contains(Lotw_owncall))
            //{
            //     //qDebug() << Q_FUNC_INFO << ":  OWNCALL found";
            //    hasOwnCall = true;
            //}
            else if (lineU.contains("<APP_LOTW_NUMREC:"))
            {
                QStringList data;
                data << lineU.split('>', QT_SKIP);

                numQSO = (data.at(1)).toInt();
                 //qDebug() << Q_FUNC_INFO << ":  QSOs: " << QString::number(numQSO);
            }
            else if (lineU.contains("<I>USERNAME/PASSWORD INCORRECT</I>"))
            {
                userPasswordError = true;
            }
        }
        // WE HAVE JUST FINISHED TO READ THE HEADER OR THE FILE, IF IT IS NOT AN ADIF
        if (!hasHeader || (numQSO<1))
        {
             //qDebug() << Q_FUNC_INFO << ":  Header not found" << _fileName;
            QString aux;
            if (userPasswordError)
            {
                msgBox.setWindowTitle(tr("KLog - LoTW user/password error"));
                msgBox.setText(tr("LoTW server did not recognized your user/password"));
                aux = QString(tr("Check your user and password and ensure your are using the right one before trying again.") );
            }
            else if(numQSO<1)
            {
                msgBox.setWindowTitle(tr("KLog - LoTW No QSOs "));
                msgBox.setText(tr("LoTW sent no QSOs"));
                aux = QString(tr("It seems that LoTW has no QSO with the Station Callsign you are using (%1).") ).arg(stationCallsign);
            }
            else
            {
                msgBox.setWindowTitle(tr("KLog - LoTW Unknown error"));
                msgBox.setText(tr("KLog can't recognize the file that has been downloaded from LoTW."));
                aux = QString(tr("Try again and send the downloaded file (%1) to the KLog developer for analysis.") ).arg(_fileName);
            }

            msgBox.setIcon(QMessageBox::Warning);

            msgBox.setDetailedText(aux);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            //qDebug() << Q_FUNC_INFO << ":  END-2";
            file.remove();
            return ;
        }

        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog - LoTW download"));
        msgBox.setText(tr("KLog downloaded %1 QSOs successfully. Do you want to update your log with the downloaded data?").arg(QString::number(numQSO)));
        aux = QString(tr("Now KLog will process the downloaded QSO and update your local log.") );

        msgBox.setDetailedText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        if (msgBox.exec() == QMessageBox::No)
        {
            //qDebug() << Q_FUNC_INFO << ":  END-2";
            return ;
        }
        //file.seek(startOfFile);
        emit actionProcessLoTWDownloadedFile(_fileName);
    }


   //Procesar los QSOs y meterlos en una tabla? o en un QStringList o alguna otra estructura


    //qDebug() << "LoTWUtilities::parseDownloadedFile - END" ;
}

/*void LoTWUtilities::showMessage(const int _messageIcon, const QString &_msg, const QString &_msgExt)
{

}
*/
