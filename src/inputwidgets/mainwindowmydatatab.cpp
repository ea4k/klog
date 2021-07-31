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
       //qDebug() << "MainWindowMyDataTab::MainWindowMyDataTab"   << Qt::endl;
    myPowerSpinBox = new QDoubleSpinBox;
    operatorLineEdit = new QLineEdit;
    stationCallSignLineEdit = new QLineEdit;
    myLocatorLineEdit = new QLineEdit;

    myUserADIFLineEdit = new QLineEdit;
    myUserADIFComboBox = new QComboBox;
    keepThisDataForNextQSOQCheckbox = new QCheckBox;

    locator = new Locator();

    lastOperatorQRZ = QString();    // Last QRZ used by the user, will remain if the button is checked and removed if not
    //lastStationQRZ = QString();     // Last QRZ used by the user, will remain if the button is checked and removed if not
    lastMyLocator = QString();      // Last locator used by the user, will remain if the button is checked and removed if not

    stationQRZ = QString();         // Defined in the configuration by the user, will be used if the user configured so in the setup
    operatorQRZ = QString();        // Defined in the configuration by the user, will be used if the user configured so in the setup
    myLocator = QString();          // Defined in the configuration by the user, will be used if the user configured so in the setup
    util = new Utilities;
    modify = false;
    createUI();
    setInitialADIFValues();
    myPower = 0;
    lastPower = 0;
       //qDebug() << "MainWindowMyDataTab::MainWindowMyDataTab - END"   << Qt::endl;

}

MainWindowMyDataTab::~MainWindowMyDataTab(){}

void MainWindowMyDataTab::createUI()
{
    //qDebug() << Q_FUNC_INFO;
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);

    myPowerSpinBox->setDecimals(2);
    myPowerSpinBox->setMaximum(9999);
    myPowerSpinBox->setSuffix(" " + tr("Watts"));

    keepThisDataForNextQSOQCheckbox->setText(tr("Keep this data"));

    QLabel *myPowerSpinBoxLabelN = new QLabel(tr("Power"));
    myPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *operatorLabelN = new QLabel(tr("Operator callsign"));
    operatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *stationCallSignLabelN = new QLabel(tr("Station Callsign"));
    stationCallSignLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *myLocatorLabelN = new QLabel(tr("My Locator"));
    myLocatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);


    QGridLayout *myDataInputTabWidgetLayout = new QGridLayout;
    myDataInputTabWidgetLayout->addWidget(myPowerSpinBoxLabelN, 0, 2);
    myDataInputTabWidgetLayout->addWidget(myPowerSpinBox, 0, 3);
    myDataInputTabWidgetLayout->addWidget(operatorLabelN, 1, 0);
    myDataInputTabWidgetLayout->addWidget(operatorLineEdit, 1, 1);

    myDataInputTabWidgetLayout->addWidget(stationCallSignLabelN, 1, 2);
    myDataInputTabWidgetLayout->addWidget(stationCallSignLineEdit, 1, 3);
    myDataInputTabWidgetLayout->addWidget(myUserADIFComboBox, 2, 0);
    myDataInputTabWidgetLayout->addWidget(myUserADIFLineEdit, 2, 1);
    myDataInputTabWidgetLayout->addWidget(myLocatorLabelN, 2, 2);
    myDataInputTabWidgetLayout->addWidget(myLocatorLineEdit, 2, 3);
    //myDataInputTabWidgetLayout->addWidget(keepLabel, 4, 2);
    myDataInputTabWidgetLayout->addWidget(keepThisDataForNextQSOQCheckbox, 4, 3);

    //myDataInputTabWidgetLayout->setSizeConstraint(QLayout::SetFixedSize);

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
    connect(myUserADIFComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotMyUserADIFComboBoxChanged() ) ) ;

    connect(myUserADIFLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSetCurrentMyUSerData() ) );
}

