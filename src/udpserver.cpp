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
#include <QDateTime>
//https://sourceforge.net/p/wsjt/wsjtx/ci/master/tree/UDPExamples/MessageServer.cpp
// Read Network\NetworkMessage.hpp in the WSJT-x sources wsjtx

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
       //qDebug() << "UDPServer::UDPServer" ;
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
        connect(socketServer,SIGNAL(readyRead()),this,SLOT(slotReadPendingDatagrams()));
}

UDPServer::~UDPServer()
{
    delete(util);
}

void UDPServer::slotReadPendingDatagrams()
{
    //qDebug() << "UDPServer::slotReadPendingDatagrams" ;
    while (socketServer->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketServer->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        //qDebug() << "UDPServer::slotReadPendingDatagrams: length = " << QString::number(socketServer->pendingDatagramSize());
        socketServer->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        parse (datagram);
        //qDebug() << "UDPServer::slotReadPendingDatagrams: = " << datagram;
    }
}

bool UDPServer::start()
{
    //qDebug() << "UDPServer::start ";
    if ( (port>0) && (port<65535) )
    {
        //qDebug() << "UDPServer::start: calling startNow ";
        return startNow(port, groupAddress);
    }
    else
    {
        //qDebug() << "UDPServer::start FALSE";
        return false;
    }
}

bool UDPServer::startNow(quint16 _port, QHostAddress const& _multicast_group_address)
{
    //qDebug() << "UDPServer::startNow ";
    //if ((_port != port) || (_multicast_group_address != groupAddress))
    if (1)
    {
        //qDebug() << "UDPServer::startNow starting...";
        leaveMultiCastGroup();
        if (socketServer->state() == QAbstractSocket::BoundState)
        {
            //qDebug() << "UDPServer::startNow: closing socket";
            socketServer->close();
        }
        groupAddress = _multicast_group_address;
        auto address = _multicast_group_address.isNull ()
                || QAbstractSocket::IPv4Protocol != groupAddress.protocol () ? QHostAddress::Any : QHostAddress::AnyIPv4;
        Q_UNUSED(address);
        if (_port && socketServer->bind(groupAddress, _port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint))
        {
            port = _port;
            joinMultiCastGroup();
        }
        else
        {
            //qDebug() << "UDPServer::startNow port = 0";
            port = 0;
        }
    }
    else
    {
       //qDebug() << "UDPServer::startNow exiting with an error... ";
    }
    //qDebug() << "UDPServer::startNow finalizing... ";
    return  socketServer->isValid();
}

void UDPServer::joinMultiCastGroup()
{
    //qDebug() << "UDPServer::joinMultiCastGroup: ";
    if (!haveNetworkInterface)
    {
        return;
    }

    if ((socketServer->state() == QAbstractSocket::BoundState) && (!groupAddress.isNull())  && groupAddress.isMulticast()   )
    {
        auto mcast_interface = socketServer->multicastInterface();
        if (( groupAddress.protocol() ==  QAbstractSocket::IPv4Protocol ) && (socketServer->localAddress().protocol() != QAbstractSocket::IPv4Protocol ))
        {
            socketServer->close();
            socketServer->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
            if (socketServer->isValid())
            {
               //qDebug() << "UDPServer::joinMultiCastGroup socket valid";
            }
        }
        bool joined {false};

        /*
        QList<QNetworkInterface> interfaces;
        interfaces.clear();
        interfaces.append(QNetworkInterface::allInterfaces());

        for (int i = 0; i < interfaces.size(); ++i)
        {
            socketServer->setMulticastInterface(interfaces.at(i));
            joined |= socketServer->joinMulticastGroup(groupAddress, interfaces.at(i));
        }
        */

        socketServer->setMulticastInterface(networkInterface);
        joined |= socketServer->joinMulticastGroup(groupAddress, networkInterface);

        if (!joined)
        {
            groupAddress.clear();
        }
        socketServer->setMulticastInterface(mcast_interface);
    }
    //qDebug() << "UDPServer::joinMultiCastGroup - END";
}

