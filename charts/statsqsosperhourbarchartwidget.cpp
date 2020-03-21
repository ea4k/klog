#include "charts/statsqsosperhourbarchartwidget.h"


StatsQSOsPerHourBarChartWidget::StatsQSOsPerHourBarChartWidget(){}

StatsQSOsPerHourBarChartWidget::StatsQSOsPerHourBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
    //qDebug() << "StatsQSOsPerHourBarChartWidget::StatsQSOsPerHourBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);

    createUI();
    prepareChart();
}

void StatsQSOsPerHourBarChartWidget::createUI()
{
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout;
    graphLayout->addWidget(chartView);
    setLayout(graphLayout);
}

void StatsQSOsPerHourBarChartWidget::prepareChart()
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
    QBarSet *set0 = new QBarSet(tr("QSOs per hour"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

    QStringList hours;
    hours.clear();
    hours << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
            << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";
     categories.append(hours);
     categoriesElem = tr("Hours");
     categoriesTitle = tr("QSOs at hour");
     aux.clear();
     for (int i = 0; i < categories.count(); i++ )
     {
         numberPerX = dataProxy->getQSOsAtHour((categories.at(i)).toInt(), -1);
          //qDebug() << "BarChartStats::prepareChart SelectedGrapth-7: QSO/hour: " << categories.at(i) << " - " << QString::number(numberPerX) << endl;
         *set0 << numberPerX;
         numberPerX = 0;


         aux = tr("Reading data ...") + "\n" + tr("Hours: ")  + QString::number(i) + "/" + QString::number(categories.count());
         progress.setLabelText(aux);
         progress.setValue(i);

         if ( progress.wasCanceled() )
         {
             i = categories.count();
         }
          //qDebug() << "BarChartStats::prepareChart CQz: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
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


