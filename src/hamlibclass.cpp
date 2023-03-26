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
    //qDebug() << Q_FUNC_INFO;
    timer = new QTimer(this);
    //my_rig = rig_init (RIG_DUMMY);
    rig_set_debug(RIG_DEBUG_NONE);
    my_rig = rig_init (RIG_MODEL_DUMMY);
    retcode = -1;
    bauds = 9600;
    //qDebug() << Q_FUNC_INFO << " - END";
}

HamLibClass::~HamLibClass()
{
    //qDebug() << Q_FUNC_INFO;
    if (rigLaunched)
    {
        rig_close(my_rig);
        rig_cleanup(my_rig);
        rigLaunched = false;
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void HamLibClass::initClass()
{
    //qDebug() << Q_FUNC_INFO ;
    //showDebugLog(Q_FUNC_INFO, "Start");
    strings.clear();
    fillRigsList();
    retcode = -1;
    rigLaunched = false;
    pollInterval = 300;
    errorCount = 0;
    readOnlyMode = false;
    justEmitted = false;
    reading = false;
    freq_old = 0.0;
    bauds = 9600;                  // default 9600
    dataBits = 8;               // default 8
    stopBits = 1;               // default 1
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()) );
    clean();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void HamLibClass::clean()
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO ;
    myrig_model = 1;        //Dummy equipment
    bauds = 9600;
    dataBits = 8;
    stopBits = 1;
    shandshake = RIG_HANDSHAKE_NONE;
    sparity = RIG_PARITY_NONE;
    serialPort = QString();
    sdtr = RIG_SIGNAL_OFF;
    srts = RIG_SIGNAL_OFF;
    networkPort = 4532;
    networkAddress = "127.0.0.1";
    rigLaunched = false;
    //qDebug() << Q_FUNC_INFO << " - END" ;
}

void HamLibClass::setPoll(const int _milsecs)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    if (_milsecs>0)
    {
        pollInterval = _milsecs;
    }
}

double HamLibClass::getFrequency()
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO;
    if (!isRunning())
    {
        //qDebug() << Q_FUNC_INFO << ": isn't running";
        return 0.0;
    }
    if (reading)
    {
        //qDebug() << Q_FUNC_INFO << ": Exiting, I was already reading ... ";
        return 0.0;
    }
    reading = true;
    //qDebug() << Q_FUNC_INFO << ": Reading ... ";
    //retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
    retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);

    if (retcode == RIG_OK)
    {
        //qDebug() << Q_FUNC_INFO << ": RIG OK for Freq: " << QString::number(freq);
        errorCount = 0;
        reading = false;
        return freq/1000000;
    }
    else
    {
        reading = false;
        //qDebug() << Q_FUNC_INFO << " error on readFreq - END";
        return 0.0;
    }
}

bool HamLibClass::readRadio(bool _forceRead)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    return readRadioInternal(_forceRead);
}

bool HamLibClass::readRadioInternal(bool _forceRead)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO;
    if (!isRunning())
    {
        //qDebug() << Q_FUNC_INFO << ": isn't running";
        return false;
    }
    if (reading)
    {
        //qDebug() << Q_FUNC_INFO << ": Exiting, I was already reading ... ";
        return false;
    }
    reading = true;
    //qDebug() << Q_FUNC_INFO << ": Reading ... ";
    retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);

    if (retcode == RIG_OK)
    {
        //qDebug() << Q_FUNC_INFO << ": RIG OK for Freq";
        errorCount = 0;
        //qDebug() << Q_FUNC_INFO << ": Freq: " << QString::number(freq);
        if ((freq_old > freq) || (freq_old < freq) || (_forceRead == true))
        {
            emit freqChanged(freq/1000000);
            freq_old = freq;
            //qDebug() << Q_FUNC_INFO << ": EMITING; " << QString::number(freq);
        }
        //qDebug() << Q_FUNC_INFO << ":  read: " << QString::number(freq);
    }
    else
    {
        reading = false;
        //qDebug() << Q_FUNC_INFO << " error on readFreq - END";
        return errorManage(Q_FUNC_INFO, retcode);
    }

    retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
    if (retcode == RIG_OK)
    {
        //qDebug() << Q_FUNC_INFO << ": RIG OK for Mode";
        errorCount = 0;
        //qDebug() << Q_FUNC_INFO << ": Mode: " << hamlibMode2Mode(rmode);
        if ((mode_old != rmode) || (_forceRead == true))
        {
            if (justEmitted)
            {
                justEmitted = false;
                reading = false;
                //qDebug() << Q_FUNC_INFO << " - justEmitted END";
                return true;
            }
            mode_old = rmode;
            //qDebug() << Q_FUNC_INFO << " - Emmiting mode: " << hamlibMode2Mode(rmode) ;
            emit modeChanged(hamlibMode2Mode(rmode));
            justEmitted = true;
        }
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - Error on readMode END";
        reading = false;
        return errorManage(Q_FUNC_INFO,  retcode);
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    reading = false;
    return true;
}

