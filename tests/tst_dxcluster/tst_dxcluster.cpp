/***************************************************************************
                          tst_dxcluster.cpp  -  description
                             -------------------
    begin                : Mar 2026
    copyright            : (C) 2026 by Jaime Robles
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

/*
 * Regression tests for issue #886:
 *   "DXCluster does not filter confirmed spots"
 *
 * Root cause: in DXClusterWidget::printSpot(), _entityStatus.status was never
 * set before calling checkIfNeedsToBePrinted(), so the filter checking
 * `_entityStatus.status == confirmed` was always false.
 *
 * Fix:
 *   1. _entityStatus.status is now set via
 *      awards->getQSOStatus(awards->getDXStatus(_entityStatus))
 *      after computing the spot colour.
 *   2. A missing `showworked` check was added to checkIfNeedsToBePrinted().
 *
 * These tests verify:
 *   A) Awards::getQSOStatus() correctly maps the integer returned by
 *      getDXStatus() to the QSOStatus enum, especially that 13 → confirmed.
 *   B) DXClusterWidget::checkIfNeedsToBePrinted() filters confirmed spots
 *      when showconfirmed is false, and shows them when it is true.
 *   C) Same behaviour for worked spots and the showworked flag.
 *   D) Default configuration (all flags true) never filters any status.
 */

#include <QtTest>
#include "../../src/awards.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/dxcluster/dxcluster.h"
#include "../../src/klogdefinitions.h"
#include "../../src/world.h"

class tst_DXCluster : public QObject
{
    Q_OBJECT

public:
    tst_DXCluster();
    ~tst_DXCluster();

private slots:
    void initTestCase();
    void cleanupTestCase();

    // A) Awards::getQSOStatus() mapping — the function added to the fix
    void test_getQSOStatusMapping();
    void test_getQSOStatusMapping_data();

    // B) confirmed-spot filter (issue #886 primary case)
    void test_confirmedSpotFilteredWhenFlagDisabled();
    void test_confirmedSpotShownWhenFlagEnabled();

    // C) worked-spot filter (secondary fix in the same commit)
    void test_workedSpotFilteredWhenFlagDisabled();
    void test_workedSpotShownWhenFlagEnabled();

    // D) default config — every status must pass through
    void test_defaultConfigShowsAllStatuses();
    void test_defaultConfigShowsAllStatuses_data();

private:
    // Helper: build an EntityStatus with the given QSOStatus and bandId=-1.
    // bandId=-1 guarantees that no band-based filter can interfere with the
    // tests (isHF/isVHF/isWARC return false for an unknown band id when all
    // band-show flags are true, which is the default).
    EntityStatus makeStatus(QSOStatus status) const;

    DataProxy_SQLite *dataProxy = nullptr;
    World            *world     = nullptr;
    Awards           *awards    = nullptr;
    DXClusterWidget  *widget    = nullptr;
};

// ─────────────────────────────────────────────────────────────────────────────
tst_DXCluster::tst_DXCluster() = default;
tst_DXCluster::~tst_DXCluster() = default;

void tst_DXCluster::initTestCase()
{
    // Follow the same lightweight setup used in tst_logwindow: create the
    // objects in dependency order without any pre-populated database content.
    // The tests that exercise checkIfNeedsToBePrinted() only rely on the
    // filter-flag members (showconfirmed, showworked, …) and do not reach any
    // SQL path when bandId == -1 and all band-show flags are true (default).
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    world     = new World(dataProxy, Q_FUNC_INFO);
    awards    = new Awards(dataProxy, world, nullptr);
    widget    = new DXClusterWidget(awards, world);
}

void tst_DXCluster::cleanupTestCase()
{
    delete widget;
    delete awards;
    delete world;
    delete dataProxy;
}

EntityStatus tst_DXCluster::makeStatus(QSOStatus status) const
{
    EntityStatus es;
    es.dxcc   = 1;   // arbitrary valid-looking DXCC
    es.bandId = -1;  // unknown band → band filters are bypassed
    es.modeId = -1;
    es.logId  = 0;
    es.status = status;
    return es;
}

