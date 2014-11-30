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
#include <QtGui>
#include <QtSql>
#include <QWizard>
#include <QWizardPage>

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

    void slotStationCallSignTextChanged();
    void slotTypeComboBoxChanged(const int _ind);
    void slotCatAssistedComboBoxChanged();
    void slotCatOperatorsComboBoxChanged();
    void slotCatPowerComboBoxChanged();
    void slotCatBandsComboBoxChanged();
    void slotBandsComboBoxChanged();
    void slotCatModeComboBoxChanged();


    //void slotCreateLog();

        //

private:
    //void createActions();
    void readLogs();

    void createLogsPanel();
    void createLogsModel();
    void createActions();

    /* Wizard to create a new log*/

    QWizardPage *createLogTypePage();
    QWizard *newLogWizard; // Wizard to create a new log

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
    QDateEdit *dateEdit;

    QLineEdit *stationCallsignLineEdit, *nameLineEdit;
    QLineEdit *operatorsLineEdit;

    QComboBox *typeComboBox;
    QComboBox *contestCatModeComboBox;
    QComboBox *contestCatOperatorsComboBox;
    QComboBox *contestCatAssistedComboBox;
    QComboBox *contestCatPowerComboBox;
    QComboBox *contestCatBandsComboBox;
    QComboBox *contestBandsComboBox;

    bool stationCallsignFilled, operatorsFilled;
    int typeOperation; // DX, CQ-WW-SSB, CQ-WW-CW, CQ-WPX-SSB, CQ-WPX-CW

};


#endif // SETUPPAGELOGS_H
