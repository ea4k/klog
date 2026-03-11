#include <QtTest>
#include "../../src/database/datacache.h"
#include "../../src/frequency.h"

class tst_DataCache : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Band cache
    void test_addBand_and_getBandFromId();
    void test_getBandFromName_caseInsensitive();
    void test_getBandFromName_notFound();
    void test_bandListIsBuilt();
    void test_band_contains();

    // Mode cache
    void test_addMode_storesCabrilloAndDeprecated();
    void test_getModeFromSubmode();
    void test_getNameFromSubMode();
    void test_modeListIsBuilt();
    void test_deprecated_mode();

private:
    DataCache *cache;

    void loadSampleBands();
    void loadSampleModes();
};

void tst_DataCache::initTestCase()
{
    cache = new DataCache();
}

void tst_DataCache::cleanupTestCase()
{
    delete cache;
}

// ─── helpers ────────────────────────────────────────────────────────────────

void tst_DataCache::loadSampleBands()
{
    // Values mirror populateTableBand() exactly
    Frequency f;
    auto add = [&](int id, const QString &name, double lo, double hi) {
        Frequency flo, fhi;
        flo.fromDouble(lo, MHz);
        fhi.fromDouble(hi, MHz);
        cache->addBand(id, name, flo, fhi);
    };

    add(1,  "160M",  1.8,    2.0);
    add(2,  "80M",   3.5,    4.0);
    add(3,  "60M",   5.102,  5.404);
    add(4,  "40M",   7.0,    7.3);
    add(5,  "30M",  10.0,   10.15);
    add(6,  "20M",  14.0,   14.35);
    add(7,  "17M",  18.068, 18.168);
    add(8,  "15M",  21.0,   21.45);
    add(9,  "12M",  24.89,  24.99);
    add(10, "10M",  28.0,   29.7);
    add(11, "6M",   50.0,   54.0);
    add(12, "5M",   54.000001, 69.9);
    add(13, "4M",   70.0,   71.0);
    add(14, "2M",  144.0,  148.0);
    add(15, "70CM",420.0,  450.0);
    add(16, "23CM",1240.0, 1300.0);
}

void tst_DataCache::loadSampleModes()
{
    cache->addMode(1, "USB",  "SSB",  "SSB",  false);
    cache->addMode(2, "LSB",  "SSB",  "SSB",  false);
    cache->addMode(3, "FT8",  "FT8",  "FT8",  false);
    cache->addMode(4, "JT9C", "JT9",  "JT9",  false);
    cache->addMode(5, "Q65",  "MFSK", "MFSK", false);
    cache->addMode(6, "PCW",  "CW",   "CW",   true);   // deprecated
}

// ─── Band tests ─────────────────────────────────────────────────────────────

void tst_DataCache::test_addBand_and_getBandFromId()
{
    loadSampleBands();

    BandEntry b = cache->getBandFromId(6); // 20M
    QVERIFY2(b.isValid(),            "20M not found by id");
    QCOMPARE(b.name,                 QString("20M"));

    BandEntry missing = cache->getBandFromId(9999);
    QVERIFY2(!missing.isValid(),     "Non-existent id should return invalid entry");
}

void tst_DataCache::test_getBandFromName_caseInsensitive()
{
    // Bug fix: getBandFromName must normalise to uppercase
    BandEntry b1 = cache->getBandFromName("20m");
    BandEntry b2 = cache->getBandFromName("20M");
    BandEntry b3 = cache->getBandFromName("20m");

    QVERIFY2(b1.isValid(), "Lowercase '20m' not found");
    QVERIFY2(b2.isValid(), "Uppercase '20M' not found");
    QCOMPARE(b1.id, b2.id);
    QCOMPARE(b1.id, b3.id);
}

void tst_DataCache::test_getBandFromName_notFound()
{
    BandEntry b = cache->getBandFromName("NOBAND");
    QVERIFY2(!b.isValid(), "Unknown band name must return invalid entry");
}

