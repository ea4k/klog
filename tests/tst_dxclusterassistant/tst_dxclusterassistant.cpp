/***************************************************************************
                          tst_dxclusterassistant.cpp  -  description
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
 * Unit tests for the DX Assistant widget (DXClusterAssistant and its model
 * and proxy). These cover the behaviour that is not part of the scoring
 * engine: the Priority column, the max-spots cap, the duplicate-spot rule,
 * the per-session view filters, spot expiry, and the two band-summary
 * metrics, whose sources deliberately differ:
 *
 *   Most active band -> ALL spots arriving from the DXCluster (needed or
 *                       not), tracked by registerBandActivity()
 *   Band to be       -> only the managed (needed) spots, through their score
 */

#include <QtTest>
#include <QSignalSpy>
#include "../../src/awards.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/dxcluster/dxassistantengine.h"
#include "../../src/dxcluster/dxclusterassistant.h"
#include "../../src/dxcluster/dxspot.h"
#include "../../src/klogdefinitions.h"
#include "../../src/world.h"

class tst_DXClusterAssistant : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    // Priority column
    void test_PriorityOrdersByScore();
    void test_PriorityBreaksTiesWithMostWanted();
    void test_PriorityTiedSpotsShareNumber();
    void test_PriorityUpdatesWhenSpotsChange();

    // Max number of spots
    void test_MaxSpotsDefault();
    void test_MaxSpotsEvictsWorst();
    void test_LoweringMaxSpotsTrimsList();

    // addOrUpdateSpot rules
    void test_DiscardedSpotNeverAdded();
    void test_HiddenCallNeverAdded();
    void test_DuplicateReplacedBySameContinentSpotter();
    void test_DuplicateKeepsScoreOtherwise();

    // View filters
    void test_HiddenCallFiltersRow();
    void test_BandFilterHidesRows();
    void test_ContinentFilterHidesRows();
    void test_ClearHiddenSpotsRestoresRows();

    // Expiry
    void test_TTLRemovesOldSpots();
    void test_DefaultTTLIs30Minutes();

    // Band summary
    void test_MostActiveBandCountsAllSpots();
    void test_MostActiveBandIgnoresViewFilters();
    void test_BandToBeUsesManagedSpotScores();
    void test_BandToBeFollowsHiddenSpots();
    void test_BandToBeFollowsBandFilter();
    void test_BandToBeFollowsWorkedQSO();
    void test_BandToBeUsesScoreNotPriority();
    void test_BandToBeFollowsExpiredSpot();
    void test_BandToBeFollowsTTLChange();
    void test_BandToBeFollowsMaxSpotsChange();
    void test_BandToBeFallsBackToMostActiveBand();
    void test_BandToBeEmptyWithoutAnyData();
    void test_ActivityPrunedByAge();

    // Columns
    void test_DefaultHiddenColumns();

private:
    DXSpot makeSpot(const QString &_call, int _bandId, int _score,
                    const QString &_continent = "EU", int _mwRank = 0,
                    const QString &_spotter = "EA4K") const;
    void addScored(const QString &_call, int _bandId, int _score,
                   const QString &_continent = "EU", int _mwRank = 0);
    int visibleRows() const;

    DataProxy_SQLite  *dataProxy = nullptr;
    World             *world     = nullptr;
    Awards            *awards    = nullptr;
    DXAssistantEngine *engine    = nullptr;
    DXClusterAssistant *widget   = nullptr;
    int band20 = -1;
    int band40 = -1;
    const QString testDbPath = QDir::temp().filePath("klog_tst_dxclusterassistant.db");
};

void tst_DXClusterAssistant::initTestCase()
{
    if (QFile::exists(testDbPath))
        QFile::remove(testDbPath);

    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, "1.5", testDbPath);
    world     = new World(dataProxy, Q_FUNC_INFO);
    awards    = new Awards(dataProxy, world, nullptr);
    engine    = new DXAssistantEngine(awards, world, dataProxy, nullptr, "EU");

    // Real band ids so the summary labels can resolve their names
    band20 = dataProxy->getIdFromBandName("20M");
    band40 = dataProxy->getIdFromBandName("40M");
    QVERIFY2(band20 > 0, "The test database should know the 20M band");
    QVERIFY2(band40 > 0, "The test database should know the 40M band");
    QVERIFY(band20 != band40);

    widget = new DXClusterAssistant(awards, world, dataProxy, Q_FUNC_INFO);
    QVERIFY(widget->init());
    widget->setEngine(engine);
}

