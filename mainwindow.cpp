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


#include <QtWidgets>
#include <QtSql>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "database.h"

#include "mainwindow.h"

//#include <qDebug>

MainWindow::MainWindow(const QString &_klogDir, const QString &tversion)
{
    qDebug() << "MainWindow::MainWindow: "<<  _klogDir << " Ver: " << tversion << endl;
    //qDebug() << "MainWindow::MainWindow: Con func: "<<  Q_FUNC_INFO << endl;

    softwareVersion = tversion;
    klogDir = _klogDir;
    logSeverity = 7;

    needToEnd = false;
    upAndRunning = false; // To define some actions that can only be run when starting the software

    util = new Utilities;
    QRZCOMAutoCheckAct = new QAction(tr("Check always the current QRZ in QRZ.com"), this);
    QRZCOMAutoCheckAct->setCheckable(true);
    QRZCOMAutoCheckAct->setChecked(false);
    QString debugName = util->getDebugLogFile();
    qDebug() << "MainWindow::MainWindow: Debug File: "<<  debugName << endl;
    debugFile = new QFile(debugName);


    if (!debugFile->open(QIODevice::WriteOnly | QIODevice::Text))
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
        logEvent(Q_FUNC_INFO, "KLog started!", 0);
    }

    //QTime start;
    //start = QTime::currentTime();
    qDebug() << "MainWindow::MainWindow: "<<  (QTime::currentTime()).toString("hhmmsszzz")<< endl;

    showErrorDialog = new ShowErrorDialog();
    UDPLogServer = new UDPServer();
    qDebug() << "MainWindow::MainWindow: BEFORE HAMLIB " << endl;
    hamlib = new HamLibClass();
    //pstRotator = new PSTRotatorSupport(this);
    //rotatorWidget = new RotatorWidget;
    qDebug() << "MainWindow::MainWindow: AFTER HAMLIB " << endl;

    dataProxy = new DataProxy_SQLite(softwareVersion, Q_FUNC_INFO);

    lotwUtilities = new LoTWUtilities(klogDir, softwareVersion, Q_FUNC_INFO, dataProxy);
    eqslUtilities = new eQSLUtilities(Q_FUNC_INFO);


    qDebug() << "MainWindow::MainWindow: Before DXCCStatusWidget " << endl;
    dxccStatusWidget = new DXCCStatusWidget(dataProxy, Q_FUNC_INFO);
    qDebug() << "MainWindow::MainWindow: After DXCCStatusWidget " << endl;
    qDebug() << "MainWindow::MainWindow: 00081" << endl;
    elogClublog = new eLogClubLog();
    qDebug() << "MainWindow::MainWindow: 00082" << endl;
    elogQRZcom = new eLogQrzLog(dataProxy, Q_FUNC_INFO);


   qDebug() << "MainWindow::MainWindow: 00083" << endl;
    updateSatsData = new UpdateSatsData(dataProxy);
    qDebug() << "MainWindow::MainWindow: 00084" << endl;
    statsWidget = new StatisticsWidget(dataProxy);

    qDebug() << "MainWindow::MainWindow: 00085" << endl;
    //statsWidget->show();

    infoLabel1 = new QLabel(tr("Status bar ..."));
    infoLabel2 = new QLabel(tr("DX Entity"));

    qDebug() << "MainWindow::MainWindow: 00086" << endl;
    logWindow = new LogWindow(dataProxy, this);
    qDebug() << "MainWindow::MainWindow: 00087" << endl;

    searchWidget = new SearchWidget (dataProxy, this);
    qDebug() << "MainWindow::MainWindow: 00087.1" << endl;
    infoWidget = new InfoWidget(dataProxy, this);

    qDebug() << "MainWindow::MainWindow: 00088" << endl;
    logEvent(Q_FUNC_INFO, "Creating AwardsWidget", 7);
    awardsWidget = new AwardsWidget(dataProxy, this);

    qDebug() << "MainWindow::MainWindow: 0009" << endl;

    aboutDialog = new AboutDialog(softwareVersion);
    tipsDialog = new TipsDialog();

    configFileName = util->getCfgFile();
    ctyDatFile = util->getCTYFile();

    downloadcty = new DownLoadCTY(klogDir, softwareVersion);

    statusBarMessage = tr("Starting KLog");

    if (!QDir::setCurrent ( klogDir )){
        QDir d1(klogDir);
        if (d1.mkdir(klogDir))
        {
            QDir::setCurrent ( klogDir );
        }
    }

    qDebug() << "MainWindow::MainWindow: 4" << endl;
    world = new World(dataProxy, klogDir, softwareVersion, Q_FUNC_INFO);

    qDebug() << "MainWindow::MainWindow: xx" << endl;

    setupDialog = new SetupDialog(dataProxy, configFileName, softwareVersion, 0, !configured);    
    qDebug() << "MainWindow::MainWindow: satTabWidget to be created" << endl;
    satTabWidget = new MainWindowSatTab(dataProxy);

    myDataTabWidget = new MainWindowMyDataTab();
    commentTabWidget = new MainWindowInputComment();
    othersTabWidget = new MainWindowInputOthers(dataProxy);
    eQSLTabWidget = new MainWindowInputEQSL(dataProxy);
    QSLTabWidget = new MainWindowInputQSL(dataProxy);
    mainQSOEntryWidget = new MainQSOEntryWidget(dataProxy);
    qDebug() << "MainWindow::MainWindow: locator to be created" << endl;
    locator = new Locator();

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    qDebug() << "MainWindow::MainWindow: 8" << endl;

    dateTime = new QDateTime();
    dateTimeTemp = new QDateTime();

    nameLineEdit = new QLineEdit;
    qthLineEdit = new QLineEdit;
    locatorLineEdit = new QLineEdit;
    rstTXLineEdit = new QLineEdit;
    rstRXLineEdit = new QLineEdit;

    // UI DX
    //previousQRZ = new QLabel(tr("Status bar..."));
    infoLabel2 = new QLabel(tr("DX Entity"));
    loggWinAct = new QAction(tr("&Log Window"), this);

    operatorLineEdit = new QLineEdit;
    stationCallSignLineEdit = new QLineEdit;
    txFreqSpinBox = new QDoubleSpinBox;
    rxFreqSpinBox = new QDoubleSpinBox;
    rxPowerSpinBox = new QDoubleSpinBox;

    qDebug() << "MainWindow::MainWindow: dxclusterwidget to be created" << endl;
    dxClusterWidget = new DXClusterWidget(dataProxy, dxclusterServerToConnect , dxclusterServerPort, this);
    qDebug() << "MainWindow::MainWindow: Awards to be created" << endl;
    awards = new Awards(dataProxy, Q_FUNC_INFO);
    qDebug() << "MainWindow::MainWindow: Awards created" << endl;
    // </UI>


    if (needToEnd)
    {
       exit(0);
    }

    qDebug() << "MainWindow::MainWindow:  UI to be created" << endl;



    qDebug() << "MainWindow::MainWindow: Software update to be created" << endl;
    softUpdate = new SoftwareUpdate(softwareVersion);
    qDebug() << "MainWindow::MainWindow: FileManager to be created" << endl;
    filemanager = new FileManager(dataProxy, klogDir, softwareVersion);
    qDebug() << "MainWindow::MainWindow: FileAwardManager to be created" << endl;
    fileAwardManager = new FileAwardManager(dataProxy, Q_FUNC_INFO);
    
    lotwCallTQSL = new QAction(tr("Upload to LoTW"), this);
    qDebug() << "MainWindow::MainWindow: AdifLoTWExportWidget to be created" << endl;
    adifLoTWExportWidget = new AdifLoTWExportWidget(dataProxy, Q_FUNC_INFO);
    qDebug() << "MainWindow::MainWindow: ShowAdifImportWidget to be created" << endl;
    showAdifImportWidget = new ShowAdifImportWidget(dataProxy, Q_FUNC_INFO);


    //lotwUtilities->download();
    logEvent(Q_FUNC_INFO, "END", logSeverity);


    qDebug() << "MainWindow::MainWindow: END" << endl;
}

void MainWindow::saveWindowsSize()
{
    //qDebug()() << "MainWindow::saveWindows" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QSize size = this->size();

    int height = size.height();
    int width = size.width();
    //qDebug()() << "MainWindow::windowsSizeAndPosition: Heigth: " << QString::number(height)  << endl;
    //qDebug()() << "MainWindow::windowsSizeAndPosition: Width: " << QString::number(width)  << endl;
    //(const QString& _filename, const QString &_field, const QString &_value)
    filemanager->modifySetupFile(configFileName, "MainWindowSize", QString::number(width) + "x" + QString::number(height));
    //return QString::number(width) + "x" + QString::number(height);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug()() << "MainWindow::windowsSizeAndPosition: END" << endl;

}

void MainWindow::setWindowsSize(const int _width, const int _height)
{
    QSize size;
    size.setHeight(_height);
    size.setWidth(_width);
    this->resize(size);

}

void MainWindow::init()
{
          //qDebug() << "MainWindow::init: START" << endl;
    logEvents = true;
    debugFileOpen = false;

    infoLabel1T = QString();
    infoLabel2T = QString();

    //Default band/modes
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW" << "RTTY";
    hamlibActive = false;
    hamlibModeNotADIFSupported = false;
    hamlibChangingMode = false;
    yearChangedDuringModification = false;

    //usePSTRotator = false;

    readingTheUI = false;
    itIsANewversion = false;

    setCleaning(false);

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
    txFreqBeingChanged = false;
    txFreqBeingAutoChanged = false;
    rxFreqBeingChanged = false;
    updatingBands = false;
    selectedYear = 0;
    defaultMode = 1;
    defaultBand = 1;
          //qDebug() << "MainWindow::init: 1 - currentMode: " << QString::number(currentMode) << endl;
    currentMode = 1;
          //qDebug() << "MainWindow::init: 2 - currentMode: " << QString::number(currentMode) << endl;
    currentModeShown = currentMode;
    currentBand = 1;
    currentBandShown = currentBand;
    currentLog = 1;
    //points = 0;
    //multipliers = 0;
    //qsoPoints = 0;
    //qsoMultiplier = 0;
    operatorQRZ = "";
    stationQRZ = "";
    mainQRZ = "";
    //myLocator = "";
    dxLocator ="";
    myPower = 0.0;
    UDPServerStart = false;   // By default the UDP server is started

          //qDebug() << "MainWindow::init: 0007" << endl;
    currentEntity = -1; // To optimize the calls to different world methods if the entity does not change. Used in slotQRZTextChanged
    previousEntity = -1;// To optimize the calls to different world methods if the entity does not change.
    //realTime=true;
    //UTCTime=true;
    keepMyData=true;
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
          //qDebug() << "MainWindow::init: 0008" << endl;
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

    qrzcomUser = QString();
    qrzcomPass = QString();

    callingUpdate = false; // to control whether the update is mannually launched or at the begining
    //previousQrz = "";
    setModifying(false);
    selectedYear = (dateTime->currentDateTime()).date().year();
    loggWinAct->setShortcut(Qt::CTRL + Qt::Key_L);

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);


    rxPowerSpinBox->setDecimals(2);
    rxPowerSpinBox->setMaximum(9999);
    rxPowerSpinBox->setSuffix(" " + tr("Watts"));

    txFreqSpinBox->setDecimals(3);
    txFreqSpinBox->setMaximum(99999);
    txFreqSpinBox->setSuffix(" " + tr("MHz"));

    rxFreqSpinBox->setDecimals(3);
    rxFreqSpinBox->setMaximum(99999);
    rxFreqSpinBox->setSuffix(" " + tr("MHz"));

    // Check date & time and set them in the UI at the begining
    //dateTime->currentDateTime();

    clublogAnswer = -1;

    defaultColor.setNamedColor("slategrey");
    neededColor.setNamedColor("yellow");
    workedColor.setNamedColor("blue");
    confirmedColor.setNamedColor("red");
    newOneColor.setNamedColor("green");

    bool existingData = QFile::exists(util->getKLogDBFile());
    if (!existingData)
    {
        world->create(ctyDatFile);
    }
   //qDebug() << "MainWindow::init: 0013" << endl;
    readConfigData();
   //qDebug() << "MainWindow::init: 0014" << endl;
    logWindow->createlogPanel(currentLog);
          //qDebug() << "MainWindow::init: 0015" << endl;
    awards->setManageModes(manageMode);
          //qDebug() << "MainWindow::init: 0016" << endl;
    if (dataProxy->getNumberOfManagedLogs()<1)
    {
              //qDebug() << "MainWindow::init: 16.1" << endl;
        openSetup(6);
              //qDebug() << "MainWindow::init: 16.2" << endl;
    }
          //qDebug() << "MainWindow::init: 17" << endl;
    checkIfNewBandOrMode();

    awardsWidget->fillOperatingYears();
    awardsWidget->showAwards();
    awardsWidget->setManageDXMarathon(manageDxMarathon);

    dxClusterWidget->setCurrentLog(currentLog);

          //qDebug() << "MainWindow::Init: calling Software update..." << endl;
    if (checkNewVersions)
    {//reportInfo
        if (reportInfo)
        {
            softUpdate->addCall(stationQRZ);
        }
        softUpdate->needToUpdate();
    }

    currentBandShown = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
    currentModeShown = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
    currentBand = currentBandShown;
    currentMode = currentModeShown;

//    timer->start(1000);
    timerInfoBars = new QTimer(this);


          //qDebug() << "MainWindow::init: Calling createUI" << endl;
    createUI();
          //qDebug() << "MainWindow::init: Calling slotClearButtonClicked" << endl;
    slotClearButtonClicked();

    infoWidget->showInfo(-1);

    //lotwTQSLpath = util->getTQSLsPath() + util->getTQSLsFileName();
    upAndRunning = true;
    mainQSOEntryWidget->setUpAndRunning(upAndRunning);
    hamlib->readRadio(true); // Forcing the radio update
          //qDebug() << "MainWindow::init: END" << endl;
}


void MainWindow::createActionsCommon(){
// Functional widgets connections
//TODO: Reimplement the possibility to enter a QSO with enter inthe following widgets:
    //connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
// Return pressed = QSO ENTRY
    //connect(qrzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    //connect(SRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    //connect(STXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstTXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(operatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(stationCallSignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );


    connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
    connect(myDataTabWidget, SIGNAL(myLocChangedSignal(QString)), this, SLOT(slotMyLocatorTextChanged(QString) ) );
    connect(myDataTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(txFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqTXChanged()) ) ;
    connect(rxFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqRXChanged()) ) ;


    connect(loggWinAct, SIGNAL(triggered()), this, SLOT(slotLogWinShow()));
    //connect(bandComboBox, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    //connect(dateEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    //connect(timeEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    //Actions to pass the focus between QRZ / SRX

    //connect(rstTXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstTXTextChanged() ) );
    //connect(rstRXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstRXTextChanged() ) );

    //connect(qslViaLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQSLViaTextChanged() ) );

    //Buttons Actions

    connect(mainQSOEntryWidget, SIGNAL(currentQRZSignal(QString)), this, SLOT(slotQRZTextChanged(QString)));
    connect(mainQSOEntryWidget, SIGNAL(debugLog(QString, QString, int)), this, SLOT(slotCaptureDebugLogs(QString, QString, int)) );
    connect(mainQSOEntryWidget, SIGNAL(showInfoLabel(QString)), this, SLOT(slotShowInfoLabel(QString)) );
    connect(mainQSOEntryWidget, SIGNAL(clearForNextQSOSignal()), this, SLOT(slotClearButtonClicked()) );
    connect(mainQSOEntryWidget, SIGNAL(OKClicked()), this, SLOT(slotQRZReturnPressed() ) );
    connect(mainQSOEntryWidget, SIGNAL(bandChanged(QString)), this, SLOT(slotBandChanged(QString) ) );
    connect(mainQSOEntryWidget, SIGNAL(modeChanged(QString)), this, SLOT(slotModeChanged(QString) ) );


    // LOGVIEW
    connect(logWindow, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(logWindow, SIGNAL(actionDeleteQSO ( int ) ), this, SLOT(slotQSODelete(int) ) );
    connect(logWindow, SIGNAL(deleteTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsDelete(QList<int>) ) );
    connect(logWindow, SIGNAL(exportToADIFTheseQSOs ( QList<int> ) ), this, SLOT(slotQSOsExportToADIF(QList<int>) ) );


    connect(logWindow, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(logWindow, SIGNAL(updateSearchText()), this, SLOT(slotSearchBoxTextChanged() ) ); //When a QSO is deleted
    connect(logWindow, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );    

    //CLUSTER
    //void clusterSpotToLog(const QStringList _qs);
    //SIGNAL dxspotclicked(const QStringList _qs)
    connect(dxClusterWidget, SIGNAL(dxspotclicked(QStringList)), this, SLOT(slotAnalyzeDxClusterSignal(QStringList) ) );

    // CLUBLOG
    connect (elogClublog, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (elogClublog, SIGNAL (actionReturnDownload(int, int)), this, SLOT (slotElogClubLogProcessAnswer(int, int)));
    connect (elogClublog, SIGNAL (disableClubLogAction(bool)), this, SLOT (slotElogClubLogDisable(bool)));
    connect (elogClublog, SIGNAL (signalFileUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogClubLogFileUploaded(QNetworkReply::NetworkError, QList<int>)));

    connect (eqslUtilities, SIGNAL (actionReturnDownload(int, int)), this, SLOT (slotElogClubLogProcessAnswer(int, int)));
    //connect (eqslUtilities, SIGNAL (disableClubLogAction(bool)), this, SLOT (slotElogClubLogDisable(bool)));
    connect (eqslUtilities, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (eqslUtilities, SIGNAL (signalFileUploaded(QNetworkReply::NetworkError, QList<int>)), this, SLOT (slotElogEQSLFileUploaded(QNetworkReply::NetworkError, QList<int>)));

    // QRZCOM
    connect (elogQRZcom, SIGNAL (showMessage(QString)), this, SLOT (slotElogQRZCOMShowMessage(QString)));
    connect (elogQRZcom, SIGNAL (dataFoundSignal(QString, QString)), this, SLOT (slotElogQRZCOMFoundData(QString, QString)));



    // SATELLITE TAB
    //connect (satTabWidget, SIGNAL (satBandTXChanged(QString)), this, SLOT (slotSatBandTXComboBoxChanged(QString)));
    //connect(satTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );
    connect(world, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );

    // QSL TAB
    connect(QSLTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );
    // SEARCH TAB
    connect(searchWidget, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(searchWidget, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );

    connect(searchWidget, SIGNAL(logRefresh() ), this, SLOT(slotLogRefresh() ) );
    connect(searchWidget, SIGNAL(toStatusBar(QString) ), this, SLOT(slotUpdateStatusBar(QString) ) );
    connect(searchWidget, SIGNAL(requestBeingShown() ), this, SLOT(slotShowSearchWidget() ) );
    connect(searchWidget, SIGNAL(actionQSODelete( int ) ), this, SLOT(slotQSODelete(int) ) );
    connect(searchWidget, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );

    connect(awards, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    connect(awards, SIGNAL(awardDXCCUpdated()), this, SLOT(slotRefreshDXCCWidget()) );
    connect(awardsWidget, SIGNAL(debugLog(QString, QString, int)), this, SLOT(slotCaptureDebugLogs(QString, QString, int)) );
    connect(awardsWidget, SIGNAL(requireCurrentLogSignal()), this, SLOT(slotAwardsWidgetSetLog()) );
    connect(awardsWidget, SIGNAL(requireCurrentYearSignal()), this, SLOT(slotAwardsWidgetSetYear()) );

    //DXCCWIDGET TAB
    //connect(dxccStatusWidget, SIGNAL(showQso(int)), this, SLOT(slotShowQSOFromDXCCWidget(int) ) );
    connect(dxccStatusWidget, SIGNAL(showQsos(QList<int>)), this, SLOT(slotShowQSOsFromDXCCWidget(QList<int>) ) );
    //connect(dxccStatusWidget, SIGNAL(updateAwards()), this, SLOT(slotShowAwards() ) );

    connect(filemanager, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    connect(filemanager, SIGNAL(addQSOToList(QStringList)), this, SLOT(slotReceiveQSOListToShowFromFile(QStringList)) );

    //connect(scoreeWinAct, SIGNAL(triggered()), this, SLOT(slotScoreWinShow()));

    // UDPLogServer - WSJT-x

   connect(UDPLogServer, SIGNAL(status_update(int, QString, double, QString, QString, QString, QString, QString, QString)), this, SLOT(slotWSJXstatusFromUDPServer(int, QString, double, QString, QString, QString, QString, QString, QString) ) );
   connect(UDPLogServer, SIGNAL( logged_qso(QString, QString, QString, double, QString, QString, QString, QString, QString, QString, QString, QString, QDateTime, QDateTime, QString, QString, QString)), this, SLOT(slotWSJTXloggedQSO (QString, QString, QString, double, QString, QString, QString, QString, QString, QString, QString, QString, QDateTime, QDateTime, QString, QString, QString) ) );


   connect(this, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
   connect(setupDialog, SIGNAL(debugLog(QString, QString, int)), this, SLOT(slotCaptureDebugLogs(QString, QString, int)) );
   connect(setupDialog, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
   connect(setupDialog, SIGNAL(exitSignal(int)), this, SLOT(slotExitFromSlotDialog(int)) );
   connect(setupDialog, SIGNAL(qrzcomAuto(bool)), this, SLOT(slotElogQRZCOMAutoCheckFromSetup(bool)) );



   connect(tipsDialog, SIGNAL(debugLog(QString, QString, int)), this, SLOT(slotCaptureDebugLogs(QString, QString, int)) );
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
   connect(satTabWidget, SIGNAL(satRxFreqChanged(double)), this, SLOT(slotSatChangeRXFreq(double)) );
   connect(satTabWidget, SIGNAL(satTxFreqChanged(double)), this, SLOT(slotSatChangeTXFreq(double)) );
   connect(satTabWidget, SIGNAL(dxLocatorChanged(QString)), this, SLOT(slotUpdateLocator(QString)) );
   connect(satTabWidget, SIGNAL(setPropModeSat(QString)), this, SLOT(slotSetPropMode(QString)) ) ;
   connect(satTabWidget, SIGNAL(satTXFreqNeeded(double)), this, SLOT(slotSatTXFreqNeeded(double)));
   connect(satTabWidget, SIGNAL(satRXFreqNeeded(double)), this, SLOT(slotSatRXFreqNeeded(double)));
   connect(satTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );

   connect(downloadcty, SIGNAL(done(bool)), this, SLOT(slotWorldReload(bool)) );

 //connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()) );
   connect(timerInfoBars, SIGNAL(timeout()), this, SLOT(slotTimeOutInfoBars()) );

   connect(hamlib, SIGNAL(freqChanged(double)), this, SLOT(slotHamlibTXFreqChanged(double)) );
   connect(hamlib, SIGNAL(modeChanged(QString)), this, SLOT(slotHamlibModeChanged(QString)) );


   connect(lotwUtilities, SIGNAL(actionProcessLoTWDownloadedFile(QString)), this, SLOT(slotLoTWDownloadedFileProcess(QString)) );

   connect(adifLoTWExportWidget, SIGNAL(selection(QString, QDate, QDate, ExportMode)), this, SLOT(slotADIFExportSelection(QString, QDate, QDate, ExportMode)) );

    connect(dataProxy, SIGNAL(debugLog(QString, QString, int)), this, SLOT(slotCaptureDebugLogs(QString, QString, int)) );
    //connect(this, SIGNAL(focusC), this, SLOT(slotTimeOutInfoBars()) );
    logEvent(Q_FUNC_INFO, "END", logSeverity);

}



void MainWindow::recommendBackupIfNeeded()
{
    if (dataProxy->getHowManyQSOInLog(-1)<1)
    {
        return;
    }
    QDateTime lastBackupDate;
    lastBackupDate = QDateTime();
    lastBackupDate = filemanager->getDateTimeOfLastBackup();
    bool backupNeeded = false;
    QString msg;
    if (lastBackupDate == QDateTime())
    {
        backupNeeded = true;
        msg = tr("It seems that you have never done a backup or exported your log to ADIF.");

    }
    else if (lastBackupDate.addMonths(1) < QDateTime::currentDateTime())
    {
        backupNeeded = true;
        msg = tr("It seems that the latest backup you did is older than one month.");
    }

    if (backupNeeded)
    {
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
            filemanager->adifLogExport(filename, 0); // 0 will save ALL the logs
            break;
        }
    }
}

void MainWindow::checkIfNewVersion()
{
    if (itIsANewversion)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog new version detected!"));
        msgBox.setText(tr("It seems that you are running this version of KLog for the first time."));
        msgBox.setInformativeText(tr("The setup will be open to allow you to do any new setup you may need."));
        msgBox.exec();
        openSetup();
    }
}

MainWindow::~MainWindow()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (hamlibActive)
    {
        hamlib->stop();
    }
    logEvent(Q_FUNC_INFO, "KLog exit", 0);
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
void MainWindow::setMainWindowTitle(const QString _s)
{
    QString aux = dataProxy->getCommentsFromLog(currentLog);
    //qDebug() << "MainWindow::setMainWindowTitle:  (comment): " << aux << endl;
    if (aux.length()>0)
    {
        setWindowTitle(tr("KLog - %1 - QSOs: %2 - %3" ).arg(stationQRZ).arg(_s).arg(aux));
    }
    else
    {
        setWindowTitle(tr("KLog - %1 - QSOs: %2" ).arg(stationQRZ).arg(_s));
    }


}

void MainWindow::createUI()
{

          //qDebug() << "MainWindow::createUI" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    createStatusBar();
    setWindowTitle(tr("KLog"));
    createUIDX();
    createActionsCommon();
    createMenusCommon();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::createUI-END" << endl;
}

void MainWindow::slotTimeOutInfoBars()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << "MainWindow::slotTimeOutInfoBars - Start" << endl;
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
        if (txFreqSpinBox->value() >= dataProxy->getLowLimitBandFromBandName("20M"))
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
          //qDebug() << "MainWindow::slotBandChanged: " << _b << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!upAndRunning)
    {
        return;
    }

    if (txFreqBeingChanged || updatingBands)
    {
              //qDebug() << "MainWindow::slotBandChanged: txFreqBeingChanged"  << endl;
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }

    bool isFRinBand = dataProxy->isThisFreqInBand(_b, QString::number(txFreqSpinBox->value()));
    if ((isFRinBand) && (txFreqSpinBox->value() >0 ))
    { // No change in txFreq
              //qDebug() << "MainWindow::slotBandChanged: idFRinBand and Freq >0"  << endl;
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

    currentBandShown = dataProxy->getIdFromBandName(_b);
    currentModeShown = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());
    currentBand = currentBandShown;
    currentMode = currentModeShown;
          //qDebug() << "MainWindow::slotBandChanged: Checking to update Freq"  << endl;
    if ((!isFRinBand) || (txFreqSpinBox->value()<=0))
    {
              //qDebug() << "MainWindow::slotBandChanged: Freq is not in band or empty"  << endl;
        double txFr = (dataProxy->getFreqFromBandId(currentBandShown)).toDouble();
        txFreqSpinBox->setValue(txFr);
              //qDebug() << "MainWindow::slotBandChanged: Ne Freq: " << QString::number(txFr)  << endl;
        if (!dataProxy->isThisFreqInBand(_b, QString::number(rxFreqSpinBox->value())))
        {
            rxFreqSpinBox->setValue(txFreqSpinBox->value());
        }
    }
          //qDebug() << "MainWindow::slotBandChanged: Checking to update Freq  - DONE"  << endl;


    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);

          //qDebug() << "MainWindow:: - calling showStatusOfDXCC-02 " << endl;
    showStatusOfDXCC(_qs);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::slotBandChanged: END" << endl;
}

void MainWindow::slotModeChanged (const QString &_m)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!upAndRunning)
    {
        return;
    }
          //qDebug() << "MainWindow::slotModeChanged: " << _m << endl;

    currentBandShown = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
           //qDebug() << "MainWindow::slotModeComboBoxChanged: currentBandShown2: " << QString::number(currentBandShown) << endl;
    currentModeShown = dataProxy->getIdFromModeName(_m);
    currentBand = currentBandShown;
    currentMode = currentModeShown;

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
          //qDebug() << "MainWindow:: - calling showStatusOfDXCC-01 " << endl;
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);
    showStatusOfDXCC(_qs);
    setRSTToMode(mainQSOEntryWidget->getMode());

    QString _modeSeen = mainQSOEntryWidget->getMode();
    if (_modeSeen == "SSB")
    {
        setModeFromFreq();
    }

    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::slotModeComboBoxChanged2: " << mainQSOEntryWidget->getMode() << endl;
}


void MainWindow::slotQRZReturnPressed()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
          //qDebug() << "MainWindow::slotQRZReturnPressed: "  << endl;
    if (mainQSOEntryWidget->getQrz().length()<=0)
    {
        //qDebug() << "MainWindow::slotQRZReturnPressed: no QRZ"  << endl;
        return;
    }
    readingTheUI = true;

    int errorCode = 0;

    // Just to prepare or some tasks before reading DATA from UI

    QSqlQuery query;
    QString queryString = readDataFromUI();    

    //qDebug() << "MainWindow::slotQRZReturnPressed: queryString: " << queryString << endl;

    if (queryString != "NULL")
    {
        if (!query.exec(queryString))
        {
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                query.finish();
                //qDebug() << "MainWindow::slotQRZReturnPressed: Query ERROR: (queryString): " << queryString << endl;
                errorCode = query.lastError().number();
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("KLog - Unexpected error"));
                msgBox.setIcon(QMessageBox::Warning);
                QString aux = tr("An unexpected error ocurred when trying to add the QSO to your log. If the problem persists, please contact the developer for analysis: ");
                msgBox.setText(aux + "MW-1#" + QString::number(errorCode));
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
                //qDebug() << "MainWindow::slotQRZReturnPressed: QSO Added! " << endl;
                actionsJustAfterAddingOneQSO();
                slotClearButtonClicked();
            }
        }
    else   // The QUERY string is NULL
        {
            logEvent(Q_FUNC_INFO, "END-2", logSeverity);
            //qDebug() << "MainWindow::slotQRZReturnPressed: QUERY string is NULL " << endl;
            readingTheUI = false;
            return;

        }

    setModifying(false);
    modifyingQSO = -1;
    yearChangedDuringModification = false;    
    readingTheUI = false;
    slotClearButtonClicked();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::actionsJustAfterAddingOneQSO()
{
    //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    int lastId = -1;
    needToSave = true;
    if (modify)
    {
    //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Modifying! " << endl;
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
              //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (Modifiying ClubLog) Lastid: "<< QString::number(lastId) << endl;
               // Delete QSO in CLubLog
               elogClublog->deleteQSO(clublogPrevQSO);
               // Add modified QSO in ClubLog
               elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(modifyingQSO));
           }
           else
           {
                        //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (No ClubLog) Lastid: "<< QString::number(lastId) << endl;
           }
           awards->setAwards(modifyingQSO);   //Update the DXCC award status
       }

       // CHECK WHAT WAS THE QSOID to add the awards, if needed

    }
    else
    {
        //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Not Modifying " << endl;
        lastId = dataProxy->getLastQSOid();
        if (lastId>=0)
        {
            //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Lastid: "<< QString::number(lastId) << endl;
            awards->setAwards(lastId);   //Update the DXCC award status

            // Send to CLUBLOG if enabled

            if ((clublogActive) && (clublogRealTime))
            {
                //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (Sending ClubLog) Lastid: "<< QString::number(lastId) << endl;
                elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(lastId));
            }
            else
            {
                //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: (No ClubLog) Lastid: "<< QString::number(lastId) << endl;
            }
            //<CLUBLOG>
        }
        else

        {
            //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Lastid < 0 "<< QString::number(lastId) << endl;
        }
        //awards->setAwards(lastId);
    }

    logWindow->refresh();
    dxccStatusWidget->refresh();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO - END" << endl;

}

