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
      //qDebug() << "SetupPageLogsNew::SetupPageLogsNew"   << QT_ENDL;
    dataProxy = dp;
    util = new Utilities;
    editing = false;
    checking = false;
    logData.clear();
    stationCallsignFilled = false;
    operatorsFilled = true;
    stationCallsign = QString();
    operators = QString();
    comment = QString();
    dateString = QString();
    stationCallsignLineEdit = new QLineEdit;
    operatorsLineEdit = new QLineEdit;

    dateEdit = new QDateEdit;
    dateEdit->setDate(QDate::currentDate ());
    commentLineEdit = new QLineEdit;

    dateLabel = new QLabel(tr("&Date"));
    stationCallsignLabel = new QLabel(tr("&Station Callsign"));
    operatorsLabel = new QLabel(tr("&Operators"));
    commentLabel = new QLabel(tr("Comm&ent"));

    okButton = new QPushButton(tr("&Ok"), this);
    cancelButton = new QPushButton(tr("&Cancel"), this);
    createWidget();

    okButton->setEnabled(false);

       //qDebug() << "SetupPageLogsNew::SetupPageLogsNew - END"   << QT_ENDL;
}

SetupPageLogsNew::~SetupPageLogsNew()
{
    delete(util);
}

void SetupPageLogsNew::clear()
{
      //qDebug() << "SetupPageLogsNew::Clear - Start"   << QT_ENDL;
    stationCallsignLineEdit->clear();
    operatorsLineEdit->clear();
    dateEdit->setDate(QDate::currentDate ());

      //qDebug() << "SetupPageLogsNew::Clear - END"   << QT_ENDL;
}

void SetupPageLogsNew::createWidget()
{
      //qDebug() << "SetupPageLogsNew::createWidget - Start" << QT_ENDL;

    stationCallsignLabel->setWordWrap(true);
    operatorsLabel->setWordWrap(true);
    commentLabel->setWordWrap(true);
    dateLabel->setBuddy(dateEdit);

    stationCallsignLabel->setBuddy(stationCallsignLineEdit);
    operatorsLabel->setBuddy(operatorsLineEdit);
    commentLabel->setBuddy(commentLineEdit);

    stationCallsignLineEdit->setToolTip(tr("Callsign used for this log."));
    operatorsLineEdit->setToolTip(tr("Comma separated list of operators: callsign1, callsign2."));

    dateEdit->setToolTip(tr("Start date of this log."));
    commentLineEdit->setToolTip(tr("Add a comment about this log. If filled, it will be shown in the main KLog title to identify the log."));

    dateLabel->setWordWrap(true);

    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotStationCallSignTextChanged() ) );
    connect(operatorsLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOperatorsTextChanged() ) );
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

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(callsLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    clear();

      //qDebug() << "SetupPageLogsNew::createWidget - End" << QT_ENDL;

}

void SetupPageLogsNew::slotOperatorsTextChanged()
{
       //qDebug() << "SetupPageLogsNew::slotOperatorsTextChanged - Start" << QT_ENDL;
//    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT( ) );
    if ((operatorsLineEdit->text()).length()<1)
    {
        return;
    }

    int cursorP = operatorsLineEdit->cursorPosition();

    QString currentQrz = util->getClearSQLi (operatorsLineEdit->text());
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
      //qDebug() << "SetupPageLogsNew::slotOperatorsTextChanged - End" << QT_ENDL;
}

void SetupPageLogsNew::slotStationCallSignTextChanged()
{
       //qDebug() << "SetupPageLogsNew::slotStationCallSignTextChanged" << QT_ENDL;
//    connect(stationCallsignLineEdit, SIGNAL(textChanged(QString)), this, SLOT( ) );
    if ((stationCallsignLineEdit->text()).length()<1)
    {
        showNOK();
        return;
    }


    int cursorP = stationCallsignLineEdit->cursorPosition();

    QString currentQrz = util->getClearSQLi (stationCallsignLineEdit->text());
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
      //qDebug() << "SetupPageLogsNew::slotStationCallSignTextChanged - End" << QT_ENDL;
}

void SetupPageLogsNew::slotOKButtonClicked()
{

       //qDebug() << "SetupPageLogsNew::slotOkButtonClicked" << QT_ENDL;
    stationCallsign = stationCallsignLineEdit->text();
    if (stationCallsign.length()<3)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You need to enter a valid callsign in the Station Callsign box.\nThe log will not be opened."));
        msgBox.exec();
        return;
    }

    operators = operatorsLineEdit->text();
    //TODO: Check if operators is really including a comma separated list of QRZ
    comment = commentLineEdit->text();
    dateString = dateEdit->date().toString("yyyy-MM-dd");

    logData.clear();
    logData << stationCallsign << operators << comment << dateString;
    if (editing)
    {
        logData << "1";
        editing = false;
    }
    else
    {
        logData << "0";
    }
    emit newLogData(logData);

    clear();
    accept();
}

