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
//using namespace G_callsignCheck;
//#include <qDebug>

//extern bool g_callsignCheck;

MainWindow::MainWindow(const QString &_klogDir, const QString &tversion)
{
    //qDebug() << Q_FUNC_INFO << ": " <<  _klogDir << " Ver: " << tversion << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    g_callsignCheck  = true;

    showKLogLogWidget = new ShowKLogLogWidget;
    showErrorDialog = new ShowErrorDialog();
    UDPLogServer = new UDPServer();
    util = new Utilities;
    qso = new QSO;

    softwareVersion = tversion;
    klogDir = _klogDir;
    logSeverity = Info;
    sendQSLByDefault = true; // This must be before reading the config
    dupeSlotInSeconds = 0;
    needToEnd = false;
    upAndRunning = false; // To define some actions that can only be run when starting the software


    QRZCOMAutoCheckAct = new QAction(tr("Check always the current callsign in QRZ.com"), this);
    QRZCOMAutoCheckAct->setCheckable(true);
    QRZCOMAutoCheckAct->setChecked(false);

    //qDebug() << "MainWindow::MainWindow: Debug File: "<<  util->getDebugLogFile() << QT_ENDL;
    debugFile = new QFile(util->getDebugLogFile());

    //qDebug() << Q_FUNC_INFO << ": BEFORE HAMLIB " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    hamlib = new HamLibClass();
    //qDebug() << Q_FUNC_INFO << ": AFTER HAMLIB " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, softwareVersion);
    lotwUtilities = new LoTWUtilities(klogDir, softwareVersion, Q_FUNC_INFO, dataProxy);
    eqslUtilities = new eQSLUtilities(Q_FUNC_INFO);

    //qDebug() << Q_FUNC_INFO << ": Before DXCCStatusWidget " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    dxccStatusWidget = new DXCCStatusWidget(dataProxy, Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << ": After DXCCStatusWidget " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    elogClublog = new eLogClubLog();
    //qDebug() << Q_FUNC_INFO << ": 00082: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    elogQRZcom = new eLogQrzLog(dataProxy, Q_FUNC_INFO, softwareVersion);

    //qDebug() << Q_FUNC_INFO << ": 00083: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    updateSatsData = new UpdateSatsData(dataProxy);
    //qDebug() << Q_FUNC_INFO << ": 00084: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    statsWidget = new StatisticsWidget(dataProxy);
    //qDebug() << Q_FUNC_INFO << ": 00085: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    infoLabel1 = new QLabel(tr("Status bar ..."));
    infoLabel2 = new QLabel(tr("DX Entity"));

     //qDebug() << "MainWindow::MainWindow: 00086" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    logWindow = new LogWindow(dataProxy, this);
    //qDebug() << Q_FUNC_INFO << ": 00087: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    searchWidget = new SearchWidget(dataProxy, this);
    //qDebug() << Q_FUNC_INFO << ": 00087.1: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    //advancedSearchWidget = new AdvancedSearchWidget(dataProxy, this);
    //qDebug() << "MainWindow::MainWindow: 00087.2" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    infoWidget = new InfoWidget(dataProxy, this);

    //qDebug() << Q_FUNC_INFO << ": 00088: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Creating AwardsWidget", Debug);
    awardsWidget = new AwardsWidget(dataProxy, this);

     //qDebug() << Q_FUNC_INFO << ": 0009: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    aboutDialog = new AboutDialog(softwareVersion);
    tipsDialog = new TipsDialog();

    downloadcty = new DownLoadCTY(klogDir, softwareVersion);

    statusBarMessage = tr("Starting KLog");

    if (!QDir::setCurrent ( klogDir )){
        QDir d1(klogDir);
        if (d1.mkdir(klogDir))
        {
            QDir::setCurrent ( klogDir );
        }
    }

    //qDebug() << Q_FUNC_INFO << ": 40: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    world = new World(dataProxy, klogDir, softwareVersion, Q_FUNC_INFO);

    //qDebug() << Q_FUNC_INFO << ": 50: " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    configFileName = util->getCfgFile();
    setupDialog = new SetupDialog(dataProxy, configFileName, softwareVersion, 0, !configured, this);
    //qDebug() << Q_FUNC_INFO << ": satTabWidget to be created " << QT_ENDL;
    satTabWidget = new MainWindowSatTab(dataProxy);

    QSOTabWidget = new MainWindowInputQSO(dataProxy);
    myDataTabWidget = new MainWindowMyDataTab();
    commentTabWidget = new MainWindowInputComment();
    othersTabWidget = new MainWindowInputOthers(dataProxy);
    eQSLTabWidget = new MainWindowInputEQSL(dataProxy);
    QSLTabWidget = new MainWindowInputQSL(dataProxy);
    mainQSOEntryWidget = new MainQSOEntryWidget(dataProxy);

   //qDebug() << Q_FUNC_INFO << ": locator to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    locator = new Locator();

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
   //qDebug() << Q_FUNC_INFO << ": 8 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    dateTime = new QDateTime();
    dateTimeTemp = new QDateTime();
    // UI DX
    infoLabel2 = new QLabel(tr("DX Entity"));
    loggWinAct = new QAction(tr("&Log Window"), this);

    //qDebug() << Q_FUNC_INFO << ": dxclusterwidget to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    dxClusterWidget = new DXClusterWidget(dataProxy, dxclusterServerToConnect , dxclusterServerPort, this);
    //qDebug() << Q_FUNC_INFO << ": Awards to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    awards = new Awards(dataProxy, Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << ": Awards created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    // </UI>


    if (needToEnd)
    {
       exit(0);
    }

    //qDebug() << Q_FUNC_INFO << ": Software update to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    softUpdate = new SoftwareUpdate(softwareVersion);
    //qDebug() << Q_FUNC_INFO << ": FileManager to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    filemanager = new FileManager(dataProxy, klogDir, softwareVersion);
    //qDebug() << Q_FUNC_INFO << ": FileAwardManager to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    fileAwardManager = new FileAwardManager(dataProxy, Q_FUNC_INFO);

    lotwCallTQSL = new QAction(tr("Upload the queued QSOs to LoTW"), this);
    //qDebug() << Q_FUNC_INFO << ": AdifLoTWExportWidget to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    adifLoTWExportWidget = new AdifLoTWExportWidget(dataProxy, Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << ": ShowAdifImportWidget to be created " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    showAdifImportWidget = new ShowAdifImportWidget(dataProxy, Q_FUNC_INFO);

    logEvent(Q_FUNC_INFO, "END", logSeverity);


    //qDebug() << Q_FUNC_INFO << ": END " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
}

MainWindow::~MainWindow()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

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
    delete(debugFile);
    delete(dateTime);
    delete(dateTimeTemp);
    delete(awards);
    delete(softUpdate);
    delete(filemanager);
    delete(fileAwardManager);
    logEvent(Q_FUNC_INFO, "KLog exit", Info);
}

void MainWindow::saveWindowsSize()
{
    //qDebug() << "MainWindow::saveWindows" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    windowSize = this->size();

    int height = windowSize.height();
    int width = windowSize.width();
    //qDebug() << "MainWindow::windowsSizeAndPosition: /" << QString::number(width) << "/" << QString::number(height)  << ")" << QT_ENDL;
    //qDebug() << "MainWindow::windowsSizeAndPosition: Heigth: " << QString::number(height)  << QT_ENDL;
      //qDebug() << "MainWindow::windowsSizeAndPosition: Width: " << QString::number(width)  << QT_ENDL;
    //(const QString& _filename, const QString &_field, const QString &_value)
    filemanager->modifySetupFile(configFileName, "MainWindowSize", QString::number(width) + "x" + QString::number(height));
    //return QString::number(width) + "x" + QString::number(height);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "MainWindow::windowsSizeAndPosition: END" << QT_ENDL;
}

