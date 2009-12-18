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

#include <QString>

class DxSpot{

public:
   DxSpot() {};
   ~DxSpot() {};
   DxSpot(QString sCall,  QString lCall, QString sCountry, QString lCountry,
           qreal spotLat, qreal spotLong, qreal logLat, qreal logLong, qreal freq) :
           spotCallsign(sCall), loggingCallsign(lCall), spotCountry(sCountry), loggingCountry(lCountry),
           spotLatitude(spotLat), spotLongitude(spotLong), loggingLatitude(logLat), loggingLongitude(logLong), frequency(freq) {};

   QString SpotCall() const { return spotCallsign; }
   QString LogCall() const { return loggingCallsign; }
   QString SpotCountry() const { return spotCountry; }
   QString LogCountry() const { return loggingCountry; }
   qreal SLat() const { return spotLatitude; }
   qreal SLong() const { return spotLongitude; }
   qreal LLat() const { return loggingLatitude; }
   qreal LLong() const { return loggingLongitude; }
   qreal Frequency() const { return frequency; }

private:
   QString spotCallsign;
   QString loggingCallsign;
   QString spotCountry;
   QString loggingCountry;
   qreal spotLatitude;
   qreal spotLongitude;
   qreal loggingLatitude;
   qreal loggingLongitude;
   qreal frequency;

};
#endif
