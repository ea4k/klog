/***************************************************************************
                          kloghamlib.h  -  description
                             -------------------
    begin                : mar mar 23 2004
    copyright            : (C) 2004 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KLOGHAMLIB_H
#define KLOGHAMLIB_H
#include <QMessageBox>
#include <QString>
//#include <klocale.h>

#include <hamlib/rig.h>
#define SERIAL_PORT "/dev/ttyS0"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//#include <qtime.h>	// This is to be deleted before releasing as it is just for debuging

//#include "adif.h"
//static bool rigLaunched = false;

class kloghamlib {
public:
    kloghamlib();
    ~kloghamlib();
    bool init ();
    void close();
    void setBauds(int tbauds);
    void setSerialPort(const QString& tserialPort);
    void setDataBits (int tdataBits);
    void setStopBits (int tstopBits);
    void setParity (int tparity);
    void setRigNumber(int trigNumber);
    void setFrequency(double tFreq);
    void setMode (const QString& tMode);

    double getFrequency(); // Returns 0.0 if error or non identified (inMHz)
    QString getMode(); // Returns "NONE" if error or non identified
    int getStrength(); // Returns -1 if error or non identified
    int getPower(); // Returns -1 if error or non identified
    void test();
    void getRigList();
    QString getStatusMessage(); // Returns the radio status message
public: // Public attributes

    RIG* radio;
    freq_t freq;
    rmode_t rmode;
    pbwidth_t width;
    vfo_t vfo;
    int strength;  /* S-Meter level */
    int retcode;  /* generic return code from functions */
    rig_model_t myrig_model;

private: // Private methods
    hamlib_port_t myport;

    bool rigLaunched;
    bool i;
    int bauds; // default value 4800
    int dataBits; // default = 8;
    int stopBits; //default  = 1;
    //serial_parity_e hamlib_port_t::parity tpar
    //serialParity = RIG_PARITY_NONE;
    //serialHandShake = RIG_HANDSHAKE_NONE;
    QString serialPort; // default value "/dev/ttyS0"
    //const char *serialPort;
    QString radioStatus; // Just a String to write & keep the status of the radio;
    //QTime t; /* This is to be deleted before releasing as it is just for debuging*/

  //  Adif adif;
};

#endif
