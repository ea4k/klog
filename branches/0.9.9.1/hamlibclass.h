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
    int getModelIdFromName (const QString &_name);
    QString getNameFromModelId(const int _id);
    void setModelId(const int _id);
    void setPort(const QString &_port);
    void setPool(const int _milsecs);

    void setData(const QString &_data);
    void setStop(const QString &_stop);
    void setFlow(const QString &_flow);
    void setParity(const QString &_parity);
    void setSpeed(const QString &_speed);
    void setRTS(const QString &_state);
    void setDTR(const QString &_state);

    void setFreq(const double _fr);
    void setMode(const QString &_m);
    void setReadOnly(const bool _r);
    bool isModeADIFMode(const QString &_m);

   // bool isModeExisting(const QString &_m);


    void init(bool _active);
    void stop();
    void readRadio();
    bool isRunning();
    void clean();
    void checkErrorCountAndStop();

    //bool openSerialPort();
    //bool closeSerialPort();


signals:
    void freqChanged(double newFreq);
    void modeChanged(QString newFreq);

public slots:
    void slotTimer();

private:
    static int addRigToList(const struct rig_caps* caps, void* data);
    QString hamlibMode2Mode(rmode_t _rmode);
    rmode_t mode2HamlibMode(const QString &_m);
    QStringList strings;
    QTimer *timer;
    QMap<QString, rig_model_t> rigName2RigId;
    QMap<rig_model_t, QString> rigId2RigName;

    RIG *my_rig;                // handle to rig (instance)
    freq_t freq, freq_old;                // Radio freq
    rmode_t rmode, mode_old;              // Radio mode
    serial_parity_e sparity;
    serial_handshake_e shandshake;
    serial_control_state_e srts, sdtr;


    int retcode;                // generic return code from functions

    rig_model_t myrig_model;    // Integer radio model
    //hamlib_port_t myport;       // Hamlib port


    pbwidth_t width;
    vfo_t vfo;              /* vfo selection */
    int strength;           /* S-Meter level */



    int bauds;                  // default 9600
    int dataBits;               // default 8
    int stopBits;               // default 1
    QString flowControl;            // default QSerialPort::NoFLowControl
    QString parity;                 // default QSerialPort::NoParity
    QString serialPort;
    int pollInterval;           // Pool interval in mSecs
    int errorCount;            // Number of times that the rig has returned an error since last time OK.
    bool rigLaunched;
    bool readOnlyMode;          // If true, KLog will not modify any parameter (freq/mode...) in the radio. KLog just will follow the radio.
    bool justEmitted;
    //bool active;

    //QSerialPort *m_serial;
};

#endif // HAMLIBCLASS_H
