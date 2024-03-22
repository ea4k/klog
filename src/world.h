#ifndef KLOG_WORLD_H
#define KLOG_WORLD_H
/***************************************************************************
                          world.h  -  description
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

//#include <QtWidgets>
#include <QString>
#include <QFile>
#include <QStringList>
//#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QProgressBar>
#include <QProgressDialog>
#include <QtGlobal>
#include <QtDebug>

#include "locator.h"
#include "dataproxy_sqlite.h"
#include "utilities.h"
//#include "klogdefinitions.h"
//#include "awards.h"


//class QStringList;

enum
{
    Entity_Name = 1,
    Entity_Continent = 2
};

class World : public QObject
{
    //friend class Awards;
    Q_OBJECT

public:
    World(DataProxy_SQLite *dp, const QString &_parentFunction);
    //World(DataProxy_SQLite *dp, const QString &_klogDir, const QString &_parentFunction);
    //World(DataProxy_SQLite *dp, const QString &_klogVer, const QString &_parentFunction);

    ~World();
    bool create(const QString &_worldFile);
    bool recreate(const QString &_worldFile);
    bool readWorld();

    QString getQRZEntityName(const QString &_qrz);
    QString getEntityName(const int _entityN);

    QString getQRZEntityMainPrefix(const QString &_qrz);
    QString getEntityMainPrefix(const int _entityN);

    QString getQRZContinentNumber(const QString &_qrz); // Returns the continent id number
    int getContinentNumber(const int _enti); // Returns the continent id number

    QString getQRZContinentShortName(const QString &_qrz); // Returns the continent shortName (EU, AF, ...)
    QString getContinentShortName(const int _enti);

    QString getQRZLocator(const QString &_qrz); // Returns the entity locator
    QString getLocator(const int _entityN); // Returns the entity locator

    double getLongitude(const int _enti); // Returns the longitude of the Entity
    double getLatitude(const int _enti); // Returns the latitude of the Entity

    int getEntityCqz(const int _enti);
    int getQRZCqz(const QString &_qrz);
    //int getPrefixCQz(const QString &_p);

    int getQRZItuz(const QString &_qrz);
    int getEntityItuz(const int _enti);

    int getQRZARRLId(const QString &_qrz); //Returns the ARRL id of the Entity from a QRZ & Returns -1 if not found.
    bool isNewCQz(const int _cqz);
    bool isNewEntity(const int _entityN);
    //int getBandIdFromFreq(const QString fr);
    QString getEntityPrefixes(const int _enti);

    //QStringList getEntitiesNames();
    int getHowManyEntities();
    bool hasSpecialEntities();

private slots:


private:
    //void identifyOS();
    bool insertSpecialEntities();
    int getPrefixId(const QString &_prefix);
    //bool readCTYDAT();
    bool readCTYCSV(const QString &_worldFile);
    bool addEntity(const QString &_name, const int _cq, const int _itu, const int _contId, const double _lat, const double _lon, const double _utc, const int _dxcc, const QString &_mainpref);
    bool addPrefix(const QString &_pref, const int _dxcc, const int _cqz, const int _ituz);
    QStringList processLine(const QString &_line);
    QStringList processLineP(const QString &_line, const int _processingEntity);
    //void createWorldModel();
    QStringList readZones(const QString &pref, const int _cq, const int _itu);

    int progressBarPosition;

    bool created;
    //QString klogVersion;
    int cqz, ituz, numberOfEntities;
    QString entityName;
    QString currentPrefix; // Used in the progressBar

    bool ret;
    QStringList list, prefixAndZones;
    QString continentName, prefix;
    int continentId;
    double lat, lon, utc;
    //int utc;
    //QString line;
    //bool readingDataOfAnEntity;
    int nullValue;

    Locator *locator;
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    QHash<QString, int> worldPrefixes;

    //int constrid; // Just an id for the constructor to check who is being executed at one specific time
    //Awards *awards;
    //FLAGS
    //QString flagsDir;
    //FLAGS-END
signals:
    //void qsoFound(const QStringList _qs); // Each: QString with format: Fieldname:value
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution
};


#endif // WORLD_H
