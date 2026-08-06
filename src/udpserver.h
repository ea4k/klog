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
#include <QDateTime>
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

// What KLog could make out of one of the lines WSJT-X decodes, like
// "CQ EA4K IN80" or "EA4K W1AW -15": who transmitted it and whether that
// station is calling CQ (and is therefore ready to be worked right now).
struct WSJTXDecodedStation
{
    QString call;             // Station that transmitted the decoded message
    bool    callingCQ = false;

    bool isEmpty() const { return call.isEmpty(); }
};

class UDPServer : public QObject
{
    Q_OBJECT
    friend class tst_UDPServer;

public:
    explicit UDPServer(QObject *parent = nullptr);
    ~UDPServer();
    bool start();

    bool stop();
    bool isStarted();
    //void setAddress(const QString &_address);
    void setPort(const int _port);
    void setNetworkInterface(const QString &_t);
    void setMultiCastAddress(const QString &_address);
    void loadSettings();

    // Extracts the station behind one of the messages WSJT-X decodes. It is
    // static and public so it can be tested without a socket.
    static WSJTXDecodedStation stationFromDecodedText(const QString &_message);

private:
    void readPendingDatagrams();
    void parse(const QByteArray &msg);
    //void adifParse(QByteArray &msg);
    void leaveMultiCastGroup();
    void joinMultiCastGroup();
    bool startNow(quint16 _port, QHostAddress const& _multicast_group_address);
    void requestReplay();       // Asks WSJT-X to resend the decodes it is showing
    static QString modeFromDecodeChar(const QString &_mode);

    QNetworkInterface networkInterface;
    QUdpSocket *socketServer;
    QHostAddress groupAddress;

    // qString address;
    int port;
    bool logging, realtime;
    bool haveNetworkInterface;

    // The decodes only carry the offset from the dial frequency, so the data
    // of the last Status message is kept to make sense of them.
    double dialFrequency;       // In MHz, 0.0 until the first Status arrives
    QString dialMode;           // Mode WSJT-X is running, as it names it
    QString wsjtxCallsign;      // Station callsign WSJT-X is operating with

    // Where the WSJT-X instance we are listening to can be reached, needed to
    // ask it for a replay of the decodes it is currently showing.
    QHostAddress clientAddress;
    quint16 clientPort;
    QByteArray clientId;
    quint32 clientSchema;
    bool replayRequested;

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

    // A station decoded by WSJT-X. The frequency is the dial frequency of the
    // last Status message plus the audio offset of the decode, in MHz, and
    // the date & time are those of the decode itself: a replay of the WSJT-X
    // window brings decodes that may be several minutes old.
    void stationDecoded(const QString &_dxCall, const double _freq, const QString &_mode,
                        const int _snr, const bool _callingCQ, const QDateTime &_dateTime);

    void clearSignal(QString _func);

private slots:
    void slotReadPendingDatagrams();
};

#endif // UDPSERVER_H
