/***************************************************************************
                          klog.cpp  -  description
                             -------------------
    begin                : sab dic  7 18:42:45 CET 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/
/******************************************************************************
*                                                                         *
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

#include <QtGui>
#include "klog.h"

Klog::Klog(QMainWindow *parent) : QMainWindow(parent) {
  //qDebug() << "KLog::Klog";
    setupUi( this );
    QTimer *internalTimer = new QTimer( this ); // create internal timer
    connect( internalTimer, SIGNAL(timeout()), SLOT(slotUpdateTime()) );
    internalTimer->start( 1000 );               // emit signal every 1 second

    Klog::KLogVersion = "0.5";
//     Klog::editdeletePixMap = new QPixmap("editdelete.png");
//     editdeleteOffPixMap = new QPixmap("editdeleteOff.png");
//     Klog::qslRecPixMap = new QPixmap("qslRec.png");
//     qslRecOffPixMap = new QPixmap("qslRecOff.png");
//     Klog::qslSenPixMap = new QPixmap("qslSen.png");
//     qslSenOffPixMap = new QPixmap("qslSenOff.png");
    blackColor.setNamedColor("#000000");
    for (i = 0; i<7; i++)
        haveAllMandatoryFields[i] = false;
    operatorStringAux = "";
    Klog::number = 0;
    //Klog::confirmed = 0;
    timeInUTC = true; // Date is shown in UTC unless configured
    mode = "SSB";
    // To check what int is the SSB mode
    imode = 0;
    band = 2;
    power = "100";
    entiBak = 0;
    callFound = false;
//	wasConfirmed = false;
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
    dxClusterCWSpots=true;
    dxClusterSSBSpots=true;
    dxClusterWXANNounces=true;
    dxClusterWCYANNounces=true;
    dxClusterANNounces=true;
    dxClusterConfirmedSpots=true;
    lastDelete = false;
    showProgressDialog = false;
    completeWithPrevious = false;
    completedWithPrevious = false;
    requireMandatory = true;
    ActionQsoDelete->setIcon(KIcon("edit-delete"));
    fileNewAction->setIcon(KIcon("address-book-new"));
    fileOpenAction->setIcon(KIcon("document-open"));
    fileSaveAction->setIcon(KIcon("document-save"));
    ActionQslRec->setIcon(KIcon("mail-receive"));
    ActionQsoSen->setIcon(KIcon("mail-folder-outbox"));
    fileSaveAsAction->setIcon(KIcon("document-save-as"));
    filePrintAction->setIcon(KIcon("document-print"));
    fileExitAction->setIcon(KIcon("application-exit"));
    setupAction->setIcon(KIcon("preferences-system"));
    ActionBugReport->setIcon(KIcon("tools-report-bug"));
    helpAboutAction->setIcon(KIcon("help-about"));

    // Connect all the slots
    connect(ActionAddKlogLog, SIGNAL(triggered()), this, SLOT(slotAddLog()));
    // connect(ActionQslNeededCheck, SIGNAL(triggered()), this, SLOT(slotQslNeededCheck()));
    connect(ActionSortLog, SIGNAL(triggered()), this, SLOT(sortLog()));
    connect(addTlfLogAction, SIGNAL(triggered()), this, SLOT(slotImportTlf()));
    //connect(helpContentsAction, SIGNAL(triggered()), this, SLOT(helpContents()));
    //connect(helpIndexAction, SIGNAL(triggered()), this, SLOT(helpIndex()));
    connect(iotaIntSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotIOTAChanged()));
    connect(iotaComboBox, SIGNAL(activated(QString)), this, SLOT(slotIOTAChanged()));
    connect(awardsComboBox, SIGNAL(textChanged(QString)), this, SLOT(slotLocalAwardChanged()));
    connect(toolsMerge_QSO_dataAction, SIGNAL(triggered()), this, SLOT(slotcompleteThePreviouslyWorked()));
    connect(ActionCabrilloImport, SIGNAL(triggered()), this, SLOT(slotImportCabrillo()));
    connect(qrzLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQrzChanged()));
    connect(logTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotQsoSelectedForEdit(QTreeWidgetItem *, int)));
    connect(searchQsosTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotQsoSearchSelectedForEdit(QTreeWidgetItem *, int)));
    connect(dxclusterListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *, int)), this, SLOT(slotClusterSpotToLog(QListWidgetItem *, int)));
    connect(ActionQsoDelete, SIGNAL(triggered()), this, SLOT(slotQsoDelete()));
    connect(ActionQslRec, SIGNAL(triggered()), this, SLOT(slotQSLRec));
    connect(ActionQsoSen, SIGNAL(triggered()), this, SLOT(slotQSLSent));



    klogDir = QDir::homePath()+"/.klog";  // We create the ~/.klog for the logs
    if (!QDir::setCurrent ( klogDir )){
        QDir d1(klogDir);
        dirExist = d1.mkdir(klogDir);
    }
    dirExist = QDir::setCurrent ( klogDir ) ;
    logFileNameToOpen = "";
    logFileNameToSave = "";
    tempLogFile = "tempklog.adi";

    //awards.readConfig();  // Starting the award checks
// HAMLIB
    hamlib = false;
    hamlibPossible = false;
    hamlibInterval = 500;
    bauds = 4800;
    serialPort = "/dev/ttyS0";
    hamlibFreq = 0.0;
    rignameNumber = 1; // dummy
// HAMLIB
    createKlogDir(); // if klogDir does not exist, we create it via slotKlogSetup
    readConf(); // read our data as myQrz, myLocator from ~/.klog/klogrc

    if (hamlib){ // The user selected hamlib in the Setup
        //listHamlib();
        hamlibPossible = KlogHamlib.init();
        if (hamlibPossible) { // Check if we have a rig plugged to the computer
            QTimer *hamlibtimer = new QTimer( this );
            connect( hamlibtimer, SIGNAL(timeout()), this, SLOT(slothamlibUpdateFrequency()) );
            hamlibtimer->start( hamlibInterval );
        }else{ // It is not possible to contact to your rig
          QMessageBox msgBox;
          msgBox.setText(i18n("KLog message:"));
          QString str = i18n("Could not connect to your radio.Could not connect to your radio.\nCheck your hamlib settings and restart KLog.\n\nKLog will run without hamlib support.\n\n");
          msgBox.setInformativeText(str);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.setIcon(QMessageBox::Warning);
          msgBox.exec();
        }
    }

    i = 0;
    QString comment;
    comment = "";
    // Check we have setup world and cty.dat is in your home folder
    haveWorld();
//    slotClearBtn(); //Not needed because it is called from slotQrzChanged
    modify = false;
    searching2QSL = false;
    actionSent = false;
    actionRec = false;
    //  prefixFound = false;
    //(qsoDateTime->dateEdit())->setOrder(QDateEdit::DMY);
    //(QSLSentdateEdit)->setOrder(QDateEdit::DMY);
    //(QSLRecdateEdit)->setOrder(QDateEdit::DMY);
    slotQrzChanged();
    slotModeChanged(0); //SSB = 0, the default mode
    needToSave = false; // Initialized here to avoid needing to save just after the start
    addingLog = false;	// True when adding a log file to the main one.
    // Finally, if we configured to open a file by default... we open it!
    if ((openLastByDefault == true) && (logFileNameToOpen !="")){
        adifReadLog(logFileNameToOpen);
    }
    //showTip();	// TODO: We show a tip when KLog start
//	dxcc.printWorkdStatus();
}

Klog::~Klog(){
//qDebug() << "KLog::~KLog";
}

bool Klog::haveWorld(){
    //qDebug() << "KLog::haveWorld";
    //TODO:setTextFormat(Qt::RichText) to display an URL as a link
    if (!world.isWorldCreated() ){
        int ret = QMessageBox::warning( this, i18n("Warning - Can't find cty.dat"),i18n("I can't find the cty.dat file with the DX Entity data.\nYou will not have any information on these.\n\nCopy an updated cty.dat file to your ~/.klog dir, please.\n\nYou can download from: www.country-files.com/cty/cty.dat"));
        switch(ret) {
            case QMessageBox::Yes: // Continue
                return true;
                break;
            case QMessageBox::No: // Continue
                return false;
                break;
        }
    }
    return true;
}

void Klog::slotLocatorChanged(){
  //qDebug() << "KLog::slotLocatorChanged";
    // If the locator is changed, we should re-calculate distances...
    // Manages the Locator of the DX
    dxLocator = getThisQSODXLocator();	//We first have to get the valid locator, from the call
                        // or from the user

    if ((dxLocator != "NULL") && (locator.isValidLocator(dxLocator) )){
        Klog::distance = locator.getDistanceKilometres(locator.getLon(getMyLocator()), locator.getLat(getMyLocator()), locator.getLon(dxLocator), locator.getLat(dxLocator));
        beam = locator.getBeam(locator.getLon(getMyLocator()), locator.getLat(getMyLocator()), locator.getLon(dxLocator), locator.getLat(dxLocator));
        showDistancesAndBeam(distance, beam);
    }else{
    // Should we call showDistancesAndBeam(0,0); ??????????
    }
}

void Klog::slotMyLocatorChanged(){
//When my locator changes, distances and beams should be recalculated!
//cout << "KLog::slotLocatorChanged" << (myLocatorLineEdit->text()).toUpper() << endl;

    dxLocator = getThisQSODXLocator();

    if ((locator.isValidLocator((myLocatorLineEdit->text()).toUpper())) && ((myLocatorLineEdit->text()).toUpper() != getMyLocator()) ){
        qso.setMyLocator((myLocatorLineEdit->text()).toUpper());
    }else{ // If we do not enter any locator, maybe we do not know it...

        //qso.setMyLocator(getMyLocator());
    }

    myLocatorTemp = qso.getMyLocator();

    Klog::distance = locator.getDistanceKilometres(locator.getLon(myLocatorTemp), locator.getLat(myLocatorTemp), locator.getLon(dxLocator), locator.getLat(dxLocator));
    beam = locator.getBeam(locator.getLon(myLocatorTemp), locator.getLat(myLocatorTemp), locator.getLon(dxLocator), locator.getLat(dxLocator));
    showDistancesAndBeam(distance, beam);
}

void Klog::showDistancesAndBeam(const int dist, const int beam){
    //qDebug() << "KLog::showDistancesAndBeam";
//cout << "KLog::showDistancesAndBeam" << endl;
    distancelCDNumber->display(dist);
    distancellCDNumber->display(40000 - dist);
    beamshortlCDNumber->display(beam);
    if (beam >= 180)
        beamlonglCDNumber->display(beam - 180);
    else
        beamlonglCDNumber->display(180 + beam);
}

QString Klog::getThisQSODXLocator (){
    //qDebug() << "KLog::getThisQSODXLocator";
    // Firstly we check if the user has entered one locator and, if hasn't
    // We read the DX QRZ and get a default locator from it.
    if (locator.isValidLocator((locatorLineEdit->text()).toUpper())) { //User's locator
        return (locatorLineEdit->text()).toUpper();
    } else {
        if (locator.isValidLocator(locator.getLocator((world.getEntByNumb(enti)).getLon(),(world.getEntByNumb(enti)).getLat()))   ){
            return locator.getLocator((world.getEntByNumb(enti)).getLon(),(world.getEntByNumb(enti)).getLat());
        }
    }
    return "NULL";
}

int Klog::getEntityFromCall(){ // We return the Entity number from the QRZ box call.
  //qDebug() << "KLog::getEntityFromCall";
    return world.findEntity((qrzLineEdit->text()).toUpper());
}

void Klog::slotQrzChanged(){   // We set the QRZ in the QSO
    qrzLineEdit->setText(((qrzLineEdit->text())).toUpper());
    callLen = (qrzLineEdit->text()).length();

    if (callLen == 0){ //TODO: Maybe the above check of length is not really needed (20090926-EA4TV)
        callLenPrev = callLen; // just to avoid a no end loop
        slotCancelSearchButton();
        slotClearBtn();
        return;
    } else if((callLen != 0) && (!modify)){ // Updating the searchQrzklineEdit if we are not modifying a QSO.
        enti = getEntityFromCall();
        if (enti>0){
            if (completeWithPrevious){ // If configured to use this feature
                showIfPreviouslyWorked();
            }
            if (entiBak == enti){
                callLenPrev = callLen;
            } else {
                entiBak = enti;
                // Only if we detect the entity we look for a previous qso
                // Just copying the string to the search box we will search for previously worked QSOs
                searchQrzkLineEdit->setText((qrzLineEdit->text()).toUpper());
            }
        }
    }
    // The next 3 were called with entiBak
    prepareAwardComboBox(enti);
    showWhere(enti);
    callLenPrev = callLen;
    searching2QSL = false;	// If the user enters a QSO we finish the search2QSL process
}

void Klog::prepareIOTAComboBox (const int tenti){
    //qDebug() << "KLog::prepareIOTAComboBox" << QString::number(tenti);
// We receive the Entity, get the continent and write it to the IOTA combobox
//cout << "KLog: prepareIOTAComboBox for entity: " << QString::number(tenti) << endl;
    i = 0;
    i = adif.continent2Number((world.getEntByNumb(tenti)).getContinent());
    iotaComboBox->setCurrentIndex (i);
}

void Klog::prepareAwardComboBox(const int tenti){
 //Finds if the Entity received has an award to be controlled and show the
//it on the award box
    //qDebug() << "KLog::prepareAwardComboBox"  << QString::number(tenti);
//cout << "KLog: prepareAwardComboBox for entity: " << QString::number(tenti) << endl;
    if (tenti <= 0){
        awardsComboBox->setEnabled(false);
        awardsComboBox->clear();
        awardSelectorName->setEnabled(false);
        awardSelectorName->setText(i18n("Award:"));
        localAwardName->setText(i18n("LOCAL:"));
        localAwardName->setEnabled(false);
        return;
    }


// Prepare to receive the award but... what if it does not change?
    awardsComboBox->clear();
    awardReferences.clear(); // We clear the list
    award.clearAward();

    award = awards.getAwardFor((world.getEntByNumb(tenti)).getPfx());
    if (award.getAwardReference() == -1){  // we do not have any award for this entity.
        awardsComboBox->setEnabled(false);
        awardsComboBox->clear();
        awardSelectorName->setEnabled(false);
        awardSelectorName->setText(i18n("Award:"));
        localAwardName->setText(i18n("LOCAL:"));
        localAwardName->setEnabled(false);
        return;
    }else{

        aux = "<b>" + award.getAwardName() + ":</b>";
        awardSelectorName->setText(aux);
        awardsComboBox->setEnabled(true);
        localAwardName->setText(aux);
        localAwardName->setEnabled(true);
        awardSelectorName->setEnabled(true);

        aux = (world.getEntByNumb(tenti)).getPfx();
                awardsComboBox->insertItems(0, awards.getAwardReferences(award));
    }
}

void Klog::slotClearBtn(){
//qDebug() << "KLog::slotClearBtn";
// This method clears all for the next QSO
// It is still missing the part to set the cursor to the qrzLineEdit->

    enti = -1;
//	was = false;
    Klog::j = 0;
    qso.clearQso(); // Clears the qso object
    LedtextLabel->setText(i18n( "<p align=\"center\"><b>KLog</b></p>"));
    bandComboBox->setCurrentIndex(band);
    modeComboBox->setCurrentIndex(imode);

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
    if  ((qrzLineEdit->text()).length() < 1){ // A double clicking of this button shall erase ALL
        operatorLineEdit->clear();
        stationCallsignLineEdit->clear();
    }
    if ((stationCallsignLineEdit->text()).length() < 3) {
        stationCallsignLineEdit->clear();
    }
    if ((operatorLineEdit->text()).length() < 3) {
        operatorLineEdit->clear();
    }

    //We update the time.
    //After that we clean the call/mode/band
    qrzLineEdit->clear();
    remarksTextEdit->clear();

    //iotaIntSpinBox->setEnabled(false);
    TSendBox->setValue(tTxValue);
    SSendBox->setValue(9);
    RSendBox->setValue(5);
    TRecBox->setValue(tRxValue);
    SRecBox->setValue(9);
    RRecBox->setValue(5);
    okBtn->setText(i18n("Ok"));
    clearBtn->setText(i18n("Clear"));

    QSLSentcheckBox->setChecked(false);
    QSLReccheckBox->setChecked(false);
    powerSpinBox->setValue(power.toInt());
    qthkLineEdit->clear();
    // operatorLineEdit->clear();
    namekLineEdit->clear();

    qslSen = QDate::currentDate();
    (QSLSentdateEdit)->setDate(qslSen);
    (QSLSentdateEdit)->setEnabled(false);

    qslRec = QDate::currentDate();
    (QSLRecdateEdit)->setDate(qslRec);
    (QSLRecdateEdit)->setEnabled(false);
    qslVialineEdit->setDisabled(true); // Next is the QSL info
    //  QSLInfotextEdit->setDisabled(true);
    qslVialineEdit->clear();
    QSLInfotextEdit->clear();
    QSLcomboBox->setCurrentIndex(0);

    locatorLineEdit->clear();
    Klog::dxLocator="NULL";

    Klog::myLocatorTemp = getMyLocator();  //My default locator from the klogrc
    myLocatorLineEdit->setText(myLocatorTemp);

    freqlCDNumber->display(0); // Setting the frequency box to 0

    showDistancesAndBeam(0,0);
    clearEntityBox();
    prepareAwardComboBox(enti);

    qrzLineEdit->setFocus();		// The default widget for next QSO is, obviously, the QRZ!
    searching2QSL = false;	// If the user decides to clear the qrzlinedit, we finish the search 2 QSL process.
    completedWithPrevious = false;
}

void Klog::clearEntityBox(){
// This only clear the Entity box, the distances, bearing, entity, ...
//qDebug() << "KLog::clearEntityBox";
    entityTextLabel->setText("");
    Klog::distance = 0;
    Klog::beam = 0;
    showDistancesAndBeam(0,0);
    entityTextLabel->setText("");
    prxTextLabel->setText("");
    continentTextLabel->setText("");
    cqLCDNumber->display(00);
    ituLCDNumber->display(00);
    LedtextLabel->setText(i18n( "<p align=\"center\"><b>KLog</b></p>"));
    beamshortlCDNumber->display(0);
    distancelCDNumber->display(0);
    beamlonglCDNumber->display(0);
    distancellCDNumber->display(0);

// COMMENTED TO EASE THE QT4 MIGRATION
//TODO:
//    QMessageBox msgBox;
//    msgBox.setText(i18n("KLog message:"));
//    QString str = i18n("This function (clearEntityBox) has been MODIFIED to help the QT4 migration.\nIt will be restored ASAP");
//    msgBox.setInformativeText(str);
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    msgBox.setDefaultButton(QMessageBox::Ok);
//    msgBox.setIcon(QMessageBox::Warning);
//    msgBox.exec();

    palette = defaultColor;
    kledColor->setColor(defaultColor);
    textLabelBand2->setPalette(palette );
    textLabelBand70cm->setPalette(palette );
    textLabelBand6->setPalette(palette );
    textLabelBand10->setPalette(palette );
    textLabelBand12->setPalette(palette );
    textLabelBand15->setPalette(palette );
    textLabelBand17->setPalette(palette );
    textLabelBand20->setPalette(palette );
    textLabelBand30->setPalette(palette );
    textLabelBand40->setPalette(palette );
    textLabelBand80->setPalette(palette );
    textLabelBand160->setPalette(palette );
}

void Klog::slotOkBtn(){
// The QSO is entered!!!
//qDebug() << "KLog::slotOkBtn";
    number++;
    readQso();
    if ((qso.getQrz()).length() >= 3){//There are no qrz with less than 3char
        needToSave = true;
        if (!modify){
            logbook.append(qso);
            templogbook.append(qso); //Save the just done QSO to do the auto-saving
            adifTempFileSave(tempLogFile, templogbook, false); //Autosave
            //addToPreviouslyWorked(qso.getQrz());
            kk = workedCall.addCall(qso.getQrz(), qso.getNumb());
            if (enti != 0){
                dxcc.worked(enti,bandComboBox->currentIndex(),modeComboBox->currentIndex());
                waz.worked( world.getCqzFromCall(qso.getQrz()) ,bandComboBox->currentIndex(),modeComboBox->currentIndex());
            }
        } else { // We are not ADDING but modifying a QSO.
            number--;
            modifyQso();
        }
        showQso();
    } else { number--; }//Closes the empty call check

    slotClearBtn();
    showLogList();
    showAwardsNumbers(); //Needed?
}


void Klog::fileSave(){
//qDebug() << "KLog::fileSave";
    if (logFileNameToSave != ""){
        //adifFileSave(); // Saving as ADIF
        adifTempFileSave(logFileNameToSave, logbook, true);
    }else{
        fileSaveAs();
    }
}

void Klog::fileSaveAs(){
//qDebug() << "KLog::fileSaveAs";
//cout << "KLog::fileSaveAs" << endl;
//  bool writ = false;
//  writ = false;
//  while (!writ){

  QString fn = QFileDialog::getSaveFileName(this, i18n("Save File"),
                            klogDir,
                            i18n("ADIF (*.adi)"));

  if ( !fn.isEmpty() )
    logFileNameToSave = fn;

  QFile file( logFileNameToSave );

// Deleted because QFile manages the case of file replacement and I suppose than better than us.


//       if ( file.exists( ) ) {
//
//       QMessageBox msgBox;
//       msgBox.setText(i18n("Warning - File exists"));
//       QString str = i18n("The file: ") + logFileNameToSave + i18n(" already exits.\nDo you want to overwrite?");
//       msgBox.setInformativeText(str);
//       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
//       msgBox.setDefaultButton(QMessageBox::No);
//       int ret = msgBox.exec();
//
//       switch(ret) {
//       case QMessageBox::Yes:
//         writ = true;
//       break;
//       case QMessageBox::No:
//         return;
//       break;
//       default: // just for sanity
//         return;
//       break;
//       }
//     }else{
//       writ = true;
//     }

//   }
  if ( !logFileNameToSave.isEmpty() ) {
    adifTempFileSave(logFileNameToSave, logbook, true);
  }else {
    //statusBar()->message( i18n("Saving aborted"), 2000 );
  }
}

void Klog::adifTempFileSave(const QString& fn, LogBook lb, bool manualSave){
//adifTempFileSave(logFileNameToSave, logbook, true)
//adifTempFileSave(logFileNameToSave, tempLogbook, false)
//qDebug() << "KLog::adifTempFileSave" << fn;
//cout << "KLog::adifTempFileSave: " << fn << endl;

    //logFileNameToSave = checkExtension(fn);
    QString fileToSave;
    fileToSave = checkExtension(fn);
    QFile file( fileToSave );
    if ( file.open( QIODevice::WriteOnly ) ) {
//		if (manualSave){
        int progresStep = 0;

// 		Q3ProgressDialog progress( i18n("Saving the log..."), i18n("Abort saving"), Klog::number,
//                 this, i18n("progress"), TRUE );

        QProgressDialog progress(i18n("Saving the log..."), i18n("Abort saving"), 0, Klog::number);

//		}

        QTextStream stream( &file );

        Klog::LogBook::iterator it;

//		Klog::LogBook::iterator itEnd;
//		itEnd = lb.end();

        stream << i18n("ADIF v1.0 (some ADIF v2 fields) Export from KLog-") + Klog::KLogVersion + " \nhttp://jaime.robles.es/klog" << "\n<APP_KLOG_NUMBER:" << QString::number( Klog::number ).length() << ">" << QString::number(Klog::number) << i18n("\nLog saved: ") << dateTime.toString("yyyyMMdd") << "-" << dateTime.toString("hhmm") << "\n<PROGRAMID:4>KLOG <PROGRAMVERSION:" + QString::number((Klog::KLogVersion).length()) << ">" << Klog::KLogVersion << " \n<EOH>\n" << endl;

        it = lb.begin();
        while (it != lb.end()){
//		for ( it = lb.begin(); it != lb.end(); ++it ){
            if ( progress.wasCanceled()){
                return;
            }
            stream << "<CALL:" <<((*it).getQrz()).length() << ">" << (*it).getQrz() << " <QSO_DATE:8>" << (*it).getDateTime().toString("yyyyMMdd") << " <TIME_ON:4>" << (*it).getDateTime().toString("hhmm") << " <MODE:" << ((*it).getMode()).length() << ">" << ((*it).getMode()).toUpper() << " <BAND:" << ((*it).getBand()).length() << ">" << ((*it).getBand()).toUpper() << " <RST_SENT:" << QString::number( (*it).getRsttx()).length() << ">" << (*it).getRsttx() << " <RST_RCVD:" << QString::number((*it).getRstrx()).length() << ">" << (*it).getRstrx() << " <QSL_RCVD:1>" << (*it).isQslRec() << " <QSL_SENT:1>" << (*it).isQslSent();
            if ( ((*it).geteQslSent()=='Y') || ((*it).geteQslSent()=='R') || ((*it).geteQslSent()=='Q') || ((*it).geteQslSent()=='I')  )
                stream << " <EQSL_QSL_SENT:1>" << (*it).geteQslRcvd();
            if ( ((*it).geteQslRcvd()=='Y') || ((*it).geteQslRcvd()=='R') || ((*it).geteQslRcvd()=='I') || ((*it).geteQslRcvd()=='V') )
                stream << " <EQSL_QSL_RCVD:1>" << (*it).geteQslRcvd();
            if ( ((*it).getPower()).toInt() > 0) {
                stream << " <TX_PWR:" << ((*it).getPower()).length() << ">" << (*it).getPower();
            }
            if ( ((*it).getrxPower()).toInt() != 0) {
                stream << " <RX_PWR:" << ((*it).getrxPower()).length() << ">" << (*it).getrxPower();
            }

            if ( ((*it).getFreq()).toDouble() != 0.0) {
                stream << " <FREQ:" <<((*it).getFreq()).length() << ">" << (*it).getFreq();
            }
            if (( ((*it).getFreq_RX()).toDouble() != 0.0) && (((*it).getFreq_RX())!=((*it).getFreq())) ){
                stream << " <FREQ_RX:" <<((*it).getFreq_RX()).length() << ">" << (*it).getFreq_RX();
            }
            if ((*it).getIotaNumber()!= 0)
                stream << " <IOTA:" <<((*it).getIota()).length() << ">" << (*it).getIota();
            if ((*it).getMyIotaNumber()!= 0)
                stream << " <MY_IOTA:" <<((*it).getMyIota()).length() << ">" << (*it).getMyIota();
            if ((*it).getLocalAwardNumber() != 0)
                stream << " <STATE:" << ( (*it).getLocalAward()).length() << ">" << (*it).getLocalAward();
            if (((*it).getName()).length()>= 2)
                stream << " <NAME:" <<((*it).getName()).length() << ">" << (*it).getName();
            if (((*it).getContinent()).length()== 2)
                stream << " <CONT:" <<((*it).getContinent()).length() << ">" << (*it).getContinent();
            if ((*it).getDXCC()!= 0)
                stream << " <DXCC:" << QString::number(((*it).getDXCC())).length() << ">" << QString::number((*it).getDXCC());
            if (((*it).getCountry()).length()== 2)
                stream << " <COUNTRY:" <<((*it).getCountry()).length() << ">" << (*it).getCountry();
            if (((*it).getMyCountry()).length()== 2)
                stream << " <MY_COUNTRY:" <<((*it).getMyCountry()).length() << ">" << (*it).getMyCountry();
            if (((*it).getMyName()).length()== 2)
                stream << " <MY_NAME:" <<((*it).getMyName()).length() << ">" << (*it).getMyName();
            if ((*it).getAge()!= -1)
                stream << " <AGE:" << QString::number(((*it).getAge())).length() << ">" << QString::number((*it).getAge());
            if ((*it).getCQz()!= -1)
                stream << " <CQZ:" << QString::number(((*it).getCQz())).length() << ">" << QString::number((*it).getCQz());
            if ((*it).getITUz()!= -1)
                stream << " <ITUZ:" << QString::number(((*it).getITUz())).length() << ">" << QString::number((*it).getITUz());
            if ((*it).getMyCQz()!= -1)
                stream << " <MY_CQ_ZONE:" << QString::number(((*it).getMyCQz())).length() << ">" << QString::number((*it).getMyCQz());
            if ((*it).getITUz()!= -1)
                stream << " <MY_ITU_ZONE:" << QString::number(((*it).getMyITUz())).length() << ">" << QString::number((*it).getMyITUz());
            if (((*it).getMyCity()).length()>= 2)
                stream << " <MY_CITY:" <<((*it).getMyCity()).length() << ">" << (*it).getMyCity();
            if ((*it).getDistance()>= 0)
                stream << " <DISTANCE:" << QString::number(((*it).getDistance())).length() << ">" << QString::number((*it).getDistance());
            if ((*it).getAnt_az()!= -1)
                stream << " <ANT_AZ:" << QString::number(((*it).getAnt_az())).length() << ">" << QString::number((*it).getAnt_az());
            if ((*it).getAnt_el()!= -1)
                stream << " <ANT_EL:" << QString::number(((*it).getAnt_el())).length() << ">" << QString::number((*it).getAnt_el());
            if ( ((*it).getAnt_Path()=='G') || ((*it).getAnt_Path()=='O') || ((*it).getAnt_Path()=='S') || ((*it).getAnt_Path()=='L')  )
                stream << " <ANT_PATH:1>" << (*it).getAnt_Path();
            if ((((*it).getBand_RX()).length()>= 2) && (((*it).getBand_RX())!=((*it).getBand())) && ((*it).getBand_RX() !="NOBAND") )
                stream << " <BAND_RX:" <<((*it).getBand_RX()).length() << ">" << (*it).getBand_RX();
            if ((*it).getA_index()!= -1)
                stream << " <A_INDEX:" << QString::number(((*it).getA_index())).length() << ">" << QString::number((*it).getA_index());
            if ((*it).getK_index()!= -1)
                stream << " <K_INDEX:" << QString::number(((*it).getK_index())).length() << ">" << QString::number((*it).getK_index());
            if ((*it).getMaxBursts()!= -1)
                stream << " <MAX_BURSTS:" << QString::number(((*it).getMaxBursts())).length() << ">" << QString::number((*it).getMaxBursts());
            if ((*it).getMaxBursts()!= -1)
                stream << " <NR_BURSTS:" << QString::number(((*it).getNRBursts())).length() << ">" << QString::number((*it).getNRBursts());
            if ( (*it).getNRPings()> 0)
                stream << " <NR_PINGS:" << QString::number(((*it).getNRPings())).length() << ">" << QString::number((*it).getNRPings());
            if ((*it).getSFI()> 0)
                stream << " <SFI:" << QString::number(((*it).getSFI())).length() << ">" << QString::number((*it).getSFI());
            if (((*it).getMSShower()).length()>= 2)
                stream << " <MS_SHOWER:" <<((*it).getMSShower()).length() << ">" << (*it).getMSShower();
            if (((*it).getSatMode()).length()>= 2)
                stream << " <SAT_MODE:" <<((*it).getSatMode()).length() << ">" << (*it).getSatMode();
            if (((*it).getSatName()).length()>= 2)
                stream << " <SAT_NAME:" <<((*it).getSatName()).length() << ">" << (*it).getSatName();
            if (((*it).getOwnerCall()).length()>= 2)
                stream << " <OWNER_CALLSIGN:" <<((*it).getOwnerCall()).length() << ">" << (*it).getOwnerCall();
            if (((*it).getPfx()).length()>= 2)
                stream << " <PFX:" <<((*it).getPfx()).length() << ">" << (*it).getPfx();
            if (((*it).getPrecedence()).length()>= 2)
                stream << " <PRECEDENCE:" <<((*it).getPrecedence()).length() << ">" << (*it).getPrecedence();

            if (((*it).getMyRig()).length()== 2)
                stream << " <MY_RIG:" <<((*it).getMyRig()).length() << ">" << (*it).getMyRig();
            if (((*it).getMySig()).length()== 2)
                stream << " <MY_SIG:" <<((*it).getMySig()).length() << ">" << (*it).getMySig();
            if (((*it).getMySigInfo()).length()== 2)
                stream << " <MY_SIG_INFO:" <<((*it).getMySigInfo()).length() << ">" << (*it).getMySigInfo();
            if (((*it).getSigInfo()).length()== 2)
                stream << " <SIG_INFO:" <<((*it).getSigInfo()).length() << ">" << (*it).getSigInfo();
            if (((*it).getSig()).length()== 2)
                stream << " <SIG:" <<((*it).getSig()).length() << ">" << (*it).getSig();
            if (((*it).getPublicKey()).length()== 2)
                stream << " <PUBLIC_KEY:" <<((*it).getPublicKey()).length() << ">" << (*it).getPublicKey();

            if (((*it).getContestID()).length()>= 2)
                stream << " <CONTEST_ID:" <<((*it).getContestID()).length() << ">" << (*it).getContestID();
            if (((*it).getClass()).length()>= 2)
                stream << " <CLASS:" <<((*it).getClass()).length() << ">" << (*it).getClass();
            if (((*it).getCheck()).length()>= 2)
                stream << " <CHECK:" <<((*it).getCheck()).length() << ">" << (*it).getCheck();
            if ( ((*it).getStx()!= -1) && ((*it).getStx()!= 0 ) )
                stream << " <STX:" << QString::number((*it).getStx()).length() << ">" << (*it).getStx();
            if (((*it).getSrx()!= -1) && ((*it).getSrx()!= 0 ) )
                stream << " <SRX:" <<QString::number((*it).getSrx()).length() << ">" << (*it).getSrx();
            if ((*it).getStx_string()!= "")
                stream << " <STX_STRING:" <<((*it).getStx_string()).length() << ">" << (*it).getStx_string();
            if ((*it).getSrx_string()!= "")
                stream << " <SRX_STRING:" <<((*it).getSrx_string()).length() << ">" << (*it).getSrx_string();
            if (((*it).getEmail()).length()>= 4)
                stream << " <EMAIL:" <<((*it).getEmail()).length() << ">" << (*it).getEmail();
            if (((*it).getWeb()).length()>= 9)
                stream << " <WEB:" <<((*it).getWeb()).length() << ">" << (*it).getWeb();
            if (((*it).getQth()).length()>= 2)
                stream << " <QTH:" <<((*it).getQth()).length() << ">" << (*it).getQth();
            if (((*it).getOperator()).length()>= 3)
                stream << " <OPERATOR:" <<((*it).getOperator()).length() << ">" << (*it).getOperator();
            if (((*it).getStationCallsign()).length()>= 3)
                stream << " <STATION_CALLSIGN:" <<((*it).getStationCallsign()).length() << ">" << (*it).getStationCallsign();
            if (((*it).getContactedOP()).length()>= 3)
                stream << " <CONTACTED_OP:" <<((*it).getContactedOP()).length() << ">" << (*it).getContactedOP();
            if (((*it).getEQCall()).length()>= 3)
                stream << " <EQ_CALL:" <<((*it).getEQCall()).length() << ">" << (*it).getEQCall();
            if ( locator.isValidLocator((*it).getLocator()) )
                stream << " <GRIDSQUARE:" <<((*it).getLocator()).length() << ">" << (*it).getLocator();
            if (locator.isValidLocator((*it).getMyLocator())){
                stream << " <MY_GRIDSQUARE:" <<((*it).getMyLocator()).length() << ">" << (*it).getMyLocator();
            }

            if (((*it).geteQslRecDate()).isValid() ){
                stream << " <EQSL_QSLRDATE:8>" << (*it).geteQslRecDate().toString("yyyyMMdd");
            }
            if (((*it).geteQslSenDate()).isValid() ){
                stream << " <EQSL_QSLSDATE:8>" << (*it).geteQslSenDate().toString("yyyyMMdd");
            }

            if  ((*it).sentTheQSL() && (((*it).getQslSenDate()).isValid() )){
                stream << " <QSLSDATE:8>" << (*it).getQslSenDate().toString("yyyyMMdd");
            }

            if ((*it).gotTheQSL()  && (((*it).getQslRecDate()).isValid() )){
                stream << " <QSLRDATE:8>" << (*it).getQslRecDate().toString("yyyyMMdd");
            }

            if (((*it).getQslVia()).compare("No QSL") == 0){ //Write nothing
            }else{
                if ((*it).getQslVia() == "Manager"){ //If there is a manager
                    if (((*it).getQslManager()).length()>0){
                        stream << " <QSL_VIA:" <<((*it).getQslManager()).length() << ">" << (*it).getQslManager();
                    }
                }else{//There is no manager but there is QSL via
                    if ((((*it).getQslVia()).length())>0)
                    stream << " <QSL_VIA:" <<((*it).getQslVia()).length() << ">" << (*it).getQslVia();
                }
            }



            if ( ((*it).getQslInfo().length()) >= 1){
                stream << " <QSLMSG:" <<((*it).getQslInfo()).length() << ">" << (*it).getQslInfo();
            }
            if (((*it).getComment()).length()>0)
                stream << " <COMMENT:" << ((*it).getComment()).length() << ">" << (*it).getComment();
            if (((*it).getAddress()).length()>0)
                stream << " <ADDRESS:" << ((*it).getAddress()).length() << ">" << (*it).getAddress();
            if (((*it).getMyState()).length()>0)
                stream << " <MY_STATE:" << ((*it).getMyState()).length() << ">" << (*it).getMyState();
            if (((*it).getMyStreet()).length()>0)
                stream << " <MY_STREET:" << ((*it).getMyStreet()).length() << ">" << (*it).getMyStreet();
            if (((*it).getMyPostalCode()).length()>0)
                stream << " <MY_POSTAL_CODE:" << ((*it).getMyPostalCode()).length() << ">" << (*it).getMyPostalCode();

            if (((*it).getARRLSect()).length()>=2)
                stream << " <ARRL_SECT:" << ((*it).getARRLSect()).length() << ">" << (*it).getARRLSect();



            stream << " <EOR>" << endl;


//	if (manualSave){
            progresStep++;
            if (showProgressDialog){
                progress.setValue( progresStep );
                qApp->processEvents();
                if ( progress.wasCanceled())
                    return;
            }
//	}

++it;
        if((it) != lb.end()){

        }

        if(it != lb.end()){

        }
        } // Closes the FOR
        file.close();
        if (manualSave){
            templogbook.clear(); // We have saved the whole log, so the temp has also been saved.
            QFile fileTemp( tempLogFile );
            if (fileTemp.remove()){
                //cout << "Temp file deleted" << endl;
            }
            needToSave = false;
        }
    } // Closes the IF
//  	needToSave = false;
}

// This function checks the filename.extension for the ADI
QString Klog::checkExtension(QString extension){
  //qDebug() << "KLog::checkExtension" << extension;
    int result;
        result = extension.indexOf(".adi", -4, Qt::CaseInsensitive);
    if (result < 0)
        extension = extension + ".adi";
//cout << "KLog::checkExtension returns: " << extension << endl;
    return extension;
};

void Klog::addQSOToLog(){
//qDebug() << "KLog::addQSOToLog" << QString::number(qso.getNumb()) << " / " << qso.getQrz();
    Klog::needToSave = true;
    logbook.append(qso);
    enti = world.findEntity(qso.getQrz());
    if (enti != 0){
        dxcc.workedString(enti, qso.getBand(), qso.getMode());
        waz.workedString(world.getCqzFromCall(qso.getQrz()), qso.getBand(), qso.getMode() );
        if (qso.gotTheQSL()){
            dxcc.confirmedString(enti, (qso.getBand()).toUpper() ,  (qso.getMode()).toUpper());
            waz.confirmedString( world.getCqzFromCall(qso.getQrz()), (qso.getBand()).toUpper() ,  (qso.getMode()).toUpper());
//			Klog::confirmed++;
        }
    }
    qso.clearQso();
}

int Klog::getProgresStepForDialog(int totalSteps){
  //qDebug() << "KLog::getProgresStepForDialog";
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

    qsoLine = qsoLine.simplified().toUpper();

    intAux = qsoLine.count('<');			// How many fields has the line?
    fields = qsoLine.split("<", QString::SkipEmptyParts);
    //fields = QStringList::split('<', qsoLine );	// Split the line in fields
    for (int a = 0; a < intAux; a++) { // It was to aux-
        if (fields[a].count(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
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
                qso.setQrz(theData.toUpper());
                enti = world.findEntity(theData.toUpper());
                haveAllMandatoryFields[0] = true;
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
            qso.setBand(theData.toUpper());
            haveAllMandatoryFields[3] = true;
        }else if (adifTab == "FREQ"){  // We are still not ready for freq reading
            if (haveAllMandatoryFields[3] == false) {
// 		// We translate to MHz if needed before converting to band
// 				qso.setBand(adif.freq2Band(QString::number((((theData.toUpper())).toDouble())/1000)));
                qso.setBand(adif.freq2Band(QString::number((((theData.toUpper())).toDouble()))));
            }else{
            //TODO: I SHOULD CHECK IF THE BAND PREVIOUSLY READ AND THE PROVIDED BY FREQ ARE THE SAME. If not... a message should be issued.
            }
// 			qso.setFreq(QString::number((((theData.toUpper())).toDouble())/1000));
            qso.setFreq(QString::number((((theData.toUpper())).toDouble())));
            haveAllMandatoryFields[3] = true;
        }else if (adifTab == "FREQ_RX"){
            qso.setBand_RX(adif.freq2Band(QString::number((((theData.toUpper())).toDouble()))));
            qso.setFreq_RX(QString::number((((theData.toUpper())).toDouble())));
        }else if (adifTab == "MODE"){
            qso.setMode(theData.toUpper());
            haveAllMandatoryFields[4] = true;
        }else if (adifTab == "TX_PWR"){
            qso.setPower(theData.toUpper());
        }else if (adifTab == "RX_PWR"){
            qso.setrxPower(theData.toUpper());
        }else if (adifTab == "COMMENT"){
            qso.setComment(theData);
        }else if (adifTab == "ADDRESS"){
            qso.setAddress(theData);
        }else if (adifTab == "MY_POSTAL_CODE"){
            qso.setMyPostalCode(theData);
        }else if (adifTab == "MY_STREET"){
            qso.setMyStreet(theData);
        }else if (adifTab == "MY_STATE"){
            qso.setMyState(theData);
        }else if (adifTab == "ARRL_SECT"){
            qso.setARRLSect(theData);
        }else if (adifTab == "CONTEST_ID"){
            qso.setContestID(theData);
        }else if (adifTab == "CHECK"){
            qso.setCheck(theData);
        }else if (adifTab == "CLASS"){
            qso.setClass(theData);
        }else if (adifTab == "EMAIL"){
            qso.setEmail(theData);
        }else if (adifTab == "STX"){
            qso.setStx(theData.toInt());
        }else if (adifTab == "SRX"){
            qso.setSrx(theData.toInt());
        }else if (adifTab == "STX_STRING"){
            qso.setStx_string(theData);
        }else if (adifTab == "SRX_STRING"){
            qso.setSrx_string(theData);
        }else if (adifTab == "WEB"){
            qso.setWeb(theData);
        }else if (adifTab == "DXCC"){
            qso.setDXCC((theData).toInt());
        }else if (adifTab == "COUNTRY"){
            qso.setCountry(theData);
        }else if (adifTab == "MY_COUNTRY"){
            qso.setMyCountry(theData);
        }else if (adifTab == "MY_NAME"){
            qso.setMyName(theData);
        }else if (adifTab == "AGE"){
            qso.setAge((theData).toInt());
        }else if (adifTab == "CQZ"){
            qso.setCQz((theData).toInt());
        }else if (adifTab == "ITUZ"){
            qso.setITUz((theData).toInt());
        }else if (adifTab == "MY_CQ_ZONE"){
            qso.setMyCQz((theData).toInt());
        }else if (adifTab == "MY_ITU_ZONE"){
            qso.setMyITUz((theData).toInt());
        }else if (adifTab == "DISTANCE"){
            qso.setDistance((theData).toInt());
        }else if (adifTab == "ANT_AZ"){
            if ((((theData).toInt())>=0) && (((theData).toInt())<=360)){
                qso.setAnt_az((theData).toInt());}
        }else if (adifTab == "ANT_EL"){
            if ((((theData).toInt())>=0) && (((theData).toInt())<=90)){
                qso.setAnt_el((theData).toInt());}
        }else if (adifTab == "ANT_PATH"){
            qso.setAnt_Path(theData[0]);
        }else if (adifTab == "BAND_RX"){
            qso.setBand_RX(theData.toUpper());
        }else if (adifTab == "A_INDEX"){
            qso.setA_index((theData).toInt());
        }else if (adifTab == "K_INDEX"){
            qso.setK_index((theData).toInt());
        }else if (adifTab == "MAX_BURSTS"){
            qso.setMaxBursts((theData).toInt());
        }else if (adifTab == "NR_BURSTS"){
            qso.setNRBursts((theData).toInt());
        }else if (adifTab == "NR_PINGS"){
            qso.setNRPings((theData).toInt());
        }else if (adifTab == "SFI"){
            qso.setSFI((theData).toInt());
        }else if (adifTab == "MS_SHOWER"){
            qso.setMSShower(theData);
        }else if (adifTab == "SAT_MODE"){
            qso.setSatMode(theData);
        }else if (adifTab == "SAT_NAME"){
            qso.setSatName(theData);
        }else if (adifTab == "MY_RIG"){
            qso.setMyRig(theData);
        }else if (adifTab == "MY_SIG"){
            qso.setMySig(theData);
        }else if (adifTab == "MY_SIG_INFO"){
            qso.setMySigInfo(theData);
        }else if (adifTab == "SIG"){
            qso.setSig(theData);
        }else if (adifTab == "PUBLIC_KEY"){
            qso.setPublicKey(theData);
        }else if (adifTab == "SIG_INFO"){
            qso.setSigInfo(theData);
        }else if (adifTab == "OWNER_CALL"){
            qso.setOwnerCall(theData);
        }else if (adifTab == "PFX"){
            qso.setPfx(theData);
        }else if (adifTab == "PRECEDENCE"){
            qso.setPrecedence(theData);
        }else if (adifTab == "QSLSDATE"){
            datesString = theData;
            datesString.insert(4,'-');
            datesString.insert(7,'-');
        }else if (adifTab == "QSLRDATE"){
            daterString = theData;
            daterString.insert(4,'-');
            daterString.insert(7,'-');
        }else if (adifTab == "QSL_RCVD"){
            qso.QslRec(theData[0]);
        }else if (adifTab == "QSL_SENT"){
            qso.QslSent(theData[0]);
        }else if (adifTab == "EQSL_QSLRDATE"){
            theData.insert(4,'-');
            theData.insert(7,'-');
            qso.seteQslRcvd('Y');
            qso.seteQslRecDateOn(QDate::fromString(theData, Qt::ISODate));
        }else if (adifTab == "EQSL_QSLSDATE"){
            theData.insert(4,'-');
            theData.insert(7,'-');
            qso.seteQslSent('Y');
            qso.seteQslSenDateOn(QDate::fromString(theData, Qt::ISODate));
        }else if (adifTab == "EQSL_QSL_RCVD"){
            qso.seteQslRcvd(theData[0]);
        }else if (adifTab == "EQSL_QSL_SENT"){
            qso.seteQslSent(theData[0]);
        }else if (adifTab == "QSL_VIA"){ //Manager?
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
        }else if (adifTab == "MY_GRIDSQUARE") {
            if(locator.isValidLocator(theData)){
                qso.setMyLocator(theData);
            }else if ((locator.isValidLocator(getMyLocator()) ) && (addingLog) ){	// This is for the case of a log addition (i.e. comming from an expedition)
                qso.setMyLocator(getMyLocator());
            }
        }else if (adifTab == "IOTA"){
            qso.setIota(theData);
        }else if (adifTab == "MY_IOTA"){
            qso.setMyIota(theData);
        }else if (adifTab == "MY_CITY"){
            qso.setMyCity(theData);
        }else if ((adifTab == "STATE")||(adifTab == "VE_PROV")){
            // We need the long name of the entity but also the reference number
            // The ref number is just for processing the filesave and so on...
            if (world.findEntity(qso.getQrz())!=0){
                award = awards.getAwardFor(world.getPrefix(qso.getQrz()));

                if (award.getReferenceNumber(awardsComboBox->currentText())){
                    if (award.isValidReference(theData)){
                        // << "Valid Reference" << endl;
                        qso.setLocalAward(theData);
                        qso.setLocalAwardNumber(award.getReferenceNumber(theData));
                    }else{
                        //cout << "Non-Valid Reference" << endl;
                    }
                }
            }else{
// 				cout << "ProcessLogLine == 0 for " << qso.getQrz() << endl;
            }
        }else if ((adifTab == "OPERATOR")||(adifTab == "GUEST_OP")){
            qso.setOperator(theData);
        }else if (adifTab == "STATION_CALLSIGN"){
            qso.setStationCallsign(theData);
        }else if (adifTab == "CONTACTED_OP"){
            qso.setContactedOP(theData);
        }else if (adifTab == "EQ_CALL"){
            qso.setEQCall(theData);
        }else if (adifTab == "NAME"){
            qso.setName(theData);
        }else if (adifTab == "CONT"){
            qso.setContinent(theData);
        }else if (adifTab == "QTH"){
            qso.setQth(theData);
        }else if (adifTab == "EOR>"){
        // We have read the EOR so if we need all the mandatory qsoLine, we add the QSO and
        // prepare to read the next QSO, if we do not have all the mandatory qsoLine we simply
        // prepare to read the next QSO.
            if (haveAllTheFields()){
                dateString = dateString+"T"+timeString;
                //cout << "KLog processLine Date: |" << dateString <<"|"<< endl;
                if ( !(QDateTime::fromString(dateString, Qt::ISODate)).isValid() ) {
                    //cout << "INVALID DATE-3: " << dateString << endl;
                }else{
                    qso.setDateTime(QDateTime::fromString(dateString, Qt::ISODate));
                }

                if (qso.gotTheQSL() && (QDate::fromString(daterString, Qt::ISODate)).isValid() ){
                    //qso.QslRec('Y');
                    qso.setQslRecDateOn(QDate::fromString(daterString, Qt::ISODate));

                }
                if (qso.sentTheQSL() && ((QDate::fromString(datesString, Qt::ISODate)).isValid())){
                    //qso.QslSent('Y');
                    qso.setQslSenDateOn(QDate::fromString(datesString, Qt::ISODate));

                }
                if (!qslViac){
                    qso.setQslVia("BUREAU");
                    qslViac = false;
                }
                        // The following is mainly for importing logs. If the imported log was done
                        // using a different call than the actual log.

                if ( (operatorStringAux.length()>2) && (!operatorStringAux.isEmpty())){
                    qso.setStationCallsign(operatorStringAux);
                }

/*				if (((qso.getOperator()).length()>2) &&( (operatorStringAux.length()<2) || (!operatorStringAux.isEmpty())) ){
                    qso.setOperator(operatorStringAux);
                }*/
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
        }
    }
}

