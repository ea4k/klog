#ifndef KLOG_DATACACHE_H
#define KLOG_DATACACHE_H
/***************************************************************************
                          datacache.h  -  description
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
/*
    This class implements Cache of data to prevent querying the database

*/
#include <QHash>
#include <QString>
#include "../frequency.h"

struct BandEntry {
    int id;
    QString name;
    Frequency minFreq;
    Frequency maxFreq;

    // Helper to check inclusion
    bool contains(Frequency freq) const { return (freq >= minFreq && freq <= maxFreq); }
    // Helper to handle "Not Found" state
    bool isValid() const { return !name.isEmpty(); }
};

struct ModeEntry {
    int id;
    QString submode;  // e.g. "USB", "FT8", "JT9C"
    QString mode;     // e.g. "SSB", "FT8", "JT9"
    QString cabrillo = QString();
    bool deprecated = false;
    bool isValid() const { return !mode.isEmpty(); }
};

class DataCache
{
    friend class tst_DataCache;

public:
    DataCache();
    ~DataCache();

    // 1. Add Data (Call this when loading from DB)
    void addBand(int id, const QString &name, Frequency min, Frequency max);
    bool isBandListOK() const;

    // 2. Lookup by Frequency (Returns ID + Name via the struct)
    BandEntry getBandFromFreq(Frequency freq) const;
    BandEntry getBandFromName(const QString &name) const;
    BandEntry getBandFromId(int id) const;

    void addMode(int id, const QString &submode, const QString &mode, const QString &cabrillo, bool deprecated);
    bool isModeListOK() const;

    ModeEntry getModeFromSubmode(const QString &submode) const;
    ModeEntry getModeFromId(int id) const;
    int getModeIdFromSubmode(const QString &submode) const;  // quick shortcut
    QString getModeNameFromSubmode(const QString &submode) const; // quick shortcut
    bool isModeDeprecated(const QString &submode) const;

private:
    QList<BandEntry> bandList;
    QList<ModeEntry> modeList;
    bool bandListIsBuilt, modeListIsBuilt;
};

#endif // DATACACHE_H
