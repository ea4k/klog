//#include <klocale.h>
//#include <kdebug.h>

#include "dxspot.h"


DxSpot::DxSpot(){
	dxcall= "";        // CALL
	dxspotter = "";        // CALL
	numb = 0;;           // STX
//QDateTime dateTime; // QSO_DATE/TIME_ON YYYYMMDD - Time HHMM (0000-2359)
	freq= 0.0;       // Frecuency
	comment="";    // COMMENT

}

DxSpot::~DxSpot(){
}


void DxSpot::setDxCall(const QString &tdxcall){
	dxcall = tdxcall;
}

void DxSpot::setSpotter(const QString &tdxcall){
  dxspotter = tdxcall;
}
QString DxSpot::getSpotter() {return "NULL";}
		
void DxSpot::setNumb (int tnumb){numb = tnumb;}
int DxSpot::getNumb() {return numb;}

void DxSpot::setDateTime(QDateTime tdateTime){dateTime = tdateTime;}
//QDateTime DxSpot::getDateTime() {return dateTime;}

void DxSpot::setDate(QDate tdate){  dateTime.setDate(tdate);}
//QDate DxSpot::getDate() const{return dateTime.date();}

void DxSpot::setTime(QTime ttime){dateTime.setTime(ttime);}
//QTime DxSpot::getTime() const{return dateTime.time();}

void DxSpot::setFreq(double tfreq){freq = tfreq;}

//double DxSpot::getFreq() const{return freq;}

void DxSpot::clearDXSpot(){}

void DxSpot::setcomment(const QString &tcomment){
  comment = tcomment;
}

//QString DxSpot::getcomment() const{return comment;}




