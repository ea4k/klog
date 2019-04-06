#include "statsqsospermonthbarchartwidget.h"


StatsQSOsPerMonthBarChartWidget::StatsQSOsPerMonthBarChartWidget(){}

StatsQSOsPerMonthBarChartWidget::StatsQSOsPerMonthBarChartWidget(DataProxy *dp, QWidget *parent)
{
    //qDebug() << "StatsQSOsPerMonthBarChartWidget::StatsQSOsPerMonthBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    prepareChart();
}

void StatsQSOsPerMonthBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsQSOsPerMonthBarChartWidget::prepareChart()
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
    QBarSet *set0 = new QBarSet(tr("QSOs per month"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, x_axis.count(), this);
    progress.setWindowModality(Qt::WindowModal);

    QStringList months;
    months.clear();
    months << tr("Jan") << tr("Feb") << tr("Mar") << tr("Apr") << tr("May") << tr("Jun")
              << tr("Jul") << tr("Aug") << tr("Sep") << tr("Oct") << tr("Nov") << tr("Dec");
     x_axis.append(months);
     x_axisElem = tr("Months");
     x_axisTitle = tr("QSOs at Month");
     aux.clear();
     for (int i = 0; i < x_axis.count(); i++ )
     {
         numberPerX = dataProxy->getQSOsOnMonth(i+1, -1);
          //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour: " << x_axis.at(i) << " - " << QString::number(numberPerX) << endl;
         *set0 << numberPerX;
         numberPerX = 0;


         aux = tr("Reading data ...") + "\n" + tr("Months: ")  + QString::number(i) + "/" + QString::number(x_axis.count());
         progress.setLabelText(aux);
         progress.setValue(i);

         if ( progress.wasCanceled() )
         {
             i = x_axis.count();
         }
          //qDebug() << "BarChartStats::prepareChart CQz: " << QString::number((x_axis.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
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
