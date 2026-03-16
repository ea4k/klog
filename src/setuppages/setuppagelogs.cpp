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

#include "setuppagelogs.h"

SetupPageLogs::SetupPageLogs(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent){
   //qDebug() << Q_FUNC_INFO << " - Start";
    dataProxy = dp;
    stationCallsign = QString();
    operators = QString();
    comment = QString();
    dateString = QString();
    selectedLog = -1;
    defaultStationCallSign.clear();
    logsAvailable.clear();
    logsModified = false;   // initialise flag
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
    editPushButton->setToolTip(tr("Edit the selected log."));
    removePushButton->setToolTip(tr("Remove the selected log."));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(newLogPushButton);
    buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(logsView);

    widgetLayout->addLayout(buttonsLayout);

    setLayout(widgetLayout);

    createActions();
    updateSelectedLogs();
   //qDebug() << "SetupPageLogs::SetupPageLogs - END";
}

SetupPageLogs::~SetupPageLogs(){
   //qDebug() << Q_FUNC_INFO << " - Start";
}


void SetupPageLogs::createNewLog()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    selectedLog = -1;
    newLog->setEditing(false);
    if (defaultStationCallSign.length()>2)
    {
       //qDebug() << "SetupPageLogs::createNewLog-1";
        newLog->setStationCallSign(defaultStationCallSign);
    }
    if (defaultOperators.length()>2)
    {
        newLog->setOperators(defaultOperators);
    }
    newLog->setDateString(QDate::currentDate().toString("yyyy-MM-dd"));
    newLog->setComment("");

    int result = newLog->exec();
   //qDebug() << "SetupPageLogs::createNewLog: result: " << QString::number(result);
    if (result == QDialog::Accepted)
    {
       //qDebug() << "SetupPageLogs::createNewLog - Accepted, emitting focusOK";
        emit focusOK();
    }
   //qDebug() << "SetupPageLogs::createNewLog - END";
}

void SetupPageLogs::slotNewButtonClicked()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    createNewLog();
}

void SetupPageLogs::slotEditButtonClicked()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    // qSqlQuery query;
    //int nameCol = -1;
    //selectedLog = getSelectedLog();
    // qString call = getStationCallSignFromLog(const int _log);

    newLog->setEditing(true);

    newLog->setStationCallSign(dataProxy->getStationCallSignFromLog(selectedLog));
    newLog->setOperators(dataProxy->getOperatorsFromLog(selectedLog));
  //qDebug() << Q_FUNC_INFO ;
    newLog->setComment(dataProxy->getCommentsFromLog(selectedLog));
    newLog->setDateString(dataProxy->getLogDateFromLog(selectedLog));
    //newLog->setTypeN(dataProxy->getLogTypeNFromLog(selectedLog).toInt());
    int result = newLog->exec();
    if (result == QDialog::Accepted)
    {
        logsModified = true;
        emit focusOK();
    }
}


void SetupPageLogs::slotRemoveButtonClicked()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    //int selectedLog = getSelectedLog();

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("KLog"),
             tr("Do you really want to remove this log?") + "\n" +
                tr("All the QSOs from this log will also be deleted..."),
             QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
       //qDebug() << Q_FUNC_INFO << " -  (selected log to remove: " << QString::number(selectedLog) << ")";
        QString stringQuery = QString("DELETE FROM logs WHERE id='%1'").arg(selectedLog);
        QSqlQuery query(stringQuery);

        bool sqlOk = query.exec();
        if (sqlOk)
        {
           //qDebug() << Q_FUNC_INFO << " -v(REMOVED: " << QString::number(selectedLog) << ")";
            logsModified = true;
            logsModel->select();
            updateSelectedLogs();
            stringQuery = QString("DELETE FROM log WHERE lognumber='%1'").arg(selectedLog);
            sqlOk = query.exec(stringQuery);
          //qDebug() << Q_FUNC_INFO << " - LastQuery: " << query.lastQuery() ;
            if (!sqlOk)
            {
                showError(tr("Log has not been removed. (#2)"));
               //qDebug() << Q_FUNC_INFO << " - (QSOS NOT REMOVED: " << QString::number(selectedLog) << ")";
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            showError(tr("Log has not been removed. (#1)"));
           //qDebug() << Q_FUNC_INFO << " -  (NOT REMOVED: " << QString::number(selectedLog) << ")";
        }
    }

    //ASK FOR A CONFIRMATION
    //DELETE ALL THE QSO IN THE REMOVED LOG
}

void SetupPageLogs::createLogsPanel()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
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
   //qDebug() << Q_FUNC_INFO << " - Start";
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
   //qDebug() << Q_FUNC_INFO << " - Start";
    int row = index.row();
    selectedLog = (logsModel->index(row, 0)).data(0).toInt();
   //qDebug() << Q_FUNC_INFO << " - " << QString::number(selectedLog) ;
}

void SetupPageLogs::slotLogDoubleClicked(const QModelIndex & index)
{
   //qDebug() << Q_FUNC_INFO << " - Start";

    int row = index.row();
    selectedLog = (logsModel->index(row, 0)).data(0).toInt();
    slotEditButtonClicked();
}


