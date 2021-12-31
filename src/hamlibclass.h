#ifndef HAMLIBCLASS_H
#define HAMLIBCLASS_H

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

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QDebug>
//#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <hamlib/rig.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
    void setStop(const int _stop);
    void setFlow(const QString &_flow);
    void setParity(const QString &_parity);
    void setSpeed(const int _speed);
    void setRTS(const QString &_state);
    void setDTR(const QString &_state);

    void setFreq(const double _fr);
    void setMode(const QString &_m);
    void setReadOnly(const bool _r);
    bool isModeADIFMode(const QString &_m);
    void setNetworkAddress(const QString &_address);
    void setNetworkPort(const int _port);

    bool init(bool _active);
    bool stop();
    bool readRadio(bool _forceRead);
    bool isRunning();
    void initClass();
    void clean();
    void checkErrorCountAndStop();

    double getFrequency();
    //void showDebugLog(const QString &_func, const QString &_log);
    //bool openSerialPort();
    //bool closeSerialPort();


signals:
    void freqChanged(double newFreq);
    void modeChanged(QString newFreq);

public slots:
    void slotTimer();

private:
    bool readRadioInternal(bool _forceRead);
    static int addRigToList(const struct rig_caps* caps, void* data);
    QString hamlibMode2Mode(rmode_t _rmode);
    bool errorManage(const QString &_func, const int _errorcode);
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

    int retcode;                // generic return code from functions

    rig_model_t myrig_model;    // Integer radio model
    //hamlib_port_t myport;       // Hamlib port


    pbwidth_t width;
    //vfo_t vfo;              /* vfo selection */
    //int strength;           /* S-Meter level */

    int bauds;                  // default 9600
    int dataBits;               // default 8
    int stopBits;               // default 1
    QString flowControl;            // default QSerialPort::NoFLowControl
    QString parity;                 // default QSerialPort::NoParity
    QString serialPort;
    QString networkAddress;
    int networkPort;
    int pollInterval;           // Poll interval in mSecs
    int errorCount;            // Number of times that the rig has returned an error since last time OK.
    bool rigLaunched;
    bool readOnlyMode;          // If true, KLog will not modify any parameter (freq/mode...) in the radio. KLog just will follow the radio.
    bool justEmitted;
    bool reading;   // Just a semaphore to prevent several readings
    //bool active;

    //QSerialPort *m_serial;
};

#endif // HAMLIBCLASS_H
