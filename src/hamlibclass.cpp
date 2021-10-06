/***************************************************************************
                          hamlibclass.cpp  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
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

#include "hamlibclass.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


HamLibClass::HamLibClass(QObject *parent) : QObject(parent)
{
     //qDebug() << "HamLibClass::HamLibClass" << QT_ENDL;
    strings.clear();
     //qDebug() << "HamLibClass::HamLibClass -10" << QT_ENDL;
    timer = new QTimer(this);
    rigLaunched = false;
    pollInterval = 300;
    errorCount = 0;
    readOnlyMode = false;
    justEmitted = false;
    freq_old = 0.0;
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()) );
     //qDebug() << "HamLibClass::HamLibClass -20" << QT_ENDL;
    clean();
     //qDebug() << "HamLibClass::HamLibClass  END" << QT_ENDL;
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

bool HamLibClass::readRadio(bool _forceRead)
{
    return readRadioInternal(_forceRead);
}

bool HamLibClass::readRadioInternal(bool _forceRead)
{
     //qDebug() << Q_FUNC_INFO << QT_ENDL;
    if (!isRunning())
    {
         //qDebug() << Q_FUNC_INFO << ": isn't running" << QT_ENDL;
        return false;
    }

    retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);

    if (retcode == RIG_OK)
    {
         //qDebug() << Q_FUNC_INFO << ": RIG OK for Freq" << QT_ENDL;
        errorCount = 0;
         //qDebug() << "HamLibClass:readRadioInternal: Freq: " << QString::number(freq) << QT_ENDL;
        if ((freq_old > freq) || (freq_old < freq) || (_forceRead == true))
        {
            emit freqChanged(freq/1000000);
            freq_old = freq;
             //qDebug() << "HamLibClass::readRadioInternal EMITING; " << QString::number(freq) << QT_ENDL;
        }
         //qDebug() << "HamLibClass::readRadioInternal read: " << QString::number(freq) << QT_ENDL;
    }
    else
    {
        return errorManage(retcode);
    }

    retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
    if (retcode == RIG_OK)
    {
         //qDebug() << Q_FUNC_INFO << ": RIG OK for Mode" << QT_ENDL;
        errorCount = 0;
            //qDebug() << "HamLibClass::slotTimer: Mode: " << hamlibMode2Mode(rmode) << QT_ENDL;
        if ((mode_old != rmode) || (_forceRead == true))
        {
            if (justEmitted)
            {
                justEmitted = false;
                return true;
            }
            mode_old = rmode;
            emit modeChanged(hamlibMode2Mode(rmode));
            justEmitted = true;
        }
    }
    else
    {
        return errorManage (retcode);
    }

    return true;
}

void HamLibClass::slotTimer()
{
     //qDebug() << Q_FUNC_INFO << QT_ENDL;
    if (!isRunning())
    {
         //qDebug() << Q_FUNC_INFO << ": Isn't running..." << QT_ENDL;
        return;
    }
    readRadioInternal(false); // We don't force the radio reading.
     //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
}

void HamLibClass::setMode(const QString &_m)
{
        //qDebug() << "HamLibClass::setMode: " << _m << QT_ENDL;
    if ((!isRunning()) || (readOnlyMode))
    {
        return;
    }

    // Check if we are already in a mode that should not be changed (CWR should not be changed to CW and so on)
    retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
    if (RIG_OK != retcode)
    {
        //qDebug() << "HamLibClass::setMode: ERROR: Could not get mode: " << QT_ENDL;
        errorManage (retcode);
    }
    QString currentMode = hamlibMode2Mode(rmode);
    if (_m == currentMode)
    {
        return;
    }

    retcode = rig_set_mode(my_rig, RIG_VFO_CURR, rig_parse_mode(_m.toLocal8Bit()), rig_passband_normal(my_rig, rig_parse_mode(_m.toLocal8Bit())));

    if (RIG_OK != retcode)
    {
        //qDebug() << "HamLibClass::setMode: ERROR: Could not set mode: " << _m << QT_ENDL;
        errorManage (retcode);
    }


    errorCount = 0;
        //qDebug() << "HamLibClass::setMode - END true " << QT_ENDL;
    return;
}
 /*
rmode_t HamLibClass::mode2HamlibMode(const QString &_m)
{


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


    return RIG_MODE_NONE;
}
   */

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
    #if defined(Q_OS_OPENBSD)
    #else
    case RIG_MODE_FMN:
        return "FM"; //TODO: Check with ADIF
    #endif
    default:
        return QString();
    }
}

