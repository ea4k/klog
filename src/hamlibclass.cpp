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
#include "utilities.h"

HamLibClass::HamLibClass(QObject *parent) : QObject(parent)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO;
    timer = new QTimer(this);
    my_rig = nullptr;
    connected = false;
    rig_state = RigState::Disconnected;
    //connect(timer, &QTimer::timeout, this, &HamLibClass::readRadioInternal);

    //qDebug() << Q_FUNC_INFO << " - END";
}

HamLibClass::~HamLibClass()
{
    //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (timer)
        timer->stop();
   //qDebug() << Q_FUNC_INFO << " - END";
    cleanup();
    delete(timer);
}

void HamLibClass::initClass()
{
    //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    strings.clear();
    fillRigsList();
    rig_set_debug(RIG_DEBUG_NONE);

    connect(timer, &QTimer::timeout, this, &HamLibClass::slotTimer);
    clean();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void HamLibClass::clean()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO;
    myrig_model = 1;        //Dummy equipment
    bauds = 9600;
    dataBits = 8;
    stopBits = 1;
    pollInterval = 300;

    retcode = -1;
    readOnlyMode = false;
    shandshake = RIG_HANDSHAKE_NONE;
    sparity = RIG_PARITY_NONE;
    serialPort = QString();
    sdtr = RIG_SIGNAL_OFF;
    srts = RIG_SIGNAL_OFF;
    networkPort = 4532;
    networkAddress = "127.0.0.1";
    freq_old = 0.0;
    rig_state = RigState::Disconnected;
    connected = false;
    errorCount = 0;
    justEmitted = false;
    reading = false;
    //qDebug() << Q_FUNC_INFO << " - END";
}

void HamLibClass::cleanup()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);

   //qDebug() << Q_FUNC_INFO << " - Start. Current State: " << static_cast<int>(rig_state);

    // 1. Check the status
    // Ignore the call if disconnected or in the process to be disconnected
    if (rig_state == RigState::Disconnected || rig_state == RigState::Disconnecting) {
       //qDebug() << Q_FUNC_INFO << " - Ignoring: Already disconnected or disconnecting.";
        return;
    }

    // 2. Change the status
    rig_state = RigState::Disconnecting;

    // 3. Cleaning
    if (timer) {
        timer->stop();
    }

   //qDebug() << Q_FUNC_INFO << " - 10";

    if (my_rig) {
       //qDebug() << Q_FUNC_INFO << " - 40";
        rig_close(my_rig);
       //qDebug() << Q_FUNC_INFO << " - 41";
        rig_cleanup(my_rig);
       //qDebug() << Q_FUNC_INFO << " - 42";
        my_rig = nullptr;
       //qDebug() << Q_FUNC_INFO << " - 43";
    }

   //qDebug() << Q_FUNC_INFO << " - 50";

    // 4. Update status
    connected = false; // To be removed when the rest of the class is updated
    rig_state = RigState::Disconnected;

   //qDebug() << Q_FUNC_INFO << " - END";
}

void HamLibClass::setPoll(const int _milsecs)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (_milsecs>0)
    {
        pollInterval = _milsecs;
    }
}

bool HamLibClass::readFreq()
{
   //qDebug() << Q_FUNC_INFO;
   logEvent(Q_FUNC_INFO, "Start", Debug);

   if (!my_rig || (rig_state != RigState::Connected) )
       return false;


   int retcode = rig_get_freq(my_rig, RIG_VFO_CURR, &freq);
   if (retcode == RIG_OK)
   {
       qDebug() << Q_FUNC_INFO << " FREQ TX: " << (double) freq;
       radioStatus.freq_VFO_TX = Frequency ((double) freq, Hz);
       errorCount = 0;
   }
   else
   {
       //qDebug() << Q_FUNC_INFO << " error on readFreq - END";
       return errorManage(Q_FUNC_INFO, retcode);
   }
   retcode = rig_get_freq(my_rig, RIG_VFO_SUB, &freq);
   if (retcode == RIG_OK)
   {
       qDebug() << Q_FUNC_INFO << " FREQ RX: " << (double) freq;
       radioStatus.freq_VFO_RX = Frequency ((double) freq, Hz);
       errorCount = 0;
   }
   else
   {
       //qDebug() << Q_FUNC_INFO << " error on readFreq - END";
       return errorManage(Q_FUNC_INFO, retcode);
   }
   return true;
}

