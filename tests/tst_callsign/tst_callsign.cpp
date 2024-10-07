/***************************************************************************
                          tst_frequency.h  -  description
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
#include "../../src/callsign.h"
//#include "QtTest/qtestcase.h"


class tst_Callsign : public QObject
{
    Q_OBJECT

public:
    tst_Callsign();
    ~tst_Callsign();

private slots:
    void initTestCase(); //will be called before the first test function is executed.
    //void initTestCase_data(); // will be called to create a global test data table.
    void cleanupTestCase(); //will be called after the last test function was executed.
    void test_Constructors();
    void test_prefixes_data();
    void test_prefixes();
    void test_prefixes2();

private:
    //Callsign *callsign;
};

tst_Callsign::tst_Callsign()
{
    //callsign = new Callsign("EA4K");
}

tst_Callsign::~tst_Callsign()
{
    //delete(callsign);
}

void tst_Callsign::initTestCase()
{
}

void tst_Callsign::cleanupTestCase()
{
    //qDebug("Called after last test.");
}

void tst_Callsign::test_Constructors()
{

}


void tst_Callsign::test_prefixes_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("fullprefix");
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<int>("areanumber");

    QTest::newRow("EA4") << "EA4" << "EA4" << "EA" << 4;
    QTest::newRow("EA") << "EA" << "EA" << "EA" << 0;
    QTest::newRow("AM100") << "AM100" << "AM100" << "AM" << 100;
    QTest::newRow("K1") << "K1" << "K1" << "K" << 1;
    QTest::newRow("KB1") << "KB1" << "KB1" << "KB" << 1;
    QTest::newRow("2E3") << "2E3" << "2E3" << "2E" << 3;
    QTest::newRow("E74") << "E74" << "E74" << "E7" << 4;
}

void tst_Callsign::test_prefixes()
{
    QFETCH(QString, string);
    QFETCH(QString, fullprefix);
    QFETCH(QString, prefix);
    QFETCH(int, areanumber);
    Callsign pref(string);

    QCOMPARE(pref.isValidPrefix(), true);
    QCOMPARE(pref.getHostPrefix(), fullprefix);
    //QCOMPARE(pref.getHostPrefixWithoutNumber(), prefix);
    //QCOMPARE(pref.getAreaNumber(), areanumber);
}


void tst_Callsign::test_prefixes2()
{
    //qDebug() << Q_FUNC_INFO;
    /*
    QString aux;
    QStringList _validPrefixes= { "K", "K1", "KB1", "EA", "EA4", "EA6",
                          "2E", "2E1", "E7", "E73" };
    QStringList _wrongPrefixes = { "1", "11", "KBA", ""};

    foreach(aux, _validPrefixes)
    {
        Callsign call(aux);
        //qDebug() << aux;
        QCOMPARE(call.isValidPrefix(), true);
        QCOMPARE(call.isValid(), false);
    }
    foreach(aux, _wrongPrefixes)
    {
        Callsign call(aux);
        QCOMPARE(call.isValidPrefix(), false);
        QCOMPARE(call.isValid(), false);
    }
    */
}

QTEST_APPLESS_MAIN(tst_Callsign)

#include "tst_callsign.moc"

