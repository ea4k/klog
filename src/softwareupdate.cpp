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
//#include <QDebug>

SoftwareUpdate::SoftwareUpdate(const QString &_klogVersion) : QObject(nullptr)
{
      //qDebug() << "SoftwareUpdate::SoftwareUpdate(): " << _klogVersion << Qt::endl;
    util = new Utilities;

    updateDialog = new SoftwareUpdateDialog();
    latestVersion = "0.0";
    //updateDialog->setVersion(_klogVersion, fale);
    //toUpdate = false;
    repositoryFound = false;
    url = new QUrl;
    //klogDir = _klogDir;
    setVersion(_klogVersion);
    //klogVersion = _klogVersion;
    //latestVersion = "0.0";
    callsign = QString();
    OSString = QString();
    //result = -1;  // Error unknown
    //reply = new QNetworkReply;
    //manager = new QNetworkAccessManager(this);
     //request = new QNetworkRequest(this);
    //request.setUrl(QUrl("http://localhost"));
    //request.setUrl(QUrl("https://download.savannah.gnu.org/releases/klog/"));
    //request.setUrl(QUrl("http://www.klog.xyz/download"));
    //setTheURL("http://download.klog.xyz/win");
    //request.setUrl(QUrl("http://download.klog.xyz"));
    messageShown = false;
    OSVersion = QOperatingSystemVersion::currentType();
      //qDebug() << "SoftwareUpdate::SoftwareUpdate OSVersion:: " << QString::number(OSVersion)  << Qt::endl;
    findOS(QOperatingSystemVersion::currentType());
    setHeader();
      //qDebug() << "SoftwareUpdate::SoftwareUpdate(): - END"  << Qt::endl;

}
void SoftwareUpdate::findOS(const int _os)
{

      //qDebug() << "SoftwareUpdate::findOS(): " << QString::number(_os)  << Qt::endl;
    switch (_os)
    {
        case QOperatingSystemVersion::MacOS:
            OSString = QString("osx");
              //qDebug() << "SoftwareUpdate::findOS(): macOS"  << Qt::endl;
        break;
        case QOperatingSystemVersion::Windows:
              //qDebug() << "SoftwareUpdate::findOS(): Windows"  << Qt::endl;
            OSString = QString("win");
        break;
        case QOperatingSystemVersion::Unknown:
              //qDebug() << "SoftwareUpdate::findOS(): Unknown"  << Qt::endl;
            OSString = QString();
        break;
              //qDebug() << "SoftwareUpdate::findOS(): OTHER"  << Qt::endl;
        default:

        // should never be reached
        break;
    }
      //qDebug() << "SoftwareUpdate::findOS() -  END"  << Qt::endl;
}

SoftwareUpdate::~SoftwareUpdate()
{

}
/*
void SoftwareUpdate::setTheURL(QString _url)
{
      //qDebug() << "SoftwareUpdate::setTheURL: " << _url << Qt::endl;
//    request.setUrl(QUrl(_url));
}
*/
void SoftwareUpdate::setVersion(const QString &_klogVersion)
{
      //qDebug() << "SoftwareUpdate::setVersion: " << _klogVersion << Qt::endl;
    klogVersion = _klogVersion;
    latestVersion = klogVersion;

    setHeader();
      //qDebug() << "SoftwareUpdate::setVersion: END "  << Qt::endl;
}

void SoftwareUpdate::slotReadyRead()
{
      //qDebug() << "SoftwareUpdate::slotReadyRead: " << Qt::endl;
}

/*
void SoftwareUpdate::slotError(int _p)
{
#ifdef QT_DEBUG
  //qDebug() << Q_FUNC_INFO << ": " << QString::number(_p) << Qt::endl;
#else
#endif
}
*/

void SoftwareUpdate::slotDownloadFinished(QNetworkReply *reply)
{
      //qDebug() << "SoftwareUpdate::slotDownloadFinished"  << Qt::endl;

    QUrl url = reply->url();
      //qDebug() << "SoftwareUpdate::slotDownloadFinished - URL: " << url.toString()  << Qt::endl;
    if (url.toString().length()< QString("https://api.github.com/repos/ea4k/klog/releases/latest").length())
    //if (url.toString().length()< QString("https://download.klog.xyz").length())
    {
          //qDebug() << "SoftwareUpdate::slotDownloadFinished - URL: URL too short"  << Qt::endl;
        return;
    }

    //QMessageBox msgBox;
    //QString aux;
    //aux.clear();
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error())
    {
          //qDebug() << "SoftwareUpdate::slotDownloadFinished: reply error: " << QString::number(reply->error())  << Qt::endl;
    }
    else if (!redirectionTarget.isNull())
    {
        repositoryFound = false;
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
          //qDebug() << "SoftwareUpdate::slotDownloadFinished: Redirect: " << newUrl.toString() << Qt::endl;
        url = newUrl;
        reply->deleteLater();
        //request.setUrl(QUrl(url.toString()));
        //setTheURL(url.toString());
        connectToURL(url.toString());
          //qDebug() << "SoftwareUpdate::slotDownloadFinished - end After connect" << Qt::endl;
        return;
    }
    else
    {
          //qDebug() << "SoftwareUpdate::slotDownloadFinished: no redirection"  << Qt::endl;
        if (checkUpdates(reply))
        {
              //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates true"  << Qt::endl;
            if (repositoryFound)
            {
                  //qDebug() << "SoftwareUpdate::slotDownloadFinished repository found"  << Qt::endl;
                if(latestVersion>klogVersion)
                {
                      //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates should update!"  << Qt::endl;
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
                  //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates false!"  << Qt::endl;
                updateDialog->setVersion(latestVersion, false);
                updateDialog->show();
            }

             //qDebug() << "SoftwareUpdate::slotDownloadFinished:  checkupdates false"  << Qt::endl;

        }
    }

    reply->deleteLater();
    //manager->deleteLater();
      //qDebug() << "SoftwareUpdate::slotDownloadFinished end"  << Qt::endl;
}