void UDPServer::leaveMultiCastGroup()
{
    //qDebug() << "UDPServer::leaveMultiCastGroup";
    if (groupAddress.isNull() && socketServer->state() && groupAddress.isMulticast())
    {
        QList<QNetworkInterface> interfaces;
        interfaces.clear();
        interfaces.append(QNetworkInterface::allInterfaces());
        for (int i = 0; i < interfaces.size(); ++i)
        {
            socketServer->leaveMulticastGroup(groupAddress, interfaces.at(i));
        }
    }
    //qDebug() << "UDPServer::leaveMultiCastGroup - END";
}

 bool UDPServer::isStarted()
 {
     return  socketServer->isValid();
 }

void UDPServer::parse(const QByteArray &msg)
{
    //qDebug() << "UDPServer::parse: " << msg;
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
    //QByteArray time_on; // Note: LOTW uses TIME_ON for their +/- 30-minute time

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
    //QDataStream out(msgOut, QIODevice::ReadWrite);
    in.setVersion(16);
    in.setByteOrder(QDataStream::BigEndian);


    in >> magic >> schema >> type >> id;
    //QByteArray ba4(QByteArray::fromRawData(cart, 6));
    //in.readRawData(type, size)
       //qDebug() << "UDPServer::parse: -  Magic = " << QString::number(magic);
       //qDebug() << "UDPServer::parse: - schema = " << QString::number(schema);
       //qDebug() << "UDPServer::parse: -   type = " << QString::number(type);
       //qDebug() << "UDPServer::parse: -   id = " << id;


    //if ((magic != 2914831322) || (id != "WSJT-X"))
    if (magic != 2914831322)
    {
        //qDebug() << "UDPServer::parse: - Magic BAD FORMAT = " << QString::number(magic);
        return;
    }

    //qDebug() << "UDPServer::parse: TYPE: " << QString::number(type);

    switch (type)
    {
        case Heartbeat:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT/IN - Heartbeat";
        break;
        case Status:
            //qDebug() << "UDPServer::parse: -   type" << QString::number(type) << " - OUT - Status";
            // unpack message
            //in >> ch;

            if (realtime)
            {
                in >> frequency >> mode >> dx_call >> report >> tx_mode >> tx_enabled >> transmitting >> decoding
                   >> rx_df >> tx_df >> de_call >> de_grid >> dx_grid >> watchdog_timeout >> sub_mode
                   >> fast_mode >> specialOpMode >> freqTolerance >> TRPeriod >> confName;
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz
               //if (!util->isValidCall(dx_call))
               // {
               //     //qDebug() << "UDPServer::parse: Status : Call not valid: -" << dx_call << "-";
               //     return;
               //}
               //qDebug() << "UDPServer::parse: Status - Freq double = " << QString::number(frequencyDouble) ;
               //qDebug() << "UDPServer::parse: Status - Mode = " << mode;
               //qDebug() << "UDPServer::parse: Status - DXCall = " << dx_call;
               //qDebug() << "UDPServer::parse: Status - Report = " << report;
               //qDebug() << "UDPServer::parse: Status - Rx DF = " << rx_df;
               //qDebug() << "UDPServer::parse: Status - Tx DF = " << tx_df;
               //qDebug() << "UDPServer::parse: Status - De_Call = " << de_call;
               //qDebug() << "UDPServer::parse: Status - De_Grid = " << de_grid;
               //qDebug() << "UDPServer::parse: Status - SubMode = " << sub_mode;
               //qDebug() << "UDPServer::parse: Status - SpecialOp = " << QString::number(specialOpMode);
               //qDebug() << "UDPServer::parse: Status - Freq Tol = " << QString::number(freqTolerance);
               //qDebug() << "UDPServer::parse: Status - T/R Period = " << QString::number(TRPeriod);
               //qDebug() << "UDPServer::parse: Status - Conf Name = " << confName;

                emit status_update (type, dx_call, frequencyDouble, mode, report, de_call, de_grid, dx_grid, sub_mode);
            }
            else
            {
                //qDebug() << "UDPServer::parse: realtime = FALSE";
            }
        break;
        case Decode:
            //() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Decode";
            in >> newDecode >> time >> snr >> deltaTime >> drift >> mode >> message >> lowConfidence >> offAir;


            if (lowConfidence)
            {
                //qDebug() << "UDPServer::parse: Decode -   Low Confidence";
            }
            else
            {
                //qDebug() << "UDPServer::parse: Decode -   Time = " << time.toString("hhmmss");
                //qDebug() << "UDPServer::parse: Decode -   SNR = " << QString::number(snr);
                //qDebug() << "UDPServer::parse: Decode -   Delta = " << QString::number(deltaTime);
                //qDebug() << "UDPServer::parse: Decode -   Drift = " << QString::number(drift);
                //qDebug() << "UDPServer::parse: Decode -   Mode = " << mode;
                //qDebug() << "UDPServer::parse: Decode -   Msg = " << message;
            }

        break;
        case Clear:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Clear";
            emit clearSignal(Q_FUNC_INFO);
        break;
        case Reply:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay ";
        break;
        case QSOLogged:
            //qDebug() << "UDPServer::parse: -   type = QSOLogged " ;


        in >> time_off >> dx_call >> dx_grid >> frequency >> mode >> report_sent >> report_received >>
                    tx_power >> comments >> name >> time_on >> operatorCall >> de_call >> de_grid >>
                    exchange_sent >> exchange_received;
            //qDebug() << "UDPServer::parse: QSOLogged -   Time_off = " << time_off.toString("yyyyMMdd-hhmmss");
            //qDebug() << "UDPServer::parse: QSOLogged -   DXCall = " << dx_call;
            //qDebug() << "UDPServer::parse: QSOLogged -   Grid = " << dx_grid;
            //qDebug() << "UDPServer::parse: QSOLogged -   Freq = " << QString::number(frequency);
            //qDebug() << "UDPServer::parse: QSOLogged -   Mode = " << mode;
            //qDebug() << "UDPServer::parse: QSOLogged -   ReportSent = " << report_sent;
            //qDebug() << "UDPServer::parse: QSOLogged -   ReportReceived = " << report_received;
            //qDebug() << "UDPServer::parse: QSOLogged -   TX_PWR = " << tx_power;
            //qDebug() << "UDPServer::parse: QSOLogged -   Comments = " << comments;
            //qDebug() << "UDPServer::parse: QSOLogged -   Name = " << name;
            //qDebug() << "UDPServer::parse: QSOLogged -   Time = " << time_on.toString("yyyyMMdd-hhmmss");
            //qDebug() << "UDPServer::parse: QSOLogged -   DeCall = " << de_call;
            //qDebug() << "UDPServer::parse: QSOLogged -   DeGrid = " << de_grid;
            //qDebug() << "UDPServer::parse: QSOLogged -   Exch Sent = " << exchange_sent;
            //qDebug() << "UDPServer::parse: QSOLogged -   Exch Recv = " << exchange_received;

            if (logging)
            {
                //qDebug() << "UDPServer::parse: logging = true ";


                //qDebug() << "UDPServer::parse: QSO to be logged: Time_on: " << time_on;
                //qDebug() << "UDPServer::parse: QSO to be logged: Time_off: " << time_off;
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz

                //qDebug() << "UDPServer::parse: Data to be logged: Comment: " << comments;

                //dateTime.setDate(QDate::currentDate());
                //dateTime.setTime(QTime::fromString(time_on, "hhmmss"));

                //dateTimeOff.setDate(QDate::currentDate());
                //dateTimeOff.setTime(QTime::fromString(time_off, "hhmmss"));

                emit logged_qso(dx_call, mode, QString(), frequencyDouble,
                                de_grid, dx_grid, report_sent, report_received,
                                comments, de_call, name, operatorCall, time_on, time_off,
                                exchange_sent, exchange_received, tx_power);
            }
            else
            {
                //qDebug() << "UDPServer::parse: logging = false";
            }

        break;
        case Close:
            //qDebug() << "UDPServer::parse: -   type = Close" ;
            socketServer->close();
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Close ";
        break;
        case Replay:
            //qDebug() << "UDPServer::parse: -   type = Reply" ;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay";
        break;
        case HaltTx:
            //qDebug() << "UDPServer::parse: -   type = HaltTx" ;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Halt TX";
        break;
        case FreeText:
            //qDebug() << "UDPServer::parse: -   type = FreeText" ;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Free Text";
        break;
        case WSPRDecode:
            //qDebug() << "UDPServer::parse: -   type = WSPRDecode" ;
            in >> newDecode >> time >> snr >> deltaTime >> frequency >> drift >> decodedCall >> decodedGrid >> power >> offAir;

            //qDebug() << "UDPServer::parse: WSPRDecode -   Time = " << time.toString("yyyyMMdd-hhmmss");
            //qDebug() << "UDPServer::parse: WSPRDecode -   SNR = " << QString::number(snr);
            //qDebug() << "UDPServer::parse: WSPRDecode -   Delta = " << QString::number(deltaTime);
            //qDebug() << "UDPServer::parse: WSPRDecode -   Frec = " << QString::number(frequency);
            //qDebug() << "UDPServer::parse: WSPRDecode -   Drift = " << QString::number(drift);
            //qDebug() << "UDPServer::parse: WSPRDecode -   Call = " << decodedCall;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Grid = " << decodedGrid;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Power = " << QString::number(power);

        break;
        case Location:
            //qDebug() << "UDPServer::parse: -   type = Location" ;
        break;
        case LoggedADIF:
           //qDebug() << "UDPServer::parse: -   type = LoggedADIF" ;
           //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ADIF";
           //in >> adifReceived;
           //adifParse(adifReceived);
            //qDebug() << "UDPServer::parse: ADIF: " << adifReceived;
           // Q_SIGNAL void logged_ADIF (QString const& id, QByteArray const& ADIF);
        break;
        case HighlightCallsign:
            //qDebug() << "UDPServer::parse: -   type = HighlightCallsign" ;
        break;
        case SwitchConfiguration:
            //qDebug() << "UDPServer::parse: -   type = SwitchConfiguration" ;
        break;
        case Configure:
            //qDebug() << "UDPServer::parse: -   type = Configure" ;
        break;
        default: //NO
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ERROR on Type";
        break;
    }
       //qDebug() << "UDPServer::parse: - Magic: = " << QString::number(magic);
}


