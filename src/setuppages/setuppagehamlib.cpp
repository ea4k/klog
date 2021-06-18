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
      //qDebug() << "SetupPageHamLib::SetupPageHamLib" << endl;
    ready = false;
    hamlib = new HamLibClass();
    activateHamlibCheckBox = new QCheckBox();
    readOnlyModeCheckBox = new QCheckBox();
    rigTypeComboBox = new QComboBox();
    pollIntervalQSpinBox = new QSpinBox();
    serialBaudsComboBox = new QComboBox();
    serialPortComboBox = new QComboBox();

    dataBitsComboBox = new QComboBox();
    flowControlComboBox = new QComboBox();
    parityComboBox = new QComboBox();
    stopBitsComboBox = new QComboBox();
    hostAddressLineEdit = new QLineEdit();

    portQSpinBox = new QSpinBox;

    serialGroup = new QGroupBox;
    networkGroup = new QGroupBox;

    //RTSCheckBox = new QCheckBox();
    //DTRCheckBox = new QCheckBox();

    scanSerialPortButton = new QPushButton();

    //serialBaudsSpinBox = new QSpinBox;

    serialPort = new QLineEdit;

   strings.clear();
   //strings << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    pollMax = 5000;
    pollMin = 10;
    rigctlport = 4532;
    networkRadio = false;
    connect(scanSerialPortButton, SIGNAL(clicked(bool)), this, SLOT(slotScanPorts()) );
    connect(rigTypeComboBox, SIGNAL(currentIndexChanged (QString)), this, SLOT(slotRadioComboBoxChanged(QString)) );

    createUI();
    setDefaults();
    ready = true;
    //qDebug() << "SetupPageHamLib::SetupPageHamLib END" << endl;
}

void SetupPageHamLib::fillSerialPortsComboBox()
{
      //qDebug() << "SetupPageHamLib::fillSerialPortsComboBox" << endl;
    serialPortComboBox->clear();
    serialPortComboBox->addItems(getAvailableSerialPorts());
    //serialPortComboBox->setCurrentIndex(0);
}

void SetupPageHamLib::slotRadioComboBoxChanged(QString _r)
{
    //qDebug() << Q_FUNC_INFO << ": " << _r;

    if (_r == "NET rigctl")
    {
        serialGroup->setEnabled (false);
        networkGroup->setEnabled (true);
        //qDebug() << Q_FUNC_INFO << ": Network radio found!" ;
        networkRadio = true;
    }
    else
    {
        serialGroup->setEnabled (true);
        networkGroup->setEnabled (false);
        networkRadio = false;
    }
}

