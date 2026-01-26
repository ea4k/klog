#ifndef CLUBLOGMOSTWANTED_H
#define CLUBLOGMOSTWANTED_H
/***************************************************************************
    clublogmostwanted.h  -  description
       -------------------
begin                : jan 2026
copyright            : (C) 2026 by Jaime Robles
email                : jaime@robles.es
***************************************************************************/

/****************************************************************************
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

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class ClubLogMostWanted : public QObject
{
    Q_OBJECT
public:
    explicit ClubLogMostWanted(QObject *parent = nullptr);

    // Triggers the download only if data hasn't been loaded yet
    void downloadList();

    // Returns the rank (1 = most wanted). Returns -1 or 999 if not found.
    int getRank(const QString &prefix);

    // Helper to check if data is ready
    bool isDataLoaded() const { return m_isLoaded; }

signals:
    void listUpdated();

private slots:
    void slotDownloadFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QMap<QString, int> prefixToRank; // Map Prefix -> Rank
    bool m_isLoaded;                 // Flag to ensure we only call the API once per session
};

#endif // CLUBLOGMOSTWANTED_H
