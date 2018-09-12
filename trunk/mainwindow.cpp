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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
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



MainWindow::MainWindow(const QString _klogDir, const QString tversion)
{

    //qDebug() << "MainWindow::MainWindow: "<<  _klogDir << " Ver: " << tversion << endl;
       //qDebug() << "MainWindow::MainWindow: Con func: "<<  Q_FUNC_INFO << endl;

   QTime start;
   start = QTime::currentTime();
     //qDebug() << "MainWindow::MainWindow: "<<  (QTime::currentTime()).toString("hhmmsszzz")<< endl;

    showErrorDialog = new ShowErrorDialog();
    UDPLogServer = new UDPServer();

    upAndRunning = false; // To define some actions that can only be run when starting the software
    //connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotDownloadFinished(QNetworkReply*))); // To download cty.csv
    //flagIcon = new QPushButton; // To paint a flag of the worked entity

    // <ui>
    softwareVersion = tversion;
    itIsANewversion = false;
    dataProxy = new DataProxy_SQLite(softwareVersion, Q_FUNC_INFO);
    doc = new QTextDocument;
    util = new Utilities;

    needToEnd = false;
    cleaning = false;
    qrzAutoChanging = false;
    dxclusterServerToConnect = "dxfun.com";
    dxclusterServerPort = 8000;
    contestMode = "DX";
    infoTimeout = 2000; // default timeout

    defaultADIFLogFile = "klog.adi";


    klogDir = _klogDir;


    InValidCharsInPrevCall = false;
    //stationCallSignShownInSearch = true;
    checkNewVersions = true;
    reportInfo = false;
    configured = false;
    modify = false;
    noMoreErrorShown = false;
    noMoreModeErrorShown = false;
    qslingNeeded = false; // When clicking on Find QSO to QSL
    manageMode = false;
    selectedYear = 0;
    defaultMode = 0;
    defaultBand = 0;
    //qDebug() << "MainWindow::MainWindow: 1 - currentMode: " << QString::number(currentMode) << endl;
    currentMode = 1;
      //qDebug() << "MainWindow::MainWindow: 2 - currentMode: " << QString::number(currentMode) << endl;
    currentModeShown = currentMode;
    currentBand = 0;
    currentBandShown = currentBand;
    currentLog = 1;
    points = 0;
    multipliers = 0;
    qsoPoints = 0;
    qsoMultiplier = 0;
    operatorQRZ = "";
    stationQRZ = "";
    mainQRZ = "";
    myLocator = "";
    dxLocator ="";
    myPower = 0.0;
    UDPServerStart = false;   // By default the UDP server is started

    //lastPower = myPower;
    //lastOperatorQRZ = operatorQRZ;
    //lastStationQRZ = stationQRZ;
    //lastMyLocator = myLocator;


      //qDebug() << "MainWindow::MainWindow: 0007" << endl;
    //entitiesList.clear();
    //propModeList.clear();
    currentEntity = -1; // To optimize the calls to different world methods if the entity does not change. Used in slotQRZTextChanged
    previousEntity = -1;// To optimize the calls to different world methods if the entity does not change.
    realTime=true;
    UTCTime=true;
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

    dxClusterShowHF=true;
    dxClusterShowVHF=true;
    dxClusterShowWARC=true;
    dxClusterShowWorked=true;
    dxClusterShowConfirmed=true;
    dxClusterShowAnn=true;
    dxClusterShowWWV=true;
    dxClusterShowWCY=true;

    keepSatPage = false;
    //qDebug() << "MainWindow::MainWindow: 0008" << endl;
    clublogActive = false;
    clublogRealTime = false;
    clublogUser = QString();
    clublogPass = QString();
    clublogEmail = QString();

    infoLabel1T = QString();
    infoLabel2T = QString();
/*
    db = new DataBase(Q_FUNC_INFO, softwareVersion, util->getKLogDBFile());
    if (!db->createConnection())
    {
           //qDebug() << "MainWindow::MainWindow: Conection not created" << endl;
        return;
    }
    else
    {
        //db->updateIfNeeded(); // Check if we need to update the DB
           //qDebug() << "MainWindow::MainWindow: DB updated was checked here" << endl;
    }

*/
    elogClublog = new eLogClubLog();
    clublogAnswer = -1;

    defaultColor.setNamedColor("slategrey");
    neededColor.setNamedColor("yellow");
    workedColor.setNamedColor("blue");
    confirmedColor.setNamedColor("red");
    newOneColor.setNamedColor("green");

    statsWidget = new StatisticsWidget(dataProxy);
    //statsWidget->show();
    //Default band/modes
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW" << "RTTY";

    dxccStatusWidget = new DXCCStatusWidget(dataProxy);
    logWindow = new LogWindow(dataProxy, this);
    connect(logWindow, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    searchWidget = new SearchWidget (dataProxy, this);
    connect(searchWidget, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    infoWidget = new InfoWidget(dataProxy, this);

    //qDebug() << "MainWindow::MainWindow: 0009" << endl;

    //helpHelpDialog = new HelpHelpDialog(softwareVersion);
      //qDebug() << "MainWindow::MainWindow: 00091" << endl;
    //helpAboutDialog = new HelpAboutDialog(softwareVersion);
    aboutDialog = new AboutDialog(softwareVersion);      

      //qDebug() << "MainWindow::MainWindow: 0010" << endl;

    recalculateAwardsButton = new QPushButton(tr("Recalculate"), this);
    recalculateAwardsButton->setToolTip(tr("Click to recalculate the award status."));

    scoreTextEdit = new QTextEdit;

    //distShortLabelN = new QLabel;
    //distLongLabelN = new QLabel;

    configFileName = util->getCfgFile();
    ctyDatFile = util->getCTYFile();

    downloadcty = new DownLoadCTY(klogDir, softwareVersion);
    connect( downloadcty, SIGNAL(done()), this, SLOT(slotWorldReload()) );

    //qDebug() << "MainWindow::MainWindow: logbook: " << QString(util->getKLogDBFile()) << endl;

        //qDebug() << "MainWindow::MainWindow: Before existing Data" << endl;
        bool existingData = QFile::exists(util->getKLogDBFile());
        //qDebug() << "MainWindow::MainWindow: After existing Data" << endl;

        if (existingData)
        {
               //qDebug() << "MainWindow::MainWindow: existing data" << endl;
           //configured= false;
        }
        else
        {
               //qDebug() << "MainWindow::MainWindow: NOT existing data" << endl;
        }

    statusBarMessage = tr("Starting KLog");

    if (!QDir::setCurrent ( klogDir )){
        QDir d1(klogDir);
        if (d1.mkdir(klogDir))
        {
            QDir::setCurrent ( klogDir );
        }
    }

    //qDebug() << "MainWindow::MainWindow: 3" << endl;


    DBinMemory = false;
    //db = new DataBase(softwareVersion, DBinMemory);

    //qDebug() << "MainWindow::MainWindow: 4" << endl;
    world = new World(dataProxy, klogDir, softwareVersion);
    connect(world, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );

    if (!existingData)
    {
           //qDebug() << "MainWindow::MainWindow: !existingData" << endl;
        world->create(ctyDatFile);
        //entitiesList = world->getEntitiesNames();
        //createData();
    }else
    {
           //qDebug() << "MainWindow::MainWindow: existingData" << endl;
    }
    //qDebug() << "MainWindow::MainWindow: proxy to be created" << endl;


    connect(dataProxy, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    connect(this, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );


    //qDebug() << "MainWindow::MainWindow: setupDialog to be created" << endl;
    //setupDialog = new SetupDialog(!configured);
    setupDialog = new SetupDialog(dataProxy, configFileName, softwareVersion, 0, !configured);
    connect(setupDialog, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    //qDebug() << "MainWindow::MainWindow: satTabWidget to be created" << endl;
    satTabWidget = new MainWindowSatTab(dataProxy);
    connect(satTabWidget, SIGNAL(newBandsToBeAdded(QStringList)), this, SLOT(slotDefineNewBands(QStringList)) );
    connect(satTabWidget, SIGNAL(rxFreqChanged(QString)), this, SLOT(slotChangeRXFreq(QString)) );
    connect(satTabWidget, SIGNAL(txFreqChanged(QString)), this, SLOT(slotChangeTXFreq(QString)) );
    connect(satTabWidget, SIGNAL(dxLocatorChanged(QString)), this, SLOT(slotUpdateLocator(QString)) );



    myDataTabWidget = new MainWindowMyDataTab();
    commentTabWidget = new MainWindowInputComment();
    othersTabWidget = new MainWindowInputOthers(dataProxy);
    eQSLTabWidget = new MainWindowInputEQSL(dataProxy);
    QSLTabWidget = new MainWindowInputQSL(dataProxy);

    //qDebug() << "MainWindow::MainWindow: fileManager to be created" << endl;
    //filemanager = new FileManager(klogDir, softwareVersion, *db);

       //qDebug() << "MainWindow::MainWindow: locator to be created" << endl;
    locator = new Locator();
       //qDebug() << "MainWindow::MainWindow: awards to be created" << endl;


       //qDebug() << "MainWindow::MainWindow: awards already created" << endl;
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    //qDebug() << "MainWindow::MainWindow: 8" << endl;
    dateTime = new QDateTime();
    selectedYear = (dateTime->currentDateTime()).date().year();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()) );
    timer->start(1000);
    timerInfoBars = new QTimer(this);
    connect(timerInfoBars, SIGNAL(timeout()), this, SLOT(slotTimeOutInfoBars()) );


    previousQrz = "";
    qrzLineEdit = new QLineEdit;

    nameLineEdit = new QLineEdit;
    qthLineEdit = new QLineEdit;
    locatorLineEdit = new QLineEdit;
    rstTXLineEdit = new QLineEdit;
    rstRXLineEdit = new QLineEdit;
    STXLineEdit = new QLineEdit;
    SRXLineEdit = new QLineEdit;
    bandComboBox = new QComboBox;
    modeComboBox = new QComboBox;
    //bandComboBox->setObjectName("bandCombo");
    //modeComboBox->setObjectName("modeCombo");
    //qrzLineEdit->setObjectName("qrzLine");

    dateEdit = new QDateEdit;
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    timeEdit = new QTimeEdit;

    OKButton = new QPushButton(tr("&Add"), this);
    //spotItButton = new QPushButton(tr("&Spot"), this);
    //spotItButton->setEnabled(false);
    clearButton = new QPushButton(tr("&Clear"), this);

    // UI DX
    infoLabel1 = new QLabel(tr("Status bar..."));
    infoLabel2 = new QLabel(tr("DX Entity"));

    //logPanel = new QWidget;
    loggWinAct = new QAction(tr("&Log Window"), this);
    scoreeWinAct = new QAction(tr("&Score Window"), this);

    scoreWindow = new QWidget;
    operatorLineEdit = new QLineEdit;
    stationCallSignLineEdit = new QLineEdit;
    myLocatorLineEdit = new QLineEdit;

    //myPowerSpinBox = new QDoubleSpinBox;
    //myPowerSpinBox->setDecimals(2);
    //myPowerSpinBox->setMaximum(9999);

    rxPowerSpinBox = new QDoubleSpinBox;
    rxPowerSpinBox->setDecimals(2);
    rxPowerSpinBox->setMaximum(9999);
    rxPowerSpinBox->setSuffix(" " + tr("Watts"));

    txFreqSpinBox = new QDoubleSpinBox;
    txFreqSpinBox->setDecimals(3);
    txFreqSpinBox->setMaximum(9999);
    txFreqSpinBox->setSuffix(" " + tr("MHz"));

    rxFreqSpinBox = new QDoubleSpinBox;
    rxFreqSpinBox->setDecimals(3);
    rxFreqSpinBox->setMaximum(9999);
    rxFreqSpinBox->setSuffix(" " + tr("MHz"));

    dxccConfirmedQLCDNumber = new QLCDNumber;
    dxccWorkedQLCDNumber = new QLCDNumber;
    wazConfirmedQLCDNumber = new QLCDNumber;
    wazWorkedQLCDNumber = new QLCDNumber;
    localConfirmedQLCDNumber = new QLCDNumber;
    localWorkedQLCDNumber = new QLCDNumber;
    qsoConfirmedQLCDNumber = new QLCDNumber;
    qsoWorkedQLCDNumber = new QLCDNumber;
    dxMarathonQSOLCDNumber = new QLCDNumber;
    dxMarathonDXCCQLCDNumber = new QLCDNumber;
    dxMarathonCQQLCDNumber = new QLCDNumber;
    dxMarathonPointsQLCDNumber = new QLCDNumber;
    operatingYearsComboBox = new QComboBox;

    qsoWorkedQLCDNumber->setDigitCount(7);
    qsoConfirmedQLCDNumber->setDigitCount(7);


    // Check date & time and set them in the UI at the begining
    dateTime->currentDateTime();
    dateEdit->setDate((dateTime->currentDateTime()).date());
    timeEdit->setTime((dateTime->currentDateTime()).time());

    //Search tab
    //searchBoxLineEdit = new QLineEdit;

    // UI DX

    // CLUSTER
    //qDebug() << "MainWindow::MainWindow: dxclusterwidget to be created" << endl;
    dxClusterWidget = new DXClusterWidget(dataProxy, dxclusterServerToConnect , dxclusterServerPort, this);


    // </CLUSTER>
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    awards = new Awards(dataProxy);
    awards->setManageModes(manageMode);
    // </UI>

//**************************************************

    //createDXClusterUI();
    connect( setupDialog, SIGNAL(exitSignal(int)), this, SLOT(slotExitFromSlotDialog(int)) );

    //qDebug() << "MainWindow::MainWindow:  readconfigdata" << endl;
    readConfigData();
    //qDebug() << "MainWindow::MainWindow:  after readconfigdata" << endl;
    if (itIsANewversion)
    {
        slotSetup();
    }
    //qDebug() << "MainWindow::MainWindow:  after readconfigdata" << endl;
    if (needToEnd)
    {
        //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
      //db->compress();
       //qDebug() << "MainWindow::MainWindow: 12.5" << endl;
       exit(0);
    }

    //qDebug() << "MainWindow::MainWindow:  UI to be created" << endl;


    logWindow->createlogPanel(currentLog);

    createUI();


    //createSearchResultsPanel();
    loggWinAct->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(loggWinAct, SIGNAL(triggered()), this, SLOT(slotLogWinShow()));

    //logPanel->addAction(loggWinAct);
    //logPanel->addAction(scoreeWinAct);

    scoreeWinAct->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(scoreeWinAct, SIGNAL(triggered()), this, SLOT(slotScoreWinShow()));
    scoreWindow->addAction(scoreeWinAct);
    scoreWindow->addAction(loggWinAct);
    createScorePanel();

    setWindowTitle(tr("KLog"));

    //qDebug() << "MainWindow::MainWindow: 16" << endl;
    if (dataProxy->getNumberOfManagedLogs()<1)
    {
        //qDebug() << "MainWindow::MainWindow: 16.1" << endl;
        slotSetup(6);
        //qDebug() << "MainWindow::MainWindow: 16.2" << endl;
    }
    //qDebug() << "MainWindow::MainWindow: 17" << endl;
    checkIfNewBandOrMode();
    //qDebug() << "MainWindow::MainWindow: 18" << endl;

    if (contestMode == "DX")
    {
        //qDebug() << "MainWindow::MainWindow: DX! 18.3" << endl;
        if (dataProxy->getLastQSOid()<=1)
        {
           //qDebug() << "MainWindow::MainWindow: 18.4" << endl;
            operatingYearsComboBox->addItem(QString::number(selectedYear));
        }
        else
        {
            //qDebug() << "MainWindow::MainWindow: 18.5 - currentLog: " << QString::number(currentLog) << endl;
            operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            //qDebug() << "MainWindow::MainWindow: 18.5.1 - currentLog: " << QString::number(currentLog) << endl;

            QStringList a;
            a.clear();
            a << dataProxy->getOperatingYears(currentLog);
            //qDebug() << "MainWindow::MainWindow: 18.5.1.1 - currentLog: " << QString::number(currentLog) << endl;
            if (!a.isEmpty())
            {
                //qDebug() << "MainWindow::MainWindow: 18.5.1.2 - currentLog: " << QString::number(currentLog) << endl;
                operatingYearsComboBox->setCurrentIndex(operatingYearsComboBox->findText(a.last(), Qt::MatchCaseSensitive));
                //qDebug() << "MainWindow::MainWindow: 18.5.1.3 - currentLog: " << QString::number(currentLog) << endl;
            }


            //qDebug() << "MainWindow::MainWindow: 18.5.2" << endl;
        }
        //qDebug() << "MainWindow::MainWindow: 18.6." << endl;

        //awards->recalculateAwards();
        //qDebug() << "MainWindow::MainWindow: 18.8" << endl;
        showAwards();
        //qDebug() << "MainWindow::MainWindow: 18.9" << endl;
        dxClusterWidget->setCurrentLog(currentLog);
        //qDebug() << "MainWindow::MainWindow: 18.10" << endl;
    }
    else if ((contestMode == "CQ-WW-SSB") || (contestMode == "CQ-WW-CW"))
    {}
    else
    {
        //TODO: Check how to do DX if nothing happens without duplicating code.
           //qDebug() << "MainWindow::MainWindow: DX! 18.3" << endl;
        if (dataProxy->getLastQSOid()<=1)
        {
               //qDebug() << "MainWindow::MainWindow: 18.4" << endl;
            operatingYearsComboBox->addItem(QString::number(selectedYear));
        }
        else
        {
               //qDebug() << "MainWindow::MainWindow: 18.5 - currentLog: " << QString::number(currentLog) << endl;
            operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
               //qDebug() << "MainWindow::MainWindow: 18.5.1 - currentLog: " << QString::number(currentLog) << endl;
            operatingYearsComboBox->setCurrentIndex(operatingYearsComboBox->findText((dataProxy->getOperatingYears(currentLog)).last(), Qt::MatchCaseSensitive));
               //qDebug() << "MainWindow::MainWindow: 18.5.2" << endl;
        }
               //qDebug() << "MainWindow::MainWindow: 18.6." << endl;

               //qDebug() << "MainWindow::MainWindow: 18.7" << endl;
        awards->recalculateAwards();
               //qDebug() << "MainWindow::MainWindow: 18.8" << endl;
        showAwards();
           //qDebug() << "MainWindow::MainWindow: 18.9" << endl;
        dxClusterWidget->setCurrentLog(currentLog);
           //qDebug() << "MainWindow::MainWindow: 18.10" << endl;

    }

    //qDebug() << "MainWindow::MainWindow: 19" << endl;
    currentBandShown = dataProxy->getIdFromBandName(bandComboBox->currentText());
    currentModeShown = dataProxy->getIdFromModeName(modeComboBox->currentText());
    currentBand = currentBandShown;
    currentMode = currentModeShown;
       //qDebug() << "MainWindow::MainWindow: 20 - currentMode: " << QString::number(currentMode) << endl;
      //qDebug() << "MainWindow::MainWindow: 21 - currentBand: " << QString::number(currentBand) << endl;
      //qDebug() << "MainWindow::MainWindow: 21.1 - currentModeShown: " << QString::number(currentModeShown) << endl;
    //qDebug() << "MainWindow::MainWindow: 21.2 - currentBandShown: " << QString::number(currentBandShown) << endl;


    slotClearButtonClicked();

    upAndRunning = true;

       //qDebug() << "MainWindow::MainWindow: "<<  (QTime::currentTime()).toString("hhmmsszzz")<< endl;

    //qDebug() << "MainWindow::MainWindow: Software update to be created" << endl;
    softUpdate = new SoftwareUpdate(softwareVersion);
    //connect(softUpdate, SIGNAL(updateNeededSignal(bool)), this, SLOT(slotShowSoftUpdateResults(bool) ) );
    callingUpdate = false; // to control whether the update is mannually launched or at the begining


       //qDebug() << "MainWindow::MainWindow: calling Software update..." << endl;
    if (checkNewVersions)
    {//reportInfo
        if (reportInfo)
        {
            softUpdate->addCall(stationQRZ);

        }
        softUpdate->needToUpdate();
    }

    connect(awards, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    connect(awards, SIGNAL(awardDXCCUpdated()), this, SLOT(slotRefreshDXCCWidget()) );

    filemanager = new FileManager(dataProxy, klogDir, softwareVersion);
    connect(filemanager, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );


    //qDebug() << "MainWindow::MainWindow: END" << endl;
}

MainWindow::~MainWindow()
{
    /*
    doc->~QTextDocument();
    recalculateAwardsButton->~QPushButton();
    scoreTextEdit->~QTextEdit();
    mainWidget->~QWidget();
    dateTime->~QDateTime();
    timer->~QTimer();
    qrzLineEdit->~QLineEdit();
    nameLineEdit->~QLineEdit();
    qthLineEdit->~QLineEdit();
    locatorLineEdit->~QLineEdit();
    rstTXLineEdit->~QLineEdit();
    rstRXLineEdit->~QLineEdit();
    STXLineEdit->~QLineEdit();
    SRXLineEdit->~QLineEdit();
    bandComboBox->~QComboBox();
    modeComboBox->~QComboBox();

    dateEdit->~QDateEdit();
    timeEdit->~QTimeEdit();

    OKButton->~QPushButton();
    clearButton->~QPushButton();

    infoLabel1->~QLabel();
    infoLabel2->~QLabel();

    loggWinAct->~QAction();
    scoreeWinAct->~QAction();

    scoreWindow->~QWidget();
    operatorLineEdit->~QLineEdit();
    stationCallSignLineEdit->~QLineEdit();
    myLocatorLineEdit->~QLineEdit();


    rxPowerSpinBox->~QDoubleSpinBox();
    txFreqSpinBox->~QDoubleSpinBox();
    rxFreqSpinBox->~QDoubleSpinBox();

    dxccConfirmedQLCDNumber->~QLCDNumber();
    dxccWorkedQLCDNumber->~QLCDNumber();
    wazConfirmedQLCDNumber->~QLCDNumber();
    wazWorkedQLCDNumber->~QLCDNumber();
    localConfirmedQLCDNumber->~QLCDNumber();
    localWorkedQLCDNumber->~QLCDNumber();
    qsoConfirmedQLCDNumber->~QLCDNumber();
    qsoWorkedQLCDNumber->~QLCDNumber();
    dxMarathonQSOLCDNumber->~QLCDNumber();
    dxMarathonDXCCQLCDNumber->~QLCDNumber();
    dxMarathonCQQLCDNumber->~QLCDNumber();
    dxMarathonPointsQLCDNumber->~QLCDNumber();
    operatingYearsComboBox->~QComboBox();

    showErrorDialog->~ShowErrorDialog();
    dataProxy->~DataProxy();
    db->~DataBase();
    util->~Utilities();
    elogClublog->~eLogClubLog();
    dxccStatusWidget->~DXCCStatusWidget();
    logWindow->~LogWindow();
    searchWidget->~SearchWidget();
    aboutDialog->~AboutDialog();
    infoWidget->~QWidget();
    downloadcty->~DownLoadCTY();
    world->~World();
    setupDialog->~SetupDialog();
    satTabWidget->~MainWindowSatTab();
    myDataTabWidget->~MainWindowMyDataTab();
    commentTabWidget->~MainWindowInputComment();
    othersTabWidget->~MainWindowInputOthers();
    eQSLTabWidget->~QWidget();
    QSLTabWidget->~MainWindowInputQSL();
    filemanager->~FileManager();
    locator->~Locator();
    awards->~Awards();
    dxClusterWidget->~DXClusterWidget();
    softUpdate->~SoftwareUpdate();
    */
}



void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createUI()
{

    //qDebug() << "MainWindow::createUI" << endl;
    createStatusBar();

    if (contestMode == "CQ-WW-SSB")
    {
        createUIDX();
        createActionsCommon();
        createActionsDX();
        createMenusCommon();

    }
    else if ( (contestMode == "CQ-WW-SSB") || (contestMode == "CQ-WW-CW") )
    {
        createUICQWW();
        createActionsCommon();
        createActionsCQWW();
        createMenusCommon();
        createMenusCQWW();

    }
    else
    { // This is DX. Depending on how KLog evolves I could remove the DX from the first place and leave this only.
        createUIDX();
        createActionsCommon();
        createActionsDX();
        createMenusCommon();
    }


}

void MainWindow::slotTimeOutInfoBars()
{
    infoLabel1->setText(infoLabel1T);
    infoLabel2->setText(infoLabel2T);
}

void MainWindow::slotModeComboBoxChanged()
{
      //qDebug() << "MainWindow::slotModeComboBoxChanged: " << QString::number(modeComboBox->currentIndex()) << endl;
      //qDebug() << "MainWindow::slotModeComboBoxChanged: " << modeComboBox->currentText() << endl;
/*
    int i;
    i = dataProxy->getSubModeIdFromSubMode(modeComboBox->currentText());
    if (i>=0)
    {
          //qDebug() << "MainWindow::MainWindow: 5 - currentMode: " << QString::number(currentMode) << endl;
        currentMode = i;
          //qDebug() << "MainWindow::MainWindow: 6 - currentMode: " << QString::number(currentMode) << endl;
    }
    //qDebug() << "MainWindow::slotModeComboBoxChanged: i: " << QString::number(i) << endl;
*/

    //qDebug() << "MainWindow::slotModeComboBoxChanged: currentMode: " << QString::number(currentMode) << endl;

       //qDebug() << "MainWindow::slotModeComboBoxChanged: " << QString::number(modeComboBox->currentIndex()) << "/" << QString::number(currentMode) << endl;
    //qDebug() << "MainWindow::slotModeComboBoxChanged: currentBandShown: " << QString::number(currentBandShown) << endl;
    currentBandShown = dataProxy->getIdFromBandName(bandComboBox->currentText());
    //qDebug() << "MainWindow::slotModeComboBoxChanged: currentBandShown2: " << QString::number(currentBandShown) << endl;
    currentModeShown = dataProxy->getIdFromModeName(modeComboBox->currentText());
    //qDebug() << "MainWindow::slotModeComboBoxChanged: currentBand: " << QString::number(currentBand) << endl;
    currentBand = currentBandShown;
    //qDebug() << "MainWindow::slotModeComboBoxChanged: currentBand2: " << QString::number(currentBand) << endl;
      //qDebug() << "MainWindow::MainWindow: 7 - currentMode: " << QString::number(currentMode) << endl;
    currentMode = currentModeShown;
      //qDebug() << "MainWindow::MainWindow: 8 - currentMode: " << QString::number(currentMode) << endl;
      //qDebug() << "MainWindow::MainWindow: 8.1 - currentBand: " << QString::number(currentBand) << endl;
      //qDebug() << "MainWindow::MainWindow: 8.2 - currentModeShown: " << QString::number(currentModeShown) << endl;
      //qDebug() << "MainWindow::MainWindow: 8.3 - currentBandShown: " << QString::number(currentBandShown) << endl;

    checkIfWorkedB4(currentQrz);

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);
    showStatusOfDXCC(_qs);


       //qDebug() << "MainWindow::slotModeComboBoxChanged2: " << modeComboBox->currentText() << endl;
}

void MainWindow::slotBandComboBoxChanged(){
    //qDebug() << "MainWindow::slotBandComboBoxChanged: " << QString::number(bandComboBox->currentIndex()) << bandComboBox->currentText()<< endl;
/*
    int i;
    i = dataProxy->getIdFromBandName(bandComboBox->currentText());
    if (i>=0)
    {
        currentBand = i;
        //txFreqSpinBox->setValue(dataProxy->getFreqFromBandId(i));
    }
*/
    //qDebug() << "MainWindow::slotBandComboBoxChanged: " << QString::number(bandComboBox->currentIndex()) << "/" << QString::number(currentBand) << endl;

    //qDebug() << "MainWindow::slotBandComboBoxChanged: currentBandShown: " << QString::number(currentBandShown) << endl;
    currentBandShown = dataProxy->getIdFromBandName(bandComboBox->currentText());
    //qDebug() << "MainWindow::slotBandComboBoxChanged: currentBandShown2: " << QString::number(currentBandShown) << endl;
    currentModeShown = dataProxy->getIdFromModeName(modeComboBox->currentText());
    //qDebug() << "MainWindow::slotBandComboBoxChanged: currentBand: " << QString::number(currentBand) << endl;
    currentBand = currentBandShown;
    //qDebug() << "MainWindow::slotBandComboBoxChanged: currentBand2: " << QString::number(currentBand) << endl;
       //qDebug() << "MainWindow::MainWindow: 9 - currentMode: " << QString::number(currentMode) << endl;
    currentMode = currentModeShown;
       //qDebug() << "MainWindow::MainWindow: 9 - currentMode: " << QString::number(currentMode) << endl;
      //qDebug() << "MainWindow::MainWindow: 9.1 - currentMode: " << QString::number(currentMode) << endl;
      //qDebug() << "MainWindow::MainWindow: 9.2 - currentBand: " << QString::number(currentBand) << endl;
      //qDebug() << "MainWindow::MainWindow: 9.3 - currentModeShown: " << QString::number(currentModeShown) << endl;
      //qDebug() << "MainWindow::MainWindow: 9.4 - currentBandShown: " << QString::number(currentBandShown) << endl;
    //qDebug() << "MainWindow::MainWindow: Going to update the UpLink with: " << bandComboBox->currentText() << endl;
    satTabWidget->setUpLink(bandComboBox->currentText());
       //currentModeShown = modeComboBox->currentIndex();
    checkIfWorkedB4(currentQrz);

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);


    showStatusOfDXCC(_qs);
}


void MainWindow::slotQRZReturnPressed()
{
      //qDebug() << "MainWindow::slotQRZReturnPressed: " << qrzLineEdit->text() << " - " << QString::number(bandComboBox->currentIndex()) << "/" << QString::number(modeComboBox->currentIndex()) << endl;
    //int newId = -1;

    int errorCode = 0;
    QString aux;
    //int _x; //for clublog management

    //bool ret = false;
    QString tqrz = qrzLineEdit->text();

    slotBandComboBoxChanged();
    slotModeComboBoxChanged();

    // Just to prepare or some tasks before reading DATA from UI
    if (contestMode == "CQ-WW-SSB")
    {
        if ((SRXLineEdit->text()).toInt() < 1    ){
        return;
        }
    }
    else
    {

    }

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
                msgBox.setIcon(QMessageBox::Warning);
                aux = tr("An unexpected error ocurred when trying to add the QSO to your log. If the problem persists, please contact the developer for analysis: ");
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

                return;
            }
            else
            {
                query.finish();
                //TODO: To move the following lines to this part to properly manage the query result!!
                //ret = true;
               //qDebug() << "MainWindow::slotQRZReturnPressed: QSO Added! " << endl;
                actionsJustAfterAddingOneQSO();
                clearForNextQSO();
            }
        }
        else   // The QUERY string is NULL
        {
            return;
           //qDebug() << "MainWindow::slotQRZReturnPressed: queryString-NULL: " << queryString << endl;
        }

    modify = false;
    modifyingQSO = -1;
    OKButton->setText(tr("&Add"));
}
void MainWindow::actionsJustAfterAddingOneQSO()
{
    //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO" << endl;
    int lastId = -1;
    needToSave = true;
    if (modify)
    {
        //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO: Modifying! " << endl;
       needToSave = true;
       if(modifyingQSO>0)
       {
           awards->setAwards(modifyingQSO);

           if ((clublogActive) & (clublogRealTime))
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
       }

       // CHECK WHAT WAS THE QSOID to add the awards, if needed
       awards->setAwards(modifyingQSO);   //Update the DXCC award status
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

            if ((clublogActive) & (clublogRealTime))
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

    }

    logWindow->refresh();
    dxccStatusWidget->refresh();
    //qDebug() << "MainWindow::actionsJustAfterAddingOneQSO - END" << endl;

}

