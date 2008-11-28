/***************************************************************************
                          adif.cpp  -  description
                             -------------------
    begin                : vie mar 7 2003
    copyright            : (C) 2003 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
 *                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/

//#include <klocale.h>
//#include <kdebug.h>
//#include "iostream"
#include "adif.h"
using namespace std;
/*
This class is really not independant FROM the GUI as the numbers to codify the
MODE and BANDS depend on the position of the band in the COMBO-box.
The BAND is being changed... the mode should be changed!

Please follow this table of equivalences:

BAND	FREQS (MHz)	GUI
2190M	0.136	0.137	1000 Still not supported in GUI
160M	1.8	2.0	0
80M	3.5	4.0	1
60M	5.102	5.404	2
40M	7.0	7.3	3
30M	10	10.15	4
20M	14	14.35	5
17M	18.068	18.168	6	
15M	21.0	21.45	7
12M	24.89	24.99	8
10M	28.0	29.7	9
6M	50	54	10
4M	70	71	11
2M	144	148	12
125CM	222	225	13
70CM	420	450	14
33CM	902	928	15
23CM	1240	1300	16
13CM	2300	2450	17
9CM	3300	3500	1018 Still not supported in GUI
6CM	5650	5925	1019 Still not supported in GUI
3CM	10000	10500	1020 Still not supported in GUI
1.25CM	24000	24250	1021 Still not supported in GUI
6MM	47000	47200	1022 Still not supported in GUI
4MM	75500	81000	1023 Still not supported in GUI
2.5MM	119980	120020	1024 Still not supported in GUI
2MM	142000	149000	1025 Still not supported in GUI
1MM	241000	250000	1026 Still not supported in GUI
*/



Adif::Adif(){
//cout << " - Class Adif" << endl;
}

Adif::~Adif(){
//cout << " - Class ~Adif" << endl;    
}


int Adif::band2Int(const QString& tband){
//cout << "- Class Adif::band2Int: #" << tband << "#" << endl;
  sAux = tband.upper();

  if (sAux == "2190M"){
    return 1000;	
  }else if (sAux == "160M"){
    return 0;
  }else if (sAux == "80M"){
    return 1;
  }else if (sAux == "60M"){
    return 2;
  }else if (sAux == "40M"){
    return 3;
  }else if (sAux == "30M"){
    return 4;
  }else if (sAux == "20M"){
    return 5;
  }else if (sAux == "17M"){
    return 6;
  }else if (sAux == "15M"){
    return 7;
  }else if (sAux == "12M"){
    return 8;
  }else if (sAux == "10M"){
    return 9;
  }else if (sAux == "6M"){
    return 10;
  }else if (sAux == "4M"){
    return 11;
  }else if (sAux == "2M"){
    return 12;
  }else if (sAux == "125CM"){
     return 13;
  }else if (sAux == "70CM"){
    return 14;
  }else if (sAux == "33CM"){
    return 15;
  }else if (sAux == "23CM"){
    return 16;
  }else if (sAux == "13CM"){
    return 17;
  }else if (sAux == "9CM"){
    return 1018;
  }else if (sAux == "6CM"){
    return 1019;
  }else if (sAux == "3CM"){
    return 1020;
  }else if (sAux == "1.25CM"){
    return 1021;
  }else if (sAux == "6MM"){
    return 1022;
  }else if (sAux == "4MM"){
    return 1023;
  }else if (sAux == "2-5MM"){
    return 1024;
  }else if (sAux == "2MM"){
    return 1025;
  }else if (sAux == "1MM"){
    return 1026;
  }else{
    return -1;
    // If Band is not in this "list" it is still not supported
  }
}

