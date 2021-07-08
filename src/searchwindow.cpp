/***************************************************************************
                          searchwindow.cpp  -  description
                             -------------------
    begin                : sept 2020
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

#include "searchwindow.h"


SearchWindow::SearchWindow(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
    //qDebug() << "SearchWindow::SearchWindow: "  << endl;
    dataProxy = dp;
    showStationCallsignInHeader = true;
    //sortingThroughProxyModel = false;
    searchModel = new SearchModel(dataProxy, this);
    util = new Utilities;
    connect(searchModel, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    //logView = new QTableView;
    treeView = new QTreeView;
    //dxccStatusWidget = new DXCCStatusWidget(dataProxy);
    //elogClublog = new eLogClubLog();
    currentLog = -1;
    //proxyModel = new LogViewSortFilterProxyModel(this);


    awards = new Awards(dataProxy, Q_FUNC_INFO);

    createUI();
    createActions();
    setDefaultData();
    //qDebug() << "SearchWindow::SearchWindow: - END"  << endl;

}

SearchWindow::~SearchWindow()
{
//    emit clearError();
}


void SearchWindow::sortColumn(const int _c)
{
    //proxyModel->sort(_c);
    searchModel->sort(_c, Qt::AscendingOrder);
}

void SearchWindow::clear()
{
      //qDebug() << "SearchWindow::clear "  << endl;
    qslingNeeded = false;
}

void SearchWindow::createUI()
{
      //qDebug() << "SearchWindow::createUI"  << endl;


    //logView->setContextMenuPolicy(Qt::CustomContextMenu);
    //logView->setSortingEnabled(true);

    QStringList labels;
    if (showStationCallsignInHeader)
    {
        labels << tr("Call") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("Station Callsign") << tr("ID") ;
        //treeView->setColumnCount(8);
    }
    else
    {
       labels << tr("Call") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("ID") ;
       //treeView->setColumnCount(7);
    }

    //searchModel->setColumnsToDX();

    /*
    QStandardItemModel model;
    model.setColumnCount (1);
    model.setHeaderData (0, Qt::Horizontal, "Player");
    */
    //treeView->setHeaderLabels(labels);
    //QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
   (treeView->header())->resizeSections(QHeaderView::ResizeToContents);


    //treeView->clear();
    //searchResultsTreeWidget->collapseAll();
    treeView->setSortingEnabled(true);

    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView->setSortingEnabled(true);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(treeView);
    //layout->addWidget(logView);
    setLayout(layout);
}

void SearchWindow::setDefaultData()
{
       //qDebug() << "SearchWindow::setDefaultData"  << endl;
}

void SearchWindow::setStationCallsignInHeader(const bool _h)
{
    showStationCallsignInHeader = _h;
    setColumnsToDX();

}

void SearchWindow::createlogPanel(const int _currentLog)
{
      //qDebug() << "SearchWindow::createlogPanel: " << QString::number(_currentLog) << endl;
    currentLog = _currentLog;
    searchModel->createSearchModel(currentLog);

    treeView->setModel(searchModel);
    treeView->setCurrentIndex(searchModel->index(0, 0));

    //logView->setModel(searchModel);
    //logView->setCurrentIndex(searchModel->index(0, 0));

    //setProxyModel(false);

    //QString contestMode = dataProxy->getLogTypeOfUserLog(currentLog);
    setColumnsToDX();
    sortColumn(1);  //Initial sort by column 1 (date & time)

    //qDebug() << "SearchWindow::createlogPanel " << searchModel->record(0).field(1).value().toString() << endl;
    //logView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //logView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //logView->resizeColumnsToContents();
    //logView->horizontalHeader()->setStretchLastSection(true);
    //logView->sortByColumn(1);

    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //treeView->resizeColumnsToContents();
    //treeView->horizontalHeader()->setStretchLastSection(true);
    treeView->sortByColumn(1);
}

