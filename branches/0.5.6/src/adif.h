/**************************************************************************
                          adif.h  -  description
                             -------------------
    begin                : vie mar 7 2003
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
#ifndef ADIF_H
#define ADIF_H

#include <QString>

 class Adif{
/*
This class is really not independant FROM the GUI as the numbers to codify the
MODE and BANDS depend on the position of the band in the COMBO-box.
The BAND is being changed... the mode should be changed!

Please follow this table of equivalences:

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
// Freqs are in MHz with a "decimal ." (optional)
// CONTINENTS AF=0, AN=1, AS=2, EU=3, NA=4, OC=5, SA=6
public:

    Adif();
    ~Adif();

    int band2Int(const QString& tband);
    int mode2Int(const QString& tmode);
    int freq2Int (const QString& tfreq); //(in MHz)   // accepts a string like "144", "10"... the MegaHertz section of a frecuency
    QString getModeBandPlan(const QString& tfreq); //(in MHz)
    QString int2Band(int tband);
    QString int2Freq(int tband);
    QString freq2Band (const QString& tfreq); // (in MHz) Accepts frecs and returns band
    QString freqCorrection (const double& tfreq); // (in MHz) Accepts frecs and returns freq in float
    bool isHF(const QString& tfreq);   // (in MHz) Input is the same as in freq2Int
    bool isVHF(const QString& tfreq);  // (in MHz) Input is the same as in freq2Int
    bool isWARC(const QString& tfreq);  // (in MHz) Input is the same as in freq2Int
    bool isCW(const QString& tfreq);  // Input is the same as in freq2Int
    bool isSSB(const QString& tfreq);  // Input is the same as in freq2Int
  // The next functions are just to adapt the specific KLog GUI to this general ADIF class

    int continent2Number(const QString& tcont);
    QString number2Continent(int tcont);
    int entity2AdifNumber(const QString& tPref); // returns the ADIF number of an entity

    QString KHz2MHz(const QString& tfreq);  // Receives a Freq in KHz and returns the same freq but in MHz.
    bool ifValidBand(const QString& tband); // Checks if the QString is a valid string band

private:

    QString sAux;
    int iInt;
    float freq;

};

#endif

