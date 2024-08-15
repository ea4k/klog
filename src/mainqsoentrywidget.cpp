
/***************************************************************************tv
                          mainqsoentrywidget.cpp  -  description
                             -------------------
    begin                : january 2020
    copyright            : (C) 2020 by Jaime Robles
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
//https://github.com/ea4k/klog/commit/1ac02c7b779922a8e5907ebee088fa64654cbdf5
#include "mainqsoentrywidget.h"

MainQSOEntryWidget::MainQSOEntryWidget(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
       //qDebug()<< Q_FUNC_INFO;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    modifyingBands = false;
    upAndRunning = false;
    dataProxy = dp;
    qrzLineEdit = new QLineEdit;
    bandComboBox = new QComboBox;
    modeComboBox = new QComboBox;
    dateEdit = new QDateEdit;
    timeEdit = new QTimeEdit;
    realtimeCheckBox = new QCheckBox;
    enabledCR = realtimeCheckBox->backgroundRole();
    manualModeCheckBox = new QCheckBox;

    QRZButton = new QPushButton("QRZ");
    OKButton = new QPushButton(tr("&Add"), this);
    clearButton = new QPushButton(tr("&Clear"), this);
    timer = new QTimer(this);
    util = new Utilities(Q_FUNC_INFO);
    realTime = true;
    duplicatedQSOSlotInSecs = 15;
    delayInputTimer = new QTimer;
    logLevel = None;

    createUI();
    setInitialData();
    //installEventFilter (this);
    logEvent (Q_FUNC_INFO, "END", Debug);
       //qDebug()<< Q_FUNC_INFO << " - END";
}

MainQSOEntryWidget::~MainQSOEntryWidget()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    delete(util);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

/*
void MainQSOEntryWidget::slotRealTimeButtonResize()
{
    int heigh = qrzLineEdit->sizeHint ().height ();
    realtimeButton->setFixedSize(QSize(heigh, heigh));
    //realtimeButton->setSizePolicy (QSizePolicy::Fixed);

}
*/
void MainQSOEntryWidget::createUI()
{
       //qDebug()<< Q_FUNC_INFO;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    qrzLineEdit->setToolTip(tr("Callsign of the QSO."));
    bandComboBox->setToolTip(tr("Band of the QSO."));
    modeComboBox->setToolTip(tr("Mode of the QSO."));
    dateEdit->setToolTip(tr("Date of the QSO."));
    timeEdit->setToolTip(tr("Time of the QSO."));
    OKButton->setToolTip(tr("Add the QSO to the log."));
    clearButton->setToolTip(tr("Clears the QSO entry."));
    QRZButton->setToolTip(tr("Look up info about the current callsign on qrz.com"));
    realtimeCheckBox->setToolTip(tr("KLog will show real time if enabled."));
    realtimeCheckBox->setText (tr("Real time"));
    //realtimeButton->setToolTip(tr("KLog will show real time if enabled."));
    manualModeCheckBox->setToolTip(tr("Stop wsjt-x and hamlib from automatically updating QSO information."));
    manualModeCheckBox->setText (tr("Manual Mode"));

    QHBoxLayout *TimeLayout = new QHBoxLayout;
    TimeLayout->addWidget(dateEdit);
    TimeLayout->addWidget(timeEdit);
    //TimeLayout->addWidget(realtimeButton);
    TimeLayout->addWidget(realtimeCheckBox);
    TimeLayout->addWidget(manualModeCheckBox);
    TimeLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));

    QHBoxLayout *BandModeLayout = new QHBoxLayout;
    BandModeLayout->addWidget(bandComboBox);
    BandModeLayout->addWidget(modeComboBox);

    QHBoxLayout *QrzBandModeLayout = new QHBoxLayout;
    QrzBandModeLayout->addWidget(qrzLineEdit);
    QrzBandModeLayout->addLayout(BandModeLayout);

    qrzgroupBox = new QGroupBox(tr("Callsign"));
    qrzgroupBox->setFlat(true);
    QVBoxLayout *qrzvbox = new QVBoxLayout;
    qrzvbox->addLayout(QrzBandModeLayout);
    qrzgroupBox->setLayout(qrzvbox);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(QRZButton);
    buttonsLayout->addWidget(OKButton);
    buttonsLayout->addWidget(clearButton);
    dateEdit->setCalendarPopup (true);;

    dateEdit->setDisplayFormat("yyyy-MM-dd");
    timeEdit->setDisplayFormat("HH:mm:ss");

    QGridLayout *widgetLayout = new QGridLayout;
    widgetLayout->addWidget(qrzgroupBox, 0, 0, 1, 0);
    widgetLayout->addLayout(TimeLayout, 1, 0);
    widgetLayout->addLayout(buttonsLayout,1, 1);
    //widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(widgetLayout);

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);
    palWhite.setColor(QPalette::Text, Qt::white);

    connect(util, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)));
    connect(qrzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotOKButtonClicked() ) );
    connect(qrzLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotStartDelayInputTimer() ) );
    connect(delayInputTimer, SIGNAL(timeout()), this, SLOT(slotDelayInputTimedOut() ) );

    connect(bandComboBox, SIGNAL(currentTextChanged (QString)), this, SLOT(slotBandComboBoxChanged(QString) ) ) ;
    connect(modeComboBox, SIGNAL(currentTextChanged (QString)), this, SLOT(slotModeComboBoxChanged(QString) ) ) ;

    connect(OKButton, SIGNAL(clicked()), this, SLOT(slotOKButtonClicked()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked()));
    connect(QRZButton, SIGNAL(clicked()), this, SLOT(slotQRZButtonClicked()));

    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()) );
    //connect(realtimeButton, SIGNAL(clicked()), this, SLOT(slotRealtimeButtonClicked()) );
    connect(realtimeCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxClicked()));
    connect(manualModeCheckBox, SIGNAL(clicked()), this, SLOT(slotManualModeCheckBoxClicked()));

    QWidget::setTabOrder (qrzLineEdit, dateEdit);
    QWidget::setTabOrder (dateEdit, timeEdit);

    logEvent (Q_FUNC_INFO, "END", Debug);
     //qDebug() << Q_FUNC_INFO << ": (" << QString::number(this->size ().width ()) << "/" << QString::number(this->size ().height ()) << ")" ;
}

