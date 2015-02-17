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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/
//
// This class implements the Dialog to add a new log
//

#include "setuppagelogsnew.h"


SetupPageLogsNew::SetupPageLogsNew(QWidget *parent)

{
    dataProxy = new DataProxy_SQLite();

    logData.clear();
    stationCallsignFilled = false;
    operatorsFilled = true;
    stationCallsign = QString();
    operators = QString();
    comment = QString();
    dateString = QString();
    typeContest = -1;
    contestCatMode = -1;
    contestCatOperators = -1;
    contestCatAssisted = -1;
    contestCatPower = -1;
    contestCatBands = -1;
    contestBands = -1;

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

    okButton = new QPushButton(tr("&Ok"), this);
    cancelButton = new QPushButton(tr("&Cancel"), this);
    createWidget();

}

void SetupPageLogsNew::createWidget()
{
    QLabel *stationCallsignLabel = new QLabel(tr("StationCallsign"));
    stationCallsignLabel->setWordWrap(true);
    QLabel *operatorsLabel = new QLabel(tr("Operators"));
    operatorsLabel->setWordWrap(true);
    QLabel *commentLabel = new QLabel(tr("Comment"));
    commentLabel->setWordWrap(true);

    stationCallsignLineEdit->setToolTip(tr("Callsign used for this log"));
    operatorsLineEdit->setToolTip(tr("Comma separated list of operators: callsign1, callsign2"));

    dateEdit->setToolTip(tr("Start date of this log"));
    commentLineEdit->setToolTip(tr("Add a comment about this log"));


    QLabel *typeLabel = new QLabel(tr("Type of Operation"));
    typeLabel->setWordWrap(true);
    QLabel *nameLabel = new QLabel(tr("Name"));
    nameLabel->setWordWrap(true);
    QLabel *dateLabel = new QLabel(tr("Date"));
    dateLabel->setWordWrap(true);

    typeComboBox->setToolTip(tr("Select the kind of operation for this log"));
    QStringList _qs;
    _qs.clear();
    _qs.append(dataProxy->getContestNames());
    //_qs << "DX" << "CQ-WW-SSB"  << "CQ-WW-CW"  << "CQ-WPX-SSB"  << "CQ-WPX-CW" ;
    typeComboBox->addItems(_qs);

    QLabel *catModeLabel = new QLabel(tr("Mode Category"));
    catModeLabel->setWordWrap(true);
    contestCatModeComboBox->setToolTip(tr("Select the mode category"));
    _qs.clear();
    _qs << "SSB" << "CW" << "MIXED";
    contestCatModeComboBox->addItems(_qs);

    QLabel *catOpLabel = new QLabel(tr("Operators Category"));
    catOpLabel->setWordWrap(true);
    contestCatOperatorsComboBox->setToolTip(tr("Select the operators category"));
    _qs.clear();
    _qs << "SINGLE" << "MULTI";
    contestCatOperatorsComboBox->addItems(_qs);


    QLabel *catAsLabel = new QLabel(tr("Assisted Category"));
    catOpLabel->setWordWrap(true);
    contestCatAssistedComboBox->setToolTip(tr("Select the assisted category"));
    _qs.clear();
    _qs << "NON-ASSISTED" << "ASSISTED";
    contestCatAssistedComboBox->addItems(_qs);

    QLabel *catPowerLabel = new QLabel(tr("Power Category"));
    catPowerLabel->setWordWrap(true);
    contestCatPowerComboBox->setToolTip(tr("Select the power category"));
    _qs.clear();
    _qs << "QRP" << "LOW" << "HIGH";
    contestCatPowerComboBox->addItems(_qs);

    QLabel *catBandsLabel = new QLabel(tr("Bands Category"));
    catBandsLabel->setWordWrap(true);
    contestCatBandsComboBox->setToolTip(tr("Select the bands category"));
    _qs.clear();
    _qs << "ALL" << "10" << "15" << "20" << "40" << "80" << "160";
    contestCatBandsComboBox->addItems(_qs);

    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotStationCallSignTextChanged() ) );
    connect(typeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotTypeComboBoxChanged(int) ) ) ;
    connect(contestCatModeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatModeComboBoxChanged() ) ) ;
    connect(contestCatAssistedComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatAssistedComboBoxChanged() ) ) ;
    connect(contestCatOperatorsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatOperatorsComboBoxChanged() ) ) ;
    connect(contestCatPowerComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatPowerComboBoxChanged() ) ) ;
    connect(contestCatBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatBandsComboBoxChanged() ) ) ;
    connect(contestBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandsComboBoxChanged() ) ) ;
    connect(okButton,SIGNAL(clicked()), this, SLOT(slotOKButtonClicked() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked() ) );

    QGridLayout *callsLayout = new QGridLayout; // Widget, row, column
    callsLayout->addWidget(commentLabel, 0, 0);
    callsLayout->addWidget(commentLineEdit, 0, 1);
    callsLayout->addWidget(stationCallsignLabel, 1, 0);
    callsLayout->addWidget(stationCallsignLineEdit, 1, 1);
    callsLayout->addWidget(dateLabel, 2, 0);
    callsLayout->addWidget(dateEdit, 2, 1);

    callsLayout->addWidget(operatorsLabel, 3, 0);
    callsLayout->addWidget(operatorsLineEdit, 3, 1);
    callsLayout->addWidget(typeLabel, 4, 0);
    callsLayout->addWidget(typeComboBox, 4, 1);
    callsLayout->addWidget(catModeLabel, 5, 0);
    callsLayout->addWidget(contestCatModeComboBox, 5, 1);
    callsLayout->addWidget(catOpLabel, 6, 0);
    callsLayout->addWidget(contestCatOperatorsComboBox, 6, 1);
    callsLayout->addWidget(catAsLabel, 7, 0);
    callsLayout->addWidget(contestCatAssistedComboBox, 7, 1);

    callsLayout->addWidget(catPowerLabel, 8, 0);
    callsLayout->addWidget(contestCatPowerComboBox, 8, 1);

    callsLayout->addWidget(catBandsLabel, 9, 0);
    callsLayout->addWidget(contestCatBandsComboBox, 9, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(callsLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    //page->setLayout(callsLayout);

}


void SetupPageLogsNew::slotStationCallSignTextChanged()
{
    //qDebug() << "SetupPageLogsNew::slotStationCallSignTextChanged" << endl;
//    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT( ) );
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
}


void SetupPageLogsNew::slotTypeComboBoxChanged(const int _ind)
{
    //qDebug() << "SetupPageLogsNew::slotTypeComboBoxChanged" << endl;
//    connect(typeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotTypeComboBoxChanged() ) ) ;

    int i = _ind;
    //int i = typeComboBox->currentIndex();
    //qDebug() << "SetupPageLogsNew::slotTypeComboBoxChanged: " << QString::number(i) << endl;


    switch (i)
    {

        case 0: // DX = No Contest
            typeOperation = 0;
            contestCatModeComboBox->setEnabled(false);
            contestCatOperatorsComboBox->setEnabled(false);
            contestCatAssistedComboBox->setEnabled(false);
            contestCatPowerComboBox->setEnabled(false);
            contestCatBandsComboBox->setEnabled(false);
            contestBandsComboBox->setEnabled(false);

        break;
        case 1: // CQ-WW-SSB
            typeOperation = 1;
            contestCatModeComboBox->setEnabled(true);
            contestCatOperatorsComboBox->setEnabled(true);
            contestCatAssistedComboBox->setEnabled(true);
            contestCatPowerComboBox->setEnabled(true);
            contestCatBandsComboBox->setEnabled(true);
            contestBandsComboBox->setEnabled(true);
        break;
        case 2: // CQ-WW-CW
            typeOperation = 2;
            contestCatModeComboBox->setEnabled(true);
            contestCatOperatorsComboBox->setEnabled(true);
            contestCatAssistedComboBox->setEnabled(true);
            contestCatPowerComboBox->setEnabled(true);
            contestCatBandsComboBox->setEnabled(true);
            contestBandsComboBox->setEnabled(true);
        break;
        case 3:  // CQ-WPX-SSB
            typeOperation = 3;
            contestCatModeComboBox->setEnabled(true);
            contestCatOperatorsComboBox->setEnabled(true);
            contestCatAssistedComboBox->setEnabled(true);
            contestCatPowerComboBox->setEnabled(true);
            contestCatBandsComboBox->setEnabled(true);
            contestBandsComboBox->setEnabled(true);
        break;
        case 4:  // CQ-WPX-CW
            typeOperation = 4;
            contestCatModeComboBox->setEnabled(true);
            contestCatOperatorsComboBox->setEnabled(true);
            contestCatAssistedComboBox->setEnabled(true);
            contestCatPowerComboBox->setEnabled(true);
            contestCatBandsComboBox->setEnabled(true);
            contestBandsComboBox->setEnabled(true);
        break;
        default:// DX = No Contest
            typeOperation = 0;
            contestCatModeComboBox->setEnabled(false);
            contestCatOperatorsComboBox->setEnabled(false);
            contestCatAssistedComboBox->setEnabled(false);
            contestCatPowerComboBox->setEnabled(false);
            contestCatBandsComboBox->setEnabled(false);
            contestBandsComboBox->setEnabled(false);
        break;

    }
}



void SetupPageLogsNew::slotCatAssistedComboBoxChanged()
{
   //qDebug() << "SetupPageLogs:slotCatAssistedComboBoxChanged: " << QString::number(contestCatAssistedComboBox->currentIndex())  << endl;
//connect(contestCatAssistedComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatAssistedComboBoxChanged() ) ) ;
}
void SetupPageLogsNew::slotCatOperatorsComboBoxChanged()
{
   //qDebug() << "SetupPageLogsNew::slotCatOperatorsComboBoxChanged(): " << QString::number(contestCatOperatorsComboBox->currentIndex())  << endl;
//connect(contestCatOperatorsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatOperatorsComboBoxChanged() ) ) ;
}
void SetupPageLogsNew::slotCatPowerComboBoxChanged()
{
    //qDebug() << "SetupPageLogsNew::slotCatPowerComboBoxChanged(): " << QString::number(contestCatPowerComboBox->currentIndex())  << endl;
//connect(contestCatPowerComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatPowerComboBoxChanged() ) ) ;
}

void SetupPageLogsNew::slotCatBandsComboBoxChanged()
{
    //qDebug() << "SetupPageLogsNew::slotCatBandsComboBoxChanged(): " << QString::number(contestCatBandsComboBox->currentIndex())  << endl;
//connect(contestCatBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatBandsComboBoxChanged() ) ) ;
}
void SetupPageLogsNew::slotBandsComboBoxChanged()
{
    //qDebug() << "SetupPageLogsNew::slotBandsComboBoxChanged(): " << QString::number(contestBandsComboBox->currentIndex()) << endl;
            //connect(contestBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandsComboBoxChanged() ) ) ;
}

void SetupPageLogsNew::slotCatModeComboBoxChanged()
{
    //qDebug() << "SetupPageLogsNew::slotModeComboBoxChanged(): " << QString::number(contestCatModeComboBox->currentIndex())  << endl;

}

void SetupPageLogsNew::slotOKButtonClicked()
{
    qDebug() << "SetupPageLogsNew::slotOkButtonClicked" << endl;
    stationCallsign = stationCallsignLineEdit->text();
    operators = operatorsLineEdit->text();
    comment = commentLineEdit->text();    
    dateString = dateEdit->date().toString("yyyy/MM/dd");

    if (typeComboBox->isEnabled())
    {
        typeContest = typeComboBox->currentIndex();
    }
    else
    {
        typeContest = -1;
    }

    if (contestCatModeComboBox->isEnabled())
    {
        contestCatMode = contestCatModeComboBox->currentIndex();
    }
    else
    {
        contestCatMode = -1;
    }

    if (contestCatOperatorsComboBox->isEnabled())
    {
        contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    }
    else
    {
        contestCatOperators = -1;
    }

    if (contestCatAssistedComboBox->isEnabled())
    {
        contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    }
    else
    {
        contestCatAssisted = -1;
    }

    if (contestCatPowerComboBox->isEnabled())
    {
        contestCatPower = contestCatPowerComboBox->currentIndex();
    }
    else
    {
        contestCatPower = -1;
    }

    if (contestCatBandsComboBox->isEnabled())
    {
        contestCatBands = contestCatBandsComboBox->currentIndex();
    }
    else
    {
        contestCatBands = -1;
    }

    if (contestBandsComboBox->isEnabled())
    {
        contestBands = contestBandsComboBox->currentIndex();
    }
    else
    {
        contestBands = -1;
    }

    gatherAndSend();
    close();


}
void SetupPageLogsNew::gatherAndSend()
{
    logData.clear();
    logData << stationCallsign << operators << comment << dateString
            << QString::number(typeContest)
            << QString::number(contestCatMode)
            << QString::number(contestCatOperators)
            << QString::number(contestCatAssisted)
            << QString::number(contestCatPower)
            << QString::number(contestCatBands)
            << QString::number(contestBands);

    emit newLogData(logData);

}
void SetupPageLogsNew::slotCancelButtonClicked()
{
    qDebug() << "SetupPageLogsNew::slotCancelButtonClicked" << endl;
    logData.clear();
    close();
}
