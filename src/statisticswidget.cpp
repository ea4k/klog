/***************************************************************************
                          statisticswidget.cpp  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

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
    connect(statisticToShowComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChartComboBoxChanged() ) ) ;
    connect(logComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLogComboBoxChanged() ) ) ;

    //barChartStats->prepareChart(1);

}


StatisticsWidget::~StatisticsWidget()
{

}

void StatisticsWidget::clear()
{
     //qDebug() <<  "StatisticsWidget::clear()" << QT_ENDL;
}

void StatisticsWidget::closeEvent(QCloseEvent *event)
{
       //qDebug() << "StatisticsWidget::closeEvent" << QT_ENDL;
    //barChartStats->clear();
    event->accept();
}

void StatisticsWidget::showEvent(QShowEvent *event)
{
      //qDebug() << "StatisticsWidget::showEvent" << QT_ENDL;
    //barChartStats->clear();
    fillLogCombo();
    event->accept();
}

void StatisticsWidget::slotChartComboBoxChanged()
{
       //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: " << statisticToShowComboBox->currentText()  << QT_ENDL;

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
    //qDebug() << Q_FUNC_INFO << " Text : " << logComboBox->currentText() << QT_ENDL;
    //qDebug() << Q_FUNC_INFO << " Log : " << QString::number(log) << QT_ENDL;
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
     statisticsToShowList << "15-" + tr("Grids per band status");
     statisticsToShowList << "16-" + tr("DXCC per band status");

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
        //cout << fonts.at(i).toLocal8Bit().constData() << QT_ENDL;
        logs<< ids.at(i) + "-" + dataProxy->getLogDateFromLog((ids.at(i)).toInt()) + "-" + dataProxy->getStationCallSignFromLog((ids.at(i)).toInt());
    }
    logComboBox->clear();
    logComboBox->addItems(logs);

}
