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
#include "qtestcase.h"
#include "../../src/qsodatacache.h"
#include "../../src/callsign.h"
#include "../../src/klogdefinitions.h"
#include "../../src/database/database.h"
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
    void test_modes_data();
    void test_modes();

private:
    // qSODataCache qdata;
    DataBase *db;
    Utilities *util;

};

tst_QSODataCache::tst_QSODataCache()
{
    QString _version = QString ("99.9");
    // qDebug() << Q_FUNC_INFO << "- 001";
    util = new Utilities(Q_FUNC_INFO);
    // qDebug() << Q_FUNC_INFO << "- 002: " << util->getKLogDBFile();

    db = new DataBase(Q_FUNC_INFO, _version, util->getKLogDBFile());
    // qDebug() << Q_FUNC_INFO << "- 003";
    QCOMPARE(db->createConnection(Q_FUNC_INFO), true);
    // qDebug() << Q_FUNC_INFO << " - END";
}

tst_QSODataCache::~tst_QSODataCache(){}

void tst_QSODataCache::initTestCase(){}

void tst_QSODataCache::cleanupTestCase(){}

void tst_QSODataCache::test_Constructor()
{
    // qdata.reloadAll();
    // qVERIFY(qdata.getModeFromSubmode("USB") == "SSB");
}

void tst_QSODataCache::test_modes_data()
{
    QTest::addColumn<QString>("mode");
    QTest::addColumn<QString>("submode");

    QFile file(":/modes.json");
    if (!file.open(QIODevice::ReadOnly)) {
        // qDebug() << "No se pudo abrir el resource modes.json";
        QFAIL("No se pudo abrir modes.json como resource");
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray arr = doc.array();

    for (const QJsonValue& v : arr) {
        QJsonObject obj = v.toObject();
        QString mode = obj.value("mode").toString();
        QJsonArray submodes = obj.value("submodes").toArray();
        for (const QJsonValue& sv : submodes) {
            QString submode = sv.toString();
            QTest::newRow(qPrintable(mode + "_" + submode)) << mode << submode;
        }
    }
}

void tst_QSODataCache::test_modes()
{
    QFETCH(QString, mode);
    QFETCH(QString, submode);

    QSODataCache cache(Q_FUNC_INFO); // Make sure you pass your DB pointer
    cache.reloadAll();

    QCOMPARE(cache.getModeFromSubmode(submode), mode);
}

QTEST_GUILESS_MAIN(tst_QSODataCache)

#include "tst_qsodatacache.moc"
