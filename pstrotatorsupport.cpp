#include "pstrotatorsupport.h"
//#include <QString>

PSTRotatorSupport::PSTRotatorSupport(QObject *parent) :
    QObject(parent)
{
     //qDebug() << "PSTRotatorSupport::PSTRotatorSupport"  << endl;
       //address = QString("127.0.0.1");
       port = 12040;
       socketServer = new QUdpSocket(this);
       //socketServer->bind(QHostAddress::AnyIPv4, port);

       //connect(socketServer,SIGNAL(readyRead()),this,SLOT(slotReadPendingDatagrams()));
}

bool PSTRotatorSupport::start()
{
    setServer(server);
    return socketServer->bind(address, port);
    //connect(socketServer,SIGNAL(readyRead()),this,SLOT(slotReadPendingDatagrams()));
}

void PSTRotatorSupport::setPort(const int _port)
{
     //qDebug() << "PSTRotatorSupport::setPort: " << QString::number(_port) << endl;
    if ((_port >= 0) && (_port<=65535))
    {
        port = _port;
    }
    else
    {
        port = 12040;
    }
}

void PSTRotatorSupport::setServer(const QString &_s)
{
    server = _s;
    address.setAddress(_s);
}


void PSTRotatorSupport::sendAzimut(const int _d)
{
    /*
        <PST><AZIMUTH>85</AZIMUTH></PST>
     */
    if ((_d >= 0) && (_d <= 365))
    {
        QString st;
        st = QString("<PST><AZIMUTH>%1</AZIMUTH></PST>").arg(_d);
        QByteArray ba;
        ba.clear();
        ba = st.toUtf8();
        socketServer->writeDatagram(ba, address, port);
    }
}

void PSTRotatorSupport::sendCall(const QString _c)
{
    /*
        <PST><CALL>EA4XXX</CALL></PST>
     */
    if (_c.length()>=3)
    {
        QString st;
        st = QString("<PST><CALL>%1</CALL></PST>").arg(_c);
        QByteArray ba;
        ba.clear();
        ba = st.toUtf8();
        socketServer->writeDatagram(ba, address, port);
    }
}
void PSTRotatorSupport::sendFreq(const int _freq, const int _radio)
{// Frequencies for antennas Receives MHz (to pstrotator must be sent in 10hz but I will do the conversion in this function)
    /* // Freq is in 10hz
     * // Radio is 1..4
        <PST><FREQUENCY>2489200</FREQUENCY><RADIO>1</RADIO></PST>
     */
    //qDebug() << "PSTRotatorSupport::sendFreq: " << QString::number(_freq) << "/" << QString::number(_radio)<< endl;
    if ((_freq>0) && (_radio>0) && (_radio <5))
    {
        int f = _freq/100;
        QString st;
        st = QString("<PST><FREQUENCY>%1</FREQUENCY><RADIO>%2</RADIO></PST>").arg(f).arg(_radio);
        QByteArray ba;
        ba.clear();
        ba = st.toUtf8();
        socketServer->writeDatagram(ba, address, port);
    }

}
