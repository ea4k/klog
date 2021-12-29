/***************************************************************************
                          setuppagehamlib.cpp  -  description
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
#include "setuppages/setuppagehamlib.h"
SetupPageHamLib::SetupPageHamLib(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
    qDebug() << Q_FUNC_INFO ;
    hamlibTestOK = false;
    hamlib = new HamLibClass();
    activateHamlibCheckBox = new QCheckBox();
    readOnlyModeCheckBox = new QCheckBox();

    tabWidget = new QTabWidget;
    serialConfigWidget = new HamLibSerialConfigWidget;    
    networkConfigWidget = new HamLibNetworkConfigWidget;

    testHamlibPushButton = new QPushButton();

    rigTypeComboBox = new QComboBox;
    pollIntervalQSpinBox = new QSpinBox;

    createUI();
    setDefaults();

    qDebug() << Q_FUNC_INFO << " - END" ;
}

void SetupPageHamLib::slotTestHamlib()
{
    qDebug() << Q_FUNC_INFO;
    hamlib->stop ();
    if ((rigTypeComboBox->currentText ().contains ("NET rigctl"))  || (rigTypeComboBox->currentText ().contains ("FLRig")))
    {
        qDebug() << Q_FUNC_INFO << " - FLRig/NetRig";
        hamlib->setNetworkPort (networkConfigWidget->getPort ());
        hamlib->setNetworkAddress (networkConfigWidget->getAddress ());
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " - Serial rig";
        hamlib->setPort (serialConfigWidget->getSerialPort ());
        hamlib->setSpeed (serialConfigWidget->getSerialBauds ());
        hamlib->setParity(serialConfigWidget->getParity ());
        hamlib->setFlow(serialConfigWidget->getFlowControl ());
        //qDebug() << Q_FUNC_INFO << ": StopBits: " << QString::number(getStopBits());
        hamlib->setStop(serialConfigWidget->getStopBits ());
        //qDebug() << Q_FUNC_INFO << " - 50";
        hamlib->setDataBits(serialConfigWidget->getDataBits ());
        //qDebug() << Q_FUNC_INFO << " - 51";
    }

    hamlib->setModelId (hamlib->getModelIdFromName (rigTypeComboBox->currentText ()));
    hamlib->setPoll (2000);
    qDebug() << Q_FUNC_INFO << " - Calling hamlib->init";
    setTestResult (hamlib->init(true));

    qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageHamLib::setTestResult(const bool _ok)
{
    //qDebug() << Q_FUNC_INFO ;
    QPalette pal = testHamlibPushButton->palette();

    if (_ok )
    {
        //qDebug() << Q_FUNC_INFO << " - OK";
        testHamlibPushButton->setText (tr("Test: OK"));
        pal.setColor(QPalette::Button, QColor(Qt::green));
        //qDebug() << Q_FUNC_INFO << " - before reading freq";
        //double freq = hamlib->getFrequency ();
        //qDebug() << Q_FUNC_INFO << " - after reading freq";
        //dataFromRigLineEdit->setText (QString::number(freq));
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - NOK";
        testHamlibPushButton->setText (tr("Test: NOK"));
        pal.setColor(QPalette::Button, QColor(Qt::red));
    }

    testHamlibPushButton->setPalette(pal);
    testHamlibPushButton->update();
    //qDebug() << Q_FUNC_INFO << " - NOK END";
}



void SetupPageHamLib::slotRadioComboBoxChanged(QString _r)
{
    //qDebug() << Q_FUNC_INFO << ": " << _r;

    if (_r == "NET rigctl")
    {
        rigctlport = 4532;
        networkConfigWidget->setPort (rigctlport);
        tabWidget->setCurrentWidget (networkConfigWidget);
        //qDebug() << Q_FUNC_INFO << ": Network radio found!" ;
        networkRadio = true;
    }
    else if (_r == "FLRig")
    {
        rigctlport = 12345;
        networkConfigWidget->setPort (rigctlport);
        tabWidget->setCurrentWidget (networkConfigWidget);
        //qDebug() << Q_FUNC_INFO << ": Network radio found!" ;
        networkRadio = true;
    }
    else
    {
        tabWidget->setCurrentWidget (serialConfigWidget);
        networkRadio = false;
    }
    setTestResult(false);
}

void SetupPageHamLib::createUI()
{
    qDebug() << Q_FUNC_INFO << " - 0";
    serialConfigWidget->createUI();
    qDebug() << Q_FUNC_INFO << " - 1";
    networkConfigWidget->createUI();
    qDebug() << Q_FUNC_INFO << " - 2";
    activateHamlibCheckBox->setText(tr("Activate HamLib"));
    activateHamlibCheckBox->setToolTip(tr("Activates the hamlib support that will enable the connection to a radio."));
    readOnlyModeCheckBox->setText(tr("Read-Only mode"));
    readOnlyModeCheckBox->setToolTip(tr("If enabled, the KLog will read Freq/Mode from the radio but will never send any command to the radio."));
    testHamlibPushButton->setText (tr("Test"));
    testHamlibPushButton->setToolTip (tr("Click to test the connection to the radio"));

    //rigTypeComboBox->clear();
    //strings.clear();
    qDebug() << Q_FUNC_INFO << " - 10";
    setRig();
    qDebug() << Q_FUNC_INFO << " - 11";
    QString pollTip = QString(tr("Defines the interval to poll the radio in msecs."));
    qDebug() << Q_FUNC_INFO << " - 12";
    pollIntervalQSpinBox->setToolTip(pollTip);
    qDebug() << Q_FUNC_INFO << " - 13";
     //showDebugLog->setMinimum(pollMin);
    qDebug() << Q_FUNC_INFO << " - 14";
     //showDebugLog->setMaximum(pollMax);
    qDebug() << Q_FUNC_INFO << " - 15";
    QLabel *pollIntervalLabel = new QLabel(tr("Poll interval"));
    pollIntervalLabel->setBuddy(rigTypeComboBox);
    pollIntervalLabel->setToolTip(pollTip);
    pollIntervalLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    pollIntervalLabel->setEnabled(true);
    qDebug() << Q_FUNC_INFO << " - 20";
    QHBoxLayout *pollIntervalLayout = new QHBoxLayout;
    pollIntervalLayout->addWidget(pollIntervalLabel);
    pollIntervalLayout->addWidget(pollIntervalQSpinBox);

    qDebug() << Q_FUNC_INFO << " - 24";

    QLabel *rigTypeLabel = new QLabel(tr("Radio"));
    rigTypeLabel->setBuddy(rigTypeComboBox);
    rigTypeLabel->setToolTip(tr("Select your rig."));
    rigTypeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    rigTypeLabel->setEnabled(true);
    qDebug() << Q_FUNC_INFO << " - 25";
    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget (rigTypeLabel);
    radioLayout->addWidget (rigTypeComboBox);
    radioLayout->addLayout (pollIntervalLayout);
    radioLayout->addWidget (testHamlibPushButton);
    //radioLayout->addWidget (dataFromRigLineEdit);
    qDebug() << Q_FUNC_INFO << " - 30";
    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(activateHamlibCheckBox);
    checkBoxLayout->addWidget(readOnlyModeCheckBox);
    qDebug() << Q_FUNC_INFO << " - 35";
    tabWidget->addTab (serialConfigWidget, tr("Serial"));
    tabWidget->addTab (networkConfigWidget, tr("Network"));

    QGridLayout *mLayout = new QGridLayout;
    //QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addLayout(checkBoxLayout, 0, 1);
    mLayout->addLayout (radioLayout, 1, 0);
    mLayout->addWidget (tabWidget, 2, 0, 2, -1);
    //mLayout->addWidget (networkConfigWidget, 2, 1);

    qDebug() << Q_FUNC_INFO << " - 199";
    setLayout(mLayout);


    //connect(serialPortComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSerialPortChanged(QString) ));
    //connect(serialBaudsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSerialSpeedChanged (int) ));
    //connect(stopBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSerialStopBitChanged (int) ));
    //connect(dataBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSerialDataBitsChanged (int) ));
    //connect(flowControlComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSerialFlowControlChanged (QString)));
    //connect(parityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSerialParityChanged (QString)));


    connect(testHamlibPushButton, SIGNAL(clicked(bool)), this, SLOT(slotTestHamlib()) );
    connect(rigTypeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotRadioComboBoxChanged(QString)) );
    qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageHamLib::setRig()
{
    qDebug() << Q_FUNC_INFO << QT_ENDL;
    // Rutine to fill the rig combo boxes
    // Do not display debug codes when load the rig's
    QStringList rigs;
    rigs.clear();
    qDebug() << Q_FUNC_INFO << " - 10" << QT_ENDL;
    rigs << hamlib->getRigList();
    qDebug() << Q_FUNC_INFO << " - rigs: " << QString::number(rigs.length())<< QT_ENDL;
    rigTypeComboBox->clear ();
    rigTypeComboBox->addItems (rigs);
    rigTypeComboBox->setCurrentIndex(0);
    //rigTypeComboBox->clear();
    //rigTypeComboBox->addItems(rigs);
    qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
}


void SetupPageHamLib::setDefaults()
{
    qDebug() << Q_FUNC_INFO;
    hamlib->initClass();

    rigctlport = 4532;
    networkRadio = false;

    rigTypeComboBox->setCurrentIndex(0);

     //showDebugLog->setValue(300);
    setTestResult(false);

    //RTSCheckBox->setChecked(false);
    //DTRCheckBox->setChecked(false);
    qDebug()  << Q_FUNC_INFO << " - END";
}

QString SetupPageHamLib::getData()
{
      //qDebug() << "SetupPageHamLib::getData" << QT_ENDL;
    QString _output;
    _output.clear();
    QString _rigType, _serialPort, _baudsSpeed;//, dataBits, stopBits, handshake, flowControlLine;

    _rigType = rigTypeComboBox->currentText ();
    _serialPort = serialConfigWidget->getSerialPort ();
    _baudsSpeed = serialConfigWidget->getSerialBauds ();

    _output.clear();
    if (activateHamlibCheckBox->isChecked())
    {
        _output = _output + "Hamlib=True;\n";
    }

    if (readOnlyModeCheckBox->isChecked())
    {
        _output = _output + "HamlibReadOnly=True;\n";
    }
    else
    {
        _output = _output + "HamlibReadOnly=False;\n";
    }

    _output = _output + "HamLibRigType=" + QString::number(hamlib->getModelIdFromName(_rigType)) + ";\n";
    _output = _output + "HamlibRigPollRate=" + QString::number(pollIntervalQSpinBox->value ()) + ";\n";
    _output = _output + "HamlibSerialPort=" + _serialPort + ";\n";
    _output = _output + "HamlibSerialBauds=" + _baudsSpeed + ";\n";
    _output = _output + "HamLibSerialDataBits=" + QString::number(getDataBits()) + ";\n";
    _output = _output + "HamLibSerialStopBits=" + QString::number(getStopBits()) + ";\n";

    _output = _output + getFlowControl() + ";\n";
    _output = _output + getParity() + ";\n";

    if ((networkConfigWidget->getAddress ()).length()>1)
    {
        _output = _output + "HamlibNetAddress=" + networkConfigWidget->getAddress () + ";\n";
        _output = _output + "HamlibNetPort=" + QString::number(networkConfigWidget->getPort ()) + ";\n";
    }
    return _output;
}

bool SetupPageHamLib::setRigType(const QString &_radio)
{
    int _index = rigTypeComboBox->findText(hamlib->getNameFromModelId(_radio.toInt()), Qt::MatchFlag::MatchExactly);
      //qDebug() << "SetupPageHamLib::setRig: After: "  << QString::number(_index)  << QT_ENDL;
    if (_index >= 0)
    {
        rigTypeComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        rigTypeComboBox->setCurrentIndex(0);
    }
    return false;
}

bool SetupPageHamLib::setSerialPort(const QString &_port)
{
    return serialConfigWidget->setSerialPort (_port);
}

bool SetupPageHamLib::setSerialSpeed(const int _speed )
{
    return serialConfigWidget->setSerialBauds (_speed);
}

void SetupPageHamLib::setActive(const QString &_active)
{
      //qDebug() << "SetupPageHamLib::setActive: " << _active << QT_ENDL;

    if (_active.toUpper() == "TRUE")
    {
        activateHamlibCheckBox->setChecked(true);
    }
    else
    {
       activateHamlibCheckBox->setChecked(false);
    }
}

void SetupPageHamLib::setReadOnly(const QString &_m)
{    
    if (_m.toUpper() == "TRUE")
    {
        readOnlyModeCheckBox->setChecked(true);
    }
    else
    {
        readOnlyModeCheckBox->setChecked(false);
    }
}

int SetupPageHamLib::getDataBits()
{
    return serialConfigWidget->getDataBits ();
}

void SetupPageHamLib::setDataBits(const int _st)
{
    serialConfigWidget->setDataBits (_st);
}

QString SetupPageHamLib::getFlowControl()
{
    return serialConfigWidget->getFlowControl ();
}

void SetupPageHamLib::setFlowControl(const QString &_st)
{
    serialConfigWidget->setFlowControl (_st);
}

QString SetupPageHamLib::getParity()
{
    return serialConfigWidget->getParity ();
}

void SetupPageHamLib::setParity(const QString &_st)
{
    serialConfigWidget->setParity(_st);
}

int SetupPageHamLib::getStopBits()
{
    return serialConfigWidget->getStopBits ();
}

void SetupPageHamLib::setStopBits(const QString &_st)
{
    serialConfigWidget->setStopBits (_st);
}

void SetupPageHamLib::setPollingInterval(const int _msecs)
{
    pollIntervalQSpinBox->setValue(_msecs);
}

void SetupPageHamLib::setRadioNetworkAddress(const QString &_m)
{
    if (_m.length ()>0)
    {
        networkConfigWidget->setAddress (_m);
    }
}

void SetupPageHamLib::setRadioNetworkPort(const int _p)
{
    networkConfigWidget->setPort (_p);
}



