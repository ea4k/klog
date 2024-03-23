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
#include "../../src/mainqsoentrywidget.h"
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
    void test_CallSign();
    void test_InitialData();
    void test_Bands();
    void test_Modes();

private:
    DataProxy_SQLite *dataProxy;
    MainQSOEntryWidget *mainQSOEntryWidget;
};

tst_MainQSOEntryWidget::tst_MainQSOEntryWidget()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    mainQSOEntryWidget = new MainQSOEntryWidget(dataProxy);
}

tst_MainQSOEntryWidget::~tst_MainQSOEntryWidget(){}

void tst_MainQSOEntryWidget::initTestCase(){}

void tst_MainQSOEntryWidget::cleanupTestCase(){}

void tst_MainQSOEntryWidget::test_Constructor()
{
    QVERIFY((mainQSOEntryWidget->getQrz()).length () == 0);
}

void tst_MainQSOEntryWidget::test_CallSign()
{
    mainQSOEntryWidget->clear ();
    mainQSOEntryWidget->setQRZ("EA4K");
    QVERIFY((mainQSOEntryWidget->getQrz()).length () == 4);
}

void tst_MainQSOEntryWidget::test_InitialData()
{
    QString mode = mainQSOEntryWidget->getMode();
    QStringList modes, bands;
    modes.clear();
    bands.clear();
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW" << "RTTY" << "USB" << "LSB";
    mainQSOEntryWidget->setModes (modes);
    mainQSOEntryWidget->setBands (bands);
    QVERIFY(mainQSOEntryWidget->getModes().length() == modes.length ());
    QVERIFY(mainQSOEntryWidget->getDate()== QDate::currentDate());
    //qDebug() << mainQSOEntryWidget->getBand(-1) ;
    //QVERIFY(mainQSOEntryWidget->getBand(-1) == "20M");
    QVERIFY(mainQSOEntryWidget->getBand(0) == "160M");
    QVERIFY(mainQSOEntryWidget->getBand(1) == "80M");
    QVERIFY(mainQSOEntryWidget->getBand(2) == "40M");
    QVERIFY(mainQSOEntryWidget->getBand(3) == "20M");
    QVERIFY(mainQSOEntryWidget->getBand(4) == "15M");
    QVERIFY(mainQSOEntryWidget->getBand(5) == "10M");
    //QVERIFY(mainQSOEntryWidget->getBand(6) == "");
    //QVERIFY(mainQSOEntryWidget->getMode(-1) == mode);
    QVERIFY(mainQSOEntryWidget->getMode(0) == "CW");
    QVERIFY(mainQSOEntryWidget->getMode(1) == "LSB");
    QVERIFY(mainQSOEntryWidget->getMode(2) == "RTTY");
    QVERIFY(mainQSOEntryWidget->getMode(3) == "SSB");
    QVERIFY(mainQSOEntryWidget->getMode(4) == "USB");
}

void tst_MainQSOEntryWidget::test_Bands()
{
    QString band = mainQSOEntryWidget->getBand();

    QVERIFY (mainQSOEntryWidget->isBandExisting ("160M") == true);
    QVERIFY (mainQSOEntryWidget->isBandExisting ("70M") == false);
    QString currentBand = mainQSOEntryWidget->getBand ();
    mainQSOEntryWidget->setBand ("10M");
    QVERIFY (mainQSOEntryWidget->getBand() == "10M");
    mainQSOEntryWidget->setBand ("15M");
    QVERIFY (mainQSOEntryWidget->getBand() == "15M");
    mainQSOEntryWidget->setBand ("20M");
    QVERIFY (mainQSOEntryWidget->getBand() == "20M");
    mainQSOEntryWidget->setBand ("40M");
    QVERIFY (mainQSOEntryWidget->getBand() == "40M");
    mainQSOEntryWidget->setBand ("80M");
    QVERIFY (mainQSOEntryWidget->getBand() == "80M");
    mainQSOEntryWidget->setBand ("160M");
    QVERIFY (mainQSOEntryWidget->getBand() == "160M");
    //qDebug() <<mainQSOEntryWidget->getBand() ;
    //mainQSOEntryWidget->setBand("12M"); // Trying to select a band that is not in the list
    mainQSOEntryWidget->setBand (band);
    QVERIFY (mainQSOEntryWidget->getBand() == band);
    //qDebug() << mainQSOEntryWidget->getBand() ;
    mainQSOEntryWidget->selectDefaultBand ();
    //qDebug() << "DefaultBand: " << mainQSOEntryWidget->getBand() ;
    QVERIFY (mainQSOEntryWidget->getBand() == currentBand);
}

void tst_MainQSOEntryWidget::test_Modes()
{
    QVERIFY (mainQSOEntryWidget->isModeExisting ("SSB") == true);
    QVERIFY (mainQSOEntryWidget->isModeExisting ("PSK") == false);
}

QTEST_MAIN(tst_MainQSOEntryWidget)

#include "tst_mainqsoentrywidget.moc"
