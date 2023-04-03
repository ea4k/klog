#ifndef ELOGCLUBLOG_H
#define ELOGCLUBLOG_H
/***************************************************************************
                          elogclublog.h  -  description
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

class eLogClubLog : public QObject {
    Q_OBJECT
public:
    explicit eLogClubLog();
    ~eLogClubLog();

    void setDefaultCallsign(const QString &_defaultStationCallsign);
    int sendQSO(QStringList _qso);

    int deleteQSO(QStringList _qso);
    //int deleteQSOid(const int _qsoId);
    int modifyQSO (QStringList _oldQSO, QStringList _newQSO);

    void sendLogFile(const QString &_file, QList<int> _qso, bool _overwrite);
    void loadSettings();

private:
    QString getClubLogAdif(const QStringList _q);
    //int sendData(const QString &_clublogCall, const QString &_q); //  Sends the data (http post) to ClubLog
    int sendDataParams(const QString &_clublogCall, const QUrlQuery &_params, bool _adding);
    QString prepareToTranslate(const QString &_m);       //  Get the message and put it in a tr to be able to translate it


    QString email, pass, api, stationCallsign;

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
    void slotFileUploadFinished(QNetworkReply* data);
    void downloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);


signals:
    void actionReturnDownload(const int _i, const int _qsoId);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);
    void showMessage(const QString &_t);
    void disableClubLogAction(const bool _b);
    void signalFileUploaded(QNetworkReply::NetworkError, QList<int>);

};
#endif // DOWNLOADCTY_H
