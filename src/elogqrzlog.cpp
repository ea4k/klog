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
#ifdef QT_DEBUG
  //qDebug() << Q_FUNC_INFO << ": " << _parentFunction;
#endif

    klogVersion = _klogVersion;
    dataProxy = dp;
    errorWhileSendingLog = false;
    sendingQSO = false;
    lastQSO = false;
    subscriptionOK = true;

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
   //qDebug()<< "eLogQrzLog::eLogQrzLog - END"  << QT_ENDL;
}

eLogQrzLog::~eLogQrzLog()
{
    showDebugLog (Q_FUNC_INFO, "END");
}

void eLogQrzLog::showDebugLog(const QString &_func, const QString &_log)
{
#ifdef QT_DEBUG
  qDebug() << _func << ": " << _log;
#else
#endif
  delete(util);
}

void eLogQrzLog::setLogBookKey(const QString &_key)
{
    //qDebug()<< "eLogQrzLog::setLogBookKey: " << _key  << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start: key: " + _key);
    if (_key.length()>0)
    {
        logbookkey = _key;
    }
    showDebugLog (Q_FUNC_INFO, "END");
}

void eLogQrzLog::parseNetworkError(QNetworkReply::NetworkError _error)
{

    showDebugLog (Q_FUNC_INFO, "Start: " + QString::number(_error));
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

    QMessageBox::warning(nullptr, tr("KLog - QRZ.com"),
                                           tr("We have received the following error from QRZ.com (%1)").arg(result) + "\n" +
                                              text,
                                           QMessageBox::Ok);

    showDebugLog (Q_FUNC_INFO, "END");
}

void eLogQrzLog::slotManagerLogFinished(QNetworkReply *data)
{
    //qDebug()<< "eLogQrzLog::slotLogManagerFinished"  << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start");
    sendingQSO = false;
    result = data->error();
    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result = " << QString::number(result) << QT_ENDL;

    const QByteArray sdata = data->readAll();
    //qDebug() << "eLogQrzLog::slotManagerLogFinished: Received: " << sdata;

    QString text = QString();

    //qDebug() << "eLogQrzLog::slotManagerLogFinished - 00010" << QT_ENDL;
    if (result == QNetworkReply::NoError)
    {
        QString dataReply(sdata);
        //parseAppAnswer(1, dataReply);
        parseAppAnswer(dataReply);
        showDebugLog (Q_FUNC_INFO, "No error");
       //qDebug()<< "eLogQrzLog::slotManageLogFinished - NO ERROR" << QT_ENDL;
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

    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result = " << QString::number(result) << QT_ENDL;
    //qDebug()<< "eLogQrzLog::slotManagerLogFinished - Result Text = " << text << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Text: " + text);
    emit showMessage(text);
    showDebugLog (Q_FUNC_INFO, "END");
}

void eLogQrzLog::parseXMLAnswer(QXmlStreamReader &xml)
{
    //qDebug() << Q_FUNC_INFO << xml.text();
    showDebugLog (Q_FUNC_INFO, "Start: " + xml.text());
    QString tdata = QString();

    while (!xml.atEnd())
    {
        tdata.clear();
        QXmlStreamReader::TokenType t = xml.readNext();
        if (t == QXmlStreamReader::StartDocument)
        {
            //qDebug() << Q_FUNC_INFO << " - quick read version: " << xml.documentVersion().toString() << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " - quick read encoding: " << xml.documentEncoding().toString() << QT_ENDL;
        }

        if (t == QXmlStreamReader::StartElement)
        {
            QString name = xml.name().toString();
            //qDebug() << Q_FUNC_INFO << " - quick read name: " << name << QT_ENDL;
            if (name == "QRZDatabase" || name == "Session" || name == "Callsign")
            {
                //qDebug() << Q_FUNC_INFO << " - No data: " << name << QT_ENDL;
            }
            else if (name == "Key")
            {
                tdata = xml.readElementText();
                //qDebug() << Q_FUNC_INFO << " - API-Key: " << tdata << QT_ENDL;
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
                   //qDebug() << Q_FUNC_INFO << " - CALL: " << tdata << QT_ENDL;
                }
                continue;
            }
            else if (name == "Error")
            {
                tdata = xml.readElementText();
                //qDebug() << Q_FUNC_INFO << " - Error: " << tdata << QT_ENDL;
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
                 //qDebug() << Q_FUNC_INFO << " - Message: " << tdata << QT_ENDL;
                 emit dataFoundSignal("message", tdata);
                 continue;
             }

            else if (name == "fname")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    emit dataFoundSignal("name", tdata);
                    //qDebug() << Q_FUNC_INFO << " - Name: " << tdata << QT_ENDL;
                }
                continue;
            }
            else if (name == "addr1")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    //emit dataFoundSignal("grid", tdata);
                   //qDebug() << Q_FUNC_INFO << " - addr1: " << tdata << QT_ENDL;
                }
                continue;
            }
            else if (name == "addr2")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    emit dataFoundSignal("qth", tdata);
                   //qDebug() << Q_FUNC_INFO << " - addr2: " << tdata;
                }
                continue;
            }
            else if (name == "grid")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    //qDebug() << Q_FUNC_INFO << " - grid: " << tdata;
                      emit dataFoundSignal("grid", tdata);
                }
                continue;
            }
            else if (name == "qslmgr")
            {
                tdata = xml.readElementText();
                if (tdata.length()>0)
                {
                    //qDebug() << Q_FUNC_INFO << " - qslmgr: " << tdata;
                    emit dataFoundSignal("qslmgr", tdata);
                }
                continue;
            }
            else if (name == "Remark")
            {
                tdata = xml.readElementText();
                //qDebug() << Q_FUNC_INFO << " - Remark: " << tdata ;
                if (tdata.length()>0)
                {
                    //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: " << tdata << QT_ENDL;
                }
                continue;
            }
            else if ((xml.readElementText() == "non-subscriber") && (subscriptionOK))
            {
                //qDebug() << Q_FUNC_INFO << " - Not Subscribed QRZ.com user";
                showDebugLog (Q_FUNC_INFO, "Non Subscribed");
                emit showMessage(tr("You are not subscribed to QRZ.com."));
                //emit disableQRZAction(true);
                subscriptionOK = false;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << " - ELSE quick read data: " << xml.readElementText() << QT_ENDL;
            }
        }
    }
    if (xml.hasError())
    {
        showDebugLog (Q_FUNC_INFO, "Error: " + xml.errorString());
        //qDebug() << Q_FUNC_INFO << " - ERROR: " << xml.errorString()  << QT_ENDL;
    }
    else if (xml.atEnd())
    {
        //qDebug() << Q_FUNC_INFO << " - XML END"  << QT_ENDL;
    }
