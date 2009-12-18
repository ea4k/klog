/***************************************************************************
                          DxSpot.h  -  description
                             -------------------
    begin                : lun ene  7 18:42:45 CET 2007
    copyright            : (C) 2007 by Jaime Robles
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


#ifndef DxSpot_H
#define DxSpot_H
#include <QString>

#include <QDateTime>
#include <QDate>

class DxSpot{

public:
   DxSpot();
   ~DxSpot();
   DxSpot(QString sCall,  QString lCall, QString sCountry, QString lCountry,
           qreal spotLat, qreal spotLong, qreal logLat, qreal logLong, qreal freq) :
           spotCallsign(sCall), loggingCallsign(lCall), spotCountry(sCountry), loggingCountry(lCountry),
           spotLatitude(spotLat), spotLongitude(spotLong), loggingLatitude(logLat), loggingLongitude(logLong), frequency(freq) {};

   QString SpotCall() const { return spotCallsign; }
   QString LogCall() const { return loggingCallsign; }
   QString SpotCountry() const { return spotCountry; }
   QString LogCountry() const { return loggingCountry; }
   QString getDxCall() const{return dxcall;}
   qreal SLat() const { return spotLatitude; }
   qreal SLong() const { return spotLongitude; }
   qreal LLat() const { return loggingLatitude; }
   qreal LLong() const { return loggingLongitude; }
   qreal Frequency() const { return frequency; }
   void setDxCall(const QString &tdxcall);
   void setSpotter(const QString &tdxcall);
   QString getSpotter();
  void setNumb (int tnumb);
  void setDateTime(QDateTime tdateTime);
  int getNumb();
  QDateTime getDateTime() const{return dateTime;}

  void setDate(QDate tdate);
  QDate getDate() const{return dateTime.date();}

  void setTime(QTime ttime);
  QTime getTime() const{return dateTime.time();}
  void setFreq(double tfreq);
  double getFreq() const{return freq;}
  void clearDXSpot();
  void setcomment(const QString &tcomment);  
  QString getcomment() const{return comment;}

  
private:
   QString spotCallsign;
   QString loggingCallsign;
   QString spotCountry;
   QString loggingCountry;
   QString dxcall, dxspotter, comment;
   int numb;
   double freq;
   qreal spotLatitude;
   qreal spotLongitude;
   qreal loggingLatitude;
   qreal loggingLongitude;
   qreal frequency;
   QDateTime dateTime;

};
#endif
