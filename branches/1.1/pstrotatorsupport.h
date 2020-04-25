#ifndef PSTROTATORSUPPORT_H
#define PSTROTATORSUPPORT_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>

class PSTRotatorSupport : public QObject
{
    Q_OBJECT

public:
    explicit PSTRotatorSupport(QObject *parent = nullptr);
    bool start();

    void setPort(const int _port);
    void setServer(const QString &_s);

    void sendAzimut(const int _d);  // Azimuth Degrees
    void sendCall(const QString _c);  // Sends the callsignto PSTRotator to point
    void sendFreq(const int _freq, const int _radio);  // Frequencies for antennas receives MHz (to pstrotator must be sent in 10hz but I will do the conversion in this function)


    //void stopRotor (const bool _s);
    //void parkRotor (const bool _s);
    //void setLPSP (const QString &_s);




private:


    QUdpSocket *socketServer;    
    QHostAddress address;

    //QString address;
    int port;
    QString server;


signals:


private slots:
    //void slotReadPendingDatagrams();


};

#endif // PSTRotatorSupport_H
