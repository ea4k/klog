#ifndef FREQUENCY_H
#define FREQUENCY_H
/***************************************************************************
                          frequency.h  -  description
                             -------------------
    begin                : Apr 2024
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
/*
    This class implements Frequency.
*/

#include <QObject>
#include "klogdefinitions.h"


class Frequency: public QObject
{
    Q_OBJECT
    friend class tst_Frequency;

public:
    //Frequency() : freq(0.0), bandInMHz(""), tolerance(0.0) {};
    Frequency();
    Frequency(const Frequency &f);
    Frequency(const double _f, FreqUnits _u = MHz);
    //Frequency(const QString &_parentName);
    //Frequency(const QString &_parentName, const Frequency &_f);
    ~Frequency();
    void clear();
    bool fromDouble(const double _f, FreqUnits _u = MHz);
    bool fromQString(const QString &_f, FreqUnits _u = MHz);
    //bool fromBand(const QString &_bandName);
    double toDouble(FreqUnits _u = MHz);                        // Returns in MHz
    QString toQString(FreqUnits _u = MHz);   // Returns in MHz with decimals
    void setTolerance(const double _t, FreqUnits _u = Hz);      // Defines the tolerance
    //QString band();                                           // Returns the band
    //int bandId();                                             // Returns the bandId
    bool isValid();
    void operator=(Frequency const &_f2);                       // Redefinition of =


private:
    double normalize(const double _f, const FreqUnits _u = MHz);
    double deNormalize(const double _f, const FreqUnits _u = MHz);
    int getDecimals(const FreqUnits _u = MHz);
    double freq;        // This must be in MHz
    QString bandInMHz;  //
    double tolerance;   // This must be in Hz
};

#endif // FREQUENCY_H
