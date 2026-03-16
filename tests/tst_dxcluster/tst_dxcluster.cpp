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
 *   E) Non-filtered statuses (unknown/ATNO/needed) always pass through even
 *      when the worked and confirmed filters are both active.
 *   F) DXClusterWidget::cleanSpotter() removes the enclosing < and >.
 *   G) DXClusterWidget::parseReceivedData() classifies "DX de", sh/dx, and
 *      other strings correctly.
 *   H) DXClusterWidget::readItem() correctly parses "DX de" format spots.
 *   I) DXClusterWidget::readItem() correctly parses sh/dx processed spots.
 *   J) DXClusterWidget::readItem() returns an invalid DXSpot for strings
 *      that are too short to parse.
 *   K) DXClusterWidget::isConnected() reflects the dxClusterConnected member.
 *   L) DXClusterWidget::setDXClusterSpotConfig() stores all eight flags.
 *   M) DXClusterWidget::setDXClusterServer() stores valid server/port and
 *      falls back to safe defaults when the inputs are invalid.
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

    // E) unknown/ATNO/needed must pass even with worked+confirmed filters active
    void test_otherStatusesAlwaysShown();
    void test_otherStatusesAlwaysShown_data();

    // F) cleanSpotter — stripping < and >
    void test_cleanSpotter();
    void test_cleanSpotter_data();

    // G) parseReceivedData — spot type classification
    void test_parseReceivedData();
    void test_parseReceivedData_data();

    // H) readItem — "DX de" format
    void test_readItem_dxdeValidSpot();
    void test_readItem_dxdeWithComment();
    void test_readItem_dxdeInvalidFreq();

    // I) readItem — sh/dx processed format (no trailing > in spotter)
    void test_readItem_shdxValidSpot();

    // J) readItem — strings that are too short return an invalid DXSpot
    void test_readItem_tooShort();
    void test_readItem_tooShort_data();

    // K) isConnected
    void test_isConnected();

    // L) setDXClusterSpotConfig stores all eight flags
    void test_setDXClusterSpotConfig_storesAllFlags();

    // M) setDXClusterServer — valid input and fallback
    void test_setDXClusterServer_valid();
    void test_setDXClusterServer_fallbackOnShortServer();
    void test_setDXClusterServer_fallbackOnZeroPort();

    // N) Locator guard + fallback — regression for issue #581
    void test_locatorGuard_emptyStringIsInvalid();
    void test_locatorFallback_entityLocatorValidForKnownCall();
    void test_locatorPrimary_getQRZLocatorValidForKnownCall();

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

// ─────────────────────────────────────────────────────────────────────────────
// E) unknown/ATNO/needed must pass even with worked+confirmed filters active
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_otherStatusesAlwaysShown_data()
{
    QTest::addColumn<int>("status");

    QTest::newRow("unknown") << static_cast<int>(unknown);
    QTest::newRow("ATNO")    << static_cast<int>(ATNO);
    QTest::newRow("needed")  << static_cast<int>(needed);
}

void tst_DXCluster::test_otherStatusesAlwaysShown()
{
    // Both worked and confirmed filters are off; only those two statuses
    // should be suppressed — unknown/ATNO/needed must still be shown.
    widget->setDXClusterSpotConfig(
        true, true, true,
        /*showworked*/    false,
        /*showconfirmed*/ false,
        true, true, true
    );

    QFETCH(int, status);
    EntityStatus es = makeStatus(static_cast<QSOStatus>(status));
    QVERIFY2(widget->checkIfNeedsToBePrinted(es),
             "unknown/ATNO/needed spots must be shown even when worked+confirmed filters are active");
}

// ─────────────────────────────────────────────────────────────────────────────
// F) cleanSpotter
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_cleanSpotter_data()
{
    QTest::addColumn<QString>("raw");
    QTest::addColumn<QString>("expected");

    QTest::newRow("simple")       << "<EA4K>"   << "EA4K";
    QTest::newRow("portable")     << "<W1AW>"   << "W1AW";
    QTest::newRow("numeric")      << "<2E0MNG>" << "2E0MNG";
    QTest::newRow("long_prefix")  << "<VK2ABC>" << "VK2ABC";
}

void tst_DXCluster::test_cleanSpotter()
{
    QFETCH(QString, raw);
    QFETCH(QString, expected);

    QString result = widget->cleanSpotter(raw);
    QCOMPARE(result, expected);
}

