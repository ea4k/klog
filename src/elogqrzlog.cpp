/***************************************************************************
                          elogqrzlog.cpp  -  description
                             -------------------
    begin                : nov 2020
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

#include "elogqrzlog.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

eLogQrzLog::eLogQrzLog(DataProxy_SQLite *dp, const QString &_parentFunction, const QString &_klogVersion)
{
   //qDebug()<< QString("eLogQrzLog::eLogQrzLog (%1) ").arg(_parentFunction)  << Qt::endl;
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
   //qDebug()<< "eLogQrzLog::eLogQrzLog - END"  << Qt::endl;
}

eLogQrzLog::~eLogQrzLog()
{
        //qDebug()<< "eLogQrzLog::~eLogQrzLog"  << Qt::endl;
}

void eLogQrzLog::setLogBookKey(const QString &_key)
{
    //qDebug()<< "eLogQrzLog::setLogBookKey: " << _key  << Qt::endl;
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
    //qDebug()<< "eLogQrzLog::slotLogManagerFinished"  << Qt::endl;
    sendingQSO = false;
    result = data->error();
    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result = " << QString::number(result) << Qt::endl;

    const QByteArray sdata = data->readAll();
    //qDebug() << "eLogQrzLog::slotManagerLogFinished: Received: " << sdata;

    QString text = QString();

    //qDebug() << "eLogQrzLog::slotManagerLogFinished - 00010" << Qt::endl;
    if (result == QNetworkReply::NoError)
    {
        QString dataReply(sdata);
        parseAppAnswer(1, dataReply);
       //qDebug()<< "eLogQrzLog::slotManageLogFinished - NO ERROR" << Qt::endl;
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

    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result = " << QString::number(result) << Qt::endl;
    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result Text = " << text << Qt::endl;
    emit showMessage(text);

}

void eLogQrzLog::parseXMLAnswer(QXmlStreamReader &xml)
{
    //qDebug() << "eLogQrzLog::parseXMLAnswer: " << xml.text() << Qt::endl;
    QString tdata = QString();

    while (!xml.atEnd())
    {
        tdata.clear();
        QXmlStreamReader::TokenType t = xml.readNext();
        if (t == QXmlStreamReader::StartDocument)
        {
            //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read version: " << xml.documentVersion().toString() << Qt::endl;
            //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read encoding: " << xml.documentEncoding().toString() << Qt::endl;
        }

        if (t == QXmlStreamReader::StartElement)
        {
            QString name = xml.name().toString();
            //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read name: " << name << Qt::endl;
            if (name == "QRZDatabase" || name == "Session" || name == "Callsign")
            {
                //qDebug() << "eLogQrzLog::parseXMLAnswer - No data: " << name << Qt::endl;
            }
            else if (name == "Key")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: API-Key: " << tdata << Qt::endl;
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
                   //qDebug() << "eLogQrzLog::parseXMLAnswer: CALL: " << tdata << Qt::endl;
                }
                continue;
            }
            else if (name == "Error")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: Error: " << tdata << Qt::endl;
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
                 //qDebug() << "eLogQrzLog::parseXMLAnswer: Message: " << tdata << Qt::endl;
                 emit dataFoundSignal("message", tdata);
                 continue;
             }

            else if (name == "fname")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    emit dataFoundSignal("name", tdata);
                    //qDebug() << "eLogQrzLog::parseXMLAnswer: Name: " << tdata << Qt::endl;
                }
                continue;
            }
            else if (name == "addr1")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    //emit dataFoundSignal("grid", tdata);
                   //qDebug() << "eLogQrzLog::parseXMLAnswer: addr1: " << tdata << Qt::endl;
                }
                continue;
            }
            else if (name == "addr2")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    emit dataFoundSignal("qth", tdata);
                   //qDebug() << "eLogQrzLog::parseXMLAnswer: addr2: " << tdata << Qt::endl;
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
                //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: "  << Qt::endl;
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: " << tdata << Qt::endl;
                }
                continue;
            }
            else
            {
                //qDebug() << "eLogQrzLog::parseXMLAnswer - name =  " << name << Qt::endl;
                //qDebug() << "eLogQrzLog::parseXMLAnswer - quick read data: " << xml.readElementText() << Qt::endl;
            }
        }
    }
    if (xml.hasError())
    {
        //qDebug() << "eLogQrzLog::parseXMLAnswer - ERROR: " << xml.errorString()  << Qt::endl;
    }
    else if (xml.atEnd())
    {
        //qDebug() << "eLogQrzLog::parseXMLAnswer - XML END"  << Qt::endl;
    }
/*
     QString tname, tdata;
     while(!xml.atEnd() && !xml.hasError())
     {
         //qDebug() << "eLogQrzLog::parseXMLAnswer - 00012" << Qt::endl;
         QXmlStreamReader::TokenType token = xml.readNext();

         if (token == QXmlStreamReader::StartDocument)
         {
             //qDebug() << "eLogQrzLog::parseXMLAnswer - StartDocument" << Qt::endl;
             continue;
         }
         //qDebug() << "eLogQrzLog::parseXMLAnswer - 00014" << Qt::endl;
         if (token == QXmlStreamReader::StartElement)
         {
             tname = xml.name().toString();

            //qDebug() << "eLogQrzLog::parseXMLAnswer - tname = " << tname << Qt::endl;

            if (tname == "Key")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: API-Key: " << tdata << Qt::endl;
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
                //qDebug() << "eLogQrzLog::parseXMLAnswer: Error: " << tdata << Qt::endl;
                emit dataFoundSignal("error", tdata);
            }
            if (tname == "Message")
             {
                 tdata = xml.readElementText();
                 //qDebug() << "eLogQrzLog::parseXMLAnswer: Message: " << tdata << Qt::endl;
                 emit dataFoundSignal("message", tdata);
             }
            if (tname == "call")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: CALL: " << tdata << Qt::endl;
                  }
                  continue;
              }
            if (tname == "fname")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("name", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: Name: " << tdata << Qt::endl;
                  }
                  continue;
              }
            if (tname == "addr1")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //emit dataFoundSignal("grid", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: addr1: " << tdata << Qt::endl;
                  }
                  continue;
              }
            if (tname == "addr2")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("qth", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: addr2: " << tdata << Qt::endl;
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
                  //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: "  << Qt::endl;
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: " << tdata << Qt::endl;
                  }
                  continue;
              }
            if (tname == "Session")
              {
                  //qDebug() << "eLogQrzLog::parseXMLAnswer: Session: "  << Qt::endl;
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //qDebug() << "eLogQrzLog::parseXMLAnswer: Session: " << tdata << Qt::endl;
                  }
                  continue;
              }

              //qDebug() << "eLogQrzLog::parseXMLAnswer: Unknown: " << tname << Qt::endl;
         }
     }
     if (xml.hasError())
     {
           //qDebug() << "eLogQrzLog::parseXMLAnswer XML error: " << "XML error: " << xml.errorString() << Qt::endl;
     }
     else if (xml.atEnd())
     {
           //qDebug() << "eLogQrzLog::parseXMLAnswer XML END: " << "Reached end, done" << Qt::endl;
     }
     */
    //qDebug() << "eLogQrzLog::parseXMLAnswer: END of token while" << Qt::endl;
    //qDebug()<< "eLogQrzLog::parseXMLAnswer - NO ERROR" << Qt::endl;
}

