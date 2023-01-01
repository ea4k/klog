#ifndef MAINWINDOWSATTAB_H
#define MAINWINDOWSATTAB_H
/***************************************************************************
                          mainwindowsattab.h  -  description
                             -------------------
    begin                : jan 2015
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
// This class implement the tab of the mainwindow that supports Satellites
//

#include <QWidget>
#include <QtWidgets>
#include "../dataproxy_sqlite.h"
//#include "../locator.h"
#include "../utilities.h"

class MainWindowSatTab : public QWidget
{
    Q_OBJECT
    friend class tst_MainWindowSatTab;
public:
    explicit MainWindowSatTab(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~MainWindowSatTab();

    QString getSatName();
    void setSatName(const QString &_t);
    void setOtherSatName(const QString &_t);
    void setNoSat();

    QString getOtherSatName();
    QString getSatMode();
    double getRXFreq();
    //double getTXFreq();
    void setSatMode(const QString &_t);

    bool getRepeatThis();
    void setRepeatThis(const bool _t);

    void addBands(QStringList _bands);
    void setDefaultBands(); //Defines the default bands for SAT communications: 10m/2m/70cm/23CM only if they exist in the selected bands

    void setUpLinkFreq(const double _t);
    void setDownLinkFreq(const double _t);
    //void updateTXFreq(const double _f);
    //void updateRXFreq(const double _f);

    //void setLocator(const QString &_t);
    void refreshData();
    void setModifying (const bool _m);
    void setKeep(const bool _b);
    bool getKeep();
    void clear(bool _full = false); //full= false leaves the "keep this data"; full = true clears everything



signals:
    void setPropModeSat(const QString &_p, const bool _keep);
    //void satBandTXChanged(const QString &_p);
    //void satBandRXChanged(const QString &_p);
    void newBandsToBeAdded(const QStringList _p);
    void satTxFreqChanged(const double _p);
    void satRxFreqChanged(const double _p);
    void satTXFreqNeeded(const double _p);
    void satRXFreqNeeded(const double _p);
    //void dxLocatorChanged(const QString &_p);
    void returnPressed();

private slots:
    void slotSatNameTextChanged();
    void slotSatModeTextChanged();
    //void slotSatDXLocTextChanged();
    void slotSatNameComboBoxChanged();
    void slotSatBandRXComboBoxChanged();
    void slotSatBandTXComboBoxChanged();
    //void slotSatFreqRXChanged(const double _f);
    //void slotSatFreqTXChanged(const double _f);
    void slotReturnPressed();
    void slotSatKeepThisDataClicked();

private:
    void createUI();
    void populateSatComboBox();
    void setSatelliteCombo(const QString &_p);
    int getSatIndex(const QString &_p);
    void setBandsOfSat(const QString &_p);
    void addNewBand(const QString &_p);
    void setUpLink(const QString &_t);
    QString bandToLetter(const QString &_band);
    void updateTXFreq(const double _f);
    void updateRXFreq(const double _f);
    void autofillSatMode();


    QLineEdit *satNameLineEdit;
    QLineEdit *satModeLineEdit;
    //QLineEdit *satDXLocatorLineEdit;
    QLabel *satOtherLabel;
    QCheckBox *keepThisDataForNextQSOQcheckbox;

    QComboBox *satNameComboBox;
    QComboBox *satBandTXComboBox, *satBandRXComboBox;
    QStringList satNames, satModes;
    QStringList satellitesList;

    bool updatingBands;
    QPalette palRed, palBlack, palWhite; // To paint Text in red or black(normal)

    //QDoubleSpinBox *txFreqSpinBox, *rxFreqSpinBox;
    DataProxy_SQLite *dataProxy;

    Utilities *util;
    double freqTX, freqRX;

    int downLinkBandId, upLinkBandId;
    QString downLinkBand, upLinkBand;

    bool modifying;
    bool getDarkMode();
};

#endif // MAINWINDOWSATTAB_H
