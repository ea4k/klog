#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H
/***************************************************************************
                          searchwidget.h  -  description
                             -------------------
    begin                : jul 2017
    copyright            : (C) 2017 by Jaime Robles
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
//
// This class implement the tab of the search widget
//
#include <QWidget>
#include <QtWidgets>
#include "dataproxy.h"
#include "dataproxy_sqlite.h"
#include "awards.h"
#include "world.h"
#include "utilities.h"
#include "filemanager.h"

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = 0);
    void setCurrentLog(const int _log);
    void setColors (const QString _newOne, const QString _needed, const QString _worked, const QString _confirmed, const QString _default);
    void setVersion (const QString _version); // Defines the KLog version to be written in the exported logs
    void setShowCallInSearch(const bool _sh);
    void clear();

    void searchToolNeededQSLToSend();

public slots:
    //void slotQsoFound(QStringList _qso);
    void slotSearchBoxTextChanged();
    void slotToolSearchQSL(const int actionQSL);

    void slotToolSearchRequestedQSLToSend();
    void slotToolSearchNeededQSLPendingToReceive();
    void slotToolSearchNeededQSLRequested();

private slots:

    void slotDoubleClickSearch( QTreeWidgetItem* item, int); // Double click on a QSO in the search box

    void slotSearchExportButtonClicked();
    void slotSearchBoxSelectAllButtonClicked();
    void slotSearchClearButtonClicked();
    void slotSearchBoxSelectionChanged();
    void slotSearchBoxReSearchButtonClicked();

    void slotRighButtonSearch(const QPoint& pos);
    void showMenuRightButtonSearchCreateActions();
    void righButtonSearchMenu(const int trow);

    void slotQsoDeleteFromSearch();
    void slotQSLSentViaBureauFromSearch();
    void slotQSLSentViaDirectFromSearch();
    void slotQSLSentViaDirectMarkDXReqFromSearch();
    void slotQSLSentViaBureuMarkDXReqFromSearch();
    void slotQSLRecViaDirectFromSearch();
    void slotQSLRecViaBureauFromSearch();
    void slotQSLRecViaDirectMarkReqFromSearch();
    void slotQSLRecViaBureauMarkReqFromSearch();
    void slotQSLSentMarkAsRequested();
    void slotQSLRecMarkAsRequested();
    void slotQSOToEditFromSearch();
    void qslRecViaBureauMarkReq(const int _qsoId);
    void qslRecViaDirectMarkReq(const int _qsoId);


signals:
    void actionQSODoubleClicked(const int _qsoid);
    void updateAwards();
    void logRefresh();
    void toStatusBar(const QString statusm);
    void requestBeingShown();
    void actionQSODelete(const int _qsoid);

private:
    void createUI();


    QLineEdit *searchBoxLineEdit;
    QPushButton *searchBoxClearButton, *searchBoxExportButton, *searchBoxSelectAllButton, *searchBoxReSearchButton;
    QRadioButton *searchAllRadioButton;
    QTreeWidget *searchResultsTreeWidget;

    bool qslingNeeded;
    bool searchSelectAllClicked, stationCallSignShownInSearch;
    int currentLog;

    DataProxy *dataProxy;
    Awards *awards;
    World *world;
    Utilities *util;
    FileManager *filemanager;


    QAction *delQSOFromSearchAct;
    QAction *qsoToEditFromSearchAct;
    QAction *qslSentViaBureauFromSearchAct;
    QAction *qslSentViaDirectFromSearchAct;
    QAction *qslSentViaDirectMarkRcvReqFromSearchAct;
    QAction *qslSentViaBureauMarkRcvReqFromSearchAct;
    QAction *qslRecViaBureauFromSearchAct;
    QAction *qslRecViaDirectFromSearchAct;
    QAction *qslRecViaBureauMarkReqFromSearchAct;
    QAction *qslRecViaDirectMarkReqFromSearchAct;

    QAction *qslSentRequestedAct;
    QAction *qslRecRequestedAct;

};

#endif // SEARCHWIDGET_H
