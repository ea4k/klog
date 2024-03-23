/***************************************************************************
                          setuppagemisc.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
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

//#include <QDebug>

#include "setuppagemisc.h"
//TODO: The defaultFileName QString can be removed and used the content of defaultFileNameLineEdit
//      instead. It is easy to change but the code would not be so clear... Think about this. :-)

SetupPageMisc::SetupPageMisc(QWidget *parent) : QWidget(parent){
       //qDebug() << "SetupPageMisc::SetupPageMisc";

    util = new Utilities(Q_FUNC_INFO);
    checkCallsCheckBox = new QCheckBox(tr("Check non-valid calls"), this);
    imperialCheckBox = new QCheckBox(tr("&Imperial system"), this);
    realTimeCheckbox = new QCheckBox(tr("&Log in real time"), this);
    showSecondsCheckBox = new QCheckBox(tr("Show seconds"), this);
    UTCCheckbox = new QCheckBox(tr("&Time in UTC"), this);
    alwaysADIFCheckBox = new QCheckBox(tr("&Save ADIF on exit"), this);
    useDefaultName = new QCheckBox(tr("Use this &default filename"), this);
    sendQSLWhenRecCheckBox = new QCheckBox(tr("Mark &QSO to send QSL when QSL is received"), this);
    completeWithPreviousCheckBox = new QCheckBox(tr("Complete QSO with previous data"));
    showStationCallWhenSearchCheckBox = new QCheckBox(tr("Show the Station &Callsign used in the search box"), this);
    //keepMyDataCheckBox = new QCheckBox(tr("&Reset to My Data for all QSOs"), this);
    checkNewVersionCheckBox = new QCheckBox(tr("&Check for new versions automatically"), this);
    //provideCallCheckBox = new QCheckBox(tr("&Provide Info for statistics"), this);
    useDxMarathonCheckBox = new QCheckBox(tr("Manage DX-Marathon"), this);

    //logSortCheckBox = new QCheckBox(tr("Sort log based in date && time"));
    sendEQSLByDefaultSearchCheckBox = new QCheckBox(tr("Mark sent eQSL && LoTW in new QSO as queued"));
    deleteAlwaysAdiFileCheckBox = new QCheckBox(tr("&Delete always temp ADIF file after uploading QSOs"));

    debugLogLevelCombo = new QComboBox;
    dupeTimeLineEdit = new QLineEdit;
    defaultFileNameLineEdit = new QLineEdit;
    dbPathLineEdit = new QLineEdit;

    fileNameButton = new QPushButton (tr("Browse"));
    dbPushButton = new QPushButton (tr("Browse"));
    moveDBPushButton = new QPushButton(tr("Move DB"));
    dbPathApplied = true;

    createUI();
    createActions();

    if (areDBPathChangesApplied())
    {
        moveDBPushButton->setEnabled(false);
    }
    else
    {
        moveDBPushButton->setEnabled(true);
    }
       //qDebug() << "SetupPageMisc::SetupPageMisc - END";
}

SetupPageMisc::~SetupPageMisc()
{
    delete(util);
       //qDebug() << "SetupPageMisc::~SetupPageMisc";
}

void SetupPageMisc::createUI()
{
    dupeTimeLineEdit->setInputMask("0000000");
    dupeTimeLineEdit->setToolTip(tr("In seconds, enter the time range to consider a duplicate if same call, band and mode is entered."));
    palWrong.setColor(QPalette::Text, Qt::red);
    palRight.setColor(QPalette::Text, Qt::black);

    //TODO: To be removed when the defaultDir is saved in the config file
    #ifdef Q_OS_WIN
       //qDebug() << "WINDOWS DETECTED!" ;
    klogDir = util->getHomeDir();  // We create the \klog for the logs and data
    defaultFileName = klogDir+"/klog.adi";
    #else
       //qDebug() << "NO WINDOWS DETECTED!" ;
    klogDir = util->getHomeDir();  // We create the ~/.klog for the logs and data
    defaultFileName = klogDir+"/klog.adi";
    #endif
    dbDirNew = klogDir;     // The new path where the DB is to be moved
    dbDirCurrent = dbDirNew;       // The path where the DB is hosted

    showSecondsCheckBox->setToolTip (tr("Show seconds in the QSO editor"));
    showSecondsCheckBox->setChecked (true);
    showSecondsCheckBox->setEnabled (true);

    defaultFileNameLineEdit->setReadOnly(false);
    defaultFileNameLineEdit->setText(defaultFileName);
    defaultFileNameLineEdit->setEnabled(false);

    dbPathLineEdit->setReadOnly(false);
    dbPathLineEdit->setText(dbDirCurrent);
    dbPathLineEdit->setEnabled(true);

    useDefaultName->setChecked(true);
    alwaysADIFCheckBox->setChecked(true);
    showStationCallWhenSearchCheckBox->setChecked(true);
    //keepMyDataCheckBox->setChecked(true);
    completeWithPreviousCheckBox->setChecked(false);
    debugLogLevelCombo->setCurrentIndex (0);
    UTCCheckbox->setChecked(true);
    realTimeCheckbox->setChecked(true);
    //logSortCheckBox->setChecked(false);
    defaultFileNameLineEdit->setEnabled(true);
    fileNameButton->setEnabled(true);
    dupeTimeLineEdit->setText("300");
    checkCallsCheckBox->setEnabled (true);
    checkCallsCheckBox->setChecked (true);
    checkCallsCheckBox->setToolTip (tr("If you disable this checkbox KLog will not check callsigns to identify wrong callsigns."));
    sendQSLWhenRecCheckBox->setToolTip(tr("QSOs will be marked as pending to send a QSL if you receive the DX QSL and have not sent yours."));
    showStationCallWhenSearchCheckBox->setToolTip(tr("The search box will also show the callsign on the air to do the QSO."));
    //keepMyDataCheckBox->setToolTip(tr("All the data from the My Data tab will be used or data from the previous QSO will be maintained."));
    checkNewVersionCheckBox->setToolTip(tr("Check if there is a new release of KLog available every time you start KLog."));
    //provideCallCheckBox->setToolTip(tr("If new version checking is selected, KLog will send the developer your callsign, KLog version and Operating system to help in improving KLog."));
    imperialCheckBox ->setToolTip(tr("Check it for Imperial system (Miles instead of Kilometers)."));
    realTimeCheckbox->setToolTip(tr("Select to use real time."));
    UTCCheckbox->setToolTip(tr("Select to use UTC time."));
    alwaysADIFCheckBox->setToolTip(tr("Select if you want to save to ADIF on exit."));
    useDefaultName->setToolTip(tr("Select to use the following name for the logfile without being asked for it again."));
    completeWithPreviousCheckBox->setToolTip(tr("Complete the current QSO with previous QSO data."));
    useDxMarathonCheckBox->setToolTip(tr("Select if you want to manage DX-Marathon."));
    defaultFileNameLineEdit->setToolTip(tr("This is the default file where ADIF data will be saved."));
    dbPathLineEdit->setToolTip(tr("This is the directory where the database (logbook.dat) will be saved."));
    fileNameButton->setToolTip(tr("Click to change the default ADIF file."));
    dbPushButton->setToolTip(tr("Click to change the path of the database."));
    moveDBPushButton->setToolTip(tr("Click to move the DB to the new directory."));
    debugLogLevelCombo->setToolTip(tr("Select the application debug log level. This may be useful if something is not working as expected. A debug file will be created in the KLog directory and/or shown with Help->Debug menu."));
    //logSortCheckBox->setToolTip(tr("Click to enable that log will be sorted based on date and time when clicking on the date column. It may be a little bit slower."));
    sendEQSLByDefaultSearchCheckBox->setToolTip(tr("Click to mark as Queued (to be sent) all the eQSL (LoTW and eQSL) in all the new QSO by default."));
    deleteAlwaysAdiFileCheckBox->setToolTip(tr("Delete Always the adif file created after uploading QSOs"));

    fillDebugComboBox();

    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->addWidget(useDefaultName);
    fileLayout->addWidget(defaultFileNameLineEdit);
    fileLayout->addWidget(fileNameButton);

    QHBoxLayout *dbLayout = new QHBoxLayout;
    dbLayout->addWidget(dbPathLineEdit);
    dbLayout->addWidget(dbPushButton);
    dbLayout->addWidget(moveDBPushButton);

    QLabel *logLevelLabel = new QLabel;
    logLevelLabel->setText(tr("Log level"));
    QHBoxLayout *logLevelLayout = new QHBoxLayout;
    logLevelLayout->addWidget(logLevelLabel);
    logLevelLayout->addWidget(debugLogLevelCombo);

    QLabel *timeRangeLabel = new QLabel;
    timeRangeLabel->setText(tr("Dupe time range:"));

    QHBoxLayout *timeRangeLayout = new QHBoxLayout;
    timeRangeLayout->addWidget(timeRangeLabel);
    timeRangeLayout->addWidget(dupeTimeLineEdit);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget (realTimeCheckbox);
    timeLayout->addWidget (showSecondsCheckBox);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(fileLayout, 0, 0, 1, -1);
    mainLayout->addLayout(dbLayout, 1, 0, 1, -1);
    mainLayout->addWidget(alwaysADIFCheckBox, 2, 0, 1, 1);
    mainLayout->addLayout(logLevelLayout, 2, 1, 1, 1);
    mainLayout->addWidget(UTCCheckbox, 3, 0, 1, 1);
    //mainLayout->addWidget(realTimeCheckbox, 3, 1, 1, 1);
    mainLayout->addLayout(timeLayout, 3, 1, 1, 1);
    mainLayout->addWidget(imperialCheckBox, 4, 0, 1, 1);
    mainLayout->addWidget(useDxMarathonCheckBox, 4, 1, 1, 1);
    mainLayout->addLayout(timeRangeLayout, 5, 0, 1, 1);
    mainLayout->addWidget(completeWithPreviousCheckBox, 5, 1, 1, 1);
    mainLayout->addWidget(sendQSLWhenRecCheckBox,6, 0, 1, 1);
    mainLayout->addWidget(sendEQSLByDefaultSearchCheckBox, 6, 1, 1, 1);
    mainLayout->addWidget(checkNewVersionCheckBox, 7, 0, 1, 1);
    //mainLayout->addWidget(provideCallCheckBox, 7, 1, 1, 1);
    //mainLayout->addWidget(logSortCheckBox, 8, 0, 1, 1);
    mainLayout->addWidget(showStationCallWhenSearchCheckBox, 8, 0, 1, 1);
    mainLayout->addWidget(deleteAlwaysAdiFileCheckBox, 8, 1, 1, 1);
    mainLayout->addWidget (checkCallsCheckBox, 9, 0, 1, 1);

    setLayout(mainLayout);
}

void SetupPageMisc::fillDebugComboBox()
{
    debugLogLevelCombo->addItems(util->getDebugLevels());
    debugLogLevelCombo->setCurrentIndex(0);
}

void SetupPageMisc::createActions()
{
//void	itemDoubleClicked ( QListWidgetItem * item )
    connect(fileNameButton, SIGNAL(clicked() ), this, SLOT(slotOpenFileButtonClicked() ) );
    connect(useDefaultName, SIGNAL(stateChanged(int) ), this, SLOT(slotUseDefaultButtonStateChanged(int) ) );
    connect(defaultFileNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotDefaultFileNameLineEditChanged() ) );
    connect(checkNewVersionCheckBox, SIGNAL(clicked() ), this, SLOT(slotcheckNewVersionCheckBoxClicked() ) );
    connect(dbPushButton, SIGNAL(clicked() ), this, SLOT(slotDBButtonClicked() ) );
    connect(dbPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotDBLineEditChanged() ) );
    connect(moveDBPushButton, SIGNAL(clicked() ), this, SLOT(slotMoveDBButtonClicked() ) );
    //connect(UDPServerCheckBox, SIGNAL(clicked () ), this, SLOT(slotUDPServerCheckBoxClicked() ) );
}

void SetupPageMisc::setDeleteAlwaysAdiFile(const bool &_t)
{
    deleteAlwaysAdiFileCheckBox->setChecked(_t);
    //qDebug() << "SetupPageMisc::setDeleteAlwaysAdiFile - DELETEALWAYSADIFILE = " << _t;
}

QString SetupPageMisc::getDeleteAlwaysAdiFile()
{
    return util->boolToQString(deleteAlwaysAdiFileCheckBox->isChecked());
}

QString SetupPageMisc::getSendEQSLByDefault()
{
    return util->boolToQString(sendEQSLByDefaultSearchCheckBox->isChecked());
}

void SetupPageMisc::setSetEQSLByDefault(const QString &_t)
{
    sendEQSLByDefaultSearchCheckBox->setChecked(util->trueOrFalse(_t));
}

QString SetupPageMisc::getRealTime()
{
    return util->boolToQString(realTimeCheckbox->isChecked());
}

void SetupPageMisc::setRealTime(const QString &_t)
{
    realTimeCheckbox->setChecked(util->trueOrFalse(_t));
}

bool SetupPageMisc::getShowSeconds()
{
    return showSecondsCheckBox->isChecked ();
}

void SetupPageMisc::setShowSeconds(const bool &_t)
{
    showSecondsCheckBox->setChecked (_t);
}

QString SetupPageMisc::getUTCTime()
{
    return util->boolToQString(UTCCheckbox->isChecked());
}


void SetupPageMisc::setUTCTime(const QString &_t)
{
    UTCCheckbox->setChecked(util->trueOrFalse(_t));
}


QString SetupPageMisc::getAlwaysADIF()
{
    return util->boolToQString(alwaysADIFCheckBox->isChecked());
}
void SetupPageMisc::setAlwaysADIF(const QString &_t)
{ // Defaul value is false
    alwaysADIFCheckBox->setChecked(util->trueOrFalse(_t));
}

QString SetupPageMisc::getDefaultFileName()
{
    return defaultFileName;
}
void SetupPageMisc::setDefaultFileName(const QString &_t)
{
       //qDebug() << "SetupPageMisc::setDefaultFileName: " << _t;
    if (_t.length ()<1)
    {
        defaultFileName = util->getDebugLogFile ();
    }
    else
    {
        defaultFileName = _t;
    }
    defaultFileNameLineEdit->setText(defaultFileName);
}

void SetupPageMisc::slotOpenFileButtonClicked()
{
    defaultFileName = QFileDialog::getOpenFileName(this, tr("Open File"), klogDir, "ADIF (*.adi)");
    defaultFileNameLineEdit->setText(defaultFileName);
}

 void SetupPageMisc::slotDefaultFileNameLineEditChanged()
 {
     //setDefaultFileName();
     defaultFileName = defaultFileNameLineEdit->text();
 }


QString SetupPageMisc::getUseDefaultName()
{
    return util->boolToQString(useDefaultName->isChecked());
}

void SetupPageMisc::setUseDefaultName(const QString &_t)
{
    useDefaultName->setChecked(util->trueOrFalse(_t));
}

QString SetupPageMisc::getImperial()
{
    return util->boolToQString(imperialCheckBox->isChecked());
}

void SetupPageMisc::setImperial(const QString &_t)
{
    imperialCheckBox->setChecked(util->trueOrFalse(_t));
}

void SetupPageMisc::slotUseDefaultButtonStateChanged(int state)
{
       //qDebug() << "SetupPageMisc::slotUseDefaultButtonStateChanged";

    if (state)
    {
        defaultFileNameLineEdit->setEnabled(true);
        moveDBPushButton->setEnabled(true);
    }
    else
    {
        defaultFileNameLineEdit->setEnabled(false);
        moveDBPushButton->setEnabled(false);
    }
}

QString SetupPageMisc::getSendQSLWhenRec()
{
    return util->boolToQString(sendQSLWhenRecCheckBox->isChecked());
}


void SetupPageMisc::setSendQSLWhenRec(const QString &_t)
{
    sendQSLWhenRecCheckBox->setChecked(util->trueOrFalse(_t));
}

QString SetupPageMisc::getShowStationCallSignInSearch()
{
    return util->boolToQString(showStationCallWhenSearchCheckBox->isChecked());
}

void SetupPageMisc::setShowStationCallSignInSearch(const QString &_t)
{
    showStationCallWhenSearchCheckBox->setChecked(util->trueOrFalse(_t));
}

/*
QString SetupPageMisc::getKeepMyData()
{
    return util->boolToQString(keepMyDataCheckBox->isChecked());
}

void SetupPageMisc::setKeepMyData(const QString &_t)
{
    keepMyDataCheckBox->setChecked(util->trueOrFalse(_t));
}
*/

