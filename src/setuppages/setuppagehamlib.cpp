/***************************************************************************
                          setuppagehamlib.cpp  -  description
                             -------------------
    begin                : feb 2018
    copyright            : (C) 2018 by Jaime Robles
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
#include "setuppagehamlib.h"
SetupPageHamLib::SetupPageHamLib(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
    Q_UNUSED(dp);
    //qDebug() << Q_FUNC_INFO ;
    hamlibTestOK = false;
    testWasRun = false;

    activateHamlibCheckBox = new QCheckBox();
    readOnlyModeCheckBox = new QCheckBox();

    tabWidget = new QTabWidget;
    serialConfigWidget = new HamLibSerialConfigWidget;
    networkConfigWidget = new HamLibNetworkConfigWidget;

    testHamlibPushButton = new QPushButton();
    defaultFreqMode = tr("000.000 / %1").arg(tr("Mode"));
    freqDisplayLabel = new QLabel(defaultFreqMode);

    rigTypeComboBox = new QComboBox;
    pollIntervalQSpinBox = new QSpinBox;


    createUI();
    setDefaults();

     //qDebug() << Q_FUNC_INFO << " - END" ;
}

void SetupPageHamLib::stopHamlib ()
{
    // No-op: the live hamlib connection is owned exclusively by MainWindow.
    // SetupDialog calls this on OK/Cancel, but with the single-connection design
    // stopping it here would kill the radio link whenever the dialog is closed.
    // MainWindow handles re-init when settings actually change.
}

bool SetupPageHamLib::wasTestRun() const
{
    return testWasRun;
}

void SetupPageHamLib::setLiveHamlib(HamLibClass *liveHamlib)
{
    if (!liveHamlib || liveHamlib == m_liveHamlib)
        return;
    m_liveHamlib = liveHamlib;
    connect(m_liveHamlib, static_cast<void (HamLibClass::*)(RadioStatus)>(&HamLibClass::radioStatusChanged),
            this, &SetupPageHamLib::slotRadioStatusChanged);
    connect(m_liveHamlib, &HamLibClass::rigDisconnected, this, [this]{
        freqDisplayLabel->setText(defaultFreqMode);
        setTestResult(false);
    });
}

void SetupPageHamLib::slotTestHamlib()
{
   //qDebug() << Q_FUNC_INFO;
    testWasRun = true;
    if (!m_liveHamlib) {
        setTestResult(false);
        return;
    }
    // Apply the form's current settings to the live hamlib, then reconnect.
    if ((rigTypeComboBox->currentText().contains("NET rigctl")) || (rigTypeComboBox->currentText().contains("FLRig"))) {
        m_liveHamlib->setNetworkPort(networkConfigWidget->getPort());
        m_liveHamlib->setNetworkAddress(networkConfigWidget->getAddress());
    } else {
        m_liveHamlib->setPort(serialConfigWidget->getSerialPort());
        m_liveHamlib->setSpeed(serialConfigWidget->getSerialBauds());
        m_liveHamlib->setParity(serialConfigWidget->getParity());
        m_liveHamlib->setFlow(serialConfigWidget->getFlowControl());
        m_liveHamlib->setStop(serialConfigWidget->getStopBits());
        m_liveHamlib->setDataBits(serialConfigWidget->getDataBits());
    }
    m_liveHamlib->setModelId(m_liveHamlib->getModelIdFromName(rigTypeComboBox->currentText()));
    m_liveHamlib->setPoll(pollIntervalQSpinBox->value());
    m_liveHamlib->stop();
    freqDisplayLabel->setText(defaultFreqMode);
    bool ok = m_liveHamlib->init(true);
    if (ok)
        ok = m_liveHamlib->readRadio();
    setTestResult(ok);
    if (!ok)
        m_liveHamlib->stop();
   //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageHamLib::slotRadioStatusChanged(RadioStatus _status)
{
    QString text = QString("%1").arg(_status.freq_VFO_RX.toDouble(MHz), 0, 'f', 4);
    if (!_status.mode_VFO_RX.isEmpty())
        text += " / " + _status.mode_VFO_RX;
    freqDisplayLabel->setText(text);
}

void SetupPageHamLib::setTestResult(const bool _ok)
{
    //qDebug() << Q_FUNC_INFO ;
    if (_ok )
    {
        //qDebug() << Q_FUNC_INFO << " - OK";
        testHamlibPushButton->setText (tr("Test: OK"));
        // Use stylesheet so the green colour survives Qt's disabled-button rendering.
        // QPalette alone is overridden by system/GTK themes on some Linux desktops.
        testHamlibPushButton->setStyleSheet(
            "QPushButton { background-color: #00cc00; color: black; }"
            "QPushButton:disabled { background-color: #00cc00; color: black; }");
        testHamlibPushButton->setEnabled(false);  // connected — nothing to test
        activateHamlibCheckBox->setEnabled (true);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - NOK";
        testHamlibPushButton->setText (tr("Test: NOK"));
        testHamlibPushButton->setStyleSheet(
            "QPushButton { background-color: #ff4444; color: white; }");
        testHamlibPushButton->setEnabled(true);   // not connected — allow retry
        activateHamlibCheckBox->setEnabled (false);
    }
    //qDebug() << Q_FUNC_INFO << " - NOK END";
}



void SetupPageHamLib::slotRadioComboBoxChanged(QString _r)
{
    //qDebug() << Q_FUNC_INFO << ": " << _r;

    if (_r == "NET rigctl")
    {
        rigctlport = 4532;
        networkConfigWidget->setPort (rigctlport);
        tabWidget->setCurrentWidget (networkConfigWidget);
         //qDebug() << Q_FUNC_INFO << ": Network radio found!" ;
        networkRadio = true;
    }
    else if (_r == "FLRig")
    {
        rigctlport = 12345;
        networkConfigWidget->setPort (rigctlport);
        tabWidget->setCurrentWidget (networkConfigWidget);
        //qDebug() << Q_FUNC_INFO << ": Network radio found!" ;
        networkRadio = true;
    }
    else
    {
        tabWidget->setCurrentWidget (serialConfigWidget);
        networkRadio = false;
    }
    setTestResult(false);
}

void SetupPageHamLib::createUI()
{
    pollMax = 10000;
    pollMin = 10;
    pollIntervalQSpinBox->setRange(pollMin, pollMax);
    pollIntervalQSpinBox->setValue (200);
     //qDebug() << Q_FUNC_INFO << " - 0";
    serialConfigWidget->createUI();
     //qDebug() << Q_FUNC_INFO << " - 1";
    networkConfigWidget->createUI();
    //qDebug() << Q_FUNC_INFO << " - 2";
    activateHamlibCheckBox->setText(tr("Activate HamLib"));
    activateHamlibCheckBox->setToolTip(tr("Activates the hamlib support that will enable the connection to a radio."));
    readOnlyModeCheckBox->setText(tr("Read-Only mode"));
    readOnlyModeCheckBox->setToolTip(tr("If enabled, the KLog will read Freq/Mode from the radio but will never send any command to the radio."));
    testHamlibPushButton->setText (tr("Test"));
    testHamlibPushButton->setToolTip (tr("Click to test the connection to the radio"));

    QFont freqFont("Courier");
    freqFont.setPointSize(14);
    freqFont.setBold(true);
    freqDisplayLabel->setFont(freqFont);
    freqDisplayLabel->setAlignment(Qt::AlignCenter);
    freqDisplayLabel->setToolTip(tr("Shows the frequency read from the radio while connected"));

    QString pollTip = QString(tr("Defines the interval to poll the radio in msecs."));

    pollIntervalQSpinBox->setToolTip(pollTip);
     //qDebug() << Q_FUNC_INFO << " - 13";
     //showDebugLog->setMinimum(pollMin);
     //qDebug() << Q_FUNC_INFO << " - 14";
     //showDebugLog->setMaximum(pollMax);
     //qDebug() << Q_FUNC_INFO << " - 15";
    QLabel *pollIntervalLabel = new QLabel(tr("Poll interval"));
    pollIntervalLabel->setBuddy(pollIntervalQSpinBox);
    pollIntervalLabel->setToolTip(pollTip);
    pollIntervalLabel->setAlignment(Qt::AlignCenter);
    pollIntervalLabel->setEnabled(true);

     //qDebug() << Q_FUNC_INFO << " - 24";

    QLabel *rigTypeLabel = new QLabel(tr("Radio"));
    rigTypeLabel->setBuddy(rigTypeComboBox);
    rigTypeLabel->setToolTip(tr("Select your rig."));
    rigTypeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rigTypeLabel->setEnabled(true);

    tabWidget->addTab (serialConfigWidget, tr("Serial"));
    tabWidget->addTab (networkConfigWidget, tr("Network"));

    // Row 0: Radio | combobox | Poll interval | spinbox | [Test]
    // Row 1:                  | Activate       | Read-Only | [freq/mode]
    // Row 2+: tabWidget (full width)
    QGridLayout *mLayout = new QGridLayout;
    mLayout->addWidget(rigTypeLabel,           0, 0);
    mLayout->addWidget(rigTypeComboBox,        0, 1);
    mLayout->addWidget(pollIntervalLabel,      0, 2);
    mLayout->addWidget(pollIntervalQSpinBox,   0, 3);
    mLayout->addWidget(testHamlibPushButton,   0, 4);

    mLayout->addWidget(activateHamlibCheckBox, 1, 2);
    mLayout->addWidget(readOnlyModeCheckBox,   1, 3);
    mLayout->addWidget(freqDisplayLabel,       1, 4);

    mLayout->addWidget(tabWidget,              2, 0, 1, -1);

     //qDebug() << Q_FUNC_INFO << " - 199";
    setLayout(mLayout);


    //connect(serialPortComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSerialPortChanged(QString) ));
    //connect(serialBaudsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSerialSpeedChanged (int) ));
    //connect(stopBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSerialStopBitChanged (int) ));
    //connect(dataBitsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSerialDataBitsChanged (int) ));
    //connect(flowControlComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSerialFlowControlChanged (QString)));
    //connect(parityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSerialParityChanged (QString)));


    connect(testHamlibPushButton, SIGNAL(clicked(bool)), this, SLOT(slotTestHamlib()) );
    connect(rigTypeComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(slotRadioComboBoxChanged(QString)) );
     //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageHamLib::setRig()
{
     //qDebug() << Q_FUNC_INFO;
    if (!m_liveHamlib) return;
    // getRigList() lazy-loads the rig database on first call (no initClass() needed).
    QStringList rigs = m_liveHamlib->getRigList();
     //qDebug() << Q_FUNC_INFO << " - rigs: " << QString::number(rigs.length())<< QT_ENDL;
    rigTypeComboBox->blockSignals(true);
    rigTypeComboBox->clear ();
    rigTypeComboBox->addItems (rigs);
    rigTypeComboBox->setCurrentIndex(0);
    rigTypeComboBox->blockSignals(false);
     //qDebug() << Q_FUNC_INFO << " - END";
}


void SetupPageHamLib::setDefaults()
{
     //qDebug() << Q_FUNC_INFO;
    rigctlport = 4532;
    networkRadio = false;

    rigTypeComboBox->setCurrentIndex(0);

     //showDebugLog->setValue(300);
    setTestResult(false);
     //qDebug() << Q_FUNC_INFO << " - END";
}


void SetupPageHamLib::showEvent(QShowEvent *event)
{
    if (!m_rigsLoaded) {
        m_rigsLoaded = true;
        setRig();        // calls hamlib->initClass() + fills combo from getRigList()
        loadSettings();  // re-apply saved settings now that combo is populated
    }
    // Reflect the live connection state each time the tab becomes visible.
    // Uses the read-only live pointer if available; does not touch the test hamlib.
    if (m_liveHamlib)
        setTestResult(m_liveHamlib->isRunning());
    QWidget::showEvent(event);
}

bool SetupPageHamLib::setRigType(const QString &_radio)
{
    if (!m_liveHamlib) return false;
    int _index = rigTypeComboBox->findText(m_liveHamlib->getNameFromModelId(_radio.toInt()), Qt::MatchFlag::MatchExactly);
       //qDebug() << "SetupPageHamLib::setRig: After: "  << QString::number(_index) ;
    if (_index >= 0)
    {
        rigTypeComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        rigTypeComboBox->setCurrentIndex(0);
    }
    return false;
}

int SetupPageHamLib::getDataBits() const
{
    return serialConfigWidget->getDataBits ();
}

QString SetupPageHamLib::getFlowControl() const
{
    return serialConfigWidget->getFlowControl ();
}

QString SetupPageHamLib::getParity() const
{
    return serialConfigWidget->getParity ();
}

void SetupPageHamLib::saveSettings()
{
     //qDebug() << Q_FUNC_INFO ;
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("HamLib");
    settings.setValue ("HamLibActive", QVariant((activateHamlibCheckBox->isChecked())));
    settings.setValue ("HamLibReadOnly", QVariant((readOnlyModeCheckBox->isChecked())));
    settings.setValue ("HamLibRigType", m_liveHamlib ? m_liveHamlib->getModelIdFromName(rigTypeComboBox->currentText()) : -1);
    settings.setValue ("HamLibRigPollRate", QString::number(pollIntervalQSpinBox->value ()));
    settings.setValue ("HamLibSerialPort", serialConfigWidget->getSerialPort ());
    settings.setValue ("HamLibSerialBauds", QString::number(serialConfigWidget->getSerialBauds ()));
    settings.setValue ("HamLibSerialDataBits", QString::number(getDataBits()));
    settings.setValue ("HamLibSerialStopBit", serialConfigWidget->getStopBits());
    settings.setValue ("HamLibSerialFlowControl", serialConfigWidget->getFlowControl ());
    settings.setValue ("HamLibSerialParity", serialConfigWidget->getParity ());
    settings.setValue ("HamLibNetAddress", networkConfigWidget->getAddress ());
    settings.setValue ("HamLibNetPort", networkConfigWidget->getPort ());
    settings.endGroup ();
}

void SetupPageHamLib::loadSettings()
{
    //qDebug() << Q_FUNC_INFO;
    testWasRun = false;
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("HamLib");
    // Block combo signals during load so slotRadioComboBoxChanged does not call
    // setTestResult(false) and clobber the connection state indicator.
    rigTypeComboBox->blockSignals(true);
    setRigType (settings.value("HamLibRigType").toString());
    rigTypeComboBox->blockSignals(false);
    pollIntervalQSpinBox->setValue(settings.value("HamLibRigPollRate", 2000).toInt ());
    serialConfigWidget->setSerialPort (settings.value("HamLibSerialPort").toString());
    serialConfigWidget->setSerialBauds (settings.value("HamLibSerialBauds", 9600).toInt ());
    serialConfigWidget->setDataBits (settings.value("HamLibSerialDataBits", 8).toInt ());
    serialConfigWidget->setStopBits(settings.value("HamLibSerialStopBit", "OneStop").toString());
    serialConfigWidget->setFlowControl (settings.value("HamLibSerialFlowControl", "None").toString());
    serialConfigWidget->setParity(settings.value("HamLibSerialParity", "Even").toString());
    networkConfigWidget->setAddress (settings.value("HamLibNetAddress", "localhost").toString());
    networkConfigWidget->setPort (settings.value("HamLibNetPort", 4532).toInt ());
    activateHamlibCheckBox->setChecked (settings.value("HamLibActive", false).toBool ());
    readOnlyModeCheckBox->setChecked (settings.value("HamLibReadOnly", false).toBool ());
    settings.endGroup ();

    snapshot.active      = activateHamlibCheckBox->isChecked();
    snapshot.readOnly    = readOnlyModeCheckBox->isChecked();
    snapshot.rigModelId  = m_liveHamlib ? m_liveHamlib->getModelIdFromName(rigTypeComboBox->currentText()) : -1;
    snapshot.pollRate    = pollIntervalQSpinBox->value();
    snapshot.serialPort  = serialConfigWidget->getSerialPort();
    snapshot.serialBauds = serialConfigWidget->getSerialBauds();
    snapshot.dataBits    = getDataBits();
    snapshot.stopBits    = serialConfigWidget->getStopBits();
    snapshot.flowControl = serialConfigWidget->getFlowControl();
    snapshot.parity      = serialConfigWidget->getParity();
    snapshot.netAddress  = networkConfigWidget->getAddress();
    snapshot.netPort     = networkConfigWidget->getPort();
 }

bool SetupPageHamLib::hasSettingsChanged() const
{
    if (snapshot.active      != activateHamlibCheckBox->isChecked())         return true;
    if (snapshot.readOnly    != readOnlyModeCheckBox->isChecked())            return true;
    if (snapshot.rigModelId  != (m_liveHamlib ? m_liveHamlib->getModelIdFromName(rigTypeComboBox->currentText()) : -1)) return true;
    if (snapshot.pollRate    != pollIntervalQSpinBox->value())                return true;
    if (snapshot.serialPort  != serialConfigWidget->getSerialPort())          return true;
    if (snapshot.serialBauds != serialConfigWidget->getSerialBauds())         return true;
    if (snapshot.dataBits    != getDataBits() )                               return true;
    if (snapshot.stopBits    != serialConfigWidget->getStopBits())            return true;
    if (snapshot.flowControl != serialConfigWidget->getFlowControl())         return true;
    if (snapshot.parity      != serialConfigWidget->getParity())              return true;
    if (snapshot.netAddress  != networkConfigWidget->getAddress())            return true;
    if (snapshot.netPort     != networkConfigWidget->getPort())               return true;
    return false;
}



