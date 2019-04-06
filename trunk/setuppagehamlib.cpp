#include "setuppagehamlib.h"

SetupPageHamLib::SetupPageHamLib(DataProxy *dp, QWidget *parent) : QWidget(parent)
{
    //qDebug() << "SetupPageHamLib::SetupPageHamLib" << endl;

    rigTypeComboBox = new QComboBox();
    serialBaudsComboBox = new QComboBox();
    serialPortComboBox = new QComboBox();

    dataBits7CheckBox = new QCheckBox();
    dataBits8CheckBox = new QCheckBox();
    stopBits1CheckBox = new QCheckBox();
    stopBits2CheckBox = new QCheckBox();
    handshakeNoneCheckBox = new QCheckBox();
    handshakeXCheckBox = new QCheckBox();
    handshakeHCheckBox = new QCheckBox();
    flowControlLinesDTRCheckBox = new QCheckBox();
    flowControlLinesRTSCheckBox = new QCheckBox();

    //serialBaudsSpinBox = new QSpinBox;

    serialPort = new QLineEdit;

    //defaultPortSpeed = 9600;
    baudSpeeds.clear();
    baudSpeeds << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";


    createUI();
    setDefaults();

    qDebug() << "SetupPageHamLib::SetupPageHamLib END" << endl;
}

void SetupPageHamLib::createUI()
{
    rigTypeComboBox->clear();
    strings.clear();
    setRig();
    serialPortComboBox->addItems(getAvailableSerialPorts());

    rigTypeComboBox->setCurrentIndex(0);

    QLabel *rigTypeLabel = new QLabel(tr("Radio"));
    rigTypeLabel->setBuddy(rigTypeComboBox);
    rigTypeLabel->setToolTip(tr("Select your rig."));
    rigTypeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    rigTypeLabel->setEnabled(true);

    serialPortComboBox->addItems(serialPorts);
    QLabel *serialPortLabel = new QLabel(tr("Port"));
    serialPortLabel->setBuddy(serialPortComboBox);
    serialPortLabel->setToolTip(tr("Select the serial port. Only the serial ports that are detected are shown."));
    serialPortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    serialPortLabel->setEnabled(true);

    serialBaudsComboBox->addItems(baudSpeeds);
    QLabel *serialBaudsLabel = new QLabel(tr("Bauds"));
    serialBaudsLabel->setBuddy(serialBaudsComboBox);
    serialBaudsLabel->setToolTip(tr("Select the serial port speed."));
    serialBaudsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    serialBaudsLabel->setEnabled(true);

    QGridLayout *topData = new QGridLayout;
    topData->addWidget(rigTypeLabel, 0, 0);
    topData->addWidget(rigTypeComboBox, 0, 1);
    topData->addWidget(serialPortLabel, 1, 0);
    topData->addWidget(serialPortComboBox, 1, 1);
    topData->addWidget(serialBaudsLabel, 2, 0);
    topData->addWidget(serialBaudsComboBox, 2, 1);
/*
    QGroupBox *dataBitsGroupBox = new QGroupBox (tr("Data Bits"));
    dataBits7CheckBox->setText(tr("7 bits"));
    dataBits8CheckBox->setText(tr("8 bits"));

    dataBitsGroupButton = new QButtonGroup(this);
    dataBitsGroupButton->setExclusive(true);
    dataBitsGroupButton->addButton(dataBits7CheckBox);
    dataBitsGroupButton->addButton(dataBits8CheckBox);
    QVBoxLayout *dataBitsButtonsLayout = new QVBoxLayout;
    dataBitsButtonsLayout->addWidget(dataBits7CheckBox);
    dataBitsButtonsLayout->addWidget(dataBits8CheckBox);
    dataBitsGroupBox->setLayout(dataBitsButtonsLayout);

    QGroupBox *stopBitsGroupBox = new QGroupBox(tr("Stop bits"));
    stopBits1CheckBox->setText(tr("1 bit"));
    stopBits2CheckBox->setText(tr("2 bits"));

    stopBitsButtonGroup = new QButtonGroup(this);
    stopBitsButtonGroup->setExclusive(true);
    stopBitsButtonGroup->addButton(stopBits1CheckBox);
    stopBitsButtonGroup->addButton(stopBits2CheckBox);
    QVBoxLayout *stopBitsButtonsLayout = new QVBoxLayout;
    stopBitsButtonsLayout->addWidget(stopBits1CheckBox);
    stopBitsButtonsLayout->addWidget(stopBits2CheckBox);
    stopBitsGroupBox->setLayout(stopBitsButtonsLayout);

    QGroupBox *handShakeGroupBox = new QGroupBox(tr("Handshake"));
    handshakeNoneCheckBox->setText(tr("None"));
    handshakeXCheckBox->setText(tr("XON/XOFF"));
    handshakeHCheckBox->setText(tr("Hardware"));

    handshakeButtonGroup = new QButtonGroup(this);
    handshakeButtonGroup->setExclusive(true);
    handshakeButtonGroup->addButton(handshakeNoneCheckBox);
    handshakeButtonGroup->addButton(handshakeXCheckBox);
    handshakeButtonGroup->addButton(handshakeHCheckBox);
    QVBoxLayout *handShakeButtonsLayout = new QVBoxLayout;
    handShakeButtonsLayout->addWidget(handshakeNoneCheckBox);
    handShakeButtonsLayout->addWidget(handshakeXCheckBox);
    handShakeButtonsLayout->addWidget(handshakeHCheckBox);
    handShakeGroupBox->setLayout(handShakeButtonsLayout);

    QGroupBox *flowControlLineGroupBox = new QGroupBox(tr("Control Lines"));
    flowControlLinesDTRCheckBox->setText(tr("DTR"));
    flowControlLinesRTSCheckBox->setText(tr("RTS"));

    flowControlLineButtonGroup = new QButtonGroup;
    flowControlLineButtonGroup->setExclusive(true);
    flowControlLineButtonGroup->addButton(flowControlLinesDTRCheckBox);
    flowControlLineButtonGroup->addButton(flowControlLinesRTSCheckBox);
    QVBoxLayout *flowControlLineButtonsLayout = new QVBoxLayout;
    flowControlLineButtonsLayout->addWidget(flowControlLinesDTRCheckBox);
    flowControlLineButtonsLayout->addWidget(flowControlLinesRTSCheckBox);
    flowControlLineGroupBox->setLayout(flowControlLineButtonsLayout);
*/
    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addLayout(topData, 0, 0);
    //mainLayout->addWidget(dataBitsGroupBox, 1, 0);
    //mainLayout->addWidget(stopBitsGroupBox, 1, 1);
    //mainLayout->addWidget(handShakeGroupBox, 2, 0);
    //mainLayout->addWidget(flowControlLineGroupBox, 2, 1);

    setLayout(mainLayout);
}

