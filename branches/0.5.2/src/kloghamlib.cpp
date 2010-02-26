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
//using namespace std;
//#include "ostream.h" // This is to be deleted before releasing as it is just for debuging

//using namespace std;

kloghamlib::kloghamlib(){
//cout << "KLogHamlib::constructor" << endl;
	//t.start();
	//qDebug( "Time elapsed: %d ms", t.elapsed() );
	myrig_model =	-1;
	rigLaunched = false;
	bauds = 4800;
	dataBits = 8;
	stopBits = 1;
	serialPort = "";
	radioStatus ="hamlib: not init";
}


kloghamlib::~kloghamlib(){
//	cout << "KLogHamlib::destructor" << endl;
	if (rigLaunched) {
		rig_close(radio); // close port 
		rig_cleanup(radio); 
		rigLaunched = false;
	}// if you care about memory 
	radioStatus ="hamlib: not init";
}



bool kloghamlib::init(){
//	cout << "KLogHamlib::init" << endl;
//	cout << "KLogHamLib: RigNumber: " << QString::number(myrig_model) << endl;
	if (myrig_model == -1)
		return false;
	if (!rigLaunched){
//		cout << "Rig not launched... beginging of launching..." << endl;
		rig_set_debug(RIG_DEBUG_NONE); 
//		cout << "KlogHamlib::init - 1" << endl;
		port_t myport;// may be overriden by backend probe
		myport.type.rig = RIG_PORT_SERIAL;
//		cout << "KlogHamlib::init - 2" << endl;
		myport.parm.serial.rate = bauds;
//		cout << "KlogHamlib::init - 3" << endl;
		myport.parm.serial.data_bits = 8;
//		cout << "KlogHamlib::init - 4" << endl;
		myport.parm.serial.stop_bits = 1;
//		cout << "KlogHamlib::init - 5" << endl;
		myport.parm.serial.parity = RIG_PARITY_NONE;
//		cout << "KlogHamlib::init - 6" << endl;
		myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
//		cout << "KlogHamlib::init - 7" << endl;
		//strncpy(myport.pathname, SERIAL_PORT, FILPATHLEN);
//		strncpy(myport.pathname, serialPort, FILPATHLEN);
                serialPort = myport.pathname;//, FILPATHLEN);
//		cout << "KlogHamlib::init - 8" << endl;
		// rig_load_all_backends();
		// myrig_model = rig_probe(&myport);
		radio = rig_init(myrig_model);
//		cout << "KlogHamlib::init - 9" << endl;
		if (!radio) {
			rigLaunched = false;
			// TODO: To make some error visible with qmessages and so on
			fprintf(stderr,"Unknown rig num: %d\n", myrig_model);
			fprintf(stderr,"Please check riglist.h\n");
			return false;
			//     exit(1); // whoops! something went wrong (mem alloc?) 
		}
//		cout << "KlogHamlib::init - 10" << endl;
//		strncpy(radio->state.rigport.pathname,serialPort,FILPATHLEN);
                serialPort = radio->state.rigport.pathname;//,FILPATHLEN);
//		cout << "KlogHamlib::init - 10.1" << endl;
		retcode = rig_open(radio);
//		cout << "KlogHamlib::init - 10.2" << endl;
		if (retcode != RIG_OK) {
//			cout << "KlogHamlib::init - 10.3" << endl;
			rigLaunched = false;
//			cout << "KlogHamlib::init - 10.4" << endl;
			// TODO: To make some error visible with qmessages and so on
			printf("rig_open: error = %s\n", rigerror(retcode));
//			cout << "KlogHamlib::init - 10.5" << endl;
			return false;
			//    exit(2);
		}
//		cout << "KlogHamlib::init - 10.6" << endl;
		//printf("Port %s opened ok\n", serialPort);
//		cout << "KlogHamlib::init - 10.7" << endl;
		rigLaunched = true;
//		cout << "KlogHamlib::init - 11" << endl;
	}else{
		//cout << "Rig is launched" << endl;
	}
		
	return true;
}




