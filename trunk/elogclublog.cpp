#include "elogclublog.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
#include <QDebug>



eLogClubLog::eLogClubLog() : QObject(0)
{    
    call= QString();
    email = QString();
    pass = QString();
    api = "9467beee93377e82a276b0a777d388b5c933d044";
}

eLogClubLog::~eLogClubLog()
{
    qDebug() << "eLogClubLog::~eLogClubLog"  << endl;
}


QString eLogClubLog::prepareToTranslate(const QString _m)
{
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
    else if (_m == "Callsign is your ow call")
    {
        return tr("Callsign is your ow call");
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
    else if (_m == "Excesive API Usage")
    {
        return tr("Excesive API Usage");
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

 void eLogClubLog::downloadFinished(QNetworkReply *data)
{
    qDebug() << "eLogClubLog::downloadFinished"  << endl;

    result = data->error();
    //qDebug() << "eLogClubLog::downloadFinished - Result = " << QString::number(result) << endl;

    const QByteArray sdata = data->readAll();

    QString text = "ClubLog: " + prepareToTranslate(sdata);



    if (result == QNetworkReply::NoError)
    {


        qDebug() << sdata;

        emit actionReturnDownload(result);
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
        qDebug() << "eLogClubLog::downloadFinished - Result = Host Not found! = " << QString::number(result)  << endl;
    }
    else if (result == QNetworkReply::TimeoutError)
    {
        qDebug() << "eLogClubLog::downloadFinished - Result = Time out error! = " << QString::number(result)  << endl;
    }
    else
    {
        qDebug() << "eLogClubLog::downloadFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
    }

    //qDebug() << "eLogClubLog::downloadFinished - Result = " << QString::number(result) << endl;
    //emit done();
    emit  showMessage(text);

}


void eLogClubLog::downloadProgress(qint64 received, qint64 total) {
    qDebug() << received << total;
    //qDebug() << "eLogClubLog::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << endl;
    emit actionShowProgres(received, total);
}

void eLogClubLog::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
    qDebug() << "eLogClubLog::slotErrorManagement: " << QString::number(networkError) << endl;
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


int eLogClubLog::sendQSO(const QStringList _qso)
{
    qDebug() << "eLogClubLog::sendQSO: " << call <<"/"<< email << "/" << pass << "/" << api  << endl;
    qDebug() << "eLogClubLog::sendQSO:: length = " << QString::number(_qso.length()) << endl;
    if (_qso.length()!=16)
    {
        return -1;
    }
    QString qso = getClubLogAdif(_qso);
    qDebug() << "eLogClubLog::sendQSO: " << qso << endl;
    return sendData(qso);

}


int eLogClubLog::sendData(const QString _q)
{
    qDebug() << "eLogClubLog::sendData: " << _q<< endl;

    QUrl serviceUrl = QUrl("https://secure.clublog.org/realtime.php");
    QByteArray postData;

    //QByteArray postData;

    QUrl params;
    params.addQueryItem("email",email);
    params.addQueryItem("password",pass);
    params.addQueryItem("callsign",call);
    params.addQueryItem("api",api);
    params.addQueryItem("adif",_q);

    postData = params.encodedQuery();

    // Call the webservice
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    networkManager->post(request, postData);

    return -1;

}

QString eLogClubLog::getClubLogAdif(const QStringList _q)

{
    qDebug() << "eLogClubLog::getClubLogAdif: " << QString::number(_q.length()) << endl;
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

qDebug() << "eLogClubLog::getClubLogAdif: 1"  << endl;

    //qso = "<QSO_DATE:" + QString::number((_q.at(0)).length()) + ">" + _q.at(0) + " ";
    qso = qso + "<TIME_ON:" + QString::number((_q.at(1)).length()) + ">" + _q.at(1) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 10"  << endl;
    qso = qso + "<QSLRDATE:" + QString::number((_q.at(2)).length()) + ">" + _q.at(2) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 20"  << endl;
    qso = qso + "<QSLSDATE:" + QString::number((_q.at(3)).length()) + ">" + _q.at(3) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 30"  << endl;
    qso = qso + "<CALL:" + QString::number((_q.at(4)).length()) + ">" + _q.at(4) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 40"  << endl;
    qso = qso + "<OPERATOR:" + QString::number((_q.at(5)).length()) + ">" + _q.at(5) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 50"  << endl;
    qso = qso + "<MODE:" + QString::number((_q.at(6)).length()) + ">" + _q.at(6) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 60"  << endl;
    qso = qso + "<BAND:" + QString::number((_q.at(7)).length()) + ">" + _q.at(7) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 70"  << endl;
    qso = qso + "<BAND_RX:" + QString::number((_q.at(8)).length()) + ">" + _q.at(8) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 80"  << endl;
    qso = qso + "<FREQ:" + QString::number((_q.at(9)).length()) + ">" + _q.at(9) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 90"  << endl;
    qso = qso + "<QSL_RCVD:" + QString::number((_q.at(10)).length()) + ">" + _q.at(10) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 100"  << endl;
    qso = qso + "<LOTW_QSL_RCVD:" + QString::number((_q.at(11)).length()) + ">" + _q.at(11) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 110"  << endl;
    qso = qso + "<QSL_SENT:" + QString::number((_q.at(12)).length()) + ">" + _q.at(12) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 120"  << endl;
    qso = qso + "<DXCC:" + QString::number((_q.at(13)).length()) + ">" + _q.at(13) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 130'"  << endl;
    qso = qso + "<PROP_MODE:" + QString::number((_q.at(14)).length()) + ">" + _q.at(14) + " ";
qDebug() << "eLogClubLog::getClubLogAdif: 140"  << endl;
    qso = qso + "<CREDIT_GRANTED:" + QString::number((_q.at(15)).length()) + ">" + _q.at(15) + " ";
    qDebug() << "eLogClubLog::getClubLogAdif: 150"  << endl;
    qso = qso + "<EOR>";
    //qDebug() << "eLogClubLog::getCLubLogAdif - QSO: "  << qso << endl;


    qDebug() << "eLogClubLog::getClubLogAdif: 100"  << endl;
    return qso;
}

void eLogClubLog::setCredentials(const QString _call, const QString _email, const QString _pass)
{
    qDebug() << "eLogClubLog::setCredentials" << endl;
    call = _call;
    email = _email;
    pass = _pass;

}


int eLogClubLog::deleteQSO(const QStringList _qso)
{
    qDebug() << "eLogClubLog::deleteQSO: length = " << QString::number(_qso.length()) << endl;
    if (_qso.length()!=16)
    {
        return -1;
    }

//http://clublog.freshdesk.com/support/solutions/articles/54908-using-adif-to-update-or-delete-qsos
//<CALL:5>VP9NO<QSO_DATE:8>20070903<TIME_ON:6>213300<BAND:3>30M<MODE:2>CW<QSLCALL:5>GH6UW<EOR>

    QString replaceCall = "<QSLCALL:" + QString::number(call.length()) + ">" + call + " " + "<EOR>";

    QString qso = getClubLogAdif(_qso);
    qso.replace("<EOR>", replaceCall);

    return sendData(qso);

}
