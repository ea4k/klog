/***************************************************************************
                          awardWAZ.h  -  description
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

#ifndef GLOBAL_DATA
#define GLOBAL_DATA
const int ENTITIES = 400;
const int ZONES = 40;
const int BANDS = 28;
const int MODES = 59;
#endif

 
#ifndef AWARDWAZ_H
#define AWARDWAZ_H
#include <QDebug>
//#include <klocale.h>
#include "adif.h"



//ENTITIES 340
// BANDS 17 {2/6/10/12/15/17/20/30/40/80/160}

/*
TODO: To isolate the numbers in the ADIF class in order to allow change independently
      The numbers/bands/...
      The master is in the ADIF class.
Please follow this table of equivalences:
{160/80/60/40/30/20/17/15/12/10/6/2}
BAND FREQS (MHz) GUI
160M 1,2  0
80M  3,4  1
60M  5  2
40M  7  3
30M  10  4
20M  14  5
17M  18  6
15M  21  7
12M  24  8
10M  28,29  9
6M   50...54  10
2M   144...148 11
125CM  219...225 12
70CM  420...450 13
33CM  902...928 14
23CM  1240...1300 15
13CM  2300...2450 16
*/

//MODES 11 {SSB/CW/FM/PSK31/SSTV/RTTY/TOR/PKT/
//          AM/ATV/PAC/CLO
//



class AwardWAZ  {

public:

  AwardWAZ();


  ~AwardWAZ();

//  void setPfx(QString &tpfx);
//  QString getPfx() const;
  void clear();
  void worked(const int tzone, const int tband, const int tmode);
  void workedString(const int tzone, const QString& tband, const QString& tmode);
  void notWorked(const int tzone, const int tband, const int tmode);
  void notWorkedString(const int tzone, const QString& tband, const QString& tmode);
  bool isWorked(const int tzone);
  bool isWorkedBand (const int tzone, const int tband);
  bool isWorkedMode (const int tzone, const int tmode);
  int howManyWorkedBand(const int tband);
  int howManyWorkedMode(const int tmode);
  int howManyWorked();


  void confirmed(const int tzone, const int band, const int tmode);
  bool isConfirmed(const int tzone);
  bool isConfirmedBand (const int tzone, const int tband);
  bool isConfirmedMode (const int tzone, const int tmode);
  void confirmedString(const int tzone, const QString& tband, const QString& tmode);
  int howManyConfirmedBand(const int tband);
  int howManyConfirmedMode(const int tmode);
  void notConfirmed(const int tzone, const int tband, const int tmode);
  void notConfirmedString(const int tzone, const QString& tband, const QString& tmode);  
  int howManyConfirmed();

private:
  Adif adif;
  int wazCountConfirmed; // How many WAZ confirmed
  int wazCountWorked; // How many WAZ worked

  int i, j, k; // just index
  int wazb[ZONES][BANDS]; // -1 Not worked, 0 = worked, 1 = confirmed
  int wazm[ZONES][MODES]; // -1 Not worked, 0 = worked, 1 = confirmed
  bool wazW[ZONES];
  bool wazC[ZONES];
  QString sAux;
  bool aux;
  int iAux;

};

#endif

