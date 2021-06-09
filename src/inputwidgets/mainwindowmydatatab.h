#ifndef MAINWINDOWMYDATATAB_H
#define MAINWINDOWMYDATATAB_H

/***************************************************************************
                          mainwindowmydatatab.h  -  description
                             -------------------
    begin                : jul 2016
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
//
// This class implement the tab of the mainwindow that supports MY DATA
//

#include <QWidget>
#include <QtWidgets>
#include "../locator.h"
#include "../utilities.h"
//#include "dataproxy_sqlite.h"
//#include "dataproxy_sqlite.h"

class MainWindowMyDataTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowMyDataTab(QWidget *parent = nullptr);
    ~MainWindowMyDataTab();

    void createUI();

    void setData(const double _power, const QString _stationQRZ, const QString _operator, const QString _myLocator);

    void setSetupMyPower(const double _power);
    void setSetupOperator(const QString _op);
    void setSetupStationQRZ(const QString _op);
    void setSetupMyLocator(const QString _op);

    void setMyPower(const double _power);
    double getMyPower();
    //double getLastPower();

    void setOperator(const QString _op);
    QString getOperator();

    //QString getLastOperator();

    void setStationQRZ(const QString _op);
    QString getStationQRZ();

    //QString getLastStationQRZ();

    void setMyLocator(const QString _op);
    QString getMyLocator();
    //QString getLastMyLocator();

    void show(); //Shows the values into the widget
    void clear();
    void setKeep(const bool _b);
    bool getKeep();

signals:
    void myLocChangedSignal(const QString _q); // MyLocator once updated
    void returnPressed();

private slots:
    void slotMyLocatorTextChanged();
    void slotOperatorTextChanged();
    void slotStationCallSignTextChanged();
    void slotReturnPressed();

private:
    //void setLastOperator(const QString _op);
    //void setLastStationQRZ(const QString _op);
    //void setLastMyLocator(const QString _op);
    //void setLastPower(const double _power);

    QDoubleSpinBox *myPowerSpinBox;
    QString stationQRZ, operatorQRZ, myLocator; //Not changed during normal operations. They come from the configuration
    QString lastOperatorQRZ, lastStationQRZ, lastMyLocator; // Change dinamically during execution
    double myPower, lastPower;
    QLineEdit *operatorLineEdit, *stationCallSignLineEdit, *myLocatorLineEdit;
    QCheckBox *keepThisDataForNextQSOQCheckbox;
    QPalette palRed, palBlack; // To paint Text in red or black(normal)
    Locator *locator;
    Utilities *util;

};

#endif // MAINWINDOWMYDATATAB_H