void Klog::adifReadLog(const QString& tfileName){
//cout << "KLog::adifReadLog" << endl;
    QFile file( tfileName );
    int totalQsos = 0; // QSOs in the log to be read
//	bool qslViac = false;
    int progresStep = 0;
    QProgressDialog progress(i18n("Reading the log..."), i18n("Abort reading"), 0, totalQsos);

    QString progressLabel;
    QTextStream stream( &file );

    int len; // for the tab length
    QString data = "";
    int aux = 0;
    bool inHeader = true;

    if ( file.open( QIODevice::ReadOnly ) ) {
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
                        data = data.simplified().toUpper();
                        aux = data.count('<');
                        QStringList fieldss = data.split("<", QString::SkipEmptyParts);
                        //QStringList fieldss = QStringList::split('<', data );
                        for (int a = 0; a < aux; a++){  //Dirty way to process just one field  	AUX is = 0 at this moment!!
                            if (fieldss[a].count("APP_KLOG_NUMBER") != 0){
                                adifTab = fieldss[a].section(':',0,0);
                            }
                            len = (fieldss[a].section(':',1)).section('>',0,0).toInt();
                            theData = ((fieldss[a].section(':',1)).section('>',1));
                            theData.truncate(len);
                            if (adifTab == "APP_KLOG_NUMBER"){
                                totalQsos = theData.toInt();
                                progress.setMaximum(totalQsos);
                                adifTab = ""; // To avoid repeating this step
                            }
                        }
                    }
                }
                if (( data.toUpper().count("<EOH>") != 0 ) ){
                // We have finished to read the Header or the file does not have any header
                    aux = 1;
                    inHeader = false;
                }
            } // Closes the while
        }else{ // LogBook
            if (((data.toUpper()).count("<EOR>"))>=0){
                //HACK: Working with files with more than one line per QSO.
                /*********************************************************/
                while (((data.toUpper()).count("<EOR>")==0)&&(!data.isEmpty())){
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
            if ( ((data.toUpper()).count("<EOR>")) == 0){
                //HACK: Working with files with more than one line per QSO.
                /*********************************************************/
                while (((data.toUpper()).count("<EOR>")==0)&&(!data.isEmpty()) ){
                    data = data + "\\n" + stream.readLine();
                }
                /*********************************************************/
            }
            processLogLine (data);
            if (showProgressDialog){
                progresStep++;
                if ( (number % getProgresStepForDialog(totalQsos) )== 0){ // To update the speed i will only show the progress once each 25 QSOs
                    progress.setValue( progresStep );
                    qApp->processEvents();
                                        progressLabel = tr("Adding QSO#: ") + QString::number(number) + " / " + (QString::number(totalQsos));
                    progress.setLabelText(progressLabel);
                }
                if ( progress.wasCanceled()){
                    return;
                }
            }
        } // Closes the while
        file.close();
        progress.close();
    } // Closes the if from the file (file could not be opened)
    //TODO Add the support to the checking for data if previously worked.
//cout << "KLog adifreadlog antes de terminar, complete with previous" << endl;
    if (completeWithPrevious){
        getAllTheCallsFromLog();
        //completeThePreviouslyWorked();
    }
// cout << "KLog adifreadlog antes de terminar, DESPUES complete with previous" << endl;
    needToSave = false;
    showLogList();
    showAwardsNumbers();

//dxcc.printWorkdStatus();
    fillDXCCList();
}

