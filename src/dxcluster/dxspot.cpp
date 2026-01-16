/***************************************************************************
                          dxspot.cpp  -  description
                             -------------------
    begin                : apr 2024
    copyright            : (C) 2024 by Jaime Robles
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
#include "dxspot.h"
#include "../callsign.h"

DXSpot::DXSpot()
{
    clear();
}


DXSpot::~DXSpot(){}

DXSpot::DXSpot(const DXSpot& other)
{
    dxcall      = other.dxcall;
    freq        = other.freq;
    spotter     = other.spotter;
    comment     = other.comment;
    dateTime    = other.dateTime;
    clickStatus = other.clickStatus;
    shdx        = other.shdx;
}

void DXSpot::clear()
{
    dxcall.clear();
    freq.clear();
    spotter.clear();
    dateTime.currentDateTimeUtc();
    clickStatus = NoClick;
    shdx        = false;
}

void DXSpot::operator=(DXSpot const &_other)
{
    dxcall          = _other.dxcall;
    freq            = _other.freq; // Might need a copy constructor for Frequency as well
    spotter         = _other.spotter;
    comment         = _other.comment;
    if (_other.dateTime.isValid())
        dateTime    = _other.dateTime;
    clickStatus     = _other.clickStatus;
    shdx            = _other.shdx;

}

bool DXSpot::isValid()
{
   // qDebug() << Q_FUNC_INFO << " - 010";
    Callsign dxc(dxcall);
    if (!dxc.isValid())
        return false;

   // qDebug() << Q_FUNC_INFO << " - 020";
    Callsign spo(spotter);
    if (!spo.isValid())
        return false;

   // qDebug() << Q_FUNC_INFO << " - 030";
    if (!freq.isValid())
        return false;

   // qDebug() << Q_FUNC_INFO << " - 040";

    if (!dateTime.isValid())
        return false;

   // qDebug() << Q_FUNC_INFO << " - 050";
    return true;
}

void DXSpot::setDXCall(const QString &_c)
{
    Callsign callsing(_c);
    if (callsing.isValid())
        dxcall = _c;
}
QString DXSpot::getDxCall() {return dxcall;}

void DXSpot::setSpotter(const QString &_c){
    Callsign callsing(_c);
    if (callsing.isValid())
        spotter = _c;
}
QString DXSpot::getSpotter(){return spotter;}

void DXSpot::setComment(const QString &c){comment = c;}
QString DXSpot::getComment(){return comment;}

void DXSpot::setDateTime(const QDateTime &d){if (d.isValid()) dateTime = d; }
QDateTime DXSpot::getDateTime(){  return dateTime;}

void DXSpot::setClickStatus(const MouseClicks &_s ){clickStatus = _s;}
MouseClicks DXSpot::getClickStatus(){return clickStatus;}

void DXSpot::setFrequency(Frequency f)
{
    if (f.isValid())
        freq = f;
   // qDebug() << Q_FUNC_INFO << ": " << freq.toQString(MHz);
}

Frequency DXSpot::getFrequency()
{
    // Return the frequency stored in the object
    return freq;
}

void DXSpot::setSHDX(bool _shdx)
{
    shdx = _shdx;
}

bool DXSpot::getSHDX(){return shdx;}
