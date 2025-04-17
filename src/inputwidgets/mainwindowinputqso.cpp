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
   //qDebug() << Q_FUNC_INFO << " - Start";
    dataProxy = dp;
    //locator = new Locator();

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
    //installEventFilter (this);
    clear();
    //qDebug() << "MainWindowInputQSO::MainWindowInputQSO - END"  ;
}

MainWindowInputQSO::~MainWindowInputQSO()
{
   //delete(dataProxy);
}

void MainWindowInputQSO::setModifying(const bool _m)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    modify = _m;
}

void MainWindowInputQSO::createUI()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
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

    txFreqSpinBox->setDecimals(6);
    txFreqSpinBox->setMaximum(99999);
    txFreqSpinBox->setSuffix(" " + tr("MHz"));
    txFreqSpinBox->setKeyboardTracking(false);
    txFreqSpinBox->setSingleStep(0.001);

    rxFreqSpinBox->setDecimals(6);
    rxFreqSpinBox->setMaximum(99999);
    rxFreqSpinBox->setSuffix(" " + tr("MHz"));
    rxFreqSpinBox->setKeyboardTracking(false);
    rxFreqSpinBox->setSingleStep(0.001);

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

    QLabel *rstLabelN = new QLabel(this);
    rstLabelN->setText(tr("RST"));
    rstLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstTxLabelN = new QLabel(this);
    rstTxLabelN->setText(tr("TX"));
    rstTxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstRxLabelN = new QLabel(this);
    rstRxLabelN->setText(tr("RX"));
    rstRxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QHBoxLayout *RSTLabelsLayout = new QHBoxLayout;
    RSTLabelsLayout->addWidget (rstTxLabelN);
    RSTLabelsLayout->addWidget (rstLabelN);
    RSTLabelsLayout->addWidget (rstRxLabelN);

    QHBoxLayout *RSTValuesLayout = new QHBoxLayout;
    RSTValuesLayout->addWidget (rstTXLineEdit);
    RSTValuesLayout->addWidget (rstRXLineEdit);

    QVBoxLayout *RSTLayout = new QVBoxLayout;
    RSTLayout->addLayout (RSTLabelsLayout);
    RSTLayout->addLayout (RSTValuesLayout);

    /*
     * QGridLayout *RSTLayout = new QGridLayout;
    RSTLayout->addWidget(rstTxLabelN, 0, 0);
    RSTLayout->addWidget(rstTXLineEdit, 1, 0);
    RSTLayout->addWidget(rstRxLabelN, 0, 1);
    RSTLayout->addWidget(rstRXLineEdit, 1, 1);
    */

    QVBoxLayout *qthLayout = new QVBoxLayout;
    qthLayout->addWidget(qthLabel);
    qthLayout->addWidget(qthLineEdit);

    QVBoxLayout *rstQTHLayout = new QVBoxLayout;
    rstQTHLayout->addLayout(RSTLayout);
    rstQTHLayout->addLayout(qthLayout);

    QLabel *freqLabelsN = new QLabel(this);
    freqLabelsN->setText(tr("Frequency"));
    freqLabelsN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //freqLabelsN->setAlignment(Qt::AlignCenter);

    QLabel *txfreqLabelN = new QLabel(this);
    txfreqLabelN->setText(tr("TX"));
    txfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //txfreqLabelN->setAlignment(Qt::AlignLeft);

    QLabel *rxfreqLabelN = new QLabel(this);
    rxfreqLabelN->setText(tr("RX"));
    rxfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //rxfreqLabelN->setAlignment(Qt::AlignRight);

    QHBoxLayout *freqTXLayout = new QHBoxLayout;
    freqTXLayout->addWidget (txfreqLabelN);
    freqTXLayout->addWidget (splitCheckBox);

    QHBoxLayout *freqTitleLayout = new QHBoxLayout;
    freqTitleLayout->addLayout(freqTXLayout);
    freqTitleLayout->addWidget(freqLabelsN);
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
    namePwrLayout->addStretch(1);
    namePwrLayout->addLayout(namePwrHLayout);
    namePwrLayout->addStretch(5);

    QVBoxLayout *qsoInputTabWidgetMainLayout = new QVBoxLayout;
    qsoInputTabWidgetMainLayout->addLayout(rstfreqLayout);
    qsoInputTabWidgetMainLayout->addLayout(namePwrLayout);
    //qsoInputTabWidgetMainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(qsoInputTabWidgetMainLayout);

    connect(rstTXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(rstRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed() ) );
    //connect(nameLineEdit, SIGNAL(paletteChanged(QPalette)), this, SLOT(slotPaletteChanged(QPalette)) );
    connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
    connect(txFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqTXChanged(double)) ) ;
    connect(rxFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqRXChanged(double)) ) ;
    connect(splitCheckBox, SIGNAL(clicked()), this, SLOT(slotSplitClicked()) ) ;

    QWidget::setTabOrder (rstTXLineEdit, rstRXLineEdit);
    QWidget::setTabOrder (rstRXLineEdit, nameLineEdit);
    QWidget::setTabOrder (nameLineEdit, qthLineEdit);
    QWidget::setTabOrder (qthLineEdit, locatorLineEdit);
    QWidget::setTabOrder (locatorLineEdit, rxPowerSpinBox);
}

