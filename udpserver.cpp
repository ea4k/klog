#include "udpserver.h"
#include <QDateTime>
//https://sourceforge.net/p/wsjt/wsjtx/ci/master/tree/UDPExamples/MessageServer.cpp

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
       //qDebug() << "UDPServer::UDPServer"  << endl;
       //address = QString("127.0.0.1");
       port = 2237;
       socketServer = new QUdpSocket(this);
       //socketServer->bind(QHostAddress::AnyIPv4, port);
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
       //qDebug() << "UDPServer::start "<< endl;
    socketServer->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    groupAddress = QHostAddress("127.0.0.1");
    socketServer->joinMulticastGroup(groupAddress);
     return  socketServer->isValid();
   // return true;
    //return socketServer->bind(QHostAddress::AnyIPv4, port);
    //return socketServer->bind(port, QUdpSocket::ShareAddress);

}

 bool UDPServer::isStarted()
 {
     return  socketServer->isValid();
 }

void UDPServer::parse(const QByteArray &msg)
{
    //qDebug() << "UDPServer::parse: " << msg << endl;
    quint32 magic;
    quint32 schema;
    quint32 type;
    QByteArray id;
    QByteArray adifReceived;

    QByteArray time_off;
    QByteArray dx_call;
    QByteArray dx_grid;
    quint64 frequency = 0; //  In Hz??
    double frequencyDouble;

    QByteArray mode;
    QByteArray report_sent;
    QByteArray report_received;
    QByteArray tx_power;
    QByteArray comments;
    QByteArray name;
    QByteArray time_on; // Note: LOTW uses TIME_ON for their +/- 30-minute time

    QByteArray report;
    QByteArray tx_mode;
    bool tx_enabled = false;
    bool transmitting = false;
    bool decoding = false;
    qint32 rx_df = -1;  // Delta frequency
    qint32 tx_df = -1;  // Delta time
    QByteArray de_call;
    QByteArray de_grid;
    bool watchdog_timeout = false;
    QByteArray sub_mode;
    bool fast_mode = false;

    //QByteArray msgOut;

    QDataStream in(msg);
    //QDataStream out(msgOut, QIODevice::ReadWrite);


    in.setVersion(16);
    in.setByteOrder(QDataStream::BigEndian);

    {
       //qDebug() << "UDPServer::parse: - Magic GOOD FORMAT = " << QString::number(magic)<< endl;
    }
       //qDebug() << "UDPServer::parse Version = " << QString::number(in.version())<< endl;

    in >> magic >> schema >> type >> id;
    //QByteArray ba4(QByteArray::fromRawData(cart, 6));
    //in.readRawData(type, size)
       //qDebug() << "UDPServer::parse: -  Magic = " << QString::number(magic)<< endl;
       //qDebug() << "UDPServer::parse: - schema = " << QString::number(schema)<< endl;
       //qDebug() << "UDPServer::parse: -   type = " << QString::number(type)<< endl;


    if (magic != 2914831322)
    {
       //qDebug() << "UDPServer::parse: - Magic BAD FORMAT = " << QString::number(magic)<< endl;
        return;
    }

    //qDebug() << "UDPServer::parse: TYPE: " << QString::number(type)<< endl;

    switch (type)
    {
        case 0:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT/IN - Heartbeat" << endl;
        break;
        case 1:            
            //qDebug() << "UDPServer::parse: -   type" << QString::number(type) << " - OUT - Status" << endl;
            // unpack message
            //in >> ch;


            if (realtime)
            {
                in >> frequency >> mode >> dx_call >> report >> tx_mode >> tx_enabled >> transmitting >> decoding
                   >> rx_df >> tx_df >> de_call >> de_grid >> dx_grid >> watchdog_timeout >> sub_mode
                   >> fast_mode;
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz
               //qDebug() << "UDPServer::parse: -   Freq quint64 = " << QString::number(frequency)  << endl;
               //qDebug() << "UDPServer::parse: -   Freq double = " << QString::number(frequencyDouble)  << endl;

                emit status_update (type, dx_call, frequencyDouble, mode, report, de_call, de_grid, dx_grid, sub_mode);
            }
            else
            {
                //qDebug() << "UDPServer::parse: realtime = FALSE" << endl;
            }


        break;
        case 2:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Decode" << endl;
        break;
        case 3:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Clear" << endl;
        break;
        case 4:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay " << endl;
        break;
        case 5:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - QSO logged" << endl;
            if (logging)
            {
                //qDebug() << "UDPServer::parse: QSO to be logged: " << endl;

                in >> time_off >> dx_call >> dx_grid >> frequency >> mode >> report_sent >> report_received >> tx_power >> comments >> name >> time_on >> de_call >> de_grid;
                //qDebug() << "UDPServer::parse: QSO to be logged: Time_on: " << time_on << endl;
                //qDebug() << "UDPServer::parse: QSO to be logged: Time_off: " << time_off << endl;                
                frequencyDouble = (double)frequency;
                frequencyDouble = frequencyDouble/1000000; // Change to MHz


                /*qso_logged (id, time_off, QString::fromUtf8 (dx_call), QString::fromUtf8 (dx_grid)
                                                              , dial_frequency, QString::fromUtf8 (mode), QString::fromUtf8 (report_sent)
                                                              , QString::fromUtf8 (report_received), QString::fromUtf8 (tx_power)
                                                              , QString::fromUtf8 (comments), QString::fromUtf8 (name), time_on
                                                              , QString::fromUtf8 (operator_call), QString::fromUtf8 (my_call)
                                                              , QString::fromUtf8 (my_grid), QString::fromUtf8 (exchange_sent)
                                                              , QString::fromUtf8 (exchange_rcvd));
                */


            }
            else
            {
                //qDebug() << "UDPServer::parse: logging = FALSE" << endl;
            }


            //emit status_update (type, dx_call, frequency, mode, report, de_call, de_grid, dx_grid, sub_mode)

            //out << type;
            //emit status_update (out);

               //qDebug() << "UDPServer::parse: -   DXCall = " << dx_call << endl;
               //qDebug() << "UDPServer::parse: -   Grid = " << dx_grid << endl;
               //qDebug() << "UDPServer::parse: -   Freq = " << QString::number(frequency) << endl;
               //qDebug() << "UDPServer::parse: -   Mode = " << mode << endl;
               //qDebug() << "UDPServer::parse: -   ReportSent = " << report_sent << endl;
               //qDebug() << "UDPServer::parse: -   TX_PWR = " << tx_power << endl;
               //qDebug() << "UDPServer::parse: -   Comments = " << comments << endl;
               //qDebug() << "UDPServer::parse: -   Name = " << name << endl;
               //qDebug() << "UDPServer::parse: -   Time = " << time_on.toString("HHMMSSzzz") << endl;

        break;
        case 6:
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Close " << endl;
        break;
        case 7:
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay" << endl;
        break;
        case 8:
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Halt TX" << endl;
        break;
        case 9:
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Free Text" << endl;
        break;
        case 10:
               //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - WSPR Decode" << endl;
        break;
        case 12:
           //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ADIF" << endl;
           in >> adifReceived;
           adifParse(adifReceived);
            //qDebug() << "UDPServer::parse: ADIF: " << adifReceived << endl;
           // Q_SIGNAL void logged_ADIF (QString const& id, QByteArray const& ADIF);

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
    QString stationcallsign = QString(); ;
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

                emit logged_qso (dx_call, mode, band, freq,
                                 mygrid, dxgrid, rstTX, rstRX, _comment, stationcallsign,
                                 datetime, datetime_off);
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
