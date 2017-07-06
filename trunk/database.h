#ifndef DATABASE_H
#define DATABASE_H
/***************************************************************************
                          database.h  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QObject>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QDateTime>
#include <QInputDialog>
#include <QProgressDialog>
#include "utilities.h"

class QSqlRelationalTableModel;

// Previous db update 0.009
const float DBVersionf = 0.010;

class DataBase
{

public:
    DataBase();
    //DataBase(const QString _softVersion, bool  inmemoryonly = false);
    DataBase(const QString _softVersion);
    ~DataBase();
    bool createConnection();
    bool reConnect();
    bool setDir(const QString _dir);

    //bool beginTransaction();

    bool isValidBand (const QString b);
    bool isValidMode (const QString b, const bool _tmp);
    bool isValidBandNumber (const int b);
    bool isValidModeNumber (const int b);
    QString getBandNameFromNumber(const int _n);
    QString getModeNameFromNumber(const int _n, const bool _tmp);
    QString getSubModeNameFromNumber(const int _n, const bool _tmp);

    int getBandIdFromName(const QString b);
    int getModeIdFromName(const QString b);
    int getModeIdFromSubMode(const QString b, const bool _tmp);

    bool createBandModeMaps();
    int getBandIDFromName2(const QString b);
    int getModeIDFromName2(const QString b);
    QString getBandNameFromID2(const int _i);
    QString getModeNameFromID2(const int _i);

    int getBandIdFromFreq(const QString fr); //Freq should be in MHz
    bool isThisFreqInBand(const QString b, const QString fr); //Freq should be in MHz

    QString getFreqFromBandId(const int _i);

    int getLogTypeNumber(const QString _logType);
    QString getLogTypeName(const int _logType);

    bool unMarkAllQSO();
    bool updateIfNeeded();
    void compress();

    bool updateTheEntityTableISONames();
    bool updateTableLogs();

private:
    bool createDataBase();
    bool isTheDBCreated();
    bool hasTheTableData(const QString _tableName);
    bool updateToLatest();
    bool updateTo003(); // Updates the DB to 0.0.3
    bool updateTo004();
    bool updateTo005();
    bool updateTo006();
    bool updateTo007();
    bool updateTo008();
    bool updateTo009(); // Updates DB and add the Satellite tables
    bool updateTo010(); // Updates DB and recreates the supportedcontest table
    bool updateTableLog(const int _v);
    bool updateDBVersion();

    bool createTheBandQuickReference();
    bool createTheModeQuickReference();

    //bool updateLog(); // Updates the log table
    bool createTableLog(bool temp = false); // false creates the production DB. True a temporal one.
    bool createTableLogs(const bool real=true); // real = true creates the production DB. False a temporal one.

    bool createTableEntity(const bool NoTmp);
    bool updateTableEntity();

    bool createTablePropModes();
    bool createTableClubLogStatus();
    bool populateTableClubLogStatus();

    bool createTableMode(const bool NoTmp);
    bool populateTableMode(const bool NoTmp);
    bool updateTheModeTableAndSyncLog();

    bool createTableBand(const bool NoTmp);
    bool populateTableBand(const bool NoTmp);

    bool recreateSatelliteData();
    bool createTableSatellites(const bool NoTmp);
    bool populateTableSatellites(const bool NoTmp);

    bool recreateContestData();
    bool recreateSupportedContest();
    bool createTableContest();
    bool createTableSupportedContest();
    bool populateContestData();
    bool populatePropagationModes();
    bool populateTableSupportedContest();

    bool howManyQSOsInLog(const int i);
    void showError();

    //bool moveFromModeIdToSubmodeId();
    bool updateModeIdFromSubModeId();
    bool updateBandIdTableLogToNewOnes();
    bool updateBandIdTableAward(const int _db);
    bool updateModeIdTableAward(const int _db);

    bool created;
    float dbVersion;    // The current version of the DB. May differ from latestReaded if we are updating the DB!
    QString softVersion;
    float latestReaded; // The latest version of DB readed in the DB itself
    bool inMemoryOnly; // The DB is to be created in memory, no file support... Faster but less safe!
    //QDateTime date;
    QHash<QString, int> bandIDHash;
    QHash<QString, int> modeIDHash;
    QHash<int, QString> IDBandHash;
    QHash<int, QString> IDModeHash;
    QHash<int, QString> freqBandIdHash;

    QMap<QString, int> bandQMap;
    QMap<QString, int> modeQMap;

    Utilities *util;

    QSqlDatabase db;
    QString dbDir;

};

#endif // DATABASE_H