bool HamLibClass::readMode()
{
    //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!my_rig || (rig_state != RigState::Connected) )
        return false;


    int retcode = rig_get_mode(my_rig, RIG_VFO_CURR, &rmode, &width);
    if (retcode == RIG_OK)
    {
        radioStatus.mode_VFO_TX = hamlibMode2Mode(rmode);
        errorCount = 0;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " error on readFreq - END";
        return errorManage(Q_FUNC_INFO, retcode);
    }
    retcode = rig_get_mode(my_rig, RIG_VFO_SUB, &rmode, &width);
    if (retcode == RIG_OK)
    {
        radioStatus.mode_VFO_RX = hamlibMode2Mode(rmode);
        errorCount = 0;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " error on readFreq - END";
        return errorManage(Q_FUNC_INFO, retcode);
    }
    return true;
}

bool HamLibClass::readRadio(bool _forceRead)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    return readRadioInternal(_forceRead);
}

bool HamLibClass::readRadioInternal(bool _forceRead)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!my_rig || (rig_state != RigState::Connected) )
        return false;

    //reading = true;

    RadioStatus statusOld = radioStatus;
    if(!readFreq())
        return false;

    if (!readMode())
        return false;
    Utilities util(Q_FUNC_INFO);
    qDebug() << Q_FUNC_INFO << " - RadioStatusChanged: " << util.boolToQString(radioStatusChanged(statusOld, radioStatus));
    //if (radioStatusChanged(statusOld, radioStatus))
        emit radioStatusChanged(radioStatus);

    //reading = false;
    return true;
}

bool HamLibClass::radioStatusChanged(const RadioStatus _old, const RadioStatus _new)
{
    return (
        _old.split != _new.split                ||
        _old.freq_VFO_RX != _new.freq_VFO_RX    ||
        _old.freq_VFO_TX != _new.freq_VFO_TX    ||
        _old.mode_VFO_RX != _new.mode_VFO_RX    ||
        _old.mode_VFO_TX != _new.mode_VFO_TX
        );
}

void HamLibClass::slotTimer()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "HamLibClass::setMode: " << _m;
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
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    logEvent(Q_FUNC_INFO, "Start", Debug);
   //qDebug() << Q_FUNC_INFO << " - Start";
    cleanup();
   //qDebug() << Q_FUNC_INFO << " - END";
    return false;
}

bool HamLibClass::init(bool _active)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
   //qDebug() << Q_FUNC_INFO << " - Start Initialization";

    // 1. BARRERA MEJORADA (Reconexión dinámica)
    // If connected we clean it first
    if (rig_state == RigState::Connected || rig_state == RigState::Connecting) {
       //qDebug() << Q_FUNC_INFO << " - Reconnecting: Closing previous connection first...";
        cleanup();
        // Al terminar cleanup(), rig_state pasará a ser Disconnected y my_rig a nullptr.
        // Así podemos continuar de forma 100% segura hacia abajo.
    }

    if (myrig_model <= 1) { // 0 = Dummy, 1 = Invalid
       //qDebug() << Q_FUNC_INFO << " - ERROR: Hamlib disabled or invalid model.";
        return false;
    }

    rig_state = RigState::Connecting;
   //qDebug() << Q_FUNC_INFO << " - 10: Initializing rig model: " << myrig_model;

    // 2. Instanciar la radio
    my_rig = rig_init(myrig_model);
    if (!my_rig) {
       //qDebug() << Q_FUNC_INFO << " - ERROR: Failed to initialize Rig (rig_init returned NULL).";
        rig_state = RigState::Disconnected;
        return false;
    }

   //qDebug() << Q_FUNC_INFO << " - 20: Configuring port";

    // 3. Configurar el puerto dependiendo de si es Red (Rigctl) o Serie local
    if (myrig_model == RIG_MODEL_NETRIGCTL) {
        // --- CONFIGURACIÓN DE RED ---
        // Hamlib espera el formato "IP:PORT" en el pathname
        QString netPath = QString("%1:%2").arg(networkAddress).arg(networkPort);
        strncpy(my_rig->state.rigport.pathname, netPath.toLatin1().constData(), FILPATHLEN - 1);
        my_rig->state.rigport.type.rig = RIG_PORT_NETWORK;
    }
    else {
        // --- CONFIGURACIÓN SERIE (USB/COM) ---
        if (!serialPort.isEmpty()) {
            strncpy(my_rig->state.rigport.pathname, serialPort.toLatin1().constData(), FILPATHLEN - 1);
            my_rig->state.rigport.type.rig = RIG_PORT_SERIAL;
        } else {
           //qDebug() << Q_FUNC_INFO << " - ERROR: Serial port is empty!";
            // Fallo intencionado si no hay puerto, no vale la pena seguir
            rig_cleanup(my_rig);
            my_rig = nullptr;
            rig_state = RigState::Disconnected;
            return false;
        }

        // Configurar la velocidad de forma nativa a través de Hamlib
        if (bauds > 0) {
            rig_set_conf(my_rig, rig_token_lookup(my_rig, "serial_speed"), QString::number(bauds).toLatin1().constData());
        }

        // (Opcional) Si quieres pasar el resto de parámetros (Paridad, DataBits, etc)
        // rig_set_conf(my_rig, rig_token_lookup(my_rig, "data_bits"), QString::number(dataBits).toLatin1().constData());
        // rig_set_conf(my_rig, rig_token_lookup(my_rig, "stop_bits"), QString::number(stopBits).toLatin1().constData());
    }

   //qDebug() << Q_FUNC_INFO << " - 30: Opening connection";

    // 4. Abrir la conexión real
    int retcode = rig_open(my_rig);

    if (retcode == RIG_OK) {
       //qDebug() << Q_FUNC_INFO << " - SUCCESS: Rig opened";
        rig_state = RigState::Connected;
        connected = true;

        // ¡IMPORTANTE! Iniciar el polling si la conexión tuvo éxito
        if (_active && timer) {
            timer->start(pollInterval);
        }
        return true;

    } else {
       //qDebug() << Q_FUNC_INFO << " - ERROR: rig_open failed with code: " << retcode;
        rig_state = RigState::Error;
        connected = false;

        // Limpieza profunda tras fallo
        if (my_rig) {
            rig_cleanup(my_rig);
            my_rig = nullptr;
        }
        return false;
    }
}

