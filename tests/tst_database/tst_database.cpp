/***************************************************************************
                          tst_database.h  -  description
                             -------------------
    begin                : Mar 2024
    copyright            : (C) 2024 by Jaime Robles
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
#include "../../src/database.h"
#include "../../src/utilities.h"
#include "../../src/world.h"
#include "../../src/qso.h"
#include "../../src/dataproxy_sqlite.h"
/*
    initTestCase() will be called before the first test function is executed.
    initTestCase_data() will be called to create a global test data table.
    cleanupTestCase() will be called after the last test function was executed.
    init() will be called before each test function is executed.
    cleanup() will be called after every test function.
*/

class tst_DataBase: public QObject
{
    Q_OBJECT

public:
    tst_DataBase();
    ~tst_DataBase();

private slots:
    void initTestCase();        // will be called before the first test function is executed.
    void test_Constructor();
    void test_CreateDB();
    void test_ExistingTables();
    void test_DataInTables();

    void test_checks();
    void test_addQSOs();
    //void initTestCase_data();   // will be called to create a global test data table.

    //void init();                // will be called before each test function is executed.
    //void cleanup();             // will be called after every test function.
    void cleanupTestCase();     // will be called after the last test function was executed.


    //void test_getProgresStepForDialog();

private:
  DataBase *db;
  Utilities *util;
  QString version;
};

tst_DataBase::tst_DataBase()
{
    version = "999";
}

tst_DataBase::~tst_DataBase()
{
    //qDebug() << Q_FUNC_INFO;
    delete (util);
    delete (db);
}

void tst_DataBase::initTestCase()
{
    qDebug() << Q_FUNC_INFO;
    Utilities util(Q_FUNC_INFO);
    QFile file(util.getKLogDBFile());
    if (file.exists())
        if (file.rename(util.getKLogDBFile() + "-test"))
            qDebug() << Q_FUNC_INFO << "DB deleted";

    qDebug() << Q_FUNC_INFO << ": " << util.getCfgFile();
    file.setFileName(util.getCfgFile());
    if (file.exists())
        if (file.rename(util.getCfgFile() +  "-test"))
            qDebug() << Q_FUNC_INFO << "Restoring the config file";
}

void tst_DataBase::test_ExistingTables()
{
    //DataBase db(Q_FUNC_INFO, version, util->getKLogDBFile());

    QCOMPARE(db->isTheTableExisting("ant_path_enumeration"), true);
    QCOMPARE(db->isTheTableExisting("arrl_sect_enumeration"), true);
    QCOMPARE(db->isTheTableExisting("award_enumeration"), true);
    QCOMPARE(db->isTheTableExisting("awarddxcc"), true);
    QCOMPARE(db->isTheTableExisting("awardwaz"), true);
    QCOMPARE(db->isTheTableExisting("band"), true);
    QCOMPARE(db->isTheTableExisting("clublog_status"), true);
    QCOMPARE(db->isTheTableExisting("contest"), true);
    QCOMPARE(db->isTheTableExisting("contestcatassisted"), true);
    QCOMPARE(db->isTheTableExisting("contestcatcategory"), true);
    QCOMPARE(db->isTheTableExisting("contestcatband"), true);
    QCOMPARE(db->isTheTableExisting("contestcatmode"), true);
    QCOMPARE(db->isTheTableExisting("contestcatoperator"), true);
    QCOMPARE(db->isTheTableExisting("contestcatoverlay"), true);
    QCOMPARE(db->isTheTableExisting("contestcatpower"), true);

    QCOMPARE(db->isTheTableExisting("continent"), true);
    QCOMPARE(db->isTheTableExisting("entity"), true);
    QCOMPARE(db->isTheTableExisting("log"), true);
    QCOMPARE(db->isTheTableExisting("logs"), true);
    QCOMPARE(db->isTheTableExisting("mode"), true);
    QCOMPARE(db->isTheTableExisting("prefixesofentity"), true);
    QCOMPARE(db->isTheTableExisting("primary_subdivisions"), true);
    QCOMPARE(db->isTheTableExisting("prop_mode_enumeration"), true);
    QCOMPARE(db->isTheTableExisting("qsl_rec_status"), true);
    QCOMPARE(db->isTheTableExisting("qsl_sent_status"), true);
    QCOMPARE(db->isTheTableExisting("qsl_via_enumeration"), true);
    QCOMPARE(db->isTheTableExisting("satellites"), true);
    QCOMPARE(db->isTheTableExisting("qso_complete_enumeration"), true);
    QCOMPARE(db->isTheTableExisting("softwarecontrol"), true);
    QCOMPARE(db->isTheTableExisting("supportedcontest"), true);
}

