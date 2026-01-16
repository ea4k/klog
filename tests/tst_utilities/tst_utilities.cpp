/***************************************************************************
                          tst_utilities.h  -  description
                             -------------------
    begin                : Jan 2021
    copyright            : (C) 2021 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/****************************************************************************
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
#include "../../src/utilities.h"
#include "../../src/dataproxy_sqlite.h"
#include "QtTest/qtestcase.h"

class tst_Utilities : public QObject
{
    Q_OBJECT

public:
    tst_Utilities();
    ~tst_Utilities();

private slots:
    void initTestCase(); //will be called before the first test function is executed.

    //void initTestCase_data(); // will be called to create a global test data table.
    void cleanupTestCase(); //will be called after the last test function was executed.
    //void init(); //will be called before each test function is executed.
    //void cleanup(); // will be called after every test function.

    void test_Constructor();
    void test_getProgresStepForDialog();
    void test_trueOrFalse();
    void test_boolToCharToSQLite();
    void test_boolToQString();
    void test_getGlobalAgent();
    void test_getHomeDir();
    void test_getNormalizedDXCCValue();
    void test_isValidFreq();
    void test_isValidGrid();
    void test_isValidVUCC();
    void test_isValidADIFField();
    void test_logLevels();
    void test_getPrefixFromCall();    
    void test_isValidEmail();
    void test_QSL();
    
private:
  Utilities *util;
  DataProxy_SQLite *dataProxy;
};

tst_Utilities::tst_Utilities()
{
    util = new Utilities(Q_FUNC_INFO);
    //util->init();
    QString version = "1.5";
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, version);
    util->setLogLevel(Devel);
}

tst_Utilities::~tst_Utilities()
{
}

void tst_Utilities::initTestCase()
{
    // qDebug("Called before everything else.");
}

void tst_Utilities::cleanupTestCase()
{
    // qDebug("Called after last test.");
}

void tst_Utilities::test_Constructor()
{
    QVERIFY(util->getVersion() == "0.0");
}

void tst_Utilities::test_getProgresStepForDialog()
{
    QVERIFY(util->getVersion() == "0.0");
    QVERIFY(util->getProgresStepForDialog(99) == 1);
    QVERIFY(util->getProgresStepForDialog(999) == 5);
    QVERIFY(util->getProgresStepForDialog(3999) == 10);
    QVERIFY(util->getProgresStepForDialog(4999) == 15);
    QVERIFY(util->getProgresStepForDialog(6999) == 20);
    QVERIFY(util->getProgresStepForDialog(9998) == 25);
    QVERIFY(util->getProgresStepForDialog(19999) == 100);
    QVERIFY(util->getProgresStepForDialog(20001) == 250);
}

void tst_Utilities::test_trueOrFalse()
{
    QVERIFY(util->trueOrFalse("TRUE") == true);
    QVERIFY(util->trueOrFalse("FALSE") == false);
}

void tst_Utilities::test_boolToCharToSQLite()
{
    QVERIFY(util->boolToCharToSQLite(true) == 'Y');
    QVERIFY(util->boolToCharToSQLite(false) == 'N');
}

void tst_Utilities::test_boolToQString()
{
     QVERIFY(util->boolToQString(true) == "True");
     QVERIFY(util->boolToQString(false) == "False");
}

void tst_Utilities::test_getGlobalAgent()
{
    QVERIFY(util->getGlobalAgent("1.5") == "KLog-1.5");
}

void tst_Utilities::test_getHomeDir()
{
    #if defined(Q_OS_WIN)
       QVERIFY(util->getHomeDir() == QDir::homePath()+"/klog" );
    #else
       QVERIFY(util->getHomeDir() == QDir::homePath()+"/.klog" );
    #endif
}

void tst_Utilities::test_getNormalizedDXCCValue()
{
    QVERIFY(util->getNormalizedDXCCValue(100) == 100);
    QVERIFY(util->getNormalizedDXCCValue(1000) == 1000);
    QVERIFY(util->getNormalizedDXCCValue(1234) == 234);
}


void tst_Utilities::test_isValidFreq()
{
    QVERIFY(util->isValidFreq("14.155") == true);
    QVERIFY(util->isValidFreq("-14.155") == false);
    QVERIFY(util->isValidFreq("14.155A") == false);
    QVERIFY(util->isValidFreq("a") == false);
} //bool isValidFreq(const QString &_b);

void tst_Utilities::test_isValidGrid()
{
/*
 * Wikipedia:
    Character pairs encode longitude first, and then latitude.
    The first pair (a field) encodes with base 18 and the letters "A" to "R".
    The second pair (square) encodes with base 10 and the digits "0" to "9".
    The third pair (subsquare) encodes with base 24 and the letters "a" to "x".
    The fourth pair (extended square) encodes with base 10 and the digits "0" to "9".
    The fifth and subsequent pairs are not formally defined, but recycling the third and fourth
      pair algorithms is one possible definition:  BL11bh16oo66

*/
    QVERIFY(util->isValidGrid("A") == false);
    QVERIFY(util->isValidGrid("1") == false);
    QVERIFY(util->isValidGrid("AA") == true);
    QVERIFY(util->isValidGrid("RR") == true);
    QVERIFY(util->isValidGrid("SS") == false);
    QVERIFY(util->isValidGrid("AA00") == true);
    QVERIFY(util->isValidGrid("RR99") == true);
    QVERIFY(util->isValidGrid("AA00aa") == true);
    QVERIFY(util->isValidGrid("RR99xx") == true);
    QVERIFY(util->isValidGrid("RR99xz") == false);
    QVERIFY(util->isValidGrid("AA00xz") == false);
    QVERIFY(util->isValidGrid("AA00aa00") == true);
    QVERIFY(util->isValidGrid("RR99xx99") == true);
    QVERIFY(util->isValidGrid("RR99xz99") == false);
}

