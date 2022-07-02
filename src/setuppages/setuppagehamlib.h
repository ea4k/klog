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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QSerialPortInfo>
#include "../hamlibclass.h"
#include "../dataproxy_sqlite.h"
#include "hamlibserialconfigwidget.h"
#include "hamlibnetworkconfigwidget.h"
#include <hamlib/rig.h>

class SetupPageHamLib : public QWidget
{
    Q_OBJECT
public:
    explicit SetupPageHamLib(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    QString getData();
    bool setRigType(const QString &_radio);
    void setPollingInterval(const int _msecs);
    bool setSerialPort(const QString &_port);
    bool setSerialSpeed(const int _speed );
    void setActive(const QString &_active);
    //void setRTS(const QString &_state);
    //void setDTR(const QString &_state);
    void setDataBits(const int _st);
    void setFlowControl(const QString &_st);
    void setParity(const QString &_st);
    void setStopBits(const QString &_st);
    //void setPollInterval(const QString &_st);
    void setReadOnly(const QString &_m);
    void setRadioNetworkAddress(const QString &_m);
    void setRadioNetworkPort(const int _p);
    void stopHamlib();

public slots:
    //void slotScanPorts();
    void slotRadioComboBoxChanged(QString _r);
    void slotTestHamlib();

private:
    void createUI();
    void setRig();
    void setDefaults();
    void setTestResult(const bool _ok);

    QStringList getAvailableSerialPorts();

    int getDataBits();
    QString getFlowControl();
    QString getParity();
    //int getStopBits();


    QTabWidget *tabWidget;
    HamLibSerialConfigWidget *serialConfigWidget;
    HamLibNetworkConfigWidget *networkConfigWidget;
    QComboBox *rigTypeComboBox;

    QPushButton  *testHamlibPushButton;

    QSpinBox *pollIntervalQSpinBox;
    int pollMin, pollMax, rigctlport;

    HamLibClass *hamlib;

    QCheckBox *activateHamlibCheckBox, *readOnlyModeCheckBox; //, *RTSCheckBox, *DTRCheckBox;
    bool networkRadio, hamlibTestOK;
};

#endif // SETUPPAGEHAMLIB_H