void SearchWindow::setColumnsToDX()
{
      //qDebug() << "SearchWindow::setColumnsToDX"  << endl;

    QString stringQuery;
    //stringQuery = QString("SELECT call, qso_date, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, id FROM log LIMIT 1");
    stringQuery = QString("SELECT * FROM log LIMIT 1");
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    //qDebug() << "SearchWindow::setColumnsToDX - Query: "  << query.lastQuery() << endl;
    QSqlRecord rec;
    rec = query.record(); // Number of columns
    int columns = rec.count();

    for (int i=0; i < columns; i++)
    {
        //logView->setColumnHidden(i, true);
        treeView->setColumnHidden(i, true);
    }

    columns = rec.indexOf("qso_date");
    treeView->setColumnHidden(columns, false);
    searchModel->setHeaderData(columns, Qt::Horizontal,tr("Date/time"));
    searchModel->setSort(columns, Qt::AscendingOrder);

    columns = rec.indexOf("call");
    treeView->setColumnHidden(columns, false);
    searchModel->setHeaderData(columns, Qt::Horizontal,tr("Call"));

    columns = rec.indexOf("bandid");
    searchModel->setBandIdColumn(columns);
    treeView->setColumnHidden(columns, false);
    searchModel->setRelation(columns, QSqlRelation("band", "id", "name"));
    searchModel->setHeaderData(columns, Qt::Horizontal,tr("Band"));


    columns = rec.indexOf("modeid");
    treeView->setColumnHidden(columns, false);
    searchModel->setHeaderData(columns, Qt::Horizontal,tr("Mode"));
    searchModel->setRelation(columns, QSqlRelation("mode", "id", "submode"));
    searchModel->setModeIdColumn(columns);

    columns = rec.indexOf("qsl_sent");
    treeView->setColumnHidden(columns, false);
    searchModel->setHeaderData(columns, Qt::Horizontal,tr("QSL Sent"));

    columns = rec.indexOf("qsl_rcvd");
    treeView->setColumnHidden(columns, false);
    searchModel->setHeaderData(columns, Qt::Horizontal,tr("QSL Rcvd"));

    if (showStationCallsignInHeader)
    {
        columns = rec.indexOf("station_callsign");
        treeView->setColumnHidden(columns, false);
        searchModel->setHeaderData(columns, Qt::Horizontal,tr("Station callsign"));
    }

    columns = rec.indexOf("dxcc");
    searchModel->setDXCCColumn(columns);
    columns = rec.indexOf("lognumber");
    searchModel->setLogNColumn(columns);

    //columns = rec.indexOf("id");

    //treeView->setColumnHidden(columns, false);
}

void SearchWindow::refresh()
{
       //qDebug() << "SearchWindow::refresh"  << endl;

    searchModel->select();
}

void SearchWindow::createActions()
{
    createActionsCommon();
}

void SearchWindow::createActionsCommon()
{
    //LOG VIEW

        //connect(logView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonFromLog( const QPoint& ) ) );
        //connect(logView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickLog( const QModelIndex& ) ) );
        connect(treeView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonFromLog( const QPoint& ) ) );
        connect(treeView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickLog( const QModelIndex& ) ) );
}

void SearchWindow::slotRighButtonFromLog(const QPoint& pos)
{
    //qDebug() << "SearchWindow::slotshowRighButtonFromLog: " << QString::number((treeView->indexAt(pos)).row())  << endl;
    //int row = (logView->indexAt(pos)).row();
    int row = (treeView->indexAt(pos)).row();
    //int _qsoID = ((searchModel->index(row, 0)).data(0)).toInt();
    showMenuRightButtonFromLogCreateActions();
    rightButtonFromLogMenu(row);
    //TODO: To be added to the SearchWindow and create an action that emist the QSO id
}

