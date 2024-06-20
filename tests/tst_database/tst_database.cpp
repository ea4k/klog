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
#include "../../src/database/db_adif_primary_subdvisions_data.h"
#include "../../src/database/queryexecutor.h"

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
    void test_bands();
    void test_modes();
    void test_subModes();
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
    QFile file(util.getKLogDBFile() + "-test");

    if (file.exists())
    {
        qDebug() << Q_FUNC_INFO << QString("DB-test exists: %1").arg(util.getKLogDBFile() + "-test");
        if (file.remove())
        {
            qDebug() << Q_FUNC_INFO << "DB-test deleted";
            QCOMPARE(file.exists(), false);
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "DB-test NOT deleted";
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO << QString("DB-test DOES NOT exists: %1").arg(util.getKLogDBFile() + "-test");
    }


    file.setFileName(util.getKLogDBFile());
    if (file.exists())
        if (file.rename(util.getKLogDBFile() + "-test"))
            qDebug() << Q_FUNC_INFO << "DB renamed";



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
    QCOMPARE(db->isTheTableExisting("contestcategory"), true);
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
    QCOMPARE(db->isTheTableExisting("supportedcontests"), true);
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
    QCOMPARE(db->hasTheTableData("entity"), true);

    QCOMPARE(db->hasTheTableData("log"), false);
    QCOMPARE(db->hasTheTableData("logs"), false);
    QCOMPARE(db->hasTheTableData("mode"), true);
    QCOMPARE(db->hasTheTableData("prefixesofentity"), true);
    QCOMPARE(db->hasTheTableData("primary_subdivisions"), false);
    QCOMPARE(db->hasTheTableData("prop_mode_enumeration"), true);
    QCOMPARE(db->hasTheTableData("qsl_rec_status"), true);
    QCOMPARE(db->hasTheTableData("qsl_sent_status"), true);
    QCOMPARE(db->hasTheTableData("qsl_via_enumeration"), true);
    QCOMPARE(db->hasTheTableData("satellites"), true);
    QCOMPARE(db->hasTheTableData("qso_complete_enumeration"), true);
    QCOMPARE(db->hasTheTableData("softwarecontrol"), true);
    QCOMPARE(db->hasTheTableData("supportedcontests"), true);
}

void tst_DataBase::test_bands()
{
    QCOMPARE(db->isTheDBCreated(), true);
    QString aux;
    QStringList _bands = { "0", "1mm", "2mm", "2.5mm", "4mm", "6mm",
                          "1.25CM", "3CM", "6CM", "9CM", "13CM", "23CM",
                          "70CM", "1.25M", "2M", "4M", "6M", "8M", "10M",
                          "12M", "15M", "17M", "20M", "30M", "40M", "60M",
                          "80M", "160M", "560M", "630M", "2190M"  };

    foreach(aux, _bands)
    {
        //qDebug() << aux;
        QCOMPARE(db->isValidBand(aux), true);
    }
}

void tst_DataBase::test_modes()
{
    QCOMPARE(db->isTheDBCreated(), true);
    QString aux;
    QStringList _modes = {"AM", "ARDOP", "TOR", "RTTY", "ATV", "CHIP", "CLO",
                          "CONTESTI", "CW", "DIGITALVOICE", "DYNAMIC", "DOMINO", "FAX", "FM", "HELL",
                          "MFSK", "FSK441", "FT8", "ISCAT", "JT4", "JT44", "JT65", "JT6M", "JT9",
                          "MSK144", "MT63", "OLIVIA", "OPERA", "PAC", "PAX", "PKT", "PSK", "PSK2K",
                          "Q15", "QRA64", "ROS", "RTTYM", "SSB", "SSTV", "T10", "THOR", "THRB", "V4",
                          "VOI", "WINMOR", "WSPR"};
    foreach(aux, _modes)
    {
        QCOMPARE(db->getModeIdFromName(aux)>0, true);
    }
}

