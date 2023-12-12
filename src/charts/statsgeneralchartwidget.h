#ifndef KLOG_CHARTS_STATSGENERALCHARTWIDGET_H
#define KLOG_CHARTS_STATSGENERALCHARTWIDGET_H
/***************************************************************************
                          statsgeneralchartwidget.h  -  description
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
#include <QWidget>
#include <QtWidgets>
#include <QtDebug>
#include "../dataproxy_sqlite.h"

class StatsGeneralChartWidget : public QWidget
{
        Q_OBJECT
public:
    StatsGeneralChartWidget();
    StatsGeneralChartWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    virtual void prepareChart(const int _log=-1);

private:
    virtual void createUI();

};

#endif // GENERALCHARTWIDGET_H
