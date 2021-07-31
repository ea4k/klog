/***************************************************************************
                          statsqsosperhourbarchatwidget.cpp  -  description
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

#include "charts/statsqsosperhourbarchartwidget.h"


StatsQSOsPerHourBarChartWidget::StatsQSOsPerHourBarChartWidget(){}

StatsQSOsPerHourBarChartWidget::StatsQSOsPerHourBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
#ifdef QT_DEBUG
    //qDebug() << "StatsDxccOnSatsWidget::StatsDxccOnSatsWidget" << Qt::endl;
#else
#endif

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    //prepareChart();
}

void StatsQSOsPerHourBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsQSOsPerHourBarChartWidget::prepareChart(const int _log)
{

    QString categoriesTitle;
    QString categoriesElem;
    QStringList categories;
    QBarSeries *series = new QBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QString aux;

    int numberPerX = 0;
    chart->removeAllSeries();
    categoriesTitle = QString();
    categoriesElem = QString();
    categories.clear();
    axis->clear();
    series->clear();
    QBarSet *set0 = new QBarSet(tr("QSOs per hour"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

    QStringList hours;
    hours.clear();
    hours << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
            << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";
     categories.append(hours);
     categoriesElem = tr("Hours");
     categoriesTitle = tr("QSOs at hour");
     aux.clear();
     for (int i = 0; i < categories.count(); i++ )
     {
         numberPerX = dataProxy->getQSOsAtHour((categories.at(i)).toInt(), _log);
            //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour: " << categories.at(i) << " - " << QString::number(numberPerX) << Qt::endl;
         *set0 << numberPerX;
         //numberPerX = 0;


         aux = tr("Reading data ...") + "\n" + tr("Hours: ")  + QString::number(i) + "/" + QString::number(categories.count());
         progress.setLabelText(aux);
         progress.setValue(i);

         if ( progress.wasCanceled() )
         {
             i = categories.count();
         }
            //qDebug() << "BarChartStats::prepareChart CQz: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX) << Qt::endl;
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


