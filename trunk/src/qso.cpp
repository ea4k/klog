/***************************************************************************
                          qso.cpp  -  description
                             -------------------
    begin                : jue ago 15 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@robles.nu
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
#include <klocale.h>
//#include <kdebug.h>
//#include <ostream.h>
#include "qso.h"


Qso::Qso(){
}

Qso::Qso(const QString& tqrz, int tnumb, QString& tmode){
  qrz = tqrz;
  numb = tnumb;
  mode = tmode;
  freq = "";
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
  localAward = ""; // LocalAward has to be "null" if empty.
myGridSquare = "NULL";

}


Qso::Qso(const int tnumb, const QString& tqrz, const int trstrx, const int trsttx, int tband, const QString& tmode, const int tpower){
    qrz = tqrz;
    numb = tnumb;
    mode = tmode;
    rstrx = trstrx;
    rsttx = trsttx;
    band = tband;
    freq = "";
    mode = tmode;
    power = tpower;
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
myGridSquare = "NULL";
}


Qso::Qso(const int tnumb, const QDateTime tdateTime, const QString& tqrz, const int trstrx, const int trsttx, const QString& tband, const QString& tmode, const QString& tpower, const QString& tcomment)
{
  qrz = checkisNotAdif(tqrz);
  numb = tnumb;
  dateTime = tdateTime;
  rstrx = trstrx;
  rsttx = trsttx;
  band = checkisNotAdif(tband);
  freq = "";
  mode = checkisNotAdif(tmode);
  power = checkisNotAdif(tpower);
  comment = checkisNotAdif(tcomment);
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
myGridSquare = "NULL";

}

Qso::Qso (const int tnumb, const QDateTime& tdateTime, const QString& tqrz, const int trstrx, const int trsttx, int tband, const QString& tmode, const int tpower, const QDate& tqslSen,  const QDate& tqslRec, const QString& tcomment){
  qrz = checkisNotAdif(tqrz);
  numb = tnumb;
  dateTime = tdateTime;
  rstrx = trstrx;
  rsttx = trsttx;
  band = tband;
  freq = "";
  mode = checkisNotAdif(tmode);
  power = tpower;
  qslSen = tqslSen;
  qslRec = tqslRec;
  comment = checkisNotAdif(tcomment);
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
myGridSquare = "NULL";
}


Qso::~Qso(){
}

void Qso::setQrz(const QString &tqrz ){

  qrz = checkisNotAdif(tqrz);
}

QString Qso::getQrz() const {
  return qrz;
}


void Qso::setNumb (int tnumb){
  numb = tnumb  ;
}

int Qso::getNumb() const{
    return numb;}

void Qso::setDateTime(QDateTime tdateTime){
    dateTime = tdateTime;}

QDateTime Qso::getDateTime() const{return dateTime;}

void Qso::setRstrx(int trstrx){
    rstrx = trstrx;  
}

int Qso::getRstrx() const{
    return rstrx;}

void Qso::setRsttx(int trsttx){
    rsttx = trsttx;
}

int Qso::getRsttx() const{
    return rsttx;}

void Qso::setBand (const QString& tband){
    band = checkisNotAdif(tband);
}

QString Qso::getBand() const{
    return band;}

void Qso::setFreq (const QString& tfreq){
    freq = checkisNotAdif(tfreq);}

QString Qso::getFreq() const{
    return freq;}

void Qso::setMode(const QString& tmode){
    mode = checkisNotAdif(tmode);
}

QString Qso::getMode() const{
    return mode;}

void Qso::setPower(const QString& tpower){
    power = checkisNotAdif(tpower);
}

QString Qso::getPower() const{
    return power;}

void Qso::setrxPower(const QString& tpower){
//	cout << "Qso::setrxPower: " << rx_power << endl;
    rx_power = checkisNotAdif(tpower);
}

QString Qso::getrxPower() const{
    return rx_power;}

void Qso::setComment(const QString &tcomment){
    comment = checkisNotAdif(tcomment);
}
QString Qso::getComment()const{
    return comment;}

void Qso::setQslSenDateOn(QDate tdate){
  qslSen = tdate;
  qslSenc = 'Y';
}

QDate Qso::getQslSenDate() const{
  return qslSen;
}

void Qso::QslSent(const QChar& qslState){
  qslSenc = qslState;
}

QChar Qso::isQslSent(){
  return  qslSenc;  
}

void Qso::setQslRecDateOn(const QDate tdate){
  qslRec = tdate;
  qslRecc = 'Y';
}

QDate Qso::getQslRecDate() const{
  return qslRec;
  
}

void Qso::QslRec(const QChar& qslState){
  qslRecc = qslState;
}

QChar Qso::isQslRec(){
  return qslRecc;  
}

bool Qso::gotTheQSL(){  // Ask if i have his QSL
  if (qslRecc == 'Y')
    return true;
  else
    return false;
}

bool Qso::sentTheQSL(){ // Did i sent my QSL?
  if (qslSenc == 'Y')
    return true;
  else
    return false;
}

void Qso::setQslVia(const QString &tInfo){
  qslVia = checkisNotAdif(tInfo);
}

QString Qso::getQslVia() const{
  return qslVia;
}

void Qso::setQslManager(const QString &tqrz){
  manager = checkisNotAdif(tqrz);

} 

QString Qso::getQslManager() const {
    return manager;}

void Qso::setQslInfo(const QString &tqslInfo){
  qslInfo = checkisNotAdif(tqslInfo);

}

QString Qso::getQslInfo()const{
    return qslInfo;}


void Qso::setLocator(const QString &tgridSquare ){
    gridSquare = checkisNotAdif(tgridSquare);
}

QString Qso::getLocator() const {
  return gridSquare;
}

void Qso::setMyLocator(const QString &tgridSquare ){
    myGridSquare = checkisNotAdif(tgridSquare);
}

QString Qso::getMyLocator() const {
  return myGridSquare;
}

void Qso::setIota(const QString &tiota){
// TODO:
// It is necessary to check the received QString to  allow only valid IOTA
// references (CC-XXX)
  iota = checkisNotAdif(tiota);
}

QString Qso::getIota() const{
    return iota;}

void Qso::setLocalAward(const QString &taward){
  localAward = checkisNotAdif(taward);
}

QString Qso::getLocalAward() const{
    return localAward;
}

void Qso::setLocalAwardNumber(int taward){
  localAwardNumber = taward;
}

int Qso::getLocalAwardNumber() const{
    return localAwardNumber;
}

QString Qso::getIotaContinent() const{
  return iota.section('-',0,0);
}

int Qso::getIotaNumber() const {
  return (iota.section('-',1,1)).toInt();
}

void Qso::setName(const QString &tname){
  name = checkisNotAdif(tname);
}

QString Qso::getName() const{
  return name;  
}

void Qso::setQth(const QString &tqth){
  qth = checkisNotAdif(tqth);
}

QString Qso::getQth() const{
  return qth;
}

void Qso::setOperator(const QString &toperator){
    callUsed = checkisNotAdif(toperator);
}

QString Qso::getOperator() const{
  return callUsed;
}

void Qso::setPropMode(const QString &tprop_mode){
  prop_mode = checkisNotAdif(tprop_mode);
}

QString Qso::getPropMode() const{
  return prop_mode;
}

void Qso::setSrx (int tnumb){srx = tnumb;};
int Qso::getSrx() const{return srx;};
void Qso::setSrx_string(const QString &tsrx){srx_string = checkisNotAdif(tsrx);};
QString Qso::getSrx_string() const{return srx_string;};
void Qso::setStx (int tnumb){stx = tnumb;};
int Qso::getStx() const {return stx;};
void Qso::setStx_string(const QString &tstx){stx_string = checkisNotAdif(tstx);};
QString Qso::getStx_string() const{return stx_string;};


void Qso::clearQso(){
//qrz = "";        // CALL
//numb = 0;           // STX
//  QDateTime dateTime; // QSO_DATE/TIME_ON YYYYMMDD - Time HHMM (0000-2359)
/*  int rstrx;          // RST_RCVD
  int rsttx;          // RST_SENT
//band;       // BAND
  QString mode;       // MODE
  QString rx_power;   // RX_PWR
  QString power;      // TX_PWR
  QDate qslSen;       // QSLSDATE
  QDate qslRec;       // QSLRDATE
  
*/  
	qslRecc = 'N';      // QSL_RCVD Y, N, R, I
	qslSenc = 'N';      // QSL_SENT Y, N, R, I
	qslVia = "";     // QSL_VIA (C) //Manager
	qslInfo = ""; //;    // ADDRESS /QSLMSG
	comment = "";    // COMMENT
	//  int qslHow;         // How is the qsl to be send
	manager = "";
	gridSquare = "";
	iota = "AF-0";   // GRIDSQUARE
	callUsed = "";   // OPERATOR
	name = "";       // NAME
	qth = "";        // QTH
	localAward = "0"; // LocalAward
	localAwardNumber = 0; // Just for managing, not saved in the log
	freq = "0.0"; // Frecuency
	rx_power = "0"; //RX_PWR
	power = "0"; //TX_PWR
}

QString Qso::checkisNotAdif(const QString& tword){
//cout << " - Class Qso::checkAdif" << endl;
	sAux = tword;
	if (sAux.contains('<')>0){
		sAux.replace( QChar('<'), QChar('('));
	}
	if (sAux.contains('>')>0){
		sAux.replace( QChar('>'), QChar(')'));
	}
	return checkLines(sAux);
}


QString Qso::checkLines(const QString& tword){
//cout << " - Class Qso::checkLines" << endl;
	sAux = tword;
	if (sAux.contains('\n')>0){
		sAux.replace( QChar('\n'), "\\n");
	}
	return sAux;


}
