/***************************************************************************
                          tst_dxassistantengine.cpp  -  description
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

/*
 * Unit tests for the DXAssistantEngine scoring logic (issue #1016).
 *
 * The Awards status list is seeded through the KLOG_TESTING injection
 * helpers (injectStatusForTest / clearStatusForTest), the same pattern used
 * in tst_dxcluster. Scoring category tests go through rescore() with the
 * identifier fields (dxcc, bandId, spotterContinent) pre-resolved, which is
 * exactly the state of a stored spot; mode-aware categories call the
 * private scoreDXCC() through the friend declaration so no database mode
 * lookup is involved.
 */

#include <QtTest>
#include "../../src/awards.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/dxcluster/clublogmostwanted.h"
#include "../../src/dxcluster/dxassistantengine.h"
#include "../../src/dxcluster/dxspot.h"
#include "../../src/klogdefinitions.h"
#include "../../src/world.h"

class tst_DXAssistantEngine : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();                       // Clears the Awards list after each test

    void test_Constants();

    // DXCC scoring categories (through rescore, mode-agnostic path)
    void test_ATNO();
    void test_NotWorkedBand();
    void test_WorkedUnconfirmedBand();
    void test_ConfirmedBandModeIsDiscarded();

    // Mode-aware categories (through scoreDXCC, explicit mode ids)
    void test_NotWorkedBandMode();
    void test_BandConfirmedModeNeverWorked();
    void test_WorkedUnconfirmedMode();
    void test_ConfirmedExactModeIsDiscarded();

    // Continent modifier
    void test_SameContinentBonus();
    void test_DifferentContinentPenalty();
    void test_UnknownContinentPenalty();

    // Most Wanted scoring modifier
    void test_MostWantedRankAddsPoints();
    void test_MostWantedSeparatesEqualCategories();
    void test_NullMostWantedPointer();

    // rescore() transitions after new QSOs
    void test_RescoreAtnoToWorked();
    void test_RescoreWorkedToConfirmed();

    // Mode awareness switch
    void test_CheckModeDisabled();

    // Edge cases
    void test_UnknownDXCCIsDiscarded();
    void test_InvalidBandIsDiscarded();
    void test_EmptySpotterGetsPenalty();

private:
    DXSpot makeSpot(int dxcc, int bandId, const QString &continent) const;
    void seed(int dxcc, int bandId, int modeId, QSOStatus status);

    DataProxy_SQLite  *dataProxy  = nullptr;
    World             *world      = nullptr;
    Awards            *awards     = nullptr;
    ClubLogMostWanted *mostWanted = nullptr;
    DXAssistantEngine *engine     = nullptr;
    const QString testDbPath = QDir::temp().filePath("klog_tst_dxassistantengine.db");
};

void tst_DXAssistantEngine::initTestCase()
{
    // Remove any leftover DB from a previous crashed run
    if (QFile::exists(testDbPath))
        QFile::remove(testDbPath);

    // Use a dedicated temp DB — never touches the real user database
    dataProxy  = new DataProxy_SQLite(Q_FUNC_INFO, "1.5", testDbPath);
    world      = new World(dataProxy, Q_FUNC_INFO);
    awards     = new Awards(dataProxy, world, nullptr);
    mostWanted = new ClubLogMostWanted;
    engine     = new DXAssistantEngine(awards, world, dataProxy, mostWanted, "EU");
}

void tst_DXAssistantEngine::cleanupTestCase()
{
    delete engine;
    delete mostWanted;
    delete awards;
    delete world;
    delete dataProxy;

    if (QFile::exists(testDbPath))
        QFile::remove(testDbPath);
}

void tst_DXAssistantEngine::cleanup()
{
    awards->clearStatusForTest();
    mostWanted->ranks.clear();   // Ranks now score points: isolate the tests
    engine->setCheckMode(true);
}

DXSpot tst_DXAssistantEngine::makeSpot(int dxcc, int bandId, const QString &continent) const
{
    DXSpot spot;
    spot.setDXCC(dxcc);
    spot.setBandId(bandId);
    spot.setSpotterContinent(continent);
    return spot;
}

void tst_DXAssistantEngine::seed(int dxcc, int bandId, int modeId, QSOStatus status)
{
    EntityStatus es;
    es.dxcc   = dxcc;
    es.bandId = bandId;
    es.modeId = modeId;
    es.status = status;
    awards->injectStatusForTest(es);
}

void tst_DXAssistantEngine::test_Constants()
{
    QVERIFY(DXAssistantEngine::SCORE_ATNO == 1000);
    QVERIFY(DXAssistantEngine::SCORE_NOT_WORKED_BAND == 800);
    QVERIFY(DXAssistantEngine::SCORE_NOT_WORKED_BAND_MODE == 600);
    QVERIFY(DXAssistantEngine::SCORE_WORKED_UNCONFIRMED_BAND == 400);
    QVERIFY(DXAssistantEngine::SCORE_WORKED_UNCONFIRMED_MODE == 200);
    QVERIFY(DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS == 100);
    QVERIFY(DXAssistantEngine::SCORE_DIFF_CONTINENT_PENALTY == -50);
    QVERIFY(DXAssistantEngine::DX_ASSISTANT_CHECK_MODE == true);
}