/*
     QString tname, tdata;
     while(!xml.atEnd() && !xml.hasError())
     {
         //qDebug() << "eLogQrzLog::parseXMLAnswer - 00012" << QT_ENDL;
         QXmlStreamReader::TokenType token = xml.readNext();

         if (token == QXmlStreamReader::StartDocument)
         {
             //qDebug() << "eLogQrzLog::parseXMLAnswer - StartDocument" << QT_ENDL;
             continue;
         }
         //qDebug() << "eLogQrzLog::parseXMLAnswer - 00014" << QT_ENDL;
         if (token == QXmlStreamReader::StartElement)
         {
             tname = xml.name().toString();

            //qDebug() << "eLogQrzLog::parseXMLAnswer - tname = " << tname << QT_ENDL;

            if (tname == "Key")
            {
                tdata = xml.readElementText();
                //qDebug() << "eLogQrzLog::parseXMLAnswer: API-Key: " << tdata << QT_ENDL;
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
                //qDebug() << "eLogQrzLog::parseXMLAnswer: Error: " << tdata << QT_ENDL;
                emit dataFoundSignal("error", tdata);
            }
            if (tname == "Message")
             {
                 tdata = xml.readElementText();
                 //qDebug() << "eLogQrzLog::parseXMLAnswer: Message: " << tdata << QT_ENDL;
                 emit dataFoundSignal("message", tdata);
             }
            if (tname == "call")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: CALL: " << tdata << QT_ENDL;
                  }
                  continue;
              }
            if (tname == "fname")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("name", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: Name: " << tdata << QT_ENDL;
                  }
                  continue;
              }
            if (tname == "addr1")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //emit dataFoundSignal("grid", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: addr1: " << tdata << QT_ENDL;
                  }
                  continue;
              }
            if (tname == "addr2")
              {
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      emit dataFoundSignal("qth", tdata);
                     //qDebug() << "eLogQrzLog::parseXMLAnswer: addr2: " << tdata << QT_ENDL;
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
                  //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: "  << QT_ENDL;
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //qDebug() << "eLogQrzLog::parseXMLAnswer: Remark: " << tdata << QT_ENDL;
                  }
                  continue;
              }
            if (tname == "Session")
              {
                  //qDebug() << "eLogQrzLog::parseXMLAnswer: Session: "  << QT_ENDL;
                  tdata = xml.readElementText();
                  if (tdata.length()>0)
                  {
                      //qDebug() << "eLogQrzLog::parseXMLAnswer: Session: " << tdata << QT_ENDL;
                  }
                  continue;
              }

              //qDebug() << "eLogQrzLog::parseXMLAnswer: Unknown: " << tname << QT_ENDL;
         }
     }
     if (xml.hasError())
     {
           //qDebug() << "eLogQrzLog::parseXMLAnswer XML error: " << "XML error: " << xml.errorString() << QT_ENDL;
     }
     else if (xml.atEnd())
     {
           //qDebug() << "eLogQrzLog::parseXMLAnswer XML END: " << "Reached end, done" << QT_ENDL;
     }
     */
    //qDebug() << "eLogQrzLog::parseXMLAnswer: END of token while" << QT_ENDL;
    //qDebug() << Q_FUNC_INFO << " - NO ERROR" ;
    showDebugLog (Q_FUNC_INFO, "END");
}

