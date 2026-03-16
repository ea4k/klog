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
    // Regression test for #903: lognumber must not be -1 after ADIF import
    void test_ADIF_Import_setsCorrectLogNumber();
    void test_ADIF_Import_invalidLogN_returnsError();

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

    // Remove the test ADIF file
    if (!adifFilePath.isEmpty() && QFile::exists(adifFilePath))
        QFile::remove(adifFilePath);

    // Remove test DB
    if (QFile::exists(realDbPath))
        QFile::remove(realDbPath);

    // Restore real user DB if it was backed up
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
    // One complete QSO record with all mandatory fields
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

// Regression test for GitHub issue #903:
// After ADIF import, every imported QSO must have lognumber == logN (not -1).
void tst_FileManager::test_ADIF_Import_setsCorrectLogNumber()
{
    QVERIFY2(!adifFilePath.isEmpty(), "ADIF file path is empty");
    QVERIFY2(QFile::exists(adifFilePath), "ADIF test file does not exist");

    int qsosBefore = dataProxy->getHowManyQSOInLog(1);

    // Import into log #1
    int imported = fileManager->adifReadLog(adifFilePath, "EA4K", 1);
    QVERIFY2(imported > 0, qPrintable(QString("adifReadLog returned %1, expected > 0").arg(imported)));

    int qsosAfter = dataProxy->getHowManyQSOInLog(1);
    QVERIFY2(qsosAfter > qsosBefore, "No QSOs were added to log #1 after import");

    // Verify the imported QSO has the correct lognumber — regression for #903
    int lastId = dataProxy->getLastInsertedQSO();
    QVERIFY2(lastId > 0, "Could not get last inserted QSO id");

    QSO importedQso = dataProxy->fromDB(lastId);
    QVERIFY2(importedQso.getLogId() == 1,
             qPrintable(QString("Imported QSO lognumber is %1, expected 1 (regression #903)")
                        .arg(importedQso.getLogId())));
}

// Regression test for GitHub issue #903:
// Calling adifReadLog with an invalid (<=0) log number must fail immediately
// without inserting any QSO with lognumber=-1 in the database.
void tst_FileManager::test_ADIF_Import_invalidLogN_returnsError()
{
    QVERIFY2(!adifFilePath.isEmpty(), "ADIF file path is empty");

    int qsosBefore = dataProxy->getHowManyQSOInLog(1);

    // logN=-1 (the default) must be rejected
    int result = fileManager->adifReadLog(adifFilePath, "EA4K", -1);
    QVERIFY2(result < 0,
             qPrintable(QString("adifReadLog with logN=-1 should return error, got %1").arg(result)));

    // logN=0 must also be rejected
    result = fileManager->adifReadLog(adifFilePath, "EA4K", 0);
    QVERIFY2(result < 0,
             qPrintable(QString("adifReadLog with logN=0 should return error, got %1").arg(result)));

    // No QSOs should have been added to the database
    int qsosAfter = dataProxy->getHowManyQSOInLog(1);
    QVERIFY2(qsosAfter == qsosBefore,
             "QSOs were unexpectedly inserted when logN was invalid (regression #903)");
}


QTEST_MAIN(tst_FileManager)

#include "tst_filemanager.moc"
