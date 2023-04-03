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
#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtAlgorithms>
#include <QDesktopServices>
#include <QUrl>

#include "setupdialog.h"
#include "aboutdialog.h"
#include "tipsdialog.h"
#include "world.h"
#include "filemanager.h"
#include "fileawardmanager.h"
#include "dataproxy_sqlite.h"
#include "dataproxy_sqlite.h"
#include "locator.h"
#include "dxcluster.h"
#include "awards.h"
#include "inputwidgets/mainwindowsattab.h"
#include "inputwidgets/mainwindowmydatatab.h"
#include "inputwidgets/mainwindowinputcomment.h"
#include "inputwidgets/mainwindowinputothers.h"
#include "inputwidgets/mainwindowinputeqsl.h"
#include "inputwidgets/mainwindowinputqsl.h"
#include "inputwidgets/mainwindowinputqso.h"
#include "mainqsoentrywidget.h"
#include "elogclublog.h"
#include "utilities.h"
#include "downloadcty.h"
#include "dxccstatuswidget.h"
#include "awardswidget.h"
#include "softwareupdate.h"
#include "logwindow.h"
#include "searchwidget.h"
#include "infowidget.h"
#include "showerrordialog.h"
#include "udpserver.h"
#include "statisticswidget.h"
#include "updatesatsdata.h"
#include "hamlibclass.h"
#include "elogqrzlog.h"
#include "lotwutilities.h"
#include "eqslutilities.h"
#include "widgets/adiflotwexportwidget.h"
#include "widgets/showadifimportwidget.h"
#include "widgets/map/mapwindowwidget.h"
#include "widgets/showkloglogwidget.h"
#include "qso.h"
#include "updatesettings.h"
#include "klogdefinitions.h"

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

//class QPoint;

class QGroupBox;
class QTabWidget;
class QFrame;
class QTableView;
class QLabel;


/*
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
*/
//enum ExportMode {ModeLotW, ModeADIF};

