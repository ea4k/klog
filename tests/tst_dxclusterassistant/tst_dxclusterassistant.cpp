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
    void test_DuplicateKeepsTheClosestSpotter();
    void test_DuplicateFollowsTheSpotterProximityLadder();

    // View filters
    void test_HiddenCallFiltersRow();
    void test_BandFilterHidesRows();
    void test_ContinentFilterHidesRows();
    void test_SpotterMyDXCCFilterHidesRows();
    void test_SpotterMyDXCCInactiveWithoutUserEntity();
    void test_SpotterMyCallFilterKeepsOnlyMySpots();
    void test_SpotterMyCallInactiveWithoutMyCallsign();
    void test_ShownSpotsFollowTheSpotterMyCallFilter();
    void test_StatusFilterHidesRows();
    void test_DXCCFilterHidesRows();
    void test_SourceFilterHidesRows();
    void test_SourceColumnShowsWhereTheSpotCameFrom();
    void test_DXCCsInViewListsHeldEntities();
    void test_FollowMyBandShowsOnlyTheCurrentBand();
    void test_FollowMyBandOverridesTheBandCheckboxes();
    void test_ClearHiddenSpotsRestoresRows();
    void test_ClearAllEmptiesTheListAndTheHiddenCalls();
    void test_ShownSpotsFollowsTheViewFilters();

    // Expiry
    void test_TTLRemovesOldSpots();
    void test_DefaultTTLIs30Minutes();

    // Band summary
    void test_MostActiveBandCountsAllSpots();
    void test_MostActiveBandIgnoresViewFilters();
    void test_BandToBeUsesManagedSpotScores();
    void test_BandToBeFollowsHiddenSpots();
    void test_BandToBeFollowsBandFilter();
    void test_BandToBeFollowsStatusFilter();
    void test_BandToBeFollowsDXCCFilter();
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

    // Embedding (the widget lives in a tab of MainWindow's dxUpRightTab)
    void test_IsNotATopLevelWindowWhenParented();
    void test_EmbedsAndDetachesAsATabPage();

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
    widget->spotterFilter = DXAssistantProxyModel::SpotterAll;
    engine->setUserCallsign(QString());
    widget->disabledStatuses.clear();
    widget->disabledDXCCs.clear();
    widget->disabledSources.clear();
    widget->followMyBand = false;
    widget->currentBandId = -1;
    widget->rigConnected = false;
    engine->setUserDXCC(-1);
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

void tst_DXClusterAssistant::test_DuplicateKeepsTheClosestSpotter()
{
    DXSpot stored = makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA7XYZ");
    stored.setDateTime(QDateTime::currentDateTimeUtc().addSecs(-600));
    widget->addOrUpdateSpot(stored);

    // A spot from another continent must not downgrade the stored one: only
    // the age is refreshed, because the station is still being heard
    DXSpot farther = makeSpot("EA1AAA", band20, 950, "NA", 0, "K1ABC");
    farther.setComment("From the cluster");
    widget->addOrUpdateSpot(farther);

    QCOMPARE(widget->model->spotCount(), 1);
    QCOMPARE(widget->model->spotAt(0).getScore(), 1100);                 // Original score kept
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("EA7XYZ"));  // ... and its spotter
    QCOMPARE(widget->model->spotAt(0).getSpotterContinent(), QString("EU"));
    QVERIFY(widget->model->spotAt(0).getComment() != QString("From the cluster"));
    QVERIFY(widget->model->spotAt(0).getDateTime().secsTo(QDateTime::currentDateTimeUtc()) < 5);
}

