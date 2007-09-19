/***************************************************************************
                          workedcalls  -  description
                             -------------------
    begin                : jue ago 19 2004
    copyright            : (C) 2004 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the Free Software              *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*
 *****************************************************************************/
#include <klocale.h>
//#include <ostream.h>
#include "workedcalls.h"

WorkedCalls::WorkedCalls(){
}
WorkedCalls::~WorkedCalls(){
}

/*
  TheWorkedCalls workedCalls;
  Calls call;
  QString  callString;
*/
void WorkedCalls::addCallToWorkedCalls(const QString& tqrz, int tnum){
	call.setNumb(tnum);
	call.setQrz(tqrz);
	workedCalls.append(call);
}

int WorkedCalls::getWorked(const QString& workedCall){
	TheWorkedCalls::iterator it;
	for ( it = workedCalls.begin(); it != workedCalls.end(); ++it ){
		if (workedCall == (*it).getQrz() )
			return (*it).getNumb();
	}
	return -1;
}

bool WorkedCalls::isThisCallWorked(const QString& workedCall){
	TheWorkedCalls::iterator it;
	for ( it = workedCalls.begin(); it != workedCalls.end(); ++it ){
		if (workedCall == (*it).getQrz() )
			return true;
	}
	return false;
}