QString MainWindow::readDataFromUI()
{

       //qDebug() << "MainWindow::readDataFromUI: " << endl;

    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (!util->isValidCall(tqrz))
    {
        return "NULL";
    }

    if (contestMode == "DX")
    {
        if (modify)
        {
            return readDataFromUIDXModifying();
        }
        else
        {
            return readDataFromUIDX();
        }
    }
    else if (contestMode == "CQ-WW-SSB")
    {}
    else
    {
        if (modify)
        {
            return readDataFromUIDXModifying();
        }
        else
        {
            return readDataFromUIDX();
        }
    }

       //qDebug() << "MainWindow::readDataFromUI: END" << endl;
    return "NULL";
}

QString MainWindow::readDataFromUIDX()
{
/*
If you make any change here, please update also readDataFromUIDXModifying to keep data integrity!
*/
    //qDebug() << "MainWindow::readDataFromUIDX:" << endl;

    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (!util->isValidCall(tqrz))
    {
        return "NULL";
    }


    QString stringQuery = "NULL";
    QString aux1, aux2, stringFields, stringData;
    //QString aux, aux2;
    int tband = currentBand;
    int tmode = currentMode;

    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");
    QString ttime = (timeEdit->time()).toString("hh:mm:ss");

    QString trsttx = rstTXLineEdit->text();
    QString trstrx = rstRXLineEdit->text();

    int dxcc = world->getQRZARRLId(tqrz);
    //int dxcc2 = getDXCCFromComboBox();
    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
      //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) << endl;
      //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) << endl;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);

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
            return  "NULL";
        }
        else
        {}
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
        aux1 = QString::number(rxFreqSpinBox->value());
        stringFields = stringFields + ", freq_rx";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = qthLineEdit->text();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", qth";
        stringData = stringData + ", '" + aux1 + "'";
    }   

    aux1 = myDataTabWidget->getOperator();
    //aux1 = operatorLineEdit->text();
    if (aux1.length()>2)
    {
        //lastOperatorQRZ = aux1.toUpper();
        stringFields = stringFields + ", operator";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getStationQRZ();
    //aux1 = (stationCallSignLineEdit->text()).toUpper();
    if (aux1.length()>2)
    {
        //lastStationQRZ = aux1.toUpper();
        stringFields = stringFields + ", station_callsign";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getMyLocator();
    //aux1 = myLocatorLineEdit->text();
    if (aux1.length()>2)
    {                   

        //lastMyLocator = aux1.toUpper();
        stringFields = stringFields + ", my_gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }
    aux1 = commentTabWidget->getComment();
    //aux1 = commentLineEdit->text();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", comment";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QSLTabWidget->getQSLMsg();
    //aux1 = qslmsgTextEdit->toPlainText();
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
    //aux1 = qslViaLineEdit->text();
    if (aux1.length()>3)
    {
        stringFields = stringFields + ", qsl_via";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(myDataTabWidget->getMyPower());
    if ((aux1.toDouble())>0.0)
    {
        //lastPower = aux1.toDouble();
        stringFields = stringFields + ", tx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(rxPowerSpinBox->value());
    if ((aux1.toDouble())>0.0)
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
    else
    {
         //qDebug() << "MainWindow::readDataFromUIDX: IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) << endl;
    }

    // EQSL-SENT

    aux1 = satTabWidget->getSatName(); //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
      //qDebug() << "MainWindow::readDataFromUIDX: SAT1 " << aux1 << endl;
   //stringFields = stringFields + ", sat_name";
   //stringData = stringData + ", '" + aux1 + "'";
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_name";
        stringData = stringData + ", '" + aux1 + "'";
    }
   // aux1 = satTabWidget->getSatMode();

    aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
    //stringFields = stringFields + ", sat_mode";
    //stringData = stringData + ", '" + aux1 + "'";

    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_mode";
        stringData = stringData + ", '" + aux1 + "'";
    }

    keepSatPage = satTabWidget->getRepeatThis();

    aux1 = othersTabWidget->getPropModeFromComboBox();
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
        stringData = stringData + ", '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", eqsl_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", eqsl_qslrdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", lotw_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "'";
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
            stringData = stringData + ", '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", lotw_qslrdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "'";
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
        stringData = stringData + ", '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "'";
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

    stringFields += ", call, bandid, modeid, qso_date, time_on, lognumber, rst_sent, rst_rcvd";

    if (stringFields.startsWith(", ") )
    {
        stringFields.remove(0,2);
    }

    if (stringData.startsWith(", ") )
    {
        stringData.remove(0,1);
    }

    stringData.remove(0,1);
    stringData += QString(", '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8'").arg(tqrz).arg(tband).arg(tmode).arg(tdate).arg(ttime).arg(QString::number(currentLog)).arg(trsttx).arg(trstrx);

    if (stringData.startsWith(", ") )
    {
        stringData.remove(0,2);
    }

    stringQuery = "INSERT INTO log (" + stringFields + ") values (" + stringData +")" ;
    return stringQuery;
}



QString MainWindow::readDataFromUIDXModifying()
{
       //qDebug() << "MainWindow::readDataFromUIDXModifying:" << endl;

/*
UPDATE table_name
SET column1 = value1, column2 = value2...., columnN = valueN
WHERE [condition];
*/
    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (!util->isValidCall(tqrz))
    {
        return "NULL";
    }


    QString stringQuery = "NULL";
    QString aux1, aux2;
    //QString aux, aux2;
    int tband = currentBand;
    int tmode = currentMode;

    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");
    QString ttime = (timeEdit->time()).toString("hh:mm:ss");

    QString trsttx = rstTXLineEdit->text();
    QString trstrx = rstRXLineEdit->text();

    int dxcc = world->getQRZARRLId(tqrz);
    int cqz = world->getEntityCqz(dxcc);
    int ituz = world->getEntityItuz(dxcc);


    /**/

    //int dxcc2 = getDXCCFromComboBox();
    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
      //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) << endl;
      //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) << endl;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);

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
        msgBox.setText( tr("You have selected an entity:") + "\n\n"+"- "+dxccn2+"\n\n"+tr("that is different from the KLog proposed entity:") + "\n\n- "+dxccn1+"\n\n"
                        +tr("Click on the prefix of the right entity or Cancel to correct."));

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
            return  "NULL";
        }
        else
        {}
    }


    /**/


    QString updateString = "UPDATE log SET call = '" + tqrz + "', bandid = '" + QString::number(tband) + "', modeid = '" + QString::number(tmode) + "', qso_date = '" + tdate + "', time_on = '" + ttime + "', rst_sent = '" + trsttx + "', rst_rcvd = '" + trstrx + "', lognumber = '" + QString::number(currentLog) + "', ";

    aux1 = nameLineEdit->text();
    if (aux1.length()>1)
    {
        updateString = updateString + "name = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = (locatorLineEdit->text()).toUpper();
    if ( locator->isValidLocator(aux1)  )
    {
        updateString = updateString + "gridsquare = '";
        updateString = updateString + aux1 + "', ";

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
        }

    }

    if ( (rxFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(rxFreqSpinBox->value());
        updateString = updateString + "freq_rx = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = qthLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "qth = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = myDataTabWidget->getOperator();
    //aux1 = operatorLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "operator = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = myDataTabWidget->getStationQRZ();
    //aux1 = (stationCallSignLineEdit->text()).toUpper();
    if (aux1.length()>2)
    {
        updateString = updateString + "station_callsign = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = myDataTabWidget->getMyLocator();
    //aux1 = myLocatorLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "my_gridsquare = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = commentTabWidget->getComment();
    //aux1 = commentLineEdit->text();
    if (aux1.length()>0)
    {
        updateString = updateString + "comment = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QSLTabWidget->getQSLMsg();
    //aux1 = qslmsgTextEdit->toPlainText();
    if (aux1.length()>0)
    {
        updateString = updateString + "qslmsg = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(dxcc);
       //qDebug() << "MainWindow::readDataFromUIDXModifying: DXCC=" << aux1 << endl;
    if (aux1.length()>0)
    {
        updateString = updateString + "dxcc = '";
        updateString = updateString + aux1 + "', ";
           //qDebug() << "MainWindow::readDataFromUIDXModifying: Saving DXCC=" << aux1 << endl;
    }

    aux1 = QString::number(cqz);
    if (aux1.length()>0)
    {
        updateString = updateString + "cqz = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(ituz);
    if (aux1.length()>0)
    {
        updateString = updateString + "ituz = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QSLTabWidget->getQSLVia();
    //aux1 = qslViaLineEdit->text();
    if (aux1.length()>3)
    {
        updateString = updateString + "qsl_via = '";
        updateString = updateString + aux1 + "', ";
    }

    //aux1 = QString::number(myPowerSpinBox->value());
    aux1 = QString::number(myDataTabWidget->getMyPower());
    if ((aux1.toDouble())>0.0)
    {
        updateString = updateString + "tx_pwr = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(rxPowerSpinBox->value());
    if ((aux1.toDouble())>0.0)
    {
        updateString = updateString + "rx_pwr = '";
        updateString = updateString + aux1 + "', ";
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
         //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) << endl;
    }

   aux1 = satTabWidget->getSatName();   //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
      //qDebug() << "MainWindow::readDataFromUIDX: SAT2 modif " << aux1 << endl;
   //updateString = updateString + "sat_name = '";
   //updateString = updateString + aux1 + "', ";

    if (aux1.length()>0)
    {
        updateString = updateString + "sat_name = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
    if (aux1.length()>0)
    {
        updateString = updateString + "sat_mode = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = othersTabWidget->getPropModeFromComboBox();
    //aux1 = getPropModeFromComboBox();
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
        //updateString = updateString + aux1 + "', ";
    }
    else
    {
          //qDebug() << "MainWindow::readDataFromUIDX: PropMode(3):  " << aux1 << endl;
    }

    //CLUBLOG

    aux1 = eQSLTabWidget->getClubLogStatus(); //Y, N, M
    if (aux1 == "Y")
    {
        updateString = updateString + "clublog_qso_upload_status = 'Y', ";
        updateString = updateString + "clublog_qso_upload_date = '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "N")
    {
        updateString = updateString + "clublog_qso_upload_status = 'N', ";
    }
    else if (aux1 == "M")
    {
        updateString = updateString + "clublog_qso_upload_status = 'M', ";
        updateString = updateString + "clublog_qso_upload_date = '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "eqsl_qslsdate = '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "eqsl_qsl_sent = 'R', ";
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "eqsl_qsl_sent = 'Q', ";
        updateString = updateString + "eqsl_qslsdate = '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "eqsl_qsl_sent = 'I', ";
        updateString = updateString + "eqsl_qslsdate = '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "eqsl_qslrdate = '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "eqsl_qslrdate = '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "lotw_qslsdate = '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "lotw_qsl_sent = 'R', ";
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "lotw_qsl_sent = 'Q', ";
        updateString = updateString + "lotw_qslsdate = '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "lotw_qsl_sent = 'I', ";
        updateString = updateString + "lotw_qslsdate = '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "lotw_qslrdate = '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'R', ";
    }
    else if (aux1 == "V")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'V', ";
        updateString = updateString + "lotw_qslrdate = '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'I', ";
        updateString = updateString + "lotw_qslrdate = '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";

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
        //updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";
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
        updateString = updateString + "qslrdate = '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "', ";
        //updateString = updateString + "confirmed = '1', ";

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
            updateString = updateString + "qslrdate = '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "', ";
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
            updateString = updateString + "qslrdate = '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "', ";
            //updateString = updateString + "confirmed = '1', ";

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

    //stringQuery = "INSERT INTO log (" + stringFields + ") values (" + stringData +")" ;
   // updateString = "UPDATE log SET call = '" + tqrz + "', bandid = '" + QString::number(tband) + "', modeid = '" + QString::number(tmode) + "', qso_date = '" + tdate + "', time_on = '" + ttime + "', lognumber = '" + QString::number(currentLog) + "', " + updateString;

    stringQuery = updateString + " WHERE id = " + "'" + QString::number(modifyingQSO) + "'";
       //qDebug() << "MainWindow::readDataFromUIDXModifying: queryCreated: " << stringQuery << endl;
    return stringQuery;
}

/*
void MainWindow::createSearchResultsPanel()
{

    searchBoxClearButton->setToolTip(tr("Clear the searches"));
    searchBoxExportButton->setToolTip(tr("Export the search result to an ADIF file"));
    searchBoxSelectAllButton->setToolTip(tr("Select/Unselect all the QSO of the box"));
    searchBoxReSearchButton->setToolTip(tr("Search in the log"));
    searchAllRadioButton->setToolTip(tr("Search in all logs"));

     searchBoxLineEdit->setToolTip(tr("Enter the QRZ to search"));
     searchResultsTreeWidget->setToolTip(tr("Search results"));

     QStringList labels;
     if (stationCallSignShownInSearch)
     {
         labels << tr("QRZ") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("Station Callsign") << tr("Id") ;
         searchResultsTreeWidget->setColumnCount(8);
     }
     else
     {
        labels << tr("QRZ") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("Id") ;
        searchResultsTreeWidget->setColumnCount(7);
     }


     searchResultsTreeWidget->setHeaderLabels(labels);
     //QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
    (searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);


     searchResultsTreeWidget->clear();
     //searchResultsTreeWidget->collapseAll();
     searchResultsTreeWidget->setSortingEnabled(true);
     //searchResultsTreeWidget->setItemsExpandable(false);


    switch (contestMode) {

        case CQ_WW_SSB:
        break;
        case CQ_WW_CW:
        break;
        default:


        break;
    }

}


*/

void MainWindow::createScorePanel()
{


    QVBoxLayout *scoreLayout = new QVBoxLayout;
    scoreLayout->addWidget(scoreTextEdit);
    scoreTextEdit->setPlainText("Test TEXT");

    scoreWindow->setLayout(scoreLayout);

}


void MainWindow::createUICQWW()
{
      //qDebug() << "MainWindow::createUICQWW" << endl;
/*
    QSqlQuery query("SELECT name FROM band");
    while (query.next()) {
        bands << query.value(0).toString();
    }
    QSqlQuery query1("SELECT name FROM mode");
    while (query1.next()) {
        modes << query1.value(0).toString();
    }

*/
    //bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    //modes << "SSB" << "CW" << "RTTY";
    bandComboBox->addItems(bands);
     //qDebug() << "MainWindow::createUICQWW - 1-" << QString::number(modes.count()) << endl;
    modeComboBox->addItems(modes);

    qrzLineEdit->setToolTip(tr("QRZ of the QSO."));
    rstTXLineEdit->setToolTip(tr("TX RST."));
    rstRXLineEdit->setToolTip(tr("RX RST."));
    STXLineEdit->setToolTip(tr("TX Exchange."));
    SRXLineEdit->setToolTip(tr("RX Exchange"));
    bandComboBox->setToolTip(tr("Band of the QSO."));
    modeComboBox->setToolTip(tr("Mode of the QSO."));
    dateEdit->setToolTip(tr("Date of the QSO."));
    timeEdit->setToolTip(tr("Time of the QSO."));
    //statusBar->setToolTip(tr("Misc information"));
    //qsoStatusBar->setToolTip(tr("QSO information"));
    OKButton->setToolTip(tr("Add the QSO to the log."));
    //spotItButton->setToolTip(tr("Spots this QSO to the DX Cluster"));
    clearButton->setToolTip(tr("Clear the box."));

    gridGroupBox = new QGroupBox(tr("Input"));
    QGridLayout *layout = new QGridLayout;

    //slotUpdateStatusBar(tr("Ready"));
    //updateQSOStatusBar(tr("Ready"));

    rstTXLineEdit->setInputMask("#990");
    rstRXLineEdit->setInputMask("#990");
    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    rstTXLineEdit->setMaxLength(3);
    rstRXLineEdit->setMaxLength(3);

    QGroupBox *RSTrxgroupBox = new QGroupBox(tr("RSTrx"));
    RSTrxgroupBox->setFlat(true);
    QVBoxLayout *RSTrxvbox = new QVBoxLayout;
    RSTrxvbox->addWidget(rstRXLineEdit);
    RSTrxvbox->addStretch(1);
    RSTrxgroupBox->setLayout(RSTrxvbox);

    QGroupBox *RSTtxgroupBox = new QGroupBox(tr("RSTtx"));
    RSTtxgroupBox->setFlat(true);
    QVBoxLayout *RSTtxvbox = new QVBoxLayout;
    RSTtxvbox->addWidget(rstTXLineEdit);
    RSTtxvbox->addStretch(1);
    RSTtxgroupBox->setLayout(RSTtxvbox);


    //QGroupBox *qrzgroupBox = new QGroupBox(tr("QRZ"));
    qrzgroupBox = new QGroupBox(tr("QRZ"));
    qrzgroupBox->setFlat(true);
    QVBoxLayout *qrzvbox = new QVBoxLayout;
    qrzvbox->addWidget(qrzLineEdit);
    qrzvbox->addStretch(1);
    qrzgroupBox->setLayout(qrzvbox);

    QGroupBox *stxgroupBox = new QGroupBox(tr("STX"));
    stxgroupBox->setFlat(true);
    QVBoxLayout *stxvbox = new QVBoxLayout;
    stxvbox->addWidget(STXLineEdit);
    stxvbox->addStretch(1);
    stxgroupBox->setLayout(stxvbox);

    QGroupBox *srxgroupBox = new QGroupBox(tr("SRX"));
    srxgroupBox->setFlat(true);
    QVBoxLayout *srxvbox = new QVBoxLayout;
    srxvbox->addWidget(SRXLineEdit);
    srxvbox->addStretch(1);
    srxgroupBox->setLayout(srxvbox);

    QHBoxLayout *RSTLayout = new QHBoxLayout;
    RSTLayout->addWidget(RSTtxgroupBox);
    RSTLayout->addWidget(RSTrxgroupBox);
    RSTLayout->addWidget(stxgroupBox);
    RSTLayout->addWidget(srxgroupBox);

    QHBoxLayout *TimeLayout = new QHBoxLayout;
    TimeLayout->addWidget(dateEdit);
    TimeLayout->addWidget(timeEdit);

    QHBoxLayout *BandModeLayout = new QHBoxLayout;
    BandModeLayout->addWidget(bandComboBox);
    BandModeLayout->addWidget(modeComboBox);

    //QHBoxLayout *statusBarLayout = new QHBoxLayout;
    //statusBarLayout->addWidget(statusBar);
    //statusBarLayout->addWidget(qsoStatusBar);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(OKButton);
    //buttonsLayout->addWidget(spotItButton);
    buttonsLayout->addWidget(clearButton);

    QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("yyyy/MM/dd");
    timeEdit->setDisplayFormat("HH:mm:ss");

    layout->addWidget(qrzgroupBox, 1, 0);
    layout->addLayout(RSTLayout, 1, 1);
    layout->addLayout(TimeLayout, 2, 0);
    layout->addLayout(BandModeLayout, 2, 1);
    layout->addLayout(buttonsLayout,3, 1);
    //layout->addLayout(statusBarLayout, 4, 0, 2 , -1);

    gridGroupBox->setLayout(layout);
    gridGroupBox->resize(gridGroupBox->minimumSize());
    mainWidget->setLayout(layout);

 }

void MainWindow::slotOKButtonClicked(){
     //qDebug() << "MainWindow::slotOKButtonClicked: "  << endl;
    slotQRZReturnPressed();
}

void MainWindow::createActionsCommon(){
// Functional widgets connections
//TODO: Reimplement the possibility to enter a QSO with enter inthe following widgets:
    //connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

// Return pressed = QSO ENTRY
    connect(qrzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(SRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(STXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstTXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(operatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(stationCallSignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );    

    connect(myLocatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
    connect(myLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotMyLocatorTextChanged() ) );

    connect(txFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqTXChanged()) ) ;
    connect(rxFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqRXChanged()) ) ;


//connect(bandComboBox, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
//connect(dateEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
//connect(timeEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

//Actions to pass the focus between QRZ / SRX
    connect(qrzLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZTextChanged() ) );
    connect(SRXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSRXTextChanged() ) );
    connect(STXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSTXTextChanged() ) );
    connect(rstTXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstTXTextChanged() ) );
    connect(rstRXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstRXTextChanged() ) );

    //connect(qslViaLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQSLViaTextChanged() ) );

    connect(bandComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandComboBoxChanged() ) ) ;
    connect(modeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotModeComboBoxChanged() ) ) ;


//Buttons Actions
    connect(OKButton, SIGNAL(clicked()), this, SLOT(slotOKButtonClicked() ) );
    //connect(spotItButton, SIGNAL(clicked()), this, SLOT(slotSpotItButtonClicked() ) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked() ) );


// SEARCH BOX VIEW

    //connect(searchBoxLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSearchBoxTextChanged() ) );

    //connect(searchAllRadioButton, SIGNAL(toggled(bool)), this, SLOT(slotSearchBoxSelectAllButtonClicked() ) );

    //connect(searchResultsTreeWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonSearch( const QPoint& ) ) );
    //connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    //connect(searchResultsTreeWidget, SIGNAL(itemSelectionChanged( ) ), this, SLOT(slotSearchBoxSelectionChanged( ) ) );

    //connect(searchBoxExportButton, SIGNAL(clicked()), this, SLOT(slotSearchExportButtonClicked() ) );
    //connect(searchBoxClearButton, SIGNAL(clicked()), this, SLOT(slotSearchClearButtonClicked() ) );
    //connect(searchBoxSelectAllButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxSelectAllButtonClicked() ) );
    //connect(searchBoxReSearchButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxReSearchButtonClicked() ) );

    connect(operatingYearsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotOperatingYearComboBoxChanged() ) ) ;
    connect(recalculateAwardsButton, SIGNAL(clicked()), this, SLOT(slotRecalculateAwardsButtonClicked() ) );
    //connect(dxccStatusWidget, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );

    // LOGVIEW
    connect(logWindow, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(logWindow, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );    
    connect(logWindow, SIGNAL(updateSearchText()), this, SLOT(slotSearchBoxTextChanged() ) ); //When a QSO is deleted

    //CLUSTER
    //void clusterSpotToLog(const QStringList _qs);
    //SIGNAL dxspotclicked(const QStringList _qs)
    connect(dxClusterWidget, SIGNAL(dxspotclicked(QStringList)), this, SLOT(slotAnalyzeDxClusterSignal(QStringList) ) );

    // CLUBLOG
    connect (elogClublog, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (elogClublog, SIGNAL (actionReturnDownload(int, int)), this, SLOT (slotElogClubLogProcessAnswer(int, int)));
    connect (elogClublog, SIGNAL (disableClubLogAction(bool)), this, SLOT (slotElogClubLogDisable(bool)));
	// SATELLITE TAB
    connect (satTabWidget, SIGNAL (satBandTXChanged(QString)), this, SLOT (slotSatBandTXComboBoxChanged(QString)));
    // QSL TAB
    connect(QSLTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );
    // SEARCH TAB
    connect(searchWidget, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(searchWidget, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(searchWidget, SIGNAL(logRefresh() ), this, SLOT(slotLogRefresh() ) );
    connect(searchWidget, SIGNAL(toStatusBar(QString) ), this, SLOT(slotUpdateStatusBar(QString) ) );
    connect(searchWidget, SIGNAL(requestBeingShown() ), this, SLOT(slotShowSearchWidget() ) );
    connect(searchWidget, SIGNAL(actionQSODelete( int ) ), this, SLOT(slotQSODelete(int) ) );

    //DXCCWIDGET TAB
    connect(dxccStatusWidget, SIGNAL(showQso(int)), this, SLOT(slotShowQSOFromDXCCWidget(int) ) );
    connect(dxccStatusWidget, SIGNAL(showQsos(QList<int>)), this, SLOT(slotShowQSOsFromDXCCWidget(QList<int>) ) );

    // UDPLogServer - WSJT-x

   connect(UDPLogServer, SIGNAL(status_update(int, QString, double, QString, QString, QString, QString, QString, QString)), this, SLOT(slotWSJXstatusFromUDPServer(int, QString, double, QString, QString, QString, QString, QString, QString) ) );
   connect(UDPLogServer, SIGNAL( logged_qso(int,QString,double,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(slotWSJTXloggedQSO(int,QString,double,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString) ) );


}



void MainWindow::slotSearchBoxTextChanged()
{
    searchWidget->slotSearchBoxTextChanged();
}

void MainWindow::slotQSODelete(const int _id)
{

    elogClublog->deleteQSO(dataProxy->getClubLogRealTimeFromId(_id));
    //awards->recalculateAwards();
}

void MainWindow::slotShowSearchWidget()
{
    //dxUpRightTab->addTab(searchWidget, tr("Search"));
    //qDebug() << "MainWindow::slotShowSearchWidget: " << QString::number(dxUpRightTab->indexOf(searchWidget)) << endl;

    dxUpRightTab->setCurrentIndex(dxUpRightTab->indexOf(searchWidget));
}

void MainWindow::slotLogRefresh()
{
    logWindow->refresh();
}

void MainWindow::slotElogClubLogDisable(const bool _b)
{
      //qDebug() << "MainWindow::slotElogClubLogDisable: " << endl;
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

}

void MainWindow::slotElogClubLogShowMessage(const QString _s)
{
       //qDebug() << "MainWindow::slotElogClubLogShowMessage: " << _s << endl;
    slotUpdateStatusBar(_s);
}

void MainWindow::slotElogClubLogProcessAnswer(const int _i, const int _qID)
{
      //qDebug() << "MainWindow::slotElogClubLogProcessAnswer: " <<QString::number(_i) << endl;

    clublogAnswer = _i;

    if (clublogAnswer == 0) // NO ERROR
    {

        dataProxy->setClubLogSent(_qID, "Y", (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd"));
    }
    else
    {
        dataProxy->setClubLogSent(_qID, "M", (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd"));
    }


}

void MainWindow::slotRecalculateAwardsButtonClicked()
{
       //qDebug() << "MainWindow::recalculateAwardsButtonClicked: " << endl;

    awards->recalculateAwards();
    showAwards();

}

void MainWindow::slotExitFromSlotDialog(const int exitID)
{
       //qDebug() << "MainWindow::slotExitFromSlotDialog: " << QString::number(exitID) << endl;

    if (exitID == 2)
    {
        needToEnd = true;
        close();
    }

}

void MainWindow::createActionsCQWW(){
// Functional widgets connections

}

void MainWindow::createActionsDX(){
// Functional widgets connections
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    //connect(iotaContinentComboBox, SIGNAL(activated ( int)), this, SLOT(slotIOTAComboBoxChanged() ) )  ;

    //QSL Actions
    //connect(qslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLSentComboBoxChanged() ) )  ;
    //connect(qslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLRecvComboBoxChanged() ) ) ;

//TODO REMOVE EQSL
    //connect(eqslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLSentComboBoxChanged() ) )  ;
    //connect(eqslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLRecvComboBoxChanged() ) ) ;
    //connect(lotwSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotLotwSentComboBoxChanged() ) )  ;
    //connect(lotwRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotLotwRecvComboBoxChanged() ) ) ;

    connect(satTabWidget, SIGNAL(setPropModeSat(QString)), this, SLOT(slotSetPropMode(QString)) ) ;


}

bool MainWindow::checkContest(){
   //qDebug() << "MainWindow::checkContest: " << contestMode << endl;
    //contestNames << "No-Contest" <<"CQ-WW-DX-SSB" << "CQ-WW-DX-CW" << "CQ-WPX-SSB" << "CQ-WPX-CW";
    QStringList qs;
    qs.clear();
    QString qsoStatus, aux;
    int currentEntity = world->getQRZARRLId(currentQrz);
    int tband = 1 + bandComboBox->currentIndex();

    //int tmode = 1 + modeComboBox->currentIndex();

    if (contestMode == "DX")
    {}
    else if (contestMode == "CQ-WW-SSB")
    {
           //qDebug() << "MainWindow::checkContest: CQ-WW-SSB:" << QString::number(currentEntity) << "/" << SRXLineEdit->text() << "/" << QString::number(tband) << endl;
        if ( currentEntity < 1) {
            return false;
        }

            //Multiplier: qs << DX-Entity << DXCQz << DX-band;


            qs << QString::number(currentEntity) << SRXLineEdit->text() << QString::number(tband);


            if (contest->isMultiplier(qs)){
                qrzgroupBox->setTitle(tr("NEW MULT"));
                //qsoStatus = tr("MULT");
                aux = " + (M + ";
                qsoMultiplier = 1;
            }else{
                //qsoStatus = tr("NO MULT");
                aux.clear();

                qsoMultiplier = 0;
            }

            // Points: //_qs << DX-Entity << DX-Continent
            qs.clear();

            qs << QString::number(currentEntity) << QString::number(world->getContinentNumber(currentEntity));

            qsoPoints = contest->getQSOPoints(qs);
            if (aux == " + (M + ")
            {
                qsoStatus = "Total: " + QString::number(contest->getTotalScore()) + aux + QString::number(qsoPoints) + " points)"; // qsoStatus + " / " + QString::number(qsoPoints) + tr(" points");
            }
            else
            {
                qsoStatus = "Total: " + QString::number(contest->getTotalScore()) + " ( " + QString::number(qsoPoints) + " points)"; // qsoStatus + " / " + QString::number(qsoPoints) + tr(" points");
            }


               //qDebug() << "MainWindow::checkContest Points: " << QString::number(contest->getQSOPoints(qs)) << endl;
               //qDebug() << "MainWindow::checkContest Continent: " << world->getQRZContinentNumber(qrzLineEdit->text()) << endl;

    }
    else
    {}

    slotUpdateStatusBar(qsoStatus);
    //statusBar()->showMessage(qsoStatus);
    //updateQSOStatusBar(qsoStatus);

    return false;
}
/*
void MainWindow::slotQSLViaTextChanged()
{
       //qDebug() << "MainWindow::slotQSLViaTextChanged: " << qslViaLineEdit->text() << " / Length: " << QString::number((qslViaLineEdit->text()).size()) << endl;
    qslViaLineEdit->setText((qslViaLineEdit->text()).toUpper());
}
*/

bool MainWindow::validCharactersInCall(const QString _qrz)
{
    for (int i = 0; i<_qrz.size();i++)
    {
        if (!( ((_qrz.at(i)).isLetterOrNumber()) || (_qrz.at(i)=='\\')  || (_qrz.at(i)=='/')  ))
        {
            return false;
        }

    }

    return true;
}

void MainWindow::slotQRZTextChanged()
{
     //qDebug()<< "MainWindow::slotQRZTextChanged: " << qrzLineEdit->text() << " / Length: " << QString::number((qrzLineEdit->text()).size()) << "###### START ######" << endl;

    if ((qrzLineEdit->text()).length()<1)
    {
        return;
    }
    int cursorP = qrzLineEdit->cursorPosition();
    //qDebug()<< "MainWindow::slotQRZTextChanged: cursor position: " << QString::number(cursorP) << endl;
    qrzLineEdit->setText((qrzLineEdit->text()).toUpper());
    if (cleaning)
    {
           //qDebug()<< "MainWindow::slotQRZTextChanged: Cleaning" << endl;
        return;
    }

    if (qrzAutoChanging)
    {
           //qDebug()<< "MainWindow::slotQRZTextChanged: qrzAutoChanging" << endl;
        qrzAutoChanging = false;
        return;
    }

    qrzAutoChanging = true;

    //qDebug()<< "MainWindow::slotQRZTextChanged: cursor position.1: " << QString::number(cursorP) << endl;

    if ( (qrzLineEdit->text()).endsWith(' ') )
    {/*Remove the space and moves the focus to SRX to write the RX exchange*/
        previousQrz = (qrzLineEdit->text()).simplified();
        qrzLineEdit->setText(previousQrz);
        SRXLineEdit->setFocus();
           //qDebug()<< "MainWindow::slotQRZTextChanged: Space detected" << endl;
    }

    //qDebug()<< "MainWindow::slotQRZTextChanged: Simplifiying & Capitalizing" << endl;
    qrzLineEdit->setText(((qrzLineEdit->text())).simplified());
    qrzLineEdit->setText((qrzLineEdit->text()).remove(" "));
    //qrzLineEdit->setText((qrzLineEdit->text()).toUpper());

    //qDebug()<< "MainWindow::slotQRZTextChanged: checking for invalid chars" << endl;
    if (!validCharactersInCall(qrzLineEdit->text()))
    {
        infoLabel1->setText(tr("Invalid characters used in the QRZ"));
        InValidCharsInPrevCall = true;
        return;
    }

    //qDebug()<< "MainWindow::slotQRZTextChanged: checking for length" << endl;
    if (((qrzLineEdit->text()).length() < 1))
    { // If QRZ box is blank, Information labels should be cleared.
        infoLabel1->clear();
        infoLabel2->clear();
        //slotClearButtonClicked();
        clearForNextQSO();
        return;
    }

    //qDebug()<< "MainWindow::slotQRZTextChanged: checking for modify or length<1" << endl;
    if (((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    //if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
           //qDebug() << "MainWindow::slotQRZTextChanged: MODIFY or Lenght < 1" << endl;
        qrzSmallModDontCalculate=false;
        return;
    }

    //qDebug()<< "MainWindow::slotQRZTextChanged: running..." << endl;
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    //int i;
    int dx_CQz = -1;
    int dxE_CQz = -1;
    int dx_ITUz = -1;
    int dxE_ITUz = -1;

    currentQrz = qrzLineEdit->text();

    if ((currentQrz).count('\\')){ // Replaces \ by / to ease operation.
        currentQrz.replace(QChar('\\'), QChar('/'));
        qrzLineEdit->setText(currentQrz);
    }

    currentQrz = qrzLineEdit->text();
    //qDebug()<< "MainWindow::slotQRZTextChanged: cursor position.3: " << QString::number(cursorP) << endl;
    if (cursorP>currentQrz.length())
    {// A Space that has been removed without updating the cursor
           //qDebug()<< "MainWindow::slotQRZTextChanged: cursorP > currentQRZ.length" << endl;
    }
    else
    {
        if (cursorP==0)
        {

        }
        else if ((currentQrz.at(cursorP-1)).isSpace())
        {
            //qDebug()<< "MainWindow::slotQRZTextChanged: cursor position.5: " << QString::number(cursorP) << endl;
            previousQrz = currentQrz.remove(cursorP-1, 1);
            //qDebug()<< "MainWindow::slotQRZTextChanged: cursor position.6: " << QString::number(cursorP) << endl;
            cursorP--;
            //qDebug()<< "MainWindow::slotQRZTextChanged: cursor position.7: " << QString::number(cursorP) << endl;
            qrzLineEdit->setText(previousQrz);
        }
    }

    currentQrz = qrzLineEdit->text();
    currentEntity = world->getQRZARRLId(currentQrz);
    //selectCorrectComboBoxEntity(currentEntity);
      //qDebug() << "MainWindow::slotQRZTextChanged: " << QString::number(currentEntity) << endl;
    othersTabWidget->setEntity(currentEntity);


      //qDebug() << "MainWindow::slotQRZTextChanged: DXCC/ISOname: " << QString::number(currentEntity) << "/" << dataProxy->getISOName(currentEntity) << endl;

      //qDebug() << "MainWindow::slotQRZTextChanged: Entity: " << QString::number(currentEntity) << endl;

    dxE_CQz = world->getEntityCqz(currentEntity);
    dx_CQz = world->getQRZCqz(currentQrz);
    dx_ITUz = world->getQRZItuz(currentQrz);
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

    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
    }
    else
    {
        dxLocator = world->getLocator(currentEntity);
    }

   // NOW ONLY SPECIFIC ACTIONS DEPENDING ON THE RUNNING MODE

    if (contestMode == "DX")
    {
           //qDebug() << "MainWindow::slotQRZTextChanged: Default:" << endl;
           //qDebug() << "MainWindow::slotQRZTextChanged: - current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) << endl;
        if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) || (dx_CQz != dxE_CQz) || (dx_ITUz != dxE_ITUz))
        {
           //qDebug() << "MainWindow::slotQRZTextChanged: currentEntity=" << QString::number(currentEntity) << "/previousEntity=" << QString::number(previousEntity)  << endl;
            previousEntity = currentEntity;
            InValidCharsInPrevCall = false;
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
            infoWidget->showDistanceAndBearing(myLocator, dxLocator);

            showStatusOfDXCC(_qs);
            showDXMarathonNeeded(currentEntity, dx_CQz, dateEdit->date().year(), currentLog);
            othersTabWidget->setIOTAContinentFromEntity(currentEntity);

        }
        else if ((dx_CQz == dxE_CQz) || (dx_ITUz = dxE_ITUz))
        {
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
        }
        else
        {
                   //qDebug() << "MainWindow::slotQRZTextChanged: Default: else" << endl;
        }
    }
    else if (contestMode == "CQ-WW-SSB")
    {
           //qDebug() << "MainWindow::slotQRZTextChanged: CQ-WW-SSB:" << endl;

        STXLineEdit->setText(QString::number(my_CQz));  // My Own CQZ
        if (dx_CQz > 0)
        {
            //if(QString::number(world->getQRZCqz(qrzLineEdit->text())) > 0 ){
            SRXLineEdit->setText(QString::number(dx_CQz));
        }else{
               //qDebug() << "MainWindow::checkContest  CQZ < 0"<< endl;
        }
        if (currentEntity>0){
            slotUpdateStatusBar(world->getEntityName(currentEntity) + "  -  CQ: " + QString::number(dx_CQz) + "  -  ITU: " + QString::number(dx_ITUz));
        }
        else
        {
            slotUpdateStatusBar(tr("Ready..."));
        }
        checkIfWorkedB4(currentQrz); // Has the QSO id if worked before
        checkContest();

    }
    else
    {
           //qDebug() << "MainWindow::slotQRZTextChanged: Default:" << endl;
           //qDebug() << "MainWindow::slotQRZTextChanged: - current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) << endl;
        if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) || (dx_CQz != dxE_CQz) || (dx_ITUz != dxE_ITUz))
        {
           //qDebug() << "MainWindow::slotQRZTextChanged: currentEntity=" << QString::number(currentEntity) << "/previousEntity=" << QString::number(previousEntity)  << endl;
            previousEntity = currentEntity;
            InValidCharsInPrevCall = false;
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
            infoWidget->showDistanceAndBearing(myLocator, dxLocator);

            showStatusOfDXCC(_qs);
            showDXMarathonNeeded(currentEntity, dx_CQz, dateEdit->date().year(), currentLog);
            othersTabWidget->setIOTAContinentFromEntity(currentEntity);

        }
        else if ((dx_CQz == dxE_CQz) || (dx_ITUz = dxE_ITUz))
        {
            infoLabel2->setText(world->getEntityName(currentEntity));
            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
        }
        else
        {
                   //qDebug() << "MainWindow::slotQRZTextChanged: Default: else" << endl;
        }
    }


    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
    //qDebug() << "MainWindow::slotQRZTextChanged: cursorP at the end : " << QString::number(cursorP) << endl;
    qrzLineEdit->setCursorPosition(cursorP);
    completeWithPreviousQSO(currentQrz);
    qrzAutoChanging = false;
    //qDebug() << "MainWindow::slotQRZTextChanged: END" << endl;
}


void MainWindow::slotQRZSpacePressed()
{
        //qDebug() << "MainWindow::slotQRZSpacePressed: "  << endl;
}

void MainWindow::slotSRXTextChanged()
{
       //qDebug() << "MainWindow::slotSRXTextChanged: " << SRXLineEdit->text()  << endl;

    srx = SRXLineEdit->text();
    //int i = srx.size();

    if ( srx.endsWith(' ')  )
    {
        /*Remove the space and moves the focus to QRZ*/

        srx = srx.simplified();
        SRXLineEdit->setText(srx);
        qrzLineEdit->setFocus(Qt::OtherFocusReason);

    }

}

void MainWindow::slotSTXTextChanged()
{
       //qDebug() << "MainWindow::slotSTXTextChanged: " << STXLineEdit->text()  << endl;

    stx = STXLineEdit->text();
    //int i = stx.size();

    if ( stx.endsWith(' ')  )
    {
        /*Remove the space and moves the focus to QRZ*/

        stx = stx.simplified();
        STXLineEdit->setText(stx);
        SRXLineEdit->setFocus(Qt::OtherFocusReason);


    }
}

void MainWindow::slotrstTXTextChanged()
{

}

void MainWindow::slotrstRXTextChanged()
{
}

void MainWindow::slotSpotItButtonClicked()
{
    if (!dxClusterWidget->isConnected())
    {
        return;
    }

}

void MainWindow::clearForNextQSO()
{
    cleaning = true;
    modify = false;
    OKButton->setText(tr("&Add"));
    modifyingQSO = -1;
    qrzLineEdit->clear();
    qrzLineEdit->setFocus(Qt::OtherFocusReason);

    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    qthLineEdit->clear();

    //qDebug() << "MainWindow::clearForNextQSO: - currentBand: " << QString::number(currentBand) << endl;
    //qDebug() << "MainWindow::clearForNextQSO: - defaultBand: " << QString::number(defaultBand) << endl;
    //qDebug() << "MainWindow::clearForNextQSO: - mode: " << QString::number(currentMode) << endl;
    //qDebug() << "MainWindow::clearForNextQSO: - defaultMode: " << QString::number(defaultMode) << endl;

    //if (currentBand < 0)
    //{
    //    currentBand = defaultBand;
    //}


    //qDebug() << "MainWindow::MainWindow: - Changing from: " << bandComboBox->currentText() << endl;

    //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(currentBand), Qt::MatchCaseSensitive));

    //qDebug() << "MainWindow::MainWindow: - Changing to: " << bandComboBox->currentText() << endl;
    //qDebug() << "MainWindow::MainWindow: 12 - currentMode: " << QString::number(currentMode) << endl;
    //if (currentMode < 0)
    // {
    //    currentMode = defaultMode;
    //}

    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getSubModeFromId(currentMode), Qt::MatchCaseSensitive));

    qsoPoints = 0;
    qsoMultiplier = 0;
    clublogAnswer = -1;
    clublogPrevQSO.clear();

    if (contestMode == "DX")
    {
        clearUIDX();
    }
    else if (contestMode == "CQ-WW-SSB")
    {

    }
    else
    {
        clearUIDX();
    }

    statusBar()->clearMessage();
    cleaning = false;
       //qDebug() << "MainWindow::clearForNextQSO: " << modeComboBox->currentText() << endl;
       //qDebug() << "MainWindow::clearForNextQSO - currentMode = " << QString::number(currentMode) << endl;
       //qDebug() << "MainWindow::clearForNextQSO - END" << endl;
}

void MainWindow::slotClearButtonClicked()
{
    //qDebug() << "MainWindow::slotClearButtonClicked - START" << endl;
    //qDebug() << "MainWindow::slotClearButtonClicked: " << modeComboBox->currentText() << endl;
    cleaning = true;
    modify = false;
    OKButton->setText(tr("&Add"));
    modifyingQSO = -1;
    qrzLineEdit->clear();
    qrzLineEdit->setFocus(Qt::OtherFocusReason);

    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    qthLineEdit->clear();

    //qDebug() << "MainWindow::slotClearButtonClicked: - currentBand: " << QString::number(currentBand) << endl;
    //qDebug() << "MainWindow::slotClearButtonClicked: - currentBand: " << bandComboBox->currentText() << endl;
    //qDebug() << "MainWindow::slotClearButtonClicked: - defaultBand: " << QString::number(defaultBand) << endl;
       //qDebug() << "MainWindow::slotClearButtonClicked: - mode: " << QString::number(currentMode) << endl;
       //qDebug() << "MainWindow::slotClearButtonClicked: - defaultMode: " << QString::number(defaultMode) << endl;
    if (currentBand < 0)
    {
        currentBand = defaultBand;
    }
    //qDebug() << "MainWindow::MainWindow: - Changing from: " << bandComboBox->currentText() << endl;

    bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(currentBand), Qt::MatchCaseSensitive));

    //qDebug() << "MainWindow::MainWindow: - Changing to: " << bandComboBox->currentText() << endl;
       //qDebug() << "MainWindow::MainWindow: 12 - currentMode: " << QString::number(currentMode) << endl;
    if (currentMode < 0)
    {
        currentMode = defaultMode;
    }

    modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getSubModeFromId(currentMode), Qt::MatchCaseSensitive));
    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(currentMode)));

    qsoPoints = 0;
    qsoMultiplier = 0;
    clublogAnswer = -1;
    clublogPrevQSO.clear();

    if (contestMode == "DX")
    {
        clearUIDX(true);

    }
    else if (contestMode == "CQ-WW-SSB")
    {
        SRXLineEdit->clear();
        STXLineEdit->setText( QString::number( world->getQRZCqz(stationQRZ) ) );

        qrzgroupBox->setTitle(tr("QRZ"));
    }
    else if (contestMode == "CQ-WW-CW")
    {
        SRXLineEdit->clear();
        STXLineEdit->setText( QString::number( world->getQRZCqz(stationQRZ) ) );
        qrzgroupBox->setTitle(tr("QRZ"));
    }
    else
    {
        clearUIDX(true);
    }

    statusBar()->clearMessage();
    cleaning = false;
       //qDebug() << "MainWindow::slotClearButtonClicked: " << modeComboBox->currentText() << endl;
       //qDebug() << "MainWindow::slotClearButtonClicked - currentMode = " << QString::number(currentMode) << endl;
    //qDebug() << "MainWindow::slotClearButtonClicked - END" << endl;
}

void MainWindow::clearUIDX(bool full)
{
    SRXLineEdit->setText("59");
    STXLineEdit->setText("59");
    nameLineEdit->clear();
    locatorLineEdit->clear();
    commentTabWidget->clear();
    infoLabel1->clear();
    infoLabel2->clear();
    rxPowerSpinBox->setValue(0);
    eQSLTabWidget->clear();
    QSLTabWidget->clear();
    othersTabWidget->clear();

    satTabWidget->clear();
    myDataTabWidget->clear(keepMyData);
    if (full)
    {
        txFreqSpinBox->setValue(0);
        rxFreqSpinBox->setValue(0);
    }

}

/*
void MainWindow::clearBandLabels()
{
    bandLabel1->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel2->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel3->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel4->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel5->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel6->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel7->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel8->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel9->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel10->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel11->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel12->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
}
*/

void MainWindow::slotRefreshDXCCWidget()
{
    dxccStatusWidget->slotRefreshButtonClicked();
}


void MainWindow::slotUpdateTime()
{
   //    //qDebug() << "MainWindow::slotUpdateTime: " << (dateTime->currentDateTime()).toString("yyyy-MM-dd - hh:mm:ss") << endl;
    dateTime->currentDateTime();

    if ( (!modify) && (realTime)  )
    {
        if (UTCTime)
        {
            timeEdit->setTime((dateTime->currentDateTime().toUTC()).time());
            dateEdit->setDate((dateTime->currentDateTime().toUTC()).date());
        }
        else
        {
            timeEdit->setTime((dateTime->currentDateTime()).time());
            dateEdit->setDate((dateTime->currentDateTime()).date());
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
       //qDebug() << "MainWindow::closeEvent" << endl;

    if (maybeSave())
    {
        dataProxy->unMarkAllQSO();
        //slotFileClose();
        dataProxy->compressDB();
        //db->compress();
        event->accept();
    }
    else
    {
        event->ignore();
    }


}

bool MainWindow::maybeSave()
{
       //qDebug() << "MainWindow::maybeSave" << endl;

    if ((alwaysADIF) || (DBinMemory) )
    {
        if (needToSave)
        {
            QMessageBox::StandardButton ret;
            ret = QMessageBox::warning(this, tr("KLog"),
                     tr("The logfile has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            if (ret == QMessageBox::Save)
            {
                if (useDefaultLogFileName)
                {
                    return saveFile(defaultADIFLogFile);
                }
                else
                {
                    return saveFileAs();
                }
            }

         else if (ret == QMessageBox::Cancel)
             return false;
        }
    }
    return true;
}



void MainWindow::createMenusCommon()
{
       //qDebug() << "MainWindow::createMenusCommon" << endl;
    fileMenu = menuBar()->addMenu(tr("&File"));

    openAct = new QAction(tr("&New..."), this);
    fileMenu->addAction(openAct);
    //openAct->setShortcut(Qt::CTRL + Qt::Key_N);
    connect(openAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
    openAct->setShortcut(Qt::CTRL + Qt::Key_O);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    ADIFImport = new QAction(tr("&Import from ADIF..."), this);
    fileMenu->addAction(ADIFImport);
    connect(ADIFImport, SIGNAL(triggered()), this, SLOT(slotADIFImport()));
    ADIFImport->setToolTip(tr("Import an ADIF file into the current log."));

    //LoTWImport = new QAction(tr("&Import from LoTW..."), this);
    //fileMenu->addAction(LoTWImport);
    //connect(LoTWImport, SIGNAL(triggered()), this, SLOT(slotLoTWImport()));
    //LoTWImport->setToolTip(tr("Import an LoTW file into the current log"));

    fileMenu->addSeparator();

    saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
    saveAct->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

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

    ReqQSLExport = new QAction(tr("Export Requested QSL to ADIF..."), this);
    fileMenu->addAction(ReqQSLExport);
    connect(ReqQSLExport, SIGNAL(triggered()), this, SLOT(slotRQSLExport()));
    ReqQSLExport->setToolTip(tr("Export all QSOs requesting QSLs to an ADIF file (e.g. to import it into a QSL tag printing program)."));

    LoTWExport = new QAction(tr("Export ADIF for LoTW..."), this);
    fileMenu->addAction(LoTWExport);
    connect(LoTWExport, SIGNAL(triggered()), this, SLOT(slotLoTWExport()));
    LoTWExport->setToolTip(tr("Export an ADIF file to be sent to LoTW. Remember to sign it with TQSL before uploading to LoTW!"));

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
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    fillQsoAct = new QAction(tr("Fill in QSO data"), this);
    toolMenu->addAction(fillQsoAct);
    //fillQsoAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(fillQsoAct, SIGNAL(triggered()), this, SLOT(fillQSOData()));
    fillQsoAct->setToolTip(tr("Go through the log reusing previous QSOs to fill missing information in other QSOs."));

    fillDXCCAct = new QAction(tr("Fill in DXCC data"), this);
    toolMenu->addAction(fillDXCCAct);
    connect(fillDXCCAct, SIGNAL(triggered()), this, SLOT(slotFillEmptyDXCCInTheLog()));
    fillDXCCAct->setToolTip(tr("Go through the log filling QSOs without a DXCC defined."));




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
    findQSLPendingToReceiveAct->setToolTip(tr("Shows the DX-QSL that has been requested or QSLs has been sent with no answer."));

    findQSLDXRequestedAct = new QAction(tr("&Find requested pending to receive"), this);
    qslToolMenu->addAction(findQSLDXRequestedAct);
    connect(findQSLDXRequestedAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLRequested()));
    findQSLDXRequestedAct->setToolTip(tr("Shows the DX-QSL that has been requested."));

    toolMenu->addSeparator();
    lotwToolMenu = toolMenu->addMenu(tr("LoTW tools..."));

    lotwMarkSentQueuedThisLogAct = new QAction(tr("Queue all QSL to be sent of this log"), this);
    lotwToolMenu->addAction(lotwMarkSentQueuedThisLogAct);
    connect(lotwMarkSentQueuedThisLogAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllQueuedThisLog()));
    lotwMarkSentQueuedThisLogAct->setToolTip(tr("Mark all non sent QSOs in this log as queued to be uploaded."));

    lotwMarkSentQueuedAct = new QAction(tr("Queue all QSL to be sent"), this);
    lotwToolMenu ->addAction(lotwMarkSentQueuedAct);
    connect(lotwMarkSentQueuedAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllQueued()));
    lotwMarkSentQueuedAct->setToolTip(tr("Mark all non sent QSOs as queued to be uploaded."));

    lotwToolMenu->addSeparator();

    lotwMarkSentYesThisLogAct = new QAction(tr("Mark as sent all queued QSO of this log"), this);
    lotwToolMenu->addAction(lotwMarkSentYesThisLogAct);
    connect(lotwMarkSentYesThisLogAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllYesThisLog()));
    lotwMarkSentYesThisLogAct->setToolTip(tr("Mark all queued QSOs in this log as sent to LoTW."));

    lotwMarkSentYesAct = new QAction(tr("Mark all queued QSO as sent"), this);
    lotwToolMenu ->addAction(lotwMarkSentYesAct);
    connect(lotwMarkSentYesAct, SIGNAL(triggered()), this, SLOT(slotToolLoTWMarkAllYes()));
    lotwMarkSentYesAct->setToolTip(tr("Mark all queued QSOs as sent to LoTW."));


    toolMenu->addSeparator();

    downloadCTYAct = new QAction (tr("&Update cty.csv"), this);
    toolMenu->addAction(downloadCTYAct);
    //downloadCTYAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(downloadCTYAct, SIGNAL(triggered()), this, SLOT(slotUpdateCTYDAT()));
    downloadCTYAct->setToolTip(tr("For updated DX-Entity data, update cty.csv."));

    toolMenu->addSeparator();

    showStatsAct = new QAction (tr("Show stats"), this);
    toolMenu->addAction(showStatsAct);
    connect(showStatsAct, SIGNAL(triggered()), this, SLOT(slotShowStats()));
    showStatsAct->setToolTip(tr("Show the statistics of your radio activity."));

    toolMenu->addSeparator();

    setupMenu = menuBar()->addMenu(tr("&Setup"));

    setupAct = new QAction(tr("&Setup..."), this);
    setupMenu->addAction(setupAct);
    setupAct->setMenuRole(QAction::PreferencesRole);
    connect(setupAct, SIGNAL(triggered()), this, SLOT(slotSetup()));

    //TODO: To be added once the help dialog has been implemented
    helpMenu = menuBar()->addMenu(tr("&Help"));

    updateAct = new QAction(tr("Check updates..."), this);
    helpMenu->addAction(updateAct);
    updateAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(updateAct, SIGNAL(triggered()), this, SLOT(slotHelpCheckUpdatesAction()));

    aboutAct = new QAction(tr("&About..."), this);
    helpMenu->addAction(aboutAct);
    aboutAct->setMenuRole(QAction::AboutRole);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotHelpAboutAction()));
    
    aboutQtAct = new QAction(tr("About Qt..."), this);
    helpMenu->addAction(aboutQtAct);
    aboutQtAct->setMenuRole(QAction::AboutQtRole);
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(slotAboutQt()));
    
    
 }

void MainWindow::slotSearchToolNeededQSLToSend()
{
    slotShowSearchWidget();
    searchWidget->searchToolNeededQSLToSend();
}

void MainWindow::slotToolSearchRequestedQSLToSend()
{
    searchWidget->slotToolSearchRequestedQSLToSend();
}

void MainWindow::slotToolSearchNeededQSLPendingToReceive()
{
    searchWidget->slotToolSearchNeededQSLPendingToReceive();
}

void MainWindow::slotToolSearchNeededQSLRequested()
{
    searchWidget->slotToolSearchNeededQSLRequested();
}


void MainWindow::slotToolLoTWMarkAllQueuedThisLog()
{
    //qDebug() << "MainWindow::slotToolLoTWMarkAllQueuedThisLog"  << endl;
    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");

    if(dataProxy->lotwSentQueue(tdate, currentLog))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All pending QSO of this log has been marked as queued for LoTW!") + "\n\n" + tr("Now you can go to the File menu to export the LoTW ADIF file and upload it to LoTW."));
        msgBox.exec();

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all pending QSO of this log as queued for LoTW!") );
        msgBox.exec();
    }
}

