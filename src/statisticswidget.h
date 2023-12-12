#ifndef KLOG_STATISTICSWIDGET_H
#define KLOG_STATISTICSWIDGET_H
/***************************************************************************
                          statisticswidget.h  -  description
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
#include <QWidget>
#include <QtWidgets>
#include <QStackedWidget>
#include "dataproxy_sqlite.h"
#include "charts/statsgeneralchartwidget.h"
#include "charts/statsqsosperyearbarchartwidget.h"
#include "charts/statsqsosperbandbarchartwidget.h"
#include "charts/statsqsospermodebarchartwidget.h"
#include "charts/statsentitiesperyearbarchartwidget.h"
#include "charts/statscqzperyearbarchartwidget.h"
#include "charts/statsqsosperdxccbarchartwidget.h"
#include "charts/statsqsospercontinentbarchartwidget.h"
#include "charts/statsqsosperhourbarchartwidget.h"
#include "charts/statsqsospermonthbarchartwidget.h"
#include "charts/statsworkedconfirmedpiechartwidget.h"
#include "charts/statsworkedsentpiechartwidget.h"
#include "charts/statssentconfirmedpiechartwidget.h"
#include "charts/statsgridsonsatswidget.h"
#include "charts/statsdxccsonsatswidget.h"
#include "charts/statsfieldperbandwidget.h"

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~StatisticsWidget();
    void clear();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void slotChartComboBoxChanged();
    void slotLogComboBoxChanged();

private:
    void createUI();
    void fillLogCombo();
    void updateChart();
   // void paintQSOperOtherBarChart(const int _i);

    QStackedWidget *stackedWidget;
    StatsQSOsPerYearBarChartWidget *w1;
    StatsEntitiesPerYearBarChartWidget *w2;
    StatsCQZPerYearBarChartWidget *w3;
    StatsQSOsPerBandBarChartWidget *w4;
    StatsQSOsPerModeBarChartWidget *w5;
    StatsQSOsPerDXCCBarChartWidget *w6;
    StatsQSOsPerContinentBarChartWidget *w7;
    StatsQSOsPerHourBarChartWidget *w8;
    StatsQSOsPerMonthBarChartWidget *w9;
    StatsWorkedConfirmedPieChartWidget *w10;
    StatsWorkedSentPieChartWidget *w11;
    StatsSentConfirmedPieChartWidget *w12;
    StatsGridsOnSatsWidget *w13;
    StatsDXCCOnSatsWidget *w14;
    StatsFieldPerBandWidget *w15;
    StatsFieldPerBandWidget *w16;



     DataProxy_SQLite *dataProxy;


     QComboBox *statisticToShowComboBox, *logComboBox;
     QStringList statisticsToShowList;
};






#endif // STATISTICSWIDGET_H