void Klog::toEditQso(){
//cout << "KLog::toEditQSO" << endl;
    Klog::modify = true;
    Klog::j = qso.getNumb();
    qsoDateEdit->setDate(qso.getDate()); // date
    qsoTimeEdit->setTime(qso.getTime()); // time
    //qsoDateTime->setDateTime(qso.getDateTime()); // date

    qrzLineEdit->setText(qso.getQrz());
    RSendBox->setValue( (QString::number(qso.getRsttx()).at(0)).digitValue() );
    SSendBox->setValue( (QString::number(qso.getRsttx()).at(1)).digitValue() );
    TSendBox->setValue( (QString::number(qso.getRsttx()).at(2)).digitValue() );
    RRecBox->setValue( (QString::number(qso.getRstrx()).at(0)).digitValue() );
    SRecBox->setValue( (QString::number(qso.getRstrx()).at(1)).digitValue() );
    TRecBox->setValue( (QString::number(qso.getRstrx()).at(2)).digitValue() );
        modeComboBox->setItemText(0, qso.getMode());
        bandComboBox->setItemText(0, qso.getBand());
    powerSpinBox->setValue((qso.getPower()).toInt());
    remarksTextEdit->setText(returnLines(qso.getComment()));
    QSLSentcheckBox->setChecked(qso.sentTheQSL());
    QSLReccheckBox->setChecked(qso.gotTheQSL());
    QSLSentdateEdit->setDate(qso.getQslSenDate());
    QSLRecdateEdit->setDate(qso.getQslRecDate());
    freqlCDNumber->display(qso.getFreq());

    if ((qso.getQth()).length() >=2)
        qthkLineEdit->setText(qso.getQth());
    else
    qthkLineEdit->clear();
    if ((qso.getOperator()).length() >=3){
        operatorLineEdit->setText(qso.getOperator());
    }else{
        operatorLineEdit->clear();
    }

    if ((qso.getStationCallsign()).length() >=3){
        stationCallsignLineEdit->setText(qso.getStationCallsign());
    }else{
        stationCallsignLineEdit->clear();
    }

    if ((qso.getName()).length() >=2)
        namekLineEdit->setText(qso.getName());
    else
    namekLineEdit->clear();
    dxLocator = qso.getLocator();

    locatorLineEdit->setText(dxLocator);
    slotLocatorChanged();

    myLocatorLineEdit->setText(qso.getMyLocator());

    //If we have sent the QSL
    if (QSLSentcheckBox->isChecked()){
        (QSLSentdateEdit)->setEnabled(true);
        (QSLSentdateEdit)->setDate(qso.getQslSenDate());
    }else{
        (QSLSentdateEdit)->setEnabled(false);
    (QSLSentdateEdit)->setDate(QDate::currentDate());
    }
    // If we have received the QSL
    if (QSLReccheckBox->isChecked()){
        (QSLRecdateEdit)->setEnabled(true);
        qslRec =  qso.getQslRecDate(); //We need the qslRec in slotQslRecvBoxChanged
        (QSLRecdateEdit)->setDate(qslRec);
    }else{
        (QSLRecdateEdit)->setEnabled(false);
    (QSLRecdateEdit)->setDate(QDate::currentDate());
    }
    //Now the QSl info information
        QSLcomboBox->setItemText(0, qso.getQslVia());
    if ((qso.getQslVia()).compare("No QSL") == 0){
        qslVialineEdit->setDisabled(true);
        //    QSLInfotextEdit->setDisabled(true);
        qslVialineEdit->clear();
        //    QSLInfotextEdit->clear();
        }else{
        if ((qso.getQslVia()).compare("Manager") == 0){
            qslVialineEdit->setEnabled(true);
            qslVialineEdit->setText(qso.getQslManager());
            }else{
            qslVialineEdit->setDisabled(true);
        }
        //    QSLInfotextEdit->setEnabled(true);
        QSLInfotextEdit->setPlainText(returnLines(qso.getQslInfo()));
    }
    // We need enti in other slots
    enti = 0;
    if (((qrzLineEdit->text()).toUpper()).length() != 0){
        enti = world.findEntity((qrzLineEdit->text()).toUpper());
    }
    // IOTA
    prepareIOTAComboBox(enti);
    if (qso.getIotaNumber() != 0) {
        i = 0;
        i = adif.continent2Number((qso.getIotaContinent()));

        iotaComboBox->setCurrentIndex(i);
        //iotaIntSpinBox->setEnabled(true);
        iotaIntSpinBox->setValue(qso.getIotaNumber());
    }
    prepareAwardComboBox(enti);
    if (award.getReferenceNumber(qso.getLocalAward()) != -1 ) {
        awardsComboBox->setCurrentIndex(award.getReferenceNumber(qso.getLocalAward()));
//		awardsComboBox->setCurrentIndex( qso.getLocalAward() );
        awardsComboBox->setEnabled(true);

    }
    showWhere(enti);
}


QString Klog::returnLines(const QString& tword){
//cout << " - Class KLog::returnLines" << endl;
    aux = tword;
    if (aux.count("\\N")>0){
        aux.replace("\\N", QChar('\n'));
    }
    if (aux.count("\\n")>0){
        aux.replace("\\n", QChar('\n'));
    }

    //cout << "KLog::returnLines: " << tword << " - " << aux << endl;
    return aux;


}

//TODO: DELETED FOR QT4 MIGRATION: Add the rightbutton
// void Klog::slotQsoRightButtonFromLog(QTreeWidgetItem * item, const QPoint &p){
// //cout << "KLog::slotQsoRightButtonFromLog" << endl;
// 	if (item){
// 		Klog::j = (item->text(0)).toInt(); // j is the QSO number
// 		showMenuRightButton(Klog::j, p);
// 	}
// }
//
// void Klog::slotQsoRightButtonFromSearch(QTreeWidgetItem * item, const QPoint &p){
// //Maybe This could be deleted and use the previous "slotQsoRightButtonFromLog" to perform
// // this actions...
// //cout << "KLog::slotQsoRightButtonfromSearch" << endl;
// 	if (item){
// 		Klog::j = (item->text(7)).toInt(); // j is the QSO number
// 		showMenuRightButton(Klog::j, p);
// 		slotSearchButton();  // This is for updating the Entity state after QSLing.
// 	}
// }

void Klog::showMenuRightButton(int qqso, const QPoint &p){
//cout << "KLog::showMenuRightButton" << endl;


// COMMENTED TO EASE THE QT4 MIGRATION
//TODO:
      QMessageBox msgBox;
      msgBox.setText(i18n("KLog message:"));
      QString str = i18n("This function (slotAddLog) has been MODIFIED to help the QT4 migration.\nIt will be restored ASAP");
      msgBox.setInformativeText(str);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.exec();


// 	if (qqso >= 0){
// 		qso = getByNumber(qqso);
// 		kk = world.findEntity( qso.getQrz() );
// 		entityState(kk);
//
// //		KMenu *qsoMenu = new KMenu( i18n ("QSO menu"), Ui_klogui );
//
// 		qsoMenu = new QMenu(i18n("QSO menu"), this);
//
//
// //		qsoMenu->insertItem( *editdeletePixMap, i18n("Delete"), this, SLOT( slotQsoDelete() ), CTRL + Key_D );
// //		qsoMenu->insertSeparator();
// 		if (( !qso.gotTheQSL() ) && ( !qso.sentTheQSL() )){
// //			qsoMenu->insertItem( *qslRecPixMap, i18n("QSL Recv and Sent"), this, SLOT( slotQSLRecSent() ), CTRL + Key_B );
//
// 		  //TODO: To check if a call to showAwardsNumbers() is needed here
//
// 		}
// 		if ( !qso.gotTheQSL() ){
// 			//qsoMenu->insertItem( *qslRecPixMap, i18n("QSL Received"), this, SLOT( slotQSLRec() ), CTRL + Key_R );
// //		  qsoActionsGroup->addAction(recQSOAct);
//
// 		  showAwardsNumbers();
// 		}
// 		if ( !qso.sentTheQSL() ){
// //			qsoMenu->insertItem( *qslSenPixMap, i18n("QSL Sent"), this, SLOT( slotQSLSent() ), CTRL + Key_S );
// //		  qsoActionsGroup->addAction(senQSOAct);
//
// 		}else{
// 		}
//
// 		qsoMenu->exec(event->globalPos());
//
//
// //		qsoMenu->insertSeparator();
// //		qsoMenu->exec(p);
// 	}
}

// void Klog::showMenuRightButtoncreateActions(){
// //cout << "KLog::showMenuRightButtoncreateActions" << endl;
//
// //TODO: Add the shortcut ( QAction::setShorCut()  )
//   delQSOAct = new QAction(i18n("&Delete"), this);
//     //newAct->setShortcuts(QKeySequence::New);
//   delQSOAct->setStatusTip(i18n("Delete a QSO"));
//   connect(delQSOAct, SIGNAL(triggered()), this, SLOT(slotQsoDelete()));
//
//   recSenQSOAct = new QAction(i18n("&Recv and Sent"), this);
//   //newAct->setShortcuts(QKeySequence::New);
//   recSenQSOAct->setStatusTip(i18n("QSL Recv and Sent"));
//   connect(recSenQSOAct, SIGNAL(triggered()), this, SLOT(slotQSLRecSent()));
//
//   recQSOAct = new QAction(i18n("&Recv QSL"), this);
//   //newAct->setShortcuts(QKeySequence::New);
//   recQSOAct->setStatusTip(i18n("QSL Received"));
//   connect(recQSOAct, SIGNAL(triggered()), this, SLOT(slotQSLRec()));
//
//   senQSOAct = new QAction(i18n("&Sent QSL"), this);
//   //newAct->setShortcuts(QKeySequence::New);
//   senQSOAct->setStatusTip(i18n("QSL Sent"));
//   connect(senQSOAct, SIGNAL(triggered()), this, SLOT(slotQSLSent()));
// }
//
// void Klog::showMenuRightButtoncreateMenus(){}
// void Klog::showMenuRightButtoncontextMenuEvent(QContextMenuEvent *event){
//      QMenu menu(this);
//      menu.addAction(delQSOAct);
//      menu.addAction(recSenQSOAct);
//      menu.addAction(recQSOAct);
//      menu.addAction(senQSOAct);
//      menu.exec(event->globalPos());
//
// qsoMenu->addAction(delQSOAct);
// qsoMenu->addAction(recSenQSOAct);
// qsoMenu->addAction(recQSOAct);
// qsoMenu->addAction(senQSOAct);
//
// }

void Klog::slotQsoSelectedForEdit(QTreeWidgetItem *item, int column){
    qDebug() << "KLog::slotQsoSelectedForEdit";
    if (item){
        slotClearBtn();
        Klog::j = (item->text(0)).toInt(); // j is the QSO number from the loglist
        qso = getByNumber(Klog::j);
        toEditQso();
        okBtn->setText(i18n("Modify"));
        clearBtn->setText(i18n("Cancel"));
        Klog::callFound = false;
        Klog::entiBak = 0;
        Klog::callLen = 0;
        Klog::callLenPrev = 0;
        Klog::lastDelete = false;
    }
}

void Klog::slotQsoSelected(QTreeWidgetItem* item){
//cout << "KLog::slotQsoSelected" << endl;
    if (item){
        kk = world.findEntity(item->text(3).toUpper());
        // kk this time is the Entity of the call selected
//		if ((kk !=0) && (kk !=-1)){
        if (kk>0){
            entityState(kk);
        }
        showWhere(kk); // We run the showWhere to change the box if the QSO is from an unknown entity
    }
}


// This slot just change the text from the OKButton to "Modify"
void Klog::slotModifyBtn(){
//cout << "KLog::slotModifyBtn" << endl;
        okBtn->setText(tr("Modify"));
}

Qso Klog::getByNumber(const int n){
/***********************************************************
* We receive a number and we try to get the Qso returning *
* a pointer to it                                          *
************************************************************
*/
//cout << "KLog::getByNumber: " << QString::number(n) << endl;
    Klog::LogBook::iterator iter;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
        if (n == (*iter).getNumb() ){
            return (*iter);
        }
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
    Klog::LogBook::iterator iter;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
        if (tqrz.compare(((*iter).getQrz())) == 0)
            return (*iter);
        }
    //Entity not found
    return (*iter);
}

void Klog::showQso(){
// This shows the data in the QTreeWidget (the botton block)
// The "modify" is still missing
// I have to look for the QSO if modifying
//cout << "KLog::showQso" << endl;
    if (!modify){
        QTreeWidgetItem * item = new QTreeWidgetItem( logTreeWidget, 0 );
        item->setText( 0, getNumberString(qso.getNumb())  );
    }
    // This does not seem to be needed. QSO already modified and item added to the table
    /* else {
        qDebug() << "MODIFY!";
        QList<QTreeWidgetItem *> item = logTreeWidget->findItems(QString::number(Klog::j, 7), Qt::MatchExactly, 0);
        qDebug() << "MODIFYing before IF!" << item.count() << QString::number(Klog::j, 7);
        qDebug() << item.at(0)->text(0);
        if (item.at(0)){
            qDebug() << "MODIFYing IF!" << item.count() << QString::number(Klog::j);
            qDebug() << item.at(0)->text(0);
            //item->setText( 0,  QString::number(Klog::j) );
            item[0]->setText( 0, getNumberString(Klog::j)  );
            qDebug() << "MODIFYing IF!";
            item[0]->setText( 1, qso.getDateTime().toString("yyyy-MM-dd") );
            qDebug() << "MODIFYing IF!";
            item[0]->setText( 2, qso.getDateTime().toString("hh:mm") );
            qDebug() << "MODIFYing IF!";
            item[0]->setText( 3, qso.getQrz().toUpper() );
            qDebug() << "MODIFYing IF!";
            item[0]->setText( 4, QString::number(qso.getRsttx()) );
            item[0]->setText( 5, QString::number(qso.getRstrx()) );
            item[0]->setText( 6, qso.getBand() );
            item[0]->setText( 7, qso.getMode() );
            item[0]->setText( 8, qso.getPower() );
            item[0]->setText( 9, qso.getComment() );
        }
    }*/
}

void Klog::showAwardsNumbers(){
/*TODO: Check the "confirmed"variable that are the confirmed QSO.
To Check:
    When a QSO is confirmed
    When the log is read
    When a QSO is deleted
    When the status of awards is read
*/
//cout << "KLog::showAwardNumbers" << endl;
    workedQSOlCDNumber->display(Klog::number);
    //confirmedQSOlCDNumber->display(Klog::confirmed);
    confirmedQSOlCDNumber->display(howManyConfirmedQSO());
    //confirmedQSOlCDNumber->display(dxcc.howManyConfirmed());
    DXCCworkedlCDNumber->display(dxcc.howManyWorked());
    DXCClCDNumber->display(dxcc.howManyConfirmed());
    WAZworkedlCDNumber->display(waz.howManyWorked());
    WAZlCDNumber->display(waz.howManyConfirmed());
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
                i18n("The log has been changed since the last save."),
                i18n("Save Now"), i18n("Cancel"), i18n("Continue anyway"), 0, 1 ) ) {
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
    if (needToSave){
        if(!didISave()) // If i did not save, i cannot exit without saving
            return;
    }
    close();
}

void Klog::fileNew(){
//cout << "KLog::fileNew" << endl;
    operatorStringAux = "";
    Klog::number = 0;
    //Klog::confirmed = 0;
    klogDir = QDir::homePath()+"./klog";
    logFileNameToSave = "klog.adi";
    slotClearBtn();
    modify = false;
    //  prefixFound = false;
    logbook.clear();
    templogbook.clear();
    clearAwards();
    //ea_tpea.clear();
    logTreeWidget->clear();
    showAwardsNumbers();

}

void Klog::fileOpen(){
    if (needToSave){
        if(!didISave()){ // If i did not save, i cannot exit without saving
            return;
        }
    }

    QString logFileNameToOpen = QFileDialog::getOpenFileName(this, tr("Open File"), klogDir, i18n("ADIF files (*.adi)"));
    if ((logFileNameToOpen.isNull() ) || (logFileNameToOpen.isEmpty()))
      return;
    clearAwards();
    if(!logbook.empty()){ //If we are using a log we have to clear it
        logbook.clear();
        templogbook.clear();
        logTreeWidget->clear();
    }
    Klog::number = 0;	// We have to reset the counter!
    adifReadLog(logFileNameToOpen);
    logFileNameToSave = logFileNameToOpen;
    needToSave = false;
}

