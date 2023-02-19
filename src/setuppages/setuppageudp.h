#ifndef SETUPPAGEUDP_H
#define SETUPPAGEUDP_H
/***************************************************************************
                          setuppageudp.h  -  description
                             -------------------
    begin                : sept 2018
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
#include <QtWidgets>
#include <QNetworkInterface>
#include "../utilities.h"


class SetupPageUDP : public QWidget
{
    Q_OBJECT
public:
    SetupPageUDP(QWidget *parent = nullptr);
    ~SetupPageUDP();

    void setLogFromWSJTx(const QString &_t);
    void setAutoLogFromWSJTx(const QString &_t);
    void setReaDataFromWSJTx(const QString &_t);
    void setUDPServer(const QString &_t);
    void setUDPServerPort(const QString &_t);
    void setTimeout(const QString &_t);
    void setNetworkInterface(const QString &_t);

    QString getLogFromWSJTx();
    QString getAutoLogFromWSJTx();
    QString getReaDataFromWSJTx();
    QString getUDPServerPort();
    QString getUDPServer();
    QString getTimeout();
    QString getNetworkInterface();
    void saveSettings();

private:
    void createUI();
    void createActions();
    void fillNetworkInterfaceComboBox();

    QCheckBox *logFromWSJTXCheckbox, *logAutomaticallyWSJTXCheckbox, *realDataFromWSJTXCheckbox, *UDPServerCheckBox;
    //QLineEdit *wsjtxIPAddress, *wsjtxPortNumber;
    QSpinBox *UDPServerPortSpinBox;
    QSpinBox *miliSecsSpinBox;
    QComboBox *networkInterfacesComboBox;

    int defaultport, defaultTimer;
    Utilities *util;

    //bool logFromWSJTx, autoLogFromWSJTx, realDataFromWSJTx;

signals:

public slots:
    void slotLogFromWSJTCheckBoxClicked();
    //void slotAutoLogFromWSJTCheckBoxClicked();
    //void slotRealFromWSJTCheckBoxClicked();
    void slotUDPServerCheckBoxClicked();


};

#endif // SETUPPAGEUDP_H