bool SoftwareUpdate::checkUpdates(QIODevice *data)
{
    // Checks if there is a new version in the repository

    //qDebug() << "SoftwareUpdate::checkUpdates: " << QString::number(data->size()) << Qt::endl;
    QString line, release;
    QStringList stringList, klogStringList;

    QRegularExpression rx;
    QString filterString;
    QString fileExtension;
    //int SO = 2; // 1 = linux, 2 = windows, 3 = macOS
    switch (OSVersion)
    {
        case QOperatingSystemVersion::Unknown:
              //qDebug() << "MainWindow::checkUpdates - Unknown"  << Qt::endl;
            rx.setPattern("klog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?.tar.gz");
            filterString = QString("klog");
            fileExtension = QString(".tar.gz");
        break;
        case QOperatingSystemVersion::Windows:
              //qDebug() << "MainWindow::checkUpdates - Windows"  << Qt::endl;
           // rx.setPattern("KLog-(\\d+\\.)?(\\d+\\.)?(\\d+)?(-\\d+)?(\\.)?-windows-installer.exe");
            rx.setPattern("KLog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?-windows-installer.exe");
            filterString = QString("KLog");
            fileExtension = QString("-windows-installer.exe");
        break;
        case QOperatingSystemVersion::MacOS:
            //rx.setPattern("KLog-(\\d+\\.)?(\\d+\\.)?(\\d+)?(-\\d+)?(\\.)?dmg");
            rx.setPattern("KLog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?.dmg");
            filterString = QString("KLog");
            fileExtension = QString(".dmg");
              //qDebug() << "MainWindow::checkUpdates - macOS"  << Qt::endl;
        break;
        default:
            rx.setPattern("klog-(\\d+)(\\.\\d+)?(\\.\\d+)?(\\.\\d+)?(-RC\\d+)?.tar.gz");
            filterString = QString("klog");
            fileExtension = QString(".tar.gz");
        // should never be reached
        break;
    }

      //qDebug() << "SoftwareUpdate::checkUpdates: Before entering the while"<< Qt::endl;

    while (!data->atEnd())
    {
          //qDebug() << "SoftwareUpdate::checkUpdates: In the while"<< Qt::endl;
        stringList.clear();
        klogStringList.clear();
        line.clear();
        line = data->readLine();
        //qDebug() << "SoftwareUpdate::checkUpdates: line: " << line << Qt::endl;
        //if ((line.contains("KLog-")) || (line.contains("klog-")))
        if ((line.contains("browser_download_url")) || (line.contains("tarball_url")))
        {
            //qDebug() << "SoftwareUpdate::checkUpdates: line contains browser_download_url or tarball_url " << Qt::endl;
            repositoryFound = true;
            stringList << line.split(",", Qt::SkipEmptyParts);
            klogStringList << stringList.filter(filterString);
            release = findMatch (rx, fileExtension, klogStringList);
            if (release.length ()>0)
            {
               updateNeeded(release);
            }
            /*
            foreach (QString str, klogStringList)
            {
                  //qDebug() << "SoftwareUpdate::checkUpdates klog: " << str << Qt::endl;
                   //str = str.chopped(3);
                     //qDebug() << "SoftwareUpdate::checkUpdates klog - chopped: " << str << Qt::endl;

                   if (rx.match(str).hasMatch())
                   {
                       //qDebug() << "SoftwareUpdate::checkUpdates: MATCH: " << str << Qt::endl;
                        release = str.section("-",1);
                       //qDebug() << "SoftwareUpdate::checkUpdates: MATCH-1: " << str << Qt::endl;
                        release = release.section(fileExtension, 0, 0);
                       //qDebug() << "SoftwareUpdate::checkUpdates: MATCH-2: " << str << Qt::endl;
                       //release = release.section("^.*([.]tar[.]gz)", 0, 0);
                       //release = release.section("^.*\\.(tar.gz)?", 0, 0);
                        updateNeeded(release);
                   }
                   else
                   {
                       //qDebug() << "SoftwareUpdate::checkUpdates: DOES NOT MATCH: " << str << Qt::endl;
                   }
            }
            */
            //qDebug() << "SoftwareUpdate::checkUpdates: " << line << Qt::endl;
        }
    }
   //qDebug() << "SoftwareUpdate::checkUpdates:Latest/Actual: " << latestVersion << "/" << klogVersion << Qt::endl;
    if (latestVersion > klogVersion)
    {
        emit updateNeededSignal (true);
          //qDebug() << "SoftwareUpdate::checkUpdates: signal true" << Qt::endl;
        return true;
    }
    else
    {
        //emit updateNeededSignal (false);
          //qDebug() << "SoftwareUpdate::checkUpdates: signal  alse 1" << Qt::endl;
        return false;
    }

    //emit updateNeededSignal (false);
      //qDebug() << "SoftwareUpdate::checkUpdates: signal false 2" << Qt::endl;
    //return false;
}

