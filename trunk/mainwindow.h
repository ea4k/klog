#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/***************************************************************************
                          mainwindow.h  -  description
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

#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtAlgorithms>

#include "database.h"
#include "setupdialog.h"
//#include "helpaboutdialog.h"
#include "aboutdialog.h"
#include "world.h"
#include "filemanager.h"
#include "contest.h"
#include "contest_cqwwdxssb.h"
#include "dataproxy.h"
#include "dataproxy_sqlite.h"
#include "locator.h"
#include "dxcluster.h"
#include "awards.h"
#include "mainwindowsattab.h"
#include "mainwindowmydatatab.h"
#include "mainwindowinputcomment.h"
#include "mainwindowinputothers.h"
#include "mainwindowinputeqsl.h"
#include "mainwindowinputqsl.h"
#include "elogclublog.h"
#include "utilities.h"
#include "downloadcty.h"
#include "dxccstatuswidget.h"
#include "softwareupdate.h"
#include "logmodel.h"
#include "logwindow.h"
#include "searchwidget.h"
#include "infowidget.h"
#include "showerrordialog.h"
#include "udpserver.h"




class QTimer;
class QDateTime;

class QPushButton;
class QLineEdit;
class QComboBox;
class QTimeEdit;
class QDateEdit;
//class QStatusBar;
class QMenuBar;
class QMenu;
class QMessageBox;
class QTextEdit;
class QProgressDialog;

class QPoint;

class QGroupBox;
class QTabWidget;
class QFrame;
class QTableView;




class QLabel;



enum
{
    Log_Id = 0,
    Log_Name = 1,
    Log_BandId = 2,
    Log_ModeId = 3,
    Log_DateId = 4,
    Log_TimeId = 5
};

enum
{
    DX = 0,
    NoContest = 0,
    CQ_WW_SSB = 1,
    CQ_WW_CW = 2,
    CQ_WPX_SSB = 3,
    CQ_WPX_CW = 4,
    CQ_WPX_RTTY = 5
};


class MainWindow : public  QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString _klogDir, const QString tversion);
     ~MainWindow();

private slots:
    //void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery);
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString queryFailed);
    void slotQRZReturnPressed();
    void slotQRZSpacePressed();
    void slotQRZTextChanged();
    void slotSRXTextChanged();
    void slotSTXTextChanged();
    void slotUpdateLocator(QString _loc);
    void slotLocatorTextChanged();
    void slotMyLocatorTextChanged();
    void slotFreqTXChanged();
    void slotFreqRXChanged();

    void slotSearchBoxTextChanged();
    void slotSearchToolNeededQSLToSend();
    void slotToolSearchRequestedQSLToSend();
    void slotToolSearchNeededQSLPendingToReceive();
    void slotToolSearchNeededQSLRequested();
    void slotToolLoTWMarkAllQueuedThisLog();
    void slotToolLoTWMarkAllQueued();
    void slotToolLoTWMarkAllYesThisLog();
    void slotToolLoTWMarkAllYes();
    //void slotSearchExportButtonClicked();
    //void slotSearchBoxSelectAllButtonClicked();
    //void slotSearchClearButtonClicked();
    //void slotSearchBoxSelectionChanged();
    //void slotSearchBoxReSearchButtonClicked();

    //void showMenuRightButtonSearchCreateActions();
    //void righButtonSearchMenu(const int trow);

    void slotModeComboBoxChanged();
    void slotBandComboBoxChanged();
    //void slotIOTAComboBoxChanged();
    void slotOperatingYearComboBoxChanged();
    void slotOKButtonClicked();
    void slotSpotItButtonClicked();
    void slotClearButtonClicked();
    void slotRefreshDXCCWidget();
    void slotUpdateTime();
    void slotLogWinShow();
    void slotLogRefresh();
    void slotScoreWinShow();
    void slotQSODelete(const int _id);

    void slotShowAwards();
    void slotUpdateStatusBar(const QString statusm);

    void slotSetup(const int _page=0);

    //void slotQsoDeleteFromSearch();
    //void slotQSLSentViaBureauFromSearch();
    //void slotQSLSentViaDirectFromSearch();
    //void slotQSLSentViaDirectMarkDXReqFromSearch();

    //void slotQSLSentViaBureuMarkDXReqFromSearch();
    //void slotQSLRecViaDirectFromSearch();
    //void slotQSLRecViaBureauFromSearch();
    //void slotQSLRecViaDirectMarkReqFromSearch();
    //void slotQSLRecViaBureauMarkReqFromSearch();
    //void slotQSLSentMarkAsRequested();
    //void slotQSLRecMarkAsRequested();
    //void slotQSOToEditFromSearch();

    void slotrstTXTextChanged();
    void slotrstRXTextChanged();
    void slotADIFExport();
    void slotLoTWImport();
    void slotLoTWExport();
    void slotADIFExportAll();
    void slotADIFImport();
    void slotRQSLExport();
    void slotCabrilloExport();
    //void slotQSLViaTextChanged();