void HamLibClass::slotTimer()
{
    //showDebugLog(Q_FUNC_INFO, "Start");
     //qDebug() << Q_FUNC_INFO;
    if (!isRunning())
    {
         //qDebug() << Q_FUNC_INFO << ": Isn't running...";
        return;
    }
    readRadioInternal(false); // We don't force the radio reading.
     //qDebug() << Q_FUNC_INFO << " - END";
}

void HamLibClass::setMode(const QString &_m)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << "HamLibClass::setMode: " << _m ;
    if ((!isRunning()) || (readOnlyMode))
    {
        //qDebug() << Q_FUNC_INFO << ": Not running or RO";
        return;
    }

    // Check if we are already in a mode that should not be changed (CWR should not be changed to CW and so on)
    retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
    if (RIG_OK != retcode)
    {
        //qDebug() << "HamLibClass::setMode: ERROR: Could not get mode: ";
        errorManage(Q_FUNC_INFO,  retcode);
    }
    QString currentMode = hamlibMode2Mode(rmode);
    if (_m == currentMode)
    {
        //qDebug() << "HamLibClass::setMode: ERROR: Same mode";
        return;
    }

    retcode = rig_set_mode(my_rig, RIG_VFO_CURR, mode2HamlibMode (_m), rig_passband_normal(my_rig, rig_parse_mode(_m.toLocal8Bit())));
    //retcode = rig_set_mode(my_rig, RIG_VFO_CURR, rig_parse_mode(_m.toLocal8Bit()), rig_passband_normal(my_rig, rig_parse_mode(_m.toLocal8Bit())));

    if (RIG_OK != retcode)
    {
        //qDebug() << "HamLibClass::setMode: ERROR: Could not set mode: " << _m;
        errorManage(Q_FUNC_INFO,  retcode);
        return;
    }

    errorCount = 0;
    //qDebug() << "HamLibClass::setMode - END true ";
    return;
}

bool HamLibClass::isModeADIFMode(const QString &_m)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
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
    //showDebugLog(Q_FUNC_INFO, "Start");
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

rmode_t HamLibClass::mode2HamlibMode (const QString &_mode)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _mode;
    if (_mode == "USB")
    {
        return RIG_MODE_USB;
    }
    else if (_mode == "LSB")
    {
        return RIG_MODE_LSB;
    }
    else if (_mode == "CW")
    {
        return RIG_MODE_CW;
    }
    else if (_mode == "AM")
    {
        return RIG_MODE_AM;
    }
    else if (_mode == "FM")
    {
        return RIG_MODE_FM;
    }
    else if (_mode == "SSB")
    {
        if (freq_old < 10.0)
        {
            return RIG_MODE_LSB;
        }
        else
        {
            return RIG_MODE_USB;
        }
    }
    else
    {
        return RIG_MODE_NONE;
    }
}

bool HamLibClass::stop()
{
    //qDebug() << Q_FUNC_INFO;
    timer->stop();
    if (!isRunning())
    {
        //qDebug() << Q_FUNC_INFO << " - ALready stopped - END";
        return true;
    }
    int errorCode = rig_close(my_rig);
    //qDebug() << Q_FUNC_INFO << " - 10";

    if (errorCode == RIG_OK)
    {
        errorCount = 0;
        //qDebug() << Q_FUNC_INFO << " rig_close OK";
        errorCode = rig_cleanup(my_rig);
        if (errorCode == RIG_OK)
        {
            errorCount = 0;
            //qDebug() << Q_FUNC_INFO << " rig_cleanUp OK";
            rigLaunched = false;
            //qDebug() << Q_FUNC_INFO << " - END true";
            return true;
        }
        else
        {
            //return errorManage(Q_FUNC_INFO,  retcode);
            errorCount++;
            //qDebug() << Q_FUNC_INFO << " rig_cleanup NOK: " << QString::number(errorCode);
        }
    }
    else
    {
        errorCount++;
        //qDebug() << Q_FUNC_INFO << " rig_close NOK: " << QString::number(errorCode);
    }
    //qDebug() << Q_FUNC_INFO << " - END false";
    return false;
}

