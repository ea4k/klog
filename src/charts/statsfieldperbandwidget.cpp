/***************************************************************************
                          statsfieldperbandwidget.cpp  -  description
                             -------------------
    begin                : jul 2021
    copyright            : (C) 2021 by Jaime Robles
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
#include "statsfieldperbandwidget.h"

StatsFieldPerBandWidget::StatsFieldPerBandWidget(DataProxy_SQLite *dp, ValidFieldsForStats _field, QWidget *parent)
{
     //qDebug() << "StatsFieldPerBandWidget::StatsFieldPerBandWidget";
    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
    selectedField = _field;
    modeComboBox = new QComboBox;

    createUI();
    //prepareChart();
}

StatsFieldPerBandWidget::~StatsFieldPerBandWidget()
{
    delete(util);
    delete(dataProxy);
}

void StatsFieldPerBandWidget::createUI()
{
    modeInUse = "ALL";
    log = -1;
    QString allString = tr("All");
    modeComboBox->clear ();
    modeComboBox->addItem (allString);
    modeComboBox->addItems (dataProxy->getModesInLog (-1));

    dataProxy->getModesInLog (-1);

    tableWidget = new QTableWidget(this);
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(3);

    tableWidget->resizeRowsToContents();
    tableWidget->sortByColumn(0, Qt::AscendingOrder);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    QLabel *textLabel = new QLabel;
    textLabel->setText(tr("Mode: "));
    textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout *labelLayout = new QHBoxLayout;
    labelLayout->addWidget(textLabel);
    labelLayout->addWidget(modeComboBox);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(labelLayout);
    layout->addWidget(tableWidget);
    setLayout(layout);

    connect(modeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotModeComboBoxChanged() ) ) ;

}

void StatsFieldPerBandWidget::prepareChart(const int _log)
{
    //qDebug() << Q_FUNC_INFO << "Log = " << QString::number(_log);

    while(tableWidget->rowCount()>0)
    {
        tableWidget->removeRow(tableWidget->rowCount()-1);
    }

    log = _log;

    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Band")));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Worked")));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Confirmed")));
    tableWidget->setStyleSheet("QHeaderView::section { background-color:cornflowerblue }");

    QStringList bands;
    bands << dataProxy->getBandsInLog (_log);
    QString aux;
    int gridsCount;
    foreach (aux, bands)
    {
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem((aux)) );
        QTableWidgetItem *item = tableWidget->item (tableWidget->rowCount()-1, 0);
        item->setTextAlignment( Qt::AlignCenter );
        gridsCount = dataProxy->getFieldInBand (selectedField, aux, false, modeInUse, _log);
        tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(gridsCount)) );
        item = tableWidget->item (tableWidget->rowCount()-1, 1);
        item->setTextAlignment( Qt::AlignCenter );
        gridsCount = dataProxy->getFieldInBand (selectedField, aux, true, modeInUse, _log);
        tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(gridsCount)) );
        item = tableWidget->item (tableWidget->rowCount()-1, 2);
        item->setTextAlignment( Qt::AlignCenter );
    }
    // Now we add the "All" row.
    tableWidget->insertRow(tableWidget->rowCount());
    tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem(tr("All")) );
    QTableWidgetItem *item = tableWidget->item (tableWidget->rowCount()-1, 0);
    item->setTextAlignment( Qt::AlignCenter );
    gridsCount = dataProxy->getFieldInBand (selectedField, "ALL", false, modeInUse, _log);
    tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(gridsCount)) );
    item = tableWidget->item (tableWidget->rowCount()-1, 1);
    item->setTextAlignment( Qt::AlignCenter );
    gridsCount = dataProxy->getFieldInBand (selectedField, "ALL", true, modeInUse, _log);
    tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(gridsCount)) );
    item = tableWidget->item (tableWidget->rowCount()-1, 2);
    item->setTextAlignment( Qt::AlignCenter );
}

void StatsFieldPerBandWidget::slotConfirmedClicked()
{
    prepareChart(log);
}

void StatsFieldPerBandWidget::slotModeComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << ": " << modeComboBox->currentText() << "/" << QString::number(modeComboBox->currentIndex ());
    if (modeComboBox->currentIndex ()==0)
    {
        modeInUse = "ALL";
    }
    else
    {
        modeInUse = modeComboBox->currentText ();
    }

    prepareChart (log);
}