void SearchWindow::rightButtonFromLogMenu(const int row)
{
    //qDebug() << "SearchWindow::slotshowRighButtonFromLogMenu: QSO_id: " << QString::number(row) << endl;

    int _qsoID = ((searchModel->index(row, 0)).data(0)).toInt();
    //qDebug() << "SearchWindow::slotshowRighButtonFromLogMenu:  QSOid: " << QString::number(_qsoID) << endl;
    bool qslReceived = isQSLReceived(_qsoID);
    bool qslSent = isQSLSent(_qsoID);
    QMenu menu(this);

    menu.addAction(delQSOFromLogAct);
    delQSOFromLogAct->setData(row);
    menu.addAction(qsoToEditFromLogAct);
    qsoToEditFromLogAct->setData(row);
    menu.addAction(checkQRZCOMFromLogAct);
    checkQRZCOMFromLogAct->setData(row);
    menu.addAction(checkDXHeatFromLogAct);
    checkDXHeatFromLogAct->setData(row);

        menu.addSeparator();
        if (qslSent)
        {
        }
        else
        {
            QMenu *menuSentQsl = menu.addMenu(tr("QSL Send"));
            menuSentQsl->addAction(qslSentViaBureauFromLogAct);
            menuSentQsl->addAction(qslSentViaDirectFromLogAct);
            menuSentQsl->addAction(qslSentRequestedAct);
            if (!qslReceived)
            {
                menuSentQsl->addAction(qslSentViaBureauMarkRcvReqFromSearchAct);
                menuSentQsl->addAction(qslSentViaDirectMarkRcvReqFromSearchAct);
                qslSentViaBureauMarkRcvReqFromSearchAct->setData(row);
                qslSentViaDirectMarkRcvReqFromSearchAct->setData(row);
            }
            qslSentViaBureauFromLogAct->setData(row);
            qslSentViaDirectFromLogAct->setData(row);
            qslSentRequestedAct->setData(row);
           }

        if (qslReceived)
        {
        }
        else
        {
            QMenu *menuRecQsl = menu.addMenu(tr("QSL Rcvd"));
            menuRecQsl->addAction(qslRecViaBureauFromLogAct);
            menuRecQsl->addAction(qslRecViaBureauMarkReqFromSearchAct);
            menuRecQsl->addAction(qslRecViaDirectFromLogAct);
            menuRecQsl->addAction(qslRecViaDirectMarkReqFromSearchAct);
            menuRecQsl->addAction(qslRecRequestedAct);
            qslRecViaBureauFromLogAct->setData(row);
            qslRecViaBureauMarkReqFromSearchAct->setData(row);
            qslRecViaDirectFromLogAct->setData(row);
            qslRecViaDirectMarkReqFromSearchAct->setData(row);
            qslRecRequestedAct->setData(row);

        }
    menu.exec(QCursor::pos());
}

void SearchWindow::slotDoubleClickLog(const QModelIndex & index)
{
    //qDebug() << "SearchWindow::slotDoubleClickLog: Row: " << QString::number(index.row()) << "Column: " << QString::number(index.column()) << endl;

    int row = index.row();
    //qsoToEdit((searchModel->index(row, 0)).data(0).toInt());
    int qsoID = ((searchModel->index(row, Qt::DisplayRole)).data(0)).toInt();

    //qDebug() << "SearchWindow::slotDoubleClickLog: n: " << QString::number (searchModel->data(index, Qt::DisplayRole).toInt()) << endl;
    //qDebug() << "SearchWindow::slotDoubleClickLog: emitted: " << QString::number (((searchModel->index(row, Qt::DisplayRole)).data(0)).toInt()) << endl;

    emit actionQSODoubleClicked(qsoID);
    //qsoToEdit((searchModel->index(row, 0)).data(0).toInt());

    //TODO: To be added to the SearchWindow and create an action that emist the QSO id to be edited

    searchModel->select();
}

bool SearchWindow::isQSLReceived(const int _qsoId)
{
       //qDebug() << "SearchWindow::isQSLReceived: " << QString::number(_qsoId) << endl;
    return dataProxy->isQSOConfirmed(_qsoId, true, false); // We check just paper QSL
    //return dataProxy->isQSLReceived(_qsoId);
}

bool SearchWindow::isQSLSent(const int _qsoId)
{
       //qDebug() << "SearchWindow::isQSLSent: " << QString::number(_qsoId) << endl;

    return dataProxy->isQSLSent(_qsoId);
}


