/***************************************************************************
                          logwindow.cpp  -  description
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

#include "logwindow.h"


LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent)
{
    //logModel = new QSqlRelationalTableModel(this);
    logView = new QTableView;

    createUI();
    setDefaultData();


}

LogWindow::~LogWindow()
{

}


void LogWindow::clear()
{

}

void LogWindow::createUI()
{
    logView->setContextMenuPolicy(Qt::CustomContextMenu);
    logView->setSortingEnabled(true);
    //logView->setCurrentIndex(logModel->index(0, 0));

}

void LogWindow::setDefaultData()
{


}

void LogWindow::createLogModel()
{

}
