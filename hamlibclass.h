#ifndef HAMLIBCLASS_H
#define HAMLIBCLASS_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QDebug>
#include <QSerialPort>
#include <hamlib/rig.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



class HamLibClass : public QObject
{
    Q_OBJECT
public:
    explicit HamLibClass(QObject *parent = nullptr);
    ~HamLibClass();
    QStringList getRigList ();
    int getModelIdFromName (const QString _name);
    QString getNameFromModelId(const int _id);
    void setModelId(const int _id);
    void setPort(const QString _port);
    void setSpeed(const int _speed);
    void setPatity(const int _parity);
    void setFlow(const int _flow);
    void setStop(const int _stop);
    void setData(const int _data);
    bool setFreq(const double _fr);

    bool init();
    bool stop();
    bool isRunning();
    void clean();

signals:
    void freqChanged(double newFreq);

public slots:
    void slotTimer();

private:
    static int addRigToList(const struct rig_caps* caps, void* data);
    QStringList strings;
    QTimer *timer;
    QMap<QString, rig_model_t> rigName2RigId;
    QMap<rig_model_t, QString> rigId2RigName;

    RIG *my_rig;                // handle to rig (instance)
    freq_t freq, freq_old;                // Radio freq
    rmode_t rmode;              // Radio mode

    int retcode;                // generic return code from functions

    rig_model_t myrig_model;    // Integer radio model
    hamlib_port_t myport;       // Hamlib port

    int bauds;                  // default 9600
    int dataBits;               // default 8
    int stopBits;               // default 1
    int flowControl;            // default QSerialPort::NoFLowControl
    int parity;                 // default QSerialPort::NoParity
    QString serialPort;

    bool rigLaunched;
};

#endif // HAMLIBCLASS_H
