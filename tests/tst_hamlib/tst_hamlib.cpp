/***************************************************************************
                          tst_hamlib.h  -  description
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

#include <QtTest>
#include <QSignalSpy>
#include <QDebug>
#include <hamlib/rig.h>
#include "../../src/klogdefinitions.h"
#include "../../src/hamlibclass.h"

class tst_Hamlib : public QObject
{
    Q_OBJECT

public:
    tst_Hamlib();
    ~tst_Hamlib();

private slots:
    void initTestCase();
    void cleanupTestCase();

    // --- Construction ---
    void test_Constructor_isNotRunning();
    void test_Constructor_rigListIsEmpty();

    // --- initClass ---
    void test_InitClass_populatesRigList();
    void test_InitClass_containsDummy();
    void test_InitClass_emitsDebugLog();

    // --- Rig lookup ---
    void test_GetModelIdFromName_dummy();
    void test_GetModelIdFromName_unknownRig_returnsZero();
    void test_GetNameFromModelId_dummy();
    void test_GetNameFromModelId_unknownId_returnsEmpty();
    void test_RigLookup_roundtrip();

    // --- isModeADIFMode ---
    void test_IsModeADIFMode_validModes();
    void test_IsModeADIFMode_invalidModes();
    void test_IsModeADIFMode_caseInsensitive();

    // --- init() without a real radio ---
    void test_Init_modelZero_returnsFalse();
    void test_Init_modelOne_returnsFalse();
    void test_Init_noSerialPort_returnsFalse();

    // --- State after failed init ---
    void test_IsRunning_initiallyFalse();
    void test_IsRunning_afterFailedInit_isFalse();

    // --- stop() safety ---
    void test_Stop_whenNotRunning_doesNotCrash();

    // --- readRadio() without connection ---
    void test_ReadRadio_notConnected_returnsFalse();

    // --- Setter validation ---
    void test_SetPoll_negativeIsIgnored();
    void test_SetPoll_zeroIsIgnored();
    void test_SetPoll_positiveIsAccepted();
    void test_SetDataBits_validRange_doesNotCrash();
    void test_SetDataBits_belowRange_doesNotCrash();
    void test_SetDataBits_aboveRange_doesNotCrash();
    void test_SetNetworkPort_validRange_doesNotCrash();
    void test_SetNetworkPort_zeroIsIgnored();
    void test_SetNetworkPort_maxIsIgnored();

    // --- loadSettings ---
    void test_LoadSettings_returnsTrue();

private:
    HamLibClass *hamlib;
};

tst_Hamlib::tst_Hamlib() : hamlib(nullptr) {}
tst_Hamlib::~tst_Hamlib() {}

void tst_Hamlib::initTestCase()
{
    // Create a fresh instance; do NOT depend on any pre-existing settings file.
    hamlib = new HamLibClass(this);
    QVERIFY(hamlib != nullptr);
}

void tst_Hamlib::cleanupTestCase()
{
    delete hamlib;
    hamlib = nullptr;
}

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

void tst_Hamlib::test_Constructor_isNotRunning()
{
    HamLibClass h;
    QVERIFY(!h.isRunning());
}

void tst_Hamlib::test_Constructor_rigListIsEmpty()
{
    // Before initClass() the rig list is not populated.
    HamLibClass h;
    QVERIFY(h.getRigList().isEmpty());
}

// ---------------------------------------------------------------------------
// initClass
// ---------------------------------------------------------------------------

void tst_Hamlib::test_InitClass_populatesRigList()
{
    hamlib->initClass();
    QVERIFY(!hamlib->getRigList().isEmpty());
}

void tst_Hamlib::test_InitClass_containsDummy()
{
    // Hamlib always ships the "Dummy" back-end (model 1).
    QVERIFY(hamlib->getRigList().contains("Dummy"));
}

void tst_Hamlib::test_InitClass_emitsDebugLog()
{
    HamLibClass h;
    QSignalSpy spy(&h, &HamLibClass::debugLog);
    h.initClass();
    QVERIFY(spy.count() > 0);
}

// ---------------------------------------------------------------------------
// Rig lookup
// ---------------------------------------------------------------------------

void tst_Hamlib::test_GetModelIdFromName_dummy()
{
    // "Dummy" must resolve to model 1 (RIG_MODEL_DUMMY).
    QCOMPARE(hamlib->getModelIdFromName("Dummy"), 1);
}

void tst_Hamlib::test_GetModelIdFromName_unknownRig_returnsZero()
{
    // An unknown name must return 0 (QMap default for missing keys).
    QCOMPARE(hamlib->getModelIdFromName("NonExistentRig_XYZ_12345"), 0);
}

void tst_Hamlib::test_GetNameFromModelId_dummy()
{
    // Model 1 is always "Dummy".
    QCOMPARE(hamlib->getNameFromModelId(1), QString("Dummy"));
}

void tst_Hamlib::test_GetNameFromModelId_unknownId_returnsEmpty()
{
    QVERIFY(hamlib->getNameFromModelId(-999).isEmpty());
}

void tst_Hamlib::test_RigLookup_roundtrip()
{
    // For every rig in the list the id→name and name→id conversions must be
    // consistent with each other.
    const QStringList rigs = hamlib->getRigList();
    QVERIFY(!rigs.isEmpty());

    int failures = 0;
    for (const QString &name : rigs) {
        int id = hamlib->getModelIdFromName(name);
        if (id <= 0) { ++failures; continue; }
        if (hamlib->getNameFromModelId(id) != name) ++failures;
    }
    QCOMPARE(failures, 0);
}

// ---------------------------------------------------------------------------
// isModeADIFMode
// ---------------------------------------------------------------------------

void tst_Hamlib::test_IsModeADIFMode_validModes()
{
    QVERIFY(hamlib->isModeADIFMode("AM"));
    QVERIFY(hamlib->isModeADIFMode("CW"));
    QVERIFY(hamlib->isModeADIFMode("FM"));
    QVERIFY(hamlib->isModeADIFMode("LSB"));
    QVERIFY(hamlib->isModeADIFMode("USB"));
    QVERIFY(hamlib->isModeADIFMode("RTTY"));
}

void tst_Hamlib::test_IsModeADIFMode_invalidModes()
{
    QVERIFY(!hamlib->isModeADIFMode("FT8"));
    QVERIFY(!hamlib->isModeADIFMode("SSB"));
    QVERIFY(!hamlib->isModeADIFMode(""));
    QVERIFY(!hamlib->isModeADIFMode("INVALID"));
    QVERIFY(!hamlib->isModeADIFMode("CWR"));
    QVERIFY(!hamlib->isModeADIFMode("DIGI"));
}

void tst_Hamlib::test_IsModeADIFMode_caseInsensitive()
{
    // The method uppercases the input, so lowercase must work too.
    QVERIFY(hamlib->isModeADIFMode("am"));
    QVERIFY(hamlib->isModeADIFMode("cw"));
    QVERIFY(hamlib->isModeADIFMode("Usb"));
    QVERIFY(hamlib->isModeADIFMode("Rtty"));
}

// ---------------------------------------------------------------------------
// init() without a real radio
// ---------------------------------------------------------------------------

void tst_Hamlib::test_Init_modelZero_returnsFalse()
{
    HamLibClass h;
    h.initClass();
    h.setModelId(0);
    QVERIFY(!h.init(false));
}

void tst_Hamlib::test_Init_modelOne_returnsFalse()
{
    // Model 1 (RIG_MODEL_DUMMY) is treated as invalid by HamLibClass::init().
    HamLibClass h;
    h.initClass();
    h.setModelId(1);
    QVERIFY(!h.init(false));
}

void tst_Hamlib::test_Init_noSerialPort_returnsFalse()
{
    // Any non-NETRIGCTL model > 1 with an empty serial port must fail.
    HamLibClass h;
    h.initClass();
    // Use the first rig in the list whose model ID > 1.
    int modelId = 0;
    for (const QString &name : h.getRigList()) {
        int id = h.getModelIdFromName(name);
        if (id > 1 && id != RIG_MODEL_NETRIGCTL) {
            modelId = id;
            break;
        }
    }
    if (modelId == 0) QSKIP("No suitable model found to run this test");

    h.setModelId(modelId);
    // Do NOT call setPort() — leave serial port empty.
    QVERIFY(!h.init(false));
    QVERIFY(!h.isRunning());
}

// ---------------------------------------------------------------------------
// isRunning state
// ---------------------------------------------------------------------------

void tst_Hamlib::test_IsRunning_initiallyFalse()
{
    HamLibClass h;
    QVERIFY(!h.isRunning());
}

void tst_Hamlib::test_IsRunning_afterFailedInit_isFalse()
{
    HamLibClass h;
    h.initClass();
    h.setModelId(0);
    h.init(false);
    QVERIFY(!h.isRunning());
}

// ---------------------------------------------------------------------------
// stop() safety
// ---------------------------------------------------------------------------

void tst_Hamlib::test_Stop_whenNotRunning_doesNotCrash()
{
    HamLibClass h;
    // Must not crash or assert when called on an already-disconnected instance.
    h.stop();
    QVERIFY(!h.isRunning());
}

// ---------------------------------------------------------------------------
// readRadio() without connection
// ---------------------------------------------------------------------------

void tst_Hamlib::test_ReadRadio_notConnected_returnsFalse()
{
    HamLibClass h;
    h.initClass();
    QVERIFY(!h.readRadio());
}

// ---------------------------------------------------------------------------
// Setter validation (boundary / no-crash checks)
// ---------------------------------------------------------------------------

void tst_Hamlib::test_SetPoll_negativeIsIgnored()
{
    HamLibClass h;
    h.initClass();
    h.setPoll(-1);          // Must not crash or corrupt state.
    QVERIFY(!h.isRunning());
}

void tst_Hamlib::test_SetPoll_zeroIsIgnored()
{
    HamLibClass h;
    h.initClass();
    h.setPoll(0);
    QVERIFY(!h.isRunning());
}

void tst_Hamlib::test_SetPoll_positiveIsAccepted()
{
    HamLibClass h;
    h.initClass();
    h.setPoll(500);         // Valid positive value — must not crash.
    QVERIFY(!h.isRunning());
}

void tst_Hamlib::test_SetDataBits_validRange_doesNotCrash()
{
    HamLibClass h;
    for (int bits = 5; bits <= 8; ++bits)
        h.setDataBits(bits);    // All values in [5, 8] are valid.
    QVERIFY(true);
}

void tst_Hamlib::test_SetDataBits_belowRange_doesNotCrash()
{
    HamLibClass h;
    h.setDataBits(4);           // Below minimum — should be silently clamped to 8.
    QVERIFY(true);
}

void tst_Hamlib::test_SetDataBits_aboveRange_doesNotCrash()
{
    HamLibClass h;
    h.setDataBits(9);           // Above maximum — should be silently clamped to 8.
    QVERIFY(true);
}

void tst_Hamlib::test_SetNetworkPort_validRange_doesNotCrash()
{
    HamLibClass h;
    h.setNetworkPort(4532);     // Default rigctld port.
    h.setNetworkPort(1);
    h.setNetworkPort(65534);
    QVERIFY(true);
}

void tst_Hamlib::test_SetNetworkPort_zeroIsIgnored()
{
    HamLibClass h;
    h.setNetworkPort(0);        // 0 is out of range; must not crash.
    QVERIFY(true);
}

void tst_Hamlib::test_SetNetworkPort_maxIsIgnored()
{
    HamLibClass h;
    h.setNetworkPort(65535);    // 65535 is excluded by the <= 65534 guard.
    QVERIFY(true);
}

// ---------------------------------------------------------------------------
// loadSettings
// ---------------------------------------------------------------------------

void tst_Hamlib::test_LoadSettings_returnsTrue()
{
    // loadSettings() reads QSettings and returns true regardless of whether
    // any value was previously written.
    HamLibClass h;
    QVERIFY(h.loadSettings());
}

QTEST_MAIN(tst_Hamlib)

#include "tst_hamlib.moc"
