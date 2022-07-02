/***************************************************************************
                          statsqsosperbandbarchatwidget.cpp  -  description
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

#include "statsqsosperbandbarchartwidget.h"

StatsQSOsPerBandBarChartWidget::StatsQSOsPerBandBarChartWidget()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    chart = new QChart();
    chartView = new QChartView(chart);
}

StatsQSOsPerBandBarChartWidget::StatsQSOsPerBandBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
      //qDebug() << "StatsQSOsPerBandBarChartWidget::StatsQSOsPerBandBarChartWidget" << QT_ENDL;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    //prepareChart();
}

StatsQSOsPerBandBarChartWidget::~StatsQSOsPerBandBarChartWidget()
{
    delete(dataProxy);
}

void StatsQSOsPerBandBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsQSOsPerBandBarChartWidget::prepareChart(const int _log)
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
    QBarSet *set0 = new QBarSet(tr("QSOs per band"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

       //qDebug() << "StatsQSOsPerBandBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << QT_ENDL;
     categories.append(dataProxy->getBandsInLog(_log));
     categoriesElem = tr("Bands");
     categoriesTitle = tr("QSOs per band distribution");

     aux.clear();
    for (int i = 0; i < categories.count();i++ )
    {
        numberPerX = dataProxy->getQSOsInBand((categories.at(i)), _log);
           //qDebug() << categories.at(i) + "-" + QString::number(numberPerX) << QT_ENDL;
        *set0 << numberPerX;
        //numberPerX = 0;
           //qDebug() << "StatsQSOsPerBandBarChartWidget::prepareChart QSOs: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX) << QT_ENDL;
        aux = tr("Reading data ...") + "\n" + tr("Bands: ")  + QString::number(i) + "/" + QString::number(categories.count());
        //aux = tr("Reading data ...") + "\n" + tr("Bands: %1/%2").arg(QString::number(i)).arg(QString::number(categories.count()));
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
