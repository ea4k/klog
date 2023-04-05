/***************************************************************************
                          eqslutilities.cpp  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
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

#include "eqslutilities.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
//#include <QDebug>

eQSLUtilities::eQSLUtilities(const QString &_parentFunction)
{
    //qDebug()<< "eQSLUtilities::eQSLUtilities" ;
    Q_UNUSED(_parentFunction);
#ifdef QT_DEBUG
  //qDebug() << Q_FUNC_INFO << ": " << _parentFunction;
#else
#endif
    user = QString();
    pass = QString();
    qsos.clear();

    currentQSO = -1;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotQsoUploadFinished(QNetworkReply*)));
    stationCallsign = QString();
    uploadingFile = false;
    util = new Utilities(Q_FUNC_INFO);
    //qDebug()<< "eQSLUtilities::eQSLUtilities - END" ;
}

eQSLUtilities::~eQSLUtilities()
{
    delete(util);
    //qDebug()<< "eQSLUtilities::~eQSLUtilities" ;
}

void eQSLUtilities::setUser(const QString &_call)
{
     //qDebug() << "eQSLUtilities::setUser: " << _call;
    user = _call;
     //qDebug() << "eQSLUtilities::setUser: END";
}

void eQSLUtilities::setPass(const QString &_pass)
{
     //qDebug() << "eQSLUtilities::setPass: " << _pass;
    pass = _pass;
     //qDebug() << "eQSLUtilities::setPass: END";
}

void eQSLUtilities::slotQsoUploadFinished(QNetworkReply *data)
{
    qDebug()<< "eQSLUtilities::slotQsoUploadFinished" ;
    QStringList parsedAnswer;
    parsedAnswer.clear();
    result = data->error();
    qDebug()<< Q_FUNC_INFO << " - Result = " << QString::number(result);

    const QByteArray sdata = data->readAll();
    QString text = QString();

    if (result == QNetworkReply::NoError)
    {

        parsedAnswer << prepareToTranslate(sdata);
        if (parsedAnswer.at(0).contains("Error"))
        {
            qDebug()<< Q_FUNC_INFO << " - error detected";
            QMessageBox::warning(nullptr, tr("KLog - eQSL"), tr("eQSL has sent the following message:\n%1").arg(parsedAnswer.at(1)), QMessageBox::Ok);
            qsos.clear();
            return;
        }

         qDebug()<< sdata;
        qDebug()<< Q_FUNC_INFO << " - NO ERROR";
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
        qDebug()<< Q_FUNC_INFO << " - Result = Host Not found! = " << QString::number(result) ;
        text = "eQSL: " + tr("Host not found!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == QNetworkReply::TimeoutError)
    {
        qDebug()<< Q_FUNC_INFO << " - Result = Time out error! = " << QString::number(result) ;
        text = "eQSL: " + tr("Timeout error!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else
    {
        qDebug()<< Q_FUNC_INFO << " - Result = UNDEFINED = " << QString::number(result) ;
        text = "eQSL: " + tr("Undefined error number (#%1)... ").arg(result);
        QMessageBox::warning(nullptr, tr("KLog - eQSL"),
                                       tr("We have received an undefined error from eQSL (%1)").arg(result) + "\n" +
                                          tr("Please check your config in the setup and contact the KLog development team if you can't fix it. eQSL uploads will be disabled."),
                                       QMessageBox::Ok);
        //TODO: Mark the previous QSO as not sent to clublog
    }

    qDebug()<< Q_FUNC_INFO << " END - Result = " << QString::number(result);
    //emit done();
    emit signalFileUploaded(result, qsos);
    emit showMessage(text);

}

void eQSLUtilities::downloadProgress(qint64 received, qint64 total) {
       //qDebug()<< "eQSLUtilities::downloadProgress: " << QString::number(received) << "/" << QString::number(total);

       //qDebug()<< received << total;
    emit actionShowProgres(received, total);
}

void eQSLUtilities::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
       //qDebug()<< "eQSLUtilities::slotErrorManagement: " << QString::number(networkError);
    result = networkError;
    /*
    if (result == QNetworkReply::NoError)
    {
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
            //qDebug()<< "eQSLUtilities::slotErrorManagement: Host not found";
    }
    else
    {
            //qDebug()<< "eQSLUtilities::slotErrorManagement: ERROR!";
    }

    */
}

void eQSLUtilities::setCredentials(const QString &_user, const QString &_pass, const QString &_defaultStationCallsign)
{
    //qDebug()<< "eQSLUtilities::setCredentials: user: " << _user << " / Pass: " << _pass << " / StationCallsign: " << _defaultStationCallsign;
    stationCallsign = _defaultStationCallsign;
    user = _user;
    pass = _pass;
}

QStringList eQSLUtilities::prepareToTranslate(const QString &_m)
{
    //qDebug()<< "eQSLUtilities:: = prepareToTranslate" << _m;
    QString msg = _m;
    QStringList result;
    result.clear();
    if (_m.contains("Error: No match on eQSL_User/eQSL_Pswd"))
    {
        result << QString("Error");
        result << QString(tr("eQSL Error: User or password incorrect"));
        pass = QString();
    }
    else if ( (_m.contains("Warning:")) && (_m.contains("Bad record: Duplicate") ) )
    {
        result << QString("Warning");
        result << QString(tr("eQSL Warning: At least one of the uplodaded QSOs is duplicated."));
    }
    else if ((_m.contains("Result:")) && (_m.contains("records added<BR>")) && (!_m.contains("Warning:")) )
    {
        result << QString("OK");
        result << QString(tr("eQSL: All the QSOs were properly uploaded."));
    }
    else
    {
        result << "Unknown" << "Unknown";
    }
    //qDebug()<< "eQSLUtilities:: = prepareToTranslate returning... " ;


    return result;
}

void eQSLUtilities::sendLogFile(const QString &_file, QList<int> _qso)
{
    qDebug()<< Q_FUNC_INFO << " " << _file;
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
    if (file->open(QIODevice::ReadOnly)) /* Flawfinder: ignore */
    {
         blob = file->readAll();
        qDebug()<< Q_FUNC_INFO << " FILE OPEN: " << blob;
    }
    else
    {
        qDebug()<< Q_FUNC_INFO << " ERROR File not opened";
        return;
    }
    file->close();
    // The rest of the form goes as usual
    qDebug()<< Q_FUNC_INFO << " e: " << user;
    qDebug()<< Q_FUNC_INFO << " pass: " << pass;
    qDebug()<< Q_FUNC_INFO << " stationcall: " << stationCallsign;

    QHttpPart userPart;
    userPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"eqsl_user\""));
    userPart.setBody(user.toUtf8());

    if (pass.length()<1)
    {
        bool ok;
        pass = QInputDialog::getText(nullptr, tr("KLog - eQSL.cc password needed"), tr("Please enter your eQSL.cc password: "), QLineEdit::Password, "", &ok);
        if (!ok)
        {
            return;
        }
    }

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
    qDebug()<< Q_FUNC_INFO << " Before sending";
    manager->post(request, multiPart);
    qDebug()<< Q_FUNC_INFO << " After sending";
    //multiPart->setParent(reply);
    qDebug()<< "eQSLUtilities::sendLogFile - END";

}


