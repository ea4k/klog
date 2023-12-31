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

#include "../../src/qso.h"
#include "../../src/klogdefinitions.h"


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


private:
    QSO *qso;
};

tst_QSO::tst_QSO()
{
    qso = new QSO;
}

tst_QSO::~tst_QSO()
{

}

void tst_QSO::initTestCase()
{

}

void tst_QSO::cleanupTestCase()
{

}

void tst_QSO::test_Constructor()
{
    //logLevel = None;
    //qsoId = -1;

    QVERIFY2(qso->getQSOid() == -1, "Wrong Id in constructor");
    QVERIFY2(qso->logLevel == None, "Wrong LogLevel in constructor");
    //qDebug() << "Testing the constructor" ;
}

void tst_QSO::test_dataEntry()
{
    qso->clear();
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
    QVERIFY2(qso->setContactedOperator("EA4K"), "Error while setting ContactedOperator");
    QVERIFY2(qso->getContactedOperator() == "EA4K", "Error while reading ContactedOperator");
    QVERIFY2(qso->setGridSquare("IN80"), "Error while setting GridSquare");
    QVERIFY2(qso->getGridSquare() == "IN80", "Error while reading GridSquare");
    QVERIFY2(qso->setMyGridSquare("IM88"), "Error while setting GridSquare");
    QVERIFY2(qso->getMyGridSquare() == "IM88", "Error while reading GridSquare");

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

/*

*/

}

QTEST_APPLESS_MAIN(tst_QSO)

#include "tst_qso.moc"