void tst_DXClusterAssistant::test_DuplicateFollowsTheSpotterProximityLadder()
{
    // Other continent < My continent < My DXCC < My call
    engine->setUserDXCC(281);          // Spain
    engine->setUserCallsign("EA4K");

    DXSpot elsewhere = makeSpot("EA1AAA", band20, 1000, "NA", 0, "K1ABC");
    elsewhere.setSpotterDXCC(291);
    widget->addOrUpdateSpot(elsewhere);
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("K1ABC"));

    // My continent takes over from another continent
    DXSpot myContinent = makeSpot("EA1AAA", band20, 1100, "EU", 0, "F5ABC");
    myContinent.setSpotterDXCC(227);   // France
    widget->addOrUpdateSpot(myContinent);
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("F5ABC"));

    // My DXCC takes over from my continent
    DXSpot myDXCC = makeSpot("EA1AAA", band20, 1200, "EU", 0, "EA7XYZ");
    myDXCC.setSpotterDXCC(281);
    widget->addOrUpdateSpot(myDXCC);
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("EA7XYZ"));

    // Another spotter in my own entity brings nothing closer: it stays
    DXSpot anotherMyDXCC = makeSpot("EA1AAA", band20, 1300, "EU", 0, "EA5WWW");
    anotherMyDXCC.setSpotterDXCC(281);
    widget->addOrUpdateSpot(anotherMyDXCC);
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("EA7XYZ"));
    QCOMPARE(widget->model->spotAt(0).getScore(), 1200);

    // Hearing it ourselves always wins
    DXSpot mine = makeSpot("EA1AAA", band20, 900, "EU", 0, "EA4K");
    mine.setSpotterDXCC(281);
    mine.setSource(SpotSourceWSJTX);
    widget->addOrUpdateSpot(mine);
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("EA4K"));
    QCOMPARE(widget->model->spotAt(0).getScore(), 900);
    QCOMPARE(widget->model->spotAt(0).getSource(), SpotSourceWSJTX);

    // ... and nothing takes the entry away from us afterwards
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1500, "EU", 0, "EA7XYZ"));
    QCOMPARE(widget->model->spotAt(0).getSpotter(), QString("EA4K"));
    QCOMPARE(widget->model->spotAt(0).getSource(), SpotSourceWSJTX);

    engine->setUserDXCC(-1);
    engine->setUserCallsign(QString());
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

    widget->spotterFilter = DXAssistantProxyModel::SpotterMyContinent;   // Engine's user continent is EU
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    widget->spotterFilter = DXAssistantProxyModel::SpotterAll;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);
}

void tst_DXClusterAssistant::test_SpotterMyDXCCFilterHidesRows()
{
    engine->setUserDXCC(281);   // Spain

    DXSpot local = makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA7XYZ");
    local.setSpotterDXCC(281);
    widget->addOrUpdateSpot(local);

    DXSpot foreign = makeSpot("EA2BBB", band20, 800, "EU", 0, "F5ABC");
    foreign.setSpotterDXCC(227);   // France: same continent, different entity
    widget->addOrUpdateSpot(foreign);
    QCOMPARE(visibleRows(), 2);

    // My continent keeps both: they are both European spotters
    widget->spotterFilter = DXAssistantProxyModel::SpotterMyContinent;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);

    // My DXCC is stricter: only the spot from my own entity survives
    widget->spotterFilter = DXAssistantProxyModel::SpotterMyDXCC;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    engine->setUserDXCC(-1);
}

void tst_DXClusterAssistant::test_SpotterMyDXCCInactiveWithoutUserEntity()
{
    // An unknown user entity leaves the My DXCC filter inactive rather than
    // hiding every spot for a reason the user cannot see.
    engine->setUserDXCC(-1);

    DXSpot spot = makeSpot("EA1AAA", band20, 1100, "EU", 0, "F5ABC");
    spot.setSpotterDXCC(227);
    widget->addOrUpdateSpot(spot);

    widget->spotterFilter = DXAssistantProxyModel::SpotterMyDXCC;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);
}

void tst_DXClusterAssistant::test_SpotterMyCallFilterKeepsOnlyMySpots()
{
    engine->setUserCallsign("EA4K");

    // Heard here: the WSJT-X decodes are spotted with our own callsign
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA4K"));
    // Reported by somebody else, whether by the cluster or by another station
    widget->addOrUpdateSpot(makeSpot("EA2BBB", band20, 800, "EU", 0, "EA7XYZ"));
    QCOMPARE(visibleRows(), 2);

    widget->spotterFilter = DXAssistantProxyModel::SpotterMyCall;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);
    QCOMPARE(widget->proxy->data(widget->proxy->index(0, DXAssistantSpotModel::ColDXCall),
                                 Qt::DisplayRole).toString(), QString("EA1AAA"));

    // The callsign is matched whatever its case
    engine->setUserCallsign("ea4k");
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    widget->spotterFilter = DXAssistantProxyModel::SpotterAll;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);

    engine->setUserCallsign(QString());
}

