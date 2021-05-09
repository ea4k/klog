/***************************************************************************
                          locator.h  -  description
                             -------------------
    begin                : vie feb 7 2003
    copyright            : (C) 2003 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#ifndef LOCATOR_H
#define LOCATOR_H
#include <math.h>
#include <QString>
#include <QRegularExpression>



const double PI = 3.141592654; //http://en.wikipedia.org/wiki/Pi
const bool LATITUDE = false;
const bool LONGITUDE = true;
const double EARTH_RADIUS = 6371; //http://en.wikipedia.org/wiki/Earth_radius
const double RADIAN = 180.0/PI;
const double DEG_TO_RAD = PI/180.0;
const double KM_IN_A_MILE = 1.609344;



class Locator{

public:

  Locator();
  ~Locator();

  bool isValidLocator(const QString& tlocator);
  double getLat(const QString& tlocator);
  double getLon(const QString& tlocator);
  QString getLocator(const double lon1, const double lat1) const;
  int getBeam(const double lon1, const double lat1, const double lon2, const double lat2);
  int getBeamBetweenLocators (const QString& tlocator1, const QString& tlocator2);
  int getDistance(const double lon1, const double lat1, const double lon2, const double lat2, const bool _imperialSystem);
  int getDistanceBetweenLocators (const QString& tlocator1, const QString& tlocator2, const bool _imperialSystem);
  //int getDistanceMilles(const double lon1, const double lat1, const double lon2, const double lat2);
  //void degTodms(const double deg);
  //double dmsTodeg (int deg, int min, int sec);

  bool checkCoords(const double lon1, const double lat1);
  QList<double> getTopLeftCoordinate(const QString& tlocator);  // Returns the top left coordinate of a Maidenhead locator

private:
  //bool valid;
  //QString myLocator;
  //QString testLocator;
  //QString otherLocator;
  //QChar theChar;
  //double my_lon, my_lat, other_lon, other_lat, beam, testNumb;
  //int ideg, imin, isec;


};

#endif
