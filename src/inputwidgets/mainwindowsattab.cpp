
/***************************************************************************
                          mainwindowsattab.cpp  -  description
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
#include "mainwindowsattab.h"
/*
This class implements the Satellite TAB of the MainWindow
*/
MainWindowSatTab::MainWindowSatTab(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowSatTab::MainWindowSatTab"   << endl;
    satNameComboBox = new QComboBox;
    satNameLineEdit = new QLineEdit;
    satModeLineEdit = new QLineEdit;
    satDXLocatorLineEdit = new QLineEdit;
    satOtherLabel = new QLabel;
    satBandTXComboBox = new QComboBox;
    satBandRXComboBox = new QComboBox;
    txFreqSpinBox = new QDoubleSpinBox;
    rxFreqSpinBox = new QDoubleSpinBox;

    keepThisDataForNextQSOQcheckbox = new QCheckBox;

    dataProxy = dp;
    locator = new Locator;
    util = new Utilities;

    createUI();
    populateSatComboBox();
    //txBandBeingChanged = false;
    //rxBandBeingChanged = false;

    modifying = false;
    //txBandsBeingChanged = false;
    updatingBands = false;
    satNameLineEdit->setEnabled(false);
    satOtherLabel->setEnabled(false);
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);

    setDefaultBands(); //TODO: Check how the bands are included not to create an inconsistence with the selected (in the setup) bands
       //qDebug() << "MainWindowSatTab::MainWindowSatTab - END"   << endl;
}

MainWindowSatTab::~MainWindowSatTab(){}

void MainWindowSatTab::createUI()
{
    connect(satNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatNameTextChanged() ) );
    connect(satModeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatModeTextChanged() ) );
    connect(satDXLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatDXLocTextChanged() ) );
    connect(satNameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()) );
    connect(satModeLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()) );
    connect(satDXLocatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()) );

    connect(satNameComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotSatNameComboBoxChanged() ) ) ;

    connect(satBandRXComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotSatBandRXComboBoxChanged()) ) ;
    connect(satBandTXComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotSatBandTXComboBoxChanged()) ) ;

    connect(txFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotSatFreqTXChanged(double)) ) ;
    connect(rxFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotSatFreqRXChanged(double)) ) ;
    connect (keepThisDataForNextQSOQcheckbox, SIGNAL(clicked()), this, SLOT(slotSatKeepThisDataClicked()) );

    QLabel *keepLabel = new QLabel();
    keepLabel->setText(tr("Keep this data"));
    keepLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    keepLabel->setToolTip(tr("Data entered in this tab will be copied into the next QSO."));

    keepThisDataForNextQSOQcheckbox->setToolTip(tr("Data entered in this tab will be copied into the next QSO."));
    QString othersat = tr("Other - Sat not in the list");
    QString aux;
    aux.clear();
    aux = QString(tr("Name of the Satellite if not in the list. Select: \"%1\" to enable this box. (format like AO-51).").arg(othersat));
    satNameLineEdit->setToolTip(aux);
    //satNameLineEdit->setToolTip(tr("Name of the Satellite if not in the list. Select Other Sat (format like AO-51)"));
    satModeLineEdit->setToolTip(tr("Satellite mode used."));
    satNameComboBox->setToolTip(tr("Select the satellite you are using."));
    satBandTXComboBox->setToolTip(tr("UpLink band."));
    satBandRXComboBox->setToolTip(tr("DownLink band."));
    satDXLocatorLineEdit->setToolTip(tr("Locator of the DX station. This box is synchronized with the Locator box in the QSO tab."));

    QLabel *upLinkLabel = new QLabel();
    upLinkLabel->setText(tr("UpLink"));
    upLinkLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *downLinkLabel = new QLabel();
    downLinkLabel->setText(tr("DownLink"));
    downLinkLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *satNameLabel = new QLabel();
    satNameLabel->setText(tr("Satellite"));
    satNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *satModeLabel = new QLabel();
    satModeLabel->setText(tr("Mode"));
    satModeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *satDXLocLabel = new QLabel();
    satDXLocLabel->setText(tr("DX Locator"));
    satDXLocLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);


    //QLabel *satOtherLabel = new QLabel();
    satOtherLabel->setText(tr("Other"));
    satOtherLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    txFreqSpinBox->setDecimals(3);
    txFreqSpinBox->setMaximum(99999);
    txFreqSpinBox->setSuffix(" " + tr("MHz"));

    rxFreqSpinBox->setDecimals(3);
    rxFreqSpinBox->setMaximum(99999);
    rxFreqSpinBox->setSuffix(" " + tr("MHz"));


    QHBoxLayout *keepLayout = new QHBoxLayout;
    keepLayout->addWidget(keepLabel);
    keepLayout->addWidget(keepThisDataForNextQSOQcheckbox);

    QHBoxLayout *lastlineLayout = new QHBoxLayout;
    //lastlineLayout->addWidget(satModeLabel);
    lastlineLayout->addWidget(satModeLineEdit);
    lastlineLayout->addWidget(satDXLocLabel);
    lastlineLayout->addWidget(satDXLocatorLineEdit);

    QGridLayout *tabLayout = new QGridLayout;

    tabLayout->addWidget(satNameLabel,0,0);
    tabLayout->addWidget(upLinkLabel,1,0);
    tabLayout->addWidget(downLinkLabel,2,0);
    tabLayout->addWidget(satModeLabel,3,0);
    tabLayout->addLayout(lastlineLayout,3,1);

    tabLayout->addWidget(satNameComboBox,0,1);
    tabLayout->addWidget(satBandTXComboBox,1,1);
    tabLayout->addWidget(satBandRXComboBox,2,1);
    //tabLayout->addWidget(satModeLineEdit,3,1);
    //tabLayout->addWidget(satModeLineEdit,3,1,1,-1);

    tabLayout->addWidget(satNameLineEdit,0,2);
    tabLayout->addWidget(txFreqSpinBox,1,2);
    tabLayout->addWidget(rxFreqSpinBox,2,2);

    tabLayout->addLayout(keepLayout,3,2);
    tabLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(tabLayout);

}

