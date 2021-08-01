/***************************************************************************
                          statsworkedconfirmedpiechatwidget.cpp  -  description
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

#include "src/charts/statsworkedconfirmedpiechartwidget.h"

StatsWorkedConfirmedPieChartWidget::StatsWorkedConfirmedPieChartWidget(){}

StatsWorkedConfirmedPieChartWidget::StatsWorkedConfirmedPieChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{

#ifdef QT_DEBUG
      //qDebug() << "StatsWorkedConfirmedPieChartWidget::StatsWorkedConfirmedPieChartWidget" << Qt::endl;
#else
#endif


    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    //prepareChart();
}

void StatsWorkedConfirmedPieChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsWorkedConfirmedPieChartWidget::prepareChart(const int _log)
{
    QPieSeries *series = new QPieSeries();
    //PieSlice append(string label, real value)
    int qsos = dataProxy->getHowManyQSOInLog(_log);
    int confirmed = dataProxy->getHowManyConfirmedQSLInLog(_log);
       //qDebug() << "QSOs: " << QString::number(qsos) << Qt::endl;
       //qDebug() << "Confirmed: " << QString::number(confirmed) << Qt::endl;
       //qDebug() << "Worked: " << QString::number(qsos - confirmed) << Qt::endl;

    series->append(tr("Worked, not confirmed - %1").arg(qsos-confirmed), qsos - confirmed);
    series->append(tr("Confirmed - %2").arg(confirmed), confirmed);

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QPieSlice *slice1 = series->slices().at(0);
    slice1->setExploded();
    slice1->setLabelVisible();
    slice1->setPen(QPen(Qt::blue, 2));
    slice1->setBrush(Qt::blue);

    chart->addSeries(series);
    chart->setTitle(tr("Worked / Confirmed status"));
    chart->legend()->hide();

    chartView->setRenderHint(QPainter::Antialiasing);


}
