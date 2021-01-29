#include "charts/statsqsosperyearbarchartwidget.h"


StatsQSOsPerYearBarChartWidget::StatsQSOsPerYearBarChartWidget(){}

StatsQSOsPerYearBarChartWidget::StatsQSOsPerYearBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
      //qDebug() << "StatsQSOsPerYearBarChartWidget::StatsQSOsPerYearBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    //prepareChart();
}

void StatsQSOsPerYearBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsQSOsPerYearBarChartWidget::prepareChart(const int _log)
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
    QBarSet *set0 = new QBarSet(tr("QSOs per year"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

     categories.append(dataProxy->getOperatingYears(_log));
     categoriesElem = tr("QSOs");
     categoriesTitle = tr("QSOs per year");

     aux.clear();
    for (int i = 0; i < categories.count();i++ )
    {
        numberPerX = dataProxy->getQSOonYear((categories.at(i)).toInt(), _log);
           //qDebug() << categories.at(i) + "-" + QString::number(numberPerX) << endl;
        *set0 << numberPerX;
        numberPerX = 0;
           //qDebug() << "StatsQSOsPerYearBarChartWidget::prepareChart QSOs: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;

        aux = tr("Reading data ...") + "\n" + tr("QSO: %1/%2").arg(QString::number(i)).arg(QString::number(categories.count()));
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
