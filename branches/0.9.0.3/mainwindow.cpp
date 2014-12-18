/***************************************************************************
                          mainwindow.cpp  -  description
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include <QtGui>
#include <QtSql>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "mainwindow.h"

//#include <qDebug>

MainWindow::MainWindow(const QString _kontestDir, const QString tversion)
{
    upAndRunning = false; // To define some actions that can only be run when starting the software
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotDownloadFinished(QNetworkReply*)));


    // <ui>
    doc = new QTextDocument;

    DBinMemory = false;
    needToEnd = false;
    dxclusterServerToConnect = "dxfun.com";
    dxclusterServerPort = 8000;
    contestMode = NoContest;

    defaultADIFLogFile = "";
    softwareVersion = tversion;

    kontestDir = _kontestDir;
    InValidCharsInPrevCall = false;
    stationCallSignShownInSearch = true;
    configured = false;
    modify = false;
    qslingNeeded = false; // When clicking on Find QSO to QSL
    defaultMode = 0;
    defaultBand = 0;
    currentMode = 0;
    currentModeShown = currentMode;
    currentBand = 0;
    currentBandShown = currentBand;
    currentLog = 0;
    points = 0;
    multipliers = 0;
    qsoPoints = 0;
    qsoMultiplier = 0;
    operatorQRZ = "";
    stationQRZ = "";
    myLocator = "";
    dxLocator ="";
    myPower = 0.0;
    currentEntity = -1; // To optimize the calls to different world methods if the entity does not change. Used in slotQRZTextChanged
    previousEntity = -1;// To optimize the calls to different world methods if the entity does not change.
    realTime=true;
    UTCTime=true;
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

    defaultColor.setNamedColor("slategrey");
    neededColor.setNamedColor("yellow");
    workedColor.setNamedColor("blue");
    confirmedColor.setNamedColor("red");
    newOneColor.setNamedColor("green");

    //Default band/modes
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW" << "RTTY";

    logModel = new QSqlRelationalTableModel(this);
    logView = new QTableView;
    logView->setContextMenuPolicy(Qt::CustomContextMenu);
    logView->setSortingEnabled(true);

    helpHelpDialog = new HelpHelpDialog(softwareVersion);
    helpAboutDialog = new HelpAboutDialog(softwareVersion);

    searchResultsTreeWidget = new QTreeWidget;
    searchResultsTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    searchResultsTreeWidget->setSortingEnabled(true);
    //searchResultsTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    searchResultsTreeWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    searchBoxClearButton = new QPushButton(tr("&Clear"), this);
    searchBoxExportButton  = new QPushButton(tr("&Export Highlited"), this);
    searchBoxSelectAllButton  = new QPushButton(tr("&Select All"), this);
    searchBoxReSearchButton = new QPushButton(tr("&Search"), this);
    searchSelectAllClicked = false;

    recalculateAwardsButton = new QPushButton(tr("Recalculate"), this);
    recalculateAwardsButton->setToolTip(tr("Click to recalculate the award status"));

    scoreTextEdit = new QTextEdit;

    distShortLabelN = new QLabel;
    distLongLabelN = new QLabel;

    #ifdef Q_OS_WIN
        //qDebug() << "WINDOWS DETECTED!"  << endl;
        //kontestDir = QDir::homePath()+"/kontest";  // We create the \kontest for the logs and data
        configFileName = kontestDir+"/klogrc.cfg";
    #else
        //qDebug() << "NO WINDOWS DETECTED!"  << endl;
        //kontestDir = QDir::homePath()+"/.kontest";  // We create the ~/.kontest for the logs and data
        configFileName = kontestDir+"/klogrc";
    #endif


    //qDebug() << "MainWindow::MainWindow: logbook: " << QString(kontestDir + "logbook.dat") << endl;

        bool existingData = QFile::exists(kontestDir + "logbook.dat");

        if (existingData)
        {
           //qDebug() << "MainWindow::MainWindow: existing data" << endl;
        }
        else
        {
            //qDebug() << "MainWindow::MainWindow: NOT existing data" << endl;
        }

    statusBarMessage = tr("Starting KLog");

    if (!QDir::setCurrent ( kontestDir )){
        QDir d1(kontestDir);
        if (d1.mkdir(kontestDir))
        {
            QDir::setCurrent ( kontestDir );
        }
    }

    db = new DataBase(softwareVersion, DBinMemory);



    //qDebug() << "MainWindow::MainWindow: 1 " << endl;
    world = new World(kontestDir, softwareVersion);

    //qDebug() << "MainWindow::MainWindow: 2" << endl;


    //readConfigData();
    //qDebug() << "MainWindow::MainWindow: 3" << endl;

    if (!db->createConnection())
    {
        //qDebug() << "MainWindow::MainWindow: 4" << endl;
        return;
    }
    else
    {

        //qDebug() << "MainWindow::MainWindow: 5" << endl;
        if (!existingData)
        {
            //qDebug() << "MainWindow::MainWindow: !existingData" << endl;
            world->create(kontestDir);

            //createData();
        }else
        {
            //qDebug() << "MainWindow::MainWindow: existingData" << endl;
        }

        db->updateIfNeeded(); // Check if we need to update the DB

    }
    if (configured)
    {
        //qDebug() << "MainWindow::MainWindow: configured = true" << endl;
    }
    else
    {
        //qDebug() << "MainWindow::MainWindow: configured = false" << endl;
    }
    setupDialog = new SetupDialog(!configured);
    dataProxy = new DataProxy_SQLite();

    filemanager = new FileManager(kontestDir, softwareVersion, *db);

    locator = new Locator();
    awards = new Awards();

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    dateTime = new QDateTime();


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()) );
    timer->start(1000);

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
    dateEdit = new QDateEdit;
    timeEdit = new QTimeEdit;
    statusBar = new QStatusBar;
    qsoStatusBar = new QStatusBar;
    OKButton = new QPushButton(tr("&Add"), this);
    spotItButton = new QPushButton(tr("&Spot"), this);
    clearButton = new QPushButton(tr("&Clear"), this);
    //mainToolBar = new QToolBar(this);
    //numberOfQso = 1;

    // UI DX
    infoLabel1 = new QLabel(tr("Status bar..."));
    infoLabel2 = new QLabel(tr("DX Entity"));
    bandLabel1 = new QLabel(tr("10M"));
    bandLabel2 = new QLabel(tr("15M"));
    bandLabel3 = new QLabel(tr("20M"));
    bandLabel4 = new QLabel(tr("40M"));
    bandLabel5 = new QLabel(tr("80M"));
    bandLabel6 = new QLabel(tr("160M"));
    bandLabel7 = new QLabel(tr("2M"));
    bandLabel8 = new QLabel(tr("6M"));
    bandLabel9 = new QLabel(tr("12M"));
    bandLabel10 = new QLabel(tr("17M"));
    bandLabel11 = new QLabel(tr("30M"));
    bandLabel12 = new QLabel(tr("70CM"));
    entityAwardLabel = new QLabel(tr("Award"));
    iotaAwardLabel = new QLabel(tr("IOTA"));
    entityNameLabel = new QLabel(tr("Entity"));
    iotaContinentComboBox = new QComboBox;
    entityAwardComboBox = new QComboBox;
    entityNameComboBox = new QComboBox;
    //notesTextEdit = new QTextEdit;
    commentLineEdit = new QLineEdit;
    continentLabel = new QLabel;
    prefixLabel = new QLabel;
    cqzLabel = new QLabel;
    ituzLabel = new QLabel;
    gradShortLabel = new QLabel;
    distShortLabel = new QLabel;
    gradLongLabel = new QLabel;
    distLongLabel = new QLabel;    
    logPanel = new QWidget;
    loggWinAct = new QAction(tr("&Log Window"), this);
    scoreeWinAct = new QAction(tr("&Score Window"), this);

    scoreWindow = new QWidget;
    operatorLineEdit = new QLineEdit;
    stationCallSignLineEdit = new QLineEdit;
    myLocatorLineEdit = new QLineEdit;

    myPowerSpinBox = new QDoubleSpinBox;
    myPowerSpinBox->setDecimals(2);
    myPowerSpinBox->setMaximum(9999);

    rxPowerSpinBox = new QDoubleSpinBox;
    rxPowerSpinBox->setDecimals(2);
    rxPowerSpinBox->setMaximum(9999);

    txFreqSpinBox = new QDoubleSpinBox;
    txFreqSpinBox->setDecimals(3);
    txFreqSpinBox->setMaximum(9999);
    txFreqSpinBox->setSuffix(tr("MHz"));

    rxFreqSpinBox = new QDoubleSpinBox;
    rxFreqSpinBox->setDecimals(3);
    rxFreqSpinBox->setMaximum(9999);
    rxFreqSpinBox->setSuffix(tr("MHz"));

    //freqQLCDNumber = new QLCDNumber;
    //freqQLCDNumber->setDigitCount(7);
    //freqQLCDNumber->setSmallDecimalPoint(true);

    dxccConfirmedQLCDNumber = new QLCDNumber;
    dxccWorkedQLCDNumber = new QLCDNumber;
    wazConfirmedQLCDNumber = new QLCDNumber;
    wazWorkedQLCDNumber = new QLCDNumber;
    localConfirmedQLCDNumber = new QLCDNumber;
    localWorkedQLCDNumber = new QLCDNumber;
    qsoConfirmedQLCDNumber = new QLCDNumber;
    qsoWorkedQLCDNumber = new QLCDNumber;
    qsoWorkedQLCDNumber->setDigitCount(7);
    qsoConfirmedQLCDNumber->setDigitCount(7);
    qslSentComboBox = new QComboBox;
    qslRecComboBox = new QComboBox;
    eqslSentComboBox = new QComboBox;
    eqslRecComboBox = new QComboBox;
    lotwSentComboBox = new QComboBox;
    lotwRecComboBox = new QComboBox;

    QStringList qsAux;
    qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Validated");
    eqslRecComboBox->addItems(qsAux);
    lotwRecComboBox->addItems(qsAux);

    qsAux.clear();
    qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    eqslSentComboBox->addItems(qsAux);
    lotwSentComboBox->addItems(qsAux);
    qslSentComboBox->addItems(qsAux);

    qsAux.clear();
    qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
    qslRecComboBox->addItems(qsAux);

    qslSentViaComboBox = new QComboBox;
    qslRecViaComboBox = new QComboBox;

    qsAux.clear();
    qsAux << tr("B-Bureau") << tr("D-Direct") << tr("E-Electronic") << tr("M-Manager");    
    qslSentViaComboBox->addItems(qsAux);
    qslRecViaComboBox->addItems(qsAux);

    eqslSentQDateEdit = new QDateEdit;
    eqslRecQDateEdit = new QDateEdit;
    lotwSentQDateEdit = new QDateEdit;
    lotwRecQDateEdit = new QDateEdit;

    qslSentQDateEdit = new QDateEdit;
    qslRecQDateEdit = new QDateEdit;
    qslmsgTextEdit = new QTextEdit;
    qslViaLineEdit = new QLineEdit;


    qslRecComboBox->setCurrentIndex(1); // Not received
    qslSentComboBox->setCurrentIndex(1); // Not sent
    eqslSentComboBox->setCurrentIndex(1);
    eqslRecComboBox->setCurrentIndex(1);
    lotwSentComboBox->setCurrentIndex(1);
    lotwRecComboBox->setCurrentIndex(1);

    // Check date & time and set them in the UI at the begining
    dateTime->currentDateTime();
    dateEdit->setDate((dateTime->currentDateTime()).date());
    timeEdit->setTime((dateTime->currentDateTime()).time());

    //Search tab
    searchBoxLineEdit = new QLineEdit;

    // UI DX

    // CLUSTER
    dxClusterWidget = new DXClusterWidget(dxclusterServerToConnect , dxclusterServerPort, this);
    //dxClusterWidget = new DXClusterWidget(this);
    //QWidget *dxClusterTabWidget = new QWidget;


    // </CLUSTER>

    // </UI>



//**************************************************


    //createDXClusterUI();
    connect( setupDialog, SIGNAL(exitSignal(int)), this, SLOT(slotExitFromSlotDialog(int)) );

    readConfigData();
    if (needToEnd)
    {
       exit(0);
    }


    createUI();
    createlogModel();
    createlogPanel();
    createSearchResultsPanel();
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
    logView->setCurrentIndex(logModel->index(0, 0));
    //searchResultsTreeWidget->setCurrentIndex(logModel->index(0, 0));

    checkIfNewBandOrMode();

    if ( (contestMode == CQ_WW_SSB) || (contestMode == CQ_WW_CW) )
    {

    }
    else if ( (contestMode == CQ_WPX_SSB) || (contestMode == CQ_WPX_CW) )
    {

    } else
    {
        updateQSLRecAndSent();
        awards->recalculateAwards();
    }

    slotClearButtonClicked();

    upAndRunning = true;
    //qDebug() << "MainWindow::MainWindow: END" << endl;

}


void MainWindow::createUI()
{

 //qDebug() << "MainWindow::createUI" << endl;

    if ( (contestMode == CQ_WW_SSB) || (contestMode == CQ_WW_CW) )
    {
        createUICQWW();
        createActionsCommon();
        createActionsCQWW();
        createMenusCommon();
        createMenusCQWW();

    }
    else if ( (contestMode == CQ_WPX_SSB) || (contestMode == CQ_WPX_CW) )
    {

    }
    else
    {
        createUIDX();
        createActionsCommon();
        createActionsDX();
        createMenusCommon();
    }
}

void MainWindow::slotModeComboBoxChanged(){
    //qDebug() << "MainWindow::slotModeComboBoxChanged: " << QString::number(modeComboBox->currentIndex()) << endl;

    int i;
    i = dataProxy->getIdFromModeName(modeComboBox->currentText());
    if (i>=0)
    {
        currentMode = i;
    }

    //qDebug() << "MainWindow::slotModeComboBoxChanged: " << QString::number(modeComboBox->currentIndex()) << "/" << QString::number(currentMode) << endl;

    currentBandShown = bandComboBox->currentIndex();
    currentModeShown = modeComboBox->currentIndex();
    i = checkIfWorkedB4(currentQrz);

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << currentQrz << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);
    showStatusOfDXCC(_qs);

    //int i = checkIfWorkedB4(currentQrz);

}

void MainWindow::slotBandComboBoxChanged(){
   //qDebug() << "MainWindow::slotBandComboBoxChanged: " << QString::number(bandComboBox->currentIndex()) << endl;
    int i;
    i = dataProxy->getIdFromBandName(bandComboBox->currentText());
    if (i>=0)
    {
        currentBand = i;
    }


    //qDebug() << "MainWindow::slotBandComboBoxChanged: " << QString::number(bandComboBox->currentIndex()) << "/" << QString::number(currentBand) << endl;

    currentBandShown = bandComboBox->currentIndex();
    currentModeShown = modeComboBox->currentIndex();
    i = checkIfWorkedB4(currentQrz);

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << currentQrz << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);
    showStatusOfDXCC(_qs);
}


void MainWindow::slotQRZReturnPressed()
{
    //qDebug() << "MainWindow::slotQRZReturnPressed: " << qrzLineEdit->text() << " - " << QString::number(bandComboBox->currentIndex()) << "/" << QString::number(modeComboBox->currentIndex()) << endl;
    //int newId = -1;
    int lastId = -1;
    int errorCode = 0;
    QString aux;

    //bool ret = false;
    QString tqrz = qrzLineEdit->text();
    //currentMode = modeComboBox->currentIndex();
    //currentBandShown = bandComboBox->currentIndex();
    slotBandComboBoxChanged();
    slotModeComboBoxChanged();


    switch (contestMode) { // Just to prepare or some tasks before reading DATA from UI
    case CQ_WW_SSB:
        //qDebug() << "MainWindow::slotQRZReturnPressed: CQ-WW-SSB:" << endl;
        if ((SRXLineEdit->text()).toInt() < 1    ){
            return;
        }
        break;
        case CQ_WW_CW:
            //qDebug() << "MainWindow::slotQRZReturnPressed: CQ-WW-CW:" << endl;
        break;
        default:
            //qDebug() << "MainWindow::slotQRZReturnPressed: Default:" << endl;
        break;

        }


        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;
        QString queryString = readDataFromUI();

        //qDebug() << "MainWindow::slotQRZReturnPressed: queryString: " << queryString << endl;

        if (queryString != "NULL") {
            if (!query.exec(queryString))
            {
                //qDebug() << "MainWindow::slotQRZReturnPressed: Query ERROR: (queryString): " << queryString << endl;
                errorCode = query.lastError().number();
                QMessageBox msgBox;
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
                //TODO: To move the following lines to this part to properly manage the query result!!
                //ret = true;

                needToSave = true;
                if (modify)
                {
                    if(modifyingQSO>0)
                    {
                        awards->setAwards(modifyingQSO);
                    }

                    // CHECK WHAT WAS THE QSOID to add the awards, if needed
                }
                else
                {
                    lastId = dataProxy->getLastQSOid();
                    if (lastId>=0)
                    {
                        awards->setAwards(lastId);   //Update the DXCC award status
                    }
                }


                logModel->select();
                slotClearButtonClicked();
            }
        }
        else   // The QUERY string is NULL
        {
            //qDebug() << "MainWindow::slotQRZReturnPressed: queryString-NULL: " << queryString << endl;
        }


    modify = false;
    modifyingQSO = -1;
    OKButton->setText(tr("&Add"));
}

QString MainWindow::readDataFromUI()
{

    //qDebug() << "MainWindow::readDataFromUI: " << endl;

    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (tqrz.length()<3)
    {
        return "NULL";
    }

    switch (contestMode)
    {
        case CQ_WW_SSB:
            //return readDataFromUICQWWSSB();
        break;
        case CQ_WW_CW:
            //return readDataFromUICQWWCW();
          break;
        default:
            if (modify)
            {
                return readDataFromUIDXModifying();
            }
            else
            {
                return readDataFromUIDX();
            }
        break;
    }

    //qDebug() << "MainWindow::readDataFromUI: END" << endl;
    return "NULL";
}

QString MainWindow::readDataFromUIDX()
{
    //qDebug() << "MainWindow::readDataFromUIDX:" << endl;

    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (tqrz.length()<3)
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

    if ( (txFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(txFreqSpinBox->value());
        stringFields = stringFields + ", freq";
        stringData = stringData + ", '" + aux1 + "'";
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

    aux1 = operatorLineEdit->text();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", operator";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = stationCallSignLineEdit->text();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", station_callsign";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myLocatorLineEdit->text();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", my_gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = commentLineEdit->text();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", comment";
        stringData = stringData + ", '" + aux1 + "'";
    }
 /*
    aux1 = notesTextEdit->toPlainText();

    if (aux1.length()>3)
    {
        //qDebug() << "MainWindow::readDataFromUIDX - Notes: " << aux1 << endl;
        stringFields = stringFields + ", notes";
        stringData = stringData + ", '" + aux1 + "'";
    }
*/
    aux1 = qslmsgTextEdit->toPlainText();
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

    aux1 = qslViaLineEdit->text();
    if (aux1.length()>3)
    {
        stringFields = stringFields + ", qsl_via";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(myPowerSpinBox->value());
    if ((aux1.toDouble())>0.0)
    {
        stringFields = stringFields + ", tx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(rxPowerSpinBox->value());
    if ((aux1.toDouble())>0.0)
    {
        stringFields = stringFields + ", rx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = iotaNumberLineEdit->text();
    if ( (aux1.toInt()) > 0 )
    {
        aux2 = iotaContinentComboBox->currentText() + "-" + aux1;
        aux1 = awards->checkIfValidIOTA(aux2);
        //qDebug() << "MainWindow::qsoToEdit: IOTA-CheckIfValidIOTA-1 " << aux2 << endl;
        //qDebug() << "MainWindow::qsoToEdit: IOTA-CheckIfValidIOTA-2 " << aux1 << endl;
        if (aux1.length() != 6) // EU-001
        {
            stringFields = stringFields + ", iota";
            stringData = stringData + ", '" + aux1 + "'";
        }
    }
    // EQSL-SENT

    int i = eqslSentComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    switch (i)
    {
        case 0: // Y-Yes
            stringFields = stringFields + ", eqsl_qsl_sent";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", eqsl_qslsdate";
            stringData = stringData + ", '" + (eqslSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 1: //N-No
            stringFields = stringFields + ", eqsl_qsl_sent";
            stringData = stringData + ", 'N'";
        break;
        case 2: // R-Requested
            stringFields = stringFields + ", eqsl_qsl_sent";
            stringData = stringData + ", 'R'";
          //stringFields = stringFields + ", eqsl_qslsdate";
          //stringData = stringData + ", '" + (eqslSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 3: // Q-Queued
            stringFields = stringFields + ", eqsl_qsl_sent";
            stringData = stringData + ", 'Q'";
            stringFields = stringFields + ", eqsl_qslsdate";
            stringData = stringData + ", '" + (eqslSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 4: // I-Ignore
      //qslSentQDateEdit->setEnabled(true);
            stringFields = stringFields + ", eqsl_qsl_sent";
            stringData = stringData + ", 'I'";
            stringFields = stringFields + ", eqsl_qslsdate";
            stringData = stringData + ", '" + (eqslSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        default: //N-No
            stringFields = stringFields + ", eqsl_qsl_sent";
            stringData = stringData + ", 'N'";
        break;
    }
   // EQSL-RECEPTION
    i = eqslRecComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    switch (i)
    {
        case 0: // Y-Yes
            stringFields = stringFields + ", eqsl_qsl_rcvd";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", eqsl_qslrdate";
            stringData = stringData + ", '" + (eqslRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 1: //N-No
            stringFields = stringFields + ", eqsl_qsl_rcvd";
            stringData = stringData + ", 'N'";
        break;
        case 2: // R-Requested
            stringFields = stringFields + ", eqsl_qsl_rcvd";
            stringData = stringData + ", 'R'";
        break;
        case 3: // Q-Queued
            stringFields = stringFields + ", eqsl_qsl_rcvd";
            stringData = stringData + ", 'Q'";
            stringFields = stringFields + ", eqsl_qslrdate";
            stringData = stringData + ", '" + (eqslRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 4: // I-Ignore
            stringFields = stringFields + ", eqsl_qsl_rcvd";
            stringData = stringData + ", 'I'";
            stringFields = stringFields + ", eqsl_qslrdate";
            stringData = stringData + ", '" + (eqslRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        default: //N-No
            stringFields = stringFields + ", eqsl_qsl_rcvd";
            stringData = stringData + ", 'N'";
        break;
    }
    // LOTW-SENT
    i = lotwSentComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    switch (i)
    {
        case 0: // Y-Yes
            stringFields = stringFields + ", lotw_qsl_sent";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", lotw_qslsdate";
            stringData = stringData + ", '" + (lotwSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 1: //N-No
            stringFields = stringFields + ", lotw_qsl_sent";
            stringData = stringData + ", 'N'";
        break;
        case 2: // R-Requested
            stringFields = stringFields + ", lotw_qsl_sent";
            stringData = stringData + ", 'R'";
        break;
        case 3: // Q-Queued
            stringFields = stringFields + ", lotw_qsl_sent";
            stringData = stringData + ", 'Q'";
            stringFields = stringFields + ", lotw_qslsdate";
            stringData = stringData + ", '" + (lotwSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 4: // I-Ignore
            stringFields = stringFields + ", lotw_qsl_sent";
            stringData = stringData + ", 'I'";
            stringFields = stringFields + ", lotw_qslsdate";
            stringData = stringData + ", '" + (lotwSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        default: //N-No
            stringFields = stringFields + ", lotw_qsl_sent";
            stringData = stringData + ", 'N'";
        break;
    }
    // LOTW-RECEPTION
    //LOTW_QSLRDATE: (only valid if LOTW_RCVD is Y, I, or V)
    i = lotwRecComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    switch (i)
    {
        case 0: // Y-Yes
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", lotw_qslrdate";
            stringData = stringData + ", '" + (lotwRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 1: //N-No
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'N'";
        break;
        case 2: // R-Requested
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'R'";
        break;
        case 3: // Q-Queued
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'Q'";
            stringFields = stringFields + ", lotw_qslrdate";
            stringData = stringData + ", '" + (lotwRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        case 4: // I-Ignore
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'I'";
            stringFields = stringFields + ", lotw_qslrdate";
            stringData = stringData + ", '" + (lotwRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
        break;
        default: //N-No
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'N'";
        break;
    }
    // QSL SENT
    i = qslSentComboBox->currentIndex();
    int ii = qslSentViaComboBox->currentIndex();
    switch (i)
    {
        case 0: // Y-Yes
            stringFields = stringFields + ", qsl_sent";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", qslsdate";
            stringData = stringData + ", '" + (qslSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
            stringFields = stringFields + ", qsl_sent_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        case 1: //N-No
            stringFields = stringFields + ", qsl_sent";
            stringData = stringData + ", 'N'";
            stringFields = stringFields + ", qsl_sent_via";
            stringData = stringData + ", 'B'";
        break;
        case 2: // R-Requested
            stringFields = stringFields + ", qsl_sent";
            stringData = stringData + ", 'R'";
            stringFields = stringFields + ", qsl_sent_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        case 3: // Q-Queued
            stringFields = stringFields + ", qsl_sent";
            stringData = stringData + ", 'Q'";
            stringFields = stringFields + ", qslsdate";
            stringData = stringData + ", '" + (qslSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
            stringFields = stringFields + ", qsl_sent_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        case 4: // I-Ignore
            stringFields = stringFields + ", qsl_sent";
            stringData = stringData + ", 'I'";
            stringFields = stringFields + ", qslsdate";
            stringData = stringData + ", '" + (qslSentQDateEdit->date()).toString("yyyy/MM/dd") + "'";
            stringFields = stringFields + ", qsl_sent_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        default: //N-No
            stringFields = stringFields + ", qsl_sent";
            stringData = stringData + ", 'N'";
            stringFields = stringFields + ", qsl_sent_via";
            stringData = stringData + ", 'B'";
        break;
    }

     // QSL RECEPTION
    i = qslRecComboBox->currentIndex();
    ii = qslRecViaComboBox->currentIndex();

    switch (i)
    { // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
        case 0: // Y-Yes
        //QSL received date: {Y, N, R, I, V}
        //(only valid if QSL_RCVD is Y, I, or V)
            stringFields = stringFields + ", qsl_rcvd";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", qslrdate";
            stringData = stringData + ", '" + (qslRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
            stringFields = stringFields + ", confirmed";
            stringData = stringData + ", '1'";
            stringFields = stringFields + ", qsl_rcvd_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        case 1: //N-No
            stringFields = stringFields + ", qsl_rcvd";
            stringData = stringData + ", 'N'";
            stringFields = stringFields + ", qsl_rcvd_via";
            stringData = stringData + ", 'B'";
            //stringFields = stringFields + ", confirmed";
            //stringData = stringData + ", '0'";
        break;
        case 2: // R-Requested
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            stringFields = stringFields + ", qsl_rcvd";
            stringData = stringData + ", 'R'";
            //stringFields = stringFields + ", confirmed";
            //stringData = stringData + ", '0'";
            stringFields = stringFields + ", qsl_rcvd_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        case 3: // I-Ignore
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            stringFields = stringFields + ", qsl_rcvd";
            stringData = stringData + ", 'I'";
            stringFields = stringFields + ", qslrdate";
            stringData = stringData + ", '" + (qslRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
            //stringFields = stringFields + ", confirmed";
            //stringData = stringData + ", '0'";
            stringFields = stringFields + ", qsl_rcvd_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        case 4: // V-Verified
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            stringFields = stringFields + ", qsl_rcvd";
            stringData = stringData + ", 'V'";
            stringFields = stringFields + ", qslrdate";
            stringData = stringData + ", '" + (qslRecQDateEdit->date()).toString("yyyy/MM/dd") + "'";
            //TODO: Check if the QSL has been received or not as this "V" could mask a received QSL as a Worked (0)
            //stringFields = stringFields + ", confirmed";
            //stringData = stringData + ", '0'";
            stringFields = stringFields + ", qsl_rcvd_via";
            switch (ii)
            {
                case 0: //B
                    stringData = stringData + ", 'B'";
                break;
                case 1: //D
                    stringData = stringData + ", 'D'";
                break;
                case 2: //E
                    stringData = stringData + ", 'E'";
                break;
                case 3: //M
                    stringData = stringData + ", 'M'";
                break;
                default:
                    stringData = stringData + ", 'B'";
                break;
            }
        break;
        default: //N-No
            stringFields = stringFields + ", qsl_rcvd";
            stringData = stringData + ", 'N'";
            stringFields = stringFields + ", qsl_rcvd_via";
            stringData = stringData + ", 'B'";
            //stringFields = stringFields + ", confirmed";
            //stringData = stringData + ", '0'";
        break;
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
    if (tqrz.length()<3)
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
        updateString = updateString + "freq = '";
        updateString = updateString + aux1 + "', ";
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

    aux1 = operatorLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "operator = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = stationCallSignLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "station_callsign = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = myLocatorLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "my_gridsquare = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = commentLineEdit->text();
    if (aux1.length()>0)
    {
        updateString = updateString + "comment = '";
        updateString = updateString + aux1 + "', ";
    }

    /*
    aux1 = notesTextEdit->toPlainText();
    if (aux1.length()>3)
    {
        updateString = updateString + "notes = '";
        updateString = updateString + aux1 + "', ";
    }
*/
    aux1 = qslmsgTextEdit->toPlainText();
    if (aux1.length()>0)
    {
        updateString = updateString + "qslmsg = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(dxcc);
    if (aux1.length()>0)
    {
        updateString = updateString + "dxcc = '";
        updateString = updateString + aux1 + "', ";
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

    aux1 = qslViaLineEdit->text();
    if (aux1.length()>3)
    {
        updateString = updateString + "qsl_via = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(myPowerSpinBox->value());
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

    aux1 = iotaNumberLineEdit->text();
    if ( (aux1.toInt()) > 0 )
    {
        aux2 = iotaContinentComboBox->currentText() + "-" + aux1;
        aux1 = awards->checkIfValidIOTA(aux2);
        if (aux1.length() != 6) // EU-001
        {
            updateString = updateString + "iota = '";
            updateString = updateString + aux1 + "', ";
        }
    }
    // EQSL-SENT

    int i = eqslSentComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    switch (i)
    {
        case 0: // Y-Yes
            updateString = updateString + "eqsl_qsl_sent = 'Y', ";
            updateString = updateString + "eqsl_qslsdate = '" + (eqslSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 1: //N-No
            updateString = updateString + "eqsl_qsl_sent = 'N', ";
        break;
        case 2: // R-Requested
            updateString = updateString + "eqsl_qsl_sent = 'R', ";
        break;
        case 3: // Q-Queued
            updateString = updateString + "eqsl_qsl_sent = 'Q', ";
            updateString = updateString + "eqsl_qslsdate = '" + (eqslSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 4: // I-Ignore
            updateString = updateString + "eqsl_qsl_sent = 'I', ";
            updateString = updateString + "eqsl_qslsdate = '" + (eqslSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        default: //N-No
            updateString = updateString + "eqsl_qsl_sent = 'N', ";
        break;
    }
   // EQSL-RECEPTION
    i = eqslRecComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Validated");
    switch (i)
    {
        case 0: // Y-Yes
            updateString = updateString + "eqsl_qsl_rcvd = 'Y', ";
            updateString = updateString + "eqsl_qslrdate = '" + (eqslRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 1: //N-No
            updateString = updateString + "eqsl_qsl_rcvd = 'N', ";
        break;
        case 2: // R-Requested
            updateString = updateString + "eqsl_qsl_rcvd = 'R', ";
        break;
        case 3: // Q-Queued
            updateString = updateString + "eqsl_qsl_rcvd = 'I', ";
            updateString = updateString + "eqsl_qslrdate = '" + (eqslRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 4: // I-Ignore
            updateString = updateString + "eqsl_qsl_rcvd = 'V', ";
            updateString = updateString + "eqsl_qslrdate = '" + (eqslRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        default: //N-No
            updateString = updateString + "eqsl_qsl_rcvd = 'N', ";
        break;

        }
    // LOTW-SENT

    i = lotwSentComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    switch (i)
    {
        case 0: // Y-Yes
            updateString = updateString + "lotw_qsl_sent = 'Y', ";
            updateString = updateString + "lotw_qslsdate = '" + (lotwSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 1: //N-No
            updateString = updateString + "lotw_qsl_sent = 'N', ";
        break;
        case 2: // R-Requested
            updateString = updateString + "lotw_qsl_sent = 'R', ";
        break;
        case 3: // Q-Queued
            updateString = updateString + "lotw_qsl_sent = 'Q', ";
            updateString = updateString + "lotw_qslsdate = '" + (lotwSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 4: // I-Ignore
            updateString = updateString + "lotw_qsl_sent = 'I', ";

            updateString = updateString + "lotw_qslsdate = '" + (lotwSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        default: //N-No
            updateString = updateString + "lotw_qsl_sent = 'N', ";
        break;
    }

    // LOTW-RECEPTION
    //LOTW_QSLRDATE: (only valid if LOTW_RCVD is Y, I, or V)
    i = lotwRecComboBox->currentIndex();
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Validated");
    switch (i)
    {
        case 0: // Y-Yes
            updateString = updateString + "lotw_qsl_rcvd = 'Y', ";
            updateString = updateString + "lotw_qslrdate = '" + (lotwRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 1: //N-No
            updateString = updateString + "lotw_qsl_rcvd = 'N', ";
        break;
        case 2: // R-Requested
            updateString = updateString + "lotw_qsl_rcvd = 'R', ";
        break;
        case 3: // Q-Queued
            updateString = updateString + "lotw_qsl_rcvd = 'I', ";
            updateString = updateString + "lotw_qslrdate = '" + (lotwRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        case 4: // I-Ignore
            updateString = updateString + "lotw_qsl_rcvd = 'V', ";

            updateString = updateString + "lotw_qslrdate = '" + (lotwRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
        break;
        default: //N-No
            updateString = updateString + "lotw_qsl_rcvd = 'N', ";
        break;
    }
    // QSL SENT
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    i = qslSentComboBox->currentIndex();
    int ii = qslSentViaComboBox->currentIndex();

    switch (i)
    {
        case 0: // Y-Yes
            updateString = updateString + "qsl_sent = 'Y', ";
            updateString = updateString + "qslsdate = '" + (qslSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";

            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_sent_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_sent_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_sent_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
            }
        break;
        case 1: //N-No
            updateString = updateString + "qsl_sent = 'N', ";
            updateString = updateString + "qsl_sent_via = 'B', ";
        break;
        case 2: // R-Requested
            updateString = updateString + "qsl_sent = 'R', ";
            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_sent_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_sent_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_sent_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
            }
        break;
        case 3: // Q-Queued
            updateString = updateString + "qsl_sent = 'Q', ";
            updateString = updateString + "qslsdate = '" + (qslSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_sent_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_sent_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_sent_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
            }
        break;
        case 4: // I-Ignore
            updateString = updateString + "qsl_sent = 'I', ";
            updateString = updateString + "qslsdate = '" + (qslSentQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_sent_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_sent_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_sent_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_sent_via = 'B', ";
                break;
            }
        break;
        default: //N-No
            updateString = updateString + "qsl_sent = 'N', ";
            updateString = updateString + "qsl_sent_via = 'B', ";
        break;
    }

     // QSL RECEPTION
    i = qslRecComboBox->currentIndex();
    ii = qslRecViaComboBox->currentIndex();

    switch (i)
    { //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Validated");
        case 0: // Y-Yes
        //QSL received date: {Y, N, R, I, V}
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'Y', ";
            updateString = updateString + "qslrdate = '" + (qslRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
            //updateString = updateString + "confirmed = '1', ";

            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_rcvd_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_rcvd_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_rcvd_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
            }
        break;
        case 1: //N-No
            updateString = updateString + "qsl_rcvd = 'N', ";
            updateString = updateString + "qsl_rcvd_via = 'B', ";
            //updateString = updateString + "confirmed = '0', ";
        break;
        case 2: // R-Requested
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'R', ";
            //updateString = updateString + "confirmed = '0', ";

            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_rcvd_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_rcvd_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_rcvd_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
            }
        break;
        case 3: // I-Ignore
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'I', ";
            //updateString = updateString + "confirmed = '0', ";

            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_rcvd_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_rcvd_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_rcvd_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
            }
        break;
        case 4: // V-Verified
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'V', ";
            updateString = updateString + "qslrdate = '" + (qslRecQDateEdit->date()).toString("yyyy/MM/dd") + "', ";
            //updateString = updateString + "confirmed = '1', ";

            switch (ii)
            {
                case 0: //B
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
                case 1: //D
                    updateString = updateString + "qsl_rcvd_via = 'D', ";
                break;
                case 2: //E
                    updateString = updateString + "qsl_rcvd_via = 'E', ";
                break;
                case 3: //M
                    updateString = updateString + "qsl_rcvd_via = 'M', ";
                break;
                default:
                    updateString = updateString + "qsl_rcvd_via = 'B', ";
                break;
            }
        break;
        default: //N-No
            updateString = updateString + "qsl_rcvd = 'N', ";
            updateString = updateString + "qsl_rcvd_via = 'B', ";
            //updateString = updateString + "confirmed = '0', ";

        break;
    }
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


void MainWindow::createlogPanel()
{
   // //qDebug() << "MainWindow::createlogPanel: "  << endl;
    logView->setModel(logModel);
    QString stringQuery;
    stringQuery = QString("SELECT * FROM log LIMIT 1");
    //stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, rst_sent, stx, rst_rcvd, points, multiplier FROM log WHERE lognumber='%1'").arg(currentLog);
    QSqlQuery query;
    query.exec(stringQuery);
    QSqlRecord rec;
    rec = query.record(); // Number of columns
    int columns = rec.count();


    for (int i=0; i < columns; i++)
    {
        logView->setColumnHidden(i, true);
    }

    QVBoxLayout *layout = new QVBoxLayout;

    switch (contestMode) {

        case CQ_WW_SSB:
            //stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, rst_sent, stx, srx, rst_rcvd, points, multiplier FROM log WHERE lognumber='%1'").arg(currentLog);
            //query.exec(stringQuery);
            //rec = query.record(); // Number of columns
            //columns = rec.count();

            logLabel = new QLabel(tr("Log"));
            logLabel->setBuddy(logView);

            layout->addWidget(logLabel);
            layout->addWidget(logView);
            logPanel->setLayout(layout);

            logPanel->addAction(loggWinAct);
            logPanel->addAction(scoreeWinAct);

            columns = rec.indexOf("call");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("qso_date");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("time_on");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("bandid");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("modeid");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("rst_sent");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("stx");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("srx");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("rst_rcvd");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("points");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("multiplier");
            logView->setColumnHidden(columns, false);

        break;
        case CQ_WW_CW:

        break;
        default:
            //stringQuery = QString("SELECT id, qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, notes FROM log WHERE lognumber='%1'").arg(currentLog);
            //query.exec(stringQuery);
            //query.next();
            //rec = query.record(); // Number of columns
            //columns = rec.count();
/*
            for (int i=0;i<columns; i++)
            {
                logView->setColumnHidden(i, true);
            }
*/
            columns = rec.indexOf("qso_date");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("time_on");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("call");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("rst_sent");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("rst_rcvd");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("bandid");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("modeid");
            logView->setColumnHidden(columns, false);
            columns = rec.indexOf("comment");
            logView->setColumnHidden(columns, false);

        break;
    }

   //logView = new QTableView;

    logView->setItemDelegate(new QSqlRelationalDelegate(this));
    //logView->setItemDelegate(new QSqlRelationalDelegate(logView));
    logView->setSelectionMode( QAbstractItemView::SingleSelection);
    logView->setSelectionBehavior(QAbstractItemView::SelectRows);
    logView->resizeColumnsToContents();
    logView->horizontalHeader()->setStretchLastSection(true);

}



void MainWindow::createSearchResultsPanel()
{

    searchBoxClearButton->setToolTip(tr("Clear the searchs"));
    searchBoxExportButton->setToolTip(tr("Export the search result to an ADIF file"));
    searchBoxSelectAllButton->setToolTip(tr("Select/Unselect all the QSO of the box"));
    searchBoxReSearchButton->setToolTip(tr("Search in the log"));

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


void MainWindow::createScorePanel()
{


    QVBoxLayout *scoreLayout = new QVBoxLayout;
    scoreLayout->addWidget(scoreTextEdit);
    scoreTextEdit->setPlainText("Test TEXT");

    scoreWindow->setLayout(scoreLayout);

}


void MainWindow::createlogModel()
{
/*
    Log_Id = 0,
    Log_Name = 1,
    Log_BandId = 2,
    Log_ModeId = 3,
    Log_DateId = 4,
    Log_TimeId = 5

setRelation ( int column, const QSqlRelation & relation )

    model->setTable("employee");
    model->setRelation(2, QSqlRelation("city", "id", "name"));

The setRelation() call specifies that column 2 in table employee
is a foreign key that maps with field id of table city, and that
the view should present the city's name field to the user.

*/
    QString stringQuery = QString("SELECT * from log LIMIT 1");
    QSqlQuery q; //(stringQuery);
    QSqlRecord rec; // = q.record();

    int nameCol;

    //stringQuery = QString("SELECT id, qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, notes FROM log WHERE lognumber='%1'").arg(currentLog);
    //stringQuery = QString(stringQuery);
    q.exec(stringQuery);
    q.next();
    rec = q.record(); // Number of columns
    //int columns = rec.count();


    logModel = new QSqlRelationalTableModel(this);
    logModel->setTable("log");



    switch (contestMode) {

        case CQ_WW_SSB:


        break;
        case CQ_WW_CW:

        break;
        default:


            nameCol = rec.indexOf("bandid");
            logModel->setRelation(nameCol, QSqlRelation("band", "id", "name"));
            nameCol = rec.indexOf("modeid");
            logModel->setRelation(nameCol, QSqlRelation("mode", "id", "name"));

            nameCol = rec.indexOf("id");
            logModel->setSort(nameCol, Qt::AscendingOrder);

            nameCol = rec.indexOf("qso_date");
            logModel->setHeaderData(nameCol, Qt::Horizontal, tr("Date"));

            nameCol = rec.indexOf("time_on");
            logModel->setHeaderData(nameCol, Qt::Horizontal, tr("Time"));

            nameCol = rec.indexOf("call");
            logModel->setHeaderData(nameCol, Qt::Horizontal,tr("QRZ"));

            nameCol = rec.indexOf("bandid");
            logModel->setHeaderData(nameCol, Qt::Horizontal, tr("Band"));

            nameCol = rec.indexOf("modeid");
            logModel->setHeaderData(nameCol, Qt::Horizontal, tr("Mode"));

            nameCol = rec.indexOf("rst_sent");
            logModel->setHeaderData(nameCol, Qt::Horizontal, tr("RSTtx"));

            nameCol = rec.indexOf("rst_rcvd");
            logModel->setHeaderData(nameCol, Qt::Horizontal, tr("RSTrx"));

            nameCol = rec.indexOf("comment");
            logModel->setHeaderData(nameCol, Qt::Horizontal, tr("Comment"));

        break;
    }


logModel->select();

}



void MainWindow::createUICQWW()
{
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
    modeComboBox->addItems(modes);

    qrzLineEdit->setToolTip(tr("QRZ of the QSO"));
    rstTXLineEdit->setToolTip(tr("TX RST"));
    rstRXLineEdit->setToolTip(tr("RX RST"));
    STXLineEdit->setToolTip(tr("TX Exchange"));
    SRXLineEdit->setToolTip(tr("RX Exchange"));
    bandComboBox->setToolTip(tr("Band of the QSO"));
    modeComboBox->setToolTip(tr("Mode of the QSO"));
    dateEdit->setToolTip(tr("Date of the QSO"));
    timeEdit->setToolTip(tr("Time of the QSO"));
    statusBar->setToolTip(tr("Misc information"));
    qsoStatusBar->setToolTip(tr("QSO information"));
    OKButton->setToolTip(tr("Add the QSO to the log"));
    spotItButton->setToolTip(tr("Spots this QSO to the DX Cluster"));
    clearButton->setToolTip(tr("Clears the box"));

    gridGroupBox = new QGroupBox(tr("Input"));
    QGridLayout *layout = new QGridLayout;

    updateStatusBar(tr("Ready"));
    updateQSOStatusBar(tr("Ready"));

    rstTXLineEdit->setInputMask("990");
    rstRXLineEdit->setInputMask("990");
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

    QHBoxLayout *statusBarLayout = new QHBoxLayout;
    statusBarLayout->addWidget(statusBar);
    statusBarLayout->addWidget(qsoStatusBar);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(OKButton);
    buttonsLayout->addWidget(spotItButton);
    buttonsLayout->addWidget(clearButton);

    QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("yyyy/MM/dd");
    timeEdit->setDisplayFormat("HH:mm:ss");

    layout->addWidget(qrzgroupBox, 1, 0);
    layout->addLayout(RSTLayout, 1, 1);
    layout->addLayout(TimeLayout, 2, 0);
    layout->addLayout(BandModeLayout, 2, 1);
    layout->addLayout(buttonsLayout,3, 1);
    layout->addLayout(statusBarLayout, 4, 0, 2 , -1);

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

// Return pressed = QSO ENTRY
    connect(qrzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(SRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(STXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstTXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(operatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(stationCallSignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );    
    connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(myLocatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
    connect(myLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotMyLocatorTextChanged() ) );


//connect(bandComboBox, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
//connect(dateEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
//connect(timeEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

//Actions to pass the focus between QRZ / SRX
    connect(qrzLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZTextChanged() ) );
    connect(SRXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSRXTextChanged() ) );
    connect(STXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSTXTextChanged() ) );
    connect(rstTXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstTXTextChanged() ) );
    connect(rstRXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstRXTextChanged() ) );

    connect(qslViaLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQSLViaTextChanged() ) );

    connect(bandComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandComboBoxChanged() ) ) ;
    connect(modeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotModeComboBoxChanged() ) ) ;


//Buttons Actions
    connect(OKButton, SIGNAL(clicked()), this, SLOT(slotOKButtonClicked() ) );
    connect(spotItButton, SIGNAL(clicked()), this, SLOT(slotSpotItButtonClicked() ) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked() ) );

//LOG VIEW
    connect(logView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonFromLog( const QPoint& ) ) );
    connect(logView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickLog( const QModelIndex& ) ) );


// SEARCH BOX VIEW

    connect(searchBoxLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSearchBoxTextChanged() ) );

    connect(searchResultsTreeWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonSearch( const QPoint& ) ) );
    connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    connect(searchResultsTreeWidget, SIGNAL(itemSelectionChanged( ) ), this, SLOT(slotSearchBoxSelectionChanged( ) ) );

    connect(searchBoxExportButton, SIGNAL(clicked()), this, SLOT(slotSearchExportButtonClicked() ) );
    connect(searchBoxClearButton, SIGNAL(clicked()), this, SLOT(slotSearchClearButtonClicked() ) );
    connect(searchBoxSelectAllButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxSelectAllButtonClicked() ) );
    connect(searchBoxReSearchButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxReSearchButtonClicked() ) );


    connect(recalculateAwardsButton, SIGNAL(clicked()), this, SLOT(slotRecalculateAwardsButtonClicked() ) );

//connect(searchResultsTreeWidget, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickSearch( const QModelIndex& ) ) );

//CLUSTER
//void clusterSpotToLog(const QStringList _qs);
//SIGNAL dxspotclicked(const QStringList _qs)
    connect(dxClusterWidget, SIGNAL(dxspotclicked(QStringList)), this, SLOT(slotAnalyzeDxClusterSignal(QStringList) ) );


}

void MainWindow::slotRecalculateAwardsButtonClicked()
{
    //qDebug() << "MainWindow::recalculateAwardsButtonClicked: " << endl;
    awards->recalculateAwards();

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

    connect(iotaContinentComboBox, SIGNAL(activated ( int)), this, SLOT(slotIOTAComboBoxChanged() ) )  ;

    //QSL Actions
    connect(qslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLSentComboBoxChanged() ) )  ;
    connect(qslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLRecvComboBoxChanged() ) ) ;

    connect(eqslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLSentComboBoxChanged() ) )  ;
    connect(eqslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLRecvComboBoxChanged() ) ) ;
}

bool MainWindow::checkContest(){
//qDebug() << "MainWindow::checkContest: " << QString::number(contestMode) << endl;
//contestNames << "No-Contest" <<"CQ-WW-DX-SSB" << "CQ-WW-DX-CW" << "CQ-WPX-SSB" << "CQ-WPX-CW";
    QStringList qs;
    qs.clear();
    QString qsoStatus, aux;
    int currentEntity = world->getQRZARRLId(currentQrz);
    int tband = 1 + bandComboBox->currentIndex();
    //int tmode = 1 + modeComboBox->currentIndex();

    switch (contestMode) {

        case CQ_WW_SSB:
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



        break;
        case CQ_WW_CW:
            //qDebug() << "MainWindow::checkContest: CQ-WW-CW:" << endl;
        break;
        default:
            //qDebug() << "MainWindow::checkContest: Default:" << endl;
        break;

    }



    updateQSOStatusBar(qsoStatus);

    return false;
}

void MainWindow::slotQSLViaTextChanged()
{
    //qDebug() << "MainWindow::slotQSLViaTextChanged: " << qslViaLineEdit->text() << " / Length: " << QString::number((qslViaLineEdit->text()).size()) << endl;
    qslViaLineEdit->setText((qslViaLineEdit->text()).toUpper());
}

bool MainWindow::validCharactersInCall(const QString _qrz)
{
    for (int i = 0; i<_qrz.size();i++)
    {
        if (!( ((_qrz.at(i)).isLetterOrNumber()) || (_qrz.at(i)=='\\')  || (_qrz.at(i)=='/') ))
        {
            return false;
        }

    }

    return true;

}
void MainWindow::slotQRZTextChanged()
{    
    //qDebug() << "MainWindow::slotQRZTextChanged: " << qrzLineEdit->text() << " / Length: " << QString::number((qrzLineEdit->text()).size()) << endl;
    int cursorP = qrzLineEdit->cursorPosition();
    infoLabel1->clear();

    qrzLineEdit->setText(((qrzLineEdit->text())).simplified());
    qrzLineEdit->setText((qrzLineEdit->text()).toUpper());

    if (!validCharactersInCall(qrzLineEdit->text()))
    {
        infoLabel1->setText(tr("Not valid characters in the QRZ box"));
        InValidCharsInPrevCall = true;
        return;
    }

    world->checkQRZValidFormat(qrzLineEdit->text());


    if (((qrzLineEdit->text()).length() < 1))
    { // If QRZ box is blank, Information labels should be cleared.
        infoLabel1->clear();
        infoLabel2->clear();
        slotClearButtonClicked();
        return;
    }
    if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
        //qDebug() << "MainWindow::slotQRZTextChanged: MODIFY or Lenght < 1" << endl;
        qrzSmallModDontCalculate=false;
        return;
    }
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    int i;    
    int dx_CQz = -1;
    int dx_ITUz = -1;


    currentQrz = qrzLineEdit->text();

    //TODO: <DELETE>
   // QString aaaa;
    //aaaa = world->getQRZEntityPrefixes(currentQrz);
    //qDebug() << "MainWindow::slotQRZTextChanged: - ALL PREFIXES" << aaaa << endl;
    //TODO: </DELETE>

    if ((currentQrz).count('\\')){ // Replaces \ by / to ease operation.
        currentQrz.replace(QChar('\\'), QChar('/'));
        qrzLineEdit->setText(currentQrz);
    }

    currentQrz = qrzLineEdit->text();
    if ((currentQrz.at(cursorP-1)).isSpace())
    {
        previousQrz = currentQrz.remove(cursorP-1, 1);
        cursorP--;
        qrzLineEdit->setText(previousQrz);
    }

    if ( currentQrz.endsWith(' ') )
    {/*Remove the space and moves the focus to SRX to write the RX exchange*/

        previousQrz = currentQrz.simplified();
        qrzLineEdit->setText(previousQrz);
        SRXLineEdit->setFocus();
    }

    currentQrz = qrzLineEdit->text();
    currentEntity = world->getQRZARRLId(currentQrz);
    dx_CQz = world->getEntityCqz(currentEntity);
    dx_ITUz = world->getEntityItuz(currentEntity);

    if (my_CQz <= 0)
    {
        my_CQz = world->getEntityCqz(currentEntity);
        STXLineEdit->setText(QString::number(my_CQz));  // My Own CQZ

    }

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << currentQrz << QString::number(currentBand) << QString::number(currentMode) << QString::number(currentLog);

    //showStatusOfDXCC(_qs);

   // NOW ONLY SPECIFIC ACTIONS DEPENDING ON THE RUNNING MODE

    switch (contestMode) {

        case CQ_WW_SSB:
            //qDebug() << "MainWindow::slotQRZTextChanged: CQ-WW-SSB:" << endl;
            STXLineEdit->setText(QString::number(my_CQz));  // My Own CQZ


            if (dx_CQz > 0)
            {
                //if(QString::number(world->getQRZCqz(qrzLineEdit->text())) > 0 ){
                SRXLineEdit->setText(QString::number(dx_CQz));
            }else{
                //qDebug() << "MainWindow::checkContest  CQZ < 0"<< endl;
            }

            i = currentEntity;

            if (i>0){
                updateStatusBar(world->getEntityName(currentEntity) + "  -  CQ: " + QString::number(dx_CQz) + "  -  ITU: " + QString::number(dx_ITUz));
            }
            else
            {
                updateStatusBar(tr("Ready..."));
            }

            i = checkIfWorkedB4(currentQrz);
            checkContest();

        break;
        case CQ_WW_CW:
            //qDebug() << "MainWindow::slotQRZTextChanged: CQ-WW-CW:" << endl;
        break;
        default:
            //qDebug() << "MainWindow::slotQRZTextChanged: Default:" << endl;
            //qDebug() << "MainWindow::slotQRZTextChanged: - current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) << endl;
        if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) )
            {
                previousEntity = currentEntity;
                InValidCharsInPrevCall = false;
                showEntityInfo(currentEntity);
                showStatusOfDXCC(_qs);
                i = (world->getContinentNumber(currentEntity));
                        ;
                if (  i > 0 )
                {
                    iotaContinentComboBox->setCurrentIndex( i - 1 );
                }
                else
                {
                    iotaContinentComboBox->setCurrentIndex( 0 );
                }
            }
        break;
    }
    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
    qrzLineEdit->setCursorPosition(cursorP);
    //qDebug() << "MainWindow::slotQRZTextChanged: END" << endl;
}



void MainWindow::slotSearchBoxReSearchButtonClicked()
{
    slotSearchBoxTextChanged();
}

void MainWindow::slotSearchClearButtonClicked()
{
    //qDebug() << "MainWindow::slotSearchClearButtonClicked: " << endl;
    searchResultsTreeWidget->clear();
    searchBoxLineEdit->clear();
    searchSelectAllClicked = false;
    qslingNeeded = false;
}

void MainWindow::slotSearchBoxSelectAllButtonClicked()
{
    //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: " << endl;
    if (searchSelectAllClicked)
    {
        //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: UN-SELECTING" << endl;
        searchSelectAllClicked = false;
        searchResultsTreeWidget->clearSelection();
        searchBoxSelectAllButton->setText(tr("&Select All"));


    }
    else
    {
        //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: SELECTING" << endl;
        searchSelectAllClicked = true;
        searchResultsTreeWidget->selectAll();
        searchBoxSelectAllButton->setText(tr("&Clear selection"));

    }

}

void MainWindow::slotSearchExportButtonClicked()
{
    //qDebug() << "MainWindow::slotSearchExportButtonClicked: " << endl;
    // MARK QSOs
    // SAVE MARKED QSOs TO FILE
    // UNMARK QSOs

    int _qsoId = 0;
    int i = 0;
    QString stringQuery;
    QSqlQuery query;
    QTreeWidgetItem *item = searchResultsTreeWidget->topLevelItem(i);


    while (i <= searchResultsTreeWidget->topLevelItemCount() )
    {
        item = searchResultsTreeWidget->topLevelItem(i);
        if (item == 0)
        {
            i = searchResultsTreeWidget->topLevelItemCount() + 1;
            //qDebug() << "MainWindow::slotSearchExportButtonClicked: ITEM = 0" << endl;
        }
        else
        {
            if (stationCallSignShownInSearch)
            {
                _qsoId = ((item)->text(7)).toInt();
            }
            else
            {
                _qsoId = ((item)->text(6)).toInt();
            }

            if ((item)->isSelected())
            {

                stringQuery = QString("UPDATE log SET marked = 'X' WHERE id='%1'").arg(_qsoId);
                query.exec(stringQuery);
                //TODO: Prepare this while/query execution
                i++;
                //qDebug() << "MainWindow::slotSearchExportButtonClicked: ITEM MARKED: " << QString::number(_qsoId) << endl;

            }
            else
            {

                stringQuery = QString("UPDATE log SET marked = 'N' WHERE id='%1'").arg(_qsoId);
                query.exec(stringQuery);
                //TODO: Prepare this while/query execution
                i++;
                //qDebug() << "MainWindow::slotSearchExportButtonClicked: ITEM NOT MARKED: " << QString::number(_qsoId) << endl;
            }
        }
    }

    //qDebug() << "MainWindow::slotSearchExportButtonClicked: to Ask filename" << endl;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               kontestDir,
                               tr("ADIF (*.adi *.adif)"));

    //qDebug() << "MainWindow::slotSearchExportButtonClicked: to call save file" << endl;
    filemanager->adifLogExportMarked(fileName);
    //qDebug() << "MainWindow::slotSearchExportButtonClicked: unmarking..." << endl;
    //db->unMarkAllQSO();

    //RECORRER EL CUADRO, MARCANDO TODOS LOS QSO EN EL LOG. DESPUES LLAMAR AL METODO QUE LO EXPORTA A UN
      //      FICHERO
        //    DESMARCAR.

/*
    QTreeWidgetItem *item = searchResultsTreeWidget->itemAt(pos);
    int _qsoID;

    if (item)
    {
        // 6 is the column in the searchResultsTreeWidget where the id is saved
        _qsoID = ((item)->text(6)).toInt();


    QString stringQuery = QString("UPDATE log SET marked = 'N' WHERE 1");
    QSqlQuery query(stringQuery);
    //qDebug() << "MainWindow::slotQSLSentViaBureauFromLog: " << stringQuery << endl;
    query.exec(stringQuery);
*/


}


void MainWindow::slotSearchBoxTextChanged()
{
    //qDebug() << "MainWindow::slotSearchBoxTextChanged: "  << searchBoxLineEdit->text() << endl;
    //int nameCol;
    QString _id, _call, _dateTime, _band, _mode, _qsltx, _qslrx, _freq, _stationcallsign;
    QStringList q;
    int cursorP = searchBoxLineEdit->cursorPosition();

    searchBoxLineEdit->setText((searchBoxLineEdit->text()).toUpper());

    if ((searchBoxLineEdit->text()).length() < 2)
    {
        searchResultsTreeWidget->clear();
        return;
    }

    qslingNeeded = false; // If I am searching I am not longer looking for QSO to QSL

    QString theCall = searchBoxLineEdit->text();

    searchResultsTreeWidget->clear();

    QSqlQuery query;
    QString queryString, aux;

    if (false)
    {
       // QChar lastChar = ;
        aux = theCall.left(theCall.length()-1);
        aux = aux.append(QChar((theCall.at(theCall.length()-1)).unicode()+1));
        queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call >= '%1' AND call < '%2' AND lognumber='%3'").arg(theCall).arg(aux).arg(currentLog);
        //qDebug() << "MainWindow::slotSearchBoxTextChanged: QS1: " << queryString << endl;
    }
    else
    {
      queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call LIKE '%%1%' AND lognumber='%2'").arg(theCall).arg(currentLog);
      //qDebug() << "MainWindow::slotSearchBoxTextChanged: QS2: " << queryString << endl;
    }



    //qDebug() << "MainWindow::slotSearchBoxTextChanged: queryString"  << queryString << endl;
    query.exec(queryString);
    //qDebug() << "MainWindow::slotSearchBoxTextChanged: queryString EXECUTED!"  << endl;
    QColor color;

    QSqlRecord rec = query.record();
    QFont font;
    font.setBold(true);

    while (query.next())
    {
        if (query.isValid())
        {


            //nameCol = rec.indexOf("call");
            //_call= (query.value(nameCol)).toString();
            _call= (query.value(0)).toString();

            //nameCol = rec.indexOf("qso_date");
            //_dateTime = (query.value(nameCol)).toString();
            _dateTime = (query.value(1)).toString();
            //nameCol = rec.indexOf("time_on");
            //_dateTime = _dateTime + "-" +(query.value(nameCol)).toString();
            _dateTime = _dateTime + "-" +(query.value(2)).toString();

            //nameCol = rec.indexOf("bandid");
            //_freq = (query.value(nameCol)).toString();
            _freq = (query.value(3)).toString();
            _band = db->getBandNameFromID2( _freq.toInt() );

            _mode = query.value(4).toString();
            //qDebug() << "MainWindow::slotSearchBoxTextChanged: mode: :  " << _mode << endl;

            //nameCol = rec.indexOf("qsl_sent");
            //_qsltx = (query.value(nameCol)).toString();
            _qsltx = (query.value(5)).toString();
            if (_qsltx.length()<1)
            {
                _qsltx = "N";
            }

            //nameCol = rec.indexOf("qsl_rcvd");
            //_qslrx = (query.value(nameCol)).toString();
            _qslrx = (query.value(6)).toString();
            if (_qslrx.length()<1)
            {
                _qslrx = "N";
            }
            if (stationCallSignShownInSearch)
            {
                _stationcallsign = (query.value(7)).toString();
                if (_stationcallsign.length()<3)
                {
                    _stationcallsign = stationQRZ;
                }
                _id = (query.value(8)).toString();
            }
            else
            {
                _id = (query.value(8)).toString();
            }




                q.clear();
                q << _call << _freq << _mode << QString::number(currentLog);
                //QColor color = Qt::red;

                //color = Qt::red;
                //TODO: Optimize the awards->getQRZDXStatusColor because is TOO slow
                color =  awards->getQRZDXStatusColor(q);
                _mode = db->getModeNameFromNumber(_mode.toInt());
/*
    awards.getQRZDXStatusColor(const QStringList _qs);
    // Receives:  QStringList _qs;
    //_qs << QRZ << BandId << lognumber;
*/
                QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);

                item->setText(0, _call);
                item->setFont(0, font);
                item->setText(1, _dateTime);
                item->setText(2, _band);
                item->setText(3, _mode);
                item->setText(4, _qslrx);
                item->setText(5, _qsltx);
                if (stationCallSignShownInSearch)
                {
                    item->setText(6, _stationcallsign);
                    item->setText(7, _id);

                }
                else
                {
                    item->setText(6, _id);
                }


            item->setForeground(0, QBrush(color));
        } // Closes if next.isValid
    } // Closes While
    (searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);
/*
    if (((theCall.at(cursorP-1)).isSpace()) && (cursorP>1))
    {
        searchBoxLineEdit->setText(theCall.remove(cursorP-1, 1));
        cursorP--;
    }
    */

    searchBoxLineEdit->setCursorPosition(cursorP);


}

void MainWindow::slotSearchBoxSelectionChanged()
{// Detects when a selection has been done in the search box and changes
 // The button to clear selection
    //qDebug() << "MainWindow::slotSearchBoxSelectionChanged: "  << endl;

    if ((searchResultsTreeWidget->selectedItems()).count() > 0 )
    {
        searchBoxSelectAllButton->setText(tr("&Clear selection"));
        searchSelectAllClicked = true;


    }
    else
    {
        searchBoxSelectAllButton->setText(tr("&Select All"));
        searchSelectAllClicked = false;

    }



/*

    if (searchSelectAllClicked)
    {
        //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: UN-SELECTING" << endl;
        searchSelectAllClicked = false;
        searchResultsTreeWidget->clearSelection();
        searchBoxSelectAllButton->setText(tr("&Select All"));


    }
    else
    {
        //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: SELECTING" << endl;
        searchSelectAllClicked = true;
        searchResultsTreeWidget->selectAll();
        searchBoxSelectAllButton->setText(tr("&Clear selection"));

    }

*/

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

}

void MainWindow::slotClearButtonClicked()
{
    //qDebug() << "MainWindow::slotClearButtonClicked" << endl;
    modify = false;
    OKButton->setText(tr("&Add"));
    modifyingQSO = -1;
    qrzLineEdit->clear();
    qrzLineEdit->setFocus(Qt::OtherFocusReason);

    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    qthLineEdit->clear();

    bandComboBox->setCurrentIndex(currentBandShown);
    modeComboBox->setCurrentIndex(defaultMode);

    qsoPoints = 0;
    qsoMultiplier = 0;

    switch (contestMode) {

        case CQ_WW_SSB:
            SRXLineEdit->clear();
            STXLineEdit->setText( QString::number( world->getQRZCqz(stationQRZ) ) );

            qrzgroupBox->setTitle(tr("QRZ"));

        break;
        case CQ_WW_CW:
            SRXLineEdit->clear();
            STXLineEdit->setText( QString::number( world->getQRZCqz(stationQRZ) ) );
            qrzgroupBox->setTitle(tr("QRZ"));

        break;
        default:
            SRXLineEdit->setText("59");
            STXLineEdit->setText("59");

            nameLineEdit->clear();
            locatorLineEdit->clear();
            txFreqSpinBox->setValue(0);
            rxFreqSpinBox->setValue(0);
            //freqQLCDNumber->display(0);
            //notesTextEdit->clear();
            commentLineEdit->clear();
            infoLabel1->clear();
            infoLabel2->clear();
            stationCallSignLineEdit->setText(stationQRZ);

            operatorLineEdit->setText(operatorQRZ);

            myLocatorLineEdit->setText(myLocator);
            myPowerSpinBox->setValue(myPower);
            rxPowerSpinBox->setValue(0);
            qslSentViaComboBox->setCurrentIndex(0); // has to be changed before the qslSentComboBox to avoid calling the slot
            qslRecViaComboBox->setCurrentIndex(0); // has to be changed before the qslRecComboBox to avoid calling the slot

            qslSentComboBox->setCurrentIndex(0);    // has to be changed before the qslSentComboBox to avoid calling the slot
            qslRecComboBox->setCurrentIndex(1); // Not received
            qslSentComboBox->setCurrentIndex(1); // Not sent
            eqslSentComboBox->setCurrentIndex(1);
            eqslRecComboBox->setCurrentIndex(1);
            lotwSentComboBox->setCurrentIndex(1);
            lotwRecComboBox->setCurrentIndex(1);
            qslmsgTextEdit->clear();
            qslViaLineEdit->clear();

            iotaContinentComboBox->setCurrentIndex(0);
            //iotaNumberLineEdit->setEnabled(false);
            iotaNumberLineEdit->setText("000");
            continentLabel->setText("");
            prefixLabel->setText("");
            cqzLabel->setText("0");
            ituzLabel->setText("0");
            clearInfoFromLocators();
            clearBandLabels();

            showAwards();
        break;
    }
}

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
void MainWindow::slotUpdateTime()
{
   // //qDebug() << "MainWindow::slotUpdateTime: " << (dateTime->currentDateTime()).toString("yyyy-MM-dd - hh:mm:ss") << endl;
//    ((dateTime->currentDateTimeUtc()).date()).toString()
    //(dateTime->currentDateTime()).date()
    dateTime->currentDateTime();

    if ( (!modify) && (realTime)  )
    {

        //dateTime->currentDateTime();

        dateEdit->setDate((dateTime->currentDateTime()).date());


        if (UTCTime)
        {
            timeEdit->setTime((dateTime->currentDateTime().toUTC()).time());
        }
        else
        {
            timeEdit->setTime((dateTime->currentDateTime()).time());
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //qDebug() << "MainWindow::closeEvent" << endl;

    if (maybeSave())
    {
        db->unMarkAllQSO();
        //slotFileClose();
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


    saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
    saveAct->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    fileMenu->addSeparator();

    printLogAct = new QAction(tr("&Print Log..."), this);
    fileMenu->addAction(printLogAct);
    printLogAct->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(printLogAct, SIGNAL(triggered()), this, SLOT(slotFilePrint()));

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    exitAct->setMenuRole(QAction::QuitRole);
    exitAct->setShortcut(Qt::CTRL + Qt::Key_X);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    toolMenu = menuBar()->addMenu(tr("&Tools"));


    ADIFExport = new QAction(tr("&Export to ADIF..."), this);
    toolMenu->addAction(ADIFExport);
    //ADIFExport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFExport, SIGNAL(triggered()), this, SLOT(slotADIFExport()));

    ADIFImport = new QAction(tr("&Import from ADIF..."), this);
    toolMenu->addAction(ADIFImport);
    //ADIFImport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFImport, SIGNAL(triggered()), this, SLOT(slotADIFImport()));

    toolMenu->addSeparator();

    fillQsoAct = new QAction(tr("Fill QSO data"), this);
    toolMenu->addAction(fillQsoAct);
    //fillQsoAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(fillQsoAct, SIGNAL(triggered()), this, SLOT(fillQSOData()));

    toolMenu->addSeparator();

    findQSO2QSLAct = new QAction(tr("&Find QSO to QSL"), this);
    toolMenu->addAction(findQSO2QSLAct);
    //findQSO2QSLAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(findQSO2QSLAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLToSend()));

    findRequestedQSLAct = new QAction(tr("Find &requested QSL"), this);
    toolMenu->addAction(findRequestedQSLAct);
    //findQSO2QSLAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(findRequestedQSLAct, SIGNAL(triggered()), this, SLOT(slotToolSearchRequestedQSLToSend()));

    ReqQSLExport = new QAction(tr("&Export Requested QSL to ADIF..."), this);
    toolMenu->addAction(ReqQSLExport);
    connect(ReqQSLExport, SIGNAL(triggered()), this, SLOT(slotRQSLExport()));

    toolMenu->addSeparator();

    downloadCTYAct = new QAction (tr("&Update CTY.CSV"), this);
    toolMenu->addAction(downloadCTYAct);
    //downloadCTYAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(downloadCTYAct, SIGNAL(triggered()), this, SLOT(slotUpdateCTYDAT()));


    toolMenu->addSeparator();

    //removeAct = new QAction(tr("&Remove Entry"), this);
    //removeAct->setEnabled(false);
    //toolMenu->addAction(removeAct);
    //connect(removeAct, SIGNAL(triggered()), addressWidget, SLOT(removeEntry()));
    //connect(addressWidget, SIGNAL(selectionChanged(QItemSelection)), this, SLOT(updateActions(QItemSelection)));

    //viewMenu = menuBar()->addMenu(tr("&View"));


    setupMenu = menuBar()->addMenu(tr("&Setup"));

    setupAct = new QAction(tr("&Setup..."), this);
    setupMenu->addAction(setupAct);
    setupAct->setMenuRole(QAction::PreferencesRole);
    connect(setupAct, SIGNAL(triggered()), this, SLOT(slotSetup()));

    //TODO: To be added once the help dialog has been implemented
    helpMenu = menuBar()->addMenu(tr("&Help"));

    helpAct = new QAction(tr("&Help..."), this);
    helpMenu->addAction(helpAct);
    connect(helpAct, SIGNAL(triggered()), this, SLOT(slotHelpHelpAction()));

    aboutAct = new QAction(tr("&About..."), this);
    helpMenu->addAction(aboutAct);
    aboutAct->setMenuRole(QAction::AboutRole);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotHelpAboutAction()));
    
    aboutQtAct = new QAction(tr("About Qt..."), this);
    helpMenu->addAction(aboutQtAct);
    aboutQtAct->setMenuRole(QAction::AboutRole);
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(slotAboutQt()));
    
    
 }

void MainWindow::slotAboutQt()
{
   QMessageBox::aboutQt(this,tr("About..."));
   
}
   
void MainWindow::slotHelpHelpAction()
{
    //qDebug() << "MainWindow::slotHelpHelpAction"  << endl;

    helpHelpDialog->exec();

}

void MainWindow::slotHelpAboutAction()
{
    //qDebug() << "MainWindow::slotHelpAboutAction "  << endl;
   // QMessageBox::about(this, tr("About KLog"),
    //            tr("<b>KLog</b> "
   //                "Find the last release at http://jaime.robles.es/klog."));



    helpAboutDialog->exec();
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

    if (!(logPanel->isVisible()) )
    {
        logWinAct->setChecked ( true );
        logPanel->show();

    }else
    {
        logWinAct->setChecked ( false );
        logPanel->hide();
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

    if (!needToEnd)
    {
    setupDialog->setData(configFileName, softwareVersion, _page, !configured);

    //SetupDialog setupDialog(configFileName, QString::number(softwareVersion), configured, _page);
    //qDebug() << "MainWindow::slotSetup - 02"  << endl;
    //return setupDialog.exec();
    //setupDialog.exec();
    setupDialog->exec();
    //qDebug() << "MainWindow::slotSetup - 03"  << endl;
    if (needToEnd)
    {
        //qDebug() << "MainWindow::slotSetup - 03.1"  << endl;
        return;
        //qDebug() << "MainWindow::slotSetup - 03.2"  << endl;
    }
    else
    {
        //qDebug() << "MainWindow::slotSetup - 03.3"  << endl;
        readConfigData();
        //qDebug() << "MainWindow::slotSetup - 03.4"  << endl;
    }


    //qDebug() << "MainWindow::slotSetup - 04"  << endl;

    if (configured){
        //contest->~Contest();
        //qDebug() << "MainWindow::slotSetup - 05"  << endl;
    }else{
        //qDebug() << "MainWindow::slotSetup - 06"  << endl;

    }
}

}

void MainWindow::openFile()
{

    if (dataProxy->clearLog())
    {
        logModel->select();
    }
    else
    {

    }
}

bool MainWindow::saveFile(const QString _fileName)
{
   //qDebug() << "MainWindow::saveFile: " << _fileName  << endl;


    QString fileName = _fileName;

    if (fileName.endsWith(".adi", Qt::CaseInsensitive))
    {
        //qDebug() << "MainWindow::saveFile: 1"  << endl;
        needToSave = !(filemanager->adifLogExport(fileName));
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
    QFileDialog dialog(this);

    QStringList filters;
    filters << "ADIF files (*.adi *.adif)"
            << "Cabrillo files (*.log)"
            << "Any files (*)";

//    dialog.setNameFilters(filters);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                        kontestDir,
                        tr("ADIF files (*.adi *.adif);;Cabrillo files (*.log);;Any file (*.*)"));

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

    slotSetup(6);
    if (dataProxy->clearLog())
    {
        points = 0;
        multipliers = 0;
        qsoPoints = 0;
        qsoMultiplier = 0;

        logModel->select();
        slotClearButtonClicked();
        searchResultsTreeWidget->clear();

    }
    else
    {
        //TODO: An error to create a new file has ocurred. Manage it!
    }


}

void MainWindow::updateStatusBar(const QString statusm)
{
    statusBarMessage = statusm;
    statusBar->showMessage(statusBarMessage);
}

void MainWindow::updateQSOStatusBar(const QString statusm)
{
    qsoStatusBar->showMessage(statusm);
}


bool MainWindow::readCtyFile()
{

    return false;
}

void MainWindow::slotDoubleClickLog(const QModelIndex & index)
{
    //qDebug() << "MainWindow::slotDoubleClickLog"  << endl;

   // QSqlQuery query;
    //QString queryString;
    int row = index.row();
    qsoToEdit((logModel->index(row, 0)).data(0).toInt());
}

void MainWindow::slotDoubleClickSearch(QTreeWidgetItem * item, int)
{
    //qDebug() << "MainWindow::slotDoubleClickSearch"  << endl;
    int number = -1;
    if (item){
        if (stationCallSignShownInSearch)
        {
            number = (item->text(7)).toInt();
        }
        else
        {
            number = (item->text(6)).toInt();
        }


        qsoToEdit(number);
    }
    else
    {}
}


void MainWindow::slotRighButtonSearch(const QPoint& pos)
{
    //qDebug() << "MainWindow::slotRighButtonSearch"  << endl;


    QTreeWidgetItem *item = searchResultsTreeWidget->itemAt(pos);
    int _qsoID = 0;

    if (item)
    {
        //qDebug() << "MainWindow::slotRighButtonSearch ITEM=true"  << endl;
        // 6 is the column in the searchResultsTreeWidget where the id is saved
        if (stationCallSignShownInSearch)
        {
            //qDebug() << "MainWindow::slotRighButtonSearch stationCallSignShownInSearch = true"  << endl;
            _qsoID = ((item)->text(7)).toInt();
            //qDebug() << "MainWindow::slotRighButtonSearch QSO1: " << QString::number(_qsoID)  << endl;
        }
        else
        {
            //qDebug() << "MainWindow::slotRighButtonSearch stationCallSignShownInSearch = false"  << endl;
            _qsoID = ((item)->text(6)).toInt();
            //qDebug() << "MainWindow::slotRighButtonSearch QSO2: " << QString::number(_qsoID)  << endl;
        }
        //qDebug() << "MainWindow::slotRighButtonSearch QSO: " << QString::number(_qsoID)  << endl;
        showMenuRightButtonSearchCreateActions();
        //qDebug() << "MainWindow::slotRighButtonSearch -05"   << endl;
        righButtonSearchMenu(_qsoID);
        //qDebug() << "MainWindow::slotRighButtonSearch -06"   << endl;
    }else
    {
        //qDebug() << "MainWindow::slotRighButtonSearch ITEM=false"  << endl;
        return;
    }

    //qDebug() << "MainWindow::slotRighButtonSearch: "  << QString::number(_qsoID) << endl;

}

void MainWindow::righButtonSearchMenu(const int trow)
{
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu:  " << QString::number(trow) << endl;

    bool qslReceived = isQSLReceived(trow);
    bool qslSent = isQSLSent(trow);

    QMenu menu(this);

    menu.addAction(delQSOFromSearchAct);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -03"  << endl;
    delQSOFromSearchAct->setData(trow);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -04"  << endl;

    menu.addAction(qsoToEditFromSearchAct);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -05"  << endl;
    qsoToEditFromSearchAct->setData(trow);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -06"  << endl;

    menu.addSeparator();
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -07"  << endl;
    if (qslSent)
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSLSent"  << endl;
    }
    else
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Sent"  << endl;
        QMenu *menuSentQsl = menu.addMenu(tr("QSL Send"));
        menuSentQsl->addAction(qslSentViaBureauFromSearchAct);
        menuSentQsl->addAction(qslSentViaDirectFromSearchAct);
        menuSentQsl->addAction(qslSentRequestedAct);
        qslSentViaBureauFromSearchAct->setData(trow);
        qslSentViaDirectFromSearchAct->setData(trow);
        qslSentRequestedAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -end qsl not sent"  << endl;

    }

    if (qslReceived)
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSLRec"  << endl;
    }
    else
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec"  << endl;
        QMenu *menuRecQsl = menu.addMenu(tr("QSL Rcvd"));
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 01"  << endl;
        menuRecQsl->addAction(qslRecViaBureauFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 02"  << endl;
        menuRecQsl->addAction(qslRecViaBureauMarkReqFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 03"  << endl;
        menuRecQsl->addAction(qslRecViaDirectFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 04"  << endl;
        menuRecQsl->addAction(qslRecViaDirectMarkReqFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 05"  << endl;
        menuRecQsl->addAction(qslRecRequestedAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 06"  << endl;
        
        qslRecViaBureauFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 07"  << endl;
        qslRecViaBureauMarkReqFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 08"  << endl;
        qslRecViaDirectFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 09"  << endl;
        qslRecViaDirectMarkReqFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 10"  << endl;
        qslRecRequestedAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -end qsl not rec"  << endl;
    }
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -end qsl"  << endl;
    menu.exec(QCursor::pos());
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -END"  << endl;
}

void MainWindow::slotRighButtonFromLog(const QPoint& pos)
{
    //qDebug() << "MainWindow::slotshowRighButtonFromLog"  << endl;
    int row = (logView->indexAt(pos)).row();
    showMenuRightButtonFromLogCreateActions();
    righButtonFromLogMenu(row);
}

void MainWindow::righButtonFromLogMenu(const int trow)
{
    //qDebug() << "MainWindow::slotshowRighButtonFromLogMenu:  " << QString::number(trow) << endl;
    int _qsoID = ((logModel->index(trow, 0)).data(0)).toInt();
    bool qslReceived = isQSLReceived(_qsoID);
    bool qslSent = isQSLSent(_qsoID);
    QMenu menu(this);

    menu.addAction(delQSOFromLogAct);
    delQSOFromLogAct->setData(trow);
    menu.addAction(qsoToEditFromLogAct);
    qsoToEditFromLogAct->setData(trow);

    switch (contestMode) {

        case CQ_WW_SSB:

        break;
        case CQ_WW_CW:

        break;
        default:
            menu.addSeparator();
            if (qslSent)
            {
            }
            else
            {
                QMenu *menuSentQsl = menu.addMenu(tr("QSL Send"));
                menuSentQsl->addAction(qslSentViaBureauFromLogAct);
                menuSentQsl->addAction(qslSentViaDirectFromLogAct);
                qslSentViaBureauFromLogAct->setData(trow);
                qslSentViaDirectFromLogAct->setData(trow);
            }

            if (qslReceived)
            {
            }
            else
            {
                QMenu *menuRecQsl = menu.addMenu(tr("QSL Rcvd"));
                menuRecQsl->addAction(qslRecViaBureauFromLogAct);
                menuRecQsl->addAction(qslRecViaDirectFromLogAct);
                qslRecViaBureauFromLogAct->setData(trow);
                qslRecViaDirectFromLogAct->setData(trow);
            }
        break;
    }
    menu.exec(QCursor::pos());
}


void MainWindow::showMenuRightButtonSearchCreateActions()
{
 //qDebug() << "MainWindow::showMenuRightButtonSearchCreateActions" << endl;

    delQSOFromSearchAct = new QAction(tr("&Delete"), this);
    delQSOFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_D);
    delQSOFromSearchAct->setStatusTip(tr("Delete a QSO"));
    connect(delQSOFromSearchAct, SIGNAL(triggered()), this, SLOT(slotQsoDeleteFromSearch()));

    qsoToEditFromSearchAct = new QAction(tr("&Edit QSO"), this);
    qsoToEditFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_E);
    qsoToEditFromSearchAct->setStatusTip(tr("Edit this QSO"));
    connect(qsoToEditFromSearchAct, SIGNAL(triggered()), this, SLOT(slotQSOToEditFromSearch()));

    qslSentViaBureauFromSearchAct = new QAction(tr("Via &bureau"), this);
    qslSentViaBureauFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_B);
    qslSentViaBureauFromSearchAct->setStatusTip(tr("Send this QSL via bureau"));
    connect(qslSentViaBureauFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauFromSearch() ));

    qslSentViaDirectFromSearchAct = new QAction(tr("D&irect"), this);
    qslSentViaDirectFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_I);
    qslSentViaDirectFromSearchAct->setStatusTip(tr("Send this QSL via direct"));
    connect(qslSentViaDirectFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectFromSearch()   ));


    qslSentRequestedAct = new QAction(tr("&Request my QSL"), this);
    qslSentRequestedAct->setShortcut(Qt::CTRL + Qt::Key_R);
    qslSentRequestedAct->setStatusTip(tr("Mark my QSL as requested"));
    connect(qslSentRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLSentMarkAsRequested()   ));

    qslRecRequestedAct = new QAction(tr("&Request the QSL"), this);
    //qslSentRequestedAct->setShortcut(Qt::CTRL + Qt::Key_R);
    qslRecRequestedAct->setStatusTip(tr("Mark the QSL as requested"));
    connect(qslRecRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLRecMarkAsRequested()   ));


    //if (sendQSLWhenRec)
    //{
        qslRecViaBureauMarkReqFromSearchAct = new QAction(tr("Via bureau && mark my QSL as requested"), this);
        qslRecViaBureauMarkReqFromSearchAct->setStatusTip(tr("QSL received via bureau & mark QSL as requested"));

        //qslRecViaBureauMarkReqFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_R);
        connect(qslRecViaBureauMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauMarkReqFromSearch() ));

        qslRecViaBureauFromSearchAct = new QAction(tr("Via bureau"), this);
        qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via bureau"));
    //}
    //else
    //{
        qslRecViaBureauFromSearchAct = new QAction(tr("Via bureau"), this);
        qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via bureau"));
    //}


    qslRecViaBureauFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(qslRecViaBureauFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauFromSearch() ));


    //if (sendQSLWhenRec)
    //{
        qslRecViaDirectMarkReqFromSearchAct = new QAction(tr("Direc&t && mark as my QSL requested"), this);
        qslRecViaDirectMarkReqFromSearchAct->setStatusTip(tr("QSL received via direct & mark my QSL as requested"));
        connect(qslRecViaDirectMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectMarkReqFromSearch() ));

        qslRecViaDirectFromSearchAct = new QAction(tr("Direc&t"), this);
        qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via direct"));
    //}
    //else
    //{
        qslRecViaDirectFromSearchAct = new QAction(tr("Direc&t"), this);
        qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via direct"));
    //}
        qslRecViaDirectFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_T);
    //qslRecViaDirectFromSearchAct->setStatusTip(tr("QSL received via direc&t"));
    connect(qslRecViaDirectFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectFromSearch() ));
}


void MainWindow::showMenuRightButtonFromLogCreateActions()
{
//qDebug() << "MainWindow::showMenuRightButtonFromLogCreateActions" << endl;

//TODO: Add the shortcut ( QAction::setShorCut()  )
  delQSOFromLogAct = new QAction(tr("&Delete"), this);
  delQSOFromLogAct->setShortcut(Qt::CTRL + Qt::Key_D);
  delQSOFromLogAct->setStatusTip(tr("Delete a QSO"));
  connect(delQSOFromLogAct, SIGNAL(triggered()), this, SLOT(slotQsoDeleteFromLog()));

  //qslSentFromLogAct = new QAction(tr("&Send via bureay"), this);
  //qslSentFromLogAct->setShortcut(Qt::CTRL + Qt::Key_S);
  //qslSentFromLogAct->setStatusTip(tr("Send the QSL"));
  //connect(qslSentFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSLSentFromLog()));

  //qslRecFromLogAct = new QAction(tr("QSL &Received"), this);
  //qslRecFromLogAct->setShortcut(Qt::CTRL + Qt::Key_R);
  //qslRecFromLogAct->setStatusTip(tr("QSL Received"));
  //connect(qslRecFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSLRecFromLog()));

  qsoToEditFromLogAct = new QAction(tr("&Edit QSO"), this);
  qsoToEditFromLogAct->setShortcut(Qt::CTRL + Qt::Key_E);
  qsoToEditFromLogAct->setStatusTip(tr("Edit this QSO"));
  connect(qsoToEditFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOToEditFromLog()));

  qslSentViaBureauFromLogAct = new QAction(tr("Via &bureau"), this);
  qslSentViaBureauFromLogAct->setShortcut(Qt::CTRL + Qt::Key_B);
  qslSentViaBureauFromLogAct->setStatusTip(tr("Send this QSL via bureau"));
  connect(qslSentViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauFromLog() ));

  qslSentViaDirectFromLogAct = new QAction(tr("D&irect"), this);
  qslSentViaDirectFromLogAct->setShortcut(Qt::CTRL + Qt::Key_I);
  qslSentViaDirectFromLogAct->setStatusTip(tr("Send this QSL via direct"));
  connect(qslSentViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectFromLog()   ));

  qslRecViaBureauFromLogAct = new QAction(tr("Via bureau"), this);
  qslRecViaBureauFromLogAct->setShortcut(Qt::CTRL + Qt::Key_R);
  qslRecViaBureauFromLogAct->setStatusTip(tr("QSL &received via bureau"));
  connect(qslRecViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauFromLog() ));

  qslRecViaDirectFromLogAct = new QAction(tr("Direct"), this);
  qslRecViaDirectFromLogAct->setShortcut(Qt::CTRL + Qt::Key_T);
  qslRecViaDirectFromLogAct->setStatusTip(tr("QSL received via direc&t"));
  connect(qslRecViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectFromLog() ));

}

