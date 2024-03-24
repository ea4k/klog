#ifndef KLOG_INPUTWIDGETS_MAINWINDOWINPUTQSO_H
#define KLOG_INPUTWIDGETS_MAINWINDOWINPUTQSO_H
/***************************************************************************
                          mainwindowinputqso.h  -  description
                             -------------------
    begin                : may 2021
    copyright            : (C) 2021 by Jaime Robles
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
// This class implement the tab of the mainwindow that supports the QSL options
//

#include <QWidget>
#include <QtWidgets>
#include "../dataproxy_sqlite.h"
//#include "../locator.h"
#include "../utilities.h"

class MainWindowInputQSO : public QWidget
{
    Q_OBJECT
    friend class tst_MainWindowInputQSO;

public:
    explicit MainWindowInputQSO(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~MainWindowInputQSO();

    void setPaletteRightName(const bool _ok);
    void setPaletteRightQTH(const bool _ok);
    void setPaletteRightDXLocator(const bool _ok);

    QString getDXLocator();
    void setDXLocator(const QString &_loc, bool _completing = false);

    QString getName();
    void setName(const QString &_st, bool _completing = false);

    QString getQTH();
    void setQTH(const QString &_st, bool _completing = false);

    void setRSTToMode(const QString &_m, const bool _reading = true);

    QString getRSTTX();
    void setRSTTX(const QString &_st);
    QString getRSTRX();
    void setRSTRX(const QString &_st);

    double getTXFreq();
    void setTXFreq(const double _ft);

    double getRXFreq();
    void setRXFreq(const double _ft);

    void setPropModeFromSat(const QString &_p);

    double getRXPwr();
    void setRXPwr(const double _pw);

    void setModifying(const bool _m);
    void receiveFocus();

    void clear();
    void cleanQRZCOM(const bool _dataFromQRZCOM);
    void clearName();
    void clearQTH();
    void clearDXLocator();

signals:
    void returnPressed();
    void dxLocatorChanged(QString _loc);
    void rxFreqChanged(double _f);
    //void rxFreqChangedForSat(double _f);
    //void txFreqBeingChanged(bool _f);
    void txFreqChanged(double _f);
    void handOverFocusSignal();
    //void txFreqChangedForSat(double _f);

private slots:
    void slotReturnPressed();
    void slotLocatorTextChanged();
    void slotFreqTXChanged (double _f);
    void slotFreqRXChanged (double _f);
    void slotSplitClicked();

private:
    bool eventFilter(QObject *object, QEvent *event);
    void createUI();
    void setDefaultData();
    bool getDarkMode();
    void setSplitCheckBox();


    QLineEdit *rstTXLineEdit, *rstRXLineEdit, *qthLineEdit, *locatorLineEdit, *nameLineEdit;
    QDoubleSpinBox *rxPowerSpinBox,  *txFreqSpinBox, *rxFreqSpinBox;
    QCheckBox *splitCheckBox;

    //Locator *locator;

    DataProxy_SQLite *dataProxy;
    Utilities *util;

    QPalette palRed, palBlack, palWhite; // To paint Text in red or black(normal)

    bool rxFreqBeingAutoChanged, txFreqBeingAutoChanged, isSATPropagation;
    QString propMode;
    double freqTX, freqRX;
    bool modify, completedWithPreviousName, completedWithPreviousQTH, completedWithPreviousLocator;
};

#endif // MAINWINDOWINPUTQSO_H