QString SetupPageMisc::getCompleteWithPrevious()
{
    return util->boolToQString(completeWithPreviousCheckBox->isChecked());
}

void SetupPageMisc::setCompleteWithPrevious(const QString &_t)
{
    completeWithPreviousCheckBox->setChecked(util->trueOrFalse(_t));
}



void SetupPageMisc::slotcheckNewVersionCheckBoxClicked()
{
    if (checkNewVersionCheckBox->isChecked())
    {
        //provideCallCheckBox->setEnabled(true);
    }
    else
    {
        //provideCallCheckBox->setEnabled(false);
        //provideCallCheckBox->setChecked(false);
    }
}

QString SetupPageMisc::getCheckNewVersions()
{
    return util->boolToQString(checkNewVersionCheckBox->isChecked());
}

void SetupPageMisc::setCheckNewVersions(const QString &_t)
{
    checkNewVersionCheckBox->setChecked(util->trueOrFalse(_t));
}

QString SetupPageMisc::getDefaultDBPath()
{
    return dbDirCurrent;
}


void SetupPageMisc::setUseDefaultDBPath(const QString &_t)
{
    if (_t.isEmpty ())
    {
        dbDirCurrent = util->getHomeDir();
    }
    else
    {
        dbDirCurrent = _t;
    }
    dbPathLineEdit->setText(dbDirCurrent);
}


