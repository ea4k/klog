#include "charts/barchartstats.h"
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

BarChartStats::BarChartStats(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{

     dataProxy = dp;
     mainWidget = new QWidget();
     mLayout = new QVBoxLayout;
     mainWidget->setLayout(mLayout);


     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(mainWidget);
     setLayout(mainLayout);

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
    qDebug() << Q_FUNC_INFO << endl;

   // qDeleteAll(mainLayout->children());
    QLayoutItem *child;
    while ((child = mLayout->takeAt(0)) != nullptr)
    {

        delete child;
    }
    qDebug() << Q_FUNC_INFO << " - END" << endl;
}


void BarChartStats::prepareChart(const int _selection, const int _log)
{
    cleanLayout();


    switch (_selection)
    {
        case 1:
    {
        //qsoPerYearBarChartWidget->prepareChart()
        genchart = new StatsQSOsPerYearBarChartWidget(dataProxy, nullptr);
    }
        break;
        case 2:
    {
        //cleanLayout();

        genchart = new StatsEntitiesPerYearBarChartWidget(dataProxy, nullptr);

    }
        break;
    case 3:
    {
        //cleanLayout();

        genchart = new StatsCQZPerYearBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 4:
    {
        //cleanLayout();
          //qDebug() << "BarChartStats::prepareChart SelectedGrapth-4: per band " << endl;
        genchart = new StatsQSOsPerBandBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 5:
    {
        //cleanLayout();
          //qDebug() << "BarChartStats::prepareChart SelectedGrapth-5: per modes " << endl;
        genchart = new StatsQSOsPerModeBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 6:
    {
        //cleanLayout();
           //qDebug() << "BarChartStats::prepareChart SelectedGrapth-6: per dxcc " << endl;
        genchart = new StatsQSOsPerDXCCBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 7:
    { // How many QSO per Continent
           //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/Continent " << endl;
        //cleanLayout();
        genchart = new StatsQSOsPerContinentBarChartWidget(dataProxy, nullptr);

    }
    break;
    case 8:
    {
           //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour " << endl;
        //cleanLayout();
        genchart = new StatsQSOsPerHourBarChartWidget(dataProxy, nullptr);

    }
    break;
    case 9:
    {
        //cleanLayout();
        genchart = new StatsQSOsPerMonthBarChartWidget(dataProxy, nullptr);

    }
    break;
    case 10:
    {
        //cleanLayout();
        genchart = new StatsWorkedConfirmedPieChartWidget(dataProxy, nullptr);

    }
    break;
    case 11:
    {
        //cleanLayout();
        genchart = new StatsWorkedSentPieChartWidget(dataProxy, nullptr);

    }
    break;
    case 12:
    {
        //cleanLayout();
        genchart = new StatsSentConfirmedPieChartWidget(dataProxy, nullptr);

    }
    break;
    case 13:
    {
        //cleanLayout();
        genchart = new StatsGridsOnSatsWidget(dataProxy, nullptr);

    }
    break;
    case 14:
    {
        //cleanLayout();
        genchart = new StatsDXCCOnSatsWidget(dataProxy, nullptr);

    }
    break;

    }

    genchart->prepareChart(_log);
    mLayout->addWidget(genchart);
    //delete genchart;
}
