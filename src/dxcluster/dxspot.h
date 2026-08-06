#ifndef KLOG_CLUSTER_DXSPOT_H
#define KLOG_CLUSTER_DXSPOT_H
/***************************************************************************
                          dxspot.h  -  description
                             -------------------
    begin                : Apr 2024
    copyright            : (C) 2024 by Jaime Robles
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
/*
    This class implements DXSpot.
*/
#include <QObject>
#include <QDateTime>
#include <QtGui/qcolor.h>
#include "../klogdefinitions.h"
#include "../frequency.h"

class DXSpot
{
    friend class tst_DXSpot;

public:
    DXSpot();
    //DXSpot(DXSpot *_other);
    DXSpot(const DXSpot& other);

    ~DXSpot();
    void operator=(DXSpot const &_other);
    void clear();

    bool isValid ();

    void setDXCall(const QString &c);
    QString getDxCall ();

    void setSpotter(const QString &c);
    QString getSpotter();

    void setComment(const QString &c);
    QString getComment();

    void setDateTime(const QDateTime &d);
    QDateTime getDateTime();

    void setClickStatus(const MouseClicks &_s );
    MouseClicks getClickStatus();

    void setFrequency(Frequency f);
    Frequency getFrequency();

    void setMode(const QString &m);
    QString getMode();

    void setQSOStatus(const QSOStatus &s);
    QSOStatus getQSOStatus();

    void setColor(const QColor &c);
    QColor getColor();


    void setSHDX(bool _shdx);   // To know if the spot arrived from the dxcluster or was requested with a sh/dx
    bool getSHDX();

    // --- DX Assistant fields ---
    void setDXCC(int _dxcc);
    int getDXCC();

    void setBandId(int _bandId);
    int getBandId();

    void setSpotterContinent(const QString &_c);
    QString getSpotterContinent();

    void setSpotterDXCC(int _dxcc);
    int getSpotterDXCC();

    void setScore(int _score);
    int getScore();

    void setStatusBand(const QSOStatus &_s);
    QSOStatus getStatusBand();

    void setMostWantedRank(int _rank);
    int getMostWantedRank();

    void setHidden(bool _h);
    bool getHidden();

    void setSource(SpotSource _source);
    SpotSource getSource();

signals:

private:
    QString     dxcall;
    Frequency   freq;
    QString     mode;
    QSOStatus   status;
    QColor      color;
    QString     spotter;
    QString     comment;
    QDateTime   dateTime;
    bool        shdx;
    MouseClicks clickStatus;

    // --- DX Assistant fields ---
    // All default to safe values; ignored by any code that does not use them.

    // Resolved at receive time (avoids re-parsing callsign+freq on every rescore)
    int         dxcc             = -1;
    int         bandId           = -1;
    QString     spotterContinent;        // Short name: "EU", "NA", ... empty if unknown
    int         spotterDXCC      = -1;   // ARRL id of the spotter; -1 if unknown

    // Scoring (populated by DXAssistantEngine; -1 = not yet scored / discard)
    int         score            = -1;
    QSOStatus   statusBand       = unknown;  // Status on this band, mode-agnostic
    int         mostWantedRank   = 0;        // 0 = not in top-N most-wanted list

    // Where the spot came from: the DXCluster, WSJT-X, ...
    SpotSource  source           = SpotSourceUnknown;

    // UI state (used by DXClusterAssistant only)
    bool        hidden           = false;    // Per-session; never persisted to disk
};

#endif // KLOG_CLUSTER_DXSPOT_H
