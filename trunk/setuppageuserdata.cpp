/***************************************************************************
                          userdatapage.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    Foobar is free software: you can redistribute it and/or modify         *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "setuppageuserdata.h"

SetupPageUserDataPage::SetupPageUserDataPage(QWidget *parent) : QWidget(parent){

   locator = new Locator();
   operatorOK = false;
   operatorsOK = false;
   tabWidget = new QTabWidget;
   QWidget *personalTab = new QWidget;
   QWidget *stationTab = new QWidget;

   tabWidget->addTab(personalTab, tr("&Personal data"));
   tabWidget->addTab(stationTab, tr("Station &data"));

   qrzLineEdit = new QLineEdit;
   operatorsLineEdit = new QLineEdit;
   nameLineEdit = new QLineEdit;
   cqzLineEdit = new QLineEdit;
   ituzLineEdit = new QLineEdit;
   myLocatorLineEdit = new QLineEdit;

   defaultPalette = new QPalette;
   //defaultPalette = operatorsLineEdit->palette();
   wrongPalette = new QPalette;
   //wrongPalette->setColor(QPalette::WindowText, setNamedColor(Qt::red));
    //redColor.setNamedColor(Qt::red);
   //redColor->setNamedColor(Qt::red);
   wrongPalette->setColor(QPalette::Text, Qt::red);


   //Personal Tab
   nameLineEdit = new QLineEdit;
   address1LineEdit = new QLineEdit;
   address2LineEdit = new QLineEdit;
   address3LineEdit = new QLineEdit;
   address4LineEdit = new QLineEdit;
   cityLineEdit = new QLineEdit;
   zipLineEdit = new QLineEdit;
   provinceLineEdit = new QLineEdit;
   countryLineEdit = new QLineEdit;

   nameLineEdit->setToolTip(tr("Enter your name"));
   address1LineEdit->setToolTip(tr("Enter your address - 1st line"));
   address2LineEdit->setToolTip(tr("Enter your address - 2nd line"));
   address3LineEdit->setToolTip(tr("Enter your address - 3st line"));
   address4LineEdit->setToolTip(tr("Enter your address - 4nd line"));
   cityLineEdit->setToolTip(tr("Enter your city"));
   zipLineEdit->setToolTip(tr("Enter your zip code"));
   provinceLineEdit->setToolTip(tr("Enter your province or state"));
   countryLineEdit->setToolTip(tr("Enter your country"));

   QLabel *nameLabel = new QLabel(tr("&Name"));
   QLabel *addressLabel = new QLabel(tr("&Address"));
   QLabel *cityLabel = new QLabel(tr("Cit&y"));
   QLabel *zipLabel = new QLabel(tr("&Zip Code"));
   QLabel *provLabel = new QLabel(tr("Pro&v/State"));
   QLabel *countryLabel = new QLabel(tr("Countr&y"));

   nameLabel->setBuddy(nameLineEdit);
   addressLabel->setBuddy(address1LineEdit);
   cityLabel->setBuddy(cityLineEdit);
   zipLabel->setBuddy(zipLineEdit);
   provLabel->setBuddy(provinceLineEdit);
   countryLabel->setBuddy(countryLineEdit);


   //void	addWidget ( QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0 )
   // 0 1 2 3
   QGridLayout *personalLayout = new QGridLayout();

   personalLayout->addWidget(nameLabel, 0, 0);
   personalLayout->addWidget(nameLineEdit, 1, 0);
   personalLayout->addWidget(addressLabel, 2, 0);
   personalLayout->addWidget(address1LineEdit, 3, 0, 1, 2);
   personalLayout->addWidget(address2LineEdit, 4, 0, 1, 2);
   personalLayout->addWidget(address3LineEdit, 5, 0, 1, 2);
   personalLayout->addWidget(address4LineEdit, 6, 0, 1, 2);

   personalLayout->addWidget(cityLabel, 2, 2);
   personalLayout->addWidget(cityLineEdit, 3, 2, 1, 1);
   personalLayout->addWidget(zipLabel, 2, 5);
   personalLayout->addWidget(zipLineEdit, 3, 5, 1, 1);

   personalLayout->addWidget(provLabel, 4, 2);
   personalLayout->addWidget(provinceLineEdit, 5, 2, 1, 1);
   personalLayout->addWidget(countryLabel, 4, 5);
   personalLayout->addWidget(countryLineEdit, 5, 5, 1, 1);

   //personalLayout->addWidget();

   //QHBoxLayout *personalAllLayout = new QHBoxLayout();
   //personalAllLayout->addLayout(nameLayout);
   //personalAllLayout->addLayout(personalLayout);

   personalTab->setLayout(personalLayout);


   // Station Tab
   rig1LineEdit = new QLineEdit;
   rig2LineEdit = new QLineEdit;
   rig3LineEdit = new QLineEdit;
   ant1LineEdit = new QLineEdit;
   ant2LineEdit = new QLineEdit;
   ant3LineEdit = new QLineEdit;
   //powerLineEdit = new QLineEdit;

   myPowerSpinBox = new QDoubleSpinBox;
   myPowerSpinBox->setDecimals(2);
   myPowerSpinBox->setMaximum(9999);

   rig1LineEdit->setToolTip(tr("Enter your rig #1 information"));
   rig2LineEdit->setToolTip(tr("Enter your rig #2 information"));
   rig3LineEdit->setToolTip(tr("Enter your rig #3 information"));
   ant1LineEdit->setToolTip(tr("Enter your antenna #1 information"));
   ant2LineEdit->setToolTip(tr("Enter your antenna #2 information"));
   ant3LineEdit->setToolTip(tr("Enter your antenna #3 information"));
   myPowerSpinBox->setToolTip(tr("Enter your power information"));

   QLabel *rig1Label = new QLabel(tr("&Rig 1"));
   QLabel *rig2Label = new QLabel(tr("R&ig 2"));
   QLabel *rig3Label = new QLabel(tr("Ri&g 3"));
   QLabel *antenna1Label = new QLabel(tr("Antenna &1"));
   QLabel *antenna2Label = new QLabel(tr("Antenna &2"));
   QLabel *antenna3Label = new QLabel(tr("Antenna &3"));
   QLabel *powerLabel = new QLabel(tr("Po&wer"));

   rig1Label->setBuddy(rig1LineEdit);
   rig2Label->setBuddy(rig2LineEdit);
   rig3Label->setBuddy(rig3LineEdit);
   antenna1Label->setBuddy(ant1LineEdit);
   antenna2Label->setBuddy(ant2LineEdit);
   antenna3Label->setBuddy(ant3LineEdit);
   powerLabel->setBuddy(myPowerSpinBox);

   //void	addWidget ( QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0 )
   // 0 1 2 3
   QGridLayout *stationLayout = new QGridLayout();
   stationLayout->addWidget(rig1Label, 0, 0);
   stationLayout->addWidget(rig1LineEdit, 1, 0, 1, 1);
   stationLayout->addWidget(rig2Label, 2, 0);
   stationLayout->addWidget(rig2LineEdit, 3, 0, 1, 1);
   stationLayout->addWidget(rig3Label, 4, 0);
   stationLayout->addWidget(rig3LineEdit, 5, 0, 1, 1);

   stationLayout->addWidget(antenna1Label, 0, 2);
   stationLayout->addWidget(ant1LineEdit, 1, 2, 1, 1);
   stationLayout->addWidget(antenna2Label, 2, 2);
   stationLayout->addWidget(ant2LineEdit, 3, 2, 1, 1);
   stationLayout->addWidget(antenna3Label, 4, 2);
   stationLayout->addWidget(ant3LineEdit, 5, 2, 1, 1);

   stationLayout->addWidget(powerLabel, 0, 4);
   stationLayout->addWidget(myPowerSpinBox, 1, 4);

    stationTab->setLayout(stationLayout);

    //TODO:Defining ALL the tooltips
    qrzLineEdit->setToolTip(tr("Enter the station callsign that will be used for logging"));
    operatorsLineEdit->setToolTip(tr("Enter the operators (comma separated if more than one)."));
    myLocatorLineEdit->setToolTip(tr("Enter the locator of your station. KLog will propose one locator based on your callsign"));


    QLabel *qrzLabel = new QLabel(tr("&QRZ"));
    QLabel *operatorsLabel = new QLabel (tr("&Operators"));
    QLabel *cqzLabel = new QLabel(tr("&CQ Zone"));
    QLabel *ituzLabel = new QLabel(tr("&ITU Zone"));
    myLocatorLabel = new QLabel(tr("&Locator"));

    qrzLabel->setBuddy(qrzLineEdit);
    operatorsLabel->setBuddy(operatorsLineEdit);
    cqzLabel->setBuddy(cqzLineEdit);
    ituzLabel->setBuddy(ituzLineEdit);
    myLocatorLabel->setBuddy(myLocatorLineEdit);

    cqzLineEdit->setInputMask("09");
    ituzLineEdit->setInputMask("09");
    cqzLineEdit->setText("00");
    ituzLineEdit->setText("00");

    //QHBoxLayout *operatorLayout = new QHBoxLayout;
    //operatorLayout->addWidget(qrzLabel);
    //operatorLayout->addWidget(qrzLineEdit);
    //operatorLayout->addWidget(operatorsLabel);
    //operatorLayout->addWidget(operatorsLineEdit);


    //QGridLayout *operatorLayout = new QGridLayout();
    //operatorLayout->addWidget(qrzLabel, 0, 0);
    //operatorLayout->addWidget(qrzLineEdit, 1, 0);
    ////operatorLayout->addWidget(nameLabel, 0, 1);
    ////operatorLayout->addWidget(nameLineEdit, 1, 1);
/*
    QGridLayout *zonesBoxLayout = new QGridLayout;
    zonesBoxLayout->addWidget(myLocatorLabel,0,0);
    zonesBoxLayout->addWidget(cqzLabel,0,1);
    zonesBoxLayout->addWidget(ituzLabel,0,2);
    zonesBoxLayout->addWidget(myLocatorLineEdit,1,0);
    zonesBoxLayout->addWidget(cqzLineEdit,1,1);
    zonesBoxLayout->addWidget(ituzLineEdit,1,2);

    QVBoxLayout *userdataLayout = new QVBoxLayout();
    userdataLayout->addLayout(operatorLayout);
    userdataLayout->addLayout(zonesBoxLayout);

*/
    QGridLayout *userdataLayout = new QGridLayout;
    userdataLayout->addWidget(qrzLabel, 0, 0);
    userdataLayout->addWidget(qrzLineEdit, 1, 0);
    userdataLayout->addWidget(operatorsLabel, 0, 1);
    userdataLayout->addWidget(operatorsLineEdit, 1, 1, 1, -1);
    userdataLayout->addWidget(myLocatorLabel, 3, 0);
    userdataLayout->addWidget(myLocatorLineEdit, 4, 0);
    userdataLayout->addWidget(cqzLabel, 3, 1);
    userdataLayout->addWidget(cqzLineEdit, 4, 1);
    userdataLayout->addWidget(ituzLabel, 3, 2);
    userdataLayout->addWidget(ituzLineEdit, 4, 2);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(userdataLayout);
    mainLayout->addWidget(tabWidget);
    //mainLayout->addStretch(1);

    connect(qrzLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZTextChanged() ) );
    connect(myLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotMyLocatorTextChanged() ) );
    connect(operatorsLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOperatorsChanged() ) );

    setLayout(mainLayout);
}


