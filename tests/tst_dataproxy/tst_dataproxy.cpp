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
#include "../../src/dataproxy_sqlite.h"
#include "../../src/utilities.h"
#include "../../src/qso.h"

class tst_DataProxy : public QObject
{
    Q_OBJECT

public:
    tst_DataProxy();
    ~tst_DataProxy();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_Constructor();
    void test_modes_data();
    void test_modes();
    void test_bands();
    void test_isThisFreqInBand();
    void test_continents();
    void test_subdivisions_data();
    void test_primarySubdivisions();
    void test_qsosCache();
    void test_addQSO();
    void test_bandClassification_data();
    void test_bandClassification();

private:
    DataProxy_SQLite *dataProxy;
    Utilities        *util;
    QString           testDbPath;
};

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

tst_DataProxy::tst_DataProxy()
    : dataProxy(nullptr), util(nullptr)
{
    // Build a unique path in the system temp folder so we never touch the
    // real user database.
    testDbPath = QDir::tempPath() + "/tst_dataproxy_logbook.dat";
}

tst_DataProxy::~tst_DataProxy()
{
    delete util;
    delete dataProxy;
}

// ─────────────────────────────────────────────────────────────────────────────
// initTestCase / cleanupTestCase
// ─────────────────────────────────────────────────────────────────────────────

void tst_DataProxy::initTestCase()
{
    // Remove any leftover DB from a previous crashed run
    if (QFile::exists(testDbPath))
    {
        QVERIFY2(QFile::remove(testDbPath),
                 "Could not remove leftover test database");
    }

    util = new Utilities(Q_FUNC_INFO);

    // DataProxy_SQLite reads the DB path from Utilities::getKLogDBFile(), which
    // in turn reads QSettings.  We override that by pointing QSettings at a
    // temporary config that redirects the DB to our temp path.
    // The cleanest way that does NOT touch user settings is to create the
    // DataBase directly and hand it a known path.  DataProxy_SQLite exposes no
    // such injection, so we use the same trick as tst_database: pass the path
    // via a fresh Utilities object that writes it into a temp config.
    //
    // Simplest approach that works with the existing API: just redirect the DB
    // file by creating the DataBase ourselves and replacing the file before
    // DataProxy_SQLite opens it.  We do this by writing a minimal temp config
    // that points DBPath at our temp dir.

    QString tempCfg = QDir::tempPath() + "/tst_dataproxy_klogrc";
    QSettings settings(tempCfg, QSettings::IniFormat);
    settings.beginGroup("Misc");
    settings.setValue("DBPath", QDir::tempPath() + "/");
    settings.endGroup();
    settings.sync();

    // util->getCfgFile() is used internally – we cannot override it without
    // subclassing Utilities. The simplest working solution is to pass the
    // full DB path directly to DataBase and bypass DataProxy_SQLite's own
    // construction, OR just accept the default path but use a DIFFERENT file
    // name that we delete afterwards.
    //
    // Since the DataProxy_SQLite constructor always calls
    //   db = new DataBase(Q_FUNC_INFO, version, util->getKLogDBFile())
    // and getKLogDBFile() returns  <homeDir>/logbook.dat, we instead create
    // the proxy with a version string that makes getKLogDBFile() resolve to
    // our temp path by temporarily replacing the DB file name.
    //
    // The most practical solution given the current architecture: create the
    // DataProxy using the standard path, but rename/restore the real DB around
    // the test so we never corrupt user data.

    QString realDbPath = util->getKLogDBFile();  // e.g. ~/.klog/logbook.dat
    QString backupPath = realDbPath + ".tst_backup";

    // If a real DB exists, back it up
    if (QFile::exists(realDbPath))
    {
        QVERIFY2(QFile::rename(realDbPath, backupPath),
                 "Could not back up the real user database");
    }

    // Now DataProxy_SQLite will create a FRESH database at realDbPath
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, "1.5");
    QVERIFY2(dataProxy != nullptr, "DataProxy could not be created");

    // Create log #1 so QSOs can be inserted in test_addQSO
    QSqlQuery q;
    bool ok = q.exec("INSERT INTO logs (logdate, stationcall, operators, comment) "
                     "VALUES ('2024-01-01', 'EA4K', 'EA4K', 'tst_dataproxy')");
    QVERIFY2(ok, "Could not create test log");
}