QString MainWindow::readDataFromUI()
{
          //qDebug() << "MainWindow::readDataFromUI: " << endl;
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
           //qDebug() << "MainWindow::readDataFromUIDX:" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QString tqrz = (mainQSOEntryWidget->getQrz()).toUpper();
    if (!util->isValidCall(tqrz))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - Not valid call"));
        QString aux = QString(tr("The call %1 is not a valid call. Do you really want to add this call to the log?") ).arg(tqrz);
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
          //qDebug() << "MainWindow::readDataFromUIDX: Band: " << QString::number(currentBand) << endl;
          //qDebug() << "MainWindow::readDataFromUIDX: Mode: " << QString::number(currentMode)  << endl;
    int tband = dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand());
    int tmode = dataProxy->getIdFromModeName(mainQSOEntryWidget->getMode());

    //int tmode = currentMode;


    QString tdate = util->getDateTimeSQLiteStringFromDateTime(mainQSOEntryWidget->getDateTime());
    QString ttime = (mainQSOEntryWidget->getTime()).toString("hh:mm:ss");    

    QString trsttx = rstTXLineEdit->text();
          //qDebug() << "MainWindow::readDataFromUIDX - RSTtx: " << trsttx << endl;
    QString trstrx = rstRXLineEdit->text();

    int dxcc = world->getQRZARRLId(tqrz);
    //int dxcc2 = getDXCCFromComboBox();
    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
             //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) << endl;
             //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) << endl;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);
    //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) << endl;
    //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) << endl;

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
        else
        {}
    }

    aux1 = dataProxy->getContinentShortNameFromEntity(dxcc);
    if (dataProxy->isValidContinentShortName(aux1))
    {
        stringFields = stringFields + ", cont";
        stringData = stringData + ", '" + aux1 + "'";
    }

    int cqz = world->getEntityCqz(dxcc);
    int ituz = world->getEntityItuz(dxcc);

    aux1 = nameLineEdit->text();
    if (aux1.length()>1)
    {
        stringFields = stringFields + ", name";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = (locatorLineEdit->text()).toUpper();

    if ( locator->isValidLocator(aux1)  )
    {
        stringFields = stringFields + ", gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }

           //qDebug() << "MainWindow::readDataFromUIDX: Reading freq...: " << QString::number(txFreqSpinBox->value()) << endl;
    if ( (txFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(txFreqSpinBox->value());        
              //qDebug() << "MainWindow::readDataFromUIDX: Reading freq...: " << aux1 << "/" << tband << endl;

        if (dataProxy->isThisFreqInBand(dataProxy->getNameFromBandId(tband), aux1) )
        {
            stringFields = stringFields + ", freq";
            stringData = stringData + ", '" + aux1 + "'";
                      //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND OK" << endl;
        }
        else
        {
                      //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND NOK" << endl;
        }
    }

    if ( (rxFreqSpinBox->value()) > 0  )
    {
          //qDebug() << "MainWindow::readDataFromUIDX: TX FREQ & RX FREQ ARE DIFFERENT AND != 0" << endl;
        aux1 = QString::number(rxFreqSpinBox->value());        
        stringFields = stringFields + ", freq_rx, band_rx";
        stringData = stringData + ", '" + aux1 + "', '" + QString::number(dataProxy->getBandIdFromFreq(rxFreqSpinBox->value())) + "'";
        //stringData = stringData + ", '" + aux1 + ", " + QString::number(dataProxy->getBandIdFromFreq(rxFreqSpinBox->value())) + "'";
    }

    aux1 = qthLineEdit->text();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", qth";
        stringData = stringData + ", '" + aux1 + "'";
    }   

    aux1 = myDataTabWidget->getOperator();
    if (aux1.length()>2)
    {
        //lastOperatorQRZ = aux1.toUpper();
        stringFields = stringFields + ", operator";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getStationQRZ();
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

    aux1 = QString::number(rxPowerSpinBox->value());
    if ((aux1.toFloat())>0.0f)
    {
        stringFields = stringFields + ", rx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = othersTabWidget->getIOTA();
    //qDebug() << "MainWindow::readDataFromUIDX: IOTA: " << aux1 << endl;
    if (aux1.length() == 6) // EU-001
    {
    //qDebug() << "MainWindow::readDataFromUIDX: IOTA to be saved" << endl;
        stringFields = stringFields + ", iota";
        stringData = stringData + ", '" + aux1 + "'";
    }

    // SATS
    aux1 = satTabWidget->getSatName(); //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
    //qDebug() << "MainWindow::readDataFromUIDX: SAT1 " << aux1 << endl;
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_name";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_mode";
        stringData = stringData + ", '" + aux1 + "'";
    }

    keepSatPage = satTabWidget->getRepeatThis();

    aux1 = othersTabWidget->getPropModeFromComboBox();
    //qDebug() << "MainWindow::readDataFromUIDX: PropMode:  " << aux1 << endl;
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

    //QSLTABWidget
    // QSL SENT: Y/N/R/Q/I
    // QSL_VIA: B/D/E/M

    aux1 = QSLTabWidget->getQSLSenStatus();
    aux2 = QSLTabWidget->getSentVia();
    //qDebug() << "MainWindow::readDataFromUIDX: aux1: " << aux1 << " / aux2: " << aux2 << endl;

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
    //stringData += QString(", '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8'").arg(tqrz).arg(tband).arg(tmode).arg(tdate).arg(ttime).arg(QString::number(currentLog)).arg(trsttx).arg(trstrx);
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
    //qDebug() << "MainWindow::readDataFromUIDXModifying:" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString tqrz = (mainQSOEntryWidget->getQrz()).toUpper();
    if (!util->isValidCall(tqrz))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog - Not valid call"));
        QString aux = QString(tr("The call %1 is not a valid call. Do you really want to add this call to the log?") ).arg(tqrz);
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

    //qDebug() << "MainWindow::readDataFromUIDXModifying: " << tqrz << endl;

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

    //    QString ttime = (mainQSOEntryWidget->getTime()).toString("hh:mm:ss");

    QString trsttx = rstTXLineEdit->text();
    QString trstrx = rstRXLineEdit->text();

    int dxcc = world->getQRZARRLId(tqrz);
    int cqz = world->getEntityCqz(dxcc);
    int ituz = world->getEntityItuz(dxcc);

    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
    //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) << endl;
    //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) << endl;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);
    //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) << endl;
    //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) << endl;

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
            //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 2: " << QString::number(dxcc2) << endl;
        }
        else if (ret == QMessageBox::ActionRole)
        {
            //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 1: " << endl;
        }
        else if (ret == QMessageBox::Cancel)
        {
            logEvent(Q_FUNC_INFO, "END-2", logSeverity);
            //qDebug() << "MainWindow::readDataFromUIDXModifying - Button 2: " << QString::number(dxcc2) << endl;
            return  "NULL";
        }
        else
        {}
    }



    /**/


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

    aux1 = nameLineEdit->text();
    if (aux1.length()>1)
    {
        updateString = updateString + "name = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "name = '', ";
    }

    aux1 = (locatorLineEdit->text()).toUpper();
    if ( locator->isValidLocator(aux1)  )
    {
        updateString = updateString + "gridsquare = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "gridsquare = '', ";
    }

    if ( (txFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(txFreqSpinBox->value());

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

    if ( (rxFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(rxFreqSpinBox->value());
        updateString = updateString + "freq_rx = '";
        updateString = updateString + aux1 + "', ";

        updateString = updateString + "band_rx = '";
        updateString = updateString + QString::number(dataProxy->getBandIdFromFreq(rxFreqSpinBox->value())) + "', ";
    }
    else
    {
        updateString = updateString + "freq_rx = '0', ";
        updateString = updateString + "band_rx = '', ";
    }

    aux1 = qthLineEdit->text();
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
    //aux1 = operatorLineEdit->text();
    if (util->isValidCall(aux1))
    {
        updateString = updateString + "operator = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "operator = '', ";
    }

    aux1 = myDataTabWidget->getStationQRZ();
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

    aux1 = commentTabWidget->getComment();
    updateString = updateString + "comment = '";
    updateString = updateString + aux1 + "', ";
    //aux1 = commentLineEdit->text();


    aux1 = QSLTabWidget->getQSLMsg();
    //aux1 = qslmsgTextEdit->toPlainText();
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

    //qDebug() << "MainWindow::readDataFromUIDXModifying: DXCC=" << aux1 << endl;
    if (dataProxy->isValidDXCC(dxcc))
    {
        updateString = updateString + "dxcc = '";
        updateString = updateString + aux1 + "', ";
                  //qDebug() << "MainWindow::readDataFromUIDXModifying: Saving DXCC=" << aux1 << endl;
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


    if (rxPowerSpinBox->value()>0.0)
    {
        aux1 = QString::number(rxPowerSpinBox->value());
        updateString = updateString + "rx_pwr = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "rx_pwr = '', ";
    }

    aux1 = othersTabWidget->getIOTA();
             //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA: " << aux1 << endl;
    if (aux1.length() == 6) // EU-001
    {
                 //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA to be saved! " << endl;
        updateString = updateString + "iota = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "iota = '', ";
                //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) << endl;
    }

   aux1 = satTabWidget->getSatName();   //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
             //qDebug() << "MainWindow::readDataFromUIDX: SAT2 modif " << aux1 << endl;
    if (aux1.length()>0)
    {
        updateString = updateString + "sat_name = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "sat_name = '', ";
    }

    aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
    if (aux1.length()>0)
    {
        updateString = updateString + "sat_mode = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
        updateString = updateString + "sat_mode = '', ";
    }

    aux1 = othersTabWidget->getPropModeFromComboBox();
    //qDebug() << "MainWindow::readDataFromUIDX: PropMode:  " << aux1 << endl;
    if ((aux1.length()>0) && (aux1 != "Not"))
    {
                  //qDebug() << "MainWindow::readDataFromUIDX: PropMode(1):  " << aux1 << endl;
        updateString = updateString + "prop_mode = '";
        updateString = updateString + aux1 + "', ";
    }
    else if ((aux1.length()==0) || (aux1 == "Not"))
    {
        //qDebug() << "MainWindow::readDataFromUIDX: PropMode(2):  " << aux1 << endl;
        updateString = updateString + "prop_mode = '',";
    }
    else
    {
        updateString = updateString + "prop_mode = '', ";
                 //qDebug() << "MainWindow::readDataFromUIDX: PropMode(3):  " << aux1 << endl;
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

    // QSL SENT
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    //int i = qslSentComboBox->currentIndex();
    //int ii = qslSentViaComboBox->currentIndex();
    aux1 = QSLTabWidget->getQSLSenStatus();
    aux2 = QSLTabWidget->getSentVia();
             //qDebug() << "MainWindow::readDataFromUIDXModifying: aux1: " << aux1 << " / aux2: " << aux2 << endl;

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
    //qDebug() << "MainWindow::readDataFromUIDXModifying: queryCreated: " << stringQuery << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return stringQuery;
}

void MainWindow::slotOKButtonClicked(){
            //qDebug() << "MainWindow::slotOKButtonClicked: "  << endl;
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
    //qDebug() << "MainWindow::slotQSOsExportToADIF " << QString::number(_id.length())  << endl;
    if (_id.length()<1)
    {
        return; // NO QSO TO EXPORT
    }
   /*
     int i=0;
    foreach(i, _id)
    {
        //qDebug() << "MainWindow::slotQSOsExportToADIFF " << QString::number(i)  << endl;
    }
º*/
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    //qDebug() << "MainWindow::slotQSOsExportToADIF: " << fileName << endl;
    if ((!fileName.endsWith(".adi")) && ( !fileName.endsWith(".adif") ))
    {
       //qDebug() << "MainWindow::slotQSOsExportToADIF: Adding the .adi to the file" << fileName << endl;
        fileName = fileName +  ".adi";
    }
    //qDebug() << "MainWindow::slotQSOsExportToADIF-1: " << fileName << endl;
    filemanager->adifQSOsExport(fileName, _id);
    //qDebug() << "MainWindow::slotQSOsExportToADIF-3" << endl;
    showNumberOfSavedQSO(fileName, _id.count());
    //qDebug() << "MainWindow::slotQSOsExportToADIF - END" << endl;
}

void MainWindow::slotQSOsDelete(QList<int> _id)
{
    //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(_id.length())  << endl;
    //foreach(int i, _id)
    //{
    //    //qDebug() << "MainWindow::slotQSOsDelete " << QString::number(i)  << endl;
    //}

    QString message = QString(tr("You have requested to delete several QSOs "));
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(message);
    msgBox.setDetailedText(tr("This operation shall remove definetly all the selected QSO and associated data and you will not be able to recover it again."));
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
                //qDebug() << "MainWindow::slotQSODelete: Just removed from log, now I will try to remove from ClubLog, if needed" << endl;
                if (clublogActive && clublogRealTime)
                {
                    //qDebug() << "MainWindow::slotQSODelete: Removing from ClubLog" << endl;
                    elogClublog->deleteQSO(qsoToDelete);
                }
                else
                {
                    //qDebug() << "MainWindow::slotQSODelete: NOT emoving from ClubLog" << endl;
                }
            }
        }
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
   //qDebug() << "MainWindow::slotQSODelete " << QString::number(_id)  << endl;

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
                //qDebug() << "MainWindow::slotQSODelete: Just removed from log, now I will try to remove from ClubLog, if needed" << endl;
                if (clublogActive && clublogRealTime)
                {
                    //qDebug() << "MainWindow::slotQSODelete: Removing from ClubLog" << endl;
                    elogClublog->deleteQSO(qsoToDelete);
                }
                else
                {
                    //qDebug() << "MainWindow::slotQSODelete: NOT emoving from ClubLog" << endl;
                }

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










   //qDebug() << "MainWindow::slotQSODelete END "  << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //awards->recalculateAwards();
}

void MainWindow::slotShowSearchWidget()
{
    //dxUpRightTab->addTab(searchWidget, tr("Search"));
           //qDebug() << "MainWindow::slotShowSearchWidget: " << QString::number(dxUpRightTab->indexOf(searchWidget)) << endl;
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
             //qDebug() << "MainWindow::slotElogClubLogDisable: " << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (_b)
    {
        clublogActive = false;
        setupDialog->setClubLogActive(false);

    }
    else
    {
        clublogActive = true;
        setupDialog->setClubLogActive(true);
    }
    //TODO: Disable clublog in the klogrc file
    //bool FileManager::modifySetupFile(const QString& _filename, const QString _field, const QString _value)
    filemanager->modifySetupFile(configFileName, "ClubLogActive", "False");
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotElogClubLogFileUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos)
{

    //qDebug() << "MainWindow::slotElogClubLogFileUploaded: " << QString::number(_reply) << endl;

    QMessageBox msgBox;
    if (_error != QNetworkReply::NoError)
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog ClubLog error"));
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
          //qDebug() << "MainWindow::slotElogClubLogFileUploaded file exist" << endl;
     }
     else
     {
         //qDebug() << "MainWindow::slotElogClubLogFileUploaded file DOES NOT exist" << endl;
     }

     msgBox.setIcon(QMessageBox::Question);
     msgBox.setWindowTitle(tr("KLog ClubLog"));
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

    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("KLog ClubLog"));
    msgBox.setText(tr("The ClubLog upload process has finished and KLog created a file (%1) in your KLog folder.\n\nDo you want KLog to remove that file?").arg(fileName));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::Yes);
    i = msgBox.exec();
    if (i == QMessageBox::Yes)
    {
        if (QFile::remove(fileName))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog ClubLog"));
            msgBox.setText(tr("The file has been removed."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogClubLogFileUploaded - FILE REMOVED: " << fileName << endl;
        }
        else
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog ClubLog"));
            msgBox.setText(tr("The file has not been removed."));
            msgBox.setDetailedText(tr("It seems that there was something that prevented KLog from removing the file\nYou can remove it manually."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogClubLogFileUploaded - FILE NOT REMOVED: " << fileName << endl;
        }
        i = msgBox.exec();
    }
}

void MainWindow::slotElogClubLogShowMessage(const QString &_s)
{
    //qDebug() << "MainWindow::slotElogClubLogShowMessage: " << _s << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    slotUpdateStatusBar(_s);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotElogClubLogProcessAnswer(const int _i, const int _qID)
{
             //qDebug() << "MainWindow::slotElogClubLogProcessAnswer: " <<QString::number(_i) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    clublogAnswer = _i;

    if (clublogAnswer == 0) // NO ERROR
    {

        //dataProxy->setClubLogSent(_qID, "Y", (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd"));

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

    //qDebug() << "MainWindow::slotElogEQSLFileUploaded: " << QString::number(_error) << endl;

    QMessageBox msgBox;
    if (_error != QNetworkReply::NoError)
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog eQSL error"));
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
          //qDebug() << "MainWindow::slotElogEQSLFileUploaded file exist" << endl;
     }
     else
     {
         //qDebug() << "MainWindow::slotElogEQSLFileUploaded file DOES NOT exist" << endl;
     }

     msgBox.setIcon(QMessageBox::Question);
     msgBox.setWindowTitle(tr("KLog eQSL"));
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

    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("KLog eQSL"));
    msgBox.setText(tr("The eQSL upload process has finished and KLog created a file (%1) in your KLog folder.\n\nDo you want KLog to remove that file?").arg(fileName));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::Yes);
    i = msgBox.exec();
    if (i == QMessageBox::Yes)
    {
        if (QFile::remove(fileName))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog eQSL"));
            msgBox.setText(tr("The file has been removed."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogEQSLFileUploaded - FILE REMOVED: " << fileName << endl;
        }
        else
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog eQSL"));
            msgBox.setText(tr("The file has not been removed."));
            msgBox.setDetailedText(tr("It seems that there was something that prevented KLog from removing the file\nYou can remove it manually."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);

            //qDebug() << "MainWindow::slotElogEQSLFileUploaded - FILE NOT REMOVED: " << fileName << endl;
        }
        i = msgBox.exec();
    }
    //qDebug() << "MainWindow::slotElogEQSLFileUploaded - END"  << endl;
}



void MainWindow::slotElogQRZCOMShowMessage(const QString &_s)
{
    //qDebug() << "MainWindow::slotElogQRZCOMShowMessage: " << _s << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    slotUpdateStatusBar(_s);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotElogQRZCOMFoundData(const QString &_t, const QString & _d)
{
   //qDebug() << "MainWindow::slotElogQRZCOMFoundData: " << _t << "/" << _d << endl;
   if (_t == "name")
   {
        nameLineEdit->setText(_d);
   }
   else if (_t == "grid")
   {
        locatorLineEdit->setText(_d);
   }
   else if (_t == "qth")
   {
        qthLineEdit->setText(_d);
   }
   else if (_t == "qslmgr")
   {
        //QSLTabWidget->setQSLVia(_d);
   }
   else if (_t == "error")
   {

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
       //qDebug() << "MainWindow::slotElogQRZCOMFoundData: QRZ value not recognized" << endl;
   }
}

void MainWindow::slotElogQRZCOMCheckThisCall()
{
  //qDebug() << "MainWindow::slotElogQRZCOMCheckThisCall: " << endl;
   elogQRZcom->checkQRZ(mainQSOEntryWidget->getQrz());
}

void MainWindow::slotElogQRZCOMAutoCheck()
{
   //qDebug() << "MainWindow::slotElogQRZCOMAutoCheck: " << util->boolToQString(QRZCOMAutoCheckAct->isChecked()) << endl;
    setupDialog->setQRZCOMAutoCheckActive(QRZCOMAutoCheckAct->isChecked());
}
void MainWindow::slotElogQRZCOMAutoCheckFromSetup(const bool _s)
{
    QRZCOMAutoCheckAct->setEnabled(_s);
}

void MainWindow::slotExitFromSlotDialog(const int exitID)
{
    //qDebug()() << "MainWindow::slotExitFromSlotDialog: " << QString::number(exitID) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (exitID == 2)
    {
        needToEnd = true;
        exitQuestion();
    }
    //qDebug()() << "MainWindow::slotExitFromSlotDialog: END "  << endl;
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
    //qDebug()() << "MainWindow::exitQuestion"  << endl;
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
        logEvent(Q_FUNC_INFO, "Exiting KLog!", 0);
            saveWindowsSize();
            exit(0);
        default:
            // should never be reached
        break;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
bool MainWindow::validCharactersInCall(const QString &_qrz)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    for (int i = 0; i<_qrz.size();i++)
    {
        if (!( ((_qrz.at(i)).isLetterOrNumber()) || (_qrz.at(i)=='\\')  || (_qrz.at(i)=='/')  ))
        {
            logEvent(Q_FUNC_INFO, "END-1", logSeverity);
            return false;
        }

    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return true;
}
*/

void MainWindow::slotQRZTextChanged(QString _qrz)
{
    //qDebug()<< "MainWindow::slotQRZTextChanged: " << _qrz << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (_qrz.length()<1)
    {
        infoLabel1->clear();
        infoLabel2->clear();
        slotClearButtonClicked();
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }   
        //qDebug()<< "MainWindow::slotQRZTextChanged: cursor position: " << endl;

    if (cleaning)
    {
               //qDebug()<< "MainWindow::slotQRZTextChanged: Cleaning" << endl;
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

     //qDebug()<< "MainWindow::slotQRZTextChanged: checking for modify or length<1" << endl;
    if (qrzSmallModDontCalculate)
    //if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
                  //qDebug() << "MainWindow::slotQRZTextChanged: MODIFY or Lenght < 1" << endl;
        qrzSmallModDontCalculate=false;
        logEvent(Q_FUNC_INFO, "END-6", logSeverity);
        return;
    }

    //qDebug()<< "MainWindow::slotQRZTextChanged: running..." << endl;
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    //int i;
    int dx_CQz = -1;
    int dxE_CQz = -1;
    int dx_ITUz = -1;
    int dxE_ITUz = -1;

    //qDebug() << "MainWindow::slotQRZTextChanged: currentQRZ: "  << endl;
    currentEntity = world->getQRZARRLId(_qrz);
    //selectCorrectComboBoxEntity(currentEntity);
          //qDebug() << "MainWindow::slotQRZTextChanged: currentEntity: " << QString::number(currentEntity) << endl;
    othersTabWidget->setEntity(currentEntity);

    dxE_CQz = world->getEntityCqz(currentEntity);
    dx_CQz = world->getQRZCqz(_qrz);
    dx_ITUz = world->getQRZItuz(_qrz);
    dxE_ITUz = world->getEntityItuz(currentEntity);
          //qDebug()<< "MainWindow::slotQRZTextChanged: CQ: " << QString::number(dx_CQz) << endl;
          //qDebug()<< "MainWindow::slotQRZTextChanged: CQe: " << QString::number(dxE_CQz) << endl;
          //qDebug()<< "MainWindow::slotQRZTextChanged: ITU: " << QString::number(dx_ITUz) << endl;
          //qDebug()<< "MainWindow::slotQRZTextChanged: ITUe: " << QString::number(dxE_ITUz) << endl;

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
          //qDebug() << "MainWindow::slotQRZTextChanged: currentEntity: " << QString::number(currentEntity) << endl;
    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
    }
    else
    {
        dxLocator = world->getLocator(currentEntity);
    }

          //qDebug() << "MainWindow::slotQRZTextChanged: Going to check the DXCC" << endl;
        //qDebug() << "MainWindow::slotQRZTextChanged: - current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) << endl;
        if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) || (dx_CQz != dxE_CQz) || (dx_ITUz != dxE_ITUz))
        {
                  //qDebug() << "MainWindow::slotQRZTextChanged: currentEntity=" << QString::number(currentEntity) << "/previousEntity=" << QString::number(previousEntity)  << endl;
            previousEntity = currentEntity;
            InValidCharsInPrevCall = false;
            //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
            infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
                  //qDebug() << "MainWindow:: - calling showStatusOfDXCC-03 " << endl;
            showStatusOfDXCC(_qs);
            showDXMarathonNeeded(currentEntity, dx_CQz, mainQSOEntryWidget->getDate().year(), currentLog);
            othersTabWidget->setIOTAContinentFromEntity(currentEntity);
        }
        else if ((dx_CQz == dxE_CQz) || (dx_ITUz = dxE_ITUz))
        {
                  //qDebug() << "MainWindow::slotQRZTextChanged: 000" << endl;
            //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
        }
        else
        {
                          //qDebug() << "MainWindow::slotQRZTextChanged: Default: else" << endl;
        }



    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
           //qDebug() << "MainWindow::slotQRZTextChanged: cursorP at the end : "  << endl;

    completeWithPreviousQSO(_qrz);
    searchWidget->setCallToSearch(_qrz);
    if (qrzcomActive && QRZCOMAutoCheckAct->isChecked())
    {
        elogQRZcom->checkQRZ(_qrz);
    }

    //qrzAutoChanging = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::slotQRZTextChanged: END" << endl;
}

/*
void MainWindow::slotQRZSpacePressed()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
               //qDebug() << "MainWindow::slotQRZSpacePressed: "  << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
*/
void MainWindow::setRSTToMode(const QString &_m)
{

          //qDebug() << "MainWindow::setRSTToMode: " << _m << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (readingTheUI)
    {
        logEvent(Q_FUNC_INFO, "END", logSeverity);
        return;
    }
    if ((_m == "SSB") || (_m== "LSB") || (_m=="USB") || (_m == "FM") || (_m == "AM") )
    {
              //qDebug() << "MainWindow::setRSTToMode: Detected SSB/LSB/USB"  << endl;
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

    rstTXLineEdit->setText(util->getDefaultRST(_m));
    rstRXLineEdit->setText(util->getDefaultRST(_m));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
void MainWindow::slotrstTXTextChanged()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotrstRXTextChanged()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
*/
/*

void MainWindow::slotSpotItButtonClicked()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!dxClusterWidget->isConnected())
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
*/
void MainWindow::setCleaning(const bool _c)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    cleaning = _c;
    mainQSOEntryWidget->setCleaning(cleaning);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotClearButtonClicked()
{
          //qDebug() << "MainWindow::slotClearButtonClicked - START" << endl;
           //qDebug() << "MainWindow::slotClearButtonClicked: " << mainQSOEntryWidget->getMode() << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    setCleaning(true);
    yearChangedDuringModification = false;
    setModifying(false);

    dateTimeTemp = dateTime;
    modifyingQSO = -1;

    setRSTToMode(mainQSOEntryWidget->getMode());

    if (currentBand < 0)
    {
        currentBand = defaultBand;
    }
    if (currentMode < 0)
    {
        currentMode = defaultMode;
      //qDebug() << "MainWindow::MainWindow: 12b - currentMode: " << QString::number(currentMode) << endl;
    }

    clublogAnswer = -1;
    clublogPrevQSO.clear();
          //qDebug() << "MainWindow::slotClearButtonClicked: - 11"  << endl;
    mainQSOEntryWidget->clear();

    clearUIDX(true);
    //completedWithPreviousName = false;
    statusBar()->clearMessage();
    setCleaning(false);

    //qDebug() << "MainWindow::slotClearButtonClicked: Log: " << QString::number(currentLog) << endl;
    setMainWindowTitle(QString::number(dataProxy->getHowManyQSOInLog(currentLog)));

              //qDebug() << "MainWindow::slotClearButtonClicked: " << mainQSOEntryWidget->getMode() << endl;
              //qDebug() << "MainWindow::slotClearButtonClicked - currentMode = " << QString::number(currentMode) << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::slotClearButtonClicked - END" << endl;
}

void MainWindow::clearUIDX(bool full)
{
    //qDebug() << "MainWindow::clearUIDX" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //SRXLineEdit->setText("59");
    //STXLineEdit->setText("59");
    qthLineEdit->clear();
    nameLineEdit->clear();
    locatorLineEdit->clear();
    commentTabWidget->clear();
    infoLabel1->clear();
    infoLabel2->clear();
    rxPowerSpinBox->setValue(0);
    eQSLTabWidget->clear();
    QSLTabWidget->clear();
    othersTabWidget->clear();
    infoWidget->clear();

    satTabWidget->clear();
    myDataTabWidget->clear(keepMyData);
    if (txFreqSpinBox->value()<=0)
    {
        txFreqSpinBox->setValue((dataProxy->getFreqFromBandId(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand()))).toDouble());
        rxFreqSpinBox->setValue(txFreqSpinBox->value());
    }
    //if (full)
    //{
    //    txFreqSpinBox->setValue((dataProxy->getFreqFromBandId(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand()))).toDouble());
    //    rxFreqSpinBox->setValue(0);
    //}
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::clearUIDX - END" << endl;

}


void MainWindow::slotRefreshDXCCWidget()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    dxccStatusWidget->slotRefreshButtonClicked();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //qDebug()() << "MainWindow::closeEvent" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (maybeSave())
    {
        //qDebug()() << "MainWindow::closeEvent saving needed" << endl;
        dataProxy->unMarkAllQSO();
        //slotFileClose();
        dataProxy->compressDB();
        //db->compress();
        saveWindowsSize();
        event->accept();
    }
    else
    {
        //qDebug()() << "MainWindow::closeEvent not saving needed" << endl;
        event->ignore();
    }
    //qDebug()() << "MainWindow::closeEvent-END" << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

bool MainWindow::maybeSave()
{
              //qDebug() << "MainWindow::maybeSave" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QString str = tr("The logfile has been modified.") + "\n" + tr("Do you want to save your changes?");

    if (alwaysADIF)
    {
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

                    //return saveFile(defaultADIFLogFile);
                    return !(filemanager->adifLogExport(defaultADIFLogFile, currentLog));
                }
                else
                {
                    logEvent(Q_FUNC_INFO, "END-2", logSeverity);
                    slotADIFImport();
                    return true;
                    //return saveFileAs();
                }
            }

         else if (ret == QMessageBox::Cancel)
            {
                logEvent(Q_FUNC_INFO, "END-3", logSeverity);
                return false;
            }
        }
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return true;
}

