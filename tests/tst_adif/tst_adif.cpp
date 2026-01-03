/***************************************************************************
                          tst_Adif.h  -  description
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
#include "../../src/adif.h"
#include "../../src/callsign.h"

class tst_Adif : public QObject
{
    Q_OBJECT

public:
    tst_Adif();
    ~tst_Adif();

private slots:
    void initTestCase(); //will be called before the first test function is executed.

    //void initTestCase_data(); // will be called to create a global test data table.
    void cleanupTestCase(); //will be called after the last test function was executed.
    void test_getADIFField();
    void test_Dates();
    void test_POTA();
    void test_WWFF();
    void test_setPair();

private:
    Adif *adif;
};

tst_Adif::tst_Adif()
{
    adif = new Adif(Q_FUNC_INFO);
}

tst_Adif::~tst_Adif(){}

void tst_Adif::initTestCase()
{
    //qDebug("Called before everything else.");
}

void tst_Adif::cleanupTestCase()
{
    //qDebug("Called after last test.");
}

void tst_Adif::test_getADIFField()
{ //QString Utilities::getADIFField(const QString &_fieldName, const QString &_data)
   QVERIFY2(adif->getADIFField("CALL", "EA4K")=="<CALL:4>EA4K ", "Call");
   QVERIFY2(adif->getADIFField("CALLA", "EA4K")==QString(), "Non Valid");
   QVERIFY2(adif->isValidPower(10), "10 Not valid power");
  //QVERIFY2(!adif->isValidPower(10W), "10W Not valid power");
}

void tst_Adif::test_Dates()
{
    QVERIFY2(adif->getADIFDateStringFromLoTWDateTime("2025-08-01 20:15:10")=="20250801", "LoTW Date is wrong");

}

void tst_Adif::test_POTA()
{
    QVERIFY2(adif->isValidPOTA("ES-0150"), "ES-0150");
    QVERIFY2(adif->isValidPOTA("US-0001"), "US-0001");
    QVERIFY2(adif->isValidPOTA("US-2133"), "US-2133");
    QVERIFY2(adif->isValidPOTA("FR-13219"), "FR-13219");
    QVERIFY2(adif->isValidPOTA("gb-5662"), "gb-56629");
}

void tst_Adif::test_WWFF()
{
    QVERIFY2(adif->isValidPOTA("KFF-4655"), "KFF-4655");
    //QVERIFY2(adif->isValidPOTA("3DAFF-0002 "), "3DAFF-0002");
}

void tst_Adif::test_setPair()
{
    //qDebug() << Q_FUNC_INFO;
    /*
struct ADIFField {
    QString field;
    QString value;
    bool valid;
    //QChar type;
};
     */
    ADIFField pair;
    pair = adif->setPair("<QSO_DATE:8>20190528");
    QVERIFY2(pair.field == "QSO_DATE", "Pair-field-1");
    QVERIFY2(pair.value == "20190528", "Pair-value-1");
    QVERIFY2(pair.valid == true, "Pair-bool-1");

    pair = adif->setPair("<APP_LoTW_RXQSO:19>2019-05-28 21:23:13 // QSO record inserted/modified at LoTW");
    //qDebug() << Q_FUNC_INFO << " - " << pair.field;
    //qDebug() << Q_FUNC_INFO << " - " << pair.value;
    QVERIFY2(pair.field == "APP_LoTW_RXQSO", "Pair-field-2");
    QVERIFY2(pair.value == "2019-05-28 21:23:13", "Pair-value-2");
    QVERIFY2(pair.valid == true, "Pair-bool-2");


}

QTEST_APPLESS_MAIN(tst_Adif)

#include "tst_adif.moc"

