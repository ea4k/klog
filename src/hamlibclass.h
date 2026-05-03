#ifndef KLOG_HAMLIBCLASS_H
#define KLOG_HAMLIBCLASS_H

/***************************************************************************
                          hamlibclass.h  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include <QDebug>
#include <QMap>
#include <QObject>
#include <QTimer>
#include <QString>
#include <QStringList>
#include "frequency.h"
//#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <hamlib/rig.h>
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <stdlib.h>
#include <QSettings>
#include "klogdefinitions.h"

// Potential fix of hamlib 4.2 migration
#ifndef HAMLIB_FILPATHLEN
#define HAMLIB_FILPATHLEN FILEPATHLEN
#endif

#ifndef FILPATHLEN
#define FILPATHLEN 100
#endif


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
    void setPoll(const int _milsecs);

    void setDataBits(const int data);
    void setStop(const QString &_stop);
    void setFlow(const QString &_flow);
    void setParity(const QString &_parity);
    void setSpeed(const int _speed);
    void setRTS(const QString &_state);
    void setDTR(const QString &_state);

    void setFreq(const Frequency &_fr, bool _TX=true);
    void setMode(const QString &_m);

    void setReadOnly(const bool _r);
    void setSplit(const bool _split);


    bool isModeADIFMode(const QString &_m);
    void setNetworkAddress(const QString &_address);
    void setNetworkPort(const int _port);

    bool init(bool _active);
    bool stop();
    bool readRadio();
    bool forceRead();
    bool isRunning();
    void initClass();
    void clean();
    void checkErrorCountAndStop();

    //double getFrequency();
    //void showDebugLog(const QString &_func, const QString &_log);
    bool loadSettings();

signals:
    void radioStatusChanged(RadioStatus);
    void freqTXChanged(Frequency newfreq);
    void modeChanged(QString newMode);
    void rigDisconnected();   // Emitted when the rig stops responding
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);

public slots:
    void slotTimer();


private:
    enum class RigState {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting,
        Error
    };
    RigState rig_state = RigState::Disconnected;

    bool readFreq();
    bool readMode();
    bool readVFO();                     //Reads the current VFO
    bool readSplit();
    void probeSplitVfoSideEffect();     //Probe for rig_get_split_vfo VFO side-effects at connect time
    void cleanup();
    bool radioStatusChanged(const RadioStatus _old, const RadioStatus _new);

    bool readRadioInternal();
    void fillRigsList();
    static int addRigToList(const struct rig_caps* caps, void* data);
    QString hamlibMode2Mode(rmode_t _rmode);
    rmode_t mode2HamlibMode (const QString &_mode);
    bool errorManage(const QString &_func, const int _errorcode);

    void logEvent(const QString &_func,
                  const QString &_msg,
                  DebugLogLevel _level);
    //rmode_t mode2HamlibMode(const QString &_m);
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
    vfo_t currentVfo;         // Cached result of rig_get_vfo(), updated each poll cycle

    //int retcode;                // generic return code from functions

    rig_model_t myrig_model;    // Integer radio model
    //hamlib_port_t myport;       // Hamlib port


    pbwidth_t width;
    //vfo_t vfo;              /* vfo selection */
    //int strength;           /* S-Meter level */

    int bauds;           // default 9600
    int dataBits;        // default 8
    int stopBits;        // default 1
    QString flowControl; // default QSerialPort::NoFLowControl
    QString parity;      // default QSerialPort::NoParity
    QString serialPort;
    QString networkAddress;
    int networkPort;
    int pollInterval; // Poll interval in mSecs
    int errorCount;   // Number of times that the rig has returned an
                      // error since last time OK.
    static constexpr int ERROR_THRESHOLD     = 1;  // Fatal comms errors before disconnect
    static constexpr int SOFT_ERROR_THRESHOLD = 10; // Soft/config errors before disconnect
    //bool connected;   // When we connect to the rig

    bool readOnlyMode; // If true, KLog will not modify any parameter
                       // (freq/mode...) in the radio.
                       // KLog just will follow the radio.
    bool justEmitted;
    bool reading; // Just a semaphore to prevent several readings
    bool vfoQuerySupported;
    bool splitQuerySupported;
    bool m_rigsLoaded = false;
    RadioStatus radioStatus;
};

#endif // HAMLIBCLASS_H
