#include "elogqrzlog.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

eLogQrzLog::eLogQrzLog(DataProxy_SQLite *dp, const QString &_parentFunction, const QString &_klogVersion)
{
   //qDebug()<< QString("eLogQrzLog::eLogQrzLog (%1) ").arg(_parentFunction)  << endl;
    klogVersion = _klogVersion;
    dataProxy = dp;
    errorWhileSendingLog = false;
    sendingQSO = false;
    lastQSO = false;

    qsos.clear();
    sessionkey = QString();
    logbookkey = QString();
    onlineMessage = new OnlineMessageWidget;
    currentQSO = -1;
    manager = new QNetworkAccessManager(this);
    managerLog = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotManagerFinished(QNetworkReply*)));
    connect(managerLog, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotManagerLogFinished(QNetworkReply*)));
    uploadingFile = false;
    logged = false;
    util = new Utilities;
    serviceUrl = QUrl("https://xmldata.qrz.com/xml/current/");
    //serviceUrl = QUrl("https://xmldata.qrz.com/xml/1.31/ ");
   //qDebug()<< "eLogQrzLog::eLogQrzLog - END"  << endl;
}

eLogQrzLog::~eLogQrzLog()
{
        //qDebug()<< "eLogQrzLog::~eLogQrzLog"  << endl;
}

void eLogQrzLog::setLogBookKey(const QString &_key)
{
    //qDebug()<< "eLogQrzLog::setLogBookKey: " << _key  << endl;
    if (_key.length()>0)
    {
        logbookkey = _key;
    }
}

void eLogQrzLog::parseNetworkError(QNetworkReply::NetworkError _error)
{

    QString text;
    switch (_error) {
        case QNetworkReply::NoError:
        break;
    case QNetworkReply::HostNotFoundError:
        text = "QRZ.com: " + tr("Host not found!");
    break;
    case QNetworkReply::TimeoutError:
        text = "QRZ.com: " + tr("Timeout error!");
    break;
    default:
        text = "QRZ.com: " + tr("Undefined error number (#%1)").arg(_error);
    }

    //return text;

    int i = QMessageBox::warning(nullptr, tr("KLog - QRZ.com"),
                                           tr("We have received the following error from QRZ.com (%1)").arg(result) + "\n" +
                                              text,
                                           QMessageBox::Ok);

}

void eLogQrzLog::slotManagerLogFinished(QNetworkReply *data)
{
    //qDebug()<< "eLogQrzLog::slotLogManagerFinished"  << endl;
    sendingQSO = false;
    result = data->error();
    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();
    //qDebug() << "eLogQrzLog::slotManagerLogFinished: Received: " << sdata;

    QString text = QString();

    //qDebug() << "eLogQrzLog::slotManagerLogFinished - 00010" << endl;
    if (result == QNetworkReply::NoError)
    {
        QString dataReply(sdata);
        parseAppAnswer(1, dataReply);
       //qDebug()<< "eLogQrzLog::slotManageLogFinished - NO ERROR" << endl;
    }
    else {
        parseNetworkError(result);

        //emit disableQRZAction(true);
    }

    if (lastQSO)
    {
        lastQSO = false;
        sendSignal(result, qsos);
    }

    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result = " << QString::number(result) << endl;
    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result Text = " << text << endl;
    emit showMessage(text);

}

