/***************************************************************************
                          setuppagelogsnew.h  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/
//
// This class implements the Dialog to add a new log
//

#include "setuppages/setuppagelogsnew.h"


SetupPageLogsNew::SetupPageLogsNew(DataProxy_SQLite *dp, QWidget *parent)
{
     //qDebug() << "SetupPageLogsNew::SetupPageLogsNew"   << endl;
    dataProxy = dp;
    editing = false;
    checking = false;
    bCass = false;
    bCOp = false;
    bCMo = false;
    bCPo = false;
    bCBa = false;
    bCOv = false;
    bCTy = false;

    logData.clear();
    stationCallsignFilled = false;
    operatorsFilled = true;
    stationCallsign = QString();
    operators = QString();
    comment = QString();
    dateString = QString();
    typeContest = 0;
    contestCatMode = 0;
    contestCatOperators = 0;
    contestCatAssisted = 0;
    contestCatPower = 0;
    contestCatBands = 0;
    contestBands = 0;
    contestCatOverlay = 0;
    typeContestSelected = 0;
    typeConteststr = QString();


    stationCallsignLineEdit = new QLineEdit;
    operatorsLineEdit = new QLineEdit;

    dateEdit = new QDateEdit;
    dateEdit->setDate(QDate::currentDate ());
    commentLineEdit = new QLineEdit;

    typeComboBox = new QComboBox;
    contestCatModeComboBox = new QComboBox;
    contestCatOperatorsComboBox = new QComboBox;
    contestCatAssistedComboBox = new QComboBox;
    contestCatPowerComboBox = new QComboBox;
    contestCatBandsComboBox = new QComboBox;
    contestBandsComboBox = new QComboBox;
    contestCatOverlayComboBox = new QComboBox;
    contestCatModeComboBox = new QComboBox;

    catAsLabel = new QLabel();
    typeLabel = new QLabel();
    catOpLabel = new QLabel();
    catModeLabel = new QLabel();
    catPowerLabel = new QLabel();
    catBandsLabel = new QLabel();
    overlayLabel = new QLabel();
    validCats = new QLabel();

    dateLabel = new QLabel(tr("&Date"));
    stationCallsignLabel = new QLabel(tr("&Station Callsign"));
    operatorsLabel = new QLabel(tr("&Operators"));
    commentLabel = new QLabel(tr("Comm&ent"));

    okButton = new QPushButton(tr("&Ok"), this);
    cancelButton = new QPushButton(tr("&Cancel"), this);
    createWidget();


    okButton->setEnabled(false);

     //qDebug() << "SetupPageLogsNew::SetupPageLogsNew - END"   << endl;
}

void SetupPageLogsNew::clear()
{
    stationCallsignLineEdit->clear();
    operatorsLineEdit->clear();
    dateEdit->setDate(QDate::currentDate ());


    typeComboBox->setCurrentIndex(0);
    contestCatModeComboBox->setCurrentIndex(0);
    contestCatOperatorsComboBox->setCurrentIndex(0);
    contestCatAssistedComboBox->setCurrentIndex(0);
    contestCatPowerComboBox->setCurrentIndex(0);
    contestCatBandsComboBox->setCurrentIndex(0);
    contestBandsComboBox->setCurrentIndex(0);
    contestCatOverlayComboBox->setCurrentIndex(0);
    contestCatModeComboBox->setCurrentIndex(0);
    typeContest = 0;
    contestCatMode = 0;
    contestCatOperators = 0;
    contestCatAssisted = 0;
    contestCatPower = 0;
    contestCatBands = 0;
    contestBands = 0;
    contestCatOverlay = 0;
    typeContestSelected = 0;

}

void SetupPageLogsNew::createWidget()
{
     //qDebug() << "SetupPageLogsNew::createWidget" << endl;

    stationCallsignLabel->setWordWrap(true);
    operatorsLabel->setWordWrap(true);
    commentLabel->setWordWrap(true);
    dateLabel->setBuddy(dateEdit);

    stationCallsignLabel->setBuddy(stationCallsignLineEdit);
    operatorsLabel->setBuddy(operatorsLineEdit);
    commentLabel->setBuddy(commentLineEdit);
    catAsLabel->setBuddy(contestCatAssistedComboBox);
    typeLabel->setBuddy(typeComboBox);
    catOpLabel->setBuddy(contestCatOperatorsComboBox);
    catModeLabel->setBuddy(contestCatModeComboBox);
    catPowerLabel->setBuddy(contestCatPowerComboBox);
    catBandsLabel->setBuddy(contestCatBandsComboBox);
    overlayLabel->setBuddy(contestCatOverlayComboBox);

    validCats->setText(tr("Select categories"));
    validCats->setWordWrap(true);


    stationCallsignLineEdit->setToolTip(tr("Callsign used for this log."));
    operatorsLineEdit->setToolTip(tr("Comma separated list of operators: callsign1, callsign2."));

    dateEdit->setToolTip(tr("Start date of this log."));
    commentLineEdit->setToolTip(tr("Add a comment about this log."));

    typeLabel->setText(tr("&Type of Operation"));
    typeLabel->setWordWrap(true);

    //nameLabel->setWordWrap(true);
    dateLabel->setWordWrap(true);


    typeComboBox->setToolTip(tr("Select the kind of operation for this log."));
    QStringList _qs;
    _qs.clear();
    _qs.append(dataProxy->getContestNames());    
    typeComboBox->addItems(_qs);
     //qDebug() << "SetupPageLogsNew::createWidget - contestNames: " << _qs.at(0) << endl;

    catModeLabel->setText(tr("&Mode Category"));
    catModeLabel->setWordWrap(true);
    contestCatModeComboBox->setToolTip(tr("Select the mode category."));
    _qs.clear();
    _qs.append(dataProxy->getContestCat(6));
    contestCatModeComboBox->addItems(_qs);

    //QLabel *catOpLabel = new QLabel(tr("Operators Category"));
    catOpLabel->setText(tr("O&perators Category"));
    catOpLabel->setWordWrap(true);
    contestCatOperatorsComboBox->setToolTip(tr("Select the operators category."));
    _qs.clear();
    _qs.append(dataProxy->getContestCat(1));
    contestCatOperatorsComboBox->addItems(_qs);

    catAsLabel->setText(tr("&Assisted Category"));
    catOpLabel->setWordWrap(true);
    contestCatAssistedComboBox->setToolTip(tr("Select the assisted category."));
    _qs.clear();
    _qs.append(dataProxy->getContestCat(2));
    contestCatAssistedComboBox->addItems(_qs);

    //QLabel *catPowerLabel = new QLabel(tr("Power Category"));
    catPowerLabel->setText(tr("Po&wer Category"));
    catPowerLabel->setWordWrap(true);
    contestCatPowerComboBox->setToolTip(tr("Select the power category."));
    _qs.clear();
    _qs.append(dataProxy->getContestCat(3));
    contestCatPowerComboBox->addItems(_qs);

    //QLabel *catBandsLabel = new QLabel(tr("Bands Category"));
    catBandsLabel->setText(tr("&Bands Category"));
    catBandsLabel->setWordWrap(true);
    contestCatBandsComboBox->setToolTip(tr("Select the bands category."));
    _qs.clear();
    _qs.append(dataProxy->getContestCat(4));
    contestCatBandsComboBox->addItems(_qs);

    overlayLabel->setText(tr("O&verlay"));
    overlayLabel->setWordWrap(true);
    contestCatOverlayComboBox->setToolTip(tr("Select the Overlay category."));
    _qs.clear();
    _qs.append(dataProxy->getContestOverlays());
    contestCatOverlayComboBox->addItems(_qs);

    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotStationCallSignTextChanged() ) );
    connect(operatorsLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOperatorsTextChanged() ) );


    connect(typeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotTypeComboBoxChanged() ) ) ;
    connect(contestCatModeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatModeComboBoxChanged() ) ) ;
    connect(contestCatAssistedComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatAssistedComboBoxChanged() ) ) ;
    connect(contestCatOperatorsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatOperatorsComboBoxChanged() ) ) ;
    connect(contestCatPowerComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatPowerComboBoxChanged() ) ) ;
    connect(contestCatBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatBandsComboBoxChanged() ) ) ;
    connect(contestCatOverlayComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatOverlayComboBoxChanged() ) ) ;

    //connect(typeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotTypeComboBoxChanged() ) ) ;
    //connect(contestCatModeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatModeComboBoxChanged() ) ) ;
    //connect(contestCatAssistedComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatAssistedComboBoxChanged() ) ) ;
    //connect(contestCatOperatorsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatOperatorsComboBoxChanged() ) ) ;
    //connect(contestCatPowerComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatPowerComboBoxChanged() ) ) ;
    //connect(contestCatBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatBandsComboBoxChanged() ) ) ;
    //connect(contestCatOverlayComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatOverlayComboBoxChanged() ) ) ;

    connect(contestBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandsComboBoxChanged() ) ) ;
    //connect(contestCatModeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatModeComboBoxChanged() ) ) ;
    connect(okButton,SIGNAL(clicked()), this, SLOT(slotOKButtonClicked() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked() ) );

    QGridLayout *callsLayout = new QGridLayout; // Widget, row, column

    callsLayout->addWidget(stationCallsignLabel, 0, 0);
    callsLayout->addWidget(stationCallsignLineEdit, 0, 1);
    callsLayout->addWidget(dateLabel, 1, 0);
    callsLayout->addWidget(dateEdit, 1, 1);
    callsLayout->addWidget(operatorsLabel, 2, 0);
    callsLayout->addWidget(operatorsLineEdit, 2, 1);
    callsLayout->addWidget(commentLabel, 3, 0);
    callsLayout->addWidget(commentLineEdit, 3, 1);
/*
    callsLayout->addWidget(typeLabel, 3, 0);
    callsLayout->addWidget(typeComboBox, 3, 1);

    callsLayout->addWidget(catModeLabel, 4, 0);
    callsLayout->addWidget(contestCatModeComboBox, 4, 1);
    callsLayout->addWidget(catOpLabel, 5, 0);
    callsLayout->addWidget(contestCatOperatorsComboBox, 5, 1);
    callsLayout->addWidget(catAsLabel, 6, 0);
    callsLayout->addWidget(contestCatAssistedComboBox, 6, 1);

    callsLayout->addWidget(catPowerLabel, 7, 0);
    callsLayout->addWidget(contestCatPowerComboBox, 7, 1);

    callsLayout->addWidget(catBandsLabel, 8, 0);
    callsLayout->addWidget(contestCatBandsComboBox, 8, 1);
    callsLayout->addWidget(contestBandsComboBox, 8, 2);


    callsLayout->addWidget(overlayLabel, 10, 0);
    callsLayout->addWidget(contestCatOverlayComboBox, 10, 1);
*/

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    //buttonsLayout->addWidget(validCats);
    buttonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(callsLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    clear();
    //page->setLayout(callsLayout);

}


