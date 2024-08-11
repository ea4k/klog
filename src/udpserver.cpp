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
               //qDebug() << Q_FUNC_INFO << ": socket valid";
            }
        }
        bool joined {false};

        socketServer->setMulticastInterface(networkInterface);
        joined |= socketServer->joinMulticastGroup(groupAddress, networkInterface);

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
       //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type);
       //qDebug() << Q_FUNC_INFO << ": id = " << id;


    //if ((magic != 2914831322) || (id != "WSJT-X"))
    if (magic != 2914831322)
    {
        //qDebug() << Q_FUNC_INFO << ": Magic BAD FORMAT = " << QString::number(magic);
        return;
    }

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

            if (realtime)
            {
                in >> frequency >> mode >> dx_call >> report >> tx_mode >> tx_enabled >> transmitting >> decoding
                   >> rx_df >> tx_df >> de_call >> de_grid >> dx_grid >> watchdog_timeout >> sub_mode
                   >> fast_mode >> specialOpMode >> freqTolerance >> TRPeriod >> confName;
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz
               //if (!util->isValidCall(dx_call))
               // {
               //     //qDebug() << Q_FUNC_INFO << ": Status : Call not valid: -" << dx_call << "-";
               //     return;
               //}
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
            //qDebug() << Q_FUNC_INFO << ": type = " << QString::number(type) << " - OUT - Decode";
            in >> newDecode >> time >> snr >> deltaTime >> drift >> mode >> message >> lowConfidence >> offAir;


            if (lowConfidence)
            {
                //qDebug() << Q_FUNC_INFO << ": Decode:    Low Confidence";
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": Decode:    Time = " << time.toString("hhmmss");
                //qDebug() << Q_FUNC_INFO << ": Decode:    SNR = " << QString::number(snr);
                //qDebug() << Q_FUNC_INFO << ": Decode:    Delta = " << QString::number(deltaTime);
                //qDebug() << Q_FUNC_INFO << ": Decode:    Drift = " << QString::number(drift);
                //qDebug() << Q_FUNC_INFO << ": Decode:    Mode = " << mode;
                //qDebug() << Q_FUNC_INFO << ": Decode:    Msg = " << message;
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
                emit logged_qso(dx_call, mode, QString(), frequencyDouble,
                                de_grid, dx_grid, report_sent, report_received,
                                comments, de_call, name, operatorCall, time_on, time_off,
                                exchange_sent, exchange_received, tx_power);
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


void UDPServer::adifParse(QByteArray &msg)
{
    qDebug() << Q_FUNC_INFO << " - Start: " << msg;

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

    QSO qso;

    QStringList adifLine;
    adifLine.clear();
    QString line = QString(msg);


    adifLine << line.split('<');
    //qDebug() << Q_FUNC_INFO << ": before the while"  <<  QT_ENDL;
    for (int i = 0; i < adifLine.length() ;i++)
    {
        //qDebug() << Q_FUNC_INFO << ": While-1"  <<  QT_ENDL;
        QString type, data;
        QStringList field;
        field.clear();
        //qDebug() << Q_FUNC_INFO << ": While-2"  <<  QT_ENDL;
        QString aux = "<" + adifLine.at(i);
        //qDebug() << Q_FUNC_INFO << ": While-3"  <<  QT_ENDL;
        field.clear();
        field << util->getValidADIFFieldAndData(aux);
        if (field.length()==2)
        {
            //qDebug() << Q_FUNC_INFO << ": While-4"  <<  QT_ENDL;
            type = (field.at(0)).toUpper();
            //qDebug() << Q_FUNC_INFO << ": While-5"  <<  QT_ENDL;
            data = field.at(1);
            //qDebug() << Q_FUNC_INFO << ": While-6"  <<  QT_ENDL;
            if (type == "CALL")
            {
               dx_call = data;
            }
            else if (type == "GRIDSQUARE")
            {
                //qDebug() << Q_FUNC_INFO << ": Rec Grid: " << data <<  QT_ENDL;
                if (util->isValidGrid(data))
                {
                    qso.setGridSquare(data);
                    dxgrid = data;
                    //qDebug() << Q_FUNC_INFO << ": Rec Grid to export: " << dxgrid <<  QT_ENDL;
                }
                else {
                    //qDebug() << Q_FUNC_INFO << ": INVALID GRID: " << dxgrid <<  QT_ENDL;
                }
            }
            else if (type == "MY_GRIDSQUARE")
            {
                //qDebug() << Q_FUNC_INFO << ": Rec mGrid: " << data <<  QT_ENDL;
                if (util->isValidGrid(data))
                {
                     qso.setMyGridSquare(data);
                    mygrid = data;
                    //qDebug() << Q_FUNC_INFO << ": Rec mGrid to export: " << mygrid <<  QT_ENDL;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO << ": INVALID mGRID: " << mygrid <<  QT_ENDL;
                }
            }
            else if (type == "MODE")
            {
                qso.setMode(data);
               mode = data;
            }
            else if (type == "RST_SENT")
            {
               qso.setRSTRX(data);
               rstRX = data;
            }
            else if (type == "RST_RCVD")
            {
               qso.setRSTTX(data);
               rstTX = data;
            }
            else if (type == "QSO_DATE")
            {
               qso.setDate(util->getDateFromADIFDateString(data));
               _date_on = util->getDateFromADIFDateString(data);
            }
            else if (type == "TIME_ON")
            {
               qso.setTimeOn(util->getTimeFromADIFTimeString(data));
              _time_on  = util->getTimeFromADIFTimeString(data);
            }
            else if (type == "QSO_DATE_OFF")
            {
              qso.setDateOff(util->getDateFromADIFDateString(data));
              _date_off  = util->getDateFromADIFDateString(data);
            }
            else if (type == "TIME_OFF")
            {
              qso.setTimeOff(util->getTimeFromADIFTimeString(data));
              _time_off = util->getTimeFromADIFTimeString(data);
            }
            else if (type == "COMMENT")
            {
              qso.setComment(data);
              _comment = data;
            }
            else if (type == "BAND")
            {
              qso.setBand(data);
              band  = data;
            }
            else if (type == "OPERATOR")
            {
              qso.setOperatorCallsign(data);
              operatorCall  = data;
            }
            else if (type == "FREQ")
            {
              qso.setFreq(data.toDouble());
              freq = data.toDouble();
            }
            else if (type == "STATION_CALLSIGN")
            {
              qso.setStationCallsign(data);
              stationcallsign = data;
            }
            else if (type == "NAME")
            {
              qso.setName(data);
              _name = data;
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
                //qDebug() << Q_FUNC_INFO << ": Emitting"  <<  QT_ENDL;
                if ((freq < 0.1) && (band.length ()<2))
                {
                    return;
                }
                emit logged(qso);
                emit logged_qso (dx_call, mode, band, freq,
                                 mygrid, dxgrid, rstTX, rstRX, _comment, stationcallsign, _name,
                                 operatorCall, datetime, datetime_off, _exchangeTX, _exchangeRX, _myPWR);
                return;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": NON captured ADIF type: " << type <<  QT_ENDL;
                //qDebug() << Q_FUNC_INFO << ": NON captured ADIF data: " << data <<  QT_ENDL;
            }
        }
        else
        {
        //qDebug() << Q_FUNC_INFO << ": Not a valid ADIF pair " << aux <<  QT_ENDL;
        }
    }
   //qDebug() << Q_FUNC_INFO << " - END";
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

   //qDebug() << Q_FUNC_INFO << " - END";
}