void SetupPageLogs::createActions()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    //connect(currentLogs, SIGNAL(currentIndexChanged (int)), this, SLOT(slotCurrentLogsComboBoxChanged() ) ) ;
    connect(newLogPushButton, SIGNAL(clicked()), this, SLOT(slotNewButtonClicked() ) );
    connect(removePushButton, SIGNAL(clicked()), this, SLOT(slotRemoveButtonClicked() ) );
    connect(editPushButton, SIGNAL(clicked()), this, SLOT(slotEditButtonClicked() ) );
    connect(newLog, SIGNAL(newLogData(QStringList)), this, SLOT(slotAnalyzeNewLogData(QStringList) ) );
    connect(logsView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotLogSelected(QModelIndex) ) );
    connect(logsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotLogDoubleClicked(QModelIndex) ) );
}

QStringList SetupPageLogs::readLogs()
{
   //qDebug() << Q_FUNC_INFO << " - Start";

    QString aux, aux2;
    QStringList _logs;
    QSqlQuery query;
    int nameCol = -1;
    bool sqlOk = false;
    // qDate date = QDate::currentDate();
    aux2.clear();
    aux.clear();
    _logs.clear();

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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return _logs;
    }
}


void SetupPageLogs::slotAnalyzeNewLogData(const QStringList _qs)
{
   //qDebug() << Q_FUNC_INFO << " -  (length=" << QString::number(_qs.length()) << ")";
    if (_qs.length()!=5)
    {
        return;
    }

    stationCallsign = _qs.at(0);
    operators = _qs.at(1);
    comment = _qs.at(2);
    dateString = _qs.at(3);
    //TODO: Check that those vars have received valid data (calls, date, ...)

    QStringList newLogq;
    newLogq.clear();

    newLogq << dateString << stationCallsign << operators << comment << QString::number(selectedLog) << _qs.at(4) ;
   //qDebug() << Q_FUNC_INFO << " - 020 - selectedLog << " << selectedLog;
    if (dataProxy->addNewLog(newLogq))
    {
        logsModel->select();
        updateSelectedLogs();
       //qDebug() << Q_FUNC_INFO << " - 021 - selectedLog << " << selectedLog;
        // When creating a new log (not editing), update selectedLog to the newly created ID
        // so getSelectedLog() returns a valid value and currentLog is not left at -1 (Closes #906)
        if (_qs.at(4) == "0")
        {
            selectedLog = dataProxy->getMaxLogNumber();
           //qDebug() << Q_FUNC_INFO << " - 022 - selectedLog << " << selectedLog;
            logsModified = true;
        }
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - 023 - selectedLog << " << selectedLog;
        showError(tr("The new log could not be created."));
    }

   //qDebug() << Q_FUNC_INFO << " - 030 - selectedLog << " << selectedLog;
    // We send the data to the main tab
    QStringList logData;
    logData.clear();
    logData << stationCallsign << operators  ;
    emit newLogData(logData);
}

void SetupPageLogs::updateSelectedLogs()
{
   //qDebug() << Q_FUNC_INFO;
    logsAvailable = readLogs();
}

int SetupPageLogs::getSelectedLog()
{
   //qDebug() << Q_FUNC_INFO;
    return selectedLog;
}

void SetupPageLogs::showError(const QString &_errorC)
{
   //qDebug() << Q_FUNC_INFO;
    QString text = QString(tr("An error has occurred showing the following error code:") + "\n'%1'").arg(_errorC);


    QMessageBox::warning(this, tr("KLog - SetupPageLogs"),
                                   text,
                                   QMessageBox::Ok);
}

void SetupPageLogs::setDefaultStationCallsign(const QString &_p)
{
   //qDebug() << Q_FUNC_INFO << " - Start: " << _p;
    defaultStationCallSign = _p;
}

void SetupPageLogs::setDefaultOperators(const QString &_p)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    defaultOperators = _p;
}

void SetupPageLogs::showEvent(QShowEvent *event)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    QWidget::showEvent(event);
    dataProxy->updateQSONumberPerLog();
    logsModel->select();
}

void SetupPageLogs::saveSettings()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getCfgFile (), QSettings::IniFormat);
    //settings.beginGroup ("Logs");
    settings.setValue ("SelectedLog", selectedLog);
   //qDebug() << Q_FUNC_INFO << " - End";
}

void SetupPageLogs::loadSettings()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    Utilities util(Q_FUNC_INFO);
   //qDebug() << Q_FUNC_INFO << " - 00";
    QSettings settings(util.getCfgFile (), QSettings::IniFormat);
   //qDebug() << Q_FUNC_INFO << " - 01";
    int i = settings.value("SelectedLog").toInt();
   //qDebug() << Q_FUNC_INFO << " - 02";
    if (!dataProxy->doesThisLogExist(i))
    {
       //qDebug() << Q_FUNC_INFO << " - 10";
        i = 0;
        while((!dataProxy->doesThisLogExist(i)) && (i <500))
        {//TODO If a user has more than 500 logs it may fail...
            i++;
           //qDebug() << Q_FUNC_INFO << " - Log: " << QString::number(i);
        }
       //qDebug() << Q_FUNC_INFO << " - 49";
    }
   //qDebug() << Q_FUNC_INFO << " - 50";
    selectedLog = i;
   //qDebug() << Q_FUNC_INFO << " - 51";
    logsView->selectRow(1);
   //qDebug() << Q_FUNC_INFO << " - 52";
    QModelIndex index = logsModel->index(selectedLog, 0);
   //qDebug() << Q_FUNC_INFO << " - 53";
    logsView->setCurrentIndex(index);
    logsModified = false;
   //qDebug() << Q_FUNC_INFO << " - END";
}

bool SetupPageLogs::hasSettingsChanged() const
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return logsModified;
}
