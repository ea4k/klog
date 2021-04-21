/***************************************************************************
                          tst_utilities.h  -  description
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
#include "../../src/utilities.h"

class tst_Utilities : public QObject
{
    Q_OBJECT

public:
    tst_Utilities();
    ~tst_Utilities();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_getProgresStepForDialog();
    void test_trueOrFalse();
    void test_boolToCharToSQLite();
    void test_boolToQString();
    void test_getGlobalAgent();
    void test_getHomeDir();
    void test_getNormalizedDXCCValue();
    void test_isValidCall();
    void test_isValidFreq();
    void test_isValidGrid();
    void test_isValidADIFField();
    void test_getPrefixFromCall();



private:
  Utilities *util;

};

tst_Utilities::tst_Utilities()
{
    util = new Utilities;
}

tst_Utilities::~tst_Utilities()
{

}

void tst_Utilities::initTestCase()
{

}

void tst_Utilities::cleanupTestCase()
{

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

void tst_Utilities::test_isValidCall()
{
    //Amateur and experimental stations19.68
    //1) –one  character
    // (provided  that  it  is  the  letter  B,  F,  G,  I,  K,  M,  N,  R  or  W)
    // and  a  single  digit,
    // followed  by  a  group  of  not  more  than  four  characters,
    // the last of which shall be a letter,
    QVERIFY(util->isValidCall("B1A") == true);
    QVERIFY(util->isValidCall("B1AA") == true);
    QVERIFY(util->isValidCall("B1AAA") == true);
    QVERIFY(util->isValidCall("B1AAAA") == true);
    QVERIFY(util->isValidCall("B11") == false);
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
    QVERIFY(util->isValidCall("EA4K") == true);
    QVERIFY(util->isValidCall("EA4KK") == true);
    QVERIFY(util->isValidCall("EA4KKK") == true);
    QVERIFY(util->isValidCall("EA4KKKK") == true);

    QVERIFY(util->isValidCall("EA4K11K") == true);
    QVERIFY(util->isValidCall("EA4K1KK") == true);
    QVERIFY(util->isValidCall("EA4KK1K") == true);
    // TODO: FIX the isValidCall to cover these cases
    //QVERIFY(util->isValidCall("EA4K111") == false);
    //QVERIFY(util->isValidCall("EA4KK11") == false);
    //QVERIFY(util->isValidCall("EA4KKK1") == false);

    // 5(WRC-03)19.68A1A)   On special occasions, for temporary use, administrations may authorize
    // use of call signs with more than the four characters referred to in No. 19.68.(WRC-03

    QVERIFY(util->isValidCall("EA4K") == true);
    QVERIFY(util->isValidCall("EA4K/P") == true);
    QVERIFY(util->isValidCall("EA4K/F") == true);
    QVERIFY(util->isValidCall("EA4K/1") == true);
    QVERIFY(util->isValidCall("EA4K/K1") == true);
    QVERIFY(util->isValidCall("K1/EA4K") == true);
    QVERIFY(util->isValidCall("K/EA4K") == true);
    // TODO: FIX the isValidCall to cover this case
    //QVERIFY(util->isValidCall("1/EA4K") == false);
    QVERIFY(util->isValidCall("EA") == false);
    QVERIFY(util->isValidCall("EA4") == false);
    QVERIFY(util->isValidCall("-") == false);
    QVERIFY(util->isValidCall("EAK4") == false);

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
    QVERIFY2(util->getPrefixFromCall ("K1AA") == QString("K"), "Wrong prefix 1" );
    QVERIFY2(util->getPrefixFromCall ("EA4K") == QString("EA"), "Wrong prefix 2" );
    QVERIFY2(util->getPrefixFromCall ("2E1AA") == QString("2E"), "Wrong prefix 2 Numb" );
    QVERIFY2(util->getPrefixFromCall ("E73E") == QString("E7"), "Wrong prefix 2 Letter/Numb");
    QVERIFY2(util->getPrefixFromCall ("AM200A") == QString("AM"), "Wrong prefix 2 Letter/Numb");

}

QTEST_APPLESS_MAIN(tst_Utilities)

#include "tst_utilities.moc"