QSO MainWindowInputQSO::getQSOData(QSO _qso)
{
    QSO qso = _qso;
    qso.setGridSquare(getDXLocator());
    qso.setName(getName());
    qso.setQTH(getQTH());
    qso.setRSTRX(getRSTRX());
    qso.setRSTTX(getRSTTX());
    qso.setFreq(getTXFreq());
    qso.setFreqRX(getRXFreq());
    qso.setRXPwr(getRXPwr());
    setRSTToMode(qso.getSubmode(), true);

    qso.setBandRX (dataProxy->getBandNameFromFreq (getRXFreq()));
    qso.setBand(dataProxy->getBandNameFromFreq (getTXFreq()));
    return qso;
}

void MainWindowInputQSO::setQSOData(const QSO &_qso)
{
    QSO qso(_qso);
    setDXLocator(qso.getGridSquare());
    setName(qso.getName());
    setQTH(qso.getQTH());
    setRSTRX(qso.getRSTRX());
    setRSTTX(qso.getRSTTX());
    setTXFreq(qso.getFreqTX());
    setRXFreq(qso.getFreqRX());
    setRXPwr(qso.getRXPwr());
}

void MainWindowInputQSO::setDefaultData()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);
    completedWithPreviousName = false;
    completedWithPreviousQTH = false;
    completedWithPreviousLocator = false;
    rxFreqBeingAutoChanged = false;
    txFreqBeingAutoChanged = false;
    isSATPropagation = false;
    propMode = QString();
    freqTX = 0.0;
    freqRX = 0.0;
    modify = false;
    readDarkMode();
}

void MainWindowInputQSO::readDarkMode()
{
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("Colors");
    darkMode = settings.value("DarkMode", false).toBool ();
    settings.endGroup ();

    if (darkMode) {
        txFreqSpinBox->setPalette(palWhite);
        rxFreqSpinBox->setPalette(palWhite);
        //qDebug() << Q_FUNC_INFO << " - DarkMode: ON";
    } else {
        //qDebug() << Q_FUNC_INFO << " - DarkMode: OFF";
        txFreqSpinBox->setPalette(palBlack);
        rxFreqSpinBox->setPalette(palBlack);
    }
}

void MainWindowInputQSO::clear()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    qthLineEdit->clear();
    nameLineEdit->clear();
    locatorLineEdit->clear();
    rxPowerSpinBox->setValue(0);
    modify = false;
}

void MainWindowInputQSO::cleanQRZCOM(const bool _dataFromQRZCOM)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (_dataFromQRZCOM)
    {
        qthLineEdit->clear();
        nameLineEdit->clear();
        locatorLineEdit->clear();
    }
}

void MainWindowInputQSO::clearName()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    nameLineEdit->clear();
    completedWithPreviousName = false;
}

void MainWindowInputQSO::clearQTH()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    qthLineEdit->clear();
    completedWithPreviousQTH = false;
}

