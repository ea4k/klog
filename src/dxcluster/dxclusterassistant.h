#ifndef DXCLUSTERASSISTANT_H
#define DXCLUSTERASSISTANT_H
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



// This widget will show the DX-Cluster assistant
// The class shows an ordered list of available spots that are available now and the
// associated freq/mode.
// Info is coming from the cluster and ordered taking into account dxcc status:
// confirmed/worked/needed/ATNO or even other factors TBD.

#include <QtWidgets>
#include "../global.h"

class DXClusterAssistant : public QWidget
{
    Q_OBJECT
public:
    explicit DXClusterAssistant(const QString &_parentFunction, QWidget *parent = nullptr);
    ~DXClusterAssistant();
    bool init();
signals:

private:
    bool createUI();

    QTableWidget *tableWidget;
    QHeaderView *hv, *hh;
};



#endif // DXCLUSTERASSISTANT_H
