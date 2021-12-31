#ifndef SETUPPAGESUBDIVISION_H
#define SETUPPAGESUBDIVISION_H
/***************************************************************************
                          setuppagesubdivision.h  -  description
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

//#include <QtWidgets>
#include <QtWidgets>
#include <QtSql>
#include <QSqlQuery>
#include "dataproxy_sqlite.h"
//#include "utilities.h"
//#include "setuppages/setuppagesubdivisionnew.h"
#include "fileawardmanager.h"


class SetupPageSubdivisions : public QWidget {
    Q_OBJECT

public:
    SetupPageSubdivisions(DataProxy_SQLite *dp, QWidget *parent=nullptr);
    ~SetupPageSubdivisions();
    int getSelectedLog();
    void setSelectedLog(const int _i);
    void createNewLog();
    void setDefaultStationCallsign (const QString &_p);
    void setDefaultOperators(const QString &_p);


private slots:

    //void slotNewButtonClicked();
    //void slotEditButtonClicked();
    void slotRemoveButtonClicked();
    //void slotAnalyzeNewLogData(const QStringList _qs);
    void slotLogSelected(const QModelIndex & index);
    //void slotLogDoubleClicked(const QModelIndex & index);
    void slotCurrentLogsComboBoxChanged();
    void slotImportButtonClicked();

    //void slotCreateLog();

        //
signals:
   //void exitSignal(const int status); // 1 = OK, -1 = NOK, 2 = Cancel clicked
    //void newLogData(const QStringList _qs); // Station QRZ + Operators to be shown in the main tab
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution
    void focusOK();

private:
    FileAwardManager *fileAwardManager;
    QStringList readLogs();

    void createLogsPanel();
    void createLogsModel();
    void createActions();
    //bool addNewLog(const QStringList _qs);
    void updateSelectedLogs();
    //void readSelectedLog(const int _i);
    void showError(const QString &_errorC);

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
    QPushButton *removePushButton; //*editPushButton, *newLogPushButton,
    QPushButton *importAwardPushButton;
    //*loadAllPushButton, *loadSelectedPushButton, *clearPushButton,;
    int lastLog;
    QString currentLogComment;
    QString currentStationCallSign;
    QComboBox *currentLogs;
    QStringList logsAvailable;

    //SetupPageSubdivisionNew *newLog;

    //QString stationCallsign, operators, comment, dateString;//, typeContest;
    //int contestCatMode, contestCatOperators, contestCatAssisted, contestCatPower, contestCatBands, contestBands;//, typeContestN;

    DataProxy_SQLite *dataProxy;
    //Utilities *util;
    int selectedLog;

    //QString defaultStationCallSign, defaultOperators;


};

#endif // SETUPPAGEREGIONALAWARDS_H
