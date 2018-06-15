#include "udpserver.h"
#include <QDateTime>

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
    qDebug() << "UDPServer::UDPServer"  << endl;
       //address = QString("127.0.0.1");
       port = 2237;
       socketServer = new QUdpSocket(this);
       //socketServer->bind(QHostAddress::AnyIPv4, port);
       socketServer->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);

       groupAddress = QHostAddress("127.0.0.1");

       socketServer->joinMulticastGroup(groupAddress);


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
    qDebug() << "UDPServer::start "<< endl;
    return true;
    //return socketServer->bind(QHostAddress::AnyIPv4, port);
    //return socketServer->bind(port, QUdpSocket::ShareAddress);

}

void UDPServer::parse(const QByteArray &msg)
{
    //qDebug() << "UDPServer::parse"<< endl;
    quint32 magic;
    quint32 schema;
    quint32 type;
    QByteArray id;

    QDateTime time_off;
    QByteArray dx_call;
    QByteArray dx_grid;
    quint64 frequency;
    QByteArray mode;
    QByteArray report_sent;
    QByteArray report_received;
    QByteArray tx_power;
    QByteArray comments;
    QByteArray name;
    QDateTime time_on; // Note: LOTW uses TIME_ON for their +/- 30-minute time

    QByteArray report;
    QByteArray tx_mode;
    bool tx_enabled {false};
    bool transmitting {false};
    bool decoding {false};
    qint32 rx_df {-1};
    qint32 tx_df {-1};
    QByteArray de_call;
    QByteArray de_grid;
    bool watchdog_timeout {false};
    QByteArray sub_mode;
    bool fast_mode {false};

    QByteArray msgOut;

    QDataStream in(msg);
    //QDataStream out(msgOut, QIODevice::ReadWrite);


    in.setVersion(16);
    in.setByteOrder(QDataStream::BigEndian);

    //qDebug() << "UDPServer::parse Version = " << QString::number(in.version())<< endl;

    in >> magic >> schema >> type >> id;
    //QByteArray ba4(QByteArray::fromRawData(cart, 6));
    //in.readRawData(type, size)
    //qDebug() << "UDPServer::parse: -  Magic = " << QString::number(magic)<< endl;
    //qDebug() << "UDPServer::parse: - schema = " << QString::number(schema)<< endl;
    //qDebug() << "UDPServer::parse: -   type = " << QString::number(type)<< endl;
    //qDebug() << "UDPServer::parse: -   id = " <<  id.toUtf8() << endl;

    if (magic != 2914831322)
    {
    //qDebug() << "UDPServer::parse: - Magic BAD FORMAT = " << QString::number(magic)<< endl;
        return;
    }
    else
    {
    //qDebug() << "UDPServer::parse: - Magic GOOD FORMAT = " << QString::number(magic)<< endl;
    }

    switch (type)
    {
        case 0:
            //qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT/IN - Heartbeat" << endl;
        break;
        case 1:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Status" << endl;
            // unpack message


            in >> frequency >> mode >> dx_call >> report >> tx_mode >> tx_enabled >> transmitting >> decoding
               >> rx_df >> tx_df >> de_call >> de_grid >> dx_grid >> watchdog_timeout >> sub_mode
               >> fast_mode;

                msgOut.append(QString::number(type));
                //+ QString::number(frequency) + QString::fromUtf8 (mode) + QString::fromUtf8 (dx_call)
                //     + QString::fromUtf8 (report) + QString::fromUtf8 (tx_mode)
                //     + QString::fromUtf8 (de_call) + QString::fromUtf8 (de_grid)
                //     + QString::fromUtf8 (dx_grid);

            emit status_update (type, dx_call, frequency, mode);

        break;
        case 2:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Decode" << endl;
        break;
        case 3:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Clear" << endl;
        break;
        case 4:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay " << endl;
        break;
        case 5:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - QSO logged" << endl;

            in >> time_off >> dx_call >> dx_grid >> frequency >> mode >> report_sent >> tx_power >> comments >> name >> time_on;
            emit status_update (type, dx_call, frequency, mode);

            //out << type;
            //emit status_update (out);
/*
            qDebug() << "UDPServer::parse: -   DXCall = " << dx_call << endl;
            qDebug() << "UDPServer::parse: -   Grid = " << dx_grid << endl;
            qDebug() << "UDPServer::parse: -   Freq = " << QString::number(dial_frequency) << endl;
            qDebug() << "UDPServer::parse: -   Mode = " << mode << endl;
            qDebug() << "UDPServer::parse: -   ReportSent = " << report_sent << endl;
            qDebug() << "UDPServer::parse: -   TX_PWR = " << tx_power << endl;
            qDebug() << "UDPServer::parse: -   Comments = " << comments << endl;
            qDebug() << "UDPServer::parse: -   Name = " << name << endl;
            qDebug() << "UDPServer::parse: -   Time = " << time_on.toString("HHMMSSzzz") << endl;
*/

        break;
        case 6:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - Close " << endl;
        break;
        case 7:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Replay" << endl;
        break;
        case 8:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Halt TX" << endl;
        break;
        case 9:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - IN - Free Text" << endl;
        break;
        case 10:
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - OUT - WSPR Decode" << endl;
        break;
        default: //NO
            qDebug() << "UDPServer::parse: -   type = " << QString::number(type) << " - ERROR on Type" << endl;
        break;
    }



    //qDebug() << "UDPServer::parse: - Magic: = " << QString::number(magic)<< endl;

}


bool UDPServer::stop()
{
    //qDebug() << "UDPServer::stop"<< endl;
    return false;
}

void UDPServer::setPort(const int _port)
{
    //qDebug() << "UDPServer::setPort: " << QString::number(_port) << endl;
    if ((_port >= 0) && (_port<=65535))
    {
        port = _port;
    }
}