SetupPageUserDataPage::~SetupPageUserDataPage()
{
    //qDebug() << "SetupPageUserDataPage::~SetupPageUserDataPage" << endl;
}

QString SetupPageUserDataPage::getStationQrz()
{
    operatorOK = world.checkQRZValidFormat(qrzLineEdit->text());
    if (operatorOK)
    {
        return qrzLineEdit->text();
    }
    else
    {
        return QString();
    }
}



void SetupPageUserDataPage::slotQRZTextChanged()
{
    //qDebug() << "SetupPageUserDataPage::slotQRZTextChanged: " << qrzLineEdit->text() << " / Length: " << QString::number((qrzLineEdit->text()).size()) << endl;
    int i = qrzLineEdit->cursorPosition();
    QString _a = qrzLineEdit->text();
    if ((_a.at(i-1)).isSpace())
    {
        qrzLineEdit->setText(_a.remove(i-1, 1));
    }


    qrzLineEdit->setText(((qrzLineEdit->text())).simplified());
    qrzLineEdit->setText((qrzLineEdit->text()).toUpper());

    cqzLineEdit->setText(QString::number(world.getQRZCqz(qrzLineEdit->text())));
    ituzLineEdit->setText(QString::number(world.getQRZItuz(qrzLineEdit->text())));
    myLocatorLineEdit->setText(world.getQRZLocator(qrzLineEdit->text()));

    qrzLineEdit->setCursorPosition(i);

    /*
     if (!locator->isValidLocator(myLocatorLineEdit->text()) )
    {
        myLocatorLineEdit->setText(world.getQRZLocator(qrzLineEdit->text()));
    }
    */
}