void tst_DXClusterAssistant::test_SpotterMyCallInactiveWithoutMyCallsign()
{
    // Without a callsign the filter is left inactive rather than hiding every
    // spot for a reason the user cannot see, as My DXCC does.
    engine->setUserCallsign(QString());
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA7XYZ"));

    widget->spotterFilter = DXAssistantProxyModel::SpotterMyCall;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);
}

void tst_DXClusterAssistant::test_ShownSpotsFollowTheSpotterMyCallFilter()
{
    // "Show to map" and the band summary use spotIsShown(), which mirrors the
    // rules of the proxy filter and has to agree with it.
    engine->setUserCallsign("EA4K");
    widget->addOrUpdateSpot(makeSpot("EA1AAA", band20, 1100, "EU", 0, "EA4K"));
    widget->addOrUpdateSpot(makeSpot("EA2BBB", band20, 800, "EU", 0, "EA7XYZ"));

    widget->spotterFilter = DXAssistantProxyModel::SpotterMyCall;
    widget->applyViewFilters();

    QList<DXSpot> shown = widget->shownSpots();
    QCOMPARE(shown.count(), 1);
    QCOMPARE(shown.first().getDxCall(), QString("EA1AAA"));

    engine->setUserCallsign(QString());
}

void tst_DXClusterAssistant::test_StatusFilterHidesRows()
{
    DXSpot atno = makeSpot("EA1AAA", band20, 1100);   // makeSpot sets ATNO
    widget->addOrUpdateSpot(atno);

    DXSpot workedSpot = makeSpot("EA2BBB", band20, 400);
    workedSpot.setStatusBand(worked);
    workedSpot.setQSOStatus(worked);
    widget->addOrUpdateSpot(workedSpot);
    QCOMPARE(visibleRows(), 2);

    widget->disabledStatuses.insert(static_cast<int>(worked));
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    widget->disabledStatuses.insert(static_cast<int>(ATNO));
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 0);

    widget->disabledStatuses.clear();
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);
}

void tst_DXClusterAssistant::test_SourceFilterHidesRows()
{
    DXSpot fromCluster = makeSpot("EA1AAA", band20, 1100);
    fromCluster.setSource(SpotSourceDXCluster);
    widget->addOrUpdateSpot(fromCluster);

    DXSpot fromWSJTX = makeSpot("EA2BBB", band20, 800);
    fromWSJTX.setSource(SpotSourceWSJTX);
    widget->addOrUpdateSpot(fromWSJTX);
    QCOMPARE(visibleRows(), 2);

    widget->disabledSources.insert(static_cast<int>(SpotSourceWSJTX));
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);
    QCOMPARE(widget->proxy->data(widget->proxy->index(0, DXAssistantSpotModel::ColDXCall),
                                 Qt::DisplayRole).toString(), QString("EA1AAA"));

    widget->disabledSources.insert(static_cast<int>(SpotSourceDXCluster));
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 0);

    widget->disabledSources.clear();
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);
}

