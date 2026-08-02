/***************************************************************************
                          tst_udpclient.cpp  -  description
                             -------------------
    begin                : Aug 2026
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

#include <QtTest>
#include <QCoreApplication>
#include <QSignalSpy>
#include <QUdpSocket>

#include "../../src/udpclient.h"
#include "../../src/qso.h"

class tst_UDPClient : public QObject
{
    Q_OBJECT

public:
    tst_UDPClient();
    ~tst_UDPClient();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Defaults();
    void test_SetPort();
    void test_NotSentWhenDisabled();
    void test_NotSentWithoutServer();
    void test_NotSentWhenQSOIsIncomplete();
    void test_SendQSO();
    void test_SendQSOToHostname();

private:
    QSO completeQSO();
    QByteArray waitForDatagram();

    UDPClient *client;
    QUdpSocket *server;    // Plays the role of KLogServer
    quint16 serverPort;
};

tst_UDPClient::tst_UDPClient() {}
tst_UDPClient::~tst_UDPClient() {}

void tst_UDPClient::initTestCase()
{
    server = new QUdpSocket(this);
    QVERIFY2(server->bind(QHostAddress::LocalHost, 0), "Not able to bind the receiving socket");
    serverPort = server->localPort();

    client = new UDPClient(this);
    client->setServer("127.0.0.1");
    client->setPort(serverPort);
    client->setEnabled(true);
}

void tst_UDPClient::cleanupTestCase()
{
    delete(client);
    client = nullptr;
}

QSO tst_UDPClient::completeQSO()
{
    QSO qso;
    qso.setCall("EA4K");
    qso.setDateTimeOn(QDateTime(QDate(2026, 8, 2), QTime(11, 22, 33), QTimeZone::UTC));
    qso.setBand("20M");
    qso.setMode("SSB");
    qso.setRSTTX("59");
    qso.setRSTRX("59");
    qso.setComment("Sent to KLogServer");
    return qso;
}

QByteArray tst_UDPClient::waitForDatagram()
{   // qWait keeps the event loop running, needed to resolve hostnames
    QDeadlineTimer deadline(3000);
    while ((!server->hasPendingDatagrams()) && (!deadline.hasExpired()))
        QTest::qWait(20);
    if (!server->hasPendingDatagrams())
        return QByteArray();

    QByteArray datagram;
    datagram.resize(server->pendingDatagramSize());
    server->readDatagram(datagram.data(), datagram.size());
    return datagram;
}

void tst_UDPClient::test_Defaults()
{
    UDPClient defaultClient;
    QCOMPARE(defaultClient.isEnabled(), false);
    QCOMPARE(defaultClient.getPort(), 2237);
    QVERIFY(defaultClient.getServer().isEmpty());
}

void tst_UDPClient::test_SetPort()
{
    UDPClient defaultClient;
    defaultClient.setPort(12060);
    QCOMPARE(defaultClient.getPort(), 12060);
    defaultClient.setPort(0);           // Not valid, back to the default port
    QCOMPARE(defaultClient.getPort(), 2237);
    defaultClient.setPort(70000);       // Not valid, back to the default port
    QCOMPARE(defaultClient.getPort(), 2237);
}

void tst_UDPClient::test_NotSentWhenDisabled()
{
    client->setEnabled(false);
    QCOMPARE(client->sendQSO(completeQSO()), false);
    QCOMPARE(server->hasPendingDatagrams(), false);
    client->setEnabled(true);
}

void tst_UDPClient::test_NotSentWithoutServer()
{
    UDPClient noServerClient;
    noServerClient.setEnabled(true);
    QSignalSpy spy(&noServerClient, SIGNAL(errorSending(QString)));
    QCOMPARE(noServerClient.sendQSO(completeQSO()), false);
    QCOMPARE(spy.count(), 1);
}

void tst_UDPClient::test_NotSentWhenQSOIsIncomplete()
{
    QSO qso;                    // Just a callsign is not a QSO
    qso.setCall("EA4K");
    QCOMPARE(client->sendQSO(qso), false);
    QCOMPARE(server->hasPendingDatagrams(), false);
}

void tst_UDPClient::test_SendQSO()
{
    QVERIFY(client->sendQSO(completeQSO()));

    const QString received = QString::fromUtf8(waitForDatagram());
    QVERIFY2(!received.isEmpty(), "No datagram was received");
    QVERIFY(received.contains("<CALL:4>EA4K"));
    QVERIFY(received.contains("<QSO_DATE:8>20260802"));
    QVERIFY(received.contains("<TIME_ON:6>112233"));
    QVERIFY(received.contains("<BAND:3>20M"));
    QVERIFY(received.contains("<MODE:3>SSB"));
    // Every ADIF field with data is sent, not just the basic ones
    QVERIFY(received.contains("<COMMENT:18>Sent to KLogServer"));
    QVERIFY(received.contains("<EOR>"));
}

void tst_UDPClient::test_SendQSOToHostname()
{   // A hostname needs to be resolved before the datagram can be sent
    client->setServer("localhost");
    QVERIFY(client->sendQSO(completeQSO()));

    const QString received = QString::fromUtf8(waitForDatagram());
    QVERIFY2(!received.isEmpty(), "No datagram was received");
    QVERIFY(received.contains("<CALL:4>EA4K"));
    client->setServer("127.0.0.1");
}

QTEST_MAIN(tst_UDPClient)

#include "tst_udpclient.moc"
