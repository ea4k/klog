/***************************************************************************
                          awardWAZ.cpp  -  description
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
//#include <kdebug.h>

#include "awardwaz.h"

//-1 Not worked, 0 = worked, 1 = confirmed
AwardWAZ::AwardWAZ(){
//qDebug()  << "AwardWAZ::AwardWAZ" << endl;
	i = 0;
	j = 0;
	k = 0;
	for (i = 0; i <  ZONES; i++){
		AwardWAZ::wazW[i] = false;
		AwardWAZ::wazC[i] = false;
		for (j = 0; j <  BANDS; j++){
			AwardWAZ::wazb[i][j] = -1;
		}
		for (k = 0; k <  MODES; k++){
			AwardWAZ::wazm[i][k] = -1;
		}
	}
}

AwardWAZ::~AwardWAZ(){
//qDebug()  << "AwardWAZ::~AwardWAZ" << endl;
}

void AwardWAZ::clear(){
//qDebug()  << "AwardWAZ::clear" << endl;
  i = 0;
  j = 0;
  k = 0;
  for (i = 0; i <  ZONES; i++){
    AwardWAZ::wazW[i] = false;
    AwardWAZ::wazC[i] = false;
    for (j = 0; j <  BANDS; j++){
        AwardWAZ::wazb[i][j] = -1;
    }
    for (k = 0; k <  MODES; k++){
        AwardWAZ::wazm[i][k] = -1;
    }
  }
}

void AwardWAZ::worked(const int tzone, const int tband, const int tmode){
//qDebug()  << "AwardWAZ::worked: " << QString::number(tzone) <<"/" << QString::number(tband)<<"/" << QString::number(tmode)<< endl;
	i = tzone;
	j = tband;
	k = tmode;
	if ( wazb[i][j] < 1){
		wazb[i][j] = 0;
	}	
	if ( wazm[i][k] < 1){
		wazm[i][k] = 0;
	}
	wazW[i] = true;

}

void AwardWAZ::workedString(const int tzone, const QString& tband, const QString& tmode){
//qDebug()  << "AwardWAZ::workedString: " << QString::number(tzone) <<"/" << tband<<"/" << tmode<< endl;
//	i = tzone;
//	sAux = tband;
	j = adif.band2Int(tband);
	if (j == -1)
		return;
//	sAux = tmode;
	k = adif.mode2Int(tmode);
	if (k == -1)
		return;

	if ( wazb[tzone][j] < 1)
		wazb[tzone][j] = 0;
	if ( wazm[tzone][k] < 1)
		wazm[tzone][k] = 0;
	wazW[i] = true;
//  wazCountWorked++;

}

void AwardWAZ::notWorked(const int tzone, const int tband, const int tmode){
//qDebug()  << "AwardWAZ::notWorked: CQZ/Band/Mode: " << QString::number(tzone) <<"/"<< QString::number(tband) <<"/"<< QString::number(tmode)<< endl;

	i = tzone;
	j = tband;
	k = tmode;
	wazb[i][j] = -1;
	wazm[i][k] = -1;
	wazW[i] = false;
	wazC[i] = false;
}

void AwardWAZ::notWorkedString(const int tzone, const QString& tband, const QString& tmode){
//qDebug()  << "AwardWAZ::notWorkedString: " << QString::number(tzone) <<"/" << tband <<"/" << tmode<< endl;
	i = tzone;
	
	sAux = tband;
	j = adif.band2Int(sAux);
	if (j == -1)
		return;
	sAux = tmode;
	k = adif.mode2Int(sAux);
	if (k == -1)
		return;

	wazb[i][j] = -1;
	wazm[i][k] = -1;
	wazW[i] = false;
	wazC[i] = false;
}




bool AwardWAZ::isWorked(const int tzone){
//qDebug()  << "AwardWAZ::isWorked" << endl;
	i = tzone;
	return wazW[i];
}

bool AwardWAZ::isWorkedBand (const int tzone, const int tband){
//qDebug()  << "AwardWAZ::isWorkedBand" << endl;
	i = tzone;
	j = tband;
	if ( (wazb[i][j] >= 0) )
		return true;
	else
		return false;
}

bool AwardWAZ::isWorkedMode (const int tzone, const int tmode){
//qDebug()  << "AwardWAZ::isWorkedMode" << endl;
	i = tzone;
	k = tmode;
//	if (!wazW[i])
//		return false;
	if (wazm[i][k] >= 0 )
		return true;
	else
		return false;
}

int AwardWAZ::howManyWorkedBand(const int tband){
//qDebug()  << "AwardWAZ::howManyWorkedBand" << endl;
	iAux = 0;
	j = tband;
	for (i = 0; i < ZONES; i++){
		for (j = 0; j < BANDS; j++)
			if (wazb[i][j] >= 0 )
			{
				iAux++;
			}
	}
	return iAux;
}

int AwardWAZ::howManyWorkedMode(const int tmode){
//qDebug()  << "AwardWAZ::howManyWorkedMode" << endl;
	iAux = 0;
	k = tmode;
	for (i = 0; i < ZONES; i++){
		for (k = 0; k < MODES; k++)
			if (wazm[i][k] >= 0 )
				iAux++;
	}
	return iAux;

}

int AwardWAZ::howManyWorked(){
//qDebug()  << "AwardWAZ::howManyWorked" << endl;
	iAux = 0;
	for (i = 0; i < ZONES; i++){
		if (wazW[i]){
			iAux++;
//			qDebug()  << "AwardWAZ::howManyWorked Zone/iAux: " << QString::number(i) <<"/"<< QString::number(iAux) << endl;
		}
			
	}
//	qDebug()  << "AwardWAZ::howManyWorked: " << QString::number(iAux) << endl;
	return iAux;
}


//================================
// CONFIRMED

void AwardWAZ::confirmed(const int tzone, const int tband, const int tmode){
//qDebug()  << "AwardWAZ::confirmed (Z/B/M): (" << QString::number(tzone) << "/" << QString::number(tband) << "/" << QString::number(tmode) << ")" << endl;
	//worked(tzone, tband, tmode);
	i = tzone;
	j = tband;
	k = tmode;
	wazb[i][j] = 1;
	wazm[i][k] = 1;
	wazC[i] = true;
	
}

void AwardWAZ::confirmedString(const int tzone, const QString& tband, const QString& tmode){
//qDebug()  << "AwardWAZ::confirmedString (Z/B/M): (" << QString::number(tzone) << "/" << tband << "/" << tmode << ")"<< endl;
	//workedString(tzone, tband, tmode);
	i = tzone;

	sAux = tband;
	j = adif.band2Int(sAux);
	if (j == -1)
		return;
	sAux = tmode;
	k = adif.mode2Int(sAux);
	if (k == -1)
		return;

	wazb[i][j] = 1;
	wazm[i][k] = 1;
	wazW[i] = true;
	wazC[i] = true;

}


bool AwardWAZ::isConfirmed(const int tzone){
//qDebug()  << "AwardWAZ::isConfirmed(Z): " << QString::number(tzone) << endl;
	i = tzone;
	return wazC[i];
}

bool AwardWAZ::isConfirmedBand (const int tzone, const int tband){
//qDebug()  << "AwardWAZ::isConfirmedBand (Z/B)" << QString::number(tzone) << "/" << QString::number(tband) << ")" << endl;
	i = tzone;
	j = tband;
//	if (!wazC[i])
//		return false;
	if (AwardWAZ::wazb[i][j] > 0 )
		return true;
	else
		return false;
}

bool AwardWAZ::isConfirmedMode (const int tzone, const int tmode){
//qDebug()  << "AwardWAZ::isConfirmedMode(Z/M)" << QString::number(tzone) << "/" << QString::number(tmode) << ")" << endl;;
	i = tzone;
	k = tmode;
	if (AwardWAZ::wazb[i][k] > 0 )
		return true;
	else
		return false;

}

int AwardWAZ::howManyConfirmedBand(const int tband){
//qDebug()  << "AwardWAZ::howManyConfirmedBand: " << QString::number(tband) << endl;
	iAux = 0;
	j = tband;
	for (i = 0; i < ZONES; i++){
		for (j = 0; j < BANDS; j++)
			if (AwardWAZ::wazb[i][j] == 1 )
				iAux++;
	}
	return iAux;
}

int AwardWAZ::howManyConfirmedMode(const int tmode){
//qDebug()  << "AwardWAZ::howManyConfirmed: " << QString::number(tmode)  << endl;
	iAux = 0;
	k = tmode;
	for (i = 0; i < ZONES; i++){
		for (k = 0; k < MODES; k++)
			if (wazm[i][k] == 1 )
				iAux++;
	}
	return iAux;

}

int AwardWAZ::howManyConfirmed(){
//qDebug() << "AwardWAZ::howManyConfirmed" << endl;
  iAux = 0;
  for (i = 0; i < ZONES; i++){
//		if (wazC[i]==true){
  if (wazC[i]){
    iAux++;
    //  qDebug() << "AwardWAZ::howManyConfirmed: " << QString::number(i)<< endl;
   }
  }
  //qDebug() << "AwardWAZ::howManyConfirmed-Total: " << iAux;	
  return iAux;
}

 void AwardWAZ::notConfirmed(const int tzone, const int tband, const int tmode){
//qDebug()  << "AwardWAZ::notConfirmed" << endl;
  //Needs to have a way to check if it
	i = tzone;
	j = tband;
	k = tmode;
	wazb[i][j] = 0;
	wazm[i][k] = 0;
	wazC[i] = false;

}

void AwardWAZ::notConfirmedString(const int tzone, const QString& tband, const QString& tmode){
//qDebug()  << "AwardWAZ::notConfirmedString" << endl;
	i = tzone;
	sAux = tband;
	j = adif.band2Int(sAux);
	if (j == -1)
		return;
	sAux = tmode;
	k = adif.mode2Int(sAux);
	if (k == -1)
		return;

	wazb[i][j] = 0;
	wazm[i][k] = 0;
	wazC[i] = false;
}
