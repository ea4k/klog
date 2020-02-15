#ifndef SETUPPAGESATS_H
#define SETUPPAGESATS_H
/***************************************************************************
                          setuppagesats.h  -  description
                             -------------------
    begin                : Dec 2018
    copyright            : (C) 2018 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

//#include <QtWidgets>
#include <QtWidgets>
#include <QtSql>
#include <QSqlQuery>
#include "updatesatsdata.h"
#include "dataproxy_sqlite.h"
#include "utilities.h"
#include "setuppagesatsnew.h"



class SetupPageSats : public QWidget {
    Q_OBJECT

public:
    SetupPageSats(DataProxy_SQLite *dp, QWidget *parent=0);
    ~SetupPageSats();
    int getSelectedSat();
    void setSelectedSat(const int _i);
    void createNewSat();
    void setDefaultStationCallsign (const QString _p);
    void setDefaultOperators(const QString _p);


private slots:
    void slotImportButtonClicked();
    void slotExportButtonClicked();
    void slotNewButtonClicked();
    void slotEditButtonClicked();
    void slotRemoveButtonClicked();
    void slotAnalyzeNewSatData(const QStringList _qs);
    void slotSatselected(const QModelIndex & index);
    void slotSatDoubleClicked(const QModelIndex & index);
    //void slotSatsCancelled(const bool _q);

signals:
   //void exitSignal(const int status); // 1 = OK, -1 = NOK, 2 = Cancel clicked
    void newSatData(const QStringList _qs); // Station QRZ + Operators to be shown in the main tab
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution

private:
    //void createActions();

    QStringList readSats();
    //bool readSatsFile();

    void createSatsPanel();
    void createSatsModel();
    void createActions();
    //bool addNewSat(const QStringList _qs);
    void updateSelectedSats();
    //void readSelectedSat(const int _i);
    void showError(const QString &_errorC);


    /* Wizard to create a new Sat*/

    //QWizardPage *createSatTypePage();
    //QWizard *newSatWizard; // Wizard to create a new Sat

    /*^Wizard to create a new satellite^*/


    QSqlRelationalTableModel *satsModel;
    QWidget *satsPanel;
    QTableView *satsView;

    QAbstractButton *finishButton;
    QPushButton *newSatPushButton,  *editPushButton, *removePushButton, *importPushButton, *exportPushButton;

    int lastSat;
    QString currentSatComment;
    QString currentStationCallSign;
    QComboBox *currentSats;
    QStringList satsAvailable;

    QString arrild, satname, uplink, downlink, modes;

    DataProxy_SQLite *dataProxy;
    int selectedSat;

    QString defaultStationCallSign, defaultOperators;

    Utilities *util;
    SetupPageSatsNew *newSat;
};


#endif // SETUPPAGESATS_H
