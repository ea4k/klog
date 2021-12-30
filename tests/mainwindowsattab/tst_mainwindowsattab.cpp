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
#include "../../src/inputwidgets/mainwindowsattab.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/database.h"
#include "../../src/klogdefinitions.h"
#include "../../src/utilities.h"
#include "../../src/qso.h"

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


private:
    DataProxy_SQLite *dataProxy;
    MainWindowSatTab *mainWindowSattab;
};

tst_MainQSOEntryWidget::tst_MainQSOEntryWidget()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    mainWindowSattab = new MainWindowSatTab(dataProxy);
}

tst_MainQSOEntryWidget::~tst_MainQSOEntryWidget()
{

}

void tst_MainQSOEntryWidget::initTestCase()
{

}

void tst_MainQSOEntryWidget::cleanupTestCase()
{

}

void tst_MainQSOEntryWidget::test_Constructor()
{
    //QVERIFY((mainQSOEntryWidget->getQrz()).length () == 0);
}

void tst_MainQSOEntryWidget::test_SatMode()
{
    //void setUpLinkFreq(const double _t);
    //void setDownLinkFreq(const double _t);
    mainWindowSattab->setSatName ("Other");
    double freq;
    freq = dataProxy->getLowLimitBandFromBandName("10M");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    //qDebug() << mainWindowSattab->getSatMode() << endl;
    QVERIFY2(mainWindowSattab->getSatMode() == "A/A", "10m Mode not A");
    qDebug() << Q_FUNC_INFO << " - 1";
    freq = dataProxy->getLowLimitBandFromBandName("15M");
    qDebug() << Q_FUNC_INFO << " - 2";
    mainWindowSattab->setDownLinkFreq(freq);
    qDebug() << Q_FUNC_INFO << " - 3";
    //freq = dataProxy->getLowLimitBandFromBandName("10M");
    qDebug() << Q_FUNC_INFO << " - 4";
    mainWindowSattab->setUpLinkFreq(freq);
    qDebug() << Q_FUNC_INFO << " - 5";
    qDebug() << mainWindowSattab->getSatMode();
    QVERIFY2(mainWindowSattab->getSatMode() == "H/A", "15m/10m Mode not OK");

    freq = dataProxy->getLowLimitBandFromBandName("2M");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    //qDebug() << mainWindowSattab->getSatMode() << endl;
    QVERIFY2(mainWindowSattab->getSatMode() == "V/V", "2m Mode not V");

    freq = dataProxy->getLowLimitBandFromBandName("70CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "U/U", "70cm Mode not U");

    freq = dataProxy->getLowLimitBandFromBandName("23CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "L/L", "23cm Mode not L");

    freq = dataProxy->getLowLimitBandFromBandName("13CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "S/S", "13cm Mode not S");

    freq = dataProxy->getLowLimitBandFromBandName("6CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "C/C", "23cm Mode not C");

    freq = dataProxy->getLowLimitBandFromBandName("3CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    QVERIFY2(mainWindowSattab->getSatMode() == "X/X", "3cm Mode not X");

    freq = dataProxy->getLowLimitBandFromBandName("1.25CM");
    mainWindowSattab->setUpLinkFreq(freq);
    mainWindowSattab->setDownLinkFreq(freq);
    //QVERIFY2(mainWindowSattab->getSatMode() == "K-K", 1.25cm Mode not K");
}


QTEST_MAIN(tst_MainQSOEntryWidget)

#include "tst_mainwindowsattab.moc"
