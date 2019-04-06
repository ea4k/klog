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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QObject>
#include <QWidget>
#include <QtWidgets>
//#include <QSerialPort>
#include <QSerialPortInfo>

#include "dataproxy.h"
#include <hamlib/rig.h>


class SetupPageHamLib : public QWidget
{
    Q_OBJECT
public:
    explicit SetupPageHamLib(DataProxy *dp, QWidget *parent = nullptr);
    QString getData();
    bool setRigType(const QString _radio);
    bool setSerialPort(const QString _port);
    bool setSerialSpeed(const QString _speed );

signals:

public slots:

private:
    void createUI();
    void setRig();
    void setDefaults();
    static int addRigToList(const struct rig_caps* caps, void* data);
    QStringList getAvailableSerialPorts();
    QButtonGroup *flowControlLineButtonGroup, *handshakeButtonGroup, *dataBitsGroupButton, *stopBitsButtonGroup;

    QComboBox *rigTypeComboBox, *serialBaudsComboBox, *serialPortComboBox;
    //QSpinBox *serialBaudsSpinBox;
    QLineEdit *serialPort;

    RIG *my_rig;            // handle to rig (instance)
    freq_t freq;
    rig_model_t myrig_model;

    QStringList strings, baudSpeeds, serialPorts;
    QCheckBox *dataBits7CheckBox, *dataBits8CheckBox, *stopBits1CheckBox, *stopBits2CheckBox,
    *handshakeNoneCheckBox, *handshakeXCheckBox, *handshakeHCheckBox,
    *flowControlLinesDTRCheckBox,  *flowControlLinesRTSCheckBox;


    //int defaultPortSpeed;
};

#endif // SETUPPAGEHAMLIB_H
