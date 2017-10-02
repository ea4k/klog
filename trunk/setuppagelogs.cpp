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
    //dateString = QDate::currentDate().toString("yyyy/MM/dd");
    dateString = QString();
    typeContest = QString();
    contestCatMode = -1;
    contestCatOperators = -1;
    contestCatAssisted = -1;
    contestCatPower = -1;
    contestCatBands = -1;
    contestBands = -1;
    typeContestN = -1;

    selectedLog = -1;
    defaultStationCallSign.clear();

    //setupD = new SetupDialog();

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


    newLogPushButton = new QPushButton(tr("&New"), this);
    editPushButton = new QPushButton(tr("&Edit"), this);
    removePushButton = new QPushButton(tr("&Remove"), this);


    newLogPushButton->setToolTip(tr("Add a new log"));
    //loadAllPushButton->setToolTip(tr("Load all the logs"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected log"));
    //clearPushButton->setToolTip(tr("Clear selection"));
    editPushButton->setToolTip(tr("Edit the selected log"));
    removePushButton->setToolTip(tr("Remove the selected log"));

    currentLogs->setToolTip(tr("Select the log you want to open"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(newLogPushButton);
    buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(logsView);
    widgetLayout->addWidget(currentLogs);
    widgetLayout->addLayout(buttonsLayout);
    //widgetLayout->addLayout(logDataLayout);


    setLayout(widgetLayout);

    dataProxy = new DataProxy_SQLite();

    //connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );
    //QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
    //connect(setupD, SIGNAL(newLogRequested(true)), this, slotNewButtonClicked() )


    createActions();
    updateSelectedLogs();



}

SetupPageLogs::~SetupPageLogs(){
    //qDebug() << "SetupPageLogs::~SetupPageLogs" << endl;
}


void SetupPageLogs::createNewLog()
{
   //qDebug() << "SetupPageLogs::createNewLog" << endl;
    selectedLog = -1;
    //newLog->clear();
    newLog->setEditing(false);
    if (defaultStationCallSign.length()>2)
    {
        newLog->setStationCallSign(defaultStationCallSign);
    }
    if (defaultOperators.length()>2)
    {
        newLog->setOperators(defaultOperators);
    }
    newLog->setDateString(QDate::currentDate().toString("yyyy/MM/dd"));
    newLog->setComment("");

    newLog->exec();
}

void SetupPageLogs::slotNewButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotNewButtonClicked" << endl;

    createNewLog();
}

void SetupPageLogs::slotEditButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotEditButtonClicked" << endl;
    QSqlQuery query;
    int nameCol = -1;

    selectedLog = getSelectedLog();
    //qDebug() << "SetupPageLogs::slotEditButtonClicked-1 (selectedlog: " << QString::number(selectedLog) << ")" << endl;
    QString stringQuery = QString("SELECT * FROM logs WHERE id='%1'").arg(selectedLog);
    //qDebug() << "SetupPageLogs::slotEditButtonClicked -2" << endl;

    bool sqlOk = query.exec(stringQuery);
    QSqlRecord rec = query.record();
    if (sqlOk)
    {
        //qDebug() << "SetupPageLogs::slotEditButtonClicked Query OK" << endl;
        QSqlRecord rec = query.record();
        if ( (query.next()) && (query.isValid()) )
        {//id/logdate/stationcall/comment/logtype/logtypeid
            //qDebug() << "SetupPageLogs::slotEditButtonClicked Query Valid" << endl;

            newLog->setEditing(true);

            nameCol = rec.indexOf("stationcall");
            newLog->setStationCallSign((query.value(nameCol)).toString());

            nameCol = rec.indexOf("operators");
            newLog->setOperators((query.value(nameCol)).toString());

            nameCol = rec.indexOf("comment");
            newLog->setComment((query.value(nameCol)).toString());

            nameCol = rec.indexOf("logdate");
            newLog->setDateString((query.value(nameCol)).toString());

            nameCol = rec.indexOf("logtypen");
            //qDebug() << "SetupPageLogs::slotEditButtonClicked -3" << endl;
            newLog->setTypeN((query.value(nameCol)).toInt());

            newLog->exec();
        }

    }
    else
    {

    }
}

void SetupPageLogs::slotLogsCancelled(const bool _q)
{
    selectedLog = -1;
}

