/***************************************************************************
                          mainwindow.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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

//#include "global.h"
#include <QtWidgets>
#include <QtSql>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "callsign.h"
#include "updatesettings.h"
//#include "database.h"
#include "mainwindow.h"


MainWindow::MainWindow(DataProxy_SQLite *dp):
   // dataProxy(Q_FUNC_INFO, tversion),
    awards(dp, Q_FUNC_INFO)     // Pass Awards instance to DXCCStatusWidget
{
    dataProxy = dp;
    softwareVersion = dataProxy->getSoftVersion();
    //qDebug() << Q_FUNC_INFO << ": " <<  " Ver: " << softwareVersion  << QTime::currentTime().toString("hh:mm:ss") ;
    //logEvent(Q_FUNC_INFO, "Start: " + _klogDir  + "/" + tversion, Debug);
    dxccStatusWidget = std::make_unique<DXCCStatusWidget>(&awards, this);
    dxClusterWidget = std::make_unique<DXClusterWidget>(&awards, this);
    searchWidget = std::make_unique<SearchWidget>(&awards, this);
    logWindow = std::make_unique<LogWindow>(&awards, this);

    showKLogLogWidget = new ShowKLogLogWidget;
    showErrorDialog = new ShowErrorDialog();
    UDPLogServer = new UDPServer();
    util = new Utilities(Q_FUNC_INFO);
    //util->setVersion(softwareVersion);
    //qDebug() << Q_FUNC_INFO << " - Creating qso - ";
    //qso = new QSO;
    //QThread::sleep(std::chrono::microseconds{1000});
    //qDebug() << Q_FUNC_INFO << " - Creating backupQSO - ";
    backupQSO = new QSO;
    //QThread::sleep(std::chrono::microseconds{1000});
    //qDebug() << Q_FUNC_INFO << " - Creating modifyingQSO - ";
    //modifyingQSO = new QSO;



    logLevel = Info;
    dupeSlotInSeconds = 15;
    needToEnd = false;
    upAndRunning = false; // To define some actions that can only be run when starting the software

    QRZCOMAutoCheckAct = new QAction(tr("Always check the current callsign in QRZ.com"), this);

       //qDebug() << "MainWindow::MainWindow: Debug File: "<<  util->getDebugLogFile() ;
    //dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, softwareVersion);
    world = new World(dataProxy, Q_FUNC_INFO);
    world->create(util->getCTYFile());
      //qDebug() << Q_FUNC_INFO << ": BEFORE HAMLIB " << QTime::currentTime().toString("hh:mm:ss") ;
    hamlib = new HamLibClass();
      //qDebug() << Q_FUNC_INFO << ": AFTER HAMLIB " << QTime::currentTime().toString("hh:mm:ss") ;


      //qDebug() << Q_FUNC_INFO << ": AFTER dataproxy ";
    lotwUtilities = new LoTWUtilities(util->getHomeDir (), softwareVersion, Q_FUNC_INFO, dataProxy);
      //qDebug() << Q_FUNC_INFO << ": AFTER lotwUtilities";
    eqslUtilities = new eQSLUtilities(Q_FUNC_INFO);
      //qDebug() << Q_FUNC_INFO << ": AFTER eQSLUtilities";
    mapWindow = new MapWindowWidget(dataProxy, this);

      //qDebug() << Q_FUNC_INFO << ": Before DXCCStatusWidget " << QTime::currentTime().toString("hh:mm:ss") ;
    //dxccStatusWidget = new DXCCStatusWidget(dataProxy, Q_FUNC_INFO);
      //qDebug() << Q_FUNC_INFO << ": After DXCCStatusWidget " << QTime::currentTime().toString("hh:mm:ss") ;
    elogClublog = new eLogClubLog();
      //qDebug() << Q_FUNC_INFO << ": 00082: " << QTime::currentTime().toString("hh:mm:ss") ;

    elogQRZcom = new eLogQrzLog(dataProxy, Q_FUNC_INFO, softwareVersion);

     //qDebug() << Q_FUNC_INFO << ": 00083: " << QTime::currentTime().toString("hh:mm:ss") ;
    updateSatsData = new UpdateSatsData(dataProxy);
     //qDebug() << Q_FUNC_INFO << ": 00084: " << QTime::currentTime().toString("hh:mm:ss") ;
    statsWidget = new StatisticsWidget(dataProxy);
     //qDebug() << Q_FUNC_INFO << ": 00085: " << QTime::currentTime().toString("hh:mm:ss") ;
    infoLabel1 = new QLabel(tr("Status bar ..."));
    infoLabel2 = new QLabel(tr("DX Entity"));

     //qDebug() << "MainWindow::MainWindow: 00086" << QTime::currentTime().toString("hh:mm:ss") ;
    //logWindow = new LogWindow(dataProxy, this);
      //qDebug() << Q_FUNC_INFO << ": 00087: " << QTime::currentTime().toString("hh:mm:ss") ;

    //searchWidget = new SearchWidget(dataProxy, this);
      //qDebug() << Q_FUNC_INFO << ": 00087.1: " << QTime::currentTime().toString("hh:mm:ss") ;
    //advancedSearchWidget = new AdvancedSearchWidget(dataProxy, this);
      //qDebug() << "MainWindow::MainWindow: 00087.2" << QTime::currentTime().toString("hh:mm:ss") ;
    infoWidget = new InfoWidget(dataProxy, this);

     //qDebug() << Q_FUNC_INFO << ": 00088: " << QTime::currentTime().toString("hh:mm:ss") ;
    awardsWidget = new AwardsWidget(dataProxy, this);

      //qDebug() << Q_FUNC_INFO << ": 0009: " << QTime::currentTime().toString("hh:mm:ss") ;

    aboutDialog = new AboutDialog(softwareVersion);
    tipsDialog = new TipsDialog();

    downloadcty = new DownLoadCTY(util->getHomeDir (), softwareVersion);

    statusBarMessage = tr("Starting KLog");

     //qDebug() << Q_FUNC_INFO << ": 40: " << QTime::currentTime().toString("hh:mm:ss") ;


       //qDebug() << Q_FUNC_INFO << ": 50: " << QTime::currentTime().toString("hh:mm:ss") ;

     //qDebug() << Q_FUNC_INFO << ": 51: " << QTime::currentTime().toString("hh:mm:ss") ;
    setupDialog = new SetupDialog(dataProxy, this);

     //qDebug() << Q_FUNC_INFO << ": satTabWidget to be created " ;
    satTabWidget = new MainWindowSatTab(dataProxy);
    //qDebug() << Q_FUNC_INFO << ": 52: " << QTime::currentTime().toString("hh:mm:ss") ;
    QSOTabWidget = new MainWindowInputQSO(dataProxy);
     //qDebug() << Q_FUNC_INFO << ": 53: " << QTime::currentTime().toString("hh:mm:ss") ;
    myDataTabWidget = new MainWindowMyDataTab(dataProxy);
      //qDebug() << Q_FUNC_INFO << ": 54: " << QTime::currentTime().toString("hh:mm:ss") ;
    commentTabWidget = new MainWindowInputComment();
      //qDebug() << Q_FUNC_INFO << ": 55: " << QTime::currentTime().toString("hh:mm:ss") ;
    othersTabWidget = new MainWindowInputOthers(dataProxy);
      //qDebug() << Q_FUNC_INFO << ": 56: " << QTime::currentTime().toString("hh:mm:ss") ;
    eQSLTabWidget = new MainWindowInputEQSL(dataProxy);
      //qDebug() << Q_FUNC_INFO << ": 57: " << QTime::currentTime().toString("hh:mm:ss") ;
    QSLTabWidget = new MainWindowInputQSL(dataProxy);
     //qDebug() << Q_FUNC_INFO << ": 58: " << QTime::currentTime().toString("hh:mm:ss") ;
    mainQSOEntryWidget = new MainQSOEntryWidget(dataProxy);

     //qDebug() << Q_FUNC_INFO << ": locator to be created 59" << QTime::currentTime().toString("hh:mm:ss") ;
    //locator = new Locator();

    mainWidget = new QWidget(this);
     //qDebug() << Q_FUNC_INFO << ": 60 " << QTime::currentTime().toString("hh:mm:ss") ;

    dateTime = std::make_unique<QDateTime>();
    // UI DX
    infoLabel2 = new QLabel(tr("DX Entity"));
    loggWinAct = new QAction(tr("&Log Window"), this);


    //dxClusterAssistant = new DXClusterAssistant(Q_FUNC_INFO);

     //qDebug() << Q_FUNC_INFO << ": Awards to be created " << QTime::currentTime().toString("hh:mm:ss") ;
    //awards = new Awards(dataProxy, Q_FUNC_INFO);
        //qDebug() << Q_FUNC_INFO << ": Awards created " << QTime::currentTime().toString("hh:mm:ss") ;
    // </UI>

    if (needToEnd)
    {
       exit(0);
    }

     //qDebug() << Q_FUNC_INFO << ": Software update to be created " << QTime::currentTime().toString("hh:mm:ss") ;
    softUpdate = new SoftwareUpdate(softwareVersion);
        //qDebug() << Q_FUNC_INFO << ": FileManager to be created " << QTime::currentTime().toString("hh:mm:ss") ;

    filemanager = new FileManager(dataProxy);

        //qDebug() << Q_FUNC_INFO << ": FileAwardManager to be created " << QTime::currentTime().toString("hh:mm:ss") ;
    //fileAwardManager = new FileAwardManager(dataProxy, Q_FUNC_INFO);

    lotwCallTQSL = new QAction(tr("Upload queued QSOs to LoTW"), this);
        //qDebug() << Q_FUNC_INFO << ": AdifLoTWExportWidget to be created " << QTime::currentTime().toString("hh:mm:ss") ;
    adifLoTWExportWidget = new AdifLoTWExportWidget(dataProxy, Q_FUNC_INFO);
       //qDebug() << Q_FUNC_INFO << ": ShowAdifImportWidget to be created " << QTime::currentTime().toString("hh:mm:ss") ;
    showAdifImportWidget = new ShowAdifImportWidget(dataProxy, Q_FUNC_INFO);

    logEvent(Q_FUNC_INFO, "END", Debug);
   //qDebug() << Q_FUNC_INFO << ": END " << QTime::currentTime().toString("hh:mm:ss") ;
}

MainWindow::~MainWindow()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (hamlibActive)
    {
        hamlib->stop();
    }

    delete(showErrorDialog);
    delete(lotwUtilities);
    delete(eqslUtilities);
    delete(elogQRZcom);
    //delete(elogClublog);
    delete(downloadcty);
    delete(world);
    delete(mapWindow);
    //delete(locator);
    //delete(qso);
    delete(backupQSO);
    //delete(modifyingQSO);
    //dateTime.reset();
    //delete(dateTime);
    //delete(dateTimeTemp);
    //delete(awards);
    delete(softUpdate);
    delete(filemanager);
    //delete(fileAwardManager);
    delete(util);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::saveWindowsSize()
{
     //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    windowSize = this->size();

    //int height = windowSize.height();
    //int width = windowSize.width();

    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.setValue ("MainWindowSize", windowSize);

    logEvent(Q_FUNC_INFO, "END", Debug);
        //qDebug() << "MainWindow::windowsSizeAndPosition: END" ;
}

void MainWindow::setWindowSize(const QSize &_size)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //QSize size;
    //size.setHeight(_height);
    //size.setWidth(_width);
    if (_size.isValid ())
    {
        this->resize(_size);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::init_variables()
{
     //qDebug() << Q_FUNC_INFO << " - Start";
    QRZCOMAutoCheckAct->setCheckable(true);
    QRZCOMAutoCheckAct->setChecked(false);
    manualMode = false;
    qrzAutoChanging = false;
    logEvents = true;
    //Default band/modes
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW";
        //qDebug() << Q_FUNC_INFO << " -  00" << (QTime::currentTime()).toString("HH:mm:ss") ;

    hamlibActive = false;
    hamlibModeNotADIFSupported = false;
    hamlibChangingMode = false;
    yearChangedDuringModification = false;

    readingTheUI = false;
    itIsANewversion = false;
    infoLabel1T = QString();
    infoLabel2T = QString();
    infoTimeout = 2000; // default timeout

    defaultADIFLogFile = "klog.adi";

    InValidCharsInPrevCall = false;

    checkNewVersions = true;
    reportInfo = false;
    configured = false;

    noMoreErrorShown = false;
    noMoreModeErrorShown = false;
    qslingNeeded = false; // When clicking on Find QSO to QSL
    manageMode = false;

    txFreqBeingAutoChanged = false;
    //rxFreqBeingChanged = false;
    updatingBands = false;
    selectedYear = 0;
    defaultMode = 1;
    defaultBand = 1;
        //qDebug() << Q_FUNC_INFO << " -  20" << (QTime::currentTime()).toString("HH:mm:ss") ;

    currentMode = 1;
    currentModeShown = currentMode;
    currentBand = 1;
    currentBandShown = currentBand;
    currentLog = 1;

    operatorQRZ = "";
    stationCallsign = "";
    mainQRZ = "";
    dxLocator ="";

    UDPServerStart = false;   // By default the UDP server is started

        //qDebug() << Q_FUNC_INFO << " -  30" << (QTime::currentTime()).toString("HH:mm:ss") ;

    currentEntity = -1; // To optimize the calls to different world methods if the entity does not change. Used in slotQRZTextChanged
    previousEntity = -1;// To optimize the calls to different world methods if the entity does not change.

    completeWithPrevious=false;
    completedWithPreviousQTH=false;
    completedWithPreviousLocator=false;
    completedWithPreviousName=false;
    completedWithPreviousIOTA=false;
    completedWithPreviousQSLVia=false;

    alwaysADIF=false;
    useDefaultLogFileName=false;
    needToSave=false;
    qrzSmallModDontCalculate=false;
    imperialSystem=false;
    sendQSLWhenRec = true;
    manageDxMarathon = false;

    dxClusterShowHF=true;
    dxClusterShowVHF=true;
    dxClusterShowWARC=true;
    dxClusterShowWorked=true;
    dxClusterShowConfirmed=true;
    dxClusterShowAnn=true;
    dxClusterShowWWV=true;
    dxClusterShowWCY=true;
    dxclusterSendSpotsToMap = false;

    keepSatPage = false;
        //qDebug() << Q_FUNC_INFO << " -  40" << (QTime::currentTime()).toString("HH:mm:ss") ;
    clublogActive = false;
    clublogRealTime = false;

    eQSLActive = false;
    qrzcomActive = false;
    lotwActive = false;
    qrzcomSubscriber = false;

    callingUpdate = false; // to control whether the update is mannually launched or at the begining
    selectedYear = (dateTime->currentDateTime()).date().year();
    loggWinAct->setShortcut(Qt::CTRL | Qt::Key_L);

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    clublogAnswer = -1;
   //qDebug() << Q_FUNC_INFO << " - Changing colors to default";
       defaultColor.fromString(QAnyStringView("slategrey")); //To be replaced by .fromString in Qt6.6
       neededColor.fromString(QAnyStringView("yellow"));
       workedColor.fromString(QAnyStringView("blue"));
       confirmedColor.fromString(QAnyStringView("red"));
       newOneColor.fromString(QAnyStringView("green"));

   //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::checkDebugFile()
{
    QFile debugFile(util->getDebugLogFile());
    if (!debugFile.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - File not open"));
        QString aux = tr("It was not possible to open the debug file for writing. No debug log will be saved!");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else
    {
        debugFile.close();
        logEvent(Q_FUNC_INFO, "KLog started!", Debug);
    }
}

void MainWindow::checkHomeDir()
{
     if (!QDir::setCurrent ( util->getHomeDir () )){
        QDir d1(util->getHomeDir ());
        if (d1.mkdir(util->getHomeDir ()))
        {
            if (!QDir::setCurrent ( util->getHomeDir () ))
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setWindowTitle(tr("KLog - KLog folder not found"));
                QString aux = tr("It was not possible to define the KLog folder. Some functions may not work properly!");
                msgBox.setText(aux);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
            }
        }
    }
}

void MainWindow::init()
{
    //qDebug() << Q_FUNC_INFO << " - Start - " << (QTime::currentTime()).toString("HH:mm:ss") ;
    logLevel = Debug;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    checkHomeDir();
    checkDebugFile();

     //qDebug() << Q_FUNC_INFO << " -  00" ;

    setupDialog->init(softwareVersion, 0, configured);
     //qDebug() << Q_FUNC_INFO << " -  01" ;

    filemanager->init();
    init_variables();

    hamlib->initClass();
    qsoInUI.clear();

    setCleaning(false);

     //qDebug() << Q_FUNC_INFO << " -  10" ;

    dxClusterWidget->init();

    setModifying(false);
        //qDebug() << Q_FUNC_INFO << " -  50" << (QTime::currentTime()).toString("HH:mm:ss") ;

    checkExistingData();
    readSettingsFile();

     //qDebug() << Q_FUNC_INFO << " -  70" << (QTime::currentTime()).toString("HH:mm:ss") ;
    mapWindow->init();
       //qDebug() << Q_FUNC_INFO << " -  71" << (QTime::currentTime()).toString("HH:mm:ss") ;
    logWindow->createlogPanel(currentLog);
        //qDebug() << Q_FUNC_INFO << " -  72" << (QTime::currentTime()).toString("HH:mm:ss") ;
    awards.setManageModes(manageMode);
        //qDebug() << Q_FUNC_INFO << " -  73" << (QTime::currentTime()).toString("HH:mm:ss") ;
    if (dataProxy->getNumberOfManagedLogs()<1)
    {
            //qDebug() << Q_FUNC_INFO << " -  73.1" << (QTime::currentTime()).toString("HH:mm:ss") ;
        openSetup(6);
            //qDebug() << Q_FUNC_INFO << " -  73.2" << (QTime::currentTime()).toString("HH:mm:ss") ;
    }
        //qDebug() << Q_FUNC_INFO << " -  74" << (QTime::currentTime()).toString("HH:mm:ss") ;

    awardsWidget->fillOperatingYears();
    awardsWidget->showAwards();
    awardsWidget->setManageDXMarathon(manageDxMarathon);

    dxClusterWidget->setCurrentLog(currentLog);
    //dxClusterAssistant->init();

     //qDebug() << Q_FUNC_INFO << " -  80" << (QTime::currentTime()).toString("HH:mm:ss") ;
     //qDebug() << Q_FUNC_INFO << ": calling Software update ..." << (QTime::currentTime()).toString("HH:mm:ss") ;
    checkVersions();

     //qDebug() << Q_FUNC_INFO << " -  90" << (QTime::currentTime()).toString("HH:mm:ss") ;
    currentBandShown = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
     //qDebug() << Q_FUNC_INFO << " -  91" << (QTime::currentTime()).toString("HH:mm:ss") ;
    currentModeShown = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
     //qDebug() << Q_FUNC_INFO << " -  92" << (QTime::currentTime()).toString("HH:mm:ss") ;

    currentBand = currentBandShown;
    currentMode = currentModeShown;

    timerInfoBars = new QTimer(this);

    //qDebug() << Q_FUNC_INFO << " - Calling createUI" << (QTime::currentTime()).toString("HH:mm:ss") ;
    createUI();
    //qDebug() << Q_FUNC_INFO << " - Calling slotClearButtonClicked" << (QTime::currentTime()).toString("HH:mm:ss") ;
       //qDebug() << Q_FUNC_INFO << " - 100";
    slotClearButtonClicked(Q_FUNC_INFO);
       //qDebug() << Q_FUNC_INFO << " - 110";
    infoWidget->showInfo(-1);
       //qDebug() << Q_FUNC_INFO << " - 120";

    upAndRunning = true;
    mainQSOEntryWidget->setUpAndRunning(upAndRunning);
       //qDebug() << Q_FUNC_INFO << " - 130";

    applySettings ();    
    //qDebug() << Q_FUNC_INFO << " - END" << (QTime::currentTime()).toString("HH:mm:ss") ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::checkExistingData()
{
     //qDebug() << Q_FUNC_INFO << " - " << (QTime::currentTime()).toString("HH:mm:ss") ;
    bool existingData = QFile::exists(util->getKLogDBFile());
     //qDebug() << Q_FUNC_INFO << " -  1" ;
    ctyDatFile = util->getCTYFile();
     //qDebug() << Q_FUNC_INFO << " -  2" ;

    if ((!existingData) || (!world->hasSpecialEntities()))
    {
         //qDebug() << Q_FUNC_INFO << " -  Recreating world" ;
        world->recreate(ctyDatFile);
         //qDebug() << Q_FUNC_INFO << " -  3" ;
        //world->create(ctyDatFile);
         //qDebug() << Q_FUNC_INFO << " -  4" ;

       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Question);

       msgBox.setWindowTitle(tr("KLog - CTY.dat update"));
       msgBox.setText(tr("KLog needs to update the Entities database."));
       msgBox.setDetailedText(tr("You can update the entities database in Tools->Update cty.csv"));
       msgBox.setInformativeText(tr("Do you want to update now?"));
       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
       msgBox.setDefaultButton(QMessageBox::Yes);
       int ret = msgBox.exec();

       switch (ret)
       {
           case QMessageBox::Yes :
               slotUpdateCTYDAT();
           break;
       default:
           break;
       }
        //qDebug() << Q_FUNC_INFO << " -  7" << (QTime::currentTime()).toString("HH:mm:ss") ;
    }
     //qDebug() << Q_FUNC_INFO << " -  END" ;
}

void MainWindow::readSettingsFile()
{
    if (util->fileExists(util->getCfgFile()))
    {
        UpdateSettings settingsUpdate;
        if (settingsUpdate.updateFile())
        {
            configured = loadSettings();
        }
    }

    QSettings settings(util->getCfgFile(), QSettings::IniFormat);
    settings.setValue("Version", softwareVersion);
}

void MainWindow::checkVersions()
{
    if (checkNewVersions)
    {
        if (reportInfo)
        {
            softUpdate->addCall(stationCallsign);
        }
        softUpdate->needToUpdate();
    }
}

void MainWindow::createActionsCommon(){
// Functional widgets connections
//TODO: Reimplement the possibility to enter a QSO with enter inthe following widgets:
    //connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    logEvent(Q_FUNC_INFO, "Start", Debug);
    connect(util, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)));
      //qDebug() << Q_FUNC_INFO << " - Connecting QSO";
    connect(&qsoInUI, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)));
      //qDebug() << Q_FUNC_INFO << " - Connected QSO";
    connect(QSOTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(QSOTabWidget, SIGNAL(dxLocatorChanged(QString)), this, SLOT(slotLocatorTextChanged(QString) ) );

    connect(myDataTabWidget, SIGNAL(myLocChangedSignal(QString)), this, SLOT(slotMyLocatorTextChanged(QString) ) );
    connect(myDataTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(myDataTabWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(QSOTabWidget, SIGNAL(rxFreqChanged(double)), this, SLOT(slotFreqRXChanged(double) )) ;
    connect(QSOTabWidget, SIGNAL(txFreqChanged(double)), this, SLOT(slotFreqTXChanged(double) )) ;
    connect(QSOTabWidget, SIGNAL(handOverFocusSignal()), this, SLOT(slotTakeOverFocusToMainQSOInput() ));
    connect(loggWinAct, SIGNAL(triggered()), this, SLOT(slotLogWinShow()));

    //Buttons Actions

    connect(mainQSOEntryWidget, SIGNAL(handOverFocusSignal()), this, SLOT(slotTakeOverFocusToQSOTabWidget()));
    connect(mainQSOEntryWidget, SIGNAL(currentQRZSignal(QString)), this, SLOT(slotQRZTextChanged(QString)));
    connect(mainQSOEntryWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(mainQSOEntryWidget, SIGNAL(showInfoLabel(QString)), this, SLOT(slotShowInfoLabel(QString)) );
    connect(mainQSOEntryWidget, SIGNAL(clearForNextQSOSignal(QString)), this, SLOT(slotClearButtonClicked(QString)) );
    connect(mainQSOEntryWidget, SIGNAL(OKClicked()), this, SLOT(slotQRZReturnPressed() ) );
    connect(mainQSOEntryWidget, SIGNAL(bandChanged(QString)), this, SLOT(slotBandChanged(QString) ) );
    connect(mainQSOEntryWidget, SIGNAL(modeChanged(QString)), this, SLOT(slotModeChanged(QString) ) );
    connect(mainQSOEntryWidget, SIGNAL(validBands(QStringList)), this, SLOT(slotValidBandsReceived(QStringList) ) );
    connect(mainQSOEntryWidget, SIGNAL(manualModeSignal(bool)), this, SLOT(slotManualMode(bool) ) );

    // LOGVIEW
    connect(logWindow.get(), SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(logWindow.get(), SIGNAL(actionDeleteQSO ( int ) ), this, SLOT(slotQSODelete(int) ) );
    connect(logWindow.get(), SIGNAL(deleteTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsDelete(QList<int>) ) );
    connect(logWindow.get(), SIGNAL(exportToADIFTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsExportToADIF(QList<int>) ) );
    connect(logWindow.get(), SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(logWindow.get(), SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );

    //CLUSTER
    connect(dxClusterWidget.get(), SIGNAL(dxspotclicked(DXSpot)), this, SLOT(slotAnalyzeDxClusterSignal(DXSpot) ) );
    connect(dxClusterWidget.get(), SIGNAL(dxspotArrived(DXSpot)), this, SLOT(slotDXClusterSpotArrived(DXSpot) ) );

    // CLUBLOG
    connect (elogClublog, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (elogClublog, SIGNAL (actionReturnDownload(int, int)), this, SLOT (slotElogClubLogProcessAnswer(int, int)));
    connect (elogClublog, SIGNAL (disableClubLogAction(bool)), this, SLOT (slotElogClubLogDisable(bool)));
    connect (elogClublog, SIGNAL (signalFileUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogClubLogFileUploaded(QNetworkReply::NetworkError, QList<int>)));
    connect (eqslUtilities, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (eqslUtilities, SIGNAL (signalFileUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogEQSLFileUploaded(QNetworkReply::NetworkError, QList<int>)));

    // QRZCOM
    connect (elogQRZcom, SIGNAL (showMessage(QString)), this, SLOT (slotElogQRZCOMShowMessage(QString)));
    connect (elogQRZcom, SIGNAL (dataFoundSignal(QString, QString)), this, SLOT (slotElogQRZCOMFoundData(QString, QString)));
    connect (elogQRZcom, SIGNAL (signalLogUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogQRZCOMLogUploaded(QNetworkReply::NetworkError, QList<int>)));
    //connect (elogQRZcom, SIGNAL (disableQRZAction(bool)), this, SLOT (slotElogQRZCOMDisable(bool)));

    connect(world, &World::queryError, this, &MainWindow::slotQueryErrorManagement);
    //connect(world, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );

    // QSL TAB
    connect(QSLTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );
    // SEARCH TAB
    connect(searchWidget.get(), SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(searchWidget.get(), SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(searchWidget.get(), SIGNAL(logRefresh() ), this, SLOT(slotLogRefresh() ) );
    connect(searchWidget.get(), SIGNAL(toStatusBar(QString) ), this, SLOT(slotUpdateStatusBar(QString) ) );
    connect(searchWidget.get(), SIGNAL(requestBeingShown() ), this, SLOT(slotShowSearchWidget() ) );
    connect(searchWidget.get(), SIGNAL(actionQSODelete( int ) ), this, SLOT(slotQSODelete(int) ) );
    connect(searchWidget.get(), SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(&awards, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(&awards, SIGNAL(awardDXCCUpdated()), this, SLOT(slotRefreshDXCCWidget()) );
    connect(awardsWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(awardsWidget, SIGNAL(requireCurrentLogSignal()), this, SLOT(slotAwardsWidgetSetLog()) );
    connect(awardsWidget, SIGNAL(requireCurrentYearSignal()), this, SLOT(slotAwardsWidgetSetYear()) );

    //DXCCWIDGET TAB
    //connect(dxccStatusWidget, SIGNAL(showQso(int)), this, SLOT(slotShowQSOFromDXCCWidget(int) ) );
    connect(dxccStatusWidget.get(), SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(dxccStatusWidget.get(), SIGNAL(showQsos(QList<int>)), this, SLOT(slotShowQSOsFromDXCCWidget(QList<int>) ) );
    connect(dxccStatusWidget.get(), SIGNAL(fillInQSOSignal()), this, SLOT(fillQSOData()) );
    //connect(dxccStatusWidget, SIGNAL(updateAwards()), this, SLOT(slotShowAwards() ) );
    connect(filemanager, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(filemanager, SIGNAL(addQSOToList(QStringList)), this, SLOT(slotReceiveQSOListToShowFromFile(QStringList)) );

    //connect(scoreeWinAct, SIGNAL(triggered()), this, SLOT(slotScoreWinShow()));

    // UDPLogServer - WSJT-x
    connect(UDPLogServer, SIGNAL(status_update(int, QString, double, QString, QString, QString, QString, QString, QString)), this, SLOT(slotWSJXstatusFromUDPServer(int, QString, double, QString, QString, QString, QString, QString, QString) ) );

    connect(UDPLogServer, SIGNAL(clearSignal(QString)), this, SLOT(slotClearButtonClicked(QString) ) );
    connect(UDPLogServer, SIGNAL(logged(QSO)), this, SLOT(slotQSOReceived(QSO) ) );

    connect(this, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(setupDialog, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(setupDialog, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(setupDialog, SIGNAL(exitSignal(int)), this, SLOT(slotExitFromSlotDialog(int)) );
    //connect(setupDialog, SIGNAL(qrzcomAuto(bool)), this, SLOT(slotElogQRZCOMAutoCheckFromSetup(bool)) );
    connect(setupDialog, SIGNAL(finished(int)), this, SLOT(slotSetupDialogFinished(int)) );
    connect(setupDialog, SIGNAL(darkModeChanged(bool)), this, SLOT(slotDarkModeChanged(bool)) );


    connect(tipsDialog, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(tipsDialog, SIGNAL(findQSL2QSOSignal()), this, SLOT(slotSearchToolNeededQSLToSend()) );
    connect(tipsDialog, SIGNAL(fillInDXCCSignal()), this, SLOT(slotFillEmptyDXCCInTheLog()) );
    connect(tipsDialog, SIGNAL(fillInQSOSignal()), this, SLOT(fillQSOData()) );
    connect(tipsDialog, SIGNAL(fileExportToPrintSignal()), this, SLOT(slotRQSLExport()) );
    connect(tipsDialog, SIGNAL(fileOpenKLogFolderSignal()), this, SLOT(slotOpenKLogFolder()));
    connect(tipsDialog, SIGNAL(toolSendPendingQSLSignal()), this, SLOT(slotToolSearchRequestedQSLToSend()));
    connect(tipsDialog, SIGNAL(toolRecPendingQSLSignal()), this, SLOT(slotToolSearchNeededQSLPendingToReceive()));
    connect(tipsDialog, SIGNAL(toolRecRecPendingQSLSignal()), this, SLOT(slotToolSearchNeededQSLRequested()));
    connect(tipsDialog, SIGNAL(toolsUploadLoTWSignal()), this, SLOT(slotLoTWExport()));

    // SATELLITES TAB
    //connect(satTabWidget, SIGNAL(newBandsToBeAdded(QStringList)), this, SLOT(slotDefineNewBands(QStringList)) );
    connect(satTabWidget, SIGNAL(satTxFreqChanged(double)), this, SLOT(slotFreqTXChangedFromSat(double)  ) );
    //connect(satTabWidget, SIGNAL(satRxFreqChanged(double)), this, SLOT(slotFreqRXChanged(double)  ) );
    //connect(satTabWidget, SIGNAL(dxLocatorChanged(QString)), this, SLOT(slotUpdateLocator(QString)) );
    connect(satTabWidget, SIGNAL(setPropModeSat(QString,bool)), this, SLOT(slotSetPropModeFromSat(QString,bool)) ) ;
    connect(satTabWidget, SIGNAL(satTXFreqNeeded(double)), this, SLOT(slotFreqTXChanged(double)));
    connect(satTabWidget, SIGNAL(satRXFreqNeeded(double)), this, SLOT(slotFreqRXChanged(double)));
    //connect(satTabWidget, SIGNAL (satBandTXChanged(QString)), this, SLOT (slotSatBandTXComboBoxChanged(QString)));
    connect(satTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );

    connect(othersTabWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(othersTabWidget, SIGNAL(setPropMode(QString)), this, SLOT(slotSetPropModeFromOther(QString)) ) ;

    connect(downloadcty, SIGNAL(done(bool)), this, SLOT(slotWorldReload(bool)) );
    connect(timerInfoBars, SIGNAL(timeout()), this, SLOT(slotTimeOutInfoBars()) );
    connect(hamlib, SIGNAL(freqChanged(double)), this, SLOT(slotHamlibTXFreqChanged(double)) );
    connect(hamlib, SIGNAL(modeChanged(QString)), this, SLOT(slotHamlibModeChanged(QString)) );
    connect(lotwUtilities, SIGNAL(actionProcessLoTWDownloadedFile(QString)), this, SLOT(slotLoTWDownloadedFileProcess(QString)) );
    connect(adifLoTWExportWidget, SIGNAL(qsosToSend(QString, QList<int>, ExportMode)), this, SLOT(slotADIFExportSelection(QString, QList<int>, ExportMode)) );
    connect(adifLoTWExportWidget, SIGNAL(askingToClose()), this, SLOT(slotADIFExportClose()) );

    connect(dataProxy, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(dataProxy, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );

    connect(showKLogLogWidget, SIGNAL(newLogLevel(DebugLogLevel)), this, SLOT(slotNewLogLevel(DebugLogLevel)) );
    //connect(this, SIGNAL(focusC), this, SLOT(slotTimeOutInfoBars()) );
    // Following calls answer calls from the QSO to receive information.
    connect (&qsoInUI, SIGNAL(getModeSignal(QString)), this, SLOT(slotQSO_SetMode(QString)));
    connect(&qsoInUI, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotQSO_SetMode(const QString _submode)
{
    qsoInUI.setMode(dataProxy->getNameFromSubMode (_submode));
}

void MainWindow::recommendBackupIfNeeded()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (dataProxy->getHowManyQSOInLog(-1)<1)
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    QDateTime lastBackupDate;
    lastBackupDate = QDateTime();
    lastBackupDate = filemanager->getDateTimeOfLastBackup();
      //qDebug() << Q_FUNC_INFO << " -  lastDate: " << util->getDateTimeSQLiteStringFromDateTime(lastBackupDate) << (QTime::currentTime()).toString(" HH:mm:ss")  ;
    bool backupNeeded = false;
    QString msg;
    if (lastBackupDate == QDateTime())
    {
        backupNeeded = true;
        msg = tr("You seem to have never backed up or exported your log to ADIF.");
    }
    else if (lastBackupDate.addMonths(1) < QDateTime::currentDateTime())
    {
          //qDebug() << Q_FUNC_INFO << " -  More than a month" << (QTime::currentTime()).toString(" HH:mm:ss")   ;
        backupNeeded = true;
        msg = tr("Your latest backup seems older than one month.");
    }

    if (backupNeeded)
    {
          //qDebug() << Q_FUNC_INFO << " -  We need to backup"  << (QTime::currentTime()).toString(" HH:mm:ss")  ;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);

        //msg = msg + tr("Do you want to backup your logs now?");
        //msgBox.setText(msg);
        msgBox.setWindowTitle(tr("Log backup recommended!"));
        msgBox.setText(msg);

        msgBox.setInformativeText(tr("Regular backups prevent data loss and are good operator practice.\n"
                                     "Once exported, copy your ADIF file to a safe place such as a USB drive, cloud drive or other offsite computer.\n\n"
                                     "KLog will remind you to backup on a monthly basis.\n\n"));

        msgBox.addButton(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Yes:
            QString filename = util->getBackupADIFile();
      //qDebug() << Q_FUNC_INFO << " -  Backup to: " << filename << (QTime::currentTime()).toString(" HH:mm:ss")   ;
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - Backup"));
            if (filemanager->adifLogExport(filename, 0)) // 0 will save ALL the logs)
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("Backup completed successfully"));
                msgBox.setInformativeText(tr("KLog will remind you again in approximately one month."));
            }
            else
            {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("Backup failed."));
                msgBox.setInformativeText(tr("Periodic data backups are recommended to prevent data loss and corruption of your log."));
            }
            msgBox.exec();
            break;
        }
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::checkIfNewVersion()
{
      //qDebug() << Q_FUNC_INFO << " - " << util->getVersion();
     logEvent(Q_FUNC_INFO, "Start", Debug);
    //itIsANewversion = true;
    if (itIsANewversion)
    {
        if (util->getVersion() == "2.2")
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog - New version detected!"));
            msgBox.setText(tr("This version of KLog requires that the DXCC database is updated."));
            msgBox.setInformativeText(tr("The database will be updated."));
            msgBox.exec();
            slotWorldReload(true);
        }
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog - New version detected!"));
        msgBox.setText(tr("It seems that you are running this version of KLog for the first time."));
        msgBox.setInformativeText(tr("The setup will now open to allow you to change your settings."));
        msgBox.exec();
        openSetup(0);
    }
       //qDebug() << Q_FUNC_INFO << " - END"  ;
}

void MainWindow::createStatusBar()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    statusBar()->showMessage(tr("Ready"));
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowMap()
{
    Locator locator;

    Coordinate center = locator.getLocatorCoordinate(world->getQRZLocator(stationCallsign));
    mapWindow->setCenter(center);

    QSize size = this->size();
      //qDebug() << Q_FUNC_INFO << QString(" - Size: %1x%2").arg(size.width()).arg(size.height());
    size = size/2;
      //qDebug() << Q_FUNC_INFO << QString(" - Size: %1x%2").arg(size.width()).arg(size.height());
    mapWindow->resize(size);
    mapWindow->show();
    //QStringList a;
    //a.clear();
    //a << dataProxy->getFilteredLocators("All", "All", "All", "All");
    //a << locator->getAll();
    //foreach (QString ai, a)
    //{
    //    mapWindow->addMarker (locator->getLocatorCoordinate (ai));
    //}
    //mapWindow->addLocators(a, QColor(0, 0, 255, 127));
}

//void MainWindow::slotShowDXClusterAssistant()
//{
    //dxClusterAssistant->show();
//}
void MainWindow::setMainWindowTitle()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QString aux = dataProxy->getCommentsFromLog(currentLog);
    int numberOfQSOs = dataProxy->getHowManyQSOInLog (currentLog);
     //qDebug() << Q_FUNC_INFO << " - (comment): " << aux ;
    QString msg;

    if (mainQRZ == stationCallsign)
    {
        msg = QString(tr("KLog-%1 - Logbook of %2 - QSOs: %3" )).arg(softwareVersion).arg(stationCallsign).arg(numberOfQSOs);
    }
    else
    {
        msg = QString(tr("KLog-%1 - Logbook of %2 - Station Callsign: %3 - QSOs: %4" )).arg(softwareVersion).arg(mainQRZ).arg(stationCallsign).arg(numberOfQSOs);
    }
    if (aux.length ()>0)
    {
        setWindowTitle(QString("%1 - %2").arg(msg).arg(aux));
    }
    else
    {
        setWindowTitle(msg);
    }
}

void MainWindow::createUI()
{
      //qDebug() << Q_FUNC_INFO << " - Start";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    createStatusBar();
    setWindowTitle(tr("KLog"));
    createUIDX();
    createActionsCommon();
    createMenusCommon();

    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << Q_FUNC_INFO << " - END" ;
}

void MainWindow::slotTimeOutInfoBars()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
        //qDebug() << Q_FUNC_INFO << " -  - Start" ;
    slotShowInfoLabel(infoLabel1T);
    infoLabel2->setText(infoLabel2T);
    logEvent(Q_FUNC_INFO, "END", Debug);
}


void MainWindow::setModeFromFreq()
{
    //TODO: define frequency ranges for the different modes
    if (hamlibActive && !manualMode)
    {
        if (QSOTabWidget->getTXFreq () >= dataProxy->getLowLimitBandFromBandName("20M"))
        {
            hamlib->setMode("USB");
        }
        else
        {
            hamlib->setMode("LSB");
        }
        if (!hamlibChangingMode)
        {
            hamlib->setMode(mainQSOEntryWidget->getMode());
        }
    }
}

void MainWindow::slotBandChanged (const QString &_b)
{
      //qDebug() << Q_FUNC_INFO << " -  " << _b ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if ((!upAndRunning) || (_b.length()<2))
    {
          //qDebug() << Q_FUNC_INFO << " -  !upAndRunning or band short"  ;
        return;
    }

    if (updatingBands)
    {
          //qDebug() << "MainWindow::slotBandChanged: updating Bands"  ;
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }

    bool isFRinBand = dataProxy->isThisFreqInBand(_b, QString::number(QSOTabWidget->getTXFreq ()));
    if ((isFRinBand) && (QSOTabWidget->getTXFreq () >0 ))
    { // No change in txFreq
          //qDebug() << "MainWindow::slotBandChanged: isFRinBand and Freq >0"  ;
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return;
    }

    currentBandShown = dataProxy->getIdFromBandName(_b);
    currentModeShown = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
    currentBand = currentBandShown;
    currentMode = currentModeShown;

    if ((!isFRinBand) || (QSOTabWidget->getTXFreq()<=0))
    {
          //qDebug() << "MainWindow::slotBandChanged: Freq is not in band or empty"  ;
          //qDebug() << "MainWindow::slotBandChanged: Band: " << mainQSOEntryWidget->getBand()  ;
          //qDebug() << "MainWindow::slotBandChanged: Freq: " << QString::number(QSOTabWidget->getTXFreq())  ;
        double txFr = (dataProxy->getFreqFromBandId(currentBandShown)).toDouble();
   //qDebug() << "MainWindow::slotBandChanged: New Freq: " << QString::number(txFr) ;

        slotFreqTXChanged (txFr);

        //if (!dataProxy->isThisFreqInBand(_b, QString::number(rxFreqSpinBox->value ())))
        //{
        //    rxFreqSpinBox->setvalue (QSOTabWidget->getTXFreq());
        //}
    }
      //qDebug() << "MainWindow::slotBandChanged: Checking to update Freq  - DONE"  ;

    EntityStatus _entityStatus;
    _entityStatus.dxcc      = currentEntity;
    _entityStatus.bandId    = currentBandShown;
    _entityStatus.modeId    = currentModeShown;
    _entityStatus.logId     = currentLog;
       //qDebug() << "MainWindow:: - calling showStatusOfDXCC-02 " ;
    if (currentEntity>0)
    {
        showStatusOfDXCC(_entityStatus);
    }

    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::slotBandChanged: END" ;
}

void MainWindow::slotModeChanged (const QString &_m)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!upAndRunning)
    {
        return;
    }
      //qDebug() << "MainWindow::slotModeChanged: " << _m ;

    currentBandShown = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
      //qDebug() << "MainWindow::slotModeComboBoxChanged: currentBandShown2: " << QString::number(currentBandShown) ;
    currentModeShown = dataProxy->getIdFromModeName(_m);
    currentBand = currentBandShown;
    currentMode = currentModeShown;

    EntityStatus _entityStatus;
    _entityStatus.dxcc      = currentEntity;
    _entityStatus.bandId    = currentBandShown;
    _entityStatus.modeId    = currentModeShown;
    _entityStatus.logId     = currentLog;

    showStatusOfDXCC(_entityStatus);
    if (!modify)
    {
        QSOTabWidget->setRSTToMode(mainQSOEntryWidget->getMode(), readingTheUI);
    }

    //QString _modeSeen = mainQSOEntryWidget->getMode();
    if (hamlibActive && !manualMode)
    {
        hamlib->setMode(mainQSOEntryWidget->getMode());
    }
/*    if (_modeSeen == "SSB")
    {
        setModeFromFreq();
    }
*/
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << Q_FUNC_INFO << " - " << mainQSOEntryWidget->getMode() ;
}

