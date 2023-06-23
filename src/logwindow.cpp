/***************************************************************************
                          logwindow.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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

#include "logwindow.h"


LogWindow::LogWindow(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
    //qDebug() << "LogWindow::LogWindow: " ;
    dataProxy = dp;
    //sortingThroughProxyModel = false;
    logModel = new LogModel(dataProxy, this);
    util = new Utilities(Q_FUNC_INFO);
    connect(logModel, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    logView = new QTableView;
    columns.clear();

    //dxccStatusWidget = new DXCCStatusWidget(dataProxy);
    //elogClublog = new eLogClubLog();
    currentLog = -1;
    //proxyModel = new LogViewSortFilterProxyModel(this);

    awards = new Awards(dataProxy, Q_FUNC_INFO);

    createUI();
    createActions();
    setDefaultData();
    //qDebug() << "LogWindow::LogWindow: - END" ;

}

LogWindow::~LogWindow()
{
    delete(util);
    delete(awards);
//    emit clearError();
}

void LogWindow::setColumns(const QStringList &_columns)
{
    //qDebug() << Q_FUNC_INFO;
    columns.clear();
    //qDebug() << Q_FUNC_INFO << "llamando a filterValidFields";
    columns << dataProxy->filterValidFields(_columns);
    logModel->setColumns(columns);
}

void LogWindow::sortColumn(const int _c)
{
    //proxyModel->sort(_c);
    logModel->sort(_c, Qt::AscendingOrder);
}

void LogWindow::clear()
{
      //qDebug() << "LogWindow::clear " ;
}

void LogWindow::createUI()
{
    //qDebug() << Q_FUNC_INFO;

    logView->setContextMenuPolicy(Qt::CustomContextMenu);
    logView->setSortingEnabled(true);
    logView->horizontalHeader ()->setSectionsMovable (true);
    //logView->setDragDropMode (QAbstractItemView::InternalMove);
    //logView->setDropIndicatorShown (true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(logView);
    setLayout(layout);

}

void LogWindow::setDefaultData()
{
    //qDebug() << Q_FUNC_INFO;
    columns.clear();
    //qDebug() << Q_FUNC_INFO << "llamando a filterValidFields";
    columns << dataProxy->filterValidFields(util->getDefaultLogFields());
       //qDebug() << "LogWindow::setDefaultData" ;
}


void LogWindow::createlogPanel(const int _currentLog)
{
    //qDebug() << "LogWindow::createlogPanel: " << QString::number(_currentLog);
    currentLog = _currentLog;
    logModel->createlogModel(currentLog);
    //proxyModel->setSourceModel(logModel);
    logView->setModel(logModel);
    logView->setCurrentIndex(logModel->index(0, 0));

    setColumnsOfLog(columns);
    sortColumn(1);  //Initial sort by column 1 (date & time)

    logView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    logView->setSelectionBehavior(QAbstractItemView::SelectRows);
    logView->resizeColumnsToContents();
    logView->horizontalHeader()->setStretchLastSection(true);
    logView->sortByColumn(1, Qt::DescendingOrder);

}

void LogWindow::setColumnsOfLog(const QStringList &_columns)
{
    //qDebug() << Q_FUNC_INFO << ": Length: " << QString::number(_columns.length());

    QString stringQuery;
    stringQuery = QString("SELECT * FROM log LIMIT 1");
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    QSqlRecord rec;
    rec = query.record(); // Number of columns
    int ncolumns = rec.count();

    for (int i=0; i < ncolumns; i++)
    {
        logView->setColumnHidden(i, true);
    }

    QString aux;
    foreach(aux, columns)
    {
        //qDebug() << Q_FUNC_INFO << ": columns-1: " << aux;
    }
    foreach(aux, _columns)
    {
        //qDebug() << Q_FUNC_INFO << ": _columns-1: " << aux;
    }
    QStringList temPColumns;
    temPColumns.clear();
    temPColumns << _columns;
    columns.clear();
    //qDebug() << Q_FUNC_INFO << "llamando a filterValidFields";
    columns <<  dataProxy->filterValidFields(temPColumns);
    foreach(aux, columns)
    {
        //qDebug() << Q_FUNC_INFO << ":-2:  " << aux;
    }

    foreach(aux, columns)
    {
        //qDebug() << Q_FUNC_INFO << ": " << aux;
        showColumn(aux);
    }
}

void LogWindow::showColumn(const QString &_columnName)
{
    QString stringQuery;
    stringQuery = QString("SELECT * FROM log LIMIT 1");
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    QSqlRecord rec;
    rec = query.record(); // Number of columns
    int columns = rec.indexOf(_columnName);
    logView->setColumnHidden(columns, false);
}

void LogWindow::refresh()
{
    //qDebug() << "LogWindow::refresh" ;
    logModel->select();
}

void LogWindow::createActions()
{
    //qDebug() << Q_FUNC_INFO;
    createActionsCommon();
    showMenuRightButtonFromLogCreateActions();
}

void LogWindow::createActionsCommon()
{
    //LOG VIEW

        connect(logView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonFromLog( const QPoint& ) ) );
        connect(logView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickLog( const QModelIndex& ) ) );
}

void LogWindow::slotRighButtonFromLog(const QPoint& pos)
{
    //qDebug() << Q_FUNC_INFO;
    int row = (logView->indexAt(pos)).row();
    QItemSelectionModel *select = logView->selectionModel();
    QModelIndexList list = select->selectedRows();

    if (select->hasSelection() && (list.length()>1) )
    {
        rightButtonMultipleFromLogMenu();
    }
    else
    {
        rightButtonFromLogMenu(row);
    }
    //TODO: To be added to the logWindow and create an action that emist the QSO id
}

void LogWindow::rightButtonMultipleFromLogMenu()
{
    //qDebug() << Q_FUNC_INFO;

    QMenu menu(this);
    menu.addAction(multipleDelQSOsFromLogAct);
    //menu.addSeparator();
    menu.addAction(multipleExportToADIFFromLogAct);
    /*
    menu.addSeparator();

    QMenu *menuEQsl = menu.addMenu(tr("eQSL"));
    menuEQsl->addAction(multipleQueueForLoTWFromLogAct);
    menuEQsl->addAction(multipleQueueForClubLogFromLogAct);
    menuEQsl->addAction(multipleQueueForEQSLFromLogAct);
    menu.addMenu(menuEQsl);

    QMenu *menuQslS = menu.addMenu(tr("QSL"));

    QMenu *menuSentQsl = menuQslS->addMenu(tr("QSLs Send"));
    menuSentQsl->addAction(multipleQslSentViaBureauFromLogAct);
    menuSentQsl->addAction(multipleQslSentViaDirectFromLogAct);

    QMenu *menuRcvdQsl = menuQslS->addMenu(tr("QSLs Rcvd"));
    menuRcvdQsl->addAction(multipleQslRecViaBureauFromLogAct);
    menuRcvdQsl->addAction(multipleQslRecViaDirectFromLogAct);
    */

    menu.addSeparator();
    menu.addAction(multipleSelectAll);
    menu.addAction(multipleDeselectAll);
    menu.exec(QCursor::pos());
}