void eLogQrzLog::parseXMLAnswer(QXmlStreamReader &xml)
{
    //qDebug() << "eLogQrzLog::parseXMLAnswer: " << xml.text() << endl;
    QString tdata = QString();

    while (!xml.atEnd())
    {
        tdata.clear();
        QXmlStreamReader::TokenType t = xml.readNext();
        if (t == QXmlStreamReader::StartDocument)
        {
            //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read version: " << xml.documentVersion().toString() << endl;
            //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read encoding: " << xml.documentEncoding().toString() << endl;
        }

        if (t == QXmlStreamReader::StartElement)
        {
            QString name = xml.name().toString();
            //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read name: " << name << endl;
            if (name == "QRZDatabase" || name == "Session" || name == "Callsign")
            {
                //qDebug() << "eLogQrzLog::parseXMLAnswer - No data: " << name << endl;
            }
            else if (name == "Key")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: API-Key: " << tdata << endl;
                if (tdata.length()>0)
                {
                    sessionkey = tdata;
                    logged = true;
                }
                continue;
            }
            else if (name == "call")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                   //qDebug() << "eLogQrzLog::parseXMLAnswer: CALL: " << tdata << endl;
                }
                continue;
            }
            else if (name == "Error")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: Error: " << tdata << endl;
                if (tdata == "Username/password incorrect ")
                {
                    pass = QString();
                }
                emit dataFoundSignal("error", tdata);
                continue;
            }
            else if (name == "Message")
             {
                 tdata = xml.readElementText();
                 //qDebug() << "eLogQrzLog::parseXMLAnswer: Message: " << tdata << endl;
                 emit dataFoundSignal("message", tdata);
                 continue;
             }

            else if (name == "fname")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    emit dataFoundSignal("name", tdata);
                    //qDebug() << "eLogQrzLog::parseXMLAnswer: Name: " << tdata << endl;
                }
                continue;
            }
            else if (name == "addr1")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    //emit dataFoundSignal("grid", tdata);
                   //qDebug() << "eLogQrzLog::parseXMLAnswer: addr1: " << tdata << endl;
                }
                continue;
            }
            else if (name == "addr2")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    emit dataFoundSignal("qth", tdata);
                   //qDebug() << "eLogQrzLog::parseXMLAnswer: addr2: " << tdata << endl;
                }
                continue;
            }
            else if (name == "grid")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                      emit dataFoundSignal("grid", tdata);
                }
                continue;
            }
            else if (name == "qslmgr")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    emit dataFoundSignal("qslmgr", tdata);
                }
                continue;
            }
            else if (name == "Remark")
            {
                //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: "  << endl;
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: " << tdata << endl;
                }
                continue;
            }
            else
            {
                //qDebug() << "eLogQrzLog::parseXMLAnswer - name =  " << name << endl;
                //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read data: " << xml.readElementText() << endl;
            }
        }
    }
    if (xml.hasError())
    {
        //qDebug() << "eLogQrzLog::parseXMLAnswer - ERROR: " << xml.errorString()  << endl;
    }
    else if (xml.atEnd())
    {
        //qDebug() << "eLogQrzLog::parseXMLAnswer - XML END"  << endl;
    }
