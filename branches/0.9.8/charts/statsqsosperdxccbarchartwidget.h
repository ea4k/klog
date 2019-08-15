#ifndef STATSQSOSPERDXCCBARCHARTWIDGET_H
#define STATSQSOSPERDXCCBARCHARTWIDGET_H
/***************************************************************************
                          statsqsosperdxccbarchatwidget.h  -  description
                             -------------------
    begin                : nov 2018
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QtCharts>
#include <QtDebug>
#include "dataproxy.h"
#include "charts/statsgeneralchartwidget.h"



class StatsQSOsPerDXCCBarChartWidget : public StatsGeneralChartWidget
{
    Q_OBJECT
public:
    StatsQSOsPerDXCCBarChartWidget(DataProxy *dp, QWidget *parent = 0);
    StatsQSOsPerDXCCBarChartWidget();
    void prepareChart();

signals:

public slots:

private:
    void createUI();
    DataProxy *dataProxy;

    QChart *chart;
    QChartView *chartView;

};

#endif // QSOSPERBANDBARCHARTWIDGET_H