bool UDPServer::stop()
{
    //qDebug() << "UDPServer::stop";
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
       //qDebug() << "UDPServer::setPort: " << QString::number(_port);
    if ((_port >= 0) && (_port<=65535))
    {
        port = _port;
    }
}

void UDPServer::setNetworkInterface(const QString &_t)
{
    //qDebug() << "UDPServer::setNetworkInterface: " << _t;
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
            //qDebug() << "UDPServer::setNetworkInterface: FOUND! " << testInterface ;
            if ((i.flags().testFlag(QNetworkInterface::IsUp)) )
            {
                networkInterface = i;
                haveNetworkInterface = true;
            }
        }
    }
}


void UDPServer::adifParse(QByteArray &msg)
{
    //qDebug() << "UDPServer::adifParse: " << msg <<  QT_ENDL;
    QString dx_call = QString();
    QString mode = QString();
    QString band = QString();
    QString mygrid = QString();
    QString dxgrid = QString();
    QString rstTX = QString();
    QString rstRX = QString();
    QString _comment = QString();
    QString _name = QString();
    QString _exchangeTX = QString();
    QString _exchangeRX = QString();
    QString _myPWR = QString();
    QString operatorCall = QString();
    QString stationcallsign = QString();
    double freq = 0.0;
    QDateTime datetime, datetime_off;
    QDate _date_on, _date_off;
    QTime _time_on, _time_off;
    QStringList adifLine;
    adifLine.clear();
    QString line = QString(msg);


    adifLine << line.split('<');
    //qDebug() << "UDPServer::adifParse: before the while"  <<  QT_ENDL;
    for (int i = 0; i < adifLine.length() ;i++)
    {
        //qDebug() << "UDPServer::adifParse: While-1"  <<  QT_ENDL;
        QString type, data;
        QStringList field;
        field.clear();
        //qDebug() << "UDPServer::adifParse: While-2"  <<  QT_ENDL;
        QString aux = "<" + adifLine.at(i);
        //qDebug() << "UDPServer::adifParse: While-3"  <<  QT_ENDL;
        field.clear();
        field << util->getValidADIFFieldAndData(aux);
        if (field.length()==2)
        {
            //qDebug() << "UDPServer::adifParse: While-4"  <<  QT_ENDL;
            type = (field.at(0)).toUpper();
            //qDebug() << "UDPServer::adifParse: While-5"  <<  QT_ENDL;
            data = field.at(1);
            //qDebug() << "UDPServer::adifParse: While-6"  <<  QT_ENDL;
            if (type == "CALL")
            {
               dx_call = data;
            }
            else if (type == "GRIDSQUARE")
            {
                //qDebug() << "UDPServer::adifParse: Rec Grid: " << data <<  QT_ENDL;
                if (util->isValidGrid(data))
                {
                    dxgrid = data;
                    //qDebug() << "UDPServer::adifParse: Rec Grid to export: " << dxgrid <<  QT_ENDL;
                }
                else {
                    //qDebug() << "UDPServer::adifParse: INVALID GRID: " << dxgrid <<  QT_ENDL;
                }
            }
            else if (type == "MY_GRIDSQUARE")
            {
                //qDebug() << "UDPServer::adifParse: Rec mGrid: " << data <<  QT_ENDL;
                if (util->isValidGrid(data))
                {
                    mygrid = data;
                    //qDebug() << "UDPServer::adifParse: Rec mGrid to export: " << mygrid <<  QT_ENDL;
                }
                else
                {
                    //qDebug() << "UDPServer::adifParse: INVALID mGRID: " << mygrid <<  QT_ENDL;
                }
            }
            else if (type == "MODE")
            {
               mode = data;
            }
            else if (type == "RST_SENT")
            {
               rstRX = data;
            }
            else if (type == "RST_RCVD")
            {
               rstTX = data;
            }
            else if (type == "QSO_DATE")
            {
               _date_on = util->getDateFromADIFDateString(data);
            }
            else if (type == "TIME_ON")
            {
              _time_on  = util->getTimeFromADIFTimeString(data);
            }
            else if (type == "QSO_DATE_OFF")
            {
              _date_off  = util->getDateFromADIFDateString(data);
            }
            else if (type == "TIME_OFF")
            {
              _time_off = util->getTimeFromADIFTimeString(data);
            }
            else if (type == "COMMENT")
            {
              _comment = data;
            }
            else if (type == "BAND")
            {
              band  = data;
            }
            else if (type == "OPERATOR")
            {
              operatorCall  = data;
            }
            else if (type == "FREQ")
            {
              freq = data.toDouble();
            }
            else if (type == "STATION_CALLSIGN")
            {
              stationcallsign = data;
            }
            else if (type == "NAME")
            {
              stationcallsign = data;
            }
            else if (type == "EOR")
            {
                if (_date_on.isValid() && _time_on.isValid())
                {
                    datetime.setDate(_date_on);
                    datetime.setTime(_time_on);
                }
                else
                {
                    datetime = QDateTime();
                }
                if (_date_off.isValid() && _time_off.isValid())
                {
                    datetime_off.setDate(_date_off);
                    datetime_off.setTime(_time_off);
                }
                else
                {
                    datetime_off = QDateTime();
                }
                //qDebug() << "UDPServer::adifParse: Emitting"  <<  QT_ENDL;
                if ((freq < 0.1) && (band.length ()<2))
                {
                    return;
                }
                emit logged_qso (dx_call, mode, band, freq,
                                 mygrid, dxgrid, rstTX, rstRX, _comment, stationcallsign, _name,
                                 operatorCall, datetime, datetime_off, _exchangeTX, _exchangeRX, _myPWR);
                return;
            }
            else
            {
                //qDebug() << "UDPServer::adifParse: NON captured ADIF type: " << type <<  QT_ENDL;
                //qDebug() << "UDPServer::adifParse: NON captured ADIF data: " << data <<  QT_ENDL;
            }
        }
        else
        {
        //qDebug() << "UDPServer::adifParse: Not a valid ADIF pair " << aux <<  QT_ENDL;
        }
    }
    //qDebug() << "UDPServer::adifParse: - END" <<  QT_ENDL;
}

void UDPServer::loadSettings()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("UDPServer");

    setNetworkInterface (settings.value("UDPNetworkInterface").toString ());
    setPort (settings.value("UDPServerPort").toInt ());
    logging = settings.value("LogFromWSJTX").toBool ();
    realtime = settings.value ("RealTimeFromWSJTX", false).toBool ();
    settings.endGroup ();

    /*
    UDPServerStart = settings.value ("UDPServer", false).toBool ();

    UDPLogServer->setRealTimeUpdate();

    infoTimeout = settings.value ("InfoTimeOut", 2000).toInt ();
    wsjtxAutoLog = settings.value ("LogAutoFromWSJTX", false).toBool ();
*/
    //qDebug() << Q_FUNC_INFO << " - END";
}
