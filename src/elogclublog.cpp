/***************************************************************************
                          elogclublog.cpp  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
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

#include "elogclublog.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

//https://clublog.freshdesk.com/support/solutions/59800

eLogClubLog::eLogClubLog()
{
    //qDebug()<< "eLogClubLog::eLogClubLog" ;

    //email = QString();
    //pass = QString();
    qsos.clear();
    api = "9467beee93377e82a276b0a777d388b5c933d044";
    currentQSO = -1;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotQsoUploadFinished(QNetworkReply*)));
    //stationCallsign = QString();
    uploadingFile = false;
    util = new Utilities(Q_FUNC_INFO);
    //qDebug()<< "eLogClubLog::eLogClubLog - END" ;
}

eLogClubLog::~eLogClubLog()
{
    delete(util);
    delete(manager);
          //qDebug()<< "eLogClubLog::~eLogClubLog" ;
}


 void eLogClubLog::slotQsoUploadFinished(QNetworkReply *data)
{
      //qDebug()<< "eLogClubLog::slotQsoUploadFinished" ;

    result = data->error();
    //qDebug()<< Q_FUNC_INFO << " - Result = " << QString::number(result);

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
          //qDebug()<< sdata;
          //qDebug()<< Q_FUNC_INFO << " - NO ERROR";
        if (uploadingFile)
        {
            uploadingFile = false;
            emit signalFileUploaded(result, qsos);
            qsos.clear();
            return;
        }
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
          //qDebug()<< Q_FUNC_INFO << " - Result = Host Not found! = " << QString::number(result) ;
        text = "ClubLog: " + tr("Host not found!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == QNetworkReply::TimeoutError)
    {
          //qDebug()<< Q_FUNC_INFO << " - Result = Time out error! = " << QString::number(result) ;
        text = "ClubLog: " + tr("Timeout error!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == 201)
    {
        text = "ClubLog: " + tr("It seems to be a CREDENTIALS ERROR; check your email and password.");

        QMessageBox::warning(nullptr, tr("KLog - ClubLog"),
                                       tr("It seems that your ClubLog credentials are not correct.") + "\n" +
                                          tr("Please check your credentials in the setup. ClubLog uploads will be disabled."),
                                       QMessageBox::Ok);
        emit disableClubLogAction(true);
    }
    /*
    else if (result == 202)
    {
          //qDebug()<< Q_FUNC_INFO << " - Result = Password Error! = " << QString::number(result) ;
        text = "ClubLog: " + tr("It seems to be a PASSWORD ERROR; check your password.");

        int i = QMessageBox::warning(nullptr, tr("KLog - ClubLog"),
                                       tr("It seems that your ClubLog password is not correct.") + "\n" +
                                          tr("Please check your password in the setup. ClubLog uploads will be disabled."),
                                       QMessageBox::Ok);

        emit disableClubLogAction(true);
        //TODO: Mark the previous QSO as not sent to clublog
    }

    */
    else if (result == 203)
    {
        text = "ClubLog: " + tr("QSO dupe or not existing (#%1)... ").arg(result);
        QMessageBox::warning(nullptr, tr("KLog - ClubLog"),
                                       tr("We have received an undefined error from Clublog (%1)").arg(result) + "\n" +
                                          tr("This error may be caused for the QSO being duplicated or, if removing, trying to remove a non existing QSO."),
                                       QMessageBox::Ok);
    }
    else
    {
          //qDebug()<< Q_FUNC_INFO << " - Result = UNDEFINED = " << QString::number(result) ;
        text = "ClubLog: " + tr("Undefined error number (#%1)... ").arg(result);
        QMessageBox::warning(nullptr, tr("KLog - ClubLog"),
                                       tr("We have received an undefined error from Clublog (%1)").arg(result) + "\n" +
                                          tr("Please check your config in the setup and contact the KLog development team if you can't fix it. ClubLog uploads will be disabled."),
                                       QMessageBox::Ok);
        emit disableClubLogAction(true);
        //TODO: Mark the previous QSO as not sent to clublog
    }

     //qDebug()<< Q_FUNC_INFO << " - Result = " << QString::number(result);
     //qDebug()<< Q_FUNC_INFO << "  - Result Text = " << text;
    //emit done();
    //emit signalFileUploaded(result, qsos);
    emit showMessage(text);
}