void tst_DataBase::test_subModes()
{
    QCOMPARE(db->isTheDBCreated(), true);
    QString aux;
    QStringList _submodes = {"CHIP64", "CHIP128", "PCW", "C4FM", "DMR", "DSTAR", "FREEDV",
        "M17", "DOM-M", "DOM4", "DOM5", "DOM8", "DOM11", "DOM16", "DOM22", "DOM44", "DOM88",
        "DOMINOEX", "DOMINOF", "VARA HF", "VARA SATELLITE", "VARA FM 1200", "VARA FM 9600",
        "FMHELL", "FSKHELL", "HELL80", "HELLX5", "HELLX9", "HFSK", "PSKHELL", "SLOWHELL",
        "ISCAT-A", "ISCAT-B", "JT4A", "JT4B", "JT4C", "JT4D", "JT4E", "JT4F", "JT4G", "JT9-1",
        "JT9-2", "JT9-5", "JT9-10", "JT9-30", "JT9A", "JT9B", "JT9C", "JT9D", "JT9E",
        "JT9E FAST", "JT9F", "JT9F FAST", "JT9G", "JT9G FAST", "JT9H", "JT9H FAST", "JT65A",
        "JT65B", "JT65B2", "JT65C", "JT65C2", "FSQCALL", "FST4", "FST4W", "FT4", "JS8", "JTMS",
        "MFSK4", "MFSK8", "MFSK11", "MFSK16", "MFSK22", "MFSK31", "MFSK32", "MFSK64",
        "MFSK64L", "MFSK128", "MFSK128L", "Q65", "OLIVIA 4/125", "OLIVIA 4/250", "OLIVIA 8/250",
        "OLIVIA 8/500", "OLIVIA 16/500", "OLIVIA 16/1000", "OLIVIA 32/1000", "OPERA-BEACON",
        "OPERA-QSO", "8PSK125", "8PSK125F", "8PSK125FL", "8PSK250", "8PSK250F", "8PSK250FL",
        "8PSK500", "8PSK500F", "8PSK1000", "8PSK1000F", "8PSK1200F", "FSK31", "PSK10", "PSK31",
        "PSK63", "PSK63F", "PSK63RC4", "PSK63RC5", "PSK63RC10", "PSK63RC20", "PSK63RC32",
        "PSK125", "PSK125C12", "PSK125R", "PSK125RC10", "PSK125RC12", "PSK125RC16", "PSK125RC4",
        "PSK125RC5", "PSK250", "PSK250C6", "PSK250R", "PSK250RC2", "PSK250RC3", "PSK250RC5",
        "PSK250RC6", "PSK250RC7", "PSK500", "PSK500C2", "PSK500C4", "PSK500R", "PSK500RC2",
        "PSK500RC3", "PSK500RC4", "PSK800C2", "PSK800RC2", "PSK1000", "PSK1000C2", "PSK1000R",
        "PSK1000RC2", "PSKAM10", "PSKAM31", "PSKAM50", "PSKFEC31", "QPSK31", "QPSK63", "QPSK125",
        "QPSK250", "QPSK500", "SIM31", "PAC2", "PAC3", "PAC4", "QRA64A", "QRA64B", "QRA64C",
        "QRA64D", "QRA64E", "ROS-EME", "ROS-HF", "ROS-MF", "ASCI", "LSB", "USB", "THOR-M",
        "THOR4", "THOR5", "THOR8", "THOR11", "THOR16", "THOR22", "THOR25X4", "THOR50X1",
        "THOR50X2", "THOR100", "AMTORFEC", "GTOR", "NAVTEX", "SITORB", "THRBX", "THRBX1",
        "THRBX2", "THRBX4", "THROB1", "THROB2", "THROB4" };

    foreach(aux, _submodes)
    {
        //qDebug() << " - Testing: " << aux;
        QCOMPARE(db->getModeIdFromSubMode(aux)>0, true);
    }
}

void tst_DataBase::test_checks()
{
    //DataBase db(Q_FUNC_INFO, version, util->getKLogDBFile());
    QCOMPARE(db->isTheDBCreated(), true);

    int i = db->getBandIdFromName("10M");
    QCOMPARE (db->getBandNameFromNumber(i), "10M");
    i = db->getBandIdFromName("30M");
    QCOMPARE (db->getBandNameFromNumber(i), "30M");
    i = db->getBandIdFromName("0");
    QCOMPARE (db->getBandNameFromNumber(i), "0");


    i = db->getModeIdFromSubMode("FT4");
    QCOMPARE (db->getSubModeNameFromNumber(i), "FT4");

/*
    QString getBandNameFromNumber(const int _n);
    QString getModeNameFromNumber(const int _n, bool _tmp=false);
    QString getSubModeNameFromNumber(const int _n, bool _tmp=false);

    int getBandIdFromName(const QString &b);
    int getModeIdFromName(const QString &b);
    int getModeIdFromSubMode(const QString &b);


    QString getSubModeNameFromNumber(const int _i);

    int getBandIdFromFreq(const QString &fr); //Freq should be in MHz
    bool isThisFreqInBand(const QString &b, const QString &fr); //Freq should be in MHz

    QString getFreqFromBandId(const int _i);
    int getNumberOfQsos(int _logNumber = -1);
    int getLastInsertedQSO();

*/
}

void tst_DataBase::test_CreateDB()
{
    qDebug() << Q_FUNC_INFO << "000";
    Utilities util(Q_FUNC_INFO);
    qDebug() << Q_FUNC_INFO << "001";
    QFile file(util.getCTYFile());
    qDebug() << Q_FUNC_INFO << "003";
    QCOMPARE( file.exists(), true); // Check if the CTYDAT file is available
    qDebug() << Q_FUNC_INFO << "003";
    DataProxy_SQLite dataProxy(Q_FUNC_INFO, version);
    qDebug() << Q_FUNC_INFO << "004";
    World world(&dataProxy, Q_FUNC_INFO);
    qDebug() << Q_FUNC_INFO << "005";
    QCOMPARE(world.create(util.getCTYFile()), true); // Read the CTY.CSV file into the DB
    qDebug() << Q_FUNC_INFO << "006";
    QCOMPARE(db->hasTheTableData("entity"), true);
    qDebug() << Q_FUNC_INFO << "007";
    QCOMPARE(db->hasTheTableData("prefixesofentity"), true);
    qDebug() << Q_FUNC_INFO << "999";
}


void tst_DataBase::test_addQSOs()
{
    int i = db->getNumberOfQsos();
    qDebug() << "Number of QSOs: " << QString::number(i);
    QSO qso;
    qso.clear();
    qso.setCall("EA4K");
    qso.setDate(QDate::fromString("20240327", "yyyyMMdd"));
    qso.setTimeOn(QTime::fromString("1000", "hhmm"));
    qso.setBand("10M");
    qso.setMode("SSB");
    qso.toDB();
    qDebug() << "Number of QSOs: " << QString::number(i);
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
    qDebug() << Q_FUNC_INFO << "- Start";
    QString _version = QString ("99.9");
    qDebug() << Q_FUNC_INFO << "- 001";
    util = new Utilities(Q_FUNC_INFO);
    qDebug() << Q_FUNC_INFO << "- 002";
    db = new DataBase(Q_FUNC_INFO, _version, util->getKLogDBFile());
    qDebug() << Q_FUNC_INFO << "- 003";
    QCOMPARE(db->createConnection(Q_FUNC_INFO), true);
    qDebug() << Q_FUNC_INFO << " - END";
}

QTEST_APPLESS_MAIN(tst_DataBase)

#include "tst_database.moc"

