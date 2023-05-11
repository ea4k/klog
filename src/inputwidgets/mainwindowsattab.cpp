
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
       //qDebug() << "MainWindowSatTab::MainWindowSatTab"  ;
    satNameComboBox = new QComboBox;
    satNameLineEdit = new QLineEdit;
    satModeLineEdit = new QLineEdit;

    satOtherLabel = new QLabel;
    satBandTXComboBox = new QComboBox;
    satBandRXComboBox = new QComboBox;
    //txFreqSpinBox = new QDoubleSpinBox;
    //rxFreqSpinBox = new QDoubleSpinBox;

    keepThisDataForNextQSOQcheckbox = new QCheckBox;

    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);

    createUI();
    populateSatComboBox();
    //txBandBeingChanged = false;
    //rxBandBeingChanged = false;

    modifying = false;
    //txBandsBeingChanged = false;
    updatingBands = false;
    updatingSat = false;
    satNameLineEdit->setEnabled(false);
    satOtherLabel->setEnabled(false);
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);

    setDefaultBands(); //TODO: Check how the bands are included not to create an inconsistence with the selected (in the setup) bands
       //qDebug() << "MainWindowSatTab::MainWindowSatTab - END"  ;
}

MainWindowSatTab::~MainWindowSatTab(){
    delete(util);
    delete(dataProxy);
}

void MainWindowSatTab::createUI()
{
    connect(satNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatNameTextChanged() ) );
    connect(satModeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatModeTextChanged() ) );
    //connect(satDXLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSatDXLocTextChanged() ) );
    connect(satNameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()) );
    connect(satModeLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()) );
    //connect(satDXLocatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()) );

    connect(satNameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSatNameComboBoxChanged() ) ) ;

    connect(satBandRXComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSatBandRXComboBoxChanged()) ) ;
    connect(satBandTXComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSatBandTXComboBoxChanged()) ) ;

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

    //QLabel *satOtherLabel = new QLabel();
    satOtherLabel->setText(tr("Other"));
    satOtherLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    //txFreqSpinBox->setDecimals(3);
    //txFreqSpinBox->setMaximum(99999);
    //txFreqSpinBox->setSuffix(" " + tr("MHz"));

    //rxFreqSpinBox->setDecimals(3);
    //rxFreqSpinBox->setMaximum(99999);
    //rxFreqSpinBox->setSuffix(" " + tr("MHz"));

    QHBoxLayout *keepLayout = new QHBoxLayout;
    keepLayout->addWidget(keepLabel);
    keepLayout->addWidget(keepThisDataForNextQSOQcheckbox);

    QHBoxLayout *lastlineLayout = new QHBoxLayout;
    //lastlineLayout->addWidget(satModeLabel);
    lastlineLayout->addWidget(satModeLineEdit);

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
    //tabLayout->addWidget(txFreqSpinBox,1,2);
    //tabLayout->addWidget(rxFreqSpinBox,2,2);

    tabLayout->addLayout(keepLayout,3,2);
    tabLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(tabLayout);
}

void MainWindowSatTab::slotSatNameComboBoxChanged()
{
   //qDebug() << "MainWindowSatTab::slotSatNameComboBoxChanged: " << satNameComboBox->currentText();
    if (modifying || (satNameComboBox->currentText().length()<4))
    {
        return;
    }
    updatingSat = true;

    int i = satNameComboBox->currentIndex();
   //qDebug() << "MainWindowSatTab::slotSatNameComboBoxChanged: " << QString::number(i);
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
        autofillSatMode();
    }
    else
    {
        emit setPropModeSat("SAT", keepThisDataForNextQSOQcheckbox->isChecked());
        satNameLineEdit->setEnabled(false);
        satOtherLabel->setEnabled(false);
        setBandsOfSat(satNameComboBox->currentText());
       //dataProxy->getSatelliteMode(satNameComboBox->currentText())
        autofillSatMode();
    }
    updatingSat = false;
}

