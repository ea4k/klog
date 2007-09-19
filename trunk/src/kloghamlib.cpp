/***************************************************************************
                          kloghamlib.cpp  -  description
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

#include "kloghamlib.h"
//#include "iostream"
//#include "ostream.h" // This is to be deleted before releasing as it is just for debuging

using namespace std;

kloghamlib::kloghamlib(){
	//cout << "KLogHamlib::constructor" << endl;
	//t.start();
	//qDebug( "Time elapsed: %d ms", t.elapsed() );
	myrig_model =	-1;
	rigLaunched = false;
	bauds = 19200;
	serialPort = "/dev/ttyS0";
	
}


kloghamlib::~kloghamlib(){
//	cout << "KLogHamlib::destructor" << endl;
	if (rigLaunched) {
		rig_close(radio); // close port 
		rig_cleanup(radio); 
		rigLaunched = false;
	}// if you care about memory 

}

void kloghamlib::setRigNumber(int trigNumber){

//	cout << "KLogHamlib::setRigNumber" << endl;
	myrig_model = trigNumber;
}

void kloghamlib::test(){
	//cout << "KLogHamlib::test" << endl;
	//rigLaunched = false;
}

bool kloghamlib::init(){
	//cout << "KLogHamlib::init" << endl;
	//cout << "KLogHamLib: RigNumber: " << QString::number(myrig_model) << endl;
	if (myrig_model == -1)
		return false;
	if (!rigLaunched){
 		//cout << "Rig not launched... beginging of launching..." << endl;
		rig_set_debug(RIG_DEBUG_NONE); 
		//cout << "KlogHamlib::init - 1" << endl;
		port_t myport;// may be overriden by backend probe
		myport.type.rig = RIG_PORT_SERIAL;
		//cout << "KlogHamlib::init - 2" << endl;
		myport.parm.serial.rate = bauds;
		//cout << "KlogHamlib::init - 3" << endl;
		myport.parm.serial.data_bits = 8;
		//cout << "KlogHamlib::init - 4" << endl;
		myport.parm.serial.stop_bits = 1;
		//cout << "KlogHamlib::init - 5" << endl;
		myport.parm.serial.parity = RIG_PARITY_NONE;
		//cout << "KlogHamlib::init - 6" << endl;
		myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
		//cout << "KlogHamlib::init - 7" << endl;
		//strncpy(myport.pathname, SERIAL_PORT, FILPATHLEN);
		strncpy(myport.pathname, serialPort, FILPATHLEN);
		//cout << "KlogHamlib::init - 8" << endl;
		// rig_load_all_backends();
		// myrig_model = rig_probe(&myport);  
		radio = rig_init(myrig_model);
		//cout << "KlogHamlib::init - 9" << endl;
		if (!radio) {
			rigLaunched = false;
			// TODO: To make some error visible with qmessages and so on
			//fprintf(stderr,"Unknown rig num: %d\n", myrig_model);
			//fprintf(stderr,"Please check riglist.h\n");
			return false;
			//     exit(1); // whoops! something went wrong (mem alloc?) 
		}
		//cout << "KlogHamlib::init - 10" << endl;
		strncpy(radio->state.rigport.pathname,SERIAL_PORT,FILPATHLEN);
		//cout << "KlogHamlib::init - 10.1" << endl;
		retcode = rig_open(radio);
		//cout << "KlogHamlib::init - 10.2" << endl;
		if (retcode != RIG_OK) {
			//cout << "KlogHamlib::init - 10.3" << endl;
			rigLaunched = false;
			// TODO: To make some error visible with qmessages and so on
			//printf("rig_open: error = %s\n", rigerror(retcode));
			return false;
			//    exit(2);
		}
		//printf("Port %s opened ok\n", serialPort);
		rigLaunched = true;
	}else{
	}
	return true;
}



void kloghamlib::close(){
	//cout << "KLogHamlib::close" << endl;
	rig_close(radio); /* close port */
	rig_cleanup(radio); /* if you care about memory */
}

double kloghamlib::getFrequency(){
	//cout << "KLogHamlib::getFrequency" << endl;
	if (!rigLaunched){
		return 0.0;
	}else{
		rig_get_freq(radio, RIG_VFO_CURR, &freq);
		return freq/1000; // We are getting the freq in KHz
	}
}