bool HamLibClass::isRunning()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    return (rig_state == RigState::Connected);
    //return connected;
}

void HamLibClass::fillRigsList()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "HamLibClass::getRigList: StringsList before filling it: ";
    // Rutine to fill the rig combo boxes
    // Do not display debug codes when load the rig's
    //rig_set_debug (RIG_DEBUG_NONE);
    //qDebug() << "HamLibClass::getRigList-01";
    // and continue...

    strings.clear();
    //qDebug() << "HamLibClass::getRigList: StringsList after cleaning it: ";
    //qDebug() << "HamLibClass::getRigList-02";
    rig_load_all_backends();
    //qDebug() << "HamLibClass::getRigList-10";
    rig_list_foreach(addRigToList, this);
    //qDebug() << "HamLibClass::getRigList-11";
    strings.sort();
}

QStringList HamLibClass::getRigList()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "HamLibClass::getRigList: StringsList before filling it: ";

    //fillRigsList ();
    //strings.sort();
    //qDebug() << "HamLibClass::getRigList-12 - Strings length: "
    //<< QString::number(strings.length());
    return strings;
 }

int HamLibClass::addRigToList (const struct rig_caps *caps, void *data)
{
    //logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "HamLibClass::addRigToList" << caps->model_name;
    QString name;
    //qDebug() << "HamLibClass::addRigToList-10";
    //HamLibClass *r = (HamLibClass *) data;
    HamLibClass *r = static_cast<HamLibClass *> (data);
    //qDebug() << "HamLibClass::addRigToList-11";
    name = caps->model_name;
    //qDebug() << "HamLibClass::addRigToList-12";
    r->rigName2RigId[name] = caps->rig_model; // We fill the equivalences between name & Id
    //qDebug() << "HamLibClass::addRigToList-13: " << caps->rig_model;
    r->rigId2RigName[caps->rig_model] = name;
    //qDebug() << "HamLibClass::addRigToList-14: " << name;
    r->strings << name;
    //qDebug() << "HamLibClass::addRigToList-END";
    return -1;                    // not 0 --> we want all rigs
}

int HamLibClass::getModelIdFromName (const QString &_name)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
   //HamLibClass *r (HamLibClass *) data;
   int i = -1;
   i = rigName2RigId[_name];
   //qDebug() << Q_FUNC_INFO << "Name: " << _name << " = " << QString::number(i);
   return i;
}

QString HamLibClass::getNameFromModelId(const int _id)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "HamLibClass::getNameFromModelId: " << QString::number(_id) << "/"
    // << rigId2RigName.value(_id);
    return rigId2RigName.value(_id);
}