void MainWindow::slotToolLoTWMarkAllQueued()
{
    //qDebug() << "MainWindow::slotToolLoTWMarkAllQueued"  << endl;
    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");
    if (dataProxy->lotwSentQueue(tdate, -1))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All pending QSO has been marked as queued for LoTW!") + "\n\n" +  tr("Now you can go to the File menu to export the LoTW ADIF file and upload it to LoTW."));
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all pending QSO of this log as queued for LoTW!") );
        msgBox.exec();

    }

}


QString MainWindow::selectStationCallsign()
{
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

        if (ok && !stationCallToUse.isEmpty())
        {
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
                return stationCallToUse;

             }
             else
             {
                 QMessageBox msgBox;
                 msgBox.setIcon(QMessageBox::Warning);

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
    return QString();
}

void MainWindow::slotToolLoTWMarkAllYesThisLog()
{
    //qDebug() << "MainWindow::slotToolLoTWMarkAllYesThisLog"  << endl;
    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");

    if(dataProxy->lotwSentYes(tdate, currentLog, "ALL"))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All queued QSO of this log has been marked as sent for LoTW!")  );
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all queued QSO of this log as sent for LoTW!") );
        msgBox.exec();
    }
}

void MainWindow::slotToolLoTWMarkAllYes()
{
    //qDebug() << "MainWindow::slotToolLoTWMarkAllYes"  << endl;


    QString stationCallToUse = selectStationCallsign();

    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");
    if (dataProxy->lotwSentYes(tdate, -1, stationCallToUse))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("All queued QSO has been marked as sent to LoTW!") );
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog LoTW"));
        msgBox.setText(tr("There was a problem to mark all queued QSO of this log as sent to LoTW!") );
        msgBox.exec();

    }
}

