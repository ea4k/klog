#ifndef WORLD_H
#define WORLD_H
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

//#include <QtWidgets>
#include <QString>
#include <QFile>
#include <QStringList>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QProgressBar>
#include <QProgressDialog>
#include <QtGlobal>
#include <QtDebug>

#include "locator.h"
#include "dataproxy.h"
#include "dataproxy_sqlite.h"
//#include "awards.h"


class QStringList;

enum
{
    Entity_Name = 1,
    Entity_Continent = 2
};

class World : public QWidget
{
    //friend class Awards;
    Q_OBJECT

public:
    World();
    World(const QString _kontestDir);
    World(const QString _kontestDir, const QString _kontestVer);

    ~World();
    bool create(const QString _kontestDir);
    bool recreate(const QString _kontestDir);

    QString getQRZEntityName(const QString _qrz);
    QString getEntityName(const int _entityN);

    QString getQRZEntityMainPrefix(const QString _qrz);
    QString getEntityMainPrefix(const int _entityN);

    QString getQRZContinentNumber(const QString _qrz); // Returns the continent id number
    int getContinentNumber(const int _enti); // Returns the continent id number

    QString getQRZContinentShortName(const QString _qrz); // Returns the continent shortName (EU, AF, ...)
    QString getContinentShortName(const int _enti);

    QString getQRZLocator(const QString _qrz); // Returns the entity locator
    QString getLocator(const int _entityN); // Returns the entity locator

    double getQRZLongitude(const QString _qrz); // Returns the longitude of the Entity
    double getLongitude(const int _enti); // Returns the longitude of the Entity

    double getQRZLatitude(const QString _qrz); // Returns the latitude of the Entity
    double getLatitude(const int _enti); // Returns the latitude of the Entity


    int getEntityCqz(const int _enti);
    int getQRZCqz(const QString _qrz);
    //int getPrefixCQz(const QString _p);

    int getQRZItuz(const QString _qrz);
    int getEntityItuz(const int _enti);

    int getQRZARRLId(const QString _qrz); //Returns the ARRL id of the Entity from a QRZ & Returns -1 if not found.
    bool isNewCQz(const int _cqz);
    bool isNewEntity(const int _entityN);
    //int getBandIdFromFreq(const QString fr);

    QString getQRZEntityPrefixes(const QString _qrz);
    QString getEntityPrefixes(const int _enti);

    bool checkQRZValidFormat(const QString _qrz);

    QStringList getEntitiesNames();
    int getHowManyEntities();

private slots:


private:
    int getPrefixId(const QString _qrz);
    bool readCTYDAT();
    bool readCTYCSV();
    QStringList processLine(const QString _line);
    QStringList processLineP(const QString _line, const int _processingEntity);
    void createWorldModel();
    QStringList readZones(const QString &pref, const int _cq, const int _itu);
    QString changeSlashAndFindPrefix(const QString _qrz); // Changes the \ into / and find the country prefix

    int progressBarPosition;

    bool created;
    QString kontestDir, kontestVersion;
    int cqz, ituz, numberOfEntities;
    QString entityName;
    QString currentPrefix; // Used in the progressBar

    bool ret;
    QStringList list, prefixAndZones;
    QString continentName, prefix;
    int continentId;
    double lat, lon, utc;
    //QString line;
    //bool readingDataOfAnEntity;
    int nullValue;

    QSqlRelationalTableModel *worldModel;
    //QProgressBar *progressBar;

    Locator *locator;
    DataProxy *dataProxy;
    //Awards *awards;

};


#endif // WORLD_H
