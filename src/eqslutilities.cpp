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
    //qDebug()<< "eQSLUtilities::eQSLUtilities"  << QT_ENDL;
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
    util = new Utilities;
    //qDebug()<< "eQSLUtilities::eQSLUtilities - END"  << QT_ENDL;
}

eQSLUtilities::~eQSLUtilities()
{
    delete(util);
    //qDebug()<< "eQSLUtilities::~eQSLUtilities"  << QT_ENDL;
}

void eQSLUtilities::setUser(const QString &_call)
{
     //qDebug() << "eQSLUtilities::setUser: " << _call << QT_ENDL;
    user = _call;
     //qDebug() << "eQSLUtilities::setUser: END" << QT_ENDL;
}

void eQSLUtilities::setPass(const QString &_pass)
{
     //qDebug() << "eQSLUtilities::setPass: " << _pass << QT_ENDL;
    pass = _pass;
     //qDebug() << "eQSLUtilities::setPass: END" << QT_ENDL;
}

void eQSLUtilities::slotQsoUploadFinished(QNetworkReply *data)
{
    //qDebug()<< "eQSLUtilities::slotQsoUploadFinished"  << QT_ENDL;
    QStringList parsedAnswer;
    parsedAnswer.clear();
    result = data->error();
    //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = " << QString::number(result) << QT_ENDL;

    const QByteArray sdata = data->readAll();
    QString text = QString();

    if (result == QNetworkReply::NoError)
    {

        parsedAnswer << prepareToTranslate(sdata);
        if (parsedAnswer.at(0).contains("Error"))
        {
            //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - error detected" << QT_ENDL;
            QMessageBox::warning(nullptr, tr("KLog - eQSL"), tr("eQSL has sent the following message:\n%1").arg(parsedAnswer.at(1)), QMessageBox::Ok);
            qsos.clear();
            return;
        }

        //qDebug()<< sdata;
        //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - NO ERROR" << QT_ENDL;
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
        //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = Host Not found! = " << QString::number(result)  << QT_ENDL;
        text = "eQSL: " + tr("Host not found!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else if (result == QNetworkReply::TimeoutError)
    {
        //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = Time out error! = " << QString::number(result)  << QT_ENDL;
        text = "eQSL: " + tr("Timeout error!");
        //TODO: Mark the previous QSO as not sent to clublog
    }
    else
    {
        //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = UNDEFINED = " << QString::number(result)  << QT_ENDL;
        text = "eQSL: " + tr("Undefined error number (#%1)... ").arg(result);
        QMessageBox::warning(nullptr, tr("KLog - eQSL"),
                                       tr("We have received an undefined error from eQSL (%1)").arg(result) + "\n" +
                                          tr("Please check your config in the setup and contact the KLog development team if you can't fix it. eQSL uploads will be disabled."),
                                       QMessageBox::Ok);
        //TODO: Mark the previous QSO as not sent to clublog
    }

    //qDebug()<< "eQSLUtilities::slotQsoUploadFinished - Result = " << QString::number(result) << QT_ENDL;
    //emit done();
    emit signalFileUploaded(result, qsos);
    emit showMessage(text);

}

void eQSLUtilities::downloadProgress(qint64 received, qint64 total) {
       //qDebug()<< "eQSLUtilities::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << QT_ENDL;

       //qDebug()<< received << total;
    emit actionShowProgres(received, total);
}

void eQSLUtilities::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
       //qDebug()<< "eQSLUtilities::slotErrorManagement: " << QString::number(networkError) << QT_ENDL;
    result = networkError;
    /*
    if (result == QNetworkReply::NoError)
    {
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
            //qDebug()<< "eQSLUtilities::slotErrorManagement: Host not found" << QT_ENDL;
    }
    else
    {
            //qDebug()<< "eQSLUtilities::slotErrorManagement: ERROR!" << QT_ENDL;
    }

    */
}

void eQSLUtilities::setCredentials(const QString &_user, const QString &_pass, const QString &_defaultStationCallsign)
{
    //qDebug()<< "eQSLUtilities::setCredentials: user: " << _user << " / Pass: " << _pass << " / StationCallsign: " << _defaultStationCallsign << QT_ENDL;
    stationCallsign = _defaultStationCallsign;
    user = _user;
    pass = _pass;
}

QStringList eQSLUtilities::prepareToTranslate(const QString &_m)
{
    //qDebug()<< "eQSLUtilities:: = prepareToTranslate" << _m << QT_ENDL;
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
    //qDebug()<< "eQSLUtilities:: = prepareToTranslate returning... "  << QT_ENDL;


    return result;
}

void eQSLUtilities::sendLogFile(const QString &_file, QList<int> _qso)
{
     //qDebug()<< "eQSLUtilities::sendLogFile: " << _file << QT_ENDL;
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
        //qDebug()<< "eQSLUtilities::sendLogFile: FILE OPEN: " << blob << QT_ENDL;
    }
    else
    {
         //qDebug()<< "eQSLUtilities::sendLogFile: ERROR File not opened" << QT_ENDL;
        return;
    }
    file->close();
    // The rest of the form goes as usual
     //qDebug()<< "eQSLUtilities::sendLogFile: e: " << user << QT_ENDL;
     //qDebug()<< "eQSLUtilities::sendLogFile: pass: " << pass << QT_ENDL;
     //qDebug()<< "eQSLUtilities::sendLogFile: stationcall: " << stationCallsign << QT_ENDL;

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
    //qDebug()<< "eQSLUtilities::sendLogFile: Before sending" << QT_ENDL;
    manager->post(request, multiPart);
    //qDebug()<< "eQSLUtilities::sendLogFile: After sending" << QT_ENDL;
    //multiPart->setParent(reply);
    //qDebug()<< "eQSLUtilities::sendLogFile - END" << QT_ENDL;

}