/*
     QString tname, tdata;
     while(!xml.atEnd() && !xml.hasError())
     {
         //qDebug() << "eLogQrzLog::parseXMLAnswer - 00012" << endl;
         QXmlStreamReader::TokenType token = xml.readNext();

         if (token == QXmlStreamReader::StartDocument)
         {
             //qDebug() << "eLogQrzLog::parseXMLAnswer - StartDocument" << endl;
             continue;
         }
         //qDebug() << "eLogQrzLog::parseXMLAnswer - 00014" << endl;
         if (token == QXmlStreamReader::StartElement)
         {
             tname = xml.name().toString();

            //qDebug() << "eLogQrzLog::parseXMLAnswer - tname = " << tname << endl;

            if (tname == "Key")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: API-Key: " << tdata << endl;
                if (tdata.length()>0)
                {
                    sessionkey = tdata;
                    logged = true;
                }
                continue;
            }
            if (tname == "Error")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: Error: " << tdata << endl;
                emit dataFoundSignal("error", tdata);
            }
            if (tname == "Message")
             {
                 tdata = xml.readElementText();
                 //qDebug() << "eLogQrzLog::parseXMLAnswer: Message: " << tdata << endl;
                 emit dataFoundSignal("message", tdata);
             }
            if (tname == "call")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: CALL: " << tdata << endl;
                  }
                  continue;
              }
            if (tname == "fname")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("name", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: Name: " << tdata << endl;
                  }
                  continue;
              }
            if (tname == "addr1")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //emit dataFoundSignal("grid", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: addr1: " << tdata << endl;
                  }
                  continue;
              }
            if (tname == "addr2")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("qth", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: addr2: " << tdata << endl;
                  }
                  continue;
              }
            if (tname == "grid")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("grid", tdata);
                  }
                  continue;
              }
            if (tname == "qslmgr")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("qslmgr", tdata);
                  }
                  continue;
              }
            if (tname == "Remark")
              {
                  //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: "  << endl;
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: " << tdata << endl;
                  }
                  continue;
              }
            if (tname == "Session")
              {
                  //qDebug() << "eLogQrzLog::parseXMLAnswer: Session: "  << endl;
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //qDebug() << "eLogQrzLog::parseXMLAnswer: Session: " << tdata << endl;
                  }
                  continue;
              }

              //qDebug() << "eLogQrzLog::parseXMLAnswer: Unknown: " << tname << endl;
         }
     }
     if (xml.hasError())
     {
           //qDebug() << "eLogQrzLog::parseXMLAnswer XML error: " << "XML error: " << xml.errorString() << endl;
     }
     else if (xml.atEnd())
     {
           //qDebug() << "eLogQrzLog::parseXMLAnswer XML END: " << "Reached end, done" << endl;
     }
     */
    //qDebug() << "eLogQrzLog::parseXMLAnswer: END of token while" << endl;
    //qDebug()<< "eLogQrzLog::parseXMLAnswer - NO ERROR" << endl;
}

void eLogQrzLog::slotManagerFinished(QNetworkReply *data)
{
    //qDebug()<< "eLogQrzLog::slotManagerFinished"  << endl;

    result = data->error();
    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();
   //qDebug() << "eLogQrzLog::slotManagerFinished: Received: " << sdata;

    QString text = QString();

   //qDebug() << "eLogQrzLog::slotManagerFinished - 00010" << endl;
    if (result == QNetworkReply::NoError)
    {
        //QXmlStreamReader reader(sdata);
        reader = new QXmlStreamReader(sdata);
        parseXMLAnswer(*reader);
    }
    else
    {
        sendSignal(result, qsos);
        //parseNetworkError(result);
    }

    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = " << QString::number(result) << endl;
    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result Text = " << text << endl;

    emit showMessage(text);

}

void eLogQrzLog::downloadProgress(qint64 received, qint64 total) {
       //qDebug()<< "eLogQrzLog::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << endl;

       //qDebug()<< received << total;
    emit actionShowProgres(received, total);
}

void eLogQrzLog::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
       //qDebug()<< "eLogQrzLog::slotErrorManagement: " << QString::number(networkError) << endl;
    result = networkError;

    if (result == QNetworkReply::NoError)
    {
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
            //qDebug()<< "eLogQrzLog::slotErrorManagement: Host not found" << endl;
    }
    else
    {
            //qDebug()<< "eLogQrzLog::slotErrorManagement: ERROR!" << endl;
    }

    //actionError(result);
}

bool eLogQrzLog::canConnect()
{
   //qDebug()<< "eLogQrzLog::canConnect: "  << endl;
   if (!logged)
   {
       //qDebug()<< "eLogQrzLog::canConnect: 10"  << endl;
       login();
       if (!logged)
       {
            //qDebug()<< "eLogQrzLog::canConnect: Not logged"  << endl;
            return false;
       }
   }
   if (sessionkey.length()<1)
   {
        //qDebug()<< "eLogQrzLog::canConnect: API not valid"  << endl;
       return false;
   }
   //qDebug()<< "eLogQrzLog::canConnect: END"  << endl;
   return true;
}