void tst_DXClusterAssistant::test_SourceColumnShowsWhereTheSpotCameFrom()
{
    DXSpot fromWSJTX = makeSpot("EA2BBB", band20, 800);
    fromWSJTX.setSource(SpotSourceWSJTX);
    widget->addOrUpdateSpot(fromWSJTX);

    QCOMPARE(widget->model->data(widget->model->index(0, DXAssistantSpotModel::ColSource),
                                 Qt::DisplayRole).toString(),
             DXAssistantSpotModel::sourceName(SpotSourceWSJTX));
    QCOMPARE(widget->model->data(widget->model->index(0, DXAssistantSpotModel::ColSource),
                                 DXAssistantSpotModel::SourceRole).toInt(),
             static_cast<int>(SpotSourceWSJTX));
    // A spot of unknown origin still shows something in the column
    widget->model->clearSpots();
    widget->addOrUpdateSpot(makeSpot("EA3CCC", band20, 500));
    QVERIFY(!widget->model->data(widget->model->index(0, DXAssistantSpotModel::ColSource),
                                 Qt::DisplayRole).toString().isEmpty());
    QVERIFY(!widget->model->headerData(DXAssistantSpotModel::ColSource, Qt::Horizontal,
                                       Qt::DisplayRole).toString().isEmpty());
}

void tst_DXClusterAssistant::test_DXCCFilterHidesRows()
{
    DXSpot spain = makeSpot("EA1AAA", band20, 1100);
    spain.setDXCC(281);
    widget->addOrUpdateSpot(spain);

    DXSpot france = makeSpot("F5ABC", band20, 800);
    france.setDXCC(227);
    widget->addOrUpdateSpot(france);
    QCOMPARE(visibleRows(), 2);

    widget->disabledDXCCs.insert(227);
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    widget->disabledDXCCs.clear();
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 2);
}

void tst_DXClusterAssistant::test_DXCCsInViewListsHeldEntities()
{
    DXSpot spain = makeSpot("EA1AAA", band20, 1100);
    spain.setDXCC(281);
    widget->addOrUpdateSpot(spain);

    DXSpot france = makeSpot("F5ABC", band20, 800);
    france.setDXCC(227);
    widget->addOrUpdateSpot(france);

    QList<int> entities = widget->dxccsInView();
    QCOMPARE(entities.count(), 2);
    QVERIFY(entities.contains(281));
    QVERIFY(entities.contains(227));

    // A filtered-out entity stays listed, otherwise it could never be
    // switched back on from the menu.
    widget->disabledDXCCs.insert(227);
    widget->applyViewFilters();
    QVERIFY(widget->dxccsInView().contains(227));
}

void tst_DXClusterAssistant::test_FollowMyBandShowsOnlyTheCurrentBand()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band40, 800);
    QCOMPARE(visibleRows(), 2);

    widget->setCurrentBand(band20);
    widget->followMyBand = true;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);

    // Working a different band moves the view with the rig
    widget->setCurrentBand(band40);
    QCOMPARE(visibleRows(), 1);
    QCOMPARE(widget->shownSpots().count(), 1);
    QCOMPARE(widget->shownSpots().first().getBandId(), band40);
}

void tst_DXClusterAssistant::test_FollowMyBandOverridesTheBandCheckboxes()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band40, 800);

    // 20M filtered out by hand, but the rig is on 20M: following the rig wins
    widget->disabledBands.insert(band20);
    widget->setCurrentBand(band20);
    widget->followMyBand = true;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);
    QCOMPARE(widget->shownSpots().first().getBandId(), band20);

    // Switching the follow off hands control back to the checkboxes
    widget->followMyBand = false;
    widget->applyViewFilters();
    QCOMPARE(visibleRows(), 1);
    QCOMPARE(widget->shownSpots().first().getBandId(), band40);
}

void tst_DXClusterAssistant::test_ClearAllEmptiesTheListAndTheHiddenCalls()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band40, 800);
    widget->hideSpotCall("EA1AAA");
    widget->registerBandActivity(makeSpot("EA3CCC", band20, 500));
    QVERIFY(widget->model->spotCount() > 0);
    QVERIFY(!widget->hiddenCalls.isEmpty());

    widget->clearAll();
    QCOMPARE(widget->model->spotCount(), 0);
    QCOMPARE(visibleRows(), 0);
    QVERIFY2(widget->hiddenCalls.isEmpty(),
             "Clear all must forget the hidden calls, otherwise they could never come back");
    QVERIFY(widget->bandActivity.isEmpty());

    // The view filters are settings, not data: they survive
    QCOMPARE(widget->maxSpots, 25);
}

