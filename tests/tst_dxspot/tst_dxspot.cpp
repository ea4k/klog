/***************************************************************************
                          tst_dxspot.h  -  description
                             -------------------
    begin                : Apr 2024
    copyright            : (C) 2024 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/****************************************************************************
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
#include <QDateTime>
#include "../../src/dxcluster/dxspot.h"
#include "../../src/klogdefinitions.h"
#include "QtCore/qobject.h"
#include "QtCore/qtmetamacros.h"
#include "QtTest/qtestcase.h"


class tst_DXSpot : public QObject
{
    Q_OBJECT

public:
    tst_DXSpot();
    ~tst_DXSpot();

private slots:
    void initTestCase(); //will be called before the first test function is executed.
    //void initTestCase_data(); // will be called to create a global test data table.
    void cleanupTestCase(); //will be called after the last test function was executed.
    void test_Constructors();
    void test_Basic();
    void test_DXAssistantFields();
    void test_DXAssistantFieldsCopy();


private:
    DXSpot *spot;
};

tst_DXSpot::tst_DXSpot()
{
    spot = new DXSpot;
}

tst_DXSpot::~tst_DXSpot()
{
    delete(spot);

}

void tst_DXSpot::initTestCase()
{
    spot->clear();
}

void tst_DXSpot::cleanupTestCase()
{
    // qDebug("Called after last test.");
}

void tst_DXSpot::test_Constructors()
{
    DXSpot spot1;
    QVERIFY2(!spot1.isValid(), "Constructor-1 not working");
}

void tst_DXSpot::test_Basic()
{

    spot->setDXCall("EA0XX");
    QVERIFY2(spot->getDxCall() == "EA0XX", "dxCall not working");

    spot->setSpotter("EA0XX");
    QVERIFY2(spot->getSpotter() == "EA0XX", "spotter not working");

    spot->setComment("Comment");
    QVERIFY2(spot->getComment() == "Comment", "comment not working");

    QDateTime t = QDateTime::currentDateTime();
    spot->setDateTime(t);
    QVERIFY2(spot->getDateTime() == t, "dateTime not working");

    //enum MouseClicks {NoClick, RightClick, SingleClick, DoubleClick};
    spot->setClickStatus(NoClick);
    QVERIFY2(spot->getClickStatus() == NoClick, "Click Status 0 not working");

    spot->setClickStatus(RightClick);
    QVERIFY2(spot->getClickStatus() == RightClick, "Click Status 1 not working");

    spot->setClickStatus(SingleClick);
    QVERIFY2(spot->getClickStatus() == SingleClick, "Click Status 2 not working");

    spot->setClickStatus(DoubleClick);
    QVERIFY2(spot->getClickStatus() == DoubleClick, "Click Status 3 not working");
}

void tst_DXSpot::test_DXAssistantFields()
{
    DXSpot spot1;
    // Defaults
    QVERIFY2(spot1.getDXCC() == -1, "dxcc default not working");
    QVERIFY2(spot1.getBandId() == -1, "bandId default not working");
    QVERIFY2(spot1.getSpotterContinent().isEmpty(), "spotterContinent default not working");
    QVERIFY2(spot1.getScore() == -1, "score default not working");
    QVERIFY2(spot1.getStatusBand() == unknown, "statusBand default not working");
    QVERIFY2(spot1.getMostWantedRank() == 0, "mostWantedRank default not working");
    QVERIFY2(!spot1.getHidden(), "hidden default not working");

    // Setters/getters
    spot1.setDXCC(281);
    QVERIFY2(spot1.getDXCC() == 281, "dxcc not working");

    spot1.setBandId(5);
    QVERIFY2(spot1.getBandId() == 5, "bandId not working");

    spot1.setSpotterContinent("EU");
    QVERIFY2(spot1.getSpotterContinent() == "EU", "spotterContinent not working");

    spot1.setScore(1000);
    QVERIFY2(spot1.getScore() == 1000, "score not working");

    spot1.setStatusBand(needed);
    QVERIFY2(spot1.getStatusBand() == needed, "statusBand not working");

    spot1.setMostWantedRank(42);
    QVERIFY2(spot1.getMostWantedRank() == 42, "mostWantedRank not working");

    spot1.setHidden(true);
    QVERIFY2(spot1.getHidden(), "hidden not working");

    // clear() resets them
    spot1.clear();
    QVERIFY2(spot1.getDXCC() == -1, "dxcc not reset by clear");
    QVERIFY2(spot1.getBandId() == -1, "bandId not reset by clear");
    QVERIFY2(spot1.getSpotterContinent().isEmpty(), "spotterContinent not reset by clear");
    QVERIFY2(spot1.getScore() == -1, "score not reset by clear");
    QVERIFY2(spot1.getStatusBand() == unknown, "statusBand not reset by clear");
    QVERIFY2(spot1.getMostWantedRank() == 0, "mostWantedRank not reset by clear");
    QVERIFY2(!spot1.getHidden(), "hidden not reset by clear");
}

void tst_DXSpot::test_DXAssistantFieldsCopy()
{
    DXSpot spot1;
    spot1.setDXCC(281);
    spot1.setBandId(5);
    spot1.setSpotterContinent("NA");
    spot1.setScore(800);
    spot1.setStatusBand(worked);
    spot1.setMostWantedRank(7);
    spot1.setHidden(true);

    // Copy constructor
    DXSpot spot2(spot1);
    QVERIFY2(spot2.getDXCC() == 281, "dxcc not copied by copy constructor");
    QVERIFY2(spot2.getBandId() == 5, "bandId not copied by copy constructor");
    QVERIFY2(spot2.getSpotterContinent() == "NA", "spotterContinent not copied by copy constructor");
    QVERIFY2(spot2.getScore() == 800, "score not copied by copy constructor");
    QVERIFY2(spot2.getStatusBand() == worked, "statusBand not copied by copy constructor");
    QVERIFY2(spot2.getMostWantedRank() == 7, "mostWantedRank not copied by copy constructor");
    QVERIFY2(spot2.getHidden(), "hidden not copied by copy constructor");

    // Assignment operator
    DXSpot spot3;
    spot3 = spot1;
    QVERIFY2(spot3.getDXCC() == 281, "dxcc not copied by operator=");
    QVERIFY2(spot3.getBandId() == 5, "bandId not copied by operator=");
    QVERIFY2(spot3.getSpotterContinent() == "NA", "spotterContinent not copied by operator=");
    QVERIFY2(spot3.getScore() == 800, "score not copied by operator=");
    QVERIFY2(spot3.getStatusBand() == worked, "statusBand not copied by operator=");
    QVERIFY2(spot3.getMostWantedRank() == 7, "mostWantedRank not copied by operator=");
    QVERIFY2(spot3.getHidden(), "hidden not copied by operator=");
}

QTEST_APPLESS_MAIN(tst_DXSpot)

#include "tst_dxspot.moc"

