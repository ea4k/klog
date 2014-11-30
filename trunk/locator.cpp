/***************************************************************************
                          locator.cpp  -  description
                             -------------------
    begin                : vie feb 7 2003
    copyright            : (C) 2003 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "locator.h"
#include <QtDebug>

Locator::Locator(){
  ideg = 0;
  imin = 0;
  isec = 0;
}

Locator::~Locator(){
}

bool Locator::isValidLocator(const QString& tlocator){
/* -------------- Subroutine -----------------------
      Check valid locator (VALID: AA00AA -> RR99XX
      Input : char *locator = 4 or 6 characters word wide locator.
      returned value ==  -1 No error. (Valid locator).
      returned value ==  0 Error.   (Invalid locator).
      Note: also string "END" is considered a valid locator, but returned value is -2.
   ------------------------------------------------- */
 //qDebug() << "Locator::isValidLocator: " << tlocator << endl;

	int lenght_of_locator;
	testLocator ="A";
	testLocator = tlocator.toUpper();
	lenght_of_locator = testLocator.length();

	if (lenght_of_locator == 4){
		testLocator = testLocator +"LM";
		lenght_of_locator = 6;
	}

	if (lenght_of_locator != 6) { 

		return false;
	}else{
		theChar = testLocator.at(0);
		if (!theChar.isLetter()){  //First letter is not a valid letter

			return false;
		}
		if ((theChar<'A') && (theChar>'R') ){  //First letter is not a valid letter

			return false;
		}
		theChar = testLocator.at(1);
		if (!theChar.isLetter()){  //Second letter is not a valid letter

			return false;
		}
		if ((theChar<'A') && (theChar>'R') ){  //Second letter is not a valid letter

			return false;
		}
		theChar = testLocator.at(2);
		if (!theChar.isDigit()){  //Second letter is not a number

			return false;
		}
		theChar = testLocator.at(3);
		if (!theChar.isDigit()){  //Second letter is not a number

			return false;
		}
		theChar = testLocator.at(4);
		if (!theChar.isLetter()){  //First letter is not a valid letter

			return false;
		}
		if ((theChar<'A') && (theChar>'X') ){  //First letter is not a valid letter

			return false;
		}
		theChar = testLocator.at(5);
		if (!theChar.isLetter()){  //Second letter is not a valid letter

			return false;
		}
		if ((theChar<'A') && (theChar>'X') ){  //Second letter is not a valid letter

			return false;
		}
	}

	return true;
}


double Locator::getLat(const QString& tlocator){
//qDebug() << "Locator::getLat: " << tlocator;

    if (!isValidLocator(tlocator))
    {
        return 0.0;
    }

    if (tlocator.length() == 2)
    {
        return (((tlocator.at(1)).toLatin1() - 65) * 10) - 90;
    }
    if (tlocator.length() == 4)
    {
        return (((tlocator.at(1)).toLatin1() - 65) * 10) + ((tlocator.at(3)).toLatin1() - 48) - 90;
    }
    else if (tlocator.length()== 6)
    {
        return (((tlocator.at(1)).toLatin1() - 65) * 10) + ((tlocator.at(3)).toLatin1() - 48) + (((tlocator.at(5)).toLatin1() - 65 + 0.5) / 24) - 90;
    }
    else
    {
        return 0.0;
    }


}

double Locator::getLon(const QString& tlocator)
{
 //qDebug() << "Locator::getLon: " << tlocator;

    if (!isValidLocator(tlocator))
    {
        return 0.0;
    }

    if  (tlocator.length() == 2)
    {
        return (((tlocator.at(0)).toLatin1() - 65) * 20) - 180;
    }

    if (tlocator.length() == 4)
    {
        return (((tlocator.at(0)).toLatin1() - 65) * 20) + (((tlocator.at(2)).toLatin1() - 48) * 2)  - 180;
    }
    else if (tlocator.length()== 6)
    {
        return (((tlocator.at(0)).toLatin1() - 65) * 20) + (((tlocator.at(2)).toLatin1() - 48) * 2) + (((tlocator.at(4)).toLatin1() - 65 + 0.5) / 12) - 180;
    }
    else
    {
        return 0.0;
    }
}

int Locator::getBeam(const double lon1, const double lat1, const double lon2, const double lat2){
  double lon_a,lat_a,lon_b,lat_b, bearing;
  //qDebug() << "Locator::getBeam1: " << QString::number(lon1) << "/" << QString::number(lat1) << endl;
  //qDebug() << "Locator::getBeam2: " << QString::number(lon2) << "/" << QString::number(lat2) << endl;


  lon_a=lon1*PI/180;   // Convert degrees to radians
  lat_a=lat1*PI/180;
  lon_b=lon2*PI/180;
  lat_b=lat2*PI/180;

//earing_Distance( double lon_a, double lat_a, /* Lon/Lat of point A */
//                  double lon_b, double lat_b, /* Lon/Lat of point B */
//                  double *bearing, double *distance )/* From A to B */
//{
  double
    gc_arc, cos_gc_arc,       /* Great circle arc   A to B */
    cos_bearing, sin_bearing, /* cos/sin of bearing A to B */
    lon_diff;                 /* Difference in longitude of B from A */

  /* Longitude differnce of B from A */
  lon_diff = lon_b - lon_a;

  /* Calculate great circle distance A to B */
  cos_gc_arc = cos(lon_diff)*cos(lat_a)*cos(lat_b) + sin(lat_a)*sin(lat_b);
  gc_arc = acos( cos_gc_arc );

  /* Distance in km */
//  *distance = eradius * gc_arc;

  /* Calculate bearing A to B */
  cos_bearing  = sin(lat_b) - sin(lat_a) * cos_gc_arc;
  sin_bearing  = sin(lon_diff) * cos(lat_a) * cos(lat_b);
  bearing = atan2(sin_bearing, cos_bearing);

  /* Correct negative (anticlockwise) bearings */

  if( bearing < 0.0 )
  {
    bearing = (2*PI) + bearing;
  }
  bearing = 360 - (180/PI*bearing);
  bearing = 360 - bearing;

  //qDebug() << "Locator::getBeam: " << QString::number(bearing) << endl;
  /* Convert to degrees */

  return (int)bearing;

}