int SetupPageUserDataPage::getCQz(){
    return (cqzLineEdit->text()).toInt();
}

int SetupPageUserDataPage::getITUz(){
    return (ituzLineEdit->text()).toInt();
}

bool SetupPageUserDataPage::setStationQrz(const QString _qrz){
    qrzLineEdit->setText((_qrz).toUpper());
    return true;
}

bool SetupPageUserDataPage::setCQz(const int _cqz){
    cqzLineEdit->setText(QString::number(_cqz));
    return true;
}

bool SetupPageUserDataPage::setITUz(const int _ituz){
    ituzLineEdit->setText(QString::number(_ituz));
    return true;
}

void SetupPageUserDataPage::slotMyLocatorTextChanged()
{
    //qDebug() << "SetupPageUserDataPage::slotMyLocatorTextChanged: " << myLocatorLineEdit->text() << endl;

    //int i;
    myLocatorLineEdit->setText(((myLocatorLineEdit->text())).simplified());
    myLocatorLineEdit->setText((myLocatorLineEdit->text()).toUpper());

    if ( ((myLocatorLineEdit->text()).length())   >3   )
    {
        if (!(locator->isValidLocator(myLocatorLineEdit->text()) ))
        {
            myLocatorLabel->setText(tr("&Locator (not valid)"));
        }
        else
        {
            myLocatorLabel->setText(tr("&Locator"));
        }

    }
}