void MainWindow::slotQSLSentViaBureauFromSearch()
{
   // //qDebug() << "MainWindow::slotQSLSentViaBureauFromSearch: " << (qslSentViaBureauFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslSentViaBureauFromSearchAct->data()).toInt();
    qslSentViaBureau(_qsoId);
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}

void MainWindow::slotQSLSentViaDirectFromSearch()
{
    //qDebug() << "MainWindow::slotQSLSentViaDirectFromSearch: " << (qslSentViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((qslSentViaDirectFromSearchAct->data()).toInt());
    dataProxy->qslSentViaDirect(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }



    //qslSentViaDirect(_qsoId);

}

void MainWindow::slotQSLSentViaBureauFromLog()
{
   //qDebug() << "MainWindow::slotQSLSentViaBureauFromLog: " << (qslSentViaBureauFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((logModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());    
    qslSentViaBureau(_qsoId);

}

void MainWindow::slotQSLSentViaDirectFromLog()
{    
    //qDebug() << "MainWindow::slotQSLSentViaDirectFromLog: " << (qslSentViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    dataProxy->qslSentViaDirect(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));

}

void MainWindow::qslSentViaBureau(const int _qsoId)
{
    //qDebug() << "MainWindow::qslSentViaBureau: " << QString::number(_qsoId) << endl;

    dataProxy->qslSentViaBureau(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));

}

