/***************************************************************************
                          setuppageregionalawards.cpp  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
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

#include "setuppagesubdivisions.h"


SetupPageSubdivisions::SetupPageSubdivisions(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent){
       //qDebug() << "SetupPageSubdivisions::SetupPageSubdivisions";
    dataProxy = dp;
    fileAwardManager = new FileAwardManager(dataProxy, Q_FUNC_INFO);
    //util = new Utilities(Q_FUNC_INFO);
    selectedLog = -1;
    currentLogs = new QComboBox();
    logsAvailable.clear();

    logsModel = new QSqlRelationalTableModel(this);
    logsView = new QTableView;
    logsView->setContextMenuPolicy(Qt::CustomContextMenu);
    logsView->setSortingEnabled(true);

    createLogsModel();
    createLogsPanel();

    logsView->setCurrentIndex(logsModel->index(0, 0));

    lastLog = 0;


    //newLogPushButton = new QPushButton(tr("&New"), this);
    //editPushButton = new QPushButton(tr("&Edit"), this);
    removePushButton = new QPushButton(tr("&Remove"), this);
    importAwardPushButton = new QPushButton(tr("Import new"));
    importAwardPushButton->setToolTip(tr("Import an AWA file with the subdivision details."));
    //newLogPushButton->setToolTip(tr("Add a new references file."));
    //editPushButton->setToolTip(tr("Edit the selected references."));
    removePushButton->setToolTip(tr("Remove the selected references."));

    currentLogs->setToolTip(tr("Select the references you want to open."));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(importAwardPushButton);
    //buttonsLayout->addWidget(newLogPushButton);
    //buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(logsView);
    widgetLayout->addWidget(currentLogs);
    widgetLayout->addLayout(buttonsLayout);

    setLayout(widgetLayout);

    createActions();
    updateSelectedLogs();


       //qDebug() << "SetupPageSubdivisions::SetupPageSubdivisions - END";
}

SetupPageSubdivisions::~SetupPageSubdivisions(){
       //qDebug() << "SetupPageSubdivisions::~SetupPageSubdivisions";
    delete(fileAwardManager);
}

/*
void SetupPageSubdivisions::slotEditButtonClicked()
{
       //qDebug() << "SetupPageSubdivisions::slotEditButtonClicked";
    //QSqlQuery query;
    //int nameCol = -1;

    selectedLog = getSelectedLog();

    QString getStationCallSignFromLog(const int _log);

    newLog->setEditing(true);
      //qDebug() << "SetupPageSubdivisions::slotEditButtonClicked";
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


}
*/

void SetupPageSubdivisions::slotRemoveButtonClicked()
{
       //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked";
    int selectedLog = getSelectedLog();

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("KLog"),
             tr("Do you really want to remove the data of this entity?") + "\n" +
                tr("All the subdivision information for this entity will be deleted..."),
             QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
           //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked (selected log to remove: " << QString::number(selectedLog) << ")";
        QString stringQuery = QString("DELETE FROM logs WHERE id='%1'").arg(selectedLog);
        QSqlQuery query(stringQuery);

        bool sqlOk = query.exec();
        if (sqlOk)
        {
               //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked (REMOVED: " << QString::number(selectedLog) << ")";
            logsModel->select();
            updateSelectedLogs();
            stringQuery = QString("DELETE FROM log WHERE lognumber='%1'").arg(selectedLog);
            query.exec(stringQuery);
            sqlOk = query.exec();
               //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked: LastQuery: " << query.lastQuery() ;
            if (sqlOk)
            {
                   //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked (QSOS REMOVED: " << QString::number(selectedLog) << ")";
                stringQuery = QString("DELETE FROM award WHERE lognumber='%2'").arg(selectedLog);
                query.exec(stringQuery);
                sqlOk = query.exec();
                   //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked: LastQuery: " << query.lastQuery() ;
                if (sqlOk)
                {
                       //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked (AWARDDXCC REMOVED: " << QString::number(selectedLog) << ")";
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                    showError(tr("Log has not been removed. (#3)"));
                       //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked (AWARDDXCC NOT REMOVED: " << QString::number(selectedLog) << ")";
                }
            }
            else
            {
                showError(tr("Log has not been removed. (#2)"));
                   //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked (QSOS NOT REMOVED: " << QString::number(selectedLog) << ")";
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            showError(tr("Log has not been removed. (#1)"));
               //qDebug() << "SetupPageSubdivisions::slotRemoveButtonClicked (NOT REMOVED: " << QString::number(selectedLog) << ")";
        }
    }

    //ASK FOR A CONFIRMATION

    //DELETE ALL THE QSO IN THE REMOVED LOG
}

void SetupPageSubdivisions::createLogsPanel()
{
       //qDebug() << "SetupPageSubdivisions::createLogsPanel";
    logsView->setModel(logsModel);
    QString stringQuery = QString("SELECT * FROM primary_subdivision");
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record(); // Number of columns
    int columns = rec.count();

    for (int i = 0; i < columns; i++ ){
        logsView->setColumnHidden(i, true);
    }

    //columns = rec.indexOf("id");
    //logsView->setColumnHidden(columns, false);


    logsView->setItemDelegate(new QSqlRelationalDelegate(this));
    logsView->setSelectionMode( QAbstractItemView::SingleSelection);
    logsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    logsView->resizeColumnsToContents();
    logsView->horizontalHeader()->setStretchLastSection(true);
}

