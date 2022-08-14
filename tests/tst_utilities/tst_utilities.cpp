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
    void test_getPrefixFullFromCall();
    void test_getPrefixFromCall();
    void test_logLevels();
    void test_newPrefixes();

private:
  Utilities *util;

};

tst_Utilities::tst_Utilities()
{
    util = new Utilities(Q_FUNC_INFO);
    util->init();
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
    QVERIFY2(util->isAPrefix("EA") == 2, "EA");
    QVERIFY2(util->isAPrefix("EA1") == 2, "EA1");
    QVERIFY2(util->isAPrefix("EA6") == 3, "EA6");
    QVERIFY2(util->isAPrefix("K") == 1, "K");
    QVERIFY2(util->isAPrefix("K1") == 1, "K1");
    QVERIFY2(util->isAPrefix("CE0X") == 4, "CE0X");
    QVERIFY2(util->isAPrefix("T30") == 3, "T30");
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
    // TODO: FIX the isValidCall to cover these cases
    //QVERIFY(util->isValidCall("B1A1") == false);
    //QVERIFY(util->isValidCall("B1AA1") == false);
    //QVERIFY(util->isValidCall("B1AAA1") == false);

    // TODO: FIX the isValidCall to cover this case
    //QVERIFY(util->isValidCall("E1A") == false);
    //QVERIFY(util->isValidCall("E1AA") == false);
    //QVERIFY(util->isValidCall("E1AAA") == false);
    //QVERIFY(util->isValidCall("E1AAAA") == false);
    //QVERIFY(util->isValidCall("E1AAAAA") == false);

    //2) or–two characters and a single digit,
    // followed by a group of not more than four characters, the last of which shall be a letter.
    //qDebug() << Q_FUNC_INFO << " 2 Letters";

    QVERIFY2(util->isValidCall("EA4K"), "EA4K");
    QVERIFY2(util->isValidCall("EA4KK"), "EA4KK");
    QVERIFY2(util->isValidCall("EA4KKK"), "EA4KKK");
    QVERIFY2(util->isValidCall("EA4KKKK"), "EA4KKKK");
    QVERIFY2(util->isValidCall("AM500MMM"), "AM500MMM");
    QVERIFY2(util->isValidCall("2E1A"), "2E1A");
    //QVERIFY2(util->isValidCall("E33E"), "E33E");
    QVERIFY2(util->isValidCall("EA5666K"), "EA5666K");

    QVERIFY2(util->isValidCall("K4X"), "K4X");
    QVERIFY2(util->isValidCall("B2AA"), "B2AA");
    QVERIFY2(util->isValidCall("N2ASD"), "N2ASD");
    QVERIFY2(util->isValidCall("A22A"), "A22A");
    QVERIFY2(util->isValidCall("I20000X"), "I20000X");
    QVERIFY2(util->isValidCall("4X4AAA"), "4X4AAA");
    QVERIFY2(util->isValidCall("3DA0RS"), "3DA0RS");
    QVERIFY2(util->isValidCall("GB75RD"), "GB75RD");

    //QVERIFY2(util->isValidCall("D9K"), "D9K");



    // 5(WRC-03)19.68A1A)   On special occasions, for temporary use, administrations may authorize
    // use of call signs with more than the four characters referred to in No. 19.68.(WRC-03
    /*
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
    QVERIFY2(!util->isValidCall("EA"), "EA");
    QVERIFY2(!util->isValidCall("EA4"), "EA4");
    QVERIFY2(!util->isValidCall("-"), "-");
    QVERIFY2(!util->isValidCall("EAK4"), "EAK4");
    QVERIFY2(!util->isValidCall("QQQ/EA4K"), "QQQ/EA4K");

    util->setCallValidation (false);
    QVERIFY2(util->isValidCall("EA"), "Should be true: EA");
    QVERIFY2(util->isValidCall("EA4"), "Should be true: EA4");
    QVERIFY2(util->isValidCall("-"), "Should be true: EAK4");
    QVERIFY2(util->isValidCall("EA4K"), "Should be true: QQQ/EA4K");
    util->setCallValidation (true);
*/
}