void MainWindowSatTab::slotSatNameTextChanged()
{
    //qDebug() << "MainWindowSatTab::slotSatNameTextChanged: " << satNameLineEdit->text();
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
    //qDebug() << "MainWindowSatTab::slotSatModeTextChanged: " << satModeLineEdit->text();
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
    //qDebug() << "MainWindowSatTab::getSatName:" << satNameComboBox->currentText();
    QString _pm = QString();
    QString satName = QString();

    _pm = (((satNameComboBox->currentText()).split(' ')).at(0)).simplified();

       //qDebug() << "MainWindowSatTab::satNameComboBox: " << _pm;
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
   setSatMode("-CLEAR-");
   keepThisDataForNextQSOQcheckbox->setChecked(false);
}

void MainWindowSatTab::setSatName(const QString &_t)
{
     //TODO: Check that the format is OK
    //qDebug() << "MainWindowSatTab::setSatName: " << _t;
    //satNameLineEdit->setText(_t);
    if (_t =="")
    {
        return;
    }
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
    //qDebug() << Q_FUNC_INFO << ": " << satModeLineEdit->text();
    return satModeLineEdit->text();
}

void MainWindowSatTab::setSatMode(const QString &_t)
{
    //qDebug() << Q_FUNC_INFO << ": " << _t ;
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
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    return keepThisDataForNextQSOQcheckbox->isChecked();
}

void MainWindowSatTab::setRepeatThis(const bool _t)
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    keepThisDataForNextQSOQcheckbox->setChecked(_t);
}

void MainWindowSatTab::clear(bool _full)
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    modifying = false;
    if ((keepThisDataForNextQSOQcheckbox->isChecked()) || (!_full))
    {
    }
    else
    {
        satModeLineEdit->clear();
        satNameComboBox->setCurrentIndex(0);
        satNameLineEdit->clear();
    }
    if (_full)
    {
        keepThisDataForNextQSOQcheckbox->setChecked (false);
    }
}

void MainWindowSatTab::refreshData()
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    populateSatComboBox();
    //autofillSatMode();
}

void MainWindowSatTab::populateSatComboBox()
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;

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
    //qDebug() << Q_FUNC_INFO << " - Start: " << _p ;
    QString aux = QString();
    int indexC = getSatIndex(_p);
    //int indexC = satNameComboBox->findText(_p, Qt::MatchContains);
       //qDebug() << "MainWindowsatTab::setSatelliteCombo: N=" << QString::number(indexC);
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
    //qDebug() << Q_FUNC_INFO << " - Start: " << _t ;
    satNameLineEdit->setText(_t);
}

QString MainWindowSatTab::getOtherSatName()
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    return QString();
}

int MainWindowSatTab::getSatIndex(const QString &_p)
{
    //qDebug() << "MainWindowSatTab::getSatIndex: " << QString::number(satNameComboBox->findText(_p, Qt::MatchContains)) ;
    return satNameComboBox->findText(_p, Qt::MatchContains);
}

void MainWindowSatTab::addBands(QStringList _bands)
{
    //qDebug() << Q_FUNC_INFO << " - Start: "  << QString::number(_bands.length());
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
    //qDebug() << "MainWindowSatTab::addBands - END" ;
}

void MainWindowSatTab::setDefaultBands()
{//Defines the default bands for SAT communications: 10m/2m/70cm/23CM only if they exist in the selected bands
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    QStringList _b;
    _b.clear();

    _b << "10M" << "2M" << "70CM" << "23CM";
    satBandRXComboBox->addItems(_b);
    satBandTXComboBox->addItems(_b);
}

void MainWindowSatTab::slotSatBandRXComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    if (updatingBands || modifying)
    {
        return;
    }

    bool freqInBand = dataProxy->isThisFreqInBand(satBandRXComboBox->currentText(), QString::number(freqRX));
    if(!freqInBand)
    { // If the freq does not belong to the current band, we need to update the band
       //qDebug() << Q_FUNC_INFO << " changing to: Band: " << satBandTXComboBox->currentText() ;
        //qDebug() << Q_FUNC_INFO << " changing to: " << QString::number(dataProxy->getLowLimitBandFromBandName(satBandTXComboBox->currentText())) ;
        setNoSat ();
        updateRXFreq(dataProxy->getLowLimitBandFromBandName(satBandRXComboBox->currentText()));
    }

    //qDebug() << Q_FUNC_INFO << " - END";
    autofillSatMode();
}

