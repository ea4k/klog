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
       //qDebug() << "UDPServer::UDPServer"  << QT_ENDL;
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
            //qDebug() << "UDPServer::UDPServer - Multicast group joined OK"  << QT_ENDL;
       }
       else
       {
            //qDebug() << "UDPServer::UDPServer - Multicast group joined NOK"  << QT_ENDL;
       }
       */
        util = new Utilities;
        logging = false;
        realtime = false;

        connect(socketServer,SIGNAL(readyRead()),this,SLOT(slotReadPendingDatagrams()));
}

void UDPServer::slotReadPendingDatagrams()
{
       //qDebug() << "UDPServer::slotReadPendingDatagrams"  << QT_ENDL;
    while (socketServer->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketServer->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
           //qDebug() << "UDPServer::slotReadPendingDatagrams: length = " << QString::number(socketServer->pendingDatagramSize()) << QT_ENDL;
        socketServer->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        parse (datagram);
        //qDebug() << "UDPServer::slotReadPendingDatagrams: = " << datagram << QT_ENDL;
    }

}

bool UDPServer::start()
{
    //qDebug() << "UDPServer::start " << QT_ENDL;
    if ( (port>0) && (port<65535) )
    {
        //qDebug() << "UDPServer::start: calling startNow " << QT_ENDL;
        return startNow(port, groupAddress);

    }
    else
    {
        //qDebug() << "UDPServer::start FALSE" << QT_ENDL;
        return false;
    }
}

bool UDPServer::startNow(quint16 _port, QHostAddress const& _multicast_group_address)
{
    //qDebug() << "UDPServer::startNow "<< QT_ENDL;
    //if ((_port != port) || (_multicast_group_address != groupAddress))
    if (1)
    {
        //qDebug() << "UDPServer::startNow starting..."<< QT_ENDL;
        leaveMultiCastGroup();
        if (socketServer->state() == QAbstractSocket::BoundState)
        {
            //qDebug() << "UDPServer::startNow: closing socket"<< QT_ENDL;
            socketServer->close();
        }
        groupAddress = _multicast_group_address;
        auto address = _multicast_group_address.isNull ()
                || QAbstractSocket::IPv4Protocol != groupAddress.protocol () ? QHostAddress::Any : QHostAddress::AnyIPv4;
        if (_port && socketServer->bind(groupAddress, _port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint))
        {
            port = _port;
            joinMultiCastGroup();

        }
        else
        {
            //qDebug() << "UDPServer::startNow port = 0"<< QT_ENDL;
            port = 0;
        }
    }
    else
    {
       //qDebug() << "UDPServer::startNow exiting... "<< QT_ENDL;
    }
    //qDebug() << "UDPServer::startNow exiting... "<< QT_ENDL;
    return  socketServer->isValid();

}

void UDPServer::joinMultiCastGroup()
{
    //qDebug() << "UDPServer::joinMultiCastGroup: "<< QT_ENDL;
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
               //qDebug() << "UDPServer::joinMultiCastGroup socket valid"<< QT_ENDL;
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
    //qDebug() << "UDPServer::joinMultiCastGroup - END"<< QT_ENDL;
}

void UDPServer::leaveMultiCastGroup()
{
    //qDebug() << "UDPServer::leaveMultiCastGroup"<< QT_ENDL;
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
    //qDebug() << "UDPServer::leaveMultiCastGroup - END"<< QT_ENDL;
}

 bool UDPServer::isStarted()
 {
     return  socketServer->isValid();
 }