void MainQSOEntryWidget::setShowSeconds(const bool &t)
{
    if (t)
    {
        timeEdit->setDisplayFormat("HH:mm:ss");
    }
    else
    {
        timeEdit->setDisplayFormat("HH:mm");
    }
}

void MainQSOEntryWidget::setLogLevel (const DebugLogLevel _b)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    logLevel = _b;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::setCallValidation (const bool _b)
{
    util->setCallValidation(_b);
    dataProxy->setCallValidation(_b);
}

void MainQSOEntryWidget::slotCheckBoxClicked()
{
   logEvent (Q_FUNC_INFO, "Start", Debug);
    if (realtimeCheckBox->isChecked())
    {
        realTime = true;
        //realtimeButton->setIcon(QIcon(":/img/play.svg"));
        timeEdit->setBackgroundRole(enabledCR);
    }
    else
    {
        realTime = false;
        //realtimeButton->setIcon(QIcon(":/img/stop.svg"));
        timeEdit->setBackgroundRole(QPalette::BrightText);
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotManualModeCheckBoxClicked()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (manualModeCheckBox->isChecked())
    {
        slotClearButtonClicked();
        emit manualModeSignal(true);
        //stop hamlib and wsjt-x communication;
    }
    else
    {
        emit manualModeSignal(false);
        //start hamlib and wsjt-x communication;
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

/*
void MainQSOEntryWidget::slotRealtimeButtonClicked()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);

    if (realtimeButton->isChecked())
    {
        realTime = true;
        realtimeButton->setIcon(QIcon(":/img/stop.svg"));
        timeEdit->setBackgroundRole(enabledCR);
    }
    else
    {
        realTime = false;
        realtimeButton->setIcon(QIcon(":/img/play.svg"));
        timeEdit->setBackgroundRole(QPalette::BrightText);
    }

    logEvent (Q_FUNC_INFO, "END", Debug);
}
*/

void MainQSOEntryWidget::setCleaning (const bool _c)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    cleaning = _c;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotQRZTextChanged()
{
    logEvent (Q_FUNC_INFO, "Start: " + qrzLineEdit->text(), Debug);
     //qDebug()<< Q_FUNC_INFO << qrzLineEdit->text() << " / Length: " << QString::number((qrzLineEdit->text()).size()) << "###### START ######";
    logEvent (Q_FUNC_INFO, "Start", Debug);

    if ((qrzLineEdit->text()).length()<1)
    {
        //qDebug() << Q_FUNC_INFO << ": qrz length <1";
        slotClearButtonClicked();
        logEvent (Q_FUNC_INFO, "END-1", Debug);
         //qDebug()<< Q_FUNC_INFO << ":  QRZ <1 - END";
    return;
    }
    int cursorP = qrzLineEdit->cursorPosition();
    //qDebug()<< Q_FUNC_INFO << ": cursor position: " << QString::number(cursorP);
    qrzLineEdit->setText((qrzLineEdit->text()).toUpper());
    if (cleaning)
    {
         //qDebug() << Q_FUNC_INFO << ": Cleaning - END";
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return;
    }

    if (qrzAutoChanging)
    {
         //qDebug()<< Q_FUNC_INFO << ": qrzAutoChanging - END";
        qrzAutoChanging = false;
        logEvent (Q_FUNC_INFO, "END-3", Debug);
        return;
    }

    qrzAutoChanging = true;

     //qDebug()<< Q_FUNC_INFO << ": cursor position.1: " << QString::number(cursorP);

    if ( (qrzLineEdit->text()).endsWith(' ') )
    {/*Remove the space and moves the focus to SRX to write the RX exchange*/
        previousQRZ = (qrzLineEdit->text()).simplified();
        qrzLineEdit->setText(previousQRZ);
        //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: Space detected";
    }

     //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: Simplifiying & Capitalizing";
    qrzLineEdit->setText(((qrzLineEdit->text())).simplified());
    qrzLineEdit->setText((qrzLineEdit->text()).remove(" "));

    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: checking for invalid chars";
    //TODO: This validCharactersInCall may be removed?
    InValidCharsInPrevCall = validCharactersInCall(qrzLineEdit->text());
     //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: checking for invalid chars 00 ";
    if (!util->isValidCall(qrzLineEdit->text()))
    {
        qrzLineEdit->setPalette(palRed);
        //emit showInfoLabel(tr("Callsign not valid"));
         //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: QRZ not valid - Red";
        //logEvent (Q_FUNC_INFO, "END-4", Debug);
    }
    else
    {
         //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: QRZ is valid - Black";
        if (getDarkMode())
        {
            qrzLineEdit->setPalette(palWhite);
        }
        else
        {
            qrzLineEdit->setPalette(palBlack);
        }

        currentQrz = qrzLineEdit->text();
        //emit showInfoLabel(tr(""));
    }
    /*
       //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: checking for length";
    if (((qrzLineEdit->text()).length() < 1))
    { // If QRZ box is blank, Information labels should be cleared.
        slotClearButtonClicked();
        logEvent (Q_FUNC_INFO, "END-5", Debug);
        return;
    }
    */

     //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: checking for modify or length<1";
    if (qrzSmallModDontCalculate)
    //if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
        //qDebug() << "MainQSOEntryWidget::slotQRZTextChanged: qrzSmallModDontCalculate < 1";
        qrzSmallModDontCalculate=false;
        logEvent (Q_FUNC_INFO, "END-6", Debug);
        return;
    }

     //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: running...";
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    currentQrz = qrzLineEdit->text();

    if ((currentQrz).count('\\'))
    { // Replaces \ by / to ease operation.
        //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: Replacing \\ by /";
        currentQrz.replace(QChar('\\'), QChar('/'));
        qrzLineEdit->setText(currentQrz);
    }

    currentQrz = qrzLineEdit->text();
     //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.3: " << QString::number(cursorP);
    if (cursorP>currentQrz.length())
    {// A Space that has been removed without updating the cursor
         //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursorP > currentQRZ.length";
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " cursorP<currentQrz.length: " << QString::number(cursorP);

        if (cursorP>0)
        {
            if ((currentQrz.at(cursorP-1)).isSpace())
            {
                //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.5: " << QString::number(cursorP);
                previousQRZ = currentQrz.remove(cursorP-1, 1);
                //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.6: " << QString::number(cursorP);
                cursorP--;
                //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.7: " << QString::number(cursorP);
                qrzLineEdit->setText(previousQRZ);
            }
        }
    }
    currentQrz = qrzLineEdit->text();
     //qDebug() << "MainQSOEntryWidget::slotQRZTextChanged: Emitting: " << currentQrz;
    emit currentQRZSignal(currentQrz);

    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
     //qDebug() << "MainQSOEntryWidget::slotQRZTextChanged: cursorP at the end : " << QString::number(cursorP);
    qrzLineEdit->setCursorPosition(cursorP);
    checkIfDupe(Q_FUNC_INFO);
    qrzAutoChanging = false;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::setCurrentQRZ(const QString &_qrz)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    currentQrz = _qrz;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotBandComboBoxChanged(const QString &_b){
    //qDebug() << Q_FUNC_INFO << ":  << _b;

    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (modifyingBands){
         //qDebug() << Q_FUNC_INFO << ": Modifying bands - END" ;
        return;
    }
    bottomBandLimit = dataProxy->getLowLimitBandFromBandName (_b);
    upperBandLimit = dataProxy->getUpperLimitBandFromBandName (_b);
    emit bandChanged(_b);
    checkIfDupe(Q_FUNC_INFO);
    logEvent (Q_FUNC_INFO, "END", Debug);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainQSOEntryWidget::slotModeComboBoxChanged(const QString &_m)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);

    emit modeChanged(_m);
    checkIfDupe(Q_FUNC_INFO);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotQRZButtonClicked()
{
    if ((qrzLineEdit->text()).length()<1)
    {
        return;
    }
    util->openQrzcom(currentQrz);
}

void MainQSOEntryWidget::slotOKButtonClicked()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ((qrzLineEdit->text()).length()<1)
    {
        return;
    }
    emit OKClicked();
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotClearButtonClicked()
{
     //qDebug() << "MainQSOEntryWidget::slotClearButtonClicked: " ;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    setCleaning (true);
    clear();
    emit clearForNextQSOSignal(Q_FUNC_INFO);
    //setModify(false);
    checkIfDupe(Q_FUNC_INFO);
    setCleaning (false);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::clear()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);

    //qDebug() << Q_FUNC_INFO;

    setModify(false);
    //OKButton->setText(tr("&Add"));
    qrzLineEdit->clear();
    qrzLineEdit->setFocus(Qt::OtherFocusReason);

    //cleaning = false;
    //qDebug() << Q_FUNC_INFO << " - END";
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::setInitialData()
{
       //qDebug()<< "MainQSOEntryWidget::setInitialData";
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //Default band/modes
    modify = false;
    qrzAutoChanging = false;
    InValidCharsInPrevCall = false;
    qrzSmallModDontCalculate=false;
    previousQRZ = QString();

    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());

    bands.clear();
    modes.clear();
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW";

    setBands(bands);
    setModes(modes);

    selectDefaultBand(true);
    selectDefaultMode(true);

    dateEdit->setDate(QDate::currentDate());
    timeEdit->setTime(QTime::currentTime());

    UTCTime = true;
    //  //qDebug()ime = true;

    timer->start(1000);

    logEvent (Q_FUNC_INFO, "END", Debug);
      //qDebug()<< "MainQSOEntryWidget::setInitialData-END";
}

bool MainQSOEntryWidget::updateBandComboBox(const QString &_band)
{
    logEvent (Q_FUNC_INFO, "Start: " + _band, Debug);
    //qDebug() << Q_FUNC_INFO << ":  << _band;
    //QString _currentBand = getBand();
    if (!isBandExisting(_band))
    {// The selected frequency is of a band that is not currently selected
        QString _currentBand = bandComboBox->currentText ();
        //qDebug() << Q_FUNC_INFO << ":  New band found: " << _band;
        if (dataProxy->getIdFromBandName(_band) > 1)
        {// Not affected if 0 (light) is the frequency
         // In this case the user should select the band in the setup
            //qDebug() << Q_FUNC_INFO << ":  Band is valid: " << _band;
            QStringList qsTemp;
            qsTemp.clear();
            qsTemp << bands;
            qsTemp << _band;
            qsTemp.removeDuplicates();
            QString aux;
            foreach(aux, bands)
            {
                //qDebug() << " bands Band: " << aux;
            }
            foreach(aux, qsTemp)
            {
                //qDebug() << " qsTemp Band: " << aux;
            }
            bands.clear();
            bands = dataProxy->sortBandNamesBottonUp(qsTemp);
            foreach(aux, bands)
            {
                //qDebug() << " bands2 Band: " << aux;
            }
            //qDebug() << Q_FUNC_INFO << ": Before setBands";
            setBands(bands);
            bandComboBox->setCurrentIndex(bandComboBox->findText(_currentBand, Qt::MatchCaseSensitive));
            //qDebug() << Q_FUNC_INFO << ": Band has been added : " << _band;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << ": (END) Band is NOT  valid: " <<_band;
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return false;
        }
    }
    //qDebug() << Q_FUNC_INFO << ": Band already existing, no need to add" ;
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

void MainQSOEntryWidget::setBands(const QStringList &_bands)
{
    logEvent (Q_FUNC_INFO, "Start: " + QString::number(_bands.length ()), Debug);
    //qDebug()<< Q_FUNC_INFO << " - Start: " << QString::number(_bands.length ());
    modifyingBands = true;
    //bands.clear();
    bands = _bands;
    bands.removeDuplicates();
    bands = dataProxy->sortBandNamesBottonUp(bands);
    bandComboBox->clear();
    bandComboBox->addItems(bands);
    emit validBands(_bands);
    //selectDefaultBand(true);
    modifyingBands = false;

    logEvent (Q_FUNC_INFO, "END", Debug);
    //qDebug()<< Q_FUNC_INFO << " - END";
}

void MainQSOEntryWidget::setModes(const QStringList &_modes)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
     //qDebug() << Q_FUNC_INFO;

    modes.clear();
    modes = _modes;
    modes.removeDuplicates();
    modes.sort();
    modeComboBox->clear();
    modeComboBox->addItems(modes);
    selectDefaultMode(true);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

QStringList MainQSOEntryWidget::getModes()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return modes;
}

bool MainQSOEntryWidget::setFreq(const double _f, bool isRX)
{
   //qDebug() << Q_FUNC_INFO << ":  << QString::number(_f);
    logEvent (Q_FUNC_INFO, "Start", Debug);

    if (isRX)
    {
        if (util->isSameFreq (freqRX, _f))
        {
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return true;
        }
        freqRX = _f;
    }

    if (util->isSameFreq (freqTX, _f))
    {
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return true;
    }
    freqTX = _f;

    if (newBandNeededForFreq (_f))
    {
       //qDebug() << Q_FUNC_INFO << ": New band needed";
        if ((bottomBandLimit<=freqTX) && (freqTX<= upperBandLimit))
        {
            logEvent (Q_FUNC_INFO, "END-3", Debug);
            return true;
        }
       //qDebug() << Q_FUNC_INFO << ": Freq is not in the current band";
        QString _newBand = dataProxy->getBandNameFromFreq(_f);
       //qDebug() << Q_FUNC_INFO << ": before setting band: " << _newBand ;
        if (isRX)
        {
           //qDebug() << Q_FUNC_INFO << ": RX Freq no more actions " ;
            logEvent (Q_FUNC_INFO, "END-4", Debug);
            return true;
        }
        logEvent (Q_FUNC_INFO, "END-5", Debug);
        return setBand(_newBand);
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << ": NO New band needed - REMOVE THIS else";
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return false;
}

bool MainQSOEntryWidget::newBandNeededForFreq(const double _f)
{
   //qDebug() << Q_FUNC_INFO << ": " << QString::number(_f);
    logEvent (Q_FUNC_INFO, "Start: " + QString::number(_f), Debug);
    QString _newBand = dataProxy->getBandNameFromFreq(_f);
    if (!updateBandComboBox (_newBand))
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
       //qDebug() << Q_FUNC_INFO << " - END false";
        return false;
    }
   //qDebug() << Q_FUNC_INFO << " - END true ";
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

bool MainQSOEntryWidget::setBand(const QString &_band)
{
    //qDebug() << Q_FUNC_INFO << ": " << _band;

    logEvent (Q_FUNC_INFO, "Start: " + _band, Debug);
    if (bandComboBox->findText(_band, Qt::MatchCaseSensitive) < 0)
    {
         //qDebug() << Q_FUNC_INFO << ": Band not found " << _band;
        if (!updateBandComboBox(_band))
        {
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return false;
        }
    }
     //qDebug() << "MainQSOEntryWidget::setBand-1: Band found " << _band;
    bandComboBox->setCurrentIndex(bandComboBox->findText(_band, Qt::MatchCaseSensitive));
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

bool MainQSOEntryWidget::setMode(const QString &_mode)
{
    logEvent (Q_FUNC_INFO, "Start" + _mode, Debug);
     //qDebug() << Q_FUNC_INFO << ":  " << _mode;
    if (modeComboBox->findText(_mode, Qt::MatchCaseSensitive) < 0)
    {
         //qDebug() << Q_FUNC_INFO << ":  NOT found";
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return false;
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << ":  Updated";
        modeComboBox->setCurrentIndex(modeComboBox->findText(_mode, Qt::MatchCaseSensitive));
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return true;
    }
}

bool MainQSOEntryWidget::setQRZ(const QString &_qrz)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    //TODO: Add validations to prevent that non valid qrz are sent from the outside of this function or at least manage this appropriately.
    qrzLineEdit->setText(_qrz.toUpper());
    logEvent (Q_FUNC_INFO, "END", Debug);
    return false;
}

bool MainQSOEntryWidget::setDateTime(const QDateTime _date)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_date.isValid())
    {
        dateEdit->setDate(_date.date());
        timeEdit->setTime(_date.time());
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return true;
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << ": - NO VALID DATE";
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return false;
    }
}

bool MainQSOEntryWidget::setTime(const QTime _time)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_time.isValid())
    {
        timeEdit->setTime(_time);
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return true;
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return false;
    }
}

