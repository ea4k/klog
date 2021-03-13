#include <QtTest>
#include "../../src/dataproxy_sqlite.h"
#include "../../src/utilities.h"
/***************************************************************************
                          tst_dataproxy.h  -  description
                             -------------------
    begin                : Mar 2021
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
class tst_DataProxy : public QObject
{
    Q_OBJECT

public:
    tst_DataProxy();
    ~tst_DataProxy();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    //void test_getProgresStepForDialog();

private:
  DataProxy_SQLite *dataProxy;
  Utilities *util;

};

tst_DataProxy::tst_DataProxy()
{
    util = new Utilities;
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, util->getVersion ());
}

tst_DataProxy::~tst_DataProxy()
{

}

void tst_DataProxy::initTestCase()
{

}

void tst_DataProxy::cleanupTestCase()
{

}

void tst_DataProxy::test_Constructor()
{
    QVERIFY2(util->getVersion() == "0.0", "Version failed");
}




QTEST_APPLESS_MAIN(tst_DataProxy)

#include "tst_dataproxy.moc"

