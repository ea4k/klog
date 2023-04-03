/***************************************************************************
                          softwareupdate.cpp  -  description
                             -------------------
    begin                : feb 2017
    copyright            : (C) 2017 by Jaime Robles
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

#include "softwareupdate.h"

SoftwareUpdate::SoftwareUpdate(const QString &_klogVersion) : QObject(nullptr)
{
      //qDebug() << "SoftwareUpdate::SoftwareUpdate(): " << _klogVersion;
    util = new Utilities(Q_FUNC_INFO);
    updateDialog = new SoftwareUpdateDialog();
    latestVersion = "0.0";
    repositoryFound = false;
    url = new QUrl;
    setVersion(_klogVersion);
    callsign = QString();
    OSString = QString();
    messageShown = false;
    OSVersion = QOperatingSystemVersion::currentType();
    //qDebug() << "SoftwareUpdate::SoftwareUpdate OSVersion:: " << QString::number(OSVersion) ;
    findOS(QOperatingSystemVersion::currentType());
    setHeader();
    //qDebug() << "SoftwareUpdate::SoftwareUpdate(): - END" ;
}

void SoftwareUpdate::findOS(const int _os)
{
     //qDebug() << Q_FUNC_INFO << ": " << QString::number(_os) ;
    switch (_os)
    {
        case QOperatingSystemVersion::MacOS:
            OSString = QString("osx");
        break;
        case QOperatingSystemVersion::Windows:
            OSString = QString("win");
        break;
        case QOperatingSystemVersion::Unknown:
            OSString = QString();
        break;
        default:
        // should never be reached
        break;
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

SoftwareUpdate::~SoftwareUpdate()
{
    delete(updateDialog);
    delete(util);
    delete(url);
}

void SoftwareUpdate::setVersion(const QString &_klogVersion)
{
    //qDebug() << Q_FUNC_INFO << ": " << _klogVersion;
    klogVersion = _klogVersion;
    latestVersion = klogVersion;
    setHeader();
    //qDebug() << "SoftwareUpdate::setVersion: END " ;
}

void SoftwareUpdate::slotReadyRead()
{
      //qDebug() << "SoftwareUpdate::slotReadyRead: ";
}

void SoftwareUpdate::slotDownloadFinished(QNetworkReply *reply)
{
    //qDebug() << Q_FUNC_INFO << " - Start ";

    QUrl url = reply->url();
    if (url.toString().length()< QString("https://api.github.com/repos/ea4k/klog/releases/latest").length())
    {
        //qDebug() << Q_FUNC_INFO << ": URL too short" ;
        return;
    }
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error())
    {
          //qDebug() << Q_FUNC_INFO << ": reply error: " << QString::number(reply->error()) ;
    }
    else if (!redirectionTarget.isNull())
    {
        repositoryFound = false;
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        //qDebug() << "SoftwareUpdate::slotDownloadFinished: Redirect: " << newUrl.toString();
        url = newUrl;
        reply->deleteLater();
        connectToURL(url.toString());
        //qDebug() << "SoftwareUpdate::slotDownloadFinished - end After connect";
        return;
    }
    else
    {
          //qDebug() << "SoftwareUpdate::slotDownloadFinished: no redirection" ;
        if (checkUpdates(reply))
        {
            //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates true" ;
            if (repositoryFound)
            {
                //qDebug() << "SoftwareUpdate::slotDownloadFinished repository found" ;
                if(latestVersion>klogVersion)
                {
                    //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates should update!" ;
                    updateDialog->setVersion(latestVersion, true);
                }
                updateDialog->show();
                latestVersion = klogVersion;
                repositoryFound = false;
            }
        }
        else
        {
            if (repositoryFound && messageShown)
            {
                  //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates false!" ;
                updateDialog->setVersion(latestVersion, false);
                updateDialog->show();
            }
             //qDebug() << "SoftwareUpdate::slotDownloadFinished:  checkupdates false" ;
        }
    }
    reply->deleteLater();
    //qDebug() << "SoftwareUpdate::slotDownloadFinished end" ;
}

bool SoftwareUpdate::checkUpdates(QIODevice *data)
{
    // Checks if there is a new version in the repository

    //qDebug() << "SoftwareUpdate::checkUpdates: " << QString::number(data->size());
    QString line, release;
    QStringList stringList, klogStringList;

    QRegularExpression rx;
    QString filterString;
    QString fileExtension;
    switch (OSVersion)
    {
        case QOperatingSystemVersion::Unknown:
              //qDebug() << "MainWindow::checkUpdates - Unknown" ;
            rx.setPattern("klog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?.tar.gz");
            filterString = QString("klog");
            fileExtension = QString(".tar.gz");
        break;
        case QOperatingSystemVersion::Windows:
              //qDebug() << "MainWindow::checkUpdates - Windows" ;
            rx.setPattern("KLog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?-windows-installer.exe");
            filterString = QString("KLog");
            fileExtension = QString("-windows-installer.exe");
        break;
        case QOperatingSystemVersion::MacOS:
            rx.setPattern("KLog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?.dmg");
            filterString = QString("KLog");
            fileExtension = QString(".dmg");
              //qDebug() << "MainWindow::checkUpdates - macOS" ;
        break;
        default:
            rx.setPattern("klog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?.tar.gz");
            filterString = QString("klog");
            fileExtension = QString(".tar.gz");
        // should never be reached
        break;
    }

    //qDebug() << "SoftwareUpdate::checkUpdates: Before entering the while"<< QT_ENDL;

    while (!data->atEnd())
    {
          //qDebug() << "SoftwareUpdate::checkUpdates: In the while"<< QT_ENDL;
        stringList.clear();
        klogStringList.clear();
        line.clear();
        line = data->readLine();
        //qDebug() << "SoftwareUpdate::checkUpdates: line: " << line;
        if ((line.contains("browser_download_url")) || (line.contains("tarball_url")))
        {
            //qDebug() << "SoftwareUpdate::checkUpdates: line contains browser_download_url or tarball_url ";
            repositoryFound = true;
            stringList << line.split(",", QT_SKIP);
            klogStringList << stringList.filter(filterString);
            release = findMatch (rx, fileExtension, klogStringList);
            if (release.length ()>0)
            {
               updateNeeded(release);
            }
            //qDebug() << "SoftwareUpdate::checkUpdates: " << line;
        }
    }
   //qDebug() << "SoftwareUpdate::checkUpdates:Latest/Actual: " << latestVersion << "/" << klogVersion;
    if (latestVersion > klogVersion)
    {
        emit updateNeededSignal (true);
          //qDebug() << "SoftwareUpdate::checkUpdates: signal true";
        return true;
    }
    else
    {
          //qDebug() << "SoftwareUpdate::checkUpdates: signal  alse 1";
        return false;
    }
}

QString SoftwareUpdate::findMatch(QRegularExpression rx, const QString &fileExtension, const QStringList &data)
{
    QString release = QString();
    foreach (QString str, data)
    {
          //qDebug() << "SoftwareUpdate::findMatch klog: " << str;
          //qDebug() << "SoftwareUpdate::findMatch klog - chopped: " << str;

           if (rx.match(str).hasMatch())
           {
               //qDebug() << "SoftwareUpdate::findMatch: MATCH: " << str;
                release = str.section("-",1);
               //qDebug() << "SoftwareUpdate::findMatch: MATCH-1: " << str;
                release = release.section(fileExtension, 0, 0);
               //qDebug() << "SoftwareUpdate::findMatch: MATCH-2: " << str;
                return release;
           }
           else
           {
               //qDebug() << "SoftwareUpdate::findMatch: DOES NOT MATCH: " << str;
           }
    }
    return release;
}

void SoftwareUpdate::updateNeeded(const QString &_newVer)
{
   //qDebug() << "SoftwareUpdate::updateNeeded: new: " << _newVer ;
   //qDebug() << "SoftwareUpdate::updateNeeded: cur: " << latestVersion ;

    if (latestVersion< _newVer)
    {
        latestVersion = _newVer;
    }
      //qDebug() << "SoftwareUpdate::updateNeeded - KLogVersion/latestVersion/newver: "<< klogVersion <<"/"<< latestVersion << "/"<<_newVer ;
}

void SoftwareUpdate::needToUpdate(bool _showWithoutVersion)
{ // This is used to connect to the main server URL.
  // If _showWithoutVersion is false: We are checking for new versions at KLog start: No message should be shown if no new version is found.
  // If _showWithoutVersion is true: The user is manually asking to check. A message should is shown if no new version is found.

      //qDebug() << "SoftwareUpdate::needToUpdate (current version: " << klogVersion  << ")" ;
    messageShown = _showWithoutVersion;
    setVersion(klogVersion);
    //setTheURL("http://download.klog.xyz");
    connectToURL("https://api.github.com/repos/ea4k/klog/releases/latest");
      //qDebug() << "SoftwareUpdate::neededToUpdate - END";
}

void SoftwareUpdate::connectToURL(const QString &_url)
{
    // This is where the connection takes place.... so first connection may be the main URL but it launches connection after redirections
    //qDebug() << "SoftwareUpdate::connectToURL: " << _url;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->get(QNetworkRequest(QUrl(_url)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
    //qDebug() << "SoftwareUpdate::conectToURL - END";
}

void SoftwareUpdate::setHeader()
{
    //qDebug() << Q_FUNC_INFO;
    QString ver = util->getAgent(klogVersion);
    //qDebug() << "SoftwareUpdate::setHeader - ver: " << ver;
    if (callsign.length()>2)
    {
        ver = ver + "-" + callsign;
    }
    QByteArray str;
    str.clear();
    str.append(ver.toUtf8 ());
    //qDebug() << "SoftwareUpdate::setHeader: " << str;
    request.setRawHeader("User-Agent", str);
    //qDebug() << "SoftwareUpdate::setHeader - END" ;
}

void SoftwareUpdate::addCall(const QString &_call)
{
      //qDebug() << "SoftwareUpdate::addCall: " << _call;
    if (_call.length()>2)
    {
        //callsign = _call;
        setHeader();
    }
      //qDebug() << "SoftwareUpdate::addCall - END";
}