QString SoftwareUpdate::findMatch(QRegularExpression rx, const QString &fileExtension, const QStringList &data)
{
    QString release = QString();
    foreach (QString str, data)
    {
          //qDebug() << "SoftwareUpdate::findMatch klog: " << str << Qt::endl;
           //str = str.chopped(3);
             //qDebug() << "SoftwareUpdate::findMatch klog - chopped: " << str << Qt::endl;

           if (rx.match(str).hasMatch())
           {
               //qDebug() << "SoftwareUpdate::findMatch: MATCH: " << str << Qt::endl;
                release = str.section("-",1);
               //qDebug() << "SoftwareUpdate::findMatch: MATCH-1: " << str << Qt::endl;
                release = release.section(fileExtension, 0, 0);
               //qDebug() << "SoftwareUpdate::findMatch: MATCH-2: " << str << Qt::endl;
                return release;
           }
           else
           {
               //qDebug() << "SoftwareUpdate::findMatch: DOES NOT MATCH: " << str << Qt::endl;
           }
    }
    return release;
}

void SoftwareUpdate::updateNeeded(const QString &_newVer)
{
   //qDebug() << "SoftwareUpdate::updateNeeded: new: " << _newVer  << Qt::endl;
   //qDebug() << "SoftwareUpdate::updateNeeded: cur: " << latestVersion  << Qt::endl;

    if (latestVersion< _newVer)
    {
        latestVersion = _newVer;
    }
/*
    if (klogVersion < _newVer)
    {
         //qDebug() << "SoftwareUpdate::updateNeeded TRUE: " << _newVer  << Qt::endl;
        if

    }
    else
    {
         //qDebug() << "SoftwareUpdate::updateNeeded - FALSE " << _newVer  << Qt::endl;
    }
*/

      //qDebug() << "SoftwareUpdate::updateNeeded - KLogVersion/latestVersion/newver: "<< klogVersion <<"/"<< latestVersion << "/"<<_newVer  << Qt::endl;
}

void SoftwareUpdate::needToUpdate(bool _showWithoutVersion)
{ // This is used to connect to the main server URL.
  // If _showWithoutVersion is false: We are checking for new versions at KLog start: No message should be shown if no new version is found.
  // If _showWithoutVersion is true: The user is manually asking to check. A message should is shown if no new version is found.

      //qDebug() << "SoftwareUpdate::needToUpdate (current version: " << klogVersion  << ")"  << Qt::endl;
    messageShown = _showWithoutVersion;
    setVersion(klogVersion);
    //setTheURL("http://download.klog.xyz");
    connectToURL("https://api.github.com/repos/ea4k/klog/releases/latest");
    //connectToURL("https://download.klog.xyz/" + OSString);
      //qDebug() << "SoftwareUpdate::neededToUpdate - END" << Qt::endl;
}


void SoftwareUpdate::connectToURL(const QString &_url)
{
    // This is where the connection takes place.... so first connection may be the main URL but it launches connection after redirections
      //qDebug() << "SoftwareUpdate::connectToURL: " << _url << Qt::endl;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    //request.setUrl(QUrl(_url));

      manager->get(QNetworkRequest(QUrl(_url)));

      QNetworkReply *reply = manager->get(request);

    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
      //qDebug() << "SoftwareUpdate::conectToURL - END" << Qt::endl;
}

void SoftwareUpdate::setHeader()
{
      //qDebug() << "SoftwareUpdate::setHeader" << Qt::endl;
    QString ver = util->getAgent(klogVersion);
      //qDebug() << "SoftwareUpdate::setHeader - ver: " << ver << Qt::endl;
    if (callsign.length()>2)
    {
        ver = ver + "-" + callsign;
    }
    QByteArray str;
    str.clear();
    str.append(ver.toUtf8 ());

      //qDebug() << "SoftwareUpdate::setHeader: " << str << Qt::endl;
    request.setRawHeader("User-Agent", str);
      //qDebug() << "SoftwareUpdate::setHeader - END"  << Qt::endl;
}

void SoftwareUpdate::addCall(const QString &_call)
{
      //qDebug() << "SoftwareUpdate::addCall: " << _call << Qt::endl;
    if (_call.length()>2)
    {
        callsign = _call;
        setHeader();
    }
      //qDebug() << "SoftwareUpdate::addCall - END" << Qt::endl;

}
