#ifndef DOWNLOADCTY_H
#define DOWNLOADCTY_H
/***************************************************************************
                          downloadcty.h  -  description
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
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
#include "utilities.h"

class QSslError;

class DownLoadCTY : public QObject {
    Q_OBJECT
public:
    explicit DownLoadCTY(const QString _kontestDir, const QString _klogVersion);
    ~DownLoadCTY();
    int download();

private:
    Utilities *util;

    //void setTarget(const QString& t);
    bool saveToDisk(const QString &filename, QIODevice *data);
    QString saveFileName(const QUrl &url);

    QNetworkAccessManager *manager;
    QNetworkRequest *request;


    //QString target;
    int result; // enum QNetworkReply::NetworkError
    QString kontestDir;
    QString urld;
    QUrl *url;

private slots:
    void slotDownloadFinished(QNetworkReply* reply);
    void slotDownloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);



signals:
    void actionReturnDownload(const int _i);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);


};
#endif // DOWNLOADCTY_H
