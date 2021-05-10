/***************************************************************************
                          tst_locator.h  -  description
                             -------------------
    begin                : May 2021
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
    void test_Locators();

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

void tst_Locator::test_Locators()
{
    QVERIFY2(locator->isValidLocator("IN80") == true, "Good 2 letters NOK");
    QVERIFY2(locator->isValidLocator("IN80") == true, "Good 4 letters NOK");
    QVERIFY2(locator->isValidLocator("IN80AA") == true, "Good 6 letters NOK");
    QVERIFY2(locator->isValidLocator("IN80AA00") == true, "Good 8 letters NOK");
    QVERIFY2(locator->isValidLocator("IN80AA00AA") == false, "Too long NOK");

    QVERIFY2(locator->isValidLocator("IN") == true, "Good 2 letters NOK");
    QVERIFY2(locator->isValidLocator("INA") == false, "Wrong 3 letters NOK");
    QVERIFY2(locator->isValidLocator("IN0") == false, "Wrong 3 chars NOK");
    QVERIFY2(locator->isValidLocator("999") == false, "Wrong 3 numbers NOK");
    QVERIFY2(locator->isValidLocator("IN9A") == false, "Wrong 4 chars NOK");
    QVERIFY2(locator->isValidLocator("IN80A") == false, "Wrong 5 chars NOK");
    QVERIFY2(locator->isValidLocator("IS80") == false, "Wrong 4 chars bad letters NOK");
    QVERIFY2(locator->isValidLocator("IN80ZA") == false, "Wrong 6 chars bad letters NOK");
    QVERIFY2(locator->isValidLocator("IN80A8") == false, "Wrong 6 chars NOK");
    QVERIFY2(locator->isValidLocator("IN80A8") == false, "Wrong 6 chars NOK");
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getBeam(-3.958333, 40.020833, 151.41092, 70.36555)) << endl;

    //  int getBeam(const double lon1, const double lat1, const double lon2, const double lat2);
    QVERIFY2(locator->getBeam(-3.958333, 40.020833, 22.041667, -87.979167) == 178, "getBeam NOK"); // IN80AA-KA12AA
    QVERIFY2(locator->getBeam(-3.958333, 40.020833, -2.55208, 54.20611) == 3, "getBeam NOK"); // IN80AA-IO84RE
    QVERIFY2(locator->getBeam(-3.958333, 40.020833, 22.58464, 41.24803) == 77, "getBeam NOK"); // IN80AA-KN11HF
    QVERIFY2(locator->getBeam(-3.958333, 40.020833, -6.24349, 15.47479) == 185, "getBeam NOK"); // IN80AA-IK65VL
    QVERIFY2(locator->getBeam(-3.958333, 40.020833, -61.293, -21.10768) == 232, "getBeam NOK"); // IN80AA-FG98IV
    QVERIFY2(locator->getBeam(-3.958333, 40.020833, 135.582, -26.57144) == 80, "getBeam NOK"); // IN80AA-PG73SK
    QVERIFY2(locator->getBeam(-3.958333, 40.020833, 151.41092, 70.36555) == 8, "getBeam NOK"); // IN80AA-QQ50QI

    // int getBeamBetweenLocators (const QString& tlocator1, const QString& tlocator2);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getBeamBetweenLocators("IN80AA", "KA12AA")) << endl;
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "KA12AA") == 178, "getBeamBetweenLocators NOK: IN80AA-KA12AA");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "IO84RE") == 3, "getBeamBetweenLocators NOK: IN80AA-IO84RE");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "KN11HF") == 77, "getBeamBetweenLocators NOK: IN80AA-KN11HF");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "IK65VL") == 185, "getBeamBetweenLocators NOK: IN80AA-IK65VL");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "FG98IV") == 232, "getBeamBetweenLocators NOK: IN80AA-FG98IV");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "PG73SK") == 80, "getBeamBetweenLocators NOK: IN80AA-PG73SK");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "QQ50QI") == 8, "getBeamBetweenLocators NOK: IN80AA-QQ50QI");

    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getLat("IN80AA")) << endl;

    // double getLat(const QString& tlocator);
    QVERIFY2(fabs(locator->getLat("IN80AA") - 40.0208) < 0.001, "getLat IN80AA NOK"); // IN80AA
    QVERIFY2(fabs(locator->getLat("IO84RE") - 54.1875) < 0.001, "getLat IO84RE NOK");
    QVERIFY2(fabs(locator->getLat("KN11HF") - 41.2292) < 0.001, "getLat KN11HF NOK");
    QVERIFY2(fabs(locator->getLat("IK65VL") - 15.4792) < 0.001, "getLat IK65VL NOK");
    QVERIFY2(fabs(locator->getLat("FG98IV") - -21.1042) < 0.001, "getLat FG98IV NOK");
    QVERIFY2(fabs(locator->getLat("PG73SK") - -26.5625) < 0.001, "getLat PG73SK NOK");
    QVERIFY2(fabs(locator->getLat("QQ50QI") - 70.3542) < 0.001, "getLat QQ50QI NOK");

    //double getLon(const QString& tlocator);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getLon("IN80AA")) << endl;
    QVERIFY2(fabs(locator->getLon("IN80AA") - -3.95833) < 0.001, "getLon IN80AA NOK"); // IN80AA
    QVERIFY2(fabs(locator->getLon("IO84RE") - -2.54167) < 0.001, "getLon IO84RE NOK");
    QVERIFY2(fabs(locator->getLon("KN11HF") - 22.625) < 0.001, "getLon KN11HF NOK");
    QVERIFY2(fabs(locator->getLon("IK65VL") - -6.20833) < 0.001, "getLon IK65VL NOK");
    QVERIFY2(fabs(locator->getLon("FG98IV") - -61.2917) < 0.001, "getLon FG98IV NOK");
    QVERIFY2(fabs(locator->getLon("PG73SK") - 135.542) < 0.001, "getLon PG73SK NOK");
    QVERIFY2(fabs(locator->getLon("QQ50QI") - 151.375) < 0.001, "getLon QQ50QI NOK");


    // int getDistance(const double lon1, const double lat1, const double lon2, const double lat2, const bool _imperialSystem);
    // qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistance(-3.958333, 40.020833, 22.041667, -87.979167, true)) << endl;

    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 22.041667, -87.979167, false) == 14255, "getDistance NOK"); // IN80AA-KA12AA
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, -2.55208, 54.20611, false) == 1580, "getDistance NOK"); // IN80AA-IO84RE
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 22.58464, 41.24803, false) == 2235, "getDistance NOK"); // IN80AA-KN11HF
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, -6.24349, 15.47479, false) == 2738, "getDistance NOK"); // IN80AA-IK65VL
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, -61.293, -21.10768, false) == 9022, "getDistance NOK"); // IN80AA-FG98IV
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 135.582, -26.57144, false) == 16009, "getDistance NOK"); // IN80AA-PG73SK
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 151.41092, 70.36555, false) == 7580, "getDistance NOK"); // IN80AA-QQ50QI
    //Now in miles
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 22.041667, -87.979167, true) == 8857, "getDistance NOK"); // IN80AA-KA12AA
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, -2.55208, 54.20611, true) == 982, "getDistance NOK"); // IN80AA-IO84RE
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 22.58464, 41.24803, true) == 1388, "getDistance NOK"); // IN80AA-KN11HF
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, -6.24349, 15.47479, true) == 1701, "getDistance NOK"); // IN80AA-IK65VL
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, -61.293, -21.10768, true) == 5606, "getDistance NOK"); // IN80AA-FG98IV
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 135.582, -26.57144, true) == 9947, "getDistance NOK"); // IN80AA-PG73SK
    QVERIFY2(locator->getDistance(-3.958333, 40.020833, 151.41092, 70.36555, true) == 4710, "getDistance NOK"); // IN80AA-QQ50QI


    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "KA12AA") == 178, "getBeamBetweenLocators NOK: IN80AA-KA12AA");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "IO84RE") == 3, "getBeamBetweenLocators NOK: IN80AA-IO84RE");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "KN11HF") == 77, "getBeamBetweenLocators NOK: IN80AA-KN11HF");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "IK65VL") == 185, "getBeamBetweenLocators NOK: IN80AA-IK65VL");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "FG98IV") == 232, "getBeamBetweenLocators NOK: IN80AA-FG98IV");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "PG73SK") == 80, "getBeamBetweenLocators NOK: IN80AA-PG73SK");
    QVERIFY2(locator->getBeamBetweenLocators("IN80AA", "QQ50QI") == 8, "getBeamBetweenLocators NOK: IN80AA-QQ50QI");

    // int getDistanceBetweenLocators (const QString& tlocator1, const QString& tlocator2, const bool _imperialSystem);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistanceBetweenLocators("IN80AA", "KA12AA", false)) << endl;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistanceBetweenLocators("IN80AA", "IO84RE", false)) << endl;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistanceBetweenLocators("IN80AA", "KN11HF", false)) << endl;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistanceBetweenLocators("IN80AA", "IK65VL", false)) << endl;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistanceBetweenLocators("IN80AA", "FG98IV", true)) << endl;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistanceBetweenLocators("IN80AA", "PG73SK", true)) << endl;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(locator->getDistanceBetweenLocators("IN80AA", "QQ50QI", true)) << endl;

    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "KA12AA", false)) == 14255, "getDistance NOK"); // IN80AA-KA12AA
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "IO84RE", false)) == 1578, "getDistance NOK"); // IN80AA-IO84RE
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "KN11HF", false)) == 2238, "getDistance NOK"); // IN80AA-KN11HF
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "IK65VL", false)) == 2737, "getDistance NOK"); // IN80AA-IK65VL
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "FG98IV", false)) == 9021, "getDistance NOK"); // IN80AA-FG98IV
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "PG73SK", false)) == 16005, "getDistance NOK"); // IN80AA-PG73SK
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "QQ50QI", false)) == 7581, "getDistance NOK"); // IN80AA-QQ50QI
    //Now in miles
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "KA12AA", true)) == 8857, "getDistance NOK"); // IN80AA-KA12AA
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "IO84RE", true)) == 981, "getDistance NOK"); // IN80AA-IO84RE
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "KN11HF", true)) == 1391, "getDistance NOK"); // IN80AA-KN11HF
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "IK65VL", true)) == 1701, "getDistance NOK"); // IN80AA-IK65VL
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "FG98IV", true)) == 5605, "getDistance NOK"); // IN80AA-FG98IV
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "PG73SK", true)) == 9945, "getDistance NOK"); // IN80AA-PG73SK
    QVERIFY2((locator->getDistanceBetweenLocators("IN80AA", "QQ50QI", true)) == 4710, "getDistance NOK"); // IN80AA-QQ50QI


    // QList<double> getTopLeftCoordinate(const QString& tlocator);  // Returns the top left coordinate of a Maidenhead locator
}



QTEST_APPLESS_MAIN(tst_Locator)

#include "tst_locator.moc"
