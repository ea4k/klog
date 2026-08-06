/***************************************************************************
                          udpserver.cpp  -  description
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

#include "udpserver.h"
#include "callsign.h"
#include <QDateTime>
//https://sourceforge.net/p/wsjt/wsjtx/ci/master/tree/UDPExamples/MessageServer.cpp
// Read Network\NetworkMessage.hpp in the WSJT-x sources wsjtx

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
       //qDebug() << Q_FUNC_INFO << " - Start";
       //address = QString("127.0.0.1");
       port = 2237;
       haveNetworkInterface = false;
       socketServer = new QUdpSocket(this);
       groupAddress = QHostAddress::Any;
       networkInterface = QNetworkInterface();
       /*
       //if (socketServer->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress))
       if (socketServer->bind(port, QAbstractSocket::ShareAddress))
       {
            //qDebug() << "UDPServer::UDPServer - Multicast group joined OK" ;
       }
       else
       {
            //qDebug() << "UDPServer::UDPServer - Multicast group joined NOK" ;
       }
       */
        util = new Utilities(Q_FUNC_INFO);
        logging = false;
        realtime = false;
        dialFrequency = 0.0;
        clientAddress = QHostAddress();
        clientPort = 0;
        clientSchema = 2;
        replayRequested = false;
        connect(socketServer,SIGNAL(readyRead()),this,SLOT(slotReadPendingDatagrams()));
       //qDebug() << Q_FUNC_INFO << " - END";
}

UDPServer::~UDPServer()
{
    delete(util);
}

void UDPServer::slotReadPendingDatagrams()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    while (socketServer->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketServer->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
       //qDebug() << Q_FUNC_INFO << " : length = " << QString::number(socketServer->pendingDatagramSize());
        socketServer->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        // WSJT-X answers on the address & port it sent the datagram from, so
        // they are needed to be able to talk back to it.
        clientAddress = sender;
        clientPort = senderPort;
        parse (datagram);
       //qDebug() << Q_FUNC_INFO << "  = " << datagram;
    }
   //qDebug() << Q_FUNC_INFO << " - END";
}

bool UDPServer::start()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if ( (port>0) && (port<65535) )
    {
        //qDebug() << Q_FUNC_INFO << ": calling startNow ";
        return startNow(port, groupAddress);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": FALSE";
        return false;
    }
}

bool UDPServer::startNow(quint16 _port, QHostAddress const& _multicast_group_address)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    //if ((_port != port) || (_multicast_group_address != groupAddress))
    if (1)
    {
        //qDebug() << Q_FUNC_INFO << ": starting...";
        leaveMultiCastGroup();
        if (socketServer->state() == QAbstractSocket::BoundState)
        {
            //qDebug() << Q_FUNC_INFO << ": closing socket";
            socketServer->close();
        }
        groupAddress = _multicast_group_address;
        // A multicast group is joined after binding to the wildcard address,
        // never by binding to the group address itself: that is what WSJT-X
        // does and the only thing that works on every platform.
        auto address = _multicast_group_address.isNull ()
                || QAbstractSocket::IPv4Protocol != groupAddress.protocol () ? QHostAddress::Any : QHostAddress::AnyIPv4;
        if (_port && socketServer->bind(address, _port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint))
        {
            port = _port;
            // A new socket knows nothing about the WSJT-X instance yet, and
            // it has to ask it again for the decodes it is showing.
            dialFrequency = 0.0;
            replayRequested = false;
            joinMultiCastGroup();
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << "  port = 0";
            port = 0;
        }
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << "  exiting with an error... ";
    }
    //qDebug() << Q_FUNC_INFO << "  finalizing... ";
    return  socketServer->isValid();
}