void tst_DataBase::test_DataInTables()
{
    //DataBase db(Q_FUNC_INFO, version, util->getKLogDBFile());

    QCOMPARE(db->hasTheTableData("ant_path_enumeration"), true);
    QCOMPARE(db->hasTheTableData("arrl_sect_enumeration"), true);
    QCOMPARE(db->hasTheTableData("award_enumeration"), true);
    QCOMPARE(db->hasTheTableData("awarddxcc"), false);
    QCOMPARE(db->hasTheTableData("awardwaz"), false);
    QCOMPARE(db->hasTheTableData("band"), true);
    QCOMPARE(db->hasTheTableData("clublog_status"), true);
    QCOMPARE(db->hasTheTableData("contest"), true);
    QCOMPARE(db->hasTheTableData("contestcatassisted"), true);
    QCOMPARE(db->hasTheTableData("contestcatcategory"), false);
    QCOMPARE(db->hasTheTableData("contestcatband"), true);
    QCOMPARE(db->hasTheTableData("contestcatmode"), true);
    QCOMPARE(db->hasTheTableData("contestcatoperator"), true);
    QCOMPARE(db->hasTheTableData("contestcatoverlay"), true);
    QCOMPARE(db->hasTheTableData("contestcatpower"), true);

    QCOMPARE(db->hasTheTableData("continent"), true);
    QCOMPARE(db->hasTheTableData("entity"), false);

    QCOMPARE(db->hasTheTableData("log"), false);
    QCOMPARE(db->hasTheTableData("logs"), false);
    QCOMPARE(db->hasTheTableData("mode"), true);
    QCOMPARE(db->hasTheTableData("prefixesofentity"), false);
    QCOMPARE(db->hasTheTableData("primary_subdivisions"), false);
    QCOMPARE(db->hasTheTableData("prop_mode_enumeration"), true);
    QCOMPARE(db->hasTheTableData("qsl_rec_status"), true);
    QCOMPARE(db->hasTheTableData("qsl_sent_status"), true);
    QCOMPARE(db->hasTheTableData("qsl_via_enumeration"), true);
    QCOMPARE(db->hasTheTableData("satellites"), true);
    QCOMPARE(db->hasTheTableData("qso_complete_enumeration"), true);
    QCOMPARE(db->hasTheTableData("softwarecontrol"), true);
    QCOMPARE(db->hasTheTableData("supportedcontest"), true);
}

void tst_DataBase::test_checks()
{
    //DataBase db(Q_FUNC_INFO, version, util->getKLogDBFile());
    QCOMPARE(db->isTheDBCreated(), true);
    QString aux;
    QStringList _bands = { "0", "1mm", "2mm", "2.5mm", "4mm", "6mm",
                "1.25CM", "3CM", "6CM", "9CM", "13CM", "23CM",
                "70CM", "1.25M", "2M", "4M", "6M", "8M", "10M",
                "12M", "15M", "17M", "20M", "30M", "40M", "60M",
                "80M", "160M", "560M", "630M", "2190M"  };

    QCOMPARE(db->isValidBand("0"), true);
    QCOMPARE(db->isValidBand("1mm"), true);

    foreach(aux, _bands)
    {
        qDebug() << aux;
        QCOMPARE(db->isValidBand(aux), true);
    }

    QStringList _modes = {"AM", "ARDOP", "AMTORFEC", "ASCI", "ATV", "CHIP", "CHIP64",
        "CHIP218", "CLO", "CONTESTI", "CW", "DIGITALVOICE", "C4FM"};
    foreach(aux, _modes)
    {
        QCOMPARE(db->isValidMode(aux), true);
    }

    int i = db->getBandIDFromName2("10M");
    QCOMPARE (db->getBandNameFromID2(i), "10M");
    i = db->getBandIDFromName2("30M");
    QCOMPARE (db->getBandNameFromID2(i), "30M");
    i = db->getBandIDFromName2("0");
    QCOMPARE (db->getBandNameFromID2(i), "0");

    i = db->getModeIDFromName2("FT4");
    QCOMPARE (db->getSubModeNameFromID2(i), "FT4");

/*

    QString getBandNameFromNumber(const int _n);
    QString getModeNameFromNumber(const int _n, bool _tmp=false);
    QString getSubModeNameFromNumber(const int _n, bool _tmp=false);

    int getBandIdFromName(const QString &b);
    int getModeIdFromName(const QString &b);
    int getModeIdFromSubMode(const QString &b);

    int getModeIDFromName2(const QString &b);
    int getSubModeIDFromName2(const QString &b);

    QString getModeNameFromID2(const int _i);
    QString getSubModeNameFromID2(const int _i);

    int getBandIdFromFreq(const QString &fr); //Freq should be in MHz
    bool isThisFreqInBand(const QString &b, const QString &fr); //Freq should be in MHz

    QString getFreqFromBandId(const int _i);
    int getNumberOfQsos(int _logNumber = -1);
    int getLastInsertedQSO();

*/
}

