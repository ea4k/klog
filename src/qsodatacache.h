#ifndef QSODATACACHE_H
#define QSODATACACHE_H

/***************************************************************************
                          qsodatacache.h  -  description
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
#include <QObject>
#include <QHash>
#include <QString>
#include <QSqlQuery>

struct ModeInfo {
    int id;
    QString mode;
};

class QSODataCache: public QObject
{
    Q_OBJECT
    friend class tst_QSODataCache;
public:
    QSODataCache(const QString &_parentFunction);
    ~QSODataCache();
public:
    QString getModeFromSubmode(const QString &_sm) const;
    int getModeIdFromSubmode(const QString &_sm) const;
    bool reloadAll(); //  To reload all the data
    bool isReady();
    bool isValidMode(const QString &_m);
    bool isValidSubMode(const QString &_m);
signals:
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution

private:
    bool loadSubmodeModeHash();
    //void loadModeIdHash();

    //QHash<QString, QString> submodeModeHash; // submode -> mode
    QHash<QString, ModeInfo> submodeModeHash; // submode -> ModeInfo
    bool ready;

};

#endif // QSODATACACHE_H