void UDPServer::joinMultiCastGroup()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if ((socketServer->state() == QAbstractSocket::BoundState) && (!groupAddress.isNull())  && groupAddress.isMulticast()   )
    {
        auto mcast_interface = socketServer->multicastInterface();
        if (( groupAddress.protocol() ==  QAbstractSocket::IPv4Protocol ) && (socketServer->localAddress().protocol() != QAbstractSocket::IPv4Protocol ))
        {
            socketServer->close();
            socketServer->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
            if (socketServer->isValid())
            {
               //qDebug() << Q_FUNC_INFO << ": socket valid";
            }
        }
        bool joined {false};

        if (haveNetworkInterface)
        {
            socketServer->setMulticastInterface(networkInterface);
            joined |= socketServer->joinMulticastGroup(groupAddress, networkInterface);
        }
        else
        {   // No interface was selected in the setup: the group is joined on
            // every interface that can do it, or KLog would hear nothing.
            foreach (QNetworkInterface i, QNetworkInterface::allInterfaces())
            {
                if ((i.flags().testFlag(QNetworkInterface::IsUp)) &&
                    (i.flags().testFlag(QNetworkInterface::CanMulticast)))
                    joined |= socketServer->joinMulticastGroup(groupAddress, i);
            }
        }

        if (!joined)
        {
            groupAddress.clear();
        }
        socketServer->setMulticastInterface(mcast_interface);
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void UDPServer::leaveMultiCastGroup()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if ((!groupAddress.isNull()) && (socketServer->state() == QAbstractSocket::BoundState) && groupAddress.isMulticast())
    {
        QList<QNetworkInterface> interfaces;
        interfaces.clear();
        interfaces.append(QNetworkInterface::allInterfaces());
        for (int i = 0; i < interfaces.size(); ++i)
        {
            socketServer->leaveMulticastGroup(groupAddress, interfaces.at(i));
        }
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

 bool UDPServer::isStarted()
 {
   //qDebug() << Q_FUNC_INFO << " - Start";
     return  socketServer->isValid();
 }

void UDPServer::parse(const QByteArray &msg)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << msg;
    //qDebug() << "UDPServer::parse: " << QString::fromStdString(msg.toStdString());
    //in >> time_off >> dx_call >> dx_grid >> frequency >> mode >> report_sent >> report_received >>
    //        tx_power >> comments >> name >> time_on >> operatorCall >> de_call >> de_grid >>
    //        exchange_sent >> exchange_received;

    quint32 magic;
    quint32 schema;
    quint32 type;
    QByteArray id;
    QByteArray adifReceived;

    QDateTime time_off, time_on;
    QByteArray dx_call;
    QByteArray operatorCall;
    QByteArray dx_grid;
    quint64 frequency = 0; //  In Hz??
    double frequencyDouble;

    QByteArray mode;
    QByteArray report_sent;
    QByteArray report_received;
    QByteArray exchange_sent;
    QByteArray exchange_received;
    QByteArray tx_power;
    QByteArray comments;
    QByteArray name;
    // qByteArray time_on; // Note: LOTW uses TIME_ON for their +/- 30-minute time

    QByteArray report;
    QByteArray tx_mode;
    bool tx_enabled = false;
    bool transmitting = false;
    bool decoding = false;

    // WSPRDecode
    bool newDecode = false;
    QTime time;
    qint32 snr;
    double deltaTime = 0.0;
    qint32 drift;
    QByteArray decodedCall, decodedGrid;
    qint32 power;
    bool offAir;

    // DECODE
    QByteArray message;
    bool lowConfidence = false;
    quint32 deltaFrequency = 0;   // Audio offset of the decode, in Hz

    qint32 rx_df = -1;  // Delta frequency
    qint32 tx_df = -1;  // Delta time
    QByteArray de_call;
    QByteArray de_grid;
    bool watchdog_timeout = false;
    QByteArray sub_mode;
    bool fast_mode = false;
    quint8 specialOpMode;
    quint32 freqTolerance;
    quint32 TRPeriod;
    QByteArray confName;

    QDataStream in(msg);
    // qDataStream out(msgOut, QIODevice::ReadWrite);
    in.setVersion(16);
    in.setByteOrder(QDataStream::BigEndian);


    in >> magic >> schema >> type >> id;
    // qByteArray ba4(QByteArray::fromRawData(cart, 6));
    //in.readRawData(type, size)
       //qDebug() << "UDPServer::parse: -  Magic = " << QString::number(magic);
       //qDebug() << "UDPServer::parse: - schema = " << QString::number(schema);
       //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type);
       //qDebug() << Q_FUNC_INFO << ": id = " << id;


    //if ((magic != 2914831322) || (id != "WSJT-X"))
    if (magic != 2914831322)
    {
        //qDebug() << Q_FUNC_INFO << ": Magic BAD FORMAT = " << QString::number(magic);
        return;
    }

    // Any valid datagram identifies the WSJT-X instance we are talking to.
    clientId = id;
    clientSchema = schema;

    //qDebug() << Q_FUNC_INFO << ": TYPE: " << QString::number(type);

    switch (type)
    {
        case Heartbeat:
            //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - OUT/IN - Heartbeat";
        break;
        case Status:
            //qDebug() << Q_FUNC_INFO << ": type" << QString::number(type) << " - OUT - Status";
            // unpack message
            //in >> ch;

            in >> frequency >> mode >> dx_call >> report >> tx_mode >> tx_enabled >> transmitting >> decoding
               >> rx_df >> tx_df >> de_call >> de_grid >> dx_grid >> watchdog_timeout >> sub_mode
               >> fast_mode >> specialOpMode >> freqTolerance >> TRPeriod >> confName;
            frequencyDouble = (double)frequency;
            frequencyDouble = frequencyDouble/1000000; // Change to MHz

            // The Status message is the only one carrying the dial frequency,
            // the mode and our own callsign, and the decodes are meaningless
            // without them, so they are kept even when the user does not want
            // WSJT-X to update the QSO being entered.
            dialFrequency = frequencyDouble;
            dialMode = QString::fromUtf8(mode);
            wsjtxCallsign = QString::fromUtf8(de_call).toUpper();

            // Now that the band is known the decodes make sense, so this is
            // the moment to ask WSJT-X for the ones it is already showing.
            if (!replayRequested && (dialFrequency > 0.0))
                requestReplay();

            if (realtime)
            {
               //qDebug() << Q_FUNC_INFO << ": Status - Freq double = " << QString::number(frequencyDouble) ;
               //qDebug() << Q_FUNC_INFO << ": Status - Mode = " << mode;
               //qDebug() << Q_FUNC_INFO << ": Status - DXCall = " << dx_call;
               //qDebug() << Q_FUNC_INFO << ": Status - Report = " << report;
               //qDebug() << Q_FUNC_INFO << ": Status - Rx DF = " << rx_df;
               //qDebug() << Q_FUNC_INFO << ": Status - Tx DF = " << tx_df;
               //qDebug() << Q_FUNC_INFO << ": Status - De_Call = " << de_call;
               //qDebug() << Q_FUNC_INFO << ": Status - De_Grid = " << de_grid;
               //qDebug() << Q_FUNC_INFO << ": Status - SubMode = " << sub_mode;
               //qDebug() << Q_FUNC_INFO << ": Status - SpecialOp = " << QString::number(specialOpMode);
               //qDebug() << Q_FUNC_INFO << ": Status - Freq Tol = " << QString::number(freqTolerance);
               //qDebug() << Q_FUNC_INFO << ": Status - T/R Period = " << QString::number(TRPeriod);
               //qDebug() << Q_FUNC_INFO << ": Status - Conf Name = " << confName;

                emit status_update (type, dx_call, frequencyDouble, mode, report, de_call, de_grid, dx_grid, sub_mode);
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": realtime = FALSE";
            }
        break;
        case Decode:
        {
            //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - OUT - Decode";
            in >> newDecode >> time >> snr >> deltaTime >> deltaFrequency >> mode >> message >> lowConfidence >> offAir;

            //qDebug() << Q_FUNC_INFO << ": Decode:    Time = " << time.toString("hhmmss");
            //qDebug() << Q_FUNC_INFO << ": Decode:    SNR = " << QString::number(snr);
            //qDebug() << Q_FUNC_INFO << ": Decode:    Delta = " << QString::number(deltaTime);
            //qDebug() << Q_FUNC_INFO << ": Decode:    DeltaFreq = " << QString::number(deltaFrequency);
            //qDebug() << Q_FUNC_INFO << ": Decode:    Mode = " << mode;
            //qDebug() << Q_FUNC_INFO << ": Decode:    Msg = " << message;

            if (lowConfidence)          // WSJT-X does not trust this decode
                break;
            if (dialFrequency <= 0.0)   // No Status yet: the band is unknown
                break;

            WSJTXDecodedStation station = stationFromDecodedText(QString::fromUtf8(message));
            if (station.isEmpty())      // Free text, telemetry or a hashed call
                break;
            if (station.call == wsjtxCallsign)  // Ourselves, heard back
                break;

            // The decode carries the audio offset only; the RF frequency is
            // the dial frequency of the last Status message plus that offset.
            double decodedFrequency = dialFrequency + (double)deltaFrequency/1000000;

            QString decodedMode = modeFromDecodeChar(QString::fromUtf8(mode));
            if (decodedMode.isEmpty())
                decodedMode = dialMode;

            // WSJT-X only tells the time of the decode: a replay may bring
            // decodes from just before midnight while it is already past it.
            QDateTime when = QDateTime(QDate::currentDate(), time, QTimeZone::UTC);
            if (when > QDateTime::currentDateTimeUtc())
                when = when.addDays(-1);

            emit stationDecoded(station.call, decodedFrequency, decodedMode, snr,
                                station.callingCQ, when);
        }
        break;
        case Clear:
            //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - OUT - Clear";
            emit clearSignal(Q_FUNC_INFO);
        break;
        case Reply:
            //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - IN - Replay ";
        break;
        case QSOLogged:
            //qDebug() << Q_FUNC_INFO << ": type = QSOLogged " ;


        in >> time_off >> dx_call >> dx_grid >> frequency >> mode >> report_sent >> report_received >>
                    tx_power >> comments >> name >> time_on >> operatorCall >> de_call >> de_grid >>
                    exchange_sent >> exchange_received;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Time_off = " << time_off.toString("yyyyMMdd-hhmmss");
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    DXCall = " << dx_call;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Grid = " << dx_grid;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Freq = " << QString::number(frequency);
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Mode = " << mode;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    ReportSent = " << report_sent;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    ReportReceived = " << report_received;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    TX_PWR = " << tx_power;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Comments = " << comments;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Name = " << name;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Time = " << time_on.toString("yyyyMMdd-hhmmss");
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    DeCall = " << de_call;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    DeGrid = " << de_grid;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Exch Sent = " << exchange_sent;
            //qDebug() << Q_FUNC_INFO << ": QSOLogged:    Exch Recv = " << exchange_received;

            if (logging)
            {
                //qDebug() << Q_FUNC_INFO << ": logging = true ";


                //qDebug() << Q_FUNC_INFO << ": QSO to be logged: Time_on: " << time_on;
                //qDebug() << Q_FUNC_INFO << ": QSO to be logged: Time_off: " << time_off;
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz

                //qDebug() << Q_FUNC_INFO << ": Data to be logged: Comment: " << comments;
                QSO qso;
                qso.clear();
                qso.setCall(dx_call);
                qso.setMode(mode);
                qso.setFreq(frequencyDouble);
                qso.setGridSquare(dx_grid);
                qso.setMyGridSquare(de_grid);
                qso.setRSTRX(report_received);
                qso.setRSTTX(report_sent);
                qso.setComment(comments);
                qso.setStationCallsign(de_call);
                qso.setName(name);
                qso.setOperatorCallsign(operatorCall);
                qso.setDateTimeOn(time_on);
                qso.setTimeOff(time_off.time());
                qso.setSrxString(exchange_received);
                qso.setStxString(exchange_sent);
                qso.setTXPwr(tx_power.toDouble());
                emit logged(qso);

            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": logging = false";
            }

        break;
        case Close:
            //qDebug() << Q_FUNC_INFO << ": type = Close" ;
            socketServer->close();
               //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - OUT - Close ";
        break;
        case Replay:
            //qDebug() << Q_FUNC_INFO << ": type = Reply" ;
               //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - IN - Replay";
        break;
        case HaltTx:
            //qDebug() << Q_FUNC_INFO << ": type = HaltTx" ;
               //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - IN - Halt TX";
        break;
        case FreeText:
            //qDebug() << Q_FUNC_INFO << ": type = FreeText" ;
               //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - IN - Free Text";
        break;
        case WSPRDecode:
            //qDebug() << Q_FUNC_INFO << ": type = WSPRDecode" ;
            in >> newDecode >> time >> snr >> deltaTime >> frequency >> drift >> decodedCall >> decodedGrid >> power >> offAir;

            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    Time = " << time.toString("yyyyMMdd-hhmmss");
            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    SNR = " << QString::number(snr);
            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    Delta = " << QString::number(deltaTime);
            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    Frec = " << QString::number(frequency);
            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    Drift = " << QString::number(drift);
            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    Call = " << decodedCall;
            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    Grid = " << decodedGrid;
            //qDebug() << Q_FUNC_INFO << ": WSPRDecode:    Power = " << QString::number(power);

        break;
        case Location:
            //qDebug() << Q_FUNC_INFO << ": type = Location" ;
        break;
        case LoggedADIF:
           //qDebug() << Q_FUNC_INFO << ": type = LoggedADIF" ;
           //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - ADIF";
           //in >> adifReceived;
           //adifParse(adifReceived);
            //qDebug() << "UDPServer::parse: ADIF: " << adifReceived;
           // Q_SIGNAL void logged_ADIF (QString const& id, QByteArray const& ADIF);
        break;
        case HighlightCallsign:
            //qDebug() << Q_FUNC_INFO << ": type = HighlightCallsign" ;
        break;
        case SwitchConfiguration:
            //qDebug() << Q_FUNC_INFO << ": type = SwitchConfiguration" ;
        break;
        case Configure:
            //qDebug() << Q_FUNC_INFO << ": type = Configure" ;
        break;
        default: //NO
               //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - ERROR on Type";
        break;
    }
       //qDebug() << Q_FUNC_INFO << ": Magic: = " << QString::number(magic);
}


void UDPServer::requestReplay()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    // The Replay message makes WSJT-X send again every decode it is showing
    // in its band activity window, so KLog does not have to wait for the next
    // decoding period to know what is on the air. WSJT-X honours it only when
    // "Accept UDP requests" is enabled, and simply ignores it otherwise.
    if ((clientPort == 0) || clientAddress.isNull() || !socketServer->isValid())
        return;
    replayRequested = true;     // Asked once per WSJT-X instance and socket

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(16);
    out.setByteOrder(QDataStream::BigEndian);
    out << quint32(0xadbccbda) << clientSchema << quint32(Replay) << clientId;
    socketServer->writeDatagram(datagram, clientAddress, clientPort);
    //qDebug() << Q_FUNC_INFO << " - END";
}

