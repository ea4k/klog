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
    void test_validCallsign_data();
    void test_validCallsign();


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
    //it is a valid callsign
    //valid = true;
    //hostPrefixWithDelimiter = match.captured("pref");
    //hostPrefix              = match.captured("hostprefix");
    //base                    = match.captured(3);
    //basePrefix              = match.captured(4);
    //basePrefixNumber        = match.captured(5);
    //suffixWithDelimiter     = match.captured(7);
    //suffix                  = match.captured(8);


    //KB1/EA4K/QRP
    QTest::addColumn<QString>("fullcall");              // KB1/EA4K/QRP
    QTest::addColumn<QString>("hostfullcall");          // KB1/EA4K
    QTest::addColumn<QString>("hostfullprefix");        // KB1
    QTest::addColumn<QString>("hostprefix");            // KB
    QTest::addColumn<int>("hostareanumber");            // 1

    QTest::addColumn<QString>("homefullcall");          // EA4K
    QTest::addColumn<QString>("homefullprefix");        // EA4
    QTest::addColumn<QString>("homeprefix");            // EA
    QTest::addColumn<int>("homeareanumber");            // 4
    QTest::addColumn<QString>("homesuffix");            // K

    QTest::addColumn<QString>("additionalsuffix");      // QRP

    QTest::addColumn<bool>("isValidPrefix");            // true
    QTest::addColumn<bool>("isValid");                  // true


    // REGEX to test
        //^(?<fullcall>((?<hostfullcall>(?<hostfullprefix>(?<hostprefix>[A-Z0-9]{1,2})(?<hostareanumber>[0-9]|[0-9]+)?)\\/)?(?<homefullcall>(?<homefullprefix>
        //(?<homeprefix>[A-Z][0-9]|[A-Z]{1,2}|[0-9][A-Z])(?<homeareanumber>[0-9]|[0-9]+))(?<homesuffix>[A-Z]+)))(\\/(?<additionalsuffix>[A-Z0-9]+))?)
    //                                                                                                                                                          isprefixValid   isValid
    QTest::newRow("KB1/EA4K/QRP")   << "KB1/EA4K/QRP"   << "KB1/EA4K"   << "KB1"    << "KB" << 1    << "EA4K"   << "EA4"    << "EA" << 4    << "K"  << "QRP"    << true     << true;
    QTest::newRow("EA4K")           << "EA4K"           << ""           << ""       << ""   << -1   << "EA4K"   << "EA4"    << "EA" << 4    << "K"  << ""       << true     << true;
    QTest::newRow("EA4K/P")         << "EA4K/P"         << ""           << ""       << ""   << -1   << "EA4K"   << "EA4"    << "EA" << 4    << "K"  << "P"      << true     << true;
    QTest::newRow("2E4K")           << "2E4K"           << ""           << ""       << ""   << -1   << "2E4K"   << "2E4"    << "2E" << 4    << "K"  << ""       << true     << true;
    QTest::newRow("AM100")          << ""               << ""           << ""       << ""   << -1   << ""       << "AM100"  << "AM" << 100  << ""   << ""        << true    << false;

    //QTest::newRow("AM100") << "AM100" << "AM100" << "AM" << 100;    // AM100
    //QTest::newRow("K1") << "K1" << "K1" << "K" << 1;                // K1
    //QTest::newRow("KB1") << "KB1" << "KB1" << "KB" << 1;            // KB1
    //QTest::newRow("2E3") << "2E3" << "2E3" << "2E" << 3;            // 2E3
    //QTest::newRow("E74") << "E74" << "E74" << "E7" << 4;            // E74
}

void tst_Callsign::test_prefixes()
{
    QFETCH(QString, fullcall);
    QFETCH(QString, hostfullprefix);
    QFETCH(QString, hostprefix);
    QFETCH(QString, homefullprefix);
    QFETCH(QString, homeprefix);
    QFETCH(int, hostareanumber);
    QFETCH(bool, isValidPrefix);
    QFETCH(bool, isValid);

    Callsign testCall(fullcall);
    qDebug() << Q_FUNC_INFO << " -         fullcall       : "  << fullcall;
    qDebug() << Q_FUNC_INFO << " -         hostfullprefix : "  << hostfullprefix;
    qDebug() << Q_FUNC_INFO << " -         hostprefix     : "  << hostprefix;
    qDebug() << Q_FUNC_INFO << " -         hostareanumber : "  << QString::number(hostareanumber);
    qDebug() << Q_FUNC_INFO << " - isValidPrefix";
    QCOMPARE(testCall.isValidPrefix(), isValidPrefix);
    qDebug() << Q_FUNC_INFO << " - isValid";
    QCOMPARE(testCall.isValid(), isValid);
    qDebug() << Q_FUNC_INFO << " - fullcall      : "      << testCall.getCallsign() << "/" << fullcall;
    QCOMPARE(testCall.getCallsign(), fullcall);
    qDebug() << Q_FUNC_INFO << " - hostfullprefix:"  << testCall.getHostFullPrefix() << "/" << hostfullprefix;
    QCOMPARE(testCall.getHostFullPrefix(), hostfullprefix);
    qDebug() << Q_FUNC_INFO << " - hostprefix    :"      << testCall.getHostPrefix() << "/" << hostprefix;
    QCOMPARE(testCall.getHostPrefix(), hostprefix);
    qDebug() << Q_FUNC_INFO << " - hostareanumber:"  << QString::number(testCall.getHostAreaNumber()) << "/" << QString::number(hostareanumber);
    QCOMPARE(testCall.getHostAreaNumber(), hostareanumber);
    qDebug() << Q_FUNC_INFO << " - homefullprefix    :"      << testCall.getHomeFullPrefix() << "/" << homefullprefix;
    QCOMPARE(testCall.getHomeFullPrefix(), homefullprefix);
    qDebug() << Q_FUNC_INFO << " - homeprefix    :"      << testCall.getHomePrefix() << "/" << homeprefix;
    QCOMPARE(testCall.getHomePrefix(), homeprefix);


    //QCOMPARE(pref.getHostPrefixWithoutNumber(), prefix);
    //QCOMPARE(pref.getAreaNumber(), areanumber);
}


void tst_Callsign::test_prefixes2()
{

    Callsign call("EA4K");
    QVERIFY2(call.getCallsign() == "EA4K", "Wrong call received");
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


void tst_Callsign::test_validCallsign_data()
{
    QTest::addColumn<QString>("callsign");
    QTest::addColumn<bool>("isValid");

    //Add valid callsigns with different formats
    QTest::newRow("Basic") << "EA4K" << true;
    QTest::newRow("With Prefix") << "EA/EA4K" << true;
    QTest::newRow("With Suffix") << "EA4K/P" << true;
    QTest::newRow("With Numbers") << "EA42K" << true;

    // Add invalid callsigns
    QTest::newRow("With Numbers at the end") << "EA42K11111" << false;
    QTest::newRow("With only numbers") << "21243434" << false;
    QTest::newRow("With only letters") << "KDSHDAUD" << false;
    QTest::newRow("With strange characters end") << "EA4K%" << false;
    QTest::newRow("With strange characters begining") << "%EA4K" << false;
    QTest::newRow("With strange characters middle") << "EA%4K" << false;
}

void  tst_Callsign::test_validCallsign()
{
    //QFETCH(QString, callsign);
    //QFETCH(bool, isValid);
    //Callsign c(callsign);
    //QCOMPARE(c.isValid(), isValid);
}


QTEST_APPLESS_MAIN(tst_Callsign)

#include "tst_callsign.moc"