class MainWindow : public  QMainWindow
{
    Q_OBJECT
    friend class tst_MainWindow;

public:
    MainWindow(const QString &tversion);
    ~MainWindow();
    void checkIfNewVersion();
    void recommendBackupIfNeeded();
    void init();
    void showNotWar();

private slots:
    //void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery);
    //void slotTest();        // Slot for testing purposes only
    void slotOpenWiki();
    void slotAWAImport();
    void slotClearNoMorErrorShown();
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString queryFailed);
    void slotQRZReturnPressed();
    //void slotQRZSpacePressed();
    void slotQRZTextChanged(QString _qrz);
    //void slotSRXTextChanged();
    //void slotSTXTextChanged();
    void slotUpdateLocator(QString _loc);
    void slotLocatorTextChanged(const QString &_loc);
    //void slotMyLocatorTextChanged();
    void slotFreqTXChanged(const double _fr);
    void slotFreqRXChanged(const double _fr);
    //void slotSplitCLicked();

    void slotSearchBoxTextChanged();
    //void slotCloseStats(bool _vis);
    void slotSearchToolNeededQSLToSend();
    void slotToolSearchRequestedQSLToSend();
    void slotToolSearchNeededQSLPendingToReceive();
    void slotToolSearchNeededQSLRequested();
    void slotToolLoTWMarkAllQueuedThisLog();
    void slotToolLoTWMarkAllQueued();
    void slotToolLoTWMarkAllYesThisLog();
    void slotToolLoTWMarkAllYes();
    void slotLoTWDownloadedFileProcess(const QString &_fn);

    void slotClubLogLogUpload();
    void sloteQSLLogUpload();
    void slotElogEQSLModifyCurrentLog();

    void slotOKButtonClicked();

    void slotClearButtonClicked(const QString &_func);
    void slotBandChanged (const QString &_b);
    void slotModeChanged (const QString &_m);
    void slotValidBandsReceived(const QStringList &_b);

    void slotRefreshDXCCWidget();

    void slotLogWinShow();
    void slotLogRefresh();

    void slotQSODelete(const int _id);
    void slotQSOsDelete(QList<int> _id);
    void slotQSOsExportToADIF(QList<int> _id);
    void slotQRZcomUpload(QList<int> _id);
    void slotQRZCOMLogUpload();
    void slotElogQRZCOMDisable(const bool _b);

    void slotShowAwards();
    void slotUpdateStatusBar(const QString &statusm);
    void setMainWindowTitle();
    void slotSetup(const int _page=0);

    void slotADIFExport();

    void slotLoTWExport();
    void slotLoTWDownload();
    void slotLoTWFullDownload();
    void slotADIFExportSelection(const QString &_st, const QString &_grid, const QDate &_startDate, const QDate &_endDate, const ExportMode _eM);
    void slotADIFExportSelection2(const QString &_call, QList<int> _qsos, ExportMode _eM);
    void slotADIFExportAll();
    void slotADIFImport();
    void slotRQSLExport();
    void slotReceiveQSOListToShowFromFile(QStringList _qs);

    void slotTimeOutInfoBars(); // Clears the infoLabels when the timeout emits the signal

    void slotSetPropModeFromOther(const QString &_p);
    void slotSetPropModeFromSat(const QString &_p, bool _keep);
    void slotFillEmptyDXCCInTheLog();
    void slotUpdateCTYDAT();
    void slotUpdateSATSDAT();
    void slotShowStats();

    void slotWorldReload(const bool _b);

    void slotExitFromSlotDialog(const int exitID);
    void slotSetupDialogFinished (const int _s);
    void exitQuestion();


    void fillQSOData();

    bool slotOpenKLogFolder();

    void slotFilePrint();
    void slotFileClose();

    void slotHelpAboutAction();
    void slotHelpCheckUpdatesAction();
    void slotAboutQt();
    void slotTipsAction();
    void slotDebugAction();

    // MainQSOEntryWidget
    void slotShowInfoLabel(const QString &_m);
    void slotAwardsWidgetSetLog();
    void slotAwardsWidgetSetYear();
    void slotManualMode(bool _enable);

    // MyDataTab
    void slotMyLocatorTextChanged(const QString &_loc);

    // logpanel
    //void slotRighButtonFromLog( const QPoint& pos);
    void slotDoubleClickLog( const int _qsoID);
    //void slotDoubleClickLog( const QModelIndex & index);

    //SEARCH
    void slotShowSearchWidget();                // The SearchWidget request being shown
    //SEARCH

    // CLUSTER
    void slotAnalyzeDxClusterSignal(const QStringList _qs);
    void slotDXClusterSpotArrived(const QString _dxCall, const QString _dxGrid, const double _freq);

    // CLUSTER
    //CLUBLOG
    void slotElogClubLogShowMessage(const QString &_s);
    void slotElogClubLogProcessAnswer(const int _i, const int _qID);
    void slotElogClubLogDisable(const bool _b);
    void slotElogClubLogFileUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos);
    void slotElogClubLogModifyCurrentLog();

    //CLUBLOG
    // EQSL
    void slotElogEQSLFileUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos);
    // EQSL
    void slotShowSoftUpdateResults(const bool _b);   // Software Update: Receives the signal to see if it is needed or not to update
    // EQSL
    // QRZCOM
    void slotElogQRZCOMShowMessage(const QString &_s);
    void slotElogQRZCOMFoundData(const QString &_t, const QString & _d);
    void slotElogQRZCOMCheckThisCall();
    void slotElogQRZCOMAutoCheck();
    void slotElogQRZCOMAutoCheckFromSetup(const bool _s);
    void slotElogQRZCOMModifyCurrentLog();
    void slotElogQRZCOMLogUploaded (QNetworkReply::NetworkError _error, QList<int> _qsos);
    // QRZCOM
    //SATELLITE
    //void slotSatBandTXComboBoxChanged(const QString &_q);
    void slotDefineNewBands (const QStringList _bands);

    //HAMLIB
    void slotHamlibTXFreqChanged(const double _f);
    void slotHamlibModeChanged(const QString &_m);
    //void slotHamlibModeNotADIF();

    // PST Rotator
    //void slotRotatorShow();
    // WORLD MAP
    void slotShowMap();

    //DXCCWIDGET
    //void slotShowQSOFromDXCCWidget(const int _q);
    void slotShowQSOsFromDXCCWidget(QList<int> _qsos);

    //UDP Server (WXJT-x)
    void slotWSJXstatusFromUDPServer(const int _type, const QString &_dxcall, const double _freq, const QString &_mode,
                                 const QString &_report, const QString &_de_call, const QString &_de_grid,
                                 const QString &_dx_grid, const QString &_sub_mode);

    void slotWSJTXloggedQSO (const QString &_dxcall, const QString &_mode, const QString &_band, const double _freq,
                     const QString &_mygrid, const QString &_dxgrid, const QString &_rstTX, const QString &_rstRX,
                     const QString &_comment, const QString &_stationcallsign, const QString &_name,
                     const QString &_opCall, const QDateTime &_datetime, const QDateTime &_datetime_off,
                     const QString &_exchangeTX, const QString &_exchangeRX, const QString &_mypwr);

    void slotCaptureDebugLogs(const QString &_func, const QString &_msg, DebugLogLevel _level);
    //void slotTakeOverFocus(int _id);
    void slotTakeOverFocusToQSOTabWidget();
    void slotTakeOverFocusToMainQSOInput();
    void slotNewLogLevel(DebugLogLevel l);

