#include "statisticswidget.h"

StatisticsWidget::StatisticsWidget(DataProxy *dp, QWidget *parent): QWidget(parent)
{

    dataProxy = dp;
    statisticToShowComboBox = new QComboBox();
    barChartStats = new BarChartStats(dp, this);
    //donutChartStats = new DonutChartStats(dp, this);

    //chartView = new QChartView(this);
    //graphWidget = new QWidget(this);

    statisticsToShowList.clear();

    createUI();
    connect(statisticToShowComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotChartComboBoxChanged() ) ) ;
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
    event->accept();
}

void StatisticsWidget::slotChartComboBoxChanged()
{    
     //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: " << statisticToShowComboBox->currentText()  << endl;
    QString text = statisticToShowComboBox->currentText();
    text.truncate(2);
     //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGrapth:  " << text << endl;

    barChartStats->prepareChart(text.toInt());

    statisticToShowComboBox->setFocus();
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


     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(statisticToShowComboBox);
     //layout->addWidget(graphWidget);
     layout->addWidget(barChartStats);
     setLayout(layout);
     resize(420,300);

 }
