/***************************************************************************
                          setup.cpp  -  description
                             -------------------
    begin                : mar jul 29 2003
    copyright            : (C) 2003 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/****************************************************************************
*                                                                           *
*   This program is free software; you can redistribute it and/or modify    *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
*                                                                           *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
*  GNU General Public License for more details.                             *
*                                                                           *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA  *
*****************************************************************************/

#include <KDE/KLocale>
#include <KDE/KIcon>
//#include <QDebug>
//#include <ostream.h>
#include "setup.h"

Setup::Setup (QDialog * parent):QDialog (parent)
{
  setupUi (this);
  klogDir = QDir::homePath () + "/.klog";	// We create the ~/.klog for the logs
  if (!QDir::setCurrent (klogDir))
    {
      QDir
      d1 (klogDir);
      dirExist = d1.mkdir (klogDir);
    }
  dirExist = QDir::setCurrent (klogDir);

  searchFilekPushButton->setIcon(KIcon("document-open"));
  awardFilekToolButton->setIcon(KIcon("document-open"));
  
  connect(awardFilekToolButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()));
//   connect(awardFilekToolButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()));
 // connect(searchFilekPushButton, SIGNAL(clicked()), this, SLOT(slotFileNameClicked()));
//connect(awardTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotAwardRightButtonFromList(QTreeWidgetItem * item)));
connect(awardTreeWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(showMenuRightButton( const QPoint& ) ) );

  ///////////////////////////////////////////////////////////////////////////////
  // Modifications by kike
  // 25-03-2004
  //////////////////////////////////////////////////////////////////////////////
  // Fill the rig combo
  setRig ();
  //////////////////////////////////////////////////////////////////////////////
  // End of modifications
  /////////////////////////////////////////////////////////////////////////////

  readConf();

  slotOpenFileButtonClicked ();
  awardToDelete = -1;
}

Setup::~Setup ()
{
}

void Setup::slotOkBtnClicked()
{
  writeConf();
  accept();
}

void
Setup::slotConfirmedColorPushButton ()
{
//qDebug()  << "Setup::slotConfirmedColorPushButton " << endl;
  palette = confirmedPushButton->palette ();
  confirmedPushButton->setPalette (giveColor(confirmedPushButton->palette().color(QPalette::Button)));
}

void
Setup::slotWorkedColorPushButton ()
{
//qDebug()  << "Setup::slotWorkedColorPushButton " << endl;
  palette = workedPushButton->palette ();
  workedPushButton->setPalette (giveColor(workedPushButton->palette().color(QPalette::Button)));
}

void
Setup::slotNeededColorPushButton ()
{
//qDebug()  << "Setup::slotNeededColorPushButton " << endl;
  palette = neededPushButton->palette ();
  neededPushButton->setPalette(giveColor(neededPushButton->palette().color(QPalette::Button)));
}

void
Setup::slotNewOneColorPushButton ()
{
//qDebug()  << "Setup::slotNeededColorPushButton " << endl;
  palette = newOnePushButton->palette();
  newOnePushButton->setPalette(giveColor(newOnePushButton->palette().color(QPalette::Button)));
}

void
Setup::slotDefaultColorPushButton ()
{
//qDebug()  << "Setup::slotDefaultColorPushButton " << endl;
  palette = defaultPushButton->palette ();
  defaultPushButton->setPalette(giveColor(defaultPushButton->palette().color(QPalette::Button)));
}

QColor
Setup::giveColor (QColor c)
{
  // Receives the actual color, shows the user a color picker and returns the color that the user selects.
  QColor colorb;
  color = c;
  colorb = color;
  color = QColorDialog::getColor (color, this, "color dialog");
  if (color.isValid ())
    {
      return color;
    }
  else
    {
      return colorb;
    }

}