bool HamLibClass::stop()
{
     //qDebug() << "HamLibClass::stop" << QT_ENDL;
    timer->stop();
    if (!isRunning())
    {
        return true;
    }
    int errorCode = rig_close(my_rig);
        //qDebug() << "HamLibClass::stop-1" << QT_ENDL;


    if (errorCode == RIG_OK)
    {
        errorCount = 0;
            //qDebug() << "HamLibClass::stop: rig_close OK" << QT_ENDL;
        errorCode = rig_cleanup(my_rig);
        if (errorCode == RIG_OK)
        {
            errorCount = 0;
                //qDebug() << "HamLibClass::stop: rig_cleanUp OK" << QT_ENDL;
            rigLaunched = false;
                //qDebug() << "HamLibClass::stop - true" << QT_ENDL;
            return true;
        }
        else
        {
            //return errorManage (retcode);
            errorCount++;
                //qDebug() << "HamLibClass::stop: rig_cleanup NOK: " << QString::number(errorCode) << QT_ENDL;
        }
    }
    else
    {
        errorCount++;
            //qDebug() << "HamLibClass::stop: rig_close NOK: " << QString::number(errorCode) << QT_ENDL;
    }
        //qDebug() << "HamLibClass::stop - false" << QT_ENDL;
    return false;
}

void HamLibClass::clean()
{
     //qDebug() << "HamLibClass::Clean" << QT_ENDL;
    myrig_model = 1;        //Dummy equipment
     //qDebug() << "HamLibClass::Clean 0" << QT_ENDL;

    bauds = 9600;
         //qDebug() << "HamLibClass::Clean - 1" << QT_ENDL;
    dataBits = 8;
         //qDebug() << "HamLibClass::Clean - 2" << QT_ENDL;
    stopBits = 1;
     //qDebug() << "HamLibClass::Clean - 3" << QT_ENDL;
    shandshake = RIG_HANDSHAKE_NONE;
     //qDebug() << "HamLibClass::Clean - 4" << QT_ENDL;
    sparity = RIG_PARITY_NONE;
     //qDebug() << "HamLibClass::Clean - 5" << QT_ENDL;

        //qDebug() << "HamLibClass::Clean - 8" << QT_ENDL;
    serialPort = QString();
    sdtr = RIG_SIGNAL_UNSET;
     //qDebug() << "HamLibClass::Clean - 9" << QT_ENDL;
    srts = RIG_SIGNAL_UNSET;
    networkPort = 4532;
    networkAddress = "127.0.0.1";

     //qDebug() << "HamLibClass::Clean - 10" << QT_ENDL;
    rigLaunched = false;
     //qDebug() << "HamLibClass::clean - END" << QT_ENDL;
}