QString MainQSOEntryWidget::getQrz()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QString aux;
    aux = qrzLineEdit->text();
    if(aux.length()<1)
    {
        logEvent(Q_FUNC_INFO, "END - Not valid", Debug);
        return QString();
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
    return aux.toUpper();
}

QString MainQSOEntryWidget::getBand(const int _b)
{
    logEvent (Q_FUNC_INFO, "Start: " + QString::number(_b), Debug);
    if (_b<0)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        //qDebug() << Q_FUNC_INFO << " (1): " << bandComboBox->currentText()<< QT_ENDL;
        return bandComboBox->currentText();
    }
    else
    {
        if (bandComboBox->count()>=_b)
        {
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            //qDebug() << Q_FUNC_INFO << " (2): " << bandComboBox->currentText()<< QT_ENDL;
            return bandComboBox->itemText(_b);
        }
        else
        {
            logEvent (Q_FUNC_INFO, "END-3", Debug);
             //qDebug() << Q_FUNC_INFO << " (3): " << bandComboBox->currentText()<< QT_ENDL;
            return bandComboBox->currentText();
        }
    }
}

QString MainQSOEntryWidget::getMode(int _m)
{
    logEvent (Q_FUNC_INFO, "Start: " + QString::number(_m), Debug);
    if (_m<0)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return modeComboBox->currentText();
    }
    else
    {
        if (modeComboBox->count()>=_m)
        {
            logEvent (Q_FUNC_INFO, "END-2", Debug);
            return modeComboBox->itemText(_m);
        }
        else
        {
            logEvent (Q_FUNC_INFO, "END-3", Debug);
            return modeComboBox->currentText();
        }
    }
}