void MainWindow::slotAWAImport()
{
    //qDebug() << "MainWindow::slotAWAImport" << endl;
    fileAwardManager->importNewAwardFile();
    //qDebug() << "MainWindow::slotAWAImport - END" << endl;
}

void MainWindow::createMenusCommon()
{
         //qDebug() << "MainWindow::createMenusCommon" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    fileMenu = menuBar()->addMenu(tr("&File"));

    //awardAddAct = new QAction(tr("Import Award definition file..."), this);
    //fileMenu->addAction(awardAddAct);
    //connect(awardAddAct , SIGNAL(triggered()), this, SLOT(slotAWAImport()));
    //awardAddAct ->setToolTip(tr("Import an Award file."));


    ADIFImport = new QAction(tr("&Import from ADIF..."), this);
    fileMenu->addAction(ADIFImport);
    connect(ADIFImport, SIGNAL(triggered()), this, SLOT(slotADIFImport()));
    ADIFImport->setToolTip(tr("Import an ADIF file into the current log."));

    //LoTWImport = new QAction(tr("&Import from LoTW..."), this);
    //fileMenu->addAction(LoTWImport);
    //connect(LoTWImport, SIGNAL(triggered()), this, SLOT(slotLoTWImport()));
    //LoTWImport->setToolTip(tr("Import an LoTW file into the current log"));

    fileMenu->addSeparator();
    /*
    saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
    saveAct->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    */

    fileMenu->addSeparator();

    ADIFExport = new QAction(tr("Export to ADIF..."), this);
    fileMenu->addAction(ADIFExport);
    //ADIFExport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFExport, SIGNAL(triggered()), this, SLOT(slotADIFExport()));
    ADIFExport->setToolTip(tr("Export the current log to an ADIF logfile."));

    ADIFExportAll = new QAction(tr("Export all logs to ADIF..."), this);
    fileMenu->addAction(ADIFExportAll);
    //ADIFExport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFExportAll, SIGNAL(triggered()), this, SLOT(slotADIFExportAll()));
    ADIFExportAll->setToolTip(tr("Export ALL the QSOs into one ADIF file, merging QSOs from all the logs."));

    //ReqQSLExport = new QAction(tr("Export Requested QSL to ADIF..."), this);
    //fileMenu->addAction(ReqQSLExport);
    //connect(ReqQSLExport, SIGNAL(triggered()), this, SLOT(slotRQSLExport()));
    //ReqQSLExport->setToolTip(tr("Export all QSOs requesting QSLs to an ADIF file (e.g. to import it into a QSL tag printing program)."));

    //LoTWExport = new QAction(tr("Export ADIF for LoTW..."), this);
    //fileMenu->addAction(LoTWExport);
    //connect(LoTWExport, SIGNAL(triggered()), this, SLOT(slotLoTWExport()));
    //LoTWExport->setToolTip(tr("Export an ADIF file to be sent to LoTW. Remember to sign it with TQSL before uploading to LoTW!"));

    fileMenu->addSeparator();

    printLogAct = new QAction(tr("&Print Log..."), this);
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

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    exitAct->setMenuRole(QAction::QuitRole);
    exitAct->setShortcut(Qt::CTRL + Qt::Key_X);
    //connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(slotFileClose()));

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    fillQsoAct = new QAction(tr("Fill in QSO data"), this);
    toolMenu->addAction(fillQsoAct);
    //fillQsoAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(fillQsoAct, SIGNAL(triggered()), this, SLOT(fillQSOData()));
    fillQsoAct->setToolTip(tr("Go through the log reusing previous QSOs to fill missing information in other QSOs."));

    //fillDXCCAct = new QAction(tr("Fill in DXCC data"), this);
    //toolMenu->addAction(fillDXCCAct);
    //connect(fillDXCCAct, SIGNAL(triggered()), this, SLOT(slotFillEmptyDXCCInTheLog()));
    //fillDXCCAct->setToolTip(tr("Go through the log filling QSOs without a DXCC defined."));

    toolMenu->addSeparator();
    qslToolMenu = toolMenu->addMenu(tr("QSL tools..."));

    //findQSO2QSLAct = new QAction(tr("&Find QSO to QSL"), this);
    //toolMenu->addAction(findQSO2QSLAct);
    //connect(findQSO2QSLAct, SIGNAL(triggered()), this, SLOT(slotSearchToolNeededQSLToSend()));
    //findQSO2QSLAct->setToolTip(tr("Shows QSOs for which you should send your QSL and request the DX QSL"));

    findQSO2QSLAct = new QAction(tr("&Find QSO to QSL"), this);
    qslToolMenu->addAction(findQSO2QSLAct);
    connect(findQSO2QSLAct, SIGNAL(triggered()), this, SLOT(slotSearchToolNeededQSLToSend()));
    findQSO2QSLAct->setToolTip(tr("Shows QSOs for which you should send your QSL and request the DX QSL."));

    findRequestedQSLAct = new QAction(tr("Find My-QSLs pending to send"), this);
    qslToolMenu->addAction(findRequestedQSLAct);
    //findQSO2QSLAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(findRequestedQSLAct, SIGNAL(triggered()), this, SLOT(slotToolSearchRequestedQSLToSend()));
    findRequestedQSLAct->setToolTip(tr("Shows the QSOs with pending requests to send QSLs. You should keep this queue empty!"));

    findQSLPendingToReceiveAct = new QAction(tr("&Find DX-QSLs pending to receive"), this);
    qslToolMenu->addAction(findQSLPendingToReceiveAct);
    connect(findQSLPendingToReceiveAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLPendingToReceive()));
    findQSLPendingToReceiveAct->setToolTip(tr("Shows DX-QSLs for which requests or QSLs have been sent with no answer."));

    findQSLDXRequestedAct = new QAction(tr("&Find requested pending to receive"), this);
    qslToolMenu->addAction(findQSLDXRequestedAct);
    connect(findQSLDXRequestedAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLRequested()));
    findQSLDXRequestedAct->setToolTip(tr("Shows the DX-QSLs that have been requested."));

    toolMenu->addSeparator();
    lotwToolMenu = toolMenu->addMenu(tr("LoTW tools..."));

    lotwMarkSentQueuedThisLogAct = new QAction(tr("Queue all QSLs from this log to be sent"), this);
    lotwToolMenu->addAction(lotwMarkSentQueuedThisLogAct);
    connect(lotwMarkSentQueuedThisLogAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllQueuedThisLog()));
    lotwMarkSentQueuedThisLogAct->setToolTip(tr("Mark all non-sent QSOs in this log as queued to be uploaded."));

    lotwMarkSentQueuedAct = new QAction(tr("Queue all QSLs to be sent"), this);
    lotwToolMenu ->addAction(lotwMarkSentQueuedAct);
    connect(lotwMarkSentQueuedAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllQueued()));
    lotwMarkSentQueuedAct->setToolTip(tr("Put all the non-sent QSOs in the queue to be uploaded."));

    lotwToolMenu->addSeparator();

    lotwMarkSentYesThisLogAct = new QAction(tr("Mark as sent all queued QSOs of this log"), this);
    lotwToolMenu->addAction(lotwMarkSentYesThisLogAct);
    connect(lotwMarkSentYesThisLogAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllYesThisLog()));
    lotwMarkSentYesThisLogAct->setToolTip(tr("Mark all queued QSOs in this log as sent to LoTW."));

    lotwMarkSentYesAct = new QAction(tr("Mark all queued QSOs as sent"), this);
    lotwToolMenu ->addAction(lotwMarkSentYesAct);
    connect(lotwMarkSentYesAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllYes()));
    lotwMarkSentYesAct->setToolTip(tr("Mark all queued QSOs as sent to LoTW."));

    lotwToolMenu ->addAction(lotwCallTQSL);
    connect(lotwCallTQSL, SIGNAL(triggered()), this, SLOT(slotLoTWExport()));
    lotwCallTQSL->setToolTip("Sends the log to LoTW calling TQSL. You will be able to select the Station Callsign and start and end dates.");

    lotwToolMenu->addSeparator();
    lotwUpdateFromLoTWAct = new QAction(tr("Download from LoTW"), this);
    lotwToolMenu ->addAction(lotwUpdateFromLoTWAct);
    connect(lotwUpdateFromLoTWAct, SIGNAL(triggered()), this, SLOT(slotLoTWDownload()));
    lotwUpdateFromLoTWAct->setToolTip("Updates your LoTW status from LoTW.");

    toolMenu->addSeparator();
    clublogToolMenu = toolMenu->addMenu(tr("ClubLog tools..."));

    clublogLogModifyCurrentLogAct = new QAction(tr("Mark all the QSO to be uploaded"), this);
    clublogToolMenu->addAction(clublogLogModifyCurrentLogAct);
    connect(clublogLogModifyCurrentLogAct, SIGNAL(triggered()), this, SLOT( slotElogClubLogModifyCurrentLog()));
    clublogLogModifyCurrentLogAct->setToolTip("Mark as modified all the QSO so they can be uploaded againto CLubLog.");

    clublogLogUploadAct = new QAction(tr("Upload the log to ClubLog"), this);
    clublogToolMenu->addAction(clublogLogUploadAct);
    connect(clublogLogUploadAct, SIGNAL(triggered()), this, SLOT(slotClubLogLogUpload()));
    clublogLogUploadAct->setToolTip("Uploads your log to ClubLog. Please ensure that you have created log for that callsign before uploading.");

    toolMenu->addSeparator();
    eQSLToolMenu = toolMenu->addMenu(tr("eQSL tools..."));

    eqslLogModifyCurrentLogAct = new QAction(tr("Mark all the QSO to be uploaded"), this);
    eQSLToolMenu->addAction(eqslLogModifyCurrentLogAct);
    connect(eqslLogModifyCurrentLogAct, SIGNAL(triggered()), this, SLOT( slotElogEQSLModifyCurrentLog()));
    eqslLogModifyCurrentLogAct->setToolTip("Mark as modified all the QSO so they can be uploaded again to eQSL.");

    eqslUploadAct = new QAction(tr("Upload the log to eQSL.cc"), this);
    eQSLToolMenu->addAction(eqslUploadAct);
    connect(eqslUploadAct, SIGNAL(triggered()), this, SLOT(sloteQSLLogUpload()));
    eqslUploadAct->setToolTip("Uploads your log to eQSL.cc.");

    toolMenu->addSeparator();
    QRZCOMToolMenu = toolMenu->addMenu(tr("QRZ.com tools..."));

    QRZCOMCheckThisCallAct = new QAction(tr("Check the current QRZ in QRZ.com"), this);
    QRZCOMToolMenu->addAction(QRZCOMCheckThisCallAct);
    connect(QRZCOMCheckThisCallAct, SIGNAL(triggered()), this, SLOT( slotElogQRZCOMCheckThisCall()));
    QRZCOMCheckThisCallAct->setToolTip("Checks the current QRZ in QRZ.com.");


    QRZCOMAutoCheckAct->setText(tr("Check always the current QRZ in QRZ.com"));
    QRZCOMToolMenu->addAction(QRZCOMAutoCheckAct);
    connect(QRZCOMAutoCheckAct, SIGNAL(triggered()), this, SLOT( slotElogQRZCOMAutoCheck()));
    QRZCOMAutoCheckAct->setToolTip("Mark as modified all the QSO so they can be uploaded againto eQSL.");

    toolMenu->addSeparator();

    downloadCTYAct = new QAction (tr("&Update cty.csv"), this);
    toolMenu->addAction(downloadCTYAct);
    //downloadCTYAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(downloadCTYAct, SIGNAL(triggered()), this, SLOT(slotUpdateCTYDAT()));
    downloadCTYAct->setToolTip(tr("For updated DX-Entity data, update cty.csv."));

    downloadSATSAct = new QAction (tr("&Update Satellite Data"), this);
    toolMenu->addAction(downloadSATSAct);
    connect(downloadSATSAct, SIGNAL(triggered()), this, SLOT(slotUpdateSATSDAT()));
    downloadSATSAct->setToolTip(tr("For updated DX-Entity data, update cty.csv."));

    toolMenu->addSeparator();

    showStatsAct = new QAction (tr("Stats"), this);
    toolMenu->addAction(showStatsAct);
    connect(showStatsAct, SIGNAL(triggered()), this, SLOT(slotShowStats()));
    showStatsAct->setToolTip(tr("Show the statistics of your radio activity."));

         //qDebug() << "MainWindow::createMenusCommon before" << endl;
    //toolMenu->addSeparator();
    //showRotatorAct = new QAction (tr("Rotator"), this);
    //toolMenu->addAction(showRotatorAct);
    //connect(showRotatorAct, SIGNAL(triggered()), this, SLOT(slotRotatorShow()));
    //showRotatorAct->setToolTip(tr("Show the rotator controller."));
         //qDebug() << "MainWindow::createMenusCommon after" << endl;

    //showWorldMapAct = new QAction(tr("CQ zones world map"), this);
    //toolMenu->addAction(showWorldMapAct);
    //connect(showWorldMapAct, SIGNAL(triggered()), this, SLOT(slotWorldMapShow()));
    //showWorldMapAct->setToolTip(tr("Show a world map with your radio activity."));

    toolMenu->addSeparator();

    setupMenu = menuBar()->addMenu(tr("&Setup"));

    setupAct = new QAction(tr("&Setup..."), this);
    setupMenu->addAction(setupAct);
    setupAct->setMenuRole(QAction::PreferencesRole);
    connect(setupAct, SIGNAL(triggered()), this, SLOT(slotSetup()));

    //TODO: To be added once the help dialog has been implemented
    helpMenu = menuBar()->addMenu(tr("&Help"));

    tipsAct = new QAction(tr("&Tips..."), this);
    helpMenu->addAction(tipsAct);
    tipsAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(tipsAct, SIGNAL(triggered()), this, SLOT(slotTipsAction()));

    helpMenu->addSeparator();

    aboutAct = new QAction(tr("&About..."), this);
    helpMenu->addAction(aboutAct);
    aboutAct->setMenuRole(QAction::AboutRole);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotHelpAboutAction()));    
    //connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotLoTWTest()) );

    aboutQtAct = new QAction(tr("About Qt..."), this);
    helpMenu->addAction(aboutQtAct);
    aboutQtAct->setMenuRole(QAction::AboutQtRole);
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(slotAboutQt()));
    logEvent(Q_FUNC_INFO, "END", logSeverity);

    helpMenu->addSeparator();

    updateAct = new QAction(tr("Check updates..."), this);
    helpMenu->addAction(updateAct);
    updateAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(updateAct, SIGNAL(triggered()), this, SLOT(slotHelpCheckUpdatesAction()));
 }