void tst_Utilities::test_getPrefixFullFromCall()
{


    QVERIFY2(util->getPrefixFullFromCall("K1AA") == "K1", "Wrong prefix 1" );
    QVERIFY2(util->getPrefixFullFromCall("K1A") == "K1", "K1");
    QVERIFY2(util->getPrefixFullFromCall("G1") == "G1", "G1");
    QVERIFY2(util->getPrefixFullFromCall("I100") == "I100", "I100");
    QVERIFY2(util->getPrefixFullFromCall("EA4K") == "EA4", "Wrong prefix 2" );
    QVERIFY2(util->getPrefixFullFromCall("2E1AA") == "2E1", "Wrong prefix 2 Numb" );
    QVERIFY2(util->getPrefixFullFromCall("E73E") == "E73", "Wrong prefix 2 Letter/Numb");
    QVERIFY2(util->getPrefixFullFromCall("AM200A") == "AM200", "Wrong prefix 2 Letter/Numb");
    QVERIFY2(util->getPrefixFullFromCall("EA4K") == "EA4", "Wrong prefix on complex 1" );
    QVERIFY2(util->getPrefixFullFromCall("VP2EA") == "VP2E", "Wrong prefix on VP2E" );
    QVERIFY2(util->getPrefixFullFromCall("EA6") == "EA6", "EA6");
    QVERIFY2(util->getPrefixFullFromCall("EA6A") == "EA6", "EA6");
    QVERIFY2(util->getPrefixFullFromCall("EA4") == "EA4", "EA4-EA");
    QVERIFY2(util->getPrefixFullFromCall("MB1") == "MB1", "MB1-MB");
    QVERIFY2(util->getPrefixFullFromCall("AM4000") == "AM4000", "AM4000");
    QVERIFY2(util->getPrefixFullFromCall("B100A") == "B100", "B1");
    QVERIFY2(util->getPrefixFullFromCall("FB1K") == "FB1", "FB1-FB");
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
    QVERIFY2(util->getPrefixFromCall("K1AA") == "K", "Wrong prefix 1" );
    QVERIFY2(util->getPrefixFromCall("K1A") == "K", "K1");
    QVERIFY2(util->getPrefixFromCall("G1") == "G", "G1");
    QVERIFY2(util->getPrefixFromCall("I100") == "I", "I1");
    QVERIFY2(util->getPrefixFromCall("EA4K") == "EA", "Wrong prefix 2" );
    QVERIFY2(util->getPrefixFromCall("2E1AA") == "2E", "Wrong prefix 2 Numb" );
    QVERIFY2(util->getPrefixFromCall("E73E") == "E7", "Wrong prefix 2 Letter/Numb");
    QVERIFY2(util->getPrefixFromCall("AM200A") == "AM", "Wrong prefix 2 Letter/Numb");
    QVERIFY2(util->getPrefixFromCall("EA4K/F") == "F", "Wrong prefix on complex 1" );
    QVERIFY2(util->getPrefixFromCall("F/EA4K") == "F", "Wrong prefix on complex 2" );
    QVERIFY2(util->getPrefixFromCall("EA4K/EA6") == "EA6", "Wrong prefix on complex 3" );
    QVERIFY2(util->getPrefixFromCall("EA6/EA4K") == "EA6", "Wrong prefix on complex 4" );
    QVERIFY2(util->getPrefixFromCall("EA6/EA4K/P") == "EA6", "Wrong prefix on complex 5" );
    QVERIFY2(util->getPrefixFromCall("EA6/EA4K/QRP") == "EA6", "Wrong prefix on complex 6" );
    QVERIFY2(util->getPrefixFromCall("EA4K/VK9X") == "VK9X", "Wrong prefix on complex 7" );
    QVERIFY2(util->getPrefixFromCall("VK9X/EA4K") == "VK9X", "Wrong prefix on complex 8" );
    QVERIFY2(util->getPrefixFromCall("VP2EA") == "VP2E", "Wrong prefix on VP2E" );    
    QVERIFY2(util->getPrefixFromCall("EA6") == "EA6", "EA6");
    QVERIFY2(util->getPrefixFromCall("EA6A") == "EA6", "EA6");
    QVERIFY2(util->getPrefixFromCall("EA4") == "EA", "EA4-EA");
    QVERIFY2(util->getPrefixFromCall("MB1") == "MB", "MB1-MB");
    QVERIFY2(util->getPrefixFromCall("AM4000") == "AM", "KB1-KB");
    QVERIFY2(util->getPrefixFromCall("B100A") == "B", "B1");
    QVERIFY2(util->getPrefixFromCall("FB1K") == "FB", "FB1-FB");
    QVERIFY2(util->getPrefixFromCall("AM4000K") == "AM", "AM4000K-AM");

    //qDebug() << Q_FUNC_INFO << " - END";
}

void tst_Utilities::test_logLevels()
{
    QVERIFY2(util->isValidLogLevel("None"), "None logLevel not detected");
    QVERIFY2(util->isValidLogLevel("None"), "Info logLevel not detected");
    QVERIFY2(util->isValidLogLevel("None"), "Debug logLevel not detected");
    QVERIFY2(!util->isValidLogLevel("Other"), "Other logLevel not detected");
}

void tst_Utilities::test_newPrefixes()
{
    //{'B', 'F', 'G', 'I', 'K', 'M', 'N', 'R', 'W'};
    QVERIFY2(util->isAOneLetterPrefix('B'), "K");
    QVERIFY2(util->isAOneLetterPrefix('F'), "F");
    QVERIFY2(util->isAOneLetterPrefix('G'), "G");
    QVERIFY2(util->isAOneLetterPrefix('I'), "I");
    QVERIFY2(util->isAOneLetterPrefix('K'), "K");
    QVERIFY2(util->isAOneLetterPrefix('M'), "M");
    QVERIFY2(util->isAOneLetterPrefix('N'), "N");
    QVERIFY2(util->isAOneLetterPrefix('R'), "R");
    QVERIFY2(util->isAOneLetterPrefix('W'), "W");
    QVERIFY2(!util->isAOneLetterPrefix('A'), "A");


    //QString Utilities::getPrefixFullFromCall(const QString &_c)
    //qDebug() << Q_FUNC_INFO << QString(" - Test: %1 = %2").arg("EA6").arg(util->startsWithLongPrefix("EA6"));
    QVERIFY2(util->startsWithLongPrefix("EA6") == "EA6", "EA6");
    QVERIFY2(util->startsWithLongPrefix("EA6AA") == "EA6", "EA6AA");
    QVERIFY2(util->startsWithLongPrefix("VK9M") == "VK9M", "VK9M");
    QVERIFY2(util->startsWithLongPrefix("VK9MA") == "VK9M", "VK9MA");
    QVERIFY2(util->startsWithLongPrefix("4U1ITU") == "4U1ITU", "4U1ITU");
    //QString Utilities::getPrefixCountryFromCall(const QString &_c)


}


QTEST_APPLESS_MAIN(tst_Utilities)

#include "tst_utilities.moc"
