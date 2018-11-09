#ifndef STATSGENERALCHARTWIDGET_H
#define STATSGENERALCHARTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QtDebug>
#include "dataproxy.h"

class StatsGeneralChartWidget : public QWidget
{
        Q_OBJECT
public:
    StatsGeneralChartWidget();
    StatsGeneralChartWidget(DataProxy *dp, QWidget *parent = 0);
    virtual void prepareChart();

private:
    virtual void createUI();
};

#endif // GENERALCHARTWIDGET_H
