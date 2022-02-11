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
    void test_LocatorCorners();
    void test_Coordinates2Locator();

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
    Coordinate testCord;
    testCord.lat = 0.0;
    testCord.lon = 0.0;
    testCord = locator->getLocatorCoordinate("IN80");
    QVERIFY2(testCord.lat == 40.5, "Lat failed for Coordinate");
    QVERIFY2(testCord.lon == -3.0, "Lon failed for Coordinate");

    QVERIFY2(locator->getLat ("IN") == 45, "Lat failed for IN");
    QVERIFY2(locator->getLon ("IN") == -10, "Lon failed for IN");
    QVERIFY2(locator->getLat ("IN80") == 40.5, "Lat failed for IN80");
    QVERIFY2(locator->getLon ("IN80") == -3.0, "Lon failed for IN80");
    //qDebug() << QString::number(static_cast<double>(locator->getLat("IN80AA")) - 40.0208);
    QVERIFY2(abs(abs(static_cast<double>(locator->getLat("IN80AA"))) - 40.0416667) <= delta, "Lat failed for IN80AA");
    QVERIFY2(abs(abs(static_cast<double>(locator->getLon("IN80AA"))) - abs(-3.95833)) <= delta, "Lon failed for IN80AA");
}

void tst_Locator::test_LocatorCorners()
{
    double delta = 0.005;
    // Test the
    //Coordinate Locator::getLocatorCorner (const QString& tlocator, bool northWest)
    Coordinate cornerManual, cornerTest;
    //North
    QString _loc = "IN";
    cornerManual.lat = 50.0;
    cornerManual.lon = -20.0;

    cornerTest = locator->getLocatorCorner (_loc);
    //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
    QVERIFY2(cornerManual.lat == cornerTest.lat, "getLocator Corner North Lat failed for IN-1");
    QVERIFY2(cornerManual.lon == cornerTest.lon, "getLocator Corner North Lon failed for IN-1");

    cornerTest = locator->getLocatorCorner (_loc, true);
    QVERIFY2(cornerManual.lat == cornerTest.lat, "getLocator Corner North Lat failed for IN-2");
    QVERIFY2(cornerManual.lon == cornerTest.lon, "getLocator Corner North Lon failed for IN-2");
    //South
    cornerManual.lat = 40.0;
    cornerManual.lon = 0.0;
    cornerTest = locator->getLocatorCorner (_loc, false);
     //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
    QVERIFY2(cornerManual.lat == cornerTest.lat, "getLocator Corner South Lat failed for IN");
    QVERIFY2(cornerManual.lon == cornerTest.lon, "getLocator Corner South Lon failed for IN");

    _loc = "IN80";
    //North
    cornerManual.lat = 41.0;
    cornerManual.lon = -4.0;

    cornerTest = locator->getLocatorCorner (_loc);
     //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
    QVERIFY2(cornerManual.lat == cornerTest.lat, "getLocator Corner North Lat failed for IN80-1");
    QVERIFY2(cornerManual.lon == cornerTest.lon, "getLocator Corner North Lon failed for IN80-1");

    cornerTest = locator->getLocatorCorner (_loc, true);
    QVERIFY2(cornerManual.lat == cornerTest.lat, "getLocator Corner North Lat failed for IN80-2");
    QVERIFY2(cornerManual.lon == cornerTest.lon, "getLocator Corner North Lon failed for IN80-2");
    //South
    cornerManual.lat = 40.0;
    cornerManual.lon = -2.0;
    cornerTest = locator->getLocatorCorner (_loc, false);
     //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
     QVERIFY2(cornerManual.lat == cornerTest.lat, "getLocator Corner South Lat failed for IN80");
     QVERIFY2(cornerManual.lon == cornerTest.lon, "getLocator Corner South Lon failed for IN80");


     _loc = "IN80AA";
     //North
     cornerManual.lat = 40.0416667;
     cornerManual.lon = -4.000;

     cornerTest = locator->getLocatorCorner (_loc);
     //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner North: Lat failed for IN80AA-1");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner North: Lon failed for IN80AA-1");

     cornerTest = locator->getLocatorCorner (_loc, true);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner North: Lat failed for IN80AA-2");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner North: Lon failed for IN80AA-2");
     //South
     cornerManual.lat = 40.0;
     cornerManual.lon = -3.9166667;
     cornerTest = locator->getLocatorCorner (_loc, false);
      //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner South: Lat failed for IN80AA");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner South: Lon failed for IN80AA");

     //OO36vg
     //North: 56.2916667, 107.750
     //South: 56.250, 107.8333333
     _loc = "OO36vg";
     //North
     cornerManual.lat = 56.2916667;
     cornerManual.lon = 107.750;

     cornerTest = locator->getLocatorCorner (_loc);
     //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner North: Lat failed for OO36vg-1");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner North: Lon failed for OO36vg-1");

     cornerTest = locator->getLocatorCorner (_loc, true);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner North: Lat failed for OO36vg-2");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner North: Lon failed for OO36vg-2");
     //South
     cornerManual.lat = 56.250;
     cornerManual.lon = 107.8333333;
     cornerTest = locator->getLocatorCorner (_loc, false);
      //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner South: Lat failed for OO36vg");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner South: Lon failed for OO36vg");

     //PG73wu
     //North: -26.125, 135.8333333
     //South: -26.1666667, 135.9166667
     _loc = "PG73wu";
     //North
     cornerManual.lat = -26.125;
     cornerManual.lon = 135.8333333;

     cornerTest = locator->getLocatorCorner (_loc);
     //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner North: Lat failed for PG73wu-1");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner North: Lon failed for PG73wu-1");

     cornerTest = locator->getLocatorCorner (_loc, true);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner North: Lat failed for PG73wu-2");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner North: Lon failed for PG73wu-2");
     //South
     cornerManual.lat = -26.1666667;
     cornerManual.lon = 135.9166667;
     cornerTest = locator->getLocatorCorner (_loc, false);
      //qDebug() << QString("CornerManual (lat/lon)=(%1/%2) - CornetTest (lat/lon)=(%3/%4)").arg(cornerManual.lat).arg(cornerManual.lon).arg(cornerTest.lat).arg(cornerTest.lon);
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lat)) - abs(cornerTest.lat)) <= delta, "Corner South: Lat failed for PG73wu");
     QVERIFY2(abs(abs(static_cast<double>(cornerManual.lon)) - abs(cornerTest.lon)) <= delta, "Corner South: Lon failed for PG73wu");
}

void tst_Locator::test_Coordinates2Locator()
{
    //qDebug() << locator->getLocator(-0.0426667,40.0);
    //QVERIFY2(locator->getLocator(-0.0426667,40.0) == "JN00AA", "JN00AA tested false");
    //qDebug() << locator->getLocator(-3.9266667,41.0416667);
    //QVERIFY2(locator->getLocator(-3.9266667,41.0416667) == "JN11XB", "JN11XB tested false");
     //qDebug() << locator->getLocator(36.612377, -92.788664);
    //QVERIFY2(locator->getLocator(-3.9266667,41.0416667) == "EM36OO", "JN11XB tested false");
}

/*

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
