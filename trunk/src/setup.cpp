/***************************************************************************
                          setup.cpp  -  description
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

#include <klocale.h>
//#include <kdebug.h>
//#include <ostream.h>
#include "setup.h"



Setup::Setup(QWidget *parent, const char *name) :  setupui(parent, name){
	klogDir = QDir::homeDirPath()+"/.klog";  // We create the ~/.klog for the logs
	if (!QDir::setCurrent ( klogDir )){
		QDir d1(klogDir);
		dirExist = d1.mkdir(klogDir, true);
	}
	dirExist = QDir::setCurrent ( klogDir ) ;
	fileName = "";
  ///////////////////////////////////////////////////////////////////////////////
  // Modifications by kike
  // 25-03-2004
  //////////////////////////////////////////////////////////////////////////////
  // Fill the rig combo
	setRig();
  //////////////////////////////////////////////////////////////////////////////
  // End of modifications
  /////////////////////////////////////////////////////////////////////////////

	readConf();

	slotOpenFileButtonClicked();
	awardToDelete = -1;

}


Setup::~Setup()
{
}

void Setup::slotOkBtnClicked(){
  writeConf();
  accept();
}


void Setup::slotConfirmedColorPushButton(){
//cout << "Setup::slotConfirmedColorPushButton " << endl;
	colorGroup = setupui::confirmedPushButton->colorGroup();
	setupui::confirmedPushButton->setPaletteBackgroundColor(giveColor(setupui::confirmedPushButton->paletteBackgroundColor()));
}


void Setup::slotWorkedColorPushButton(){
//cout << "Setup::slotWorkedColorPushButton " << endl;
	colorGroup = setupui::workedPushButton->colorGroup();
	setupui::workedPushButton->setPaletteBackgroundColor(giveColor(setupui::workedPushButton->paletteBackgroundColor()));
}

void Setup::slotNeededColorPushButton(){
//cout << "Setup::slotNeededColorPushButton " << endl;
	colorGroup = setupui::neededPushButton->colorGroup();
	setupui::neededPushButton->setPaletteBackgroundColor(giveColor(setupui::neededPushButton->paletteBackgroundColor()));
}

void Setup::slotNewOneColorPushButton(){
//cout << "Setup::slotNeededColorPushButton " << endl;
	colorGroup = setupui::newOnePushButton->colorGroup();
	setupui::newOnePushButton->setPaletteBackgroundColor(giveColor(setupui::newOnePushButton->paletteBackgroundColor()));
}


void Setup::slotDefaultColorPushButton(){
//cout << "Setup::slotDefaultColorPushButton " << endl;
	colorGroup = setupui::defaultPushButton->colorGroup();
	setupui::defaultPushButton->setPaletteBackgroundColor(giveColor(setupui::defaultPushButton->paletteBackgroundColor()));
}
QColor Setup::giveColor(QColor c){
/* Receives the actual color, shows the user a color picker and returns the color
that the user selects.
*/
//cout << "Setup::giveColor " << endl;
	QColor colorb;
	color = c;
	colorb = color;
	color = QColorDialog::getColor(color,this, "color dialog" );
	if (color.isValid()){
		return color;
	}else{
		return colorb;
	}

}