bool HamLibClass::init(bool _active)
{
    //qDebug()<< Q_FUNC_INFO << ": " << getNameFromModelId(myrig_model);

    if (!_active)
    {
        //qDebug()<< Q_FUNC_INFO << ": not active, exiting";
        rigLaunched = false;
        stop();
        return false;
    }



    if ((getNameFromModelId(myrig_model)).length ()<1)
    {
        //qDebug()<< Q_FUNC_INFO << ": no rig model!";
        rigLaunched = false;
        return false;
    }

    if (rigLaunched)
    {
        //qDebug()<< Q_FUNC_INFO << ": Already init" ;
        return true;
    }

    my_rig = rig_init(myrig_model);
    //qDebug()<< Q_FUNC_INFO << ": set after init" ;
    //if (my_rig == nullptr)
    if (!my_rig)
    {
       //qDebug()<< Q_FUNC_INFO << ": Init failed, hamlib returned fail!";
       return false;
    }
    else
    {
        //qDebug()<< Q_FUNC_INFO << ": rig_init went OK!";
    }
    // Code of DG1VS (Thank you!)
    if (myrig_model == RIG_MODEL_NETRIGCTL)
    {
        //qDebug()<< Q_FUNC_INFO << ": RIG_PORT_NETWORK";
        // network based communication
        my_rig->state.rigport.type.rig = RIG_PORT_NETWORK;
        QString netAddPort = QString("%1:%2").arg (networkAddress).arg(networkPort);
        qstrncpy (my_rig->state.rigport.pathname, netAddPort.toLocal8Bit().constData(), FILPATHLEN);
        // the other stuff is hardcoded in hamlib!
    }
    else if (myrig_model == RIG_MODEL_FLRIG)
    {
       //qDebug()<< Q_FUNC_INFO << ": RIG_PORT_RPC";
        my_rig->state.rigport.type.rig = RIG_PORT_RPC;
        //my_rig->state.rigport.type.rig = RIG_PORT_NETWORK;
        QString netAddPort = QString("%1:%2").arg (networkAddress).arg(networkPort);
        //qDebug()<< Q_FUNC_INFO << ": " << netAddPort;
        qstrncpy (my_rig->state.rigport.pathname, netAddPort.toLocal8Bit().constData(), FILPATHLEN);
    }
    else
    {
        //qDebug()<< Q_FUNC_INFO << ": !RIG_PORT_NETWORK";
        //qDebug()<< Q_FUNC_INFO << ": serialport2: " << serialPort.toLocal8Bit();
        my_rig->state.rigport.type.rig = RIG_PORT_SERIAL;
        //qstrncpy (my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constData(), FILPATHLEN);
        QVariant aux = QVariant(serialPort);
        QByteArray portStr = aux.toByteArray();
        const char* port = portStr.constData();
        strncpy(my_rig->state.rigport.pathname, port, FILPATHLEN - 1);
        //qDebug()<< Q_FUNC_INFO << ": rigport: " << my_rig->state.rigport.pathname ;
        my_rig->state.rigport.parm.serial.rate = bauds;
        //qDebug()<< Q_FUNC_INFO << ": serial rate: " << QString::number(my_rig->state.rigport.parm.serial.rate) ;
        my_rig->state.rigport.parm.serial.data_bits = dataBits;
        //qDebug()<< Q_FUNC_INFO << ": data bits: " << QString::number(my_rig->state.rigport.parm.serial.data_bits) ;
        my_rig->state.rigport.parm.serial.stop_bits = stopBits;
        //qDebug()<< Q_FUNC_INFO << ": stop bits: " << QString::number(my_rig->state.rigport.parm.serial.stop_bits);
        my_rig->state.rigport.parm.serial.parity = sparity;
        //qDebug()<< Q_FUNC_INFO << ": handshake before" ;
        my_rig->state.rigport.parm.serial.handshake = shandshake;
       //qDebug()<< Q_FUNC_INFO << ": after handshake " ;
        // Config done
    }
    //qDebug()<< Q_FUNC_INFO << ": Rig model config " ;
    // Config done
    retcode = rig_open(my_rig);
    //qDebug()<< Q_FUNC_INFO << ": retcode: " << QString::number(retcode) ;

    if (retcode != RIG_OK)
    {
        //qDebug()<< Q_FUNC_INFO << ": Can't open: " << rigerror(retcode);
        rig_cleanup(my_rig);
        return errorManage(Q_FUNC_INFO,  retcode);
    }
    //qDebug()<< Q_FUNC_INFO << ": Rig open!" ;
    errorCount = 0;
    rigLaunched = true;
    freq_old = 0.0;
    timer->start(pollInterval);

    //qDebug()<< Q_FUNC_INFO << ": END TRUE";
    return true;
}