void tst_DXClusterAssistant::cleanupTestCase()
{
    delete widget;
    delete engine;
    delete awards;
    delete world;
    delete dataProxy;

    if (QFile::exists(testDbPath))
        QFile::remove(testDbPath);
}

void tst_DXClusterAssistant::cleanup()
{   // Reset the widget to a pristine state between tests
    widget->model->clearSpots();
    widget->bandActivity.clear();
    widget->hiddenCalls.clear();
    widget->disabledBands.clear();
    widget->onlyMyContinentSpotters = false;
    widget->setTTL(DXClusterAssistant::SPOT_TTL_MINUTES);
    widget->maxSpots = DXClusterAssistant::MAX_SPOTS_DEFAULT;
    widget->applyViewFilters();
}

DXSpot tst_DXClusterAssistant::makeSpot(const QString &_call, int _bandId, int _score,
                                        const QString &_continent, int _mwRank,
                                        const QString &_spotter) const
{
    DXSpot spot;
    spot.setDXCall(_call);
    spot.setSpotter(_spotter);
    spot.setBandId(_bandId);
    spot.setDXCC(100);
    spot.setScore(_score);
    spot.setSpotterContinent(_continent);
    spot.setMostWantedRank(_mwRank);
    spot.setStatusBand(ATNO);
    spot.setDateTime(QDateTime::currentDateTimeUtc());
    return spot;
}

void tst_DXClusterAssistant::addScored(const QString &_call, int _bandId, int _score,
                                       const QString &_continent, int _mwRank)
{
    widget->addOrUpdateSpot(makeSpot(_call, _bandId, _score, _continent, _mwRank));
}

int tst_DXClusterAssistant::visibleRows() const
{
    return widget->proxy->rowCount();
}

// ─────────────────────────────────────────────────────────────────────────────
// Priority column
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_PriorityOrdersByScore()
{
    addScored("EA1AAA", band20, 500);
    addScored("EA2BBB", band20, 1100);
    addScored("EA3CCC", band20, 800);

    // Priority 1 goes to the highest score, growing as value drops
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(0)), 3);   // 500
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(1)), 1);   // 1100
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(2)), 2);   // 800
}

void tst_DXClusterAssistant::test_PriorityBreaksTiesWithMostWanted()
{
    addScored("EA1AAA", band20, 1100, "EU", 200);   // Same score, less wanted
    addScored("EA2BBB", band20, 1100, "EU", 5);     // Same score, more wanted

    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(1)), 1);   // rank 5
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(0)), 2);   // rank 200
}

void tst_DXClusterAssistant::test_PriorityTiedSpotsShareNumber()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band40, 1100);
    addScored("EA3CCC", band20, 400);

    // Equally valuable spots share the same priority number
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(0)), 1);
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(1)), 1);
    // ...and the next one is pushed behind both of them
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(2)), 3);
}

void tst_DXClusterAssistant::test_PriorityUpdatesWhenSpotsChange()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band20, 800);
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(1)), 2);

    // Removing the best spot promotes the next one to priority 1
    widget->model->removeSpotAt(0);
    QCOMPARE(widget->model->spotCount(), 1);
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(0)), 1);

    // A better spot arriving demotes it again
    addScored("EA3CCC", band20, 1200);
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(0)), 2);
    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(1)), 1);
}

// ─────────────────────────────────────────────────────────────────────────────
// Max number of spots
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_MaxSpotsDefault()
{
    QCOMPARE(DXClusterAssistant::MAX_SPOTS_DEFAULT, 25);
    QCOMPARE(widget->maxSpots, 25);
}