void MainWindowMyDataTab::clear(bool _full)
{
    //qDebug() << Q_FUNC_INFO;
    modify = false;
    if (!keepThisDataForNextQSOQCheckbox->isChecked())
    {
        //qDebug() << "MainWindowMyDataTab::clear: NOT checked"  << Qt::endl;
        myPowerSpinBox->setValue(myPower);
        if (util->isValidCall(operatorQRZ))
        {
            //qDebug() << Q_FUNC_INFO << ": " << operatorQRZ;
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

        my_rig = QString();
        my_sota = QString();
        my_antenna = QString();
        myUserADIFComboBox->setCurrentIndex(0);
        myUserADIFLineEdit->clear();
    }
    if (_full)
    {
        operatorLineEdit->clear ();
        stationCallSignLineEdit->clear ();
        myLocatorLineEdit->clear ();
        myPowerSpinBox->clear ();
        my_rig = QString();
        my_sota = QString();
        my_antenna = QString();
        my_vucc_grids = QString();
        myUserADIFComboBox->setCurrentIndex(0);
        myUserADIFLineEdit->clear();
        keepThisDataForNextQSOQCheckbox->setChecked (false);
    }
}

void MainWindowMyDataTab::show()
{
    //qDebug() << Q_FUNC_INFO;
    //qDebug() << "MainWindowMyDataTab::show: " << QString::number(myPower) << "/" << operatorQRZ << "/" << stationQRZ << "/" << myLocator << Qt::endl;
    myPowerSpinBox->setValue(myPower);
    operatorLineEdit->setText(operatorQRZ);
    stationCallSignLineEdit->setText(stationQRZ);
    //qDebug() << "MainWindowMyDataTab::show: setMyLocator: " << myLocator  << Qt::endl;
    myLocatorLineEdit->setText(myLocator);

}

void MainWindowMyDataTab::slotMyLocatorTextChanged()
{
    //qDebug() << Q_FUNC_INFO;
     //qDebug() << "MainWindowMyDataTab::slotMyLocatorTextChanged: " << myLocatorLineEdit->text() << Qt::endl;
    //logEvent(Q_FUNC_INFO, "Start", logSeverity);
    int cursorP = myLocatorLineEdit->cursorPosition();
    myLocatorLineEdit->setText(util->getClearSQLi(myLocatorLineEdit->text()).toUpper());
    //qDebug() << "MainWindowMyDataTab::clear: setMyLocator: " << myLocatorLineEdit->text()  << Qt::endl;
    if ( locator->isValidLocator(myLocatorLineEdit->text()))
    {
        if (!modify)
        {
            myLocator = (myLocatorLineEdit->text()).toUpper();
        }

        if (getDarkMode())
        {
            myLocatorLineEdit->setPalette(palWhite);
        }
        else
        {
            myLocatorLineEdit->setPalette(palBlack);
        }

        myLocatorLineEdit->setToolTip(tr("My QTH locator."));
        myLocatorLineEdit->setCursorPosition(cursorP);
        emit myLocChangedSignal(myLocatorLineEdit->text());

        //dxccStatusWidget->setMyLocator(myLocator);
              //qDebug() << "MainWindowMyDataTab::slotMyLocatorTextChanged: My LOCATOR CHANGED TO: " << myLocator << Qt::endl;
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
    //qDebug() << Q_FUNC_INFO;
    emit returnPressed();
}

void MainWindowMyDataTab::setSetupMyPower(const double _power)
{
    //qDebug() << Q_FUNC_INFO;
    myPower = _power;
    myPowerSpinBox->setValue(_power);
}

void MainWindowMyDataTab::setSetupOperator(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO;
    if (!util->isValidCall (_op))
    {
        return;
    }
    operatorQRZ = _op.toUpper();
    operatorLineEdit->setText (operatorQRZ);
}

void MainWindowMyDataTab::setSetupStationQRZ(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op ;
    if (!util->isValidCall (_op))
    {
        return;
    }
    stationQRZ = _op.toUpper();
    stationCallSignLineEdit->setText(stationQRZ);
}

void MainWindowMyDataTab::setSetupMyLocator(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO;
    if (!locator->isValidLocator (_op))
    {
        return;
    }
    myLocator = _op.toUpper();
    myLocatorLineEdit->setText (myLocator);

}

void MainWindowMyDataTab::setMyPower(const double _power)
{
      //qDebug() << "MainWindowMyDataTab::setMyPower: " << QString::number(_power) << Qt::endl;
    //qDebug() << Q_FUNC_INFO;
    myPowerSpinBox->setValue(_power);
}

double MainWindowMyDataTab::getMyPower()
{
    //qDebug() << Q_FUNC_INFO;
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

void MainWindowMyDataTab::setOperator(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    //qDebug() << Q_FUNC_INFO;
    operatorLineEdit->setText(_op);
}

QString MainWindowMyDataTab::getOperator()
{
     //qDebug() << Q_FUNC_INFO << ": " << (operatorLineEdit->text()).toUpper();
    lastOperatorQRZ = (operatorLineEdit->text()).toUpper();
    return lastOperatorQRZ;
}


void MainWindowMyDataTab::setStationQRZ(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    stationCallSignLineEdit->setText(_op);
}

QString MainWindowMyDataTab::getStationQRZ()
{
    //qDebug() << Q_FUNC_INFO << ": " << (stationCallSignLineEdit->text()).toUpper();
    return (stationCallSignLineEdit->text()).toUpper();
    //return lastStationQRZ;
}

void MainWindowMyDataTab::setMyLocator(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    myLocatorLineEdit->setText(_op);
}

QString MainWindowMyDataTab::getMyLocator()
{
    //qDebug() << Q_FUNC_INFO;
    lastMyLocator = (myLocatorLineEdit->text()).toUpper();
    return lastMyLocator.toUpper();
}


void MainWindowMyDataTab::setData(const double _power, const QString &_stationQRZ, const QString &_operator, const QString &_myLocator)
{
    //qDebug() << Q_FUNC_INFO;
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
        setStationQRZ (_stationQRZ);
        //stationQRZ = _stationQRZ;
    }
    else
    {
        stationQRZ = QString();
    }

    if (_operator.length()>0)
    {
        setOperator (_operator);
        //operatorQRZ = _operator;
    }
    else
    {
        operatorQRZ = QString();
    }

    if (_myLocator.length()>0)
    {
        myLocator = _myLocator;
        setMyLocator (_myLocator);
    }
    else
    {
        myLocator = QString();
    }
}

void MainWindowMyDataTab::slotOperatorTextChanged()
{
    //qDebug() << Q_FUNC_INFO << operatorLineEdit->text();
    int cursorP = operatorLineEdit->cursorPosition();

    operatorLineEdit->setText(util->getClearSQLi(operatorLineEdit->text()).toUpper());
    if (util->isValidCall(operatorLineEdit->text()))
    {
        if (getDarkMode())
        {
            operatorLineEdit->setPalette(palWhite);
        }
        else
        {
            operatorLineEdit->setPalette(palBlack);
        }

        if (!modify)
        {
            operatorQRZ = (operatorLineEdit->text());
        }
    }
    else if (operatorLineEdit->text ().isEmpty ())
    {
        if (!modify)
        {
            operatorQRZ.clear ();
        }
    }
    else
    {
        operatorLineEdit->setPalette(palRed);
    }
    operatorLineEdit->setCursorPosition(cursorP);
}

void MainWindowMyDataTab::slotStationCallSignTextChanged()
{
    //qDebug() << Q_FUNC_INFO;
    int cursorP = stationCallSignLineEdit->cursorPosition();

    stationCallSignLineEdit->setText(util->getClearSQLi(stationCallSignLineEdit->text()).toUpper());
    if (util->isValidCall(stationCallSignLineEdit->text()))
    {
        if (getDarkMode())
        {
            stationCallSignLineEdit->setPalette(palWhite);
        }
        else
        {
            stationCallSignLineEdit->setPalette(palBlack);
        }

        if (!modify)
        {
            stationQRZ = (stationCallSignLineEdit->text());
        }
    }
    else
    {
        stationCallSignLineEdit->setPalette(palRed);
    }
    stationCallSignLineEdit->setCursorPosition(cursorP);
}

void MainWindowMyDataTab::setKeep(const bool _b)
{
    //qDebug() << Q_FUNC_INFO;
    keepThisDataForNextQSOQCheckbox->setChecked (_b);
}

bool MainWindowMyDataTab::getKeep()
{
    //qDebug() << Q_FUNC_INFO;
    return keepThisDataForNextQSOQCheckbox->isChecked ();
}

bool MainWindowMyDataTab::setInitialADIFValues()
{
    //qDebug() << Q_FUNC_INFO;
    adifValidTypes.clear ();
    adifValidTypes << "01-" + tr("My Rig") << "02-" + tr("My Antenna") << "03-" + tr("My SOTA_Ref")<< "04-" + tr("My VUCC_GRIDS");
    myUserADIFComboBox->clear ();
    myUserADIFComboBox->addItems (adifValidTypes);
    return true;
}

bool MainWindowMyDataTab::setUserADIFTypeComboBox(const QString &_value)
{
    //qDebug() << Q_FUNC_INFO << ": " << _value;
    if (_value == "MY_RIG")
    {
        myUserADIFComboBox->setCurrentIndex (0);
    }
    else if (_value == "MY_ANTENNA")
    {
        myUserADIFComboBox->setCurrentIndex (1);
    }
    else if (_value == "MY_SOTA_REF")
    {
        myUserADIFComboBox->setCurrentIndex (2);
    }
    else if (_value == "MY_VUCC_GRIDS")
    {
        myUserADIFComboBox->setCurrentIndex (3);
    }
    else
    {
        myUserADIFComboBox->setCurrentIndex (0);
        return false;
    }
    return true;
}

QString MainWindowMyDataTab::getUserADIFTypeComboBox()
{
    //qDebug() << Q_FUNC_INFO;
    int value = (((myUserADIFComboBox->currentText ()).split('-')).at(0)).toInt ();
    switch (value)
    {

    case 1:
        return "MY_RIG";
    case 2:
        return "MY_ANTENNA";
    case 3:
        return "MY_SOTA_REF";
    case 4:
        return "MY_VUCC_GRIDS";
    default:
        return QString();
    }
}

bool MainWindowMyDataTab::setUserADIFValue(const QString &_adifValue)
{
    //qDebug() << Q_FUNC_INFO;
    myUserADIFLineEdit->setText (_adifValue);
    return true;
}

QString MainWindowMyDataTab::getUserADIFValue()
{
    //qDebug() << Q_FUNC_INFO;
    return myUserADIFLineEdit->text();
}

bool MainWindowMyDataTab::setMyRig(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    my_rig = _op;
    slotMyUserADIFComboBoxChanged();
    return true;
}

QString MainWindowMyDataTab::getMyRig()
{
    //qDebug() << Q_FUNC_INFO;
    return my_rig;
}

bool MainWindowMyDataTab::setMyAntenna(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    my_antenna = _op;
    slotMyUserADIFComboBoxChanged();
    return true;
}

QString MainWindowMyDataTab::getMyAntenna()
{
    //qDebug() << Q_FUNC_INFO;
    return my_antenna;
}

bool MainWindowMyDataTab::setMySOTA(const QString &_op)
{
    //qDebug() << Q_FUNC_INFO << ": " << _op;
    my_sota = _op;
    slotMyUserADIFComboBoxChanged();
    return true;
}

QString MainWindowMyDataTab::getMySOTA()
{
    //qDebug() << Q_FUNC_INFO;
    return my_sota;
}

bool MainWindowMyDataTab::setMyVUCCGrids(const QString &_op)
{
    qDebug() << Q_FUNC_INFO << ": " << _op;
    if (checkMyVUCC_GRIDS(_op))
    {
        my_vucc_grids = _op;
        slotMyUserADIFComboBoxChanged();
        return true;
    }
    return false;
}

bool MainWindowMyDataTab::checkMyVUCC_GRIDS(const QString &_string)
{
    qDebug() << Q_FUNC_INFO << ": " << _string;

    if (util->isValidVUCCGrids (_string))
    {
        setColorsForMyUserADIFLineEdit();
        return true;
    }
    else
    {
        myUserADIFLineEdit->setPalette (palRed);
        return false;
    }

}

QString MainWindowMyDataTab::getMyVUCCGrids()
{
    if (checkMyVUCC_GRIDS (my_vucc_grids))
    {
        return my_vucc_grids;
    }
    else
    {
        return QString();
    }
}

void MainWindowMyDataTab::setColorsForMyUserADIFLineEdit()
{
    if (getDarkMode())
    {
        myUserADIFLineEdit->setPalette(palWhite);
    }
    else
    {
        myUserADIFLineEdit->setPalette(palBlack);
    }
}

void MainWindowMyDataTab::slotMyUserADIFComboBoxChanged()
{
    qDebug() << Q_FUNC_INFO << ": " << getUserADIFTypeComboBox ();
    QString currentTag = getUserADIFTypeComboBox ();

    setColorsForMyUserADIFLineEdit();

    if (currentTag == "MY_RIG")
    {
        myUserADIFLineEdit->setText (my_rig);
    }
    else if (currentTag == "MY_ANTENNA")
    {
        myUserADIFLineEdit->setText (my_antenna);
    }
    else if (currentTag == "MY_SOTA_REF")
    {
        myUserADIFLineEdit->setText (my_sota);
    }
    else if (currentTag == "MY_VUCC_GRIDS")
    {
        myUserADIFLineEdit->setText (my_vucc_grids);
    }
}


void MainWindowMyDataTab::slotSetCurrentMyUSerData()
{
    QString currentTag = getUserADIFTypeComboBox ();
    qDebug() << Q_FUNC_INFO << ": " << currentTag;
    int currentPos = myUserADIFLineEdit->cursorPosition ();

    if (currentTag == "MY_RIG")
    {
        my_rig = myUserADIFLineEdit->text();
    }
    else if (currentTag == "MY_ANTENNA")
    {
        my_antenna = myUserADIFLineEdit->text();
    }
    else if (currentTag == "MY_SOTA_REF")
    {
        my_sota = myUserADIFLineEdit->text();
    }
    else if (currentTag == "MY_VUCC_GRIDS")
    {
        if (checkMyVUCC_GRIDS(myUserADIFLineEdit->text()))
        {}

        my_vucc_grids = myUserADIFLineEdit->text().toUpper();
        myUserADIFLineEdit->setText (my_vucc_grids);
    }

    myUserADIFLineEdit->setCursorPosition (currentPos);
}

void MainWindowMyDataTab::setModify(const bool _modify)
{
    //emit debugLog(Q_FUNC_INFO, "Start", Debug);
    modify = _modify;
    //emit debugLog(Q_FUNC_INFO, "END", Debug);
}

bool MainWindowMyDataTab::getDarkMode()
{
    if (operatorLineEdit->palette().color (QPalette::Base) == "#646464")
    {
        return true;
    }
    else
    {
        return false;
    }
}
