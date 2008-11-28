//#include <klocale.h>
//#include <kdebug.h>

#include "dxspot.h"


DxSpot::DxSpot(){
	dxcall= "";        // CALL
	dxspotter = "";        // CALL
	numb = 0;;           // STX
//QDateTime dateTime; // QSO_DATE/TIME_ON YYYYMMDD - Time HHMM (0000-2359)
	freq= 0.0;       // Frecuency
//comment;    // COMMENT

}

DxSpot::~DxSpot(){
}


void DxSpot::setDxCall(const QString &tdxcall){
	dxcall = tdxcall;
}

QString DxSpot::getDxCall() const{
	return dxcall;
}

void DxSpot::setSpotter(const QString &tdxcall){}
QString DxSpot::getSpotter() const{}
		
void DxSpot::setNumb (int tnumb){}
int DxSpot::getNumb() const{}

void DxSpot::setDateTime(QDateTime tdateTime){}
QDateTime DxSpot::getDateTime() const{}

void DxSpot::setDate(QDate tdate){}
QDate DxSpot::getDate() const{}

void DxSpot::setTime(QTime ttime){}
QTime DxSpot::getTime() const{}

void DxSpot::setFreq(double tfreq){}
double DxSpot::getFreq() const{}

void DxSpot::clearDXSpot(){}

void DxSpot::setcomment(const QString &tcomment){}
QString DxSpot::getcomment() const{}