void MainWindow::slotQSLSentMarkAsRequested()
{
   // bool qslSentAsRequested(const int _qsoId, const QString _updateDate);  

    int _qsoId = (qslSentRequestedAct->data()).toInt();
    dataProxy->qslSentAsRequested(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}


void MainWindow::slotQSLRecMarkAsRequested()
{
    int _qsoId = (qslRecRequestedAct->data()).toInt();
    dataProxy->qslRecAsRequested(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}


void MainWindow::slotQSLRecViaBureauFromLog()
{
   //qDebug() << "MainWindow::slotQSLRecViaBureauFromLog: " << (qslRecViaBureauAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaBureauAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = ((logModel->index( ( (qslRecViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaBureau(_qsoId);
}

void MainWindow::slotQSLRecViaDirectFromLog()
{
    //qDebug() << "MainWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaDirect(_qsoId);
    // Mark Sent, Bureau, date, update log.
}

void MainWindow::slotQSLRecViaBureauFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaBureauFromLog: " << "- Id = " << QString::number( ((logModel->index( ( (qslRecViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslRecViaBureauFromSearchAct->data()).toInt();
    qslRecViaBureau(_qsoId);
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }

}

void MainWindow::slotQSLRecViaBureauMarkReqFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaBureauMarkReqFromLog: " << "- Id = " << QString::number( ((logModel->index( ( (qslRecViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslRecViaBureauMarkReqFromSearchAct->data()).toInt();
    qslRecViaBureauMarkReq(_qsoId);
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }

}


void MainWindow::slotQSLRecViaDirectFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslRecViaDirectFromSearchAct->data()).toInt();
    qslRecViaDirect(_qsoId);
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}

void MainWindow::slotQSLRecViaDirectMarkReqFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslRecViaDirectMarkReqFromSearchAct->data()).toInt();
    qslRecViaDirectMarkReq(_qsoId);
    if(qslingNeeded)
    {
        slotToolSearchNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}

void MainWindow::qslRecViaBureau(const int _qsoId)
{
   // //qDebug() << "MainWindow::qslRecViaBureau: " << QString::number(_qsoId) << "/" << (dateTime->currentDateTime()).toString("yyyy/MM/dd") << endl;
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);
    dataProxy->qslRecViaBureau(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"), false);
    awards->setAwards(_qsoId);   //Update the DXCC award status

    logModel->select();
    showAwards();

}


void MainWindow::qslRecViaDirect(const int _qsoId)
{
    //qDebug() << "MainWindow::qslRecViaDirect: " << QString::number(_qsoId) << endl;

    dataProxy->qslRecViaDirect(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"), false);
    awards->setAwards(_qsoId);
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);

    showAwards();
}

void MainWindow::qslRecViaBureauMarkReq(const int _qsoId)
{
   // //qDebug() << "MainWindow::qslRecViaBureau: " << QString::number(_qsoId) << "/" << (dateTime->currentDateTime()).toString("yyyy/MM/dd") << endl;
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);
    dataProxy->qslRecViaBureau(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"), true);
    awards->setAwards(_qsoId);   //Update the DXCC award status

    logModel->select();
    showAwards();

}


void MainWindow::qslRecViaDirectMarkReq(const int _qsoId)
{
    //qDebug() << "MainWindow::qslRecViaDirect: " << QString::number(_qsoId) << endl;

    dataProxy->qslRecViaDirect(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"), true);
    awards->setAwards(_qsoId);
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);

    showAwards();
}

bool MainWindow::isQSLReceived(const int _qsoId)
{
    //qDebug() << "MainWindow::isQSLReceived: " << QString::number(_qsoId) << endl;
    return dataProxy->isQSLReceived(_qsoId);
}

bool MainWindow::isQSLSent(const int _qsoId)
{
    //qDebug() << "MainWindow::isQSLSent: " << QString::number(_qsoId) << endl;

    return dataProxy->isQSLSent(_qsoId);
}


void MainWindow::slotQSOToEditFromLog()
{
    //qDebug() << "slotQSOToEditFromLog: " << (qsoToEditFromLogAct->data()).toString() << endl;


    qsoToEdit((logModel->index((qsoToEditFromLogAct->data()).toInt(), 0)).data(0).toInt());

}

void MainWindow::slotQSOToEditFromSearch()
{
    //qDebug() << "slotQSOToEditFromSearch: " << (qsoToEditFromSearchAct->data()).toString() << endl;

    qsoToEdit((qsoToEditFromSearchAct->data()).toInt());

}


