#include "udpserver.h"
#include <QDateTime>
//https://sourceforge.net/p/wsjt/wsjtx/ci/master/tree/UDPExamples/MessageServer.cpp
// Read Network\NetworkMessage.hpp in the WSJT-x sources wsjtx

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
       //qDebug() << "UDPServer::UDPServer"  << endl;
       //address = QString("127.0.0.1");
       port = 2237;
       socketServer = new QUdpSocket(this);
       groupAddress = QHostAddress::Any;
       /*
       //if (socketServer->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress))
       if (socketServer->bind(port, QAbstractSocket::ShareAddress))
       {
            //qDebug() << "UDPServer::UDPServer - Multicast group joined OK"  << endl;
       }
       else
       {
            //qDebug() << "UDPServer::UDPServer - Multicast group joined NOK"  << endl;
       }
       */
        util = new Utilities;
       logging = false;
       realtime = false;

       connect(socketServer,SIGNAL(readyRead()),this,SLOT(slotReadPendingDatagrams()));
}

void UDPServer::slotReadPendingDatagrams()
{
       //qDebug() << "UDPServer::slotReadPendingDatagrams"  << endl;
    while (socketServer->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socketServer->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
           //qDebug() << "UDPServer::slotReadPendingDatagrams: length = " << QString::number(socketServer->pendingDatagramSize()) << endl;
        socketServer->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        parse (datagram);
        //qDebug() << "UDPServer::slotReadPendingDatagrams: = " << datagram << endl;
    }

}

bool UDPServer::start()
{
    //qDebug() << "UDPServer::start " << endl;
    if ( (port>0) && (port<65535) )
    {
        //qDebug() << "UDPServer::start: calling startNow " << endl;
        return startNow(port, groupAddress);

    }
    else
    {
        //qDebug() << "UDPServer::start FALSE" << endl;
        return false;
    }
}

bool UDPServer::startNow(quint16 _port, QHostAddress const& _multicast_group_address)
{
    //qDebug() << "UDPServer::startNow "<< endl;
    //if ((_port != port) || (_multicast_group_address != groupAddress))
    if (1)
    {
        //qDebug() << "UDPServer::startNow starting..."<< endl;
        leaveMultiCastGroup();
        if (socketServer->state() == QAbstractSocket::BoundState)
        {
            //qDebug() << "UDPServer::startNow: closing socket"<< endl;
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
            //qDebug() << "UDPServer::startNow port = 0"<< endl;
            port = 0;
        }


    }
    else
    {
       //qDebug() << "UDPServer::startNow exiting... "<< endl;
    }
    //qDebug() << "UDPServer::startNow exiting... "<< endl;
    return  socketServer->isValid();

}

void UDPServer::joinMultiCastGroup()
{
    //qDebug() << "UDPServer::joinMultiCastGroup: "<< endl;
    if ((socketServer->state() == QAbstractSocket::BoundState) && (!groupAddress.isNull())  && groupAddress.isMulticast()   )
    {
        auto mcast_interface = socketServer->multicastInterface();
        if (( groupAddress.protocol() ==  QAbstractSocket::IPv4Protocol ) && (socketServer->localAddress().protocol() != QAbstractSocket::IPv4Protocol ))
        {
            socketServer->close();
            socketServer->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
            if (socketServer->isValid())
            {
               //qDebug() << "UDPServer::joinMultiCastGroup socket valid"<< endl;
            }
        }
        bool joined {false};


        QList<QNetworkInterface> interfaces;
        interfaces.clear();
        interfaces.append(QNetworkInterface::allInterfaces());

        for (int i = 0; i < interfaces.size(); ++i)
        {
            socketServer->setMulticastInterface(interfaces.at(i));
            joined |= socketServer->joinMulticastGroup(groupAddress, interfaces.at(i));
        }
        if (!joined)
        {
            groupAddress.clear();
        }
        socketServer->setMulticastInterface(mcast_interface);
    }
    //qDebug() << "UDPServer::joinMultiCastGroup - END"<< endl;
}

