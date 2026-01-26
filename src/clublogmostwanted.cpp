/***************************************************************************
    clublogmostwanted.cpp  -  description
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

#include "clublogmostwanted.h"

ClubLogMostWanted::ClubLogMostWanted(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    m_isLoaded = false;
    connect(manager,
            &QNetworkAccessManager::finished,
            this,
            &ClubLogMostWanted::slotDownloadFinished);
}

void ClubLogMostWanted::downloadList()
{
    // Prevent repeated API calls if we already have the data
    if (m_isLoaded) {
        qDebug() << "ClubLogMostWanted: Data already loaded. Skipping API call.";
        emit listUpdated(); // Signal that data is ready to use
        return;
    }

    qDebug() << "ClubLogMostWanted: Downloading Most Wanted list from ClubLog API...";
    QUrl url("https://clublog.org/mostwanted.php?api=1");
    QNetworkRequest request(url);
    // Optional: Set User-Agent so ClubLog knows it's KLog
    request.setHeader(QNetworkRequest::UserAgentHeader, "KLog");
    manager->get(request);
}

void ClubLogMostWanted::slotDownloadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        prefixToRank.clear();
        QByteArray data = reply->readAll();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        for (auto it = obj.begin(); it != obj.end(); ++it) {
            int rank = it.key().toInt();
            QString prefix = it.value().toString();
            prefixToRank.insert(prefix, rank);
        }

        if (!prefixToRank.isEmpty()) {
            m_isLoaded = true; // Mark as successfully loaded
            qDebug() << "ClubLogMostWanted: Successfully loaded" << prefixToRank.size()
                     << "entities.";
            emit listUpdated();
        }
    } else {
        qDebug() << "ClubLogMostWanted: Error downloading list" << reply->errorString();
        // We do NOT set m_isLoaded to true, so the user can try again later if it failed.
    }
    reply->deleteLater();
}

int ClubLogMostWanted::getRank(const QString &prefix)
{
    // Return a default low priority (e.g., 999) if not found
    if (!m_isLoaded)
        return 999;
    return prefixToRank.value(prefix, 999);
}