void MainWindowInputQSO::clearDXLocator()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    locatorLineEdit->clear ();
    completedWithPreviousLocator = false;
}

void MainWindowInputQSO::slotReturnPressed()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    emit returnPressed();
}

void MainWindowInputQSO::slotLocatorTextChanged()
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
   //qDebug() << Q_FUNC_INFO << " - Start";
    int cursorP = locatorLineEdit->cursorPosition();

    locatorLineEdit->setText((util->getClearSQLi(locatorLineEdit->text())).toUpper());

    if ( util->isValidGrid((locatorLineEdit->text()).toUpper()) || locatorLineEdit->text ().isEmpty ())
    {
        //qDebug() << Q_FUNC_INFO << ": VALID: " << locatorLineEdit->text();
        setPaletteRightDXLocator(true);
        emit dxLocatorChanged (locatorLineEdit->text());

        //dxLocator = (locatorLineEdit->text());
        //infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
        //satTabWidget->setLocator(dxLocator);
        locatorLineEdit->setToolTip(tr("DX QTH locator."));
        //qDebug() << Q_FUNC_INFO << ": " << locator->getLat(locatorLineEdit->text());
        //qDebug() << Q_FUNC_INFO << ": LON: " << locator->getLon(locatorLineEdit->text());
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": NOT VALID: " << locatorLineEdit->text();
        setPaletteRightDXLocator(false);
        locatorLineEdit->setToolTip(tr("DX QTH locator. Format should be Maidenhead like IN70AA up to 10 characters."));
        locatorLineEdit->setCursorPosition(cursorP);
        return;
    }
    locatorLineEdit->setCursorPosition(cursorP);
}

QString MainWindowInputQSO::getDXLocator()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return locatorLineEdit->text();
}

void MainWindowInputQSO::setDXLocator(const QString &_loc, bool _completing)
{
  //qDebug() << Q_FUNC_INFO << " - Start";
    if (!util->isValidGrid(_loc))
       return;
    if (_completing)
    {
        locatorLineEdit->setText (_loc.toUpper ());
    }

    if ((_loc.length()>=0) && ((getDXLocator()).length()<=0) )
    {
        setPaletteRightDXLocator (true);
        locatorLineEdit->setText (_loc.toUpper ());
        completedWithPreviousLocator=true;
    }
    else if (completedWithPreviousLocator && (_loc != getDXLocator()))
    {
        completedWithPreviousLocator = false;
        setPaletteRightDXLocator(false);
    }
}

QString MainWindowInputQSO::getName()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return nameLineEdit->text ();
}

void MainWindowInputQSO::setName(const QString &_st, bool _completing)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (!_completing)
    {
        nameLineEdit->setText (_st);
        completedWithPreviousName = false;
        return;
    }

    if ((_st.length()>=0) && (getName().length()<=0) )
    {
        //qDebug() << Q_FUNC_INFO << ": name: 1" ;
        nameLineEdit->setText (_st);
        setPaletteRightName (true);
        completedWithPreviousName = true;
    }
    else if (completedWithPreviousName && (_st != getName()))
    {
        //qDebug() << Q_FUNC_INFO << ": name: 2" ;
        completedWithPreviousName = false;
        setPaletteRightName (false);
    }
}

double MainWindowInputQSO::getTXFreq()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return txFreqSpinBox->value();
}

void MainWindowInputQSO::setTXFreq(const double _ft)
{
   //qDebug() << Q_FUNC_INFO << " - Start";

    if ( (_ft >= double(0)) && (_ft <= txFreqSpinBox->maximum()))
    {
        //qDebug() << Q_FUNC_INFO << ": defining FR: " << QString::number(_ft);
        txFreqSpinBox->setValue(_ft);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": defining FR: 0" ;
        txFreqSpinBox->setValue(0);
    }
    //qDebug() << Q_FUNC_INFO;
}

double MainWindowInputQSO::getRXFreq()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return rxFreqSpinBox->value();
}

