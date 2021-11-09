#ifndef KLOGDEFINITIONS_H
#define KLOGDEFINITIONS_H
/***************************************************************************
                          klogdefinitions.h  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include <QString>

using namespace std;

enum ExportMode {ModeLotW, ModeADIF, ModeClubLog, ModeEQSL, ModeQRZ, ModeWSJTX};
enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ}; //, HamQTH, HRDLog
enum OnlineErrorCode {Ok, Fail};
enum OnlineErrorReason {Other, Auth, DupeQSO, WrongLogBook};
enum DebugLogLevel {Info, Debug};
enum ValidFieldsForStats {DXCC, GridSquare};
enum AntPaths {G, O, S, L}

struct EntityData { // Used to pass a list of data from World to dxccstatuswidget
  int dxcc;
  QString mainprefix;
  QString name;
  QString isoname;
};

struct EntityBandStatus { // Used to pass a list of data from Awards to dxccstatuswidget
  int dxcc;
  int bandid;
  bool confirmed;
};

enum
{
    CQZones = 40,
    ITUZones = 90,
    DXCCEntities = 521 // http://www.adif.org/adif302.htm#Country%20Codes
};

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 2))
#define QT_SKIP Qt::SkipEmptyParts
#define QT_ENDL Qt::endl
#define QT_RETURNBYVALUE Qt::ReturnByValue
#else
#define QT_SKIP QString::SkipEmptyParts
#define QT_ENDL endl
#define QT_RETURNBYVALUE
 #endif


//#if QT_VERSION>=0x041502
//#else
//#endif

#endif // KLOGDEFINITIONS_H