void SetupPageLogsNew::slotOperatorsTextChanged()
{
     //qDebug() << "SetupPageLogsNew::slotOperatorsTextChanged" << endl;
//    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT( ) );
    if ((operatorsLineEdit->text()).length()<1)
    {
        return;
    }

    int cursorP = operatorsLineEdit->cursorPosition();

    QString currentQrz = operatorsLineEdit->text();
    if ((currentQrz.at(cursorP-1)).isSpace())
    {
        currentQrz = currentQrz.remove(cursorP-1, 1);
        cursorP--;
        operatorsLineEdit->setText(currentQrz);
    }

    operatorsLineEdit->setText(((operatorsLineEdit->text())).simplified());
    operatorsLineEdit->setText((operatorsLineEdit->text()).toUpper());

    operatorsLineEdit->setCursorPosition(cursorP);

    if (currentQrz.length()>=3)
    {//TODO: Add a check of the format (comma separated)
        operatorsFilled= true;
    }
}

void SetupPageLogsNew::slotStationCallSignTextChanged()
{
     //qDebug() << "SetupPageLogsNew::slotStationCallSignTextChanged" << endl;
//    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT( ) );
    if ((stationCallsignLineEdit->text()).length()<1)
    {
        showNOK();
        return;
    }

    int cursorP = stationCallsignLineEdit->cursorPosition();

    QString currentQrz = stationCallsignLineEdit->text();
    if ((currentQrz.at(cursorP-1)).isSpace())
    {
        currentQrz = currentQrz.remove(cursorP-1, 1);
        cursorP--;
        stationCallsignLineEdit->setText(currentQrz);
    }

    stationCallsignLineEdit->setText(((stationCallsignLineEdit->text())).simplified());
    stationCallsignLineEdit->setText((stationCallsignLineEdit->text()).toUpper());

    stationCallsignLineEdit->setCursorPosition(cursorP);

    if (currentQrz.length()>=3)
    {
        stationCallsignFilled = true;
    }
    showOK();
}

