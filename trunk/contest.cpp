/***************************************************************************
                          contest.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/
/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify         *
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "contest.h"
#include <QDebug>
Contest::Contest()
{
    constrid = 1;
     qDebug() << "Contest::Contest - START:END"  << endl;
}

Contest::Contest(const QStringList _qs)
{
    constrid = 2;
     qDebug() << "Contest::Contest (_qs) - START:END"  << endl;
}

Contest::~Contest()
{
}


bool Contest::isMultiplier(const QStringList _qs)
{
     qDebug() << "Contest::isMultiplier"  << endl;
    return false;
}

int Contest::getQSOPoints(const QStringList _qs)
{
     qDebug() << "Contest::getQSOPoints"  << endl;
    return 0;
}

int Contest::getTotalScore()
{
    return 0;
}

int Contest::getMultipliers()
{
    return 0;
}
int Contest::getPoints()
{
    return 0;
}

bool Contest::saveFileToSend(const QString& _fileName)
{
     qDebug() << "Contest::saveFileToSend"  << endl;
    return false;
}