/*
void MainWindow::slotCloseStats(bool _vis)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    statsWidget->clear();
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
           //qDebug() << "MainWindow::slotToolLoTWMarkAllQueuedThisLog"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());


    if(dataProxy->lotwSentQueue(mainQSOEntryWidget->getDate(), currentLog))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All pending QSOs of this log has been marked as queued for LoTW!") + "\n\n" + tr("Now you can go to the File menu to export the LoTW ADIF file and upload it to LoTW."));
        msgBox.exec();

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all pending QSOs of this log as queued for LoTW!") );
        msgBox.exec();
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
 void MainWindow::slotLoTWTest()
{
     //qDebug() << "MainWindow::slotLoTWTest: "  << endl;
     slotLoTWDownloadedFileProcess("lotwimport.adi");
     //qDebug() << "MainWindow::slotLoTWTest - END"  << endl;
}
*/

void MainWindow::slotLoTWDownloadedFileProcess(const QString &_fn)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << "MainWindow::slotLoTWDownloadedFileProcess: " << _fn << endl;
    QList<int> a;
    a.clear();

    a.append(filemanager->adifLoTWReadLog(_fn, currentLog));
    QString aux;
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("KLog LoTW"));

    if (a.length()>0)
    {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Your log has been updated with the LoTW downloaded QSOs."));
        aux = QString(tr("KLog has updated %1 QSOs from LoTW.")).arg(a.length());
        msgBox.setInformativeText(aux);
        msgBox.exec();
        showAdifImportWidget->show();
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
           //qDebug() << "MainWindow::slotToolLoTWMarkAllQueued"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());

    if (dataProxy->lotwSentQueue(mainQSOEntryWidget->getDate(), -1))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All pending QSOs has been marked as queued for LoTW!") + "\n\n" +  tr("Now you can go to the File menu to export the LoTW ADIF file and upload it to LoTW."));
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all pending QSOs of this log as queued for LoTW!") );
        msgBox.exec();

    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

bool MainWindow::callTQSL(const QString &_filename, const QString &_call)
{ //https://lotw.arrl.org/lotw-help/cmdline/
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << "MainWindow::callTQSL: " << lotwTQSLpath << endl;

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
        //qDebug() << "MainWindow::callTQSL error: " << (QProcess::readAllStandardError()) << endl;
        //qDebug() << "MainWindow::callTQSL-ok: " << QString::number(ok) << endl;

        switch (ok)
        {
        case 0: // success: all qsos submitted were signed and saved or signed and uploaded
                  //qDebug() << "MainWindow::callTQSL: 0"  << endl;
            //msg = tr("All the QSOs were signed and uploaded with no error.");
            msg = tr("TQSL finished with no error.");
            msgBox.setIcon(QMessageBox::Information);
        break;
        case 1: // cancelled by user
                  //qDebug() << "MainWindow::callTQSL: 1"  << endl;
            msg = tr("Error #1: The process was cancelled by the user or TQSL was not configured. No QSOs were uploaded.");
        break;
        case 2: // rejected by LoTW
                  //qDebug() << "MainWindow::callTQSL: 2"  << endl;
            msg = tr("Error #2: Upload was rejected by LoTW, please check your data.");
            break;
        case 3: // unexpected response from TQSL server
                  //qDebug() << "MainWindow::callTQSL: 3"  << endl;
            msg = tr("Error #3: The TQSL server returned an unexpected response.");
            break;
        case 4: // TQSL error
                  //qDebug() << "MainWindow::callTQSL: 4"  << endl;
            msg = tr("Error #4: There was a TQSL error.");
            break;
        case 5: // TQSLlib error
                  //qDebug() << "MainWindow::callTQSL: 5"  << endl;
            msg = tr("Error #5: There was a TQSLLib error.");
            break;
        case 6: // unable to open input file
                  //qDebug() << "MainWindow::callTQSL: 6"  << endl;
            msg = tr("Error #6: It was not possible to open the input file.");
            break;
        case 7: // unable to open output file
                  //qDebug() << "MainWindow::callTQSL: 7"  << endl;
            msg = tr("Error #7: It was not possible to open the ouput file.");
            break;
        case 8: // No QSOs were processed since some QSOs were duplicates or out of date range
                  //qDebug() << "MainWindow::callTQSL: 8"  << endl;
            msg = tr("Error #8: No QSOs were processed since some QSOs were duplicates or out of date range.");
            break;
        case 9: // Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range
                  //qDebug() << "MainWindow::callTQSL: 9"  << endl;
            msg = tr("Error #9: Some QSOs were processed, and some QSOs were ignored because they were duplicates or out of date range.");
            break;
        case 10: // command syntax error
                  //qDebug() << "MainWindow::callTQSL: 10"  << endl;
            msg = tr("Error #10: Command syntax error. KLog sent a bad syntax command.");
            break;
        case 11: // LoTW Connection error (no network or LoTW is unreachable)
                  //qDebug() << "MainWindow::callTQSL: 11"  << endl;
            msg = tr("Error #11: LoTW Connection error (no network or LoTW is unreachable).");
            break;
        default:
            msg = tr("Error #00: Unexpected error. Please contact the development team.");
            return false;

                // should never be reached
        }
    }

    msgBox.setText(msg);
    msgBox.exec();
    if (ok>0)
    {
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
                                                     tr("You have selected no callsign. KLog will mark QSOs without a station callsign defined and those with the call you are entering here.") + "\n\n" + tr("Enter the station callsign to use for this log or leave it empty for QSO without station callsign defined:"), QLineEdit::Normal,
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
           //qDebug() << "MainWindow::slotToolLoTWMarkAllYesThisLog"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());

    if(dataProxy->lotwSentYes(mainQSOEntryWidget->getDate(), currentLog, "ALL"))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All queued QSOs of this log has been marked as sent for LoTW!")  );
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all queued QSOs of this log as sent for LoTW!") );
        msgBox.exec();
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotToolLoTWMarkAllYes()
{
           //qDebug() << "MainWindow::slotToolLoTWMarkAllYes"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString stationCallToUse = selectStationCallsign();

    //QString tdate = util->getDateSQLiteStringFromDate(mainQSOEntryWidget->getDate());
    if (dataProxy->lotwSentYes(mainQSOEntryWidget->getDate(), -1, stationCallToUse))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All queued QSOs has been marked as sent to LoTW!") );
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all queued QSOs of this log as sent to LoTW!") );
        msgBox.exec();
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotReceiveQSOListToShowFromFile(QStringList _qs)
{
    //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile: " << QString::number(_qs.length())  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    // Receiving:  modifiedQSOList << _call << _date + "-" +_time << _band << _mode << _qslrdate;
    // Must send QRZ-DX, Date-Time(yyyyMMdd-hhmmss), Band, Mode
    if (_qs.length()!=4)
    {
        //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid qso list received - length: " << QString::number(_qs.length()) << endl;
        return;
    }

    if (!util->isValidCall(_qs.at(0)))
    {
        //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid QRZ received - " << _qs.at(0) << endl;
        return;
    }

    if (dataProxy->getIdFromBandName(_qs.at(2))<0)
    {
       //qDebug() << "SMainWindow::slotReceiveQSOListToShowFromFile - NO valid BAND received"<< _qs.at(2) << endl;
        return;
    }
    if (dataProxy->getIdFromModeName(_qs.at(3))<0)
    {
        //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile - NO valid Mode received" << _qs.at(3)<< endl;
        return;
    }
    if (!util->isValidDateTimeFromString(_qs.at(1)))

    {
        //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile No valid date" << endl;
            return;
    }
    //qDebug() << "MainWindow::slotReceiveQSOListToShowFromFile ALL OK" << endl;
    //QStringList qsoToSend;
    //qsoToSend.clear();
    //qsoToSend << _qs.at(0) << _qs.at(1) << _qs.at(2) << _qs.at(3);
    showAdifImportWidget->addQSOToTheList(_qs);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotAboutQt()
{
          //qDebug() << "MainWindow::slotAboutQt"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QMessageBox::aboutQt(this,tr("About..."));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotHelpAboutAction()
{
              //qDebug() << "MainWindow::slotHelpAboutAction "  << endl;
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
          //qDebug() << "MainWindow::slotTipsAction "  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    tipsDialog->exec();


    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotHelpCheckUpdatesAction()
{
          //qDebug() << "MainWindow::slotHelpCheckUpdatesAction" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    callingUpdate = true;
    softUpdate->addCall(stationQRZ);
    softUpdate->needToUpdate(true);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotShowSoftUpdateResults(const bool _b)
{

             //qDebug() << "MainWindow::slotShowSoftUpdateResults: " << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (_b == true)
    {
             //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = TRUE " << endl;
    }
    else
    {
                 //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = FALSE " << endl;

    }

    if (callingUpdate == true)
    {
        if (_b == false)
        {
                     //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NOT NEEDED" << endl;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog update checking result"));
            msgBox.setText(tr("Congratulations!") + "\n\n" + tr("You already have the latest version."));
            msgBox.exec();
        }
        else
        {
                     //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NEEDED" << endl;
        }
    }
    callingUpdate = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}



void MainWindow::slotLogWinShow()
{
              //qDebug() << "MainWindow::slotLogWinShow: "  << endl;
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

/*
void MainWindow::slotScoreWinShow()
{
          //qDebug() << "MainWindow::slotScoreWinShow: "  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (!(scoreWindow->isVisible()) )
    {
        scoreWinAct->setChecked ( true );
        scoreWindow->show();

    }else
    {
        scoreWinAct->setChecked ( false );
        scoreWindow->hide();
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
*/

void MainWindow::slotSetup(const int _page)
{
          //qDebug() << "MainWindow::slotSetup - 01"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    configured = false;
    openSetup(_page);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::openSetup(const int _page)
{
          //qDebug() << "MainWindow::slotSetup - 01"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (!needToEnd)
    {
        logEvent(Q_FUNC_INFO, "Just before setData", logSeverity);
              //qDebug() << "MainWindow::slotSetup - Just before setupDialog->exec"  << endl;
        setupDialog->setData(configFileName, softwareVersion, _page, !configured);
        if ((!configured) || (itIsANewversion))
        {
            logEvent(Q_FUNC_INFO, "Just before SetupDialog->exec", logSeverity);
            itIsANewversion = false;
            setupDialog->exec();
            //setupDialog->open();
            logEvent(Q_FUNC_INFO, "Just after setupDialog->exec", logSeverity);
                  //qDebug() << "MainWindow::slotSetup - Just after setupDialog->exec"  << endl;
        }
        else
        {
            logEvent(Q_FUNC_INFO, "No setupDialog->exec needed", logSeverity);
                  //qDebug() << "MainWindow::slotSetup - No setupDialog->exec needed"  << endl;
        }



        if (needToEnd)
        {
            logEvent(Q_FUNC_INFO, "END-1", logSeverity);
            return;
        }
        else
        {
           //qDebug() << "MainWindow::slotSetup - Just before readConfigData"  << endl;
            logEvent(Q_FUNC_INFO, "Just before readConfigData", logSeverity);
            readConfigData();
            reconfigureDXMarathonUI(manageDxMarathon);
            //if (contestMode == "DX")
            //{
            //}
            logEvent(Q_FUNC_INFO, "Just after readConfigData", logSeverity);
           //qDebug() << "MainWindow::slotSetup - Just after readConfigData"  << endl;
        }


      //qDebug() << "MainWindow::MainWindow: logmodel to be created-2" << endl;
        logEvent(Q_FUNC_INFO, "logmodel to be created-2", logSeverity);
        logWindow->createlogPanel(currentLog);
        logEvent(Q_FUNC_INFO, "logmodel has been created-2", logSeverity);
      //qDebug() << "MainWindow::MainWindow: logmodel has been created-2" << endl;

    }

    defineStationCallsign();
    logEvent(Q_FUNC_INFO, "before db->reConnect", logSeverity);
      //qDebug() << "MainWindow::MainWindow: before db->reConnect" << endl;
    dataProxy->reconnectDB();
    logEvent(Q_FUNC_INFO, "after db->reConnect", logSeverity);
      //qDebug() << "MainWindow::MainWindow: after db->reConnect" << endl;
    if (hamlibActive)
    {
        qDebug() << "MainWindow::MainWindow: Hamlib is active, let's read the VFO Freq/Mode" << endl;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
void MainWindow::openFile()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    int lastLog = currentLog;

    openSetup(6);

    if (lastLog == currentLog)
    { // It seems that the user didn't really want a new log
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }

    logWindow->refresh();
    logEvent(Q_FUNC_INFO, "END", logSeverity);

}
*/

/*
bool MainWindow::saveFile(const QString &_fileName)
{
            //qDebug() << "MainWindow::saveFile: " << _fileName  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString fileName = _fileName;

    if (fileName.endsWith(".adi", Qt::CaseInsensitive))
    {
                  //qDebug() << "MainWindow::saveFile: 1"  << endl;
        needToSave = !(filemanager->adifLogExport(fileName, currentLog));
    }
    else if (fileName.endsWith(".log", Qt::CaseInsensitive))
    {
                  //qDebug() << "MainWindow::saveFile: 2"  << endl;
        needToSave = !(filemanager->cabrilloLogExport(fileName, contestMode, currentLog));
        //contest->saveFileToSend(fileName);
    }
    else
    {
                  //qDebug() << "MainWindow::saveFile: 3"  << endl;
        //TODO: Message "You must select a proper file format
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setText(tr("Nothing has been saved. You have to select a valid file type."));
       msgBox.exec();
       logEvent(Q_FUNC_INFO, "END-1", logSeverity);
       return false;
    }
              //qDebug() << "MainWindow::saveFile: 4"  << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return needToSave;

}

bool MainWindow::saveFileAs()
{
             //qDebug() << "MainWindow::saveFileAs"  << endl;
    //QFileDialog dialog(this);
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QStringList filters;
    filters << "ADIF files (*.adi *.adif)"
            << "Cabrillo files (*.log)"
            << "Any files (*)";

   // klogDir+"/"+defaultADIFLogFile,
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                   util->getHomeDir(),
                        tr("ADIF file") + "(*.adi *.adif);;" + tr("Cabrillo files") + "(*.log);;" + tr("Any file") + "(*.*)");

    if ( (fileName.endsWith(".adi", Qt::CaseInsensitive)) || (fileName.endsWith(".log", Qt::CaseInsensitive)) )
    {
        useDefaultLogFileName = true;
        defaultADIFLogFile = fileName;
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);

        return saveFile(fileName);
    }
    else if (fileName.length()==0)
    {
        // The user clicked on cancel, no msg to be shown
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return false;
    }
    else
    {

        //TODO: Message "You must select a proper file format
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setText(tr("Nothing has been saved. You have to select a valid file type."));
       msgBox.exec();
       logEvent(Q_FUNC_INFO, "END", logSeverity);
       return false;
    }

    //return false;

}
*/
/*
void MainWindow::newFile()
{
               //qDebug() << "MainWindow::newFile"  << endl;
     //TODO: Ask for a confirmation to the user
    //TODO: Clean the DB & query.exec("VACUUM");
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    int lastLog = currentLog;

    openSetup(6);

    if (lastLog == currentLog)
    { // It seems that the user didn't really want a new log
        return;
    }

    //points = 0;
    //multipliers = 0;
    //qsoPoints = 0;
    //qsoMultiplier = 0;
    logWindow->refresh();
    slotClearButtonClicked();
    searchWidget->clear();
    //searchResultsTreeWidget->clear();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
*/

bool MainWindow::slotOpenKLogFolder()
{
           //qDebug() << "MainWindow::slotOpenKLogFolder: " << configFileName << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //configFileName = klogDir+"/klogrc.cfg";
    QString _aux = "<ul><li><a href=file://" + util->getHomeDir() + ">file://" + util->getHomeDir() + "</a></li>" +
                    "<li><a href=file://" + util->getKLogDBFile() + ">file://" + util->getKLogDBFile() + "</a></i></ul>" ;
    QString _text = tr("You can find the KLog data folder here: ") + _aux;
/*
    int ret = QMessageBox::information(this, tr("KLog"),
                                   _text,
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
*/
    QMessageBox::information(this, tr("KLog"),
                                   _text,
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
           //qDebug() << "MainWindow::slotOpenKLogFolder: END"  << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return true;

}


void MainWindow::slotUpdateStatusBar(const QString &statusm)
{
              //qDebug() << "MainWindow::slotUpdateStatusBar: " << statusm  << endl;
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
             //qDebug() << "MainWindow::slotDoubleClickLog: QSOid: " << QString::number(_qsoID) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    //int row = _qsoID.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    qsoToEdit(_qsoID);
    logEvent(Q_FUNC_INFO, "END", logSeverity);

    //TODO: To be added to the logWindow and create an action that emist the QSO id to be edited
}


void MainWindow::keyPressEvent(QKeyEvent *event){

    /*
    if (!isStarted || isPaused || curPiece.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }
    */
          //qDebug() << "MainWindow::keyPressEvent: "  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    switch (event->key()) {
    case Qt::Key_Return:
        // ENTER PRESSED
        slotQRZReturnPressed();
              //qDebug() << "MainWindow::keyPressEvent: RETURN"  << endl;
        break;
    case Qt::Key_Enter:

        // ENTER PRESSED
              //qDebug() << "MainWindow::keyPressEvent: ENTER"  << endl;
        slotQRZReturnPressed();
        break;

    default:
        //QFrame::keyPressEvent(event);
        ;

    }

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

/*
void MainWindow::checkIfWorkedB4(const QString &_qrz)
{
              //qDebug() << "MainWindow::checkIfWorkedB4: " << _qrz << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    int i = dataProxy->isWorkedB4(_qrz, currentLog); // Gets the QSO id if worked before



    if (contestMode == "DX")
    {}
    else if (contestMode == "CQ-WW-SSB")
    {
        if (i>=0)
        {
            qrzgroupBox->setTitle(tr("DUPE"));
        }
        else
        {
            qrzgroupBox->setTitle(tr("QRZ"));
        }
    }
    else
    {}

    logEvent(Q_FUNC_INFO, "END", logSeverity);

}
*/



void MainWindow::readConfigData()
{
   //qDebug() << "MainWindow::readConfigData - 01" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (needToEnd)
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
       //qDebug() << "MainWindow::readConfigData: File not found" << configFileName << endl;
        if (configured)
        {
           //qDebug() << "MainWindow::readConfigData: configured = true" << endl;
        }
        else
        {
           //qDebug() << "MainWindow::readConfigDataw: configured = false" << endl;
        }
        openSetup();
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }
    hamlibActive = false;
   //qDebug() << "MainWindow::readConfigData: Before processConfigLine "  << endl;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        processConfigLine(line);
    }
   //qDebug() << "MainWindow::readConfigData: After processConfigLine "  << endl;
    defineStationCallsign();

   //qDebug() << "MainWindow::readConfigData: " << defaultADIFLogFile << endl;

    if ((useDefaultLogFileName) && (defaultADIFLogFile.length()>0))
    {
        useDefaultLogFileName = true;
    }
    else
    {
        useDefaultLogFileName = false;
    }
          //qDebug() << "MainWindow::readConfigData-01"  << endl;
    infoWidget->setImperialSystem(imperialSystem);
    //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
    infoLabel2->setText(world->getEntityName(currentEntity));
          //qDebug() << "MainWindow::readConfigData-89"  << endl;
    infoWidget->showEntityInfo(currentEntity);
          //qDebug() << "MainWindow::readConfigData-90"  << endl;
    configured = true;
    searchWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    awards->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setDXClusterSpotConfig(dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY );
    dxClusterWidget->setMyQRZ(stationQRZ);
          //qDebug() << "MainWindow::readConfigData-97"  << endl;
    checkIfNewBandOrMode();
          //qDebug() << "MainWindow::readConfigData-98"  << endl;
    //initialContestModeConfiguration();
          //qDebug() << "MainWindow::readConfigData: 99" << endl;

    if (upAndRunning)
    { // Next actions will not be executed in the first run
       //qDebug() << "MainWindow::readConfigData: 99.1" << endl;
        slotClearButtonClicked();
       //qDebug() << "MainWindow::readConfigData: 99.2" << endl;
        //createSearchResultsPanel();
    }

    // I need to init the CLUBLOG
    if (clublogActive)
    {
        //           setCredentials(const QString &_call, const QString &_email, const QString &_pass, const bool _useQSOStationCall)
      //qDebug() << "MainWindow::readConfigData: Setting ClublogCredentials" << endl;
        //elogClublog->setCredentials(clublogUser, clublogEmail, clublogPass, clublogUseStationCallSign);
        elogClublog->setCredentials(clublogEmail, clublogPass, stationQRZ);
    }
    else
    {
      //qDebug() << "MainWindow::readConfigData: NOT Setting ClublogCredentials" << endl;
    }
   //qDebug() << "MainWindow::readConfigData: QRZcom active????" << endl;
    if (qrzcomActive)
    {
       //qDebug() << "MainWindow::readConfigData: QRZcom active" << endl;
        elogQRZcom->setCredentials(qrzcomUser, qrzcomPass);
       //qDebug() << "MainWindow::readConfigData: login" << endl;
        elogQRZcom->login();
       //qDebug() << "MainWindow::readConfigData: after login" << endl;
    }
   //qDebug() << "MainWindow::readConfigData: calling checkIfNewBandOrMode" << endl;


           //qDebug() << "MainWindow::readConfigData: 100" << endl;
    util->setVersion(softwareVersion);
           //qDebug() << "MainWindow::readConfigData: 101" << endl;
    searchWidget->setVersion(softwareVersion);
           //qDebug() << "MainWindow::readConfigData: 102" << endl;
    searchWidget->setCurrentLog(currentLog);
           //qDebug() << "MainWindow::readConfigData: 103" << endl;
    infoWidget->setCurrentLog(currentLog);
          //qDebug() << "MainWindow::readConfigData: 104" << endl;
    //searchWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    infoWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());

    satTabWidget->refreshData();

    QString aux;

    QString errorMSG;

    if (UDPServerStart)
    {
               //qDebug() << "MainWindow::readConfigData: 104: UDPServerStart TRUE" << endl;
        if (!UDPLogServer->isStarted())
        {
                   //qDebug() << "MainWindow::readConfigData: 104: Server off" << endl;
        //start(quint16 _port, QHostAddress const& _multicast_group_address)
            if (!UDPLogServer->start())
            {
                errorMSG =  tr("start");
                aux = tr("UDP Server error\nThe UDP server failed to %1.", "start or stop").arg(errorMSG);
                showErrorDialog->setText(aux);
                showErrorDialog->exec();
            }
            else
            {
                     //qDebug() << "MainWindow::readConfigData: UDP Log server started!" << endl;
            }
        }
        else
        {
                  //qDebug() << "MainWindow::readConfigData: UDP Log server already started no need to restart!" << endl;
        }
    }        
    else
    {
               //qDebug() << "MainWindow::readConfigData: 104: UDPServerStart FALSE" << endl;
        if (UDPLogServer->isStarted())
        {
            if (!UDPLogServer->stop())
            {
                errorMSG =  tr("stop");
                aux = tr("UDP Server error\nThe UDP server failed to %1.", "start or stop").arg(errorMSG);
                showErrorDialog->setText(aux);
                showErrorDialog->exec();
            }
            else
            {
                      //qDebug() << "MainWindow::readConfigData: UDP Log server stopped!" << endl;
            }
        }
        else
        {
                  //qDebug() << "MainWindow::readConfigData: UDP Log server already stopped no need to restop!" << endl;
        }
    }

          //qDebug() << "MainWindow::readConfigData: hamlib" << endl;
    if (hamlibActive)
    {
              //qDebug() << "MainWindow::readConfigData: STARTING HAMLIB" << endl;
        hamlib->init(true);
              //qDebug() << "MainWindow::readConfigData: HAMLIB STARTED";
    }
    else
    {
              //qDebug() << "MainWindow::readConfigData: STOPPING HAMLIB";
        hamlib->stop();
              //qDebug() << "MainWindow::readConfigData: NOT STARTING HAMLIB";
    }

   //qDebug() << "MainWindow::readConfigData - END" << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

bool MainWindow::processConfigLine(const QString &_line){
    //qDebug() << "MainWindow::processConfigLine: " << _line << endl;
    logEvent(Q_FUNC_INFO, "Start: ", logSeverity);
    int _logWithMoreQSOs = 0; // At the end, if the this variable is >0 the Selectedlog will have to be changed in the file.
    QString line = _line.simplified();
    //line.simplified();
    QString aux;

    QStringList values = line.split("=", QString::SkipEmptyParts);


    if (line.startsWith('#')){
                  //qDebug() << "MainWindow::processConfigLine: notes Line!" << endl;
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return true;
    }
    if (!( (line.contains('=')) && (line.contains(';')))){
                  //qDebug() << "MainWindow::processConfigLine: Wrong Line!" << endl;
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return false;
    }
    QString field = (values.at(0)).toUpper();
    QString value = values.at(1);

    int endValue = value.indexOf(';');
    if (endValue>-1){

        value = value.left(value.length() - (value.length() - endValue));
    }

    if (field == "CALLSIGN"){
        if (util->isValidCall(value))
        {
            mainQRZ = value;
        }

    }else if (field=="CQZ"){
        my_CQz = value.toInt();
    }else if (field=="ITUZ"){
        my_ITUz = value.toInt();
    }else if (field=="CONTEST"){
                  //qDebug() << "MainWindow::processConfigLine: CONTEST: " << endl;
        contestMode = value;

    }else if (field=="MODES"){
        readActiveModes(value.split(", ", QString::SkipEmptyParts));
    }else if (field=="BANDS"){
                 //qDebug() << "MainWindow::processConfigLine: BANDS: " << value << endl;
        readActiveBands(value.split(", ", QString::SkipEmptyParts));
    }else if (field=="REALTIME"){
                  //qDebug() << "MainWindow::processConfigLine: REALTIME: " << value.toUpper() << endl;
        mainQSOEntryWidget->setRealTime(util->trueOrFalse(value));
        //realTime = util->trueOrFalse(value);
    }
    else if (field =="DXCLUSTERSERVERTOUSE"){
        aux = value;  //dxfun.com:8000
        if (aux.contains(':'))
        {
            dxclusterServerToConnect = (aux.split(':', QString::SkipEmptyParts)).at(0);
            dxclusterServerPort = ((aux.split(':', QString::SkipEmptyParts)).at(1)).toInt();
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
              //qDebug() << "MainWindow::processConfigLine: Marathon-1 - Value: " << value << endl;
        manageDxMarathon = util->trueOrFalse(value);
              //qDebug() << "MainWindow::processConfigLine: Marathon-2" << endl;
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
            logSeverity = 7;
            logEvent(Q_FUNC_INFO, "Log enabled");
        }
        else
        {
            logEvent(Q_FUNC_INFO, "Log disabled");
            logSeverity = 0;
        }
    }
    else if (field=="UTCTIME")
    {
                  //qDebug() << "MainWindow::processConfigLine: UTCTIME: " << value.toUpper() <<endl;
        //UTCTime = util->trueOrFalse(value);
        mainQSOEntryWidget->setUTC(util->trueOrFalse(value));
    }
    else if (field=="KEEPMYDATA")
    {
        keepMyData  = util->trueOrFalse(value);
    }
    /*
    else if (field=="LOGSORT")
    {
        logWindow->setProxyModel(util->trueOrFalse(value));
    }
    */
    else if (field=="SENDEQSLBYDEFAULT")
    {
        eQSLTabWidget->setQueueSentByDefault(util->trueOrFalse(value));
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
                  //qDebug() << "MainWindow::processConfigLine: " << defaultADIFLogFile << endl;
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
               //qDebug() << "MainWindow::processConfigLine: UDPSERVER: " << value.toUpper()  << endl;
        UDPServerStart = util->trueOrFalse(value);
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
              //qDebug() << "MainWindow::processConfigLine: HAMLIBRIGTYPE: " << value << endl;
        hamlib->setModelId(value.toInt());
              //qDebug() << "MainWindow::processConfigLine: HAMLIBRIGTYPE: " << value << endl;
    }
    else if(field == "HAMLIBSERIALPORT")
    {
              //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALPORT: " << value << endl;
        hamlib->setPort(value);
              //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALPORT: " << value << endl;
    }
    else if (field == "HAMLIBSERIALBAUDS")
    {
              //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALBAUDS: " << value << endl;
        hamlib->setSpeed(value);
              //qDebug() << "MainWindow::processConfigLine: HAMLIBSERIALBAUDS: " << value << endl;
    }else if(field =="HAMLIBSERIALDATABITS"){
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDATABITS: " << value << endl;
        hamlib->setData(value);
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDATABITS: " << value << endl;
    }else if(field =="HAMLIBSERIALSTOPBITS"){
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALSTOPBITS: " << value << endl;
        hamlib->setStop(value);
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALSTOPBITS: " << value << endl;
    }else if(field =="HAMLIBSERIALFLOWCONTROL"){
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALFLOWCONTROL: " << value << endl;
        hamlib->setFlow(value);
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALFLOWCONTROL: " << value << endl;
    }else if(field =="HAMLIBSERIALPARITY"){
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALPARITY: " << value << endl;
        hamlib->setParity(value);
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALPARITY: " << value << endl;
    }else if(field =="HAMLIBSERIALRTS"){
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALRTS: " << value << endl;
        //hamlib->setRTS(value);
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALRTS: " << value << endl;
    }else if(field =="HAMLIBSERIALDTR"){
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDTR: " << value << endl;
        //hamlib->setDTR(value);
           //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDTR: " << value << endl;
    }else if (field == "HAMLIBRIGPOLLRATE"){
        hamlib->setPoll(value.toInt());
    }else if (field == "HAMLIB")
    {
        qDebug() << "MainWindow::processConfigLine: HAMLIB: " << value << endl;
        hamlibActive = util->trueOrFalse(value);
        qDebug() << "MainWindow::processConfigLine: HAMLIB: " << value << endl;
    }
    else if (field == "HAMLIBREADONLY")
    {
              //qDebug() << "MainWindow::processConfigLine: HAMLIBREADONLY: " << value << endl;
        hamlib->setReadOnly(util->trueOrFalse(value));
    }
    else if (field=="REALTIMEFROMWSJTX")
    {
               //qDebug() << "MainWindow::processConfigLine: REALTIMEFROMWSJTX: " << value << endl;
        UDPLogServer->setRealTimeUpdate(util->trueOrFalse(value));
    }
    else if(field=="SELECTEDLOG")
    {
        currentLog = value.toInt();
                //qDebug() << "MainWindow::processConfigLine: currentLog - SelectedLog: " << QString::number(currentLog) << endl;

        //if ( ((dataProxy->doesThisLogExist(currentLog))  && (dataProxy->getHowManyQSOInLog(currentLog) > 0)) )
        if ( ((dataProxy->doesThisLogExist(currentLog)) ) )
        {
                    //qDebug() << "MainWindow::processConfigLine: currentLog - Log with QSO - SelectedLog: " << QString::number(currentLog) << endl;
        }
        else
        {
            int _howManyQSOMax = -1;     // NUmber of QSO of the log with more QSO
            int _howManyQSOMaxT = 0;    // Number of QSO in ine specific log
            QStringList logs = QStringList();


            logs << dataProxy->getListOfManagedLogs();
                   //qDebug() << "MainWindow::processConfigLine: logs: " << QString::number(logs.size()) << endl;
            for (int i = 0;i<logs.length();i++)
            {
                _howManyQSOMaxT = dataProxy->getHowManyQSOInLog(i);
                        //qDebug() << "MainWindow::processConfigLine: SelectedLog-x: " << QString::number(i) << " - QSOs: " << QString::number(_howManyQSOMaxT) << endl;
                if (_howManyQSOMax < _howManyQSOMaxT)
                {
                            //qDebug() << "MainWindow::processConfigLine: Found log with more QSO: " << logs.at(i) << endl;
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
/*
                    //qDebug() << "MainWindow::processConfigLine: currentLog - Log without QSO - SelectedLog: " << QString::number(currentLog) << endl;
            QMessageBox msgBox;

            msgBox.setIcon(QMessageBox::Warning);
            QString aux = tr("The selected log is not existing or it is still empty.") + "\n\n" + tr("Click Yes and KLog will open an empty log.") + "\n" +
                    tr("Click No and KLog will select another log with data.") + "\n\n" +
                    tr("You can modify the config file accordingly, if needed.");
            msgBox.setText(aux);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();
            switch (ret)
            {
                case QMessageBox::Yes:
                break;

                    case QMessageBox::No:

                    logs << dataProxy->getListOfManagedLogs();
                           //qDebug() << "MainWindow::processConfigLine: logs: " << QString::number(logs.size()) << endl;
                    for (int i = 0;i<logs.length();i++)
                {
                    _howManyQSOMaxT = dataProxy->getHowManyQSOInLog(i);
                            //qDebug() << "MainWindow::processConfigLine: SelectedLog-x: " << QString::number(i) << " - QSOs: " << QString::number(_howManyQSOMaxT) << endl;
                    if (_howManyQSOMax < _howManyQSOMaxT)
                    {
                                //qDebug() << "MainWindow::processConfigLine: Found log with more QSO: " << logs.at(i) << endl;
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
                break;

            default:
                // should never be reached
            break;
            }
*/
        }
        dxClusterWidget->setCurrentLog(currentLog);
        dxccStatusWidget->setCurrentLog(currentLog);
               //qDebug() << "MainWindow::processConfigLine: currentLog: " << value << endl;
    }
    else if(field=="CLUBLOGACTIVE")
    {
                //qDebug() << "MainWindow::processConfigLine: clublogActive: " << value << endl;
        clublogActive = util->trueOrFalse(value);
    }
    else if(field=="CLUBLOGREALTIME")
    {
                 //qDebug() << "MainWindow::processConfigLine: clublogRealTime: " << value << endl;
        clublogRealTime = util->trueOrFalse(value);
    }   
    else if(field=="CLUBLOGPASS")
    {
       //qDebug() << "MainWindow::processConfigLine: clublogPass: " << value << endl;
        clublogPass = value;

    }
    else if(field=="CLUBLOGEMAIL")
    {
       //qDebug() << "MainWindow::processConfigLine: clublogEmail: " << value << endl;
        clublogEmail = value;
    }
    else if(field=="QRZCOMACTIVE")
    {
        qrzcomActive = util->trueOrFalse(value);
    }
    else if(field =="QRZCOMAUTO")
    {
       //qDebug() << "MainWindow::processConfigLine: QRZCOMAuto: " << value << endl;
       //qDebug() << "MainWindow::processConfigLine: QRZCOMAuto was: " << util->boolToQString(QRZCOMAutoCheckAct->isChecked()) << endl;

        QRZCOMAutoCheckAct->setChecked(util->trueOrFalse(value));        
       //qDebug() << "MainWindow::processConfigLine: QRZCOMAuto is: " << util->boolToQString(QRZCOMAutoCheckAct->isChecked()) << endl;
    }
    else if(field=="QRZCOMPASS")
    {
        qrzcomPass = value;
    }
    else if(field=="QRZCOMUSER")
    {
        qrzcomUser = value;
    }
    else if(field =="EQSLACTIVE"){
        eQSLActive = util->trueOrFalse(value);
       //qDebug() << "MainWindow::processConfigLine - EQSLACTIVE" << endl;
    }
    else if(field =="EQSLREALTIME"){
        eQSLRealTime = util->trueOrFalse(value);
       //qDebug() << "MainWindow::processConfigLine - EQSLREALTIME" << endl;
    }
    else if(field =="EQSLCALL"){
       //qDebug() << "MainWindow::processConfigLine - EQSLPASS" << endl;
        eqslUtilities->setUser(value);
    }
    else if(field =="EQSLPASS"){
       //qDebug() << "MainWindow::processConfigLine - EQSLPASS" << endl;
        eqslUtilities->setPass(value);
    }
    else if(field =="EQSLUSESTATIONCALLSIGN"){
       //qDebug() << "MainWindow::processConfigLine - EQSLUSESTATIONCALLSIGN" << endl;
        eQSLUseQSOStationCallSign = util->trueOrFalse(value);
    }
    else if(field =="LOTWACTIVE"){
              //qDebug() << "MainWindow::processConfigLine - LOTWACTIVE" << endl;
        if (util->trueOrFalse(value))
        {
            lotwCallTQSL->setEnabled(true);
            lotwCallTQSL->setWhatsThis(tr("Sends the log to LoTW calling TQSL."));
        }
        else
        {
            lotwCallTQSL->setEnabled(false);
            lotwCallTQSL->setWhatsThis(tr("This function is disabled. Go to the Setup->LoTW tab to enable it."));
        }
              //qDebug() << "MainWindow::processConfigLine - LOTWACTIVE-END" << endl;
    }
    else if(field =="LOTWPATH"){
        //qDebug() << "MainWindow::processConfigLine - LOTWPATH" << endl;
        lotwTQSLpath = value;
    }
    else if(field =="LOTWUSER"){
        //qDebug() << "MainWindow::processConfigLine - LOTWUSER: -" << value <<"-" << endl;
        lotwUtilities->setUser(value);
        //qDebug() << "MainWindow::processConfigLine - AFTER LOTWUSER" << endl;
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
            setWindowsSize(values.at(0).toInt(), values.at(1).toInt());
        }
    }
    else
    {
    //qDebug() << "MainWindow::processConfigLine: NONE: " << endl;
    }

    // Lines are: Option = value;
    //qDebug() << "MainWindow::processConfigLine: END" << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    return true;
}


void MainWindow::checkIfNewBandOrMode()
{//Checks the log to see if there is a QSO with a band/mode
//that is not currently selected as active
          //qDebug() << "MainWindow::checkIfNewBandOrMode - START" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    setupDialog->checkIfNewBandOrMode(); // Update the Setup dialog with new bands or modes
          //qDebug() << "MainWindow::checkIfNewBandOrMode - 1" << endl;
    QStringList bandsInLog = dataProxy->getBandsInLog(currentLog);
          //qDebug() << "MainWindow::checkIfNewBandOrMode - 2" << endl;
    QStringList modesInLog = dataProxy->getModesInLog(currentLog);
          //qDebug() << "MainWindow::checkIfNewBandOrMode - 3" << endl;
    QStringList qsTemp;
    qsTemp.clear();
          //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.1" << endl;
    bands << bandsInLog;
    bands.removeDuplicates();
          //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.2" << endl;
    qsTemp << dataProxy->sortBandNamesBottonUp(bands);
          //qDebug() << "MainWindow::checkIfNewBandOrMode - 3.3" << endl;
    bands.clear();
    bands = qsTemp;
    
    modes << modesInLog;
    modes.removeDuplicates();
    modes.sort();

          //qDebug() << "MainWindow::checkIfNewBandOrMode - bands -" << QString::number(bands.length()) << endl;
    //bandComboBox->clear();
    mainQSOEntryWidget->setBands(bands);
    //bandComboBox->addItems(bands);
    satTabWidget->addBands(bands);

          //qDebug() << "MainWindow::checkIfNewBandOrMode - modes -" << QString::number(modes.length()) << endl;
    mainQSOEntryWidget->setModes(modes);
    //modeComboBox->clear();
    //modeComboBox->addItems(modes);

          //qDebug() << "MainWindow::checkIfNewBandOrMode - CurrentBand/CurrentBandShown: " << QString::number(currentBand) << "/" << QString::number(currentBandShown) << endl;
    dxccStatusWidget->setBands(bands);
          //qDebug() << "MainWindow::checkIfNewBandOrMode-98" << endl;
    selectDefaultBand();
          //qDebug() << "MainWindow::checkIfNewBandOrMode-99" << endl;
    selectDefaultMode();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::checkIfNewBandOrMode END" << endl;
}

void MainWindow::selectDefaultBand()
{
          //qDebug() << "MainWindow::selectDefaultBand" << endl;
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

           //qDebug() << "MainWindow::selectDefaultBand_END" << endl;
}

void MainWindow::selectDefaultMode()
{
          //qDebug() << "MainWindow::selectDefaultMode" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (!upAndRunning)
    {
        return;
    }
    //int aux = 1;
    defaultMode = dataProxy->getMostUsedMode(currentLog);
          //qDebug() << "MainWindow::selectDefaultMode: " << QString::number(defaultMode) << endl;

    if (defaultMode < 1)
    {
        defaultMode = dataProxy->getSubModeIdFromSubMode(mainQSOEntryWidget->getBand(0));
        //defaultMode = dataProxy->getSubModeIdFromSubMode((modeComboBox->itemText(0)));
              //qDebug() << "MainWindow::selectDefaultMode2: " << QString::number(defaultMode) << endl;
              //qDebug() << "MainWindow::selectDefaultMode2S: "  << endl;

    }

    //aux = dataProxy->getNameFromSubModeId(defaultMode);

    mainQSOEntryWidget->setMode(dataProxy->getNameFromSubModeId(defaultMode));
    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode)));

         //qDebug() << "MainWindow::selectDefaultMode3: " << QString::number(defaultMode) << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
         //qDebug() << "MainWindow::selectDefaultMode-END" << endl;
}


void MainWindow::readActiveBands (const QStringList actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
           //qDebug() << "MainWindow::readActiveBands: " << actives << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    for (int i=0;i<actives.length();i++)
    {
               //qDebug() << "MainWindow::readActiveBands: actives: " << actives.at(i) << endl;
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


    //QStringList values = actives.split(", ", QString::SkipEmptyParts);

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
               //qDebug() << "MainWindow::readActiveBands: bands: " << bands.at(i) << endl;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
           //qDebug() << "MainWindow::readActiveBands - END" << endl;
}

void MainWindow::readActiveModes (const QStringList actives)
{
           //qDebug() << "MainWindow::readActiveModes: " << actives << endl;
          //qDebug() << "MainWindow::readActiveModes: " << endl;
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
    //QStringList values = actives.split(", ", QString::SkipEmptyParts);

    for (int i = 0; i < __modes.size() ; i++)
    {
               //qDebug() << "MainWindow::readActiveModes: checking: " << __modes.at(i) << endl;
        if (dataProxy->getIdFromModeName(__modes.at(i)) > 0)        
        {
                   //qDebug() << "MainWindow::readActiveModes: checking-exist: " << __modes.at(i) << endl;
            //if (!atLeastOne)
            //{
            //    atLeastOne = true;

            //}
            aux = __modes.at(i);

            if (aux.length()>0)
            {
                      //qDebug() << "MainWindow::readActiveModes: adding: " << aux << endl;
               modes << aux;
            }

           // modes << actives.at(i);
        }
    }
    modes.removeDuplicates();
    modes.sort();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::readActiveModes - END" << endl;
}

void MainWindow::createData()
{
          //qDebug() << "MainWindow::createData " << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    logEvent(Q_FUNC_INFO, "END", logSeverity);

}


void MainWindow::createUIDX()
{
    //      //qDebug() << "MainWindow::createUIDX << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    //bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    //modes << "SSB" << "CW" << "RTTY";
    //
    //bandComboBox->clear();
    //bandComboBox->addItems(dataProxy->sortBandNamesBottonUp(bands));
             //qDebug() << "MainWindow::createUIDX - 1-" << QString::number(modes.count()) << endl;
    //modeComboBox->clear();
    //modeComboBox->addItems(modes);
    /*
    int mi = 0;
    for (int i = 0; i < modes.size(); ++i)
    {
        if (mi > (modes.at(i)).size())
        {
            mi = (modes.at(i)).size();
        }
    }
    //modeComboBox->setMinimumContentsLength(mi);
    modeComboBox->setMinimumWidth(mi);
    */
    //modeComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);


    txFreqSpinBox->setToolTip(tr("TX Frequency in MHz."));
    rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));
    //myPowerSpinBox->setToolTip(tr("Power used for the QSO in watts"));
    rxPowerSpinBox->setToolTip(tr("Power used by the contacted station."));
    operatorLineEdit->setToolTip(tr("Logging operator's callsign."));
    stationCallSignLineEdit->setToolTip(tr("Callsign used over the air."));


    nameLineEdit->setToolTip(tr("Name of the contacted operator."));
    qthLineEdit->setToolTip(tr("QTH of the contacted station."));
    locatorLineEdit->setToolTip(tr("Locator of the contacted station."));
          //qDebug() << "MainWindow::createUIDX-10" << endl;
    //freqQLCDNumber->setToolTip(tr("Frequency of the QSO"));
    //qrzLineEdit->setToolTip(tr("QRZ of the QSO."));
    rstTXLineEdit->setToolTip(tr("TX RST."));
          //qDebug() << "MainWindow::createUIDX-11" << endl;
    rstRXLineEdit->setToolTip(tr("RX RST."));
          //qDebug() << "MainWindow::createUIDX-12" << endl;

    infoLabel1->setToolTip(tr("Status of the DX entity."));
          //qDebug() << "MainWindow::createUIDX-13" << endl;
    infoLabel2->setToolTip(tr("Name of the DX entity."));
          //qDebug() << "MainWindow::createUIDX-14" << endl;

    //STXLineEdit->setToolTip(tr("TX Exchange."));
    //SRXLineEdit->setToolTip(tr("RX Exchange."));
    //bandComboBox->setToolTip(tr("Band of the QSO."));
    //modeComboBox->setToolTip(tr("Mode of the QSO."));
    //dateEdit->setToolTip(tr("Date of the QSO."));
    //timeEdit->setToolTip(tr("Time of the QSO."));
    //statusBar->setToolTip(tr("Misc information"));
    //qsoStatusBar->setToolTip(tr("QSO information"));
    //OKButton->setToolTip(tr("Add the QSO to the log."));
    //spotItButton->setToolTip(tr("Spots this QSO to the DX Cluster - This function is still not implemented"));
    //clearButton->setToolTip(tr("Clears the QSO entry."));
    //      //qDebug() << "MainWindow::createUIDX-25" << endl;
    /*
    dxccConfirmedQLCDNumber->setToolTip(tr("Number of confirmed DXCC entities."));
    dxccWorkedQLCDNumber->setToolTip(tr("Number of worked DXCC entities."));
    wazConfirmedQLCDNumber->setToolTip(tr("Number of confirmed WAZ zones."));
    wazWorkedQLCDNumber->setToolTip(tr("Number of worked WAZ zones."));
    localConfirmedQLCDNumber->setToolTip(tr("Number of confirmed local references."));
    localWorkedQLCDNumber->setToolTip(tr("Number of worked local references."));
    qsoConfirmedQLCDNumber->setToolTip(tr("Number of confirmed QSOs."));
    qsoWorkedQLCDNumber->setToolTip(tr("Number of worked QSOs."));
    dxMarathonQSOLCDNumber->setToolTip(tr("Number of QSOs worked on the selected year."));
    dxMarathonDXCCQLCDNumber->setToolTip(tr("Number of DXCC worked on the selected year."));
    dxMarathonCQQLCDNumber->setToolTip(tr("Number of CQ Zones worked on the selected year."));
    dxMarathonPointsQLCDNumber->setToolTip(tr("Score for the DXMarathon on the selected year."));
    operatingYearsComboBox->setToolTip(tr("Select the year you want to check."));
    */
          //qDebug() << "MainWindow::createUIDX-20" << endl;



    dxUpLeftInputFrame = new QFrame;
    //dxUpRightOutputFrame = new QFrame;
    //dxUpRightOutputFrame->setFrameShadow(QFrame::Raised);
    //dxUpRightOutputFrame->setFrameStyle(QFrame::StyledPanel);

    dxUpLeftTab = new QTabWidget;
    dxUpRightTab = new QTabWidget;
    dxBottonTab = new QTabWidget;

    dxUpLeftTab->setTabPosition(QTabWidget::North);
    dxUpRightTab->setTabPosition(QTabWidget::South);
          //qDebug() << "MainWindow::createUIDX-30" << endl;
    QWidget *qsoInputTabWidget = new QWidget;
    //QFormLayout *qsoInputTabWidgetLayout = new QFormLayout;
    QLabel *nameLabel = new QLabel(qsoInputTabWidget);
    nameLabel->setText(tr("Name"));
    nameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *qthLabel = new QLabel(qsoInputTabWidget);
    qthLabel->setText(tr("QTH"));
    qthLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *locLabel = new QLabel(qsoInputTabWidget);
    locLabel->setText(tr("DX Locator"));
    locLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
      //qDebug() << "MainWindow::createUIDX-40" << endl;
    QLabel *rxPowerSpinBoxLabelN = new QLabel(tr("Power(rx)"));
    rxPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstTxLabelN = new QLabel(tr("RST(tx)"));
    rstTxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstRxLabelN = new QLabel(tr("RST(rx)"));
    rstRxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //(mainQSOEntryWidget->getMode());

    QGridLayout *RSTLayout = new QGridLayout;
    RSTLayout->addWidget(rstTxLabelN, 0, 0);
    RSTLayout->addWidget(rstTXLineEdit, 1, 0);
    RSTLayout->addWidget(rstRxLabelN, 0, 1);
    RSTLayout->addWidget(rstRXLineEdit, 1, 1);

    //qDebug() << "MainWindow::createUIDX-50" << endl;
    //QHBoxLayout *qthHLayout = new QHBoxLayout;
    //qthHLayout->addWidget(qthLabel);

    QVBoxLayout *qthLayout = new QVBoxLayout;
    qthLayout->addWidget(qthLabel);
    qthLayout->addWidget(qthLineEdit);

    QVBoxLayout *rstQTHLayout = new QVBoxLayout;
    rstQTHLayout->addLayout(RSTLayout);
    rstQTHLayout->addLayout(qthLayout);

    QLabel *txfreqLabelN = new QLabel(tr("Freq TX"));
    txfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *rxfreqLabelN = new QLabel(tr("Freq RX"));
    rxfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QGridLayout *freqLayout = new QGridLayout;
    freqLayout->addWidget(txfreqLabelN, 0, 0);
    freqLayout->addWidget(rxfreqLabelN, 0, 1);
    freqLayout->addWidget(txFreqSpinBox, 1, 0);
    freqLayout->addWidget(rxFreqSpinBox, 1, 1);

    QVBoxLayout *locVLayout = new QVBoxLayout;
    locVLayout->addWidget(locLabel);
    locVLayout->addWidget(locatorLineEdit);
          //qDebug() << "MainWindow::createUIDX-60" << endl;
    //QLabel *spacerLocLabel = new QLabel(tr("SPACERLoc"));
    //QHBoxLayout *locLayout = new QHBoxLayout;
    //locLayout->addSpacerItem(new QSpacerItem(50,1));
    //locLayout->addWidget(spacerLocLabel);
    //locLayout->addLayout(locVLayout);

    QVBoxLayout *freqLocLayout = new QVBoxLayout;
    freqLocLayout->addLayout(freqLayout);
    freqLocLayout->addLayout(locVLayout);

    QHBoxLayout *rstfreqLayout = new QHBoxLayout;
    rstfreqLayout->addLayout(rstQTHLayout);
    rstfreqLayout->addLayout(freqLocLayout);

    QVBoxLayout *rxPowerLayout = new QVBoxLayout;
    rxPowerLayout->addWidget(rxPowerSpinBoxLabelN);
    rxPowerLayout->addWidget(rxPowerSpinBox);

    //QLabel *spacerNameLabel = new QLabel(tr("SPACERName"));
    //QHBoxLayout *nameHLayout = new QHBoxLayout;
    //nameHLayout->addWidget(nameLabel);
    //nameHLayout->addSpacerItem(new QSpacerItem(50,1));
    //nameHLayout->addWidget(spacerNameLabel );
          //qDebug() << "MainWindow::createUIDX-70" << endl;
    QVBoxLayout *nameLayout = new QVBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameLineEdit);

    QVBoxLayout *rxPwrLayout = new QVBoxLayout;
    rxPwrLayout->addWidget(rxPowerSpinBoxLabelN);
    rxPwrLayout->addWidget(rxPowerSpinBox);

    //QLabel *spacerHBottomLabel = new QLabel(tr("SPACERHBottom"));
    QHBoxLayout *namePwrHLayout = new QHBoxLayout;
    namePwrHLayout->addLayout(nameLayout);
    //namePwrHLayout->addSpacerItem(new QSpacerItem(50,1));
    //namePwrHLayout->addWidget(spacerHBottomLabel);
    namePwrHLayout->addLayout(rxPwrLayout);


    //QLabel *spacerBottomLabel = new QLabel(tr("SPACERBottom"));
    QVBoxLayout *namePwrLayout = new QVBoxLayout;
    //namePwrLayout->addSpacerItem(new QSpacerItem(1,50));
    //namePwrLayout->addWidget(spacerBottomLabel);
    namePwrLayout->addLayout(namePwrHLayout);

    QVBoxLayout *qsoInputTabWidgetMainLayout = new QVBoxLayout;
    qsoInputTabWidgetMainLayout->addLayout(rstfreqLayout);
    qsoInputTabWidgetMainLayout->addLayout(namePwrLayout);
    qsoInputTabWidget->setLayout(qsoInputTabWidgetMainLayout);

    dxUpLeftTab->addTab(qsoInputTabWidget, tr("QSO"));
          //qDebug() << "MainWindow::createUIDX-80" << endl;

/*
    // NOTES tab starts here
    QGridLayout *notesInputTabWidgetLayout = new QGridLayout;
    notesInputTabWidgetLayout->addWidget(notesTextEdit, 0, 0);
    notesInputTabWidget->setLayout(notesInputTabWidgetLayout);
    i = dxUpLeftTab->addTab(notesInputTabWidget, tr("Notes"));
*/
    //dxUpLeftTab->addTab(qslInputTabWidget, tr("QSL"));
    dxUpLeftTab->addTab(QSLTabWidget, tr("QSL"));
    dxUpLeftTab->addTab(eQSLTabWidget, tr("eQSL"));
    dxUpLeftTab->addTab(commentTabWidget, tr("Comment"));

    othersTabWidget->setEntitiesList(world->getEntitiesNames());
    dxUpLeftTab->addTab(othersTabWidget, tr("Others"));

    dxUpLeftTab->addTab(myDataTabWidget, tr("My Data"));
    dxUpLeftTab->addTab(satTabWidget, tr("Satellite"));


          //qDebug() << "MainWindow::createUIDX-90" << endl;
    QSplitter *upLeftSplitter = new QSplitter (this);
    //upLeftSplitter->addWidget(dxUpLeftInputFrame);
    upLeftSplitter->addWidget(mainQSOEntryWidget);
    upLeftSplitter->addWidget(dxUpLeftTab);
    upLeftSplitter->setOrientation(Qt::Vertical);


    QVBoxLayout *dxUpRightFixLayout = new QVBoxLayout;
    dxUpRightFixLayout->addWidget(infoLabel1);
    dxUpRightFixLayout->addWidget(infoLabel2);

    infoLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    infoLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    dxUpRightTab->addTab(infoWidget, tr("Info"));
          //qDebug() << "MainWindow::createUIDX-100" << endl;

    reconfigureDXMarathonUI(manageDxMarathon);

    //qDebug() << "MainWindow::createUIDX-122" << endl;
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

    mainWidget->setLayout(mLayout);


          //qDebug() << "MainWindow::createUIDX - OS DETECTION"  << endl;

#ifdef Q_OS_WIN
              //qDebug() << "MainWindow::createUIDX - WINDOWS DETECTED!"  << endl;

    dxUpLeftInputFrame->setFrameShadow(QFrame::Raised);
    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel);

#else
          //qDebug() << "MainWindow::createUIDX - NO WINDOWS DETECTED!"  << endl;

    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


    infoLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


#endif
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::createUIDX-END" << endl;

 }


void MainWindow::slotADIFExport()
{
    //qDebug() << "MainWindow::slotADIFExport - Start" << endl;

   // QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");

    adifLoTWExportWidget->setExportMode(ModeADIF);
    adifLoTWExportWidget->show();

    //filemanager->adifLogExport(fileName, currentLog);
    //qDebug() << "MainWindow::slotADIFExport -END " << endl;
}

void MainWindow::showNumberOfSavedQSO(const QString &_fn, const int _n)
{
    //qDebug() << "MainWindow::showNumberOfSavedQSO: " << _fn << "/" << QString::number(_n) << endl;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(tr("KLog ADIF export"));
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
    //qDebug() << "MainWindow::showNumberOfSavedQSO - END" << endl;
}

void MainWindow::fileExportADIF(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
    //qDebug() << "MainWindow::fileExportADIF " << _st << endl;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeADIF);

    showNumberOfSavedQSO(fileName, qsos.count());

    //qDebug() << "MainWindow::fileExportADIF - END" << endl;
}

void MainWindow::slotADIFExportAll()
{
    //qDebug() << "MainWindow::slotADIFExportAll " << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString _callToUse = "ALL";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"), util->getHomeDir(), "ADIF (*.adi *.adif)");
    //qDebug() << "MainWindow::slotADIFExportAll: " << fileName << endl;
    if ((!fileName.endsWith(".adi")) && ( !fileName.endsWith(".adif") ))
    {
       //qDebug() << "MainWindow::slotADIFExportAll: Adding the .adi to the file" << fileName << endl;
        fileName = fileName +  ".adi";
    }
        //qDebug() << "MainWindow::slotADIFExportAll-1: " << fileName << endl;
    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _callToUse, dataProxy->getFirstQSODateFromCall(_callToUse), dataProxy->getLastQSODateFromCall(_callToUse), -1, ModeADIF);
    //qDebug() << "MainWindow::slotADIFExportAll-3" << endl;
    showNumberOfSavedQSO(fileName, qsos.count());

    //filemanager->adifLogExport(fileName, 0);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::fileExportLoTW(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
    //qDebug() << "MainWindow::fileExportLoTW  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << endl;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
        //qDebug() << "MainWindow::fileExportLoTW - no valid call" << endl;
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
        //qDebug() << "MainWindow::fileExportLoTW - no valid date" << endl;
        return;
    }

    //QString fileName = "klog-lotw-upload.adi";
    QString fileName = util->getLoTWAdifFile();


    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeLotW);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
        //qDebug() << "MainWindow::fileExportLoTW NO QSOs" << endl;
        return;
    }
    //qDebug() << "MainWindow::fileExportLoTW - 50" << endl;
    bool uploadedToLoTW = callTQSL(fileName, _st);
    //bool uploadedToLoTW = true;
    //qDebug() << "MainWindow::fileExportLoTW - 51" << endl;

    int i ;
    if (uploadedToLoTW)
    {
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("If you uploaded any QSO to LoTW while you were using TQSL you can now mark them as sent in KLog.\n\nDo you want to mark as Sent all the QSOs uploaded to LoTW?") );

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

    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("KLog LoTW"));
    msgBox.setText(tr("The LoTW upload process has finished and KLog created a file (%1) in your KLog folder.\n\nDo you want KLog to remove that file?").arg(fileName));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::Yes);
    i = msgBox.exec();
    if (i == QMessageBox::Yes)
    {
        if (QFile::remove(fileName))
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog LoTW"));
            msgBox.setText(tr("The file has been removed."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
        }
    }
    //qDebug() << "MainWindow::fileExportLoTW -END " << endl;
}

