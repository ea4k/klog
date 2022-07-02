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
#include <QDialog>
#include <QtWidgets>
#include <QSqlQuery>
#include "../dataproxy_sqlite.h"
#include "../utilities.h"

//TODO: Read the data when the user clicks the OK button
//TODO: Fill the data from the list of logs when the user wants to edit a log

class SetupPageLogsNew : public QDialog
{
    Q_OBJECT

public:
    //SetupPageLogsNew(QWidget *parent = 0);
    SetupPageLogsNew(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~SetupPageLogsNew();
    void setEditing(const bool b);
    void setStationCallSign(const QString &_st);
    void setOperators(const QString &_st);
    void setComment(const QString &_st);
    void setDateString(const QString &_st);

private slots:
    void slotOKButtonClicked();
    void slotCancelButtonClicked();
    void slotStationCallSignTextChanged();
    void slotOperatorsTextChanged();

signals:
    void newLogData(const QStringList _qs); //
    void cancelled(const bool _c); // Cancel button is clicked

private:
    void createWidget();
    void gatherAndSend();
    void showOK();
    void showNOK();
    void clear();


    DataProxy_SQLite *dataProxy;
    Utilities *util;

    QDateEdit *dateEdit;

    QLineEdit *stationCallsignLineEdit;
    QLineEdit *operatorsLineEdit, *commentLineEdit;
    QString stationCallsign, operators, comment, dateString;
    bool stationCallsignFilled, operatorsFilled;

    QPushButton *okButton, *cancelButton;

    QStringList logData;
    bool editing;

    //QLabel *catAsLabel, *catOpLabel, *catModeLabel, *catPowerLabel, *catBandsLabel, *overlayLabel;
    //QLabel *typeLabel, *validCats,
    QLabel *stationCallsignLabel, *operatorsLabel, *commentLabel, *dateLabel;

    bool checking; //, bCass, bCOp, bCMo, bCPo, bCBa, bCOv, bCTy;

};

#endif // SETUPPAGELOGSNEW_H