void SetupPageHamLib::createUI()
{
    //qDebug() << Q_FUNC_INFO;

    activateHamlibCheckBox->setText(tr("Activate HamLib"));
    activateHamlibCheckBox->setToolTip(tr("Activates the hamlib support that will enable the connection to a radio."));
    readOnlyModeCheckBox->setText(tr("Read-Only mode"));
    readOnlyModeCheckBox->setToolTip(tr("If enabled, the KLog will read Freq/Mode from the radio but will never send any command to the radio."));

    hostAddressLineEdit->setToolTip (tr("Enter the hostname or address of the radio."));
    portQSpinBox->setToolTip (tr("Set de network port of the radio."));

    serialGroup->setTitle (tr("Serial radio"));
    networkGroup->setTitle (tr("Network radio"));

    portQSpinBox->setMaximum (65535);
    portQSpinBox->setMinimum (0);
    portQSpinBox->setValue (rigctlport);


    rigTypeComboBox->clear();
    strings.clear();
    setRig();
    //qDebug() << Q_FUNC_INFO << " - 10";

    fillSerialPortsComboBox();
    //qDebug() << Q_FUNC_INFO << " - 11";

    rigTypeComboBox->setCurrentIndex(0);
    //qDebug() << Q_FUNC_INFO << " - 12";

    QLabel *rigTypeLabel = new QLabel(tr("Radio"));
    rigTypeLabel->setBuddy(rigTypeComboBox);
    rigTypeLabel->setToolTip(tr("Select your rig."));
    rigTypeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    rigTypeLabel->setEnabled(true);
    //qDebug() << Q_FUNC_INFO << " - 30";

    QString pollTip = QString(tr("Defines the interval to poll the radio in msecs."));
    pollIntervalQSpinBox->setToolTip(pollTip);
    pollIntervalQSpinBox->setMinimum(pollMin);
    pollIntervalQSpinBox->setMaximum(pollMax);

    QLabel *pollIntervalLabel = new QLabel(tr("Poll interval"));
    pollIntervalLabel->setBuddy(rigTypeComboBox);
    pollIntervalLabel->setToolTip(pollTip);
    pollIntervalLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    pollIntervalLabel->setEnabled(true);

    QHBoxLayout *pollIntervalLayout = new QHBoxLayout;
    pollIntervalLayout->addWidget(pollIntervalLabel);
    pollIntervalLayout->addWidget(pollIntervalQSpinBox);

    QLabel *serialPortLabel = new QLabel();
    //qDebug() << Q_FUNC_INFO << " - 40";
    serialPortComboBox->addItems(serialPorts);
    serialPortLabel->setBuddy(serialPortComboBox);
    serialPortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *serialBaudsLabel = new QLabel(tr("Bauds"));
    serialBaudsLabel->setBuddy(serialBaudsComboBox);
    serialBaudsLabel->setToolTip(tr("Select the serial port speed."));
    serialBaudsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //qDebug() << Q_FUNC_INFO << " - 50";

    serialPortLabel->setText (tr("Port"));
    serialPortLabel->setToolTip(tr("Select the serial port. Only the serial ports that are detected are shown."));
    serialPortLabel->setEnabled(true);
    scanSerialPortButton->setText(tr("Scan"));
    scanSerialPortButton->setToolTip(tr("Click to identify the serial ports available in your computer."));
    //qDebug() << Q_FUNC_INFO << " - 56";
    strings.clear();
    strings << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    serialBaudsComboBox->addItems(strings);
    serialBaudsLabel->setEnabled(true);

    //qDebug() << Q_FUNC_INFO << " - 60";

    strings.clear();
    strings << tr("5 bits") << tr("6 bits") << tr("7 bits") << tr("8 bits");
    dataBitsComboBox->addItems(strings);
    QLabel *dataBitsLabel = new QLabel(tr("Data bits"));
    dataBitsLabel->setBuddy(dataBitsComboBox);
    dataBitsLabel->setToolTip(tr("Select the serial data bits."));
    dataBitsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    dataBitsLabel->setEnabled(true);

    //qDebug() << Q_FUNC_INFO << " - 70";
    strings.clear();
    strings << tr("None") << tr("Hardware") << tr("Software XON/XOFF");
    flowControlComboBox->addItems(strings);
    QLabel *flowControlLabel = new QLabel(tr("Flow control"));
    flowControlLabel->setBuddy(flowControlComboBox);
    flowControlLabel->setToolTip(tr("Select the serial flow control"));
    flowControlLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    flowControlLabel->setEnabled(true);

    //qDebug() << Q_FUNC_INFO << " - 80";
    strings.clear();
    strings << tr("No parity") << tr("Even") << tr("Odd") << tr("Space") << tr("Mark") ;
    parityComboBox->addItems(strings);
    QLabel *parityLabel = new QLabel(tr("Parity"));
    parityLabel->setBuddy(parityComboBox);
    parityLabel->setToolTip(tr("Select the serial parity."));
    parityLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    parityLabel->setEnabled(true);

    //qDebug() << Q_FUNC_INFO << " - 90";
    strings.clear();
    strings << tr("1 bit") << tr("1.5 bits") << tr("2 bits");
    stopBitsComboBox->addItems(strings);
    QLabel *stopBitsLabel = new QLabel(tr("Stop bits"));
    stopBitsLabel->setBuddy(stopBitsComboBox);
    stopBitsLabel->setToolTip(tr("Select the serial stop bits."));
    stopBitsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    stopBitsLabel->setEnabled(true);
    //qDebug() << Q_FUNC_INFO << " - 100";

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget (rigTypeLabel);
    radioLayout->addWidget (rigTypeComboBox);
    radioLayout->addLayout (pollIntervalLayout);

    QGridLayout *serialLayout = new QGridLayout;
    serialLayout->addWidget(serialPortLabel, 1, 0);
    serialLayout->addWidget(serialPortComboBox, 1, 1);
    serialLayout->addWidget(scanSerialPortButton, 1, 2);
    serialLayout->addWidget(serialBaudsLabel, 2, 0);
    serialLayout->addWidget(serialBaudsComboBox, 2, 1);
    serialLayout->addWidget(dataBitsLabel, 3, 0);
    serialLayout->addWidget(dataBitsComboBox, 3, 1);
    serialLayout->addWidget(stopBitsLabel, 4, 0);
    serialLayout->addWidget(stopBitsComboBox, 4, 1);
    serialLayout->addWidget(flowControlLabel, 5, 0);
    serialLayout->addWidget(flowControlComboBox, 5, 1);
    serialLayout->addWidget(parityLabel, 6, 0);
    serialLayout->addWidget(parityComboBox, 6, 1);
    serialGroup->setLayout (serialLayout);

    QLabel *hostAddLabel = new QLabel(tr("Host/Address"));
    hostAddLabel->setBuddy(hostAddressLineEdit);
    hostAddLabel->setToolTip(tr("Enter the hostname or address of the radio."));
    hostAddLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    hostAddLabel->setEnabled(true);

    QLabel *portNetLabel = new QLabel(tr("Port"));
    portNetLabel->setBuddy(portQSpinBox);
    portNetLabel->setToolTip(tr("Enter the port of the radio."));
    portNetLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    portNetLabel->setEnabled(true);

    QGridLayout *networkLayout = new QGridLayout;
    //qDebug() << Q_FUNC_INFO << " - 120";
    networkLayout->addWidget (hostAddLabel, 0, 0);
    networkLayout->addWidget (hostAddressLineEdit, 0, 1);
    networkLayout->addWidget (portNetLabel, 1, 0);
    networkLayout->addWidget (portQSpinBox, 1, 1);

    networkGroup->setLayout (networkLayout);

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(activateHamlibCheckBox);
    checkBoxLayout->addWidget(readOnlyModeCheckBox);

    QGridLayout *mLayout = new QGridLayout;
    //QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addLayout(checkBoxLayout, 0, 1);
    mLayout->addLayout (radioLayout, 1, 0);
    mLayout->addWidget (serialGroup, 2, 0);
    mLayout->addWidget (networkGroup, 2, 1);

    //qDebug() << Q_FUNC_INFO << " - 199";
    setLayout(mLayout);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageHamLib::setRig()
{
   //qDebug()() << "SetupPageHamLib::SetRig" << endl;
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's
    QStringList rigs;
    rigs.clear();
    rigs << hamlib->getRigList();
   //qDebug()() << "SetupPageHamLib::SetRig - rigs: " << QString::number(rigs.length()) << endl;
    rigTypeComboBox->clear();
    rigTypeComboBox->addItems(rigs);
    //qDebug() << "SetupPageHamLib::SetRig - rigs: " << QString::number(rigs.length()) << endl;
    //rigTypeComboBox->insertItems(0, rigs);
/*
    rig_set_debug (RIG_DEBUG_NONE);

  // and continue...
  rig_load_all_backends ();
  rig_list_foreach (addRigToList, this);
  strings.sort ();
  rigTypeComboBox->insertItems (0, strings);
  strings.clear ();
  */
   //qDebug()() << "SetupPageHamLib::SetRig - END" << endl;
}
/*
int SetupPageHamLib::addRigToList (const struct rig_caps *caps, void *data)
{
      //qDebug() << "SetupPageHamLib::addRigToList" << endl;
  QString name;
  SetupPageHamLib *r = (SetupPageHamLib *) data;
  name = caps->model_name;
    //qDebug() << "SetupPageHamLib::addRigToList: " << name << endl;
  //if (name == "Dummy")
  //  name = "None";
  //r->rigname2rigid[name] = caps->rig_model;
  //r->rigid2rigname[caps->rig_model] = name;
  r->strings << name;
  return -1;                    // not 0 --> we want all rigs
}
*/

QStringList SetupPageHamLib::getAvailableSerialPorts()
{
    QStringList st;
    st.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        //st << info.portName();
        st << info.systemLocation();
    }
    return st;

}