void tst_DataBase::test_CreateDB()
{
    Utilities util(Q_FUNC_INFO);
    QFile file(util.getCTYFile());
    QCOMPARE( file.exists(), true); // Check if the CTYDAT file is available

    DataProxy_SQLite dataProxy(Q_FUNC_INFO, version);
    //qDebug() << Q_FUNC_INFO << "003";
    World world(&dataProxy, Q_FUNC_INFO);
    QCOMPARE(world.create(util.getCTYFile()), true); // Read the CTY.CSV file into the DB
    QCOMPARE(db->hasTheTableData("entity"), true);
    QCOMPARE(db->hasTheTableData("prefixesofentity"), true);
}

void tst_DataBase::test_addQSOs()
{
    int i = db->getNumberOfQsos();
    QSO qso;
    qso.clear();
    qso.setCall("EA4K");
    qso.setDate(QDate::fromString("20240327", "yyyyMMdd"));
    qso.setTimeOn(QTime::fromString("1000", "hhmm"));
    qso.setBand("10M");
    qso.setMode("SSB");
    qso.toDB();
    QCOMPARE(db->getNumberOfQsos(), i+1);
}

void tst_DataBase::cleanupTestCase()
{
    qDebug() << Q_FUNC_INFO;
    Utilities util(Q_FUNC_INFO);
    QFile file(util.getKLogDBFile());
    if (file.exists())
        if (file.rename(util.getKLogDBFile() + "-test-db"))
            qDebug() << Q_FUNC_INFO << "DB deleted";
    qDebug() << Q_FUNC_INFO << ": " << util.getCfgFile();

    file.setFileName(util.getCfgFile());
    if (file.exists())
        if (file.rename(util.getCfgFile() +  "-test-db"))
            qDebug() << Q_FUNC_INFO << "Restoring the config file";


    file.setFileName(util.getKLogDBFile() + "-test");
    if (file.exists())
        if (file.rename(util.getKLogDBFile()))
            qDebug() << Q_FUNC_INFO << "DB deleted";
    file.setFileName(util.getCfgFile() +  "-test");
    if (file.exists())
        if (file.rename(util.getCfgFile()))
            qDebug() << Q_FUNC_INFO << "Restoring the config file";
}

/*
void tst_DataBase::cleanupTestCase()
{
    //qDebug() << Q_FUNC_INFO;
}

void tst_DataBase::init()
{
    //qDebug() << Q_FUNC_INFO;
}

void tst_DataBase::cleanup()
{
    //qDebug() << Q_FUNC_INFO;
}
*/

void tst_DataBase::test_Constructor()
{
    QString _version = QString ("99.9");
    util = new Utilities(Q_FUNC_INFO);
    db = new DataBase(Q_FUNC_INFO, _version, util->getKLogDBFile());
    QCOMPARE(db->createConnection(Q_FUNC_INFO), true);
}

QTEST_APPLESS_MAIN(tst_DataBase)

#include "tst_database.moc"

