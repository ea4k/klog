/***************************************************************************
                          tst_Frequency.h  -  description
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
#include "../../src/frequency.h"
#include "../../src/klogdefinitions.h"
#include "QtTest/qtestcase.h"


class tst_Frequency : public QObject
{
    Q_OBJECT

public:
    tst_Frequency();
    ~tst_Frequency();

private slots:
    void initTestCase(); //will be called before the first test function is executed.
    //void initTestCase_data(); // will be called to create a global test data table.
    void cleanupTestCase(); //will be called after the last test function was executed.
    void test_Constructors();
    void test_Basic();
    void test_toDouble();
    void test_toQString();


private:
    Frequency *freq;
    Frequency *freq2;
};

tst_Frequency::tst_Frequency()
{
    freq = new Frequency();
    freq2 = new Frequency();
}

tst_Frequency::~tst_Frequency(){}

void tst_Frequency::initTestCase()
{
    //qDebug("Called before everything else.");
}

void tst_Frequency::cleanupTestCase()
{
    //qDebug("Called after last test.");
}

void tst_Frequency::test_Constructors()
{
    Frequency freq3(freq);
    QVERIFY2(qFuzzyCompare(freq->toDouble(),freq3.toDouble()), "Freq assignment (freq) not working");
    Frequency freq4(7.090,MHz);
    QVERIFY2(qFuzzyCompare(freq4.toDouble(),(double)7.090), "Freq assignment (double,unit) not working");
    Frequency freq5(7.091);
    QVERIFY2(qFuzzyCompare(freq5.toDouble(),(double)7.091), "Freq assignment (double) not working");
}

void tst_Frequency::test_Basic()
{ //num FreqUnits {Hz, KHz, MHz, GHz, THz};
    freq->clear();
    QVERIFY2(qFuzzyCompare(freq->toDouble(Hz),(double)0.0), "Freq cleared");

    freq->fromDouble(28.495);
    freq2->clear();
    freq2 = freq;
    QVERIFY2(qFuzzyCompare(freq->toDouble(),freq2->toDouble()), "Freq assignment (=) not working");
    QVERIFY2(freq2->isValid(), "Freq isValid not working");



}

void tst_Frequency::test_toDouble()
{
    QVERIFY2(freq->fromDouble(14.195, MHz) == true, "Could not import the double from MHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(THz), (double)0.000014195), "Freq 14.195 in THz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(GHz), (double)0.014195), "Freq 14.195 in GHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(),(double)14.195), "Freq 14.195 in MHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(MHz),(double)14.195), "Freq 14.195 in MHz Stating MHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(KHz),(double)14195), "Freq 14.195 in KHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(Hz),(double)14195000), "Freq 14.195in Hz");

    QVERIFY2(freq->fromDouble(1.296, GHz) == true, "Could not import the double from GHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(GHz),(double)1.296), "Freq 1.296GHz in GHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(MHz),(double)1296.0), "Freq 1.296GHz in MHz stating MHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(),(double)1296.0), "Freq 1.296GHz in MHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(KHz),(double)1296000.0), "Freq 1.296GHz in KHz");
    QVERIFY2(qFuzzyCompare(freq->toDouble(Hz),(double)1296000000.0), "Freq 1.296GHz in Hz");
}

void tst_Frequency::test_toQString()
{
    Frequency freq1(21.295);
    qDebug() << Q_FUNC_INFO << ": " << freq1.toQString();

}
/*

    bool fromQString(const QString &_f, FreqUnits _u = MHz);
    QString toQString(int _decimals = 3, FreqUnits _u = MHz);   // Returns in MHz with decimals
    void setTolerance(const double _t, FreqUnits _u = Hz);      // Defines the tolerance
*/

QTEST_APPLESS_MAIN(tst_Frequency)

#include "tst_frequency.moc"

