#include "statsentitiesperyearbarchartwidget.h"


StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget(){}

StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget(DataProxy *dp, QWidget *parent)
{
    qDebug() << "StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    prepareChart();
}

void StatsEntitiesPerYearBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsEntitiesPerYearBarChartWidget::prepareChart()
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

     qDebug() << "StatsEntitiesPerYearBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << endl;
     qDebug() << "BarChartStats::prepareChart: SelectedGrapth-2: DXCC " << endl;
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
             qDebug() << "BarChartStats::prepareChart DXCCs: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
        }

    series->append(set0);
    set0->setLabel(x_axisElem);
    chart->addSeries(series);
    chart->setTitle(x_axisTitle);

    axis->append(x_axis);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
}
