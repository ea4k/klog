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

QString DxSpot::getDxCall() const{
	return dxcall;
}

void DxSpot::setSpotter(const QString &tdxcall){
  dxspotter = tdxcall;
}
QString DxSpot::getSpotter() const{return "NULL";}
		
void DxSpot::setNumb (int tnumb){numb = tnumb;}
int DxSpot::getNumb() const{return numb;}

void DxSpot::setDateTime(QDateTime tdateTime){dateTime = tdateTime;}
QDateTime DxSpot::getDateTime() const{return dateTime;}

void DxSpot::setDate(QDate tdate){}
QDate DxSpot::getDate() const{}

void DxSpot::setTime(QTime ttime){}
QTime DxSpot::getTime() const{}

void DxSpot::setFreq(double tfreq){freq = tfreq;}

double DxSpot::getFreq() const{return freq;}

void DxSpot::clearDXSpot(){}

void DxSpot::setcomment(const QString &tcomment){
  comment = tcomment;
}
QString DxSpot::getcomment() const{return comment;}




