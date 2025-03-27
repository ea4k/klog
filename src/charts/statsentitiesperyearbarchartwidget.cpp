/***************************************************************************
                          statsentitiesperyearbarchatwidget.cpp  -  description
                             -------------------
    begin                : oct 2018
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
#include "statsentitiesperyearbarchartwidget.h"


StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
    Q_UNUSED(parent);

#ifdef QT_DEBUG
//qDebug() << "StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget";
#else
#endif

    dataProxy = dp;
    //chart = new QChart();
    chartView = new QChartView();

    createUI();
    //prepareChart();
}

StatsEntitiesPerYearBarChartWidget::~StatsEntitiesPerYearBarChartWidget()
{
    //delete(dataProxy);
}

void StatsEntitiesPerYearBarChartWidget::createUI()
{
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsEntitiesPerYearBarChartWidget::prepareChart(const int _log)
{
    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QString categoriesTitle;
    QString categoriesElem;
    QStringList categories;
    QBarSeries *series = new QBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QString aux;

    //int numberPerX = 0;
    chart->removeAllSeries();
    categoriesTitle = tr("DXCC Entities");
    categoriesElem = tr("DXCC Entities");
    categories.clear();
    axis->clear();
    series->clear();
    QBarSet *set0 = new QBarSet(tr("Chart title"));

    //*set0->remove(0, set0->count()-1);
    //qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

       //qDebug() << "StatsEntitiesPerYearBarChartWidget::prepareChart: SelectedGrapth-1: YEARS ";
       //qDebug() << "BarChartStats::prepareChart: SelectedGrapth-2: DXCC ";
        categories.append(dataProxy->getOperatingYears(_log));
        //categoriesElem = tr("DXCC Entities");
        //categoriesTitle = tr("DXCC Entities");
        aux.clear();
        //int numberPerX;
        for (int i = 0; i < categories.count();i++ )
        {
            *set0 << dataProxy->getDXCConYear((categories.at(i)).toInt(), _log);
            //*set0 << numberPerX;
            //numberPerX = 0;

            aux = tr("Reading data ...") + "\n" + tr("Entities: ")  + QString::number(i) + "/" + QString::number(categories.count());
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                i = categories.count();
            }
               //qDebug() << "BarChartStats::prepareChart DXCCs: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX);
        }
    series->append(set0);
    set0->setLabel(categoriesElem);
    chart->addSeries(series);
    chart->setTitle(categoriesTitle);

    axis->append(categories);
    //chart->createDefaultAxes();
    //series->attachAxis(axis);
    chart->addAxis(axis, Qt::AlignBottom);
    chartView->setChart (chart);
}
