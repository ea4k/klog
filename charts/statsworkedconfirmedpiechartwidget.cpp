#include "charts/statsworkedconfirmedpiechartwidget.h"

StatsWorkedConfirmedPieChartWidget::StatsWorkedConfirmedPieChartWidget(){}

StatsWorkedConfirmedPieChartWidget::StatsWorkedConfirmedPieChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
      //qDebug() << "StatsWorkedConfirmedPieChartWidget::StatsWorkedConfirmedPieChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    //prepareChart();
}

void StatsWorkedConfirmedPieChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsWorkedConfirmedPieChartWidget::prepareChart(const int _log)
{
    QPieSeries *series = new QPieSeries();
    //PieSlice append(string label, real value)
    int qsos = dataProxy->getHowManyQSOInLog(_log);
    int confirmed = dataProxy->getHowManyConfirmedQSLInLog(_log);
       //qDebug() << "QSOs: " << QString::number(qsos) << endl;
       //qDebug() << "Confirmed: " << QString::number(confirmed) << endl;
       //qDebug() << "Worked: " << QString::number(qsos - confirmed) << endl;

    series->append(tr("Worked, not confirmed - %1").arg(qsos-confirmed), qsos - confirmed);
    series->append(tr("Confirmed - %2").arg(confirmed), confirmed);

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QPieSlice *slice1 = series->slices().at(0);
    slice1->setExploded();
    slice1->setLabelVisible();
    slice1->setPen(QPen(Qt::blue, 2));
    slice1->setBrush(Qt::blue);

    chart->addSeries(series);
    chart->setTitle(tr("Worked / Confirmed status"));
    chart->legend()->hide();

    chartView->setRenderHint(QPainter::Antialiasing);


}
