
/***************************************************************************
                          klog.cpp  -  description
                             -------------------
    begin                : sáb dic  7 18:42:45 CET 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/
/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the Free Software              *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*
 *****************************************************************************/
//#include <klocale.h>
//#include <ostream.h>
#include "klog.h"

using namespace std;

Klog::Klog(QWidget *parent, const char *name) : klogui(parent, name)
{
	//cout << "KLog::Constructor" << endl;
	QTimer *internalTimer = new QTimer( this ); // create internal timer
	connect( internalTimer, SIGNAL(timeout()), SLOT(slotUpdateTime()) );
	internalTimer->start( 1000 );               // emit signal every 5 seconds

	// This QTimer if for hamlib. We only create it.
	QTimer *hamlibtimer = new QTimer( this ); // The QTimer is always created but only started if needed
	//hamlibtimer = new QTimer( this ); // The QTimer is always created but
	//connect( hamlibtimer, SIGNAL(timeout()), this, SLOT(slothamlibUpdateFrequency()) );
	connect( hamlibtimer, SIGNAL(timeout()), this, SLOT(slothamlibUpdateFrequency()) );
	//   QTimer *hamlibtimer = new QTimer( this );
	//   connect( hamlibtimer, SIGNAL(timeout()), this, SLOT(slothamlibUpdateFrequency()) );
	   //hamlibtimer->start(200);


	listHamlib(); 

	Klog::KLogVersion = "0.4.2";
	Klog::editdeletePixMap = new QPixmap("editdelete.png");
	editdeleteOffPixMap = new QPixmap("editdeleteOff.png");
	Klog::qslRecPixMap = new QPixmap("qslRec.png");
	qslRecOffPixMap = new QPixmap("qslRecOff.png");
	Klog::qslSenPixMap = new QPixmap("qslSen.png");
	qslSenOffPixMap = new QPixmap("qslSenOff.png");
	blackColor.setNamedColor("#000000");
	for (i = 0; i<7; i++)
		haveAllMandatoryFields[i] = false;
	operatorStringAux = "";
	Klog::number = 0;
	Klog::confirmed = 0;
	timeInUTC = true; // Date is shown in UTC unless configured
	mode = "SSB";
	imode = 0;
	band = 2;
	power = "100";
	entiBak = 0;
	callFound = false;
	wasConfirmed = false;
	callLen = 0;
	callLenPrev = 0;
	callLenFound = 0;
	tTxValue = 0;  // SSB default
	tRxValue = 0;
	sTxValue = 9;
	sRxValue = 9;
	dxClusterPort = 0; // The cluster won't start if port == 0
	dxClusterConnected = false;
	dxClusterHFSpots=true;
	dxClusterVHFSpots=true;
	dxClusterWARCSpots=true;
	dxClusterWXANNounces=true;
	dxClusterWCYANNounces=true;  
	dxClusterANNounces=true;
	dxClusterConfirmedSpots=true;
	lastDelete = false;
	showProgressDialog = false;
	completeWithPrevious = true;
	requireMandatory = true;
	// hamlib support
	hamlib = false;
	hamlibPossible = false;
	rignameNumber = -1;
	bauds = 0;
	//strength = 0;
	hamlibInterval = 200;

	klogDir = QDir::homeDirPath()+"/.klog";  // We create the ~/.klog for the logs
	if (!QDir::setCurrent ( klogDir )){
		QDir d1(klogDir);
		dirExist = d1.mkdir(klogDir, true);
	}
	dirExist = QDir::setCurrent ( klogDir ) ;
	logFileNameToOpen = "";
	logFileNameToSave = "";

	
	tempLogFile = "tempklog.adi";

	//awards.readConfig();  // Starting the award checks
	
	
	readConf(); // read our data as myQrz, myLocator from ~/.klog/klogrc
	
	 

	if ((hamlibPossible) && (hamlib)){
		hamlibtimer->start(hamlibInterval);
		//hamlibtimer->start(0);
	}
	i = 0;
	QString comment;
	comment = "";
	//  world.create();
	slotClearBtn();
	modify = false;
	searching2QSL = false;
	actionSent = false;
	actionRec = false;
	//  prefixFound = false;
	(klogui::qsoDateTime->dateEdit())->setOrder(QDateEdit::DMY);
	(klogui::QSLSentdateEdit)->setOrder(QDateEdit::DMY);
	(klogui::QSLRecdateEdit)->setOrder(QDateEdit::DMY);
	slotQrzChanged();
	slotModeChanged(0); //SSB = 0, the default mode
	needToSave = false; // Initialized here to avoid needing to save just after the start
	// Finally, if we configured to open a file by default... we open it!
	if ((openLastByDefault == true) && (logFileNameToOpen !="")){
		adifReadLog(logFileNameToOpen);
	}	
	//showTip();	// TODO: We show a tip when KLog start
}

Klog::~Klog(){
//cout << "KLog::Destructor" << endl; 
}

bool Klog::haveWorld(){
//cout << "KLog::haveWorld" << endl;

  if (!world.isWorldCreated() ){
    switch( QMessageBox::information( this, i18n("Warning - Can't find cty.dat"),
                                      i18n("I can't find the cty.dat file with the data\n"
                                      "of the Entities. Do you want to continue without\nthis data?\n\n(Copy an updated cty.dat file to your ~/.klog dir, please.)"),
                                        i18n("Yes"), i18n("No"), 0, 1 ) ) {
      case 0: // Continue
        return true;
        break;
      case 1: // Continue
        return false;
        break;
    }
  }

  return true;

  
}


void Klog::slotLocatorChanged(){
// If the locator is changed, we should re-calculate distances...
// Manages the Locator of the DX
//cout << "KLog::slotLocatorChanged" << endl;

	dxLocator = getThisQSODXLocator();	//We first have to get the valid locator, from the call 
						// or from the user
					

	if (dxLocator != "NULL"){
		Klog::distance = locator.getDistanceKilometres(locator.getLon(getMyLocator()), locator.getLat(getMyLocator()), locator.getLon(dxLocator), locator.getLat(dxLocator));
		beam = locator.getBeam(locator.getLon(getMyLocator()), locator.getLat(getMyLocator()), locator.getLon(dxLocator), locator.getLat(dxLocator));
		showDistancesAndBeam(distance, beam);
	}else{
	// Should we call showDistancesAndBeam(0,0); ??????????
	}
}

void Klog::slotMyLocatorChanged(){
//When my locator changes, distances and beams should be recalculated!
//cout << "KLog::slotLocatorChanged" << endl;
	
	dxLocator = getThisQSODXLocator();

	if ((locator.isValidLocator((klogui::myLocatorLineEdit->text()).upper())) && ((klogui::myLocatorLineEdit->text()).upper() != getMyLocator()) ){
		qso.setMyLocator((klogui::myLocatorLineEdit->text()).upper());
	}else{
		qso.setMyLocator(getMyLocator());
	}
	
	myLocatorTemp = qso.getMyLocator();

	Klog::distance = locator.getDistanceKilometres(locator.getLon(myLocatorTemp), locator.getLat(myLocatorTemp), locator.getLon(dxLocator), locator.getLat(dxLocator));
	beam = locator.getBeam(locator.getLon(myLocatorTemp), locator.getLat(myLocatorTemp), locator.getLon(dxLocator), locator.getLat(dxLocator));
	showDistancesAndBeam(distance, beam);


}

void Klog::showDistancesAndBeam(const int dist, const int beam)
{
//cout << "KLog::showDistancesAndBeam" << endl;
	klogui::distancelCDNumber->display(dist);
	klogui::distancellCDNumber->display(40000 - dist);
	klogui::beamshortlCDNumber->display(beam);
	if (beam >= 180)
		klogui::beamlonglCDNumber->display(beam - 180);
	else
		klogui::beamlonglCDNumber->display(180 + beam);
}

QString Klog::getThisQSODXLocator (){
//cout << "KLog::getLocatorFromCall" << endl;
// Firstly we check if the user has entered one locator and, if hasn't
// We read the DX QRZ and get a default locator from it.

	if (locator.isValidLocator((klogui::locatorLineEdit->text()).upper())) { //User's locator
		return (klogui::locatorLineEdit->text()).upper();
	}else{
		if (locator.isValidLocator(locator.getLocator((world.getEntByNumb(enti)).getLon(),(world.getEntByNumb(enti)).getLat()))   ){
			return locator.getLocator((world.getEntByNumb(enti)).getLon(),(world.getEntByNumb(enti)).getLat());
		}
	}
	return "NULL";
}


int Klog::getEntityFromCall(){ // We return the Entity number from the QRZ box call.
//cout << "KLog::getEntityFromCall" << endl;
	return world.findEntity((klogui::qrzLineEdit->text()).upper());
}

void Klog::slotQrzChanged()   // We set the QRZ in the QSO
{
//cout << "KLog: slotQrzChanged" << endl;
//  needToSave = true;
	klogui::qrzLineEdit->setText(((klogui::qrzLineEdit->text())).upper()); 
	callLen = (klogui::qrzLineEdit->text()).length();
	if ((callLen == 0) && (callLen<callLenPrev)){ // We are deleting...
		callLenPrev = callLen; // just to avoid a no end loop  
  		slotCancelSearchButton(); 
		slotClearBtn();
 		return;
	}else if ((callLen !=0)&&(!modify)){ // Updating the searchQrzklineEdit if we are not modifying a QSO.
		enti = getEntityFromCall();
		if (enti!=0){ 
			if (entiBak == enti){
				callLenPrev = callLen;
				showWhere(entiBak); 
				return; // We do not need to do nothing except to
					// update the callen
					// We do not have to update the awards, ...
			}
			entiBak = enti;
			// Only if we detect the entity we look for a previous qso 
			// Just copying the string to the search box we will search for previously worked QSOs
			klogui::searchQrzkLineEdit->setText((klogui::qrzLineEdit->text()).upper());
		}else{ //Enti = 0 so no Entity has been located...

		}
		
	}
	// The next 3 were called with entiBak
	prepareIOTAComboBox(enti);
	prepareAwardComboBox(enti);
	showWhere(enti);

	callLenPrev = callLen;
	showIfPreviouslyWorked();
	searching2QSL = false;	// If the user enters a QSO we finish the search2QSL process
}


void Klog::prepareIOTAComboBox (const int tenti){
// We receive the Entity, get the continent and write it to the IOTA combobox
//cout << "KLog: prepareIOTAComboBox" << endl;
	i = 0;
	i = adif.continent2Number((world.getEntByNumb(tenti)).getContinent());
	klogui::iotaComboBox->setCurrentItem (i);
}

void Klog::prepareAwardComboBox(const int tenti){
//Finds if the Entity received has an award to be controlled and show the
//it on the award box
//cout << "KLog: prepareAwardComboBox" << endl;
	if (tenti <= 0)
		return;

// Prepare to receive the award but... what if it does not change?
	klogui::awardsComboBox->clear();
	awardReferences.clear(); // We clear the list
	award.clearAward();

	award = awards.getAwardFor((world.getEntByNumb(tenti)).getPfx());
	if (award.getAwardReference() == -1){  // we do not have any award for this entity.
		klogui::awardsComboBox->setEnabled(false);
		klogui::awardsComboBox->clear();
		klogui::awardSelectorName->setEnabled(false);
		klogui::awardSelectorName->setText(i18n("Award:"));  
		klogui::localAwardName->setText(i18n("LOCAL:"));
		klogui::localAwardName->setEnabled(false);
		return;
	}else{
		
		aux = "<b>" + award.getAwardName() + ":</b>";
		klogui::awardSelectorName->setText(aux);
		klogui::awardsComboBox->setEnabled(true);
		klogui::localAwardName->setText(aux);
		klogui::localAwardName->setEnabled(true);
		klogui::awardSelectorName->setEnabled(true);
		
		aux = (world.getEntByNumb(tenti)).getPfx();
		klogui::awardsComboBox->insertStringList(awards.getAwardReferences(award));
	}
}


void Klog::slotClearBtn(){
// This method clears all for the next QSO
// It is still missing the part to set the cursor to the klogui::qrzLineEdit-> 

//cout << "KLog::slotClearBtn" << endl;
	enti = -1;
	wasConfirmed = false;
	Klog::j = 0;
	qso.clearQso(); // Clears the qso object
	//if (!modify){
		klogui::bandComboBox->setCurrentItem(band);
		klogui::modeComboBox->setCurrentItem(imode);
	//	//    klogui::operatorLineEdit->clear();
	//}else{
	//	klogui::bandComboBox->setCurrentItem(2);
	//	klogui::modeComboBox->setCurrentItem(1);
	//	klogui::operatorLineEdit->clear(); 
	//}
	Klog::modify = false; // We will add the QSOs
	Klog::actionSent = false;
	Klog::actionRec = false;
	//  Klog::prefixFound = false; // We will look for the Entity
	Klog::award = awards.getDefaultAward();
	Klog::entiBak = 0;
	Klog::enti = 0;
	Klog::callFound = false;
	Klog::callLen = 0;
	Klog::callLenPrev = 0;
	Klog::callLenFound = 0;  
	Klog::lastDelete = false;
	if   ((klogui::qrzLineEdit->text()).length() < 1){ // A double clicking of this button shall erase ALL
		klogui::operatorLineEdit->clear();
	}
	if ((klogui::operatorLineEdit->text()).length() < 3) {
		klogui::operatorLineEdit->clear();
	}
	//We update the time.
	//After that we clean the call/mode/band
	klogui::qrzLineEdit->clear();
	klogui::remarksTextEdit->clear();
	klogui::iotaComboBox->setCurrentItem(0);
	klogui::iotaIntSpinBox->setValue(0);
	//klogui::iotaIntSpinBox->setEnabled(false);
	klogui::TSendBox->setValue(tTxValue);
	klogui::SSendBox->setValue(9);
	klogui::RSendBox->setValue(5);
	klogui::TRecBox->setValue(tRxValue);
	klogui::SRecBox->setValue(9);
	klogui::RRecBox->setValue(5);
	klogui::okBtn->setText(i18n("Ok"));
	klogui::clearBtn->setText(i18n("Clear"));

	klogui::QSLSentcheckBox->setChecked(false);
	klogui::QSLReccheckBox->setChecked(false);
	klogui::powerSpinBox->setValue(power.toInt());
	klogui::qthkLineEdit->clear();
	// klogui::operatorLineEdit->clear();
	klogui::namekLineEdit->clear();
	//TODO: This date is not valid, it is out of range!
	qslSen = QDate::fromString("0000-00-00",Qt::ISODate);
	(klogui::QSLSentdateEdit)->setDate(qslSen);
	(klogui::QSLSentdateEdit)->setEnabled(false);
	//TODO: This date is not valid, it is out of range!
	qslRec = QDate::fromString("0000-00-00",Qt::ISODate);
	(klogui::QSLRecdateEdit)->setDate(qslRec);
	(klogui::QSLRecdateEdit)->setEnabled(false);
	klogui::qslVialineEdit->setDisabled(true); // Next is the QSL info
	//  klogui::QSLInfotextEdit->setDisabled(true);
	klogui::qslVialineEdit->clear();
	klogui::QSLInfotextEdit->clear();
	klogui::QSLcomboBox->setCurrentItem(0);

	klogui::locatorLineEdit->clear();
	Klog::dxLocator="NULL";
	klogui::myLocatorLineEdit->clear();
	Klog::myLocatorTemp = getMyLocator();  //My default locator from the klogrc
	qso.setMyLocator(getMyLocator());

	klogui::freqlCDNumber->display(0); // Setting the frequency box to 0
	klogui::localAwardName->setText("LOCAL:");
	klogui::localAwardName->setEnabled(false);
	klogui::awardsComboBox->clear();
	klogui::awardsComboBox->setEnabled(false);
	klogui::awardsComboBox->clear();
	klogui::awardSelectorName->setEnabled(false);
	klogui::awardSelectorName->setText("Award:");
	showDistancesAndBeam(0,0);
	clearEntityBox();
	// hamlib
	//strength = 0;
	hamlibReading = true;  
	klogui::qrzLineEdit->setFocus();		// The default widget for next QSO is, obviously, the QRZ!
	searching2QSL = false;	// If the user decides to clear the qrzlinedit, we finish the search 2 QSL process.
}

void Klog::clearEntityBox(){
// This only clear the Entity box, the distances, bearing, entity, ...
//cout << "KLog::clearEntityBox" << endl;
	klogui::entityTextLabel->setText("");
	Klog::distance = 0;
	Klog::beam = 0;
	showDistancesAndBeam(0,0);
	klogui::entityTextLabel->setText("");
	klogui::prxTextLabel->setText("");
	klogui::continentTextLabel->setText("");
	klogui::cqLCDNumber->display(00);
	klogui::ituLCDNumber->display(00);
	klogui::LedtextLabel->setText(i18n( "<p align=\"center\"><b>KLog</b></p>"));
	klogui::beamshortlCDNumber->display(0);
	klogui::distancelCDNumber->display(0);
	klogui::beamlonglCDNumber->display(0);
	klogui::distancellCDNumber->display(0);

	klogui::newEntitykLed->setColor(defaultColor);
	klogui::textLabelBand2->setPaletteBackgroundColor(QColor::QColor(defaultColor) );
	klogui::textLabelBand70cm->setPaletteBackgroundColor(QColor::QColor(defaultColor) );
	klogui::textLabelBand6->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand10->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand12->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand15->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand17->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand20->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand30->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand40->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand80->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	klogui::textLabelBand160->setPaletteBackgroundColor(QColor::QColor( defaultColor ) );
	
}


void Klog::slotOkBtn(){
// The QSO is entered!!!
//cout << "KLog::slotOkBtn" << endl;
	number++;
	readQso();
	if ((qso.getQrz()).length() >= 3){//There are no qrz with less than 3char
		needToSave = true;
		
		if (!modify){
			
			logbook.append(qso);
			templogbook.append(qso); //Save the just done QSO to do the auto-saving
			adifTempFileSave(tempLogFile, templogbook, false); //Saving
			addToPreviouslyWorked(); 
			if (enti != 0){
				dxcc.worked(enti,klogui::bandComboBox->currentItem(),klogui::modeComboBox->currentItem());

				waz.worked( world.getCqz() ,klogui::bandComboBox->currentItem(),klogui::modeComboBox->currentItem());
			}
		}else{ // We are not ADDING but modifying a QSO.
			number--;
			modifyQso();
		}
		showQso();
	}else{number--;}//Closes the empty call check  

	slotClearBtn();
	showLogList();
	showAwardsNumbers(); //Needed?
}


void Klog::fileSave(){
//cout << "KLog::fileSave" << endl;
	if (logFileNameToSave != ""){
		//adifFileSave(); // Saving as ADIF
		adifTempFileSave(logFileNameToSave, logbook, true);
	}else{
		fileSaveAs(); 
	}
}

