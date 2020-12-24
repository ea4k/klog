#include "charts/statssentconfirmedpiechartwidget.h"

StatsSentConfirmedPieChartWidget::StatsSentConfirmedPieChartWidget(){}

StatsSentConfirmedPieChartWidget::StatsSentConfirmedPieChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
      //qDebug() << "StatsSentConfirmedPieChartWidget::StatsSentConfirmedPieChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    prepareChart();
}

void StatsSentConfirmedPieChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsSentConfirmedPieChartWidget::prepareChart()
{
    QPieSeries *series = new QPieSeries();
    //PieSlice append(string label, real value)
    int sent = dataProxy->getHowManyQSLSentInLog(-1);
    int confirmed = dataProxy->getHowManyConfirmedQSLInLog(-1);

    //qDebug() << "Confirmed: " << QString::number(confirmed) << endl;


    series->append(tr("Sent - %1").arg(sent), sent);
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
    chart->setTitle(tr("Sent / Confirmed status"));
    chart->legend()->hide();

    chartView->setRenderHint(QPainter::Antialiasing);


}
