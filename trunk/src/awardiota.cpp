/***************************************************************************
                          awardiota.cpp  -  description
                             -------------------
    begin                : mié jul 2 2003
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
#include "awardiota.h"

AwardIOTA::AwardIOTA(){
  i = 0;
  j = 0;
  k = 0;
  for (i = 0; i <  MODES; i++){     
    for (j = 0; j <  BANDS; j++){
	thisIOTAStatus[i][j]=false;
    }
  }

}


AwardIOTA::~AwardIOTA(){}

void AwardIOTA::clear(){
  i = 0;
  j = 0;
  k = 0;
  for (i = 0; i <  MODES; i++){     
    for (j = 0; j <  BANDS; j++){
	thisIOTAStatus[i][j] = false;
    }
  }

}

void AwardIOTA::worked(const QString& tiota, const int tband, const int tmode){
/*  i = tzone;
  j = tband;
  k = tmode;
  iotab[i][j] = 0;
  iotam[i][k] = 0;
  iotaW[i] = true;
*/
//I have commented the folloing 3 lines just for test!
    thisIOTAStatus[j][k] = false;
    sAux = tiota;
//    AwardIOTA::iotaMap[sAux] = iotaStatus;
}

void AwardIOTA::workedString(const QString& tiota, const QString& tband, const QString& tmode){
/*  i = tzone;
  sAux = tband;
  j = adif.band2Int(sAux);
  if (j == -1)
    return;
  sAux = tmode;
  k = adif.mode2Int(sAux);
  if (k == -1)
    return;

  iotab[i][j] = 0;
  iotam[i][k] = 0;
  iotaW[i] = true;
  */
//  iotaCountWorked++;
//AwardIOTA::iotaMap[IOTA] = cqzone;
}

void AwardIOTA::notWorked(const QString& tiota, const int tband, const int tmode){
  //Needs to have a way to check if it
/*  i = tzone;
  j = tband;
  k = tmode;
  iotab[i][j] = -1;
  iotam[i][k] = -1;
  for (j = 0; j < BANDS; j++)
    if (iotab[i][j] >=0)
      return;
  for (k = 0; k < MODES; k++)
    if (iotam[i][k] >=0)
      return;
  iotaW[i] = false;
*/
}

void AwardIOTA::notWorkedString(const QString& tiota, const QString& tband, const QString& tmode){
/*  i = tzone;
  sAux = tband;
  j = adif.band2Int(sAux);
  if (j == -1)
    return;
  sAux = tmode;
  k = adif.mode2Int(sAux);
  if (k == -1)
    return;
  sAux = tband;
  j = adif.band2Int(sAux);
  if (j == -1)
    return;
  sAux = tmode;
  k = adif.mode2Int(sAux);
  if (k == -1)
    return;

  iotab[i][j] = -1;
  iotam[i][k] = -1;
  for (j = 0; j < BANDS; j++)
    if (iotab[i][j] >=0)
      return;
  for (k = 0; k < MODES; k++)
    if (iotam[i][k] >=0)
      return;
  iotaW[i] = false;
*/

}




bool AwardIOTA::isWorked(const QString& tiota){
    /*
  i = tzone;
  return iotaW[i];
  */
    return true;
}

bool AwardIOTA::isWorkedBand (const QString& tiota, const int tband){
    /*
  i = tzone;
  j = tband;
  if (!iotaW[i])
    return false;
  for (i = 0; i <  ZONES; i++){
    for (j = 0; j <  BANDS; j++){
     if (iotab[i][j] >= 0 )
        return true;
    }
  }*/
  return false;

}

bool AwardIOTA::isWorkedMode (const QString& tiota, const int tmode){
    /*
  i = tzone;
  k = tmode;
  if (!iotaW[i])
    return false;
  for (i = 0; i <  ZONES; i++){
    for (k = 0; k <  MODES; k++){
     if (iotam[i][k] >= 0 )
        return true;
    }
  }*/
  return false;

}

int AwardIOTA::howManyWorkedBand(const int tband){
    /*
  iAux = 0;
  j = tband;
  for (i = 0; i < ZONES; i++){
    for (j = 0; j < BANDS; j++)
     if (iotab[i][j] >= 0 )
        iAux++;
  }
  return iAux;
  */
    return -1;
}

