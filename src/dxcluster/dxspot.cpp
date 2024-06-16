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

DXSpot::DXSpot()
{
    valid = false;
}

DXSpot::DXSpot(DXSpot *_other)
{
    dxcall = _other->dxcall;
    spotter = _other->spotter;
    freq = _other->freq; // Might need a copy constructor for Frequency as well
    comment = _other->comment;
    dateTime = _other->dateTime;
    clickStatus = _other->clickStatus;
    valid = _other->valid;
}

DXSpot::~DXSpot(){}

void DXSpot::clear()
{
    dxcall.clear();
    freq.clear();
    spotter.clear();
    dateTime.currentDateTimeUtc();
    valid = false;
    clickStatus = NoClick;
}

void DXSpot::operator=(DXSpot const &_other)
{
    dxcall = _other.dxcall;
    freq = _other.freq; // Might need a copy constructor for Frequency as well
    spotter = _other.spotter;
    comment = _other.comment;
    dateTime = _other.dateTime;
    clickStatus = _other.clickStatus;
    valid = _other.valid;
}

void DXSpot::setValid(const bool _v){valid = _v;}
bool DXSpot::isValid(){return valid;}

void DXSpot::setDXCall(const QString &_c){dxcall = _c;}
QString DXSpot::getDxCall(){return dxcall;}

void DXSpot::setSpotter(const QString &_c){spotter = _c;}
QString DXSpot::getSpotter(){return spotter;}

void DXSpot::setComment(const QString &c){comment = c;}
QString DXSpot::getComment(){return comment;}

void DXSpot::setDateTime(const QDateTime &d){if (d.isValid()) dateTime = d;}
QDateTime DXSpot::getDateTime(){ return dateTime;}

void DXSpot::setClickStatus(const MouseClicks &_s ){clickStatus = _s;}
bool DXSpot::getClickStatus(){return clickStatus;}

void DXSpot::setFrequency(Frequency f)
{
    if (f.isValid())
        freq = f;
}
Frequency DXSpot::getFrequency()
{
    return &freq;
}
