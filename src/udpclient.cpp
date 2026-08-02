/***************************************************************************
                          udpclient.cpp  -  description
                             -------------------
    begin                : aug 2026
    copyright            : (C) 2026 by Jaime Robles
    user                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "udpclient.h"
#include <QSettings>

UDPClient::UDPClient(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    util = new Utilities(Q_FUNC_INFO);
    server = QString();
    port = defaultPort;
    enabled = false;
    lookupId = -1;
}

UDPClient::~UDPClient()
{
    if (lookupId >= 0)
        QHostInfo::abortHostLookup(lookupId);
    delete(util);
}

void UDPClient::loadSettings()
{
    QSettings settings(util->getCfgFile(), QSettings::IniFormat);
    settings.beginGroup("KLogServer");
    setServer(settings.value("KLogServerAddress").toString());
    setPort(settings.value("KLogServerPort", defaultPort).toInt());
    setEnabled(settings.value("SendToKLogServer", false).toBool());
    settings.endGroup();
}

void UDPClient::setEnabled(const bool _enabled)
{
    enabled = _enabled;
    if (!enabled)
        pendingDatagrams.clear();
}

bool UDPClient::isEnabled() const
{
    return enabled;
}

void UDPClient::setServer(const QString &_server)
{
    const QString newServer = _server.trimmed();
    if (newServer == server)
        return;
    server = newServer;
    clearServerAddress();
    // An IP address needs no name resolution, so it can be used right away.
    const QHostAddress address(server);
    if (!address.isNull())
        serverAddress = address;
}

QString UDPClient::getServer() const
{
    return server;
}

void UDPClient::setPort(const int _port)
{
    if ((_port > 0) && (_port <= 65535))
        port = _port;
    else
        port = defaultPort;
}

int UDPClient::getPort() const
{
    return port;
}

bool UDPClient::sendQSO(const QSO &_qso)
{
    if (!enabled)
        return false;
    // getADIF is not const and returns an empty string for an incomplete QSO.
    QSO qso(_qso);
    return sendADIF(qso.getADIF());
}

bool UDPClient::sendADIF(const QString &_adif)
{
    if (!enabled)
        return false;
    if (_adif.trimmed().isEmpty())
        return false;
    if (server.isEmpty())
    {
        emit errorSending(tr("KLogServer: No server has been defined, the QSO has not been sent."));
        return false;
    }
    return queueOrSend(buildDatagram(_adif));
}

QByteArray UDPClient::buildDatagram(const QString &_adif) const
{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(16);     // Qt_5_4, the stream version used by the schema 3
    out.setByteOrder(QDataStream::BigEndian);
    out << magicNumber << schemaNumber << static_cast<quint32>(ADIFLogged)
        << QByteArray("KLog") << _adif.toUtf8();
    return datagram;
}

bool UDPClient::queueOrSend(const QByteArray &_datagram)
{
    if (!serverAddress.isNull())
        return writeDatagram(_datagram);

    // The hostname is not resolved yet: keep the QSO until it is. Dropping the
    // oldest one keeps the queue bounded if the server can't be resolved.
    if (pendingDatagrams.size() >= maxPendingDatagrams)
        pendingDatagrams.dequeue();
    pendingDatagrams.enqueue(_datagram);
    resolveServer();
    return true;
}

bool UDPClient::writeDatagram(const QByteArray &_datagram)
{
    const qint64 sent = socket->writeDatagram(_datagram, serverAddress, static_cast<quint16>(port));
    if (sent == _datagram.size())
        return true;
    emit errorSending(tr("KLogServer: The QSO could not be sent to %1: %2").arg(server, socket->errorString()));
    return false;
}

void UDPClient::resolveServer()
{
    if (lookupId >= 0)      // A lookup is already running
        return;
    if (server.isEmpty())
        return;
    lookupId = QHostInfo::lookupHost(server, this, SLOT(slotHostInfoReceived(QHostInfo)));
}

void UDPClient::clearServerAddress()
{
    if (lookupId >= 0)
    {
        QHostInfo::abortHostLookup(lookupId);
        lookupId = -1;
    }
    serverAddress.clear();
    pendingDatagrams.clear();
}

void UDPClient::slotHostInfoReceived(const QHostInfo &_hostInfo)
{
    lookupId = -1;
    const QList<QHostAddress> addresses = _hostInfo.addresses();
    if ((_hostInfo.error() != QHostInfo::NoError) || (addresses.isEmpty()))
    {
        pendingDatagrams.clear();
        emit errorSending(tr("KLogServer: The server %1 could not be found.").arg(server));
        return;
    }

    serverAddress = addresses.first();
    for (const QHostAddress &address : addresses)
    {   // IPv4 is preferred as it is what KLogServer binds to by default.
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            serverAddress = address;
            break;
        }
    }

    while (!pendingDatagrams.isEmpty())
        writeDatagram(pendingDatagrams.dequeue());
}
