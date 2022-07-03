#ifndef BARCHARTSTATS_H
#define BARCHARTSTATS_H
/***************************************************************************
                          barchartstats.h  -  description
                             -------------------
    begin                : ago 2018
    copyright            : (C) 2018 by Jaime Robles
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
#include <QObject>
#include <QWidget>
#include <QtCharts>
#include "../dataproxy_sqlite.h"
#include "statsgeneralchartwidget.h"
#include "statsqsosperyearbarchartwidget.h"
#include "statsqsosperbandbarchartwidget.h"
#include "statsqsospermodebarchartwidget.h"
#include "statsentitiesperyearbarchartwidget.h"
#include "statscqzperyearbarchartwidget.h"
#include "statsqsosperdxccbarchartwidget.h"
#include "statsqsospercontinentbarchartwidget.h"
#include "statsqsosperhourbarchartwidget.h"
#include "statsqsospermonthbarchartwidget.h"
#include "statsworkedconfirmedpiechartwidget.h"
#include "statsworkedsentpiechartwidget.h"
#include "statssentconfirmedpiechartwidget.h"
#include "statsgridsonsatswidget.h"
#include "statsdxccsonsatswidget.h"
#include "statsfieldperbandwidget.h"

class BarChartStats : public QWidget
{
    Q_OBJECT
public:
    BarChartStats(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~BarChartStats();
    void prepareChart(const int _selection, const int _log=-1);
    void clear();

signals:

public slots:

private:
    void createUI();
    void cleanLayout();
    DataProxy_SQLite *dataProxy;

    QWidget *mainWidget;
    StatsGeneralChartWidget *genchart;
    QVBoxLayout *mLayout;
};

#endif // BARCHARTSTATS_H
