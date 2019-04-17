#include "hamlibclass.h"


HamLibClass::HamLibClass(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    //m_serial = new QSerialPort();

    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()) );

    //connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    //connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    //timer->start(1000);
    clean();
}

HamLibClass::~HamLibClass()
{
    if (rigLaunched)
    {
        rig_close(my_rig);
        rig_cleanup(my_rig);
        rigLaunched = false;
    }
}

void HamLibClass::slotTimer()
{
    //freq_old = freq;
    int retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);


    if (retcode == RIG_OK)
    {
        qDebug() << "HamLibClass::slotTimer: Freq: " << QString::number(freq) << endl;
        if (freq_old != freq)
        {
            emit freqChanged(freq/1000000);
            freq_old = freq;
            qDebug() << "HamLibClass::slotTimer EMITING; " << QString::number(freq) << endl;

        }
        qDebug() << "HamLibClass::slotTimer read: " << QString::number(freq) << endl;

    }
    else
    {
        qDebug() << "HamLibClass::slotTimer Unable to read FREQ - Error: " << QString::number(retcode) << endl;
        qDebug() << "HamLibClass::slotTimer Unable to read FREQ - Error: " << rigerror(retcode) << endl;
    }

}

bool HamLibClass::stop()
{
    qDebug() << "HamLibClass::stop" << endl;
    int errorCode = rig_close(my_rig);
    qDebug() << "HamLibClass::stop-1" << endl;
    timer->stop();

    if (errorCode == RIG_OK)
    {
        qDebug() << "HamLibClass::stop: rig_close OK" << endl;
        errorCode = rig_cleanup(my_rig);
        if (errorCode == RIG_OK)
        {
            qDebug() << "HamLibClass::stop: rig_cleanUp OK" << endl;
            rigLaunched = false;
            qDebug() << "HamLibClass::stop - true" << endl;
            return true;
        }
        else
        {
            qDebug() << "HamLibClass::stop: rig_cleanup NOK: " << QString::number(errorCode) << endl;
        }
    }
    else
    {
        qDebug() << "HamLibClass::stop: rig_close NOK: " << QString::number(errorCode) << endl;
    }
    qDebug() << "HamLibClass::stop - false" << endl;
    return false;
}

void HamLibClass::clean()
{            
    myrig_model = -1;        //Dummy equipment
    myport.parm.serial.rate = 57600;
    myport.parm.serial.data_bits = 8;
    myport.parm.serial.stop_bits = 1;
    myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
    myport.parm.serial.parity = RIG_PARITY_NONE;
    myport.type.rig = RIG_PORT_SERIAL;
    myport.parm.serial.dtr_state = RIG_SIGNAL_OFF;
    myport.parm.serial.rts_state = RIG_SIGNAL_OFF;
    serialPort = QString();

    rigLaunched = false;

}

bool HamLibClass::init()
{
    qDebug() << "HamLibClass::init: " << QString::number(myrig_model) << endl;
    //qDebug() << "HamLibClass::init: " << getNameFromModelId(myrig_model) << endl;
    //qDebug() << "HamLibClass::init: " << getNameFromModelId(myrig_model) << endl;

    if (myrig_model == -1)
    {
        return false;
    }
    stop();

    if (!rigLaunched)
    {
        rig_set_debug(RIG_DEBUG_NONE);
        qDebug() << "HamLibClass::init: 1" << endl;

        //qstrncpy(myport.pathname, serialPort.toLocal8Bit().constData(), serialPort.length());
        //serialPort = myport.pathname;
        qDebug() << "HamLibClass::init: 3 (pathname): " << myport.pathname << endl;

        rig_load_all_backends();
        my_rig = rig_init(myrig_model);
        if (!my_rig)
        {
            rigLaunched = false;
            qDebug() << "HamLibClass::init: END FALSE1" << endl;
            return false;
        }
        //qstrncpy(myport.pathname, serialPort.toLocal8Bit(), FILPATHLEN - 1);
        qDebug() << "HamLibClass::init: 4 - SerialPort: " << serialPort << endl;
        qstrncpy(my_rig->state.rigport.pathname,serialPort.toLocal8Bit(),FILPATHLEN - 1);
        qDebug() << "HamLibClass::init: 4 - SerialPort-2: " << my_rig->state.rigport.pathname << endl;

        //qstrncpy(my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constEnd(), serialPort.length());
        //qstrncpy(my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constEnd(), serialPort.length());
        //serialPort = my_rig->state.rigport.pathname;
        int retcode = rig_open(my_rig);

        qDebug() << "HamLibClass::init: 5 - " << endl;
        if (retcode != RIG_OK)
        {
            rigLaunched = false;
            qDebug() << "HamLibClass::init: Error: " << QString::number(retcode) << "-" << rigerror(retcode);
            qDebug() << "HamLibClass::init: END FALSE2" << endl;
            return false;
        }
        else
        {
            qDebug() << "HamLibClass::init: RADIO OPENED" << endl;
        }
        rigLaunched = true;
        timer->start(1000);
    }
    else
    {
        qDebug() << "HamLibClass::init: Rig was already launched" << endl;
    }
    qDebug() << "HamLibClass::init: END TRUE" << endl;
    return true;
}