int Adif::mode2Int(const QString& tmode){
//cout << " - Class Adif::mode2Int: " << tmode << endl;

  sAux = tmode.upper();
  if (sAux == "SSB"){
    return 0;
  }else if (sAux == "USB"){  // hamlib adaptation
    return 0;
  }else if (sAux == "LSB"){   // hamlib adaptation
    return 0;
  }else if (sAux == "CW"){
    return 1;
  }else if (sAux == "CWR"){
    return 1;
  }else if (sAux == "FM"){
    return 2;
  }else if (sAux == "PSK31"){
    return 3;
  }else if (sAux == "SSTV"){
    return 4;
  }else if (sAux == "RTTY"){
    return 5;
  }else if (sAux == "RTTYR"){
    return 5;
  }else if (sAux == "MTTY"){
    return 6;
  }else if (sAux == "PKT"){
    return 7;
  }else if (sAux == "AM"){
    return 8;
  }else if (sAux == "FAX"){
    return 9;
  }else if (sAux == "HELL"){
    return 10;
  }else if (sAux == "JT44"){
    return 11;
  }else if (sAux == "MT63"){
    return 12;
  }else if (sAux == "CLO"){
    return 13;
  }else if (sAux == "TOR"){
    return 14;
  }else if (sAux == "GTOR"){
    return 15;
  }else if (sAux == "FSK"){
    return 16;
  }else if (sAux == "MFSK8"){
    return 17;
  }else if (sAux == "MFSK16"){
    return 18;
  }else if (sAux == "HFSK"){
    return 19;
  }else if (sAux == "ATV"){
    return 20;
  }else if (sAux == "PAC"){
    return 21;
  }else if (sAux == "PAC2"){
    return 22;
  }else if (sAux == "PAC3"){
    return 23;
  }else if (sAux == "PCW"){
    return 24;
  }else if (sAux == "PSK63"){
    return 25;
  }else if (sAux == "PSK125"){
    return 26;
  }else if (sAux == "Q15"){
    return 27;
  }else if (sAux == "THRB"){
    return 28;
  }else if (sAux == "ASCI"){
    return 29;
  }else if (sAux == "AMTORFEC"){
    return 30;
  }else if (sAux == "CHIP64"){
    return 31;
  }else if (sAux == "CHIP128"){
    return 32;
  }else if (sAux == "CONTESTI"){
    return 33;
  }else if (sAux == "DSTAR"){
    return 34;
  }else if (sAux == "DOMINO"){
    return 35;
  }else if (sAux == "DOMINOF"){
    return 36;
  }else if (sAux == "FMHELL"){
    return 37;
  }else if (sAux == "FSK31"){
    return 38;
  }else if (sAux == "FSK441"){
    return 39;
  }else if (sAux == "HELL80"){
    return 41;
  }else if (sAux == "JT65"){
    return 42;
  }else if (sAux == "OLIVIA"){
    return 43;
  }else if (sAux == "PAX"){
    return 44;
  }else if (sAux == "PAX2"){
    return 45;
  }else if (sAux == "PSK10"){
    return 46;
  }else if (sAux == "PSK63F"){
    return 47;
  }else if (sAux == "PSKAM10"){
    return 48;
  }else if (sAux == "PSKAM31"){
    return 49;
  }else if (sAux == "PSKAM50"){
    return 50;
  }else if (sAux == "PSKFEC31"){
    return 51;
  }else if (sAux == "PSKHELL"){
    return 52;
  }else if (sAux == "QPSK31"){
    return 53;
  }else if (sAux == "QPSK63"){
    return 54;
  }else if (sAux == "QPSK125"){
    return 55;
  }else if (sAux == "RTTYM"){
    return 56;
  }else if (sAux == "THRBX"){
    return 57;
  }else if (sAux == "VOI"){
    return 58;
  }else{return -1;}
}