bool HamLibClass::init(bool _active)
{
     //qDebug()<< "HamLibClass::init: "  << QT_ENDL;
     //qDebug() << "HamLibClass::init: " << getNameFromModelId(myrig_model) << QT_ENDL;

    if (!_active)
    {
        //qDebug()<< "HamLibClass::init: Stopping..."  << QT_ENDL;
        //qDebug() << Q_FUNC_INFO << ": Calling stop";
        return stop();
        //qDebug() << "HamLibClass::init: Stopped!"  << QT_ENDL;
    }

    if (myrig_model < 0)
    {
        //qDebug()<< "HamLibClass::init: Rig Model not valid"  << QT_ENDL;
        return false;
    }

    rig_set_debug(RIG_DEBUG_NONE);
    //qDebug()<< "HamLibClass::init: set Debug NONE"  << QT_ENDL;
    my_rig = rig_init(myrig_model);
    //qDebug()<< "HamLibClass::init: set after init"  << QT_ENDL;
    if (my_rig == nullptr)
    {
       //qDebug()<< "HamLibClass::init: Init failed, hamlib returned fail!" << QT_ENDL;
       return false;
    }
    else
    {
        //qDebug() << "HamLibClass::init: rig_init went OK!" << QT_ENDL;
    }
    // Code of DG1VS (Thank you!)
    if (myrig_model == RIG_MODEL_NETRIGCTL)
    {
        //qDebug() << "HamLibClass::init: RIG_PORT_NETWORK" << QT_ENDL;
        // network based communication
        my_rig->state.rigport.type.rig = RIG_PORT_NETWORK;
        QString netAddPort = QString("%1:%2").arg (networkAddress).arg(networkPort);
        qstrncpy (my_rig->state.rigport.pathname, netAddPort.toLocal8Bit().constData(), FILPATHLEN);

        // the other stuff is hardcoded in hamlib!
    }
    else if (myrig_model == RIG_MODEL_FLRIG)
    {
        //qDebug() << "HamLibClass::init: RIG_PORT_RPC" << QT_ENDL;
        //my_rig->state.rigport.type.rig = RIG_PORT_RPC;
        my_rig->state.rigport.type.rig = RIG_PORT_NETWORK;
        QString netAddPort = QString("%1:%2").arg (networkAddress).arg(networkPort);
        //qDebug() << "HamLibClass::init: " << netAddPort << QT_ENDL;
        qstrncpy (my_rig->state.rigport.pathname, netAddPort.toLocal8Bit().constData(), FILPATHLEN);
    }
    else
    {
        //qDebug() << "HamLibClass::init: !RIG_PORT_NETWORK" << QT_ENDL;
        //qDebug() << "HamLibClass::init: serialport2: " << serialPort.toLocal8Bit() << QT_ENDL;
        my_rig->state.rigport.type.rig = RIG_PORT_SERIAL;
        //strncpy (my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constData(), FILPATHLEN);
        qstrncpy (my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constData(), FILPATHLEN);


         //qDebug() << "HamLibClass::init: rigport: " << my_rig->state.rigport.pathname << QT_ENDL;
        my_rig->state.rigport.parm.serial.rate = bauds;
         //qDebug() << "HamLibClass::init: serial rate: " << QString::number(my_rig->state.rigport.parm.serial.rate) << QT_ENDL;
        my_rig->state.rigport.parm.serial.data_bits = dataBits;
         //qDebug() << "HamLibClass::init: data bits: " << QString::number(my_rig->state.rigport.parm.serial.data_bits) << QT_ENDL;
        my_rig->state.rigport.parm.serial.stop_bits = stopBits;
         //qDebug() << "HamLibClass::init: stop bits: " << QString::number(my_rig->state.rigport.parm.serial.stop_bits) << QT_ENDL;
        my_rig->state.rigport.parm.serial.parity = sparity;
         //qDebug() << "HamLibClass::init: handshake before"  << QT_ENDL;
        my_rig->state.rigport.parm.serial.handshake = shandshake;
         //qDebug() << "HamLibClass::init: after handshake "  << QT_ENDL;
        // Config done
    }

    //qDebug() << "HamLibClass::init: after handshake "  << QT_ENDL;
    // Config done
    retcode = rig_open(my_rig);
    //qDebug() << "HamLibClass::init: retcode"  << QT_ENDL;

    if (retcode != RIG_OK)
    {
        //qDebug()<< "HamLibClass::init: Can't open: " << rigerror(retcode) << QT_ENDL;

        rig_cleanup(my_rig);
        return errorManage (retcode);
    }

    //qDebug()<< "HamLibClass::init: Rig open!"  << QT_ENDL;
    errorCount = 0;
    rigLaunched = true;
    freq_old = 0.0;
    timer->start(pollInterval);

    //qDebug() << "HamLibClass::init: END TRUE" << QT_ENDL;
    return true;
}

