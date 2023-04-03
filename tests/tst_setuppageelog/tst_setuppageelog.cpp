/***************************************************************************
                          tst_setuppageelog.cpp  -  description
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
#include "../../src/setuppages/setuppageelog.h"

class tst_SetupPageELog: public QObject
{
    Q_OBJECT

public:
    tst_SetupPageELog();
    ~tst_SetupPageELog();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_ClubLog();
    void test_LoTW();
    void test_eQSL();
    void test_QRZCOM();

private:
    SetupPageELog *page;

};

tst_SetupPageELog::tst_SetupPageELog()
{
    page = new SetupPageELog();
}

tst_SetupPageELog::~tst_SetupPageELog()
{

}

void tst_SetupPageELog::initTestCase()
{

}

void tst_SetupPageELog::cleanupTestCase()
{

}

void tst_SetupPageELog::test_Constructor()
{

}

void tst_SetupPageELog::test_ClubLog()
{
    /*
    page->setClubLogActive(false);
    QVERIFY2(!page->getClubLogActive(), "ClubLog not Active failed");
    page->setClubLogActive(true);
    QVERIFY2(page->getClubLogActive(), "ClubLog Active failed");

    page->setClubLogRealTime(false);
    QVERIFY2(!page->getClubLogRealTime(), "ClubLog not Real Time failed");
    page->setClubLogRealTime(true);
    QVERIFY2(page->getClubLogRealTime(), "ClubLog Real Time failed");

    page->setClubLogEmail("test@klog.xyz");
    QVERIFY2(page->getClubLogEmail() == "test@klog.xyz", "ClubLog get Email failed");
    page->setClubLogPassword("secretpassword");
    QVERIFY2(page->getClubLogPassword() == "secretpassword", "ClubLog get Password failed");
    */
}

void tst_SetupPageELog::test_LoTW()
{
 /*
     page->setLoTWActive(false);
    QVERIFY2(!page->getLoTWActive(), "LoTW not Active failed");
    page->setLoTWActive(true);
    QVERIFY2(page->getLoTWActive(), "LoTW Active failed");
    page->setTQSLPath("/usr/bin/path/");
    QVERIFY2(page->getTQSLPath() == "/usr/bin/path/", "TQSL get path failed");
    page->setLoTWUser("ea4k");
    QVERIFY2(page->getLoTWUser() == "EA4K", "LoTW get User failed");
    page->setLoTWPass("secretpassword");
    QVERIFY2(page->getLoTWPass() == "secretpassword", "LoTW get Password failed");
    */
}

void tst_SetupPageELog::test_eQSL()
{
    page->setEQSLActive(false);
    QVERIFY2(!page->getEQSLActive(), "eQSL not Active failed");
    page->setEQSLActive(true);
    QVERIFY2(page->getEQSLActive(), "eQSL Active failed");

    page->setEQSLUser("ea4k");
    QVERIFY2(page->getEQSLUser() == "EA4K", "eQSL get User failed");
    page->setEQSLPassword("secretpassword");
    QVERIFY2(page->getEQSLPassword() == "secretpassword", "eQSL get Password failed");
}

void tst_SetupPageELog::test_QRZCOM()
{
    page->setQRZCOMActive(false);
    QVERIFY2(!page->getQRZCOMActive(), "QRZ not Active failed");
    page->setQRZCOMActive(true);
    QVERIFY2(page->getQRZCOMActive(), "QRZ Active failed");

    page->setQRZCOMSubscriber(false);
    QVERIFY2(!page->getQRZCOMSubscriber(), "QRZ not Subscriber failed");
    page->setQRZCOMSubscriber(true);
    QVERIFY2(page->getQRZCOMSubscriber(), "QRZ Subscriber failed");

    page->setQRZCOMAutoCheck(false);
    QVERIFY2(!page->getQRZCOMAutoCheck(), "QRZ not Autocheck failed");
    page->setQRZCOMAutoCheck(true);
    QVERIFY2(page->getQRZCOMAutoCheck(), "QRZ Autocheck failed");

    page->setQRZCOMUser("ea4k");
    QVERIFY2(page->getQRZCOMUser() == "EA4K", "QRZ get User failed");
    page->setQRZCOMPassword("secretpassword");
    QVERIFY2(page->getQRZCOMPassword() == "secretpassword", "QRZ get LogBook-key failed");

    page->setQRZCOMLogBookKEY("XXXYYY");
    QVERIFY2(page->getQRZCOMLogBookKEY() == "XXXYYY", "QRZ get LogBook-key failed");
}

QTEST_MAIN(tst_SetupPageELog)

#include "tst_setuppageelog.moc"