QDate MainQSOEntryWidget::getDate()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return dateEdit->date();
}


QDateTime MainQSOEntryWidget::getDateTime()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //logEvent (Q_FUNC_INFO, "END", Debug);
    QDateTime dateTime;
    dateTime.setDate(dateEdit->date());
    dateTime.setTime(timeEdit->time());
    logEvent (Q_FUNC_INFO, "END", Debug);
    return dateTime;
}

QTime MainQSOEntryWidget::getTime()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return timeEdit->time();
}

void MainQSOEntryWidget::toggleRealTime()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ( realtimeCheckBox->isChecked ())
    {
        setRealTime (false);
    }
    else
    {
        setRealTime (true);
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainQSOEntryWidget::getRealTime()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return realtimeCheckBox->isChecked ();
}

void MainQSOEntryWidget::setRealTime(const bool _realTime)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //qDebug << Q_FUNC_INFO << ": " << util->boolToQString (_realTime);

    realTime = _realTime;
    realtimeCheckBox->setChecked(realTime);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::setManualMode(const bool _manualMode)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    manualModeCheckBox->setChecked (_manualMode);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainQSOEntryWidget::getManualMode()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return manualModeCheckBox->isChecked ();
    //logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::setUTC(const bool _utc)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    UTCTime = _utc;
    setDateAndTimeInternally();
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::setModify(const bool _modify)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString(_modify);
    modify = _modify;
    if (modify)
    {
        OKButton->setText(tr("&Save"));
        clearButton->setText(tr("&Cancel"));
        realtimeCheckBox->setChecked (false);
    }
    else
    {
        OKButton->setText(tr("&Add"));
        clearButton->setText(tr("&Clear"));
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainQSOEntryWidget::getModifying()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return modify;
}