void MainWindow::slotQsoDeleteFromLog()
{
    //qDebug() << "MainWindow::slotQsoDeleteFromLog: " << (delQSOFromLogAct->data()).toString() << endl;

    QMessageBox msgBox;
    msgBox.setText(tr("You have requested to delete this QSO."));
    msgBox.setInformativeText(tr("Are you sure?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Yes:
          logModel->removeRow((delQSOFromLogAct->data()).toInt());
          slotSearchBoxTextChanged();
          awards->recalculateAwards();
          break;
      case QMessageBox::No:
          // No was clicked
          break;
      default:
          // should never be reached
          break;
    }
}

void MainWindow::slotQsoDeleteFromSearch()
{
    //qDebug() << "MainWindow::slotQsoDeleteFromSearch: " << (delQSOFromSearchAct->data()).toString() << endl;

    //qDebug() << "MainWindow::slotQsoDeleteFromSearch: " << QString::number((logModel->index((delQSOFromSearchAct->data(6)).toInt(), 0)).data(0).toInt()) << endl;

    QString _qrz = dataProxy->getCallFromId((delQSOFromSearchAct->data()).toInt());
    if (_qrz.length()>=3)
    {

        QString message = QString(tr("You have requested to delete the QSO with: %1")).arg(_qrz);

        QMessageBox msgBox;
        msgBox.setText(message);
        msgBox.setInformativeText(tr("Are you sure?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Yes:
            if(dataProxy->deleteQSO((delQSOFromSearchAct->data()).toInt()))
            {
                logModel->select();
                if(qslingNeeded)
                {
                    slotToolSearchNeededQSLToSend();
                }
                else
                {
                    slotSearchBoxTextChanged();
                }
                awards->recalculateAwards();

            }
            else
            {
                //TODO: The QSO could not be removed...
            }

            break;
            case QMessageBox::No:
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

int MainWindow::checkIfWorkedB4(const QString _qrz)
{
    //qDebug() << "MainWindow::checkIfWorkedB4: " << _qrz << endl;

    int i = dataProxy->isWorkedB4(_qrz, currentLog);

    switch (contestMode) {

        case CQ_WW_SSB:
            if (i>=0)
            {
                qrzgroupBox->setTitle(tr("DUPE"));
            }
            else
            {
                qrzgroupBox->setTitle(tr("QRZ"));
            }
        break;
        case CQ_WW_CW:
            if (i>=0)
            {
                qrzgroupBox->setTitle(tr("DUPE"));
            }
            else
            {
                qrzgroupBox->setTitle(tr("QRZ"));
            }
        break;
        default:

        break;

    }

    return i;
}


void MainWindow::readConfigData()
{
    //qDebug() << "MainWindow::slotReadConfigData - 01" << endl;
    if (needToEnd)
    {
        return;
    }
    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //qDebug() << "MainWindow::slotReadConfigData: File not found" << configFileName << endl;
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

    if ((useDefaultLogFileName) && (defaultADIFLogFile.length()>0))
    {
        useDefaultLogFileName = true;
    }
    else
    {
        useDefaultLogFileName = false;
    }


    if (imperialSystem)
    {
        distShortLabelN->setText(tr("Miles"));
        distLongLabelN->setText(tr("Miles"));
        //distShortLabel->setText( QString::number( Km2Mile(imperialSystem, (distShortLabel->text()).toInt() )) );
        //distLongLabel->setText( QString::number(Km2Mile(imperialSystem, (distLongLabel->text()).toInt()) ) );
    }
    else
    {
        distShortLabelN->setText(tr("Km"));
        distLongLabelN->setText(tr("Km"));
    }
    showEntityInfo(currentEntity);


    configured = true;
    awards->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    //dxClusterWidget->setDXClusterSpotConfig(const QString &_showhf, const QString &_showvhf, const QString &_showwarc, const QString &_showworked, const QString &_showconfirmed, const QString &_showann, const QString &_showwwv, const QString &_showwcy )


    initialContestModeConfiguration();

    if (upAndRunning)
    { // Next actions will not be executed in the first run
        slotClearButtonClicked();
        createSearchResultsPanel();
    }
//qDebug() << "MainWindow::slotReadConfigData - END" << endl;

}

bool MainWindow::processConfigLine(const QString _line){
    //qDebug() << "MainWindow::processConfigLine: " << _line << endl;

    QString line = (_line.toUpper()).simplified();
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

    QString value = values.at(1);
    int endValue = value.indexOf(';');
    if (endValue>-1){

        value = value.left(value.length() - (value.length() - endValue));
    }

    if (values.at(0) == "CALLSIGN"){
        stationQRZ = value;
    }else if (values.at(0)=="CQZ"){
        my_CQz = value.toInt();
    }else if (values.at(0)=="ITUZ"){
        my_ITUz = value.toInt();
    }else if (values.at(0)=="CONTEST"){
        //qDebug() << "MainWindow::processConfigLine: CONTEST: " << endl;
        if (value=="CQ-WW-SSB"){
            contestMode = CQ_WW_SSB;
        } else if (value=="CQ-WW-CW"){
            contestMode = CQ_WW_CW;
        } else if (value=="CQ-WPX-SSB"){
            contestMode = CQ_WPX_SSB;
        } else if (value=="CQ-WPX-CW"){
            contestMode = CQ_WPX_CW;
        } else if (value=="CQ-WPX-RTTY"){
            contestMode = CQ_WPX_RTTY;
        } else{
            contestMode = NoContest;
        }

    }else if (values.at(0)=="MODES"){
        //qDebug() << "MainWindow::processConfigLine: MODES: " << endl;
        readActiveModes(value.split(", ", QString::SkipEmptyParts));
    }else if (values.at(0)=="BANDS"){
        //qDebug() << "MainWindow::processConfigLine: BANDS: " << endl;
        readActiveBands(value.split(", ", QString::SkipEmptyParts));
    }else if (values.at(0)=="REALTIME"){
        //qDebug() << "MainWindow::processConfigLine: REALTIME: " << value.toUpper() << endl;
        if ( (value.toUpper()) == "FALSE")
        {
            realTime=false;
        }
        else
        {
            realTime=true;
        }
    }
    else if (values.at(0)=="INMEMORY")
    {
    //qDebug() << "MainWindow::processConfigLine: INMEMORY: " << value.toUpper() << endl;
        if ( (value.toUpper()) == "FALSE")
        {
            DBinMemory=false;
        }
        else
        {
            DBinMemory=true;
        }
    }
    else if (values.at(0) =="DXCLUSTERSERVERTOUSE"){
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


    else if(values.at(0)=="POWER")
    {
        if (value.toDouble()>0.0)
        {
            myPower = value.toDouble();

        }

    }
    else if (values.at(0)=="USEDEFAULTNAME")
    {
        if ( (value.toUpper()) == "TRUE")
        {
            useDefaultLogFileName=true;
        }
        else
        {
            useDefaultLogFileName=false;
        }
    }

    else if (values.at(0)=="IMPERIALSYSTEM")
    {
        if ( (value.toUpper()) == "TRUE")
        {
            imperialSystem=true;
        }
        else
        {
            imperialSystem=false;
        }
    }
    else if (values.at(0)=="SENDQSLWHENREC")
    {
        if ( (value.toUpper()) == "TRUE")
        {
            sendQSLWhenRec=true;
        }
        else
        {
            sendQSLWhenRec=false;
        }
    }

    else if (values.at(0)=="SHOWCALLSIGNINSEARCH")
    {
        if ( (value.toUpper()) == "TRUE")
        {
            stationCallSignShownInSearch=true;
        }
        else
        {
            stationCallSignShownInSearch=false;
        }
    }

    else if (values.at(0)=="ALWAYSADIF")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            alwaysADIF=false;
        }
        else
        {
            alwaysADIF=true;
        }
    }
    else if (values.at(0)=="UTCTIME")
    {
        //qDebug() << "MainWindow::processConfigLine: UTCTIME: " << value.toUpper() <<endl;
        if ( (value.toUpper()) == "FALSE")
        {
            UTCTime=false;
        }
        else
        {
            UTCTime=true;
        }

    }

    else if (values.at(0)=="DXCLUSTERSHOWHF")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowHF=false;
        }
        else
        {
            dxClusterShowHF=true;
        }
    }

    else if (values.at(0)=="DXCLUSTERSHOWVHF")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowVHF=false;
        }
        else
        {
            dxClusterShowVHF=true;
        }
    }

    else if (values.at(0)=="DXCLUSTERSHOWWARC")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowWARC=false;
        }
        else
        {
            dxClusterShowWARC=true;
        }
    }

    else if (values.at(0)=="DXCLUSTERSHOWWORKED")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowWorked=false;
        }
        else
        {
            dxClusterShowWorked=true;
        }
    }

    else if (values.at(0)=="DXCLUSTERSHOWCONFIRMED")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowConfirmed=false;
        }
        else
        {
            dxClusterShowConfirmed=true;
        }
    }

    else if (values.at(0)=="DXCLUSTERSHOWANN")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowAnn=false;
        }
        else
        {
            dxClusterShowAnn=true;
        }
    }


    else if (values.at(0)=="DXCLUSTERSHOWWWV")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowWWV=false;
        }
        else
        {
            dxClusterShowWWV=true;
        }
    }


    else if (values.at(0)=="DXCLUSTERSHOWWCY")
    {
        if ( (value.toUpper()) == "FALSE")
        {
            dxClusterShowWCY=false;
        }
        else
        {
            dxClusterShowWCY=true;
        }
    }

    else if (values.at(0)=="DEFAULTADIFFILE")
    {
        defaultADIFLogFile = value.toLower();
    }

    else if (values.at(0)=="STATIONLOCATOR")
    {

        if ( locator->isValidLocator(value) )
        {
            myLocator = value.toUpper();
        }
    }
    else if(values.at(0)=="NEWONECOLOR")
    {
        newOneColor.setNamedColor(value);
    }
    else if(values.at(0)=="NEEDEDCOLOR")
    {
        neededColor.setNamedColor(value);
    }
    else if(values.at(0)=="WORKEDCOLOR")
    {
        workedColor.setNamedColor(value);
    }
    else if(values.at(0)=="CONFIRMEDCOLOR")
    {
        confirmedColor.setNamedColor(value);
    }
    else if(values.at(0)=="DEFAULTCOLOR")
    {
        defaultColor.setNamedColor(value);
    }
    else
    {
        //qDebug() << "MainWindow::processConfigLine: NONE: " << endl;
    }



    // Lines are: Option = value;


    return true;


}


/*********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************/


void MainWindow::checkIfNewBandOrMode()
{//Checks the log to see if there is a QSO with a band/mode
 //that is not currently selected as active
    //qDebug() << "MainWindow::checkIfNewBandOrMode" << endl;
//    modes
//    bands
    int errorCode = 0;
    QHash<int, int> hashb;
    QHash<int, int> hashm;

    int bandIdInt = -1;
    int modeIdInt = -1;

    QString stringQuery, aux;

    QSqlQuery query, query1, query2;

    // We run the bands... identifying all of the already added, same for modes
    for (int i = 0; i < bands.count();  i++)
    {

        bandIdInt = dataProxy->getIdFromBandName(bands.at(i));
        if (bandIdInt>=0)
        {
            hashb[bandIdInt] = bandIdInt;
        }
        else
        {}

    }

    for (int i = 0; i < modes.count();  i++)
    {

        modeIdInt = dataProxy->getIdFromModeName(modes.at(i));
        if (modeIdInt>=0)
        {
            hashb[modeIdInt] = modeIdInt;
        }
        else
        {}
    }
    //qDebug() << "MainWindow::checkIfNewBandOrMode-1" << endl;
    stringQuery = QString("SELECT bandid, modeid FROM log WHERE lognumber='%1'").arg(currentLog);

    if(query.exec (stringQuery))
    {
        QSqlRecord rec = query.record();

        int nameCol;

        while (query.next())
        {
            if (query.isValid())
            {
            nameCol = rec.indexOf("bandid");
            //qDebug() << (query.value(nameCol)).toString(); // output all bands
            if (!(hashb.contains((query.value(nameCol)).toInt())))
            {

                //void MainWindow::readActiveBands (const QStringList actives)


                stringQuery = QString("SELECT name FROM band WHERE id='%1'").arg((query.value(nameCol)).toString());
                query1.exec(stringQuery);
                query1.next();
                if ( query1.isValid() )
                {
                    if ( db->isValidBand((query1.value(0)).toString())  )
                    {
                        hashb.insert((query.value(nameCol)).toInt(), (query.value(nameCol)).toInt() );

                        aux = (query1.value(0)).toString();
                        if (aux.length()>0)
                        {
                            bands << aux;
                        }

                        //qDebug() << "MainWindow::checkIfNewBandOrMode: New BAND: " << (query1.value(0)).toString() << " - "  << QString::number((query.value(nameCol)).toInt()) << endl;


                        //TODO: EA4TV añadir al hashb la banda nueva
                       // stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(bands.at(i));
                        //query2.exec(stringQuery);
                        //query2.next();
                        //hashb[(query2.value(0)).toInt()] = (query2.value(0)).toInt();
                    }
                }
            }
            else
            {
                //qDebug() << "MainWindow::checkIfNewBandOrMode: BAND not new: " << (query1.value(0)).toString() << endl;
            }

            nameCol = rec.indexOf("modeid");
            //qDebug() << (query.value(nameCol)).toString(); // output all bands

            if (!(hashm.contains((query.value(nameCol)).toInt())))
            {
                //void MainWindow::readActiveBands (const QStringList actives)
                stringQuery = QString("SELECT name FROM mode WHERE id='%1'").arg((query.value(nameCol)).toString());
                query1.exec(stringQuery);
                query1.next();
                if ( query1.isValid() )
                {
                    if ( db->isValidMode((query1.value(0)).toString())  )
                    {
                        hashm.insert((query.value(nameCol)).toInt(), (query.value(nameCol)).toInt() );

                        aux = (query1.value(0)).toString();
                        if (aux.length()>0)
                        {
                            modes << aux;
                        }
                        //modes << (query1.value(0)).toString();
                        //qDebug() << "MainWindow::checkIfNewBandOrMode: New MODE" << (query1.value(0)).toString() << endl;

                        //TODO: Add to the hashm the new modes
                    }
                }
            }
            else
            {
                //qDebug() << "MainWindow::checkIfNewBandOrMode: MODE not new" << endl;
            }
            }//Closes the next.isValid
        }//Closes the While
        bandComboBox->clear();
        bandComboBox->addItems(bands);
        modeComboBox->clear();
        modeComboBox->addItems(modes);

    }
    else
    {
        errorCode = query.lastError().number();
        QMessageBox msgBox;
        aux = tr("An unexpected error ocurred while looking for new bands & modes in your log. If the problem persists, please contact the developer for analysis: ");
        msgBox.setText(aux + "MW-2#" + QString::number(errorCode));
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
    //qDebug() << "MainWindow::checkIfNewBandOrMode-END" << endl;
}

/*********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
**********************************************************************/

void MainWindow::readActiveBands (const QStringList actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
    // bands used in the program
    //qDebug() << "MainWindow::readActiveBands: " << actives << endl;
    QString aux;
    bool atLeastOne = false;
    //QStringList values = actives;
    //QStringList values = actives.split(", ", QString::SkipEmptyParts);

    for (int i = 0; i < actives.count() ; i++)
    {
        if (db->isValidBand(actives.at(i)))
        {
            if (!atLeastOne)
            {
                //qDebug() << "MainWindow::readActiveBands (at least One!): " << values.at(i) << endl;
                atLeastOne = true;
                bands.clear();
            }
            aux = actives.at(i);
            if (aux.length()>0)
            {
               bands << aux;
            }
            //bands << actives.at(i);

            //qDebug() << "MainWindow::readActiveBands: " << values.at(i) << endl;
        }

    }
    bands.removeDuplicates();
}

void MainWindow::readActiveModes (const QStringList actives)
{
    //qDebug() << "MainWindow::readActiveModes: " << actives << endl;

    bool atLeastOne = false;
    QString aux;

    //QStringList values = actives.split(", ", QString::SkipEmptyParts);

    for (int i = 0; i < actives.count() ; i++)
    {
        if (db->isValidMode(actives.at(i)))
        {
            if (!atLeastOne)
            {
                atLeastOne = true;
                modes.clear();
            }
            aux = actives.at(i);
            if (aux.length()>0)
            {
               modes << aux;
            }

           // modes << actives.at(i);
        }

    }
    modes.removeDuplicates();

}



void MainWindow::createData()
{
    //qDebug() << "MainWindow::createData " << endl;



}

void MainWindow::createUIDX()
{
    //qDebug() << "MainWindow::createUIDX" << endl;

    QStringList continents;
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
    QSqlQuery query2("SELECT shortname FROM continent");
    while (query2.next()) {
        if (query2.isValid())
        {
            continents << query2.value(0).toString();
        }
    }

    iotaContinentComboBox->addItems(continents);
    iotaNumberLineEdit = new QLineEdit;
    iotaNumberLineEdit->setInputMask("000");
    iotaNumberLineEdit->setText("000");


    //bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    //modes << "SSB" << "CW" << "RTTY";
    bandComboBox->addItems(bands);
    modeComboBox->addItems(modes);
    txFreqSpinBox->setToolTip(tr("TX Frequency in MHz"));
    rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz"));
    myPowerSpinBox->setToolTip(tr("Power used for the QSO in watts"));
    rxPowerSpinBox->setToolTip(tr("Power used by the DX"));
    operatorLineEdit->setToolTip(tr("Logging operator's callsign"));
    stationCallSignLineEdit->setToolTip(tr("Callsign used over the air"));
    myLocatorLineEdit->setToolTip(tr("My QTH locator"));
    nameLineEdit->setToolTip(tr("Name of the DX"));
    qthLineEdit->setToolTip(tr("QTH of the DX"));
    locatorLineEdit->setToolTip(tr("Locator of the DX"));
    //freqQLCDNumber->setToolTip(tr("Frequency of the QSO"));
    qrzLineEdit->setToolTip(tr("QRZ of the QSO"));
    rstTXLineEdit->setToolTip(tr("TX RST"));
    rstRXLineEdit->setToolTip(tr("RX RST"));
    STXLineEdit->setToolTip(tr("TX Exchange"));
    SRXLineEdit->setToolTip(tr("RX Exchange"));
    bandComboBox->setToolTip(tr("Band of the QSO"));
    modeComboBox->setToolTip(tr("Mode of the QSO"));
    dateEdit->setToolTip(tr("Date of the QSO"));
    timeEdit->setToolTip(tr("Time of the QSO"));
    statusBar->setToolTip(tr("Misc information"));
    qsoStatusBar->setToolTip(tr("QSO information"));
    OKButton->setToolTip(tr("Add the QSO to the log"));
    spotItButton->setToolTip(tr("Spots this QSO to the DX Cluster"));
    clearButton->setToolTip(tr("Clears the box"));
    eqslSentComboBox->setToolTip(tr("Status of the eQSL sending"));
    eqslRecComboBox->setToolTip(tr("Status of the eQSL reception"));
    lotwSentComboBox->setToolTip(tr("Status of the LotW sending"));
    lotwRecComboBox->setToolTip(tr("Status of the LotW reception"));
    eqslSentQDateEdit->setToolTip(tr("Date of the eQSL sending"));
    eqslRecQDateEdit->setToolTip(tr("Date of the eQSL reception"));
    lotwSentQDateEdit->setToolTip(tr("Date of the LotW sending"));
    lotwRecQDateEdit->setToolTip(tr("Date of the LotW reception"));

    qslSentComboBox->setToolTip(tr("Status of the QSL sending"));
    qslRecComboBox->setToolTip(tr("Status of the QSL reception"));
    qslSentViaComboBox->setToolTip(tr("QSL sending information"));
    qslRecViaComboBox->setToolTip(tr("QSL reception information"));

    qslSentQDateEdit->setToolTip(tr("Date of the QSL sending"));
    qslRecQDateEdit->setToolTip(tr("Date of the QSL reception"));
    qslmsgTextEdit->setToolTip(tr("Message of the QSL"));
    qslViaLineEdit->setToolTip(tr("QSL via information"));

    dxccConfirmedQLCDNumber->setToolTip(tr("Number of confirmed DXCC entities"));
    dxccWorkedQLCDNumber->setToolTip(tr("Number of worked DXCC entities"));
    wazConfirmedQLCDNumber->setToolTip(tr("Number of confirmed WAZ zones"));
    wazWorkedQLCDNumber->setToolTip(tr("Number of worked  WAZ zones"));
    localConfirmedQLCDNumber->setToolTip(tr("Number of confirmed local references"));
    localWorkedQLCDNumber->setToolTip(tr("Number of worked local references"));
    qsoConfirmedQLCDNumber->setToolTip(tr("Number of confirmed QSO"));
    qsoWorkedQLCDNumber->setToolTip(tr("Number of worked QSO"));

    infoLabel1->setToolTip(tr("Status of the DX entity"));
    infoLabel2->setToolTip(tr("Name of the DX entity"));

    //QGridLayout *layout = new QGridLayout;

    dxUpLeftInputFrame = new QFrame;

    //dxUpRightOutputFrame = new QFrame;
    //dxUpRightOutputFrame->setFrameShadow(QFrame::Raised);
    //dxUpRightOutputFrame->setFrameStyle(QFrame::StyledPanel);


    dxUpLeftTab = new QTabWidget;
    dxUpRightTab = new QTabWidget;
    dxBottonTab = new QTabWidget; //rename to botton

    dxUpLeftTab->setTabPosition(QTabWidget::North);
    dxUpRightTab->setTabPosition(QTabWidget::South);

    QWidget *qsoInputTabWidget = new QWidget;
    //QFormLayout *qsoInputTabWidgetLayout = new QFormLayout;
    QLabel *nameLabel = new QLabel(qsoInputTabWidget);

    nameLabel->setText("Name");
    nameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *qthLabel = new QLabel(qsoInputTabWidget);
    qthLabel->setText("QTH");
    qthLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *locLabel = new QLabel(qsoInputTabWidget);
    locLabel->setText("Locator");
    locLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    //qDebug() << "MainWindow::createUIDX: Start" << endl;

    QLabel *rxPowerSpinBoxLabelN = new QLabel(tr("Power(rx)"));
    rxPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    QLabel *rstTxLabelN = new QLabel(tr("RST(tx)"));
    rstTxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstRxLabelN = new QLabel(tr("RST(rx)"));
    rstRxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    rstTXLineEdit->setInputMask("990");
    rstRXLineEdit->setInputMask("990");
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

    QWidget *qslInputTabWidget = new QWidget;
    QWidget *eqslInputTabWidget = new QWidget;
    QWidget *commentInputTabWidget = new QWidget;
    QWidget *othersInputTabWidget = new QWidget;
    QWidget *myDataInputTabWidget = new QWidget;

    int i = dxUpLeftTab->addTab(qsoInputTabWidget, tr("QSO"));

    // QSL Tab definition starts here

    QLabel *QSLSentLabelN = new QLabel(tr("QSL Sent"));
    QSLSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *QSLRecLabelN = new QLabel(tr("QSL Rec"));
    QSLRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *QSLViaLabelN = new QLabel(tr("QSL Via"));
    QSLViaLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);


/*
    QHBoxLayout *qslSentLayout = new QHBoxLayout;
    qslSentLayout->addWidget(qslSentComboBox);
    qslSentLayout->addWidget(qslSentQDateEdit);    
    qslSentLayout->addWidget(qslSentViaComboBox);

    QHBoxLayout *qslRecLayout = new QHBoxLayout;
    qslRecLayout->addWidget(qslRecComboBox);
    qslRecLayout->addWidget(qslRecQDateEdit);    
    qslRecLayout->addWidget(qslRecViaComboBox);

    QFormLayout *qslInputTabWidgetLayout = new QFormLayout;
    qslInputTabWidgetLayout->addRow(QSLSentLabelN, qslSentLayout);
    qslInputTabWidgetLayout->addRow(QSLRecLabelN, qslRecLayout);

    QHBoxLayout *qslViaLayout = new QHBoxLayout;
    qslViaLayout->addWidget(QSLViaLabelN);
    qslViaLayout->addWidget(qslViaLineEdit);

    QVBoxLayout *qslTabLayout = new QVBoxLayout;
    qslTabLayout->addLayout(qslInputTabWidgetLayout);
    qslTabLayout->addLayout(qslViaLayout);
    qslTabLayout->addWidget(qslmsgTextEdit);
    */

    QGridLayout *QSLLayout = new QGridLayout;
    QSLLayout->addWidget(QSLSentLabelN, 0, 0);
    QSLLayout->addWidget(QSLRecLabelN, 1, 0);
    QSLLayout->addWidget(QSLViaLabelN, 2, 0);

    QSLLayout->addWidget(qslSentComboBox, 0, 1);
    QSLLayout->addWidget(qslRecComboBox, 1, 1);
    QSLLayout->addWidget(qslViaLineEdit, 2, 1, 1, -1);

    QSLLayout->addWidget(qslSentQDateEdit, 0, 2);
    QSLLayout->addWidget(qslRecQDateEdit, 1, 2);
    QSLLayout->addWidget(qslSentViaComboBox, 0, 3);
    QSLLayout->addWidget(qslRecViaComboBox, 1, 3);


    qslInputTabWidget->setLayout(QSLLayout);
    i = dxUpLeftTab->addTab(qslInputTabWidget, tr("QSL"));


    // eQSL Tab definition starts here


    QLabel *eQSLSentLabelN = new QLabel(tr("eQSL Sent"));
    eQSLSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *eQSLRecLabelN = new QLabel(tr("eQSL Rec"));
    eQSLRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *lotWSentLabelN = new QLabel(tr("LotW Sent"));
    lotWSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *lotWRecLabelN = new QLabel(tr("LotW Rec"));
    lotWRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

/*
    QHBoxLayout *eqslSentLayout = new QHBoxLayout;
    eqslSentLayout->addWidget(eqslSentComboBox);
    eqslSentLayout->addWidget(eqslSentQDateEdit);
    QHBoxLayout *eqslRecLayout = new QHBoxLayout;
    eqslRecLayout->addWidget(eqslRecComboBox);
    eqslRecLayout->addWidget(eqslRecQDateEdit);
    QHBoxLayout *lotwSentLayout = new QHBoxLayout;
    lotwSentLayout->addWidget(lotwSentComboBox);
    lotwSentLayout->addWidget(lotwSentQDateEdit);
    QHBoxLayout *lotwRecLayout = new QHBoxLayout;
    lotwRecLayout->addWidget(lotwRecComboBox);
    lotwRecLayout->addWidget(lotwRecQDateEdit);

    QFormLayout *eqslInputTabWidgetLayout = new QFormLayout;

    eqslInputTabWidgetLayout->addRow(eQSLSentLabelN, eqslSentLayout);
    eqslInputTabWidgetLayout->addRow(eQSLRecLabelN, eqslRecLayout);
    eqslInputTabWidgetLayout->addRow(lotWSentLabelN, lotwSentLayout);
    eqslInputTabWidgetLayout->addRow(lotWRecLabelN, lotwRecLayout);
*/
    QGridLayout *eqslInputTabWidgetLayout = new QGridLayout;
    eqslInputTabWidgetLayout->addWidget(eQSLSentLabelN, 0, 0);
    eqslInputTabWidgetLayout->addWidget(eQSLRecLabelN, 1, 0);
    eqslInputTabWidgetLayout->addWidget(lotWSentLabelN, 2, 0);
    eqslInputTabWidgetLayout->addWidget(lotWRecLabelN, 3, 0);

    eqslInputTabWidgetLayout->addWidget(eqslSentComboBox, 0, 1);
    eqslInputTabWidgetLayout->addWidget(eqslRecComboBox, 1, 1);
    eqslInputTabWidgetLayout->addWidget(lotwSentComboBox, 2, 1);
    eqslInputTabWidgetLayout->addWidget(lotwRecComboBox, 3, 1);

    eqslInputTabWidgetLayout->addWidget(eqslSentQDateEdit, 0, 2);
    eqslInputTabWidgetLayout->addWidget(eqslRecQDateEdit, 1, 2);
    eqslInputTabWidgetLayout->addWidget(lotwSentQDateEdit, 2, 2);
    eqslInputTabWidgetLayout->addWidget(lotwRecQDateEdit, 3, 2);

    eqslInputTabWidget->setLayout(eqslInputTabWidgetLayout);

    i = dxUpLeftTab->addTab(eqslInputTabWidget, tr("eQSL"));
/*
    // NOTES tab starts here
    QGridLayout *notesInputTabWidgetLayout = new QGridLayout;
    notesInputTabWidgetLayout->addWidget(notesTextEdit, 0, 0);
    notesInputTabWidget->setLayout(notesInputTabWidgetLayout);
    i = dxUpLeftTab->addTab(notesInputTabWidget, tr("Notes"));
*/
    // COMMENT tab starts here
    QGridLayout *commentInputTabWidgetLayout = new QGridLayout;
    commentInputTabWidgetLayout->addWidget(commentLineEdit, 0, 0);
    commentInputTabWidget->setLayout(commentInputTabWidgetLayout);
    i = dxUpLeftTab->addTab(commentInputTabWidget, tr("Comment"));


    //entityAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //iotaAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    //entityNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    entityAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    iotaAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    entityNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

// Others Tab starts here

/*
    QHBoxLayout *othersIotaInputLayout = new QHBoxLayout;
    othersIotaInputLayout->addWidget(iotaContinentComboBox);
    othersIotaInputLayout->addWidget(iotaNumberLineEdit);

    QFormLayout *othersInputTabWidgetLayout = new QFormLayout;
    othersInputTabWidgetLayout->addRow(iotaAwardLabel, othersIotaInputLayout);
    //othersInputTabWidgetLayout->addWidget(iotaContinentComboBox, 0, 1);
    //othersInputTabWidgetLayout->addWidget(iotaNumberLineEdit, 0, 2);
    othersInputTabWidgetLayout->addRow(entityAwardLabel, entityAwardComboBox);
    othersInputTabWidgetLayout->addRow(entityNameLabel, entityNameComboBox);
    //othersInputTabWidgetLayout->addWidget(entityAwardComboBox, 1, 1);
*/

    QGridLayout *othersInputTabWidgetLayout = new QGridLayout;
    othersInputTabWidgetLayout->addWidget(iotaAwardLabel, 0, 0);
    othersInputTabWidgetLayout->addWidget(iotaContinentComboBox, 0, 1);
    othersInputTabWidgetLayout->addWidget(iotaNumberLineEdit, 0, 2);
    othersInputTabWidgetLayout->addWidget(entityAwardLabel, 1, 0);
    othersInputTabWidgetLayout->addWidget(entityAwardComboBox, 1, 1, 1, 2);
    othersInputTabWidgetLayout->addWidget(entityNameLabel, 2, 0);
    othersInputTabWidgetLayout->addWidget(entityNameComboBox, 2, 1, 1, 2);

    othersInputTabWidget->setLayout(othersInputTabWidgetLayout);
    i = dxUpLeftTab->addTab(othersInputTabWidget, tr("Others"));


// MyData tab starts here

    QLabel *myPowerSpinBoxLabelN = new QLabel(tr("Power"));
    myPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *operatorLabelN = new QLabel(tr("Operator"));

    operatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *stationCallSignLabelN = new QLabel(tr("Station Callsign"));

    stationCallSignLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *myLocatorLabelN = new QLabel(tr("My Locator"));

    myLocatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QGridLayout *myDataInputTabWidgetLayout = new QGridLayout;
    myDataInputTabWidgetLayout->addWidget(myPowerSpinBoxLabelN, 0, 0);
    myDataInputTabWidgetLayout->addWidget(operatorLabelN, 1, 0);
    myDataInputTabWidgetLayout->addWidget(stationCallSignLabelN, 2, 0);
    myDataInputTabWidgetLayout->addWidget(myLocatorLabelN, 3, 0);

    myDataInputTabWidgetLayout->addWidget(myPowerSpinBox, 0, 1);
    myDataInputTabWidgetLayout->addWidget(operatorLineEdit, 1, 1);
    myDataInputTabWidgetLayout->addWidget(stationCallSignLineEdit, 2, 1);
    myDataInputTabWidgetLayout->addWidget(myLocatorLineEdit, 3, 1);

    myDataInputTabWidget->setLayout(myDataInputTabWidgetLayout);
    i = dxUpLeftTab->addTab(myDataInputTabWidget, tr("My Data"));

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
    //qrzvbox->addWidget(qrzLineEdit);
    qrzvbox->addLayout(QrzBandModeLayout);
    //qrzvbox->addLayout(BandModeLayout);
    qrzvbox->addStretch(1);
    qrzgroupBox->setLayout(qrzvbox);


    QHBoxLayout *statusBarLayout = new QHBoxLayout;
    statusBarLayout->addWidget(statusBar);
    statusBarLayout->addWidget(qsoStatusBar);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(OKButton);
    buttonsLayout->addWidget(spotItButton);
    buttonsLayout->addWidget(clearButton);

    QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("yyyy/MM/dd");
    timeEdit->setDisplayFormat("HH:mm:ss");


    QGridLayout *dxUpLeftInputFrameLayout = new QGridLayout;
    dxUpLeftInputFrameLayout->addWidget(qrzgroupBox, 0, 0, 1, 0);
    dxUpLeftInputFrameLayout->addLayout(TimeLayout, 1, 0);
    //dxUpLeftInputFrameLayout->addLayout(BandModeLayout, 1, 1);
    dxUpLeftInputFrameLayout->addLayout(buttonsLayout,1, 1);

    dxUpLeftInputFrame->setLayout(dxUpLeftInputFrameLayout);

    QSplitter *upLeftSplitter = new QSplitter (this);
    upLeftSplitter->addWidget(dxUpLeftInputFrame);
    upLeftSplitter->addWidget(dxUpLeftTab);
    upLeftSplitter->setOrientation(Qt::Vertical);


    //QVBoxLayout *dxUpLeftLayout = new QVBoxLayout;
    //dxUpLeftLayout->addWidget(dxUpLeftInputFrame);
    //dxUpLeftLayout->addWidget(dxUpLeftTab);


    QVBoxLayout *dxUpRightFixLayout = new QVBoxLayout;
    dxUpRightFixLayout->addWidget(infoLabel1);
    dxUpRightFixLayout->addWidget(infoLabel2);


    infoLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    infoLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    bandLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel3->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel4->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel5->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel6->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel7->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel8->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel9->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel10->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel11->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel12->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *continentLabelN = new QLabel(tr("Continent"));
    continentLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    continentLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *prefixLabelN = new QLabel(tr("Prefix"));
    prefixLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    prefixLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *cqzLabelN = new QLabel(tr("CQ"));
    cqzLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    cqzLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *ituzLabelN = new QLabel(tr("ITU"));
    ituzLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    ituzLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    QLabel *shortLabelN = new QLabel(tr("Short Path"));
    shortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    QLabel *longLabelN = new QLabel(tr("Long Path"));
    longLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *gradShortLabelN = new QLabel(tr("Grad"));
    gradShortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    gradShortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    //QLabel *distShortLabelN = new QLabel();
    //QLabel *distLongLabelN = new QLabel();
/*
    if (imperialSystem)
    {
        distShortLabelN->setText(tr("Miles"));
        distLongLabelN->setText(tr("Miles"));
    }
    else
    {
        distShortLabelN->setText(tr("Km"));
        distLongLabelN->setText(tr("Km"));
    }
*/
    distShortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *gradLongLabelN = new QLabel(tr("Grad"));
    gradLongLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distShortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    distLongLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distLongLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QGridLayout *dxUpRightInfoTabmini1Layout = new QGridLayout;
    dxUpRightInfoTabmini1Layout->addWidget(continentLabelN, 0, 0);
    dxUpRightInfoTabmini1Layout->addWidget(continentLabel, 1, 0);
    dxUpRightInfoTabmini1Layout->addWidget(prefixLabelN, 0, 1);
    dxUpRightInfoTabmini1Layout->addWidget(prefixLabel, 1, 1);
    dxUpRightInfoTabmini1Layout->addWidget(cqzLabelN, 0, 2);
    dxUpRightInfoTabmini1Layout->addWidget(cqzLabel, 1, 2);
    dxUpRightInfoTabmini1Layout->addWidget(ituzLabelN, 0, 3);
    dxUpRightInfoTabmini1Layout->addWidget(ituzLabel, 1, 3);

    QGridLayout *dxUpRightInfoTabmini2Layout = new QGridLayout;
    dxUpRightInfoTabmini2Layout->addWidget(shortLabelN, 0, 0, 1, 0);
    dxUpRightInfoTabmini2Layout->addWidget(gradShortLabelN, 1, 0);
    dxUpRightInfoTabmini2Layout->addWidget(gradShortLabel, 1, 1);
    dxUpRightInfoTabmini2Layout->addWidget(distShortLabelN, 1, 2);
    dxUpRightInfoTabmini2Layout->addWidget(distShortLabel, 1, 3);

    QGridLayout *dxUpRightInfoTabmini3Layout = new QGridLayout;
    dxUpRightInfoTabmini3Layout->addWidget(longLabelN, 0, 0, 1, 0);
    dxUpRightInfoTabmini3Layout->addWidget(gradLongLabelN, 1, 0);
    dxUpRightInfoTabmini3Layout->addWidget(gradLongLabel, 1, 1);
    dxUpRightInfoTabmini3Layout->addWidget(distLongLabelN, 1, 2);
    dxUpRightInfoTabmini3Layout->addWidget(distLongLabel, 1, 3);

    QHBoxLayout *dxUpRightInfoTabmini4Layout = new QHBoxLayout;
    dxUpRightInfoTabmini4Layout->addLayout(dxUpRightInfoTabmini2Layout);
    dxUpRightInfoTabmini4Layout->addLayout(dxUpRightInfoTabmini3Layout);

    QWidget *infoTabWidget = new QWidget;

    QGridLayout *dxUpRightInfoBandsTabLayout = new QGridLayout;
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel1, 0, 0);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel2, 0, 1);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel3, 0, 2);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel4, 0, 3);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel5, 0, 4);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel6, 0, 5);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel7, 1, 0);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel8, 1, 1);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel9, 1, 2);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel10, 1, 3);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel11, 1, 4);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel12, 1, 5);

    QVBoxLayout *dxUpRightInfoTabLayout = new QVBoxLayout;
    dxUpRightInfoTabLayout->addLayout(dxUpRightInfoBandsTabLayout);
    dxUpRightInfoTabLayout->addLayout(dxUpRightInfoTabmini1Layout);
    dxUpRightInfoTabLayout->addLayout(dxUpRightInfoTabmini4Layout);


    infoTabWidget->setLayout(dxUpRightInfoTabLayout);