void eLogQrzLog::slotManagerFinished(QNetworkReply *data)
{
    //qDebug()<< "eLogQrzLog::slotManagerFinished"  << Qt::endl;

    result = data->error();
    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = " << QString::number(result) << Qt::endl;

    const QByteArray sdata = data->readAll();
   //qDebug() << "eLogQrzLog::slotManagerFinished: Received: " << sdata;

    QString text = QString();

   //qDebug() << "eLogQrzLog::slotManagerFinished - 00010" << Qt::endl;
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

    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = " << QString::number(result) << Qt::endl;
    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result Text = " << text << Qt::endl;

    emit showMessage(text);

}

void eLogQrzLog::downloadProgress(qint64 received, qint64 total) {
       //qDebug()<< "eLogQrzLog::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << Qt::endl;

       //qDebug()<< received << total;
    emit actionShowProgres(received, total);
}

void eLogQrzLog::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
       //qDebug()<< "eLogQrzLog::slotErrorManagement: " << QString::number(networkError) << Qt::endl;
    result = networkError;

    if (result == QNetworkReply::NoError)
    {
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
            //qDebug()<< "eLogQrzLog::slotErrorManagement: Host not found" << Qt::endl;
    }
    else
    {
            //qDebug()<< "eLogQrzLog::slotErrorManagement: ERROR!" << Qt::endl;
    }

    //actionError(result);
}