void UDPServer::leaveMultiCastGroup()
{
    //qDebug() << "UDPServer::leaveMultiCastGroup"<< endl;
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
    //qDebug() << "UDPServer::leaveMultiCastGroup - END"<< endl;
}

 bool UDPServer::isStarted()
 {
     return  socketServer->isValid();
 }

void UDPServer::parse(const QByteArray &msg)
{
    //qDebug() << "UDPServer::parse: " << msg << endl;
    //qDebug() << "UDPServer::parse: " << QString::fromStdString(msg.toStdString()) << endl;
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
       //qDebug() << "UDPServer::parse: -  Magic = " << QString::number(magic)<< endl;
       //qDebug() << "UDPServer::parse: - schema = " << QString::number(schema)<< endl;
       //qDebug() << "UDPServer::parse: -   type = " << QString::number(type)<< endl;
       //qDebug() << "UDPServer::parse: -   id = " << id << endl;


    if ((magic != 2914831322) || (id != "WSJT-X"))
    {
        //qDebug() << "UDPServer::parse: - Magic BAD FORMAT = " << QString::number(magic)<< endl;
        return;
    }

    //qDebug() << "UDPServer::parse: TYPE: " << QString::number(type)<< endl;
    //QDateTime dateTime, dateTimeOff;
    //QString line;
    switch (type)
    {
        case Heartbeat:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT/IN - Heartbeat" << endl;
        break;
        case Status:
            //qDebug() << "UDPServer::parse: -   type" << QString::number(type) << " - OUT - Status" << endl;
            // unpack message
            //in >> ch;


            if (realtime)
            {
                in >> frequency >> mode >> dx_call >> report >> tx_mode >> tx_enabled >> transmitting >> decoding
                   >> rx_df >> tx_df >> de_call >> de_grid >> dx_grid >> watchdog_timeout >> sub_mode
                   >> fast_mode >> specialOpMode >> freqTolerance >> TRPeriod >> confName;
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz

               //qDebug() << "UDPServer::parse: Status - Freq double = " << QString::number(frequencyDouble)  << endl;
               //qDebug() << "UDPServer::parse: Status - Mode = " << mode << endl;
               //qDebug() << "UDPServer::parse: Status - DXCall = " << dx_call << endl;
               //qDebug() << "UDPServer::parse: Status - Report = " << report << endl;
               //qDebug() << "UDPServer::parse: Status - Rx DF = " << rx_df << endl;
               //qDebug() << "UDPServer::parse: Status - Tx DF = " << tx_df << endl;
               //qDebug() << "UDPServer::parse: Status - De_Call = " << de_call << endl;
               //qDebug() << "UDPServer::parse: Status - De_Grid = " << de_grid << endl;
               //qDebug() << "UDPServer::parse: Status - SubMode = " << sub_mode << endl;
               //qDebug() << "UDPServer::parse: Status - SpecialOp = " << QString::number(specialOpMode) << endl;
               //qDebug() << "UDPServer::parse: Status - Freq Tol = " << QString::number(freqTolerance) << endl;
               //qDebug() << "UDPServer::parse: Status - T/R Period = " << QString::number(TRPeriod) << endl;
               //qDebug() << "UDPServer::parse: Status - Conf Name = " << confName << endl;

                emit status_update (type, dx_call, frequencyDouble, mode, report, de_call, de_grid, dx_grid, sub_mode);
            }
            else
            {
                //qDebug() << "UDPServer::parse: realtime = FALSE" << endl;
            }
        break;
        case Decode:
            //() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Decode" << endl;
            in >> newDecode >> time >> snr >> deltaTime >> drift >> mode >> message >> lowConfidence >> offAir;


            if (lowConfidence)
            {
                //qDebug() << "UDPServer::parse: Decode -   Low Confidence" << endl;
            }
            else
            {
                //qDebug() << "UDPServer::parse: Decode -   Time = " << time.toString("hhmmss") << endl;
                //qDebug() << "UDPServer::parse: Decode -   SNR = " << QString::number(snr) << endl;
                //qDebug() << "UDPServer::parse: Decode -   Delta = " << QString::number(deltaTime) << endl;
                //qDebug() << "UDPServer::parse: Decode -   Drift = " << QString::number(drift) << endl;
                //qDebug() << "UDPServer::parse: Decode -   Mode = " << mode << endl;
                //qDebug() << "UDPServer::parse: Decode -   Msg = " << message << endl;
            }

        break;
        case Clear:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Clear" << endl;
            //emit clearSignal();
        break;
        case Reply:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay " << endl;
        break;
        case QSOLogged:
            //qDebug() << "UDPServer::parse: -   type = QSOLogged "  << endl;


        in >> time_off >> dx_call >> dx_grid >> frequency >> mode >> report_sent >> report_received >>
                    tx_power >> comments >> name >> time_on >> operatorCall >> de_call >> de_grid >>
                    exchange_sent >> exchange_received;
            //qDebug() << "UDPServer::parse: QSOLogged -   Time_off = " << time_off.toString("yyyyMMdd-hhmmss") << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   DXCall = " << dx_call << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Grid = " << dx_grid << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Freq = " << QString::number(frequency) << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Mode = " << mode << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   ReportSent = " << report_sent << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   ReportReceived = " << report_received << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   TX_PWR = " << tx_power << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Comments = " << comments << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Name = " << name << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Time = " << time_on.toString("yyyyMMdd-hhmmss") << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   DeCall = " << de_call << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   DeGrid = " << de_grid << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Exch Sent = " << exchange_sent << endl;
            //qDebug() << "UDPServer::parse: QSOLogged -   Exch Recv = " << exchange_received << endl;

            if (logging)
            {
                //qDebug() << "UDPServer::parse: logging = true " << endl;


                //qDebug() << "UDPServer::parse: QSO to be logged: Time_on: " << time_on << endl;
                //qDebug() << "UDPServer::parse: QSO to be logged: Time_off: " << time_off << endl;                
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz

                //qDebug() << "UDPServer::parse: Data to be logged: Comment: " << comments << endl;

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
                //qDebug() << "UDPServer::parse: logging = false" << endl;
            }

        break;
        case Close:
            //qDebug() << "UDPServer::parse: -   type = Close"  << endl;
            socketServer->close();
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Close " << endl;
        break;
        case Replay:
            //qDebug() << "UDPServer::parse: -   type = Reply"  << endl;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay" << endl;
        break;
        case HaltTx:
            //qDebug() << "UDPServer::parse: -   type = HaltTx"  << endl;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Halt TX" << endl;
        break;
        case FreeText:
            //qDebug() << "UDPServer::parse: -   type = FreeText"  << endl;
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Free Text" << endl;
        break;
        case WSPRDecode:
            //qDebug() << "UDPServer::parse: -   type = WSPRDecode"  << endl;
            in >> newDecode >> time >> snr >> deltaTime >> frequency >> drift >> decodedCall >> decodedGrid >> power >> offAir;

            //qDebug() << "UDPServer::parse: WSPRDecode -   Time = " << time.toString("yyyyMMdd-hhmmss") << endl;
            //qDebug() << "UDPServer::parse: WSPRDecode -   SNR = " << QString::number(snr) << endl;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Delta = " << QString::number(deltaTime) << endl;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Frec = " << QString::number(frequency) << endl;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Drift = " << QString::number(drift) << endl;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Call = " << decodedCall << endl;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Grid = " << decodedGrid << endl;
            //qDebug() << "UDPServer::parse: WSPRDecode -   Power = " << QString::number(power) << endl;

        break;
        case Location:
            //qDebug() << "UDPServer::parse: -   type = Location"  << endl;
        break;
        case LoggedADIF:
            //qDebug() << "UDPServer::parse: -   type = LoggedADIF"  << endl;
           //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ADIF" << endl;
           in >> adifReceived;
           adifParse(adifReceived);
            //qDebug() << "UDPServer::parse: ADIF: " << adifReceived << endl;
           // Q_SIGNAL void logged_ADIF (QString const& id, QByteArray const& ADIF);
        break;
        case HighlightCallsign:
            //qDebug() << "UDPServer::parse: -   type = HighlightCallsign"  << endl;
        break;
        case SwitchConfiguration:
            //qDebug() << "UDPServer::parse: -   type = SwitchConfiguration"  << endl;
        break;
        case Configure:
            //qDebug() << "UDPServer::parse: -   type = Configure"  << endl;
        break;
        default: //NO
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ERROR on Type" << endl;
        break;

    }



       //qDebug() << "UDPServer::parse: - Magic: = " << QString::number(magic)<< endl;

}