bool HamLibClass::isRunning()
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    return rigLaunched;
}

void HamLibClass::fillRigsList()
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug()<< "HamLibClass::getRigList: StringsList before filling it: ";
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's
  //rig_set_debug (RIG_DEBUG_NONE);
     //qDebug() << "HamLibClass::getRigList-01";
  // and continue...

  strings.clear();
  //qDebug()<< "HamLibClass::getRigList: StringsList after cleaning it: ";
   //qDebug() << "HamLibClass::getRigList-02";
  rig_load_all_backends();
   //qDebug() << "HamLibClass::getRigList-10";
  rig_list_foreach (addRigToList, this);
     //qDebug() << "HamLibClass::getRigList-11";
  strings.sort();
}

QStringList HamLibClass::getRigList ()
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug()<< "HamLibClass::getRigList: StringsList before filling it: ";

    //fillRigsList ();
    //strings.sort();
    //qDebug()<< "HamLibClass::getRigList-12 - Strings length: " << QString::number(strings.length());
    return strings;
 }

int HamLibClass::addRigToList (const struct rig_caps *caps, void *data)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug()<< "HamLibClass::addRigToList" << caps->model_name;
    QString name;
    //qDebug()<< "HamLibClass::addRigToList-10" ;
    //HamLibClass *r = (HamLibClass *) data;
    HamLibClass *r = static_cast<HamLibClass *> (data);
    //qDebug()<< "HamLibClass::addRigToList-11" ;
    name = caps->model_name;
    //qDebug()<< "HamLibClass::addRigToList-12" ;
    r->rigName2RigId[name] = caps->rig_model; // We fill the equivalences between name & Id
    //qDebug()<< "HamLibClass::addRigToList-13: " <<  caps->rig_model;
    r->rigId2RigName[caps->rig_model] = name;
    //qDebug()<< "HamLibClass::addRigToList-14: " << name;
    r->strings << name;
    //qDebug()<< "HamLibClass::addRigToList-END" ;
    return -1;                    // not 0 --> we want all rigs
}

int HamLibClass::getModelIdFromName (const QString &_name)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
   //HamLibClass *r (HamLibClass *) data;
   int i = -1;
   i = rigName2RigId[_name];
   //qDebug() << Q_FUNC_INFO << "Name: " << _name << " = " << QString::number(i);
   return i;
}

QString HamLibClass::getNameFromModelId(const int _id)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << "HamLibClass::getNameFromModelId: " << QString::number(_id) << "/" << rigId2RigName.value(_id) ;
    return rigId2RigName.value(_id);
}

void HamLibClass::setModelId(const int _id)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << "HamLibClass::setModelId: " << QString::number(_id);
    rigLaunched = false;
    myrig_model = _id;
}

void HamLibClass::setPort(const QString &_port)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << "HamLibClass::setPort: " << _port;
    serialPort = _port;
    rigLaunched = false;
    //strncpy (my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constData(), FILPATHLEN);
    //qstrncpy(myport.pathname, serialPort.toLocal8Bit().constData(), serialPort.length()+1);
}

void HamLibClass::setSpeed(const int _speed)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //TODO: Check that it is a valid speed
    bauds = _speed;
    rigLaunched = false;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(bauds);
}

void HamLibClass::setDataBits(const int _data)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": rec: " << QString::number(_data);
    if ((_data >= 5) && (_data <= 8))
    {
        dataBits = _data;
    }
    else
    {
        dataBits = 8;
    }
    rigLaunched = false;
   //qDebug() << Q_FUNC_INFO << ": final: " << QString::number(dataBits);
}

