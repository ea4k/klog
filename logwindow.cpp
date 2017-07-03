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


LogWindow::LogWindow(QWidget *parent) : QTableView(parent)
{
   //qDebug() << "LogWindow::LogWindow: "  << endl;
    //logModel = new QSqlRelationalTableModel(this);
    //logView = new QTableView;

    dataProxy = new DataProxy_SQLite();

    createUI();
    setDefaultData();

}

LogWindow::~LogWindow()
{

}


void LogWindow::clear()
{
   //qDebug() << "LogWindow::clear "  << endl;
}

void LogWindow::createUI()
{
   //qDebug() << "LogWindow::createUI"  << endl;
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSortingEnabled(true);


    //QVBoxLayout *layout = new QVBoxLayout;
    //layout->addWidget(logView);

    //setLayout(layout);
}

void LogWindow::setDefaultData()
{

    //qDebug() << "LogWindow::setDefaultData"  << endl;
}


void LogWindow::createlogPanel(const int _contestID)
{
   //qDebug() << "LogWindow::createlogPanel: " << QString::number(_contestID) << endl;

    int contestMode = dataProxy->getLogTypeOfUserLog(_contestID);

    if (contestMode == dataProxy->getLogTypeNumber("DX"))
    {
        //qDebug() << "LogWindow::createlogPanel: DX"  << endl;
        setColumnsToDX();
    }
    else if (contestMode == dataProxy->getLogTypeNumber("CQ_WW_SSB"))
    {

    }
    else if (contestMode == dataProxy->getLogTypeNumber("CQ_WW_CW"))
    {

    }
    else
    {
        //qDebug() << "LogWindow::createlogPanel: Default"  << endl;
        setColumnsToDX();
    }

    setItemDelegate(new QSqlRelationalDelegate(this));
    setSelectionMode( QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    resizeColumnsToContents();
    horizontalHeader()->setStretchLastSection(true);

}

void LogWindow::setColumnsToDX()
{
   //qDebug() << "LogWindow::setColumnsToDX"  << endl;
    QString stringQuery;
    stringQuery = QString("SELECT * FROM log LIMIT 1");
    QSqlQuery query;
    query.exec(stringQuery);
    QSqlRecord rec;
    rec = query.record(); // Number of columns
    int columns = rec.count();


    for (int i=0; i < columns; i++)
    {
        setColumnHidden(i, true);
    }

    columns = rec.indexOf("qso_date");
    setColumnHidden(columns, false);
    columns = rec.indexOf("time_on");
    setColumnHidden(columns, false);
    columns = rec.indexOf("call");
    setColumnHidden(columns, false);
    columns = rec.indexOf("rst_sent");
    setColumnHidden(columns, false);
    columns = rec.indexOf("rst_rcvd");
    setColumnHidden(columns, false);
    columns = rec.indexOf("bandid");
    setColumnHidden(columns, false);
    columns = rec.indexOf("modeid");
    setColumnHidden(columns, false);
    columns = rec.indexOf("comment");
    setColumnHidden(columns, false);
}