void MainWindowSatTab::slotSatNameComboBoxChanged()
{
    //qDebug() << "MainWindowSatTab::slotSatNameComboBoxChanged: " << satNameComboBox->currentText() << endl;
    if (modifying || (satNameComboBox->currentText().length()<4))
    {
        return;
    }

    int i = satNameComboBox->currentIndex();
   //qDebug() << "MainWindowSatTab::slotSatNameComboBoxChanged: " << QString::number(i) << endl;
    //QString _pm = (((satNameComboBox->currentText()).split(' ')).at(0)).simplified();

    satNameLineEdit->clear();

    if (i == 0)
    {
        emit setPropModeSat("Not", false);
        satNameLineEdit->setEnabled(false);
        satOtherLabel->setEnabled(false);
        satModeLineEdit->clear();

    }
    else if(i == 1)
    {
        emit setPropModeSat("SAT", keepThisDataForNextQSOQcheckbox->isChecked());
        satNameLineEdit->setEnabled(true);
        satOtherLabel->setEnabled(true);
    }
    else
    {
        emit setPropModeSat("SAT", keepThisDataForNextQSOQcheckbox->isChecked());
        satNameLineEdit->setEnabled(false);
        satOtherLabel->setEnabled(false);
        setBandsOfSat(satNameComboBox->currentText());

        //dataProxy->getSatelliteMode(satNameComboBox->currentText())
    }

    //autofillSatMode();

}

void MainWindowSatTab::slotSatNameTextChanged()
{
    //qDebug() << "MainWindowSatTab::slotSatNameTextChanged: " << satNameLineEdit->text() << endl;
    int cursor = satNameLineEdit->cursorPosition ();


    satNameLineEdit->setText((util->getClearSQLi (satNameLineEdit->text())).toUpper());

    if (modifying )
    {
        satNameLineEdit->setCursorPosition (cursor);
        return;
    }

    if ((satNameLineEdit->text()).length()>0)
    {
        emit setPropModeSat("SAT", keepThisDataForNextQSOQcheckbox->isChecked());
    }
    else if ((satModeLineEdit->text()).length()<1)
    {
        emit setPropModeSat("Not", false);
    }
    satNameLineEdit->setCursorPosition (cursor);


}

