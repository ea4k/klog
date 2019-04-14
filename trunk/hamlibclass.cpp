#include "hamlibclass.h"


HamLibClass::HamLibClass(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()) );
    //timer->start(1000);
    clean();
}

void HamLibClass::slotTimer()
{
    //freq_old = freq;
    int retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
    if (retcode == RIG_OK)
    {
        if (freq_old != freq)
        {
            emit freqChanged(freq/1000000);
            freq_old = freq;
            qDebug() << "HamLibClass::slotTimer EMITING; " << QString::number(freq) << endl;
        }
        //qDebug() << "HamLibClass::slotTimer read: " << QString::number(freq) << endl;

    }
    else
    {
        qDebug() << "HamLibClass::slotTimer Unable to read FREQ" << endl;
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
    myrig_model = 1;        //Dummy equipment
    rigLaunched = false;
    parity = QSerialPort::NoParity;
    flowControl = QSerialPort::NoFlowControl;
    dataBits = QSerialPort::Data8;
    stopBits = QSerialPort::OneStop;
    bauds = QSerialPort::Baud9600;
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
    return rigId2RigName[_id];
}

void HamLibClass::setModelId(const int _id)
{
    myrig_model = _id;
}

void HamLibClass::setPort(const QString _port)
{
    serialPort = _port;
    qstrncpy(myport.pathname, serialPort.toLocal8Bit().constData(), serialPort.length());
}

void HamLibClass::setSpeed(const int _speed)
{
    bauds = _speed;
    switch (_speed)
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
        break;
        case QSerialPort::Baud115200:
            myport.parm.serial.rate = 115200;
        break;
        default:
            myport.parm.serial.rate = 9600;
        break;
    }
}

void HamLibClass::setPatity(const int _parity)
{
    parity = _parity;
    switch (_parity)
    {
        case QSerialPort::NoParity:
            myport.parm.serial.parity = RIG_PARITY_NONE;
        break;
        case QSerialPort::EvenParity:
            myport.parm.serial.parity = RIG_PARITY_EVEN;
        break;
        case QSerialPort::OddParity:
            myport.parm.serial.parity = RIG_PARITY_ODD;
        break;
        case QSerialPort::SpaceParity:
            myport.parm.serial.parity = RIG_PARITY_SPACE;
        break;
        case QSerialPort::MarkParity:
            myport.parm.serial.parity = RIG_PARITY_MARK;
        break;
        default:
            myport.parm.serial.parity = RIG_PARITY_NONE;
        break;
    }
}

void HamLibClass::setFlow(const int _flow)
{
    flowControl = _flow;
    switch (_flow)
    {
        case QSerialPort::QSerialPort::NoFlowControl:
            myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
        break;
        case QSerialPort::HardwareControl:
            myport.parm.serial.handshake = RIG_HANDSHAKE_HARDWARE;
        break;
        case QSerialPort::SoftwareControl:
            myport.parm.serial.handshake = RIG_HANDSHAKE_XONXOFF;
        break;
        default:
            myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
        break;
    }

}

void HamLibClass::setStop(const int _stop)
{
    stopBits = _stop;

    if ((_stop >=1) || (_stop<=3))
    {
        myport.parm.serial.data_bits = _stop;
    }
    else
    {
        myport.parm.serial.data_bits = 1;
    }

}

void HamLibClass::setData(const int _data)
{
    dataBits = _data;
    if ((_data >=5) || (_data<=8))
    {
        myport.parm.serial.data_bits = _data;
    }
    else
    {
        myport.parm.serial.data_bits = 8;
    }


}

bool HamLibClass::init()
{
    qDebug() << "HamLibClass::init: " << endl;
    if (myrig_model == -1)
    {
        return false;
    }
    if (!rigLaunched)
    {
        rig_set_debug(RIG_DEBUG_NONE);
        qDebug() << "HamLibClass::init: " << endl;
        myport.type.rig = RIG_PORT_SERIAL;
        qDebug() << "HamLibClass::init: 1" << endl;
        //myport.parm.serial.rate = bauds;
        //myport.parm.serial.data_bits = dataBits;
        //myport.parm.serial.stop_bits = stopBits;
        qDebug() << "HamLibClass::init: 2" << endl;
        //TODO: CHECK PARITY
        //myport.parm.serial.parity = RIG_PARITY_NONE;
        //TODO: CHECK HANDSHAKE
        //myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
        //qstrncpy(myport.pathname, serialPort.toLocal8Bit().constData(), serialPort.length());
        //serialPort = myport.pathname;
        qDebug() << "HamLibClass::init: 3" << endl;
        my_rig = rig_init((myrig_model));
        if (!my_rig)
        {
            rigLaunched = false;
            qDebug() << "HamLibClass::init: END FALSE1" << endl;
            return false;
        }
        qDebug() << "HamLibClass::init: 4" << endl;
        //qstrncpy(my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constEnd(), serialPort.length());
        //serialPort = my_rig->state.rigport.pathname;
        int recode = rig_open(my_rig);

        qDebug() << "HamLibClass::init: 5" << endl;
        if (retcode != RIG_OK)
        {
            rigLaunched = false;
            qDebug() << "HamLibClass::init: Error: " << QString::number(retcode) << "-" << rigerror(retcode);
            qDebug() << "HamLibClass::init: END FALSE2" << endl;
            return false;
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

bool HamLibClass::setFreq(const double _fr)
{
    qDebug() << "HamLibClass::setFreq: " << QString::number(_fr) << endl;

    freq = _fr * 1000000;
    int retcode = rig_set_freq(my_rig, RIG_VFO_CURR, freq);
    if (retcode != RIG_OK)
    {
        qDebug() << "HamLibClass::setFreq NOK: "  << endl;

        return false;
    }
    else
    {
        qDebug() << "HamLibClass::setFreq OK: " << QString::number(freq) << endl;
        retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
        if (retcode == RIG_OK)
        {
            qDebug() << "HamLibClass::setFreq read: " << QString::number(freq) << endl;
        }
        else
        {
            qDebug() << "HamLibClass::setFreq Unable to read FREQ" << endl;
        }
        return true;
    }

}



