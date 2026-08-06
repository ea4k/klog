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
    void test_StationFromDecodedTextCompoundCalls();
    void test_StationFromDecodedTextDXpedition();
    void test_StationFromDecodedTextNotAStation();
    void test_ModeFromDecodeChar();
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
    QCOMPARE(station.call, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);
    QCOMPARE(station.isEmpty(), false);

    station = UDPServer::stationFromDecodedText("CQ W1AW");
    QCOMPARE(station.call, QString("W1AW"));
    QCOMPARE(station.callingCQ, true);

    // A decode may arrive with the hint markers WSJT-X adds to the line
    station = UDPServer::stationFromDecodedText("CQ EA4K IN80          a1");
    QCOMPARE(station.call, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);

    station = UDPServer::stationFromDecodedText("QRZ EA4K IN80");
    QCOMPARE(station.call, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);
}

void tst_UDPServer::test_StationFromDecodedTextCQWithDirective()
{
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("CQ DX EA4K IN80");
    QCOMPARE(station.call, QString("EA4K"));
    QCOMPARE(station.callingCQ, true);

    station = UDPServer::stationFromDecodedText("CQ EU EA4K IN80");
    QCOMPARE(station.call, QString("EA4K"));

    station = UDPServer::stationFromDecodedText("CQ TEST W1AW FN31");
    QCOMPARE(station.call, QString("W1AW"));

    station = UDPServer::stationFromDecodedText("CQ POTA EA4K");
    QCOMPARE(station.call, QString("EA4K"));
}

void tst_UDPServer::test_StationFromDecodedTextExchange()
{
    // The station transmitting is the second one: the first is being called
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("EA4K W1AW -15");
    QCOMPARE(station.call, QString("W1AW"));
    QCOMPARE(station.callingCQ, false);

    station = UDPServer::stationFromDecodedText("EA4K W1AW RR73");
    QCOMPARE(station.call, QString("W1AW"));

    station = UDPServer::stationFromDecodedText("EA4K W1AW R+03");
    QCOMPARE(station.call, QString("W1AW"));

    // A hashed callsign KLog cannot resolve is not a station to spot
    station = UDPServer::stationFromDecodedText("<...> W1AW -15");
    QCOMPARE(station.call, QString("W1AW"));
    station = UDPServer::stationFromDecodedText("EA4K <...> -15");
    QCOMPARE(station.isEmpty(), true);
}

void tst_UDPServer::test_StationFromDecodedTextCompoundCalls()
{
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("CQ EA4K/P");
    QCOMPARE(station.call, QString("EA4K/P"));
    QCOMPARE(station.callingCQ, true);

    // WSJT-X sends a known non standard callsign between brackets
    station = UDPServer::stationFromDecodedText("W1AW <PJ4/EA4K> R+03");
    QCOMPARE(station.call, QString("PJ4/EA4K"));
    QCOMPARE(station.callingCQ, false);
}

void tst_UDPServer::test_StationFromDecodedTextDXpedition()
{
    // DXpedition (fox & hound) mode: the fox is the one between brackets
    WSJTXDecodedStation station = UDPServer::stationFromDecodedText("W1AW RR73; EA4K <KH1/KH7Z> -08");
    QCOMPARE(station.call, QString("KH1/KH7Z"));
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
    // 14.074 MHz dial + 1500 Hz of audio offset
    QCOMPARE(arguments.at(1).toDouble(), 14.0755);
    QCOMPARE(arguments.at(2).toString(), QString("FT8"));
    QCOMPARE(arguments.at(3).toInt(), -12);
    QCOMPARE(arguments.at(4).toBool(), true);
    QCOMPARE(arguments.at(5).toDateTime().time(), QTime(12, 34, 15));

    // Our own callsign, as WSJT-X knows it, is never spotted
    server.parse(decodeDatagram("CQ EA4K IN80", "~", -12, 1500));
    QCOMPARE(spy.count(), 0);
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
