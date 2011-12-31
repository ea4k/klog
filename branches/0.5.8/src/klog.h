/***************************************************************************
                          klog.h  -  description
                             -------------------
    begin                : sb dic  7 18:42:45 CET 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
 *                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/

#ifndef KLOG_H
#define KLOG_H

#include <KDE/KApplication>

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QList>
#include <QPixmap>
#include <QTextStream>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDateTime>
#include <QSpinBox>
#include <QString>
#include <QLabel>
#include <QPoint>
#include <QLCDNumber>
#include <QDoubleSpinBox>
#include <KDE/KLocale>
#include <KDE/KLocalizedString>
#include <QCheckBox>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QStatusBar>
#include <QPalette>
#include <QColor>
#include <QBrush>
#include <QDialog>
#include <QInputDialog>
#include <QTimer>
#include <QMainWindow>
#include<QTextBrowser>
//#include <QAbstractSocket>
#include "ui_klog.h"
// The next two are for the cluster box
#include <QTcpSocket>
//#include <QUrl>
// The next two are for printing
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <kdeprintdialog.h>

#include <QProgressDialog>
#include <QSplashScreen>
#include <QTabWidget>
#include <QStringList>
#include <KDE/KIcon>

#include "awarddxcc.h"
#include "awardwaz.h"
#include "qso.h"
#include "calls.h"
#include "world.h"
#include "locator.h"
#include "setup.h"
#include "adif.h"
#include "workedcalls.h"
#include "kloghamlib.h"
#include "awards.h"
#include "award.h"
#include "reference.h"
#include "cabrillomanagement.h"
#include "klognetwork.h"
// To use the dxmap just uncomment out the define tag below. It will activate all relevant code. Still broken though.
//#define DXMAP 1
#ifdef DXMAP
#include "dxmap.h"
#endif

using namespace std;

// BANDS 9 {10/12/15/17/20/30/40/80/160}
// MODES 11 {SSB/CW/FM/PSK31/SSTV/RTTY/TOR/PKT/
//          AM/ATV/PAC/CLO
//
// CONTINENTS AF, AN, AS, EU, NA, OC, SA

//const char SEPARATOR = ',';

/** Klog is the base class of the project */

class Klog : public QMainWindow, private Ui::klog
{
    Q_OBJECT
    friend class dxClusterSpotItem;
    friend class bandMapSpotItem;

    protected:
     void closeEvent(QCloseEvent *event);

    public:
    /** construtor */
    Klog(const QString& tversion, QMainWindow *parent = 0);
    /** destructor */
    ~Klog();
    //Qso qso;

    World world;
//    Entity entity;
    WorkedCalls workedCall;// worked and it is posible to reuse the data.

    typedef QList<Qso> LogBook;

    LogBook logbook;
    LogBook oLogbook; //Ordered logbook (by date) (exit from orderLog)
    LogBook templogbook; //The "not yet saved" logbook.

    //public:
    void haveWorld();

//	virtual void run();

    public slots:
    void slotUpdateTime();
    void slotQSLRec();
    void slotQSLSeveralRec();
    void slotQSLSent();
    void slotQSLRecSent();
    void slotQsoDelete();
    //  slotDeleteQso();
    void slotCancelSearchButton();
    void slotSearchButton();
    void slotQrzChanged();
    void slotOkBtn();
    void slotClearBtn();
    void clearEntityBox();
    void slotQslSentBoxChanged();
    void slotQslRecvBoxChanged();
    void slotQSLcomboBoxChanged();
    void slotSearchQSO2QSL(); // Shows in the search box QSO from which we need the QSL and we have not sent it.
    void slotPreferences();
    void fileNew();
    void fileSave();
    void fileSaveAs();
    void filePrint();
    //void printLabels(); // Generate the file for GLabels or KBarcode
    void slothamlibUpdateFrequency();
    void slotUpdateCTYDATFile();
    void slotUpdateCTYDATFileManually();

//	void adifFileSave();
    void adifTempFileSave(const QString& fn, LogBook lb, bool manualSave);  // To save the templog automatically after each QSO is added
    //  void adifSaveAs(const QString& tfileName);
    void adifReadLog(const QString& tfileName);
//	void cabrilloReadLog(const QString& tfileName);
    int getProgresStepForDialog(int totalSteps);
    //  void newAdifReadLog (const QString& tfileName);
    void tlfReadLog(const QString& tfileName);
    void cabrilloReadLog(const QString& tfileName);
    void fileOpen();
    void fileExit();
    void helpAbout();
    void readQso(); //Just read the values an fill the qso
    void modifyQso(); // Modify an existing QSO with the data on the boxes
    void slotQsoSelectedForEdit(QTreeWidgetItem *);//Puts the QSO in the edit box
    void slotQsoSearchSelectedForEdit( QTreeWidgetItem* item, int); //Puts the QSO in the edit box
    //void slotQsoSearchSelectedForEdit( QTreeWidgetItem* item); //Puts the QSO in the edit box
    void slotQsoSelected (QTreeWidgetItem* item); // Just to see qso data
    void slotLogQSOSelectionChanged(); // Grabs the selected QSO from the log
//    void contextMenuEvent(QContextMenuEvent*); // To detect the right clicks
    void showRighButtonSearchMenu( const QPoint& pos );
    void showRighButtonLogMenu( const QPoint& pos );
    void slotModifyBtn();
    void showLogList();   // Shows the log in the botton widget
//    void showMenuRightButton(int qqso, const QPoint &p); //This is called from  slotQsoRightButton and show the actions menu
    void showMenuRightButton(int qqso); //This is called from  slotQsoRightButton and show the actions menu

// Printing modules
//     void doPrint(KPrinter *printer);
//     void doPaint(QPainter *p, int autofit, int w, int h);
//  void slotPrint();
//  void doPrint(KPrinter *printer);
//  void doPaint(QPainter *p, int autofit, int w, int h);

