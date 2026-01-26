/***************************************************************************
    spotrecommender.cpp  -  description
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
#include "spotrecommender.h"
#include <algorithm>

SpotRecommender::SpotRecommender(DataProxy_SQLite *dp,
                                 ClubLogMostWanted *cl,
                                 World *w,
                                 QObject *parent)
    : QObject(parent)
    , dataProxy(dp)
    , clubLog(cl)
    , world(w) // Initialize World
{
    cleanupTimer = new QTimer(this);
    connect(cleanupTimer, &QTimer::timeout, this, &SpotRecommender::slotCleanupOldSpots);
    cleanupTimer->start(60000);
}

void SpotRecommender::processSpot(const DXSpot &spot)
{
    // Note: ensure DXSpot::isValid() is const as discussed previously
    if (!spot.isValid())
        return;

    activeSpots.append(spot);
    if (activeSpots.size() > 500)
        activeSpots.removeFirst();

    emit recommendationsUpdated();
}

void SpotRecommender::slotCleanupOldSpots()
{
    QDateTime cutoff = QDateTime::currentDateTimeUtc().addSecs(-1800);
    activeSpots.removeIf([cutoff](const DXSpot &s) { return s.getDateTime() < cutoff; });
    emit recommendationsUpdated();
}

int SpotRecommender::calculateScore(const DXSpot &spot)
{
    int score = 0;
    QString call = spot.getDxCall();
    double freq = spot.getFrequency().toDouble();
    int bandId = dataProxy->getBandIdFromFreq(freq);

    // 1. Get DXCC using World class
    // getQRZARRLId returns the int ID (e.g., 291 for USA) from a callsign string
    int dxcc = world->getQRZARRLId(call);

    if (dxcc > 0) {
        // 2. ATNO (All Time New One) Check
        // Check if this DXCC has been worked on ANY band
        Creo que el código del recomemder debería estar en
            dxcluster.Es esa clase la que recibe los DXSpots.Podría tener sentido sacarla si
                llegaran datos también de wsjtx,
            por ejemplo

            if (!dataProxy->isDXCCWorked(dxcc))
        {
            score += 10000;
        }
        // 3. Band New One
        // Check if this DXCC has been worked on THIS band
        else if (!dataProxy->isDXCCWorked(dxcc, bandId))
        {
            score += 5000;
        }

        // 4. Most Wanted Points
        // Get the Main Prefix (e.g. "P5") to query ClubLog list
        QString prefix = world->getEntityMainPrefix(dxcc); // Using World to get prefix from ID
        int rank = clubLog->getRank(prefix);

        if (rank > 0 && rank < 350) {
            score += (350 - rank) * 10;
        }
    }
    return score;
}

QList<RecommendedSpot> SpotRecommender::getTopSpots(int limit)
{
    QList<RecommendedSpot> result;

    for (const auto &spot : activeSpots) {
        RecommendedSpot rec;
        rec.spot = spot;
        rec.score = calculateScore(spot);

        QString bandName = dataProxy->getNameFromBandId(
            dataProxy->getBandIdFromFreq(spot.getFrequency().toDouble()));
        rec.buttonText = QString("%1 (%2)").arg(spot.getDxCall()).arg(bandName);

        result.append(rec);
    }

    std::sort(result.begin(), result.end(), [](const RecommendedSpot &a, const RecommendedSpot &b) {
        return a.score > b.score;
    });

    if (result.size() > limit)
        result = result.mid(0, limit);
    return result;
}
