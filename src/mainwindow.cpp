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

#include "global.h"
#include <QtWidgets>
#include <QtSql>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "database.h"
#include "mainwindow.h"

void MainWindow::showNotWar()
{
   logEvent(Q_FUNC_INFO, "Start", Debug);
    /*

#include <QtGlobal>
#if QT_VERSION >= 0x050000
    #include <QApplication>
#else
    #include <QtGui/QApplication>
#endif

*/
    //qDebug() << QT_VERSION_STR;
    int callDXCC = world->getQRZARRLId(mainQRZ);

    int EURusId = 54;   // ADIF code Eu Russia
    int ASRusId = 15;   // ADIF code As Russia
    int KaRusId = 126;  // Kaliningrad
    int UkrId = 288;    // UKraine

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr("KLog - Stop the war in Ukraine!"));
    QString aux;
    aux.clear();
    #if defined(Q_OS_OSX)
    aux = tr("KLog - Stop the war in Ukraine!\n\n");
    #endif

     //qDebug() << "DXCC: " << QString::number(callDXCC);
    if ((callDXCC == EURusId) || (callDXCC == ASRusId) || (callDXCC == KaRusId))
    {
         //qDebug() << "RUSSIA";
        aux = aux + QString(tr("You are using a Russian call (%1).\n\nYou can probably help to stop the Russian war, people are dying.\n\nPlease call your leaders to stop the war.")).arg(mainQRZ);
    }
    else if (callDXCC == UkrId)
    {
         //qDebug() << "UKRAINE";
        aux = aux + tr("You are using an Ukranian call.\n\nPlease take care and protect yourself and your family. \n\nThe KLog developers wish you the best of luck in this very difficult moment.");
    }
    else
    {
         //qDebug() << "Other";
        aux = aux + tr("People are dying in the war of Russia against Ukraine.\n\nYou can help to stop the Russian war against Ukraine.\n\nPlease show your disagreement to your leaders, or in your social network to stop the war.");
    }

    msgBox.setText(aux);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

MainWindow::MainWindow(const QString &tversion)
{
    //qDebug() << Q_FUNC_INFO << ": " <<  " Ver: " << tversion << QTime::currentTime().toString("hh:mm:ss") ;
    //logEvent(Q_FUNC_INFO, "Start: " + _klogDir  + "/" + tversion, Debug);
    showKLogLogWidget = new ShowKLogLogWidget;
    showErrorDialog = new ShowErrorDialog();
    UDPLogServer = new UDPServer();
    util = new Utilities(Q_FUNC_INFO);

    qso = new QSO;

    softwareVersion = tversion;

    logLevel = Info;
    sendQSLByDefault = true; // This must be before reading the config
    dupeSlotInSeconds = 15;
    needToEnd = false;
    upAndRunning = false; // To define some actions that can only be run when starting the software

    QRZCOMAutoCheckAct = new QAction(tr("Check always the current callsign in QRZ.com"), this);

     //qDebug() << "MainWindow::MainWindow: Debug File: "<<  util->getDebugLogFile() ;


     //qDebug() << Q_FUNC_INFO << ": BEFORE HAMLIB " << QTime::currentTime().toString("hh:mm:ss") ;
    hamlib = new HamLibClass();
     //qDebug() << Q_FUNC_INFO << ": AFTER HAMLIB " << QTime::currentTime().toString("hh:mm:ss") ;

    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, softwareVersion);
    lotwUtilities = new LoTWUtilities(util->getHomeDir (), softwareVersion, Q_FUNC_INFO, dataProxy);
    eqslUtilities = new eQSLUtilities(Q_FUNC_INFO);
    mapWindow = new MapWindowWidget(dataProxy, this);

     //qDebug() << Q_FUNC_INFO << ": Before DXCCStatusWidget " << QTime::currentTime().toString("hh:mm:ss") ;
    dxccStatusWidget = new DXCCStatusWidget(dataProxy, Q_FUNC_INFO);
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
    logWindow = new LogWindow(dataProxy, this);
     //qDebug() << Q_FUNC_INFO << ": 00087: " << QTime::currentTime().toString("hh:mm:ss") ;

    searchWidget = new SearchWidget(dataProxy, this);
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
    world = new World(dataProxy, Q_FUNC_INFO);

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
    locator = new Locator();

    mainWidget = new QWidget(this);
     //qDebug() << Q_FUNC_INFO << ": 60 " << QTime::currentTime().toString("hh:mm:ss") ;

    dateTime = new QDateTime();
    dateTimeTemp = new QDateTime();
    // UI DX
    infoLabel2 = new QLabel(tr("DX Entity"));
    loggWinAct = new QAction(tr("&Log Window"), this);

      //qDebug() << Q_FUNC_INFO << ": dxclusterwidget to be created " << QTime::currentTime().toString("hh:mm:ss") ;
    //dxClusterWidget = new DXClusterWidget(dataProxy, dxclusterServerToConnect , dxclusterServerPort, this);
    dxClusterWidget = new DXClusterWidget(dataProxy, this);

     //qDebug() << Q_FUNC_INFO << ": Awards to be created " << QTime::currentTime().toString("hh:mm:ss") ;
    awards = new Awards(dataProxy, Q_FUNC_INFO);
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
    fileAwardManager = new FileAwardManager(dataProxy, Q_FUNC_INFO);

    lotwCallTQSL = new QAction(tr("Upload the queued QSOs to LoTW"), this);
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
    delete(elogClublog);
    delete(downloadcty);
    delete(world);
    delete(locator);
    delete(qso);
    delete(dateTime);
    delete(dateTimeTemp);
    delete(awards);
    delete(softUpdate);
    delete(filemanager);
    delete(fileAwardManager);
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

    QSettings settings(util->getSetFile (), QSettings::IniFormat);
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

void MainWindow::init()
{
     //qDebug() << Q_FUNC_INFO << " - Start - " << (QTime::currentTime()).toString("HH:mm:ss") ;
    logLevel = Debug;
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    //qDebug() << "MainWindow::init - 00" ;
    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());

    //qDebug() << "MainWindow::init - 000" ;
    setupDialog->init(softwareVersion, 0, configured);
     //qDebug() << "MainWindow::init - 01" ;
    filemanager->init();

    manualMode = false;
    qrzAutoChanging = false;

    QRZCOMAutoCheckAct->setCheckable(true);
    QRZCOMAutoCheckAct->setChecked(false);
    logEvents = true;
    hamlib->initClass();
    util->setCallValidation (true);
    infoLabel1T = QString();
    infoLabel2T = QString();
    qso->clear();

    //Default band/modes
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW";
     //qDebug() << "MainWindow::init - 00" << (QTime::currentTime()).toString("HH:mm:ss") ;

    hamlibActive = false;
    hamlibModeNotADIFSupported = false;
    hamlibChangingMode = false;
    yearChangedDuringModification = false;

    readingTheUI = false;
    itIsANewversion = false;

    setCleaning(false);
     //qDebug() << "MainWindow::init - 10" ;
    dxClusterWidget->init();

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
     //qDebug() << "MainWindow::init - 20" << (QTime::currentTime()).toString("HH:mm:ss") ;
    currentMode = 1;
    currentModeShown = currentMode;
    currentBand = 1;
    currentBandShown = currentBand;
    currentLog = 1;

    operatorQRZ = "";
    stationCallsign = "";
    mainQRZ = "";
    //myLocator = "";
    dxLocator ="";

    UDPServerStart = false;   // By default the UDP server is started

     //qDebug() << "MainWindow::init - 30" << (QTime::currentTime()).toString("HH:mm:ss") ;
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
     //qDebug() << "MainWindow::init - 40" << (QTime::currentTime()).toString("HH:mm:ss") ;
    clublogActive = false;
    clublogRealTime = false;

    eQSLActive = false;
    qrzcomActive = false;
    lotwActive = false;
    qrzcomSubscriber = false;

    callingUpdate = false; // to control whether the update is mannually launched or at the begining
    //previousQrz = "";
    setModifying(false);
     //qDebug() << "MainWindow::init - 50" << (QTime::currentTime()).toString("HH:mm:ss") ;
    selectedYear = (dateTime->currentDateTime()).date().year();
    loggWinAct->setShortcut(Qt::CTRL + Qt::Key_L);

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    clublogAnswer = -1;

    defaultColor.setNamedColor("slategrey");
    neededColor.setNamedColor("yellow");
    workedColor.setNamedColor("blue");
    confirmedColor.setNamedColor("red");
    newOneColor.setNamedColor("green");
     //qDebug() << "MainWindow::init - 60" << (QTime::currentTime()).toString("HH:mm:ss") ;
    bool existingData = QFile::exists(util->getKLogDBFile());
     //qDebug() << "MainWindow::init - 60.1" ;
    ctyDatFile = util->getCTYFile();
     //qDebug() << "MainWindow::init - 60.2" ;
    if (!existingData)
    {
         //qDebug() << "MainWindow::init - 61" ;
        world->create(ctyDatFile);
         //qDebug() << "MainWindow::init - 62" ;
    }
    else if (!world->hasSpecialEntities())
    {
         //qDebug() << "MainWindow::init - 63" ;
        QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Question);

        msgBox.setWindowTitle(tr("KLog - CTY.dat update"));
        msgBox.setText(tr("KLog needs to update the Entities database."));
        msgBox.setDetailedText(tr("You can update the entities database in Tools->Update cty.csv"));
        msgBox.setInformativeText(tr("Do you want to do it now?"));
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
         //qDebug() << "MainWindow::init - 69" << (QTime::currentTime()).toString("HH:mm:ss") ;
    }

     //qDebug() << "MainWindow::init - 70" << (QTime::currentTime()).toString("HH:mm:ss") ;

    //qDebug() << "MainWindow::init - Reading config file" ;
    if (QFile::exists(util->getSetFile ()))
    {
         //qDebug() << "MainWindow::init - We have settings, so we load them" ;
        configured = loadSettings ();
    }
    else if (QFile::exists(util->getCfgFile ()))
    {
        //qDebug() << "MainWindow::init - We have OLD settings, so we translate them" ;
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - Settings update"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("The settings system has been changed and KLog will update your settings file."));
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
       UpdateSettings settingsUpdate;
       if (settingsUpdate.updateFile ())
       {
           configured = loadSettings ();
       }

    }
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.setValue ("Version", softwareVersion);

    mapWindow->init();

    //qDebug() << "MainWindow::init - 71" << (QTime::currentTime()).toString("HH:mm:ss") ;
    logWindow->createlogPanel(currentLog);
     //qDebug() << "MainWindow::init - 72" << (QTime::currentTime()).toString("HH:mm:ss") ;
    awards->setManageModes(manageMode);
     //qDebug() << "MainWindow::init - 73" << (QTime::currentTime()).toString("HH:mm:ss") ;
    if (dataProxy->getNumberOfManagedLogs()<1)
    {
         //qDebug() << "MainWindow::init - 73.1" << (QTime::currentTime()).toString("HH:mm:ss") ;
        openSetup(6);
         //qDebug() << "MainWindow::init - 73.2" << (QTime::currentTime()).toString("HH:mm:ss") ;
    }
     //qDebug() << "MainWindow::init - 74" << (QTime::currentTime()).toString("HH:mm:ss") ;

     //qDebug() << "MainWindow::init - 75" << (QTime::currentTime()).toString("HH:mm:ss") ;
    awardsWidget->fillOperatingYears();
    awardsWidget->showAwards();
    awardsWidget->setManageDXMarathon(manageDxMarathon);

    dxClusterWidget->setCurrentLog(currentLog);
     //qDebug() << "MainWindow::init - 80" << (QTime::currentTime()).toString("HH:mm:ss") ;
     //qDebug() << "MainWindow::Init: calling Software update ..." << (QTime::currentTime()).toString("HH:mm:ss") ;
    if (checkNewVersions)
    {//reportInfo
        if (reportInfo)
        {
            softUpdate->addCall(stationCallsign);
        }
        softUpdate->needToUpdate();
    }
     //qDebug() << "MainWindow::init - 90" << (QTime::currentTime()).toString("HH:mm:ss") ;
    currentBandShown = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
     //qDebug() << "MainWindow::init - 91" << (QTime::currentTime()).toString("HH:mm:ss") ;
    currentModeShown = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
     //qDebug() << "MainWindow::init - 92" << (QTime::currentTime()).toString("HH:mm:ss") ;
    currentBand = currentBandShown;
    currentMode = currentModeShown;

    timerInfoBars = new QTimer(this);

     //qDebug() << "MainWindow::init: Calling createUI" << (QTime::currentTime()).toString("HH:mm:ss") ;
    createUI();
     //qDebug() << "MainWindow::init: Calling slotClearButtonClicked" << (QTime::currentTime()).toString("HH:mm:ss") ;
     //qDebug() << Q_FUNC_INFO << " - 100";
    slotClearButtonClicked(Q_FUNC_INFO);
     //qDebug() << Q_FUNC_INFO << " - 110";
    infoWidget->showInfo(-1);
     //qDebug() << Q_FUNC_INFO << " - 120";
    //lotwTQSLpath = util->getTQSLsPath() + util->getTQSLsFileName();
    world->readWorld ();
    upAndRunning = true;
    mainQSOEntryWidget->setUpAndRunning(upAndRunning);
     //qDebug() << Q_FUNC_INFO << " - 130";

    applySettings ();
     //qDebug() << "MainWindow::init: END" << (QTime::currentTime()).toString("HH:mm:ss") ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}