void Klog::fileSaveAs(){
//cout << "KLog::fileSaveAs" << endl;
  bool writ;
  writ = false;
  while (!writ){

    QString fn = QFileDialog::getSaveFileName(
                    klogDir,
                    "ADIF (*.adi *.ADI);;Tlf (*.log)",
                    this,
                    i18n("save file dialog"
                    "Choose a filename to save under") );

    if ( !fn.isEmpty() ) 
      logFileNameToSave = fn;
    QFile file( logFileNameToSave );
      if ( file.exists( ) ) {


      switch( QMessageBox::information( this, i18n("Warning - File exists"),
                                      i18n("The file: " + logFileNameToSave +" already exits.\n"
                                      "Do you want to overwrite?"),
                                      i18n("Overwrite"), i18n("Cancel"), i18n("Rename"),
                                      0, 1 ) ) {
      case 0:
        writ = true;
      break;
      case 1:
        return;
      break;
      default: // just for sanity
        return;
      break;
      }
    }else{
      writ = true; 
    }
  }
  if ( !logFileNameToSave.isEmpty() ) {
    //adifFileSave();
	adifTempFileSave(logFileNameToSave, logbook, true);
  }else {
    statusBar()->message( i18n("Saving aborted"), 2000 );
  }
}

/*
void Klog::adifFileSave(){
cout << "KLog::adifFileSave" << endl;
      logFileNameToSave = checkExtension(logFileNameToSave);
	QFile file( logFileNameToSave );

	if ( file.open( IO_WriteOnly ) ) {
	int progresStep = 0;
	QProgressDialog progress( i18n("Saving the log..."), i18n("Abort saving"), Klog::number,
                          this, i18n("progress"), TRUE );

	QTextStream stream( &file );
	Klog::LogBook::iterator it;
	stream << i18n("ADIF v1.0 (some ADIF v2 fields) Export from KLog-") + Klog::KLogVersion + " \nhttp://jaime.robles.nu/klog"
    << "\n<APP_KLOG_NUMBER:" << QString::number( Klog::number ).length() << ">" << QString::number(Klog::number) << i18n("\nLog saved: ") << dateTime.toString("yyyyMMdd") << "-" << dateTime.toString("hhmm") << "\n<PROGRAMID:4>KLOG <PROGRAMVERSION:" + QString::number((Klog::KLogVersion).length()) << ":" << Klog::KLogVersion << "> \n<EOH>\n" << endl;
	for ( it = logbook.begin(); it != logbook.end(); ++it ){
		if ( progress.wasCanceled())
			return;
		stream << "<CALL:" <<((*it).getQrz()).length() << ">" << (*it).getQrz()
         << " <QSO_DATE:8>" << (*it).getDateTime().toString("yyyyMMdd")
        << " <TIME_ON:4>" << (*it).getDateTime().toString("hhmm")
        << " <MODE:" << ((*it).getMode()).length() << ">" << ((*it).getMode()).upper()
        << " <BAND:" << ((*it).getBand()).length() << ">" << ((*it).getBand()).upper()
        << " <RST_SENT:" << QString::number( (*it).getRsttx()).length() << ">" << (*it).getRsttx()
        << " <RST_RCVD:" << QString::number((*it).getRstrx()).length() << ">" << (*it).getRstrx()
        << " <TX_PWR:" << ((*it).getPower()).length() << ">" << (*it).getPower()
        << " <QSL_RCVD:1>" << (*it).isQslRec()
        << " <QSL_SENT:1>" << (*it).isQslSent();
	if ( ((*it).getFreq()).toDouble() != 0.0) {
          stream << " <FREQ:" <<((*it).getFreq()).length() << ">" << (*it).getFreq();
        }
	if ( ((*it).getrxPower()).toInt() != 0) {
		stream << " <RX_PWR:" << ((*it).getrxPower()).length() << ">" << (*it).getrxPower();
        }
        if ((*it).getIotaNumber()!= 0)
          stream << " <IOTA:" <<((*it).getIota()).length() << ">" << (*it).getIota();
	if ((*it).getLocalAwardNumber() != 0)
          stream << " <STATE:" << ( (*it).getLocalAward()).length() << ">" << (*it).getLocalAward();
        if (((*it).getName()).length()>= 2)
          stream << " <NAME:" <<((*it).getName()).length() << ">" << (*it).getName();

        if (((*it).getQth()).length()>= 2)
          stream << " <QTH:" <<((*it).getQth()).length() << ">" << (*it).getQth();

        if (((*it).getOperator()).length()>= 3)
          stream << " <OPERATOR:" <<((*it).getOperator()).length() << ">" << (*it).getOperator();

        if ((((*it).getLocator()).length() == 6) || (((*it).getLocator()).length() == 6))
          stream << " <GRIDSQUARE:" <<((*it).getLocator()).length() << ">" << (*it).getLocator();

	if (((*it).getMyLocator())!="NULL"){
		          stream << " <MY_GRIDSQUARE:" <<((*it).getMyLocator()).length() << ">" << (*it).getMyLocator();
	}
        if ((*it).sentTheQSL()){
          stream << " <QSLSDATE:8>" << (*it).getQslSenDate().toString("yyyyMMdd");
        }
        if ((*it).gotTheQSL()){
          stream << " <QSLRDATE:8>" << (*it).getQslRecDate().toString("yyyyMMdd");
        }

        if (((*it).getQslVia()).compare("No QSL") == 0){ //Write nothing

        }else{
          if ((*it).getQslVia() == "Manager"){ //If there is a manager
            if (((*it).getQslManager()).length()>0)
              stream << " <QSL_VIA:" <<((*it).getQslManager()).length() << ">" << (*it).getQslManager();
          }//There is no manager but there is QSL via
          else{
            if ((((*it).getQslVia()).length())>0)
              stream << " <QSL_VIA:" <<((*it).getQslVia()).length() << ">" << (*it).getQslVia();
          }
        }
        if ( ((*it).getQslInfo().length()) >= 1){
          stream << " <QSLMSG:" <<((*it).getQslInfo()).length() << ">" << (*it).getQslInfo();          
        }
        if (((*it).getComment()).length()>0)
          stream << " <COMMENT:" << ((*it).getComment()).length() << ">" << (*it).getComment();
        stream << " <EOR>" << endl;
        progresStep++;
        if (showProgressDialog){
          progress.setProgress( progresStep );
          qApp->processEvents();
          if ( progress.wasCanceled())
            return;
        }

    }
    file.close();
	templogbook.clear(); // We have saved the whole log, so the temp has also been saved.
  }
  needToSave = false;

}
*/

void Klog::adifTempFileSave(const QString& fn, LogBook lb, bool manualSave){
//adifTempFileSave(logFileNameToSave, logbook, true)
//adifTempFileSave(logFileNameToSave, tempLogbook, false)
//cout << "KLog::adifTempFileSave" << endl;

	//logFileNameToSave = checkExtension(fn);
	QString fileToSave;
	fileToSave = checkExtension(fn);

	QFile file( fileToSave );

	if ( file.open( IO_WriteOnly ) ) {
//		if (manualSave){
			int progresStep = 0;
			QProgressDialog progress( i18n("Saving the log..."), i18n("Abort saving"), Klog::number,
                          this, i18n("progress"), TRUE );
//		}

	QTextStream stream( &file );
	Klog::LogBook::iterator it;
	stream << i18n("ADIF v1.0 (some ADIF v2 fields) Export from KLog-") + Klog::KLogVersion + " \nhttp://jaime.robles.nu/klog"
    << "\n<APP_KLOG_NUMBER:" << QString::number( Klog::number ).length() << ">" << QString::number(Klog::number) << i18n("\nLog saved: ") << dateTime.toString("yyyyMMdd") << "-" << dateTime.toString("hhmm") << "\n<PROGRAMID:4>KLOG <PROGRAMVERSION:" + QString::number((Klog::KLogVersion).length()) << ":" << Klog::KLogVersion << "> \n<EOH>\n" << endl;
	for ( it = lb.begin(); it != lb.end(); ++it ){
		if ( progress.wasCanceled())
			return;
		stream << "<CALL:" <<((*it).getQrz()).length() << ">" << (*it).getQrz()
         << " <QSO_DATE:8>" << (*it).getDateTime().toString("yyyyMMdd")
        << " <TIME_ON:4>" << (*it).getDateTime().toString("hhmm")
        << " <MODE:" << ((*it).getMode()).length() << ">" << ((*it).getMode()).upper()
        << " <BAND:" << ((*it).getBand()).length() << ">" << ((*it).getBand()).upper()
        << " <RST_SENT:" << QString::number( (*it).getRsttx()).length() << ">" << (*it).getRsttx()
        << " <RST_RCVD:" << QString::number((*it).getRstrx()).length() << ">" << (*it).getRstrx()
        << " <TX_PWR:" << ((*it).getPower()).length() << ">" << (*it).getPower()
        << " <QSL_RCVD:1>" << (*it).isQslRec()
        << " <QSL_SENT:1>" << (*it).isQslSent();

	if ( ((*it).getrxPower()).toInt() != 0) {
		stream << " <RX_PWR:" << ((*it).getrxPower()).length() << ">" << (*it).getrxPower();
        }

        //if ( ((*it).getFreq()).length() != 0) {
	if ( ((*it).getFreq()).toDouble() != 0.0) {
		stream << " <FREQ:" <<((*it).getFreq()).length() << ">" << (*it).getFreq();
	}
	if ((*it).getIotaNumber()!= 0)
		stream << " <IOTA:" <<((*it).getIota()).length() << ">" << (*it).getIota();
	if ((*it).getLocalAwardNumber() != 0)
		stream << " <STATE:" << ( (*it).getLocalAward()).length() << ">" << (*it).getLocalAward();
	if (((*it).getName()).length()>= 2)
		stream << " <NAME:" <<((*it).getName()).length() << ">" << (*it).getName();
	if (((*it).getQth()).length()>= 2)
		stream << " <QTH:" <<((*it).getQth()).length() << ">" << (*it).getQth();
	if (((*it).getOperator()).length()>= 3)
		stream << " <OPERATOR:" <<((*it).getOperator()).length() << ">" << (*it).getOperator();
	if ((((*it).getLocator()).length() == 6) || (((*it).getLocator()).length() == 6))
		stream << " <GRIDSQUARE:" <<((*it).getLocator()).length() << ">" << (*it).getLocator();
	if (((*it).getMyLocator())!="NULL"){
		          stream << " <MY_GRIDSQUARE:" <<((*it).getMyLocator()).length() << ">" << (*it).getMyLocator();
	}
	if  ((*it).sentTheQSL() && (((*it).getQslSenDate()).isValid() )){
		stream << " <QSLSDATE:8>" << (*it).getQslSenDate().toString("yyyyMMdd");
	}
	if ( ( (*it).gotTheQSL() ) && ( ((*it).getQslRecDate()).isValid() ) ){
		stream << " <QSLRDATE:8>" << (*it).getQslRecDate().toString("yyyyMMdd");
	}
	if (((*it).getQslVia()).compare("No QSL") == 0){ //Write nothing
	}else{
		if ((*it).getQslVia() == "Manager"){ //If there is a manager
			if (((*it).getQslManager()).length()>0)
				stream << " <QSL_VIA:" <<((*it).getQslManager()).length() << ">" << (*it).getQslManager();
		}//There is no manager but there is QSL via
		else{
			if ((((*it).getQslVia()).length())>0)
				stream << " <QSL_VIA:" <<((*it).getQslVia()).length() << ">" << (*it).getQslVia();
		}
	}
	if ( ((*it).getQslInfo().length()) >= 1){
		stream << " <QSLMSG:" <<((*it).getQslInfo()).length() << ">" << (*it).getQslInfo();          
	}
	if (((*it).getComment()).length()>0)
		stream << " <COMMENT:" << ((*it).getComment()).length() << ">" << (*it).getComment();
		stream << " <EOR>" << endl;


//	if (manualSave){
		progresStep++;
		if (showProgressDialog){
			progress.setProgress( progresStep );
			qApp->processEvents();
			if ( progress.wasCanceled())
				return;
		}
//	}

    }
	file.close();
	if (manualSave){
		templogbook.clear(); // We have saved the whole log, so the temp has also been saved.
		QFile fileTemp( tempLogFile );
		if (fileTemp.remove()){
			//cout << "Temp file deleted" << endl;
		}
	}
	}
	needToSave = false;
}




// This function checks the filename.extension for the ADI
QString Klog::checkExtension(QString extension){
//cout << "KLog::checkExtension" << endl;
	int result;
	result = extension.find(".adi", -4, FALSE);
	if (result < 0)
		extension = extension + ".adi";
	return extension;
};

void Klog::addQSOToLog(){
//cout << "KLog::addQSOToLog: " << QString::number(qso.getNumb()) << " / " << qso.getQrz()<< endl;
    	Klog::needToSave = true;
          logbook.append(qso);
          if (enti != 0){
            dxcc.workedString(enti, qso.getBand(), qso.getMode());
            waz.workedString( world.getCqz(), qso.getBand(), qso.getMode() );
          }
        QListViewItem * item = new QListViewItem( klogui::logListView, 0 );
//        item->setText( 0, QString::number(qso.getNumb()) );
        item->setText( 0, getNumberString(qso.getNumb())  ); 
        item->setText( 1, qso.getDateTime().toString("yyyy-MM-dd") );
        item->setText( 2, qso.getDateTime().toString("hh:mm"));
        item->setText( 3, qso.getQrz().upper() );
        item->setText( 4, QString::number(qso.getRsttx()) );
        item->setText( 5, QString::number(qso.getRstrx()) );
        item->setText( 6, qso.getBand() );
        item->setText( 7, qso.getMode() );
        item->setText( 8, qso.getPower() );
        item->setText( 9, qso.getComment() );
 
// The next lines are for the previously worked functionality
/* Maybe they introduce BIG delays in the log reading from disk... to be checked
*/
/*
 kk = workedCall.getWorked( qso.getQrz()); 
    if (  kk < 0){
      workedCall.addCallToWorkedCalls(qso.getQrz(), qso.getNumb()); 
    }
*/
/*The previous lines are for the previouslyworked functionality*/

 qso.clearQso();
}

int Klog::getProgresStepForDialog(int totalSteps){
//cout << "KLog::getProgresStepForDialog" << endl;
	if (totalSteps <=100)
		return 2;
	else if (totalSteps <=1000)
		return 25;
	else if (totalSteps <=4000)
		return 100;
	else if (totalSteps <=5000)
		return 150;
	else if (totalSteps <=7000)
		return 250;
	else if (totalSteps <=9999)
		return 500;
	else
		return 1000;
}
// This is the readlog to read logs when each record is not only
// one line
// The next one is the old "newadifReadLog", it is a "testing" adifReadLog.

 // The actual "newadifReadLog" is a working/stable function but it cannot read "several-lines-per-qso"
// adif files.

void Klog::listHamlib(){
//cout << "KLog::listHamlib" << endl;
	//int status = 0; // the 0 is just for testing
	//cout << "ListHamlib: Before loading" << endl;
	rig_load_all_backends ();
	//cout << "ListHamlib: After loading" << endl;
//        status = rig_list_foreach (riglist_make_list, NULL);
 


}

