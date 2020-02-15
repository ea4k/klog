#include "elogclublog.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>



eLogClubLog::eLogClubLog() : QObject(nullptr)
{
     //qDebug() << "eLogClubLog::eLogClubLog"  << endl;
    call= QString();
    email = QString();
    pass = QString();
    api = "9467beee93377e82a276b0a777d388b5c933d044";
    currentQSO = -1;
    useQSOStationCallsign = false;
    stationCallsign = QString();
     //qDebug() << "eLogClubLog::eLogClubLog - END"  << endl;
}

eLogClubLog::~eLogClubLog()
{
     //qDebug() << "eLogClubLog::~eLogClubLog"  << endl;
}


 void eLogClubLog::slotQsoUploadFinished(QNetworkReply *data)
{
     //qDebug() << "eLogClubLog::slotQsoUploadFinished"  << endl;

    result = data->error();
     //qDebug() << "eLogClubLog::slotQsoUploadFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();

    QString text = QString();

    if (currentQSO>0)
    {
        emit actionReturnDownload(result, currentQSO);
        currentQSO = -1;
    }


    if (result == QNetworkReply::NoError)
    {

        text = "ClubLog: " + prepareToTranslate(sdata);
        //qDebug() << sdata;


    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
        //qDebug() << "eLogClubLog::slotQsoUploadFinished - Result = Host Not found! = " << QString::number(result)  << endl;
        text = "ClubLog: " + tr("Host not found!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == QNetworkReply::TimeoutError)
    {
        //qDebug() << "eLogClubLog::slotQsoUploadFinished - Result = Time out error! = " << QString::number(result)  << endl;
        text = "ClubLog: " + tr("Timeout error!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == 202)
    {
        //qDebug() << "eLogClubLog::slotQsoUploadFinished - Result = Password Error! = " << QString::number(result)  << endl;
        text = "ClubLog: " + tr("It seems to be a PASSWORD ERROR; check your password.");

        QMessageBox::warning(0, tr("KLog - ClubLog"),
                                       tr("It seems that your ClubLog password is not correct.") + "\n" +
                                          tr("Please check your password in the setup. ClubLog uploads will be disabled."),
                                       QMessageBox::Ok);
        emit disableClubLogAction(true);
        //TODO: Mark the previous QSO as not sent to clublog


    }
    else
    {
        //qDebug() << "eLogClubLog::slotQsoUploadFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
        text = "ClubLog: " + tr("Undefined error...");
        //TODO: Mark the previous QSO as not sent to clublog
    }

     //qDebug() << "eLogClubLog::slotQsoUploadFinished - Result = " << QString::number(result) << endl;
    //emit done();
    emit  showMessage(text);

}

void eLogClubLog::slotFileUploadFinished(QNetworkReply *data)
{
    //qDebug() << "eLogClubLog::slotFileUploadFinished"  << endl;

    result = data->error();
     //qDebug() << "eLogClubLog::slotFileUploadFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();

    QString text = QString();


    if (result == QNetworkReply::NoError)
    {

        text = "ClubLog: " + prepareToTranslate(sdata);
        //qDebug() << "eLogClubLog::slotFileUploadFinished - Result = NoError = " << QString::number(result)  << endl;
        //qDebug() << sdata;


    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
        //qDebug() << "eLogClubLog::slotFileUploadFinished - Result = Host Not found! = " << QString::number(result)  << endl;
        text = "ClubLog: " + tr("Host not found!");
    }
    else if (result == QNetworkReply::TimeoutError)
    {
        //qDebug() << "eLogClubLog::slotFileUploadFinished - Result = Time out error! = " << QString::number(result)  << endl;
        text = "ClubLog: " + tr("Timeout error!");
    }
    else
    {
        //qDebug() << "eLogClubLog::slotFileUploadFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
        text = "ClubLog: " + tr("Undefined error...");
    }

     //qDebug() << "eLogClubLog::slotFileUploadFinished - Result = " << QString::number(result) << endl;
    //emit done();
    emit  showMessage(text);
}

void eLogClubLog::downloadProgress(qint64 received, qint64 total) {
    //qDebug() << "eLogClubLog::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << endl;

    //qDebug() << received << total;
    emit actionShowProgres(received, total);
}

void eLogClubLog::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
    //qDebug() << "eLogClubLog::slotErrorManagement: " << QString::number(networkError) << endl;
    result = networkError;

    if (result == QNetworkReply::NoError)
    {
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
         //qDebug() << "eLogClubLog::slotErrorManagement: Host not found" << endl;
    }
    else
    {
         //qDebug() << "eLogClubLog::slotErrorManagement: ERROR!" << endl;
    }

    //actionError(result);
}


int eLogClubLog::sendQSO(QStringList _qso)
{
    //qDebug() << "eLogClubLog::sendQSO: " << call <<"/"<< email << "/" << pass << "/" << api  << endl;
     //qDebug() << "eLogClubLog::sendQSO:: length = " << QString::number(_qso.length()) << endl;
    // First Data in the QStringList is the QSO id, not to be sent to clublog but used in the signal actionReturnDownload(const int _i, const int _qsoId);

    if (_qso.length()!=18)
    {
        return -1;
    }
    currentQSO = (_qso.at(0)).toInt();

    _qso.removeFirst();

    stationCallsign = QString();
    if (useQSOStationCallsign)
    {
        stationCallsign = _qso.last();      
    }
    //qDebug() << "eLogClubLog::sendQSO (stationCallsign =  " << _qso.last() << ")" << endl;
     _qso.removeLast();

    QString qso = getClubLogAdif(_qso);
    //qDebug() << "eLogClubLog::sendQSO: " << qso << endl;
    return sendData(qso);

}


int eLogClubLog::sendData(const QString _q)
{
    //qDebug() << "eLogClubLog::sendData: " << _q << endl;


    //Posiblemente tenga que usar una de estas: void QUrlQuery::addQueryItem(const QString & key, const QString & value)

    QUrl serviceUrl = QUrl("https://secure.clublog.org/realtime.php");
    QByteArray postData;

    //QByteArray postData;
/*
    QUrl params;
    params.addQueryItem("email",email);
    params.addQueryItem("password",pass);
    if ((useQSOStationCallsign) && (stationCallsign.length()>2))
    {
        params.addQueryItem("callsign",stationCallsign);
        //qDebug() << "eLogClubLog::sendData - callsign 1: " << stationCallsign << endl;
    }
    else
    {
        params.addQueryItem("callsign",call);
        //qDebug() << "eLogClubLog::sendData - callsign 2: " << call << endl;
    }

    params.addQueryItem("api",api);
    params.addQueryItem("adif",_q);

    postData = params.encodedQuery();
*/
    QUrlQuery params;
    params.addQueryItem("email",email);
    params.addQueryItem("password",pass);
    if ((useQSOStationCallsign) && (stationCallsign.length()>2))
    {
        params.addQueryItem("callsign",stationCallsign);
        //qDebug() << "eLogClubLog::sendData - callsign 1: " << stationCallsign << endl;
    }
    else
    {
        params.addQueryItem("callsign",call);
        //qDebug() << "eLogClubLog::sendData - callsign 2: " << call << endl;
    }

    params.addQueryItem("api",api);
    params.addQueryItem("adif",_q);

    postData = params.query(QUrl::FullyEncoded).toUtf8();
    //postData = params.encodedQuery();

    // Call the webservice
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotQsoUploadFinished(QNetworkReply*)));

    networkManager->post(request, postData);

    return -1;

}


QString eLogClubLog::getClubLogAdif(const QStringList _q)

{
     //qDebug() << "eLogClubLog::getClubLogAdif: " << QString::number(_q.length()) << endl;
    // _qso must include 16 ordered fields than can be empty or contain data. This function builds the ADIF QSO
/* http://clublog.freshdesk.com/support/solutions/articles/53202-which-adif-fields-does-club-log-use-
ClubLog only accepts the following ADIF fields:
  •QSO_DATE
  •TIME_ON
  •QSLRDATE
  •QSLSDATE
  •CALL
  •OPERATOR
  •MODE
  •BAND
  •BAND_RX
  •FREQ
  •QSL_RCVD
  •LOTW_QSL_RCVD
  •QSL_SENT
  •DXCC
  •PROP_MODE
  •CREDIT_GRANTED
*/
    if (_q.length()!=16)
    {
        return QString();
    }

    QString qso, aux1;
    qso.clear();

    aux1 = _q.at(0);
    if ((aux1.length()) == 10){
        aux1.remove(QChar('-'), Qt::CaseInsensitive);
        aux1.remove(QChar('/'), Qt::CaseInsensitive);
        qso = "<QSO_DATE:" + QString::number(aux1.length()) + ">" +  aux1  +  " ";
    }

    //qso = "<QSO_DATE:" + QString::number((_q.at(0)).length()) + ">" + _q.at(0) + " ";
    qso = qso + "<TIME_ON:" + QString::number((_q.at(1)).length()) + ">" + _q.at(1) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 10"  << endl;
    qso = qso + "<QSLRDATE:" + QString::number((_q.at(2)).length()) + ">" + _q.at(2) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 20"  << endl;
    qso = qso + "<QSLSDATE:" + QString::number((_q.at(3)).length()) + ">" + _q.at(3) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 30"  << endl;
    qso = qso + "<CALL:" + QString::number((_q.at(4)).length()) + ">" + _q.at(4) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 40"  << endl;
    qso = qso + "<OPERATOR:" + QString::number((_q.at(5)).length()) + ">" + _q.at(5) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 50"  << endl;
    qso = qso + "<MODE:" + QString::number((_q.at(6)).length()) + ">" + _q.at(6) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 60"  << endl;
    qso = qso + "<BAND:" + QString::number((_q.at(7)).length()) + ">" + _q.at(7) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 70"  << endl;
    if ((_q.at(8)).length()> 2)
    {
        qso = qso + "<BAND_RX:" + QString::number((_q.at(8)).length()) + ">" + _q.at(8) + " ";
    }

    if ((_q.at(9)).length()> 2)
    {
        qso = qso + "<FREQ:" + QString::number((_q.at(9)).length()) + ">" + _q.at(9) + " ";
    }


 //qDebug() << "eLogClubLog::getClubLogAdif: 90"  << endl;
    qso = qso + "<QSL_RCVD:" + QString::number((_q.at(10)).length()) + ">" + _q.at(10) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 100"  << endl;
    qso = qso + "<LOTW_QSL_RCVD:" + QString::number((_q.at(11)).length()) + ">" + _q.at(11) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 110"  << endl;
    qso = qso + "<QSL_SENT:" + QString::number((_q.at(12)).length()) + ">" + _q.at(12) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 120"  << endl;
    if ((_q.at(13)).toInt()> 0)
    {
        qso = qso + "<DXCC:" + QString::number((_q.at(13)).length()) + ">" + _q.at(13) + " ";
    }


 //qDebug() << "eLogClubLog::getClubLogAdif: 130'"  << endl;
    qso = qso + "<PROP_MODE:" + QString::number((_q.at(14)).length()) + ">" + _q.at(14) + " ";
 //qDebug() << "eLogClubLog::getClubLogAdif: 140"  << endl;
    qso = qso + "<CREDIT_GRANTED:" + QString::number((_q.at(15)).length()) + ">" + _q.at(15) + " ";
     //qDebug() << "eLogClubLog::getClubLogAdif: 150"  << endl;
    qso = qso + "<EOR>";
    //qDebug() << "eLogClubLog:: - QSO: "  << qso << endl;


     //qDebug() << "eLogClubLog::getClubLogAdif: 100"  << endl;
    return qso;
}

void eLogClubLog::setCredentials(const QString _call, const QString _email, const QString _pass, const bool _useQSOStationCall)
{
    //qDebug() << "eLogClubLog::setCredentials" << endl;
    call = _call;
    email = _email;
    pass = _pass;
    useQSOStationCallsign = _useQSOStationCall;

}


int eLogClubLog::deleteQSO(QStringList _qso)
{
    //qDebug() << "eLogClubLog::deleteQSO: length = " << QString::number(_qso.length()) << endl;
    if (_qso.length()!=17)
    {
        return -1;
    }

    _qso.removeFirst();

//http://clublog.freshdesk.com/support/solutions/articles/54908-using-adif-to-update-or-delete-qsos
//<CALL:5>VP9NO<QSO_DATE:8>20070903<TIME_ON:6>213300<BAND:3>30M<MODE:2>CW<QSLCALL:5>GH6UW<EOR>

    QString replaceCall = "<QSLCALL:" + QString::number(call.length()) + ">" + call + " " + "<EOR>";

    QString qso = getClubLogAdif(_qso);
    qso.replace("<EOR>", replaceCall);

    //qDebug() << "eLogClubLog::deleteQSO: ready to send = " << qso << endl;

    return sendData(qso);


}

QString eLogClubLog::prepareToTranslate(const QString _m)
{
    //qDebug() << "eLogClubLog:: = prepareToTranslate" << _m << endl;
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


int eLogClubLog::modifyQSO (QStringList _oldQSO, QStringList _newQSO)
{
   int x = -1;

    x = deleteQSO(_oldQSO);
    x = sendQSO(_newQSO);
    return x;
}