void tst_DXAssistantEngine::test_ATNO()
{
    DXSpot spot = makeSpot(100, 3, "EU");
    QVERIFY2(engine->rescore(spot), "An ATNO spot must not be discarded");
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS);
    QCOMPARE(spot.getStatusBand(), ATNO);
}

void tst_DXAssistantEngine::test_NotWorkedBand()
{
    seed(100, 5, 1, worked);   // Worked on another band only
    DXSpot spot = makeSpot(100, 3, "EU");
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_NOT_WORKED_BAND
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS);
    QCOMPARE(spot.getStatusBand(), needed);
}

void tst_DXAssistantEngine::test_WorkedUnconfirmedBand()
{
    seed(100, 3, 1, worked);   // Worked on this band, unconfirmed
    DXSpot spot = makeSpot(100, 3, "EU");
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_WORKED_UNCONFIRMED_BAND
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS);
    QCOMPARE(spot.getStatusBand(), worked);
}

void tst_DXAssistantEngine::test_ConfirmedBandModeIsDiscarded()
{
    seed(100, 3, 1, confirmed);
    DXSpot spot = makeSpot(100, 3, "EU");
    QVERIFY2(!engine->rescore(spot), "A confirmed spot must be discarded");
    QCOMPARE(spot.getScore(), -1);
    QCOMPARE(spot.getStatusBand(), confirmed);
}

void tst_DXAssistantEngine::test_NotWorkedBandMode()
{
    seed(100, 3, 1, worked);   // Worked on this band with mode 1 only
    QSOStatus statusBand = unknown;
    QSOStatus statusBandMode = unknown;
    int score = engine->scoreDXCC(100, 3, 99, statusBand, statusBandMode);
    QCOMPARE(score, DXAssistantEngine::SCORE_NOT_WORKED_BAND_MODE);
    QCOMPARE(statusBand, worked);
    QCOMPARE(statusBandMode, needed);
}

void tst_DXAssistantEngine::test_BandConfirmedModeNeverWorked()
{
    seed(100, 3, 1, confirmed);   // Band confirmed with mode 1; mode 2 never worked
    QSOStatus statusBand = unknown;
    QSOStatus statusBandMode = unknown;
    int score = engine->scoreDXCC(100, 3, 2, statusBand, statusBandMode);
    QCOMPARE(score, DXAssistantEngine::SCORE_NOT_WORKED_BAND_MODE);
    QCOMPARE(statusBand, confirmed);
    QCOMPARE(statusBandMode, needed);
}

void tst_DXAssistantEngine::test_WorkedUnconfirmedMode()
{
    seed(100, 3, 1, confirmed);   // Band confirmed with mode 1
    seed(100, 3, 2, worked);      // Mode 2 worked but not confirmed
    QSOStatus statusBand = unknown;
    QSOStatus statusBandMode = unknown;
    int score = engine->scoreDXCC(100, 3, 2, statusBand, statusBandMode);
    QCOMPARE(score, DXAssistantEngine::SCORE_WORKED_UNCONFIRMED_MODE);
    QCOMPARE(statusBand, confirmed);
    QCOMPARE(statusBandMode, worked);
}

void tst_DXAssistantEngine::test_ConfirmedExactModeIsDiscarded()
{
    seed(100, 3, 1, confirmed);
    QSOStatus statusBand = unknown;
    QSOStatus statusBandMode = unknown;
    int score = engine->scoreDXCC(100, 3, 1, statusBand, statusBandMode);
    QCOMPARE(score, -1);
    QCOMPARE(statusBandMode, confirmed);
}

void tst_DXAssistantEngine::test_SameContinentBonus()
{
    DXSpot spot = makeSpot(100, 3, "EU");   // User continent is EU
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS);
}

void tst_DXAssistantEngine::test_DifferentContinentPenalty()
{
    DXSpot spot = makeSpot(100, 3, "NA");
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_DIFF_CONTINENT_PENALTY);
}

void tst_DXAssistantEngine::test_UnknownContinentPenalty()
{
    DXSpot spot = makeSpot(100, 3, QString());
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_DIFF_CONTINENT_PENALTY);
}

void tst_DXAssistantEngine::test_MostWantedRankAddsPoints()
{
    QVERIFY(mostWanted->parse(R"([{"dxcc":100,"rank":5}])"));
    DXSpot spot = makeSpot(100, 3, "EU");
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getMostWantedRank(), 5);

    // The rank earns points: linear from SCORE_MOST_WANTED_MAX (rank 1)
    // down to 0 across the rank span
    int expectedBonus = (DXAssistantEngine::SCORE_MOST_WANTED_MAX
                         * (DXAssistantEngine::MOST_WANTED_RANK_SPAN - 5 + 1))
                        / DXAssistantEngine::MOST_WANTED_RANK_SPAN;
    QVERIFY(expectedBonus > 0);
    QVERIFY(expectedBonus <= DXAssistantEngine::SCORE_MOST_WANTED_MAX);
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS
                                  + expectedBonus);

    // The bonus can never reorder the DXCC category ladder: worst case is
    // max bonus + same-continent vs no bonus + different-continent, which
    // must stay below the 200-point category gap
    QVERIFY(DXAssistantEngine::SCORE_MOST_WANTED_MAX
                + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS
                - DXAssistantEngine::SCORE_DIFF_CONTINENT_PENALTY
            < DXAssistantEngine::SCORE_NOT_WORKED_BAND_MODE
                - DXAssistantEngine::SCORE_WORKED_UNCONFIRMED_BAND);
}

