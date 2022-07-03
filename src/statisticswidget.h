#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H
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
#include "dataproxy_sqlite.h"
#include "charts/barchartstats.h"

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


     DataProxy_SQLite *dataProxy;
     //QWidget *graphWidget;

     //QChartView *chartView;

     BarChartStats *barChartStats;
     //DonutChartStats *donutChartStats;

     QComboBox *statisticToShowComboBox, *logComboBox;
     QStringList statisticsToShowList;
};






#endif // STATISTICSWIDGET_H