void tst_DXClusterAssistant::test_MaxSpotsEvictsWorst()
{
    widget->setMaxSpots(3);
    addScored("EA1AAA", band20, 1000);
    addScored("EA2BBB", band20, 900);
    addScored("EA3CCC", band20, 800);
    addScored("EA4DDD", band20, 700);   // Worst: must be evicted immediately
    addScored("EA5EEE", band20, 1200);  // Best: pushes out the next worst (800)

    QCOMPARE(widget->model->spotCount(), 3);
    QStringList calls;
    for (int i = 0; i < widget->model->spotCount(); i++)
        calls << widget->model->spotAt(i).getDxCall();
    QVERIFY2(calls.contains("EA5EEE"), "The best spot must be kept");
    QVERIFY2(calls.contains("EA1AAA"), "The second best spot must be kept");
    QVERIFY2(calls.contains("EA2BBB"), "The third best spot must be kept");
    QVERIFY2(!calls.contains("EA4DDD"), "The worst spot must be evicted");
}

void tst_DXClusterAssistant::test_LoweringMaxSpotsTrimsList()
{
    for (int i = 1; i <= 5; i++)
        addScored(QString("EA%1AAA").arg(i), band20, 400 + i * 100);
    QCOMPARE(widget->model->spotCount(), 5);

    widget->setMaxSpots(2);
    QCOMPARE(widget->model->spotCount(), 2);
    // Only the two best survive
    for (int i = 0; i < widget->model->spotCount(); i++)
        QVERIFY(widget->model->spotAt(i).getScore() >= 800);
}

// ─────────────────────────────────────────────────────────────────────────────
// addOrUpdateSpot rules
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_DiscardedSpotNeverAdded()
{
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, -1));
    QCOMPARE(widget->model->spotCount(), 0);
}

void tst_DXClusterAssistant::test_HiddenCallNeverAdded()
{
    widget->hideSpotCall("EA1AAA");
    addScored("EA1AAA", band20, 1100);
    QCOMPARE(widget->model->spotCount(), 0);
}

void tst_DXClusterAssistant::test_DuplicateReplacedBySameContinentSpotter()
{
    // Stored spot reported from another continent
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1000, "NA", 0, "K1ABC"));
    QCOMPARE(widget->model->spotCount(), 1);

    // A spotter in the user's continent is "more positive": replace entirely
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA7XYZ"));
    QCOMPARE(widget->model->spotCount(), 1);
    QCOMPARE(widget->model->spotAt(0).getScore(), 1100);
    QCOMPARE(widget->model->spotAt(0).getSpotterContinent(), QString("EU"));
}

void tst_DXClusterAssistant::test_DuplicateKeepsScoreOtherwise()
{
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA7XYZ"));
    // A spot from another continent must not downgrade the stored one
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 950, "NA", 0, "K1ABC"));

    QCOMPARE(widget->model->spotCount(), 1);
    QCOMPARE(widget->model->spotAt(0).getScore(), 1100);   // Original score kept
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("K1ABC"));  // Metadata refreshed
}

// ─────────────────────────────────────────────────────────────────────────────
// View filters
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_HiddenCallFiltersRow()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band20, 800);
    QCOMPARE(visibleRows(), 2);

    widget->hideSpotCall("EA1AAA");
    QCOMPARE(visibleRows(), 1);
    QCOMPARE(widget->model->spotCount(), 2);   // Still managed, just not shown
}

void tst_DXClusterAssistant::test_BandFilterHidesRows()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band40, 800);
    QCOMPARE(visibleRows(), 2);

    widget->disabledBands.insert(band40);
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    widget->disabledBands.remove(band40);
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);
}

void tst_DXClusterAssistant::test_ContinentFilterHidesRows()
{
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA7XYZ"));
    widget->addOrUpdateSpot(makeSpot("EA2BBB", band20, 800, "NA", 0, "K1ABC"));
    QCOMPARE(visibleRows(), 2);

    widget->onlyMyContinentSpotters = true;   // Engine's user continent is EU
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    widget->onlyMyContinentSpotters = false;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);
}

