/***************************************************************************
                          tst_mainwindowsattab.h  -  description
                             -------------------
    begin                : Feb 2021
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
#include "../../src/database/database.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/frequency.h"
#include "../../src/inputwidgets/mainwindowsattab.h"
#include "../../src/klogdefinitions.h"
#include "../../src/qso.h"
#include "../../src/utilities.h"

class tst_MainQSOEntryWidget : public QObject
{
    Q_OBJECT

public:
    tst_MainQSOEntryWidget();
    ~tst_MainQSOEntryWidget();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_SatMode();
    void test_SetQSOData();

private:
    DataProxy_SQLite *dataProxy;
    MainWindowSatTab *mainWindowSattab;
};

tst_MainQSOEntryWidget::tst_MainQSOEntryWidget()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    mainWindowSattab = new MainWindowSatTab(dataProxy);
}

tst_MainQSOEntryWidget::~tst_MainQSOEntryWidget(){}

void tst_MainQSOEntryWidget::initTestCase(){}

void tst_MainQSOEntryWidget::cleanupTestCase(){}

void tst_MainQSOEntryWidget::test_Constructor()
{
    // qVERIFY((mainQSOEntryWidget->getQrz()).length () == 0);
}

void tst_MainQSOEntryWidget::test_SatMode()
{
    //void setUpLinkFreq(const double _t);
    //void setDownLinkFreq(const double _t);
    QStringList _bands;
    _bands.clear();
    _bands << "2M" << "70CM" << "23CM" << "13CM" << "6CM" << "3CM" << "1.25CM";
    mainWindowSattab->addBands(_bands);

    mainWindowSattab->setSatName ("Other");
    Frequency freq;
    /*
    freq = dataProxy->getLowLimitBandFromBandName("15M");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    //qDebug() << mainWindowSattab->getSatMode() ;
    QVERIFY2(mainWindowSattab->getSatMode() == "A/A", "10m Mode not A");
    //qDebug() << Q_FUNC_INFO << " - 1";
    freq = dataProxy->getLowLimitBandFromBandName("15M");
    mainWindowSattab->setDownLinkFreq(freq);
    mainWindowSattab->setUpLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "H/A", "15m/10m Mode not OK");
*/
    //qDebug() << Q_FUNC_INFO << ": 2m" ;
    freq = dataProxy->getLowLimitBandFromBandName("2M");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    //qDebug() << mainWindowSattab->getSatMode();
    //qDebug() << QString::number(mainWindowSattab->getRXFreq());
    //qDebug() << QString::number(mainWindowSattab->getTXFreq());
    QVERIFY2(mainWindowSattab->getSatMode() == "V/V", "2m Mode not V");

    //qDebug() << Q_FUNC_INFO << ": 70cm";
    freq = dataProxy->getLowLimitBandFromBandName("70CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "U/U", "70cm Mode not U");

    //qDebug() << Q_FUNC_INFO << ": 23cm";
    freq = dataProxy->getLowLimitBandFromBandName("23CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "L/L", "23cm Mode not L");

    //qDebug() << Q_FUNC_INFO << ": 13cm" ;
    freq = dataProxy->getLowLimitBandFromBandName("13CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "S/S", "13cm Mode not S");

    //qDebug() << Q_FUNC_INFO << ": 6cm" ;
    freq = dataProxy->getLowLimitBandFromBandName("6CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    // qVERIFY2(mainWindowSattab->getSatMode() == "C/C", "6cm Mode not C");

    //qDebug() << Q_FUNC_INFO << ": 3cm" ;
    freq = dataProxy->getLowLimitBandFromBandName("3CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "X/X", "3cm Mode not X");

    //qDebug() << Q_FUNC_INFO << ": 1.25cm";
    freq = dataProxy->getLowLimitBandFromBandName("1.25CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "K/K", "1.25cm Mode not K");
}


void tst_MainQSOEntryWidget::test_SetQSOData()
{
    // Ensure satellite-relevant bands are available in the combo boxes
    QStringList bands;
    bands << "10M" << "2M" << "70CM" << "23CM";
    mainWindowSattab->addBands(bands);

    // ----------------------------------------------------------------
    // Test 1 (regression #894): setQSOData must populate the tab even
    // when satNameComboBox starts at index 0 (the "no satellite" state).
    // The old code had an early return that prevented any data from
    // being set.
    // ----------------------------------------------------------------
    mainWindowSattab->setNoSat();
    QVERIFY2(mainWindowSattab->getSatName().isEmpty(),
             "Pre-condition failed: sat name should be empty after setNoSat()");

    QSO qso1;
    qso1.setSatName("AO-51");
    qso1.setSatMode("V/U");
    qso1.setFreq(145.920);    // 2M uplink
    qso1.setFreqRX(435.300);  // 70CM downlink

    mainWindowSattab->setQSOData(qso1);

    QVERIFY2(mainWindowSattab->getSatName() == "AO-51",
             "Regression #894: sat name not populated when combo was at index 0");
    QVERIFY2(mainWindowSattab->getSatMode() == "V/U",
             "Regression #894: sat mode not populated when combo was at index 0");
    QVERIFY2(mainWindowSattab->getRXFreq() == Frequency(435.300),
             "Regression #894: downlink frequency not set by setQSOData");

    // ----------------------------------------------------------------
    // Test 2: different known satellite – verifies correct behaviour
    // across consecutive calls and that each field is set independently.
    // ----------------------------------------------------------------
    mainWindowSattab->setNoSat();

    QSO qso2;
    qso2.setSatName("AO-7");
    qso2.setSatMode("U/V");
    qso2.setFreq(432.180);    // 70CM uplink
    qso2.setFreqRX(145.920);  // 2M downlink

    mainWindowSattab->setQSOData(qso2);

    QVERIFY2(mainWindowSattab->getSatName() == "AO-7",
             "Known satellite AO-7: name not populated by setQSOData");
    QVERIFY2(mainWindowSattab->getSatMode() == "U/V",
             "Known satellite AO-7: sat mode not populated");
    QVERIFY2(mainWindowSattab->getRXFreq() == Frequency(145.920),
             "Known satellite AO-7: downlink frequency not set");

    // ----------------------------------------------------------------
    // Test 3: satellite not in the database must be stored via the
    // "Other" entry and be fully retrievable through getSatName().
    // ----------------------------------------------------------------
    mainWindowSattab->setNoSat();

    QSO qso3;
    qso3.setSatName("XO-99");
    qso3.setSatMode("V/U");
    qso3.setFreq(145.900);
    qso3.setFreqRX(435.100);

    mainWindowSattab->setQSOData(qso3);

    QVERIFY2(mainWindowSattab->getSatName() == "XO-99",
             "Unknown satellite: name not stored via the 'Other' combo entry");
    QVERIFY2(mainWindowSattab->getSatMode() == "V/U",
             "Unknown satellite: sat mode not populated");
    QVERIFY2(mainWindowSattab->getRXFreq() == Frequency(435.100),
             "Unknown satellite: downlink frequency not set");

    // ----------------------------------------------------------------
    // Test 4: QSO with no satellite data must not crash and must leave
    // both satellite name and mode empty.
    // ----------------------------------------------------------------
    mainWindowSattab->setNoSat();

    QSO qso4; // no satName, no satMode, no frequencies
    mainWindowSattab->setQSOData(qso4);

    QVERIFY2(mainWindowSattab->getSatName().isEmpty(),
             "QSO with no satellite data should yield an empty satellite name");
    QVERIFY2(mainWindowSattab->getSatMode().isEmpty(),
             "QSO with no satellite data should yield an empty satellite mode");
}

QTEST_MAIN(tst_MainQSOEntryWidget)

#include "tst_mainwindowsattab.moc"