void eLogClubLog::slotFileUploadFinished(QNetworkReply *data)
{
         //qDebug()<< "eLogClubLog::slotFileUploadFinished" ;

    result = data->error();
          //qDebug()<< "eLogClubLog::slotFileUploadFinished - Result = " << QString::number(result);

    const QByteArray sdata = data->readAll();

    QString text;

    if (result == QNetworkReply::NoError)
    {
        text = "ClubLog: " + prepareToTranslate(sdata);
             //qDebug()<< "eLogClubLog::slotFileUploadFinished - Result = NoError = " << QString::number(result) ;
             //qDebug()<< sdata;
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
             //qDebug()<< "eLogClubLog::slotFileUploadFinished - Result = Host Not found! = " << QString::number(result) ;
        text = "ClubLog: " + tr("Host not found!");
    }
    else if (result == QNetworkReply::TimeoutError)
    {
             //qDebug()<< "eLogClubLog::slotFileUploadFinished - Result = Time out error! = " << QString::number(result) ;
        text = "ClubLog: " + tr("Timeout error!");
    }
    else
    {
             //qDebug()<< "eLogClubLog::slotFileUploadFinished - Result = UNDEFINED = " << QString::number(result) ;
        text = "ClubLog: " + tr("Undefined error...");
    }

          //qDebug()<< "eLogClubLog::slotFileUploadFinished - Result = " << QString::number(result);
    //emit done();
    emit  showMessage(text);
}

void eLogClubLog::downloadProgress(qint64 received, qint64 total) {
         //qDebug()<< "eLogClubLog::downloadProgress: " << QString::number(received) << "/" << QString::number(total);

         //qDebug()<< received << total;
    emit actionShowProgres(received, total);
}

void eLogClubLog::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
         //qDebug()<< "eLogClubLog::slotErrorManagement: " << QString::number(networkError);
    result = networkError;
}

int eLogClubLog::sendQSO(QStringList _qso)
{
      //qDebug() << "eLogClubLog::sendQSO: " << email << "/" << pass << "/" << api ;
       //qDebug()<< "eLogClubLog::sendQSO:: length = " << QString::number(_qso.length());
    // First Data in the QStringList is the QSO id, not to be sent to clublog but used in the signal actionReturnDownload(const int _i, const int _qsoId);
   for(int i = 0; i<_qso.length(); i++)
    {
         //qDebug()<< "eLogClubLog::sendQSO = qso-at: "<< QString::number(i) << "- " << _qso.at(i);
    }
    if (_qso.length()!=18)
    {
          //qDebug() << "eLogClubLog::sendQSO:: length - END";
        return -1;
    }

    currentQSO = (_qso.at(0)).toInt();
    _qso.removeFirst();
    //_qso.removeFirst();
     //qDebug()<< "eLogClubLog::sendQSO CALL TO USE (before checking): " << _qso.at(16);
     //qDebug()<< "eLogClubLog::sendQSO default StationCallsign): " << stationCallsign;

    QString tempCall = _qso.at(16);
    if (tempCall.length()<1)
    {
        tempCall = stationCallsign;
    }
     //qDebug()<< "eLogClubLog::sendQSO CALL TO USE: " << tempCall;
    _qso.removeLast();

    QString qso = getClubLogAdif(_qso);
      //qDebug()<< "eLogClubLog::sendQSO: " << qso;
    QUrlQuery params;
    params.addQueryItem("adif",qso);
     //qDebug() << "eLogClubLog::sendQSO: QSO: " << qso;
      //qDebug() << "eLogClubLog::sendQSO: END";
    uploadingFile = false;
    return sendDataParams(tempCall, params, true);
    //return sendData(tempCall, qso);
}
/*
int eLogClubLog::deleteQSOid(const int _qsoId)
{
     //qDebug()<< "eLogClubLog::deleteQSOid: " << QString::number(_qsoId);
   // email, password, callsign, dxcall, datetime, bandid, api
    QString dxcall, datatime, bandid;
    QUrlQuery params;
    params

}
*/

