/***************************************************************************
                          setup.h  -  description
                             -------------------
    begin                : mar jul 29 2003
    copyright            : (C) 2003 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
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

#ifndef SETUP_H
#define SETUP_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <qwidget.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
//#include <kcombobox.h>
//#include <qcombobox.h>
#include <qlistview.h>

#include <qdir.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qcolordialog.h>
//#include <qpalette.h>
#include <klineedit.h>
//#include <knuminput.h>
#include <qspinbox.h>
#include <kcolorcombo.h>
//#include <kpushbutton.h>
#include <kpopupmenu.h>
#include <qurl.h>
#include <qinputdialog.h>
#include "kloghamlib.h"
#include "setupui.h"
#include "kloghamlib.h"
#include "award.h"
#include "awards.h"
#include <qdialog.h>



//#include "hamlib/rig.h"

//#include "ostream.h"


class Setup : public setupui{
  Q_OBJECT
  public:
    /** construtor */
    Setup(QWidget* parent=0, const char *name=0);
    /** destructor */
    ~Setup();

	//QColorDialog colorDialog;

  public slots:
  void setRig();

  private slots:
	void slotOkBtnClicked();
	void slotFileNameClicked();
	void slotOpenFileButtonClicked();
	void slotSearchAwardFilekPushButtonClicked();
	void slotAddButtonClicked();
	void slotAwardClearButtonClicked();
	void slotAwardRemoveButtonClicked();
	void slotAddDxClusterPushButtonClicked(); //Adds a dxcluster server
	void slotClearDxClusterPushButtonClicked();
	void slotAwardRightButtonFromList(QListViewItem * item, const QPoint &p);
	void slotAwardSelected(QListViewItem* item);
	void slotAwardDelete ();
//	void showMenuRightButton(int taward, const QPoint &p);
	void showMenuRightButton(QListViewItem * item, const QPoint &p);
	void slotConfirmedColorPushButton();
	void slotDefaultColorPushButton();
	void slotNewOneColorPushButton();
	void slotNeededColorPushButton();
	void slotWorkedColorPushButton();

	
	int getLastAwardNumber();
	bool checkIfValidDXCluster(const QString &tdxcluster); // Is a valid URL?
	bool checkIfNewDXCluster(const QString &tdxcluster); //Checks if the dxcluster added is repeated or new
	int bauds2Num(const QString& tbauds); // Returns the number of the Combobox from the bauds

  private:
	void readConf();
	void writeConf();
	bool dirExist;
	QColor giveColor(QColor c);
	QString fileName;	
	QString awardFileName;
	QString klogDir;
	QString DXClusterServerToUse;
	QString aux;
	QString aux2;
	QColor color;
	QColorGroup colorGroup;
	
	int power;
	int i; // Auxiliar variable
	int j; // Auxiliar variable
	int awardN;
	int awardToDelete;
	Award award;
	int dxclusterServerListItems;
	int DXclusterServerPort;
	QString DXClusterServer;
//  confirmedkColorCombo
//  workedkColorCombo
//  neededkColorCombo
	static int addRigToList(const struct rig_caps* caps, void* data);
	QMap<QString, rig_model_t> rigname2rigid;
	QMap<rig_model_t, QString> rigid2rigname;
	QStringList strings;
	QStringList prefixesOfAwards;
	//QString fileOfAwardToErase;
	QPixmap *editdeletePixMap;
	//QListViewItem awarditem;
	

};

#endif