void MainWindowSatTab::slotSatModeTextChanged()
{
    //qDebug() << "MainWindowSatTab::slotSatModeTextChanged: " << satModeLineEdit->text() << endl;
/*
    satModeLineEdit->setText((satModeLineEdit->text()).toUpper());

    if ((satModeLineEdit->text()).length()>0)
    {
        emit setPropModeSat("SAT");
    }
    else if ((satNameLineEdit->text()).length()<1)
    {
        emit setPropModeSat("Not");
    }
*/

}

void MainWindowSatTab::setLocator(const QString &_t)
{
    //qDebug() << "MainWindowSatTab::setLocator: " << _t << endl;
    satDXLocatorLineEdit->setText(_t.toUpper());
    //qDebug() << "MainWindowSatTab::setLocator - END: "  << endl;
}

void MainWindowSatTab::slotSatDXLocTextChanged()
{
    //qDebug() << "MainWindowSatTab::slotSatDXLocTextChanged: " << satDXLocatorLineEdit->text() << endl;
    int cursorP = satDXLocatorLineEdit->cursorPosition();

    satDXLocatorLineEdit->setText((util->getClearSQLi(satDXLocatorLineEdit->text())).toUpper());

    if ( locator->isValidLocator(satDXLocatorLineEdit->text()) )
    {
        if (getDarkMode())
        {
            satDXLocatorLineEdit->setPalette(palWhite);
        }
        else
        {
            satDXLocatorLineEdit->setPalette(palBlack);
        }

        satDXLocatorLineEdit->setToolTip(tr("Locator of the DX station. This box is synchronized with the Locator box in the QSO tab."));

        //if (!modifying)
        //{
        //    emit dxLocatorChanged((satDXLocatorLineEdit->text()).toUpper());
        //}
    }
    else
    {
        satDXLocatorLineEdit->setPalette(palRed);
        satDXLocatorLineEdit->setToolTip(tr("Locator of the DX station. Format should be Maidenhead like IN70AA up to 10 characters."));
    }
    satDXLocatorLineEdit->setCursorPosition(cursorP);
    emit dxLocatorChanged(satDXLocatorLineEdit->text());

    //qDebug() << "MainWindowSatTab::slotSatDXLocTextChanged - END"  << endl;
}

QString MainWindowSatTab::getSatName()
{
    // Sat name must follow the format CC-NN to make it compatible with LOTW
    // C = Character
    // N = Number
/*
    QString satName;
    satName = satNameLineEdit->text();
    //TODO: Check that the format is OK
    return satName;
 */
    //qDebug() << "MainWindowSatTab::getSatName:" << satNameComboBox->currentText() << endl;
    QString _pm = QString();
    QString satName = QString();

    _pm = (((satNameComboBox->currentText()).split(' ')).at(0)).simplified();

       //qDebug() << "MainWindowSatTab::satNameComboBox: " << _pm << endl;
    if (satNameComboBox->currentIndex() == 0)
    {
        return QString();
    }
    else if(satNameComboBox->currentIndex() == 1)
    {
        satName = satNameLineEdit->text();
        if (satName.length()>0)
        {
            return satName;
        }
        else
        {
            return QString();
        }
    }
    else
    {
        return _pm;
    }

}

void MainWindowSatTab::setNoSat()
{
   satNameComboBox->setCurrentIndex(0);
   keepThisDataForNextQSOQcheckbox->setChecked(false);
}

void MainWindowSatTab::setSatName(const QString &_t)
{
     //TODO: Check that the format is OK
    //qDebug() << "MainWindowSatTab::setSatName: " << _t << endl;
    //satNameLineEdit->setText(_t);
    if (getSatIndex(_t) > 0)
    {
        setSatelliteCombo(_t);
    }
    else
    {
        satNameComboBox->setCurrentIndex(1);
        satNameLineEdit->setText(_t);
    }
}

