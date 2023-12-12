#ifndef KLOG_AWARDSWIDGET_H
#define KLOG_AWARDSWIDGET_H
/***************************************************************************
                          awardswidget.h  -  description
                             -------------------
    begin                : nov 2019
    copyright            : (C) 2019 by Jaime Robles
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
//
// This class implement the tab of the search widget
//
#include <QWidget>
#include <QtWidgets>
#include "dataproxy_sqlite.h"
#include "awards.h"
//#include "world.h"
//#include "utilities.h"
//#include "filemanager.h"


class AwardsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AwardsWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~AwardsWidget();
    void setManageDXMarathon(const bool _dx);
    void setLog(const int _log);
    void setYear(const int _year);
    void fillOperatingYears();
    void showAwards();

    void clear();

private slots:
     void slotRecalculateAwardsButtonClicked();
     void slotOperatingYearComboBoxChanged();


signals:
    //void actionQSODoubleClicked(const int _qsoid);
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution
    //void recalculateAwardsSignal();
    void requireCurrentLogSignal();
    void requireCurrentYearSignal();


private:
    void createUI();
    void setToolTips();

    void showDXMarathon(const int _year);
    void checkIfValidLog();
    void reconfigureDXMarathonUI(const bool _dxM);

    QLCDNumber *dxccConfirmedQLCDNumber, *dxccWorkedQLCDNumber,
                *wazConfirmedQLCDNumber, *wazWorkedQLCDNumber,
                *localConfirmedQLCDNumber, *localWorkedQLCDNumber,
                *qsoConfirmedQLCDNumber, *qsoWorkedQLCDNumber,
                *yearlyQSOLCDNumber, *yearlyDXCCQLCDNumber, *yearlyCQQLCDNumber, *yearlyScoreQLCDNumber;
    QLabel *yearlyLabelN, *yearlyScoreLabelN;
    QPushButton *recalculateAwardsButton;
    QComboBox *operatingYearsComboBox;

    DataProxy_SQLite *dataProxy;
    bool manageDXMarathon;
    int selectedYear;
    int currentLog;
    DebugLogLevel logLevel;

    Awards *awards;


};

#endif // AWARDSWIDGET_H