void MainWindow::fileExportClubLog(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
    //qDebug() << "MainWindow::fileExportClubLog  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << endl;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
        //qDebug() << "MainWindow::fileExportClubLog - no valid call" << endl;
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
        //qDebug() << "MainWindow::fileExportClubLog - no valid date" << endl;
        return;
    }

    //QString fileName = "klog-clublog-upload.adi";
    QString fileName = util->getClubLogFile();

    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeClubLog);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
        //qDebug() << "MainWindow::fileExportClubLog NO QSOs" << endl;
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
    //qDebug() << "MainWindow::fileExportClubLog - 50" << endl;
    elogClublog->sendLogFile(fileName, qsos, overwrite);



    //qDebug() << "MainWindow::fileExportClubLog -END " << endl;
}

void MainWindow::fileExportEQSL(const QString &_st, const QDate &_startDate, const QDate &_endDate)
{
    //qDebug() << "MainWindow::fileExportEQSL  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << endl;

    QMessageBox msgBox;

    if (!util->isValidCall(_st))
    {
        //qDebug() << "MainWindow::fileExportEQSL - no valid call" << endl;
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
        //qDebug() << "MainWindow::fileExportEQSL - no valid date" << endl;
        return;
    }

    //QString fileName = "klog-eqsl-upload.adi";
    QString fileName = util->getEQSLFile();
    //QString eQSLUser = ;
    //QString eQSLPass = ;

    QList<int> qsos = filemanager->adifLogExportReturnList(fileName, _st, _startDate, _endDate, currentLog, ModeEQSL);

    if (qsos.count() <= 0)
    { // TODO: Check if errors should be managed.
        //qDebug() << "MainWindow::fileExportEQSL NO QSOs" << endl;
        return;
    }
    /*
    msgBox.setWindowTitle(tr("KLog - eQSL"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("Do you want to add this QSOs to your eQSL existing log?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
    msgBox.setDetailedText(tr("If you don't agree, this upload will overwrite your current ClubLog existing log."));
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    bool overwrite = false;
    switch (ret)
    {
    case QMessageBox::Ok:
            overwrite = false;
        break;
    case QMessageBox::No:
            overwrite = true;
        break;
    }
    //qDebug() << "MainWindow::fileExportEQSL - 50" << endl;
    */     
    eqslUtilities->sendLogFile(fileName, qsos);

    //qDebug() << "MainWindow::fileExportEQSL -END " << endl;
}