int AwardIOTA::howManyWorkedMode(const int tmode){
    /*
  iAux = 0;
  k = tmode;
  for (i = 0; i < ZONES; i++){
    for (k = 0; k < MODES; k++)
     if (iotam[i][k] >= 0 )
        iAux++;
  }
  return iAux;
*/
    return -1;
}

int AwardIOTA::howManyWorked(){
/*
  iAux = 0;
  for (i = 0; i < ZONES; i++){
     if (iotaW[i])
        iAux++;
  }
  return iAux;
//  return  iotaCountWorked;
  */
    return 0;
}


//================================
// CONFIRMED

void AwardIOTA::confirmed(const QString& tiota, const int tband, const int tmode){
    /*
  i = tzone;
  j = tband;
  k = tmode;
  iotab[i][j] = 1;
  iotam[i][k] = 1;
  iotaC[i] = true;
  */
}

void AwardIOTA::confirmedString(const QString& tiota, const QString& tband, const QString& tmode){
 /* i = tzone;

  sAux = tband;
  j = adif.band2Int(sAux);
  if (j == -1)
    return;
  sAux = tmode;
  k = adif.mode2Int(sAux);
  if (k == -1)
    return;
  iotab[i][j] = 1;
  iotam[i][k] = 1;
  iotaC[i] = true;
*/
}


bool AwardIOTA::isConfirmed(const QString& tiota){
 /* i = tzone;
  return iotaC[i];
  */
    return false;
}

bool AwardIOTA::isConfirmedBand (const QString& tiota, const int tband){
    /*
  i = tzone;
  j = tband;
  if (!iotaC[i])
    return false;
  for (i = 0; i <  ZONES; i++){
    for (j = 0; j <  BANDS; j++){
     if (iotab[i][j] == 1 )
        return true;
    }
  }
  */
  return false;

}

bool AwardIOTA::isConfirmedMode (const QString& tiota, const int tmode){
 /* i = tzone;
  k = tmode;
  if (!iotaC[i])
    return false;
  for (i = 0; i < ZONES; i++){
    for (k = 0; k <  MODES; k++){
     if (iotam[i][k] == 1 )
        return true;
    }
  }*/
  return false;

}

int AwardIOTA::howManyConfirmedBand(const int tband){
 /* iAux = 0;
  j = tband;
  for (i = 0; i < ZONES; i++){
    for (j = 0; j < BANDS; j++)
     if (iotab[i][j] == 1 )
        iAux++;
  }
  return iAux;
  */
  return -1;
}

int AwardIOTA::howManyConfirmedMode(const int tmode){
 /* iAux = 0;
  k = tmode;
  for (i = 0; i < ZONES; i++){
    for (k = 0; k < MODES; k++)
     if (iotam[i][k] == 1 )
        iAux++;
  }
  return iAux;
*/
    return -1;
}

int AwardIOTA::howManyConfirmed(){
/*
  iAux = 0;
  for (i = 0; i < ZONES; i++){
     if (iotaC[i])
        iAux++;
  }
  return iAux;
*/
    return -1;
}


 void AwardIOTA::notConfirmed(const QString& tiota, const int tband, const int tmode){
  //Needs to have a way to check if it
 /*
 i = tzone;
  j = tband;
  k = tmode;
  iotab[i][j] = 0;
  iotam[i][k] = 0;
  for (j = 0; j < BANDS; j++)
    if (iotab[i][j] == 1)
      return;
  for (k = 0; k < MODES; k++)
    if (iotam[i][k] == 1)
      return;
  iotaC[i] = false;
*/

}

void AwardIOTA::notConfirmedString(const QString& tiota, const QString& tband, const QString& tmode){
/*  i = tzone;

  sAux = tband;
  j = adif.band2Int(sAux);
  if (j == -1)
    return;
  sAux = tmode;
  k = adif.mode2Int(sAux);
  if (k == -1)
    return;

  iotab[i][j] = 0;
  iotam[i][k] = 0;
  for (j = 0; j < BANDS; j++)
    if (iotab[i][j] ==1)
      return;
  for (k = 0; k < MODES; k++)
    if (iotam[i][k] ==1)
      return;
  iotaC[i] = false;
*/}