void SetupPageLogsNew::slotTypeComboBoxChanged()
{
     //qDebug() << "SetupPageLogsNew::slotTypeComboBoxChanged" << endl;
//    connect(typeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotTypeComboBoxChanged() ) ) ;


    contestCatMode = contestCatModeComboBox->currentIndex();
    contestCatBands = contestCatBandsComboBox->currentIndex();
    contestCatPower = contestCatPowerComboBox->currentIndex();
    contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    typeContestSelected = typeComboBox->currentIndex();
    contestCatOverlay= contestCatOverlayComboBox->currentIndex();

    typeContest = getSelectedTypeContest();

    if (typeContest < 0)
    {
        typeLabel->setStyleSheet("QLabel {color : red; }");
        validCats->setText(tr("Categories not OK"));
        validCats->setStyleSheet("QLabel {color : red; }");
        okButton->setEnabled(false);
    }
    else
    {
         typeLabel->setStyleSheet("QLabel {color : black; }");
            showOK();
    }
}



void SetupPageLogsNew::slotCatAssistedComboBoxChanged()
{
    //qDebug() << "SetupPageLogs:slotCatAssistedComboBoxChanged: " << QString::number(contestCatAssistedComboBox->currentIndex())  << endl;
//connect(contestCatAssistedComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatAssistedComboBoxChanged() ) ) ;

   contestCatMode = contestCatModeComboBox->currentIndex();
   contestCatBands = contestCatBandsComboBox->currentIndex();
   contestCatPower = contestCatPowerComboBox->currentIndex();
   contestCatOperators = contestCatOperatorsComboBox->currentIndex();
   contestCatAssisted = contestCatAssistedComboBox->currentIndex();
   typeContestSelected = typeComboBox->currentIndex();
   contestCatOverlay= contestCatOverlayComboBox->currentIndex();

   typeContest = getSelectedTypeContest();

   if (typeContest < 0)
   {
       catAsLabel->setStyleSheet("QLabel {color : red; }");
       showNOK();
   }
   else
   {
        catAsLabel->setStyleSheet("QLabel {color : black; }");
        showOK();
   }

}

