/***************************************************************************
                          statsdxccsonsatswidget.cpp  -  description
                             -------------------
    begin                : dec 2020
    copyright            : (C) 2020 by Jaime Robles
    email                : jaime@robles.es
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

#include "statsdxccsonsatswidget.h"



StatsDXCCOnSatsWidget::StatsDXCCOnSatsWidget(DataProxy_SQLite *dp, QWidget *parent)
{

#ifdef QT_DEBUG
    //qDebug() << "StatsDxccOnSatsWidget::StatsDxccOnSatsWidget" << QT_ENDL;
#else
#endif

    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
    confirmedOnlyCheckBox = new QCheckBox;
    onlyLEOSatCheckBox = new QCheckBox;
    numberLabel = new QLabel;

    log = -1;

    //chart = new QChart();
    //chartView = new QChartView(chart);
    //chart->setTheme(QChart::ChartThemeQt);
    //chart->setTheme(QChart::ChartThemeBlueCerulean);

    createUI();
    //prepareChart();
}

void StatsDXCCOnSatsWidget::createUI()
{
    confirmedOnlyCheckBox->setText(tr("Show confirmed only"));
    confirmedOnlyCheckBox->setChecked(false);
    onlyLEOSatCheckBox->setText(tr("Only LEO sats", "LEO means Low Earth Orbiting and it is a well known word for hams. Do not translate if not sure."));
    onlyLEOSatCheckBox->setChecked(false);

    tableWidget = new QTableWidget(this);
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(7);

    tableWidget->resizeRowsToContents();
    tableWidget->sortByColumn(2, Qt::AscendingOrder);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    //logView->sortByColumn(1);




    QLabel *textLabel = new QLabel;
    textLabel->setText(tr("Number"));
    textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    numberLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    numberLabel->setText(QString::number(0));

    QHBoxLayout *labelLayout = new QHBoxLayout;
    labelLayout->addWidget(textLabel);
    labelLayout->addWidget(numberLabel);

    QHBoxLayout *checksLayout = new QHBoxLayout;
    checksLayout->addWidget(confirmedOnlyCheckBox);
    checksLayout->addWidget(onlyLEOSatCheckBox);
    checksLayout->addLayout(labelLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(checksLayout);
    layout->addWidget(tableWidget);
    setLayout(layout);

    connect(confirmedOnlyCheckBox, SIGNAL(clicked()), this, SLOT(slotConfirmedClicked() ) );
    connect(onlyLEOSatCheckBox, SIGNAL(clicked()), this, SLOT(slotConfirmedClicked() ) );

}

StatsDXCCOnSatsWidget::~StatsDXCCOnSatsWidget()
{
    delete(util);
}

void StatsDXCCOnSatsWidget::prepareChart(const int _log)
{
    //qDebug() << Q_FUNC_INFO << "Log = " << QString::number(_log) << QT_ENDL;

     while(tableWidget->rowCount()>0)
     {
         tableWidget->removeRow(tableWidget->rowCount()-1);
     }

     log = _log;
     QList<QSO*> _qsos;
     _qsos.clear();
     _qsos << dataProxy->getSatDXCCStats(log);
     tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Callsign")));
     tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Date")));
     tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Band")));
     tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Mode")));
     tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("DXCC")));
     tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Satellite")));
     tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Confirmed")));
     tableWidget->setStyleSheet("QHeaderView::section { background-color:cornflowerblue }");
     //qDebug() << "StatsDxccOnSatsWidget::prepareChart: QSOs: " << QString::number(_qsos.length()) << QT_ENDL;

     int number = 0;
     QList<int> entities;
     entities.clear();

     if (_qsos.length()>0)
     {
         for (int i = 0; i<_qsos.length(); i++)
         {
             bool confirmed = false;
             bool printThisOne = true;
             QString satName = _qsos.at(i)->getSatName();

             QString qslStatus = QString();
             if (_qsos.at(i)->getLoTWQSL_RCVD() == "Y")
             {
                qslStatus = "LoTW";
                confirmed = true;
             }
             else if (_qsos.at(i)->getQSL_RCVD() == "Y")
             {
                qslStatus = "QSL";
                confirmed = true;
             }
             else
             {
                 qslStatus = QString(tr("No"));
                 confirmed = false;
             }

             if (confirmedOnlyCheckBox->isChecked() && !confirmed)
             {
                 printThisOne = false;
             }
             if ((onlyLEOSatCheckBox->isChecked()) && (satName == "QO-100") )
             {
                 printThisOne = false;
             }
             if (entities.contains(_qsos.at(i)->getDXCC()))
             {
                 printThisOne = false;
             }

             if (printThisOne)
             {
                 //qDebug() << "StatsDxccOnSatsWidget::prepareChart: QSOs: printThisOne: " << (_qsos.at(i)->getCall())  << QT_ENDL;
                 entities.append(_qsos.at(i)->getDXCC());
                 number++;
                 tableWidget->insertRow(tableWidget->rowCount());
                 tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem((_qsos.at(i)->getCall())) );
                 tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem( util->getDateSQLiteStringFromDate(_qsos.at(i)->getDate()) ) );
                 tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem((_qsos.at(i)->getBand())) );
                 tableWidget->setItem(tableWidget->rowCount()-1, 3, new QTableWidgetItem((_qsos.at(i)->getMode())) );
                 tableWidget->setItem(tableWidget->rowCount()-1, 4, new QTableWidgetItem((dataProxy->getEntityNameFromId(_qsos.at(i)->getDXCC()))) );
                 tableWidget->setItem(tableWidget->rowCount()-1, 5, new QTableWidgetItem(satName));
                 tableWidget->setItem(tableWidget->rowCount()-1, 6, new QTableWidgetItem(qslStatus) );
             }

         }
         numberLabel->setText(QString::number(number));
     }
}

void StatsDXCCOnSatsWidget::slotConfirmedClicked()
{
    prepareChart(log);
}