    void slotAddLog();
    void slotImportTlf();
    void slotImportCabrillo();
    void slotExportNeeded();	// Export all the needed QSO (new ones and new bands)
    void slotIOTAChanged();
    void sortLog(); //To order by date the log and produce OLog
    void slotcompleteThePreviouslyWorked();

  Qso getByCall(const QString& tqrz);
  Qso getByNumber(const int n);
  void showQso(); // Gets data from the Input part and show in the botton widget
  void toEditQso(); //Puts the QSO data in the edit box
  void accept();

  private slots:
    void slotLocatorChanged();
    void slotMyLocatorChanged();
    void slotClusterCloseConnection();
    void slotClusterSendToServer();
    void slotClusterSocketReadyRead();
    void slotClusterSocketConnected();
    void slotClusterSocketConnectionClosed();
    void slotClusterSocketClosed();
    //void slotClusterSocketError( int e );
    void slotClusterSocketError(QAbstractSocket::SocketError socketError);
    void slotClusterConnect();
    void slotClusterClearInputLine();
    void slotClusterSpotToLog(QListWidgetItem* item);
    void slotClusterSpotCheck(QListWidgetItem* item);
    void slotClusterSpotSelectionChanged();
//    void slotClusterSetup();
//    void slotKlogSetup();
    void slotBugReport();
    void slotModeChanged (int i);
    void slotBandChanged ();
    void slotTXFreqChanged(double i);
    void fillDXCCList();	// Fill the KLog tab


  private:
    Qso qso, qsoSelected, qsoSearchSelected;
//	CabrilloManagement cabrilloLog;
//	Qso previousQso;       // This and workedCall is to check if a call has been previously
    bool requestDownloadctydat;
    bool completeWithPrevious;	// Configuration parameter
    bool completedWithPrevious;
    bool qsoSelectedBool, qsoSearchSelectedBool; 	//To know if a QSO has been selected at least once
    bool requireMandatory;		// Configuration parameter (requires all the mandatory data for all the QSO) (QRZ, date, time, band, mode, RST (TX&RX))
    bool bandFreqLock; // Avoids the deadlock of automatically changing band combobox to freqTX and viceversa forever
    QString tempLogFile;	// Temp log to save automatically

    typedef QList<Reference> References; // The list of references of the current award.
    References references;		// The references of the current award.