int eLogClubLog::sendDataParams(const QString &_clublogCall, const QUrlQuery &_params, bool _adding)
{
      //qDebug()<< Q_FUNC_INFO << ":  Call: " << _clublogCall;
      //qDebug()<< Q_FUNC_INFO << ": : Params: " << _params.query(QUrl::FullyEncoded).toUtf8();
      //qDebug()<< Q_FUNC_INFO << ":  email = " << email;
      //qDebug()<< Q_FUNC_INFO << ":  Pass = " << pass;

    QUrl serviceUrl;
    if (_adding)
    {
     //serviceUrl = QUrl("https://secure.clublog.org/realtime.php");
        serviceUrl = QUrl("https://clublog.org/realtime.php");
    }
    else
    {
        //serviceUrl = QUrl("https://secure.clublog.org/delete.php");
      serviceUrl = QUrl("https://clublog.org/delete.php");
    }

    QByteArray postData;

    QUrlQuery params;
    params.addQueryItem("email",email);
    params.addQueryItem("password",pass);

    if  (_clublogCall.length()>2)
    {
        params.addQueryItem("callsign",_clublogCall);
           //qDebug()<< Q_FUNC_INFO << ":  - callsign 1: " << _clublogCall;
    }
    else
    {
        params.addQueryItem("callsign",stationCallsign);
    }

      //qDebug()<< Q_FUNC_INFO << ":  - query before send/delete: " << params.query(QUrl::FullyEncoded).toUtf8();
    if (_adding)
    {
        params.addQueryItem("api",api);
        postData = params.query(QUrl::FullyEncoded).toUtf8();
        postData = postData + "&" + _params.query(QUrl::FullyEncoded).toUtf8();
        params.setQuery(postData);

        //TODO: ADD the params
    }
    else
    {
        //TODO: ADD the params
        postData = params.query(QUrl::FullyEncoded).toUtf8();
        postData = postData + "&" + _params.query(QUrl::FullyEncoded).toUtf8() ;
        params.setQuery(postData);
        params.addQueryItem("api",api);
    }

    postData = params.query(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //qDebug() << Q_FUNC_INFO << ": " << postData;
    manager->post(request, postData);
    //qDebug()<< Q_FUNC_INFO << ":  - END";
    return -1;
}

/*
int eLogClubLog::sendData(const QString &_clublogCall, const QString &_q)
{
      //qDebug()<< "eLogClubLog::sendData: " << _q;

    QUrl serviceUrl = QUrl("https://secure.clublog.org/realtime.php");
    QByteArray postData;

    //QByteArray postData;

    QUrlQuery params;
    params.addQueryItem("email",email);
    params.addQueryItem("password",pass);
    if  (_clublogCall.length()>2)
    {
        params.addQueryItem("callsign",_clublogCall);
             //qDebug()<< "eLogClubLog::sendData - callsign 1: " << _clublogCall;
    }
    else
    {
        params.addQueryItem("callsign",stationCallsign);
        //ERROR
    }

    params.addQueryItem("api",api);
    params.addQueryItem("adif",_q);

    postData = params.query(QUrl::FullyEncoded).toUtf8();
       //qDebug()<< "eLogClubLog::sendData - query: " << postData;
    //postData = params.encodedQuery();

    // Call the webservice
    //QNetworkAccessManager *networkManager = new QNetworkAccessManager;

    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotQsoUploadFinished(QNetworkReply*)));

    manager->post(request, postData);

    return -1;

}

*/
QString eLogClubLog::getClubLogAdif(const QStringList _q)

{
      //qDebug() << "eLogClubLog::getClubLogAdif: " << QString::number(_q.length());
    // _qso must include 16 ordered fields than can be empty or contain data. This function builds the ADIF QSO
/* http://clublog.freshdesk.com/support/solutions/articles/53202-which-adif-fields-does-club-log-use-
ClubLog only accepts the following ADIF fields:
QSO_DATE
TIME_ON
TIME_OFF (this is stored independently and used for log matching as well as ADIF exports)
QSLRDATE
QSLSDATE
CALL
OPERATOR
MODE
BAND
BAND_RX
FREQ
QSL_RCVD
LOTW_QSL_RCVD
QSL_SENT
DXCC
PROP_MODE
CREDIT_GRANTED
RST_SENT
RST_RCVD
NOTES
*/
    if (_q.length()!=16)
    {
        return QString();
    }
    for (int i = 0; i< _q.length(); i++)
    {
          //qDebug()<< QString("eLogClubLog::getClubLogAdif: (%1): %2").arg(i).arg(_q.at(i));
    }

    QString qso, aux1;
    qso.clear();
    aux1 = _q.at(0);
      //qDebug()<< "eLogClubLog::getClubLogAdif: Date: " << _q.at(0) ;
    if (QDate::fromString(aux1, "yyyyMMdd").isValid())
    {
        qso = "<QSO_DATE:" + QString::number(aux1.length()) + ">" +  aux1  +  " ";
    }
    else
    {
          //qDebug() << "eLogClubLog::getClubLogAdif: END error ";
        return QString();
    }

    //qso = "<QSO_DATE:" + QString::number((_q.at(0)).length()) + ">" + _q.at(0) + " ";
    qso = qso + "<TIME_ON:" + QString::number((_q.at(1)).length()) + ">" + _q.at(1) + " ";
       //qDebug()<< "eLogClubLog::getClubLogAdif: 10" ;
    if (!(_q.at(2)).isEmpty())
    {
        qso = qso + "<QSLRDATE:" + QString::number((_q.at(2)).length()) + ">" + _q.at(2) + " ";
    }
       //qDebug()<< "eLogClubLog::getClubLogAdif: 20" ;
    if (!(_q.at(3)).isEmpty())
    {
        qso = qso + "<QSLSDATE:" + QString::number((_q.at(3)).length()) + ">" + _q.at(3) + " ";
    }

       //qDebug()<< "eLogClubLog::getClubLogAdif: 30" ;
    qso = qso + "<CALL:" + QString::number((_q.at(4)).length()) + ">" + _q.at(4) + " ";
       //qDebug()<< "eLogClubLog::getClubLogAdif: 40" ;
    if (!(_q.at(5)).isEmpty())
    {
        qso = qso + "<OPERATOR:" + QString::number((_q.at(5)).length()) + ">" + _q.at(5) + " ";
    }
       //qDebug()<< "eLogClubLog::getClubLogAdif: 50" ;
    qso = qso + "<MODE:" + QString::number((_q.at(6)).length()) + ">" + _q.at(6) + " ";
       //qDebug()<< "eLogClubLog::getClubLogAdif: 60" ;
    qso = qso + "<BAND:" + QString::number((_q.at(7)).length()) + ">" + _q.at(7) + " ";
       //qDebug()<< "eLogClubLog::getClubLogAdif: 70" ;
    if ((_q.at(8)).length()> 2)
    {
        qso = qso + "<BAND_RX:" + QString::number((_q.at(8)).length()) + ">" + _q.at(8) + " ";
    }

    if ((_q.at(9)).length()> 2)
    {
        qso = qso + "<FREQ:" + QString::number((_q.at(9)).length()) + ">" + _q.at(9) + " ";
    }

       //qDebug()<< "eLogClubLog::getClubLogAdif: 90" ;
    qso = qso + "<QSL_RCVD:" + QString::number((_q.at(10)).length()) + ">" + _q.at(10) + " ";
       //qDebug()<< "eLogClubLog::getClubLogAdif: 100" ;
    qso = qso + "<LOTW_QSL_RCVD:" + QString::number((_q.at(11)).length()) + ">" + _q.at(11) + " ";
      //qDebug()<< "eLogClubLog::getClubLogAdif: 110" ;
    //qso = qso + "<QSL_SENT:" + QString::number((_q.at(12)).length()) + ">" + _q.at(12) + " ";
       //qDebug()<< "eLogClubLog::getClubLogAdif: 120" ;
    if ((_q.at(13)).toInt()> 0)
    {
        qso = qso + "<DXCC:" + QString::number((_q.at(13)).length()) + ">" + _q.at(13) + " ";
    }

       //qDebug()<< "eLogClubLog::getClubLogAdif: 130'" ;
    if ((_q.at(14)).toInt()> 0)
    {
        qso = qso + "<PROP_MODE:" + QString::number((_q.at(14)).length()) + ">" + _q.at(14) + " ";
    }

       //qDebug()<< "eLogClubLog::getClubLogAdif: 140" ;
    if (!(_q.at(15)).isEmpty())
    {
        qso = qso + "<CREDIT_GRANTED:" + QString::number((_q.at(15)).length()) + ">" + _q.at(15) + " ";
    }

       //qDebug()<< "eLogClubLog::getClubLogAdif: 150" ;
    qso = qso + "<EOR>";
      //qDebug()<< "eLogClubLog:: - QSO: "  << qso;
       //qDebug()<< "eLogClubLog::getClubLogAdif: 100" ;
    return qso;
}

void eLogClubLog::setDefaultCallsign(const QString &_defaultStationCallsign)
{
    stationCallsign = _defaultStationCallsign;
}

int eLogClubLog::deleteQSO(QStringList _qso)
{
      //qDebug()<< "eLogClubLog::deleteQSO: length = " << QString::number(_qso.length());
      //qDebug()<< "eLogClubLog::deleteQSO: " << email << "/" << pass << "/" << api ;
      //qDebug()<< "eLogClubLog::deleteQSO: email = " << email;
      //qDebug()<< "eLogClubLog::deleteQSO: Pass = " << pass;

    // email, password, callsign, dxcall, datetime (sqlite format, not ADIF), bandid (only the number, not ADIF), api
    if (_qso.length()!=18)
    {
        return -1;
    }
    for (int i = 0; i<_qso.length(); i++)
    {
          //qDebug()<< QString("eLogClubLog::deleteQSO: qso.at(%1) = %2").arg(i).arg(_qso.at(i));
    }

    QString dxcall, sdateTime, bandid;

    QString tempCall = _qso.at(17);
    if (tempCall.length()<1)
    {
        tempCall = stationCallsign;
    }

    dxcall = _qso.at(5);
    QDateTime dateTime;
    dateTime.setDate(QDate::fromString(_qso.at(1), "yyyyMMdd"));
    dateTime.setTime(QTime::fromString(_qso.at(2), "HHmmss"));
    if (!dateTime.isValid())
    {
        return -1;
    }

    sdateTime = dateTime.toString("yyyy-MM-dd HH:mm:ss");
      //qDebug()<< QString("eLogClubLog::deleteQSO: DateTime = %1").arg(sdateTime);
       //qDebug()<< QString("eLogClubLog::deleteQSO: band-1: ") << _qso.at(8);
    bandid = (_qso.at(8)).chopped(1);
       //qDebug()<< QString("eLogClubLog::deleteQSO: band-2: ") << bandid;
    bool ok;
    bandid.toInt(&ok);
    if (!ok)
    { // This check is to capture potential QSOs in 222Mhz (AKA 1.25)
        bandid = bandid.chopped(1);
          //qDebug()<< QString("eLogClubLog::deleteQSO: band-3: ") << bandid;
    }
      //qDebug()<< QString("eLogClubLog::deleteQSO: bandid = %1").arg(bandid);
    if (bandid.toInt()<=0)
    {
        return -2;
    }

    QUrlQuery params;
    params.addQueryItem("dxcall", dxcall);
    params.addQueryItem("datetime", sdateTime);
    params.addQueryItem("bandid", bandid);
    //params.addQueryItem("adif",qso);
    uploadingFile = false;
    return sendDataParams(tempCall, params, false);
    //return sendData(qso);
}

QString eLogClubLog::prepareToTranslate(const QString &_m)
{
         //qDebug()<< "eLogClubLog:: = prepareToTranslate" << _m;
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
       //qDebug()<< QString("eLogClubLog::modifyQSO");
   int x = -1;

    x = deleteQSO(_oldQSO);
    x = sendQSO(_newQSO);
    return x;
}

void eLogClubLog::sendLogFile(const QString &_file, QList<int> _qso, bool _overwrite)
{
    //qDebug()<< Q_FUNC_INFO << ": " << _file;
    qsos.clear();
    qsos.append(_qso);
    QUrl serviceUrl;
    serviceUrl = QUrl("https://clublog.org/putlogs.php");

    QByteArray postData;

    QUrlQuery params;

    // FIRST PARAMS is the file
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QByteArray blob;

    //QFile *file = new QFile("_file");
    QFile *file = new QFile(util->getClubLogFile());
    if (file->open(QIODevice::ReadOnly))        /* Flawfinder: ignore */
    {
         blob = file->readAll();
    }
    else
    {
           //qDebug()<< Q_FUNC_INFO << ":  ERROR File not opened";
        return;
    }
    file->close();
    // The rest of the form goes as usual
    //qDebug()<< Q_FUNC_INFO << ": email: " << email;
    //qDebug()<< Q_FUNC_INFO << ": apPass: " << appPass;
    //qDebug()<< Q_FUNC_INFO << ": stationcall: " << stationCallsign;
    //qDebug()<< Q_FUNC_INFO << ": api: " << api;

    QHttpPart emailPart;
    emailPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"email\""));
    emailPart.setBody(email.toUtf8());

    QHttpPart passPart;
    passPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"password\""));
    passPart.setBody(appPass.toUtf8());

    QHttpPart callPart;
    callPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"callsign\""));
    callPart.setBody(stationCallsign.toUtf8());

    QHttpPart apiPart;
    apiPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api\""));
    apiPart.setBody(api.toUtf8());
    QString one;
    if (_overwrite)
    {
        one  = QString("1");
    }
    else
    {
        one = QString("0");
    }
    //QString one = QString("1");
    QHttpPart clearPart;
    clearPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"clear\""));
    clearPart.setBody(one.toUtf8());

    QHttpPart filePart;
    QString aux = QString("form-data; name=\"file\"; filename=\"%1\"").arg(_file);
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(aux));
    filePart.setBody(blob);

    multiPart->append(filePart);
    multiPart->append(emailPart);
    multiPart->append(passPart);
    multiPart->append(callPart);
    multiPart->append(clearPart);
    multiPart->append(apiPart);

    uploadingFile = true;

    QNetworkRequest request(serviceUrl);
    manager->post(request, multiPart);
    //qDebug() << Q_FUNC_INFO << ": " << multiPart;

    //qDebug()<< "eLogClubLog::sendLogFile - END";
}

void eLogClubLog::loadSettings()
{
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("ClubLog");
    email = settings.value ("ClubLogEmail").toString ();
    pass = settings.value ("ClubLogPass").toString ();
    appPass = settings.value ("ClubLogAppPass").toString ();
    settings.endGroup ();
}