void SearchWindow::showMenuRightButtonFromLogCreateActions()
{
   //qDebug() << "SearchWindow::showMenuRightButtonFromLogCreateActions" << endl;

    delQSOFromLogAct = new QAction(tr("&Delete"), this);
    delQSOFromLogAct->setStatusTip(tr("Delete a QSO"));
    connect(delQSOFromLogAct, SIGNAL(triggered()), this, SLOT(slotQsoDeleteFromLog()));

    qsoToEditFromLogAct = new QAction(tr("&Edit QSO"), this);
    qsoToEditFromLogAct->setStatusTip(tr("Edit this QSO"));
    connect(qsoToEditFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOToEditFromLog()));

    qslSentViaBureauFromLogAct = new QAction(tr("Via &bureau"), this);
    qslSentViaBureauFromLogAct->setStatusTip(tr("Send this QSL via bureau"));
    connect(qslSentViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauFromLog() ));

    qslSentViaDirectFromLogAct = new QAction(tr("D&irect"), this);
    qslSentViaDirectFromLogAct->setStatusTip(tr("Send this QSL via direct"));
    connect(qslSentViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectFromLog()   ));

    qslRecViaBureauFromLogAct = new QAction(tr("Via bureau"), this);
    qslRecViaBureauFromLogAct->setStatusTip(tr("QSL &received via bureau"));
    connect(qslRecViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauFromLog() ));

    qslRecViaDirectFromLogAct = new QAction(tr("Direct"), this);
    qslRecViaDirectFromLogAct->setStatusTip(tr("QSL received via direc&t"));
    connect(qslRecViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectFromLog() ));

    checkQRZCOMFromLogAct = new QAction(tr("Check in QRZ.com"), this);
    checkQRZCOMFromLogAct->setStatusTip(tr("Check this callsign in QRZ.com"));
    connect(checkQRZCOMFromLogAct, SIGNAL(triggered()), this, SLOT( slotCheckQRZCom() ));

    checkDXHeatFromLogAct = new QAction(tr("Check in DXHeat.com"), this);
    checkDXHeatFromLogAct->setStatusTip(tr("Check this callsign in DXHeat.com"));
    connect(checkDXHeatFromLogAct, SIGNAL(triggered()), this, SLOT( slotCheckDXHeatCom() ));

    qslSentRequestedAct = new QAction(tr("&Request my QSL"), this);
    qslSentRequestedAct->setStatusTip(tr("Mark my QSL as requested"));
    connect(qslSentRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLSentMarkAsRequested()   ));

    qslSentViaDirectMarkRcvReqFromSearchAct = new QAction(tr("Via Direct and mark DX QSL as requested"), this);
    qslSentViaDirectMarkRcvReqFromSearchAct->setStatusTip(tr("Send this QSL via direct and mark DX QSL as requested"));
    connect(qslSentViaDirectMarkRcvReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectMarkDXReqFromSearch() ));

    qslSentViaBureauMarkRcvReqFromSearchAct = new QAction(tr("Via Bureau and mark DX QSL as requested"), this);
    qslSentViaBureauMarkRcvReqFromSearchAct->setStatusTip(tr("Send this QSL via bureau and mark DX QSL as requested"));
    connect(qslSentViaBureauMarkRcvReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauMarkDXReqFromSearch() ));

    qslRecRequestedAct = new QAction(tr("&Request the QSL"), this);
    qslRecRequestedAct->setStatusTip(tr("Mark the QSL as requested"));
    connect(qslRecRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLRecMarkAsRequested()   ));

    qslRecViaBureauMarkReqFromSearchAct = new QAction(tr("Via bureau and mark my QSL as requested"), this);
    qslRecViaBureauMarkReqFromSearchAct->setStatusTip(tr("QSL received via bureau and mark my QSL as requested"));
    connect(qslRecViaBureauMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauMarkReqFromSearch() ));

    qslRecViaDirectMarkReqFromSearchAct = new QAction(tr("Direc&t and mark as my QSL requested"), this);
    qslRecViaDirectMarkReqFromSearchAct->setStatusTip(tr("QSL received via direct and mark my QSL as requested"));
    connect(qslRecViaDirectMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectMarkReqFromSearch() ));

}


