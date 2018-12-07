#include "barchartstats.h"
/***************************************************************************
                          barchartstats.cpp  -  description
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

BarChartStats::BarChartStats(DataProxy *dp, QWidget *parent) : QWidget(parent)
{

     //chart = new QChart();
     //chartView = new QChartView(chart);

     //chart->setAnimationOptions(QChart::SeriesAnimations);
     //chart->legend()->setVisible(true);
     //chart->legend()->setAlignment(Qt::AlignBottom);
     //chartView->setRenderHint(QPainter::Antialiasing);

     dataProxy = dp;
     mainWidget = new QWidget();
     mLayout = new QVBoxLayout;
     mainWidget->setLayout(mLayout);

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(mainWidget);
     setLayout(mainLayout);
     //cleanLayout();
     //createUI();
}

BarChartStats::~BarChartStats(){}

void BarChartStats::clear()
{
    //qDebug() << "BarChartStats::clear()" << endl;
    //cleanLayout();
    prepareChart(1);
}

void BarChartStats::createUI()
{

    //QVBoxLayout *graphLayout = new QVBoxLayout;
    //graphLayout->addWidget(chartView);
    //setLayout(graphLayout);
}

void BarChartStats::cleanLayout()
{

    QLayoutItem *child;
    while ((child = mLayout->takeAt(0)) != 0) {
        delete child;
    }

}


void BarChartStats::prepareChart(const int _selection)
{
    cleanLayout();
    switch (_selection)
    {
        case 1:
    {
        //cleanLayout();
        genchart = new StatsQSOsPerYearBarChartWidget(dataProxy, 0);
        mLayout->addWidget(genchart);
    }
        break;
        case 2:
    {
        //cleanLayout();

        genchart = new StatsEntitiesPerYearBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
        break;
    case 3:
    {
        //cleanLayout();

        genchart = new StatsCQZPerYearBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 4:
    {
        //cleanLayout();
        //qDebug() << "BarChartStats::prepareChart SelectedGrapth-4: per band " << endl;
        genchart = new StatsQSOsPerBandBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 5:
    {
        //cleanLayout();
        //qDebug() << "BarChartStats::prepareChart SelectedGrapth-5: per modes " << endl;
        genchart = new StatsQSOsPerModeBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);

    }
    break;
    case 6:
    {
        //cleanLayout();
         //qDebug() << "BarChartStats::prepareChart SelectedGrapth-6: per dxcc " << endl;
        genchart = new StatsQSOsPerDXCCBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 7:
    { // How many QSO per Continent
         //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/Continent " << endl;
        //cleanLayout();
        genchart = new StatsQSOsPerContinentBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 8:
    {
         //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour " << endl;
        //cleanLayout();
        genchart = new StatsQSOsPerHourBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 9:
    {
        //cleanLayout();
        genchart = new StatsQSOsPerMonthBarChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 10:
    {
        //cleanLayout();
        genchart = new StatsWorkedConfirmedPieChartWidget(dataProxy, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 11:
    {
        //cleanLayout();
        genchart = new StatsWorkedSentPieChartWidget(dataProxy, nullptr);
        //genchart = new StatsPieChartWidget(dataProxy, Worked, Sent, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    case 12:
    {
        //cleanLayout();
        genchart = new StatsSentConfirmedPieChartWidget(dataProxy, nullptr);
        //genchart = new StatsPieChartWidget(dataProxy, Sent, Confirmed, nullptr);
        //genchart = new StatsPieChartWidget(dataProxy, Worked, Confirmed, nullptr);
        mLayout->addWidget(genchart);
    }
    break;
    }


}