int Adif::freq2Int (const QString& tfreq){ // Receives MHz/String
// cout << "Adif::freq2Int: " << tfreq << endl;
	freq = (tfreq.toFloat());
	
   if ((freq >=0.136)&&(freq <=0.137)){
    return 1000;
   }else if ((freq >=1.8)&&(freq <=2.0)){
    return 0;
   }else if ((freq >=3.5)&&(freq <=4.0)){
    return 1;
   }else if ((freq >= 5.102)&&(freq <=5.404)){
    return 2;
   }else if ((freq >= 7)&&(freq <=7.3)){
    return 3;
  }else if ((freq >= 10)&&(freq <=10.15)){
    return 4;
  }else if ((freq >= 14)&&(freq <=14.35)){
    return 5;
  }else if ((freq >= 18.068)&&(freq <=18.168)){
    return 6;
  }else if ((freq >= 21)&&(freq <=21.45)){
    return 7;
  }else if ((freq >= 24.89)&&(freq <=24.99)){
    return 8;
  }else if ((freq >=28)&&(freq <=29.7)){
    return 9;
  }else if ((freq >=50)&&(freq <=54)){
    return 10;
  }else if ((freq >=70)&&(freq <=71)){
    return 11;
  }else if ((freq >=144)&&(freq <=148)){
     return 12;
   }else if ((freq >=220)&&(freq <=225)){
     return 13;
   }else if ((freq >=420)&&(freq <=450)){
     return 14;
   }else if ((freq >=902)&&(freq <=928)){
     return 15;
   }else if ((freq >=1240)&&(freq <=1300)){
     return 16;
   }else if ((freq >=2300)&&(freq <=2450)) {
     return 17;
   }else if ((freq >=3300)&&(freq <=3500)) {
     return 1018;
   }else if ((freq >=5650)&&(freq <=5925)) {
     return 1019;
   }else if ((freq >=10000)&&(freq <=10500)) {
     return 1020;
   }else if ((freq >=24000)&&(freq <=24250)) {
     return 1021;
   }else if ((freq >=47000)&&(freq <=47200)) {
     return 1022;
   }else if ((freq >=75500)&&(freq <=81000)) {
     return 1023;
   }else if ((freq >=119980)&&(freq <=120020)) {
     return 1024;
   }else if ((freq >=142000)&&(freq <=149000)) {
     return 1025;
   }else if ((freq >=241000)&&(freq <=250000)) {
     return 1026;
  }else{
    return -1;
    // If Band is not in this "list" it is still not supported
  }

}

QString Adif::getModeBandPlan(const QString& tfreq){ // (in MHz)
// cout << "Adif::getModeBandPlan: " << tfreq << endl;
	 freq = (tfreq.toFloat());



   if ((freq >=1.810)&&(freq < 1.84)){
    return "CW";
   }else if ((freq >=1.840)&&(freq <=2.000)){
    return "SSB";
   }else if ((freq >=3.5)&&(freq < 3.6)){
    return "CW";
   }else if ((freq >=3.6)&&(freq <=4.0)){
    return "SSB";
   }else if ((freq >=7.0)&&(freq < 7.04)){
    return "CW";
   }else if ((freq >=7.04)&&(freq <=7.2)){
    return "SSB";
   }else if ((freq >=10.1)&&(freq <=10.150)){
    return "CW";
   }else if ((freq >=14.0)&&(freq < 14.07)){
    return "CW";
   }else if ((freq >=14.070)&&(freq <=14.074)){
    return "PSK31";
   }else if ((freq >=14.075)&&(freq <14.1)){
    return "RTTY";
   }else if ((freq >=14.235)&&(freq <=14.24)){
    return "SSTV";
   }else if ((freq >=14.1)&&(freq <=14.35)){
    return "SSB";
   }else if ((freq >=18.06)&&(freq <18.111)){
    return "CW";
   }else if ((freq >=18.111)&&(freq <=18.168)){
    return "SSB";
   }else if ((freq >=21.0)&&(freq <21.15)){
    return "CW";
   }else if ((freq >=21.15)&&(freq <=21.45)){
    return "SSB";
   }else if ((freq >=24.8)&&(freq < 24.94)){
    return "CW";
   }else if ((freq >=24.94)&&(freq <=24.99)){
    return "SSB";
   }else if ((freq >=28.0)&&(freq < 28.32)){
    return "CW";
   }else if ((freq >=28.320)&&(freq <29.5)){
    return "SSB";
   }else if ((freq >=29.500)&&(freq <=29.7)){
    return "FM";
   }else if ((freq >=50.0)&&(freq < 50.1)){
    return "CW";
   }else if ((freq >=50.1)&&(freq <=54.0)){
    return "SSB";
   }else if ((freq >=144.0)&&(freq < 50.18)){
    return "CW";
   }else if ((freq >=144.18)&&(freq <=144.399)){
    return "SSB";
   }else if ((freq >=144.994)&&(freq <=145.806)){
    return "SSB";

  }else{
    return "SSB";
    // If Band is not in this "list" it is still not supported
  }


}