//TODO: REMOVE EQSL
    //void slotQSLRecvComboBoxChanged();
    //void slotQSLSentComboBoxChanged();
    //void sloteQSLRecvComboBoxChanged();
    //void sloteQSLSentComboBoxChanged();
    //void slotLotwRecvComboBoxChanged();
    //void slotLotwSentComboBoxChanged();

    void slotSetPropMode(const QString _p);
    void slotFillEmptyDXCCInTheLog();
    void slotUpdateCTYDAT();
    void slotWorldReload();

    void slotExitFromSlotDialog(const int exitID);

    //void slotDownloadFinished(QNetworkReply *reply);

    void fillQSOData();

    void newFile();
    void openFile();
    bool saveFile(const QString _fileName);
    bool saveFileAs();
    bool slotOpenKLogFolder();

    void slotFilePrint();
    //void slotFileClose();

    //void slotHelpHelpAction();
    void slotHelpAboutAction();
    void slotHelpCheckUpdatesAction();
    void slotAboutQt();
       
    void slotRecalculateAwardsButtonClicked();

    // logpanel
    //void slotRighButtonFromLog( const QPoint& pos);
    void slotDoubleClickLog( const int _qsoID);
    //void slotDoubleClickLog( const QModelIndex & index);

    //SEARCH    
    void slotShowSearchWidget();                // The SearchWidget request being shown
    //void slotDoubleClickSearch( QTreeWidgetItem* item, int); // Double click on a QSO in the search box
    //void slotRighButtonSearch(const QPoint& pos);
    //void slotToolSearchNeededQSLToSend();
    //void slotToolSearchNeededQSLPendingToReceive();
    //void slotToolSearchNeededQSLRequested();
    //void slotToolSearchQSL(const int actionQSL);
    //SEARCH

    // CLUSTER
    void slotAnalyzeDxClusterSignal(const QStringList _qs);

    // CLUSTER
    //CLUBLOG
    void slotElogClubLogShowMessage(const QString _s);
    void slotElogClubLogProcessAnswer(const int _i, const int _qID);
    void slotElogClubLogDisable(const bool _b);
    //CLUBLOG

    void slotShowSoftUpdateResults(const bool _b);   // Software Update: Receives the signal to see if it is needed or not to update

    //SATELLITE
    void slotSatBandTXComboBoxChanged(const QString _q);

    void slotDefineNewBands(const QStringList _bands);
    void slotChangeRXFreq(const QString _f);
    void slotChangeTXFreq(const QString _f);
	
    //DXCCWIDGET
    void slotShowQSOFromDXCCWidget(const int _q);
    void slotShowQSOsFromDXCCWidget(QList<int> _qsos);

    //UDP Server (WXJT-x)
    void slotWSJXstatusFromUDPServer(const int _type, const QString _dxcall, const double _freq, const QString _mode,
                                 const QString _report, const QString _de_call, const QString _de_grid,
                                 const QString _dx_grid, const QString _sub_mode);



    void slotWSJTXloggedQSO(const int _type, const QString _dxcall, const double _freq, const QString _mode,
                             const QString _dx_grid, const QString _time_off, const QString _report_sent, const QString _report_rec,
                             const QString _tx_power, const QString _comments, const QString _name, const QString _time_on);