// ─────────────────────────────────────────────────────────────────────────────
// G) parseReceivedData — spot type classification
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_parseReceivedData_data()
{
    QTest::addColumn<QString>("raw");
    QTest::addColumn<int>("expectedType");

    // "DX de" strings
    QTest::newRow("dxde_basic")
        << "DX de EA4K: 14250.0 IZ3WUW 1429Z"
        << static_cast<int>(dxde);
    QTest::newRow("dxde_with_comment")
        << "DX de EA4K: 14250.0 IZ3WUW good DX 73 1430Z"
        << static_cast<int>(dxde);

    // sh/dx strings: valid frequency in first field, last token ends with '>'
    QTest::newRow("shdx_basic")
        << "14250.0 IZ3WUW 14-Mar-2026 1429Z <EA4K>"
        << static_cast<int>(shdx);
    QTest::newRow("shdx_with_comment")
        << "14250.0 IZ3WUW 14-Mar-2026 1429Z FT8 -12dB <EA4K>"
        << static_cast<int>(shdx);
    QTest::newRow("shdx_7mhz")
        << "7160.0 GB70RS 19-Feb-2025 1202Z SES <2E0MNG>"
        << static_cast<int>(shdx);

    // "other" strings
    QTest::newRow("other_text")
        << "To all: welcome to the DX cluster"
        << static_cast<int>(other);
    QTest::newRow("other_singleword")
        << "singleword"
        << static_cast<int>(other);
    QTest::newRow("other_freq_no_angle")
        << "14250.0 IZ3WUW 14-Mar-2026 1429Z EA4K"
        << static_cast<int>(other);
}

void tst_DXCluster::test_parseReceivedData()
{
    QFETCH(QString, raw);
    QFETCH(int, expectedType);

    TypeOfDXSpot result = widget->parseReceivedData(raw);
    QCOMPARE(static_cast<int>(result), expectedType);
}

// ─────────────────────────────────────────────────────────────────────────────
// H) readItem — "DX de" format
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_readItem_dxdeValidSpot()
{
    // Minimum valid "DX de" string (no comment between dxcall and time).
    // Format: "DX de SPOTTER: FREQ_KHZ DXCALL HHmmZ"
    DXSpot spot = widget->readItem("DX de EA4K: 14250.0 IZ3WUW 1429Z");

    QVERIFY2(spot.isValid(), "readItem must produce a valid DXSpot for a well-formed DX-de string");
    QCOMPARE(spot.getDxCall(),  QString("IZ3WUW"));
    QCOMPARE(spot.getSpotter(), QString("EA4K"));
    QVERIFY2(!spot.getSHDX(),   "DX-de spot must not have the SHDX flag set");

    // Frequency must be 14.250 MHz (= 14250 kHz)
    Frequency f = spot.getFrequency();
    QVERIFY2(f.isValid(), "frequency must be valid");
    QVERIFY2(qAbs(f.toDouble(MHz) - 14.250) < 0.001,
             "frequency must be 14.250 MHz");

    // Time: "1429Z" → 14:29
    QCOMPARE(spot.getDateTime().time().hour(),   14);
    QCOMPARE(spot.getDateTime().time().minute(), 29);
}

void tst_DXCluster::test_readItem_dxdeWithComment()
{
    // "DX de" string that carries a comment between dxcall and time.
    DXSpot spot = widget->readItem("DX de W1AW: 7160.0 G3ZZZ good DX 73 1530Z");

    QVERIFY2(spot.isValid(), "readItem must produce a valid DXSpot for a DX-de string with a comment");
    QCOMPARE(spot.getDxCall(),  QString("G3ZZZ"));
    QCOMPARE(spot.getSpotter(), QString("W1AW"));
    QCOMPARE(spot.getComment(), QString("good DX 73"));
    QVERIFY2(!spot.getSHDX(), "DX-de spot must not have the SHDX flag set");
}

void tst_DXCluster::test_readItem_dxdeInvalidFreq()
{
    // If the frequency field is not a valid number the spot must be invalid.
    DXSpot spot = widget->readItem("DX de EA4K: NOTAFREQ IZ3WUW test 1429Z");
    QVERIFY2(!spot.isValid(), "readItem must return an invalid DXSpot when the frequency field is not numeric");
}

