/***************************************************************************
                          tst_qso.h  -  description
                             -------------------
    begin                : Aug 2022
    copyright            : (C) 2022 by Jaime Robles
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
#include <QCoreApplication>

#include "../../src/qso.h"
#include "../../src/qsodatacache.h"
#include "../../src/callsign.h"
#include "../../src/klogdefinitions.h"
#include "../../src/database/database.h"
#include "../../src/utilities.h"


class tst_QSO : public QObject
{
    Q_OBJECT

public:
    tst_QSO();
    ~tst_QSO();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_dataEntry();
    void test_isValid();
    void test_AdifCreation();
    //TODO: void test_LoTWImport();
    void test_Copy();
    void test_ModeManagement();

private:
    QSO *qso;
    void setQSOData();
};

tst_QSO::tst_QSO()
{
    Utilities util(Q_FUNC_INFO);
    QString version = QCoreApplication::applicationVersion();
    DataBase db(Q_FUNC_INFO, version, util.getKLogDBFile());
    // qDebug() << Q_FUNC_INFO << " -  After Start of DB Activities";

    if (!db.createConnection(Q_FUNC_INFO))
    {
        // qDebug() << Q_FUNC_INFO << " -  NO DB";
    }
    qso = new QSO;
}

tst_QSO::~tst_QSO(){}

void tst_QSO::setQSOData()
{
    //TODO: Complete the assignment of values
    qso->clear();
    qso->setCall("EA0A");
    qso->setOperatorCallsign("EA0Z");
    qso->setBand("10M");
    qso->setMode("USB");
    qso->setDateTimeOn(QDateTime::currentDateTimeUtc());
    qso->setQSOid(999);
    qso->setLogId(1);
    qso->setBandRX("20M");
    qso->setRSTTX("59");
    qso->setRSTRX("47");
    qso->setFreq(28.5);
    qso->setFreqRX(14.2);
    qso->setQTH("Madrid");
    qso->setGridSquare("IN80");
    qso->setName("Pepe");
    qso->setRXPwr(10.0);
    // QSL TAB
    qso->setQSL_SENT("Y");
    qso->setQSLSDate(QDate::currentDate());
    qso->setQSL_RCVD("Y");
    qso->setQSLRDate(QDate::currentDate());
    qso->setQSLVia("EA0V");
    qso->setQSLSenVia("B");
    qso->setQSLRecVia("D");
    qso->setQSLMsg("Thanks for QSO");

    // eQSL tab
    qso->setClubLogStatus("Y");

    // Comment Tab
    qso->setComment("This is a comment");
    qso->setKeepComment(true);

    qso->setAnt_az(33.0);
    qso->setAnt_el(25.0);
    qso->setA_Index(5.0);
    qso->setMaxBursts(5);
    qso->setMyDXCC(280);
    qso->setNrBursts(10);
    qso->setNrPings(10);
    qso->setTenTen(10);
    qso->setUksmg(10);
    qso->setIotaID(10);
    qso->setSIG("POTA");
    qso->setSIG_INFO("US-2134");
    qso->setMySig("POTA");
    qso->setMySigInfo("US-2133");


    //ant_az, a_index, ant_el, max_bursts, my_dxcc, nr_bursts, nr_pings
    //ten_ten, usksmg
    //iota_ID

    /*

    qso->setClubLogDate(const QDate &_c);
    qso->setEQSLQSL_RCVD(const QString &_c);
    qso->setEQSLQSL_SENT(const QString &_c);
    qso->setEQSLQSLRDate(const QDate &_c);
    qso->setEQSLQSLSDate(const QDate &_c);
    qso->setLoTWQSL_RCVD(const QString &_c);
    qso->setLoTWQSL_SENT(const QString &_c);
    qso->setLoTWQSLRDate(const QDate &_c);
    qso->setLoTWQSLSDate(const QDate &_c);
    qso->setQRZCOMStatus(const QString &_c);
    qso->setQRZCOMDate(const QDate &_c);
    qso->setDefaultEQSLSentServices(const bool _send);



    // Others Tab
    qso->setDXCC(const int _i);
    qso->setPropMode(const QString &_c);
    qso->setSOTA_REF(const QString &_c);
    qso->setAge(const double _c);
    qso->setIOTA(const QString &_c);
    qso->setKeepOthers(bool _k);
    qso->setVUCCGrids(const QString &_c);


    // My data Tab
    qso->setTXPwr(double _f);
    qso->setOperatorCallsign(const QString &_c);
    qso->setStationCallsign(const QString &_c);
    qso->setMyGridSquare(const QString &_c);
    qso->setKeepMyData(bool _k);
    qso->setMySOTA_REF(const QString &_c);
    qso->setMyRig(const QString &_c);
    qso->setMyAntenna(const QString &_c);
    qso->setMyArrlSect(const QString &_c);
    qso->setMyVUCCGrids(const QString &_c);

    // Satellite Tab
    qso->setSatName(const QString &_c);
    qso->setSatMode(const QString &_c);
    qso->setKeepSatTab(bool _k);

    qso->setAddress(const QString &_c);
    qso->setA_Index(const int _i);
    qso->setAnt_az(const double _c);
    qso->setAnt_el(const double _c);
    qso->setAnt_Path(const QString &_c);
    qso->setARRL_Sect(const QString &_c);
    qso->setCheck(const QString &_c);
    qso->setClass(const QString &_c);
    qso->setClubLogDate(const QDate &_c);
    qso->setContinent(const QString &_c);
    qso->setDistance(const double _i);
    qso->setOwnerCallsign(const QString &_c);
    qso->setEQ_Call(const QString &_c); // Contacted station owner
    qso->setHRDUpdateDate(const QDate &_c);
    qso->setHRDLogStatus(const QString &_c);
    qso->setK_Index(const int _i);
    qso->setDateOff(const QDate &_c);
    qso->setTimeOff(const QTime &_c);
    qso->setRig(const QString &_c);
    qso->setCountry(const QString &_c);
    qso->setAwardGranted(const QString &_c);
    qso->setAwardSubmitted(const QString &_c);
    qso->setCounty(const QString &_c);
    qso->setContactedOperator(const QString &_c);
    qso->setContestID(const QString &_c);
    qso->setCQZone(const int _i);
    qso->setCreditGranted(const QString &_c);
    qso->setCreditSubmitted(const QString &_c);
    qso->setDarcDok(const QString &_c);
    qso->setEmail(const QString &_c);
    qso->setFists(const int _i);
    qso->setFistsCC(const int _i);
    qso->setForceInit(bool _k);
    qso->setIotaID(const int _i);
    qso->setItuZone(const int _i);
    qso->setLatitude(const QString &_c);
    qso->setLongitude(const QString &_c);
    qso->setQSOComplete(const QString &_c);
    qso->setNrBursts(const int _i);
    qso->setMaxBursts(const int _i);
    qso->setNrPings(const int _i);
    qso->setMsShower(const QString &_c);
    qso->setQSORandom(bool _k);
    qso->setMyCity(const QString &_c);
    qso->setMyCounty(const QString &_c);
    qso->setMyCountry(const QString &_c);
    qso->setMyCQZone(const int _i);
    qso->setMyDXCC(const int _i);
    qso->setMyFists(const int _c);
    qso->setMyIOTA(const QString &_c);
    qso->setMyIotaID(const int _i);
    int getMyIotaID();
    bool setMyITUZone(const int _i);
    int getMyITUZone();
    bool setMyLatitude(const QString &_c);
    QString getMyLatitude();
    bool setMyLongitude(const QString &_c);
    QString getMyLongitude();
    bool setMyName(const QString &_c);
    QString getMyName();
    bool setMyPostalCode(const QString &_c);
    QString getMyPostalCode();

    QString getMySig();

    QString getMySigInfo();
    bool setMyState(const QString &_c);
    QString getMyState();
    bool setMyStreet(const QString &_c);
    QString getMyStreet();
    bool setMyUsacaCounties(const QString &_c);
    QString getMyUsacaCounties();
    bool setNotes(const QString &_c);
    QString getNotes();
    bool setPrefix(const QString &_c);
    QString getPrefix();
    bool setPrecedence(const QString &_c);
    QString getPrecedence();
    bool setPublicKey(const QString &_c);
    QString getPublicKey();
    bool setRegion(const QString &_c);
    QString getRegion();
    bool setTenTen(const int _i);
    int getTenTen();
    bool setSFI(const int _i);
    int getSFI();

    QString getSIG();

    QString getSIG_INFO();
    bool setSilentKey(bool _k);
    bool getSilentKey();
    bool setSkcc(const QString &_c);
    QString getSkcc();
    bool setSrx(const int _i);
    int getSrx();
    bool setSrxString(const QString &_c);
    QString getSrxString();
    bool setState(const QString &_c);
    QString getState();
    bool setStx(const int _i);
    int getStx();
    bool setStxString(const QString &_c);
    QString getStxString();
    //bool setSubmode(const QString &_c, bool requestMode = false);
    bool setSubmode(const QString &_c);
    QString getSubmode();
    bool setSwl(bool _k);
    bool getSwl();
    bool setUksmg(const int _i);
    int getUksmg();
    bool setUsacaCounties(const QString &_c);
    QString getUsacaCounties();
    bool setVeProv(const QString &_c);
    QString getVeProv();
    bool setWeb(const QString &_c);
    QString getWeb();
    bool setWWFFRef(const QString &_c);
    QString getWWFFRef();
    bool setMyWwffRef(const QString &_c);

*/
}