void MainQSOEntryWidget::slotUpdateTime()
{
    //logEvent (Q_FUNC_INFO, "Start", Debug);
    if ( (!modify) && (realtimeCheckBox->isChecked())  )
    {
        //qDebug()<< Q_FUNC_INFO << ":  Real Time & update";
        setDateAndTimeInternally();
    }
    //logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::setDateAndTimeInternally()
{
    //logEvent (Q_FUNC_INFO, "Start", Debug);
    if (UTCTime)
    {
        dateEdit->setDate(QDateTime::currentDateTime().toUTC().date());
        timeEdit->setTime(QDateTime::currentDateTime().toUTC().time());
    }
    else
    {
        dateEdit->setDate(QDateTime::currentDateTime().date());
        timeEdit->setTime(QDateTime::currentDateTime().time());
    }
    //logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainQSOEntryWidget::validCharactersInCall(const QString &_qrz)
{
     //qDebug()<< Q_FUNC_INFO << ": " << _qrz;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    for (int i = 0; i<_qrz.size()-1;i++)
    {
        if (!( ((_qrz.at(i)).isLetterOrNumber()) || (_qrz.at(i)=='\\')  || (_qrz.at(i)=='/')  ))
        {
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return false;
        }
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return true;
}

bool MainQSOEntryWidget::isModeExisting(const QString &_m)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (modeComboBox->findText(_m, Qt::MatchCaseSensitive) >= 0)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return true;
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return false;
    }
}

bool MainQSOEntryWidget::isBandExisting(const QString &_b)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
     //qDebug() << Q_FUNC_INFO << ": " << _b;
    if (bandComboBox->findText(_b, Qt::MatchCaseSensitive) >= 0)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
         //qDebug() << Q_FUNC_INFO << " - END true";
        return true;
    }
    else
    {
        logEvent (Q_FUNC_INFO, "END-2", Debug);
         //qDebug() << Q_FUNC_INFO << " - END false";
        return false;
    }
}