void Klog::processLogLine (const QString& tLogLine){
//cout << "KLog::processLogLine" << tLogLine << endl;
	qsoLine="";
	adifTab="";
	theData="";
	dateString="";
	timeString="";
	datesString="";
	daterString="";
	qslViac = false;
	int intAux = 0;
	qsoLine = tLogLine;
	if (qsoLine.isEmpty()){
		return;
	}
	
	qsoLine = qsoLine.simplifyWhiteSpace().upper();

	intAux = qsoLine.contains('<');			// How many fields has the line?
	fields = QStringList::split('<', qsoLine );	// Split the line in fields
	for (int a = 0; a < intAux; a++) { // It was to aux-
		if (fields[a].contains(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
			adifTab = fields[a].section(':',0,0);
			callLen = (fields[a].section(':',1,1)).toInt();
			theData = ((fields[a]).section('>',1));
			theData.truncate(callLen);
		}else{
			adifTab = fields[a].section(':',0,0);
			callLen = (fields[a].section(':',1)).section('>',0,0).toInt();
			theData = ((fields[a]).section('>',1));
			theData.truncate(callLen);
		}
		if (adifTab == "CALL"){
			if (theData.length() >=2 ){  // Valid calls have at least 3 characters
				qso.setQrz(theData.upper());
				enti = world.findEntity(theData.upper());
				haveAllMandatoryFields[0] = true;
//				cout << "ReadLog: CALL: " << theData << endl;
			}
		}else if (adifTab == "EOR>"){
		// We have read the EOR so if we need all the mandatory qsoLine, we add the QSO and
		// prepare to read the next QSO, if we do not have all the mandatory qsoLine we simply
		// prepare to read the next QSO.
			if (haveAllTheFields()){
				dateString = dateString+"T"+timeString;
				qso.setDateTime(QDateTime::fromString(dateString, Qt::ISODate));
				if (qso.gotTheQSL()){
					qso.setQslRecDateOn(QDate::fromString(daterString, Qt::ISODate));
					dxcc.confirmedString(enti, qso.getBand(), qso.getMode());
					waz.confirmedString( world.getCqz(), qso.getBand(), qso.getMode());
					Klog::confirmed++;
				}
				if (qso.sentTheQSL()){
					qso.setQslSenDateOn(QDate::fromString(datesString, Qt::ISODate));
				}
				if (!qslViac){
					qso.setQslVia("No QSL");
					qslViac = false;
				}
                		// The following is mainly for importing logs. If the imported log was done
                		// using a different call than the actual log.
				if (((qso.getOperator()).length()<2) && (operatorStringAux.length()>2) && (!operatorStringAux.isEmpty())){
					qso.setOperator(operatorStringAux);
				}
				if (!textStringAux.isEmpty() ){ // We read the comment at the begining
					qso.setComment(qso.getComment() + " -- " + textStringAux);
				}
				Klog::number++;
				qso.setNumb(Klog::number);
				addQSOToLog();
				qso.setPower("0");  // A default power for the next QSO.
			}else{ // I do not have all the mandatory fields so NO QSO is readed
				//NO OPERATION
			}
            		// Resetting the mandatory fields to read a new QSO
			qso.clearQso();
			for (i = 0; i<7; i++){
				haveAllMandatoryFields[i] = false;
			}
		}else if (adifTab == "QSO_DATE"){
			dateString = theData;
			dateString.insert(4,'-');
			dateString.insert(7,'-');
			haveAllMandatoryFields[1] = true;
		}else if (adifTab == "TIME_ON"){
			timeString = theData;
			if (callLen == 4){
				timeString.insert(2,':');
				timeString = timeString + ":00";
			}else if (callLen == 6){
				timeString.insert(2,':');
				timeString.insert(5,':');
			}else{
				timeString.insert(13,":");
			}
			haveAllMandatoryFields[2] = true;
		}else if (adifTab == "RST_RCVD"){
			qso.setRstrx(theData.toInt());
			haveAllMandatoryFields[6] = true;
		}else if (adifTab == "RST_SENT"){
			qso.setRsttx(theData.toInt());
			haveAllMandatoryFields[5] = true;
		}else if (adifTab == "BAND"){
			qso.setBand(theData.upper());
			haveAllMandatoryFields[3] = true;
		}else if (adifTab == "FREQ"){  // We are still not ready for freq reading
			qso.setBand(adif.freq2Band(theData.upper()));
			qso.setFreq(theData.upper());
			//TODO: I SHOULD READ AND SET THE BAND JUST IF NOT ADDED BEFORE
		}else if (adifTab == "MODE"){
			qso.setMode(theData.upper());
			haveAllMandatoryFields[4] = true;
		}else if (adifTab == "TX_PWR"){
			qso.setPower(theData.upper());
		}else if (adifTab == "RX_PWR"){
			qso.setrxPower(theData.upper());
		}else if (adifTab == "COMMENT"){
			qso.setComment(theData);
		}else if (adifTab == "QSLSDATE"){
			datesString = theData;
			datesString.insert(4,'-');
			datesString.insert(7,'-');
			daterString = theData;
			daterString.insert(4,'-');
			daterString.insert(7,'-');
		}else if (adifTab == "QSL_RCVD"){
			qso.QslRec(theData[0]);
		}else if (adifTab == "QSL_SENT"){
			qso.QslSent(theData[0]);
		}else if (adifTab == "QSL_VIA"){ //¿Manager?
			if (theData == "No QSL"){
				qso.setQslVia(theData);
				qslViac = true;
			}else if (theData == "Bureau"){
				qso.setQslVia(theData);
				qslViac = true;
 			}else if (theData == "QRZ.com"){
				qso.setQslVia(theData);
				qslViac = true;
			}else if (theData == "Direct"){
				qso.setQslVia(theData);
				qslViac = true;
 			}else{
				qso.setQslVia("Manager");
				qso.setQslManager(theData);
				qslViac = true;
			}
		}else if (adifTab == "QSLMSG"){
			qso.setQslInfo(theData);
		}else if (adifTab == "GRIDSQUARE"){
			if(locator.isValidLocator(theData)){
				qso.setLocator(theData);
			}
		}else if (adifTab == "MY_GRIDSQUARE"){
			if(locator.isValidLocator(theData)){
				qso.setMyLocator(theData);
			}
		}else if (adifTab == "IOTA"){
			qso.setIota(theData);
		}else if ((adifTab == "STATE")||(adifTab == "VE_PROV")){
			// We need the long name of the entity but also the reference number
			// The ref number is just for processing the filesave and so on...
			if (world.findEntity(qso.getQrz())!=0){
				award = awards.getAwardFor(world.getPrefix());
				if (award.getReferenceNumber(klogui::awardsComboBox->currentText())){
					if (award.isValidReference(theData)){
						// << "Valid Reference" << endl;
						qso.setLocalAward(theData);
						qso.setLocalAwardNumber(award.getReferenceNumber(theData));
					}else{
						//cout << "Non-Valid Reference" << endl;
					}
				}
			}
		}else if ((adifTab == "OPERATOR")||(adifTab == "GUEST_OP")){
			qso.setOperator(theData);
		}else if (adifTab == "NAME"){
			qso.setName(theData);
		}else if (adifTab == "QTH"){
			qso.setQth(theData);
		}
	}
}

void Klog::adifReadLog(const QString& tfileName){
	//cout << "KLog::adifReadLog" << endl;
//  QDateTime start;
//  start = QDateTime::currentDateTime(Qt::LocalTime);
//  int aa = 0;
	QFile file( tfileName );
	int totalQsos = 0; // QSOs in the log to be read
//	bool qslViac = false;
	int progresStep = 0;
	QProgressDialog progress( i18n("Reading the log..."), i18n("Abort reading"), 0,
                          this, i18n("progress"), TRUE );
	QString progressLabel;
	QTextStream stream( &file );
	//QString line;
/*	QString adifTab;
	QString theData;
	QString dateString = "";
	QString timeString = "";
	QString datesString = "";
	QString daterString = ""; 
	*/
	int len; // for the tab length
	QString data = "";
	int aux = 0;
	bool inHeader = true;
	
	if ( file.open( IO_ReadOnly ) ) {
		data = stream.readLine();
		if (data.isEmpty()){	// If the logfile starts with empty lines...
			while (data.isEmpty()){
				data = stream.readLine();
			}
		}
		if (!data.startsWith("<") && (inHeader)){ // HEADER
			while ( (!stream.atEnd()) && (inHeader) ){
				data = stream.readLine();	// First line in header should not have usefull info.
				if (showProgressDialog){ // The user selected to see the progress dialog
					if(!data.isEmpty()){	
						data = data.simplifyWhiteSpace().upper();
						aux = data.contains('<');
						QStringList fieldss = QStringList::split('<', data );
						for (int a = 0; a < aux; a++){  //Dirty way to process just one field  	AUX is = 0 at this moment!!
							if (fieldss[a].contains("APP_KLOG_NUMBER") != 0){
								adifTab = fieldss[a].section(':',0,0);
							}
							len = (fieldss[a].section(':',1)).section('>',0,0).toInt();
							theData = ((fieldss[a].section(':',1)).section('>',1));
							theData.truncate(len);
							if (adifTab == "APP_KLOG_NUMBER"){
								totalQsos = theData.toInt();
								progress.setTotalSteps(totalQsos);
								adifTab = ""; // To avoid repeating this step
							}
						}
					}
				}
				if (( data.upper().contains("<EOH>") != 0 ) ){
				// We have finished to read the Header or the file does not have any header
					aux = 1;
					inHeader = false;
				}
			} // Closes the while	
		}else{ // LogBook
			if (((data.upper()).contains("<EOR>"))>=0){
				//HACK: Working with files with more than one line per QSO.
				/*********************************************************/
				while (((data.upper()).contains("<EOR>")==0)&&(!data.isEmpty())){
					data = data + "\\n" + stream.readLine();
				}
				/*********************************************************/
			}
			processLogLine (data);
			progresStep++;
		}
		// After processing the Header or the first line... we process the rest
		while ( (!stream.atEnd()) ){
			data = stream.readLine();
			if ( ((data.upper()).contains("<EOR>")) == 0){
				//HACK: Working with files with more than one line per QSO.
				/*********************************************************/
				while (((data.upper()).contains("<EOR>")==0)&&(!data.isEmpty()) ){
					data = data + "\\n" + stream.readLine();
				}
				/*********************************************************/
			}
			processLogLine (data);
			if (showProgressDialog){
				progresStep++;
				if ( (number % getProgresStepForDialog(totalQsos) )== 0){ // To update the speed i will only show the progress once each 25 QSOs
					progress.setProgress( progresStep );
					qApp->processEvents();
					progressLabel = i18n("Adding QSO#: ") + QString::number(number) + " / " + i18n(QString::number(totalQsos));
					progress.setLabelText(progressLabel);
				}
				if ( progress.wasCanceled()){
					return;
				}
			}
		} // Closes the while	
		file.close();
	} // Closes the if from the file (file could not be opened)

	//TODO Add the support to the checking for data if previously worked.
	if (!completeWithPrevious){
		getAllTheCallsFromLog();
	}
	needToSave = false;
	showAwardsNumbers();
}

void Klog::toEditQso(){
//cout << "KLog::toEditQSO" << endl;
	Klog::modify = true;
	Klog::j = qso.getNumb();
	klogui::qsoDateTime->setDateTime(qso.getDateTime()); // date
	klogui::qrzLineEdit->setText(qso.getQrz());
	klogui::RSendBox->setValue( (QString::number(qso.getRsttx()).at(0)).digitValue() );
	klogui::SSendBox->setValue( (QString::number(qso.getRsttx()).at(1)).digitValue() );
	klogui::TSendBox->setValue( (QString::number(qso.getRsttx()).at(2)).digitValue() );
	klogui::RRecBox->setValue( (QString::number(qso.getRstrx()).at(0)).digitValue() );
	klogui::SRecBox->setValue( (QString::number(qso.getRstrx()).at(1)).digitValue() );
	klogui::TRecBox->setValue( (QString::number(qso.getRstrx()).at(2)).digitValue() );
	klogui::modeComboBox->setCurrentText(qso.getMode());
	klogui::bandComboBox->setCurrentText(qso.getBand());
	klogui::powerSpinBox->setValue((qso.getPower()).toInt());
	klogui::remarksTextEdit->setText(returnLines(qso.getComment()));
	klogui::QSLSentcheckBox->setChecked(qso.sentTheQSL());
	klogui::QSLReccheckBox->setChecked(qso.gotTheQSL());
	klogui::QSLSentdateEdit->setDate(qso.getQslSenDate());
	klogui::QSLRecdateEdit->setDate(qso.getQslRecDate());

	if ((qso.getQth()).length() >=2)
		klogui::qthkLineEdit->setText(qso.getQth());
	if ((qso.getOperator()).length() >=3){
		klogui::operatorLineEdit->setText(qso.getOperator());
	}else{
		klogui::operatorLineEdit->clear();
	}
	if ((qso.getName()).length() >=2)
		klogui::namekLineEdit->setText(qso.getName());
	dxLocator = qso.getLocator();

	klogui::locatorLineEdit->setText(dxLocator);
	slotLocatorChanged();
	
	klogui::myLocatorLineEdit->setText(qso.getMyLocator());

	//If we have sent the QSL
	if (klogui::QSLSentcheckBox->isChecked()){
		(klogui::QSLSentdateEdit)->setEnabled(true);
		(klogui::QSLSentdateEdit)->setDate(qso.getQslSenDate());
	}else{
		(klogui::QSLSentdateEdit)->setEnabled(false);
	}
	// If we have received the QSL     
  	if (klogui::QSLReccheckBox->isChecked()){
		(klogui::QSLRecdateEdit)->setEnabled(true);
		qslRec =  qso.getQslRecDate(); //We need the qslRec in slotQslRecvBoxChanged
		(klogui::QSLRecdateEdit)->setDate(qslRec);

	}else{
		(klogui::QSLRecdateEdit)->setEnabled(false);
	}
	//Now the QSl info information
	klogui::QSLcomboBox->setCurrentText(qso.getQslVia());
  	if ((qso.getQslVia()).compare("No QSL") == 0){
		klogui::qslVialineEdit->setDisabled(true);
		//    klogui::QSLInfotextEdit->setDisabled(true);
		klogui::qslVialineEdit->clear();
		//    klogui::QSLInfotextEdit->clear();
		}else{
		if ((qso.getQslVia()).compare("Manager") == 0){
			klogui::qslVialineEdit->setEnabled(true);
			klogui::qslVialineEdit->setText(qso.getQslManager());
    		}else{
			klogui::qslVialineEdit->setDisabled(true);
		}
		//    klogui::QSLInfotextEdit->setEnabled(true);
		klogui::QSLInfotextEdit->setText(returnLines(qso.getQslInfo()));
	}
	// We need enti in other slots
	enti = 0;
	if (((klogui::qrzLineEdit->text()).upper()).length() != 0){
		enti = world.findEntity((klogui::qrzLineEdit->text()).upper());
	}
	// IOTA
	prepareIOTAComboBox(enti);
	if (qso.getIotaNumber() != 0) {
		i = 0;
		i = adif.continent2Number((qso.getIotaContinent()));
		
		klogui::iotaComboBox->setCurrentItem(i);
		//klogui::iotaIntSpinBox->setEnabled(true);
		klogui::iotaIntSpinBox->setValue(qso.getIotaNumber());
	}
	prepareAwardComboBox(enti);
	if (award.getReferenceNumber(qso.getLocalAward()) != -1 ) {	
		klogui::awardsComboBox->setCurrentItem(award.getReferenceNumber(qso.getLocalAward()));
//		klogui::awardsComboBox->setCurrentItem( qso.getLocalAward() );
		klogui::awardsComboBox->setEnabled(true);
		
	}
	showWhere(enti);
}


QString Klog::returnLines(const QString& tword){
//cout << " - Class KLog::returnLines" << endl;
	aux = tword;
	if (aux.contains("\\N")>0){
		aux.replace("\\N", QChar('\n'));
	}
	if (aux.contains("\\n")>0){
		aux.replace("\\n", QChar('\n'));
	}

	//cout << "KLog::returnLines: " << tword << " - " << aux << endl;
	return aux;


}

void Klog::slotQsoRightButtonFromLog(QListViewItem * item, const QPoint &p){
//cout << "KLog::slotQsoRightButtonFromLog" << endl;
	if (item){
		Klog::j = (item->text(0)).toInt(); // j is the QSO number
		showMenuRightButton(Klog::j, p);
	}    
}

void Klog::slotQsoRightButtonFromSearch(QListViewItem * item, const QPoint &p){
//Maybe This could be deleted and use the previous "slotQsoRightButtonFromLog" to perform
// this actions...    
//cout << "KLog::slotQsoRightButtonfromSearch" << endl;
	if (item){
		Klog::j = (item->text(7)).toInt(); // j is the QSO number
		showMenuRightButton(Klog::j, p);
		slotSearchButton();  // This is for updating the Entity state after QSLing.
	}
}

void Klog::showMenuRightButton(int qqso, const QPoint &p){
//cout << "KLog::showMenuRightButton" << endl;
	if (qqso >= 0){
		qso = getByNumber(qqso);
		kk = world.findEntity( qso.getQrz() );
		entityState(kk);
		KPopupMenu *qsoMenu = new KPopupMenu( this );
		qsoMenu->insertItem( *editdeletePixMap, i18n("Delete"), this, SLOT( slotQsoDelete() ), CTRL + Key_D );
		qsoMenu->insertSeparator();
		if (( !qso.gotTheQSL() ) && ( !qso.sentTheQSL() )){
			qsoMenu->insertItem( *qslRecPixMap, i18n("QSL Recv and Sent"), this, SLOT( slotQSLRecSent() ), CTRL + Key_B );
		}
		if ( !qso.gotTheQSL() ){
			qsoMenu->insertItem( *qslRecPixMap, i18n("QSL Received"), this, SLOT( slotQSLRec() ), CTRL + Key_R );
			showAwardsNumbers();
		}else{
		}
		if ( !qso.sentTheQSL() ){
			qsoMenu->insertItem( *qslSenPixMap, i18n("QSL Sent"), this, SLOT( slotQSLSent() ), CTRL + Key_S );
		}else{
		}
		qsoMenu->insertSeparator();
		qsoMenu->exec(p);
	}
}

void Klog::slotQsoSelectedForEdit(QListViewItem* item){
//cout << "KLog::slotQsoSelectedForEdit" << endl;
	if (item){
		slotClearBtn();
		hamlibReading = false;
		Klog::j = (item->text(0)).toInt(); // j is the QSO number from the loglist
		qso = getByNumber(Klog::j);
		toEditQso();
		klogui::okBtn->setText(i18n("Modify"));
		klogui::clearBtn->setText(i18n("Cancel"));
		Klog::callFound = false;
		Klog::entiBak = 0;
		Klog::callLen = 0;
		Klog::callLenPrev = 0;
		Klog::lastDelete = false;
 	}
}

void Klog::slotQsoSelected(QListViewItem* item){
//cout << "KLog::slotQsoSelected" << endl;
	if (item){
		kk = world.findEntity(item->text(3).upper());
		// kk this time is the Entity of the call selected
		if ((kk !=0) && (kk !=-1)){
			entityState(kk);
		}
		showWhere(kk); // We run the showWhere to change the box if the QSO is from an unknown entity
	}
}


// This slot just change the text from the OKButton to "Modify"
void Klog::slotModifyBtn(){
//cout << "KLog::slotModifyBtn" << endl;
	klogui::okBtn->setText(i18n("Modify"));
}

Qso Klog::getByNumber(const int n){
/***********************************************************
* We receive a number and we try to get the Qso returning *
* a pointer to it                                          *
************************************************************
*/
//cout << "KLog::getByNumber" << endl;
	LogBook::iterator iter;
	for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
		if (n == (*iter).getNumb() )
			return (*iter);
	}
	//Entity not found
	return (*iter);
}


Qso Klog::getByCall(const QString& tqrz){
/***********************************************************
* We receive a call and we try to get the Qso returning *
* a pointer to it                                          *
************************************************************
*/
//cout << "KLog::getByCall" << endl;
	LogBook::iterator iter;
	for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
		if (tqrz.compare(((*iter).getQrz())) == 0)    
			return (*iter);
		}
	//Entity not found
	return (*iter);
}

void Klog::showQso(){
//  This shows the data in the QListView (the botton block)
// The "modify" is still missing
// I have to look for the QSO if modifying
//cout << "KLog::showQso" << endl;
	if (!modify){
		QListViewItem * item = new QListViewItem( klogui::logListView, 0 );
		//item->setText( 0,  QString::number(qso.getNumb()) );
		item->setText( 0, getNumberString(qso.getNumb())  );
	}else{
		QListViewItem *item = klogui::logListView->findItem(QString::number(Klog::j),0);
		if (item){
			//item->setText( 0,  QString::number(Klog::j) );
			item->setText( 0, getNumberString(Klog::j)  ); 
    			item->setText( 1, qso.getDateTime().toString("yyyy-MM-dd") );
			item->setText( 2, qso.getDateTime().toString("hh:mm") );
    			item->setText( 3, qso.getQrz().upper() );
    			item->setText( 4, QString::number(qso.getRsttx()) );
    			item->setText( 5, QString::number(qso.getRstrx()) );
    			item->setText( 6, qso.getBand() );
    			item->setText( 7, qso.getMode() );
    			item->setText( 8, qso.getPower() );
    			item->setText( 9, qso.getComment() );  
  		}
	}
	// showAwardsNumbers();
}

void Klog::showAwardsNumbers(){
/*Hay que revisar la variable "confirmed" que son los QSOs confirmados...
HAy que revisar:
	Cuando se confirma un QSO.
	Cuando se lee el log
	Cuando se borra un QSO
	Cuando se lee el estado de los diplomas
ea4tv TODO: delete

*/
//cout << "KLog::showAwardNumbers" << endl;
	klogui::workedQSOlCDNumber->display(Klog::number);
	klogui::confirmedQSOlCDNumber->display(Klog::confirmed);
	//klogui::confirmedQSOlCDNumber->display(dxcc.howManyConfirmed());
	klogui::DXCCworkedlCDNumber->display(dxcc.howManyWorked());
	klogui::DXCClCDNumber->display(dxcc.howManyConfirmed());
	klogui::WAZworkedlCDNumber->display(waz.howManyWorked());
	klogui::WAZlCDNumber->display(waz.howManyConfirmed());
}

void Klog::clearAwards(){
//cout << "KLog::clearAwards" << endl;
	dxcc.clear();
	waz.clear();
}

bool Klog::didISave(){
//cout << "KLog::didISave" << endl;
	if (needToSave){
		switch( QMessageBox::information( this, i18n("Warning - Save log?"),
                                      i18n("The log has been changed since "
                                      "the last save."),
                                      i18n("Save Now"), i18n("Cancel"), i18n("Continue anyway"),
                                      0, 1 ) ) {
			case 0:
				fileSave();
				return true;
			break;
			case 1:
				default: // just for sanity
				return false;
			break;
			case 2:
				return true;
			break;
		}
	}
	return false;
}