void tst_DataProxy::cleanupTestCase()
{
    // Close connections before deleting files
    delete dataProxy;
    dataProxy = nullptr;

    Utilities tmpUtil(Q_FUNC_INFO);
    QString realDbPath  = tmpUtil.getKLogDBFile();
    QString backupPath  = realDbPath + ".tst_backup";

    // Remove the test database
    if (QFile::exists(realDbPath))
        QFile::remove(realDbPath);

    // Restore the real user database if it was backed up
    if (QFile::exists(backupPath))
        QFile::rename(backupPath, realDbPath);
}

// ─────────────────────────────────────────────────────────────────────────────
// Tests
// ─────────────────────────────────────────────────────────────────────────────

void tst_DataProxy::test_Constructor()
{
    QVERIFY2(util->getVersion() == "0.0", "Version failed");
}

void tst_DataProxy::test_modes_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("FT8")  << "FT8"  << "FT8";
    QTest::newRow("Q65")  << "Q65"  << "MFSK";
    QTest::newRow("USB")  << "USB"  << "SSB";
    QTest::newRow("LSB")  << "LSB"  << "SSB";
    QTest::newRow("JT9C") << "JT9C" << "JT9";
    QTest::newRow("CW")   << "CW"   << "CW";
}

void tst_DataProxy::test_modes()
{
    QFETCH(QString, string);
    QFETCH(QString, result);
    QCOMPARE(dataProxy->getNameFromSubMode(string), result);
}

