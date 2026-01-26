#ifndef SPOTRECOMMENDER_H
#define SPOTRECOMMENDER_H
/***************************************************************************
    spotrecommender.h  -  description
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

#include <QList>
#include <QObject>
#include <QTimer>
#include "clublogmostwanted.h"
#include "dataproxy_sqlite.h"
#include "dxcluster/dxspot.h"
#include "world.h" // Include World header

struct RecommendedSpot
{
    QString buttonText;
    DXSpot spot;
    int score;
};

class SpotRecommender : public QObject
{
    Q_OBJECT
public:
    // Update constructor to receive World pointer
    explicit SpotRecommender(DataProxy_SQLite *dp,
                             ClubLogMostWanted *cl,
                             World *w,
                             QObject *parent = nullptr);

    void processSpot(const DXSpot &spot);
    QList<RecommendedSpot> getTopSpots(int limit = 5);

signals:
    void recommendationsUpdated();

private slots:
    void slotCleanupOldSpots();

private:
    DataProxy_SQLite *dataProxy;
    ClubLogMostWanted *clubLog;
    World *world; // Store the pointer to World
    QTimer *cleanupTimer;
    QList<DXSpot> activeSpots;

    int calculateScore(const DXSpot &spot);
};

#endif // SPOTRECOMMENDER_H
