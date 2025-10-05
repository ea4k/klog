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

QSODataCache::QSODataCache() {
    qDebug() << Q_FUNC_INFO;
}
QSODataCache::~QSODataCache() {}

QString QSODataCache::getModeFromSubmode(const QString &_sm)
{
    qDebug() << Q_FUNC_INFO;
    return submodeModeHash.value(_sm);
}

bool QSODataCache::loadSubmodeModeHash()
{
    //qDebug() << Q_FUNC_INFO;
    submodeModeHash.clear();

    QSqlQuery query;
    QString queryString = "SELECT submode, name FROM mode";

    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        //emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        //qDebug() << Q_FUNC_INFO << "END-FAIL-1 - !sqlOK";
        return false;
    }

    while (query.next()) {
        if (query.isValid()) {
            QString submode = query.value(0).toString();
            QString mode    = query.value(1).toString();
            submodeModeHash.insert(submode, mode);
            //qDebug() << Q_FUNC_INFO << submode << "/" << mode;
        }
    }

    query.finish();
    //qDebug() << Q_FUNC_INFO << "END";
    //qDebug() << Q_FUNC_INFO << ": count: " << QString::number(submodeModeHash.count());
    return true;
}

void QSODataCache::reloadAll()
{
    qDebug() << Q_FUNC_INFO;
    loadSubmodeModeHash();
    //loadModeIdHash();
}
