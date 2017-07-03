#ifndef LOGWINDOW_H
#define LOGWINDOW_H

/***************************************************************************
                          logwindow.h  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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

#include <QtWidgets>
#include <QWidget>
//#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QAction>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include "dataproxy.h"
#include "dataproxy_sqlite.h"



class LogWindow : public  QTableView
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();
    void createlogPanel(const int _contestID);
    void clear();

private:    
    void createUI();
    void setDefaultData();
    void setColumnsToDX();

    //void createLogModel();
    DataProxy *dataProxy;
    //QSqlRelationalTableModel *logModel;
    QTableView *logView;
    QLabel *logLabel;

};



#endif // LOGWINDOW_H