void SetupPageLogsNew::slotCatOperatorsComboBoxChanged()
{
    //qDebug() << "SetupPageLogsNew::slotCatOperatorsComboBoxChanged(): " << QString::number(contestCatOperatorsComboBox->currentIndex())  << endl;

   contestCatMode = contestCatModeComboBox->currentIndex();
   contestCatBands = contestCatBandsComboBox->currentIndex();
   contestCatPower = contestCatPowerComboBox->currentIndex();
   contestCatOperators = contestCatOperatorsComboBox->currentIndex();
   contestCatAssisted = contestCatAssistedComboBox->currentIndex();
   typeContestSelected = typeComboBox->currentIndex();
   contestCatOverlay= contestCatOverlayComboBox->currentIndex();

   typeContest = getSelectedTypeContest();

   if (typeContest < 0)
   {
       catOpLabel->setStyleSheet("QLabel {color : red; }");
       showNOK();
   }
   else
   {
        catOpLabel->setStyleSheet("QLabel {color : black; }");
        showOK();
   }

}

void SetupPageLogsNew::slotCatPowerComboBoxChanged()
{
     //qDebug() << "SetupPageLogsNew::slotCatPowerComboBoxChanged(): " << QString::number(contestCatPowerComboBox->currentIndex())  << endl;
//connect(contestCatPowerComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatPowerComboBoxChanged() ) ) ;

    contestCatMode = contestCatModeComboBox->currentIndex();
    contestCatBands = contestCatBandsComboBox->currentIndex();
    contestCatPower = contestCatPowerComboBox->currentIndex();
    contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    typeContestSelected = typeComboBox->currentIndex();
    contestCatOverlay= contestCatOverlayComboBox->currentIndex();

    typeContest = getSelectedTypeContest();

    if (typeContest < 0)
    {
        catPowerLabel->setStyleSheet("QLabel {color : red; }");
        showNOK();
    }
    else
    {
         catPowerLabel->setStyleSheet("QLabel {color : black; }");
         showOK();
    }

}