QString SetupPageMisc::getDXMarathon(){
    return util->boolToQString(useDxMarathonCheckBox->isChecked());
}

void SetupPageMisc::setDXMarathon(const QString &_t){
    //QString st = t;
    useDxMarathonCheckBox->setChecked(util->trueOrFalse(_t));
}

QString SetupPageMisc::getDebugLogLevel()
{
    return debugLogLevelCombo->currentText ();
}

void SetupPageMisc::setDebugLogLevel(const QString &_t)
{
    if (util->isValidLogLevel(_t))
        debugLogLevelCombo->setCurrentIndex(debugLogLevelCombo->findText(_t, Qt::MatchCaseSensitive));
    else
        debugLogLevelCombo->setCurrentIndex(0);
}

void SetupPageMisc::slotDBButtonClicked()
{
//    QString dbDirBack = dbDir;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    dbDirCurrent,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir.length()>=1)
    {
        dbDirNew = dir;
    }

    dbPathLineEdit->setText(dbDirNew);
    if (dbDirCurrent == dbDirNew)
    {
        dbPathApplied = true;
    }
    else
    {
        dbPathApplied = false;
        moveDBPushButton->setEnabled(true);
    }
    //setUseDefaultDBPath(dbDirNew);
}

void SetupPageMisc::slotDBLineEditChanged()
{
       //qDebug() << "SetupPageMisc::slotDBLineEditChanged: " << dbPathLineEdit->text();
    QString aux;
    aux = dbPathLineEdit->text();

   if ( QFile::exists(aux) )
   {
       dbPathLineEdit->setToolTip(tr("This is the directory where DB (logbook.dat) will be saved."));
       dbPathLineEdit->setPalette(palRight);
       dbDirNew = dbPathLineEdit->text();
   }
   else
   {
        dbPathLineEdit->setToolTip(tr("Please specify an existing directory where the database (logbook.dat) will be saved."));
        dbPathLineEdit->setPalette(palWrong);
   }

   if (dbDirCurrent == dbDirNew)
   {
       dbPathApplied = true;
       moveDBPushButton->setEnabled(false);
   }
   else
   {
       dbPathApplied = false;
       moveDBPushButton->setEnabled(true);
   }
}