QString UDPServer::modeFromDecodeChar(const QString &_mode)
{
    // The decodes do not name the mode: they carry the single character that
    // WSJT-X shows in its band activity window. Anything unknown is left to
    // the caller, that falls back to the mode of the Status message.
    if (_mode.length() != 1)
        return _mode.trimmed();

    switch (_mode.at(0).toLatin1())
    {
        case '~': return "FT8";
        case '+': return "FT4";
        case '#': return "JT65";
        case '@': return "JT9";
        case '&': return "MSK144";
        case ':': return "Q65";
        case '`': return "FST4";
        case '$': return "JT4";
        default:  return QString();
    }
}

WSJTXDecodedStation UDPServer::stationFromDecodedText(const QString &_message)
{
    // The station to spot is the one that transmitted the message, which in
    // every standard WSJT-X message is either the one calling CQ or the one
    // right after the callsign being called:
    //      "CQ EA4K IN80" / "CQ DX EA4K" / "CQ POTA EA4K IN80"
    //      "W1AW EA4K -15" / "W1AW EA4K RR73" / "<W1AW> PJ4/EA4K R+03"
    //      "W1AW RR73; K1ABC <KH1/KH7Z> -08"   (DXpedition mode: the fox)
    WSJTXDecodedStation station;
    const QStringList tokens = _message.simplified().toUpper().split(' ', Qt::SkipEmptyParts);
    if (tokens.count() < 2)
        return station;     // Free text or telemetry: no station to spot

    // A hashed callsign KLog cannot resolve arrives as "<...>"; when WSJT-X
    // does know it, it sends it between brackets and it is perfectly usable.
    auto callFromToken = [](const QString &_token)
    {
        QString call = _token;
        if (call.startsWith('<'))
            call.remove(0, 1);
        if (call.endsWith('>'))
            call.chop(1);
        Callsign callsign(call);
        return callsign.isValid() ? call : QString();
    };

    if ((tokens.at(0) == "CQ") || (tokens.at(0) == "QRZ"))
    {
        // The directive between CQ and the callsign is optional and may be a
        // continent, a contest or a region ("CQ DX", "CQ EU", "CQ TEST"): the
        // callsign is simply the first token that is one.
        for (int i = 1; (i < tokens.count()) && (i < 3); i++)
        {
            station.call = callFromToken(tokens.at(i));
            if (!station.call.isEmpty())
                break;
        }
        station.callingCQ = !station.call.isEmpty();
        return station;
    }

    if ((tokens.count() > 3) && tokens.at(1).startsWith("RR73;"))
    {   // DXpedition mode: the fox is the station between brackets
        station.call = callFromToken(tokens.at(3));
        return station;
    }

    station.call = callFromToken(tokens.at(1));
    return station;
}

