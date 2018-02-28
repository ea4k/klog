#include "udpserver.h"

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
       //address = QString("127.0.0.1");
       port = 2237;
       socketServer=new QUdpSocket(this);

       connect(socketServer,SIGNAL(readyRead()),this,SLOT(slotReadPendingDatagrams()));
}

void UDPServer::slotReadPendingDatagrams()
{
    QByteArray datagram;
    while (socketServer->hasPendingDatagrams()) {

        datagram.resize(socketServer->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        socketServer->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
        //qDebug() << "UDPServer::readPendingDatagrams: RX: " << datagram << endl;
    }
    analyzeData(datagram);
}

void UDPServer::analyzeData(const QByteArray _qsoData)
{
    QStringList qs, oneField;
    QString str = QString();
    QString field = QString();
    QString data = QString();
    QString _call = QString();
    QString _freq = QString();
    QString _mode = QString();
    QString _grisquare = QString();
    QString _my_grisquare = QString();
    QString _qso_date = QString();
    QString _qso_date_off = QString();
    QString _time_on = QString();
    QString _time_off = QString();
    QString _rst_sent = QString();
    QString _rst_rcvd = QString();
    QString _rx_pwr = QString();
    QString _tx_pwr = QString();
    QString _station_callsign = QString();

    //QDate date;
    //QTime time;

    qs.clear();
    oneField.clear();


    qs << data.split("<", QString::SkipEmptyParts);

    foreach (str, qs)
    {
        //qDebug() << "UDPServer::analyzeData: " << str << endl;
        if ( !( (str.contains(":")) && (str.contains(">")) ) )
        {
            //qDebug() << "UDPServer::analyzeData: NOT (contains : and >): " << str << endl;
        }
        else
        {
            //qDebug() << "UDPServer::analyzeData: (contains : and >): " << str << endl;

            oneField = str.split(">", QString::SkipEmptyParts);

            //qDebug() << "UDPServer::analyzeData: (oneField)" << oneField << endl;
            //if (checkADIFValidFormat(oneField))
            int i = 0;
            if (1)
            {
                i = (qs.at(0)).count(":");
                field = (oneField.at(0)).trimmed(); // Needs to be cleared FIELD:4:D
                data = (oneField.at(1)).trimmed();
                //data = util->checkAndFixASCIIinADIF(data);


                if (field == "CALL")
                {
                    _call = data;
                    //haveCall = true;

                    //qDebug() << "UDPServer::analyzeData-CALL:" << data << endl;
                }
                else if (field == "QSO_DATE")
                {
                    //qDebug() << "UDPServer::analyzeData-QSO_DATE:" << data << endl;
                    _qso_date = data;
                    //haveDate = true;
                }

                else if (field == "MODE")
                {
                    _mode = data;
                }

                else if (field == "TIME_ON")
                {
                    if (data.length() == 4)
                    {
                        data = data + "00";
                    }

                    _time_on =  data;

                }
                else if (field == "QSO_DATE_OFF")
                {
                    _qso_date_off = data;
                }


                else if (field == "TIME_OFF")
                {
                    if (data.length() == 4)
                    {
                        data = data + "00";
                    }

                    _time_off = data;
                }

                else if (field == "RST_SENT")
                {
                    _rst_sent = data;
                }

                else if (field == "RST_RCVD")
                {
                    _rst_rcvd = data;
                }

                else if (field == "FREQ")
                {
                    _freq = data;
                }

                else if (field == "GRIDSQUARE")
                {
                    _grisquare = data;
                }

                else if (field == "MY_GRIDSQUARE")
                {
                    _my_grisquare = data;
                }

                else if (field == "STATION_CALLSIGN")
                {
                    _station_callsign = data;
                }

                else if (field == "RX_PWR")
                {
                    _rx_pwr = data;
                }
                else if (field == "TX_PWR")
                {
                    _tx_pwr = data;
                }

                else
                {
                }
            }
            else
            {
                //qDebug() << "UDPServer::analyzeData (field) CheckAdif FALSE: " << field << endl;
            }
        }
    }




}

bool UDPServer::start()
{
    //qDebug() << "UDPServer::start "<< endl;
    return socketServer->bind(QHostAddress::LocalHost, port);
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