void eLogQrzLog::fetchData()
{
    //qDebug()<< "eLogQrzLog::fetchData"  << endl;
    if (!canConnect())
    {
        return;
    }
   //qDebug()<< "eLogQrzLog::checkQRZ: Preparing the query"  << endl;
    QUrlQuery params;


    params.addQueryItem("action", "fetch");
    params.addQueryItem("option", "all");
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));

    sendDataParams(params);
}

void eLogQrzLog::checkQRZ(const QString &_qrz)
{
    //qDebug()<< "eLogQrzLog::checkQRZ: " << _qrz << endl;
    if (!util->isValidCall(_qrz))
    {
        //qDebug()<< "eLogQrzLog::checkQRZ: CALL not valid"  << endl;
        return;
    }
    serviceUrl = QUrl("https://xmldata.qrz.com/xml/current/");

    if (!canConnect())
    {
        return;
    }

    //qDebug()<< "eLogQrzLog::checkQRZ: Preparing the query"  << endl;
    QUrlQuery params;
    params.addQueryItem("s", sessionkey);
    params.addQueryItem("callsign", _qrz);
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));
    sendDataParams(params);
}

int eLogQrzLog::sendQSOs(QList<int> _qsos)
{
    //qDebug()<< "eLogQrzLog::sendQSOs: QSOs: " << QString::number(_qsos.length()) << endl;
    errorWhileSendingLog = false;
    if (logbookkey.length()<1)
    {
        //qDebug()<< "eLogQrzLog::sendQSOs: No valid KEY (-2)" << endl;
        int i = QMessageBox::warning(nullptr, tr("KLog - QRZ.com"),
                                       tr("Not valid KEY found") + "\n" +
                                          tr("Please configure your QRZ.com API key. You will find it in your QRZ.com Logbook settings webpage.\nYou need a QRZ.com subscription to use this feature."),
                                       QMessageBox::Ok);
        return -2;
    }
    qsos.clear();
    qsos << _qsos;
    lastQSO = false;
    foreach (int i, _qsos)
    {
        sendQSO(i);
        if (i == _qsos.last())
        {
            lastQSO = true;
        }
    }
    return 1;
}

int eLogQrzLog::sendQSO(const int _qsoID)
{
    //qDebug() << "eLogQrzLog::sendQSO: "  << QString::number(_qsoID) << endl;

    QString adifQSO = dataProxy->getADIFQSO(_qsoID);

    //qDebug()<< "eLogQrzLog::sendQSO: (ADIF) :" << adifQSO << endl;
    //qDebug()<< "eLogQrzLog::sendQSO: (KEY) :" << logbookkey << endl;
    QUrlQuery params;

    params.addQueryItem("KEY", logbookkey);
    params.addQueryItem("ACTION", "INSERT");
    params.addQueryItem("ADIF",adifQSO);
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));
    //qDebug() << "eLogQrzLog::sendQSO: END" << endl;

    QByteArray postData;

    postData = params.query(QUrl::FullyEncoded).toUtf8();
    params.setQuery(postData);    

    QNetworkRequest requestLog(QUrl("https://logbook.qrz.com/api"));
    requestLog.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //qDebug()<< "eLogQrzLog::sendQSO: postData: " << postData << endl;
    while (sendingQSO)
    {
        //qDebug() << "eLogQrzLog::sendQSO: sending QSO... just wait!" << endl;
    }
    managerLog->post(requestLog, postData);
    return 1;
}
bool eLogQrzLog::hasLogBookKey()
{
    if (logbookkey.length()>0)
    {
        return true;
    }
    else
    {
     return false;
    }
}


