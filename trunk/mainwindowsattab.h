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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
//
// This class implement the tab of the mainwindow that supports Satellites
//

#include <QWidget>
#include <QtWidgets>
#include "dataproxy.h"

class MainWindowSatTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowSatTab(DataProxy *dp, QWidget *parent = 0);
    ~MainWindowSatTab();

    QString getSatName();
    void setSatName(const QString _t);   
    void setOtherSatName(const QString _t);
    QString getOtherSatName();

    QString getSatMode();
    void setSatMode(const QString _t);

    bool getRepeatThis();
    void setRepeatThis(const bool _t);

    void addBands(QStringList _bands);
    void setDefaultBands(); //Defines the default bands for SAT communications: 10m/2m/70cm/23CM only if they exist on the selected bands

    void setUpLink(const QString _t);

    void clear();

signals:
    void setPropModeSat(const QString _p);
    void satBandTXChanged(const QString _p);
    void satBandRXChanged(const QString _p);
    void newBandsToBeAdded(const QStringList _p);
    void txFreqChanged(const QString _p);
    void rxFreqChanged(const QString _p);

private slots:
    void slotSatNameTextChanged();
    void slotSatModeTextChanged();
    void slotSatNameComboBoxChanged();
    void slotSatBandRXComboBoxChanged();
    void slotSatBandTXComboBoxChanged();
    void slotSatFreqRXChanged();
    void slotSatFreqTXChanged();

private:
    void createUI();
    void populateSatComboBox();
    void setSatelliteCombo(const QString _p);
    int getSatIndex(const QString _p);
    void setBandsOfSat(const QString _p);
    void addNewBand(const QString _p);

    QLineEdit *satNameLineEdit;
    QLineEdit *satModeLineEdit;
    QLabel *satOtherLabel;
    QRadioButton *keepThisDataForNextQSORadiobutton;
    //QComboBox *satNameComboBox;
    //QPushButton *satNamePushButon;
    //QComboBox *satNameComboBox, *satModeComboBox;
    QComboBox *satNameComboBox;
    QComboBox *satBandTXComboBox, *satBandRXComboBox;
    QStringList satNames, satModes;
    QStringList satellitesList;

    QDoubleSpinBox *txFreqSpinBox, *rxFreqSpinBox;
    DataProxy *dataProxy;
};

#endif // MAINWINDOWSATTAB_H