QString Adif::freq2Band (const QString& tfreq){ // MHz
//cout << "Adif::freq2Band: " << tfreq << endl;

	return int2Band(freq2Int(QString::number(((tfreq.left(tfreq.find("."))).toInt()))));
}

QString Adif::int2Band(int tband){
//cout << " - Class Adif::int2Band: " << QString::number(tband) << endl;

	if (tband == 1000){
		return "2190M";
	}else if (tband == 0){
		return "160M";
	}else if (tband == 1){
		return "80M";
	}else if (tband == 2){
		return "60M";
	}else if (tband == 3){
		return "40M";
	}else if (tband == 4){
		return "30M";
	}else if (tband == 5){
		return "20M";
	}else if (tband == 6){
		return "17M";
	}else if (tband == 7){
		return "15M";
	}else if (tband == 8){
		return "12M";
	}else if (tband == 9){
		return "10M";
	}else if (tband == 10){
		return "6M";
	}else if (tband == 11){
		return "4M";
	}else if (tband == 12){
		return "2M";
	}else if (tband == 13){
		return "125CM";
	}else if (tband == 14){
		return "70CM";
	}else if (tband == 15){
		return "33CM";
	}else if (tband == 16){
		return "23CM";
	}else if (tband == 17){
		return "13CM";
	}else if (tband == 1018){
		return "9CM";
	}else if (tband == 1019){
		return "6CM";
	}else if (tband == 1020){
		return "3CM";
	}else if (tband == 1021){
		return "1.25CM";
	}else if (tband == 1022){
		return "6MM";
	}else if (tband == 1023){
		return "4MM";
	}else if (tband == 1024){
		return "2.5MM";
	}else if (tband == 1025){
		return "2MM";
	}else if (tband == 1026){
		return "1MM";
	}else{
		return "NOBAND";
//		return  QString::null;
    // If Band is not in this "list" it is still not supported
	}

}
bool Adif::isCW(const QString& tfreq){ //(MHz)
//cout << " - Class Adif::isCW" << endl;    
  // If i don't recognice if true/false i set FALSE (To help DX-Cluster SH/DX commands)
    if (getModeBandPlan(tfreq)=="CW"){
        return true;
   }else{
       return false;
   }

}

bool Adif::isSSB(const QString& tfreq){
//cout << " - Class Adif::isCW" << endl;    
  // If i don't recognice if true/false i set FALSE (To help DX-Cluster SH/DX commands)
    if (getModeBandPlan(tfreq)=="SSB"){
        return true;
   }else{
       return false;
   }

}

bool Adif::isHF(const QString& tfreq){ // (in MHz)

	iInt = freq2Int(tfreq);
//cout << " - Class Adif::isHF: (freq/numb)" << tfreq <<"/" <<QString::number(iInt)<< endl; 
	// If i don't recognice I set FALSE (To help DX-Cluster SH/DX commands)
	//TODO: The 1000 is to help the 2190M band. Its is pending the band combobox reorganization.
	if (((iInt < 10) && (iInt>=0) ) || (iInt==1000) ){
        	return true;
   	}else{
		return false;
	}

}

bool Adif::isVHF(const QString& tfreq){ // (in MHz)
// To check if 6m band or more only - returns FALSE as a default
//cout << " - Class Adif::isVHF" << tfreq << endl;
	
	iInt = freq2Int(tfreq);
//cout << " - Class Adif::isVHF: (freq/numb)" << tfreq <<"/" <<QString::number(iInt)<< endl; 
	//TODO: The next line will be valid only until I
	// reorder the bands so the 2190M band is HF
	
	if ((iInt >= 10) && (iInt != 1000)){ //6m or more
		return true;
	}else{
		return false;
	}
}