void MainWindow::setWindowSize(const QSize &_size)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //QSize size;
    //size.setHeight(_height);
    //size.setWidth(_width);
    if (_size.isValid ())
    {
        this->resize(_size);
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::init()
{
    //qDebug() << "MainWindow::init: START " << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    if (!debugFile->open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
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
        debugFileOpen = true;
        logEvent(Q_FUNC_INFO, "KLog started!", Info);
    }

    logEvents = true;

    hamlib->initClass();
    util->setCallValidation (true);
    infoLabel1T = QString();
    infoLabel2T = QString();
    qso->clear();

    //Default band/modes
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW" << "RTTY";
    //qDebug() << "MainWindow::init - 00" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;

    hamlibActive = false;
    hamlibModeNotADIFSupported = false;
    hamlibChangingMode = false;
    yearChangedDuringModification = false;

    readingTheUI = false;
    itIsANewversion = false;

    setCleaning(false);
    //qDebug() << "MainWindow::init - 10" << QT_ENDL;
    dxclusterServerToConnect = "dxfun.com";
    dxclusterServerPort = 8000;
    contestMode = "DX";
    infoTimeout = 2000; // default timeout

    defaultADIFLogFile = "klog.adi";

    InValidCharsInPrevCall = false;
    //stationCallSignShownInSearch = true;
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
    //qDebug() << "MainWindow::init - 20" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
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
    myPower = 0.0;
    UDPServerStart = false;   // By default the UDP server is started

    //qDebug() << "MainWindow::init - 30" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    currentEntity = -1; // To optimize the calls to different world methods if the entity does not change. Used in slotQRZTextChanged
    previousEntity = -1;// To optimize the calls to different world methods if the entity does not change.
    //realTime=true;
    //UTCTime=true;

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

    keepSatPage = false;
    //qDebug() << "MainWindow::init - 40" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    //clublogUser = QString();
    clublogPass = QString();
    clublogEmail = QString();
    clublogActive = false;
    clublogRealTime = false;

    //clublogUseStationCallSign = false;
    eQSLActive = false;
    eQSLRealTime = false;
    eQSLUseQSOStationCallSign = false;
    qrzcomActive = false;
    lotwActive = false;
    qrzcomSubscriber = false;

    qrzcomUser = QString();
    qrzcomPass = QString();

    callingUpdate = false; // to control whether the update is mannually launched or at the begining
    //previousQrz = "";
    setModifying(false);
    //qDebug() << "MainWindow::init - 50" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    selectedYear = (dateTime->currentDateTime()).date().year();
    loggWinAct->setShortcut(Qt::CTRL + Qt::Key_L);

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);


    // Check date & time and set them in the UI at the begining
    //dateTime->currentDateTime();

    clublogAnswer = -1;

    defaultColor.setNamedColor("slategrey");
    neededColor.setNamedColor("yellow");
    workedColor.setNamedColor("blue");
    confirmedColor.setNamedColor("red");
    newOneColor.setNamedColor("green");
    //qDebug() << "MainWindow::init - 60" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    bool existingData = QFile::exists(util->getKLogDBFile());
    ctyDatFile = util->getCTYFile();
    if (!existingData)
    {
        //qDebug() << "MainWindow::init - 61" << QT_ENDL;
        world->create(ctyDatFile);
        //qDebug() << "MainWindow::init - 62" << QT_ENDL;
    }
    else if (!world->hasSpecialEntities())
    {
        //qDebug() << "MainWindow::init - 63" << QT_ENDL;
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
        //qDebug() << "MainWindow::init - 69" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    }
    //qDebug() << "MainWindow::init - 70" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    readConfigData();

    //qDebug() << "MainWindow::init - 71" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    logWindow->createlogPanel(currentLog);
    //qDebug() << "MainWindow::init - 72" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    awards->setManageModes(manageMode);
    //qDebug() << "MainWindow::init - 73" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    if (dataProxy->getNumberOfManagedLogs()<1)
    {
        //qDebug() << "MainWindow::init - 73.1" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
        openSetup(6);
        //qDebug() << "MainWindow::init - 73.2" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    }
    //qDebug() << "MainWindow::init - 74" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    //checkIfNewBandOrMode();
    //qDebug() << "MainWindow::init - 75" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    awardsWidget->fillOperatingYears();
    awardsWidget->showAwards();
    awardsWidget->setManageDXMarathon(manageDxMarathon);

    dxClusterWidget->setCurrentLog(currentLog);
    //qDebug() << "MainWindow::init - 80" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    //qDebug() << "MainWindow::Init: calling Software update ..." << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    if (checkNewVersions)
    {//reportInfo
        if (reportInfo)
        {
            softUpdate->addCall(stationCallsign);
        }
        softUpdate->needToUpdate();
    }
    //qDebug() << "MainWindow::init - 90" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    currentBandShown = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
    //qDebug() << "MainWindow::init - 91" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    currentModeShown = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
    //qDebug() << "MainWindow::init - 92" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    currentBand = currentBandShown;
    currentMode = currentModeShown;

    timerInfoBars = new QTimer(this);

    //qDebug() << "MainWindow::init: Calling createUI" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    createUI();
    //qDebug() << "MainWindow::init: Calling slotClearButtonClicked" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    //qDebug() << Q_FUNC_INFO;
    slotClearButtonClicked();

    infoWidget->showInfo(-1);

    //lotwTQSLpath = util->getTQSLsPath() + util->getTQSLsFileName();
    upAndRunning = true;
    mainQSOEntryWidget->setUpAndRunning(upAndRunning);
    startServices();

    //qDebug() << "MainWindow::init: END" << (QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
}


void MainWindow::createActionsCommon(){
// Functional widgets connections
//TODO: Reimplement the possibility to enter a QSO with enter inthe following widgets:
    //connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
// Return pressed = QSO ENTRY

    //connect(operatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    //connect(stationCallSignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(QSOTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(QSOTabWidget, SIGNAL(dxLocatorChanged(QString)), this, SLOT(slotLocatorTextChanged(QString) ) );

    //connect(QSOTabWidget, SIGNAL(dxLocatorChanged(QString)) this, SLOT(slotLocatorTextChanged(QString) ) );
    //connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
    connect(myDataTabWidget, SIGNAL(myLocChangedSignal(QString)), this, SLOT(slotMyLocatorTextChanged(QString) ) );
    connect(myDataTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(QSOTabWidget, SIGNAL(rxFreqChanged(double)), this, SLOT(slotFreqRXChanged(double) )) ;
    connect(QSOTabWidget, SIGNAL(txFreqChanged(double)), this, SLOT(slotFreqTXChanged(double) )) ;
    connect(QSOTabWidget, SIGNAL(handOverFocusSignal()), this, SLOT(slotTakeOverFocusToMainQSOInput() ));
    connect(loggWinAct, SIGNAL(triggered()), this, SLOT(slotLogWinShow()));

    //Buttons Actions

    connect(mainQSOEntryWidget, SIGNAL(handOverFocusSignal()), this, SLOT(slotTakeOverFocusToQSOTabWidget()));
    connect(mainQSOEntryWidget, SIGNAL(currentQRZSignal(QString)), this, SLOT(slotQRZTextChanged(QString)));
    connect(mainQSOEntryWidget, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    connect(mainQSOEntryWidget, SIGNAL(showInfoLabel(QString)), this, SLOT(slotShowInfoLabel(QString)) );
    connect(mainQSOEntryWidget, SIGNAL(clearForNextQSOSignal()), this, SLOT(slotClearButtonClicked()) );
    connect(mainQSOEntryWidget, SIGNAL(OKClicked()), this, SLOT(slotQRZReturnPressed() ) );
    connect(mainQSOEntryWidget, SIGNAL(bandChanged(QString)), this, SLOT(slotBandChanged(QString) ) );
    connect(mainQSOEntryWidget, SIGNAL(modeChanged(QString)), this, SLOT(slotModeChanged(QString) ) );
    connect(mainQSOEntryWidget, SIGNAL(validBands(QStringList)), this, SLOT(slotValidBandsReceived(QStringList) ) );
    connect(mainQSOEntryWidget, SIGNAL(hamlibSetActiveSignal(bool)), this, SLOT(slotActiveHamlib(bool) ) );



    // LOGVIEW
    connect(logWindow, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(logWindow, SIGNAL(actionDeleteQSO ( int ) ), this, SLOT(slotQSODelete(int) ) );
    connect(logWindow, SIGNAL(deleteTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsDelete(QList<int>) ) );
    connect(logWindow, SIGNAL(exportToADIFTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsExportToADIF(QList<int>) ) );

    //connect(logWindow, SIGNAL(uploadToQRZcomTheseQSOs ( QList<int> ) ), this, SLOT(slotQRZcomUpload(QList<int>) ) );

    connect(logWindow, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(logWindow, SIGNAL(updateSearchText()), this, SLOT(slotSearchBoxTextChanged() ) ); //When a QSO is deleted
    connect(logWindow, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );

    //CLUSTER
    //void clusterSpotToLog(const QStringList _qs);
    //SIGNAL dxspotclicked(const QStringList _qs)
    connect(dxClusterWidget, SIGNAL(dxspotclicked(QStringList)), this, SLOT(slotAnalyzeDxClusterSignal(QStringList) ) );

    // CLUBLOG
    connect (elogClublog, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (elogClublog, SIGNAL (actionReturnDownload(int, int)), this, SLOT (slotElogClubLogProcessAnswer(int, int)));
    connect (elogClublog, SIGNAL (disableClubLogAction(bool)), this, SLOT (slotElogClubLogDisable(bool)));
    connect (elogClublog, SIGNAL (signalFileUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogClubLogFileUploaded(QNetworkReply::NetworkError, QList<int>)));

    //connect (eqslUtilities, SIGNAL (actionReturnDownload(int, int)), this, SLOT (slotElogClubLogProcessAnswer(int, int)));
    //connect (eqslUtilities, SIGNAL (disableClubLogAction(bool)), this, SLOT (slotElogClubLogDisable(bool)));
    connect (eqslUtilities, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (eqslUtilities, SIGNAL (signalFileUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogEQSLFileUploaded(QNetworkReply::NetworkError, QList<int>)));

    // QRZCOM
    connect (elogQRZcom, SIGNAL (showMessage(QString)), this, SLOT (slotElogQRZCOMShowMessage(QString)));
    connect (elogQRZcom, SIGNAL (dataFoundSignal(QString, QString)), this, SLOT (slotElogQRZCOMFoundData(QString, QString)));
    connect (elogQRZcom, SIGNAL (signalLogUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogQRZCOMLogUploaded(QNetworkReply::NetworkError, QList<int>)));
    //connect (elogQRZcom, SIGNAL (disableQRZAction(bool)), this, SLOT (slotElogQRZCOMDisable(bool)));

    // SATELLITE TAB
    //connect (satTabWidget, SIGNAL (satBandTXChanged(QString)), this, SLOT (slotSatBandTXComboBoxChanged(QString)));
    //connect(satTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );
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

   connect(UDPLogServer, SIGNAL(clearSignal()), this, SLOT(slotClearButtonClicked() ) );

   connect(this, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
   connect(setupDialog, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
   connect(setupDialog, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
   connect(setupDialog, SIGNAL(exitSignal(int)), this, SLOT(slotExitFromSlotDialog(int)) );
   connect(setupDialog, SIGNAL(qrzcomAuto(bool)), this, SLOT(slotElogQRZCOMAutoCheckFromSetup(bool)) );
   connect(setupDialog, SIGNAL(finished(int)), this, SLOT(slotSetupDialogFinished(int)) );


   connect(tipsDialog, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
   connect(tipsDialog, SIGNAL(findQSL2QSOSignal()), this, SLOT(slotSearchToolNeededQSLToSend()) );
   connect(tipsDialog, SIGNAL(fillInDXCCSignal()), this, SLOT(slotFillEmptyDXCCInTheLog()) );
   connect(tipsDialog, SIGNAL(fillInQSOSignal()), this, SLOT(fillQSOData()) );
   connect(tipsDialog, SIGNAL(fileExportToPrintSignal()), this, SLOT(slotRQSLExport()) );
   //connect(tipsDialog, SIGNAL(fileExportForLoTWSignal()), this, SLOT(slotLoTWExport()));
   connect(tipsDialog, SIGNAL(fileOpenKLogFolderSignal()), this, SLOT(slotOpenKLogFolder()));
   connect(tipsDialog, SIGNAL(toolSendPendingQSLSignal()), this, SLOT(slotToolSearchRequestedQSLToSend()));
   connect(tipsDialog, SIGNAL(toolRecPendingQSLSignal()), this, SLOT(slotToolSearchNeededQSLPendingToReceive()));
   connect(tipsDialog, SIGNAL(toolRecRecPendingQSLSignal()), this, SLOT(slotToolSearchNeededQSLRequested()));
   connect(tipsDialog, SIGNAL(toolsUploadLoTWSignal()), this, SLOT(slotLoTWExport()));

   connect(satTabWidget, SIGNAL(newBandsToBeAdded(QStringList)), this, SLOT(slotDefineNewBands(QStringList)) );
   connect(satTabWidget, SIGNAL(satTxFreqChanged(double)), this, SLOT(slotFreqTXChanged(double)  ) );
   connect(satTabWidget, SIGNAL(satRxFreqChanged(double)), this, SLOT(slotFreqRXChanged(double)  ) );
   connect(satTabWidget, SIGNAL(dxLocatorChanged(QString)), this, SLOT(slotUpdateLocator(QString)) );
   connect(satTabWidget, SIGNAL(setPropModeSat(QString, bool)), this, SLOT(slotSetPropModeFromSat(QString, bool)) ) ;
   connect(satTabWidget, SIGNAL(satTXFreqNeeded(double)), this, SLOT(slotFreqTXChanged(double)));
   connect(satTabWidget, SIGNAL(satRXFreqNeeded(double)), this, SLOT(slotFreqRXChanged(double)));
   connect(satTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );

    connect(othersTabWidget, SIGNAL(setPropMode(QString)), this, SLOT(slotSetPropModeFromOther(QString)) ) ;
    connect(downloadcty, SIGNAL(done(bool)), this, SLOT(slotWorldReload(bool)) );
    connect(timerInfoBars, SIGNAL(timeout()), this, SLOT(slotTimeOutInfoBars()) );
    connect(hamlib, SIGNAL(freqChanged(double)), this, SLOT(slotHamlibTXFreqChanged(double)) );
    connect(hamlib, SIGNAL(modeChanged(QString)), this, SLOT(slotHamlibModeChanged(QString)) );
    connect(lotwUtilities, SIGNAL(actionProcessLoTWDownloadedFile(QString)), this, SLOT(slotLoTWDownloadedFileProcess(QString)) );
    connect(adifLoTWExportWidget, SIGNAL(selection(QString, QDate, QDate, ExportMode)), this, SLOT(slotADIFExportSelection(QString, QDate, QDate, ExportMode)) );
    connect(dataProxy, SIGNAL(queryError(QString, QString, QString, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, QString, QString)) );
    connect(dataProxy, SIGNAL(debugLog(QString, QString, DebugLogLevel)), this, SLOT(slotCaptureDebugLogs(QString, QString, DebugLogLevel)) );
    //connect(this, SIGNAL(focusC), this, SLOT(slotTimeOutInfoBars()) );
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}



void MainWindow::recommendBackupIfNeeded()
{
    //qDebug() << "MainWindow::recommendBackupIfNeeded" << QT_ENDL;
    if (dataProxy->getHowManyQSOInLog(-1)<1)
    {
        return;
    }
    QDateTime lastBackupDate;
    lastBackupDate = QDateTime();
    lastBackupDate = filemanager->getDateTimeOfLastBackup();
    //qDebug() << "MainWindow::recommendBackupIfNeeded lastDate: " << util->getDateTimeSQLiteStringFromDateTime(lastBackupDate) << QT_ENDL;
    bool backupNeeded = false;
    QString msg;
    if (lastBackupDate == QDateTime())
    {
        backupNeeded = true;
        msg = tr("It seems that you have never done a backup or exported your log to ADIF.");
    }
    else if (lastBackupDate.addMonths(1) < QDateTime::currentDateTime())
    {
        //qDebug() << "MainWindow::recommendBackupIfNeeded More than a month"  << QT_ENDL;
        backupNeeded = true;
        msg = tr("It seems that the latest backup you did is older than one month.");
    }

    if (backupNeeded)
    {
        //qDebug() << "MainWindow::recommendBackupIfNeeded We need to backup"  << QT_ENDL;
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
            //QString filename = (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmm") + "-klogbackup.adi";
            QString filename = util->getBackupADIFile();
            //qDebug() << "MainWindow::recommendBackupIfNeeded: Backup to: " << filename  << QT_ENDL;
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
    //qDebug() << "MainWindow::recommendBackupIfNeeded - END"  << QT_ENDL;
}

void MainWindow::checkIfNewVersion()
{
     //qDebug() << "MainWindow::checkIfNewVersion"  << QT_ENDL;
    //itIsANewversion = true;
    if (itIsANewversion)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog - New version detected!"));
        msgBox.setText(tr("It seems that you are running this version of KLog for the first time."));
        msgBox.setInformativeText(tr("The setup will be open to allow you to do any new setup you may need."));
        msgBox.exec();
        openSetup(0);
    }
     //qDebug() << "MainWindow::checkIfNewVersion - END"  << QT_ENDL;
}

void MainWindow::createStatusBar()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    statusBar()->showMessage(tr("Ready"));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
void MainWindow::slotWorldMapShow()
{
    //worldMapWidget->resize(500,300);
    //worldMapWidget->loadMap();
    worldMapWidget->show();
}
*/
void MainWindow::setMainWindowTitle()
{
    QString aux = dataProxy->getCommentsFromLog(currentLog);
    int numberOfQSOs = dataProxy->getHowManyQSOInLog (currentLog);
      //qDebug() << "MainWindow::setMainWindowTitle:  (comment): " << aux << QT_ENDL;
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
    //qDebug() << "MainWindow::createUI" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    createStatusBar();
    setWindowTitle(tr("KLog"));
    createUIDX();
    createActionsCommon();
    createMenusCommon();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
            //qDebug() << "MainWindow::createUI-END" << QT_ENDL;
}

void MainWindow::slotTimeOutInfoBars()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
      //qDebug() << "MainWindow::slotTimeOutInfoBars - Start" << QT_ENDL;
    slotShowInfoLabel(infoLabel1T);
    //slotShowInfoLabel(infoLabel2T);
    //infoLabel1->setText(infoLabel1T);
    infoLabel2->setText(infoLabel2T);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::setModeFromFreq()
{
    //TODO: define frequency ranges for the different modes
    if (hamlibActive)
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
    //qDebug() << "MainWindow::slotBandChanged: " << _b << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if ((!upAndRunning) || (_b.length()<2))
    {
        //qDebug() << "MainWindow::slotBandChanged: !upAndRunning or band short"  << QT_ENDL;
        return;
    }

    if (updatingBands)
    {
        //qDebug() << "MainWindow::slotBandChanged: updating Bands"  << QT_ENDL;
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }

    bool isFRinBand = dataProxy->isThisFreqInBand(_b, QString::number(QSOTabWidget->getTXFreq ()));
    if ((isFRinBand) && (QSOTabWidget->getTXFreq () >0 ))
    { // No change in txFreq
        //qDebug() << "MainWindow::slotBandChanged: isFRinBand and Freq >0"  << QT_ENDL;
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

    currentBandShown = dataProxy->getIdFromBandName(_b);
    currentModeShown = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
    currentBand = currentBandShown;
    currentMode = currentModeShown;

    if ((!isFRinBand) || (QSOTabWidget->getTXFreq()<=0))
    {
        //qDebug() << "MainWindow::slotBandChanged: Freq is not in band or empty"  << QT_ENDL;
        //qDebug() << "MainWindow::slotBandChanged: Band: " << mainQSOEntryWidget->getBand()  << QT_ENDL;
        //qDebug() << "MainWindow::slotBandChanged: Freq: " << QString::number(QSOTabWidget->getTXFreq())  << QT_ENDL;
        double txFr = (dataProxy->getFreqFromBandId(currentBandShown)).toDouble();
         //qDebug() << "MainWindow::slotBandChanged: New Freq: " << QString::number(txFr) << QT_ENDL;

        slotFreqTXChanged (txFr);

        //if (!dataProxy->isThisFreqInBand(_b, QString::number(rxFreqSpinBox->value())))
        //{
        //    rxFreqSpinBox->setValue(QSOTabWidget->getTXFreq());
        //}
    }
    //qDebug() << "MainWindow::slotBandChanged: Checking to update Freq  - DONE"  << QT_ENDL;


    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);

     //qDebug() << "MainWindow:: - calling showStatusOfDXCC-02 " << QT_ENDL;
    showStatusOfDXCC(_qs);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::slotBandChanged: END" << QT_ENDL;
}

void MainWindow::slotModeChanged (const QString &_m)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!upAndRunning)
    {
        return;
    }
            //qDebug() << "MainWindow::slotModeChanged: " << _m << QT_ENDL;

    currentBandShown = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
             //qDebug() << "MainWindow::slotModeComboBoxChanged: currentBandShown2: " << QString::number(currentBandShown) << QT_ENDL;
    currentModeShown = dataProxy->getIdFromModeName(_m);
    currentBand = currentBandShown;
    currentMode = currentModeShown;

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
            //qDebug() << "MainWindow:: - calling showStatusOfDXCC-01 " << QT_ENDL;
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);
    showStatusOfDXCC(_qs);
    QSOTabWidget->setRSTToMode(mainQSOEntryWidget->getMode(), readingTheUI);

    QString _modeSeen = mainQSOEntryWidget->getMode();
    if (_modeSeen == "SSB")
    {
        setModeFromFreq();
    }

    logEvent(Q_FUNC_INFO, "END", logSeverity);
            //qDebug() << "MainWindow::slotModeComboBoxChanged2: " << mainQSOEntryWidget->getMode() << QT_ENDL;
}


void MainWindow::slotQRZReturnPressed()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
            //qDebug() << "MainWindow::slotQRZReturnPressed: "  << QT_ENDL;
    if (mainQSOEntryWidget->getQrz().length()<=0)
    {
          //qDebug() << "MainWindow::slotQRZReturnPressed: no QRZ"  << QT_ENDL;
        return;
    }
    readingTheUI = true;

    //int errorCode = 0;

    // Just to prepare or some tasks before reading DATA from UI

    QSqlQuery query;
    QString queryString = readDataFromUI();

      //qDebug() << "MainWindow::slotQRZReturnPressed: queryString: " << queryString << QT_ENDL;

    if (queryString != "NULL")
    {
        if (!query.exec(queryString))
        {
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                query.finish();
                  //qDebug() << "MainWindow::slotQRZReturnPressed: Query ERROR: (queryString): " << queryString << QT_ENDL;
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
                logEvent(Q_FUNC_INFO, "END-1", logSeverity);
                return;
            }
            else
            {
                query.finish();
                //TODO: To move the following lines to this part to properly manage the query result!!
                //ret = true;
                  //qDebug() << "MainWindow::slotQRZReturnPressed: QSO Added! " << QT_ENDL;
                actionsJustAfterAddingOneQSO();
                //slotClearButtonClicked();
            }
        }
    else   // The QUERY string is NULL
        {
            logEvent(Q_FUNC_INFO, "END-2", logSeverity);
              //qDebug() << "MainWindow::slotQRZReturnPressed: QUERY string is NULL " << QT_ENDL;
            readingTheUI = false;
            return;
        }

    yearChangedDuringModification = false;
    readingTheUI = false;
    //qDebug() << Q_FUNC_INFO;
    slotClearButtonClicked();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::actionsJustAfterAddingOneQSO()
{
      //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    int lastId = -1;
    needToSave = true;
    if (modify)
    {
      //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Modifying! " << QT_ENDL;
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
                //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (Modifiying ClubLog) Lastid: "<< QString::number(lastId) << QT_ENDL;
               // Delete QSO in CLubLog
               elogClublog->deleteQSO(clublogPrevQSO);
               // Add modified QSO in ClubLog
               elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(modifyingQSO));
           }
           else
           {
                          //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (No ClubLog) Lastid: "<< QString::number(lastId) << QT_ENDL;
           }
           awards->setAwards(modifyingQSO);   //Update the DXCC award status
       }
       // CHECK WHAT WAS THE QSOID to add the awards, if needed
    }
    else
    {
          //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Not Modifying " << QT_ENDL;
        lastId = dataProxy->getLastQSOid();
        if (lastId>=0)
        {
              //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Lastid: "<< QString::number(lastId) << QT_ENDL;
            awards->setAwards(lastId);   //Update the DXCC award status
            // Send to CLUBLOG if enabled
            if ((clublogActive) && (clublogRealTime))
            {
                  //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (Sending ClubLog) Lastid: "<< QString::number(lastId) << QT_ENDL;
                elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(lastId));
            }
            else
            {
                  //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (No ClubLog) Lastid: "<< QString::number(lastId) << QT_ENDL;
            }
            //<CLUBLOG>
        }
        else
        {
              //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Lastid < 0 "<< QString::number(lastId) << QT_ENDL;
        }
        //awards->setAwards(lastId);
    }
    logWindow->refresh();
    dxccStatusWidget->refresh();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
     //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO - END" << QT_ENDL;
}

QString MainWindow::readDataFromUI()
{
    //qDebug() << "MainWindow::readDataFromUI: " << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (modify)
    {
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return readDataFromUIDXModifying();
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-3", logSeverity);
        return readDataFromUIDX();
    }
    //logEvent(Q_FUNC_INFO, "END", logSeverity);
}

QString MainWindow::readDataFromUIDX()
{
/*
If you make any change here, please update also readDataFromUIDXModifying to keep data integrity!
*/
    //qDebug() << "MainWindow::readDataFromUIDX:" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
     //qDebug() << "MainWindow::readDataFromUIDX: Band: " << QString::number(currentBand) << QT_ENDL;
     //qDebug() << "MainWindow::readDataFromUIDX: Mode: " << QString::number(currentMode)  << QT_ENDL;
    int tband = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
    int tmode = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());

    QString tdate = util->getDateTimeSQLiteStringFromDateTime(mainQSOEntryWidget->getDateTime());
    QString ttime = (mainQSOEntryWidget->getTime()).toString("hh:mm:ss");

    QString trsttx = QSOTabWidget->getRSTTX ();
     //qDebug() << "MainWindow::readDataFromUIDX - RSTtx: " << trsttx << QT_ENDL;
    QString trstrx = QSOTabWidget->getRSTRX ();

    int dxcc = world->getQRZARRLId(tqrz);
    //int dxcc2 = getDXCCFromComboBox();
    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
     //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) << QT_ENDL;
     //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) << QT_ENDL;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);
     //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) << QT_ENDL;
     //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) << QT_ENDL;

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
            logEvent(Q_FUNC_INFO, "END-2", logSeverity);
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

    aux1 = QSOTabWidget->getDXLocator();

    if ( locator->isValidLocator(aux1)  )
    {
        stringFields = stringFields + ", gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }

             //qDebug() << "MainWindow::readDataFromUIDX: Reading freq...: " << QString::number(txFreqSpinBox->value()) << QT_ENDL;
    if ( QSOTabWidget->getTXFreq() > 0  )
    {
        aux1 = QString::number(QSOTabWidget->getTXFreq());
                //qDebug() << "MainWindow::readDataFromUIDX: Reading freq...: " << aux1 << "/" << tband << QT_ENDL;

        if (dataProxy->isThisFreqInBand(dataProxy->getNameFromBandId(tband), aux1) )
        {
            stringFields = stringFields + ", freq";
            stringData = stringData + ", '" + aux1 + "'";
                        //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND OK" << QT_ENDL;
        }
        else
        {
                        //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND NOK" << QT_ENDL;
        }
    }

    if ( QSOTabWidget->getRXFreq() > 0  )
    {
        //qDebug() << "MainWindow::readDataFromUIDX: TX FREQ & RX FREQ ARE DIFFERENT AND != 0" << QT_ENDL;
        aux1 = QString::number(QSOTabWidget->getRXFreq());
        stringFields = stringFields + ", freq_rx, band_rx";
        stringData = stringData + ", '" + aux1 + "', '" + QString::number(dataProxy->getBandIdFromFreq(QSOTabWidget->getRXFreq())) + "'";
        //stringData = stringData + ", '" + aux1 + ", " + QString::number(dataProxy->getBandIdFromFreq(rxFreqSpinBox->value())) + "'";
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
        //lastPower = aux1.toDouble();
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
    //qDebug() << "MainWindow::readDataFromUIDX: IOTA: " << aux1 << QT_ENDL;
    if (aux1.length() == 6) // EU-001
    {
      //qDebug() << "MainWindow::readDataFromUIDX: IOTA to be saved" << QT_ENDL;
        stringFields = stringFields + ", iota";
        stringData = stringData + ", '" + aux1 + "'";
    }

    // SATS

    aux1 = satTabWidget->getSatName(); //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
    //qDebug() << "MainWindow::readDataFromUIDX: SAT1 " << aux1 << QT_ENDL;
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_name";
        stringData = stringData + ", '" + aux1 + "'";
        aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
        if (aux1.length()>0)
        {
            stringFields = stringFields + ", sat_mode";
            stringData = stringData + ", '" + aux1 + "'";
        }
    }



    keepSatPage = satTabWidget->getRepeatThis();

    aux1 = othersTabWidget->getPropModeFromComboBox();
      //qDebug() << "MainWindow::readDataFromUIDX: PropMode:  " << aux1 << QT_ENDL;
    if ((aux1.length()>0) && (aux1 != "Not"))
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
      //qDebug() << "MainWindow::readDataFromUIDX: aux1: " << aux1 << " / aux2: " << aux2 << QT_ENDL;

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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return stringQuery;
}

QString MainWindow::readDataFromUIDXModifying()
{
      //qDebug() << "MainWindow::readDataFromUIDXModifying:" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

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

        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
    }

      //qDebug() << "MainWindow::readDataFromUIDXModifying: " << tqrz << QT_ENDL;

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
      //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) << QT_ENDL;
      //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) << QT_ENDL;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);
      //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) << QT_ENDL;
      //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) << QT_ENDL;

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
              //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 2: " << QString::number(dxcc2) << QT_ENDL;
        }
        else if (ret == QMessageBox::ActionRole)
        {
              //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 1: " << QT_ENDL;
        }
        else if (ret == QMessageBox::Cancel)
        {
            logEvent(Q_FUNC_INFO, "END-2", logSeverity);
              //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 2: " << QString::number(dxcc2) << QT_ENDL;
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
    //aux1 = (stationCallSignLineEdit->text()).toUpper();
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
    //qDebug() << "MainWindow::readDataFromUIDXModifying: DXCC=" << aux1 << QT_ENDL;
    if (dataProxy->isValidDXCC(dxcc))
    {
        updateString = updateString + "dxcc = '";
        updateString = updateString + aux1 + "', ";
        //qDebug() << "MainWindow::readDataFromUIDXModifying: Saving DXCC=" << aux1 << QT_ENDL;
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

    //aux1 = QString::number(myPowerSpinBox->value());

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
               //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA: " << aux1 << QT_ENDL;
    if (aux1.length() == 6) // EU-001
    {
                   //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA to be saved! " << QT_ENDL;
        updateString = updateString + "iota = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "iota = '', ";
                  //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) << QT_ENDL;
    }
    bool satQSO = false;
    aux1 = satTabWidget->getSatName();   //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
               //qDebug() << "MainWindow::readDataFromUIDX: SAT2 modif " << aux1 << QT_ENDL;
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
      //qDebug() << "MainWindow::readDataFromUIDX: PropMode:  " << aux1 << QT_ENDL;
    if ((aux1.length()>0) && (aux1 != "Not"))
    {
                    //qDebug() << "MainWindow::readDataFromUIDX: PropMode(1):  " << aux1 << QT_ENDL;
        updateString = updateString + "prop_mode = '";
        updateString = updateString + aux1 + "', ";
    }
    else if ((aux1.length()==0) || (aux1 == "Not"))
    {
          //qDebug() << "MainWindow::readDataFromUIDX: PropMode(2):  " << aux1 << QT_ENDL;
        updateString = updateString + "prop_mode = '',";
    }
    else
    {
        updateString = updateString + "prop_mode = '', ";
                   //qDebug() << "MainWindow::readDataFromUIDX: PropMode(3):  " << aux1 << QT_ENDL;
    }

    //CLUBLOG

    aux1 = eQSLTabWidget->getClubLogStatus(); //Y, N, M
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
               //qDebug() << "MainWindow::readDataFromUIDXModifying: aux1: " << aux1 << " / aux2: " << aux2 << QT_ENDL;

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
      //qDebug() << "MainWindow::readDataFromUIDXModifying: queryCreated: " << stringQuery << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return stringQuery;
}

void MainWindow::slotOKButtonClicked(){
              //qDebug() << "MainWindow::slotOKButtonClicked: "  << QT_ENDL;
    slotQRZReturnPressed();
}

void MainWindow::slotSearchBoxTextChanged()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    searchWidget->slotSearchBoxTextChanged();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotQSOsExportToADIF(QList<int> _id)
{
     //qDebug() << "MainWindow::slotQSOsExportToADIF " << QString::number(_id.length())  << QT_ENDL;
    if (_id.length()<1)
    {
        return; // NO QSO TO EXPORT
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
      //qDebug() << "MainWindow::slotQSOsExportToADIF: " << fileName << QT_ENDL;
    if ((!fileName.endsWith(".adi")) && ( !fileName.endsWith(".adif") ))
    {
         //qDebug() << "MainWindow::slotQSOsExportToADIF: Adding the .adi to the file" << fileName << QT_ENDL;
        fileName = fileName +  ".adi";
    }
     //qDebug() << "MainWindow::slotQSOsExportToADIF-1: " << fileName << QT_ENDL;
    filemanager->adifQSOsExport(fileName, _id);
     //qDebug() << "MainWindow::slotQSOsExportToADIF-3" << QT_ENDL;
    showNumberOfSavedQSO(fileName, _id.count());
      //qDebug() << "MainWindow::slotQSOsExportToADIF - END" << QT_ENDL;
}

void MainWindow::slotQRZcomUpload(QList<int> _id)
{
    //qDebug() << "MainWindow::slotQRZcomUpload " << QString::number(_id.length())  << QT_ENDL;
   //elogQRZcom->fetchData();

    elogQRZcom->sendQSOs(_id);

    //qDebug() << "MainWindow::slotQRZcomUpload - END" << QT_ENDL;
}
void MainWindow::slotQSOsDelete(QList<int> _id)
{
      //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(_id.length())  << QT_ENDL;
    //foreach(int i, _id)
    //{
    //      //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(i)  << QT_ENDL;
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
                  //qDebug() << "MainWindow::slotQSODelete: Just removed from log, now I will try to remove from ClubLog, if needed" << QT_ENDL;
                if (clublogActive && clublogRealTime)
                {
                      //qDebug() << "MainWindow::slotQSODelete: Removing from ClubLog" << QT_ENDL;
                    elogClublog->deleteQSO(qsoToDelete);
                }
                else
                {
                      //qDebug() << "MainWindow::slotQSODelete: NOT emoving from ClubLog" << QT_ENDL;
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
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
     //qDebug() << "MainWindow::slotQSODelete " << QString::number(_id)  << QT_ENDL;

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
                  //qDebug() << "MainWindow::slotQSODelete: Just removed from log, now I will try to remove from ClubLog, if needed" << QT_ENDL;
                if (clublogActive && clublogRealTime)
                {
                      //qDebug() << "MainWindow::slotQSODelete: Removing from ClubLog" << QT_ENDL;
                    elogClublog->deleteQSO(qsoToDelete);
                }
                else
                {
                      //qDebug() << "MainWindow::slotQSODelete: NOT emoving from ClubLog" << QT_ENDL;
                }

                dxccStatusWidget->refresh();
                logWindow->refresh();
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
    //qDebug() << "MainWindow::slotQSODelete END "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //awards->recalculateAwards();
}

void MainWindow::slotShowSearchWidget()
{
    //dxUpRightTab->addTab(searchWidget, tr("Search"));
             //qDebug() << "MainWindow::slotShowSearchWidget: " << QString::number(dxUpRightTab->indexOf(searchWidget)) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    dxUpRightTab->setCurrentIndex(dxUpRightTab->indexOf(searchWidget));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotLogRefresh()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    logWindow->refresh();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotElogClubLogDisable(const bool _b)
{
               //qDebug() << "MainWindow::slotElogClubLogDisable: " << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    clublogActive = !_b;
    setupDialog->setClubLogActive(clublogActive);

    //TODO: Disable clublog in the klogrc file
    //bool FileManager::modifySetupFile(const QString& _filename, const QString &_field, const QString &_value)
    filemanager->modifySetupFile(configFileName, "ClubLogActive", "False");
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotElogClubLogFileUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos)
{
      //qDebug() << "MainWindow::slotElogClubLogFileUploaded: " << QT_ENDL;

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
            //qDebug() << "MainWindow::slotElogClubLogFileUploaded file exist" << QT_ENDL;
     }
     else
     {
           //qDebug() << "MainWindow::slotElogClubLogFileUploaded file DOES NOT exist" << QT_ENDL;
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

                  //qDebug() << "MainWindow::slotElogClubLogFileUploaded - FILE REMOVED: " << fileName << QT_ENDL;
            }
            else
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(tr("KLog - ClubLog"));
                msgBox.setText(tr("The file has not been removed."));
                msgBox.setDetailedText(tr("It seems that there was something that prevented KLog from removing the file\nYou can remove it manually."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);

                  //qDebug() << "MainWindow::slotElogClubLogFileUploaded - FILE NOT REMOVED: " << fileName << QT_ENDL;
            }
            i = msgBox.exec();
        }
    }
    else
    {
        QFile::remove(fileName);
    }
}

void MainWindow::slotElogClubLogShowMessage(const QString &_s)
{
      //qDebug() << "MainWindow::slotElogClubLogShowMessage: " << _s << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    slotUpdateStatusBar(_s);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotElogClubLogProcessAnswer(const int _i, const int _qID)
{
               //qDebug() << "MainWindow::slotElogClubLogProcessAnswer: " <<QString::number(_i) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    clublogAnswer = _i;

    if (clublogAnswer == 0) // NO ERROR
    {
        dataProxy->setClubLogSent(_qID, "Y", eQSLTabWidget->getClubLogDate());
    }
    else
    {
        dataProxy->setClubLogSent(_qID, "M", eQSLTabWidget->getClubLogDate());
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::slotElogEQSLFileUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos)
{
      //qDebug() << "MainWindow::slotElogEQSLFileUploaded: " << QString::number(_error) << QT_ENDL;

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
            //qDebug() << "MainWindow::slotElogEQSLFileUploaded file exist" << fileName <<endl;
    }
     else
     {
           //qDebug() << "MainWindow::slotElogEQSLFileUploaded file DOES NOT exist" << QT_ENDL;
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

                  //qDebug() << "MainWindow::slotElogEQSLFileUploaded - FILE REMOVED: " << fileName << QT_ENDL;
            }
            else
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(tr("KLog - eQSL"));
                msgBox.setText(tr("The file has not been removed."));
                msgBox.setDetailedText(tr("It seems that there was something that prevented KLog from removing the file\nYou can remove it manually."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);

                  //qDebug() << "MainWindow::slotElogEQSLFileUploaded - FILE NOT REMOVED: " << fileName << QT_ENDL;
            }
            i = msgBox.exec();
        }
    }
    else
    {
        QFile::remove(fileName);
    }
      //qDebug() << "MainWindow::slotElogEQSLFileUploaded - END"  << QT_ENDL;
}

void MainWindow::slotElogQRZCOMDisable(const bool _b)
{
    //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
        setupDialog->setQRZCOMAutoCheckActive (false);
        filemanager->modifySetupFile(configFileName, "QRZcomActive", "False");
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotElogQRZCOMLogUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos)
{
     //qDebug() << "MainWindow::slotElogQRZCOMLogUploaded: " << QString::number(_error) << QT_ENDL;

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
           //qDebug() << "MainWindow::slotElogQRZCOMLogUploaded file exist" << QT_ENDL;
   }
    else
    {
          //qDebug() << "MainWindow::slotElogEQSLFileUploaded file DOES NOT exist" << QT_ENDL;
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

     //qDebug() << "MainWindow::slotElogEQSLFileUploaded - END"  << QT_ENDL;
}

void MainWindow::slotElogQRZCOMShowMessage(const QString &_s)
{
    //qDebug() << "MainWindow::slotElogQRZCOMShowMessage: " << _s << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    slotUpdateStatusBar(_s);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
void MainWindow::cleanQRZCOMreceivedDataFromUI()
{
    //qDebug() << Q_FUNC_INFO;
    if (!modify)
    {
        QSOTabWidget->cleanQRZCOM();
        completedWithPreviousName = false;
        completedWithPreviousName = false;
        completedWithPreviousLocator = false;
    }
}

void MainWindow::slotElogQRZCOMFoundData(const QString &_t, const QString & _d)
{
  //qDebug() << "MainWindow::slotElogQRZCOMFoundData: " << _t << "/" << _d << QT_ENDL;
   if (_t == "name")
   {
       if (QSOTabWidget->getName().length()<1)
       {
           QSOTabWidget->setName(_d);
       }
   }
   else if (_t == "grid")
   {
       //qDebug() << Q_FUNC_INFO << " Grid found: " << _d;
       if ((QSOTabWidget->getDXLocator()).length()<1)
       {
           QSOTabWidget->setDXLocator(_d);
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
           QSOTabWidget->setQTH(_d);
       }
   }
   else if (_t == "qslmgr")
   {
        //QSLTabWidget->setQSLVia(_d);
   }
//   else if (_t == "message")
//   {
//       QMessageBox msgBox;
//       msgBox.setIcon(QMessageBox::Information);
//       msgBox.setWindowTitle(tr("KLog - QRZ.com message"));
//       QString aux = QString(tr("KLog has received a message from QRZ.com.") );
//       msgBox.setText(aux);
//       msgBox.setDetailedText(_d);
//       msgBox.setStandardButtons(QMessageBox::Ok);
//       msgBox.setDefaultButton(QMessageBox::Ok);
//       msgBox.exec();
//   }
   else if (_t == "error")
    {
        //qDebug() << "MainWindow::slotElogQRZCOMFoundData: ERROR" << _t << "/" << _d << QT_ENDL;
        if (_d.contains("Not found: "))
        {
            cleanQRZCOMreceivedDataFromUI();
            //qDebug() << "MainWindow::slotElogQRZCOMFoundData: call Not found" << QT_ENDL;
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
        //qDebug() << "MainWindow::slotElogQRZCOMFoundData: QRZ value not recognized" << QT_ENDL;
   }
}

void MainWindow::slotElogQRZCOMCheckThisCall()
{
     //qDebug() << Q_FUNC_INFO  << QT_ENDL;
    if (qrzcomActive)
    {
        elogQRZcom->checkQRZ(mainQSOEntryWidget->getQrz());
    }
    else
    {
      showMessageToEnableTheOnlineService(QRZ)  ;
    }
    //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
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
     //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString(QRZCOMAutoCheckAct->isChecked()) << QT_ENDL;
    if (!qrzcomActive)
    {
        showMessageToEnableTheOnlineService(QRZ);
        return;
    }
    setupDialog->setQRZCOMAutoCheckActive(QRZCOMAutoCheckAct->isChecked());
    //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
}

void MainWindow::slotElogQRZCOMAutoCheckFromSetup(const bool _s)
{
    //qDebug() << Q_FUNC_INFO << QT_ENDL;

    if (qrzcomActive)
    {
        QRZCOMAutoCheckAct->setEnabled(_s);
    }
    else
    {
      showMessageToEnableTheOnlineService(QRZ);
    }

    //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
}

void MainWindow::slotExitFromSlotDialog(const int exitID)
{
      //qDebug() << "MainWindow::slotExitFromSlotDialog: " << QString::number(exitID) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (exitID == 2)
    {
        needToEnd = true;
        exitQuestion();
    }
      //qDebug() << "MainWindow::slotExitFromSlotDialog: END "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotFileClose()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    exitQuestion();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::exitQuestion()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
      //qDebug() << "MainWindow::exitQuestion"  << QT_ENDL;
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
        logEvent(Q_FUNC_INFO, "Exiting KLog!", Info);
            //maybeSave();
            saveWindowsSize();
            close();
            exit(0);
        default:
            // should never be reached
        break;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotQRZTextChanged(QString _qrz)
{
    //qDebug()<< Q_FUNC_INFO << ": " << _qrz << QT_ENDL;

    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (_qrz.length()<1)
    {
       //qDebug()<< Q_FUNC_INFO << ": Empty... " << QT_ENDL;
        infoLabel1->clear();
        infoLabel2->clear();
        //qDebug() << Q_FUNC_INFO;
        slotClearButtonClicked();
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    //qDebug()<< Q_FUNC_INFO << ": cursor position: " << QT_ENDL;

    if (cleaning)
    {
        //qDebug()<< Q_FUNC_INFO << ": Cleaning" << QT_ENDL;
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

    if (modify)
    {
        return;
    }

    //qDebug()<< Q_FUNC_INFO << ": checking for modify or length<1" << QT_ENDL;
    if (qrzSmallModDontCalculate)
    //if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
        //qDebug()<< Q_FUNC_INFO << ": MODIFY or Lenght < 1" << QT_ENDL;
        qrzSmallModDontCalculate=false;
        logEvent(Q_FUNC_INFO, "END-6", logSeverity);
        return;
    }

    //qDebug()<< Q_FUNC_INFO << ": running ..." << QT_ENDL;
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    //int i;
    int dx_CQz = -1;
    int dxE_CQz = -1;
    int dx_ITUz = -1;
    int dxE_ITUz = -1;
    //cleanQRZCOMreceivedDataFromUI();
    //qDebug()<< Q_FUNC_INFO << ": currentQRZ: " <<_qrz << QT_ENDL;
    QString pref = util->getPrefixFromCall(_qrz);
    //qDebug()<< Q_FUNC_INFO << ": pref: " << pref << QT_ENDL;

    if (pref.length ()>0)
    {
        currentEntity = world->getQRZARRLId(pref);
    }
    else
    {
        currentEntity = world->getQRZARRLId(_qrz);
    }

    //currentEntity = world->getQRZARRLId(util->getPrefixFromCall(_qrz));
    //currentEntity = world->getQRZARRLId(_qrz);
    //selectCorrectComboBoxEntity(currentEntity);
    //qDebug()<< Q_FUNC_INFO << ": currentEntity: " << QString::number(currentEntity) << QT_ENDL;
    othersTabWidget->setEntity(currentEntity);

    dxE_CQz = world->getEntityCqz(currentEntity);
    dx_CQz = world->getQRZCqz(_qrz);
    dx_ITUz = world->getQRZItuz(_qrz);
    dxE_ITUz = world->getEntityItuz(currentEntity);
    //qDebug()<< Q_FUNC_INFO << ": CQ: " << QString::number(dx_CQz) << QT_ENDL;
    //qDebug()<< Q_FUNC_INFO << ": CQe: " << QString::number(dxE_CQz) << QT_ENDL;
    //qDebug()<< Q_FUNC_INFO << ": ITU: " << QString::number(dx_ITUz) << QT_ENDL;
    //qDebug()<< Q_FUNC_INFO << ": ITUe: " << QString::number(dxE_ITUz) << QT_ENDL;

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
    //qDebug()<< Q_FUNC_INFO << ": currentEntity: " << QString::number(currentEntity) << QT_ENDL;
    if ( locator->isValidLocator(QSOTabWidget->getDXLocator()))
    {
        dxLocator = QSOTabWidget->getDXLocator();
    }
    else
    {
        dxLocator = world->getLocator(currentEntity);
    }

    //qDebug()<< Q_FUNC_INFO << ": Going to check the DXCC" << QT_ENDL;
    //qDebug()<< Q_FUNC_INFO << ": current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) << QT_ENDL;
        if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) || (dx_CQz != dxE_CQz) || (dx_ITUz != dxE_ITUz))
        {
            //qDebug()<< Q_FUNC_INFO << ": currentEntity=" << QString::number(currentEntity) << "/previousEntity=" << QString::number(previousEntity)  << QT_ENDL;
            previousEntity = currentEntity;
            InValidCharsInPrevCall = false;
            //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
            infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
            //qDebug()<< Q_FUNC_INFO << ": calling showStatusOfDXCC-03 " << QT_ENDL;
            showStatusOfDXCC(_qs);
            showDXMarathonNeeded(currentEntity, dx_CQz, mainQSOEntryWidget->getDate().year(), currentLog);
            othersTabWidget->setIOTAContinentFromEntity(currentEntity);
        }
        else if ((dx_CQz == dxE_CQz) || (dx_ITUz = dxE_ITUz))
        {
            //qDebug()<< Q_FUNC_INFO << ": 000" << QT_ENDL;
            //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
        }
        else
        {
           //qDebug()<< Q_FUNC_INFO << ": Default: else" << QT_ENDL;
        }

    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
    //qDebug()<< Q_FUNC_INFO << ": cursorP at the end : "  << QT_ENDL;

  if (completeWithPrevious)
  {
    completeWithPreviousQSO(_qrz);
  }

  if (!modify)
    {
        searchWidget->setCallToSearch(_qrz);

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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug()<< Q_FUNC_INFO << ": END" << QT_ENDL;
}

void MainWindow::setCleaning(const bool _c)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    cleaning = _c;
    mainQSOEntryWidget->setCleaning(cleaning);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotClearButtonClicked()
{
    //qDebug() << "MainWindow::slotClearButtonClicked - START" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    bool needToRecover = modify;
    setCleaning(true);
    yearChangedDuringModification = false;
    setModifying(false);

    dateTimeTemp = dateTime;
    modifyingQSO = -1;

    QSOTabWidget->setRSTToMode(mainQSOEntryWidget->getMode(), readingTheUI);

    if (currentBand < 0)
    {
        currentBand = defaultBand;
    }
    if (currentMode < 0)
    {
        currentMode = defaultMode;
        //qDebug() << "MainWindow::MainWindow: 12b - currentMode: " << QString::number(currentMode) << QT_ENDL;
    }

    clublogAnswer = -1;
    clublogPrevQSO.clear();
    //qDebug() << "MainWindow::slotClearButtonClicked: - 11"  << QT_ENDL;

    setCleaning(false);

    //qDebug() << "MainWindow::slotClearButtonClicked: Log: " << QString::number(currentLog) << QT_ENDL;
    setMainWindowTitle();
    if (needToRecover)
    {
        //qDebug() << Q_FUNC_INFO << ": Recovening the previous status...";
        restoreCurrentQSO(true);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO;
        clearUIDX();
        statusBar()->clearMessage();

        //qDebug() << Q_FUNC_INFO << ": NOT recovening the previous status...";
    }

    //qDebug() << "MainWindow::slotClearButtonClicked: " << mainQSOEntryWidget->getMode() << QT_ENDL;
    //qDebug() << "MainWindow::slotClearButtonClicked - currentMode = " << QString::number(currentMode) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
            //qDebug() << "MainWindow::slotClearButtonClicked - END" << QT_ENDL;
}

void MainWindow::clearUIDX(bool _full)
{
    //qDebug() << Q_FUNC_INFO << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

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

     //qDebug() << "MainWindow::clearUIDX deciding wether to change or not the Freq: " << QString::number(txFreqSpinBox->value()) << QT_ENDL;
    if (QSOTabWidget->getTXFreq()<=0)
    {
        //qDebug() << "MainWindow::clearUIDX Setting TX Freq from: " << QString::number(txFreqSpinBox->value()) << QT_ENDL;
        QSOTabWidget->setTXFreq((dataProxy->getFreqFromBandId(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand()))).toDouble());
        //qDebug() << "MainWindow::clearUIDX Setting TX Freq to: " << QString::number(txFreqSpinBox->value()) << QT_ENDL;
        QSOTabWidget->setRXFreq(QSOTabWidget->getTXFreq());
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
}


void MainWindow::slotRefreshDXCCWidget()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    dxccStatusWidget->slotRefreshButtonClicked();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
     //qDebug() << "MainWindow::closeEvent" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (maybeSave())
    {
         //qDebug() << "MainWindow::closeEvent saving needed" << QT_ENDL;
        dataProxy->unMarkAllQSO();
        dataProxy->compressDB();
        saveWindowsSize();
        event->accept();
    }
    else
    {
         //qDebug() << "MainWindow::closeEvent not saving needed" << QT_ENDL;
        event->ignore();
    }
     //qDebug() << "MainWindow::closeEvent-END" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

bool MainWindow::maybeSave()
{
    //qDebug() << "MainWindow::maybeSave" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QString str = tr("The logfile has been modified.") + "\n" + tr("Do you want to save your changes?");

    if (alwaysADIF)
    {
        //qDebug() << "MainWindow::maybeSave - Saving" << QT_ENDL;
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
                    logEvent(Q_FUNC_INFO, "END-1", logSeverity);
                    //qDebug() << "MainWindow::maybeSave - Use default file name" << QT_ENDL;

                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("KLog - ADIF export"));
                    msgBox.setInformativeText(tr("It is important to export to ADIF and save a copy as a backup."));
                    if (filemanager->adifLogExport(defaultADIFLogFile, currentLog)) // 0 will save ALL the logs)
                    {
                        msgBox.setIcon(QMessageBox::Information);
                        msgBox.setText(tr("Saving the log was done successfully."));
                        msgBox.exec();
                        //qDebug() << "MainWindow::maybeSave - Log exported" << QT_ENDL;
                        return true;
                    }
                    else
                    {
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText(tr("The ADIF export was not properly done."));
                        msgBox.exec();
                        //qDebug() << "MainWindow::maybeSave - ERROR Log not exported" << QT_ENDL;
                        return false;
                    }
                }
                else
                {
                    logEvent(Q_FUNC_INFO, "END-2", logSeverity);
                    //qDebug() << "MainWindow::maybeSave - Going to import??" << QT_ENDL;
                    slotADIFImport();
                    return true;
                    //return saveFileAs();
                }
            }
            else if (ret == QMessageBox::Cancel)
            {
                logEvent(Q_FUNC_INFO, "END-3", logSeverity);
                //qDebug() << "MainWindow::maybeSave - Cancel" << QT_ENDL;
                return false;
            }
        }
        else
        {
            //qDebug() << "MainWindow::maybeSave - Not needing to save" << QT_ENDL;
        }
    }
    else
    {
       //qDebug() << "MainWindow::maybeSave - Not Checking if needed to save" << QT_ENDL;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return true;
}

void MainWindow::slotAWAImport()
{
      //qDebug() << "MainWindow::slotAWAImport" << QT_ENDL;
    fileAwardManager->importNewAwardFile();
      //qDebug() << "MainWindow::slotAWAImport - END" << QT_ENDL;
}

void MainWindow::createMenusCommon()
{
           //qDebug() << "MainWindow::createMenusCommon" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
    fileMenu->addSeparator();

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

    lotwMarkSentQueuedThisLogAct = new QAction(tr("Queue all QSLs from this log to be sent"), this);
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

    //qDebug() << "MainWindow::createMenusCommon before" << QT_ENDL;
    //toolMenu->addSeparator();
    //showRotatorAct = new QAction (tr("Rotator"), this);
    //toolMenu->addAction(showRotatorAct);
    //connect(showRotatorAct, SIGNAL(triggered()), this, SLOT(slotRotatorShow()));
    //showRotatorAct->setToolTip(tr("Show the rotator controller."));
    //qDebug() << "MainWindow::createMenusCommon after" << QT_ENDL;

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
    #else                   // I can define the setMenuRole but then no standard locations are used on macOS
    aboutQtAct = new QAction(tr("About Qt ..."), this);
    helpMenu->addAction(aboutQtAct);
    //aboutQtAct->setMenuRole(QAction::AboutQtRole);
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(slotAboutQt()));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    #endif
    helpMenu->addSeparator();

    updateAct = new QAction(tr("Check updates ..."), this);
    helpMenu->addAction(updateAct);
    //updateAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(updateAct, SIGNAL(triggered()), this, SLOT(slotHelpCheckUpdatesAction()));
 }

void MainWindow::slotDebugAction()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    showKLogLogWidget->show();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
void MainWindow::slotTest()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << Q_FUNC_INFO << QT_ENDL;
    //showKLogLogWidget->setWindowModality(false);
    showKLogLogWidget->show();
    //advancedSearchWidget->show();
    //qDebug() << Q_FUNC_INFO << " - END "<< QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
*/

void MainWindow::slotSearchToolNeededQSLToSend()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    slotShowSearchWidget();
    searchWidget->searchToolNeededQSLToSend();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotToolSearchRequestedQSLToSend()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    searchWidget->slotToolSearchRequestedQSLToSend();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotToolSearchNeededQSLPendingToReceive()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    searchWidget->slotToolSearchNeededQSLPendingToReceive();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotToolSearchNeededQSLRequested()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    searchWidget->slotToolSearchNeededQSLRequested();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::slotToolLoTWMarkAllQueuedThisLog()
{
    //qDebug() << "MainWindow::slotToolLoTWMarkAllQueuedThisLog"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END", logSeverity);
    }
}

/*
 void MainWindow::slotLoTWTest()
{
       //qDebug() << "MainWindow::slotLoTWTest: "  << QT_ENDL;
     slotLoTWDownloadedFileProcess("lotwimport.adi");
       //qDebug() << "MainWindow::slotLoTWTest - END"  << QT_ENDL;
}
*/

void MainWindow::slotLoTWDownloadedFileProcess(const QString &_fn)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
     //qDebug() << "MainWindow::slotLoTWDownloadedFileProcess: " << _fn << QT_ENDL;
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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotToolLoTWMarkAllQueued()
{
    //qDebug() << "MainWindow::slotToolLoTWMarkAllQueued"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END", logSeverity);
    }
}