// ─────────────────────────────────────────────────────────────────────────────
// I) readItem — sh/dx processed format
//
// printSHDX() splits the multi-spot string on '>' and passes each individual
// sub-string (without the closing '>') to printSpot() / readItem().  The
// spotter token therefore looks like "<CALLSIGN" (no trailing '>') by the
// time readItem() receives it.
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_readItem_shdxValidSpot()
{
    // Simulates the string that printSHDX() would hand to printSpot() after
    // splitting "14250.0 IZ3WUW 14-Mar-2026 1429Z FT8 <EA4K>" on '>'.
    DXSpot spot = widget->readItem("14250.0 IZ3WUW 14-Mar-2026 1429Z FT8 <EA4K");

    QVERIFY2(spot.isValid(), "readItem must produce a valid DXSpot for a sh/dx processed string");
    QCOMPARE(spot.getDxCall(),  QString("IZ3WUW"));
    QCOMPARE(spot.getSpotter(), QString("EA4K"));
    QVERIFY2(spot.getSHDX(), "sh/dx spot must have the SHDX flag set");

    Frequency f = spot.getFrequency();
    QVERIFY2(f.isValid(), "frequency must be valid");
    QVERIFY2(qAbs(f.toDouble(MHz) - 14.250) < 0.001,
             "frequency must be 14.250 MHz");
}

// ─────────────────────────────────────────────────────────────────────────────
// J) readItem — strings that are too short return an invalid DXSpot
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_readItem_tooShort_data()
{
    QTest::addColumn<QString>("raw");

    QTest::newRow("empty")           << "";
    QTest::newRow("one_word")        << "DX";
    QTest::newRow("three_words")     << "DX de EA4K";
    QTest::newRow("four_words")      << "DX de EA4K: 14250.0";
}

void tst_DXCluster::test_readItem_tooShort()
{
    QFETCH(QString, raw);
    DXSpot spot = widget->readItem(raw);
    QVERIFY2(!spot.isValid(),
             "readItem must return an invalid DXSpot when fewer than 5 fields are present");
}

// ─────────────────────────────────────────────────────────────────────────────
// K) isConnected reflects the dxClusterConnected member
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_isConnected()
{
    // Use the friend relationship to set the internal flag directly and then
    // verify that the public isConnected() accessor returns the right value.
    widget->dxClusterConnected = false;
    QVERIFY2(!widget->isConnected(), "isConnected() must return false when dxClusterConnected is false");

    widget->dxClusterConnected = true;
    QVERIFY2(widget->isConnected(), "isConnected() must return true when dxClusterConnected is true");

    widget->dxClusterConnected = false; // restore
}

// ─────────────────────────────────────────────────────────────────────────────
// L) setDXClusterSpotConfig stores all eight flags
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_setDXClusterSpotConfig_storesAllFlags()
{
    // Set all flags to false and verify each member.
    widget->setDXClusterSpotConfig(false, false, false, false, false, false, false, false);
    QVERIFY2(!widget->showhf,       "showhf must be false");
    QVERIFY2(!widget->showvhf,      "showvhf must be false");
    QVERIFY2(!widget->showwarc,     "showwarc must be false");
    QVERIFY2(!widget->showworked,   "showworked must be false");
    QVERIFY2(!widget->showconfirmed,"showconfirmed must be false");
    QVERIFY2(!widget->showann,      "showann must be false");
    QVERIFY2(!widget->showwwv,      "showwwv must be false");
    QVERIFY2(!widget->showwcy,      "showwcy must be false");

    // Set all flags to true and verify each member.
    widget->setDXClusterSpotConfig(true, true, true, true, true, true, true, true);
    QVERIFY2(widget->showhf,       "showhf must be true");
    QVERIFY2(widget->showvhf,      "showvhf must be true");
    QVERIFY2(widget->showwarc,     "showwarc must be true");
    QVERIFY2(widget->showworked,   "showworked must be true");
    QVERIFY2(widget->showconfirmed,"showconfirmed must be true");
    QVERIFY2(widget->showann,      "showann must be true");
    QVERIFY2(widget->showwwv,      "showwwv must be true");
    QVERIFY2(widget->showwcy,      "showwcy must be true");

    // Verify that individual flags are stored independently.
    widget->setDXClusterSpotConfig(true, false, true, false, true, false, true, false);
    QVERIFY2( widget->showhf,       "showhf must be true");
    QVERIFY2(!widget->showvhf,      "showvhf must be false");
    QVERIFY2( widget->showwarc,     "showwarc must be true");
    QVERIFY2(!widget->showworked,   "showworked must be false");
    QVERIFY2( widget->showconfirmed,"showconfirmed must be true");
    QVERIFY2(!widget->showann,      "showann must be false");
    QVERIFY2( widget->showwwv,      "showwwv must be true");
    QVERIFY2(!widget->showwcy,      "showwcy must be false");

    // Restore defaults for subsequent tests.
    widget->setDXClusterSpotConfig(true, true, true, true, true, true, true, true);
}

