/***************************************************************************
                          tst_qso.h  -  description
                             -------------------
    begin                : Aug 2022
    copyright            : (C) 2022 by Jaime Robles
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
#include <QCoreApplication>

#include "../../src/qsodatacache.h"
#include "../../src/callsign.h"
#include "../../src/klogdefinitions.h"
#include "../../src/database.h"
#include "../../src/utilities.h"


class tst_QSODataCache : public QObject
{
    Q_OBJECT

public:
    tst_QSODataCache();
    ~tst_QSODataCache();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_mode_data();
    void test_modes();

private:
    //QSODataCache qdata;

};

tst_QSODataCache::tst_QSODataCache()
{
    Utilities util(Q_FUNC_INFO);
    QString version = QCoreApplication::applicationVersion();
    DataBase db(Q_FUNC_INFO, version, util.getKLogDBFile());

    //qDebug() << Q_FUNC_INFO << " -  After Start of DB Activities";

    if (!db.createConnection(Q_FUNC_INFO))
    {
        QFAIL("No DB connection!");
        //qDebug() << Q_FUNC_INFO << " -  NO DB";
    }
}

tst_QSODataCache::~tst_QSODataCache(){}

void tst_QSODataCache::initTestCase(){}

void tst_QSODataCache::cleanupTestCase(){}

void tst_QSODataCache::test_Constructor()
{
    //qdata.reloadAll();
    //QVERIFY(qdata.getModeFromSubmode("USB") == "SSB");

}

void tst_QSODataCache::test_mode_data()
{
    qDebug() << Q_FUNC_INFO;
    QTest::addColumn<QString>("testString");
    QTest::addColumn<QString>("mode");
    QTest::addColumn<QString>("submode");

    QTest::newRow("USB")   << "SSB"   << "USB";
    QTest::newRow("LSB")   << "SSB"   << "LSB";
}

void tst_QSODataCache::test_modes()
{
    qDebug() << Q_FUNC_INFO;
    QFETCH(QString, testString);
    QFETCH(QString, mode);
    QFETCH(QString, submode);

    QSODataCache qm;
    qm.reloadAll();
    qDebug() << qm.getModeFromSubmode(submode);

    //QCOMPARE(qm.getModeFromSubmode(submode), mode);
}

QTEST_GUILESS_MAIN(tst_QSODataCache)

#include "tst_qsodatacache.moc"