void MainWindow::slotAboutQt()
{
       //qDebug() << "MainWindow::slotAboutQt"  << endl;
   QMessageBox::aboutQt(this,tr("About..."));
   
}
/*
void MainWindow::slotHelpHelpAction()
{
       //qDebug() << "MainWindow::slotHelpHelpAction"  << endl;

    //helpHelpDialog->exec();
    //aboutDialog->exec();

}
*/

void MainWindow::slotHelpAboutAction()
{
       //qDebug() << "MainWindow::slotHelpAboutAction "  << endl;
   // QMessageBox::about(this, tr("About KLog"),
    //            tr("<b>KLog</b> "
   //                "Find the last release at http://jaime.robles.es/klog."));


    aboutDialog->exec();
    //helpAboutDialog->exec();
}

void MainWindow::slotHelpCheckUpdatesAction()
{
      //qDebug() << "MainWindow::slotHelpCheckUpdatesAction" << endl;
    callingUpdate = true;
    softUpdate->addCall(stationQRZ);
    softUpdate->needToUpdate(true);
    //callingUpdate = false;
}

void MainWindow::slotShowSoftUpdateResults(const bool _b)
{

      //qDebug() << "MainWindow::slotShowSoftUpdateResults: " << endl;
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
}


void MainWindow::createMenusCQWW()
{
       //qDebug() << "MainWindow::createMenusCQWW" << endl;
/*
    logWinAct = new QAction(tr("&Log Window"), this);
    logWinAct->setCheckable(true);
    logWinAct->setShortcut(Qt::CTRL + Qt::Key_L);
    viewMenu->addAction(logWinAct);
    connect(logWinAct, SIGNAL(triggered()), this, SLOT(slotLogWinShow()));

    scoreWinAct = new QAction(tr("&Points Window"), this);
    scoreWinAct->setCheckable(true);
    scoreWinAct->setShortcut(Qt::CTRL + Qt::Key_P);
    viewMenu->addAction(scoreWinAct);
    connect(scoreWinAct, SIGNAL(triggered()), this, SLOT(slotScoreWinShow()));



    CabrilloExport = new QAction(tr("&Export to Cabrillo..."), this);
    toolMenu->addAction(CabrilloExport);
    connect(CabrilloExport, SIGNAL(triggered()), this, SLOT(slotCabrilloExport()));
    */

 }



