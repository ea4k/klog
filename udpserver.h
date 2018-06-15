#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include <QDataStream>

class UDPServer : public QObject
{
    Q_OBJECT

public:
    explicit UDPServer(QObject *parent = 0);
    bool start();
    bool stop();
    //void setAddress(const QString _address);
    void setPort(const int _port);




private:
    void readPendingDatagrams();
    void parse(const QByteArray &msg);

    QUdpSocket *socketServer;    
    QHostAddress groupAddress;

    //QString address;
    int port;

signals:
    void status_update (const int _type, const QString _dxcall, const quint64 _freq, const QString _mode);


private slots:
    void slotReadPendingDatagrams();


};

#endif // UDPSERVER_H
