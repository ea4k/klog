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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
#include "barchartstats.h"

BarChartStats::BarChartStats(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{

     dataProxy = dp;
     mainWidget = new QWidget();
     mLayout = new QVBoxLayout;
     mainWidget->setLayout(mLayout);
     genchart = new StatsGeneralChartWidget();

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(mainWidget);
     setLayout(mainLayout);
}

BarChartStats::~BarChartStats()
{
    delete(genchart);
}

void BarChartStats::clear()
{
      //qDebug() << "BarChartStats::clear()" << QT_ENDL;
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
    //qDebug() << Q_FUNC_INFO << " - Start";
    QLayoutItem *child;
    //qDebug() << Q_FUNC_INFO << " - 01";

    while (((child = mLayout->takeAt(0)) != nullptr) && (mLayout->count()>0))
    {

        //qDebug() << Q_FUNC_INFO << " - In the while: 1 - Widgets: " << QString::number(mLayout->count());
        delete child->widget(); // delete the widget
        //qDebug() << Q_FUNC_INFO << " - In the while: 1";
        delete child;   // delete the layout item
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}


void BarChartStats::prepareChart(const int _selection, const int _log)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    cleanLayout();

    switch (_selection)
    {
        case 1:
    {
        //qDebug() << Q_FUNC_INFO << " - 01";
        genchart = new StatsQSOsPerYearBarChartWidget(dataProxy, nullptr);
    }
        break;
        case 2:
    {
        //qDebug() << Q_FUNC_INFO << " - 02";
        genchart = new StatsEntitiesPerYearBarChartWidget(dataProxy, nullptr);
    }
        break;
    case 3:
    {
        //qDebug() << Q_FUNC_INFO << " - 03";
        genchart = new StatsCQZPerYearBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 4:
    {
        //qDebug() << Q_FUNC_INFO << " - 04";
        genchart = new StatsQSOsPerBandBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 5:
    {
        //qDebug() << Q_FUNC_INFO << " - 05";
        genchart = new StatsQSOsPerModeBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 6:
    {
        //qDebug() << Q_FUNC_INFO << " - 06";
        genchart = new StatsQSOsPerDXCCBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 7:
    { // How many QSO per Continent
        //qDebug() << Q_FUNC_INFO << " - 07";
        genchart = new StatsQSOsPerContinentBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 8:
    {
        //qDebug() << Q_FUNC_INFO << " - 08";
        genchart = new StatsQSOsPerHourBarChartWidget(dataProxy, nullptr);
    }
    break;
    case 9:
    {
        //qDebug() << Q_FUNC_INFO << " - 09";
        genchart = new StatsQSOsPerMonthBarChartWidget(dataProxy, nullptr);

    }
    break;
    case 10:
    {
        //qDebug() << Q_FUNC_INFO << " - 10";
        genchart = new StatsWorkedConfirmedPieChartWidget(dataProxy, nullptr);
    }
    break;
    case 11:
    {
        //qDebug() << Q_FUNC_INFO << " - 11";
        genchart = new StatsWorkedSentPieChartWidget(dataProxy, nullptr);

    }
    break;
    case 12:
    {
        //qDebug() << Q_FUNC_INFO << " - 12";
        genchart = new StatsSentConfirmedPieChartWidget(dataProxy, nullptr);
    }
    break;
    case 13:
    {
        //qDebug() << Q_FUNC_INFO << " - 13";
        genchart = new StatsGridsOnSatsWidget(dataProxy, nullptr);
    }
    break;
    case 14:
    {
        //qDebug() << Q_FUNC_INFO << " - 14";
        genchart = new StatsDXCCOnSatsWidget(dataProxy, nullptr);

    }
    break;
    case 15:
    {
        //qDebug() << Q_FUNC_INFO << " - 15";
        genchart = new StatsFieldPerBandWidget(dataProxy, GridSquare, nullptr);
    }
    break;
    case 16:
    {
        //qDebug() << Q_FUNC_INFO << " - 16";
        genchart = new StatsFieldPerBandWidget(dataProxy, DXCC, nullptr);
    }
    break;

    }
    //qDebug() << Q_FUNC_INFO << " - 30";

    genchart->prepareChart(_log);
    //qDebug() << Q_FUNC_INFO << " - 31";
    mLayout->addWidget(genchart);
    //qDebug() << Q_FUNC_INFO << " - END - widgets: " << QString::number(mLayout->count());
}
