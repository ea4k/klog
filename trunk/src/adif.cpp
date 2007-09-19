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
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the Free Software              *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*
 *****************************************************************************/

//#include <klocale.h>
//#include <kdebug.h>
//#include <ostream.h>
#include "adif.h"
/*
This class is really not independant FROM the GUI as the numbers to codify the
MODE and BANDS depend on the position of the band in the COMBO-box.
The BAND is being changed... the mode should be changed!

Please follow this table of equivalences:

BAND	FREQS (MHz)	GUI
160M	1,2		0
80M	3,4		1
60M	5		2
40M	7		3
30M	10		4
20M	14		5
17M	18		6	
15M	21		7
12M	24		8
10M	28,29		9
6M	50...54		10
2M	144...14	11
125CM  219...225	12
70CM  420...450		13
33CM  902...928		14
23CM  1240...1300	15
13CM  2300...2450	16
*/


Adif::Adif(){
//cout << " - Class Adif" << endl;
}

Adif::~Adif(){
//cout << " - Class ~Adif" << endl;    
}


int klog2Adif(int klogN){
// At this moment this conversion is just adding 10 but
// In case of adding a new band, this may change.
// The real code is commented and this is just a hack to save execution time
// using the first statement! ;-)    
return klogN+10;
/*
if (klogN = 0)
    return 10;
else if (klogN = 1)
    return 11;
else if (klogN = 2)
    return 12;
else if (klogN = 3)
    return 13;
else if (klogN = 4)
    return 14;
else if (klogN = 5)
    return 15;
else if (klogN = 6)
    return 16;
else if (klogN = 7)
    return 17;
else if (klogN = 8)
    return 18;
else if (klogN = 9)
    return 19;
else if (klogN = 10)
    return 20;
else if (klogN = 11)
    return 21;
else if (klogN = 12)
    return 22;
else if (klogN = 13)
    return 23;
else if (klogN = 14)
    return 24;
else if (klogN = 15)
    return 25;
else if (klogN = 16)
    return 26;
else
    return -1;
*/
}

int adif2Klog(int adifN){
// At this moment this conversion is just sustracting 10 but
// In case of adding a new band, this may change.
// The real code is commented and this is just a hack to save execution time
// using the first statement! ;-)    
return adifN-10;
/*
if (adifN = 10)
    return 0;
else if (adifN = 11)
    return 1;
else if (adifN = 12)
    return 2;
else if (adifN = 13)
    return 3;
else if (adifN = 14)
    return 4;
else if (adifN = 15)
    return 5;
else if (adifN = 16)
    return 6;
else if (adifN = 17)
    return 7;
else if (adifN = 18)
    return 8;
else if (adifN = 19)
    return 9;
else if (adifN = 20)
    return 10;
else if (adifN = 1)
    return 11;
else if (adifN = 22)
    return 12;
else if (adifN = 23)
    return 13;
else if (adifN = 24)
    return 14;
else if (adifN = 25)
    return 15;
else if (adifN = 26)
    return 16;
else
    return -1;
*/
}

int Adif::band2Int(const QString& tband){
/*****************************************************************    
    // I think that THE RETURN of freq2Int and Band2Int are not coherent
BUG BUG BUG BUGBUG BUG BUG BUGBUG BUG BUG BUGBUG BUG BUG BUGBUG BUG BUG BUG  
*****************************************************************/
//cout << " - Class Adif::band2Int" << endl;
  sAux = tband;
  if (tband == "160M"){
    return 0;
  }else if (tband == "80M"){
    return 1;
  }else if (tband == "60M"){
    return 2;
  }else if (tband == "40M"){
    return 3;
  }else if (tband == "30M"){
    return 4;
  }else if (tband == "20M"){
    return 5;
  }else if (tband == "17M"){
    return 6;
  }else if (tband == "15M"){
    return 7;
  }else if (tband == "12M"){
    return 8;
  }else if (tband == "10M"){
    return 9;
  }else if (tband == "6M"){
    return 10;
  }else if (tband == "2M"){
    return 11;
  }else if (tband == "125CM"){
     return 12;
  }else if (tband == "70CM"){
    return 13;
  }else if (tband == "33CM"){
    return 14;
  }else if (tband == "23CM"){
    return 15;
  }else if (tband == "13CM"){
    return 16;
  }else{
    return -1;
    // If Band is not in this "list" it is still not supported
  }
}

