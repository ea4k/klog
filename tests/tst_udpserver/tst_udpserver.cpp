/***************************************************************************
                          tst_udpserver.cpp  -  description
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
#include <QUdpSocket>
#include "../../src/udpserver.h"

class tst_UDPServer : public QObject
{
    Q_OBJECT

public:
    tst_UDPServer();
    ~tst_UDPServer();

private slots:
    void test_StationFromDecodedTextCQ();
    void test_StationFromDecodedTextCQWithDirective();
    void test_StationFromDecodedTextExchange();
    void test_StationFromDecodedTextCQHasNoRemoteStation();
    void test_StationFromDecodedTextCompoundCalls();
    void test_StationFromDecodedTextDXpedition();
    void test_StationFromDecodedTextNotAStation();
    void test_ModeFromDecodeChar();
    void test_DecodeDateTimeUsesTheUTCDate();
    void test_ParseDecodeDatagram();
    void test_ParseDecodeDatagramWithoutStatus();
    void test_ParseLowConfidenceDecodeDatagram();
    void test_ReplayIsRequestedToWSJTX();

private:
    // Builds a datagram exactly as WSJT-X serializes it
    QByteArray decodeDatagram(const QString &_message, const QString &_mode,
                              const qint32 _snr, const quint32 _deltaFrequency,
                              const bool _lowConfidence = false);
    QByteArray statusDatagram(const quint64 _frequency, const QString &_mode,
                              const QString &_deCall);
    static QDataStream *streamFor(QByteArray &_datagram, const quint32 _type);
};

tst_UDPServer::tst_UDPServer(){}
tst_UDPServer::~tst_UDPServer(){}

QDataStream *tst_UDPServer::streamFor(QByteArray &_datagram, const quint32 _type)
{
    QDataStream *out = new QDataStream(&_datagram, QIODevice::WriteOnly);
    out->setVersion(16);
    out->setByteOrder(QDataStream::BigEndian);
    *out << quint32(0xadbccbda) << quint32(2) << _type << QByteArray("WSJT-X");
    return out;
}

QByteArray tst_UDPServer::decodeDatagram(const QString &_message, const QString &_mode,
                                         const qint32 _snr, const quint32 _deltaFrequency,
                                         const bool _lowConfidence)
{
    QByteArray datagram;
    QDataStream *out = streamFor(datagram, 2);   // Decode
    *out << true << QTime(12, 34, 15) << _snr << double(0.2) << _deltaFrequency
         << _mode.toUtf8() << _message.toUtf8() << _lowConfidence << false;
    delete out;
    return datagram;
}

QByteArray tst_UDPServer::statusDatagram(const quint64 _frequency, const QString &_mode,
                                         const QString &_deCall)
{
    QByteArray datagram;
    QDataStream *out = streamFor(datagram, 1);   // Status
    *out << _frequency << _mode.toUtf8() << QByteArray("W1AW") << QByteArray("-10")
         << _mode.toUtf8() << true << false << true << qint32(1500) << qint32(1500)
         << _deCall.toUtf8() << QByteArray("IN80") << QByteArray("FN31") << false
         << QByteArray("") << false << quint8(0) << quint32(50) << quint32(15)
         << QByteArray("Default");
    delete out;
    return datagram;
}

void tst_UDPServer::test_StationFromDecodedTextCQ()
{
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("CQ EA4K IN80");
    QCOMPARE(station.caller, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);
    QCOMPARE(station.isEmpty(), false);

    station = UDPServer::stationFromDecodedText("CQ W1AW");
    QCOMPARE(station.caller, QString("W1AW"));
    QCOMPARE(station.callingCQ, true);

    // A decode may arrive with the hint markers WSJT-X adds to the line
    station = UDPServer::stationFromDecodedText("CQ EA4K IN80          a1");
    QCOMPARE(station.caller, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);

    station = UDPServer::stationFromDecodedText("QRZ EA4K IN80");
    QCOMPARE(station.caller, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);
}

void tst_UDPServer::test_StationFromDecodedTextCQWithDirective()
{
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("CQ DX EA4K IN80");
    QCOMPARE(station.caller, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);

    station = UDPServer::stationFromDecodedText("CQ EU EA4K IN80");
    QCOMPARE(station.caller, QString("EA4K"));

    station = UDPServer::stationFromDecodedText("CQ TEST W1AW FN31");
    QCOMPARE(station.caller, QString("W1AW"));

    station = UDPServer::stationFromDecodedText("CQ POTA EA4K");
    QCOMPARE(station.caller, QString("EA4K"));
}

void tst_UDPServer::test_StationFromDecodedTextExchange()
{
    // The station transmitting is the second one: the first is the remote
    // station it is calling, which we are not hearing ourselves
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("EA4K W1AW -15");
    QCOMPARE(station.caller, QString("W1AW"));
    QCOMPARE(station.remoteStation, QString("EA4K"));
    QCOMPARE(station.callingCQ, false);

    station = UDPServer::stationFromDecodedText("EA4K W1AW RR73");
    QCOMPARE(station.caller, QString("W1AW"));
    QCOMPARE(station.remoteStation, QString("EA4K"));

    station = UDPServer::stationFromDecodedText("EA4K W1AW R+03");
    QCOMPARE(station.caller, QString("W1AW"));
    QCOMPARE(station.remoteStation, QString("EA4K"));

    // A hashed callsign KLog cannot resolve is not a station to spot
    station = UDPServer::stationFromDecodedText("<...> W1AW -15");
    QCOMPARE(station.caller, QString("W1AW"));
    QCOMPARE(station.remoteStation.isEmpty(), true);
    station = UDPServer::stationFromDecodedText("EA4K <...> -15");
    QCOMPARE(station.isEmpty(), true);
    QCOMPARE(station.remoteStation.isEmpty(), true);
}

void tst_UDPServer::test_StationFromDecodedTextCQHasNoRemoteStation()
{
    // Nobody is being called, so there is only the station calling CQ
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("CQ EA4K IN80");
    QCOMPARE(station.caller, QString("EA4K"));
    QCOMPARE(station.remoteStation.isEmpty(), true);

    station = UDPServer::stationFromDecodedText("CQ DX EA4K IN80");
    QCOMPARE(station.remoteStation.isEmpty(), true);
}

void tst_UDPServer::test_StationFromDecodedTextCompoundCalls()
{
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("CQ EA4K/P");
    QCOMPARE(station.caller, QString("EA4K/P"));
    QCOMPARE(station.callingCQ, true);

    // WSJT-X sends a known non standard callsign between brackets
    station = UDPServer::stationFromDecodedText("W1AW <PJ4/EA4K> R+03");
    QCOMPARE(station.caller, QString("PJ4/EA4K"));
    QCOMPARE(station.callingCQ, false);
}

void tst_UDPServer::test_StationFromDecodedTextDXpedition()
{
    // DXpedition (fox & hound) mode: the fox is the one between brackets
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("W1AW RR73; EA4K <KH1/KH7Z> -08");
    QCOMPARE(station.caller, QString("KH1/KH7Z"));
    QCOMPARE(station.remoteStation, QString("W1AW"));
    QCOMPARE(station.callingCQ, false);
}

void tst_UDPServer::test_StationFromDecodedTextNotAStation()
{
    QCOMPARE(UDPServer::stationFromDecodedText("").isEmpty(), true);
    QCOMPARE(UDPServer::stationFromDecodedText("CQ").isEmpty(), true);
    QCOMPARE(UDPServer::stationFromDecodedText("CQ TEST").isEmpty(), true);
    QCOMPARE(UDPServer::stationFromDecodedText("TNX 73 GL").isEmpty(), true);
    QCOMPARE(UDPServer::stationFromDecodedText("0123456789ABCDEF").isEmpty(), true);
}

void tst_UDPServer::test_ModeFromDecodeChar()
{
    QCOMPARE(UDPServer::modeFromDecodeChar("~"), QString("FT8"));
    QCOMPARE(UDPServer::modeFromDecodeChar("+"), QString("FT4"));
    QCOMPARE(UDPServer::modeFromDecodeChar("#"), QString("JT65"));
    QCOMPARE(UDPServer::modeFromDecodeChar("@"), QString("JT9"));
    QCOMPARE(UDPServer::modeFromDecodeChar("&"), QString("MSK144"));
    // Anything KLog does not know is left to the caller
    QCOMPARE(UDPServer::modeFromDecodeChar("%"), QString());
    // Some programs send the name of the mode instead of the character
    QCOMPARE(UDPServer::modeFromDecodeChar("FT8"), QString("FT8"));
}

void tst_UDPServer::test_DecodeDateTimeUsesTheUTCDate()
{
    // Half past midnight UTC, which for an operator west of Greenwich is
    // still the day before: the date of a decode is the UTC one, or every
    // one of them would be filed a day out and look hours old.
    const QDateTime nowUtc(QDate(2026, 3, 15), QTime(0, 30, 0), QTimeZone::UTC);

    QCOMPARE(UDPServer::decodeDateTime(QTime(0, 29, 15), nowUtc),
             QDateTime(QDate(2026, 3, 15), QTime(0, 29, 15), QTimeZone::UTC));

    // A decode from just before midnight belongs to the day before, and not
    // to a few minutes into the future
    QCOMPARE(UDPServer::decodeDateTime(QTime(23, 58, 45), nowUtc),
             QDateTime(QDate(2026, 3, 14), QTime(23, 58, 45), QTimeZone::UTC));

    // Whatever the time of day, a decode is never in the future and never
    // older than a day
    const QDateTime midday(QDate(2026, 3, 15), QTime(12, 0, 0), QTimeZone::UTC);
    QCOMPARE(UDPServer::decodeDateTime(QTime(11, 59, 0), midday),
             QDateTime(QDate(2026, 3, 15), QTime(11, 59), QTimeZone::UTC));
    QCOMPARE(UDPServer::decodeDateTime(QTime(12, 1, 0), midday),
             QDateTime(QDate(2026, 3, 14), QTime(12, 1), QTimeZone::UTC));
}

void tst_UDPServer::test_ParseDecodeDatagram()
{
    UDPServer server;
    QSignalSpy spy(&server, &UDPServer::stationDecoded);

    // The dial frequency is only known through the Status message
    server.parse(statusDatagram(14074000, "FT8", "EA4K"));
    server.parse(decodeDatagram("CQ W1AW FN31", "~", -12, 1500));

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("W1AW"));
    QCOMPARE(arguments.at(1).toString(), QString());   // Calling CQ: no remote station
    // 14.074 MHz dial + 1500 Hz of audio offset
    QCOMPARE(arguments.at(2).toDouble(), 14.0755);
    QCOMPARE(arguments.at(3).toString(), QString("FT8"));
    QCOMPARE(arguments.at(4).toInt(), -12);
    QCOMPARE(arguments.at(5).toBool(), true);
    // WSJT-X gives the time of the decode in UTC and nothing else: the date
    // has to put it in the last 24 hours, wherever the operator is
    const QDateTime decodedAt = arguments.at(6).toDateTime();
    const QDateTime nowUtc = QDateTime::currentDateTimeUtc();
    QCOMPARE(decodedAt.time(), QTime(12, 34, 15));
    QVERIFY2(decodedAt <= nowUtc, "A decode cannot have been made in the future");
    QVERIFY2(decodedAt > nowUtc.addDays(-1), "A decode made today cannot be a day old");

    // An exchange brings both stations: the one we decoded and the one it is
    // calling, whose callsign we only read out of the message
    server.parse(decodeDatagram("K1ABC W1AW -15", "~", -12, 1500));
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("W1AW"));
    QCOMPARE(arguments.at(1).toString(), QString("K1ABC"));
    QCOMPARE(arguments.at(5).toBool(), false);

    // Our own callsign, as WSJT-X knows it, is never spotted
    server.parse(decodeDatagram("CQ EA4K IN80", "~", -12, 1500));
    QCOMPARE(spy.count(), 0);

    // ... not even when it is the one being called
    server.parse(decodeDatagram("EA4K W1AW -15", "~", -12, 1500));
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("W1AW"));
    QCOMPARE(arguments.at(1).toString(), QString());
}

void tst_UDPServer::test_ParseDecodeDatagramWithoutStatus()
{
    UDPServer server;
    QSignalSpy spy(&server, &UDPServer::stationDecoded);

    // Without a Status message there is no dial frequency, so the frequency
    // of the decode cannot be known and the station cannot be spotted
    server.parse(decodeDatagram("CQ W1AW FN31", "~", -12, 1500));
    QCOMPARE(spy.count(), 0);
}

void tst_UDPServer::test_ParseLowConfidenceDecodeDatagram()
{
    UDPServer server;
    QSignalSpy spy(&server, &UDPServer::stationDecoded);

    server.parse(statusDatagram(14074000, "FT8", "EA4K"));
    server.parse(decodeDatagram("CQ W1AW FN31", "~", -12, 1500, true));
    QCOMPARE(spy.count(), 0);
}

void tst_UDPServer::test_ReplayIsRequestedToWSJTX()
{
    // Once the band is known, KLog asks WSJT-X to send again the decodes it
    // is showing so the DX Assistant does not start empty.
    const quint16 testPort = 22370;
    UDPServer server;
    server.setPort(testPort);
    QVERIFY(server.start());

    QUdpSocket client;
    QVERIFY(client.bind(QHostAddress::LocalHost, 0));
    client.writeDatagram(statusDatagram(14074000, "FT8", "EA4K"),
                         QHostAddress::LocalHost, testPort);

    QVERIFY(QTest::qWaitFor([&client]() { return client.hasPendingDatagrams(); }, 3000));

    QByteArray datagram;
    datagram.resize(client.pendingDatagramSize());
    client.readDatagram(datagram.data(), datagram.size());

    QDataStream in(datagram);
    in.setVersion(16);
    in.setByteOrder(QDataStream::BigEndian);
    quint32 magic = 0, schema = 0, type = 0;
    QByteArray id;
    in >> magic >> schema >> type >> id;
    QCOMPARE(magic, quint32(0xadbccbda));
    QCOMPARE(type, quint32(7));     // Replay
    QCOMPARE(id, QByteArray("WSJT-X"));

    QVERIFY(server.stop());
}

QTEST_MAIN(tst_UDPServer)
#include "tst_udpserver.moc"