    // Awards supported by KLog
#ifdef DXMAP
    DXMap *dxMap;
#endif
    Adif adif;
    AwardDXCC dxcc;
    AwardWAZ waz;
    Awards awards;
    Award award;
    QStringList awardReferences;	// All the references of the current award to prepare the combobox
    QString DXClusterServerToUse;
    void processLogLine (const QString& tLogLine); // Process a line of data/qso from adifReadLog
    void clearGUI();
    bool didISave();
    void createActions();
    bool haveAllTheFields();
    void entityState(const int tentity);
    void addQSOToLog();
    int howManyConfirmedQSO();
    void createKlogDir();
    void readConf();
    void prepareIOTAComboBox (const int tenti);
    void prepareAwardBox (const int tenti);
    void readAwardsStatus();	//Reads the full DXCC&WAZ status
    bool checkIfValidDXCluster(const QString &tdxcluster); // Is a valid URL?
//    void slotQslNeededCheck();
    void setMyQrz(const QString &tqrz);
    QString getMyQrz() const;
    void setMyLocator(const QString &tlocator);
    QString getMyLocator() const;
    void showWhere(const int enti);
    QString getThisQSODXLocator ();
    int needToWorkFromCluster(const QString &tqrz, const int tband); // 0 confirmed, 1 worked, 2 new one, -1 error
//    QColor stateColor(const QString &tqrz, const int tband);
    void showAwardsNumbers(); // Shows how many worked, confirmed, ...
    void clearAwards();
    void showDistancesAndBeam(const int dist, const int beam); // Shows distance and beam

// Worked before
    void completeAllQSOsFromLog();
    //void KloggetAllTheCallsFromLog(); // To be run once after the log is read from the disk.
    void fillEntityBandState(const int enti);
    void showIfPreviouslyWorked(); // Uses previousQso and workedCall
    //void addToPreviouslyWorked(const QString &qrz); // Uses previousQso and workedCall
    void getAllTheCallsFromLog();// To be run once after the log is read from the disk.

    QString getNumberString(const int intNumber); //Receives a number a returns a string of a fixed length
    QString getShortNumberString(const int intNumber); //Same that previous but with 3 characters
    int getEntityFromCall(); // Just read the call from the QRZ box and returns the entity number

    void prepareAwardComboBox(const int tenti);
    QString checkExtension(QString extension); // To check the extension of the file // TNX Kike

    void listHamlib();
    void addDXSpotToBandMap(QString& freq, QString& dx, QString& from, QColor& dxcolor); // Adds a DXSpot to the bandmap from dxcluster
    void showTip();
    QString returnLines(const QString& tword); //Puts \n where \\n
   void showMenuRightButtoncreateActions();
   bool checkCTYDATFile(); // Checks if the cty.dat file is in the ~/.klog directory
//    void showMenuRightButtoncreateMenus();
//    void showMenuRightButtoncontextMenuEvent(QContextMenuEvent *event);
//    int band2Num(const QString &tband);

  //  Qso qso;
    bool showProgressDialog;
    bool qslViac;
    //bool itemSearchClicked;  // To know if a QSO has been clicked on the search box
    QColor ledColor;
    Locator locator;
    QString myLocatorTemp;  // To manage temp locators and distances
    QString dxLocator;
    QString myQrz;
    QString myLocator;
    QString klogDir;
    QString logFileNameToOpen;
    QString logFileNameToSave;
    QString aux;
    QString operatorStringAux;
    QString textStringAux;
    QString KLogVersion;
    QString dateString;
    QString timeString;
    QString datesString;
    QString daterString;
    QString qsoLine;
    QString adifTab;
    QString theData;


    QPrinter printer;
//    QPainter p;

    QPalette palette; // For cluster
    QPainter clusterPainter; // For cluster
    QPalette qrzDefaultPalette, qrzWorkedB4Palette; // palette for the QRZLineEdit to now if a QRZ has been previously worked.
    QColor confirmedColor;
    QColor workedColor;
    QColor neededColor;
    QColor newOneColor;
    QColor defaultColor;
    QColor dxSpotColor;
    QColor blackColor;
    QColor colorInUse;  // Till now only in use in Klog::slotSearchButton()
    QBrush qbrush; // for the DXCluster colors
    int number;  // QSO number
    //int confirmed; //confirmed QSOs
    int rstrx;  // RST received from the other peer
    int rsttx;  // RST you pass to the other peer
    int tTxValue;
    int tRxValue;
    int sTxValue;
    int sRxValue;