void SetupPageLogsNew::slotCatBandsComboBoxChanged()
{
     //qDebug() << "SetupPageLogsNew::slotCatBandsComboBoxChanged(): " << QString::number(contestCatBandsComboBox->currentIndex())  << endl;
//connect(contestCatBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatBandsComboBoxChanged() ) ) ;

    contestCatMode = contestCatModeComboBox->currentIndex();
    contestCatBands = contestCatBandsComboBox->currentIndex();
    contestCatPower = contestCatPowerComboBox->currentIndex();
    contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    typeContestSelected = typeComboBox->currentIndex();
    contestCatOverlay= contestCatOverlayComboBox->currentIndex();

    typeContest = getSelectedTypeContest();

    if (typeContest < 0)
    {
        catBandsLabel->setStyleSheet("QLabel {color : red; }");
        showNOK();
    }
    else
    {
         catBandsLabel->setStyleSheet("QLabel {color : black; }");
         showOK();
    }

}

void SetupPageLogsNew::slotBandsComboBoxChanged()
{
     //qDebug() << "SetupPageLogsNew::slotBandsComboBoxChanged(): " << QString::number(contestBandsComboBox->currentIndex()) << endl;
            //connect(contestBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandsComboBoxChanged() ) ) ;

}


void SetupPageLogsNew::slotCatOverlayComboBoxChanged()
{
      //qDebug() << "SetupPageLogsNew::slotCatOverlayComboBoxChanged(): " << QString::number(contestCatOverlayComboBox->currentIndex())  << endl;

     contestCatMode = contestCatModeComboBox->currentIndex();
     contestCatBands = contestCatBandsComboBox->currentIndex();
     contestCatPower = contestCatPowerComboBox->currentIndex();
     contestCatOperators = contestCatOperatorsComboBox->currentIndex();
     contestCatAssisted = contestCatAssistedComboBox->currentIndex();
     typeContestSelected = typeComboBox->currentIndex();
     contestCatOverlay= contestCatOverlayComboBox->currentIndex();

     typeContest = getSelectedTypeContest();
      //qDebug() << "SetupPageLogsNew::slotCatOverlayComboBoxChanged(): typeContest =  " << QString::number(typeContest)  << endl;

     if (typeContest < 0)
     {
         overlayLabel->setStyleSheet("QLabel {color : red; }");
         showNOK();
     }
     else
     {
          overlayLabel->setStyleSheet("QLabel {color : black; }");
          showOK();
     }

}