void tst_DXClusterAssistant::test_ShownSpotsFollowsTheViewFilters()
{
    addScored("EA1AAA", band20, 1100);
    addScored("EA2BBB", band40, 800);
    QCOMPARE(widget->shownSpots().count(), 2);

    widget->disabledBands.insert(band40);
    widget->applyViewFilters();

    QList<DXSpot> shown = widget->shownSpots();
    QCOMPARE(shown.count(), 1);
    QCOMPARE(shown.first().getDxCall(), QString("EA1AAA"));
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
    widget->spotterFilter = DXAssistantProxyModel::SpotterMyContinent;
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

void tst_DXClusterAssistant::test_BandToBeFollowsStatusFilter()
{
    // 40M wins only thanks to a single high-value spot, and that spot is the
    // only Worked one: filtering Worked out must move the band to be.
    addScored("EA1AAA", band20, 500);
    addScored("EA2BBB", band20, 500);
    DXSpot workedSpot = makeSpot("EA3CCC", band40, 1200);
    workedSpot.setStatusBand(worked);
    workedSpot.setQSOStatus(worked);
    widget->addOrUpdateSpot(workedSpot);

    QString name20 = dataProxy->getNameFromBandId(band20);
    QString name40 = dataProxy->getNameFromBandId(band40);
    QVERIFY(widget->bandToBeLabel->text().contains(name40));

    widget->disabledStatuses.insert(static_cast<int>(worked));
    widget->applyViewFilters();
    QVERIFY2(widget->bandToBeLabel->text().contains(name20),
             "A status filtered out of the view must not feed the band to be");
}

void tst_DXClusterAssistant::test_BandToBeFollowsDXCCFilter()
{
    DXSpot spain = makeSpot("EA1AAA", band20, 500);
    spain.setDXCC(281);
    widget->addOrUpdateSpot(spain);

    DXSpot france = makeSpot("F5ABC", band40, 1200);
    france.setDXCC(227);
    widget->addOrUpdateSpot(france);

    QString name20 = dataProxy->getNameFromBandId(band20);
    QString name40 = dataProxy->getNameFromBandId(band40);
    QVERIFY(widget->bandToBeLabel->text().contains(name40));

    widget->disabledDXCCs.insert(227);
    widget->applyViewFilters();
    QVERIFY2(widget->bandToBeLabel->text().contains(name20),
             "An entity filtered out of the view must not feed the band to be");
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

// ─────────────────────────────────────────────────────────────────────────────
// Embedding
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXClusterAssistant::test_IsNotATopLevelWindowWhenParented()
{
    // The widget must set no window flags of its own: given a parent it has
    // to behave as a plain child widget so it can be put inside a tab.
    QWidget host;
    DXClusterAssistant embedded(awards, world, dataProxy, Q_FUNC_INFO, &host);
    QVERIFY(embedded.init());
    QVERIFY2(!embedded.isWindow(), "The DX Assistant must not be a top-level window");
    QCOMPARE(embedded.parentWidget(), &host);
}

void tst_DXClusterAssistant::test_EmbedsAndDetachesAsATabPage()
{
    QTabWidget tabWidget;
    DXClusterAssistant *embedded = new DXClusterAssistant(awards, world, dataProxy,
                                                          Q_FUNC_INFO, &tabWidget);
    QVERIFY(embedded->init());

    const int index = tabWidget.addTab(embedded, "DX Assistant");
    QCOMPARE(tabWidget.indexOf(embedded), index);
    QCOMPARE(tabWidget.widget(index), static_cast<QWidget *>(embedded));
    QCOMPARE(tabWidget.tabText(index), QString("DX Assistant"));

    // Disabling the feature only removes the tab; the page survives, which is
    // what lets MainWindow bring back the spots it holds when re-enabled.
    tabWidget.removeTab(index);
    QCOMPARE(tabWidget.indexOf(embedded), -1);
    QCOMPARE(tabWidget.count(), 0);
    embedded->setParent(nullptr);
    delete embedded;
}

QTEST_MAIN(tst_DXClusterAssistant)

#include "tst_dxclusterassistant.moc"