void eLogQrzLog::slotManagerFinished(QNetworkReply *data)
{
    //qDebug() << Q_FUNC_INFO;
    showDebugLog (Q_FUNC_INFO, "Start");
    result = data->error();
    //qDebug() << Q_FUNC_INFO << " - Result = " << QString::number(result) << QT_ENDL;

    const QByteArray sdata = data->readAll();
   //qDebug() << Q_FUNC_INFO << " - Received: " << sdata;

    QString text = QString();

   //qDebug() << Q_FUNC_INFO << " - 00010" << QT_ENDL;
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

    //qDebug() << Q_FUNC_INFO << " - Result = " << QString::number(result) << QT_ENDL;
    //qDebug() << Q_FUNC_INFO << " - Result Text = " << text << QT_ENDL;

    emit showMessage(text);
    showDebugLog (Q_FUNC_INFO, "Text: " + text);
    showDebugLog (Q_FUNC_INFO, "END");

}

void eLogQrzLog::downloadProgress(qint64 received, qint64 total) {
       //qDebug()<< "eLogQrzLog::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start: " + QString::number(received) +"/"+QString::number(total));
       //qDebug()<< received << total;
    emit actionShowProgres(received, total);
    showDebugLog (Q_FUNC_INFO, "END");
}

void eLogQrzLog::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
       //qDebug()<< "eLogQrzLog::slotErrorManagement: " << QString::number(networkError) << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start: " + QString::number(networkError));
    result = networkError;

    if (result == QNetworkReply::NoError)
    {
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
            //qDebug()<< "eLogQrzLog::slotErrorManagement: Host not found" << QT_ENDL;
    }
    else
    {
            //qDebug()<< "eLogQrzLog::slotErrorManagement: ERROR!" << QT_ENDL;
    }

    //actionError(result);
    showDebugLog (Q_FUNC_INFO, "END");
}

bool eLogQrzLog::canConnect()
{
   showDebugLog (Q_FUNC_INFO, "Start");
   if (!logged)
   {
       //qDebug() << Q_FUNC_INFO << " - 10";
       login();
       if (!logged)
       {
            //qDebug() << Q_FUNC_INFO << " - Not logged";
           showDebugLog (Q_FUNC_INFO, "END-false-1");
            return false;
       }
   }
   if (sessionkey.length()<1)
   {
       //qDebug() << Q_FUNC_INFO << " - API not valid";
       showDebugLog (Q_FUNC_INFO, "END-false-2");
       return false;
   }
   showDebugLog (Q_FUNC_INFO, "END-true");
   return true;
}