bool HamLibClass::isRunning()
{
    return rigLaunched;
}

QStringList HamLibClass::getRigList ()
{
    //qDebug()<< "HamLibClass::getRigList: StringsList before filling it: " << QT_ENDL;
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's
  rig_set_debug (RIG_DEBUG_NONE);
     //qDebug() << "HamLibClass::getRigList-01" << QT_ENDL;
  // and continue...

  strings.clear();
  //qDebug()<< "HamLibClass::getRigList: StringsList after cleaning it: " << QT_ENDL;
   //qDebug() << "HamLibClass::getRigList-02" << QT_ENDL;
  rig_load_all_backends();
   //qDebug() << "HamLibClass::getRigList-10" << QT_ENDL;
  rig_list_foreach (addRigToList, this);
     //qDebug() << "HamLibClass::getRigList-11" << QT_ENDL;

  strings.sort();
  //qDebug()<< "HamLibClass::getRigList-12 - Strings length: " << QString::number(strings.length()) << QT_ENDL;
  return strings;
 }

int HamLibClass::addRigToList (const struct rig_caps *caps, void *data)
{
    //qDebug()<< "HamLibClass::addRigToList" << caps->model_name << QT_ENDL;
    QString name;
    //qDebug()<< "HamLibClass::addRigToList-10"  << QT_ENDL;
    //HamLibClass *r = (HamLibClass *) data;
    HamLibClass *r = static_cast<HamLibClass *> (data);

    //qDebug()<< "HamLibClass::addRigToList-11"  << QT_ENDL;
    name = caps->model_name;
    //qDebug()<< "HamLibClass::addRigToList-12"  << QT_ENDL;
    r->rigName2RigId[name] = caps->rig_model; // We fill the equivalences between name & Id
    //qDebug()<< "HamLibClass::addRigToList-13"  << QT_ENDL;
    r->rigId2RigName[caps->rig_model] = name;
    //qDebug()<< "HamLibClass::addRigToList-14"  << QT_ENDL;
    r->strings << name;
    //qDebug()<< "HamLibClass::addRigToList-END"  << QT_ENDL;
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
    //qDebug() << "HamLibClass::getNameFromModelId: " << QString::number(_id) << "/" << rigId2RigName.value(_id)<< QT_ENDL;

    return rigId2RigName.value(_id);
}

void HamLibClass::setModelId(const int _id)
{
     //qDebug() << "HamLibClass::setModelId: " << QString::number(_id) << QT_ENDL;
    myrig_model = _id;
}

