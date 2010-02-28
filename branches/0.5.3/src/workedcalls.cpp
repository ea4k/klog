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
//#include <klocale.h>

//#include "iostream"
#include "workedcalls.h"

//using namespace std;


WorkedCalls::WorkedCalls(){
//	itQR = qrzMap.begin();
}
WorkedCalls::~WorkedCalls(){
}


int WorkedCalls::addCall(const QString& workedCall, const int tnum){
//qDebug() << "WorkedCalls::addCall: " << workedCall << "/" << QString::number(tnum) << endl;
	i = 0;
	i = findCall(workedCall);
	if (i <= 0){
		i = tnum;
		WorkedCalls::qrzMap[workedCall] = i;
		return i;
	}else{
		//qDebug() << "WorkedCalls::addCall: Call already worked " << endl;
		return 0;
	}
}

int WorkedCalls::findCall(const QString& workedCall){
//qDebug() << "WorkedCalls::findCall: " << workedCall << endl;
	if ((workedCall=="NULL")||(workedCall.isEmpty())||(workedCall.isNull())){ // We are calling this function just after start KLog	
//		qDebug() << "WorkedCalls::findCall: NULL CALL or empty"  << endl;
		return 0;
	}


// 	itQR = qrzMap.begin();
// 	itQR = qrzMap.find(workedCall);
// 	if (qrzMap.find(workedCall) != qrzMap.end()){
// 		return itQR.data();
// 	}
// 	else
// 		return 0;



	for (itQR = qrzMap.begin(); itQR != qrzMap.end(); ++itQR){
		//qDebug() << "WorkedCall: " << workedCall << "/" << itQR.key() << endl;
		if (itQR.key() == workedCall){
//			qDebug() << "WorkedCall: FOUND!" << workedCall << "/" << QString::number(itQR.data())  << endl;	
                        return itQR.value();
		}
	
	}
//	qDebug() << "WorkedCall::findCall: NOT FOUND: " << workedCall  << endl;
	return 0;
}