void Klog::fileExit(){
// Check for saving if modified
//cout << "KLog::fileExit" << endl;
	if (hamlib){
		KlogHamlib.close();
	}
	if (needToSave){
		if(!didISave()){ // If i did not save, i cannot exit without saving
			return;
		}
	}else{
	}
//	QFile file( tempLogFile ); 
//	tempLogFile.remove();
	close();
}

void Klog::fileNew(){
//cout << "KLog::fileNew" << endl;
	operatorStringAux = "";
	Klog::number = 0;
	Klog::confirmed = 0;
	klogDir = QDir::homeDirPath()+"./klog";
	logFileNameToSave = "klog.adi";
	slotClearBtn();
	modify = false;
	//  prefixFound = false;
	logbook.clear();
	templogbook.clear();
	clearAwards();
	//ea_tpea.clear();
	klogui::logListView->clear();
	showAwardsNumbers();
  
}




void Klog::fileOpen(){
//cout << "KLog::fileNew" << endl;
	if (needToSave){
		if(!didISave()){ // If i did not save, i cannot exit without saving
			return;
		}
	} 
	QFileDialog fileOpenDialog(this, i18n("Open a file"), TRUE);
	fileOpenDialog.setFilter(i18n("ADIF files (*.adi *.ADI *.adif)"));
	fileOpenDialog.setDir(klogDir);  
	if (fileOpenDialog.exec() == QDialog::Accepted)
		logFileNameToOpen=fileOpenDialog.selectedFile();
	else
		return;
	clearAwards();
	if(!logbook.empty()){ //If we are using a log we have to clear it    
		logbook.clear();
		templogbook.clear();
		klogui::logListView->clear();
	}
	Klog::number = 0;	// We have to reset the counter!
	adifReadLog(logFileNameToOpen);
	logFileNameToSave = logFileNameToOpen;
	needToSave = false;
}




void Klog::slotQslSentBoxChanged(){
//cout << "KLog::SlotQslSentBoxChanged" << endl;
	if (enti == 0) 
		return;
	if (klogui::QSLSentcheckBox->isChecked()){
		if ((!modify) || (!qso.sentTheQSL())){
			qslSen = QDate::currentDate();
		}
		qso.QslSent('Y');
		(klogui::QSLSentdateEdit)->setEnabled(true);
		(klogui::QSLSentdateEdit)->setDate(qslSen);
	}else{
		qso.QslSent('N');
		//TODO: This date is not valid, it is out of range!
		qslSen = QDate::fromString("0000-00-00",Qt::ISODate);
		(klogui::QSLSentdateEdit)->setDate(qslSen);
		(klogui::QSLSentdateEdit)->setEnabled(false);
	}
	if ( (!modify) && (qso.sentTheQSL()) )
		qso.setQslSenDateOn(qslSen);
}


void Klog::slotQslRecvBoxChanged(){
//cout << "KLog::slotQslRecvBoxChanged" << endl;
	wasConfirmed = qso.gotTheQSL(); // Was this QSO previously confirmed 
	if ((enti == 0) | ( (klogui::qrzLineEdit->text()).length() == 0)){
		return;
	}
  // If we are modifying the date should be the previous one if existed
  //if (!modify)
	qslRec = QDate::currentDate();
	if (klogui::QSLReccheckBox->isChecked()){ // The QSO is confirmed
		qso.QslRec('Y');
		(klogui::QSLRecdateEdit)->setDate(qslRec);
		(klogui::QSLRecdateEdit)->setEnabled(true);
		if (!wasConfirmed){
			confirmed++;
		}
		dxcc.confirmedString(enti,(klogui::bandComboBox->currentText()).upper() ,  (klogui::modeComboBox->currentText()).upper());
		waz.confirmedString( world.getCqz() ,qso.getBand(),qso.getMode());
	}else{ // The QSO is NOT confirmed
		qso.QslRec('N');
		(klogui::QSLRecdateEdit)->setEnabled(false);
		//TODO: This date is not valid, it is out of range!
		qslRec = QDate::fromString("0000-00-00",Qt::ISODate);
		(klogui::QSLRecdateEdit)->setDate(qslRec);
		if (!modify){
			(klogui::QSLRecdateEdit)->setDate(qslRec);
			if (dxcc.isConfirmed(enti)){
				dxcc.notConfirmedString(enti, (klogui::bandComboBox->currentText()).upper(),  (klogui::modeComboBox->currentText()).upper());
				waz.notConfirmedString( world.getCqz() ,qso.getBand(),qso.getMode()); 
			}
		}else{ // I am modifying
			if (wasConfirmed){ // If i am "deselecting" a previously confirmed QSO...
				confirmed--;
			}
			if (dxcc.isConfirmed(enti)){
				dxcc.notConfirmedString(enti, (klogui::bandComboBox->currentText()).upper(), (klogui::modeComboBox->currentText()).upper());
				waz.notConfirmedString( world.getCqz() ,qso.getBand(),qso.getMode()); 
			}
		}
	} 
	if ((!modify) && (qso.gotTheQSL())){
		qso.setQslRecDateOn(qslRec);
	}
	readAwardsStatus();
	showAwardsNumbers();
}

void Klog::readQso(){ //Just read the values an fill the qso
//cout << "KLog::readQso" << endl;
	qso.setQrz((klogui::qrzLineEdit->text()).upper());
	// Calculating RST values
	i = klogui::TSendBox->value();
	rsttx = i;
	i = klogui::SSendBox->value() * 10;
	rsttx = rsttx + i;
	i = klogui::RSendBox->value() * 100;
	rsttx = rsttx + i;
	i = klogui::TRecBox->value();
	rstrx = i;
	i = klogui::SRecBox->value() * 10;
	rstrx = rstrx + i;
	i = klogui::RRecBox->value() * 100;
	rstrx = rstrx + i;

	qso.setNumb (number);

	dateTime =   klogui::qsoDateTime->dateTime();    

	qso.setDateTime(dateTime);
	qso.setRstrx(rstrx);
	qso.setRsttx(rsttx);

	band = klogui::bandComboBox->currentItem();
	imode = klogui::modeComboBox->currentItem();
	power = (klogui::powerSpinBox->text()).upper();
	qso.setBand ((klogui::bandComboBox->currentText()).upper());
	qso.setMode((klogui::modeComboBox->currentText()).upper());

	qso.setPower(power);

  // Running the QSL card slots
	if (!modify){
		slotQslRecvBoxChanged();
		slotQslSentBoxChanged();
	}
	slotIOTAChanged();

	if (enti!=0){
		award = awards.getAwardFor(world.getPrefix());
		if (award.getReferenceNumber(klogui::awardsComboBox->currentText()) && ((klogui::awardsComboBox->currentItem())!=0)){
			qso.setLocalAward(klogui::awardsComboBox->currentText());
			qso.setLocalAwardNumber(award.getReferenceNumber(klogui::awardsComboBox->currentText()));
			award.workReference(klogui::awardsComboBox->currentText(), true);

		}

	}

	if ((klogui::remarksTextEdit->text()).length() >0)
		qso.setComment(klogui::remarksTextEdit->text());

	qso.setQslVia(klogui::QSLcomboBox->currentText());

	// Check if the locator is valid
	if (locator.isValidLocator((klogui::locatorLineEdit->text()).upper())){
		qso.setLocator((klogui::locatorLineEdit->text()).upper());
	}
	if (!modify){
		if (locator.isValidLocator((klogui::myLocatorLineEdit->text()).upper())){
			qso.setMyLocator((klogui::myLocatorLineEdit->text()).upper());
		}else if (locator.isValidLocator(getMyLocator())) {
			qso.setMyLocator(getMyLocator());
		}
	}

	if(klogui::qslVialineEdit->isEnabled())
		qso.setQslManager((klogui::qslVialineEdit->text()).upper());

	if ((klogui::QSLInfotextEdit->text()).length() > 0)
		//  if(klogui::QSLInfotextEdit->isEnabled())
		qso.setQslInfo(klogui::QSLInfotextEdit->text());

	if((klogui::namekLineEdit->text()).length() >= 2)
		qso.setName((klogui::namekLineEdit->text()).upper());

	if((klogui::qthkLineEdit->text()).length() >= 2)
		qso.setQth((klogui::qthkLineEdit->text()).upper());

	if((klogui::operatorLineEdit->text()).length() >= 3)
		qso.setOperator((klogui::operatorLineEdit->text()).upper());
	
	if (klogui::freqlCDNumber->value() >= 0)
		qso.setFreq(QString::number(klogui::freqlCDNumber->value()));
}

void Klog::modifyQso(){ // Modify an existing QSO with the data on the boxes
//cout << "KLog::modifyQso" << endl;
  LogBook::iterator iter;

  for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
    if ( Klog::j == (*iter).getNumb() ){

      (*iter).setQrz( (klogui::qrzLineEdit->text()).upper() );

      (*iter).setDateTime(klogui::qsoDateTime->dateTime());
      (*iter).setRstrx(rstrx);
      (*iter).setRsttx(rsttx);
      (*iter).setBand ((klogui::bandComboBox->currentText()).upper());
      (*iter).setMode((klogui::modeComboBox->currentText()).upper());
      (*iter).setPower((klogui::powerSpinBox->text()).upper());
      (*iter).setQth((klogui::qthkLineEdit->text()).upper());
      (*iter).setOperator((klogui::operatorLineEdit->text()).upper()); 

      if ((klogui::remarksTextEdit->text()).length() >0)      
        (*iter).setComment(klogui::remarksTextEdit->text());
      if((klogui::namekLineEdit->text()).length() >= 2)
        (*iter).setName((klogui::namekLineEdit->text()).upper());
      if((klogui::qthkLineEdit->text()).length() >= 2)
        (*iter).setQth((klogui::qthkLineEdit->text()).upper());
      if((klogui::operatorLineEdit->text()).length() >= 3)
        (*iter).setOperator((klogui::operatorLineEdit->text()).upper());

      if ((klogui::iotaIntSpinBox->value() != 0)) // IOTA
        (*iter).setIota(iota);

	if ((klogui::awardsComboBox->currentItem() != 0)){
		award = awards.getAwardFor(world.getPrefix());
		if (award.getReferenceNumber(klogui::awardsComboBox->currentText())){
		(*iter).setLocalAward(klogui::awardsComboBox->currentText());
		(*iter).setLocalAwardNumber(award.getReferenceNumber(klogui::awardsComboBox->currentText()));
		
		//award.workReference(klogui::awardsComboBox->currentText(), true);
		}
	}

      if (locator.isValidLocator((klogui::locatorLineEdit->text()).upper() ))
        (*iter).setLocator( (klogui::locatorLineEdit->text()).upper() );


	if (locator.isValidLocator((klogui::myLocatorLineEdit->text()).upper())){
		(*iter).setMyLocator((klogui::myLocatorLineEdit->text()).upper());
	}else if (locator.isValidLocator(getMyLocator())) {
		(*iter).setMyLocator(getMyLocator());
	}

      if (klogui::QSLSentcheckBox->isChecked()){
        qslSen = klogui::QSLSentdateEdit->date();
        (*iter).setQslSenDateOn(qslSen);
      }else{
        (*iter).QslSent('N');
      }

      if (klogui::QSLReccheckBox->isChecked()){
        qslRec = klogui::QSLRecdateEdit->date();
        (*iter).setQslRecDateOn(qslRec);
        if ((*iter).gotTheQSL()){
//          if (!dxcc.isConfirmed(enti))
            dxcc.confirmedString(enti, (*iter).getBand(), (*iter).getMode());
            waz.confirmedString( world.getCqz() ,(*iter).getBand(),(*iter).getMode());
        }
    }else{
      (*iter).QslRec('N');
      if (dxcc.isConfirmed(enti)){
        dxcc.notConfirmedString(enti, (klogui::bandComboBox->currentText()).upper(), (klogui::modeComboBox->currentText()).upper());
        waz.notConfirmedString( world.getCqz() ,(klogui::bandComboBox->currentText()).upper(), (klogui::modeComboBox->currentText()).upper());
      }
    }
    
    (*iter).setQslVia(klogui::QSLcomboBox->currentText()); //QSL Info
    if(klogui::qslVialineEdit->isEnabled())
      (*iter).setQslManager(klogui::qslVialineEdit->text());
    if ((klogui::QSLInfotextEdit->text()).length() >0)
//    if(klogui::QSLInfotextEdit->isEnabled())
      (*iter).setQslInfo(klogui::QSLInfotextEdit->text());
  }

  }

}


void Klog::helpAbout(){
//cout << "KLog::helpAbout" << endl;
  /*QString description;

  description = "The KDE Ham Radio Logging program";
  KAboutData aboutData( "klog", I18N_NOOP("Klog"), KLogVersion, description, KAboutData::License_GPL, "(c) 2002-2005, Jaime Robles, EA4TV", 0, 0, "jaime@kde.org");
  aboutData.addAuthor("Jaime Robles, EA4TV",0, "jaime@kde.org");
//  KAboutApplication kAbout(this);
  KAboutDialog kAbout(this);

  kAbout.setTitle ("About KLog");
  kAbout.setAuthor("Jaime Robles, EA4TV", "jaime@kde.org", "http:://jaime.robles.nu/klog", "Main Developer");
  kAbout.setVersion ( Klog::KLogVersion);
  kAbout.show(this);
*/


QMessageBox::about( this, i18n("KLog-"+Klog::KLogVersion + " - The KDE Ham Radio Logging program"),
                            i18n("KLog: "+Klog::KLogVersion + " - The KDE Ham Radio Logging program\n"
                            "You can find the last version on http://jaime.robles.nu/klog\nJaime Robles, EA4TV, jaime@robles.nu"));

	//KLog::aboutData->show(this);  
}

void Klog::slotQSLcomboBoxChanged(){    
//cout << "KLog::slotQSLcomboChanged" << endl; 
	QString combo = (klogui::QSLcomboBox)->currentText();

	if (combo.compare("No QSL") == 0){
		klogui::qslVialineEdit->setDisabled(true);
		//    klogui::QSLInfotextEdit->setDisabled(true);
		klogui::qslVialineEdit->clear();
		//    klogui::QSLInfotextEdit->clear();
	}else{
		if (combo.compare("Manager") == 0)
			klogui::qslVialineEdit->setEnabled(true);
		else
			klogui::qslVialineEdit->setDisabled(true);
	//    klogui::QSLInfotextEdit->setEnabled(true);
	}
}

// The next slots run/shows the setup dialog to setup KLog
void Klog::slotPreferences(){
//cout << "KLog::slotPreferences" << endl;
}

void Klog::accept(){
//cout << "KLog::accept" << endl;
}

void Klog::readConf(){
//cout << "KLog::readConf" << endl;
	DXClusterServerToUse ="NOSERVER";
	dxClusterHost="NOSERVER";
	dxClusterPort = -1;
	if (hamlib){
		//KlogHamlib.~ kloghamlib();
		hamlib = false;
		//hamlibtimer->stop();
	}
	
	QFile file( "klogrc" );
	if ( file.open( IO_ReadOnly ) ) {
		QTextStream stream( &file );
		QString adifTab;
		QString theData;
		QString data = "";
		data = ""; //
		while (!stream.atEnd()){
			data = stream.readLine();
			if(!data.isEmpty()){ // If there is another line I read it
				data = data.simplifyWhiteSpace();
				QStringList fields = QStringList::split('=', data );
				adifTab = fields[0].upper();
				theData = fields[1];
				if (adifTab == "CALL"){
					setMyQrz(theData);
				}else if (adifTab == "LOCATOR"){
					setMyLocator(theData.upper());
				}else if (adifTab == "DXCLUSTERSERVER"){
					dxClusterHost = theData.upper();
					if ((dxClusterPort>=1)||(DXClusterServerToUse=="NOSERVER")){
						if (checkIfValidDXCluster((dxClusterHost+":"+QString::number(dxClusterPort)).lower())){
								DXClusterServerToUse = (dxClusterHost+":"+QString::number(dxClusterPort)).lower();	
						}
					}
				}else if (adifTab == "DXCLUSTERPORT"){
					dxClusterPort = theData.toInt();
					if ((dxClusterPort>=1)||(dxClusterHost!="NOSERVER")||(DXClusterServerToUse=="NOSERVER")){
						if (checkIfValidDXCluster((dxClusterHost+":"+QString::number(dxClusterPort)).lower())){
							
							DXClusterServerToUse =  (dxClusterHost+":"+QString::number(dxClusterPort)).lower();
							
						}
					}
				}else if (adifTab == "DXCLUSTERSERVERTOUSE"){
					if ((theData.contains(":"))==0){
						theData = theData+":41112";
					}
					if (checkIfValidDXCluster(theData.lower())){
						DXClusterServerToUse = theData.lower();
					}
				}else if (adifTab == "TIMEUTC"){
					if (theData.upper() == "TRUE"){
						timeInUTC=true;
					}else if ((theData.upper() == "FALSE")){
						timeInUTC=false;
					}
				}else if (adifTab == "DXCLUSTERHFSPOTS"){
					if (theData.upper() == "TRUE"){
						dxClusterHFSpots=true;
					}else if ((theData.upper() == "FALSE")){
						dxClusterHFSpots=false;
					}
				}else if (adifTab == "DXCLUSTERVHFSPOTS"){
					if (theData.upper() == "TRUE"){
						dxClusterVHFSpots=true;
					}else if ((theData.upper() == "FALSE")){
						dxClusterVHFSpots=false;
					}
				}else if (adifTab == "DXCLUSTERWARCSPOTS"){
					if (theData.upper() == "TRUE"){
						dxClusterWARCSpots=true;
					}else if ((theData.upper() == "FALSE")){
						dxClusterWARCSpots=false;
					}
				}else if (adifTab == "DXCLUSTERCONFIRMEDANNOUNCES"){
					if (theData.upper() == "TRUE"){
						dxClusterConfirmedSpots=true;
					}else if ((theData.upper() == "FALSE")){
						dxClusterConfirmedSpots=false;
					}
				}else if (adifTab == "DXCLUSTERANNOUNCES"){
					if (theData.upper() == "TRUE"){
						dxClusterANNounces=true;
					}else if ((theData.upper() == "FALSE")){
						dxClusterANNounces=false;
					}
				}else if (adifTab == "DXCLUSTERWXANNOUNCES"){
					if (theData.upper() == "TRUE"){
						dxClusterWXANNounces=true;
					}else if ((theData.upper() == "FALSE")){
						dxClusterWXANNounces=false;
					}
				}else if (adifTab == "DXCLUSTERWCYANNOUNCES"){
					if (theData.upper() == "TRUE"){
						dxClusterWCYANNounces=true;
					}else if ((theData.upper() == "FALSE")){
						dxClusterWCYANNounces=false;
					}
				}else if (adifTab == "REALTIMELOG"){
					if (theData.upper() == "TRUE"){
						realTimeLog = true;
					}else if ((theData.upper() == "FALSE")){
						realTimeLog = false;
					}
				}else if (adifTab == "DEFAULTBAND"){
					band =  theData.toInt();
				}else if (adifTab == "DEFAULTMODE"){
					imode =  theData.toInt();
				}else if (adifTab == "CONFIRMEDCOLOR"){
					confirmedColor.setNamedColor(theData);
				}else if (adifTab == "WORKEDCOLOR"){
					workedColor.setNamedColor(theData.upper());
				}else if (adifTab == "NEEDEDCOLOR"){
					neededColor.setNamedColor(theData.upper());
				}else if (adifTab == "DEFAULTCOLOR"){
					defaultColor.setNamedColor(theData.upper());
				}else if (adifTab == "POWER"){
					power = theData.upper();  
				}else if (adifTab == "OPENLASTFILE"){
					if (theData.upper() == "TRUE"){
						openLastByDefault = true;
					}else if ((theData.upper() == "FALSE")){
						openLastByDefault = false;
					}
				}else if (adifTab == "HAMLIB"){
					if (theData.upper() == "TRUE"){
						hamlib = true;
						hamlibReading = true;
					}else if ((theData.upper() == "FALSE")){
						hamlib = false;
						hamlibReading = false;
					}
				}else if (adifTab == "RIGNAME"){
					rignameNumber = (theData.upper()).toInt();
					KlogHamlib.setRigNumber(rignameNumber);
				}else if (adifTab == "HAMLIBINTERVAL"){
					hamlibInterval = (theData.upper()).toInt();
				}else if (adifTab == "BAUDS"){
					bauds = (theData.upper()).toInt();
					KlogHamlib.setBauds(bauds);
				}else if (adifTab == "PORT"){
					serialPort = theData;
					KlogHamlib.setSerialPort(serialPort);
				}else if (adifTab == "COMPLETEWITHPREVIOUS"){
					if (theData.upper() == "TRUE"){
						completeWithPrevious = true;
					}else if ((theData.upper() == "FALSE")){
						completeWithPrevious = false;
					}
				}else if (adifTab == "REQUIREMANDATORY"){
					if (theData.upper() == "TRUE"){
						requireMandatory = true;
					}else if ((theData.upper() == "FALSE")){
						requireMandatory = false;
					}

				}else if (adifTab == "LOGFILENAME"){
					logFileNameToOpen = theData;
					//logFileNameToSave = theData;	//We are just reading the file to Open
				}else if (adifTab == "SHOWPROGRESSDIALOG"){
					if (theData.upper() == "TRUE"){
						showProgressDialog = true;
					}else if ((theData.upper() == "FALSE")){
						showProgressDialog = false;
					}
				}else{
				}
			}//Closes the if != empty
		}// Closes the while
		file.close();
		if (hamlib){
			hamlibPossible = KlogHamlib.init();
			if ((hamlibPossible) && (hamlib)){
			}
		}else{
		}
	}else{
		slotKlogSetup();
		// the file klogrc with preferences does not exist so we have to create it
	}
	if ((openLastByDefault)&&(logFileNameToOpen !="")){	// Check if the user wants to work on a default logfile.
		logFileNameToSave = logFileNameToOpen;
	}
	awards.readConfig();	// I have to read the awards as part of the config!
}