void SearchWindow::slotQSLSentViaBureauFromLog()
{
      //qDebug() << "SearchWindow::slotQSLSentViaBureauFromLog: " << (qslSentViaBureauFromLogAct->data()).toString() << " - Id = " << QString::number( ((searchModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((searchModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslSentViaBureau(_qsoId);
    searchModel->select();

}

void SearchWindow::slotQSLSentViaDirectFromLog()
{
       //qDebug() << "SearchWindow::slotQSLSentViaDirectFromLog: " << (qslSentViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((searchModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
     int _qsoId = ((searchModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //dataProxy->qslSentViaDirect(_qsoId, (QDateTime::currentDateTime()).toString("yyyy-MM-dd"));
    dataProxy->qslSentViaDirect(_qsoId, QDate::currentDate());
    searchModel->select();

}

void SearchWindow::slotQSLRecViaBureauFromLog()
{
      //qDebug() << "SearchWindow::slotQSLRecViaBureauFromLog: " << endl;

    int _qsoId = ((searchModel->index( ( (qslRecViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaBureau(_qsoId);
    searchModel->select();
    //TODO: To be added to the SearchWindow and create an action that emist the QSO id
}

void SearchWindow::slotQSLRecViaDirectFromLog()
{
       //qDebug() << "SearchWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((searchModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((searchModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaDirect(_qsoId);
    searchModel->select();
    // Mark Sent, Bureau, date, update log.
    //TODO: To be added to the SearchWindow and create an action that emist the QSO id
}

void SearchWindow::slotQSOToEditFromLog()
{
    //qDebug() << "slotQSOToEditFromLog: " << (qsoToEditFromLogAct->data()).toString() << endl;

    int QSOid = ((searchModel->index((qsoToEditFromLogAct->data()).toInt(), 0)).data(0)).toInt();
    //int QSOid = qsoToEditFromLogAct->data().toInt();
    emit actionQSODoubleClicked(QSOid);



    //TODO: To be added to the SearchWindow and create an action that emit the QSO id
}
/*
void SearchWindow::deleteQSO(const int _qsoID)
{
    //qDebug() << "SearchWindow::deleteQSO: " << QString::number(_qsoID) << endl;

    //int QSOid = (delQSOFromLogAct->data()).toInt();
    emit actionDeleteQSO(_qsoID);



    elogClublog->deleteQSO(dataProxy->getClubLogRealTimeFromId(_qsoID));
    dataProxy->deleteQSO(_qsoID);


    refresh();

    searchModel->select();
    emit updateAwards();
    emit updateSearchText();

}
*/
void SearchWindow::slotQsoDeleteFromLog()
{
    //qDebug() << "SearchWindow::slotQsoDeleteFromLog: qsoID1: " << QString::number((delQSOFromLogAct->data()).toInt()) << endl;
    int QSOid = ((searchModel->index((delQSOFromLogAct->data()).toInt(), 0)).data(0)).toInt();

    //int QSOid = (delQSOFromLogAct->data()).toInt();
    //qDebug() << "SearchWindow::slotQsoDeleteFromLog: qsoID1: " << QString::number(QSOid) << endl;
    //qDebug() << "SearchWindow::slotQsoDeleteFromLog: qsoID2: " << QString::number((delQSOFromLogAct->data()).toInt()) << endl;
    emit actionDeleteQSO(QSOid);
    searchModel->select();
}

void SearchWindow::qslSentViaBureau(const int _qsoId)
{
       //qDebug() << "SearchWindow::qslSentViaBureau: " << QString::number(_qsoId)yyyy-MM-dd
    dataProxy->qslSentViaBureau(_qsoId, QDate::currentDate());
    searchModel->select();
}

void SearchWindow::qslRecViaBureau(const int _qsoId)
{
   //    //qDebug() << "LogWyyyy-MM-ddRecViaBureau: " << QString::number(_qsoIyyyy-MM-dd<< (dateTime->currentDateTime()).toString("yyyy/MM/dd") << endl;
    dataProxy->qslRecViaBureau(_qsoId, QDate::currentDate(), false);
    awards->setAwards(_qsoId);   //Update the Award status
    searchModel->select();
    //refresh();
    emit updateAwards();
}

void SearchWindow::qslRecViaDirect(const int _qsoId)
{
       //qDebug() << "SearchWindow::qslRecViaDirect: " << QString::number(_qsoId)yyyy-MM-dd
    dataProxy->qslRecViaDirect(_qsoId, QDate::currentDate(), false);
    awards->setAwards(_qsoId);
    searchModel->select();
    //refresh();
    emit updateAwards();
}

void SearchWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery)
{
    emit queryError(functionFailed, errorCodeS, errorCodeN, failedQuery);
}

void SearchWindow::slotCheckQRZCom()
{
    QString _qrz = ((searchModel->index( ( (checkQRZCOMFromLogAct->data()).toInt()  ) , 2)).data(Qt::DisplayRole).toString());
    //qDebug() << "SearchWindow::sloTCheckQRZCom: " << _qrz << endl;
    QString url = "https://www.qrz.com/db/" + _qrz;

    QDesktopServices::openUrl(QUrl(url));

}

void SearchWindow::slotCheckDXHeatCom()
{
    //int _qsoId = ((searchModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //QString _qrz = dataProxy->getCallFromId(_qsoId);
    QString _qrz = ((searchModel->index( ( (checkDXHeatFromLogAct->data()).toInt()  ) , 2)).data(Qt::DisplayRole).toString());
      //qDebug() << "SearchWindow::slotCheckDXHeatCom(): " << _qrz << endl;
    QString url = "https://www.dxheat.com/db/" + _qrz;
    QDesktopServices::openUrl(QUrl(url));
}

void SearchWindow::setFilterString(const QString &_st)
{
    //qDebug() << "SearchWindow::setFilterString: " << _st << endl;
    searchModel->setFilterString(_st);


    //qDebug() << "SearchWindow::setFilterString: - END "  << endl;
}

void SearchWindow::selectAll()
{
    //qDebug() << "SearchWindow::selectAll: " << QString::number(searchModel->rowCount()) << endl;
    int rowCount = searchModel->rowCount();
     //logView->selectAll();
     treeView->selectAll();
    for (int i=0;i<rowCount; i++)
    {
        //searchModel->selectRow(i);
    }


}

void SearchWindow::clearSelection()
{
    //qDebug() << "SearchWindow::clearSelection" << endl;
    //logView->clearSelection();
    treeView->clearSelection();

}

QList<int> SearchWindow::getSelectedQSOs()
{
    //qDebug() << "SearchWindow::getSelectedQSOs: (Total: " << QString::number(searchModel->rowCount()) << ")"<< endl;
    QList<int> selectedQSOs;

    //QModelIndexList selection = //logView->selectionModel()->selectedRows();
    QModelIndexList selection = treeView->selectionModel()->selectedRows();
    foreach (QModelIndex index, selection)
    {
        int row, qsoID;
        row = index.row();
        qsoID = ((searchModel->index(row, 0)).data(0)).toInt();
        selectedQSOs.append(qsoID);
    }
    //qDebug() << "SearchWindow::getSelectedQSOs: (Selected: (" << QString::number(selectedQSOs.count()) << ")" << endl;
    return selectedQSOs;

}

void SearchWindow::slotQSLRecMarkAsRequested()
{

    int _qsoId = ((searchModel->index((qslRecRequestedAct->data()).toInt(), 0)).data(0)).toInt();
    dataProxy->qslRecAsRequested(_qsoId, QDate::currentDate());
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        emit updateSearchLineEdit();
    }

    searchModel->select();
}

void SearchWindow::setNeedingQSL(bool const _q)
{
    qslingNeeded = _q;

}

void SearchWindow::slotToolSearchQSL(const int actionQSL)
{
    //qDebug() << "SearchWidget::slotToolSearchQSL: " << QString::number(actionQSL) << " - LogNumber: " << QString::number(currentLog) << endl;
    // 2 means QSL_RCVD = 'R'
    QString stringQuery = QString();
    QString message = QString();
    QString aux = QString();
    QString filter;
    filter = QString();

    //int i = -1;

    switch (actionQSL)
    {
        case 0://void searchToolNeededQSLToSend();
            //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 0" << endl;


            stringQuery = QString("SELECT call, qso_date,dxcc, bandid, modeid, qsl_sent, qsl_rcvd, lotw_qsl_rcvd, station_callsign, id FROM log WHERE (qsl_rcvd<>'Y' AND lotw_qsl_rcvd<>'Y') AND qsl_sent<>'Y' AND qsl_sent<>'Q' AND qsl_sent<>'R' AND lognumber='%1' AND (bandid, dxcc) NOT IN (SELECT distinct bandid, dxcc from log WHERE qsl_rcvd='Y' OR lotw_qsl_rcvd='Y')").arg(currentLog);
            //stringQuery = QString("SELECT call, qso_date, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, log.id FROM log JOIN awarddxcc ON awarddxcc.qsoid=log.id WHERE awarddxcc.confirmed='0' AND log.qsl_sent!='Y' AND log.qsl_sent!='Q' AND log.qsl_sent!='R' AND log.lognumber='%1'").arg(currentLog);
            message = tr("Needed QSO to send the QSL");
            setNeedingQSL(true);
            //qslingNeeded = true;
            emit requestFocus();
        break;
        case 1:
               //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 1" << endl;
            filter = QString("qsl_sent=='R' AND lognumber='%1'").arg(currentLog);
            message = tr("My QSL requested to be sent");
        break;
        case 2://void slotToolSearchNeededQSLPendingToReceive();
               //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 2" << endl;
            filter = QString("lognumber='%1' AND ( (qsl_sent='Y' AND qsl_rcvd!='Y' AND qsl_rcvd!='I') OR qsl_rcvd='R')").arg(currentLog);
            message = tr("DX QSL pending to be received");
        break;
    case 3://void slotToolSearchNeededQSLRequested()
               //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 3" << endl;
        filter = QString("WHERE lognumber='%1' AND  qsl_rcvd='R'").arg(currentLog);
        message = tr("DX QSL pending to be received");
    break;
        default:
               //qDebug() << "SearchWidget::slotToolSearchQSL: CASE DEFAULT" << endl;
        // should never be reached
            return;
//        break;
    }



    clear();
    setFilterString(filter);
}

void SearchWindow::searchToolNeededQSLToSend()
{
       //qDebug() << "SearchWidget::slotToolSearchQSLToSend - TO PREPARE THE QUERY and optimize the function" << endl;
    slotToolSearchQSL(0);
       //qDebug() << "SearchWidget::slotToolSearchQSLToSend - END" << endl;
}

void SearchWindow::slotQSLSentMarkAsRequested()
{
    //qDebug() << "SearchWindow::slotQSLSentMarkAsRequested: " << QString::number( (qslSentRequestedAct->data()).toInt() ) << endl;
   // bool qslSentAsRequested(const int _qsoId, const QString _updateDate);
    int _qsoId = ((searchModel->index((qslSentRequestedAct->data()).toInt(), 0)).data(0)).toInt();

    dataProxy->qslSentAsRequested(_qsoId, QDate::currentDate());
    if(qslingNeeded)
    {
        slotToolSearchQSL(0);
    }
    else
    {
        emit updateSearchText();
    }
    searchModel->select();
}

void SearchWindow::slotQSLSentViaDirectMarkDXReqFromSearch()
{
       //qDebug() << "slotQSLSentViaDirectMarkDXReqFromSearch: " << endl;
    int _qsoId = ((searchModel->index((qslSentViaDirectMarkRcvReqFromSearchAct->data()).toInt(), 0)).data(0)).toInt();
    //int _qsoId = (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toInt();

    dataProxy->qslSentViaDirect(_qsoId, QDate::currentDate());
    dataProxy->qslRecAsRequested(_qsoId, QDate::currentDate());


    if(qslingNeeded)
    {
        slotToolSearchQSL(0);
    }
    else
    {
        emit updateSearchText();
    }
    searchModel->select();
    // Mark Sent, Bureau, date, update log.
}

void SearchWindow::slotQSLSentViaBureauMarkDXReqFromSearch()
{
    //qDebug() << "slotQSLSentViaBureauMarkDXReqFromSearch: " << QString::number( (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt() ) << endl;
     //int _qsoId = (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt();
    int _qsoId = ((searchModel->index((qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt(), 0)).data(0)).toInt();
    dataProxy->qslSentViaBureau(_qsoId, QDate::currentDate());
    dataProxy->qslRecAsRequested(_qsoId, QDate::currentDate());


    if(qslingNeeded)
    {
        slotToolSearchQSL(0);
    }
    else
    {
        emit updateSearchText();
    }
    searchModel->select();
    //qDebug() << "slotQSLSentViaBureauMarkDXReqFromSearch: - END " << endl;
    // Mark Sent, Bureau, date, update log.

}

void SearchWindow::slotQSLRecViaBureauMarkReqFromSearch()
{
    //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog -  Start" << endl;
    //int _qsoId = (qslRecViaBureauMarkReqFromSearchAct->data()).toInt();
    int _qsoId = ((searchModel->index((qslRecViaBureauMarkReqFromSearchAct->data()).toInt(), 0)).data(0)).toInt();
       //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 01" << endl;
    //qslRecViaBureauMarkReq(_qsoId);
    dataProxy->qslRecViaBureau(_qsoId, QDate::currentDate(), true);
       //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 02: n: " << QString::number(_qsoId) << endl;
    if(qslingNeeded)
    {
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 03" << endl;
        slotToolSearchQSL(0);
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 04" << endl;
    }
    else
    {
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 05" << endl;
        emit updateSearchText();
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 06" << endl;
    }
    searchModel->select();
       //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 07" << endl;
}

void SearchWindow::slotQSLRecViaDirectMarkReqFromSearch()
{
       //qDebug() << "SearchWidget::slotQSLRecViaDirectFromLog: " << endl;
     //int _qsoId = (qslRecViaDirectMarkReqFromSearchAct->data()).toInt();
     int _qsoId = ((searchModel->index((qslRecViaDirectMarkReqFromSearchAct->data()).toInt(), 0)).data(0)).toInt();
    //qslRecViaDirectMarkReq(_qsoId);
    dataProxy->qslRecViaDirect(_qsoId, QDate::currentDate(), true);
    if(qslingNeeded)
    {
        slotToolSearchQSL(0);
    }
    else
    {
        emit updateSearchText();
    }
    searchModel->select();
    // Mark Sent, Bureau, date, update log.
}

/*
 void SearchWindow::qslRecViaBureauMarkReq(const int _qsoId)
{
    //qDebug() << "SearchWidget::qslRecViaBureau: " << QString::number(_qsoId) << "/" << QDate::currentDate() << endl;
    dataProxy->qslRecViaBureau(_qsoId, QDate::currentDate(), true);
    //qDebug() << "SearchWidget::qslRecViaBureau: END" << endl;
}
*/

/*
void SearchWindow::qslRecViaDirectMarkReq(const int _qsoId)
{
    //qDebug() << "SearchWidget::qslRecViaDirect: " << QString::number(_qsoId) << endl;
    dataProxy->qslRecViaDirect(_qsoId, QDate::currentDate(), true);
}
*/
/*
void SearchWindow::colorTheList()
{
    //qDebug() << "SearchWidget::colorTheList: " << QString::number(treeView->model()->rowCount()) << endl;
    for (int i = 0; i < treeView->model()->rowCount(); i++)
    {

       //QString _qrz = ((searchModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(1).toString());
       QString _qrz =  (searchModel->index(i, 2)).data(Qt::DisplayRole).toString();

        //QColor getQRZDXStatusColor(const QStringList &_qs); // Receives Entity, band, mode & log
       //QColor color = awards->getQRZDXStatusColor()

       QColor color = QColor(Qt::blue);
        //searchModel->setItemData(searchModel->index(i, 2), color);
        //qDebug() << "SearchWidget::colorTheList: " << _qrz << endl;
    }

}
*/

void SearchWindow::setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default)
{
       //qDebug() << "DXClusterWidget::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default << endl;
    // Just to pass the colors to the awards class
    searchModel->setColors(_newOne,  _needed, _worked,  _confirmed, _default);
}