/*
addLayout ( QLayout * layout, int row, int column,
int rowSpan, int columnSpan, Qt::Alignment alignment = 0 )
*/

    QWidget *searchTabWidget = new QWidget;
    i = dxUpRightTab->addTab(infoTabWidget, tr("Info"));

    QWidget *awardsTabWidget = new QWidget;

    QLabel *awardLabelN = new QLabel(tr("Award"));    
    awardLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *confirmedLabelN = new QLabel(tr("Confirmed"));
    confirmedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *workedLabelN = new QLabel(tr("Worked"));    
    workedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *dxccLabelN = new QLabel(tr("DXCC"));

    //dxccLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    dxccLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *wazLabelN = new QLabel(tr("WAZ"));    
    wazLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *localLabelN = new QLabel(tr("Local"));    
    localLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *qsoNLabelN = new QLabel(tr("QSOs"));
    qsoNLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

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
    dxUpRightAwardsTabLayout->addWidget(recalculateAwardsButton, 5, 1);



    awardsTabWidget->setLayout(dxUpRightAwardsTabLayout);

    i = dxUpRightTab->addTab(awardsTabWidget, tr("Awards"));

/*
    searchgroupBox = new QGroupBox(tr("Search"));
    searchgroupBox->setFlat(true);
    QVBoxLayout *searchvbox = new QVBoxLayout;
    searchvbox->addWidget(searchBoxLineEdit);
    searchvbox->addStretch(1);
    searchgroupBox->setLayout(searchvbox);
*/

    QHBoxLayout *dxUpRightButtonsLayout = new QHBoxLayout;
    //dxUpRightLineAndButtonsLayout->addWidget(searchBoxLineEdit);

    dxUpRightButtonsLayout->addWidget(searchBoxReSearchButton);
    dxUpRightButtonsLayout->addWidget(searchBoxClearButton);
    dxUpRightButtonsLayout->addWidget(searchBoxSelectAllButton);
    dxUpRightButtonsLayout->addWidget(searchBoxExportButton);
/*
    QGridLayout *dxUpRightSearchTabLayout = new QGridLayout;
    //dxUpRightSearchTabLayout->addWidget(searchgroupBox, 0, 0 );
    //dxUpRightSearchTabLayout->addLayout(dxUpRightLineAndButtonsLayout, 0, 1 );
    dxUpRightSearchTabLayout->addWidget(searchBoxLineEdit, 0, 0, -1, 0);
    dxUpRightSearchTabLayout->addLayout(dxUpRightButtonsLayout, 1, 0, -1, 0);
    dxUpRightSearchTabLayout->addWidget(searchResultsTreeWidget, 2, 0, -1, 0 );
*/
    QVBoxLayout *dxUpRightSearchTabLayout = new QVBoxLayout;
    //dxUpRightSearchTabLayout->addWidget(searchgroupBox, 0, 0 );
    //dxUpRightSearchTabLayout->addLayout(dxUpRightLineAndButtonsLayout, 0, 1 );
    dxUpRightSearchTabLayout->addWidget(searchBoxLineEdit);
    dxUpRightSearchTabLayout->addLayout(dxUpRightButtonsLayout);
    dxUpRightSearchTabLayout->addWidget(searchResultsTreeWidget);

    searchTabWidget->setLayout(dxUpRightSearchTabLayout);

    //searchTabWidget->setLayout(dxUpRightSearchALLLayout);

    i = dxUpRightTab->addTab(searchTabWidget, tr("Search"));

    QWidget *logTabWidget = new QWidget;
    QWidget *dxClusterTabWidget = new QWidget;

    QHBoxLayout *logTabWidgetLayout = new QHBoxLayout;
    QHBoxLayout *dxClusterTabWidgetLayout = new QHBoxLayout;

    logTabWidgetLayout->addWidget(logView);//
    logTabWidget->setLayout(logTabWidgetLayout);

    dxClusterTabWidgetLayout->addWidget(dxClusterWidget);
    dxClusterTabWidget->setLayout(dxClusterTabWidgetLayout);


    i = dxBottonTab->addTab(logTabWidget, tr("Log"));
    i = dxBottonTab->addTab(dxClusterTabWidget, tr("DX-Cluster"));


    QVBoxLayout *dxUpRightLayout = new QVBoxLayout;
    dxUpRightLayout->addLayout(dxUpRightFixLayout);
    dxUpRightLayout->addWidget(dxUpRightTab);

    QWidget *upRightWidget = new QWidget;
    upRightWidget->setLayout(dxUpRightLayout);

    //QWidget *upLeftWidget = new QWidget;
    //upLeftWidget->setLayout(dxUpLeftLayout);

    ////////////
    QSplitter *upSplitter = new QSplitter (this);
    //upSplitter->addWidget(upLeftWidget);
    upSplitter->addWidget(upLeftSplitter);
    upSplitter->addWidget(upRightWidget);


    //QHBoxLayout *upperLayout = new QHBoxLayout;
    //upperLayout->addLayout(dxUpLeftLayout);
    //upperLayout->addLayout(dxUpRightLayout);

    //QWidget *upper = new QWidget;
    //upper->setLayout(upperLayout);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(upSplitter);
    splitter->addWidget(dxBottonTab);
    splitter->setOrientation(Qt::Vertical);

    QHBoxLayout *mLayout = new QHBoxLayout;
    mLayout->addWidget(splitter);

    mainWidget->setLayout(mLayout);

    //layout->addLayout(dxUpLeftLayout, 0, 0);
    //layout->addLayout(dxUpRightLayout, 0, 1);
    //layout->addWidget(dxBottonTab, 1, 0, 1, 2);

    //mainWidget->setLayout(layout);

    // Disabled widgets to be enabled as functionality is deployed
    //iotaContinentComboBox->setEnabled(false);
    //iotaNumberLineEdit->setEnabled(false);
    //myPowerSpinBox->setEnabled(false);

    //locatorLineEdit->setEnabled(false);
    //eqslSentComboBox->setEnabled(false);
    //eqslRecComboBox->setEnabled(false);
    //lotwSentComboBox->setEnabled(false);
    //lotwRecComboBox->setEnabled(false);

    eqslSentQDateEdit->setEnabled(false);
    eqslRecQDateEdit->setEnabled(false);
    lotwSentQDateEdit->setEnabled(false);
    lotwRecQDateEdit->setEnabled(false);

    //qslSentCheckBox->setEnabled(false);
    //qslRecCheckBox->setEnabled(false);
    qslSentQDateEdit->setEnabled(false);
    qslRecQDateEdit->setEnabled(false);

    qslSentViaComboBox->setEnabled(false);
    qslRecViaComboBox->setEnabled(false);
    //qslmsgTextEdit->setEnabled(false);
    //qslViaLineEdit->setEnabled(false);
    entityAwardComboBox->setEnabled(false);
    entityNameComboBox->setEnabled(false);

//qDebug() << "MainWindow::createUIDX - OS DETECTION"  << endl;

#ifdef Q_OS_WIN
    //qDebug() << "MainWindow::createUIDX - WINDOWS DETECTED!"  << endl;

    //txFreqSpinBox->setFrame(true);
    //txFreqSpinBox->setFrameShadow(QFrame::Raised);
    //txFreqSpinBox->setFrameStyle(QFrame::StyledPanel);
    //rxFreqSpinBox->setFrameShadow(QFrame::Raised);
    //rxFreqSpinBox->setFrameStyle(QFrame::StyledPanel);

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
    continentLabel->setFrameShadow(QFrame::Raised);
    continentLabel->setFrameStyle(QFrame::StyledPanel);
    continentLabelN->setFrameShadow(QFrame::Raised);
    continentLabelN->setFrameStyle(QFrame::StyledPanel);
    prefixLabelN->setFrameShadow(QFrame::Raised);
    prefixLabelN->setFrameStyle(QFrame::StyledPanel);
    prefixLabel->setFrameShadow(QFrame::Raised);
    prefixLabel->setFrameStyle(QFrame::StyledPanel);
    cqzLabelN->setFrameShadow(QFrame::Raised);
    cqzLabelN->setFrameStyle(QFrame::StyledPanel);
    cqzLabel->setFrameShadow(QFrame::Raised);
    cqzLabel->setFrameStyle(QFrame::StyledPanel);
    ituzLabel->setFrameShadow(QFrame::Raised);
    ituzLabel->setFrameStyle(QFrame::StyledPanel);
    ituzLabelN->setFrameShadow(QFrame::Raised);
    ituzLabelN->setFrameStyle(QFrame::StyledPanel);    
    shortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    shortLabelN->setFrameShadow(QFrame::Raised);
    shortLabelN->setFrameStyle(QFrame::StyledPanel);

    longLabelN->setFrameShadow(QFrame::Raised);
    longLabelN->setFrameStyle(QFrame::StyledPanel);
    gradShortLabelN->setFrameShadow(QFrame::Raised);
    gradShortLabelN->setFrameStyle(QFrame::StyledPanel);
    gradShortLabel->setFrameShadow(QFrame::Raised);
    gradShortLabel->setFrameStyle(QFrame::StyledPanel);
    distShortLabelN->setFrameShadow(QFrame::Raised);
    distShortLabelN->setFrameStyle(QFrame::StyledPanel);
    distShortLabel->setFrameShadow(QFrame::Raised);
    distShortLabel->setFrameStyle(QFrame::StyledPanel);
    gradLongLabelN->setFrameShadow(QFrame::Raised);
    gradLongLabelN->setFrameStyle(QFrame::StyledPanel);
    gradLongLabel->setFrameShadow(QFrame::Raised);
    gradLongLabel->setFrameStyle(QFrame::StyledPanel);
    distLongLabelN->setFrameShadow(QFrame::Raised);
    distLongLabelN->setFrameStyle(QFrame::StyledPanel);
    distLongLabel->setFrameShadow(QFrame::Raised);
    distLongLabel->setFrameStyle(QFrame::StyledPanel);

    //entityAwardLabel->setFrameShadow(QFrame::Raised);
    //entityAwardLabel->setFrameStyle(QFrame::StyledPanel);
    //iotaAwardLabel->setFrameShadow(QFrame::Raised);
    //iotaAwardLabel->setFrameStyle(QFrame::StyledPanel);
    //entityNameLabel->setFrameShadow(QFrame::Raised);
    //entityNameLabel->setFrameStyle(QFrame::StyledPanel);

    bandLabel1->setFrameShadow(QFrame::Raised);
    bandLabel1->setFrameStyle(QFrame::StyledPanel);
    bandLabel2->setFrameShadow(QFrame::Raised);
    bandLabel2->setFrameStyle(QFrame::StyledPanel);
    bandLabel3->setFrameShadow(QFrame::Raised);
    bandLabel3->setFrameStyle(QFrame::StyledPanel);
    bandLabel4->setFrameShadow(QFrame::Raised);
    bandLabel4->setFrameStyle(QFrame::StyledPanel);
    bandLabel5->setFrameShadow(QFrame::Raised);
    bandLabel5->setFrameStyle(QFrame::StyledPanel);
    bandLabel6->setFrameShadow(QFrame::Raised);
    bandLabel6->setFrameStyle(QFrame::StyledPanel);

    bandLabel7->setFrameShadow(QFrame::Raised);
    bandLabel7->setFrameStyle(QFrame::StyledPanel);
    bandLabel8->setFrameShadow(QFrame::Raised);
    bandLabel8->setFrameStyle(QFrame::StyledPanel);
    bandLabel9->setFrameShadow(QFrame::Raised);
    bandLabel9->setFrameStyle(QFrame::StyledPanel);
    bandLabel10->setFrameShadow(QFrame::Raised);
    bandLabel10->setFrameStyle(QFrame::StyledPanel);
    bandLabel11->setFrameShadow(QFrame::Raised);
    bandLabel11->setFrameStyle(QFrame::StyledPanel);
    bandLabel12->setFrameShadow(QFrame::Raised);
    bandLabel12->setFrameStyle(QFrame::StyledPanel);

    infoLabel1->setFrameShadow(QFrame::Raised);
    infoLabel1->setFrameStyle(QFrame::StyledPanel);
    infoLabel2->setFrameShadow(QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel);