void MainWindowSatTab::slotSatBandTXComboBoxChanged()
{
    //qDebug() << Q_FUNC_INFO << " - " << satBandTXComboBox->currentText();
    if (updatingBands || modifying || (satBandTXComboBox->currentText().length()<2))
    {
        return;
    }
    //qDebug() << Q_FUNC_INFO << " - Freq: " << QString::number(freqTX);
    QString tmpBand = satBandTXComboBox->currentText();
    bool freqInBand = dataProxy->isThisFreqInBand(tmpBand, QString::number(freqTX));
    if(!freqInBand)
    { // If the freq does not belong to the current band, we need to update the band
       //qDebug() << "MainWindowsatTab::slotSatBandTXComboBoxChanged changing to: Band: " << satBandTXComboBox->currentText() ;
        //qDebug() << "MainWindowsatTab::slotSatBandTXComboBoxChanged changing to: " << QString::number(dataProxy->getLowLimitBandFromBandName(satBandTXComboBox->currentText())) ;
        QString tmpFreq = dataProxy->getFreqFromBandId(dataProxy->getIdFromBandName(satBandTXComboBox->currentText()));
        //qDebug() << ": Normal" << dataProxy->getSatelliteUplink(getSatName());
        //qDebug() << ": Normal1" << dataProxy->getSatelliteUplink(getSatName(), 1);
        //qDebug() << ": Normal2" << dataProxy->getSatelliteUplink(getSatName(), 2);
        //qDebug() << ": Full" << dataProxy->getSatelliteFullUplink(getSatName());

        double upLink = tmpFreq.toDouble();
        double downLink;
        if (dataProxy->isThisFreqInBand(tmpBand,dataProxy->getSatelliteUplink(getSatName(),1)) )
        {
            upLink = (dataProxy->getSatelliteUplink(getSatName(),1)).toDouble();
            downLink = (dataProxy->getSatelliteDownlink(getSatName(),1)).toDouble();
            updateRXFreq(downLink);
        }
        else if (dataProxy->isThisFreqInBand(tmpBand,dataProxy->getSatelliteUplink(getSatName(),0)) )
        {
            upLink = (dataProxy->getSatelliteUplink(getSatName(),0)).toDouble();
            downLink = (dataProxy->getSatelliteDownlink(getSatName(),1)).toDouble();
            updateRXFreq(downLink);
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - Selected satelite does not work in the selected band. NO sat should be selected";
            setNoSat ();
        }

        updateTXFreq(upLink);
    }

    autofillSatMode();
    qDebug() << Q_FUNC_INFO << " - END" ;
}

void MainWindowSatTab::setUpLink(const QString &_t)
{
   //qDebug() << Q_FUNC_INFO << "Start: " << _t ;
    if (satBandTXComboBox->currentText () == _t)
    {
        //qDebug() << Q_FUNC_INFO << "END-1";
        return;
    }
    int index = satBandTXComboBox->findText(_t, Qt::MatchCaseSensitive);
    //qDebug() << "MainWindowsatTab::setUpLink: new index: " << QString::number(index);
    //qDebug() << "MainWindowsatTab::setUpLink: current index: " << QString::number(satBandTXComboBox->currentIndex());
    //int indexRX;
    if (index>=0)
    {
        satBandTXComboBox->setCurrentIndex(index);
    }
    /*
    if (index>=0)
    {
        satBandTXComboBox->setCurrentIndex(index);
        if ( dataProxy->getIdFromBandName("2M") ==  dataProxy->getIdFromBandName(_t) )
        {
             //qDebug() << satNameComboBox->currentText() ;
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
    */
    //qDebug() << Q_FUNC_INFO << "END";
}

void MainWindowSatTab::setUpLinkFreq(const double _t)
{
    qDebug() << Q_FUNC_INFO << ": " << QString::number(_t);
    if (!updatingSat)
    {
        setNoSat ();
    }
    updateTXFreq(_t);
    setUpLink(dataProxy->getBandNameFromFreq(_t));
    qDebug() << Q_FUNC_INFO << "END";
}

double MainWindowSatTab::getRXFreq()
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    return freqRX;
}

//double MainWindowSatTab::getTXFreq()
//{
//    return freqTX;
//}