void SetupPageLogsNew::slotCatModeComboBoxChanged()
{
     //qDebug() << "SetupPageLogsNew::slotCatModeComboBoxChanged(): " << QString::number(contestCatModeComboBox->currentIndex())  << endl;

    contestCatMode = contestCatModeComboBox->currentIndex();
    contestCatBands = contestCatBandsComboBox->currentIndex();
    contestCatPower = contestCatPowerComboBox->currentIndex();
    contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    typeContestSelected = typeComboBox->currentIndex();
    contestCatOverlay= contestCatOverlayComboBox->currentIndex();

    typeContest = getSelectedTypeContest();
     //qDebug() << "SetupPageLogsNew::slotCatModeComboBoxChanged(): " << QString::number(typeContest)  << endl;

    if (typeContest < 0)
    {
        catModeLabel->setStyleSheet("QLabel {color : red; }");
        showNOK();
    }
    else
    {
         catModeLabel->setStyleSheet("QLabel {color : black; }");
         showOK();
    }

}

void SetupPageLogsNew::slotOKButtonClicked()
{

     //qDebug() << "SetupPageLogsNew::slotOkButtonClicked" << endl;
    stationCallsign = stationCallsignLineEdit->text();
    if (stationCallsign.length()<3)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You need to enter a valid QRZ in the Station Callsign box.\nThe log will not be opened."));
        msgBox.exec();
        return;
    }

    operators = operatorsLineEdit->text();
    //TODO: Check if operators is really including a comma separated list of QRZ
    comment = commentLineEdit->text();    
    dateString = dateEdit->date().toString("yyyy/MM/dd");

    //typeContest, contestCatOperators, contestCatAssisted, contestCatPower,
    //contestCatBands, contestCatOverlay, contestCatMode

    if (typeComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: type" << endl;
        typeContestSelected = typeComboBox->currentIndex();
        typeConteststr = typeComboBox->currentText();
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: type" << endl;
        typeContestSelected = 0;
        typeConteststr = QString();
    }

    if (contestCatModeComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: mode" << endl;
        contestCatMode = contestCatModeComboBox->currentIndex();
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: mode" << endl;
        contestCatMode = 0;
    }

    if (contestCatOperatorsComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: oper" << endl;
       contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: oper" << endl;
        contestCatOperators = 0;
    }

    if (contestCatAssistedComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: ass" << endl;
        contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: ass" << endl;
        contestCatAssisted = 0;
    }

    if (contestCatPowerComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: pwr" << endl;
        contestCatPower = contestCatPowerComboBox->currentIndex();
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: pwr - " << contestCatPowerComboBox->currentText() << endl;
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: pwr" << endl;
         contestCatPower = 0;
    }

    if (contestCatBandsComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: Cbands" << endl;
        contestCatBands = contestCatBandsComboBox->currentIndex();
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: Cbands" << endl;
        contestCatBands = 0;
    }

    if (contestBandsComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: bands" << endl;
        contestBands = contestBandsComboBox->currentIndex();
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: bands" << endl;
        contestBands = 0;
    }

    if (contestCatOverlayComboBox->isEnabled())
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked ENA: over" << endl;
        contestCatOverlay = contestCatOverlayComboBox->currentIndex();
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::slotOkButtonClicked NOT ENA: over" << endl;
        contestCatOverlay = 0;
    }

    //typeContest, contestCatOperators, contestCatAssisted, contestCatPower,
    //contestCatBands, contestCatOverlay, contestCatMode
    typeContest = getSelectedTypeContest();
    if (typeContest < 0)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You selected an invalid combination.\nThe log will not be opened."));
        msgBox.exec();

    }
    else
    {
        gatherAndSend();
        close();

    }


}
void SetupPageLogsNew::gatherAndSend()
{
      //qDebug() << "SetupPageLogsNew::gatherAndSend: " << typeConteststr << endl;

    // The following lines will be removed once more contest types have been added
    contestCatMode = contestCatModeComboBox->currentIndex();
    contestCatBands = contestCatBandsComboBox->currentIndex();
    contestCatPower = contestCatPowerComboBox->currentIndex();
    contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    typeContestSelected = typeComboBox->currentIndex();
    contestCatOverlay = contestCatOverlayComboBox->currentIndex();
    typeContest = getSelectedTypeContest();

    // The previous lines will be removed once more contest types have been added

    logData.clear();

    logData << stationCallsign << operators << comment << dateString
            << typeConteststr
            << QString::number(contestCatMode)
            << QString::number(contestCatOperators)
            << QString::number(contestCatAssisted)
            << QString::number(contestCatPower)
            << QString::number(contestCatBands)
            << QString::number(contestBands)
            << QString::number(contestCatOverlay)
            << QString::number(typeContest);

    if (editing)
    {
        logData << "1";
        editing = false;
    }
    else
    {
        logData << "0";
    }

    //logData << QString::number(typeContest)
    // Update the SetupPageLogs::slotAnalyzeNewLogData if you add or remove any field (Today 12)

     //qDebug() << "SetupPageLogsNew::gatherAndSend: EMITED" << endl;
    emit newLogData(logData);

}

