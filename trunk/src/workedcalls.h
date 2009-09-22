/***************************************************************************
                          workedcalls  -  description
                             -------------------
    begin                : mar ago 17 2004
    copyright            : (C) 2004 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
 *                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/
#ifndef WORKEDCALLS_H
#define WORKEDCALLS_H
#include <QList>
#include <QString>
#include <QMap>
#include "calls.h"
//TODO:
/* IMPORTANT NOTE !!
  Maybe just a table with a CALL <-> "array of numbers" with the QSOs of
  the calls would be OK instead of saving all the data related to every
  call.
  Maybe I should optimize this in order to save memory when executing.
*/


class WorkedCalls{
public:

    WorkedCalls();
    ~WorkedCalls();

//	New functions and optimizations

    int addCall(const QString& workedCall, const int tnum);
    int findCall(const QString& workedCall); // 0 or the number of the QSO

private:
    typedef QList<Calls> TheWorkedCalls;
    TheWorkedCalls workedCalls;

    Calls call;
    QString  callString;
    int i;

//	New functions and optimizations

    typedef QMap<QString, int> QRZMap;  	// To map QRZ/first QSO Number
    QRZMap qrzMap;				// Prefix/Entity
    QRZMap::Iterator itQR;			//Iterator


};
#endif


