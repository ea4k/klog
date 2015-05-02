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
   // manager = new QNetworkAccessManager();
    //QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(downloadFinished(QNetworkReply*)));

}

eLogClubLog::~eLogClubLog()
{
    qDebug() << "eLogClubLog::~eLogClubLog"  << endl;
}



 void eLogClubLog::downloadFinished(QNetworkReply *data)
{
    qDebug() << "eLogClubLog::downloadFinished"  << endl;

    result = data->error();
    //qDebug() << "eLogClubLog::downloadFinished - Result = " << QString::number(result) << endl;

    if (result == QNetworkReply::NoError)
    {
        const QByteArray sdata = data->readAll();

        qDebug() << sdata;
        qDebug() << "eLogClubLog::downloadFinished: and CTY file created!" << endl;
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
        //qDebug() << "eLogClubLog::downloadFinished: Host not found" << endl;
    }
    else
    {
        //qDebug() << "eLogClubLog::downloadFinished: ERROR!" << endl;
    }

    //actionError(result);
}


int eLogClubLog::sendQSO(const QStringList _qso)
{
    qDebug() << "eLogClubLog::sendQSO: " << call <<"/"<< email << "/" << pass << "/" << api  << endl;
    QString qso = getClubLogAdif(_qso);
    qDebug() << "eLogClubLog::sendQSO: " << qso << endl;

    //target = "https://secure.clublog.org/realtime.php";
    //QUrl url = QUrl::fromEncoded(this->target.toLocal8Bit());


    //QNetworkRequest request(url);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
   // reply = manager.get(request);
/*******************/
    QUrl serviceUrl = QUrl("https://secure.clublog.org/realtime.php");
    QByteArray postData;

    //QByteArray postData;

    QUrl params;
    params.addQueryItem("email",email);
    params.addQueryItem("password",pass);
    params.addQueryItem("callsign",call);
    params.addQueryItem("api",api);
    params.addQueryItem("adif",qso);

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
    qDebug() << "eLogClubLog::getCLubLogAdif" << endl;
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
    qso = qso + "<QSLRDATE:" + QString::number((_q.at(2)).length()) + ">" + _q.at(2) + " ";
    qso = qso + "<QSLSDATE:" + QString::number((_q.at(3)).length()) + ">" + _q.at(3) + " ";
    qso = qso + "<CALL:" + QString::number((_q.at(4)).length()) + ">" + _q.at(4) + " ";
    qso = qso + "<OPERATOR:" + QString::number((_q.at(5)).length()) + ">" + _q.at(5) + " ";
    qso = qso + "<MODE:" + QString::number((_q.at(6)).length()) + ">" + _q.at(6) + " ";
    qso = qso + "<BAND:" + QString::number((_q.at(7)).length()) + ">" + _q.at(7) + " ";
    qso = qso + "<BAND_RX:" + QString::number((_q.at(8)).length()) + ">" + _q.at(8) + " ";
    qso = qso + "<FREQ:" + QString::number((_q.at(9)).length()) + ">" + _q.at(9) + " ";
    qso = qso + "<QSL_RCVD:" + QString::number((_q.at(10)).length()) + ">" + _q.at(10) + " ";
    qso = qso + "<LOTW_QSL_RCVD:" + QString::number((_q.at(11)).length()) + ">" + _q.at(11) + " ";
    qso = qso + "<QSL_SENT:" + QString::number((_q.at(12)).length()) + ">" + _q.at(12) + " ";
    qso = qso + "<DXCC:" + QString::number((_q.at(13)).length()) + ">" + _q.at(13) + " ";
    qso = qso + "<PROP_MODE:" + QString::number((_q.at(14)).length()) + ">" + _q.at(14) + " ";
    qso = qso + "<CREDIT_GRANTED:" + QString::number((_q.at(15)).length()) + ">" + _q.at(15) + " ";
    qso = qso + "<EOR>";
    //qDebug() << "eLogClubLog::getCLubLogAdif - QSO: "  << qso << endl;
    return qso;
}

void eLogClubLog::setCredentials(const QString _call, const QString _email, const QString _pass)
{
    qDebug() << "eLogClubLog::setCredentials" << endl;
    call = _call;
    email = _email;
    pass = _pass;

}

