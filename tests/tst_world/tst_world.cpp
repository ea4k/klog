/***************************************************************************
                          tst_world.h  -  description
                             -------------------
    begin                : Jan 2021
    copyright            : (C) 2021 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of Testing suite of KLog.                               *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    The testing suite of KLog is distributed in the hope that it will      *
 *    be useful, but WITHOUT ANY WARRANTY; without even the implied          *
 *    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.       *
 *    See the GNU General Public License for more details.                   *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with Testing suite of KLog.                                      *
 *    If not, see <https://www.gnu.org/licenses/>.                           *
 *                                                                           *
 *****************************************************************************/

#include <QtTest>
#include "../../src/world.h"
#include "../../src/qso.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/database.h"
#include "../../src/utilities.h"

class tst_World : public QObject
{
    Q_OBJECT

public:
    tst_World();
    ~tst_World();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_WorldCreation();
    void test_EntityIdentification();
    void test_ZonesIdentification();
    void test_SeveralIdentification();
/*

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

    int getHowManyEntities();
    bool hasSpecialEntities();
*/
private:
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    World *world;
    QString version;
};

tst_World::tst_World()
{
    //qDebug() << Q_FUNC_INFO << "Start";
    version = "1.5";

    //qDebug() << Q_FUNC_INFO << "001";
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, version);
    //qDebug() << Q_FUNC_INFO << "002";
    util = new Utilities(Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << "003";
    world = new World(dataProxy, Q_FUNC_INFO);

    //QString ctyDatFile = util->getCTYFile();
    //world->create(ctyDatFile);
    //qDebug() << Q_FUNC_INFO << "END";
}

tst_World::~tst_World()
{

}

void tst_World::initTestCase()
{

}

void tst_World::cleanupTestCase()
{

}

void tst_World::test_Constructor()
{
    //QVERIFY(util->getVersion() == "0.0");
    //qDebug() << "Testing the constructor" ;

}

void tst_World::test_WorldCreation()
{
    //qDebug() << "Testing the world";

    //QFAIL( "World YES to be created");
    //QVERIFY2(1==2, "Failure in the qverify2");

}

void tst_World::test_EntityIdentification()
{
    QVERIFY2(world->getQRZARRLId("VA1A") == 1, "VE Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("K1AA") == 291, "K Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("K1A") == 291, "K Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("G1A") == 223, "G Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("GW1A") == 294, "GW Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("GM1A") == 279, "GM Entity not properly identified");

    QVERIFY2(world->getQRZARRLId("K100A") == 291, "B Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("BY1AA") == 318, "BY Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("FB1A") == 227, "FB Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("F4AA") == 227, "F Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("E") < 0, "E Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA") == 281, "EA Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA4") == 281, "EA4 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA4K") == 281, "EA-1 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA4KK") == 281, "EA-2 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA4KKK") == 281, "EA-3 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA4KKKK") == 281, "EA-4 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA6") == 21, "EA6 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("EA6A") == 21, "EA6-1 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("2E1AA") == 223, "2E Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("E73A") == 501, "E7 Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("AM200A") == 281, "AM Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("VP2ME") == 96, "VP2M Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("VP2VE") == 65, "VP2V Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("VP2EE") == 12, "VP2E Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("VK9AA") == 35, "VK9AA Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("VK9MA") == 171, "VP2M Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("AX9X") == 35, "AX9X Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("VK9XX") == 35, "VK9X Entity not properly identified");
    QVERIFY2(world->getQRZARRLId("4U2STAYHOME") == 1206, "4U2 Entity not properly identified");
}

void tst_World::test_ZonesIdentification()
{
    QVERIFY2(world->getQRZCqz("EA4K") == 14, "CQz for EA not properly identified");
    QVERIFY2(world->getQRZCqz("PY") == 11, "CQz for PY not properly identified");
    QVERIFY2(world->getQRZCqz("AX9XA") == 29, "CQz for AX9XA not properly identified");

    QVERIFY2(world->getQRZItuz("EA4K") == 37, "ITUz for EA not properly identified");
    QVERIFY2(world->getQRZItuz("PY") == 15, "ITUz for PY not properly identified");
    QVERIFY2(world->getQRZItuz("AX9XA") ==54, "ITUz for AX9XA not properly identified");
}

void tst_World::test_SeveralIdentification()
{
    QVERIFY2(world->getQRZEntityName("EA4K") == "Spain", "Entity name for EA4K not properly identified");
    QVERIFY2(world->getQRZEntityName("EA4K/P") == "Spain", "Entity name for EA4K/P not properly identified");
    QVERIFY2(world->getQRZEntityName("F4K/EA") == "Spain", "Entity name for F4K/EA not properly identified");
    QVERIFY2(world->getQRZEntityName("EA/F4K") == "Spain", "Entity name for EA/F4K not properly identified");
    QVERIFY2(world->getQRZEntityName("EA5/F4K") == "Spain", "Entity name for EA/F4K not properly identified");
    QVERIFY2(world->getQRZEntityName("EA8/F4K") == "Canary Islands", "Entity name for EA8/F4K not properly identified");
    QVERIFY2(world->getQRZEntityName("PY") == "Brazil", "Entity name for EA4K not properly identified");

    QVERIFY2(world->getEntityName(281) == "Spain", "Entity name for 281 not properly identified");
    QVERIFY2(world->getEntityCqz(281) == 14, "CQz name for 281 not properly identified");
    QVERIFY2(world->getEntityItuz(281) == 37, "ITUz name for 281 not properly identified");
    QVERIFY2(world->getQRZEntityMainPrefix("AM4A") == "EA", "Main prefix for AM4A not properly identified");
    QVERIFY2(world->getEntityMainPrefix(281) == "EA", "Entity main prefix for 281 not properly identified");

    QVERIFY2(world->getQRZContinentShortName("EA4K") == "EU", "Continent for EA4K not properly identified");
    QVERIFY2(world->getQRZContinentShortName("PY") == "SA", "Continent for PY not properly identified");
    QVERIFY2(world->getQRZContinentShortName("AX9XA") == "OC", "Continent for AX9XA not properly identified");

    QVERIFY2(world->getContinentShortName(281) == "EU", "Continent for 281 not properly identified");

    QVERIFY2(world->getLongitude(281) == -3.43, "Longitude for 281 not properly identified");

    QVERIFY2(world->getLatitude(281) == 40.32, "Latitude for 281 not properly identified");

    QVERIFY2(world->getQRZLocator("EA4K") == "IN80GH", "Locator for EA4K not properly identified");
    QVERIFY2(world->getLocator(281) == "IN80GH", "Locator for 281 not properly identified");
}


QTEST_APPLESS_MAIN(tst_World)

#include "tst_world.moc"
