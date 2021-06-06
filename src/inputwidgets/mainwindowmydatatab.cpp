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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include "mainwindowmydatatab.h"

MainWindowMyDataTab::MainWindowMyDataTab(QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowMyDataTab::MainWindowMyDataTab"   << endl;
    myPowerSpinBox = new QDoubleSpinBox;
    operatorLineEdit = new QLineEdit;
    stationCallSignLineEdit = new QLineEdit;
    myLocatorLineEdit = new QLineEdit;
    keepThisDataForNextQSORadiobutton = new QRadioButton;
    locator = new Locator();

    lastOperatorQRZ = QString();    // Last QRZ used by the user, will remain if the button is checked and removed if not
    lastStationQRZ = QString();     // Last QRZ used by the user, will remain if the button is checked and removed if not
    lastMyLocator = QString();      // Last locator used by the user, will remain if the button is checked and removed if not

    stationQRZ = QString();         // Defined in the configuration by the user, will be used if the user configured so in the setup
    operatorQRZ = QString();        // Defined in the configuration by the user, will be used if the user configured so in the setup
    myLocator = QString();          // Defined in the configuration by the user, will be used if the user configured so in the setup
    util = new Utilities;

    createUI();

    myPower = 0;
    lastPower = 0;
       //qDebug() << "MainWindowMyDataTab::MainWindowMyDataTab - END"   << endl;

}

MainWindowMyDataTab::~MainWindowMyDataTab(){}