void HamLibClass::setPort(const QString &_port)
{
     //qDebug() << "HamLibClass::setPort: " << _port << QT_ENDL;
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
     //qDebug() << "HamLibClass::setFreq: " << QString::number(_fr) << QT_ENDL;
    if ((!isRunning()) || (readOnlyMode))
    {
        return;
    }

    freq = _fr * 1000000;
    int retcode = rig_set_freq(my_rig, RIG_VFO_CURR, freq);
    if (retcode != RIG_OK)
    {
        errorManage (retcode);
    }
    else
    {
        errorCount = 0;
         //qDebug() << "HamLibClass::setFreq OK: " << QString::number(freq) << QT_ENDL;
        retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
        if (retcode == RIG_OK)
        {
            errorCount = 0;
             //qDebug() << "HamLibClass::setFreq read: " << QString::number(freq) << QT_ENDL;
        }
        else
        {
            errorManage (retcode);
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
     //qDebug() << Q_FUNC_INFO;
    if (errorCount > 10)
    {
         //qDebug() << Q_FUNC_INFO << ": Error>10 - calling stop";
        stop();
    }
}

void HamLibClass::setReadOnly(const bool _r)
{
    readOnlyMode = _r;
}

void HamLibClass::setNetworkAddress(const QString &_address)
{
    networkAddress = _address;
}

void HamLibClass::setNetworkPort(const int _port)
{
    if ((_port>0) && (_port<65535))
    {
        networkPort = _port;
    }
}

bool HamLibClass::errorManage(const int _errorcode)
{
    //qDebug() << Q_FUNC_INFO << ": error: " << QString::number(_errorcode);

    if (RIG_OK == _errorcode)
    {
        //qDebug() << Q_FUNC_INFO << " - RIG_OK";
        return true;
    }
    switch (_errorcode)
    {

    case (RIG_EINVAL):
        //qDebug() << Q_FUNC_INFO << ": Error: 1 invalid parameter";
    break;
    case (RIG_ECONF):
        //qDebug() << Q_FUNC_INFO << ": Error: 2 invalid configuration (serial,..) ";
    break;
    case (RIG_ENOMEM):
        //qDebug() << Q_FUNC_INFO << ": Error: 3 memory shortage";
    break;
    case (RIG_ENIMPL):
        //qDebug() << Q_FUNC_INFO << ": Error: 4 function not implemented, but will be ";
    break;
    case (RIG_ETIMEOUT):
        //qDebug() << Q_FUNC_INFO << ": Error: 5 communication timed ou";
    break;
    case (RIG_EIO):
        //qDebug() << Q_FUNC_INFO << ": Error: 6 IO error, including open failed";
    break;
    case (RIG_EINTERNAL):
        //qDebug() << Q_FUNC_INFO << ": Error: 7 Internal Hamlib error, huh!";
    break;
    case (RIG_EPROTO):
        //qDebug() << Q_FUNC_INFO << ": Error: 9 Command rejected by the rig";
    break;
    case (RIG_ETRUNC):
        //qDebug() << Q_FUNC_INFO << ": Error: 10 Command performed, but arg truncated";
    break;
    case (RIG_ENAVAIL):
        //qDebug() << Q_FUNC_INFO << ": Error: 11 Function not available";
    break;
    case (RIG_ENTARGET):
        //qDebug() << Q_FUNC_INFO << ": Error: 12 VFO not targetable";
    break;
    case (RIG_BUSERROR):
        //qDebug() << Q_FUNC_INFO << ": Error: 13 Error talking on the bus";
    break;
    case (RIG_BUSBUSY):
        //qDebug() << Q_FUNC_INFO << ": Error: 14 Collision on the bus";
    break;
    case (RIG_EARG):
        //qDebug() << Q_FUNC_INFO << ": Error: 15 NULL RIG handle or any invalid pointer parameter in get arg";
    break;
    case (RIG_EVFO):
        //qDebug() << Q_FUNC_INFO << ": Error: 16 Invalid VFO";
    break;
    case (RIG_EDOM):
        //qDebug() << Q_FUNC_INFO << ": Error: 17 Argument out of domain of func";
    break;
    default:
      //qDebug() << Q_FUNC_INFO << ": Error: ?? Unknown error";
    break;
    }
    if (_errorcode == RIG_EINVAL || _errorcode == RIG_ENIMPL || _errorcode == RIG_ERJCTED \
            || _errorcode == RIG_ETRUNC || _errorcode == RIG_ENAVAIL || _errorcode == RIG_ENTARGET \
            || _errorcode == RIG_EVFO || _errorcode == RIG_EDOM)
    {
        //qDebug() << Q_FUNC_INFO << ": Soft error: Invalid parameters - No reason to re-initialize the hardware";
    }


    if (errorCount<10)
    {
        errorCount++;
       //qDebug() << Q_FUNC_INFO << ": RIG NOK for Mode: - " << QString::number(errorCount) << QT_ENDL;
        return false;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": RIG NOK for Mode" << QT_ENDL;
         //qDebug() << Q_FUNC_INFO << ": Calling stop";
        stop();
        return false;
    }
}