    int band;   // Band used in the QSO
    int imode;
    QString power;  // Power used for the QSO (in your side)
    QStringList fields; // Auxiliary variable to split log/formated strings
    int i, j;
    int kk; // This is just for intra-fuction use
    int itemPos; // for modifying
    int enti;   //Just for entity name show
    int entiBak;
    int callLen;  // The call's lenght
    int callLenPrev;  // The previous call's lenght to detect if deleting
    int callLenFound;
    int beam; // the beam in degrees DELETE
    int distance; // Distance between the two stations in the QSO
    bool lastDelete;  // Last key pressed is delete?
    bool callFound;   // Boolean to show if the call has been found
    bool modify;
    bool addingLog;		// I am adding a logfile to the main log (to keep my_locator)
    bool searching2QSL;
    bool prefixFound;
    bool dirExist;
    bool needToSave;
    bool actionSent;
    bool actionRec;
    bool timeInUTC;   // Time in UTC or LocalTime
    bool realTimeLog; // The clock will be in real time or stopped to add previous QSOs
//bool justOneTime; //  For slotQslRec to avoid twice confirmed--
    //bool wasConfirmed; // For the same purporse as above.
    bool openLastByDefault;
    QString mode;
    QString qrz;  // Just used in the searching functions
    QString iota;
    QString localAward;	// A String as the iota to temporaly save the localAward (if any)
    QDateTime dateTime;
    QDateTime dateTimeContinuous;
    QDate qslSen;
    QDate qslRec;
    //QDate date;
    //QTime time;
    QString comment;
    QString dateTimeString;
    //Preferences setupDialog;

    QPixmap *editdeletePixMap;
    QPixmap *editdeleteOffPixMap;
    QPixmap *qslRecPixMap;
    QPixmap *qslRecOffPixMap;
    QPixmap *qslSenPixMap;
    QPixmap *qslSenOffPixMap;
// Cluster variables
//	int neededWorkedConfirmed; // 0 needed, 1 worked, 2 confirmed, 3 default
    QTcpSocket *socket;
    QLineEdit *inputText;
    QString dxClusterHost;
    QString dxClusterString;
    QString dxClusterDxCall;
//	QString dxClusterShow;
//QString dxClustertmp;
    int dxClusterPort;
    int dxClusterCallLen;
    int dxClusterEnti;
    int dxClusterBand; // tmp variable
    bool dxClusterConnected;
    bool dxClusterHFSpots;
    bool dxClusterVHFSpots;
    bool dxClusterWARCSpots;
    bool dxClusterCWSpots;
    bool dxClusterSSBSpots;
    bool dxClusterWXANNounces;
    bool dxClusterWCYANNounces;
    bool dxClusterANNounces;
    bool dxClusterConfirmedSpots;
//void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment );
//  virtual void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int align );

//	KBugReport  *bugReport;
    // 0 QRZ
    // 1 Date
    // 2 Time
    // 3 Band
    // 4 Mode
    // 5 RST tx
    // 6 RST rx
    bool haveAllMandatoryFields[7];

    // hamlib support
    kloghamlib KlogHamlib;
    bool hamlib;
    bool hamlibPossible;
    double hamlibFreq;
    int bauds;
    int rignameNumber;
    //int strength;
    int hamlibInterval;
    QString serialPort;
    QTimer *hamlibtimer;
    QMenu *qsoMenu;

    
    QAction *delQSOAct;
    QAction *recSenQSOAct;
    QAction *recQSOAct;
    QAction *senQSOAct;
   // QAction *actionUpdateCtyDat;
    //QAction *actionUpdateCtyDatManually;
    QActionGroup *qsoActionsGroup;
    
    
    
};

class dxClusterSpotItem : public QListWidgetItem {
public:
dxClusterSpotItem( QListWidget* parent, const QString& spot, const QColor& color );
~dxClusterSpotItem();
//virtual void paintCell( QPainter *p, const QPalette &cg, int column, int width, int alignment );

//Message *message() { return myMessage; }

protected:
  //  Message *myMessage;
  QColor spotColor;

};


class searchBoxItem : public QTreeWidgetItem {
public:
searchBoxItem( QTreeWidget *parent, const QString& call, const QString& date, const QString& time, const QString& band, const QString& mode, const QString& RSTsent, const QString& RSTrec, const QString& numb, const QColor& color );
~searchBoxItem();
//virtual void paintCell( QPainter *p, const QPalette &cg, int column, int width, int alignment );


protected:
  QColor qsoColor;
  QString call;
  QString date;
  QString band;
  QString mode;
  QString RSTsent;
  QString RSTrec;
};




class dxccItem : public QTreeWidgetItem {
public:
dxccItem( QTreeWidget *parent, const QString& numb, const QString& ent, const QString& pref, const QString& m10, const QString& m12, const QString& m15, const QString& m17, const QString& m20, const QString& m30, const QString& m40, const QString& m80, const QString& m160, const QColor& color );

~dxccItem();
//virtual void paintCell( QPainter *p, const QColorGroup &cg,int column, int width, int alignment );


protected:
  QColor qsoColor;

};


#endif
