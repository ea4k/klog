/***************************************************************************
                          setuppageudp.cpp  -  description
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

#include "setuppageudp.h"

SetupPageUDP::SetupPageUDP(QWidget *parent) : QWidget(parent)
{
    //qDebug() << "SetupPageUDP::SetupPageUDP";
    logFromWSJTXCheckbox = new QCheckBox(this);
    realDataFromWSJTXCheckbox  = new QCheckBox(this);
    logAutomaticallyWSJTXCheckbox  = new QCheckBox(this);

    UDPServerCheckBox = new QCheckBox(tr("Start UDP Server"), this);
    UDPServerPortSpinBox = new QSpinBox;
    miliSecsSpinBox = new QSpinBox;
    networkInterfacesComboBox = new QComboBox;
    //qDebug() << "SetupPageUDP::SetupPageUDP: 1";
    util = new Utilities(Q_FUNC_INFO);

    defaultport = 2237;     // Default WSJTX port
    defaultTimer = 2000;  // 2 secs

    //qDebug() << "SetupPageUDP::SetupPageUDP: 2";
    createUI();
    //qDebug() << "SetupPageUDP::SetupPageUDP: 3";
    createActions();
    //qDebug() << "SetupPageUDP::SetupPageUDP: 4";
    fillNetworkInterfaceComboBox();
    //qDebug() << "SetupPageUDP::SetupPageUDP - END";
}

SetupPageUDP::~SetupPageUDP()
{
    delete(util);
       //qDebug() << "SetupPageUDP::~SetupPageUDP";
}

void SetupPageUDP::createUI()
{
    QString msgAuto = QString(tr("Automatically log QSOs from WSJT-X"));
    logFromWSJTXCheckbox->setText(tr("Allow WSJT-X to send logged QSOs to KLog"));
    logFromWSJTXCheckbox->setToolTip(tr("QSOs logged in WSJT-X will be sent to KLog and KLog will ask before logging into KLog unless \"%1\" is selected").arg(msgAuto));

    logAutomaticallyWSJTXCheckbox->setText(msgAuto);
    logAutomaticallyWSJTXCheckbox->setToolTip(tr("KLog will automatically log any QSO coming from WSJT-X without any manual confirmation."));


    realDataFromWSJTXCheckbox->setText(tr("Update status information from WSJT-X"));
    realDataFromWSJTXCheckbox->setToolTip(tr("KLog will automatically show and update data coming from WSJT-X (DX callsign, Report, mode, ...)"));

    UDPServerCheckBox->setChecked(false);
    logFromWSJTXCheckbox->setChecked(false);
    logAutomaticallyWSJTXCheckbox->setChecked(false);
    realDataFromWSJTXCheckbox->setChecked(false);

    logFromWSJTXCheckbox->setEnabled(false);
    logAutomaticallyWSJTXCheckbox->setEnabled(false);
    realDataFromWSJTXCheckbox->setEnabled(false);

    QVBoxLayout *checkLayout = new QVBoxLayout;
    checkLayout->addWidget(logFromWSJTXCheckbox);
    checkLayout->addWidget(logAutomaticallyWSJTXCheckbox);
    checkLayout->addWidget(realDataFromWSJTXCheckbox);

    QString labelTip = tr("UDP port number where the UDP Server will listen for packets.") + "\n" + tr("Make sure it is the same port that the other programs are sending the data to. Default port is 2237.");
    UDPServerCheckBox->setToolTip(tr("UDP Server will receive QSOs sent from other programs like WSJT-X allowing you to log in KLog automatically from those programs."));
    UDPServerPortSpinBox->setToolTip(labelTip);

    UDPServerPortSpinBox->setMinimum(0);
    UDPServerPortSpinBox->setMaximum(65535);
    UDPServerPortSpinBox->setValue(defaultport);

    QLabel *udpPortLabel = new QLabel(tr("UDP Port"));
    udpPortLabel->setBuddy(UDPServerPortSpinBox);
    udpPortLabel->setToolTip(labelTip);
    udpPortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    udpPortLabel->setEnabled(true);

    QHBoxLayout *UDPPortLayout = new QHBoxLayout;
    UDPPortLayout->addWidget(udpPortLabel);
    UDPPortLayout->addWidget(UDPServerPortSpinBox);

    networkInterfacesComboBox->setToolTip(tr("Select the interface to listen for UDP datagrams coming from WSJT-X."));
    QLabel *networkInterfaceLabel = new QLabel(tr("Network interface"));
    networkInterfaceLabel->setToolTip(tr("Select the interface to listen for UDP datagrams coming from WSJT-X."));
    QHBoxLayout *networkIfacesLayout = new QHBoxLayout;
    networkIfacesLayout->addWidget(networkInterfaceLabel);
    networkIfacesLayout->addWidget(networkInterfacesComboBox);

    miliSecsSpinBox->setMinimum(0);
    miliSecsSpinBox->setMaximum(30000);
    miliSecsSpinBox->setValue(defaultTimer);
    QLabel *miliSecsSpinBoxLabel = new QLabel(tr("QSOs notification timeout (milisecs)"));
    miliSecsSpinBox->setToolTip(tr("Miliseconds that the notification of QSOs received from WSJTX will be shown."));
    miliSecsSpinBoxLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    miliSecsSpinBoxLabel->setEnabled(true);
    miliSecsSpinBox->setEnabled(false);

    QHBoxLayout *UDPTimeLayout = new QHBoxLayout;
    UDPTimeLayout->addWidget(miliSecsSpinBoxLabel);
    UDPTimeLayout->addWidget(miliSecsSpinBox);

    //QHBoxLayout *UDPLayout = new QHBoxLayout;
    //UDPLayout->addLayout(networkIfacesLayout);
    //UDPLayout->addLayout(UDPPortLayout);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addLayout(networkIfacesLayout, 0, 0);
    gridLayout->addLayout(UDPPortLayout, 0, 1);
    gridLayout->addLayout(UDPTimeLayout, 1, 0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(UDPServerCheckBox);
    layout->addLayout(gridLayout);
    layout->addLayout(checkLayout);

    setLayout(layout);
}

void SetupPageUDP::createActions()
{
    connect(logFromWSJTXCheckbox, SIGNAL(clicked()), this, SLOT(slotLogFromWSJTCheckBoxClicked() ) );
    //connect(logAutomaticallyWSJTXCheckbox, SIGNAL(clicked () ), this, SLOT(slotAutoLogFromWSJTCheckBoxClicked() ) );
    //connect(realDataFromWSJTXCheckbox, SIGNAL(clicked () ), this, SLOT(slotRealFromWSJTCheckBoxClicked() ) );
    connect(UDPServerCheckBox, SIGNAL(clicked()), this, SLOT(slotUDPServerCheckBoxClicked() ) );
}

void SetupPageUDP::slotUDPServerCheckBoxClicked()
 {
       //qDebug() << "SetupPageUDP::slotUDPServerCheckBoxClicked";

    if (UDPServerCheckBox->isChecked())
    {
        logFromWSJTXCheckbox->setEnabled(true);
        logFromWSJTXCheckbox->setChecked(true);
        logAutomaticallyWSJTXCheckbox->setEnabled(true);
        realDataFromWSJTXCheckbox->setEnabled(true);
        miliSecsSpinBox->setEnabled(true);
    }
    else
    {
        logFromWSJTXCheckbox->setChecked(false);
        logAutomaticallyWSJTXCheckbox->setChecked(false);
        realDataFromWSJTXCheckbox->setChecked(false);
        logFromWSJTXCheckbox->setEnabled(false);
        logAutomaticallyWSJTXCheckbox->setEnabled(false);
        realDataFromWSJTXCheckbox->setEnabled(false);
        miliSecsSpinBox->setEnabled(false);
    }
 }

void SetupPageUDP::fillNetworkInterfaceComboBox()
{
    QNetworkInterface loopBack;
    QStringList interfacesNames;
    interfacesNames.clear();
    networkInterfacesComboBox->clear();
    QList<QNetworkInterface> ifaces;
    ifaces.clear();
    ifaces << QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface i, ifaces)
    {
        if ((i.flags().testFlag(QNetworkInterface::IsUp)) && (i.flags().testFlag(QNetworkInterface::IsLoopBack)))
        {
            loopBack = i;
        }
        else if ((i.flags().testFlag(QNetworkInterface::IsUp)) && (i.humanReadableName().length()>0) && (i.flags().testFlag(QNetworkInterface::CanMulticast)) )
        {
            interfacesNames.append(i.humanReadableName() + "-" + i.hardwareAddress() );
        }
    }
    interfacesNames.insert(0, loopBack.humanReadableName());
    networkInterfacesComboBox->addItems(interfacesNames);
}

void SetupPageUDP::slotLogFromWSJTCheckBoxClicked()
{
       //qDebug() << "SetupPageUDP::slotLogFromWSJTCheckBoxClicked";
    if (logFromWSJTXCheckbox->isChecked())
    {
        logAutomaticallyWSJTXCheckbox->setEnabled(true);
    }
    else
    {
        logAutomaticallyWSJTXCheckbox->setEnabled(false);
        logAutomaticallyWSJTXCheckbox->setChecked(false);
    }
}


QString SetupPageUDP::getUDPServer()
{
    return util->boolToQString(UDPServerCheckBox->isChecked());
}

void SetupPageUDP::setUDPServer(const bool _t)
{
       //qDebug() << "SetupPageUDP::setUDPServer: "  << t;
    UDPServerCheckBox->setChecked(_t);
    slotUDPServerCheckBoxClicked();
}

void SetupPageUDP::setUDPServerPort(const int _t)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number (_t);
    if (  (_t>=0) && (_t<=65535))
    {
        UDPServerPortSpinBox->setValue(_t);
    }
    else
    {
        UDPServerPortSpinBox->setValue(defaultport);
    }
}

QString SetupPageUDP::getUDPServerPort()
{
    int t = UDPServerPortSpinBox->value();

    if (  (t>=0) && (t<=65535) )
    {
        return QString::number(t);
    }
    else
    {
        return QString::number(defaultport);
    }
}

void SetupPageUDP::setTimeout(const int _t)
{
    if (  (_t>=0) && (_t<=30000))
    {
        miliSecsSpinBox->setValue(_t);
    }
    else
    {
        miliSecsSpinBox->setValue(defaultTimer);
    }
}

/*
int SetupPageUDP::getTimeout()
{
    int t = miliSecsSpinBox->value();

    if (  (t>=0) && (t<=30000) )
    {
        return t
    }
    else
    {
        return defaultTimer;
    }
}
*/