void tst_DXClusterAssistant::test_ClearHiddenSpotsRestoresRows()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band20, 800);
    widget->hideSpotCall("EA1AAA");
    QCOMPARE(visibleRows(), 1);

    widget->clearHiddenSpots();
    QCOMPARE(visibleRows(), 2);
    QVERIFY(widget->hiddenCalls.isEmpty());
}

// ─────────────────────────────────────────────────────────────────────────────
// Expiry
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_TTLRemovesOldSpots()
{
    DXSpot fresh = makeSpot("EA1AAA", band20, 1100);
    DXSpot old   = makeSpot("EA2BBB", band20, 800);
    old.setDateTime(QDateTime::currentDateTimeUtc().addSecs(-60 * 45));   // 45 min ago
    widget->addOrUpdateSpot(fresh);
    widget->addOrUpdateSpot(old);
    QCOMPARE(widget->model->spotCount(), 2);

    widget->slotTimerTick();   // TTL is 30 minutes
    QCOMPARE(widget->model->spotCount(), 1);
    QCOMPARE(widget->model->spotAt(0).getDxCall(), QString("EA1AAA"));
}

void tst_DXClusterAssistant::test_DefaultTTLIs30Minutes()
{
    QCOMPARE(DXClusterAssistant::SPOT_TTL_MINUTES, 30);
    QCOMPARE(widget->ttlMinutes, 30);
}

// ─────────────────────────────────────────────────────────────────────────────
// Band summary
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_MostActiveBandCountsAllSpots()
{
    // 40M carries more raw cluster activity, but every one of those spots is
    // already confirmed (score -1) so none of them is managed by the widget.
    for (int i = 1; i <= 4; i++)
        widget->registerBandActivity(makeSpot(QString("EA%1AAA").arg(i), band40, -1));
    // 20M has fewer spots but they are the workable ones
    widget->registerBandActivity(makeSpot("EA5AAA", band20, 1100));
    addScored("EA5AAA", band20, 1100);

    QCOMPARE(widget->bandActivity.value(band40).count(), 4);
    QCOMPARE(widget->bandActivity.value(band20).count(), 1);

    // Most active band must follow the raw activity (40M), not the managed list
    QVERIFY2(widget->mostActiveBandLabel->text().contains(dataProxy->getNameFromBandId(band40)),
             "Most active band must count ALL cluster spots, needed or not");
    // Band to be follows the scores of the managed spots (only 20M has any)
    QVERIFY2(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band20)),
             "Band to be must follow the score of the needed spots");
}

void tst_DXClusterAssistant::test_MostActiveBandIgnoresViewFilters()
{
    for (int i = 1; i <= 3; i++)
        widget->registerBandActivity(makeSpot(QString("EA%1AAA").arg(i), band40, 500));
    widget->registerBandActivity(makeSpot("EA9ZZZ", band20, 500));

    QString expected = dataProxy->getNameFromBandId(band40);
    QVERIFY(widget->mostActiveBandLabel->text().contains(expected));

    // Filtering 40M out of the view must not change the activity metric
    widget->disabledBands.insert(band40);
    widget->applyViewFilters();
    QVERIFY2(widget->mostActiveBandLabel->text().contains(expected),
             "View filters must not affect Most active band");

    // Neither must the continent filter
    widget->onlyMyContinentSpotters = true;
    widget->applyViewFilters();
    QVERIFY(widget->mostActiveBandLabel->text().contains(expected));
}

void tst_DXClusterAssistant::test_BandToBeUsesManagedSpotScores()
{
    // 20M: many low-value spots. 40M: fewer, but far more valuable.
    addScored("EA1AAA", band20, 200);
    addScored("EA2BBB", band20, 200);
    addScored("EA3CCC", band20, 200);
    addScored("EA4DDD", band40, 1100);

    QVERIFY2(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band40)),
             "Band to be must pick the band with the highest cumulative score");
}

