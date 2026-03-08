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
    //qDebug() << Q_FUNC_INFO << "Start";
    dataProxy = dp;
    statisticToShowComboBox = new QComboBox();
    logComboBox = new QComboBox();

    stackedWidget = new QStackedWidget ;
    w1 = new StatsQSOsPerYearBarChartWidget(dataProxy, this);
    w2 = new StatsEntitiesPerYearBarChartWidget(dataProxy, this);
    w3 = new StatsCQZPerYearBarChartWidget(dataProxy, this);
    w4 = new StatsQSOsPerBandBarChartWidget(dataProxy, nullptr);
    w5 = new StatsQSOsPerModeBarChartWidget(dataProxy, nullptr);
    w6 = new StatsQSOsPerDXCCBarChartWidget(dataProxy, nullptr);
    w7 = new StatsQSOsPerContinentBarChartWidget(dataProxy, nullptr);
    w8 = new StatsQSOsPerHourBarChartWidget(dataProxy, nullptr);
    w9 = new StatsQSOsPerMonthBarChartWidget(dataProxy, nullptr);
    w10 = new StatsWorkedConfirmedPieChartWidget(dataProxy, nullptr);
    w11 = new StatsWorkedSentPieChartWidget(dataProxy, nullptr);
    w12 = new StatsSentConfirmedPieChartWidget(dataProxy, nullptr);
    w13 = new StatsGridsOnSatsWidget(dataProxy, nullptr);
    w14 = new StatsDXCCOnSatsWidget(dataProxy, nullptr);
    w15= new StatsFieldPerBandWidget(dataProxy, GridSquare, nullptr);
    w16 = new StatsFieldPerBandWidget(dataProxy, DXCC, nullptr);;


    statisticsToShowList.clear();

    createUI();
    connect(statisticToShowComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChartComboBoxChanged() ) ) ;
    connect(logComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLogComboBoxChanged() ) ) ;
    //qDebug() << Q_FUNC_INFO << "END";
}


StatisticsWidget::~StatisticsWidget()
{
    //qDebug() << Q_FUNC_INFO << "Start-END";
}

void StatisticsWidget::clear()
{
    //qDebug() << Q_FUNC_INFO << "Start-END";
     //qDebug() <<  "StatisticsWidget::clear()";
}

void StatisticsWidget::closeEvent(QCloseEvent *event)
{
       //qDebug() << "StatisticsWidget::closeEvent";
    //barChartStats->clear();
    //qDebug() << Q_FUNC_INFO << "Start-END";
    event->accept();
    //qDebug() << Q_FUNC_INFO << "END";
}

void StatisticsWidget::showEvent(QShowEvent *event)
{
      //qDebug() << "StatisticsWidget::showEvent";
    //qDebug() << Q_FUNC_INFO << "Start";
    fillLogCombo();
    event->accept();
    //qDebug() << Q_FUNC_INFO << "END";
}

void StatisticsWidget::slotChartComboBoxChanged()
{
       //qDebug() << "StatisticsWidget::slotChartComboBoxChanged: " << statisticToShowComboBox->currentText() ;
    //qDebug() << Q_FUNC_INFO << "Start";
    updateChart();
    statisticToShowComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << "END";
}

void StatisticsWidget::slotLogComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << "Start";
    updateChart();
    logComboBox->setFocus();
    //qDebug() << Q_FUNC_INFO << "END";
}

