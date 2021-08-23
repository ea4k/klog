/***************************************************************************
                          setuppagelogs.cpp  -  description
                             -------------------
    begin                : feb 2012
    copyright            : (C) 2012 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

//#include <QDebug>

#include "setuppages/setuppagelogs.h"

SetupPageLogs::SetupPageLogs(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent){
       //qDebug() << "SetupPageLogs::SetupPageLogs" << QT_ENDL;
    dataProxy = dp;
    stationCallsign = QString();
    operators = QString();
    comment = QString();
    //dateString = QDate::currentDate().toString("yyyy/MM/dd");
    dateString = QString();
    //typeContest = QString();
    //contestCatMode = -1;
    //contestCatOperators = -1;
    //contestCatAssisted = -1;
    //contestCatPower = -1;
    //contestCatBands = -1;
    //contestBands = -1;
    //typeContestN = -1;

    selectedLog = -1;
    defaultStationCallSign.clear();

    //setupD = new SetupDialog();


    logsAvailable.clear();

    newLog = new SetupPageLogsNew(dataProxy);
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


    newLogPushButton->setToolTip(tr("Add a new log."));
    //loadAllPushButton->setToolTip(tr("Load all the logs"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected log"));
    //clearPushButton->setToolTip(tr("Clear selection"));
    editPushButton->setToolTip(tr("Edit the selected log."));
    removePushButton->setToolTip(tr("Remove the selected log."));



    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(newLogPushButton);
    buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(logsView);

    widgetLayout->addLayout(buttonsLayout);
    //widgetLayout->addLayout(logDataLayout);


    setLayout(widgetLayout);



    //connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );
    //QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
    //connect(setupD, SIGNAL(newLogRequested(true)), this, slotNewButtonClicked() )


    createActions();
    updateSelectedLogs();


       //qDebug() << "SetupPageLogs::SetupPageLogs - END" << QT_ENDL;
}

SetupPageLogs::~SetupPageLogs(){
       //qDebug() << "SetupPageLogs::~SetupPageLogs" << QT_ENDL;
}


void SetupPageLogs::createNewLog()
{
      //qDebug() << "SetupPageLogs::createNewLog" << QT_ENDL;
    selectedLog = -1;
    //newLog->clear();
    newLog->setEditing(false);
    if (defaultStationCallSign.length()>2)
    {
          //qDebug() << "SetupPageLogs::createNewLog-1" << QT_ENDL;
        newLog->setStationCallSign(defaultStationCallSign);
    }
    if (defaultOperators.length()>2)
    {
        newLog->setOperators(defaultOperators);
    }
    newLog->setDateString(QDate::currentDate().toString("yyyy-MM-dd"));
    newLog->setComment("");

    int result = newLog->exec();
      //qDebug() << "SetupPageLogs::createNewLog: result: " << QString::number(result) << QT_ENDL;
    if (result == QDialog::Accepted)
    {
          //qDebug() << "SetupPageLogs::createNewLog - Accepted, emitting focusOK" << QT_ENDL;
        emit focusOK();
    }
      //qDebug() << "SetupPageLogs::createNewLog - END" << QT_ENDL;
}

void SetupPageLogs::slotNewButtonClicked()
{
       //qDebug() << "SetupPageLogs::slotNewButtonClicked" << QT_ENDL;

    createNewLog();
}

void SetupPageLogs::slotEditButtonClicked()
{
       //qDebug() << "SetupPageLogs::slotEditButtonClicked" << QT_ENDL;
    //QSqlQuery query;
    //int nameCol = -1;

    //selectedLog = getSelectedLog();

    QString getStationCallSignFromLog(const int _log);

    newLog->setEditing(true);
      //qDebug() << "SetupPageLogs::slotEditButtonClicked" << QT_ENDL;
    newLog->setStationCallSign(dataProxy->getStationCallSignFromLog(selectedLog));
    newLog->setOperators(dataProxy->getOperatorsFromLog(selectedLog));
    newLog->setComment(dataProxy->getCommentsFromLog(selectedLog));
    newLog->setDateString(dataProxy->getLogDateFromLog(selectedLog));
    //newLog->setTypeN(dataProxy->getLogTypeNFromLog(selectedLog).toInt());
    int result = newLog->exec();
    if (result == QDialog::Accepted)
    {
        emit focusOK();
    }

/*
       //qDebug() << "SetupPageLogs::slotEditButtonClicked-1 (selectedlog: " << QString::number(selectedLog) << ")" << QT_ENDL;
    QString stringQuery = QString("SELECT * FROM logs WHERE id='%1'").arg(selectedLog);
       //qDebug() << "SetupPageLogs::slotEditButtonClicked -2" << QT_ENDL;

    bool sqlOk = query.exec(stringQuery);
    QSqlRecord rec = query.record();
    if (sqlOk)
    {
           //qDebug() << "SetupPageLogs::slotEditButtonClicked Query OK" << QT_ENDL;
        QSqlRecord rec = query.record();
        if ( (query.next()) && (query.isValid()) )
        {//id/logdate/stationcall/comment/logtype/logtypeid
               //qDebug() << "SetupPageLogs::slotEditButtonClicked Query Valid" << QT_ENDL;

            //nameCol = rec.indexOf("stationcall");
            //newLog->setStationCallSign((query.value(nameCol)).toString());
            //nameCol = rec.indexOf("operators");
            //newLog->setOperators((query.value(nameCol)).toString());

            //nameCol = rec.indexOf("comment");
            //newLog->setComment((query.value(nameCol)).toString());

            //nameCol = rec.indexOf("logdate");
            //newLog->setDateString((query.value(nameCol)).toString());

            //nameCol = rec.indexOf("logtypen");
               //qDebug() << "SetupPageLogs::slotEditButtonClicked -3" << QT_ENDL;
            //newLog->setTypeN((query.value(nameCol)).toInt());

            newLog->exec();
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    */
}