void tst_DXClusterAssistant::test_BandToBeFollowsHiddenSpots()
{
    // 40M wins only thanks to a single high-value spot
    addScored("EA1AAA", band20, 500);
    addScored("EA2BBB", band20, 500);
    addScored("EA3CCC", band40, 1200);
    QString name20 = dataProxy->getNameFromBandId(band20);
    QString name40 = dataProxy->getNameFromBandId(band40);
    QVERIFY(widget->bandToBeLabel->text().contains(name40));

    // Hiding that spot removes its value: 20M becomes the band to be
    widget->hideSpotCall("EA3CCC");
    QVERIFY2(widget->bandToBeLabel->text().contains(name20),
             "Band to be must be recalculated with the shown spots when one is hidden");

    // Restoring it brings 40M back
    widget->clearHiddenSpots();
    QVERIFY(widget->bandToBeLabel->text().contains(name40));
}

void tst_DXClusterAssistant::test_BandToBeFollowsBandFilter()
{
    addScored("EA1AAA", band20, 500);
    addScored("EA2BBB", band40, 1200);
    QString name20 = dataProxy->getNameFromBandId(band20);
    QVERIFY(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band40)));

    // A band the user filtered out offers no reachable value
    widget->disabledBands.insert(band40);
    widget->applyViewFilters();
    QVERIFY2(widget->bandToBeLabel->text().contains(name20),
             "A filtered-out band must not be proposed as the band to be");
}

void tst_DXClusterAssistant::test_BandToBeFollowsWorkedQSO()
{
    addScored("EA1AAA", band20, 500);
    addScored("EA2BBB", band40, 1200);
    QString name20 = dataProxy->getNameFromBandId(band20);
    QVERIFY(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band40)));

    // Working/confirming the 40M station: the engine discards the spot on the
    // next recalculation, which must move the band to be to 20M.
    awards->clearStatusForTest();
    EntityStatus es;
    es.dxcc   = 100;          // The DXCC every test spot carries
    es.bandId = band40;
    es.modeId = -1;
    es.status = confirmed;
    awards->injectStatusForTest(es);

    widget->recalculateAll();
    awards->clearStatusForTest();

    QCOMPARE(widget->model->spotCount(), 1);   // The confirmed spot is gone
    QVERIFY2(widget->bandToBeLabel->text().contains(name20),
             "Band to be must be recalculated after a QSO is worked");
}

void tst_DXClusterAssistant::test_BandToBeUsesScoreNotPriority()
{
    // 40M holds the single best spot, so it owns priority 1. 20M holds three
    // lesser spots whose scores add up to more. Band to be must follow the
    // cumulative score (20M), not the position in the priority ranking.
    addScored("EA1AAA", band40, 1200);   // priority 1
    addScored("EA2BBB", band20, 600);
    addScored("EA3CCC", band20, 600);
    addScored("EA4DDD", band20, 600);    // 1800 total on 20M

    QCOMPARE(widget->model->priorityOfSpot(widget->model->spotAt(0)), 1);
    QVERIFY2(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band20)),
             "Band to be must weigh the accumulated score, not the best priority");
}

void tst_DXClusterAssistant::test_BandToBeFollowsExpiredSpot()
{
    addScored("EA1AAA", band20, 500);
    DXSpot leader = makeSpot("EA2BBB", band40, 1200);
    widget->addOrUpdateSpot(leader);
    QVERIFY(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band40)));

    // Age the 40M spot past the TTL and let the timer tick expire it
    DXSpot aged = widget->model->spotAt(1);
    aged.setDateTime(QDateTime::currentDateTimeUtc().addSecs(-60 * 45));
    widget->model->replaceSpot(1, aged);
    widget->slotTimerTick();

    QCOMPARE(widget->model->spotCount(), 1);
    QVERIFY2(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band20)),
             "Band to be must be recalculated when a spot expires");
}

void tst_DXClusterAssistant::test_BandToBeFollowsTTLChange()
{
    addScored("EA1AAA", band20, 500);
    DXSpot older = makeSpot("EA2BBB", band40, 1200);
    older.setDateTime(QDateTime::currentDateTimeUtc().addSecs(-60 * 20));   // 20 min old
    widget->addOrUpdateSpot(older);
    QVERIFY(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band40)));

    // Shortening the max age to 15 minutes expires the 40M spot right away
    widget->setTTL(15);
    QCOMPARE(widget->model->spotCount(), 1);
    QVERIFY2(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band20)),
             "Band to be must be recalculated when the max age changes");
}