void SetupPageMisc::slotMoveDBButtonClicked()
{
    QString source = dbDirCurrent + "/logbook.dat";
    QString target = dbDirNew + "/logbook.dat";
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - Move DB"));

       //qDebug() << "SetupPageMisc::slotMoveDBButtonClicked (source): " << source;
       //qDebug() << "SetupPageMisc::slotMoveDBButtonClicked (target): " << target;
    if ( QFile::exists(dbDirNew) )
    {
        if (QFile::exists(target))
        {
            //qDebug() << "SetupPageMisc::slotMoveDBButtonClicked (target EXISTS): " << target;
        }
        if (QFile::copy(source, target))
        {
            dbDirCurrent = dbDirNew;
            if (QFile::remove(source))
            {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("File moved"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                dbPathApplied = true;
            }
            else
            {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("File copied"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                dbPathApplied = true;
            }
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);

            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);

            if (QFile::exists(target))
            {
                //qDebug() << "SetupPageMisc::slotMoveDBButtonClicked (target EXISTS): " << target;

                msgBox.setText(tr("File already exist."));
                msgBox.setDetailedText(tr("The destination file already exist and KLog will not replace it. Please remove the file from the destination folder before moving the file with KLog to make sure KLog can copy the file."));
                dbPathApplied = true;
            }
            else
            {
                msgBox.setText(tr("File NOT copied"));
                msgBox.setDetailedText(tr("The file was not copied due to an unknown problem."));
                dbPathApplied = false;
            }
            msgBox.exec();

            moveDBPushButton->setEnabled(true);
        }
    }
    else
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("The target directory does not exist. Please select an existing directory."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        dbPathApplied = false;
        moveDBPushButton->setEnabled(true);
    }
}

