#ifndef MAINWINDOWINPUTQSO_H
#define MAINWINDOWINPUTQS=_H
/***************************************************************************
                          mainwindowinputqs0.h  -  description
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
#include <QWidget>
#include "dataproxy_sqlite.h"
#include "locator.h"
//#include "utilities.h"

class MainWindowInputQSO : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowInputQSO(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~MainWindowInputQSO();
    QString getDXLocator();
    void setDXLocator(const QString &_loc);

    void clear();


signals:
    void returnPressed();
    void dxLocatorChanged(QString _loc);

private slots:

    void slotReturnPressed();
    void slotLocatorTextChanged();

private:

    void createUI();
    void setDefaultData();

    QLineEdit *rstTXLineEdit, *rstRXLineEdit, *qthLineEdit, *locatorLineEdit, *nameLineEdit;
    QDoubleSpinBox *rxPowerSpinBox,  *txFreqSpinBox, *rxFreqSpinBox;
    QCheckBox *splitCheckBox;

    Locator *locator;

    DataProxy_SQLite *dataProxy;
    Utilities *util;

    QPalette palRed, palBlack; // To paint Text in red or black(normal)

};

#endif // MAINWINDOWINPUTQSO_H
