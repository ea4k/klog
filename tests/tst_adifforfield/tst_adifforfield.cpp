/***************************************************************************
                          tst_adifforfield.cpp  -  description
                             -------------------
    begin                : Oct 2021
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
#include "../../src/locator.h"
#include "../../src/klogdefinitions.h"
#include "../../src/adifforfield.h"

class tst_ADIFForField : public QObject
{
    Q_OBJECT

public:
    tst_ADIFForField();
    ~tst_ADIFForField();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_getADIFForQSODate();
    void test_getADIFForCall();
    void test_getADIFForRSTSent();
    void test_getADIFForRSTRcvd();
    void test_getADIFForBand();
    void test_getADIFForMode();
    void test_getADIFForSubMode();
    void test_getADIFForCQz();
    void test_getADIFForITUz();
    void test_getADIFForDXCC();
    void test_getADIFForAddress();
    void test_getADIFForAge();
    void test_getADIFForCNTY();
    void test_getADIFForComment();
    void test_getADIFForA_Index();
    void test_getADIFForAnt_az();
    void test_getADIFForAnt_el();
    void test_getADIFForAnt_path();
    void test_getADIFForARRL_sect();

private:
    ADIFForField *adifForField;

};

tst_ADIFForField::tst_ADIFForField()
{
    //util = new Utilities;
    adifForField = new ADIFForField();
}

tst_ADIFForField::~tst_ADIFForField()
{

}

void tst_ADIFForField::initTestCase()
{

}

void tst_ADIFForField::cleanupTestCase()
{

}

void tst_ADIFForField::test_Constructor()
{
    //QVERIFY(util->getVersion() == "0.0");
}


void tst_ADIFForField::test_getADIFForQSODate()
{
    //yyyy-MM-dd hh:mm:ss
    qDebug() << adifForField->getADIFForQSODate ("2021-10-17 12:52:12");
    QVERIFY2(adifForField->getADIFForQSODate ("2021-10-17 12:52:12") == "<QSO_DATE:8>20211017 <TIME_ON:6>125212 ", "Date not properly exported");
    QVERIFY2(adifForField->getADIFForQSODate ("2021-10-17 12:52:12", ModeEQSL) == "<QSO_DATE:8>20211017 <TIME_ON:4>1252 ", "Date not properly exported - EM");
    QVERIFY2(adifForField->getADIFForQSODate ("BAD date") == QString(), "Bad Date not properly exported");
    QVERIFY2(adifForField->getADIFForQSODate ("BAD date", ModeEQSL) == QString(), "Bad Date not properly exported - EM");
}

void tst_ADIFForField::test_getADIFForCall()
{
  QVERIFY2(adifForField->getADIFForCall ("EA4K") == "<CALL:4>EA4K ", "Call not properly exported");
  QVERIFY2(adifForField->getADIFForCall ("") == QString(), "Empty call not properly exported");
}
void tst_ADIFForField::test_getADIFForRSTSent()
{
    QVERIFY2(adifForField->getADIFForRSTSent ("599") == "<RST_SENT:3>599 ", "RSTSent not properly exported");
    QVERIFY2(adifForField->getADIFForRSTSent ("59") == "<RST_SENT:2>59 ", "RSTSent not properly exported");
    QVERIFY2(adifForField->getADIFForRSTSent ("") == QString(), "Empty RSTSent not properly exported");
}

void tst_ADIFForField::test_getADIFForRSTRcvd()
{
    QVERIFY2(adifForField->getADIFForRSTRcvd ("599") == "<RST_RCVD:3>599 ", "RSTRcvd not properly exported");
    QVERIFY2(adifForField->getADIFForRSTRcvd ("59") == "<RST_RCVD:2>59 ", "RSTRcvd not properly exported");
    QVERIFY2(adifForField->getADIFForRSTRcvd ("") == QString(), "Empty RSTSent not properly exported");
}

void tst_ADIFForField::test_getADIFForBand()
{
    QVERIFY2(adifForField->getADIFForBand ("40M") == "<BAND:3>40M ", "Band not properly exported 40M");
    QVERIFY2(adifForField->getADIFForBand ("2M") == "<BAND:2>2M ", "Band not properly exported 2M");
    QVERIFY2(adifForField->getADIFForBand ("") == QString(), "Empty Band not properly exported");
}

void tst_ADIFForField::test_getADIFForMode()
{
    QVERIFY2(adifForField->getADIFForMode ("SSB") == "<MODE:3>SSB ", "Band not properly exported SSB");
    QVERIFY2(adifForField->getADIFForMode ("CW") == "<MODE:2>CW ", "Band not properly exported CW");
    QVERIFY2(adifForField->getADIFForMode ("") == QString(), "Empty Band not properly exported");
}

void tst_ADIFForField::test_getADIFForSubMode()
{
    QVERIFY2(adifForField->getADIFForSubMode ("USB") == "<SUBMODE:3>USB ", "Band not properly exported SSB");
    QVERIFY2(adifForField->getADIFForSubMode ("OLIVIA 4/125") == "<SUBMODE:12>OLIVIA 4/125 ", "Band not properly exported CW");
    QVERIFY2(adifForField->getADIFForSubMode ("") == QString(), "Empty Band not properly exported");
}
void tst_ADIFForField::test_getADIFForCQz()
{
    QVERIFY2(adifForField->getADIFForCQz ("14") == "<CQZ:2>14 ", "CQz not properly exported 14");
    QVERIFY2(adifForField->getADIFForCQz ("50") == QString(), "Bad CQz not properly exported 50");
    QVERIFY2(adifForField->getADIFForCQz ("CW") == QString(), "Bad CQ not properly exported CW");
    QVERIFY2(adifForField->getADIFForCQz ("") == QString(), "Empty CQ not properly exported");
}

void tst_ADIFForField::test_getADIFForITUz()
{
    QVERIFY2(adifForField->getADIFForITUz ("37") == "<ITUZ:2>37 ", "ITUz not properly exported 37");
    QVERIFY2(adifForField->getADIFForITUz ("97") == QString(), "Bad ITUz not properly exported 50");
    QVERIFY2(adifForField->getADIFForITUz ("CW") == QString(), "Bad String ITUz not properly exported CW");
    QVERIFY2(adifForField->getADIFForITUz ("") == QString(), "Empty ITUz not properly exported");
}
void tst_ADIFForField::test_getADIFForDXCC()
{
    QVERIFY2(adifForField->getADIFForDXCC ("37") == "<DXCC:2>37 ", "DXCC not properly exported 37");
    QVERIFY2(adifForField->getADIFForDXCC ("1234") == QString(), "Bad DXCC not properly exported 1234");
    QVERIFY2(adifForField->getADIFForDXCC ("CW") == QString(), "Bad String DXCC not properly exported CW");
    QVERIFY2(adifForField->getADIFForDXCC ("") == QString(), "Empty DXCC not properly exported");
}

void tst_ADIFForField::test_getADIFForAddress()
{
    QVERIFY2(adifForField->getADIFForAddress ("My address") == "<ADDRESS:10>My address ", "DXCC not properly exported 37");
    QVERIFY2(adifForField->getADIFForAddress ("") == QString(), "Empty ADDRESS not properly exported");
}

void tst_ADIFForField::test_getADIFForAge()
{
    QVERIFY2(adifForField->getADIFForAge ("37") == "<AGE:2>37 ", "Age not properly exported 37");
    QVERIFY2(adifForField->getADIFForAge ("500") == QString(), "Bad Age not properly exported 50");
    qDebug() << Q_FUNC_INFO << ": " << adifForField->getADIFForAge ("CW");
    QVERIFY2(adifForField->getADIFForAge ("CW") == QString(), "Bad String Age not properly exported CW");
    QVERIFY2(adifForField->getADIFForAge ("") == QString(), "Empty Age not properly exported");
}

void tst_ADIFForField::test_getADIFForCNTY(){}
void tst_ADIFForField::test_getADIFForComment(){}
void tst_ADIFForField::test_getADIFForA_Index(){}
void tst_ADIFForField::test_getADIFForAnt_az(){}
void tst_ADIFForField::test_getADIFForAnt_el(){}
void tst_ADIFForField::test_getADIFForAnt_path(){}
void tst_ADIFForField::test_getADIFForARRL_sect(){}

QTEST_APPLESS_MAIN(tst_ADIFForField)

#include "tst_adifforfield.moc"