void MainWindow::slotLogWinShow()
{
       //qDebug() << "MainWindow::slotLogWinShow: "  << endl;

    if (!(logWindow->isVisible()) )
    {
        logWinAct->setChecked ( true );
        logWindow->show();
    }else
    {
        logWinAct->setChecked ( false );
        logWindow->hide();
    }
}

void MainWindow::slotScoreWinShow()
{
       //qDebug() << "MainWindow::slotScoreWinShow: "  << endl;

    if (!(scoreWindow->isVisible()) )
    {
        scoreWinAct->setChecked ( true );
        scoreWindow->show();

    }else
    {
        scoreWinAct->setChecked ( false );
        scoreWindow->hide();
    }
}

void MainWindow::slotSetup(const int _page)
{
      //qDebug() << "MainWindow::slotSetup - 01"  << endl;
    itIsANewversion = false;
    if (!needToEnd)
    {
        setupDialog->setData(configFileName, softwareVersion, _page, !configured);
        setupDialog->exec();
           //qDebug() << "MainWindow::slotSetup - JUst after setupDialog->exec"  << endl;

        if (needToEnd)
        {
            return;
        }
        else
        {
            //qDebug() << "MainWindow::slotSetup - Just before readConfigData"  << endl;
            readConfigData();            
            //qDebug() << "MainWindow::slotSetup - Just after readConfigData"  << endl;
        }


           //qDebug() << "MainWindow::MainWindow: logmodel to be created-2" << endl;
        logWindow->createlogPanel(currentLog);
           //qDebug() << "MainWindow::MainWindow: logmodel has been created-2" << endl;

    }
    defineStationCallsign();
       //qDebug() << "MainWindow::MainWindow: before db->reConnect" << endl;
    dataProxy->reconnectDB();
    //db->reConnect();
       //qDebug() << "MainWindow::MainWindow: after db->reConnect" << endl;

}

void MainWindow::openFile()
{

    int lastLog = currentLog;

    slotSetup(6);

    if (lastLog == currentLog)
    { // It seems that the user didn't really want a new log
        return;
    }

    logWindow->refresh();

}

bool MainWindow::saveFile(const QString _fileName)
{
     //qDebug() << "MainWindow::saveFile: " << _fileName  << endl;


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
       return false;
    }
       //qDebug() << "MainWindow::saveFile: 4"  << endl;
    return needToSave;

}

bool MainWindow::saveFileAs()
{
      //qDebug() << "MainWindow::saveFileAs"  << endl;
    //QFileDialog dialog(this);

    QStringList filters;
    filters << "ADIF files (*.adi *.adif)"
            << "Cabrillo files (*.log)"
            << "Any files (*)";

//    dialog.setNameFilters(filters);

/*
 QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                            "/home/jana/untitled.png",
                            tr("Images (*.png *.xpm *.jpg)"));
*/

   // klogDir+"/"+defaultADIFLogFile,
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                   util->getHomeDir(),
                        tr("ADIF file") + "(*.adi *.adif);;" + tr("Cabrillo files") + "(*.log);;" + tr("Any file") + "(*.*)");

    if ( (fileName.endsWith(".adi", Qt::CaseInsensitive)) || (fileName.endsWith(".log", Qt::CaseInsensitive)) )
    {
        useDefaultLogFileName = true;
        defaultADIFLogFile = fileName;

        return saveFile(fileName);
    }
    else if (fileName.length()==0)
    {
        // The user clicked on cancel, no msg to be shown
        return false;
    }
    else
    {

        //TODO: Message "You must select a proper file format
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setText(tr("Nothing has been saved. You have to select a valid file type."));
       msgBox.exec();
       return false;
    }

    return false;

}


void MainWindow::newFile()
{
        //qDebug() << "MainWindow::newFile"  << endl;
     //TODO: Ask for a confirmation to the user
    //TODO: Clean the DB & query.exec("VACUUM");

    int lastLog = currentLog;

    slotSetup(6);

    if (lastLog == currentLog)
    { // It seems that the user didn't really want a new log
        return;
    }

    points = 0;
    multipliers = 0;
    qsoPoints = 0;
    qsoMultiplier = 0;
    logWindow->refresh();
    slotClearButtonClicked();
    searchWidget->clear();
    //searchResultsTreeWidget->clear();


/*
    if (dataProxy->clearLog())
    {



    }
    else
    {
        //TODO: An error to create a new file has ocurred. Manage it!
    }
*/

}

bool MainWindow::slotOpenKLogFolder()
{
    //qDebug() << "MainWindow::slotOpenKLogFolder: " << configFileName << endl;

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
    return true;

}


void MainWindow::slotUpdateStatusBar(const QString statusm)
{
       //qDebug() << "MainWindow::slotUpdateStatusBar: " << statusm  << endl;
    statusBar()->showMessage(statusm, 2000);
}




bool MainWindow::readCtyFile()
{
    return false;
}

void MainWindow::slotDoubleClickLog(const int _qsoID)
{
      //qDebug() << "MainWindow::slotDoubleClickLog: QSOid: " << QString::number(_qsoID) << endl;

    //int row = _qsoID.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    qsoToEdit(_qsoID);

    //TODO: To be added to the logWindow and create an action that emist the QSO id to be edited
}


void MainWindow::keyPressEvent(QKeyEvent *event){

    /*
    if (!isStarted || isPaused || curPiece.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }
    */
    switch (event->key()) {
    case Qt::Key_Return:
        // ENTER PRESSED
        //slotQRZReturnPressed();
        break;
    case Qt::Key_Enter:

        // ENTER PRESSED
       // slotQRZReturnPressed();
        break;

    default:
        //QFrame::keyPressEvent(event);
        ;

    }
}

void MainWindow::checkIfWorkedB4(const QString _qrz)
{
       //qDebug() << "MainWindow::checkIfWorkedB4: " << _qrz << endl;

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


}


