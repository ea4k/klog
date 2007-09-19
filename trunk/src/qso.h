/***************************************************************************
                          qso.h  -  description
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

#ifndef QSO_H
#define QSO_H
#include <qstring.h>
//#include <qchar.h>
#include <qdatetime.h>

 #define CALL_LENGTH 15
// Modes: 12

// Bands: 9

class Qso{
public:
  Qso();
  Qso(const QString& tqrz, int tnumb, QString& tmode);
  Qso (const int tnumb, const QString& tqrz, const int trstrx, const int trsttx, int tband, const QString& tmode, const int tpower);
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

  void setFreq (const QString& tfreq);
  QString getFreq() const;

  void setMode(const QString& tmode);
  QString getMode() const;  

	void setPower(const QString& tpower);
	QString getPower() const;

	void setrxPower(const QString& tpower);
	QString getrxPower() const;

  void setQslSenDateOn(const QDate tdate);
    QDate getQslSenDate() const;

  void QslSent(const QChar& qslState);
    QChar isQslSent();

  void setQslRecDateOn(QDate tdate);
  QDate getQslRecDate() const;

  void QslRec(const QChar& qslState);
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

  void setLocalAward(const QString &taward);
  QString getLocalAward() const;


  void setLocalAwardNumber(int taward); // Those two functions are just for awards reference
  int getLocalAwardNumber() const;      // management

  void setName(const QString &tname);
  QString getName() const;  

  void setQth(const QString &tqth);
  QString getQth() const;

  void setOperator(const QString &toperator); // Call used to work this QSO
  QString getOperator() const;                // Call used to work this QSO

 void setPropMode(const QString &tprop_mode);
 QString getPropMode() const;

/********************* CONTESTS ********************/
	void setSrx (int tnumb);
	int getSrx() const;
	void setSrx_string(const QString &tsrx);
	QString getSrx_string() const;
	void setStx (int tnumb);
	int getStx() const;
	void setStx_string(const QString &tstx);
	QString getStx_string() const;
/********************* CONTESTS ********************/
  void clearQso();

private:
	QString	sAux;	//Auxiliar variable
  QString qrz;        // CALL
  int numb;           // STX
  QDateTime dateTime; // QSO_DATE/TIME_ON YYYYMMDD - Time HHMM (0000-2359)
  int rstrx;          // RST_RCVD
  int rsttx;          // RST_SENT
  QString freq;       // Frecuency
  QString band;       // BAND

  QString mode;       // MODE
  QString rx_power;   // RX_PWR
  QString power;      // TX_PWR
  QDate qslSen;       // QSLSDATE
  QDate qslRec;       // QSLRDATE
  QString comment;    // COMMENT
  QChar qslRecc;      // QSL_RCVD Y, N, R, I
  QChar qslSenc;      // QSL_SENT Y, N, R, I
  QString qslVia;     // QSL_VIA (C) //Manager
  QString qslInfo;    // ADDRESS /QSLMSG
  QString manager;
  int qslHow;         // How is the qsl to be send
  QString gridSquare; // GRIDSQUARE (the DX's one)
	QString myGridSquare; //(My grid)
  QString iota;       // IOTA
  QString localAward;	// Each Entity may have a local award reference (TPEA, WAS, ...)
  int localAwardNumber; // Just a help for award references management but not saved into the log
  QString callUsed;   // OPERATOR/Call used to work this QSO
  QString state;    // STATE
  QString name;       // NAME
  QString qth;        // QTH
  QString prop_mode;  // PROP_MODE QSO propagation mode
/***************** CONTEST FIELDS *************************/
	int srx;		//contest QSO received serial number
	int stx;		//contest QSO transmitted serial number
	QString srx_string; 	//contest QSO received information
	QString stx_string; 	//contest QSO transmitted information
/***************** CONTEST FIELDS *************************/
	
	// Checks that the data is not an ADIF field and replaces </> by a (/).
	QString checkisNotAdif(const QString& tword);
	QString checkLines(const QString& tword);


	//int age;	//Contacted operator's age
	//int ant_az;	//Antenna azimut in degrees
	//int ant_el;	//Antena elevation
	//QString band_rx; // In split opèrations the RX band
	//Contest_id;	// QSO Contest indentifier
	//cqz;		// CQ Zone 
	//ituz;		// ITU Zone
	//QString freq_rx;

};


#endif

