#include "awarddxmarathon.h"
/***************************************************************************
                          awarddxmarathon.cpp  -  description
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

#include <QDebug>

DXMarathon::DXMarathon(DataProxy_SQLite *dp)
{
       //qDebug() << "DXMarathon::DXMarathon" ;
    dataProxy = dp;
       //qDebug() << "DXMarathon::DXMarathon - END" ;
}


int DXMarathon::getDXMarathonQSO(const int _year, const int _logNumber)
{
         //qDebug() << "DXMarathon::getDXMarathonQSO: " << QString::number(_year);
    return dataProxy->getQSOonYear(_year, _logNumber);
}

int DXMarathon::getDXMarathonDXCC(const int _year, const int _logNumber)
{
         //qDebug() << "DXMarathon::getDXMarathonDXCC: " << QString::number(_year);
    return dataProxy->getDXCConYear(_year, _logNumber);
}

int DXMarathon::getDXMarathonCQ(const int _year, const int _logNumber)
{
         //qDebug() << "DXMarathon::getDXMarathonCQ: " << QString::number(_year);
    return dataProxy->getCQzonYear(_year, _logNumber);
}

int DXMarathon::getDXMarathonScore(const int _year, const int _logNumber)
{
         //qDebug() << "DXMarathon::getDXMarathonScore: " << QString::number(_year);

    return ( getDXMarathonDXCC(_year, _logNumber) + getDXMarathonCQ(_year, _logNumber));

}

bool DXMarathon::neededForDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber)
{
    return dataProxy->newDXMarathon(_dxcc, _cq, _year, _logNumber);

}
