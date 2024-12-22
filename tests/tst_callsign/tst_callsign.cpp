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
#include "qtestcase.h"
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
    void test_callsigns();
    void test_callsigns_data();     // Data function

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

void tst_Callsign::test_callsigns_data()
{
    // Functions to test
    //      QString getCallsign();                  // Returns the FULL callsign                                            (fullCall)
    //      QString getHostFullPrefix();            // The complete host prefix (simple + area number if exists)            (hostFullPrefix)
    //      QString getHostPrefix();                // The host prefix (simple without area number if exists)               (hostPrefix)
    //      int getHostAreaNumber();                // Get host area number                                                 (hostAreaNumber)


    //      QString getHomeCallsign();              // Returns the base / home callsign like EA4K in K1/EA4K, or EA4K/QRP   (fullCall)
    //      QString getHomeFullPrefix();            // The complete home prefix (simple + area number if exists)            (homeFullPrefix)
    //      QString getHomePrefix();                // The Home prefix without area number                                  (homePrefix)
    //      int getHomeAreaNumber();                // Get the home area number                                             (homeAreaNumber)
    //      QString getHomeSuffix();                // The suffix of the home call                                          (homeSuffix)
    //      QString getSuffix();                    // Additional suffixes like /P, /QRP, /MM, ...                          (generalSuffix)

    //      bool isValid();                         // True if it is a full callsign
    //      bool isValidPrefix();                   // True if it is a prefix, but not a call



    //KB1/EA4K/QRP
    QTest::addColumn<QString>("testString");            // KB1/EA4K/QRP
    QTest::addColumn<QString>("fullcallsign");              // KB1/EA4K/QRP
    QTest::addColumn<QString>("hostfullprefix");        // KB1
    QTest::addColumn<QString>("hostprefix");            // KB
    QTest::addColumn<int>("hostareanumber");            // 1

    QTest::addColumn<QString>("homecallsign");          // EA4K, 3D2A, 3D20A
    QTest::addColumn<QString>("homefullprefix");        // EA4, 3D2, 3D20
    QTest::addColumn<QString>("homeprefix");            // EA, 3D2, 3D2
    QTest::addColumn<int>("homeareanumber");            // 4, -1, 0
    QTest::addColumn<QString>("homesuffix");            // K, A, A
    QTest::addColumn<QString>("suffix");                // QRP

    QTest::addColumn<bool>("isValidPrefix");            // true
    QTest::addColumn<bool>("isValid");                  // true

    //                              testString          fullcall            hostfullpre hostpre hostarea  homecal     fullhomep homep      homeAr  homesuf  suff                                                                                                   isprefixValid   isValid

    QTest::newRow("KB1/EA4K/QRP")   << "KB1/EA4K/QRP"   << "KB1/EA4K/QRP"   << "KB1"    << "KB" << 1    << "EA4K"   << "EA4"    << "EA"     << 4    << "K"  << "QRP"    << true     << true;
    QTest::newRow("EA4K")           << "EA4K"           << "EA4K"           << ""       << ""   << -1   << "EA4K"   << "EA4"    << "EA"     << 4    << "K"  << ""       << true     << true;
    QTest::newRow("EA4K/P")         << "EA4K/P"         << "EA4K/P"         << ""       << ""   << -1   << "EA4K"   << "EA4"    << "EA"     << 4    << "K"  << "P"      << true     << true;
    QTest::newRow("9M6/EA0AA")      << "9M6/EA0AA"      << "9M6/EA0AA"      << "9M6"    << "9M" << 6    << "EA0AA"  << "EA0"    << "EA"     << 0    << "AA" << ""       << true     << true;
    QTest::newRow("M6/EA0AA")       << "M6/EA0AA"       << "M6/EA0AA"       << "M6"     << "M"  << 6    << "EA0AA"  << "EA0"    << "EA"     << 0    << "AA" << ""       << true     << true;
    QTest::newRow("M/EA0AA")        << "M/EA0AA"        << "M/EA0AA"        << "M"      << "M"  << -1   << "EA0AA"  << "EA0"    << "EA"     << 0    << "AA" << ""       << true     << true;
    QTest::newRow("3D2/EA0AA")      << "3D2/EA0AA"      << "3D2/EA0AA"      << "3D2"    << "3D2"<< -1   << "EA0AA"  << "EA0"    << "EA"     << 0    << "AA" << ""       << true     << true;
    QTest::newRow("3D20/EA0AA")     << "3D20/EA0AA"     << "3D20/EA0AA"     << "3D20"   << "3D2"<< 0    << "EA0AA"  << "EA0"    << "EA"     << 0    << "AA" << ""       << true     << true;
    QTest::newRow("2E4K")           << "2E4K"           << "2E4K"           << ""       << ""   << -1   << "2E4K"   << "2E4"    << "2E"     << 4    << "K"  << ""       << true     << true;
    QTest::newRow("3D20CR")         << "3D20CR"         << "3D20CR"         << ""       << ""   << -1   << "3D20CR" << "3D20"   << "3D2"    << 0    << "CR" << ""       << true     << true;
    QTest::newRow("3D2A")           << "3D2A"           << "3D2A"           << ""       << ""   << -1   << "3D2A"   << "3D2"    << "3D2"    << -1   << "A"  << ""       << true     << true;
    QTest::newRow("S50K")           << "S50K"           << "S50K"           << ""       << ""   << -1   << "S50K"   << "S50"    << "S5"     << 0    << "K"  << ""       << true     << true;
    QTest::newRow("AM100")          << "AM100"          << ""               << ""       << ""   << -1   << ""       << "AM100"  << "AM"     << 100  << ""   << ""       << true     << false;
    QTest::newRow("K1")             << "K1"             << ""               << ""       << ""   << -1   << ""       << "K1"     << "K"      << 1    << ""   << ""       << true     << false;
    QTest::newRow("KB1")            << "KB1"            << ""               << ""       << ""   << -1   << ""       << "KB1"    << "KB"     << 1    << ""   << ""       << true     << false;
    QTest::newRow("EA")             << "EA"             << ""               << ""       << ""   << -1   << ""       << "EA"     << "EA"     << -1   << ""   << ""       << true     << false;
    QTest::newRow("2E3")            << "2E3"            << ""               << ""       << ""   << -1   << ""       << "2E3"    << "2E"     << 3    << ""   << ""       << true     << false;
    QTest::newRow("E74")            << "E74"            << ""               << ""       << ""   << -1   << ""       << "E74"    << "E7"     << 4    << ""   << ""       << true     << false;
    QTest::newRow("A2")             << "A2"             << ""               << ""       << ""   << -1   << ""       << "A2"     << "A2"     << -1   << ""   << ""       << true     << false;
    //QTest::newRow("3D2")            << "3D2"            << ""               << ""       << ""   << -1   << ""       << "3D2"    << "3D2" << ""    << -1   << ""   << ""       << true     << false;

    // 3D20CR, 3D2C, 3D2NV/P, 3D3HY/R, 3V8ST/J, 4J75T/FF, UF/UA6GG/FF
    // 5Z4RT/Y2K, CO7WT/6Y
    // BY1BJ/1DX
    // VE3FY/BY1TTY (China), BG1LLB/0, B7D
    // XR9A/8
    // XR96IARU, R1AND/A, VP7/UT1KY, VU2JBK/AT10
    // CY2NJ/ND, CS5E/LH, CX2ANP/D
    // EA4K/6
    // AO150ITU/8
    // FR/F6KDF/T
    // M2001Y/71B, 2IONGM/NHS
    // W0S, N6J
    // VP6UU/VP6D (VP6/d)
    // mount Athos SV2A

    // FAIL: E/EA0K, E0J, KKK1J
}

