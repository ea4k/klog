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

StatisticsWidget::~StatisticsWidget(){}

void StatisticsWidget::slotChartComboBoxChanged()
{    
    //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: " << statisticToShowComboBox->currentText()  << endl;
    QString text = statisticToShowComboBox->currentText();
    text.truncate(2);
    //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGrapth:  " << text << endl;
    //QHBoxLayout *l = new QHBoxLayout;
    //l->setContentsMargins(0, 0, 0, 0);

    switch (text.toInt())
    {

    case 1:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-1:  " << text << endl;
            barChartStats->prepareChart(1);
            //l->addWidget(barChartStats);

    break;
    case 2:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-2:  " << text << endl;
            barChartStats->prepareChart(2);
            //l->addWidget(barChartStats);
    break;
    case 3:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-3:  " << text << endl;
            barChartStats->prepareChart(3);
             //l->addWidget(barChartStats);
    break;
    case 4:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-3:  " << text << endl;
            barChartStats->prepareChart(4);
             //l->addWidget(barChartStats);
    break;
    case 5:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-3:  " << text << endl;
            barChartStats->prepareChart(5);
             //l->addWidget(barChartStats);
    break;

    case 6:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-6:  " << text << endl;
            barChartStats->prepareChart(6);
    break;
    case 7:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-7:  " << text << endl;
            barChartStats->prepareChart(7);
             //l->addWidget(barChartStats);
    break;
    case 8:
            //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-8:  " << text << endl;
            barChartStats->prepareChart(8);
             //l->addWidget(barChartStats);
    break;
/*    case 4:
        //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: SelectedGraph-4:  " << text << endl;
        //donutChartStats->prepareChart(3);
       //l->addWidget(donutChartStats);
*/
     // break;
        }
        //graphWidget->setLayout(l);
        //barChartStats->show();
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


     statisticToShowComboBox->addItems(statisticsToShowList);


     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(statisticToShowComboBox);
     //layout->addWidget(graphWidget);
     layout->addWidget(barChartStats);
     setLayout(layout);
     resize(420,300);

 }