void Setup::readConf(){
	QFile file( "klogrc" );
	if ( file.open( IO_ReadOnly ) ) {
	
	QTextStream stream( &file );
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
	prefixesOfAwards.clear();
	setupui::awardListView->clear();

//	cout << "Setup::readConf() - 1 " << endl;
    while (!stream.eof()){
      data = stream.readLine();
      if(!data.isEmpty()){ // If there is another line i read it
        data = data.simplifyWhiteSpace();
        QStringList fields = QStringList::split('=', data );

        adifTab = fields[0].upper();
        theData = fields[1];
        if (adifTab == "CALL"){
          setupui::MyCallkLineEdit->setText(theData.upper());

        }else if (adifTab == "LOCATOR"){
          setupui::MyLocatorkLineEdit->setText(theData.upper());
	}else if(adifTab == "AWARD"){
		//cout << "Setup:readConf-Award: "<< theData << endl;
  		QFile file( theData );
  		if ( file.open( IO_ReadOnly ) ) {
			award.clearAward();
			award.readAwardHeaders(theData);
		
			strings = award.getEntityPrefix();	// Strings here includes the list of prefixes for the award that is being read.
	for ( QStringList::Iterator it = prefixesOfAwards.begin(); it != prefixesOfAwards.end(); ++it ) {
		for ( QStringList::Iterator ite = strings.begin(); ite != strings.end(); ++ite ) {
			if ((*it)==(*ite)){ 
				QMessageBox::about( this, i18n("KLog Warning!"),
                            i18n("Prefix from - " +(*it)+ " - the award: - "+award.getAwardName()+ " - is repeated!\n"
                            "KLog can only manage one award per prefix.\nThis award will not be included\nCheck your klogrc file and/or the award file and fix the problem."));
				return;
			}
		}
	}
			QListViewItem * item = new QListViewItem( setupui::awardListView, 0 );
			awardN++;
			item->setText( 0, QString::number(awardN) );
			item->setText( 1, award.getAwardName() );
			item->setText( 2, (strings).join(", ") );
			item->setText( 3, theData );
			prefixesOfAwards = prefixesOfAwards + strings;
//			prefixesOfAwards.append(award.getEntityPrefix());
		}else{
			QMessageBox::about( this, i18n("KLog Warning!"),
                            i18n("KLog can not open the file: "+theData + "!\n"
                            "That award will not be read.\nCheck your klogrc file and fix the problem."));
		}
        }else if (adifTab == "DXCLUSTERSERVER"){
		DXClusterServer = theData.lower();
		if (DXclusterServerPort>=1){
		aux2 = (DXClusterServer+":"+QString::number(DXclusterServerPort)).lower();
		if (checkIfValidDXCluster(aux2)){
			if(checkIfNewDXCluster(aux2)){
				setupui::dxClustercomboBox->insertItem(aux2);	
			}

		}
		}

		//	setupui::DXClusterServerkLineEdit->setText(theData);

        }else if (adifTab == "DXCLUSTERPORT"){
			
		DXclusterServerPort = theData.toInt();
//          setupui::DXClusterPortkIntNumInput->setValue(theData.toInt());
		if ((DXclusterServerPort>=1)||(DXClusterServer!="NOSERVER")){
			aux2 = (DXClusterServer+":"+QString::number(DXclusterServerPort)).lower();;
			if (checkIfValidDXCluster(aux2)){
				if(checkIfNewDXCluster(aux2)){
					setupui::dxClustercomboBox->insertItem(aux2);
				}
			}
		}


        }else if (adifTab == "DXCLUSTERSERVERPORT"){
		
		if ((theData.contains(":"))==0){
			theData = theData+":41112";		
		}
		if (checkIfValidDXCluster(theData.lower())) {
			if(checkIfNewDXCluster(theData)){
				setupui::dxClustercomboBox->insertItem(theData.lower());
			}
		}


	}else if (adifTab == "DXCLUSTERSERVERTOUSE"){
		if ((theData.contains(":"))==0){
			theData = theData+":41112";
		}
		if (checkIfValidDXCluster(theData.lower())){
		
			if(checkIfNewDXCluster(theData)){
				setupui::dxClustercomboBox->insertItem(theData.lower());
				DXClusterServerToUse = theData.lower();
			}
		}


        }else if (adifTab == "COMPLETEWITHPREVIOUS"){
          if (theData.upper() == "TRUE"){
            setupui::completeWithPreviouscheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::completeWithPreviouscheckBox->setChecked(false);
          }
        }else if (adifTab == "REQUIREMANDATORY"){
          if (theData.upper() == "TRUE"){
            setupui::mandatoryDatacheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::mandatoryDatacheckBox->setChecked(false);
          }

        }else if (adifTab == "TIMEUTC"){
          if (theData.upper() == "TRUE"){
            setupui::UTCcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::UTCcheckBox->setChecked(false);
          }

        }else if (adifTab == "OPENLASTFILE"){
          if (theData.upper() == "TRUE"){
            setupui::lastFilecheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::lastFilecheckBox->setChecked(false);
          }
        }else if (adifTab == "DXCLUSTERHFSPOTS"){
          if (theData.upper() == "TRUE"){
            setupui::HFcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::HFcheckBox->setChecked(false);
          }
        }else if (adifTab == "DXCLUSTERVHFSPOTS"){
          if (theData.upper() == "TRUE"){
            setupui::VHFcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::VHFcheckBox->setChecked(false);
          }
        }else if (adifTab == "DXCLUSTERWARCSPOTS"){
          if (theData.upper() == "TRUE"){
            setupui::WARCcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::WARCcheckBox->setChecked(false);
          }

       }else if (adifTab == "DXCLUSTERCWSPOTS"){
          if (theData.upper() == "TRUE"){
            setupui::CWcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::CWcheckBox->setChecked(false);
          }
       }else if (adifTab == "DXCLUSTERSSBSPOTS"){
          if (theData.upper() == "TRUE"){
            setupui::SSBcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::SSBcheckBox->setChecked(false);
          }


      }else if (adifTab == "DXCLUSTERANNOUNCES"){
          if (theData.upper() == "TRUE"){
            setupui::ANNcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::ANNcheckBox->setChecked(false);
          }
        }else if (adifTab == "DXCLUSTERWXANNOUNCES"){
          if (theData.upper() == "TRUE"){
            setupui::WXcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::WXcheckBox->setChecked(false);
          }
        }else if (adifTab == "DXCLUSTERCONFIRMEDANNOUNCES"){
          if (theData.upper() == "TRUE"){
            setupui::confirmedcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::confirmedcheckBox->setChecked(false);
          }
        }else if (adifTab == "DXCLUSTERWCYANNOUNCES"){
          if (theData.upper() == "TRUE"){
            setupui::WCYcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::WCYcheckBox->setChecked(false);
          }   
        }else if (adifTab == "REALTIMELOG"){
          if (theData.upper() == "TRUE"){
            setupui::realTimeLogCheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::realTimeLogCheckBox->setChecked(false);
          }
        }else if (adifTab == "CONFIRMEDCOLOR"){
          color.setNamedColor(theData.upper());
          //setupui::confirmedkColorCombo->setColor(color);
	setupui::confirmedPushButton->setPaletteBackgroundColor(color);
	}else if (adifTab == "WORKEDCOLOR"){
          color.setNamedColor(theData.upper());
//          setupui::workedkColorCombo->setColor(color);
setupui::workedPushButton->setPaletteBackgroundColor(color);
        }else if (adifTab == "NEEDEDCOLOR"){
          color.setNamedColor(theData.upper());
//          setupui::neededkColorCombo->setColor(color);
setupui::neededPushButton->setPaletteBackgroundColor(color);
        }else if (adifTab == "NEWONECOLOR"){
          color.setNamedColor(theData.upper());
//          setupui::neededkColorCombo->setColor(color);
setupui::newOnePushButton->setPaletteBackgroundColor(color);
        }else if (adifTab == "DEFAULTCOLOR"){
          color.setNamedColor(theData.upper());
//          setupui::defaultkColorCombo->setColor(color);
setupui::defaultPushButton->setPaletteBackgroundColor(color);
        }else if (adifTab == "POWER"){
          setupui::powerkIntNumInput->setValue(theData.toInt());
        }else if (adifTab == "DEFAULTBAND"){
          setupui::bandkComboBox->setCurrentItem(theData.toInt());
        }else if (adifTab == "DEFAULTMODE"){
          setupui::modekComboBox->setCurrentItem(theData.toInt());
        }else if (adifTab == "SHOWPROGRESSDIALOG"){
          if (theData.upper() == "TRUE")
            setupui::checkBoxProgressDialog->setChecked(true);
        }else if (adifTab == "LOGFILENAME"){
          fileName = theData;
          setupui::filenamekLineEdit->setText(fileName);
       }else if (adifTab == "HAMLIB"){
          if (theData.upper() == "TRUE"){
            setupui::hamlibcheckBox->setChecked(true);
          }else if ((theData.upper() == "FALSE")){
            setupui::hamlibcheckBox->setChecked(false);
          }

          ///////////////////////////////////////////////////////////////////////////////
          // Modifications by kike
          // 25-03-2004
          //////////////////////////////////////////////////////////////////////////////
          }else if (adifTab == "RIGNAME"){
		comboBoxTrvList->setCurrentText(rigid2rigname[theData.toInt()]);
          }else if (adifTab == "PORT"){
//       comboBoxTrvPortList->setCurrentText(theData);
		devicelineEdit->setText(theData);
          }else if (adifTab == "BAUDS"){
       //comboBoxTrvSpeedList->setCurrentText(theData);
       		//speedlineEdit->setText(theData);
//		cout << "Klog::readConf: " << QString::number(theData.toInt()) << endl;

		setupui::comboBoxTrvSpeedList->setCurrentItem(bauds2Num(theData));
       }
          //////////////////////////////////////////////////////////////////////////////
          // End of modifications
          //////////////////////////////////////////////////////////////////////////////
		else if (adifTab == "HAMLIBINTERVAL"){
			setupui::hamlibIntervalspinBox->setValue(theData.toInt());
			
          }else{
        }
      }
    }
	if (DXClusterServerToUse!="NOSERVER"){
		dxclusterServerListItems = setupui::dxClustercomboBox->count();
		if  (dxclusterServerListItems>=1){
			for (i = 0; i<=dxclusterServerListItems-1; i++){
				setupui::dxClustercomboBox->setCurrentItem(i);
				if (DXClusterServerToUse==setupui::dxClustercomboBox->currentText()){
					j = i;
				}
			}
			setupui::dxClustercomboBox->setCurrentItem(j);
			
		}


	}
    file.close();
  }else{
  // the file klogrc with preferences does not exist so we have to create it
  }
}