void MainQSOEntryWidget::setUpAndRunning(const bool _u)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    upAndRunning = _u;
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::selectDefaultBand(bool _init)
{
       //qDebug() << Q_FUNC_INFO;
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ((upAndRunning) || (!_init))
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
           //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    QString aux;
    aux = QString();
    int defaultBand = dataProxy->getMostUsedBand(-1); //TODO: The log could be defined here
       //qDebug() << Q_FUNC_INFO << ": " << QString::number(defaultBand) << dataProxy->getNameFromBandId (defaultBand)<< QT_ENDL;
    if (defaultBand<1)
    {
        defaultBand = dataProxy->getIdFromBandName(getBand(0));
    }

    aux = dataProxy->getNameFromBandId(defaultBand);
     //qDebug() << Q_FUNC_INFO << ": before setting band: " << aux;
    setBand(aux);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::selectDefaultMode(bool _init)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if ((upAndRunning) || (!_init))
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    int defaultMode = dataProxy->getMostUsedMode(-1); //TODO: The log could be defined here
       //qDebug() << Q_FUNC_INFO << ":  " << QString::number(defaultMode);

    if (defaultMode < 1)
    {
        defaultMode = dataProxy->getSubModeIdFromSubMode(getBand(0));
    }
    setMode(dataProxy->getNameFromSubModeId(defaultMode));
    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode)));

      //qDebug() << Q_FUNC_INFO << ":  " << QString::number(defaultMode);
    logEvent (Q_FUNC_INFO, "END", Debug);
      //qDebug() << Q_FUNC_INFO << " - END";
}