bool SetupPageMisc::areDBPathChangesApplied()
{
    return dbPathApplied;
}

void SetupPageMisc::setDupeTime(const int _t)
{
    dupeTimeLineEdit->setText(QString::number(_t));
}

int SetupPageMisc::getDupeTime()
{
    return dupeTimeLineEdit->text().toInt();
}

bool SetupPageMisc::getCheckCalls()
{
    return checkCallsCheckBox->isChecked ();
}

void SetupPageMisc::setCheckCalls(const bool &_t)
{
    checkCallsCheckBox->setChecked (_t);
}

void SetupPageMisc::saveSettings()
{
    //qDebug() << Q_FUNC_INFO ;
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("Misc");
    settings.setValue ("RealTime", QVariant((realTimeCheckbox->isChecked())));
    settings.setValue ("ShowSeconds", QVariant((showSecondsCheckBox->isChecked())));
    settings.setValue ("UTCTime", QVariant((UTCCheckbox->isChecked())));
    settings.setValue ("AlwaysADIF", QVariant((alwaysADIFCheckBox->isChecked())));
    settings.setValue ("UseDefaultName", QVariant((useDefaultName->isChecked())));
    settings.setValue ("DefaultADIFFile", defaultFileName);
    settings.setValue ("DBPath", dbDirCurrent);
    settings.setValue ("ImperialSystem", QVariant((imperialCheckBox->isChecked())));
    settings.setValue ("SendQSLWhenRec", QVariant((sendQSLWhenRecCheckBox->isChecked())));
    settings.setValue ("ShowCallsignInSearch", QVariant((showStationCallWhenSearchCheckBox->isChecked())));
    settings.setValue ("CompleteWithPrevious", QVariant((completeWithPreviousCheckBox->isChecked())));
    settings.setValue ("CheckNewVersions", QVariant((checkNewVersionCheckBox->isChecked())));
    settings.setValue ("ManageDXMarathon", QVariant((useDxMarathonCheckBox->isChecked())));
    settings.setValue ("DebugLog", debugLogLevelCombo->currentText ());
    settings.setValue ("SendEQSLByDefault", QVariant((sendEQSLByDefaultSearchCheckBox->isChecked())));
    settings.setValue ("DeleteAlwaysAdiFile", QVariant((deleteAlwaysAdiFileCheckBox->isChecked())));
    settings.setValue ("CheckValidCalls", QVariant((checkCallsCheckBox->isChecked())));
    settings.setValue ("DuplicatedQSOSlot", dupeTimeLineEdit->text());
    //settings.setValue ("ProvideInfo", QVariant((provideCallCheckBox->isChecked())));

    //stream << "PSTRotatorActive=" << interfacesWindowsPage->getSendToPSTRotator() << ";";
    //stream << "PSTRotatorServer=" << interfacesWindowsPage->getPSTRotatorUDPServer() << ";";
    //stream << "PSTRotatorPort=" << interfacesWindowsPage->getPSTRotatorUDPServerPort() << ";";
    settings.endGroup ();
}

