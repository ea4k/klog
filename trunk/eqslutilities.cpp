#include "eqslutilities.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>



eQSLUtilities::eQSLUtilities(const QString &_parentFunction)
{
    //qDebug()<< "eQSLUtilities::eQSLUtilities"  << endl;

    user = QString();
    pass = QString();
    qsos.clear();

    currentQSO = -1;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotQsoUploadFinished(QNetworkReply*)));
    stationCallsign = QString();
    uploadingFile = false;
    util = new Utilities;
    //qDebug()<< "eQSLUtilities::eQSLUtilities - END"  << endl;
}

eQSLUtilities::~eQSLUtilities()
{
        //qDebug()<< "eQSLUtilities::~eQSLUtilities"  << endl;
}

void eQSLUtilities::setUser(const QString &_call)
{
     //qDebug() << "eQSLUtilities::setUser: " << _call << endl;
    user = _call;
     //qDebug() << "eQSLUtilities::setUser: END" << endl;
}

void eQSLUtilities::setPass(const QString &_pass)
{
     //qDebug() << "eQSLUtilities::setPass: " << _pass << endl;
    pass = _pass;
     //qDebug() << "eQSLUtilities::setPass: END" << endl;
}
 void eQSLUtilities::slotQsoUploadFinished(QNetworkReply *data)
{
    //qDebug()<< "eQSLUtilities::slotQsoUploadFinished"  << endl;

    result = data->error();
    //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();
    QString text = QString();

    if (currentQSO>0)
    {
        emit actionReturnDownload(result, currentQSO);
        currentQSO = -1;
    }

    if (result == QNetworkReply::NoError)
    {

        text = "eQSL: " + prepareToTranslate(sdata);
        //qDebug()<< sdata;
         //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - NO ERROR" << endl;
        if (uploadingFile)
        {
            uploadingFile = false;
            emit signalFileUploaded(QNetworkReply::NoError, qsos);
            qsos.clear();
            return;
        }
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
        //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = Host Not found! = " << QString::number(result)  << endl;
        text = "eQSL: " + tr("Host not found!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == QNetworkReply::TimeoutError)
    {
        //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = Time out error! = " << QString::number(result)  << endl;
        text = "eQSL: " + tr("Timeout error!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else
    {
        //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
        text = "eQSL: " + tr("Undefined error number (#%1)... ").arg(result);
        int i = QMessageBox::warning(nullptr, tr("KLog - eQSL"),
                                       tr("We have received an undefined error from eQSL (%1)").arg(result) + "\n" +
                                          tr("Please check your config in the setup and contact the KLog development team if you can't fix it. eQSL uploads will be disabled."),
                                       QMessageBox::Ok);
        //TODO: Mark the previous QSO as not sent to clublog
    }

    //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = " << QString::number(result) << endl;
    //emit done();
    emit signalFileUploaded(result, qsos);
    emit  showMessage(text);

}

void eQSLUtilities::slotFileUploadFinished(QNetworkReply *data)
{
       //qDebug()<< "eQSLUtilities::slotFileUploadFinished"  << endl;

    result = data->error();
        //qDebug()<< "eQSLUtilities::slotFileUploadFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();

    QString text = QString();

    if (result == QNetworkReply::NoError)
    {

        text = "eQSL: " + prepareToTranslate(sdata);
           //qDebug()<< "eQSLUtilities::slotFileUploadFinished - Result = NoError = " << QString::number(result)  << endl;
           //qDebug()<< sdata;
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
           //qDebug()<< "eQSLUtilities::slotFileUploadFinished - Result = Host Not found! = " << QString::number(result)  << endl;
        text = "eQSL: " + tr("Host not found!");
    }
    else if (result == QNetworkReply::TimeoutError)
    {
           //qDebug()<< "eQSLUtilities::slotFileUploadFinished - Result = Time out error! = " << QString::number(result)  << endl;
        text = "eQSL: " + tr("Timeout error!");
    }
    else
    {
        //qDebug()<< "eQSLUtilities::slotFileUploadFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
        text = "eQSL: " + tr("Undefined error...");
    }

        //qDebug()<< "eQSLUtilities::slotFileUploadFinished - Result = " << QString::number(result) << endl;
    //emit done();
    emit  showMessage(text);
}

void eQSLUtilities::downloadProgress(qint64 received, qint64 total) {
       //qDebug()<< "eQSLUtilities::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << endl;

       //qDebug()<< received << total;
    emit actionShowProgres(received, total);
}

void eQSLUtilities::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
       //qDebug()<< "eQSLUtilities::slotErrorManagement: " << QString::number(networkError) << endl;
    result = networkError;

    if (result == QNetworkReply::NoError)
    {
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
            //qDebug()<< "eQSLUtilities::slotErrorManagement: Host not found" << endl;
    }
    else
    {
            //qDebug()<< "eQSLUtilities::slotErrorManagement: ERROR!" << endl;
    }

    //actionError(result);
}

void eQSLUtilities::setCredentials(const QString &_user, const QString &_pass, const QString _defaultStationCallsign)
{
    //qDebug()<< "eQSLUtilities::setCredentials: user: " << _user << " / Pass: " << _pass << " / StationCallsign: " << _defaultStationCallsign << endl;
    stationCallsign = _defaultStationCallsign;
    user = _user;
    pass = _pass;
}

QString eQSLUtilities::prepareToTranslate(const QString &_m)
{
       //qDebug()<< "eQSLUtilities:: = prepareToTranslate" << _m << endl;
    if (_m == "Callsign missing")
    {
        return tr("Callsign missing");
    }
    else if (_m == "Invalid callsign")
    {
        return tr("Invalid callsign");
    }
    else if (_m == "Skipping SWL callsign")
    {
        return tr("Skipping SWL callsign");
    }
    else if (_m == "Callsign is your own call")
    {
        return tr("Callsign is your own call");
    }
    else if (_m == "Invalid callsign with no DXCC mapping")
    {
        return tr("Invalid callsign with no DXCC mapping");
    }
    else if (_m == "Updated QSO")
    {
        return tr("Updated QSO");
    }
    else if (_m == "Invalid ADIF record")
    {
        return tr("Invalid ADIF record");
    }
    else if (_m == "Missing ADIF record")
    {
        return tr("Missing ADIF record");
    }
    else if (_m == "Test mode - parameters ok, no action taken")
    {
        return tr("Test mode - parameters ok, no action taken");
    }
    else if (_m == "Excessive API Usage")
    {
        return tr("Excessive API Usage");
    }
    else if (_m == "Internal Error")
    {
        return tr("Internal Error");
    }
    else if (_m == "Rejected")
    {
        return tr("Rejected");
    }
    else if (_m == "QSO Duplicate")
    {
        return tr("QSO Duplicate");
    }
    else if (_m == "QSO Modified")
    {
        return tr("QSO Modified");
    }
    else if (_m == "Missing Login")
    {
        return tr("Missing Login");
    }
    else if (_m == "QSO OK")
    {
        return tr("QSO OK");
    }
    else if (_m == "Upload denied")
    {
        return tr("Upload denied");
    }
    else if (_m == "No callsign selected")
    {
        return tr("No callsign selected");
    }
    else if (_m == "No match found")
    {
        return tr("No match found");
    }
    else if (_m == "Dropped QSO")
    {
        return tr("Dropped QSO");
    }
    else if (_m == "OK")
    {
        return tr("OK");
    }
    else if (_m == "Login rejected")
    {
        return tr("Login rejected");
    }
    else if (_m == "Upload denied")
    {
        return tr("Upload denied");
    }
    else if (_m == "Rejected: Callsign is your own call")
    {
        return tr("Rejected: Callsign is your own call");
    }
    else
    {
        return _m;
    }
}

void eQSLUtilities::sendLogFile(const QString &_file, QList<int> _qso)
{
     //qDebug()<< "eQSLUtilities::sendLogFile: " << _file << endl;
    qsos.clear();
    qsos.append(_qso);
    QUrl serviceUrl;
    serviceUrl = QUrl("https://www.eQSL.cc/qslcard/ImportADIF.cfm");

    QByteArray postData;

    QUrlQuery params;

    // FIRST PARAMS is the file
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QByteArray blob;


    QFile *file = new QFile(_file);
    if (file->open(QIODevice::ReadOnly))
    {
         blob = file->readAll();
        //qDebug()<< "eQSLUtilities::sendLogFile: FILE OPEN: " << blob << endl;
    }
    else
    {
         //qDebug()<< "eQSLUtilities::sendLogFile: ERROR File not opened" << endl;
        return;
    }
    file->close();
    // The rest of the form goes as usual
     //qDebug()<< "eQSLUtilities::sendLogFile: e: " << user << endl;
     //qDebug()<< "eQSLUtilities::sendLogFile: pass: " << pass << endl;
     //qDebug()<< "eQSLUtilities::sendLogFile: stationcall: " << stationCallsign << endl;

    QHttpPart userPart;
    userPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"eqsl_user\""));
    userPart.setBody(user.toUtf8());

    QHttpPart passPart;
    passPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"eqsl_pswd\""));
    passPart.setBody(pass.toUtf8());

    QHttpPart filePart;
    QString aux = QString("form-data; name=\"Filename\"; filename=\"%1\"").arg(_file);
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(aux));
    filePart.setBody(blob);

    multiPart->append(userPart);
    multiPart->append(passPart);
    multiPart->append(filePart);

    uploadingFile = true;
    QNetworkRequest request(serviceUrl);
    manager->post(request, multiPart);
    //multiPart->setParent(reply);

     //qDebug()<< "eQSLUtilities::sendLogFile - END" << endl;

}


