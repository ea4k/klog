/***************************************************************************
 *   Copyright (C) 2009 by Jaime Robles, EA4TV                             *
 *   http://jaime.robles.es/klog - jaime@robles.es                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "iostream"
#include "cabrillomanagement.h"

//using namespace std;

CabrilloManagement::CabrilloManagement(){
	qso.clearQso();
};


CabrilloManagement::~CabrilloManagement(){}

void CabrilloManagement::cabrilloReadLog(const QString& tfileName){
//LogBook CabrilloManagement::cabrilloReadLog(const QString& tfileName){
//cout << "CabrilloManagement::cabrilloReadLog" << endl;

	QFile file( tfileName );
//	int totalQsos = 0; // QSOs in the log to be read
//	bool qslViac = false;
//	int progresStep = 0;

	QTextStream stream( &file );

//	int len; // for the tab length
	QString data = "";
//	int aux = 0;
//	bool inHeader = true;
	
	if ( file.open( QIODevice::ReadOnly ) ) {
		data = stream.readLine();
		while (data.isEmpty() && (!stream.atEnd()) ){
			data = stream.readLine();
			processLogLine (data);
		}
		

		file.close();
	} // Closes the if from the file (file could not be opened)
	//return logbook;
}

void CabrilloManagement::processLogLine (const QString& tLogLine){
//cout << "CabrilloManagement::processLogLine" << endl;
        int a=0;
	int intAux;

	qsoLine="";
	qsoLine = tLogLine;

	if (qsoLine.isEmpty()){
		return;
	}
	
	qsoLine = qsoLine.simplified().toUpper();

	intAux = qsoLine.count(' ');			// How many words has the line?
	fields = qsoLine.split(" ", QString::SkipEmptyParts);
	//fields = QStringList::split(' ', qsoLine );	// Split the line in fields

	cabrilloTab = fields[a].section(':',0,0);
//	cout << "CabrilloManagement::processLogLine: " << cabrilloTab << endl;

//	theData = ((fields[a]).section(':',1));
//	cout << "CabrilloManagement::processLogLine - qsoLine: " << qsoLine << endl;
//	cout << "CabrilloManagement::processLogLine - AdifTab: " << adifTab << endl;
//	cout << "CabrilloManagement::processLogLine - theData: " << theData << endl;


}