bool HamLibClass::isRunning()
{
    return rigLaunched;
}

QStringList HamLibClass::getRigList ()
{
    qDebug() << "HamLibClass::getRigList" << endl;
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's
  rig_set_debug (RIG_DEBUG_NONE);

  // and continue...

  strings.clear();
  rig_load_all_backends ();
  rig_list_foreach (addRigToList, this);

  strings.sort ();
  return strings;
 }

int HamLibClass::addRigToList (const struct rig_caps *caps, void *data)
{
  qDebug() << "HamLibClass::addRigToList" << endl;
  QString name;
  HamLibClass *r = (HamLibClass *) data;
  name = caps->model_name;

  r->rigName2RigId[name] = caps->rig_model; // We fill the equivalences between name & Id
  r->rigId2RigName[caps->rig_model] = name;
  r->strings << name;
  return -1;                    // not 0 --> we want all rigs
}

int HamLibClass::getModelIdFromName (const QString _name)
{
   //HamLibClass *r (HamLibClass *) data;
   int i = -1;
   i = rigName2RigId[_name];
   return i;
}

QString HamLibClass::getNameFromModelId(const int _id)
{
    qDebug() << "HamLibClass::getNameFromModelId: " << QString::number(_id) << "/" << rigId2RigName.value(_id)<< endl;

    return rigId2RigName.value(_id);
}

void HamLibClass::setModelId(const int _id)
{
    qDebug() << "HamLibClass::setModelId: " << QString::number(_id) << endl;
    myrig_model = _id;
}

void HamLibClass::setPort(const QString _port)
{
    qDebug() << "HamLibClass::setPort: " << _port << endl;
    serialPort = _port;
    qstrncpy(myport.pathname, serialPort.toLocal8Bit().constData(), serialPort.length()+1);
}

void HamLibClass::setSpeed(const QString _speed)
{
    bauds = _speed.toInt();
    switch (bauds)
    {
        case QSerialPort::Baud1200:
            myport.parm.serial.rate = 1200;
        break;
        case QSerialPort::Baud2400:
            myport.parm.serial.rate = 2400;
        break;
        case QSerialPort::Baud4800:
            myport.parm.serial.rate = 4800;
        break;
        case QSerialPort::Baud9600:
            myport.parm.serial.rate = 9600;
        break;
        case QSerialPort::Baud19200:
            myport.parm.serial.rate = 19200;
        break;
        case QSerialPort::Baud38400:
            myport.parm.serial.rate = 38400;
        break;
        case QSerialPort::Baud57600:
            myport.parm.serial.rate = 57600;
            qDebug() << "HamLibClass::setSpeed 57600" << endl;
        break;
        case QSerialPort::Baud115200:
            myport.parm.serial.rate = 115200;
        break;
        default:
            myport.parm.serial.rate = 9600;
        break;
    }
}