void LogWindow::rightButtonFromLogMenu(const int trow)
{
    //qDebug() << "LogWindow::slotshowRighButtonFromLogMenu:  " << QString::number(trow);
    //qDebug() << Q_FUNC_INFO;
    int _qsoID = ((logModel->index(trow, 0)).data(0)).toInt();
    //qDebug() << "LogWindow::slotshowRighButtonFromLogMenu:  QSOid: " << QString::number(_qsoID);
    bool qslReceived = isQSLReceived(_qsoID);
    bool qslSent = isQSLSent(_qsoID);
    QMenu menu(this);
    //menu.addAction(multipleQueueForQRZCOMFromLogAct);
    menu.addAction(multipleExportToADIFFromLogAct);
    menu.addAction(delQSOFromLogAct);
    delQSOFromLogAct->setData(trow);
    menu.addAction(qsoToEditFromLogAct);
    qsoToEditFromLogAct->setData(trow);
    menu.addAction(checkQRZCOMFromLogAct);
    checkQRZCOMFromLogAct->setData(trow);
    menu.addAction(checkDXHeatFromLogAct);
    checkDXHeatFromLogAct->setData(trow);

        menu.addSeparator();
        if (!qslSent)
        {
            QMenu *menuSentQsl = menu.addMenu(tr("QSL Send"));
            menuSentQsl->addAction(qslSentViaBureauFromLogAct);
            menuSentQsl->addAction(qslSentViaDirectFromLogAct);
            qslSentViaBureauFromLogAct->setData(trow);
            qslSentViaDirectFromLogAct->setData(trow);
        }

        if (!qslReceived)
        {
            QMenu *menuRecQsl = menu.addMenu(tr("QSL Rcvd"));
            menuRecQsl->addAction(qslRecViaBureauFromLogAct);
            menuRecQsl->addAction(qslRecViaDirectFromLogAct);
            qslRecViaBureauFromLogAct->setData(trow);
            qslRecViaDirectFromLogAct->setData(trow);
        }
        menu.addSeparator();
        menu.addAction(multipleSelectAll);
        menu.addAction(multipleDeselectAll);
    menu.exec(QCursor::pos());
}