void eLogQrzLog::fetchData()
{
    //qDebug() << Q_FUNC_INFO;
    showDebugLog (Q_FUNC_INFO, "Start");
    if (!canConnect())
    {
        showDebugLog (Q_FUNC_INFO, "END- can't connect");
        return;
    }
    QUrlQuery params;

    params.addQueryItem("action", "fetch");
    params.addQueryItem("option", "all");
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));

    sendDataParams(params);
    showDebugLog (Q_FUNC_INFO, "END");
}

void eLogQrzLog::checkQRZ(const QString &_qrz)
{
    showDebugLog (Q_FUNC_INFO, "Start: " + _qrz);
    if (!util->isValidCall(_qrz))
    {
        //qDebug()<< "eLogQrzLog::checkQRZ: CALL not valid"  << QT_ENDL;
        showDebugLog (Q_FUNC_INFO, "CALL not valid");
        return;
    }
    serviceUrl = QUrl("https://xmldata.qrz.com/xml/current/");

    if (!canConnect())
    {
        showDebugLog (Q_FUNC_INFO, "END-can't connect");
        return;
    }

    //qDebug() << Q_FUNC_INFO << " - Preparing the query"  << QT_ENDL;
    QUrlQuery params;
    params.addQueryItem("s", sessionkey);
    params.addQueryItem("callsign", _qrz);
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));
    sendDataParams(params);
    showDebugLog (Q_FUNC_INFO, "END");
}

