/***************************************************************************
                          statsqsospermodebarchatwidget.cpp  -  description
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

#include "statsqsospermodebarchartwidget.h"


StatsQSOsPerModeBarChartWidget::StatsQSOsPerModeBarChartWidget()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    chart = new QChart();
    chartView = new QChartView(chart);
}

StatsQSOsPerModeBarChartWidget::StatsQSOsPerModeBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{

#ifdef QT_DEBUG
//qDebug() << "StatsQSOsPerModeBarChartWidget::StatsQSOsPerModeBarChartWidget";
#else
#endif

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    //prepareChart();
}

StatsQSOsPerModeBarChartWidget::~StatsQSOsPerModeBarChartWidget()
{
    delete(dataProxy);
}

void StatsQSOsPerModeBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsQSOsPerModeBarChartWidget::prepareChart(const int _log)
{

    QString categoriesTitle;
    QString categoriesElem;
    QStringList categories;
    QBarSeries *series = new QBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QString aux;

    int numberPerX = 0;
    chart->removeAllSeries();
    //categoriesTitle = QString();
    //categoriesElem = QString();
    categories.clear();
    axis->clear();
    series->clear();
    QBarSet *set0 = new QBarSet(tr("QSOs per mode"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

       //qDebug() << "StatsQSOsPerModeBarChartWidget::prepareChart: SelectedGrapth-1: MODES ";
     categories.append(dataProxy->getModesInLog(_log));
     categoriesElem = tr("Modes");
     categoriesTitle = tr("QSOs per mode distribution");

     aux.clear();
    for (int i = 0; i < categories.count();i++ )
    {
        numberPerX = dataProxy->getQSOsInMode((categories.at(i)), _log);
           //qDebug() << categories.at(i) + "-" + QString::number(numberPerX);
        *set0 << numberPerX;
        //numberPerX = 0;
           //qDebug() << "StatsQSOsPerModeBarChartWidget::prepareChart QSOs: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX);
        aux = tr("Reading data ...") + "\n" + tr("Modes: ")  + QString::number(i) + "/" + QString::number(categories.count());
        //aux = tr("Reading data ...") + "\n" + tr("Modes: %1/%2").arg(QString::number(i)).arg(QString::number(categories.count()));
        progress.setLabelText(aux);
        progress.setValue(i);

        if ( progress.wasCanceled() )
        {
            i = categories.count();
        }
    }
    sum = set0->sum();
    set0->setLabel(QString::number(sum));

    series->append(set0);
    set0->setLabel(categoriesElem);
    chart->addSeries(series);
    chart->setTitle(categoriesTitle);

    axis->append(categories);
    //chart->createDefaultAxes();
    //series->attachAxis(axis);
    chart->addAxis(axis, Qt::AlignBottom);

    //chart->setAxisX(axis, series);
}