void tst_Utilities::test_isValidVUCC()
{
    QVERIFY2(util->isValidVUCCGrids("A") == false, "Error with wrong loc A");
    QVERIFY2(util->isValidVUCCGrids("1") == false, "Error with wrong loc 1");
    QVERIFY2(util->isValidVUCCGrids("AA") == false, "Error with right loc AA");
    QVERIFY2(util->isValidVUCCGrids("SS") == false, "Error with wrong loc SS");
    QVERIFY2(util->isValidVUCCGrids("AA00") == false, "Error with right loc AA00");
    QVERIFY2(util->isValidVUCCGrids("AA00aa") == false, "Error with right loc AA00aa");
    QVERIFY2(util->isValidVUCCGrids("RR99xx") == false, "Error with right loc RR99xx");
    QVERIFY2(util->isValidVUCCGrids("RR99xz") == false, "Error with wrong loc RR99xz");
    QVERIFY2(util->isValidVUCCGrids("AA00xz") == false, "Error with right loc AA00xz");
    QVERIFY2(util->isValidVUCCGrids("AA00aa00") == false, "Error with right loc AA00aa00");

    QVERIFY2(util->isValidVUCCGrids("AA,AA") == true, "Error with right loc AA,AA");
    QVERIFY2(util->isValidVUCCGrids("IN80DE,IN70AA") == true, "Error with right loc AA,AA");
    QVERIFY2(util->isValidVUCCGrids("AA10,AA10") == true, "Error with right loc AA10,AA10");
    QVERIFY2(util->isValidVUCCGrids("AA10,IN10,IN20,IN30") == true, "Error with right loc AA10,AA10");
    QVERIFY2(util->isValidVUCCGrids("AA10,IN10,IN20") == false, "Error with right loc AA10,AA10");
}

void tst_Utilities::test_isValidADIFField()
{   //Test the full ADIF suite
    QVERIFY(util->isValidADIFField("<CALL:4>EA4K") == true);
    QVERIFY(util->isValidADIFField("<CALL:5>EA4K") == false);
}