void StatisticsWidget::updateChart()
{
    //qDebug() << Q_FUNC_INFO << "Start";
    QString text = statisticToShowComboBox->currentText();
    text.truncate(2);
    //qDebug() << Q_FUNC_INFO << " - 010";
    int log = ((logComboBox->currentText()).section('-', 0, 0)).toInt();
    //qDebug() << Q_FUNC_INFO << " - 011";
    //qDebug() << Q_FUNC_INFO << " Text : " << logComboBox->currentText();
    //qDebug() << Q_FUNC_INFO << " Log : " << QString::number(log);
    //barChartStats->prepareChart(text.toInt(), log);
    switch (text.toInt())
    {
        case 1:
    {
        //qDebug() << Q_FUNC_INFO << "10";
        w1->prepareChart (log);
        //qDebug() << Q_FUNC_INFO << "11";
        stackedWidget->setCurrentWidget (w1);
        //qDebug() << Q_FUNC_INFO << "12";
    }
        break;
        case 2:
    {
        //qDebug() << Q_FUNC_INFO << "20";
        w2->prepareChart (log);
        stackedWidget->setCurrentWidget (w2);
    }
        break;

    case 3:
    {
        //qDebug() << Q_FUNC_INFO << "30";
        w3->prepareChart (log);
        stackedWidget->setCurrentWidget (w3);
    }
    break;
    case 4:
    {
        //qDebug() << Q_FUNC_INFO << "40";
        w4->prepareChart (log);
        stackedWidget->setCurrentWidget (w4);
    }
    break;
    case 5:
    {
        //qDebug() << Q_FUNC_INFO << "50";
        w5->prepareChart (log);
        stackedWidget->setCurrentWidget (w5);
    }
    break;
    case 6:
    {
        //qDebug() << Q_FUNC_INFO << "60";
        w6->prepareChart (log);
        stackedWidget->setCurrentWidget (w6);
    }
    break;
    case 7:
    { // How many QSO per Continent
        //qDebug() << Q_FUNC_INFO << "70";
        w7->prepareChart (log);
        stackedWidget->setCurrentWidget (w7);
    }
    break;
    case 8:
    {
        //qDebug() << Q_FUNC_INFO << "80";
        w8->prepareChart (log);
        stackedWidget->setCurrentWidget (w8);
    }
    break;
    case 9:
    {
        //qDebug() << Q_FUNC_INFO << "90";
        w9->prepareChart (log);
        stackedWidget->setCurrentWidget (w9);
    }
    break;
    case 10:
    {
        //qDebug() << Q_FUNC_INFO << "100";
        w10->prepareChart (log);
        stackedWidget->setCurrentWidget (w10);
    }
    break;
    case 11:
    {
        //qDebug() << Q_FUNC_INFO << "110";
        w11->prepareChart (log);
        stackedWidget->setCurrentWidget (w11);
    }
    break;
    case 12:
    {
        //qDebug() << Q_FUNC_INFO << "120";
        w12->prepareChart (log);
        stackedWidget->setCurrentWidget (w12);
    }
    break;
    case 13:
    {
        //qDebug() << Q_FUNC_INFO << "130";
        w13->prepareChart (log);
        stackedWidget->setCurrentWidget (w13);
    }
    break;
    case 14:
    {
        //qDebug() << Q_FUNC_INFO << "140";
        w14->prepareChart (log);
        stackedWidget->setCurrentWidget (w14);
    }
    break;
    case 15:
    {
        //qDebug() << Q_FUNC_INFO << "150";
        w15->prepareChart (log);
        stackedWidget->setCurrentWidget (w15);
        //genchart = new StatsFieldPerBandWidget(dataProxy, GridSquare, nullptr);
    }
    break;
    case 16:
    {
        //qDebug() << Q_FUNC_INFO << "160";
        w16->prepareChart (log);
        stackedWidget->setCurrentWidget (w16);
        //genchart = new StatsFieldPerBandWidget(dataProxy, DXCC, nullptr);
    }
    break;
    }
    //qDebug() << Q_FUNC_INFO << "END";
}

void StatisticsWidget::createUI()
 {
    //qDebug() << Q_FUNC_INFO << "Start";
    stackedWidget->addWidget(w1);
    stackedWidget->addWidget(w2);
    stackedWidget->addWidget(w3);
    stackedWidget->addWidget(w4);
    stackedWidget->addWidget(w5);
    stackedWidget->addWidget(w6);
    stackedWidget->addWidget(w7);
    stackedWidget->addWidget(w8);
    stackedWidget->addWidget(w9);
    stackedWidget->addWidget(w10);
    stackedWidget->addWidget(w11);
    stackedWidget->addWidget(w12);
    stackedWidget->addWidget(w13);
    stackedWidget->addWidget(w14);
    stackedWidget->addWidget(w15);
    stackedWidget->addWidget(w16);

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
     layout->addWidget(stackedWidget);
     setLayout(layout);
     resize(420,300);
     //qDebug() << Q_FUNC_INFO << "END";
 }

void StatisticsWidget::fillLogCombo()
{
    //qDebug() << Q_FUNC_INFO << "Start";
    QStringList ids;
    ids.clear();
    ids << dataProxy->getListOfManagedLogs();
    QStringList logs;
    logs.clear();

    for (int i = 0; i < ids.size(); ++i)
    {
        //cout << fonts.at(i).toLocal8Bit().constData();
        logs<< ids.at(i) + "-" + dataProxy->getLogDateFromLog((ids.at(i)).toInt()) + "-" + dataProxy->getStationCallSignFromLog((ids.at(i)).toInt());
    }
    logComboBox->clear();
    logComboBox->addItems(logs);
    //qDebug() << Q_FUNC_INFO << "END";
}
