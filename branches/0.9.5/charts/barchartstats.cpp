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

     dataProxy = dp;
     chart = new QChart();
     chartView = new QChartView(chart);

     createUI();
}

BarChartStats::~BarChartStats(){}

void BarChartStats::createUI()
{


    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void BarChartStats::prepareChart(const int _selection)
{

    QString x_axisTitle;
    QString x_axisElem;
    QStringList x_axis;
    QBarSeries *series = new QBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QString aux;

    int numberPerX = 0;
    chart->removeAllSeries();
    x_axisTitle = QString();
    x_axisElem = QString();
    x_axis.clear();
    axis->clear();
    series->clear();
    QBarSet *set0 = new QBarSet(tr("Chart title"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, x_axis.count(), this);
    progress.setWindowModality(Qt::WindowModal);

    switch (_selection)
    {
        case 1:
    {
           //qDebug() << "BarChartStats::prepareChart: SelectedGrapth-1: YEARS " << endl;
            x_axis.append(dataProxy->getOperatingYears(-1));
            x_axisElem = tr("QSOs");
            x_axisTitle = tr("QSOs per year");
           //qDebug() << "BarChartStats::prepareChart years: " << QString::number(x-axis.count()) << endl;
            aux.clear();
           for (int i = 0; i < x_axis.count();i++ )
           {
               numberPerX = dataProxy->getQSOonYear((x_axis.at(i)).toInt(), -1);
               //qDebug() << x_axis.at(i) + "-" + QString::number(numberPerX) << endl;
               *set0 << numberPerX;
               numberPerX = 0;
               //qDebug() << "BarChartStats::prepareChart QSOs: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;

               aux = tr("Reading data ...") + "\n" + tr("QSO: %1/%2").arg(QString::number(i)).arg(QString::number(x_axis.count()));
               progress.setLabelText(aux);
               progress.setValue(i);

               if ( progress.wasCanceled() )
               {
                   i = x_axis.count();
               }
           }
           sum = set0->sum();
           set0->setLabel(QString::number(sum));
    }
        break;
        case 2:
    {
            //qDebug() << "BarChartStats::prepareChart: SelectedGrapth-2: DXCC " << endl;
            x_axis.append(dataProxy->getOperatingYears(-1));
            x_axisElem = tr("DXCC Entities");
            x_axisTitle = tr("DXCC Entities per year");
            aux.clear();
            for (int i = 0; i < x_axis.count();i++ )
            {
                numberPerX = dataProxy->getDXCConYear((x_axis.at(i)).toInt(), -1);
                *set0 << numberPerX;
                numberPerX = 0;

                aux = tr("Reading data ...") + "\n" + tr("Entities: ")  + QString::number(i) + "/" + QString::number(x_axis.count());
                progress.setLabelText(aux);
                progress.setValue(i);

                if ( progress.wasCanceled() )
                {
                    i = x_axis.count();
                }
                //qDebug() << "BarChartStats::prepareChart DXCCs: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
            }
    }
        break;
    case 3:
    {
       //qDebug() << "BarChartStats::prepareChart SelectedGrapth-3: WAZ " << endl;
        x_axis.append(dataProxy->getOperatingYears(-1));
        x_axisElem = tr("CQ zones");
        x_axisTitle = tr("CQ zone per year");
        aux.clear();
        for (int i = 0; i < x_axis.count();i++ )
        {
            numberPerX = dataProxy->getCQzonYear((x_axis.at(i)).toInt(), -1);
            *set0 << numberPerX;
            numberPerX = 0;

            aux = tr("Reading data ...") + "\n" + tr("Years: %1/%2").arg(QString::number(i)).arg(QString::number(x_axis.count()));
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                i = x_axis.count();
            }
            //qDebug() << "BarChartStats::prepareChart CQz: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
        }
    }
    break;
    case 4:
    {
       //qDebug() << "BarChartStats::prepareChart SelectedGrapth-4: per modes " << endl;
       x_axis.append(dataProxy->getBandsInLog(-1));
       x_axisElem = tr("Bands");
       x_axisTitle = tr("QSO per band distribution");
        aux.clear();
        for (int i = 0; i < x_axis.count();i++ )
        {
            numberPerX = dataProxy->getQSOsInBand(x_axis.at(i), -1);
            *set0 << numberPerX;
            numberPerX = 0;

            aux = tr("Reading data ...") + "\n" + tr("Bands: ")  + QString::number(i) + "/" + QString::number(x_axis.count());
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                i = x_axis.count();
            }
            //qDebug() << "BarChartStats::prepareChart Bands: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
        }
    }
    break;
    case 5:
    {
       //qDebug() << "BarChartStats::prepareChart SelectedGrapth-5: per modes " << endl;
       x_axis.append(dataProxy->getModesInLog(-1));
       x_axisElem = tr("Modes");
       x_axisTitle = tr("QSO per mode distribution");
       //x_axis = _modes.count();
        aux.clear();
        for (int i = 0; i < x_axis.count();i++ )
        {
            numberPerX = dataProxy->getQSOsInMode(x_axis.at(i), -1);
            *set0 << numberPerX;
            numberPerX = 0;

            aux = tr("Reading data...") + "\n" + tr("Modes: ")  + QString::number(i) + "/" + QString::number(x_axis.count());
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                i = x_axis.count();
            }

                    //qDebug() << "BarChartStats::prepareChart Modes: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
        }
    }
    break;
    case 6:
    {
        //qDebug() << "BarChartStats::prepareChart SelectedGrapth-6: per dxcc " << endl;

         //bool noMore = false;
         aux = tr("Reading data...") ;
         progress.setLabelText(aux);
         progress.setValue(0);

         QStringList entities;
         entities.clear();

         entities << dataProxy->getEntitiesIds();


         QMap<int, int> map; // key,value = number of QSO, dxcc
         int qsos = -1;
         map.clear();
         aux.clear();

         for (int i = 0; i < entities.size(); ++i)
         {
             qsos = dataProxy->getQSOsWithDXCC((entities.at(i)).toInt(), -1);
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
                             //qDebug() << "Removing: " << QString::number(it.key()) << " / " << QString::number(it.value()) << endl;
                             //qDebug() << "Replacing by: " << entities.at(i) << " / " << QString::number(qsos) << " - " << dataProxy->getEntityNameFromId((entities.at(i)).toInt()) << endl;
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
          //qDebug() << "End of for iteration" << endl;
         }

         QMapIterator<int, int> it(map);
         while (it.hasNext()) {
             it.next();
             x_axis.append(dataProxy->getEntityMainPrefix(it.value()));

             numberPerX = it.key();
             *set0 << numberPerX;
             numberPerX = 0;
             //qDebug() << "End of while iteration" << endl;
         }
         //qDebug() << "Out of while" << endl;

         x_axisElem = tr("DXCC");
         x_axisTitle = tr("Top ten DXCC per QSO");
    }
    break;
    case 7:
    { // How many QSO per Continent
        //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/Continent " << endl;
        QStringList continents;
        continents.clear();
        continents << dataProxy->getContinentShortNames();

         x_axis.append(continents);
         x_axisElem = tr("Continents");
         x_axisTitle = tr("QSOs per continent");
         aux.clear();
         for (int i = 0; i < x_axis.count(); i++ )
         {
             numberPerX = dataProxy->getQSOsWithContinent(x_axis.at(i), -1);
             //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour: " << x_axis.at(i) << " - " << QString::number(numberPerX) << endl;
             *set0 << numberPerX;
             numberPerX = 0;


             aux = tr("Reading data ...") + "\n" + tr("Hours: ")  + QString::number(i) + "/" + QString::number(x_axis.count());
             progress.setLabelText(aux);
             progress.setValue(i);

             if ( progress.wasCanceled() )
             {
                 i = x_axis.count();
             }
             //qDebug() << "BarChartStats::prepareChart CQz: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
         }
    }
    break;
    case 8:
    {
        //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour " << endl;
        QStringList hours;
        hours.clear();
        hours << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
                << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";
         x_axis.append(hours);
         x_axisElem = tr("Hours");
         x_axisTitle = tr("QSOs at hour");
         aux.clear();
         for (int i = 0; i < x_axis.count(); i++ )
         {
             numberPerX = dataProxy->getQSOsAtHour((x_axis.at(i)).toInt(), -1);
             //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour: " << x_axis.at(i) << " - " << QString::number(numberPerX) << endl;
             *set0 << numberPerX;
             numberPerX = 0;


             aux = tr("Reading data ...") + "\n" + tr("Hours: ")  + QString::number(i) + "/" + QString::number(x_axis.count());
             progress.setLabelText(aux);
             progress.setValue(i);

             if ( progress.wasCanceled() )
             {
                 i = x_axis.count();
             }
             //qDebug() << "BarChartStats::prepareChart CQz: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
         }
    }
    break;
    }

    series->append(set0);
    set0->setLabel(x_axisElem);
    chart->addSeries(series);
    chart->setTitle(x_axisTitle);

    axis->append(x_axis);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
}
