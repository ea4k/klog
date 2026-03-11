/***************************************************************************
                          datacache.cpp  -  description
                             -------------------
    begin                : Feb 2026
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
#include "datacache.h"

DataCache::DataCache() {
    bandListIsBuilt = false;
    modeListIsBuilt = false;
}

DataCache::~DataCache() {};

void DataCache::addBand(int id, const QString &name, Frequency min, Frequency max)
{
    BandEntry entry = {
        id,
        name.toUpper(),
        min,
        max,
    };
    bandList.append(entry);
    bandListIsBuilt = true;    
}

BandEntry DataCache::getBandFromFreq(Frequency freq) const
{
    for (const auto &band : bandList) {
        if (band.contains(freq)) {
            return band;
        }
    }
    // Return empty/invalid entry if not found
    return BandEntry{-1, "", Frequency(0.0), Frequency(0.0)};
}

BandEntry DataCache::getBandFromName(const QString &name) const
{
    const QString upper = name.toUpper();
    for (const auto &band : bandList) {
        if (band.name == upper  ) {
            return band;
        }
    }
    // Return empty/invalid entry if not found
    return BandEntry{-1, "", Frequency(0.0), Frequency(0.0)};
}

bool DataCache::isBandListOK() const {
    return bandListIsBuilt;
}
bool DataCache::isModeListOK() const {
    return modeListIsBuilt;
}


BandEntry DataCache::getBandFromId(int id) const
{
    for (const auto &band : bandList) {
        if (band.id == id) {
            return band;
        }
    }
    // Return empty/invalid entry if not found
    return BandEntry{-1, "", Frequency(0.0), Frequency(0.0)};
}

void DataCache::addMode(int id, const QString &submode, const QString &mode,
                        const QString &cabrillo, bool deprecated)
{
    ModeEntry entry = { id, submode.toUpper(), mode.toUpper(),
                       cabrillo.toUpper(), deprecated };
    modeList.append(entry);
    modeListIsBuilt = true;
}

ModeEntry DataCache::getModeFromSubmode(const QString &submode) const
{
    const QString upper = submode.toUpper();
    for (const auto &m : modeList)
        if (m.submode == upper)
            return m;
    return ModeEntry{-1, "", ""};
}

ModeEntry DataCache::getModeFromId(int id) const
{
    for (const auto &m : modeList)
        if (m.id == id)
            return m;
    return ModeEntry{-1, "", ""};
}

int DataCache::getModeIdFromSubmode(const QString &submode) const
{
    return getModeFromSubmode(submode).id;
}

QString DataCache::getModeNameFromSubmode(const QString &submode) const
{
    return getModeFromSubmode(submode).mode;
}

bool DataCache::isModeDeprecated(const QString &submode) const
{
    return getModeFromSubmode(submode).deprecated;
}
