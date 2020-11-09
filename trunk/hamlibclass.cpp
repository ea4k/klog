#include "hamlibclass.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


HamLibClass::HamLibClass(QObject *parent) : QObject(parent)
{
    qDebug() << "HamLibClass::HamLibClass" << endl;
    strings.clear();
    qDebug() << "HamLibClass::HamLibClass -10" << endl;
    timer = new QTimer(this);
    rigLaunched = false;
    pollInterval = 300;
    errorCount = 0;
    readOnlyMode = false;
    justEmitted = false;

    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()) );
    qDebug() << "HamLibClass::HamLibClass -20" << endl;
    clean();
    qDebug() << "HamLibClass::HamLibClass  END" << endl;
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

void HamLibClass::setPoll(const int _milsecs)
{
    if (_milsecs>0)
    {
        pollInterval = _milsecs;
    }

}
void HamLibClass::readRadio(bool _forceRead)
{
    readRadioInternal(_forceRead);
}

void HamLibClass::readRadioInternal(bool _forceRead)
{
    if (!isRunning())
    {
        return;
    }

    retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);

    if (retcode == RIG_OK)
    {
        errorCount = 0;
          //qDebug() << "HamLibClass:readRadioInternal: Freq: " << QString::number(freq) << endl;
        if ((freq_old > freq) || (freq_old < freq) || (_forceRead == true))
        {
            emit freqChanged(freq/1000000);
            freq_old = freq;
            qDebug() << "HamLibClass::readRadioInternal EMITING; " << QString::number(freq) << endl;

        }
          //qDebug() << "HamLibClass::readRadioInternal read: " << QString::number(freq) << endl;

    }
    else
    {
          //qDebug() << "HamLibClass::readRadioInternal Unable to read FREQ - Error: " << QString::number(retcode) << endl;
          //qDebug() << "HamLibClass::readRadioInternal Unable to read FREQ - Error: " << rigerror(retcode) << endl;
        stop();
    }

    retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
    if (retcode == RIG_OK)
    {
        errorCount = 0;
          //qDebug() << "HamLibClass::slotTimer: Mode: " << hamlibMode2Mode(rmode) << endl;
        if ((mode_old != rmode) || (_forceRead == true))
        {
            if (justEmitted)
            {
                justEmitted = false;
                return;
            }
            mode_old = rmode;
            emit modeChanged(hamlibMode2Mode(rmode));
            justEmitted = true;
        }
    }
    else
    {
        stop();
    }
}

void HamLibClass::slotTimer()
{
    //freq_old = freq;
    if (!isRunning())
    {
        return;
    }
    readRadioInternal(false); // We don't force the radio reading.

}

void HamLibClass::setMode(const QString &_m)
{
      //qDebug() << "HamLibClass::setMode: " << _m << endl;
    if ((!isRunning()) || (readOnlyMode))
    {
        return;
    }

    // Check if we are already in a mode that should not be changed (CWR should not be changed to CW and so on)
    retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
    QString currentMode = hamlibMode2Mode(rmode);
    if (_m == currentMode)
    {
        return;
    }

    retcode = rig_set_mode(my_rig, RIG_VFO_CURR, rig_parse_mode(_m.toLocal8Bit()), rig_passband_normal(my_rig, rig_parse_mode(_m.toLocal8Bit())));
    if (retcode != RIG_OK)
    {
        errorCount++;
          //qDebug() << "HamLibClass::setMode: ERROR: Could not set mode: " << _m << endl;
        return;

    }
    errorCount = 0;
      //qDebug() << "HamLibClass::setMode - END true " << endl;
    return;
}

rmode_t HamLibClass::mode2HamlibMode(const QString &_m)
{

    /*
    if (_m == "USB")
    {
        return RIG_MODE_USB;
    }
    else if (_m == "LSB")
    {
        return RIG_MODE_LSB;
    }
    else if (_m == "CW")
    {
        return RIG_MODE_CW;
    }
    else if (_m == "FM")
    {
        return RIG_MODE_FM;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }
    else if (_m == "")
    {
        return RIG_MODE_;
    }

    */
    return RIG_MODE_NONE;
}