void MainWindowInputQSO::setRXFreq(const double _ft)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
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
   //qDebug() << Q_FUNC_INFO << " - Start: " << _pw;
    if (_pw>=0)
    {
        rxPowerSpinBox->setValue(_pw);
    }
}

QString MainWindowInputQSO::getQTH()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return qthLineEdit->text();
}

void MainWindowInputQSO::setQTH(const QString &_st, bool _completing)
{// if _completing, I a completing from a previous QSO
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (!_completing)
    {
        qthLineEdit->setText(_st);
        return;
    }

    if ((_st.length()>=0) && (getQTH().length()<=0) )
    {
        setPaletteRightQTH (true);
        completedWithPreviousQTH = true;
        qthLineEdit->setText(_st);
    }
    else if (completedWithPreviousQTH && (_st != getQTH()))
    {
        completedWithPreviousQTH = false;
        setPaletteRightQTH (false);
    }
}

QString MainWindowInputQSO::getRSTTX()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return rstTXLineEdit->text();
}

void MainWindowInputQSO::setRSTTX(const QString &_st)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    rstTXLineEdit->setText (_st);
}

QString MainWindowInputQSO::getRSTRX()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    return rstRXLineEdit->text();
}

void MainWindowInputQSO::setRSTRX(const QString &_st)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    rstRXLineEdit->setText (_st);
}


void MainWindowInputQSO::setRSTToMode(const QString &_m, const bool _reading)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    //qDebug() << Q_FUNC_INFO << ": " << _m ;

    if ((_m == "SSB") || (_m== "LSB") || (_m=="USB") || (_m == "FM") || (_m == "AM") )
    {
         //qDebug() << Q_FUNC_INFO << ": Detected SSB/LSB/USB" ;
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
         //qDebug() << Q_FUNC_INFO << ": reading the UI";
        rstTXLineEdit->setText(util->getDefaultRST(_m));
        rstRXLineEdit->setText(util->getDefaultRST(_m));
    }
}

//void MainWindowInputQSO::slotPaletteChanged(QPalette _p)
//{
   //qDebug() << Q_FUNC_INFO << " - " << nameLineEdit->palette().color(QPalette::Base);
//}

//bool MainWindowInputQSO::getdarkMode
//{
    //qDebug() << Q_FUNC_INFO << " - Start: " << nameLineEdit->palette().color(QPalette::Base).name(QColor::HexRgb);
    //New faf9f8
    //FOrmer color #646464
    //qDebug() << Q_FUNC_INFO << " - Start: " << util->boolToQString((nameLineEdit->palette().color(QPalette::Base).name(QColor::HexRgb) == "#646464"));
//    return darkMode;
    //return  (nameLineEdit->palette().color(QPalette::Base).name(QColor::HexRgb) == "#faf9f8");
//}

void MainWindowInputQSO::setPaletteRightName(const bool _ok)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (_ok)
    {
        if (darkMode)
        {
            nameLineEdit->setPalette (palWhite);
        }
        else
        {
            nameLineEdit->setPalette (palBlack);
        }
    }
    else
    {
        nameLineEdit->setPalette (palRed);
    }
}

void MainWindowInputQSO::setPaletteRightQTH(const bool _ok)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (_ok)
    {
        if (darkMode)
        {
            qthLineEdit->setPalette (palWhite);
        }
        else
        {
            qthLineEdit->setPalette (palBlack);
        }
    }
    else
    {
        qthLineEdit->setPalette (palRed);
    }
}

void MainWindowInputQSO::setPaletteRightDXLocator(const bool _ok)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (_ok)
    {
        if (darkMode)
        {
            locatorLineEdit->setPalette (palWhite);
        }
        else
        {
            locatorLineEdit->setPalette (palBlack);
        }
    }
    else
    {
        locatorLineEdit->setPalette (palRed);
    }
}

