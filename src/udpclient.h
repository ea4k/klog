#ifndef KLOG_UDPCLIENT_H
#define KLOG_UDPCLIENT_H
/***************************************************************************
                          udpclient.h  -  description
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

// Sends the QSOs that are logged in KLog to a KLogServer instance.
//
// The QSO travels as a plain ADIF record (the very same one that KLog writes
// when exporting to a file, so all the ADIF fields with data are sent) in a
// single UDP datagram. KLogServer listens in the same way it listens for
// WSJT-X or N1MM data and appends the record to its ADIF file.

#include <QHostAddress>
#include <QHostInfo>
#include <QObject>
#include <QQueue>
#include <QUdpSocket>

#include "qso.h"
#include "utilities.h"

class UDPClient : public QObject
{
    Q_OBJECT

public:
    explicit UDPClient(QObject *parent = nullptr);
    ~UDPClient();

    void loadSettings();

    void setEnabled(const bool _enabled);
    bool isEnabled() const;

    // The server may be an IP address (no name resolution is needed) or a hostname.
    void setServer(const QString &_server);
    QString getServer() const;

    void setPort(const int _port);
    int getPort() const;

    // Both return false when nothing was sent (disabled, not configured or an
    // empty/incomplete QSO). A true return means the datagram was sent or
    // queued while the hostname is being resolved.
    bool sendQSO(const QSO &_qso);
    bool sendADIF(const QString &_adif);

signals:
    void errorSending(const QString &_error);

private slots:
    void slotHostInfoReceived(const QHostInfo &_hostInfo);

private:
    bool queueOrSend(const QByteArray &_datagram);
    bool writeDatagram(const QByteArray &_datagram);
    void resolveServer();
    void clearServerAddress();

    QUdpSocket *socket;
    QHostAddress serverAddress;   // Null while the hostname has not been resolved
    QString server;
    int port;
    bool enabled;
    int lookupId;                 // -1 when no name resolution is running
    QQueue<QByteArray> pendingDatagrams;
    Utilities *util;

    // Datagrams waiting for the hostname to be resolved. QSOs are logged one by
    // one and a lookup takes a moment, so a short queue is more than enough.
    static constexpr int maxPendingDatagrams = 20;
    static constexpr int defaultPort = 2237;
};

#endif // KLOG_UDPCLIENT_H