bool MainWindow::callTQSL(const QString &_filename, const QString &_call)
{ //https://lotw.arrl.org/lotw-help/cmdline/
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
      //qDebug() << "MainWindow::callTQSL: " << lotwTQSLpath << QT_ENDL;

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

          //qDebug() << "MainWindow::callTQSL-ok: " << QString::number(ok) << QT_ENDL;

        switch (ok)
        {
        case 0: // success: all qsos submitted were signed and saved or signed and uploaded
                    //qDebug() << "MainWindow::callTQSL: 0"  << QT_ENDL;
            //msg = tr("All the QSOs were signed and uploaded with no error.");
            //msg = tr("TQSL finished with no error.");
            //msgBox.setIcon(QMessageBox::Information);
        break;
        case 1: // cancelled by user
                    //qDebug() << "MainWindow::callTQSL: 1"  << QT_ENDL;
            msg = tr("Error #1: The process was cancelled by the user or TQSL was not configured. No QSOs were uploaded.");
        break;
        case 2: // rejected by LoTW
                    //qDebug() << "MainWindow::callTQSL: 2"  << QT_ENDL;
            msg = tr("Error #2: Upload was rejected by LoTW, please check your data.");
            break;
        case 3: // unexpected response from TQSL server
                    //qDebug() << "MainWindow::callTQSL: 3"  << QT_ENDL;
            msg = tr("Error #3: The TQSL server returned an unexpected response.");
            break;
        case 4: // TQSL error
                    //qDebug() << "MainWindow::callTQSL: 4"  << QT_ENDL;
            msg = tr("Error #4: There was a TQSL error.");
            break;
        case 5: // TQSLlib error
                    //qDebug() << "MainWindow::callTQSL: 5"  << QT_ENDL;
            msg = tr("Error #5: There was a TQSLLib error.");
            break;
        case 6: // unable to open input file
                    //qDebug() << "MainWindow::callTQSL: 6"  << QT_ENDL;
            msg = tr("Error #6: It was not possible to open the input file.");
            break;
        case 7: // unable to open output file
                    //qDebug() << "MainWindow::callTQSL: 7"  << QT_ENDL;
            msg = tr("Error #7: It was not possible to open the ouput file.");
            break;
        case 8: // No QSOs were processed since some QSOs were duplicates or out of date range
                    //qDebug() << "MainWindow::callTQSL: 8"  << QT_ENDL;
            msg = tr("Error #8: No QSOs were processed since some QSOs were duplicates or out of date range.");
            break;
        case 9: // Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range
                    //qDebug() << "MainWindow::callTQSL: 9"  << QT_ENDL;
            msg = tr("Error #9: Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range.");
            break;
        case 10: // command syntax error
                    //qDebug() << "MainWindow::callTQSL: 10"  << QT_ENDL;
            msg = tr("Error #10: Command syntax error. KLog sent a bad syntax command.");
            break;
        case 11: // LoTW Connection error (no network or LoTW is unreachable)
                    //qDebug() << "MainWindow::callTQSL: 11"  << QT_ENDL;
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
    //logEvent(Q_FUNC_INFO, "END", logSeverity);
}

QString MainWindow::selectStationCallsign()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
            logEvent(Q_FUNC_INFO, "END-1", logSeverity);
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
                    logEvent(Q_FUNC_INFO, "END-2", logSeverity);
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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return QString();
}