void
Setup::readConf ()
{
  QFile file ("klogrc");
  if (file.open (QIODevice::ReadOnly))
    {
      QTextStream stream (&file);
      QString adifTab;
      QString theData;
      QString data = "";
      data = "";
      //DXclusterServerPort = 41112;
      DXclusterServerPort = -1;
      DXClusterServer = "NOSERVER";
      DXClusterServerToUse = "NOSERVER";
      dxclusterServerListItems = -1;

      awardN = 0;
      prefixesOfAwards.clear ();
      awardTreeWidget->clear ();

      while (!stream.atEnd ())
    {
      data = stream.readLine ();
      if (!data.isEmpty ())
        {			// If there is another line i read it
          data = data.simplified ();
          QStringList fields = data.split ("=", QString::KeepEmptyParts);
          adifTab = fields[0].toUpper ();
          theData = fields[1];
          if (adifTab == "CALL")
        {
          MyCallkLineEdit->setText (theData.toUpper ());
        }
          else if (adifTab == "LOCATOR")
        {
          MyLocatorkLineEdit->setText (theData.toUpper ());
        }
          else if (adifTab == "AWARD")
        {
          QFile file (theData);
          if (file.open (QIODevice::ReadOnly))
            {
              award.clearAward ();
              award.readAwardHeaders(theData);
              strings = award.getEntityPrefix ();	// Strings here includes the list of prefixes for the award that is being read.
              for (QStringList::Iterator it =
               prefixesOfAwards.begin ();
               it != prefixesOfAwards.end (); ++it)
            {
              for (QStringList::Iterator ite = strings.begin (); ite != strings.end (); ++ite)
                {
                  if ((*it) == (*ite))
                {
                  QMessageBox::about (this,
                              i18n("KLog Warning!"),
                              i18n("Prefix from - %1 - the award: - %2 - is repeated!\nKLog can only manage one award per prefix.\nThis award will not be included\nCheck your klogrc file and/or the award file and fix the problem.",(*it), award.getAwardName()));
                  return;
                }
                }
            }
              QTreeWidgetItem *item = new QTreeWidgetItem (awardTreeWidget, 0);
              awardN++;
              item->setText (0, QString::number (awardN));
              item->setText (1, award.getAwardName ());
              item->setText (2, (strings).join (", "));
              item->setText (3, theData);
              prefixesOfAwards = prefixesOfAwards + strings;
            }
          else
            {
              QMessageBox::about (this, i18n ("KLog Warning!"),
                      i18n
                      ("KLog can not open the file: %1 !\n"
                       "That award will not be read.\nCheck your klogrc file and fix the problem.").
                      arg (theData));
            }
        }
          else if (adifTab == "DXCLUSTERSERVER")
        {
          DXClusterServer = theData.toLower ();
          if (DXclusterServerPort >= 1)
            {
              aux2 =
            (DXClusterServer + ":" +
             QString::number (DXclusterServerPort)).toLower ();
              if (checkIfValidDXCluster (aux2))
            {
              if (checkIfNewDXCluster (aux2))
                {
                  dxClustercomboBox->insertItem (0, aux2);
                }
            }
            }
        }
          else if (adifTab == "DXCLUSTERPORT")
        {
          DXclusterServerPort = theData.toInt ();
          if ((DXclusterServerPort >= 1)
              || (DXClusterServer != "NOSERVER"))
            {
              aux2 =
            (DXClusterServer + ":" +
             QString::number (DXclusterServerPort)).toLower ();;
              if (checkIfValidDXCluster (aux2))
            {
              if (checkIfNewDXCluster (aux2))
                {
                  dxClustercomboBox->insertItem (0, aux2);
                }
            }
            }
        }
          else if (adifTab == "DXCLUSTERSERVERPORT")
        {
          if ((theData.contains (":")) == 0)
            {
              theData = theData + ":41112";
            }
          if (checkIfValidDXCluster (theData.toLower ()))
            {
              if (checkIfNewDXCluster (theData))
            {
              dxClustercomboBox->insertItem (0,
                             theData.toLower ());
            }
            }
        }
          else if (adifTab == "DXCLUSTERSERVERTOUSE")
        {
          if ((theData.contains (":")) == 0)
            {
              theData = theData + ":41112";
            }
          if (checkIfValidDXCluster (theData.toLower ()))
            {
              if (checkIfNewDXCluster (theData))
            {
              dxClustercomboBox->insertItem (0,
                             theData.toLower ());
              DXClusterServerToUse = theData.toLower ();
            }
            }

        }
          else if (adifTab == "COMPLETEWITHPREVIOUS")
        {
          if (theData.toUpper () == "TRUE")
            {
              completeWithPreviouscheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              completeWithPreviouscheckBox->setChecked (false);
            }
        }
          else if (adifTab == "REQUIREMANDATORY")
        {
          if (theData.toUpper () == "TRUE")
            {
              mandatoryDatacheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              mandatoryDatacheckBox->setChecked (false);
            }
        }
          else if (adifTab == "TIMEUTC")
        {
          if (theData.toUpper () == "TRUE")
            {
              UTCcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              UTCcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "OPENLASTFILE")
        {
          if (theData.toUpper () == "TRUE")
            {
              lastFilecheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              lastFilecheckBox->setChecked (false);
            }
        }
          else if (adifTab == "DXCLUSTERHFSPOTS")
        {
          if (theData.toUpper () == "TRUE")
            {
              HFcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              HFcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "DXCLUSTERVHFSPOTS")
        {
          if (theData.toUpper () == "TRUE")
            {
              VHFcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              VHFcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "DXCLUSTERWARCSPOTS")
        {
          if (theData.toUpper () == "TRUE")
            {
              WARCcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              WARCcheckBox->setChecked (false);
            }

        }
          else if (adifTab == "DXCLUSTERCWSPOTS")
        {
          if (theData.toUpper () == "TRUE")
            {
              CWcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              CWcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "DXCLUSTERSSBSPOTS")
        {
          if (theData.toUpper () == "TRUE")
            {
              SSBcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              SSBcheckBox->setChecked (false);
            }


        }
          else if (adifTab == "DXCLUSTERANNOUNCES")
        {
          if (theData.toUpper () == "TRUE")
            {
              ANNcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              ANNcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "DXCLUSTERWXANNOUNCES")
        {
          if (theData.toUpper () == "TRUE")
            {
              WXcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              WXcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "DXCLUSTERCONFIRMEDANNOUNCES")
        {
          if (theData.toUpper () == "TRUE")
            {
              confirmedcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              confirmedcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "DXCLUSTERWCYANNOUNCES")
        {
          if (theData.toUpper () == "TRUE")
            {
              WCYcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              WCYcheckBox->setChecked (false);
            }
        }
          else if (adifTab == "REALTIMELOG")
        {
          if (theData.toUpper () == "TRUE")
            {
              realTimeLogCheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              realTimeLogCheckBox->setChecked (false);
            }
        }
          else if (adifTab == "CONFIRMEDCOLOR")
        {
          color.setNamedColor (theData.toUpper ());
          //confirmedkColorCombo->setColor(color);
          confirmedPushButton->setPalette (color);
        }
          else if (adifTab == "WORKEDCOLOR")
        {
          color.setNamedColor (theData.toUpper ());
//          workedkColorCombo->setColor(color);
          workedPushButton->setPalette (color);
        }
          else if (adifTab == "NEEDEDCOLOR")
        {
          color.setNamedColor (theData.toUpper ());
//          neededkColorCombo->setColor(color);
          neededPushButton->setPalette (color);
        }
          else if (adifTab == "NEWONECOLOR")
        {
          color.setNamedColor (theData.toUpper ());
//          neededkColorCombo->setColor(color);
          newOnePushButton->setPalette (color);
        }
          else if (adifTab == "DEFAULTCOLOR")
        {
          color.setNamedColor (theData.toUpper ());
//          defaultkColorCombo->setColor(color);
          defaultPushButton->setPalette (color);
        }
          else if (adifTab == "POWER")
        {
          powerkIntNumInput->setValue (theData.toInt ());
        }
          else if (adifTab == "DEFAULTBAND")
        {
          bandkComboBox->setCurrentIndex (theData.toInt ());
        }
          else if (adifTab == "DEFAULTMODE")
        {
          modekComboBox->setCurrentIndex (theData.toInt ());
        }
          else if (adifTab == "SHOWPROGRESSDIALOG")
        {
          if (theData.toUpper () == "TRUE")
            checkBoxProgressDialog->setChecked (true);
        }
          else if (adifTab == "LOGFILENAME")
        {
          filenamekLineEdit->setText(theData);
        }
          else if (adifTab == "HAMLIB")
        {
          if (theData.toUpper () == "TRUE")
            {
              hamlibcheckBox->setChecked (true);
            }
          else if ((theData.toUpper () == "FALSE"))
            {
              hamlibcheckBox->setChecked (false);
            }

          ///////////////////////////////////////////////////////////////////////////////
          // Modifications by kike
          // 25-03-2004
          //////////////////////////////////////////////////////////////////////////////
        }
          else if (adifTab == "RIGNAME")
        {
          comboBoxTrvList->setItemText (comboBoxTrvList->
                        currentIndex (),
                        rigid2rigname[theData.
                                  toInt ()]);
        }
          else if (adifTab == "PORT")
        {
//       comboBoxTrvPortList->setCurrentText(theData);
          devicelineEdit->setText (theData);
        }
          else if (adifTab == "BAUDS")
        {
          //comboBoxTrvSpeedList->setCurrentText(theData);
          //speedlineEdit->setText(theData);
//              qDebug()  << "Klog::readConf: " << QString::number(theData.toInt()) << endl;

          comboBoxTrvSpeedList->setCurrentIndex (bauds2Num (theData));
        }
          //////////////////////////////////////////////////////////////////////////////
          // End of modifications
          //////////////////////////////////////////////////////////////////////////////
          else if (adifTab == "HAMLIBINTERVAL")
        {
          hamlibIntervalspinBox->setValue (theData.toInt ());

        }
          else
        {
        }
        }
    }
      if (DXClusterServerToUse != "NOSERVER")
    {
      dxclusterServerListItems = dxClustercomboBox->count ();
      if (dxclusterServerListItems >= 1)
        {
          for (i = 0; i <= dxclusterServerListItems - 1; i++)
        {
          dxClustercomboBox->setCurrentIndex (i);
          if (DXClusterServerToUse ==
              dxClustercomboBox->currentText ())
            {
              j = i;
            }
        }
          dxClustercomboBox->setCurrentIndex (j);

        }


    }
      file.close ();
    }
  else
    {
      // the file klogrc with preferences does not exist so we have to create it
      writeConf();
    }
}

void
Setup::writeConf ()
{
  QFile file ("klogrc");
  QString tmp;
  tmp = "true";
  if (file.open (QIODevice::WriteOnly))
    {
      QTextStream stream (&file);
      stream << "call=" << (MyCallkLineEdit->text ()).toUpper () << endl;
      stream << "locator=" << (MyLocatorkLineEdit->text ()).
    toUpper () << endl;

// WE HAVE TO PRINT ALL THE DXCLUSTER SERVERS IN THE NEW (0.3.4) FORMAT
// SERVER:PORT
      //stream << "DXClusterServerPort=" << endl;

      dxclusterServerListItems = dxClustercomboBox->count ();
      if (dxclusterServerListItems >= 1)
    {
      stream << "DXClusterServerToUse=" << dxClustercomboBox->
        currentText () << endl;
      for (i = 0; i <= dxclusterServerListItems - 1; i++)
        {
          dxClustercomboBox->setCurrentIndex (i);
          stream << "DXClusterServerPort=" << dxClustercomboBox->
        currentText () << endl;
        }

    }
//              stream << "dxclusterserver=" << (DXClusterServerkLineEdit->text()).toUpper() << endl;
//              stream << "dxclusterport=" << QString::number(DXClusterPortkIntNumInput->value()) << endl;
      stream << "defaultBand=" << QString::number (bandkComboBox->
                           currentIndex ()) << endl;
      stream << "defaultMode=" << QString::number (modekComboBox->
                           currentIndex ()) << endl;
      if (UTCcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "timeutc=" + tmp << endl;
      if (HFcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclusterhfspots=" + tmp << endl;
      if (CWcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclustercwspots=" + tmp << endl;
      if (SSBcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclusterssbspots=" + tmp << endl;


      if (completeWithPreviouscheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "completewithprevious=" + tmp << endl;
      if (mandatoryDatacheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "requiremandatory=" + tmp << endl;
      if (VHFcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclustervhfspots=" + tmp << endl;
      if (WARCcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclusterwarcspots=" + tmp << endl;
      if (ANNcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclusterannounces=" + tmp << endl;
      if (WXcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclusterwxannounces=" + tmp << endl;

      if (WCYcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclusterwcyannounces=" + tmp << endl;
      if (confirmedcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "dxclusterconfirmedannounces=" + tmp << endl;
      if (realTimeLogCheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "realTimeLog=" + tmp << endl;
      // Color scheme...

//              stream << "ConfirmedColor=" + (confirmedkColorCombo->color()).name() << endl;
      palette = confirmedPushButton->palette ();
      stream << "ConfirmedColor=" +
    (confirmedPushButton->palette ().color (QPalette::Button)).
    name () << endl;
      stream << "WorkedColor=" +
    (workedPushButton->palette ().color (QPalette::Button)).
    name () << endl;
// (workedkColorCombo->color()).name() << endl;
      stream << "NeededColor=" +
    (neededPushButton->palette ().color (QPalette::Button)).
    name () << endl;
// (neededkColorCombo->color()).name() << endl;
      stream << "NewOneColor=" +
    (newOnePushButton->palette ().color (QPalette::Button)).
    name () << endl;
// (neededkColorCombo->color()).name() << endl;
      stream << "DefaultColor=" +
    (defaultPushButton->palette ().color (QPalette::Button)).
    name () << endl;
// (defaultkColorCombo->color()).name() << endl;
      stream << "Power=" +
    QString::number (powerkIntNumInput->value ()) << endl;
      if (lastFilecheckBox->isChecked () && (filenamekLineEdit->text () != ""))
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
    stream << "OpenLastFile=" + tmp << endl;
       if (filenamekLineEdit->text() != "")
          stream << "LogFileName=" + filenamekLineEdit->text () << endl;
       if (checkBoxProgressDialog->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }
      stream << "ShowProgressDialog=" + tmp << endl;
      if (hamlibcheckBox->isChecked ())
    {
      tmp = "true";
    }
      else
    {
      tmp = "false";
    }

      QTreeWidgetItemIterator itl (awardTreeWidget);
       while (*itl){
	stream << "Award=" + (*itl)->text (3) << endl;
        ++itl;
     }


//              for ( ; itl.current(); ++itl ){
//                       stream << "Award=" + (*itl)->text(3) << endl;
//               }

      stream << "HamLib=" + tmp << endl;
      stream << "RigName=" << rigname2rigid[comboBoxTrvList->
                        currentText ()] << endl;
      stream << "Port=" << devicelineEdit->text () << endl;
      stream << "Bauds=" << comboBoxTrvSpeedList->currentText () << endl;
      stream << "HamlibInterval=" << QString::number (hamlibIntervalspinBox->
                              value ()) << endl;

      file.close ();
    }

}

void
Setup::slotFileNameClicked (){
//    qDebug() << "Setup::slotFileNameClicked: ";
  QString fileName = QFileDialog::getOpenFileName (this,
                           i18n ("Select a file"),
                           klogDir,
                           i18n("ADIF files (*.adi)"));

  if (!fileName.isEmpty ())
    {
      filenamekLineEdit->setText (fileName);
    }
}


void
Setup::slotOpenFileButtonClicked ()
{
  if (lastFilecheckBox->isChecked ())
    {
      filenamekLineEdit->setEnabled (true);
      searchFilekPushButton->setEnabled (true);
    }
  else
    {
      filenamekLineEdit->setEnabled (false);
      searchFilekPushButton->setEnabled (false);
    }
}

void
Setup::slotSearchAwardFilekPushButtonClicked ()
{
//qDebug()  << "Setup::slotSearchAwardFilekPushButton" << endl;

  QString awardFileName = QFileDialog::getOpenFileName (this,
                           i18n ("Open a file"),
                           klogDir,
                           i18n("Award files (*.awa)"));

  if (!awardFileName.isEmpty ())
    {
      awardFilenamekLineEdit->setText (awardFileName);
    }
}


void Setup::slotAddButtonClicked (){
//qDebug() << "Setup::slotAddButtonClicked" << endl;
  awardFileName = awardFilenamekLineEdit->text ();
QString auxString;
  if (awardFileName.isEmpty ()){
      slotSearchAwardFilekPushButtonClicked ();
  }else{
    QFile file (awardFileName);
    if (file.open (QIODevice::ReadOnly)){
      award.clearAward ();
      award.readAwardHeaders (awardFileName);
      strings = award.getEntityPrefix ();	// Strings here includes the list of prefixes for the award that is being read.
//Check if the prefixes of the award to be checked are already added!
// KLog can manage only ONE award per Entity
    
      for (QStringList::Iterator it = prefixesOfAwards.begin (); it != prefixesOfAwards.end (); ++it){
	for (QStringList::Iterator ite = strings.begin (); ite != strings.end (); ++ite){
          if ((*it) == (*ite)){
	    auxString = i18n("Prefix from - %1 - the award: - %2 - repeated!\nKLog can only manage one award per prefix.\nThis award will not be included\nCheck your klogrc file and/or the award file and fix the problem.", (*it), award.getAwardName() );
	    QMessageBox::about (this, i18n ("KLog Warning!"), auxString);
	    return;
	  }
	}
      }
      QTreeWidgetItem *item = new QTreeWidgetItem (awardTreeWidget, 0);
      i = getLastAwardNumber();
      i++;
      item->setText (0, QString::number (i));
      item->setText (1, award.getAwardName ());
      item->setText (2, (strings).join (", "));
      item->setText (3,
             awardFileName.right (awardFileName.length () -
                          klogDir.length () - 1));

// I have ALL the prefixes passed for previous awards
      prefixesOfAwards = prefixesOfAwards + strings;
    }else{
      
      QMessageBox::about (this, i18n ("KLog Warning!"),
                  i18n ("KLog can not open the file:-%1!\nThat award will not be read.\nCheck your klogrc file and fix the problem.", awardFileName));
    }

    }
}

void
Setup::slotAwardClearButtonClicked (){
//      qDebug()  << "Setup::slotAwardClearButtonClicked" << endl;

  awardFilenamekLineEdit->clear ();


}

void
Setup::slotAwardRemoveButtonClicked (){
//qDebug()  << "Setup::slotAwardRemoveButtonClicked" << endl;

  QMessageBox msgBox;
  msgBox.setText(i18n("KLog message:"));
  QString str = i18n("This function has still not been implemented.\nRemove the appropriate \"Award=file.awa\" line from the ~/.klog/klogrc file.\n\n");
  msgBox.setInformativeText(str);
  msgBox.setStandardButtons(QMessageBox::Ok);
  msgBox.setDefaultButton(QMessageBox::Ok);
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.exec();



//   awardFileName = awardFilenamekLineEdit->text ();
//   if (awardFileName.isEmpty ()){
//   }else{
//     if (awardToDelete != -1){
//       // Before calling to delete we have to prepare it all
//       slotAwardDelete ();
//     }
//   }
}

void
Setup::slotAddDxClusterPushButtonClicked ()
{
  //qDebug()  << "Setup::slotAddDxClusterPushButtonClicked()" << endl;
  bool ok;
  ok = false;
  while (!ok)
    {
      QString text = QInputDialog::getText (this,
                        i18n("KLog: Add a DXCluster server"),
                        i18n("Add the address followed by the :port\nExample: eadx.net:23\nIf no port is specified, 41112 will be used by default.:"),
                        QLineEdit::Normal, QString::null,
                        &ok);
      if (ok && !text.isEmpty ())
    {
      if (checkIfValidDXCluster (text))
        {
          if (checkIfNewDXCluster (text))
        {
          ok = true;
          if ((text.contains (":")) == 0)
            {
              text = text + ":41112";
            }
          dxClustercomboBox->insertItem (0, text);
        }
          else
        {
          ok = false;
        }
        }
      else
        {
          ok = false;
        }
    }
      else
    {
      // user entered nothing or pressed Cancel
      ok = true;
    }
    }				//Adds a dxcluster server
}

void
Setup::slotClearDxClusterPushButtonClicked (){
  //qDebug()  << "Setup::slotClearDxClusterPushButtonClicked()" << endl;
/*

        dxclusterServerListItems = dxClustercomboBox->count();
    for (i = 0; i<=dxclusterServerListItems; i++){
                dxClustercomboBox->setCurrentIndex(i);
                stream << "DXClusterServerPort=" << dxClustercomboBox->currentText() << endl;
    }
*/
  dxClustercomboBox->removeItem (dxClustercomboBox->currentIndex ());

}

//TODO: DELETED FOR QT4 MIGRATION: Add the rightbutton option to edit/remove            
void Setup::slotAwardRightButtonFromList(QTreeWidgetItem * item){
//qDebug()  << "Setup::slotAwardRightButtonFromLog" << endl;
   if (item){
     Setup::i = (item->text(0)).toInt(); // i is the QSO number
     slotAwardSelected(item);
     awardToDelete = ((item)->text(0)).toInt();
//     showMenuRightButton(item, p);
   }
}

void Setup::showMenuRightButton(const QPoint &p){
//qDebug()  << "Setup::showMenuRightButton" << endl;
/*  Setup::editdeletePixMap = new QPixmap("editdelete.png");
  awardToDelete = ((item)->text(0)).toInt();
  KMenu *awardMenu = new KMenu( this );
  awardMenu->insertItem( *editdeletePixMap, i18n("Delete"), this, SLOT( slotAwardDelete() ),  QKeySequence(Qt::CTRL + Qt::Key_D));
  awardMenu->insertSeparator();
  awardMenu->exec(p);
  */
  QTreeWidgetItem *item = awardTreeWidget->itemAt(p);
  QMenu menu(this);
  delAWAAct = new QAction(i18n("&Delete"), this);
  
  if (item){
  //  qDebug()  << "Setup::showMenuRightButton = " << item->text(1) << endl;
    awardToDelete = ((item)->text(0)).toInt();
    //showMenuRightButton(awardToDelete);
    if (awardToDelete >0){
      
     //newAct->setShortcuts(QKeySequence::New);
    //  delAWAAct->setStatusTip(i18n("Delete a QSO"));
      connect(delAWAAct, SIGNAL(triggered()), this, SLOT(slotAwardDelete()));

      
      menu.addAction(delAWAAct);
      //menu.addSeparator();
      menu.exec(QCursor::pos());
      
    }else{
      return;
    }
  }else{
    //qDebug()  << "Setup::showMenuRightButton NO Item selected" << endl;
    return;
  }
  
  
}

void Setup::slotAwardSelected (QTreeWidgetItem * item) {
//void Setup::slotAwardSelected(QTreeWidgetItem* item){
//qDebug()  << "Setup::slotAwardSelected: " << klogDir << "/" << (item->text (3)) << endl;
  awardFilenamekLineEdit->setText (klogDir + "/" + (item->text (3)));
  awardToDelete = ((item)->text (0)).toInt();
}



void Setup::slotAwardDelete() {
  //qDebug()  << "Setup::slotAwardDelete: " << QString::number(awardToDelete) << endl;
  QMessageBox::about (this, i18n ("KLog Warning!"),
              i18n ("This function is still not implemented.\nTo erase an award, delete the appropriate line of your klogrc file."));

/*

  QTreeWidgetItemIterator itl( awardTreeWidget );
  QStringList::iterator ita;
  QStringList::iterator itb;
    //QStringList::iterator itc;

  QString auxString;
  QString firstPrefix;
  int numberOfPrefixes;
  QStringList auxPrefixesOfAwards;
  auxPrefixesOfAwards.clear();
  int _ii=1;

  
  while (*itl){
 // for ( ; itl.current(); ++itl ){
    if ((((*itl)->text(0)).toInt()) == awardToDelete){
      auxPrefixesOfAwards = ((*itl)->text(2)).split(", ");
      //auxPrefixesOfAwards = QStringList::split( ", ",  ((*itl)->text(2)));
      awardTreeWidget->takeTopLevelItem(awardToDelete);
      //awardTreeWidget->takeItem(*itl);
    }
    for ( itb = auxPrefixesOfAwards.begin(); itb != auxPrefixesOfAwards.end(); ++itb  ){
      for ( ita = prefixesOfAwards.begin(); ita != prefixesOfAwards.end(); ++ita){
	if ((*ita)==(*itb)){	  
	  prefixesOfAwards.removeAt(_ii);
	  _ii++;
	}
      }
    }
    ++itl;
  }
  awardToDelete = -1;
  */
}

int Setup::getLastAwardNumber() {
//qDebug()  << "Setup::getLastAwardNumber"  << endl;
  awardN = 0;
  
  QTreeWidgetItemIterator itl (awardTreeWidget);
   
  while (*itl){
    awardN = ((*itl)->text (0)).toInt ();
    ++itl;
  }
/*     for ( ; itl.current(); ++itl ){
           
              awardN = ((*itl)->text(0)).toInt();
	
    }*/
  return awardN+1;
}

bool Setup::checkIfValidDXCluster (const QString & tdxcluster) {
  QUrl url ("http://" + tdxcluster);
  if ((!url.host ().isEmpty ()) || (url.port () != -1))
    return true;
  else
    return false;
}

bool Setup::checkIfNewDXCluster (const QString & tdxcluster) {
  //qDebug()  << "checkIfNewDXCluster: -" << tdxcluster << "-"<< endl;
  dxclusterServerListItems = dxClustercomboBox->count ();
  for (i = 0; i <= dxclusterServerListItems - 1; i++) {
      if ((dxClustercomboBox->currentText ()) == (tdxcluster)) {
      return false;
    }
      else
    {
      return true;
    }
    }
  return true;
}

/*
void Setup::showMenuRightButton(int taward, const QPoint &p){
    ////qDebug()  << "KLog::showMenuRightButton" << endl;
    if (taward >= 0){
        qso = getByNumber(qqso);
        KMenu *awardMenu = new KMenu( this );
        awardMenu->insertItem( *editdeletePixMap, i18n("Delete"), this, SLOT( slotQsoDelete() ), CTRL + Key_D );
        awardMenu->insertSeparator();

        awardMenu->exec(p);
    }
}

*/
///////////////////////////////////////////////////////////////////////////////
// Modifications by kike
// 25-03-2004
//////////////////////////////////////////////////////////////////////////////
int
Setup::addRigToList (const struct rig_caps *caps, void *data)
{
  QString name;
  Setup *r = (Setup *) data;
  name = caps->model_name;
  if (name == "Dummy")
    name = "None";
  r->rigname2rigid[name] = caps->rig_model;
  r->rigid2rigname[caps->rig_model] = name;
  r->strings << name;
  return -1;			// not 0 --> we want all rigs
}

void
Setup::setRig ()
{
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's
  rig_set_debug (RIG_DEBUG_NONE);

  // and continue...
  rig_load_all_backends ();
  rig_list_foreach (addRigToList, this);
  strings.sort ();
  comboBoxTrvList->insertItems (0, strings);
  strings.clear ();
}

//////////////////////////////////////////////////////////////////////////////
// End of modifications
//////////////////////////////////////////////////////////////////////////////

int
Setup::bauds2Num (const QString & tbauds)
{
  switch (tbauds.toInt ())
    {

    case 1200:
      return 0;
      break;

    case 2400:
      return 1;
      break;

    case 4800:
      return 2;
      break;

    case 9600:
      return 3;
      break;

    case 19200:
      return 4;
      break;

    case 38400:
      return 5;
      break;

    case 57600:
      return 6;
      break;

    case 115200:
      return 7;
      break;

    default:			//
      return 2;
      break;
    }
}
