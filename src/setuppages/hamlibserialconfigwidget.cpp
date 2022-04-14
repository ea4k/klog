#include "hamlibserialconfigwidget.h"

HamLibSerialConfigWidget::HamLibSerialConfigWidget(QWidget *parent) : QWidget(parent)
{
    //qDebug() << Q_FUNC_INFO;
    serialBaudsComboBox = new QComboBox();
    serialPortComboBox = new QComboBox();
    dataBitsComboBox = new QComboBox();
    flowControlComboBox = new QComboBox();
    parityComboBox = new QComboBox();
    stopBitsComboBox = new QComboBox();
    scanSerialPortButton = new QPushButton;
    //qDebug() << Q_FUNC_INFO << " - END";
}

void HamLibSerialConfigWidget::createUI()
{
    //qDebug() << Q_FUNC_INFO ;
    QLabel *serialPortLabel = new QLabel();
    //qDebug() << Q_FUNC_INFO << " - 10";
    serialPortComboBox->addItems(serialPorts);
    serialPortLabel->setBuddy(serialPortComboBox);
    serialPortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *serialBaudsLabel = new QLabel(tr("Bauds"));
    serialBaudsLabel->setBuddy(serialBaudsComboBox);
    serialBaudsLabel->setToolTip(tr("Select the serial port speed."));
    serialBaudsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //qDebug() << Q_FUNC_INFO << " - 50";

    serialPortLabel->setText (tr("Port"));
    //qDebug() << Q_FUNC_INFO << " - 52";
    serialPortLabel->setToolTip(tr("Select the serial port. Only the serial ports that are detected are shown."));
    //qDebug() << Q_FUNC_INFO << " - 53";
    serialPortLabel->setEnabled(true);
    //qDebug() << Q_FUNC_INFO << " - 54";
    scanSerialPortButton->setText(tr("Scan"));
    //qDebug() << Q_FUNC_INFO << " - 55";
    scanSerialPortButton->setToolTip(tr("Click to identify the serial ports available in your computer."));
    //qDebug() << Q_FUNC_INFO << " - 56";
    QStringList strings;
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
    setLayout (serialLayout);

    fillSerialPortsComboBox();

    connect(scanSerialPortButton, SIGNAL(clicked(bool)), this, SLOT(slotScanPorts()) );
    //qDebug() << Q_FUNC_INFO << " - END";
}


QStringList HamLibSerialConfigWidget::getAvailableSerialPorts()
{
    //qDebug() << Q_FUNC_INFO;
    QStringList st;
    st.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        //st << info.portName();
        st << info.systemLocation();
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    return st;
}

void HamLibSerialConfigWidget::fillSerialPortsComboBox()
{
    //qDebug() << "SetupPageHamLib::fillSerialPortsComboBox" ;
    serialPortComboBox->clear();
    serialPortComboBox->addItems(getAvailableSerialPorts());
    serialPortComboBox->setCurrentIndex(0);
}

void HamLibSerialConfigWidget::slotScanPorts()
{
    //qDebug() << Q_FUNC_INFO;
    fillSerialPortsComboBox();
}

int HamLibSerialConfigWidget::getDataBits()
{
    return ((dataBitsComboBox->currentText()).left(1)).toInt();
}

void HamLibSerialConfigWidget::setDataBits (const int _b)
{
    switch (_b)
    {
        case 5:
            dataBitsComboBox->setCurrentIndex(0);
        break;
        case 6:
            dataBitsComboBox->setCurrentIndex(1);
            break;
        case 7:
            dataBitsComboBox->setCurrentIndex(2);
            break;
        default:
        // should never be reached
            dataBitsComboBox->setCurrentIndex(3);
        break;
    }
}

//FlowControl { NoFlowControl, HardwareControl, SoftwareControl, UnknownFlowControl }
QString HamLibSerialConfigWidget::getFlowControl()
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

void HamLibSerialConfigWidget::setFlowControl(const QString &_st)
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
QString HamLibSerialConfigWidget::getParity()
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

void HamLibSerialConfigWidget::setParity(const QString &_st)
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
QString HamLibSerialConfigWidget::getStopBits()
{

   int ret = stopBitsComboBox->currentIndex();
    switch (ret)
    {
        case 0:
            return "OneStop";
            //return QSerialPort::OneStop;
        break;
        case 1:
            return "OneAndHalfStop";
            //return QSerialPort::OneAndHalfStop;
            break;
        case 2:
            return "TwoStop";
            //return QSerialPort::TwoStop;
            break;
          default:
        // should never be reached
            return "UnknownStopBits";
        break;
    }

}

void HamLibSerialConfigWidget::setStopBits(const QString &_st)
{


    if (_st == "OneStop")
    {
        stopBitsComboBox->setCurrentIndex(0);
    }
    else if (_st == "OneAndHalfStop")
    {
        stopBitsComboBox->setCurrentIndex(1);
    }
    else if (_st == "TwoStop")
    {
        stopBitsComboBox->setCurrentIndex(2);
    }
    else
    {
       stopBitsComboBox->setCurrentIndex(0);
    }
}

bool HamLibSerialConfigWidget::setSerialPort(const QString &_port)
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

bool HamLibSerialConfigWidget::setSerialBauds(const int _speed )
{
    int _index = serialBaudsComboBox->findText(QString::number(_speed), Qt::MatchFlag::MatchExactly);
    if (_index >= 0)
    {
        serialBaudsComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        serialBaudsComboBox->setCurrentIndex(0);
    }
    return false;
}

QString HamLibSerialConfigWidget::getSerialPort()
{
    return serialPortComboBox->currentText ();
}

int HamLibSerialConfigWidget::getSerialBauds()
{
    return (serialBaudsComboBox->currentText ()).toInt ();
}


