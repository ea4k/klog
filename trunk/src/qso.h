/***************************************************************************
                          qso.h  -  description
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

#ifndef QSO_H
#define QSO_H
#include <QString>
//#include <qchar.h>
#include <QDateTime>
#include <QDebug>

 #define CALL_LENGTH 15
// Modes: 12

// Bands: 9

class Qso{
public:
  Qso();
  Qso(const QString& tqrz, int tnumb, QString& tmode);
  Qso (const int tnumb, const QString& tqrz, const int trstrx, const int trsttx, const QString& tmode, const QString& tband, const int tpower);
  // Next constructor is used to read log
  Qso (const int tnumb, const QDateTime tdateTime, const QString& tqrz, const int trstrx, const int trsttx, const QString& tband, const QString& tmode, const QString& tpower, const QString& tcomment);
  Qso (const int tnumb, const QDateTime& tdateTime, const QString& tqrz, const int trstrx, const int trsttx, int tband, const QString& tmode, const int tpower, const QDate& tqslSen,  const QDate& tqslRec, const QString& tcomment);
  ~Qso();

  void setQrz(const QString &tqrz);
  QString getQrz() const;

//  void setRrst(int trst);
//  int getRrst() const;

  void setNumb (int tnumb);
  int getNumb() const;

  void setDateTime(QDateTime tdateTime);
  QDateTime getDateTime() const;

  void setDate(QDate tdate);
  QDate getDate() const;

  void setTime(QTime ttime);
  QTime getTime() const;

  void setRstrx(int trstrx);
  int getRstrx() const;

  void setRsttx(int trsttx);
  int getRsttx() const;

  void setBand (const QString& tband);
  QString getBand() const;

  void setBand_RX (const QString& tband);
  QString getBand_RX() const;


  void setFreq (const QString& tfreq); // In MHz (14, 14.195, ...)
  QString getFreq() const;

  void setFreq_RX (const QString& tfreq); // In MHz (14, 14.195, ...)
  QString getFreq_RX() const;

  void setMode(const QString& tmode);
  QString getMode() const;

    void setPower(const QString& tpower);
    QString getPower() const;

    void setrxPower(const QString& tpower);
    QString getrxPower() const;

    void setDXCC (int tnumb);	// ADIF DXCC Country code
    int getDXCC() const;		// ADIF DXCC Country code

    void setCountry(const QString& tcountry);	// ADIF DXCC Country name
    QString getCountry() const;			// ADIF DXCC Country name

    void setMyCountry(const QString& tcountry);	// ADIF DXCC Country name
    QString getMyCountry() const;			// ADIF DXCC Country name

    void setAge (int tnumb);	// Contacted operator age in years
    int getAge() const;		// Contacted operator age in years

    void setAnt_az (int tnumb);	//Antenna azimut in degrees
    int getAnt_az() const;		//Antenna azimut in degrees
    void setAnt_el (int tnumb);	//Antenna elevation in degrees
    int getAnt_el() const;		//Antenna elevation in degrees


    void setA_index (int tnumb); // geomagnetic A index at the time of the QSO
    int getA_index() const;	// geomagnetic A index at the time of the QSO
    void setK_index (int tnumb); // geomagnetic K index at the time of the QSO
    int getK_index() const;	// geomagnetic K index at the time of the QSO


   void setQslSenDateOn(QDate tdate);
   QDate getQslSenDate() const;

  void seteQslSenDateOn(QDate tdate);
  QDate geteQslSenDate() const;
  void seteQslRecDateOn(QDate tdate);
  QDate geteQslRecDate() const;

   void seteQslSent(const QChar& eqslState); // eQSL_Sent Y, N, R, Q, I
   QChar geteQslSent();
   void seteQslRcvd(const QChar& eqslState); // eQSL_RCVD Y, N, R, I, V
   QChar geteQslRcvd();

  void QslSent(const QChar& qslState);
  QChar isQslSent();

  void setQslRecDateOn(QDate tdate);
  QDate getQslRecDate() const;

  void QslRec(const QChar& qslrState);
  QChar isQslRec();

  bool gotTheQSL();  // Ask if i have his QSL
  bool sentTheQSL(); // Did i sent my QSL?

  void setComment(const QString &tcomment);
  QString getComment()const;

  void setQslVia(const QString &tInfo); // (bureau, direct, ...)
  QString getQslVia() const;

  void setQslManager(const QString &tqrz); // (Qrz of manager)
  QString getQslManager() const;

  void setQslInfo(const QString &tqslInfo);  // Address, info...
  QString getQslInfo()const;

  void setLocator(const QString &tqrz);
  QString getLocator() const;

  void setMyLocator(const QString &tqrz);
  QString getMyLocator() const;

  void setIota(const QString &tiota);
  QString getIota() const;
  QString getIotaContinent() const;
  int getIotaNumber() const;

  void setMyIota(const QString &tiota);
  QString getMyIota() const;
  QString getMyIotaContinent() const;
  int getMyIotaNumber() const;


  void setLocalAward(const QString &taward);
  QString getLocalAward() const;

  void setLocalAwardNumber(int taward); // Those two functions are just for awards reference
  int getLocalAwardNumber() const;      // management

  void setName(const QString &tname);
  QString getName() const;

  void setMyName(const QString &tname);
  QString getMyName() const;

  void setAddress (const QString& taddress);
  QString getAddress() const;

  void setMyPostalCode (const QString& taddress);
  QString getMyPostalCode() const;

  void setMyStreet (const QString& taddress);
  QString getMyStreet() const;

  void setMyState (const QString& taddress);
  QString getMyState() const;

  void setARRLSect (const QString& tarrls);
  QString getARRLSect() const;

  void setContinent(const QString &tcont);
  QString getContinent() const;

  void setContactedOP(const QString &tqrz);
  QString getContactedOP() const;

  void setEQCall(const QString &tqrz); // the  contacted station's owner's callsign
  QString getEQCall() const;

  void setEmail(const QString &temail);
  QString getEmail() const;

  void setWeb(const QString &tweb);
  QString getWeb() const;


  void setCQz (int tnumb);
  int getCQz() const;
  void setITUz (int tnumb);
  int getITUz() const;

  void setMyCQz (int tnumb);
  int getMyCQz() const;
  void setMyITUz (int tnumb);
  int getMyITUz() const;


  void setDistance (int tnumb);
  int getDistance() const;

  void setQth(const QString &tqth);
  QString getQth() const;

  void setMyCity(const QString &tqth);
  QString getMyCity() const;

  void setOperator(const QString &toperator); // Operator that works this QSO
  QString getOperator() const;                // Operator that works this QSO

  void setStationCallsign(const QString &toperator); // Call used to work this QSO
  QString getStationCallsign() const;                // Call used to work this QSO

 void setPropMode(const QString &tprop_mode);
 QString getPropMode() const;

 void setSatMode(const QString &tsat);
 QString getSatMode() const;
 void setSatName(const QString &tsat);
 QString getSatName() const;

 void setMaxBursts (int tnumb);	//maximum length of meteor scatter bursts heard by the logging station, in seconds
 int getMaxBursts() const;

 void setNRBursts (int tnumb);
 int getNRBursts() const;

 void setNRPings (int tnumb);
 int getNRPings() const;

 void setSFI (int tnumb);
 int getSFI() const;

 void setMSShower(const QString &tmsshower);	//For Meteor Scatter QSOs, the name of the meteor shower in progress
 QString getMSShower() const;

 void setOwnerCall(const QString &tstring);
 QString getOwnerCall() const;
 void setPfx(const QString &tstring);
 QString getPfx() const;
 void setPrecedence(const QString &tstring);
 QString getPrecedence() const;

void setMyRig (const QString& taux);
QString getMyRig() const;
void setMySig (const QString& taux);
QString getMySig() const;
void setMySigInfo (const QString& taux);
QString getMySigInfo() const;
void setSigInfo (const QString& taux);
QString getSigInfo() const;
void setSig (const QString& taux);
QString getSig() const;
void setPublicKey (const QString& taux);
QString getPublicKey() const;

// WORK 3+
void setQsoRandom  (bool taux);
bool getQsoRandom() const;
void setSwl  (bool taux);
bool getSwl() const;
void setTenten  (const int taux);
int getTenten() const;
/////////////////////

/********************* CONTESTS ********************/
    void setSrx (int tnumb);
    int getSrx() const;
    void setSrx_string(const QString &tsrx);
    QString getSrx_string() const;
    void setStx (int tnumb);
    int getStx() const;
    void setStx_string(const QString &tstx);
    QString getStx_string() const;
    void setContestID(const QString &tcontestid); // Contest identifier
    QString getContestID() const;                // Contest identifier
    void setCheck(const QString &tcheck); // Contest check
    QString getCheck() const;                // Contest check
    void setClass(const QString &tcheck); // Contest Class
    QString getClass() const;                // Contest Class

