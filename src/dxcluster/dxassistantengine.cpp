/***************************************************************************
                          dxassistantengine.cpp  -  description
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
#include "dxassistantengine.h"
#include "clublogmostwanted.h"
#include "../awards.h"
#include "../world.h"
#include "../dataproxy_sqlite.h"

DXAssistantEngine::DXAssistantEngine(Awards *_awards, World *_world,
                                     DataProxy_SQLite *_dataProxy,
                                     ClubLogMostWanted *_mostWanted,
                                     const QString &_userContinent,
                                     QObject *parent) : QObject(parent)
{
    awards        = _awards;
    world         = _world;
    dataProxy     = _dataProxy;
    mostWanted    = _mostWanted;
    userContinent = _userContinent;
    checkMode     = DX_ASSISTANT_CHECK_MODE;
}

void DXAssistantEngine::setUserContinent(const QString &_continent)
{
    userContinent = _continent;
}

QString DXAssistantEngine::getUserContinent() const
{
    return userContinent;
}

void DXAssistantEngine::setCheckMode(bool _check)
{
    checkMode = _check;
}

bool DXAssistantEngine::getCheckMode() const
{
    return checkMode;
}

bool DXAssistantEngine::score(DXSpot &spot) const
{
    if ((awards == nullptr) || (world == nullptr) || (dataProxy == nullptr))
        return false;

    // Resolve identifiers once at receive time; rescore() reuses them.
    spot.setDXCC(world->getQRZARRLId(spot.getDxCall()));
    if (spot.getDXCC() <= 0)
    {
        spot.setScore(-1);
        return false;
    }

    spot.setBandId(dataProxy->getBandIdFromFreq(spot.getFrequency()));
    if (spot.getBandId() <= 0)
    {
        spot.setScore(-1);
        return false;
    }

    if (spot.getSpotter().isEmpty())
        spot.setSpotterContinent(QString());
    else
        spot.setSpotterContinent(world->getQRZContinentShortName(spot.getSpotter()));

    return rescore(spot);
}

bool DXAssistantEngine::rescore(DXSpot &spot) const
{
    if (awards == nullptr)
        return false;

    if ((spot.getDXCC() <= 0) || (spot.getBandId() <= 0))
    {
        spot.setScore(-1);
        return false;
    }

    // The rank may have been refreshed since the spot was first scored.
    spot.setMostWantedRank((mostWanted != nullptr) ? mostWanted->getRank(spot.getDXCC()) : 0);

    QSOStatus statusBand     = unknown;
    QSOStatus statusBandMode = unknown;
    int baseScore = scoreDXCC(spot.getDXCC(), spot.getBandId(),
                              resolveModeId(spot.getMode()),
                              statusBand, statusBandMode);

    spot.setStatusBand(statusBand);
    spot.setQSOStatus(statusBandMode);  // Same field DXClusterWidget uses for colour

    if (baseScore < 0)
    {   // Confirmed in band+mode → discard
        spot.setScore(-1);
        return false;
    }

    spot.setScore(baseScore + scoreContinentModifier(spot.getSpotterContinent()));
    return true;
}

int DXAssistantEngine::scoreDXCC(int dxcc, int bandId, int modeId,
                                 QSOStatus &outStatusBand, QSOStatus &outStatusBandMode) const
{
    // Note: an ATNO band status means the entity is absent from the Awards
    // status list altogether, so it doubles as the overall (all-bands) check.
    outStatusBand = awards->getQSOStatus(dxcc, bandId, -1);

    if (checkMode && (modeId > 0))
        outStatusBandMode = awards->getQSOStatus(dxcc, bandId, modeId);
    else
        outStatusBandMode = outStatusBand;   // Mode unknown or ignored

    switch (outStatusBand)
    {
    case ATNO:
        return SCORE_ATNO;                                  // Never worked this DXCC
    case needed:
        return SCORE_NOT_WORKED_BAND;                       // Worked DXCC but not on this band
    case worked:
        if (outStatusBandMode == needed)
            return SCORE_NOT_WORKED_BAND_MODE;              // Worked on this band but not this mode
        return SCORE_WORKED_UNCONFIRMED_BAND;               // Worked on this band, unconfirmed
    case confirmed:
        if (outStatusBandMode == needed)
            return SCORE_NOT_WORKED_BAND_MODE;              // Band confirmed, mode never worked
        if (outStatusBandMode == worked)
            return SCORE_WORKED_UNCONFIRMED_MODE;           // Band confirmed, mode not confirmed
        return -1;                                          // Confirmed in band+mode → discard
    case unknown:
    default:
        return -1;                                          // Unresolvable → discard
    }
}

int DXAssistantEngine::scoreContinentModifier(const QString &spotterContinent) const
{
    if (!spotterContinent.isEmpty() && (spotterContinent == userContinent))
        return SCORE_SAME_CONTINENT_BONUS;
    return SCORE_DIFF_CONTINENT_PENALTY;   // Different or unknown continent
}

int DXAssistantEngine::resolveModeId(const QString &mode) const
{
    if (!checkMode || mode.isEmpty() || (dataProxy == nullptr))
        return -1;
    return dataProxy->getIdFromModeName(mode);
}
