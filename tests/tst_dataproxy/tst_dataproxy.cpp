#include <QtTest>
#include "../../src/dataproxy_sqlite.h"
#include "../../src/utilities.h"
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
    void test_modes();
    void test_bands();
    //void test_getProgresStepForDialog();

private:
  DataProxy_SQLite *dataProxy;
  Utilities *util;

};

tst_DataProxy::tst_DataProxy()
{
    util = new Utilities;
    QString version = "1.5";
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, version);
}

tst_DataProxy::~tst_DataProxy()
{

}

void tst_DataProxy::initTestCase()
{

}

void tst_DataProxy::cleanupTestCase()
{

}

void tst_DataProxy::test_Constructor()
{
    QVERIFY2(util->getVersion() == "0.0", "Version failed");
    //qDebug() << Q_FUNC_INFO << ": " << dataProxy->getSoftVersion ();
    //qDebug() << Q_FUNC_INFO << ": " << dataProxy->getDBVersion ();
}

void tst_DataProxy::test_modes()
{

    QVERIFY2(dataProxy->getIdFromModeName("FT8") == dataProxy->getSubModeIdFromSubMode ("FT8"), "FT8 mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("Q65") == "MFSK", "Q65 mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("FT8") == "FT8", "FT8 mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("USB") == "SSB", "USB mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("LSB") == "SSB", "USB mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("JT9C") == "JT9", "JT9C mode/Submode failed");
    QVERIFY2(dataProxy->getNameFromSubMode ("CW") == "CW", "CW mode/Submode failed");
    QVERIFY2(dataProxy->getSubModeFromId (dataProxy->getIdFromModeName ("FT8")) == "FT8", "Submode from Id failed");
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
    //QVERIFY2(dataProxy->getBandNameFromFreq (41) == "8M", "8M band failed");
    QVERIFY2(dataProxy->getBandNameFromFreq (52.1) == "6M", "6M band failed");
    //QVERIFY2(dataProxy->getBandNameFromFreq (57) == "5M", "5M band failed");
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

    QVERIFY2(dataProxy->isThisFreqInBand ("80M", "3.775"), "Freq in band failed");
    QVERIFY2(!dataProxy->isThisFreqInBand ("80M", "28.775"), "Freq in band failed");

    QVERIFY2(dataProxy->getNameFromBandId (dataProxy->getIdFromBandName ("20M")) == "20M", "Band names and Id failed");




}

QTEST_APPLESS_MAIN(tst_DataProxy)

#include "tst_dataproxy.moc"