/*


bool kloghamlib::init(){
cout << "KLogHamLib:init: " << QString::number(myrig_model) << endl;
	radioStatus ="hamlib initializing";
	serialPort ="/dev/ttyUSB0";
	rig_set_debug(RIG_DEBUG_NONE);

	myport.type.rig = RIG_PORT_SERIAL;
	myport.parm.serial.rate = 4800;
//	myport.parm.serial.data_bits = 8;
//	myport.parm.serial.stop_bits = 1;
//	myport.parm.serial.parity = RIG_PARITY_NONE;
//	myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
                //strncpy(myport.pathname, SERIAL_PORT, FILPATHLEN);
                //strncpy(myport.pathname, serialPort, FILPATHLEN);
	radio = rig_init(122);
 	strncpy(myport.pathname, SERIAL_PORT, FILPATHLEN);

	if (!radio){ // Radio unknown
			rigLaunched = false;
			cout << "KlogHamlib::init - ERROR !radio" << endl;
			radioStatus ="hamlib error: Unknown rig num";

	}else{ // Radio model OK
		cout << "KlogHamlib::radio model OK" << endl;
		strncpy(radio->state.rigport.pathname,SERIAL_PORT,FILPATHLEN);
		retcode = rig_open(radio);
        	if (retcode != RIG_OK) { // Connection to radio failed
			cout << "KlogHamlib:: radio connection failed: " << rigerror(retcode) << endl;
			rigLaunched = false;
        	}else{ // Connection to radio OK
			cout << "KlogHamlib::radio connection OK" << endl;
			rigLaunched = true;
		}
	}

	return rigLaunched;
	//return false;
}
*/

/*

bool kloghamlib::init(){
cout << "KLogHamLib:init: " << QString::number(myrig_model) << endl;

		rig_set_debug(RIG_DEBUG_NONE); 
		myport.type.rig = RIG_PORT_SERIAL;
		myport.parm.serial.rate = 4800;
		myport.parm.serial.data_bits = 8;
		myport.parm.serial.stop_bits = 1;
		myport.parm.serial.parity = RIG_PARITY_NONE;
		myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
		strncpy(myport.pathname, "/dev/ttyUSB0", FILPATHLEN);
	
//	myrig_model = rig_probe(&myport);
	
	radio = rig_init(122);
	if (!radio){ // Radio unknown
			rigLaunched = false;
			cout << "KlogHamlib::init - ERROR !radio" << endl;
			radioStatus ="hamlib error: Unknown rig num";

	}else{ // Radio model OK
		cout << "KlogHamlib::radio model OK" << endl;


		//strncpy(radio->state.rigport.pathname,serialPort,FILPATHLEN);
		retcode = rig_open(radio);
        	if (retcode != RIG_OK) { // Connection to radio failed
			cout << "KlogHamlib:: radio connection failed: " << rigerror(retcode) << endl;
			rigLaunched = false;
        	}else{ // Connection to radio OK
			cout << "KlogHamlib::radio connection OK" << endl;
			rigLaunched = true;
		}
	}

	return rigLaunched;
	//return false;
}


*/