void MainWindow::createActionsCommon(){
// Functional widgets connections
//TODO: Reimplement the possibility to enter a QSO with enter inthe following widgets:
    //connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    logEvent(Q_FUNC_INFO, "Start", Debug);
    connect(util, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)));
     //qDebug() << Q_FUNC_INFO << " - Connecting QSO";
    connect(qso, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)));
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
    connect(logWindow, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(logWindow, SIGNAL(actionDeleteQSO ( int ) ), this, SLOT(slotQSODelete(int) ) );
    connect(logWindow, SIGNAL(deleteTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsDelete(QList<int>) ) );
    connect(logWindow, SIGNAL(exportToADIFTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsExportToADIF(QList<int>) ) );
    connect(logWindow, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(logWindow, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );

    //CLUSTER
    connect(dxClusterWidget, SIGNAL(dxspotclicked(QStringList)), this, SLOT(slotAnalyzeDxClusterSignal(QStringList) ) );
    connect(dxClusterWidget, SIGNAL(dxspotArrived(QString, QString, double)), this, SLOT(slotDXClusterSpotArrived(QString, QString, double) ) );

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

    connect(world, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );

    // QSL TAB
    connect(QSLTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );
    // SEARCH TAB
    connect(searchWidget, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(searchWidget, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(searchWidget, SIGNAL(logRefresh() ), this, SLOT(slotLogRefresh() ) );
    connect(searchWidget, SIGNAL(toStatusBar(QString) ), this, SLOT(slotUpdateStatusBar(QString) ) );
    connect(searchWidget, SIGNAL(requestBeingShown() ), this, SLOT(slotShowSearchWidget() ) );
    connect(searchWidget, SIGNAL(actionQSODelete( int ) ), this, SLOT(slotQSODelete(int) ) );
    connect(searchWidget, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(awards, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(awards, SIGNAL(awardDXCCUpdated()), this, SLOT(slotRefreshDXCCWidget()) );
    connect(awardsWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(awardsWidget, SIGNAL(requireCurrentLogSignal()), this, SLOT(slotAwardsWidgetSetLog()) );
    connect(awardsWidget, SIGNAL(requireCurrentYearSignal()), this, SLOT(slotAwardsWidgetSetYear()) );

    //DXCCWIDGET TAB
    //connect(dxccStatusWidget, SIGNAL(showQso(int)), this, SLOT(slotShowQSOFromDXCCWidget(int) ) );
    connect(dxccStatusWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(dxccStatusWidget, SIGNAL(showQsos(QList<int>)), this, SLOT(slotShowQSOsFromDXCCWidget(QList<int>) ) );
    connect(dxccStatusWidget, SIGNAL(fillInQSOSignal()), this, SLOT(fillQSOData()) );
    //connect(dxccStatusWidget, SIGNAL(updateAwards()), this, SLOT(slotShowAwards() ) );
    connect(filemanager, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(filemanager, SIGNAL(addQSOToList(QStringList)), this, SLOT(slotReceiveQSOListToShowFromFile(QStringList)) );

    //connect(scoreeWinAct, SIGNAL(triggered()), this, SLOT(slotScoreWinShow()));

    // UDPLogServer - WSJT-x
    connect(UDPLogServer, SIGNAL(status_update(int, QString, double, QString, QString, QString, QString, QString, QString)), this, SLOT(slotWSJXstatusFromUDPServer(int, QString, double, QString, QString, QString, QString, QString, QString) ) );
    connect(UDPLogServer, SIGNAL( logged_qso(QString, QString, QString, double, QString, QString, QString, QString, QString, QString, QString, QString, QDateTime, QDateTime, QString, QString, QString)), this, SLOT(slotWSJTXloggedQSO (QString, QString, QString, double, QString, QString, QString, QString, QString, QString, QString, QString, QDateTime, QDateTime, QString, QString, QString) ) );
    connect(UDPLogServer, SIGNAL(clearSignal(QString)), this, SLOT(slotClearButtonClicked(QString) ) );

    connect(this, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(setupDialog, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(setupDialog, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(setupDialog, SIGNAL(exitSignal(int)), this, SLOT(slotExitFromSlotDialog(int)) );
    //connect(setupDialog, SIGNAL(qrzcomAuto(bool)), this, SLOT(slotElogQRZCOMAutoCheckFromSetup(bool)) );
    connect(setupDialog, SIGNAL(finished(int)), this, SLOT(slotSetupDialogFinished(int)) );

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
    connect(satTabWidget, SIGNAL (satBandTXChanged(QString)), this, SLOT (slotSatBandTXComboBoxChanged(QString)));
    connect(satTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );

    connect(othersTabWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(othersTabWidget, SIGNAL(setPropMode(QString)), this, SLOT(slotSetPropModeFromOther(QString)) ) ;
    connect(downloadcty, SIGNAL(done(bool)), this, SLOT(slotWorldReload(bool)) );
    connect(timerInfoBars, SIGNAL(timeout()), this, SLOT(slotTimeOutInfoBars()) );
    connect(hamlib, SIGNAL(freqChanged(double)), this, SLOT(slotHamlibTXFreqChanged(double)) );
    connect(hamlib, SIGNAL(modeChanged(QString)), this, SLOT(slotHamlibModeChanged(QString)) );
    connect(lotwUtilities, SIGNAL(actionProcessLoTWDownloadedFile(QString)), this, SLOT(slotLoTWDownloadedFileProcess(QString)) );
    connect(adifLoTWExportWidget, SIGNAL(selection(QString, QString, QDate, QDate, ExportMode)), this, SLOT(slotADIFExportSelection(QString, QString, QDate, QDate, ExportMode)) );
    connect(adifLoTWExportWidget, SIGNAL(qsosToSend(QString, QList<int>, ExportMode)), this, SLOT(slotADIFExportSelection2(QString, QList<int>, ExportMode)) );
    connect(dataProxy, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(dataProxy, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );

    connect(showKLogLogWidget, SIGNAL(newLogLevel(DebugLogLevel)), this, SLOT(slotNewLogLevel(DebugLogLevel)) );
    //connect(this, SIGNAL(focusC), this, SLOT(slotTimeOutInfoBars()) );

    logEvent(Q_FUNC_INFO, "END", Debug);
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
     //qDebug() << "MainWindow::recommendBackupIfNeeded lastDate: " << util->getDateTimeSQLiteStringFromDateTime(lastBackupDate) << (QTime::currentTime()).toString(" HH:mm:ss")  ;
    bool backupNeeded = false;
    QString msg;
    if (lastBackupDate == QDateTime())
    {
        backupNeeded = true;
        msg = tr("It seems that you have never done a backup or exported your log to ADIF.");
    }
    else if (lastBackupDate.addMonths(1) < QDateTime::currentDateTime())
    {
         //qDebug() << "MainWindow::recommendBackupIfNeeded More than a month" << (QTime::currentTime()).toString(" HH:mm:ss")   ;
        backupNeeded = true;
        msg = tr("It seems that the latest backup you did is older than one month.");
    }

    if (backupNeeded)
    {
         //qDebug() << "MainWindow::recommendBackupIfNeeded We need to backup"  << (QTime::currentTime()).toString(" HH:mm:ss")  ;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);

        //msg = msg + tr("Do you want to backup your logs now?");
        //msgBox.setText(msg);
        msgBox.setWindowTitle(tr("Log backup recommended!"));
        msgBox.setText(msg);

        msgBox.setInformativeText(tr("It is a good practice to backup your full log regularly to avoid loosing data in case of a problem.\n"
                                     "Once you export your log to an ADIF file, you should copy that file to a safe place, like an USB drive, cloud drive, another computer, ...\n\n"
                                     "KLog will remind you to backup on a monthly basis.\n\n"));

        msgBox.addButton(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Yes:
            QString filename = util->getBackupADIFile();
     //qDebug() << "MainWindow::recommendBackupIfNeeded: Backup to: " << filename << (QTime::currentTime()).toString(" HH:mm:ss")   ;
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("KLog - Backup"));
            if (filemanager->adifLogExport(filename, 0)) // 0 will save ALL the logs)
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("The backup was done successfully"));
                msgBox.setInformativeText(tr("KLog will remind you to backup your data again in aprox one month."));
            }
            else
            {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("The backup was not properly done."));
                msgBox.setInformativeText(tr("It is recommended to backup your data periodically to prevent lose or corruption of your log."));
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
        msgBox.setInformativeText(tr("The setup will be open to allow you to do any new setup you may need."));
        msgBox.exec();
        openSetup(0);
    }
      //qDebug() << "MainWindow::checkIfNewVersion - END"  ;
}

void MainWindow::createStatusBar()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    statusBar()->showMessage(tr("Ready"));
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowMap()
{
    Coordinate center = locator->getLocatorCoordinate(world->getQRZLocator(stationCallsign));
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

void MainWindow::setMainWindowTitle()
{
    QString aux = dataProxy->getCommentsFromLog(currentLog);
    int numberOfQSOs = dataProxy->getHowManyQSOInLog (currentLog);
       //qDebug() << "MainWindow::setMainWindowTitle:  (comment): " << aux ;
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
     //qDebug() << "MainWindow::createUI" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    createStatusBar();
    setWindowTitle(tr("KLog"));
    createUIDX();
    createActionsCommon();
    createMenusCommon();

    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << "MainWindow::createUI-END" ;
}

void MainWindow::slotTimeOutInfoBars()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
       //qDebug() << "MainWindow::slotTimeOutInfoBars - Start" ;
    slotShowInfoLabel(infoLabel1T);
    //slotShowInfoLabel(infoLabel2T);
    //infoLabel1->setText(infoLabel1T);
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
     //qDebug() << "MainWindow::slotBandChanged: " << _b ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if ((!upAndRunning) || (_b.length()<2))
    {
         //qDebug() << "MainWindow::slotBandChanged: !upAndRunning or band short"  ;
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


    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);

      //qDebug() << "MainWindow:: - calling showStatusOfDXCC-02 " ;
    if (currentEntity>0)
    {
    showStatusOfDXCC(_qs);
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

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
     //qDebug() << "MainWindow:: - calling showStatusOfDXCC-01 " ;
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);
    showStatusOfDXCC(_qs);
    if (!modify)
    {
        QSOTabWidget->setRSTToMode(mainQSOEntryWidget->getMode(), readingTheUI);
    }

    //QString _modeSeen = mainQSOEntryWidget->getMode();
    if (hamlibActive && !manualMode)
    {
        hamlib->setMode (mainQSOEntryWidget->getMode());
    }
/*    if (_modeSeen == "SSB")
    {
        setModeFromFreq();
    }
*/
    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << "MainWindow::slotModeComboBoxChanged2: " << mainQSOEntryWidget->getMode() ;
}


void MainWindow::slotQRZReturnPressed()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
     //qDebug() << "MainWindow::slotQRZReturnPressed: "  ;
    if (mainQSOEntryWidget->getQrz().length()<=0)
    {
   //qDebug() << "MainWindow::slotQRZReturnPressed: no QRZ"  ;
        return;
    }
    readingTheUI = true;

    //int errorCode = 0;

    // Just to prepare or some tasks before reading DATA from UI

    QSqlQuery query;
    QString queryString = readDataFromUI();

       //qDebug() << "MainWindow::slotQRZReturnPressed: queryString: " << queryString ;

    if (queryString != "NULL")
    {
        if (!query.exec(queryString))
        {
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                query.finish();
           //qDebug() << "MainWindow::slotQRZReturnPressed: Query ERROR: (queryString): " << queryString ;
        //errorCode = query.lastError().nativeErrorCode();
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("KLog - Unexpected error"));
                msgBox.setIcon(QMessageBox::Warning);
                QString aux = tr("An unexpected error ocurred when trying to add the QSO to your log. If the problem persists, please contact the developer for analysis: ");
                msgBox.setText(aux + "MW-1: " + query.lastError().nativeErrorCode());
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
                logEvent(Q_FUNC_INFO, "END-1", Debug);
                return;
            }
            else
            {
                query.finish();
        //TODO: To move the following lines to this part to properly manage the query result!!
        //ret = true;
           //qDebug() << "MainWindow::slotQRZReturnPressed: QSO Added! " ;
                actionsJustAfterAddingOneQSO();
            }
        }
    else   // The QUERY string is NULL
        {
            logEvent(Q_FUNC_INFO, "END-2", Debug);
       //qDebug() << "MainWindow::slotQRZReturnPressed: QUERY string is NULL " ;
            readingTheUI = false;
            return;
        }

    yearChangedDuringModification = false;
    readingTheUI = false;
    QString lastLocator = dataProxy->getLocatorFromId(dataProxy->getLastQSOid());
     //qDebug() << Q_FUNC_INFO << ": Locator: " << lastLocator;
    mapWindow->addLocator(lastLocator, workedColor);

     //qDebug() << Q_FUNC_INFO << "Just before cleaning";
    slotClearButtonClicked(Q_FUNC_INFO);

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::actionsJustAfterAddingOneQSO()
{
       //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    int lastId = -1;
    needToSave = true;
    if (modify)
    {
       //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Modifying! " ;
       needToSave = true;
       if(modifyingQSO>0)
       {
           awards->setAwards(modifyingQSO);
           if (yearChangedDuringModification)
           {
               awardsWidget->fillOperatingYears();
               yearChangedDuringModification = false;
           }
           if ((clublogActive) && (clublogRealTime))
           {
         //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (Modifiying ClubLog) Lastid: "<< QString::number(lastId) ;
       // Delete QSO in CLubLog
               elogClublog->deleteQSO(clublogPrevQSO);
       // Add modified QSO in ClubLog
               elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(modifyingQSO));
           }
           else
           {
                   //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (No ClubLog) Lastid: "<< QString::number(lastId) ;
           }
           awards->setAwards(modifyingQSO);   //Update the DXCC award status
       }
       // CHECK WHAT WAS THE QSOID to add the awards, if needed
    }
    else
    {
   //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Not Modifying " ;
        lastId = dataProxy->getLastQSOid();
        if (lastId>=0)
        {
       //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Lastid: "<< QString::number(lastId) ;
            awards->setAwards(lastId);   //Update the DXCC award status
    // Send to CLUBLOG if enabled
            if ((clublogActive) && (clublogRealTime))
            {
           //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (Sending ClubLog) Lastid: "<< QString::number(lastId) ;
                elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(lastId));
            }
            else
            {
           //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (No ClubLog) Lastid: "<< QString::number(lastId) ;
            }
    //<CLUBLOG>
        }
        else
        {
       //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Lastid < 0 "<< QString::number(lastId) ;
        }
        //awards->setAwards(lastId);
    }
    logWindow->refresh();
    dxccStatusWidget->refresh();
    searchWidget->refresh();
    awardsWidget->showAwards ();
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO - END" ;
}

QString MainWindow::readDataFromUI()
{
     //qDebug() << "MainWindow::readDataFromUI: " ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (modify)
    {
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return readDataFromUIDXModifying();
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-3", Debug);
        return readDataFromUIDX();
    }
    //logEvent(Q_FUNC_INFO, "END", Debug);
}

QString MainWindow::readDataFromUIDX()
{
/*
If you make any change here, please update also readDataFromUIDXModifying to keep data integrity!
*/
     //qDebug() << "MainWindow::readDataFromUIDX:" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString tqrz = (mainQSOEntryWidget->getQrz()).toUpper();
    if (!util->isValidCall(tqrz))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - Not valid call"));
        QString aux = QString(tr("The callsign %1 is not a valid call. Do you really want to add this callsign to the log?") ).arg(tqrz);
        msgBox.setText(aux);
        msgBox.setInformativeText(tr("Adding non-valid calls to the log may create problems when applying for awards, exporting ADIF files to other systems or applications."));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Yes:
      // Ok was clicked
            break;
            case QMessageBox::No:
            return "NULL";
            default:
        // should never be reached
            break;
        }
    }
    QString stringQuery = "NULL";
    QString aux1, aux2, stringFields, stringData;
    //QString aux, aux2;
      //qDebug() << "MainWindow::readDataFromUIDX: Band: " << QString::number(currentBand) ;
      //qDebug() << "MainWindow::readDataFromUIDX: Mode: " << QString::number(currentMode)  ;
    int tband = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
    int tmode = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());

    QString tdate = util->getDateTimeSQLiteStringFromDateTime(mainQSOEntryWidget->getDateTime());
    QString ttime = (mainQSOEntryWidget->getTime()).toString("hh:mm:ss");

    QString trsttx = QSOTabWidget->getRSTTX ();
      //qDebug() << "MainWindow::readDataFromUIDX - RSTtx: " << trsttx ;
    QString trstrx = QSOTabWidget->getRSTRX ();

    int dxcc = world->getQRZARRLId(tqrz);
    //int dxcc2 = getDXCCFromComboBox();
    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
      //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) ;
      //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) ;
    dxcc = util->getNormalizedDXCCValue (dxcc);
    dxcc2 = util->getNormalizedDXCCValue (dxcc2);

      //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) ;
      //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) ;

    if (dxcc!=dxcc2)
    {
        QString dxccn1 = world->getEntityName(dxcc);
        dxccn1 = dxccn1 + " - " + world->getEntityMainPrefix(dxcc);

        QString dxccn2 = world->getEntityName(dxcc2);
        dxccn2 = dxccn2 + " - " + world->getEntityMainPrefix(dxcc2);

        QPushButton *button2 = new QPushButton(this);
        QPushButton *button1 = new QPushButton(this);

        button1->setText(world->getEntityMainPrefix(dxcc));
        button2->setText(world->getEntityMainPrefix(dxcc2));

        int ret;

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - Select correct entity"));
        msgBox.setText( tr("You have selected an entity:") + "\n\n"+"- "+dxccn2+"\n\n"+tr("that is different from the KLog proposed entity:") + "\n\n"+ "- "+dxccn1+"\n\n"
                        +tr("Click on the prefix of the correct entity or Cancel to edit the QSO again."));

        msgBox.addButton(button2, QMessageBox::AcceptRole);
        msgBox.addButton(button1, QMessageBox::ActionRole);
        msgBox.addButton(QMessageBox::Cancel);
        ret = msgBox.exec();

        if (ret == QMessageBox::AcceptRole)
        {
            dxcc = dxcc2;
        }
        else if (ret == QMessageBox::Cancel)
        {
            logEvent(Q_FUNC_INFO, "END-2", Debug);
            return  "NULL";
        }
    }

    aux1 = dataProxy->getContinentShortNameFromEntity(dxcc);
    if (dataProxy->isValidContinentShortName(aux1))
    {
        stringFields = stringFields + ", cont";
        stringData = stringData + ", '" + aux1 + "'";
    }

    int cqz = world->getEntityCqz(dxcc);
    int ituz = world->getEntityItuz(dxcc);

    aux1 = QSOTabWidget->getName();
    if (aux1.length()>1)
    {
        stringFields = stringFields + ", name";
        stringData = stringData + ", '" + aux1 + "'";
    }


    if (infoWidget->getDistance ()>0)
    {
        stringFields = stringFields + ", distance";
        stringData = stringData + ", '" + QString::number(infoWidget->getDistance ()) + "'";
    }

    aux1 = QSOTabWidget->getDXLocator();

    if ( locator->isValidLocator(aux1)  )
    {
        stringFields = stringFields + ", gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }
    //qDebug() << "MainWindow::readDataFromUIDX: Reading freq...: " << QString::number(QSOTabWidget->getTXFreq()) ;
    if ( QSOTabWidget->getTXFreq() > 0  )
    {
        aux1 = QString::number(QSOTabWidget->getTXFreq());
         //qDebug() << "MainWindow::readDataFromUIDX: Reading freq...: " << aux1 << "/" << tband ;

        if (dataProxy->isThisFreqInBand(dataProxy->getNameFromBandId(tband), aux1) )
        {
            stringFields = stringFields + ", freq";
            stringData = stringData + ", '" + aux1 + "'";
                 //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND OK" ;
        }
        else
        {
                 //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND NOK" ;
        }
    }

    if ( QSOTabWidget->getRXFreq() > 0  )
    {
         //qDebug() << "MainWindow::readDataFromUIDX: TX FREQ & RX FREQ ARE DIFFERENT AND != 0" ;
        aux1 = QString::number(QSOTabWidget->getRXFreq());
        stringFields = stringFields + ", freq_rx, band_rx";
        stringData = stringData + ", '" + aux1 + "', '" + QString::number(dataProxy->getBandIdFromFreq(QSOTabWidget->getRXFreq())) + "'";
        //stringData = stringData + ", '" + aux1 + ", " + QString::number(dataProxy->getBandIdFromFreq(rxFreqSpinBox->value ())) + "'";
    }
    aux1 = QSOTabWidget->getQTH();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", qth";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getOperator();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", operator";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getStationCallsign();
     //qDebug() << Q_FUNC_INFO << "StationCallSign: " << aux1;
    if (aux1.length()>2)
    {
        //lastStationQRZ = aux1.toUpper();
        stringFields = stringFields + ", station_callsign";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getMyLocator();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", my_gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getMyRig();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", my_rig";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getMyAntenna();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", my_antenna";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getMySOTA();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", my_sota_ref";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getMyVUCCGrids ();
    if (util->isValidVUCCGrids (aux1))
    {
        stringFields = stringFields + ", my_vucc_grids";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = commentTabWidget->getComment();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", comment";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QSLTabWidget->getQSLMsg();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", qslmsg";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(dxcc);
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", dxcc";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(cqz);
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", cqz";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(ituz);
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", ituz";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QSLTabWidget->getQSLVia();
    if (aux1.length()>3)
    {
        stringFields = stringFields + ", qsl_via";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(myDataTabWidget->getMyPower());
    if ((aux1.toFloat())>0.0f)
    {
        stringFields = stringFields + ", tx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(QSOTabWidget->getRXPwr ());
    if ((aux1.toFloat())>0.0f)
    {
        stringFields = stringFields + ", rx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    // OTHERS TAB
    // User Selectable field
    aux1 = othersTabWidget->getSOTA();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sota_ref";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(othersTabWidget->getAge());
    if (othersTabWidget->getAge()>0)
    {
        stringFields = stringFields + ", age";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = othersTabWidget->getVUCCGrids ();
    if (util->isValidVUCCGrids (aux1))
    {
        stringFields = stringFields + ", vucc_grids";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = othersTabWidget->getIOTA();
     //qDebug() << "MainWindow::readDataFromUIDX: IOTA: " << aux1 ;
    if (aux1.length() == 6) // EU-001
    {
       //qDebug() << "MainWindow::readDataFromUIDX: IOTA to be saved" ;
        stringFields = stringFields + ", iota";
        stringData = stringData + ", '" + aux1 + "'";
    }

    // SATS

    aux1 = satTabWidget->getSatName(); //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
     //qDebug() << Q_FUNC_INFO << ": " << aux1;
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_name";
        stringData = stringData + ", '" + aux1 + "'";
        aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
         //qDebug() << Q_FUNC_INFO << ": " << aux1;
        if (aux1.length()>0)
        {
            stringFields = stringFields + ", sat_mode";
            stringData = stringData + ", '" + aux1 + "'";
        }
    }

    keepSatPage = satTabWidget->getRepeatThis();

    aux1 = othersTabWidget->getPropModeFromComboBox();
     //qDebug() << Q_FUNC_INFO << ": " << aux1;
    if (util->isValidPropMode (aux1))
    {
        stringFields = stringFields + ", prop_mode";
        stringData = stringData + ", '" + aux1 + "'";
    }

    //CLUBLOG
    aux1 = eQSLTabWidget->getClubLogStatus(); //Y, N, M
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", clublog_qso_upload_date";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getClubLogDate()) + "'";
    }
    else if (aux1 == "N")
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'N'";
    }
    else if (aux1 == "M")
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'M'";
        stringFields = stringFields + ", clublog_qso_upload_date";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getClubLogDate()) + "'";
    }
    else //TODO: This should be equivalent to N?
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'N'";
    }
    //CLUBLOG
   aux1 = eQSLTabWidget->getEQSLSenStatus();
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", eqsl_qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLSenDate())+ "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", eqsl_qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLSenDate())+ "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", eqsl_qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLSenDate())+ "'";
    }
    else // N
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'N'";
    }

    aux1 = eQSLTabWidget->getEQSLRecStatus();
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", eqsl_qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLRecDate())+ "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", eqsl_qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLRecDate())+ "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", eqsl_qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLRecDate())+ "'";
    }
    else
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'N'";
    }

    // LOTW-SENT

    aux1 = eQSLTabWidget->getLOTWSenStatus();
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", lotw_qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWSenDate())+ "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", lotw_qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWSenDate())+ "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", lotw_qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWSenDate())+ "'";
    }
    else
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'N'";
    }

    // LOTW-RECEPTION
    //LOTW_QSLRDATE: (only valid if LOTW_RCVD is Y, I, or V)

    aux1 = eQSLTabWidget->getLOTWRecStatus();
    if (aux1 == "Y")
    {
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", lotw_qslrdate";
            stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWRecDate())+ "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "V")
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'V'";
        stringFields = stringFields + ", lotw_qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWRecDate())+ "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", lotw_qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWRecDate())+ "'";
    }
    else
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'N'";
    }

    aux1 = eQSLTabWidget->getQRZCOMStatus();
    if (aux1 == "Y")
    {
            stringFields = stringFields + ", qrzcom_qso_upload_status";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", qrzcom_qso_upload_date";
            stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getQRZCOMDate())+ "'";
    }
    else if(aux1 == "M")
    {
            stringFields = stringFields + ", qrzcom_qso_upload_status";
            stringData = stringData + ", 'M'";
            stringFields = stringFields + ", qrzcom_qso_upload_date";
            stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getQRZCOMDate())+ "'";
    }
    else
    {
            stringFields = stringFields + ", qrzcom_qso_upload_status";
            stringData = stringData + ", 'N'";
    }

    //QSLTABWidget
    // QSL SENT: Y/N/R/Q/I
    // QSL_VIA: B/D/E/M

    aux1 = QSLTabWidget->getQSLSenStatus();
    aux2 = QSLTabWidget->getSentVia();
       //qDebug() << "MainWindow::readDataFromUIDX: aux1: " << aux1 << " / aux2: " << aux2 ;

    //TODO: the aux2 switch is repeated and could be improved

    if (aux1=="Y")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", qslsdate";
        //stringData = stringData + ", '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "'";

        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLSenDate())+ "'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'R'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLSenDate())+ "'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", qslsdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLSenDate())+ "'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'N'";
        stringFields = stringFields + ", qsl_sent_via";
        stringData = stringData + ", 'B'";
    }

     // QSL RECEPTION
    //i = qslRecComboBox->currentIndex();
    //ii = qslRecViaComboBox->currentIndex();
    aux1 = QSLTabWidget->getQSLRecStatus(); // Y/N/R/I/V
    aux2 = QSLTabWidget->getRecVia();       // B/D/E/M

    if (aux1 == "Y")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLRecDate())+ "'";
        //stringFields = stringFields + ", confirmed";
        //stringData = stringData + ", '1'";
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 =="R")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'R'";
        //stringFields = stringFields + ", confirmed";
        //stringData = stringData + ", '0'";
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2=="D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2=="E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2=="M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
            stringData = stringData + ", 'B'";
        }
    }
    else if (aux1=="I")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLRecDate())+ "'";
        //stringFields = stringFields + ", confirmed";
        //stringData = stringData + ", '0'";
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2=="D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2=="E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2=="M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
            stringData = stringData + ", 'B'";
        }
    }
    else if (aux1=="V")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'V'";
        stringFields = stringFields + ", qslrdate";
        stringData = stringData + ", '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLRecDate())+ "'";
        //TODO: Check if the QSL has been received or not as this "V" could mask a received QSL as a Worked (0)
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2=="D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2=="E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2=="M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
            stringData = stringData + ", 'B'";
        }
    }
    else
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'N'";
        stringFields = stringFields + ", qsl_rcvd_via";
        stringData = stringData + ", 'B'";
    }

    // The data reading finish here. Now, we prepare the data to insert into the DB

    if (stringFields.startsWith(", ") )
    {
        stringFields.remove(0,2);
    }

    stringFields += ", call, bandid, modeid, qso_date, lognumber, rst_sent, rst_rcvd";

    if (stringFields.startsWith(", ") )
    {
        stringFields.remove(0,2);
    }

    if (stringData.startsWith(", ") )
    {
        stringData.remove(0,1);
    }

    stringData.remove(0,1);


    stringData += QString(", '%1', '%2', '%3', '%4', '%5', '%6', '%7'").arg(tqrz).arg(tband).arg(tmode).arg(tdate).arg(QString::number(currentLog)).arg(trsttx).arg(trstrx);

    if (stringData.startsWith(", ") )
    {
        stringData.remove(0,2);
    }

    stringQuery = "INSERT INTO log (" + stringFields + ") values (" + stringData +")" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
    return stringQuery;
}

