/****************************************************************************
** Form interface generated from reading ui file './klogui.ui'
**
** Created: jue sep 13 17:59:03 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef KLOGUI_H
#define KLOGUI_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QTabWidget;
class QWidget;
class QLabel;
class QLCDNumber;
class KListView;
class QListViewItem;
class KLineEdit;
class KPushButton;
class QFrame;
class QDateTimeEdit;
class KComboBox;
class KLed;
class QSpinBox;
class QDateEdit;
class QCheckBox;
class KTextEdit;
class QComboBox;
class KIntSpinBox;
class QLineEdit;
class QListView;

class klogui : public QMainWindow
{
    Q_OBJECT

public:
    klogui( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~klogui();

    QTabWidget* outputTab;
    QWidget* info;
    QLabel* LedtextLabel;
    QLabel* entityTextLabel;
    QLabel* textLabelBand10;
    QLabel* textLabelBand15;
    QLabel* textLabelBand20;
    QLabel* textLabelBand40;
    QLabel* textLabelBand80;
    QLabel* textLabelBand160;
    QLabel* textLabelBand2;
    QLabel* textLabelBand6;
    QLabel* textLabelBand12;
    QLabel* textLabelBand17;
    QLabel* textLabelBand30;
    QLabel* textLabelBand70cm;
    QLabel* prxTextLabel;
    QLabel* prxtTextLabel;
    QLabel* continentTextLabel;
    QLabel* continenttTextLabel;
    QLabel* textLabel5;
    QLCDNumber* distancelCDNumber;
    QLabel* showtPathtextLabel;
    QLabel* textLabel11;
    QLCDNumber* beamshortlCDNumber;
    QLabel* itutTextLabel;
    QLabel* cqtTextLabel;
    QLCDNumber* cqLCDNumber;
    QLCDNumber* ituLCDNumber;
    QLCDNumber* beamlonglCDNumber;
    QLabel* textLabel10;
    QLCDNumber* distancellCDNumber;
    QLabel* textLabel12;
    QLabel* textLabel13;
    QWidget* awards;
    QLabel* textLabel1_5;
    QLabel* textLabel3_3;
    QLabel* dxcctextLabel;
    QLabel* textLabel2;
    QLabel* textLabel4;
    QLabel* localAwardName;
    QLabel* textLabel1_9;
    QLCDNumber* IOTAlCDNumber;
    QLCDNumber* LOCALlCDNumber;
    QLCDNumber* confirmedQSOlCDNumber;
    QLCDNumber* DXCCworkedlCDNumber;
    QLCDNumber* WAZworkedlCDNumber;
    QLCDNumber* IOTAworkedlCDNumber;
    QLCDNumber* LOCALworkedlCDNumber;
    QLCDNumber* workedQSOlCDNumber;
    QLabel* textLabel2_3;
    QLCDNumber* DXCClCDNumber;
    QLCDNumber* WAZlCDNumber;
    QWidget* tab;
    KListView* searchQsoskListView;
    QLabel* textLabel6_2;
    KLineEdit* previousQthkLineEdit;
    QLabel* textLabel7_2;
    QLabel* textLabelTime;
    KLineEdit* timekLineEdit;
    KLineEdit* previousNamekLineEdit;
    KLineEdit* searchQrzkLineEdit;
    KPushButton* searchSearchkPushButton;
    KPushButton* searchCancelkPushButton;
    QFrame* frame3;
    QDateTimeEdit* qsoDateTime;
    KPushButton* clearBtn;
    KPushButton* okBtn;
    KLineEdit* qrzLineEdit;
    KComboBox* modeComboBox;
    KComboBox* bandComboBox;
    KLed* newEntitykLed;
    QTabWidget* inputTab;
    QWidget* qso;
    QLabel* textLabel1_2;
    QSpinBox* RSendBox;
    QSpinBox* RRecBox;
    QSpinBox* SSendBox;
    QSpinBox* SRecBox;
    QSpinBox* TSendBox;
    QSpinBox* TRecBox;
    KLineEdit* namekLineEdit;
    QLabel* textLabel1_8_2;
    QLabel* textLabel2_6_2;
    KLineEdit* qthkLineEdit;
    KLineEdit* locatorLineEdit;
    QLCDNumber* freqlCDNumber;
    QWidget* qsl;
    QLabel* textLabel2_2;
    QDateEdit* QSLRecdateEdit;
    QCheckBox* QSLSentcheckBox;
    QLabel* textLabel3_2;
    QDateEdit* QSLSentdateEdit;
    QCheckBox* QSLReccheckBox;
    KLineEdit* qslVialineEdit;
    KComboBox* QSLcomboBox;
    KTextEdit* QSLInfotextEdit;
    QLabel* textLabel4_3;
    QWidget* remarks;
    QLabel* remarksTextLabel;
    KTextEdit* remarksTextEdit;
    QWidget* others;
    KComboBox* iotaComboBox;
    QLabel* awardSelectorName;
    QComboBox* awardsComboBox;
    KIntSpinBox* iotaIntSpinBox;
    QLabel* textLabel;
    QWidget* TabPage;
    QLabel* powerTextLabel;
    QSpinBox* powerSpinBox;
    QLabel* textLabel2_6_2_2;
    QLineEdit* myLocatorLineEdit;
    KLineEdit* operatorLineEdit;
    QLabel* operatortextLabel_2;
    QTabWidget* downTabs;
    QWidget* logdisplayTab;
    QListView* logListView;
    QWidget* clusterTab;
    KListView* dxClusterkListView;
    KLineEdit* ClusterkLineEditInPut;
    KPushButton* ClusterkPushButtonClear;
    KPushButton* ClusterkPushButtonOK;
    QMenuBar *menubar;
    QPopupMenu *fileMenu;
    QPopupMenu *PopupMenu;
    QPopupMenu *setupMenu;
    QPopupMenu *PopupMenu_2;
    QPopupMenu *helpMenu;
    QToolBar *toolBar;
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* filePrintAction;
    QAction* fileExitAction;
    QAction* editUndoAction;
    QAction* editRedoAction;
    QAction* editCutAction;
    QAction* editCopyAction;
    QAction* editPasteAction;
    QAction* editFindAction;
    QAction* helpContentsAction;
    QAction* helpIndexAction;
    QAction* helpAboutAction;
    QAction* setupAction;
    QAction* addTlfLogAction;
    QAction* ActionQsoDelete;
    QAction* ActionQsoSen;
    QAction* ActionQslRec;
    QAction* ActionAddKlogLog;
    QAction* ActionDXClusterConnect;
    QAction* ActionDXClusterDisconnect;
    QAction* ActionDXClusterSetup;
    QAction* ActionBugReport;
    QAction* ActionQslNeededCheck;
    QAction* ActionSortLog;
    QAction* toolsFind_QSO_to_QSLAction;

public slots:
    virtual void fileExit();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void filePrint();
    virtual void slotQsoSearchOver( QListViewItem * );
    virtual void slotQSLcomboBoxChanged();
    virtual void slotModifyBtn();
    virtual void slotClearBtn();
    virtual void slotOkBtn();
    virtual void helpIndex();
    virtual void helpContents();
    virtual void helpAbout();
    virtual void slotBandSelected();
    virtual void slotModeSelected();
    virtual void slotQrzChanged();
    virtual void slotQsoSelected(QListViewItem* item);
    virtual void slotQSLSent();
    virtual void slotQSLrec();
    virtual void slotQslSentBoxChanged();
    virtual void slotQslRecvBoxChanged();
    virtual void slotPreferences();
    virtual void slotImportTlf();
    virtual void slotIOTAChanged();
    virtual void slotSearchButton();
    virtual void slotCancelSearchButton();
    virtual void slotQrzSearchChanged();
    virtual void slotQsoSearchSelected( QListViewItem * );
    virtual void slotQsoRightButtonFromSearch( QListViewItem *, const QPoint & );
    virtual void slotQsoSelectedForEdit( QListViewItem * );
    virtual void slotQSLRec();
    virtual void slotQsoSearchSelectedForEdit( QListViewItem * );
    virtual void slotQsoDelete();
    virtual void slotQsoRightButton( QListViewItem * );
    virtual void slotQsoRightButtonFromLog( QListViewItem *, const QPoint & );
    virtual void slotAddLog();
    virtual void slotClusterConnect();
    virtual void slotClusterCloseConnection();
    virtual void slotClusterSendToServer();
    virtual void slotClusterClearInputLine();
    virtual void slotKlogSetup();
    virtual void slotBugReport();
    virtual void slotModeChanged(int );
    virtual void slotBandChanged();
    virtual void slotQslNeededCheck();
    virtual void slotLocatorChanged();
    virtual void slotClusterSpotToLog(QListViewItem *);
    virtual void slotClusterSpotCheck(QListViewItem * item);
    virtual void sortLog();
    virtual void fileNew();
    virtual void slotLocalAwardChanged();
    virtual void slotSearchQSO2QSL();
    virtual void slotMyLocatorChanged();

protected:
    QGridLayout* kloguiLayout;
    QVBoxLayout* infoLayout;
    QHBoxLayout* layout15_2;
    QHBoxLayout* layout16;
    QGridLayout* layout29;
    QGridLayout* layout27;
    QGridLayout* layout23;
    QGridLayout* layout28;
    QGridLayout* layout22;
    QGridLayout* awardsLayout;
    QGridLayout* tabLayout;
    QVBoxLayout* layout20;
    QHBoxLayout* layout21;
    QGridLayout* frame3Layout;
    QSpacerItem* Spacer2_2;
    QHBoxLayout* layout75;
    QGridLayout* qsoLayout;
    QHBoxLayout* layout22_2;
    QVBoxLayout* layout26;
    QVBoxLayout* layout27_2;
    QVBoxLayout* layout28_2;
    QHBoxLayout* layout21_2;
    QGridLayout* qslLayout;
    QGridLayout* layout19;
    QGridLayout* layout22_3;
    QVBoxLayout* remarksLayout;
    QGridLayout* othersLayout;
    QGridLayout* TabPageLayout;
    QSpacerItem* Spacer2_2_2;
    QHBoxLayout* layout23_2;
    QVBoxLayout* logdisplayTabLayout;
    QGridLayout* clusterTabLayout;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;
    QPixmap image5;
    QPixmap image6;
    QPixmap image7;
    QPixmap image8;
    QPixmap image9;
    QPixmap image10;
    QPixmap image11;
    QPixmap image12;
    QPixmap image13;

};

#endif // KLOGUI_H
