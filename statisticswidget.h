#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H
#include <QWidget>
#include <QtWidgets>
#include "dataproxy_sqlite.h"
#include <QtCharts>
#include "charts/barchartstats.h"
//#include "charts/donutchartstats.h"


class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~StatisticsWidget();
    void clear();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void slotChartComboBoxChanged();
    void slotLogComboBoxChanged();

private:
    void createUI();
    void fillLogCombo();
    void updateChart();
   // void paintQSOperOtherBarChart(const int _i);


     DataProxy_SQLite *dataProxy;
     //QWidget *graphWidget;

     QChartView *chartView;

     BarChartStats *barChartStats;
     //DonutChartStats *donutChartStats;

     QComboBox *statisticToShowComboBox, *logComboBox;
     QStringList statisticsToShowList;
};






#endif // STATISTICSWIDGET_H