void Klog::setMyQrz(const QString &tqrz){
//cout << "KLog::setMyQrz" << endl;
	myQrz = tqrz;
}

QString Klog::getMyQrz() const{
//cout << "KLog::getMyQrz" << endl;
	return myQrz;
}

void Klog::setMyLocator(const QString &tlocator){
//cout << "KLog::setMyLocator" << endl;
	if (locator.isValidLocator(tlocator.upper() ))
		myLocator = tlocator;
}

QString Klog::getMyLocator() const{
//cout << "KLog::getMyLocator" << endl;
	return myLocator;
}

void Klog::showWhere(const int enti){
//cout << "KLog::showWhere: " << QString::number(enti) << endl;
	if ((enti != 0)&&(enti != -1)){
		klogui::entityTextLabel->setText((world.getEntByNumb(enti)).getEntity());
		klogui::prxTextLabel->setText((world.getEntByNumb(enti)).getPfx());
		klogui::continentTextLabel->setText((world.getEntByNumb(enti)).getContinent());

		klogui::cqLCDNumber->display(world.getCqz());
		klogui::ituLCDNumber->display(world.getItuz());

		// If the QSO has a locator and it is valid, calculation is more exact!
		if (locator.isValidLocator((klogui::locatorLineEdit->text()).upper())){
		// The following code is copy&pasted from "slotLocatorChanged"
			dxLocator = (klogui::locatorLineEdit->text()).upper();

			Klog::distance = locator.getDistanceKilometres(locator.getLon(qso.getMyLocator()), locator.getLat(qso.getMyLocator()), locator.getLon(dxLocator), locator.getLat(dxLocator));
			beam = locator.getBeam(locator.getLon(qso.getMyLocator()), locator.getLat(qso.getMyLocator()), locator.getLon(dxLocator), locator.getLat(dxLocator));
		}else{
			Klog::distance = locator.getDistanceKilometres(locator.getLon(qso.getMyLocator()), locator.getLat(qso.getMyLocator()), (world.getEntByNumb(enti)).getLon(), (world.getEntByNumb(enti)).getLat());
			beam = locator.getBeam(locator.getLon(qso.getMyLocator()), locator.getLat(qso.getMyLocator()), (world.getEntByNumb(enti)).getLon(), (world.getEntByNumb(enti)).getLat());


		}
		showDistancesAndBeam(distance, beam);
		entityState(enti);
		fillEntityBandState(enti);

	}else{ // This is what happens if we do not know the Entity

		clearEntityBox();
	}
}

void Klog::fillEntityBandState(const int enti){
//  bandsDefault = "10-12-15-17-20-30-40-80-160-2m-70cm";  
//cout << "KLog::fillEntityBandState" << endl;
    if(dxcc.isConfirmedBand(enti, adif.band2Int("2M"))){ // 2m band
      // RED for confirmed
      klogui::textLabelBand2->setPaletteBackgroundColor(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("2M"))){
        // Yellow for worked but not confirmed
        klogui::textLabelBand2->setPaletteBackgroundColor(workedColor);
      }else{
        //GREEN if new one
        klogui::textLabelBand2->setPaletteBackgroundColor(neededColor);
      }
    }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("6M"))){ // 6m band
      // RED for confirmed
      klogui::textLabelBand6->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("6M"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand6->setPaletteBackgroundColor(workedColor);
    }else{
      //GREEN if new one
      klogui::textLabelBand6->setPaletteBackgroundColor(neededColor);
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("10M"))){ // 10m band
    // RED for confirmed
    klogui::textLabelBand10->setPaletteBackgroundColor(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("10M"))){
        // Yellow for worked but not confirmed
        klogui::textLabelBand10->setPaletteBackgroundColor(workedColor);
      }else{
        klogui::textLabelBand10->setPaletteBackgroundColor(neededColor);
        //GREEN if new one
      }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("12M"))){ // 12m band
      // RED for confirmed
      klogui::textLabelBand12->setPaletteBackgroundColor(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("12M"))){
        // Yellow for worked but not confirmed
        klogui::textLabelBand12->setPaletteBackgroundColor(workedColor);
      }else{
        klogui::textLabelBand12->setPaletteBackgroundColor(neededColor);
        //GREEN if new one
      }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("15M"))){ // 15m band
    // RED for confirmed
    klogui::textLabelBand15->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("15M"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand15->setPaletteBackgroundColor(workedColor);
    }else{
      klogui::textLabelBand15->setPaletteBackgroundColor(neededColor);
        //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("17M"))){ // 17m band
    // RED for confirmed
      klogui::textLabelBand17->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("17M"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand17->setPaletteBackgroundColor(workedColor);
    }else{
      klogui::textLabelBand17->setPaletteBackgroundColor(neededColor);
      //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("20M"))){ // 20m band
    // RED for confirmed
    klogui::textLabelBand20->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("20M"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand20->setPaletteBackgroundColor(workedColor);
    }else{
      klogui::textLabelBand20->setPaletteBackgroundColor(neededColor);
      //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("30M"))){ // 30m band
    // RED for confirmed
    klogui::textLabelBand30->setPaletteBackgroundColor(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("30M"))){
      // Yellow for worked but not confirmed
        klogui::textLabelBand30->setPaletteBackgroundColor(workedColor);
      }else{
        klogui::textLabelBand30->setPaletteBackgroundColor(neededColor);
        //GREEN if new one
      }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("40M"))){ // 40m band
      // RED for confirmed
    klogui::textLabelBand40->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("40M"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand40->setPaletteBackgroundColor(workedColor);
    }else{
      klogui::textLabelBand40->setPaletteBackgroundColor(neededColor);
        //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("80M"))){ // 80m band
    // RED for confirmed
    klogui::textLabelBand80->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("80M"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand80->setPaletteBackgroundColor(workedColor);
    }else{
      klogui::textLabelBand80->setPaletteBackgroundColor(neededColor);
        //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("70CM"))){ // 70CM band
    // RED for confirmed
    klogui::textLabelBand70cm->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("70CM"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand70cm->setPaletteBackgroundColor(workedColor);
    }else{
      klogui::textLabelBand70cm->setPaletteBackgroundColor(neededColor);
      //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("160M"))){ // 160m band
    // RED for confirmed
    klogui::textLabelBand160->setPaletteBackgroundColor(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("160M"))){
      // Yellow for worked but not confirmed
      klogui::textLabelBand160->setPaletteBackgroundColor(workedColor);
    }else{
      klogui::textLabelBand160->setPaletteBackgroundColor(neededColor);
      //GREEN if new one
    }
  }  
}


void Klog::tlfReadLog(const QString& tfileName){
//TODO: It is needed to improve the eficiency
// It is VEEEEEEEEERY slow if the log is big/long
//cout << "KLog::tlfReadLog" << endl;
	int totalQsos = 0; // QSOs in the log to be read
	int progresStep = 0;
	QProgressDialog progress( i18n("Reading the log..."), i18n("Abort reading"), 0,
                          this, i18n("progress"), TRUE );
	QString progressLabel;


	bool ok;
	QString text = QInputDialog::getText(
            i18n("KLog - TLF Import"), i18n("Enter a remark for ALL the imported QSO:\n(Leave it empty and press OK if no remark)"), QLineEdit::Normal,
            QString::null, &ok, this );
	if ( ok && !text.isEmpty() ) {
	// user entered something and pressed OK
	}else if (!ok){ // The user clicked CANCEL
		return;
	}else{
	// user entered nothing but clicked over OK
	}
	aux ="NULL";  // aux is used for the call used in the contest
	//TODO: It is necessary to add a check to know if the call used in the contest is valid.
	QString aux = QInputDialog::getText(i18n("KLog - TLF Import"), 
     i18n("Enter the used call:\n(Leave it empty and press OK if no diferent call was used)"), QLineEdit::Normal,
            QString::null, &ok, this );
	if ( ok && !text.isEmpty() ) {
		if (aux.length()<=2){
			switch( QMessageBox::information( this, i18n("Warning - QRZ not valid"),
                                      i18n("Do you want to import without a QRZ?\n"),
                                        i18n("Yes"), i18n("No"), 0, 1 ) ) {
				case 0:// The user does not want to continue without a call
					aux ="NULL";
				break;
				case 1: // The user clicked over YES so import
					return;
				break;
				default: // 
					return;
				break; 
			}
		}
	}else if (!ok){ // The user pressed Cancel
		return;
	}else{
	}
	
	QString data;
	QFile file( tfileName );
	QTextStream stream( &file );
	if (showProgressDialog){
		int totalQsos = 0;
		if ( file.open( IO_ReadOnly ) ) { // We are going to count the number of QSOs of the file
			while (!stream.atEnd()){
				data = stream.readLine();
				if (data){
					if(!data.isEmpty() && (!data.startsWith(";"))  ){
						totalQsos = totalQsos + 1;
   						//getProgresStepForDialog
					}
				}
			}
		}
	}
	file.close();
	if ( file.open( IO_ReadOnly ) ) {
		QString qband;
		QString qmode;
		QString qdate;
		QString qtime;
		QString qstx;
		QString qqrz;
		QString qrsttx;
		QString qrstrx;
		QString qstx_string;
		QString qsrx_string;
		QString qauxString;
		QString otherAux;      // NO YEAR
		QString monthString;
		QString dayString;
		QString dateString = "";
		QString timeString = "";
		int len; // for the tab length
		data = ""; 
		while (!stream.atEnd()){
			data = stream.readLine();
			if (data){
				if((!data.isEmpty()) && (!data.startsWith(";")) ){
					data = data.simplifyWhiteSpace().upper();
					QStringList fields = QStringList::split(' ', data );
					qauxString = fields[0];
					qdate = fields[1];
					QStringList datelist = QStringList::split( '-', qdate );
					otherAux = datelist[2];
					monthString = datelist[1];
					dayString = datelist[0];
					//TODO: Fix this hack
					//HACK: It is not a good solution as it is only valid until 2009
					if ( (otherAux.toInt())>9){ //NOT THE BEST SOLUTION
						otherAux = "19" + otherAux;
					}else{
						otherAux = "20" + otherAux;// only valid till 2009!!!
					}
					if (monthString == "JAN"){
						dateString = otherAux+"-"+"01-"+dayString;
					}else if (monthString == "FEB"){
						dateString = otherAux+"-"+"02-"+dayString;
					}else if (monthString == "MAR"){
						dateString = otherAux+"-"+"03-"+dayString;
					}else if (monthString == "APR"){
						dateString = otherAux+"-"+"04-"+dayString;
					}else if (monthString == "MAY"){
						dateString = otherAux+"-"+"05-"+dayString;
					}else if (monthString == "JUN"){
						dateString = otherAux+"-"+"06-"+dayString;
					}else if (monthString == "JUL"){
						dateString = otherAux+"-"+"07-"+dayString;
					}else if (monthString == "AGO"){
						dateString = otherAux+"-"+"08-"+dayString;
					}else if (monthString == "AUG"){
						dateString = otherAux+"-"+"08-"+dayString;   
					}else if (monthString == "SEP"){
						dateString = otherAux+"-"+"09-"+dayString;
					}else if (monthString == "OCT"){
						dateString = otherAux+"-"+"10-"+dayString;
					}else if (monthString == "NOV"){
						dateString = otherAux+"-"+"11-"+dayString;
					}else if (monthString == "DEC"){
						dateString = otherAux+"-"+"12-"+dayString;
					}else{
					}
					qtime = fields[2];
					qstx_string = fields[3];
					qqrz = fields[4];
					qrsttx = fields[5];
					qrstrx = fields[6];
					qsrx_string = fields[7];
					dateString = dateString + "T" + qtime + ":00";
					len = 0;
					while( (qauxString.at(len)).isDigit()){ //Find how many numbers are in the first block of characters (20SSB, 160CW, ...)
						len++;
					}
					qband = qauxString.left(len)+"M";
					qmode = qauxString.right(qauxString.length()-len);
					if (aux != "NULL"){
						qso.setOperator(aux);
					}
					qso.setQrz(qqrz.upper());
					Klog::number++;
					qso.setNumb(Klog::number);
					qso.setDateTime(QDateTime::fromString(dateString, Qt::ISODate));
					qso.setRstrx(qrstrx.toInt());
					qso.setRsttx(qrsttx.toInt());
					qso.setBand(qband.upper());
					qso.setMode(qmode.upper());
					qso.setPower("0"); // No power known
					// qauxString = "";
					if ( ok && !text.isEmpty() ){ // We read the comment at the begining
						qauxString = text; // Now we want to save a comment and the contest exchange for all QSO.
						if ((fields[3].length()>0)&&(fields[7].length()>0)){
								qauxString = qauxString + " --  "+fields[3]+"/"+fields[7]; // We are saving the contest exchange.     
							qso.setComment(qauxString);
						}
					}else{
						if ((fields[3].length()>0)&&(fields[7].length()>0)){
							qauxString = fields[3]+"/"+fields[7]; // We are saving the contest exchange
						}
						qso.setComment(qauxString);
					}
 					/*
						if ( !logbook.empty() ) {
						// OK, modify the first item
						logbook.last();
					}
					*/
					qauxString = "";
					addQSOToLog();
					if (showProgressDialog){
						if ( (number % getProgresStepForDialog(totalQsos) )== 0){ // To update the speed i will only show the progress once each 25 QSOs
							progresStep = number;
							progressLabel = i18n("Adding QSO#: ") + QString::number(number) + " / " + i18n(QString::number(totalQsos));
							progress.setLabelText(progressLabel);
							progress.setProgress(progresStep );
							qApp->processEvents();
						}
						if ( progress.wasCanceled()){
							return;
						}
					}
 				}
			}
		}
	}
	file.close();
	showAwardsNumbers();
}

void Klog::slotAddLog(){
//cout << "KLog::slotAddLog" << endl;
	QString fileName;
	bool ok;
	textStringAux = "";
	textStringAux = QInputDialog::getText(i18n("KLog - Log Add"), i18n("Enter a remark for ALL the imported QSO:\n(Leave it empty and press OK if no remark)"), QLineEdit::Normal, QString::null, &ok, this );
	if ( ok && !textStringAux.isEmpty() ) {
        	// user entered something and pressed OK
	}else if (!ok){ // The user clicked CANCEL
		return;
	}else{
		// user entered nothing but clicked over OK
	}
	operatorStringAux ="";
	operatorStringAux = QInputDialog::getText(i18n("KLog - Log Add"), i18n("Enter the used call:\n(Leave it empty and press OK if no diferent call was used)"), QLineEdit::Normal, QString::null, &ok, this );
	if ( ok && !textStringAux.isEmpty() ) {
		if (operatorStringAux.length()<=2){
			switch( QMessageBox::information( this, i18n("Warning - QRZ not valid"), i18n("Do you want to import without a QRZ?\n"), i18n("Yes"), i18n("No"), 0, 1 ) ) {
				case 0:// The user does not want to continue without a call
					operatorStringAux ="";
				break;
				case 1: // The user clicked over YES so import
					return;
				break;
				default: //
					return;
				break;
 
			}
		}
	}else if (!ok){ // The user pressed Cancel
		return;
	}else{}
	QFileDialog fileOpenDialog(this, i18n("Open a file"), TRUE);
	fileOpenDialog.setFilter(i18n("ADIF files (*.adi *.ADI *.adif)"));
	fileOpenDialog.setDir(klogDir);
	if (fileOpenDialog.exec() == QDialog::Accepted)
		fileName=fileOpenDialog.selectedFile();
	else
		return;
	adifReadLog(fileName);
	needToSave = true;
	QMessageBox::about( this, i18n("KLog message:"),
                            i18n("The log file has been\n"
                            "successfully added!"));
	textStringAux = "";
 	operatorStringAux ="";
}

void Klog::slotImportTlf(){
//cout << "KLog::slotImportTlf" << endl;
	QString fileName;
	QFileDialog fileOpenDialog(this, i18n("Open a file"), TRUE);
	fileOpenDialog.setFilter(i18n("TLF files (*.log *.LOG)"));
	fileOpenDialog.setDir(klogDir);
	if (fileOpenDialog.exec() == QDialog::Accepted)
		fileName=fileOpenDialog.selectedFile();
	else
		return;
	tlfReadLog(fileName);
}

void Klog::slotIOTAChanged(){
//cout << "KLog::slotIOTAChanged value: " << QString::number(klogui::iotaIntSpinBox->value()) << endl;
	//klogui::iotaIntSpinBox->setEnabled(true);
	
	if ((klogui::iotaIntSpinBox->value() != 0)){ //
		iota = klogui::iotaComboBox->currentText() + "-" + QString::number(klogui::iotaIntSpinBox->value());
		if (!modify)
			qso.setIota(iota);
	}
}

void Klog::entityState(const int tentity){
// Prints the LED and message
//cout << "KLog::entityState" << endl;
	int i = tentity;
	if (i == -1)
		return;

	if (dxcc.isConfirmed(i) ){
		if(dxcc.isConfirmedBand(i, adif.band2Int((klogui::bandComboBox->currentText()).upper()))){
    		// CONFIRMED IN THIS BAND... NO WORK NEEDED
    		// LED = RED
			klogui::LedtextLabel->setText(i18n( "<p align=\"center\"><b>Worked & confirmed before</b></p>"));
			klogui::newEntitykLed->setColor(confirmedColor );

		}else{
			// CONFIRMED BUT NOT IN THIS BAND SO WORK IT!
			if (dxcc.isWorkedBand(i, adif.band2Int((klogui::bandComboBox->currentText()).upper()))){
	// IT IS WORKED IN THIS BAND BUT STILL NOT CONFIRMED, WORK IT!
    // LED = YELLOW
				klogui::LedtextLabel->setText(i18n( "<p align=\"center\"><b>Not confirmed, WORK IT!</b></p>"));
				klogui::newEntitykLed->setColor(workedColor );
			}else{
     // IT IS NOT WORKED IN THIS BAND SO WORK IT!
     // LED = GREEN
				klogui::LedtextLabel->setText(i18n( "<p align=\"center\"><b>New one in this band, WORK IT!</b></p>"));
				klogui::newEntitykLed->setColor(neededColor );
			}
		}
	}else{
	// IT IS NOT CONFIRMED SO WORK IT!
		if (dxcc.isWorked(i)){
    	// IT IS WORKED.... BUT NOT CONFIRMED WORK IT!
			if (dxcc.isWorkedBand(i, adif.band2Int((klogui::bandComboBox->currentText()).upper()))){
    // IT IS WORKED IN THIS BAND BUT STILL NOT CONFIRMED, WORK IT!
    // LED = YELLOW
				klogui::LedtextLabel->setText(i18n( "<p align=\"center\"><b>Not confirmed, WORK IT!</b></p>"));
				klogui::newEntitykLed->setColor(workedColor );
			}else{
    // IT IS NOT WORKED IN THIS BAND, SO WORK IT!
    // LED = GREEN
				klogui::newEntitykLed->setColor(neededColor );
			}
		} else{
    // IT IS A COMPLETE NEW ONE WORK IT AT ANY PRICE!!!!
    // LED = GREEN
			klogui::LedtextLabel->setText(i18n( "<p align=\"center\"><b>NEW ONE, WORK IT!</b></p>"));
			klogui::newEntitykLed->setColor(neededColor );
		}
	}
//    showAwardsNumbers();
//    klogui::workedQSOlCDNumber->display(number);
//    klogui::confirmedQSOlCDNumber->display(confirmed);
}

void Klog::slotSearchButton(){
//cout << "KLog::slotSearchButton" << endl;

	if (searching2QSL){
		slotSearchQSO2QSL();
	}else{
	qrz = (klogui::searchQrzkLineEdit->text()).upper();
	klogui::searchQsoskListView->clear();  
	if (qrz.length()<2){ // This is a hack to avoid slow searches for every previous QSO that
                       // contains one specific letter (nearly every QSO has just one letter!!)
		klogui::previousQthkLineEdit->clear(); 
		klogui::previousNamekLineEdit->clear();
		return;
	}

	LogBook::iterator iter;
	for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
		if (((*iter).getQrz()).contains(qrz) > 0){
			i++;
			switch(needToWorkFromCluster((*iter).getQrz(),adif.band2Int((*iter).getBand()))){
			// 0 confirmed, 1 worked, 2 new one, -1 error
				case 0: // Confirmed
					colorInUse = confirmedColor;
					break;
				case 1: // Worked
					colorInUse = workedColor;
					break;
				case 2: // New One
					colorInUse = neededColor;
					break;
				case -1: // ERROR
					colorInUse = defaultColor;
					break;
				default:
					colorInUse = defaultColor;
					break;
			}
			searchBoxItem * item = new searchBoxItem(klogui::searchQsoskListView, (*iter).getQrz(), (*iter).getDateTime().toString("yyyy-MM-dd"),(*iter).getDateTime().toString("hh:mm"), (*iter).getBand(), (*iter).getMode(), (*iter).isQslSent(), (*iter).isQslRec(), getNumberString((*iter).getNumb()), colorInUse );
			colorInUse = defaultColor;
 
			if ( ((*iter).getName()).length() > 1){
				klogui::previousNamekLineEdit->setText((*iter).getName());
			}else{
				klogui::previousNamekLineEdit->clear();
			}
			if ( ((*iter).getQth()).length() > 1){
				klogui::previousQthkLineEdit->setText((*iter).getQth());
			}else{
				klogui::previousQthkLineEdit->clear();
			}
		}
	}
	enti = world.findEntity(qrz);
	if (enti!=0){
		entityState(enti);
	}
	}
 
}

void Klog::slotSearchQSO2QSL(){
//cout << "KLog::searchQSO2QSL" << endl;
//TODO: Maybe I should add a button for this action
	
	searching2QSL = true;
	klogui::searchQsoskListView->clear();

	LogBook::iterator iter;
	for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
		if (!((*iter).sentTheQSL())&&(needToWorkFromCluster((*iter).getQrz(),adif.band2Int((*iter).getBand()))!=0)){  // We do not need to QSL a QSO with a confirmed entity.
			i++;
			switch(needToWorkFromCluster((*iter).getQrz(),adif.band2Int((*iter).getBand()))){
			// 0 confirmed, 1 worked, 2 new one, -1 error
				case 0: // Confirmed
					colorInUse = confirmedColor;
				break;
				case 1: // Worked
					colorInUse = workedColor;
				break;
				case 2: // New One
					colorInUse = neededColor;
				break;
				case -1: // ERROR
					colorInUse = defaultColor;
				break;
				default:
					colorInUse = defaultColor;
				break;
			}
			searchBoxItem * item = new searchBoxItem(klogui::searchQsoskListView, (*iter).getQrz(), (*iter).getDateTime().toString("yyyy-MM-dd"),(*iter).getDateTime().toString("hh:mm"), (*iter).getBand(), (*iter).getMode(), (*iter).isQslSent(), (*iter).isQslRec(), getNumberString((*iter).getNumb()), colorInUse );
			colorInUse = defaultColor;
			if ( ((*iter).getName()).length() > 1){
				klogui::previousNamekLineEdit->setText((*iter).getName());
			}else{
				klogui::previousNamekLineEdit->clear();
			}
			if ( ((*iter).getQth()).length() > 1){
				klogui::previousQthkLineEdit->setText((*iter).getQth());
			}else{
				klogui::previousQthkLineEdit->clear();
			}
		}//
	}
	enti = world.findEntity(qrz);
	if (enti!=0){
		entityState(enti);
	}
	klogui::outputTab->setCurrentPage(2);
}

