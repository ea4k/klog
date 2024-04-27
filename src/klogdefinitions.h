#ifndef KLOG_KLOGDEFINITIONS_H
#define KLOG_KLOGDEFINITIONS_H
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

//using namespace std;

enum ExportMode {ModeLotW, ModeADIF, ModeClubLog, ModeEQSL, ModeQRZ};
enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ}; //, HamQTH, HRDLog
enum OnlineErrorCode {Ok, Fail};
enum OnlineErrorReason {Other, Auth, DupeQSO, WrongLogBook};
enum DebugLogLevel {None, Info, Debug, Devel};
enum ValidFieldsForStats {DXCC, GridSquare};
enum FilesToDownload {CTY, Sats};
enum QSOStatus {unknown, ATNO, needed, worked, confirmed};
enum FreqUnits {Hz, KHz, MHz, GHz, THz};
enum MouseClicks {NoClick, RightClick, SingleClick, DoubleClick};

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

struct Coordinate {
    double lat;
    double lon;
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

#endif // KLOGDEFINITIONS_H
