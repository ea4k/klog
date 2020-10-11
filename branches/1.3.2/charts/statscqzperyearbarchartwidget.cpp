#include "charts/statscqzperyearbarchartwidget.h"


StatsCQZPerYearBarChartWidget::StatsCQZPerYearBarChartWidget(){}

StatsCQZPerYearBarChartWidget::StatsCQZPerYearBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
      //qDebug() << "StatsCQZPerYearBarChartWidget::StatsCQZPerYearBarChartWidget" << endl;

    dataProxy = dp;
    chart = new QChart();
    chartView = new QChartView(chart);
    //chart->setTheme(QChart::ChartThemeQt);
    //chart->setTheme(QChart::ChartThemeBlueCerulean);

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

    QString categoriesTitle;
    QString categoriesElem;
    QStringList categories;
    QBarSeries *series = new QBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QString aux;

    int numberPerX = 0;
    chart->removeAllSeries();

    categories.clear();
    axis->clear();
    series->clear();
    QBarSet *set0 = new QBarSet(tr("CQ Zones per year"));

    //*set0->remove(0, set0->count()-1);
    qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

       //qDebug() << "StatsCQZPerYearBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << endl;
     categories.append(dataProxy->getOperatingYears(-1));
     categoriesElem = tr("CQ zones");
     categoriesTitle = tr("CQ zones per year");

     aux.clear();
    for (int i = 0; i < categories.count();i++ )
    {
        numberPerX = dataProxy->getCQzonYear((categories.at(i)).toInt(), -1);
        //numberPerX = dataProxy->getQSOonYear((categories.at(i)).toInt(), -1);
           //qDebug() << categories.at(i) + "-" + QString::number(numberPerX) << endl;
        *set0 << numberPerX;

           //qDebug() << "StatsCQZPerYearBarChartWidget::prepareChart QSOs: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;

        aux = tr("Reading data ...") + "\n" + tr("Years: %1/%2").arg(QString::number(i)).arg(QString::number(categories.count()));
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
