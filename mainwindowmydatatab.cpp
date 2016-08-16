/***************************************************************************
                          mainwindowmydatatab.cpp  -  description
                             -------------------
    begin                : Jul 2016
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include "mainwindowmydatatab.h"

MainWindowMyDataTab::MainWindowMyDataTab(QWidget *parent) :
    QWidget(parent)
{
    myPowerSpinBox = new QDoubleSpinBox;
    operatorLineEdit = new QLineEdit;
    stationCallSignLineEdit = new QLineEdit;
    myLocatorLineEdit = new QLineEdit;
    keepThisDataForNextQSORadiobutton = new QRadioButton;

    lastOperatorQRZ = QString();    // Last QRZ used by the user, will remain if the button is checked and removed if not
    lastStationQRZ = QString();     // Last QRZ used by the user, will remain if the button is checked and removed if not
    lastMyLocator = QString();      // Last locator used by the user, will remain if the button is checked and removed if not

    stationQRZ = QString();         // Defined in the configuration by the user, will be used if the user configured so in the setup
    operatorQRZ = QString();        // Defined in the configuration by the user, will be used if the user configured so in the setup
    myLocator = QString();          // Defined in the configuration by the user, will be used if the user configured so in the setup

    createUI();

    myPower = 0;
    lastPower = 0;

}

MainWindowMyDataTab::~MainWindowMyDataTab(){}

void MainWindowMyDataTab::createUI()
{
    myPowerSpinBox->setDecimals(2);
    myPowerSpinBox->setMaximum(9999);

    QLabel *keepLabel = new QLabel();
    keepLabel->setText(tr("Keep this data"));
    keepLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    keepLabel->setToolTip(tr("Data entered in this tab will be copied into the next QSO"));

    keepThisDataForNextQSORadiobutton->setToolTip(tr("Data entered in this tab will be copied into the next QSO"));

    myPowerSpinBox->setToolTip(tr("Power used for the QSO in watts"));
    operatorLineEdit->setToolTip(tr("Logging operator's callsign"));
    stationCallSignLineEdit->setToolTip(tr("Callsign used over the air"));
    myLocatorLineEdit->setToolTip(tr("My QTH locator"));

    QLabel *myPowerSpinBoxLabelN = new QLabel(tr("Power"));
    myPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *operatorLabelN = new QLabel(tr("Operator"));
    operatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *stationCallSignLabelN = new QLabel(tr("Station Callsign"));

    stationCallSignLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *myLocatorLabelN = new QLabel(tr("My Locator"));

    myLocatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QGridLayout *myDataInputTabWidgetLayout = new QGridLayout;
    myDataInputTabWidgetLayout->addWidget(myPowerSpinBoxLabelN, 0, 0);
    myDataInputTabWidgetLayout->addWidget(operatorLabelN, 1, 0);
    myDataInputTabWidgetLayout->addWidget(stationCallSignLabelN, 2, 0);
    myDataInputTabWidgetLayout->addWidget(myLocatorLabelN, 3, 0);

    myDataInputTabWidgetLayout->addWidget(myPowerSpinBox, 0, 1);
    myDataInputTabWidgetLayout->addWidget(operatorLineEdit, 1, 1);
    myDataInputTabWidgetLayout->addWidget(stationCallSignLineEdit, 2, 1);
    myDataInputTabWidgetLayout->addWidget(myLocatorLineEdit, 3, 1);
    myDataInputTabWidgetLayout->addWidget(keepLabel, 4, 1);
    myDataInputTabWidgetLayout->addWidget(keepThisDataForNextQSORadiobutton, 4, 2);

    setLayout(myDataInputTabWidgetLayout);

    //myDataInputTabWidget->setLayout(myDataInputTabWidgetLayout);
   // i = dxUpLeftTab->addTab(myDataInputTabWidget, tr("My Data"));

}

void MainWindowMyDataTab::clear(const bool _keepMyData)
{
    // _keepMyData comes from the setup
    if (_keepMyData)
    {
       //qDebug() << "MainWindowMyDataTab::clear:  TRUE"  << endl;
    }
    else
    {
       //qDebug() << "MainWindowMyDataTab::clear:  FALSE"  << endl;
    }

    if (keepThisDataForNextQSORadiobutton->isChecked())
    {
        myPowerSpinBox->setValue(lastPower);
        operatorLineEdit->setText(lastOperatorQRZ.toUpper());
        stationCallSignLineEdit->setText(lastStationQRZ.toUpper());
        myLocatorLineEdit->setText(lastMyLocator);
    }
    else
    {
        if (_keepMyData)
        {
            myPowerSpinBox->setValue(myPower);
            operatorLineEdit->setText(operatorQRZ.toUpper());
            stationCallSignLineEdit->setText(stationQRZ.toUpper());
            myLocatorLineEdit->setText(myLocator);
        }
        else
        {
            myPowerSpinBox->setValue(0);
            operatorLineEdit->clear();
            stationCallSignLineEdit->clear();
            myLocatorLineEdit->clear();
        }
    }
}

void MainWindowMyDataTab::show()
{

    //qDebug() << "MainWindowMyDataTab::show: " << QString::number(myPower) << "/" << operatorQRZ << "/" << stationQRZ << "/" << myLocator << endl;
    myPowerSpinBox->setValue(myPower);
    operatorLineEdit->setText(operatorQRZ);
    stationCallSignLineEdit->setText(stationQRZ);
    myLocatorLineEdit->setText(myLocator);
}

void MainWindowMyDataTab::setSetupMyPower(const double _power)
{
    myPower = _power;
}

void MainWindowMyDataTab::setSetupOperator(const QString _op)
{
    operatorQRZ = _op.toUpper();
}

void MainWindowMyDataTab::setSetupStationQRZ(const QString _op)
{
    stationQRZ = _op.toUpper();
}

void MainWindowMyDataTab::setSetupMyLocator(const QString _op)
{
    myLocator = _op.toUpper();
}

void MainWindowMyDataTab::setMyPower(const double _power)
{
   //qDebug() << "MainWindowMyDataTab::setMyPower: " << QString::number(_power) << endl;
    myPowerSpinBox->setValue(_power);
}

double MainWindowMyDataTab::getMyPower()
{
    if (myPowerSpinBox->value() > 0)
    {
      lastPower = myPowerSpinBox->value();
      return lastPower;
    }
    else
    {
        return 0.0;
    }

    //return myPowerSpinBox->value();
}

void MainWindowMyDataTab::setOperator(const QString _op)
{
   //qDebug() << "MainWindowMyDataTab::setOperator: " << _op << endl;
    operatorLineEdit->setText(_op);
}

QString MainWindowMyDataTab::getOperator()
{
    lastOperatorQRZ = (operatorLineEdit->text()).toUpper();
    return lastOperatorQRZ;
}


void MainWindowMyDataTab::setStationQRZ(const QString _op)
{
   //qDebug() << "MainWindowMyDataTab::setStationQRZ: " << _op << endl;
    stationCallSignLineEdit->setText(_op);
}

QString MainWindowMyDataTab::getStationQRZ()
{
    lastStationQRZ = (stationCallSignLineEdit->text()).toUpper();
    return lastStationQRZ.toUpper();
}

void MainWindowMyDataTab::setMyLocator(const QString _op)
{
    myLocatorLineEdit->setText(_op);
}

QString MainWindowMyDataTab::getMyLocator()
{
    lastMyLocator = (myLocatorLineEdit->text()).toUpper();
    return lastMyLocator.toUpper();
}


void MainWindowMyDataTab::setData(const double _power, const QString _stationQRZ, const QString _operator, const QString _myLocator)
{
    if (_power > 0.0)
    {
        myPower = _power;
    }
    else
    {
        myPower = 0;
    }

    if (_stationQRZ.length()>0)
    {
        stationQRZ = _stationQRZ;
    }
    else
    {
        stationQRZ = QString();
    }

    if (_operator.length()>0)
    {
        operatorQRZ = _operator;
    }
    else
    {
        operatorQRZ = QString();
    }

    if (_myLocator.length()>0)
    {
        myLocator = _myLocator;
    }
    else
    {
        myLocator = QString();
    }
}