private:
    bool maybeSave();


    //UPDATE CTY.DAT
    DownLoadCTY *downloadcty;
    //</UPDATE CTY.DAT>

    void createStatusBar();
    void createUI();
    void createUICQWW();
    void createUIDX();
    //void createDXClusterUI();
    void clearBandLabels();

    void createMenusCommon();
    void createMenusCQWW();
    void createActionsCommon();
    void createActionsCQWW();
    void createActionsDX();

    void createScorePanel();
    void initialContestModeConfiguration();


    bool readCtyFile();

    bool isQSLReceived(const int _qsoId);
    bool isQSLSent(const int _qsoId);

    bool validCharactersInCall(const QString _qrz); // Looks for SQLi and no valid chars in the QRZ
    QString readDataFromUI(); // Reads the QSO data from the UI and returns the SQL Query
    QString readDataFromUIDX();
    QString readDataFromUIDXModifying();
    void clearForNextQSO();
    void clearUIDX(bool full=false); //full= false leaves some data to allow pileup or normal Dx in same band; full removes freqs and everything

    void setAwardDXCC(const int _qsoId, bool modifying); // Adds or modify the status of a DXCC entity
    // data << dxcc(id) << band(id) << mode(id) << confirmed(0/1) << qsoid(id) << modify(0/1);

    void checkIfWorkedB4(const QString _qrz);
    bool checkContest();    
    void showStatusOfDXCC(const QStringList _qs);
    void showDXMarathonNeeded(const int _dxcc, const int _cqz, const int _year, const int _log);

    bool createConnection();
    void createData();
    bool processConfigLine(const QString _line);
    void readConfigData();    
    void defineStationCallsign();
    QString selectStationCallsign();

    void checkIfNewBandOrMode();
    void selectDefaultBand();
    void selectDefaultMode();
    void readActiveBands (const QStringList actives);
    void readActiveModes (const QStringList actives);
    bool checkIfNewMode(const QString _mode);
    void addNewValidMode(const QString _mode);

    void qsoToEdit (const int _qso);

    void completeWithPreviousQSO(const QString _call);

    void showAwards();
    void showDXMarathon(const int _year);
    void updateQSLRecAndSent();
    double checkFreqRanges(double _f);

    // CLUSTER
    void clusterSpotToLog(const QString _call, const QString _freq);
    QStringList dxclusterServersList;
    QString dxclusterServerToConnect;
    int dxclusterServerPort;
    // CLUSTER


    QWidget *mainWidget;
    //QWidget *dxClusterTabWidget;//, *searchTabWidget;
    World *world;
    FileManager *filemanager;
    Locator *locator;
    Awards *awards;
    Utilities *util;
    //WorldEditorWidget *worldEditor;

    //HelpHelpDialog *helpHelpDialog;
    //HelpAboutDialog *helpAboutDialog;
    AboutDialog *aboutDialog;

    QPushButton *addButton;
    //QLabel *distShortLabelN;
    //QLabel *distLongLabelN;

    DXCCStatusWidget *dxccStatusWidget;
    //QSqlRelationalTableModel *logModel;
    //QWidget *logPanel;
    //QTableView *logView;
    //QTreeWidget *searchResultsTreeWidget;
    QLabel *logLabel;

    QWidget *scoreWindow; //, dxWindow;
    QTextEdit *scoreTextEdit;

    LogModel *logModel;
    LogWindow *logWindow;
    UDPServer *UDPLogServer;

    ShowErrorDialog *showErrorDialog;


    // <UI>
    QGroupBox *gridGroupBox, *qrzgroupBox;//, *searchgroupBox;
    QFrame *dxUpLeftInputFrame;//, *dxUpRightOutputFrame;

    QLineEdit *qrzLineEdit, *nameLineEdit, *qthLineEdit, *locatorLineEdit;
    QComboBox *bandComboBox, *modeComboBox;
    QDateEdit *dateEdit;
    QTimeEdit *timeEdit;
    //QStatusBar *statusBar, *qsoStatusBar;
    QString statusBarMessage;

    QTabWidget *dxBottonTab, *dxUpLeftTab, *dxUpRightTab;

    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *qslToolMenu;
    QMenu *lotwToolMenu;
    //QMenu *lotwMarkAllAsQueuedMenu;
    //QMenu *lotwMarkAllInThisLogAsQueuedMenu;
    QMenu *viewMenu;
    QMenu *setupMenu;
    QMenu *helpMenu;

    QAction *klogFolderAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *printLogAct;
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;
    QAction *logWinAct;
    QAction *setupAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *updateAct;

    QAction *scoreWinAct;
    QAction *scoreeWinAct;
    QAction *ADIFExport;
    QAction *ADIFExportAll;
    QAction *ADIFImport;
    QAction *LoTWExport;
    QAction *LoTWImport;
    QAction *ReqQSLExport;
    QAction *CabrilloExport;
    QAction *fillQsoAct;
    QAction *fillDXCCAct;
    QAction *findQSO2QSLAct;
    QAction *findRequestedQSLAct;
    QAction *findQSLPendingToReceiveAct;
    QAction *findQSLDXRequestedAct;
    QAction *lotwMarkSentQueuedThisLogAct;
    QAction *lotwMarkSentQueuedAct;
    QAction *lotwMarkSentYesThisLogAct;
    QAction *lotwMarkSentYesAct;

    QAction *downloadCTYAct;

    QAction *loggWinAct;