void MainWindow::slotOKButtonClicked(){
    logEvent(Q_FUNC_INFO, "Start", Debug);
    slotQRZReturnPressed();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotQRZReturnPressed()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
   //qDebug() << Q_FUNC_INFO << " - Start"  ;
    if (mainQSOEntryWidget->getQrz().length()<=0)
    {
       //qDebug() << Q_FUNC_INFO << " -  no QRZ"  ;
        return;
    }

    readingTheUI = true;


    if (!readQSOFromUI())
    {
        //qDebug() << Q_FUNC_INFO << " - readQSOFromUI returned FALSE";
        return;
    }
    if (!qsoInUI.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - QSO Not valid!";
        return;
    }

    int addedOK = qsoInUI.toDB (modifyingQSOid);
   //qDebug() << Q_FUNC_INFO << ": id: " <<  QString::number(addedOK);
    if (addedOK>0)
    {
        //qDebug() << Q_FUNC_INFO << ": QSO Added: " << QString::number(addedOK);
        mapWindow->addLocator(qsoInUI.getGridSquare(), workedColor);
        actionsJustAfterAddingOneQSO();
    }

    // Just to prepare or some tasks before reading DATA from UI
    yearChangedDuringModification = false;
    readingTheUI = false;

   //qDebug() << Q_FUNC_INFO << "Just before cleaning";
    slotClearButtonClicked(Q_FUNC_INFO);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::actionsJustAfterAddingOneQSO()
{
   //qDebug() << Q_FUNC_INFO << " - Start" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    int lastId = -1;
    needToSave = true;
    if (modify)
    {
        //qDebug() << Q_FUNC_INFO << " -  Modifying! " ;
       needToSave = true;
       if(modifyingQSOid>0)
       {           
           awards.setAwards();
           if (yearChangedDuringModification)
           {
               awardsWidget->fillOperatingYears();
               yearChangedDuringModification = false;
           }
           if ((clublogActive) && (clublogRealTime))
           {
                 //qDebug() << Q_FUNC_INFO << " -  (Modifiying ClubLog) Lastid: "<< QString::number(lastId) ;
                // Delete QSO in CLubLog
               elogClublog->deleteQSO(clublogPrevQSO);
                // Add modified QSO in ClubLog
               elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(modifyingQSOid));
           }
           else
           {
                    //qDebug() << Q_FUNC_INFO << " -  (No ClubLog) Lastid: "<< QString::number(lastId) ;
           }
           awards.setAwards();   //Update the DXCC award status
       }
       // CHECK WHAT WAS THE QSOID to add the awards, if needed
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << " -  Not Modifying " ;
        lastId = dataProxy->getLastQSOid();
        if (lastId>=0)
        {
             //qDebug() << Q_FUNC_INFO << " -  Lastid: "<< QString::number(lastId) ;
            awards.setAwards();   //Update the DXCC award status
            // Send to CLUBLOG if enabled
            if ((clublogActive) && (clublogRealTime))
            {
            //qDebug() << Q_FUNC_INFO << " -  (Sending ClubLog) Lastid: "<< QString::number(lastId) ;
                elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(lastId));
            }
            else
            {
            //qDebug() << Q_FUNC_INFO << " -  (No ClubLog) Lastid: "<< QString::number(lastId) ;
            }
            //<CLUBLOG>
        }
        else
        {
        //qDebug() << Q_FUNC_INFO << " -  Lastid < 0 "<< QString::number(lastId) ;
        }
        //awards.setAwards(lastId);
    }
    logWindow->refresh();
    //awards.updateDXCCStatus(-1);
    dxccStatusWidget->refresh();
    searchWidget->refresh();
    awardsWidget->showAwards ();
    logEvent(Q_FUNC_INFO, "END", Debug);
   //qDebug() << Q_FUNC_INFO << " - END" ;
}


bool MainWindow::checkValidCallBeforeAddingToLog(const QString &_call)
{
    //qDebug() << Q_FUNC_INFO << " : "  << _call;
    Callsign callsign(_call);
    if (!callsign.isValid())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - Not valid call"));
        QString aux = QString(tr("The callsign %1 is not a valid call. Do you really want to add this callsign to the log?") ).arg(_call);
        msgBox.setText(aux);
        msgBox.setInformativeText(tr("Adding non-valid calls to the log may create problems when applying for awards, exporting ADIF files to other systems or applications."));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret)
        {
        case QMessageBox::Yes:
            // Ok was clicked
            return true;
        case QMessageBox::No:
            return false;
        default:
            // should never be reached
            return true;
        }
    }
    return true;
}

int MainWindow::checkDXCCBeforeAddingToLog(const int dxcc_Call, const int dxcc_qso)
{
    int selected = world->selectEntity(dxcc_Call, dxcc_qso);

    if ((dxcc_Call!=dxcc_qso) && (selected < 0))
    {

        QString dxcc1_name    = world->getEntityName(dxcc_Call);
        QString dxcc1_prefix  = world->getEntityMainPrefix(dxcc_Call);

        QString dxcc2_name    = world->getEntityName(dxcc_qso);
        QString dxcc2_prefix  = world->getEntityMainPrefix(dxcc_qso);

        QString message           = QString(tr("The entity that is selected is different from the one proposed by KLog:") + "\n\n");

        QString message_dxcc_qso  = dxcc1_prefix + " - " + dxcc1_name + "\n\n";
        if (dxcc1_prefix.length()<1)
        {
          dxcc1_prefix      = tr ("Unknown", "Keep it short, its a button text");
          message_dxcc_qso  = QString(tr("- There is no selected DXCC.") + "\n\n");
        }

        QString message_dxcc_klog = dxcc2_prefix + " - " + dxcc2_name;
        if (dxcc2_prefix.length()<1)
        {
          dxcc2_prefix      = tr ("Unknown", "Keep it short, its a button text");
          message_dxcc_klog = QString (tr("- KLog couldn't find a DXCC")  + "\n\n");
        }

        message = message + message_dxcc_qso + message_dxcc_klog + "\n\n" + tr("Please select the one you want to keep for this QSO.");

        QPushButton *button2 = new QPushButton(this);
        QPushButton *button1 = new QPushButton(this);

        button1->setText(dxcc1_prefix);
        button2->setText(dxcc2_prefix);

        int ret;

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - Select correct entity"));
        msgBox.setText(message);

        msgBox.addButton(button2, QMessageBox::AcceptRole);
        msgBox.addButton(button1, QMessageBox::ActionRole);
        msgBox.addButton(QMessageBox::Cancel);
        ret = msgBox.exec();

        if (ret == QMessageBox::AcceptRole)
        {
            return dxcc_qso;
        }
        else if (ret == QMessageBox::ActionRole)
        {
            return dxcc_Call;
        }
        else if (ret == QMessageBox::Cancel)
        {
            return -1;
        }        
    }

    if (selected>0)
        return selected;
    return dxcc_Call;
}

void MainWindow::getQSODataFromUI()
{
   //qDebug() << Q_FUNC_INFO << " -  Call-01   : " << qsoInUI.getCall();
    qsoInUI.clear();
    qsoInUI = mainQSOEntryWidget->getQSOData(qsoInUI);
    qsoInUI = QSOTabWidget->getQSOData(qsoInUI);
    qsoInUI = commentTabWidget->getQSOData(qsoInUI);
    qsoInUI = othersTabWidget->getQSOData(qsoInUI);
    qsoInUI = eQSLTabWidget->getQSOData(qsoInUI);
    qsoInUI = QSLTabWidget->getQSOData(qsoInUI);
    qsoInUI = mainQSOEntryWidget->getQSOData(qsoInUI);
    qsoInUI = satTabWidget->getQSOData(qsoInUI);
    qsoInUI = myDataTabWidget->getQSOData(qsoInUI);

    //*qso = mainQSOEntryWidget->getQSOData(*qso);
    //*qso = QSOTabWidget->getQSOData(*qso);
    //*qso = commentTabWidget->getQSOData(*qso);
    //*qso = othersTabWidget->getQSOData(*qso);
    //*qso = eQSLTabWidget->getQSOData(*qso);
    //*qso = QSLTabWidget->getQSOData(*qso);
    //*qso = mainQSOEntryWidget->getQSOData(*qso);
    //*qso = satTabWidget->getQSOData(*qso);
    //*qso = myDataTabWidget->getQSOData(*qso);
   //qDebug() << Q_FUNC_INFO << " -  Call-02   : " << qsoInUI.getCall();
}

bool MainWindow::readQSOFromUI()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << " - 010";
    //qDebug() << Q_FUNC_INFO << " -  CALL-01    : " << qsoInUI.getCall();

    getQSODataFromUI();

    if (!checkValidCallBeforeAddingToLog(qsoInUI.getCall()))
    {
        //qDebug() << Q_FUNC_INFO << ": Not valid Call" ;
        return false;
    }

    //qDebug() << Q_FUNC_INFO << " - 030";
    QString tqrz = (qsoInUI.getCall());
    //qDebug() << Q_FUNC_INFO << " - tqrz: " <<  tqrz;
    //qDebug() << Q_FUNC_INFO << " - ARRLid: " <<  world->getQRZARRLId(tqrz);
    //qDebug() << Q_FUNC_INFO << " - DXCC  : " <<  qsoInUI.getDXCC();

    int dxcc = checkDXCCBeforeAddingToLog(world->getQRZARRLId(tqrz), qsoInUI.getDXCC());
   //qDebug() << Q_FUNC_INFO << ": " << dxcc;
    if (dxcc < 0)
    {
        return false;
    }

    qsoInUI.setDXCC (dxcc);
    //qDebug() << Q_FUNC_INFO << " - 040";
    qsoInUI.setContinent (dataProxy->getContinentShortNameFromEntity(dxcc));
    qsoInUI.setCQZone(infoWidget->getCQ());
    qsoInUI.setItuZone(infoWidget->getITU());
    qsoInUI.setMyCQZone(my_CQz);
    qsoInUI.setMyITUZone(my_ITUz);
    qsoInUI.setLogId (currentLog);
    //qDebug() << Q_FUNC_INFO << " - 050";

    if (qsoInUI.getDistance()<=0.0)
        qsoInUI.setDistance (infoWidget->getDistance ());

    keepSatPage = satTabWidget->getKeep();
    //qDebug() << Q_FUNC_INFO << " - 060";
    //qDebug() << Q_FUNC_INFO << " -  END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);

    return true;
}



void MainWindow::slotQSOsExportToADIF(QList<int> _qsos)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_qsos.length ()), Debug);
    if (_qsos.length()<1)
    {
        return; // NO QSO TO EXPORT
    }
     //qDebug() << Q_FUNC_INFO << " - xxy";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
     //qDebug() << Q_FUNC_INFO << fileName ;
    if ((!fileName.endsWith(".adi")) && ( !fileName.endsWith(".adif") ))
    {
   //qDebug() <<  Q_FUNC_INFO << ": Adding the .adi to the file" << fileName ;
        fileName = fileName +  ".adi";
    }
       //qDebug() <<  Q_FUNC_INFO << "-1: " << fileName ;
    filemanager->adifQSOsExport2(fileName, QString(), _qsos, ModeADIF);
    //filemanager->adifQSOsExport(fileName, _qsos);
       //qDebug() <<  Q_FUNC_INFO << "-3" ;
    showNumberOfSavedQSO(fileName, _qsos.count());
        //qDebug() <<  Q_FUNC_INFO << " - END" ;
}

void MainWindow::slotQRZcomUpload(QList<int> _qsos)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_qsos.length ()), Debug);

    elogQRZcom->sendQSOs(_qsos);

      //qDebug() << "MainWindow::slotQRZcomUpload - END" ;
}

