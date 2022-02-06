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
#include "../../src/klogdefinitions.h"
#include "../../src/locator.h"

class tst_Locator : public QObject
{
    Q_OBJECT

public:
    tst_Locator();
    ~tst_Locator();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_isValid();
    void test_Coordinates();

private:
    Locator *locator;
};

tst_Locator::tst_Locator()
{
    locator = new Locator;
}

tst_Locator::~tst_Locator()
{

}

void tst_Locator::initTestCase()
{

}

void tst_Locator::cleanupTestCase()
{

}

void tst_Locator::test_Constructor()
{
    //QVERIFY(util->getVersion() == "0.0");
    //qDebug() << "Testing the constructor" << endl;
}

void tst_Locator::test_isValid()
{
    //isValidLocator(const QString& tlocator){
    QVERIFY2(locator->isValidLocator ("IN"), "IN tested false");
    QVERIFY2(locator->isValidLocator ("IN80"), "IN80 tested false");
    QVERIFY2(locator->isValidLocator ("IN80aa"), "IN80aa tested false");
    QVERIFY2(locator->isValidLocator ("IN80aa"), "IN80aa tested false");
    QVERIFY2(locator->isValidLocator ("IN80aa16"), "IN80aa16 tested false");
    QVERIFY2(locator->isValidLocator ("IN80xx"), "IN80xx tested false");
    QVERIFY2(locator->isValidLocator ("IN99xx"), "IN80xx tested false");
    QVERIFY2(locator->isValidLocator ("IN80xx99"), "IN80aa16 tested false");

    QVERIFY2(!locator->isValidLocator ("I"), "I tested true");
    QVERIFY2(!locator->isValidLocator ("I8"), "I8 tested true");
    QVERIFY2(!locator->isValidLocator ("IN8"), "IN8 tested true");
    QVERIFY2(!locator->isValidLocator ("IN80a"), "IN80a tested true");
    QVERIFY2(!locator->isValidLocator ("IN80aa1"), "IN80aa16 tested true");

    QVERIFY2(!locator->isValidLocator ("IS"), "IS tested true");
    QVERIFY2(!locator->isValidLocator ("SN"), "SN tested true");
    QVERIFY2(!locator->isValidLocator ("IN80ay"), "IN80as tested true");
    QVERIFY2(!locator->isValidLocator ("IN80ya"), "IN80sa tested true");
}

void tst_Locator::test_Coordinates()
{
    double delta = 0.005;
    //QString loc = "IN80AA";
    //qDebug() << Q_FUNC_INFO << QString(": %1=> Lat/long = %2/%3").arg(loc).arg(locator->getLat(loc)).arg(locator->getLon(loc));

    //qDebug() << Q_FUNC_INFO << ": Lat: "  << QString("%1").arg(locator->getLat ("IN80aa"));
    //qDebug() << Q_FUNC_INFO << ": Lon: "  << QString("%1").arg(locator->getLon ("IN80aa"));
    //qDebug() << Q_FUNC_INFO << ": Lat: "  << QString("%1").arg(locator->getLat ("IO"));
    //qDebug() << Q_FUNC_INFO << ": Lon: "  << QString("%1").arg(locator->getLon ("IO"));
    //qDebug() << Q_FUNC_INFO << ": Lat: "  << QString("%1").arg(locator->getLat ("IO93"));
    //qDebug() << Q_FUNC_INFO << ": Lon: "  << QString("%1").arg(locator->getLon ("IO93"));
    //qDebug() << Q_FUNC_INFO << ": Lat: "  << QString("%1").arg(locator->getLat ("IO93ob"));
    //qDebug() << Q_FUNC_INFO << ": Lon: "  << QString("%1").arg(locator->getLon ("IO93ob"));

    QVERIFY2(locator->getLat ("IN") == 45, "Lat failed for IN");
    QVERIFY2(locator->getLon ("IN") == -10, "Lon failed for IN");
    QVERIFY2(locator->getLat ("IN80") == 40.5, "Lat failed for IN80");
    QVERIFY2(locator->getLon ("IN80") == -3.0, "Lon failed for IN80");
    //qDebug() << QString::number(static_cast<double>(locator->getLat("IN80AA")) - 40.0208);
    QVERIFY2(abs(abs(static_cast<double>(locator->getLat("IN80AA"))) - 40.0208) <= delta, "Lat failed for IN80AA");
    QVERIFY2(abs(abs(static_cast<double>(locator->getLon("IN80AA"))) - abs(-3.95833)) <= delta, "Lon failed for IN80AA");

    // Test the
    //Coordinate Locator::getLocatorCorner (const QString& tlocator, bool northWest)
    QString loc = "QG20";
    Coordinate corner;
    qDebug() << "Center: " << QString::number(locator->getLat (loc)) << "," << QString::number(locator->getLon (loc));
    corner = locator->getLocatorCorner (loc, true);
    qDebug() << "North: " << QString::number(corner.lat) << "," << QString::number(corner.lon);
    corner = locator->getLocatorCorner (loc, false);
    qDebug() << "South: " << QString::number(corner.lat) << "," << QString::number(corner.lon);

}

/*
  double getLat(const QString& tlocator);
  double getLon(const QString& tlocator);
  QString getLocator(const double lon1, const double lat1) const;
  int getBeam(const double lon1, const double lat1, const double lon2, const double lat2);
  int getBeamBetweenLocators (const QString& tlocator1, const QString& tlocator2);
  int getDistance(const double lon1, const double lat1, const double lon2, const double lat2, const bool _imperialSystem);
  int getDistanceBetweenLocators (const QString& tlocator1, const QString& tlocator2, const bool _imperialSystem);
  Coordinate getLocatorCorner (const QString& tlocator, bool northWest = true); //northWest = returns the Noth West corner, false implies South East
  bool checkCoords(const double lon1, const double lat1);
*/
QTEST_APPLESS_MAIN(tst_Locator)

#include "tst_locator.moc"