QString MainWindowSatTab::getSatMode()
{
    //qDebug() << "MainWindowSatTab::getSatMode: " << satModeLineEdit->text() << endl;
    return satModeLineEdit->text();
}

void MainWindowSatTab::setSatMode(const QString &_t)
{
    //qDebug() << "MainWindowSatTab::setSatMode: " << _t << endl;
    if (_t == "-CLEAR-")
    {
        satModeLineEdit->clear();
    }
    else
    {
        satModeLineEdit->setText(_t);
    }
}

bool MainWindowSatTab::getRepeatThis()
{
    //qDebug() << "MainWindowSatTab::getRepeatThis: "  << endl;
    return keepThisDataForNextQSOQcheckbox->isChecked();
}

void MainWindowSatTab::setRepeatThis(const bool _t)
{
    //qDebug() << "MainWindowSatTab::setRepeatThis: "  << endl;
    keepThisDataForNextQSOQcheckbox->setChecked(_t);
}

void MainWindowSatTab::clear(bool _full)
{
    //qDebug() << "MainWindowSatTab::clear"  << endl;
    modifying = false;
    if ((keepThisDataForNextQSOQcheckbox->isChecked()) || (!_full))
    {
        satDXLocatorLineEdit->clear();
    }
    else
    {
        satModeLineEdit->clear();
        satNameComboBox->setCurrentIndex(0);
        satNameLineEdit->clear();
        satDXLocatorLineEdit->clear();
    }
    if (_full)
    {
        keepThisDataForNextQSOQcheckbox->setChecked (false);
    }

}

void MainWindowSatTab::refreshData()
{
    //qDebug() << "MainWindowSatTab::refreshData"  << endl;
    populateSatComboBox();
    //autofillSatMode();
}

void MainWindowSatTab::populateSatComboBox()
{
    //qDebug() << "MainWindowSatTab::populateSatComboBox: " << endl;

    QString nosat = tr("Not Sat QSO");
    QString othersat = tr("Other - Sat not in the list");
    satellitesList.clear();
    satellitesList = dataProxy->getSatellitesList();
    satellitesList.prepend(othersat);
    satellitesList.prepend("No-SAT - " + nosat);

    if (satellitesList.size()>1)
    {
        satNameComboBox->clear();
        satNameComboBox->addItems(satellitesList);
    }
    else
    {
        //TODO: Check how to do it better... now I could simply remove the if
        satNameComboBox->addItems(satellitesList);
    }
}

