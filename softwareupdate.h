#ifndef SOFTWAREUPDATE_H
#define SOFTWAREUPDATE_H
/***************************************************************************
                          softwareupdate.h  -  description
                             -------------------
    begin                : feb 2017
    copyright            : (C) 2017 by Jaime Robles
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
#include <QCoreApplication>
#include <QMessageBox>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
//#include <QFile>
//#include <QDirIterator>
#include <QList>
#include <QRegularExpression>
//#include <QDebug>
#include "utilities.h"
#include "softwareupdatedialog.h"


class QSslError;

class SoftwareUpdate: public QObject {
    Q_OBJECT

public:
    SoftwareUpdate(const QString _klogVersion);
    ~SoftwareUpdate();

    void addCall(const QString _call);

    void needToUpdate();

private slots:
    void slotReadyRead();
    void slotError(int _p);
    //void slotSslErrors(QList _p);
    //void replyFinished(QNetworkReply *data);

    void slotDownloadFinished(QNetworkReply *reply);

signals:
    void updateNeededSignal(const bool _q); // Will be TRUE if updated if needed and FALSE if we already have the latest version

private:
    bool checkUpdates(QIODevice *data);
    void updateNeeded(QString _newVer);
    void setTheURL(QString _url);
    void setHeader();

    Utilities *util;

    QString klogVersion, latestVersion, callsign;
    QString urld;
    QUrl *url;
    
    QNetworkAccessManager *manager;
    QNetworkRequest request;

    SoftwareUpdateDialog *updateDialog;

    int result;
    bool toUpdate;

};

#endif // SOFTWAREUPDATE_H



