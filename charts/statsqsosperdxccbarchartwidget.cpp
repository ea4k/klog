#include "charts/statsqsosperdxccbarchartwidget.h"


StatsQSOsPerDXCCBarChartWidget::StatsQSOsPerDXCCBarChartWidget(){}

StatsQSOsPerDXCCBarChartWidget::StatsQSOsPerDXCCBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
    //qDebug() << "StatsQSOsPerDXCCBarChartWidget::StatsQSOsPerDXCCBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    prepareChart();
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

void StatsQSOsPerDXCCBarChartWidget::prepareChart()
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
    QBarSet *set0 = new QBarSet(tr("QSOs per DXCC"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, x_axis.count(), this);
    progress.setWindowModality(Qt::WindowModal);
    aux = tr("Reading data...") ;
    progress.setLabelText(aux);
    progress.setValue(0);

    QStringList entities;
    entities.clear();
    entities << dataProxy->getEntitiesIds();

     //qDebug() << "StatsQSOsPerDXCCBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << endl;

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

    sum = set0->sum();
    set0->setLabel(QString::number(sum));

    series->append(set0);
    set0->setLabel(x_axisElem);
    chart->addSeries(series);
    chart->setTitle(x_axisTitle);

    axis->append(x_axis);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
}
