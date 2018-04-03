#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QObject>

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
    void analyzeData(const QByteArray _qsoData);
    QUdpSocket *socketServer;

    //QString address;
    int port;

signals:

private slots:
    void slotReadPendingDatagrams();


};

#endif // UDPSERVER_H
