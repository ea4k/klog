#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include <QDataStream>
#include "utilities.h"

class UDPServer : public QObject
{
    Q_OBJECT

public:
    explicit UDPServer(QObject *parent = nullptr);
    bool start();
    bool stop();
    bool isStarted();
    void setLogging(const bool _t);
    void setRealTimeUpdate(const bool _t);
    //void setAddress(const QString _address);
    void setPort(const int _port);

private:
    void readPendingDatagrams();
    void parse(const QByteArray &msg);
    void adifParse(QByteArray &msg);

    QUdpSocket *socketServer;    
    QHostAddress groupAddress;

    //QString address;
    int port;
    bool logging, realtime;

    Utilities *util;

signals:
    void status_update (const int _type, const QString _dxcall, const double _freq, const QString _mode,
                        const QString report, const QString de_call, const QString de_grid,
                        const QString dx_grid, const QString sub_mode);

    //void logged_qso (const int _type, const QString _dxcall, const double _freq, const QString _mode,
    //                 const QString _dx_grid, const QString _time_off, const QString _report_sent, const QString _report_rec,
    //                 const QString _tx_power, const QString _comments, const QString _name, const QString _time_on, const QString _de_call, const QString _de_grid);

    void logged_qso (const QString _dxcall, const QString _mode, const QString band, const double _freq,
                     const QString mygrid, const QString dxgrid, const QString rstTX, const QString rstRX, const QString comment, const QString stationcallsign,
                     const QDateTime datetime, const QDateTime datetime_off);

private slots:
    void slotReadPendingDatagrams();


};

#endif // UDPSERVER_H
