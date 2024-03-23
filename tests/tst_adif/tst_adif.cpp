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
}

QTEST_APPLESS_MAIN(tst_Adif)

#include "tst_adif.moc"