void MainQSOEntryWidget::setDuplicatedQSOSlot (const int _secs)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (_secs >= 0)
    {
        duplicatedQSOSlotInSecs = _secs;
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::checkIfDupe(const QString &_func)
{
    Q_UNUSED(_func);
#ifdef QT_DEBUG
    //qDebug() << Q_FUNC_INFO << "(" << _func << ")";
#else
#endif
    logEvent (Q_FUNC_INFO, "Start", Debug);
    QDateTime _dateTime;
    _dateTime.setDate(dateEdit->date());
    _dateTime.setTime(timeEdit->time());
    QSO q;
    q.setCall(qrzLineEdit->text());
    q.setDateTimeOn(_dateTime);
    q.setBand(bandComboBox->currentText());
    q.setMode(modeComboBox->currentText());
    if ((dataProxy->isThisQSODuplicated(q, duplicatedQSOSlotInSecs).length()<2) || modify)
    {
         //qDebug() << Q_FUNC_INFO << " - NOT DUPE ";
         //qDebug() << Q_FUNC_INFO << " - Modify: " << util->boolToQString(modify);
        qrzgroupBox->setTitle(tr("Callsign"));
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << " - NOT DUPE ";
        qrzgroupBox->setTitle(tr("DUPE", "Translator: DUPE is a common world for hams. Do not translate of not sure"));
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotStartDelayInputTimer()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    if (cleaning)
    {
        logEvent (Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    if (qrzLineEdit->text ().length ()<1)
    {
         //qDebug() << Q_FUNC_INFO;
        slotClearButtonClicked ();
        logEvent (Q_FUNC_INFO, "END-2", Debug);
        return;
    }

    int cursor = qrzLineEdit->cursorPosition ();
    QString aux = util->getClearSQLi (qrzLineEdit->text ());
    qrzLineEdit->setText (aux.toUpper());
    qrzLineEdit->setCursorPosition (cursor);
    delayInputTimer->start(300);
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotDelayInputTimedOut()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    delayInputTimer->stop();
    QString text = qrzLineEdit->text();
    if( text != lastQrz)
    {
         //qDebug() << Q_FUNC_INFO << ": going for slotQRZTextChanged";
        text = lastQrz;
        slotQRZTextChanged();
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
}

bool MainQSOEntryWidget::getDarkMode()
{
    logEvent (Q_FUNC_INFO, "Start-End", Debug);
    return (OKButton->palette().color (QPalette::Base) == "#646464");
}

/*
void MainQSOEntryWidget::keyPressEvent( QKeyEvent *event)
{
     //qDebug() << Q_FUNC_INFO;
    if(event->key()==Qt::Key_Enter)
    {
         //qDebug() << Q_FUNC_INFO << " TAB...";
    }
}
*/

bool MainQSOEntryWidget::eventFilter(QObject *object, QEvent *event)
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    Q_UNUSED(object);
    if (!(event->type() == QEvent::Paint ))
    {
         //qDebug() << Q_FUNC_INFO << ": " << QString::number(event->type ());
    }

    if ((event->type() == QEvent::KeyPress) || (event->type() == QEvent::ShortcutOverride)) {
         //qDebug() << Q_FUNC_INFO << "KEY PRESSED";
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Tab) {
             //qDebug() << Q_FUNC_INFO << "KEY PRESSED TAB";
            if ((realtimeCheckBox->isChecked ()) && (qrzLineEdit->hasFocus ()))
            {
                 //qDebug() << Q_FUNC_INFO << "KEY PRESSED TAB AND REAL TIME CHECKED";
                  //qDebug() << Q_FUNC_INFO << "emitting to hand over to QSO TAB-1";
                emit handOverFocusSignal();
            }
            else if((!realtimeCheckBox->isChecked ()) && timeEdit->hasFocus () && (timeEdit->currentSection() == QTimeEdit::SecondSection))
            {
                 //qDebug() << Q_FUNC_INFO << "emitting to hand over to QSO TAB-2";
                emit handOverFocusSignal();
            }
            // special tab handling here
            logEvent (Q_FUNC_INFO, "END-1", Debug);
            return true;
        }
    }
    logEvent (Q_FUNC_INFO, "END", Debug);
    return QWidget::event(event);
}

void MainQSOEntryWidget::setFocusToOK()
{
    logEvent (Q_FUNC_INFO, "Start", Debug);
    OKButton->setFocus ();
    logEvent (Q_FUNC_INFO, "END", Debug);
}

void MainQSOEntryWidget::slotCaptureDebugLogs(const QString &_func, const QString &_msg, DebugLogLevel _level)
{
     //qDebug() << Q_FUNC_INFO << _func << "/" << _msg << "/" << QString::number(_level);
    logEvent(_func, _msg, _level);
}

void MainQSOEntryWidget::logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level)
{
     //qDebug() << Q_FUNC_INFO;
    if (logLevel<=_level)
    {
         //qDebug() << Q_FUNC_INFO << "Emitting...";
        emit debugLog (_func, _msg, _level);
    }
}