/*
void SetupPageLogs::slotLogsCancelled(const bool _q)
{
    selectedLog = -1;
}
*/

void SetupPageLogs::slotRemoveButtonClicked()
{
       //qDebug() << "SetupPageLogs::slotRemoveButtonClicked" << QT_ENDL;
    //int selectedLog = getSelectedLog();

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("KLog"),
             tr("Do you really want to remove this log?") + "\n" +
                tr("All the QSOs from this log will also be deleted..."),
             QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
           //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (selected log to remove: " << QString::number(selectedLog) << ")" << QT_ENDL;
        QString stringQuery = QString("DELETE FROM logs WHERE id='%1'").arg(selectedLog);
        QSqlQuery query(stringQuery);

        bool sqlOk = query.exec();
        if (sqlOk)
        {
               //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (REMOVED: " << QString::number(selectedLog) << ")" << QT_ENDL;
            logsModel->select();
            updateSelectedLogs();
            stringQuery = QString("DELETE FROM log WHERE lognumber='%1'").arg(selectedLog);
            query.exec(stringQuery);
            sqlOk = query.exec();
               //qDebug() << "SetupPageLogs::slotRemoveButtonClicked: LastQuery: " << query.lastQuery()  << QT_ENDL;
            if (sqlOk)
            {
                   //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (QSOS REMOVED: " << QString::number(selectedLog) << ")" << QT_ENDL;
                stringQuery = QString("DELETE FROM awarddxcc WHERE lognumber='%2'").arg(selectedLog);
                query.exec(stringQuery);
                sqlOk = query.exec();
                   //qDebug() << "SetupPageLogs::slotRemoveButtonClicked: LastQuery: " << query.lastQuery()  << QT_ENDL;
                if (sqlOk)
                {
                       //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (AWARDDXCC REMOVED: " << QString::number(selectedLog) << ")" << QT_ENDL;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                    showError(tr("Log has not been removed. (#3)"));
                       //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (AWARDDXCC NOT REMOVED: " << QString::number(selectedLog) << ")" << QT_ENDL;
                }

            }
            else
            {
                showError(tr("Log has not been removed. (#2)"));
                   //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (QSOS NOT REMOVED: " << QString::number(selectedLog) << ")" << QT_ENDL;
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            showError(tr("Log has not been removed. (#1)"));
               //qDebug() << "SetupPageLogs::slotRemoveButtonClicked (NOT REMOVED: " << QString::number(selectedLog) << ")" << QT_ENDL;
        }
    }

    //ASK FOR A CONFIRMATION

    //DELETE ALL THE QSO IN THE REMOVED LOG

}