void LogWindow::slotDoubleClickLog(const QModelIndex & index)
{
       //qDebug() << "LogWindow::slotDoubleClickLog: Row: " << QString::number(index.row()) << "Column: " << QString::number(index.column());

    int row = index.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    int qsoID = ((logModel->index(row, Qt::DisplayRole)).data(0)).toInt();

       //qDebug() << "LogWindow::slotDoubleClickLog: n: " << QString::number (logModel->data(index, Qt::DisplayRole).toInt());
       //qDebug() << "LogWindow::slotDoubleClickLog: emitted: " << QString::number (((logModel->index(row, Qt::DisplayRole)).data(0)).toInt());

    emit actionQSODoubleClicked(qsoID);
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());

    //TODO: To be added to the logWindow and create an action that emist the QSO id to be edited

    logModel->select();
}

bool LogWindow::isQSLReceived(const int _qsoId)
{
       //qDebug() << "LogWindow::isQSLReceived: " << QString::number(_qsoId);
    return dataProxy->isQSOConfirmed(_qsoId, true, false); // We check just paper QSL
    //return dataProxy->isQSLReceived(_qsoId);
}

bool LogWindow::isQSLSent(const int _qsoId)
{
       //qDebug() << "LogWindow::isQSLSent: " << QString::number(_qsoId);

    return dataProxy->isQSLSent(_qsoId);
}