int Adif::mode2Int(const QString& tmode){
//cout << " - Class Adif::mode2Int" << endl;    
  sAux = tmode;
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
  }else{return -1;}
}

int Adif::freq2Int (const QString& tfreq){ // Receives KHz
/*****************************************************************    
    // I think that THE RETURN of freq2Int and Band2Int are not coherent
BUG BUG BUG BUGBUG BUG BUG BUGBUG BUG BUG BUGBUG BUG BUG BUGBUG BUG BUG BUG  
*****************************************************************/
//cout << "Adif::freq2Int: " << tfreq << endl;
   sAux = tfreq;
   iInt = (tfreq.left(tfreq.find("."))).toInt();
   //iInt = sAux.toInt();
//   cout << " - Class Adif::freq2Int String/NumInt: " << sAux << "/" << QString::number(iInt) << endl;
   if ((iInt >=1000)&&(iInt <=2500)){
    return 0;
   }else if ((iInt >=3000)&&(iInt <=4500)){
    return 1;
   }else if ((iInt >= 5000)&&(iInt <=5500)){
    return 2;
   }else if ((iInt >= 7000)&&(iInt <=7500)){
    return 3;
  }else if ((iInt >= 10000)&&(iInt <=10200)){
    return 4;
  }else if ((iInt >= 14000)&&(iInt <=14350)){
    return 5;
  }else if ((iInt >= 18000)&&(iInt <=18200)){
    return 6;
  }else if ((iInt >= 21000)&&(iInt <=21450)){
    return 7;
  }else if ((iInt >= 24800)&&(iInt <=25000)){
    return 8;
  }else if ((iInt >=28000)&&(iInt <=29900)){
    return 9;
  }else if ((iInt >=50000)&&(iInt <=55000)){
    return 10;
  }else if ((iInt >=144000)&&(iInt <=149000)){
     return 11;
   }else if ((iInt >=220000)&&(iInt <=225000)){
     return 12;
   }else if ((iInt >=420000)&&(iInt <=450000)){
     return 13;
   }else if ((iInt >=902000)&&(iInt <=928000)){
     return 14;
   }else if ((iInt >=1240000)&&(iInt <=1300000)){
     return 15;
   }else if ((iInt >=2300000)&&(iInt <=2550000)) {
     return 16;
  }else{
    return -1;
    // If Band is not in this "list" it is still not supported
  }

}

QString Adif::freq2Band (const QString& tfreq){
	//cout << "Adif::freq2Band: " << tfreq << endl;

	return int2Band(freq2Int(QString::number(((tfreq.left(tfreq.find("."))).toInt()))));
//return int2Band( freq2Int(1000*QString::number(tfreq.left(tfreq.find(".") ) ) ) );
}

QString Adif::int2Band(int tband){
//cout << " - Class Adif::int2Band: " << QString::number(tband) << endl;
	if (tband == 0){
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
		return "2M";
	}else if (tband == 12){
		return "125CM";
	}else if (tband == 13){
		return "70CM";
	}else if (tband == 14){
		return "33CM";
	}else if (tband == 15){
		return "23CM";
	}else if (tband == 16){
		return "13CM";
	}else{
		return "";
    // If Band is not in this "list" it is still not supported
	}

}

bool Adif::isHF(const QString& tfreq){
//cout << " - Class Adif::isHF" << endl;    
  // If i don't recognice if true/false i set FALSE (To help DX-Cluster SH/DX commands)
    if ((freq2Int(tfreq) < 10) && (freq2Int(tfreq)>=0) ){
        return true;
   }else{
       return false;
   }

}

bool Adif::isVHF(const QString& tfreq){
// To check if 6m band or more only    
//cout << " - Class Adif::isVHF" << endl;    
  // I return FALSE as a default
  if (freq2Int(tfreq) >= 10){ //6m or more
     return true;
  }else{
    return false;
  }
}


bool Adif::isWARC(const QString& tfreq){
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



