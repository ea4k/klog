#include <QtTest>
#include "../../src/world.h"
#include "../../src/qso.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/database.h"

/***************************************************************************
                          tst_world.h  -  description
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
class tst_World : public QObject
{
    Q_OBJECT

public:
    tst_World();
    ~tst_World();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();

private:
    DataProxy_SQLite *dataProxy;


};

tst_World::tst_World()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
}

tst_World::~tst_World()
{

}

void tst_World::initTestCase()
{

}

void tst_World::cleanupTestCase()
{

}

void tst_World::test_Constructor()
{
    //QVERIFY(util->getVersion() == "0.0");
}



QTEST_APPLESS_MAIN(tst_World)

#include "tst_world.moc"
