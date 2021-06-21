#ifndef EQSLUTILITIES_H
#define EQSLUTILITIES_H
/***************************************************************************
                          eqslutilities.h  -  description
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
#include <QObject>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QProgressDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QHttpPart>
#include "utilities.h"
#include "securelogin.h"

// https://www.eqsl.cc/qslcard/Programming.cfm
class eQSLUtilities : public SecureLogin {
    Q_OBJECT
public:
    explicit eQSLUtilities(const QString &_parentFunction);
    ~eQSLUtilities();
    //int sendQSO(QStringList _qso);
    //int deleteQSO(QStringList _qso);
    //int modifyQSO (QStringList _oldQSO, QStringList _newQSO);

    void sendLogFile(const QString &_file, QList<int> _qso);


private:
    //QString geteQSLAdif(const QStringList _q);
    //int sendData(const QString &_eQSLUser, const QString &_q); //  Sends the data (http post) to eQSL
    //int sendDataParams(const QString &_eQSLUser, const QUrlQuery &_params);
    QStringList prepareToTranslate(const QString &_m);       //  Get the message and put it in a tr to be able to translate it


    QNetworkAccessManager *manager;
    QNetworkReply* reply;
    int currentQSO;
    QNetworkReply::NetworkError result;
    QString target;
    bool uploadingFile;
    QList<int> qsos;
    Utilities *util;
    //bool useQSOStationCallsign;

private slots:
    void slotQsoUploadFinished(QNetworkReply* data);
    //void slotFileUploadFinished(QNetworkReply* data);
    void downloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);


signals:
    void actionReturnDownload(const int _i, const int _qsoId);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);
    void showMessage(const QString _t);
    void disableeQSLAction(const bool _b);
    void signalFileUploaded(QNetworkReply::NetworkError, QList<int>);

};
#endif // EQSLUTILITIES_H
