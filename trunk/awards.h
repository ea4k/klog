#ifndef AWARDS_H
#define AWARDS_H
/***************************************************************************
                          awards.h  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
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

#include "world.h"
#include "awarddxmarathon.h"
#include "dataproxy.h"
#include "dataproxy_sqlite.h"

class QProgressDialog;

class Awards
{
     //Q_OBJECT
    //friend class World;

public:
    Awards();
    ~Awards();
    void setAwards(const int _qsoId);
    void setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId);
    /*
    _workedOrConfirmed = 0     Set as Worked
    _workedOrConfirmed = 1     Set as Confirmed
    */


    void recalculateAwards();
    //void setAwards(const int _qsoId, bool modify);
    int getDXCCWorked(const int _logNumber);
    int getDXCCConfirmed(const int _logNumber);
    int getWAZWorked(const int _logNumber);
    int getWAZConfirmed(const int _logNumber);

    int getQSOsInLog(const int _logNumber);

    bool getIsDXCCConfirmed(const int _dxcc, const int _logNumber);

    QString checkIfValidIOTA(const QString _tiota);

    int getDXStatus (const QStringList _qs);
    QString getDXStatusString (const int _status); // Needs to be called with the output of getDXStatus

    void setColors (const QString _newOne, const QString _needed, const QString _worked, const QString _confirmed, const QString _default);
    QColor getQRZDXStatusColor(const QStringList _qs);
    QColor getDefaultColor();

    int getDXMarathonDXCC(const int _year, const int _logNumber);
    int getDXMarathonCQ(const int _year, const int _logNumber);
    int getDXMarathonScore(const int _year, const int _logNumber);

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
    bool setAwardDXCC(const int _dxcc, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId);
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

    void setAwardWAZ(const int _qsoId);
    bool setAwardWAZ(const int _cqz, const int _band, const int _mode, const int _workedOrConfirmed, const int _logNumber, const int _qsoId);
    int setAwardWAZst(const int _cqz, const int _band, const int _mode, const bool _confirmed, const int _logNumber, const int _qsoId);
    /*
    _confirmed = 0     Set as Worked
    _confirmed = 1     Set as Confirmed
    */
    int getProgresStepForDialog(int totalSteps);

    QColor newOneColor;     //
    QColor neededColor;     //
    QColor workedColor;     //
    QColor confirmedColor;  //
    QColor defaultColor;

    World *world;
    DataProxy *dataProxy;
    DXMarathon *dxMarathon;


    typedef QMultiHash<int, int> DXStatus;

    DXStatus dxccWorked, dxccConfirmed, wazWorked, wazConfirmed;


};
#endif // AWARDS_H