void MainWindowInputQSO::setPropModeFromSat(const QString &_p)
{
    //qDebug() << Q_FUNC_INFO << ": " << _p;
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
   //qDebug() << Q_FUNC_INFO << ": " << QString::number(_f);
    if (util->isSameFreq (_f, freqTX))
    {
       //qDebug() << Q_FUNC_INFO << ": Same Freq return" ;
        return;
    }
    freqTX = _f;
    int bandId = dataProxy->getBandIdFromFreq(_f);
    if (bandId > 1)
    { // If the freq belongs to one ham band
        txFreqSpinBox->setToolTip(tr("TX Frequency in MHz."));
        if (darkMode)
        {
            //qDebug() << Q_FUNC_INFO << " - We are in darkmode";
            txFreqSpinBox->setPalette(palWhite);
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - We are NOT in darkmode";
            txFreqSpinBox->setPalette(palBlack);
        }
       //qDebug() << Q_FUNC_INFO << ": emitting: " << QString::number(_f);
        emit txFreqChanged (_f);
    }
    else
    {
        txFreqSpinBox->setToolTip(tr("TX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        txFreqSpinBox->setPalette(palRed);
        //qDebug() << Q_FUNC_INFO << ":RED - Not in band " ;
    }
    if ((!splitCheckBox->isChecked()) && !modify)
    {
        rxFreqSpinBox->setValue (_f);
       //qDebug() << Q_FUNC_INFO << ": copying to RX Freq " ;
    }
   //qDebug() << Q_FUNC_INFO << " - END" ;
    setSplitCheckBox();
}

void MainWindowInputQSO::slotSplitClicked()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (!splitCheckBox->isChecked())
    {
        rxFreqSpinBox->setValue(txFreqSpinBox->value());
    }
}

void MainWindowInputQSO::slotFreqRXChanged(double _f)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (util->isSameFreq (_f, freqRX))
    {
        return;
    }
    freqRX = _f;
    int bandId = dataProxy->getBandIdFromFreq(_f);
    if (bandId > 1)
    { // If the freq belongs to one ham band
        if (darkMode)
        {
            //qDebug() << Q_FUNC_INFO << " - We are in darkmode";
            rxFreqSpinBox->setPalette(palWhite);
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - We are NOT in darkmode";
            rxFreqSpinBox->setPalette(palBlack);
        }
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));
        emit rxFreqChanged(rxFreqSpinBox->value());
    }
    else
    {
        rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        rxFreqSpinBox->setPalette(palRed);
         //qDebug() << "MainWindow::slotFreqRXChanged Freq is not in ANY ham band";
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
                //qDebug() << "MainWindow::slotFreqRXChanged Freq is not in ANY ham band";
    }
    if (!rxFreqBeingAutoChanged)
    {
                //qDebug() << "MainWindow::slotFreqTXChanged: Updating SAT Downlink";
        emit rxFreqChangedForSat(rxFreqSpinBox->value());
    }
    */
    //qDebug() << "MainWindow::slotFreqRXChanged: END";
    setSplitCheckBox();
}

void MainWindowInputQSO::setSplitCheckBox()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
   if (util->isSameFreq(txFreqSpinBox->value(), rxFreqSpinBox->value()))
    {
        splitCheckBox->setChecked(false);
    }
    else
    {
        splitCheckBox->setChecked(true);
    }
}

void MainWindowInputQSO::receiveFocus()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    rstTXLineEdit->setFocus ();
}

bool MainWindowInputQSO::eventFilter (QObject *object, QEvent *event)
{
    Q_UNUSED(object);
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (!(event->type() == QEvent::Paint ))
    {
        //qDebug() << Q_FUNC_INFO << ": " << QString::number(event->type ());
    }
    //qDebug() << Q_FUNC_INFO << ": " << object->objectName();

    if ((event->type() == QEvent::KeyPress) || (event->type() == QEvent::ShortcutOverride)) {
        //qDebug() << Q_FUNC_INFO << "KEY PRESSED";
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Tab) {
            //qDebug() << Q_FUNC_INFO << "KEY PRESSED TAB";
            if (rxPowerSpinBox->hasFocus())
            {
                //qDebug() << Q_FUNC_INFO << "emitting to hand over to mainQSO Input";
                emit handOverFocusSignal();
            }
            // special tab handling here
            return true;
        }
    }
    return QWidget::event(event);
}