QString SetupPageUserDataPage::getStationLocator()
{
    if (!(locator->isValidLocator(myLocatorLineEdit->text()) ))
    {
        return "";
    }
    else
    {
        return (myLocatorLineEdit->text()).toUpper();
    }

}

bool SetupPageUserDataPage::setStationLocator(const QString _loc)
{

    if (!(locator->isValidLocator(_loc) ))
    {
        return false;
    }
    else
    {
        myLocatorLineEdit->setText((_loc).toUpper());
        return true;
    }
}

QString SetupPageUserDataPage::getName()
{
    return nameLineEdit->text();
}


QStringList SetupPageUserDataPage::getAddress()
{
    QStringList a;
    a.clear();
    a << address1LineEdit->text() << address2LineEdit->text() << address3LineEdit->text() << address4LineEdit->text();
    return a;
}

QString SetupPageUserDataPage::getAddress1()
{
    return address1LineEdit->text();
}

QString SetupPageUserDataPage::getAddress2()
{
    return address2LineEdit->text();
}

QString SetupPageUserDataPage::getAddress3()
{
    return address3LineEdit->text();
}

QString SetupPageUserDataPage::getAddress4()
{
    return address4LineEdit->text();
}

QString SetupPageUserDataPage::getRig1()
{
    return rig1LineEdit->text();

}

