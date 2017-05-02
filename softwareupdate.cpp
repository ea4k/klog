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
    latestVersion = klogVersion;
    callsign = QString();
    result = -1;  // Error unknown
    //reply = new QNetworkReply;
    manager = new QNetworkAccessManager(this);
     //request = new QNetworkRequest(this);    
    //request.setUrl(QUrl("http://localhost"));
    //request.setUrl(QUrl("https://download.savannah.gnu.org/releases/klog/"));
    request.setUrl(QUrl("http://download.klog.xyz/redirect"));

    setHeader();

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

  if (reply->error()) {
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

  } else {
      //QString filename = saveFileName(url);
      if (checkUpdates(reply))
      {
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


  //emit done();

}

bool SoftwareUpdate::checkUpdates(QIODevice *data)
{    
   //qDebug() << "SoftwareUpdate::checkUpdates: "  << endl;
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

    while (!data->atEnd())
    {
        stringList.clear();
        klogStringList.clear();
        line.clear();
        line = data->readLine();
        if (line.contains("klog-"))
        {
            stringList << line.split(">", QString::SkipEmptyParts);
            klogStringList << stringList.filter("klog");
            foreach (const QString &str, klogStringList)
            {
                  //qDebug() << "SoftwareUpdate::checkUpdates klog: " << str << endl;
                   if (rx.match(str).hasMatch())
                   {                      
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