void tst_DataCache::test_bandListIsBuilt()
{
    // After loading, flag must be true
    QVERIFY2(cache->bandListIsBuilt, "bandListIsBuilt should be true after addBand calls");

    // Fresh cache: flag must start false
    DataCache fresh;
    QVERIFY2(!fresh.bandListIsBuilt, "bandListIsBuilt should be false in a new DataCache");
}

void tst_DataCache::test_band_contains()
{
    BandEntry b20 = cache->getBandFromName("20M"); // 14.0 – 14.35 MHz
    QVERIFY2( b20.contains(Frequency(14.195)), "14.195 must be inside 20M");
    QVERIFY2( b20.contains(Frequency(14.0)),   "14.0 lower edge must be inside 20M");
    QVERIFY2( b20.contains(Frequency(14.35)),  "14.35 upper edge must be inside 20M");
    QVERIFY2(!b20.contains(Frequency(7.1)),    "7.1 must NOT be inside 20M");
    QVERIFY2(!b20.contains(Frequency(0.0)),    "0.0 must NOT be inside 20M");
}

// ─── Mode tests ─────────────────────────────────────────────────────────────

void tst_DataCache::test_addMode_storesCabrilloAndDeprecated()
{
    loadSampleModes();

    // Bug fix: addMode must store cabrillo and deprecated
    ModeEntry ft8 = cache->getModeFromSubmode("FT8");
    QVERIFY2(ft8.id > 0,                        "FT8 not found");
    QCOMPARE(ft8.cabrillo,  QString("FT8"));
    QCOMPARE(ft8.deprecated, false);

    ModeEntry pcw = cache->getModeFromSubmode("PCW");
    QVERIFY2(pcw.id > 0,                        "PCW not found");
    QCOMPARE(pcw.deprecated, true);
}

void tst_DataCache::test_getModeFromSubmode()
{
    ModeEntry usb = cache->getModeFromSubmode("USB");
    QVERIFY2(usb.isValid(),         "USB not found");
    QCOMPARE(usb.mode, QString("SSB"));

    ModeEntry jt9c = cache->getModeFromSubmode("JT9C");
    QVERIFY2(jt9c.isValid(),        "JT9C not found");
    QCOMPARE(jt9c.mode, QString("JT9"));

    ModeEntry none = cache->getModeFromSubmode("NOMODE");
    QVERIFY2(!none.isValid(),       "Unknown submode must return invalid entry");
}

void tst_DataCache::test_getNameFromSubMode()
{
    QCOMPARE(cache->getModeNameFromSubmode("USB"),  QString("SSB"));
    QCOMPARE(cache->getModeNameFromSubmode("FT8"),  QString("FT8"));
    QCOMPARE(cache->getModeNameFromSubmode("JT9C"), QString("JT9"));
    QCOMPARE(cache->getModeNameFromSubmode("Q65"),  QString("MFSK"));
    QCOMPARE(cache->getModeNameFromSubmode("????"), QString()); // not found → empty
}

void tst_DataCache::test_modeListIsBuilt()
{
    // Bug fix: modeListIsBuilt must be initialised to false
    DataCache fresh;
    QVERIFY2(!fresh.modeListIsBuilt, "modeListIsBuilt should be false in a new DataCache");

    fresh.addMode(1, "CW", "CW", "CW", false);
    QVERIFY2(fresh.modeListIsBuilt,  "modeListIsBuilt should be true after addMode");
}

void tst_DataCache::test_deprecated_mode()
{
    ModeEntry pcw = cache->getModeFromSubmode("PCW");
    QVERIFY2(pcw.deprecated, "PCW should be marked deprecated");

    ModeEntry usb = cache->getModeFromSubmode("USB");
    QVERIFY2(!usb.deprecated, "USB should NOT be marked deprecated");
}

QTEST_APPLESS_MAIN(tst_DataCache)
#include "tst_datacache.moc"