bool UDPServer::stop()
{
    //qDebug() << "UDPServer::stop"<< endl;
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
       //qDebug() << "UDPServer::setPort: " << QString::number(_port) << endl;
    if ((_port >= 0) && (_port<=65535))
    {
        port = _port;
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
    //qDebug() << "UDPServer::adifParse: " << msg <<  endl;


    QString dx_call = QString();
    QString mode = QString();
    QString band = QString();
    QString mygrid = QString();
    QString dxgrid = QString();
    QString rstTX = QString();
    QString rstRX = QString();
    QString _comment = QString();
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
    //qDebug() << "UDPServer::adifParse: before the while"  <<  endl;
    for (int i = 0; i < adifLine.length() ;i++)
    {
        //qDebug() << "UDPServer::adifParse: While-1"  <<  endl;
        QString type, data;
        QStringList field;
        field.clear();
        //qDebug() << "UDPServer::adifParse: While-2"  <<  endl;
        QString aux = "<" + adifLine.at(i);
        //qDebug() << "UDPServer::adifParse: While-3"  <<  endl;
        field.clear();
        field << util->getValidADIFFieldAndData(aux);
        if (field.length()==2)
        {
            //qDebug() << "UDPServer::adifParse: While-4"  <<  endl;
            type = (field.at(0)).toUpper();
            //qDebug() << "UDPServer::adifParse: While-5"  <<  endl;
            data = field.at(1);
            //qDebug() << "UDPServer::adifParse: While-6"  <<  endl;
            if (type == "CALL")
            {
               dx_call = data;
            }
            else if (type == "GRIDSQUARE")
            {
                //qDebug() << "UDPServer::adifParse: Rec Grid: " << data <<  endl;
                if (util->isValidGrid(data))
                {
                    dxgrid = data;
                    //qDebug() << "UDPServer::adifParse: Rec Grid to export: " << dxgrid <<  endl;
                }
                else {
                    //qDebug() << "UDPServer::adifParse: INVALID GRID: " << dxgrid <<  endl;
                }
            }
            else if (type == "MY_GRIDSQUARE")
            {
                //qDebug() << "UDPServer::adifParse: Rec mGrid: " << data <<  endl;
                if (util->isValidGrid(data))
                {
                    mygrid = data;
                    //qDebug() << "UDPServer::adifParse: Rec mGrid to export: " << mygrid <<  endl;
                }
                else {
                    //qDebug() << "UDPServer::adifParse: INVALID mGRID: " << mygrid <<  endl;
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
                //qDebug() << "UDPServer::adifParse: Emitting"  <<  endl;
                /*
                emit logged_qso (dx_call, mode, band, freq,
                                 mygrid, dxgrid, rstTX, rstRX, _comment, stationcallsign,
                                 operatorCall, datetime, datetime_off);
                */
                return;
            }
            else
            {
                //qDebug() << "UDPServer::adifParse: NON captured ADIF type: " << type <<  endl;
                //qDebug() << "UDPServer::adifParse: NON captured ADIF data: " << data <<  endl;
            }
        }
        else
        {
        //qDebug() << "UDPServer::adifParse: Not a valid ADIF pair " << aux <<  endl;
        }


    }

    //qDebug() << "UDPServer::adifParse: - END" <<  endl;
}