void Klog::slotQslSentBoxChanged(){
//  qDebug() << "KLog::slotQslSentBoxChanged";
    if (enti == 0)
        return;
    if (QSLSentcheckBox->isChecked()){
        if ((!modify) || (!qso.sentTheQSL())){
            qslSen = QDate::currentDate();
        }
        qso.QslSent('Y');
        (QSLSentdateEdit)->setEnabled(true);
        (QSLSentdateEdit)->setDate(qslSen);
    }else{
        qso.QslSent('N');
/*		//TODO: This date is not valid, it is out of range!
        if ( !(QDateTime::fromString("0000-00-00", Qt::ISODate)).isValid() ) {
//			cout << "FECHA NO VALIDA-6: (todo ceros) " << dateString << endl;
            qslSen = QDate::fromString("0000-00-00",Qt::ISODate);
        }*/
        (QSLSentdateEdit)->setDate(qslSen);
        (QSLSentdateEdit)->setEnabled(false);
    }
    if ( (!modify) && (qso.sentTheQSL()))
        qso.QslSent('Y');
        if (qslSen.isValid()){
            qso.setQslSenDateOn(qslSen);
        }
}

void Klog::slotQslRecvBoxChanged(){
//cout << "KLog::slotQslRecvBoxChanged" << endl;
//	wasConfirmed = qso.gotTheQSL(); // Was this QSO previously confirmed
    if ((enti == 0) | ( (qrzLineEdit->text()).length() == 0)){
        return;
    }
  // If we are modifying the date should be the previous one if existed
  //if (!modify)
    qslRec = QDate::currentDate();
    if (QSLReccheckBox->isChecked()){ // The QSO is confirmed
        qso.QslRec('Y');
        (QSLRecdateEdit)->setDate(qslRec);
        (QSLRecdateEdit)->setEnabled(true);
// 		if (!wasConfirmed){
// 			confirmed++;
// 		}
        dxcc.confirmedString(enti,(bandComboBox->currentText()).toUpper() ,  (modeComboBox->currentText()).toUpper());
        waz.confirmedString( world.getCqzFromCall(qso.getQrz()), (qso.getBand()).toUpper() ,  (qso.getMode()).toUpper());
    }else{ // The QSO is NOT confirmed
        qso.QslRec('N');
        (QSLRecdateEdit)->setEnabled(false);
        //TODO: This date is not valid, it is out of range!
//cout << "DAte out of range-7" << endl;
        qslRec = QDate::fromString("0000-00-00",Qt::ISODate);
        (QSLRecdateEdit)->setDate(qslRec);
        if (!modify){
            (QSLRecdateEdit)->setDate(qslRec);
            if (dxcc.isConfirmed(enti)){
                dxcc.notConfirmedString(enti, (bandComboBox->currentText()).toUpper(),  (modeComboBox->currentText()).toUpper());
                waz.notConfirmedString( world.getCqzFromCall(qso.getQrz()) ,qso.getBand(),qso.getMode());
            }
        }else{ // I am modifying
/*			if (wasConfirmed){ // If i am "deselecting" a previously confirmed QSO...
                confirmed--;
            }*/
            if (dxcc.isConfirmed(enti)){
                dxcc.notConfirmedString(enti, (bandComboBox->currentText()).toUpper(), (modeComboBox->currentText()).toUpper());
                waz.notConfirmedString( world.getCqzFromCall(qso.getQrz()) ,qso.getBand(),qso.getMode());
            }
        }
    }
    if ((!modify) && (qso.gotTheQSL())){
        qso.QslRec('Y');
        if (qslRec.isValid()){
        qso.setQslRecDateOn(qslRec);
        }
    }
    readAwardsStatus();
    showAwardsNumbers();
}

void Klog::readQso(){ //Just read the values an fill the qso
//cout << "KLog::readQso" << endl;
    qso.setQrz((qrzLineEdit->text()).toUpper());
    // Calculating RST values
    i = TSendBox->value();
    rsttx = i;
    i = SSendBox->value() * 10;
    rsttx = rsttx + i;
    i = RSendBox->value() * 100;
    rsttx = rsttx + i;
    i = TRecBox->value();
    rstrx = i;
    i = SRecBox->value() * 10;
    rstrx = rstrx + i;
    i = RRecBox->value() * 100;
    rstrx = rstrx + i;

    qso.setNumb (number);

    //dateTime =   qsoDateTime->dateTime();
    dateTime =   QDateTime(qsoDateEdit->date(), qsoTimeEdit->time() );
    if (dateTime.isValid()){
        qso.setDateTime(dateTime);
    }else{
        slotClearBtn();
        return;
    }
    qso.setRstrx(rstrx);
    qso.setRsttx(rsttx);

    band = bandComboBox->currentIndex();
    imode = modeComboBox->currentIndex();
    power = (powerSpinBox->text()).toUpper();
    qso.setBand ((bandComboBox->currentText()).toUpper());
    qso.setMode((modeComboBox->currentText()).toUpper());

    qso.setPower(power);

  // Running the QSL card slots
    if (!modify){
        slotQslRecvBoxChanged();
        slotQslSentBoxChanged();
    }
    slotIOTAChanged();

    if (enti!=0){
        award = awards.getAwardFor(world.getPrefix(qso.getQrz()));
        if (award.getReferenceNumber(awardsComboBox->currentText()) && ((awardsComboBox->currentIndex())!=0)){
            qso.setLocalAward(awardsComboBox->currentText());
            qso.setLocalAwardNumber(award.getReferenceNumber(awardsComboBox->currentText()));
            award.workReference(awardsComboBox->currentText(), true);

        }

    }

    if ((remarksTextEdit->toPlainText()).length() >0)
        qso.setComment(remarksTextEdit->toPlainText());

    qso.setQslVia(QSLcomboBox->currentText());

    // Check if the locator is valid
    if (locator.isValidLocator((locatorLineEdit->text()).toUpper()))
        qso.setLocator((locatorLineEdit->text()).toUpper());

    if (locator.isValidLocator((myLocatorLineEdit->text()).toUpper()))
        qso.setMyLocator((myLocatorLineEdit->text()).toUpper());

    if(qslVialineEdit->isEnabled())
        qso.setQslManager((qslVialineEdit->text()).toUpper());

    if ((QSLInfotextEdit->toPlainText()).length() > 0)
        //  if(QSLInfotextEdit->isEnabled())
        qso.setQslInfo(QSLInfotextEdit->toPlainText());

    if((namekLineEdit->text()).length() >= 2)
        qso.setName((namekLineEdit->text()).toUpper());

    if((qthkLineEdit->text()).length() >= 2)
        qso.setQth((qthkLineEdit->text()).toUpper());

    if((operatorLineEdit->text()).length() >= 3)
        qso.setOperator((operatorLineEdit->text()).toUpper());

    if((stationCallsignLineEdit->text()).length() >= 3)
        qso.setStationCallsign((stationCallsignLineEdit->text()).toUpper());

    if (freqlCDNumber->value() >= 0)
        qso.setFreq(QString::number(freqlCDNumber->value()));
}

void Klog::modifyQso(){
// Modify an existing QSO with the data on the boxes
    Klog::LogBook::iterator iter;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ) {
        if ( Klog::j == (*iter).getNumb() ){
            (*iter).setQrz( (qrzLineEdit->text()).toUpper() );
            (*iter).setDateTime(QDateTime(qsoDateEdit->date(), qsoTimeEdit->time() ) );
            //(*iter).setDateTime(qsoDateTime->dateTime());
            (*iter).setRstrx(rstrx);
            (*iter).setRsttx(rsttx);
            (*iter).setBand ((bandComboBox->currentText()).toUpper());
            (*iter).setMode((modeComboBox->currentText()).toUpper());
            (*iter).setPower((powerSpinBox->text()).toUpper());
            (*iter).setQth((qthkLineEdit->text()).toUpper());
            (*iter).setOperator((operatorLineEdit->text()).toUpper());
            (*iter).setStationCallsign((stationCallsignLineEdit->text()).toUpper());
            if ((remarksTextEdit->toPlainText()).length() >0)
                (*iter).setComment(remarksTextEdit->toPlainText());
            if((namekLineEdit->text()).length() >= 2)
                (*iter).setName((namekLineEdit->text()).toUpper());
            if((qthkLineEdit->text()).length() >= 2)
                (*iter).setQth((qthkLineEdit->text()).toUpper());
            if((operatorLineEdit->text()).length() >= 3)
                (*iter).setOperator((operatorLineEdit->text()).toUpper());
            if((stationCallsignLineEdit->text()).length() >= 3)
                (*iter).setStationCallsign((stationCallsignLineEdit->text()).toUpper());
            if ((iotaIntSpinBox->value() != 0)) // IOTA
                (*iter).setIota(iota);
            if ((awardsComboBox->currentIndex() != 0)){
                award = awards.getAwardFor(world.getPrefix(qso.getQrz()));
                if (award.getReferenceNumber(awardsComboBox->currentText())){
                    (*iter).setLocalAward(awardsComboBox->currentText());
                    (*iter).setLocalAwardNumber(award.getReferenceNumber(awardsComboBox->currentText()));
                    //award.workReference(awardsComboBox->currentText(), true);
                }
            }
            if (locator.isValidLocator((locatorLineEdit->text()).toUpper() ))
                (*iter).setLocator( (locatorLineEdit->text()).toUpper() );
            if (locator.isValidLocator((myLocatorLineEdit->text()).toUpper())){
                (*iter).setMyLocator((myLocatorLineEdit->text()).toUpper());
            }//else if (locator.isValidLocator(getMyLocator())) {
            //	(*iter).setMyLocator(getMyLocator());
            //}
            if (QSLSentcheckBox->isChecked()){
                qslSen = QSLSentdateEdit->date();
                (*iter).QslSent('Y');
                if (qslSen.isValid()){
                    (*iter).setQslSenDateOn(qslSen);
                }
            } else {
                (*iter).QslSent('N');
            }
            if (QSLReccheckBox->isChecked()){
                qslRec = QSLRecdateEdit->date();
                (*iter).QslRec('Y');
                if (qslRec.isValid()){
                    (*iter).setQslRecDateOn(qslRec);
                }
            }
            if ((*iter).gotTheQSL()){
                dxcc.confirmedString(enti, ((*iter).getBand()).toUpper(), ((*iter).getMode()).toUpper());
                waz.confirmedString( world.getCqzFromCall((*iter).getQrz()) ,((*iter).getBand()).toUpper(),((*iter).getMode()).toUpper());
            }
        } else {
            (*iter).QslRec('N');
            if (dxcc.isConfirmed(enti)){
                dxcc.notConfirmedString(enti, (bandComboBox->currentText()).toUpper(), (modeComboBox->currentText()).toUpper());
                waz.notConfirmedString( world.getCqzFromCall((*iter).getQrz()) ,(bandComboBox->currentText()).toUpper(), (modeComboBox->currentText()).toUpper());
            }
        }
    }
// Not sure but this below seems to be a duplication of above

/*    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
        if ( Klog::j == (*iter).getNumb() ){
            (*iter).setQrz( (qrzLineEdit->text()).toUpper() );
            (*iter).setDateTime(qsoDateTime->dateTime());
            (*iter).setRstrx(rstrx);
            (*iter).setRsttx(rsttx);
            (*iter).setBand ((bandComboBox->currentText()).toUpper());
            (*iter).setMode((modeComboBox->currentText()).toUpper());
            (*iter).setPower((powerSpinBox->text()).toUpper());
            (*iter).setQth((qthkLineEdit->text()).toUpper());
            (*iter).setOperator((operatorLineEdit->text()).toUpper());
            (*iter).setStationCallsign((stationCallsignLineEdit->text()).toUpper());

            if ((remarksTextEdit->toPlainText()).length() >0)
                (*iter).setComment(remarksTextEdit->toPlainText());
            if((namekLineEdit->text()).length() >= 2)
                (*iter).setName((namekLineEdit->text()).toUpper());
            if((qthkLineEdit->text()).length() >= 2)
                (*iter).setQth((qthkLineEdit->text()).toUpper());
            if((operatorLineEdit->text()).length() >= 3)
                (*iter).setOperator((operatorLineEdit->text()).toUpper());
            if((stationCallsignLineEdit->text()).length() >= 3)
                (*iter).setStationCallsign((stationCallsignLineEdit->text()).toUpper());
            if ((iotaIntSpinBox->value() != 0)) // IOTA
                (*iter).setIota(iota);

            if ((awardsComboBox->currentIndex() != 0)){
                award = awards.getAwardFor(world.getPrefix(qso.getQrz()));
                if (award.getReferenceNumber(awardsComboBox->currentText())){
                    (*iter).setLocalAward(awardsComboBox->currentText());
                    (*iter).setLocalAwardNumber(award.getReferenceNumber(awardsComboBox->currentText()));
                    //award.workReference(awardsComboBox->currentText(), true);
                    //>>>>>>> .r28
                }
                (*iter).setQslVia(QSLcomboBox->currentText()); //QSL Info
                if(qslVialineEdit->isEnabled())
                    (*iter).setQslManager(qslVialineEdit->text());
                if ((QSLInfotextEdit->toPlainText()).length() >0)
                //    if(QSLInfotextEdit->isEnabled())
                    (*iter).setQslInfo(QSLInfotextEdit->toPlainText());
            }
        }
    } */
}

void Klog::helpAbout() {
//cout << "KLog::helpAbout" << endl;
  /*QString description;

  description = "The KDE Ham Radio Logging program";
  KAboutData aboutData( "klog", I18N_NOOP("Klog"), KLogVersion, description, KAboutData::License_GPL, "(c) 2002-2005, Jaime Robles, EA4TV", 0, 0, "jaime@kde.org");
  aboutData.addAuthor("Jaime Robles, EA4TV",0, "jaime@kde.org");
//  K3AboutApplication kAbout(this);
  KAboutDialog kAbout(this);

  kAbout.setTitle ("About KLog");
  kAbout.setAuthor("Jaime Robles, EA4TV", "jaime@kde.org", "http:://jaime.robles.es/klog", "Main Developer");
  kAbout.setVersion ( Klog::KLogVersion);
  kAbout.show(this);
*/


QMessageBox::about( this, ("KLog-"+Klog::KLogVersion + " - 2008\nThe KDE Ham Radio Logging program"),
                            ("KLog: "+Klog::KLogVersion + " - The KDE Ham Radio Logging program\n"
                            "You can find the last version on http://jaime.robles.es/klog\nJaime Robles, EA4TV, jaime@robles.es"));

    //KLog::aboutData->show(this);
}

void Klog::slotQSLcomboBoxChanged(){
//cout << "KLog::slotQSLcomboChanged" << endl;
    QString combo = (QSLcomboBox)->currentText();

    if (combo.compare("No QSL") == 0){
        qslVialineEdit->setDisabled(true);
        //    QSLInfotextEdit->setDisabled(true);
        qslVialineEdit->clear();
        //    QSLInfotextEdit->clear();
    }else{
        if (combo.compare("Manager") == 0)
            qslVialineEdit->setEnabled(true);
        else
            qslVialineEdit->setDisabled(true);
    //    QSLInfotextEdit->setEnabled(true);
    }
}

// The next slots run/shows the setup dialog to setup KLog
 void Klog::slotPreferences(){
   //qDebgug() << "KLog::slotPreferences";
     Setup setupDialog;
     setupDialog.exec();
     readConf();
 }

void Klog::accept(){
//cout << "KLog::accept" << endl;
}

void Klog::createKlogDir(){
  //qDebug() << "KLog::createKlogDir";
  QFile file( "klogrc" );
  if ( !(file.open( QIODevice::ReadOnly ) ) ) {

    QMessageBox msgBox;
    msgBox.setText(i18n("KLog message:"));
    QString str = i18n("It seems to be the first time you run KLog in this computer.\nThe setup dialog will start to help you to configure KLog.\nWelcome to KLog.\n\nThe KLog team.");
    msgBox.setInformativeText(str);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

    slotPreferences();
  }

}

void Klog::readConf(){
//cout << "KLog::readConf" << endl;

    DXClusterServerToUse ="NOSERVER";
    dxClusterHost="NOSERVER";

    QFile file( "klogrc" );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString adifTab;
        QString theData;
        QString data = "";
        data = ""; //
        while (!stream.atEnd()){
            data = stream.readLine();
            if(!data.isEmpty()){ // If there is another line I read it
                data = data.simplified();
                QStringList fields = data.split("=", QString::KeepEmptyParts);
                //QStringList fields = QStringList::split('=', data );
                adifTab = fields[0].toUpper();
                theData = fields[1];
                if (adifTab == "CALL"){
                    setMyQrz(theData);
                }else if (adifTab == "LOCATOR"){
                    setMyLocator(theData.toUpper());
                }else if (adifTab == "DXCLUSTERSERVER"){
                    dxClusterHost = theData.toUpper();
                    if ((dxClusterPort>=1)||(DXClusterServerToUse=="NOSERVER")){
                        if (checkIfValidDXCluster((dxClusterHost+":"+QString::number(dxClusterPort)).toLower())){
                                DXClusterServerToUse = (dxClusterHost+":"+QString::number(dxClusterPort)).toLower();
                        }
                    }
                }else if (adifTab == "DXCLUSTERPORT"){
                    dxClusterPort = theData.toInt();
                    if ((dxClusterPort>=1)||(dxClusterHost!="NOSERVER")||(DXClusterServerToUse=="NOSERVER")){
                        if (checkIfValidDXCluster((dxClusterHost+":"+QString::number(dxClusterPort)).toLower())){
                            DXClusterServerToUse =  (dxClusterHost+":"+QString::number(dxClusterPort)).toLower();
                        }
                    }
                }else if (adifTab == "DXCLUSTERSERVERTOUSE"){
                    if ((theData.count(":"))==0){
                        theData = theData+":41112";
                    }
                    if (checkIfValidDXCluster(theData.toLower())){
                        DXClusterServerToUse = theData.toLower();
                    }
                }else if (adifTab == "TIMEUTC"){
                    if (theData.toUpper() == "TRUE"){
                        timeInUTC=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        timeInUTC=false;
                    }
                }else if (adifTab == "DXCLUSTERHFSPOTS"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterHFSpots=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterHFSpots=false;
                    }
                }else if (adifTab == "DXCLUSTERVHFSPOTS"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterVHFSpots=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterVHFSpots=false;
                    }
                }else if (adifTab == "DXCLUSTERWARCSPOTS"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterWARCSpots=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterWARCSpots=false;
                    }
                }else if (adifTab == "DXCLUSTERSSBSPOTS"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterSSBSpots=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterSSBSpots=false;
                    }
                }else if (adifTab == "DXCLUSTERCWSPOTS"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterCWSpots=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterCWSpots=false;
                    }
                }else if (adifTab == "DXCLUSTERCONFIRMEDANNOUNCES"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterConfirmedSpots=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterConfirmedSpots=false;
                    }
                }else if (adifTab == "DXCLUSTERANNOUNCES"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterANNounces=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterANNounces=false;
                    }
                }else if (adifTab == "DXCLUSTERWXANNOUNCES"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterWXANNounces=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterWXANNounces=false;
                    }
                }else if (adifTab == "DXCLUSTERWCYANNOUNCES"){
                    if (theData.toUpper() == "TRUE"){
                        dxClusterWCYANNounces=true;
                    }else if ((theData.toUpper() == "FALSE")){
                        dxClusterWCYANNounces=false;
                    }
                }else if (adifTab == "REALTIMELOG"){
                    if (theData.toUpper() == "TRUE"){
                        realTimeLog = true;
                    }else if ((theData.toUpper() == "FALSE")){
                        realTimeLog = false;
                    }
                }else if (adifTab == "DEFAULTBAND"){
                    band =  theData.toInt();
                }else if (adifTab == "DEFAULTMODE"){
                    imode =  theData.toInt();
                }else if (adifTab == "CONFIRMEDCOLOR"){
                    confirmedColor.setNamedColor(theData);
                }else if (adifTab == "WORKEDCOLOR"){
                    workedColor.setNamedColor(theData.toUpper());
                }else if (adifTab == "NEEDEDCOLOR"){
                    neededColor.setNamedColor(theData.toUpper());
                }else if (adifTab == "NEWONECOLOR"){
                    newOneColor.setNamedColor(theData.toUpper());
                }else if (adifTab == "DEFAULTCOLOR"){
                    defaultColor.setNamedColor(theData.toUpper());
                }else if (adifTab == "POWER"){
                    power = theData.toUpper();
                }else if (adifTab == "OPENLASTFILE"){
                    if (theData.toUpper() == "TRUE"){
                        openLastByDefault = true;
                    }else if ((theData.toUpper() == "FALSE")){
                        openLastByDefault = false;
                    }
                }else if (adifTab == "HAMLIB"){
                    if (theData.toUpper() == "TRUE"){
                        hamlib = true;
                    }else if ((theData.toUpper() == "FALSE")){
                        hamlib = false;
                    }
                }else if (adifTab == "RIGNAME"){
                    rignameNumber = (theData.toUpper()).toInt();
                    KlogHamlib.setRigNumber(rignameNumber);
                }else if (adifTab == "HAMLIBINTERVAL"){
                    hamlibInterval = (theData.toUpper()).toInt();
                }else if (adifTab == "BAUDS"){
                    bauds = (theData.toUpper()).toInt();
                    KlogHamlib.setBauds(bauds);
                }else if (adifTab == "PORT"){
                    serialPort = theData;
                    KlogHamlib.setSerialPort(serialPort);
                }else if (adifTab == "COMPLETEWITHPREVIOUS"){
                    if (theData.toUpper() == "TRUE"){
                        completeWithPrevious = true;
                    }else if ((theData.toUpper() == "FALSE")){
                        completeWithPrevious = false;
                    }
                }else if (adifTab == "REQUIREMANDATORY"){
                    if (theData.toUpper() == "TRUE"){
                        requireMandatory = true;
                    }else if ((theData.toUpper() == "FALSE")){
                        requireMandatory = false;
                    }
                }else if (adifTab == "LOGFILENAME"){
                    logFileNameToOpen = theData;
                    //logFileNameToSave = theData;	//We are just reading the file to Open
                }else if (adifTab == "SHOWPROGRESSDIALOG"){
                    if (theData.toUpper() == "TRUE"){
                        showProgressDialog = true;
                    }else if ((theData.toUpper() == "FALSE")){
                        showProgressDialog = false;
                    }
                }else{
                }
            }//Closes the if != empty
        }// Closes the while
        file.close();
    }else{
      // the file klogrc with preferences does not exist so we have to create it
       // slotKlogSetup(); // commenting the slotKlogSetup here we break a loop :-)
      QMessageBox msgBox;
      msgBox.setText(i18n("KLog message:"));
      QString str = i18n("You still have not configured KLog.\nPlease click on:\nSetup->Preferences\n and configure KLog.");
      msgBox.setInformativeText(str);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Information);
      msgBox.exec();

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
    if (locator.isValidLocator(tlocator.toUpper() ))
        myLocator = tlocator;
}

QString Klog::getMyLocator() const{
//cout << "KLog::getMyLocator" << endl;
    return myLocator;
}

