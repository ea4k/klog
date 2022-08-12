/***************************************************************************
                          downloadcty.cpp  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
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

#include "downloadcty.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

DownLoadCTY::DownLoadCTY(const QString &_klogDir, const QString &_klogVersion) : QObject(0)
{
    //qDebug() << "DownLoadCTY::DownLoadCTY(): " << _klogDir << QT_ENDL;
    util = new Utilities(Q_FUNC_INFO);
    url = new QUrl;
    klogDir = _klogDir;
    result = -1;  // Error unknown

    manager = new QNetworkAccessManager;
    request = new QNetworkRequest;
    //request->setUrl(QUrl("https://www.country-files.com/cty/cty.csv"));
    request->setUrl(QUrl("https://www.country-files.com/bigcty/cty.csv"));
    QString ver = "KLog"+_klogVersion;
    QByteArray str;
    str.clear();
    str.append(util->getAgent(_klogVersion).toUtf8());

    request->setRawHeader("User-Agent", str);
    //request->setHeader(QNetworkRequest::UserAgentHeader, str);

     //qDebug() << "DownLoadCTY::DownLoadCTY() - UserAgent: " <<  request->rawHeader("QNetworkRequest::UserAgentHeader") << QT_ENDL;

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
     //qDebug() << "DownLoadCTY::DownLoadCTY(): - END"  << QT_ENDL;
}

DownLoadCTY::~DownLoadCTY()
{
    delete(util);
    delete(request);
    delete(manager);
    delete(url);

   //qDebug() << "DownLoadCTY::~DownLoadCTY"  << QT_ENDL;
}



 void DownLoadCTY::slotDownloadFinished(QNetworkReply *reply)
{
   //qDebug() << "DownLoadCTY::slotDownloadFinished"  << QT_ENDL;
   emit downloadStopped();
   QUrl url = reply->url();
   //qDebug() << "DownLoadCTY::slotDownloadFinished - URL: " << url.toString()  << QT_ENDL;

   QMessageBox msgBox;
   QString aux;
   aux.clear();

   if (reply->error()) {

       //fprintf(stderr, "Download of %s failed: %s\n",
       //        url.toEncoded().constData(),
       //        qPrintable(reply->errorString()));


       //errorCode = query.lastError().nativeErrorCode();

       msgBox.setIcon(QMessageBox::Warning);
       aux = tr("Download of cty.csv failed with the following error code: ");
       msgBox.setText(aux + reply->errorString());
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.setDefaultButton(QMessageBox::Ok);
       msgBox.exec();


   } else {
       QString filename = saveFileName(url);
       if (saveToDisk(filename, reply))
       {
           msgBox.setIcon(QMessageBox::Information);
           aux = tr("Download of cty.csv done.");
           msgBox.setText(aux);
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.setDefaultButton(QMessageBox::Ok);
           msgBox.exec();
           emit actionReturnDownload(QNetworkReply::NoError);
       }
           //printf("Download of %s succeeded (saved to %s)\n",
           //       url.toEncoded().constData(), qPrintable(filename));
   }

   reply->deleteLater();

   emit done(true);

}

int DownLoadCTY::download()
{
   //qDebug() << "DownLoadCTY::download..." << QT_ENDL;

    manager->get(*request);
    return 1;
}

void DownLoadCTY::slotDownloadProgress(qint64 received, qint64 total) {
   //qDebug() << "DownLoadCTY::slotDownloadProgress: " << QT_ENDL;

    //qDebug() << "DownLoadCTY::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << QT_ENDL;
     //qDebug() << received << total;

    emit actionShowProgres(received, total);
}

void DownLoadCTY::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
    //qDebug() << "DownLoadCTY::slotErrorManagement: " << QString::number(networkError) << QT_ENDL;

    result = networkError;

    if (result == QNetworkReply::NoError)
    {
        //qDebug() << "DownLoadCTY::downloadFinished: No error" << QT_ENDL;
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
         //qDebug() << "DownLoadCTY::downloadFinished: Host not found" << QT_ENDL;
    }
    else
    {
        //qDebug() << "DownLoadCTY::downloadFinished: ERROR: " << QString::number(result) << QT_ENDL;
    }

    emit actionError(result);
}

QString DownLoadCTY::saveFileName(const QUrl &url)
{
    //qDebug() << "DownLoadCTY::saveFileName" << QT_ENDL;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();
    QMessageBox msgBox;
    QString aux;
    aux.clear();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {

        msgBox.setIcon(QMessageBox::Warning);
        aux = tr("There is already a cty.csv file in the folder but it will be replaced with the new one.");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();


        // already exists, don't overwrite
        //int i = 0;
        //basename += '.';
        //while (QFile::exists(basename + QString::number(i)))
        //    ++i;

        //basename += QString::number(i);
    }

    return basename;
}

bool DownLoadCTY::saveToDisk(const QString &filename, QIODevice *data)
{
     //qDebug() << "DownLoadCTY::saveToDisk: " << filename << QT_ENDL;
    QFile file(filename);
    QMessageBox msgBox;
    QString aux;
    aux.clear();
    if (!file.open(QIODevice::WriteOnly)) { /* Flawfinder: ignore */

        msgBox.setIcon(QMessageBox::Warning);
        aux = tr("Could not open %1 for writing").arg(filename);
        //aux = tr("Could not open ") + filename + tr(" for writing.");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();


        //fprintf(stderr, "Could not open %s for writing: %s\n",
         //       qPrintable(filename),
         //       qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

