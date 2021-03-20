#ifndef STATSGENERALCHARTWIDGET_H
#define STATSGENERALCHARTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QtDebug>
#include "dataproxy_sqlite.h"

class StatsGeneralChartWidget : public QWidget
{
        Q_OBJECT
public:
    StatsGeneralChartWidget();
    StatsGeneralChartWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    virtual void prepareChart(const int _log=-1);

private:
    virtual void createUI();
};

#endif // GENERALCHARTWIDGET_H