void MainWindow::readConfigData()
{
    //qDebug() << "MainWindow::readConfigData - 01" << endl;

    if (needToEnd)
    {
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
        slotSetup();

        return;
    }

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

    infoWidget->setImperialSystem(imperialSystem);

    infoLabel2->setText(world->getEntityName(currentEntity));
    infoWidget->showEntityInfo(currentEntity);

    configured = true;
    awards->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setDXClusterSpotConfig(dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY );
    dxClusterWidget->setMyQRZ(stationQRZ);

    checkIfNewBandOrMode();
    initialContestModeConfiguration();
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
        elogClublog->setCredentials(clublogUser, clublogEmail, clublogPass, false);
    }
    else
    {

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
    searchWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    infoWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());

    QString aux = tr("UDP Server error") + "\n" + tr("The UDP server failed to") + " ";
    QString errorMSG;

    if (UDPServerStart)
    {
        //qDebug() << "MainWindow::readConfigData: 104: UDPServerStart TRUE" << endl;
        if (!UDPLogServer->isStarted())
        {
            //qDebug() << "MainWindow::readConfigData: 104: Server off" << endl;
            if (!UDPLogServer->start())
            {
                errorMSG =  tr("start");
                showErrorDialog->setText(aux + errorMSG + ".");
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
                showErrorDialog->setText(aux + errorMSG + ".");
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


   //qDebug() << "MainWindow::readConfigData - END" << endl;

}

bool MainWindow::processConfigLine(const QString _line){
     //qDebug() << "MainWindow::processConfigLine: " << _line << endl;
    int _logWithMoreQSOs = 0; // At the end, if the this variable is >0 the Selectedlog will have to be changed in the file.
    QString line = _line.simplified();
    //line.simplified();
    QString aux;

    QStringList values = line.split("=", QString::SkipEmptyParts);


    if (line.startsWith('#')){
           //qDebug() << "MainWindow::processConfigLine: notes Line!" << endl;
        return true;
    }
    if (!( (line.contains('=')) && (line.contains(';')))){
           //qDebug() << "MainWindow::processConfigLine: Wrong Line!" << endl;
        return false;
    }
    QString field = (values.at(0)).toUpper();
    QString value = values.at(1);

    int endValue = value.indexOf(';');
    if (endValue>-1){

        value = value.left(value.length() - (value.length() - endValue));
    }


    if (field == "CALLSIGN"){
        mainQRZ = value;
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
        realTime = util->trueOrFalse(value);
    }
    else if (field=="INMEMORY")
    {
       //qDebug() << "MainWindow::processConfigLine: INMEMORY: " << value.toUpper() << endl;
         DBinMemory = util->trueOrFalse(value);

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
        if (value.toDouble()>0.0)
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
    else if (field=="UTCTIME")
    {
           //qDebug() << "MainWindow::processConfigLine: UTCTIME: " << value.toUpper() <<endl;
        UTCTime = util->trueOrFalse(value);
    }
    else if (field=="KEEPMYDATA")
    {
        keepMyData  = util->trueOrFalse(value);
    }
    else if (field=="COMPLETEWITHPREVIOUS")
    {
        completeWithPrevious  = util->trueOrFalse(value);
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
            myLocator = value.toUpper();
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
    else if (field=="UDPSERVER")
    {
        //qDebug() << "MainWindow::processConfigLine: UDPSERVER: " << value.toUpper()  << endl;
        if (value.toUpper() == "TRUE")
        {
            UDPServerStart = true;            
        }
        else
        {
            UDPServerStart = false;
        }
    }
    else if (field=="UDPSERVERPORT")
    {
        UDPLogServer->setPort(value.toInt());
    }   
    else if (field=="INFOTIMEOUT")
    {
        int a = value.toInt();
        if ((a>0) && (a<=30000))
        {
            infoTimeout = a;
        }


    }
    else if (field=="LOGFROMWSJTX")
    {
        if (value.toUpper() == "TRUE")
        {
            UDPLogServer->setLogging(true);
        }
        else
        {
            UDPLogServer->setLogging(false);
        }
    }
    else if (field=="LOGAUTOFROMWSJTX")
    {
        if (value.toUpper() == "TRUE")
        {
            wsjtxAutoLog = true;
            //UDPLogServer->setLogging(true);
        }
        else
        {
            wsjtxAutoLog = false;
            //UDPLogServer->setLogging(false);
        }
    }
    else if (field=="REALTIMEFROMWSJTX")
    {
        //qDebug() << "MainWindow::processConfigLine: REALTIMEFROMWSJTX: " << value << endl;
        if (value.toUpper() == "TRUE")
        {
            UDPLogServer->setRealTimeUpdate(true);
        }
        else
        {
            UDPLogServer->setRealTimeUpdate(false);
        }
    }
    else if(field=="SELECTEDLOG")
    {
        currentLog = value.toInt();
         //qDebug() << "MainWindow::processConfigLine: currentLog - SelectedLog: " << QString::number(currentLog) << endl;

        if ( ((dataProxy->doesThisLogExist(currentLog))  && (dataProxy->getHowManyQSOInLog(currentLog) > 0)) )
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
            QString aux = tr("It seems that there are no QSO in the database.") + "\n\n" + tr("If you are sure that the database contains QSOs and KLog is not able to find them, please contact the developers (see About KLog) for help.");
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
                    QString aux = tr("It seems that there are no QSO in the database.") + "\n\n" + tr("If you are sure that the database contains QSOs and KLog is not able to find them, please contact the developers (see About KLog) for help.");
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
    else if(field=="CLUBLOGCALL")
    {
        clublogUser = value;
    }
    else if(field=="CLUBLOGPASS")
    {
        clublogPass = value;

    }
    else if(field=="CLUBLOGEMAIL")
    {
        clublogEmail = value;
    }
    else if(field=="VERSION")
    {
        if (softwareVersion!=value)
        {
            itIsANewversion = true;
        }
    }
    else
    {
           //qDebug() << "MainWindow::processConfigLine: NONE: " << endl;
    }

    // Lines are: Option = value;

    return true;
}


void MainWindow::checkIfNewBandOrMode()
{//Checks the log to see if there is a QSO with a band/mode
//that is not currently selected as active
    //qDebug() << "MainWindow::checkIfNewBandOrMode - bands: " << QString::number(bands.length()) << endl;

    setupDialog->checkIfNewBandOrMode(); // Update the Setup dialog with new bands or modes

    QStringList bandsInLog = dataProxy->getBandsInLog(currentLog);
    QStringList modesInLog = dataProxy->getModesInLog(currentLog);

    QStringList qsTemp;
    qsTemp.clear();

    bands << bandsInLog;
    qsTemp << dataProxy->sortBandNamesBottonUp(bands);
    bands.clear();
    bands << qsTemp;
    
    modes << modesInLog;
    modes.removeDuplicates();

    //qDebug() << "MainWindow::checkIfNewBandOrMode - bands -" << QString::number(bands.length()) << endl;
    bandComboBox->clear();
    bandComboBox->addItems(bands);
    satTabWidget->addBands(bands);

      //qDebug() << "MainWindow::checkIfNewBandOrMode - modes -" << QString::number(modes.length()) << endl;
    modeComboBox->clear();     
    modeComboBox->addItems(modes);

      //qDebug() << "MainWindow::checkIfNewBandOrMode - CurrentBand/CurrentBandShown: " << QString::number(currentBand) << "/" << QString::number(currentBandShown) << endl;
    dxccStatusWidget->setBands(bands);

    selectDefaultBand();
    selectDefaultMode();


    //qDebug() << "MainWindow::checkIfNewBandOrMode END" << endl;
}

void MainWindow::selectDefaultBand()
{
    //qDebug() << "MainWindow::selectDefaultBand" << endl;
    QString aux;
    aux = QString();
    defaultBand = dataProxy->getMostUsedBand(currentLog);
    if (defaultBand<1)
    {
        defaultBand = dataProxy->getIdFromBandName(bandComboBox->itemText(1));
    }
        aux = dataProxy->getNameFromBandId(defaultBand);
        bandComboBox->setCurrentIndex(bandComboBox->findText(aux));

    //qDebug() << "MainWindow::selectDefaultBand_END" << endl;
}

void MainWindow::selectDefaultMode()
{
    //qDebug() << "MainWindow::selectDefaultMode" << endl;

    int aux = -1;

    defaultMode = dataProxy->getMostUsedMode(currentLog);
  //qDebug() << "MainWindow::selectDefaultMode: " << QString::number(defaultMode) << endl;

    if (defaultMode < 1)
    {

        defaultMode = dataProxy->getSubModeIdFromSubMode((modeComboBox->itemText(0)));
        //qDebug() << "MainWindow::selectDefaultMode2: " << QString::number(defaultMode) << endl;
        //qDebug() << "MainWindow::selectDefaultMode2S: " << modeComboBox->itemText(1) << endl;

    }
        aux = dataProxy->getModeFromId(defaultMode);
        modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromModeId(aux)));

        //qDebug() << "MainWindow::selectDefaultMode3: " << QString::number(defaultMode) << endl;
        //qDebug() << "MainWindow::selectDefaultMode3S: " << modeComboBox->itemText(0) << endl;

    //qDebug() << "MainWindow::selectDefaultMode-END" << endl;
}

void MainWindow::slotDefineNewBands (const QStringList _bands)
{
       //qDebug() << "MainWindow::defineNewBands: "  << endl;
    QStringList qsTemp;
    qsTemp.clear();


    //qsTemp << _bands;
    //qsTemp.removeDuplicates();

    qsTemp << dataProxy->sortBandNamesBottonUp(_bands);
    bands.clear();
    bands << qsTemp;

    bandComboBox->clear();
    bandComboBox->addItems(bands);
    satTabWidget->addBands(bands);
}


void MainWindow::readActiveBands (const QStringList actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
    //qDebug() << "MainWindow::readActiveBands: " << actives << endl;

    for (int i=0;i<actives.length();i++)
    {
        //qDebug() << "MainWindow::readActiveBands: actives: " << actives.at(i) << endl;
    }
    bool atLeastOne = false;
    QString aux;
    bands.clear();
    QStringList __bands;
    __bands.clear();
    __bands << actives;
    __bands << dataProxy->getBandsInLog(currentLog);
    __bands.removeDuplicates();

    bands << __bands;


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
    //qDebug() << "MainWindow::readActiveBands - END" << endl;
}

void MainWindow::readActiveModes (const QStringList actives)
{
    //qDebug() << "MainWindow::readActiveModes: " << actives << endl;

    //bool atLeastOne = false;
    QString aux;
    aux.clear();

    QStringList __modes;
    __modes.clear();
    __modes << actives;
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

    //qDebug() << "MainWindow::readActiveModes - END" << endl;
}

void MainWindow::createData()
{
       //qDebug() << "MainWindow::createData " << endl;

}


void MainWindow::createUIDX()
{
       //qDebug() << "MainWindow::createUIDX" << endl;

    //bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    //modes << "SSB" << "CW" << "RTTY";
    bandComboBox->addItems(bands);
      //qDebug() << "MainWindow::createUIDX - 1-" << QString::number(modes.count()) << endl;
    modeComboBox->addItems(modes);
    txFreqSpinBox->setToolTip(tr("TX Frequency in MHz."));
    rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz."));
    //myPowerSpinBox->setToolTip(tr("Power used for the QSO in watts"));
    rxPowerSpinBox->setToolTip(tr("Power used by the DX."));
    operatorLineEdit->setToolTip(tr("Logging operator's callsign."));
    stationCallSignLineEdit->setToolTip(tr("Callsign used over the air."));
    myLocatorLineEdit->setToolTip(tr("My QTH locator."));
    nameLineEdit->setToolTip(tr("Name of the DX."));
    qthLineEdit->setToolTip(tr("QTH of the DX."));
    locatorLineEdit->setToolTip(tr("Locator of the DX."));
    //freqQLCDNumber->setToolTip(tr("Frequency of the QSO"));
    qrzLineEdit->setToolTip(tr("QRZ of the QSO."));
    rstTXLineEdit->setToolTip(tr("TX RST."));
    rstRXLineEdit->setToolTip(tr("RX RST."));
    STXLineEdit->setToolTip(tr("TX Exchange."));
    SRXLineEdit->setToolTip(tr("RX Exchange."));
    bandComboBox->setToolTip(tr("Band of the QSO."));
    modeComboBox->setToolTip(tr("Mode of the QSO."));
    dateEdit->setToolTip(tr("Date of the QSO."));
    timeEdit->setToolTip(tr("Time of the QSO."));
    //statusBar->setToolTip(tr("Misc information"));
    //qsoStatusBar->setToolTip(tr("QSO information"));
    OKButton->setToolTip(tr("Add the QSO to the log."));
    //spotItButton->setToolTip(tr("Spots this QSO to the DX Cluster - This function is still not implemented"));
    clearButton->setToolTip(tr("Clears the QSO entry."));

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

    infoLabel1->setToolTip(tr("Status of the DX entity."));
    infoLabel2->setToolTip(tr("Name of the DX entity."));

    dxUpLeftInputFrame = new QFrame;

    //dxUpRightOutputFrame = new QFrame;
    //dxUpRightOutputFrame->setFrameShadow(QFrame::Raised);
    //dxUpRightOutputFrame->setFrameStyle(QFrame::StyledPanel);

    dxUpLeftTab = new QTabWidget;
    dxUpRightTab = new QTabWidget;
    dxBottonTab = new QTabWidget;

    dxUpLeftTab->setTabPosition(QTabWidget::North);
    dxUpRightTab->setTabPosition(QTabWidget::South);

    QWidget *qsoInputTabWidget = new QWidget;
    //QFormLayout *qsoInputTabWidgetLayout = new QFormLayout;
    QLabel *nameLabel = new QLabel(qsoInputTabWidget);
    nameLabel->setText(tr("Name"));
    nameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *qthLabel = new QLabel(qsoInputTabWidget);
    qthLabel->setText(tr("QTH"));
    qthLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *locLabel = new QLabel(qsoInputTabWidget);
    locLabel->setText(tr("Locator"));
    locLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rxPowerSpinBoxLabelN = new QLabel(tr("Power(rx)"));
    rxPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    QLabel *rstTxLabelN = new QLabel(tr("RST(tx)"));
    rstTxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstRxLabelN = new QLabel(tr("RST(rx)"));
    rstRxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    rstTXLineEdit->setInputMask("#990");
    rstRXLineEdit->setInputMask("#990");
    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    rstTXLineEdit->setMaxLength(3);
    rstRXLineEdit->setMaxLength(3);
    //rstTXLineEdit->setFixedWidth(30);
    //rstRXLineEdit->setFixedWidth(30);

    QGridLayout *RSTLayout = new QGridLayout;
    RSTLayout->addWidget(rstTxLabelN, 0, 0);
    RSTLayout->addWidget(rstTXLineEdit, 1, 0);
    RSTLayout->addWidget(rstRxLabelN, 0, 1);
    RSTLayout->addWidget(rstRXLineEdit, 1, 1);

    //QLabel *spacerLabel = new QLabel(tr("SPACERQTH"));

    QHBoxLayout *qthHLayout = new QHBoxLayout;
    qthHLayout->addWidget(qthLabel);
    //qthHLayout->addSpacerItem(new QSpacerItem(50,1));
    //qthHLayout->addWidget(spacerLabel);

    QVBoxLayout *qthLayout = new QVBoxLayout;
    qthLayout->addLayout(qthHLayout);
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

    //QLabel *spacerLocLabel = new QLabel(tr("SPACERLoc"));
    QHBoxLayout *locLayout = new QHBoxLayout;
    //locLayout->addSpacerItem(new QSpacerItem(50,1));
    //locLayout->addWidget(spacerLocLabel);
    locLayout->addLayout(locVLayout);

    QVBoxLayout *freqLocLayout = new QVBoxLayout;
    freqLocLayout->addLayout(freqLayout);
    freqLocLayout->addLayout(locLayout);

    QHBoxLayout *rstfreqLayout = new QHBoxLayout;
    rstfreqLayout->addLayout(rstQTHLayout);
    rstfreqLayout->addLayout(freqLocLayout);

    QVBoxLayout *rxPowerLayout = new QVBoxLayout;
    rxPowerLayout->addWidget(rxPowerSpinBoxLabelN);
    rxPowerLayout->addWidget(rxPowerSpinBox);



    //QLabel *spacerNameLabel = new QLabel(tr("SPACERName"));
    QHBoxLayout *nameHLayout = new QHBoxLayout;
    nameHLayout->addWidget(nameLabel);
    //nameHLayout->addSpacerItem(new QSpacerItem(50,1));
    //nameHLayout->addWidget(spacerNameLabel );

    QVBoxLayout *nameLayout = new QVBoxLayout;
    nameLayout->addLayout(nameHLayout);
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

    QHBoxLayout *TimeLayout = new QHBoxLayout;
    TimeLayout->addWidget(dateEdit);
    TimeLayout->addWidget(timeEdit);

    QHBoxLayout *BandModeLayout = new QHBoxLayout;
    BandModeLayout->addWidget(bandComboBox);
    BandModeLayout->addWidget(modeComboBox);

    QHBoxLayout *QrzBandModeLayout = new QHBoxLayout;
    QrzBandModeLayout->addWidget(qrzLineEdit);
    QrzBandModeLayout->addLayout(BandModeLayout);

    qrzgroupBox = new QGroupBox(tr("QRZ"));
    qrzgroupBox->setFlat(true);
    QVBoxLayout *qrzvbox = new QVBoxLayout;
    qrzvbox->addLayout(QrzBandModeLayout);
    qrzvbox->addStretch(1);
    qrzgroupBox->setLayout(qrzvbox);


    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(OKButton);
    buttonsLayout->addWidget(clearButton);

    QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("yyyy/MM/dd");
    timeEdit->setDisplayFormat("HH:mm:ss");

    QGridLayout *dxUpLeftInputFrameLayout = new QGridLayout;
    dxUpLeftInputFrameLayout->addWidget(qrzgroupBox, 0, 0, 1, 0);
    dxUpLeftInputFrameLayout->addLayout(TimeLayout, 1, 0);
    dxUpLeftInputFrameLayout->addLayout(buttonsLayout,1, 1);

    dxUpLeftInputFrame->setLayout(dxUpLeftInputFrameLayout);

    QSplitter *upLeftSplitter = new QSplitter (this);
    upLeftSplitter->addWidget(dxUpLeftInputFrame);
    upLeftSplitter->addWidget(dxUpLeftTab);
    upLeftSplitter->setOrientation(Qt::Vertical);


    QVBoxLayout *dxUpRightFixLayout = new QVBoxLayout;
    dxUpRightFixLayout->addWidget(infoLabel1);
    dxUpRightFixLayout->addWidget(infoLabel2);

    infoLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    infoLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *dxMarathonTopQSOsLabelN = new QLabel(tr("QSOs"));
    QLabel *dxMarathonTopDXCCLabelN = new QLabel(tr("DXCC"));
    QLabel *dxMarathonTopCQLabelN = new QLabel(tr("CQ"));
    QLabel *dxMarathonTopScoreLabelN = new QLabel(tr("Score"));
    QLabel *dxMarathonLabelN = new QLabel(tr("DX-Marathon"));

    dxMarathonTopQSOsLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    dxMarathonTopDXCCLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    dxMarathonTopCQLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    dxMarathonTopScoreLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    dxMarathonLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    dxUpRightTab->addTab(infoWidget, tr("Info"));

    QWidget *awardsTabWidget = new QWidget;

    QLabel *awardLabelN = new QLabel(tr("Award"));    
    awardLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);    

    QLabel *confirmedLabelN = new QLabel(tr("Confirmed"));
    confirmedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *workedLabelN = new QLabel(tr("Worked"));    
    workedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *dxccLabelN = new QLabel(tr("DXCC"));
    dxccLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *wazLabelN = new QLabel(tr("WAZ"));    
    wazLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *localLabelN = new QLabel(tr("Local"));    
    localLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *qsoNLabelN = new QLabel(tr("QSOs"));
    qsoNLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QGridLayout *dxMarathonDLayout = new QGridLayout;

    dxMarathonDLayout->addWidget(dxMarathonTopQSOsLabelN, 0, 0);
    dxMarathonDLayout->addWidget(dxMarathonTopDXCCLabelN, 0, 1);
    dxMarathonDLayout->addWidget(dxMarathonTopCQLabelN, 0, 2);
    dxMarathonDLayout->addWidget(dxMarathonTopScoreLabelN, 0, 3);
    dxMarathonDLayout->addWidget(dxMarathonQSOLCDNumber, 1, 0);
    dxMarathonDLayout->addWidget(dxMarathonDXCCQLCDNumber, 1, 1);
    dxMarathonDLayout->addWidget(dxMarathonCQQLCDNumber, 1, 2);
    dxMarathonDLayout->addWidget(dxMarathonPointsQLCDNumber, 1, 3);

    QVBoxLayout *dxMarathonTLayout = new QVBoxLayout;
    dxMarathonTLayout->addWidget(dxMarathonLabelN);
    dxMarathonTLayout->addWidget(operatingYearsComboBox);

    QGridLayout *dxUpRightAwardsTabLayout = new QGridLayout;
    dxUpRightAwardsTabLayout->addWidget(awardLabelN, 0, 0);
    dxUpRightAwardsTabLayout->addWidget(workedLabelN, 0, 1);
    dxUpRightAwardsTabLayout->addWidget(confirmedLabelN, 0, 2);
    dxUpRightAwardsTabLayout->addWidget(dxccLabelN, 1, 0);
    dxUpRightAwardsTabLayout->addWidget(dxccWorkedQLCDNumber, 1, 1);
    dxUpRightAwardsTabLayout->addWidget(dxccConfirmedQLCDNumber, 1, 2);
    dxUpRightAwardsTabLayout->addWidget(wazLabelN, 2, 0);
    dxUpRightAwardsTabLayout->addWidget(wazWorkedQLCDNumber, 2, 1);
    dxUpRightAwardsTabLayout->addWidget(wazConfirmedQLCDNumber, 2, 2);
    dxUpRightAwardsTabLayout->addWidget(localLabelN, 3, 0);
    dxUpRightAwardsTabLayout->addWidget(localWorkedQLCDNumber, 3, 1);
    dxUpRightAwardsTabLayout->addWidget(localConfirmedQLCDNumber, 3, 2);
    dxUpRightAwardsTabLayout->addWidget(qsoNLabelN, 4, 0);
    dxUpRightAwardsTabLayout->addWidget(qsoWorkedQLCDNumber, 4, 1);
    dxUpRightAwardsTabLayout->addWidget(qsoConfirmedQLCDNumber, 4, 2);

    dxUpRightAwardsTabLayout->addLayout(dxMarathonTLayout, 5, 0);
    dxUpRightAwardsTabLayout->addLayout(dxMarathonDLayout, 5, 1, 1, -1);
    dxUpRightAwardsTabLayout->addWidget(recalculateAwardsButton, 6, 1);

    awardsTabWidget->setLayout(dxUpRightAwardsTabLayout);

    dxUpRightTab->addTab(awardsTabWidget, tr("Awards"));
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

    qsoNLabelN->setFrameShadow(QFrame::Raised);
    qsoNLabelN->setFrameStyle(QFrame::StyledPanel);
    wazLabelN->setFrameShadow(QFrame::Raised);
    wazLabelN->setFrameStyle(QFrame::StyledPanel);
    localLabelN->setFrameShadow(QFrame::Raised);
    localLabelN->setFrameStyle(QFrame::StyledPanel);
    dxccLabelN->setFrameShadow(QFrame::Raised);
    dxccLabelN->setFrameStyle(QFrame::StyledPanel);
    workedLabelN->setFrameShadow(QFrame::Raised);
    workedLabelN->setFrameStyle(QFrame::StyledPanel);
    confirmedLabelN->setFrameShadow(QFrame::Raised);
    confirmedLabelN->setFrameStyle(QFrame::StyledPanel);
    awardLabelN->setFrameShadow(QFrame::Raised);
    awardLabelN->setFrameStyle(QFrame::StyledPanel);

    dxMarathonTopQSOsLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonTopDXCCLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonTopCQLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonTopScoreLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonLabelN->setFrameShadow(QFrame::Raised);

    dxMarathonTopQSOsLabelN->setFrameStyle(QFrame::StyledPanel);
    dxMarathonTopDXCCLabelN->setFrameStyle(QFrame::StyledPanel);
    dxMarathonTopCQLabelN->setFrameStyle(QFrame::StyledPanel);
    dxMarathonTopScoreLabelN->setFrameStyle(QFrame::StyledPanel);
    dxMarathonLabelN->setFrameStyle(QFrame::StyledPanel);

    infoLabel1->setFrameShadow(QFrame::Raised);
    infoLabel1->setFrameStyle(QFrame::StyledPanel);
    infoLabel2->setFrameShadow(QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel);


#else
       //qDebug() << "MainWindow::createUIDX - NO WINDOWS DETECTED!"  << endl;

    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    localLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    qsoNLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    wazLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxccLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    workedLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    confirmedLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    awardLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonTopQSOsLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonTopDXCCLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonTopCQLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonTopScoreLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    infoLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


#endif


 }

void MainWindow::slotADIFExport(){
       //qDebug() << "MainWindow::slotADIFExport " << endl;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               "ADIF (*.adi *.adif)");

    filemanager->adifLogExport(fileName, currentLog);

}

void MainWindow::slotLoTWExport(){
       //qDebug() << "MainWindow::slotLoTWExport " << endl;
    QString aux;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               "ADIF (*.adi *.adif)");

    int exportedQSO = filemanager->adifLoTWLogExport(fileName, currentLog) ;
    //qDebug() << "MainWindow::slotLoTWExport - exported: " << QString::number(exportedQSO) << endl;
    if (exportedQSO > 0)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        aux = tr("LoTW logfile has been properly exported!") + "\n\n" + tr("Remember to:") + "\n\n-" + tr("Before uploading: sign the LoTW log; and") + "\n-" + tr("After uploading: mark as sent all the queued QSO (LoTW Tools).");


        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok );

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
    else if (exportedQSO == 0)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        aux = tr("There was no QSO to be exported.") + "\n\n" + tr("If you think that some QSO should have been exported, please look for them and ensure that the eQSL LoTW QSL sent box is marked as:") + "\n\n " + tr("Q - Queued") + "." ;
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok );

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
    else
    {
        QMessageBox msgBox;

        msgBox.setIcon(QMessageBox::Warning);
        aux = tr("There was an error while exporting the LoTW. The log has not been exported!");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok );

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


