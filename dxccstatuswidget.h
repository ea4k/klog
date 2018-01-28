#ifndef DXCCSTATUSWIDGET_H
#define DXCCSTATUSWIDGET_H
/***************************************************************************
                          dxccstatuswidget.h  -  description
                             -------------------
    begin                : feb 2016
    copyright            : (C) 2016 by Jaime Robles
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
#include <QWidget>
#include <QtWidgets>
#include <QString>
//#include <QTableView>
#include "awards.h"
#include "world.h"
#include "dataproxy.h"


//TODO: Creating the widget to show the DXCC status

class DXCCStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DXCCStatusWidget(DataProxy *dp, QWidget *parent = 0);
    ~DXCCStatusWidget();
    
    void update();    
    void setBands(const QStringList _ent, const bool _creating = false); // Receives the list of bandIDs
    void setCurrentLog(const int _logN);
    void refresh();

signals:

public slots:
    //void slotSearchLineEditTextChanged();
    void slotRefreshButtonClicked();
private:
    void createUI();
    void setDefaultBands();
    void addEntity(QStringList const _ent); // DXCC id, bandid, bandid, ...
    //QStringList sortBandNamesBottonUp(const QStringList _qs);

    QTableWidget *dxccView;
    Awards *awards;
    World *world;
    DataProxy *dataProxy;

    QHeaderView *hv, *hh;
    //QLineEdit *searchLineEdit;
    QPushButton *refreshButton;
    //QRadioButton *showAllLogsButton;


    int numberOfColumns; // Columns will be number Of Bands + 2 (Id + Name)
    QStringList bandNames, validBands;
    int logNumber, tempLog; // log in use in the log / log to be used in the widget

};

#endif // DXCCSTATUSWIDGET_H