void MainWindow::slotADIFExportSelection(const QString &_st, const QDate &_startDate, const QDate &_endDate, const ExportMode _eM)
{
    //qDebug() << "MainWindow::slotADIFExportSelection  - Start: " << _st << "/" <<_startDate.toString("yyyyMMdd") <<"/" << _endDate.toString("yyyyMMdd") << endl;

    switch (_eM)
    {
    case ModeADIF:         // General ADIF
         //qDebug() << "MainWindow::slotADIFExportSelection  - ADIF" << endl;
        fileExportADIF(_st, _startDate, _endDate);
        break;
    case ModeLotW:         // LoTW
        //qDebug() << "MainWindow::slotADIFExportSelection  - LoTW" << endl;
        fileExportLoTW(_st, _startDate, _endDate);
        break;
    case ModeClubLog:         // General ADIF
        //qDebug() << "MainWindow::slotADIFExportSelection  - ClubLog" << endl;
        fileExportClubLog(_st, _startDate, _endDate);
        break;
    case ModeEQSL:         // General eQSL
        //qDebug() << "MainWindow::slotADIFExportSelection  - ClubLog" << endl;
        fileExportEQSL(_st, _startDate, _endDate);
        break;
    }

    //qDebug() << "MainWindow::slotADIFExportSelection -END " << endl;
}
void MainWindow::slotLoTWExport()
{
    // 1.- Selec call
    // 2.- Select file and export (fixed filename?)
    // 3.- Call tqsl with the filename
    // 4.- Ask for the user to remove or not the file
    //qDebug() << "MainWindow::slotLoTWExport - Start" << endl;
   // bool emptyCall = false;
    adifLoTWExportWidget->setExportMode(ModeLotW);
    adifLoTWExportWidget->show();
    //qDebug() << "MainWindow::slotLoTWExport- END" << endl;
}

void MainWindow::slotLoTWDownload()
{
    //qDebug() << "MainWindow::slotDownUpload - Start" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QStringList calls;
    calls << dataProxy->getStationCallSignsFromLog(-1);

    bool ok;

    QString callToUse = QInputDialog::getItem(this, tr("KLog - Select the Station Callsign."),
                                         tr("Select the Station Callsign to use when quering LoTW:"), calls, 0, false, &ok);

      //qDebug() << "MainWindow::slotDownUpload: " << callToUse << endl;
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
    //qDebug() << "MainWindow::slotDownUpload - END" << endl;
}

void MainWindow::slotElogClubLogModifyCurrentLog()
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


void MainWindow::slotElogEQSLModifyCurrentLog()
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


void MainWindow::slotClubLogLogUpload()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    adifLoTWExportWidget->setExportMode(ModeClubLog);
    adifLoTWExportWidget->show();

    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::sloteQSLLogUpload()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    adifLoTWExportWidget->setExportMode(ModeEQSL);
    adifLoTWExportWidget->show();

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


/*
 * void MainWindow::slotCabrilloExport()
{
              //qDebug() << "MainWindow::slotCabrilloExport " << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Cabrillo File"),
                               util->getHomeDir(),
                               "Cabrillo (*.log)");


    //contest->saveFileToSend(fileName);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}
*/

void MainWindow::slotADIFImport(){
             //qDebug() << "MainWindow::slotADIFImport " << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     util->getHomeDir(),
                                                     "ADIF (*.adi *.adif)");
    if (fileName.isNull())
    {
    }
    else
    {
               //qDebug() << "MainWindow::slotADIFImport -1" << endl;
        filemanager->adifReadLog(fileName, currentLog);
        updateQSLRecAndSent();


               //qDebug() << "MainWindow::slotADIFImport -2" << endl;

        logWindow->refresh();
               //qDebug() << "MainWindow::slotADIFImport -3" << endl;
        checkIfNewBandOrMode();
               //qDebug() << "MainWindow::slotADIFImport -4" << endl;

        if (contestMode == "DX")
        {
                   //qDebug() << "MainWindow::slotADIFImport-DX" << endl;
            //operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            awardsWidget->fillOperatingYears();
                   //qDebug() << "MainWindow::slotADIFImport-DX-1" << endl;
            slotShowAwards();
            awardsWidget->showAwards();
                   //qDebug() << "MainWindow::slotADIFImport-DX-1-end" << endl;
        }
        else if (contestMode == "CQ-WW-SSB")
        {}
        else
        {
            awardsWidget->fillOperatingYears();
            //operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            slotShowAwards();
            awardsWidget->showAwards();
        }

               //qDebug() << "MainWindow::slotADIFImport-7" << endl;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
           //qDebug() << "MainWindow::slotADIFImport-END" << endl;
}

