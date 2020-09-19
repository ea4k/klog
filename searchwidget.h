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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
//
// This class implement the tab of the search widget
//
#include <QWidget>
#include <QtWidgets>
#include "dataproxy_sqlite.h"
#include "awards.h"
#include "world.h"
#include "utilities.h"
#include "filemanager.h"
#include "searchwindow.h"

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~SearchWidget();
    void setCurrentLog(const int _log);
    //void setColors (const QString _newOne, const QString _needed, const QString _worked, const QString _confirmed, const QString _default);
    void setVersion (const QString _version); // Defines the KLog version to be written in the exported logs
    void setShowCallInSearch(const bool _sh);
    void clear();
    void showQSO(const int _q);
    void showQSOs(QList<int> qsoIdList);
    void setStationCallsign(const QString &_st);

    void searchToolNeededQSLToSend();
    void setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default);

public slots:
    void slotSearchBoxTextChanged();

    void slotToolSearchRequestedQSLToSend();
    void slotToolSearchNeededQSLPendingToReceive();
    void slotToolSearchNeededQSLRequested();

private slots:

    //void slotDoubleClickSearch( QTreeWidgetItem* item, int); // Double click on a QSO in the search box

    void slotSearchExportButtonClicked();
    void slotSearchBoxSelectAllButtonClicked();
    void slotSearchClearButtonClicked();
    void slotSearchBoxSelectionChanged();
    void slotSearchBoxReSearchButtonClicked();

    //void slotRightButtonSearch(const QPoint& pos);
    //void showMenuRightButtonSearchCreateActions();
    //void rightButtonSearchMenu(const int trow);

    void slotQsoDeleteFromSearch(const int _qsoId);
    //void slotQSLSentViaBureauFromSearch();
    void slotQSLSentViaDirectFromSearch();
    void slotQSLSentViaDirectMarkDXReqFromSearch();
    //void slotQSLSentViaBureuMarkDXReqFromSearch();
    void slotQSLRecViaDirectFromSearch();
    void slotQSLRecViaBureauFromSearch();
    //void slotQSLRecViaDirectMarkReqFromSearch();
    //void slotQSLRecViaBureauMarkReqFromSearch();
    //void slotQSLSentMarkAsRequested();
    //void slotQSLRecMarkAsRequested();
    void slotQSOToEditFromSearch(const int _qsoId);
    //void qslRecViaBureauMarkReq(const int _qsoId);
//    void qslRecViaDirectMarkReq(const int _qsoId);
    void slotStationCallsignChanged();
    void slotRadioButtonToggled();

    void slotStartDelayInputTimer();
    void slotDelayInputTimedOut();
    void slotRequestFocus();


signals:
    void actionQSODoubleClicked(const int _qsoid);
    void updateAwards();
    void logRefresh();
    void toStatusBar(const QString statusm);
    void requestBeingShown();
    void actionQSODelete(const int _qsoid);
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution


private:
    void createUI();
    void setNeedingQSL(bool const _q);
    void selectStationCallSign();
    void fillStationCallsignComboBox();
    void setModelFilter();
    // The following  function is adding a line to the search list
    //void addQSOToSearchList(const QString _call, const QString _dateTime, const QString _band, const QString _mode, const QString _qslrx, const QString _qsltx, const QString _stationcallsign, const QString _id, const QColor _color);
    bool fillTheList(const QString _query);

//    void addQSOToTheList(const int _id);

    QString callFilter, stationCallsignFilter, currentLogFilter;
    QLineEdit *searchBoxLineEdit;
    QPushButton *searchBoxClearButton, *searchBoxExportButton, *searchBoxSelectAllButton, *searchBoxReSearchButton;
    QRadioButton *searchAllRadioButton;
    //QTreeWidget *searchResultsTreeWidget;
    QComboBox *stationCallsignComboBox;

    SearchWindow *searchWindow;

    bool qslingNeeded;
    bool searchSelectAllClicked, stationCallSignShownInSearch;
    int currentLog;
    QString mainStationCallsign;
    QString lastSearch;

    DataProxy_SQLite *dataProxy;
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
    //QAction *qslRecViaBureauMarkReqFromSearchAct;
    //QAction *qslRecViaDirectMarkReqFromSearchAct;

    QAction *qslSentRequestedAct;
    QAction *qslRecRequestedAct;

    QTimer *delayInputTimer;



};

#endif // SEARCHWIDGET_H