void SetupPageLogsNew::slotCancelButtonClicked()
{
     //qDebug() << "SetupPageLogsNew::slotCancelButtonClicked" << endl;
    logData.clear();
    emit cancelled(true);
    close();
}


/*
QStringList SetupPageLogsNew::getValidCatOptions(const int _currentCat, const int _higherCat)
{// currentCat is the current category and highercat is the category we want to know the information
// being:
// 1 - contestcatoperator
// 2 - contestcatassisted
// 3 - contestcatpower
// 4 - contestcatband
// 5 - contestcatoverlay
// 6 - contestcatmode

     //qDebug() << "SetupPageLogsNew::getValidCatOptions: " << QString::number(_currentCat) <<"/"<< QString::number(_higherCat) << endl;
    //return dataProxy->getValidCatOptions(_currentCat, _higherCat);
    return QStringList();
}
*/

void SetupPageLogsNew::setStationCallSign(const QString _st)
{
    stationCallsign = _st;
    stationCallsignLineEdit->setText(stationCallsign.toUpper());
}

void SetupPageLogsNew::setOperators(const QString _st)
{
    operators = _st;
    operatorsLineEdit->setText(operators.toUpper());
}

void SetupPageLogsNew::setComment(const QString _st)
{
    comment = _st;
    commentLineEdit->setText(comment.toUpper());
}

void SetupPageLogsNew::setDateString(const QString _st)
{
    dateString = _st;
    dateEdit->setDate(QDate::fromString(dateString, "yyyy/MM/dd"));

}
/*
void SetupPageLogsNew::setType(const QString _st)
{
    typeConteststr = _st;
    //typeConteststr = typeComboBox->currentText();
    typeComboBox->setCurrentIndex(typeComboBox->findText(typeConteststr, Qt::MatchExactly));

//findText ( const QString & text, Qt::MatchFlags flags = static_cast<Qt::MatchFlags> ( Qt::MatchExactly | Qt::MatchCaseSensitive ) ) const
}

void SetupPageLogsNew::setCMode(const int _n)
{
    contestCatMode = _n;
    contestCatModeComboBox->setCurrentIndex(contestCatMode);
}

void SetupPageLogsNew::setCOperators(const int _n)
{
    contestCatOperators = _n;
    contestCatOperatorsComboBox->setCurrentIndex(contestCatOperators);
}

void SetupPageLogsNew::setCAssisted(const int _n)
{
    contestCatAssisted = _n;
    contestCatAssistedComboBox->setCurrentIndex(contestCatAssisted);
}

void SetupPageLogsNew::setCPower(const int _n)
{
    contestCatPower = _n;
    contestCatPowerComboBox->setCurrentIndex(contestCatPower);
}

void SetupPageLogsNew::setCBands(const int _n)
{
    contestCatBands = _n;
    contestCatBandsComboBox->setCurrentIndex(contestCatBands);
}

void SetupPageLogsNew::setBands(const int _n)
{
    contestBands = _n;
    contestBandsComboBox->setCurrentIndex(contestBands);
}

void SetupPageLogsNew::setCOverlay(const int _n)
{
    contestCatOverlay = _n;
    contestCatOverlayComboBox->setCurrentIndex(contestCatOverlay);

}
*/

