#ifndef ELOGQRZLOG_H
#define ELOGQRZLOG_H
/***************************************************************************
                          elogqrzlog.h  -  description
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
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
#include "dataproxy_sqlite.h"

class eLogQrzLog : public QObject {
    Q_OBJECT
public:
    explicit eLogQrzLog(DataProxy_SQLite *dp, const QString &_parentFunction, const QString &_klogVersion);
    ~eLogQrzLog();
    void login();
    void setCredentials(const QString &_user, const QString &_pass);
    void setLogBookKey(const QString &_key);
    int sendQSO(const int _qsoID);
    void checkQRZ(const QString &_qrz);
    void fetchData();
    //int deleteQSOid(const int _qsoId);
    //int modifyQSO (QStringList _oldQSO, QStringList _newQSO);

    //void sendLogFile(const QString &_file, QList<int> _qso, bool _overwrite);


private:
    //QString getClubLogAdif(const QStringList _q);
    //int sendData(const QString &_clublogCall, const QString &_q); //  Sends the data (http post) to ClubLog
    int sendDataParams(const QUrlQuery &_params);
    QString prepareToTranslate(const QString &_m);       //  Get the message and put it in a tr to be able to translate it
    bool canConnect();

    QString sessionkey, logbookkey;
    QString user, pass;
    QString klogVersion;
    DataProxy_SQLite *dataProxy;
    QNetworkAccessManager *manager;
    QNetworkReply* reply;
    int currentQSO;
    QNetworkReply::NetworkError result;
    QString target;
    bool uploadingFile;
    QList<int> qsos;
    Utilities *util;
    bool logged;
    QUrl serviceUrl;
    QXmlStreamReader *reader;
    //bool useQSOStationCallsign;

private slots:
    void slotManagerFinished(QNetworkReply* data);
    //void slotFileUploadFinished(QNetworkReply* data);
    void downloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);


signals:
    void actionReturnDownload(const int _i, const int _qsoId);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);
    void showMessage(const QString _t);
    void disableClubLogAction(const bool _b);
    void signalFileUploaded(QNetworkReply::NetworkError, QList<int>);
    void dataFoundSignal(const QString &_type, const QString _data);

};
#endif // DOWNLOADCTY_H