void SetupPageLogsNew::gatherAndSend()
{
        //qDebug() << "SetupPageLogsNew::gatherAndSend: "  << QT_ENDL;

    // The following lines will be removed once more contest types have been added
    //contestCatMode = contestCatModeComboBox->currentIndex();
    //contestCatBands = contestCatBandsComboBox->currentIndex();
    //contestCatPower = contestCatPowerComboBox->currentIndex();
    //contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    //contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    //typeContestSelected = typeComboBox->currentIndex();
    //contestCatOverlay = contestCatOverlayComboBox->currentIndex();
    //typeContest = getSelectedTypeContest();

    // The previous lines will be removed once more contest types have been added

    logData.clear();
    logData << stationCallsign << operators << comment << dateString;

    /*
            << typeConteststr
            << QString::number(contestCatMode)
            << QString::number(contestCatOperators)
            << QString::number(contestCatAssisted)
            << QString::number(contestCatPower)
            << QString::number(contestCatBands)
            << QString::number(contestBands)
            << QString::number(contestCatOverlay)
            << QString::number(typeContest);
    */

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

       //qDebug() << "SetupPageLogsNew::gatherAndSend: EMITED" << QT_ENDL;
    emit newLogData(logData);

}

void SetupPageLogsNew::slotCancelButtonClicked()
{
       //qDebug() << "SetupPageLogsNew::slotCancelButtonClicked" << QT_ENDL;
    logData.clear();
    emit cancelled(true);
    setResult(QDialog::Rejected);
    clear();
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

       //qDebug() << "SetupPageLogsNew::getValidCatOptions: " << QString::number(_currentCat) <<"/"<< QString::number(_higherCat) << QT_ENDL;
    //return dataProxy->getValidCatOptions(_currentCat, _higherCat);
    return QStringList();
}
*/

void SetupPageLogsNew::setStationCallSign(const QString &_st)
{
    stationCallsign = _st;
    stationCallsignLineEdit->setText(stationCallsign.toUpper());
}

void SetupPageLogsNew::setOperators(const QString &_st)
{
    operators = _st;
    operatorsLineEdit->setText(operators.toUpper());
}

void SetupPageLogsNew::setComment(const QString &_st)
{
    comment = _st;
    commentLineEdit->setText(comment);
}

void SetupPageLogsNew::setDateString(const QString &_st)
{
    dateString = _st;
    dateEdit->setDate(QDate::fromString(dateString, "yyyy-MM-dd"));

}

/*
void SetupPageLogsNew::setType(const QString &_st)
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

/*
int SetupPageLogsNew::getSelectedTypeContest()
{
       //qDebug() << "SetupPageLogsNew::getSelectedTypeContest: " << QT_ENDL;
    //is/contest/catoperator/catassisted/catpower/catband/catoverlay/catmode
    int i = dataProxy->getContestTypeN(typeContestSelected, contestCatOperators, contestCatAssisted, contestCatPower, contestCatBands, contestCatOverlay, contestCatMode);
       //qDebug() << "SetupPageLogsNew::getSelectedTypeContest: " << QString::number(i) << QT_ENDL;
    return i;

}

void SetupPageLogsNew::setTypeN(const int _n)
{
       //qDebug() << "SetupPageLogsNew::setTypeN: " << QString::number(_n) << QT_ENDL;
    typeContestSelected = _n;
    fillWithType(typeContestSelected);
}

void SetupPageLogsNew::fillWithType(const int _n)
{
       //qDebug() << "SetupPageLogsNew::fillWithType - n = " << QString::number(_n) << QT_ENDL;
    typeContestSelected = _n;
    QStringList contestData;
    contestData << dataProxy->getDataFromContestType(_n);
       //qDebug() << "SetupPageLogsNew::fillWithType-1 (length = " << QString::number(contestData.length()) << ")" << QT_ENDL;
    if (contestData.length()== 8)
    {

           //qDebug() << "SetupPageLogsNew::fillWithType-2" << QT_ENDL;
        //setCOperators ((contestData.at(1)).toInt());
        //setCAssisted ((contestData.at(2)).toInt());
        //setCPower ((contestData.at(3)).toInt());
        //setCOverlay ((contestData.at(4)).toInt());
        //setCMode ((contestData.at(5)).toInt());
        //setType(contestData.at(6));
        //setCBands((contestData.at(7)).toInt());
           //qDebug() << "SetupPageLogsNew::fillWithType: " << contestData.at(6) << QT_ENDL;
    }
    else
    {
           //qDebug() << "SetupPageLogsNew::fillWithType-3" << QT_ENDL;
        return;
    }

}


void SetupPageLogsNew::updateAllCats()
{
       //qDebug() << "SetupPageLogsNew::updateAllCats" << QT_ENDL;

    contestCatMode = contestCatModeComboBox->currentIndex();
    contestCatBands = contestCatBandsComboBox->currentIndex();
    contestCatPower = contestCatPowerComboBox->currentIndex();
    contestCatOperators = contestCatOperatorsComboBox->currentIndex();
    contestCatAssisted = contestCatAssistedComboBox->currentIndex();
    typeContestSelected = typeComboBox->currentIndex();
    contestCatOverlay= contestCatOverlayComboBox->currentIndex();
    typeContest = getSelectedTypeContest();

}
    */

void SetupPageLogsNew::showOK()
{
    //validCats->setText(tr("Data OK"));
    //validCats->setStyleSheet("QLabel {color : black; }");
    okButton->setEnabled(true);

}

void SetupPageLogsNew::showNOK()
{
    //validCats->setText(tr("Data not OK"));
    //validCats->setStyleSheet("QLabel {color : red; }");
    okButton->setEnabled(false);
}
