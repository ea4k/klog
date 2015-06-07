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

#include <QtGui>
//#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>

#include <QNetworkRequest>
#include <QNetworkReply>


#include "database.h"
#include "setupdialog.h"
#include "helpaboutdialog.h"
#include "helphelpdialog.h"
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
#include "elogclublog.h"

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
    MainWindow(const QString _kontestDir, const QString tversion);

private slots:

    void slotQRZReturnPressed();
    void slotQRZSpacePressed();
    void slotQRZTextChanged();
    void slotSRXTextChanged();
    void slotSTXTextChanged();
    void slotLocatorTextChanged();
    void slotMyLocatorTextChanged();

    void slotSearchBoxTextChanged();
    void slotSearchExportButtonClicked();
    void slotSearchBoxSelectAllButtonClicked();
    void slotSearchClearButtonClicked();
    void slotSearchBoxSelectionChanged();
    void slotSearchBoxReSearchButtonClicked();
    //void slotSearchBoxOnItemChanged( QTreeWidgetItem * item, int column);

    void slotModeComboBoxChanged();
    void slotBandComboBoxChanged();
    void slotIOTAComboBoxChanged();
    void slotOperatingYearComboBoxChanged();
    void slotOKButtonClicked();
    void slotSpotItButtonClicked();
    void slotClearButtonClicked();
    void slotUpdateTime();
    void slotLogWinShow();
    void slotScoreWinShow();
    void slotSetup(const int _page=0);
    void slotQsoDeleteFromLog();
    void slotQSLSentViaBureauFromLog();
    void slotQSLSentViaDirectFromLog();
    void slotQSLRecViaDirectFromLog();
    void slotQSLRecViaBureauFromLog();
    void slotQSOToEditFromLog();

    void slotQsoDeleteFromSearch();
    void slotQSLSentViaBureauFromSearch();
    void slotQSLSentViaDirectFromSearch();
    void slotQSLSentViaDirectMarkDXReqFromSearch();

    void slotQSLSentViaBureuMarkDXReqFromSearch();
    void slotQSLRecViaDirectFromSearch();
    void slotQSLRecViaBureauFromSearch();
    void slotQSLRecViaDirectMarkReqFromSearch();
    void slotQSLRecViaBureauMarkReqFromSearch();
    void slotQSLSentMarkAsRequested();
    void slotQSLRecMarkAsRequested();
    void slotQSOToEditFromSearch();

    void slotrstTXTextChanged();
    void slotrstRXTextChanged();
    void slotADIFExport();
    void slotADIFExportAll();
    void slotADIFImport();
    void slotRQSLExport();
    void slotCabrilloExport();
    void slotQSLRecvComboBoxChanged();
    void slotQSLSentComboBoxChanged();
    void sloteQSLRecvComboBoxChanged();
    void sloteQSLSentComboBoxChanged();
    void slotQSLViaTextChanged();

    void slotSetPropMode(const QString _p);

    void slotUpdateCTYDAT();
    void slotToolSearchNeededQSLToSend();
    void slotToolSearchRequestedQSLToSend();
    void slotToolSearchNeededQSLPendingToReceive();
    void slotToolSearchNeededQSLRequested();
    void slotToolSearchQSL(const int actionQSL);

    void slotExitFromSlotDialog(const int exitID);

    void slotDownloadFinished(QNetworkReply *reply);

    void fillQSOData();

    void newFile();
    void openFile();
    bool saveFile(const QString _fileName);
    bool saveFileAs();


    void slotFilePrint();
    //void slotFileClose();

    void slotHelpHelpAction();
    void slotHelpAboutAction();
    void slotAboutQt();
       

    // logpanel
    void slotRighButtonFromLog( const QPoint& pos);
    void slotDoubleClickLog( const QModelIndex & index);
    void slotDoubleClickSearch( QTreeWidgetItem* item, int); // Double click on a QSO in the search box

    void slotRecalculateAwardsButtonClicked();

    //SEARCH
    void slotRighButtonSearch(const QPoint& pos);
    ///SEARCH

    // CLUSTER
    void slotAnalyzeDxClusterSignal(const QStringList _qs);

    // CLUSTER
    //CLUBLOG
    void slotElogClubLogShowMessage(const QString _s);
    void slotElogClubLogProcessAnswer(const int _i, const int _qID);
    void slotElogClubLogDisable(const bool _b);
    //CLUBLOG


