#ifndef SETUPPAGELOGS_H
#define SETUPPAGELOGS_H
/***************************************************************************
                          setuppagelogs.h  -  description
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

//#include <QtWidgets>
#include <QtWidgets>
#include <QtSql>
#include "dataproxy.h"
#include "dataproxy_sqlite.h"


#include "setuppagelogsnew.h"

const int CONTEST_DX = 0;
const int CONTEST_CQ_WW_SSB = 1;
const int CONTEST_CQ_WW_CW = 2;
const int CONTEST_CQ_WPX_SSB = 3;
const int CONTEST_CQ_WPX_CW = 4;

class SetupPageLogs : public QWidget {
    Q_OBJECT

public:
    SetupPageLogs(QWidget *parent=0);
    ~SetupPageLogs();
    int getSelectedLog();
    void setSelectedLog(const int _i);
/*
    QString getBands(); // 10m, 12m, 15m
    QString getModes(); //ssb, CW

    void setActiveBands(QStringList q);
    void setActiveModes(QStringList q);
    */

private slots:
/*
    void slotBandActiveItemDoubleClicked ( QListWidgetItem * item );
    void slotBandNotActiveItemDoubleClicked ( QListWidgetItem * item );
    void slotBandSelButtonClicked( );
    void slotBandUnSelButtonClicked( );
    void slotModeSelButtonClicked( );
    void slotModeUnSelButtonClicked( );
*/
    void slotNewButtonClicked();
    void slotEditButtonClicked();
    void slotRemoveButtonClicked();
    void slotAnalyzeNewLogData(const QStringList _qs);
    void slotLogSelected(const QModelIndex & index);
    void slotLogDoubleClicked(const QModelIndex & index);

    //void slotCreateLog();

        //
signals:
   //void exitSignal(const int status); // 1 = OK, -1 = NOK, 2 = Cancel clicked

private:
    //void createActions();
    QStringList readLogs();

    void createLogsPanel();
    void createLogsModel();
    void createActions();
    //bool addNewLog(const QStringList _qs);
    void updateSelectedLogs();
    void readSelectedLog(const int _i);
    void showError(const QString _errorC);

    /* Wizard to create a new log*/

    //QWizardPage *createLogTypePage();
    //QWizard *newLogWizard; // Wizard to create a new log

    /*^Wizard to create a new log^*/


    QSqlRelationalTableModel *logsModel;
    QWidget *logsPanel;
    QTableView *logsView;

    QAbstractButton *finishButton;

    //QWidget *logsWidget;
    //QTreeWidget *logsQTreeWidget;
    //QListWidget *bandsNotActiveListWidget, *bandsActiveListWidget;
    //QListWidget *modesNotActiveListWidget, *modesActiveListWidget;
    //QStringList bands, modes;
    QPushButton *newLogPushButton,  *editPushButton, *removePushButton;
    //*loadAllPushButton, *loadSelectedPushButton, *clearPushButton,;
    int lastLog;
    QString currentLogComment;
    QString currentStationCallSign;
    QComboBox *currentLogs;
    QStringList logsAvailable;

    SetupPageLogsNew *newLog;

    QString stationCallsign, operators, comment, dateString, typeContest;
    int contestCatMode, contestCatOperators, contestCatAssisted, contestCatPower, contestCatBands, contestBands, typeContestN;

    DataProxy *dataProxy;
};


#endif // SETUPPAGELOGS_H