// ─────────────────────────────────────────────────────────────────────────────
// A) Awards::getQSOStatus() mapping
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_getQSOStatusMapping_data()
{
    QTest::addColumn<int>("dxStatusInt");
    QTest::addColumn<int>("expectedQsoStatus");

    // The integer codes come from Awards::getDXStatus() comments.
    // The critical mapping for issue #886 is 13 → confirmed.
    QTest::newRow("0  → ATNO")      << 0  << static_cast<int>(ATNO);
    QTest::newRow("1  → needed")    << 1  << static_cast<int>(needed);
    QTest::newRow("2  → needed")    << 2  << static_cast<int>(needed);
    QTest::newRow("3  → worked")    << 3  << static_cast<int>(worked);
    QTest::newRow("4  → needed")    << 4  << static_cast<int>(needed);
    QTest::newRow("5  → needed")    << 5  << static_cast<int>(needed);
    QTest::newRow("6  → needed")    << 6  << static_cast<int>(needed);
    QTest::newRow("7  → needed")    << 7  << static_cast<int>(needed);
    QTest::newRow("8  → worked")    << 8  << static_cast<int>(worked);
    QTest::newRow("9  → needed")    << 9  << static_cast<int>(needed);
    QTest::newRow("10 → worked")    << 10 << static_cast<int>(worked);
    QTest::newRow("11 → needed")    << 11 << static_cast<int>(needed);
    QTest::newRow("12 → worked")    << 12 << static_cast<int>(worked);
    QTest::newRow("13 → confirmed") << 13 << static_cast<int>(confirmed); // KEY: #886
    QTest::newRow("-1 → unknown")   << -1 << static_cast<int>(unknown);
    QTest::newRow("99 → unknown")   << 99 << static_cast<int>(unknown);
}

void tst_DXCluster::test_getQSOStatusMapping()
{
    QFETCH(int, dxStatusInt);
    QFETCH(int, expectedQsoStatus);

    QSOStatus result = awards->getQSOStatus(dxStatusInt);
    QCOMPARE(static_cast<int>(result), expectedQsoStatus);
}

// ─────────────────────────────────────────────────────────────────────────────
// B) confirmed-spot filter
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_confirmedSpotFilteredWhenFlagDisabled()
{
    // Regression test for issue #886:
    // When showconfirmed is false a spot whose entity is already confirmed
    // must be filtered out (checkIfNeedsToBePrinted must return false).
    widget->setDXClusterSpotConfig(
        /*showhf*/      true,
        /*showvhf*/     true,
        /*showwarc*/    true,
        /*showworked*/  true,
        /*showconfirmed*/ false,   // <-- the flag under test
        /*showann*/     true,
        /*showwwv*/     true,
        /*showwcy*/     true
    );

    EntityStatus es = makeStatus(confirmed);
    QVERIFY2(!widget->checkIfNeedsToBePrinted(es),
             "confirmed spot must be filtered when showconfirmed is false");
}

void tst_DXCluster::test_confirmedSpotShownWhenFlagEnabled()
{
    widget->setDXClusterSpotConfig(true, true, true, true, /*showconfirmed*/true, true, true, true);

    EntityStatus es = makeStatus(confirmed);
    QVERIFY2(widget->checkIfNeedsToBePrinted(es),
             "confirmed spot must be shown when showconfirmed is true");
}

// ─────────────────────────────────────────────────────────────────────────────
// C) worked-spot filter
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_workedSpotFilteredWhenFlagDisabled()
{
    widget->setDXClusterSpotConfig(
        true, true, true,
        /*showworked*/ false,   // <-- the flag under test
        true, true, true, true
    );

    EntityStatus es = makeStatus(worked);
    QVERIFY2(!widget->checkIfNeedsToBePrinted(es),
             "worked spot must be filtered when showworked is false");
}

void tst_DXCluster::test_workedSpotShownWhenFlagEnabled()
{
    widget->setDXClusterSpotConfig(true, true, true, /*showworked*/true, true, true, true, true);

    EntityStatus es = makeStatus(worked);
    QVERIFY2(widget->checkIfNeedsToBePrinted(es),
             "worked spot must be shown when showworked is true");
}

// ─────────────────────────────────────────────────────────────────────────────
// D) default configuration — all statuses pass through
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_defaultConfigShowsAllStatuses_data()
{
    QTest::addColumn<int>("status");

    QTest::newRow("unknown")   << static_cast<int>(unknown);
    QTest::newRow("ATNO")      << static_cast<int>(ATNO);
    QTest::newRow("needed")    << static_cast<int>(needed);
    QTest::newRow("worked")    << static_cast<int>(worked);
    QTest::newRow("confirmed") << static_cast<int>(confirmed);
}

void tst_DXCluster::test_defaultConfigShowsAllStatuses()
{
    // Restore default: show everything
    widget->setDXClusterSpotConfig(true, true, true, true, true, true, true, true);

    QFETCH(int, status);
    EntityStatus es = makeStatus(static_cast<QSOStatus>(status));
    QVERIFY2(widget->checkIfNeedsToBePrinted(es),
             "default config must show spots of any status");
}

QTEST_MAIN(tst_DXCluster)
#include "tst_dxcluster.moc"