QString SetupPageUDP::getLogFromWSJTx()
{
    return util->boolToQString(logFromWSJTXCheckbox->isChecked());
}

QString SetupPageUDP::getAutoLogFromWSJTx()
{
    return  util->boolToQString(logAutomaticallyWSJTXCheckbox->isChecked());
}

QString SetupPageUDP::getReaDataFromWSJTx()
{
    return util->boolToQString(realDataFromWSJTXCheckbox->isChecked());
}

void SetupPageUDP::setLogFromWSJTx(const bool _t)
{
    logFromWSJTXCheckbox->setChecked(_t);
    slotLogFromWSJTCheckBoxClicked();
}

void SetupPageUDP::setAutoLogFromWSJTx(const bool _t)
{
    logAutomaticallyWSJTXCheckbox->setChecked(_t);
}

void SetupPageUDP::setReaDataFromWSJTx(const bool _t)
{
    realDataFromWSJTXCheckbox->setChecked(_t);
}

QString SetupPageUDP::getNetworkInterface()
{
    if (networkInterfacesComboBox->currentIndex() == 0)
    {
        return  networkInterfacesComboBox->currentText();
    }
    else
    {
        return networkInterfacesComboBox->currentText().split('-').at(1);
    }
}

void SetupPageUDP::setNetworkInterface(const QString &_t)
{
    //qDebug() << "SetupPageUDP::setNetworkInterface: " << _t;
    if (networkInterfacesComboBox->findText(_t, Qt::MatchEndsWith) >= 0)
    {
        //qDebug() << "SetupPageUDP::setNetworkInterface: found: " << _t;
       networkInterfacesComboBox->setCurrentIndex(networkInterfacesComboBox->findText(_t, Qt::MatchEndsWith));
    }
    else
    {
        //qDebug() << "SetupPageUDP::setNetworkInterface: NOT found: " << _t;
        networkInterfacesComboBox->setCurrentIndex(0);
    }
}

