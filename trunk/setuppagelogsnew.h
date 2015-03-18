#ifndef SETUPPAGELOGSNEW_H
#define SETUPPAGELOGSNEW_H
/***************************************************************************
                          setuppagelogsnew.h  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
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
//
// This class implements the Dialog to add a new log
//
#include "dataproxy.h"
#include "dataproxy_sqlite.h"

#include <QDialog>
#include <QtGui>

//TODO: Read the data when the user clicks the OK button
//TODO: Fill the data from the list of logs when the user wants to edit a log

class SetupPageLogsNew : public QDialog
{
    Q_OBJECT

public:
    SetupPageLogsNew(QWidget *parent = 0);


private slots:
    void slotOKButtonClicked();
    void slotCancelButtonClicked();

    void slotStationCallSignTextChanged();
    void slotTypeComboBoxChanged(const int _ind);
    void slotCatAssistedComboBoxChanged();
    void slotCatOperatorsComboBoxChanged();
    void slotOperatorsTextChanged();
    void slotCatPowerComboBoxChanged();
    void slotCatBandsComboBoxChanged();
    void slotBandsComboBoxChanged();
    void slotCatModeComboBoxChanged();
    void slotCatOverlayComboBoxChanged();

signals:
    void newLogData(const QStringList _qs); //
private:

    bool isThereAnyNotManagedLog();
    void createWidget();
    void gatherAndSend();
    QStringList getValidCatOptions(const int _currentCat, const int _higherCat);


    DataProxy *dataProxy;

    QDateEdit *dateEdit;

    QLineEdit *stationCallsignLineEdit;
    QLineEdit *operatorsLineEdit, *commentLineEdit;

    QComboBox *typeComboBox;
    QComboBox *contestCatModeComboBox;
    QComboBox *contestCatOperatorsComboBox;
    QComboBox *contestCatAssistedComboBox;
    QComboBox *contestCatPowerComboBox;
    QComboBox *contestCatBandsComboBox;
    QComboBox *contestBandsComboBox;
    QComboBox *contestCatOverlayComboBox;


    QString stationCallsign, operators, comment, dateString, typeConteststr;
    int typeContest, contestCatMode, contestCatOperators, contestCatAssisted, contestCatPower, contestCatBands, contestBands, contestCatOverlay;

    bool stationCallsignFilled, operatorsFilled;
    int typeOperation; // DX, CQ-WW-SSB, CQ-WW-CW, CQ-WPX-SSB, CQ-WPX-CW

    QPushButton *okButton, *cancelButton;

    QStringList logData;


};

#endif // SETUPPAGELOGSNEW_H
