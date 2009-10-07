/***************************************************************************
                          qso.cpp  -  description
                             -------------------
    begin                : jue ago 15 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@robles.es
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
#include "qso.h"
//using namespace std;

Qso::Qso(){}

Qso::Qso(const QString& tqrz, int tnumb, QString& tmode){
  qrz = tqrz;
  numb = tnumb;
  mode = tmode;
  freq = "";
  freq_rx = "";
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
  localAward = ""; // LocalAward has to be "null" if empty. (it was just "")
  myGridSquare = "NULL";
  stx=-1;
  srx=-1;
        nr_pings = -1;
        sfi=-1;
  stx_string="";
  srx_string="";
  dxcc = 0;
  contest_id="";
  station_callsign="";
  dateTime = QDateTime::fromString("00000000000000", "yyyyMMddHHmmss");
}


Qso::Qso(const int tnumb, const QString& tqrz, const int trstrx, const int trsttx, const QString& tmode, const QString& tband, const int tpower){
    qrz = tqrz;
    numb = tnumb;
    mode = tmode;
    rstrx = trstrx;
    rsttx = trsttx;
    band = tband;
    freq = "";
    freq_rx = "";
    mode = tmode;
    power = tpower;
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
  myGridSquare = "NULL";
        dxcc = 0;
  stx=-1;
  srx=-1;
        sfi=-1;
        nr_pings = -1;
        stx_string="";
  srx_string="";
  contest_id="";
  station_callsign="";
  dateTime = QDateTime::fromString("00000000000000", "yyyyMMddHHmmss");
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
  freq_rx = "";
  mode = checkisNotAdif(tmode);
  power = checkisNotAdif(tpower);
  comment = checkisNotAdif(tcomment);
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
  myGridSquare = "NULL";
  dxcc = 0;
  stx=-1;
  srx=-1;
  sfi=-1;
  nr_pings = -1;
  stx_string="";
  srx_string="";
  contest_id="";
  station_callsign="";
  check="";
}

Qso::Qso (const int tnumb, const QDateTime& tdateTime, const QString& tqrz, const int trstrx, const int trsttx, int tband, const QString& tmode, const int tpower, const QDate& tqslSen,  const QDate& tqslRec, const QString& tcomment){
  qrz = checkisNotAdif(tqrz);
  numb = tnumb;
  dateTime = tdateTime;
  rstrx = trstrx;
  rsttx = trsttx;
  band = tband;
  freq = "";
  freq_rx = "";
  mode = checkisNotAdif(tmode);
  power = tpower;
  qslSen = tqslSen;
  qslRec = tqslRec;
  comment = checkisNotAdif(tcomment);
  qslRecc = 'N';      // QSL_RCVD Y, N, R, I
  qslSenc = 'N';      // QSL_SENT Y, N, R, I
  myGridSquare = "NULL";
  dxcc = 0;
  stx=-1;
  srx=-1;
  nr_pings = -1;
        sfi=-1;
  stx_string="";
  srx_string="";
  contest_id="";
  station_callsign="";
}

Qso::~Qso(){}

void Qso::setAge (int tnumb){age = tnumb;};	// Contacted operator age in years
int Qso::getAge() const{return age;};		// Contacted operator age in years
void Qso::setA_index (int tnumb){a_index = tnumb;}; // geomagnetic A index at the time of the QSO
int Qso::getA_index() const{return a_index;};	// geomagnetic A index at the time of the QSO
void Qso::setK_index (int tnumb){k_index = tnumb;}; // geomagnetic K index at the time of the QSO
int Qso::getK_index() const{return k_index;};	// geomagnetic K index at the time of the QSO
void Qso::setMaxBursts (int tnumb){maxbursts = tnumb;};
int Qso::getMaxBursts() const{return maxbursts;};
void Qso::setNRBursts (int tnumb){nr_bursts = tnumb;};
int Qso::getNRBursts() const{return nr_bursts;};
void Qso::setNRPings (int tnumb){nr_pings = tnumb;};
int Qso::getNRPings() const{return nr_pings;};
void Qso::setSFI (int tnumb){sfi = tnumb;};
int Qso::getSFI() const{return sfi;};

void Qso::setSatMode(const QString &tsat){
        sat_mode = tsat;
}

QString Qso::getSatMode() const{return sat_mode;}

void Qso::setSatName(const QString &tsat){
        sat_name = tsat;
}

QString Qso::getSatName() const{return sat_name;}

void Qso::setMSShower(const QString &tmsshower){	//For Meteor Scatter QSOs, the name of the meteor shower in progress
        msshower = checkisNotAdif(tmsshower);
}
 QString Qso::getMSShower() const{return msshower;}


void Qso::setOwnerCall(const QString &tstring){
        owner_callsign = checkisNotAdif(tstring);
}

QString Qso::getOwnerCall() const{return owner_callsign;}

void Qso::setPfx(const QString &tstring){
        pfx = checkisNotAdif(tstring);
}

QString Qso::getPfx() const{return pfx;}

void Qso::setPrecedence(const QString &tstring){
        precedence = checkisNotAdif(tstring);
}

QString Qso::getPrecedence() const{return precedence;}

void Qso::setAnt_az (int tnumb){	//Antenna azimut in degrees
        if ((tnumb>=0)&&(tnumb<=360))
                ant_az = tnumb;
}

int Qso::getAnt_az() const{return ant_az;};		//Antenna azimut in degrees

void Qso::setAnt_el (int tnumb){		//Antenna elevation in degrees
        if ((tnumb>=0)&&(tnumb<=90))
                ant_el = tnumb;
}

int Qso::getAnt_el() const{return ant_az;};		//Antenna elevation in degrees

void Qso::setQrz(const QString &tqrz ){
//	cout << "QSO::setQrz: " << tqrz << endl;
        qrz = checkisNotAdif(tqrz);
}

QString Qso::getQrz() const {
//	cout << "QSO::getQrz " << endl;
        return qrz;
}

void Qso::setNumb (int tnumb){
//	cout << "QSO::setNumb: " << QString::number(tnumb) << endl;
        numb = tnumb  ;
}

int Qso::getNumb() const{
//	cout << "QSO::getNumb: " << QString::number(numb) <<endl;
        return numb;
}

void Qso::setDateTime(QDateTime tdateTime){
        //cout << "QSO::setDateTime: " << tdateTime.toString() << endl;
        dateTime = tdateTime;
}

QDateTime Qso::getDateTime() const{
        //cout << "QSO::getDateTime: " << dateTime.toString() <<endl;
        return dateTime;
}

void Qso::setDate(QDate tdate){
    dateTime.setDate(tdate);
}

QDate Qso::getDate() const{
    return dateTime.date();
}

void Qso::setTime(QTime ttime){
    dateTime.setTime(ttime);
}

QTime Qso::getTime() const{
    return dateTime.time();
}

void Qso::setRstrx(int trstrx){
//	cout << "QSO::setRstrx: " << QString::number(trstrx) <<endl;
        rstrx = trstrx;
}

int Qso::getRstrx() const{
//	cout << "QSO::getRstrx: " << QString::number(rstrx) <<endl;
    return rstrx;
}

void Qso::setRsttx(int trsttx){
//	 << "QSO::setRsttx: " << QString::number(trsttx) << endl;
        rsttx = trsttx;
}

int Qso::getRsttx() const{
//	cout << "QSO::getRsttx: " << QString::number(rsttx) << endl;
        return rsttx;
}

void Qso::setCQz (int tnumb){
        if ((tnumb>=0) && (tnumb<=40)){
                cqz = tnumb;
        }else{
                cqz = -1;
        }
}

int Qso::getCQz() const {return cqz;}

void Qso::setITUz (int tnumb){
//TODO: Check how many ITU zones are O:-)
        if ((tnumb>=0) && (tnumb<=100)){
                ituz = tnumb;
        }else{
                ituz = -1;
        }

}

int Qso::getITUz() const{return ituz;}

void Qso::setMyCQz (int tnumb){
        if ((tnumb>=0) && (tnumb<=40)){
                my_cqz = tnumb;
        }else{
                my_cqz = -1;
        }
}

int Qso::getMyCQz() const {return my_cqz;}

void Qso::setMyITUz (int tnumb){
//TODO: Check how many ITU zones are, 90?
        if ((tnumb>=0) && (tnumb<=100)){
                my_ituz = tnumb;
        }else{
                my_ituz = -1;
        }

}
int Qso::getMyITUz() const{return my_ituz;}


void Qso::setDistance (int tnumb){
        if (tnumb>0){
                distance = tnumb;
        }else{
                distance = -1;
        }

}
int Qso::getDistance() const{return distance;}


void Qso::setBand (const QString& tband){
//TODO: I may set also a default freq per band when calling this function
// 100 when 20m, 21000 when 15m and so on...
// Some kind of checking would be great.
// Importing adif.h and using "bool ifValidBand" from here would be ok but
// I am trying to avoid importing adif.h here.
        //cout << "QSO::setBand: " << tband << endl;
        band = checkisNotAdif(tband);
}

QString Qso::getBand() const{
        //cout << "QSO::getBand: " << band << endl;
        return band;
}

void Qso::setBand_RX (const QString& tband){
//TODO: I may set also a default freq per band when calling this function
// 14000 when 20m, 21000 when 15m and so on...
// Some kind of checking would be great.
// Importing adif.h and using "bool ifValidBand" from here would be ok but
// I am trying to avoid importing adif.h here.
        //cout << "QSO::setBand: " << tband << endl;
        band_rx = checkisNotAdif(tband);
}

QString Qso::getBand_RX() const{
        //cout << "QSO::getBand: " << band << endl;
        return band_rx;
}

void Qso::setFreq (const QString& tfreq){
//TODO: When setting the frequency I should set also the band
//	cout << "QSO::setFreq: " << tfreq << endl;
        freq = checkisNotAdif(tfreq);
}

QString Qso::getFreq() const{
//	cout << "QSO::getFreq: " << freq << endl;
        return freq;
}

void Qso::setFreq_RX (const QString& tfreq){
//TODO: When setting the frequency I should set also the band
//	cout << "QSO::setFreq: " << tfreq << endl;
        freq_rx = checkisNotAdif(tfreq);
}

QString Qso::getFreq_RX() const{
//	cout << "QSO::getFreq: " << freq << endl;
        return freq_rx;
}

void Qso::setMode(const QString& tmode){
    mode = checkisNotAdif(tmode);
}

void Qso::setDXCC (int tnumb){dxcc = tnumb;}

int Qso::getDXCC() const{return dxcc;};

void Qso::setCountry(const QString& tcountry){	// ADIF DXCC Country name
        country = checkisNotAdif(tcountry);
}

QString Qso::getCountry() const{return country;}			// ADIF DXCC Country name

void Qso::setMyCountry(const QString& tcountry){	// ADIF DXCC Country name
        my_country = checkisNotAdif(tcountry);
}

QString Qso::getMyCountry() const{return my_country;}			// ADIF DXCC Country name

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
        //cout << "QSO::setQslSenDateOn: " << tdate.toString() << endl;
        if (tdate.isValid()){
                qslSen = tdate;
        }
        qslSenc = 'Y';
}

QDate Qso::getQslSenDate() const{
  return qslSen;
}

void Qso::QslSent(const QChar& qslState){
//cout << "QSO:QslSent: " << qslState << endl;
// Y, N, R, Q, I

        if ( (qslState=='Y') || (qslState=='R') || (qslState=='I') || (qslState=='Q') ){
                qslSenc = qslState;
        }else{
                qslSenc = 'N';
        }


}

void Qso::QslRec(const QChar& qslrState){
//cout << "QSO: (" << getQrz() << ")QslRec: " << qslrState << endl;
//Y, N, R, I. V
        if ( (qslrState=='Y') || (qslrState=='R') || (qslrState=='I') || (qslrState=='V') ) {
        qslRecc = qslrState;
        }else{
                qslRecc = 'N';
        }
}

QChar Qso::isQslSent(){
  return  qslSenc;
}

void Qso::setQslRecDateOn(QDate tdate){
        //cout << "QSO::setQslRecDateOn: " << tdate.toString() << endl;
        if (tdate.isValid()){
                qslRec = tdate;
        }
        qslRecc = 'Y';
}

QDate Qso::getQslRecDate() const{
  return qslRec;

}

QChar Qso::isQslRec(){
  return qslRecc;
}

bool Qso::gotTheQSL(){  // Ask if i have his QSL
//cout << "QSO::gotTheQSL: " << getQrz() << endl;
        if (qslRecc == 'Y'){
                return true;
        }else{
                return false;
        }
}

bool Qso::sentTheQSL(){ // Did i sent my QSL?
  if (qslSenc == 'Y')
    return true;
  else
    return false;
}

void Qso::setQslVia(const QString &tInfo){
        //cout << "QSO::setQslVia: " << tInfo << endl;
        qslVia = checkisNotAdif(tInfo);
}

QString Qso::getQslVia() const{
  return qslVia;
}

void Qso::setQslManager(const QString &tqrz){
        //cout << "QSO::setQslManager: " << tqrz << endl;
        manager = checkisNotAdif(tqrz);

}

QString Qso::getQslManager() const {
    return manager;}

void Qso::setQslInfo(const QString &tqslInfo){
        //cout << "QSO::setQslInfo: " << tqslInfo << endl;
        qslInfo = checkisNotAdif(tqslInfo);

}

QString Qso::getQslInfo()const{
        return qslInfo;
}

void Qso::seteQslSenDateOn(QDate tdate){
        //cout << "QSO::setQslSenDateOn: " << tdate.toString() << endl;
        if (tdate.isValid()){
                eqsl_qslsdate = tdate;
        }
        eQSL_qsl_sent = 'Y';
}

QDate Qso::geteQslSenDate() const{
  return eqsl_qslsdate;
}

void Qso::seteQslRecDateOn(QDate tdate){
        //cout << "QSO::setQslSenDateOn: " << tdate.toString() << endl;
        if (tdate.isValid()){
                eqsl_qslrdate = tdate;
        }
        eQSL_qsl_rcvd = 'Y';
}

QDate Qso::geteQslRecDate() const{
  return eqsl_qslrdate;
}

void Qso::seteQslSent(const QChar& eqslState){ // eQSL_Sent Y, N, R, Q, I
        if ((eqslState == 'Y') || (eqslState == 'N') || (eqslState == 'R') || (eqslState == 'Q') || (eqslState == 'I') ){
                eQSL_qsl_sent = eqslState;
        }
}
QChar Qso::geteQslSent(){return eQSL_qsl_sent;}

void Qso::seteQslRcvd(const QChar& eqslState){  // eQSL_RCVD Y, N, R, I, V
        if ((eqslState == 'Y') || (eqslState == 'N') || (eqslState == 'R') || (eqslState == 'I') || (eqslState == 'V') ){
                eQSL_qsl_rcvd = eqslState;
        }

}
QChar Qso::geteQslRcvd(){return eQSL_qsl_rcvd;}

void Qso::setLocator(const QString &tgridSquare ){
        //cout << "QSO::setLocator: " << tgridSquare << endl;
        gridSquare = checkisNotAdif(tgridSquare);
}

QString Qso::getLocator() const {
  return gridSquare;
}

void Qso::setMyLocator(const QString &tgridSquare ){
        //cout << "QSO::setMyLocator: " << tgridSquare << endl;
        myGridSquare = checkisNotAdif(tgridSquare);
}

QString Qso::getMyLocator() const {
  return myGridSquare;
}

void Qso::setIota(const QString &tiota){
// TODO:
// It is necessary to check the received QString to  allow only valid IOTA
// references (CC-XXX)
        //cout << "QSO::setIota: " << tiota << endl;
        iota = checkisNotAdif(tiota);
}

QString Qso::getIota() const{
    return iota;}

void Qso::setMyIota(const QString &tiota){
// TODO:
// It is necessary to check the received QString to  allow only valid IOTA
// references (CC-XXX)
        //cout << "QSO::setIota: " << tiota << endl;
        my_iota = checkisNotAdif(tiota);
}

QString Qso::getMyIota() const{
    return my_iota;}

void Qso::setLocalAward(const QString &taward){
        //cout << "QSO::setLocalAward: " << taward << endl;
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

QString Qso::getMyIotaContinent() const{
  return my_iota.section('-',0,0);
}

int Qso::getMyIotaNumber() const {
  return (my_iota.section('-',1,1)).toInt();
}

void Qso::setName(const QString &tname){
  name = checkisNotAdif(tname);
}

QString Qso::getName() const{
  return name;
}

void Qso::setMyName(const QString &tname){
  my_name = checkisNotAdif(tname);
}

QString Qso::getMyName() const{
  return my_name;
}

void Qso::setAddress (const QString& taddress) {
        address = checkisNotAdif(taddress);
}

QString Qso::getAddress() const{
        return address;
}

void Qso::setMyPostalCode (const QString& taddress) {
        my_postal_code = checkisNotAdif(taddress);
}

QString Qso::getMyPostalCode() const{
        return my_postal_code;
}

void Qso::setMyStreet (const QString& taddress) {
        my_street = checkisNotAdif(taddress);
}

QString Qso::getMyStreet() const{
        return my_street;
}

void Qso::setMyState (const QString& taddress) {
        my_state = checkisNotAdif(taddress);
}

QString Qso::getMyState() const{
        return my_state;
}

void Qso::setARRLSect (const QString& tarrls){
//TODO: Check if it is a valid ARRL Section from the ADIF specs
        arrl_sect = checkisNotAdif(tarrls);
}

QString Qso::getARRLSect() const{
        return arrl_sect;
}

void Qso::setMyRig (const QString& taux){
        my_rig = checkisNotAdif(taux);
}

QString Qso::getMyRig() const{
        return my_rig;
}

void Qso::setMySig (const QString& taux){
         my_sig = checkisNotAdif(taux);
}

QString Qso::getMySig() const{
        return my_sig;
}

void Qso::setMySigInfo (const QString& taux){
         my_sig_info = checkisNotAdif(taux);
}

QString Qso::getMySigInfo() const{
        return my_sig_info;
}

void Qso::setSigInfo (const QString& taux){
         sig_info = checkisNotAdif(taux);
}

QString Qso::getSigInfo() const{
        return sig_info;
}

void Qso::setSig (const QString& taux){
         sig = checkisNotAdif(taux);
}

QString Qso::getSig() const{
        return sig;
}

void Qso::setPublicKey (const QString& taux){
         public_key = checkisNotAdif(taux);
}

QString Qso::getPublicKey() const{
        return public_key;
}

void Qso::setQsoRandom  (bool taux){
          qso_random = taux;
}

bool Qso::getQsoRandom() const{
        return qso_random;
}

void Qso::setSwl  (bool taux){
         swl = taux;
}

bool Qso::getSwl() const{
        return swl;
}

void Qso::setTenten  (const int taux){
          ten_ten = taux;
}

int Qso::getTenten() const{
        return ten_ten;
}

void Qso::setContinent(const QString &tcont){
        if ( (tcont == "NA") || (tcont == "SA") || (tcont == "EU") || (tcont == "AF") || (tcont == "OC") || (tcont == "") || (tcont == "AS") || (tcont == "AN")  ){
                cont = tcont;
        }else{
                cont="N";
        }
}

QString Qso::getContinent() const{
  return cont;
}

void Qso::setContactedOP(const QString &tqrz){
        contacted_op = checkisNotAdif(tqrz);
}

QString Qso::getContactedOP() const{return contacted_op;}

void Qso::setEQCall(const QString &tqrz){
        eq_call = checkisNotAdif(tqrz);
}

QString Qso::getEQCall() const{return eq_call;}

void Qso::setEmail(const QString &temail){
        email = checkisNotAdif(temail);
}

QString Qso::getEmail() const{return email;}

void Qso::setWeb(const QString &tweb){
        web = checkisNotAdif(tweb);
}

QString Qso::getWeb() const{return web;}


void Qso::setQth(const QString &tqth){
  qth = checkisNotAdif(tqth);
}

QString Qso::getQth() const{
  return qth;
}

void Qso::setMyCity(const QString &tqth){
  my_city = checkisNotAdif(tqth);
}

QString Qso::getMyCity() const{
  return my_city;
}

void Qso::setOperator(const QString &toperator){
    operatorc = checkisNotAdif(toperator);
}

QString Qso::getOperator() const{
  return operatorc;
}

void Qso::setStationCallsign(const QString &toperator){ // Call used to work this QSO
        station_callsign = toperator;
}
QString Qso::getStationCallsign() const{                // Call used to work this QSO
        return station_callsign;
}

void Qso::setPropMode(const QString &tprop_mode){
// TODO: Check if the propmode is correct before saving (ADIF has a table of valid data)
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
void Qso::setContestID(const QString &tcontestid){contest_id = checkisNotAdif(tcontestid);};
QString Qso::getContestID() const{return contest_id;};
void Qso::setCheck(const QString &tcheck){check = checkisNotAdif(tcheck);}; // Contest check
QString Qso::getCheck() const{return check;};                // Contest check
void Qso::setClass(const QString &tcclass){cclass = checkisNotAdif(tcclass);}; // Contest class
QString Qso::getClass() const{return cclass;};                // Contest class

void Qso::clearQso(){
        //cout << "Qso::clearQso" << endl;
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
        qslRecc = 'N';      // QSL_RCVD Y, N, R, I. V
        qslSenc = 'N';      // QSL_SENT Y, N, R, Q, I
        qslVia = "";     // QSL_VIA (C) //Manager
        qslInfo = ""; //;    // ADDRESS /QSLMSG
        comment = "";    // COMMENT
        address = "";
        //  int qslHow;         // How is the qsl to be send
        manager = "";
        gridSquare = "";
        myGridSquare = "";
        iota = "AF-0";   // GRIDSQUARE
        my_iota = "AF-0";   // GRIDSQUARE
        operatorc = "";   // OPERATOR
        name = "";       // NAME
        qth = "";        // QTH
        my_city="";
        localAward = "0"; // LocalAward
        localAwardNumber = 0; // Just for managing, not saved in the log
        dxcc=0;
        country="";
        my_country="";
        freq = "0.0"; // Frecuency
        freq_rx = freq;
        rx_power = "0"; //RX_PWR
        power = "0"; //TX_PWR
        age = -1;
        a_index=-1;
        k_index=-1;
        sfi=-1;
        contest_id="";
        cclass="";
        check="";
        ant_az=-1;
        ant_el=-1;	//Antena elevation
        ant_path='N'; // NULL
        band_rx="";
        cont='N';
        contacted_op ="";
        maxbursts=-1;
        nr_bursts=-1;
        nr_pings = -1;
        cqz = -1;
        ituz = -1;
        my_cqz = -1;
        my_ituz = -1;
        distance = -1;
        email ="";
        eq_call="";  // The contacted station's owner's callsign
        stx=-1;		// Contest stuff;
        srx=-1;		// Contest stuff;
        srx_string="";	// Contest stuff;
        stx_string="";	// Contest stuff;
        contest_id="";
        station_callsign="";
        arrl_sect="";
        msshower="";
        my_city="";
	dateTime = QDateTime::fromString("00000000000000", "yyyyMMddHHmmss");
}

QString Qso::checkisNotAdif(const QString& tword){
//cout << " - Class Qso::checkAdif" << tword << endl;
        sAux = tword;
        if (sAux.count('<')>0){
                sAux.replace( QChar('<'), QChar('('));
        }
        if (sAux.count('>')>0){
                sAux.replace( QChar('>'), QChar(')'));
        }
        return checkLines(sAux);
}

QString Qso::checkLines(const QString& tword){
//cout << " - Class Qso::checkLines" << tword << endl;
        sAux = tword;
        if (sAux.count('\n')>0){
                sAux.replace( QChar('\n'), "\\n");
        }
        return sAux;


}

void Qso::setAnt_Path(const QChar& tantpath){ // ANT_PATH  = G, O, S, L
        if ((tantpath == 'G') || (tantpath == 'O') || (tantpath == 'S') || (tantpath == 'L') ){
                ant_path = tantpath;
        }else{
                ant_path = 'N';
        }
}

QChar Qso::getAnt_Path(){return ant_path;}