void tst_Utilities::test_getPrefixFromCall()
{
    // qDebug() << "Returned: " << util->getPrefixFromCall("K1AA");
    QVERIFY2(util->getPrefixFromCall("K1AA") == "K", "K1AA" );
    // qDebug() << "Returned: " << util->getPrefixFromCall("K1AA");
    QVERIFY2(util->getPrefixFromCall("K1A") == "K", "K1A");
    // qDebug() << "Returned: " << util->getPrefixFromCall("K1AA");
    QVERIFY2(util->getPrefixFromCall("G1") == "G", "G1");
    QVERIFY2(util->getPrefixFromCall("I100") == "I", "I100");
    QVERIFY2(util->getPrefixFromCall("K100A") == "K", "K100");
    QVERIFY2(util->getPrefixFromCall("I100KK") == "I", "I100KK");
    // qDebug() << "Returned: " << util->getPrefixFromCall("FB1K");
    QVERIFY2(util->getPrefixFromCall("FB1K") == "F", "FB1K");
    QVERIFY2(util->getPrefixFromCall("E") == QString(), "E");
    // qDebug() << "Returned: " << util->getPrefixFromCall("EA");
    QVERIFY2(util->getPrefixFromCall("EA") == "EA", "EA");
    QVERIFY2(util->getPrefixFromCall("EA4") == "EA", "EA4-EA");
    QVERIFY2(util->getPrefixFromCall("EA4K") == "EA", "EA4K" );
    QVERIFY2(util->getPrefixFromCall("EA4KK") == "EA", "EA4KK" );
    QVERIFY2(util->getPrefixFromCall("EA6") == "EA6", "EA6");
    QVERIFY2(util->getPrefixFromCall("EA6A") == "EA6", "EA6");
    QVERIFY2(util->getPrefixFromCall("2E1AA") == "2E", "2E1AA" );
    QVERIFY2(util->getPrefixFromCall("E73E") == "E7", "E73E");
    QVERIFY2(util->getPrefixFromCall("AM200A") == "AM", "AM200A");
    QVERIFY2(util->getPrefixFromCall("AM4000") == "AM", "AM4000");
    QVERIFY2(util->getPrefixFromCall("VP2EE") == "VP2E", "VP2EE");
    QVERIFY2(util->getPrefixFromCall("VP2ME") == "VP2M", "VP2ME");
    QVERIFY2(util->getPrefixFromCall("VP2VE") == "VP2V", "VP2VE");
    QVERIFY2(util->getPrefixFromCall("VK9AA") == "VK9AA", "VK9AA");
    QVERIFY2(util->getPrefixFromCall("VK9MA") == "VK9M", "VK9MA");
    QVERIFY2(util->getPrefixFromCall("4U2STAYHOME") == "4U2STAYHOME", "4U2STAYHOME");

    QVERIFY2(util->getPrefixFromCall("K1AA", true) == "K1", "K1AA-True" );
    QVERIFY2(util->getPrefixFromCall("K1A", true) == "K1", "K1A-True");
    // qDebug() << Q_FUNC_INFO << ": TESTING: " << util->getPrefixFromCall("G1", true);
    QVERIFY2(util->getPrefixFromCall("G1", true) == "G1", "G1-True");
    QVERIFY2(util->getPrefixFromCall("I100", true) == "I1", "I100-True");
    QVERIFY2(util->getPrefixFromCall("K100A", true) == "K1", "K1-True");
    QVERIFY2(util->getPrefixFromCall("I100KK", true) == "I1", "I100KK-True");
  // qDebug() << Q_FUNC_INFO << ": FB1K: " << util->getPrefixFromCall("FB1K", true);
    QVERIFY2(util->getPrefixFromCall("FB1K", true) == "FB1", "FB1K-True");
    QVERIFY2(util->getPrefixFromCall("E", true) == QString(), "E-True");
    QVERIFY2(util->getPrefixFromCall("EA", true) == "EA", "EA-True");
    // qDebug() << Q_FUNC_INFO << ": EA4: " << util->getPrefixFromCall("EA4", true);
    QVERIFY2(util->getPrefixFromCall("EA4", true) == "EA4", "EA4-True");
    QVERIFY2(util->getPrefixFromCall("EA4K", true) == "EA4", "EA4K-True" );
    QVERIFY2(util->getPrefixFromCall("EA4KK", true) == "EA4", "EA4KK-True" );
    QVERIFY2(util->getPrefixFromCall("EA6", true) == "EA6", "EA6-True");
    QVERIFY2(util->getPrefixFromCall("EA6A", true) == "EA6", "EA6-True");
    QVERIFY2(util->getPrefixFromCall("2E1AA", true) == "2E1", "2E1AA-True" );
    QVERIFY2(util->getPrefixFromCall("E73E", true) == "E73", "E73E-True");
    QVERIFY2(util->getPrefixFromCall("AM200A", true) == "AM2", "AM200A-True");
    QVERIFY2(util->getPrefixFromCall("AM4000", true) == "AM4", "AM4000-True");
    QVERIFY2(util->getPrefixFromCall("VK9XX", true) == "VK9X", "VK9XX-True");
    QVERIFY2(util->getPrefixFromCall("VK9AA", true) == "VK9AA", "VK9AA-True");
    QVERIFY2(util->getPrefixFromCall("VK9MA", true) == "VK9M", "VK9MA-True");
    QVERIFY2(util->getPrefixFromCall("4U2STAYHOME", true) == "4U2STAYHOME", "4U2STAYHOME-True");

    QVERIFY2(util->getAreaNumberFromCall("EA") == -1, "Area call: EA");
    QVERIFY2(util->getAreaNumberFromCall("EA4K") == 4, "Area call: EA4K");
    QVERIFY2(util->getAreaNumberFromCall("EA8K") == 8, "Area call: EA8K");
    QVERIFY2(util->getAreaNumberFromCall("A41A") == 1, "Area call: A41A");
    QVERIFY2(util->getAreaNumberFromCall("2Q0VIM") == 0, "Area call: 2Q0VIM");
    QVERIFY2(util->getAreaNumberFromCall("KL7AA") == 7, "Area call: KL7AA");
    // qVERIFY2(util->getAreaNumberFromCall("EA4K/3") == 3, "Area call: EA4K/3");
    // qVERIFY2(util->getAreaNumberFromCall("I100KK") == 100, "Area call: I100KK");
    // qVERIFY2(util->getAreaNumberFromCall("RP67ST") == 67, "Area call: RP67ST");



}

