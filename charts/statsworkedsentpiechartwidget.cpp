#include "statsworkedsentpiechartwidget.h"

StatsWorkedSentPieChartWidget::StatsWorkedSentPieChartWidget(){}

StatsWorkedSentPieChartWidget::StatsWorkedSentPieChartWidget(DataProxy *dp, QWidget *parent)
{
    //qDebug() << "StatsWorkedSentPieChartWidget::StatsWorkedSentPieChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    prepareChart();
}

void StatsWorkedSentPieChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsWorkedSentPieChartWidget::prepareChart()
{
    QPieSeries *series = new QPieSeries();
    //PieSlice append(string label, real value)
    int qsos = dataProxy->getHowManyQSOInLog(-1);
    int confirmed = dataProxy->getHowManyQSLSentInLog(-1);
     //qDebug() << "QSOs: " << QString::number(qsos) << endl;
     //qDebug() << "Confirmed: " << QString::number(confirmed) << endl;
     //qDebug() << "Worked: " << QString::number(qsos - confirmed) << endl;

    series->append(tr("Worked - %1").arg(qsos-confirmed), qsos - confirmed);
    series->append(tr("Sent - %2").arg(confirmed), confirmed);

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);


    chart->addSeries(series);
    chart->setTitle("Worked / Confirmed status");
    chart->legend()->hide();

    chartView->setRenderHint(QPainter::Antialiasing);


}