bool Adif::isWARC(const QString& tfreq){ // (in MHz)
//cout << " - Class Adif::isWARC" << endl;    
// Just 30m, 17m or 12m (To help DX-Cluster SH/DX commands)
iInt = freq2Int(tfreq);
  if ((iInt == 4) | (iInt == 6) | (iInt == 8)){ //30m/17m/12m 
     return true;
  }else{
    return false;
  }
}


int Adif::continent2Number(const QString& tcont){
// CONTINENTS AF=0, AN=1, AS=2, EU=3, NA=4, OC=5, SA=6     
  sAux = tcont.stripWhiteSpace();
    if (sAux == "AF"){
    return 0;
  }else if (sAux == "AN"){ 
    return 1;
  }else if (sAux == "AS"){ 
    return 2;
  }else if (sAux == "EU"){
    return 3;
  }else if (sAux == "NA"){
    return 4;
  }else if (sAux == "OC"){
    return 5;
  }else if (sAux == "SA"){
    return 6;
  }else{
  	return -1;
  }
}


QString Adif::number2Continent(int tcont){
// CONTINENTS AF=0, AN=1, AS=2, EU=3, NA=4, OC=5, SA=6     
   if (tcont == 0){
     return "AF";
  }else if (tcont == 1){
    return "AN";
  }else if (tcont == 2){
    return "AS";
  }else if (tcont == 3){
    return "EU";
  }else if (tcont == 4){
    return "NA";
  }else if (tcont == 5){
    return "OC";
  }else if (tcont == 6){
    return "SA";
  }else{
      return "NO-VALID";
    // If the continent is not in this "list" it is still not supported
  }

}

 QString Adif::KHz2MHz(const QString& tfreq){
// // Receives a Freq and returns the same freq but in MHz (in case it was received in KHz)
//cout << "Adif::KHz2MHz: " << tfreq << endl;

	return QString::number((tfreq.toFloat())/1000);

}


int Adif::entity2AdifNumber(const QString& tPref){ // returns the ADIF number of an entity
	if (tPref == "VE"){
		return 1;
	}else if (tPref == "ABU AIL IS"){
		return 2;
	}else if (tPref == "YA"){
		return 3;
	}else if (tPref == "3B6"){
		return 4;
	}else if (tPref == "OH0"){
		return 5;
	}else if (tPref == "KL"){
		return 6;
	}else if (tPref == "ZA"){
		return 7;
	}else if (tPref == "ALDABRA"){
		return 8;
	}else if (tPref == "KH8"){
		return 9;
	}else if (tPref == "FT5Z"){
		return 10;
	}else if (tPref == "VU4"){
		return 11;
	}else if (tPref == "VP2E"){
		return 12;
	}else if (tPref == "CE9"){
		return 13;
	}else if (tPref == "EK"){
		return 14;
	}else if (tPref == "UA9"){
		return 15;
	}else if (tPref == "ZL9"){
		return 16;
	}else if (tPref == "YV0"){
		return 17;
	}else if (tPref == "4J"){
		return 18;
	}else if (tPref == "BAJO NUEVO"){
		return 19;
	}else if (tPref == "KH1"){
		return 20;
	}else if (tPref == "EA6"){
		return 21;
	}else if (tPref == "T8"){
		return 22;
	}else if (tPref == "BLENHEIM REEF"){
		return 23;
	}else if (tPref == "3Y/B"){
		return 24;
	}else if (tPref == "BRITISH N. BORNEO"){
		return 25;
	}else if (tPref == "BRITISH SOMALI"){
		return 26;
	}else if (tPref == "EU"){
		return 27;
	}else if (tPref == "CANAL ZONE"){
		return 28;
	}else if (tPref == "EA8"){
		return 29;
	}else if (tPref == "CELEBE/MOLUCCA IS"){
		return 30;
	}else if (tPref == "T31"){
		return 31;
	}else if (tPref == "EA9"){
		return 32;

	}else{
		return -1;
	}

}

bool Adif::ifValidBand(const QString& tband){
//cout << "Adif::ifValidBand: " << tband << endl;
	if (band2Int(tband)>1){
		return true;
	}else{
		return false;
	}
}

