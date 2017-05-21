#include "downloadcty.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

DownLoadCTY::DownLoadCTY(const QString _kontestDir, const QString _klogVersion) : QObject(0)
{    
   //qDebug() << "DownLoadCTY::DownLoadCTY(): " << _kontestDir << endl;
    util = new Utilities;
    url = new QUrl;
    kontestDir = _kontestDir;
    result = -1;  // Error unknown

    manager = new QNetworkAccessManager;
    request = new QNetworkRequest;
    //request->setUrl(QUrl("http://www.country-files.com/cty/cty.csv"));
    request->setUrl(QUrl("http://www.country-files.com/bigcty/cty.csv"));
    QString ver = "KLog"+_klogVersion;
    QByteArray str;
    str.clear();
    str.append(util->getAgent(_klogVersion));
    //str.append(_klogVersion);

    //request.setUrl(QUrl("http://qt.nokia.com"));
    request->setRawHeader("User-Agent", str);
    //request->setHeader(QNetworkRequest::UserAgentHeader, str);

    //qDebug() << "DownLoadCTY::DownLoadCTY() - UserAgent: " <<  request->rawHeader("QNetworkRequest::UserAgentHeader") << endl;

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));

}

DownLoadCTY::~DownLoadCTY()
{
  //qDebug() << "DownLoadCTY::~DownLoadCTY"  << endl;
}



 void DownLoadCTY::slotDownloadFinished(QNetworkReply *reply)
{
  //qDebug() << "DownLoadCTY::slotDownloadFinished"  << endl;

   QUrl url = reply->url();
  //qDebug() << "DownLoadCTY::slotDownloadFinished - URL: " << url.toString()  << endl;

   QMessageBox msgBox;
   QString aux;
   aux.clear();

   if (reply->error()) {

       //fprintf(stderr, "Download of %s failed: %s\n",
       //        url.toEncoded().constData(),
       //        qPrintable(reply->errorString()));


       //errorCode = query.lastError().number();

       msgBox.setIcon(QMessageBox::Warning);
       aux = tr("Download of the CTY.CSV failed with the following error code: ");
       msgBox.setText(aux + reply->errorString());
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.setDefaultButton(QMessageBox::Ok);
       int ret = msgBox.exec();


   } else {
       QString filename = saveFileName(url);
       if (saveToDisk(filename, reply))
       {
           msgBox.setIcon(QMessageBox::Information);
           aux = tr("Download of the CTY.CSV done.");
           msgBox.setText(aux);
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.setDefaultButton(QMessageBox::Ok);
           int ret = msgBox.exec();
           emit actionReturnDownload(QNetworkReply::NoError);
       }
           //printf("Download of %s succeeded (saved to %s)\n",
           //       url.toEncoded().constData(), qPrintable(filename));
   }

   reply->deleteLater();

   emit done();

}

int DownLoadCTY::download()
{
  //qDebug() << "DownLoadCTY::download..." << endl;

    manager->get(*request);
    return 1;
}

void DownLoadCTY::slotDownloadProgress(qint64 received, qint64 total) {
  //qDebug() << "DownLoadCTY::slotDownloadProgress: " << endl;

   //qDebug() << "DownLoadCTY::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << endl;
    //qDebug() << received << total;

    emit actionShowProgres(received, total);
}

void DownLoadCTY::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
   //qDebug() << "DownLoadCTY::slotErrorManagement: " << QString::number(networkError) << endl;

    result = networkError;

    if (result == QNetworkReply::NoError)
    {
       //qDebug() << "DownLoadCTY::downloadFinished: No error" << endl;
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
        //qDebug() << "DownLoadCTY::downloadFinished: Host not found" << endl;
    }
    else
    {
       //qDebug() << "DownLoadCTY::downloadFinished: ERROR: " << QString::number(result) << endl;
    }

    actionError(result);
}

QString DownLoadCTY::saveFileName(const QUrl &url)
{
   //qDebug() << "DownLoadCTY::saveFileName" << endl;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();
    QMessageBox msgBox;
    QString aux;
    aux.clear();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {

        msgBox.setIcon(QMessageBox::Warning);
        aux = tr("There is already a CTY.CSV file in the folder but it will be replaced with the new one.");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();



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
    //qDebug() << "DownLoadCTY::saveToDisk: " << filename << endl;
    QFile file(filename);
    QMessageBox msgBox;
    QString aux;
    aux.clear();
    if (!file.open(QIODevice::WriteOnly)) {

        msgBox.setIcon(QMessageBox::Warning);
        aux = tr("Could not open ") + filename + tr(" for writing.");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();


        //fprintf(stderr, "Could not open %s for writing: %s\n",
         //       qPrintable(filename),
         //       qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

