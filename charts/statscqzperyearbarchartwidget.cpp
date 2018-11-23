#include "statscqzperyearbarchartwidget.h"


StatsCQZPerYearBarChartWidget::StatsCQZPerYearBarChartWidget(){}

StatsCQZPerYearBarChartWidget::StatsCQZPerYearBarChartWidget(DataProxy *dp, QWidget *parent)
{
    qDebug() << "StatsCQZPerYearBarChartWidget::StatsCQZPerYearBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    prepareChart();
}

void StatsCQZPerYearBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsCQZPerYearBarChartWidget::prepareChart()
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
    QBarSet *set0 = new QBarSet(tr("CQ Zones per year"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, x_axis.count(), this);
    progress.setWindowModality(Qt::WindowModal);

     qDebug() << "StatsCQZPerYearBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << endl;
     x_axis.append(dataProxy->getOperatingYears(-1));
     x_axisElem = tr("CQ zones");
     x_axisTitle = tr("CQ zones per year");

     aux.clear();
    for (int i = 0; i < x_axis.count();i++ )
    {
        numberPerX = dataProxy->getCQzonYear((x_axis.at(i)).toInt(), -1);
        //numberPerX = dataProxy->getQSOonYear((x_axis.at(i)).toInt(), -1);
         qDebug() << x_axis.at(i) + "-" + QString::number(numberPerX) << endl;
        *set0 << numberPerX;
        numberPerX = 0;
         qDebug() << "StatsCQZPerYearBarChartWidget::prepareChart QSOs: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;

        aux = tr("Reading data ...") + "\n" + tr("Years: %1/%2").arg(QString::number(i)).arg(QString::number(x_axis.count()));
        progress.setLabelText(aux);
        progress.setValue(i);

        if ( progress.wasCanceled() )
        {
            i = x_axis.count();
        }
    }
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