void tst_DXClusterAssistant::test_BandToBeFollowsMaxSpotsChange()
{
    // 20M wins on accumulated score while all three of its spots are kept
    addScored("EA1AAA", band40, 1000);
    addScored("EA2BBB", band20, 600);
    addScored("EA3CCC", band20, 600);
    addScored("EA4DDD", band20, 600);
    QVERIFY(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band20)));

    // Capping the list at 2 evicts two of the 20M spots, so 40M takes over
    widget->setMaxSpots(2);
    QCOMPARE(widget->model->spotCount(), 2);
    QVERIFY2(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band40)),
             "Band to be must be recalculated when the max number of spots changes");
}

void tst_DXClusterAssistant::test_BandToBeFallsBackToMostActiveBand()
{
    // Cluster activity on 40M, but every spot there is already confirmed so
    // none of them is workable: there is no score to gain anywhere.
    for (int i = 1; i <= 3; i++)
        widget->registerBandActivity(makeSpot(QString("EA%1AAA").arg(i), band40, -1));
    QCOMPARE(widget->model->spotCount(), 0);

    QString name40 = dataProxy->getNameFromBandId(band40);
    QVERIFY(widget->mostActiveBandLabel->text().contains(name40));
    QVERIFY2(widget->bandToBeLabel->text().contains(name40),
             "Without spots to score, Band to be must fall back to the most active band");

    // A workable spot elsewhere takes precedence again
    widget->registerBandActivity(makeSpot("EA9ZZZ", band20, 700));
    addScored("EA9ZZZ", band20, 700);
    QVERIFY2(widget->bandToBeLabel->text().contains(dataProxy->getNameFromBandId(band20)),
             "A band with reachable score must win over the fallback");
    QVERIFY2(widget->mostActiveBandLabel->text().contains(name40),
             "Most active band must still follow raw activity");
}

void tst_DXClusterAssistant::test_BandToBeEmptyWithoutAnyData()
{
    // No spots and no activity at all: both labels show the em dash
    QVERIFY(widget->mostActiveBandLabel->text().contains(QStringLiteral("—")));
    QVERIFY(widget->bandToBeLabel->text().contains(QStringLiteral("—")));
}

void tst_DXClusterAssistant::test_ActivityPrunedByAge()
{
    DXSpot old = makeSpot("EA1AAA", band40, 500);
    old.setDateTime(QDateTime::currentDateTimeUtc().addSecs(-60 * 45));   // Past the 30 min TTL
    widget->registerBandActivity(old);
    widget->registerBandActivity(makeSpot("EA2BBB", band20, 500));

    // The over-age entry is dropped, so 20M is the only activity left
    QVERIFY2(!widget->bandActivity.contains(band40), "Over-age activity must be pruned");
    QCOMPARE(widget->bandActivity.value(band20).count(), 1);
    QVERIFY(widget->mostActiveBandLabel->text().contains(dataProxy->getNameFromBandId(band20)));
}

// ─────────────────────────────────────────────────────────────────────────────
// Columns
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_DefaultHiddenColumns()
{
    // Priority replaces Score and MW Rank in the default view; Mode is hidden
    // because most cluster spots carry none.
    QVERIFY2(!widget->tableView->isColumnHidden(DXAssistantSpotModel::ColPriority),
             "Priority must be visible by default");
    QVERIFY2(widget->tableView->isColumnHidden(DXAssistantSpotModel::ColScore),
             "Score must be hidden by default");
    QVERIFY2(widget->tableView->isColumnHidden(DXAssistantSpotModel::ColMWRank),
             "MW Rank must be hidden by default");
    QVERIFY2(widget->tableView->isColumnHidden(DXAssistantSpotModel::ColMode),
             "Mode must be hidden by default");
    // Priority is the first column
    QCOMPARE(static_cast<int>(DXAssistantSpotModel::ColPriority), 0);
}

QTEST_MAIN(tst_DXClusterAssistant)

#include "tst_dxclusterassistant.moc"