void MainWindow::slotQSOsDelete(QList<int> _qsos)
{
        //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(_qsos.length())  ;
    //foreach(int i, _qsos)
    //{
    //        //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(i)  ;
    //}

    QString message = QString(tr("You have requested to delete several QSOs "));
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(message);
    msgBox.setDetailedText(tr("This operation shall remove definitely all the selected QSO and associated data and you will not be able to recover it again."));
    msgBox.setInformativeText(tr("Are you sure?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret)
    {
        case QMessageBox::Yes :

        foreach (int i, _qsos)
        {
            QStringList qsoToDelete;
            qsoToDelete.clear();
            qsoToDelete << dataProxy->getClubLogRealTimeFromId(i);
            if(dataProxy->deleteQSO(i))
            {
            //qDebug() << "MainWindow::slotQSODelete: Just removed from log, now I will try to remove from ClubLog, if needed" ;
                if (clublogActive && clublogRealTime)
                {
                //qDebug() << "MainWindow::slotQSODelete: Removing from ClubLog" ;
                    elogClublog->deleteQSO(qsoToDelete);
                }
                else
                {
                //qDebug() << "MainWindow::slotQSODelete: NOT emoving from ClubLog" ;
                }
            }
        }
        dxccStatusWidget->refresh();
        logWindow->refresh();
        slotShowAwards();


        break;
    case QMessageBox::No :
        break;
    }
}

void MainWindow::slotQSODelete(const int _id)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_id), Debug);
       //qDebug() << "MainWindow::slotQSODelete " << QString::number(_id)  ;

    int QSOid = _id;
    //int x = -1;

    QString _qrz = dataProxy->getCallFromId(QSOid);
    if (_qrz.length()>=3)
    {
        QString message = QString(tr("You have requested to delete the QSO with: %1").arg(_qrz));

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(message);
        msgBox.setInformativeText(tr("Are you sure?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Yes :
            {
            QStringList qsoToDelete;
            qsoToDelete.clear();
            qsoToDelete << dataProxy->getClubLogRealTimeFromId(QSOid);
            if(dataProxy->deleteQSO(QSOid))
            {
            //qDebug() << "MainWindow::slotQSODelete: Just removed from log, now I will try to remove from ClubLog, if needed" ;
                if (clublogActive && clublogRealTime)
                {
                //qDebug() << "MainWindow::slotQSODelete: Removing from ClubLog" ;
                    elogClublog->deleteQSO(qsoToDelete);
                }
                else
                {
                //qDebug() << "MainWindow::slotQSODelete: NOT emoving from ClubLog" ;
                }

                dxccStatusWidget->refresh();
                logWindow->refresh();
                searchWidget->refresh();
                slotShowAwards();
       //emit updateSearchText();
            }
            else
            {
        //TODO: The QSO could not be removed...
            }
            }
            break;
            case QMessageBox::No :
      // No was clicked
            break;
            default:
      // should never be reached
            break;
        }
    }
    else
    {
         // TODO: The QSO to be removed was not found in the log
    }
      //qDebug() << "MainWindow::slotQSODelete END "  ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowSearchWidget()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    dxUpRightTab->setCurrentIndex(dxUpRightTab->indexOf(searchWidget.get()));
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotLogRefresh()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    logWindow->refresh();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotElogClubLogDisable(const bool _b)
{
     //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    clublogActive = !_b;
    //setupDialog->setClubLogActive(clublogActive);


    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("ClubLog");
    settings.setValue ("ClubLogActive", _b);
    settings.endGroup();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotElogClubLogFileUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_error) + "/" + QString::number(_qsos.length ()), Debug);

    QMessageBox msgBox;
    if (_error != QNetworkReply::NoError)
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - ClubLog error"));
        msgBox.setText(tr("The ClubLog upload process has finished with an error and the log was possibly not uploaded."));
        msgBox.setDetailedText(tr("Please check your credentials, your Internet connection and your Clublog account. The received error code was: %1").arg(_error));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString fileName = util->getClubLogFile();
     if (QFile::exists(fileName))
     {
      //qDebug() << "MainWindow::slotElogClubLogFileUploaded file exist" ;
     }
     else
     {
     //qDebug() << "MainWindow::slotElogClubLogFileUploaded file DOES NOT exist" ;
     }

     msgBox.setIcon(QMessageBox::Question);
     msgBox.setWindowTitle(tr("KLog - ClubLog"));
     msgBox.setText(tr("Do you want to mark as Uploaded all the QSOs uploaded to ClubLog?") );
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
     msgBox.setDefaultButton(QMessageBox::Yes);
     int i = msgBox.exec();

     if (i == QMessageBox::Yes)
     {
         bool uploadedToClubLog = dataProxy->clublogSentQSOs(_qsos);
         slotLogRefresh();

         // TODO: Check if QSOS where sent
        if (!uploadedToClubLog)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - ClubLog"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("There was an error while updating to Yes the ClubLog QSO upload information."));
            msgBox.setStandardButtons(QMessageBox::Ok );
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
     }
    if (!deleteAlwaysAdiFile)
    {
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - ClubLog"));
        msgBox.setText(tr("The ClubLog upload process has finished and KLog created a file (%1) in your KLog folder.\n\nDo you want KLog to remove that file?").arg(fileName));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::Yes);
        i = msgBox.exec();
        if (i == QMessageBox::Yes)
        {
            if (QFile::remove(fileName))
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(tr("KLog - ClubLog"));
                msgBox.setText(tr("The file has been removed."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogClubLogFileUploaded - FILE REMOVED: " << fileName ;
            }
            else
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(tr("KLog - ClubLog"));
                msgBox.setText(tr("The file has not been removed."));
                msgBox.setDetailedText(tr("It seems that there was something that prevented KLog from removing the file\nYou can remove it manually."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogClubLogFileUploaded - FILE NOT REMOVED: " << fileName ;
            }
            //i = msgBox.exec();
        }
    }
    else
    {
        QFile::remove(fileName);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotElogClubLogShowMessage(const QString &_s)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    slotUpdateStatusBar(_s);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotElogClubLogProcessAnswer(const int _i, const int _qID)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_i) + "/" + QString::number(_qID), Debug);

    clublogAnswer = _i;

    if (clublogAnswer == 0) // NO ERROR
    {
        dataProxy->setClubLogSent(_qID, "Y", eQSLTabWidget->getClubLogDate());
    }
    else
    {
        dataProxy->setClubLogSent(_qID, "M", eQSLTabWidget->getClubLogDate());
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}


void MainWindow::slotElogEQSLFileUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_error) , Debug);

    QMessageBox msgBox;
    if (_error != QNetworkReply::NoError)
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - eQSL error"));
        msgBox.setText(tr("The eQSL upload process has finished with an error and the log was possibly not uploaded."));
        msgBox.setDetailedText(tr("Please check your credentials, your Internet connection and your eQSL account. The received error code was: %1").arg(_error));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString fileName = util->getEQSLFile();
    if (QFile::exists(fileName))
    {
      //qDebug() << "MainWindow::slotElogEQSLFileUploaded file exist" << fileName ;
    }
     else
     {
     //qDebug() << "MainWindow::slotElogEQSLFileUploaded file DOES NOT exist" ;
     }

     msgBox.setIcon(QMessageBox::Question);
     msgBox.setWindowTitle(tr("KLog - eQSL"));
     msgBox.setText(tr("Do you want to mark as Uploaded all the QSOs uploaded to eQSL?") );
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
     msgBox.setDefaultButton(QMessageBox::Yes);
     int i = msgBox.exec();

     if (i == QMessageBox::Yes)
     {
         bool uploadedToeQSL = dataProxy->eQSLSentQSOs(_qsos);
         slotLogRefresh();

         // TODO: Check if QSOS where sent
        if (!uploadedToeQSL)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - eQSL"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("There was an error while updating to Yes the eQSL QSO upload information."));
            msgBox.setStandardButtons(QMessageBox::Ok );
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
     }

     if (!deleteAlwaysAdiFile)
        {
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - eQSL"));
        msgBox.setText(tr("The eQSL upload process has finished and KLog created a file (%1) in your KLog folder.\n\nDo you want KLog to remove that file?").arg(fileName));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::Yes);
        i = msgBox.exec();
        if (i == QMessageBox::Yes)
        {
            if (QFile::remove(fileName))
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(tr("KLog - eQSL"));
                msgBox.setText(tr("The file has been removed."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogEQSLFileUploaded - FILE REMOVED: " << fileName ;
            }
            else
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(tr("KLog - eQSL"));
                msgBox.setText(tr("The file has not been removed."));
                msgBox.setDetailedText(tr("It seems that there was something that prevented KLog from removing the file\nYou can remove it manually."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogEQSLFileUploaded - FILE NOT REMOVED: " << fileName ;
            }
            //i = msgBox.exec();
        }
    }
    else
    {
        QFile::remove(fileName);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotElogQRZCOMDisable(const bool _b)
{
     //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if ((_b) && (elogQRZcom->getSubscription ()))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - QRZ.com warning"));
        msgBox.setText(tr("QRZ.com has returned a non-subcribed error and queries to QRZ.com will be disabled."));
        msgBox.setDetailedText(tr("Please check your QRZ.com subcription or credentials."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        qrzcomActive = false;
        QSettings settings(util->getCfgFile (), QSettings::IniFormat);
        settings.beginGroup ("QRZcom");
        settings.setValue ("QRZcomActive", false);
        settings.endGroup ();
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotElogQRZCOMLogUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos)
{
       //qDebug() << "MainWindow::slotElogQRZCOMLogUploaded: " << QString::number(_error) ;
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_error) , Debug);

       QMessageBox msgBox;
   if (_error != QNetworkReply::NoError)
   {
       msgBox.setIcon(QMessageBox::Warning);
       msgBox.setWindowTitle(tr("KLog - QRZ.com error"));
       msgBox.setText(tr("The QRZ.com upload process has finished with an error and the log was possibly not uploaded."));
       msgBox.setDetailedText(tr("Please check your credentials, your Internet connection and your eQSL account. The received error code was: %1").arg(_error));
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.setDefaultButton(QMessageBox::Ok);
       msgBox.exec();
       return;
   }

   QString fileName = util->getEQSLFile();
   if (QFile::exists(fileName))
   {
     //qDebug() << "MainWindow::slotElogQRZCOMLogUploaded file exist" ;
   }
    else
    {
    //qDebug() << "MainWindow::slotElogEQSLFileUploaded file DOES NOT exist" ;
    }

    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("KLog - QRZ.com"));
    msgBox.setText(tr("Do you want to mark as Uploaded all the QSOs uploaded to QRZ.com?") );

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::Yes);
    int i = msgBox.exec();

    if (i == QMessageBox::Yes)
    {
        bool uploadedToeQSL = dataProxy->QRZCOMSentQSOs(_qsos);
        slotLogRefresh();

        // TODO: Check if QSOS where sent
       if (!uploadedToeQSL)
       {
           QMessageBox msgBox;
           msgBox.setWindowTitle(tr("KLog - QRZ.com"));
           msgBox.setIcon(QMessageBox::Warning);
           msgBox.setText(tr("There was an error while updating to Yes the QRZ.com QSO upload information."));
           msgBox.setStandardButtons(QMessageBox::Ok );
           msgBox.setDefaultButton(QMessageBox::Ok);
           msgBox.exec();
       }
    }


   msgBox.setIcon(QMessageBox::Information);
   msgBox.setWindowTitle(tr("KLog - QRZ.com"));
   msgBox.setText(tr("The QRZ.com upload process has finished successfully"));
   msgBox.setStandardButtons(QMessageBox::Ok );
   msgBox.setDefaultButton(QMessageBox::Ok);
   msgBox.exec();

       //qDebug() << "MainWindow::slotElogEQSLFileUploaded - END"  ;
}

void MainWindow::slotElogQRZCOMShowMessage(const QString &_s)
{
      //qDebug() << "MainWindow::slotElogQRZCOMShowMessage: " << _s ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    slotUpdateStatusBar(_s);
    logEvent(Q_FUNC_INFO, "END", Debug);
}
void MainWindow::cleanQRZCOMreceivedDataFromUI()
{
   //qDebug() << Q_FUNC_INFO;
    if (!modify)
    {
        QSOTabWidget->cleanQRZCOM(true);
    }
    completedWithPreviousName = false;
    completedWithPreviousLocator = false;
}

void MainWindow::slotElogQRZCOMFoundData(const QString &_t, const QString & _d)
{
     //qDebug() << Q_FUNC_INFO << ": " << _t << "/" << _d ;
   if (_t == "name")
   {
       if (QSOTabWidget->getName().length()<1)
       {
           qrzAutoChanging = true;
           QSOTabWidget->setName(_d);
           qrzAutoChanging = false;
       }
   }
   else if (_t == "grid")
   {
         //qDebug() << Q_FUNC_INFO << " Grid found: " << _d;
       if ((QSOTabWidget->getDXLocator()).length()<1)
       {
           qrzAutoChanging = true;
           QSOTabWidget->setDXLocator(_d);
           qrzAutoChanging = false;
       }
       else
       {
     //qDebug() << Q_FUNC_INFO << " There was already a Grid: " << QSOTabWidget->getDXLocator();
       }
   }
   else if (_t == "qth")
   {
       if (QSOTabWidget->getQTH().length()<1)
       {
           qrzAutoChanging = true;
           QSOTabWidget->setQTH(_d);
           qrzAutoChanging = false;
       }
   }
   else if (_t == "qslmgr")
   {
        //QSLTabWidget->setQSLVia(_d);
   }
   else if (_t == "error")
    {
          //qDebug() << Q_FUNC_INFO << " ERROR" << _t << "/" << _d ;
        if (_d.contains("Not found: "))
        {
            cleanQRZCOMreceivedDataFromUI();
      //qDebug() << Q_FUNC_INFO << ": call Not found" ;
            slotUpdateStatusBar(tr("Call not found in QRZ.com"));
            return;
        }
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Warning);
       msgBox.setWindowTitle(tr("KLog - QRZ.com error"));
       QString aux = QString(tr("KLog has received an error from QRZ.com.") );
       msgBox.setText(aux);
       msgBox.setDetailedText(_d);
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.setDefaultButton(QMessageBox::Ok);
       msgBox.exec();
   }
   else
   {
          //qDebug() << Q_FUNC_INFO << ": QRZ value not recognized" ;
   }
}

void MainWindow::slotElogQRZCOMCheckThisCall()
{
       //qDebug() << Q_FUNC_INFO  ;
    if (qrzcomActive)
    {
        elogQRZcom->checkQRZ(mainQSOEntryWidget->getQrz());
    }
    else
    {
      showMessageToEnableTheOnlineService(QRZ)  ;
    }
      //qDebug() << Q_FUNC_INFO << " - END" ;
}

void MainWindow::showMessageToEnableTheOnlineService(const OnLineProvider _service)
{
    QString aux = util->getOnlineServiceName(_service);
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - %1").arg(aux));
    msgBox.setText(tr("You need to activate the %1 service in the eLog preferences.").arg(aux) );
    msgBox.exec();
}

void MainWindow::slotElogQRZCOMAutoCheck()
{
     //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString(QRZCOMAutoCheckAct->isChecked()) ;
    if (!qrzcomActive)
    {
        showMessageToEnableTheOnlineService(QRZ);
        return;
    }
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("QRZcom");
    settings.setValue ("QRZcomAuto", false);
    settings.endGroup ();

      //qDebug() << Q_FUNC_INFO << " - END" ;
}


void MainWindow::slotExitFromSlotDialog(const int exitID)
{
        //qDebug() << "MainWindow::slotExitFromSlotDialog: " << QString::number(exitID) ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (exitID == 2)
    {
        needToEnd = true;
        exitQuestion();
    }
        //qDebug() << "MainWindow::slotExitFromSlotDialog: END "  ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotFileClose()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    exitQuestion();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::exitQuestion()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
        //qDebug() << "MainWindow::exitQuestion"  ;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("KLog - Exit"));
    QString aux = QString(tr("Do you really want to exit KLog?") );
    msgBox.setText(aux);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret)
    {
        case QMessageBox::Yes:
  // Ok was clicked
        logEvent(Q_FUNC_INFO, "Exiting KLog!", Debug);
    //maybeSave();
            saveWindowsSize();
            close();
            exit(0);
        default:
    // should never be reached
        break;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotQRZTextChanged(QString _qrz)
{
   //qDebug()<< Q_FUNC_INFO << ": " << _qrz ;

    logEvent(Q_FUNC_INFO, QString("Start: %1").arg(_qrz), Debug);
     //qDebug()<< Q_FUNC_INFO << " - 10" ;
    if (_qrz.length()<1)
    {
         //qDebug()<< Q_FUNC_INFO << " - 11" ;
        infoLabel1->clear();
        infoLabel2->clear();
        slotClearButtonClicked(Q_FUNC_INFO);
        logEvent(Q_FUNC_INFO, "END-Empty", Devel);
        return;
    }
     //qDebug()<< Q_FUNC_INFO << " - 20" ;
    if (cleaning)
    {
          //qDebug()<< Q_FUNC_INFO << ": Cleaning" ;
        logEvent(Q_FUNC_INFO, "END-Cleaning", Devel);
        return;
    }
     //qDebug()<< Q_FUNC_INFO << " - 30" ;
    if (modify)
    {
        logEvent(Q_FUNC_INFO, "END-Modify", Devel);
        return;
    }

      //qDebug()<< Q_FUNC_INFO << ": checking for modify or length<1" ;
    if (qrzSmallModDontCalculate)
    //if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
         //qDebug()<< Q_FUNC_INFO << ": MODIFY or Lenght < 1" ;
        qrzSmallModDontCalculate=false;
        logEvent(Q_FUNC_INFO, "END-Small QRZ, don't calculate", Devel);
        return;
    }
     //qDebug()<< Q_FUNC_INFO << " - 40" ;
     //qDebug()<< Q_FUNC_INFO << ": running ..." ;
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    //int i;
    int dx_CQz = -1;
    int dxE_CQz = -1;
    int dx_ITUz = -1;
    int dxE_ITUz = -1;
    cleanQRZCOMreceivedDataFromUI();
    //qDebug()<< Q_FUNC_INFO << ": currentQRZ: " <<_qrz ;

    currentEntity = world->getQRZARRLId(_qrz);
   //qDebug()<< Q_FUNC_INFO << " - 50 - currentEntity: " << QString::number(currentEntity) ;

    logEvent(Q_FUNC_INFO, QString("Entity: %1").arg(currentEntity), Devel);

    //othersTabWidget->setEntity(currentEntity);
    dxE_CQz = world->getEntityCqz(currentEntity);
    dx_CQz = world->getQRZCqz(_qrz);
    dx_ITUz = world->getQRZItuz(_qrz);
    dxE_ITUz = world->getEntityItuz(currentEntity);
     //qDebug()<< Q_FUNC_INFO << " - 60" ;
      //qDebug()<< Q_FUNC_INFO << ": CQ: " << QString::number(dx_CQz) ;
      //qDebug()<< Q_FUNC_INFO << ": CQe: " << QString::number(dxE_CQz) ;
      //qDebug()<< Q_FUNC_INFO << ": ITU: " << QString::number(dx_ITUz) ;
      //qDebug()<< Q_FUNC_INFO << ": ITUe: " << QString::number(dxE_ITUz) ;

    if (dx_CQz == dxE_CQz)
    {
        dx_CQz = dxE_CQz;
    }

    if (dx_ITUz == dxE_ITUz)
    {
        dx_ITUz = dxE_ITUz;
    }

    EntityStatus _entityStatus;
    _entityStatus.dxcc      = currentEntity;
    _entityStatus.bandId    = currentBandShown;
    _entityStatus.modeId    = currentModeShown;
    _entityStatus.logId     = currentLog;

    Locator locator;
   //qDebug()<< Q_FUNC_INFO << ": 60 - currentEntity: " << QString::number(currentEntity) ;
    if ( locator.isValidLocator(QSOTabWidget->getDXLocator()))
    {
        dxLocator = QSOTabWidget->getDXLocator();
    }
    else
    {
        dxLocator = world->getLocator(currentEntity);
    }
    //TODO: Look for a way to prevent updating when not needed. i.e. if the prefix is already defined and only suffix is being sent
    //      Maybe a wat could be to send the hostprefix and not the callsign?
   //qDebug()<< Q_FUNC_INFO << " - currentEntity: " << QString::number(currentEntity);
   //qDebug()<< Q_FUNC_INFO << " - c_qrz        : " << _qrz;
    othersTabWidget->setEntityAndPrefix(currentEntity, _qrz);

         //qDebug()<< Q_FUNC_INFO << ": Going to check the DXCC" ;
         //qDebug()<< Q_FUNC_INFO << ": current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) ;
    if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) || (dx_CQz != dxE_CQz) || (dx_ITUz != dxE_ITUz))
    {
         //qDebug()<< Q_FUNC_INFO << ": currentEntity=" << QString::number(currentEntity) << "/previousEntity=" << QString::number(previousEntity)  ;
        previousEntity = currentEntity;
        InValidCharsInPrevCall = false;

        infoLabel2->setText(world->getEntityName(currentEntity));
        infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
        infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
         //qDebug()<< Q_FUNC_INFO << ": 70";

        showStatusOfDXCC(_entityStatus);
        showDXMarathonNeeded(currentEntity, dx_CQz, mainQSOEntryWidget->getDate().year(), currentLog);
        //othersTabWidget->setEntity(currentEntity);
        // we need to update the list of Primary Subdivisions
    }
    else if ((dx_CQz == dxE_CQz) || (dx_ITUz == dxE_ITUz))
    {
         //qDebug()<< Q_FUNC_INFO << ": 101" ;
        //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
        infoLabel2->setText(world->getEntityName(currentEntity));
        infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
    }
    else
    {
             //qDebug()<< Q_FUNC_INFO << ": 110" ;
    }
         //qDebug()<< Q_FUNC_INFO << ": 120" ;
    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
     //qDebug()<< Q_FUNC_INFO << ": 150 - cursorP at the end : "  ;

  if (completeWithPrevious)
  {
     //qDebug()<< Q_FUNC_INFO << ": 160" ;
    completeWithPreviousQSO(_qrz);
  }
   //qDebug()<< Q_FUNC_INFO << ": 170" ;

  if (!modify)
    {
         //qDebug()<< Q_FUNC_INFO << ": 180" ;
        searchWidget->setCallToSearch(_qrz);
          //qDebug() << Q_FUNC_INFO << " qrz.length>2: " << _qrz;
          //qDebug() << Q_FUNC_INFO << " qrzcomActive: " << util->boolToQString (qrzcomActive);
          //qDebug() << Q_FUNC_INFO << " QRZCOMAutoCheckAct: " << util->boolToQString (QRZCOMAutoCheckAct->isChecked());

        if (qrzcomActive && QRZCOMAutoCheckAct->isChecked() && (_qrz.length ()>2))
        {
         //qDebug()<< Q_FUNC_INFO << ": 185 Checking QRZ.com";
            elogQRZcom->checkQRZ(_qrz);
        }
        else
        {
             //qDebug()<< Q_FUNC_INFO << ": 189 NOT checking QRZ.com";
        }
    }
     //qDebug()<< Q_FUNC_INFO << ": 190" ;
   //qrzAutoChanging = false;
    logEvent(Q_FUNC_INFO, "END", Debug);
   //qDebug()<< Q_FUNC_INFO << ": END" ;
}

void MainWindow::setCleaning(const bool _c)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    cleaning = _c;
    mainQSOEntryWidget->setCleaning(cleaning);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotClearButtonClicked(const QString &_func)
{
     //qDebug() << Q_FUNC_INFO << " - Start: " << _func ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    Q_UNUSED(_func);

    bool needToRecover = modify;
    setCleaning(true);
    yearChangedDuringModification = false;
    setModifying(false);

    currentEntity = -1;
    //dateTimeTemp = dateTime;
    modifyingQSOid = -1;

    QSOTabWidget->setRSTToMode(mainQSOEntryWidget->getMode(), readingTheUI);
      //qDebug() << Q_FUNC_INFO << " - 10" ;
    if (currentBand < 0)
    {
        currentBand = defaultBand;
    }
    if (currentMode < 0)
    {
        currentMode = defaultMode;
          //qDebug() << Q_FUNC_INFO << " 12b - currentMode: " << QString::number(currentMode) ;
    }
      //qDebug() << Q_FUNC_INFO << " - 20" ;
    clublogAnswer = -1;
    clublogPrevQSO.clear();
      //qDebug() << Q_FUNC_INFO << " - 11"  ;

    setCleaning(false);
      //qDebug() << Q_FUNC_INFO << " - 25" ;
      //qDebug() << Q_FUNC_INFO << " Log: " << QString::number(currentLog) ;
    setMainWindowTitle();
      //qDebug() <<  Q_FUNC_INFO << " - 27" ;
    if (needToRecover)
    {
          //qDebug() << Q_FUNC_INFO << " - 28" ;
          //qDebug() << Q_FUNC_INFO << ": Recovening the previous status...";
        restoreCurrentQSO(true);
    }
    else
    {
          //qDebug() << Q_FUNC_INFO << " - 40" ;
        clearUIDX();                    // Clear the UI
        qsoInUI.clear();                   // Clear the QSO
        statusBar()->clearMessage();
          //qDebug() << Q_FUNC_INFO << ": NOT recovening the previous status...";
    }
      //qDebug() << Q_FUNC_INFO << " - currentMode = " << QString::number(currentMode) ;
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << Q_FUNC_INFO << " - END" ;
}

void MainWindow::clearUIDX(bool _full)
{
     //qDebug() << Q_FUNC_INFO << " - Start" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    mainQSOEntryWidget->clear();
    QSOTabWidget->clear();
    commentTabWidget->clear(_full);
    infoLabel1->clear();
    infoLabel2->clear();

    eQSLTabWidget->clear();
    QSLTabWidget->clear();
    othersTabWidget->clear(_full);
    infoWidget->clear();
    satTabWidget->clear(_full);
    myDataTabWidget->clear(_full);

    completedWithPreviousName = false;
    completedWithPreviousQTH = false;
    completedWithPreviousLocator = false;

     //qDebug() << Q_FUNC_INFO << " deciding wether to change or not the Freq: " << QString::number(QSOTabWidget->getTXFreq()) ;
    if (QSOTabWidget->getTXFreq()<=0)
    {
          //qDebug() << Q_FUNC_INFO << " Setting TX Freq from: " << QString::number(QSOTabWidget->getTXFreq()) ;
        QSOTabWidget->setTXFreq((dataProxy->getFreqFromBandId(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand()))).toDouble());
          //qDebug() << Q_FUNC_INFO << " Setting TX Freq to: " << QString::number(QSOTabWidget->getTXFreq()) ;
        QSOTabWidget->setRXFreq(QSOTabWidget->getTXFreq());
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << Q_FUNC_INFO << " - END" ;
}


void MainWindow::slotRefreshDXCCWidget()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    dxccStatusWidget->slotRefreshButtonClicked();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
       //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    saveWindowsSize();
    if (maybeSave())
    {
   //qDebug() << Q_FUNC_INFO << " saving needed" ;
        dataProxy->unMarkAllQSO();
        dataProxy->compressDB();
        event->accept();
    }
    else
    {
   //qDebug() << Q_FUNC_INFO << " not saving needed" ;
        event->ignore();
    }
       //qDebug() << Q_FUNC_INFO << " - END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