private:
    //UPDATE CTY.DAT

    bool downloadCTYFile();
    bool downloadCtyDatFile();
    bool saveToDisk(const QString &filename, QIODevice *data);
    bool maybeSave();


    QString saveFileName(const QUrl &url);

    QNetworkAccessManager manager;
    QNetworkRequest request;

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
    void createlogPanel();
    void createlogModel(const int _i);

    void createScorePanel();
    void createSearchResultsPanel();

    void initialContestModeConfiguration();

    void createKeyEvents();
    void updateStatusBar(const QString statusm);

    bool readCtyFile();


    void showMenuRightButtonFromLogCreateActions();
    void righButtonFromLogMenu(const int trow);

    void showMenuRightButtonSearchCreateActions();
    void righButtonSearchMenu(const int trow);

    void qslSentViaBureau(const int _qsoId);
    //void qslSentViaDirect(const int _qsoId);
    void qslRecViaBureau(const int _qsoId);
    void qslRecViaDirect(const int _qsoId);

    void qslRecViaBureauMarkReq(const int _qsoId);
    void qslRecViaDirectMarkReq(const int _qsoId);

    bool isQSLReceived(const int _qsoId);
    bool isQSLSent(const int _qsoId);

    bool validCharactersInCall(const QString _qrz); // Looks for SQLi and no valid chars in the QRZ
    QString readDataFromUI(); // Reads the QSO data from the UI and returns the SQL Query
    QString readDataFromUIDX();
    QString readDataFromUIDXModifying();

    int getDXCCFromComboBox();
    QString getPropModeFromComboBox();

    //QString readDataFromUICQWW();

    void setAwardDXCC(const int _qsoId, bool modifying); // Adds or modify the status of a DXCC entity
    // data << dxcc(id) << band(id) << mode(id) << confirmed(0/1) << qsoid(id) << modify(0/1);

    int checkIfWorkedB4(const QString _qrz);
    bool checkContest();    
    void showStatusOfDXCC(const QStringList _qs);

    bool createConnection();
    void createData();
    bool processConfigLine(const QString _line);
    void readConfigData();
    bool trueOrFalse(const QString _s); // reads a String and return true if s.upper()== TRUE :-)
    void defineStationCallsign();

    void checkIfNewBandOrMode();
    void readActiveBands (const QStringList actives);
    void readActiveModes (const QStringList actives);


    void qsoToEdit (const int _qso);
    void showInfoFromLocators(const QString _loc1, const QString _loc2);
    void clearInfoFromLocators();

    //void showQRZEntityInfo(const QString _qrz);
    void showEntityInfo(const int _enti);
    QString getStyleColorToLabelFromBand(const QString _b, const QString _q);
    void showAwards();
    void showDXMarathon(const int _year);
    void updateQSLRecAndSent();
    double checkFreqRanges(double _f);

    void selectCorrectComboBoxEntity(const int _ent); // Select the appropriate entity in the ComboBox

    //int Km2Mile(const bool _imperialSystemSelected, const bool _dataInMiles, const int _km);

    // CLUSTER
    void clusterSpotToLog(const QString _call, const QString _freq);
    QStringList dxclusterServersList;
    QString dxclusterServerToConnect;
    int dxclusterServerPort;
    // CLUSTER



    QWidget *mainWidget;
    QWidget *dxClusterTabWidget, *searchTabWidget;
    World *world;
    FileManager *filemanager;
    Locator *locator;
    Awards *awards;
    //WorldEditorWidget *worldEditor;

    HelpHelpDialog *helpHelpDialog;
    HelpAboutDialog *helpAboutDialog;

    QPushButton *addButton;
    QLabel *distShortLabelN;
    QLabel *distLongLabelN;

    //QLabel distShortLabelN;
    //QLabel distLongLabelN;


    QSqlRelationalTableModel *logModel;
    QWidget *logPanel;
    QTableView *logView;
    QTreeWidget *searchResultsTreeWidget;
    QLabel *logLabel;

    QWidget *scoreWindow; //, dxWindow;
    QTextEdit *scoreTextEdit;

    // <UI>
    QGroupBox *gridGroupBox, *qrzgroupBox, *searchgroupBox;
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
    QMenu *viewMenu;
    QMenu *setupMenu;
    QMenu *helpMenu;

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

    QAction *scoreWinAct;
    QAction *scoreeWinAct;
    QAction *ADIFExport;
    QAction *ADIFExportAll;
    QAction *ADIFImport;
    QAction *ReqQSLExport;
    QAction *CabrilloExport;
    QAction *fillQsoAct;
    QAction *findQSO2QSLAct;
    QAction *findRequestedQSLAct;
    QAction *findQSLPendingToReceiveAct;
    QAction *findQSLDXRequestedAct;

    QAction *downloadCTYAct;


    QAction *loggWinAct;

    QAction *delQSOFromLogAct;
    QAction *qsoToEditFromLogAct; 
    QAction *qslSentViaBureauFromLogAct;
    QAction *qslSentViaDirectFromLogAct;
    QAction *qslRecViaBureauFromLogAct;
    QAction *qslRecViaDirectFromLogAct;
    QAction *qslSentRequestedAct;
    QAction *qslRecRequestedAct;

    QAction *delQSOFromSearchAct;
    QAction *qsoToEditFromSearchAct;
    QAction *qslSentViaBureauFromSearchAct;
    QAction *qslSentViaDirectFromSearchAct;
    QAction *qslSentViaDirectMarkRcvReqFromSearchAct;
    QAction *qslSentViaBureauMarkRcvReqFromSearchAct;
    QAction *qslRecViaBureauFromSearchAct;
    QAction *qslRecViaDirectFromSearchAct;
    QAction *qslRecViaBureauMarkReqFromSearchAct;
    QAction *qslRecViaDirectMarkReqFromSearchAct;

    QLineEdit *rstTXLineEdit;
    QLineEdit *rstRXLineEdit;
    QLineEdit *STXLineEdit;
    QLineEdit *SRXLineEdit;
    QPushButton *OKButton, *spotItButton, *clearButton;
    QStringList bands;
    QStringList modes;
    QStringList entitiesList, propModeList;

    // UI DX
    QLabel *entityPrimLabel, *entitySecLabel, *iotaAwardLabel, *entityNameLabel, *propModeLabel;
    QLabel *infoLabel1, *infoLabel2;
    QLabel *bandLabel1, *bandLabel2, *bandLabel3, *bandLabel4;
    QLabel *bandLabel5, *bandLabel6, *bandLabel7, *bandLabel8;
    QLabel *bandLabel9, *bandLabel10, *bandLabel11, *bandLabel12;
    QLabel *continentLabel, *prefixLabel, *cqzLabel, *ituzLabel;
    QLabel *gradShortLabel, *distShortLabel;
    QLabel *gradLongLabel, *distLongLabel;

    QComboBox *iotaContinentComboBox, *entityPrimDivComboBox, *entitySecDivComboBox, *entityNameComboBox, *propModeComboBox;
    QComboBox *operatingYearsComboBox;
    QLineEdit *iotaNumberLineEdit, *operatorLineEdit, *stationCallSignLineEdit, *myLocatorLineEdit, *commentLineEdit;
    QTextEdit *notesTextEdit;
    QDoubleSpinBox *myPowerSpinBox, *rxPowerSpinBox,  *txFreqSpinBox, *rxFreqSpinBox;
    QLCDNumber *freqQLCDNumber;
    QLCDNumber *dxccConfirmedQLCDNumber, *dxccWorkedQLCDNumber,
                *wazConfirmedQLCDNumber, *wazWorkedQLCDNumber,
                *localConfirmedQLCDNumber, *localWorkedQLCDNumber,
                *qsoConfirmedQLCDNumber, *qsoWorkedQLCDNumber,
                *dxMarathonDXCCQLCDNumber, *dxMarathonCQQLCDNumber, *dxMarathonPointsQLCDNumber;
    QComboBox *eqslSentComboBox, *eqslRecComboBox, *lotwSentComboBox, *lotwRecComboBox, *clublogComboBox;
    QDateEdit *eqslSentQDateEdit, *eqslRecQDateEdit, *lotwSentQDateEdit, *lotwRecQDateEdit, *clublogQDateEdit;

    QComboBox *qslSentComboBox, *qslRecComboBox, *qslSentViaComboBox, *qslRecViaComboBox;
    QDateEdit *qslSentQDateEdit, *qslRecQDateEdit;
    QTextEdit *qslmsgTextEdit;
    QLineEdit *qslViaLineEdit;


    QLineEdit *searchBoxLineEdit;
    QPushButton *searchBoxClearButton, *searchBoxExportButton, *searchBoxSelectAllButton, *searchBoxReSearchButton;
    QRadioButton *searchAllRadioButton;
    QPushButton *recalculateAwardsButton;
    bool searchSelectAllClicked, stationCallSignShownInSearch;
    bool qslingNeeded;

    MainWindowSatTab *satTabWidget;
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

    QString kontestDir, defaultADIFLogFile, configFileName;
    QString softwareVersion;


    QString currentQrz;
    QString previousQrz;

    QString stx;
    QString srx;

    bool realTime, UTCTime, alwaysADIF, needToSave, useDefaultLogFileName, upAndRunning, qrzSmallModDontCalculate, imperialSystem, sendQSLWhenRec, keepMyData;
    bool cleaning;
    // Station Setup
    bool configured, modify;
    bool needToEnd; // Just to control if the software needs to end.
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
    int contestMode, points, qsoPoints, multipliers, qsoMultiplier;

    Contest *contest;
    // Contest
    DataProxy *dataProxy;

    DataBase *db;
    bool DBinMemory;

    QTextDocument *doc;


protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

};

#endif