void SetupPageUDP::saveSettings()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("UDPServer");
    settings.setValue ("UDPServer", QVariant((UDPServerCheckBox->isChecked())));
    settings.setValue ("UDPNetworkInterface", getNetworkInterface());
    settings.setValue ("UDPServerPort", getUDPServerPort());
    settings.setValue ("LogFromWSJTX", QVariant((logFromWSJTXCheckbox->isChecked())));
    settings.setValue ("LogAutoFromWSJTX", QVariant((logAutomaticallyWSJTXCheckbox->isChecked())));
    settings.setValue ("RealTimeFromWSJTX", QVariant((realDataFromWSJTXCheckbox->isChecked())));
    settings.setValue ("InfoTimeOut", miliSecsSpinBox->value());
    settings.endGroup ();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageUDP::loadSettings()
{
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("UDPServer");
    setUDPServer (settings.value("UDPServer").toBool ());
    setNetworkInterface (settings.value("UDPNetworkInterface").toString ());
    setUDPServerPort (settings.value("UDPServerPort").toInt ());
    setUDPServer (settings.value("UDPServer").toBool ());
    setLogFromWSJTx(settings.value("LogFromWSJTX").toBool ());
    logAutomaticallyWSJTXCheckbox->setChecked(settings.value("LogAutoFromWSJTX").toBool ());
    realDataFromWSJTXCheckbox->setChecked(settings.value("RealTimeFromWSJTX").toBool ());
    setTimeout(settings.value("InfoTimeOut").toInt ());
    settings.endGroup ();
}