void Klog::showWhere(const int enti){
//cout << "KLog::showWhere: " << QString::number(enti) << endl;
//	if ((enti != 0)&&(enti != -1)){
    if (enti >0){
        entityTextLabel->setText((world.getEntByNumb(enti)).getEntity());
        prxTextLabel->setText((world.getEntByNumb(enti)).getPfx());
        continentTextLabel->setText((world.getEntByNumb(enti)).getContinent());
        cqLCDNumber->display(world.getCQzFromEntity(enti));
        ituLCDNumber->display(world.getITUzFromEntity(enti));

        // If the QSO has a locator and it is valid, calculation is more exact!
        if (locator.isValidLocator((locatorLineEdit->text()).toUpper())){
        // The following code is copy&pasted from "slotLocatorChanged"
            dxLocator = (locatorLineEdit->text()).toUpper();

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
// Reads if the entity is worked/confirmed and show it
qDebug() << "KLog::fillEntityBandState: " << QString::number(enti) << endl;


    QPalette confirmedPalette (confirmedColor, QPalette::Window);
    QPalette workedPalette (workedColor, QPalette::Window);
    if (!dxcc.isWorked(enti)){ // IT IS A NEW ONE!!!!!!!

// Qpalette::Qpalette (
//   const QColor & foreground,
//   const QColor & background,
//   const QColor & light,
//   const QColor & dark,
//   const QColor & mid,
//   const QColor & text,
//   const QColor & base )

//QPalette newOneColorG ( newOneColor, newOneColor, newOneColor, newOneColor, newOneColor, newOneColor, newOneColor );

//TODO: DELETED FOR QT4 MIGRATION:
// SET THE COLOR
// 		textLabelBand2->setPalette(newOneColor);
// 		textLabelBand6->setPalette(newOneColor);
// 		textLabelBand10->setPalette(newOneColor);
// 		textLabelBand12->setPalette(newOneColor);
// 		textLabelBand15->setPalette(newOneColor);
// 		textLabelBand17->setPalette(newOneColor);
// 		textLabelBand20->setPalette(newOneColor);
// 		textLabelBand30->setPalette(newOneColor);
// 		textLabelBand40->setPalette(newOneColor);
// 		textLabelBand80->setPalette(newOneColor);
// 		textLabelBand160->setPalette(newOneColor);
// 		textLabelBand70cm->setPalette(newOneColor);
    }else{

    if(dxcc.isConfirmedBand(enti, adif.band2Int("2M"))){ // 2m band
      // RED for confirmed
      textLabelBand2->setPalette(confirmedPalette);
//      textLabelBand2->setPalette(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("2M"))){
        // Yellow for worked but not confirmed
//         textLabelBand2->setPalette(workedColor);
         textLabelBand2->setPalette(workedPalette);
      }else{
        //GREEN if new one
         textLabelBand2->setPalette(neededColor);
      }
    }

  if(dxcc.isConfirmedBand(enti, adif.band2Int("6M"))){ // 6m band
      // RED for confirmed
       textLabelBand6->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("6M"))){
      // Yellow for worked but not confirmed
       textLabelBand6->setPalette(workedColor);
    }else{
      //GREEN if new one
       textLabelBand6->setPalette(neededColor);
    }
  }

  if(dxcc.isConfirmedBand(enti, adif.band2Int("10M"))){ // 10m band
//	cout << "KLog::fillEntityBandState confirmed: " << QString::number(enti) << endl;
    // RED for confirmed
     textLabelBand10->setPalette(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("10M"))){
         textLabelBand10->setPalette(workedColor);
      }else{
       textLabelBand10->setPalette(neededColor);
        //GREEN if new one
      }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("12M"))){ // 12m band
// SET THE COLOR
     textLabelBand12->setPalette(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("12M"))){
        // Yellow for worked but not confirmed
        textLabelBand12->setPalette(workedColor);
      }else{
        textLabelBand12->setPalette(neededColor);
        //GREEN if new one
      }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("15M"))){ // 15m band
    // RED for confirmed
    textLabelBand15->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("15M"))){
      // Yellow for worked but not confirmed
      textLabelBand15->setPalette(workedColor);
    }else{
       textLabelBand15->setPalette(neededColor);
        //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("17M"))){ // 17m band
    // RED for confirmed
       textLabelBand17->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("17M"))){
      // Yellow for worked but not confirmed
       textLabelBand17->setPalette(workedColor);
    }else{
       textLabelBand17->setPalette(neededColor);
      //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("20M"))){ // 20m band
     textLabelBand20->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("20M"))){
// SET THE COLOR
       textLabelBand20->setPalette(workedColor);
    }else{
       textLabelBand20->setPalette(neededColor);
      //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("30M"))){ // 30m band
    // RED for confirmed
     textLabelBand30->setPalette(confirmedColor);
    }else{
      if(dxcc.isWorkedBand(enti, adif.band2Int("30M"))){
      // Yellow for worked but not confirmed
         textLabelBand30->setPalette(workedColor);
      }else{
         textLabelBand30->setPalette(neededColor);
        //GREEN if new one
      }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("40M"))){ // 40m band
      // RED for confirmed
     textLabelBand40->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("40M"))){
      // Yellow for worked but not confirmed
       textLabelBand40->setPalette(workedColor);
    }else{
       textLabelBand40->setPalette(neededColor);
        //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("80M"))){ // 80m band
    // RED for confirmed
     textLabelBand80->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("80M"))){
      // Yellow for worked but not confirmed
       textLabelBand80->setPalette(workedColor);
    }else{
       textLabelBand80->setPalette(neededColor);
        //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("70CM"))){ // 70CM band
    // RED for confirmed
     textLabelBand70cm->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("70CM"))){
      // Yellow for worked but not confirmed
       textLabelBand70cm->setPalette(workedColor);
    }else{
       textLabelBand70cm->setPalette(neededColor);
      //GREEN if new one
    }
  }
  if(dxcc.isConfirmedBand(enti, adif.band2Int("160M"))){ // 160m band
     textLabelBand160->setPalette(confirmedColor);
  }else{
    if(dxcc.isWorkedBand(enti, adif.band2Int("160M"))){
       textLabelBand160->setPalette(workedColor);
    }else{
       textLabelBand160->setPalette(neededColor);
      //GREEN if new one
    }
  }
}
}

void Klog::tlfReadLog(const QString& tfileName){
//TODO: It is needed to improve the eficiency
// It is VEEEEEEEEERY slow if the log is big/long
//cout << "KLog::tlfReadLog" << endl;
    bool year2000 = true;
    int totalQsos = 0; // QSOs in the log to be read
    int progresStep = 0;
/*	Q3ProgressDialog progress( i18n("Reading the log..."), i18n("Abort reading"), 0,
                          this, i18n("progress"), TRUE );*/

    QProgressDialog progress(i18n("Reading the log..."), i18n("Abort reading"), 0, totalQsos);

    QString progressLabel;


    bool ok;
    QInputDialog qinputD;
    qinputD.setLabelText(i18n("Enter a remark for ALL the imported QSO:\n(Leave it empty and press OK if no remark)"));
    QString text = qinputD.getText(widget, i18n("KLog - TLF Import"),
      i18n("Enter a remark for ALL the imported QSO:\n(Leave it empty and press OK if no remark)"), QLineEdit::Normal, QString::null, &ok);
/*	QString text = QInputDialog::getText(this, i18n("KLog - TLF Import"),
    i18n("Enter a remark for ALL the imported QSO:\n(Leave it empty and press OK if no remark)"));*/
    if ( ok && !text.isEmpty() ) {
    // user entered something and pressed OK
    }else if (!ok){ // The user clicked CANCEL
        return;
    }else{
    // user entered nothing but clicked over OK
    }
    aux ="NULL";  // aux is used for the call used in the contest
    //TODO: It is necessary to add a check to know if the call used in the contest is valid.

     QString aux = QInputDialog::getText(this, i18n("KLog - TLF Import"),
                                          i18n("Enter the used call:\n(Leave it empty and press OK if no diferent call was used)"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

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


    QMessageBox msgBox;
    msgBox.setText(i18n("Warning - Possible dates conflict:"));
    QString str = i18n("Please define if the QSOs were done in the 1900s or in the 2000s years\nAre these QSO dated before year 2000?");
    msgBox.setInformativeText(str);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch(ret){
        case QMessageBox::Yes: // The QSOs were done in 19xx
            year2000 = false;
                break;
        case QMessageBox::No: // The QSOs were done in 20xx
            year2000 = true;
            break;

    }



    QString data =QString();
    QFile file( tfileName );
    QTextStream stream( &file );
    if (showProgressDialog){
        int totalQsos = 0;
        if ( file.open( QIODevice::ReadOnly ) ) { // We are going to count the number of QSOs of the file
            while (!stream.atEnd()){
                data = stream.readLine();
                if(!data.isEmpty() && (!data.startsWith(";") && (!data.isNull()))  ){
                    totalQsos = totalQsos + 1;
                    //getProgresStepForDialog
                }
            }
        }
        progress.setMaximum(totalQsos);
    }
    file.close();
    if ( file.open( QIODevice::ReadOnly ) ) {
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
                if((!data.isNull()) && (!data.isEmpty()) && (!data.startsWith(";")) ){
                    data = data.simplified().toUpper();
                    QStringList fields = data.split(" ", QString::SkipEmptyParts);
                    //QStringList fields = QStringList::split(' ', data );
                    qauxString = fields[0];
                    qdate = fields[1];
                    QStringList datelist = qdate.split("-", QString::SkipEmptyParts);
                    //QStringList datelist = QStringList::split( '-', qdate );
                    otherAux = datelist[2];
                    monthString = datelist[1];
                    dayString = datelist[0];
                    //TODO: Fix this hack
                    //HACK: It is not a good solution as it is only valid until 2099 but... who cares :-P

                    if (year2000 == false){
//					cout << "TLF import 1900" << endl;
                        otherAux = "19" + otherAux;
                    }else{
                        otherAux = "20" + otherAux;// only valid till 2009!!!
//						cout << "TLF import 2000" << endl;
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
                    qso.setQrz(qqrz.toUpper());
                    Klog::number++;
                    qso.setNumb(Klog::number);

                    qso.setDateTime(QDateTime::fromString(dateString, Qt::ISODate));
                    qso.setRstrx(qrstrx.toInt());
                    qso.setRsttx(qrsttx.toInt());
                    qso.setBand(qband.toUpper());
                    qso.setMode(qmode.toUpper());
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
                            progressLabel = i18n("Adding QSO#: ") + QString::number(number);
                            progressLabel = progressLabel + " / " + QString::number(totalQsos);
                            progress.setLabelText(progressLabel);
                            progress.setValue(progresStep );
                            qApp->processEvents();
                        }
                        if ( progress.wasCanceled()){
                            return;
                        }
                    }
                }
        }
    }
    file.close();

    needToSave = true;
    showLogList();
    showAwardsNumbers();

}