// ─────────────────────────────────────────────────────────────────────────────
// M) setDXClusterServer — valid input and fallback
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_setDXClusterServer_valid()
{
    widget->setDXClusterServer("cluster.example.com", 7373);
    QCOMPARE(widget->server,       QString("cluster.example.com"));
    QCOMPARE(static_cast<int>(widget->port), 7373);
}

void tst_DXCluster::test_setDXClusterServer_fallbackOnShortServer()
{
    // A server name shorter than 3 characters is invalid; the widget must
    // fall back to "dxfun.com" / 8000.
    widget->setDXClusterServer("ab", 9000);
    QCOMPARE(widget->server,       QString("dxfun.com"));
    QCOMPARE(static_cast<int>(widget->port), 8000);
}

void tst_DXCluster::test_setDXClusterServer_fallbackOnZeroPort()
{
    // Port 0 is invalid; the widget must fall back to "dxfun.com" / 8000.
    widget->setDXClusterServer("valid.cluster.com", 0);
    QCOMPARE(widget->server,       QString("dxfun.com"));
    QCOMPARE(static_cast<int>(widget->port), 8000);
}

// ─────────────────────────────────────────────────────────────────────────────
// N) Locator guard + fallback — regression for issue #581
//
// slotDXClusterSpotArrived() was fixed to:
//   1. Accept an empty/invalid locator from getQRZLocator() and fall back to
//      world->getLocator(world->getQRZARRLId(call)).
//   2. Skip the map marker entirely when neither path yields a valid locator.
//
// These tests verify the two building blocks so that a future refactor cannot
// silently break the guard without the tests catching it.
// ─────────────────────────────────────────────────────────────────────────────

void tst_DXCluster::test_locatorGuard_emptyStringIsInvalid()
{
    // The guard uses Locator::isValidLocator() to reject empty strings.
    // An empty locator must never be sent to the map.
    Locator locator;
    QVERIFY2(!locator.isValidLocator(QString()),
             "Empty string must not be a valid locator");
    QVERIFY2(!locator.isValidLocator(""),
             "Empty literal string must not be a valid locator");
}

void tst_DXCluster::test_locatorFallback_entityLocatorValidForKnownCall()
{
    // When getQRZLocator() returns nothing useful, the fallback path calls
    // world->getLocator(world->getQRZARRLId(call)).
    // For any well-known callsign the entity id must be positive and the
    // resulting locator string must be non-empty and valid.
    Locator locator;

    // K1AA → entity 291 (United States)
    int entityId = world->getQRZARRLId("K1AA");
    QVERIFY2(entityId > 0, "getQRZARRLId must return a positive id for K1AA");

    QString fallbackLocator = world->getLocator(entityId);
    QVERIFY2(!fallbackLocator.isEmpty(),
             "Entity locator must not be empty for a known DXCC entity");
    QVERIFY2(locator.isValidLocator(fallbackLocator),
             "Entity locator returned by world->getLocator() must be a valid Maidenhead locator");
}

void tst_DXCluster::test_locatorPrimary_getQRZLocatorValidForKnownCall()
{
    // The primary path calls world->getQRZLocator(call).
    // For a well-known callsign it must return a valid Maidenhead locator so
    // that the fallback is not needed.
    Locator locator;

    QString primaryLocator = world->getQRZLocator("EA4K");
    QVERIFY2(!primaryLocator.isEmpty(),
             "getQRZLocator must return a non-empty locator for a known callsign");
    QVERIFY2(locator.isValidLocator(primaryLocator),
             "getQRZLocator must return a valid Maidenhead locator for a known callsign");
}

QTEST_MAIN(tst_DXCluster)
#include "tst_dxcluster.moc"
