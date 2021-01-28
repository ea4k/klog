#include "charts/statsqsosperbandbarchartwidget.h"


StatsQSOsPerBandBarChartWidget::StatsQSOsPerBandBarChartWidget(){}

StatsQSOsPerBandBarChartWidget::StatsQSOsPerBandBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
      //qDebug() << "StatsQSOsPerBandBarChartWidget::StatsQSOsPerBandBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    //prepareChart();
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
    categoriesTitle = QString();
    categoriesElem = QString();
    categories.clear();
    axis->clear();
    series->clear();
    QBarSet *set0 = new QBarSet(tr("QSOs per band"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

       //qDebug() << "StatsQSOsPerBandBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << endl;
     categories.append(dataProxy->getBandsInLog(_log));
     categoriesElem = tr("Bands");
     categoriesTitle = tr("QSOs per band distribution");

     aux.clear();
    for (int i = 0; i < categories.count();i++ )
    {
        numberPerX = dataProxy->getQSOsInBand((categories.at(i)), _log);
           //qDebug() << categories.at(i) + "-" + QString::number(numberPerX) << endl;
        *set0 << numberPerX;
        numberPerX = 0;
           //qDebug() << "StatsQSOsPerBandBarChartWidget::prepareChart QSOs: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
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
