/***************************************************************************
                          frequency.cpp  -  description
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
#include "frequency.h"


Frequency::Frequency(){}

Frequency::Frequency(double _f)
{
    freq = _f;
}

Frequency::~Frequency(){}

void Frequency::clear()
{
    freq = 0.0;
    bandInMHz = QString();
}

bool Frequency::fromDouble(const double _f, FreqUnits _u)
{
    freq = normalize(_f, _u);
    return isValid();
}

bool Frequency::fromQString(const QString &_f, FreqUnits _u)
{
    bool ok;
    freq =_f.toDouble(&ok);
    if (!ok)
        return false;
    freq = normalize(freq, _u);
    return isValid();
}

bool Frequency::fromBand(const QString &_bandName)
{
    DataProxy_SQLite dataProxy(Q_FUNC_INFO);
    int id = dataProxy.getIdFromBandName(_bandName);
    freq = fromQString(dataProxy.getFreqFromBandId(id));
    return isValid();
}

double Frequency::toDouble(FreqUnits _u)
{
    switch (_u) {
    case Hz:
        return freq * 1000000;
        break;
    case KHz:
        return freq * 1000;
        break;
    default:
        return freq;
        break;
    }
}

QString Frequency::toQString(int _decimals, FreqUnits _u)
{
    double d = toDouble(_u);
    return  QString("%1").arg(d, 0, 'f', _decimals);
}

double Frequency::normalize(const double _f, const FreqUnits _u)
{ // Convert to MHz any frequency
    switch (_u) {
    case Hz:
        return _f / 1000000;
        break;
    case KHz:
        return _f / 1000;
        break;
    default:
        return _f;
        break;
    }
}

void Frequency::setTolerance(const double _t, FreqUnits _u)
{
    if (_t<=0.0)
        return;

    switch (_u) {
    case KHz:
        tolerance = _t * 1000;
        break;
    case MHz:
        tolerance = _t * 1000000;
        break;
    default:
        tolerance = _t;
        break;
    }
}

QString Frequency::band()
{
    DataProxy_SQLite dataProxy(Q_FUNC_INFO);
    return dataProxy.getBandNameFromFreq(toDouble());
}

int Frequency::bandId()
{
    DataProxy_SQLite dataProxy(Q_FUNC_INFO);
    return dataProxy.getBandIdFromFreq(toDouble());
}

bool Frequency::isValid()
{
    return (freq>=0);
}
