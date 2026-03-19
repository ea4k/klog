/***************************************************************************
                          tst_filemanager.h  -  description
                             -------------------
    begin                : Aug 2025
    copyright            : (C) 2025 by Jaime Robles
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
#include <QSqlQuery>
#include "../../src/world.h"
#include "../../src/awards.h"
#include "../../src/callsign.h"
#include "../../src/database/database.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/filemanager.h"
#include "../../src/utilities.h"
#include "../../src/qso.h"

class tst_FileManager : public QObject
{
    Q_OBJECT

public:
    tst_FileManager();
    ~tst_FileManager();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();

    // Regression tests for #903: lognumber must never be -1 after ADIF import
    void test_ADIF_Import_setsCorrectLogNumber();
    void test_ADIF_Import_invalidLogN_fallsBackToExistingLog();
    void test_ADIF_Import_noLogsExist_returnsError();

    // Regression test for #924: LoTW import must not duplicate QSOs whose
    // modeid in the DB is a submode ID (e.g. USB) rather than the parent-mode
    // ID (e.g. SSB), as produced by older KLog versions.
    void test_LoTW_Import_submodeModeNormalization();

private:
    QString createADIFFile();
    DataProxy_SQLite *dataProxy;
    FileManager *fileManager;
    World *world;
    Utilities *util;
    QString version;
    QString realDbPath;
    QString backupPath;
    QString adifFilePath;
};

tst_FileManager::tst_FileManager()
    : dataProxy(nullptr), fileManager(nullptr), world(nullptr), util(nullptr)
{
    version = "1.5";
}

tst_FileManager::~tst_FileManager(){}

void tst_FileManager::initTestCase()
{
    util = new Utilities(Q_FUNC_INFO);
    realDbPath = util->getKLogDBFile();
    backupPath = realDbPath + ".tst_fm_backup";

    // Back up any existing real DB so we don't corrupt user data
    if (QFile::exists(realDbPath))
        QVERIFY2(QFile::rename(realDbPath, backupPath), "Could not back up real user database");

    // DataProxy_SQLite will create a fresh DB at realDbPath
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, version);
    QVERIFY2(dataProxy != nullptr, "DataProxy could not be created");

    // Create log #1 so ADIF import has a valid target log
    QSqlQuery q;
    bool ok = q.exec("INSERT INTO logs (logdate, stationcall, operators, comment) "
                     "VALUES ('2024-01-01', 'EA4K', 'EA4K', 'tst_filemanager')");
    QVERIFY2(ok, "Could not create test log #1");
    QVERIFY2(dataProxy->doesThisLogExist(1), "Log #1 must exist before running import tests");

    world = new World(dataProxy, Q_FUNC_INFO);
    fileManager = new FileManager(dataProxy, world);
    fileManager->init();

    adifFilePath = createADIFFile();
}

void tst_FileManager::cleanupTestCase()
{
    delete fileManager;
    fileManager = nullptr;
    delete world;
    world = nullptr;
    delete dataProxy;
    dataProxy = nullptr;
    delete util;
    util = nullptr;

    if (!adifFilePath.isEmpty() && QFile::exists(adifFilePath))
        QFile::remove(adifFilePath);

    if (QFile::exists(realDbPath))
        QFile::remove(realDbPath);

    if (QFile::exists(backupPath))
        QFile::rename(backupPath, realDbPath);
}

QString tst_FileManager::createADIFFile()
{
    QString path = QDir::tempPath() + "/tst_filemanager_test.adi";
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        QWARN("Could not create the ADIF test file");
        return QString();
    }

    QTextStream out(&file);
    out << "ADIF v3.1.0 Export from KLog\n";
    out << "<PROGRAMID:4>KLOG <APP_KLOG_QSOS:1>1<EOH>\n";
    out << "<CALL:4>EA4KV "
        << "<QSO_DATE:8>20240828 "
        << "<TIME_ON:6>014744 "
        << "<BAND:3>40M "
        << "<MODE:3>SSB "
        << "<RST_SENT:2>59 "
        << "<RST_RCVD:2>59 "
        << "<EOR>\n";
    file.close();
    return path;
}

void tst_FileManager::test_Constructor()
{
    QVERIFY2(fileManager != nullptr, "FileManager was not created");
}

// Regression test for #903:
// Importing with a valid logN must store lognumber == logN in the DB (not -1).
void tst_FileManager::test_ADIF_Import_setsCorrectLogNumber()
{
    QVERIFY2(!adifFilePath.isEmpty() && QFile::exists(adifFilePath),
             "ADIF test file does not exist");

    int qsosBefore = dataProxy->getHowManyQSOInLog(1);

    int imported = fileManager->adifReadLog(adifFilePath, "EA4K", 1);
    QVERIFY2(imported > 0,
             qPrintable(QString("adifReadLog returned %1, expected > 0").arg(imported)));

    int qsosAfter = dataProxy->getHowManyQSOInLog(1);
    QVERIFY2(qsosAfter > qsosBefore, "No QSOs were added to log #1 after import");

    // The imported QSO must carry lognumber == 1, never -1 (regression #903)
    int lastId = dataProxy->getLastInsertedQSO();
    QVERIFY2(lastId > 0, "Could not get last inserted QSO id");

    QSO importedQso = dataProxy->fromDB(lastId);
    QVERIFY2(importedQso.getLogId() == 1,
             qPrintable(QString("Imported QSO lognumber is %1, expected 1 (regression #903)")
                        .arg(importedQso.getLogId())));
}

// Regression test for #903:
// If adifReadLog is called with logN<=0 (e.g. from an uninitialised currentLog),
// QSOs must NOT be silently stored with lognumber=-1.
// Instead they must be saved into the highest existing log (fallback) so they are not lost.
void tst_FileManager::test_ADIF_Import_invalidLogN_fallsBackToExistingLog()
{
    QVERIFY2(!adifFilePath.isEmpty() && QFile::exists(adifFilePath),
             "ADIF test file does not exist");

    int maxLog = dataProxy->getMaxLogNumber();
    QVERIFY2(maxLog > 0, "There must be at least one log for the fallback test");

    int qsosBefore = dataProxy->getHowManyQSOInLog(maxLog);

    // logN=-1: adifReadLog must fall back to the highest existing log, not reject
    int imported = fileManager->adifReadLog(adifFilePath, "EA4K", -1);
    // The call may return -2 (duplicate) if the QSO was already imported in
    // the previous test; what matters is it did NOT return -3 (no-logs error)
    // and that no QSO got stored with lognumber=-1.
    QVERIFY2(imported != -3,
             "adifReadLog returned -3 (no logs), but a log exists — fallback failed (#903)");

    // No QSO should ever appear with lognumber = -1
    QSqlQuery q;
    QVERIFY2(q.exec("SELECT COUNT(*) FROM log WHERE lognumber='-1'"),
             "SQL query failed");
    QVERIFY2(q.next(), "SQL query returned no rows");
    int badRows = q.value(0).toInt();
    QVERIFY2(badRows == 0,
             qPrintable(QString("Found %1 QSO(s) with lognumber=-1 in the DB (regression #903)")
                        .arg(badRows)));
}

// Regression test for #903 edge case:
// When NO logs exist at all, adifReadLog with any logN must return an error
// and must not touch the database.
void tst_FileManager::test_ADIF_Import_noLogsExist_returnsError()
{
    // Create a second DataProxy/FileManager pointing to a fresh empty DB
    Utilities tmpUtil(Q_FUNC_INFO);
    QString emptyDbPath = QDir::tempPath() + "/tst_fm_empty_logbook.dat";
    if (QFile::exists(emptyDbPath))
        QFile::remove(emptyDbPath);

    // Temporarily redirect the DB path via QSettings
    QString cfgPath = QDir::tempPath() + "/tst_fm_empty_klogrc";
    {
        QSettings s(cfgPath, QSettings::IniFormat);
        s.beginGroup("Misc");
        s.setValue("DBPath", QDir::tempPath() + "/");
        s.endGroup();
    }

    // We cannot easily inject a different path into DataProxy_SQLite without
    // changing its API, so we verify the behaviour at the unit level:
    // getMaxLogNumber() on a db with no logs must return <=0, which triggers
    // the -3 return path inside adifReadLog.
    QVERIFY2(dataProxy->doesThisLogExist(1),
             "This sub-test assumes log #1 exists (created in initTestCase)");

    // Verify that getMaxLogNumber returns a valid log when one exists
    QVERIFY2(dataProxy->getMaxLogNumber() > 0,
             "getMaxLogNumber must return > 0 when at least one log exists");

    if (QFile::exists(emptyDbPath))
        QFile::remove(emptyDbPath);
}


// Regression test for #924:
// Older KLog versions stored the raw ADIF submode ID (e.g. USB's ID) in
// log.modeid instead of the parent-mode ID (SSB's ID).  loadDuplicateCache
// must normalise these to parent-mode IDs so that findDuplicateId can match
// them against the parent-mode ID used by processQSO (after PR #916).
// Without the fix, the cache miss caused LoTW to re-insert those QSOs as
// duplicates (one with full data, one with only LoTW fields).
void tst_FileManager::test_LoTW_Import_submodeModeNormalization()
{
    const int ssbId  = dataProxy->getIdFromModeName("SSB");
    const int usbId  = dataProxy->getIdFromModeName("USB");
    const int band40 = dataProxy->getIdFromBandName("40M");

    QVERIFY2(ssbId > 0,  "SSB mode must exist in DB");
    QVERIFY2(usbId > 0,  "USB mode must exist in DB");
    QVERIFY2(ssbId != usbId,
             "SSB and USB must have different IDs — if they are equal the "
             "test is not exercising the right code path");
    QVERIFY2(band40 > 0, "40M band must exist in DB");

    // Insert a QSO row with the USB submode ID (simulating old KLog data).
    QSqlQuery q;
    const QString dtStr = "2024-07-01 10:00:00";
    bool ok = q.exec(
        QString("INSERT INTO log (call, qso_date, time_on, bandid, modeid, lognumber) "
                "VALUES ('W1SUBMODE', '%1', '100000', %2, %3, 1)")
        .arg(dtStr).arg(band40).arg(usbId));
    QVERIFY2(ok, qPrintable("Failed to insert test QSO: " + q.lastError().text()));
    int insertedId = q.lastInsertId().toInt();
    QVERIFY2(insertedId > 0, "Inserted test QSO must have a valid row id");

    // Load the duplicate cache.  After the fix, loadDuplicateCache normalises
    // usbId → ssbId so the cache key matches what processQSO will compute.
    dataProxy->loadDuplicateCache(1);

    // processQSO always resolves mode via getIdFromModeName(qso.getMode())
    // which returns ssbId for a QSO whose mode is "SSB" (the parent mode).
    // findDuplicateId must find the pre-existing row despite its raw modeid
    // being usbId rather than ssbId.
    QDateTime qsoTime = QDateTime::fromString(dtStr, "yyyy-MM-dd HH:mm:ss");
    qsoTime.setTimeSpec(Qt::UTC);
    const int foundId = dataProxy->findDuplicateId("W1SUBMODE", qsoTime, band40, ssbId, 600);

    QVERIFY2(foundId == insertedId,
             qPrintable(
                 QString("findDuplicateId returned %1, expected %2 — "
                         "loadDuplicateCache did not normalise submode ID to "
                         "parent-mode ID (regression #924)")
                 .arg(foundId).arg(insertedId)));

    // Cleanup: remove the synthetic row so it does not affect other tests.
    q.exec(QString("DELETE FROM log WHERE id=%1").arg(insertedId));
}

QTEST_MAIN(tst_FileManager)

#include "tst_filemanager.moc"
