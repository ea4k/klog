#include "elogqrzlog.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

// https://www.qrz.com/XML/current_spec.html
// https://www.qrz.com/page/xml_data.html


eLogQrzLog::eLogQrzLog(DataProxy_SQLite *dp, const QString &_parentFunction)
{
   //qDebug()<< QString("eLogQrzLog::eLogQrzLog (%1) ").arg(_parentFunction)  << endl;
    dataProxy = dp;
    qsos.clear();
    apikey = QString();
    currentQSO = -1;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotManagerFinished(QNetworkReply*)));
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

 void eLogQrzLog::slotManagerFinished(QNetworkReply *data)
{
    //qDebug()<< "eLogQrzLog::slotManagerFinished"  << endl;

    result = data->error();
    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();
   //qDebug() << "eLogQrzLog::slotManagerFinished: " << sdata;

    QString text = QString();

   //qDebug() << "eLogQrzLog::slotManagerFinished - 00010" << endl;
    if (result == QNetworkReply::NoError)
    {
        //QXmlStreamReader reader(sdata);
        reader = new QXmlStreamReader(sdata);
       //qDebug() << "eLogQrzLog::slotManagerFinished - 00011" << endl;

        QString tname, tdata;
        while(!reader->atEnd() && !reader->hasError())
        {
            //qDebug() << "eLogQrzLog::slotManagerFinished - 00012" << endl;
            QXmlStreamReader::TokenType token = reader->readNext();


            if (token == QXmlStreamReader::StartDocument)
            {
                continue;
            }
            //qDebug() << "eLogQrzLog::slotManagerFinished - 00014" << endl;
            if (token == QXmlStreamReader::StartElement)
            {
                tname = reader->name().toString();

               //qDebug() << "eLogQrzLog::slotManagerFinished - tname = " << tname << endl;
                 if (tname == "Key")
                 {
                     tdata = reader->readElementText();
                    //qDebug() << "eLogQrzLog::slotManagerFinished: API-Key: " << tdata << endl;
                     if (tdata.length()>0)
                     {
                         apikey = tdata;
                         logged = true;
                     }
                     continue;
                 }
                if (tname == "Error")
                {
                    tdata = reader->readElementText();
                    //qDebug() << "eLogQrzLog::slotManagerFinished: Error: " << tdata << endl;
                    emit dataFoundSignal("error", tdata);
                }
                 if (tname == "call")
                 {
                     tdata = reader->readElementText();
                     if (tdata.length()>0)
                     {
                        //qDebug() << "eLogQrzLog::slotManagerFinished: CALL: " << tdata << endl;
                     }
                     continue;
                 }
                 if (tname == "fname")
                 {
                     tdata = reader->readElementText();
                     if (tdata.length()>0)
                     {
                         emit dataFoundSignal("name", tdata);
                        //qDebug() << "eLogQrzLog::slotManagerFinished: Name: " << tdata << endl;
                     }
                     continue;
                 }
                 if (tname == "addr1")
                 {
                     tdata = reader->readElementText();
                     if (tdata.length()>0)
                     {
                         //emit dataFoundSignal("grid", tdata);
                        //qDebug() << "eLogQrzLog::slotManagerFinished: addr1: " << tdata << endl;
                     }
                     continue;
                 }
                 if (tname == "addr2")
                 {
                     tdata = reader->readElementText();
                     if (tdata.length()>0)
                     {
                         emit dataFoundSignal("qth", tdata);
                        //qDebug() << "eLogQrzLog::slotManagerFinished: addr2: " << tdata << endl;
                     }
                     continue;
                 }
                 if (tname == "grid")
                 {
                     tdata = reader->readElementText();
                     if (tdata.length()>0)
                     {
                         emit dataFoundSignal("grid", tdata);
                     }
                     continue;
                 }
                 if (tname == "qslmgr")
                 {
                     tdata = reader->readElementText();
                     if (tdata.length()>0)
                     {
                         emit dataFoundSignal("qslmgr", tdata);
                     }
                     continue;
                 }
            }
        }
        if (reader->hasError())
        {
              //qDebug() << "eLogQrzLog::slotManagerFinished XML error: " << "XML error: " << reader->errorString() << endl;
        }
        else if (reader->atEnd())
        {
              //qDebug() << "eLogQrzLog::slotManagerFinished XML END: " << "Reached end, done" << endl;
        }
       //qDebug() << "eLogQrzLog::slotManagerFinished: END of token while" << endl;
       //qDebug()<< "eLogQrzLog::slotManagerFinished - NO ERROR" << endl;
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
        //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = Host Not found! = " << QString::number(result)  << endl;
        text = "QRZ.com: " + tr("Host not found!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == QNetworkReply::TimeoutError)
    {
        //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = Time out error! = " << QString::number(result)  << endl;
        text = "QRZ.com: " + tr("Timeout error!");
        //TODO: Mark the previous QSO as not sent to clublog
    }    
    else
    {
        //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
        text = "QRZ.com: " + tr("Undefined error number (#%1)... ").arg(result);
        int i = QMessageBox::warning(nullptr, tr("KLog - QRZ.com"),
                                       tr("We have received an undefined error from Clublog (%1)").arg(result) + "\n" +
                                          tr("Please check your config in the setup and contact the KLog development team if you can't fix it. QRZ.com uploads will be disabled."),
                                       QMessageBox::Ok);
        emit disableClubLogAction(true);
        //TODO: Mark the previous QSO as not sent to clublog
    }

    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result = " << QString::number(result) << endl;
    //qDebug()<< "eLogQrzLog::slotManagerFinished - Result Text = " << text << endl;
    //emit done();
    //emit signalFileUploaded(result, qsos);
    emit showMessage(text);

}

void eLogQrzLog::slotFileUploadFinished(QNetworkReply *data)
{
       //qDebug()<< "eLogQrzLog::slotFileUploadFinished"  << endl;

    result = data->error();
        //qDebug()<< "eLogQrzLog::slotFileUploadFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();

    QString text = QString();


    if (result == QNetworkReply::NoError)
    {

        text = "QRZ.com: " + prepareToTranslate(sdata);
           //qDebug()<< "eLogQrzLog::slotFileUploadFinished - Result = NoError = " << QString::number(result)  << endl;
            //qDebug()<< sdata;


    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
           //qDebug()<< "eLogQrzLog::slotFileUploadFinished - Result = Host Not found! = " << QString::number(result)  << endl;
        text = "QRZ.com: " + tr("Host not found!");
    }
    else if (result == QNetworkReply::TimeoutError)
    {
           //qDebug()<< "eLogQrzLog::slotFileUploadFinished - Result = Time out error! = " << QString::number(result)  << endl;
        text = "QRZ.com: " + tr("Timeout error!");
    }
    else
    {
           //qDebug()<< "eLogQrzLog::slotFileUploadFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
        text = "QRZ.com: " + tr("Undefined error...");
    }

        //qDebug()<< "eLogQrzLog::slotFileUploadFinished - Result = " << QString::number(result) << endl;
    //emit done();
    emit  showMessage(text);
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
       login();
       if (!logged)
       {
          //qDebug()<< "eLogQrzLog::canConnect: Not logged"  << endl;
           return false;
       }
   }
   if (apikey.length()<1)
   {
      //qDebug()<< "eLogQrzLog::canConnect: API not valid"  << endl;
       return false;
   }
   return true;
}

void eLogQrzLog::checkQRZ(const QString &_qrz)
{
    //qDebug()<< "eLogQrzLog::checkQRZ: " << _qrz << endl;
    if (!util->isValidCall(_qrz))
    {
        //qDebug()<< "eLogQrzLog::checkQRZ: CALL not valid"  << endl;
        return;
    }
    if (!canConnect())
    {
        return;
    }
   //qDebug()<< "eLogQrzLog::checkQRZ: Preparing the query"  << endl;
    QUrlQuery params;

    params.addQueryItem("s", apikey);
    params.addQueryItem("callsign", _qrz);
    sendDataParams(params);
}

int eLogQrzLog::sendQSO(const int _qsoID)
{
   //qDebug() << "eLogQrzLog::sendQSO: "  << QString::number(_qsoID) << endl;
    QString adifQSO = QString();
    if (apikey.length()<1)
    {
        return -1;
    }
    adifQSO = dataProxy->getADIFQSO(_qsoID);


   //qDebug()<< "eLogQrzLog::sendQSO: (ADIF) :" << adifQSO << endl;
    QUrlQuery params;
    params.addQueryItem("KEY", apikey);
    params.addQueryItem("ACTION", "INSERT");
    params.addQueryItem("adif",adifQSO);
   //qDebug() << "eLogQrzLog::sendQSO: END" << endl;
    //uploadingFile = false;
    return sendDataParams(params);
}

void eLogQrzLog::login()
{
   //qDebug()<< "eLogQrzLog::login"  << endl;
    if (logged)
    {
       //qDebug()<< "eLogQrzLog::login Already logged!"  << endl;
        return;
    }
    if ((user.length()<1) || (pass.length()<1))
    {
       //qDebug()<< "eLogQrzLog::login error 2"  << endl;
        return;
    }
    QUrlQuery params;

    params.addQueryItem("username", user);
    params.addQueryItem("password", pass);
    //params.addQueryItem("username", loginString);
    sendDataParams(params);
   //qDebug()<< "eLogQrzLog::login - END"  << endl;

}

int eLogQrzLog::sendDataParams(const QUrlQuery &_params)
{     
     //qDebug()<< "eLogQrzLog::sendDataParams: Params: " << _params.query(QUrl::FullyEncoded).toUtf8() << endl;

    QByteArray postData;

    QUrlQuery params;

    //params.addQueryItem("api",api);
    postData = params.query(QUrl::FullyEncoded).toUtf8();
    postData = postData + "&" + _params.query(QUrl::FullyEncoded).toUtf8();
    params.setQuery(postData);

    postData = params.query(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    //qDebug()<< "eLogQrzLog::sendDataParams: URL: " << request.url() << endl;

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





