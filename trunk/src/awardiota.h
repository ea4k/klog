/***************************************************************************
                          awardiota.h  -  description
                             -------------------
    begin                : mi jul 2 2003
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
#ifndef GLOBAL_DATA
#define GLOBAL_DATA
const int ENTITIES = 400;
const int ZONES = 40+1;
const int BANDS = 28;
const int MODES = 59;
#endif
#ifndef AWARDIOTA_H
#define AWARDIOTA_H
//#include <klocale.h>
#include <qmap.h>
#include "adif.h"

typedef bool iotaStatus[BANDS][MODES]; //0 = worked, 1 = confirmed
	//static iotaStatus thisIOTAStatus;

class AwardIOTA  {
/*************************************************************************************
** An IOTA reference is always a continent + reference:                             **
** EU-001, NA-034, AS-123                                                           **
** Continents are AF, AN, AS, EU, NA, OC or SA.                                     **
**                                                                                  **
**************************************************************************************/

public:

  AwardIOTA();


  ~AwardIOTA();

  void clear();
  void worked(const QString& tiota, const int tband, const int tmode);
  void workedString(const QString& tiota, const QString& tband, const QString& tmode);
  void notWorked(const QString& tiota, const int tband, const int tmode);
  void notWorkedString(const QString& tiota, const QString& tband, const QString& tmode);
  bool isWorked(const QString& tiota);
  bool isWorkedBand (const QString& tiota, const int tband);
  bool isWorkedMode (const QString& tiota, const int tmode);
  int howManyWorkedBand(const int tband);
  int howManyWorkedMode(const int tmode);
  int howManyWorked();


  void confirmed(const QString& tiota, const int band, const int tmode);
  bool isConfirmed(const QString& tiota);
  bool isConfirmedBand (const QString& tiota, const int tband);
  bool isConfirmedMode (const QString& tiota, const int tmode);
  void confirmedString(const QString& tiota, const QString& tband, const QString& tmode);
  int howManyConfirmedBand(const int tband);
  int howManyConfirmedMode(const int tmode);
  void notConfirmed(const QString& tiota, const int tband, const int tmode);
  void notConfirmedString(const QString& tiota, const QString& tband, const QString& tmode);  
  int howManyConfirmed();

private:
	Adif adif;
	int iotaCountConfirmed; // How many ZONES confirmed
	int iotaCountWorked; // How many ZONES worked
	QString sAux;
	bool aux;
	int iAux;
	iotaStatus thisIOTAStatus;

  int i, j, k; // just index
/*  int iotab[ZONES][BANDS]; // -1 Not worked, 0 = worked, 1 = confirmed
  int iotam[ZONES][MODES]; // -1 Not worked, 0 = worked, 1 = confirmed
*/
/*bool iotaW[ZONES];
  bool iotaC[ZONES];
*/  

	
	typedef QMap<QString, iotaStatus> IOTAMap;  // To map IOTA/Status
	IOTAMap iotaMap; 
	IOTAMap::Iterator it;
  

};

#endif