void MainWindowMyDataTab::createUI()
{
    //qDebug() << "MainWindowMyDataTab::createUI: "  << endl;
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    myPowerSpinBox->setDecimals(2);
    myPowerSpinBox->setMaximum(9999);
    myPowerSpinBox->setSuffix(" " + tr("Watts"));

    QLabel *keepLabel = new QLabel();
    keepLabel->setText(tr("Keep this data"));
    keepLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    keepLabel->setToolTip(tr("Data entered in this tab will be copied into the next QSO."));

    keepThisDataForNextQSORadiobutton->setToolTip(tr("Data entered in this tab will be copied into the next QSO."));

    myPowerSpinBox->setToolTip(tr("Power used for the QSO in watts."));
    operatorLineEdit->setToolTip(tr("Logging operator's callsign."));
    stationCallSignLineEdit->setToolTip(tr("Callsign used over the air."));
    myLocatorLineEdit->setToolTip(tr("My QTH locator."));

    QLabel *myPowerSpinBoxLabelN = new QLabel(tr("Power"));
    myPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *operatorLabelN = new QLabel(tr("Operator callsign"));
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
    //connect(stationCallSignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(myLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotMyLocatorTextChanged() ) );
    connect(myLocatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(operatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(operatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOperatorTextChanged()) );
    connect(stationCallSignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(stationCallSignLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotStationCallSignTextChanged() ) );
}

void MainWindowMyDataTab::clear()
{
    //qDebug() << "MainWindowMyDataTab::clear: "  << endl;
    if (keepThisDataForNextQSORadiobutton->isChecked())
    {
        //qDebug() << "MainWindowMyDataTab::clear: checked"  << endl;
        //myPowerSpinBox->setValue(lastPower);
        //operatorLineEdit->setText(lastOperatorQRZ.toUpper());
        //stationCallSignLineEdit->setText(lastStationQRZ.toUpper());
        //qDebug() << "MainWindowMyDataTab::clear-1: setMyLocator: " << myLocator  << endl;
        //myLocatorLineEdit->setText(lastMyLocator);
    }
    else
    {
        //qDebug() << "MainWindowMyDataTab::clear: NOT checked"  << endl;
        myPowerSpinBox->setValue(myPower);
        if (util->isValidCall(operatorQRZ))
        {
            operatorLineEdit->setText(operatorQRZ);
        }
        else
        {
            operatorLineEdit->clear();
        }
        if (util->isValidCall(stationQRZ))
        {
            stationCallSignLineEdit->setText(stationQRZ);
        }
        else
        {
            stationCallSignLineEdit->clear();
        }
        if (util->isValidGrid(myLocator))
        {
            myLocatorLineEdit->setText(myLocator);
        }
        else
        {
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
    //qDebug() << "MainWindowMyDataTab::show: setMyLocator: " << myLocator  << endl;
    myLocatorLineEdit->setText(myLocator);
}

void MainWindowMyDataTab::slotMyLocatorTextChanged()
{
     //qDebug() << "MainWindowMyDataTab::slotMyLocatorTextChanged: " << myLocatorLineEdit->text() << endl;
    //logEvent(Q_FUNC_INFO, "Start", logSeverity);
    int cursorP = myLocatorLineEdit->cursorPosition();



    myLocatorLineEdit->setText(util->getClearSQLi(myLocatorLineEdit->text()).toUpper());
    //qDebug() << "MainWindowMyDataTab::clear: setMyLocator: " << myLocatorLineEdit->text()  << endl;
    if ( locator->isValidLocator(myLocatorLineEdit->text()))
    {
        myLocator = (myLocatorLineEdit->text()).toUpper();
        myLocatorLineEdit->setPalette(palBlack);
        myLocatorLineEdit->setToolTip(tr("My QTH locator."));
        myLocatorLineEdit->setCursorPosition(cursorP);
        emit myLocChangedSignal(myLocator);

        //dxccStatusWidget->setMyLocator(myLocator);
              //qDebug() << "MainWindowMyDataTab::slotMyLocatorTextChanged: My LOCATOR CHANGED TO: " << myLocator << endl;
        //slotLocatorTextChanged();
    }
    else
    {
        //logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        myLocatorLineEdit->setPalette(palRed);
        myLocatorLineEdit->setToolTip(tr("My QTH locator. Format should be Maidenhead like IN70AA up to 10 characters."));
        myLocatorLineEdit->setCursorPosition(cursorP);
        return;
    }
    //logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindowMyDataTab::slotReturnPressed()
{
    emit returnPressed();
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
    //qDebug() << "MainWindowMyDataTab::setMyLocator: " << _op << endl;
    myLocatorLineEdit->setText(_op);
}

QString MainWindowMyDataTab::getMyLocator()
{
    lastMyLocator = (myLocatorLineEdit->text()).toUpper();
    return lastMyLocator.toUpper();
}


void MainWindowMyDataTab::setData(const double _power, const QString _stationQRZ, const QString _operator, const QString _myLocator)
{
    //qDebug() << "MainWindowMyDataTab::setData: "  << endl;
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

void MainWindowMyDataTab::slotOperatorTextChanged()
{
    int cursorP = operatorLineEdit->cursorPosition();



    operatorLineEdit->setText(util->getClearSQLi(operatorLineEdit->text()).toUpper());
    if (util->isValidCall(operatorLineEdit->text()))
    {
        operatorLineEdit->setPalette(palBlack);
        operatorQRZ = (operatorLineEdit->text());
    }
    else
    {
        operatorLineEdit->setPalette(palRed);
    }
    operatorLineEdit->setCursorPosition(cursorP);
}

void MainWindowMyDataTab::slotStationCallSignTextChanged()
{
    int cursorP = stationCallSignLineEdit->cursorPosition();


    stationCallSignLineEdit->setText(util->getClearSQLi(stationCallSignLineEdit->text()).toUpper());
    if (util->isValidCall(stationCallSignLineEdit->text()))
    {
        stationCallSignLineEdit->setPalette(palBlack);
        stationQRZ = (stationCallSignLineEdit->text());
    }
    else
    {
        stationCallSignLineEdit->setPalette(palRed);
    }
    stationCallSignLineEdit->setCursorPosition(cursorP);
}

void MainWindowMyDataTab::setKeep(const bool _b)
{
    keepThisDataForNextQSORadiobutton->setChecked (_b);
}

bool MainWindowMyDataTab::getKeep()
{
    return keepThisDataForNextQSORadiobutton->isChecked ();
}