void Klog::cabrilloReadLog(const QString& tfileName){
//cout << "KLog::cabrilloReadLog" << endl;
    int totalQsos = 0; // QSOs in the log to be read
    int progresStep = 0;
    QString contest = "";
    QString usedCallsign = "";
    QProgressDialog progress(i18n("Reading the log..."), i18n("Abort reading"), 0, totalQsos);
    QString progressLabel;
    bool ok;
    QString text = QInputDialog::getText(this, i18n("KLog - Cabrillo Import"),
                                          i18n("Enter a remark for ALL the imported QSO:\n(Leave it empty and press OK if no remark)"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

    if ( ok && !text.isEmpty() ) {
    // user entered something and pressed OK
    }else if (!ok){ // The user clicked CANCEL
        return;
    }else{
    // user entered nothing but clicked over OK
    }

    QString data;
    QFile file( tfileName );
    QTextStream stream( &file );
    if (showProgressDialog){
        int totalQsos = 0;
        if ( file.open( QIODevice::ReadOnly ) ) { // We are going to count the number of QSOs of the file
            while (!stream.atEnd()){
                data = stream.readLine();
                    if((!data.isNull()) && (!data.isEmpty()) && (data.startsWith("CONTEST:"))  ){
                        data = data.simplified().toUpper();
                        QStringList fields = data.split(" ", QString::SkipEmptyParts);
                        //QStringList fields = QStringList::split(' ', data );
                        contest = fields[1];

                        // Now we have to adapt cabrillo to ADIF contest names
                        if (contest =="FCG-FQP"){
                            contest = "FL-QSO-PARTY";
                        }

                    }
                    if(!data.isEmpty() && (data.startsWith("CALLSIGN:"))  ){
                        data = data.simplified().toUpper();
                        QStringList fields = data.split(" ", QString::SkipEmptyParts);
                        //QStringList fields = QStringList::split(' ', data );
                        usedCallsign = fields[1];

                    }
                    if(!data.isEmpty() && (data.startsWith("QSO:"))  ){
                        totalQsos = totalQsos + 1;
                        //getProgresStepForDialog
                    }
            }
        }
        progress.setMaximum(totalQsos);
    }
    file.close();

    // Now the QSO reading starts

    if ( file.open( QIODevice::ReadOnly ) ) {
        QString qfreq;
        QString qmode;
        QString qdateString;
        QString qtimeString;
        QDateTime qdateTime;
        QString qmyCall;
        QString qrsttx;
        QString qrstrx;
        int stxNum =-1;
        int srxNum =-1;
        QString stxString="";
        QString srxString="";
        QString qdxCall;
        QString qt;

        data = "";
        while (!stream.atEnd()){
            data = stream.readLine();
                if((!data.isNull()) && (!data.isEmpty()) && (data.startsWith("QSO:")) ){
                    data = data.simplified().toUpper();
                    QStringList fields = data.split(" ", QString::SkipEmptyParts);
                    //QStringList fields = QStringList::split(' ', data );
/*
The first one to develop is WPX SSB
0    1     2  3          4    5             6   7      8             9   10    11
QSO: freq  mo date       time call          rst exch   call          rst exch   t
QSO: ***** ** yyyy-mm-dd nnnn ************* nnn ****** ************* nnn ****** n
QSO:  3799 PH 1999-03-06 0711 HC8N          59  001    W1AW          59  001    0
000000000111111111122222222223333333333444444444455555555556666666666777777777788
123456789012345678901234567890123456789012345678901234567890123456789012345678901

*/
                    if ((contest =="CQ-WPX-SSB") || (contest =="IARU-HF") || (contest =="CQ-WPX-SSB") || (contest =="WAEDC") || (contest =="JIDX-CW") || (contest =="CQ-WPX-RTTY")){
                        qfreq = fields[1];
                        qmode = fields[2];
                        qdateString = fields[3];
                        qtimeString = (fields[4]).insert(2,':');
                        qmyCall = fields[5];
                        qrsttx = fields[6];
                        qrstrx= fields[9];
                        stxNum = (fields[7]).toInt();
                        srxNum = (fields[10]).toInt();
                        qdxCall = fields[8];
                        qt = fields[11];
                    }else if (contest =="AP-SPRINT"){
                        qfreq = fields[1];
                        qmode = fields[2];
                        qdateString = fields[3];
                        qtimeString = (fields[4]).insert(2,':');
                        qmyCall = fields[5];
                        qrsttx = fields[6];
                        qrstrx= fields[9];
                        stxNum = (fields[7]).toInt();
                        srxNum = (fields[10]).toInt();
                        qdxCall = fields[8];
                    }else if ((contest =="ARRL-10") || (contest =="ARRL-160") || (contest =="ARRL-DX-SSB") || (contest =="STEW-PERRY") || (contest =="NEQP")){
                        qfreq = fields[1];
                        qmode = fields[2];
                        qdateString = fields[3];
                        qtimeString = (fields[4]).insert(2,':');
                        qmyCall = fields[5];
                        qrsttx = fields[6];
                        qrstrx= fields[9];
                        stxString = fields[7];
                        srxString = fields[10];
                        qdxCall = fields[8];
//TODO:FAILS with  STEW in the 160m band

                    }else if ((contest =="ARRL-VHF-SEP") || (contest =="CQ-VHF")){
                        qfreq = fields[1];
                        qmode = fields[2];
                        qdateString = fields[3];
                        qtimeString = (fields[4]).insert(2,':');
                        qmyCall = fields[5];
                        stxString = fields[6];
                        srxString = fields[8];
                        qdxCall = fields[7];
                    }else if ((contest =="CQ-WW-RTTY") || (contest =="RSGB-IOTA")){
                        qfreq = fields[1];
                        qmode = fields[2];
                        qdateString = fields[3];
                        qtimeString = (fields[4]).insert(2,':');
                        qmyCall = fields[5];
                        qrsttx = fields[6];
                        qrstrx= fields[10];
                        stxNum = (fields[7]).toInt();
                        srxNum = (fields[11]).toInt();
                        stxString = fields[8];
                        srxString = fields[12];
                        qdxCall = fields[9];
                    }else{ // If we do not detect the contest, we will try WPX but
                        // it may not be properly imported
                        //TODO: Show a message alerting about the imposibility to detect
                        // the contest ID.
                        qfreq = fields[1];
                        qmode = fields[2];
                        qdateString = fields[3];
                        qtimeString = (fields[4]).insert(2,':');
                        qmyCall = fields[5];
                        qrsttx = fields[6];
                        qrstrx= fields[9];
                        qdxCall = fields[8];
                        qt = fields[11];
                    }

                    // Mode adaptation Cabrillo->ADIF
                    if (qmode == "PH"){
                        qmode = "SSB";
                    }else if (qmode == "RY"){
                        qmode = "RTTY";
                    }

                    qdateString = qdateString +"T"+qtimeString;
                    //qdateTime = QDate::fromString(qdateString,Qt::ISODate);
                    //qso.setDateTime(qdateTime);

                    //cout << "KLog processLine Date: |" << dateString <<"|"<< endl;
                    if ( !(QDateTime::fromString(qdateString, Qt::ISODate)).isValid() ) {
                    //cout << "INVALID DATE-3: " << dateString << endl;
                    }else{
                        qso.setDateTime(QDateTime::fromString(qdateString, Qt::ISODate));
                    }

                    Klog::number++;
                    qso.setNumb(Klog::number);
                    qso.setComment(text);
                    qso.setQrz(qdxCall);
                    qso.setStationCallsign(qmyCall);
                    qso.setMode(qmode);
                    qso.setRstrx(qrstrx.toInt());
                    qso.setRsttx(qrsttx.toInt());
                    qso.setContestID(contest);
//					qso.setBand(adif.freq2Band(QString::number((qfreq.toDouble())/1000)));

                    qso.setFreq(QString::number((qfreq.toDouble())/1000));

                    qso.setBand(adif.freq2Band(qso.getFreq()));

                    if (stxNum!=-1){
                        qso.setStx(stxNum);
                    }
                    if (srxNum!=-1){
                        qso.setSrx(srxNum);
                    }
                    if (stxString!=""){
                        qso.setStx_string(stxString);
                    }
                    if (srxString!=""){
                        qso.setSrx_string(srxString);
                    }
// cout << "KLog-Cabrillo Import: =========================== BEGINING" << endl;
// cout << "KLog-Cabrillo Import: NUMBER: " << QString::number(qso.getNumb()) << endl;
// cout << "KLog-Cabrillo Import: DX QRZ: " << qso.getQrz() << endl;
// cout << "KLog-Cabrillo Import: My Call: " << qso.getStationCallsign()<< endl;
// cout << "KLog-Cabrillo Import: RST TX: " << QString::number(qso.getRsttx())<< endl;
// cout << "KLog-Cabrillo Import: RST RX: " << QString::number(qso.getRstrx())<< endl;
// cout << "KLog-Cabrillo Import: MODE: " << qso.getMode()<< endl;
// cout << "KLog-Cabrillo Import: FREQ: " << qso.getFreq()<< endl;
// cout << "KLog-Cabrillo Import: BAND: " << qso.getBand()<< endl;
// cout << "KLog-Cabrillo Import: STX: " << QString::number(qso.getStx())<< endl;
// cout << "KLog-Cabrillo Import: SRX: " << QString::number(qso.getSrx())<< endl;
// cout << "KLog-Cabrillo Import: Date: " << qso.getDateTime().toString("yyyy-MM-dd") << endl;
// cout << "KLog-Cabrillo Import: time: " << qso.getDateTime().toString("hh:mm") << endl;
// cout << "KLog-Cabrillo Import: TimeString: " << qtimeString << endl;
// cout << "KLog-Cabrillo Import: =========================== END\n" << endl;

                    addQSOToLog();
                    if (showProgressDialog){
                        if ( (number % getProgresStepForDialog(totalQsos) )== 0){ // To update the speed i will only show the progress once each 25 QSOs
                            progresStep = number;
                                                        progressLabel = tr("Adding QSO#: ") + QString::number(number) + " / " + (QString::number(totalQsos));
                            progress.setLabelText(progressLabel);
                            progress.setValue(progresStep );
                            qApp->processEvents();
                        }
                        if ( progress.wasCanceled()){
                            return;
                        }
                    }
                }
        }
    }
    file.close();

    needToSave = true;
    showLogList();
    showAwardsNumbers();


}

void Klog::slotAddLog(){
  //qDebug() << "KLog::slotAddLog";
  QString fn = "";
  QString tempLocator;
  QString tempOriginalLocator;
  bool ok;


  fn = QFileDialog::getOpenFileName(this, i18n("Save File"),
                            klogDir,
                            i18n("ADIF (*.adi)"));

  if ( fn.isEmpty() ) //TODO: check if it is better any other checking
    return;

  QFile file( fn );
  if ( file.exists( ) ) {	// We will check for all the data only if the file exists

    textStringAux = "";
    textStringAux = QInputDialog::getText(this, i18n("KLog - Log Add"),
                    i18n("Enter a remark for ALL the imported QSO:\n(Leave it empty and press OK if no remark)"), QLineEdit::Normal,
                    QDir::home().dirName(), &ok);
    if ( ok && !textStringAux.isEmpty() ) {
            // user entered something and pressed OK
    }else if (!ok){ // The user clicked CANCEL
        return;
    }else{
        // user entered nothing but clicked over OK
    }

    operatorStringAux ="BADCALL";
    ok = true;
    operatorStringAux ="";
    operatorStringAux = QInputDialog::getText(this, i18n("KLog - Log Add"),
        i18n("Enter the used call:\n(Leave it empty and press OK if the call ")+ getMyQrz() + i18n(" was used)"), QLineEdit::Normal,
            QDir::home().dirName(), &ok);

    if ( ok && !operatorStringAux.isEmpty() ) {
      if (operatorStringAux.length()<=2){

    QMessageBox msgBox;
    msgBox.setText(i18n("Warning - QRZ not valid"));
    QString str = i18n("Do you want to import without a QRZ?");
    msgBox.setInformativeText(str);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

      switch(ret){
        case QMessageBox::No:// The user does not want to continue without a call
          operatorStringAux ="";
        break;
        case QMessageBox::Yes: // The user clicked over YES so import
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

    tempLocator="";
    tempLocator = QInputDialog::getText(this, i18n("KLog - Log Add"),
                                          i18n("Enter the locator of the activity:\n(Leave it empty and press OK if the locator was ") + getMyLocator() + ".", QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);


    if ( ok && !tempLocator.isEmpty() ) {
      if (!locator.isValidLocator(tempLocator)){
    QMessageBox msgBox;
    msgBox.setText(i18n("Warning - Locator not valid"));
    QString str = i18n("Do you want to import without a Locator and use ")
      + getMyLocator() + "?\n";
    msgBox.setInformativeText(str);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch(ret){
      case QMessageBox::No:// The user does not want to continue without a call
        tempLocator = "";
      break;
      case QMessageBox::Yes: // The user clicked over YES so import
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

    addingLog = true;
    tempOriginalLocator = getMyLocator();	// Replace the default locator with the new one
    setMyLocator(tempLocator);		// just for this log addition


    adifReadLog(fn);
    setMyLocator(tempOriginalLocator);	// Set the original locator again.
    needToSave = true;
    addingLog = false;			// I hace finished the addition.

    QMessageBox msgBox;
    msgBox.setText(i18n("KLog message:"));
//	  QString str = i18n("The log file has been\nsucessfully added!");
      QString str = i18n("The log file has been\nsucessfully added!");
      msgBox.setInformativeText(str);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

  }
  textStringAux = "";
  operatorStringAux ="";
  tempLocator="";

}
void Klog::slotImportCabrillo(){
//cout << "KLog::slotImportCabrillo" << endl;

    //qDebug() << "KLog::slotImportCabrillo - DELETED TO HELP THE QT4 MIGRATION - TO BE RESTORED ASAP";

      QMessageBox msgBox;
      msgBox.setText(i18n("KLog message:"));
      QString str = i18n("This function (slotImportCabrillo) has been deleted to help the QT4 migration.\nIt will be restored ASAP");
      msgBox.setInformativeText(str);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.exec();


// 	  QMessageBox msgBox;
// 	  msgBox.setText(i18n("KLog message:"));
// 	  QString str = i18n("The Cabrillo import tool is a new feature and has not been fully tested with all the contests data.\nAlthough it is supposed to work well, please backup your logs and check them after adding the Cabrillo file.\n\nIn case you find an error please contact EA4TV.\nTNX.");
// 	  msgBox.setInformativeText(str);
// 	  msgBox.setStandardButtons(QMessageBox::Ok);
// 	  msgBox.setDefaultButton(QMessageBox::Ok);
// 	  msgBox.setIcon(QMessageBox::Warning);
// 	  msgBox.exec();


//	cabrilloReadLog(fileName);




// 	Q3FileDialog fileOpenDialog(this, i18n("Open a file"), TRUE);
// 	fileOpenDialog.setFilter(i18n("CAB files (*.cab *.CAB)"));
// 	fileOpenDialog.setDir(klogDir);
// 	if (fileOpenDialog.exec() == QDialog::Accepted)
// 		fileName=fileOpenDialog.selectedFile();
// 	else
// 		return;
// 	cabrilloReadLog(fileName);

    //cabrilloLog.cabrilloReadLog("/home/jaime/.klog/cabrillo");
}

void Klog::slotImportTlf(){
//cout << "KLog::slotImportTlf" << endl;
//	QString fileName;

//       QString fileName = QFileDialog::getOpenFileName(
//                     this,
//                     "Open a TLF file",
//                     klogDir,
//                     "TLF (*.log *.LOG)");
//
//
// 		    tlfReadLog(fileName);
//qDebug() << "KLog::slotImportTlf - DELETED TO HELP THE QT4 MIGRATION - TO BE RESTORED ASAP";

      QMessageBox msgBox;
      msgBox.setText(i18n("KLog message:"));
      QString str = i18n("This function (slotImportTlf) has been deleted to help the QT4 migration.\nIt will be restored ASAP");
      msgBox.setInformativeText(str);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.exec();


}

void Klog::slotIOTAChanged(){
//cout << "KLog::slotIOTAChanged value: " << QString::number(iotaIntSpinBox->value()) << endl;
    //iotaIntSpinBox->setEnabled(true);

    if ((iotaIntSpinBox->value() != 0)){ //
        iota = iotaComboBox->currentText() + "-" + QString::number(iotaIntSpinBox->value());
        if (!modify)
            qso.setIota(iota);
    }
}

void Klog::entityState(const int tentity){
  //qDebug() << "KLog::entityState: " << QString::number(tentity) << endl;
// Prints the LED and message

    int i = tentity;
    if (i == -1){
        return;
    }
        QPalette palette;
    if (dxcc.isConfirmed(i) ){
        if(dxcc.isConfirmedBand(i, adif.band2Int((bandComboBox->currentText()).toUpper()))){
            // CONFIRMED IN THIS BAND... NO WORK NEEDED
                        LedtextLabel->setText(tr( "<p align=\"center\"><b>Worked & confirmed before</b></p>"));
                        //ledColorTextLabel->setPalette(confirmedColor);
//                        palette.setColor(ledColorTextLabel->backgroundRole(), confirmedColor);
                        kledColor->setColor(confirmedColor);
        }else {
            // CONFIRMED BUT NOT IN THIS BAND SO WORK IT!
            if (dxcc.isWorkedBand(i, adif.band2Int((bandComboBox->currentText()).toUpper()))){
    // IT IS WORKED IN THIS BAND BUT STILL NOT CONFIRMED, WORK IT!
                LedtextLabel->setText(i18n( "<p align=\"center\"><b>Not confirmed in this band, WORK IT!</b></p>"));
                //newEntitykLed->setColor(workedColor );
//                                palette.setColor(ledColorTextLabel->backgroundRole(), workedColor);
                                kledColor->setColor(workedColor);
            }else{
     // IT IS NOT WORKED IN THIS BAND SO WORK IT!
     // LED = GREEN
                LedtextLabel->setText(i18n( "<p align=\"center\"><b>New one in this band, WORK IT!</b></p>"));
                                //palette.setColor(ledColorTextLabel->backgroundRole(), neededColor);
                                kledColor->setColor(neededColor);
                        }
        }
    }else{	// END OF CONFIRMED
    // IT IS NOT CONFIRMED SO WORK IT!
        if (dxcc.isWorked(i)){
            // IT IS WORKED.... BUT NOT CONFIRMED WORK IT!
            if (dxcc.isWorkedBand(i, adif.band2Int((bandComboBox->currentText()).toUpper()))){
                // IT IS WORKED IN THIS BAND BUT STILL NOT CONFIRMED, WORK IT!
                LedtextLabel->setText(i18n( "<p align=\"center\"><b>Not confirmed, WORK IT!</b></p>"));
                                //palette.setColor(ledColorTextLabel->backgroundRole(), workedColor);
                                kledColor->setColor(workedColor);
                        }else{
                    // IT IS NOT WORKED IN THIS BAND, SO WORK IT!
                                //palette.setColor(ledColorTextLabel->backgroundRole(), neededColor);
                                kledColor->setColor(neededColor);
            }
        } else{
                // IT IS A COMPLETE NEW ONE WORK IT AT ANY PRICE!!!!
            LedtextLabel->setText(i18n( "<p align=\"center\"><b>NEW ONE, WORK IT!</b></p>"));
            //newEntitykLed->setColor(neededColor );
                        //palette.setColor(ledColorTextLabel->backgroundRole(), newOneColor);
                        kledColor->setColor(newOneColor);
                }
    }

}
int Klog::howManyConfirmedQSO(){
//cout << "KLog::howManyConfirmedQSO" << endl;
    Klog::LogBook::iterator iter;
    int howManyConfirmed = 0;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
        if ((*iter).gotTheQSL()) {
            howManyConfirmed++;
        }
    }
    return howManyConfirmed;
}
void Klog::slotSearchButton(){
//qDebug() << "KLog::slotSearchButton";
    if (searching2QSL){
        slotSearchQSO2QSL();
    }else{
    qrz = (searchQrzkLineEdit->text()).toUpper();
    searchQsosTreeWidget->clear();
    if (qrz.length()<2){ // This is a hack to avoid slow searches for every previous QSO that
                       // count one specific letter (nearly every QSO has just one letter!!)
        previousQthkLineEdit->clear();
        previousNamekLineEdit->clear();
        return;
    }

    Klog::LogBook::iterator iter;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
        if (((*iter).getQrz()).count(qrz) > 0){
            i++;
            switch(needToWorkFromCluster((*iter).getQrz(),adif.band2Int((*iter).getBand()))){
            // 0 confirmed, 1 worked, 2 new one, -1 error
                case 0: // Confirmed
                    colorInUse = confirmedColor;
                    break;
                case 1: // Worked
                    colorInUse = workedColor;
                    break;
                case 2: // Needed
                    colorInUse = neededColor;
                    break;
                case 3: // New One
                    colorInUse = newOneColor;
                    break;
                case -1: // ERROR
                    colorInUse = defaultColor;
                    break;
                default:
                    colorInUse = defaultColor;
                    break;
            }
            searchBoxItem *item = new searchBoxItem(searchQsosTreeWidget, (*iter).getQrz(),
                                       (*iter).getDateTime().toString("yyyy-MM-dd"),(*iter).getDateTime().toString("hh:mm"),
                                       (*iter).getBand(), (*iter).getMode(), (*iter).isQslSent(), (*iter).isQslRec(),
                                       getNumberString((*iter).getNumb()), colorInUse );

            colorInUse = defaultColor;

            if ( ((*iter).getName()).length() > 1){
                previousNamekLineEdit->setText((*iter).getName());
            }else{
                previousNamekLineEdit->clear();
            }
            if ( ((*iter).getQth()).length() > 1){
                previousQthkLineEdit->setText((*iter).getQth());
            }else{
                previousQthkLineEdit->clear();
            }
        }
    }
    enti = world.findEntity(qrz);
        if (enti>0){
            entityState(enti);
        }
    }

}

void Klog::slotSearchQSO2QSL(){
qDebug() << "KLog::searchQSO2QSL" ;
//TODO: Maybe I should add a button for this action
//TODO: After mark a QSO as sent, keep the list in the next QRZ to be QSLed

    searching2QSL = true;
    searchQsosTreeWidget->clear();

    Klog::LogBook::iterator iter;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
        if (!((*iter).sentTheQSL())&&(needToWorkFromCluster((*iter).getQrz(),adif.band2Int((*iter).getBand()))!=0)){  // We do not need to QSL a QSO with a confirmed entity.
            i++;

            if(!dxcc.isConfirmed(world.findEntity((*iter).getQrz()))){
                colorInUse = newOneColor;
            }else{
            switch(needToWorkFromCluster((*iter).getQrz(),adif.band2Int((*iter).getBand()))){
            // 0 confirmed, 1 worked, 2 new one, -1 error
                case 0: // Confirmed
                    colorInUse = confirmedColor;
                    break;
                case 1: // Worked
                    colorInUse = workedColor;
                    break;
                case 2: // Needed
                    colorInUse = neededColor;
                    break;
                case 3: // New One
                    colorInUse = newOneColor;
                    break;

                case -1: // ERROR
                    colorInUse = defaultColor;
                    break;
                default:
                    colorInUse = defaultColor;
                break;
            }
            }
            searchBoxItem * item = new searchBoxItem(searchQsosTreeWidget, (*iter).getQrz(), (*iter).getDateTime().toString("yyyy-MM-dd"),(*iter).getDateTime().toString("hh:mm"), (*iter).getBand(), (*iter).getMode(), (*iter).isQslSent(), (*iter).isQslRec(), getNumberString((*iter).getNumb()), colorInUse );
            colorInUse = defaultColor;
            if ( ((*iter).getName()).length() > 1){
                previousNamekLineEdit->setText((*iter).getName());
            }else{
                previousNamekLineEdit->clear();
            }
            if ( ((*iter).getQth()).length() > 1){
                previousQthkLineEdit->setText((*iter).getQth());
            }else{
                previousQthkLineEdit->clear();
            }
        }//
    }
    enti = world.findEntity(qrz);
//	if (enti!=0){
    if (enti>0){
        entityState(enti);
    }
        outputTab->setCurrentIndex(2);
}

void  Klog::slotCancelSearchButton(){
//qDebug() << "KLog::slotCancelSearchButton" << endl;
    //If we are searching QSO to send the QSL we simply call to that slot again :-)
        searchQrzkLineEdit->clear();
        searchQsosTreeWidget->clear();
        searching2QSL = false;
}


// The following is to select a QSO from the search box
void Klog::slotQsoSearchSelectedForEdit( QTreeWidgetItem * item, int){
qDebug() << "KLog::slotQsoSearchSelectedForEdit" << endl;
    if (item){
        int number = (item->text(7)).toInt();
        // Removing this fixed the double click search issue. It can also be fixed by saving the item->number
        // before running the slot below. When running the slot below though it clears your search.
        //slotClearBtn();
        qso = getByNumber(number);
        toEditQso();
        okBtn->setText(i18n("Modify"));
        clearBtn->setText(i18n("Cancel"));
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
qDebug() << "KLog::slotQsoDelete" << endl;
    if ((!modify) && (Klog::j == 0)){
        return;
    } else {
        Klog::LogBook::iterator iter;
        for ( iter = logbook.begin(); iter != logbook.end(); ++iter ) {
            if ( j == (*iter).getNumb() ) {

               QMessageBox msgBox;
               msgBox.setText(i18n("Warning - QSO Deletion"));
               QString str = i18n("Do you want to delete the QSO with:\n")
               + (*iter).getQrz() + i18n(" of ") + (*iter).getDateTime().toString("yyyy-MM-dd") + "?";
               msgBox.setInformativeText(str);
               msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
               msgBox.setDefaultButton(QMessageBox::No);
               int ret = msgBox.exec();

               switch(ret){
                    case QMessageBox::Yes:
                        dxcc.notWorked(world.findEntity((*iter).getQrz().toUpper()), adif.band2Int((*iter).getBand()), adif.mode2Int((*iter).getMode()));
                        waz.notWorked(world.getCqzFromCall((*iter).getQrz().toUpper()), adif.band2Int((*iter).getBand()), adif.mode2Int((*iter).getMode()));
//  						if ((*iter).gotTheQSL()){
//  							Klog::confirmed--;  //To decrease the showed number
//  						}
                        // If there is only one entry in the log then you want to delete it the it's easier to create a new log.
                        if ( Klog::j == 1 ) {
                            fileNew();
                        } else {
                            logbook.erase(iter);
                            Klog::number--;  //To decrease the showed number
                        }
                        slotClearBtn();
                        readAwardsStatus();
                        showLogList();
                        showAwardsNumbers();
                        return;
                        break;
                    case QMessageBox::No:
                        break;
                    default: // just for sanity
                        return;
                    break;
                }
            }
        }
    }
}

void Klog::readAwardsStatus(){
// Re-read the DXCC and WAZ status. Maybe I could extract to another function...
        Klog::LogBook::iterator ite;
        for ( ite = logbook.begin(); ite != logbook.end(); ++ite ){
            dxcc.worked(world.findEntity((*ite).getQrz().toUpper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
            if ((*ite).gotTheQSL()){
                dxcc.confirmed(world.findEntity((*ite).getQrz().toUpper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
            }

            waz.worked(world.getCqzFromCall((*ite).getQrz().toUpper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
            if ((*ite).gotTheQSL()){
                waz.confirmed(world.getCqzFromCall((*ite).getQrz().toUpper()), adif.band2Int((*ite).getBand()), adif.mode2Int((*ite).getMode()));
            }
        }
}

// To print the whole log in the botton box
void Klog::showLogList(){
//qDebug() << "KLog::showLogList" << endl;
  if (logbook.isEmpty()){	// if no QSOs, we do not show the log ;-)
    return;
  }

//  while (logbook.


  /*QList<Qso>::const_iterator it;
 for (i = list.constBegin(); i != list.constEnd(); ++i)
     qDebug() << *i << endl;*/

/*  for (int i = 0; i < 10; ++i)
     items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));*/

  QList<QTreeWidgetItem *> items;

  logTreeWidget->clear();	// Clear the log

//  logTreeWidget->insertTopLevelItems(0, items);

//ESTOY AQUI TRATANDO DE MOSTRAR EL LOG EN EL QTREEWIDGET DE UNA FORMA EFICIENTE


    Klog::LogBook::iterator it;
    //TODO: CALLS COULD BE IN COLORS TO SHOW IF WORKED/NEEDED, ...
    // re-implementation using paintcell as in cluster is needed to do so
    for ( it = logbook.begin(); it != logbook.end(); ++it ){
        QTreeWidgetItem * item = new QTreeWidgetItem( logTreeWidget, 0 );
        // item->setText( 0, QString::number((*it).getNumb()) );
        item->setText( 0, getNumberString((*it).getNumb())  );
        item->setText( 1, (*it).getDateTime().toString("yyyy-MM-dd") );
        item->setText( 2, (*it).getDateTime().toString("hh:mm") );
        item->setText( 3, (*it).getQrz().toUpper() );
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
   before showing it in the logTreeWidget in order to be able to sort it.
*/
//qDebug() << "KLog::getNumberString" << endl;

    if (intNumber < 10){
        //qDebug() << "Number: " << "     0" + QString::number(intNumber) << endl;
        return "000000" + QString::number(intNumber);
    }else if (intNumber < 100){
        //qDebug() << "Number: " << "00000" + QString::number(intNumber) << endl;
        return "00000" + QString::number(intNumber);
    }else if (intNumber < 1000){
        //qDebug() << "Number: " << "    " + QString::number(intNumber) << endl;
        return "0000" + QString::number(intNumber);
    }else if (intNumber < 10000){
        //qDebug() << "Number: " << "   " + QString::number(intNumber) << endl;
        return "000" + QString::number(intNumber);
    }else if (intNumber < 100000){
        //qDebug() << "Number: " << "  " + QString::number(intNumber) << endl;
        return "00" + QString::number(intNumber);
    }else if (intNumber < 1000000){
        //qDebug() << "Number: " << " " + QString::number(intNumber) << endl;
        return "0" + QString::number(intNumber);
    }else if (intNumber < 10000000){
        //qDebug() << "Number: " << QString::number(intNumber) << endl;
        return QString::number(intNumber);
    }else{
        //qDebug() << "Number2: " << QString::number(intNumber) << endl;
        return QString::number(intNumber);
    }
}

void Klog::slotQSLRec(){
qDebug() << "KLog::slotQSLRec" << endl;
// 	wasConfirmed = qso.gotTheQSL(); // Was this QSO previously confirmed
// 	if (!wasConfirmed){
// 		confirmed++; // checked
// 		wasConfirmed = true;
// 	}
  if ((!qso.gotTheQSL()) || (modify) ){
    Klog::j = qso.getNumb();
    qslRec = QDate::currentDate();
    Klog::LogBook::iterator iter;
    for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
      if ( Klog::j == (*iter).getNumb() ){
    (*iter).QslRec('Y');
    if (qslRec.isValid()){
            (*iter).setQslRecDateOn(qslRec);
    }
    needToSave = true;
        dxcc.confirmedString(kk, (qso.getBand()).toUpper() ,  (qso.getMode()).toUpper());

        waz.confirmedString(world.getCqzFromCall(qso.getQrz()), qso.getBand(),qso.getMode());
        showLogList();
        showWhere(kk);
        //entityState(kk);
        showAwardsNumbers();
      }
    }
  }
}


void Klog::slotQSLSent(){
//We have sent the QSL
qDebug() << "KLog::slotQSLSent" << endl;
    if (!qso.sentTheQSL()){
        Klog::j = qso.getNumb();
        qslSen = QDate::currentDate();
        Klog::LogBook::iterator iter;
        for ( iter = logbook.begin(); iter != logbook.end(); ++iter ){
            if ( Klog::j == (*iter).getNumb() ){
                (*iter).QslSent('Y');
                if (qslSen.isValid()){
                    (*iter).setQslSenDateOn(qslSen);
                }
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
//qDebug() << "KLog::slotQSLRecSent" << endl;
    slotQSLRec();
    slotQSLSent();
}

void Klog::slotUpdateTime(){
//qDebug() << "KLog::slotUpdateTime";
    if (timeInUTC){
        //dateTimeContinuous = QDateTime::currentDateTime(Qt::UTC);
        dateTimeContinuous = (QDateTime::currentDateTime()).toUTC();
    }else{ // I am not in realTime
    //	dateTimeContinuous = QDateTime::currentDateTime(Qt::LocalTime);
      dateTimeContinuous = (QDateTime::currentDateTime()).toLocalTime();
    }

    if ( (!modify) && (realTimeLog) ){
        //qsoDateTime->setAutoAdvance (true);
        dateTime = dateTimeContinuous;
        qsoDateEdit->setDate(dateTime.date());
        qsoTimeEdit->setTime(dateTime.time());
        //qsoDateTime->setDateTime(dateTime);
    }else{
        //qsoDateTime->setAutoAdvance (false);
    }
    //if ((realTimeLog) && (modify)){
        // This checking is just a test... maybe it is not necesary to update
        //    qsoDateTime->setDateTime(dateTime);
    //}
}
/*
void Klog::showTip(){
// TODO: Provide the list of tips
// To show a tip on the begining of KLog
// Another way could be receibing an integer to show one or other tip. The tip number can be saved in the configuration file.
//	qDebug() << "KLog::showTip" << endl;
    KlogTip *klogtip = new KlogTip();

    klogtip->show(); // Single threaded // exec


}
*/

void Klog::fillDXCCList(){	// Fill the KLog tab
//qDebug() << "KLog::fillDXCCList: " << endl;
    int howManyEntities = world.howManyEntities();
//qDebug() << "KLog::fillDXCCList: " << QString::number(howManyEntities) << endl;


    dxccTreeWidget->clear();

    //TODO: Should show colors (worked, confirmed, .
    // re-implementation using paintcell as in cluster is needed to do so
    for ( i = 1; i <= howManyEntities ; i++ ) {

//		dxccItem * item = new dxccItem( dxccTreeWidget, 0 );
        QTreeWidgetItem * item = new QTreeWidgetItem( dxccTreeWidget, 0 );

        item->setText( 0, getShortNumberString(i)  );
        item->setText( 1, (world.getEntByNumb(i)).getEntity() );
        item->setText( 2, (world.getEntByNumb(i)).getPfx() );


        if (dxcc.isConfirmedBand(i, adif.band2Int("10M"))){
            item->setText( 3, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("10M"))){
            item->setText( 3, "W" );
        }else {
            item->setText( 3, "N" );
        }

        if (dxcc.isConfirmedBand(i, adif.band2Int("12M"))){
            item->setText( 4, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("12M"))){
            item->setText( 4, "W" );
        }else {
            item->setText( 4, "N" );
        }

        if (dxcc.isConfirmedBand(i, adif.band2Int("15M"))){
            item->setText( 5, "C" );
        }else if (dxcc.isWorkedBand(enti, adif.band2Int("15M"))){
            item->setText( 5, "W" );
        }else {
            item->setText( 5, "N" );
        }

        if (dxcc.isConfirmedBand(i, adif.band2Int("17M"))){
            item->setText( 6, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("17M"))){
            item->setText( 6, "W" );
        }else {
            item->setText( 6, "N" );
        }


        if (dxcc.isConfirmedBand(i, adif.band2Int("20M"))){
            item->setText( 7, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("20M"))){
            item->setText( 7, "W" );
        }else {
            item->setText( 7, "N" );
        }

        if (dxcc.isConfirmedBand(i, adif.band2Int("30M"))){
            item->setText( 8, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("30M"))){
            item->setText( 8, "W" );
        }else {
            item->setText( 8, "N" );
        }

        if (dxcc.isConfirmedBand(i, adif.band2Int("40M"))){
            item->setText( 9, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("40M"))){
            item->setText( 9, "W" );
        }else {
            item->setText( 9, "N" );
        }

        if (dxcc.isConfirmedBand(i, adif.band2Int("80M"))) {
            item->setText( 10, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("80M"))){
            item->setText( 10, "W" );
        }else {
            item->setText( 10, "N" );
        }

        if (dxcc.isConfirmedBand(i, adif.band2Int("160M"))){
            item->setText( 11, "C" );
        }else if (dxcc.isWorkedBand(i, adif.band2Int("160M"))){
            item->setText( 11, "W" );
        }else {
            item->setText( 11, "N" );
        }
    }
}


QString Klog::getShortNumberString(const int intNumber){
/* This is to pass from an integer to an "always-the-same-length" string the number
   before showing it in the logTreeWidget in order to be able to sort it.
*/
//qDebug() << "KLog::getShortNumberString" << endl;

    if (intNumber < 10){
        return "00" + QString::number(intNumber);
    }else if (intNumber < 100){
        return "0" + QString::number(intNumber);
    }else{
        return QString::number(intNumber);
    }
}


/********************************************************************************
****                        Begining of Printing Stuff                       ****
********************************************************************************/
void Klog::filePrint(){
// Part of this code comes from KEdit
//  bool aborted = false;
//qDebug() << "KLog::filePrint - DELETED TP HELP THE QT4 MIGRATION - TO BE RESTORED ASAP";

      QMessageBox msgBox;
      msgBox.setText(i18n("KLog message:"));
      QString str = i18n("This function has been deleted to help the QT4 migration.\nIt will be restored ASAP");
      msgBox.setInformativeText(str);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.exec();

    // First task to do is sort the log

/*
    sortLog();
    int progresStep = 0;

    QProgressDialog progress(i18n("Sorting the log..."), i18n("Abort sorting"), 0, Klog::number);
    bool longQRZ = false;



QString headerLeft = i18n("Printing date: ") + (QDate::currentDate()).toString(Qt::LocalDate);


//QString headerLeft = i18n("Printing date: %1");
  QString headerMid = "KLog-" + Klog::KLogVersion + " - http://jaime.robles.es/klog";
  QString headerRight;

  QString headerLog = (i18n("Number")).leftJustified(6,' ') + "\t" + (i18n("Date")).leftJustified(10,' ') + "\t" + (i18n("Time")).leftJustified(5,' ') + "\t" + (i18n("QRZ")).leftJustified(10,' ') + "\t" + i18n("RST(tx/rx)") +"\t" + (i18n("Band")).leftJustified(5,' ') + "\t" + (i18n("Mode")).leftJustified(7,' ');


  QFont printFont ("Times", 10 );
  QFont headerFont(printFont);
  headerFont.setBold(true);
  QFontMetrics printFontMetrics(printFont);
  QFontMetrics headerFontMetrics(headerFont);
  QPrinter *printer = new QPrinter;
     if(printer->setup(this) ) {

    // set up KPrinter
        printer->setFullPage(false);
        printer->setCreator("KLog");
//        if ( !m_caption.isEmpty() )
//            printer->setDocName(m_caption);

        QPainter *p = new QPainter;
        p->begin( printer );
        Q3PaintDeviceMetrics metrics( printer );
        int dy = 0;
        p->setFont(headerFont);
        int w = printFontMetrics.width("M");
//		p->setTabStops(8*w);

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
            p->drawText(0, y, metrics.width(), y, Qt::TextExpandTabs | Qt::TextWordWrap, headerLog);
            y += dy;

//        for ( it = logbook.begin(); it != logbook.end(); ++it ){
            while(lineCount < maxLineCount) {
//              QString text = eframe->textLine(lineCount);
                if ((*it).getQrz().length() >= 8){
                    longQRZ = true;
                }else{
                    longQRZ = false;
                }

                QString text = (QString::number((*it).getNumb())).leftJustified(6,' ') + "\t" + (*it).getDateTime().toString("dd-MM-yyyy") + "\t" + (*it).getDateTime().toString("hh:mm") + "\t" + ((*it).getQrz()).leftJustified(10,' ');

// 				if (!longQRZ)  // If it is a SHORT QRZ, we need only TWO tabs
// 					text = text + "\t";
// 				if(((*it).getQrz()).count('/'))
// 					text = text + "\t";


// 				if ((QString::number((*it).getRstrx())).length()>2)
// 					text = text + "      ";
// 				else
// 					text = text + "       ";

                text = text + "\t" + ( (QString::number((*it).getRsttx())).leftJustified(4,' ') + "/" + (QString::number((*it).getRstrx())).leftJustified(3,' ')).leftJustified(8,' ') + "\t" + ((*it).getBand()).leftJustified(5,' ') + "\t" + ((*it).getMode()).leftJustified(7,' ');

                it++;

                progresStep++;
                if (showProgressDialog){
                    progress.setValue( progresStep );
                    qApp->processEvents();
                }
                if ( progress.wasCanceled())
                    return;

                longQRZ = false;
                if( text == "" )
                    text = " ";     // don't ignore empty lines
                QRect r = p->boundingRect(0, y, body.width(), body.height(), Qt::TextExpandTabs | Qt::TextWordWrap, text);
                dy = r.height();
                if (y+dy > metrics.height()) break;
                if ((*it).getNumb() != 0){
                    p->drawText(0, y, metrics.width(), metrics.height() - y, Qt::TextExpandTabs | Qt::TextWordWrap, text);
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
        if ( progress.wasCanceled())
            return;
        p->end();
        delete p;
    }
    delete printer;
*/

// This is just to show the state but it does nothing
//     if (aborted)
//       setGeneralStatusField(i18n("Printing aborted."));
//     else
//       setGeneralStatusField(i18n("Printing complete."));

}

void Klog::sortLog(){
// I will read the Log from the UI and sorting using the numbers.
//TODO: This sorting is highly inefficient. It should be rewritten and optimized
qDebug() << "KLog::sortLog" << endl;

  if (logbook.isEmpty()){	// if no QSOs, we do not show the log ;-)
    return;
  }

  int progresStep = 0;
  Qso tQso; // Temp QSO
  oLogbook.clear();                             // an empty list
  Klog::LogBook::iterator it;
  QTreeWidgetItemIterator itl( logTreeWidget );

/*  Q3ProgressDialog progress( i18n("Sorting the log..."), i18n("Abort sorting"), Klog::number,
                          this, "progress", TRUE );*/
  QProgressDialog progress(i18n("Sorting the log..."), i18n("Abort sorting"), 0, Klog::number);

//TODO: Write the sorting function: Deleted to ease the QT4 migration
//    while (*itl){
//    //for ( ; itl.current(); ++itl ){
//      for ( it = logbook.begin(); it != logbook.end(); ++it ){  //We run the log...
//        if ( (*it).getNumb() == (itl.current()->text(0)).toInt() ){
//          progresStep++;
//          if (showProgressDialog){
//            progress.setValue( progresStep );
//            qApp->processEvents();
//          }
//          tQso = (*it);
//          if ( progress.wasCanceled())
//            return;
//          oLogbook.append(tQso);
//        }
//          if ( progress.wasCanceled())
//            return;
//
//      }
//    itl++;
//    }
  logbook = oLogbook;
}

/********************************************************************************
****                        End of Printing Stuff                            ****
********************************************************************************/

/********************************************************************************
**** The following slots are for the Cluster support                         ****
*********************************************************************************/


void Klog::slotClusterConnect(){
    if (dxClusterConnected)
        return; // If we are connected we don't want to start another connection

    if ((DXClusterServerToUse.section(':',1,1)).toInt() <= 0){
      QMessageBox msgBox;
      msgBox.setText(i18n("KLog message:"));
      QString str = i18n("Check your cluster settings\nin Setup->Preferences\nbefore connecting!");
      msgBox.setInformativeText(str);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.exec();
      return;
    }

    // create the socket and connect various of its signals
    socket = new QTcpSocket( this );
    connect( socket, SIGNAL(connected()), SLOT(slotClusterSocketConnected()) );
    connect( socket, SIGNAL(connectionClosed()), SLOT(slotClusterSocketConnectionClosed()) );
    connect( socket, SIGNAL(readyRead()), SLOT(slotClusterSocketReadyRead()) );
    connect( socket, SIGNAL(error(int)), SLOT(slotClusterSocketError(int)) );
    // connect to the server

    dxclusterListWidget->setSortingEnabled ( false);
    downTabs->setCurrentIndex(1); // We set the active tab to be the cluster's
    dxClusterSpotItem * item = new dxClusterSpotItem(dxclusterListWidget, i18n("Trying to connect to the server\n"), defaultColor);
    socket->connectToHost( DXClusterServerToUse.section(':',0,0), (DXClusterServerToUse.section(':',1,1)).toInt() );
}

void Klog::slotClusterCloseConnection(){
//qDebug() << "KLog::slotClusterCloseConnection" << endl;
    if (!dxClusterConnected)
        return; // If we are not we can't close any conection
    socket->close();
    if ( socket->state() == QAbstractSocket::ClosingState ) {
        // We have a delayed close.
        connect( socket, SIGNAL(delayedCloseFinished()), SLOT(socketClosed()) );
    } else {
        // The socket is closed.
        slotClusterSocketClosed();
    }
}

void Klog::slotClusterSendToServer(){
//qDebug() << "KLog::slotClusterSendToServer" << endl;
    if (!dxClusterConnected)
        return; // If we are not connected we cannot send any text to the server
    if ( ClusterkLineEditInPut->text().length() < 1 ) {
        return;
    }
    //  write to the server
    QTextStream os(socket);
    os << ClusterkLineEditInPut->text() << "\n";
    ClusterkLineEditInPut->clear();
}

void Klog::slotClusterSocketReadyRead(){
// read from the server
// The while could block the flow of the program?
// ATENTION: The Cluster freq is in KHz and KLog works in MHz!
    while ( socket->canReadLine() ) {
        dxClusterString =  socket->readLine();
        // changed this to trimmed from simplfied() so the output string is easier to read as a spot
        dxClusterString = dxClusterString.trimmed();
        // Put here to check for callsigns that crash klog. To do with the QString ASSERT error.
        qDebug() << "KLog::slotClusterSocketReadyRead: " << dxClusterString;

        QStringList tokens = dxClusterString.split(" ", QString::SkipEmptyParts);
        //QStringList tokens = QStringList::split( ' ', dxClusterString );

        // It is a "DX de SP0TTER FREC DXCALL"
        //0 = DX, 1 = de, 2 = spotter, 3 = Freq, 4 = dxcall, 5 = comment
        //tokens[0] = tokens[0].simplified(); // we remove the spaces just in case it is a freq


        if ((tokens[0] == "DX") && (tokens[1] == "de")){
            if ( (!dxClusterHFSpots) && (adif.isHF(adif.KHz2MHz(tokens[3])))){ //Check the freq
                return;
            }
            if ( (!dxClusterCWSpots) && (adif.isCW(adif.KHz2MHz(tokens[3])))){
                return;
            }
            if ( (!dxClusterSSBSpots) && (adif.isSSB(adif.KHz2MHz(tokens[3])))){
                return;
            }
            if ( (!dxClusterVHFSpots) && (adif.isVHF(adif.KHz2MHz(tokens[3])))){
                return;
            }
            if ( (!dxClusterWARCSpots) && (adif.isWARC(adif.KHz2MHz(tokens[3])))){
                return;
            }
if (	(!dxClusterConfirmedSpots) && (needToWorkFromCluster(tokens[4],adif.freq2Int(adif.KHz2MHz(tokens[3]))) == 0)) {
                return;
            }
        switch(needToWorkFromCluster(tokens[4],adif.freq2Int(adif.KHz2MHz(tokens[3])))){
            // 0 confirmed, 1 worked, 2 new one, -1 error
        case 0: // Continue
            //CONFIRMED - Red
            //neededWorkedConfirmed = 2;
            dxSpotColor = confirmedColor;
        break;
        case 1: // Continue
            //WORKED - Yellow
        {//	neededWorkedConfirmed = 1;
            dxSpotColor = workedColor;
// 			bandMapSpotItem * item = new bandMapSpotItem(bandmapListView, tokens[3], tokens[4], tokens[2], dxSpotColor);
        }
        break;
        case 2:{
            //New one - Green
            //neededWorkedConfirmed = 0;
            dxSpotColor = neededColor;
// 			bandMapSpotItem * item = new bandMapSpotItem(bandmapListView, tokens[3], tokens[4], tokens[2], dxSpotColor);

        }
        break;
        case 3:{
            //New one - Green
            //neededWorkedConfirmed = 0;
            dxSpotColor = newOneColor;
// 			bandMapSpotItem * item = new bandMapSpotItem(bandmapListView, tokens[3], tokens[4], tokens[2], dxSpotColor);
        }

        break;
        case -1:
        // An error
            //neededWorkedConfirmed = 3;
            dxSpotColor = defaultColor;
        break;
        default:
            //neededWorkedConfirmed = 3;
            dxSpotColor = defaultColor;
        break;
        }// Closes the Switch
        // Maybe we have just asked for a "SH/DX" command so data is presented
        // in another way.
        // 0 = freq, 1 = dxcall, 2 = date, ...

        }else if (    ((adif.isHF(adif.KHz2MHz(tokens[0]))) || (adif.isVHF(adif.KHz2MHz(tokens[0]))))  && (tokens[0] != "***" )  ) {
        // I am nearly copying the previous section... so maybe an optimization should be done.

            if ( (!dxClusterCWSpots) && (adif.isCW(adif.KHz2MHz(tokens[0])))){
                return;
            }
            if ( (!dxClusterSSBSpots) && (adif.isSSB(adif.KHz2MHz(tokens[0])))){
                return;
            }
            if ( (!dxClusterHFSpots) && (adif.isHF(adif.KHz2MHz(tokens[0])))){ //Check the freq
                return;
            }
            if ( (!dxClusterVHFSpots) && (adif.isVHF(adif.KHz2MHz(tokens[0])))){
                return;
            }

            if ( (!dxClusterConfirmedSpots) && (dxcc.isConfirmed(world.findEntity(adif.KHz2MHz(tokens[0]))))){
                return;
            }

        switch(needToWorkFromCluster(tokens[1],adif.freq2Int(adif.KHz2MHz(tokens[0])))){
        // 0 confirmed, 1 worked, 2 new one, -1 error
        case 0: // Continue
            //CONFIRMED - Red
            //neededWorkedConfirmed = 2;
            dxSpotColor = confirmedColor;
        break;
        case 1: //WORKED
        {
            //neededWorkedConfirmed = 1;
            dxSpotColor = workedColor;
/*			bandMapSpotItem * item = new bandMapSpotItem(bandmapListView, tokens[0], tokens[1], "Unknown", dxSpotColor);*/
        }
        break;
        case 2://New one - Green
            {
            //neededWorkedConfirmed = 0;
            dxSpotColor = neededColor;
/*			bandMapSpotItem * item = new bandMapSpotItem(bandmapListView, tokens[0], tokens[1], "Unknown", dxSpotColor);*/
        }
        break;
        case 3://New one - Green
            {
            //neededWorkedConfirmed = 0;
            dxSpotColor = newOneColor;
/*			bandMapSpotItem * item = new bandMapSpotItem(bandmapListView, tokens[0], tokens[1], "Unknown", dxSpotColor);*/
        }
        break;

        case -1: // An error
            //neededWorkedConfirmed = 3;
            dxSpotColor = defaultColor;
        break;
        default:
            //neededWorkedConfirmed = 3;
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
//        dxClusterSpotItem * item = new dxClusterSpotItem(dxclusterListWidget, dxClusterString, dxSpotColor);
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(dxClusterString);
        item->setFont(QFont ("Courier", 8));
        dxclusterListWidget->insertItem(0,item);
        dxSpotColor = defaultColor; // The color should be default by default
    }
    }
}

void Klog::slotClusterSocketConnected(){
//qDebug() << "KLog::slotClusterSocketConnected" << endl;
    dxClusterSpotItem * item = new dxClusterSpotItem(dxclusterListWidget, i18n("Connected to server"), defaultColor);
    dxClusterConnected = true;

    if ( dxClusterConnected ) {
        bool ok;
        QString callsignText = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("Enter you callsign to connect to the cluster:"), QLineEdit::Normal, "", &ok);
        QTextStream os(socket);
        if ( callsignText.length() > 2 && ok ) {
            os << callsignText << "\n";
        } else {
            os << "Not logged on, you may to enter your callsign again." << "\n";
        }
    }
}

void Klog::slotClusterSocketConnectionClosed(){
//qDebug() << "KLog::slotClusterSocketConnectionClosed" << endl;
    dxClusterSpotItem * item = new dxClusterSpotItem(dxclusterListWidget, i18n("Connection closed by the server"), defaultColor);
    dxClusterConnected = false;
}

void Klog::slotClusterSocketClosed(){
//qDebug() << "KLog::slotClusterSocketCluster" << endl;
    dxClusterSpotItem * item = new dxClusterSpotItem(dxclusterListWidget, i18n("Connection closed"), defaultColor );
    dxClusterConnected = false;
}

void Klog::slotClusterSocketError( int e ){
//qDebug() << "KLog::slotClusterSocketError" << endl;
//QSocket::ErrConnectionRefused - if the connection was refused
//QSocket::ErrHostNotFound - if the host was not found
//QSocket::ErrSocketRead - if a read from the socket failed
    if (e == QAbstractSocket::ConnectionRefusedError){
//  QMessageBox::about( this, i18n("KLog message:"),
// i18n("DX-Cluster server refused the connection\n")+DXClusterServerToUse+
// i18n("\nMaybe you are already connected, use another call like \"YOURCALL-1\"!")
//  );
                       QMessageBox msgBox;
                       msgBox.setText(i18n("KLog message:"));
                       QString str = i18n("DX-Cluster server refused the connection\n")
                       +DXClusterServerToUse+
                       i18n("\nMaybe you are already connected, use another call like \"YOURCALL-1\"!");
                       msgBox.setInformativeText(str);
                       msgBox.setStandardButtons(QMessageBox::Ok);
                       msgBox.setDefaultButton(QMessageBox::Ok);
                       msgBox.setIcon(QMessageBox::Warning);
                       msgBox.exec();

    }else if (e == QAbstractSocket::HostNotFoundError){
                       QMessageBox msgBox;
                       msgBox.setText(i18n("KLog message:"));
                       QString str = i18n("Host not found:\n")
                       +DXClusterServerToUse
                       + i18n("\nCheck your network settings!");
                       msgBox.setInformativeText(str);
                       msgBox.setStandardButtons(QMessageBox::Ok);
                       msgBox.setDefaultButton(QMessageBox::Ok);
                       msgBox.setIcon(QMessageBox::Warning);
                       msgBox.exec();



    }/*else if (e == QTcpSocket::ErrSocketRead){
                       QMessageBox msgBox;
                       msgBox.setText(i18n("KLog message:"));
                       QString str = i18n("Error socket read,\n")
                       + i18n("unknown error!");
                       msgBox.setInformativeText(str);
                       msgBox.setStandardButtons(QMessageBox::Ok);
                       msgBox.setDefaultButton(QMessageBox::Ok);
                       msgBox.setIcon(QMessageBox::Warning);
                       msgBox.exec();



    } */

  dxClusterSpotItem * item = new dxClusterSpotItem(dxclusterListWidget, i18n("Error number %1 occurred").arg(e), defaultColor);
}

void Klog::slotClusterClearInputLine(){
//qDebug() << "KLog::slotClusterClearInputLine" << endl;
    ClusterkLineEditInPut->setText( "" );
}


int Klog::needToWorkFromCluster(const QString &tqrz, const int tband){
// 0 confirmed, 1 worked, 2 needed, 3 new one, -1 error
//qDebug() << "KLog::needToWorkFromCluster: (" << tqrz << "/" << QString::number(tband) << ")"<< endl;
    if (tband == -1){ // If the spot's band is not supported by Klog, we can't say if worked B4
        return -1;
    }
    dxClusterEnti = 0;
    dxClusterCallLen = (tqrz).length();
    dxClusterEnti = world.findEntity(tqrz.toUpper());


    if (dxClusterEnti !=0 ){
        if (dxcc.isConfirmedBand(dxClusterEnti, tband)){  // Confirmed in this band
            return 0;
        }else if(dxcc.isWorkedBand(dxClusterEnti, tband)){	// Worked in this band
            return 1;
        } else if(dxcc.isWorked(dxClusterEnti)){	// Worked in other bands
            return 2;
        }else{		// It's a new one!
            return 3;
        }
    }

    return 3; // WFWL (Work First, Worry Later) so if we do not know we work it.
}

// This takes a DX-spot from the DXCluster window and copies to the QSO entry box
// when the user clicks on it.
void Klog::slotClusterSpotToLog(QListWidgetItem * item, int row){
//cout << "KLog::slotClusterSpotToLog" << endl;
    if (item)
        dxClusterString = item->text();
    else
        return;
    slotClearBtn();
    QStringList tokens = dxClusterString.split(" ", QString::SkipEmptyParts);
    //QStringList tokens = QStringList::split( ' ', dxClusterString );


  // It is a "DX de SP0TTER FREC DXCALL", if not... we won't try to add to the log ;-)
    if ((tokens[0] == "DX") && (tokens[1] == "de")){
        if ((tokens[1]).length() != 4){
            qrzLineEdit->setText(tokens[4]);
            bandComboBox->setCurrentIndex(adif.freq2Int(  adif.KHz2MHz(tokens[3])));
            freqlCDNumber->display(tokens[3].toDouble());      // We show the frequency in the box in MHz
        }
    }else if ( ((adif.isHF(adif.KHz2MHz(tokens[0]))) || (adif.isVHF(adif.KHz2MHz(tokens[0]))))  && (tokens[0] != "***" ) ){
        if ((tokens[1]).length() != 0){
            qrzLineEdit->setText(tokens[1]);
            bandComboBox->setCurrentIndex(adif.freq2Int(adif.KHz2MHz(tokens[0])));
            freqlCDNumber->display(((tokens[0])).toDouble()); // We show the frequency in the box (in KHz)

        }
    }else    // It is NOT an spot but an announce or similar.
        return;
// Getting and proposing a MODE could be good...
    enti = 0;
    if (((qrzLineEdit->text()).toUpper()).length() != 0){
        enti = world.findEntity((qrzLineEdit->text()).toUpper());
    }
    showWhere(enti);
}

void Klog::slotClusterSpotCheck(QListWidgetItem * item){
//cout << "KLog::slotClusterSpotCheck" << endl;
  if (item)
    dxClusterString = item->text();
  else{
    //cout << "KLog: slotClusterSpotCheck Limpiamos con clearEntityBox-1" << endl;
    clearEntityBox();
    return;
  }
  enti = 0;
  QStringList tokens = dxClusterString.split(" ", QString::SkipEmptyParts);
  //QStringList tokens = QStringList::split( ' ', dxClusterString );
  // It is a "DX de SP0TTER FREC DXCALL", if not... we won't try to add to the log ;-)
  if ((tokens[0] == "DX") && (tokens[1] == "de")){
    if ((tokens[4]).length() != 0)
      enti = world.findEntity((tokens[4]).toUpper());

  }else if ( ((adif.isHF(adif.KHz2MHz(tokens[0]))) || (adif.isVHF(adif.KHz2MHz(tokens[0]))))  && (tokens[0] != "***" ) ){
    if ((tokens[1]).length() != 0)
      enti = world.findEntity((tokens[1]).toUpper());

  }else{    // It is NOT an spot but an announce or similar.
    //cout << "KLog: slotClusterSpotCheck Limpiamos con clearEntityBox-2" << endl;
    clearEntityBox();
    return;
  }
  //QRZ = tokens[4]
  // Band = adif.freq2Int(tokens[3])
//  bandComboBox->setCurrentIndex(adif.freq2Int(tokens[3]));
// Getting and proposing a MODE could be good...

  showWhere(enti);


}


//void Klog::addDXSpotToBandMap(QString freq, QString dx, QString from){


//cout << "KLog::addDXSpotToBandMap" << endl;
/*
        QListViewItem * item = new QListViewItem( logTreeWidget, 0 );
        item->setText( 0, getNumberString(qso.getNumb())  );
        item->setText( 1, qso.getDateTime().toString("yyyy-MM-dd") );
*/
/*
          logbook.append(qso);
          if (enti != 0){
            dxcc.workedString(enti, qso.getBand(), qso.getMode());
            waz.workedString( world.getCqz(), qso.getBand(), qso.getMode() );
          }
        QListViewItem * item = new QListViewItem( logTreeWidget, 0 );
//        item->setText( 0, QString::number(qso.getNumb()) );
        item->setText( 0, getNumberString(qso.getNumb())  );
        item->setText( 1, qso.getDateTime().toString("yyyy-MM-dd") );
        item->setText( 2, qso.getDateTime().toString("hh:mm"));
        item->setText( 3, qso.getQrz().toUpper() );
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
        if ((!url.isEmpty())||(url.port() != -1))
        return true;
    else
        return false;
}

void Klog::slotBugReport(){
//cout << "KLog::slotBugReport" << endl;
//	if( bugReport == 0 ){
//		bugReport = new KBugReport();
//	}
//	bugReport->show();

QString auxString = i18n("Please send your bug-report/wish to jaime@robles.es\nInclude the KLog version you are using (") +Klog::KLogVersion + ")"+ i18n(" All your suggestion will be welcome!");

// QMessageBox::about( this, i18n("KLog message: Bug-reporting"),
//                             i18n(auxString+"\n	All your suggestions will be welcome!"));

 QMessageBox msgBox;
 msgBox.setText(auxString);
 msgBox.exec();

}

void Klog::slotModeChanged (int i){
//cout << "KLog::slotModeChanged" << endl;

// Commented to fix an error reported by EA4RCT (Alvaro)
//	if (((qrzLineEdit->text()).length())==0)
//		return;

    if (i == 0){ // The mode is SSB
        //    TRecBox->setHidden(true);
        TRecBox->setEnabled(false);
        //    TSendBox->setHidden(true);
        TSendBox->setEnabled(false);
        //    textLabel16->setHidden(true);
        //    textLabel16->setEnabled(false);
        tTxValue = 0;
        tRxValue = 0;
        TSendBox->setMaximum(0);
        TRecBox->setMaximum(0);
    }else if (i == 2){ // The mode is FM
        //    TRecBox->setHidden(true);
        TRecBox->setEnabled(false);
//    TSendBox->setHidden(true);
    TSendBox->setEnabled(false);
//    textLabel16->setHidden(true);
 //   textLabel16->setEnabled(false); This is from the RST (deleted 27/feb/2005)
    tTxValue = 0;
    tRxValue = 0;
    TSendBox->setMaximum(0);
    TRecBox->setMaximum(0);
  }else if (i == 4){ // The mode is SSTV
//    TRecBox->setHidden(false);
    TRecBox->setEnabled(true);
//    TSendBox->setHidden(false);
    TSendBox->setEnabled(true);
//    textLabel16->setHidden(false);
//    textLabel16->setEnabled(true);
    tTxValue = 5;
    tRxValue = 5;
    TSendBox->setMaximum(5);
    TRecBox->setMaximum(5);
  }else if (i == 8){ // The mode is AM
//    TRecBox->setHidden(true);
    TRecBox->setEnabled(false);
//    TSendBox->setHidden(true);
    TSendBox->setEnabled(false);
//    textLabel16->setHidden(true);
//    textLabel16->setEnabled(false); This is from the RST (deleted 27/feb/2005)
    tTxValue = 0;
    tRxValue = 0;
    TSendBox->setMaximum(0);
    TRecBox->setMaximum(0);
  }else{
    TSendBox->setMaximum(9);
    TRecBox->setMaximum(9);
    tTxValue = 9;
    tRxValue = 9;
//    TRecBox->setHidden(false);
    TRecBox->setEnabled(true);
//    TSendBox->setHidden(false);
    TSendBox->setEnabled(true);
//    textLabel16->setHidden(false);
//    textLabel16->setEnabled(true);
  }
  imode = i;
  TSendBox->setValue(tTxValue);
  TRecBox->setValue(tRxValue);
  entityState(enti);
}

void Klog::slotBandChanged (){
//TODO: To check if this slot is really necessary.
//cout << "KLog::slotBandChanged: "<< QString::number(enti) << endl;
    if (((qrzLineEdit->text()).length())==0)
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


//Intended to complete the actual QSO if has been worked before.
void Klog::showIfPreviouslyWorked(){ // Uses previousQso and workedCall
//cout << "KLog::showIfPreviouslyWorked" << endl;
// The affected fields are:
//	Name, QTH, Locator, QSLVia & Manager
//To add a tab in the right box to show the data from previous QSOs for a call.
// This tab will have a button to copy the data into the actual QSO box if wished.
//	kk = 0;
    Qso prevQso;
    int _enti;
    if (!completeWithPrevious){ // If we have configured KLog not to use this feature
//		cout << "KLog::showIfPreviouslyWorked FALSE" << endl;
        return;

    }else{
//		cout << "KLog::showIfPreviouslyWorked: TRUE" << endl;
        kk = workedCall.findCall(qrzLineEdit->text());
        _enti = getEntityFromCall();

        if ((  kk > 0) ){
//			cout << "KLog::showIfPreviouslyWorked: Worked before: " << QString::number(kk) << endl;
            prevQso = getByNumber(kk);
            if ((prevQso.getQth()).length() >=2){
                qthkLineEdit->setText(prevQso.getQth());
            }
            if ((prevQso.getName()).length() >=2){
//				cout << "KLog::showIfPreviouslyWorked: Worked before - Name: " << prevQso.getName() << endl;
                namekLineEdit->setText(prevQso.getName());
            }
            dxLocator = prevQso.getLocator();
            locatorLineEdit->setText(dxLocator);
            slotLocatorChanged();


            // IOTA
            prepareIOTAComboBox(_enti);
            if (prevQso.getIotaNumber() != 0) {
                i = 0;
                i = adif.continent2Number((prevQso.getIotaContinent()));

                iotaComboBox->setCurrentIndex(i);
                iotaIntSpinBox->setValue(prevQso.getIotaNumber());
            }
            // Local Award
            prepareAwardComboBox(_enti);
            if (award.getReferenceNumber(prevQso.getLocalAward()) != -1 ) {
                awardsComboBox->setCurrentIndex(award.getReferenceNumber(prevQso.getLocalAward()));
                awardsComboBox->setEnabled(true);
            }

            completedWithPrevious = true;
            kk = 0;
        }else{ // If the call is NOT worked, we have to clean the texts
//		cout << "KLog::showIfPreviouslyWorked: no Worked before" << endl;
            if (completedWithPrevious){
//	cout << "KLog::showIfPreviouslyWorked: no Worked deleting..." << endl;
                qthkLineEdit->clear();
                namekLineEdit->clear();
                locatorLineEdit->clear();
                slotLocatorChanged();

//				iotaComboBox->setCurrentIndex(0);
                iotaIntSpinBox->setValue(0);
                prepareIOTAComboBox(_enti);
            //DELETE THE ISLAND
                awardsComboBox->setCurrentIndex(0);
                prepareAwardComboBox(_enti);
            //DELETE THE CODE


                completedWithPrevious = false;
            }
        }
    }

//
//		//QSL Info
//		QSLcomboBox->setItemText(previousQso.getQslVia());
//		if ((previousQso.getQslVia()).compare("No QSL") == 0){
//			qslVialineEdit->setDisabled(true);
//			// QSLInfotextEdit->setDisabled(true);
//			qslVialineEdit->clear();
//			// QSLInfotextEdit->clear();
//		}else{
//			if ((previousQso.getQslVia()).compare("Manager") == 0){
//				qslVialineEdit->setEnabled(true);
//				qslVialineEdit->setText(previousQso.getQslManager());
//			}else
//				qslVialineEdit->setDisabled(true);
            //        QSLInfotextEdit->setEnabled(true);
    //		QSLInfotextEdit->setText(previousQso.getQslInfo());
    //	}
        // IOTA
        ////cout << "KLog::showIfPreviouslyWorked - IOTA: " << QString::number(previousQso.getIotaNumber()) << endl;
        /*i = 0;
        if (previousQso.getIotaNumber() != 0) {
            i = adif.continent2Number((qso.getIotaContinent()));
            iotaComboBox->setCurrentIndex(i);
            iotaIntSpinBox->setValue(previousQso.getIotaNumber());
        }*/
        // Local Award
        /*if (previousQso.getLocalAward() != 0) {
            iotaComboBox->setCurrentIndex(previousQso.getIotaContinent());
            iotaIntSpinBox->setEnabled(true);
            iotaIntSpinBox->setValue(previousQso.getIotaNumber());
        }*/
//	}else{ // If not worked B4, we clean the boxes...
        // Is it really needed? It contradict the "auto IOTA box setting (prepareIOTAComboBox)
        /*i = 0;
        if (iotaIntSpinBox->value() != 0);
            i = iotaIntSpinBox->value();
        clearGUI();*/
//	}
}

void Klog::clearGUI(){
//cout << "KLog: clearGUI" << endl;
    qthkLineEdit->clear();
    namekLineEdit->clear();
    locatorLineEdit->clear();
    myLocatorLineEdit->clear();

    Klog::dxLocator="";
    Klog::myLocatorTemp="";
    slotLocatorChanged();
    slotMyLocatorChanged();
    remarksTextEdit->clear();
    //QSLcomboBox->clear();
    qslVialineEdit->clear();
    QSLInfotextEdit->clear();
    iotaComboBox->setCurrentIndex(0);
    iotaIntSpinBox->setValue(0);
    //iotaIntSpinBox->setEnabled(false);
}

void Klog::slotcompleteThePreviouslyWorked(){
//cout << "KLog::slotcompleteThePreviouslyWorked" << endl;
    Qso _previousQso;
    int _aa; //auxiliar just for this
    QString _aux;
/////// Progress dialog
    int _totalQsos = number; // QSOs in the log to be read
    int _progresStep = 0;
    int _i = 0;
// 	QProgressDialog _progress( i18n("Merging information..."), i18n("Abort"), 0,
//                           this, i18n("progress"), TRUE );

    QProgressDialog _progress(i18n("Merging information..."), i18n("Abort"), 0, _totalQsos);

    QString _progressLabel;
/////// Progress dialog

    Klog::LogBook::iterator _it1;
    Klog::LogBook::iterator _itEnd = logbook.end();
    Klog::LogBook::iterator _it2;


    for ( _it1 = logbook.begin(); _it1 != _itEnd; ++_it1 ){
        _i++;

        _aa = workedCall.findCall((*_it1).getQrz());

        if (_aa>=0){ // Call already worked, completing...
            //(*_it2) = getByNumber(_aa);

            _previousQso = getByNumber(_aa);

            for ( _it2 = logbook.begin(); _it2 != _itEnd; ++_it2 ){
                if (_previousQso.getNumb() == (*_it2).getNumb()){

                    if( ( ((*_it2).getName()).isEmpty() ) && (!(((*_it1).getName()).isEmpty()) ) ){

                       QMessageBox msgBox;
                       msgBox.setText(i18n("Warning: Callsign to complete found"));
                       QString str = i18n("Completing a call could cause data to be no accurate. Do you want this data to be merged?\n QSO N: #")
                       + QString::number((*_it2).getNumb()) +
                       i18n(" - Asign the name ") + (*_it1).getName() + i18n(" for ")
                       + _previousQso.getQrz() + "?";
                       msgBox.setInformativeText(str);
                       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
                       msgBox.setDefaultButton(QMessageBox::No);
                       int ret = msgBox.exec();


                        switch(ret){
                        case QMessageBox::Yes: // Continue
                            (*_it2).setName((*_it1).getName());
                            break;
                        case QMessageBox::No: // Continue
                            break;
                        }
                    }

                    if( ( ((*_it2).getQth()).isEmpty() ) && (!(((*_it1).getQth()).isEmpty()) ) ){

                       QMessageBox msgBox;
                       msgBox.setText(i18n("Warning: Callsign to complete found"));
                       QString str = i18n("Completing a call could cause data to be no accurate. Do you want this data to be merged?\n QSO N: #")
                       + QString::number((*_it2).getNumb()) +
                       i18n(" - Asign the QTH ") + (*_it1).getQth() + i18n(" for ") + _previousQso.getQrz() + "?";
                       msgBox.setInformativeText(str);
                       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
                       msgBox.setDefaultButton(QMessageBox::No);
                       int ret = msgBox.exec();

                        switch(ret){
                        case QMessageBox::Yes: // Continue
                            (*_it2).setQth((*_it1).getQth());
                            break;
                        case QMessageBox::No: // Continue
                            break;
                        }
                    }
                    if( ( ((*_it2).getLocator()).isEmpty() ) && (!(((*_it1).getLocator()).isEmpty()) ) ){
                       QMessageBox msgBox;
                       msgBox.setText(i18n("Warning: Callsign to complete found"));
                       QString str = i18n("Completing a call could cause data to be no accurate. Do you want this data to be merged?\n QSO N: #")
                       + QString::number((*_it2).getNumb())
                       + i18n(" - Asign the Locator ")
                       + (*_it1).getLocator() + i18n(" for ") + _previousQso.getQrz() + "?";
                       msgBox.setInformativeText(str);
                       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
                       msgBox.setDefaultButton(QMessageBox::No);
                       int ret = msgBox.exec();

                        switch(ret){

                        case QMessageBox::Yes: // Continue
                            (*_it2).setLocator((*_it1).getLocator());
                            break;
                        case QMessageBox::No: // Continue
                            break;
                        }
                    } // End Locator
//IOTA
                    if( ( ((*_it2).getIotaNumber())==0 ) && ( (*_it1).getIotaNumber()!=0 ) ){
                      QMessageBox msgBox;
                       msgBox.setText(i18n("Warning: Callsign to complete found"));
                       QString str = i18n("Completing a call could cause data to be no accurate. Do you want this data to be merged?\n QSO N: #")
                       + QString::number((*_it2).getNumb())
                       + i18n(" - Asign the IOTA reference ")
                       + (*_it1).getIota() + i18n(" for ")
                       + _previousQso.getQrz() + "?";
                       msgBox.setInformativeText(str);
                       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
                       msgBox.setDefaultButton(QMessageBox::No);
                       int ret = msgBox.exec();

                        switch(ret){

                        case QMessageBox::Yes: // Continue
                            (*_it2).setIota((*_it1).getIota());
                            break;
                        case QMessageBox::No: // Continue
                            break;
                        }


                    }else{ //cout << "NO IOTA" << endl;
                    } // End IOTA

//LOCAL AWARD




                    if( (((*_it1).getLocalAward()).length()>1 ) && !( ((*_it2).getLocalAward()).length()<1 ) ) {

                      QMessageBox msgBox;
                       msgBox.setText(i18n("Warning: Callsign to complete found"));
                       QString str = i18n("Completing a call could cause data to be no accurate. Do you want this data to be merged?\n QSO N: #")
                       + QString::number((*_it2).getNumb())
                       + i18n (" - Asign the Local Award reference ")
                       + (*_it1).getLocalAward()
                       + i18n(" for ")
                       + _previousQso.getQrz()
                       + "?";
                       msgBox.setInformativeText(str);

                       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
                       msgBox.setDefaultButton(QMessageBox::No);
                       int ret = msgBox.exec();

                        switch(ret){


                        case QMessageBox::Yes: // Continue
                            (*_it2).setLocalAward((*_it1).getLocalAward());
                            (*_it2).setLocalAwardNumber((*_it1).getLocalAwardNumber());

                            break;
                        case QMessageBox::No: // Continue
                            break;
                        }


                    }else{
                    } // End LOCAL AWARD





//QSL Info

if ( ( (((*_it1).getQslVia()).length()>1)  && !(((*_it2).getQslVia()).length()>1) ) ||
( (((*_it1).getQslManager()).length()>1)  && !(((*_it2).getQslManager()).length()>1) ) ||
( (((*_it1).getQslInfo()).length()>1)  && !(((*_it2).getQslInfo()).length()>1) ) )
{
  QMessageBox msgBox;
  msgBox.setText(i18n("Warning: Callsign to complete found"));
  QString str = i18n("Completing a call could cause data to be no accurate. Do you want this data to be merged?\n QSO N: #")
    + QString::number((*_it2).getNumb())
    + i18n(" - Do you want to copy the QSL information received in other QSOs for ")
    + (*_it2).getQrz()
    + "?";
  msgBox.setInformativeText(str);

  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();

                        switch(ret) {
                        case QMessageBox::Yes: // Continue
                            (*_it2).setQslVia((*_it1).getQslVia());
                            (*_it2).setQslManager((*_it1).getQslManager());
                            (*_it2).setQslInfo((*_it1).getQslInfo());
                            break;
                        case QMessageBox::No: // Continue
                            break;
                        }
                    }else{
                    } // End QSL Info





/*
        if (previousQso.getIotaNumber() != 0) {
            iotaComboBox->setCurrentIndex(adif.continent2Number(previousQso.getIotaContinent()));
            //iotaIntSpinBox->setEnabled(true);
            iotaIntSpinBox->setValue(previousQso.getIotaNumber());
        }
QSLINFO
previousQso.getQslManager()
previousQso.getQslVia()
previousQso.getQslInfo()
*/

                }

            }

        }else{
        }


        if (showProgressDialog){
            _progresStep++;
            if ( (_i % getProgresStepForDialog(_totalQsos) )== 0){ // To update the speed i will only show the progress once each 25 QSOs
                _progress.setValue( _progresStep );
                qApp->processEvents();
                _progressLabel = i18n("Merging calls: ");
                _progressLabel = _progressLabel + QString::number(_i) + " / " + QString::number(_totalQsos);
                _progress.setLabelText(_progressLabel);
            }
            if ( _progress.wasCanceled()){
                return;
            }
        }

    }

}

void Klog::getAllTheCallsFromLog(){
//cout << "KLog:: getAllTheCallsFromLog" << endl;
    int _aa; //auxiliar just for this

/////// Progress dialog
    int totalQsos = number; // QSOs in the log to be read
    int progresStep = 0;
    int _i = 0;
// 	QProgressDialog progress( i18n("Getting all the calls..."), i18n("Abort"), 0,
//                           this, i18n("progress"), TRUE );

     QProgressDialog progress(i18n("Getting all the calls..."), i18n("Abort"), 0, totalQsos);

     progress.setWindowModality(Qt::WindowModal);
    QString progressLabel;
/////// Progress dialog

    Klog::LogBook::iterator it;
    for ( it = logbook.begin(); it != logbook.end(); ++it ){
        _i++;
        _aa = workedCall.addCall((*it).getQrz(), (*it).getNumb());

        if (showProgressDialog){
            progresStep++;
            if ( (_i % getProgresStepForDialog(totalQsos) )== 0){ // To update the speed i will only show the progress once each 25 QSOs
                progress.setValue( progresStep );
                qApp->processEvents();
                progressLabel = i18n("Adding Call: ");
                progressLabel = progressLabel + QString::number(_i);
                progressLabel = progressLabel + " / ";
                progressLabel = progressLabel + QString::number(totalQsos);
                progress.setLabelText(progressLabel);
            }
            if ( progress.wasCanceled()){
                return;
            }
        }
    }
}

void Klog::completeAllQSOsFromLog(){
//qDebug() << "KLog::completeAllQSOsFromLog";

  Klog::LogBook::iterator it;

  for ( it = logbook.begin(); it != logbook.end(); ++it ){
    if (qso.getQrz() == (*it).getQrz() ){
      qso.setName((*it).getName());
      qso.setQth((*it).getQth());
      qso.setLocator((*it).getLocator());
      qso.setQslVia((*it).getQslVia());
      qso.setQslManager((*it).getQslManager());
      qso.setQslInfo((*it).getQslInfo());
      qso.setComment((*it).getComment());
      qso.setIota((*it).getIota());
    qso.setAddress ((*it).getAddress());
    qso.setARRLSect ((*it).getARRLSect());
          qso.setComment(remarksTextEdit->toPlainText());
  qso.setQslVia(QSLcomboBox->currentText());
  // Check if the locator is valid
  if (locator.isValidLocator((locatorLineEdit->text()).toUpper()))
    qso.setLocator((locatorLineEdit->text()).toUpper());


  if(qslVialineEdit->isEnabled())
    qso.setQslManager((qslVialineEdit->text()).toUpper());

  if ((QSLInfotextEdit->toPlainText()).length() > 0)
  //if(QSLInfotextEdit->isEnabled())
    qso.setQslInfo(QSLInfotextEdit->toPlainText());

  if((namekLineEdit->text()).length() >= 2)
    qso.setName((namekLineEdit->text()).toUpper());

  if((qthkLineEdit->text()).length() >= 2)
    qso.setQth((qthkLineEdit->text()).toUpper());


    }
  }
}


// Hamlib support
void Klog::slothamlibUpdateFrequency(){
/****************************************************
*  We read the frequency and mode from the radio
*
*****************************************************/
  //qDebug() << "KLog::slothamlibUpdateFrequency: " << KlogHamlib.getStatusMessage();
//cout << "KLog::slothamlibUpdateFrequency: " << KlogHamlib.getStatusMessage() << endl;

    hamlibFreq = 0.0;
        hamlibFreq = KlogHamlib.getFrequency();
        if (hamlibFreq > 0.0){
                band = adif.band2Int(adif.freq2Band(QString::number(hamlibFreq)));
                bandComboBox->setCurrentIndex(band);
                freqlCDNumber->display(hamlibFreq);
        }else{
    //cout << "KLog::slothamlibUpdateFrequency - NO Freq: " << QString::number(hamlibFreq) << endl;
    }

}


/***************************************************************************
** This is an auxiliary class intended to provide color to the DX-Cluster **
** spots.                                                                 **
** It may be moved to a self .h & .cpp archives                           **
****************************************************************************/
dxClusterSpotItem::dxClusterSpotItem( QListWidget *parent, const QString& spot, const QColor& color ) : QListWidgetItem( parent ){
//cout << "KLog::dxClusterSpotItem - Constructor" << endl;
  spotColor = color;
  setText(spot);
  // Experimenting with fonts for the cluster
  QFont f("Helvetica");
  f.setFixedPitch(TRUE);
  setFont(f);
}

dxClusterSpotItem::~dxClusterSpotItem(){
//cout << "KLog::dxClusterSpotItem - Destructor" << endl;
}

void dxClusterSpotItem::paintCell( QPainter *p, const QPalette &cg,
                                 int column, int width, int alignment ){
//cout << "KLog::dxClusterSpotItem - paintCell" << endl;
  QPalette _cg( cg );
  //QColor c = _cg.text();
  _cg.setColor( QPalette::Text, spotColor );
//TODO: How to replace paintcell in a QTreeWidgetItem to paint it?
//  QTreeWidgetItem::paintCell( p, _cg, column, width, alignment );
  _cg.setColor( QPalette::Text, spotColor );

}

/***************************************************************************
** This is an auxiliary class intended to provide color to the SEARCH BOX **
** It may be moved to a self .h & .cpp archives                           **
****************************************************************************/
searchBoxItem::searchBoxItem( QTreeWidget *parent, const QString& call, const QString& date, const QString& time,const QString& band, const QString& mode, const QString& RSTsent, const QString& RSTrec, const QString& numb, const QColor& color ) : QTreeWidgetItem( parent ){
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

void searchBoxItem::paintCell( QPainter *p, const QPalette &cg,
                                 int column, int width, int alignment ){
//cout << "KLog::searchBoxItem - paintCell" << endl;
  QPalette _cg( cg );
  //QColor c = _cg.text();
  _cg.setColor( QPalette::Text, qsoColor );
//TODO: How to replace paintcell in a QTreeWidgetItem to paint it?
//  QTreeWidgetItem::paintCell( p, _cg, column, width, alignment );
  _cg.setColor( QPalette::Text, qsoColor );

}


/***************************************************************************
** This is an auxiliary class intended to provide color to the DX-Cluster **
** spots.                                                                 **
** It may be moved to a self .h & .cpp archives
** BANDMAP						                     **
****************************************************************************/
// bandMapSpotItem::bandMapSpotItem( K3ListView *parent, const QString& freq, const QString& dx, const QString& from, const QColor& dxcolor ) : K3ListViewItem( parent ){
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
// void bandMapSpotItem::paintCell( QPainter *p, const Qpalette &cg,
//                                  int column, int width, int alignment )
// {
// //cout << "KLog::bandMapSpotItem - paintCell" << endl;
//   Qpalette _cg( cg );
//   QColor c = _cg.text();
//   _cg.setColor( Qpalette::Text, spotColor );
//
//   QListViewItem::paintCell( p, _cg, column, width, alignment );
//   _cg.setColor( Qpalette::Text, spotColor );
//
// }
//