void tst_QSO::initTestCase(){}

void tst_QSO::cleanupTestCase(){}

void tst_QSO::test_Constructor()
{
    //logLevel = None;
    // qsoId = -1;

    QVERIFY2(qso->getQSOid() == -1, "Wrong Id in constructor");
    QVERIFY2(qso->logLevel == None, "Wrong LogLevel in constructor");
    // qDebug() << "Testing the constructor" ;
}

void  tst_QSO::test_ModeManagement()
{
    qso->setMode("FM");
    qso->setSubmode("FM");
    // qDebug() << Q_FUNC_INFO << "Mode: " << qso->getMode();
    // qDebug() << Q_FUNC_INFO << "Submode: " << qso->getSubmode();
    // qDebug() << Q_FUNC_INFO << qso->getModeIdFromModeName();
}



void tst_QSO::test_dataEntry()
{
    qso->clear();
    // qDebug() << Q_FUNC_INFO << qso->getFreqRX();
    QVERIFY2(qso->getFreqRX() == -1, "Wrong FreqRX");
    QVERIFY2(qso->getFreqTX() == -1, "Wrong FreqTX");
    QVERIFY2(qso->setPrefix("EA4"), "Error while setting Prefix");
    QVERIFY2(qso->getPrefix() == "EA4", "Error while reading Prefix");
    QVERIFY2(qso->setCall("EA4K"), "Error while setting Call");
    QVERIFY2(qso->getCall() == "EA4K", "Error while reading Call");
    QVERIFY2(qso->setOperatorCallsign("EA4K"), "Error while setting OperatorCallsign");
    QVERIFY2(qso->getOperatorCallsign() == "EA4K", "Error while reading OperatorCallsign");
    QVERIFY2(qso->setStationCallsign("EA4K"), "Error while setting StationCallsign");
    QVERIFY2(qso->getStationCallsign() == "EA4K", "Error while reading StationCallsign");
    QVERIFY2(qso->setOwnerCallsign("EA4K"), "Error while setting OwnerCallsign");
    QVERIFY2(qso->getOwnerCallsign() == "EA4K", "Error while reading OwnerCallsign");
    QVERIFY2(qso->setComment("Comment to test"), "Error while setting Commente");
    QVERIFY2(qso->getComment() == "Comment to test", "Error while reading Comment");
    QVERIFY2(qso->setContactedOperator("EA4K"), "Error while setting ContactedOperator");
    QVERIFY2(qso->getContactedOperator() == "EA4K", "Error while reading ContactedOperator");
    QVERIFY2(qso->setGridSquare("IN80"), "Error while setting GridSquare");
    QVERIFY2(qso->getGridSquare() == "IN80", "Error while reading GridSquare");
    QVERIFY2(qso->setMyGridSquare("IM88"), "Error while setting GridSquare");
    QVERIFY2(qso->getMyGridSquare() == "IM88", "Error while reading GridSquare");

    QVERIFY2(qso->setAge(33.0), "Error while setting AGE");
    QVERIFY2(qso->getAge() == 33.0, "Error while reading AGE");
    QVERIFY2(qso->setDistance(33.0), "Error while setting Distance");
    QVERIFY2(qso->getDistance() == 33.0, "Error while reading Distance");

    QVERIFY2(qso->setAnt_az(33.0), "Error while setting ANT_AZ");
    QVERIFY2(qso->getAnt_az() == 33.0, "Error while reading ANT_AZ");
    QVERIFY2(qso->setAnt_el(25.0), "Error while setting ANT_EL");
    QVERIFY2(qso->getAnt_el() == 25.0, "Error while reading ANT_EL");
    QVERIFY2(qso->setA_Index(5.0), "Error while setting A_INDEX");
    QVERIFY2(qso->getA_Index() == 5.0, "Error while reading A_INDEX");
    QVERIFY2(qso->setMaxBursts(5), "Error while setting Max Bursts");
    QVERIFY2(qso->getMaxBursts() == 5, "Error while reading Max Bursts");
    QVERIFY2(qso->setMyDXCC(280), "Error while setting My DXCC");
    QVERIFY2(qso->getMyDXCC() == 280, "Error while reading My DXCC");
    QVERIFY2(qso->setNrBursts(10), "Error while setting NR Bursts");
    QVERIFY2(qso->getNrBursts() == 10, "Error while reading NR Bursts");
    QVERIFY2(qso->setNrPings(10), "Error while setting NR Pings");
    QVERIFY2(qso->getNrPings() == 10, "Error while reading NR Pings");
    QVERIFY2(qso->setTenTen(10), "Error while setting TenTen");
    QVERIFY2(qso->getTenTen() == 10, "Error while reading TenTen");
    QVERIFY2(qso->setUksmg(10), "Error while setting UKSMG");
    QVERIFY2(qso->getUksmg() == 10, "Error while reading UKSMG");
    QVERIFY2(qso->setIotaID(10), "Error while setting IOTA-ID");
    QVERIFY2(qso->getIotaID() == 10, "Error while reading IOTA-ID");


    QVERIFY2(qso->setSIG("POTA"), "Error while setting SIG");
    QVERIFY2(qso->getSIG() == "POTA", "Error while reading SIG");
    QVERIFY2(qso->setSIG_INFO("US-2133"), "Error while setting SIG_INFO");
    QVERIFY2(qso->getSIG_INFO() == "US-2133", "Error while reading SIG_INFO");

    QVERIFY2(qso->setMySig("POTA"), "Error while setting MY_SIG");
    QVERIFY2(qso->getMySig() == "POTA", "Error while reading MY_SIG");
    QVERIFY2(qso->setMySigInfo("US-2134"), "Error while setting MY_SIG_INFO");
    QVERIFY2(qso->getMySigInfo() == "US-2134", "Error while reading MY_SIG_INFO");

    qso->setLogLevel(Info);
    QVERIFY2(qso->logLevel == Info, "Error while setting logLevel (Info)");
    qso->setLogLevel(Debug);
    QVERIFY2(qso->logLevel == Debug, "Error while setting logLevel (Debug)");
    qso->setLogLevel(Devel);
    QVERIFY2(qso->logLevel == Devel, "Error while setting logLevel (Devel)");
    qso->setRealTime(true);
    QVERIFY2(qso->realTime, "Error while reading real time true");
    qso->setRealTime(false);
    QVERIFY2(!qso->realTime, "Error while reading real time false");
    qso->setManualMode(true);
    QVERIFY2(qso->getManualMode(), "Error while reading manual mode true");
    qso->setManualMode(false);
    QVERIFY2(!qso->getManualMode(), "Error while reading manual mode false");
    qso->setBackup(true);
    QVERIFY2(qso->getBackup(), "Error while reading backup true");
    qso->setBackup(false);
    QVERIFY2(!qso->getBackup(), "Error while reading backup false");
    qso->setModifying(true);
    QVERIFY2(qso->getModifying(), "Error while reading Modifying true");
    qso->setModifying(false);
    QVERIFY2(!qso->getModifying(), "Error while reading Modifying false");
}

