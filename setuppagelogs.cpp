/***************************************************************************
                          setuppagelogs.cpp  -  description
                             -------------------
    begin                : feb 2012
    copyright            : (C) 2012 by Jaime Robles
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

//#include <QDebug>

#include "setuppagelogs.h"


SetupPageLogs::SetupPageLogs(QWidget *parent) : QWidget(parent){
//qDebug() << "SetupPageLogs::SetupPageLogs" << endl;

    //SetupPageLogsNew newLog;


    logsModel = new QSqlRelationalTableModel(this);
    logsView = new QTableView;
    logsView->setContextMenuPolicy(Qt::CustomContextMenu);
    logsView->setSortingEnabled(true);

    createLogsModel();
    createLogsPanel();

    logsView->setCurrentIndex(logsModel->index(0, 0));

    lastLog = 0;


    newLogPushButton = new QPushButton(tr("New"), this);
    //loadAllPushButton = new QPushButton(tr("Load All"), this);
    //loadSelectedPushButton = new QPushButton(tr("Load"), this);
    //clearPushButton = new QPushButton(tr("Clear"), this);
    editPushButton = new QPushButton(tr("Edit"), this);
    removePushButton = new QPushButton(tr("Remove"), this);


    newLogPushButton->setToolTip(tr("Add a new log"));
    //loadAllPushButton->setToolTip(tr("Load all the logs"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected log"));
    //clearPushButton->setToolTip(tr("Clear selection"));
    editPushButton->setToolTip(tr("Edit the selected log"));
    removePushButton->setToolTip(tr("Remove the selected log"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(newLogPushButton);
    //buttonsLayout->addWidget(loadSelectedPushButton);
    //buttonsLayout->addWidget(loadAllPushButton);
    buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);
    //buttonsLayout->addWidget(clearPushButton);


    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(logsView);
    widgetLayout->addLayout(buttonsLayout);
    //widgetLayout->addLayout(logDataLayout);


    setLayout(widgetLayout);

    //connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );

   // readLogs();
    createActions();

}

SetupPageLogs::~SetupPageLogs(){
    ////qDebug() << "SetupPageLogs::~SetupPageLogs" << endl;
}



void SetupPageLogs::slotNewButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotNewButtonClicked" << endl;
    newLog.show();
}

void SetupPageLogs::slotEditButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotEditButtonClicked" << endl;
}

void SetupPageLogs::slotRemoveButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotRemoveButtonClicked" << endl;
}

void SetupPageLogs::createLogsPanel()
{
    logsView->setModel(logsModel);
    QString stringQuery = QString("SELECT * FROM logs");
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record(); // Number of columns
    int columns = rec.count();

    for (int i = 0; i < columns; i++ ){
        logsView->setColumnHidden(i, true);
    }

    columns = rec.indexOf("id");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("logdate");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("stationcall");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("comment");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("logtype");
    logsView->setColumnHidden(columns, false);

    logsView->setItemDelegate(new QSqlRelationalDelegate(this));
    logsView->setSelectionMode( QAbstractItemView::SingleSelection);
    logsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    logsView->resizeColumnsToContents();
    logsView->horizontalHeader()->setStretchLastSection(true);

}

void SetupPageLogs::createLogsModel()
{
        QString stringQuery = QString("SELECT * FROM logs");
        QSqlQuery q(stringQuery);
        QSqlRecord rec = q.record();
/*
      query.exec("CREATE TABLE logs ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "logdate VARCHAR(10), "
                 "stationcall VARCHAR(15) NOT NULL, "
                 "comment VARCHAR, "
                 "logtype VARCHAR )");

*/
        int nameCol;

        //logsModel = new QSqlRelationalTableModel(this);
        logsModel->setTable("logs");

        nameCol = rec.indexOf("id");
        logsModel->setSort(nameCol, Qt::AscendingOrder);
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("ID"));

        nameCol = rec.indexOf("logdate");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Date"));

        nameCol = rec.indexOf("stationcall");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Station Callsign"));

        nameCol = rec.indexOf("comment");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Comments"));

        nameCol = rec.indexOf("logtype");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Type"));


        logsModel->select();
}


void SetupPageLogs::createActions()
{
    connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );
    connect(removePushButton, SIGNAL(clicked ( )), this, SLOT(slotRemoveButtonClicked() ) );
    connect(editPushButton, SIGNAL(clicked ( )), this, SLOT(slotEditButtonClicked() ) );


    //loadAllPushButton->setToolTip(tr("Load all the logs"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected log"));
    //clearPushButton->setToolTip(tr("Clear selection"));

}

void SetupPageLogs::readLogs()
{
    //qDebug() << "SetupPageLogs::readLogs" << endl;

    QString aux, callUsed;
    QStringList _logs;
    QSqlQuery query;
    QDate date = QDate::currentDate();

    _logs.clear();
    aux = "SELECT lognumber FROM log";

    query.exec(aux);
    QSqlRecord rec = query.record();

    while ( (query.next()) && (query.isValid()) ) {
        aux = (query.value(0)).toString();
        //qDebug() << "SetupPageLogs::readLogs: " << aux << endl;
        if (!(_logs.contains(aux)))
        {
            _logs += aux;
        }
    }
    int i = 0;
    int nameCol = 0;
    while (i <= _logs.size())
    {
        aux = QString("SELECT qso_date, station_callsign, operator FROM log WHERE lognumber = '%1'").arg(_logs.at(i));
        while ( (query.next()) && (query.isValid()) )
        {
            nameCol = rec.indexOf("qso_date");
            aux = (query.value(nameCol)).toString();
            if (date < QDate::fromString(aux, "yyyy/MM/dd"))
            {
                date = QDate::fromString(aux, "yyyy/MM/dd");
            }

                nameCol = rec.indexOf("station_callsign");
                aux = (query.value(nameCol)).toString();
                nameCol = rec.indexOf("operator");
                //revisar como va
                aux = (query.value(nameCol)).toString();
        }
    }
        //qDebug() << "SetupPageLogs::readLogs: " << QString::number(_logs.size())<< endl;
}