bool MainWindow::maybeSave()
{
      //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString str = tr("The logfile has been modified.") + "\n" + tr("Do you want to save your changes?");

    if (alwaysADIF)
    {
          //qDebug() << Q_FUNC_INFO << "- Saving" ;
        if (needToSave)
        {
            QMessageBox::StandardButton ret;
            ret = QMessageBox::warning(this, "KLog",
                     str,
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            if (ret == QMessageBox::Save)
            {
                if (useDefaultLogFileName)
                {
                    logEvent(Q_FUNC_INFO, "END-1", Debug);
              //qDebug() << Q_FUNC_INFO << " - Use default file name" ;

                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("KLog - ADIF export"));
                    msgBox.setInformativeText(tr("It is important to export to ADIF and save a copy as a backup."));
                    if (filemanager->adifLogExport(defaultADIFLogFile, currentLog)) // 0 will save ALL the logs)
                    {
                        msgBox.setIcon(QMessageBox::Information);
                        msgBox.setText(tr("Saving the log was done successfully."));
                        msgBox.exec();
                  //qDebug() << Q_FUNC_INFO << " - Log exported" ;
                        return true;
                    }
                    else
                    {
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText(tr("The ADIF export was not properly done."));
                        msgBox.exec();
                  //qDebug() << Q_FUNC_INFO << " - ERROR Log not exported" ;
                        return false;
                    }
                }
                else
                {
                    logEvent(Q_FUNC_INFO, "END-2", Debug);
              //qDebug() << Q_FUNC_INFO << " - Going to import??" ;
                    slotADIFImport();
                    return true;
            //return saveFileAs();
                }
            }
            else if (ret == QMessageBox::Cancel)
            {
                logEvent(Q_FUNC_INFO, "END-3", Debug);
          //qDebug() << Q_FUNC_INFO << "- Cancel" ;
                return false;
            }
        }
        else
        {
      //qDebug() << Q_FUNC_INFO << " - Not needing to save" ;
        }
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << "- Not Checking if needed to save" ;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
    return true;
}

/*
void MainWindow::slotAWAImport()
{
        //qDebug() << Q_FUNC_INFO ;
    fileAwardManager->importNewAwardFile();
        //qDebug() << Q_FUNC_INFO << " - END" ;
}
*/

void MainWindow::createMenusCommon()
{
     //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    fileMenu = menuBar()->addMenu(tr("&File"));

    //awardAddAct = new QAction(tr("Import Award definition file ..."), this);
    //fileMenu->addAction(awardAddAct);
    //connect(awardAddAct , SIGNAL(triggered()), this, SLOT(slotAWAImport()));
    //awardAddAct ->setToolTip(tr("Import an Award file."));

    //TestAct = new QAction(tr("TEST: Advanced search ..."), this);
    //fileMenu->addAction(TestAct);
    //connect(TestAct, SIGNAL(triggered()), this, SLOT(slotTest()));

    ADIFImport = new QAction(tr("&Import from ADIF ..."), this);
    fileMenu->addAction(ADIFImport);
    connect(ADIFImport, SIGNAL(triggered()), this, SLOT(slotADIFImport()));
    ADIFImport->setToolTip(tr("Import an ADIF file into the current log."));

    //LoTWImport = new QAction(tr("&Import from LoTW ..."), this);
    //fileMenu->addAction(LoTWImport);
    //connect(LoTWImport, SIGNAL(triggered()), this, SLOT(slotLoTWImport()));
    //LoTWImport->setToolTip(tr("Import an LoTW file into the current log"));

    fileMenu->addSeparator();
    //fileMenu->addSeparator();

    ADIFExport = new QAction(tr("Export to ADIF ..."), this);
    fileMenu->addAction(ADIFExport);
    //ADIFExport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFExport, SIGNAL(triggered()), this, SLOT(slotADIFExport()));
    ADIFExport->setToolTip(tr("Export the current log to an ADIF logfile."));

    ADIFExportAll = new QAction(tr("Export all logs to ADIF ..."), this);
    fileMenu->addAction(ADIFExportAll);
    //ADIFExport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFExportAll, SIGNAL(triggered()), this, SLOT(slotADIFExportAll()));
    ADIFExportAll->setToolTip(tr("Export ALL the QSOs into one ADIF file, merging QSOs from all the logs."));

    fileMenu->addSeparator();

    printLogAct = new QAction(tr("&Print Log ..."), this);
    fileMenu->addAction(printLogAct);
    printLogAct->setShortcut(Qt::CTRL | Qt::Key_P);
    printLogAct->setToolTip(tr("Print your log."));
    connect(printLogAct, SIGNAL(triggered()), this, SLOT(slotFilePrint()));

    fileMenu->addSeparator();

    klogFolderAct = new QAction(tr("KLog folder"), this);
    fileMenu->addAction(klogFolderAct);
    printLogAct->setToolTip(tr("Opens the data folder of KLog."));
    connect(klogFolderAct, SIGNAL(triggered()), this, SLOT(slotOpenKLogFolder()));

    fileMenu->addSeparator();

    setupAct = new QAction(tr("Settings ..."), this);
    fileMenu->addAction(setupAct);
    //setupAct->setMenuRole(QAction::PreferencesRole);
    connect(setupAct, SIGNAL(triggered()), this, SLOT(slotSetup()));

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    //exitAct->setMenuRole(QAction::QuitRole);
    exitAct->setShortcut(Qt::CTRL | Qt::Key_X);
    //connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(slotFileClose()));

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    fillQsoAct = new QAction(tr("Fill in QSO data"), this);
    toolMenu->addAction(fillQsoAct);
    //fillQsoAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(fillQsoAct, SIGNAL(triggered()), this, SLOT(fillQSOData()));
    fillQsoAct->setToolTip(tr("Go through the log reusing previous QSOs to fill missing information in other QSOs."));

    toolMenu->addSeparator();
    qslToolMenu = toolMenu->addMenu(tr("QSL tools ..."));

    findQSO2QSLAct = new QAction(tr("Find QSO to QSL"), this);
    qslToolMenu->addAction(findQSO2QSLAct);
    connect(findQSO2QSLAct, SIGNAL(triggered()), this, SLOT(slotSearchToolNeededQSLToSend()));
    findQSO2QSLAct->setToolTip(tr("Shows QSOs for which you should send your QSL and request the DX QSL."));

    findRequestedQSLAct = new QAction(tr("Find My-QSLs pending to send"), this);
    qslToolMenu->addAction(findRequestedQSLAct);
    //findQSO2QSLAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(findRequestedQSLAct, SIGNAL(triggered()), this, SLOT(slotToolSearchRequestedQSLToSend()));
    findRequestedQSLAct->setToolTip(tr("Shows the QSOs with pending requests to send QSLs. You should keep this queue empty!"));

    findQSLPendingToReceiveAct = new QAction(tr("Find DX-QSLs pending to receive"), this);
    qslToolMenu->addAction(findQSLPendingToReceiveAct);
    connect(findQSLPendingToReceiveAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLPendingToReceive()));
    findQSLPendingToReceiveAct->setToolTip(tr("Shows DX-QSLs for which requests or QSLs have been sent with no answer."));

    findQSLDXRequestedAct = new QAction(tr("Find requested pending to receive"), this);
    qslToolMenu->addAction(findQSLDXRequestedAct);
    connect(findQSLDXRequestedAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLRequested()));
    findQSLDXRequestedAct->setToolTip(tr("Shows the DX-QSLs that have been requested."));

    toolMenu->addSeparator();
    lotwToolMenu = toolMenu->addMenu(tr("LoTW tools ..."));

    lotwMarkSentQueuedThisLogAct = new QAction(tr("Queue all QSOs from this log to be sent"), this);
    lotwToolMenu->addAction(lotwMarkSentQueuedThisLogAct);
    connect(lotwMarkSentQueuedThisLogAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllQueuedThisLog()));
    lotwMarkSentQueuedThisLogAct->setToolTip(tr("Mark all non-sent QSOs in this log as queued to be uploaded."));

    lotwMarkSentQueuedAct = new QAction(tr("Queue all QSLs to be sent"), this);
    lotwToolMenu ->addAction(lotwMarkSentQueuedAct);
    connect(lotwMarkSentQueuedAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllQueued()));
    lotwMarkSentQueuedAct->setToolTip(tr("Put all the non-sent QSOs in the queue to be uploaded."));

    lotwToolMenu->addSeparator();

    lotwMarkSentYesThisLogAct = new QAction(tr("Mark all queued QSOs from this log as sent"), this);
    lotwToolMenu->addAction(lotwMarkSentYesThisLogAct);
    connect(lotwMarkSentYesThisLogAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllYesThisLog()));
    lotwMarkSentYesThisLogAct->setToolTip(tr("Mark all queued QSOs in this log as sent to LoTW."));

    lotwMarkSentYesAct = new QAction(tr("Mark all queued QSOs as sent"), this);
    lotwToolMenu ->addAction(lotwMarkSentYesAct);
    connect(lotwMarkSentYesAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllYes()));
    lotwMarkSentYesAct->setToolTip(tr("Mark all queued QSOs as sent to LoTW."));

    lotwToolMenu->addSeparator();

    lotwToolMenu ->addAction(lotwCallTQSL);
    connect(lotwCallTQSL, SIGNAL(triggered()), this, SLOT(slotLoTWExport()));
    lotwCallTQSL->setToolTip("Sends the log to LoTW calling TQSL. You will be able to select the Station Callsign and start and end dates.");

    lotwToolMenu->addSeparator();
    lotwUpdateFromLoTWAct = new QAction(tr("Download from LoTW ..."), this);
    lotwToolMenu ->addAction(lotwUpdateFromLoTWAct);
    connect(lotwUpdateFromLoTWAct, SIGNAL(triggered()), this, SLOT(slotLoTWDownload()));
    lotwUpdateFromLoTWAct->setToolTip("Updates your LoTW status from LoTW.");

    lotwFullDownloadFromLoTWAct = new QAction(tr("Download the full log from LoTW ..."), this);
    lotwToolMenu ->addAction(lotwFullDownloadFromLoTWAct);
    connect(lotwFullDownloadFromLoTWAct, SIGNAL(triggered()), this, SLOT(slotLoTWFullDownload()));

    toolMenu->addSeparator();
    clublogToolMenu = toolMenu->addMenu(tr("ClubLog tools ..."));

    clublogLogModifyCurrentLogAct = new QAction(tr("Queue all the QSOs to be uploaded"), this);
    clublogToolMenu->addAction(clublogLogModifyCurrentLogAct);
    connect(clublogLogModifyCurrentLogAct, SIGNAL(triggered()), this, SLOT( slotElogClubLogModifyCurrentLog()));
    clublogLogModifyCurrentLogAct->setToolTip("Mark as modified all the QSO so they can be uploaded again to CLubLog.");

    clublogLogUploadAct = new QAction(tr("Upload the queued QSOs to ClubLog ..."), this);
    clublogToolMenu->addAction(clublogLogUploadAct);
    connect(clublogLogUploadAct, SIGNAL(triggered()), this, SLOT(slotClubLogLogUpload()));
    clublogLogUploadAct->setToolTip("Uploads your log to ClubLog. Please ensure that you have created log for that callsign before uploading.");

    toolMenu->addSeparator();
    eQSLToolMenu = toolMenu->addMenu(tr("eQSL tools ..."));

    eqslLogModifyCurrentLogAct = new QAction(tr("Queue all the QSOs to be uploaded"), this);
    eQSLToolMenu->addAction(eqslLogModifyCurrentLogAct);
    connect(eqslLogModifyCurrentLogAct, SIGNAL(triggered()), this, SLOT( slotElogEQSLModifyCurrentLog()));
    eqslLogModifyCurrentLogAct->setToolTip("Mark as modified all the QSO so they can be uploaded again to eQSL.");

    eqslUploadAct = new QAction(tr("Upload the queued QSOs to eQSL.cc ..."), this);
    eQSLToolMenu->addAction(eqslUploadAct);
    connect(eqslUploadAct, SIGNAL(triggered()), this, SLOT(sloteQSLLogUpload()));
    eqslUploadAct->setToolTip("Uploads your log to eQSL.cc.");

    toolMenu->addSeparator();
    QRZCOMToolMenu = toolMenu->addMenu(tr("QRZ.com tools ..."));

    QRZCOMCheckThisCallAct = new QAction(tr("Check the current callsign in QRZ.com"), this);
    QRZCOMLogModifyCurrentLogAct = new QAction(tr("Queue all the QSO to be uploaded"), this);
    QRZCOMLogUploadAct = new QAction(tr("Upload the queued QSOs to QRZ.com ..."), this);

    QRZCOMToolMenu->addAction(QRZCOMCheckThisCallAct);
    connect(QRZCOMCheckThisCallAct, SIGNAL(triggered()), this, SLOT( slotElogQRZCOMCheckThisCall()));
    QRZCOMCheckThisCallAct->setToolTip("Checks the current callsign in QRZ.com.");

    QRZCOMAutoCheckAct->setText(tr("Check always the current callsign in QRZ.com"));
    QRZCOMToolMenu->addAction(QRZCOMAutoCheckAct);
    connect(QRZCOMAutoCheckAct, SIGNAL(triggered()), this, SLOT( slotElogQRZCOMAutoCheck()));
    QRZCOMAutoCheckAct->setToolTip("Checks always the current callsign in QRZ.com");

    QRZCOMToolMenu->addSeparator();

    QRZCOMToolMenu->addAction(QRZCOMLogModifyCurrentLogAct);
    connect(QRZCOMLogModifyCurrentLogAct, SIGNAL(triggered()), this, SLOT( slotElogQRZCOMModifyCurrentLog()));
    QRZCOMLogModifyCurrentLogAct->setToolTip("Mark as modified all the QSO so they can be uploaded again to QRZ.com.");

    QRZCOMToolMenu->addAction(QRZCOMLogUploadAct);
    connect(QRZCOMLogUploadAct, SIGNAL(triggered()), this, SLOT(slotQRZCOMLogUpload()));
    QRZCOMLogUploadAct->setToolTip("Uploads your log to QRZ.com. Please ensure that you have created log and the API-KEY configured in the setup for that callsign before uploading.");

    toolMenu->addSeparator();

    downloadCTYAct = new QAction (tr("Update cty.csv"), this);
    toolMenu->addAction(downloadCTYAct);
    //downloadCTYAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(downloadCTYAct, SIGNAL(triggered()), this, SLOT(slotUpdateCTYDAT()));
    downloadCTYAct->setToolTip(tr("For updated DX-Entity data, update cty.csv."));

    downloadSATSAct = new QAction (tr("Update Satellite Data"), this);
    toolMenu->addAction(downloadSATSAct);
    connect(downloadSATSAct, SIGNAL(triggered()), this, SLOT(slotUpdateSATSDAT()));
    downloadSATSAct->setToolTip(tr("For updated DX-Entity data, update cty.csv."));

    toolMenu->addSeparator();

    showStatsAct = new QAction (tr("Stats"), this);
    toolMenu->addAction(showStatsAct);
    connect(showStatsAct, SIGNAL(triggered()), this, SLOT(slotShowStats()));
    showStatsAct->setToolTip(tr("Show the statistics of your radio activity."));

    showMapAct = new QAction (tr("Show Map"), this);
    toolMenu->addAction(showMapAct);
    connect(showMapAct, SIGNAL(triggered()), this, SLOT(slotShowMap()));
    showMapAct->setToolTip(tr("Show the statistics of your radio activity."));

    //dxClusterAssistantAct = new QAction (tr("DXCluster Assistant"), this);
    //toolMenu->addAction(dxClusterAssistantAct);
    //connect(dxClusterAssistantAct, SIGNAL(triggered()), this, SLOT(slotShowDXClusterAssistant()));
    //dxClusterAssistantAct->setToolTip(tr("Show the statistics of your radio activity."));


     //qDebug() << "MainWindow::createMenusCommon before" ;
    //toolMenu->addSeparator();
    //showRotatorAct = new QAction (tr("Rotator"), this);
    //toolMenu->addAction(showRotatorAct);
    //connect(showRotatorAct, SIGNAL(triggered()), this, SLOT(slotRotatorShow()));
    //showRotatorAct->setToolTip(tr("Show the rotator controller."));
      //qDebug() << "MainWindow::createMenusCommon after" ;

    //showWorldMapAct = new QAction(tr("CQ zones world map"), this);
    //toolMenu->addAction(showWorldMapAct);
    //connect(showWorldMapAct, SIGNAL(triggered()), this, SLOT(slotWorldMapShow()));
    //showWorldMapAct->setToolTip(tr("Show a world map with your radio activity."));

    //toolMenu->addSeparator();

    //setupMenu = menuBar()->addMenu(tr("Setup"));



    //TODO: To be added once the help dialog has been implemented
    helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addSeparator();
    helpAct= new QAction(tr("Online manual (F1) ..."), this);
    helpMenu->addAction(helpAct);
    //helpAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(helpAct, SIGNAL(triggered()), this, SLOT(slotOpenWiki()));

    tipsAct = new QAction(tr("&Tips ..."), this);
    helpMenu->addAction(tipsAct);
    //tipsAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(tipsAct, SIGNAL(triggered()), this, SLOT(slotTipsAction()));
    helpMenu->addSeparator();

    debugAct = new QAction(tr("&Debug ..."), this);
    helpMenu->addAction(debugAct);
    //debugAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(debugAct, SIGNAL(triggered()), this, SLOT(slotDebugAction()));

    aboutAct = new QAction(tr("&About ..."), this);
    helpMenu->addAction(aboutAct);
    //aboutAct->setMenuRole(QAction::AboutRole);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotHelpAboutAction()));
    //connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotLoTWTest()) );
    #if defined(Q_OS_MACOS) // It seems that on MAC if I try to show both, only the last one (or Qt) is shown
    #else           // I can define the setMenuRole but then no standard locations are used on macOS
    aboutQtAct = new QAction(tr("About Qt ..."), this);
    helpMenu->addAction(aboutQtAct);
    //aboutQtAct->setMenuRole(QAction::AboutQtRole);
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(slotAboutQt()));
    logEvent(Q_FUNC_INFO, "END", Debug);
    #endif
    helpMenu->addSeparator();

    updateAct = new QAction(tr("Check updates ..."), this);
    helpMenu->addAction(updateAct);
    //updateAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(updateAct, SIGNAL(triggered()), this, SLOT(slotHelpCheckUpdatesAction()));
 }

void MainWindow::slotDebugAction()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    showKLogLogWidget->show();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

/*
void MainWindow::slotTest()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
      //qDebug() << Q_FUNC_INFO ;
    //showKLogLogWidget->setWindowModality(false);
    showKLogLogWidget->show();
    //advancedSearchWidget->show();
      //qDebug() << Q_FUNC_INFO << " - END ";
    logEvent(Q_FUNC_INFO, "END", Debug);
}
*/

void MainWindow::slotSearchToolNeededQSLToSend()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    slotShowSearchWidget();
    searchWidget->searchToolNeededQSLToSend();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotToolSearchRequestedQSLToSend()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    searchWidget->slotToolSearchRequestedQSLToSend();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotToolSearchNeededQSLPendingToReceive()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    searchWidget->slotToolSearchNeededQSLPendingToReceive();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotToolSearchNeededQSLRequested()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    searchWidget->slotToolSearchNeededQSLRequested();
    logEvent(Q_FUNC_INFO, "END", Debug);
}


void MainWindow::slotToolLoTWMarkAllQueuedThisLog()
{
      //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());

    QMessageBox msgConfirm;
    msgConfirm.setIcon(QMessageBox::Question);
    msgConfirm.setWindowTitle(tr("KLog - LoTW"));
    msgConfirm.setText(tr("Do you really want to mark ALL the QSOs of this log to be UPLOADED? Must be done ONLY IF THIS IS YOUR FIRST TIME uploading these QSOs to LoTW."));
    msgConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgConfirm.setDefaultButton(QMessageBox::No);
    int i = msgConfirm.exec();

    if (i == QMessageBox::Yes)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - LoTW"));
        if(dataProxy->lotwSentQueue(mainQSOEntryWidget->getDate(), currentLog))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("All pending QSOs of this log has been marked as queued for LoTW!") + "\n\n" + tr("Now you can upload them to LoTW."));
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("There was a problem to mark all pending QSOs of this log as queued for LoTW!") );
        }
        msgBox.exec();
        logEvent(Q_FUNC_INFO, "END", Debug);
    }
}

/*
 void MainWindow::slotLoTWTest()
{
         //qDebug() << "MainWindow::slotLoTWTest: "  ;
     slotLoTWDownloadedFileProcess("lotwimport.adi");
         //qDebug() << "MainWindow::slotLoTWTest - END"  ;
}
*/

void MainWindow::slotLoTWDownloadedFileProcess(const QString &_fn)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
       //qDebug() << Q_FUNC_INFO << _fn ;
    //QList<int> a;
    //a.clear();
    //a.append(filemanager->adifLoTWReadLog2(_fn, currentLog));
    int added_qsos = filemanager->adifLoTWReadLog2(_fn, currentLog);
    QString aux;
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - LoTW"));

    if (added_qsos>0)
    {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Your log has been updated with the LoTW downloaded QSOs."));
        aux = QString(tr("KLog has updated %1 QSOs from LoTW.")).arg(added_qsos);
        msgBox.setInformativeText(aux);
        msgBox.exec();
        logWindow->refresh();
        dxccStatusWidget->refresh();
        //TODO: Add the QSOs to the widget and show showAdifImportWidget->show();
    }
    else
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Your log has not been updated."));
        aux = QString(tr("No QSO was updated with the data coming from LoTW. This may be because of errors in the logfile or simply because your log was already updated."));
        msgBox.setInformativeText(aux);
        msgBox.exec();
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotToolLoTWMarkAllQueued()
{
      //qDebug() << Q_FUNC_INFO  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());

    QMessageBox msgConfirm;
    msgConfirm.setIcon(QMessageBox::Question);
    msgConfirm.setWindowTitle(tr("KLog - LoTW"));
    msgConfirm.setText(tr("Do you really want to mark ALL pending QSOs to be UPLOADED? Must be done ONLY IF THIS IS YOUR FIRST TIME uploading these QSOs to LoTW."));
    msgConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgConfirm.setDefaultButton(QMessageBox::No);
    int i = msgConfirm.exec();

    if (i == QMessageBox::Yes)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - LoTW"));

        if (dataProxy->lotwSentQueue(mainQSOEntryWidget->getDate(), -1))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("All pending QSOs has been marked as queued for LoTW!") + "\n\n" +  tr("Now you can upload them to LoTW."));
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("There was a problem to mark all pending QSOs as queued for LoTW!") );
        }
        msgBox.exec();
        logEvent(Q_FUNC_INFO, "END", Debug);
    }
}

bool MainWindow::callTQSL(const QString &_filename, const QString &_call)
{ //https://lotw.arrl.org/lotw-help/cmdline/
    logEvent(Q_FUNC_INFO, "Start", Debug);
        //qDebug() << Q_FUNC_INFO << lotwTQSLpath ;

    QStringList arguments;
    arguments.clear();
    //arguments << "--action=compliant" << QString("-c %1").arg(_call) << "-d" << "-u"  << "-x" << _filename;
    arguments  << QString("-c %1").arg(_call) << "-u" << _filename;
    int ok = -1;
    QString msg;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - TQSL"));

    if (!QFile::exists(lotwTQSLpath))
    {
        msg = tr("TQSL is not installed or KLog can't find it. Please check the configuration.");
        ok = 6; // A positive value here will return false, an error below.
    }
    else
    {
        ok = QProcess::execute(lotwTQSLpath, arguments);

    //qDebug() << Q_FUNC_INFO << " -ok: " << QString::number(ok) ;

        switch (ok)
        {
        case 0: // success: all qsos submitted were signed and saved or signed and uploaded
              //qDebug() << Q_FUNC_INFO << ":  0"  ;
    //msg = tr("All the QSOs were signed and uploaded with no error.");
    //msg = tr("TQSL finished with no error.");
    //msgBox.setIcon(QMessageBox::Information);
        break;
        case 1: // cancelled by user
              //qDebug() << Q_FUNC_INFO << ": 1"  ;
            msg = tr("Error #1: The process was cancelled by the user or TQSL was not configured. No QSOs were uploaded.");
        break;
        case 2: // rejected by LoTW
              //qDebug() << Q_FUNC_INFO << ": 2"  ;
            msg = tr("Error #2: Upload was rejected by LoTW, please check your data.");
            break;
        case 3: // unexpected response from TQSL server
              //qDebug() << Q_FUNC_INFO << "L: 3"  ;
            msg = tr("Error #3: The TQSL server returned an unexpected response.");
            break;
        case 4: // TQSL error
              //qDebug() << Q_FUNC_INFO << ": 4"  ;
            msg = tr("Error #4: There was a TQSL error.");
            break;
        case 5: // TQSLlib error
              //qDebug() << Q_FUNC_INFO << ": 5"  ;
            msg = tr("Error #5: There was a TQSLLib error.");
            break;
        case 6: // unable to open input file
              //qDebug() << Q_FUNC_INFO << ": 6"  ;
            msg = tr("Error #6: It was not possible to open the input file.");
            break;
        case 7: // unable to open output file
              //qDebug() << Q_FUNC_INFO << "L: 7"  ;
            msg = tr("Error #7: It was not possible to open the ouput file.");
            break;
        case 8: // No QSOs were processed since some QSOs were duplicates or out of date range
              //qDebug() << Q_FUNC_INFO << ": 8"  ;
            msg = tr("Error #8: No QSOs were processed since some QSOs were duplicates or out of date range.");
            break;
        case 9: // Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range
              //qDebug() << Q_FUNC_INFO << ": 9"  ;
            msg = tr("Error #9: Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range.");
            break;
        case 10: // command syntax error
              //qDebug() << Q_FUNC_INFO << ": 10"  ;
            msg = tr("Error #10: Command syntax error. KLog sent a bad syntax command.");
            break;
        case 11: // LoTW Connection error (no network or LoTW is unreachable)
              //qDebug() << Q_FUNC_INFO << ": 11"  ;
            msg = tr("Error #11: LoTW Connection error (no network or LoTW is unreachable).");
            break;
        default:
            msg = tr("Error #00: Unexpected error. Please contact the development team.");
            return false;

        // should never be reached
        }
    }

    if (ok>0)
    {
        msgBox.setText(msg);
        msgBox.exec();
        return false;
    }
    else
    {
        return true;
    }
    //logEvent(Q_FUNC_INFO, "END", Debug);
}

QString MainWindow::selectStationCallsign()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString stationCallToUse = QString();
    QStringList stationCallSigns;
    stationCallSigns.clear();
    stationCallSigns << "NONE";
    stationCallSigns << dataProxy->getStationCallSignsFromLog(-1);
    //bool callsignTyped = false;

    if (stationCallSigns.length()>1)
    {
        QString msg = QString(tr("The log that you have selected contains more than just one station callsign.") + "\n\n" + tr("Please select the station callsign you want to mark as sent to LoTW:"));

        bool ok;
        stationCallToUse = QInputDialog::getItem(this, tr("Station Callsign:"),
                                             msg, stationCallSigns, 0, false, &ok);
        Callsign callsign(stationCallToUse);

        if (ok && callsign.isValid())
        //if (ok && !stationCallToUse.isEmpty())
        {
            logEvent(Q_FUNC_INFO, "END-1", Debug);
            return stationCallToUse;
        }
        else
        {
            stationCallToUse = (QInputDialog::getText(this, tr("Define Station Callsign"),
                                                     tr("You have selected no callsign. KLog will complete the QSOs without a station callsign defined and those with the callsign you are entering here.") + "\n\n" + tr("Enter the station callsign to use for this log or leave it empty for QSO without station callsign defined:"), QLineEdit::Normal,
                                                     "", &ok)).toUpper();
             if (ok)
             {
                callsign(stationCallToUse);
                if (callsign.isValid())
                 {
                    logEvent(Q_FUNC_INFO, "END-2", Debug);
                    return stationCallToUse;
                 }
             }
             else
             {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setWindowTitle(tr("KLog - No station selected"));
                 QString aux = QString(tr("No station callsign has been selected and therefore no log will be marked") );
                 msgBox.setText(aux);
                 msgBox.setStandardButtons(QMessageBox::Ok);
                 int ret = msgBox.exec();
                 switch (ret) {
                   case QMessageBox::Ok:
               // Ok was clicked
                 //return ;
                       break;
                   default:
                 // should never be reached
                       break;
                 }
             }
        }
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
    return QString();
}

