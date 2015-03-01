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
    typeContest = QString();
    contestCatMode = -1;
    contestCatOperators = -1;
    contestCatAssisted = -1;
    contestCatPower = -1;
    contestCatBands = -1;
    contestBands = -1;

    currentLogs = new QComboBox();
    logsAvailable.clear();

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

    currentLogs->setToolTip(tr("Select the log you want to open"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(newLogPushButton);
    //buttonsLayout->addWidget(loadSelectedPushButton);
    //buttonsLayout->addWidget(loadAllPushButton);
    buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);
    //buttonsLayout->addWidget(clearPushButton);


    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(logsView);
    widgetLayout->addWidget(currentLogs);
    widgetLayout->addLayout(buttonsLayout);
    //widgetLayout->addLayout(logDataLayout);


    setLayout(widgetLayout);

    //connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );


    createActions();
    updateSelectedLogs();

}

SetupPageLogs::~SetupPageLogs(){
    //qDebug() << "SetupPageLogs::~SetupPageLogs" << endl;
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
    //qDebug() << "SetupPageLogs::createLogsPanel" << endl;
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
    //qDebug() << "SetupPageLogs::createLogsModel" << endl;

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
    //qDebug() << "SetupPageLogs::createActions" << endl;
    connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );
    connect(removePushButton, SIGNAL(clicked ( )), this, SLOT(slotRemoveButtonClicked() ) );
    connect(editPushButton, SIGNAL(clicked ( )), this, SLOT(slotEditButtonClicked() ) );
    connect(newLog, SIGNAL(newLogData(QStringList)), this, SLOT(slotAnalyzeNewLogData(QStringList) ) );


    //loadAllPushButton->setToolTip(tr("Load all the logs"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected log"));
    //clearPushButton->setToolTip(tr("Clear selection"));

}

QStringList SetupPageLogs::readLogs()
{
    //qDebug() << "SetupPageLogs::readLogs" << endl;

    QString aux, aux2;
    QStringList _logs;
    QSqlQuery query;
    int nameCol = -1;
    bool sqlOk = false;
    QDate date = QDate::currentDate();
    aux2.clear();
    aux.clear();
    _logs.clear();


    aux = "SELECT id, logdate, stationcall, logtype FROM logs";

    sqlOk = query.exec(aux);
    if (sqlOk)
    {
        QSqlRecord rec = query.record();

        while ( (query.next()) && (query.isValid()) )
        {
            aux2.clear();

            nameCol = rec.indexOf("id");
            aux2 = (query.value(nameCol)).toString();

            nameCol = rec.indexOf("logdate");
            aux2 = aux2.append("--");
            aux2.append((query.value(nameCol)).toString());

            nameCol = rec.indexOf("stationcall");
            aux2 = aux2.append("-");
            aux2.append((query.value(nameCol)).toString());

            nameCol = rec.indexOf("logtype");
            aux2 = aux2.append("-");
            aux2.append((query.value(nameCol)).toString());


            _logs.append(aux2);

        }
        return _logs;
    }
    else
    {
        return _logs;
    }


    _logs.clear();
    return _logs;
    //qDebug() << "SetupPageLogs::readLogs: " << QString::number(_logs.size())<< endl;
}


void SetupPageLogs::slotAnalyzeNewLogData(const QStringList _qs)
{
    qDebug() << "SetupPageLogs::slotAnalyzeNewLogData (length=" << QString::number(_qs.length()) << ")" << endl;

    if (_qs.length()!=12)
    {
        return;
    }

    stationCallsign = _qs.at(0);
    operators = _qs.at(1);
    comment = _qs.at(2);
    dateString = _qs.at(3);
    typeContest  = _qs.at(4);
    contestCatMode  = (_qs.at(5)).toInt();
    contestCatOperators  = (_qs.at(6)).toInt();
    contestCatAssisted  = (_qs.at(7)).toInt();
    contestCatPower  = (_qs.at(8)).toInt();
    contestCatBands  = (_qs.at(9)).toInt();
    contestBands  = (_qs.at(10)).toInt();

    QStringList newLog;
    newLog.clear();
    newLog << dateString << stationCallsign << _qs.at(4) << comment;
    addNewLog(newLog);

}

bool SetupPageLogs::addNewLog(const QStringList _qs)
{
    qDebug() << "SetupPageLogs::addNewLog: " << _qs.at(2) << endl;
    QString aux = QString();
    int nameCol = -1;

    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);
    QString _typeContest = _qs.at(2);
    QString _comment = _qs.at(3);


    QString queryString = QString("SELECT * FROM logs WHERE logdate='%1' AND stationcall='%2' AND logtype='%3'").arg(_dateString).arg(_stationCallsign).arg(_typeContest);
    //"logs"
    //"id, logdate, stationcall, comment, logtype"
    qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << endl;
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

    qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << endl;
    sqlOK = query.exec(queryString);
    if (sqlOK)
    {
        //qDebug() << "SetupPageLogs::addNewLog ADDED! id = "  << endl;
        logsModel->select();
        updateSelectedLogs();
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

void SetupPageLogs::updateSelectedLogs()
{
    qDebug() << "SetupPageLogs::updateSelectedLogs" << endl;
    logsAvailable = readLogs();

    if (logsAvailable.length()>0)
    {
        currentLogs->clear();
        currentLogs->addItems(logsAvailable);
    }
    else
    {
        qDebug() << "SetupPageLogs::updateSelectedLogs Not selected (less than 1)" << endl;
    }
}

int SetupPageLogs::getSelectedLog()
{
    QString selectedLog = currentLogs->currentText();
    int i = 0;
    QStringList qs;
    qs.clear();
    qs << selectedLog.split("-");
    i = (qs.at(0)).toInt();
    if (i>=1)
    {
        return i;
    }
    else
    {
        return 0;
    }
    return 0;
}

void SetupPageLogs::setSelectedLog(const int _i)
{

    QString n = QString::number(_i) + "--";
    int selected = currentLogs->findText(n, Qt::MatchStartsWith);
    if (selected > 0)
    {
        currentLogs->setCurrentIndex(selected);
    }
    else
    {
        return;
    }
}