void  Klog::slotCancelSearchButton(){
//cout << "KLog::slotCancelSearchButton" << endl;
	//If we are searching QSO to send the QSL we simply call to that slot again :-)
		klogui::searchQrzkLineEdit->clear();
		klogui::searchQsoskListView->clear();
		searching2QSL = false;
}


// The following is to select a QSO from the search box
void Klog::slotQsoSearchSelectedForEdit( QListViewItem * item){
//cout << "KLog::slotQsoSearchSelectedForEdit" << endl;
	if (item){
		slotClearBtn();
		hamlibReading = false;      
		qso = getByNumber((item->text(7)).toInt());
		toEditQso();
		klogui::okBtn->setText(i18n("Modify"));
		klogui::clearBtn->setText(i18n("Cancel"));
		// The following is to modify the number to allow to modify the qso selected
		Klog::j = qso.getNumb(); // j is the QSO number
		Klog::modify = true;
		//    Klog::prefixFound = false;
		Klog::callFound = false;
		Klog::entiBak = 0;
		Klog::callLen = 0;
		Klog::callLenPrev = 0;
		Klog::lastDelete = false;
	}
}

// We are going to delete a QSO from the log
void Klog::slotQsoDelete(){
//cout << "KLog::slotQsoDelete: " << endl;
	//j = qsoToDelete;
	if ((!modify) && (Klog::j == 0)){
		return;
	}else{
		LogBook::iterator iter;
		for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
			if ( j == (*iter).getNumb()){
				switch( QMessageBox::information( this, i18n("Warning - QSO Deletion"),
                                      i18n("Do you want to delete the QSO with:\n"
                                      + (*iter).getQrz() + " of " + (*iter).getDateTime          ().toString("yyyy-MM-dd") + "?"),
                                        i18n("Yes"), i18n("No"), 0, 1 ) ) {
					case 0:

						dxcc.notWorked(world.findEntity((*iter).getQrz().upper()), adif.band2Int((*iter).getBand()), adif.mode2Int((*iter).getMode()));

						waz.notWorked(world.getCqzFromCall((*iter).getQrz().upper()), adif.band2Int((*iter).getBand()), adif.mode2Int((*iter).getMode()));
 						if ((*iter).gotTheQSL()){
 							Klog::confirmed--;  //To decrease the showed number
 						}
						logbook.erase(iter);
						Klog::number--;  //To decrease the showed number

						slotClearBtn();
						readAwardsStatus();
						showLogList();
						showAwardsNumbers();
      						return;
					case 1:
					default: // just for sanity
						return;
					break;
					case 2:
					break;
				}
			}
		}
	}
}

