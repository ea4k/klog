/***************************************************************************
                          tst_dataproxy.h  -  description
                             -------------------
    begin                : Mar 2021
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
#include "../../src/database/datacache.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/utilities.h"
#include "../../src/qso.h"
/*
    initTestCase() will be called before the first test function is executed.
    initTestCase_data() will be called to create a global test data table.
    cleanupTestCase() will be called after the last test function was executed.
    init() will be called before each test function is executed.
    cleanup() will be called after every test function.
*/
class tst_DataProxy : public QObject
{
    Q_OBJECT

public:
    tst_DataProxy();
    ~tst_DataProxy();

private slots:
    //void initTestCase();        // will be called before the first test function is executed.
    //void initTestCase_data();   // will be called to create a global test data table.
    //void cleanupTestCase();     // will be called after the last test function was executed.
    //void init();                // will be called before each test function is executed.
    //void cleanup();             // will be called after every test function.
    void test_Constructor();
    void test_modes_data();
    void test_modes();
    void test_bands();
    //void test_EntityAndPrefixes();  // getEntityMainPrefix
    void test_continents();

    void test_subdivisions_data();
    void test_primarySubdivisions();
    void test_qsosCache();


    //void test_getProgresStepForDialog();

private:
  DataProxy_SQLite *dataProxy;
  Utilities *util;
};

tst_DataProxy::tst_DataProxy()
{
    util = new Utilities(Q_FUNC_INFO);
    QString version = "1.5";
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, version);
}

tst_DataProxy::~tst_DataProxy()
{
    // qDebug() << Q_FUNC_INFO;
    delete (util);
    delete (dataProxy);
}

//void tst_DataProxy::initTestCase()
//{
    // qDebug() << Q_FUNC_INFO;
//}

/*
void tst_DataProxy::cleanupTestCase()
{
    // qDebug() << Q_FUNC_INFO;
}

void tst_DataProxy::init()
{
    // qDebug() << Q_FUNC_INFO;
}

void tst_DataProxy::cleanup()
{
    // qDebug() << Q_FUNC_INFO;
}
*/

void tst_DataProxy::test_Constructor()
{
    QVERIFY2(util->getVersion() == "0.0", "Version failed");
    // qDebug() << Q_FUNC_INFO << ": " << dataProxy->getSoftVersion ();
    // qDebug() << Q_FUNC_INFO << ": " << dataProxy->getDBVersion ();
}

/*
void tst_DataProxy::test_modes()
{
    QVERIFY2(dataProxy->getIdFromModeName("FT8") == dataProxy->getIdFromModeName ("FT8"), "FT8 mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("Q65") == "MFSK", "Q65 mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("FT8") == "FT8", "FT8 mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("USB") == "SSB", "USB mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("LSB") == "SSB", "USB mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("JT9C") == "JT9", "JT9C mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("CW") == "CW", "CW mode/Submode failed");
    QVERIFY2(dataProxy->getSubModeFromId (dataProxy->getIdFromModeName ("FT8")) == "FT8", "Submode from Id failed");
}
*/

void tst_DataProxy::test_subdivisions_data()
{
    //struct PrimarySubdivision is defined in klogdefinitions.h
    // Data is in database/db_adif_primary_subdvisions_data.cpp

    /*
    QTest::addColumn<QString>("string");
    QTest::addColumn<int>("dxcc");
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("shortname");
    QTest::addColumn<int>("cqz");
    QTest::addColumn<int>("ituz");


    QTest::newRow("EA8-GC") << 29 << "EA8" << "Las Palmas" << "GC" << "33" << "36";
    QTest::newRow("EA8-TF") << 29 << "EA8" << "Tenerife" << "TF" << "33" << "36";
    QTest::newRow("EA6-IB") << 21 << "EA6" << "Baleares" << "IB" << "14" << "37";
    */
}

void tst_DataProxy::test_modes_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("FT8") << "FT8" << "FT8";
    QTest::newRow("Q65") << "Q65" << "MFSK";
    QTest::newRow("USB") << "USB" << "SSB";
    QTest::newRow("LSB") << "LSB" << "SSB";
    QTest::newRow("JT9C") << "JT9C" << "JT9";
    QTest::newRow("CW") << "CW" << "CW";
}

void tst_DataProxy::test_modes()
{
        QFETCH(QString, string);
        QFETCH(QString, result);
        QCOMPARE(dataProxy->getNameFromSubMode(string), result);
}

