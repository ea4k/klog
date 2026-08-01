/***************************************************************************
                          tst_clublogmostwanted.cpp  -  description
                             -------------------
    begin                : Aug 2026
    copyright            : (C) 2026 by Jaime Robles
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
#include <QSignalSpy>
#include <QTemporaryDir>
#include "../../src/dxcluster/clublogmostwanted.h"

class tst_ClubLogMostWanted : public QObject
{
    Q_OBJECT

private slots:
    void test_Defaults();
    void test_Constants();
    void test_ParseArrayFormat();
    void test_ParsePrefixFormat();
    void test_ParsePrefixFormatWithoutResolver();
    void test_ParseInvalidData();
    void test_ParseKeepsOldDataOnFailure();
    void test_Threshold();
    void test_Staleness();
    void test_CacheRoundTrip();
};

void tst_ClubLogMostWanted::test_Defaults()
{
    ClubLogMostWanted mw;
    QVERIFY2(!mw.hasData(), "hasData should be false before any parse");
    QVERIFY2(mw.getRank(344) == 0, "getRank should return 0 without data");
    QVERIFY2(!mw.isInThreshold(344), "isInThreshold should be false without data");
    QVERIFY2(!mw.getLastUpdate().isValid(), "lastUpdate should be invalid without data");
}

void tst_ClubLogMostWanted::test_Constants()
{
    QVERIFY2(ClubLogMostWanted::MOST_WANTED_THRESHOLD == 50, "MOST_WANTED_THRESHOLD should be 50");
    QVERIFY2(ClubLogMostWanted::MOST_WANTED_TTL_DAYS == 30, "MOST_WANTED_TTL_DAYS should be 30");
}

void tst_ClubLogMostWanted::test_ParseArrayFormat()
{
    ClubLogMostWanted mw;
    QByteArray data = R"([{"dxcc":344,"rank":1},{"dxcc":24,"rank":2},{"dxcc":247,"rank":13}])";
    QVERIFY2(mw.parse(data), "Array format should parse");
    QVERIFY2(mw.hasData(), "hasData should be true after parse");
    QVERIFY2(mw.getRank(344) == 1, "Rank of DXCC 344 should be 1");
    QVERIFY2(mw.getRank(24) == 2, "Rank of DXCC 24 should be 2");
    QVERIFY2(mw.getRank(247) == 13, "Rank of DXCC 247 should be 13");
    QVERIFY2(mw.getRank(100) == 0, "Unlisted DXCC should rank 0");
}

void tst_ClubLogMostWanted::test_ParsePrefixFormat()
{
    ClubLogMostWanted mw;
    mw.setPrefixResolver([](const QString &prefix) -> int {
        if (prefix == "P5")   return 344;
        if (prefix == "3Y/B") return 24;
        return -1;
    });
    QByteArray data = R"({"1":"P5","2":"3Y\/B","3":"ZZ9"})";
    QVERIFY2(mw.parse(data), "Prefix format should parse with a resolver");
    QVERIFY2(mw.getRank(344) == 1, "Rank of P5 (344) should be 1");
    QVERIFY2(mw.getRank(24) == 2, "Rank of 3Y/B (24) should be 2");
    QVERIFY2(mw.getRank(-1) == 0, "Unresolvable prefixes should be skipped");
}

void tst_ClubLogMostWanted::test_ParsePrefixFormatWithoutResolver()
{
    ClubLogMostWanted mw;
    QByteArray data = R"({"1":"P5","2":"3Y\/B"})";
    QVERIFY2(!mw.parse(data), "Prefix format without resolver should not parse");
    QVERIFY2(!mw.hasData(), "No data should be stored");
}

void tst_ClubLogMostWanted::test_ParseInvalidData()
{
    ClubLogMostWanted mw;
    QVERIFY2(!mw.parse("not json at all"), "Garbage should not parse");
    QVERIFY2(!mw.parse("<html><body>Most Wanted</body></html>"), "HTML should not parse");
    QVERIFY2(!mw.parse(""), "Empty data should not parse");
    QVERIFY2(!mw.parse("[]"), "Empty array should not parse");
    QVERIFY2(!mw.parse("{}"), "Empty object should not parse");
    QVERIFY2(!mw.hasData(), "No data should be stored after failed parses");
}

void tst_ClubLogMostWanted::test_ParseKeepsOldDataOnFailure()
{
    ClubLogMostWanted mw;
    QVERIFY(mw.parse(R"([{"dxcc":344,"rank":1}])"));
    QVERIFY2(!mw.parse("garbage"), "Garbage should not parse");
    QVERIFY2(mw.getRank(344) == 1, "Old data should survive a failed parse");
}

void tst_ClubLogMostWanted::test_Threshold()
{
    ClubLogMostWanted mw;
    QByteArray data = R"([{"dxcc":100,"rank":50},{"dxcc":101,"rank":51}])";
    QVERIFY(mw.parse(data));
    QVERIFY2(mw.isInThreshold(100), "Rank 50 should be inside the threshold");
    QVERIFY2(!mw.isInThreshold(101), "Rank 51 should be outside the threshold");
    QVERIFY2(!mw.isInThreshold(999), "Unlisted DXCC should be outside the threshold");
}

void tst_ClubLogMostWanted::test_Staleness()
{
    ClubLogMostWanted mw;
    QVERIFY2(mw.isStale(), "No data should be stale");

    mw.lastUpdate = QDateTime::currentDateTimeUtc();
    QVERIFY2(!mw.isStale(), "Fresh data should not be stale");

    mw.lastUpdate = QDateTime::currentDateTimeUtc().addDays(-(ClubLogMostWanted::MOST_WANTED_TTL_DAYS - 1));
    QVERIFY2(!mw.isStale(), "Data younger than the TTL should not be stale");

    mw.lastUpdate = QDateTime::currentDateTimeUtc().addDays(-ClubLogMostWanted::MOST_WANTED_TTL_DAYS);
    QVERIFY2(mw.isStale(), "Data as old as the TTL should be stale");
}

void tst_ClubLogMostWanted::test_CacheRoundTrip()
{
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    QString cachePath = tempDir.filePath("mostwanted.json");

    {   // First instance: parse and save
        ClubLogMostWanted mw;
        mw.setCacheFile(cachePath);   // File does not exist yet: nothing loaded
        QVERIFY(!mw.hasData());
        QVERIFY(mw.parse(R"([{"dxcc":344,"rank":1},{"dxcc":247,"rank":13}])"));
        mw.saveToCache(QByteArray());
        QVERIFY2(QFile::exists(cachePath), "Cache file should have been written");
    }

    {   // Second instance: load from cache, no resolver needed
        ClubLogMostWanted mw;
        QSignalSpy spy(&mw, &ClubLogMostWanted::mostWantedUpdated);
        mw.setCacheFile(cachePath);
        QVERIFY2(mw.hasData(), "Cache should have been loaded");
        QVERIFY2(mw.getRank(344) == 1, "Rank of 344 should survive the cache round trip");
        QVERIFY2(mw.getRank(247) == 13, "Rank of 247 should survive the cache round trip");
        QVERIFY2(mw.getLastUpdate().isValid(), "lastUpdate should be restored from the cache");
        QVERIFY2(!mw.isStale(), "Freshly cached data should not be stale");
        QVERIFY2(spy.count() == 1, "mostWantedUpdated should be emitted after loading the cache");
    }
}

QTEST_GUILESS_MAIN(tst_ClubLogMostWanted)

#include "tst_clublogmostwanted.moc"