private:
    //void setWidgetsOrder();

    void startServices();
    void backupCurrentQSO();
    void restoreCurrentQSO(const bool restoreConfig);
    void showMessageToEnableTheOnlineService(const OnLineProvider _service);
    void cleanQRZCOMreceivedDataFromUI();
    void saveWindowsSize();
    void setWindowSize(const QSize &_size);
    bool maybeSave();
    void setCleaning(const bool _c);
    bool setHamlib(const bool _b);
    bool setUDPServer(const bool _b);
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    void setLogLevel(const DebugLogLevel _sev);
    void fileExportLoTW(const QString &_st, const QString &_grid, const QDate &_startDate, const QDate &_endDate);
    void fileExportLoTW2(const QString &_call, QList<int> _qsos);
    void fileExportClubLog(const QString &_st, const QDate &_startDate, const QDate &_endDate);
    void fileExportClubLog2(const QString &_call, QList<int> _qsos);
    void fileExportEQSL(const QString &_st, const QDate &_startDate, const QDate &_endDate);
    void fileExportADIF(const QString &_st, const QString &_grid, const QDate &_startDate, const QDate &_endDate);
    bool callTQSL(const QString &_filename, const QString &_call);
    void showNumberOfSavedQSO(const QString &_fn, const int _n);
    //QString getCallToUseForLoTWExportUpload();
    UpdateSatsData *updateSatsData;
    //UPDATE CTY.DAT
    DownLoadCTY *downloadcty;
    HamLibClass *hamlib;
    QSO *qso;

    bool hamlibActive;
    bool hamlibChangingMode;
    bool hamlibModeNotADIFSupported;

    bool sendQSLByDefault;
    bool deleteAlwaysAdiFile;

    MapWindowWidget *mapWindow;

    // PST Rotator
    //PSTRotatorSupport *pstRotator;
    //bool usePSTRotator;
    //RotatorWidget *rotatorWidget;
    //</UPDATE CTY.DAT>
    void createStatusBar();
    void createUI();
    void createUIDX();
    void reconfigureDXMarathonUI(const bool _dxM);
    //void createDXClusterUI();
    void clearBandLabels();

    void createMenusCommon();
    void createActionsCommon();


    //bool validCharactersInCall(const QString &_qrz); // Looks for SQLi and no valid chars in the QRZ
    QString readDataFromUI(); // Reads the QSO data from the UI and returns the SQL Query
    QString readDataFromUIDX();
    QString readDataFromUIDXModifying();
    void actionsJustAfterAddingOneQSO();
    //void clearForNextQSO();
    void clearUIDX(bool _full = false); //full= false leaves the "keep this data"; full = true clears everything

    void setAwardDXCC(const int _qsoId, bool modifying); // Adds or modify the status of a DXCC entity
    // data << dxcc(id) << band(id) << mode(id) << confirmed(0/1) << qsoid(id) << modify(0/1);

    //void checkIfWorkedB4(const QString &_qrz);
    //bool checkContest();
    void showStatusOfDXCC(const QStringList _qs);
    void showDXMarathonNeeded(const int _dxcc, const int _cqz, const int _year, const int _log);

    bool createConnection();
    void openSetup(const int _page=0);
    //bool processConfigLine(const QString &_line);
    //void readConfigData();
    //void saveSettings();
    bool loadSettings();
    bool applySettings();
    void selectTheLog(const int _i);    // Receives a log number from loadSettings and setups all about the logN
    void defineStationCallsign(const QString &_call);
    QString selectStationCallsign();

    void checkIfNewBandOrMode();
    void selectDefaultBand();
    void selectDefaultMode();
    void readActiveBands (const QStringList actives);
    void readActiveModes (const QStringList actives);
    bool checkIfNewMode(const QString &_mode);
    void addNewValidMode(const QString &_mode);
    void setModeFromFreq();

    void qsoToEdit (const int _qso);
    void setModifying(const bool _m);

    void completeWithPreviousQSO(const QString &_call);

    //void showAwards();
    //void showDXMarathon(const int _year);
    void updateQSLRecAndSent();
    //double checkFreqRanges(double _f);
    //void setRSTToMode(const QString &_m);


    // CLUSTER
    void clusterSpotToLog(const QString &_call, const QString &_freq);
    QStringList dxclusterServersList;
    //QString dxclusterServerToConnect;
    //int dxclusterServerPort;
    bool dxclusterSendSpotsToMap;
    // CLUSTER

    //LOTW
    AdifLoTWExportWidget *adifLoTWExportWidget;
    //LOTW
    ShowAdifImportWidget *showAdifImportWidget;

    QWidget *mainWidget;
    //QWidget *dxClusterTabWidget;//, *searchTabWidget;
    World *world;
    FileManager *filemanager;
    FileAwardManager *fileAwardManager;
    Locator *locator;
    Awards *awards;
    Utilities *util;
    //SetupPageMisc *miscPage;


    //WorldEditorWidget *worldEditor;

    //HelpHelpDialog *helpHelpDialog;
    //HelpAboutDialog *helpAboutDialog;
    AboutDialog *aboutDialog;
    TipsDialog *tipsDialog;
    ShowKLogLogWidget * showKLogLogWidget;
    StatisticsWidget *statsWidget;
    DXCCStatusWidget *dxccStatusWidget;
    QLabel *logLabel;

    //LogModel *logModel;
    LogWindow *logWindow;
    UDPServer *UDPLogServer;

    ShowErrorDialog *showErrorDialog;

    // <UI>
    QFrame *dxUpLeftInputFrame;//, *dxUpRightOutputFrame;
    QString statusBarMessage;

    QTabWidget *dxBottonTab, *dxUpLeftTab, *dxUpRightTab;

    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *qslToolMenu;
    QMenu *lotwToolMenu;
    QMenu *clublogToolMenu;
    QMenu *eQSLToolMenu;
    QMenu *QRZCOMToolMenu;
    //QMenu *lotwMarkAllAsQueuedMenu;
    //QMenu *lotwMarkAllInThisLogAsQueuedMenu;
    QMenu *viewMenu;
    //QMenu *setupMenu;
    QMenu *helpMenu;

    //QAction *TestAct;       // Action for testing purposes only
    QAction *awardAddAct;
    QAction *klogFolderAct;
    //QAction *openAct;
    //QAction *saveAct;
    QAction *exitAct;
    QAction *printLogAct;
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;
    QAction *logWinAct;
    QAction *setupAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *tipsAct;
    QAction *debugAct;
    QAction *aboutQtAct;
    QAction *updateAct;

    QAction *scoreWinAct;
    QAction *scoreeWinAct;
    QAction *ADIFExport;
    QAction *ADIFExportAll;
    QAction *ADIFImport;
    //QAction *LoTWExport;
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
    QAction *lotwCallTQSL;
    QAction *lotwUpdateFromLoTWAct;
    QAction *lotwFullDownloadFromLoTWAct;

    QAction *clublogLogUploadAct;
    QAction *clublogLogModifyCurrentLogAct;
    QAction *eqslUploadAct;
    QAction *eqslLogModifyCurrentLogAct;

    QAction *QRZCOMCheckThisCallAct;
    QAction *QRZCOMAutoCheckAct;
    QAction *QRZCOMLogUploadAct;
    QAction *QRZCOMLogModifyCurrentLogAct;

    QAction *downloadCTYAct;
    QAction *downloadSATSAct;
    QAction *showStatsAct;
    //QAction *showRotatorAct;
    //QAction *showDXCCSummaryAct;

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
    QAction *showMapAct;

    QStringList bands;
    QStringList modes;
    QStringList entitiesList, propModeList;

    // UI DX
    //QLabel *entitySecLabel, *iotaAwardLabel, *entityNameLabel, *propModeLabel; //entityPrimLabel
    QLabel *infoLabel1, *infoLabel2, *dxMarathonLabelN, *dxMarathonTopScoreLabelN;

    QString lotwTQSLpath;

    bool checkNewVersions, reportInfo; // Selected in the Setup->Misc to check if new versions and report info back to KLog's servers
    bool logEvents;                     // Should KLog log the events for debugging

    DebugLogLevel logLevel;    // Manages as syslog, the severity of the application debug log (7 means debug, 0 emergency)

    bool txFreqBeingChanged,  updatingBands; //rxFreqBeingChanged  // When the freqs is being modified it is defined to true to prevent other automated to change.
    bool txFreqBeingAutoChanged, rxFreqBeingAutoChanged;        // This is defined to true when freq is being changed by the Sat tab to prevent a loop.
    bool qslingNeeded;
    bool noMoreErrorShown;              // If true, the errors shown in slotQueryErrorManagement will not be shown anymore in that KLog execution
    bool noMoreModeErrorShown;          // If true, the non-valid modes received from WSJT-x will not be showed to the user
    bool wsjtxAutoLog;                  // If true, logged QSO are automatically logged into the log when sent from WSJTX
    bool manualMode;                    // If true, hamlib and WSJTX are not updatind the interface
    MainWindowSatTab *satTabWidget;
    MainWindowMyDataTab *myDataTabWidget;
    MainWindowInputComment *commentTabWidget;
    MainWindowInputOthers *othersTabWidget;
    MainWindowInputEQSL *eQSLTabWidget;
    MainWindowInputQSL *QSLTabWidget;
    MainWindowInputQSO *QSOTabWidget;
    MainQSOEntryWidget *mainQSOEntryWidget;

    AwardsWidget *awardsWidget;
    SearchWidget *searchWidget;
    InfoWidget *infoWidget;

    //AdvancedSearchWidget *advancedSearchWidget;

    bool keepSatPage;

    // UI DX
    SetupDialog *setupDialog;

    // <CLUSTER>
    DXClusterWidget *dxClusterWidget;
    bool dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY;
    // </CLUSTER>

    // </UI>
    int infoTimeout; // timeout that temporary info will stay in the infobars
    QTimer *timer, *timerInfoBars;
    QDateTime *dateTime, *dateTimeTemp;
    bool yearChangedDuringModification;
    QString infoLabel1T, infoLabel2T;

    QString ctyDatFile, defaultADIFLogFile;
    QString softwareVersion;
    bool itIsANewversion;
    int dupeSlotInSeconds;

    QString stx;
    QString srx;
    QPalette palRed, palBlack; // To paint Text in red or black(normal)
    bool  alwaysADIF, needToSave, useDefaultLogFileName, upAndRunning, qrzSmallModDontCalculate, imperialSystem, sendQSLWhenRec, manageDxMarathon, completeWithPrevious, completedWithPreviousQTH, completedWithPreviousLocator, completedWithPreviousName, completedWithPreviousIOTA, completedWithPreviousQSLVia;
    // bool realTime, UTCTime;
    bool cleaning;
    bool manageMode; // If true, the DXCC and awards will take the mode into consideration to decide if needed or worked. False implies only band is taken into account
    // Station Setup
    bool configured, modify;
    bool needToEnd; // Just to control if the software needs to end.
    bool qrzAutoChanging; //To remove the data coming from QRZ.com only when data is coming.
    QString mainQRZ, stationCallsign, operatorQRZ, dxLocator;

    //int my_CQz, my_ITUz; Not used
    int defaultMode, defaultBand, currentMode, currentModeShown, currentBand, currentBandShown;
    int currentEntity, previousEntity;
    bool InValidCharsInPrevCall;
    int currentLog; // This is to be able to manage multiple logs without showing
                    // them all at the same time.
    int modifyingQSO; // When modifying, the QSO is saved here.
    int selectedYear;
    bool readingTheUI;  // While reading the data from UI after QSO intro or modificationthis is true

    // Station Setup

    QColor defaultColor;
    QColor neededColor;
    QColor workedColor;
    QColor confirmedColor;
    QColor newOneColor;

    QSize windowSize;

    //<CLUBLOG>
    bool clublogActive, clublogRealTime, eQSLActive; //clublogUseStationCallSign,

    eLogClubLog *elogClublog;
    int clublogAnswer;
    QStringList clublogPrevQSO;

    //</CLUBLOG>
    // QRZ.com
    bool qrzcomActive;
    bool qrzcomSubscriber;
    eLogQrzLog *elogQRZcom;
    QString qrzcomUser, qrzcomPass;
    // QRz.com - END

    // Contest
    //int points, qsoPoints, multipliers, qsoMultiplier;
    QString contestMode;
    //Contest *contest;
    // Contest
    DataProxy_SQLite *dataProxy;

    //DataBase *db;


    SoftwareUpdate *softUpdate;
    bool callingUpdate;

    bool UDPServerStart;
    // LOTWUTILITIES
    LoTWUtilities *lotwUtilities;
    bool lotwActive;

    //eQSLUtilities
    eQSLUtilities *eqslUtilities;

    //LOGVIEW
    //QString bandOld, modeOld;
    //LOGVIEW

signals:
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

};

#endif

