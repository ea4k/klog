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
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify         *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
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

class QSqlRelationalTableModel;

// Last db update 0.0.6
// Previous db update 0.0.5
const float DBVersionf = 0.006;

class DataBase
{

public:
    DataBase();
    DataBase(const QString _softVersion, bool  inmemoryonly = false);
    ~DataBase();
    bool createConnection();

    //bool beginTransaction();

    bool isValidBand (const QString b);
    bool isValidMode (const QString b);
    bool isValidBandNumber (const int b);
    bool isValidModeNumber (const int b);
    QString getBandNameFromNumber(const int _n);
    QString getModeNameFromNumber(const int _n);

    int getBandIdFromName(const QString b);
    int getModeIdFromName(const QString b);

    bool createBandModeMaps();
    int getBandIDFromName2(const QString b);
    int getModeIDFromName2(const QString b);
    QString getBandNameFromID2(const int _i);
    QString getModeNameFromID2(const int _i);

    int getBandIdFromFreq(const QString fr); //Freq should be in MHz
    bool isThisFreqInBand(const QString b, const QString fr); //Freq should be in MHz

    double getFreqFromBandId(const int _i);

    bool unMarkAllQSO();
    bool updateIfNeeded();
    void compress();



private:
    bool createDataBase();
    bool isTheDBCreated();
    bool updateToLatest();
    bool updateTo003(); // Updates the DB to 0.0.3
    bool updateTo004();
    bool updateTo005();
    bool updateTo006();
    bool updateTableLogTo006();

    bool createTheBandQuickReference();
    bool createTheModeQuickReference();

    bool updateLog(); // Updates the log table
    bool createTableLog(bool temp = false); // false creates the prouction DB. True a temporal one.
    bool createTableLogs();
    bool createTablePropModes();
    bool createTableClubLogStatus();
    bool populateTableClubLogStatus();

    bool recreateContestData();
    bool createTableContest();
    bool populateContestData();
    bool populatePropagationModes();

    bool howManyQSOsInLog(const int i);
    void showError(const QString _errorC);

    float dbVersion;    // The current version of the DB. May differ from latestReaded if we are updating the DB!
    QString softVersion;
    float latestReaded; // The latest version of DB readed in the DB itself
    bool inMemoryOnly; // The DB is to be created in memory, no file support... Faster but less safe!
    QDateTime date;
    QHash<QString, int> bandIDHash;
    QHash<QString, int> modeIDHash;
    QHash<int, QString> IDBandHash;
    QHash<int, QString> IDModeHash;
    QHash<int, double> freqBandIdHash;

    QMap<QString, int> bandQMap;
    QMap<QString, int> modeQMap;


    //QSqlDatabase *db;

};

#endif // DATABASE_H

