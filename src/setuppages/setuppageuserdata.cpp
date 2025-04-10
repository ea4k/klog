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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "setuppageuserdata.h"
#include "../callsign.h"


SetupPageUserDataPage::SetupPageUserDataPage(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent){
  //qDebug() << Q_FUNC_INFO << " - Start";
   slotQRZRunning = false;
   locator = new Locator();
   util = new Utilities(Q_FUNC_INFO);
   dataProxy = dp;
   world = new World(dataProxy, Q_FUNC_INFO);
   mainCallOK = false;
   tabWidget = new QTabWidget;
   QWidget *personalTab = new QWidget;
   QWidget *stationTab = new QWidget;

   tabWidget->addTab(personalTab, tr("&Personal data"));
   tabWidget->addTab(stationTab, tr("Station &data"));

   maincallsignLineEdit = new QLineEdit;
   operatorsLineEdit = new QLineEdit;
   nameLineEdit = new QLineEdit;
   cqzLineEdit = new QLineEdit;
   ituzLineEdit = new QLineEdit;
   myLocatorLineEdit = new QLineEdit;

   defaultPalette = new QPalette;
   wrongPalette = new QPalette;
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

   nameLineEdit->setToolTip(tr("Enter your name."));
   address1LineEdit->setToolTip(tr("Enter your address - 1st line."));
   address2LineEdit->setToolTip(tr("Enter your address - 2nd line."));
   address3LineEdit->setToolTip(tr("Enter your address - 3rd line."));
   address4LineEdit->setToolTip(tr("Enter your address - 4th line."));
   cityLineEdit->setToolTip(tr("Enter your city."));
   zipLineEdit->setToolTip(tr("Enter your zip code."));
   provinceLineEdit->setToolTip(tr("Enter your province or state."));
   countryLineEdit->setToolTip(tr("Enter your country."));

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

   personalTab->setLayout(personalLayout);

   // Station Tab
   rig1LineEdit = new QLineEdit;
   rig2LineEdit = new QLineEdit;
   rig3LineEdit = new QLineEdit;
   ant1LineEdit = new QLineEdit;
   ant2LineEdit = new QLineEdit;
   ant3LineEdit = new QLineEdit;

   myPowerSpinBox = new QDoubleSpinBox;
   myPowerSpinBox->setDecimals(2);
   myPowerSpinBox->setMaximum(9999);

   rig1LineEdit->setToolTip(tr("Enter your information for rig") + " #1.");
   rig2LineEdit->setToolTip(tr("Enter your information for rig") + " #2.");
   rig3LineEdit->setToolTip(tr("Enter your information for rig") + " #3.");
   ant1LineEdit->setToolTip(tr("Enter your information for antenna") + " #1.");
   ant2LineEdit->setToolTip(tr("Enter your information for antenna") + " #2.");
   ant3LineEdit->setToolTip(tr("Enter your information for antenna") + " #3.");
   myPowerSpinBox->setToolTip(tr("Enter your power information."));

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
    maincallsignLineEdit->setToolTip(tr("Enter the station callsign that will be used for logging."));
    operatorsLineEdit->setToolTip(tr("Enter the operators (comma separated if more than one)."));
    myLocatorLineEdit->setToolTip(tr("Enter the locator of your station. Alternatively, KLog can use an approximate locator based on your callsign."));

    QLabel *qrzLabel = new QLabel(tr("&Callsign"));
    QLabel *operatorsLabel = new QLabel (tr("&Operators"));
    QLabel *cqzLabel = new QLabel(tr("&CQ Zone"));
    QLabel *ituzLabel = new QLabel(tr("&ITU Zone"));
    myLocatorLabel = new QLabel(tr("&Locator"));

    qrzLabel->setBuddy(maincallsignLineEdit);
    operatorsLabel->setBuddy(operatorsLineEdit);
    cqzLabel->setBuddy(cqzLineEdit);
    ituzLabel->setBuddy(ituzLineEdit);
    myLocatorLabel->setBuddy(myLocatorLineEdit);

    cqzLineEdit->setInputMask("09");
    ituzLineEdit->setInputMask("09");
    cqzLineEdit->setText("00");
    ituzLineEdit->setText("00");

    QGridLayout *userdataLayout = new QGridLayout;
    userdataLayout->addWidget(qrzLabel, 0, 0);
    userdataLayout->addWidget(maincallsignLineEdit, 1, 0);
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

    connect(maincallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZTextChanged() ) );
    connect(maincallsignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(operatorsLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(cqzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(ituzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(myLocatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );

    connect(address1LineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(address2LineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(address3LineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(address4LineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(cityLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(zipLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(provinceLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(countryLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );

    connect(myLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotMyLocatorTextChanged() ) );
    connect(operatorsLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOperatorsChanged() ) );

    setLayout(mainLayout);
    maincallsignLineEdit->setFocus();

    //qDebug() << Q_FUNC_INFO << " - END";
}


SetupPageUserDataPage::~SetupPageUserDataPage()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    delete(locator);
    delete(util);
    //delete(world);
    delete(defaultPalette);
    delete(wrongPalette);
   //delete(dataProxy);
}

void SetupPageUserDataPage::setPrefixes()
{
    //util->setLongPrefixes(dataProxy->getLongPrefixes());
    //util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    //world->readWorld ();
}

QString SetupPageUserDataPage::getMainCallsign()
{
    Callsign callsign (maincallsignLineEdit->text());
    if (callsign.isValid())
    {
        return maincallsignLineEdit->text().toUpper();
    }
    else
    {
        return QString();
    }
}

void SetupPageUserDataPage::slotEnterKeyPressed()
{
    emit enterKey();
}

void SetupPageUserDataPage::slotQRZTextChanged()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (slotQRZRunning)
    {
       //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    slotQRZRunning = true;
   //qDebug() << Q_FUNC_INFO << " - " << maincallsignLineEdit->text() << " / Length: " << QString::number((maincallsignLineEdit->text()).size());

    int i = maincallsignLineEdit->cursorPosition();

    QString _a = util->getClearSQLi (maincallsignLineEdit->text());
    if (i<1)
    {
        slotQRZRunning = false;
        //qDebug() << Q_FUNC_INFO << " - END-2";
        return;
    }

    if ((_a.at(i-1)).isSpace())
    {
        maincallsignLineEdit->setText(_a.remove(i-1, 1));
    }

    maincallsignLineEdit->setText(((maincallsignLineEdit->text())).simplified());
    maincallsignLineEdit->setText((maincallsignLineEdit->text()).toUpper());

    cqzLineEdit->setText(QString::number(world->getQRZCqz(maincallsignLineEdit->text())));
    ituzLineEdit->setText(QString::number(world->getQRZItuz(maincallsignLineEdit->text())));
    myLocatorLineEdit->setText(world->getQRZLocator(maincallsignLineEdit->text()));

    maincallsignLineEdit->setCursorPosition(i);

    emit mainCallsignSignal(maincallsignLineEdit->text());


    /*
     if (!locator->isValidLocator(myLocatorLineEdit->text()) )
    {
        myLocatorLineEdit->setText(world->getQRZLocator(maincallsignLineEdit->text()));
    }
    */
    //qDebug() << Q_FUNC_INFO << " - END-3";
    slotQRZRunning = false;
}

int SetupPageUserDataPage::getCQz()
{
    return (cqzLineEdit->text()).toInt();
}

int SetupPageUserDataPage::getITUz()
{
    return (ituzLineEdit->text()).toInt();
}

bool SetupPageUserDataPage::setMainCallsign(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    Callsign callsign(_qrz);
    if (!callsign.isValid())
        return false;
    maincallsignLineEdit->setText((_qrz).toUpper());
    return true;
}

bool SetupPageUserDataPage::setCQz(const int _cqz)
{
    cqzLineEdit->setText(QString::number(_cqz));
    return true;
}

bool SetupPageUserDataPage::setITUz(const int _ituz)
{
    ituzLineEdit->setText(QString::number(_ituz));
    return true;
}

void SetupPageUserDataPage::slotMyLocatorTextChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";

    myLocatorLineEdit->setText(((util->getClearSQLi(myLocatorLineEdit->text()))).simplified());
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

bool SetupPageUserDataPage::setStationLocator(const QString &_loc)
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

bool SetupPageUserDataPage::setName (const QString &_aux)
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

bool SetupPageUserDataPage::setAddress1 (const QString &_aux)
{
    address1LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setAddress2 (const QString &_aux)
{
    address2LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setAddress3 (const QString &_aux)
{
    address3LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setAddress4 (const QString &_aux)
{
    address4LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setCity (const QString &_aux)
{
    cityLineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setZipCode(const QString &_aux)
{
    zipLineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setProvince (const QString &_aux)
{
    provinceLineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setCountry (const QString &_aux)
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

double SetupPageUserDataPage::getPower()
{
    return myPowerSpinBox->value();
}


bool SetupPageUserDataPage::setPower(const float _aux)
{
    myPowerSpinBox->setValue(_aux);
    return true;
}

bool SetupPageUserDataPage::setRig1 (const QString &_aux)
{
    rig1LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setRig2 (const QString &_aux)
{
    rig2LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setRig3 (const QString &_aux)
{
    rig3LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setAntenna1 (const QString &_aux)
{
    ant1LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setAntenna2 (const QString &_aux)
{
    ant2LineEdit->setText(_aux);
    return true;
}

bool SetupPageUserDataPage::setAntenna3 (const QString &_aux)
{
    ant3LineEdit->setText(_aux);
    return true;
}

void SetupPageUserDataPage::slotOperatorsChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    //QString _operators = operatorsLineEdit->text();


    if (operatorsLineEdit->text().length() < 1)
        return;
    int i = operatorsLineEdit->cursorPosition();

    QString _a = operatorsLineEdit->text();

    if ((_a.at(i-1)).isSpace())
    {
        operatorsLineEdit->setText(_a.remove(i-1, 1));
    }

    operatorsLineEdit->setText(util->getClearSQLi (_a).simplified().toUpper());
    _a = operatorsLineEdit->text();

    if (checkOperatorsLineQString(_a))
    {
        //qDebug() << Q_FUNC_INFO << "  VALID FORMAT";
        operatorsLineEdit->setPalette(*defaultPalette);
        emit operatorsSignal(operatorsLineEdit->text());
    }
    else
    {
        operatorsLineEdit->setPalette(*wrongPalette);
            //qDebug() << Q_FUNC_INFO << "  NOT VALID FORMAT";
    }

       //qDebug() << Q_FUNC_INFO << " -05";
    operatorsLineEdit->setCursorPosition(i);
       //qDebug() << Q_FUNC_INFO << " -END";
}

QString SetupPageUserDataPage::getOperators()
{
    if (checkOperatorsLineQString(operatorsLineEdit->text()))
    {
        return operatorsLineEdit->text();
    }
    else
    {
        return QString();
    }
}

bool SetupPageUserDataPage::setOperators(const QString &_aux)
{
    if (checkOperatorsLineQString(_aux))
    {
        operatorsLineEdit->setText(_aux);
        return true;
    }
    return false;
}

bool  SetupPageUserDataPage::checkOperatorsLineQString(const QString &_auxLine)
{
    QStringList _aux = _auxLine.split(',');
    bool auxBool = false;
    for (int ii = 0; ii < _aux.size(); ++ii)
    {
        Callsign callsign(_aux.at(ii));
        if (callsign.isValid())
            auxBool = true;
    }
    return auxBool;
}

void SetupPageUserDataPage::setStationFocus()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    maincallsignLineEdit->setFocus();
}

void SetupPageUserDataPage::saveSettings()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("UserData");
    settings.setValue ("Callsign", getMainCallsign());
    settings.setValue ("Operators", getOperators());
    settings.setValue ("CQz", getCQz ());
    settings.setValue ("ITUz", getITUz());
    settings.setValue ("StationLocator", getStationLocator());
    settings.setValue ("Name", getName());
    settings.setValue ("Address1", getAddress1());
    settings.setValue ("Address2", getAddress2());
    settings.setValue ("Address3", getAddress3());
    settings.setValue ("Address4", getAddress4());
    settings.setValue ("City", getCity());
    settings.setValue ("ZipCode", getZipCode());
    settings.setValue ("ProvinceState", getProvince());
    settings.setValue ("Country", getCountry());
    settings.setValue ("Rig1", getRig1());
    settings.setValue ("Rig2", getRig2());
    settings.setValue ("Rig3", getRig3());
    settings.setValue ("Antenna1", getAntenna1());
    settings.setValue ("Antenna2", getAntenna2());
    settings.setValue ("Antenna3",getAntenna3());
    settings.setValue ("Power", getPower ());
    settings.endGroup ();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageUserDataPage::loadSettings()
{
    //qDebug() << Q_FUNC_INFO << " - Start";

    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("UserData");
    setMainCallsign(settings.value ("Callsign").toString ());
    setOperators (settings.value ("Operators").toString ());
    setStationLocator (settings.value ("StationLocator").toString ());
    cqzLineEdit->setText (settings.value ("CQz").toString ());
    ituzLineEdit->setText (settings.value ("ITUz").toString ());
    nameLineEdit->setText (settings.value ("Name").toString ());
    address1LineEdit->setText(settings.value ("Address1").toString ());
    address2LineEdit->setText(settings.value ("Address2").toString ());
    address3LineEdit->setText(settings.value ("Address3").toString ());
    address4LineEdit->setText(settings.value ("Address4").toString ());
    cityLineEdit->setText (settings.value ("City").toString ());
    zipLineEdit->setText (settings.value ("ZipCode").toString ());
    provinceLineEdit->setText(settings.value ("ProvinceState").toString ());
    countryLineEdit->setText (settings.value ("Country").toString ());
    rig1LineEdit->setText (settings.value ("Rig1").toString ());
    rig2LineEdit->setText (settings.value ("Rig2").toString ());
    rig3LineEdit->setText (settings.value ("Rig3").toString ());
    ant1LineEdit->setText (settings.value ("Antenna1").toString ());
    ant2LineEdit->setText (settings.value ("Antenna2").toString ());
    ant3LineEdit->setText (settings.value ("Antenna3").toString ());
    myPowerSpinBox->setValue(settings.value ("Power").toDouble ());
    settings.endGroup ();
}