QString SetupPageUserDataPage::getRig2()
{
    return rig2LineEdit->text();
}

QString SetupPageUserDataPage::getRig3()
{
    return rig3LineEdit->text();
}

QString SetupPageUserDataPage::getAntenna1()
{
    return ant1LineEdit->text();
}

QString SetupPageUserDataPage::getAntenna2()
{
    return ant2LineEdit->text();
}

QString SetupPageUserDataPage::getAntenna3()
{
    return ant3LineEdit->text();
}

QString SetupPageUserDataPage::getCity()
{
    return cityLineEdit->text();
}


QString SetupPageUserDataPage::getZipCode()
{
    return zipLineEdit->text();
}
QString SetupPageUserDataPage::getProvince()
{
    return provinceLineEdit->text();
}

QString SetupPageUserDataPage::getCountry()
{
    return countryLineEdit->text();
}

bool SetupPageUserDataPage::setName (const QString _aux)
{
    nameLineEdit->setText(_aux);
    return true;
}
bool SetupPageUserDataPage::setAddress (const QStringList _aux)
{

    address1LineEdit->setText(_aux.at(0));
    address2LineEdit->setText(_aux.at(1));
    address3LineEdit->setText(_aux.at(2));
    address4LineEdit->setText(_aux.at(3));
    return true;

}

bool SetupPageUserDataPage::setAddress1 (const QString _aux)
{
    address1LineEdit->setText(_aux);
    return true;

}

bool SetupPageUserDataPage::setAddress2 (const QString _aux)
{
    address2LineEdit->setText(_aux);
    return true;

}
bool SetupPageUserDataPage::setAddress3 (const QString _aux)
{
    address3LineEdit->setText(_aux);
    return true;

}
bool SetupPageUserDataPage::setAddress4 (const QString _aux)
{
    address4LineEdit->setText(_aux);
    return true;

}
bool SetupPageUserDataPage::setCity (const QString _aux)
{
    cityLineEdit->setText(_aux);
    return true;
}
bool SetupPageUserDataPage::setZipCode(const QString _aux)
{
    zipLineEdit->setText(_aux);
    return true;
}
bool SetupPageUserDataPage::setProvince (const QString _aux)
{
    provinceLineEdit->setText(_aux);
    return true;
}
bool SetupPageUserDataPage::setCountry (const QString _aux)
{
    countryLineEdit->setText(_aux);
    return true;
}

QStringList SetupPageUserDataPage::getRigs()
{
    QStringList a;
    a.clear();
    a << rig1LineEdit->text() << rig2LineEdit->text() << rig3LineEdit->text();
    return a;

}
QStringList SetupPageUserDataPage::getAntennas()
{
    QStringList a;
    a.clear();
    a << ant1LineEdit->text() << ant2LineEdit->text() << ant3LineEdit->text();
    return a;
}

QString SetupPageUserDataPage::getPower()
{
    return QString::number(myPowerSpinBox->value());
}


bool SetupPageUserDataPage::setPower(const QString _aux)
{
    myPowerSpinBox->setValue(_aux.toFloat());
    return true;
}

bool SetupPageUserDataPage::setRig1 (const QString _aux)
{
    rig1LineEdit->setText(_aux);
    return true;

}

bool SetupPageUserDataPage::setRig2 (const QString _aux)
{
    rig2LineEdit->setText(_aux);
    return true;

}
bool SetupPageUserDataPage::setRig3 (const QString _aux)
{
    rig3LineEdit->setText(_aux);
    return true;

}