QString MainWindow::readDataFromUIDXModifying()
{
       //qDebug() << "MainWindow::readDataFromUIDXModifying:" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QString tqrz = (mainQSOEntryWidget->getQrz()).toUpper();
    if (!util->isValidCall(tqrz))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - Not valid callsign"));
        QString aux = QString(tr("The callsign %1 is not a valid callsign. Do you really want to add this callsign to the log?") ).arg(tqrz);
        msgBox.setText(aux);
        msgBox.setInformativeText(tr("Adding non-valid calls to the log may create problems when applying for awards, exporting ADIF files to other systems or applications."));
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Yes:
      // Ok was clicked
            break;
            case QMessageBox::No:
            return "NULL";
            default:
        // should never be reached
            break;
        }

        logEvent(Q_FUNC_INFO, "END-1", Debug);
    }

       //qDebug() << "MainWindow::readDataFromUIDXModifying: " << tqrz ;

    QString stringQuery = "NULL";
    QString aux1, aux2;


    int tband = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
    int tmode = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
    QString tdate = util->getDateTimeSQLiteStringFromDateTime(mainQSOEntryWidget->getDateTime());
    //QString tdate = (mainQSOEntryWidget->getDate()).toString("yyyy/MM/dd");
    QString ttime = (mainQSOEntryWidget->getTime()).toString("hh:mm:ss");

    if ((mainQSOEntryWidget->getDate().year()) && (dateTimeTemp->date().year()))
    {
        yearChangedDuringModification = true;
    }

    QString trsttx = QSOTabWidget->getRSTTX();
    QString trstrx = QSOTabWidget->getRSTRX();

    int dxcc = world->getQRZARRLId(tqrz);
    int cqz = world->getEntityCqz(dxcc);
    int ituz = world->getEntityItuz(dxcc);

    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
       //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) ;
       //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) ;
    dxcc = util->getNormalizedDXCCValue (dxcc);
    dxcc2 = util->getNormalizedDXCCValue (dxcc2);
       //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) ;
       //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) ;

    if (dxcc!=dxcc2)
    {
        QString dxccPref1, dxccPref2;
        QString dxccn1 = world->getEntityName(dxcc);
        if (dxccn1.length()>0)
        {
            dxccPref1 = world->getEntityMainPrefix(dxcc);
            dxccn1 = dxccn1 + " - " + dxccPref1;
        }
        else
        {
            dxccn1 = tr("No DXCC");
            dxccPref1 = tr("None");
        }

        QString dxccn2 = world->getEntityName(dxcc2);
        if (dxccn2.length()>0)
        {
            dxccPref2 = world->getEntityMainPrefix(dxcc2);
            dxccn2 = dxccn2 + " - " + dxccPref2;
        }
        else
        {
            dxccn2 = tr("No DXCC");
            dxccPref2 = tr("None");
        }

        QPushButton *button2 = new QPushButton(this);
        QPushButton *button1 = new QPushButton(this);
        button1->setText(dxccPref1);
        button2->setText(dxccPref2);
        int ret;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - Select correct entity"));
        msgBox.setText( tr("You have selected an entity:") + "\n\n"+"- "+dxccn2+"\n\n"+tr("that is different from the KLog proposed entity:") + "\n\n- "+dxccn1+"\n\n"
                        +tr("Click on the prefix of the right entity or Cancel to correct."));

        msgBox.addButton(button2, QMessageBox::AcceptRole);
        msgBox.addButton(button1, QMessageBox::ActionRole);
        msgBox.addButton(QMessageBox::Cancel);
        ret = msgBox.exec();

        if (ret == QMessageBox::AcceptRole)
        {
            dxcc = dxcc2;
       //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 2: " << QString::number(dxcc2) ;
        }
        else if (ret == QMessageBox::ActionRole)
        {
       //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 1: " ;
        }
        else if (ret == QMessageBox::Cancel)
        {
            logEvent(Q_FUNC_INFO, "END-2", Debug);
       //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 2: " << QString::number(dxcc2) ;
            return  "NULL";
        }
    }

    QString updateString = "UPDATE log SET call = '" + tqrz + "', bandid = '" + QString::number(tband) + "', modeid = '" + QString::number(tmode) + "', qso_date = '" + tdate + "', rst_sent = '" + trsttx + "', rst_rcvd = '" + trstrx + "', lognumber = '" + QString::number(currentLog) + "', ";

    aux1 = dataProxy->getContinentShortNameFromEntity(dxcc);
    if (dataProxy->isValidContinentShortName(aux1))
    {
        updateString = updateString + "cont = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "cont = '', ";
    }

    aux1 = QSOTabWidget->getName();
    if (aux1.length()>1)
    {
        updateString = updateString + "name = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "name = '', ";
    }



    if (infoWidget->getDistance ()>0)
    {
        updateString = updateString + "distance = '";
        updateString = updateString + QString::number(infoWidget->getDistance ()) + "', ";
    }
    else
    {
        updateString = updateString + "distance = '', ";
    }

    aux1 = QSOTabWidget->getDXLocator ();
    if ( locator->isValidLocator(aux1)  )
    {
        updateString = updateString + "gridsquare = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "gridsquare = '', ";
    }

    if ( QSOTabWidget->getTXFreq() > 0  )
    {
        aux1 = QString::number(QSOTabWidget->getTXFreq());

        if (dataProxy->isThisFreqInBand(dataProxy->getNameFromBandId(tband), aux1) )
        //if (db->isThisFreqInBand(db->getBandNameFromID2(tband), aux1) )
        {
            updateString = updateString + "freq = '";
            updateString = updateString + aux1 + "', ";
        }
        else
        {
            updateString = updateString + "freq = '0', ";
        }
    }
    else
    {
        updateString = updateString + "freq = '0', ";
    }

    if ( (QSOTabWidget->getRXFreq()) > 0  )
    {
        aux1 = QString::number(QSOTabWidget->getRXFreq());
        updateString = updateString + "freq_rx = '";
        updateString = updateString + aux1 + "', ";

        updateString = updateString + "band_rx = '";
        updateString = updateString + QString::number(dataProxy->getBandIdFromFreq(QSOTabWidget->getRXFreq())) + "', ";
    }
    else
    {
        updateString = updateString + "freq_rx = '0', ";
        updateString = updateString + "band_rx = '', ";
    }

    aux1 = QSOTabWidget->getQTH();
    if (aux1.length()>2)
    {
        updateString = updateString + "qth = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "qth = '', ";
    }

    aux1 = myDataTabWidget->getOperator();

    if (util->isValidCall(aux1))
    {
        updateString = updateString + "operator = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "operator = '', ";
    }

    aux1 = myDataTabWidget->getStationCallsign();
    if (util->isValidCall(aux1))
    {
        updateString = updateString + "station_callsign = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "station_callsign = '', ";
    }

    aux1 = (myDataTabWidget->getMyLocator()).toUpper();

    if (locator->isValidLocator(aux1))
    {
        updateString = updateString + "my_gridsquare = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "my_gridsquare = '', ";
    }

    aux1 = (myDataTabWidget->getMyRig());
    if (aux1.length ()>0)
    {
        updateString = updateString + "my_rig = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "my_rig = '', ";
    }

    aux1 = (myDataTabWidget->getMyAntenna());
    if (aux1.length ()>0)
    {
        updateString = updateString + "my_antenna = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "my_antenna = '', ";
    }

    aux1 = (myDataTabWidget->getMySOTA());
    if (aux1.length ()>0)
    {
        updateString = updateString + "my_sota_ref = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "my_sota_ref = '', ";
    }

    aux1 = myDataTabWidget->getMyVUCCGrids ();
    if (util->isValidVUCCGrids (aux1))
    {
        updateString = updateString + "my_vucc_grids = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "my_vucc_grids = '', ";
    }

    aux1 = commentTabWidget->getComment();
    updateString = updateString + "comment = '";
    updateString = updateString + aux1 + "', ";

    aux1 = QSLTabWidget->getQSLMsg();
    if (aux1.length()>0)
    {
        updateString = updateString + "qslmsg = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "qslmsg = '', ";
    }

    aux1 = QString::number(dxcc);
     //qDebug() << "MainWindow::readDataFromUIDXModifying: DXCC=" << aux1 ;
    if (dataProxy->isValidDXCC(dxcc))
    {
        updateString = updateString + "dxcc = '";
        updateString = updateString + aux1 + "', ";
         //qDebug() << "MainWindow::readDataFromUIDXModifying: Saving DXCC=" << aux1 ;
    }
    else
    {
        updateString = updateString + "dxcc = '', ";
    }

    if ((cqz>0) && (cqz<41))
    {
        aux1 = QString::number(cqz);
        updateString = updateString + "cqz = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "cqz = '', ";
    }

    if ((ituz>0) && (ituz<91))
    {
        aux1 = QString::number(ituz);
        updateString = updateString + "ituz = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "ituz = '', ";
    }

    aux1 = QSLTabWidget->getQSLVia();
    //aux1 = qslViaLineEdit->text();
    if (aux1.length()>3)
    {
        updateString = updateString + "qsl_via = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "qsl_via = '', ";
    }

    if (myDataTabWidget->getMyPower()>0.0)
    {
        aux1 = QString::number(myDataTabWidget->getMyPower());
        updateString = updateString + "tx_pwr = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "tx_pwr = '', ";
    }


    if (QSOTabWidget->getRXPwr ()>0.0)
    {
        aux1 = QString::number(QSOTabWidget->getRXPwr ());
        updateString = updateString + "rx_pwr = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "rx_pwr = '', ";
    }
    // OTHERS TAB
    // User Selectable field

    aux1 = (othersTabWidget->getSOTA ());
    if (aux1.length ()>0)
    {
        updateString = updateString + "sota_ref = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "sota_ref = '', ";
    }

    aux1 = QString::number(othersTabWidget->getAge());
    if (othersTabWidget->getAge()>0)
    {
        updateString = updateString + "age = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "age = '', ";
    }

    aux1 = othersTabWidget->getVUCCGrids ();
    if (util->isValidVUCCGrids (aux1))
    {
        updateString = updateString + "vucc_grids = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "vucc_grids = '', ";
    }

    aux1 = othersTabWidget->getIOTA();
        //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA: " << aux1 ;
    if (aux1.length() == 6) // EU-001
    {
            //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA to be saved! " ;
        updateString = updateString + "iota = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "iota = '', ";
           //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) ;
    }
    bool satQSO = false;
    aux1 = satTabWidget->getSatName();   //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
        //qDebug() << "MainWindow::readDataFromUIDX: SAT2 modif " << aux1 ;
    if (aux1.length()>0)
    {
        updateString = updateString + "sat_name = '";
        updateString = updateString + aux1 + "', ";
        satQSO = true;
    }
    else
    {
        updateString = updateString + "sat_name = '', ";
    }

    aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
    if ((aux1.length()>0) && satQSO)
    {
        updateString = updateString + "sat_mode = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "sat_mode = '', ";
    }

    aux1 = othersTabWidget->getPropModeFromComboBox();
       //qDebug() << "MainWindow::readDataFromUIDX: PropMode:  " << aux1 ;
    if (util->isValidPropMode (aux1))
    {
             //qDebug() << "MainWindow::readDataFromUIDX: PropMode(1):  " << aux1 ;
        updateString = updateString + "prop_mode = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "prop_mode = '', ";
     //qDebug() << "MainWindow::readDataFromUIDX: PropMode(3):  " << aux1 ;
    }

    //CLUBLOG

    aux1 = eQSLTabWidget->getClubLogStatus(); //Y, N, M
    //qDebug() << Q_FUNC_INFO << ": ClubLogStatus" << aux1;
    if (aux1 == "Y")
    {
        updateString = updateString + "clublog_qso_upload_status = 'Y', ";
        //updateString = updateString + "clublog_qso_upload_date = '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "', ";
        updateString = updateString + "clublog_qso_upload_date = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getClubLogDate()) + "', ";
    }
    else if (aux1 == "N")
    {
        updateString = updateString + "clublog_qso_upload_status = 'N', ";
    }
    else if (aux1 == "M")
    {
        updateString = updateString + "clublog_qso_upload_status = 'M', ";
        updateString = updateString + "clublog_qso_upload_date = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getClubLogDate()) + "', ";
    }
    else //TODO: This should be equivalent to N?
    {
        updateString = updateString + "clublog_qso_upload_status = 'N', ";
    }
    //CLUBLOG


    // EQSL-SENT
    aux1 = eQSLTabWidget->getEQSLSenStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "eqsl_qsl_sent = 'Y', ";
        updateString = updateString + "eqsl_qslsdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLSenDate()) + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "eqsl_qsl_sent = 'R', ";
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "eqsl_qsl_sent = 'Q', ";
        updateString = updateString + "eqsl_qslsdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLSenDate()) + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "eqsl_qsl_sent = 'I', ";
        updateString = updateString + "eqsl_qslsdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLSenDate()) + "', ";
    }
    else // N
    {
        updateString = updateString + "eqsl_qsl_sent = 'N', ";
    }


   // EQSL-RECEPTION
    aux1 = eQSLTabWidget->getEQSLRecStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'Y', ";
        updateString = updateString + "eqsl_qslrdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLRecDate()) + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'R', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'I', ";
    }
    else if (aux1 == "V")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'V', ";
        updateString = updateString + "eqsl_qslrdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getEQSLRecDate()) + "', ";
    }
    else
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'N', ";
    }


    // LOTW-SENT
    aux1 = eQSLTabWidget->getLOTWSenStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "lotw_qsl_sent = 'Y', ";
        updateString = updateString + "lotw_qslsdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWSenDate()) + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "lotw_qsl_sent = 'R', ";
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "lotw_qsl_sent = 'Q', ";
        updateString = updateString + "lotw_qslsdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWSenDate()) + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "lotw_qsl_sent = 'I', ";
        updateString = updateString + "lotw_qslsdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWSenDate()) + "', ";
    }
    else
    {
        updateString = updateString + "lotw_qsl_sent = 'N', ";
    }


    // LOTW-RECEPTION
    //LOTW_QSLRDATE: (only valid if LOTW_RCVD is Y, I, or V)
    aux1 = eQSLTabWidget->getLOTWRecStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'Y', ";
        updateString = updateString + "lotw_qslrdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWRecDate()) + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'R', ";
    }
    else if (aux1 == "V")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'V', ";
        updateString = updateString + "lotw_qslrdate = '" +  util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWRecDate()) + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'I', ";
        updateString = updateString + "lotw_qslrdate = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getLOTWRecDate()) + "', ";
    }
    else
    {
        updateString = updateString + "lotw_qsl_rcvd = 'N', ";
    }

    // QRZCOM
    aux1 = eQSLTabWidget->getQRZCOMStatus(); //Y, N, M
    if (aux1 == "Y")
    {
        updateString = updateString + "qrzcom_qso_upload_status = 'Y', ";
        updateString = updateString + "qrzcom_qso_upload_date = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getQRZCOMDate()) + "', ";
    }
    else if (aux1 == "M")
    {
        updateString = updateString + "qrzcom_qso_upload_status = 'M', ";
        updateString = updateString + "qrzcom_qso_upload_date = '" + util->getDateSQLiteStringFromDate(eQSLTabWidget->getQRZCOMDate()) + "', ";
    }
    else //TODO: This is equivalent to N
    {
        updateString = updateString + "qrzcom_qso_upload_status = 'N', ";
    }
    // QRZCOM


    // QSL SENT
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    //int i = qslSentComboBox->currentIndex();
    //int ii = qslSentViaComboBox->currentIndex();
    aux1 = QSLTabWidget->getQSLSenStatus();
    aux2 = QSLTabWidget->getSentVia();
        //qDebug() << "MainWindow::readDataFromUIDXModifying: aux1: " << aux1 << " / aux2: " << aux2 ;

    if (aux1 == "Y")
    {
        updateString = updateString + "qsl_sent = 'Y', ";
        updateString = updateString + "qslsdate = '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLSenDate())  + "', ";

        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "qsl_sent = 'R', ";
        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "qsl_sent = 'Q', ";
        updateString = updateString + "qslsdate = '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLSenDate()) + "', ";

        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "qsl_sent = 'I', ";
        updateString = updateString + "qslsdate = '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLSenDate()) + "', ";

        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }
    }
    else
    {
        updateString = updateString + "qsl_sent = 'N', ";
        updateString = updateString + "qsl_sent_via = 'B', ";
    }


     // QSL RECEPTION
    //i = qslRecComboBox->currentIndex();
    //ii = qslRecViaComboBox->currentIndex();
    aux1 = QSLTabWidget->getQSLRecStatus();
    aux2 = QSLTabWidget->getRecVia();

    if (aux1 == "Y")
    {
        updateString = updateString + "qsl_rcvd = 'Y', ";
        updateString = updateString + "qslrdate = '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLRecDate()) + "', ";

        if (aux2 == "D")
        {
            updateString = updateString + "qsl_rcvd_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_rcvd_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_rcvd_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_rcvd_via = 'B', ";
        }
    }
    else if (aux1 == "R")
    {
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'R', ";
    //updateString = updateString + "confirmed = '0', ";
            if (aux2 == "D")
            {
                updateString = updateString + "qsl_rcvd_via = 'D', ";
            }
            else if (aux2 == "E")
            {
                updateString = updateString + "qsl_rcvd_via = 'E', ";
            }
            else if (aux2 == "M")
            {
                updateString = updateString + "qsl_rcvd_via = 'M', ";
            }
            else
            {
                updateString = updateString + "qsl_rcvd_via = 'B', ";
            }
    }
    else if (aux1 == "I")
    {
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'I', ";

            updateString = updateString + "qslrdate = '" +util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLRecDate()) + "', ";
            updateString = updateString + "qslrdate = '" +  + "', ";

    //updateString = updateString + "confirmed = '0', ";

            if (aux2 == "D")
            {
                updateString = updateString + "qsl_rcvd_via = 'D', ";
            }
            else if (aux2 == "E")
            {
                updateString = updateString + "qsl_rcvd_via = 'E', ";
            }
            else if (aux2 == "M")
            {
                updateString = updateString + "qsl_rcvd_via = 'M', ";
            }
            else
            {
                updateString = updateString + "qsl_rcvd_via = 'B', ";
            }
    }
    else if (aux1 == "V")
    {
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'V', ";
            updateString = updateString + "qslrdate = '" + util->getDateSQLiteStringFromDate(QSLTabWidget->getQSLRecDate()) + "', ";

            if (aux2 == "D")
            {
                updateString = updateString + "qsl_rcvd_via = 'D', ";
            }
            else if (aux2 == "E")
            {
                updateString = updateString + "qsl_rcvd_via = 'E', ";
            }
            else if (aux2 == "M")
            {
                updateString = updateString + "qsl_rcvd_via = 'M', ";
            }
            else
            {
                updateString = updateString + "qsl_rcvd_via = 'B', ";
            }
    }

    else
    {
        updateString = updateString + "qsl_rcvd = 'N', ";
        updateString = updateString + "qsl_rcvd_via = 'B', ";
    }


    keepSatPage = satTabWidget->getRepeatThis();


    // The data reading finish here. Now, we prepare the data to insert into the DB

    if ( updateString.endsWith(", ") )
    {
        updateString.chop(2);
    }

    stringQuery = updateString + " WHERE id = " + "'" + QString::number(modifyingQSO) + "'";
       //qDebug() << "MainWindow::readDataFromUIDXModifying: queryCreated: " << stringQuery ;
    logEvent(Q_FUNC_INFO, "END", Debug);
    return stringQuery;
}

