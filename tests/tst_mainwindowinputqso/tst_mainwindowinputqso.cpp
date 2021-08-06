/***************************************************************************
                          tst_mainqsoentrywidget.h  -  description
                             -------------------
    begin                : Jan 2021
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
#include "src/inputwidgets/mainwindowinputqso.h"
#include "src/dataproxy_sqlite.h"
#include "src/utilities.h"
#include "src/locator.h"
#include "src/qso.h"

class tst_MainWindowInputQSO : public QObject
{
    Q_OBJECT

public:
    tst_MainWindowInputQSO();
    ~tst_MainWindowInputQSO();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_WriteRead();
    void test_RST2Modes();
    void test_Cleaners();

/*

    void setPropModeFromSat(const QString &_p);

    void setModifying(const bool _m);

*/

private:
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    MainWindowInputQSO *mainWindowInputQSO;

};

tst_MainWindowInputQSO::tst_MainWindowInputQSO()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    mainWindowInputQSO = new MainWindowInputQSO(dataProxy);
    util = new Utilities;
}

tst_MainWindowInputQSO::~tst_MainWindowInputQSO()
{

}

void tst_MainWindowInputQSO::initTestCase()
{

}

void tst_MainWindowInputQSO::cleanupTestCase()
{

}

void tst_MainWindowInputQSO::test_Constructor()
{
    QVERIFY2(mainWindowInputQSO->getTXFreq () == 0.0, "Wrong TX Freq");
    QVERIFY2(mainWindowInputQSO->getRXFreq () == 0.0, "Wrong RX Freq");
}

void tst_MainWindowInputQSO::test_WriteRead()
{
/*

    void setPropModeFromSat(const QString &_p);
*/
    mainWindowInputQSO->setRXPwr(1.1);
    QVERIFY2( util->isSameFreq (mainWindowInputQSO->getRXPwr(), 1.1), "Wrong RX Pwr");
    mainWindowInputQSO->setDXLocator("IN80");
    QVERIFY2(mainWindowInputQSO->getDXLocator() == "IN80", "Wrong locator");
    mainWindowInputQSO->setName("Jon Doe");
    QVERIFY2(mainWindowInputQSO->getName() == "Jon Doe", "Wrong name");
    mainWindowInputQSO->setQTH("Neverland");
    QVERIFY2(mainWindowInputQSO->getQTH() == "Neverland", "Wrong QTH");
    mainWindowInputQSO->setTXFreq (14.1);
    QVERIFY2( util->isSameFreq (mainWindowInputQSO->getTXFreq (), 14.1), "Wrong TX Freq");
    mainWindowInputQSO->setRXFreq (14.1);
    QVERIFY2( util->isSameFreq (mainWindowInputQSO->getRXFreq (), 14.1), "Wrong RX Freq");

}

