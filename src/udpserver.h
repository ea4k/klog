#ifndef KLOG_UDPSERVER_H
#define KLOG_UDPSERVER_H
/***************************************************************************
                          udpserver.h  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
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

#include <QDataStream>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QObject>
#include <QSettings>
#include <QUdpSocket>
#include "qso.h"
//#include "frequency.h"
#include "utilities.h"

enum Type
    {
      Heartbeat,
      Status,
      Decode,
      Clear,
      Reply,
      QSOLogged,
      Close,
      Replay,
      HaltTx,
      FreeText,
      WSPRDecode,
      Location,
      LoggedADIF,
      HighlightCallsign,
      SwitchConfiguration,
      Configure,
      maximum_message_type_     // ONLY add new message types
                                // immediately before here
};

class UDPServer : public QObject
{
    Q_OBJECT

public:
    explicit UDPServer(QObject *parent = nullptr);
    ~UDPServer();
    bool start();

    bool stop();
    bool isStarted();
    //void setAddress(const QString &_address);
    void setPort(const int _port);
    void setNetworkInterface(const QString &_t);
    void loadSettings();

private:
    void readPendingDatagrams();
    void parse(const QByteArray &msg);
    //void adifParse(QByteArray &msg);
    void leaveMultiCastGroup();
    void joinMultiCastGroup();
    bool startNow(quint16 _port, QHostAddress const& _multicast_group_address);

    QNetworkInterface networkInterface;
    QUdpSocket *socketServer;
    QHostAddress groupAddress;

    // qString address;
    int port;
    bool logging, realtime;
    bool haveNetworkInterface;

    Utilities *util;

#if QT_VERSION >= 0x050400
    static quint32 constexpr schema_number {3};
#elif QT_VERSION >= 0x050200
    static quint32 constexpr schema_number {2};
#else
    // Schema 1 (Qt_5_0) is broken
#error "Qt version 5.2 or greater required"
#endif

signals:
    void status_update (const int _type, const QString &_dxcall, const double _freq, const QString &_mode,
                        const QString report, const QString de_call, const QString de_grid,
                        const QString dx_grid, const QString sub_mode);

    void logged(const QSO &qso);

    void clearSignal(QString _func);

private slots:
    void slotReadPendingDatagrams();
};

#endif // UDPSERVER_H