void MainWindow::slotOKButtonClicked(){
    logEvent(Q_FUNC_INFO, "Start", Debug);
    slotQRZReturnPressed();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotQSOsExportToADIF(QList<int> _id)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_id.length ()), Debug);
    if (_id.length()<1)
    {
        return; // NO QSO TO EXPORT
    }
    //qDebug() << Q_FUNC_INFO << " - xxy";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    //qDebug() << Q_FUNC_INFO << fileName ;
    if ((!fileName.endsWith(".adi")) && ( !fileName.endsWith(".adif") ))
    {
  //qDebug() << "MainWindow::slotQSOsExportToADIF: Adding the .adi to the file" << fileName ;
        fileName = fileName +  ".adi";
    }
      //qDebug() << "MainWindow::slotQSOsExportToADIF-1: " << fileName ;
    filemanager->adifQSOsExport(fileName, _id);
      //qDebug() << "MainWindow::slotQSOsExportToADIF-3" ;
    showNumberOfSavedQSO(fileName, _id.count());
       //qDebug() << "MainWindow::slotQSOsExportToADIF - END" ;
}

void MainWindow::slotQRZcomUpload(QList<int> _id)
{
    logEvent(Q_FUNC_INFO, "Start: " + QString::number(_id.length ()), Debug);

    elogQRZcom->sendQSOs(_id);

     //qDebug() << "MainWindow::slotQRZcomUpload - END" ;
}
void MainWindow::slotQSOsDelete(QList<int> _id)
{
       //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(_id.length())  ;
    //foreach(int i, _id)
    //{
    //       //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(i)  ;
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

        foreach (int i, _id)
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
    dxUpRightTab->setCurrentIndex(dxUpRightTab->indexOf(searchWidget));
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


    QSettings settings(util->getSetFile (), QSettings::IniFormat);
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
            i = msgBox.exec();
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
        QSettings settings(util->getSetFile (), QSettings::IniFormat);
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
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
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
    if (_qrz.length()<1)
    {
        infoLabel1->clear();
        infoLabel2->clear();
        slotClearButtonClicked(Q_FUNC_INFO);
        logEvent(Q_FUNC_INFO, "END-Empty", Devel);
        return;
    }

    if (cleaning)
    {
         //qDebug()<< Q_FUNC_INFO << ": Cleaning" ;
        logEvent(Q_FUNC_INFO, "END-Cleaning", Devel);
        return;
    }

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

    //qDebug()<< Q_FUNC_INFO << ": running ..." ;
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    //int i;
    int dx_CQz = -1;
    int dxE_CQz = -1;
    int dx_ITUz = -1;
    int dxE_ITUz = -1;
    cleanQRZCOMreceivedDataFromUI();
     //qDebug()<< Q_FUNC_INFO << ": currentQRZ: " <<_qrz ;

    //QString pref = util->getPrefixFromCall(_qrz);
    //logEvent(Q_FUNC_INFO, QString("Call/Prefix: %1/%2").arg(_qrz).arg(pref), Devel);
    //currentEntity = world->getQRZARRLId(pref);
    //validar por que no puedo tirar o usar  el prefijo directamente
    currentEntity = world->getQRZARRLId(_qrz);
    /*
    if (pref.length ()>0)
    {
        currentEntity = world->getQRZARRLId(pref);
    }
    else
    {
        currentEntity = world->getQRZARRLId(_qrz);
    }
    */
    logEvent(Q_FUNC_INFO, QString("Entity: %1").arg(currentEntity), Devel);

    othersTabWidget->setEntity(currentEntity);
    dxE_CQz = world->getEntityCqz(currentEntity);
    dx_CQz = world->getQRZCqz(_qrz);
    dx_ITUz = world->getQRZItuz(_qrz);
    dxE_ITUz = world->getEntityItuz(currentEntity);
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

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBand) << QString::number(currentMode) << QString::number(currentLog);
     //qDebug()<< Q_FUNC_INFO << ": currentEntity: " << QString::number(currentEntity) ;
    if ( locator->isValidLocator(QSOTabWidget->getDXLocator()))
    {
        dxLocator = QSOTabWidget->getDXLocator();
    }
    else
    {
        dxLocator = world->getLocator(currentEntity);
    }

     //qDebug()<< Q_FUNC_INFO << ": Going to check the DXCC" ;
     //qDebug()<< Q_FUNC_INFO << ": current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) ;
        if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) || (dx_CQz != dxE_CQz) || (dx_ITUz != dxE_ITUz))
        {
     //qDebug()<< Q_FUNC_INFO << ": currentEntity=" << QString::number(currentEntity) << "/previousEntity=" << QString::number(previousEntity)  ;
            previousEntity = currentEntity;
            InValidCharsInPrevCall = false;
    //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
            infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
     //qDebug()<< Q_FUNC_INFO << ": calling showStatusOfDXCC-03 " ;
            showStatusOfDXCC(_qs);
            showDXMarathonNeeded(currentEntity, dx_CQz, mainQSOEntryWidget->getDate().year(), currentLog);
            othersTabWidget->setIOTAContinentFromEntity(currentEntity);
        }
        else if ((dx_CQz == dxE_CQz) || (dx_ITUz = dxE_ITUz))
        {
     //qDebug()<< Q_FUNC_INFO << ": 000" ;
    //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
        }
        else
        {
    //qDebug()<< Q_FUNC_INFO << ": Default: else" ;
        }

    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
     //qDebug()<< Q_FUNC_INFO << ": cursorP at the end : "  ;

  if (completeWithPrevious)
  {
    completeWithPreviousQSO(_qrz);
  }

  if (!modify)
    {
        searchWidget->setCallToSearch(_qrz);
         //qDebug() << Q_FUNC_INFO << " qrz.length>2: " << _qrz;
         //qDebug() << Q_FUNC_INFO << " qrzcomActive: " << util->boolToQString (qrzcomActive);
         //qDebug() << Q_FUNC_INFO << " QRZCOMAutoCheckAct: " << util->boolToQString (QRZCOMAutoCheckAct->isChecked());

        if (qrzcomActive && QRZCOMAutoCheckAct->isChecked() && (_qrz.length ()>2))
        {
     //qDebug()<< Q_FUNC_INFO << ": Checking QRZ.com";
            elogQRZcom->checkQRZ(_qrz);
        }
        else
        {
     //qDebug()<< Q_FUNC_INFO << ": NOT checking QRZ.com";
        }
    }
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
    dateTimeTemp = dateTime;
    modifyingQSO = -1;

    QSOTabWidget->setRSTToMode(mainQSOEntryWidget->getMode(), readingTheUI);
     //qDebug() << Q_FUNC_INFO << " - 10" ;
    if (currentBand < 0)
    {
        currentBand = defaultBand;
    }
    if (currentMode < 0)
    {
        currentMode = defaultMode;
         //qDebug() << "MainWindow::MainWindow: 12b - currentMode: " << QString::number(currentMode) ;
    }
     //qDebug() << Q_FUNC_INFO << " - 20" ;
    clublogAnswer = -1;
    clublogPrevQSO.clear();
     //qDebug() << "MainWindow::slotClearButtonClicked: - 11"  ;

    setCleaning(false);
     //qDebug() << Q_FUNC_INFO << " - 25" ;
     //qDebug() << "MainWindow::slotClearButtonClicked: Log: " << QString::number(currentLog) ;
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
         //qDebug() << Q_FUNC_INFO;
        clearUIDX();
        statusBar()->clearMessage();
         //qDebug() << Q_FUNC_INFO << ": NOT recovening the previous status...";
    }
     //qDebug() << "MainWindow::slotClearButtonClicked - currentMode = " << QString::number(currentMode) ;
    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << "MainWindow::slotClearButtonClicked - END" ;
}

void MainWindow::clearUIDX(bool _full)
{
     //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    mainQSOEntryWidget->clear();
    QSOTabWidget->clear();
    commentTabWidget->clear(_full);
    infoLabel1->clear();
    infoLabel2->clear();

    QSOTabWidget->clear();
    eQSLTabWidget->clear();
    QSLTabWidget->clear();
    othersTabWidget->clear(_full);
    infoWidget->clear();
    satTabWidget->clear(_full);
    myDataTabWidget->clear(_full);

    completedWithPreviousName = false;
    completedWithPreviousQTH = false;
    completedWithPreviousLocator = false;

    //qDebug() << "MainWindow::clearUIDX deciding wether to change or not the Freq: " << QString::number(QSOTabWidget->getTXFreq()) ;
    if (QSOTabWidget->getTXFreq()<=0)
    {
         //qDebug() << "MainWindow::clearUIDX Setting TX Freq from: " << QString::number(QSOTabWidget->getTXFreq()) ;
        QSOTabWidget->setTXFreq((dataProxy->getFreqFromBandId(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand()))).toDouble());
         //qDebug() << "MainWindow::clearUIDX Setting TX Freq to: " << QString::number(QSOTabWidget->getTXFreq()) ;
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
      //qDebug() << "MainWindow::closeEvent" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    saveWindowsSize();
    if (maybeSave())
    {
  //qDebug() << "MainWindow::closeEvent saving needed" ;
        dataProxy->unMarkAllQSO();
        dataProxy->compressDB();
        event->accept();
    }
    else
    {
  //qDebug() << "MainWindow::closeEvent not saving needed" ;
        event->ignore();
    }
      //qDebug() << "MainWindow::closeEvent-END" ;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

bool MainWindow::maybeSave()
{
     //qDebug() << "MainWindow::maybeSave" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString str = tr("The logfile has been modified.") + "\n" + tr("Do you want to save your changes?");

    if (alwaysADIF)
    {
         //qDebug() << "MainWindow::maybeSave - Saving" ;
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
             //qDebug() << "MainWindow::maybeSave - Use default file name" ;

                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("KLog - ADIF export"));
                    msgBox.setInformativeText(tr("It is important to export to ADIF and save a copy as a backup."));
                    if (filemanager->adifLogExport(defaultADIFLogFile, currentLog)) // 0 will save ALL the logs)
                    {
                        msgBox.setIcon(QMessageBox::Information);
                        msgBox.setText(tr("Saving the log was done successfully."));
                        msgBox.exec();
                 //qDebug() << "MainWindow::maybeSave - Log exported" ;
                        return true;
                    }
                    else
                    {
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText(tr("The ADIF export was not properly done."));
                        msgBox.exec();
                 //qDebug() << "MainWindow::maybeSave - ERROR Log not exported" ;
                        return false;
                    }
                }
                else
                {
                    logEvent(Q_FUNC_INFO, "END-2", Debug);
             //qDebug() << "MainWindow::maybeSave - Going to import??" ;
                    slotADIFImport();
                    return true;
            //return saveFileAs();
                }
            }
            else if (ret == QMessageBox::Cancel)
            {
                logEvent(Q_FUNC_INFO, "END-3", Debug);
         //qDebug() << "MainWindow::maybeSave - Cancel" ;
                return false;
            }
        }
        else
        {
     //qDebug() << "MainWindow::maybeSave - Not needing to save" ;
        }
    }
    else
    {
        //qDebug() << "MainWindow::maybeSave - Not Checking if needed to save" ;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
    return true;
}

void MainWindow::slotAWAImport()
{
       //qDebug() << "MainWindow::slotAWAImport" ;
    fileAwardManager->importNewAwardFile();
       //qDebug() << "MainWindow::slotAWAImport - END" ;
}