/********************* CONTESTS ********************/

   void setAnt_Path(const QChar& tantpath); // ANT_PATH  = G, O, S, L
   QChar getAnt_Path();


  void clear();

private:
    QString	sAux;	//Auxiliar variable
  QString qrz;        // CALL
  int numb;           //
  QDateTime dateTime; // QSO_DATE/TIME_ON YYYYMMDD - Time HHMM (0000-2359)
  int rstrx;          // RST_RCVD
  int rsttx;          // RST_SENT
  QString freq;       // Frecuency
  QString freq_rx;       // Frecuency
  QString band;       // BAND
  QString band_rx;       // BAND_RX

    int dxcc;	// ADIF DXCC country code
  QString country;	// ADIF DXCC country Name
  QString mode;       // MODE
  QString rx_power;   // RX_PWR
  QString power;      // TX_PWR
  QDate qslSen;       // QSLSDATE
  QDate qslRec;       // QSLRDATE
  QDate eqsl_qslrdate;	//EQSL_QSLRDATE
  QDate eqsl_qslsdate;	//EQSL_QSLSDATE
  QString comment;    // COMMENT
  QString address;    // ADDRESS
  QChar qslRecc;      // QSL_RCVD Y, N, R, I
  QChar qslSenc;      // QSL_SENT Y, N, R, I
  QChar eQSL_qsl_rcvd; // eQSL_RCVD Y, N, R, I, V
  QChar eQSL_qsl_sent; // eQSL_Sent Y, N, R, Q, I
  QChar ant_path;	// G, O, S, L, N(null)
  QString qslVia;     // QSL_VIA (C) //Manager
  QString qslInfo;    // ADDRESS /QSLMSG
  QString manager;
  int qslHow;         // How is the qsl to be send
  QString gridSquare; // GRIDSQUARE (the DX's one)
    QString myGridSquare; //(My grid)
  QString iota;       // IOTA
  QString localAward;	// Each Entity may have a local award reference (TPEA, WAS, ...)
  int localAwardNumber; // Just a help for award references management but not saved into the log
  QString station_callsign;   // Call used to work this QSO.
  QString operatorc;   // OPERATOR/Call(only if not station_callsign is present) used to work this QSO
  QString contacted_op; // the callsign of the individual operating the  contacted station
  QString eq_call;	//The  contacted station's owner's callsign
  QString check;	// contest check
  QString cclass;	// contest class
  QString state;    // STATE
  QString name;       // NAME
  QString cont;		// Contacted Continent
  QString qth;        // QTH
  QString email;	// Contacted station's email
  QString web;		// Contacted station's web
  QString arrl_sect;		// Contacted station's ARRL Section
  int age;		// Contacted operator age in years
  int a_index;		// geomagnetic A index at the time of the QSO
  int k_index;		// geomagnetic K index at the time of the QSO
  int ant_az;	//Antenna azimut in degrees
  int ant_el;	//Antena elevation
  int cqz;
  int ituz;
  int distance;
  int maxbursts;	// maximum length of meteor scatter bursts heard by the logging station, in seconds
  QString msshower;
  QString prop_mode;  // PROP_MODE QSO propagation mode
  int my_cqz;
  int my_ituz;
  QString my_iota;
  QString my_city;
  QString my_country;
  QString my_name;
  QString my_postal_code;
  QString my_street;
  QString my_state;
  int nr_bursts;
  int nr_pings;
  int sfi;		// the solar flux at the time of the QSO
  QString sat_mode;
  QString sat_name;
  QString owner_callsign;
  QString pfx;
  QString precedence;

  QString my_rig;
  QString my_sig;
  QString my_sig_info;
  QString sig;
  QString sig_info;
  QString public_key;

