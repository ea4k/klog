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
#include "src/dataproxy_sqlite.h"
#include "src/charts/statsgeneralchartwidget.h"
#include "src/charts/statsqsosperyearbarchartwidget.h"
#include "src/charts/statsqsosperbandbarchartwidget.h"
#include "src/charts/statsqsospermodebarchartwidget.h"
#include "src/charts/statsentitiesperyearbarchartwidget.h"
#include "src/charts/statscqzperyearbarchartwidget.h"
#include "src/charts/statsqsosperdxccbarchartwidget.h"
#include "src/charts/statsqsospercontinentbarchartwidget.h"
#include "src/charts/statsqsosperhourbarchartwidget.h"
#include "src/charts/statsqsospermonthbarchartwidget.h"
#include "src/charts/statsworkedconfirmedpiechartwidget.h"
#include "src/charts/statsworkedsentpiechartwidget.h"
#include "src/charts/statssentconfirmedpiechartwidget.h"
#include "src/charts/statsgridsonsatswidget.h"
#include "src/charts/statsdxccsonsatswidget.h"
#include "src/charts/statsfieldperbandwidget.h"
//#include "src/charts/statspiechartwidget.h"


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


    //QChart *chart;
    //QChartView *chartView;

    QWidget *mainWidget;
    StatsGeneralChartWidget *genchart;
    QVBoxLayout *mLayout;

    //QBarCategoryAxis *axis;

};

#endif // BARCHARTSTATS_H