void SetupPageHamLib::setDefaults()
{
    //qDebug() << Q_FUNC_INFO;
    rigTypeComboBox->setCurrentIndex(0);
    serialPortComboBox->setCurrentIndex(0);
    serialBaudsComboBox->setCurrentIndex(0);
    dataBitsComboBox->setCurrentIndex(3);
    flowControlComboBox->setCurrentIndex(0);
    parityComboBox->setCurrentIndex(0);
    stopBitsComboBox->setCurrentIndex(0);
    pollIntervalQSpinBox->setValue(300);

    //RTSCheckBox->setChecked(false);
    //DTRCheckBox->setChecked(false);
    //qDebug()  << Q_FUNC_INFO << " - END";
}

QString SetupPageHamLib::getData()
{
      //qDebug() << "SetupPageHamLib::getData" << endl;
    QString _output;
    _output.clear();
    QString _rigType, _serialPort, _baudsSpeed;//, dataBits, stopBits, handshake, flowControlLine;

    _output.clear();
    if (activateHamlibCheckBox->isChecked())
    {
        _output = _output + "Hamlib=True;\n";
    }
    else
    {

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
    _output = _output + "HamlibRigPollRate=" + QString::number(pollIntervalQSpinBox->value()) + ";\n";
    _output = _output + "HamlibSerialPort=" + _serialPort + ";\n";
    _output = _output + "HamlibSerialBauds=" + _baudsSpeed + ";\n";
    _output = _output + getDataBits() + ";\n";
    _output = _output + getStopBits() + ";\n";
    _output = _output + getFlowControl() + ";\n";
    _output = _output + getParity() + ";\n";

    if (hostAddressLineEdit->text ().length()>1)
    {
        _output = _output + "HamlibNetAddress=" + hostAddressLineEdit->text() + ";\n";
        _output = _output + "HamlibNetPort=" + QString::number(portQSpinBox->value()) + ";\n";
    }
    return _output;
}

bool SetupPageHamLib::setRigType(const QString &_radio)
{
      //qDebug() << "SetupPageHamLib::setRig: " << _radio << endl;

    int _index = rigTypeComboBox->findText(hamlib->getNameFromModelId(_radio.toInt()), Qt::MatchFlag::MatchExactly);
      //qDebug() << "SetupPageHamLib::setRig: After: "  << QString::number(_index)  << endl;
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
    int _index = serialPortComboBox->findText(_port, Qt::MatchFlag::MatchExactly);
    if (_index >= 0)
    {
        serialPortComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        serialPortComboBox->setCurrentIndex(0);
    }
    return false;
}

bool SetupPageHamLib::setSerialSpeed(const QString &_speed )
{
    int _index = serialBaudsComboBox->findText(_speed, Qt::MatchFlag::MatchExactly);
    if (_index >= 0)
    {
        serialBaudsComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        serialPortComboBox->setCurrentIndex(0);
    }
    return false;
}

void SetupPageHamLib::setActive(const QString &_active)
{
      //qDebug() << "SetupPageHamLib::setActive: " << _active << endl;

    if (_active.toUpper() == "TRUE")
    {
        activateHamlibCheckBox->setChecked(true);
    }
    else {
       activateHamlibCheckBox->setChecked(false);
    }
}

void SetupPageHamLib::setReadOnly(const QString &_m)
{
    if (_m.toUpper() == "TRUE")
    {
        readOnlyModeCheckBox->setChecked(true);
    }
    else {
        readOnlyModeCheckBox->setChecked(false);
    }
}

void SetupPageHamLib::slotScanPorts()
{
      //qDebug() << "SetupPageHamLib::slotScanPorts"  << endl;
    fillSerialPortsComboBox();
}

//DataBits { Data5, Data6, Data7, Data8, UnknownDataBits }
QString SetupPageHamLib::getDataBits()
{
      //qDebug() << "SetupPageHamLib::getDataBits"  << endl;
    int ret = dataBitsComboBox->currentIndex();
    QString output;
    output = "HamLibSerialDataBits=";
    switch (ret)
    {
        case 0:
            output = output + "5";
              //qDebug() << "SetupPageHamLib::getDataBits-0-5"  << endl;
        break;
        case 1:
            output = output + "6";
              //qDebug() << "SetupPageHamLib::getDataBits-1-6"  << endl;
        break;
        case 2:
            output = output + "7";
        break;
              //qDebug() << "SetupPageHamLib::getDataBits-2-7"  << endl;
        case 3:
            output = output + "8";
              //qDebug() << "SetupPageHamLib::getDataBits-3-8"  << endl;
        break;
        default:
        // should never be reached
              //qDebug() << "SetupPageHamLib::getDataBits-d-8"  << endl;
            output = output + "8";
        break;
    }
    return output;
}

void SetupPageHamLib::setDataBits(const QString &_st)
{
    if (_st == "5")
    {
        dataBitsComboBox->setCurrentIndex(0);
    }
    else if (_st == "6")
    {
        dataBitsComboBox->setCurrentIndex(1);
    }
    else if (_st == "7")
    {
        dataBitsComboBox->setCurrentIndex(2);
    }
    else
    {
        dataBitsComboBox->setCurrentIndex(3);
    }
}

//FlowControl { NoFlowControl, HardwareControl, SoftwareControl, UnknownFlowControl }
QString SetupPageHamLib::getFlowControl()
{
    int ret = flowControlComboBox->currentIndex();
    QString output;
    output = "HamLibSerialFlowControl=";
    switch (ret)
    {
        case 0:
            output = output + "None";
        break;
        case 1:
            output = output + "Hardware";
            break;
        case 2:
            output = output + "Software";
            break;
        default:
        // should never be reached
            output = output + "None";
        break;
    }
    return output;
}
void SetupPageHamLib::setFlowControl(const QString &_st)
{
    QString _s = _st.toUpper();
    if (_s == "HARDWARE")
    {
        flowControlComboBox->setCurrentIndex(1);
    }
    else if (_s == "SOFTWARE")
    {
        flowControlComboBox->setCurrentIndex(2);
    }
    else
    {
        flowControlComboBox->setCurrentIndex(0);
    }
}

//Parity { NoParity, EvenParity, OddParity, SpaceParity, MarkParity, UnknownParity }
QString SetupPageHamLib::getParity()
{
    int ret = parityComboBox->currentIndex();
    QString output;
    output = "HamLibSerialParity=";
    switch (ret)
    {
        case 0:
            output = output + "None";
        break;
        case 1:
            output = output + "Even";
            break;
        case 2:
            output = output + "Odd";
            break;
        case 3:
            output = output + "Space";
            break;
        case 4:
            output = output + "Mark";
            break;
        default:
        // should never be reached
            output = output + "None";
        break;
    }
    return output;
}
void SetupPageHamLib::setParity(const QString &_st)
{
    QString _s = _st.toUpper();
    if (_s == "EVEN")
    {
        parityComboBox->setCurrentIndex(1);
    }
    else if (_s == "ODD")
    {
        parityComboBox->setCurrentIndex(2);
    }
    else if (_s == "SPACE")
    {
        parityComboBox->setCurrentIndex(3);
    }
    else if (_s == "MARK")
    {
        parityComboBox->setCurrentIndex(4);
    }
    else
    {
        parityComboBox->setCurrentIndex(0);
    }
}

//StopBits { OneStop, OneAndHalfStop, TwoStop, UnknownStopBits }

QString SetupPageHamLib::getStopBits()
{
    int ret = stopBitsComboBox->currentIndex();
    QString output;
    output = "HamLibSerialStopBits=";
    switch (ret)
    {
        case 0:
            output = output + "1";
        break;
        case 1:
            output = output + "1.5";
            break;
        case 2:
            output = output + "2";
            break;

        default:
        // should never be reached
            output = output + "1";
        break;
    }
    return output;
}
void SetupPageHamLib::setStopBits(const QString &_st)
{
    if (_st == "1.5")
    {

        stopBitsComboBox->setCurrentIndex(1);
    }
    else if (_st == "2")
    {
        stopBitsComboBox->setCurrentIndex(2);
    }
    else
    {
        stopBitsComboBox->setCurrentIndex(0);
    }
}
void SetupPageHamLib::setPollingInterval(const int _msecs)
{
    if ((_msecs>=pollMin) && (_msecs<=pollMax))
    {
        pollIntervalQSpinBox->setValue(_msecs);
    }
}

void SetupPageHamLib::setRadioNetworkAddress(const QString &_m)
{
    if (_m.length ()>0)
    {
        hostAddressLineEdit->setText (_m);
    }

}

void SetupPageHamLib::setRadioNetworkPort(const int _p)
{
    if ((_p>0) && (_p<65535))
    {
        portQSpinBox->setValue (_p);
    }
}


