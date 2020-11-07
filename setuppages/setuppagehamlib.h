#ifndef SETUPPAGEHAMLIB_H
#define SETUPPAGEHAMLIB_H

/***************************************************************************
                          setuppagehamlib.h  -  description
                             -------------------
    begin                : feb 2018
    copyright            : (C) 2018 by Jaime Robles
    email                : jaime@robles.es
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QSerialPortInfo>
#include "hamlibclass.h"
#include "dataproxy_sqlite.h"
#include <hamlib/rig.h>


class SetupPageHamLib : public QWidget
{
    Q_OBJECT
public:
    explicit SetupPageHamLib(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    QString getData();
    bool setRigType(const QString &_radio);
    bool setPollingInterval(const int _msecs);
    bool setSerialPort(const QString &_port);
    bool setSerialSpeed(const QString &_speed );
    void setActive(const QString &_active);
    //void setRTS(const QString &_state);
    //void setDTR(const QString &_state);
    void setDataBits(const QString &_st);
    void setFlowControl(const QString &_st);
    void setParity(const QString &_st);
    void setStopBits(const QString &_st);
    //void setPollInterval(const QString &_st);
    void setReadOnly(const QString &_m);



signals:

public slots:
    void slotScanPorts();

private:
    void createUI();
    void setRig();
    void setDefaults();
    void fillSerialPortsComboBox();
    //static int addRigToList(const struct rig_caps* caps, void* data);
    QStringList getAvailableSerialPorts();

    QString getDataBits();
    QString getFlowControl();
    QString getParity();
    QString getStopBits();
    QString getPollInterval();

/*

DataBits { Data5, Data6, Data7, Data8, UnknownDataBits }
FlowControl { NoFlowControl, HardwareControl, SoftwareControl, UnknownFlowControl }
Parity { NoParity, EvenParity, OddParity, SpaceParity, MarkParity, UnknownParity }
StopBits { OneStop, OneAndHalfStop, TwoStop, UnknownStopBits }

*/

    QPushButton *scanSerialPortButton;
    QComboBox *rigTypeComboBox, *serialBaudsComboBox, *serialPortComboBox, *dataBitsComboBox,
                *flowControlComboBox, *parityComboBox, *stopBitsComboBox;
    //QSpinBox *serialBaudsSpinBox;
    QLineEdit *serialPort;
    QSpinBox *pollIntervalQSpinBox;
    int pollMin, pollMax;

    HamLibClass *hamlib;

    //RIG *my_rig;            // handle to rig (instance)
    //freq_t freq;
    //rig_model_t myrig_model;

    QStringList strings, serialPorts;
    QCheckBox *activateHamlibCheckBox, *readOnlyModeCheckBox; //, *RTSCheckBox, *DTRCheckBox;


    //int defaultPortSpeed;
};

#endif // SETUPPAGEHAMLIB_H