void LogWindow::showMenuRightButtonFromLogCreateActions()
{
   //qDebug() << "LogWindow::showMenuRightButtonFromLogCreateActions";

    delQSOFromLogAct = new QAction(tr("&Delete"), this);
    delQSOFromLogAct->setShortcut(Qt::CTRL + Qt::Key_D);
    delQSOFromLogAct->setStatusTip(tr("Delete a QSO"));
    connect(delQSOFromLogAct, SIGNAL(triggered()), this, SLOT(slotQsoDeleteFromLog()));

    qsoToEditFromLogAct = new QAction(tr("&Edit QSO"), this);
    qsoToEditFromLogAct->setShortcut(Qt::CTRL + Qt::Key_E);
    qsoToEditFromLogAct->setStatusTip(tr("Edit this QSO"));
    connect(qsoToEditFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOToEditFromLog()));

    qslSentViaBureauFromLogAct = new QAction(tr("Via &bureau"), this);
    qslSentViaBureauFromLogAct->setShortcut(Qt::CTRL + Qt::Key_B);
    qslSentViaBureauFromLogAct->setStatusTip(tr("Send this QSL via bureau"));
    connect(qslSentViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauFromLog() ));

    qslSentViaDirectFromLogAct = new QAction(tr("D&irect"), this);
    qslSentViaDirectFromLogAct->setShortcut(Qt::CTRL + Qt::Key_I);
    qslSentViaDirectFromLogAct->setStatusTip(tr("Send this QSL via direct"));
    connect(qslSentViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectFromLog()   ));

    qslRecViaBureauFromLogAct = new QAction(tr("Via bureau"), this);
    qslRecViaBureauFromLogAct->setShortcut(Qt::CTRL + Qt::Key_R);
    qslRecViaBureauFromLogAct->setStatusTip(tr("QSL &received via bureau"));
    connect(qslRecViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauFromLog() ));

    qslRecViaDirectFromLogAct = new QAction(tr("Direct"), this);
    qslRecViaDirectFromLogAct->setShortcut(Qt::CTRL + Qt::Key_T);
    qslRecViaDirectFromLogAct->setStatusTip(tr("QSL received via direc&t"));
    connect(qslRecViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectFromLog() ));

    checkQRZCOMFromLogAct = new QAction(tr("Check in QRZ.com"), this);
    checkQRZCOMFromLogAct->setShortcut(Qt::CTRL + Qt::Key_Q);
    checkQRZCOMFromLogAct->setStatusTip(tr("Check this callsign in QRZ.com"));
    connect(checkQRZCOMFromLogAct, SIGNAL(triggered()), this, SLOT( slotCheckQRZCom() ));

    checkDXHeatFromLogAct = new QAction(tr("Check in DXHeat.com"), this);
    checkDXHeatFromLogAct->setShortcut(Qt::CTRL + Qt::Key_Q);
    checkDXHeatFromLogAct->setStatusTip(tr("Check this callsign in DXHeat.com"));
    connect(checkDXHeatFromLogAct, SIGNAL(triggered()), this, SLOT( slotCheckDXHeatCom() ));

    multipleDelQSOsFromLogAct = new QAction(tr("Delete selected QSOs"), this);
    //multipleDelQSOsFromLogAct->setShortcut(Qt::CTRL + Qt::Key_D);
    multipleDelQSOsFromLogAct->setStatusTip(tr("Delete the selected QSOs"));
    connect(multipleDelQSOsFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOsDeleteFromLog()));

    multipleExportToADIFFromLogAct = new QAction(tr("Export to ADIF"), this);
    multipleExportToADIFFromLogAct->setStatusTip(tr("Export the selected QSOs to an ADIF file."));
    connect(multipleExportToADIFFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOsExportFromLog()));

    //multipleQueueForQRZCOMFromLogAct = new QAction(tr("Upload to QRZ.com"), this);
    //multipleQueueForQRZCOMFromLogAct->setStatusTip(tr("Send the selected QSOs to QRZ.com."));
    //connect(multipleQueueForQRZCOMFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOsQRZUploadFromLog()));


    multipleQueueForLoTWFromLogAct = new QAction(tr("Upload to LoTW"), this);
    multipleQueueForLoTWFromLogAct->setStatusTip(tr("Upload the selected QSOs to LoTW"));
    connect(multipleQueueForLoTWFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOsUploadToLoTWFromLog()));

    multipleQueueForClubLogFromLogAct = new QAction(tr("Upload to ClubLog"), this);
    multipleQueueForClubLogFromLogAct->setStatusTip(tr("Upload the selected QSOs to ClubLog"));
    connect(multipleQueueForClubLogFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOsUploadToClubLogFromLog()));

    multipleQueueForEQSLFromLogAct = new QAction(tr("Upload to eQSL.cc"), this);
    multipleQueueForEQSLFromLogAct->setStatusTip(tr("Upload the selected QSOs to eQSL.cc"));
    connect(multipleQueueForEQSLFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOsUploadToEQSLFromLog()));

   multipleQslSentViaBureauFromLogAct = new QAction(tr("Via bureau"), this);
   multipleQslSentViaBureauFromLogAct->setStatusTip(tr("Send these QSLs via bureau"));
   connect(multipleQslSentViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotMultipleQSLSentViaBureauFromLog() ));

   multipleQslSentViaDirectFromLogAct = new QAction(tr("Direct"), this);
   multipleQslSentViaDirectFromLogAct->setStatusTip(tr("Send these QSLs via direct"));
   connect(multipleQslSentViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotMultipleQSLSentViaDirectFromLog()   ));

   multipleQslRecViaBureauFromLogAct = new QAction(tr("Via bureau"), this);
   multipleQslRecViaBureauFromLogAct->setStatusTip(tr("QSLs received via bureau"));
   connect(multipleQslRecViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotMultipleQSLRecViaBureauFromLog() ));

   multipleQslRecViaDirectFromLogAct = new QAction(tr("Direct"), this);
   multipleQslRecViaDirectFromLogAct->setStatusTip(tr("QSLs received via direc&t"));
   connect(multipleQslRecViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotMultipleQSLRecViaDirectFromLog() ));

   multipleDeselectAll = new QAction(tr("Select none"), this);
   multipleDeselectAll->setStatusTip(tr("Remove all selections"));
   connect(multipleDeselectAll, SIGNAL(triggered()), this, SLOT( slotQSOsDeselectAll() ));

   multipleSelectAll = new QAction(tr("Select all"), this);
   multipleSelectAll->setStatusTip(tr("Select all the QSOs"));
   connect(multipleSelectAll, SIGNAL(triggered()), this, SLOT( slotQSOsSelectAll() ));
}

