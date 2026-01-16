/***************************************************************************
                          qsodatacache.cpp  -  description
                             -------------------
    begin                : oct 2025
    copyright            : (C) 2025 by Jaime Robles
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

#include "qsodatacache.h"

QSODataCache::QSODataCache(const QString &_parentFunction) {
#ifdef QT_DEBUG
    // qDebug() << Q_FUNC_INFO << ": " << _parentFunction;
#else
//    // qDebug() << Q_FUNC_INFO << "Running a release build";
#endif
    (void)_parentFunction;
    ready = false;
}
QSODataCache::~QSODataCache(){}

// qString QSODataCache::getModeFromSubmode(const QString &_sm)
//{
//    // qDebug() << Q_FUNC_INFO;
//    return submodeModeHash.value(_sm);
//}

int QSODataCache::getModeIdFromSubmode(const QString &_sm) const
{
    // qDebug() << Q_FUNC_INFO;
    auto it = submodeModeHash.constFind(_sm);
    if (it != submodeModeHash.constEnd()) {
        return it.value().id;
    }
    return -1; //  "not found"
}

QString QSODataCache::getModeFromSubmode(const QString &_sm) const
{
    // qDebug() << Q_FUNC_INFO << _sm;
    auto it = submodeModeHash.constFind(_sm);
    if (it != submodeModeHash.constEnd()) {
        return it.value().mode;
    }
    return QString(); // or return a fallback/default value if you prefer
}

bool QSODataCache::isReady()
{
    // qDebug() << Q_FUNC_INFO;
    return ready;
}

bool QSODataCache::loadSubmodeModeHash()
{
    // qDebug() << Q_FUNC_INFO;
    submodeModeHash.clear();
    // qDebug() << Q_FUNC_INFO << " - 001";

    QSqlQuery query;
    // qDebug() << Q_FUNC_INFO << " - 002";
    QString queryString = "SELECT id, submode, name FROM mode";
    // qDebug() << Q_FUNC_INFO << " - 003";
    bool sqlOK = query.exec(queryString);
    // qDebug() << Q_FUNC_INFO << " - 010";
    if (!sqlOK)
    {
        //emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        // qDebug() << Q_FUNC_INFO << "END-FAIL-1 - !sqlOK";
        return false;
    }
    // qDebug() << Q_FUNC_INFO << " - 020";
    while (query.next()) {
        if (query.isValid()) {
            //submodeModeHash["USB"] = {1, "SSB"};
            QString submode = query.value(1).toString();

            ModeInfo mInfo;
            mInfo.id = query.value(0).toInt();
            mInfo.mode = query.value(2).toString();
            submodeModeHash.insert(submode, mInfo);
            //submodeModeHash.insert(submode, mode);
            // qDebug() << Q_FUNC_INFO << QString::number(mInfo.id) << "/" << submode << "/" << mInfo.mode;
        }
    }
    // qDebug() << Q_FUNC_INFO << " - 030";
    query.finish();
    // qDebug() << Q_FUNC_INFO << "END";
    // qDebug() << Q_FUNC_INFO << ": count: " << QString::number(submodeModeHash.count());
    return true;
}

bool QSODataCache::reloadAll()
{
    // qDebug() << Q_FUNC_INFO;
    ready = loadSubmodeModeHash();
    return isReady();
}

bool QSODataCache::isValidMode(const QString &_m)
{
    // qDebug() << Q_FUNC_INFO << "Checking mode:" << _m;

    if (!ready) {
        // qDebug() << Q_FUNC_INFO << "Not ready, reloading...";
        if (!reloadAll()) {
            // qDebug() << Q_FUNC_INFO << "Reload failed.";
            return false;
        }
    }

    // Efficient search: check if any value's mode matches _m
    for (const auto &info : submodeModeHash) {
        if (info.mode == _m)
            return true;
    }
    return false;
}

bool QSODataCache::isValidSubMode(const QString &_m)
{
    // qDebug() << Q_FUNC_INFO << "Checking submode:" << _m;

    if (!ready) {
        if (!reloadAll()) {
            // qDebug() << Q_FUNC_INFO << "Reload failed.";
            return false;
        }
    }

    return submodeModeHash.contains(_m);
}
