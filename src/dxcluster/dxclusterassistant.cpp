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
    header << tr("DX") << tr("Freq") << tr("Status"); // tr("Mode");
    tableWidget->setColumnCount(header.length());
    tableWidget->setHorizontalHeaderLabels(header);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(tableWidget, 3, 0, 1, -1);


    setLayout(mainLayout);
    return true;

    //connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelPushButtonClicked() ) );
        //qDebug() << Q_FUNC_INFO << " - END";
}

void DXClusterAssistant::newDXClusterSpot(proposedQSOs _q)
{
    list.append(_q);
    //qDebug() << Q_FUNC_INFO << " - 50";
    //qDebug() << Q_FUNC_INFO << " - 60";
    //qDebug() << Q_FUNC_INFO << QString("Data received: Call: %1 - Freq: %2 - Status: %3").arg(_call).arg(_freq).arg(getStringFromStatus(_status));
    addCall();
}

/*
void DXClusterAssistant::newDXClusterSpot(const QString &_call, const QSOStatus _status, double _freq)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    //Utilities util(Q_FUNC_INFO);
    //if (!util.isValidCall(_call, true))
    //{
    //    //qDebug() << Q_FUNC_INFO << " - Not Valid Call: " << _call;
    //    return;
    //}

    //qDebug() << Q_FUNC_INFO << ": Call: " << _call;
    //qDebug() << Q_FUNC_INFO << ": Freq: " << QString::number(_freq);
    //qDebug() << Q_FUNC_INFO << ": Status: " << ;
    proposedQSOs newArrival;
    newArrival.call = _call;
    newArrival.freq().fromDouble(_freq);
    newArrival.status = _status;
    newArrival.priority = 1;
    list.append(newArrival);
    //qDebug() << Q_FUNC_INFO << " - 50";

    //qDebug() << Q_FUNC_INFO << " - 60";
    //qDebug() << Q_FUNC_INFO << QString("Data received: Call: %1 - Freq: %2 - Status: %3").arg(_call).arg(_freq).arg(getStringFromStatus(_status));
    addCall();
    //qDebug() << Q_FUNC_INFO << " - END";
}
*/

QString DXClusterAssistant::getStringFromStatus(QSOStatus _s)
{
    switch (_s) {
    case unknown:
        return "unknown";
        break;
    case ATNO:
        return "ATNO";
        break;
    case needed:
        return "needed";
        break;
    case worked:
        return "worked";
        break;
    case confirmed:
        return "confirmed";
        break;
    default:
        return "unknown";
        break;
    }
}

void DXClusterAssistant::addCall()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    //qDebug() << Q_FUNC_INFO << ": Lenght of list: " << QString::number(list.count());
    proposedQSOs aux;
    foreach(aux, list)
    {
        //qDebug() << Q_FUNC_INFO << "Call: " << aux.call;
        QTableWidgetItem *newItemCall = new QTableWidgetItem(aux.call, QTableWidgetItem::Type);
        //QTableWidgetItem *newItemFreq = new QTableWidgetItem(aux.freq().toQString(), QTableWidgetItem::Type);
        QTableWidgetItem *newItemStatus = new QTableWidgetItem(getStringFromStatus(aux.status), QTableWidgetItem::Type);

        tableWidget->insertRow(tableWidget->rowCount());
        int row = tableWidget->rowCount();
        tableWidget->setItem(row-1, 0, newItemCall);
        //tableWidget->setItem(row-1, 1, newItemFreq);
        tableWidget->setItem(row-1, 2, newItemStatus);
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}
