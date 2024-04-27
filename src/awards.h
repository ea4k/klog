#ifndef KLOG_AWARDS_H
#define KLOG_AWARDS_H
/***************************************************************************
                          awards.h  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
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

//#include <QObject> // For the "tr" function

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QColor>
#include <QStringList>
#include <QProgressDialog>
#include <QMultiHash>
#include <QObject>
#include <QtGlobal>

#include "world.h"
#include "awarddxmarathon.h"
#include "dataproxy_sqlite.h"
#include "utilities.h"
#include "klogdefinitions.h"

class QProgressDialog;

struct EntityStatus { // Used to pass the Data of the status of an Entity
    int entityId = -1;
    int bandId = -1;
    int modeId = -1;
    int log = -1;
};

class Awards : public QObject {
     Q_OBJECT
    //friend class World;

public:
    Awards(DataProxy_SQLite *dp, const QString &_parentFunction);
    ~Awards();
    void setAwards(const int _qsoId);
    void setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId);
    /*
    _workedOrConfirmed = 0     Set as Worked
    _workedOrConfirmed = 1     Set as Confirmed
    */
    void setManageModes(const bool _manageModes=false);   // Defines if the mode is to be taken into account when calculating an award

    void recalculateAwards();
    //void recalculateDXCC(const int _logNumber);
    //void setAwards(const int _qsoId, bool modify);
    int getDXCCWorked(const int _logNumber);
    int getDXCCConfirmed(const int _logNumber);
    int getWAZWorked(const int _logNumber);
    int getWAZConfirmed(const int _logNumber);
    bool updateDXCCBandsStatus(const int _logNumber=0);


    int getQSOsInLog(const int _logNumber);

    bool getIsDXCCConfirmed(const int _dxcc, const int _logNumber);
    bool isThisSpotConfirmed(EntityStatus _entityStatus);

    QString checkIfValidIOTA(const QString &_tiota); //TODO: There is an equivalent function in the MainWindowInputOthers class. I should use only one!

    int getEntityDXStatus (const int _enti, const int _bandid, int _modeid = -1, int _log = -1);
    QString getQSOofAward (const int _enti, const int _bandid, const int _log, const bool _confirmed);
    int getQSOIdofAward (const int _enti, const int _bandid, const int _log, const bool _confirmed);

    int getDXStatus (EntityStatus _entitystatus);
    QSOStatus getQSOStatus(const int &_status); // Needs to be called with the output of getDXStatus)
    QString getDXStatusString (const int &_status); // Needs to be called with the output of getDXStatus
    QString getDXCCStatusBand2(const int _dxcc, const int _band, const int _logNumber=0); // Returns -, W or C (Not worked, worked, Confirmed)
    QString getDXCCStatusBand(const int _dxcc, const int _band); // Returns -, W or C (Not worked, worked, Confirmed)

    void setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default);
    QColor getQRZDXStatusColor(EntityStatus _entitystatus); // Receives Entity, band, mode & log
    QColor getDefaultColor();

    int getDXMarathonQSO(const int _year, const int _logNumber);
    int getDXMarathonDXCC(const int _year, const int _logNumber);
    int getDXMarathonCQ(const int _year, const int _logNumber);
    int getDXMarathonScore(const int _year, const int _logNumber);
    bool isDXMarathonNeed(const int _dxcc, const int _cq, const int _year, const int _logNumber);

    // Receives:  QStringList _qs;
    //_qs << QRZ << BandId << lognumber;
    /*
    0 - New one.
    1 - Worked but not confirmed: New one in this band.
    2 - Worked but not confirmed: Worked in this band.
    3 - Confirmed: New one in this band.
    4 - Confirmed: Worked in this band.
    5 - Confirmed: Confirmed in this band.
    6 - Default: Not applicable
    */

private:
    void setAwardDXCC(const int _qsoId);
    //bool setAwardDXCC(const int _dxcc, const int _band, const int _mode, const QString &_workedOrConfirmed, const int _logNumber, const int _qsoId);
    int setAwardDXCCst(const int _dxcc, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId);

    int setAwardDXCCConfirmed(const int _band, const int _mode, const int _dxcc, const int _newQSOid); // Changes the status of a DXCC from worked to confirmed
    int setDXCCToQSO(const int _dxcc, const int _qsoid); // Defines the DXCC in a QSO
    int setCQToQSO(const int _cqz, const int _qsoid); // Defines the CQ zone in a QSO

    int dxccStatusBandMode(const int _ent, const int _band, const int _mode, const int _logNumber, bool _checkingMode); //-1 error / 0 Not worked / 1 worked / 2 confirmed
    int dxccStatus(const int _ent, const int _logNumber); //-1 error / 0 Not worked / 1 worked / 2 confirmed

    /*
    _confirmed = 0     Set as Worked
    _cConfirmed = 1     Set as Confirmed
    */


    /**/
    int dxccStatusBand(const int _ent, const int band, const int _logNumber); //-1 error / 0 Not worked / 1 worked / 2 confirmed
    int dxccStatusMode(const int _ent, const int band, const int _logNumber); //-1 error / 0 Not worked / 1 worked / 2 confirmed
    /**/

    void setAwardWAZ(const int _qsoId);
    //bool setAwardWAZ(const int _cqz, const int _band, const int _mode, const QString &_workedOrConfirmed, const int _logNumber, const int _qsoId);
    int setAwardWAZst(const int _cqz, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId);
    /*
    _confirmed = 0     Set as Worked
    _confirmed = 1     Set as Confirmed
    */
    //int getProgresStepForDialog(int totalSteps);

    QColor newOneColor;     //
    QColor neededColor;     //
    QColor workedColor;     //
    QColor confirmedColor;  //
    QColor defaultColor;

    World *world;
    DataProxy_SQLite *dataProxy;
    DXMarathon *dxMarathon;
    Utilities *util;

    typedef QMultiHash<int, int> DXStatus;

    DXStatus dxccWorked, dxccConfirmed, wazWorked, wazConfirmed;
    bool manageModes;

    QList<EntityBandStatus> dxccStatusList;

signals:
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution
    void awardDXCCUpdated();
};
#endif // AWARDS_H
