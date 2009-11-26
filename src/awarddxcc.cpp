/***************************************************************************
                          awardDXCC.cpp  -  description
                             -------------------
    begin                : vie ene 31 2003
    copyright            : (C) 2003 by Jaime Robles
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
#include "awarddxcc.h"

//using namespace std;

//-1 Not worked, 0 = worked, 1 = confirmed
AwardDXCC::AwardDXCC(){
////cout << "AwardDXCC::AwardDXCC" << endl;
	i = 0;
	j = 0;
	k = 0;
	for (i = 0; i <  ENTITIES; i++){
		AwardDXCC::dxccW[i] = false;
		AwardDXCC::dxccC[i] = false;
		for (j = 0; j <  BANDS; j++){
			AwardDXCC::dxccb[i][j] = -1;
		}
		for (k = 0; k <  MODES; k++){
			AwardDXCC::dxccm[i][k] = -1;
		}
	}
}

AwardDXCC::~AwardDXCC(){
////cout << "AwardDXCC::~AwardDXCC" << endl;
}

void AwardDXCC::clear(){
////cout << "AwardDXCC::clear" << endl;
  i = 0;
  j = 0;
  k = 0;
  for (i = 0; i <  ENTITIES; i++){
    AwardDXCC::dxccW[i] = false;
    AwardDXCC::dxccC[i] = false;
    for (j = 0; j <  BANDS; j++){
        AwardDXCC::dxccb[i][j] = -1;
    }
    for (k = 0; k <  MODES; k++){
        AwardDXCC::dxccm[i][k] = -1;
    }
  }
}

void AwardDXCC::worked(const int tentity, const int tband, const int tmode){
//qDebug () << "AwardDXCC::worked: " << QString::number(tentity) << "/"  << QString::number(tband) << "/"  << QString::number(tmode) << "/" << endl;
	i = tentity;
	j = tband;
	k = tmode;
	if ( dxccb[i][j] < 1){
		dxccb[i][j] = 0;
	}
	if ( dxccm[i][k] < 1){
		dxccm[i][k] = 0;
	}	
	dxccW[i] = true;

}

void AwardDXCC::workedString(const int tentity, const QString& tband,const QString& tmode){
//cout << "AwardDXCC::workedString: Entity: " << QString::number(tentity)<< "- Band: " << tband << " - Mode: " << tmode << endl;
	i = tentity;
	j = adif.band2Int(tband);
	dxccW[i] = true;

	if (j == -1){ // band not valid
//cout << "AwardDXCC::workedString: ERROR Entity: " << QString::number(tentity)<< "- Band: " << tband << "- Mode: " << tmode << " NOT VALID BAND" << endl;
		return;
	}
	k = adif.mode2Int(tmode);
	if (k == -1){ // mode not valid
		return;
	}
	if ( dxccb[i][j] < 1){
		dxccb[i][j] = 0;
	}
	if ( dxccm[i][k] < 1){
		dxccm[i][k] = 0;
	}

//  dxccCountWorked++;

}

void AwardDXCC::notWorked(const int tentity, const int tband, const int tmode){
//cout << "AwardDXCC::notWorked: Entity: " << QString::number(tentity)<< "/Band: " << tband << "/Mode: " << tmode << endl;
//Needs to have a way to check if it
// //cout << "AwardDXCC::notWorked: Enti/Band/Mode: " << QString::number(tentity) <<"/"<< QString::number(tband) <<"/"<< QString::number(tmode)<< endl;
	i = tentity;
	j = tband;
	k = tmode;
	dxccb[i][j] = -1;
	dxccm[i][k] = -1;
	dxccW[i] = false;
	dxccC[i] = false;
}

void AwardDXCC::notWorkedString(const int tentity, const QString& tband, const QString& tmode){
//cout << "AwardDXCC::notWorkedString: Entity: " << QString::number(tentity)<< "/Band: " << tband << "/Mode: " << tmode << endl;
	i = tentity;

	j = adif.band2Int(tband);
	if (j == -1){ // band not valid
		return;
	}
	//sAux = tband;
	j = adif.band2Int(tband);
	if (j == -1)
		return;
	//sAux = tmode;
	k = adif.mode2Int(tmode);
	if (k == -1)
		return;

	dxccb[i][j] = -1;
	dxccm[i][k] = -1;
	dxccW[i] = false;
	dxccC[i] = false;
}




bool AwardDXCC::isWorked(const int tentity){
//cout << "AwardDXCC::isWorked; " << QString::number(tentity) << endl;
	i = tentity;
	return dxccW[i];
}

bool AwardDXCC::isWorkedBand (const int tentity, const int tband){
//cout << "AwardDXCC::isWorkedBand: Entity: " << QString::number(tentity) << "/Band: " << QString::number(tband) << endl;
	i = tentity;
	j = tband;
	if ( (dxccb[i][j] >= 0) ){
		return true;
	}else{
		return false;
	}
}

bool AwardDXCC::isWorkedMode (const int tentity, const int tmode){
// //cout << "AwardDXCC::isWorkedMode" << endl;
//cout << "AwardDXCC::isWorkedMode: Entity: " << QString::number(tentity) << "/Band: " << QString::number(tmode)<< endl;
	i = tentity;
	k = tmode;
//	if (!dxccW[i])
//		return false;
	if (dxccm[i][k] >= 0 )
		return true;
	else
		return false;
}

int AwardDXCC::howManyWorkedBand(const int tband){
//cout << "AwardDXCC::howManyWorkedBand: " << QString::number(tband) << endl;
	iAux = 0;
	j = tband;
	for (i = 0; i < ENTITIES; i++){
		for (j = 0; j < BANDS; j++)
			if (dxccb[i][j] >= 0 )
				iAux++;
	}
	return iAux;
}

int AwardDXCC::howManyWorkedMode(const int tmode){
//cout << "AwardDXCC::howManyWorkedMode: " << QString::number(tmode)<< endl;
  iAux = 0;
  k = tmode;
  for (i = 0; i < ENTITIES; i++){
    for (k = 0; k < MODES; k++)
     if (dxccm[i][k] >= 0 )
        iAux++;
  }
  return iAux;

}

int AwardDXCC::howManyWorked(){
//cout << "AwardDXCC::howManyWorked" << endl;
	iAux = 0;
	for (i = 0; i < ENTITIES; i++){
		if (dxccW[i]){
			iAux++;}
			////cout << "AwardDXCC::howManyWorked enti/iAux: " << QString::number(i) <<"/"<< QString::number(iAux) << endl;
	}
// //cout << "AwardDXCC::howManyWorked: " << QString::number(iAux) << endl;
	return iAux;
//  return  dxccCountWorked;
}


//================================
// CONFIRMED

void AwardDXCC::confirmed(const int tentity, const int tband, const int tmode){
//qDebug() << "AwardDXCC::confirmed: Entity: " << QString::number(tentity) << "/Band: " << QString::number(tband) << QString::number(tmode)<< endl;
	i = tentity;
	j = tband;
	k = tmode;
	dxccb[i][j] = 1;
	dxccm[i][k] = 1;
	dxccC[i] = true;
	dxccW[i] = true;
}

void AwardDXCC::confirmedString(const int tentity, const QString& tband, const QString& tmode){
//cout << "AwardDXCC::confirmedString: Entity: " << QString::number(tentity) << " - Band: " << tband << "- Mode: " << tmode << endl;
	i = tentity;


	j = adif.band2Int(tband);
	if (j == -1){ // band not valid
		return; 
	}
	k = adif.mode2Int(tmode);
	if (k == -1){ // mode not valid
		return;
	}

	dxccW[i] = true;
	dxccC[i] = true;
	dxccb[i][j] = 1;
	dxccm[i][k] = 1;

}


bool AwardDXCC::isConfirmed(const int tentity){
//cout << "AwardDXCC::isConfirmed: " << QString::number(tentity) << endl;
	i = tentity;
	return dxccC[i];
}

bool AwardDXCC::isConfirmedBand (const int tentity, const int tband){
//cout << "AwardDXCC::isConfirmedBand: " << QString::number(tentity) << "/" << adif.int2Band(tband) << " = " << QString::number(AwardDXCC::dxccb[tentity][tband]) << endl;
	i = tentity;
	j = tband;

	if ( (AwardDXCC::dxccb[i][j] > 0) ){
		return true;
	}else{
		return false;
	}
}

bool AwardDXCC::isConfirmedMode (const int tentity, const int tmode){
//cout << "AwardDXCC::isConfirmedMode: " << QString::number(tentity) << "/" << QString::number(tmode) << endl;
	i = tentity;
	k = tmode;
//	if (!dxccC[i])
//		return false;
	if (dxccb[i][k] > 0 )
		return true;
	else
		return false;

}

int AwardDXCC::howManyConfirmedBand(const int tband){
//cout << "AwardDXCC::howManyConfirmedBand: " << QString::number(tband) << endl;
	iAux = 0;
	j = tband;
	for (i = 0; i < ENTITIES; i++){
		for (j = 0; j < BANDS; j++)
			if (dxccb[i][j] == 1 )
				iAux++;
	}
	return iAux;
}

int AwardDXCC::howManyConfirmedMode(const int tmode){
//cout << "AwardDXCC::howManyConfirmedMode"  << QString::number(tmode) << endl;
	iAux = 0;
	k = tmode;
	for (i = 0; i < ENTITIES; i++){
		for (k = 0; k < MODES; k++)
			if (dxccm[i][k] == 1 )
				iAux++;
	}
	return iAux;

}

int AwardDXCC::howManyConfirmed(){
//cout << "AwardDXCC::howManyConfirmed" << endl;
	iAux = 0;
	for (i = 0; i < ENTITIES; i++){
		if (dxccC[i])
			iAux++;
	}
	return iAux;
}


 void AwardDXCC::notConfirmed(const int tentity, const int tband, const int tmode){
//cout << "AwardDXCC::notConfirmed: e:" << QString::number(tentity) << "/b:" << QString::number(tband) << "/m:"<< QString::number(tmode) << endl;
  //Needs to have a way to check if it
  i = tentity;
  j = tband;
  k = tmode;
  dxccb[i][j] = 0;
  dxccm[i][k] = 0;
  dxccC[i] = false;

}

void AwardDXCC::notConfirmedString(const int tentity, const QString& tband, const QString& tmode){
//cout << "AwardDXCC::notConfirmedString: e:" << tentity << "/b:" << tband << "/m:"<< tmode << endl;
  i = tentity;

  //sAux = tband;
  j = adif.band2Int(tband);
  if (j == -1)
    return;
  //sAux = tmode;
  k = adif.mode2Int(tmode);
  if (k == -1)
    return;
  dxccb[i][j] = 0;
  dxccm[i][k] = 0;
  dxccC[i] = false;
}