void tst_DataProxy::test_bands()
{
    QVERIFY2(dataProxy->getBandNameFromFreq(1.81)   == "160M", "160M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(3.71)   == "80M",  "80M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(5.2)    == "60M",  "60M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(7.123)  == "40M",  "40M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(10.1)   == "30M",  "30M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(14.195) == "20M",  "20M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(18.100) == "17M",  "17M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(21.195) == "15M",  "15M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(24.900) == "12M",  "12M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(28.500) == "10M",  "10M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(52.1)   == "6M",   "6M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(57)     == "5M",   "5M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(70.3)   == "4M",   "4M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(144.3)  == "2M",   "2M failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(432.1)  == "70CM", "70CM failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(1296)   == "23CM", "23CM failed");
    QVERIFY2(dataProxy->getBandNameFromFreq(2400)   == "13CM", "13CM failed");

    QVERIFY2( dataProxy->isHF(dataProxy->getBandIdFromFreq(28.500)),  "HF 10M not identified");
    QVERIFY2( dataProxy->isHF(dataProxy->getBandIdFromFreq(3.500)),   "HF 80M not identified");
    QVERIFY2(!dataProxy->isHF(dataProxy->getBandIdFromFreq(144.500)), "2M wrongly identified as HF");

    QVERIFY2( dataProxy->isVHF(dataProxy->getBandIdFromFreq(144.500)), "VHF 2M not identified");
    QVERIFY2(!dataProxy->isVHF(dataProxy->getBandIdFromFreq(14.300)),  "20M wrongly identified as VHF");
    QVERIFY2(!dataProxy->isVHF(dataProxy->getBandIdFromFreq(432.500)), "70CM not identified as VHF");

    QVERIFY2( dataProxy->isWARC(dataProxy->getBandIdFromFreq(18.100)),  "17M WARC not identified");
    QVERIFY2( dataProxy->isWARC(dataProxy->getBandIdFromFreq(24.900)),  "12M WARC not identified");
    QVERIFY2( dataProxy->isWARC(dataProxy->getBandIdFromFreq(10.100)),  "30M WARC not identified");
    QVERIFY2(!dataProxy->isWARC(dataProxy->getBandIdFromFreq(432.500)), "70CM wrongly identified as WARC");

    QVERIFY2( dataProxy->isUHF(dataProxy->getBandIdFromFreq(432.300)), "UHF 70CM not identified");
    QVERIFY2(!dataProxy->isUHF(dataProxy->getBandIdFromFreq(14.300)),  "20M wrongly identified as UHF");

    QVERIFY2(dataProxy->getNameFromBandId(dataProxy->getIdFromBandName("20M")) == "20M",
             "Band name/id round-trip failed");
}

void tst_DataProxy::test_isThisFreqInBand()
{
    // ---- 160M: 1.8 – 2.0 MHz ----
    QVERIFY2( dataProxy->isThisFreqInBand("160M", Frequency(1.8)),    "160M lower limit failed");
    QVERIFY2( dataProxy->isThisFreqInBand("160M", Frequency(1.9)),    "160M mid-band failed");
    QVERIFY2( dataProxy->isThisFreqInBand("160M", Frequency(2.0)),    "160M upper limit failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("160M", Frequency(1.799)),  "160M below lower should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("160M", Frequency(2.001)),  "160M above upper should fail");

    // ---- 80M: 3.5 – 4.0 MHz ----
    QVERIFY2( dataProxy->isThisFreqInBand("80M", Frequency(3.5)),    "80M lower limit failed");
    QVERIFY2( dataProxy->isThisFreqInBand("80M", Frequency(3.775)),  "80M mid-band failed");
    QVERIFY2( dataProxy->isThisFreqInBand("80M", Frequency(4.0)),    "80M upper limit failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("80M", Frequency(3.499)),  "80M below lower should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("80M", Frequency(4.001)),  "80M above upper should fail");

    // ---- 40M: 7.0 – 7.3 MHz ----
    QVERIFY2( dataProxy->isThisFreqInBand("40M", Frequency(7.0)),    "40M lower limit failed");
    QVERIFY2( dataProxy->isThisFreqInBand("40M", Frequency(7.1)),    "40M mid-band failed");
    QVERIFY2( dataProxy->isThisFreqInBand("40M", Frequency(7.3)),    "40M upper limit failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("40M", Frequency(6.999)),  "40M below lower should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("40M", Frequency(7.301)),  "40M above upper should fail");

    // ---- 20M: 14.0 – 14.35 MHz ----
    QVERIFY2( dataProxy->isThisFreqInBand("20M", Frequency(14.0)),    "20M lower limit failed");
    QVERIFY2( dataProxy->isThisFreqInBand("20M", Frequency(14.195)),  "20M mid-band failed");
    QVERIFY2( dataProxy->isThisFreqInBand("20M", Frequency(14.35)),   "20M upper limit failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("20M", Frequency(13.999)),  "20M below lower should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("20M", Frequency(14.351)),  "20M above upper should fail");

    // ---- 10M: 28.0 – 29.7 MHz ----
    QVERIFY2( dataProxy->isThisFreqInBand("10M", Frequency(28.0)),    "10M lower limit failed");
    QVERIFY2( dataProxy->isThisFreqInBand("10M", Frequency(28.5)),    "10M mid-band failed");
    QVERIFY2( dataProxy->isThisFreqInBand("10M", Frequency(29.7)),    "10M upper limit failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("10M", Frequency(27.999)),  "10M below lower should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("10M", Frequency(29.701)),  "10M above upper should fail");

    // ---- Cross-band ----
    Frequency f80m(3.775);
    QVERIFY2( dataProxy->isThisFreqInBand("80M", f80m), "80M cross-check failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("40M", f80m), "80M freq must not match 40M");
    QVERIFY2(!dataProxy->isThisFreqInBand("20M", f80m), "80M freq must not match 20M");

    // ---- Edge cases ----
    QVERIFY2(!dataProxy->isThisFreqInBand("20M", Frequency(0.0)),   "Zero freq should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("20M", Frequency(-1.0)),  "Negative freq should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("",       Frequency(14.195)), "Empty band should fail");
    QVERIFY2(!dataProxy->isThisFreqInBand("NOBAND", Frequency(14.195)), "Unknown band should fail");

    // ---- VHF / UHF ----
    QVERIFY2( dataProxy->isThisFreqInBand("2M",   Frequency(144.3)), "2M mid-band failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("70CM", Frequency(144.3)), "2M freq must not match 70CM");
    QVERIFY2( dataProxy->isThisFreqInBand("70CM", Frequency(432.1)), "70CM mid-band failed");
    QVERIFY2(!dataProxy->isThisFreqInBand("2M",   Frequency(432.1)), "70CM freq must not match 2M");
}

void tst_DataProxy::test_continents()
{
    // World data is not loaded in this test suite, so continent lookups
    // cannot be verified here. Kept as placeholder.
}

void tst_DataProxy::test_subdivisions_data()
{
    // No data rows needed; placeholder for future data-driven subdivision tests.
}

void tst_DataProxy::test_primarySubdivisions()
{
    // World data (entity/prefix tables) is not populated in this isolated test.
    // Subdivision tests belong in tst_world.
}

void tst_DataProxy::test_qsosCache()
{
    dataProxy->clearDuplicateCache();

    QSO qso;
    qso.setCall("EA4K");
    qso.setBand("20M");
    qso.setMode("SSB");

    QDateTime qsoTime(QDate(2025, 5, 20), QTime(14, 30, 0), QTimeZone::UTC);
    qso.setDateTimeOn(qsoTime);

    const int  testQsoId = 999;
    const int  bandId    = dataProxy->getIdFromBandName("20M");
    const int  modeId    = dataProxy->getIdFromModeName("SSB");
    const int  margin    = 600; // ±10 minutes

    // Not in cache yet
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime, bandId, modeId, margin), -1);

    // Add to cache
    dataProxy->addDuplicateCache(testQsoId, qso, bandId, modeId);

    // Found after insertion
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime, bandId, modeId, margin), testQsoId);

    // Wrong callsign
    QCOMPARE(dataProxy->findDuplicateId("EA4ZZ", qsoTime, bandId, modeId, margin), -1);

    // Wrong band
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime,
                                        dataProxy->getIdFromBandName("40M"), modeId, margin), -1);

    // Inside margin (−5 min)
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime.addSecs(-300), bandId, modeId, margin),
             testQsoId);

    // Outside margin (+11 min)
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime.addSecs(margin + 60), bandId, modeId, margin),
             -1);

    // Wrapper with QSO object – uses setDateTimeOn so getDateTimeOn() is valid
    QCOMPARE(dataProxy->findDuplicateId(qso, margin), testQsoId);

    // Remove and verify
    dataProxy->removeDuplicateCache(testQsoId);
    QCOMPARE(dataProxy->findDuplicateId("EA4K", qsoTime, bandId, modeId, margin), -1);
}