void SetupPageLogs::createLogsPanel()
{
       //qDebug() << "SetupPageLogs::createLogsPanel" << QT_ENDL;
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
    columns = rec.indexOf("logtypen");
    logsView->setColumnHidden(columns, false);

    logsView->setItemDelegate(new QSqlRelationalDelegate(this));
    logsView->setSelectionMode( QAbstractItemView::SingleSelection);
    logsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    logsView->resizeColumnsToContents();
    logsView->horizontalHeader()->setStretchLastSection(true);

}

void SetupPageLogs::createLogsModel()
{
       //qDebug() << "SetupPageLogs::createLogsModel" << QT_ENDL;

        QString stringQuery = QString("SELECT * FROM logs");
        QSqlQuery q(stringQuery);
        QSqlRecord rec = q.record();

        int nameCol;

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

        nameCol = rec.indexOf("logtypen");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("QSOs"));

        logsModel->select();
}

void SetupPageLogs::slotLogSelected(const QModelIndex & index)
{

    int row = index.row();
    selectedLog = (logsModel->index(row, 0)).data(0).toInt();
    //qDebug() << "SetupPageLogs::slotLogSelected: " << QString::number(selectedLog)  << QT_ENDL;
    //setSelectedLog(selectedLog);

}

void SetupPageLogs::slotLogDoubleClicked(const QModelIndex & index)
{
      //qDebug() << "SetupPageLogs::slotLogDoubleClicked"  << QT_ENDL;

    int row = index.row();
    selectedLog = (logsModel->index(row, 0)).data(0).toInt();
    //setSelectedLog(selectedLog);
    slotEditButtonClicked();

}


void SetupPageLogs::createActions()
{
       //qDebug() << "SetupPageLogs::createActions" << QT_ENDL;
    //connect(currentLogs, SIGNAL(currentIndexChanged (int)), this, SLOT(slotCurrentLogsComboBoxChanged() ) ) ;
    connect(newLogPushButton, SIGNAL(clicked()), this, SLOT(slotNewButtonClicked() ) );
    connect(removePushButton, SIGNAL(clicked()), this, SLOT(slotRemoveButtonClicked() ) );
    connect(editPushButton, SIGNAL(clicked()), this, SLOT(slotEditButtonClicked() ) );
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
       //qDebug() << "SetupPageLogs::readLogs" << QT_ENDL;

    QString aux, aux2;
    QStringList _logs;
    QSqlQuery query;
    int nameCol = -1;
    bool sqlOk = false;
    //QDate date = QDate::currentDate();
    aux2.clear();
    aux.clear();
    _logs.clear();


    //aux = "SELECT id, logdate, stationcall, logtype FROM logs";
    aux = "SELECT id, logdate, stationcall FROM logs";

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

            //nameCol = rec.indexOf("logtype");
            //aux2 = aux2.append("-");
            //aux2.append((query.value(nameCol)).toString());

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return _logs;
    }

/*
    _logs.clear();
       //qDebug() << "SetupPageLogs::readLogs: " << QString::number(_logs.size())<< QT_ENDL;

    return _logs;
*/
}


