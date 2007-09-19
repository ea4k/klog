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
//#include <kdebug.h>
//#include <ostream.h>
#include "awarddxcc.h"

//-1 Not worked, 0 = worked, 1 = confirmed
AwardDXCC::AwardDXCC(){
// cout << "AwardDXCC::AwardDXCC" << endl;
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
//cout << "AwardDXCC::~AwardDXCC" << endl;
}

void AwardDXCC::clear(){
//cout << "AwardDXCC::clear" << endl;
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
//cout << "AwardDXCC::worked: " << QString::number(tentity) << "/"  << QString::number(tband) << "/"  << QString::number(tmode) << "/" << endl;
	i = tentity;
	j = tband;
	k = tmode;
	if ( dxccb[i][j] != 1)
		dxccb[i][j] = 0;
	if ( dxccm[i][k] != 1)
		dxccm[i][k] = 0;
	
	dxccW[i] = true;

}

void AwardDXCC::workedString(const int tentity, const QString& tband, const QString& tmode){
//cout << "AwardDXCC::workedString" << endl;
	i = tentity;
	sAux = tband;
	j = adif.band2Int(sAux);
	if (j == -1)
		return;
	sAux = tmode;
	k = adif.mode2Int(sAux);
	if (k == -1)
		return;

	if ( dxccb[i][j] != 1)
		dxccb[i][j] = 0;
	if ( dxccm[i][k] != 1)
		dxccm[i][k] = 0;
	dxccW[i] = true;
//  dxccCountWorked++;

}

void AwardDXCC::notWorked(const int tentity, const int tband, const int tmode){
// cout << "AwardDXCC::notWorked" << endl;
  //Needs to have a way to check if it
// cout << "AwardDXCC::notWorked: Enti/Band/Mode: " << QString::number(tentity) <<"/"<< QString::number(tband) <<"/"<< QString::number(tmode)<< endl;
	i = tentity;
	j = tband;
	k = tmode;
	dxccb[i][j] = -1;
	dxccm[i][k] = -1;
	dxccW[i] = false;
	dxccC[i] = false;
}

void AwardDXCC::notWorkedString(const int tentity, const QString& tband, const QString& tmode){
// cout << "AwardDXCC::notWorkedString" << endl;
	i = tentity;
	sAux = tband;
	j = adif.band2Int(sAux);
	if (j == -1)
		return;
	sAux = tmode;
	k = adif.mode2Int(sAux);
	if (k == -1)
		return;

	dxccb[i][j] = -1;
	dxccm[i][k] = -1;
	dxccW[i] = false;
	dxccC[i] = false;
}




bool AwardDXCC::isWorked(const int tentity){
// cout << "AwardDXCC::isWorked" << endl;
	i = tentity;
	return dxccW[i];
}

bool AwardDXCC::isWorkedBand (const int tentity, const int tband){
// cout << "AwardDXCC::isWorkedBand" << endl;
	i = tentity;
	j = tband;
//	if (!dxccW[i])
//		return false;
	if ( (dxccb[i][j] >= 0) )
		return true;
	else
		return false;
}

bool AwardDXCC::isWorkedMode (const int tentity, const int tmode){
// cout << "AwardDXCC::isWorkedMode" << endl;
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
// cout << "AwardDXCC::howManyWorkedBand" << endl;
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
// cout << "AwardDXCC::howManyWorkedMode" << endl;
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
		if (dxccW[i] == true){
			iAux++;}
			//cout << "AwardDXCC::howManyWorked enti/iAux: " << QString::number(i) <<"/"<< QString::number(iAux) << endl;
	}
// cout << "AwardDXCC::howManyWorked: " << QString::number(iAux) << endl;
	return iAux;
//  return  dxccCountWorked;
}


//================================
// CONFIRMED

void AwardDXCC::confirmed(const int tentity, const int tband, const int tmode){
// cout << "AwardDXCC::confirmed" << endl;
	i = tentity;
	j = tband;
	k = tmode;
	dxccb[i][j] = 1;
	dxccm[i][k] = 1;
	dxccC[i] = true;
}

void AwardDXCC::confirmedString(const int tentity, const QString& tband, const QString& tmode){
//cout << "AwardDXCC::confirmedString" << endl;
	i = tentity;

	sAux = tband;
	j = adif.band2Int(sAux);
	if (j == -1)
		return;
	sAux = tmode;
	k = adif.mode2Int(sAux);
	if (k == -1)
		return;

	dxccb[i][j] = 1;
	dxccm[i][k] = 1;
	dxccW[i] = true;
	dxccC[i] = true;
}


bool AwardDXCC::isConfirmed(const int tentity){
// cout << "AwardDXCC::isConfirmed" << endl;
	i = tentity;
	return dxccC[i];
}

bool AwardDXCC::isConfirmedBand (const int tentity, const int tband){
//cout << "AwardDXCC::isConfirmedBand: " << QString::number(tentity) << "/" << QString::number(tband) << endl;
	i = tentity;
	j = tband;
//	if (!dxccC[i])
//		return false;
	if (dxccb[i][j] == 1 ){
		return true;
	}else{
		return false;
	}
}

bool AwardDXCC::isConfirmedMode (const int tentity, const int tmode){
// cout << "AwardDXCC::isConfirmedMode" << endl;
	i = tentity;
	k = tmode;
//	if (!dxccC[i])
//		return false;
	if (dxccb[i][k] == 1 )
		return true;
	else
		return false;

}

int AwardDXCC::howManyConfirmedBand(const int tband){
// cout << "AwardDXCC::howManyConfirmedBand" << endl;
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
// cout << "AwardDXCC::howManyConfirmed" << endl;
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
// cout << "AwardDXCC::howManyConfirmed" << endl;
	iAux = 0;
	for (i = 0; i < ENTITIES; i++){
		if (dxccC[i]==true)
			iAux++;
	}
	return iAux;
}


 void AwardDXCC::notConfirmed(const int tentity, const int tband, const int tmode){
// cout << "AwardDXCC::notConfirmed" << endl;
  //Needs to have a way to check if it
  i = tentity;
  j = tband;
  k = tmode;
  dxccb[i][j] = 0;
  dxccm[i][k] = 0;
  dxccC[i] = false;

}

void AwardDXCC::notConfirmedString(const int tentity, const QString& tband, const QString& tmode){
// cout << "AwardDXCC::notConfirmedString" << endl;
  i = tentity;

  sAux = tband;
  j = adif.band2Int(sAux);
  if (j == -1)
    return;
  sAux = tmode;
  k = adif.mode2Int(sAux);
  if (k == -1)
    return;
  dxccb[i][j] = 0;
  dxccm[i][k] = 0;
  dxccC[i] = false;
}
