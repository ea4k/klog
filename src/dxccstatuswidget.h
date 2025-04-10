#ifndef KLOG_DXCCSTATUSWIDGET_H
#define KLOG_DXCCSTATUSWIDGET_H
/***************************************************************************
                          dxccstatuswidget.h  -  description
                             -------------------
    begin                : feb 2016
    copyright            : (C) 2016 by Jaime Robles
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
#include <QWidget>
#include <QtWidgets>
#include <QString>
//#include <QTableView>
#include "awards.h"
#include "world.h"
#include "dataproxy_sqlite.h"
#include "locator.h"
#include "klogdefinitions.h"


//TODO: Creating the widget to show the DXCC status

class DXCCStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DXCCStatusWidget(Awards *awards, QWidget *parent = nullptr);
    //explicit DXCCStatusWidget(DataProxy_SQLite *dp, const QString &_parentFunction, Awards &awards, QWidget *parent = nullptr);
    ~DXCCStatusWidget();

    void update();
    //void awardsUpdated();
    void setBands(const QString &_callingFunc, const QStringList &_listOfNewBands, const bool _creating = false); // Receives the list of bandIDs
    void setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default);
    void setCurrentLog(const int _logN);
    void setMyLocator(const QString &_loc);
    void refresh();

signals:
    //void showQso(const int _qsoid); // identified QSO double clicking on DXCC
    void showQsos(QList<int> _qsos);
    void fillInQSOSignal();
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    void updateAwards();

public slots:
    //void slotSearchLineEditTextChanged();
    void slotRefreshButtonClicked();

private slots:
    //void slotRightButton(const QPoint& pos);
    //void slotWikipedia();
    void slotItemEntered(QTableWidgetItem  * item );
    void slotItemDoubleClicked(QTableWidgetItem  * item );


private:
    void createUI();
    void updateDXCCViewHeaders();               // Updaqtes the header, just for readability
    QStringList filterValidBands(const QStringList &sortedBands);    // Removes a string if not a valid band
    void fillData();                            // Calls all the functions to clean the table & fill the data
    void resetDXCCView();                       // deletes the entities and bands from the table
    void handleDXCCStatusUpdateFailure();       // Shows a message when there is a problem in update()
    void processEntities();                     // Processes the Entities in update();
    QList<int> getBandIds();                    // Return the list of bands
    QIcon getFlagIcon(const QString _isoName);  // Returns the flag icon
    void setDefaultBands();
    void addEntity(const QList<int> &_ent); // DXCC id, bandid, bandid, ...
//    void showMenuRightButtonFromLogCreateActions();
//    void righButtonFromLogMenu(const int trow);

    QTableWidget *dxccView;
    Awards *awards;         // Reference to the Awards instance
    World *world;
    DataProxy_SQLite *dataProxy;
    Locator *locator;


    QHeaderView *hv, *hh;
    //QLineEdit *searchLineEdit;
    QPushButton *refreshButton;
    //QCheckBox *showAllLogsButton;

    int numberOfColumns; // Columns will be number Of Bands + 2 (Id + Name)
    QStringList bandNames;//, validBands;
    int logNumber, tempLog; // log in use in the log / log to be used in the widget
    QString loc; // The locator of the user.

    QAction *showDXCCWikipediaAct;
    int currentLog;
};

#endif // DXCCSTATUSWIDGET_H