void tst_DXAssistantEngine::test_MostWantedSeparatesEqualCategories()
{
    QVERIFY(mostWanted->parse(R"([{"dxcc":100,"rank":5},{"dxcc":200,"rank":75}])"));
    DXSpot rare   = makeSpot(100, 3, "EU");
    DXSpot common = makeSpot(200, 3, "EU");
    QVERIFY(engine->rescore(rare));
    QVERIFY(engine->rescore(common));

    // Both spots are ATNO with the same continent: the Most Wanted rank
    // makes the rarer one visibly more valuable
    QVERIFY2(rare.getScore() > common.getScore(),
             "A more wanted entity must score higher than a less wanted one");
    QCOMPARE(rare.getMostWantedRank(), 5);
    QCOMPARE(common.getMostWantedRank(), 75);
}

void tst_DXAssistantEngine::test_NullMostWantedPointer()
{
    DXAssistantEngine noMwEngine(awards, world, dataProxy, nullptr, "EU");
    DXSpot spot = makeSpot(100, 3, "EU");
    spot.setMostWantedRank(99);   // Must be overwritten with 0
    QVERIFY2(noMwEngine.rescore(spot), "A null ClubLogMostWanted must not crash");
    QCOMPARE(spot.getMostWantedRank(), 0);
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS);
}

void tst_DXAssistantEngine::test_RescoreAtnoToWorked()
{
    DXSpot spot = makeSpot(100, 3, "EU");
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS);

    // A QSO on this band is logged
    seed(100, 3, 1, worked);
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_WORKED_UNCONFIRMED_BAND
                                  + DXAssistantEngine::SCORE_SAME_CONTINENT_BONUS);
    QCOMPARE(spot.getStatusBand(), worked);
}

void tst_DXAssistantEngine::test_RescoreWorkedToConfirmed()
{
    seed(100, 3, 1, worked);
    DXSpot spot = makeSpot(100, 3, "EU");
    QVERIFY(engine->rescore(spot));

    // The QSO gets confirmed
    awards->clearStatusForTest();
    seed(100, 3, 1, confirmed);
    QVERIFY2(!engine->rescore(spot), "A just-confirmed spot must be discarded");
    QCOMPARE(spot.getScore(), -1);
}

void tst_DXAssistantEngine::test_CheckModeDisabled()
{
    engine->setCheckMode(false);
    seed(100, 3, 1, worked);
    QSOStatus statusBand = unknown;
    QSOStatus statusBandMode = unknown;
    // With mode checks disabled the mode id must be ignored: band status is
    // used for both statuses and the band-level category wins.
    int score = engine->scoreDXCC(100, 3, 99, statusBand, statusBandMode);
    QCOMPARE(score, DXAssistantEngine::SCORE_WORKED_UNCONFIRMED_BAND);
    QCOMPARE(statusBand, worked);
    QCOMPARE(statusBandMode, worked);
    QCOMPARE(engine->resolveModeId("CW"), -1);
}

void tst_DXAssistantEngine::test_UnknownDXCCIsDiscarded()
{
    // The test World has no cty data, so any callsign resolves to -1
    DXSpot spot;
    spot.setDXCall("EA4K");
    QVERIFY2(!engine->score(spot), "An unresolvable DXCC must discard the spot");
    QCOMPARE(spot.getScore(), -1);
}

void tst_DXAssistantEngine::test_InvalidBandIsDiscarded()
{
    DXSpot spot = makeSpot(100, -1, "EU");   // No band match for the frequency
    QVERIFY2(!engine->rescore(spot), "A spot without band must be discarded");
    QCOMPARE(spot.getScore(), -1);
}

void tst_DXAssistantEngine::test_EmptySpotterGetsPenalty()
{
    // score() must not crash on an empty spotter; continent stays unknown
    // and the different-continent penalty applies (checked via rescore path
    // because score() needs a resolvable DXCC).
    DXSpot spot = makeSpot(100, 3, QString());
    QVERIFY(engine->rescore(spot));
    QCOMPARE(spot.getScore(), DXAssistantEngine::SCORE_ATNO
                                  + DXAssistantEngine::SCORE_DIFF_CONTINENT_PENALTY);
    QVERIFY(spot.getSpotterContinent().isEmpty());
}

QTEST_MAIN(tst_DXAssistantEngine)

#include "tst_dxassistantengine.moc"
