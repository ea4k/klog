#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H
#include <QWidget>
#include <QtWidgets>
#include "dataproxy.h"
#include <QtCharts>
#include "charts/barchartstats.h"
//#include "charts/donutchartstats.h"


class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(DataProxy *dp, QWidget *parent = nullptr);
    ~StatisticsWidget();

private slots:
    void slotChartComboBoxChanged();

private:
    void createUI();
   // void paintQSOperOtherBarChart(const int _i);


     DataProxy *dataProxy;
     //QWidget *graphWidget;

     QChartView *chartView;

     BarChartStats *barChartStats;
     //DonutChartStats *donutChartStats;

     QComboBox *statisticToShowComboBox;
     QStringList statisticsToShowList;
};






#endif // STATISTICSWIDGET_H
