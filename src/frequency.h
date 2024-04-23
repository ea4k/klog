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
    This class implements Frequency
    This classs hould not need to query the DB neither the DataProxy Class
*/

#include <QObject>
#include "klogdefinitions.h"
#include "dataproxy_sqlite.h"

class Frequency: public QObject {
    Q_OBJECT
public:
    Frequency();
    Frequency(double _f);
    ~Frequency();
    void clear();
    bool fromDouble(const double _f, FreqUnits _u = MHz);
    bool fromQString(const QString &_f, FreqUnits _u = MHz);
    bool fromBand(const QString &_bandName);
    double toDouble(FreqUnits _u = MHz);                        // Returns in MHz
    QString toQString(int _decimals = 3, FreqUnits _u = MHz);   // Returns in MHz with decimals
    void setTolerance(const double _t, FreqUnits _u = Hz);      // Defines the tolerance
    QString band();                                             // Returns the band
    int bandId();                                               // Returns the bandId
    bool isValid();


private:
    double normalize(const double _f, const FreqUnits _u = MHz);
    double freq;        // This must be in MHz
    QString bandInMHz;  //
    double tolerance;   // This must be in Hz

};

#endif // FREQUENCY_H