void MainWindow::slotToolLoTWMarkAllYesThisLog()
{
          //qDebug() << Q_FUNC_INFO ;
        //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - LoTW"));
        logEvent(Q_FUNC_INFO, "Start", Debug);
        if(dataProxy->lotwSentYes(mainQSOEntryWidget->getDate(), currentLog, "ALL"))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("All queued QSOs of this log has been marked as sent to LoTW!")  );
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("There was a problem to mark all queued QSOs of this log as sent to LoTW!") );
        }
        msgBox.exec();
        logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotToolLoTWMarkAllYes()
{
       //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QString stationCallToUse = selectStationCallsign();

    //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - LoTW"));

    if (dataProxy->lotwSentYes(mainQSOEntryWidget->getDate(), -1, stationCallToUse))
    {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("All queued QSOs has been marked as sent to LoTW!") );
    }
    else
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("There was a problem to mark all queued QSOs as sent to LoTW!") );
    }
    msgBox.exec();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotReceiveQSOListToShowFromFile(QStringList _qs)
{
   //qDebug() << Q_FUNC_INFO << QString::number(_qs.length())  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    // Receiving:  modifiedQSOList << _call << _date + "-" +_time << _band << _mode << _qslrdate;
    // Must send QRZ-DX, Date-Time(yyyyMMdd-hhmmss), Band, Mode
    if (_qs.length()!=4)
    {
    //qDebug() << Q_FUNC_INFO << " - NO valid qso list received - length: " << QString::number(_qs.length()) ;
        return;
    }
    Callsign callsign(_qs.at(0));
    if (!callsign.isValid())
    {
    //qDebug() << Q_FUNC_INFO << " - NO valid QRZ received - " << _qs.at(0) ;
        return;
    }

    if (dataProxy->getIdFromBandName(_qs.at(2))<0)
    {
   //qDebug() << Q_FUNC_INFO << " - NO valid BAND received"<< _qs.at(2) ;
        return;
    }
    if (dataProxy->getIdFromModeName(_qs.at(3))<0)
    {
    //qDebug() << Q_FUNC_INFO << " - NO valid Mode received" << _qs.at(3);
        return;
    }
    if (!util->isValidDateTimeFromString(_qs.at(1)))

    {
    //qDebug() << Q_FUNC_INFO << " No valid date" ;
            return;
    }
        //qDebug() << Q_FUNC_INFO << " ALL OK" ;
    //QStringList qsoToSend;
    //qsoToSend.clear();
    //qsoToSend << _qs.at(0) << _qs.at(1) << _qs.at(2) << _qs.at(3);
    showAdifImportWidget->addQSOToTheList(_qs);

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotAboutQt()
{
      //qDebug() << Q_FUNC_INFO  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QMessageBox::aboutQt(this,tr("About ..."));
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotHelpAboutAction()
{
          //qDebug() << Q_FUNC_INFO ;
   // QMessageBox::about(this, tr("About KLog"),
    //            tr("<b>KLog</b> "
   //                "Find the last release at https://jaime.robles.es/klog."));

    logEvent(Q_FUNC_INFO, "Start", Debug);
    aboutDialog->exec();
    logEvent(Q_FUNC_INFO, "END", Debug);
    //helpAboutDialog->exec();
}
void MainWindow::slotTipsAction()
{
      //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    tipsDialog->exec();


    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotHelpCheckUpdatesAction()
{
      //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    callingUpdate = true;
    softUpdate->addCall(stationCallsign);
    softUpdate->needToUpdate(true);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowSoftUpdateResults(const bool _b)
{
         //qDebug() << Q_FUNC_INFO  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (_b == true)
    {
         //qDebug() << Q_FUNC_INFO << " _b = TRUE " ;
    }
    else
    {
             //qDebug() << Q_FUNC_INFO << " _b = FALSE " ;
    }

    if (callingUpdate == true)
    {
        if (_b == false)
        {
                 //qDebug() << Q_FUNC_INFO << " UPDATE NOT NEEDED" ;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog - Update checking result"));
            msgBox.setText(tr("Congratulations!") + "\n\n" + tr("You already have the latest version."));
            msgBox.exec();
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << ": UPDATE NEEDED" ;
        }
    }
    callingUpdate = false;
    logEvent(Q_FUNC_INFO, "END", Debug);
}



void MainWindow::slotLogWinShow()
{
          //qDebug() << Q_FUNC_INFO  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (!(logWindow->isVisible()) )
    {
        logWinAct->setChecked ( true );
        logWindow->show();
    }else
    {
        logWinAct->setChecked ( false );
        logWindow->hide();
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotSetup(const int _page)
{
       //qDebug() <<  Q_FUNC_INFO << QString::number(_page)  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    configured = false;
      //qDebug() << Q_FUNC_INFO << " - 010"  ;
    backupCurrentQSO();
      //qDebug() << Q_FUNC_INFO << " - 020"  ;
    openSetup(_page);
      //qDebug() << Q_FUNC_INFO << " - END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}


void MainWindow::openSetup(const int _page)
{
     //qDebug() << Q_FUNC_INFO << ": " << QString::number(_page)  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //int result = -1;
     //qDebug() << Q_FUNC_INFO << " - 000 - " << (QTime::currentTime()).toString("HH:mm:ss");
    hamlib->stop();
     //qDebug() << Q_FUNC_INFO << " - 001 - " << (QTime::currentTime()).toString("HH:mm:ss");
    if (!needToEnd)
    {
         //qDebug() << Q_FUNC_INFO << " - 010 - " << (QTime::currentTime()).toString("HH:mm:ss");
        logEvent(Q_FUNC_INFO, "Just before setData", Devel);
         //qDebug() << "MainWindow::openSetup - Just before setupDialog->exec-1"  ;
        if (upAndRunning)
        {
         //qDebug() << Q_FUNC_INFO << " - 011 - " << (QTime::currentTime()).toString("HH:mm:ss");
            setupDialog->setData(softwareVersion, _page, !configured);
         //qDebug() << Q_FUNC_INFO << " - 012 - " << (QTime::currentTime()).toString("HH:mm:ss");
        }
        else
        {
             //qDebug() << Q_FUNC_INFO << " - 013 - " << (QTime::currentTime()).toString("HH:mm:ss");
            setupDialog->setData(softwareVersion, 0, !configured);
             //qDebug() << Q_FUNC_INFO << " - 014 - " << (QTime::currentTime()).toString("HH:mm:ss");
        }
        if ( (!configured) || (itIsANewversion) )
        {
         //qDebug() << Q_FUNC_INFO << " - 015 - " << (QTime::currentTime()).toString("HH:mm:ss");
            logEvent(Q_FUNC_INFO, "Just before SetupDialog->exec", Devel);
            itIsANewversion = false;
    //setupDialog->exec();

            setupDialog->setModal(true);
             //qDebug() << Q_FUNC_INFO << " - 016 - " << (QTime::currentTime()).toString("HH:mm:ss");
            setupDialog->show();
             //qDebug() << Q_FUNC_INFO << " - 017 - " << (QTime::currentTime()).toString("HH:mm:ss");
    // move part of this code to slotSetupDialogFinished
            logEvent(Q_FUNC_INFO, "Just after setupDialog->show", Devel);
             //qDebug() << "MainWindow::openSetup - Just after setupDialog->show" ;
        }
        else
        {
             //qDebug() << Q_FUNC_INFO << " - 020 - " << (QTime::currentTime()).toString("HH:mm:ss");
            logEvent(Q_FUNC_INFO, "No setupDialog->exec needed", Devel);
             //qDebug() << "MainWindow::openSetup - No setupDialog->show needed"  ;
        }
    }
     //qDebug() << Q_FUNC_INFO << " - 050 - " << (QTime::currentTime()).toString("HH:mm:ss");
       //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotSetupDialogFinished (const int _s)
{
      //qDebug() << Q_FUNC_INFO << ": " <<  QString::number(_s) << " - " << (QTime::currentTime()).toString ("HH:mm:ss");
    if (needToEnd)
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    //bool restoreQSOConfig = false;
    if (_s == QDialog::Accepted)
    {
         //qDebug() << Q_FUNC_INFO << " - QDialog::Accepted - " << (QTime::currentTime()).toString ("HH:mm:ss");
        logEvent(Q_FUNC_INFO, "Just before loadSettings", Debug);
        //readConfigData();
        configured = loadSettings ();
        applySettings ();
          //qDebug() << Q_FUNC_INFO << " - 010 - " << (QTime::currentTime()).toString ("HH:mm:ss");
        reconfigureDXMarathonUI(manageDxMarathon);
        logEvent(Q_FUNC_INFO, "Just after loadSettings", Debug);
          //qDebug() << "MainWindow::slotSetupDialogFinished: logmodel to be created-2" ;
        logEvent(Q_FUNC_INFO, "logmodel to be created-2", Debug);
          //qDebug() << Q_FUNC_INFO << " - 011 - " << (QTime::currentTime()).toString ("HH:mm:ss");
        logWindow->createlogPanel(currentLog);
          //qDebug() << Q_FUNC_INFO << " - 012 - " << (QTime::currentTime()).toString ("HH:mm:ss");
        logEvent(Q_FUNC_INFO, "logmodel has been created-2", Debug);
        //defineStationCallsign(stationCallsign);
          //qDebug() << Q_FUNC_INFO << " - 013 - " << (QTime::currentTime()).toString ("HH:mm:ss");
        logEvent(Q_FUNC_INFO, "before db->reConnect", Debug);
   //qDebug() << "MainWindow::openSetup: before db->reConnect" ;
        dataProxy->reconnectDB();

          //qDebug() << Q_FUNC_INFO << " - 014 - " << (QTime::currentTime()).toString ("HH:mm:ss");
        logEvent(Q_FUNC_INFO, "after db->reConnect", Debug);
          //qDebug() << "MainWindow::openSetup: after db->reConnect" ;
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << " - !QDialog::Accepted";
          //qDebug() << Q_FUNC_INFO << " - 019 - " << (QTime::currentTime()).toString ("HH:mm:ss");
    }
      //qDebug() << Q_FUNC_INFO << " - 020 - " << (QTime::currentTime()).toString ("HH:mm:ss");
    if (qsoInUI.getBackup())
    {
          //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss") << Q_FUNC_INFO << " - 021 - ";
          //qDebug() << Q_FUNC_INFO << ": Restoring..." ;
        restoreCurrentQSO (QDialog::Accepted);
          //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss") << Q_FUNC_INFO << " - 022 - " ;
    }
    else
    {
          //qDebug() << "MainWindow::slotSetupDialogFinished: NO Restoring qsoInUI..." ;
          //qDebug()<< (QTime::currentTime()).toString ("HH:mm:ss") << Q_FUNC_INFO << " - 023 - ";
    }
      //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss") << Q_FUNC_INFO << " - 030 - " ;
    hamlibActive = setHamlib(hamlibActive);

      //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss") << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
}

bool MainWindow::slotOpenKLogFolder()
{
      //qDebug() << "MainWindow::slotOpenKLogFolder: " << util->getHomeDir() ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QString _aux = "<ul><li><a href=file://" + util->getHomeDir() + ">file://" + util->getHomeDir() + "</a></li>" +
                    "<li><a href=file://" + util->getKLogDBFile() + ">file://" + util->getKLogDBFile() + "</a></i></ul>" ;
    QString _text = tr("You can find the KLog data folder here: ") + _aux;

    QMessageBox::information(this, tr("KLog"),
                                   _text,
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
       //qDebug() << "MainWindow::slotOpenKLogFolder: END"  ;
    logEvent(Q_FUNC_INFO, "END", Debug);
    return true;
}


void MainWindow::slotUpdateStatusBar(const QString &statusm)
{
      //qDebug() << "MainWindow::slotUpdateStatusBar: " << statusm  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    statusBar()->showMessage(statusm, 2000);
    logEvent(Q_FUNC_INFO, "END", Debug);
}


//bool MainWindow::readCtyFile()
//{
//    logEvent(Q_FUNC_INFO, "Start", Debug);
//    logEvent(Q_FUNC_INFO, "END", Debug);
//    return false;
//}

void MainWindow::slotDoubleClickLog(const int _qsoID)
{
      //qDebug() << Q_FUNC_INFO << ": QSOid: " << QString::number(_qsoID) ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //int row = _qsoID.row();

    qsoToEdit(_qsoID);
    logEvent(Q_FUNC_INFO, "END", Debug);

    //TODO: To be added to the logWindow and create an action that emist the QSO id to be edited
}

bool MainWindow::setUDPServer(const bool _b)
{
      //qDebug() << Q_FUNC_INFO << ": upAndRunning: " << util->boolToQString (upAndRunning) ;
      //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (_b) ;
    QString errorMSG, aux;
    if (_b)
    {
          //qDebug() << Q_FUNC_INFO << ": Starting the server" ;
        if (!UDPLogServer->isStarted())
        {
      //qDebug() << Q_FUNC_INFO << ": Server is not started, starting...";
            if (!UDPLogServer->start())
            {
          //qDebug() << Q_FUNC_INFO << ": Server could not be started, exiting...";
                errorMSG =  tr("start");
                aux = tr("UDP Server error\nThe UDP server failed to %1.", "start or stop").arg(errorMSG);
                showErrorDialog->setText(aux);
                showErrorDialog->exec();
                return false;
            }
            else
            {
          //qDebug() << Q_FUNC_INFO << ": UDP Log server started!" << QTime::currentTime().toString("hh:mm:ss") ;
            }
            return true;
        }
        else
        {
            return true;
      //qDebug() << Q_FUNC_INFO << ": UDP Log server already started no need to restart!" << QTime::currentTime().toString("hh:mm:ss") ;
        }
    }
    else
    {
          //qDebug() << Q_FUNC_INFO << ": UDPServerStart FALSE" << QTime::currentTime().toString("hh:mm:ss") ;
        if (UDPLogServer->isStarted())
        {
      //qDebug() << Q_FUNC_INFO << ": Server is startted and should be Disabled";
            if (!UDPLogServer->stop())
            {
          //qDebug() << Q_FUNC_INFO << ": Server should be disabled and I can't stop it";
                errorMSG =  tr("stop");
                aux = tr("UDP Server error\nThe UDP server failed to %1.", "start or stop").arg(errorMSG);
                showErrorDialog->setText(aux);
                showErrorDialog->exec();
                return UDPLogServer->isStarted ();
            }
            else
            {
          //qDebug() << Q_FUNC_INFO << ": UDP Log server stopped!" << QTime::currentTime().toString("hh:mm:ss") ;
                return false;
            }
        }
        else
        {
      //qDebug() << Q_FUNC_INFO << ": UDP Log server already stopped no need to restop!" ;
            return false;
        }
    }
      //qDebug() << Q_FUNC_INFO << ": It should not reach this, default to false";
    return false;
}

bool MainWindow::setHamlib(const bool _b)
{
      //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss - ") << Q_FUNC_INFO << ": " << util->boolToQString (_b) ;

    if (!upAndRunning)
    {
          //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss - ") << Q_FUNC_INFO << ": Hamlib upAndRunning FALSE";
        return false;
    }
    if (_b)
    {
          //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss - ") << Q_FUNC_INFO << ": Hamlib active";
        hamlib->init(true);
          //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss - ")  << Q_FUNC_INFO << ": After Hamlib active";
        return hamlib->readRadio(true); // Forcing the radio update
    }
    else
    {
          //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss - ") << Q_FUNC_INFO << ": Hamlib NOT active";
        hamlib->init(false);
          //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss - ") << Q_FUNC_INFO << ": After Hamlib NOT active";
        return false;
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
      //qDebug() << Q_FUNC_INFO ;
    (void)event;
    setWindowSize(windowSize);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
      //qDebug() << "MainWindow::keyPressEvent: "  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    switch (event->key()) {
    case Qt::Key_Return:
        // ENTER PRESSED
        slotQRZReturnPressed();
          //qDebug() << "MainWindow::keyPressEvent: RETURN"  ;
        break;
    case Qt::Key_Enter:
        // ENTER PRESSED
          //qDebug() << "MainWindow::keyPressEvent: ENTER"  ;
        slotQRZReturnPressed();
        break;
    case Qt::Key_F1:
            slotOpenWiki();
        break;
    case Qt::Key_F4:
    //qDebug << "MainWindow::keyPressEvent: F4"  ;
            mainQSOEntryWidget->toggleRealTime ();
        break;

    default:
        //QFrame::keyPressEvent(event);
        break;
    }

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotOpenWiki()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
      //qDebug() << Q_FUNC_INFO ;
    QDesktopServices::openUrl(QUrl("https://github.com/ea4k/klog/wiki/"));
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default)
{
   //qDebug() << Q_FUNC_INFO <<  "Confirmed: " << _newOne.name(QColor::HexRgb) << " /  Needed: " << _needed.name(QColor::HexRgb) ;
    //                             " / Worked: " << _worked.name(QColor::HexRgb) << " / Confirmed: " << _confirmed.name(QColor::HexRgb) <<
    //                            " / Default: " << _default.name(QColor::HexRgb);
    searchWidget->setColors(_newOne, _needed, _worked, _confirmed, _default);
    awards.setColors (_newOne, _needed, _worked, _confirmed, _default);
    mapWindow->setColors (_worked, _confirmed, _default);
    dxClusterWidget->setColors (_newOne, _needed, _worked, _confirmed, _default);
    infoWidget->setColors(_newOne, _needed, _worked, _confirmed, _default);
    dxccStatusWidget->setColors(_newOne, _needed, _worked, _confirmed, _default);
}

bool MainWindow::applySettings()
{
     //qDebug() << Q_FUNC_INFO << " - Start";

    if ((useDefaultLogFileName) && (!defaultADIFLogFile.isEmpty()))
    {
        useDefaultLogFileName = true;
    }
    else
    {
        useDefaultLogFileName = false;
    }

    checkIfNewBandOrMode ();
    infoWidget->setImperialSystem(imperialSystem);
    infoLabel2->setText(world->getEntityName(currentEntity));
    infoWidget->showEntityInfo(currentEntity);
    dxClusterWidget->setDXClusterSpotConfig(dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY );
    setMainWindowTitle();
    dxClusterWidget->setMyQRZ(stationCallsign);
    elogClublog->setDefaultCallsign (stationCallsign);

    if (qrzcomActive)
    {
        //elogQRZcom->setCredentials(qrzcomUser, qrzcomPass);
        elogQRZcom->login();
    }
    util->setVersion(softwareVersion);

    searchWidget->setVersion(softwareVersion);
    searchWidget->setCurrentLog(currentLog);
    infoWidget->setCurrentLog(currentLog);


    satTabWidget->refreshData();
    adifLoTWExportWidget->setLogNumber (currentLog);
    //setWindowSize (windowSize);
    if (upAndRunning)
    {
        startServices();
    }


      //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
    return true;
}

void MainWindow::startServices()
{
      //qDebug() << QTime::currentTime().toString("hh:mm:ss - ") ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    hamlibActive = setHamlib(hamlibActive);
    setUDPServer(UDPServerStart);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::checkIfNewBandOrMode()
{//Checks the log to see if there is a QSO with a band/mode
//that is not currently selected as active
     //qDebug() << "MainWindow::checkIfNewBandOrMode - START " << QTime::currentTime().toString("hh:mm:ss") ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString currentBand = mainQSOEntryWidget->getBand();
    QString currentMode = mainQSOEntryWidget->getMode();
    setupDialog->checkIfNewBandOrMode(); // Update the Setup dialog with new bands or modes
      //qDebug() << "MainWindow::checkIfNewBandOrMode - 1 " << QTime::currentTime().toString("hh:mm:ss") ;
    QStringList bandsInLog = dataProxy->getBandsInLog(currentLog);
      //qDebug() << "MainWindow::checkIfNewBandOrMode - 2 " << QTime::currentTime().toString("hh:mm:ss") ;
    QStringList modesInLog = dataProxy->getModesInLog(currentLog);
      //qDebug() << "MainWindow::checkIfNewBandOrMode - 3 " << QTime::currentTime().toString("hh:mm:ss") ;
    QStringList qsTemp;
    qsTemp.clear();
      //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.1 " << QTime::currentTime().toString("hh:mm:ss") ;
    bands << bandsInLog;
    bands.removeDuplicates();
      //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.2 " << QTime::currentTime().toString("hh:mm:ss") ;
    qsTemp << dataProxy->sortBandNamesBottonUp(bands);
      //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.3 " << QTime::currentTime().toString("hh:mm:ss") ;
    bands.clear();
    bands = qsTemp;

    modes << modesInLog;
    modes.removeDuplicates();
    modes.sort();

      //qDebug() << "MainWindow::checkIfNewBandOrMode - bands -" << QString::number(bands.length()) << " - " << QTime::currentTime().toString("hh:mm:ss") ;
    mainQSOEntryWidget->setBands(bands);
    satTabWidget->addBands(bands);
    mapWindow->setBands(bands);

      //qDebug() << "MainWindow::checkIfNewBandOrMode - modes -" << QString::number(modes.length()) << " - " << QTime::currentTime().toString("hh:mm:ss") ;
    mainQSOEntryWidget->setModes(modes);
    mapWindow->setModes(modes);


     //qDebug() << "MainWindow::checkIfNewBandOrMode - setting bands" << QTime::currentTime().toString("hh:mm:ss") ;
    logEvent(Q_FUNC_INFO, "Setting bands", Debug);
    dxccStatusWidget->setBands(Q_FUNC_INFO, bands, true);

     //qDebug() << "MainWindow::checkIfNewBandOrMode - currentBand: " << currentBand << QTime::currentTime().toString("hh:mm:ss") ;
    if (bands.contains(currentBand))
    {
        mainQSOEntryWidget->setBand(currentBand);
    }
    else
    {
        selectDefaultBand();
    }

      //qDebug() << "MainWindow::checkIfNewBandOrMode-99 " << QTime::currentTime().toString("hh:mm:ss") ;
    if (modes.contains(currentMode))
    {
        mainQSOEntryWidget->setMode(currentMode);
    }
    else
    {
        selectDefaultMode();
    }


    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::checkIfNewBandOrMode END " << QTime::currentTime().toString("hh:mm:ss") ;
}

void MainWindow::selectDefaultBand()
{
      //qDebug() << "MainWindow::selectDefaultBand" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!upAndRunning)
    {
        return;
    }
    QString aux;
    aux = QString();
    defaultBand = dataProxy->getMostUsedBand(currentLog);
    if (defaultBand<1)
    {
        defaultBand = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand(0));
        //defaultBand = dataProxy->getIdFromBandName(bandComboBox->itemText(1));
    }

        aux = dataProxy->getNameFromBandId(defaultBand);
        mainQSOEntryWidget->setBand(aux);
        //bandComboBox->setCurrentIndex(bandComboBox->findText(aux));
        logEvent(Q_FUNC_INFO, "END", Debug);

       //qDebug() << "MainWindow::selectDefaultBand_END" ;
}

void MainWindow::selectDefaultMode()
{
      //qDebug() << "MainWindow::selectDefaultMode" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!upAndRunning)
    {
        return;
    }
    //int aux = 1;
    defaultMode = dataProxy->getMostUsedMode(currentLog);
      //qDebug() << "MainWindow::selectDefaultMode: " << QString::number(defaultMode) ;

    if (defaultMode < 1)
    {
        defaultMode = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode(0));
        //defaultMode = dataProxy->getIdFromModeName((modeComboBox->itemText(0)));
          //qDebug() << "MainWindow::selectDefaultMode2: " << QString::number(defaultMode) ;
          //qDebug() << "MainWindow::selectDefaultMode2S: "  ;
    }



    mainQSOEntryWidget->setMode(dataProxy->getSubModeFromId(defaultMode));
    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getSubModeFromId(defaultMode)));

     //qDebug() << "MainWindow::selectDefaultMode3: " << QString::number(defaultMode) ;
    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << "MainWindow::selectDefaultMode-END" ;
}


void MainWindow::readActiveBands (const QStringList actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
      //qDebug() << Q_FUNC_INFO << " - Start";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    foreach (QString aux, actives)
    {
       //qDebug() << Q_FUNC_INFO << ": " << aux;
    }
    bool atLeastOne = false;
    QString aux;
    bands.clear();
    QStringList __bands;
    __bands.clear();
    __bands = actives;
    __bands << dataProxy->getBandsInLog(currentLog);
    __bands.removeDuplicates();

    bands = __bands;

    for (int i = 0; i < __bands.size() ; i++)
    {
        if (dataProxy->getIdFromBandName(__bands.at(i)) > 0)
        {
            if (!atLeastOne)
            {
                atLeastOne = true;
            }
            aux = __bands.at(i);
            if (!aux.isEmpty())
            {
         //qDebug() << Q_FUNC_INFO << ": " << aux;
               bands << aux;
            }
        }
    }
    bands.removeDuplicates();
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::readActiveBands - END" ;
}

void MainWindow::readActiveModes (const QStringList actives)
{
     //qDebug() << Q_FUNC_INFO << " - Start";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString aux;
    aux.clear();

    QStringList __modes;
    __modes.clear();
    __modes = actives;
    __modes << dataProxy->getModesInLog(currentLog);
    __modes.removeDuplicates();
    modes.clear();

    for (int i = 0; i < __modes.size() ; i++)
    {
         //qDebug() << "MainWindow::readActiveModes: checking: " << __modes.at(i) ;
        if (dataProxy->getIdFromModeName(__modes.at(i)) > 0)
        {
      //qDebug() << "MainWindow::readActiveModes: checking-exist: " << __modes.at(i) ;
            aux = __modes.at(i);
            if (!aux.isEmpty())
            {
        //qDebug() << "MainWindow::readActiveModes: adding: " << aux ;
               modes << aux;
            }
        }
    }
    modes.removeDuplicates();
    modes.sort();
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::readActiveModes - END" ;
}

void MainWindow::createUIDX()
{
    //          //qDebug() << "MainWindow::createUIDX ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    infoLabel1->setToolTip(tr("Status of the DX entity."));
      //qDebug() << "MainWindow::createUIDX-13" ;
    infoLabel2->setToolTip(tr("Name of the DX entity."));
      //qDebug() << "MainWindow::createUIDX-14" ;

    dxUpLeftInputFrame = new QFrame;
    dxUpLeftTab = new QTabWidget;
    dxUpRightTab = new QTabWidget;
    dxBottonTab = new QTabWidget;

    dxUpLeftTab->setTabPosition(QTabWidget::North);
    dxUpRightTab->setTabPosition(QTabWidget::South);
      //qDebug() << "MainWindow::createUIDX-30" ;

    QHBoxLayout *dxUpLeftInputFrameLayout = new QHBoxLayout;
    dxUpLeftInputFrameLayout->addWidget (mainQSOEntryWidget);
    //dxUpLeftInputFrameLayout->setSizeConstraint(QLayout::SetFixedSize);
    dxUpLeftInputFrame->setLayout (dxUpLeftInputFrameLayout);


    dxUpLeftTab->addTab (QSOTabWidget, tr("QSO"));
    dxUpLeftTab->addTab(QSLTabWidget, tr("QSL"));
    dxUpLeftTab->addTab(eQSLTabWidget, tr("eQSL"));
    dxUpLeftTab->addTab(commentTabWidget, tr("Comment"));

    dxUpLeftTab->addTab(othersTabWidget, tr("Others"));

    dxUpLeftTab->addTab(myDataTabWidget, tr("My Data"));
    dxUpLeftTab->addTab(satTabWidget, tr("Satellite"));


      //qDebug() << "MainWindow::createUIDX-90" ;
    QSplitter *upLeftSplitter = new QSplitter (this);
    upLeftSplitter->addWidget(dxUpLeftInputFrame);
    //upLeftSplitter->addWidget(mainQSOEntryWidget);
    upLeftSplitter->addWidget(dxUpLeftTab);
    upLeftSplitter->setOrientation(Qt::Vertical);

    QVBoxLayout *dxUpRightFixLayout = new QVBoxLayout;
    dxUpRightFixLayout->addWidget(infoLabel1);
    dxUpRightFixLayout->addWidget(infoLabel2);

    infoLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    infoLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    dxUpRightTab->addTab(infoWidget, tr("Info"));
      //qDebug() << "MainWindow::createUIDX-100" ;

    reconfigureDXMarathonUI(manageDxMarathon);

      //qDebug() << "MainWindow::createUIDX-122" ;
    dxUpRightTab->addTab(awardsWidget, tr("Awards"));
    dxUpRightTab->addTab(searchWidget.get(), tr("Search"));

    dxBottonTab->addTab(logWindow.get(), tr("Log"));
    dxBottonTab->addTab(dxClusterWidget.get(), tr("DX-Cluster"));
    dxBottonTab->addTab(dxccStatusWidget.get(), tr("DXCC"));

    QVBoxLayout *dxUpRightLayout = new QVBoxLayout;
    dxUpRightLayout->addLayout(dxUpRightFixLayout);
    dxUpRightLayout->addWidget(dxUpRightTab);

    QWidget *upRightWidget = new QWidget;
    upRightWidget->setLayout(dxUpRightLayout);

    QSplitter *upSplitter = new QSplitter (this);
    upSplitter->addWidget(upLeftSplitter);
    upSplitter->addWidget(upRightWidget);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(upSplitter);
    splitter->addWidget(dxBottonTab);
    splitter->setOrientation(Qt::Vertical);

    QHBoxLayout *mLayout = new QHBoxLayout;
    mLayout->addWidget(splitter);
    //mLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainWidget->setLayout(mLayout);

      //qDebug() << "MainWindow::createUIDX - OS DETECTION"  ;

#ifdef Q_OS_WIN
      //qDebug() << "MainWindow::createUIDX - WINDOWS DETECTED!"  ;

    dxUpLeftInputFrame->setFrameShadow(QFrame::Raised);
    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel);

#else
     //qDebug() << "MainWindow::createUIDX - NO WINDOWS DETECTED!"  ;

    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
#endif
    setCentralWidget(mainWidget);
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::createUIDX-END" ;
 }


void MainWindow::slotADIFExport()
{
        //qDebug() << "MainWindow::slotADIFExport - Start" ;
    adifLoTWExportWidget->setExportMode(ModeADIF);
    adifLoTWExportWidget->show();

    //filemanager->adifLogExport(fileName, currentLog);
        //qDebug() << "MainWindow::slotADIFExport -END " ;
}

void MainWindow::showNumberOfSavedQSO(const QString &_fn, const int _n)
{
        //qDebug() << "MainWindow::showNumberOfSavedQSO: " << _fn << "/" << QString::number(_n) ;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(tr("KLog - ADIF export"));
    if (_n <= 0)
    { // TODO: Check if errors should be managed.
        msgBox.setText(tr("No QSOs have been exported to ADIF.") );
    }
    else
    {
        QString msg = QString(tr("KLog has exported %1 QSOs to the ADIF file: %2")).arg(QString::number(_n)).arg(_fn);
        msgBox.setText(msg);
    }

    msgBox.setStandardButtons(QMessageBox::Ok );
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
        //qDebug() << "MainWindow::showNumberOfSavedQSO - END" ;
}

/*
void MainWindow::fileExportADIF(const QString &_st, const QString &_grid, const QDate &_startDate, const QDate &_endDate)
{
     //qDebug() << Q_FUNC_INFO << ": " << _st ;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _grid, _startDate, _endDate, currentLog, ModeADIF);
    showNumberOfSavedQSO(fileName, qsos.count());
     //qDebug() << Q_FUNC_INFO << " - END";
}
*/

void MainWindow::fileExportADIF2(const QString &_call, QList<int> _qsos)
{
     //qDebug() << Q_FUNC_INFO ;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _call, _qsos, ModeADIF, currentLog);
    showNumberOfSavedQSO(fileName, qsos.count());
     //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::slotADIFExportAll()
{
     //qDebug() << Q_FUNC_INFO << " - Start";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString _callToUse = "ALL";
    QList <int> _qsos;
    _qsos.clear();
    _qsos.append(-1); //Code to specify to export ALL QSOs;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _callToUse, _qsos, ModeADIF, currentLog);

    showNumberOfSavedQSO(fileName, dataProxy->getHowManyQSOInLog(-1));


    logEvent(Q_FUNC_INFO, "END", Debug);
}


void MainWindow::fileExportLoTW2(const QString &_call, QList<int> _qsos)
{
      //qDebug() << Q_FUNC_INFO << " - Start";
      //qDebug() << Q_FUNC_INFO << QString(" - Start: %1 / QSOs: %2" ).arg(_call).arg(_qsos.length ());
    if (_qsos.length ()<1)
    {
        //TODO SHOW MESSAGE Asking for at least one QSO and exit
    }

    QMessageBox msgBox;
    Callsign callsign(_call);
    if (!callsign.isValid())
    {
    //qDebug() << Q_FUNC_INFO << " - no valid call" ;
        if (_call == "ALL")
        {
            msgBox.setWindowTitle(tr("KLog - LoTW"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("You need to select one station callsign to be able to send your log to LoTW."));
            msgBox.setStandardButtons(QMessageBox::Ok );
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        return;
    }

    QString fileName = util->getLoTWAdifFile();

    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _call, _qsos, ModeLotW, currentLog);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
          //qDebug() << Q_FUNC_INFO << " - NO QSOs" ;
        return;
    }
        //qDebug() << Q_FUNC_INFO << " - 50" ;
    bool uploadedToLoTW = callTQSL(fileName, _call);
      //qDebug() << Q_FUNC_INFO << " - 51" ;

    int i ;
    if (uploadedToLoTW)
    {
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - LoTW"));
        msgBox.setText(tr("TQSL finished with no error.\n\nDo you want to mark as Sent all the QSOs uploaded to LoTW?") );
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::Yes);
        int i = msgBox.exec();
        if (i == QMessageBox::Yes)
        {
           uploadedToLoTW = dataProxy->lotwSentQSOs(qsos);
           slotLogRefresh();

           if (!uploadedToLoTW)
           {
               QMessageBox msgBox;
               msgBox.setWindowTitle(tr("KLog - LoTW"));
               msgBox.setIcon(QMessageBox::Warning);
               msgBox.setText(tr("There was an error while updating to Yes the LoTW QSL sent information."));
               msgBox.setStandardButtons(QMessageBox::Ok );
               msgBox.setDefaultButton(QMessageBox::Ok);
               msgBox.exec();
           }
        }
    }
    if (!deleteAlwaysAdiFile)
    {
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - LoTW"));
        msgBox.setText(tr("The LoTW upload process has finished and KLog created a file (%1) in your KLog folder.\n\nDo you want KLog to remove that file?").arg(fileName));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::Yes);
        i = msgBox.exec();
        if (i == QMessageBox::Yes)
        {
            if (QFile::remove(fileName))
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(tr("KLog - LoTW"));
                msgBox.setText(tr("The file has been removed."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
            }
        }
    }
    else
    {
         QFile::remove(fileName);
    }
      //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::fileExportClubLog2(const QString &_call, QList<int> _qsos)
{
     //qDebug() << Q_FUNC_INFO << QString(" - Start: %1 / QSOs: %2" ).arg(_call).arg(_qsos.length ());
    QMessageBox msgBox;
    Callsign callsign(_call);
    if (!callsign.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - no valid call" ;
      if (_call == "ALL")
      {
          msgBox.setWindowTitle(tr("KLog - ClubLog"));
          msgBox.setIcon(QMessageBox::Warning);
          msgBox.setText(tr("You need to select one station callsign to be able to send your log to ClubLog."));
          msgBox.setStandardButtons(QMessageBox::Ok );
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
      }
      return;
  }

  QString fileName = util->getClubLogFile();
  QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _call, _qsos, ModeClubLog, currentLog);

  if (qsos.count() <= 0)
  { // TODO: Check if errors should be managed.
        //qDebug() << Q_FUNC_INFO << " -  NO QSOs" ;
      msgBox.setWindowTitle(tr("KLog - ClubLog"));
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.setText(tr("The selection you have done does not include any QSO"));
      msgBox.setStandardButtons(QMessageBox::Ok );
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return;
  }

  msgBox.setWindowTitle(tr("KLog - ClubLog"));
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.setText(tr("Do you want to add this QSOs to your ClubLog existing log?"));
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
  msgBox.setDetailedText(tr("If you don't agree, this upload will overwrite your current ClubLog existing log."));
  msgBox.setDefaultButton(QMessageBox::Ok);
  int ret = msgBox.exec();
  bool overwrite = false;
  switch (ret)
  {
  case QMessageBox::Ok:         // General ADIF
          overwrite = false;
      break;
  case QMessageBox::No:         // ClubLog
          overwrite = true;
      break;
  }
     //qDebug() << Q_FUNC_INFO << " - 50" ;
    elogClublog->sendLogFile(fileName, _qsos, overwrite);
    logWindow->refresh();
     //qDebug() << Q_FUNC_INFO << " - END " ;
}


void MainWindow::fileExportEQSL2(const QString &_call, QList<int> _qsos)
{
     //qDebug() << Q_FUNC_INFO << QString(" - Start:  QSOs: %2" ).arg(_qsos.length ());

    //QString fileName = "klog-eqsl-upload.adi";
    QString fileName = util->getEQSLFile();
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _call, _qsos, ModeEQSL, currentLog);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
          //qDebug() << Q_FUNC_INFO << " -  NO QSOs" ;
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - ClubLog"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("The selection you have done does not include any QSO."));
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    eqslUtilities->sendLogFile(fileName, _qsos);
    logWindow->refresh();
     //qDebug() << Q_FUNC_INFO << "-END " ;
}


void MainWindow::slotADIFExportSelection(const QString &_call, QList<int> _qsos, ExportMode _eM)
{
     //qDebug() << Q_FUNC_INFO << QString(" - Start: %1 / QSOs: %2" ).arg(_call).arg(_qsos.length ());
     //qDebug() << Q_FUNC_INFO << " - Trigered by signal: selection";
    switch (_eM)
    {
    case ModeADIF:         // General ADIF
        fileExportADIF2(_call, _qsos);
        break;
    case ModeLotW:         // LoTW
          //qDebug() << Q_FUNC_INFO << " - LoTW" ;
        fileExportLoTW2(_call, _qsos);
        break;
    case ModeClubLog:         // ClubLog
          //qDebug() << Q_FUNC_INFO << " - ClubLog" ;
        fileExportClubLog2(_call, _qsos);
        break;
    case ModeEQSL:         // General eQSL
          //qDebug() << Q_FUNC_INFO << " - eQSL" ;
        fileExportEQSL2(_call, _qsos);
        break;
    case ModeQRZ:         // General eQSL
          //qDebug() << Q_FUNC_INFO << " - QRZ.com" ;
        elogQRZcom->sendQSOs(_qsos);
        break;
    }
    logWindow->refresh();
      //qDebug() << Q_FUNC_INFO << " - END " ;
}

void MainWindow::slotADIFExportClose()
{
  adifLoTWExportWidget->close();
}

void MainWindow::slotLoTWExport()
{
    // 1.- Selec call
    // 2.- Select file and export (fixed filename?)
    // 3.- Call tqsl with the filename
    // 4.- Ask for the user to remove or not the file
        //qDebug() << "MainWindow::slotLoTWExport - Start" ;
   // bool emptyCall = false;
    if (!lotwActive)
    {
        showMessageToEnableTheOnlineService(LoTW);
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeLotW);
    adifLoTWExportWidget->show();
        //qDebug() << "MainWindow::slotLoTWExport- END" ;
}

void MainWindow::slotLoTWDownload()
{
        //qDebug() << "MainWindow::slotDownUpload - Start" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QStringList calls;
    calls << dataProxy->getStationCallSignsFromLog(-1);

    bool ok;

    QString callToUse = QInputDialog::getItem(this, tr("KLog - Select the Station Callsign."),
                                         tr("Select the Station Callsign to use when quering LoTW:"), calls, 0, false, &ok);

      //qDebug() << "MainWindow::slotDownUpload: " << callToUse ;
    if (ok && !callToUse.isEmpty())
    {
        lotwUtilities->setStationCallSign(callToUse);
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }

    if (!lotwUtilities->getIsReady())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - LoTW"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Please check the LoTW setup"));
        msgBox.setInformativeText(tr("You have not defined a LoTW user or a proper Station Callsign.\nOpen the LoTW tab in the Setup and configure your LoTW connection."));
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return;
    }

    lotwUtilities->download();
    logEvent(Q_FUNC_INFO, "END", Debug);
        //qDebug() << "MainWindow::slotDownUpload - END" ;
}

void MainWindow::slotLoTWFullDownload()
{
        //qDebug() << "MainWindow::slotLoTWFullDownload - Start" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QStringList calls;
    calls << dataProxy->getStationCallSignsFromLog(-1);

    bool ok;

    QString callToUse = QInputDialog::getItem(this, tr("KLog - Select the Station Callsign."),
                                         tr("Select the Station Callsign to use when quering LoTW:"), calls, 0, false, &ok);

      //qDebug() << "MainWindow::slotLoTWFullDownload: " << callToUse ;
    if (ok && !callToUse.isEmpty())
    {
        lotwUtilities->setStationCallSign(callToUse);
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }

    if (!lotwUtilities->getIsReady())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - LoTW"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Please check the LoTW setup"));
        msgBox.setInformativeText(tr("You have not defined a LoTW user or a proper Station Callsign.\nOpen the LoTW tab in the Setup and configure your LoTW connection."));
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return;
    }

    lotwUtilities->fullDownload();
    logEvent(Q_FUNC_INFO, "END", Debug);
        //qDebug() << "MainWindow::slotLoTWFullDownload - END" ;
}