void MainWindow::createMenusCommon()
{
    //qDebug() << "MainWindow::createMenusCommon" ;
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
    printLogAct->setShortcut(Qt::CTRL + Qt::Key_P);
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
    exitAct->setShortcut(Qt::CTRL + Qt::Key_X);
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
     //qDebug() << "MainWindow::slotToolLoTWMarkAllQueuedThisLog"  ;
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
      //qDebug() << "MainWindow::slotLoTWDownloadedFileProcess: " << _fn ;
    QList<int> a;
    a.clear();
    a.append(filemanager->adifLoTWReadLog2(_fn, currentLog));
    QString aux;
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog - LoTW"));

    if (a.length()>0)
    {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Your log has been updated with the LoTW downloaded QSOs."));
        aux = QString(tr("KLog has updated %1 QSOs from LoTW.")).arg(a.length());
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
    //filemanager->adifLoTWReadLog(_fn);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotToolLoTWMarkAllQueued()
{
     //qDebug() << "MainWindow::slotToolLoTWMarkAllQueued"  ;
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
       //qDebug() << "MainWindow::callTQSL: " << lotwTQSLpath ;

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

   //qDebug() << "MainWindow::callTQSL-ok: " << QString::number(ok) ;

        switch (ok)
        {
        case 0: // success: all qsos submitted were signed and saved or signed and uploaded
             //qDebug() << "MainWindow::callTQSL: 0"  ;
    //msg = tr("All the QSOs were signed and uploaded with no error.");
    //msg = tr("TQSL finished with no error.");
    //msgBox.setIcon(QMessageBox::Information);
        break;
        case 1: // cancelled by user
             //qDebug() << "MainWindow::callTQSL: 1"  ;
            msg = tr("Error #1: The process was cancelled by the user or TQSL was not configured. No QSOs were uploaded.");
        break;
        case 2: // rejected by LoTW
             //qDebug() << "MainWindow::callTQSL: 2"  ;
            msg = tr("Error #2: Upload was rejected by LoTW, please check your data.");
            break;
        case 3: // unexpected response from TQSL server
             //qDebug() << "MainWindow::callTQSL: 3"  ;
            msg = tr("Error #3: The TQSL server returned an unexpected response.");
            break;
        case 4: // TQSL error
             //qDebug() << "MainWindow::callTQSL: 4"  ;
            msg = tr("Error #4: There was a TQSL error.");
            break;
        case 5: // TQSLlib error
             //qDebug() << "MainWindow::callTQSL: 5"  ;
            msg = tr("Error #5: There was a TQSLLib error.");
            break;
        case 6: // unable to open input file
             //qDebug() << "MainWindow::callTQSL: 6"  ;
            msg = tr("Error #6: It was not possible to open the input file.");
            break;
        case 7: // unable to open output file
             //qDebug() << "MainWindow::callTQSL: 7"  ;
            msg = tr("Error #7: It was not possible to open the ouput file.");
            break;
        case 8: // No QSOs were processed since some QSOs were duplicates or out of date range
             //qDebug() << "MainWindow::callTQSL: 8"  ;
            msg = tr("Error #8: No QSOs were processed since some QSOs were duplicates or out of date range.");
            break;
        case 9: // Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range
             //qDebug() << "MainWindow::callTQSL: 9"  ;
            msg = tr("Error #9: Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range.");
            break;
        case 10: // command syntax error
             //qDebug() << "MainWindow::callTQSL: 10"  ;
            msg = tr("Error #10: Command syntax error. KLog sent a bad syntax command.");
            break;
        case 11: // LoTW Connection error (no network or LoTW is unreachable)
             //qDebug() << "MainWindow::callTQSL: 11"  ;
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

        if (ok && util->isValidCall(stationCallToUse))
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
        //callsignTyped = true;
                 if (util->isValidCall(stationCallToUse))
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
         //qDebug() << "MainWindow::slotToolLoTWMarkAllYesThisLog"  ;
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
      //qDebug() << "MainWindow::slotToolLoTWMarkAllYes"  ;
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
       //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile: " << QString::number(_qs.length())  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    // Receiving:  modifiedQSOList << _call << _date + "-" +_time << _band << _mode << _qslrdate;
    // Must send QRZ-DX, Date-Time(yyyyMMdd-hhmmss), Band, Mode
    if (_qs.length()!=4)
    {
   //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid qso list received - length: " << QString::number(_qs.length()) ;
        return;
    }

    if (!util->isValidCall(_qs.at(0)))
    {
   //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid QRZ received - " << _qs.at(0) ;
        return;
    }

    if (dataProxy->getIdFromBandName(_qs.at(2))<0)
    {
  //qDebug() << "SMainWindow::slotReceiveQSOListToShowFromFile - NO valid BAND received"<< _qs.at(2) ;
        return;
    }
    if (dataProxy->getIdFromModeName(_qs.at(3))<0)
    {
   //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid Mode received" << _qs.at(3);
        return;
    }
    if (!util->isValidDateTimeFromString(_qs.at(1)))

    {
   //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile No valid date" ;
            return;
    }
       //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile ALL OK" ;
    //QStringList qsoToSend;
    //qsoToSend.clear();
    //qsoToSend << _qs.at(0) << _qs.at(1) << _qs.at(2) << _qs.at(3);
    showAdifImportWidget->addQSOToTheList(_qs);

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotAboutQt()
{
     //qDebug() << "MainWindow::slotAboutQt"  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QMessageBox::aboutQt(this,tr("About ..."));
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotHelpAboutAction()
{
         //qDebug() << "MainWindow::slotHelpAboutAction "  ;
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
     //qDebug() << "MainWindow::slotTipsAction "  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    tipsDialog->exec();


    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotHelpCheckUpdatesAction()
{
     //qDebug() << "MainWindow::slotHelpCheckUpdatesAction" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    callingUpdate = true;
    softUpdate->addCall(stationCallsign);
    softUpdate->needToUpdate(true);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowSoftUpdateResults(const bool _b)
{
        //qDebug() << "MainWindow::slotShowSoftUpdateResults: " ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (_b == true)
    {
        //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = TRUE " ;
    }
    else
    {
            //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = FALSE " ;
    }

    if (callingUpdate == true)
    {
        if (_b == false)
        {
                //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NOT NEEDED" ;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog - Update checking result"));
            msgBox.setText(tr("Congratulations!") + "\n\n" + tr("You already have the latest version."));
            msgBox.exec();
        }
        else
        {
                //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NEEDED" ;
        }
    }
    callingUpdate = false;
    logEvent(Q_FUNC_INFO, "END", Debug);
}



void MainWindow::slotLogWinShow()
{
         //qDebug() << "MainWindow::slotLogWinShow: "  ;
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
      //qDebug() << "MainWindow::slotSetup: " << QString::number(_page)  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    configured = false;
     //qDebug() << "MainWindow::slotSetup - 010"  ;
    backupCurrentQSO();
     //qDebug() << "MainWindow::slotSetup - 020"  ;
    openSetup(_page);
     //qDebug() << "MainWindow::slotSetup - END" ;
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
        defineStationCallsign(stationCallsign);
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
    if (qso->getBackup())
    {
         //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss") << Q_FUNC_INFO << " - 021 - ";
         //qDebug() << Q_FUNC_INFO << ": Restoring..." ;
        restoreCurrentQSO (QDialog::Accepted);
         //qDebug() << (QTime::currentTime()).toString ("HH:mm:ss") << Q_FUNC_INFO << " - 022 - " ;
    }
    else
    {
         //qDebug() << "MainWindow::slotSetupDialogFinished: NO Restoring QSO..." ;
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
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
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
    QDesktopServices::openUrl(QUrl("https://wiki.klog.xyz/"));
    logEvent(Q_FUNC_INFO, "END", Debug);
}



bool MainWindow::applySettings()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    if ((useDefaultLogFileName) && (defaultADIFLogFile.length()>0))
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
    searchWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    awards->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    mapWindow->setColors (workedColor, confirmedColor, defaultColor);
    dxClusterWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
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
    infoWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());

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
        defaultMode = dataProxy->getSubModeIdFromSubMode(mainQSOEntryWidget->getBand(0));
        //defaultMode = dataProxy->getSubModeIdFromSubMode((modeComboBox->itemText(0)));
         //qDebug() << "MainWindow::selectDefaultMode2: " << QString::number(defaultMode) ;
         //qDebug() << "MainWindow::selectDefaultMode2S: "  ;
    }

    //aux = dataProxy->getNameFromSubModeId(defaultMode);

    mainQSOEntryWidget->setMode(dataProxy->getNameFromSubModeId(defaultMode));
    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode)));

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
            if (aux.length()>0)
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
            if (aux.length()>0)
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
    //         //qDebug() << "MainWindow::createUIDX ;
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

    //othersTabWidget->setEntitiesList(world->getEntitiesNames());
    othersTabWidget->setEntitiesList(dataProxy->getEntitiesNames());
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
    dxUpRightTab->addTab(searchWidget, tr("Search"));

    dxBottonTab->addTab(logWindow, tr("Log"));
    dxBottonTab->addTab(dxClusterWidget, tr("DX-Cluster"));
    dxBottonTab->addTab(dxccStatusWidget, tr("DXCC"));

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

void MainWindow::fileExportADIF(const QString &_st, const QString &_grid, const QDate &_startDate, const QDate &_endDate)
{
    //qDebug() << Q_FUNC_INFO << ": " << _st ;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _grid, _startDate, _endDate, currentLog, ModeADIF);
    showNumberOfSavedQSO(fileName, qsos.count());
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::fileExportADIF2(const QString &_call, QList<int> _qsos)
{
    //qDebug() << Q_FUNC_INFO ;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    QList<int> qsos = filemanager->adifLogExportReturnList2(fileName, _call, _qsos, ModeADIF, currentLog);
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
    QList<int> qsos = filemanager->adifLogExportReturnList2(fileName, _callToUse, _qsos, ModeADIF, currentLog);
    showNumberOfSavedQSO(fileName, qsos.count());

      //qDebug() << "MainWindow::slotADIFExportAll-1: " << fileName ;
    //QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _callToUse, QString(), dataProxy->getFirstQSODateFromCall(_callToUse), dataProxy->getLastQSODateFromCall(_callToUse), -1, ModeADIF);
      //qDebug() << "MainWindow::slotADIFExportAll-3" ;
    //showNumberOfSavedQSO(fileName, qsos.count());

    //filemanager->adifLogExport(fileName, 0);
    logEvent(Q_FUNC_INFO, "END", Debug);
}
/*


*/

void MainWindow::fileExportLoTW(const QString &_st, const QString &_grid, const QDate &_startDate, const QDate &_endDate)
{
     //qDebug() << "MainWindow::fileExportLoTW  - Start: " << _st << "/" << _grid <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") ;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
   //qDebug() << "MainWindow::fileExportLoTW - no valid call" ;
        if (_st == "ALL")
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
    if ((!_startDate.isValid()) || (!_endDate.isValid()))
    {
   //qDebug() << "MainWindow::fileExportLoTW - no valid date" ;
        return;
    }

    //QString fileName = "klog-lotw-upload.adi";
    QString fileName = util->getLoTWAdifFile();


    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _grid, _startDate, _endDate, currentLog, ModeLotW);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
   //qDebug() << "MainWindow::fileExportLoTW NO QSOs" ;
        return;
    }
       //qDebug() << "MainWindow::fileExportLoTW - 50" ;
    bool uploadedToLoTW = callTQSL(fileName, _st);
    //bool uploadedToLoTW = true;
       //qDebug() << "MainWindow::fileExportLoTW - 51" ;

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
    if (!deleteAlwaysAdiFile){
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
       //qDebug() << "MainWindow::fileExportLoTW -END " ;
    }else{
         QFile::remove(fileName);
    }
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

    if (!util->isValidCall(_call))
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

    QList<int> qsos = filemanager->adifLogExportReturnList2(fileName, _call, _qsos, ModeLotW, currentLog);

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

  if (!util->isValidCall(_call))
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
  QList<int> qsos = filemanager->adifLogExportReturnList2(fileName, _call, _qsos, ModeClubLog, currentLog);

  if (qsos.count() <= 0)
  { // TODO: Check if errors should be managed.
       //qDebug() << Q_FUNC_INFO << " -  NO QSOs" ;
      msgBox.setWindowTitle(tr("KLog - ClubLog"));
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.setText(tr("The selection you have done does not include any QSO."));
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

void MainWindow::fileExportClubLog(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
    //qDebug() << Q_FUNC_INFO << "- Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") ;
    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
   //qDebug() << Q_FUNC_INFO << " - no valid call" ;
        if (_st == "ALL")
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
    if ((!_startDate.isValid()) || (!_endDate.isValid()))
    {
   //qDebug() << Q_FUNC_INFO << " - no valid date" ;
        return;
    }

    //QString fileName = "klog-clublog-upload.adi";
    QString fileName = util->getClubLogFile();

    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, QString(), _startDate, _endDate, currentLog, ModeClubLog);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
   //qDebug() << Q_FUNC_INFO << " NO QSOs" ;
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
    elogClublog->sendLogFile(fileName, qsos, overwrite);
    logWindow->refresh();
       //qDebug() << Q_FUNC_INFO << " -END " ;
}

void MainWindow::fileExportEQSL(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
      //qDebug() << Q_FUNC_INFO << " - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") ;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
   //qDebug() << Q_FUNC_INFO << "- no valid call" ;
        if (_st == "ALL")
        {
            msgBox.setWindowTitle(tr("KLog - eQSL"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("You need to select one station callsign to be able to send your log to eQSL.cc."));
            msgBox.setStandardButtons(QMessageBox::Ok );
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        return;
    }
    if ((!_startDate.isValid()) || (!_endDate.isValid()))
    {
   //qDebug() << Q_FUNC_INFO << "- no valid date" ;
        return;
    }

    //QString fileName = "klog-eqsl-upload.adi";
    QString fileName = util->getEQSLFile();

    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, QString(), _startDate, _endDate, currentLog, ModeEQSL);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
   //qDebug() << Q_FUNC_INFO << "NO QSOs" ;
        return;
    }

    eqslUtilities->sendLogFile(fileName, qsos);
    logWindow->refresh();

       //qDebug() << Q_FUNC_INFO << "-END " ;
}

void MainWindow::fileExportEQSL2(const QString &_call, QList<int> _qsos)
{
    //qDebug() << Q_FUNC_INFO << QString(" - Start:  QSOs: %2" ).arg(_qsos.length ());

    //QString fileName = "klog-eqsl-upload.adi";
    QString fileName = util->getEQSLFile();
    QList<int> qsos = filemanager->adifLogExportReturnList2(fileName, _call, _qsos, ModeEQSL, currentLog);

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

void MainWindow::slotADIFExportSelection(const QString &_st, const QString &_grid, const QDate &_startDate, const QDate &_endDate, const ExportMode _eM)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << _st << "/" <<_grid << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") ;
    //qDebug() << Q_FUNC_INFO << " - Trigered by signal: selection";
    switch (_eM)
    {
    case ModeADIF:         // General ADIF
         //qDebug() << Q_FUNC_INFO << " - ADIF" ;
        fileExportADIF(_st, _grid, _startDate, _endDate);
        break;
    case ModeLotW:         // LoTW
         //qDebug() << Q_FUNC_INFO << " - LoTW" ;
        fileExportLoTW(_st, _grid, _startDate, _endDate);
        break;
    case ModeClubLog:         // General ADIF
         //qDebug() << Q_FUNC_INFO << " - ClubLog" ;
        fileExportClubLog(_st, _startDate, _endDate);
        break;
    case ModeEQSL:         // General eQSL
         //qDebug() << Q_FUNC_INFO << " - eQSL" ;
        fileExportEQSL(_st, _startDate, _endDate);
        break;
    case ModeQRZ:         // General eQSL
         //qDebug() << Q_FUNC_INFO << " - QRZ.com" ;
        elogQRZcom->sendQSOs(dataProxy->getQSOsListQRZCOMToSent(_st, _startDate, _endDate, true));
        break;
    }
    logWindow->refresh();
       //qDebug() << Q_FUNC_INFO << " - END " ;
}

void MainWindow::slotADIFExportSelection2(const QString &_call, QList<int> _qsos, ExportMode _eM)
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
        filemanager->adifReadLog(fileName, currentLog);
        updateQSLRecAndSent();


          //qDebug() << "MainWindow::slotADIFImport -2" ;
        //dxccStatusWidget->refresh();

        logWindow->refresh();
         //qDebug() << "MainWindow::slotADIFImport -3";

        checkIfNewBandOrMode();
          //qDebug() << "MainWindow::slotADIFImport -4" ;

    //operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
        awardsWidget->fillOperatingYears();
              //qDebug() << "MainWindow::slotADIFImport-DX-1" ;
        slotShowAwards();
        awardsWidget->showAwards();
              //qDebug() << "MainWindow::slotADIFImport-DX-1-end" ;

          //qDebug() << "MainWindow::slotADIFImport-7" ;
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::slotADIFImport-END" ;
}

void MainWindow::qsoToEdit (const int _qso)
{
     //qDebug() << Q_FUNC_INFO  << QString::number(_qso) ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!modify)
    {
        backupCurrentQSO ();
    }

     //qDebug() << Q_FUNC_INFO;
    clearUIDX(true);
    readingTheUI = true;
    int nameCol;
    QString aux1;
    double testValueDouble; // Variable just to test if the value is in the appropriate range

    setModifying(true);
    modifyingQSO = _qso;

    //TODO: Optimize the following query. Maybe the * is not needed.
    QString stringQuery = QString("SELECT * FROM log WHERE id ='%1' AND lognumber='%2'").arg(_qso).arg(currentLog);
         //qDebug() << "MainWindow::qsoToEdit: " << stringQuery ;

    QSqlQuery query(stringQuery);
    bool sqlOK = query.exec();
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }

    query.next();
    if (query.isValid())
    {
        if ((clublogActive) && (clublogRealTime))
        {
            clublogPrevQSO = dataProxy->getClubLogRealTimeFromId(_qso);
        }

        QSqlRecord rec = query.record();

        manualMode = true;      // We stop hamlib & wsjtx receiving data while editing a QSO
        mainQSOEntryWidget->setManualMode (manualMode);

    // ADD THE DATA THAT IS PRESENT IN ALL THE MODES

        // ** Start of SAT data
        // ** BAND / MODE / Locator shoule be executed after SAT or may be removed

        nameCol = rec.indexOf("sat_name");
        aux1 = (query.value (nameCol)).toString();
        if (aux1.length()>0)
        {
            satTabWidget->setSatName(aux1);
        }
        else
        {
            satTabWidget->clear();
        }

        nameCol = rec.indexOf("sat_mode");
        aux1 = (query.value (nameCol)).toString();
        if (aux1.length()>1)
        {
            satTabWidget->setSatMode(aux1);
        }
        else
        {
            satTabWidget->setSatMode("-CLEAR-");
        }

        // END of SAT data


    //QString currentQrz = dataProxy->getCallFromId(modifyingQSO);
    nameCol = rec.indexOf("call");
    aux1 = (query.value (nameCol)).toString();
    mainQSOEntryWidget->setQRZ(aux1);

    QString currentQrz = aux1;
    currentEntity = world->getQRZARRLId(currentQrz);

     //qDebug() << "MainWindow::qsoToEdit - currentEntity " << QString::number(currentEntity) ;

    nameCol = rec.indexOf("qso_date");
    aux1 = (query.value (nameCol)).toString();
       //qDebug() << "MainWindow::qsoToEdit - date: " << aux1 ;
    mainQSOEntryWidget->setDateTime(util->getDateTimeFromSQLiteString(aux1));
    //mainQSOEntryWidget->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
    dateTimeTemp->setDate(util->getDateFromSQliteString(aux1));

    nameCol = rec.indexOf("bandid");
    aux1 = (query.value (nameCol)).toString();
    stringQuery = QString("SELECT name FROM band WHERE id ='%1'").arg(aux1);
    QSqlQuery queryAux(stringQuery);

    sqlOK = queryAux.exec();
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, queryAux.lastError().databaseText(), queryAux.lastError().nativeErrorCode(), queryAux.lastQuery());
    }

    queryAux.next();
    if (queryAux.isValid())
    {
        aux1 = (queryAux.value (0)).toString();
          //qDebug() << "MainWindow::qsoToEdit - bandid-1 " << aux1 ;
          //qDebug() << "MainWindow::qsoToEdit: - Changing from: " << mainQSOEntryWidget->getBand() ;
        mainQSOEntryWidget->setBand(aux1);
        //bandComboBox->setCurrentIndex(bandComboBox->findText(aux1, Qt::MatchCaseSensitive));
          //qDebug() << "MainWindow::qsoToEdit: - Changing to: " << mainQSOEntryWidget->getBand() ;
    }
    else
    {
          //qDebug() << "MainWindow::qsoToEdit - bandid-NO "  ;
        mainQSOEntryWidget->setBand(dataProxy->getNameFromBandId(defaultBand));
        //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
        //bandComboBox->setCurrentIndex(defaultBand);
    }

       //qDebug() << "MainWindow::qsoToEdit: Check mode " <<  QT_ENDL;
    nameCol = rec.indexOf("modeid");
    aux1 = (query.value (nameCol)).toString();
       //qDebug() << "MainWindow::qsoToEdit: (aux1)-1: " << aux1 ;


    aux1 = dataProxy->getNameFromSubModeId(aux1.toInt());
    QSOTabWidget->setRSTToMode(aux1, readingTheUI);

    if (mainQSOEntryWidget->isModeExisting(aux1))
    {
        mainQSOEntryWidget->setMode(aux1);
    }
    else
    {
        mainQSOEntryWidget->setMode(dataProxy->getNameFromSubModeId(defaultMode));
    }

    stringQuery = QString("SELECT submode FROM mode WHERE id ='%1'").arg(aux1);
    sqlOK = queryAux.exec(stringQuery);

      //qDebug() << "MainWindow::qsoToEdit: After ALL Mode actions" ;

    nameCol = rec.indexOf("rst_sent");
    aux1 = (query.value (nameCol)).toString();
    QSOTabWidget->setRSTTX (aux1);
       //qDebug() << "MainWindow::qsoToEdit: - RST_SENT: " << aux1  ;

    nameCol = rec.indexOf("rst_rcvd");
    aux1 = (query.value (nameCol)).toString();
    QSOTabWidget->setRSTRX (aux1);

         //qDebug() << "MainWindow::qsoToEdit: - before switch"  ;

        nameCol = rec.indexOf("qsl_via");
        aux1 = (query.value (nameCol)).toString();
        QSLTabWidget->setQSLVia(aux1);

        nameCol = rec.indexOf("qslmsg");
        aux1 = (query.value (nameCol)).toString();
        QSLTabWidget->setQSLMsg(aux1);

        //qslmsgTextEdit->setText(aux1);

        nameCol = rec.indexOf("comment");
        aux1 = (query.value (nameCol)).toString();
        if (aux1.length()>0)
        {
            commentTabWidget->setData(aux1);
    //commentLineEdit->setText(aux1);
        }
        else
        {
            commentTabWidget->clear();
    //commentLineEdit->clear();
        }

        nameCol = rec.indexOf("name");
        aux1 = (query.value (nameCol)).toString();
             //qDebug() << "MainWindow::qsoToEdit: - NAME: " << aux1  ;

        if (aux1.length()>0)
        {
            QSOTabWidget->setName (aux1);
        }
        else
        {
            QSOTabWidget->clearName ();
        }

        nameCol = rec.indexOf("qth");
        aux1 = (query.value (nameCol)).toString();
        QSOTabWidget->setQTH (aux1);

        nameCol = rec.indexOf("gridsquare");
        aux1 = (query.value (nameCol)).toString();
         //qDebug() << "MainWindow::qsoToEdit: - GRIDSQUARE: " << aux1  ;
        QSOTabWidget->setDXLocator(aux1);

        nameCol = rec.indexOf("operator");
        aux1 = (query.value (nameCol)).toString();
   //qDebug() << "MainWindow::qsoToEdit: - OPERATOR: " << aux1  ;
        myDataTabWidget->setOperator(aux1);

        nameCol = rec.indexOf("station_callsign");
        aux1 = (query.value (nameCol)).toString();
   //qDebug() << "MainWindow::qsoToEdit: - STATIONQRZ: " << aux1  ;
        myDataTabWidget->setStationCallsign(aux1);

        nameCol = rec.indexOf("my_gridsquare");
        aux1 = (query.value (nameCol)).toString();
        myDataTabWidget->setMyLocator(aux1);

        nameCol = rec.indexOf("my_vucc_grids");
        aux1 = (query.value (nameCol)).toString();
        myDataTabWidget->setMyVUCCGrids(aux1);

        nameCol = rec.indexOf("my_rig");
        aux1 = (query.value (nameCol)).toString();
        if (!aux1.isEmpty ())
        {
            myDataTabWidget->setMyRig (aux1);
        }

        nameCol = rec.indexOf("my_antenna");
        aux1 = (query.value (nameCol)).toString();
        if (!aux1.isEmpty ())
        {
            myDataTabWidget->setMyAntenna (aux1);
        }

        nameCol = rec.indexOf("my_sota_ref");
        aux1 = (query.value (nameCol)).toString();
        if (!aux1.isEmpty ())
        {
           myDataTabWidget->setMySOTA (aux1);
        }

        nameCol = rec.indexOf("tx_pwr");
        myDataTabWidget->setMyPower((query.value (nameCol)).toDouble());

        nameCol = rec.indexOf("rx_pwr");
        aux1 = (query.value (nameCol)).toString();
        testValueDouble = aux1.toDouble();
        if (testValueDouble >=0)
        {
            QSOTabWidget->setRXPwr(testValueDouble);
        }
        else
        {
            QSOTabWidget->setRXPwr(0.0);
        }

        nameCol = rec.indexOf("freq");
        aux1 = (query.value (nameCol)).toString();
        QSOTabWidget->setTXFreq (aux1.toDouble ());

        nameCol = rec.indexOf("freq_rx");
        aux1 = (query.value (nameCol)).toString();
        QSOTabWidget->setRXFreq (aux1.toDouble ());

        //QSL SENT

        nameCol = rec.indexOf("qsl_sent");
        aux1 = (query.value (nameCol)).toString();

        QSLTabWidget->setQSLSenStatus(aux1);
        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("qslsdate");
        aux1 = (query.value (nameCol)).toString();

        if (util->getDateFromSQliteString(aux1).isValid()  )

        //if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            QSLTabWidget->setQSLSenDate(util->getDateFromSQliteString(aux1));
        }

        nameCol = rec.indexOf("qsl_sent_via");
        aux1 = (query.value (nameCol)).toString();
        QSLTabWidget->setQSLSenVia(aux1);

        //QSL RECEPTION

        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
        // tr("B-Bureau") << tr("D-Direct") << tr("E-Electronic") << tr("M-Manager");
        //QSLRDATE: (only valid if QSL_RCVD is Y, I, or V)

        nameCol = rec.indexOf("qsl_rcvd");
        aux1 = (query.value (nameCol)).toString();
        QSLTabWidget->setQSLRecStatus(aux1);
        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("qslrdate");
        aux1 = (query.value (nameCol)).toString();
        if (util->getDateFromSQliteString(aux1).isValid()  )
        {
            QSLTabWidget->setQSLRecDate(util->getDateFromSQliteString(aux1));
        }

         nameCol = rec.indexOf("qsl_rcvd_via");
        aux1 = (query.value (nameCol)).toString();
        QSLTabWidget->setQSLRecVia(aux1);

        //TODO: BUG: When something is selected while modifying the QSL is deleted???

        //CLUBLOG
        nameCol = rec.indexOf("clublog_qso_upload_status");
        aux1 = (query.value (nameCol)).toString();
        eQSLTabWidget->setClubLogStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("clublog_qso_upload_date");
        aux1 = (query.value (nameCol)).toString();
        if (util->getDateFromSQliteString(aux1).isValid()  )
        {
            eQSLTabWidget->setClubLogDate(util->getDateFromSQliteString(aux1));
        }

        //CLUBLOG


        //EQSL_QSL_SENT: {Y, N, R, Q, I}
        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
       //EQSL_QSLSDATE (only valid if EQSL_SENT is Y, Q, or I)

        nameCol = rec.indexOf("eqsl_qsl_sent");
        aux1 = (query.value (nameCol)).toString();
        eQSLTabWidget->setEQSLSenStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("eqsl_qslsdate");
        aux1 = (query.value (nameCol)).toString();
        if (util->getDateFromSQliteString(aux1).isValid()  )
        {
            eQSLTabWidget->setEQSLSenDate(util->getDateFromSQliteString(aux1));
        }


        //E-QSL RECEPTION

        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
        // EQSL_QSL_RCVD: {Y, N, R, I, V}
        // EQSL_QSLRDATE: (only valid if EQSL_RCVD is Y, I, or V)

            nameCol = rec.indexOf("eqsl_qsl_rcvd");
            aux1 = (query.value (nameCol)).toString();
            eQSLTabWidget->setEQSLRecStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
            nameCol = rec.indexOf("eqsl_qslrdate");
            aux1 = (query.value (nameCol)).toString();
            if (util->isValidDateFromString(aux1))
            {
                eQSLTabWidget->setEQSLRecDate(util->getDateFromSQliteString(aux1));
            }

    //LOTW_QSL_SENT: {Y, N, R, Q, I}
    // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
   //LOTW_QSLSDATE (only valid if LOTW_SENT is Y, Q, or I)

            nameCol = rec.indexOf("lotw_qsl_sent");
            aux1 = (query.value (nameCol)).toString();
            eQSLTabWidget->setLOTWSenStatus(aux1.toUpper());
              //qDebug() << "MainWindow::qsoToEdit: - LoTW Sent Status: " << aux1  ;

    //TODO: Depending on the Value a date should or not exist.
    //      This code may be importing dates when they should not exist.
            nameCol = rec.indexOf("lotw_qslsdate");
            aux1 = (query.value (nameCol)).toString();

            if ( util->isValidDateFromString(aux1) )
            {
                eQSLTabWidget->setLOTWSenDate(util->getDateFromSQliteString(aux1));
            }

    //E-QSL RECEPTION

        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
        // lotw_QSL_RCVD: {Y, N, R, I, V}
        // lotw_QSLRDATE: (only valid if lotw_RCVD is Y, I, or V)

                nameCol = rec.indexOf("lotw_qsl_rcvd");
                aux1 = (query.value (nameCol)).toString();
                eQSLTabWidget->setLOTWRecStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
                nameCol = rec.indexOf("lotw_qslrdate");
                aux1 = (query.value (nameCol)).toString();
                if ( util->isValidDateFromString(aux1) )
                {
                    eQSLTabWidget->setLOTWRecDate(util->getDateFromSQliteString(aux1));
                }

        //QRZCOM
                nameCol = rec.indexOf("qrzcom_qso_upload_status");
                aux1 = (query.value (nameCol)).toString();
                eQSLTabWidget->setQRZCOMStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
                nameCol = rec.indexOf("qrzcom_qso_upload_date");
                aux1 = (query.value (nameCol)).toString();
                if (util->getDateFromSQliteString(aux1).isValid()  )
                {
                    eQSLTabWidget->setQRZCOMDate(util->getDateFromSQliteString(aux1));
                }

        //QRZCOM

        // OTHERS TAB

                nameCol = rec.indexOf("sota_ref");
                aux1 = (query.value (nameCol)).toString();
                if (!aux1.isEmpty ())
                {
                    othersTabWidget->setSOTA (aux1);
                }
                nameCol = rec.indexOf("age");
                aux1 = (query.value (nameCol)).toString();
                if (aux1.toDouble ()>0)
                {
                    othersTabWidget->setAge (aux1.toDouble ());
                }
                nameCol = rec.indexOf("distance");
                aux1 = (query.value (nameCol)).toString();
                if (aux1.toDouble ()>0)
                {
                    othersTabWidget->setDistance(aux1.toDouble ());
                }

                nameCol = rec.indexOf("vucc_grids");
                aux1 = (query.value (nameCol)).toString();
         //qDebug() << Q_FUNC_INFO << ": VUCC_GRIDS: " << aux1;
                if (util->isValidVUCCGrids (aux1))
                {
                    othersTabWidget->setVUCCGrids (aux1);
                }
      //qDebug() << "MainWindow::qsoToEdit: - just before IOTA"  ;

                nameCol = rec.indexOf("iota");
                aux1 = (query.value (nameCol)).toString();

                aux1 = awards->checkIfValidIOTA(aux1);
                othersTabWidget->setIOTA(aux1);

                    //qDebug() << "MainWindow::qsoToEdit: - in default - 100: " << QString::number(currentEntity)  ;

                nameCol = rec.indexOf("dxcc");
                aux1  = (query.value (nameCol)).toString();

                    //qDebug() << "MainWindow::qsoToEdit: Checking DXCC: " << aux1 << " - " << world->getEntityName(aux1.toInt()) ;

                if (aux1.toInt()>=1)
                {
                    if (aux1.toInt() != util->getNormalizedDXCCValue (currentEntity))
                    {
                        currentEntity = aux1.toInt();
                    }
                        //qDebug() << "MainWindow::qsoToEdit: - in default - 101: " << QString::number(currentEntity)  ;
                }
                else
                {
                    currentEntity = world->getQRZARRLId(currentQrz);
                        //qDebug() << "MainWindow::qsoToEdit: - in default - 103: " << QString::number(currentEntity)  ;
                }
                    //qDebug() << "MainWindow::qsoToEdit: - in default - 104: " << QString::number(currentEntity)  ;

                nameCol = rec.indexOf("prop_mode");
                aux1  = (query.value (nameCol)).toString();
                othersTabWidget->setPropMode(aux1, false);
        //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
                infoLabel2->setText(world->getEntityName(currentEntity));
                infoWidget->showEntityInfo(currentEntity);
        //selectCorrectComboBoxEntity(currentEntity);
                    //qDebug() << "MainWindow::qsoToEdit: " << QString::number(currentEntity) ;
                othersTabWidget->setEntity(currentEntity);
                     //qDebug() << "MainWindow::qsoToEdit: - in default - 101"  ;

                QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
                _qs.clear();
        //TODO: The band sometimes fails here. Check

                _qs << QString::number(currentEntity) << QString::number(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand())) << QString::number(dataProxy->getIdFromBandName(mainQSOEntryWidget->getMode()))  << QString::number(currentLog);


                     //qDebug() << "MainWindow::qsoToEdit: - in default - 104"  ;
                 //qDebug() << "MainWindow:: - calling showStatusOfDXCC-05 " ;
                showStatusOfDXCC(_qs);

    //qDebug() << "MainWindow::qsoToEdit: - in default - 106"  ;
    } //Closes the next.isValid
         //qDebug() << "MainWindow::qsoToEdit: - in default - END"  ;
    readingTheUI = false;
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::setModifying(const bool _m)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    modify = _m;
    QSOTabWidget->setModifying (_m);
    mainQSOEntryWidget->setModify(_m);
    satTabWidget->setModifying(_m);
    myDataTabWidget->setModify (_m);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotLocatorTextChanged(const QString &_loc)
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
    //qDebug() << "MainWindow::slotLocatorTextChanged: " << _loc;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if ( locator->isValidLocator(_loc) )
    {
        infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), _loc);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotMyLocatorTextChanged(const QString &_loc)
{
     //qDebug() << "MainWindowMy::slotMyLocatorTextChanged: " <<_loc ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if ( locator->isValidLocator(_loc))
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

void MainWindow::showStatusOfDXCC(const QStringList _qs)
{
     //qDebug() << "MainWindow::showStatusOfDXC: Entity: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) ;
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

    if ((_qs.length() != 4) || (_qs.at(1) == "-1")) // is the qs valid?
    {
        infoWidget->clear();
        slotShowInfoLabel("--");
        //infoLabel1->setText("--");
        logEvent(Q_FUNC_INFO, "END-1", Debug);
         //qDebug() << "MainWindow::showStatusOfDXCC -- END" ;
        return;
    }
    // Set the status bar with the appropriate message
    int status = awards->getDXStatus (_qs);
    QString message = QString();

     //qDebug() << "MainWindow::showStatusOfDXC: " << QString::number(status) ;

    message = awards->getDXStatusString(status);
    slotShowInfoLabel(message);
    //infoLabel1->setText(message);
    //infoWidget->showInfo((_qs.at(0)).toInt(), (_qs.at(1)).toInt(), (_qs.at(2)).toInt(), (_qs.at(3)).toInt() );
    infoWidget->showInfo((_qs.at(0)).toInt());
     //qDebug() << "MainWindow::showStatusOfDXC: -END" ;
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
    if ( awards->isDXMarathonNeed(_dxcc, _cqz, _year, _log))
    {
        slotShowInfoLabel(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
        //infoLabel1->setText(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotShowAwards()
{ //To be called from the logWindow & searchWidget
     //qDebug() << "MainWindow::slotShowAwards"  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    awards->recalculateAwards();
     //qDebug() << "MainWindow::slotShowAwards-1";
    //logWindow->refresh();
     //qDebug() << "MainWindow::slotShowAwards-2";

    awardsWidget->showAwards();
      //qDebug() << "MainWindow::slotShowAwards-3"  ;
    //dxccStatusWidget->refresh();
    setMainWindowTitle();
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::slotShowAwards-END"  ;
}

void MainWindow::fillQSOData()
{ // Updates all QSO with the dxcc, CQZ, ... if empty.
     //qDebug() << "MainWindow::fillQSOData" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    //QString stringQuery = QString("SELECT call, bandid, modeid, qso_date, lognumber, id, cqz, ituz, dxcc, cont FROM log WHERE ((dxcc<1) OR (cqz<1) OR (ituz<1) OR (dxcc IS NULL) OR (cqz IS NULL) OR (ituz IS NULL)) AND lognumber='%1'").arg(currentLog);
    QString stringQuery = QString("SELECT call, bandid, modeid, qso_date, lognumber, id, cqz, ituz, dxcc, cont FROM log WHERE ((dxcc<1) OR (cqz<1) OR (ituz<1) OR (dxcc IS NULL) OR (cqz IS NULL) OR (ituz IS NULL))");
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
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
    //nameCol = rec.indexOf("lognumber");
    //if ( (query.value (nameCol)).isValid() )
    //{
    //    _lognumber = (query.value (nameCol)).toString();
    //}
    //nameCol = rec.indexOf("confirmed");
    //if ( (query.value (nameCol)).isValid() )
    //{
    //    _confirmed = (query.value (nameCol)).toString();
    //}
            nameCol = rec.indexOf("id");
            if ( (query.value (nameCol)).isValid() )
            {
                _id = (query.value (nameCol)).toString();
            }
                 //qDebug() << "MainWindow::fillQSOData: ID: " << _id ;
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

     //qDebug() << "MainWindow::fillQSOData: DXCC" ;

            nameCol = rec.indexOf("dxcc");
            _dxcc = (query.value (nameCol)).toInt();
     //qDebug() << "MainWindow::fillQSOData: DXCC: " << QString::number(_dxcc) ;
            if (_dxcc < 1)
            {
                aux1 = QString::number(world->getQRZARRLId(_call) );
         //qDebug() << "MainWindow::fillQSOData: DXCC proposed: " << aux1 ;
                updateString = updateString + ", dxcc='" + aux1 + "'";
                toModify = true;
                _dxcc = aux1.toInt();
            }
            else
            {
         //qDebug() << "MainWindow::fillQSOData: DXCC already existed"  ;
            }

            nameCol = rec.indexOf("cont");
            if (( (query.value (nameCol)).toString()).length() < 2 )
            {
                aux1 = world->getContinentShortName(_dxcc);
        //aux1 = QString::number( world->getQRZItuz(_call) );
                updateString = updateString + ", ituz='" + aux1 + "'";
                toModify = true;
            }

            _dxcc = -1;
                 //qDebug() << "MainWindow::fillQSOData1: " << updateString ;
            if (toModify)
            {
                updateString = updateString + " WHERE id = " + "'" + _id + "'";
                     //qDebug() << "MainWindow::fillQSOData2: " << updateString ;
                sqlOK = query1.exec(updateString);
                if (sqlOK)
                {
                         //qDebug() << "MainWindow::fillQSOData: sqlOK=True" ;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().nativeErrorCode(), query1.lastQuery());
                         //qDebug() << "MainWindow::fillQSOData: sqlOK=False" ;
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
    dataProxy->fillEmptyDXCCInTheLog();
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
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
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
                aux = dataProxy->getNameFromSubModeId(aux.toInt());
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

void MainWindow::slotAnalyzeDxClusterSignal(QStringList ql)
{
        //qDebug() << "MainWindow::slotAnalyzeDxClusterSignal: 1: " << ql.at(0) <<"/1: " << ql.at(1) << "/2: " << ql.at(2) ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QStringList qls;
    int _entity = world->getQRZARRLId(ql.at(0));
    qls.clear();
    QString _mode = "-1";
    if (!manageMode)
    {
        _mode = "-1";
    }

    if (ql.length()==3)
    {
        if ((ql.at(2)) == "double")
        {
            clusterSpotToLog(ql.at(0), ql.at(1));
        }
        else if ((ql.at(2)) == "selected")
        {
    //slotShowInfoLabel(world->getEntityName(_entity), 2);
            infoLabel2->setText(world->getEntityName(_entity));
            infoWidget->showEntityInfo( _entity );

    // Becareful, he Frecuency arrives in KHz instead of bandid!!
    // db.getBandFromFreq expects a MHz!
    //(ql.at(1)).toDouble()

            qls << QString::number(_entity) << QString::number(dataProxy->getBandIdFromFreq((ql.at(1).toDouble()/1000))) << _mode <<  QString::number(currentLog);
    // We use a mode = -1 because we don't know the mode info from the DXCluster spot

    // TODO: Check if we can know the mode and replace the "-1" in previous sentence

    //qls << QRZ << BandId << ModeId << lognumber;
            showStatusOfDXCC(qls);
        }
    }
    //else
    //{ // Signal was not properly emited
    //}
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotDXClusterSpotArrived(const QString _dxCall, const QString _dxGrid, const double _freq)
{
     //qDebug() << Q_FUNC_INFO << ": " << _dxCall;
    (void)_dxCall;
    (void)_freq;
    if (!dxclusterSendSpotsToMap)
    {
        return;
    }
    logEvent(Q_FUNC_INFO, "Start", Debug);
    Coordinate coord = locator->getLocatorCoordinate (_dxGrid);
     //qDebug() << Q_FUNC_INFO << QString("  %1: Locator: %2 - (lat/lon)=>(%3/%4)").arg(_dxCall).arg(_dxGrid).arg(coord.lat).arg(coord.lon);
     //qDebug() << "Lat: " << QString::number(coord.lat) << " - Lon: " << QString::number(coord.lon);
    mapWindow->addMarker(coord, _dxGrid);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

//void MainWindow::clusterSpotToLog(const QStringList _qs)
void MainWindow::clusterSpotToLog(const QString &_call, const QString &_freq)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);

    QString _aux;
    double _freqN = (_freq.toDouble()) / 1000;
    mainQSOEntryWidget->setQRZ(_call);
    QSOTabWidget->setTXFreq (_freqN);

    //freqQLCDNumber->display(_freqN);

    _aux = QString::number(_freqN);

         //qDebug() << "MainWindow::clusterSpotToLog - Freq: " << _aux ;

    int _bandi = dataProxy->getBandIdFromFreq(_aux.toDouble());
         //qDebug() << "MainWindow::clusterSpotToLog - Bandi: " << QString::number(_bandi) ;
    _aux = QString::number(_bandi);
    _aux = QString("SELECT name FROM band WHERE id ='%1'").arg(_aux);

         //qDebug() << "MainWindow::clusterSpotToLog - Band: " << _aux ;

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
     //qDebug() << "MainWindow::clusterSpotToLog - END "  ;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }

    queryString = QString("UPDATE log SET qsl_sent='N' WHERE qsl_sent ='' AND lognumber='%1'").arg(currentLog);

    sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
    //qDebug() << "MainWindow::updateQSLRecAndSent - END"  ;
}



void MainWindow::defineStationCallsign(const QString &_call)
{
     //qDebug() << "MainWindow::defineStationCallsign (currentLog): " << QString::number(currentLog) ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (util->isValidCall (_call))
    {
        stationCallsign = _call;
    }
    else
    { // If no call is detected, qwe try to find it from the log
        QString logQRZ;
        logQRZ = dataProxy->getStationCallSignFromLog(currentLog);
         //qDebug() << "MainWindow::defineStationCallsign (logQrz): " << logQRZ ;

        if (util->isValidCall(logQRZ))
        {
     //qDebug() << "MainWindow::defineStationCallsign TRUE "  ;
            stationCallsign = logQRZ;
        }
    }

      //qDebug() << "MainWindow::defineStationCallsign: " << stationCallsign  ;

    filemanager->setStationCallSign(stationCallsign);
     //qDebug() << "MainWindow::defineStationCallsign: AFTER"  ;
    myDataTabWidget->setData(stationCallsign, operatorQRZ, myDataTabWidget->getMyLocator());
    dxccStatusWidget->setMyLocator(myDataTabWidget->getMyLocator());
    searchWidget->setStationCallsign(stationCallsign);
    if (lotwActive)
    {
        lotwUtilities->setStationCallSign(stationCallsign);
    }
    adifLoTWExportWidget->setDefaultStationCallsign(stationCallsign);

    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << "MainWindow::defineStationCallsign: " << stationCallsign << " - END" ;
}

void MainWindow::slotSetPropModeFromSat(const QString &_p, bool _keep)
{
         //qDebug() << "MainWindow::slotSetPropModeFromSat: " << _p ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
     //qDebug() << Q_FUNC_INFO << ": _keep" << util->boolToQString(_keep) ;

    othersTabWidget->setPropMode(_p, _keep);
    QSOTabWidget->setPropModeFromSat(_p);
    logEvent(Q_FUNC_INFO, "END", Debug);
    //int indexC = propModeComboBox->findText(" - " + _p + " - ", Qt::MatchContains);
    //propModeComboBox->setCurrentIndex(indexC);
}

void MainWindow::slotSetPropModeFromOther(const QString &_p)
{
    //Debug() << Q_FUNC_INFO << ": " << _p;
    if (_p!="SAT")
    {
         //qDebug() << Q_FUNC_INFO << ": Is NOT SAT propagation mode";
        satTabWidget->setNoSat();
    }
}

void MainWindow::completeWithPreviousQSO(const QString &_call)
{
       //qDebug() << "MainWindow::completeWithPreviousQSO" ;
    //This function completes: Name, QTH, Locator, Entity, Iota
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if ((!completeWithPrevious) || (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    //if ( (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    {
   //qDebug() << "MainWindow::completeWithPreviousQSO NOT completing..." ;
        if (completedWithPreviousName)
        {
            QSOTabWidget->clearName();
            completedWithPreviousName = false;
        }
        if (completedWithPreviousQTH)
        {
            QSOTabWidget->clearQTH();
            completedWithPreviousQTH = false;
         }
        if (completedWithPreviousLocator)
        {
            QSOTabWidget->clearDXLocator();
            completedWithPreviousLocator = false;
        }
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
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }
       //qDebug() << "MainWindow::completeWithPreviousQSO completing..." ;
    QString aux = QString();

    aux = dataProxy->getNameFromQRZ(_call);
       //qDebug() << "MainWindow::completeWithPreviousQSO aux: " << aux ;
       //qDebug() << "MainWindow::completeWithPreviousQSO nameLineEdit: " << QSOTabWidget->getName() ;

       //qDebug() << "MainWindow::completeWithPreviousQSO aux length: " << QString::number(aux.length()) ;
       //qDebug() << "MainWindow::completeWithPreviousQSO nameL length: " << QString::number((QSOTabWidget->getName()).length()) ;

    if ((aux.length()>=0) && (QSOTabWidget->getName().length()<=0) )
    {
   //qDebug() << "MainWindow::completeWithPreviousQSO name: 1" ;
        QSOTabWidget->setName(aux);
        QSOTabWidget->setPaletteRightName (true);
        completedWithPreviousName = true;
    }
    else if (completedWithPreviousName && (aux != QSOTabWidget->getName()))
    {
   //qDebug() << "MainWindow::completeWithPreviousQSO name: 2" ;
        completedWithPreviousName = false;
        QSOTabWidget->setPaletteRightName (false);
    }
    else
    {
   //qDebug() << "MainWindow::completeWithPreviousQSO name: 3" ;
    }

    aux = dataProxy->getQTHFromQRZ(_call);
    if ((aux.length()>=0) && (QSOTabWidget->getQTH().length()<=0) )
    {
        QSOTabWidget->setPaletteRightQTH (true);
        completedWithPreviousQTH = true;
        QSOTabWidget->setQTH(aux);
    }
    else if (completedWithPreviousQTH && (aux != QSOTabWidget->getQTH()))
    {
        completedWithPreviousQTH = false;
        QSOTabWidget->setPaletteRightQTH (false);
    }

    aux = dataProxy->getLocatorFromQRZ(_call);
    if ((aux.length()>=0) && ((QSOTabWidget->getDXLocator()).length()<=0) )
    {
        QSOTabWidget->setPaletteRightDXLocator (true);
        QSOTabWidget->setDXLocator(aux);
        completedWithPreviousLocator=true;
    }
    else if (completedWithPreviousLocator && (aux != QSOTabWidget->getDXLocator()))
    {
        completedWithPreviousLocator = false;
        QSOTabWidget->setPaletteRightDXLocator(false);
    }

    aux = dataProxy->getIOTAFromQRZ(_call);
    //othersTabWidget->setIOTA(aux);

    if ((aux.length()>=0) && (othersTabWidget->isIOTAModified()) )
    {
        aux = awards->checkIfValidIOTA(aux);

        if ((aux.length())==6)
        {
            othersTabWidget->setIOTA(aux);
    //TODO: Decide if it is better this way or like in : void MainWindowInputQSL::setQSLVia(const QString &_qs, QColor qColor)
    //QStringList values = aux.split("-", QT_SKIP);
    //iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(values.at(0) ) );
    //iotaNumberLineEdit->setPalette(palRed);
    //iotaNumberLineEdit->setText(values.at(1));
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

    aux = dataProxy->getQSLViaFromQRZ(_call);
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

    //mainQSOEntryWidget->setFreq (_fr, true);
    QSOTabWidget->setRXFreq (_fr);
    satTabWidget->setDownLinkFreq(_fr);

    logEvent(Q_FUNC_INFO, "END", Debug);
    //qDebug() << "MainWindow::slotFreqRXChanged - END"  ;
}

void MainWindow::slotFreqTXChangedFromSat(const double _fr)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!upAndRunning)
    {
         //qDebug() << "MainWindow::slotFreqTXChanged !upAndRunning" ;
        return;
    }
    if (dataProxy->isThisFreqInBand (mainQSOEntryWidget->getBand (), QString::number(_fr)))
    {
        return;
    }
    slotFreqTXChanged (_fr);

    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotFreqTXChanged(const double _fr)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (!upAndRunning)
    {
         //qDebug() << "MainWindow::slotFreqTXChanged !upAndRunning" ;
        return;
    }
    QSOTabWidget->setTXFreq (_fr);
    mainQSOEntryWidget->setFreq (_fr);
    satTabWidget->setUpLinkFreq(_fr);
    if (hamlibActive && !manualMode)
    {
        hamlib->setFreq(_fr);
    }
     //qDebug() << Q_FUNC_INFO << " - Freq: " << QString::number (_fr);
    if (_fr<10.0)
    {
         //qDebug() << Q_FUNC_INFO << " - Lower and mode: " << mainQSOEntryWidget->getMode ();
        if (mainQSOEntryWidget->getMode() == "USB")
        {
     //qDebug() << Q_FUNC_INFO << " - We need to change";
            mainQSOEntryWidget->setMode ("LSB");
        }
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << " - Higher and mode: " << mainQSOEntryWidget->getMode ();
        if (mainQSOEntryWidget->getMode() == "LSB")
        {
     //qDebug() << Q_FUNC_INFO << " - We need to change";
            mainQSOEntryWidget->setMode ("USB");
        }
    }


    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << "MainWindow::slotFreqTXChanged - END"  ;
}

void MainWindow::slotShowQSOsFromDXCCWidget(QList<int> _qsos)
{
     //qDebug() << "MainWindow::slotShowQSOsFromDXCCWidget" ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    slotShowSearchWidget();
    searchWidget->showQSOs(_qsos);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void MainWindow::slotWSJTXloggedQSO (const QString &_dxcall, const QString &_mode, const QString &_band, const double _freq,
                 const QString &_mygrid, const QString &_dxgrid, const QString &_rstTX, const QString &_rstRX,
                 const QString &_comment, const QString &_stationcallsign, const QString &_name,
                 const QString &_opCall, const QDateTime &_datetime, const QDateTime &_datetime_off,
                 const QString &_exchangeTX, const QString &_exchangeRX, const QString &_mypwr)
{
     //qDebug() << "MainWindow::slotWSJTX-loggedQSO" ;
    //logEvent(Q_FUNC_INFO, "Start", Debug);
    bool logTheQso = false;
    QString opCall = stationCallsign;
    if (util->isValidCall(_opCall))
    {
        opCall = _opCall.toUpper();
    }
    double pwr = _mypwr.toDouble();
    if (pwr<=0.0)
    {
        pwr = myDataTabWidget->getMyPower ();
    }

    if (!_datetime.isValid() || !_datetime_off.isValid())
    {
   //qDebug() << Q_FUNC_INFO << " DATES NOT VALID " ;
        return ;
    }

     //qDebug() << Q_FUNC_INFO << " dxcall: " << _dxcall ;
     //qDebug() << Q_FUNC_INFO << " freq: " << QString::number(_freq/1000000) ;
     //qDebug() << Q_FUNC_INFO << " freq no div: " << QString::number(_freq) ;
     //qDebug() << Q_FUNC_INFO << " mode: " << _mode ;
     //qDebug() << Q_FUNC_INFO << " band: " << _band ;
     //qDebug() << Q_FUNC_INFO << " my_grid: " << _mygrid ;
     //qDebug() << Q_FUNC_INFO << " dx_grid: " << _dxgrid ;
     //qDebug() << Q_FUNC_INFO << " comment: " << _comment ;
     //qDebug() << Q_FUNC_INFO << " StationCall: " << _stationcallsign ;
     //qDebug() << Q_FUNC_INFO << " _opCall: " << _opCall ;
     //qDebug() << Q_FUNC_INFO << " opCall: " << opCall ;
     //qDebug() << Q_FUNC_INFO << " time_on: " << util->getDateTimeSQLiteStringFromDateTime(_datetime) ;
     //qDebug() << Q_FUNC_INFO << " time_off: " << util->getDateTimeSQLiteStringFromDateTime(_datetime_off) ;
     //qDebug() << Q_FUNC_INFO << " report_sent: " << _rstTX ;
     //qDebug() << Q_FUNC_INFO << " report_rec: " << _rstRX ;
     //qDebug() << Q_FUNC_INFO << " exchange_sent: " << _exchangeTX ;
     //qDebug() << Q_FUNC_INFO << " exchange_rec: " << _exchangeRX ;
     //qDebug() << Q_FUNC_INFO << " MY_PWR: " << _mypwr ;


    if (wsjtxAutoLog)
    { // Log automatically, without confirmation
        logTheQso = true;
   //qDebug() << Q_FUNC_INFO << " LogTheQSO = true"  ;
    }
    else
    { // Ask for confirmation before logging
       //qDebug() << Q_FUNC_INFO << " LogTheQSO = false - we ask for confirmation"  ;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog - QSO received"));
            msgBox.setTextFormat(Qt::RichText);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
            msgBox.setDefaultButton(QMessageBox::Yes);
            QString aux;
            aux = tr("The following QSO data has been received from WSJT-X to be logged:") + "\n\n" +
                    "<UL>" +
                    "<LI>" +
                    "<b>" + tr("Callsign") + ": " + "</b>" + _dxcall.toUpper() +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Freq") + ": " + "</b>" + QString::number(_freq) +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Mode") + ": " + "</b>" + _mode.toUpper() +
                    "</LI>" +
                    "<LI>" +
            //"<b>" + tr("Time On") + ": " + "</b>" + (QDateTime::fromString(_time_on, "yyyyMMddhhmmss")).toString("yyyy/MM/dd - hh:mm:ss") +
                    "<b>" + tr("Time On") + ": " + "</b>" + util->getDateTimeSQLiteStringFromDateTime(_datetime) +
                    "</LI>" +
                    "<LI>" +
            //"<b>" + tr("Time Off") + ": " + "</b>" + (QDateTime::fromString(_time_off, "yyyyMMddhhmmss")).toString("yyyy/MM/dd - hh:mm:ss") +
                    "<b>" + tr("Time Off") + ": " + "</b>" + util->getDateTimeSQLiteStringFromDateTime(_datetime_off) +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("RST TX") + ": " + "</b>" + _rstTX + " - <b>" + tr("RST RX") + ": " + "</b>" + _rstRX  +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Comment") + ": " + "</b>" + _comment  +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("DX-Grid") + ": " + "</b>" + _dxgrid.toUpper()  +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Local-Grid") + ": " + "</b>" + _mygrid.toUpper() +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Station Callsign") + ": " + "</b>" + _stationcallsign.toUpper() +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Operator Callsign") + ": " + "</b>" + opCall.toUpper() +
                    "</LI>" +
                    "</UL>" ;

            msgBox.setText(aux);
            int ret = msgBox.exec();
            switch (ret)
            {
                case QMessageBox::Yes:
                    logTheQso = true;
                break;
                case QMessageBox::No:
                    logTheQso = false;
                    break;
                default:
        // should never be reached
                logTheQso = false;
                break;
            }
        }
        //bool saveThisQSO = true;
        if (logTheQso)
        {
       //qDebug() << Q_FUNC_INFO << " QSO must be logged" ;
            bool qsoLogged = false;
            int dxcc = world->getQRZARRLId(_dxcall);
            dxcc = util->getNormalizedDXCCValue (dxcc);

            QString _myLoc = _mygrid;
            if (!(locator->isValidLocator(_myLoc)))
            {
                _myLoc = myDataTabWidget->getMyLocator();
            }

            if ((dataProxy->isThisQSODuplicated(Q_FUNC_INFO, _dxcall, _datetime,  dataProxy->getBandIdFromFreq(_freq),  dataProxy->getIdFromModeName(_mode), dupeSlotInSeconds)).length()>1)
            {
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

                    break;
                    case QMessageBox::Discard:
                        logEvent(Q_FUNC_INFO, "END-1", Debug);
                        return; //No more error shown
              //  break;
                    default:
            // should never be reached
                        logEvent(Q_FUNC_INFO, "END-3", Debug);
                    return;   // The user wants to keepseeing errors
            //break;
                }
            }


            qsoLogged = dataProxy->addQSOFromWSJTX(_dxcall, _mode, _band,  _freq,
                                                _myLoc, _dxgrid, _rstTX, _rstRX,
                                                _exchangeRX, _exchangeTX, _comment,
                                                _stationcallsign, _name, opCall,
                                                _datetime, _datetime_off, pwr, dxcc, currentLog, sendQSLByDefault);


            if (qsoLogged)
            {
         //qDebug() << Q_FUNC_INFO << " Logged QSO OK: " << _dxcall ;
                actionsJustAfterAddingOneQSO();
                infoLabel1T = infoLabel1->text();
                infoLabel2T = infoLabel2->text();
                slotShowInfoLabel(tr("QSO logged from WSJT-X:"));
        //slotShowInfoLabel(_dxcall + " - " + dataProxy->getBandNameFromFreq(_freq) + "/" + _mode, 2);
        //infoLabel1->setText(tr("QSO logged from WSJT-X:"));
                infoLabel2->setText(_dxcall + " - " + dataProxy->getBandNameFromFreq(_freq) + "/" + _mode);
        //timerInfoBars->start(infoTimeout);

        //actionsJustAfterAddingOneQSO();
         //qDebug() << Q_FUNC_INFO;
                slotClearButtonClicked(Q_FUNC_INFO);
        //UDPLogServer->start();

                if (clublogActive && clublogRealTime)
                {
                    elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(dataProxy->getLastQSOid()));
                }
            }
            else
            {
           //qDebug() << Q_FUNC_INFO << " Logged QSO NOK: " << _dxcall ;
            }
        }
        else
        {
       //qDebug() << Q_FUNC_INFO << " QSO must NOT be logged ... ending" ;
        }


    logEvent(Q_FUNC_INFO, "END", Debug);
       //qDebug() << Q_FUNC_INFO << " - END" ;
}



bool MainWindow::checkIfNewMode(const QString &_mode)
{
       //qDebug() << "MainWindow::checkIfNewMode: " << _mode ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (dataProxy->getSubModeIdFromSubMode(_mode)<0)
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
             mainQSOEntryWidget->setQRZ(_dxcall);
             if ((!noMoreModeErrorShown) && (dataProxy->getSubModeIdFromSubMode(_mode)>0) )
             {
                 mainQSOEntryWidget->setMode(_mode);
        //modeComboBox->setCurrentIndex(modeComboBox->findText(_mode, Qt::MatchCaseSensitive));
             }
      //qDebug() << "MainWindow::slotWSJXstatusFromUDPServer updating txFreqSpinBox" << QString::number(_freq) ;
             QSOTabWidget->setTXFreq (_freq);
             QSOTabWidget->setRXFreq (_freq);
             if (_dx_grid.length()>0)
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
     //qDebug() << "MainWindow::slotQueryErrorManagement: Function: " << functionFailed ;
     //qDebug() << "MainWindow::slotQueryErrorManagement: Error: " << functionFailed << errorCodeS;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (noMoreErrorShown)
    {
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return;
    }

    if ((functionFailed == "virtual bool DataProxy_SQLite::addSatellite(QString, QString, QString, QString, QString)") && (nativeError.toInt() == 19))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - Duplicated satellite"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("A duplicated satellite has been detected in the file and will not be imported."));
        msgBox.setInformativeText(tr("Please check the satellite information file and ensure it is properly populated.") + "\n" + tr("Now you will see a more detailed error that can be used for debugging..."));
        msgBox.exec();
    }
    // TODO: An error on DB has been detected.
    // KLog should suggest to export ALL the data to an ADIF file to prevent any log lose

    QString aux = "<br><b>" + tr("An unexpected error ocurred!!") + "</b><br><br>" + tr("If the problem persists, please contact the developers") + "(<a href=mailto:klog@groups.io>klog@groups.io</a>)" + tr("for analysis:") + "<br>";
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
    logEvent(Q_FUNC_INFO, "Start", Debug);
    qso->clear ();
    logEvent(Q_FUNC_INFO, "- 001", Devel);
    qso->setBackup (true);
    logEvent(Q_FUNC_INFO, "- 002", Devel);
    qso->setModifying (mainQSOEntryWidget->getModifying());
    logEvent(Q_FUNC_INFO, "- 003", Devel);
    qso->setRealTime (mainQSOEntryWidget->getRealTime());
    logEvent(Q_FUNC_INFO, "- 004", Devel);
    qso->setManualMode (mainQSOEntryWidget->getManualMode());
    logEvent(Q_FUNC_INFO, "- 005", Devel);
    qso->setLogId (currentLog);
    logEvent(Q_FUNC_INFO, "- 010", Devel);
    // MainQSOEntryWidget
    qso->setCall (mainQSOEntryWidget->getQrz ());

    logEvent(Q_FUNC_INFO, "- 011", Devel);
    qso->setBand (mainQSOEntryWidget->getBand ());
    logEvent(Q_FUNC_INFO, "- 012", Devel);
    qso->setMode (mainQSOEntryWidget->getMode ());
      //qDebug() << Q_FUNC_INFO << " - 013";
    qso->setDateTimeOn (mainQSOEntryWidget->getDateTime ());
    logEvent(Q_FUNC_INFO, "- 020", Devel);

    //  MainWindowInputQSO
    qso->setRSTTX (QSOTabWidget->getRSTTX ());
    qso->setRSTRX (QSOTabWidget->getRSTRX ());
    qso->setFreqTX (QSOTabWidget->getTXFreq ());
    qso->setFreqRX (QSOTabWidget->getRXFreq ());
    qso->setGridSquare (QSOTabWidget->getDXLocator ());
    qso->setName (QSOTabWidget->getName ());
    qso->setQTH(QSOTabWidget->getQTH ());
    qso->setRXPwr(QSOTabWidget->getRXPwr ());
     logEvent(Q_FUNC_INFO, "- 030", Devel);
    // MainWindowInputQSL
    qso->setQSL_SENT (QSLTabWidget->getQSLSenStatus ());
    qso->setQSL_RCVD (QSLTabWidget->getQSLRecStatus ());
    qso->setQSLRDate (QSLTabWidget->getQSLRecDate ());
    qso->setQSLSDate (QSLTabWidget->getQSLSenDate ());
    qso->setQSLSenVia (QSLTabWidget->getSentVia ());
    qso->setQSLRecVia (QSLTabWidget->getRecVia ());
    qso->setQSLVia (QSLTabWidget->getQSLVia ());
    qso->setQSLMsg (QSLTabWidget->getQSLMsg ());
     logEvent(Q_FUNC_INFO, "- 040", Devel);
    // MainWindowInputEQSL
    qso->setClubLogStatus (eQSLTabWidget->getClubLogStatus ());
    qso->setClubLogDate (eQSLTabWidget->getClubLogDate ());
    qso->setEQSLQSL_SENT (eQSLTabWidget->getEQSLSenStatus ());
    qso->setEQSLQSLSDate (eQSLTabWidget->getEQSLSenDate ());
    qso->setEQSLQSL_RCVD (eQSLTabWidget->getEQSLRecStatus ());
    qso->setEQSLQSLRDate (eQSLTabWidget->getEQSLRecDate ());
    qso->setLoTWQSL_SENT (eQSLTabWidget->getLOTWSenStatus ());
    qso->setLoTWQSLSDate (eQSLTabWidget->getLOTWSenDate ());
    qso->setLoTWQSL_RCVD (eQSLTabWidget->getLOTWRecStatus ());
    qso->setLoTWQSLRDate (eQSLTabWidget->getLOTWRecDate ());
    qso->setClubLogStatus (eQSLTabWidget->getClubLogStatus ());
    qso->setClubLogDate (eQSLTabWidget->getClubLogDate ());
     logEvent(Q_FUNC_INFO, "- 050", Devel);
    // MainWindowInputComment
    qso->setComment (commentTabWidget->getComment ());
    logEvent(Q_FUNC_INFO, "- 054", Devel);
    qso->setKeepComment (commentTabWidget->getKeep ());
    logEvent(Q_FUNC_INFO, "- 055", Devel);
    // MainWindowInputOthers
    qso->setDXCC (othersTabWidget->getEntity ());
    logEvent(Q_FUNC_INFO, "- 056", Devel);
    qso->setIOTA (othersTabWidget->getIOTA ());
    logEvent(Q_FUNC_INFO, "- 057", Devel);
    qso->setPropMode (othersTabWidget->getPropModeFromComboBox());
    logEvent(Q_FUNC_INFO, "- 058", Devel);
    qso->setKeepOthers (othersTabWidget->getKeep ());
    logEvent(Q_FUNC_INFO, "- 059", Devel);
    qso->setVUCCGrids (othersTabWidget->getVUCCGrids ());
     logEvent(Q_FUNC_INFO, "- 060", Devel);
    // MainWindowMyDataTab
    qso->setTXPwr (myDataTabWidget->getMyPower());
    logEvent(Q_FUNC_INFO, "- 061", Devel);
    qso->setOperatorCallsign (myDataTabWidget->getOperator ());
    logEvent(Q_FUNC_INFO, "- 062", Devel);
    qso->setStationCallsign (myDataTabWidget->getStationCallsign ());

    logEvent(Q_FUNC_INFO, "- 063", Devel);
    qso->setMySOTA_REF (myDataTabWidget->getMySOTA ());
    logEvent(Q_FUNC_INFO, "- 064", Devel);
    qso->setMyRig (myDataTabWidget->getMyRig());
    logEvent(Q_FUNC_INFO, "- 065", Devel);
    qso->setMyAntenna (myDataTabWidget->getMyAntenna ());
    logEvent(Q_FUNC_INFO, "- 066", Devel);
    qso->setMyVUCCGrids (myDataTabWidget->getMyVUCCGrids ());
    logEvent(Q_FUNC_INFO, "- 067", Devel);
    qso->setMyGridSquare (myDataTabWidget->getMyLocator ());
    logEvent(Q_FUNC_INFO, "- 068", Devel);
    qso->setKeepMyData (myDataTabWidget->getKeep ());
     logEvent(Q_FUNC_INFO, "- 070", Devel);
    //MainWindowSatTab
    qso->setSatName (satTabWidget->getSatName ());
    logEvent(Q_FUNC_INFO, "- 071", Devel);
    qso->setSatMode (satTabWidget->getSatMode ());
    qso->setKeepSatTab (satTabWidget->getKeep ());
    logEvent(Q_FUNC_INFO, "END", Debug);
      //qDebug() << Q_FUNC_INFO << ": Realtime: " << util->boolToQString (qso->getRealTime ());
}

void MainWindow::restoreCurrentQSO(const bool restoreConfig)
{ // This function restores a QSO that was backed up to the UI.
    // MainQSOEntryWidget
     //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (restoreConfig);
    clearUIDX();
    if (qso->getModifying())
    {
        mainQSOEntryWidget->setModify(true);
    }
    mainQSOEntryWidget->setQRZ (qso->getCall ());
    mainQSOEntryWidget->setBand (qso->getBand ());
    mainQSOEntryWidget->setMode (qso->getMode ());
    mainQSOEntryWidget->setDateTime (qso->getDateTimeOn ());

     //qDebug() << Q_FUNC_INFO << ": restoring config: " << util->boolToQString (restoreConfig);
    if (restoreConfig)
    {
        mainQSOEntryWidget->setRealTime (qso->getRealTime());
        manualMode = qso->getManualMode();
         //qDebug() << Q_FUNC_INFO << ": manualMode: " << util->boolToQString (manualMode);
        mainQSOEntryWidget->setManualMode (manualMode);
    }

    //  MainWindowInputQSO
    QSOTabWidget->setRSTRX (qso->getRSTRX ());
    QSOTabWidget->setRSTTX (qso->getRSTTX ());
    QSOTabWidget->setTXFreq (qso->getFreqTX ());
    QSOTabWidget->setRXFreq (qso->getFreqRX ());
    QSOTabWidget->setDXLocator (qso->getGridSquare ());
    QSOTabWidget->setName (qso->getName ());
    QSOTabWidget->setQTH (qso->getQTH ());
    QSOTabWidget->setRXPwr (qso->getRXPwr ());
    // MainWindowInputQSL
    QSLTabWidget->setQSLSenStatus (qso->getQSL_SENT ());
    QSLTabWidget->setQSLRecStatus (qso->getQSL_RCVD ());
    QSLTabWidget->setQSLRecDate (qso->getQSLRDate ());
    QSLTabWidget->setQSLSenDate (qso->getQSLSDate ());

    QSLTabWidget->setQSLSenVia (qso->getQSLSentVia());
    QSLTabWidget->setQSLRecVia (qso->getQSLRecVia());
    QSLTabWidget->setQSLVia (qso->getQSLVia ());
    QSLTabWidget->setQSLMsg (qso->getQSLMsg ());

    // MainWindowInputEQSL
    eQSLTabWidget->setClubLogStatus (qso->getClubLogStatus ());
    eQSLTabWidget->setClubLogDate (qso->getClubLogDate ());
    eQSLTabWidget->setEQSLSenStatus (qso->getEQSLQSL_SENT ());
    eQSLTabWidget->setEQSLSenDate (qso->getEQSLQSLSDate ());
    eQSLTabWidget->setEQSLRecStatus (qso->getEQSLQSL_RCVD ());
    eQSLTabWidget->setEQSLRecDate (qso->getEQSLQSLRDate ());
    eQSLTabWidget->setLOTWSenStatus (qso->getLoTWQSL_SENT ());
    eQSLTabWidget->setLOTWSenDate (qso->getLoTWQSLSDate ());
    eQSLTabWidget->setLOTWRecStatus (qso->getLoTWQSL_RCVD ());
    eQSLTabWidget->setLOTWRecDate (qso->getLoTWQSLRDate ());
    eQSLTabWidget->setQRZCOMStatus (qso->getQRZCOMStatus ());
    eQSLTabWidget->setQRZCOMDate (qso->getQRZCOMDate ());

    // MainWindowInputComment
    commentTabWidget->setData (qso->getComment ());
    commentTabWidget->setKeep (qso->getKeepComment ());

    // MainWindowInputOthers

    othersTabWidget->setVUCCGrids (qso->getVUCCGrids ());
    othersTabWidget->setSOTA (qso->getSOTA_REF ());
    othersTabWidget->setAge(qso->getAge ());
    othersTabWidget->setDistance(qso->getDistance());

    othersTabWidget->setEntity (qso->getDXCC ());
    othersTabWidget->setIOTA (qso->getIOTA ());
    othersTabWidget->setPropMode (qso->getPropMode ());
    othersTabWidget->setKeep (qso->getKeepOthers ());

    // MainWindowMyDataTab
    if (qso->getLogId () == currentLog)
    {
        myDataTabWidget->setStationCallsign (qso->getStationCallsign());
    }
    else
    {
        myDataTabWidget->setStationCallsign (stationCallsign);
    }
    myDataTabWidget->setMyPower (qso->getTXPwr ());
    myDataTabWidget->setOperator (qso->getOperatorCallsign());

    myDataTabWidget->setMyLocator (qso->getMyGridSquare ());
    myDataTabWidget->setKeep (qso->getKeepMyData ());
    myDataTabWidget->setMyRig (qso->getMyRig ());
    myDataTabWidget->setMyAntenna (qso->getMyAntenna ());
    myDataTabWidget->setMySOTA (qso->getMySOTA_REF ());
    myDataTabWidget->setMyVUCCGrids (qso->getMyVUCCGrids ());
        //MainWindowSatTab
    satTabWidget->setSatName (qso->getSatName ());
    satTabWidget->setSatMode (qso->getSatMode ());
    satTabWidget->setKeep (qso->getKeepSatTab ());
    qso->setBackup (false);
     //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::setLogLevel(const DebugLogLevel _sev)
{
    //qDebug() << Q_FUNC_INFO << ": " << util->debugLevelToString(_sev);
    logEvent(Q_FUNC_INFO, "Start", Debug);
    logLevel = _sev;
    showKLogLogWidget->setLogLevel(logLevel);
    //setupDialog->setLogLevel(logLevel);

    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("Misc");
    settings.setValue ("DebugLog",util->debugLevelToString (logLevel));
    settings.endGroup ();

    tipsDialog->setLogLevel(logLevel);
    dataProxy->setLogLevel(logLevel);
    mainQSOEntryWidget->setLogLevel(logLevel);
    util->setLogLevel(logLevel);
    qso->setLogLevel(logLevel);
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
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("Misc");
    settings.setValue ("DebugLog", util->debugLevelToString(l));
    settings.endGroup ();
}

bool MainWindow::loadSettings()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getSetFile (), QSettings::IniFormat);

     //qDebug() << Q_FUNC_INFO << " - 10 - General";
    QString value = settings.value ("Version").toString ();
    if (softwareVersion!=value)
    {
        //qDebug() << Q_FUNC_INFO << " - It seems it is a new version ";
        //qDebug() << Q_FUNC_INFO << QString("softwareversion: %1 / version: %2").arg(softwareVersion).arg(value);
        itIsANewversion = true;
    }
    selectTheLog(currentLog = settings.value ("SelectedLog").toInt());
    setWindowSize (settings.value ("MainWindowSize").toSize ());

     //qDebug() << Q_FUNC_INFO << " - 20 - user";
    settings.beginGroup ("UserData");
    value = settings.value ("CallSign").toString ();
    if (util->isValidCall(value))
    {
        mainQRZ = value;
    }
    value = settings.value ("StationLocator").toString ();
    if ( locator->isValidLocator(value) )
    {
        myDataTabWidget->setMyLocator(value.toUpper());
        adifLoTWExportWidget->setDefaultMyGrid(value.toUpper());
    }
    settings.endGroup ();

    myDataTabWidget->loadSettings ();

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

     //qDebug() << Q_FUNC_INFO << " - 40 - logview";
    logWindow->setColumns(settings.value ("LogViewFields").toStringList ());
     //qDebug() << Q_FUNC_INFO << " - 41 - logs";


     //qDebug() << Q_FUNC_INFO << " - 50 - dxcluster";
    settings.beginGroup ("DXCluster");
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
    newOneColor.setNamedColor(settings.value ("NewOneColor", "#FF0000").toString ());
    neededColor.setNamedColor(settings.value ("NeededColor","#FF8C00").toString ());
    workedColor.setNamedColor(settings.value ("WorkedColor", "#FFD700").toString ());
    confirmedColor.setNamedColor(settings.value ("ConfirmedColor", "#32CD32").toString ());
    defaultColor.setNamedColor(settings.value ("DefaultColor", "#00BFFF").toString ());
    settings.endGroup ();
    setupDialog->loadDarkMode ();

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
    sendQSLByDefault = settings.value ("SendEQSLByDefault", true).toBool ();
    eQSLTabWidget->setQueueSentByDefault(sendQSLByDefault);
    dupeSlotInSeconds = settings.value ("DuplicatedQSOSlot", 300).toInt ();
    filemanager->setDuplicatedQSOSlot(dupeSlotInSeconds);
    mainQSOEntryWidget->setDuplicatedQSOSlot(dupeSlotInSeconds);
    completeWithPrevious = settings.value ("CompleteWithPrevious", true).toBool ();
    defaultADIFLogFile = settings.value ("DefaultADIFFile").toString ();
    deleteAlwaysAdiFile = settings.value ("DeleteAlwaysAdiFile", true).toBool ();
    util->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
    mainQSOEntryWidget->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
    filemanager->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
    adifLoTWExportWidget->setCallValidation(settings.value ("CheckValidCalls", true).toBool ());
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
            QSettings settings(util->getSetFile (), QSettings::IniFormat);
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
    stationCallsign = dataProxy->getStationCallSignFromLog (currentLog);
    defineStationCallsign (stationCallsign);
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
