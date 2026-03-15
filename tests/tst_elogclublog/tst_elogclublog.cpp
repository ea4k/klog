/***************************************************************************
                          tst_elogclublog.cpp  -  description
                             -------------------
    begin                : Mar 2026
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
#include "../../src/elog/elogclublog.h"
#include "../../src/utilities.h"

// Subclass to expose the protected getClubLogAdif() method for testing
class TestableClubLog : public eLogClubLog
{
public:
    QString testGetClubLogAdif(const QStringList &q) { return getClubLogAdif(q); }
};

class tst_eLogClubLog : public QObject
{
    Q_OBJECT

public:
    tst_eLogClubLog();
    ~tst_eLogClubLog();

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Regression test for issue #257: User-Agent must contain "KLog"
    void test_setVersionStoresValue();
    void test_userAgentContainsKLog();

    // Tests for getClubLogAdif() output correctness
    void test_getClubLogAdif_validInput();
    void test_getClubLogAdif_invalidLength();
    void test_getClubLogAdif_invalidDate();
    void test_getClubLogAdif_emptyOptionalFields();
    void test_getClubLogAdif_containsRequiredFields();

private:
    TestableClubLog *clublog;
    Utilities *util;

    // Builds a valid 16-element QSO list for getClubLogAdif()
    // Fields: date, timeOn, qslrdate, qslsdate, call, operator,
    //         mode, band, bandRx, freq, qslRcvd, lotwQslRcvd,
    //         qslSent, dxcc, propMode, creditGranted
    QStringList buildValidQsoList();
};

tst_eLogClubLog::tst_eLogClubLog()
{
    clublog = new TestableClubLog();
    util    = new Utilities(Q_FUNC_INFO);
}

tst_eLogClubLog::~tst_eLogClubLog()
{
    delete clublog;
    delete util;
}

void tst_eLogClubLog::initTestCase()
{
    util->setLogLevel(Devel);
}

void tst_eLogClubLog::cleanupTestCase() {}

QStringList tst_eLogClubLog::buildValidQsoList()
{
    // 16 fields expected by getClubLogAdif()
    QStringList q;
    q << "20210601"   // 0  QSO_DATE
      << "1230"       // 1  TIME_ON
      << ""           // 2  QSLRDATE  (optional)
      << ""           // 3  QSLSDATE  (optional)
      << "EA4K"       // 4  CALL
      << ""           // 5  OPERATOR  (optional)
      << "SSB"        // 6  MODE
      << "20m"        // 7  BAND
      << ""           // 8  BAND_RX   (optional)
      << ""           // 9  FREQ      (optional)
      << "N"          // 10 QSL_RCVD
      << "N"          // 11 LOTW_QSL_RCVD
      << "N"          // 12 QSL_SENT  (skipped in current implementation)
      << "0"          // 13 DXCC      (0 → omitted)
      << "0"          // 14 PROP_MODE (0 → omitted)
      << "";          // 15 CREDIT_GRANTED (optional)
    return q;
}

// ----- Regression tests for issue #257 -----

void tst_eLogClubLog::test_setVersionStoresValue()
{
    // After setVersion(), the agent string produced by Utilities must contain
    // that version so it can be embedded in the User-Agent header.
    const QString version = "2.4.4";
    clublog->setVersion(version);
    // Verify indirectly: Utilities::getAgent() must include the version.
    QString agent = util->getAgent(version);
    QVERIFY2(agent.contains(version),
             qPrintable(QString("User-Agent '%1' does not contain version '%2'").arg(agent, version)));
    QVERIFY2(agent.startsWith("KLog"),
             qPrintable(QString("User-Agent '%1' does not start with 'KLog'").arg(agent)));
}

void tst_eLogClubLog::test_userAgentContainsKLog()
{
    // Utilities::getAgent() is the function used to build the User-Agent.
    // It must always return a string that starts with "KLog" so that ClubLog
    // can identify the software in its statistics (issue #257).
    const QStringList versions = {"", "1.0", "2.4.4", "99.99"};
    for (const QString &ver : versions)
    {
        QString agent = util->getAgent(ver);
        QVERIFY2(agent.startsWith("KLog"),
                 qPrintable(QString("User-Agent for version '%1' is '%2' — must start with 'KLog'")
                                .arg(ver, agent)));
    }
}

// ----- Tests for getClubLogAdif() -----

void tst_eLogClubLog::test_getClubLogAdif_validInput()
{
    QStringList q = buildValidQsoList();
    QString adif = clublog->testGetClubLogAdif(q);
    QVERIFY2(!adif.isEmpty(), "getClubLogAdif() returned empty string for valid input");
    QVERIFY2(adif.contains("<EOR>"), "ADIF record must end with <EOR>");
}

void tst_eLogClubLog::test_getClubLogAdif_invalidLength()
{
    // Anything other than 16 fields must return an empty string
    QStringList tooShort;
    tooShort << "20210601" << "1230";
    QVERIFY2(clublog->testGetClubLogAdif(tooShort).isEmpty(),
             "Expected empty string for list with fewer than 16 fields");

    QStringList tooLong = buildValidQsoList();
    tooLong << "extra";
    QVERIFY2(clublog->testGetClubLogAdif(tooLong).isEmpty(),
             "Expected empty string for list with more than 16 fields");
}

void tst_eLogClubLog::test_getClubLogAdif_invalidDate()
{
    QStringList q = buildValidQsoList();
    q[0] = "not-a-date";
    QVERIFY2(clublog->testGetClubLogAdif(q).isEmpty(),
             "Expected empty string when QSO_DATE is invalid");
}

void tst_eLogClubLog::test_getClubLogAdif_emptyOptionalFields()
{
    QStringList q = buildValidQsoList();
    // Optional fields (QSLRDATE, QSLSDATE, OPERATOR) are empty → must NOT appear in output
    QString adif = clublog->testGetClubLogAdif(q);
    QVERIFY2(!adif.contains("<QSLRDATE:"), "Empty QSLRDATE must not appear in ADIF");
    QVERIFY2(!adif.contains("<QSLSDATE:"), "Empty QSLSDATE must not appear in ADIF");
    QVERIFY2(!adif.contains("<OPERATOR:"), "Empty OPERATOR must not appear in ADIF");
}

void tst_eLogClubLog::test_getClubLogAdif_containsRequiredFields()
{
    QStringList q = buildValidQsoList();
    QString adif = clublog->testGetClubLogAdif(q);
    QVERIFY2(adif.contains("<QSO_DATE:"),    "ADIF must contain QSO_DATE");
    QVERIFY2(adif.contains("<TIME_ON:"),     "ADIF must contain TIME_ON");
    QVERIFY2(adif.contains("<CALL:"),        "ADIF must contain CALL");
    QVERIFY2(adif.contains("<MODE:"),        "ADIF must contain MODE");
    QVERIFY2(adif.contains("<BAND:"),        "ADIF must contain BAND");
    QVERIFY2(adif.contains("<QSL_RCVD:"),    "ADIF must contain QSL_RCVD");
    QVERIFY2(adif.contains("<LOTW_QSL_RCVD:"), "ADIF must contain LOTW_QSL_RCVD");
}

QTEST_MAIN(tst_eLogClubLog)
#include "tst_elogclublog.moc"