void SetupPageLogs::slotRemoveButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotRemoveButtonClicked" << endl;
    int selectedLog = getSelectedLog();

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("KLog"),
             tr("Do you really want to remove this log?") + "\n" +
                tr("All the QSOs from this log will be also deleted..."),
             QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (selected log to remove: " << QString::number(selectedLog) << ")" << endl;
        QString stringQuery = QString("DELETE FROM logs WHERE id='%1'").arg(selectedLog);
        QSqlQuery query(stringQuery);

        bool sqlOk = query.exec();
        if (sqlOk)
        {
            //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (REMOVED: " << QString::number(selectedLog) << ")" << endl;
            logsModel->select();
            updateSelectedLogs();
            stringQuery = QString("DELETE FROM log WHERE lognumber='%1'").arg(selectedLog);
            query.exec(stringQuery);
            sqlOk = query.exec();
            //qDebug() << "SetupPageLogs::slotRemoveButtonClicked: LastQuery: " << query.lastQuery()  << endl;
            if (sqlOk)
            {
                //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (QSOS REMOVED: " << QString::number(selectedLog) << ")" << endl;
                stringQuery = QString("DELETE FROM awarddxcc WHERE lognumber='%2'").arg(selectedLog);
                query.exec(stringQuery);
                sqlOk = query.exec();
                //qDebug() << "SetupPageLogs::slotRemoveButtonClicked: LastQuery: " << query.lastQuery()  << endl;
                if (sqlOk)
                {
                    //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (AWARDDXCC REMOVED: " << QString::number(selectedLog) << ")" << endl;

                }
                else
                {
                    showError(tr("Log has not been removed. (#3)"));
                    //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (AWARDDXCC NOT REMOVED: " << QString::number(selectedLog) << ")" << endl;
                }


            }
            else
            {
                showError(tr("Log has not been removed. (#2)"));
                //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (QSOS NOT REMOVED: " << QString::number(selectedLog) << ")" << endl;
            }
        }
        else
        {
            showError(tr("Log has not been removed. (#1)"));
            //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (NOT REMOVED: " << QString::number(selectedLog) << ")" << endl;
        }

    }



    //ASK FOR A CONFIRMATION

    //DELETE ALL THE QSO IN THE REMOVED LOG

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
    columns = rec.indexOf("operators");
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

        nameCol = rec.indexOf("operators");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Operators"));

        nameCol = rec.indexOf("comment");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Comments"));

        nameCol = rec.indexOf("logtype");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Type"));

        logsModel->select();
}

void SetupPageLogs::slotLogSelected(const QModelIndex & index)
{
    //qDebug() << "SetupPageLogs::slotLogSelected"  << endl;
    int row = index.row();
    setSelectedLog((logsModel->index(row, 0)).data(0).toInt());

}

void SetupPageLogs::slotLogDoubleClicked(const QModelIndex & index)
{
    //qDebug() << "SetupPageLogs::slotLogDoubleClicked"  << endl;

    int row = index.row();
    setSelectedLog((logsModel->index(row, 0)).data(0).toInt());
    slotEditButtonClicked();

}

void SetupPageLogs::createActions()
{
    //qDebug() << "SetupPageLogs::createActions" << endl;
    connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );
    connect(removePushButton, SIGNAL(clicked ( )), this, SLOT(slotRemoveButtonClicked() ) );
    connect(editPushButton, SIGNAL(clicked ( )), this, SLOT(slotEditButtonClicked() ) );
    connect(newLog, SIGNAL(newLogData(QStringList)), this, SLOT(slotAnalyzeNewLogData(QStringList) ) );
    connect(logsView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotLogSelected(QModelIndex) ) );
    connect(logsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotLogDoubleClicked(QModelIndex) ) );
    //connect(logView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickLog( const QModelIndex& ) ) );

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
    //QDate date = QDate::currentDate();
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

            nameCol = rec.indexOf("stationcall");
            aux2 = aux2.append("-");
            aux2.append((query.value(nameCol)).toString());

            nameCol = rec.indexOf("logtype");
            aux2 = aux2.append("-");
            aux2.append((query.value(nameCol)).toString());

            nameCol = rec.indexOf("logdate");
            aux2 = aux2.append(" (");
            aux2.append((query.value(nameCol)).toString());
            aux2 = aux2.append(")");

            _logs.append(aux2);

        }
        return _logs;
    }
    else
    {
        return _logs;
    }


    _logs.clear();
    //qDebug() << "SetupPageLogs::readLogs: " << QString::number(_logs.size())<< endl;

    return _logs;

}


