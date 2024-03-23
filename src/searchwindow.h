#ifndef KLOG_SEARCHWINDOW_H
#define KLOG_SEARCHWINDOW_H

/***************************************************************************
                          searchwindow.h  -  description
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

#include <QtWidgets>
#include <QWidget>
//#include <QTableView>
#include <QTreeView>
#include <QAction>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QDesktopServices>
#include "dataproxy_sqlite.h"
#include "searchmodel.h"
#include "awards.h"
//#include "elogclublog.h"
#include "utilities.h"

class SearchWindow : public  QWidget
{
    Q_OBJECT

public:
    explicit SearchWindow(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~SearchWindow();
    void createlogPanel(const int _currentLog);
    void clear();
    void refresh();

    void selectAll();
    void clearSelection();
    void setStationCallsignInHeader(const bool _h);


    void qslSentViaBureau(const int _qsoId);    //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaBureau(const int _qsoId);     //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaDirect(const int _qsoId);

    bool isQSLReceived(const int _qsoId);
    bool isQSLSent(const int _qsoId);
    //void setProxyModel (const bool _p);
    void sortColumn(const int _c);
    void setFilterString(const QString &_st);
    QList<int> getSelectedQSOs();
    void setNeedingQSL(bool const _q);
    void slotToolSearchQSL(const int actionQSL);
    void setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default);



signals:
    void actionQSODoubleClicked(const int _qsoid);
    void actionDeleteQSO(const int _qsoid);
    void updateAwards();
    void updateSearchText();
    void updateSearchLineEdit();
    void requestFocus();
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution

private slots:

    void slotDoubleClickLog(const QModelIndex & index);

    void slotRighButtonFromLog(const QPoint& pos);
    void slotQSLSentViaBureauFromLog();
    void slotQSLSentViaDirectFromLog();
    void slotQSLRecViaDirectFromLog();
    void slotQSLRecViaBureauFromLog();
    void slotQSLSentMarkAsRequested();
    void slotQSLRecMarkAsRequested();
    void slotQsoDeleteFromLog();
    void slotQSOToEditFromLog();
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery);
    void slotCheckQRZCom();
    void slotCheckDXHeatCom();

    void slotQSLSentViaDirectMarkDXReqFromSearch();
    void slotQSLSentViaBureauMarkDXReqFromSearch();
    //void slotQSLRecMarkAsRequested();
    void slotQSLRecViaDirectMarkReqFromSearch();
    void slotQSLRecViaBureauMarkReqFromSearch();


private:    
    void createUI();
    void createActionsCommon();
    void createActions();

    //void deleteQSO(const int _qsoID);
    void rightButtonFromLogMenu(const int row);
    void showMenuRightButtonFromLogCreateActions();
    //void slotToolSearchQSL(const int actionQSL);
    void searchToolNeededQSLToSend();
    void setDefaultData();
    void setColumnsToDX();
    //void qslRecViaDirectMarkReq(const int _qsoId);
    //void qslRecViaBureauMarkReq(const int _qsoId);
    //void colorTheList();

    bool qslingNeeded;
    DataProxy_SQLite *dataProxy;
    SearchModel *searchModel;
    Awards *awards;

    //eLogClubLog *elogClublog;

    //QTableView *logView;
    QTreeView *treeView;
    QLabel *logLabel;

    QAction *delQSOFromLogAct;
    QAction *qsoToEditFromLogAct;
    QAction *qslSentViaBureauFromLogAct;
    QAction *qslSentViaDirectFromLogAct;
    QAction *qslRecViaBureauFromLogAct;
    QAction *qslRecViaDirectFromLogAct;
    QAction *checkQRZCOMFromLogAct;
    QAction *checkDXHeatFromLogAct;

    QAction *qslSentRequestedAct;
    QAction *qslSentViaDirectMarkRcvReqFromSearchAct;
    QAction *qslSentViaBureauMarkRcvReqFromSearchAct;
    QAction *qslRecViaBureauMarkReqFromSearchAct;
    QAction *qslRecViaDirectMarkReqFromSearchAct;
    QAction *qslRecRequestedAct;

    int currentLog;

    Utilities *util;
    bool showStationCallsignInHeader;
};

#endif // SEARCHWINDOW_H