void HamLibClass::setModelId(const int _id)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "HamLibClass::setModelId: " << QString::number(_id);
    connected = false;
    rig_state = RigState::Disconnected;
    myrig_model = _id;
}

void HamLibClass::setPort(const QString &_port)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "HamLibClass::setPort: " << _port;
    serialPort = _port;
    connected = false;
    rig_state = RigState::Disconnected;
    //strncpy (my_rig->state.rigport.pathname, serialPort.toLocal8Bit().constData(), FILPATHLEN);
    // qstrncpy(myport.pathname, serialPort.toLocal8Bit().constData(), serialPort.length()+1);
}

void HamLibClass::setSpeed(const int _speed)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //TODO: Check that it is a valid speed
    bauds = _speed;
    connected = false;
    rig_state = RigState::Disconnected;
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(bauds);
}

void HamLibClass::setDataBits(const int _data)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    ////qDebug() << Q_FUNC_INFO << ": rec: " << QString::number(_data);
    if ((_data >= 5) && (_data <= 8))
    {
        dataBits = _data;
    }
    else
    {
        dataBits = 8;
    }
    connected = false;
    rig_state = RigState::Disconnected;
    //qDebug() << Q_FUNC_INFO << ": final: " << QString::number(dataBits);
}

void HamLibClass::setStop(const QString &_stop)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    ////qDebug() << Q_FUNC_INFO << ": " << _stop;

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
    connected = false;
    rig_state = RigState::Disconnected;
}

void HamLibClass::setFlow(const QString &_flow)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    connected = false;
    rig_state = RigState::Disconnected;
}

void HamLibClass::setParity(const QString &_parity)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    connected = false;
    rig_state = RigState::Disconnected;
}

void HamLibClass::setFreq(const Frequency &_fr, bool _TX)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (!isRunning() || readOnlyMode) {
        return;
    }


    freq = _fr.toDouble(Hz);

    logEvent(Q_FUNC_INFO, "Start", Debug);
    qDebug() << "HamLibClass::setFreq" << (_TX ? "(TX):" : "(RX):") << freq;

    vfo_t target_vfo = _TX ? RIG_VFO_CURR : RIG_VFO_SUB;
    int retcode = rig_set_freq(my_rig, target_vfo, freq);

    // Error management
    if (retcode != RIG_OK) {
        errorManage(Q_FUNC_INFO, retcode);
        return;
    }

    errorCount = 0;
}

void HamLibClass::setRTS(const QString &_state)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    connected = false;
    rig_state = RigState::Disconnected;
}

void HamLibClass::setDTR(const QString &_state)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << _state;
    if (_state.toUpper() == "TRUE")
    {
        sdtr = RIG_SIGNAL_ON;
    }
    else
    {
        sdtr = RIG_SIGNAL_OFF;
    }
    connected = false;
    rig_state = RigState::Disconnected;
}

void HamLibClass::checkErrorCountAndStop()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO;
    if (errorCount > 10)
    {
        //qDebug() << Q_FUNC_INFO << ": Error>10 - calling stop";
        stop();
    }
}

void HamLibClass::setReadOnly(const bool _r)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    readOnlyMode = _r;
}

void HamLibClass::setNetworkAddress(const QString &_address)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << _address;
    networkAddress = _address;
    connected = false;
    rig_state = RigState::Disconnected;
}

void HamLibClass::setNetworkPort(const int _port)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_port);
    if ((_port>0) && (_port<65535))
    {
        networkPort = _port;
    }
    connected = false;
    rig_state = RigState::Disconnected;
}

bool HamLibClass::errorManage(const QString &_func, const int _errorcode)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    case (RIG_ERJCTED):
        //qDebug() << Q_FUNC_INFO << ": Error: 8 Command rejected by the rig";
        break;
    case (RIG_EPROTO):
        //qDebug() << Q_FUNC_INFO << ": Error: 9 Protocol Error";
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
        //qDebug() << Q_FUNC_INFO
        // << ": Error: 15 NULL RIG handle or any invalid pointer parameter in get arg";
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
        //qDebug() << Q_FUNC_INFO
        // << ": Soft error: Invalid parameters - No reason to re-initialize the hardware";
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

bool HamLibClass::loadSettings()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("HamLib");
    setModelId(settings.value("HamLibRigType", 0).toInt());
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
    if (pollInterval < 250)
        pollInterval = 250; // Limit saturation
    return true;
}

void HamLibClass::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    emit debugLog (_func, _msg, _level);
}