/*
    QSLViaLabelN->setFrameShadow(QFrame::Raised);
    QSLViaLabelN->setFrameStyle(QFrame::StyledPanel);
    QSLSentLabelN->setFrameShadow(QFrame::Raised);
    QSLSentLabelN->setFrameStyle(QFrame::StyledPanel);
    QSLRecLabelN->setFrameShadow(QFrame::Raised);
    QSLRecLabelN->setFrameStyle(QFrame::StyledPanel);
    eQSLSentLabelN->setFrameShadow(QFrame::Raised);
    eQSLSentLabelN->setFrameStyle(QFrame::StyledPanel);
    eQSLRecLabelN->setFrameShadow(QFrame::Raised);
    eQSLRecLabelN->setFrameStyle(QFrame::StyledPanel);
    lotWSentLabelN->setFrameShadow(QFrame::Raised);
    lotWSentLabelN->setFrameStyle(QFrame::StyledPanel);
    myPowerSpinBoxLabelN->setFrameShadow(QFrame::Raised);
    myPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel);
    rxPowerSpinBoxLabelN->setFrameShadow(QFrame::Raised);
    rxPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel);
    operatorLabelN->setFrameShadow(QFrame::Raised);
    operatorLabelN->setFrameStyle(QFrame::StyledPanel);
    stationCallSignLabelN->setFrameShadow(QFrame::Raised);
    stationCallSignLabelN->setFrameStyle(QFrame::StyledPanel);
    myLocatorLabelN->setFrameShadow(QFrame::Raised);
    myLocatorLabelN->setFrameStyle(QFrame::StyledPanel);
    lotWRecLabelN->setFrameShadow(QFrame::Raised);
    lotWRecLabelN->setFrameStyle(QFrame::StyledPanel);
*/
    //nameLabel->setFrameShape(QFrame::StyledPanel);

    //nameLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //nameLineEdit->setFrameShadow(QFrame::Raised);
    //nameLineEdit->setFrameStyle(QFrame::StyledPanel);
    //qthLabel->setFrameShape(QFrame::Box);
    //qthLabel->setFrameShadow(QFrame::Raised);
    //qthLabel->setFrameStyle(QFrame::StyledPanel);

    //qthLineEdit->setFrameShadow(QFrame::Raised);
    //qthLineEdit->setFrameStyle(QFrame::StyledPanel);

    //locLabel->setFrameShadow(QFrame::Raised);
    //locLabel->setFrameStyle(QFrame::StyledPanel);

    //locatorLineEdit->setFrameShadow(QFrame::Raised);
    //locatorLineEdit->setFrameStyle(QFrame::StyledPanel);

    //rxPowerLayout->setFrameShadow(QFrame::Raised);
    //rxPowerLayout->setFrameStyle(QFrame::StyledPanel);

    //txfreqLabelN->setFrameShadow(QFrame::Raised);
    //txfreqLabelN->setFrameStyle(QFrame::StyledPanel);
    //rxfreqLabelN->setFrameShadow(QFrame::Raised);
    //rxfreqLabelN->setFrameStyle(QFrame::StyledPanel);



    //freqLabelN->setFrameShadow(QFrame::Raised);
    //freqLabelN->setFrameStyle(QFrame::StyledPanel);
/*
    nameLabel->setFrameShadow(QFrame::Raised);
    nameLabel->setFrameStyle(QFrame::StyledPanel);
    qthLabel->setFrameShadow(QFrame::Raised);
    qthLabel->setFrameStyle(QFrame::StyledPanel);
    locLabel->setFrameShadow(QFrame::Raised);
    locLabel->setFrameStyle(QFrame::StyledPanel);
    rstRxLabelN->setFrameShadow(QFrame::Raised);
    rstRxLabelN->setFrameStyle(QFrame::StyledPanel);
    rstTxLabelN->setFrameShadow(QFrame::Raised);
    rstTxLabelN->setFrameStyle(QFrame::StyledPanel);
*/


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
    continentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    continentLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    prefixLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    prefixLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    cqzLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    cqzLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ituzLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ituzLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    shortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    longLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradShortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradShortLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distShortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distShortLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradLongLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distLongLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distLongLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradLongLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //iotaAwardLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //entityAwardLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //entityNameLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    bandLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel3->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel4->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel5->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel6->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel7->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel8->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel9->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel10->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel11->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel12->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


    //QSLViaLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //QSLSentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //QSLRecLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //eQSLSentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //eQSLRecLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //lotWSentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //myPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rxPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //operatorLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //stationCallSignLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //myLocatorLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //lotWRecLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    //nameLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //nameLineEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //qthLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //qthLineEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //locLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //locatorLineEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rxPowerLayout->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


    //txfreqLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rxfreqLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rstRxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rstTxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //nameLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //qthLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //locLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


#endif


 }

void MainWindow::slotADIFExport(){
    //qDebug() << "MainWindow::slotADIFExport " << endl;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               kontestDir,
                               tr("ADIF (*.adi *.adif)"));


    filemanager->adifLogExport(fileName);


}

void MainWindow::slotRQSLExport()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               kontestDir,
                               tr("ADIF (*.adi *.adif)"));


    filemanager->adifReqQSLExport(fileName);

}

void MainWindow::slotCabrilloExport()
{
    //qDebug() << "MainWindow::slotCabrilloExport " << endl;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Cabrillo File"),
                               kontestDir,
                               tr("Cabrillo (*.log)"));


    contest->saveFileToSend(fileName);

}

void MainWindow::slotADIFImport(){
    //qDebug() << "MainWindow::slotADIFImport " << endl;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     kontestDir,
                                                     tr("ADIF (*.adi *.adif)"));
    if (fileName.isNull())
    {
    }
    else
    {
        filemanager->adifReadLog(fileName, currentLog);
        logModel->select();

        checkIfNewBandOrMode();

        switch (contestMode) {

            case CQ_WW_SSB:
            break;
            case CQ_WW_CW:
            break;
            default:
                awards->recalculateAwards();
                showAwards();
            break;

        }
        //qDebug() << "MainWindow::slotADIFImport-7" << endl;
    }



}

void  MainWindow::initialContestModeConfiguration()
{

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

     /*

    QSqlQuery query("SELECT id FROM log");
    QSqlRecord rec = query.record();
    int nameCol;

    while (query.next()) {
        nameCol = rec.indexOf("call");
*/
    //qDebug() << "MainWindow::initialContestModeConfiguration: - 04" << endl;
    switch (contestMode) {

        case CQ_WW_SSB:
        //qDebug() << "MainWindow::initialContestModeConfiguration: - 05" << endl;
            query.exec("SELECT id FROM mode WHERE name='SSB'");
            query.next();
            if (query.isValid())
            {
                defaultMode = (query.value(0)).toInt() -1;
            }

            SRXLineEdit->setInputMask("09");
            STXLineEdit->setInputMask("09");

            contestQS << QString::number(world->getQRZARRLId(stationQRZ)) << QString::number(world->getQRZCqz(stationQRZ)) << world->getQRZContinentNumber(stationQRZ) <<  world->getQRZContinentNumber("K");
            contest = new ContestCQWWDXSSB(contestQS);

        break;
        case CQ_WW_CW:
        //qDebug() << "MainWindow::initialContestModeConfiguration: - 06" << endl;
            SRXLineEdit->setInputMask("09");
            STXLineEdit->setInputMask("09");


            contestQS << QString::number(world->getQRZARRLId(stationQRZ)) << QString::number(world->getQRZCqz(stationQRZ)) << world->getQRZContinentNumber(stationQRZ) <<  world->getQRZContinentNumber("K");
            //contest = new ContestCQWWDXSSB(contestQS);

        break;
        default:
            //qDebug() << "MainWindow::initialContestModeConfiguration: - 07" << endl;
        break;
    }


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

    query.next();
    if (query.isValid())
    {

    QSqlRecord rec = query.record();


    // ADD THE DATA THAT IS PRESENT IN ALL THE MODES

    nameCol = rec.indexOf("call");
    aux1 = (query.value(nameCol)).toString();
    qrzLineEdit->setText(aux1);
    currentQrz = aux1;
    currentEntity = world->getQRZARRLId(currentQrz);


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
    queryAux.next();
    if (queryAux.isValid())
    {
        aux1 = (queryAux.value(0)).toString();
        bandComboBox->setCurrentIndex(bandComboBox->findText(aux1));
    }
    else
    {
        bandComboBox->setCurrentIndex(defaultBand);
    }

    nameCol = rec.indexOf("modeid");
    aux1 = (query.value(nameCol)).toString();

    stringQuery = QString("SELECT name FROM mode WHERE id ='%1'").arg(aux1);
    queryAux.exec(stringQuery);
    queryAux.next();
    if (queryAux.isValid())
    {
        aux1 = (queryAux.value(0)).toString();
        modeComboBox->setCurrentIndex(modeComboBox->findText(aux1));
    }
    else
    {
        modeComboBox->setCurrentIndex(defaultMode);
    }

    nameCol = rec.indexOf("rst_sent");
    aux1 = (query.value(nameCol)).toString();
    rstTXLineEdit->setText(aux1);

    nameCol = rec.indexOf("rst_rcvd");
    aux1 = (query.value(nameCol)).toString();
    rstRXLineEdit->setText(aux1);

    //qDebug() << "MainWindow::qsoToEdit: - before switch"  << endl;

    switch (contestMode) {

        case CQ_WW_SSB:


        break;
        case CQ_WW_CW:

        break;
        default:

        //qDebug() << "MainWindow::qsoToEdit: - in default"  << endl;

        nameCol = rec.indexOf("qsl_via");
        aux1 = (query.value(nameCol)).toString();
        qslViaLineEdit->setText(aux1);

        nameCol = rec.indexOf("qslmsg");
        aux1 = (query.value(nameCol)).toString();
        qslmsgTextEdit->setText(aux1);

        nameCol = rec.indexOf("comment");
        aux1 = (query.value(nameCol)).toString();
        if (aux1.length()>0)
        {
            commentLineEdit->setText(aux1);
        }


        //nameCol = rec.indexOf("notes");
        //aux1 = (query.value(nameCol)).toString();
        //notesTextEdit->setText(aux1);

        nameCol = rec.indexOf("name");
        aux1 = (query.value(nameCol)).toString();
        nameLineEdit->setText(aux1);

        nameCol = rec.indexOf("qth");
        aux1 = (query.value(nameCol)).toString();
        qthLineEdit->setText(aux1);

        nameCol = rec.indexOf("gridsquare");
        aux1 = (query.value(nameCol)).toString();
        locatorLineEdit->setText(aux1);

        nameCol = rec.indexOf("operator");
        aux1 = (query.value(nameCol)).toString();
        operatorLineEdit->setText(aux1);

        nameCol = rec.indexOf("station_callsign");
        aux1 = (query.value(nameCol)).toString();
        stationCallSignLineEdit->setText(aux1);

        nameCol = rec.indexOf("my_gridsquare");
        aux1 = (query.value(nameCol)).toString();
        myLocatorLineEdit->setText(aux1);

        nameCol = rec.indexOf("tx_pwr");        
        aux1 = (query.value(nameCol)).toString();
        testValueDouble = aux1.toDouble();
        if (testValueDouble >=0)
        {
            myPowerSpinBox->setValue(testValueDouble);
        }
        else
        {
            myPowerSpinBox->setValue(0.0);
        }

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

        if ((testValueDouble >=0) && (testValueDouble <= txFreqSpinBox->maximum()) )
        {
            txFreqSpinBox->setValue(testValueDouble);
            //qDebug() << "MainWindow::qsoToEdit: Freq - OverFlow "  << endl;

            //freqQLCDNumber->display(testValueDouble);
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

        if ((testValueDouble >=0) && (testValueDouble <= rxFreqSpinBox->maximum()) )
        {
            rxFreqSpinBox->setValue(testValueDouble);
            //qDebug() << "MainWindow::qsoToEdit: Freq_RX - OverFlow "  << endl;

            //freqQLCDNumber->display(testValueDouble);
        }
        else
        {
            //qDebug() << "MainWindow::qsoToEdit: Freq_RX - OK "  << endl;
            rxFreqSpinBox->setValue(0);
        }

       // myPowerSpinBox->setValue(aux1.toDouble());

        //QSL SENT

        nameCol = rec.indexOf("qsl_sent");
        aux1 = (query.value(nameCol)).toString();

        if (  (aux1.toUpper()) == "Y" )
        {
            //qDebug() << "MainWindow::qsoToEdit: - QSL Sent "  << endl;
            qslSentComboBox->setCurrentIndex( qslSentComboBox->findText(tr("Y-Yes")) );

            nameCol = rec.indexOf("qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslSentQDateEdit->setEnabled(true);
                //qslSentViaComboBox->setEnabled(true);
                qslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }
              else if ( (aux1.toUpper()) == "N")
        {
            qslSentComboBox->setCurrentIndex( qslSentComboBox->findText(tr("N-No") ) );
        }
        else if ((aux1.toUpper()) == "R")
        {
            qslSentComboBox->setCurrentIndex( qslSentComboBox->findText(tr("R-Requested") ) );
        }
        else if ((aux1.toUpper()) == "Q")
        {
            qslSentComboBox->setCurrentIndex( qslSentComboBox->findText(tr("Q-Queued")) );

            nameCol = rec.indexOf("qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslSentQDateEdit->setEnabled(true);
                //qslSentViaComboBox->setEnabled(true);
                qslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }
        else if ((aux1.toUpper()) =="I")
        {

            qslSentComboBox->setCurrentIndex( qslSentComboBox->findText(tr("I-Ignore")) );

            nameCol = rec.indexOf("qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslSentQDateEdit->setEnabled(true);
                //qslSentViaComboBox->setEnabled(true);
                qslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }

        }
        else
        {
            qslSentComboBox->setCurrentIndex( qslSentComboBox->findText(tr("N-No") ) );
        }

        nameCol = rec.indexOf("qsl_sent_via");
        aux1 = (query.value(nameCol)).toString();

        if (  (aux1.toUpper()) == "B" )
        {
             qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("B-Bureau"))) ;

        }
        else if (  (aux1.toUpper()) == "D" ){
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("D-Direct"))) ;
        }
        else if (  (aux1.toUpper()) == "E" ){
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("E-Electronic"))) ;
        }
        else if (  (aux1.toUpper()) == "M" ){
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("M-Manager"))) ;
        }
        else
        {
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("B-Bureau"))) ;
        }

    //QSL RECEPTION

        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
        // tr("B-Bureau") << tr("D-Direct") << tr("E-Electronic") << tr("M-Manager");
        //QSLRDATE: (only valid if QSL_RCVD is Y, I, or V)


        nameCol = rec.indexOf("qsl_rcvd");
        aux1 = (query.value(nameCol)).toString();

        if (  (aux1.toUpper()) == "Y" )
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("Y-Yes")) );

            nameCol = rec.indexOf("qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslRecQDateEdit->setEnabled(true);
                qslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }

        else if ( (aux1.toUpper()) == "N")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("N-No") ) );
        }

        else if ((aux1.toUpper()) == "R")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("R-Requested") ) );
        }

        else if ((aux1.toUpper()) =="I")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("I-Ignore")) );

            nameCol = rec.indexOf("qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslRecQDateEdit->setEnabled(true);
                qslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }

        }

        else if ((aux1.toUpper()) == "V")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("V-Verified") ) );

            nameCol = rec.indexOf("qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslRecQDateEdit->setEnabled(true);
                qslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }


        else
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("N-No") ) );
        }

        nameCol = rec.indexOf("qsl_rcvd_via");
        aux1 = (query.value(nameCol)).toString();

        if (  (aux1.toUpper()) == "B" )
        {
             qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("B-Bureau"))) ;

        }
        else if (  (aux1.toUpper()) == "D" ){
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("D-Direct"))) ;
        }
        else if (  (aux1.toUpper()) == "E" ){
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("E-Electronic"))) ;
        }
        else if (  (aux1.toUpper()) == "M" ){
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("M-Manager"))) ;
        }
        else
        {
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("B-Bureau"))) ;
        }


    //TODO: BUG: When something is selected while modifying the QSL is deleted???

        //EQSL_QSL_SENT: {Y, N, R, Q, I}
        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
       //EQSL_QSLSDATE (only valid if EQSL_SENT is Y, Q, or I)

        nameCol = rec.indexOf("eqsl_qsl_sent");
        aux1 = (query.value(nameCol)).toString();

        if (  (aux1.toUpper()) == "Y" )
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("Y-Yes")) );

            nameCol = rec.indexOf("eqsl_qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eqslSentQDateEdit->setEnabled(true);
                eqslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }

        else if ( (aux1.toUpper()) == "N")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("N-No") ) );
        }

        else if ((aux1.toUpper()) == "R")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("R-Requested") ) );
        }

        else if ((aux1.toUpper()) =="Q")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("Q-Queued")) );

            nameCol = rec.indexOf("eqsl_qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eqslSentQDateEdit->setEnabled(true);
                eqslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }

        }

        else if ((aux1.toUpper()) == "I")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("I-Ignore") ) );

            nameCol = rec.indexOf("eqsl_qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eqslSentQDateEdit->setEnabled(true);
                eqslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }


        else
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("N-No") ) );
        }


        //E-QSL RECEPTION

            // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
            // EQSL_QSL_RCVD: {Y, N, R, I, V}
            // EQSL_QSLRDATE: (only valid if EQSL_RCVD is Y, I, or V)


            nameCol = rec.indexOf("eqsl_qsl_rcvd");
            aux1 = (query.value(nameCol)).toString();

            if (  (aux1.toUpper()) == "Y" )
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("Y-Yes")) );

                nameCol = rec.indexOf("eqsl_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eqslRecQDateEdit->setEnabled(true);
                    eqslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }

            else if ( (aux1.toUpper()) == "N")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("N-No") ) );
            }

            else if ((aux1.toUpper()) == "R")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("R-Requested") ) );
            }

            else if ((aux1.toUpper()) =="I")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("I-Ignore")) );

                nameCol = rec.indexOf("eqsl_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eqslRecQDateEdit->setEnabled(true);
                    eqslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }

            }

            else if ((aux1.toUpper()) == "V")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("V-Verified") ) );

                nameCol = rec.indexOf("eqsl_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eqslRecQDateEdit->setEnabled(true);
                    eqslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }


            else
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("N-No") ) );
            }

            //LOTW_QSL_SENT: {Y, N, R, Q, I}
            // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
           //LOTW_QSLSDATE (only valid if LOTW_SENT is Y, Q, or I)

            nameCol = rec.indexOf("lotw_qsl_sent");
            aux1 = (query.value(nameCol)).toString();

            if (  (aux1.toUpper()) == "Y" )
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("Y-Yes")) );

                nameCol = rec.indexOf("lotw_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    lotwSentQDateEdit->setEnabled(true);
                    lotwSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }

            else if ( (aux1.toUpper()) == "N")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("N-No") ) );
            }

            else if ((aux1.toUpper()) == "R")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("R-Requested") ) );
            }

            else if ((aux1.toUpper()) =="Q")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("Q-Queued")) );

                nameCol = rec.indexOf("lotw_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    lotwSentQDateEdit->setEnabled(true);
                    lotwSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }

            }

            else if ((aux1.toUpper()) == "I")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("I-Ignore") ) );

                nameCol = rec.indexOf("lotw_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    lotwSentQDateEdit->setEnabled(true);
                    lotwSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }


            else
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("N-No") ) );
            }


            //E-QSL RECEPTION

                // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
                // lotw_QSL_RCVD: {Y, N, R, I, V}
                // lotw_QSLRDATE: (only valid if lotw_RCVD is Y, I, or V)


                nameCol = rec.indexOf("lotw_qsl_rcvd");
                aux1 = (query.value(nameCol)).toString();

                if (  (aux1.toUpper()) == "Y" )
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("Y-Yes")) );

                    nameCol = rec.indexOf("lotw_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                    {
                        lotwRecQDateEdit->setEnabled(true);
                        lotwRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                    }
                }

                else if ( (aux1.toUpper()) == "N")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("N-No") ) );
                }

                else if ((aux1.toUpper()) == "R")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("R-Requested") ) );
                }

                else if ((aux1.toUpper()) =="I")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("I-Ignore")) );

                    nameCol = rec.indexOf("lotw_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                    {
                        lotwRecQDateEdit->setEnabled(true);
                        lotwRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                    }

                }

                else if ((aux1.toUpper()) == "V")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("V-Verified") ) );

                    nameCol = rec.indexOf("lotw_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                    {
                        lotwRecQDateEdit->setEnabled(true);
                        lotwRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                    }
                }


                else
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("N-No") ) );
                }

                //qDebug() << "MainWindow::qsoToEdit: - just before IOTA"  << endl;

                nameCol = rec.indexOf("iota");
                aux1 = (query.value(nameCol)).toString();

                aux1 = awards->checkIfValidIOTA(aux1);

                //qDebug() << "MainWindow::qsoToEdit: IOTA-CheckIfValidIOTA-1 " << aux1 << endl;
                if ((aux1.length())==6){
                    QStringList values = aux1.split("-", QString::SkipEmptyParts);
                    //qDebug() << "MainWindow::qsoToEdit: IOTA " << aux1 << endl;

                    iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(values.at(0) ) );
                    iotaNumberLineEdit->setText(values.at(1));

                }

                //qDebug() << "MainWindow::qsoToEdit: - in default - 100: " << QString::number(currentEntity)  << endl;
                showEntityInfo(currentEntity);
                //qDebug() << "MainWindow::qsoToEdit: - in default - 101"  << endl;

                QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
                _qs.clear();
                //TODO: The band sometimes fails here. Check
                _qs << currentQrz << QString::number(bandComboBox->currentIndex()) << QString::number(modeComboBox->currentIndex()) << QString::number(currentLog);
                //qDebug() << "MainWindow::qsoToEdit: - in default - 104"  << endl;
                showStatusOfDXCC(_qs);

//qDebug() << "MainWindow::qsoToEdit: - in default - 105"  << endl;
                //qDebug() << "MainWindow::qsoToEdit: - just after de IOTA"  << endl;
        break;
        } // Closesthe switch (contest or default)
    //qDebug() << "MainWindow::qsoToEdit: - in default - 106"  << endl;
    } //Closes the next.isValid
    //qDebug() << "MainWindow::qsoToEdit: - in default - END"  << endl;
}

void MainWindow::slotIOTAComboBoxChanged()
{
    //qDebug() << "MainWindow::slotIOTAComboBoxChanged" << endl;
    //iotaNumberLineEdit->setEnabled(true);
}

void MainWindow::slotQSLRecvComboBoxChanged(){
    //qDebug() << "MainWindow::slotQSLRecvComboBoxChanged" << endl;

//QSLRDATE (only valid if QSL_RCVD is Y-0, I-3, or V-4)
//Y-Yes-0
//N-No-1
//R-Requested-2
//I-Ignore-3
//V-Verified-4

    int i = qslRecComboBox->currentIndex();

    switch (i)
    {
        case 0:
            qslRecQDateEdit->setEnabled(true);
            qslRecViaComboBox->setEnabled(true);
            qslRecQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            qslRecQDateEdit->setEnabled(true);
        break;
        case 4:
            qslRecQDateEdit->setEnabled(true);
        break;
        default: //NO
            qslRecQDateEdit->setEnabled(false);
            qslRecViaComboBox->setEnabled(false);


        break;


    }

}


