#ifndef KLOG_INPUTWIDGETS_MAINWINDOWMYDATATAB_H
#define KLOG_INPUTWIDGETS_MAINWINDOWMYDATATAB_H

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
#include "../klogdefinitions.h"
#include "../locator.h"
#include "../utilities.h"
#include "../adif.h"
#include "../dataproxy_sqlite.h"
//#include "dataproxy_sqlite.h"

class MainWindowMyDataTab : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindowMyDataTab(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~MainWindowMyDataTab();

    void createUI();

    void setData(const QString &_stationCallsign, const QString &_operator, const QString &_myLocator);


    //void setSetupStationQRZ(const QString &_op);
    void setSetupMyLocator(const QString &_op);

    void setMyPower(const double _power);
    double getMyPower();
    //double getLastPower();

    void setOperator(const QString &_op);
    QString getOperator();

    //QString getLastOperator();

    void setStationCallsign(const QString &_op);    // Sets ONLY the LineEdit, not the internal/configured stationCallsign
    QString getStationCallsign();

    //QString getLastStationQRZ();

    void setMyLocator(const QString &_op);
    QString getMyLocator();
    bool setMyRig(const QString &_op);
    QString getMyRig();
    bool setMyAntenna(const QString &_op);
    QString getMyAntenna();
    bool setMySOTA(const QString &_op);
    QString getMySOTA();
    bool setMyVUCCGrids(const QString &_op);
    QString getMyVUCCGrids(); // Returns the VUCC list in LOC1, LOC2 or LOC1,LOC2, LOC3, LOC4 format

    bool setMyPota_ref(const QString &_op);
    QString getMyPota_ref();
    bool setMySig(const QString &_op);
    QString getMySig();
    bool setMySig_info(const QString &_op);
    QString getMySig_info();
    bool setMyWWFF_Ref(const QString &_op);
    QString getMyWWFF_Ref();

    bool setUserADIFTypeComboBox(const QString &_value);
    QString getUserADIFTypeComboBox();
    bool setUserADIFValue(const QString &_adifValue);
    QString getUserADIFValue();

    void show(); //Shows the values into the widget
    void clear(bool _full = false); //full= false leaves the "keep this data"; full = true clears everything
    void setKeep(const bool _b);
    bool getKeep();
    void setModify(const bool _modify);
    void setLogLevel (const DebugLogLevel _b);
    void loadSettings();
signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    void myLocChangedSignal(const QString &_q); // MyLocator once updated
    void returnPressed();

private slots:
    void slotMyLocatorTextChanged();
    void slotOperatorTextChanged();
    void slotStationCallSignTextChanged();
    void slotReturnPressed();
    void slotMyUserADIFComboBoxChanged();
    void slotSetCurrentMyUSerData();

private:
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    bool setInitialADIFValues();
    void setColorsForMyUserADIFLineEdit();
    bool checkMyVUCC_GRIDS(const QString &_string);
    QStringList adifValidTypes;

    QDoubleSpinBox *myPowerSpinBox;
    QString stationCallsign, operatorQRZ, myLocator; //Not changed during normal operations. They come from the configuration
    QString lastOperatorQRZ, lastMyLocator; //lastStationQRZ, Change dinamically during execution
    double myPower, lastPower;

    QLineEdit *operatorLineEdit, *stationCallSignLineEdit, *myLocatorLineEdit, *myUserADIFLineEdit;
    QComboBox *myUserADIFComboBox;
    QCheckBox *keepThisDataForNextQSOQCheckbox;

    QPalette palRed, palBlack, palWhite; // To paint Text in red or black(normal)
    Locator *locator;
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    bool modify;
    QString my_rig, my_sota, my_antenna, my_vucc_grids, my_pota_ref, my_sig, my_sig_info, my_wwff_ref;
    DebugLogLevel logLevel;
    bool getDarkMode();
};

#endif // MAINWINDOWMYDATATAB_H
