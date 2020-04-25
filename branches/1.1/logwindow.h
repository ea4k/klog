#ifndef LOGWINDOW_H
#define LOGWINDOW_H

/***************************************************************************
                          logwindow.h  -  description
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

#include <QtWidgets>
#include <QWidget>
//#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QAction>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QDesktopServices>
#include "dataproxy_sqlite.h"
#include "logmodel.h"
#include "awards.h"
//#include "dxccstatuswidget.h"
#include "elogclublog.h"
//#include "logviewsortfilterproxymodel.h"

//class  LogViewSortFilterProxyModel;

class LogWindow : public  QWidget
{
    Q_OBJECT

public:
    explicit LogWindow(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~LogWindow();
    void createlogPanel(const int _currentLog);
    void clear();
    void refresh();
    void setCurrentLog(const int _currentLog);

    void qslSentViaBureau(const int _qsoId);    //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaBureau(const int _qsoId);     //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaDirect(const int _qsoId);

    bool isQSLReceived(const int _qsoId);
    bool isQSLSent(const int _qsoId);
    //void setProxyModel (const bool _p);
    void sortColumn(const int _c);

signals:
    void actionQSODoubleClicked(const int _qsoid);
    void updateAwards();
    void updateSearchText();   
    //void qsoFound(const QStringList _qs); // Each: QString with format: Fieldname:value
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution
    //void clearError();

private slots:

    void slotDoubleClickLog(const QModelIndex & index);

    void slotRighButtonFromLog(const QPoint& pos);
    void slotQSLSentViaBureauFromLog();
    void slotQSLSentViaDirectFromLog();
    void slotQSLRecViaDirectFromLog();
    void slotQSLRecViaBureauFromLog();
    void slotQsoDeleteFromLog();
    void slotQSOToEditFromLog();
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery);
    void slotCheckQRZCom();
    void slotCheckDXHeatCom();


private:    
    void createUI();
    void createActionsCommon();
    void createActions();

    void deleteQSO(const int _qsoID);
    void righButtonFromLogMenu(const int trow);
    void showMenuRightButtonFromLogCreateActions();


    void setDefaultData();
    void setColumnsToDX();


    DataProxy_SQLite *dataProxy;
    LogModel *logModel;
    Awards *awards;
    //DXCCStatusWidget *dxccStatusWidget;
    eLogClubLog *elogClublog;

    QTableView *logView;
    QLabel *logLabel;

    QAction *delQSOFromLogAct;
    QAction *qsoToEditFromLogAct;
    QAction *qslSentViaBureauFromLogAct;
    QAction *qslSentViaDirectFromLogAct;
    QAction *qslRecViaBureauFromLogAct;
    QAction *qslRecViaDirectFromLogAct;
    QAction *checkQRZCOMFromLogAct;
    QAction *checkDXHeatFromLogAct;

    int currentLog;

   //LogViewSortFilterProxyModel *proxyModel;
   //bool sortingThroughProxyModel;
};



#endif // LOGWINDOW_H