bool SetupPageUserDataPage::setAntenna1 (const QString _aux)
{
    ant1LineEdit->setText(_aux);
    return true;

}

bool SetupPageUserDataPage::setAntenna2 (const QString _aux)
{
    ant2LineEdit->setText(_aux);
    return true;

}

bool SetupPageUserDataPage::setAntenna3 (const QString _aux)
{
    ant3LineEdit->setText(_aux);
    return true;
}

void SetupPageUserDataPage::slotOperatorsChanged()
{
    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged" << endl;
    //QString _operators = operatorsLineEdit->text();
    if (operatorsLineEdit->text().length() < 1)
        return;
    int i = operatorsLineEdit->cursorPosition();
    //QColor defaultColor = (operatorsLineEdit->palette()).color(QPalette::WindowText);

    int ent = -1;

    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-00" << endl;
    QString _a = operatorsLineEdit->text();

    if ((_a.at(i-1)).isSpace())
    {
        operatorsLineEdit->setText(_a.remove(i-1, 1));
    }

    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-01" << endl;

    //operatorsLineEdit->setText(((operatorsLineEdit->text())).simplified());
    //operatorsLineEdit->setText((operatorsLineEdit->text()).toUpper());
    operatorsLineEdit->setText(_a.simplified().toUpper());
    _a = operatorsLineEdit->text();

    QStringList operators = _a.split(",", QString::SkipEmptyParts);

    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-02" << endl;
    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-02.5 Size: " << QString::number(operators.size()) << endl;

    for (int ii = 0; ii < operators.size(); ++ii)
    {
        //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-03 - " << QString::number(ii) << endl;

        operatorsOK = world.checkQRZValidFormat(operators.at(ii));

        //ent = world.getQRZARRLId(operators.at(ii));

        if (operatorsOK)
        {
            //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged: NO  VALID CALL: " <<  operators.at(ii) << endl;
        }
        else
        {
            //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged: VALID CALL: " <<  operators.at(ii) << endl;
        }
    }
    //          cout << fonts.at(i).toLocal8Bit().constData() << endl;

    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-04" << endl;

    if (operatorsOK)
    {
        //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged: VALID FORMAT" << endl;
        //QColor defaultColor = (operatorsLineEdit->palette()).color(QPalette::WindowText);

        operatorsLineEdit->setPalette(*defaultPalette);

    }
    else
    {
         operatorsLineEdit->setPalette(*wrongPalette);
        //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged: NOT VALID FORMAT" << endl;
    }
    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-05" << endl;

 /*
    cqzLineEdit->setText(QString::number(world.getQRZCqz(qrzLineEdit->text())));
    ituzLineEdit->setText(QString::number(world.getQRZItuz(qrzLineEdit->text())));
    myLocatorLineEdit->setText(world.getQRZLocator(qrzLineEdit->text()));


  */
    //operatorsLineEdit->setText(_a);
    operatorsLineEdit->setCursorPosition(i);
    //qDebug() << "SetupPageUserDataPage::slotOperatorsChanged-END" << endl;
}

QString SetupPageUserDataPage::getOperators()
{
    if (operatorsOK)
    {
        return operatorsLineEdit->text();
    }
    else
    {
        return QString();
    }

}
bool SetupPageUserDataPage::setOperators(const QString _aux)
{
    if (checkOperatorsLineQString(_aux))
    {
        operatorsLineEdit->setText(_aux);
        return true;
    }

    return false;

}

bool  SetupPageUserDataPage::checkOperatorsLineQString(const QString _auxLine)
{
    QStringList _aux = _auxLine.split(',');
    for (int ii = 0; ii < _aux.size(); ++ii)
    {
        operatorsOK = world.checkQRZValidFormat(_aux.at(ii));
        if (!operatorsOK)
            return operatorsOK;

    }
    return true;
}
