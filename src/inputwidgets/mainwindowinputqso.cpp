/***************************************************************************
                          mainwindowinputqso.cpp  -  description
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
//
// This class implement the tab of the mainwindow that supports the QSL options
//

#include "mainwindowinputqso.h"

MainWindowInputQSO::MainWindowInputQSO(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowInputQSO::MainWindowInputQSO"   << endl;
    dataProxy = dp;
    locator = new Locator();

    nameLineEdit = new QLineEdit;
    qthLineEdit = new QLineEdit;
    locatorLineEdit = new QLineEdit;
    rstTXLineEdit = new QLineEdit;
    rstRXLineEdit = new QLineEdit;
    txFreqSpinBox = new QDoubleSpinBox;
    rxFreqSpinBox = new QDoubleSpinBox;
    splitCheckBox = new QCheckBox;
    rxPowerSpinBox = new QDoubleSpinBox;

    dataProxy = dp;

    createUI();
    setDefaultData();
    clear();
       //qDebug() << "MainWindowInputQSO::MainWindowInputQSO - END"   << endl;

}

MainWindowInputQSO::~MainWindowInputQSO(){}

void MainWindowInputQSO::setModifying(const bool _m)
{
    modify = _m;
}

void MainWindowInputQSO::createUI()
{
    rstTXLineEdit->setToolTip(tr("TX RST."));
    rstRXLineEdit->setToolTip(tr("RX RST."));
    txFreqSpinBox->setToolTip(tr("TX Frequency in MHz."));
    rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));
    rxPowerSpinBox->setToolTip(tr("Power used by the contacted station."));
    nameLineEdit->setToolTip(tr("Name of the contacted operator."));
    qthLineEdit->setToolTip(tr("QTH of the contacted station."));
    locatorLineEdit->setToolTip(tr("Locator of the contacted station."));

    rxPowerSpinBox->setDecimals(2);
    rxPowerSpinBox->setMaximum(9999);
    rxPowerSpinBox->setSuffix(" " + tr("Watts"));

    txFreqSpinBox->setDecimals(3);
    txFreqSpinBox->setMaximum(99999);
    txFreqSpinBox->setSuffix(" " + tr("MHz"));

    rxFreqSpinBox->setDecimals(3);
    rxFreqSpinBox->setMaximum(99999);
    rxFreqSpinBox->setSuffix(" " + tr("MHz"));

    splitCheckBox->setText(tr("Split", "Translator: Split is a common hamradio term. Do not translate unless you are sure."));
    splitCheckBox->setChecked(false);

    QLabel *nameLabel = new QLabel(this);
    nameLabel->setText(tr("Name"));
    nameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *qthLabel = new QLabel(this);
    qthLabel->setText(tr("QTH"));
    qthLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *locLabel = new QLabel(this);
    locLabel->setText(tr("DX Locator"));
    locLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rxPowerSpinBoxLabelN = new QLabel(this);
    rxPowerSpinBoxLabelN->setText(tr("Power(rx)"));
    rxPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstTxLabelN = new QLabel(this);
    rstTxLabelN->setText(tr("RST(tx)"));
    rstTxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstRxLabelN = new QLabel(this);
    rstRxLabelN->setText(tr("RST(rx)"));
    rstRxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QGridLayout *RSTLayout = new QGridLayout;
    RSTLayout->addWidget(rstTxLabelN, 0, 0);
    RSTLayout->addWidget(rstTXLineEdit, 1, 0);
    RSTLayout->addWidget(rstRxLabelN, 0, 1);
    RSTLayout->addWidget(rstRXLineEdit, 1, 1);

    QVBoxLayout *qthLayout = new QVBoxLayout;
    qthLayout->addWidget(qthLabel);
    qthLayout->addWidget(qthLineEdit);

    QVBoxLayout *rstQTHLayout = new QVBoxLayout;
    rstQTHLayout->addLayout(RSTLayout);
    rstQTHLayout->addLayout(qthLayout);

    QLabel *txfreqLabelN = new QLabel(this);
    txfreqLabelN->setText(tr("Freq TX"));
    txfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    txfreqLabelN->setAlignment(Qt::AlignLeft);

    QLabel *rxfreqLabelN = new QLabel(this);
    rxfreqLabelN->setText(tr("Freq RX"));
    rxfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    rxfreqLabelN->setAlignment(Qt::AlignRight);

    QHBoxLayout *freqTitleLayout = new QHBoxLayout;
    freqTitleLayout->addWidget(txfreqLabelN);
    freqTitleLayout->addWidget(splitCheckBox);
    freqTitleLayout->addWidget(rxfreqLabelN);

    QHBoxLayout *freqDataLayout = new QHBoxLayout;
    freqDataLayout->addWidget(txFreqSpinBox);
    freqDataLayout->addWidget(rxFreqSpinBox);

    QVBoxLayout *freqLayout = new QVBoxLayout;
    freqLayout->addLayout(freqTitleLayout);
    freqLayout->addLayout(freqDataLayout);

    QVBoxLayout *locVLayout = new QVBoxLayout;
    locVLayout->addWidget(locLabel);
    locVLayout->addWidget(locatorLineEdit);

    QVBoxLayout *freqLocLayout = new QVBoxLayout;
    freqLocLayout->addLayout(freqLayout);
    freqLocLayout->addLayout(locVLayout);

    QHBoxLayout *rstfreqLayout = new QHBoxLayout;
    rstfreqLayout->addLayout(rstQTHLayout);
    rstfreqLayout->addLayout(freqLocLayout);

    QVBoxLayout *rxPowerLayout = new QVBoxLayout;
    rxPowerLayout->addWidget(rxPowerSpinBoxLabelN);
    rxPowerLayout->addWidget(rxPowerSpinBox);

    QVBoxLayout *nameLayout = new QVBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameLineEdit);

    QVBoxLayout *rxPwrLayout = new QVBoxLayout;
    rxPwrLayout->addWidget(rxPowerSpinBoxLabelN);
    rxPwrLayout->addWidget(rxPowerSpinBox);

    QHBoxLayout *namePwrHLayout = new QHBoxLayout;
    namePwrHLayout->addLayout(nameLayout);
    namePwrHLayout->addLayout(rxPwrLayout);

    QVBoxLayout *namePwrLayout = new QVBoxLayout;
    namePwrLayout->addLayout(namePwrHLayout);

    QVBoxLayout *qsoInputTabWidgetMainLayout = new QVBoxLayout;
    qsoInputTabWidgetMainLayout->addLayout(rstfreqLayout);
    qsoInputTabWidgetMainLayout->addLayout(namePwrLayout);

    setLayout(qsoInputTabWidgetMainLayout);

    connect(rstTXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(rstRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
    connect(txFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqTXChanged(double)) ) ;
    connect(rxFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqRXChanged(double)) ) ;
    connect(splitCheckBox, SIGNAL(clicked()), this, SLOT(slotSplitClicked()) ) ;
}


void MainWindowInputQSO::setDefaultData()
{
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);

    rxFreqBeingAutoChanged = false;
    txFreqBeingAutoChanged = false;
    isSATPropagation = false;
    propMode = QString();
    freqTX = 0.0;
    freqRX = 0.0;
    modify = false;
}

void MainWindowInputQSO::clear()
{
    qthLineEdit->clear();
    nameLineEdit->clear();
    locatorLineEdit->clear();
    rxPowerSpinBox->setValue(0);
    modify = false;
}

void MainWindowInputQSO::cleanQRZCOM()
{
    qthLineEdit->clear();
    nameLineEdit->clear();
    locatorLineEdit->clear();
}

void MainWindowInputQSO::clearName()
{
    nameLineEdit->clear();
    if (getDarkMode ())
    {
        nameLineEdit->setPalette(palBlack);
    }
    else
    {
        nameLineEdit->setPalette(palBlack);
    }
}

void MainWindowInputQSO::clearQTH()
{
    qthLineEdit->clear();
}

void MainWindowInputQSO::clearDXLocator()
{
    locatorLineEdit->clear ();
    if (getDarkMode ())
    {
        locatorLineEdit->setPalette(palBlack);
    }
    else
    {
        locatorLineEdit->setPalette(palBlack);
    }
}

void MainWindowInputQSO::slotReturnPressed()
{
    emit returnPressed();
}

void MainWindowInputQSO::slotLocatorTextChanged()
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
    //qDebug() << Q_FUNC_INFO << ": " << locatorLineEdit->text() << endl;
    int cursorP = locatorLineEdit->cursorPosition();

    locatorLineEdit->setText((util->getClearSQLi(locatorLineEdit->text())).toUpper());

    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) || locatorLineEdit->text ().isEmpty ())
    {
        //qDebug() << Q_FUNC_INFO << ": VALID: " << locatorLineEdit->text() << endl;
        locatorLineEdit->setPalette(palBlack);
        emit dxLocatorChanged (locatorLineEdit->text());

        //dxLocator = (locatorLineEdit->text());
        //infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
        //satTabWidget->setLocator(dxLocator);
        locatorLineEdit->setToolTip(tr("DX QTH locator."));
        //qDebug() << Q_FUNC_INFO << ": " << locator->getLat(locatorLineEdit->text()) << endl;
        //qDebug() << Q_FUNC_INFO << ": LON: " << locator->getLon(locatorLineEdit->text()) << endl;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": NOT VALID: " << locatorLineEdit->text() << endl;
        locatorLineEdit->setPalette(palRed);
        locatorLineEdit->setToolTip(tr("DX QTH locator. Format should be Maidenhead like IN70AA up to 10 characters."));
        locatorLineEdit->setCursorPosition(cursorP);
        return;
    }
    locatorLineEdit->setCursorPosition(cursorP);
}

QString MainWindowInputQSO::getDXLocator()
{
    return locatorLineEdit->text();
}

void MainWindowInputQSO::setDXLocator(const QString &_loc)
{
    if (locator->isValidLocator (_loc))
    {
        locatorLineEdit->setText (_loc.toUpper ());
    }
}

QString MainWindowInputQSO::getName()
{
    return nameLineEdit->text ();
}

void MainWindowInputQSO::setName(const QString &_st)
{
    nameLineEdit->setText (_st);
}

double MainWindowInputQSO::getTXFreq()
{
    return txFreqSpinBox->value();
}

void MainWindowInputQSO::setTXFreq(const double _ft)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_ft) << endl;

    if ( (_ft >= double(0)) && (_ft <= txFreqSpinBox->maximum()))
    {
        //qDebug() << Q_FUNC_INFO << ": defining FR: " << QString::number(_ft) << endl;
        txFreqSpinBox->setValue(_ft);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": defining FR: 0"  << endl;
        txFreqSpinBox->setValue(0);
    }
    //qDebug() << Q_FUNC_INFO << endl;
}

double MainWindowInputQSO::getRXFreq()
{
    return rxFreqSpinBox->value();
}

void MainWindowInputQSO::setRXFreq(const double _ft)
{
    if ( (_ft >= double(0)) && (_ft <= rxFreqSpinBox->maximum()))
    {
        rxFreqSpinBox->setValue(_ft);
    }
    else
    {
        rxFreqSpinBox->setValue(0);
    }
}

double MainWindowInputQSO::getRXPwr()
{
    return rxPowerSpinBox->value();
}

void MainWindowInputQSO::setRXPwr(const double _pw)
{
    if (_pw>=0)
    {
        rxPowerSpinBox->setValue(_pw);
    }
}

QString MainWindowInputQSO::getQTH()
{
    return qthLineEdit->text();
}

void MainWindowInputQSO::setQTH(const QString &_st)
{
    qthLineEdit->setText(_st);
}

QString MainWindowInputQSO::getRSTTX()
{
    return rstTXLineEdit->text();
}

void MainWindowInputQSO::setRSTTX(const QString &_st)
{
    rstTXLineEdit->setText (_st);
}

QString MainWindowInputQSO::getRSTRX()
{
    return rstRXLineEdit->text();
}

void MainWindowInputQSO::setRSTRX(const QString &_st)
{
    rstRXLineEdit->setText (_st);
}


void MainWindowInputQSO::setRSTToMode(const QString &_m, const bool _reading)
{
    //qDebug() << Q_FUNC_INFO << ": " << _m ;

    if ((_m == "SSB") || (_m== "LSB") || (_m=="USB") || (_m == "FM") || (_m == "AM") )
    {
         //qDebug() << Q_FUNC_INFO << ": Detected SSB/LSB/USB"  << endl;
        rstTXLineEdit->setInputMask("#DD");
        rstRXLineEdit->setInputMask("#DD");

        //rstTXLineEdit->setText("59");
        //rstRXLineEdit->setText("59");
        rstTXLineEdit->setMaxLength(2);
        rstRXLineEdit->setMaxLength(2);
    }
    else if ((_m == "CW") || (_m == "RTTY"))
    {
        rstTXLineEdit->setInputMask("DDD");
        rstRXLineEdit->setInputMask("DDD");
        //rstTXLineEdit->setText("599");
        //rstRXLineEdit->setText("599");
        rstTXLineEdit->setMaxLength(3);
        rstRXLineEdit->setMaxLength(3);
    }
    else if (_m == "PSK31")
    {
        rstTXLineEdit->setInputMask("#DD0");
        rstRXLineEdit->setInputMask("#DD0");
        //rstTXLineEdit->setText("599");
        //rstRXLineEdit->setText("599");
        rstTXLineEdit->setMaxLength(4);
        rstRXLineEdit->setMaxLength(4);
    }
    else if ( (_m.contains("FT", Qt::CaseInsensitive)) || (_m.contains("JT", Qt::CaseInsensitive)) || (_m.contains("QRA64", Qt::CaseInsensitive)) || (_m.contains("JS", Qt::CaseInsensitive)))
    {
        rstTXLineEdit->setInputMask("#99");
        rstRXLineEdit->setInputMask("#99");
        //rstTXLineEdit->setText("0");
        //rstRXLineEdit->setText("0");
        rstTXLineEdit->setMaxLength(3);
        rstRXLineEdit->setMaxLength(3);
    }
    else
    { // By default SSB RST is configured but anything could be added
        rstTXLineEdit->setInputMask("#xxx");
        rstRXLineEdit->setInputMask("#xxx");
        //rstTXLineEdit->setText("59");
        //rstRXLineEdit->setText("59");
        rstTXLineEdit->setMaxLength(4);
        rstRXLineEdit->setMaxLength(4);
    }
    if (!_reading)
    {
         //qDebug() << Q_FUNC_INFO << ": reading the UI" << endl;
        rstTXLineEdit->setText(util->getDefaultRST(_m));
        rstRXLineEdit->setText(util->getDefaultRST(_m));
    }
}

bool MainWindowInputQSO::getDarkMode()
{
    return false;
}
void MainWindowInputQSO::setPaletteRigthName(const bool _ok)
{
    if (getDarkMode())
    {
        if (_ok)
        {
            nameLineEdit->setPalette (palBlack);
        }
        else
        {
            nameLineEdit->setPalette (palRed);
        }
    }
    else
    {
        if (_ok)
        {
            nameLineEdit->setPalette (palBlack);
        }
        else
        {
            nameLineEdit->setPalette (palRed);
        }
    }
}

void MainWindowInputQSO::setPaletteRigthQTH(const bool _ok)
{
    if (getDarkMode())
    {
        if (_ok)
        {
            qthLineEdit->setPalette (palBlack);
        }
        else
        {
            qthLineEdit->setPalette (palRed);
        }
    }
    else
    {
        if (_ok)
        {
            qthLineEdit->setPalette (palBlack);
        }
        else
        {
            qthLineEdit->setPalette (palRed);
        }
    }
}

void MainWindowInputQSO::setPaletteRigthDXLocator(const bool _ok)
{
    if (getDarkMode())
    {
        if (_ok)
        {
            locatorLineEdit->setPalette (palBlack);
        }
        else
        {
            locatorLineEdit->setPalette (palRed);
        }
    }
    else
    {
        if (_ok)
        {
            locatorLineEdit->setPalette (palBlack);
        }
        else
        {
            locatorLineEdit->setPalette (palRed);
        }
    }
}

void MainWindowInputQSO::setPropModeFromSat(const QString &_p)
{
    //qDebug() << Q_FUNC_INFO << ": " << _p << endl;
    propMode = _p;
    if (propMode == "SAT")
    {
        splitCheckBox->setChecked (true);
    }
    else
    {
        splitCheckBox->setChecked (false);
    }
}

void MainWindowInputQSO::slotFreqTXChanged (double _f)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_f) << endl;
    if (util->isSameFreq (_f, freqTX))
    {
        //qDebug() << Q_FUNC_INFO << ": Same Freq return"  << endl;
        return;
    }
    freqTX = _f;
    int bandId = dataProxy->getBandIdFromFreq(_f);
    if (bandId > 1)
    { // If the freq belongs to one ham band
        txFreqSpinBox->setToolTip(tr("TX Frequency in MHz."));
        txFreqSpinBox->setPalette(palBlack);
        //qDebug() << Q_FUNC_INFO << ": emitting: " << QString::number(_f) << endl;
        emit txFreqChanged (_f);
    }
    else
    {
        txFreqSpinBox->setToolTip(tr("TX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        txFreqSpinBox->setPalette(palRed);
         //qDebug() << Q_FUNC_INFO << ":RED - Not in band "  << endl;
    }
    if ((!splitCheckBox->isChecked()) || modify )
    {
        rxFreqSpinBox->setValue (_f);
        //qDebug() << Q_FUNC_INFO << ": copying to RX Freq "  << endl;
    }
    //qDebug() << Q_FUNC_INFO << " - END"  << endl;
    setSplitCheckBox();
}

void MainWindowInputQSO::slotSplitClicked()
{
    if ((!splitCheckBox->isChecked())  )
    {
        rxFreqSpinBox->setValue(txFreqSpinBox->value());
    }
}

void MainWindowInputQSO::slotFreqRXChanged(double _f)
{
    if (util->isSameFreq (_f, freqRX))
    {
        return;
    }
    freqRX = _f;
    int bandId = dataProxy->getBandIdFromFreq(_f);
    if (bandId > 1)
    { // If the freq belongs to one ham band
        rxFreqSpinBox->setPalette(palBlack);
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));
        emit rxFreqChanged(rxFreqSpinBox->value());
    }
    else
    {
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        rxFreqSpinBox->setPalette(palRed);
         //qDebug() << "MainWindow::slotFreqRXChanged Freq is not in ANY ham band" << endl;
    }
    if ((!splitCheckBox->isChecked()) && !modify)
    {
        txFreqSpinBox->setValue (_f);
    }
    /*
    int bandId = dataProxy->getBandIdFromFreq(_f);
    if (bandId > 1)
    { // If the freq belongs to one ham band
        rxFreqSpinBox->setPalette(palBlack);
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));
        emit rxFreqChanged(rxFreqSpinBox->value());
    }
    else
    {
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        rxFreqSpinBox->setPalette(palRed);
                //qDebug() << "MainWindow::slotFreqRXChanged Freq is not in ANY ham band" << endl;
    }
    if (!rxFreqBeingAutoChanged)
    {
                //qDebug() << "MainWindow::slotFreqTXChanged: Updating SAT Downlink" << endl;
        emit rxFreqChangedForSat(rxFreqSpinBox->value());
    }
    */
    //qDebug() << "MainWindow::slotFreqRXChanged: END" << endl;
    setSplitCheckBox();
}

void MainWindowInputQSO::setSplitCheckBox()
{
   if (util->isSameFreq(txFreqSpinBox->value(), rxFreqSpinBox->value()))
    {
        splitCheckBox->setChecked(false);
    }
    else
    {
        splitCheckBox->setChecked(true);
    }
}

