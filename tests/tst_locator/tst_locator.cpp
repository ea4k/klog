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

/*
  bool isValidLocator(const QString& tlocator);
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
