#ifndef KLOG_CLUSTER_DXASSISTANTENGINE_H
#define KLOG_CLUSTER_DXASSISTANTENGINE_H
/***************************************************************************
                          dxassistantengine.h  -  description
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
    The DX Assistant scoring engine: a pure-logic class (no UI) that takes a
    raw DXSpot from the cluster, populates the DX Assistant fields and returns
    it scored and ready for the widget. Stored spots can be re-scored after a
    new QSO is logged.

    Future scoring dimensions (CQ zone, ITU zone, subdivisions, POTA, SOTA)
    are added as new private score methods summed in rescore() — the public
    interface does not change.
*/
#include <QObject>
#include "dxspot.h"
#include "../klogdefinitions.h"

class Awards;
class World;
class DataProxy_SQLite;
class ClubLogMostWanted;

class DXAssistantEngine : public QObject
{
    Q_OBJECT
    friend class tst_DXAssistantEngine;

public:
    // DXCC status scores
    static constexpr int SCORE_ATNO                    = 1000;  // Never worked this DXCC
    static constexpr int SCORE_NOT_WORKED_BAND         = 800;   // Worked DXCC but not on this band
    static constexpr int SCORE_NOT_WORKED_BAND_MODE    = 600;   // Worked on this band but not this mode
    static constexpr int SCORE_WORKED_UNCONFIRMED_BAND = 400;   // Worked on this band, unconfirmed
    static constexpr int SCORE_WORKED_UNCONFIRMED_MODE = 200;   // Band confirmed, mode not confirmed
    // Confirmed in band+mode → spot is discarded (score left at -1)

    // Continent modifier (spotter continent vs. user continent)
    static constexpr int SCORE_SAME_CONTINENT_BONUS    = 100;
    static constexpr int SCORE_DIFF_CONTINENT_PENALTY  = -50;

    // Mode awareness default. Future: loaded from QSettings
    static constexpr bool DX_ASSISTANT_CHECK_MODE      = true;

    explicit DXAssistantEngine(Awards *awards, World *world,
                               DataProxy_SQLite *dataProxy,
                               ClubLogMostWanted *mostWanted,
                               const QString &userContinent,
                               QObject *parent = nullptr);

    // Score a fresh spot from the DXCluster.
    // Populates the DX Assistant fields on the spot in-place.
    // Returns false if the spot should be discarded
    // (e.g. already confirmed in band+mode, or unresolvable DXCC).
    bool score(DXSpot &spot) const;

    // Re-score an already-stored spot after Awards data has changed
    // (dxcc, bandId, spotterContinent are already resolved — no need to
    // re-parse callsign or frequency).
    // Returns false if the spot should now be discarded (e.g. just confirmed).
    bool rescore(DXSpot &spot) const;

    void setUserContinent(const QString &_continent);
    QString getUserContinent() const;

    void setCheckMode(bool _check);   // Runtime switch behind DX_ASSISTANT_CHECK_MODE
    bool getCheckMode() const;

private:
    // Each scoring dimension is a separate private method; rescore() sums them.
    // Returns the base score, or -1 to discard (confirmed in band+mode).
    int scoreDXCC(int dxcc, int bandId, int modeId,
                  QSOStatus &outStatusBand, QSOStatus &outStatusBandMode) const;
    int scoreContinentModifier(const QString &spotterContinent) const;
    // Future: int scoreCQZone(int cqz) const;
    // Future: int scoreITUZone(int ituz) const;

    int resolveModeId(const QString &mode) const;

    Awards *awards;
    World *world;
    DataProxy_SQLite *dataProxy;
    ClubLogMostWanted *mostWanted;
    QString userContinent;
    bool checkMode;
};

#endif // KLOG_CLUSTER_DXASSISTANTENGINE_H
