#ifndef KLOG_CLUSTER_CLUBLOGMOSTWANTED_H
#define KLOG_CLUSTER_CLUBLOGMOSTWANTED_H
/***************************************************************************
                          clublogmostwanted.h  -  description
                             -------------------
    begin                : Aug 2026
    copyright            : (C) 2026 by Jaime Robles
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
/*
    This class fetches, parses and caches the ClubLog global Most Wanted
    list. The DX Assistant scoring engine uses it as a secondary tiebreaker.
*/
#include <QObject>
#include <QHash>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QUrl>
#include <functional>

class ClubLogMostWanted : public QObject
{
    Q_OBJECT
    friend class tst_ClubLogMostWanted;
    friend class tst_DXAssistantEngine;

public:
    // Only top-N entities participate in tiebreaking
    static constexpr int MOST_WANTED_THRESHOLD = 50;
    // Re-fetch interval: once a month. Future: loaded from QSettings
    static constexpr int MOST_WANTED_TTL_DAYS  = 30;

    explicit ClubLogMostWanted(QObject *parent = nullptr);
    ~ClubLogMostWanted();

    void fetch();                          // Trigger a network fetch
    int  getRank(int dxcc) const;          // Returns rank (1 = most wanted), 0 if not in list
    bool isInThreshold(int dxcc) const;    // getRank > 0 && getRank <= MOST_WANTED_THRESHOLD

    bool hasData() const;                  // True once a list has been parsed (network or cache)
    QDateTime getLastUpdate() const;       // UTC time of the data currently held; invalid if none

    // The fetch URL is injectable so a future personalized (authenticated)
    // list only requires changing the URL — no structural changes.
    void setUrl(const QString &_url);
    void setKLogVersion(const QString &_version);   // Used for the User-Agent header

    // ClubLog only serves the JSON list when an application API key is
    // passed (the bare URL returns an HTML page, which parse() rejects).
    void setApiKey(const QString &_apiKey);

    // ClubLog publishes the list as rank -> prefix; the resolver maps a
    // prefix to a DXCC entity id (injected to keep this class decoupled
    // from World/DataProxy and hermetically testable).
    void setPrefixResolver(std::function<int(const QString &)> _resolver);

    // Optional on-disk cache. When set, parsed data is saved after each
    // successful fetch and loaded back on start, so KLog does not hit the
    // network more than once per MOST_WANTED_TTL_DAYS across restarts.
    void setCacheFile(const QString &_filePath);

    // Fetch only if the data currently held is missing or older than the TTL.
    void fetchIfStale();

    QString getLastError() const;          // Why the last fetch/parse failed; empty if none

signals:
    void mostWantedUpdated();              // Emitted after a successful parse
    void fetchFailed(const QString &reason);   // Network or parse failure (old data kept)

private slots:
    void slotDownloadFinished(QNetworkReply *reply);
    void slotDailyCheck();

private:
    bool parse(const QByteArray &data);    // Fills ranks; false if nothing usable
    QUrl buildFetchUrl() const;            // url + api key query parameter
    bool loadFromCache();
    void saveToCache(const QByteArray &data);
    bool isStale() const;

    QHash<int, int> ranks;                 // DXCC entity id -> rank (1 = most wanted)
    QDateTime lastUpdate;                  // UTC; when the held data was fetched
    QString url;
    QString apiKey;
    QString klogVersion;
    QString cacheFile;
    QString lastError;
    std::function<int(const QString &)> prefixResolver;
    QNetworkAccessManager *manager;
    QTimer *dailyTimer;                    // A 30-day interval overflows QTimer's int
                                           // msec range, so staleness is checked daily
};

#endif // KLOG_CLUSTER_CLUBLOGMOSTWANTED_H