void MainWindowSatTab::setDownLinkFreq(const double _t)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_t);
    if (util->isSameFreq (freqRX, _t))
    {
        //qDebug() << Q_FUNC_INFO << " - Is same freq";
        return;
    }

    updateRXFreq(_t);
    QString downLinkBand = dataProxy->getBandNameFromFreq(freqRX);

    int index = satBandRXComboBox->findText(downLinkBand, Qt::MatchCaseSensitive);
    //qDebug() << Q_FUNC_INFO << ": " << downLinkBand;
    if (index>=0)
    {
        satBandRXComboBox->setCurrentIndex(index);
    }
   //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindowSatTab::updateRXFreq(const double _f)
{
    //qDebug() << Q_FUNC_INFO << QString::number(_f);

    if (util->isSameFreq (freqRX, _f))
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    freqRX = _f;

    if (modifying)
    {
        //qDebug() << Q_FUNC_INFO << " - END-2";
        return;
    }


    int bandId = dataProxy->getBandIdFromFreq(freqRX);
    if (bandId>=1)
    { //This prevent that a non-hamradio frequency is used on TX
        //qDebug() << Q_FUNC_INFO << " - Freq not in ham band";
        bool freqInBand = dataProxy->isThisFreqInBand(satBandRXComboBox->currentText(), QString::number(freqRX));
        if(!freqInBand)
        { // If the freq does not belong to the current band, we need to update the band
            //qDebug() << Q_FUNC_INFO << " - Freq in current band";
            QString aux;
            aux = dataProxy->getBandNameFromFreq(freqRX);
            if (satBandRXComboBox->findText(aux, Qt::MatchCaseSensitive)<0)
            {
                addNewBand(aux);
            }
            //addNewBand(const QString &_p)
            satBandRXComboBox->setCurrentIndex(satBandRXComboBox->findText(aux, Qt::MatchCaseSensitive));
        }
    }
    emit satRxFreqChanged(freqRX);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindowSatTab::updateTXFreq(const double _f)
{
    //qDebug() << Q_FUNC_INFO << QString::number(txFreqSpinBox->value());
        // user changes TX Freq
        // If band is real and band is configured, bandcombo is selected
        // If band is real and not configured, we launch the band config and select the band.
        // if band is real emit the band
        if (util->isSameFreq (freqTX, _f))
        {
            //qDebug() << Q_FUNC_INFO << " - END-1";
            return;
        }
        freqTX = _f;

        if (modifying)
        {
            //qDebug() << Q_FUNC_INFO << " - END-2";
            return;
        }

        int bandId = dataProxy->getBandIdFromFreq(freqTX);
        if (bandId>=1)
        { //This prevent that a non-hamradio frequency is used on TX
            bool freqInBand = dataProxy->isThisFreqInBand(satBandTXComboBox->currentText(), QString::number(freqTX));
            if(!freqInBand)
            { // If the freq does not belong to the current band, we need to update the band
                //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: If the freq does not belong to the current band, we need to update the band";
                //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: We define UpLink to: " << dataProxy->getBandNameFromFreq(txFreqSpinBox->value());
                QString aux = dataProxy->getBandNameFromFreq(freqTX);
                //qDebug() << Q_FUNC_INFO << QString("FreqTx = %1 / Band = %2").arg(freqTX).arg(aux);
                if (satBandTXComboBox->findText(aux, Qt::MatchCaseSensitive)<0)
                {
                    addNewBand(aux);
                }
                satBandTXComboBox->setCurrentIndex(satBandTXComboBox->findText(aux, Qt::MatchCaseSensitive));
                //setUpLinkFreq(dataProxy->getLowLimitBandFromBandName(satBandTXComboBox->currentText()));
            }
        //autofillSatMode();
        }

        //qDebug() << "MainWindowsatTab::slotSatFreqTXChanged: Emitting: " << QString::number(txFreqSpinBox->value());
        emit satTxFreqChanged(freqTX);
        //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindowSatTab::setBandsOfSat(const QString &_p)
{
    // Until the data is in the DB, this function tries to solve data of active sats from: http://www.amsat.org/status/
    // This functio:
    //  - identifies the freqs & bands that the sat is using
    //qDebug() << Q_FUNC_INFO << ": " << _p << " - Short: " << _p.section(' ', 0, 0);
    //"AO-7 - AMSAT-OSCAT 7"
    //2M/10M << 2M/70CM
    if (modifying)
    {
        return;
    }

    double upLink = (dataProxy->getSatelliteUplink(_p.section(' ', 0, 0))).toDouble();
    double downLink = (dataProxy->getSatelliteDownlink(_p.section(' ', 0, 0))).toDouble();

    //qDebug() << Q_FUNC_INFO << ": upLink: " << QString::number(upLink)<< QT_ENDL;
    //qDebug() << Q_FUNC_INFO << ":  downLink: " << QString::number(downLink)<< QT_ENDL;

    if (upLink>0)
    {
        //qDebug() << "MainWindowSatTab::setBandsOfSat upLink: emitting: " << QString::number(upLink)<< QT_ENDL;
        emit satTXFreqNeeded(upLink);
          //qDebug() << "MainWindowSatTab::setBandsOfSat upLink: emitted: " << QString::number(upLink)<< QT_ENDL;
    }
    else
    {
        //qDebug() << "MainWindowSatTab::setBandsOfSat upLink: setting to ZERO (should be = RX) " <<  QT_ENDL;
        updateTXFreq(0.0);
    }

    if (downLink>0)
    {
          //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: emitting: " << QString::number(downLink)<< QT_ENDL;
        emit satRXFreqNeeded(downLink);
          //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: emitted: " << QString::number(downLink)<< QT_ENDL;
    }
    else
    {
          //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: setting to ZERO" <<  QT_ENDL;
        updateRXFreq(0.0);
        //satBandRXComboBox->setCurrentIndex(0);
    }
      //qDebug() << "MainWindowSatTab::setBandsOfSat downLink: emiting: " << QString::number(downLink)<< QT_ENDL;
    //emit satRxFreqChanged(downLink);
      //qDebug() << "MainWindowSatTab::setBandsOfSat: " << _p << " - END " ;
}

void MainWindowSatTab::addNewBand(const QString &_p)
{
    //qDebug() << "MainWindowSatTab::addNewBand: " << _p;
    if (dataProxy->getIdFromBandName(_p)<0)
    {
        //qDebug() << "MainWindowSatTab::addNewBand: Id: " <<  QString::number(dataProxy->getIdFromBandName(_p));
        return;
    }
    QStringList bands;
    bands.clear();
    int indexRX = satBandRXComboBox->currentIndex();
    int indexTX = satBandTXComboBox->currentIndex();

       //qDebug() << "MainWindowSatTab::addNewBand: RX Id: " <<  QString::number(satBandRXComboBox->count());
       //qDebug() << "MainWindowSatTab::addNewBand: TX Id: " <<  QString::number(satBandTXComboBox->count());

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
    satBandRXComboBox->setCurrentIndex(indexRX);
    satBandTXComboBox->setCurrentIndex(indexTX);
    //addBands(bands);
    //qDebug() << "MainWindowSatTab::addNewBand: 2 RX Id: " <<  QString::number(satBandRXComboBox->count());
    //qDebug() << "MainWindowSatTab::addNewBand: 2 TX Id: " <<  QString::number(satBandTXComboBox->count());
}

void MainWindowSatTab::setModifying (const bool _m)
{
    //qDebug() << "MainWindowsatTab::setModifying";
    modifying = _m;
}

void MainWindowSatTab::slotReturnPressed()
{
    //qDebug() << "MainWindowsatTab::slotReturnPressed";
    emit returnPressed();
}

void MainWindowSatTab::slotSatKeepThisDataClicked()
{
    //qDebug() << Q_FUNC_INFO;
    if (keepThisDataForNextQSOQcheckbox->isChecked())
    {
        //qDebug() << Q_FUNC_INFO << ": True";
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": False";
    }
    slotSatNameComboBoxChanged();
}

void MainWindowSatTab::autofillSatMode()
{
    //qDebug() << Q_FUNC_INFO ;
    QString downLinkBand = bandToLetter(dataProxy->getBandNameFromFreq(freqRX));
    QString upLinkBand = bandToLetter(dataProxy->getBandNameFromFreq(freqTX));
    satModeLineEdit->setText(upLinkBand + "/" + downLinkBand);
}

QString MainWindowSatTab::bandToLetter(const QString &_band)
{
    //qDebug() << Q_FUNC_INFO << ": " << _band;
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
   //qDebug() << Q_FUNC_INFO << ": " << _band << " -> " << letter;
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