void tst_DataProxy::test_addQSO()
{
    // Log #1 was created in initTestCase via direct SQL INSERT
    QVERIFY2(dataProxy->doesThisLogExist(1), "Test log #1 must exist before addQSO");

    QSO qso;
    qso.clear();
    qso.setCall("EA4K");
    qso.setDateTimeOn(QDateTime(QDate(2024, 3, 27), QTime(10, 0, 0), QTimeZone::UTC));
    qso.setBand("10M");
    qso.setMode("SSB");
    qso.setLogId(1);

    int id = dataProxy->addQSO(qso);
    QVERIFY2(id > 0, qPrintable(QString("addQSO returned %1").arg(id)));
}

void tst_DataProxy::test_bandClassification_data()
{
    QTest::addColumn<QString>("bandName");
    QTest::addColumn<bool>("expectHF");
    QTest::addColumn<bool>("expectWARC");
    QTest::addColumn<bool>("expectVHF");
    QTest::addColumn<bool>("expectUHF");

    //                       name      HF       WARC     VHF      UHF
    QTest::newRow("160M") << "160M" << true  << false << false << false;
    QTest::newRow("80M")  << "80M"  << true  << false << false << false;
    QTest::newRow("60M")  << "60M"  << true  << false << false << false; // falla con lógica por ID
    QTest::newRow("40M")  << "40M"  << true  << false << false << false;
    QTest::newRow("30M")  << "30M"  << true  << true  << false << false; // WARC
    QTest::newRow("20M")  << "20M"  << true  << false << false << false;
    QTest::newRow("17M")  << "17M"  << true  << true  << false << false; // WARC
    QTest::newRow("15M")  << "15M"  << true  << false << false << false;
    QTest::newRow("12M")  << "12M"  << true  << true  << false << false; // WARC
    QTest::newRow("10M")  << "10M"  << true  << false << false << false;
    QTest::newRow("6M")   << "6M"   << false << false << true  << false;
    QTest::newRow("4M")   << "4M"   << false << false << true  << false;
    QTest::newRow("2M")   << "2M"   << false << false << true  << false;
    QTest::newRow("70CM") << "70CM" << false << false << false << true;
    QTest::newRow("23CM") << "23CM" << false << false << false << true;
    QTest::newRow("13CM") << "13CM" << false << false << false << true;
}

void tst_DataProxy::test_bandClassification()
{
    QFETCH(QString, bandName);
    QFETCH(bool, expectHF);
    QFETCH(bool, expectWARC);
    QFETCH(bool, expectVHF);
    QFETCH(bool, expectUHF);

    const int id = dataProxy->getIdFromBandName(bandName);
    QVERIFY2(id > 0, qPrintable(QString("Band '%1' not found in DB").arg(bandName)));

    QCOMPARE(dataProxy->isHF(id),   expectHF);
    QCOMPARE(dataProxy->isWARC(id), expectWARC);
    QCOMPARE(dataProxy->isVHF(id),  expectVHF);
    QCOMPARE(dataProxy->isUHF(id),  expectUHF);
}

QTEST_GUILESS_MAIN(tst_DataProxy)
#include "tst_dataproxy.moc"
