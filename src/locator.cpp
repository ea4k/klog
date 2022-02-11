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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "locator.h"
#include <QtDebug>

Locator::Locator(){

}

Locator::~Locator(){
}

bool Locator::isValidLocator(const QString& tlocator){
/* -------------- Subroutine -----------------------
      Check valid locator (VALID: AA00AA -> RR99XX)
      Input : char *locator = 4 - 8 characters word wide locator.
      returned value ==  -1 No error. (Valid locator).
      returned value ==  0 Error.   (Invalid locator).
      Note: also string "END" is considered a valid locator, but returned value is -2.
   -------------------------------------------------
Wikipedia:
    Character pairs encode longitude first, and then latitude.
    The first pair (a field) encodes with base 18 and the letters "A" to "R".
    The second pair (square) encodes with base 10 and the digits "0" to "9".
    The third pair (subsquare) encodes with base 24 and the letters "a" to "x".
    The fourth pair (extended square) encodes with base 10 and the digits "0" to "9".
    The fifth and subsequent pairs are not formally defined, but recycling the third and fourth
      pair algorithms is one possible definition:  BL11bh16oo66

*/

    //qDebug() << "Locator::isValidLocator: " << tlocator << QT_ENDL;

    //int lenght_of_locator;

    QString testLocator ="A";
    testLocator = tlocator.toUpper();
    //lenght_of_locator = testLocator.length(); Locators up to 8 digits!
    //IN, IN70, IN70DD, IN70DD20, IN70DD20
    QRegularExpression rx;
    rx.setPattern("^[A-R]{2}$");
    if (rx.match(testLocator).hasMatch())
    {
        //qDebug() << "Locator::isValidLocator: Match 2: " << testLocator;
        return true;
    }
    else
    {
        rx.setPattern("^[A-R]{2}[0-9]{2}$");
        if (rx.match(testLocator).hasMatch())
        {
            //qDebug() << "Locator::isValidLocator: Match 4: " << testLocator;
            return true;
        }
        else
        {
            rx.setPattern("^[A-R]{2}[0-9]{2}[A-X]{2}$");
            if (rx.match(testLocator).hasMatch())
            {
                //qDebug() << "Locator::isValidLocator: Match 6: " << testLocator;
                return true;
            }
            else
            {
                rx.setPattern("^[A-R]{2}[0-9]{2}[A-X]{2}[0-9]{2}$");
                if (rx.match(testLocator).hasMatch())
                {
                    //qDebug() << "Locator::isValidLocator: Match 8: " << testLocator;
                    return true;
                }
                else
                {
                    //qDebug() << "Locator::isValidLocator: NOT VALID: " << testLocator;
                    return false;
                }
            }
        }
    }
}

Coordinate Locator::getLocatorCoordinate(const QString _tlocator)
{
    Coordinate _position;
    _position.lat = 0.0;
    _position.lon = 0.0;
    if (!isValidLocator(_tlocator))
    {
        //qDebug() << Q_FUNC_INFO << ": Not valid: " << tlocator;
        return _position;
    }
    _position.lat = getLat (_tlocator);
    _position.lon = getLon (_tlocator);
    return _position;

}

Coordinate Locator::getLocatorCorner (const QString& tlocator, bool northWest)
{
    //qDebug() << Q_FUNC_INFO << ": " << tlocator;
    Coordinate _position, _north, _south;
    _position.lat = 0.0;
    _position.lon = 0.0;

    Coordinate _positionC = _position;
    _north = _position;
    _south = _position;

    if (!isValidLocator(tlocator))
    {
        //qDebug() << Q_FUNC_INFO << ": Not valid: " << tlocator;
        return _position;
    }


    _positionC.lat = getLat (tlocator);
    _positionC.lon = getLon (tlocator);

    if (tlocator.length() == 2)
    {
        _north.lat = _positionC.lat + 5;
        _north.lon = _positionC.lon - 10;

        _south.lat = _positionC.lat - 5;
        _south.lon = _positionC.lon + 10;
    }
    else if (tlocator.length() == 4)
    {
        _north.lat = _positionC.lat + 0.5;
        _north.lon = _positionC.lon -1;

        _south.lat = _positionC.lat - 0.5;
        _south.lon = _positionC.lon + 1;
    }
    else if (tlocator.length() == 6)
    {
        _north.lat = _positionC.lat ;//+ 0.020833333333333; //(2.5/60/2);
        _north.lon = _positionC.lon - 0.0416666666666667;//(5/60/2);

        _south.lat = _positionC.lat - (2*0.020833333333333); //(2.5/60/2);
        _south.lon = _positionC.lon + 0.0416666666666667;//(5/60/2);
    }
    else if (tlocator.length() == 8)
    {

    }

    //qDebug() << Q_FUNC_INFO;
    //qDebug() << "Center: lat/lon" << QString::number(_positionC.lat) << "/" << QString::number(_positionC.lon);
    //qDebug() << QString ("North : %1, %2").arg(_north.lat).arg(_north.lon);
    //qDebug() << QString ("South : %1, %2").arg(_south.lat).arg(_south.lon);
    if (northWest)
    {
        return _north;
        //qDebug() << QString ("North : %1, %2").arg(_position.lat).arg(_position.lon);
    }
    else
    {
        return _south;
        //qDebug() << QString ("South : %1, %2").arg(_position.lat).arg(_position.lon);
    }
}

