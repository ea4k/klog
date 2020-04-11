#include "charts/statsentitiesperyearbarchartwidget.h"


StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget(){}

StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget(DataProxy_SQLite *dp, QWidget *parent)
{
      //qDebug() << "StatsEntitiesPerYearBarChartWidget::StatsEntitiesPerYearBarChartWidget" << endl;

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
    QBarSet *set0 = new QBarSet(tr("Chart title"));

    //*set0->remove(0, set0->count()-1);
    //qreal sum = 0;
    QProgressDialog progress(tr("Reading data ... "), tr("Abort reading"), 0, categories.count(), this);
    progress.setWindowModality(Qt::WindowModal);

       //qDebug() << "StatsEntitiesPerYearBarChartWidget::prepareChart: SelectedGrapth-1: YEARS " << endl;
       //qDebug() << "BarChartStats::prepareChart: SelectedGrapth-2: DXCC " << endl;
        categories.append(dataProxy->getOperatingYears(-1));
        categoriesElem = tr("DXCC Entities");
        categoriesTitle = tr("DXCC Entities per year");
        aux.clear();
        for (int i = 0; i < categories.count();i++ )
        {
            numberPerX = dataProxy->getDXCConYear((categories.at(i)).toInt(), -1);
            *set0 << numberPerX;
            numberPerX = 0;

            aux = tr("Reading data ...") + "\n" + tr("Entities: ")  + QString::number(i) + "/" + QString::number(categories.count());
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                i = categories.count();
            }
               //qDebug() << "BarChartStats::prepareChart DXCCs: " << QString::number((categories.at(i)).toInt()) << "/" << QString::number(numberPerX) << endl;
        }

    series->append(set0);
    set0->setLabel(categoriesElem);
    chart->addSeries(series);
    chart->setTitle(categoriesTitle);

    axis->append(categories);
    //chart->createDefaultAxes();
    //series->attachAxis(axis);
    chart->addAxis(axis, Qt::AlignBottom);

    // chart->setAxisX(axis, series);

}