bool isModeADIFMode(const QString &_m)
{
    QString _mode;
    _mode = _m.toUpper();
    if ((_mode == "AM") ||  (_mode == "CW") || (_mode == "FM") || (_mode == "LSB") ||
            (_mode == "USB") || (_mode == "RTTY") )
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString HamLibClass::hamlibMode2Mode(rmode_t _rmode)
{

    switch (_rmode)
    {
        case RIG_MODE_NONE:
            return QString();        
        case RIG_MODE_AM:
            return "AM";
        case RIG_MODE_CW:
            return "CW";
        case RIG_MODE_USB:
            return "USB";
        case RIG_MODE_LSB:
            return "LSB";
        case RIG_MODE_RTTY:
            return "RTTY";
        case RIG_MODE_FM:
            return "FM";
        case RIG_MODE_WFM:
            return "FM";
        case RIG_MODE_CWR:
            return "CW"; //TODO: Check with ADIF
        case RIG_MODE_RTTYR:
            return "RTTY"; // TODO Check
        case RIG_MODE_AMS://TODO: Check with ADIF
            return "AM";
        case RIG_MODE_PKTLSB:
            return "LSB";//TODO: Check with ADIF
        case RIG_MODE_PKTUSB:
            return "USB";//TODO: Check with ADIF
        case RIG_MODE_PKTFM:
            return "FM";//TODO: Check with ADIF
        case RIG_MODE_ECSSUSB:
            return "USB";//TODO: Check with ADIF
        case RIG_MODE_ECSSLSB:
            return "LSB";//TODO: Check with ADIF
        case RIG_MODE_FAX:
            return "FAX";
        case RIG_MODE_SAM:
            return "AM"; //TODO: Check with ADIF
        case RIG_MODE_SAL:
            return "LSB";//TODO: Check with ADIF
        case RIG_MODE_SAH:
            return "USB";//TODO: Check with ADIF
       // case RIG_MODE_DSB:
       //     return "SSB";//TODO: Check with ADIF
        case RIG_MODE_FMN:
            return "FM";//TODO: Check with ADIF
        default:
            return QString();
    }
}

void HamLibClass::stop()
{
      //qDebug() << "HamLibClass::stop" << endl;
    timer->stop();
    if (!isRunning())
    {
        return;
    }
    int errorCode = rig_close(my_rig);
      //qDebug() << "HamLibClass::stop-1" << endl;


    if (errorCode == RIG_OK)
    {
        errorCount = 0;
          //qDebug() << "HamLibClass::stop: rig_close OK" << endl;
        errorCode = rig_cleanup(my_rig);
        if (errorCode == RIG_OK)
        {
            errorCount = 0;
              //qDebug() << "HamLibClass::stop: rig_cleanUp OK" << endl;
            rigLaunched = false;
              //qDebug() << "HamLibClass::stop - true" << endl;
            return;
        }
        else
        {
            errorCount++;
              //qDebug() << "HamLibClass::stop: rig_cleanup NOK: " << QString::number(errorCode) << endl;
        }
    }
    else
    {
        errorCount++;
          //qDebug() << "HamLibClass::stop: rig_close NOK: " << QString::number(errorCode) << endl;
    }
      //qDebug() << "HamLibClass::stop - false" << endl;
    return;
}

void HamLibClass::clean()
{            
    qDebug() << "HamLibClass::Clean" << endl;
    myrig_model = 1;        //Dummy equipment
    qDebug() << "HamLibClass::Clean 0" << endl;

    bauds = 9600;
       //qDebug() << "HamLibClass::Clean - 1" << endl;
    dataBits = 8;
       //qDebug() << "HamLibClass::Clean - 2" << endl;
    stopBits = 1;
    qDebug() << "HamLibClass::Clean - 3" << endl;
    shandshake = RIG_HANDSHAKE_NONE;
    qDebug() << "HamLibClass::Clean - 4" << endl;
    sparity = RIG_PARITY_NONE;
    qDebug() << "HamLibClass::Clean - 5" << endl;

      //qDebug() << "HamLibClass::Clean - 8" << endl;
    serialPort = QString();
    sdtr = RIG_SIGNAL_UNSET;
    qDebug() << "HamLibClass::Clean - 9" << endl;
    srts = RIG_SIGNAL_UNSET;

    qDebug() << "HamLibClass::Clean - 10" << endl;
    rigLaunched = false;
    qDebug() << "HamLibClass::clean - END" << endl;
}

void HamLibClass::init(bool _active)
{
    qDebug()<< "HamLibClass::init: "  << endl;
      //qDebug() << "HamLibClass::init: " << getNameFromModelId(myrig_model) << endl;
      //qDebug() << "HamLibClass::init: " << getNameFromModelId(myrig_model) << endl;
    if (!_active)
    {
        qDebug()<< "HamLibClass::init: Stopping..."  << endl;
        stop();
        qDebug() << "HamLibClass::init: Stopped!"  << endl;
        return;
    }

    if (myrig_model == -1)
    {
        qDebug()<< "HamLibClass::init: Rig Model not valid"  << endl;
        return;
    }

    rig_set_debug(RIG_DEBUG_NONE);
    qDebug()<< "HamLibClass::init: set Debug NONE"  << endl;
    my_rig = rig_init(myrig_model);
    qDebug()<< "HamLibClass::init: set after init"  << endl;
    if (my_rig == nullptr)
    {
       qDebug()<< "HamLibClass::init: Init failed, hamlib returned fail!" << endl;
       return;
    }
    else
    {
     qDebug() << "HamLibClass::init: rig_init went OK!" << endl;
    }
    my_rig->state.rigport.type.rig = RIG_PORT_SERIAL;

    qDebug() << "HamLibClass::init: serialport2: " << serialPort.toLocal8Bit() << endl;
    strncpy (my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constData(), FILPATHLEN);
      //qDebug() << "HamLibClass::init: rigport: " << my_rig->state.rigport.pathname << endl;
    my_rig->state.rigport.parm.serial.rate = bauds;
    qDebug() << "HamLibClass::init: serial rate: " << QString::number(my_rig->state.rigport.parm.serial.rate) << endl;
    my_rig->state.rigport.parm.serial.data_bits = dataBits;
    qDebug() << "HamLibClass::init: data bits: " << QString::number(my_rig->state.rigport.parm.serial.data_bits) << endl;
    my_rig->state.rigport.parm.serial.stop_bits = stopBits;
    qDebug() << "HamLibClass::init: stop bits: " << QString::number(my_rig->state.rigport.parm.serial.stop_bits) << endl;
    my_rig->state.rigport.parm.serial.parity = sparity;

    qDebug() << "HamLibClass::init: handshake before"  << endl;
    my_rig->state.rigport.parm.serial.handshake = shandshake;
    qDebug() << "HamLibClass::init: after handshake "  << endl;
    // Config done
    retcode = rig_open(my_rig);
    qDebug() << "HamLibClass::init: retcode"  << endl;

    if (retcode != RIG_OK)
    {
        qDebug()<< "HamLibClass::init: Can't open: " << rigerror(retcode) << endl;
        errorCount++;
        rig_cleanup(my_rig);
        return ;
    }
    qDebug()<< "HamLibClass::init: Rig open!"  << endl;
    errorCount = 0;
    rigLaunched = true;
    timer->start(pollInterval);


    qDebug() << "HamLibClass::init: END TRUE" << endl;
    return;
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
    qDebug() << "HamLibClass::getRigList-01" << endl;
  // and continue...

  strings.clear();
  qDebug() << "HamLibClass::getRigList-02" << endl;
  rig_load_all_backends();
  qDebug() << "HamLibClass::getRigList-10" << endl;
  rig_list_foreach (addRigToList, this);
    //qDebug() << "HamLibClass::getRigList-11" << endl;

  strings.sort();
  qDebug() << "HamLibClass::getRigList-12" << endl;
  return strings;
 }

int HamLibClass::addRigToList (const struct rig_caps *caps, void *data)
{
    qDebug() << "HamLibClass::addRigToList" << caps->model_name << endl;
    QString name;
    qDebug() << "HamLibClass::addRigToList-10"  << endl;
    HamLibClass *r = (HamLibClass *) data;
    qDebug() << "HamLibClass::addRigToList-11"  << endl;
    name = caps->model_name;
    qDebug() << "HamLibClass::addRigToList-12"  << endl;
    r->rigName2RigId[name] = caps->rig_model; // We fill the equivalences between name & Id
    qDebug() << "HamLibClass::addRigToList-13"  << endl;
    r->rigId2RigName[caps->rig_model] = name;
    qDebug() << "HamLibClass::addRigToList-14"  << endl;
    r->strings << name;
    qDebug() << "HamLibClass::addRigToList-END"  << endl;
    return -1;                    // not 0 --> we want all rigs
}

int HamLibClass::getModelIdFromName (const QString &_name)
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

void HamLibClass::setPort(const QString &_port)
{
    qDebug() << "HamLibClass::setPort: " << _port << endl;
    serialPort = _port;
    //strncpy (my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constData(), FILPATHLEN);
    //qstrncpy(myport.pathname, serialPort.toLocal8Bit().constData(), serialPort.length()+1);
}

void HamLibClass::setSpeed(const QString &_speed)
{
    bauds = _speed.toInt();
}

void HamLibClass::setData(const QString &_data)
{
    dataBits = _data.toInt();
}

void HamLibClass::setStop(const QString &_stop)
{
    stopBits = _stop.toInt();
}

void HamLibClass::setFlow(const QString &_flow)
{

    flowControl = _flow.toUpper();

    if (flowControl == "HARDWARE")
    {        
        shandshake = RIG_HANDSHAKE_HARDWARE;
    }
    else if (flowControl == "SOFTWARE")
    {
        shandshake = RIG_HANDSHAKE_XONXOFF;
    }
    else
    {
        shandshake = RIG_HANDSHAKE_NONE;
    }
}

void HamLibClass::setParity(const QString &_parity)
{   
    parity = _parity.toUpper();
    if (parity == "EVEN")
    {
        sparity= RIG_PARITY_EVEN;
    }
    else if (parity == "ODD")
    {
        sparity = RIG_PARITY_ODD;
    }
    else if (parity == "SPACE")
    {
        sparity = RIG_PARITY_SPACE;
    }
    else if (parity == "MARK")
    {
        sparity = RIG_PARITY_MARK;
    }
    else
    {
        sparity = RIG_PARITY_NONE;
    }
}

void HamLibClass::setFreq(const double _fr)
{
    qDebug() << "HamLibClass::setFreq: " << QString::number(_fr) << endl;
    if ((!isRunning()) || (readOnlyMode))
    {
        return;
    }

    freq = _fr * 1000000;
    int retcode = rig_set_freq(my_rig, RIG_VFO_CURR, freq);
    if (retcode != RIG_OK)
    {
        qDebug() << "HamLibClass::setFreq NOK: "  << endl;
        stop();
        errorCount++;

        return;
    }
    else
    {
        errorCount = 0;
        qDebug() << "HamLibClass::setFreq OK: " << QString::number(freq) << endl;
        retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
        if (retcode == RIG_OK)
        {
            errorCount = 0;
            qDebug() << "HamLibClass::setFreq read: " << QString::number(freq) << endl;
        }
        else
        {
            stop();
            errorCount++;
            qDebug() << "HamLibClass::setFreq Unable to read FREQ" << endl;
        }
        return;
    }
}

void HamLibClass::setRTS(const QString &_state)
{

    if (shandshake == RIG_HANDSHAKE_HARDWARE)
    { // Note: An attempt to control the RTS signal in the HardwareControl mode will fail with error code
      // set to UnsupportedOperationError, because the signal is automatically controlled by the driver.
        return;
    }

    if (_state.toUpper() == "TRUE")
    {
        srts = RIG_SIGNAL_ON;
    }
    else
    {
        srts = RIG_SIGNAL_OFF;
    }
}

void HamLibClass::setDTR(const QString &_state)
{

    if (_state.toUpper() == "TRUE")
    {
        sdtr = RIG_SIGNAL_ON;
    }
    else
    {
        sdtr = RIG_SIGNAL_OFF;
    }
}

void HamLibClass::checkErrorCountAndStop()
{
    if (errorCount > 10)
    {
        stop();
    }
}

void HamLibClass::setReadOnly(const bool _r)
{
    readOnlyMode = _r;
}
