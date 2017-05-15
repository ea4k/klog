#include "softwareupdate.h"

SoftwareUpdate::SoftwareUpdate(const QString _klogVersion) : QObject(0)
{
    //qDebug() << "SoftwareUpdate::SoftwareUpdate(): " << _klogDir << endl;
    util = new Utilities;
    updateDialog = new SoftwareUpdateDialog();
    updateDialog->setVersion(_klogVersion);
    toUpdate = false;
    url = new QUrl;
    //klogDir = _klogDir;
    klogVersion = _klogVersion;
    //latestVersion = klogVersion;
    latestVersion = "0.0";
    callsign = QString();
    result = -1;  // Error unknown
    //reply = new QNetworkReply;
    manager = new QNetworkAccessManager(this);
     //request = new QNetworkRequest(this);    
    //request.setUrl(QUrl("http://localhost"));
    //request.setUrl(QUrl("https://download.savannah.gnu.org/releases/klog/"));
    //request.setUrl(QUrl("http://www.klog.xyz/download"));
    //setTheURL("http://www.klog.xyz/download");
    request.setUrl(QUrl("http://download.klog.xyz"));

    setHeader();

}

void SoftwareUpdate::setTheURL(QString _url)
{
   //qDebug() << "SoftwareUpdate::setTheURL: " << _url << endl;
 request.setUrl(QUrl(_url));
}


SoftwareUpdate::~SoftwareUpdate()
{

}

void SoftwareUpdate::slotReadyRead()
{
   //qDebug() << "SoftwareUpdate::slotReadyRead: " << endl;
}

void SoftwareUpdate::slotError(int _p)
{
    //qDebug() << "SoftwareUpdate::slotError: " << endl;

}

void SoftwareUpdate::slotDownloadFinished(QNetworkReply *reply)
{
//qDebug() << "SoftwareUpdate::slotDownloadFinished"  << endl;

  QUrl url = reply->url();
 //qDebug() << "SoftwareUpdate::slotDownloadFinished - URL: " << url.toString()  << endl;

  QMessageBox msgBox;
  QString aux;
  aux.clear();
  QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

  if (reply->error()) {
      //qDebug() << "SoftwareUpdate::slotDownloadFinished: reply error"  << endl;
/*
      fprintf(stderr, "Updates %s failed: %s\n",
              url.toEncoded().constData(),
              qPrintable(reply->errorString()));


      //errorCode = query.lastError().number();

      msgBox.setIcon(QMessageBox::Warning);
      aux = tr("The following error code was received when trying to check for updates: ");
      msgBox.setText(aux + reply->errorString());
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      int ret = msgBox.exec();

*/

  } else if (!redirectionTarget.isNull()) {
         QUrl newUrl = url.resolved(redirectionTarget.toUrl());
     //qDebug() << "SoftwareUpdate::slotDownloadFinished: Redirect: " << newUrl.toString() << endl;
               url = newUrl;
               reply->deleteLater();
               //file->open(QIODevice::WriteOnly);
               //file->resize(0);
               setTheURL(url.toString());
               needToUpdate();
               return;
  } else {
      //qDebug() << "SoftwareUpdate::slotDownloadFinished: no reply error"  << endl;
      //QString filename = saveFileName(url);
      if (checkUpdates(reply))
      {
          //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates true"  << endl;
          updateDialog->setVersion(latestVersion);
          updateDialog->show();
          latestVersion = klogVersion;
/*
          msgBox.setIcon(QMessageBox::Information);
          aux = tr("There is a new KLog version, please update!");
          msgBox.setText(aux);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          int ret = msgBox.exec();
*/
      }
      else
      {
          //qDebug() << "SoftwareUpdate::slotDownloadFinished:  checkupdates false"  << endl;
/*
          msgBox.setIcon(QMessageBox::Information);
          aux = tr("You already have the latest version of KLog!");
          msgBox.setText(aux);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          int ret = msgBox.exec();
*/
      }
          //printf("Download of %s succeeded (saved to %s)\n",
          //       url.toEncoded().constData(), qPrintable(filename));
  }

  reply->deleteLater();
  //manager->deleteResource(request);

    //qDebug() << "SoftwareUpdate::slotDownloadFinished end"  << endl;
  //emit done();

}



bool SoftwareUpdate::checkUpdates(QIODevice *data)
{    
   //qDebug() << "SoftwareUpdate::checkUpdates: " << QString::number(data->size()) << endl;
    QString line, release;
    QStringList stringList, klogStringList;

    QRegularExpression rx("href=\"klog-(\\d\\.)+tar.gz");

    if (rx.isValid())
    {
       //qDebug() << "SoftwareUpdate::checkUpdates: RX is VALID"<< endl;
    }
    else
    {
      //qDebug() << "SoftwareUpdate::checkUpdates: RX is NOT VALID"<< endl;
    }
   //qDebug() << "SoftwareUpdate::checkUpdates: Before entering the while"<< endl;

    while (!data->atEnd())
    {
       //qDebug() << "SoftwareUpdate::checkUpdates: In the while"<< endl;
        stringList.clear();
        klogStringList.clear();
        line.clear();
        line = data->readLine();
       //qDebug() << "SoftwareUpdate::checkUpdates: line: " << line << endl;
        if (line.contains("klog-"))
        {
            stringList << line.split(">", QString::SkipEmptyParts);
            klogStringList << stringList.filter("klog");
            foreach (const QString &str, klogStringList)
            {
                 //qDebug() << "SoftwareUpdate::checkUpdates klog: " << str << endl;
                   if (rx.match(str).hasMatch())
                   {
                      //qDebug() << "SoftwareUpdate::checkUpdates: MATCH: " << str << endl;
                       release = str.section("-",1);                       
                       release = release.section("\.tar.gz", 0, 0);                       
                       updateNeeded(release);
                   }
                   else
                   {
                      //qDebug() << "SoftwareUpdate::checkUpdates: DOES NOT MATCH: " << str << endl;
                   }
            }
           //qDebug() << "SoftwareUpdate::checkUpdates: " << line << endl;
        }
    }
   //qDebug() << "SoftwareUpdate::checkUpdates:Latest/Actual: " << latestVersion <<"/" << klogVersion << endl;
    if (latestVersion > klogVersion)
    {
        emit updateNeededSignal (true);
        return true;
    }
    else
    {
        emit updateNeededSignal (false);
        return false;
    }

    emit updateNeededSignal (false);
    return false;


}

void SoftwareUpdate::updateNeeded(QString _newVer)
{
   //qDebug() << "SoftwareUpdate::updateNeeded: " << _newVer  << endl;

    if (klogVersion < _newVer)
    {
       //qDebug() << "SoftwareUpdate::updateNeeded TRUE: " << _newVer  << endl;
        if (latestVersion < _newVer)
        {
            latestVersion = _newVer;
        }

    }
    else
    {
       //qDebug() << "SoftwareUpdate::updateNeeded - FALSE " << _newVer  << endl;
    }
}

void SoftwareUpdate::needToUpdate()
{
   //qDebug() << "SoftwareUpdate::needToUpdate (current version: " << klogVersion  << ")"  << endl;

    QNetworkReply *reply = manager->get(request);



    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));

}

void SoftwareUpdate::setHeader()
{
    QString ver = util->getAgent(klogVersion);
    if (callsign.length()>2)
    {
        ver = ver + "-" + callsign;
    }
    QByteArray str;
    str.clear();
    str.append(ver);

    request.setRawHeader("User-Agent", str);
}

void SoftwareUpdate::addCall(const QString _call)
{
    if (_call.length()>2)
    {
        callsign = _call;
        setHeader();
    }

}