void SetupPageMisc::loadSettings()
{
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("Misc");
    realTimeCheckbox->setChecked (settings.value("RealTime").toBool ());
    showSecondsCheckBox->setChecked (settings.value("ShowSeconds").toBool ());
    UTCCheckbox->setChecked (settings.value("UTCTime").toBool ());
    alwaysADIFCheckBox->setChecked (settings.value("AlwaysADIF").toBool ());
    useDefaultName->setChecked (settings.value("UseDefaultName").toBool ());
    imperialCheckBox->setChecked (settings.value("ImperialSystem").toBool ());
    sendQSLWhenRecCheckBox->setChecked (settings.value("SendQSLWhenRec").toBool ());
    showStationCallWhenSearchCheckBox->setChecked (settings.value("ShowCallsignInSearch").toBool ());
    completeWithPreviousCheckBox->setChecked (settings.value("CompleteWithPrevious").toBool ());
    checkNewVersionCheckBox->setChecked (settings.value("CheckNewVersions").toBool ());
    useDxMarathonCheckBox->setChecked (settings.value("ManageDXMarathon").toBool ());
    sendEQSLByDefaultSearchCheckBox->setChecked (settings.value("SendEQSLByDefault").toBool ());
    deleteAlwaysAdiFileCheckBox->setChecked (settings.value("DeleteAlwaysAdiFile").toBool ());
    checkCallsCheckBox->setChecked (settings.value("CheckValidCalls").toBool ());
    //provideCallCheckBox->setChecked (settings.value("ProvideInfo").toBool ());

    setDefaultFileName(settings.value("DefaultADIFFile").toString ());
    setUseDefaultDBPath(settings.value("DBPath").toString ());
    setDebugLogLevel(settings.value("DebugLog").toString ());
    dupeTimeLineEdit->setText (settings.value("DuplicatedQSOSlot").toString ());
    settings.endGroup ();
}