void MainWindow::slotADIFExportAll(){
       //qDebug() << "MainWindow::slotADIFExportAll " << endl;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               "ADIF (*.adi *.adif)");

    filemanager->adifLogExport(fileName, 0);

}

void MainWindow::slotRQSLExport()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               "ADIF (*.adi *.adif)");


    filemanager->adifReqQSLExport(fileName);

}


void MainWindow::slotCabrilloExport()
{
       //qDebug() << "MainWindow::slotCabrilloExport " << endl;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Cabrillo File"),
                               util->getHomeDir(),
                               tr("Cabrillo (*.log)"));


    contest->saveFileToSend(fileName);

}
void MainWindow::slotLoTWImport(){
       //qDebug() << "MainWindow::slotLoTWImport " << endl;

     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      util->getHomeDir(),
                                                      "ADIF (*.adi *.adif)");
     if (fileName.isNull())
     {
     }
     else
     {
           //qDebug() << "MainWindow::slotLoTWImport -1" << endl;
        filemanager->adifLoTWReadLog(fileName);
           //qDebug() << "MainWindow::slotLoTWImport -2" << endl;

        logWindow->refresh();
           //qDebug() << "MainWindow::slotLoTWImport -3" << endl;
        //checkIfNewBandOrMode();
           //qDebug() << "MainWindow::slotLoTWImport -4" << endl;
      }
       //qDebug() << "MainWindow::slotLoTWImport-END" << endl;
 }


void MainWindow::slotADIFImport(){
      //qDebug() << "MainWindow::slotADIFImport " << endl;

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
            operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            //qDebug() << "MainWindow::slotADIFImport-DX-1" << endl;
            slotShowAwards();
            //qDebug() << "MainWindow::slotADIFImport-DX-1-end" << endl;
        }
        else if (contestMode == "CQ-WW-SSB")
        {}
        else
        {
            operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            slotShowAwards();
        }

        //qDebug() << "MainWindow::slotADIFImport-7" << endl;
    }
    //qDebug() << "MainWindow::slotADIFImport-END" << endl;
}

void  MainWindow::initialContestModeConfiguration()
{
    //QString aux = QString();

    //qDebug() << "MainWindow::initialContestModeConfiguration: - 0" << endl;

     if (!configured){
            //qDebug() << "MainWindow::initialContestModeConfiguration: - 01" << endl;
        slotSetup();
           //qDebug() << "MainWindow::initialContestModeConfiguration: - 02" << endl;
        return;
    }
        //qDebug() << "MainWindow::initialContestModeConfiguration: - 03" << endl;
    QSqlQuery query;
    QStringList contestQS;

    //qDebug() << "MainWindow::initialContestModeConfiguration: - 04" << endl;

    if (contestMode == "DX")
    {
        selectDefaultBand();
        selectDefaultMode();
        //defaultMode = dataProxy->getMostUsedMode(currentLog);
        //defaultBand = dataProxy->getMostUsedBand(currentLog);
        //aux = dataProxy->getNameFromBandId(defaultBand);
        //bandComboBox->setCurrentIndex(bandComboBox->findText(aux));

        //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));

        //qDebug() << "MainWindow::initialContestModeConfiguration-1: " << QString::number(defaultBand) << endl;
        //qDebug() << "MainWindow::initialContestModeConfiguration-1-index: " << bandComboBox->currentText() << endl;
    }
    else if (contestMode == "CQ-WW-SSB")
    {
           //qDebug() << "MainWindow::initialContestModeConfiguration: - 05" << endl;
            defaultMode = dataProxy->getIdFromModeName("SSB");
            SRXLineEdit->setInputMask("09");
            STXLineEdit->setInputMask("09");

            contestQS << QString::number(world->getQRZARRLId(stationQRZ)) << QString::number(world->getQRZCqz(stationQRZ)) << world->getQRZContinentNumber(stationQRZ) <<  world->getQRZContinentNumber("K");
            contest = new ContestCQWWDXSSB(contestQS);
    }
    else if (contestMode == "CQ-WW-CW")
    {
        SRXLineEdit->setInputMask("09");
        STXLineEdit->setInputMask("09");
        contestQS << QString::number(world->getQRZARRLId(stationQRZ)) << QString::number(world->getQRZCqz(stationQRZ)) << world->getQRZContinentNumber(stationQRZ) <<  world->getQRZContinentNumber("K");
    }
    else
    {
        selectDefaultBand();
        selectDefaultMode();

        //defaultMode = dataProxy->getMostUsedMode(currentLog);
        //defaultBand = dataProxy->getMostUsedBand(currentLog);
        //qDebug() << "MainWindow::initialContestModeConfiguration-2: " <<dataProxy->getNameFromBandId(defaultBand) << endl;

        //bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
        //qDebug() << "MainWindow::initialContestModeConfiguration-2: " << QString::number(defaultBand) << endl;
        //qDebug() << "MainWindow::initialContestModeConfiguration-2-index: " << bandComboBox->currentText() << endl;
    }
    //qDebug() << "MainWindow::initialContestModeConfiguration END: " << bandComboBox->currentText() << endl;
}


void MainWindow::qsoToEdit (const int _qso)
{
     //qDebug() << "MainWindow::qsoToEdit: " << QString::number(_qso) << endl;


    int nameCol;
    QString aux1;
    double testValueDouble; // Variable just to test if the value is in the appropriate range

    modify = true;
    modifyingQSO = _qso;
    OKButton->setText(tr("&Modify"));

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

    nameCol = rec.indexOf("call");
    aux1 = (query.value(nameCol)).toString();
    qrzLineEdit->setText(aux1);
    currentQrz = aux1;
    currentEntity = world->getQRZARRLId(currentQrz);

      //qDebug() << "MainWindow::qsoToEdit - currentEntity " << QString::number(currentEntity) << endl;

    nameCol = rec.indexOf("qso_date");
    aux1 = (query.value(nameCol)).toString();
    dateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));

    nameCol = rec.indexOf("time_on");
    aux1 = (query.value(nameCol)).toString();
    timeEdit->setTime(QTime::fromString(aux1, "hh:mm:ss"));

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
        //qDebug() << "MainWindow::qsoToEdit: - Changing from: " << bandComboBox->currentText() << endl;

        bandComboBox->setCurrentIndex(bandComboBox->findText(aux1, Qt::MatchCaseSensitive));
        //qDebug() << "MainWindow::qsoToEdit: - Changing to: " << bandComboBox->currentText() << endl;


    }
    else
    {
        //qDebug() << "MainWindow::qsoToEdit - bandid-NO "  << endl;
        bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
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
        if (modeComboBox->findText(aux1, Qt::MatchCaseSensitive)>=0)
        {
               //qDebug() << "MainWindow::qsoToEdit: Mode in the Combobox: " << aux1 << " - Result: " << QString::number(modeComboBox->findText(aux1)) << endl;
            modeComboBox->setCurrentIndex(modeComboBox->findText(aux1, Qt::MatchCaseSensitive));
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
        modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode), Qt::MatchCaseSensitive));
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
        locatorLineEdit->setText(aux1);
        satTabWidget->setLocator(aux1);


        nameCol = rec.indexOf("operator");
        aux1 = (query.value(nameCol)).toString();
        myDataTabWidget->setOperator(aux1);

        nameCol = rec.indexOf("station_callsign");
        aux1 = (query.value(nameCol)).toString();
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
            txFreqSpinBox->setValue(0);
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
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            QSLTabWidget->setQSLSenDate(QDate::fromString(aux1, "yyyy/MM/dd"));
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
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            QSLTabWidget->setQSLRecDate(QDate::fromString(aux1, "yyyy/MM/dd"));
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
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            eQSLTabWidget->setClubLogDate((QDate::fromString(aux1, "yyyy/MM/dd")));
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
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            eQSLTabWidget->setEQSLSenDate(QDate::fromString(aux1, "yyyy/MM/dd"));
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
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid() )
            {
                eQSLTabWidget->setEQSLRecDate(QDate::fromString(aux1, "yyyy/MM/dd"));
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
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eQSLTabWidget->setLOTWSenDate(QDate::fromString(aux1, "yyyy/MM/dd"));
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
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eQSLTabWidget->setLOTWRecDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }

                   //qDebug() << "MainWindow::qsoToEdit: - just before IOTA"  << endl;

                nameCol = rec.indexOf("iota");
                aux1 = (query.value(nameCol)).toString();

                aux1 = awards->checkIfValidIOTA(aux1);
                othersTabWidget->setIOTA(aux1);

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

                infoLabel2->setText(world->getEntityName(currentEntity));
                infoWidget->showEntityInfo(currentEntity);
                //selectCorrectComboBoxEntity(currentEntity);
                  //qDebug() << "MainWindow::qsoToEdit: " << QString::number(currentEntity) << endl;
                othersTabWidget->setEntity(currentEntity);
                   //qDebug() << "MainWindow::qsoToEdit: - in default - 101"  << endl;

                QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
                _qs.clear();
                //TODO: The band sometimes fails here. Check

                _qs << QString::number(currentEntity) << QString::number(dataProxy->getIdFromBandName(bandComboBox->currentText())) << QString::number(dataProxy->getIdFromBandName(modeComboBox->currentText()))  << QString::number(currentLog);


                   //qDebug() << "MainWindow::qsoToEdit: - in default - 104"  << endl;
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


}

void MainWindow::slotLocatorTextChanged()
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
       //qDebug() << "MainWindow::slotLocatorTextChanged: " << locatorLineEdit->text() << endl;
    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
        infoWidget->showDistanceAndBearing(myLocator, dxLocator);
        satTabWidget->setLocator(dxLocator);
        //showInfoFromLocators(myLocator, dxLocator);
    }
    else
    {
        return;
    }
}

void MainWindow::slotMyLocatorTextChanged()
{
       //qDebug() << "MainWindow::slotMyLocatorTextChanged: " << myLocatorLineEdit->text() << endl;

    if ( locator->isValidLocator((myLocatorLineEdit->text()).toUpper()) )
    {
        myLocator = (myLocatorLineEdit->text()).toUpper();
           //qDebug() << "MainWindow::slotMyLocatorTextChanged: My LOCATOR CHANGED TO: " << myLocator << endl;
        slotLocatorTextChanged();
    }
    else
    {
        return;
    }
}

void MainWindow::showStatusOfDXCC(const QStringList _qs)
{
       //qDebug() << "MainWindow::showStatusOfDXC: Entity: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
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
        infoLabel1->setText(tr("--"));
        return;
    }
    // Set the status bar with the appropriate message
    int status = awards->getDXStatus (_qs);
    QString message = QString();

       //qDebug() << "MainWindow::showStatusOfDXC: " << QString::number(status) << endl;

    message = awards->getDXStatusString(status);
    infoLabel1->setText(message);
    //infoWidget->showInfo((_qs.at(0)).toInt(), (_qs.at(1)).toInt(), (_qs.at(2)).toInt(), (_qs.at(3)).toInt() );
    infoWidget->showInfo((_qs.at(0)).toInt());
}


void MainWindow::showDXMarathonNeeded(const int _dxcc, const int _cqz, const int _year, const int _log)
{
       //qDebug() << "MainWindow::showDXMarathonNeeded" << endl;
    if ((_dxcc<=0) || (_cqz<=0))
    {
        return;
    }
    if ( awards->isDXMarathonNeed(_dxcc, _cqz, _year, _log))
    {        
        infoLabel1->setText(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
    }
}
void MainWindow::slotShowAwards()
{ //To be called from the logWindow & searchWidget
    //qDebug() << "MainWindow::slotShowAwards"  << endl;
    awards->recalculateAwards();
    //qDebug() << "MainWindow::slotShowAwards-1"  << endl;
    logWindow->refresh();
    //qDebug() << "MainWindow::slotShowAwards-2"  << endl;
    showAwards();
    //qDebug() << "MainWindow::slotShowAwards-3"  << endl;
    dxccStatusWidget->refresh();
    //qDebug() << "MainWindow::slotShowAwards-END"  << endl;
}

void MainWindow::showAwards()
{ // Updates and show all the award status tab.
   //qDebug() << "MainWindow::showAwards" << endl;
/*
  WAZ
  Local
*/
    int _num = 0;
    QSqlQuery query;
    QString aux;

/*
    aux = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(currentLog);
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
    */
    _num = dataProxy->getHowManyQSOInLog(currentLog);
    qsoWorkedQLCDNumber->display(_num);

    _num = dataProxy->getHowManyConfirmedQSLInLog(currentLog);

    qsoConfirmedQLCDNumber->display(_num);
    _num = 0;

    dxccWorkedQLCDNumber->display(awards->getDXCCWorked(currentLog));
    _num = 0;

    dxccConfirmedQLCDNumber->display(awards->getDXCCConfirmed(currentLog));
    _num = 0;

    wazWorkedQLCDNumber->display(awards->getWAZWorked(currentLog));
    _num = 0;

    wazConfirmedQLCDNumber->display(awards->getWAZConfirmed(currentLog));

    showDXMarathon(selectedYear);
    //qDebug() << "MainWindow::showAwards - END" << endl;

}

void MainWindow::showDXMarathon(const int _year)
{
       //qDebug() << "MainWindow::MainWindow::showDXMarathon: Year: " << QString::number(_year) << endl;
    int i = 0;

    i = awards->getDXMarathonQSO(_year, currentLog);
       //qDebug() << "MainWindow::MainWindow::showDXMarathon: QSO: " << QString::number(i) << endl;
    dxMarathonQSOLCDNumber->display(i);

    i = awards->getDXMarathonDXCC(_year, currentLog);
       //qDebug() << "MainWindow::MainWindow::showDXMarathon: DXCC: " << QString::number(i) << endl;
    dxMarathonDXCCQLCDNumber->display(i);

    i = awards->getDXMarathonCQ(_year, currentLog);
    dxMarathonCQQLCDNumber->display(i);
       //qDebug() << "MainWindow::MainWindow::showDXMarathon: CQ: " << QString::number(i) << endl;

    i = awards->getDXMarathonScore(_year, currentLog);
    dxMarathonPointsQLCDNumber->display(i);
       //qDebug() << "MainWindow::MainWindow::showDXMarathon: Score: " << QString::number(i) << endl;
}

void MainWindow::fillQSOData()
{ // Updates all QSO with the dxcc, CQZ, ... if empty.
      //qDebug() << "MainWindow::fillQSOData" << endl;

    //QString stringQuery = QString("SELECT call, bandid, modeid, qso_date, time_on, lognumber, confirmed, id, cqz, ituz, dxcc FROM log WHERE lognumber='%1'").arg(currentLog);
    QString stringQuery = QString("SELECT call, bandid, modeid, qso_date, time_on, lognumber, id, cqz, ituz, dxcc FROM log WHERE lognumber='%1'").arg(currentLog);

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
            nameCol = rec.indexOf("time_on");
            if ( (query.value(nameCol)).isValid() )
            {
                _ttime = (query.value(nameCol)).toString();
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
            updateString = "UPDATE log SET call = '" + _call + "', bandid = '" + _bandid + "', modeid = '" + _modeid + "', qso_date = '" + _tdate + "', time_on = '" + _ttime + "', lognumber = '" + _lognumber + "'";//  + "', confirmed = '" + _confirmed + "'";

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
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number(world->getQRZARRLId(_call) );
                   //qDebug() << "MainWindow::fillQSOData: DXCC proposed: " << aux1 << endl;
                if (aux1.toInt()>0)
                {
                    updateString = updateString + ", dxcc='" + aux1 + "'";
                    toModify = true;
                       //qDebug() << "MainWindow::fillQSOData: DXCC: " << aux1 << endl;
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

            aux = tr("Filling QSOs...\n QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                   //qDebug() << "MainWindow::fillQSOData3: " << endl;
                noMoreQso = true;
            }
        } // Closes the next.isValid
    } // Closes the While
}

void MainWindow::slotFillEmptyDXCCInTheLog()
{
    dataProxy->fillEmptyDXCCInTheLog();
}

void MainWindow::slotUpdateCTYDAT()
{
      //qDebug() << "MainWindow::slotUpdateCTYDAT" << endl;
    downloadcty->download();
}

void MainWindow::slotShowStats()
{
    statsWidget->show();
}

void MainWindow::slotWorldReload()
{
      //qDebug() << "MainWindow::slotWorldReload" << endl;
    //TODO: world.recreate returns a boolean, so it is possible to manage the errors
    world->recreate(ctyDatFile);
}

void MainWindow::slotFilePrint()
{
       //qDebug() << "MainWindow::slotFilePrint" << endl;
    QPrinter printer;
    QString aux;
    QSqlQuery query, query1;
    int row = 0;
    int _numberOfQsos = 0;
    bool cancelPrinting = false;
    bool sqlOK;
    _numberOfQsos = dataProxy->getHowManyQSOInLog(currentLog);
    int step = util->getProgresStepForDialog(_numberOfQsos);


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
    cursor.insertText(tr("Date"));
    cursor = textTable->cellAt(row, 2).firstCursorPosition();
    cursor.insertText(tr("Time"));
    cursor = textTable->cellAt(row, 3).firstCursorPosition();
    cursor.insertText(tr("QRZ"));
    cursor = textTable->cellAt(row, 4).firstCursorPosition();
    cursor.insertText(tr("RSTtx"));
    cursor = textTable->cellAt(row, 5).firstCursorPosition();
    cursor.insertText(tr("RSTrx"));
    cursor = textTable->cellAt(row, 6).firstCursorPosition();
    cursor.insertText(tr("Band"));
    cursor = textTable->cellAt(row, 7).firstCursorPosition();
    cursor.insertText(tr("Mode"));
    cursor = textTable->cellAt(row, 8).firstCursorPosition();
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


        QString stringQuery = QString("SELECT id, qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, comment FROM log WHERE lognumber='%1'").arg(currentLog);
        sqlOK = query.exec(stringQuery);
        if (!sqlOK)
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
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

                nameCol = rec.indexOf("time_on");
                cursor = textTable->cellAt(row, 2).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("call");
                cursor = textTable->cellAt(row, 3).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("rst_sent");
                cursor = textTable->cellAt(row, 4).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("rst_rcvd");
                cursor = textTable->cellAt(row, 5).firstCursorPosition();
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
                        cursor = textTable->cellAt(row, 6).firstCursorPosition();
                        cursor.insertText((query1.value(0)).toString());
                    }
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
                }


                nameCol = rec.indexOf("modeid");
                aux = (query.value(nameCol)).toString();
                aux = dataProxy->getNameFromSubModeId(aux.toInt());
                if (aux.length()>1)
                {
                    cursor = textTable->cellAt(row, 7).firstCursorPosition();
                    cursor.insertText((query1.value(0)).toString());
                }

                nameCol = rec.indexOf("comment");
                aux = (query.value(nameCol)).toString();
                if ((aux.length())>0)
                {
                    cursor = textTable->cellAt(row, 8).firstCursorPosition();
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

}

//DX-CLUSTER - DXCLUSTER

void MainWindow::slotAnalyzeDxClusterSignal(QStringList ql)
{
      //qDebug() << "MainWindow::slotAnalyzeDxClusterSignal: 1: " << ql.at(0) <<"/1: " << ql.at(1) << "/2: " << ql.at(2) << endl;
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


}

double MainWindow::checkFreqRanges(double _f)
{
    if ( (_f > double(0)) && (_f <= txFreqSpinBox->maximum()))
    {
        return _f;
    }
        else
    {
        return 0;
    }
}

//void MainWindow::clusterSpotToLog(const QStringList _qs)
void MainWindow::clusterSpotToLog(const QString _call, const QString _freq)
{
       //qDebug() << "MainWindow::clusterSpotToLog: " << _call <<"/" << _freq << endl;

    QString _aux;    
    double _freqN = (_freq.toDouble()) / 1000;
    qrzLineEdit->setText(_call);
    //qrzLineEdit->setText(_qs.at(0));

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
        bandComboBox->setCurrentIndex(bandComboBox->findText(_aux, Qt::MatchCaseSensitive));
    }
    else
    {
        bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand), Qt::MatchCaseSensitive));
        //bandComboBox->setCurrentIndex(defaultBand);
    }

}
//DX-CLUSTER - DXCLUSTER