int Locator::getDistance(const double lon1, const double lat1, const double lon2, const double lat2, const bool _imperialSystem){
  //http://en.wikipedia.org/wiki/Haversine_formula
// qDebug() << "Locator::getDistanceKilometres: MyPos("<< QString::number(lon1) << "/"
// << QString::number(lat1)  << ") - DxPos(" << QString::number(lon2) << "/" << QString::number(lat2) << ")" << endl;
  double lo1,la1,lo2,la2;

// TODO: Is it needed to check if the longitude and latitude are correct and/or between the magins?  
//   if (!( (checkCoords(lon1, lat1) ) && (checkCoords(lon2, lat2)) ))
//     return 0;
  
  lo1=lon1* DEG_TO_RAD;   // Convert degrees to radians
  la1=lat1* DEG_TO_RAD;
  lo2=lon2* DEG_TO_RAD;  
  la2=lat2* DEG_TO_RAD;

  if (!_imperialSystem){
  //qDebug() << "Locator::getDistance (Km): " << QString::number((int)(acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS)) << endl;
    return (int)(acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS);
  }else{ // In milles
    //qDebug() << "Locator::getDistance (Milles): " << QString::number(((int)(acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS))* 0.62137) << endl;
    return ((int)(acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS)) * 0.62137;
  }
}


bool Locator::checkCoords(const double lon1, const double lat1){
//qDebug() << "Locator::checkCoords" ;
// Checks if a coordinates is correct.
  if ((lat1 > 90.0 || lat1 < -90.0) && (lon1 > 180.0 || lon1 < -180.0)){
      return true;
  }else{
    return false;
  }
}

QString Locator::getLocator(const double lon1, const double lat1) const{
/* -------------- Subroutine -----------------------
   Calculate locator from longitude and latitude
   Input : lon = Longitude in decimal degrees (+ = West;  - = East).
           lat = Latitude in decimal degrees (+ = North; - = South).
   Output: locator = 6 characters world wide locator.
   ------------------------------------------------- */
//qDebug() << "Locator::getLocator: (" << QString::number(lon1) << "/" << QString::number(lat1) << ")" << endl;
  QString locat = ""; //NO locator

  double lo, la;
  int alo,bla,clo,dla,elo,fla;

  lo=(-lon1+180)/20;
  la = (lat1+90)/10;

  alo=(int)floor(lo);
  bla=(int)floor(la);
  lo=(lo-(double)alo)*10;
  la=(la-(double)bla)*10;

  clo = (int)floor(lo);
  dla = (int)floor(la);

  elo = (int)floor((lo-(double)clo)*24);
  fla = (int)floor((la-(double)dla)*24);

//TODO: Test if locators are calculated correctly.
// generation function has been changed because of the QT4 migration
  locat = locat + QChar(alo+'A');
  locat = locat + QChar(bla+'A');
  locat = locat + QChar(clo+'0');
  locat = locat + QChar(dla+'0');
  locat = locat + QChar(elo+'A');
  locat = locat + QChar(fla+'A');

//   locat.at(0)=QChar(alo+'A');
  
//   locat.at(1)=QChar(bla+'A');
//   locat.at(2)=QChar(clo+'0');
//   locat.at(3)=QChar(dla+'0');
//   locat.at(4)=QChar(elo+'A');
//   locat.at(5)=QChar(fla+'A');
  


return locat;
}

void Locator::degTodms(const double deg){
  double temp;
  double ddeg;
  ddeg = 0;
  ddeg += 1.0/7200.0; /* Round-up to 0.5 sec */
  ideg = (int)ddeg;
  temp = ( deg - (double)ideg ) * 60.0;
  imin = (int)temp;
  temp = ( temp - (double)imin ) * 60.0;
  isec = (int)(temp); 
}

double Locator::dmsTodeg (int deg, int min, int sec){
    return (double)deg + (double)min/60.0 + (double)sec/3600.0;
}


int Locator::getBeamBetweenLocators (const QString& tlocator1, const QString& tlocator2)
{    
    //qDebug() << "Locator::getBeamBetweenLocators: " << tlocator1 << "/" << tlocator2 << endl;
    if (  !(isValidLocator(tlocator1) && isValidLocator(tlocator2) )  )
    {
        return -1;
    }
    else
    {
        double lon1 = getLon(tlocator1);
        double lon2 = getLon(tlocator2);
        double lat1 = getLat(tlocator1);
        double lat2 = getLat(tlocator2);

        return getBeam(lon1, lat1, lon2, lat2);

    }

}

int Locator::getDistanceBetweenLocators (const QString& tlocator1, const QString& tlocator2, const bool _imperialSystem)
{
    if (  !(isValidLocator(tlocator1) && isValidLocator(tlocator2) )  )
    {
        return -1;
    }
    else
    {
        double lon1 = getLon(tlocator1);
        double lon2 = getLon(tlocator2);
        double lat1 = getLat(tlocator1);
        double lat2 = getLat(tlocator2);

        return getDistance(lon1, lat1, lon2, lat2, _imperialSystem);
        //return getBeam(lon1, lat1, lon2, lat2);

    }
}