void MainWindow::slotToolLoTWMarkAllYesThisLog()
{
        //qDebug() << "MainWindow::slotToolLoTWMarkAllYesThisLog"  << QT_ENDL;
        //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("KLog - LoTW"));
        logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotToolLoTWMarkAllYes()
{
             //qDebug() << "MainWindow::slotToolLoTWMarkAllYes"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotReceiveQSOListToShowFromFile(QStringList _qs)
{
      //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile: " << QString::number(_qs.length())  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    // Receiving:  modifiedQSOList << _call << _date + "-" +_time << _band << _mode << _qslrdate;
    // Must send QRZ-DX, Date-Time(yyyyMMdd-hhmmss), Band, Mode
    if (_qs.length()!=4)
    {
          //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid qso list received - length: " << QString::number(_qs.length()) << QT_ENDL;
        return;
    }

    if (!util->isValidCall(_qs.at(0)))
    {
          //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid QRZ received - " << _qs.at(0) << QT_ENDL;
        return;
    }

    if (dataProxy->getIdFromBandName(_qs.at(2))<0)
    {
         //qDebug() << "SMainWindow::slotReceiveQSOListToShowFromFile - NO valid BAND received"<< _qs.at(2) << QT_ENDL;
        return;
    }
    if (dataProxy->getIdFromModeName(_qs.at(3))<0)
    {
          //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid Mode received" << _qs.at(3)<< QT_ENDL;
        return;
    }
    if (!util->isValidDateTimeFromString(_qs.at(1)))

    {
          //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile No valid date" << QT_ENDL;
            return;
    }
      //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile ALL OK" << QT_ENDL;
    //QStringList qsoToSend;
    //qsoToSend.clear();
    //qsoToSend << _qs.at(0) << _qs.at(1) << _qs.at(2) << _qs.at(3);
    showAdifImportWidget->addQSOToTheList(_qs);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotAboutQt()
{
            //qDebug() << "MainWindow::slotAboutQt"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QMessageBox::aboutQt(this,tr("About ..."));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotHelpAboutAction()
{
                //qDebug() << "MainWindow::slotHelpAboutAction "  << QT_ENDL;
   // QMessageBox::about(this, tr("About KLog"),
    //            tr("<b>KLog</b> "
   //                "Find the last release at https://jaime.robles.es/klog."));

    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    aboutDialog->exec();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //helpAboutDialog->exec();
}
void MainWindow::slotTipsAction()
{
            //qDebug() << "MainWindow::slotTipsAction "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    tipsDialog->exec();


    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotHelpCheckUpdatesAction()
{
            //qDebug() << "MainWindow::slotHelpCheckUpdatesAction" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    callingUpdate = true;
    softUpdate->addCall(stationCallsign);
    softUpdate->needToUpdate(true);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotShowSoftUpdateResults(const bool _b)
{
               //qDebug() << "MainWindow::slotShowSoftUpdateResults: " << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (_b == true)
    {
               //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = TRUE " << QT_ENDL;
    }
    else
    {
                   //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = FALSE " << QT_ENDL;
    }

    if (callingUpdate == true)
    {
        if (_b == false)
        {
                       //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NOT NEEDED" << QT_ENDL;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog - Update checking result"));
            msgBox.setText(tr("Congratulations!") + "\n\n" + tr("You already have the latest version."));
            msgBox.exec();
        }
        else
        {
                       //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NEEDED" << QT_ENDL;
        }
    }
    callingUpdate = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}



void MainWindow::slotLogWinShow()
{
                //qDebug() << "MainWindow::slotLogWinShow: "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (!(logWindow->isVisible()) )
    {
        logWinAct->setChecked ( true );
        logWindow->show();
    }else
    {
        logWinAct->setChecked ( false );
        logWindow->hide();
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotSetup(const int _page)
{
     //qDebug() << "MainWindow::slotSetup: " << QString::number(_page)  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    configured = false;
    backupCurrentQSO ();
    openSetup(_page);
     //qDebug() << "MainWindow::slotSetup - END"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::openSetup(const int _page)
{
     //qDebug() << Q_FUNC_INFO << ": " << QString::number(_page)  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //int result = -1;
    hamlib->stop();
    if (!needToEnd)
    {
        logEvent(Q_FUNC_INFO, "Just before setData", logSeverity);
         //qDebug() << "MainWindow::openSetup - Just before setupDialog->exec-1"  << QT_ENDL;
        if (upAndRunning)
        {
            setupDialog->setData(configFileName, softwareVersion, _page, !configured);
        }
        else
        {
            setupDialog->setData(configFileName, softwareVersion, 0, !configured);
        }
        if ( (!configured) || (itIsANewversion) )
        {
            logEvent(Q_FUNC_INFO, "Just before SetupDialog->exec", logSeverity);
            itIsANewversion = false;
            //setupDialog->exec();

            setupDialog->setModal(true);
            setupDialog->show();
            // move part of this code to slotSetupDialogFinished
            logEvent(Q_FUNC_INFO, "Just after setupDialog->show", logSeverity);
             //qDebug() << "MainWindow::openSetup - Just after setupDialog->show" << QT_ENDL;
        }
        else
        {
            logEvent(Q_FUNC_INFO, "No setupDialog->exec needed", logSeverity);
             //qDebug() << "MainWindow::openSetup - No setupDialog->show needed"  << QT_ENDL;
        }
    }
     //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotSetupDialogFinished (const int _s)
{
    //qDebug() << Q_FUNC_INFO << ": " <<  QString::number(_s);

    if (needToEnd)
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    if (_s == QDialog::Accepted)
    {
        //qDebug() << Q_FUNC_INFO << " - QDialog::Accepted";
        logEvent(Q_FUNC_INFO, "Just before readConfigData", logSeverity);
        readConfigData();
        reconfigureDXMarathonUI(manageDxMarathon);
        logEvent(Q_FUNC_INFO, "Just after readConfigData", logSeverity);
        //qDebug() << "MainWindow::slotSetupDialogFinished: logmodel to be created-2" << QT_ENDL;
        logEvent(Q_FUNC_INFO, "logmodel to be created-2", logSeverity);
        logWindow->createlogPanel(currentLog);
        logEvent(Q_FUNC_INFO, "logmodel has been created-2", logSeverity);
        defineStationCallsign(stationCallsign);
        logEvent(Q_FUNC_INFO, "before db->reConnect", logSeverity);
         //qDebug() << "MainWindow::openSetup: before db->reConnect" << QT_ENDL;
        dataProxy->reconnectDB();
        logEvent(Q_FUNC_INFO, "after db->reConnect", logSeverity);
        //qDebug() << "MainWindow::openSetup: after db->reConnect" << QT_ENDL;

        if (hamlibActive)
        {
             //qDebug() << "MainWindow::slotSetupDialogFinished: Hamlib is active, let's read the VFO Freq/Mode" << QT_ENDL;
        }
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << " - !QDialog::Accepted";
    }

    if (qso->getBackup ())
    {
        //qDebug() << Q_FUNC_INFO << ": Restoring..." << QT_ENDL;
        restoreCurrentQSO (QDialog::Accepted);
    }
    else
    {
        //qDebug() << "MainWindow::slotSetupDialogFinished: NO Restoring..." << QT_ENDL;
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

bool MainWindow::slotOpenKLogFolder()
{
    //qDebug() << "MainWindow::slotOpenKLogFolder: " << configFileName << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //configFileName = klogDir+"/klogrc.cfg";
    QString _aux = "<ul><li><a href=file://" + util->getHomeDir() + ">file://" + util->getHomeDir() + "</a></li>" +
                    "<li><a href=file://" + util->getKLogDBFile() + ">file://" + util->getKLogDBFile() + "</a></i></ul>" ;
    QString _text = tr("You can find the KLog data folder here: ") + _aux;

    QMessageBox::information(this, tr("KLog"),
                                   _text,
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
             //qDebug() << "MainWindow::slotOpenKLogFolder: END"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return true;
}


void MainWindow::slotUpdateStatusBar(const QString &statusm)
{
    //qDebug() << "MainWindow::slotUpdateStatusBar: " << statusm  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    statusBar()->showMessage(statusm, 2000);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


bool MainWindow::readCtyFile()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return false;
}

void MainWindow::slotDoubleClickLog(const int _qsoID)
{
               //qDebug() << "MainWindow::slotDoubleClickLog: QSOid: " << QString::number(_qsoID) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //int row = _qsoID.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    qsoToEdit(_qsoID);
    logEvent(Q_FUNC_INFO, "END", logSeverity);

    //TODO: To be added to the logWindow and create an action that emist the QSO id to be edited
}

bool MainWindow::setUDPServer(const bool _b)
{
    //qDebug() << Q_FUNC_INFO << ": upAndRunning: " << util->boolToQString (upAndRunning) << QT_ENDL;
    //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (_b) << QT_ENDL;
    QString errorMSG, aux;
    if (_b)
    {
        if (!UDPLogServer->isStarted())
        {
            if (!UDPLogServer->start())
            {
                errorMSG =  tr("start");
                aux = tr("UDP Server error\nThe UDP server failed to %1.", "start or stop").arg(errorMSG);
                showErrorDialog->setText(aux);
                showErrorDialog->exec();
                return false;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": UDP Log server started!" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
            }
            return true;
        }
        else
        {
            return true;
            //qDebug() << Q_FUNC_INFO << ": UDP Log server already started no need to restart!" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        }
    }
    else
    {
        //qDebug() << "qDebug() << Q_FUNC_INFO << ": UDPServerStart FALSE" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        if (UDPLogServer->isStarted())
        {
            if (!UDPLogServer->stop())
            {
                errorMSG =  tr("stop");
                aux = tr("UDP Server error\nThe UDP server failed to %1.", "start or stop").arg(errorMSG);
                showErrorDialog->setText(aux);
                showErrorDialog->exec();
                return UDPLogServer->isStarted ();
            }
            else
            {
                return false;
                //qDebug() << Q_FUNC_INFO << ": UDP Log server stopped!" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
            }
        }
        else
        {
            return false;
                   //qDebug() << Q_FUNC_INFO << ": UDP Log server already stopped no need to restop!" << QT_ENDL;
        }
    }
}

bool MainWindow::setHamlib(const bool _b)
{
    //qDebug() << Q_FUNC_INFO << ": upAndRunning: " << util->boolToQString (upAndRunning) << QT_ENDL;
    //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (_b) << QT_ENDL;
    if (!upAndRunning)
    {
        //qDebug() << Q_FUNC_INFO << ": Hamlib upAndRunning FALSE";
        return false;
    }
    if (_b)
    {
        //qDebug() << Q_FUNC_INFO << ": Hamlib active";
        hamlib->init(true);
        //qDebug() << Q_FUNC_INFO << ": After Hamlib active";
        return hamlib->readRadio(true); // Forcing the radio update
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": Hamlib NOT active";
        hamlib->stop();
        return false;
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    //qDebug() << Q_FUNC_INFO << QT_ENDL;
    setWindowSize(windowSize);
    setHamlib (hamlibActive);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    //qDebug() << "MainWindow::keyPressEvent: "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    switch (event->key()) {
    case Qt::Key_Return:
        // ENTER PRESSED
        slotQRZReturnPressed();
                //qDebug() << "MainWindow::keyPressEvent: RETURN"  << QT_ENDL;
        break;
    case Qt::Key_Enter:
        // ENTER PRESSED
                //qDebug() << "MainWindow::keyPressEvent: ENTER"  << QT_ENDL;
        slotQRZReturnPressed();
        break;
    case Qt::Key_F1:
            slotOpenWiki();
        break;
    case Qt::Key_F4:
            //qDebug << "MainWindow::keyPressEvent: F4"  << QT_ENDL;
            mainQSOEntryWidget->toggleRealTime ();
        break;

    default:
        //QFrame::keyPressEvent(event);
        break;
    }

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotOpenWiki()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << Q_FUNC_INFO << QT_ENDL;
    QDesktopServices::openUrl(QUrl("https://wiki.klog.xyz/"));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::readConfigData()
{
    //qDebug() << Q_FUNC_INFO << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (needToEnd)
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        //qDebug() << "MainWindow::readConfigData - END - 1" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        return;
    }
    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //qDebug() << Q_FUNC_INFO << ": File not found" << configFileName << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        if (configured)
        {
          //qDebug() << Q_FUNC_INFO << ": configured = true" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << ": configured = false" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        }
        //qDebug() << Q_FUNC_INFO << ": Calling openSetup" << QT_ENDL;
        openSetup(0);
        //qDebug() << Q_FUNC_INFO << ": After calling openSetup" << QT_ENDL;
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        //qDebug() << Q_FUNC_INFO << ": - END - 2" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        return;
    }
    hamlibActive = false;
    eQSLActive = false;
    clublogActive = false;
    lotwActive = false;
    deleteAlwaysAdiFile = false;

   //qDebug() << Q_FUNC_INFO << ": Before processConfigLine "  << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line;
        line.clear ();
        line = in.readLine();
        processConfigLine(line);
    }
    file.close ();


    //qDebug() << Q_FUNC_INFO << ": After processConfigLines "  << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    //defineStationCallsign(mainQRZ);

     //qDebug() << Q_FUNC_INFO << ":  " << defaultADIFLogFile << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    if ((useDefaultLogFileName) && (defaultADIFLogFile.length()>0))
    {
        useDefaultLogFileName = true;
    }
    else
    {
        useDefaultLogFileName = false;
    }
    //qDebug() << Q_FUNC_INFO << ": 01"  << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    infoWidget->setImperialSystem(imperialSystem);

    infoLabel2->setText(world->getEntityName(currentEntity));
    //qDebug() << "MainWindow::readConfigData-89"  << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    infoWidget->showEntityInfo(currentEntity);
    //qDebug() << Q_FUNC_INFO << ": 90"  << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    configured = true;
    searchWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    awards->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setDXClusterSpotConfig(dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY );
    setMainWindowTitle();
    dxClusterWidget->setMyQRZ(stationCallsign);
    //qDebug() << Q_FUNC_INFO << ": 97"  << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    checkIfNewBandOrMode();
    //qDebug() << Q_FUNC_INFO << ": 98"  << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    //initialContestModeConfiguration();
    //qDebug() << "MainWindow::readConfigData: 99" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

    /*
    if (upAndRunning)
    { // Next actions will not be executed in the first run
        slotClearButtonClicked();
    }
    */
    // I need to init the CLUBLOG
    if (clublogActive)
    {
        elogClublog->setCredentials(clublogEmail, clublogPass, stationCallsign);
    }
    else
    {
       //qDebug() << "MainWindow::readConfigData: NOT Setting ClublogCredentials" << QT_ENDL;
    }
    //qDebug() << Q_FUNC_INFO << ": QRZcom active????" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    if (qrzcomActive)
    {
        //qDebug() << "MainWindow::readConfigData: QRZcom active"<< QTime::currentTime().toString("hh:mm:ss")  << QT_ENDL;
        elogQRZcom->setCredentials(qrzcomUser, qrzcomPass);
        //qDebug() << "MainWindow::readConfigData: QRZcom credentials"<< QTime::currentTime().toString("hh:mm:ss")  << QT_ENDL;
        elogQRZcom->login();
        //qDebug() << "MainWindow::readConfigData: login" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
        //elogQRZcom->login();
        //qDebug() << "MainWindow::readConfigData: after login" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    }
    //qDebug() << "MainWindow::readConfigData: calling checkIfNewBandOrMode" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;

   //qDebug() << "MainWindow::readConfigData: 100" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    util->setVersion(softwareVersion);
            //qDebug() << "MainWindow::readConfigData: 101" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    searchWidget->setVersion(softwareVersion);
            //qDebug() << "MainWindow::readConfigData: 102" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    searchWidget->setCurrentLog(currentLog);
            //qDebug() << "MainWindow::readConfigData: 103" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    infoWidget->setCurrentLog(currentLog);
           //qDebug() << "MainWindow::readConfigData: 104" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    //searchWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    infoWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());

    satTabWidget->refreshData();
    adifLoTWExportWidget->setLogNumber (currentLog);
    QString aux;
    QString errorMSG;
    if (upAndRunning)
    {
        startServices();
    }
    //qDebug() << Q_FUNC_INFO << " - END" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::startServices()
{
    setWindowSize (windowSize);
    setHamlib(hamlibActive);
    setUDPServer(UDPServerStart);
}

bool MainWindow::processConfigLine(const QString &_line){
    //qDebug() << Q_FUNC_INFO << ": " << _line << QT_ENDL;

    logEvent(Q_FUNC_INFO, QString("Start: %1").arg(_line), logSeverity);
    int _logWithMoreQSOs = 0; // At the end, if the this variable is >0 the Selectedlog will have to be changed in the file.
    QString line = _line.simplified();
    //qDebug() << Q_FUNC_INFO << " - 2"  << QT_ENDL;
    if (!( (line.contains('=')) && (line.contains(';')))){
        //qDebug() << "MainWindow::processConfigLine: Wrong Line!" << QT_ENDL;
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - 5"  << QT_ENDL;
    if (line.startsWith('#'))
    {
        //qDebug() << "MainWindow::processConfigLine: notes Line!" << QT_ENDL;
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return true;
    }

    //qDebug() << Q_FUNC_INFO << " - 10 "  << QT_ENDL;
    QStringList values;
    values.clear();

    //values << line.split("=", QT_SKIP);
    values << line.split("=", QT_SKIP);
    //qDebug() << Q_FUNC_INFO << ": value length: " << QString::number(values.length ()) << QT_ENDL;
    if (values.length ()!=2)
    {
        //qDebug() << Q_FUNC_INFO << ": Returning false, wrong lenght!" << QT_ENDL;
        return false;
    }

    //qDebug() << Q_FUNC_INFO << ": value.at(0): " << values.at(0) << QT_ENDL;
    //qDebug() << Q_FUNC_INFO << ": value.at(1): " << values.at(1) << QT_ENDL;
    //qDebug() << Q_FUNC_INFO << " - 11 "  << QT_ENDL;

    //qDebug() << Q_FUNC_INFO << " - 20 "  << QT_ENDL;

    //qDebug() << Q_FUNC_INFO << " - 25"  << QT_ENDL;
    QString field = (values.at(0)).toUpper();
    //qDebug() << Q_FUNC_INFO << " - 26"  << QT_ENDL;
    QString value = values.at(1);
    //qDebug() << Q_FUNC_INFO << " - 28"  << QT_ENDL;
    int endValue = value.indexOf(';');
    if (endValue>-1){
        value = value.left(value.length() - (value.length() - endValue));
    }
    //qDebug() << Q_FUNC_INFO << " - 30 "  << QT_ENDL;
    QString aux;
    if (field == "CALLSIGN"){
        //qDebug() << Q_FUNC_INFO << " - 30.1 - Callsign: " << value << QT_ENDL;
        if (util->isValidCall(value))
        {
            mainQRZ = value;
        }
    }else if (field=="CQZ"){
        my_CQz = value.toInt();
    }else if (field=="ITUZ"){
        my_ITUz = value.toInt();
    }else if (field=="CONTEST"){
                    //qDebug() << "MainWindow::processConfigLine: CONTEST: " << QT_ENDL;
        contestMode = value;
    }else if (field=="MODES"){
        readActiveModes(value.split(", ", QT_SKIP));
    }else if (field=="BANDS"){
        //qDebug() << "MainWindow::processConfigLine: BANDS: " << value << QT_ENDL;
        readActiveBands(value.split(", ", QT_SKIP));
    }else if (field=="REALTIME"){
        //qDebug << "MainWindow::processConfigLine: REALTIME: " << value.toUpper() << QT_ENDL;
        mainQSOEntryWidget->setRealTime(util->trueOrFalse(value));
        //realTime = util->trueOrFalse(value);
    }else if (field=="LOGVIEWFIELDS"){
        //qDebug() << "MainWindow::processConfigLine: LOGVIEWFIELDS: " << value.toUpper() << QT_ENDL;
        logWindow->setColumns(value.split(",", QT_SKIP));
    }else if (field =="DXCLUSTERSERVERTOUSE"){
        aux = value;  //dxfun.com:8000
        if (aux.contains(':'))
        {
            dxclusterServerToConnect = (aux.split(':', QT_SKIP)).at(0);
            dxclusterServerPort = ((aux.split(':', QT_SKIP)).at(1)).toInt();
        }

        if ((dxclusterServerToConnect.length()< 3) || (dxclusterServerPort <= 0))
        {
            dxclusterServerToConnect = "dxfun.com";
            dxclusterServerPort = 8000;
        }
        dxClusterWidget->setDXClusterServer(dxclusterServerToConnect, dxclusterServerPort);
    }

    else if(field=="POWER")
    {
        if (value.toFloat()>0.0f)
        {
            myPower = value.toDouble();
            myDataTabWidget->setSetupMyPower(myPower);
        }
    }
    else if (field=="USEDEFAULTNAME")
    {
         useDefaultLogFileName = util->trueOrFalse(value);
    }

    else if (field=="IMPERIALSYSTEM")
    {
        imperialSystem = util->trueOrFalse(value);
    }
    else if (field=="SENDQSLWHENREC")
    {
        sendQSLWhenRec = util->trueOrFalse(value);
    }
    else if (field=="MANAGEDXMARATHON")
    {
                //qDebug() << "MainWindow::processConfigLine: Marathon-1 - Value: " << value << QT_ENDL;
        manageDxMarathon = util->trueOrFalse(value);
                //qDebug() << "MainWindow::processConfigLine: Marathon-2" << QT_ENDL;
        awardsWidget->setManageDXMarathon(manageDxMarathon);
    }

    else if (field=="SHOWCALLSIGNINSEARCH")
    {
        searchWidget->setShowCallInSearch(util->trueOrFalse(value));
        //stationCallSignShownInSearch = util->trueOrFalse(value);
    }

    else if (field=="CHECKNEWVERSIONS"){
        checkNewVersions = util->trueOrFalse(value);
    }

    else if (field=="PROVIDEINFO"){
        reportInfo = util->trueOrFalse(value);
    }

    else if (field=="ALWAYSADIF")
    {
        alwaysADIF = util->trueOrFalse(value);
    }
    else if (field=="DEBUGLOG")
    {
        if (util->trueOrFalse(value))
        {
            logSeverity = Info;
            logEvent(Q_FUNC_INFO, "Log enabled");
        }
        else
        {
            logEvent(Q_FUNC_INFO, "Log disabled");
            logSeverity = Info;
        }
    }
    else if (field=="UTCTIME")
    {
                    //qDebug() << "MainWindow::processConfigLine: UTCTIME: " << value.toUpper() <<endl;
        //UTCTime = util->trueOrFalse(value);
        mainQSOEntryWidget->setUTC(util->trueOrFalse(value));
    }

    else if (field=="SENDEQSLBYDEFAULT")
    {
        sendQSLByDefault=util->trueOrFalse(value);
        eQSLTabWidget->setQueueSentByDefault(sendQSLByDefault);
    }
    else if (field=="DUPLICATEDQSOSLOT"){
        if (value.toInt()>=0)
        {
            dupeSlotInSeconds = value.toInt();
            filemanager->setDuplicatedQSOSlot(dupeSlotInSeconds);
            mainQSOEntryWidget->setDuplicatedQSOSlot(dupeSlotInSeconds);
        }
    }

    else if (field=="COMPLETEWITHPREVIOUS")
    {
        completeWithPrevious  = util->trueOrFalse(value);
    }
    else if (field=="DXCLUSTERSAVE")
    {
        dxClusterWidget->setSaveSpots(util->trueOrFalse(value));
    }
    else if (field=="DXCLUSTERSHOWHF")
    {
        dxClusterShowHF  = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWVHF")
    {
        dxClusterShowVHF = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWWARC")
    {
        dxClusterShowWARC  = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWWORKED")
    {
        dxClusterShowWorked = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWCONFIRMED")
    {
        dxClusterShowConfirmed = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWANN")
    {
        dxClusterShowAnn = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWWWV")
    {
        dxClusterShowWWV = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWWCY")
    {
        dxClusterShowWCY = util->trueOrFalse(value);
    }

    else if (field=="DEFAULTADIFFILE")
    {
        defaultADIFLogFile = value.toLower();
                    //qDebug() << "MainWindow::processConfigLine: " << defaultADIFLogFile << QT_ENDL;
    }
    else if (field=="STATIONLOCATOR")
    {
        if ( locator->isValidLocator(value) )
        {
            //myLocator = ;
            myDataTabWidget->setMyLocator(value.toUpper());
        }
    }
    else if(field=="NEWONECOLOR")
    {
        newOneColor.setNamedColor(value);
    }
    else if(field=="NEEDEDCOLOR")
    {
        neededColor.setNamedColor(value);
    }
    else if(field=="WORKEDCOLOR")
    {
        workedColor.setNamedColor(value);
    }
    else if(field=="CONFIRMEDCOLOR")
    {
        confirmedColor.setNamedColor(value);
    }
    else if(field=="DEFAULTCOLOR")
    {
        defaultColor.setNamedColor(value);
    }
    //else if (field=="PSTROTATORACTIVE")
    //{
        //usePSTRotator = true;
    //}
    //else if (field=="PSTROTATORPORT")
    //{
    //    pstRotator->setPort(value.toInt());
    //}
    //else if (field=="PSTROTATORSERVER")
    //{
    //    pstRotator->setServer(value);
    //}
    else if (field=="UDPSERVER")
    {
                 //qDebug() << "MainWindow::processConfigLine: UDPSERVER: " << value.toUpper()  << QT_ENDL;
        UDPServerStart = util->trueOrFalse(value);
    }
    else if (field=="UDPNETWORKINTERFACE")
    {
        UDPLogServer->setNetworkInterface(value);
    }
    else if (field=="UDPSERVERPORT")
    {
        UDPLogServer->setPort(value.toInt());
    }
    else if (field=="INFOTIMEOUT")
    {
        int a = value.toInt();
        if ((a>0) && (a<=65535))
        {
            infoTimeout = a;
        }
    }
    else if (field=="LOGFROMWSJTX")
    {
        UDPLogServer->setLogging(util->trueOrFalse(value));
    }
    else if (field=="LOGAUTOFROMWSJTX")
    {
        wsjtxAutoLog = util->trueOrFalse(value);
    }
    else if (field == "HAMLIBRIGTYPE" )
    {
                //qDebug() << "MainWindow::processConfigLine: HAMLIBRIGTYPE: " << value << QT_ENDL;
        hamlib->setModelId(value.toInt());
                //qDebug() << "MainWindow::processConfigLine: HAMLIBRIGTYPE: " << value << QT_ENDL;
    }
    else if(field == "HAMLIBSERIALPORT")
    {
        //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALPORT: " << value << QT_ENDL;
        hamlib->setPort(value);
        //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALPORT: " << value << QT_ENDL;
    }
    else if (field == "HAMLIBSERIALBAUDS")
    {
                //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALBAUDS: " << value << QT_ENDL;
        hamlib->setSpeed(value.toInt());
                //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALBAUDS: " << value << QT_ENDL;
    }else if(field =="HAMLIBSERIALDATABITS"){
        //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDATABITS: " << value << QT_ENDL;
        hamlib->setDataBits(value.toInt());
        //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDATABITS: " << value << QT_ENDL;
    }else if(field =="HAMLIBSERIALSTOPBITS"){
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALSTOPBITS: " << value << QT_ENDL;
        hamlib->setStop(value.toInt ());
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALSTOPBITS: " << value << QT_ENDL;
    }else if(field =="HAMLIBSERIALFLOWCONTROL"){
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALFLOWCONTROL: " << value << QT_ENDL;
        hamlib->setFlow(value);
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALFLOWCONTROL: " << value << QT_ENDL;
    }else if(field =="HAMLIBSERIALPARITY"){
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALPARITY: " << value << QT_ENDL;
        hamlib->setParity(value);
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALPARITY: " << value << QT_ENDL;
    }else if(field =="HAMLIBSERIALRTS"){
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALRTS: " << value << QT_ENDL;
        //hamlib->setRTS(value);
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALRTS: " << value << QT_ENDL;
    }else if(field =="HAMLIBSERIALDTR"){
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDTR: " << value << QT_ENDL;
        //hamlib->setDTR(value);
             //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDTR: " << value << QT_ENDL;
    }else if (field == "HAMLIBRIGPOLLRATE"){
        hamlib->setPoll(value.toInt());
    }else if (field == "HAMLIB")
    {
        //qDebug() << "MainWindow::processConfigLine: HAMLIB: " << value << QT_ENDL;
        hamlibActive = util->trueOrFalse(value);
        //qDebug() << "MainWindow::processConfigLine: HAMLIB: " << value << QT_ENDL;
    }
    else if (field == "HAMLIBREADONLY")
    {
                //qDebug() << "MainWindow::processConfigLine: HAMLIBREADONLY: " << value << QT_ENDL;
        hamlib->setReadOnly(util->trueOrFalse(value));
    }
    else if (field == "HAMLIBNETADDRESS"){
        //hamlibPage->setRadioNetworkAddress (value);
        hamlib->setNetworkAddress (value);
    }
    else if (field == "HAMLIBNETPORT"){
        hamlib->setNetworkPort (value.toInt ());
        //hamlibPage->setRadioNetworkPort (value.toInt ());
    }
    else if (field=="REALTIMEFROMWSJTX")
    {
                 //qDebug() << "MainWindow::processConfigLine: REALTIMEFROMWSJTX: " << value << QT_ENDL;
        UDPLogServer->setRealTimeUpdate(util->trueOrFalse(value));
    }
    else if(field=="SELECTEDLOG")
    {
        currentLog = value.toInt();
                  //qDebug() << "MainWindow::processConfigLine: currentLog - SelectedLog: " << QString::number(currentLog) << QT_ENDL;

        //if ( ((dataProxy->doesThisLogExist(currentLog))  && (dataProxy->getHowManyQSOInLog(currentLog) > 0)) )
        if ( ((dataProxy->doesThisLogExist(currentLog)) ) )
        {
            //qDebug() << "MainWindow::processConfigLine: currentLog - Log with QSO - SelectedLog: " << QString::number(currentLog) << QT_ENDL;
        }
        else
        {
            int _howManyQSOMax = -1;     // NUmber of QSO of the log with more QSO
            int _howManyQSOMaxT = 0;    // Number of QSO in ine specific log
            QStringList logs = QStringList();

            logs << dataProxy->getListOfManagedLogs();
            //qDebug() << "MainWindow::processConfigLine: logs: " << QString::number(logs.size()) << QT_ENDL;
            for (int i = 0;i<logs.length();i++)
            {
                _howManyQSOMaxT = dataProxy->getHowManyQSOInLog(i);
                //qDebug() << "MainWindow::processConfigLine: SelectedLog-x: " << QString::number(i) << " - QSOs: " << QString::number(_howManyQSOMaxT) << QT_ENDL;
                if (_howManyQSOMax < _howManyQSOMaxT)
                {
                 //qDebug() << "MainWindow::processConfigLine: Found log with more QSO: " << logs.at(i) << QT_ENDL;
                    _howManyQSOMax = _howManyQSOMaxT;
                   _logWithMoreQSOs = (logs.at(i)).toInt();
                }
            }
            if (_logWithMoreQSOs>0)
            {
                currentLog = _logWithMoreQSOs;
                filemanager->modifySetupFile(configFileName, "SelectedLog", QString::number(currentLog));
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
                 //qDebug() << "MainWindow::processConfigLine: currentLog: " << value << QT_ENDL;
    }
    else if(field=="CLUBLOGACTIVE")
    {
                  //qDebug() << "MainWindow::processConfigLine: clublogActive: " << value << QT_ENDL;
        clublogActive = util->trueOrFalse(value);
        setupDialog->setClubLogActive(clublogActive);
    }
    else if(field=="CLUBLOGREALTIME")
    {
        //qDebug() << "MainWindow::processConfigLine: clublogRealTime: " << value << QT_ENDL;
        clublogRealTime = util->trueOrFalse(value);
    }
    else if(field=="CLUBLOGPASS")
    {
         //qDebug() << "MainWindow::processConfigLine: clublogPass: " << value << QT_ENDL;
        clublogPass = value;
    }
    else if(field=="CLUBLOGEMAIL")
    {
         //qDebug() << "MainWindow::processConfigLine: clublogEmail: " << value << QT_ENDL;
        clublogEmail = value;
    }
    else if(field=="QRZCOMACTIVE")
    {
        qrzcomActive = util->trueOrFalse(value);
        setupDialog->setQRZCOMAutoCheckActive(QRZCOMAutoCheckAct->isChecked());
        //slotElogQRZCOMAutoCheck();
    }
    else if(field=="QRZCOMSUBSCRIBER")
    {
        qrzcomSubscriber = util->trueOrFalse(value);
        elogQRZcom->setSubcription (util->trueOrFalse(value));
    }
    else if(field =="QRZCOMAUTO")
    {
         //qDebug() << "MainWindow::processConfigLine: QRZCOMAuto: " << value << QT_ENDL;
         //qDebug() << "MainWindow::processConfigLine: QRZCOMAuto was: " << util->boolToQString(QRZCOMAutoCheckAct->isChecked()) << QT_ENDL;

        QRZCOMAutoCheckAct->setChecked(util->trueOrFalse(value));
        setupDialog->setQRZCOMAutoCheckActive(util->trueOrFalse(value));
         //qDebug() << "MainWindow::processConfigLine: QRZCOMAuto is: " << util->boolToQString(QRZCOMAutoCheckAct->isChecked()) << QT_ENDL;
    }
    else if(field=="QRZCOMPASS")
    {
        qrzcomPass = value;
    }
    else if(field=="QRZCOMUSER")
    {
        qrzcomUser = value;
    }
    else if (field =="QRZCOMLOGBOOKKEY"){
        elogQRZcom->setLogBookKey(value);
    }
    else if(field =="EQSLACTIVE"){
        eQSLActive = util->trueOrFalse(value);
        setupDialog->setEQSLActive(eQSLActive);
         //qDebug() << "MainWindow::processConfigLine - EQSLACTIVE" << QT_ENDL;
    }
    else if(field =="EQSLREALTIME"){
        eQSLRealTime = util->trueOrFalse(value);

         //qDebug() << "MainWindow::processConfigLine - EQSLREALTIME" << QT_ENDL;
    }
    else if(field =="EQSLCALL"){
         //qDebug() << "MainWindow::processConfigLine - EQSLPASS" << QT_ENDL;
        eqslUtilities->setUser(value);
    }
    else if(field =="EQSLPASS"){
         //qDebug() << "MainWindow::processConfigLine - EQSLPASS" << QT_ENDL;
        eqslUtilities->setPass(value);
    }
    else if(field =="EQSLUSESTATIONCALLSIGN"){
         //qDebug() << "MainWindow::processConfigLine - EQSLUSESTATIONCALLSIGN" << QT_ENDL;
        eQSLUseQSOStationCallSign = util->trueOrFalse(value);
    }
    else if(field =="LOTWACTIVE"){
                //qDebug() << "MainWindow::processConfigLine - LOTWACTIVE" << QT_ENDL;
        if (util->trueOrFalse(value))
        {
            lotwActive = true;
            lotwCallTQSL->setEnabled(true);
            lotwCallTQSL->setWhatsThis(tr("Sends the log to LoTW calling TQSL."));
        }
        else
        {
            lotwActive = false;
            lotwCallTQSL->setEnabled(false);
            lotwCallTQSL->setWhatsThis(tr("This function is disabled. Go to the Setup->LoTW tab to enable it."));
        }
                //qDebug() << "MainWindow::processConfigLine - LOTWACTIVE-END" << QT_ENDL;
    }
    else if(field =="LOTWPATH"){
          //qDebug() << "MainWindow::processConfigLine - LOTWPATH" << QT_ENDL;
        lotwTQSLpath = value;
    }
    else if(field =="LOTWUSER"){
          //qDebug() << "MainWindow::processConfigLine - LOTWUSER: -" << value <<"-" << QT_ENDL;
        lotwUtilities->setUser(value);
          //qDebug() << "MainWindow::processConfigLine - AFTER LOTWUSER" << QT_ENDL;
    }
    else if(field =="LOTWPASS"){
        lotwUtilities->setPass(value);
    }
    else if(field=="VERSION")
    {
        if (softwareVersion!=value)
        {
            itIsANewversion = true;
        }
    }
    else if(field=="MAINWINDOWSIZE")
    {
        QStringList values;
        values.clear();
        values << value.split("x");
        if ((values.at(0).toInt()>0) && (values.at(1).toInt()>0))
        {
            windowSize.setWidth(values.at(0).toInt());
            windowSize.setHeight(values.at(1).toInt());
            //setWindowSize(values.at(0).toInt(), values.at(1).toInt());
        }
    }
    else if(field=="DELETEALWAYSADIFILE")
    {
        deleteAlwaysAdiFile = util->trueOrFalse(value);
        //qDebug() << "Delete Aways Adif File = " << deleteAlwaysAdiFile <<endl;
    }
    else if (field == "CHECKVALIDCALLS")
    {
         g_callsignCheck = util->trueOrFalse (value);
        //util->setCallValidation (util->trueOrFalse (value));
    }
    //else if(field=="LATESTBACKUP")
    //{
    //}
    //else
    //{
      //qDebug() << "MainWindow::processConfigLine: NONE: " << QT_ENDL;
    //}

    // Lines are: Option = value;
    //qDebug() << Q_FUNC_INFO << "(" << field << "/" << value << ")" << " - END";
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return true;
}

void MainWindow::checkIfNewBandOrMode()
{//Checks the log to see if there is a QSO with a band/mode
//that is not currently selected as active
    //qDebug() << "MainWindow::checkIfNewBandOrMode - START " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QString currentBand = mainQSOEntryWidget->getBand();
    QString currentMode = mainQSOEntryWidget->getMode();
    setupDialog->checkIfNewBandOrMode(); // Update the Setup dialog with new bands or modes
    //qDebug() << "MainWindow::checkIfNewBandOrMode - 1 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    QStringList bandsInLog = dataProxy->getBandsInLog(currentLog);
    //qDebug() << "MainWindow::checkIfNewBandOrMode - 2 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    QStringList modesInLog = dataProxy->getModesInLog(currentLog);
    //qDebug() << "MainWindow::checkIfNewBandOrMode - 3 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    QStringList qsTemp;
    qsTemp.clear();
    //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.1 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    bands << bandsInLog;
    bands.removeDuplicates();
    //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.2 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    qsTemp << dataProxy->sortBandNamesBottonUp(bands);
    //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.3 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    bands.clear();
    bands = qsTemp;

    modes << modesInLog;
    modes.removeDuplicates();
    modes.sort();

    //qDebug() << "MainWindow::checkIfNewBandOrMode - bands -" << QString::number(bands.length()) << " - " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    mainQSOEntryWidget->setBands(bands);
    satTabWidget->addBands(bands);

    //qDebug() << "MainWindow::checkIfNewBandOrMode - modes -" << QString::number(modes.length()) << " - " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    mainQSOEntryWidget->setModes(modes);


    //qDebug() << "MainWindow::checkIfNewBandOrMode - setting bands" << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Setting bands", Debug);
    dxccStatusWidget->setBands(Q_FUNC_INFO, bands, true);

    //qDebug() << "MainWindow::checkIfNewBandOrMode - currentBand: " << currentBand << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    if (bands.contains(currentBand))
    {
        mainQSOEntryWidget->setBand(currentBand);
    }
    else
    {
        selectDefaultBand();
    }

    //qDebug() << "MainWindow::checkIfNewBandOrMode-99 " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
    if (modes.contains(currentMode))
    {
        mainQSOEntryWidget->setMode(currentMode);
    }
    else
    {
        selectDefaultMode();
    }


    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::checkIfNewBandOrMode END " << QTime::currentTime().toString("hh:mm:ss") << QT_ENDL;
}

void MainWindow::selectDefaultBand()
{
            //qDebug() << "MainWindow::selectDefaultBand" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END", logSeverity);

             //qDebug() << "MainWindow::selectDefaultBand_END" << QT_ENDL;
}

void MainWindow::selectDefaultMode()
{
            //qDebug() << "MainWindow::selectDefaultMode" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!upAndRunning)
    {
        return;
    }
    //int aux = 1;
    defaultMode = dataProxy->getMostUsedMode(currentLog);
            //qDebug() << "MainWindow::selectDefaultMode: " << QString::number(defaultMode) << QT_ENDL;

    if (defaultMode < 1)
    {
        defaultMode = dataProxy->getSubModeIdFromSubMode(mainQSOEntryWidget->getBand(0));
        //defaultMode = dataProxy->getSubModeIdFromSubMode((modeComboBox->itemText(0)));
                //qDebug() << "MainWindow::selectDefaultMode2: " << QString::number(defaultMode) << QT_ENDL;
                //qDebug() << "MainWindow::selectDefaultMode2S: "  << QT_ENDL;
    }

    //aux = dataProxy->getNameFromSubModeId(defaultMode);

    mainQSOEntryWidget->setMode(dataProxy->getNameFromSubModeId(defaultMode));
    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode)));

           //qDebug() << "MainWindow::selectDefaultMode3: " << QString::number(defaultMode) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
           //qDebug() << "MainWindow::selectDefaultMode-END" << QT_ENDL;
}


void MainWindow::readActiveBands (const QStringList actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
    //qDebug() << "MainWindow::readActiveBands: " << actives << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    for (int i=0;i<actives.length();i++)
    {
                 //qDebug() << "MainWindow::readActiveBands: actives: " << actives.at(i) << QT_ENDL;
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
        //if (db->isValidMode(actives.at(i), false))
        {
            if (!atLeastOne)
            {
                atLeastOne = true;
                //bands.clear();
            }
            aux = __bands.at(i);
            if (aux.length()>0)
            {
               bands << aux;
            }
           // bands << actives.at(i);
        }
    }
    bands.removeDuplicates();
    for (int i=0;i<bands.length();i++)
    {
                 //qDebug() << "MainWindow::readActiveBands: bands: " << bands.at(i) << QT_ENDL;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::readActiveBands - END" << QT_ENDL;
}

void MainWindow::readActiveModes (const QStringList actives)
{
             //qDebug() << "MainWindow::readActiveModes: " << actives << QT_ENDL;
            //qDebug() << "MainWindow::readActiveModes: " << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //bool atLeastOne = false;
    QString aux;
    aux.clear();

    QStringList __modes;
    __modes.clear();
    __modes = actives;
    __modes << dataProxy->getModesInLog(currentLog);
    __modes.removeDuplicates();
    modes.clear();
    //QStringList values = actives.split(", ", QT_SKIP);

    for (int i = 0; i < __modes.size() ; i++)
    {
                 //qDebug() << "MainWindow::readActiveModes: checking: " << __modes.at(i) << QT_ENDL;
        if (dataProxy->getIdFromModeName(__modes.at(i)) > 0)
        {
                     //qDebug() << "MainWindow::readActiveModes: checking-exist: " << __modes.at(i) << QT_ENDL;
            //if (!atLeastOne)
            //{
            //    atLeastOne = true;

            //}
            aux = __modes.at(i);

            if (aux.length()>0)
            {
                        //qDebug() << "MainWindow::readActiveModes: adding: " << aux << QT_ENDL;
               modes << aux;
            }

           // modes << actives.at(i);
        }
    }
    modes.removeDuplicates();
    modes.sort();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
            //qDebug() << "MainWindow::readActiveModes - END" << QT_ENDL;
}

void MainWindow::createData()
{
            //qDebug() << "MainWindow::createData " << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::createUIDX()
{
    //        //qDebug() << "MainWindow::createUIDX << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    //operatorLineEdit->setToolTip(tr("Logging operator's callsign."));
    //stationCallSignLineEdit->setToolTip(tr("Callsign used over the air."));

    infoLabel1->setToolTip(tr("Status of the DX entity."));
            //qDebug() << "MainWindow::createUIDX-13" << QT_ENDL;
    infoLabel2->setToolTip(tr("Name of the DX entity."));
            //qDebug() << "MainWindow::createUIDX-14" << QT_ENDL;

    dxUpLeftInputFrame = new QFrame;
    dxUpLeftTab = new QTabWidget;
    dxUpRightTab = new QTabWidget;
    dxBottonTab = new QTabWidget;

    dxUpLeftTab->setTabPosition(QTabWidget::North);
    dxUpRightTab->setTabPosition(QTabWidget::South);
    //qDebug() << "MainWindow::createUIDX-30" << QT_ENDL;

    QHBoxLayout *dxUpLeftInputFrameLayout = new QHBoxLayout;
    dxUpLeftInputFrameLayout->addWidget (mainQSOEntryWidget);
    //dxUpLeftInputFrameLayout->setSizeConstraint(QLayout::SetFixedSize);
    dxUpLeftInputFrame->setLayout (dxUpLeftInputFrameLayout);


    dxUpLeftTab->addTab (QSOTabWidget, tr("QSO"));
    dxUpLeftTab->addTab(QSLTabWidget, tr("QSL"));
    dxUpLeftTab->addTab(eQSLTabWidget, tr("eQSL"));
    dxUpLeftTab->addTab(commentTabWidget, tr("Comment"));

    othersTabWidget->setEntitiesList(world->getEntitiesNames());
    dxUpLeftTab->addTab(othersTabWidget, tr("Others"));

    dxUpLeftTab->addTab(myDataTabWidget, tr("My Data"));
    dxUpLeftTab->addTab(satTabWidget, tr("Satellite"));


    //qDebug() << "MainWindow::createUIDX-90" << QT_ENDL;
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
    //qDebug() << "MainWindow::createUIDX-100" << QT_ENDL;

    reconfigureDXMarathonUI(manageDxMarathon);

    //qDebug() << "MainWindow::createUIDX-122" << QT_ENDL;
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

    //qDebug() << "MainWindow::createUIDX - OS DETECTION"  << QT_ENDL;

#ifdef Q_OS_WIN
    //qDebug() << "MainWindow::createUIDX - WINDOWS DETECTED!"  << QT_ENDL;

    dxUpLeftInputFrame->setFrameShadow(QFrame::Raised);
    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel);

#else
   //qDebug() << "MainWindow::createUIDX - NO WINDOWS DETECTED!"  << QT_ENDL;

    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
#endif
    logEvent(Q_FUNC_INFO, "END", logSeverity);
            //qDebug() << "MainWindow::createUIDX-END" << QT_ENDL;
 }


void MainWindow::slotADIFExport()
{
      //qDebug() << "MainWindow::slotADIFExport - Start" << QT_ENDL;

   // QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    adifLoTWExportWidget->setExportMode(ModeADIF);
    adifLoTWExportWidget->show();

    //filemanager->adifLogExport(fileName, currentLog);
      //qDebug() << "MainWindow::slotADIFExport -END " << QT_ENDL;
}

void MainWindow::showNumberOfSavedQSO(const QString &_fn, const int _n)
{
      //qDebug() << "MainWindow::showNumberOfSavedQSO: " << _fn << "/" << QString::number(_n) << QT_ENDL;
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
      //qDebug() << "MainWindow::showNumberOfSavedQSO - END" << QT_ENDL;
}

void MainWindow::fileExportADIF(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
      //qDebug() << "MainWindow::fileExportADIF " << _st << QT_ENDL;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeADIF);

    showNumberOfSavedQSO(fileName, qsos.count());

      //qDebug() << "MainWindow::fileExportADIF - END" << QT_ENDL;
}

void MainWindow::slotADIFExportAll()
{
      //qDebug() << "MainWindow::slotADIFExportAll " << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString _callToUse = "ALL";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
      //qDebug() << "MainWindow::slotADIFExportAll: " << fileName << QT_ENDL;
    if (fileName.length()<1)
    {
        return;
    }
    if ((!fileName.endsWith(".adi")) && ( !fileName.endsWith(".adif") ))
    {
         //qDebug() << "MainWindow::slotADIFExportAll: Adding the .adi to the file" << fileName << QT_ENDL;
        fileName = fileName +  ".adi";
    }
     //qDebug() << "MainWindow::slotADIFExportAll-1: " << fileName << QT_ENDL;
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _callToUse, dataProxy->getFirstQSODateFromCall(_callToUse), dataProxy->getLastQSODateFromCall(_callToUse), -1, ModeADIF);
     //qDebug() << "MainWindow::slotADIFExportAll-3" << QT_ENDL;
    showNumberOfSavedQSO(fileName, qsos.count());

    //filemanager->adifLogExport(fileName, 0);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::fileExportLoTW(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
      //qDebug() << "MainWindow::fileExportLoTW  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << QT_ENDL;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
          //qDebug() << "MainWindow::fileExportLoTW - no valid call" << QT_ENDL;
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
          //qDebug() << "MainWindow::fileExportLoTW - no valid date" << QT_ENDL;
        return;
    }

    //QString fileName = "klog-lotw-upload.adi";
    QString fileName = util->getLoTWAdifFile();


    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeLotW);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
          //qDebug() << "MainWindow::fileExportLoTW NO QSOs" << QT_ENDL;
        return;
    }
      //qDebug() << "MainWindow::fileExportLoTW - 50" << QT_ENDL;
    bool uploadedToLoTW = callTQSL(fileName, _st);
    //bool uploadedToLoTW = true;
      //qDebug() << "MainWindow::fileExportLoTW - 51" << QT_ENDL;

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
      //qDebug() << "MainWindow::fileExportLoTW -END " << QT_ENDL;
    }else{
         QFile::remove(fileName);
    }
}

void MainWindow::fileExportClubLog(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
      //qDebug() << "MainWindow::fileExportClubLog  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << QT_ENDL;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
          //qDebug() << "MainWindow::fileExportClubLog - no valid call" << QT_ENDL;
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
          //qDebug() << "MainWindow::fileExportClubLog - no valid date" << QT_ENDL;
        return;
    }

    //QString fileName = "klog-clublog-upload.adi";
    QString fileName = util->getClubLogFile();

    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeClubLog);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
          //qDebug() << "MainWindow::fileExportClubLog NO QSOs" << QT_ENDL;
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
      //qDebug() << "MainWindow::fileExportClubLog - 50" << QT_ENDL;
    elogClublog->sendLogFile(fileName, qsos, overwrite);



      //qDebug() << "MainWindow::fileExportClubLog -END " << QT_ENDL;
}

void MainWindow::fileExportEQSL(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
     //qDebug() << "MainWindow::fileExportEQSL  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << QT_ENDL;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
          //qDebug() << "MainWindow::fileExportEQSL - no valid call" << QT_ENDL;
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
          //qDebug() << "MainWindow::fileExportEQSL - no valid date" << QT_ENDL;
        return;
    }

    //QString fileName = "klog-eqsl-upload.adi";
    QString fileName = util->getEQSLFile();

    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeEQSL);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
          //qDebug() << "MainWindow::fileExportEQSL NO QSOs" << QT_ENDL;
        return;
    }

    eqslUtilities->sendLogFile(fileName, qsos);

      //qDebug() << "MainWindow::fileExportEQSL -END " << QT_ENDL;
}

void MainWindow::slotADIFExportSelection(const QString &_st, const QDate &_startDate, const QDate &_endDate, const ExportMode _eM)
{
      //qDebug() << "MainWindow::slotADIFExportSelection  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << QT_ENDL;

    switch (_eM)
    {
    case ModeADIF:         // General ADIF
           //qDebug() << "MainWindow::slotADIFExportSelection  - ADIF" << QT_ENDL;
        fileExportADIF(_st, _startDate, _endDate);
        break;
    case ModeLotW:         // LoTW
          //qDebug() << "MainWindow::slotADIFExportSelection  - LoTW" << QT_ENDL;
        fileExportLoTW(_st, _startDate, _endDate);
        break;
    case ModeClubLog:         // General ADIF
          //qDebug() << "MainWindow::slotADIFExportSelection  - ClubLog" << QT_ENDL;
        fileExportClubLog(_st, _startDate, _endDate);
        break;
    case ModeEQSL:         // General eQSL
          //qDebug() << "MainWindow::slotADIFExportSelection  - eQSL" << QT_ENDL;
        fileExportEQSL(_st, _startDate, _endDate);
        break;
    case ModeQRZ:         // General eQSL
          //qDebug() << "MainWindow::slotADIFExportSelection  - QRZ.com" << QT_ENDL;
        elogQRZcom->sendQSOs(dataProxy->getQSOsListQRZCOMToSent(_st, _startDate, _endDate, true));
        break;
    }

      //qDebug() << "MainWindow::slotADIFExportSelection -END " << QT_ENDL;
}
void MainWindow::slotLoTWExport()
{
    // 1.- Selec call
    // 2.- Select file and export (fixed filename?)
    // 3.- Call tqsl with the filename
    // 4.- Ask for the user to remove or not the file
      //qDebug() << "MainWindow::slotLoTWExport - Start" << QT_ENDL;
   // bool emptyCall = false;
    if (!lotwActive)
    {
        showMessageToEnableTheOnlineService(LoTW);
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeLotW);
    adifLoTWExportWidget->show();
      //qDebug() << "MainWindow::slotLoTWExport- END" << QT_ENDL;
}

void MainWindow::slotLoTWDownload()
{
      //qDebug() << "MainWindow::slotDownUpload - Start" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QStringList calls;
    calls << dataProxy->getStationCallSignsFromLog(-1);

    bool ok;

    QString callToUse = QInputDialog::getItem(this, tr("KLog - Select the Station Callsign."),
                                         tr("Select the Station Callsign to use when quering LoTW:"), calls, 0, false, &ok);

    //qDebug() << "MainWindow::slotDownUpload: " << callToUse << QT_ENDL;
    if (ok && !callToUse.isEmpty())
    {
        lotwUtilities->setStationCallSign(callToUse);
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

    lotwUtilities->download();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "MainWindow::slotDownUpload - END" << QT_ENDL;
}

void MainWindow::slotLoTWFullDownload()
{
      //qDebug() << "MainWindow::slotLoTWFullDownload - Start" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QStringList calls;
    calls << dataProxy->getStationCallSignsFromLog(-1);

    bool ok;

    QString callToUse = QInputDialog::getItem(this, tr("KLog - Select the Station Callsign."),
                                         tr("Select the Station Callsign to use when quering LoTW:"), calls, 0, false, &ok);

    //qDebug() << "MainWindow::slotLoTWFullDownload: " << callToUse << QT_ENDL;
    if (ok && !callToUse.isEmpty())
    {
        lotwUtilities->setStationCallSign(callToUse);
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

    lotwUtilities->fullDownload();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "MainWindow::slotLoTWFullDownload - END" << QT_ENDL;
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
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!clublogActive)
    {
        showMessageToEnableTheOnlineService(ClubLog);
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeClubLog);
    adifLoTWExportWidget->show();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::sloteQSLLogUpload()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (!eQSLActive)
    {
        showMessageToEnableTheOnlineService(eQSL)  ;
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeEQSL);
    adifLoTWExportWidget->show();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotQRZCOMLogUpload()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << Q_FUNC_INFO << QT_ENDL;
    if (!qrzcomActive)
    {
        showMessageToEnableTheOnlineService(QRZ)  ;
        //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
        return;
    }

    if (!elogQRZcom->hasLogBookKey())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog - QRZ.com"));
        msgBox.setText(tr("You need to define a proper API Key for your QRZ.com logbook in the eLog preferences.") );
        msgBox.exec();
        //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
        return;
    }
    adifLoTWExportWidget->setExportMode(ModeQRZ);
    adifLoTWExportWidget->show();
    //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotRQSLExport()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               "ADIF (*.adi *.adif)");

    filemanager->adifReqQSLExport(fileName);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::slotADIFImport(){
               //qDebug() << "MainWindow::slotADIFImport " << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     util->getHomeDir(),
                                                     "ADIF (*.adi *.adif)");
    if (!fileName.isNull())
    {
        //qDebug() << "MainWindow::slotADIFImport -1" << QT_ENDL;
        filemanager->adifReadLog(fileName, currentLog);
        updateQSLRecAndSent();


                 //qDebug() << "MainWindow::slotADIFImport -2" << QT_ENDL;
        //dxccStatusWidget->refresh();
        logWindow->refresh();
                 //qDebug() << "MainWindow::slotADIFImport -3" << QT_ENDL;
        checkIfNewBandOrMode();
                 //qDebug() << "MainWindow::slotADIFImport -4" << QT_ENDL;

        if (contestMode == "DX")
        {
                     //qDebug() << "MainWindow::slotADIFImport-DX" << QT_ENDL;
            //operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            awardsWidget->fillOperatingYears();
                     //qDebug() << "MainWindow::slotADIFImport-DX-1" << QT_ENDL;
            slotShowAwards();
            awardsWidget->showAwards();
                     //qDebug() << "MainWindow::slotADIFImport-DX-1-end" << QT_ENDL;
        }
        //else if (contestMode == "CQ-WW-SSB")
        //{}
        else
        {
            awardsWidget->fillOperatingYears();
            //operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            slotShowAwards();
            awardsWidget->showAwards();
        }
                 //qDebug() << "MainWindow::slotADIFImport-7" << QT_ENDL;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
             //qDebug() << "MainWindow::slotADIFImport-END" << QT_ENDL;
}

void MainWindow::qsoToEdit (const int _qso)
{
    //qDebug() << "MainWindow::qsoToEdit: " << QString::number(_qso) << QT_ENDL;

    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
                //qDebug() << "MainWindow::qsoToEdit: " << stringQuery << QT_ENDL;



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


    // ADD THE DATA THAT IS PRESENT IN ALL THE MODES

        // ** Start of SAT data
        // ** BAND / MODE / Locator shoule be executed after SAT or may be removed

        nameCol = rec.indexOf("sat_name");
        aux1 = (query.value(nameCol)).toString();
        if (aux1.length()>0)
        {
            satTabWidget->setSatName(aux1);
        }
        else
        {
            satTabWidget->clear();
        }


        nameCol = rec.indexOf("sat_mode");
        aux1 = (query.value(nameCol)).toString();
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
    aux1 = (query.value(nameCol)).toString();
    mainQSOEntryWidget->setQRZ(aux1);

    QString currentQrz = aux1;
    currentEntity = world->getQRZARRLId(currentQrz);

               //qDebug() << "MainWindow::qsoToEdit - currentEntity " << QString::number(currentEntity) << QT_ENDL;

    nameCol = rec.indexOf("qso_date");
    aux1 = (query.value(nameCol)).toString();
      //qDebug() << "MainWindow::qsoToEdit - date: " << aux1 << QT_ENDL;
    mainQSOEntryWidget->setDateTime(util->getDateTimeFromSQLiteString(aux1));
    //mainQSOEntryWidget->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
    dateTimeTemp->setDate(util->getDateFromSQliteString(aux1));

    nameCol = rec.indexOf("bandid");
    aux1 = (query.value(nameCol)).toString();
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
        aux1 = (queryAux.value(0)).toString();
                 //qDebug() << "MainWindow::qsoToEdit - bandid-1 " << aux1 << QT_ENDL;
                 //qDebug() << "MainWindow::qsoToEdit: - Changing from: " << mainQSOEntryWidget->getBand() << QT_ENDL;
        mainQSOEntryWidget->setBand(aux1);
        //bandComboBox->setCurrentIndex(bandComboBox->findText(aux1, Qt::MatchCaseSensitive));
                 //qDebug() << "MainWindow::qsoToEdit: - Changing to: " << mainQSOEntryWidget->getBand() << QT_ENDL;
    }
    else
    {
                 //qDebug() << "MainWindow::qsoToEdit - bandid-NO "  << QT_ENDL;
        mainQSOEntryWidget->setBand(dataProxy->getNameFromBandId(defaultBand));
        //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
        //bandComboBox->setCurrentIndex(defaultBand);
    }

      //qDebug() << "MainWindow::qsoToEdit: Check mode " <<  QT_ENDL;
    nameCol = rec.indexOf("modeid");
    aux1 = (query.value(nameCol)).toString();
      //qDebug() << "MainWindow::qsoToEdit: (aux1)-1: " << aux1 << QT_ENDL;


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

     //qDebug() << "MainWindow::qsoToEdit: After ALL Mode actions" << QT_ENDL;

    nameCol = rec.indexOf("rst_sent");
    aux1 = (query.value(nameCol)).toString();
    QSOTabWidget->setRSTTX (aux1);
      //qDebug() << "MainWindow::qsoToEdit: - RST_SENT: " << aux1  << QT_ENDL;

    nameCol = rec.indexOf("rst_rcvd");
    aux1 = (query.value(nameCol)).toString();
    QSOTabWidget->setRSTRX (aux1);

                //qDebug() << "MainWindow::qsoToEdit: - before switch"  << QT_ENDL;

    if (contestMode == "DX")
    {
                    //qDebug() << "MainWindow::qsoToEdit: - in default"  << QT_ENDL;

        nameCol = rec.indexOf("qsl_via");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLVia(aux1);

        nameCol = rec.indexOf("qslmsg");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLMsg(aux1);

        //qslmsgTextEdit->setText(aux1);

        nameCol = rec.indexOf("comment");
        aux1 = (query.value(nameCol)).toString();
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
        aux1 = (query.value(nameCol)).toString();
                    //qDebug() << "MainWindow::qsoToEdit: - NAME: " << aux1  << QT_ENDL;

        if (aux1.length()>0)
        {
            QSOTabWidget->setName (aux1);
        }
        else
        {
            QSOTabWidget->clearName ();
        }

        nameCol = rec.indexOf("qth");
        aux1 = (query.value(nameCol)).toString();
        QSOTabWidget->setQTH (aux1);

        nameCol = rec.indexOf("gridsquare");
        aux1 = (query.value(nameCol)).toString();
          //qDebug() << "MainWindow::qsoToEdit: - GRIDSQUARE: " << aux1  << QT_ENDL;
        QSOTabWidget->setDXLocator(aux1);
        satTabWidget->setLocator(aux1);

        nameCol = rec.indexOf("operator");
        aux1 = (query.value(nameCol)).toString();
          //qDebug() << "MainWindow::qsoToEdit: - OPERATOR: " << aux1  << QT_ENDL;
        myDataTabWidget->setOperator(aux1);

        nameCol = rec.indexOf("station_callsign");
        aux1 = (query.value(nameCol)).toString();
          //qDebug() << "MainWindow::qsoToEdit: - STATIONQRZ: " << aux1  << QT_ENDL;
        myDataTabWidget->setStationCallsign(aux1);

        nameCol = rec.indexOf("my_gridsquare");
        aux1 = (query.value(nameCol)).toString();
        myDataTabWidget->setMyLocator(aux1);

        nameCol = rec.indexOf("my_vucc_grids");
        aux1 = (query.value(nameCol)).toString();
        myDataTabWidget->setMyVUCCGrids(aux1);

        nameCol = rec.indexOf("my_rig");
        aux1 = (query.value(nameCol)).toString();
        if (!aux1.isEmpty ())
        {
            myDataTabWidget->setMyRig (aux1);
        }

        nameCol = rec.indexOf("my_antenna");
        aux1 = (query.value(nameCol)).toString();
        if (!aux1.isEmpty ())
        {
            myDataTabWidget->setMyAntenna (aux1);
        }

        nameCol = rec.indexOf("my_sota_ref");
        aux1 = (query.value(nameCol)).toString();
        if (!aux1.isEmpty ())
        {
           myDataTabWidget->setMySOTA (aux1);
        }

        nameCol = rec.indexOf("tx_pwr");
        myDataTabWidget->setMyPower((query.value(nameCol)).toDouble());

        nameCol = rec.indexOf("rx_pwr");
        aux1 = (query.value(nameCol)).toString();
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
        aux1 = (query.value(nameCol)).toString();
        QSOTabWidget->setTXFreq (aux1.toDouble ());

        nameCol = rec.indexOf("freq_rx");
        aux1 = (query.value(nameCol)).toString();
        QSOTabWidget->setRXFreq (aux1.toDouble ());

        //QSL SENT

        nameCol = rec.indexOf("qsl_sent");
        aux1 = (query.value(nameCol)).toString();

        QSLTabWidget->setQSLSenStatus(aux1);
        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("qslsdate");
        aux1 = (query.value(nameCol)).toString();

        if (util->getDateFromSQliteString(aux1).isValid()  )

        //if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            QSLTabWidget->setQSLSenDate(util->getDateFromSQliteString(aux1));
        }


        nameCol = rec.indexOf("qsl_sent_via");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLSenVia(aux1);


        //QSL RECEPTION

        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
        // tr("B-Bureau") << tr("D-Direct") << tr("E-Electronic") << tr("M-Manager");
        //QSLRDATE: (only valid if QSL_RCVD is Y, I, or V)


        nameCol = rec.indexOf("qsl_rcvd");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLRecStatus(aux1);
        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("qslrdate");
        aux1 = (query.value(nameCol)).toString();
        if (util->getDateFromSQliteString(aux1).isValid()  )
        {
            QSLTabWidget->setQSLRecDate(util->getDateFromSQliteString(aux1));
        }

         nameCol = rec.indexOf("qsl_rcvd_via");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLRecVia(aux1);

     //TODO: BUG: When something is selected while modifying the QSL is deleted???

        //CLUBLOG
        nameCol = rec.indexOf("clublog_qso_upload_status");
        aux1 = (query.value(nameCol)).toString();
        eQSLTabWidget->setClubLogStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("clublog_qso_upload_date");
        aux1 = (query.value(nameCol)).toString();
        if (util->getDateFromSQliteString(aux1).isValid()  )
        {
            eQSLTabWidget->setClubLogDate(util->getDateFromSQliteString(aux1));
        }

        //CLUBLOG


        //EQSL_QSL_SENT: {Y, N, R, Q, I}
        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
       //EQSL_QSLSDATE (only valid if EQSL_SENT is Y, Q, or I)

        nameCol = rec.indexOf("eqsl_qsl_sent");
        aux1 = (query.value(nameCol)).toString();
        eQSLTabWidget->setEQSLSenStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("eqsl_qslsdate");
        aux1 = (query.value(nameCol)).toString();
        if (util->getDateFromSQliteString(aux1).isValid()  )
        {
            eQSLTabWidget->setEQSLSenDate(util->getDateFromSQliteString(aux1));
        }


        //E-QSL RECEPTION

            // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
            // EQSL_QSL_RCVD: {Y, N, R, I, V}
            // EQSL_QSLRDATE: (only valid if EQSL_RCVD is Y, I, or V)


            nameCol = rec.indexOf("eqsl_qsl_rcvd");
            aux1 = (query.value(nameCol)).toString();
            eQSLTabWidget->setEQSLRecStatus(aux1.toUpper());

            //TODO: Depending on the Value a date should or not exist.
            //      This code may be importing dates when they should not exist.
            nameCol = rec.indexOf("eqsl_qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (util->isValidDateFromString(aux1))
            {
                eQSLTabWidget->setEQSLRecDate(util->getDateFromSQliteString(aux1));
            }

            //LOTW_QSL_SENT: {Y, N, R, Q, I}
            // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
           //LOTW_QSLSDATE (only valid if LOTW_SENT is Y, Q, or I)

            nameCol = rec.indexOf("lotw_qsl_sent");
            aux1 = (query.value(nameCol)).toString();
            eQSLTabWidget->setLOTWSenStatus(aux1.toUpper());
                     //qDebug() << "MainWindow::qsoToEdit: - LoTW Sent Status: " << aux1  << QT_ENDL;

            //TODO: Depending on the Value a date should or not exist.
            //      This code may be importing dates when they should not exist.
            nameCol = rec.indexOf("lotw_qslsdate");
            aux1 = (query.value(nameCol)).toString();

            if ( util->isValidDateFromString(aux1) )
            {
                eQSLTabWidget->setLOTWSenDate(util->getDateFromSQliteString(aux1));
            }

            //E-QSL RECEPTION

                // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
                // lotw_QSL_RCVD: {Y, N, R, I, V}
                // lotw_QSLRDATE: (only valid if lotw_RCVD is Y, I, or V)


                nameCol = rec.indexOf("lotw_qsl_rcvd");
                aux1 = (query.value(nameCol)).toString();
                eQSLTabWidget->setLOTWRecStatus(aux1.toUpper());

                //TODO: Depending on the Value a date should or not exist.
                //      This code may be importing dates when they should not exist.
                nameCol = rec.indexOf("lotw_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if ( util->isValidDateFromString(aux1) )
                {
                    eQSLTabWidget->setLOTWRecDate(util->getDateFromSQliteString(aux1));
                }

                //QRZCOM
                nameCol = rec.indexOf("qrzcom_qso_upload_status");
                aux1 = (query.value(nameCol)).toString();
                eQSLTabWidget->setQRZCOMStatus(aux1.toUpper());

                //TODO: Depending on the Value a date should or not exist.
                //      This code may be importing dates when they should not exist.
                nameCol = rec.indexOf("qrzcom_qso_upload_date");
                aux1 = (query.value(nameCol)).toString();
                if (util->getDateFromSQliteString(aux1).isValid()  )
                {
                    eQSLTabWidget->setQRZCOMDate(util->getDateFromSQliteString(aux1));
                }

                //QRZCOM

                // OTHERS TAB

                nameCol = rec.indexOf("sota_ref");
                aux1 = (query.value(nameCol)).toString();
                if (!aux1.isEmpty ())
                {
                    othersTabWidget->setSOTA (aux1);
                }
                nameCol = rec.indexOf("age");
                aux1 = (query.value(nameCol)).toString();
                if (aux1.toDouble ()>0)
                {
                    othersTabWidget->setAge (aux1.toDouble ());
                }

                nameCol = rec.indexOf("vucc_grids");
                aux1 = (query.value(nameCol)).toString();
                //qDebug() << Q_FUNC_INFO << ": VUCC_GRIDS: " << aux1;
                if (util->isValidVUCCGrids (aux1))
                {
                    othersTabWidget->setVUCCGrids (aux1);
                }
             //qDebug() << "MainWindow::qsoToEdit: - just before IOTA"  << QT_ENDL;

                nameCol = rec.indexOf("iota");
                aux1 = (query.value(nameCol)).toString();

                aux1 = awards->checkIfValidIOTA(aux1);
                othersTabWidget->setIOTA(aux1);

                           //qDebug() << "MainWindow::qsoToEdit: - in default - 100: " << QString::number(currentEntity)  << QT_ENDL;

                nameCol = rec.indexOf("dxcc");
                aux1  = (query.value(nameCol)).toString();

                           //qDebug() << "MainWindow::qsoToEdit: Checking DXCC: " << aux1 << " - " << world->getEntityName(aux1.toInt()) << QT_ENDL;

                if (aux1.toInt()>=1)
                {
                    if (aux1.toInt() != util->getNormalizedDXCCValue(currentEntity))
                    {
                        currentEntity = aux1.toInt();
                    }
                               //qDebug() << "MainWindow::qsoToEdit: - in default - 101: " << QString::number(currentEntity)  << QT_ENDL;
                }
                else
                {
                    currentEntity = world->getQRZARRLId(currentQrz);
                               //qDebug() << "MainWindow::qsoToEdit: - in default - 103: " << QString::number(currentEntity)  << QT_ENDL;
                }
                           //qDebug() << "MainWindow::qsoToEdit: - in default - 104: " << QString::number(currentEntity)  << QT_ENDL;

                nameCol = rec.indexOf("prop_mode");
                aux1  = (query.value(nameCol)).toString();
                othersTabWidget->setPropMode(aux1, false);
                //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
                infoLabel2->setText(world->getEntityName(currentEntity));
                infoWidget->showEntityInfo(currentEntity);
                //selectCorrectComboBoxEntity(currentEntity);
                           //qDebug() << "MainWindow::qsoToEdit: " << QString::number(currentEntity) << QT_ENDL;
                othersTabWidget->setEntity(currentEntity);
                            //qDebug() << "MainWindow::qsoToEdit: - in default - 101"  << QT_ENDL;

                QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
                _qs.clear();
                //TODO: The band sometimes fails here. Check

                _qs << QString::number(currentEntity) << QString::number(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand())) << QString::number(dataProxy->getIdFromBandName(mainQSOEntryWidget->getMode()))  << QString::number(currentLog);


                            //qDebug() << "MainWindow::qsoToEdit: - in default - 104"  << QT_ENDL;
                        //qDebug() << "MainWindow:: - calling showStatusOfDXCC-05 " << QT_ENDL;
                showStatusOfDXCC(_qs);

                            //qDebug() << "MainWindow::qsoToEdit: - in default - 105"  << QT_ENDL;
                            //qDebug() << "MainWindow::qsoToEdit: - just after de IOTA"  << QT_ENDL;
    }
    else if (contestMode == "CQ-WW-SSB")
    {
    }
    else
    {
        //TODO: Manage the else... is it a DX or show a message?
    }
                //qDebug() << "MainWindow::qsoToEdit: - in default - 106"  << QT_ENDL;
    } //Closes the next.isValid
                //qDebug() << "MainWindow::qsoToEdit: - in default - END"  << QT_ENDL;
    readingTheUI = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::setModifying(const bool _m)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    modify = _m;
    QSOTabWidget->setModifying (_m);
    mainQSOEntryWidget->setModify(_m);
    satTabWidget->setModifying(_m);
    myDataTabWidget->setModify (_m);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotLocatorTextChanged(const QString &_loc)
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
      //qDebug() << "MainWindow::slotLocatorTextChanged: " << locatorLineEdit->text() << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if ( locator->isValidLocator(_loc) )
    {
        infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), _loc);
        satTabWidget->setLocator(_loc);
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::slotMyLocatorTextChanged(const QString &_loc)
{
             //qDebug() << "MainWindowMy::slotMyLocatorTextChanged: " <<_loc << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if ( locator->isValidLocator(_loc))
    {
        dxccStatusWidget->setMyLocator(_loc);

        slotLocatorTextChanged(QSOTabWidget->getDXLocator ());
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::showStatusOfDXCC(const QStringList _qs)
{
    //qDebug() << "MainWindow::showStatusOfDXC: Entity: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        //qDebug() << "MainWindow::showStatusOfDXCC -- END" << QT_ENDL;
        return;
    }
    // Set the status bar with the appropriate message
    int status = awards->getDXStatus (_qs);
    QString message = QString();

            //qDebug() << "MainWindow::showStatusOfDXC: " << QString::number(status) << QT_ENDL;

    message = awards->getDXStatusString(status);
    slotShowInfoLabel(message);
    //infoLabel1->setText(message);
    //infoWidget->showInfo((_qs.at(0)).toInt(), (_qs.at(1)).toInt(), (_qs.at(2)).toInt(), (_qs.at(3)).toInt() );
    infoWidget->showInfo((_qs.at(0)).toInt());
    //qDebug() << "MainWindow::showStatusOfDXC: -END" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::showDXMarathonNeeded(const int _dxcc, const int _cqz, const int _year, const int _log)
{
            //qDebug() << "MainWindow::showDXMarathonNeeded" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if ((_dxcc<=0) || (_cqz<=0) || (!manageDxMarathon))
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    if ( awards->isDXMarathonNeed(_dxcc, _cqz, _year, _log))
    {
        slotShowInfoLabel(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
        //infoLabel1->setText(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
void MainWindow::slotShowAwards()
{ //To be called from the logWindow & searchWidget
            //qDebug() << "MainWindow::slotShowAwards"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    awards->recalculateAwards();
             //qDebug() << "MainWindow::slotShowAwards-1"  << QT_ENDL;
    logWindow->refresh();
             //qDebug() << "MainWindow::slotShowAwards-2"  << QT_ENDL;

    awardsWidget->showAwards();
             //qDebug() << "MainWindow::slotShowAwards-3"  << QT_ENDL;
    //dxccStatusWidget->refresh();
    setMainWindowTitle();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
             //qDebug() << "MainWindow::slotShowAwards-END"  << QT_ENDL;
}



void MainWindow::fillQSOData()
{ // Updates all QSO with the dxcc, CQZ, ... if empty.
    //qDebug() << "MainWindow::fillQSOData" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

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
            if ( (query.value(nameCol)).isValid() )
            {
                _call = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("bandid");
            if ( (query.value(nameCol)).isValid() )
            {
                _bandid = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("modeid");
            if ( (query.value(nameCol)).isValid() )
            {
                _modeid = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("qso_date");
            if ( (query.value(nameCol)).isValid() )
            {
                _tdate = (query.value(nameCol)).toString();
            }
            //nameCol = rec.indexOf("lognumber");
            //if ( (query.value(nameCol)).isValid() )
            //{
            //    _lognumber = (query.value(nameCol)).toString();
            //}
            //nameCol = rec.indexOf("confirmed");
            //if ( (query.value(nameCol)).isValid() )
            //{
            //    _confirmed = (query.value(nameCol)).toString();
            //}
            nameCol = rec.indexOf("id");
            if ( (query.value(nameCol)).isValid() )
            {
                _id = (query.value(nameCol)).toString();
            }
                        //qDebug() << "MainWindow::fillQSOData: ID: " << _id << QT_ENDL;
            //TODO: Prepare this query
            updateString = "UPDATE log SET call = '" + _call + "', bandid = '" + _bandid + "', modeid = '" + _modeid + "', qso_date = '" + _tdate + "'" ;//  + "', confirmed = '" + _confirmed + "'";

            nameCol = rec.indexOf("cqz");
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number(world->getQRZCqz(_call));
                updateString = updateString + ", cqz='" + aux1 + "'";
            toModify = true;
            }

            nameCol = rec.indexOf("ituz");
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number( world->getQRZItuz(_call) );
                updateString = updateString + ", ituz='" + aux1 + "'";
                toModify = true;
            }

            //qDebug() << "MainWindow::fillQSOData: DXCC" << QT_ENDL;

            nameCol = rec.indexOf("dxcc");
            _dxcc = (query.value(nameCol)).toInt();
            //qDebug() << "MainWindow::fillQSOData: DXCC: " << QString::number(_dxcc) << QT_ENDL;
            if (_dxcc < 1)
            {
                aux1 = QString::number(world->getQRZARRLId(_call) );
                //qDebug() << "MainWindow::fillQSOData: DXCC proposed: " << aux1 << QT_ENDL;
                updateString = updateString + ", dxcc='" + aux1 + "'";
                toModify = true;
                _dxcc = aux1.toInt();
            }
            else
            {
                //qDebug() << "MainWindow::fillQSOData: DXCC already existed"  << QT_ENDL;
            }

            nameCol = rec.indexOf("cont");
            if (( (query.value(nameCol)).toString()).length() < 2 )
            {
                aux1 = world->getContinentShortName(_dxcc);
                //aux1 = QString::number( world->getQRZItuz(_call) );
                updateString = updateString + ", ituz='" + aux1 + "'";
                toModify = true;
            }

            _dxcc = -1;
                        //qDebug() << "MainWindow::fillQSOData1: " << updateString << QT_ENDL;
            if (toModify)
            {
                updateString = updateString + " WHERE id = " + "'" + _id + "'";
                            //qDebug() << "MainWindow::fillQSOData2: " << updateString << QT_ENDL;
                sqlOK = query1.exec(updateString);
                if (sqlOK)
                {
                                //qDebug() << "MainWindow::fillQSOData: sqlOK=True" << QT_ENDL;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().nativeErrorCode(), query1.lastQuery());
                                //qDebug() << "MainWindow::fillQSOData: sqlOK=False" << QT_ENDL;
                }
            }
            else
            {
                updateString.clear();
            }

            aux = tr("Filling DXCC, CQz, ITUz, Continent in QSOs...\n QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                            //qDebug() << "MainWindow::fillQSOData3: " << QT_ENDL;
                noMoreQso = true;
            }
        } // Closes the next.isValid
    } // Closes the While
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotFillEmptyDXCCInTheLog()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    dataProxy->fillEmptyDXCCInTheLog();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotUpdateCTYDAT()
{
               //qDebug() << "MainWindow::slotUpdateCTYDAT" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    downloadcty->download();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotUpdateSATSDAT()
{
             //qDebug() << "MainWindow::slotUpdateSATSDAT" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    updateSatsData->readSatDataFile();
    satTabWidget->refreshData();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotShowStats()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    statsWidget->show();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotWorldReload(const bool _b)
{
               //qDebug() << "MainWindow::slotWorldReload" << QT_ENDL;
    //TODO: world.recreate returns a boolean, so it is possible to manage the errors
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (_b)
    {
        world->recreate(ctyDatFile);
    }

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotFilePrint()
{
                //qDebug() << "MainWindow::slotFilePrint" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
            logEvent(Q_FUNC_INFO, "END-1", logSeverity);
            return;
            //TODO: Print a message showing an error and exit.
        }
        QSqlRecord rec = query.record();

        aux = tr("Printing the log...\n QSO: ")  + QString::number(_qsos) + "/" + QString::number(_numberOfQsos);
        progress.setLabelText(aux);
        progress.setValue(_qsos);

        while ((query.next()) && (!cancelPrinting))
        {
            if (query.isValid())
            {
                textTable->appendRows(1);
                row++;
                _qsos++;
                            //qDebug() << "MainWindow::slotFilePrint: QSO: " << QString::number(_qsos) << " - Step: " << QString::number(step) << " - Div: " << QString::number(_qsos % step)<< QT_ENDL;
                if (( (_qsos % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = tr("Printing the log...\n QSO: ")  + QString::number(_qsos) + "/" + QString::number(_numberOfQsos);
                    progress.setLabelText(aux);
                    progress.setValue(_qsos);
                }

                nameCol = rec.indexOf("id");
                cursor = textTable->cellAt(row, 0).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qso_date");
                cursor = textTable->cellAt(row, 1).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("call");
                cursor = textTable->cellAt(row, 2).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("rst_sent");
                cursor = textTable->cellAt(row, 3).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("rst_rcvd");
                cursor = textTable->cellAt(row, 4).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("bandid");
                aux = (query.value(nameCol)).toString();
                stringQuery = QString("SELECT name FROM band WHERE id='%1'").arg(aux);
                sqlOK = query1.exec(stringQuery);
                if (sqlOK)
                {
                    query1.next();
                    if (query1.isValid())
                    {
                        cursor = textTable->cellAt(row, 5).firstCursorPosition();
                        cursor.insertText((query1.value(0)).toString());
                    }
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().nativeErrorCode(), query1.lastQuery());
                }

                        //qDebug() << "MainWindow::slotFilePrint: Band: " << aux << QT_ENDL;
                nameCol = rec.indexOf("modeid");
                        //qDebug() << "MainWindow::slotFilePrint: nameCol: " << QString::number(nameCol) << QT_ENDL;
                aux = (query.value(nameCol)).toString();
                        //qDebug() << "MainWindow::slotFilePrint: Mode1: " << aux << QT_ENDL;
                aux = dataProxy->getNameFromSubModeId(aux.toInt());
                        //qDebug() << "MainWindow::slotFilePrint: Mode2: " << aux << QT_ENDL;
                if (aux.length()>1)
                {
                    cursor = textTable->cellAt(row, 6).firstCursorPosition();
                    cursor.insertText(aux);
                }
                else
                {
                            //qDebug() << "MainWindow::slotFilePrint: ERROR: " << aux << QT_ENDL;
                }

                nameCol = rec.indexOf("comment");
                aux = (query.value(nameCol)).toString();
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


        progress.setValue(_numberOfQsos);
        if (!cancelPrinting)
        {
            doc->print(&printer);
        }
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

//DX-CLUSTER - DXCLUSTER

void MainWindow::slotAnalyzeDxClusterSignal(QStringList ql)
{
               //qDebug() << "MainWindow::slotAnalyzeDxClusterSignal: 1: " << ql.at(0) <<"/1: " << ql.at(1) << "/2: " << ql.at(2) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

//void MainWindow::clusterSpotToLog(const QStringList _qs)
void MainWindow::clusterSpotToLog(const QString &_call, const QString &_freq)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString _aux;
    double _freqN = (_freq.toDouble()) / 1000;
    mainQSOEntryWidget->setQRZ(_call);
    QSOTabWidget->setTXFreq (_freqN);

    //freqQLCDNumber->display(_freqN);

    _aux = QString::number(_freqN);

                //qDebug() << "MainWindow::clusterSpotToLog - Freq: " << _aux << QT_ENDL;

    int _bandi = dataProxy->getBandIdFromFreq(_aux.toDouble());
                //qDebug() << "MainWindow::clusterSpotToLog - Bandi: " << QString::number(_bandi) << QT_ENDL;
    _aux = QString::number(_bandi);
    _aux = QString("SELECT name FROM band WHERE id ='%1'").arg(_aux);

                //qDebug() << "MainWindow::clusterSpotToLog - Band: " << _aux << QT_ENDL;

    QSqlQuery query(_aux);
    query.next();
    if (query.isValid())
    {
        _aux = (query.value(0)).toString();
        mainQSOEntryWidget->setBand(_aux);
        //bandComboBox->setCurrentIndex(bandComboBox->findText(_aux, Qt::MatchCaseSensitive));
    }
    else
    {
        mainQSOEntryWidget->setBand(dataProxy->getNameFromBandId(defaultBand));
        //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
        //bandComboBox->setCurrentIndex(defaultBand);
    }
    //qDebug() << "MainWindow::clusterSpotToLog - END "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
//DX-CLUSTER - DXCLUSTER

void MainWindow::updateQSLRecAndSent()
{
      //qDebug() << "MainWindow::updateQSLRecAndSent "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
           //qDebug() << "MainWindow::updateQSLRecAndSent - END"  << QT_ENDL;
}



void MainWindow::defineStationCallsign(const QString &_call)
{
    //qDebug() << "MainWindow::defineStationCallsign (currentLog): " << QString::number(currentLog) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (util->isValidCall (_call))
    {
        stationCallsign = _call;
    }
    else
    { // If no call is detected, qwe try to find it from the log
        QString logQRZ;
        logQRZ = dataProxy->getStationCallSignFromLog(currentLog);
        //qDebug() << "MainWindow::defineStationCallsign (logQrz): " << logQRZ << QT_ENDL;

        if ((world->checkQRZValidFormat(logQRZ)) && (util->isValidCall(logQRZ)))
        {
            //qDebug() << "MainWindow::defineStationCallsign TRUE "  << QT_ENDL;
            stationCallsign = logQRZ;
        }
    }

     //qDebug() << "MainWindow::defineStationCallsign: " << stationCallsign  << QT_ENDL;
    filemanager->setStationCallSign(stationCallsign);
    //qDebug() << "MainWindow::defineStationCallsign: AFTER"  << QT_ENDL;
    myDataTabWidget->setData(myPower, stationCallsign, operatorQRZ, myDataTabWidget->getMyLocator());
    dxccStatusWidget->setMyLocator(myDataTabWidget->getMyLocator());
    searchWidget->setStationCallsign(stationCallsign);
    lotwUtilities->setStationCallSign(stationCallsign);
    adifLoTWExportWidget->setDefaultStationCallsign(stationCallsign);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
             //qDebug() << "MainWindow::defineStationCallsign: " << stationCallsign << " - END" << QT_ENDL;
}

void MainWindow::slotSetPropModeFromSat(const QString &_p, bool _keep)
{
                //qDebug() << "MainWindow::slotSetPropModeFromSat: " << _p << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << Q_FUNC_INFO << ": _keep" << util->boolToQString(_keep) << QT_ENDL;

    othersTabWidget->setPropMode(_p, _keep);
    QSOTabWidget->setPropModeFromSat(_p);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
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
      //qDebug() << "MainWindow::completeWithPreviousQSO" << QT_ENDL;
    //This function completes: Name, QTH, Locator, Entity, Iota
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if ((!completeWithPrevious) || (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    //if ( (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    {
          //qDebug() << "MainWindow::completeWithPreviousQSO NOT completing..." << QT_ENDL;
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
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
      //qDebug() << "MainWindow::completeWithPreviousQSO completing..." << QT_ENDL;
    QString aux = QString();

    aux = dataProxy->getNameFromQRZ(_call);
      //qDebug() << "MainWindow::completeWithPreviousQSO aux: " << aux << QT_ENDL;
      //qDebug() << "MainWindow::completeWithPreviousQSO nameLineEdit: " << nameLineEdit->text() << QT_ENDL;

      //qDebug() << "MainWindow::completeWithPreviousQSO aux length: " << QString::number(aux.length()) << QT_ENDL;
      //qDebug() << "MainWindow::completeWithPreviousQSO nameL length: " << QString::number((nameLineEdit->text()).length()) << QT_ENDL;

    if ((aux.length()>=0) && (QSOTabWidget->getName().length()<=0) )
    {
          //qDebug() << "MainWindow::completeWithPreviousQSO name: 1" << QT_ENDL;
        QSOTabWidget->setName(aux);
        QSOTabWidget->setPaletteRightName (true);
        completedWithPreviousName = true;
    }
    else if (completedWithPreviousName && (aux != QSOTabWidget->getName()))
    {
          //qDebug() << "MainWindow::completeWithPreviousQSO name: 2" << QT_ENDL;
        completedWithPreviousName = false;
        QSOTabWidget->setPaletteRightName (false);
    }
    else
    {
          //qDebug() << "MainWindow::completeWithPreviousQSO name: 3" << QT_ENDL;
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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotValidBandsReceived(const QStringList &_b)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << Q_FUNC_INFO << QT_ENDL;
    dxccStatusWidget->setBands(Q_FUNC_INFO, _b, true);
    satTabWidget->addBands(_b);
    //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotFreqRXChanged(const double _fr)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!upAndRunning)
    {
        //qDebug() << Q_FUNC_INFO << " - not running" << QT_ENDL;
        return;
    }
    int bandId = dataProxy->getBandIdFromFreq(_fr);
    if (bandId < 1)
    {
        //qDebug() << Q_FUNC_INFO << " - wrong band" << QT_ENDL;
        return;
    }

    //mainQSOEntryWidget->setFreq (_fr, true);
    QSOTabWidget->setRXFreq (_fr);
    satTabWidget->setDownLinkFreq(_fr);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
   //qDebug() << "MainWindow::slotFreqRXChanged - END"  << QT_ENDL;
}

void MainWindow::slotFreqTXChanged(const double _fr)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (!upAndRunning)
    {
        //qDebug() << "MainWindow::slotFreqTXChanged !upAndRunning" << QT_ENDL;
        return;
    }
    QSOTabWidget->setTXFreq (_fr);
    mainQSOEntryWidget->setFreq (_fr);
    satTabWidget->setUpLinkFreq(_fr);
    if (hamlibActive)
    {
        hamlib->setFreq(_fr);
    }

    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::slotFreqTXChanged - END"  << QT_ENDL;
}

void MainWindow::slotShowQSOsFromDXCCWidget(QList<int> _qsos)
{
    //qDebug() << "MainWindow::slotShowQSOsFromDXCCWidget" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    slotShowSearchWidget();
    searchWidget->showQSOs(_qsos);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotWSJTXloggedQSO (const QString &_dxcall, const QString &_mode, const QString &_band, const double _freq,
                 const QString &_mygrid, const QString &_dxgrid, const QString &_rstTX, const QString &_rstRX,
                 const QString &_comment, const QString &_stationcallsign, const QString &_name,
                 const QString &_opCall, const QDateTime &_datetime, const QDateTime &_datetime_off,
                 const QString &_exchangeTX, const QString &_exchangeRX, const QString &_mypwr)
{
    //qDebug() << "MainWindow::slotWSJTX-loggedQSO" << QT_ENDL;
    //logEvent(Q_FUNC_INFO, "Start", logSeverity);
    bool logTheQso = false;
    QString opCall = stationCallsign;
    if (util->isValidCall(_opCall))
    {
        opCall = _opCall.toUpper();
    }
    double pwr = _mypwr.toDouble();
    if (pwr<=0.0)
    {
        pwr = myPower;
    }

    if (!_datetime.isValid() || !_datetime_off.isValid())
    {
          //qDebug() << Q_FUNC_INFO << " DATES NOT VALID " << QT_ENDL;
        return ;
    }

            //qDebug() << Q_FUNC_INFO << " dxcall: " << _dxcall << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " freq: " << QString::number(_freq/1000000) << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " freq no div: " << QString::number(_freq) << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " mode: " << _mode << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " band: " << _band << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " my_grid: " << _mygrid << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " dx_grid: " << _dxgrid << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " comment: " << _comment << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " StationCall: " << _stationcallsign << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " _opCall: " << _opCall << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " opCall: " << opCall << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " time_on: " << util->getDateTimeSQLiteStringFromDateTime(_datetime) << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " time_off: " << util->getDateTimeSQLiteStringFromDateTime(_datetime_off) << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " report_sent: " << _rstTX << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " report_rec: " << _rstRX << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " exchange_sent: " << _exchangeTX << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " exchange_rec: " << _exchangeRX << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " MY_PWR: " << _mypwr << QT_ENDL;


    if (wsjtxAutoLog)
    { // Log automatically, without confirmation
        logTheQso = true;
          //qDebug() << Q_FUNC_INFO << " LogTheQSO = true"  << QT_ENDL;
    }
    else
    { // Ask for confirmation before logging
              //qDebug() << Q_FUNC_INFO << " LogTheQSO = false - we ask for confirmation"  << QT_ENDL;
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
              //qDebug() << Q_FUNC_INFO << " QSO must be logged" << QT_ENDL;
            bool qsoLogged = false;
            int dxcc = world->getQRZARRLId(_dxcall);
            dxcc = util->getNormalizedDXCCValue(dxcc);

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
                        logEvent(Q_FUNC_INFO, "END-1", logSeverity);

                    break;
                    case QMessageBox::Discard:
                        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
                        return; //No more error shown
                      //  break;
                    default:
                    // should never be reached
                        logEvent(Q_FUNC_INFO, "END-3", logSeverity);
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
                //qDebug() << Q_FUNC_INFO << " Logged QSO OK: " << _dxcall << QT_ENDL;
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
                slotClearButtonClicked();
                //UDPLogServer->start();

                if (clublogActive && clublogRealTime)
                {
                    elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(dataProxy->getLastQSOid()));
                }
            }
            else
            {
                  //qDebug() << Q_FUNC_INFO << " Logged QSO NOK: " << _dxcall << QT_ENDL;
            }
        }
        else
        {
              //qDebug() << Q_FUNC_INFO << " QSO must NOT be logged ... ending" << QT_ENDL;
        }


    logEvent(Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << Q_FUNC_INFO << " - END" << QT_ENDL;
}



bool MainWindow::checkIfNewMode(const QString &_mode)
{
      //qDebug() << "MainWindow::checkIfNewMode: " << _mode << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (dataProxy->getSubModeIdFromSubMode(_mode)<0)
    {// The mode is not existing; it is not an accepted mode for KLog
     // TODO: Show an error to the user
                //qDebug() << "MainWindow::checkIfNewMode: Mode not valid! - " << _mode << QT_ENDL;

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
                logEvent(Q_FUNC_INFO, "END-1", logSeverity);
                return false;   // The user wants to keepseeing errors
            //break;
            case QMessageBox::No:
                logEvent(Q_FUNC_INFO, "END-1", logSeverity);
                return true; //No more error shown
              //  break;
            default:
            // should never be reached
                logEvent(Q_FUNC_INFO, "END-3", logSeverity);
            return false;   // The user wants to keepseeing errors
            //break;
        }
    }
    else
    {
        //noMoreModeErrorShown = false;
        //TODO: Add the new mode to the list of active modes
                //qDebug() << "MainWindow::checkIfNewMode: VALID NEW MODE: Adding... - " << _mode << QT_ENDL;
        addNewValidMode(_mode);
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return false;
}

void MainWindow::slotWSJXstatusFromUDPServer(const int _type, const QString &_dxcall, const double _freq, const QString &_mode,
                                             const QString &_report, const QString &_de_call, const QString &_de_grid,
                                             const QString &_dx_grid, const QString &_sub_mode)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (modify)
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
            //qDebug() << Q_FUNC_INFO << " type: " << QString::number(_type) << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " dxcall: " << _dxcall << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " freq: " << QString::number(_freq/1000000) << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " mode: " << _mode << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " report: " << _report << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " de_call: " << _de_call << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " _de_grid: " << _de_grid << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " dx_grid: " << _dx_grid << QT_ENDL;
            //qDebug() << Q_FUNC_INFO << " sub_mode: " << _sub_mode << QT_ENDL;

    if ((!mainQSOEntryWidget->isModeExisting(_mode)) && (!noMoreModeErrorShown))
    {
        noMoreModeErrorShown = checkIfNewMode(_mode);
    }

    switch (_type)
    {
        case 0:
              //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT/IN - Heartbeat" << QT_ENDL;
        break;
        case 1:
            //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT - Status" << QT_ENDL;
             mainQSOEntryWidget->setQRZ(_dxcall);
             if ((!noMoreModeErrorShown) && (dataProxy->getSubModeIdFromSubMode(_mode)>0) )
             {
                 mainQSOEntryWidget->setMode(_mode);
                //modeComboBox->setCurrentIndex(modeComboBox->findText(_mode, Qt::MatchCaseSensitive));
             }
             //qDebug() << "MainWindow::slotWSJXstatusFromUDPServer updating txFreqSpinBox" << QString::number(_freq) << QT_ENDL;
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
              //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - ERROR on Type" << QT_ENDL;
        break;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::addNewValidMode(const QString &_mode)
{
      //qDebug() << "MainWindow::addNewMode: " << _mode << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QStringList _newM;
    _newM.clear();
    _newM << _mode;
    _newM << mainQSOEntryWidget->getModes();

    readActiveModes (_newM);
    mainQSOEntryWidget->setModes(modes);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "MainWindow::addNewValidMode: END"  << QT_ENDL;
}

void MainWindow::slotClearNoMorErrorShown()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    noMoreErrorShown = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString queryFailed)
{
    //qDebug() << "MainWindow::slotQueryErrorManagement: Function: " << functionFailed << QT_ENDL;
    //qDebug() << "MainWindow::slotQueryErrorManagement: Error N#: " << QString::number(errorCodeN) << QT_ENDL;
    //qDebug() << "MainWindow::slotQueryErrorManagement: Error: " << functionFailed << errorCodeS << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (noMoreErrorShown)
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
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

    QString aux = "<br><b>" + tr("An unexpected error ocurred!!") + "</b><br><br>" + tr("If the problem persists, please contact the developers") + "(<a href=mailto:klog-devel@nongnu.org>klog-devel@nongnu.org</a>)" + tr("for analysis:") + "<br>";
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
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotDefineNewBands (const QStringList _bands)
{
        //qDebug() << "MainWindow::defineNewBands: "  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QStringList qsTemp;
    qsTemp.clear();

    //qsTemp << _bands;
    //qsTemp.removeDuplicates();

    qsTemp << dataProxy->sortBandNamesBottonUp(_bands);
    qsTemp.removeDuplicates();

    bands.clear();
    bands = qsTemp;
    mainQSOEntryWidget->setBands(bands);

    satTabWidget->addBands(bands);
        //qDebug() << "MainWindow::defineNewBands - END"  << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotHamlibTXFreqChanged(const double _f)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(_f) << QT_ENDL;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (upAndRunning)
    {
        if (!util->isSameFreq (_f, QSOTabWidget->getTXFreq ()))
        {
            //qDebug() << Q_FUNC_INFO << ": Updating the freq... " << QT_ENDL;
            QSOTabWidget->setTXFreq (_f);
        }
    }
    //qDebug() << "MainWindow::slotHamlibTXFreqChanged - END " << QT_ENDL;

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotHamlibModeChanged(const QString &_m)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (_m.length()<2)
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
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
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }
    if (mainQSOEntryWidget->isModeExisting(_m))
    {
             //qDebug() << "slotHamlibModeChanged: Mode in the Combobox: " << QT_ENDL;
        mainQSOEntryWidget->setMode(_m);
        //modeComboBox->setCurrentIndex(modeComboBox->findText(_m, Qt::MatchCaseSensitive));
    }
    else
    {
                //qDebug() << "MainWindow::slotHamlibModeChanged: Mode not found in combobox" << _m << QT_ENDL;
    }
    hamlibChangingMode = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
void MainWindow::slotRotatorShow()
{
    rotatorWidget->show();
}
*/
void MainWindow::slotUpdateLocator(QString _loc)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << "MainWindow::slotUpdateLocator: " << _loc << endl;
    QSOTabWidget->setDXLocator (_loc);
      //qDebug() << "MainWindow::slotUpdateLocator - END" << QT_ENDL;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::reconfigureDXMarathonUI(const bool _dxM)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    dxClusterWidget->setDXMarathon(_dxM);
    awardsWidget->setManageDXMarathon(_dxM);
    //awardsWidget->reconfigureDXMarathonUI(_dxM);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
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

void MainWindow::slotActiveHamlib(bool _enable)
{
    if (_enable)
    {
        hamlib->initClass();
    }
    else
    {
        hamlib->stop();
    }
}

void MainWindow::backupCurrentQSO()
{ // This function reads the full UI and stores it in a QSO
     //qDebug() << Q_FUNC_INFO;
    qso->clear ();
    qso->setBackup (true);
    qso->setModifying (mainQSOEntryWidget->getModifying());
    qso->setLogId (currentLog);
     //qDebug() << Q_FUNC_INFO << " - 010";
    // MainQSOEntryWidget
    qso->setCall (mainQSOEntryWidget->getQrz ());
     //qDebug() << Q_FUNC_INFO << " - 011";
    qso->setBand (mainQSOEntryWidget->getBand ());
     //qDebug() << Q_FUNC_INFO << " - 012";
    qso->setMode (mainQSOEntryWidget->getMode ());
     //qDebug() << Q_FUNC_INFO << " - 013";
    qso->setDateTimeOn (mainQSOEntryWidget->getDateTime ());
     //qDebug() << Q_FUNC_INFO << " - 014";
    qso->setRealTime (mainQSOEntryWidget->getRealTime ());
     //qDebug() << Q_FUNC_INFO << " - 020";
    //  MainWindowInputQSO
    qso->setRSTTX (QSOTabWidget->getRSTTX ());
    qso->setRSTRX (QSOTabWidget->getRSTRX ());
    qso->setFreqTX (QSOTabWidget->getTXFreq ());
    qso->setFreqRX (QSOTabWidget->getRXFreq ());
    qso->setGridSquare (QSOTabWidget->getDXLocator ());
    qso->setName (QSOTabWidget->getName ());
    qso->setQTH (QSOTabWidget->getQTH ());
    qso->setRXPwr (QSOTabWidget->getRXPwr ());
     //qDebug() << Q_FUNC_INFO << " - 030";
    // MainWindowInputQSL
    qso->setQSL_SENT (QSLTabWidget->getQSLSenStatus ());
    qso->setQSL_RCVD (QSLTabWidget->getQSLRecStatus ());
    qso->setQSLRDate (QSLTabWidget->getQSLRecDate ());
    qso->setQSLSDate (QSLTabWidget->getQSLSenDate ());
    qso->setQSLSenVia (QSLTabWidget->getSentVia ());
    qso->setQSLRecVia (QSLTabWidget->getRecVia ());
    qso->setQSLVia (QSLTabWidget->getQSLVia ());
    qso->setQSLMsg (QSLTabWidget->getQSLMsg ());
     //qDebug() << Q_FUNC_INFO << " - 040";
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
     //qDebug() << Q_FUNC_INFO << " - 050";
    // MainWindowInputComment
    qso->setComment (commentTabWidget->getComment ());
    qso->setKeepComment (commentTabWidget->getKeep ());
    // MainWindowInputOthers
    qso->setDXCC (othersTabWidget->getEntity ());
    qso->setIOTA (othersTabWidget->getIOTA ());
    qso->setPropMode (othersTabWidget->getPropModeFromComboBox ());
    qso->setKeepOthers (othersTabWidget->getKeep ());
    qso->setVUCCGrids (othersTabWidget->getVUCCGrids ());
     //qDebug() << Q_FUNC_INFO << " - 060";
    // MainWindowMyDataTab
    qso->setTXPwr (myDataTabWidget->getMyPower ());
    qso->setOperatorCallsign (myDataTabWidget->getOperator ());
    qso->setStationCallsign (myDataTabWidget->getStationCallsign ());
    qso->setMySOTA_REF (myDataTabWidget->getMySOTA ());
    qso->setMyRig (myDataTabWidget->getMyRig());
    qso->setMyAntenna (myDataTabWidget->getMyAntenna ());
    qso->setMyVUCCGrids (myDataTabWidget->getMyVUCCGrids ());
    qso->setMyGridSquare (myDataTabWidget->getMyLocator ());
    qso->setKeepMyData (myDataTabWidget->getKeep ());
     //qDebug() << Q_FUNC_INFO << " - 070";
    //MainWindowSatTab
    qso->setSatName (satTabWidget->getSatName ());
    qso->setSatMode (satTabWidget->getSatMode ());
    qso->setKeepSatTab (satTabWidget->getKeep ());
     //qDebug() << Q_FUNC_INFO << ": Realtime: " << util->boolToQString (qso->getRealTime ());
}

void MainWindow::restoreCurrentQSO(const bool restoreConfig)
{ // This function restores a QSO that was backed up to the UI.
    // MainQSOEntryWidget
    //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (restoreConfig);
    clearUIDX ();
    if (qso->getModifying())
    {
        mainQSOEntryWidget->setModify(true);
    }
    mainQSOEntryWidget->setQRZ (qso->getCall ());
    mainQSOEntryWidget->setBand (qso->getBand ());
    mainQSOEntryWidget->setMode (qso->getMode ());
    mainQSOEntryWidget->setDateTime (qso->getDateTimeOn ());

    if (restoreConfig)
    {
        //qDebug << Q_FUNC_INFO << ": restoring config: " << util->boolToQString (qso->getRealTime ());
        mainQSOEntryWidget->setRealTime (qso->getRealTime ());
    }
    else
    {
        //qDebug << Q_FUNC_INFO << ": NO restoring config";
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
    QSLTabWidget->setQSLSenVia (qso->getSentVia ());
    QSLTabWidget->setQSLRecVia (qso->getRecVia ());
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

    //qDebug() << Q_FUNC_INFO << " - END";
}

void MainWindow::setSeverity(const DebugLogLevel _sev)
{
    logSeverity = _sev;
    setupDialog->setSeverity(logSeverity);
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
    //qDebug() << "MainWindow::slotCaptureDebugLogs: " << _func << "_/" << _msg << QString::number(_level) << QT_ENDL;
    logEvent(_func, _msg, _level);
}

void MainWindow::logEvent(const QString &_func, const QString &_msg, const DebugLogLevel _level)
{   //This function is the only one not logging the activity
    showKLogLogWidget->addLog(_func, _msg, _level);

   // if ((!logEvents) || (!debugFileOpen) || (_level<=Debug)) // Increase to 7 show the full Debug
   // {
   //     //qDebug() << "MainWindow::slotCaptureDebugLogs: Not logging: " << _func << " / " << _msg << " / " << QString::number(_level) << QT_ENDL;
   //    return;
   // }
    //Criticality

    QTextStream out(debugFile);
    out << (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmsszzz") << " - " << _func << " - " << _msg << QT_ENDL;
}