void tst_DataProxy::test_bands()
{
    QVERIFY2(dataProxy->getBandNameFromFreq (1.81) == "160M", "160M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (3.71) == "80M", "80M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (5.2) == "60M", "60M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (7.123) == "40M", "40M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (10.1) == "30M", "30M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (14.195) == "20M", "20M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (18.100) == "17M", "17M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (21.195) == "15M", "15M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (24.900) == "12M", "12M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (28.500) == "10M", "10M band failed");
    // qVERIFY2(dataProxy->getBandNameFromFreq (41) == "8M", "8M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (52.1) == "6M", "6M band failed");
    // qVERIFY2(dataProxy->getBandNameFromFreq (57) == "5M", "5M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (70.3) == "4M", "4M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (144.3) == "2M", "2M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (432.1) == "70CM", "70CM band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (1296) == "23CM", "23CM band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (2400) == "13CM", "13CM band failed");

    QVERIFY2(dataProxy->isHF (dataProxy->getBandIdFromFreq (28.500)), "HF not identified");
    QVERIFY2(dataProxy->isHF (dataProxy->getBandIdFromFreq (3.500)), "HF not identified");
    QVERIFY2(!dataProxy->isHF (dataProxy->getBandIdFromFreq (144.500)), "HF not identified");
    QVERIFY2(dataProxy->isVHF (dataProxy->getBandIdFromFreq (144.500)), "VHF not identified");
    QVERIFY2(!dataProxy->isVHF (dataProxy->getBandIdFromFreq (14.300)), "VHF not identified");
    QVERIFY2(dataProxy->isVHF (dataProxy->getBandIdFromFreq (432.500)), "VHF not identified");

    QVERIFY2(dataProxy->isWARC (dataProxy->getBandIdFromFreq (18.100)), "WARC not identified");
    QVERIFY2(dataProxy->isWARC (dataProxy->getBandIdFromFreq (24.900)), "WARC not identified");
    QVERIFY2(dataProxy->isWARC (dataProxy->getBandIdFromFreq (10.100)), "WARC not identified");
    QVERIFY2(!dataProxy->isWARC (dataProxy->getBandIdFromFreq (432.500)), "WARC not identified");

    QVERIFY2(dataProxy->isUHF (dataProxy->getBandIdFromFreq (432.300)), "UHF not identified");
    QVERIFY2(!dataProxy->isUHF (dataProxy->getBandIdFromFreq (14.300)), "UHF not identified");

    Frequency f1(3.775);
    QVERIFY2(dataProxy->isThisFreqInBand("80M", f1), "Freq in band failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("20M", f1), "Freq in band failed");

    QVERIFY2(dataProxy->getNameFromBandId (dataProxy->getIdFromBandName ("20M")) == "20M", "Band names and Id failed");
}

void tst_DataProxy::test_qsosCache()
{
    // ==========================================
    // PREPARATION (SETUP)
    // ==========================================
    dataProxy->clearDuplicateCache(); // Clear the cache

    QSO qso;
    qso.setCall("EA4K");
    qso.setBand("20M");
    qso.setMode("SSB");

    QDateTime qsoTime(QDate(2025, 5, 20), QTime(14, 30, 0));
    qso.setTimeOn(qsoTime.time());
    qso.setDate(qsoTime.date());

    int testQsoId = 999;
    int bandId = dataProxy->getIdFromBandName("20M");
    int modeId = dataProxy->getIdFromModeName("SSB");
    int margin = 600; // 10 minutes margin for searchs


    // ==========================================
    // ADD & CHECK
    // ==========================================
    // Chec before we add to test it is not existinf
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime, bandId, modeId, margin), -1);

    // Add the QSO
    dataProxy->addDuplicateCache(testQsoId, qso);

    // Search the QSO after adding it and check the ID
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime, bandId, modeId, margin), testQsoId);


    // ==========================================
    // LIMIT VALIDATION & ERRORS
    // ==========================================
    // Wrong call
    QCOMPARE(dataProxy->findDuplicateId("EA4ZZ", qsoTime, bandId, modeId, margin), -1);

    // wrong band
    int wrongBandId = dataProxy->getIdFromBandName("40M");
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime, wrongBandId, modeId, margin), -1);

    // Works in the margin
    QDateTime timeInsideMargin = qsoTime.addSecs(-300);
    QCOMPARE(dataProxy->findDuplicateId("EA4K", timeInsideMargin, bandId, modeId, margin), testQsoId);

    // Fails out of the marging
    QDateTime timeOutsideMargin = qsoTime.addSecs(margin + 60);
    QCOMPARE(dataProxy->findDuplicateId("EA4K", timeOutsideMargin, bandId, modeId, margin), -1);

    // Test the wrapper
    QCOMPARE(dataProxy->findDuplicateId(qso, margin), -1);

    // ==========================================
    // REMOVE
    // ==========================================
    // Remove using the ID
    dataProxy->removeDuplicateCache(testQsoId);

    // Check that is not existing anymore
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime, bandId, modeId, margin), -1);

}