void tst_Callsign::test_callsigns()
{
    QFETCH(QString, testString);
    QFETCH(QString, fullcallsign);
    QFETCH(QString, hostfullprefix);
    QFETCH(QString, hostprefix);
    QFETCH(int, hostareanumber);
    QFETCH(QString, homecallsign);
    QFETCH(QString, homefullprefix);
    QFETCH(QString, homeprefix);
    QFETCH(int, homeareanumber);
    QFETCH(QString, homesuffix);
    QFETCH(QString, suffix);
    QFETCH(bool, isValid);

    Callsign testCall(testString);

    qDebug() << Q_FUNC_INFO << " -         fullcall       : "  << fullcallsign;

    qDebug() << Q_FUNC_INFO << " - isValid";
    QCOMPARE(testCall.isValid(), isValid);

    if (isValid)
    {
        qDebug() << Q_FUNC_INFO << " - fullcallsign     : "      << testCall.getCallsign() << "/" << fullcallsign;
        QCOMPARE(testCall.getCallsign(), testString);

        qDebug() << Q_FUNC_INFO << " - hostfullprefix   :"  << testCall.getHostFullPrefix() << "/" << hostfullprefix;
        QCOMPARE(testCall.getHostFullPrefix(), hostfullprefix);

        qDebug() << Q_FUNC_INFO << " - hostprefix       :"      << testCall.getHostPrefix() << "/" << hostprefix;
        QCOMPARE(testCall.getHostPrefix(), hostprefix);

        qDebug() << Q_FUNC_INFO << " - hostareanumber   :"  << QString::number(testCall.getHostAreaNumber()) << "/" << QString::number(hostareanumber);
        QCOMPARE(testCall.getHostAreaNumber(), hostareanumber);

        qDebug() << Q_FUNC_INFO << " - homecall         :"      << testCall.getHomeCallsign() << "/" << homecallsign;
        QCOMPARE(testCall.getHomeCallsign(), homecallsign);

        qDebug() << Q_FUNC_INFO << " - homefullprefix   :"      << testCall.getHomeFullPrefix() << "/" << homefullprefix;
        QCOMPARE(testCall.getHomeFullPrefix(), homefullprefix);

        qDebug() << Q_FUNC_INFO << " - homeprefix       :"      << testCall.getHomePrefix() << "/" << homeprefix;
        QCOMPARE(testCall.getHomePrefix(), homeprefix);

        qDebug() << Q_FUNC_INFO << " - homeareanumber   :"  << QString::number(testCall.getHomeAreaNumber()) << "/" << QString::number(homeareanumber);
        QCOMPARE(testCall.getHomeAreaNumber(), homeareanumber);

        qDebug() << Q_FUNC_INFO << " - homesuffix       :"      << testCall.getHomeSuffix() << "/" << homesuffix;
        QCOMPARE(testCall.getHomeSuffix(), homesuffix);

        qDebug() << Q_FUNC_INFO << " - suffix           :"      << testCall.getSuffix() << "/" << suffix;
        QCOMPARE(testCall.getSuffix(), suffix);
    }
}



QTEST_APPLESS_MAIN(tst_Callsign)

#include "tst_callsign.moc"

