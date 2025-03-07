#ifndef KLOG_LOTWUTILITIES_H
#define KLOG_LOTWUTILITIES_H
/***************************************************************************
                          lotwutilities.h  -  description
                             -------------------
    begin                : apr 2020
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
#include <QString>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QTimer>
#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>
#include "dataproxy_sqlite.h"
#include "qso.h"

// https://lotw.arrl.org/lotw-help/developer-query-qsos-qsls/?lang=en
class QSslError;

class LoTWUtilities : public QObject
{
    Q_OBJECT

public:
    explicit LoTWUtilities(const QString &_klogDir, const QString &_klogVersion, const QString &_parentFunction, DataProxy_SQLite *dp);
    ~LoTWUtilities();
    bool setStationCallSign(const QString &_call);
    int download();
    int fullDownload();
    void setUser(const QString &_call);
    void setPass(const QString &_pass);
    bool getIsReady();
    void setFileName(const QString &_fn);
    QString getFileName();


private:
    void startRequest(QUrl url);
    int startThefullDownload();
    bool selectQuery(const int _queryId);
    void parseDownloadedFile(const QString &_fn);
    //void showMessage(const int _messageIcon, const QString &_msg, const QString &_msgExt);

    //Utilities *util;

    //void setTarget(const QString& t);

    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    QFile *file;
    bool downloadAborted;
    //qint64 fileSize;
    QUrl url;
    QProgressDialog *pDialog;

    QString klogDir, klogVersion;
    QString urld;
    QString fileName;

    QString stationCallsign;
    QString startDate;
    QString lotwQuery;    
    QString lotwUser, lotwPassword;

    DataProxy_SQLite *dataProxy;//, *dataProxyPrepared;
    QCalendarWidget *calendar;
    QDate firstDate;

private slots:
    void slotReadyRead();
    void slotFinished();
    void slotDownloadProgress(qint64 bytesRead);
    void slotCancelDownload();
    void slotCalendarDateSelected(const QDate _d);


signals:
    void actionProcessLoTWDownloadedFile(QString _fn);
};

#endif // LOTWUTILITIES_H