void tst_DataProxy::test_continents()
{// TODO: Can't test subdivisions without the World creation. Find a way to fill the DB without having to
    // to include World or simply test it from World
    /*
    // qDebug() << Q_FUNC_INFO << ": Shortname 281: " << dataProxy->getContinentShortNameFromEntity (281);
    QVERIFY2(dataProxy->getContinentShortNameFromEntity (281) == "EU", "Continent for Spain (dxcc=281) failed");
    QVERIFY2(dataProxy->getContinentShortNameFromEntity (1) == "NA", "Continent for Canada (dxcc=1) failed");
    QVERIFY2(dataProxy->getContinentShortNameFromEntity (100) == "SA", "Continent for Argentina (dxcc=100) failed");
    QVERIFY2(dataProxy->getContinentShortNameFromEntity (318) == "AS", "Continent for China (dxcc=318) failed");
    QVERIFY2(dataProxy->getContinentShortNameFromEntity (150) == "OC", "Continent for Australia (dxcc=150) failed");
    QVERIFY2(dataProxy->getContinentShortNameFromEntity (483) == "AF", "Continent for Togo (dxcc=483) failed");
    // qVERIFY2(dataProxy->getContinentShortNameFromEntity (13) == "AN", "Continent for Antartica (dxcc=13) failed");
    */
}

void tst_DataProxy::test_primarySubdivisions()
{ // TODO: Can't test subdivisions without the World creation. Find a way to fill the DB without having to
  // to include World or simply test it from World
/*
    QList<PrimarySubdivision> subdivisions;
    subdivisions.clear();
    // Testing that the functions are working
    subdivisions.append(dataProxy->getPrimarySubDivisions(281, "EA4"));

    QVERIFY2(subdivisions.first().name == "Badajoz", "Primary Subdivision first name failed (EA4)");
    QVERIFY2(subdivisions.first().shortName == "BA", "Primary Subdivision first shortname failed (EA4)");
    QVERIFY2(subdivisions.first().cqz == 14, "Primary Subdivision first cqz failed (EA4)");
    QVERIFY2(subdivisions.first().ituz == 37, "Primary Subdivision first ituz failed (EA4)");

    QVERIFY2(subdivisions.last().name == "Toledo", "Primary Subdivision last name failed (EA4)");
    QVERIFY2(subdivisions.last().shortName == "TO", "Primary Subdivision last shortname failed (EA4)");
    QVERIFY2(subdivisions.last().cqz == 14, "Primary Subdivision last cqz failed (EA4)");
    QVERIFY2(subdivisions.last().ituz == 37, "Primary Subdivision last  ituz failed (EA4)");

    // Testing that all the divisions have the proper numer

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(6, QString()));
    QVERIFY2(subdivisions.count() == 1, "Primary Subdivision number failed (6 - Alaska)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(1, QString()));
    QVERIFY2(subdivisions.count() == 20, "Primary Subdivision number failed (1 - Canada)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(281, QString()));
    QVERIFY2(subdivisions.count() == 47, "Primary Subdivision number failed (281 - Spain)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(50, QString()));
    QVERIFY2(subdivisions.count() == 32, "Primary Subdivision number failed (50 - Mexico)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(100, QString()));
    QVERIFY2(subdivisions.count() == 24, "Primary Subdivision number failed (100 - Argentina)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(108, QString()));
    QVERIFY2(subdivisions.count() == 27, "Primary Subdivision number failed (108 - Brazil)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(272, QString()));
    QVERIFY2(subdivisions.count() == 18, "Primary Subdivision number failed (272 - Portugal)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(291, QString()));
    QVERIFY2(subdivisions.count() == 61, "Primary Subdivision number failed (291 - United States)");

    subdivisions.clear();
    subdivisions.append(dataProxy->getPrimarySubDivisions(339, QString()));
    QVERIFY2(subdivisions.count() == 47, "Primary Subdivision number failed (339 - Japan)");

    //PrimarySubdivision subdivision;
    //foreach(subdivision, subdivisions)
    //{
    //  // qDebug() << Q_FUNC_INFO << ": " << subdivision.name;
    //}
   //getPrimarySubDivisions(currentInt, prefUsed)
*/

}
QTEST_GUILESS_MAIN(tst_DataProxy)

#include "tst_dataproxy.moc"

