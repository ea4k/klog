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
#include "../klogdefinitions.h"
#include "../frequency.h"

class DXSpot : public QObject
{
    Q_OBJECT
    friend class tst_DXSpot;

public:
    DXSpot();
    //DXSpot(DXSpot *_other);
    DXSpot(const DXSpot& other) : dxcall(other.dxcall), freq(other.freq), spotter(other.spotter), comment(other.comment), dateTime(other.dateTime), clickStatus(other.clickStatus), valid(other.valid) {};

    ~DXSpot();
    void operator=(DXSpot const &_other);
    void clear();

    void setValid(const bool _v);
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

signals:

private:
    QString dxcall;
    Frequency freq;
    QString spotter;
    QString comment;
    QDateTime dateTime;
    MouseClicks clickStatus;
    bool valid;
};

#endif // KLOG_CLUSTER_DXSPOT_H