void SetupPageHamLib::setRig ()
{
    qDebug() << "SetupPageHamLib::SetRig" << endl;
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's
  rig_set_debug (RIG_DEBUG_NONE);

  // and continue...
  rig_load_all_backends ();
  rig_list_foreach (addRigToList, this);
  strings.sort ();
  rigTypeComboBox->insertItems (0, strings);
  strings.clear ();
}

int SetupPageHamLib::addRigToList (const struct rig_caps *caps, void *data)
{
    qDebug() << "SetupPageHamLib::addRigToList" << endl;
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
    rigTypeComboBox->setCurrentIndex(0);
    serialPortComboBox->setCurrentIndex(0);
    serialBaudsComboBox->setCurrentIndex(0);

    //dataBits8CheckBox->setChecked(true);
    //stopBits1CheckBox->setChecked(true);
    //handshakeNoneCheckBox->setChecked(true);

}

QString SetupPageHamLib::getData()
{
    qDebug() << "SetupPageHamLib::getData" << endl;
    QString _output;
    _output.clear();
    QString _rigType, _serialPort, _baudsSpeed;//, dataBits, stopBits, handshake, flowControlLine;

    _rigType = rigTypeComboBox->currentText();
    _serialPort = serialPortComboBox->currentText();
    _baudsSpeed = serialBaudsComboBox->currentText();

     qDebug() << "SetupPageHamLib::getData: " << _rigType << endl;
/*
    if (dataBits7CheckBox->isChecked())
    {
        dataBits = "7;";
    }
    else
    {
        dataBits = "8;";
    }

    if (stopBits1CheckBox->isChecked())
    {
        stopBits = "1;";
    }
    else
    {
        stopBits = "2;";
    }

    if (handshakeHCheckBox->isChecked())
    {
        handshake = "Hardware;";
    }
    else if (handshakeXCheckBox->isChecked())
    {
        handshake = "XON;";
    }
    else
    {
        handshake = "None;";
    }

    if (flowControlLinesDTRCheckBox->isChecked())
    {
        flowControlLine = "DRT;";
    }
    else if (flowControlLinesRTSCheckBox->isChecked())
    {
        flowControlLine ="RTS;";
    }
    else
    {
        flowControlLine = "None";
    }
*/
    _output.clear();
    _output = _output + "HamLibRigType=" + _rigType + "\n";
    _output = _output + "HamlibSerialPort=" + _serialPort + "\n";
    _output = _output + "HamlibSerialBauds=" + _baudsSpeed + "\n";

    //_output + "SerialDataBits=" + dataBits + "\n";
    //_output + "SerialStopBits=" + stopBits + "\n";
    //_output + "SerialHandShake=" + handshake + "\n";
    //_output + "SerialFlowControlLine=" + flowControlLine + "\n";

    qDebug() << "SetupPageHamLib::getData: " << _output << endl;
    return _output;
}

bool SetupPageHamLib::setRigType(const QString _radio)
{
    int _index = rigTypeComboBox->findText(_radio, Qt::MatchFlag::MatchExactly);
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

bool SetupPageHamLib::setSerialPort(const QString _port)
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

bool SetupPageHamLib::setSerialSpeed(const QString _speed )
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
