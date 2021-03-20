#ifndef STATSDXCCSONSATSWIDGET_H
#define STATSDXCCSONSATSWIDGET_H
/***************************************************************************
                          statsdxccsonsatswidget.h  -  description
                             -------------------
    begin                : dec 2020
    copyright            : (C) 2020 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QtCharts>
#include <QtDebug>
#include <QTableWidget>
#include "dataproxy_sqlite.h"
#include "qso.h"
#include "utilities.h"
#include "charts/statsgeneralchartwidget.h"

class StatsDXCCOnSatsWidget : public StatsGeneralChartWidget
{
    Q_OBJECT
public:
    StatsDXCCOnSatsWidget();
    StatsDXCCOnSatsWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    void prepareChart(const int _log=-1) override;


signals:

private slots:
    void slotConfirmedClicked();

private:
    void createUI() override;
    DataProxy_SQLite *dataProxy;
    QTableWidget *tableWidget;
    Utilities *util;
    QCheckBox *confirmedOnlyCheckBox, *onlyLEOSatCheckBox;
    QLabel *numberLabel;
    int log;

};
#endif // STATSGRIDSONSATSWIDGET_H
