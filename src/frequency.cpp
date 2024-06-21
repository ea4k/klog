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
#include <QDebug>

Frequency::Frequency(){}

Frequency::Frequency(const Frequency &f){
    freq = f.freq;
}

Frequency::Frequency(const double _f, FreqUnits _u)
{
    freq = normalize(_f, _u);
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
    //qDebug() << Q_FUNC_INFO << ": " << _f;
    bool ok;
    freq =_f.toDouble(&ok);
    //qDebug() << Q_FUNC_INFO << " - freq: " << _f;
    if (!ok)
        return false;
    //qDebug() << Q_FUNC_INFO << " - Trying to normalize...";
    freq = normalize(freq, _u);
    //qDebug() << Q_FUNC_INFO << " - freqNormalized: " << _f;
    return isValid();
}

/*
bool Frequency::fromBand(const QString &_bandName)
{
    DataProxy_SQLite dataProxy(Q_FUNC_INFO);
    int id = dataProxy.getIdFromBandName(_bandName);
    freq = fromQString(dataProxy.getFreqFromBandId(id));
    return isValid();
}
*/

double Frequency::toDouble(FreqUnits _u)
{
    return deNormalize(freq, _u);
}

QString Frequency::toQString(FreqUnits _u)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(freq);
    //qDebug() << Q_FUNC_INFO << ":u: " << QString::number(_u);

    double d = toDouble(_u);
    //qDebug() << Q_FUNC_INFO << ":d: " << QString::number(freq);

    int dec = getDecimals(_u);
    //qDebug() << Q_FUNC_INFO << ":u: " << QString::number(_u);


    //qDebug() << Q_FUNC_INFO << ": Freq: " << QString::number(toDouble());
    //qDebug() << Q_FUNC_INFO << ":  dec: " << QString::number(dec);

    return  QString("%1").arg(d, 0, 'f', dec);
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
    case GHz:
        return _f * 1000;
        break;
    case THz:
        return _f * 1000000;
        break;
    default:                // Default is in MHz
        return _f;
        break;
    }
}

double Frequency::deNormalize(const double _f, const FreqUnits _u)
{ // Convert MHz to other unit
    switch (_u) {
    case Hz:
        return _f * 1000000;
        break;
    case KHz:
        return _f * 1000;
        break;
    case GHz:
        return _f / 1000;
        break;
    case THz:
        return _f / 1000000;
        break;
    default:                // Default is in MHz
        return _f;
        break;
    }
}

int Frequency::getDecimals(const FreqUnits _u)
{ // Get the right number of decimals to display
    switch (_u) {
    case Hz:
        return 0;
        break;
    case KHz:
        return 0;
        break;
    case MHz:
        return 3;
        break;
    case GHz:
        return 6;
        break;
    case THz:
        return 9;
        break;
    default:                // Default is in MHz
        return 6;
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
/*
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
*/

bool Frequency::isValid()
{
    return (freq>=0);
}

void Frequency::operator=(Frequency const &_f2)
{
    freq = _f2.freq;
}