void UDPServer::parse(const QByteArray &msg)
{
    //qDebug() << "UDPServer::parse: " << msg << QT_ENDL;
    //qDebug() << "UDPServer::parse: " << QString::fromStdString(msg.toStdString()) << QT_ENDL;
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


    //QByteArray msgOut;

    QDataStream in(msg);
    //QDataStream out(msgOut, QIODevice::ReadWrite);
    in.setVersion(16);
    in.setByteOrder(QDataStream::BigEndian);


    in >> magic >> schema >> type >> id;
    //QByteArray ba4(QByteArray::fromRawData(cart, 6));
    //in.readRawData(type, size)
       //qDebug() << "UDPServer::parse: -  Magic = " << QString::number(magic)<< QT_ENDL;
       //qDebug() << "UDPServer::parse: - schema = " << QString::number(schema)<< QT_ENDL;
       //qDebug() << "UDPServer::parse: -   type = " << QString::number(type)<< QT_ENDL;
       //qDebug() << "UDPServer::parse: -   id = " << id << QT_ENDL;


    //if ((magic != 2914831322) || (id != "WSJT-X"))
    if (magic != 2914831322)
    {
        //qDebug() << "UDPServer::parse: - Magic BAD FORMAT = " << QString::number(magic)<< QT_ENDL;
        return;
    }

    //qDebug() << "UDPServer::parse: TYPE: " << QString::number(type)<< QT_ENDL;
    //QDateTime dateTime, dateTimeOff;
    //QString line;
    switch (type)
    {
        case Heartbeat:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT/IN - Heartbeat" << QT_ENDL;
        break;
        case Status:
            //qDebug() << "UDPServer::parse: -   type" << QString::number(type) << " - OUT - Status" << QT_ENDL;
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
               //     //qDebug() << "UDPServer::parse: Status : Call not valid: -" << dx_call << "-" << QT_ENDL;
               //     return;
               //}
               //qDebug() << "UDPServer::parse: Status - Freq double = " << QString::number(frequencyDouble)  << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - Mode = " << mode << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - DXCall = " << dx_call << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - Report = " << report << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - Rx DF = " << rx_df << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - Tx DF = " << tx_df << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - De_Call = " << de_call << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - De_Grid = " << de_grid << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - SubMode = " << sub_mode << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - SpecialOp = " << QString::number(specialOpMode) << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - Freq Tol = " << QString::number(freqTolerance) << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - T/R Period = " << QString::number(TRPeriod) << QT_ENDL;
               //qDebug() << "UDPServer::parse: Status - Conf Name = " << confName << QT_ENDL;

                emit status_update (type, dx_call, frequencyDouble, mode, report, de_call, de_grid, dx_grid, sub_mode);
            }
            else
            {
                //qDebug() << "UDPServer::parse: realtime = FALSE" << QT_ENDL;
            }
        break;
        case Decode:
            //() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Decode" << QT_ENDL;
            in >> newDecode >> time >> snr >> deltaTime >> drift >> mode >> message >> lowConfidence >> offAir;


            if (lowConfidence)
            {
                //qDebug() << "UDPServer::parse: Decode -   Low Confidence" << QT_ENDL;
            }
            else
            {
                //qDebug() << "UDPServer::parse: Decode -   Time = " << time.toString("hhmmss") << QT_ENDL;
                //qDebug() << "UDPServer::parse: Decode -   SNR = " << QString::number(snr) << QT_ENDL;
                //qDebug() << "UDPServer::parse: Decode -   Delta = " << QString::number(deltaTime) << QT_ENDL;
                //qDebug() << "UDPServer::parse: Decode -   Drift = " << QString::number(drift) << QT_ENDL;
                //qDebug() << "UDPServer::parse: Decode -   Mode = " << mode << QT_ENDL;
                //qDebug() << "UDPServer::parse: Decode -   Msg = " << message << QT_ENDL;
            }

        break;
        case Clear:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Clear" << QT_ENDL;
            emit clearSignal();
        break;
        case Reply:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay " << QT_ENDL;
        break;
        case QSOLogged:
            //qDebug() << "UDPServer::parse: -   type = QSOLogged "  << QT_ENDL;


        in >> time_off >> dx_call >> dx_grid >> frequency >> mode >> report_sent >> report_received >>
                    tx_power >> comments >> name >> time_on >> operatorCall >> de_call >> de_grid >>
                    exchange_sent >> exchange_received;
            //qDebug() << "UDPServer::parse: QSOLogged -   Time_off = " << time_off.toString("yyyyMMdd-hhmmss") << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   DXCall = " << dx_call << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Grid = " << dx_grid << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Freq = " << QString::number(frequency) << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Mode = " << mode << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   ReportSent = " << report_sent << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   ReportReceived = " << report_received << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   TX_PWR = " << tx_power << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Comments = " << comments << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Name = " << name << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Time = " << time_on.toString("yyyyMMdd-hhmmss") << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   DeCall = " << de_call << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   DeGrid = " << de_grid << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Exch Sent = " << exchange_sent << QT_ENDL;
            //qDebug() << "UDPServer::parse: QSOLogged -   Exch Recv = " << exchange_received << QT_ENDL;

            if (logging)
            {
                //qDebug() << "UDPServer::parse: logging = true " << QT_ENDL;


                //qDebug() << "UDPServer::parse: QSO to be logged: Time_on: " << time_on << QT_ENDL;
                //qDebug() << "UDPServer::parse: QSO to be logged: Time_off: " << time_off << QT_ENDL;
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz

                //qDebug() << "UDPServer::parse: Data to be logged: Comment: " << comments << QT_ENDL;

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
                //qDebug() << "UDPServer::parse: logging = false" << QT_ENDL;
            }

        break;
        case Close:
            //qDebug() << "UDPServer::parse: -   type = Close"  << QT_ENDL;
            socketServer->close();
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Close " << QT_ENDL;
        break;
        case Replay:
            //qDebug() << "UDPServer::parse: -   type = Reply"  << QT_ENDL;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay" << QT_ENDL;
        break;
        case HaltTx:
            //qDebug() << "UDPServer::parse: -   type = HaltTx"  << QT_ENDL;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Halt TX" << QT_ENDL;
        break;
        case FreeText:
            //qDebug() << "UDPServer::parse: -   type = FreeText"  << QT_ENDL;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Free Text" << QT_ENDL;
        break;
        case WSPRDecode:
            //qDebug() << "UDPServer::parse: -   type = WSPRDecode"  << QT_ENDL;
            in >> newDecode >> time >> snr >> deltaTime >> frequency >> drift >> decodedCall >> decodedGrid >> power >> offAir;

            //qDebug() << "UDPServer::parse: WSPRDecode -   Time = " << time.toString("yyyyMMdd-hhmmss") << QT_ENDL;
            //qDebug() << "UDPServer::parse: WSPRDecode -   SNR = " << QString::number(snr) << QT_ENDL;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Delta = " << QString::number(deltaTime) << QT_ENDL;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Frec = " << QString::number(frequency) << QT_ENDL;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Drift = " << QString::number(drift) << QT_ENDL;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Call = " << decodedCall << QT_ENDL;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Grid = " << decodedGrid << QT_ENDL;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Power = " << QString::number(power) << QT_ENDL;

        break;
        case Location:
            //qDebug() << "UDPServer::parse: -   type = Location"  << QT_ENDL;
        break;
        case LoggedADIF:
           //qDebug() << "UDPServer::parse: -   type = LoggedADIF"  << QT_ENDL;
           //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ADIF" << QT_ENDL;
           //in >> adifReceived;
           //adifParse(adifReceived);
            //qDebug() << "UDPServer::parse: ADIF: " << adifReceived << QT_ENDL;
           // Q_SIGNAL void logged_ADIF (QString const& id, QByteArray const& ADIF);
        break;
        case HighlightCallsign:
            //qDebug() << "UDPServer::parse: -   type = HighlightCallsign"  << QT_ENDL;
        break;
        case SwitchConfiguration:
            //qDebug() << "UDPServer::parse: -   type = SwitchConfiguration"  << QT_ENDL;
        break;
        case Configure:
            //qDebug() << "UDPServer::parse: -   type = Configure"  << QT_ENDL;
        break;
        default: //NO
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ERROR on Type" << QT_ENDL;
        break;

    }



       //qDebug() << "UDPServer::parse: - Magic: = " << QString::number(magic)<< QT_ENDL;

}


