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
#include "dataproxy_sqlite.h"
#include "widgets/onlinemessagewidget.h"

// https://www.qrz.com/XML/current_spec.html
// https://www.qrz.com/page/xml_data.html
// https://www.qrz.com/docs/logbook/QRZLogbookAPI.html

class eLogQrzLog : public QObject {
    Q_OBJECT
public:
    explicit eLogQrzLog(DataProxy_SQLite *dp, const QString &_parentFunction, const QString &_klogVersion);
    ~eLogQrzLog();
    void login();
    void setCredentials(const QString &_user, const QString &_pass);
    void setLogBookKey(const QString &_key);
    bool hasLogBookKey();
    int sendQSOs(QList<int> _qsos);
    void checkQRZ(const QString &_qrz);
    void fetchData();
    void setSubcription(const bool _ok);
    bool getSubscription();
    //int deleteQSOid(const int _qsoId);
    //int modifyQSO (QStringList _oldQSO, QStringList _newQSO);

    //void sendLogFile(const QString &_file, QList<int> _qso, bool _overwrite);

private slots:
    void slotManagerFinished(QNetworkReply* data);
    void slotManagerLogFinished(QNetworkReply* data);
    //void slotFileUploadFinished(QNetworkReply* data);
    void downloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);


signals:
    void actionReturnDownload(const int _i, const int _qsoId);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);
    void showMessage(const QString &_t);
    void disableQRZAction(const bool _b);
    void signalLogUploaded(QNetworkReply::NetworkError, QList<int>);
    void dataFoundSignal(const QString &_type, const QString &_data);

private:
    void showDebugLog(const QString &_func, const QString &_log);
    //QString getClubLogAdif(const QStringList _q);
    //int sendData(const QString &_clublogCall, const QString &_q); //  Sends the data (http post) to ClubLog
    int sendDataParams(const QUrlQuery &_params);
    QString prepareToTranslate(const QString &_m);       //  Get the message and put it in a tr to be able to translate it
    bool canConnect();
    //void parseAppAnswer (const int howManyQSOs, const QString &_m);
    void parseAppAnswer (const QString &_m);
    void parseNetworkError(QNetworkReply::NetworkError _error);
    void parseXMLAnswer(QXmlStreamReader &xml);
    int sendQSO(const int _qsoID);
    void sendSignal(QNetworkReply::NetworkError _error, QList<int> _qsos);
    bool errorWhileSendingLog;
    bool sendingQSO;
    bool lastQSO;
    bool subscriptionOK;
    QString sessionkey, logbookkey;
    QString user, pass;
    QString klogVersion;
    DataProxy_SQLite *dataProxy;
    QNetworkAccessManager *manager;
    QNetworkAccessManager *managerLog;
    int currentQSO;
    QNetworkReply::NetworkError result;
    QString target;
    bool uploadingFile;
    QList<int> qsos;
    Utilities *util;
    bool logged;
    QUrl serviceUrl;

    QXmlStreamReader *reader;

    OnlineMessageWidget *onlineMessage;
    //bool useQSOStationCallsign;

};
#endif // DOWNLOADCTY_H