/*
bool kloghamlib::init(){
cout << "KLogHamlib::init" << endl;
cout << "KLogHamLib: RigNumber: " << QString::number(myrig_model) << endl;

	rig_set_debug(RIG_DEBUG_NONE); 
	myport.type.rig = RIG_PORT_SERIAL;
	myport.parm.serial.rate = bauds;
//	myport.parm.serial.data_bits = dataBits;
//	myport.parm.serial.stop_bits = stopBits;
//	myport.parm.serial.parity = RIG_PARITY_NONE;
//	myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
	strncpy(myport.pathname, serialPort, FILPATHLEN);


	if (myrig_model == -1){
		rig_load_all_backends();
		myrig_model = rig_probe(&myport);
	}



	if (!rigLaunched){
		cout << "Rig not launched... beginging of launching..." << endl;
		
		radio = rig_init(myrig_model); // After seting all configuration we load the rig

		if (!radio) {		// UNKNOWN RIG NUMBER
			rigLaunched = false;
			// TODO: To make some error visible with qmessages and so on
			//fprintf(stderr,"Unknown rig num: %d\n", myrig_model);
			//fprintf(stderr,"Please check riglist.h\n");

			cout << "KlogHamlib::init - ERROR !radio" << endl;
			radioStatus ="hamlib error: Unknown rig num";
			//     exit(1); // whoops! something went wrong (mem alloc?) 
		}else{
			strncpy(radio->state.rigport.pathname,SERIAL_PORT,FILPATHLEN);
			retcode = rig_open(radio);
			if (retcode != RIG_OK) {	// COULD NOT OPEN THE RIG
				rigLaunched = false;
				// TODO: To make some error visible with qmessages and so on
				//printf("rig_open: error = %s\n", rigerror(retcode));
				cout << "KlogHamlib::init - ERROR !RIG_OK" << endl;
				radioStatus ="hamlib error: " + QString(rigerror(retcode));
			}else{
				cout << "KlogHamlib::init - RIG_OK - LAUNCHED" << endl;
				rigLaunched = true;
			}
		}
		
	}else{	// rigLaunched = true
		cout << "KlogHamlib::init - Rig was already launched" << endl;
		rigLaunched = true;
		radioStatus ="hamlib: initialized";
	}
	radioStatus ="hamlib: NOT Initialized";
	return rigLaunched;
}


*/

void kloghamlib::setRigNumber(int trigNumber){

//cout << "KLogHamlib::setRigNumber" << endl;
	myrig_model = trigNumber;
}

void kloghamlib::test(){
//cout << "KLogHamlib::test" << endl;
	//rigLaunched = false;
}



void kloghamlib::close(){
//cout << "KLogHamlib::close" << endl;
	rig_close(radio); /* close port */
	rig_cleanup(radio); /* if you care about memory */
	radioStatus ="hamlib: Closed";
}

double kloghamlib::getFrequency(){
//cout << "KLogHamlib::getFrequency" << endl;
/*
	if (!init()){
		cout << "KLogHamlib::getFrequency: cannot be launched" << endl;
		return 0.0;
	}
*/
	if (!rigLaunched){
//	cout << "KLogHamlib::getFrequency:not launched" << endl;
	radioStatus ="radio status: (getFrecuency) Error: Not previously launched";
		return 0.0;
	}else{
//		cout << "KLogHamlib::getFrequency: YES launched" << endl;
		rig_get_freq(radio, RIG_VFO_CURR, &freq);
		return freq/1000; // We are getting the freq in KHz

//		retcode = rig_get_freq(radio, RIG_VFO_CURR, &freq);
		if (retcode == RIG_OK ) {
//			cout << "KLogHamlib::getFrequency: " << QString::number(freq) << endl;
			radioStatus ="radio status: (getFrecuency) Geting freq:: " + QString::number(freq);
			return freq; // We are geting the freq in MHz
        	} else {
			radioStatus ="radio status: (getFrecuency) Error: getting freq: " + QString(rigerror(retcode));
				printf("rig_get_freq: error =  %s \n", rigerror(retcode));
			return 0.0;
		
		}
	}
}