void MainWindow::slotElogClubLogModifyCurrentLog()
{
    QMessageBox msgConfirm;
    msgConfirm.setIcon(QMessageBox::Question);
    msgConfirm.setWindowTitle(tr("KLog - ClubLog"));
    msgConfirm.setText(tr("Do you really want to mark ALL your QSOs to be UPLOADED? Must be done ONLY IF THIS IS YOUR FIRST TIME uploading QSOs to %1").arg(tr("ClubLog")));
    msgConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgConfirm.setDefaultButton(QMessageBox::No);
    int i = msgConfirm.exec();
    if (i == QMessageBox::Yes)
    {
        QMessageBox msgBox;
        if (dataProxy->clublogModifyFullLog(currentLog))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("The log is ready to be uploaded to ClubLog."));
            msgBox.setDetailedText(tr("All the QSOs in this log has been marked as Modified in the ClubLog status field"));
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("KLog could not mark the full log to be sent to ClubLog"));
            msgBox.setDetailedText(tr("Something prevented KLog from marking the QSOs as modified. Restart KLog and try again before contacting the KLog developers."));
        }
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::slotElogEQSLModifyCurrentLog()
{
    QMessageBox msgConfirm;
    msgConfirm.setIcon(QMessageBox::Question);
    msgConfirm.setWindowTitle(tr("KLog - eQSL"));
    msgConfirm.setText(tr("Do you really want to mark ALL your QSOs to be UPLOADED? Must be done ONLY IF THIS IS YOUR FIRST TIME uploading QSOs to %1").arg(tr("eQSL")));
    msgConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgConfirm.setDefaultButton(QMessageBox::No);
    int i = msgConfirm.exec();
    if (i == QMessageBox::Yes)
    {
        QMessageBox msgBox;
        if (dataProxy->eQSLModifyFullLog(currentLog))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("The log is ready to be uploaded to eQSL.cc."));
            msgBox.setDetailedText(tr("All the QSOs in this log has been marked as Modified in the eQSL.cc status field"));
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("KLog could not mark the full log to be sent to eQSL"));
            msgBox.setDetailedText(tr("Something prevented KLog from marking the QSOs as modified. Restart KLog and try again before contacting the KLog developers."));
        }
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::slotElogQRZCOMModifyCurrentLog()
{
    QMessageBox msgConfirm;
    msgConfirm.setIcon(QMessageBox::Question);
    msgConfirm.setWindowTitle(tr("KLog - QRZ.COM"));
    msgConfirm.setText(tr("Do you really want to mark ALL your QSOs to be UPLOADED? Must be done ONLY IF THIS IS YOUR FIRST TIME uploading QSOs to %1").arg(tr("QRZ.COM")));
    msgConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgConfirm.setDefaultButton(QMessageBox::No);
    int i = msgConfirm.exec();
    if (i == QMessageBox::Yes)
    {
        QMessageBox msgBox;
        if (dataProxy->QRZCOMModifyFullLog(currentLog))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("The log is ready to be uploaded to QRZ.com."));
            msgBox.setDetailedText(tr("All the QSOs in this log has been marked as Modified in the QRZ.com status field"));
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("KLog could not mark the full log to be sent to QRZ.com"));
            msgBox.setDetailedText(tr("Something prevented KLog from marking the QSOs as modified. Restart KLog and try again before contacting the KLog developers."));
        }
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::slotClubLogLogUpload()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!clublogActive)
    {
        showMessageToEnableTheOnlineService(ClubLog);
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeClubLog);
    adifLoTWExportWidget->show();

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::sloteQSLLogUpload()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (!eQSLActive)
    {
        showMessageToEnableTheOnlineService(eQSL)  ;
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeEQSL);
    adifLoTWExportWidget->show();

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotQRZCOMLogUpload()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
      //qDebug() << Q_FUNC_INFO ;
    if (!qrzcomActive)
    {
        showMessageToEnableTheOnlineService(QRZ)  ;
          //qDebug() << Q_FUNC_INFO << " - END" ;
        return;
    }

    if (!qrzcomSubscriber)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - QRZ.com"));
        msgBox.setText(tr("To upload QSOs you need a qrz.com subscription. If you have one, go to Setup->QRZ.com tab to enable it.") );
        msgBox.exec();
          //qDebug() << Q_FUNC_INFO << " - END" ;
        return;
    }

    if (!elogQRZcom->hasLogBookKey())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - QRZ.com"));
        msgBox.setText(tr("You need to define a proper API Key for your QRZ.com logbook in the eLog preferences.") );
        msgBox.exec();
          //qDebug() << Q_FUNC_INFO << " - END" ;
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeQRZ);
    adifLoTWExportWidget->show();
      //qDebug() << Q_FUNC_INFO << " - END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotRQSLExport()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               "ADIF (*.adi *.adif)");

    filemanager->adifReqQSLExport(fileName);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotADIFImport(){
         //qDebug() << "MainWindow::slotADIFImport " ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     util->getHomeDir(),
                                                     "ADIF (*.adi *.adif)");
    if (!fileName.isNull())
    {
          //qDebug() << "MainWindow::slotADIFImport -1" ;
        //filemanager->adifReadLog(fileName, currentLog);
        int loggedQSOs = filemanager->adifReadLog(fileName, QString(), currentLog);  // Empty StationCallsign by default
        if (loggedQSOs>0)
        {
            updateQSLRecAndSent();
            logWindow->refresh();
             //qDebug() << Q_FUNC_INFO << " -3";
            checkIfNewBandOrMode();
             //qDebug() << Q_FUNC_INFO << " -4" ;
            awardsWidget->fillOperatingYears();
             //qDebug() << Q_FUNC_INFO << " -5" ;
            slotShowAwards();
            awardsWidget->showAwards();
             //qDebug() << Q_FUNC_INFO << " -6" ;
        }
           //qDebug() << "MainWindow::slotADIFImport-7" ;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
       //qDebug() << "MainWindow::slotADIFImport-END" ;
}


void MainWindow::sendQSOToUI(const QSO &_qso)
{
    //qDebug() << Q_FUNC_INFO << (_qso.getEQSLQSLSDate()).toString("yyyy-MM-dd");
    QSOTabWidget->setRSTToMode(_qso.getSubmode(), false);
    mainQSOEntryWidget->setQSOData(_qso);
    commentTabWidget->setQSOData(_qso);
    satTabWidget->setQSOData(_qso);
    othersTabWidget->setQSOData(_qso);
    QSLTabWidget->setQSOData(_qso);
    eQSLTabWidget->setQSOData(_qso);
    myDataTabWidget->setQSOData(_qso);
    satTabWidget->setQSOData(_qso);
    QSOTabWidget->setQSOData(_qso);
}