void MainWindow::updateQSLRecAndSent()
{
    //qDebug() << "MainWindow::updateQSLRecAndSent "  << endl;
    // Checks the log to fill all the qsl_rcvd and qsl_sent
    QSqlQuery query, query1;
    QString queryString, aux, idT;
    //int nameCol=0;

    //queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE lognumber='%1'").arg(currentLog);
    //queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE qsl_rcvd !='Y' AND qsl_rcvd !='N' AND qsl_rcvd !='R' AND qsl_rcvd !='I' AND qsl_rcvd !='V' AND lognumber='%1'").arg(currentLog);
    //queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE qsl_rcvd ='' OR qsl_sent ='' AND lognumber='%1'").arg(currentLog);
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

  /*


    QSqlRecord rec = query.record();
    int numberOfQsos = 0;
    int i = 0;

    numberOfQsos = dataProxy->getHowManyQSOInLog(currentLog);
    int step = util->getProgresStepForDialog(numberOfQsos);

    QProgressDialog progress(tr("Updating QSL status..."), tr("Abort updating"), 0, numberOfQsos, this);
    progress.setWindowModality(Qt::WindowModal);

    while (query.next())
    {

        if (query.isValid())
        {
            i++;
            nameCol = rec.indexOf("id");
            idT = (query.value(nameCol)).toString();
            //qDebug() << "MainWindow::updateQSLRecAndSent: " << idT  << endl;

            // {Y, N, R, I, V}
            nameCol = rec.indexOf("qsl_rcvd");
            aux = (query.value(nameCol)).toString();
            if ( (aux != "Y") && (aux != "N") && (aux != "R") && (aux != "I") && (aux != "V") )
            {//QSL_RCVD
                nameCol = rec.indexOf("qsl_sent");
                aux = (query.value(nameCol)).toString();

                //{Y, N, R, Q, I}

                if ( (aux != "Y") && (aux != "N") && (aux != "R") && (aux != "Q") && (aux != "I") )
                {//QSL_SENT
                    queryString = QString("UPDATE log SET qsl_rcvd='N', qsl_sent='N' WHERE id='%1'").arg(idT);
                    if (!query1.exec(queryString))
                    {
                        emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
                    }
                }
                else
                {
                    queryString = QString("UPDATE log SET qsl_rcvd='N' WHERE id='%1'").arg(idT);
                    if(!query1.exec(queryString))
                    {
                        emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
                    }

                }

            }
            else
            {//QSL_SENT

                nameCol = rec.indexOf("qsl_sent");
                aux = (query.value(nameCol)).toString();

                //{Y, N, R, Q, I}

                if ( (aux != "Y") && (aux != "N") && (aux != "R") && (aux != "Q") && (aux != "I") )
                {//QSL_SENT
                    queryString = QString("UPDATE log SET qsl_sent='N' WHERE id='%1'").arg(idT);
                    if(!query1.exec(queryString))
                    {
                        emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number(), query1.lastQuery());
                    }
                }
                else
                {

                }
            }
        }

        if (( (i % step )== 0) )
        { // To update the speed I will only show the progress once each X QSOs
            aux = tr("Updating QSLs...\n QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);

        }


        if ( progress.wasCanceled() )
        {
            return;
        }


    }
    */
    //qDebug() << "MainWindow::updateQSLRecAndSent - END"  << endl;
}

void MainWindow::slotOperatingYearComboBoxChanged()
{
       //qDebug() << "MainWindow::slotOperatingYearComboBoxChanged: " << operatingYearsComboBox->currentText() << endl;
    selectedYear = (operatingYearsComboBox->currentText()).toInt();
    showDXMarathon(selectedYear);
}

void MainWindow::defineStationCallsign()
{

    //qDebug() << "MainWindow::defineStationCallsign (currentLog): " << QString::number(currentLog) << endl;
    QString logQRZ;
    logQRZ = dataProxy->getStationCallSignFromLog(currentLog);
      //qDebug() << "MainWindow::defineStationCallsign (logQrz): " << logQRZ << endl;

    if (world->checkQRZValidFormat(logQRZ))
    {
        stationQRZ = logQRZ;
    }
    else
    {
        stationQRZ = mainQRZ;
    }
    myDataTabWidget->setData(myPower, stationQRZ, operatorQRZ, myLocator);

    //qDebug() << "MainWindow::defineStationCallsign: " << stationQRZ << " - END" << endl;

}

void MainWindow::slotSetPropMode(const QString _p)
{
       //qDebug() << "MainWindow::slotSetPropMode: " << _p << endl;
    //if(modify)
    //{
    //    return;
    //}
    othersTabWidget->setPropMode(_p);
    //int indexC = propModeComboBox->findText(" - " + _p + " - ", Qt::MatchContains);
    //propModeComboBox->setCurrentIndex(indexC);
}



void MainWindow::completeWithPreviousQSO(const QString _call)
{
       //qDebug() << "MainWindow::completeWithPreviousQSO" << endl;
    //This function completes: Name, QTH, Locator, Entity, Iota
    if ((!completeWithPrevious) || (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    //if ( (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    {
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
        return;
    }

    QString aux = QString();

    aux = dataProxy->getNameFromQRZ(_call);
    if ((aux.length()>=0) && ((nameLineEdit->text()).length()<=0) )
    {
        nameLineEdit->setPalette(palRed);
        completedWithPreviousName = true;
        nameLineEdit->setText(aux);
    }
    else if (completedWithPreviousName)
    {
        nameLineEdit->clear();
        completedWithPreviousName = false;
        nameLineEdit->setPalette(palBlack);
    }
    else
    {
    }

    aux = dataProxy->getQTHFromQRZ(_call);
    if ((aux.length()>=0) && ((qthLineEdit->text()).length()<=0) )
    {
        qthLineEdit->setPalette(palRed);
        completedWithPreviousQTH = true;
        qthLineEdit->setText(aux);
    }
    else if (completedWithPreviousQTH)
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
    else if (completedWithPreviousLocator)
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
        else if (completedWithPreviousIOTA)
        {
            othersTabWidget->clearIOTA();
            completedWithPreviousName = false;
        }
    }
    else if (completedWithPreviousIOTA)
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
    else if (completedWithPreviousQSLVia)
    {
        QSLTabWidget->setQSLVia("");
    }
}

void MainWindow::slotSatBandTXComboBoxChanged(const QString _q)
{
    //qDebug() << "MainWindow::slotSatBandTXComboBoxChanged" << _q << endl;
    if (modify)
    {
        return;
    }
    //qDebug() << "MainWindow::slotSatBandTXComboBoxChanged1: " << bandComboBox->currentText() << endl;
    bandComboBox->setCurrentIndex(bandComboBox->findText(_q, Qt::MatchCaseSensitive));
    //qDebugf() << "MainWindow::slotSatBandTXComboBoxChanged2: " << bandComboBox->currentText() << endl;
}

void MainWindow::slotFreqTXChanged()
{
    //qDebug() << "MainWindow::slotFreqTXChanged" << QString::number(txFreqSpinBox->value()) << endl;

    QString _q;
    int v = dataProxy->getBandIdFromFreq(txFreqSpinBox->value());
    //if ((v<0) || (modify))
    if (v<0)
    {
        return;
    }

    _q = dataProxy->getNameFromBandId (v);
    if (bandComboBox->findText(_q, Qt::MatchCaseSensitive) < 0)
    {// The selected frequency is of a band that is not currently selected
           //qDebug() << "MainWindow::slotFreqTXChanged - New band found: " << _q << endl;
        if (dataProxy->getIdFromBandName(_q) > 1)
        {// Not affected if 0 (light) is the frequency
         // In this case the user should select the band in the setup
               //qDebug() << "MainWindow::slotFreqTXChanged - Band is valid: " << _q << endl;
            QStringList qsTemp;
            qsTemp.clear();
            qsTemp << bands;
            qsTemp << _q;
            bands.clear();
            bands << dataProxy->sortBandNamesBottonUp(qsTemp);
            bandComboBox->clear();
            bandComboBox->addItems(bands);
            dxccStatusWidget->setBands(bands);
            satTabWidget->addBands(bands);

               //qDebug() << "MainWindow::slotFreqTXChanged - Band has been added!" << endl;
        }
        else
        {
               //qDebug() << "MainWindow::slotFreqTXChanged - Band is NOT  valid: " << _q << endl;
            return;
        }
    }
    //qDebug() << "MainWindow::slotFreqTXChanged - band1: " << bandComboBox->currentText() << endl;
    bandComboBox->setCurrentIndex(bandComboBox->findText(_q, Qt::MatchCaseSensitive));
    //qDebug() << "MainWindow::slotFreqTXChanged - band2: " << bandComboBox->currentText() << endl;
}

void MainWindow::slotFreqRXChanged()
{
    QString _q;
    int v = dataProxy->getBandIdFromFreq(rxFreqSpinBox->value());

    //qDebug() << "MainWindow::slotFreqRXChanged: F/v(rx): " << QString::number(rxFreqSpinBox->value()) << "/" << QString::number(v) << endl;

    if ((v<0) || (modify))
    {
        return;
    }

    //int txv = dataProxy->getBandIdFromFreq(txFreqSpinBox->value());

    if ((rxFreqSpinBox->value() > 0) && (txFreqSpinBox->value()<=0))
    { // There is no TX freq but there is a RX Freq. Band Used (bandid) will be updated to the RX Freq
        _q = dataProxy->getNameFromBandId (v);
        //qDebug() << "MainWindow::slotFreqRXChanged: band: " << _q << endl;
        bandComboBox->setCurrentIndex(bandComboBox->findText(_q, Qt::MatchCaseSensitive));
        //qDebug() << "MainWindow::slotFreqRXChanged: band2: " << bandComboBox->currentText() << endl;
        //qDebug() << "MainWindow::slotFreqRXChanged: NO TX, so RX" << endl;
    }
    else
    {
        //qDebug() << "MainWindow::slotFreqRXChanged: TX is there, no change" << endl;
        //_q = dataProxy->getNameFromBandId (v);
    }
    //qDebug() << "MainWindow::slotFreqRXChanged: END" << endl;
}

void MainWindow::slotShowQSOFromDXCCWidget(const int _q)
{
    //qDebug() << "MainWindow::slotShowQSOFromDXCCWidget: " << QString::number(_q)<< endl;
}

void MainWindow::slotShowQSOsFromDXCCWidget(QList<int> _qsos)
{
    //qDebug() << "MainWindow::slotShowQSOsFromDXCCWidget" << endl;
    slotShowSearchWidget();
    searchWidget->showQSOs(_qsos);

}

void MainWindow::slotWSJTXloggedQSO(const int _type, const QString _dxcall, const double _freq, const QString _mode,
                                              const QString _dx_grid, const QString _time_off, const QString _report_sent, const QString _report_rec,
                                              const QString _tx_power, const QString _comments, const QString _name, const QString _time_on, const QString _de_call, const QString _de_grid)
{

    //qDebug() << "MainWindow::slotWSJTX-loggedQSO type: " << QString::number(_type) << endl;

    bool logTheQso = false;

   //qDebug() << "MainWindow::slotWSJTX-loggedQSO type: " << QString::number(_type) << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO dxcall: " << _dxcall << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO freq: " << QString::number(_freq/1000000) << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO mode: " << _mode << endl;

   //qDebug() << "MainWindow::slotWSJTX-loggedQSO dx_grid: " << _dx_grid << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO time_on: " << _time_on << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO time_off: " << _time_off << endl;

   //qDebug() << "MainWindow::slotWSJTX-loggedQSO report_sent: " << _report_sent << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO report_rec: " << _report_rec << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO tx_power: " << _tx_power << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO comments: " << _comments << endl;
   //qDebug() << "MainWindow::slotWSJTX-loggedQSO name: " << _name << endl;

    if (_type == 5)
    {
        if (wsjtxAutoLog)
        { // Log automatically, without confirmation
            logTheQso = true;
        }
        else
        { // Ask for confirmation before logging
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
                    "<b>" + tr("Time On") + ": " + "</b>" + (QDateTime::fromString(_time_on, "yyyyMMddhhmmss")).toString("yyyy/MM/dd - hh:mm:ss") +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Time Off") + ": " + "</b>" + (QDateTime::fromString(_time_off, "yyyyMMddhhmmss")).toString("yyyy/MM/dd - hh:mm:ss") +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("RST TX") + ": " + "</b>" + _report_sent + " - <b>" + tr("RST RX") + ": " + "</b>" + _report_rec  +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("DX-Grid") + ": " + "</b>" + _dx_grid.toUpper()  +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Name") + ": " + "</b>" + _name  +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Comments") + ": " + "</b>" + _comments +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("TX Pwr") + ": " + "</b>" + _tx_power +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Operator") + ": " + "</b>" + _de_call.toUpper() +
                    "</LI>" +
                    "<LI>" +
                    "<b>" + tr("Local-Grid") + ": " + "</b>" + _de_grid +
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

            QString _oper = _de_call;
            if (!(util->isValidCall(_oper)))
            {
                _oper = operatorQRZ;
            }

            QString _myLoc = _de_grid;
            if (!(locator->isValidLocator(_myLoc)))
            {
                _myLoc = myLocator;
            }

            qsoLogged = dataProxy->addQSOFromWSJTX(_dxcall.toUpper(), _freq,  _mode, _dx_grid, _time_off, _report_sent, _report_rec, _tx_power, _comments, _name, _time_on, dxcc, _oper, stationQRZ, _myLoc, currentLog);

            if (qsoLogged)
            {
                //qDebug() << "MainWindow::slotWSJTX-loggedQSO: Logged QSO OK: " << _dxcall << endl;
                actionsJustAfterAddingOneQSO();
                infoLabel1T = infoLabel1->text();
                infoLabel2T = infoLabel2->text();

                infoLabel1->setText(tr("QSO logged from WSJTX:"));
                infoLabel2->setText(_dxcall + " - " + dataProxy->getBandNameFromFreq(_freq) + "/" + _mode);
                timerInfoBars->start(infoTimeout);
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

    }
    else
    {
        //qDebug() << "MainWindow::slotWSJTX-loggedQSO: type != 5, nothing to do or an error"<< endl;
    }
    //qDebug() << "MainWindow::slotWSJTX-loggedQSO: - END" << endl;
}

bool MainWindow::checkIfNewMode(const QString _mode)
{
   //qDebug() << "MainWindow::checkIfNewMode: " << _mode << endl;
    if (dataProxy->getSubModeIdFromSubMode(_mode)<0)
    {// The mode is not existing; it is not an accepted mode for KLog
     // TODO: Show an error to the user
       //qDebug() << "MainWindow::checkIfNewMode: Mode not valid! - " << _mode << endl;

        QMessageBox msgBox;

        msgBox.setIcon(QMessageBox::Warning);
        QString aux = tr("A new mode not supported by KLog has been received from an external software:") + "(" + _mode + ")\n\n" + tr("If the received mode is correct, please contact KLog development team and request support for that mode") +  "\n\n" + tr("Do you want to keep receiving this alerts? (disabling this alerts will prevent that non-valid modes are detected)");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );

        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Yes:
                return false;   // The user wants to keepseeing errors
            break;
            case QMessageBox::No:
                return true; //No more error shown
                break;
            default:
            // should never be reached
            return false;   // The user wants to keepseeing errors
            break;
        }
    }
    else
    {
        //noMoreModeErrorShown = false;
        //TODO: Add the new mode to the list of active modes
       //qDebug() << "MainWindow::slotStatusFromUDPServer: VALID NEW MODE: Adding... - " << _mode << endl;
        addNewValidMode(_mode);
    }

    return false;
}

void MainWindow::slotWSJXstatusFromUDPServer(const int _type, const QString _dxcall, const double _freq, const QString _mode,
                                             const QString _report, const QString _de_call, const QString _de_grid,
                                             const QString _dx_grid, const QString _sub_mode)
{
    if (modify)
    {
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

    if ((modeComboBox->findText(_mode, Qt::MatchCaseSensitive)<0) && (!noMoreModeErrorShown))
    {


       //qDebug() << "MainWindow::slotStatusFromUDPServer New mode: " << _mode << endl;
        noMoreModeErrorShown = checkIfNewMode(_mode);
/*
        if (dataProxy->getSubModeIdFromSubMode(_mode)<0)
        {// The mode is not existing; it is not an accepted mode for KLog
         // TODO: Show an error to the user
           //qDebug() << "MainWindow::slotStatusFromUDPServer: Mode not valid! - " << _mode << endl;

            QMessageBox msgBox;

            msgBox.setIcon(QMessageBox::Warning);
            QString aux = tr("A new mode not supported by KLog has been received from an external software:") + "(" + _mode + ")\n\n" + tr("If the received mode is correct, please contact KLog development team and request support for that mode") +  "\n\n" + tr("Do you want to keep receiving this alerts? (disabling this alerts will prevent that non-valid modes are detected)");
            msgBox.setText(aux);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );

            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();
            switch (ret)
            {
                case QMessageBox::Yes:
                break;
                case QMessageBox::No:
                    noMoreModeErrorShown = true;
                    break;
                default:
                // should never be reached
                break;
            }
        }
        else
        {
            //noMoreModeErrorShown = false;
            //TODO: Add the new mode to the list of active modes
           //qDebug() << "MainWindow::slotStatusFromUDPServer: VALID NEW MODE: Adding... - " << _mode << endl;
            addNewValidMode(_mode);
        }
    */
    }

    switch (_type)
    {
        case 0:
            //MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT/IN - Heartbeat" << endl;
        break;
        case 1:
           //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - OUT - Status" << endl;
             qrzLineEdit->setText(_dxcall);
             if ((!noMoreModeErrorShown) && (dataProxy->getSubModeIdFromSubMode(_mode)>0) )
             {
                modeComboBox->setCurrentIndex(modeComboBox->findText(_mode, Qt::MatchCaseSensitive));
             }

             txFreqSpinBox->setValue(_freq);
             slotUpdateLocator(_dx_grid);
             rstTXLineEdit->setText(_report);
             myDataTabWidget->setMyLocator(_de_grid);
             myDataTabWidget->setStationQRZ(_de_call.toUpper());

             //TODO: Check what to do with _de_call -> Check if _de_call == station callsign and update if needed.
             //TODO: Check what to do with _de_grid -> Check if _de_grid == My Grid and update if needed.
             //TODO: Check what to do with _submode.


             //bandComboBox->setCurrentIndex(bandComboBox->findText(, Qt::MatchCaseSensitive));

        default: //NO
           //qDebug() << "MainWindow::slotStatusFromUDPServer: -   type = " << QString::number(_type) << " - ERROR on Type" << endl;
        break;
    }
}



void MainWindow::addNewValidMode(const QString _mode)
{
   //qDebug() << "MainWindow::addNewMode: " << _mode << endl;
    QStringList _newM;
    _newM.clear();
    _newM << _mode;
    for (int i = 0;i < modeComboBox->count(); i++)
    {
        _newM << modeComboBox->itemText(i);
    }

    readActiveModes (_newM);
    modeComboBox->clear();
    modeComboBox->addItems(modes);
}

void MainWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString queryFailed)
{
    //qDebug() << "MainWindow::slotQueryErrorManagement: Function: " << functionFailed << endl;
    //qDebug() << "MainWindow::slotQueryErrorManagement: Error N#: " << QString::number(errorCodeN) << endl;
    //qDebug() << "MainWindow::slotQueryErrorManagement: Error: " << functionFailed << errorCodeS << endl;


    if (noMoreErrorShown)
    {return;}
    // TODO: An error on DB has been detected.
    // KLog should suggest to export ALL the data to an ADIF file to prevent any log lose

    QString aux = "<br><b>" + tr("An unexpected error ocurred!!") + "</b><br><br>" + tr("If the problem persists, please contact the developers") + "(<a href=mailto:klog-devel@nongnu.org>klog-devel@nongnu.org</a>)" + tr("for analysis:") + "<br>";
    QString errorMSG =  "<ul>"
                        "<li><b>" + tr("Error in function") + ":</b> " + functionFailed + "</li>" +
                        "<li><b>" + tr("Error code") +":</b> " + QString::number(errorCodeN) + "</li>" +
                        "<li><b>" + tr("Error text") + ":</b> " + errorCodeS + "</li>" +
                        "<li><b>" + tr("Failed query") + ":</b> " + queryFailed + "</li>" +
                        "</ul><br>"
                        "<b>Recomendation:</b> Export, periodically, your data to ADIF to prevent a potential data loss.<br>";

    showErrorDialog->setText(aux + errorMSG);
    //showErrorDialog->setModal(true);
    showErrorDialog->exec();

    QMessageBox msgBox;

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
}

void MainWindow::slotChangeRXFreq(const QString _f)
{
    rxFreqSpinBox->setValue(_f.toDouble());
}

void MainWindow::slotChangeTXFreq(const QString _f)
{
    txFreqSpinBox->setValue(_f.toDouble());
}

void MainWindow::slotUpdateLocator(QString _loc)
{
    locatorLineEdit->setText(_loc.toUpper());
}