QString kloghamlib::getMode(){
//cout << "KLogHamlib::getMode" << endl;
	if (!rigLaunched){
//cout << "KLogHamlib::getMode:Not launched" << endl;
		return "NONE";
	}else{
	{
		retcode = rig_get_mode(radio, RIG_VFO_CURR, &rmode, &width);
		if (retcode == RIG_OK ) {
			radioStatus ="hamlib: Mode readed";
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
					radioStatus ="hamlib error: Geting mode: " + QString(rigerror(retcode));
//cout << "KLogHamlib::getMode: no-mode" << endl;
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

/*
RIG_LEVEL_STRENGTH: val is an integer, representing the S Meter level in dB, according to the ideal S Meter scale. The ideal S Meter scale is as follow: S0=-54, S1=-48, S2=-42, S3=-36, S4=-30, S5=-24, S6=-18, S7=-12, S8=-6, S9=0, +10=10, +20=20, +30=30, +40=40, +50=50 and +60=60. This is the responsability of the backend to return values calibrated for this scale.
*/
//	i = -1; // Aux variable
	if (!rigLaunched){
		return -1;
	}else{ //rigLaunched = true
		retcode = rig_get_strength(radio, RIG_VFO_CURR, &strength);
	}
	if (retcode == RIG_OK ) {
		radioStatus ="hamlib: Strength readed" + QString::number(strength);
	//      cout << "getStrength: " << QString::number(strength) << endl;
		if (strength>=60){
			return 60;
		}else if (strength>=50){
			return 50;
		}else if (strength>=40){
			return 40;
		}else if (strength>=30){
			return 30;
		}else if (strength>=20){
			return 20;
		}else if (strength>=10){
			return 10;
		}else if (strength>=0){
			return 9;
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
			return 1;
		}else if (strength>=-54){
			return 0;
		}else{
			return -1;
		}
		//Finishes the switch
	} else {
		radioStatus ="hamlib error: Geting signal stregth: " + QString(rigerror(retcode));
		return -1;
	} // Closes the Rig_OK
}

void kloghamlib::setFrequency(double tFreq){ // Freq is in MHz
//    cout << "kloghamlib - Frecuency: " << QString::number(tFreq) << endl;
	if (!rigLaunched){
		return;
	}else{
		retcode = rig_set_freq(radio, RIG_VFO_CURR, tFreq);
		radioStatus ="hamlib: Changed freq to: " + QString::number(tFreq);
        	if (retcode != RIG_OK ) {
			radioStatus ="hamlib error: Seting freq: " + QString(rigerror(retcode));
        	} 
	}
}

void kloghamlib::setMode (const QString& tMode){
//cout << "kloghamlib: Mode received: " << tMode << endl;
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
		}else if (tMode == "FAX"){
			retcode = rig_set_mode(radio, RIG_VFO_CURR, RIG_MODE_FAX, RIG_PASSBAND_NORMAL);

		}else{ // IF MODE IS NOT KNOWN FOR HAMLIB, NO OPERATION
		}
        	if (retcode != RIG_OK ) {
			radioStatus ="hamlib error: Seting mode: " + QString(rigerror(retcode));
        	} else {
			radioStatus ="hamlib: Mode changed to " + tMode;
		}
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
//cout << "KLogHamlib::setBauds: " << QString::number(tbauds) << endl;
//	bauds = 4800;
	bauds = tbauds;
	myport.parm.serial.rate = bauds;
}

void kloghamlib::setSerialPort(const QString& tserialPort){
//cout << "KLogHamlib::setSerialPort: " << tserialPort << endl;
	serialPort = tserialPort;
}


QString kloghamlib::getStatusMessage(){
//cout << "KLogHamlib::getStatusMessage: " << radioStatus << endl;
	return radioStatus;
}

void kloghamlib::setDataBits (int tdataBits){
//cout << "KLogHamlib::setDataBits: " << tdataBits << endl;
	dataBits = tdataBits;
	myport.parm.serial.data_bits = dataBits;


}

void kloghamlib::setStopBits (int tstopBits){
//cout << "KLogHamlib::setStopBits: " << tstopBits << endl;
	stopBits = tstopBits;
	myport.parm.serial.stop_bits = stopBits;
}

void kloghamlib::setParity (int tparity){
//cout << "KLogHamlib::setParity: " << tparity << endl;
		switch(tparity){
			// 0 none, 1 odd, 2 even
		case 0: // No parity
			myport.parm.serial.parity = RIG_PARITY_NONE;
		break;
		case 1: // Continue
			myport.parm.serial.parity = RIG_PARITY_ODD;
		break;
		case 2:
			myport.parm.serial.parity = RIG_PARITY_EVEN;
		break;
		default: //In case it fails, assigns NONE
			myport.parm.serial.parity = RIG_PARITY_NONE;
		break;
		}// Closes the Switch
}