/*
    QAction *delQSOFromLogAct;
    QAction *qsoToEditFromLogAct; 
    QAction *qslSentViaBureauFromLogAct;
    QAction *qslSentViaDirectFromLogAct;
    QAction *qslRecViaBureauFromLogAct;
    QAction *qslRecViaDirectFromLogAct;
*/
    QAction *qslSentRequestedAct;
    QAction *qslRecRequestedAct;

    //QAction *delQSOFromSearchAct;
    //QAction *qsoToEditFromSearchAct;
    //QAction *qslSentViaBureauFromSearchAct;
    //QAction *qslSentViaDirectFromSearchAct;
    //QAction *qslSentViaDirectMarkRcvReqFromSearchAct;
    //QAction *qslSentViaBureauMarkRcvReqFromSearchAct;
    //QAction *qslRecViaBureauFromSearchAct;
    //QAction *qslRecViaDirectFromSearchAct;
    //QAction *qslRecViaBureauMarkReqFromSearchAct;
    //QAction *qslRecViaDirectMarkReqFromSearchAct;

    QLineEdit *rstTXLineEdit;
    QLineEdit *rstRXLineEdit;
    QLineEdit *STXLineEdit;
    QLineEdit *SRXLineEdit;
    QPushButton *OKButton, *spotItButton, *clearButton;
    QStringList bands;
    QStringList modes;
    QStringList entitiesList, propModeList;

    // UI DX
    //QLabel *entitySecLabel, *iotaAwardLabel, *entityNameLabel, *propModeLabel; //entityPrimLabel
    QLabel *infoLabel1, *infoLabel2;
    //QPushButton *flagIcon; // To paint a flag of the worked entity
    //QLabel *bandLabel1, *bandLabel2, *bandLabel3, *bandLabel4;
    //QLabel *bandLabel5, *bandLabel6, *bandLabel7, *bandLabel8;
    //QLabel *bandLabel9, *bandLabel10, *bandLabel11, *bandLabel12;
    //QLabel *continentLabel, *prefixLabel, *cqzLabel, *ituzLabel;
    //QLabel *gradShortLabel, *distShortLabel;
    //QLabel *gradLongLabel, *distLongLabel;

    //QComboBox *iotaContinentComboBox, *entityPrimDivComboBox, *entitySecDivComboBox, *entityNameComboBox, *propModeComboBox;
    QComboBox *operatingYearsComboBox;
    QLineEdit *operatorLineEdit, *stationCallSignLineEdit, *myLocatorLineEdit;//, *commentLineEdit, *iotaNumberLineEdit;
    QTextEdit *notesTextEdit;
    QDoubleSpinBox *rxPowerSpinBox,  *txFreqSpinBox, *rxFreqSpinBox; //*myPowerSpinBox,
    QLCDNumber *freqQLCDNumber;
    QLCDNumber *dxccConfirmedQLCDNumber, *dxccWorkedQLCDNumber,
                *wazConfirmedQLCDNumber, *wazWorkedQLCDNumber,
                *localConfirmedQLCDNumber, *localWorkedQLCDNumber,
                *qsoConfirmedQLCDNumber, *qsoWorkedQLCDNumber,
                *dxMarathonQSOLCDNumber, *dxMarathonDXCCQLCDNumber, *dxMarathonCQQLCDNumber, *dxMarathonPointsQLCDNumber;
    //eQSL
    //QComboBox *eqslSentComboBox, *eqslRecComboBox, *lotwSentComboBox, *lotwRecComboBox, *clublogComboBox;
    //QDateEdit *eqslSentQDateEdit, *eqslRecQDateEdit, *lotwSentQDateEdit, *lotwRecQDateEdit, *clublogQDateEdit;

    //QComboBox *qslSentComboBox, *qslRecComboBox, *qslSentViaComboBox, *qslRecViaComboBox;
    //QDateEdit *qslSentQDateEdit, *qslRecQDateEdit;
    //QTextEdit *qslmsgTextEdit;
    //QLineEdit *qslViaLineEdit;


    //QLineEdit *searchBoxLineEdit;
    //QPushButton *searchBoxClearButton, *searchBoxExportButton, *searchBoxSelectAllButton, *searchBoxReSearchButton;
    //QRadioButton *searchAllRadioButton;
    QPushButton *recalculateAwardsButton;
    //bool searchSelectAllClicked, stationCallSignShownInSearch;

    bool checkNewVersions, reportInfo; // Selected in the Setup->Misc to check if new versions and report info back to KLog's servers

    bool qslingNeeded;
    bool noMoreErrorShown;              // If true, the errors shown in slotQueryErrorManagement will not be shown anymore in that KLog execution
    bool noMoreModeErrorShown;          // If true, the non-valdi modes received from WSJT-x will not be showed to the user
    MainWindowSatTab *satTabWidget;
    MainWindowMyDataTab *myDataTabWidget;
    MainWindowInputComment *commentTabWidget;
    MainWindowInputOthers *othersTabWidget;
    MainWindowInputEQSL *eQSLTabWidget;
    MainWindowInputQSL *QSLTabWidget;

    SearchWidget *searchWidget;
    InfoWidget *infoWidget;

    bool keepSatPage;

    // UI DX
    SetupDialog *setupDialog;

    // <CLUSTER>
    DXClusterWidget *dxClusterWidget;
    bool dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY;
    //QWidget *dxClusterWidget;
    //QListWidget *dxClusterListWidget;
    //QPushButton *sendDXClusterButton;
    //QLineEdit *inputDXClusterLineEdit;
    // </CLUSTER>

    // </UI>
    QTimer *timer;
    QDateTime *dateTime;

    QString klogDir, ctyDatFile, defaultADIFLogFile, configFileName;
    QString softwareVersion;
    bool itIsANewversion;


    QString currentQrz;
    QString previousQrz;

    QString stx;
    QString srx;
    QPalette palRed, palBlack; // To paint Text in red or black(normal)
    bool realTime, UTCTime, alwaysADIF, needToSave, useDefaultLogFileName, upAndRunning, qrzSmallModDontCalculate, imperialSystem, sendQSLWhenRec, keepMyData, completeWithPrevious, completedWithPreviousQTH, completedWithPreviousLocator, completedWithPreviousName, completedWithPreviousIOTA, completedWithPreviousQSLVia;
    bool cleaning;
    bool manageMode; // If true, the DXCC and awards will take the mode into consideration to decide if needed or worked. False implies only band is taken into account
    // Station Setup
    bool configured, modify;
    bool needToEnd; // Just to control if the software needs to end.
    bool qrzAutoChanging; //To stop executing the slotQRZTextChanged just because KLog uppercase a letter
    QString mainQRZ, stationQRZ, operatorQRZ, myLocator, dxLocator;
    QString lastOperatorQRZ, lastStationQRZ, lastMyLocator;
    double myPower, lastPower;

    int my_CQz, my_ITUz, defaultMode, defaultBand, currentMode, currentModeShown, currentBand, currentBandShown;
    int currentEntity, previousEntity;
    bool InValidCharsInPrevCall;
    int currentLog; // This is to be able to manage multiple logs without showing
                    // them all at the same time.
    int modifyingQSO; // When modifying, the QSO is saved here.
    int selectedYear;

    // Station Setup

    QColor defaultColor;
    QColor neededColor;
    QColor workedColor;
    QColor confirmedColor;
    QColor newOneColor;

    //<CLUBLOG>
    bool clublogActive, clublogRealTime;
    QString clublogUser, clublogPass, clublogEmail;

    eLogClubLog *elogClublog;
    int clublogAnswer;
    QStringList clublogPrevQSO;

    //</CLUBLOG>

    // Contest
    int points, qsoPoints, multipliers, qsoMultiplier;
    QString contestMode;
    Contest *contest;
    // Contest
    DataProxy *dataProxy;

    DataBase *db;
    bool DBinMemory;

    QTextDocument *doc;

    SoftwareUpdate *softUpdate;
    bool callingUpdate;
    
    bool UDPServerStart;

    //LOGVIEW
    //QString bandOld, modeOld;
    //LOGVIEW

signals:
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution


protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

};

#endif

