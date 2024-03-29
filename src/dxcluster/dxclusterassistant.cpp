/***************************************************************************
    dxclusterassistant.h  -  description
       -------------------
begin                : mar 2024
copyright            : (C) 2024 by Jaime Robles
email                : jaime@robles.es
***************************************************************************/

/****************************************************************************
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
#include "dxclusterassistant.h"

DXClusterAssistant::DXClusterAssistant(const QString &_parentFunction, QWidget *parent)
    : QWidget{parent}
{
#ifdef QT_DEBUG
    //qDebug() << ": " << _parentFunction;
#else
#endif
    (void)_parentFunction;
    //qDebug() << Q_FUNC_INFO << " - Start: " + _parentFunction;

    tableWidget = new QTableWidget;
}

DXClusterAssistant::~DXClusterAssistant(){};

bool DXClusterAssistant::init()
{
    list.clear();
    return createUI();
}

bool DXClusterAssistant::createUI()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    tableWidget->setSortingEnabled (true);

    hv = tableWidget->verticalHeader();
    hv->hide();
    hv->setStretchLastSection(true);
    hh = tableWidget->horizontalHeader();

    QStringList header;
    header.clear();
    header << tr("DX") << tr("Freq") << tr("Priority"); // tr("Mode");
    tableWidget->setColumnCount(header.length());
    tableWidget->setHorizontalHeaderLabels(header);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(tableWidget, 3, 0, 1, -1);


    setLayout(mainLayout);
    return true;

    //connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelPushButtonClicked() ) );
        //qDebug() << Q_FUNC_INFO << " - END";
}

void DXClusterAssistant::newDXClusterSpot(const QString &_call, const double &_freq, const QSOStatus _status)
{
    qDebug() << Q_FUNC_INFO << " - Start";
    proposedQSOs newArrival;
    newArrival.call = _call;
    newArrival.freq = _freq;
    newArrival.priority = 1;
    list.append(newArrival);

    QString status;
    switch (_status) {
    case unknown:
        status ="unknown";
        break;
    case ATNO:
        status ="ATNO";
        break;
    case needed:
        status ="needed";
        break;
    case worked:
        status ="worked";
        break;
    case confirmed:
        status ="confirmed";
        break;
    default:
        status ="unknown";
        break;
    }
    qDebug() << Q_FUNC_INFO << QString("Data received: Call: %1 - Freq: %2 - Status: %3").arg(_call).arg(_freq).arg(status);
    addCall();
}

void DXClusterAssistant::addCall()
{
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    proposedQSOs aux;
    foreach(aux, list)
    {
        qDebug() << Q_FUNC_INFO << "Call: " << aux.call;
        QTableWidgetItem *newItemCall = new QTableWidgetItem(aux.call);
        QTableWidgetItem *newItemFreq = new QTableWidgetItem(QString::number(aux.freq));
        int row = tableWidget->rowCount();
        tableWidget->setItem(row, 0, newItemCall);
        tableWidget->setItem(row, 1, newItemFreq);
        tableWidget->insertRow(tableWidget->rowCount());
    }
}