void tst_MainWindowInputQSO::test_RST2Modes ()
{
//void MainWindowInputQSO::setRSTToMode(const QString &_m, const bool _reading)

    mainWindowInputQSO->setRSTTX("56");
    QVERIFY2(mainWindowInputQSO->getRSTTX ()== "56", "Wrong RST TX");
    mainWindowInputQSO->setRSTRX("43");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "43", "Wrong RST RX");

    mainWindowInputQSO->setRSTToMode ("RTTY", true);
    mainWindowInputQSO->setRSTTX("577");
    QVERIFY2(mainWindowInputQSO->getRSTTX ()== "577", "Wrong RST TX - RTTY");
    mainWindowInputQSO->setRSTRX("577");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "577", "Wrong RST RX - RTTY");
    mainWindowInputQSO->setRSTRX("57A");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "57", "Wrong invalid RST RX - RTTY");
    //qDebug() << Q_FUNC_INFO << ": RTTY Wrong RST: " << mainWindowInputQSO->getRSTRX () << Qt::endl;

    mainWindowInputQSO->setRSTToMode ("PSK31", true);
    mainWindowInputQSO->setRSTTX("577");
    QVERIFY2(mainWindowInputQSO->getRSTTX ()== "577", "Wrong RST TX - PSK31");
    mainWindowInputQSO->setRSTRX("577");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "577", "Wrong RST RX - PSK31");
    mainWindowInputQSO->setRSTRX("57A");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "57", "Wrong invalid RST RX - PSK31");
    //qDebug() << Q_FUNC_INFO << ": PSK31 Wrong RST: " << mainWindowInputQSO->getRSTRX () << Qt::endl;

    mainWindowInputQSO->setRSTToMode ("FT8", true);
    mainWindowInputQSO->setRSTTX("-18");
    QVERIFY2(mainWindowInputQSO->getRSTTX ()== "-18", "Wrong RST TX - FT8");
    mainWindowInputQSO->setRSTRX("18");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "18", "Wrong RST RX - FT8");
    mainWindowInputQSO->setRSTRX("57A");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "57", "Wrong invalid RST RX - FT8");
    //qDebug() << Q_FUNC_INFO << ": FT8 Wrong RST: " << mainWindowInputQSO->getRSTRX () << Qt::endl;

    mainWindowInputQSO->setRSTToMode ("ELSE", true);
    mainWindowInputQSO->setRSTTX("555");
    QVERIFY2(mainWindowInputQSO->getRSTTX ()== "555", "Wrong RST TX - ELSE");
    mainWindowInputQSO->setRSTRX("555");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "555", "Wrong RST RX - ELSE");
    mainWindowInputQSO->setRSTRX("57AAAA");
    QVERIFY2(mainWindowInputQSO->getRSTRX ()== "57AA", "Wrong invalid RST RX - ELSE");
    //qDebug() << Q_FUNC_INFO << ": ELSE Wrong RST: " << mainWindowInputQSO->getRSTRX () << Qt::endl;

}

void tst_MainWindowInputQSO::test_Cleaners()
{
    //void clear();
    mainWindowInputQSO->setQTH ("Neverland");
    mainWindowInputQSO->setDXLocator ("IN80");
    mainWindowInputQSO->setRXPwr (1.0);
    mainWindowInputQSO->clear ();
    QVERIFY2(mainWindowInputQSO->getQTH () == "", "Wrong clear: Not removed QTH");
    QVERIFY2(mainWindowInputQSO->getDXLocator () == "", "Wrong clear: Not removed Locator");
    QVERIFY2(mainWindowInputQSO->getRXPwr () < 0.01 , "Wrong clear: Not removed RX Power");

    //void cleanQRZCOM();
    mainWindowInputQSO->setQTH ("Neverland");
    mainWindowInputQSO->setDXLocator ("IN80");
    mainWindowInputQSO->setRXPwr (1.0);
    mainWindowInputQSO->cleanQRZCOM ();
    QVERIFY2(mainWindowInputQSO->getQTH () == "", "Wrong cleanQRZCOM: Not removed QTH");
    QVERIFY2(mainWindowInputQSO->getDXLocator () == "", "Wrong cleanQRZCOM: Not removed Locator");
    QVERIFY2(util->isSameFreq (mainWindowInputQSO->getRXPwr (), 1.0), "Wrong cleanQRZCOM: Removed RX Power");

    //void clearName();
    mainWindowInputQSO->setQTH ("Neverland");
    mainWindowInputQSO->setDXLocator ("IN80");
    mainWindowInputQSO->setRXPwr (1.0);
    mainWindowInputQSO->clearName();
    QVERIFY2(mainWindowInputQSO->getQTH () == "Neverland", "Wrong clearName: Removed QTH");
    QVERIFY2(mainWindowInputQSO->getDXLocator () == "IN80", "Wrong clearName: Removed Locator");
    QVERIFY2(util->isSameFreq (mainWindowInputQSO->getRXPwr (), 1.0), "Wrong clearName: Removed RX Power");
    //void clearQTH();
    mainWindowInputQSO->clearQTH ();
    QVERIFY2(mainWindowInputQSO->getQTH () == "", "Wrong cleanQTH: Not removed QTH");
    //void clearDXLocator();
    mainWindowInputQSO->clearDXLocator ();
    QVERIFY2(mainWindowInputQSO->getDXLocator () == "", "Wrong cleanDXLocator: Not removed Locator");
}

QTEST_MAIN(tst_MainWindowInputQSO)

#include "tests/tst_mainwindowinputqso/tst_mainwindowinputqso.moc"