void SetupPageLogs::slotAnalyzeNewLogData(const QStringList _qs)
{
       //qDebug() << "SetupPageLogs::slotAnalyzeNewLogData (length=" << QString::number(_qs.length()) << ")" << QT_ENDL;


    if (_qs.length()!=5)
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
   //typeContest  = _qs.at(4);
/*
    contestCatMode  = (_qs.at(5)).toInt();
    contestCatOperators  = (_qs.at(6)).toInt();
    contestCatAssisted  = (_qs.at(7)).toInt();
    contestCatPower  = (_qs.at(8)).toInt();
    contestCatBands  = (_qs.at(9)).toInt();
    contestBands  = (_qs.at(10)).toInt();
    typeContestN = (_qs.at(12)).toInt();
*/
    /*
    bool editing;
    if ( (_qs.at(4)).toInt() == 1)
    {
        editing = true;
    }
    else
    {
        editing = false;
    }
    */
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
    //newLogq << dateString << stationCallsign << operators << typeContest << comment << "1" << QString::number(selectedLog) << _qs.at(13) ;
    newLogq << dateString << stationCallsign << operators << comment << QString::number(selectedLog) << _qs.at(4) ;

    if (dataProxy->addNewLog(newLogq))
    {
        logsModel->select();
        updateSelectedLogs();
    }
    else
    {
        showError(tr("The new log could not be created."));
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
       //qDebug() << "SetupPageLogs::addNewLog: " << _qs.at(2) << QT_ENDL;
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
       //qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << QT_ENDL;
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    QSqlRecord rec = query.record(); // Number of columns

    while ( (query.next()) && (query.isValid()) )
    {
        nameCol = rec.indexOf("id");
        aux = (query.value(nameCol)).toString();
           //qDebug() << "SetupPageLogs::addNewLog: id = " << aux << QT_ENDL;
        return false;
    }
    queryString = QString("INSERT INTO logs (logdate, stationcall, comment, logtype, logtypen) values('%1','%2','%3','%4', '%5')").arg(_dateString).arg(_stationCallsign).arg(_comment).arg(_typeContest).arg(_typeContestN);

       //qDebug() << "SetupPageLogs::addNewLog query1: " << queryString << QT_ENDL;
    sqlOK = query.exec(queryString);
    if (sqlOK)
    {
           //qDebug() << "SetupPageLogs::addNewLog ADDED! id = "  << QT_ENDL;
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
       //qDebug() << "SetupPageLogs::updateSelectedLogs" << QT_ENDL;
    logsAvailable = readLogs();

   /*
     if (logsAvailable.length()>0)
    {
        currentLogs->clear();
        currentLogs->addItems(logsAvailable);
    }
    else
    {
           //qDebug() << "SetupPageLogs::updateSelectedLogs Not selected (less than 1)" << QT_ENDL;
        currentLogs->clear();
    }
    */
}

int SetupPageLogs::getSelectedLog()
{
      //qDebug() << "SetupPageLogs::getSelectedLog: " << currentLogs->currentText() << QT_ENDL;
    return selectedLog;
    /*
    QString selectedLog = currentLogs->currentText();
    int i = 0;
    QStringList qs;
    qs.clear();
    qs << selectedLog.split("-");
    i = (qs.at(0)).toInt();
    //qDebug() << "SetupPageLogs::getSelectedLog: " << QString::number(i) << QT_ENDL;
    if (i>=1)
    {
        return i;
    }
    else
    {
        return 0;
    }
    */
}



void SetupPageLogs::setSelectedLog(const int _i)
{
    //qDebug() << "SetupPageLogs::SetupPageLogs::setSelectedLog: " << QString::number(_i) << QT_ENDL;
    //QString n = QString::number(_i) + "-";
    //selectedLog = (logsModel->index(row, 0)).data(0).toInt();

    selectedLog = _i;
    logsView->selectRow(1);

    QModelIndex index = logsModel->index(selectedLog, 0);

    logsView->setCurrentIndex(index);

}

/*
void SetupPageLogs::readSelectedLog(const int _i)
{


}
*/
void SetupPageLogs::showError(const QString &_errorC)
{
    QString text = QString(tr("An error has occurred showing the following error code:") + "\n'%1'").arg(_errorC);


    QMessageBox::warning(this, tr("KLog - SetupPageLogs"),
                                   text,
                                   QMessageBox::Ok);

}

void SetupPageLogs::setDefaultStationCallsign(const QString &_p)
{
       //qDebug() << "SetupPageLogs::setDefaultStationCallsign: " << _p << QT_ENDL;
    defaultStationCallSign = _p;
}

void SetupPageLogs::setDefaultOperators(const QString &_p)
{
       //SetupPageLogs
    defaultOperators = _p;

}

void SetupPageLogs::showEvent(QShowEvent *event)
{
    //qDebug() << Q_FUNC_INFO << QT_ENDL;

    QWidget::showEvent(event);
    dataProxy->updateQSONumberPerLog();
    logsModel->select();
    //logsView->selectRow(1);


    /*
    int row = index.row();
    selectedLog = (logsModel->index(row, 0)).data(0).toInt();
    */


    //setSelectedLog(selectedLog);
}