void tst_Utilities::test_logLevels()
{
    QVERIFY2(util->isValidLogLevel("None"), "None logLevel not detected");
    QVERIFY2(util->isValidLogLevel("None"), "Info logLevel not detected");
    QVERIFY2(util->isValidLogLevel("None"), "Debug logLevel not detected");
    QVERIFY2(!util->isValidLogLevel("Other"), "Other logLevel not detected");
}



void tst_Utilities::test_isValidEmail()
{
    QVERIFY2(util->isValidEmail("test@gmail.com") == true, "test@gmail.com");
    QVERIFY2(util->isValidEmail("test+a@gmail.com") == true, "test+a@gmail.com");
    QVERIFY2(util->isValidEmail("test@gmail.test.com") == true, "test@gmail.test.com");
    QVERIFY2(util->isValidEmail("test@gmail.co") == true, "test@gmail.co");
    QVERIFY2(util->isValidEmail("gmail.com") == false, "gmail.com");
    QVERIFY2(util->isValidEmail("test@gmail.") == false, "test@gmail.");
    QVERIFY2(util->isValidEmail("test@co") == false, "test@co");
    QVERIFY2(util->isValidEmail("gmail") == false, "gmail");
}

void tst_Utilities::test_QSL()
{
  QVERIFY2(util->isValidQSL_Rcvd("Y"), "Y is a valid QSL RCVD");
  QVERIFY2(util->isValidQSL_Rcvd("N"), "N is a valid QSL RCVD");
  QVERIFY2(util->isValidQSL_Rcvd("R"), "R is a valid QSL RCVD");
  QVERIFY2(util->isValidQSL_Rcvd("I"), "I is a valid QSL RCVD");
  QVERIFY2(util->isValidQSL_Rcvd("V"), "V is a valid QSL RCVD");
  QVERIFY2(util->isValidQSL_Rcvd("I"), "Y is a valid QSL RCVD");
  QVERIFY2(!util->isValidQSL_Rcvd("H"), "H is not a valid QSL RCVD");
  QVERIFY2(!util->isValidQSL_Rcvd("K"), "K is not a valid QSL RCVD");
}

QTEST_GUILESS_MAIN(tst_Utilities)

#include "tst_utilities.moc"
