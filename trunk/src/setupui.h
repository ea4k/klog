/****************************************************************************
** Form interface generated from reading ui file './setupui.ui'
**
** Created: jue sep 13 17:59:04 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SETUPUI_H
#define SETUPUI_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTabWidget;
class QWidget;
class QLabel;
class KLineEdit;
class QFrame;
class QCheckBox;
class KPushButton;
class KIntNumInput;
class KComboBox;
class QGroupBox;
class QComboBox;
class KColorCombo;
class QSpinBox;
class QLineEdit;
class QListView;
class QListViewItem;

class setupui : public QDialog
{
    Q_OBJECT

public:
    setupui( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~setupui();

    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QTabWidget* tabWidget2;
    QWidget* tabPersonal;
    QLabel* textLabel1;
    KLineEdit* MyLocatorkLineEdit;
    QLabel* textLabel2;
    KLineEdit* MyCallkLineEdit;
    QWidget* tab;
    QFrame* frame3;
    QCheckBox* lastFilecheckBox;
    KLineEdit* filenamekLineEdit;
    KPushButton* searchFilekPushButton;
    QCheckBox* checkBoxProgressDialog;
    QCheckBox* UTCcheckBox;
    QCheckBox* realTimeLogCheckBox;
    QLabel* textLabel1_4;
    KIntNumInput* powerkIntNumInput;
    QLabel* textLabel1_5;
    KComboBox* bandkComboBox;
    KComboBox* modekComboBox;
    QLabel* textLabel2_4;
    QWidget* tabCluster;
    QGroupBox* groupBox3;
    QCheckBox* HFcheckBox;
    QCheckBox* VHFcheckBox;
    QCheckBox* WARCcheckBox;
    QCheckBox* confirmedcheckBox;
    QGroupBox* groupBox2;
    QCheckBox* ANNcheckBox;
    QCheckBox* WCYcheckBox;
    QCheckBox* WXcheckBox;
    QPushButton* addDxClusterpushButton;
    QPushButton* clearDxClusterpushButton;
    QComboBox* dxClustercomboBox;
    QWidget* tab_2;
    QLabel* textLabel1_3;
    QLabel* textLabel2_3;
    QLabel* defaulttextLabel;
    QLabel* textLabel3;
    KColorCombo* confirmedkColorCombo;
    KColorCombo* workedkColorCombo;
    KColorCombo* neededkColorCombo;
    KColorCombo* defaultkColorCombo;
    QWidget* TabPage;
    QCheckBox* completeWithPreviouscheckBox;
    QCheckBox* mandatoryDatacheckBox;
    QWidget* TabPage_2;
    QCheckBox* hamlibcheckBox;
    QSpinBox* hamlibIntervalspinBox;
    QLabel* textLabel1_6;
    QLabel* textLabel1_7;
    QLineEdit* devicelineEdit;
    QComboBox* comboBoxTrvSpeedList;
    QLabel* textLabel1_8;
    QComboBox* comboBoxTrvList;
    QWidget* TabPage_3;
    KPushButton* awardFilekPushButton;
    QPushButton* awardRemovepushButton;
    KLineEdit* awardFilenamekLineEdit;
    QPushButton* awardClearpushButton;
    QPushButton* awardAddpushButton;
    QListView* awardListView;

public slots:
    virtual void slotOkBtnClicked();
    virtual void slotCancelBtnClicked();
    virtual void slotFileNameClicked();
    virtual void slotOpenFileButtonClicked();
    virtual void slotSearchAwardFilekPushButtonClicked();
    virtual void slotAddButtonClicked();
    virtual void slotAwardClearButtonClicked();
    virtual void slotAwardRemoveButtonClicked();
    virtual void slotAddDxClusterPushButtonClicked();
    virtual void slotClearDxClusterPushButtonClicked();
    virtual void slotAwardSelected(QListViewItem* item);
    virtual void slotAwardRightButtonFromList(QListViewItem * item, const QPoint &p);
    virtual void slotAwardDelete(int tint);

protected:
    QGridLayout* setupuiLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;
    QGridLayout* tabPersonalLayout;
    QSpacerItem* spacer3;
    QSpacerItem* spacer2;
    QGridLayout* tabLayout;
    QGridLayout* frame3Layout;
    QVBoxLayout* layout5;
    QHBoxLayout* layout4;
    QHBoxLayout* layout2;
    QGridLayout* tabClusterLayout;
    QHBoxLayout* layout10;
    QGridLayout* groupBox3Layout;
    QGridLayout* groupBox2Layout;
    QHBoxLayout* layout13;
    QSpacerItem* spacer8_2;
    QGridLayout* tabLayout_2;
    QSpacerItem* spacer9;
    QSpacerItem* spacer10;
    QSpacerItem* spacer10_2;
    QSpacerItem* spacer8;
    QGridLayout* TabPageLayout;
    QGridLayout* TabPageLayout_2;
    QHBoxLayout* layout6;
    QHBoxLayout* layout5_2;
    QHBoxLayout* layout10_2;
    QHBoxLayout* layout7;
    QHBoxLayout* layout14;
    QGridLayout* TabPageLayout_3;
    QSpacerItem* spacer4_2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

};

#endif // SETUPUI_H