void SetupPageLogsNew::setEditing(const bool b)
{
    editing = b;
    if (!editing)
    {
        clear();
    }

}

int SetupPageLogsNew::getSelectedTypeContest()
{
     //qDebug() << "SetupPageLogsNew::getSelectedTypeContest: " << endl;
    //is/contest/catoperator/catassisted/catpower/catband/catoverlay/catmode
    int i = dataProxy->getContestTypeN(typeContestSelected, contestCatOperators, contestCatAssisted, contestCatPower, contestCatBands, contestCatOverlay, contestCatMode);
     //qDebug() << "SetupPageLogsNew::getSelectedTypeContest: " << QString::number(i) << endl;
    return i;

}

void SetupPageLogsNew::setTypeN(const int _n)
{
     //qDebug() << "SetupPageLogsNew::setTypeN: " << QString::number(_n) << endl;
    typeContestSelected = _n;
    fillWithType(typeContestSelected);
}


void SetupPageLogsNew::fillWithType(const int _n)
{
     //qDebug() << "SetupPageLogsNew::fillWithType - n = " << QString::number(_n) << endl;
    typeContestSelected = _n;
    QStringList contestData;
    contestData << dataProxy->getDataFromContestType(_n);
     //qDebug() << "SetupPageLogsNew::fillWithType-1 (length = " << QString::number(contestData.length()) << ")" << endl;
    if (contestData.length()== 8)
    {

         //qDebug() << "SetupPageLogsNew::fillWithType-2" << endl;
        //setCOperators ((contestData.at(1)).toInt());
        //setCAssisted ((contestData.at(2)).toInt());
        //setCPower ((contestData.at(3)).toInt());
        //setCOverlay ((contestData.at(4)).toInt());
        //setCMode ((contestData.at(5)).toInt());
        //setType(contestData.at(6));
        //setCBands((contestData.at(7)).toInt());
         //qDebug() << "SetupPageLogsNew::fillWithType: " << contestData.at(6) << endl;
    }
    else
    {
         //qDebug() << "SetupPageLogsNew::fillWithType-3" << endl;
        return;
    }

}

void SetupPageLogsNew::updateAllCats()
{
     //qDebug() << "SetupPageLogsNew::updateAllCats" << endl;

    contestCatMode = contestCatModeComboBox->currentIndex();
    contestCatBands = contestCatBandsComboBox->currentIndex();
    contestCatPower = contestCatPowerComboBox->currentIndex();
    contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    typeContestSelected = typeComboBox->currentIndex();
    contestCatOverlay= contestCatOverlayComboBox->currentIndex();

    typeContest = getSelectedTypeContest();


}

void SetupPageLogsNew::showOK()
{
    validCats->setText(tr("Categories OK"));
    validCats->setStyleSheet("QLabel {color : black; }");
    okButton->setEnabled(true);

}

void SetupPageLogsNew::showNOK()
{
    validCats->setText(tr("Categories not OK"));
    validCats->setStyleSheet("QLabel {color : red; }");
    okButton->setEnabled(false);
}