void tst_QSO::test_isValid()
{
    QSO qt;
    QVERIFY2(!qt.isValid(), "Empty QSO Should not be valid");
    qt.setCall("EA4K");
    QVERIFY2(!qt.isValid(), "QSOs with just a call should not be valid");
    qt.setBand("10M");
    QVERIFY2(!qt.isValid(), "QSOs with just a call and band should not be valid");
    qt.setMode("SSB");
    QVERIFY2(!qt.isValid(), "QSOs with just a call, band and mode should not be valid");}

void tst_QSO::test_AdifCreation()
{
    setQSOData();   // To fill a QSO with all the data

}

void tst_QSO::test_Copy()
{
    QSO qso1;
    qso1.clear();
    qso1.setCall("EA4K");
    qso1.setMyCity("Madrid");
    qso1.setComment("QSO1-comment");
    QSO qso2(qso1);
    QSO qso3;
    qso3.clear();
    qso3.copy(qso1);
    // qDebug() << Q_FUNC_INFO << " -1- " << qso1.getCall();
    // qDebug() << Q_FUNC_INFO << " -2- " << qso2.getCall();
    // qDebug() << Q_FUNC_INFO << " -3- " << qso3.getCall();
    QVERIFY2(qso2.getCall() == "EA4K", "Wrong Call in constructor copy");
    QVERIFY2(qso3.getCall() == "EA4K", "Wrong Call in copy");
    QVERIFY2(qso2.getMyCity() == "Madrid", "Wrong My_CITY in constructor copy");
    QVERIFY2(qso3.getMyCity() == "Madrid", "Wrong My_CITY in copy");

    QVERIFY2(qso2.getComment() == "QSO1-comment", "Wrong Comment in constructor copy");
    QVERIFY2(qso3.getComment() == "QSO1-comment", "Wrong Comment in copy");
}

// qTEST_APPLESS_MAIN(tst_QSO)
QTEST_GUILESS_MAIN(tst_QSO)

#include "tst_qso.moc"
