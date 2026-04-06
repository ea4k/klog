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

#include <QColor>
#include <QString>
#include "frequency.h"

// Default QSO status colors — single source of truth for the whole application.
// These match the values shown in Setup → Colors → Default palette.
inline const QColor KLOG_COLOR_NEW_ONE   { QColor("#FF0000") };  // Red      – All Time New One (ATNO)
inline const QColor KLOG_COLOR_NEEDED    { QColor("#FF8C00") };  // Orange   – Needed in band
inline const QColor KLOG_COLOR_WORKED    { QColor("#FFD700") };  // Gold     – Worked, not confirmed
inline const QColor KLOG_COLOR_CONFIRMED { QColor("#32CD32") };  // Lime     – Confirmed
inline const QColor KLOG_COLOR_DEFAULT   { QColor("#00BFFF") };  // Sky blue – Default / unknown

// Alpha value for locator overlays on the map: ~31% opaque (69% transparent).
inline constexpr int KLOG_LOCATOR_ALPHA = 100;

//using namespace std;

enum ExportMode {ModeLotW, ModeADIF, ModeClubLog, ModeEQSL, ModeQRZ};
enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ}; //, HamQTH, HRDLog
enum OnlineErrorCode {Ok, Fail};
enum OnlineErrorReason {Other, Auth, DupeQSO, WrongLogBook};
enum DebugLogLevel {
    None,       // No logging
    Fatal,      // Non recuprable error. App can't continue
    Error,      // Recuperable error, failed operation
    Warning,    // Not normal situation
    Info,       // Informational event (connection, disconnection, ...)
    Debug,      // Debug for diagnostic
    Devel       // Verbose entry (i.e. Start of each method)
};

enum ValidFieldsForStats {DXCC, GridSquare};
enum FilesToDownload {CTY, Sats};
enum QSOStatus {unknown, ATNO, needed, worked, confirmed};
enum WJTXStatus {NewContinent, NewContinentInBand, NewContinentInMode,
                 NewCQ, NewCQInBand, NewCQInMode,
                 NewITU, NewITUInBand, NewITUInMode,
                 NewDXCC, NewDXCCInBand, NewDXCCInMode,
                 NewGrid, NewGridInBand, NewGridInMode,
                 NewCall, NewCallInBand, NewCallInMode };

enum MouseClicks {NoClick, RightClick, SingleClick, DoubleClick};
//enum DataTableHash {WorldData, BandData, ModeData};                 // Used by World & DataProxy to select the table to build a Hash

//struct EntityData { // Used to pass a list of data from World to dxccstatuswidget
//  int dxcc;
//  QString mainprefix;
//  QString name;
//  QString isoname;
//};

struct EntityData { // Used to pass a list of data from World to dxccstatuswidget
    int dxcc            = -1;
    QString mainprefix  = QString();
    QString name        = QString();
    QString isoname     = QString();

    bool operator<(const EntityData &other) const {
        return std::tie(dxcc, mainprefix, name, isoname) < std::tie(other.dxcc, other.mainprefix, other.name, other.isoname);
    }

    bool operator==(const EntityData &other) const {
        return std::tie(dxcc, mainprefix, name, isoname) == std::tie(other.dxcc, other.mainprefix, other.name, other.isoname);
    }
};

struct EntityStatus { // Used to pass a list of data from Awards to dxccstatuswidget
    int dxcc            = -1;
    int bandId          = -1;
    int modeId          = -1;
    QSOStatus status    = unknown;   // status of this Entity in this band & mode
    int qsoId           = -1;   // QSOid that provides this status
    int logId           = -1;   // Log where we are checking the status (TODO: This may be redundant as the qsoId may be used to get the log)
};

struct Coordinate {
    double lat      = 0.0;
    double lon      = 0.0;
};

struct PrimarySubdivision { // Used to return data to MainWindow for each prefix
    QString name;
    QString shortName;
    QString prefix;
    int cqz             = -1;
    int ituz            = -1;
    int dxcc            = -1;
};

struct ADIFField {
    QString field;
    QString value;
    bool valid      = false;
    // qChar type;
};

struct RadioStatus {
    Frequency freq_VFO_TX;
    Frequency freq_VFO_RX;
    bool split              = false;
    QString mode_VFO_TX;
    QString mode_VFO_RX;
    bool memoryMode         = false;
    int memoryChannel       = -1;
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
