#include "statisticswidget.h"

StatisticsWidget::StatisticsWidget(DataProxy_SQLite *dp, QWidget *parent): QWidget(parent)
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

    //barChartStats->prepareChart(1);

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
    //barChartStats->clear();
    fillLogCombo();
    event->accept();
}

void StatisticsWidget::slotChartComboBoxChanged()
{    
       //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: " << statisticToShowComboBox->currentText()  << endl;

    updateChart();
    statisticToShowComboBox->setFocus();
}

void StatisticsWidget::slotLogComboBoxChanged()
{
    updateChart();
    logComboBox->setFocus();
}

void StatisticsWidget::updateChart()
{
    QString text = statisticToShowComboBox->currentText();
    text.truncate(2);
    //int log = ((logComboBox->currentText()).left((logComboBox->currentText()).indexOf('-')+1)).toInt();
    int log = ((logComboBox->currentText()).section('-', 0, 0)).toInt();
    //qDebug() << Q_FUNC_INFO << " Text : " << logComboBox->currentText() << endl;
    //qDebug() << Q_FUNC_INFO << " Log : " << QString::number(log) << endl;
    //barChartStats = new BarChartStats(dataProxy, this);
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
     statisticsToShowList << "13-" + tr("Satellite grid status");
     statisticsToShowList << "14-" + tr("Satellite DXCC status");

     statisticToShowComboBox->addItems(statisticsToShowList);

    fillLogCombo();

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(statisticToShowComboBox);
    hLayout->addWidget(logComboBox);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addLayout(hLayout);
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

}