void HamLibClass::setStop(const QString &_stop)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _stop;

    if (_stop == "OneStop")
    {
        stopBits = 1;
    }
    else if (_stop == "OneAndHalfStop")
    {
        stopBits = 3;
    }
    else if (_stop == "TwoStop")
    {
        stopBits = 2;
    }
    else
    {
       stopBits = -1;
    }
    rigLaunched = false;
}

void HamLibClass::setFlow(const QString &_flow)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _flow;
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
    rigLaunched = false;
}

void HamLibClass::setParity(const QString &_parity)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _parity;
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
    rigLaunched = false;
}

void HamLibClass::setFreq(const double _fr)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
     //qDebug() << "HamLibClass::setFreq: " << QString::number(_fr);
    if ((!isRunning()) || (readOnlyMode))
    {
        return;
    }

    freq = _fr * 1000000;
    int retcode = rig_set_freq(my_rig, RIG_VFO_CURR, freq);
    if (retcode != RIG_OK)
    {
        errorManage(Q_FUNC_INFO,  retcode);
    }
    else
    {
        errorCount = 0;
         //qDebug() << "HamLibClass::setFreq OK: " << QString::number(freq);
        retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
        if (retcode == RIG_OK)
        {
            errorCount = 0;
             //qDebug() << "HamLibClass::setFreq read: " << QString::number(freq);
        }
        else
        {
            errorManage(Q_FUNC_INFO,  retcode);
        }
        return;
    }
}

void HamLibClass::setRTS(const QString &_state)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _state;
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
    rigLaunched = false;
}

void HamLibClass::setDTR(const QString &_state)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _state;
    if (_state.toUpper() == "TRUE")
    {
        sdtr = RIG_SIGNAL_ON;
    }
    else
    {
        sdtr = RIG_SIGNAL_OFF;
    }
    rigLaunched = false;
}

void HamLibClass::checkErrorCountAndStop()
{
    //showDebugLog(Q_FUNC_INFO, "Start");
     //qDebug() << Q_FUNC_INFO;
    if (errorCount > 10)
    {
         //qDebug() << Q_FUNC_INFO << ": Error>10 - calling stop";
        stop();
    }
}

void HamLibClass::setReadOnly(const bool _r)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    readOnlyMode = _r;
}

void HamLibClass::setNetworkAddress(const QString &_address)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _address;
    networkAddress = _address;
    rigLaunched = false;
}

void HamLibClass::setNetworkPort(const int _port)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_port);
    if ((_port>0) && (_port<65535))
    {
        networkPort = _port;
    }
    rigLaunched = false;
}

bool HamLibClass::errorManage(const QString &_func, const int _errorcode)
{
    //showDebugLog(Q_FUNC_INFO, "Start");
    //qDebug() << Q_FUNC_INFO << ": " << _func << ": error: " << QString::number(_errorcode);
    Q_UNUSED(_func);
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
        //qDebug() << Q_FUNC_INFO << ": RIG NOK for Mode: - " << QString::number(errorCount);
        return false;
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << ": RIG NOK for Mode";
         //qDebug() << Q_FUNC_INFO << ": Calling stop";
        stop();
        return false;
    }
}

/*void HamLibClass::showDebugLog(const QString &_func, const QString &_log)
{
#ifdef QT_DEBUG
  //qDebug() << _func << ": " << _log;
#else
#endif
}
*/

bool HamLibClass::loadSettings()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("HamLib");
    setModelId(settings.value ("HamLibRigType").toInt());
    setPort(settings.value ("HamlibSerialPort").toString());
    setSpeed(settings.value ("HamlibSerialBauds", 9600).toInt ());
    setDataBits(settings.value ("HamLibSerialDataBits", 8).toInt ());
    setStop(settings.value ("HamLibSerialStopBit", "OneStop").toString());
    setFlow(settings.value ("HamLibSerialFlowControl", "None").toString());
    setParity(settings.value ("HamLibSerialParity", "Even").toString());
    setPoll(settings.value ("HamlibRigPollRate", 2000).toInt ());
    setReadOnly(settings.value ("HamlibReadOnly", false).toBool ());
    setNetworkAddress (settings.value ("HamlibNetAddress").toString());
    setNetworkPort (settings.value ("HamlibNetPort", 4532).toInt ());
    settings.endGroup ();
    return true;
}
