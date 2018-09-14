#include "donutchartstats.h"
/***************************************************************************
                          donutchartstats.cpp  -  description
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

DonutChartStats::DonutChartStats(DataProxy *dp, QWidget *parent) : QWidget(parent)
{
    dataProxy = dp;
    chartView = new QChartView();

    createUI();
}




DonutChartStats::~DonutChartStats(){}

void DonutChartStats::createUI()
{
    //chart->setAnimationOptions(QChart::SeriesAnimations);
    //chart->legend()->setVisible(true);
    //chart->legend()->setAlignment(Qt::AlignBottom);
    //chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void DonutChartStats::prepareChart(const int _selection)
{

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);


    //QPieSlice *slice = series->append("Fat 15.6%", 15.6);
    //slice->setExploded();
    //slice->setLabelVisible();
    //series->append("Other 23.8%", 23.8);
    //series->append("Carbs 56.4%", 56.4);

//  QStringList getBandsInLog(const int _log);

    switch (_selection)
    {
        case 1:
           //qDebug() << "DonutChartStats::prepareChart: SelectedGrapth-1: Bands/year " << endl;
            //  QStringList getBandsInLog(const int _log);
          // for (int i = 0; i < x_axis.count();i++ )
          // {

          // }
        break;
        case 2:
            //qDebug() << "DonutChartStats::prepareChart: SelectedGrapth-1: Modes/year " << endl;
            // QStringList getModesInLog(const int _log);

        break;
    case 3:
        int confirmed = dataProxy->getHowManyConfirmedQSLInLog(-1);
        series->append("Confirmed", confirmed);
        series->append("Non confirmed", dataProxy->getHowManyQSOInLog(-1) - confirmed);

        //int getHowManyQSOInLog(const int _log);
        //int getHowManyConfirmedQSLInLog(const int _log);
    break;
    }


    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setTitle("Confirmed / No confirmed");
    chartView->chart()->addSeries(series);
    chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView->chart()->legend()->setFont(QFont("Arial", 7));

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();
}