void SetupPageLogs::slotAnalyzeNewLogData(const QStringList _qs)
{
    //qDebug() << "SetupPageLogs::slotAnalyzeNewLogData (length=" << QString::number(_qs.length()) << ")" << endl;


    if (_qs.length()!=14)
    {
        return;
    }
/*     From SetupPageLogsNew::gatherAndSend()
    logData.clear();
    logData << stationCallsign << operators << comment << dateString
            << typeConteststr
            << QString::number(contestCatMode)
            << QString::number(contestCatOperators)
            << QString::number(contestCatAssisted)
            << QString::number(contestCatPower)
            << QString::number(contestCatBands)
            << QString::number(contestBands)
            << QString::number(contestCatOverlay)
            << QString::number(typeContest);
            << editing (1/0)


*/
    stationCallsign = _qs.at(0);
    operators = _qs.at(1);
    comment = _qs.at(2);
    dateString = _qs.at(3);
    typeContest  = _qs.at(4);
/*
    contestCatMode  = (_qs.at(5)).toInt();
    contestCatOperators  = (_qs.at(6)).toInt();
    contestCatAssisted  = (_qs.at(7)).toInt();
    contestCatPower  = (_qs.at(8)).toInt();
    contestCatBands  = (_qs.at(9)).toInt();
    contestBands  = (_qs.at(10)).toInt();
    typeContestN = (_qs.at(12)).toInt();
*/
    bool editing;
    if ( (_qs.at(13)).toInt() == 1)
    {
        editing = true;
    }
    else
    {
        editing = false;
    }
    //OVERLAY = 11
/*
    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);
    QString _operators = _qs.at(2);

    QString _typeContest = _qs.at(3);
    QString _comment = _qs.at(4);
    QString _typeContestN = _qs.at(5);
    QString id = _qs.at(6);
    QString editing = _qs.at(7);
*/

    QStringList newLogq;
    newLogq.clear();
    //If qs.at(12) == 1 then we are editing, any other value is a new log
    //Date/Call/Operators/"DX"/comment/"1"
    newLogq << dateString << stationCallsign << operators << typeContest << comment << "1" << QString::number(selectedLog) << _qs.at(13) ;

    if (dataProxy->addNewLog(newLogq))
    {
        logsModel->select();
        updateSelectedLogs();
    }

    // We send the data to the main tab
    QStringList logData;
    logData.clear();
    logData << stationCallsign << operators  ;
    emit newLogData(logData);
}
/*
bool SetupPageLogs::addNewLog(const QStringList _qs)
{
    //qDebug() << "SetupPageLogs::addNewLog: " << _qs.at(2) << endl;
    QString aux = QString();
    int nameCol = -1;

    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);
    QString _typeContest = _qs.at(2);
    QString _comment = _qs.at(3);
    QString _typeContestN = _qs.at(4);


    QString queryString = QString("SELECT * FROM logs WHERE logdate='%1' AND stationcall='%2' AND logtype='%3' AND logtypen='%4'").arg(_dateString).arg(_stationCallsign).arg(_typeContest).arg(_typeContestN);
    //"logs"
    //"id, logdate, stationcall, comment, logtype"
    //qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << endl;
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    QSqlRecord rec = query.record(); // Number of columns

    while ( (query.next()) && (query.isValid()) )
    {
        nameCol = rec.indexOf("id");
        aux = (query.value(nameCol)).toString();
        //qDebug() << "SetupPageLogs::addNewLog: id = " << aux << endl;
        return false;
    }
    queryString = QString("INSERT INTO logs (logdate, stationcall, comment, logtype, logtypen) values('%1','%2','%3','%4', '%5')").arg(_dateString).arg(_stationCallsign).arg(_comment).arg(_typeContest).arg(_typeContestN);

    //qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << endl;
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
*/
void SetupPageLogs::updateSelectedLogs()
{
    //qDebug() << "SetupPageLogs::updateSelectedLogs" << endl;
    logsAvailable = readLogs();

    if (logsAvailable.length()>0)
    {
        currentLogs->clear();
        currentLogs->addItems(logsAvailable);
    }
    else
    {
        //qDebug() << "SetupPageLogs::updateSelectedLogs Not selected (less than 1)" << endl;
        currentLogs->clear();
    }
}

int SetupPageLogs::getSelectedLog()
{
   //qDebug() << "SetupPageLogs::getSelectedLog: " << currentLogs->currentText() << endl;
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
    //qDebug() << "SetupPageLogs::SetupPageLogs::setSelectedLog: " << QString::number(_i) << endl;

    QString n = QString::number(_i) + "--";
    int selected = currentLogs->findText(n, Qt::MatchStartsWith);
    if (selected >= 0)
    {
        //qDebug() << "SetupPageLogs::SetupPageLogs::setSelectedLog selected>0: " << QString::number(selected) << endl;
        currentLogs->setCurrentIndex(selected);
    }
    else
    {
        //qDebug() << "SetupPageLogs::SetupPageLogs::setSelectedLog not selcted" << endl;
        return;
    }
}


void SetupPageLogs::readSelectedLog(const int _i)
{

    /*
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
*/
}

void SetupPageLogs::showError(const QString _errorC)
{
    QString text = QString(tr("An error has occurred showing the following error code:") + "\n'%1'").arg(_errorC);


    QMessageBox::warning(this, tr("KLog - SetupPageLogs"),
                                   text,
                                   QMessageBox::Ok);

}

void SetupPageLogs::setDefaultStationCallsign(const QString _p)
{
    //qDebug() << "SetupPageLogs::setDefaultStationCallsign: " << _p << endl;
    defaultStationCallSign = _p;
}

void SetupPageLogs::setDefaultOperators(const QString _p)
{
    //qDebug() << "SetupPageLogs::setDefaultOperators: " << _p << endl;
    defaultOperators = _p;

}