//TODO: Add support for the following ADIF data:
// Adding support means at least:
// 1- create the set and get functions in the QSO class
// 2- Initialize and/or clear it appropriately in qso.clear()
// 3- Save this data in the adifTempFileSave in Klog class
// 4- Read this data in the processLogLine in the Klog class


  int my_lat;
  int my_lon;
  int lat;
  int lon;

  bool qso_random;	// 1&2
  bool swl;		// 1&2
  int ten_ten;		// 1&2


  QString notes; // Multiline... but similar to comment?
  QString rig; // Multiline...
  QString qso_complete; //(Char)

///////////////////////////////////////////////////////

/***************** CONTEST FIELDS *************************/
    int srx;		//contest QSO received serial number
    int stx;		//contest QSO transmitted serial number
    QString srx_string; 	//contest QSO received information
    QString stx_string; 	//contest QSO transmitted information
    QString contest_id;	// Contest identifier
/***************** CONTEST FIELDS *************************/

    // Checks that the data is not an ADIF field and replaces </> by a (/).
    QString checkisNotAdif(const QString& tword);
    QString checkLines(const QString& tword);

    //QString band_rx; // In split oprations the RX band

    //cqz;		// CQ Zone
    //ituz;		// ITU Zone
    //QString freq_rx;

};


#endif

