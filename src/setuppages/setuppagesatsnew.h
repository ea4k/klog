#ifndef SETUPPAGESATSNEW_H
#define SETUPPAGESATSNEW_H
/***************************************************************************
                          setuppagesatsnew.h  -  description
                             -------------------
    begin                : dec 2018
    copyright            : (C) 2018 by Jaime Robles
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
// This class implements the Dialog to add a new satellite
//
#include "dataproxy_sqlite.h"

#include <QDialog>
#include <QtWidgets>
#include <QSqlQuery>

//TODO: Read the data when the user clicks the OK button
//TODO: Fill the data from the list of sats when the user wants to edit a sat

class SetupPageSatsNew : public QDialog
{
    Q_OBJECT

public:
    SetupPageSatsNew(DataProxy_SQLite *dp, QWidget *parent = nullptr);

    void setEditing(const bool b);

    void setShortName(const QString st);
    void setName(const QString st);
    void setUpLink(const QString st);
    void setDownLink(const QString st);
    void setModes(const QString st);

    void clear();


private slots:
    void slotOKButtonClicked();
    void slotCancelButtonClicked();

    void slotShortNameTextChanged();
    void slotNameTextChanged();
    void slotUpLinkTextChanged();
    void slotDownLinkTextChanged();
    void slotModesTextChanged();

signals:
    void newSatData(const QStringList _qs); //
    void cancelled(const bool _c); // Cancel button is clicked

private:

    //bool isThereAnyNotManagedSat();
    void createUI();
    void gatherAndSend();
    bool isItAFreq(const QString _st);

    //void fillWithType(const int _n);
    //void updateAllCats();
    //void showOK();
    //void showNOK();
    //void clear();


    DataProxy_SQLite *dataProxy;

    QLineEdit *shortNameLineEdit, *nameLineEdit;
    QLineEdit *uplinkLineEdit, *downlinkLineEdit;
    QLineEdit *modesLineEdit;

    QLabel *shortNameLabel, *nameLabel;
    QLabel *uplinkLabel, *downlinkLabel;
    QLabel *modesLabel;


    QString shortName, name, uplink, downlink, modes;
    bool hasShortName, hasName, hasUplink, hasDownlink, hasModes;

    QPushButton *okButton, *cancelButton;

    QStringList satData;
    bool editing;

};

#endif // SETUPPAGESATSNEW_H