void Klog::readAwardsStatus(){
// Re-read the DXCC and WAZ status. Maybe I could extract to another function...
		LogBook::iterator ite;
		for ( ite = logbook.begin(); ite != logbook.end(); ++ite ){
			dxcc.worked(world.findEntity((*ite).getQrz().upper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
			if ((*ite).gotTheQSL()){
				dxcc.confirmed(world.findEntity((*ite).getQrz().upper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
			}

			waz.worked(world.getCqzFromCall((*ite).getQrz().upper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
			if ((*ite).gotTheQSL()){
				waz.confirmed(world.getCqzFromCall((*ite).getQrz().upper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
			}
		}
}

// To print the whole log in the botton box
void Klog::showLogList(){
//cout << "KLog::showLogList" << endl;
	klogui::logListView->clear();
	Klog::LogBook::iterator it;
	//TODO: CALLS COULD BE IN COLORS TO SHOW IF WORKED/NEEDED, ...
	// re-implementation using paintcell as in cluster is needed to do so
	for ( it = logbook.begin(); it != logbook.end(); ++it ){
		QListViewItem * item = new QListViewItem( klogui::logListView, 0 );
		// item->setText( 0, QString::number((*it).getNumb()) );
		item->setText( 0, getNumberString((*it).getNumb())  );
		item->setText( 1, (*it).getDateTime     ().toString("yyyy-MM-dd") );
		item->setText( 2, (*it).getDateTime().toString("hh:mm") );
		item->setText( 3, (*it).getQrz().upper() );
		item->setText( 4, QString::number((*it).getRsttx()) );
		item->setText( 5, QString::number((*it).getRstrx()) );
		item->setText( 6, (*it).getBand() );
		item->setText( 7, (*it).getMode() );
		item->setText( 8, (*it).getPower() );
		item->setText( 9, (*it).getComment() );
	}
}

QString Klog::getNumberString(const int intNumber){
/* This is to pass from an integer to an "always-the-same-length" string the number
   before showing it in the logListView in order to be able to sort it.
*/
//cout << "KLog::getNumberString" << endl;

	if (intNumber < 10){
		//cout << "Number: " << "     0" + QString::number(intNumber) << endl;
		return "000000" + QString::number(intNumber);
	}else if (intNumber < 100){
		//cout << "Number: " << "00000" + QString::number(intNumber) << endl;
		return "00000" + QString::number(intNumber);
	}else if (intNumber < 1000){
		//cout << "Number: " << "    " + QString::number(intNumber) << endl;
		return "0000" + QString::number(intNumber);
	}else if (intNumber < 10000){
		//cout << "Number: " << "   " + QString::number(intNumber) << endl;
		return "000" + QString::number(intNumber);
	}else if (intNumber < 100000){
		//cout << "Number: " << "  " + QString::number(intNumber) << endl;
		return "00" + QString::number(intNumber);
	}else if (intNumber < 1000000){
		//cout << "Number: " << " " + QString::number(intNumber) << endl;
		return "0" + QString::number(intNumber);
	}else if (intNumber < 10000000){
		//cout << "Number: " << QString::number(intNumber) << endl;
		return QString::number(intNumber);
	}else{
		//cout << "Number2: " << QString::number(intNumber) << endl;
		return QString::number(intNumber);
	}
}

void Klog::slotQSLRec(){
//cout << "KLog::slotQSLRec" << endl;
	wasConfirmed = qso.gotTheQSL(); // Was this QSO previously confirmed 
	if (!wasConfirmed){
		confirmed++; // checked
		wasConfirmed = true;
	}
  if ((!qso.gotTheQSL()) || (modify) ){
    Klog::j = qso.getNumb();
    qslRec = QDate::currentDate();
    LogBook::iterator iter;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
      if ( Klog::j == (*iter).getNumb() ){
        (*iter).setQslRecDateOn(qslRec);
	needToSave = true;
        dxcc.confirmedString(kk, (qso.getBand()).upper() ,  (qso.getMode()));

        waz.confirmedString( (world.getEntByNumb(kk)).getCqz() ,qso.getBand(),qso.getMode());
        showLogList();
        showWhere(kk);
        entityState(kk);
        showAwardsNumbers();
      }
    }
  }
}


void Klog::slotQSLSent(){
//We have sent the QSL
//cout << "KLog::slotQSLSent" << endl;
	if (!qso.sentTheQSL()){
		Klog::j = qso.getNumb();
		qslSen = QDate::currentDate();
		LogBook::iterator iter;
		for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
			if ( Klog::j == (*iter).getNumb() ){
				(*iter).setQslSenDateOn(qslSen);
				needToSave = true;
				//        showLogList(); // i commented it to make the qsl send right button easier
				showWhere(kk);
				entityState(kk);
			}
		}
	}
}

void Klog::slotQSLRecSent(){
//We have sent and received the QSL	
//cout << "KLog::slotQSLRecSent" << endl;
	slotQSLRec();
	slotQSLSent();
}

void Klog::slotUpdateTime(){
//cout << "KLog::slotUpdateTime" << endl; 
	if (timeInUTC){
		dateTimeContinuous = QDateTime::currentDateTime(Qt::UTC);
	}else{ // I am not in realTime
		dateTimeContinuous = QDateTime::currentDateTime(Qt::LocalTime);
	}

	if ( (!modify) && (realTimeLog) ){
		klogui::qsoDateTime->setAutoAdvance (true);
		dateTime = dateTimeContinuous;
		klogui::qsoDateTime->setDateTime(dateTime);
	}else{
		klogui::qsoDateTime->setAutoAdvance (false);
	}
	//if ((realTimeLog) && (modify)){
		// This checking is just a test... maybe it is not necesary to update
		//    klogui::qsoDateTime->setDateTime(dateTime);
	//}
}
/*
void Klog::showTip(){
// TODO: Provide the list of tips
// To show a tip on the begining of KLog
// Another way could be receibing an integer to show one or other tip. The tip number can be saved in the configuration file.
//	cout << "KLog::showTip" << endl;
	KlogTip *klogtip = new KlogTip();
	
	klogtip->show(); // Single threaded // exec


}
*/

/********************************************************************************
****                        Begining of Printing Stuff                       ****
********************************************************************************/
void Klog::filePrint(){
// Part of this code comes from KEdit
//  bool aborted = false;
//cout << "KLog::filePrint" << endl;
  bool longQRZ = false;
  QString headerLeft = i18n("Printing date: %1").arg(KGlobal::locale()->formatDate(QDate::currentDate(),true));
  
  QString headerMid = "KLog-" + Klog::KLogVersion + " - http://jaime.robles.nu/klog";
  QString headerRight;
  QString headerLog = "Number\tDate              Time\tQRZ\t\tRST(rx)  RST(tx)\tBandMode\tPower" ;

  QFont printFont ("Times", 10 );
  QFont headerFont(printFont);
  headerFont.setBold(true);
  QFontMetrics printFontMetrics(printFont);
  QFontMetrics headerFontMetrics(headerFont);
  KPrinter *printer = new KPrinter;
//      KPrinter printer;
  if(printer->setup(this, i18n("Print %1") ) ) {

    // set up KPrinter
    printer->setFullPage(false);
    printer->setCreator("KLog");
//        if ( !m_caption.isEmpty() )
//            printer->setDocName(m_caption);

    QPainter *p = new QPainter;
    p->begin( printer );

    QPaintDeviceMetrics metrics( printer );
    int dy = 0;

    p->setFont(headerFont);
    int w = printFontMetrics.width("M");
    p->setTabStops(8*w);

    int page = 1;
    int lineCount = 0;
    // This maxLineCount should be the QSO max number
    int maxLineCount = Klog::number;
    Klog::LogBook::iterator it;
    it = logbook.begin(); // I am possitioning at the Log's start

    while(true) {
      headerRight = QString(i18n("Page: %1")).arg(page);
      dy = headerFontMetrics.lineSpacing();
      QRect body( 0, dy*2,  metrics.width(), metrics.height()-dy*2);

      p->drawText(0, 0, metrics.width(), dy, Qt::AlignLeft, headerLeft);
      p->drawText(0, 0, metrics.width(), dy, Qt::AlignHCenter, headerMid);
      p->drawText(0, 0, metrics.width(), dy, Qt::AlignRight, headerRight);

      QPen pen;
      pen.setWidth(3);
      p->setPen(pen);

      p->drawLine(0, dy+dy/2, metrics.width(), dy+dy/2);
      int y = dy*2;
      p->drawText(0, y, metrics.width(), y, QPainter::ExpandTabs | QPainter::WordBreak, headerLog);
      y += dy;

      
//        for ( it = logbook.begin(); it != logbook.end(); ++it ){           
      while(lineCount < maxLineCount) {
//              QString text = eframe->textLine(lineCount);
        if ((*it).getQrz().length() >= 8)
          longQRZ = true;
        else
          longQRZ = false;
     
     
        QString text = QString::number((*it).getNumb()) + "\t" + (*it).getDateTime().toString("dd-MM-yyyy") + " - " + (*it).getDateTime().toString("hh:mm") + "\t" + (*it).getQrz();
        if (!longQRZ)  // If it is a SHORT QRZ, we need only TWO tabs
          text = text + "\t";
        if(((*it).getQrz()).contains('/'))
          text = text + "\t";
        text = text + "\t" + QString::number((*it).getRstrx()) ;
 if ((QString::number((*it).getRstrx())).length()>2)
          text = text + "      ";
        else
   text = text + "       ";
     text = text + QString::number((*it).getRsttx()) + "\t" + (*it).getBand() + "\t" + (*it).getMode() + "\t" + (*it).getPower();
        it++;
        longQRZ = false;
        if( text == "" )
          text = " ";     // don't ignore empty lines
        QRect r = p->boundingRect(0, y, body.width(), body.height(), QPainter::ExpandTabs | QPainter::WordBreak, text);
        dy = r.height();
        if (y+dy > metrics.height()) break;
        if ((*it).getNumb() != 0){
          p->drawText(0, y, metrics.width(), metrics.height() - y, QPainter::ExpandTabs | QPainter::WordBreak, text);
          y += dy;
        }
        lineCount++;
      }
      if (lineCount >= maxLineCount)
        break;
      if (it == logbook.end() )
        break;
      printer->newPage();
      page++;
    }
    p->end();
    delete p;
  }
  delete printer;
/*
// This is just to show the state but it does nothing
    if (aborted)
      setGeneralStatusField(i18n("Printing aborted."));
    else
      setGeneralStatusField(i18n("Printing complete."));
*/
}

/*


*/



void Klog::sortLog(){
// I will read the Log from the UI and sorting using the numbers.
//cout << "KLog::sortLog" << endl;
  if ( !klogui::logListView->firstChild() ) // This is a health check to avoid sorting
    return;
  int progresStep = 0;
  Qso tQso; // Temp QSO
  oLogbook.clear();                             // an empty list
  Klog::LogBook::iterator it;
  QListViewItemIterator itl( klogui::logListView );

  QProgressDialog progress( "Sorting the log...", "Abort sorting", Klog::number,
                          this, "progress", TRUE );

  for ( ; itl.current(); ++itl ){
    for ( it = logbook.begin(); it != logbook.end(); ++it ){  //We run the log...
      if ( (*it).getNumb() == (itl.current()->text(0)).toInt() ){
        progresStep++;
        if (showProgressDialog){
          progress.setProgress( progresStep );
          qApp->processEvents();
        }
        tQso = (*it);
        if ( progress.wasCanceled())
          return;
        oLogbook.append(tQso);
      }
        if ( progress.wasCanceled())
          return;

    }
  }
  logbook = oLogbook;
}

/********************************************************************************
****                        End of Printing Stuff                            ****
********************************************************************************/

/********************************************************************************
**** The following slots are for the Cluster support                         ****
*********************************************************************************/


void Klog::slotClusterConnect(){
//cout << "KLog::slotClusterConnect" << endl;
	if (dxClusterConnected)
		return; // If we are connected we don't want to start another connection
	
	if ((DXClusterServerToUse.section(':',1,1)).toInt() <= 0){
		QMessageBox::about( this, i18n("KLog message:"),
                            i18n("Check your cluster settings\n"
                            "in Setup->Preferences\nbefore connecting!"));

		return;
	}

	// create the socket and connect various of its signals
	socket = new QSocket( this );
	connect( socket, SIGNAL(connected()), SLOT(slotClusterSocketConnected()) );
	connect( socket, SIGNAL(connectionClosed()), SLOT(slotClusterSocketConnectionClosed()) );
	connect( socket, SIGNAL(readyRead()), SLOT(slotClusterSocketReadyRead()) );
	connect( socket, SIGNAL(error(int)), SLOT(slotClusterSocketError(int)) );
	// connect to the server

	klogui::dxClusterkListView->setSorting(-1, false);
	klogui::downTabs->setCurrentPage(1); // We set the active tab to be the cluster's
	dxClusterSpotItem * item = new dxClusterSpotItem(klogui::dxClusterkListView, i18n("Trying to connect to the server\n"), defaultColor);

	socket->connectToHost( DXClusterServerToUse.section(':',0,0), (DXClusterServerToUse.section(':',1,1)).toInt() );

}

void Klog::slotClusterCloseConnection(){
//cout << "KLog::slotClusterCloseConnection" << endl;
	if (!dxClusterConnected)
		return; // If we are not we can't close any conection
	socket->close();
	if ( socket->state() == QSocket::Closing ) {
	// We have a delayed close.
		connect( socket, SIGNAL(delayedCloseFinished()), SLOT(socketClosed()) );
	} else {
	// The socket is closed.
		slotClusterSocketClosed();
	}
}

void Klog::slotClusterSendToServer(){
//cout << "KLog::slotClusterSendToServer" << endl;
	if (!dxClusterConnected)
		return; // If we are not connected we cannot send any text to the server
		// write to the server
	QTextStream os(socket);
	//  if (ClusterkLineEditInput->text() == "bye")
	//    os << ann/local "Try the free Logging software for Linux: KLog: http://jaime.roble.nu/klog
	os << ClusterkLineEditInPut->text() << "\n";
	ClusterkLineEditInPut->setText( "" );
}

void Klog::slotClusterSocketReadyRead(){
// read from the server
// The while could block the flow of the program?
//cout << "KLog::slotClusterSocketReadyRead" << endl;
	while ( socket->canReadLine() ) {
		dxClusterString =  socket->readLine();
		dxClusterShow = dxClusterString;
		QStringList tokens = QStringList::split( ' ', dxClusterString );
		
		// It is a "DX de SP0TTER FREC DXCALL"
		//0 = DX, 1 = de, 2 = spotter, 3 = Freq, 4 = dxcall
		if ((tokens[0] == "DX") && (tokens[1] == "de")){
			if ( (!dxClusterHFSpots) && (adif.isHF(tokens[3]))){ //Check the freq
//				cout << "KLog::slotClusterSocketReady: isHF" << endl;
				return;
			}
			if ( (!dxClusterVHFSpots) && (adif.isVHF(tokens[3]))){
//				cout << "KLog::slotClusterSocketReady: isVHF" << endl;
				return;
			}
			if ( (!dxClusterWARCSpots) && (adif.isWARC(tokens[3]))){
//				cout << "KLog::slotClusterSocketReady: isWARC" << endl;
				return;
			}
			if ( (!dxClusterConfirmedSpots) && ((needToWorkFromCluster(tokens[4],adif.freq2Int(tokens[3])))==0)){
//				cout << "KLog::slotClusterSocketReady: isConfirmed and don't want them" << endl;
//				cout << "KLog::slotClusterSocketReady: "<< tokens[4] << " / " <<tokens[3] << endl;
				return;
			}

//		cout << "Cluster Receive: " << tokens[4] << " - " << tokens[3] << " / " << QString::number(adif.freq2Int( tokens[3]) ) << endl;
		switch(needToWorkFromCluster(tokens[4],adif.freq2Int(tokens[3]))){
			// 0 confirmed, 1 worked, 2 new one, -1 error
		case 0: // Continue
			//CONFIRMED - Red
			neededWorkedConfirmed = 2;
			dxSpotColor = confirmedColor;
		break;
		case 1: // Continue
			//WORKED - Yellow
		{	neededWorkedConfirmed = 1;
			dxSpotColor = workedColor;
// 			bandMapSpotItem * item = new bandMapSpotItem(klogui::bandmapListView, tokens[3], tokens[4], tokens[2], dxSpotColor);
		}
		break;
		case 2:{
			//New one - Green
			neededWorkedConfirmed = 0;
			dxSpotColor = neededColor;
// 			bandMapSpotItem * item = new bandMapSpotItem(klogui::bandmapListView, tokens[3], tokens[4], tokens[2], dxSpotColor);

		}
		break;
		case -1:
		// An error
			neededWorkedConfirmed = 3;
			dxSpotColor = defaultColor;
		break;
		default:
			neededWorkedConfirmed = 3;
			dxSpotColor = defaultColor;
		break;
		}// Closes the Switch
// Maybe we have just asked for a "SH/DX" command so data is presented
// in another way.
		// 0 = freq, 1 = dxcall, 2 = date, ...
		}else if (    ((adif.isHF(tokens[0])) || (adif.isVHF(tokens[0])))  && (tokens[0] != "***" )  ) {
// I am nearly copying the previous section... so maybe an optimization should be done.


			if ( (!dxClusterHFSpots) && (adif.isHF(tokens[0]))){ //Check the freq
//				cout << "KLog::slotClusterSocketReady: isHF" << endl;
				return;
			}
			if ( (!dxClusterVHFSpots) && (adif.isVHF(tokens[0]))){
//				cout << "KLog::slotClusterSocketReady: isVHF" << endl;
				return;
			}

			if ( (!dxClusterConfirmedSpots) && (dxcc.isConfirmed(world.findEntity(tokens[4])))){
//				cout << "KLog::slotClusterSocketReady: isConfirmed" << endl;
				return;
			}

		switch(needToWorkFromCluster(tokens[1],adif.freq2Int(tokens[0]))){
		// 0 confirmed, 1 worked, 2 new one, -1 error
		case 0: // Continue
			//CONFIRMED - Red
			neededWorkedConfirmed = 2;
			dxSpotColor = confirmedColor;
		break;
		case 1: //WORKED
		{
			neededWorkedConfirmed = 1;
			dxSpotColor = workedColor;
/*			bandMapSpotItem * item = new bandMapSpotItem(klogui::bandmapListView, tokens[0], tokens[1], "Unknown", dxSpotColor);*/
		}
		break;
		case 2://New one - Green
			{neededWorkedConfirmed = 0;
			dxSpotColor = neededColor;
/*			bandMapSpotItem * item = new bandMapSpotItem(klogui::bandmapListView, tokens[0], tokens[1], "Unknown", dxSpotColor);*/
		}
		break;
		case -1: // An error
			neededWorkedConfirmed = 3;
			dxSpotColor = defaultColor;
		break;
		default:
			neededWorkedConfirmed = 3;
			dxSpotColor = defaultColor;
		break;
		}// Closes the Switch

	}else if ((tokens[0] == "To") && (tokens[1] == "ALL") && (tokens[2] == "de")){
		// Now look for ANNOUNCES (ANN/FULL)
		dxSpotColor = defaultColor;
		if (dxClusterANNounces)
			return;
	}else if ((tokens[0] == "To") && (tokens[1] == "LOCAL") && (tokens[2] == "de")){ // Now look for ANNOUNCES
      // Now look for ANNOUNCES (ANN/LOCAL)
		dxSpotColor = defaultColor;
		if (dxClusterANNounces)
			return;
	}else if ((tokens[0] == "WWV") && (tokens[1] == "de")){ // Now look for WWANNOUNCES
	// Now look for WWV (ANN/LOCAL)
		dxSpotColor = defaultColor;
		if (dxClusterWXANNounces)
			return;
	}else if ((tokens[0] == "WCY") && (tokens[1] == "de")){ // Now look for WCYANNOUNCES
	// Now look for WWV (ANN/LOCAL)
		dxSpotColor = defaultColor;
		if (dxClusterWCYANNounces)
			return;
	}else{	// next was defaultColor
		dxSpotColor = defaultColor;
	}
	if (dxClusterString.length()>=5){
		dxClusterSpotItem * item = new dxClusterSpotItem(klogui::dxClusterkListView, dxClusterString, dxSpotColor);
		dxSpotColor = defaultColor; // The color should be default by default
	} 
	}
}

void Klog::slotClusterSocketConnected(){
//cout << "KLog::slotClusterSocketConnected" << endl;
	dxClusterSpotItem * item = new dxClusterSpotItem(klogui::dxClusterkListView, i18n("Connected to server"), defaultColor);
	dxClusterConnected = true;
}

void Klog::slotClusterSocketConnectionClosed(){
//cout << "KLog::slotClusterSocketConnectionClosed" << endl;
	dxClusterSpotItem * item = new dxClusterSpotItem(klogui::dxClusterkListView, i18n("Connection closed by the server"), defaultColor);
	dxClusterConnected = false;
}

void Klog::slotClusterSocketClosed(){
//cout << "KLog::slotClusterSocketCluster" << endl;
	dxClusterSpotItem * item = new dxClusterSpotItem(klogui::dxClusterkListView, i18n("Connection closed"), defaultColor );
	dxClusterConnected = false;
}

void Klog::slotClusterSocketError( int e ){
//cout << "KLog::slotClusterSocketError" << endl;
//QSocket::ErrConnectionRefused - if the connection was refused 
//QSocket::ErrHostNotFound - if the host was not found 
//QSocket::ErrSocketRead - if a read from the socket failed    
    if (e == QSocket::ErrConnectionRefused){
 QMessageBox::about( this, i18n("KLog message:"),
       i18n("DX-Cluster server refused the connection\n"+DXClusterServerToUse+
                            "\nMaybe you are already connected, use another call like \"YOURCALL-1\"!"));

 
    }else if (e == QSocket::ErrHostNotFound){
 QMessageBox::about( this, i18n("KLog message:"),
       i18n("Host not found:\n"+DXClusterServerToUse+
                            "\nCheck your network settings!")); 
    }else if (e == QSocket::ErrSocketRead){
 QMessageBox::about( this, i18n("KLog message:"),
       i18n("Error Socket Read,\n"
                            "Error unknown!"));
    }

  dxClusterSpotItem * item = new dxClusterSpotItem(klogui::dxClusterkListView, i18n("Error number %1 occurred").arg(e), defaultColor);    
}

void Klog::slotClusterClearInputLine(){
//cout << "KLog::slotClusterClearInputLine" << endl;
	ClusterkLineEditInPut->setText( "" );
}


int Klog::needToWorkFromCluster(const QString &tqrz, const int tband){
// 0 confirmed, 1 worked, 2 new one, -1 error
//cout << "KLog::needToWorkFromCluster" << endl;
	if (tband == -1){ // If the spot's band is not supported by Klog, we can't say if worked B4
		return -1;
	}
	dxClusterEnti = 0;
	dxClusterCallLen = (tqrz).length();
	dxClusterEnti = world.findEntity(tqrz.upper());

	if (dxClusterEnti !=0 ){
		if(dxcc.isConfirmedBand(dxClusterEnti, tband)) {
			return 0;
		}else{
			if(dxcc.isWorkedBand(dxClusterEnti, tband)){    // NOT CONFIRMED... ¿Worked?
			// IT IS WORKED IN THIS BAND BUT STILL NOT CONFIRMED, WORK IT!
				return 1;
			}else{
			// IT IS NOT WORKED IN THIS BAND SO WORK IT!
				return 2;
			}
		}
	}else{
		return -1;
	}
}

// This takes a DX-spot from the DXCluster window and copies to the QSO entry box
// when the user clicks on it.
void Klog::slotClusterSpotToLog(QListViewItem * item){
//cout << "KLog::slotClusterSpotToLog" << endl;
  if (item)
    dxClusterString = item->text(0);
  else
   return;
  slotClearBtn(); 
  QStringList tokens = QStringList::split( ' ', dxClusterString );
  // It is a "DX de SP0TTER FREC DXCALL", if not... we won't try to add to the log ;-)
  if ((tokens[0] == "DX") && (tokens[1] == "de")){
    if ((tokens[1]).length() != 4){
      klogui::qrzLineEdit->setText(tokens[4]);
      klogui::bandComboBox->setCurrentItem(adif.freq2Int(tokens[3]));    
      klogui::freqlCDNumber->display((tokens[3]).toDouble());      // We show the frequency in the box      
       if ((hamlibPossible) && (hamlib)){
      // Here we set the transceiver to the frequency and mode to work the DX-spot!
    KlogHamlib.setFrequency((tokens[3]).toDouble());


    if ( adif.freq2Int((tokens[3]))>3 ){
 KlogHamlib.setMode("USB");
    }else{
 KlogHamlib.setMode("LSB");
    }

      }
    }
  }else if ( ((adif.isHF(tokens[0])) || (adif.isVHF(tokens[0])))  && (tokens[0] != "***" ) ){
    if ((tokens[1]).length() != 0){
      klogui::qrzLineEdit->setText(tokens[1]);
      klogui::bandComboBox->setCurrentItem(adif.freq2Int(tokens[0]));
      klogui::freqlCDNumber->display((tokens[0]).toDouble());      // We show the frequency in the box      
      if (hamlib){
      // Here we set the transceiver to the frequency and mode to work the DX-spot!
   KlogHamlib.setFrequency((tokens[0]).toDouble());
       if (adif.freq2Int((tokens[0]))>3){
 KlogHamlib.setMode("USB");
    }else{
 KlogHamlib.setMode("LSB");
    }

      }

    }
  }else    // It is NOT an spot but an announce or similar.
   return;
// Getting and proposing a MODE could be good...

	enti = 0;
	if (((klogui::qrzLineEdit->text()).upper()).length() != 0){
		enti = world.findEntity((klogui::qrzLineEdit->text()).upper());
	}
	showWhere(enti);

}

void Klog::slotClusterSpotCheck(QListViewItem * item){
//cout << "KLog::slotClusterSpotCheck" << endl;
  if (item)
    dxClusterString = item->text(0);
  else{
	//cout << "KLog: slotClusterSpotCheck Limpiamos con clearEntityBox-1" << endl;
    clearEntityBox();
    return;
  }
  enti = 0;
  QStringList tokens = QStringList::split( ' ', dxClusterString );
  // It is a "DX de SP0TTER FREC DXCALL", if not... we won't try to add to the log ;-)
  if ((tokens[0] == "DX") && (tokens[1] == "de")){
    if ((tokens[4]).length() != 0)
      enti = world.findEntity((tokens[4]).upper());

  }else if ( ((adif.isHF(tokens[0])) || (adif.isVHF(tokens[0])))  && (tokens[0] != "***" ) ){
    if ((tokens[1]).length() != 0)
      enti = world.findEntity((tokens[1]).upper());

  }else{    // It is NOT an spot but an announce or similar.
	//cout << "KLog: slotClusterSpotCheck Limpiamos con clearEntityBox-2" << endl;
    clearEntityBox();
    return;
  }
  //QRZ = tokens[4]
  // Band = adif.freq2Int(tokens[3])
//  klogui::bandComboBox->setCurrentItem(adif.freq2Int(tokens[3]));
// Getting and proposing a MODE could be good...

  showWhere(enti);


}


//void Klog::addDXSpotToBandMap(QString freq, QString dx, QString from){


//cout << "KLog::addDXSpotToBandMap" << endl;
/*
        QListViewItem * item = new QListViewItem( klogui::logListView, 0 );
        item->setText( 0, getNumberString(qso.getNumb())  ); 
        item->setText( 1, qso.getDateTime().toString("yyyy-MM-dd") );
*/
/*
          logbook.append(qso);
          if (enti != 0){
            dxcc.workedString(enti, qso.getBand(), qso.getMode());
            waz.workedString( world.getCqz(), qso.getBand(), qso.getMode() );
          }
        QListViewItem * item = new QListViewItem( klogui::logListView, 0 );
//        item->setText( 0, QString::number(qso.getNumb()) );
        item->setText( 0, getNumberString(qso.getNumb())  ); 
        item->setText( 1, qso.getDateTime().toString("yyyy-MM-dd") );
        item->setText( 2, qso.getDateTime().toString("hh:mm"));
        item->setText( 3, qso.getQrz().upper() );
        item->setText( 4, QString::number(qso.getRsttx()) );
        item->setText( 5, QString::number(qso.getRstrx()) );
        item->setText( 6, qso.getBand() );
        item->setText( 7, qso.getMode() );
        item->setText( 8, qso.getPower() );
        item->setText( 9, qso.getComment() );
*/
//}


/******************************************************************************
****          END OF THE CLUSTER'S FUNCTIONS                               ****
******************************************************************************/
/*
void Klog::slotQslNeededCheck(){
  
  QslNeeded *qslneeded = new QslNeeded();
  qslneeded->exec();
  
}
*/

bool Klog::checkIfValidDXCluster(const QString &tdxcluster){
//cout << "KLog::checkIfValidDXCluster" << endl;
	QUrl url("http://"+tdxcluster);
	if ((url.hasHost())||(url.hasPort()))
		return true;
	else
		return false;
}

void Klog::slotKlogSetup(){
// The user wants to configure KLog
//cout << "KLog::slotKlogSetup" << endl;
	Setup *setup = new Setup();
	setup->exec(); // Single threaded
	//setup->show();
	readConf();
	// It is necesary to update all the values affected by the configuration:
	// Till now only power is shown in the GUI
	klogui::powerSpinBox->setValue(power.toInt());
	// It is necessary to re-paint the color affected widgets
}

void Klog::slotBugReport(){
//cout << "KLog::slotBugReport" << endl;
//	if( bugReport == 0 ){
//		bugReport = new KBugReport();
//	}
//	bugReport->show();

QString auxString = "Please send your bug-report/wish to jaime@robles.nu\nInclude the KLog version you are using ("+Klog::KLogVersion + ")";

QMessageBox::about( this, i18n("KLog message: Bug-reporting"),
                            i18n(auxString+"\n"
                            "All your suggestions will be welcome!"));

}

void Klog::slotModeChanged (int i){
//cout << "KLog::slotModeChanged" << endl;
	if (((klogui::qrzLineEdit->text()).length())==0)
		return;

	if (i == 0){ // The mode is SSB
		//    klogui::TRecBox->setHidden(true);
		klogui::TRecBox->setEnabled(false);
		//    klogui::TSendBox->setHidden(true);
		klogui::TSendBox->setEnabled(false);
		//    klogui::textLabel16->setHidden(true);
		//    klogui::textLabel16->setEnabled(false); 
		tTxValue = 0;
		tRxValue = 0;
		klogui::TSendBox->setMaxValue(0);
		klogui::TRecBox->setMaxValue(0);
	}else if (i == 2){ // The mode is FM
		//    klogui::TRecBox->setHidden(true);
		klogui::TRecBox->setEnabled(false);
//    klogui::TSendBox->setHidden(true);
    klogui::TSendBox->setEnabled(false);
//    klogui::textLabel16->setHidden(true);
 //   klogui::textLabel16->setEnabled(false); This is from the RST (deleted 27/feb/2005)
    tTxValue = 0;
    tRxValue = 0;
    klogui::TSendBox->setMaxValue(0);
    klogui::TRecBox->setMaxValue(0);
  }else if (i == 4){ // The mode is SSTV
//    klogui::TRecBox->setHidden(false);
    klogui::TRecBox->setEnabled(true);    
//    klogui::TSendBox->setHidden(false);
    klogui::TSendBox->setEnabled(true);
//    klogui::textLabel16->setHidden(false);
//    klogui::textLabel16->setEnabled(true); 
    tTxValue = 5;
    tRxValue = 5;
    klogui::TSendBox->setMaxValue(5);
    klogui::TRecBox->setMaxValue(5);
  }else if (i == 8){ // The mode is AM
//    klogui::TRecBox->setHidden(true);
    klogui::TRecBox->setEnabled(false);
//    klogui::TSendBox->setHidden(true);
    klogui::TSendBox->setEnabled(false);
//    klogui::textLabel16->setHidden(true);
//    klogui::textLabel16->setEnabled(false); This is from the RST (deleted 27/feb/2005)
    tTxValue = 0;
    tRxValue = 0;
    klogui::TSendBox->setMaxValue(0);
    klogui::TRecBox->setMaxValue(0);
  }else{
    klogui::TSendBox->setMaxValue(9);
    klogui::TRecBox->setMaxValue(9);
    tTxValue = 9;
    tRxValue = 9;
//    klogui::TRecBox->setHidden(false);
    klogui::TRecBox->setEnabled(true);    
//    klogui::TSendBox->setHidden(false);
    klogui::TSendBox->setEnabled(true);
//    klogui::textLabel16->setHidden(false);
//    klogui::textLabel16->setEnabled(true);
  }
  imode = i;
  klogui::TSendBox->setValue(tTxValue);
  klogui::TRecBox->setValue(tRxValue);
  entityState(enti);
}

void Klog::slotBandChanged (){
//TODO: To check if this slot is really necessary.
//cout << "KLog::slotBandChanged: "<< QString::number(enti) << endl;
	if (((klogui::qrzLineEdit->text()).length())==0)
		return;
	entityState(enti);
}


bool Klog::haveAllTheFields(){
//cout << "KLog::haveAllTheFields" << endl;
	if (requireMandatory){
		for (i=0;i<7;i++){
			if (!haveAllMandatoryFields[i]){
//				cout << "Klog::haveAllTheFields(): " << QString::number(i) << endl;
				return false;
			}
		}
	}
	return true;
}

void Klog::addToPreviouslyWorked(){
// There is still something to implement.
// If we add some data that was not in the fist QSO, this QSO (first) is not
// updated.    
// Name, QTH, QSL via, locator, ¿IOTA?
//cout << "KLog::addToPreviouslyWorked" << endl;
	if (!completeWithPrevious) // If we have configured not to use this capability
		return;
	kk = workedCall.getWorked( (klogui::searchQrzkLineEdit->text()).upper() ); 
	if (  kk < 0){
		workedCall.addCallToWorkedCalls(qso.getQrz(), qso.getNumb()); 
	}    
}


//Intended to complete the actual QSO if has been worked before.
void Klog::showIfPreviouslyWorked(){ // Uses previousQso and workedCall
//cout << "KLog::showIfPreviouslyWorked" << endl;
// The affected fields are:
//	Name, QTH, Locator, QSLVia & Manager

// kk is just an auxiliar int variable for "intrafunction" use only    

//To add a tab inb the right box to show the data from previous QSOs for a call.
// This tab will have a button to copy the data into the actual QSO box if wished.


	////cout << "KLog::showIfPreviouslyWorked" << endl;
//	if (!completeWithPrevious) // If we have configured KLog not to use this feature
//		return;
//	kk = workedCall.getWorked( (klogui::searchQrzkLineEdit->text()).upper() ); 
//	if (  kk >= 0){
//		previousQso = getByNumber(kk);
//		if ((previousQso.getQth()).length() >=2)
//			klogui::qthkLineEdit->setText(previousQso.getQth());
//		if ((previousQso.getName()).length() >=2)
//			klogui::namekLineEdit->setText(previousQso.getName());
//		dxLocator = previousQso.getLocator();
//		klogui::locatorLineEdit->setText(dxLocator);
//		slotLocatorChanged();
//		
//		//QSL Info
//		klogui::QSLcomboBox->setCurrentText(previousQso.getQslVia());
//		if ((previousQso.getQslVia()).compare("No QSL") == 0){
//			klogui::qslVialineEdit->setDisabled(true);
//			// klogui::QSLInfotextEdit->setDisabled(true);
//			klogui::qslVialineEdit->clear();
//			// klogui::QSLInfotextEdit->clear();
//		}else{
//			if ((previousQso.getQslVia()).compare("Manager") == 0){
//				klogui::qslVialineEdit->setEnabled(true);
//				klogui::qslVialineEdit->setText(previousQso.getQslManager());
//			}else
//				klogui::qslVialineEdit->setDisabled(true);
			//        klogui::QSLInfotextEdit->setEnabled(true);
	//		klogui::QSLInfotextEdit->setText(previousQso.getQslInfo());
	//	}
		// IOTA
		////cout << "KLog::showIfPreviouslyWorked - IOTA: " << QString::number(previousQso.getIotaNumber()) << endl;
		/*i = 0;
		if (previousQso.getIotaNumber() != 0) {
			i = adif.continent2Number((qso.getIotaContinent()));
			klogui::iotaComboBox->setCurrentItem(i);
			klogui::iotaIntSpinBox->setValue(previousQso.getIotaNumber());
		}*/
		// Local Award
		/*if (previousQso.getLocalAward() != 0) {
			klogui::iotaComboBox->setCurrentItem(previousQso.getIotaContinent());
			klogui::iotaIntSpinBox->setEnabled(true);
			klogui::iotaIntSpinBox->setValue(previousQso.getIotaNumber());
		}*/
//	}else{ // If not worked B4, we clean the boxes...
		// Is it really needed? It contradict the "auto IOTA box setting (prepareIOTAComboBox)
		/*i = 0;
		if (klogui::iotaIntSpinBox->value() != 0);
			i = klogui::iotaIntSpinBox->value();
		clearGUI();*/
//	}
}

void Klog::clearGUI(){
//cout << "KLog: clearGUI" << endl;
	klogui::qthkLineEdit->clear();
	klogui::namekLineEdit->clear();
	klogui::locatorLineEdit->clear();
	klogui::myLocatorLineEdit->clear();

	Klog::dxLocator="";
	Klog::myLocatorTemp="";
	slotLocatorChanged();
	slotMyLocatorChanged();
	klogui::remarksTextEdit->clear();
	//klogui::QSLcomboBox->clear();
	klogui::qslVialineEdit->clear();
	klogui::QSLInfotextEdit->clear();
	klogui::iotaComboBox->setCurrentItem(0);
	klogui::iotaIntSpinBox->setValue(0);
	//klogui::iotaIntSpinBox->setEnabled(false);
}


void Klog::completeThePreviouslyWorked(){
//cout << "KLog::completeThePreviouslyWorked" << endl;
// kk is just an auxiliar int variable for "intrafunction" use only    
	kk = workedCall.getWorked( (klogui::searchQrzkLineEdit->text()).upper() ); 
	if (  kk >= 0){
		previousQso = getByNumber(kk);
		if ((previousQso.getQth()).length() >=2)
			klogui::qthkLineEdit->setText(previousQso.getQth());
		if ((previousQso.getName()).length() >=2)
			klogui::namekLineEdit->setText(previousQso.getName());
		dxLocator = previousQso.getLocator();
		klogui::locatorLineEdit->setText(dxLocator);
		slotLocatorChanged();
		klogui::remarksTextEdit->setText(previousQso.getComment());
        //Now the QSl info information

		klogui::QSLcomboBox->setCurrentText(previousQso.getQslVia());
		if ((previousQso.getQslVia()).compare("No QSL") == 0){
			klogui::qslVialineEdit->setDisabled(true);
			//        klogui::QSLInfotextEdit->setDisabled(true);
			klogui::qslVialineEdit->clear();
			//        klogui::QSLInfotextEdit->clear();
		}else{
			if ((previousQso.getQslVia()).compare("Manager") == 0){
				klogui::qslVialineEdit->setEnabled(true);
				klogui::qslVialineEdit->setText(previousQso.getQslManager());
			}else
				klogui::qslVialineEdit->setDisabled(true);
			//        klogui::QSLInfotextEdit->setEnabled(true);
        		klogui::QSLInfotextEdit->setText(previousQso.getQslInfo());
		}
	// IOTA
		if (previousQso.getIotaNumber() != 0) {
			klogui::iotaComboBox->setCurrentItem(previousQso.getIotaContinent());
			//klogui::iotaIntSpinBox->setEnabled(true);
			klogui::iotaIntSpinBox->setValue(previousQso.getIotaNumber());
		}
	}
}

void Klog::getAllTheCallsFromLog(){
// The next lines are for the previously worked functionality
/* Maybe they introduce BIG delays in the log reading from disk... to be checked
*/
//cout << "KLog::getAllTheCallsFromLog" << endl;
	LogBook::iterator it;
	for ( it = logbook.begin(); it != logbook.end(); ++it ){
		kk = workedCall.getWorked( qso.getQrz()); 
		if (  kk < 0){
			workedCall.addCallToWorkedCalls(qso.getQrz(), qso.getNumb()); 
		}
	}
}

void Klog::completeAllQSOsFromLog(){
//cout << "KLog::completeAllQSOsFromLog" << endl; 

  LogBook::iterator it;
  
  for ( it = logbook.begin(); it != logbook.end(); ++it ){
    if (qso.getQrz() == (*it).getQrz() ){
      qso.setName((*it).getName());
      qso.setQth((*it).getQth());
      qso.setLocator((*it).getLocator());
      qso.setQslVia((*it).getQslVia());
      qso.setQslManager((*it).getQslManager());
      qso.setQslVia((*it).getQslVia());
      qso.setQslInfo((*it).getQslInfo());
      qso.setComment((*it).getComment());
      qso.setIota((*it).getIota());
          qso.setComment(klogui::remarksTextEdit->text());
  qso.setQslVia(klogui::QSLcomboBox->currentText());
  // Check if the locator is valid
  if (locator.isValidLocator((klogui::locatorLineEdit->text()).upper()))
    qso.setLocator((klogui::locatorLineEdit->text()).upper());


  if(klogui::qslVialineEdit->isEnabled())
    qso.setQslManager((klogui::qslVialineEdit->text()).upper());

  if ((klogui::QSLInfotextEdit->text()).length() > 0)
  //if(klogui::QSLInfotextEdit->isEnabled())
    qso.setQslInfo(klogui::QSLInfotextEdit->text());

  if((klogui::namekLineEdit->text()).length() >= 2)
    qso.setName((klogui::namekLineEdit->text()).upper());

  if((klogui::qthkLineEdit->text()).length() >= 2)
    qso.setQth((klogui::qthkLineEdit->text()).upper());


    }
  }
}


// Hamlib support
void Klog::slothamlibUpdateFrequency(){
//cout << "KLog::slothamlibUpdateFrequency" << endl;
	if (!hamlib){
		return;
		//hamlibtimer->stop();
	}	 
	if ((!hamlib)||(!hamlibReading)||(!hamlibPossible)){
		return;
	}
	hamlibFreq = 0;
	hamlibFreq = KlogHamlib.getFrequency();
	if (hamlibFreq > 0){
		band = adif.band2Int(adif.freq2Band(QString::number(hamlibFreq)));
		klogui::bandComboBox->setCurrentItem(band);
		klogui::freqlCDNumber->display(hamlibFreq);
	}
	i = adif.mode2Int(KlogHamlib.getMode());
	if (i >= 0){
		klogui::modeComboBox->setCurrentItem(i);
	}
}


/***************************************************************************
** This is an auxiliary class intended to provide color to the DX-Cluster **
** spots.                                                                 **
** It may be moved to a self .h & .cpp archives                           **
****************************************************************************/
dxClusterSpotItem::dxClusterSpotItem( KListView *parent, const QString& spot, const QColor& color ) : KListViewItem( parent ){
//cout << "KLog::dxClusterSpotItem - Constructor" << endl;
  spotColor = color;
  setText(0, spot);
}

dxClusterSpotItem::~dxClusterSpotItem(){
//cout << "KLog::dxClusterSpotItem - Destructor" << endl;
}

void dxClusterSpotItem::paintCell( QPainter *p, const QColorGroup &cg,
                                 int column, int width, int alignment ){
//cout << "KLog::dxClusterSpotItem - paintCell" << endl;
  QColorGroup _cg( cg );
  QColor c = _cg.text();
  _cg.setColor( QColorGroup::Text, spotColor );

  QListViewItem::paintCell( p, _cg, column, width, alignment );
  _cg.setColor( QColorGroup::Text, spotColor );
    
}

/***************************************************************************
** This is an auxiliary class intended to provide color to the SEARCH BOX **
** It may be moved to a self .h & .cpp archives                           **
****************************************************************************/
searchBoxItem::searchBoxItem( KListView *parent, const QString& call, const QString& date, const QString& time,const QString& band, const QString& mode, const QString& RSTsent, const QString& RSTrec, const QString& numb, const QColor& color ) : KListViewItem( parent ){
//cout << "KLog::searchBoxItem - Constructor" << endl;
  qsoColor = color;
  setText(0, call);
  setText(1, date);
  setText(2, time);  
  setText(3, band);
  setText(4, mode);
  setText(5, RSTsent);
  setText(6, RSTrec);
  setText(7, numb);

}

searchBoxItem::~searchBoxItem(){
//cout << "KLog::searchBoxItem - Destructor" << endl;
}

void searchBoxItem::paintCell( QPainter *p, const QColorGroup &cg,
                                 int column, int width, int alignment ){
//cout << "KLog::searchBoxItem - paintCell" << endl;
  QColorGroup _cg( cg );
  QColor c = _cg.text();
  _cg.setColor( QColorGroup::Text, qsoColor );

  QListViewItem::paintCell( p, _cg, column, width, alignment );
  _cg.setColor( QColorGroup::Text, qsoColor );

}


/***************************************************************************
** This is an auxiliary class intended to provide color to the DX-Cluster **
** spots.                                                                 **
** It may be moved to a self .h & .cpp archives
** BANDMAP						                     **
****************************************************************************/
// bandMapSpotItem::bandMapSpotItem( KListView *parent, const QString& freq, const QString& dx, const QString& from, const QColor& dxcolor ) : KListViewItem( parent ){
// //cout << "KLog::bandMapSpotItem - Constructor" << endl;
// 	spotColor = dxcolor;
// 	setText(0, freq);
// 	setText (1, dx);
// 	//setText (2, ); // DATE
// 	//setText (3, ); // TIME
// 	setText (4, from);
// 	//setText (5, ); NUMB
// //TODO The "FROM" is inserted with ":" and some other things...
// 
// }
// 
// bandMapSpotItem::~bandMapSpotItem(){
// //cout << "KLog::bandMapSpotItem - Destructor" << endl;
// }
// 
// void bandMapSpotItem::paintCell( QPainter *p, const QColorGroup &cg,
//                                  int column, int width, int alignment )
// {
// //cout << "KLog::bandMapSpotItem - paintCell" << endl;
//   QColorGroup _cg( cg );
//   QColor c = _cg.text();
//   _cg.setColor( QColorGroup::Text, spotColor );
// 
//   QListViewItem::paintCell( p, _cg, column, width, alignment );
//   _cg.setColor( QColorGroup::Text, spotColor );
// 
// }
// 