void MainWindow::qsoToEdit (const int _qso)
{
            //qDebug() << "MainWindow::qsoToEdit: " << QString::number(_qso) << endl;

    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    readingTheUI = true;
    int nameCol;
    QString aux1;
    double testValueDouble; // Variable just to test if the value is in the appropriate range

    setModifying(true);

    modifyingQSO = _qso;
    //OKButton->setText(tr("&Modify"));

    //TODO: Optimize the following query. Maybe the * is not needed.
    QString stringQuery = QString("SELECT * FROM log WHERE id ='%1' AND lognumber='%2'").arg(_qso).arg(currentLog);
              //qDebug() << "MainWindow::qsoToEdit: " << stringQuery << endl;



    QSqlQuery query(stringQuery);
    bool sqlOK = query.exec();
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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

             //qDebug() << "MainWindow::qsoToEdit - currentEntity " << QString::number(currentEntity) << endl;

    nameCol = rec.indexOf("qso_date");
    aux1 = (query.value(nameCol)).toString();
    //qDebug() << "MainWindow::qsoToEdit - date: " << aux1 << endl;
    mainQSOEntryWidget->setDate(util->getDateTimeFromSQLiteString(aux1));
    //mainQSOEntryWidget->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
    dateTimeTemp->setDate(util->getDateFromSQliteString(aux1));

    nameCol = rec.indexOf("bandid");
    aux1 = (query.value(nameCol)).toString();
    stringQuery = QString("SELECT name FROM band WHERE id ='%1'").arg(aux1);
    QSqlQuery queryAux(stringQuery);

    sqlOK = queryAux.exec();
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, queryAux.lastError().databaseText(), queryAux.lastError().number(), queryAux.lastQuery());
    }

    queryAux.next();
    if (queryAux.isValid())
    {
        aux1 = (queryAux.value(0)).toString();
               //qDebug() << "MainWindow::qsoToEdit - bandid-1 " << aux1 << endl;
               //qDebug() << "MainWindow::qsoToEdit: - Changing from: " << mainQSOEntryWidget->getBand() << endl;
        mainQSOEntryWidget->setBand(aux1);
        //bandComboBox->setCurrentIndex(bandComboBox->findText(aux1, Qt::MatchCaseSensitive));
               //qDebug() << "MainWindow::qsoToEdit: - Changing to: " << mainQSOEntryWidget->getBand() << endl;
    }
    else
    {
               //qDebug() << "MainWindow::qsoToEdit - bandid-NO "  << endl;
        mainQSOEntryWidget->setBand(dataProxy->getNameFromBandId(defaultBand));
        //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
        //bandComboBox->setCurrentIndex(defaultBand);
    }

           //qDebug() << "MainWindow::qsoToEdit: Check mode " <<  endl;
    nameCol = rec.indexOf("modeid");
    aux1 = (query.value(nameCol)).toString();
              //qDebug() << "MainWindow::qsoToEdit: (aux1)-1: " << aux1 << endl;

    stringQuery = QString("SELECT submode FROM mode WHERE id ='%1'").arg(aux1);
    sqlOK = queryAux.exec(stringQuery);

    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, queryAux.lastError().databaseText(), queryAux.lastError().number(), queryAux.lastQuery());
    }

    queryAux.next();
    if (queryAux.isValid())
    {
        aux1 = (queryAux.value(0)).toString();
                  //qDebug() << "MainWindow::qsoToEdit: Mode query valid: -" << aux1 << "-Length: " << QString::number(aux1.length()) << endl;
        if (mainQSOEntryWidget->isModeExisting(aux1))
        //if (modeComboBox->findText(aux1, Qt::MatchCaseSensitive)>=0)
        {
                      //qDebug() << "MainWindow::qsoToEdit: Mode in the Combobox: "<< endl;
            mainQSOEntryWidget->setMode(aux1);
            //modeComboBox->setCurrentIndex(modeComboBox->findText(aux1, Qt::MatchCaseSensitive));
        }
        else
        {
            //TODO: Add this mode to the list modes in use
                      //qDebug() << "MainWindow::qsoToEdit: Mode query valid but not in comboBox: " << aux1 << endl;
            /*
            for (int i = 0; i < (modeComboBox->count()); i++)
            {
                          //qDebug() << "MainWindow::qsoToEdit: Mode: " << modeComboBox->itemText(i) << endl;

            }
            */
        }
                  //qDebug() << "MainWindow::qsoToEdit: After Mode IF" << endl;

    }
    else
    {
                  //qDebug() << "MainWindow::qsoToEdit: Mode query not valid" << endl;

        mainQSOEntryWidget->setMode(dataProxy->getNameFromSubModeId(defaultMode));
        //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode), Qt::MatchCaseSensitive));
        //modeComboBox->setCurrentIndex(defaultMode);
    }

              //qDebug() << "MainWindow::qsoToEdit: After ALL Mode actions" << endl;

    nameCol = rec.indexOf("rst_sent");
    aux1 = (query.value(nameCol)).toString();
    rstTXLineEdit->setText(aux1);
              //qDebug() << "MainWindow::qsoToEdit: - RST_SENT: " << aux1  << endl;

    nameCol = rec.indexOf("rst_rcvd");
    aux1 = (query.value(nameCol)).toString();
    rstRXLineEdit->setText(aux1);

              //qDebug() << "MainWindow::qsoToEdit: - before switch"  << endl;

    if (contestMode == "DX")
    {
                  //qDebug() << "MainWindow::qsoToEdit: - in default"  << endl;

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
                  //qDebug() << "MainWindow::qsoToEdit: - NAME: " << aux1  << endl;

        if (aux1.length()>0)
        {
            nameLineEdit->setText(aux1);
        }
        else
        {
            nameLineEdit->clear();
        }

        nameCol = rec.indexOf("qth");
        aux1 = (query.value(nameCol)).toString();
        qthLineEdit->setText(aux1);

        nameCol = rec.indexOf("gridsquare");
        aux1 = (query.value(nameCol)).toString();
        //qDebug() << "MainWindow::qsoToEdit: - GRIDSQUARE: " << aux1  << endl;
        locatorLineEdit->setText(aux1);
        satTabWidget->setLocator(aux1);

        nameCol = rec.indexOf("operator");
        aux1 = (query.value(nameCol)).toString();
        //qDebug() << "MainWindow::qsoToEdit: - OPERATOR: " << aux1  << endl;
        myDataTabWidget->setOperator(aux1);

        nameCol = rec.indexOf("station_callsign");
        aux1 = (query.value(nameCol)).toString();
        //qDebug() << "MainWindow::qsoToEdit: - STATIONQRZ: " << aux1  << endl;
        myDataTabWidget->setStationQRZ(aux1);

        nameCol = rec.indexOf("my_gridsquare");
        aux1 = (query.value(nameCol)).toString();
        myDataTabWidget->setMyLocator(aux1);

        nameCol = rec.indexOf("tx_pwr");
        myDataTabWidget->setMyPower((query.value(nameCol)).toDouble());

        nameCol = rec.indexOf("rx_pwr");
        aux1 = (query.value(nameCol)).toString();
        testValueDouble = aux1.toDouble();
        if (testValueDouble >=0)
        {
            rxPowerSpinBox->setValue(testValueDouble);
        }
        else
        {
            rxPowerSpinBox->setValue(0.0);
        }

        nameCol = rec.indexOf("freq");
        aux1 = (query.value(nameCol)).toString();
                  //qDebug() << "MainWindow::qsoToEdit (freq STRING):"  << aux1 << endl;
        testValueDouble = aux1.toDouble();
                  //qDebug() << "MainWindow::qsoToEdit (freq):"  << QString::number(testValueDouble) << endl;

        if ((testValueDouble >0) && (testValueDouble <= txFreqSpinBox->maximum()) )
        {
            txFreqSpinBox->setValue(testValueDouble);
                      //qDebug() << "MainWindow::qsoToEdit: Freq - OverFlow "  << endl;
        }
        else
        {
                      //qDebug() << "MainWindow::qsoToEdit: Freq - OK "  << endl;
            testValueDouble = (dataProxy->getFreqFromBandId(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand()))).toDouble();
            if (testValueDouble>0)
            {
                txFreqSpinBox->setValue(testValueDouble);
            }
            else
            {
                txFreqSpinBox->setValue(0);
            }
        }


        nameCol = rec.indexOf("freq_rx");
        aux1 = (query.value(nameCol)).toString();
                  //qDebug() << "MainWindow::qsoToEdit (freq_rx STRING):"  << aux1 << endl;
        testValueDouble = aux1.toDouble();
                  //qDebug() << "MainWindow::qsoToEdit (freq_rx):"  << QString::number(testValueDouble) << endl;

        if ((testValueDouble >0) && (testValueDouble <= rxFreqSpinBox->maximum()) )
        {
            rxFreqSpinBox->setValue(testValueDouble);
                      //qDebug() << "MainWindow::qsoToEdit: Freq_RX - OverFlow "  << endl;
        }
        else
        {
                      //qDebug() << "MainWindow::qsoToEdit: Freq_RX - OK "  << endl;
            rxFreqSpinBox->setValue(0);
        }

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
                   //qDebug() << "MainWindow::qsoToEdit: - LoTW Sent Status: " << aux1  << endl;

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

                //qDebug() << "MainWindow::qsoToEdit: - just before IOTA"  << endl;

                nameCol = rec.indexOf("iota");
                aux1 = (query.value(nameCol)).toString();

                aux1 = awards->checkIfValidIOTA(aux1);
                othersTabWidget->setIOTA(aux1);

                         //qDebug() << "MainWindow::qsoToEdit: - in default - 100: " << QString::number(currentEntity)  << endl;

                nameCol = rec.indexOf("dxcc");
                aux1  = (query.value(nameCol)).toString();

                         //qDebug() << "MainWindow::qsoToEdit: Checking DXCC: " << aux1 << " - " << world->getEntityName(aux1.toInt()) << endl;

                if (aux1.toInt()>=1)
                {

                    if (aux1.toInt() == util->getNormalizedDXCCValue(currentEntity))
                    {

                    }
                    else
                    {
                        currentEntity = aux1.toInt();
                    }

                             //qDebug() << "MainWindow::qsoToEdit: - in default - 101: " << QString::number(currentEntity)  << endl;
                }
                else
                {
                    currentEntity = world->getQRZARRLId(currentQrz);
                             //qDebug() << "MainWindow::qsoToEdit: - in default - 103: " << QString::number(currentEntity)  << endl;

                }
                         //qDebug() << "MainWindow::qsoToEdit: - in default - 104: " << QString::number(currentEntity)  << endl;

                nameCol = rec.indexOf("prop_mode");
                aux1  = (query.value(nameCol)).toString();
                othersTabWidget->setPropMode(aux1);                
                //slotShowInfoLabel(world->getEntityName(currentEntity), 2);
                infoLabel2->setText(world->getEntityName(currentEntity));
                infoWidget->showEntityInfo(currentEntity);
                //selectCorrectComboBoxEntity(currentEntity);
                         //qDebug() << "MainWindow::qsoToEdit: " << QString::number(currentEntity) << endl;
                othersTabWidget->setEntity(currentEntity);
                          //qDebug() << "MainWindow::qsoToEdit: - in default - 101"  << endl;

                QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
                _qs.clear();
                //TODO: The band sometimes fails here. Check

                _qs << QString::number(currentEntity) << QString::number(dataProxy->getIdFromBandName(mainQSOEntryWidget->getBand())) << QString::number(dataProxy->getIdFromBandName(mainQSOEntryWidget->getMode()))  << QString::number(currentLog);


                          //qDebug() << "MainWindow::qsoToEdit: - in default - 104"  << endl;
                      //qDebug() << "MainWindow:: - calling showStatusOfDXCC-05 " << endl;
                showStatusOfDXCC(_qs);

                          //qDebug() << "MainWindow::qsoToEdit: - in default - 105"  << endl;
                          //qDebug() << "MainWindow::qsoToEdit: - just after de IOTA"  << endl;
    }
    else if (contestMode == "CQ-WW-SSB")
    {

    }
    else
    {
        //TODO: Manage the else... is it a DX or show a message?
    }


              //qDebug() << "MainWindow::qsoToEdit: - in default - 106"  << endl;
    } //Closes the next.isValid
              //qDebug() << "MainWindow::qsoToEdit: - in default - END"  << endl;
    readingTheUI = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);

}

void MainWindow::setModifying(const bool _m)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    modify = _m;
    mainQSOEntryWidget->setModify(_m);
    satTabWidget->setModifying(_m);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotLocatorTextChanged()
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
    //qDebug() << "MainWindow::slotLocatorTextChanged: " << locatorLineEdit->text() << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    int cursorP = locatorLineEdit->cursorPosition();

    locatorLineEdit->setText((locatorLineEdit->text()).toUpper());

    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        //qDebug() << "MainWindow::slotLocatorTextChanged: VALID: " << locatorLineEdit->text() << endl;
        locatorLineEdit->setPalette(palBlack);
        dxLocator = (locatorLineEdit->text());
        infoWidget->showDistanceAndBearing(myDataTabWidget->getMyLocator(), dxLocator);
        satTabWidget->setLocator(dxLocator);
        locatorLineEdit->setToolTip(tr("My QTH locator."));
              //qDebug() << "MainWindow::slotLocatorTextChanged: LAT: " << locator->getLat(locatorLineEdit->text()) << endl;
              //qDebug() << "MainWindow::slotLocatorTextChanged: LON: " << locator->getLon(locatorLineEdit->text()) << endl;
        //showInfoFromLocators(myLocator, dxLocator);
    }
    else
    {
        //qDebug() << "MainWindow::slotLocatorTextChanged: NOT VALID: " << locatorLineEdit->text() << endl;
        locatorLineEdit->setPalette(palRed);
        locatorLineEdit->setToolTip(tr("My QTH locator. Format should be Maidenhead like IN70AA up to 10 characters."));
        locatorLineEdit->setCursorPosition(cursorP);
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }
    locatorLineEdit->setCursorPosition(cursorP);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::slotMyLocatorTextChanged(const QString _loc)
{
           //qDebug() << "MainWindowMy::slotMyLocatorTextChanged: " <<_loc << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if ( locator->isValidLocator(_loc))
    {
        dxccStatusWidget->setMyLocator(_loc);

        slotLocatorTextChanged();

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
          //qDebug() << "MainWindow::showStatusOfDXC: Entity: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
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
        return;
    }
    // Set the status bar with the appropriate message
    int status = awards->getDXStatus (_qs);
    QString message = QString();

          //qDebug() << "MainWindow::showStatusOfDXC: " << QString::number(status) << endl;

    message = awards->getDXStatusString(status);
    slotShowInfoLabel(message);
    //infoLabel1->setText(message);
    //infoWidget->showInfo((_qs.at(0)).toInt(), (_qs.at(1)).toInt(), (_qs.at(2)).toInt(), (_qs.at(3)).toInt() );
    infoWidget->showInfo((_qs.at(0)).toInt());
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}


void MainWindow::showDXMarathonNeeded(const int _dxcc, const int _cqz, const int _year, const int _log)
{
          //qDebug() << "MainWindow::showDXMarathonNeeded" << endl;
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
          //qDebug() << "MainWindow::slotShowAwards"  << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    awards->recalculateAwards();
           //qDebug() << "MainWindow::slotShowAwards-1"  << endl;
    logWindow->refresh();
           //qDebug() << "MainWindow::slotShowAwards-2"  << endl;

    awardsWidget->showAwards();
           //qDebug() << "MainWindow::slotShowAwards-3"  << endl;
    dxccStatusWidget->refresh();
    setMainWindowTitle(QString::number(dataProxy->getHowManyQSOInLog(currentLog)));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
           //qDebug() << "MainWindow::slotShowAwards-END"  << endl;
}