void MainWindowSatTab::setSatelliteCombo(const QString &_p)
{
    //qDebug() << "MainWindowsatTab::setSatelliteCombo: " << _p << endl;
    QString aux = QString();
    int indexC = getSatIndex(_p);
    //int indexC = satNameComboBox->findText(_p, Qt::MatchContains);
       //qDebug() << "MainWindowsatTab::setSatelliteCombo: N=" << QString::number(indexC) << endl;
    if (indexC>0)
    {
        satNameComboBox->setCurrentIndex(indexC);
    }
    else
    {
        satNameComboBox->setCurrentIndex(0);
        if (_p.length()>0)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            aux = tr("KLog has detected a satellite name that it does not recognise. If it should use one of the names of known satellites instead, please select it from the list. Alternatively, please contact the development team to add the new satellite name.") + "\n\n";
            msgBox.setText(aux + tr("The satellite you have in your QSO is: ") + _p + "\n\n" + tr("Please be aware that the satellite name will not be saved if it is not in the list, so that information may be lost!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            switch (ret)
            {
                case QMessageBox::Ok:
                break;
                default:
                // should never be reached
                break;
            }
        }
    }

}

void MainWindowSatTab::setOtherSatName(const QString &_t)
{
    //qDebug() << "MainWindowsatTab::setOtherSatName: " << _t << endl;
    satNameLineEdit->setText(_t);
}

QString MainWindowSatTab::getOtherSatName()
{
    //qDebug() << "MainWindowSatTab::getOtherSatName"  << endl;
    return QString();
}

int MainWindowSatTab::getSatIndex(const QString &_p)
{
    //qDebug() << "MainWindowSatTab::getSatIndex: " << QString::number(satNameComboBox->findText(_p, Qt::MatchContains))  << endl;
    return satNameComboBox->findText(_p, Qt::MatchContains);
}

void MainWindowSatTab::addBands(QStringList _bands)
{
    //qDebug() << "MainWindowSatTab::addBands: " << QString::number(_bands.length()) << endl;
    updatingBands = true;
    QString _txBand = satBandTXComboBox->currentText();
    QString _rxBand = satBandRXComboBox->currentText();
    //txBandsBeingChanged = true;
    satBandRXComboBox->clear();
    satBandTXComboBox->clear();
    satBandTXComboBox->addItems(_bands);
    satBandRXComboBox->addItems(_bands);
    satBandTXComboBox->setCurrentIndex(satBandTXComboBox->findText(_txBand, Qt::MatchCaseSensitive));
    satBandRXComboBox->setCurrentIndex(satBandRXComboBox->findText(_rxBand, Qt::MatchCaseSensitive));
    //txBandsBeingChanged = false;
    updatingBands = false;
    //qDebug() << "MainWindowSatTab::addBands - END"  << endl;
}

void MainWindowSatTab::setDefaultBands()
{//Defines the default bands for SAT communications: 10m/2m/70cm/23CM only if they exist in the selected bands
    //qDebug() << "MainWindowsatTab::setDefaultBands: " << endl;
    QStringList _b;
    _b.clear();

    _b << "10M" << "2M" << "70CM" << "23CM";
    satBandRXComboBox->addItems(_b);
    satBandTXComboBox->addItems(_b);

}

void MainWindowSatTab::slotSatBandRXComboBoxChanged()
{
    //qDebug() << "MainWindowsatTab::slotSatBandRXComboBoxChanged" << endl;
    if (updatingBands || modifying)
    {
        return;
    }
    bool freqInBand = dataProxy->isThisFreqInBand(satBandRXComboBox->currentText(), QString::number(rxFreqSpinBox->value()));
    if(!freqInBand)
    { // If the freq does not belong to the current band, we need to update the band
        rxFreqSpinBox->setValue(dataProxy->getLowLimitBandFromBandName(satBandRXComboBox->currentText()));
    }
    //qDebug() << "MainWindowsatTab::slotSatBandRXComboBoxChanged-END" << endl;
    autofillSatMode();
}

void MainWindowSatTab::slotSatFreqRXChanged(const double _f)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(rxFreqSpinBox->value()) << endl;

    if (util->isSameFreq (freqRX, _f))
    {
        return;
    }
    freqRX = _f;

    if (getDarkMode())
    {
        rxFreqSpinBox->setPalette(palWhite);
    }
    else
    {
        rxFreqSpinBox->setPalette(palBlack);
    }

    if (modifying)
    {
        return;
    }

    int bandId = dataProxy->getBandIdFromFreq(rxFreqSpinBox->value());
    if (bandId<1)
    { //This prevent that a non-hamradio frequency is used on TX
        //qDebug() << Q_FUNC_INFO << ":  Not in band, exiting... " << endl;
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        rxFreqSpinBox->setPalette(palRed);
    }
    else
    {
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));
        bool freqInBand = dataProxy->isThisFreqInBand(satBandRXComboBox->currentText(), QString::number(rxFreqSpinBox->value()));
        if(!freqInBand)
        { // If the freq does not belong to the current band, we need to update the band
            satBandRXComboBox->setCurrentIndex(satBandRXComboBox->findText(dataProxy->getBandNameFromFreq(rxFreqSpinBox->value()), Qt::MatchCaseSensitive));

        }
    }

    emit satRxFreqChanged(rxFreqSpinBox->value());

    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindowSatTab::slotSatBandTXComboBoxChanged()
{
    //qDebug() << "MainWindowsatTab::slotSatBandTXComboBoxChanged: -" << satBandTXComboBox->currentText()  << endl;
    if (updatingBands || modifying || (satBandTXComboBox->currentText().length()<2))
    {
        return;
    }
    bool freqInBand = dataProxy->isThisFreqInBand(satBandTXComboBox->currentText(), QString::number(txFreqSpinBox->value()));
    if(!freqInBand)
    { // If the freq does not belong to the current band, we need to update the band
       //qDebug() << "MainWindowsatTab::slotSatBandTXComboBoxChanged changing to: Band: " << satBandTXComboBox->currentText()  << endl;
       //qDebug() << "MainWindowsatTab::slotSatBandTXComboBoxChanged changing to: " << QString::number(dataProxy->getLowLimitBandFromBandName(satBandTXComboBox->currentText()))  << endl;
        txFreqSpinBox->setValue(dataProxy->getLowLimitBandFromBandName(satBandTXComboBox->currentText()));
        //setUpLinkFreq(dataProxy->getLowLimitBandFromBandName(satBandTXComboBox->currentText()));
    }
   //qDebug() << "MainWindowsatTab::slotSatBandTXComboBoxChanged-END" << endl;
    autofillSatMode();
}