double Locator::getLat(const QString& tlocator){
    //qDebug() << "Locator::getLat: " << tlocator;
    // Read formula from: https://unclassified.software/files/source/MaidenheadLocator.cs
    //Revisar las formulas porque salen distancias erroneas
    if (!isValidLocator(tlocator))
    {
        return 0.0;
    }
    QString aux = tlocator.toUpper();

    if (aux.length() == 2)
    {
        return (((aux.at(1)).toLatin1() - 'A') * 10) - 85;
    }
    if (aux.length() == 4)
    {
        //return (((aux.at(1)).toLatin1() - 'A') * 10) + ((aux.at(3)).toLatin1() - '0' * 1) - 85;
        return (((aux.at(1)).toLatin1() - 'A') * 10) + ((aux.at(3)).digitValue() + 0.5) - 90;
    }
    if (aux.length() == 6)
    {
        double result = (((aux.at(1)).toLatin1() - 'A') * 10 ) + (aux.at(3).digitValue()) + (((aux.at(5)).toLatin1() - 'A') * (2.5/60)) + (static_cast<double>(2.5/60)) - 90;

        //qDebug() << QString("%1: Locator/Latitude: %2/%3").arg(Q_FUNC_INFO).arg(aux).arg(result);
        return result;
    }
    if (aux.length()== 8)
    {
        return ((aux.at(1)).toLatin1() - 'A') * 10 + ((aux.at(3)).digitValue() * 1) + (((aux.at(5)).toLatin1() - 'A') * (2.5/60))  + ((aux.at(7)).toLatin1() - '0' * 0.5) / 240 - 90;
    }
    if (aux.length()== 10)
    {
        return ((aux.at(1)).toLatin1() - 'A') * 10 + ((aux.at(3)).toLatin1() - '0') + ((aux.at(5)).toLatin1() - 'A' + 0.0) / 24 + ((aux.at(7)).toLatin1() - '0' + 0.0) / 240 + static_cast<double>((aux.at(9)).toLatin1() - 'A' + 0.5) / 240 / 24 - 90;
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
    QString aux = tlocator.toUpper ();

    if  (aux.length() == 2)
    {
        return (((aux.at(0)).toLatin1() - 'A') * 20) + 10 - 180;
    }

    if (aux.length() == 4)
    {
            //qDebug() << QString("%1 + %2 + %3 - 180 = %4")
            //            .arg((((aux.at(0)).toLatin1()  - 'A') * 20))
            //            .arg((((aux.at(2)).digitValue () ) * 2))
            //            .arg(1)
            //            .arg((((aux.at(0)).toLatin1()  - 'A') * 20) + (((aux.at(2)).digitValue () ) * 2)  +1 - 180);

       // return ((aux.at(0)).toLatin1()  - 'A') * 20 + 10 + (((aux.at(2)).digitValue () ) * 2)  + 0.0416666666666667 - 180;

        return (((aux.at(0)).toLatin1()  - 'A') * 20) +
                (((aux.at(2)).digitValue () ) * 2) + 1 - 180;
    }
    if (aux.length() == 6)
    {
        //qDebug() << QString("%1 + %2 + %3 + %4 - 180 = %5").arg((((aux.at(0)).toLatin1()  - 'A') * 20)).arg((((aux.at(2)).digitValue () ) * 2)).arg((((aux.at(4)).toLatin1() - 'A')/12 )).arg(0.0416666666666667).arg((((aux.at(0)).toLatin1()  - 'A') * 20) +  (((aux.at(2)).digitValue () ) * 2) + (static_cast<double>((aux.at(4)).toLatin1() - 'A')/12.0 ) + 0.0416666666666667 - 180);


    return (((aux.at(0)).toLatin1()  - 'A') * 20) +
            (((aux.at(2)).digitValue () ) * 2) +
            (static_cast<double>(((aux.at(4)).toLatin1()) - 'A')/12.0 ) +
            0.0416666666666667 - 180;
    }
    if (aux.length()== 8)
    {
        return ((aux.at(0)).toLatin1() - 'A') * 20 + ((aux.at(2)).digitValue()) * 2 + static_cast<double>((aux.at(4)).toLatin1() - 'A' + 0.0) / 12 + static_cast<double>((aux.at(6)).toLatin1() - '0' + 0.5) / 120 - 180;
    }
    //else if (aux.length()== 10)
    //{
    //        return ((aux.at(0)).toLatin1() - 'A') * 20 + ((aux.at(2)).toLatin1() - '0') * 2 + ((aux.at(4)).toLatin1() - 'A' + 0.0) / 12 + ((aux.at(6)).toLatin1() - '0' + 0.0) / 120 + ((aux.at(8)).toLatin1() - 'A' + 0.5) / 120 / 24 - 180;
    //    }
    else
    {
        return 0.0;
    }
}

int Locator::getBeam(const double lon1, const double lat1, const double lon2, const double lat2){
  double lon_a,lat_a,lon_b,lat_b, bearing;
     //qDebug() << "Locator::getBeam1: " << QString::number(lon1) << "/" << QString::number(lat1) << QT_ENDL;
     //qDebug() << "Locator::getBeam2: " << QString::number(lon2) << "/" << QString::number(lat2) << QT_ENDL;


  lon_a=lon1*PI/180;   // Convert degrees to radians
  lat_a=lat1*PI/180;
  lon_b=lon2*PI/180;
  lat_b=lat2*PI/180;

//earing_Distance( double lon_a, double lat_a, /* Lon/Lat of point A */
//                  double lon_b, double lat_b, /* Lon/Lat of point B */
//                  double *bearing, double *distance )/* From A to B */
//{
  double
    cos_gc_arc,       /* Great circle arc   A to B */
    cos_bearing, sin_bearing, /* cos/sin of bearing A to B */
    lon_diff;                 /* Difference in longitude of B from A */
    //double gc_arc;
  /* Longitude differnce of B from A */
  lon_diff = lon_b - lon_a;

  /* Calculate great circle distance A to B */
  cos_gc_arc = cos(lon_diff)*cos(lat_a)*cos(lat_b) + sin(lat_a)*sin(lat_b);
  //gc_arc = acos( cos_gc_arc );

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

     //qDebug() << "Locator::getBeam: " << QString::number(bearing) << QT_ENDL;
  /* Convert to degrees */

  return int(bearing);
}

int Locator::getDistance(const double lon1, const double lat1, const double lon2, const double lat2, const bool _imperialSystem){
  //http://en.wikipedia.org/wiki/Haversine_formula
       //qDebug() << "Locator::getDistanceKilometres: MyPos("<< QString::number(lon1) << ")" << QT_ENDL;
// << QString::number(lat1)  << ") - DxPos(" << QString::number(lon2) << "/" << QString::number(lat2) << ")" << QT_ENDL;
  double lo1,la1,lo2,la2;

// TODO: Is it needed to check if the longitude and latitude are correct and/or between the magins?
//   if (!( (checkCoords(lon1, lat1) ) && (checkCoords(lon2, lat2)) ))
//     return 0;

  lo1=lon1* DEG_TO_RAD;   // Convert degrees to radians
  la1=lat1* DEG_TO_RAD;
  lo2=lon2* DEG_TO_RAD;
  la2=lat2* DEG_TO_RAD;

  if (!_imperialSystem){
     //qDebug() << "Locator::getDistance (Km): " << QString::number((int)(acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS)) << QT_ENDL;
    return int(acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS);
  }else{ // In milles
       //qDebug() << "Locator::getDistance (Milles): " << QString::number(((int)(acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS))* 0.62137) << QT_ENDL;
    return int(((acos(cos(la1)*cos(lo1)*cos(la2)*cos(lo2)+cos(la1)*sin(lo1)*cos(la2)*sin(lo2)+sin(la1)*sin(la2)) * EARTH_RADIUS)) * 0.62137);
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

QString Locator::getLocator(const double lon1, const double lat1, int length) const{
/* -------------- Subroutine -----------------------
   Calculate locator from longitude and latitude
   Input : lon = Longitude in decimal degrees (+ = West;  - = East).
           lat = Latitude in decimal degrees (+ = North; - = South).
   Output: locator = 6 characters world wide locator.
   ------------------------------------------------- */
    //qDebug() << "Locator::getLocator: (" << QString::number(lon1) << "/" << QString::number(lat1) << ")" << QT_ENDL;
    QString locat = ""; //NO locator

    double lo, la;
    int alo,bla,clo,dla,elo,fla;

    lo = (lon1+180)/20;
    la = (lat1+90)/10;

    alo=int(floor(lo));
    bla=int(floor(la));

    locat = locat + QChar(alo+'A');
    locat = locat + QChar(bla+'A');
    //qDebug() << Q_FUNC_INFO << ": " << locat;
    if (length == 2)
    {
        return locat;
    }

    lo=(lo-(double(alo)))*10;
    la=(la-(double(bla)))*10;

    clo = int(floor(lo));
    dla = int(floor(la));
    locat = locat + QChar(clo+'0');
    locat = locat + QChar(dla+'0');

    //qDebug() << Q_FUNC_INFO << ": " << locat;
    if (length == 4)
    {
        return locat;
    }

    elo = int(floor((lo-double(clo) ) * 24 )) ;
    fla = int(floor((la-double(dla) ) * 24 ));

    locat = locat + QChar(elo+'A');
    locat = locat + QChar(fla+'A');
    //qDebug() << Q_FUNC_INFO << ": " << locat;
  //locat = locat + QChar(elo+'A');
  //locat = locat + QChar(fla+'A');
    //qDebug() << Q_FUNC_INFO << ": " << locat;
    return locat;
}

int Locator::getBeamBetweenLocators (const QString& tlocator1, const QString& tlocator2)
{
       //qDebug() << "Locator::getBeamBetweenLocators: " << tlocator1 << "/" << tlocator2 << QT_ENDL;
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
    }
}

QStringList Locator::getAll(int _length)
{
    qDebug() << Q_FUNC_INFO << QString(" - %1").arg(_length);

    QStringList list;
    list.clear();
    QString locator;
    QChar letter1, letter2, letter3, letter4, num1, num2;
    for (int i = 0; i<18; i++)
    {
        letter1 = 'A' + i;
        for (int j = 0; j<18; j++)
        {
            letter2 = 'A' + j;
            if (_length<=2)
            {
                locator.append(letter1);
                locator.append(letter2);
                list.append(locator);
                //qDebug() << Q_FUNC_INFO << locator ;
                locator.clear();
            }
            else
            {
                for (int k = 0; k<10; k++)
                {
                    num1 = '0' + k;
                    for (int l = 0; l<10; l++)
                    {
                        num2 = '0' + l;
                        if (_length<=4)
                        {
                            locator.append(letter1);
                            locator.append(letter2);
                            locator.append(num1);
                            locator.append(num2);
                            list.append(locator);
                            //qDebug() << Q_FUNC_INFO << locator ;
                            locator.clear();
                        }
                        else
                        {
                            for (int m = 0; m<24; m++)
                            {
                                letter3 = 'a' + m;
                                for (int n = 0; n<24; n++)
                                {
                                    letter4 = 'a' + n;
                                    if (_length<=6)
                                    {
                                        locator.append(letter1);
                                        locator.append(letter2);
                                        locator.append(num1);
                                        locator.append(num2);
                                        locator.append(letter3);
                                        locator.append(letter4);
                                        list.append(locator);
                                        //qDebug() << Q_FUNC_INFO << locator ;
                                        locator.clear();
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }
    }
    return list;
}