bool eLogQrzLog::canConnect()
{
   //qDebug()<< "eLogQrzLog::canConnect: "  << Qt::endl;
   if (!logged)
   {
       //qDebug()<< "eLogQrzLog::canConnect: 10"  << Qt::endl;
       login();
       if (!logged)
       {
            //qDebug()<< "eLogQrzLog::canConnect: Not logged"  << Qt::endl;
            return false;
       }
   }
   if (sessionkey.length()<1)
   {
        //qDebug()<< "eLogQrzLog::canConnect: API not valid"  << Qt::endl;
       return false;
   }
   //qDebug()<< "eLogQrzLog::canConnect: END"  << Qt::endl;
   return true;
}

void eLogQrzLog::fetchData()
{
    //qDebug()<< "eLogQrzLog::fetchData"  << Qt::endl;
    if (!canConnect())
    {
        return;
    }
   //qDebug()<< "eLogQrzLog::checkQRZ: Preparing the query"  << Qt::endl;
    QUrlQuery params;


    params.addQueryItem("action", "fetch");
    params.addQueryItem("option", "all");
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));

    sendDataParams(params);
}

void eLogQrzLog::checkQRZ(const QString &_qrz)
{
    //qDebug()<< "eLogQrzLog::checkQRZ: " << _qrz << Qt::endl;
    if (!util->isValidCall(_qrz))
    {
        //qDebug()<< "eLogQrzLog::checkQRZ: CALL not valid"  << Qt::endl;
        return;
    }
    serviceUrl = QUrl("https://xmldata.qrz.com/xml/current/");

    if (!canConnect())
    {
        return;
    }

    //qDebug()<< "eLogQrzLog::checkQRZ: Preparing the query"  << Qt::endl;
    QUrlQuery params;
    params.addQueryItem("s", sessionkey);
    params.addQueryItem("callsign", _qrz);
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));
    sendDataParams(params);
}

int eLogQrzLog::sendQSOs(QList<int> _qsos)
{
    //qDebug()<< "eLogQrzLog::sendQSOs: QSOs: " << QString::number(_qsos.length()) << Qt::endl;
    errorWhileSendingLog = false;
    if (logbookkey.length()<1)
    {
        //qDebug()<< "eLogQrzLog::sendQSOs: No valid KEY (-2)" << Qt::endl;
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
    //qDebug() << "eLogQrzLog::sendQSO: "  << QString::number(_qsoID) << Qt::endl;

    QString adifQSO = dataProxy->getADIFQSO(_qsoID);

    //qDebug()<< "eLogQrzLog::sendQSO: (ADIF) :" << adifQSO << Qt::endl;
    //qDebug()<< "eLogQrzLog::sendQSO: (KEY) :" << logbookkey << Qt::endl;
    QUrlQuery params;

    params.addQueryItem("KEY", logbookkey);
    params.addQueryItem("ACTION", "INSERT");
    params.addQueryItem("ADIF",adifQSO);
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));
    //qDebug() << "eLogQrzLog::sendQSO: END" << Qt::endl;

    QByteArray postData;

    postData = params.query(QUrl::FullyEncoded).toUtf8();
    params.setQuery(postData);

    QNetworkRequest requestLog(QUrl("https://logbook.qrz.com/api"));
    requestLog.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //qDebug()<< "eLogQrzLog::sendQSO: postData: " << postData << Qt::endl;
    while (sendingQSO)
    {
        //qDebug() << "eLogQrzLog::sendQSO: sending QSO... just wait!" << Qt::endl;
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
    //qDebug()<< "eLogQrzLog::login"  << Qt::endl;
    if (logged)
    {
        //qDebug()<< "eLogQrzLog::login Already logged!"  << Qt::endl;
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
             //qDebug() << "eLogQrzLog::login - END 1" <<  Qt::endl;
            return;
        }
    }


    if ((user.length()<1) || (pass.length()<1))
    {
        //qDebug()<< "eLogQrzLog::login error 2"  << Qt::endl;
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
    //qDebug()<< "eLogQrzLog::login - END"  << Qt::endl;

}