void MainWindowSatTab::slotSatFreqTXChanged(const double _f)
{
   //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: " << QString::number(txFreqSpinBox->value()) << endl;
    // user changes TX Freq
    // If band is real and band is configured, bandcombo is selected
    // If band is real and not configured, we launch the band config and select the band.
    // if band is real emit the band
    if (util->isSameFreq (freqTX, _f))
    {
        return;
    }
    freqTX = _f;

    if (getDarkMode())
    {
        txFreqSpinBox->setPalette(palWhite);
    }
    else
    {
        txFreqSpinBox->setPalette(palBlack);
    }

    if (modifying)
    {
        return;
    }

    int bandId = dataProxy->getBandIdFromFreq(txFreqSpinBox->value());
    if (bandId<1)
    { //This prevent that a non-hamradio frequency is used on TX
          //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: Not in band, exiting... " << endl;
        txFreqSpinBox->setToolTip(tr("TX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        txFreqSpinBox->setPalette(palRed);
    }
    else
    {
        txFreqSpinBox->setToolTip(tr("TX Frequency in MHz."));
        bool freqInBand = dataProxy->isThisFreqInBand(satBandTXComboBox->currentText(), QString::number(txFreqSpinBox->value()));
        if(!freqInBand)
        { // If the freq does not belong to the current band, we need to update the band
            //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: If the freq does not belong to the current band, we need to update the band" << endl;
            //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: We define UoLink to: " << dataProxy->getBandNameFromFreq(txFreqSpinBox->value()) << endl;
            satBandTXComboBox->setCurrentIndex(satBandTXComboBox->findText(dataProxy->getBandNameFromFreq(txFreqSpinBox->value()), Qt::MatchCaseSensitive));
            //setUpLinkFreq(dataProxy->getLowLimitBandFromBandName(satBandTXComboBox->currentText()));
        }
    //autofillSatMode();
    }

    //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: Emitting: " << QString::number(txFreqSpinBox->value()) << endl;
    emit satTxFreqChanged(txFreqSpinBox->value());

    //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged-END" << endl;
}

void MainWindowSatTab::setUpLink(const QString &_t)
{
    //qDebug() << "MainWindowsatTab::setUpLink: " << _t << endl;
    if (satBandTXComboBox->currentText () == _t)
    {
        return;
    }
    int index = satBandTXComboBox->findText(_t, Qt::MatchCaseSensitive);
    //qDebug() << "MainWindowsatTab::setUpLink: new index: " << QString::number(index) << endl;
    //qDebug() << "MainWindowsatTab::setUpLink: current index: " << QString::number(satBandTXComboBox->currentIndex()) << endl;
    int indexRX;
    if (index>=0)
    {
        satBandTXComboBox->setCurrentIndex(index);
         if ( dataProxy->getIdFromBandName("2M") ==  dataProxy->getIdFromBandName(_t) )
         {
             //qDebug() << satNameComboBox->currentText() << endl;
             if (satNameComboBox->findText("AO-7 - AMSAT-OSCAT 7", Qt::MatchCaseSensitive))
             {
                 indexRX = satBandRXComboBox->findText("10M", Qt::MatchCaseSensitive);
             }
             else
             {
                 indexRX = satBandRXComboBox->findText("70CM", Qt::MatchCaseSensitive);
             }

             satBandRXComboBox->setCurrentIndex(indexRX);
         }
         else if ( dataProxy->getIdFromBandName("70CM") ==  dataProxy->getIdFromBandName(_t) )
         {
             indexRX = satBandRXComboBox->findText("2M", Qt::MatchCaseSensitive);
             satBandRXComboBox->setCurrentIndex(indexRX);
         }
     }
}

void MainWindowSatTab::setUpLinkFreq(const double _t)
{
    //qDebug() << "MainWindowsatTab::setUpLinkFreq: " << QString::number(_t) << endl;
    if (util->isSameFreq (freqTX, _t))
    {
        return;
    }

    txFreqSpinBox->setValue(_t);
    setUpLink(dataProxy->getBandNameFromFreq(_t));

   //qDebug() << "MainWindowsatTab::setUpLinkFreq END" << endl;
}
double MainWindowSatTab::getRXFreq()
{
    //qDebug() << "MainWindowsatTab::getRXFreq " << endl;
    return rxFreqSpinBox->value();
}

void MainWindowSatTab::setDownLinkFreq(const double _t)
{
    //qDebug() << "MainWindowsatTab::setDownLinkFreq: " << QString::number(_t) << endl;
    if (util->isSameFreq (freqRX, _t))
    {
        return;
    }

    rxFreqSpinBox->setValue(_t);
    QString downLinkBand = dataProxy->getBandNameFromFreq(_t);

    int index = satBandRXComboBox->findText(downLinkBand, Qt::MatchCaseSensitive);
    if (index>=0)
    {
        satBandRXComboBox->setCurrentIndex(index);
    }

    //qDebug() << "MainWindowsatTab::setDownLinkFreq END" << endl;
}

void MainWindowSatTab::setBandsOfSat(const QString &_p)
{
    // Until the data is in the DB, this function tries to solve data of active sats from: http://www.amsat.org/status/
    // This functio:
    //  - identifies the freqs & bands that the sat is using
    //qDebug() << "MainWindowSatTab::setBandsOfSat: " << _p << " - Short: " << _p.section(' ', 0, 0) << endl;
    //"AO-7 - AMSAT-OSCAT 7"
    //2M/10M << 2M/70CM
    if (modifying)
    {
        return;
    }

    double upLink = (dataProxy->getSatelliteUplink(_p.section(' ', 0, 0))).toDouble();
    double downLink = (dataProxy->getSatelliteDownlink(_p.section(' ', 0, 0))).toDouble();

    //qDebug() << "MainWindowSatTab::setBandsOfSat upLink: " << QString::number(upLink)<< endl;
    //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: " << QString::number(downLink)<< endl;

    if (upLink>0)
    {
        //qDebug() << "MainWindowSatTab::setBandsOfSat upLink: emitting: " << QString::number(upLink)<< endl;
        emit satTXFreqNeeded(upLink);
          //qDebug() << "MainWindowSatTab::setBandsOfSat upLink: emitted: " << QString::number(upLink)<< endl;
    }
    else
    {
        //qDebug() << "MainWindowSatTab::setBandsOfSat upLink: setting to ZERO (should be = RX) " <<  endl;
        txFreqSpinBox->setValue(0);
        //satBandTXComboBox->setCurrentIndex(0);
    }

    if (downLink>0)
    {
          //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: emitting: " << QString::number(downLink)<< endl;
        emit satRXFreqNeeded(downLink);
          //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: emitted: " << QString::number(downLink)<< endl;

    }
    else
    {
          //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: setting to ZERO" <<  endl;
        rxFreqSpinBox->setValue(0);
        //satBandRXComboBox->setCurrentIndex(0);
    }
      //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: emiting: " << QString::number(downLink)<< endl;
    //emit satRxFreqChanged(downLink);


      //qDebug() << "MainWindowSatTab::setBandsOfSat: " << _p << " - END "  << endl;
}

void MainWindowSatTab::addNewBand(const QString &_p)
{
    //qDebug() << "MainWindowSatTab::addNewBand: " << _p << endl;
    if (dataProxy->getIdFromBandName(_p)<0)
    {
        //qDebug() << "MainWindowSatTab::addNewBand: Id: " <<  QString::number(dataProxy->getIdFromBandName(_p)) << endl;
        return;
    }
    QStringList bands;
    bands.clear();

       //qDebug() << "MainWindowSatTab::addNewBand: RX Id: " <<  QString::number(satBandRXComboBox->count()) << endl;
       //qDebug() << "MainWindowSatTab::addNewBand: TX Id: " <<  QString::number(satBandTXComboBox->count()) << endl;

    for (int i = 0; i < satBandTXComboBox->count(); i++)
    {
        bands << satBandTXComboBox->itemText(i);
        for (int ii = 0; ii < satBandRXComboBox->count(); ii++)
        {
            bands << satBandRXComboBox->itemText(ii);
        }
    }


    bands << _p;
    //bands.removeDuplicates();

    emit newBandsToBeAdded(bands);
    //addBands(bands);
    //qDebug() << "MainWindowSatTab::addNewBand: 2 RX Id: " <<  QString::number(satBandRXComboBox->count()) << endl;
    //qDebug() << "MainWindowSatTab::addNewBand: 2 TX Id: " <<  QString::number(satBandTXComboBox->count()) << endl;

}

void MainWindowSatTab::setModifying (const bool _m)
{
    //qDebug() << "MainWindowsatTab::setModifying" << endl;
    modifying = _m;
}

void MainWindowSatTab::slotReturnPressed()
{
    //qDebug() << "MainWindowsatTab::slotReturnPressed" << endl;
    emit returnPressed();
}

void MainWindowSatTab::slotSatKeepThisDataClicked()
{
    //qDebug() << Q_FUNC_INFO << endl;
    if (keepThisDataForNextQSOQcheckbox->isChecked())
    {
        //qDebug() << Q_FUNC_INFO << ": True" << endl;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": False" << endl;
    }
    slotSatNameComboBoxChanged();
}

void MainWindowSatTab::autofillSatMode()
{
    QString downLinkBand = bandToLetter(dataProxy->getBandNameFromFreq(rxFreqSpinBox->value()));
    QString upLinkBand = bandToLetter(dataProxy->getBandNameFromFreq(txFreqSpinBox->value()));
    satModeLineEdit->setText(upLinkBand + "/" + downLinkBand);
}

QString MainWindowSatTab::bandToLetter(const QString _band)
{
   QString letter = "";

   if (_band == "15M")
   {
       letter = "H";
   }
   else if (_band == "10M")
   {
       letter =  "A";
   }
   else if (_band == "2M")
   {
       letter =  "V";
   }
   else if (_band == "70CM")
   {
       letter =  "U";
   }
   else if (_band == "23CM")
   {
       letter =  "L";
   }
   else if (_band == "13CM")
   {
       letter =  "S";
   }
   else if (_band == "5CM")
   {
       letter =  "C";
   }
   else if (_band == "3CM")
   {
       letter =  "X";
   }
   else if (_band == "1.25CM")
   {
       letter =  "K";
   }
   else
   {
       letter = "-";
   }
   return letter;
}

void MainWindowSatTab::setKeep(const bool _b)
{
    keepThisDataForNextQSOQcheckbox->setChecked (_b);
}

bool MainWindowSatTab::getKeep()
{
    return keepThisDataForNextQSOQcheckbox->isChecked ();
}

bool MainWindowSatTab::getDarkMode()
{
    if (satNameLineEdit->palette().color (QPalette::Base) == "#646464")
    {
        return true;
    }
    else
    {
        return false;
    }
}