void SetupPageSubdivisions::createLogsModel()
{
       //qDebug() << "SetupPageSubdivisions::createLogsModel";

        QString stringQuery = QString("SELECT * FROM primary_subdivisions");
        QSqlQuery q(stringQuery);
        QSqlRecord rec = q.record();

        int nameCol;

        logsModel->setTable("primary_subdivisions");

        nameCol = rec.indexOf("id");
        logsModel->setSort(nameCol, Qt::AscendingOrder);
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("ID"));

        nameCol = rec.indexOf("name");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Name"));

        nameCol = rec.indexOf("shortname");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Short Name"));

        nameCol = rec.indexOf("cqz");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("CQ Zone"));

        nameCol = rec.indexOf("ituz");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("ITU Zone"));

        nameCol = rec.indexOf("deleted");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Deleted"));

        nameCol = rec.indexOf("start_date");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Start Date"));

        nameCol = rec.indexOf("end_date");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("End Date"));

        nameCol = rec.indexOf("dxcc");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("DXCC"));
        logsModel->select();
}

void SetupPageSubdivisions::slotLogSelected(const QModelIndex & index)
{
       //qDebug() << "SetupPageSubdivisions::slotLogSelected" ;
    int row = index.row();
    setSelectedLog((logsModel->index(row, 0)).data(0).toInt());
}

void SetupPageSubdivisions::createActions()
{
       //qDebug() << "SetupPageSubdivisions::createActions";
    connect(currentLogs, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCurrentLogsComboBoxChanged() ) ) ;
    connect(importAwardPushButton, SIGNAL(clicked()), this, SLOT(slotImportButtonClicked() ) );

    connect(removePushButton, SIGNAL(clicked()), this, SLOT(slotRemoveButtonClicked() ) );
    connect(logsView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotLogSelected(QModelIndex) ) );
}

QStringList SetupPageSubdivisions::readLogs()
{
       //qDebug() << "SetupPageSubdivisions::readLogs";
    QString aux, aux2;
    QStringList _logs;
    QSqlQuery query;
    int nameCol = -1;
    bool sqlOk = false;
    aux2.clear();
    aux.clear();
    _logs.clear();

    aux = "SELECT DISTINCT dxcc from primary_subdivisions";
    //int counter = 0;
    sqlOk = query.exec(aux);
    if (sqlOk)
    {
        QSqlRecord rec = query.record();

        while ( (query.next()) && (query.isValid()) )
        {
            //counter ++;
            aux2.clear();

            nameCol = rec.indexOf("dxcc");
            //aux2 = QString::number(counter) + "-";
            aux2 = (query.value(nameCol)).toString() + "-" + dataProxy->getEntityNameFromId(query.value(nameCol).toInt());
            //aux2.append((query.value(nameCol)).toString());

            _logs.append(aux2);
        }
        return _logs;
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return _logs;
    }
}

void SetupPageSubdivisions::updateSelectedLogs()
{
       //qDebug() << "SetupPageSubdivisions::updateSelectedLogs";
    logsAvailable = readLogs();

    if (logsAvailable.length()>0)
    {
        currentLogs->clear();
        currentLogs->addItems(logsAvailable);
    }
    else
    {
           //qDebug() << "SetupPageSubdivisions::updateSelectedLogs Not selected (less than 1)";
        currentLogs->clear();
    }
}

int SetupPageSubdivisions::getSelectedLog()
{
      //qDebug() << "SetupPageSubdivisions::getSelectedLog: " << currentLogs->currentText();
    QString selectedLog = currentLogs->currentText();
    int i = 0;
    QStringList qs;
    qs.clear();
    qs << selectedLog.split("-");
    i = (qs.at(0)).toInt();
      //qDebug() << "SetupPageSubdivisions::getSelectedLog: " << QString::number(i);
    if (i>=1)
    {
        return i;
    }
    else
    {
        return 0;
    }
}

void SetupPageSubdivisions::slotCurrentLogsComboBoxChanged()
{
      //qDebug() << "SetupPageSubdivisions::slotCurrentLogsComboBoxChanged: " << currentLogs->currentText();
    QString a = (currentLogs->currentText()).section('-', 0, 0);
      //qDebug() << "SetupPageSubdivisions::slotCurrentLogsComboBoxChanged: a: " << a;

    setSelectedLog(a.toInt());
}

void SetupPageSubdivisions::setSelectedLog(const int _i)
{
      //qDebug() << "SetupPageSubdivisions::SetupPageSubdivisions::setSelectedLog: " << QString::number(_i);

    QString n = QString::number(_i) + "-";
    int selected = currentLogs->findText(n, Qt::MatchStartsWith);
    if (selected >= 0)
    {
           //qDebug() << "SetupPageSubdivisions::SetupPageSubdivisions::setSelectedLog selected>=0: " << QString::number(selected);
        currentLogs->setCurrentIndex(selected);
    }
    else
    {
          //qDebug() << "SetupPageSubdivisions::SetupPageSubdivisions::setSelectedLog not selected";
        return;
    }
}

void SetupPageSubdivisions::showError(const QString &_errorC)
{
    QString text = QString(tr("An error has occurred showing the following error code:") + "\n'%1'").arg(_errorC);


    QMessageBox::warning(this, tr("KLog - SetupPageSubdivisions"),
                                   text,
                                   QMessageBox::Ok);
}

void SetupPageSubdivisions::slotImportButtonClicked()
{
    //qDebug() << "SetupPageSubdivisions::slotImportButtonClicked";
    if (fileAwardManager->importNewAwardFile())
    {
        //qDebug() << "SetupPageSubdivisions::slotImportButtonClicked - OK";
    }
    else
    {
        //qDebug() << "SetupPageSubdivisions::slotImportButtonClicked - NOK";
    }
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open Award file"), util->getHomeDir(), tr("Award files (*.awa)"));
    //qDebug() << "SetupPageSubdivisions::slotImportButtonClicked - END";
}