int eLogQrzLog::sendQSOs(QList<int> _qsos)
{
    //qDebug()<< "eLogQrzLog::sendQSOs: QSOs: " << QString::number(_qsos.length()) << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start");
    errorWhileSendingLog = false;
    if (logbookkey.length()<1)
    {
        //qDebug()<< "eLogQrzLog::sendQSOs: No valid KEY (-2)" << QT_ENDL;
        QMessageBox::warning(nullptr, tr("KLog - QRZ.com"),
                                       tr("Not valid KEY found") + "\n" +
                                          tr("Please configure your QRZ.com API key. You will find it in your QRZ.com Logbook settings webpage.\nYou need a QRZ.com subscription to use this feature."),
                                       QMessageBox::Ok);
        showDebugLog (Q_FUNC_INFO, "END-2");
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
    showDebugLog (Q_FUNC_INFO, "END");
    return 1;
}

int eLogQrzLog::sendQSO(const int _qsoID)
{
    //qDebug() << "eLogQrzLog::sendQSO: "  << QString::number(_qsoID) << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start: " + QString::number(_qsoID));
    QString adifQSO = dataProxy->getADIFQSO(_qsoID);

    //qDebug()<< "eLogQrzLog::sendQSO: (ADIF) :" << adifQSO << QT_ENDL;
    //qDebug()<< "eLogQrzLog::sendQSO: (KEY) :" << logbookkey << QT_ENDL;
    QUrlQuery params;

    params.addQueryItem("KEY", logbookkey);
    params.addQueryItem("ACTION", "INSERT");
    params.addQueryItem("ADIF",adifQSO);
    params.addQueryItem("agent", util->getGlobalAgent(klogVersion));
    //qDebug() << "eLogQrzLog::sendQSO: END" << QT_ENDL;

    QByteArray postData;

    postData = params.query(QUrl::FullyEncoded).toUtf8();
    params.setQuery(postData);

    QNetworkRequest requestLog(QUrl("https://logbook.qrz.com/api"));
    requestLog.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    showDebugLog (Q_FUNC_INFO, "postData: " + QString(postData));

    //qDebug()<< "eLogQrzLog::sendQSO: postData: " << postData << QT_ENDL;
    while (sendingQSO)
    {
        //qDebug() << "eLogQrzLog::sendQSO: sending QSO... just wait!" << QT_ENDL;
    }
    managerLog->post(requestLog, postData);
    showDebugLog (Q_FUNC_INFO, "END");
    return 1;
}
bool eLogQrzLog::hasLogBookKey()
{
    showDebugLog (Q_FUNC_INFO, "Start");
    if (logbookkey.length()>0)
    {
        showDebugLog (Q_FUNC_INFO, "END-true");
        return true;
    }
    else
    {
        showDebugLog (Q_FUNC_INFO, "END-false");
     return false;
    }
}


void eLogQrzLog::login()
{
    //qDebug()<< "eLogQrzLog::login"  << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start");
    if (logged)
    {
        //qDebug()<< "eLogQrzLog::login Already logged!"  << QT_ENDL;
        showDebugLog (Q_FUNC_INFO, "END-1");
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
             //qDebug() << "eLogQrzLog::login - END 1" <<  QT_ENDL;
            showDebugLog (Q_FUNC_INFO, "END-2");
            return;
        }
    }


    if ((user.length()<1) || (pass.length()<1))
    {
        //qDebug()<< "eLogQrzLog::login error 2"  << QT_ENDL;
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
    //qDebug()<< "eLogQrzLog::login - END"  << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "END");

}

int eLogQrzLog::sendDataParams(const QUrlQuery &_params)
{
    //qDebug()<< "eLogQrzLog::sendDataParams: Params: " << _params.query(QUrl::FullyEncoded).toUtf8() << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start");

    QByteArray postData;

    QUrlQuery params;

    postData = params.query(QUrl::FullyEncoded).toUtf8();
    postData = postData + "&" + _params.query(QUrl::FullyEncoded).toUtf8();
    params.setQuery(postData);

    postData = params.query(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //qDebug()<< "eLogQrzLog::sendDataParams: postData: " << postData << QT_ENDL;
    manager->post(request, postData);
     //qDebug()<< "eLogQrzLog::sendDataParams - END" << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "END");
    return -1;
}

void eLogQrzLog::setCredentials(const QString &_user, const QString &_pass)
{
    //qDebug()<< "eLogQrzLog::setCredentials: user: " << _user << " / Pass: " << _pass  << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start: " + _user + "/" + _pass);
    user = _user;
    pass = _pass;
    showDebugLog (Q_FUNC_INFO, "END");
}

QString eLogQrzLog::prepareToTranslate(const QString &_m)
{
       //qDebug()<< "eLogQrzLog:: = prepareToTranslate" << _m << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start: " + _m);
    if (_m == "Callsign missing")
    {
        return tr("Callsign missing");
    }
    else
    {
        return _m;
    }
}

void eLogQrzLog::parseAppAnswer (const QString &_m)
{
    //qDebug()<< "eLogQrzLog::parseAppAnswer: " << _m  << QT_ENDL;
    showDebugLog (Q_FUNC_INFO, "Start: " + _m);
    QStringList response;
    response.clear();
    response << _m.split('&');

    QString responseType = response.at(0).split('=').at(0);
    QString responseResult = response.at(0).split('=').at(1);
    //qDebug()<< "eLogQrzLog::parseAppAnswer: responseType: " << responseType  << QT_ENDL;
    //qDebug()<< "eLogQrzLog::parseAppAnswer: responseResult: " << responseResult << QT_ENDL;

    if ((responseType == "STATUS") && (responseResult == "OK"))
    {
        //qDebug()<< "eLogQrzLog::parseAppAnswer: OK"  << QT_ENDL;
    }
    else if ((responseType == "STATUS") && (responseResult == "FAIL"))
    {
        errorWhileSendingLog = true;
        if (response.at(1).split('=').at(0) == "REASON")
        {
            QString reason = response.at(1).split('=').at(1);
           //qDebug()<< "eLogQrzLog::parseAppAnswer: " << reason  << QT_ENDL;
        }
    }
    else if ((responseType == "STATUS") && (responseResult == "AUTH"))
    {
        errorWhileSendingLog = true;
        if (response.at(1).split('=').at(0) == "REASON")
        {
            QString reason = response.at(1).split('=').at(1);
            //qDebug()<< "eLogQrzLog::parseAppAnswer: " << reason  << QT_ENDL;
            //onlineMessage->showMessage(QNetworkReply::NoError, QRZ, Fail, Other, reason);
        }
    }
}


void eLogQrzLog::sendSignal(QNetworkReply::NetworkError _error, QList<int> _qsos)
{
    showDebugLog (Q_FUNC_INFO, "Error: " + QString::number(_error) + " - List: " + QString::number(_qsos.length ()));
    errorWhileSendingLog = false;
    emit signalLogUploaded(_error, _qsos);
}


void eLogQrzLog::setSubcription(const bool _ok)
{
    showDebugLog (Q_FUNC_INFO, util->boolToQString (_ok));
    subscriptionOK = _ok;
}

bool eLogQrzLog::getSubscription()
{
    showDebugLog (Q_FUNC_INFO, util->boolToQString (subscriptionOK));
    return subscriptionOK;
}
