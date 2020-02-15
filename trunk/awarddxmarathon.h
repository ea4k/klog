#ifndef AWARDDXMARATHON_H
#define AWARDDXMARATHON_H
/***************************************************************************
                          awarddxmarathon.h  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include "dataproxy_sqlite.h"


class DXMarathon
{
public:
    DXMarathon(DataProxy_SQLite *dp);

    int getDXMarathonQSO(const int _year, const int _logNumber);
    int getDXMarathonDXCC(const int _year, const int _logNumber);
    int getDXMarathonCQ(const int _year, const int _logNumber);
    int getDXMarathonScore(const int _year, const int _logNumber);
    bool neededForDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber);

private:
    DataProxy_SQLite *dataProxy;

};

#endif // AWARDDXMARATHON_H