bool UDPServer::stop()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    socketServer->close();
    if (socketServer->isValid())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void UDPServer::setPort(const int _port)
{
   //qDebug() << Q_FUNC_INFO << " - Start: " << QString::number(_port);
    if ((_port >= 0) && (_port<=65535))
    {
        port = _port;
    }
}

void UDPServer::setNetworkInterface(const QString &_t)
{
   //qDebug() << Q_FUNC_INFO << " - Start: " << _t;
    QString testInterface;
    testInterface.clear();
    QList<QNetworkInterface> ifaces;
    ifaces.clear();
    ifaces << QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface i, ifaces)
    {
        testInterface = i.humanReadableName() + "-" + i.hardwareAddress();
        if (testInterface.contains(_t))
        {
           //qDebug() << Q_FUNC_INFO << ": FOUND! :" << testInterface ;
            if ((i.flags().testFlag(QNetworkInterface::IsUp)) )
            {
                networkInterface = i;
                haveNetworkInterface = true;
            }
        }
    }
}

void UDPServer::setMultiCastAddress(const QString &_address)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << _address;
    // An empty or invalid address keeps KLog listening on plain unicast, as
    // it has always done; a multicast group lets it share the datagrams with
    // other programs like JTAlert or GridTracker.
    QHostAddress address(_address.trimmed());
    if (_address.trimmed().isEmpty() || address.isNull() || !address.isMulticast())
        groupAddress = QHostAddress::Any;
    else
        groupAddress = address;
}

void UDPServer::loadSettings()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("UDPServer");

    setNetworkInterface (settings.value("UDPNetworkInterface").toString ());
    setMultiCastAddress (settings.value("UDPMultiCastAddress").toString ());
    setPort (settings.value("UDPServerPort").toInt ());
    logging = settings.value("LogFromWSJTX").toBool ();
    realtime = settings.value ("RealTimeFromWSJTX", false).toBool ();
    settings.endGroup ();

   //qDebug() << Q_FUNC_INFO << " - END";
}
