#include "statisticswidget.h"

StatisticsWidget::StatisticsWidget(DataProxy *dp, QWidget *parent): QWidget(parent)
{

    dataProxy = dp;
    statisticToShowComboBox = new QComboBox();
    logComboBox = new QComboBox();
    barChartStats = new BarChartStats(dp, this);
    //donutChartStats = new DonutChartStats(dp, this);

    //chartView = new QChartView(this);
    //graphWidget = new QWidget(this);

    statisticsToShowList.clear();

    createUI();
    connect(statisticToShowComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotChartComboBoxChanged() ) ) ;
    connect(logComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotLogComboBoxChanged() ) ) ;

    barChartStats->prepareChart(1);

}


StatisticsWidget::~StatisticsWidget()
{

}

void StatisticsWidget::clear()
{
   //qDebug() <<  "StatisticsWidget::clear()" << endl;
}

void StatisticsWidget::closeEvent(QCloseEvent *event)
{
     //qDebug() << "StatisticsWidget::closeEvent" << endl;
    //barChartStats->clear();
    event->accept();
}

void StatisticsWidget::showEvent(QShowEvent *event)
{
    //qDebug() << "StatisticsWidget::showEvent" << endl;
    barChartStats->clear();
    fillLogCombo();
    event->accept();
}

void StatisticsWidget::slotChartComboBoxChanged()
{    
     //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: " << statisticToShowComboBox->currentText()  << endl;
    //QString text = statisticToShowComboBox->currentText();
    //text.truncate(2);
     //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGrapth:  " << text << endl;

    //barChartStats->prepareChart(text.toInt());
    updateChart();
    statisticToShowComboBox->setFocus();
}

void StatisticsWidget::slotLogComboBoxChanged()
{

}

void StatisticsWidget::updateChart()
{
    QString text = statisticToShowComboBox->currentText();
    text.truncate(2);
    int log = ((logComboBox->currentText()).left((logComboBox->currentText()).indexOf('-')+1)).toInt();

    barChartStats->prepareChart(text.toInt(), log);
}

void StatisticsWidget::createUI()
 {
     statisticsToShowList << "01-" + tr("QSO per year");
     statisticsToShowList << "02-" + tr("DXCC per year");
     statisticsToShowList << "03-" + tr("CQ zones per year");
     statisticsToShowList << "04-" + tr("QSO per band");
     statisticsToShowList << "05-" + tr("QSO per mode");
     statisticsToShowList << "06-" + tr("QSO per DXCC");
     statisticsToShowList << "07-" + tr("QSO per Continent");
     statisticsToShowList << "08-" + tr("QSO per hour");
     statisticsToShowList << "09-" + tr("QSO per month");
     statisticsToShowList << "10-" + tr("Worked / Confirmed status");
     statisticsToShowList << "11-" + tr("Worked / Sent status");
     statisticsToShowList << "12-" + tr("Sent / Confirmed status");

     statisticToShowComboBox->addItems(statisticsToShowList);

    fillLogCombo();

     QHBoxLayout *hLayout = new QHBoxLayout;
     hLayout->addWidget(statisticToShowComboBox);
     hLayout->addWidget(logComboBox);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addLayout(hLayout);
     //layout->addWidget(statisticToShowComboBox);
     //layout->addWidget(graphWidget);
     layout->addWidget(barChartStats);
     setLayout(layout);
     resize(420,300);

 }

void StatisticsWidget::fillLogCombo()
{
    QStringList ids;
    ids.clear();
    ids << dataProxy->getListOfManagedLogs();
    QStringList logs;
    logs.clear();

    for (int i = 0; i < ids.size(); ++i)
    {
        //cout << fonts.at(i).toLocal8Bit().constData() << endl;
        logs<< ids.at(i) + "-" + dataProxy->getLogDateFromLog((ids.at(i)).toInt()) + "-" + dataProxy->getStationCallSignFromLog((ids.at(i)).toInt());
    }
    logComboBox->clear();
    logComboBox->addItems(logs);

/*
    QStringList getListOfManagedLogs();
    int getMaxLogNumber();
    QString getStationCallSignFromLog(const int _log);
    QStringList getStationCallSignsFromLog(const int _log);
    QString getOperatorsFromLog(const int _log);
    QString getCommentsFromLog(const int _log);
    QString getLogDateFromLog(const int _log);
    QString getLogTypeNFromLog(const int _log);
    bool addNewLog (const QStringList _qs);
    bool doesThisLogExist(const int _log);


*/
}