bool UDPServer::stop()
{
    //qDebug() << "UDPServer::stop"<< QT_ENDL;
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
       //qDebug() << "UDPServer::setPort: " << QString::number(_port) << QT_ENDL;
    if ((_port >= 0) && (_port<=65535))
    {
        port = _port;
    }
}

void UDPServer::setNetworkInterface(const QString &_t)
{
    //qDebug() << "UDPServer::setNetworkInterface: " << _t << QT_ENDL;
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
            //qDebug() << "UDPServer::setNetworkInterface: FOUND! " << testInterface  << QT_ENDL;
            if ((i.flags().testFlag(QNetworkInterface::IsUp)) )
            {
                networkInterface = i;
                haveNetworkInterface = true;
            }

        }
    }
}

void UDPServer::setLogging(const bool _t)
{
       //qDebug() << "UDPServer::setLogging: " <<   endl;
    if (_t)
    {
           //qDebug() << "UDPServer::setLogging: TRUE " <<   endl;
    }
    else
    {
           //qDebug() << "UDPServer::setLogging: FALSE" <<   endl;
    }
    logging = _t;
}


void UDPServer::setRealTimeUpdate(const bool _t)
{
       //qDebug() << "UDPServer::setRealTimeUpdate: " <<   endl;
    if (_t)
    {
           //qDebug() << "UDPServer::setRealTimeUpdate: TRUE " <<   endl;
    }
    else
    {
           //qDebug() << "UDPServer::setRealTimeUpdate: FALSE" <<   endl;
    }
       realtime = _t;
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
