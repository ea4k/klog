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
// This class implements the Dialog to add a new log
//
#include "dataproxy_sqlite.h"
#include "utilities.h"

#include <QDialog>
#include <QtWidgets>
#include <QSqlQuery>

//TODO: Read the data when the user clicks the OK button
//TODO: Fill the data from the list of logs when the user wants to edit a log

class SetupPageLogsNew : public QDialog
{
    Q_OBJECT

public:
    //SetupPageLogsNew(QWidget *parent = 0);
    SetupPageLogsNew(DataProxy_SQLite *dp, QWidget *parent = nullptr);

    void setEditing(const bool b);
    void setStationCallSign(const QString &_st);
    void setOperators(const QString &_st);
    void setComment(const QString &_st);
    void setDateString(const QString &_st);
    //void setTypeN(const int _n);

    //void setType(const QString _st);
    //void setCMode(const int _n);
    //void setCOperators(const int _n);
    //void setCAssisted(const int _n);
    //void setCPower(const int _n);
    //void setCBands(const int _n);
    //void setBands(const int _n);
    //void setCOverlay(const int _n);



private slots:
    void slotOKButtonClicked();
    void slotCancelButtonClicked();

    void slotStationCallSignTextChanged();
    void slotOperatorsTextChanged();
    //void slotTypeComboBoxChanged();
    //void slotCatAssistedComboBoxChanged();
    //void slotCatOperatorsComboBoxChanged();
    //void slotCatPowerComboBoxChanged();
    //void slotCatBandsComboBoxChanged();
    //void slotBandsComboBoxChanged();
    //void slotCatModeComboBoxChanged();
    //void slotCatOverlayComboBoxChanged();


signals:
    void newLogData(const QStringList _qs); //
    void cancelled(const bool _c); // Cancel button is clicked

private:

    //bool isThereAnyNotManagedLog();
    void createWidget();
    void gatherAndSend();
    //QStringList getValidCatOptions(const int _currentCat, const int _higherCat);
    //int getSelectedTypeContest();
    //void fillWithType(const int _n);
    //void updateAllCats();
    void showOK();
    void showNOK();
    void clear();


    DataProxy_SQLite *dataProxy;
    Utilities *util;

    QDateEdit *dateEdit;

    QLineEdit *stationCallsignLineEdit;
    QLineEdit *operatorsLineEdit, *commentLineEdit;

    /*
    QComboBox *typeComboBox;
    QComboBox *contestCatModeComboBox;
    QComboBox *contestCatOperatorsComboBox;
    QComboBox *contestCatAssistedComboBox;
    QComboBox *contestCatPowerComboBox;
    QComboBox *contestCatBandsComboBox;
    QComboBox *contestBandsComboBox;
    QComboBox *contestCatOverlayComboBox;
    */

    QString stationCallsign, operators, comment, dateString;//, typeConteststr;
    //int typeContest, typeContestSelected, contestCatMode, contestCatOperators, contestCatAssisted, contestCatPower, contestCatBands, contestBands, contestCatOverlay;

    bool stationCallsignFilled, operatorsFilled;
    //int typeOperation; // DX, CQ-WW-SSB, CQ-WW-CW, CQ-WPX-SSB, CQ-WPX-CW

    QPushButton *okButton, *cancelButton;

    QStringList logData;
    bool editing;

    //QLabel *catAsLabel, *catOpLabel, *catModeLabel, *catPowerLabel, *catBandsLabel, *overlayLabel;
    //QLabel *typeLabel, *validCats,
    QLabel *stationCallsignLabel, *operatorsLabel, *commentLabel, *dateLabel;

    //nameLabel->setBuddy();

    bool checking; //, bCass, bCOp, bCMo, bCPo, bCBa, bCOv, bCTy;

};

#endif // SETUPPAGELOGSNEW_H
