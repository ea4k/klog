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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include <QObject>
//#include<QtCore/QtCore>
//#include<QtCore/QObject>
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
// Previous db update 0.011

const float DBVersionf = 0.018f; // This is the latest version of the DB.


struct AwarddxccEntry
{ // Information to update the awarddxcc table; For other Award tables, the DXCC is just the ID of the award element
    QString dxcc;
    QString band;
    QString mode;
    QString status;
    QString logNumber;
    QString qsoID;
};

struct AwarddxccEntryCheck
{ // Information to update the awarddxcc table; For other Award tables, the DXCC is just the ID of the award element
    QString dxcc;
    QString band;
    QString mode;
    QString status;
    QString logNumber;
};

class DataBase //: public QObject
{
   // Q_OBJECT

public:
    DataBase(const QString &_parentClass, const QString &_DBName);
    //DataBase(const QString _softVersion, bool  inmemoryonly = false);
    DataBase(const QString &_parentClass, const QString &_softVersion, const QString &_DBName);
    //virtual ~DataBase();
    ~DataBase();

    QString getSoftVersion();
    QString getDBVersion();
    QString getDBName();

    bool createConnection(const QString &function, bool newDB=false);    // If true that means that we are creating the DB,
                                                // not just connecting to an existing one.
                                                // That will be done in the default path
    bool reConnect(const QString &_DBName);
    //bool setDir(const QString _dir);
    QStringList getColumnNamesFromTable(const QString &_tableName);


    bool isValidBand (const QString &b);
    bool isValidMode (const QString &b, const bool _tmp=false);
    bool isValidBandNumber (const int b);
    bool isValidModeNumber (const int b);
    QString getBandNameFromNumber(const int _n);
    QString getModeNameFromNumber(const int _n, bool _tmp=false);
    QString getSubModeNameFromNumber(const int _n, bool _tmp=false);

    int getBandIdFromName(const QString &b);
    int getModeIdFromName(const QString &b);
    int getModeIdFromSubMode(const QString &b);

    bool createBandModeMaps();
    int getBandIDFromName2(const QString &b);
    int getModeIDFromName2(const QString &b);
    int getSubModeIDFromName2(const QString &b);

    QString getBandNameFromID2(const int _i);
    QString getModeNameFromID2(const int _i);
    QString getSubModeNameFromID2(const int _i);

    int getBandIdFromFreq(const QString &fr); //Freq should be in MHz
    bool isThisFreqInBand(const QString &b, const QString &fr); //Freq should be in MHz

    QString getFreqFromBandId(const int _i);

    //int getLogTypeNumber(const QString &_logType);
    //QString getLogTypeName(const int _logType);

    bool unMarkAllQSO();
    bool updateIfNeeded();
    void backupB4Update();
    void logBackup();
    void compress();


    bool updateTheEntityTableISONames();
    bool updateTableLogs();

    //bool queryAddField(const QString &_field, const QString &value);
    //bool queryPrepare();
    //bool queryExec();

    //bool queryPrepare(const QString &_query);
    //bool queryBind(const QString _field, const QString value);
    bool updateAwardDXCCTable();
    bool updateAwardWAZTable();
    int getNumberOfQsos(const int _logNumber);

//private slots:
//    void slotPrintErrors(QString _func, QString _msg, int _level);

private:
    //bool beginTransaction();
    //bool commitTransaction();
    bool execQuery(const QString &function, const QString &stringQuery);

    bool createDataBase();
    bool isTheDBCreated();
    bool isTheTableExisting(const QString &_tableName);
    bool hasTheTableData(const QString &_tableName);
    bool requiresManualUpgrade();
    bool updateToLatest();
    bool updateTo003(); // Updates the DB to 0.0.3
    bool updateTo004();
    bool updateTo005();
    bool updateTo006();
    bool updateTo007();
    bool updateTo008();
    bool updateTo009(); // Updates DB and add the Satellite tables
    bool updateTo010(); // Updates DB and recreates the supportedcontest table
    bool updateTo011(); // Updates DB and recreates Satellite data
    bool updateTo012(); // Updates the flags and so on.
    bool updateTo013(); // Update the qsl_via_enumeration bug caused for calling the table just qsl_via
    bool updateTo014(); // Update the Satellite DB
    bool updateTo015(); // Adds the FT4 mode
    bool updateTo016(); // Recreates the satellite table to add the QO-100
    bool updateTo017(); // Merges the qso_date & time_on fields
    bool updateTo018(); // Creates Subdivision

    bool updateTableLog(const int _v);
    bool updateDBVersion(QString _softV, QString _dbV);

    bool createTheBandQuickReference();
    bool createTheModeQuickReference();

    //bool updateLog(); // Updates the log table
    bool recreateTableLog();
    bool createTableLog(bool temp = false); // false creates the production DB. True a temporal one.
    bool createTableLogs(const bool real=true); // real = true creates the production DB. False a temporal one.

    bool createTableEntity(const bool NoTmp);
    bool updateTableEntity();

    bool createTableClubLogStatus();
    bool populateTableClubLogStatus();

    bool createTableMode(const bool NoTmp);
    bool populateTableMode(const bool NoTmp);
    bool updateTheModeTableAndSyncLog();

    bool createTableBand(const bool NoTmp);
    bool populateTableBand(const bool NoTmp);
    bool recreateTableBand();

    bool recreateTableDXCC();
    bool createTableAwardDXCC();
    bool recreateTableWAZ();
    bool createTableAwardWAZ();

    bool createTableSubdivision(const bool NoTmp);
    
    bool recreateSatelliteData();
    bool createTableSatellites(const bool NoTmp);
    bool populateTableSatellites(const bool NoTmp);

    bool recreatePropModes();
    bool createTablePropModes();
    bool populatePropagationModes();

    bool recreateContestData();
    bool recreateSupportedContest();
    bool createTableContest();
    bool createTableSupportedContest();
    bool populateContestData();

    bool populateTableSupportedContest();

    bool createTableQSL_Via_enumeration();
    bool populateTableQSL_Via_enumeration();

    bool howManyQSOsInLog(const int i);
    //void showError();

    //bool moveFromModeIdToSubmodeId();
    bool updateModeIdFromSubModeId();
    bool updateBandIdTableLogToNewOnes();
    bool updateBandIdTableAward(const int _db);
    bool updateModeIdTableAward(const int _db);

    void queryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery);



    bool created;
    float dbVersion;    // The current version of the DB. May differ from latestReaded if we are updating the DB!
    QString dbConnectionName;
    QString softVersion;
    float latestReaded; // The latest version of DB readed in the DB itself
    //bool inMemoryOnly; // The DB is to be created in memory, no file support... Faster but less safe!
    //QDateTime date;
    QHash<QString, int> bandIDHash;
    QHash<QString, int> modeIDHash;
    QHash<QString, int> subModeIDHash;
    QHash<int, QString> IDBandHash;
    QHash<int, QString> IDModeHash;
    QHash<int, QString> IDSubModeHash;
    QHash<int, QString> freqBandIdHash;

    QMap<QString, int> bandQMap;
    QMap<QString, int> modeQMap;

    Utilities *util;

    QSqlDatabase db;
    QString dbDir, dbName;

    QStringList insertPreparedQueries, insertQueryFields;
    QSqlQuery preparedQuery;

    int constrid; // Just an id for the constructor to check who is being executed at one specific time

//signals:
    //void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution
    //void debugLog(QString functionFailed, QString errorCode, int level); // emitted when a function retuns false due to an error


};

#endif // DATABASE_H

