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
