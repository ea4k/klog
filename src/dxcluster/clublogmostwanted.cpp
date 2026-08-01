/***************************************************************************
                          clublogmostwanted.cpp  -  description
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
#include "clublogmostwanted.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

ClubLogMostWanted::ClubLogMostWanted(QObject *parent) : QObject(parent)
{
    url         = "https://clublog.org/mostwanted.php";
    klogVersion = QString();
    manager     = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &ClubLogMostWanted::slotDownloadFinished);

    dailyTimer = new QTimer(this);
    dailyTimer->setInterval(24 * 60 * 60 * 1000);
    connect(dailyTimer, &QTimer::timeout, this, &ClubLogMostWanted::slotDailyCheck);
    dailyTimer->start();
}

ClubLogMostWanted::~ClubLogMostWanted()
{
}

void ClubLogMostWanted::setUrl(const QString &_url)
{
    if (!_url.isEmpty())
        url = _url;
}

void ClubLogMostWanted::setKLogVersion(const QString &_version)
{
    klogVersion = _version;
}

void ClubLogMostWanted::setApiKey(const QString &_apiKey)
{
    apiKey = _apiKey;
}

QUrl ClubLogMostWanted::buildFetchUrl() const
{
    QUrl fetchUrl(url);
    if (!apiKey.isEmpty())
    {
        QUrlQuery query(fetchUrl);
        query.addQueryItem("api", apiKey);
        fetchUrl.setQuery(query);
    }
    return fetchUrl;
}

void ClubLogMostWanted::setPrefixResolver(std::function<int(const QString &)> _resolver)
{
    prefixResolver = _resolver;
}

void ClubLogMostWanted::setCacheFile(const QString &_filePath)
{
    cacheFile = _filePath;
    if (!cacheFile.isEmpty())
        loadFromCache();
}

void ClubLogMostWanted::fetch()
{
    QNetworkRequest request{buildFetchUrl()};
    request.setRawHeader("User-Agent", QString("KLog-%1").arg(klogVersion).toUtf8());
    manager->get(request);
}

void ClubLogMostWanted::fetchIfStale()
{
    if (isStale())
        fetch();
}

bool ClubLogMostWanted::isStale() const
{
    if (!lastUpdate.isValid())
        return true;
    return lastUpdate.daysTo(QDateTime::currentDateTimeUtc()) >= MOST_WANTED_TTL_DAYS;
}

int ClubLogMostWanted::getRank(int dxcc) const
{
    return ranks.value(dxcc, 0);
}

bool ClubLogMostWanted::isInThreshold(int dxcc) const
{
    int rank = getRank(dxcc);
    return (rank > 0) && (rank <= MOST_WANTED_THRESHOLD);
}

bool ClubLogMostWanted::hasData() const
{
    return !ranks.isEmpty();
}

QDateTime ClubLogMostWanted::getLastUpdate() const
{
    return lastUpdate;
}

QString ClubLogMostWanted::getLastError() const
{
    return lastError;
}

void ClubLogMostWanted::slotDownloadFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError)
    {   // Keep whatever data we already have; getRank keeps answering (0 if none)
        lastError = QString("Network error fetching the Most Wanted list: %1").arg(reply->errorString());
        qWarning() << "ClubLogMostWanted:" << lastError;
        emit fetchFailed(lastError);
        return;
    }

    QByteArray data = reply->readAll();
    if (!parse(data))
    {
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastError = QString("Could not parse the Most Wanted response (HTTP %1): %2 [%3...]")
                        .arg(status).arg(lastError,
                                         QString::fromUtf8(data.left(80)).simplified());
        qWarning() << "ClubLogMostWanted:" << lastError;
        emit fetchFailed(lastError);
        return;
    }

    lastError.clear();
    lastUpdate = QDateTime::currentDateTimeUtc();
    if (!cacheFile.isEmpty())
        saveToCache(data);
    emit mostWantedUpdated();
}

void ClubLogMostWanted::slotDailyCheck()
{
    fetchIfStale();
}

bool ClubLogMostWanted::parse(const QByteArray &data)
{
    // ClubLog publishes the Most Wanted list as JSON. Two shapes are accepted:
    //  - an object mapping rank to prefix: {"1":"P5","2":"3Y/B", ...}
    //    (prefixes are mapped to DXCC ids through the injected resolver)
    //  - an array of objects carrying the entity id: [{"dxcc":344,"rank":1}, ...]
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        lastError = QString("not valid JSON (%1)").arg(parseError.errorString());
        return false;
    }

    QHash<int, int> newRanks;
    int entriesSeen = 0;
    bool resolverMissing = false;

    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        for (auto it = obj.constBegin(); it != obj.constEnd(); ++it)
        {
            bool rankOk = false;
            int rank = it.key().toInt(&rankOk);
            if (!rankOk || rank <= 0)
                continue;

            entriesSeen++;
            int dxcc = -1;
            if (it.value().isString())
            {
                if (!prefixResolver)
                {
                    resolverMissing = true;
                    continue;
                }
                dxcc = prefixResolver(it.value().toString());
            }
            else if (it.value().isObject())
            {
                QJsonObject entity = it.value().toObject();
                if (entity.contains("dxcc"))
                    dxcc = entity.value("dxcc").toInt(-1);
                else if (entity.contains("prefix") && prefixResolver)
                    dxcc = prefixResolver(entity.value("prefix").toString());
            }

            if (dxcc > 0 && (!newRanks.contains(dxcc) || rank < newRanks.value(dxcc)))
                newRanks.insert(dxcc, rank);
        }
    }
    else if (doc.isArray())
    {
        const QJsonArray array = doc.array();
        for (const QJsonValue &value : array)
        {
            if (!value.isObject())
                continue;
            QJsonObject entity = value.toObject();
            entriesSeen++;
            int dxcc = entity.value("dxcc").toInt(-1);
            int rank = entity.value("rank").toInt(-1);
            if (dxcc > 0 && rank > 0 &&
                (!newRanks.contains(dxcc) || rank < newRanks.value(dxcc)))
                newRanks.insert(dxcc, rank);
        }
    }

    if (newRanks.isEmpty())
    {
        if (resolverMissing)
            lastError = QString("no prefix resolver set to map the %1 prefixes to DXCC ids").arg(entriesSeen);
        else if (entriesSeen > 0)
            lastError = QString("none of the %1 entries resolved to a DXCC id").arg(entriesSeen);
        else
            lastError = "the response contained no usable entries";
        return false;
    }

    ranks = newRanks;
    return true;
}

void ClubLogMostWanted::saveToCache(const QByteArray &data)
{
    Q_UNUSED(data)
    // The parsed ranks are saved (not the raw payload) so reloading the
    // cache never depends on the prefix resolver being set first.
    QJsonObject rankObj;
    for (auto it = ranks.constBegin(); it != ranks.constEnd(); ++it)
        rankObj.insert(QString::number(it.key()), it.value());

    QJsonObject root;
    root.insert("lastUpdate", QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    root.insert("ranks", rankObj);

    QDir().mkpath(QFileInfo(cacheFile).absolutePath());
    QFile file(cacheFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    file.write(QJsonDocument(root).toJson(QJsonDocument::Compact));
    file.close();
}

bool ClubLogMostWanted::loadFromCache()
{
    QFile file(cacheFile);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject())
        return false;

    QJsonObject root = doc.object();
    QDateTime cachedUpdate = QDateTime::fromString(root.value("lastUpdate").toString(), Qt::ISODate);
    QJsonObject rankObj = root.value("ranks").toObject();

    QHash<int, int> newRanks;
    for (auto it = rankObj.constBegin(); it != rankObj.constEnd(); ++it)
    {
        bool dxccOk = false;
        int dxcc = it.key().toInt(&dxccOk);
        int rank = it.value().toInt(-1);
        if (dxccOk && dxcc > 0 && rank > 0)
            newRanks.insert(dxcc, rank);
    }
    if (newRanks.isEmpty())
        return false;

    ranks = newRanks;
    lastUpdate = cachedUpdate.isValid() ? cachedUpdate.toUTC() : QDateTime();
    emit mostWantedUpdated();
    return true;
}
