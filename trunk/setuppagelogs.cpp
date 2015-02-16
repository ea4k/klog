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

    stationCallsign = QString();
    operators = QString();
    comment = QString();
    dateString = QString();
    typeContest = -1;
    contestCatMode = -1;
    contestCatOperators = -1;
    contestCatAssisted = -1;
    contestCatPower = -1;
    contestCatBands = -1;
    contestBands = -1;


    newLog = new SetupPageLogsNew();
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

    newLog->exec();
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
    connect(newLog, SIGNAL(newLogData(QStringList)), this, SLOT(slotAnalyzeNewLogData(QStringList) ) );


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

void SetupPageLogs::slotAnalyzeNewLogData(const QStringList _qs)
{
    qDebug() << "SetupPageLogs::slotAnalyzeNewLogData" << endl;

    if (_qs.length()!=11)
    {
        qDebug() << "SetupPageLogs::slotAnalyzeNewLogData != 11" << endl;
        return;
    }

    stationCallsign = _qs.at(0);
    operators = _qs.at(1);
    comment = _qs.at(2);
    dateString = _qs.at(3);
    typeContest  = (_qs.at(4)).toInt();
    contestCatMode  = (_qs.at(5)).toInt();
    contestCatOperators  = (_qs.at(6)).toInt();
    contestCatAssisted  = (_qs.at(7)).toInt();
    contestCatPower  = (_qs.at(8)).toInt();
    contestCatBands  = (_qs.at(9)).toInt();
    contestBands  = (_qs.at(10)).toInt();

    QStringList newLog;
    newLog.clear();
    newLog << dateString << stationCallsign << QString::number(typeContest) << comment;
    addNewLog(newLog);

}

bool SetupPageLogs::addNewLog(const QStringList _qs)
{
    qDebug() << "SetupPageLogs::addNewLog" << endl;
    QString aux = QString();
    int nameCol = -1;

    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);
    QString _typeContest = _qs.at(2);
    QString _comment = _qs.at(3);


    QString queryString = QString("SELECT * FROM logs WHERE logdate='%1' AND stationcall='%2' AND logtype='%3'").arg(_dateString).arg(_stationCallsign).arg(_typeContest);
    //"logs"
    //"id, logdate, stationcall, comment, logtype"
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    QSqlRecord rec = query.record(); // Number of columns

    while ( (query.next()) && (query.isValid()) )
    {
        nameCol = rec.indexOf("id");
        aux = (query.value(nameCol)).toString();
        qDebug() << "SetupPageLogs::addNewLog: id = " << aux << endl;
        return false;
    }
    queryString = QString("INSERT INTO logs (logdate, stationcall, comment, logtype) values('%1','%2','%3','%4')").arg(_dateString).arg(_stationCallsign).arg(_comment).arg(_typeContest);

    sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        qDebug() << "SetupPageLogs::addNewLog ADDED! id = "  << endl;
    }
    else
    {
        return false;
    }


    La query de insercion no da fallo pero no se ve el log

}



