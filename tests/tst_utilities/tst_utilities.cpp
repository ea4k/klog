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
    void test_isAPrefix();
    void test_boolToCharToSQLite();
    void test_boolToQString();
    void test_getGlobalAgent();
    void test_getHomeDir();
    void test_getNormalizedDXCCValue();
    void test_isValidCall();
    void test_isValidFreq();
    void test_isValidGrid();
    void test_isValidADIFField();
    void test_logLevels();
    void test_isValidSimpleCall();
    void test_getPrefixFromCall();
    void test_getMainCallFromComplexCall();

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
    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    util->setLogLevel(Devel);
}

tst_Utilities::~tst_Utilities()
{

}

void tst_Utilities::initTestCase()
{
    //qDebug("Called before everything else.");
}

void tst_Utilities::cleanupTestCase()
{
    //qDebug("Called after last test.");
}

void tst_Utilities::test_Constructor()
{
    QVERIFY(util->getVersion() == "0.0");
}

void tst_Utilities::test_isAPrefix()
{
    /*
    QVERIFY2(util->isAPrefix("EA") , "EA");
    QVERIFY2(util->isAPrefix("EA1"), "EA1");
    QVERIFY2(util->isAPrefix("EA6") , "EA6");
    //qDebug() << Q_FUNC_INFO <<": --" << QString::number(util->isAPrefix("K1") );
    QVERIFY2(util->isAPrefix("K"), "K");
    QVERIFY2(util->isAPrefix("K1") , "K1");
    //qDebug() << QString::number(util->isAPrefix("CE0X") );
    QVERIFY2(util->isAPrefix("CE0X"), "CE0X");
    QVERIFY2(util->isAPrefix("VK9M"), "VK0M");
    QVERIFY2(util->isAPrefix("PY0F"), "PY0F");
    QVERIFY2(util->isAPrefix("T30"), "T30");
    */
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

void tst_Utilities::test_isValidCall()
{
    //Amateur and experimental stations19.68
    //1) –one  character
    // (provided  that  it  is  the  letter  B,  F,  G,  I,  K,  M,  N,  R  or  W)
    // and  a  single  digit,
    // followed  by  a  group  of  not  more  than  four  characters,
    // the last of which shall be a letter,
    // We will check all formats shown here: https://en.wikipedia.org/wiki/Amateur_radio_call_signs
    util->setCallValidation(true);
    //qDebug() << Q_FUNC_INFO << " 1 Letter" << endl;
    QVERIFY2(util->isValidCall("B1A"), "B1A");
    QVERIFY2(util->isValidCall("B1AA"), "B1AA");
    QVERIFY2(util->isValidCall("B1AAA"), "B1AAA");
    QVERIFY2(util->isValidCall("B1AAAA"), "B1AAAA");
    QVERIFY2(!util->isValidCall("B11"), "B11");

    //2) or–two characters and a single digit,
    // followed by a group of not more than four characters, the last of which shall be a letter.
    //qDebug() << Q_FUNC_INFO << " 2 Letters";

    QVERIFY2(util->isValidCall("EA4K"), "EA4K");
    QVERIFY2(util->isValidCall("EA4KK"), "EA4KK");
    QVERIFY2(util->isValidCall("EA4KKK"), "EA4KKK");
    QVERIFY2(util->isValidCall("EA4KKKK"), "EA4KKKK");
    QVERIFY2(util->isValidCall("AM500MMM"), "AM500MMM");
    QVERIFY2(util->isValidCall("2E1A"), "2E1A");
    QVERIFY2(util->isValidCall("2E1AA"), "2E1AA" );
    QVERIFY2(util->isValidCall("E33E"), "E33E");
    QVERIFY2(util->isValidCall("E73E"), "E73E");
    QVERIFY2(util->isValidCall("EA5666K"), "EA5666K");
    QVERIFY2(util->isValidCall("VK0M/ZL4DB/P"), "VK0M/ZL4DB/P");
    QVERIFY2(util->isValidCall("EA5/DL4EA"), "EA5/DL4EA");
    QVERIFY2(util->isValidCall("DL4EA/EA5"), "DL4EA/EA5");

    QVERIFY2(util->isValidCall("K4X"), "K4X");
    QVERIFY2(util->isValidCall("B2AA"), "B2AA");
    QVERIFY2(util->isValidCall("N2ASD"), "N2ASD");
    QVERIFY2(util->isValidCall("A22A"), "A22A");
    QVERIFY2(util->isValidCall("I20000X"), "I20000X");
    QVERIFY2(util->isValidCall("4X4AAA"), "4X4AAA");
    QVERIFY2(util->isValidCall("3DA0RS"), "3DA0RS");
    QVERIFY2(util->isValidCall("VP2EE"), "VP2EE");
    QVERIFY2(util->isValidCall("EA6A"), "EA6A");

    QVERIFY2(util->isValidCall("AM200A"), "AM200A");
    QVERIFY2(util->isValidCall("VK9AA"), "VK9AA"); // This is a full special call
    QVERIFY2(util->isValidCall("VK9MA"), "VK9MA");
    QVERIFY2(util->isValidCall("4U2STAYHOME") , "4U2STAYHOME");
    QVERIFY2(util->isValidCall("4U1A") , "4U1A");
    // 5(WRC-03)19.68A1A)   On special occasions, for temporary use, administrations may authorize
    // use of call signs with more than the four characters referred to in No. 19.68.(WRC-03

    //qDebug() << Q_FUNC_INFO << " Complex";
    QVERIFY2(util->isValidCall("EA4K/P"), "EA4K/P");
    //qDebug() << Q_FUNC_INFO << " Complex-1";
    QVERIFY2(util->isValidCall("K/EA4K/P"), "EA4K/K");
    //qDebug() << Q_FUNC_INFO << " Complex-2";
    QVERIFY2(util->isValidCall("EA4K/F"), "EA4K/F");
    //qDebug() << Q_FUNC_INFO << " Complex-3";
    QVERIFY2(util->isValidCall("EA4K/1"), "EA4K/1");
    QVERIFY2(util->isValidCall("EA4K/K1"), "EA4K/K1");
    QVERIFY2(util->isValidCall("K1/EA4K"), "K1/EA4K");
    //qDebug() << Q_FUNC_INFO << " Complex-6";
    QVERIFY2(util->isValidCall("K/EA4K"), "K/EA4K");
    //qDebug() << Q_FUNC_INFO << " Complex-7";
    // TODO: FIX the isValidCall to cover this case
    //QVERIFY(util->isValidCall("1/EA4K") == false);

    //qDebug() << Q_FUNC_INFO << " Wrong calls";
    QVERIFY2(!util->isValidCall("G1"), "G1");
    QVERIFY2(!util->isValidCall("I100"), "I100");
    QVERIFY2(util->isValidCall("K100A"), "K1");
    QVERIFY2(util->isValidCall("I100KK"), "I100KK");
    QVERIFY2(util->isValidCall("FB1K") , "FB1K");
    QVERIFY2(!util->isValidCall("E"), "E");
    QVERIFY2(!util->isValidCall("EA"), "EA");
    QVERIFY2(!util->isValidCall("EA4"), "EA4-EA");

    QVERIFY2(!util->isValidCall("-"), "-");
    QVERIFY2(!util->isValidCall("EAK4"), "EAK4");
    QVERIFY2(!util->isValidCall("QQQ/EA4K"), "QQQ/EA4K");
    QVERIFY2(!util->isValidCall("EA/"), "EA/");

    QVERIFY2(!util->isValidCall("DL4EA/"), "DL4EA/");
    //QVERIFY2(util->isValidCall("RM1O"), "Should be true: RM1O");

    util->setCallValidation (false);
    QVERIFY2(util->isValidCall("EA"), "Should be true: EA");
    QVERIFY2(util->isValidCall("EA4"), "Should be true: EA4");
    QVERIFY2(util->isValidCall("-"), "Should be true: EAK4");
    QVERIFY2(util->isValidCall("QQQ/EA4K"), "Should be true: QQQ/EA4K");
    util->setCallValidation (true);
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

void tst_Utilities::test_isValidADIFField()
{   //Test the full ADIF suite
    QVERIFY(util->isValidADIFField("<CALL:4>EA4K") == true);
    QVERIFY(util->isValidADIFField("<CALL:5>EA4K") == false);
}

void tst_Utilities::test_getPrefixFromCall()
{
    //qDebug() << "Returned: " << util->getPrefixFromCall("K1AA");
    QVERIFY2(util->getPrefixFromCall("K1AA") == "K", "K1AA" );
    //qDebug() << "Returned: " << util->getPrefixFromCall("K1AA");
    QVERIFY2(util->getPrefixFromCall("K1A") == "K", "K1A");
    //qDebug() << "Returned: " << util->getPrefixFromCall("K1AA");
    QVERIFY2(util->getPrefixFromCall("G1") == "G", "G1");
    QVERIFY2(util->getPrefixFromCall("I100") == "I", "I100");
    QVERIFY2(util->getPrefixFromCall("K100A") == "K", "K100");
    QVERIFY2(util->getPrefixFromCall("I100KK") == "I", "I100KK");
    //qDebug() << "Returned: " << util->getPrefixFromCall("FB1K");
    QVERIFY2(util->getPrefixFromCall("FB1K") == "F", "FB1K");
    QVERIFY2(util->getPrefixFromCall("E") == QString(), "E");
    //qDebug() << "Returned: " << util->getPrefixFromCall("EA");
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
    QVERIFY2(util->getPrefixFromCall("G1", true) == "G1", "G1-True");
    QVERIFY2(util->getPrefixFromCall("I100", true) == "I1", "I100-True");
    QVERIFY2(util->getPrefixFromCall("K100A", true) == "K1", "K1-True");
    QVERIFY2(util->getPrefixFromCall("I100KK", true) == "I1", "I100KK-True");
    //qDebug() << Q_FUNC_INFO << ": FB1K: " << util->getPrefixFromCall("FB1K", true);
    QVERIFY2(util->getPrefixFromCall("FB1K", true) == "FB1", "FB1K-True");
    QVERIFY2(util->getPrefixFromCall("E", true) == QString(), "E-True");
    QVERIFY2(util->getPrefixFromCall("EA", true) == "EA", "EA-True");
    //qDebug() << Q_FUNC_INFO << ": EA4: " << util->getPrefixFromCall("EA4", true);
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
}

void tst_Utilities::test_logLevels()
{
    QVERIFY2(util->isValidLogLevel("None"), "None logLevel not detected");
    QVERIFY2(util->isValidLogLevel("None"), "Info logLevel not detected");
    QVERIFY2(util->isValidLogLevel("None"), "Debug logLevel not detected");
    QVERIFY2(!util->isValidLogLevel("Other"), "Other logLevel not detected");
}

void tst_Utilities::test_isValidSimpleCall()
{
    QVERIFY2(util->isValidSimpleCall("B1A"), "B1A");
    QVERIFY2(util->isValidSimpleCall("B1AA"), "B1AA");
    QVERIFY2(util->isValidSimpleCall("B1AAA"), "B1AAA");
    QVERIFY2(util->isValidSimpleCall("B1AAAA"), "B1AAAA");
    QVERIFY2(!util->isValidSimpleCall("B11"), "B11");

    //2) or–two characters and a single digit,
    // followed by a group of not more than four characters, the last of which shall be a letter.

    QVERIFY2(util->isValidSimpleCall("EA4K"), "EA4K");
    QVERIFY2(util->isValidSimpleCall("EA4KK"), "EA4KK");
    QVERIFY2(util->isValidSimpleCall("EA4KKK"), "EA4KKK");
    QVERIFY2(util->isValidSimpleCall("EA4KKKK"), "EA4KKKK");
    QVERIFY2(util->isValidSimpleCall("AM500MMM"), "AM500MMM");
    QVERIFY2(util->isValidSimpleCall("2E1A"), "2E1A");
    QVERIFY2(util->isValidSimpleCall("2E1AA"), "2E1AA" );
    QVERIFY2(util->isValidSimpleCall("E33E"), "E33E");
    QVERIFY2(util->isValidSimpleCall("E73E"), "E73E");
    QVERIFY2(util->isValidSimpleCall("EA5666K"), "EA5666K");
    QVERIFY2(!util->isValidSimpleCall("VK0M/ZL4DB/P"), "VK0M/ZL4DB/P");

    QVERIFY2(util->isValidSimpleCall("K4X"), "K4X");
    QVERIFY2(util->isValidSimpleCall("B2AA"), "B2AA");
    QVERIFY2(util->isValidSimpleCall("N2ASD"), "N2ASD");
    QVERIFY2(util->isValidSimpleCall("A22A"), "A22A");
    QVERIFY2(util->isValidSimpleCall("I20000X"), "I20000X");
    QVERIFY2(util->isValidSimpleCall("4X4AAA"), "4X4AAA");
    QVERIFY2(util->isValidSimpleCall("3DA0RS"), "3DA0RS");
    QVERIFY2(util->isValidSimpleCall("VP2EE"), "VP2EE");
    QVERIFY2(util->isValidSimpleCall("EA6A"), "EA6A");

    QVERIFY2(util->isValidSimpleCall("AM200A"), "AM200A");
    QVERIFY2(util->isValidSimpleCall("VK9AA"), "VK9AA");
    QVERIFY2(util->isValidSimpleCall("VK9MA"), "VK9MA");
    QVERIFY2(util->isValidSimpleCall("4U2STAYHOME") , "4U2STAYHOME");

    // 5(WRC-03)19.68A1A)   On special occasions, for temporary use, administrations may authorize
    // use of call signs with more than the four characters referred to in No. 19.68.(WRC-03

    //qDebug() << Q_FUNC_INFO << " Complex";
    QVERIFY2(!util->isValidSimpleCall("EA4K/P"), "EA4K/P");
    //qDebug() << Q_FUNC_INFO << " Complex-1";
    QVERIFY2(!util->isValidSimpleCall("K/EA4K/P"), "EA4K/K");
    //qDebug() << Q_FUNC_INFO << " Complex-2";
    QVERIFY2(!util->isValidSimpleCall("K1/EA4K"), "K1/EA4K");
    QVERIFY2(!util->isValidSimpleCall("K/EA4K"), "K/EA4K");

    QVERIFY2(!util->isValidSimpleCall("G1"), "G1");
    QVERIFY2(!util->isValidSimpleCall("I100"), "I100");
    QVERIFY2(util->isValidSimpleCall("K100A"), "K1");
    QVERIFY2(util->isValidSimpleCall("I100KK"), "I100KK");
    QVERIFY2(util->isValidSimpleCall("FB1K") , "FB1K");
    QVERIFY2(!util->isValidSimpleCall("E"), "E");
    QVERIFY2(!util->isValidSimpleCall("EA"), "EA");
    QVERIFY2(!util->isValidSimpleCall("EA4"), "EA4-EA");

    QVERIFY2(!util->isValidSimpleCall("-"), "-");
    QVERIFY2(!util->isValidSimpleCall("EAK4"), "EAK4");
    QVERIFY2(!util->isValidSimpleCall("QQQ/EA4K"), "QQQ/EA4K");

    QVERIFY2(!util->isValidSimpleCall("EA6"), "EA6 is just a prefix");
    QVERIFY2(util->isValidSimpleCall("EE6AAA"), "EE6AAA");
    QVERIFY2(!util->isValidSimpleCall("VP2E"), "VP2E is just a prefix");
    QVERIFY2(util->isValidSimpleCall("EA4K"), "EA4K");
    QVERIFY2(!util->isValidSimpleCall("EA4K/p"), "EA4K/p");
    QVERIFY2(!util->isValidSimpleCall("VK9M"), "VK9M");
    QVERIFY2(util->isValidSimpleCall("VK9MA"), "VK9MA");
    QVERIFY2(!util->isValidSimpleCall("XXXX"), "XXXX");
}

void tst_Utilities::test_getMainCallFromComplexCall()
{
    QVERIFY2(util->getMainCallFromComplexCall("EA4K")=="EA4K", "EA4K");
    QVERIFY2(util->getMainCallFromComplexCall("EA4K/P")=="EA4K", "EA4K/P");
    QVERIFY2(util->getMainCallFromComplexCall("EA4K/MM")=="EA4K", "EA4K/MM");
    QVERIFY2(util->getMainCallFromComplexCall("MM/EA4K")=="MM", "MM/EA4K");
    QVERIFY2(util->getMainCallFromComplexCall("EA4K/F")=="F", "EA4K/F");
    QVERIFY2(util->getMainCallFromComplexCall("EA4K/1")=="EA4K", "EA4K/1");
    QVERIFY2(util->getMainCallFromComplexCall("VK9/EA4K")=="VK9", "VK9/EA4K");
}


QTEST_APPLESS_MAIN(tst_Utilities)

#include "tst_utilities.moc"