void Setup::writeConf(){
	QFile file("klogrc");
	QString tmp;
	tmp = "true";
	if ( file.open( IO_WriteOnly ) ) {
		QTextStream stream( &file );
		stream << "call=" << (setupui::MyCallkLineEdit->text()).upper() << endl;
		stream << "locator=" << (setupui::MyLocatorkLineEdit->text()).upper()<< endl;

// WE HAVE TO PRINT ALL THE DXCLUSTER SERVERS IN THE NEW (0.3.4) FORMAT 
// SERVER:PORT
        	//stream << "DXClusterServerPort=" << endl;
		
		dxclusterServerListItems = setupui::dxClustercomboBox->count();
		if  (dxclusterServerListItems>=1){
			stream << "DXClusterServerToUse="<< setupui::dxClustercomboBox->currentText() << endl;
			for (i = 0; i<=dxclusterServerListItems-1; i++){
				setupui::dxClustercomboBox->setCurrentItem(i);
				stream << "DXClusterServerPort=" << setupui::dxClustercomboBox->currentText() << endl;
			}
			
		}
//		stream << "dxclusterserver=" << (setupui::DXClusterServerkLineEdit->text()).upper() << endl;
//		stream << "dxclusterport=" << QString::number(setupui::DXClusterPortkIntNumInput->value()) << endl;
		stream << "defaultBand=" << QString::number(setupui::bandkComboBox->currentItem()) << endl;
		stream << "defaultMode=" << QString::number(setupui::modekComboBox->currentItem()) << endl;
		if (setupui::UTCcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "timeutc=" + tmp << endl;
		if (setupui::HFcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclusterhfspots=" + tmp << endl;
		if (setupui::CWcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclustercwspots=" + tmp << endl;
		if (setupui::SSBcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclusterssbspots=" + tmp << endl;


		if (setupui::completeWithPreviouscheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "completewithprevious=" + tmp << endl;
		if (setupui::mandatoryDatacheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "requiremandatory=" + tmp << endl;
		if (setupui::VHFcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclustervhfspots=" + tmp << endl;    
		if (setupui::WARCcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclusterwarcspots=" + tmp << endl;
		if (setupui::ANNcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclusterannounces=" + tmp << endl;
		if (setupui::WXcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclusterwxannounces=" + tmp << endl;
    
		if (setupui::WCYcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclusterwcyannounces=" + tmp << endl;      
		if (setupui::confirmedcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "dxclusterconfirmedannounces=" + tmp << endl;          
		if (setupui::realTimeLogCheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "realTimeLog=" + tmp << endl;
		// Color scheme...

//		stream << "ConfirmedColor=" + (setupui::confirmedkColorCombo->color()).name() << endl;
		colorGroup = setupui::confirmedPushButton->colorGroup();
		stream << "ConfirmedColor=" + (setupui::confirmedPushButton->paletteBackgroundColor()).name() << endl;
		stream << "WorkedColor=" + (setupui::workedPushButton->paletteBackgroundColor()).name() << endl;
// (setupui::workedkColorCombo->color()).name() << endl;
		stream << "NeededColor=" + (setupui::neededPushButton->paletteBackgroundColor()).name() << endl;
// (setupui::neededkColorCombo->color()).name() << endl;
		stream << "NewOneColor=" + (setupui::newOnePushButton->paletteBackgroundColor()).name() << endl;
// (setupui::neededkColorCombo->color()).name() << endl;
		stream << "DefaultColor=" + (setupui::defaultPushButton->paletteBackgroundColor()).name() << endl;
// (setupui::defaultkColorCombo->color()).name() << endl;    
		stream << "Power=" + QString::number(setupui::powerkIntNumInput->value()) << endl;
		if (setupui::lastFilecheckBox->isChecked() && (setupui::filenamekLineEdit->text() != "")){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "OpenLastFile=" + tmp << endl;
		if (fileName != "")
			stream << "LogFileName=" + setupui::filenamekLineEdit->text() << endl;
		if (setupui::checkBoxProgressDialog->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		stream << "ShowProgressDialog=" + tmp << endl;    
		if (setupui::hamlibcheckBox->isChecked()){
			tmp="true";
		}else{
			tmp="false";
		}
		
		QListViewItemIterator itl( setupui::awardListView );
		for ( ; itl.current(); ++itl ){
			stream << "Award=" + (*itl)->text(3) << endl;
		}

		stream << "HamLib=" + tmp << endl;
		stream << "RigName=" << rigname2rigid[comboBoxTrvList->currentText()] << endl;
			stream << "Port=" << devicelineEdit->text() << endl;
			stream << "Bauds=" << setupui::comboBoxTrvSpeedList->currentText()<<endl;
			stream << "HamlibInterval=" << QString::number(setupui::hamlibIntervalspinBox->value()) << endl;
     ///////////////////////////////////////////////////////////////////////////////
    // Modifications by kike
    // 25-03-2004
    //////////////////////////////////////////////////////////////////////////////
		
    //////////////////////////////////////////////////////////////////////////////
    // End of modifications
    //////////////////////////////////////////////////////////////////////////////

		file.close();
	}
  
}

void Setup::slotFileNameClicked(){ 
  QFileDialog fileOpenDialog(this, i18n("Select the file"), TRUE);
  fileOpenDialog.setFilter(i18n("ADIF files (*.adi *.ADI)"));
  fileOpenDialog.setDir(klogDir);
  if (fileOpenDialog.exec() == QDialog::Accepted){
    fileName=fileOpenDialog.selectedFile();
    setupui::filenamekLineEdit->setText(fileName);

  }  
  else
    return;
}


void Setup::slotOpenFileButtonClicked(){
    if (setupui::lastFilecheckBox->isChecked()){
      setupui::filenamekLineEdit->setEnabled(true);
      setupui::searchFilekPushButton->setEnabled(true);
    }else{
      setupui::filenamekLineEdit->setEnabled(false);
      setupui::searchFilekPushButton->setEnabled(false);
    }
}

void Setup::slotSearchAwardFilekPushButtonClicked(){
//cout << "Setup::slotSearchAwardFilekPushButton" << endl;

	QFileDialog fileOpenDialog(this, i18n("Open a file"), TRUE);
	fileOpenDialog.setFilter(i18n("Award files (*.awa *.AWA)"));
	fileOpenDialog.setDir(klogDir);  
	if (fileOpenDialog.exec() == QDialog::Accepted)
		awardFileName=fileOpenDialog.selectedFile();
	else
		return;
	setupui::awardFilenamekLineEdit->setText(awardFileName);
}


void Setup::slotAddButtonClicked(){
//cout << "Setup::slotAddButtonClicked" << endl;
	awardFileName = setupui::awardFilenamekLineEdit->text();
	if (awardFileName.isEmpty()){
		slotSearchAwardFilekPushButtonClicked();
	}else{
  		QFile file(awardFileName);
		if ( file.open( IO_ReadOnly ) ) {
			award.clearAward();
			award.readAwardHeaders(awardFileName);
			strings = award.getEntityPrefix();	// Strings here includes the list of prefixes for the award that is being read.
			
//Check if the prefixes of the award to be checked are already added!
// KLog can manage only ONE award per Entity

	for ( QStringList::Iterator it = prefixesOfAwards.begin(); it != prefixesOfAwards.end(); ++it ) {
		for ( QStringList::Iterator ite = strings.begin(); ite != strings.end(); ++ite ) {
			if ((*it)==(*ite)){ 
				QMessageBox::about( this, i18n("KLog Warning!"),
                            i18n("Prefix from - " +(*it)+ " - the award: - "+award.getAwardName()+ " - repeated!\n"
                            "KLog can only manage one award per prefix.\nThis award will not be included\nCheck your klogrc file and/or the award file and fix the problem."));
				return;
			}
		}
	}
	QListViewItem * item = new QListViewItem( setupui::awardListView, 0 );
	i = getLastAwardNumber();
	i++;
	item->setText( 0, QString::number(i) );
	item->setText( 1, award.getAwardName() );
	item->setText( 2, (strings).join(", ") );
	item->setText( 3, awardFileName.right(awardFileName.length()-klogDir.length()-1) );

// I have ALL the prefixes passed for previous awards
	prefixesOfAwards = prefixesOfAwards + strings;
		}else{
			QMessageBox::about( this, i18n("KLog Warning!"),
                            i18n("KLog can not open the file:-"+awardFileName+ "!\n"
                            "That award will not be read.\nCheck your klogrc file and fix the problem."));
		}

	}
}

void Setup::slotAwardClearButtonClicked(){
//	cout << "Setup::slotAwardClearButtonClicked" << endl;

	setupui::awardFilenamekLineEdit->clear();


}

void Setup::slotAwardRemoveButtonClicked(){
	//cout << "Setup::slotAwardRemoveButtonClicked" << endl;
	awardFileName = setupui::awardFilenamekLineEdit->text();
	if (awardFileName.isEmpty()){
		
	}else{
		if (awardToDelete != -1){
			// Before calling to delete we have to prepare it all
			slotAwardDelete ();
		}	

	}

}

void Setup::slotAddDxClusterPushButtonClicked(){
	//cout << "Setup::slotAddDxClusterPushButtonClicked()" << endl;
	bool ok;
	ok = false;
	while (!ok){
		QString text = QInputDialog::getText(
            "KLog: Add a DXCluster server", "Add the address followed by the :port\nExample: eadx.net:23\nIf no port is specified, 41112 will be used by default.:", QLineEdit::Normal,
            QString::null, &ok, this );
		if ( ok && !text.isEmpty() ) {
			if (checkIfValidDXCluster(text)){
				if(checkIfNewDXCluster(text)){
					ok = true;
					if ((text.contains(":"))==0){
						text = text+":41112";
					}
					setupui::dxClustercomboBox->insertItem(text);
				}else{
					ok = false;
				}
			}else{
				ok = false;
			}
		}else {
        // user entered nothing or pressed Cancel
			ok = true;
		}
	} //Adds a dxcluster server
}
void Setup::slotClearDxClusterPushButtonClicked(){
	//cout << "Setup::slotClearDxClusterPushButtonClicked()" << endl;
/*

	dxclusterServerListItems = setupui::dxClustercomboBox->count();
	for (i = 0; i<=dxclusterServerListItems; i++){
		setupui::dxClustercomboBox->setCurrentItem(i);
		stream << "DXClusterServerPort=" << setupui::dxClustercomboBox->currentText() << endl;
	}
*/
	setupui::dxClustercomboBox->removeItem(setupui::dxClustercomboBox->currentItem());

}


void Setup::slotAwardRightButtonFromList(QListViewItem * item, const QPoint &p){
	//cout << "Setup::slotAwardRightButtonFromLog" << endl;
	if (item){
		Setup::i = (item->text(0)).toInt(); // i is the QSO number
		//showMenuRightButton(Setup::i, p);
		showMenuRightButton(item, p);
	}
}

void Setup::slotAwardSelected(QListViewItem* item){
	//cout << "Setup::slotAwardSelected" << endl;
	setupui::awardFilenamekLineEdit->setText(klogDir+"/"+(item->text(3)));
	awardToDelete = ((item)->text(0)).toInt();
}


void Setup::showMenuRightButton(QListViewItem * item, const QPoint &p){

	//cout << "Setup::showMenuRightButton = " << item->text(1) << endl;
	Setup::editdeletePixMap = new QPixmap("editdelete.png");
	awardToDelete = ((item)->text(0)).toInt();
	KPopupMenu *awardMenu = new KPopupMenu( this );
	awardMenu->insertItem( *editdeletePixMap, i18n("Delete"), this, SLOT( slotAwardDelete() ), CTRL + Key_D );
	awardMenu->insertSeparator();
	awardMenu->exec(p);

}

void Setup::slotAwardDelete (){
	//cout << "Setup::slotAwardDelete: " << QString::number(awardToDelete) << endl;
				QMessageBox::about( this, i18n("KLog Warning!"),
                            i18n("This function is still not implemented.\nTo erase an award, delete the appropriate line of your klogrc file."));

/*

	QListViewItemIterator itl( setupui::awardListView );
	QStringList::iterator ita;
	QStringList::iterator itb;
	//QStringList::iterator itc;

	QString auxString;
	QString firstPrefix;
	int numberOfPrefixes; 
	QStringList auxPrefixesOfAwards;
	auxPrefixesOfAwards.clear();

	for ( ; itl.current(); ++itl ){
		if ((((*itl)->text(0)).toInt()) == awardToDelete){
			auxPrefixesOfAwards = QStringList::split( ", ",  ((*itl)->text(2)));
			setupui::awardListView->takeItem(*itl);
		}	

		for ( itb = auxPrefixesOfAwards.begin(); itb != auxPrefixesOfAwards.end(); ++itb  ){
			for ( ita = prefixesOfAwards.begin(); ita != prefixesOfAwards.end(); ++ita){
				if ((*ita)==(*itb)){	
					prefixesOfAwards.remove(ita);	
				} 
	
			}
			
		}
	}
	awardToDelete = -1;

*/
}

int Setup::getLastAwardNumber(){
	awardN = 0;
	QListViewItemIterator itl( setupui::awardListView );
//	itl.last()
	for ( ; itl.current(); ++itl ){
		awardN = ((*itl)->text(0)).toInt();
		}
	return awardN+1;
}

bool Setup::checkIfValidDXCluster(const QString &tdxcluster){
	QUrl url("http://"+tdxcluster);
	if ((url.hasHost())||(url.hasPort()))
		return true;
	else
		return false;
}

bool Setup::checkIfNewDXCluster(const QString &tdxcluster){
	//cout << "checkIfNewDXCluster: -" << tdxcluster << "-"<< endl;
	dxclusterServerListItems = setupui::dxClustercomboBox->count();
	for (i = 0; i<=dxclusterServerListItems-1; i++){
		if ((setupui::dxClustercomboBox->currentText())==(tdxcluster)){
			return false;
		}else{
			return true;
		} 
	}
	return true;
}
/*
void Setup::showMenuRightButton(int taward, const QPoint &p){
	////cout << "KLog::showMenuRightButton" << endl;
	if (taward >= 0){
		qso = getByNumber(qqso);
		KPopupMenu *awardMenu = new KPopupMenu( this );
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
int Setup::addRigToList(const struct rig_caps* caps, void* data){
	QString name;
	Setup* r = (Setup*)data;
	name=caps->model_name;
	if (name == "Dummy")
		name = "None";
	r->rigname2rigid[name] = caps->rig_model;
	r->rigid2rigname[caps->rig_model] = name;
	r->strings << name;
	return -1; // not 0 --> we want all rigs
}

void Setup::setRig()
{
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's
	rig_set_debug(RIG_DEBUG_NONE);

	// and continue...
	rig_load_all_backends();
	rig_list_foreach(addRigToList, this);
	strings.sort();
	comboBoxTrvList->insertStringList(strings);
	strings.clear();
}
//////////////////////////////////////////////////////////////////////////////
// End of modifications
//////////////////////////////////////////////////////////////////////////////

int Setup::bauds2Num(const QString& tbauds){
	switch (tbauds.toInt()){

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

		default: // 
			return 2;
		break; 
	}


}