void MainWindow::qsoToEdit (const int _qso)
{    

   //qDebug() << Q_FUNC_INFO  << QString::number(_qso) ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
   //qDebug() << Q_FUNC_INFO  << " - 000";
    if (!modify)
    {
        backupCurrentQSO ();
       qDebug() << Q_FUNC_INFO << ": Callsign: " << backupQSO->getCall();
       qDebug() << Q_FUNC_INFO << ": Mode " << backupQSO->getMode();
       qDebug() << Q_FUNC_INFO << ": SubMode " << backupQSO->getSubmode();
    }
   //qDebug() << Q_FUNC_INFO  << " - 005";
    clearUIDX(true);
    readingTheUI = true;

    setModifying(true);

   //qDebug() << Q_FUNC_INFO  << " - 010";
    modifyingQSOid = _qso;

   //qDebug() << Q_FUNC_INFO << " - 051 ";
    if ((clublogActive) && (clublogRealTime))
    {
        clublogPrevQSO = dataProxy->getClubLogRealTimeFromId(modifyingQSOid);
    }

    manualMode = true;      // We stop hamlib & wsjtx receiving data while editing a QSO
    mainQSOEntryWidget->setManualMode (manualMode);

    // ** Start of SAT data

     //qDebug() << Q_FUNC_INFO << " - SATELLITE - satName" ;
    satTabWidget->setFillingToEdit(true);
    QSO qsoE;
    qsoE.fromDB(modifyingQSOid);

    qsoInUI.copy(qsoE);
    //qDebug() << Q_FUNC_INFO  << " - m: " << (qsoE.getEQSLQSLSDate()).toString("yyyy-MM-dd");
    sendQSOToUI(qsoE);

    QString currentQrz = qsoE.getCall();
    currentEntity = world->getQRZARRLId(currentQrz);


    currentEntity = qsoE.getDXCC();
    infoLabel2->setText(world->getEntityName(currentEntity));
    infoWidget->showEntityInfo(currentEntity);

   //qDebug() << Q_FUNC_INFO << " - in default - 101"  ;
    EntityStatus _entityStatus;
    _entityStatus.dxcc      = currentEntity;
    _entityStatus.bandId    = currentBandShown;
    _entityStatus.modeId    = currentModeShown;
    _entityStatus.logId     = currentLog;

   //qDebug() << Q_FUNC_INFO << " - in default - 104"  ;
    showStatusOfDXCC(_entityStatus);

    readingTheUI = false;
    satTabWidget->setFillingToEdit(false);
   //qDebug() << Q_FUNC_INFO << " - END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::setModifying(const bool _m)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    modify = _m;
    QSOTabWidget->setModifying (modify);
    mainQSOEntryWidget->setModify(modify);
    satTabWidget->setModifying(modify);
    myDataTabWidget->setModify (modify);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotLocatorTextChanged(const QString &_loc)
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
     //qDebug() << "MainWindow::slotLocatorTextChanged: " << _loc;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    Locator locator;
    if ( locator.isValidLocator(_loc) )
    {
        infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), _loc);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotMyLocatorTextChanged(const QString &_loc)
{
      //qDebug() << "MainWindowMy::slotMyLocatorTextChanged: " <<_loc ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    Locator locator;
    if ( locator.isValidLocator(_loc))
    {
        dxccStatusWidget->setMyLocator(_loc);
        slotLocatorTextChanged(QSOTabWidget->getDXLocator());
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::showStatusOfDXCC(EntityStatus _entityStatus)
{
     //qDebug() << Q_FUNC_INFO << " - Entity: " << _entityStatus.dxcc << "/ Bandid :" << _entityStatus.bandId << "/Modeid: " << _entityStatus.modeId ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    // Receives:  QStringList _qs;
    //_qs << Entity << BandId << ModeId << lognumber;

    // Check if new one, needed, worked, confirmed
    // Print the infoLabel1
    // Print/Color the band boxes/labels

    /*
    0 - New One
    1 - Needed
    2 - Worked
    3 - Confirmed
    */
     //qDebug() << Q_FUNC_INFO << " - 10";
    if (_entityStatus.bandId <= 0) // is the status valid?
    {
         //qDebug() << Q_FUNC_INFO << " - 20";
        infoWidget->clear();
        slotShowInfoLabel("--");
        //infoLabel1->setText("--");
        logEvent(Q_FUNC_INFO, "END-1", Debug);
          //qDebug() << Q_FUNC_INFO <<  " - END-1" ;
        return;
    }
     //qDebug() << Q_FUNC_INFO << " - 30";
    // Set the status bar with the appropriate message
    int status = awards.getDXStatus (_entityStatus);

     //qDebug() << Q_FUNC_INFO << " -  " << QString::number(status) ;

    QString message = awards.getDXStatusString(status);
     //qDebug() << Q_FUNC_INFO << " - 40";
    slotShowInfoLabel(message);
     //qDebug() << Q_FUNC_INFO << " - 10";
    //infoLabel1->setText(message);
    //infoWidget->showInfo((_qs.at(0)).toInt(), (_qs.at(1)).toInt(), (_qs.at(2)).toInt(), (_qs.at(3)).toInt() );
     //qDebug() << Q_FUNC_INFO << " - 50";
    infoWidget->showInfo(_entityStatus.dxcc);
     //qDebug() << Q_FUNC_INFO << " - 51";
     //qDebug() << Q_FUNC_INFO << " - END-2" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::showDXMarathonNeeded(const int _dxcc, const int _cqz, const int _year, const int _log)
{
     //qDebug() << "MainWindow::showDXMarathonNeeded" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if ((_dxcc<=0) || (_cqz<=0) || (!manageDxMarathon))
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    if ( awards.isDXMarathonNeed(_dxcc, _cqz, _year, _log))
    {
        slotShowInfoLabel(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
        //infoLabel1->setText(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowAwards()
{ //To be called from the logWindow & searchWidget
     //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    awards.recalculateAwards();
     //qDebug() << Q_FUNC_INFO << " - 1";
    //logWindow->refresh();
     //qDebug() << Q_FUNC_INFO << " - 2";

    awardsWidget->showAwards();
     //qDebug() << Q_FUNC_INFO << " - 3";
    //dxccStatusWidget->refresh();
    setMainWindowTitle();
    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::fillQSOData()
{ // Updates all QSO with the dxcc, CQZ, ... if empty.
      //qDebug() << "MainWindow::fillQSOData" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QString stringQuery = QString("SELECT call, bandid, modeid, qso_date, lognumber, id, cqz, ituz, dxcc, cont FROM log WHERE ((dxcc<1) OR (cqz<1) OR (ituz<1) OR (dxcc IS NULL) OR (cqz IS NULL) OR (ituz IS NULL))");
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return;
    }

    QSqlQuery query1;
    QSqlRecord rec = query.record();
    int nameCol;
    QString aux, queryString;
    QString _call, _bandid, _modeid, _tdate, _ttime, _lognumber, _id, aux1, updateString;//, _confirmed;
    bool toModify = false;
    bool noMoreQso = false;

    int numberOfQsos = 0;
    int i = 0;
    int _dxcc = 0;

    numberOfQsos = dataProxy->getHowManyQSOInLog(currentLog);

    //int progressBarPosition = 0;

    QProgressDialog progress(tr("Filling QSOs ..."), tr("Abort filling"), 0, numberOfQsos, this);
    progress.setWindowModality(Qt::WindowModal);


    while ( (query.next()) && (!noMoreQso) ) {
        if (query.isValid())
        {
            i++;
            toModify = false;
            nameCol = rec.indexOf("call");
            if ( (query.value (nameCol)).isValid() )
            {
                _call = (query.value (nameCol)).toString();
            }
            nameCol = rec.indexOf("bandid");
            if ( (query.value (nameCol)).isValid() )
            {
                _bandid = (query.value (nameCol)).toString();
            }
            nameCol = rec.indexOf("modeid");
            if ( (query.value (nameCol)).isValid() )
            {
                _modeid = (query.value (nameCol)).toString();
            }
            nameCol = rec.indexOf("qso_date");
            if ( (query.value (nameCol)).isValid() )
            {
                _tdate = (query.value (nameCol)).toString();
            }
            nameCol = rec.indexOf("id");
            if ( (query.value (nameCol)).isValid() )
            {
                _id = (query.value (nameCol)).toString();
            }
                  //qDebug() << Q_FUNC_INFO << ":   ID: " << _id ;
    //TODO: Prepare this query
            updateString = "UPDATE log SET call = '" + _call + "', bandid = '" + _bandid + "', modeid = '" + _modeid + "', qso_date = '" + _tdate + "'" ;//  + "', confirmed = '" + _confirmed + "'";

            nameCol = rec.indexOf("cqz");
            if (( (query.value (nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number(world->getQRZCqz(_call));
                updateString = updateString + ", cqz='" + aux1 + "'";
            toModify = true;
            }

            nameCol = rec.indexOf("ituz");
            if (( (query.value (nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number( world->getQRZItuz(_call) );
                updateString = updateString + ", ituz='" + aux1 + "'";
                toModify = true;
            }

      //qDebug() << Q_FUNC_INFO << ":   DXCC" ;

            nameCol = rec.indexOf("dxcc");
            _dxcc = (query.value (nameCol)).toInt();
      //qDebug() << Q_FUNC_INFO << ":   DXCC: " << QString::number(_dxcc) ;
            if (_dxcc < 1)
            {
                aux1 = QString::number(world->getQRZARRLId(_call) );
          //qDebug() << Q_FUNC_INFO << ":   DXCC proposed: " << aux1 ;
                updateString = updateString + ", dxcc='" + aux1 + "'";
                toModify = true;
                _dxcc = aux1.toInt();
            }
            else
            {
          //qDebug() << Q_FUNC_INFO << ":   DXCC already existed"  ;
            }

            nameCol = rec.indexOf("cont");
            if (( (query.value (nameCol)).toString()).length() < 2 )
            {
                aux1 = world->getContinentShortName(_dxcc);     
                updateString = updateString + ", cont='" + aux1 + "'";
                toModify = true;
            }

            //_dxcc = -1;
            //qDebug() << "MainWindow::fillQSOData1: " << updateString ;
            if (toModify)
            {
                updateString = updateString + " WHERE id = " + "'" + _id + "'";
                      //qDebug() << "MainWindow::fillQSOData2: " << updateString ;
                sqlOK = query1.exec(updateString);
                if (sqlOK)
                {
                          //qDebug() << Q_FUNC_INFO << ":   sqlOK=True" ;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().nativeErrorCode(), query1.lastQuery());
                          //qDebug() << Q_FUNC_INFO << ":   sqlOK=False" ;
                }
            }
            else
            {
                updateString.clear();
            }

            aux = tr("Filling DXCC, CQz, ITUz, Continent in QSOs...\n QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue (i);

            if ( progress.wasCanceled() )
            {
                      //qDebug() << "MainWindow::fillQSOData3: " ;
                noMoreQso = true;
            }
        } // Closes the next.isValid
    } // Closes the While
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotFillEmptyDXCCInTheLog()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    world->fillEmptyDXCCInTheLog();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotUpdateCTYDAT()
{
    //qDebug() << "MainWindow::slotUpdateCTYDAT" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    downloadcty->download();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotUpdateSATSDAT()
{
      //qDebug() << "MainWindow::slotUpdateSATSDAT" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    updateSatsData->readSatDataFile();
    satTabWidget->refreshData();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowStats()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    statsWidget->show();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotWorldReload(const bool _b)
{
         //qDebug() << "MainWindow::slotWorldReload" ;
    //TODO: world.recreate returns a boolean, so it is possible to manage the errors
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (_b)
    {
        world->recreate(ctyDatFile);
    }

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotFilePrint()
{
          //qDebug() << "MainWindow::slotFilePrint" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QPrinter printer;
    QString aux;
    QSqlQuery query, query1;
    int row = 0;
    int _numberOfQsos = 0;
    bool cancelPrinting = false;
    bool sqlOK;
    _numberOfQsos = dataProxy->getHowManyQSOInLog(currentLog);
    int step = util->getProgresStepForDialog(_numberOfQsos);


    QTextDocument *doc = new QTextDocument;
    QTextCursor cursor(doc);
    QTextTable *textTable = cursor.insertTable(1, 9);

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setCellPadding(5);
    tableFormat.setAlignment(Qt::AlignCenter);

    textTable->setFormat(tableFormat); //The first row is the header

    cursor = textTable->cellAt(row, 0).firstCursorPosition();
    cursor.setBlockFormat(centerAlignment);
    cursor.insertText(tr("Number"));
    cursor = textTable->cellAt(row, 1).firstCursorPosition();
    cursor.insertText(tr("Date/Time"));
    cursor = textTable->cellAt(row, 2).firstCursorPosition();
    cursor.insertText(tr("Callsign"));
    cursor = textTable->cellAt(row, 3).firstCursorPosition();
    cursor.insertText(tr("RSTtx"));
    cursor = textTable->cellAt(row, 4).firstCursorPosition();
    cursor.insertText(tr("RSTrx"));
    cursor = textTable->cellAt(row, 5).firstCursorPosition();
    cursor.insertText(tr("Band"));
    cursor = textTable->cellAt(row, 6).firstCursorPosition();
    cursor.insertText(tr("Mode"));
    cursor = textTable->cellAt(row, 7).firstCursorPosition();
    cursor.insertText(tr("Comment"));

    int nameCol = 0;

    //int qsoPerPage = 25;
    //int maxPages = (int)(Klog::number / qsoPerPage)+1; // To print just 10 QSO per page
    //int printedSoFar = 0;
    //int page = 1;

    printer.setPageOrientation(QPageLayout::Landscape); // For testing, the log will be printed landscape.
    printer.setDocName(stationCallsign+"-log");

    QPrintDialog printDialog(&printer, this);
    printDialog.setWindowTitle(tr("Print Log"));

    if (printDialog.exec() == QDialog::Accepted)
    {
        int _qsos = 0;
        QProgressDialog progress(tr("Printing the log ..."), tr("Abort printing"), 0, _numberOfQsos, this);
        progress.setMaximum(_numberOfQsos);
        progress.setWindowModality(Qt::WindowModal);


        QString stringQuery = QString("SELECT id, qso_date, call, rst_sent, rst_rcvd, bandid, modeid, comment FROM log WHERE lognumber='%1'").arg(currentLog);
        sqlOK = query.exec(stringQuery);
        if (!sqlOK)
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            logEvent(Q_FUNC_INFO, "END-1", Debug);
            return;
    //TODO: Print a message showing an error and exit.
        }
        QSqlRecord rec = query.record();

        aux = tr("Printing the log...\n QSO: ")  + QString::number(_qsos) + "/" + QString::number(_numberOfQsos);
        progress.setLabelText(aux);
        progress.setValue (_qsos);

        while ((query.next()) && (!cancelPrinting))
        {
            if (query.isValid())
            {
                textTable->appendRows(1);
                row++;
                _qsos++;
                      //qDebug() << "MainWindow::slotFilePrint: QSO: " << QString::number(_qsos) << " - Step: " << QString::number(step) << " - Div: " << QString::number(_qsos % step);
                if (( (_qsos % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = tr("Printing the log...\n QSO: ")  + QString::number(_qsos) + "/" + QString::number(_numberOfQsos);
                    progress.setLabelText(aux);
                    progress.setValue (_qsos);
                }

                nameCol = rec.indexOf("id");
                cursor = textTable->cellAt(row, 0).firstCursorPosition();
                cursor.insertText((query.value (nameCol)).toString());

                nameCol = rec.indexOf("qso_date");
                cursor = textTable->cellAt(row, 1).firstCursorPosition();
                cursor.insertText((query.value (nameCol)).toString());

                nameCol = rec.indexOf("call");
                cursor = textTable->cellAt(row, 2).firstCursorPosition();
                cursor.insertText((query.value (nameCol)).toString());

                nameCol = rec.indexOf("rst_sent");
                cursor = textTable->cellAt(row, 3).firstCursorPosition();
                cursor.insertText((query.value (nameCol)).toString());

                nameCol = rec.indexOf("rst_rcvd");
                cursor = textTable->cellAt(row, 4).firstCursorPosition();
                cursor.insertText((query.value (nameCol)).toString());

                nameCol = rec.indexOf("bandid");
                aux = (query.value (nameCol)).toString();
                stringQuery = QString("SELECT name FROM band WHERE id='%1'").arg(aux);
                sqlOK = query1.exec(stringQuery);
                if (sqlOK)
                {
                    query1.next();
                    if (query1.isValid())
                    {
                        cursor = textTable->cellAt(row, 5).firstCursorPosition();
                        cursor.insertText((query1.value (0)).toString());
                    }
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().nativeErrorCode(), query1.lastQuery());
                }

                  //qDebug() << "MainWindow::slotFilePrint: Band: " << aux ;
                nameCol = rec.indexOf("modeid");
                  //qDebug() << "MainWindow::slotFilePrint: nameCol: " << QString::number(nameCol) ;
                aux = (query.value (nameCol)).toString();
                  //qDebug() << "MainWindow::slotFilePrint: Mode1: " << aux ;
                aux = dataProxy->getSubModeFromId(aux.toInt());
                  //qDebug() << "MainWindow::slotFilePrint: Mode2: " << aux ;
                if (aux.length()>1)
                {
                    cursor = textTable->cellAt(row, 6).firstCursorPosition();
                    cursor.insertText(aux);
                }
                else
                {
                      //qDebug() << "MainWindow::slotFilePrint: ERROR: " << aux ;
                }

                nameCol = rec.indexOf("comment");
                aux = (query.value (nameCol)).toString();
                if ((aux.length())>0)
                {
                    cursor = textTable->cellAt(row, 7).firstCursorPosition();
                    cursor.insertText(aux);
                }
            }
            if ( progress.wasCanceled() )
            {
                    cancelPrinting = true;
            }
        }


        progress.setValue (_numberOfQsos);
        if (!cancelPrinting)
        {
            doc->print(&printer);
        }
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

//DX-CLUSTER - DXCLUSTER

void MainWindow::slotAnalyzeDxClusterSignal(const DXSpot &_spot)
{
     //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    DXSpot spot = _spot;
    //if (spot.isValid())
        //qDebug() << Q_FUNC_INFO << " - Spot is Valid";
    //else
        //qDebug() << Q_FUNC_INFO << " - Spot is NOT Valid";
    //qDebug() << Q_FUNC_INFO << ": spot-dxCall       : " << spot.getDxCall();
    //qDebug() << Q_FUNC_INFO << ": spot-Spotter      : " << spot.getSpotter();
    //qDebug() << Q_FUNC_INFO << ": spot-Freq         : " << spot.getFrequency().toQString();
    //qDebug() << Q_FUNC_INFO << ": spot-Comment      : " << spot.getComment();

    EntityStatus _entityStatus;
    _entityStatus.dxcc = world->getQRZARRLId(spot.getDxCall());

    if (!manageMode)
    {
        _entityStatus.modeId = -1;
    }

    if (spot.getClickStatus() == SingleClick)
    {
        infoLabel2->setText(world->getEntityName(_entityStatus.dxcc));
        infoWidget->showEntityInfo(_entityStatus.dxcc );

        // Becareful, he Frecuency arrives in KHz instead of bandid!!
        // db.getBandFromFreq expects a MHz!
        //(ql.at(1)).toDouble()
        _entityStatus.bandId = dataProxy->getBandIdFromFreq((spot.getFrequency().toDouble()));
        //qls << QRZ << BandId << ModeId << lognumber;
        showStatusOfDXCC(_entityStatus);
    }
    else if (spot.getClickStatus() == DoubleClick)
    {
        clusterSpotToLog(spot.getDxCall(), spot.getFrequency());
    }

    int statusI = awards.getDXStatus (_entityStatus);

    //proposedQSOs pQSO;

    //pQSO.status = awards.getQSOStatus(statusI);
    //Callsign callsign(spot.getDxCall());
    //if (callsign.isValid())
    //{
    //    pQSO.call = spot.getDxCall();
    //    dxClusterAssistant->newDXClusterSpot(pQSO);
    //}

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotDXClusterSpotArrived(const DXSpot &_spot)
{
     //qDebug() << Q_FUNC_INFO;
    //(void)_dxCall;
    //(void)_freq;

    DXSpot sp = _spot;
    if (!sp.isValid())
        return;
    Callsign callsign(sp.getDxCall());
    //if (callsign.isValid())
    //{
    //   proposedQSOs pQSO;
    //    pQSO.call = sp.getDxCall();
    //    pQSO.status = ATNO;
    //    pQSO.freq = sp.getFrequency();
    //    dxClusterAssistant->newDXClusterSpot(pQSO);
       //qDebug() << Q_FUNC_INFO << ": DXCall not valid: " << pQSO.call;
    //}
    //else
    //{
         //qDebug() << Q_FUNC_INFO << ": DXCall not valid: " << sp.getDxCall();
    //}


    if (!dxclusterSendSpotsToMap)
    {
        return;
    }
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString dxGrid = world->getQRZLocator (sp.getDxCall());
    Locator locator;
    Coordinate coord = locator.getLocatorCoordinate (dxGrid);
      //qDebug() << Q_FUNC_INFO << QString("  %1: Locator: %2 - (lat/lon)=>(%3/%4)").arg(sp.getDxCall()).arg(dxGrid).arg(coord.lat).arg(coord.lon);
      //qDebug() << "Lat: " << QString::number(coord.lat) << " - Lon: " << QString::number(coord.lon);
    mapWindow->addMarker(coord, dxGrid);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::clusterSpotToLog(const QString &_call, Frequency _fr)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QString _aux;
    double _freqN = _fr.toDouble(MHz);
     //qDebug() << Q_FUNC_INFO << " - calling setQRZ-2" ;
    mainQSOEntryWidget->setQRZ(_call);
    QSOTabWidget->setTXFreq (_freqN);
    QSOTabWidget->setRXFreq(_freqN);

    //freqQLCDNumber->display(_freqN);

    //_aux = QString::number(_freqN);

          //qDebug() << Q_FUNC_INFO << " - Freq: " << _aux ;

    int _bandi = dataProxy->getBandIdFromFreq(_freqN);
          //qDebug() << Q_FUNC_INFO << " - Bandi: " << QString::number(_bandi) ;
    _aux = QString::number(_bandi);
    _aux = QString("SELECT name FROM band WHERE id ='%1'").arg(_aux);

          //qDebug() << Q_FUNC_INFO << " - Band: " << _aux ;

    QSqlQuery query(_aux);
    query.next();
    if (query.isValid())
    {
        _aux = (query.value (0)).toString();
        mainQSOEntryWidget->setBand(_aux);
        //bandComboBox->setCurrentIndex(bandComboBox->findText(_aux, Qt::MatchCaseSensitive));
    }
    else
    {
        mainQSOEntryWidget->setBand(dataProxy->getNameFromBandId(defaultBand));
        //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
        //bandComboBox->setCurrentIndex(defaultBand);
    }
      //qDebug() << Q_FUNC_INFO << " - END "  ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}
//DX-CLUSTER - DXCLUSTER

void MainWindow::updateQSLRecAndSent()
{
        //qDebug() << "MainWindow::updateQSLRecAndSent "  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    // Checks the log to fill all the qsl_rcvd and qsl_sent
    QSqlQuery query, query1;
    QString queryString, aux, idT;
    //int nameCol=0;

    //queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE lognumber='%1'").arg(currentLog);
    //queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE qsl_rcvd !='Y' AND qsl_rcvd !='N' AND qsl_rcvd !='R' AND qsl_rcvd !='I' AND qsl_rcvd !='V' AND lognumber='%1'").arg(currentLog);
    //queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE qsl_rcvd ='' OR qsl_rcvd IS NULL OR qsl_sent ='' AND lognumber='%1'").arg(currentLog);
    queryString = QString("UPDATE log SET qsl_rcvd='N' WHERE qsl_rcvd ='' AND lognumber='%1'").arg(currentLog);

    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }

    queryString = QString("UPDATE log SET qsl_sent='N' WHERE qsl_sent ='' AND lognumber='%1'").arg(currentLog);

    sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << "MainWindow::updateQSLRecAndSent - END"  ;
}

QString MainWindow::findStationCallsignToUse()
{
    //QString foundCall = dataProxy->getStationCallSignFromLog (currentLog);
    QString foundCall = dataProxy->getStationCallSignFromLog (currentLog);
    Callsign callsign(foundCall);
    if (callsign.isValid())
        return foundCall;

    return mainQRZ;
}

void MainWindow::defineStationCallsign()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString logQRZ = findStationCallsignToUse();
   //qDebug() << Q_FUNC_INFO << ": StationCallsign: " << logQRZ;
    Callsign callsign(logQRZ);
    if (!callsign.isValid())
    {
        return;
    }
    stationCallsign = logQRZ;

     //qDebug() << Q_FUNC_INFO << ": " << stationCallsign  ;

    filemanager->setStationCallSign(stationCallsign);
      //qDebug() << Q_FUNC_INFO << ": AFTER"  ;
    myDataTabWidget->setData(stationCallsign, operatorQRZ, myDataTabWidget->getMyLocator());
    dxccStatusWidget->setMyLocator(myDataTabWidget->getMyLocator());
    searchWidget->setStationCallsign(stationCallsign);
    if (lotwActive)
    {
        lotwUtilities->setStationCallSign(stationCallsign);
    }
    dxClusterWidget->setMyQRZ(stationCallsign);
    adifLoTWExportWidget->setDefaultStationCallsign(stationCallsign);

    logEvent(Q_FUNC_INFO, "END", Debug);
       //qDebug() << Q_FUNC_INFO << ": " << stationCallsign << " - END" ;
}

void MainWindow::slotSetPropModeFromSat(const QString &_p, bool _keep)
{
     //qDebug() << Q_FUNC_INFO << ": " << _p << "- _keep: " << util->boolToQString(_keep) ;;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    othersTabWidget->setPropMode(_p, _keep);
    QSOTabWidget->setPropModeFromSat(_p);
    logEvent(Q_FUNC_INFO, "END", Debug);
    //int indexC = propModeComboBox->findText(" - " + _p + " - ", Qt::MatchContains);
    //propModeComboBox->setCurrentIndex(indexC);
}

void MainWindow::slotSetPropModeFromOther(const QString &_p)
{
     //qDebug() << Q_FUNC_INFO << ": " << _p;
    if (_p!="SAT")
    {
         //qDebug() << Q_FUNC_INFO << ": Is NOT SAT propagation mode";
        satTabWidget->setNoSat();
    }
}

void MainWindow::clearIfNotCompleted()
{
    if (completedWithPreviousIOTA)
    {
        othersTabWidget->clearIOTA();
        completedWithPreviousIOTA = false;
    }
    if (completedWithPreviousQSLVia)
    {
        QSLTabWidget->setQSLVia("");
        completedWithPreviousQSLVia = false;
    }
}

void MainWindow::completeWithPreviousQSO(const QString &_call)
{
        //qDebug() << "MainWindow::completeWithPreviousQSO" ;
    //This function completes: Name, QTH, Locator, Entity, Iota
    logEvent(Q_FUNC_INFO, "Start", Debug);
    int previousQSOId = dataProxy->isWorkedB4(_call, -1);
    if ((!completeWithPrevious) || (_call.length()<=0) || (previousQSOId<=0))
    //if ( (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    {
    //qDebug() << "MainWindow::completeWithPreviousQSO NOT completing..." ;
        QSOTabWidget->setName(QString());
        QSOTabWidget->setQTH(QString());
        QSOTabWidget->setDXLocator(QString());
        clearIfNotCompleted();
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    QSO prevQSO;
    prevQSO.fromDB(previousQSOId);
        //qDebug() << "MainWindow::completeWithPreviousQSO completing..." ;
    QString aux = QString();

    QSOTabWidget->setName(prevQSO.getName(), true);
    QSOTabWidget->setQTH(prevQSO.getQTH(), true);
    QSOTabWidget->setDXLocator(prevQSO.getGridSquare(), true);

    aux = prevQSO.getIOTA();

    if ((aux.length()>=0) && (othersTabWidget->isIOTAModified()) )
    {
        aux = awards.checkIfValidIOTA(aux);

        if ((aux.length())==6)
        {
            othersTabWidget->setIOTA(aux);
            completedWithPreviousIOTA=true;
        }
        else if (completedWithPreviousIOTA && (aux != othersTabWidget->getIOTA()))
        {
            othersTabWidget->clearIOTA();
            completedWithPreviousIOTA = false;
        }
    }
    else if (completedWithPreviousIOTA )
    {
        othersTabWidget->clearIOTA();
        completedWithPreviousIOTA = false;
    }
    aux = prevQSO.getQSLVia();
    if ((aux.length()>=0) && ((QSLTabWidget->getQSLVia()).length()<=0) )
    {
        QSLTabWidget->setQSLVia(aux, Qt::red);
        completedWithPreviousQSLVia=true;
    }
    else if (completedWithPreviousQSLVia && (aux != QSLTabWidget->getQSLVia()))
    {
        QSLTabWidget->setQSLVia("");
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotValidBandsReceived(const QStringList &_b)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
     //qDebug() << Q_FUNC_INFO ;
    dxccStatusWidget->setBands(Q_FUNC_INFO, _b, true);
    satTabWidget->addBands(_b);
    mapWindow->setBands(_b);
     //qDebug() << Q_FUNC_INFO << " - END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotFreqRXChanged(const double _fr)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_fr);
    if (!upAndRunning)
    {
        //qDebug() << Q_FUNC_INFO << " - not running" ;
        return;
    }
    int bandId = dataProxy->getBandIdFromFreq(_fr);
    if (bandId < 1)
    {
        //qDebug() << Q_FUNC_INFO << " - wrong band" ;
        return;
    }

    // The following line is needed to ensure that the band of the freqRX is included
    // in the list of available bands and include it if it is not.
    mainQSOEntryWidget->setFreq (_fr, true);

    QSOTabWidget->setRXFreq (_fr);
    satTabWidget->setDownLinkFreq(_fr);
    //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);    
}

void MainWindow::slotFreqTXChangedFromSat(const double _fr)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!upAndRunning)
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    if (dataProxy->isThisFreqInBand (mainQSOEntryWidget->getBand (), QString::number(_fr)))
    {
        //qDebug() << Q_FUNC_INFO << " - END-2";
        return;
    }
    slotFreqTXChanged (_fr);
    //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotFreqTXChanged(const double _fr)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_fr);
    if (!upAndRunning)
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }

    //qDebug() << Q_FUNC_INFO << " - 10";
    QSOTabWidget->setTXFreq (_fr);
    //qDebug() << Q_FUNC_INFO << " - 11";
    mainQSOEntryWidget->setFreq (_fr);
    //qDebug() << Q_FUNC_INFO << " - calling setUpLinkFreq";
    satTabWidget->setUpLinkFreq(_fr);
    if (hamlibActive && !manualMode)
    {
        hamlib->setFreq(_fr);
    }
    mainQSOEntryWidget->setMode(util->getDefaultModeForFreq(_fr));

    logEvent(Q_FUNC_INFO, "END", Debug);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::slotShowQSOsFromDXCCWidget(QList<int> _qsos)
{
      //qDebug() << "MainWindow::slotShowQSOsFromDXCCWidget" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    slotShowSearchWidget();
    searchWidget->showQSOs(_qsos);
    logEvent(Q_FUNC_INFO, "END", Debug);
}


    void MainWindow::slotQSOReceived(const QSO &_qso)
{    
    //qDebug() <<  Q_FUNC_INFO << " - Start";
    //logEvent(Q_FUNC_INFO, "Start", Debug);

    if (!wsjtxAutoLog)
        if (!askToAddQSOReceived(_qso))
            return;
   //qDebug() << Q_FUNC_INFO << "010";
    QSO q;
   //qDebug() << Q_FUNC_INFO << "020";
    q.copy(_qso);
    q.setLogId(currentLog);
   //qDebug() << Q_FUNC_INFO << "030";
   //qDebug() << Q_FUNC_INFO << "Call: " << q.getCall();
   //qDebug() << Q_FUNC_INFO << "Mode: " << q.getMode();

    int dxcc = world->getQRZARRLId(q.getCall());
     //qDebug() << Q_FUNC_INFO << "040";
    dxcc = util->getNormalizedDXCCValue (dxcc);
     //qDebug() << Q_FUNC_INFO << "050";
    q.setDXCC(dxcc);
    q.setClubLogStatus(clublogSentDefault);
    q.setLoTWQSL_SENT(lotwSentDefault);
    q.setEQSLQSL_SENT(eqslSentDefault);
    q.setQRZCOMStatus(qrzcomSentDefault);
     //qDebug() << Q_FUNC_INFO << "060";

    if (!showWSJTXDuplicatedMSG(q))
        return;

     //qDebug() << Q_FUNC_INFO << "070";
    int addedQSO = q.toDB();
   //qDebug() << Q_FUNC_INFO << "addedQSO: " << addedQSO;
    if (addedQSO>0)
    {
         //qDebug() << Q_FUNC_INFO << "090";
         //qDebug() <<  Q_FUNC_INFO << " - QSO added: " << QString::number(addedQSO);
        actionsJustAfterAddingOneQSO();
        slotShowInfoLabel(tr("QSO logged from WSJT-X:"));
        infoLabel2->setText(q.getCall() + " - " + dataProxy->getBandNameFromFreq(q.getFreqTX()) + "/" + q.getSubmode());
        slotClearButtonClicked(Q_FUNC_INFO);
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << "100";
    }

     //qDebug() <<  Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
}

bool MainWindow::askToAddQSOReceived(const QSO &_qso)
{
   //qDebug() <<  Q_FUNC_INFO << " - Start";
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("KLog - QSO received - NEW"));
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::Yes);
    QSO qsoM(_qso);

    QString aux  = QString(tr("<HTML><body>ASK-The following QSO data has been received from to be logged:\n\n"
                           "<table>"
                           "<tr><TH>Callsign:</TH><TD>%1</TD></TR>"
                           "<TR><TH>Freq:</TH><TD>%2</TD></TR>"
                           "<TR><TH>Mode:</TH><TD>%3</TD></TR>"
                           "<TR><TH>Time On:</TH><TD>%4</TD></TR>"
                           "<TR><TH>Time Off:</TH><TD>%5</TD></TR>"
                           "<TR><TH>RST TX:</TH><TD>%6</TD></TR>"
                           "<TR><TH>RST RX:</TH><TD>%7</TD></TR>"
                           "<TR><TH>Comment:</TH><TD>%8</TD></TR>"
                           "<TR><TH>DX-Grid:</TH><TD>%9</TD></TR>"
                           "<TR><TH>Local-Grid:</TH><TD>%10</TD></TR>"
                           "<TR><TH>Station Callsign:</TH><TD>%11</TD></TR>"
                             "<TR><TH>Operator Callsign:</TH><TD>%12</TD></TR></table></body></html>")).arg(qsoM.getCall(), QString::number(qsoM.getFreqTX()), qsoM.getSubmode(),
                            util->getADIFTimeFromQTime(qsoM.getTimeOn()), util->getADIFTimeFromQTime(qsoM.getTimeOff()), qsoM.getRSTTX(), qsoM.getRSTRX(),
                            qsoM.getComment(), qsoM.getGridSquare(), qsoM.getMyGridSquare(),
                            qsoM.getStationCallsign(), qsoM.getOperatorCallsign());

    msgBox.setText(aux);
    int ret = msgBox.exec();
    switch (ret)
    {
    //case QMessageBox::Yes:
    //break;
    case QMessageBox::No:
        //logTheQso = false;
        return false;
        break;
    default:
        // should never be reached
        //logTheQso = false;
        return true;
        break;
    }
}

bool MainWindow::showWSJTXDuplicatedMSG(const QSO &_qso)
{
    QSO q = _qso;
  //qDebug() << Q_FUNC_INFO << " - Calling isThisQSODuplicated with call: " << q.getCall();
    if (!((dataProxy->isThisQSODuplicated(q, dupeSlotInSeconds)).length()>0))
        return true;

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - WSJTX Dupe QSO"));

    msgBox.setIcon(QMessageBox::Warning);
    QString aux = tr("This QSO seems to be duplicated. Do you want to save or discard it?");
    msgBox.setText(aux);
    msgBox.setDetailedText(tr("Duplicated QSOs have to match another existing QSO with the same call, band, mode, date and time, taking into account the period that can be defined in the settings."));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard );

    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret)
    {
        case QMessageBox::Save:
            logEvent(Q_FUNC_INFO, "END-1", Debug);
            return true;

        break;
        case QMessageBox::Discard:
            logEvent(Q_FUNC_INFO, "END-1", Debug);
            return false; //No more error shown
            //  break;
        default:
            // should never be reached
            logEvent(Q_FUNC_INFO, "END-3", Debug);
        return false;   // The user wants to keepseeing errors
    //break;
    }
}

bool MainWindow::checkIfNewMode(const QString &_mode)
{
        //qDebug() << "MainWindow::checkIfNewMode: " << _mode ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (dataProxy->getIdFromModeName(_mode)<0)
    {// The mode is not existing; it is not an accepted mode for KLog
     // TODO: Show an error to the user
          //qDebug() << "MainWindow::checkIfNewMode: Mode not valid! - " << _mode ;

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - Non-supported mode"));

        msgBox.setIcon(QMessageBox::Warning);
        QString aux = tr("A new mode not supported by KLog has been received from an external program or radio:") + "(" + _mode + ")\n\n" + tr("If the received mode is correct, please contact KLog development team and request support for that mode") +  "\n\n" + tr("Do you want to keep receiving these alerts? (disabling these alerts will prevent non-valid modes being detected)");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );

        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Yes:
                logEvent(Q_FUNC_INFO, "END-1", Debug);
                return false;   // The user wants to keepseeing errors
    //break;
            case QMessageBox::No:
                logEvent(Q_FUNC_INFO, "END-1", Debug);
                return true; //No more error shown
      //  break;
            default:
    // should never be reached
                logEvent(Q_FUNC_INFO, "END-3", Debug);
            return false;   // The user wants to keepseeing errors
    //break;
        }
    }
    else
    {
        //noMoreModeErrorShown = false;
        //TODO: Add the new mode to the list of active modes
          //qDebug() << "MainWindow::checkIfNewMode: VALID NEW MODE: Adding... - " << _mode ;
        addNewValidMode(_mode);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
    return false;
}

void MainWindow::slotWSJXstatusFromUDPServer(const int _type, const QString &_dxcall, const double _freq, const QString &_mode,
                                             const QString &_report, const QString &_de_call, const QString &_de_grid,
                                             const QString &_dx_grid, const QString &_sub_mode)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    (void)_sub_mode;
    if (manualMode)
    {
        return;
    }
    if (modify)
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }
      //qDebug() << Q_FUNC_INFO << " type: " << QString::number(_type) ;
      //qDebug() << Q_FUNC_INFO << " dxcall: " << _dxcall ;
      //qDebug() << Q_FUNC_INFO << " freq: " << QString::number(_freq/1000000) ;
      //qDebug() << Q_FUNC_INFO << " mode: " << _mode ;
      //qDebug() << Q_FUNC_INFO << " report: " << _report ;
      //qDebug() << Q_FUNC_INFO << " de_call: " << _de_call ;
      //qDebug() << Q_FUNC_INFO << " _de_grid: " << _de_grid ;
      //qDebug() << Q_FUNC_INFO << " dx_grid: " << _dx_grid ;
      //qDebug() << Q_FUNC_INFO << " sub_mode: " << _sub_mode ;

    if ((!mainQSOEntryWidget->isModeExisting(_mode)) && (!noMoreModeErrorShown))
    {
        noMoreModeErrorShown = checkIfNewMode(_mode);
    }

    switch (_type)
    {
        case 0:
        //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT/IN - Heartbeat" ;
        break;
        case 1:
      //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT - Status" ;
             //qDebug() << Q_FUNC_INFO << " - calling setQRZ-3" ;
             mainQSOEntryWidget->setQRZ(_dxcall);
             if ((!noMoreModeErrorShown) && (dataProxy->getIdFromModeName(_mode)>0) )
             {
                 mainQSOEntryWidget->setMode(_mode);
        //modeComboBox->setCurrentIndex(modeComboBox->findText(_mode, Qt::MatchCaseSensitive));
             }
       //qDebug() << "MainWindow::slotWSJXstatusFromUDPServer updating txFreqSpinBox" << QString::number(_freq) ;
             QSOTabWidget->setTXFreq (_freq);
             QSOTabWidget->setRXFreq (_freq);
             if (!_dx_grid.isEmpty())
             {
                slotUpdateLocator(_dx_grid);
             }
            QSOTabWidget->setRSTTX (_report);
             myDataTabWidget->setMyLocator(_de_grid);
             myDataTabWidget->setStationCallsign(_de_call.toUpper());

     //TODO: Check what to do with _de_call -> Check if _de_call == station callsign and update if needed.
     //TODO: Check what to do with _de_grid -> Check if _de_grid == My Grid and update if needed.
     //TODO: Check what to do with _submode.


     //bandComboBox->setCurrentIndex(bandComboBox->findText(, Qt::MatchCaseSensitive));
            break;

        default: //NO
        //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - ERROR on Type" ;
        break;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::addNewValidMode(const QString &_mode)
{
        //qDebug() << "MainWindow::addNewMode: " << _mode ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QStringList _newM;
    _newM.clear();
    _newM << _mode;
    _newM << mainQSOEntryWidget->getModes();

    readActiveModes (_newM);
    mainQSOEntryWidget->setModes(modes);
    mapWindow->setModes(modes);

    logEvent(Q_FUNC_INFO, "END", Debug);
        //qDebug() << "MainWindow::addNewValidMode: END"  ;
}

void MainWindow::slotClearNoMorErrorShown()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    noMoreErrorShown = false;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString queryFailed)
{
  //qDebug() << Q_FUNC_INFO << " -  Function: " << functionFailed ;
  //qDebug() << Q_FUNC_INFO << " -  Error: - " << errorCodeS;
  //qDebug() << Q_FUNC_INFO << " -  Native: - " << nativeError;
  //qDebug() << Q_FUNC_INFO << " -  QueryFailed: - " << queryFailed;

    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (noMoreErrorShown)
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }
    QString aux;
    bool showDebug = true;

    if (nativeError.toInt() == 2067)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        if (functionFailed == "int QSO::toDB(int)")
        {
                msgBox.setWindowTitle(tr("KLog - QSO Dupe"));
                msgBox.setText(tr("A dupe QSO has been detected in the file and will not be added to the log."));
                msgBox.setInformativeText(tr("Please check the QSO information file and ensure it is properly added.") );
                showDebug = false;
        }
        else if (functionFailed == "virtual bool DataProxy_SQLite::addSatellite(QString, QString, QString, QString, QString)")
        {
            msgBox.setWindowTitle(tr("KLog - Duplicated satellite"));
            msgBox.setText(tr("A duplicated satellite has been detected in the file and will not be imported."));
            msgBox.setInformativeText(tr("Please check the satellite information file and ensure it is properly populated.") + "\n" + tr("Now you will see a more detailed error that can be used for debugging..."));
        }
        msgBox.exec();
        return;
    }
    if (showDebug)
    {
        // TODO: An error on DB has been detected.
        // KLog should suggest to export ALL the data to an ADIF file to prevent any log lose

        aux = "<br><b>" + tr("An unexpected error ocurred!!") + "</b><br><br>" + tr("If the problem persists, please contact the developers") + "(<a href=mailto:klog@groups.io>klog@groups.io</a>)" + tr("for analysis:") + "<br>";
        QString errorMSG =  "<ul>"
                        "<li><b>" + tr("Error in function") + ":</b> " + functionFailed + "</li>" +
                        "<li><b>" + tr("Native Error") +":</b> " + nativeError + "</li>" +
                        "<li><b>" + tr("Error text") + ":</b> " + errorCodeS + "</li>" +
                        "<li><b>" + tr("Failed query") + ":</b> " + queryFailed + "</li>" +
                        "</ul><br>" +
                        "<b>" + tr("Recommendation:") + "</b>" + tr("Periodically export your data to ADIF to prevent a potential data loss.") + "<br>";

        showErrorDialog->setText(aux + errorMSG);
    //showErrorDialog->setModal(true);
        showErrorDialog->exec();
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - Show errors"));
    msgBox.setIcon(QMessageBox::Question);
    aux = tr("Do you want to keep showing errors?");
    msgBox.setText(aux);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Ok );

    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret)
    {
        case QMessageBox::Ok:
        break;
        case QMessageBox::No:
            noMoreErrorShown = true;
            break;
        default:
        // should never be reached
        break;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotDefineNewBands (const QStringList _bands)
{
          //qDebug() << "MainWindow::defineNewBands: "  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QStringList qsTemp;
    qsTemp.clear();

    //qsTemp << _bands;
    //qsTemp.removeDuplicates();

    qsTemp << dataProxy->sortBandNamesBottonUp(_bands);
    qsTemp.removeDuplicates();

    bands.clear();
    bands = qsTemp;
    mainQSOEntryWidget->setBands(bands);
    mapWindow->setBands(bands);

    satTabWidget->addBands(bands);
          //qDebug() << "MainWindow::defineNewBands - END"  ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotHamlibTXFreqChanged(const double _f)
{
      //qDebug() << Q_FUNC_INFO << ": " << QString::number(_f) ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (manualMode)
    {
        return;
    }
    if (upAndRunning)
    {
        if (!util->isSameFreq (_f, QSOTabWidget->getTXFreq ()))
        {
      //qDebug() << Q_FUNC_INFO << ": Updating the freq... " ;
            QSOTabWidget->setTXFreq (_f);
        }
        else
        {
     //qDebug() << "MainWindow::slotHamlibTXFreqChanged - Not updating Freq" ;
        }
    }
    else
    {
          //qDebug() << "MainWindow::slotHamlibTXFreqChanged - Not Up&Running" ;
    }
      //qDebug() << "MainWindow::slotHamlibTXFreqChanged - END " ;

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotHamlibModeChanged(const QString &_m)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (manualMode)
    {
        return;
    }
    if (_m.length()<2)
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }

    if ((mainQSOEntryWidget->getMode()).toUpper() == _m.toUpper())
    {
        return;
    }

    hamlibChangingMode = true;
    if (checkIfNewMode(_m))
    {
        hamlibChangingMode = false;
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return;
    }
    if (mainQSOEntryWidget->isModeExisting(_m))
    {
       //qDebug() << "slotHamlibModeChanged: Mode in the Combobox: " ;
        mainQSOEntryWidget->setMode(_m);
        //modeComboBox->setCurrentIndex(modeComboBox->findText(_m, Qt::MatchCaseSensitive));
    }
    else
    {
          //qDebug() << "MainWindow::slotHamlibModeChanged: Mode not found in combobox" << _m ;
    }
    hamlibChangingMode = false;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

/*
void MainWindow::slotRotatorShow()
{
    rotatorWidget->show();
}
*/
void MainWindow::slotUpdateLocator(QString _loc)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
     //qDebug() << "MainWindow::slotUpdateLocator: " << _loc;
    QSOTabWidget->setDXLocator (_loc);
      //qDebug() << "MainWindow::slotUpdateLocator - END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::reconfigureDXMarathonUI(const bool _dxM)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    dxClusterWidget->setDXMarathon(_dxM);
    awardsWidget->setManageDXMarathon(_dxM);
    //awardsWidget->reconfigureDXMarathonUI(_dxM);

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowInfoLabel(const QString &_m)
{
    infoLabel1->setText(_m);
}

void MainWindow::slotAwardsWidgetSetLog()
{
    awardsWidget->setLog(currentLog);
}

void MainWindow::slotAwardsWidgetSetYear()
{
    awardsWidget->setYear(selectedYear);
}

void MainWindow::slotManualMode(bool _enable)
{
      //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (_enable);
    manualMode = _enable;
    if ((manualMode) && (hamlibActive))
    {
        hamlib->readRadio(true);
    }
}

void MainWindow::backupCurrentQSO()
{ // This function reads the full UI and stores it in a QSO
   //qDebug() << Q_FUNC_INFO << " - Start" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    backupQSO->clear();
    getQSODataFromUI();
    backupQSO->copy(qsoInUI);
    backupQSO->setBackup (true);
    backupQSO->setModifying (mainQSOEntryWidget->getModifying());
    backupQSO->setRealTime (mainQSOEntryWidget->getRealTime());
    backupQSO->setManualMode (mainQSOEntryWidget->getManualMode());
    backupQSO->setLogId (currentLog);

    backupQSO->setKeepComment (commentTabWidget->getKeep ());
    backupQSO->setKeepOthers (othersTabWidget->getKeep ());
    backupQSO->setKeepMyData (myDataTabWidget->getKeep ());   
    backupQSO->setKeepSatTab (satTabWidget->getKeep ());
   //qDebug() << Q_FUNC_INFO << ": Callsign: " << backupQSO->getCall();
    logEvent(Q_FUNC_INFO, "END", Debug);
   //qDebug() << Q_FUNC_INFO << " - END" ;
       //qDebug() << Q_FUNC_INFO << ": Realtime: " << util->boolToQString (backupQSO->getRealTime ());
}

void MainWindow::restoreCurrentQSO(const bool restoreConfig)
{ // This function restores a QSO that was backed up to the UI.
    // MainQSOEntryWidget
      //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (restoreConfig);
    clearUIDX();
    if (backupQSO->getModifying())
    {
        mainQSOEntryWidget->setModify(true);
    }
    //qso = backupQSO;
     //qDebug() << Q_FUNC_INFO << " - calling setQRZ-4" ;
    mainQSOEntryWidget->setQRZ(backupQSO->getCall ());
    mainQSOEntryWidget->setBand(backupQSO->getBand ());
    //mainQSOEntryWidget->setMode(backupQSO->getMode ());
    mainQSOEntryWidget->setMode(backupQSO->getSubmode());
    mainQSOEntryWidget->setDateTime (backupQSO->getDateTimeOn ());

      //qDebug() << Q_FUNC_INFO << ": restoring config: " << util->boolToQString (restoreConfig);
    if (restoreConfig)
    {
        mainQSOEntryWidget->setRealTime (backupQSO->getRealTime());
        manualMode = backupQSO->getManualMode();
          //qDebug() << Q_FUNC_INFO << ": manualMode: " << util->boolToQString (manualMode);
        mainQSOEntryWidget->setManualMode (manualMode);
    }

    //  MainWindowInputQSO
    QSOTabWidget->setRSTRX (backupQSO->getRSTRX ());
    QSOTabWidget->setRSTTX (backupQSO->getRSTTX ());
    QSOTabWidget->setTXFreq (backupQSO->getFreqTX ());
    QSOTabWidget->setRXFreq (backupQSO->getFreqRX ());
    QSOTabWidget->setDXLocator (backupQSO->getGridSquare ());
    QSOTabWidget->setName (backupQSO->getName ());
    QSOTabWidget->setQTH (backupQSO->getQTH ());
    QSOTabWidget->setRXPwr (backupQSO->getRXPwr ());
    // MainWindowInputQSL
    QSLTabWidget->setQSLSenStatus (backupQSO->getQSL_SENT ());
    QSLTabWidget->setQSLRecStatus (backupQSO->getQSL_RCVD ());
    QSLTabWidget->setQSLRecDate (backupQSO->getQSLRDate ());
    QSLTabWidget->setQSLSenDate (backupQSO->getQSLSDate ());

    QSLTabWidget->setQSLSenVia (backupQSO->getQSLSentVia());
    QSLTabWidget->setQSLRecVia (backupQSO->getQSLRecVia());
    QSLTabWidget->setQSLVia (backupQSO->getQSLVia ());
    QSLTabWidget->setQSLMsg (backupQSO->getQSLMsg ());

    // MainWindowInputEQSL
    eQSLTabWidget->setClubLogStatus (backupQSO->getClubLogStatus ());
    eQSLTabWidget->setClubLogDate (backupQSO->getClubLogDate ());
    eQSLTabWidget->setEQSLSenStatus (backupQSO->getEQSLQSL_SENT ());
    eQSLTabWidget->setEQSLSenDate (backupQSO->getEQSLQSLSDate ());
    eQSLTabWidget->setEQSLRecStatus (backupQSO->getEQSLQSL_RCVD ());
    eQSLTabWidget->setEQSLRecDate (backupQSO->getEQSLQSLRDate ());
    eQSLTabWidget->setLOTWSenStatus (backupQSO->getLoTWQSL_SENT ());
    eQSLTabWidget->setLOTWSenDate (backupQSO->getLoTWQSLSDate ());
    eQSLTabWidget->setLOTWRecStatus (backupQSO->getLoTWQSL_RCVD ());
    eQSLTabWidget->setLOTWRecDate (backupQSO->getLoTWQSLRDate ());
    eQSLTabWidget->setQRZCOMStatus (backupQSO->getQRZCOMStatus ());
    eQSLTabWidget->setQRZCOMDate (backupQSO->getQRZCOMDate ());

    // MainWindowInputComment
    commentTabWidget->setData (backupQSO->getComment ());
    commentTabWidget->setKeep (backupQSO->getKeepComment ());

    // MainWindowInputOthers

    othersTabWidget->setVUCCGrids (backupQSO->getVUCCGrids ());
    othersTabWidget->setSOTA (backupQSO->getSOTA_REF ());
    othersTabWidget->setAge(backupQSO->getAge ());
    othersTabWidget->setDistance(backupQSO->getDistance());

    othersTabWidget->setPOTA_REF(backupQSO->getPOTA_Ref());
    othersTabWidget->setSIG(backupQSO->getSIG());
    othersTabWidget->setSIG_INFO(backupQSO->getSIG_INFO());
    othersTabWidget->setWWFF_Ref(backupQSO->getWWFF_Ref());

    othersTabWidget->setEntity (backupQSO->getDXCC ());
    othersTabWidget->setIOTA (backupQSO->getIOTA ());
     //qDebug() << Q_FUNC_INFO << ": Restoring propMode";
    othersTabWidget->setPropMode (backupQSO->getPropMode ());
     //qDebug() << Q_FUNC_INFO << ": Restoring Keep";
    othersTabWidget->setKeep (backupQSO->getKeepOthers ());
     //qDebug() << Q_FUNC_INFO << ": END";

    // MainWindowMyDataTab
    if (backupQSO->getLogId () == currentLog)
    {
        myDataTabWidget->setStationCallsign (backupQSO->getStationCallsign());
    }
    else
    {
        myDataTabWidget->setStationCallsign (stationCallsign);
    }
    myDataTabWidget->setMyPower (backupQSO->getTXPwr ());
    myDataTabWidget->setOperator (backupQSO->getOperatorCallsign());

    myDataTabWidget->setMyLocator (backupQSO->getMyGridSquare ());
    myDataTabWidget->setKeep (backupQSO->getKeepMyData ());
    myDataTabWidget->setMyRig (backupQSO->getMyRig ());
    myDataTabWidget->setMyAntenna (backupQSO->getMyAntenna ());
    myDataTabWidget->setMySOTA (backupQSO->getMySOTA_REF ());
    myDataTabWidget->setMyVUCCGrids (backupQSO->getMyVUCCGrids ());
    myDataTabWidget->setMyPota_ref(backupQSO->getMyPOTA_Ref());
    myDataTabWidget->setMySig(backupQSO->getMySig());
    myDataTabWidget->setMySig_info(backupQSO->getMySigInfo());
    myDataTabWidget->setMyWWFF_Ref(backupQSO->getMyWWFF_Ref());

        //MainWindowSatTab
    satTabWidget->setSatName (backupQSO->getSatName ());
    satTabWidget->setSatMode (backupQSO->getSatMode ());
    satTabWidget->setKeep (backupQSO->getKeepSatTab ());
    backupQSO->setBackup (false);
    backupQSO->clear();
      //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::setLogLevel(const DebugLogLevel _sev)
{
     //qDebug() << Q_FUNC_INFO << ": " << util->debugLevelToString(_sev);
    logEvent(Q_FUNC_INFO, "Start", Debug);
    logLevel = _sev;
    showKLogLogWidget->setLogLevel(logLevel);
    //setupDialog->setLogLevel(logLevel);

    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("Misc");
    settings.setValue ("DebugLog",util->debugLevelToString (logLevel));
    settings.endGroup ();

    tipsDialog->setLogLevel(logLevel);
    dataProxy->setLogLevel(logLevel);
    mainQSOEntryWidget->setLogLevel(logLevel);
    util->setLogLevel(logLevel);
    qsoInUI.setLogLevel(logLevel);
    othersTabWidget->setLogLevel(logLevel);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotTakeOverFocusToQSOTabWidget()
{
      //qDebug() << Q_FUNC_INFO;
    dxUpLeftTab->setCurrentIndex (0);
    QSOTabWidget->raise ();
    QSOTabWidget->setFocus ();
}

void MainWindow::slotTakeOverFocusToMainQSOInput()
{
      //qDebug() << Q_FUNC_INFO;
    mainQSOEntryWidget->raise();
    mainQSOEntryWidget->setFocus ();
    mainQSOEntryWidget->setFocusToOK ();
}

void MainWindow::slotCaptureDebugLogs(const QString &_func, const QString &_msg, DebugLogLevel _level)
{
    //qDebug() << Q_FUNC_INFO << _func << "/" << _msg << "/" << QString::number(_level);
    logEvent(_func, _msg, _level);
}


void MainWindow::slotNewLogLevel(DebugLogLevel l)
{
     //qDebug() << Q_FUNC_INFO;
    setLogLevel(l);
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("Misc");
    settings.setValue ("DebugLog", util->debugLevelToString(l));
    settings.endGroup ();
}

bool MainWindow::loadSettings()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
     //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);

      //qDebug() << Q_FUNC_INFO << " - 10 - General";
    QString value = settings.value ("Version").toString ();
    if (softwareVersion!=value)
    {
         //qDebug() << Q_FUNC_INFO << " - It seems it is a new version ";
         //qDebug() << Q_FUNC_INFO << QString("softwareversion: %1 / version: %2").arg(softwareVersion).arg(value);
        itIsANewversion = true;
    }
    currentLog = settings.value ("SelectedLog").toInt();

    setWindowSize (settings.value ("MainWindowSize").toSize ());

      //qDebug() << Q_FUNC_INFO << " - 20 - user";
    settings.beginGroup ("UserData");
    value = settings.value ("Callsign").toString ();
     //qDebug() << Q_FUNC_INFO << " stationCallSign: " << value;
    Callsign callsign(value);
    if (callsign.isValid())
    {
        mainQRZ = value;
    }
    my_ITUz = 0;
    my_CQz = 0;
    my_CQz = settings.value ("CQz").toInt ();
    my_ITUz = settings.value ("ITUz").toInt ();

    // We Select the log after the mainQRZ is defined to prevent call conflicts
    selectTheLog(currentLog);
    Locator locator;
    value = settings.value ("StationLocator").toString ();
    if ( locator.isValidLocator(value) )
    {
        myDataTabWidget->setMyLocator(value.toUpper());
        adifLoTWExportWidget->setDefaultMyGrid(value.toUpper());
    }
    settings.endGroup ();

    myDataTabWidget->loadSettings ();

    settings.beginGroup ("ClubLog");
    clublogSentDefault = settings.value("ClubLogSentDefault", "M").toString();
    settings.endGroup ();

    settings.beginGroup ("eQSL");
    eqslSentDefault = settings.value("eQSLSentDefault", "Q").toString();
    settings.endGroup ();

    settings.beginGroup ("QRZcom");
    qrzcomSentDefault = settings.value("QRZcomSentDefault", "M").toString();
    settings.endGroup ();

    settings.beginGroup ("LoTW");
    lotwSentDefault = settings.value("LoTWSentDefault", "Q").toString();
    settings.endGroup ();

    eQSLTabWidget->loadSettings();

      //qDebug() << Q_FUNC_INFO << " - 30 - modes";
    settings.beginGroup ("BandMode");
    QStringList listAux;
    listAux.clear();
    listAux << "SSB" << "CW";
    readActiveModes (settings.value("Modes", listAux ).toStringList ());

     //qDebug() << Q_FUNC_INFO << " - 31 - bands";
    listAux.clear();
    listAux << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    readActiveBands (settings.value("Bands", listAux).toStringList ());
    settings.endGroup ();

    logWindow->setColumns(settings.value ("LogViewFields").toStringList ());
   //qDebug() << Q_FUNC_INFO << " - 41 - logs";


      //qDebug() << Q_FUNC_INFO << " - 50 - dxcluster";
    settings.beginGroup ("DXCluster");

    // Get the server string from settings
    QString server = settings.value("DXClusterServerToUse", "dxfun.com:8000").toString();

    // Split the server string into host and port
    QStringList serverList = server.split(':');
    QString host = "dxfun.com";
    int port = 8000;
    // Check if the server string has both host and port
    if (serverList.size() == 2) {
        host = serverList.at(0); // Extract host
        port = serverList.at(1).toInt(); // Convert port to integer
    }
    dxClusterWidget->setDXClusterServer(host, port);

    dxClusterWidget->setSaveSpots(settings.value ("DXClusterSave", false).toBool ());
    dxClusterShowHF = settings.value ("DXClusterShowHF", true).toBool ();
    dxClusterShowVHF = settings.value ("DXClusterShowVHF", true).toBool ();
    dxClusterShowWARC = settings.value ("DXClusterShowWARC", true).toBool ();
    dxClusterShowWorked = settings.value ("DXClusterShowWorked", true).toBool ();
    dxClusterShowConfirmed = settings.value ("DXClusterShowConfirmed", true).toBool ();
    dxClusterShowAnn = settings.value ("DXClusterShowAnn", true).toBool ();
    dxClusterShowWWV = settings.value ("DXClusterShowWWV", true).toBool ();
    dxClusterShowWCY = settings.value ("DXClusterShowWCY", true).toBool ();
    dxclusterSendSpotsToMap = settings.value ("DXClusterSendToMap", true).toBool ();
    dxClusterWidget->loadSettings ();
    settings.endGroup ();

     //qDebug() << Q_FUNC_INFO << " - 60 - colors";
    settings.beginGroup ("Colors");
    newOneColor = settings.value("NewOneColor").value<QColor>();
    neededColor = settings.value("NeededColor").value<QColor>();
    workedColor = settings.value("WorkedColor").value<QColor>();
    confirmedColor = settings.value("ConfirmedColor").value<QColor>();
    defaultColor = settings.value("DefaultColor").value<QColor>();
    bool darkMode = settings.value("DarkMode", false).toBool ();

   //qDebug() << Q_FUNC_INFO << " - NewOneColor:    " << newOneColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - NewOneColor:    " << newOneColor.name();
   //qDebug() << Q_FUNC_INFO << " - NeededColor:    " << neededColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - WorkedColor:    " << workedColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - ConfirmedColor: " << confirmedColor.name(QColor::HexRgb);
   //qDebug() << Q_FUNC_INFO << " - DefaultColor:   " << defaultColor.name(QColor::HexRgb);

    settings.endGroup ();
    setColors(newOneColor, neededColor, workedColor, confirmedColor, defaultColor);

    setupDialog->loadDarkMode ();
    setDarkMode(darkMode);

      //qDebug() << Q_FUNC_INFO << " - 70 - misc";
    settings.beginGroup ("Misc");
    mainQSOEntryWidget->setRealTime (settings.value ("RealTime", true).toBool ());
    mainQSOEntryWidget->setShowSeconds (settings.value ("ShowSeconds", false).toBool ());
    useDefaultLogFileName = (settings.value ("UseDefaultName", true).toBool ());
    imperialSystem = (settings.value ("ImperialSystem", false).toBool ());
    sendQSLWhenRec = (settings.value ("SendQSLWhenRec", true).toBool ());
    manageDxMarathon = (settings.value ("ManageDXMarathon", false).toBool ());
    awardsWidget->setManageDXMarathon (manageDxMarathon);
    searchWidget->setShowCallInSearch(settings.value ("ShowCallsignInSearch", true).toBool ());
    checkNewVersions = settings.value ("CheckNewVersions", true).toBool ();
    reportInfo = false;
    alwaysADIF = settings.value ("AlwaysADIF", true).toBool ();
    setLogLevel(util->stringToDebugLevel(settings.value ("DebugLog").toString ()));
    mainQSOEntryWidget->setUTC(settings.value ("UTCTime", true).toBool ());

    dupeSlotInSeconds = settings.value ("DuplicatedQSOSlot", 300).toInt ();
    filemanager->setDuplicatedQSOSlot(dupeSlotInSeconds);
    mainQSOEntryWidget->setDuplicatedQSOSlot(dupeSlotInSeconds);
    completeWithPrevious = settings.value ("CompleteWithPrevious", true).toBool ();
    defaultADIFLogFile = settings.value ("DefaultADIFFile").toString ();
    deleteAlwaysAdiFile = settings.value ("DeleteAlwaysAdiFile", true).toBool ();


    //util->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
    //mainQSOEntryWidget->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
    //filemanager->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
    //adifLoTWExportWidget->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
    settings.endGroup ();

      //qDebug() << Q_FUNC_INFO << " - 90 - elog";
    settings.beginGroup ("ClubLog");
    clublogActive = settings.value ("ClubLogActive", false).toBool ();
    //setupDialog->setClubLogActive(clublogActive);
    clublogRealTime = settings.value ("ClubLogRealTime", false).toBool ();
    settings.endGroup ();
    elogClublog->loadSettings ();

    settings.beginGroup ("QRZcom");
    qrzcomActive = settings.value ("QRZcomActive", false).toBool ();
    //setupDialog->setQRZCOMAutoCheckActive(QRZCOMAutoCheckAct->isChecked());
    qrzcomSubscriber = settings.value ("QRZcomSubscriber", false).toBool ();
    elogQRZcom->setSubcription (qrzcomSubscriber);
    QRZCOMAutoCheckAct->setChecked(settings.value ("QRZcomSubscriber", false).toBool ());
    //setupDialog->setQRZCOMAutoCheckActive(settings.value ("QRZcomSubscriber", false).toBool ());
    elogQRZcom->setUser (settings.value ("QRZcomUser").toString ());
    elogQRZcom->setPassword (settings.value ("QRZcomPass").toString ());
    elogQRZcom->setLogBookKey(settings.value ("QRZcomLogBookKey").toString ());
    settings.endGroup ();

    settings.beginGroup ("eQSL");
    eQSLActive = settings.value ("eQSLActive", false).toBool ();
    eqslUtilities->setUser(settings.value ("eQSLCall").toString ());
    eqslUtilities->setPass(settings.value ("eQSLPass").toString ());
    settings.endGroup ();

    settings.beginGroup ("LoTW");
    lotwActive = settings.value ("LoTWActive", false).toBool ();
    lotwTQSLpath = settings.value ("LoTWPath").toString ();
    lotwUtilities->setUser(settings.value ("LoTWUser").toString ());
    lotwUtilities->setPass(settings.value ("LoTWPass").toString ());
    settings.endGroup ();

      //qDebug() << Q_FUNC_INFO << " - 50 - UDPServer";

    UDPLogServer->loadSettings ();
    settings.beginGroup ("UDPServer");
    UDPServerStart = settings.value ("UDPServer", false).toBool ();
      //qDebug() << Q_FUNC_INFO << "UDPServer = " << util->boolToQString (UDPServerStart);
    //UDPLogServer->setNetworkInterface(settings.value ("UDPNetworkInterface").toString ());
    //UDPLogServer->setPort(settings.value ("UDPServerPort", 2237).toInt ());
    infoTimeout = settings.value ("InfoTimeOut", 2000).toInt ();
    //UDPLogServer->setLogging(settings.value ("LogFromWSJTX", false).toBool ());
    //UDPLogServer->setRealTimeUpdate(settings.value ("RealTimeFromWSJTX", false).toBool ());
    wsjtxAutoLog = settings.value ("LogAutoFromWSJTX", false).toBool ();

    settings.endGroup ();


      //qDebug() << Q_FUNC_INFO << " - 110 - Sats";

      //qDebug() << Q_FUNC_INFO << " - 120 - HamLib";
    settings.beginGroup ("HamLib");
    hamlib->loadSettings ();
    hamlibActive = settings.value ("HamlibActive").toBool ();
    settings.endGroup ();

    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << Q_FUNC_INFO << " - END";
    return true;
}

void MainWindow::setDarkMode(const bool _dm)
{
  othersTabWidget->setDarkMode(_dm);
  QSOTabWidget->setDarkMode(_dm);
  myDataTabWidget->setDarkMode(_dm);
}

void MainWindow::slotDarkModeChanged(const bool _dm)
{
   setDarkMode(_dm);
}

void MainWindow::selectTheLog(const int _i)
{
     //qDebug() << Q_FUNC_INFO;
    currentLog = _i;
    if (!dataProxy->doesThisLogExist(currentLog))
    {
        int _howManyQSOMax = -1;     // NUmber of QSO of the log with more QSO
        int _howManyQSOMaxT = 0;    // Number of QSO in ine specific log
        int _logWithMoreQSOs = -1;
        QStringList logs = QStringList();

        logs << dataProxy->getListOfManagedLogs();
        for (int i = 0;i<logs.length();i++)
        {
            _howManyQSOMaxT = dataProxy->getHowManyQSOInLog(i);
            if (_howManyQSOMax < _howManyQSOMaxT)
            {
                _howManyQSOMax = _howManyQSOMaxT;
                _logWithMoreQSOs = (logs.at(i)).toInt();
            }
        }
        if (_logWithMoreQSOs>0)
        {
            currentLog = _logWithMoreQSOs;
            QSettings settings(util->getCfgFile (), QSettings::IniFormat);
            settings.setValue ("SelectedLog", currentLog);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            QString aux = tr("It seems that there are no QSOs in the database.") + "\n\n" + tr("If you are sure that the database contains QSOs and KLog is not able to find them, please contact the developers (see About KLog) for help.");
            msgBox.setText(aux);
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

    defineStationCallsign ();
    dxClusterWidget->setCurrentLog(currentLog);
    dxccStatusWidget->setCurrentLog(currentLog);
}

void MainWindow::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{   //This function is the only one not logging the activity
      //qDebug() << Q_FUNC_INFO << "_level: " << _func << "/" << _msg << "/" << util->debugLevelToString(_level);
      //qDebug() << Q_FUNC_INFO << "upAndRunning: " << util->boolToQString(upAndRunning);

    showKLogLogWidget->add(_func, _msg, _level);
    return;
    if (!upAndRunning)
    {
        showKLogLogWidget->add(_func, _msg, None);
        return;
    }

    if (logLevel<=_level)
        showKLogLogWidget->add(_func, _msg, _level);
}