void MainWindow::fillQSOData()
{ // Updates all QSO with the dxcc, CQZ, ... if empty.
          //qDebug() << "MainWindow::fillQSOData" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString stringQuery = QString("SELECT call, bandid, modeid, qso_date, lognumber, id, cqz, ituz, dxcc, cont FROM log WHERE lognumber='%1'").arg(currentLog);

    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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

    QProgressDialog progress(tr("Filling QSOs..."), tr("Abort filling"), 0, numberOfQsos, this);
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


            nameCol = rec.indexOf("lognumber");
            if ( (query.value(nameCol)).isValid() )
            {
                _lognumber = (query.value(nameCol)).toString();
            }
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
                      //qDebug() << "MainWindow::fillQSOData: ID: " << _id << endl;
            //TODO: Prepare this query
            updateString = "UPDATE log SET call = '" + _call + "', bandid = '" + _bandid + "', modeid = '" + _modeid + "', qso_date = '" + _tdate + "', lognumber = '" + _lognumber + "'";//  + "', confirmed = '" + _confirmed + "'";

            nameCol = rec.indexOf("cqz");
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number(world->getQRZCqz(_call));
                updateString = updateString + ", cqz='" + aux1 + "'";
            toModify = true;
            }
            else
            {
            }            

            nameCol = rec.indexOf("ituz");
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number( world->getQRZItuz(_call) );
                updateString = updateString + ", ituz='" + aux1 + "'";
                toModify = true;
            }
            else
            {}
              //qDebug() << "MainWindow::fillQSOData: DXCC" << endl;

            nameCol = rec.indexOf("dxcc");
            _dxcc = (query.value(nameCol)).toInt();
            //if (( (query.value(nameCol)).toString()).length() < 1 )
            if (_dxcc < 1)
            {
                aux1 = QString::number(world->getQRZARRLId(_call) );
                          //qDebug() << "MainWindow::fillQSOData: DXCC proposed: " << aux1 << endl;
                if (aux1.toInt()>0)
                {
                    updateString = updateString + ", dxcc='" + aux1 + "'";
                    toModify = true;
                              //qDebug() << "MainWindow::fillQSOData: DXCC: " << aux1 << endl;
                    _dxcc = aux1.toInt();
                }
                else
                {                    
                              //qDebug() << "MainWindow::fillQSOData: no DXCC identified"  << endl;
                }

            }
            else
            {

                          //qDebug() << "MainWindow::fillQSOData: DXCC already existed"  << endl;
            }
            nameCol = rec.indexOf("cont");
            if (( (query.value(nameCol)).toString()).length() < 2 )
            {
                aux1 = world->getContinentShortName(_dxcc);
                //aux1 = QString::number( world->getQRZItuz(_call) );
                updateString = updateString + ", ituz='" + aux1 + "'";
                toModify = true;
            }
            else
            {}
            _dxcc = -1;
                      //qDebug() << "MainWindow::fillQSOData1: " << updateString << endl;
            if (toModify)
            {
                updateString = updateString + " WHERE id = " + "'" + _id + "'";
                          //qDebug() << "MainWindow::fillQSOData2: " << updateString << endl;
                sqlOK = query1.exec(updateString);
                if (sqlOK)
                {
                              //qDebug() << "MainWindow::fillQSOData: sqlOK=True" << endl;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
                              //qDebug() << "MainWindow::fillQSOData: sqlOK=False" << endl;
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
                          //qDebug() << "MainWindow::fillQSOData3: " << endl;
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
             //qDebug() << "MainWindow::slotUpdateCTYDAT" << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    downloadcty->download();
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotUpdateSATSDAT()
{
           //qDebug() << "MainWindow::slotUpdateSATSDAT" << endl;
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
             //qDebug() << "MainWindow::slotWorldReload" << endl;
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
              //qDebug() << "MainWindow::slotFilePrint" << endl;
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
    cursor.insertText(tr("QRZ"));
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

    printer.setOrientation(QPrinter::Landscape); // For testing, the log will be printed landscape.
    printer.setDocName(stationQRZ+"-log");

    QPrintDialog printDialog(&printer, this);
    printDialog.setWindowTitle(tr("Print Log"));

    if (printDialog.exec() == QDialog::Accepted)
    {
        int _qsos = 0;
        QProgressDialog progress(tr("Printing the log..."), tr("Abort printing"), 0, _numberOfQsos, this);
        progress.setMaximum(_numberOfQsos);
        progress.setWindowModality(Qt::WindowModal);


        QString stringQuery = QString("SELECT id, qso_date, call, rst_sent, rst_rcvd, bandid, modeid, comment FROM log WHERE lognumber='%1'").arg(currentLog);
        sqlOK = query.exec(stringQuery);
        if (!sqlOK)
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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
                          //qDebug() << "MainWindow::slotFilePrint: QSO: " << QString::number(_qsos) << " - Step: " << QString::number(step) << " - Div: " << QString::number(_qsos % step)<< endl;
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
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
                }

                      //qDebug() << "MainWindow::slotFilePrint: Band: " << aux << endl;
                nameCol = rec.indexOf("modeid");
                      //qDebug() << "MainWindow::slotFilePrint: nameCol: " << QString::number(nameCol) << endl;
                aux = (query.value(nameCol)).toString();
                      //qDebug() << "MainWindow::slotFilePrint: Mode1: " << aux << endl;
                aux = dataProxy->getNameFromSubModeId(aux.toInt());
                      //qDebug() << "MainWindow::slotFilePrint: Mode2: " << aux << endl;
                if (aux.length()>1)
                {
                    cursor = textTable->cellAt(row, 6).firstCursorPosition();
                    cursor.insertText(aux);
                }
                else
                {
                          //qDebug() << "MainWindow::slotFilePrint: ERROR: " << aux << endl;
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
             //qDebug() << "MainWindow::slotAnalyzeDxClusterSignal: 1: " << ql.at(0) <<"/1: " << ql.at(1) << "/2: " << ql.at(2) << endl;
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
        else
        {
        }
    }
    else
    { // Signal was not properly emited
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

double MainWindow::checkFreqRanges(double _f)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if ( (_f > double(0)) && (_f <= txFreqSpinBox->maximum()))
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return _f;
    }
    else
    {
        logEvent(Q_FUNC_INFO, "END-2", logSeverity);
        return 0;
    }
}

//void MainWindow::clusterSpotToLog(const QStringList _qs)
void MainWindow::clusterSpotToLog(const QString &_call, const QString &_freq)
{
          //qDebug() << "MainWindow::clusterSpotToLog: " << _call <<"/" << _freq << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString _aux;    
    double _freqN = (_freq.toDouble()) / 1000;
    mainQSOEntryWidget->setQRZ(_call);


    txFreqSpinBox->setValue(checkFreqRanges(_freqN));
    //freqQLCDNumber->display(_freqN);

    _aux = QString::number(_freqN);

              //qDebug() << "MainWindow::clusterSpotToLog - Freq: " << _aux << endl;

    int _bandi = dataProxy->getBandIdFromFreq(_aux.toDouble());
              //qDebug() << "MainWindow::clusterSpotToLog - Bandi: " << QString::number(_bandi) << endl;
    _aux = QString::number(_bandi);
    _aux = QString("SELECT name FROM band WHERE id ='%1'").arg(_aux);

              //qDebug() << "MainWindow::clusterSpotToLog - Band: " << _aux << endl;

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

    logEvent(Q_FUNC_INFO, "END", logSeverity);

}
//DX-CLUSTER - DXCLUSTER

void MainWindow::updateQSLRecAndSent()
{
    //qDebug() << "MainWindow::updateQSLRecAndSent "  << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }

    queryString = QString("UPDATE log SET qsl_sent='N' WHERE qsl_sent ='' AND lognumber='%1'").arg(currentLog);

    sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
         //qDebug() << "MainWindow::updateQSLRecAndSent - END"  << endl;
}



void MainWindow::defineStationCallsign()
{
          //qDebug() << "MainWindow::defineStationCallsign (currentLog): " << QString::number(currentLog) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QString logQRZ;
    logQRZ = dataProxy->getStationCallSignFromLog(currentLog);
          //qDebug() << "MainWindow::defineStationCallsign (logQrz): " << logQRZ << endl;

    if ((world->checkQRZValidFormat(logQRZ)) && (util->isValidCall(logQRZ)))
    {
              //qDebug() << "MainWindow::defineStationCallsign TRUE "  << endl;
        stationQRZ = logQRZ;        
    }
    else
    {
              //qDebug() << "MainWindow::defineStationCallsign FALSE "  << endl;
        stationQRZ = mainQRZ;
    }

          //qDebug() << "MainWindow::defineStationCallsign: " << stationQRZ  << endl;
    filemanager->setStationCallSign(stationQRZ);
          //qDebug() << "MainWindow::defineStationCallsign: AFTER"  << endl;
    myDataTabWidget->setData(myPower, stationQRZ, operatorQRZ, myDataTabWidget->getMyLocator());
    dxccStatusWidget->setMyLocator(myDataTabWidget->getMyLocator());
    searchWidget->setStationCallsign(stationQRZ);
    lotwUtilities->setStationCallSign(stationQRZ);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
           //qDebug() << "MainWindow::defineStationCallsign: " << stationQRZ << " - END" << endl;

}

void MainWindow::slotSetPropMode(const QString &_p)
{
              //qDebug() << "MainWindow::slotSetPropMode: " << _p << endl;
    //if(modify)
    //{
    //    return;
    //}
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    othersTabWidget->setPropMode(_p);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //int indexC = propModeComboBox->findText(" - " + _p + " - ", Qt::MatchContains);
    //propModeComboBox->setCurrentIndex(indexC);
}



void MainWindow::completeWithPreviousQSO(const QString &_call)
{
    //qDebug() << "MainWindow::completeWithPreviousQSO" << endl;
    //This function completes: Name, QTH, Locator, Entity, Iota
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if ((!completeWithPrevious) || (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    //if ( (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    {
        //qDebug() << "MainWindow::completeWithPreviousQSO NOT completing..." << endl;
        if (completedWithPreviousName)
        {
            nameLineEdit->clear();
            completedWithPreviousName = false;
            nameLineEdit->setPalette(palBlack);
        }
        if (completedWithPreviousQTH)
        {
            qthLineEdit->clear();
            completedWithPreviousQTH = false;
            qthLineEdit->setPalette(palBlack);
         }
        if (completedWithPreviousLocator)
        {
            locatorLineEdit->clear();
            completedWithPreviousLocator = false;
            locatorLineEdit->setPalette(palBlack);
        }
        if (completedWithPreviousIOTA)
        {
            othersTabWidget->clearIOTA();
            //iotaContinentComboBox->setCurrentIndex(0);
            //iotaNumberLineEdit->setText("000");
            completedWithPreviousIOTA = false;
            //iotaNumberLineEdit->setPalette(palBlack);
        }
        if (completedWithPreviousQSLVia)
        {
            QSLTabWidget->setQSLVia("");

            //qslViaLineEdit->clear();
            completedWithPreviousQSLVia = false;
            //qslViaLineEdit->setPalette(palBlack);
        }
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    //qDebug() << "MainWindow::completeWithPreviousQSO completing..." << endl;
    QString aux = QString();

    aux = dataProxy->getNameFromQRZ(_call);
    //qDebug() << "MainWindow::completeWithPreviousQSO aux: " << aux << endl;
    //qDebug() << "MainWindow::completeWithPreviousQSO nameLineEdit: " << nameLineEdit->text() << endl;

    //qDebug() << "MainWindow::completeWithPreviousQSO aux length: " << QString::number(aux.length()) << endl;
    //qDebug() << "MainWindow::completeWithPreviousQSO nameL length: " << QString::number((nameLineEdit->text()).length()) << endl;

    if ((aux.length()>=0) && ((nameLineEdit->text()).length()<=0) )
    {
        //qDebug() << "MainWindow::completeWithPreviousQSO name: 1" << endl;
        nameLineEdit->setPalette(palRed);
        completedWithPreviousName = true;
        nameLineEdit->setText(aux);
    }
    else if (completedWithPreviousName && (aux != nameLineEdit->text()))
    {
        //qDebug() << "MainWindow::completeWithPreviousQSO name: 2" << endl;
        nameLineEdit->clear();
        completedWithPreviousName = false;
        nameLineEdit->setPalette(palBlack);
    }
    else
    {
        //qDebug() << "MainWindow::completeWithPreviousQSO name: 3" << endl;
    }

    aux = dataProxy->getQTHFromQRZ(_call);
    if ((aux.length()>=0) && ((qthLineEdit->text()).length()<=0) )
    {
        qthLineEdit->setPalette(palRed);
        completedWithPreviousQTH = true;
        qthLineEdit->setText(aux);
    }
    else if (completedWithPreviousQTH && (aux != qthLineEdit->text()))
    {
        qthLineEdit->clear();
        completedWithPreviousQTH = false;
        qthLineEdit->setPalette(palBlack);

    }

    aux = dataProxy->getLocatorFromQRZ(_call);
    if ((aux.length()>=0) && ((locatorLineEdit->text()).length()<=0) )
    {
        locatorLineEdit->setPalette(palRed);
        locatorLineEdit->setText(aux);
        completedWithPreviousLocator=true;
    }
    else if (completedWithPreviousLocator && (aux != locatorLineEdit->text()))
    {
        locatorLineEdit->clear();
        completedWithPreviousLocator = false;
        locatorLineEdit->setPalette(palBlack);
    }

    aux = dataProxy->getIOTAFromQRZ(_call);
    //othersTabWidget->setIOTA(aux);

    if ((aux.length()>=0) && (othersTabWidget->isIOTAModified()) )
    {

        aux = awards->checkIfValidIOTA(aux);

        if ((aux.length())==6)
        {
            othersTabWidget->setIOTA(aux, false);
            //TODO: Decide if it is better this way or like in : void MainWindowInputQSL::setQSLVia(const QString _qs, QColor qColor)
            //QStringList values = aux.split("-", QString::SkipEmptyParts);
            //iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(values.at(0) ) );
            //iotaNumberLineEdit->setPalette(palRed);
            //iotaNumberLineEdit->setText(values.at(1));
            completedWithPreviousIOTA=true;
        }
        else if (completedWithPreviousIOTA && (aux != othersTabWidget->getIOTA()))
        {
            othersTabWidget->clearIOTA();
            completedWithPreviousName = false;
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

void MainWindow::updateBandComboBox(const QString &_band)
{
          //qDebug() << "MainWindow::updateBandComboBox: " << _band << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    updatingBands = true;
    QString _currentBand = mainQSOEntryWidget->getBand();
    if (!mainQSOEntryWidget->isBandExisting(_currentBand))
    {// The selected frequency is of a band that is not currently selected
              //qDebug() << "MainWindow::updateBandComboBox - New band found: " << _band << endl;
        if (dataProxy->getIdFromBandName(_band) > 1)
        {// Not affected if 0 (light) is the frequency
         // In this case the user should select the band in the setup
                  //qDebug() << "MainWindow::updateBandComboBox- Band is valid: " << _band << endl;
            QStringList qsTemp;
            qsTemp.clear();
            qsTemp << bands;
            qsTemp << _band;
            qsTemp.removeDuplicates();
            bands.clear();
            bands = dataProxy->sortBandNamesBottonUp(qsTemp);
            mainQSOEntryWidget->setBands(bands);
            dxccStatusWidget->setBands(bands);
                  //qDebug() << "MainWindow::updateBandComboBox - Before adding bands!" << endl;
            satTabWidget->addBands(bands);
                  //qDebug() << "MainWindow::updateBandComboBox - Band has been added!" << endl;

        }
        else
        {
                  //qDebug() << "MainWindow::updateBandComboBox- (END) Band is NOT  valid: " <<_band<< endl;
            updatingBands = false;
            logEvent(Q_FUNC_INFO, "END-1", logSeverity);
            return;
        }
    }
    mainQSOEntryWidget->setBand(_currentBand);
    //bandComboBox->setCurrentIndex(bandComboBox->findText(_currentBand, Qt::MatchCaseSensitive));
    updatingBands = false;
    //bandComboBox->setCurrentIndex(bandComboBox->findText(_band, Qt::MatchCaseSensitive));
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::updateBandComboBox- END"  << endl;
}

void MainWindow::slotFreqTXChanged()
{

          //qDebug() << "MainWindow::slotFreqTXChanged" << QString::number(txFreqSpinBox->value()) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    txFreqBeingChanged = true;
    int bandId = dataProxy->getBandIdFromFreq(txFreqSpinBox->value());
    if (bandId > 1)
    { // If the freq belongs to one ham band
       txFreqSpinBox->setPalette(palBlack);
       txFreqSpinBox->setToolTip(tr("TX Frequency in MHz."));
       if ((!modify) && (upAndRunning))
       {
           if (hamlibActive)
           {
            hamlib->setFreq(txFreqSpinBox->value());
           }
           //if (usePSTRotator)
           //{
           //    pstRotator->sendFreq(txFreqSpinBox->value(), 1); // KLog is only able to manage one radio
           //}
       }

        bool freqInBand = dataProxy->isThisFreqInBand(mainQSOEntryWidget->getBand(), QString::number(txFreqSpinBox->value()));
        if(!freqInBand)
        { // If the freq does not belong to the current band, we need to update the band
                  //qDebug() << "MainWindow::slotFreqTXChanged Freq is not in the current band" << endl;
            QString _newBand = dataProxy->getBandNameFromFreq(txFreqSpinBox->value());
            updateBandComboBox(_newBand);
            mainQSOEntryWidget->setBand(_newBand);
            //bandComboBox->setCurrentIndex(bandComboBox->findText(_newBand, Qt::MatchCaseSensitive));
        }
    }
    else
    {        
        txFreqSpinBox->setToolTip(tr("TX Frequency in MHz.\nFrequency is not in a hamradio band!"));
        txFreqSpinBox->setPalette(palRed);
              //qDebug() << "MainWindow::slotFreqTXChanged Freq is not in ANY ham band" << endl;
    }
    if (!txFreqBeingAutoChanged)
    {
              //qDebug() << "MainWindow::slotFreqTXChanged: Updating SAT Uplink" << endl;
        satTabWidget->setUpLinkFreq(txFreqSpinBox->value());

    }

    txFreqBeingChanged = false;

    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::slotFreqTXChanged - END"  << endl;
}

void MainWindow::slotFreqRXChanged()
{
          //qDebug() << "MainWindow::slotFreqRXChanged: " << QString::number(rxFreqSpinBox->value()) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    rxFreqBeingChanged = true;
    int bandId = dataProxy->getBandIdFromFreq(rxFreqSpinBox->value());
    if (bandId > 1)
    { // If the freq belongs to one ham band
       rxFreqSpinBox->setPalette(palBlack);
       rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));

        bool freqInBand = dataProxy->isThisFreqInBand(mainQSOEntryWidget->getBand(), QString::number(rxFreqSpinBox->value()));
        if(!freqInBand)
        { // If the freq does not belong to the current band, we need to update the band
                  //qDebug() << "MainWindow::slotFreqTXChanged Freq is not in the current band" << endl;
            QString _newBand = dataProxy->getBandNameFromFreq(rxFreqSpinBox->value());
            updateBandComboBox(_newBand);
            //bandComboBox->setCurrentIndex(bandComboBox->findText(_newBand, Qt::MatchCaseSensitive));
        }
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
        satTabWidget->setDownLinkFreq(rxFreqSpinBox->value());
    }

    rxFreqBeingChanged = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::slotFreqRXChanged: END" << endl;
}
/*
void MainWindow::slotShowQSOFromDXCCWidget(const int _q)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
           //qDebug() << "MainWindow::slotShowQSOFromDXCCWidget: " << QString::number(_q)<< endl;
}
*/
void MainWindow::slotShowQSOsFromDXCCWidget(QList<int> _qsos)
{
           //qDebug() << "MainWindow::slotShowQSOsFromDXCCWidget" << endl;
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
    //qDebug() << "MainWindow::slotWSJTX-loggedQSO" << endl;
    //logEvent(Q_FUNC_INFO, "Start", logSeverity);
    bool logTheQso = false;
    QString opCall = stationQRZ;
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
        //qDebug() << "MainWindow::slotWSJTX-loggedQSO: DATES NOT VALID " << endl;
        return ;
    }

          //qDebug() << "MainWindow::slotWSJTX-loggedQSO dxcall: " << _dxcall << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO freq: " << QString::number(_freq/1000000) << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO freq no div: " << QString::number(_freq) << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO mode: " << _mode << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO band: " << _band << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO my_grid: " << _mygrid << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO dx_grid: " << _dxgrid << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO comment: " << _comment << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO StationCall: " << _stationcallsign << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO _opCall: " << _opCall << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO opCall: " << opCall << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO time_on: " << util->getDateTimeSQLiteStringFromDateTime(_datetime) << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO time_off: " << util->getDateTimeSQLiteStringFromDateTime(_datetime_off) << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO report_sent: " << _rstTX << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO report_rec: " << _rstRX << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO exchange_sent: " << _exchangeTX << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO exchange_rec: " << _exchangeRX << endl;
          //qDebug() << "MainWindow::slotWSJTX-loggedQSO MY_PWR: " << _mypwr << endl;


    if (wsjtxAutoLog)
    { // Log automatically, without confirmation
        logTheQso = true;
        //qDebug() << "MainWindow::slotWSJTX-loggedQSO: LogTheQSO = true"  << endl;
    }
    else
    { // Ask for confirmation before logging
            //qDebug() << "MainWindow::slotWSJTX-loggedQSO: LogTheQSO = false - we ask for confirmation"  << endl;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog QSO received"));
            msgBox.setTextFormat(Qt::RichText);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
            msgBox.setDefaultButton(QMessageBox::Yes);
            QString aux;
            aux = tr("The following QSO data has been received from WSJT-X to be logged:") + "\n\n" +
                    "<UL>" +
                    "<LI>" +
                    "<b>" + tr("Call") + ": " + "</b>" + _dxcall.toUpper() +
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

        if (logTheQso)
        {
            //qDebug() << "MainWindow::slotWSJTX-loggedQSO: QSO must be logged" << endl;
            bool qsoLogged = false;
            int dxcc = world->getQRZARRLId(_dxcall);
            dxcc = util->getNormalizedDXCCValue(dxcc);

            QString _myLoc = _mygrid;
            if (!(locator->isValidLocator(_myLoc)))
            {
                _myLoc = myDataTabWidget->getMyLocator();
            }


            qsoLogged = dataProxy->addQSOFromWSJTX(_dxcall, _mode, _band,  _freq,
                                                _myLoc, _dxgrid, _rstTX, _rstRX,
                                                _exchangeRX, _exchangeTX, _comment,
                                                _stationcallsign, _name, opCall,
                                                _datetime, _datetime_off, pwr, dxcc, currentLog);


            if (qsoLogged)
            {
                //qDebug() << "MainWindow::slotWSJTX-loggedQSO: Logged QSO OK: " << _dxcall << endl;
                actionsJustAfterAddingOneQSO();
                infoLabel1T = infoLabel1->text();
                infoLabel2T = infoLabel2->text();
                slotShowInfoLabel(tr("QSO logged from WSJT-X:"));
                //slotShowInfoLabel(_dxcall + " - " + dataProxy->getBandNameFromFreq(_freq) + "/" + _mode, 2);
                //infoLabel1->setText(tr("QSO logged from WSJT-X:"));
                infoLabel2->setText(_dxcall + " - " + dataProxy->getBandNameFromFreq(_freq) + "/" + _mode);
                timerInfoBars->start(infoTimeout);
                elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(dataProxy->getLastQSOid()));
            }
            else
            {
                //qDebug() << "MainWindow::slotWSJTX-loggedQSO: Logged QSO NOK: " << _dxcall << endl;
            }
        }
        else
        {
            //qDebug() << "MainWindow::slotWSJTX-loggedQSO: QSO must NOT be logged ... ending" << endl;
        }


    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::slotWSJTX-loggedQSO: - END" << endl;
}

bool MainWindow::checkIfNewMode(const QString &_mode)
{
    //qDebug() << "MainWindow::checkIfNewMode: " << _mode << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (dataProxy->getSubModeIdFromSubMode(_mode)<0)
    {// The mode is not existing; it is not an accepted mode for KLog
     // TODO: Show an error to the user
              //qDebug() << "MainWindow::checkIfNewMode: Mode not valid! - " << _mode << endl;

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
              //qDebug() << "MainWindow::checkIfNewMode: VALID NEW MODE: Adding... - " << _mode << endl;
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
          //qDebug() << "MainWindow::slotStatusFromUDPServer type: " << QString::number(_type) << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer dxcall: " << _dxcall << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer freq: " << QString::number(_freq/1000000) << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer mode: " << _mode << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer report: " << _report << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer de_call: " << _de_call << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer _de_grid: " << _de_grid << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer dx_grid: " << _dx_grid << endl;
          //qDebug() << "MainWindow::slotStatusFromUDPServer sub_mode: " << _sub_mode << endl;

    if ((!mainQSOEntryWidget->isModeExisting(_mode)) && (!noMoreModeErrorShown))
    {
        noMoreModeErrorShown = checkIfNewMode(_mode);
    }



    switch (_type)
    {
        case 0:
            //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT/IN - Heartbeat" << endl;
        break;
        case 1:
                  //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT - Status" << endl;
             mainQSOEntryWidget->setQRZ(_dxcall);
             if ((!noMoreModeErrorShown) && (dataProxy->getSubModeIdFromSubMode(_mode)>0) )
             {
                 mainQSOEntryWidget->setMode(_mode);
                //modeComboBox->setCurrentIndex(modeComboBox->findText(_mode, Qt::MatchCaseSensitive));
             }
             //qDebug() << "MainWindow::slotWSJXstatusFromUDPServer updating txFreqSpinBox" << QString::number(_freq) << endl;
             txFreqSpinBox->setValue(_freq);
             rxFreqSpinBox->setValue(_freq);
             slotUpdateLocator(_dx_grid);
             rstTXLineEdit->setText(_report);
             myDataTabWidget->setMyLocator(_de_grid);
             myDataTabWidget->setStationQRZ(_de_call.toUpper());

             //TODO: Check what to do with _de_call -> Check if _de_call == station callsign and update if needed.
             //TODO: Check what to do with _de_grid -> Check if _de_grid == My Grid and update if needed.
             //TODO: Check what to do with _submode.


             //bandComboBox->setCurrentIndex(bandComboBox->findText(, Qt::MatchCaseSensitive));
            break;
        default: //NO
            //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - ERROR on Type" << endl;
        break;
    }
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::addNewValidMode(const QString &_mode)
{
    //qDebug() << "MainWindow::addNewMode: " << _mode << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    QStringList _newM;
    _newM.clear();
    _newM << _mode;
    _newM << mainQSOEntryWidget->getModes();

    readActiveModes (_newM);
    mainQSOEntryWidget->setModes(modes);

    logEvent(Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "MainWindow::addNewValidMode: END"  << endl;
}

void MainWindow::slotClearNoMorErrorShown()
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    noMoreErrorShown = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString queryFailed)
{
           //qDebug() << "MainWindow::slotQueryErrorManagement: Function: " << functionFailed << endl;
          //qDebug() << "MainWindow::slotQueryErrorManagement: Error N#: " << QString::number(errorCodeN) << endl;
           //qDebug() << "MainWindow::slotQueryErrorManagement: Error: " << functionFailed << errorCodeS << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    if (noMoreErrorShown)
    {
        logEvent(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }

    if ((functionFailed == "virtual bool DataProxy_SQLite::addSatellite(QString, QString, QString, QString, QString)") && (errorCodeN == 19))
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
                        "<li><b>" + tr("Error code") +":</b> " + QString::number(errorCodeN) + "</li>" +
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
      //qDebug() << "MainWindow::defineNewBands: "  << endl;
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
    //bandComboBox->clear();
    //bandComboBox->addItems(bands);
    satTabWidget->addBands(bands);
      //qDebug() << "MainWindow::defineNewBands - END"  << endl;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotSatChangeRXFreq(const double _f)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
      //qDebug() << "MainWindow::slotSatChangeRXFreq updating rxFreqSpinBox" << QString::number(_f) << endl;
    rxFreqBeingAutoChanged = true;
    rxFreqSpinBox->setValue(_f);
    rxFreqBeingAutoChanged = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
}

void MainWindow::slotSatChangeTXFreq(const double _f)
{
      //qDebug() << "MainWindow::slotSatChangeTXFreq updating txFreqSpinBox" << QString::number(_f) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    txFreqBeingAutoChanged = true;
    txFreqSpinBox->setValue(_f);
    txFreqBeingAutoChanged = false;
    logEvent(Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "MainWindow::slotSatChangeTXFreq updating txFreqSpinBox - END"  << endl;
}

void MainWindow::slotSatTXFreqNeeded(const double _f)
{
          //qDebug() << "MainWindow::slotSatTXFreqNeeded: " << QString::number(_f) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString _band = dataProxy->getBandNameFromFreq(_f);
    if (!mainQSOEntryWidget->isBandExisting(_band))
    {
        updateBandComboBox(_band);
    }
    slotSatChangeTXFreq(_f);

    satTabWidget->setUpLinkFreq(_f);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::slotSatTXFreqNeeded - END " << endl;
}

void MainWindow::slotSatRXFreqNeeded(const double _f)
{
          //qDebug() << "MainWindow::slotSatRXFreqNeeded: " << QString::number(_f) << endl;
    logEvent(Q_FUNC_INFO, "Start", logSeverity);

    QString _band = dataProxy->getBandNameFromFreq(_f);
    if (!mainQSOEntryWidget->isBandExisting(_band))
    {
        updateBandComboBox(_band);
    }
    slotSatChangeRXFreq(_f);
    satTabWidget->setDownLinkFreq(_f);
    logEvent(Q_FUNC_INFO, "END", logSeverity);
          //qDebug() << "MainWindow::slotSatRXFreqNeeded - END " << endl;
}

void MainWindow::slotHamlibTXFreqChanged(const double _f)
{
    logEvent(Q_FUNC_INFO, "Start", logSeverity);
    if (upAndRunning)
    {
        txFreqSpinBox->setValue(_f);
    }

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
           //qDebug() << "slotHamlibModeChanged: Mode in the Combobox: " << endl;
        mainQSOEntryWidget->setMode(_m);
        //modeComboBox->setCurrentIndex(modeComboBox->findText(_m, Qt::MatchCaseSensitive));
    }
    else
    {
              //qDebug() << "MainWindow::slotHamlibModeChanged: Mode not found in combobox" << _m << endl;
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
    //qDebug() << "MainWindow::slotUpdateLocator: " << _loc<< endl;
    locatorLineEdit->setText(_loc.toUpper());
    //qDebug() << "MainWindow::slotUpdateLocator - END" << endl;
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

void MainWindow::slotShowInfoLabel(const QString _m)
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

void MainWindow::setSeverity(const int _sev)
{
    logSeverity = _sev;
    setupDialog->setSeverity(logSeverity);
}

void MainWindow::slotCaptureDebugLogs(const QString &_func, const QString &_msg, const int _level)
{
          //qDebug() << "MainWindow::slotCaptureDebugLogs: " << _func << "_/" << _msg << QString::number(_level) << endl;
    logEvent(_func, _msg, _level);
}

void MainWindow::logEvent(const QString &_func, const QString &_msg, const int _level)
{   //This function is the only one not logging the activity
    if ((!logEvents) | (!debugFileOpen) | (_level<=7)) // Increase to 7 show the full Debug
    {
        //qDebug() << "MainWindow::slotCaptureDebugLogs: Not logging: " << _func << " / " << _msg << " / " << QString::number(_level) << endl;
        return;
    }
    //Criticality

    QTextStream out(debugFile);
    out << (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmsszzz") << " - " << _func << " - " << _msg << endl;
}