void LogWindow::slotQSOsSelectAll()
{
    logView->selectAll();
}

void LogWindow::slotQSOsDeselectAll()
{
    logView->selectionModel()->clearSelection();
}

void LogWindow::slotQSLSentViaBureauFromLog()
{
      //qDebug() << "LogWindow::slotQSLSentViaBureauFromLog: " << (qslSentViaBureauFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) );
    int _qsoId = ((logModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslSentViaBureau(_qsoId);
}

void LogWindow::slotQSLSentViaDirectFromLog()
{
       //qDebug() << "LogWindow::slotQSLSentViaDirectFromLog: " << (qslSentViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) );
     int _qsoId = ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //dataProxy->qslSentViaDirect(_qsoId, (QDateTime::currentDateTime()).toString("yyyy-MM-dd"));
    dataProxy->qslSentViaDirect(_qsoId, QDate::currentDate());

}

void LogWindow::slotQSLRecViaBureauFromLog()
{
      //qDebug() << "LogWindow::slotQSLRecViaBureauFromLog: ";

    int _qsoId = ((logModel->index( ( (qslRecViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaBureau(_qsoId);
    //TODO: To be added to the logWindow and create an action that emist the QSO id
}

void LogWindow::slotQSLRecViaDirectFromLog()
{
       //qDebug() << "LogWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) );
    int _qsoId = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaDirect(_qsoId);
    // Mark Sent, Bureau, date, update log.
    //TODO: To be added to the logWindow and create an action that emist the QSO id
}

void LogWindow::slotQSOToEditFromLog()
{
       //qDebug() << "slotQSOToEditFromLog: " << (qsoToEditFromLogAct->data()).toString();

    //qsoToEdit((logModel->index((qsoToEditFromLogAct->data()).toInt(), 0)).data(0).toInt());
    int QSOid = ((logModel->index((qsoToEditFromLogAct->data()).toInt(), 0)).data(0)).toInt();
    /*
    int row = index.row();
    int qsoID = ((logModel->index(row, Qt::DisplayRole)).data(0)).toInt();
    emit actionQSODoubleClicked(qsoID);
    */

    //int row = index.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    emit actionQSODoubleClicked(QSOid);



    //TODO: To be added to the logWindow and create an action that emit the QSO id
}

void LogWindow::deleteQSO(const int _qsoId)
{
   //qDebug() << "LogWindow::deleteQSO " << QString::number(_qsoId);
    emit actionDeleteQSO(_qsoId);
}

/*
void LogWindow::deleteQSO(const int _qsoID)
{
       //qDebug() << "LogWindow::deleteQSO: " << QString::number(_qsoID);




    emit actionDeleteQSO(_qsoID);
    //elogClublog->deleteQSO(dataProxy->getClubLogRealTimeFromId(_qsoID));
    dataProxy->deleteQSO(_qsoID);

    //logModel->removeRow((delQSOFromLogAct->data()).toInt()); //TODO: This has been replaced by the previous line
    //awards->recalculateAwards();
    refresh();
    //dxccStatusWidget->refresh();
    emit updateAwards();
    emit updateSearchText();

}
*/
void LogWindow::slotQsoDeleteFromLog()
{
      //qDebug() << "LogWindow::slotQsoDeleteFromLog: " << (delQSOFromLogAct->data()).toString();
    //TODO: To be added to the logWindow and create an action that emist the QSO id

    int QSOid = ((logModel->index((delQSOFromLogAct->data()).toInt(), 0)).data(0)).toInt();
    deleteQSO(QSOid);
      //qDebug() << "LogWindow::slotQsoDeleteFromLog (id): " << QString::number(QSOid);
    /*
    switch (ret) {
      case QMessageBox::Yes:
          //qDebug() << "LogWindow::slotQsoDeleteFromLog (id): -1";
        deleteQSO(QSOid);

          break;
      case QMessageBox::No:
          // No was clicked
          break;
      default:
          // should never be reached
          break;
    }
    */
}

void LogWindow::slotQSOsDeleteFromLog()
{
    //qDebug() << "LogWindow::slotQSOsDeleteFromLog";
    QItemSelectionModel *select = logView->selectionModel();
    QList<int> qsos;
    qsos.clear();
    if (select->hasSelection())
    {
        QModelIndexList list = select->selectedRows();
        foreach (QModelIndex index, list)
        {
            qsos.append(index.data(0).toInt());
            //qDebug() << "LogWindow::slotshowRighButtonFromLog: " << QString::number(index.row()) ;
            //qDebug() << "LogWindow::slotshowRighButtonFromLog: " << QString::number(index.data(0).toInt()) ;
        }
    }
    if (qsos.length()>0)
    {
        emit deleteTheseQSOs(qsos);
    }
}

void LogWindow::slotQSOsExportFromLog()
{
   //qDebug() << "LogWindow::slotQSOsExportFromLog";
    QItemSelectionModel *select = logView->selectionModel();
    QList<int> qsos;
    qsos.clear();
    if (select->hasSelection())
    {
        QModelIndexList list = select->selectedRows();
        foreach (QModelIndex index, list)
        {
            qsos.append(index.data(0).toInt());
           //qDebug() << "LogWindow::slotQSOsExportFromLog: " << QString::number(index.row()) ;
           //qDebug() << "LogWindow::slotQSOsExportFromLog: " << QString::number(index.data(0).toInt()) ;
        }
    }
    if (qsos.length()>0)
    {
        emit exportToADIFTheseQSOs(qsos);
    }
   //qDebug() << "LogWindow::slotQSOsExportFromLog - END - " << QString::number(qsos.length());
}


void LogWindow::slotQSOsUploadToLoTWFromLog()
{
    //qDebug() << "LogWindow::slotQSOsUploadToLoTWFromLog - TO BE IMPLEMENTED";
}

void LogWindow::slotQSOsUploadToClubLogFromLog()
{
    //qDebug() << "LogWindow::slotQSOsUploadToClubLogFromLog - TO BE IMPLEMENTED";
}

void LogWindow::slotQSOsQRZUploadFromLog()
{
   //qDebug() << "LogWindow::slotQSOsQRZUploadFromLog - TO BE IMPLEMENTED";
    QItemSelectionModel *select = logView->selectionModel();
    QList<int> qsos;
    qsos.clear();
    if (select->hasSelection())
    {
        QModelIndexList list = select->selectedRows();
        foreach (QModelIndex index, list)
        {
            qsos.append(index.data(0).toInt());
            //qDebug() << "LogWindow::slotQSOsQRZUploadFromLog: " << QString::number(index.row()) ;
            //qDebug() << "LogWindow::slotQSOsQRZUploadFromLog: " << QString::number(index.data(0).toInt()) ;
        }
    }
    if (qsos.length()>0)
    {
        emit uploadToQRZcomTheseQSOs(qsos);
    }
}

void LogWindow::slotQSOsUploadToEQSLFromLog()
{
    //qDebug() << "LogWindow::slotQSOsUploadToEQSLFromLog - TO BE IMPLEMENTED";
}

void LogWindow::slotMultipleQSLSentViaBureauFromLog()
{
    //qDebug() << "LogWindow::slotMultipleQSLSentViaBureauFromLog - TO BE IMPLEMENTED";
}

void LogWindow::slotMultipleQSLSentViaDirectFromLog()
{
    //qDebug() << "LogWindow::slotMultipleQSLSentViaDirectFromLog - TO BE IMPLEMENTED";
}

void LogWindow::slotMultipleQSLRecViaBureauFromLog()
{
    //qDebug() << "LogWindow::slotMultipleQSLRecViaBureauFromLog - TO BE IMPLEMENTED";
}

void LogWindow::slotMultipleQSLRecViaDirectFromLog()
{
    //qDebug() << "LogWindow::slotMultipleQSLRecViaDirectFromLog - TO BE IMPLEMENTED";
}


void LogWindow::qslSentViaBureau(const int _qsoId)
{
       //qDebug() << "LogWindow::qslSentViaBureau: " << QString::number(_qsoId)yyyy-MM-dd
    dataProxy->qslSentViaBureau(_qsoId, QDate::currentDate());
}

void LogWindow::qslRecViaBureau(const int _qsoId)
{
   //    //qDebug() << "LogWyyyy-MM-ddRecViaBureau: " << QString::number(_qsoIyyyy-MM-dd<< (dateTime->currentDateTime()).toString("yyyy/MM/dd");
    dataProxy->qslRecViaBureau(_qsoId, QDate::currentDate(), false);
    awards->setAwards(_qsoId);   //Update the Award status

    refresh();
    emit updateAwards();
}


void LogWindow::qslRecViaDirect(const int _qsoId)
{
       //qDebug() << "LogWindow::qslRecViaDirect: " << QString::number(_qsoId)yyyy-MM-dd
    dataProxy->qslRecViaDirect(_qsoId, QDate::currentDate(), false);
    awards->setAwards(_qsoId);

    refresh();
    emit updateAwards();
}


void LogWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery)
{
    emit queryError(functionFailed, errorCodeS, nativeError, failedQuery);
}

void LogWindow::slotCheckQRZCom()
{
    QString _qrz = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 2)).data(Qt::DisplayRole).toString());
    //int _qsoId = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //QString _qrz = dataProxy->getCallFromId(_qsoId);
      //qDebug() << "LogWindow::sloTCheckQRZCom: " << _qrz;
    util->openQrzcom (_qrz);
    //QString url = "https://www.qrz.com/db/" + _qrz;
    //QDesktopServices::openUrl(QUrl(url));
}

void LogWindow::slotCheckDXHeatCom()
{
    QString _qrz = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 2)).data(Qt::DisplayRole).toString());
    //int _qsoId = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //QString _qrz = dataProxy->getCallFromId(_qsoId);
      //qDebug() << "LogWindow::slotCheckDXHeatCom(): " << _qrz;
    QString url = "https://www.dxheat.com/db/" + _qrz;
    QDesktopServices::openUrl(QUrl(url));
}

