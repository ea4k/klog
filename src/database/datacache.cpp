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
    bandListIsBuild = false;
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
    bandListIsBuild = true;
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

BandEntry DataCache::getBandFromName(const QString &name) const {
    for (const auto &band : bandList) {
        if (band.name == name  ) {
            return band;
        }
    }
    // Return empty/invalid entry if not found
    return BandEntry{-1, "", Frequency(0.0), Frequency(0.0)};
}

bool DataCache::isBandListOK() const {
    return bandListIsBuild;
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