void MainWindow::slotQSLSentComboBoxChanged(){
    //qDebug() << "MainWindow::slotQSLSentComboBoxChanged" << endl;

    int i = qslSentComboBox->currentIndex();
//{Y, N, R, I, V}
    //(QSLSDATE is only valid if QSL_SENT is Y-0, Q-3, or I-4)
    // Y-Yes = 0
    // N-No = 1
    // R-Requested = 2
    // Q-Queued = 3
    // I-Ignore = 4
    switch (i)
    {
        case 0:
            qslSentQDateEdit->setEnabled(true);
            qslSentViaComboBox->setEnabled(true);
            qslSentQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            qslSentQDateEdit->setEnabled(true);
        break;
        case 4:
            qslSentQDateEdit->setEnabled(true);
        break;

        default: //NO
            qslSentQDateEdit->setEnabled(false);
            qslSentViaComboBox->setEnabled(false);
        break;
    }
}


void MainWindow::sloteQSLRecvComboBoxChanged(){
    //qDebug() << "MainWindow::sloteQSLRecvComboBoxChanged" << endl;

//QSLRDATE (only valid if QSL_RCVD is Y-0, I-3, or V-4)
//Y-Yes-0
//N-No-1
//R-Requested-2
//I-Ignore-3
//V-Verified-4

    int i = eqslRecComboBox->currentIndex();

    switch (i)
    {
        case 0:
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            eqslRecQDateEdit->setEnabled(true);
        break;
        case 4:
            eqslRecQDateEdit->setEnabled(true);
        break;
        default: //NO
            eqslRecQDateEdit->setEnabled(false);
        break;


    }

}


void MainWindow::sloteQSLSentComboBoxChanged(){
    //qDebug() << "MainWindow::sloteQSLSentComboBoxChanged" << endl;

    int i = eqslSentComboBox->currentIndex();
//{Y, N, R, I, V}
    //(QSLSDATE is only valid if QSL_SENT is Y-0, Q-3, or I-4)
    // Y-Yes = 0
    // N-No = 1
    // R-Requested = 2
    // Q-Queued = 3
    // I-Ignore = 4
    switch (i)
    {
        case 0:
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            eqslSentQDateEdit->setEnabled(true);
        break;
        case 4:
            eqslSentQDateEdit->setEnabled(true);
        break;

        default: //NO
            eqslSentQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindow::showInfoFromLocators(const QString _loc1, const QString _loc2)
{// Local / DX
    //qDebug() << "MainWindow::showInfoFromLocators: " << _loc1 << "/" << _loc2 << endl;
    QString lloc = _loc1.toUpper();
    QString ldx = _loc2.toUpper();

    if ( locator->isValidLocator(lloc)  )
    {
        if ( locator->isValidLocator(ldx)  )
        {

            int beam = locator->getBeamBetweenLocators(lloc, ldx);


            gradShortLabel->setText( QString::number(beam) );

             if (beam >= 180)
             {
                   gradLongLabel->setText( QString::number(beam -180 ) );
             }
             else
             {
                 gradLongLabel->setText( QString::number(beam + 180 ) );
             }

             distShortLabel->setText( QString::number( locator->getDistanceBetweenLocators(lloc, ldx, imperialSystem) ) );
             distLongLabel->setText( QString::number( 40000 - locator->getDistanceBetweenLocators(lloc, ldx, imperialSystem) ) );
        }
        else
        {
            clearInfoFromLocators();
            return;
        }
    }
    else
    {
        clearInfoFromLocators();
        return ;
    }
}

void MainWindow::slotLocatorTextChanged()
{
    //qDebug() << "MainWindow::slotLocatorTextChanged: " << locatorLineEdit->text() << endl;
    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
        showInfoFromLocators(myLocator, dxLocator);
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


void MainWindow::clearInfoFromLocators()
{
    //qDebug() << "MainWindow::clearInfoFromLocators" << endl;
    gradShortLabel->setText( "0" );
    gradLongLabel->setText( "0" );
    distShortLabel->setText( "0" );
    distLongLabel->setText( "0" );
}

void MainWindow::showEntityInfo(const int _enti)
{
    //qDebug() << "MainWindow::showEntityInfo" << QString::number(_enti) << endl;

    if (_enti<=0)
    {
        return;
    }

    infoLabel2->setText(world->getEntityName(_enti));
    continentLabel->setText( world->getContinentShortName(_enti) );
    prefixLabel->setText( world->getEntityMainPrefix(_enti));

    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
    }
    else
    {
        dxLocator = world->getLocator(_enti);
    }

    showInfoFromLocators (myLocator, dxLocator);

    int i = world->getEntityCqz(_enti);


    if( i > 0 )
    {
        cqzLabel->setText( QString::number(i) );
    }
    else
    {
        cqzLabel->setText("0");
    }
    i = world->getEntityItuz(_enti);

    if ( i > 0  )
    {
        ituzLabel->setText( QString::number(i) );
    }
    else
    {
        ituzLabel->setText("0");
    }
}

void MainWindow::showStatusOfDXCC(const QStringList _qs)
{
    //qDebug() << "MainWindow::showStatusOfDXC: QRZ: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
    // Receives:  QStringList _qs;
    //_qs << QRZ << BandId << ModeId << lognumber;

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
        clearBandLabels();
        infoLabel1->setText(tr("No message"));
        return;
    }
    // Set the status bar with the appropriate message
    int status = awards->getDXStatus (_qs);

    //qDebug() << "MainWindow::showStatusOfDXC: " << QString::number(status) << endl;

    switch (status) {

        case 0:
        //qDebug() << "MainWindow::showStatusOfDXC: (new one) QRZ: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
            infoLabel1->setText(tr("New One!"));
        break;
        case 1:
        //qDebug() << "MainWindow::showStatusOfDXC: (needed) QRZ: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
            infoLabel1->setText(tr("Needed QSO!"));
        break;
        case 2:
        //qDebug() << "MainWindow::showStatusOfDXC: (Worked) QRZ: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
            infoLabel1->setText(tr("Worked but not confirmed!"));
        break;
        case 3:
        //qDebug() << "MainWindow::showStatusOfDXC: (Confirmed) QRZ: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
            infoLabel1->setText(tr("Confirmed"));
        break;
        default:
        //qDebug() << "MainWindow::showStatusOfDXC: (noMessage) QRZ: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
            infoLabel1->setText(tr("No message"));
        break;
    }


    //Run all the bandLabel1-12 to set the appropriate color
    // qs.at(0) = QRZ
    bandLabel1->setStyleSheet(getStyleColorToLabelFromBand(bandLabel1->text(), _qs.at(0)));    
    bandLabel2->setStyleSheet(getStyleColorToLabelFromBand(bandLabel2->text(), _qs.at(0)));
    bandLabel3->setStyleSheet(getStyleColorToLabelFromBand(bandLabel3->text(), _qs.at(0)));
    bandLabel4->setStyleSheet(getStyleColorToLabelFromBand(bandLabel4->text(), _qs.at(0)));
    bandLabel5->setStyleSheet(getStyleColorToLabelFromBand(bandLabel5->text(), _qs.at(0)));
    bandLabel6->setStyleSheet(getStyleColorToLabelFromBand(bandLabel6->text(), _qs.at(0)));
    bandLabel7->setStyleSheet(getStyleColorToLabelFromBand(bandLabel7->text(), _qs.at(0)));
    bandLabel8->setStyleSheet(getStyleColorToLabelFromBand(bandLabel8->text(), _qs.at(0)));
    bandLabel9->setStyleSheet(getStyleColorToLabelFromBand(bandLabel9->text(), _qs.at(0)));
    bandLabel10->setStyleSheet(getStyleColorToLabelFromBand(bandLabel10->text(), _qs.at(0)));
    bandLabel11->setStyleSheet(getStyleColorToLabelFromBand(bandLabel11->text(), _qs.at(0)));
    bandLabel12->setStyleSheet(getStyleColorToLabelFromBand(bandLabel12->text(), _qs.at(0)));
}

QString MainWindow::getStyleColorToLabelFromBand(const QString _b, const QString _q)
{
    //qDebug() << "MainWindow::getStyleColorToLabelFromBand: " << _b << "/" << _q << endl;
   QStringList _qs;
    _qs.clear();
    _qs << _q << QString::number(db->getBandIdFromName(_b)) << QString::number(-1) << QString::number(currentLog);
    //TODO: Check if we can know the mode and replace the -1
    //qDebug() << "MainWindow::getStyleColorToLabelFromBand (Band/background-color): " << _b << (awards->getQRZDXStatusColor(_qs)).name()  << endl;
    return "* { background-color: " + (awards->getQRZDXStatusColor(_qs)).name() + "; }";

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

    aux = "SELECT count(id) FROM log";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
    qsoWorkedQLCDNumber->display(_num);
    _num = 0;

    aux = "SELECT count(id) FROM log WHERE qsl_rcvd='Y'";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
    qsoConfirmedQLCDNumber->display(_num);
    _num = 0;
/*
    aux = "SELECT count (*) from  (SELECT DISTINCT dxcc FROM awarddxcc)";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
*/
    dxccWorkedQLCDNumber->display(awards->getDXCCWorked(currentLog));

    _num = 0;
/*
    aux = "SELECT COUNT (*) FROM (SELECT DISTINCT dxcc FROM awarddxcc WHERE confirmed='1')";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
*/
    dxccConfirmedQLCDNumber->display(awards->getDXCCConfirmed(currentLog));
    _num = 0;
/*
    aux = "SELECT count (*) from  (SELECT DISTINCT cqz FROM awardwaz)";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
*/
    wazWorkedQLCDNumber->display(awards->getWAZWorked(currentLog));
    _num = 0;
/*
    aux = "SELECT COUNT (*) FROM (SELECT DISTINCT cqz FROM awardwaz WHERE confirmed='1')";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
*/
    wazConfirmedQLCDNumber->display(awards->getWAZConfirmed(currentLog));

}

void MainWindow::fillQSOData()
{ // Updates all QSO with the dxcc, CQZ, ... if empty.
    //qDebug() << "MainWindow::fillQSOData" << endl;

    QSqlQuery query("SELECT call, bandid, modeid, qso_date, time_on, lognumber, confirmed, id, cqz, ituz, dxcc FROM log");
    QSqlQuery query1;
    QSqlRecord rec = query.record();
    int nameCol;
    QString aux, queryString;
    QString _call, _bandid, _modeid, _tdate, _ttime, _lognumber, _id, aux1, updateString, _confirmed;
    bool toModify = false;
    bool noMoreQso = false;
    bool sqlOK;

    int numberOfQsos = 0;
    int i = 0;

    aux = "SELECT count(id) FROM log";
    query1.exec(aux);
    query1.next();
    if (query1.isValid())
    {
        numberOfQsos = (query1.value(0)).toInt();
    }


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
            nameCol = rec.indexOf("confirmed");
            if ( (query.value(nameCol)).isValid() )
            {
                _confirmed = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("id");
            if ( (query.value(nameCol)).isValid() )
            {
                _id = (query.value(nameCol)).toString();
            }
            //qDebug() << "MainWindow::fillQSOData: ID: " << _id << endl;
            //TODO: Prepare this query
            updateString = "UPDATE log SET call = '" + _call + "', bandid = '" + _bandid + "', modeid = '" + _modeid + "', qso_date = '" + _tdate + "', time_on = '" + _ttime + "', lognumber = '" + _lognumber  + "', confirmed = '" + _confirmed + "'";

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


void MainWindow::slotUpdateCTYDAT()
{
    //qDebug() << "MainWindow::slotUpdateCTYDAT" << endl;
    downloadCTYFile();


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

    aux = "SELECT count(id) FROM log";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _numberOfQsos = (query.value(0)).toInt();
    }



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
        query.exec(stringQuery);
        QSqlRecord rec = query.record();

        while ((query.next()) && (!cancelPrinting))
        {
            if (query.isValid())
            {

                textTable->appendRows(1);
                row++;
                _qsos++;
                aux = tr("Printing the log...\n QSO: ")  + QString::number(_qsos) + "/" + QString::number(_numberOfQsos);
                progress.setLabelText(aux);
                progress.setValue(_qsos);

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
                query1.exec(stringQuery);
                query1.next();
                if (query1.isValid())
                {
                    cursor = textTable->cellAt(row, 6).firstCursorPosition();
                    cursor.insertText((query1.value(0)).toString());
                }

                nameCol = rec.indexOf("modeid");
                aux = (query.value(nameCol)).toString();
                stringQuery = QString("SELECT name FROM mode WHERE id='%1'").arg(aux);
                query1.exec(stringQuery);
                query1.next();
                if (query1.isValid())
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
//UPDATE CTY.CSV


bool MainWindow::downloadCTYFile()
{
    //qDebug() << "MainWindow::downloadCTYFile" << endl;

    QMessageBox msgBox;
    msgBox.setText("You are going to download the last CTY.CVS file. Do you want to proceed?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
          // Yes was clicked
            return downloadCtyDatFile();
          break;
      case QMessageBox::No:
          // No Save was clicked
            return false;
          break;
      default:
          // should never be reached
          break;
    }
    return false;
}

bool MainWindow::downloadCtyDatFile()
{
    //qDebug() << "MainWindow::downloadCtyDatFile" << endl;
    //http://www.country-files.com/cty/cty.csv
    QString urld = QString();
    switch (contestMode)
    {
    case NoContest:
        urld = "http://www.country-files.com/bigcty/cty.csv";
        break;
    default: // It is a contest!
        urld = "http://www.country-files.com/cty/cty.csv";
        break;
    }
    QUrl url ( urld.toLocal8Bit() );

    //QUrl url(  QString("http://www.country-files.com/cty/cty.csv").toLocal8Bit()  );

    request.setUrl(url);
    QNetworkReply *reply= manager.get(request);

    //qDebug() << "MainWindow::downloadCtyDatFile - END" << endl;

    return true;
}

void MainWindow::slotDownloadFinished(QNetworkReply *reply)
{
    //qDebug() << "MainWindow::downloadFinished" << endl;

    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        QString filename = saveFileName(url);
        if (saveToDisk(filename, reply))
            //printf("Download of %s succeeded (saved to %s)\n",
            //       url.toEncoded().constData(), qPrintable(filename));
        {
            QMessageBox msgBox;
            msgBox.setText(tr("The file has been downloaded."));
             msgBox.exec();
        }

    }


    reply->deleteLater();

}

bool MainWindow::saveToDisk(const QString &filename, QIODevice *data)
{
    //qDebug() << "MainWindow::saveToDisk" << endl;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

QString MainWindow::saveFileName(const QUrl &url)
{
    //qDebug() << "MainWindow::saveFileName" << endl;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}
///UPDATE CTY.CSV

 void MainWindow::slotToolSearchRequestedQSLToSend()
 {

    //qDebug() << "MainWindow::slotToolSearchRequestedQSLToSend" << endl;

     int nameCol = -1;
     QString _call, _dateTime, _band, _mode, _freq, _qsltx, _qslrx, _id, _stationcallsign;
     QFont font;
     font.setBold(true);
     QColor color;
     QStringList q;

     QString stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, id FROM log  WHERE qsl_sent='R'AND lognumber='%1'").arg(currentLog);

     searchResultsTreeWidget->clear();
     QSqlQuery query(stringQuery);
     QSqlRecord rec = query.record();

     if (!query.exec())
     {
         //TODO: Control the error!!
     }
     else
     {
         while(query.next())
         {
         if (query.isValid())
         {
             QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
             nameCol = rec.indexOf("call");
             _call= (query.value(nameCol)).toString();
             nameCol = rec.indexOf("qso_date");
             _dateTime = (query.value(nameCol)).toString();
             nameCol = rec.indexOf("time_on");
             _dateTime = _dateTime + "-" +(query.value(nameCol)).toString();

             nameCol = rec.indexOf("bandid");
             _freq = (query.value(nameCol)).toString();
             _band = db->getBandNameFromNumber( _freq.toInt() );

             nameCol = rec.indexOf("modeid");
             _mode = db->getModeNameFromNumber( (query.value(nameCol)).toInt() );

             nameCol = rec.indexOf("qsl_sent");
             _qsltx = (query.value(nameCol)).toString();
             if (_qsltx.length()<1)
             {
                 _qsltx = "N";
             }

             nameCol = rec.indexOf("qsl_rcvd");
             _qslrx = (query.value(nameCol)).toString();
             if (_qslrx.length()<1)
             {
                 _qslrx = "N";
             }

             nameCol = rec.indexOf("id");
             _id= (query.value(nameCol)).toString();

             nameCol = rec.indexOf("station_callsign");
             _stationcallsign = (query.value(nameCol)).toString();
             if (_stationcallsign.length()<3)
             {
                 _stationcallsign = stationQRZ;
             }


             q.clear();
             q << _call << _freq << _mode << QString::number(currentLog);
             //q << _call << _freq << "-1" << QString::number(currentLog);

             color = awards->getQRZDXStatusColor(q);
             item->setText(0, _call);
             item->setText(1, _dateTime);
             item->setText(2, _band);
             item->setText(3, _mode);
             item->setText(4, _qsltx);
             item->setText(5, _qslrx);
             if (stationCallSignShownInSearch)
             {
                 item->setText(6, _stationcallsign);
                 item->setText(7, _id);
             }
             else
             {
                item->setText(6, _id);
             }

             item->setForeground(0, QBrush(color));

         }
         else
         {

         }
     }
         qslingNeeded = true;
         dxUpRightTab->setCurrentIndex(2);
     }



 }

void MainWindow::slotToolSearchNeededQSLToSend()
{
    //qDebug() << "MainWindow::slotToolSearchQSLToSend - TO PREPARE THE QUERY and optimize the function" << endl;
    int nameCol = -1;
    QString _call, _dateTime, _band, _mode, _freq, _qsltx, _qslrx, _id, _stationcallsign;
    QFont font;
    font.setBold(true);
    QColor color;
    QStringList q;



    QString stringQuery = ("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, log.id FROM log JOIN awarddxcc ON awarddxcc.qsoid=log.id WHERE awarddxcc.confirmed='0' AND log.qsl_sent!='Y';");


    searchResultsTreeWidget->clear();
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record();

    if (!query.exec())
    {
    //TODO: Control the error!!
    }
    else
    {
        while(query.next())
        {
        if (query.isValid())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
            nameCol = rec.indexOf("call");
            _call= (query.value(nameCol)).toString();
            nameCol = rec.indexOf("qso_date");
            _dateTime = (query.value(nameCol)).toString();
            nameCol = rec.indexOf("time_on");
            _dateTime = _dateTime + "-" +(query.value(nameCol)).toString();

            nameCol = rec.indexOf("bandid");
            _freq = (query.value(nameCol)).toString();
            _band = db->getBandNameFromNumber( _freq.toInt() );

            nameCol = rec.indexOf("modeid");
            _mode = db->getModeNameFromNumber( (query.value(nameCol)).toInt() );
            //qDebug() << "MainWindow::slotToolSearchQSLToSend: mode " << QString::number((query.value(nameCol)).toInt()) << endl;

            nameCol = rec.indexOf("qsl_sent");
            _qsltx = (query.value(nameCol)).toString();
            if (_qsltx.length()<1)
            {
                _qsltx = "N";
            }

            nameCol = rec.indexOf("qsl_rcvd");
            _qslrx = (query.value(nameCol)).toString();
            if (_qslrx.length()<1)
            {
                _qslrx = "N";
            }

            if (stationCallSignShownInSearch)
            {

                nameCol = rec.indexOf("station_callsign");
                _stationcallsign = (query.value(nameCol)).toString();
                if (_stationcallsign.length()<3)
                {
                    _stationcallsign = stationQRZ;
                }
            }

            nameCol = rec.indexOf("id");
            _id= (query.value(nameCol)).toString();



            q.clear();
            q << _call << _freq << _mode << QString::number(currentLog);
            //q << _call << _freq << "-1" << QString::number(currentLog);

            color = awards->getQRZDXStatusColor(q);
            //color = workedColor;
            item->setText(0, _call);
            //item->setFont(0,font);
            item->setText(1, _dateTime);
            item->setText(2, _band);
            item->setText(3, _mode);
            item->setText(4, _qslrx);
            item->setText(5, _qsltx);
            if (stationCallSignShownInSearch)
            {
                item->setText(7, _stationcallsign);
                item->setText(7, _id);
            }
            else
            {
                item->setText(6, _id);
            }

            item->setForeground(0, QBrush(color));

        }
        else
        {
        //TODO: Check what is happening here!
        }
    }
        qslingNeeded = true;
        dxUpRightTab->setCurrentIndex(2);
        //dxUpRightTab->setCurrentWidget(2);
        //searchBoxLineEdit->setFocus(Qt::OtherFocusReason);
    }


}

//DX-CLUSTER - DXCLUSTER

void MainWindow::slotAnalyzeDxClusterSignal(QStringList ql)
{
    //qDebug() << "MainWindow::slotAnalyzeDxClusterSignal: 1: " << ql.at(0) <<"/1: " << ql.at(1) << "/2: " << ql.at(2) << endl;
    QStringList qls;
    qls.clear();

    if (ql.length()==3)
    {
        if ((ql.at(2)) == "double")
        {
            clusterSpotToLog(ql.at(0), ql.at(1));
        }
        else if ((ql.at(2)) == "selected")
        {
            showEntityInfo(   world->getQRZARRLId(ql.at(0))  );

            // Becareful, he Frecuency arrives in KHz instead of bandid!!
            // db.getBandFromFreq expects a MHz!
            //(ql.at(1)).toDouble()

            qls << ql.at(0) << QString::number(db->getBandIdFromFreq(QString::number((ql.at(1).toDouble()/1000)))) << QString::number(-1) <<  QString::number(currentLog);
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
    if ( (_f > 0) && (_f <= txFreqSpinBox->maximum()))
    {
        return _f;
    }
        else
    {
        return 0;
    }
    return 0;
}

//void MainWindow::clusterSpotToLog(const QStringList _qs)
void MainWindow::clusterSpotToLog(const QString _call, const QString _freq)
{
    //qDebug() << "MainWindow::clusterSpotToLog: " << _call <<"/" << _freq << endl;

    QString _aux;
    double _freqN = (_freq.toFloat()) / 1000;
    qrzLineEdit->setText(_call);
    //qrzLineEdit->setText(_qs.at(0));

    txFreqSpinBox->setValue(checkFreqRanges(_freqN));
    //freqQLCDNumber->display(_freqN);

    _aux = QString::number(_freqN);

    //qDebug() << "MainWindow::clusterSpotToLog - Freq: " << _aux << endl;

    int _bandi = db->getBandIdFromFreq(_aux);
    //qDebug() << "MainWindow::clusterSpotToLog - Bandi: " << QString::number(_bandi) << endl;
    _aux = QString::number(_bandi);
    _aux = QString("SELECT name FROM band WHERE id ='%1'").arg(_aux);

    //qDebug() << "MainWindow::clusterSpotToLog - Band: " << _aux << endl;

    QSqlQuery query(_aux);
    query.next();
    if (query.isValid())
    {
        _aux = (query.value(0)).toString();
        bandComboBox->setCurrentIndex(bandComboBox->findText(_aux));
    }
    else
    {
        bandComboBox->setCurrentIndex(defaultBand);
    }

}
//DX-CLUSTER - DXCLUSTER

void MainWindow::updateQSLRecAndSent()
{
    //qDebug() << "MainWindow::updateQSLRecAndSent "  << endl;
    // Checks the log to fill all the qsl_rcvd and qsl_sent
    QSqlQuery query, query1;
    QString queryString, aux, idT;
    int nameCol=0;

    queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE lognumber='%1'").arg(currentLog);

    query.exec(queryString);

    QSqlRecord rec = query.record();

    while (query.next())
    {
        if (query.isValid())
        {
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
                    queryString = QString("UPDATE log SET qsl_rcvd='N' qsl_sent='N' WHERE id='%1'").arg(idT);
                    query1.exec(queryString);
                }
                else
                {
                    queryString = QString("UPDATE log SET qsl_rcvd='N' WHERE id='%1'").arg(idT);
                    query1.exec(queryString);

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
                    query1.exec(queryString);
                }
                else
                {

                }
            }
        }
    }
}

