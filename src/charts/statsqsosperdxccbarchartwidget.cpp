/***************************************************************************
                          statsqsosperdxccbarchatwidget.cpp  -  description
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

#include "charts/statsqsosperdxccbarchartwidget.h"


StatsQSOsPerDXCCBarChartWidget::StatsQSOsPerDXCCBarChartWidget(){}

StatsQSOsPerDXCCBarChartWidget::StatsQSOsPerDXCCBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{

#ifdef QT_DEBUG
//qDebug() << "StatsQSOsPerDXCCBarChartWidget::StatsQSOsPerDXCCBarChartWidget" << QT_ENDL;
#else
#endif

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
   // prepareChart();
}

void StatsQSOsPerDXCCBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsQSOsPerDXCCBarChartWidget::prepareChart(const int _log)
{

    QString categoriesTitle;
    QString categoriesElem;
    QStringList categories;
    QBarSeries *series = new QBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QString aux;

    //int numberPerX = 0;
    chart->removeAllSeries();
    categoriesTitle = QString();
    categoriesElem = QString();
    categories.clear();
    axis->clear();
    series->clear();
    QBarSet *set0 = new QBarSet(tr("QSOs per DXCC"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);
    aux = tr("Reading data...") ;
    progress.setLabelText(aux);
    progress.setValue(0);

    QStringList entities;
    entities.clear();
    entities << dataProxy->getEntitiesIds();

       //qDebug() << "StatsQSOsPerDXCCBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << QT_ENDL;

     QMap<int, int> map; // key,value = number of QSO, dxcc
     //int qsos = -1;
     map.clear();
     aux.clear();

     QList<QList<int>> data;
     data.clear();
    data <<  dataProxy->getTop10QSOPerDXCC(_log);
    //qDebug() << "StatsQSOsPerDXCCBarChartWidget::prepareChart: Length: " << QString::number(data.length()) << QT_ENDL;

    foreach(QList<int> pair, data)
    {
        map.insert(pair.at(0), pair.at(1));
        //qDebug() << "StatsQSOsPerDXCCBarChartWidget::prepareChart:Pair(0): " << QString::number(pair.at(0)) << QT_ENDL;
        //qDebug() << "StatsQSOsPerDXCCBarChartWidget::prepareChart:Pair(1): " << QString::number(pair.at(1)) << QT_ENDL;
        categories.append(dataProxy->getEntityMainPrefix(pair.at(0)) + " (" + QString::number(pair.at(1)) + ")");
        set0->append(pair.at(1));
        //*set0 << pair.at(1);
    }

    /*
     for (int i = 0; i < entities.size(); ++i)
     {
         qsos = dataProxy->getQSOsWithDXCC((entities.at(i)).toInt(), _log);
         if (qsos>0)
         {
                //qDebug() << "Checking: " << dataProxy->getEntityNameFromId((entities.at(i)).toInt()) << " - " << entities.at(i) << " - QSOs: " << QString::number(qsos) ;

             if (map.size()<10)
             {
                    //qDebug() << "Inserting: " << dataProxy->getEntityNameFromId((entities.at(i)).toInt()) << " - " << entities.at(i) << " - QSOs: " << QString::number(qsos) ;
                 map.insert(qsos, (entities.at(i)).toInt());
             }
             else
             {
                 QMapIterator<int, int> it(map);
                 while (it.hasNext()) {
                     it.next();
                     if (it.key()< qsos)
                     {
                            //qDebug() << "Removing: " << QString::number(it.key()) << " / " << QString::number(it.value()) << QT_ENDL;
                            //qDebug() << "Replacing by: " << entities.at(i) << " / " << QString::number(qsos) << " - " << dataProxy->getEntityNameFromId((entities.at(i)).toInt()) << QT_ENDL;
                         map.remove(it.key());
                         map.insert(qsos, (entities.at(i)).toInt());
                         it.toBack();
                     }
                 }
             }
         }
         aux = tr("Reading data...") + "\n" + tr("Entity: ")  + QString::number(i) + "/" + QString::number(entities.size());
         progress.setLabelText(aux);
         progress.setValue(i);

         if ( progress.wasCanceled() )
         {
             i = entities.size();
         }
         //qDebug() << "End of for iteration" << QT_ENDL;
     }
    */
    /*
     QMapIterator<int, int> it(map);
     while (it.hasNext()) {
         it.next();
         categories.append(dataProxy->getEntityMainPrefix(it.value()));
         //qDebug() << "While iteration: " << dataProxy->getEntityMainPrefix(it.value()) << QT_ENDL;

         numberPerX = it.key();
         *set0 << numberPerX;
         numberPerX = 0;
            //qDebug() << "End of while iteration" << QT_ENDL;
     }
        //qDebug() << "Out of while" << QT_ENDL;
    */
     categoriesElem = tr("DXCC");
     categoriesTitle = tr("Top ten DXCC per QSO");

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