int eLogQrzLog::sendDataParams(const QUrlQuery &_params)
{
    //qDebug()<< "eLogQrzLog::sendDataParams: Params: " << _params.query(QUrl::FullyEncoded).toUtf8() << Qt::endl;

    QByteArray postData;

    QUrlQuery params;

    postData = params.query(QUrl::FullyEncoded).toUtf8();
    postData = postData + "&" + _params.query(QUrl::FullyEncoded).toUtf8();
    params.setQuery(postData);

    postData = params.query(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //qDebug()<< "eLogQrzLog::sendDataParams: postData: " << postData << Qt::endl;
    manager->post(request, postData);
     //qDebug()<< "eLogQrzLog::sendDataParams - END" << Qt::endl;
    return -1;
}

void eLogQrzLog::setCredentials(const QString &_user, const QString &_pass)
{
    //qDebug()<< "eLogQrzLog::setCredentials: user: " << _user << " / Pass: " << _pass  << Qt::endl;
    user = _user;
    pass = _pass;
}

QString eLogQrzLog::prepareToTranslate(const QString &_m)
{
       //qDebug()<< "eLogQrzLog:: = prepareToTranslate" << _m << Qt::endl;
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
    //qDebug()<< "eLogQrzLog::parseAppAnswer: " << _m  << Qt::endl;
    QStringList response;
    response.clear();
    response << _m.split('&');

    QString responseType = response.at(0).split('=').at(0);
    QString responseResult = response.at(0).split('=').at(1);
    //qDebug()<< "eLogQrzLog::parseAppAnswer: responseType: " << responseType  << Qt::endl;
    //qDebug()<< "eLogQrzLog::parseAppAnswer: responseResult: " << responseResult << Qt::endl;

    if ((responseType == "STATUS") && (responseResult == "OK"))
    {
        //qDebug()<< "eLogQrzLog::parseAppAnswer: OK"  << Qt::endl;
    }
    else if ((responseType == "STATUS") && (responseResult == "FAIL"))
    {
        errorWhileSendingLog = true;
        if (response.at(1).split('=').at(0) == "REASON")
        {
            QString reason = response.at(1).split('=').at(1);
           //qDebug()<< "eLogQrzLog::parseAppAnswer: " << reason  << Qt::endl;
        }
    }
    else if ((responseType == "STATUS") && (responseResult == "AUTH"))
    {
        errorWhileSendingLog = true;
        if (response.at(1).split('=').at(0) == "REASON")
        {
            QString reason = response.at(1).split('=').at(1);
            //qDebug()<< "eLogQrzLog::parseAppAnswer: " << reason  << Qt::endl;
            //onlineMessage->showMessage(QNetworkReply::NoError, QRZ, Fail, Other, reason);
        }
    }
}


void eLogQrzLog::sendSignal(QNetworkReply::NetworkError _error, QList<int> _qsos)
{
    errorWhileSendingLog = false;
    emit signalLogUploaded(_error, _qsos);
}