void eLogQrzLog::login()
{
    //qDebug()<< "eLogQrzLog::login"  << endl;
    if (logged)
    {
        //qDebug()<< "eLogQrzLog::login Already logged!"  << endl;
        return;
    }

    //bool savePassword = true;
    if (pass.length()<1)
    {
        //savePassword = false;

        bool ok;
        pass = QInputDialog::getText(nullptr, tr("KLog - QRZ.com password needed"),                                                   tr("Please enter your QRZ.com password: "), QLineEdit::Password, "", &ok);
        if (!ok)
        {
             //qDebug() << "eLogQrzLog::login - END 1" <<  endl;
            return;
        }
    }


    if ((user.length()<1) || (pass.length()<1))
    {
        //qDebug()<< "eLogQrzLog::login error 2"  << endl;
        //if (!savePassword)
        //{// We delete the password as soon as possible if the user is not willing to save it
        //    pass = QString();
        //}
        return;
    }

    QUrlQuery params;

    params.addQueryItem("username", user);
    params.addQueryItem("password", pass);    
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));
    sendDataParams(params);

    //if (!savePassword)
    //{// We delete the password as soon as possible if the user is not willing to save it
    //        pass = QString();
    //}
    //qDebug()<< "eLogQrzLog::login - END"  << endl;

}

int eLogQrzLog::sendDataParams(const QUrlQuery &_params)
{     
    //qDebug()<< "eLogQrzLog::sendDataParams: Params: " << _params.query(QUrl::FullyEncoded).toUtf8() << endl;

    QByteArray postData;

    QUrlQuery params;

    postData = params.query(QUrl::FullyEncoded).toUtf8();
    postData = postData + "&" + _params.query(QUrl::FullyEncoded).toUtf8();
    params.setQuery(postData);

    postData = params.query(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //qDebug()<< "eLogQrzLog::sendDataParams: postData: " << postData << endl;
    manager->post(request, postData);
     //qDebug()<< "eLogQrzLog::sendDataParams - END" << endl;
    return -1;
}

void eLogQrzLog::setCredentials(const QString &_user, const QString &_pass)
{
    //qDebug()<< "eLogQrzLog::setCredentials: user: " << _user << " / Pass: " << _pass  << endl;
    user = _user;
    pass = _pass;
}

QString eLogQrzLog::prepareToTranslate(const QString &_m)
{
       //qDebug()<< "eLogQrzLog:: = prepareToTranslate" << _m << endl;
    if (_m == "Callsign missing")
    {
        return tr("Callsign missing");
    }
    else
    {
        return _m;
    }
}

void eLogQrzLog::parseAppAnswer (const int howManyQSOs, const QString &_m)
{
    //qDebug()<< "eLogQrzLog::parseAppAnswer: " << _m  << endl;
    QStringList response;
    response.clear();
    response << _m.split('&');

    QString responseType = response.at(0).split('=').at(0);
    QString responseResult = response.at(0).split('=').at(1);
    //qDebug()<< "eLogQrzLog::parseAppAnswer: responseType: " << responseType  << endl;
    //qDebug()<< "eLogQrzLog::parseAppAnswer: responseResult: " << responseResult << endl;

    if ((responseType == "STATUS") && (responseResult == "OK"))
    {
        //qDebug()<< "eLogQrzLog::parseAppAnswer: OK"  << endl;
    }
    else if ((responseType == "STATUS") && (responseResult == "FAIL"))
    {
        errorWhileSendingLog = true;
        if (response.at(1).split('=').at(0) == "REASON")
        {
            QString reason = response.at(1).split('=').at(1);
           //qDebug()<< "eLogQrzLog::parseAppAnswer: " << reason  << endl;
        }
    }
    else if ((responseType == "STATUS") && (responseResult == "AUTH"))
    {
        errorWhileSendingLog = true;
        if (response.at(1).split('=').at(0) == "REASON")
        {
            QString reason = response.at(1).split('=').at(1);
            //qDebug()<< "eLogQrzLog::parseAppAnswer: " << reason  << endl;
            //onlineMessage->showMessage(QNetworkReply::NoError, QRZ, Fail, Other, reason);
        }
    }
}


void eLogQrzLog::sendSignal(QNetworkReply::NetworkError _error, QList<int> _qsos)
{
    errorWhileSendingLog = false;
    emit signalLogUploaded(_error, _qsos);
}