QString kloghamlib::getMode(){
	//cout << "KLogHamlib::getMode" << endl;
	if (!rigLaunched){
		return "NONE";
	}else{
	{
		retcode = rig_get_mode(radio, RIG_VFO_CURR, &rmode, &width);
		if (retcode == RIG_OK ) {
					switch (rmode){
						case RIG_MODE_NONE:
							return "NONE";
						break;
						case RIG_MODE_AM:
							return "AM";
						break;
						case RIG_MODE_CW:
							return "CW";
						break;
						case RIG_MODE_USB:
							return "USB";
						break;
						case RIG_MODE_LSB:
							return "LSB";
						break;
						case RIG_MODE_RTTY:
							return "RTTY";
						break;
						case RIG_MODE_FM:
							return "FM";
						break;
						case RIG_MODE_WFM:
							return "WFM";
						break;
						case RIG_MODE_ECSSLSB:
							return "ECSSLSB";
						break;
						case RIG_MODE_CWR:
							return "CWR";
						break;
						case RIG_MODE_RTTYR:
							return "RTTYR";
						break;
						case RIG_MODE_AMS:
							return "AMS";
						break;
						case RIG_MODE_PKTLSB:
							return "PKTLSB";
						break;
						case RIG_MODE_PKTUSB:
							return "PKTUSB";
						break;
						case RIG_MODE_PKTFM:
							return "PKTFM";
						break;
						case RIG_MODE_ECSSUSB:
							return "ECSSUSB";
						break;
						case RIG_MODE_FAX:
							return "FAX";
						break;
						default:
							return "NONE";
						break;
					}
				} else {
					// TODO: To make some error visible with qmessages and so on
					//printf("rig_get_mode: error2 =  %s \n", rigerror(retcode));
					return "NONE";
				}
			}
		}
	}

int kloghamlib::getStrength(){
//    S0=-54, S1=-48, S2=-42, S3=-36, S4=-30, S5=-24, S6=-18, S7=-12, S8=-6, S9=0, +10=10, +20=20, +30=30, +40=40, +50=50 and +60=60.
	//cout << "KLogHamlib::getStrength" << endl;
	i = -1; // Aux variable
	if (!rigLaunched){
		return -1;
	}else{ //rigLaunched = true
		retcode = rig_get_strength(radio, RIG_VFO_CURR, &strength);
	}
	if (retcode == RIG_OK ) {
	//      cout << "getStrength: " << QString::number(strength) << endl;
		if (strength>=10){
			return strength;
		}else if (strength>=0){
			return 0;
		}else if (strength>=-6){
			return 8;
		}else if (strength>=-12){
			return 7;
		}else if (strength>=-18){
			return 6;
		}else if (strength>=-24){
			return 5;
		}else if (strength>=-30){
			return 4;
		}else if (strength>=-36){
			return 3;
		}else if (strength>=-42){
			return 2;
		}else if (strength>=-48){
			return 0;
		}else{
			return -1;
		}
		//Finishes the switch
	} else {
		return -1;
	} // Closes the Rig_OK
}

void kloghamlib::setFrequency(double tFreq){
//    cout << "kloghamlib - Frecuencia: " << QString::number(tFreq) << endl;
	//cout << "KLogHamlib::setFrequency" << endl;
	if (!rigLaunched){
		return;
	}else{
		retcode = rig_set_freq(radio, RIG_VFO_CURR, tFreq*1000);
	}
}

void kloghamlib::setMode (const QString& tMode){
//	cout << "KLogHamlib::setMode" << endl;
//cout << "Mode received: " << tMode << endl;
	if (!rigLaunched){
		return;
	}else{
		if (tMode == "AM"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_AM, RIG_PASSBAND_NORMAL);
		}else if (tMode == "CW"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_CW, RIG_PASSBAND_NORMAL);     
		}else if (tMode == "CWR"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_CWR, RIG_PASSBAND_NORMAL);        
		}else if (tMode == "USB"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_USB, RIG_PASSBAND_NORMAL);       
		}else if (tMode == "LSB"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_LSB, RIG_PASSBAND_NORMAL);          
		}else if (tMode == "RTTY"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_RTTY, RIG_PASSBAND_NORMAL);     
		}else if (tMode == "RTTYR"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_RTTYR, RIG_PASSBAND_NORMAL);     
		}else if (tMode == "FM"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_FM, RIG_PASSBAND_NORMAL);     
		}else if (tMode == "WFM"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_WFM, RIG_PASSBAND_NORMAL);     
		}else{}
	}
}

int kloghamlib::getPower(){
    //TODO: This is To Be Done
	//cout << "KLogHamlib::getPower" << endl;
	if (!rigLaunched){
		return 0;
	}else{
		//    retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_USB, RIG_PASSBAND_NORMAL);
		return 0;
	}
}

void kloghamlib::getRigList(){
	//cout << "KLogHamlib::getRigLilst" << endl;
	rig_load_all_backends();
}

void kloghamlib::setBauds(int tbauds){
	//cout << "KLogHamlib::setBauds" << endl;
	bauds = tbauds;
}

void kloghamlib::setSerialPort(const QString& tserialPort){
	//cout << "KLogHamlib::setSerialPort" << endl;
	serialPort = tserialPort;
}

