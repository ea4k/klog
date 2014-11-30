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
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

//#include <QtWidgets>
#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QAction>

class QStringList;
class QLabel;
class QVBoxLayout;

enum {
    Log_Id = 0,
    Log_Name = 1,
    Log_BandId = 2,
    Log_ModeId = 3,
    Log_DateId = 4,
    Log_TimeId = 5
};

class LogWindow : public  QWidget
{
    Q_OBJECT

public:
    LogWindow();
    ~LogWindow();


private:
    QSqlRelationalTableModel *logModel;
    QWidget *logPanel;
    QTableView *logView;
    QLabel *logLabel;

    QAction *logWinAct;
    QAction *loggWinAct;


    void createlogPanel();

};



#endif // LOGWINDOW_H