void HamLibClass::setData(const QString _data)
{
    dataBits = _data.toInt();
    if ((dataBits >=5) || (dataBits<=8))
    {
        myport.parm.serial.data_bits = dataBits;
    }
    else
    {
        myport.parm.serial.data_bits = 8;
    }
}
void HamLibClass::setStop(const QString _stop)
{
    stopBits = _stop.toInt();

    if ((stopBits >=1) || (stopBits<=3))
    {
        myport.parm.serial.stop_bits = stopBits;
    }
    else
    {
        myport.parm.serial.stop_bits = 1;
    }
}

void HamLibClass::setFlow(const QString _flow)
{

    flowControl = _flow.toUpper();

    if (flowControl == "HARDWARE")
    {
        myport.parm.serial.handshake = RIG_HANDSHAKE_HARDWARE;
    }
    else if (flowControl == "SOFTWARE")
    {
        myport.parm.serial.handshake = RIG_HANDSHAKE_XONXOFF;
    }
    else
    {
        myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
    }
}

void HamLibClass::setParity(const QString _parity)
{   
    parity = _parity.toUpper();
    if (parity == "EVEN")
    {
        myport.parm.serial.parity = RIG_PARITY_EVEN;
    }
    else if (parity == "ODD")
    {
        myport.parm.serial.parity = RIG_PARITY_ODD;
    }
    else if (parity == "SPACE")
    {
        myport.parm.serial.parity = RIG_PARITY_SPACE;
    }
    else if (parity == "MARK")
    {
        myport.parm.serial.parity = RIG_PARITY_MARK;
    }
    else
    {
        myport.parm.serial.parity = RIG_PARITY_NONE;
    }
}


bool HamLibClass::setFreq(const double _fr)
{
    //qDebug() << "HamLibClass::setFreq: " << QString::number(_fr) << endl;

    freq = _fr * 1000000;
    int retcode = rig_set_freq(my_rig, RIG_VFO_CURR, freq);
    if (retcode != RIG_OK)
    {
        //qDebug() << "HamLibClass::setFreq NOK: "  << endl;

        return false;
    }
    else
    {
        //qDebug() << "HamLibClass::setFreq OK: " << QString::number(freq) << endl;
        retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
        if (retcode == RIG_OK)
        {
            //qDebug() << "HamLibClass::setFreq read: " << QString::number(freq) << endl;
        }
        else
        {
            //qDebug() << "HamLibClass::setFreq Unable to read FREQ" << endl;
        }
        return true;
    }
}
/*
bool HamLibClass::openSerialPort()
{
   qDebug() << "HamLibClass::openSerialPort: Opening: " << serialPort << endl;
   closeSerialPort();
    m_serial->setPortName(serialPort);
    m_serial->setBaudRate(bauds);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::HardwareControl);
    if (m_serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "HamLibClass::openSerialPort: OPEN OK: " << serialPort << endl;
        return true;
    }
    else
    {

        qDebug() << "HamLibClass::openSerialPort: OPEN ERROR: " << m_serial->errorString() << endl;
        return false;
    }
}


bool HamLibClass::closeSerialPort()
{
    qDebug() << "HamLibClass::closeSerialPort" << endl;
    if (m_serial->isOpen())
        m_serial->close();
    return true;
}
*/

void HamLibClass::setRTS(const QString _state)
{
    if (myport.parm.serial.handshake == RIG_HANDSHAKE_HARDWARE)
    { // Note: An attempt to control the RTS signal in the HardwareControl mode will fail with error code
      // set to UnsupportedOperationError, because the signal is automatically controlled by the driver.
        return;
    }

    if (_state.toUpper() == "TRUE")
    {
        myport.parm.serial.rts_state = RIG_SIGNAL_ON;
    }
    else
    {
           myport.parm.serial.rts_state = RIG_SIGNAL_OFF;
    }
}

void HamLibClass::setDTR(const QString _state)
{
    if (_state.toUpper() == "TRUE")
    {
        myport.parm.serial.dtr_state = RIG_SIGNAL_ON;
    }
    else
    {
           myport.parm.serial.dtr_state = RIG_SIGNAL_OFF;
    }

}



