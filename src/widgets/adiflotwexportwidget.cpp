/***************************************************************************
                          adiflotwexportwidget.cpp  -  description
                             -------------------
    begin                : July 2020
    copyright            : (C) 2020 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
#include <QMessageBox>
#include "adiflotwexportwidget.h"
#include "../callsign.h"
#include "../locator.h"


AdifLoTWExportWidget::AdifLoTWExportWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent) : QWidget(parent)
{
#ifdef QT_DEBUG
  //qDebug() << ": " << _parentFunction;
#else
#endif
    (void)_parentFunction;
    //qDebug() << Q_FUNC_INFO << " - Start: " + _parentFunction;
    dataProxy = dp;
    starting = true;
    //util = new Utilities(Q_FUNC_INFO);
    //util->setLongPrefixes(dataProxy->getLongPrefixes());
    //util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    stationCallsignComboBox = new QComboBox;
    myGridSquareComboBox = new QComboBox;
    startDate = new QDateEdit;
    endDate = new QDateEdit;
    okButton = new QPushButton;
    cancelButton = new QPushButton;
    tableWidget = new QTableWidget;
    topLabel = new QLabel;
    numberLabel = new QLabel;
    selectedEMode = ModeLotW;   //By default this widget will be used for LoTW Export.
    defaultStationCallsign = QString();
    defaultMyGrid = QString();
    numOfQSOsInThisLog = 0;
    createUI();
    starting = false;
    qsos.clear ();
     //qDebug() << Q_FUNC_INFO << " - END";
}
AdifLoTWExportWidget::~AdifLoTWExportWidget()
{
    //delete(util) ;
}

void AdifLoTWExportWidget::setDefaultStationCallsign(const QString &_st)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    Callsign callsign(_st);
    if (callsign.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - Call not valid!";
        defaultStationCallsign = _st;
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::setDefaultMyGrid(const QString &_st)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    Locator locator;
    if (locator.isValidLocator(_st))
    {
        defaultMyGrid = _st;
    }
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::createUI()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    tableWidget->setSortingEnabled (true);
    stationCallsignComboBox->setToolTip(tr("Select the Station Callsign that you want to use to upload the log."));
    startDate->setCalendarPopup (true);
    endDate->setCalendarPopup (true);
    startDate->clear();
    startDate->setToolTip(tr("Select the start date to export the QSOs. The default date is the date of the first QSO with this station callsign."));
    endDate->clear();

    //endDate->setDate(QDate::currentDate());
    endDate->setToolTip(tr("Select the end date to export the QSOs. The default date is the date of the last QSO with this station callsign."));

    QLabel *stationLabel = new QLabel;
    stationLabel->setText(tr("Station callsign"));

    QLabel *myGridLabel = new QLabel;
    myGridLabel->setText(tr("My Locator"));

    QLabel *startLabel = new QLabel;
    startLabel->setText(tr("Start date"));

    QLabel *endLabel = new QLabel;
    endLabel->setText(tr("End date"));

    okButton->setText(tr("Ok"));
    cancelButton->setText(tr("Cancel"));

    hv = tableWidget->verticalHeader();
    hv->hide();
    hv->setStretchLastSection(true);
    hh = tableWidget->horizontalHeader();

    QStringList header;
    header.clear();
    header << tr("DX") << tr("Date/Time") << tr("My Locator") << tr("Band") << tr("Mode");
    tableWidget->setColumnCount(header.length());
    tableWidget->setHorizontalHeaderLabels(header);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(topLabel, 0, 0, 1, -1);
    mainLayout->addWidget(stationLabel, 1, 0);
    mainLayout->addWidget(stationCallsignComboBox, 2, 0);

    mainLayout->addWidget(myGridLabel, 1, 1);
    mainLayout->addWidget(myGridSquareComboBox, 2, 1);

    mainLayout->addWidget(startLabel, 1, 2);
    mainLayout->addWidget(startDate, 2, 2);
    mainLayout->addWidget(endLabel, 1, 3);
    mainLayout->addWidget(endDate, 2, 3);
    mainLayout->addWidget(tableWidget, 3, 0, 1, -1);
    mainLayout->addWidget(numberLabel, 4, 0);
    mainLayout->addWidget(okButton, 4, 1);
    mainLayout->addWidget(cancelButton, 4, 2);

    setLayout(mainLayout);
    connect(startDate, SIGNAL(dateChanged(QDate)), this, SLOT(slotDateChanged())) ;
    connect(endDate, SIGNAL(dateChanged(QDate)), this, SLOT(slotDateChanged() ));
    connect(stationCallsignComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotStationCallsignChanged() ) ) ;
    connect(myGridSquareComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotMyGridChanged() ) ) ;
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOKPushButtonClicked() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelPushButtonClicked() ) );
     //qDebug() << Q_FUNC_INFO << " - END";
}


void AdifLoTWExportWidget::setDefaultStationComboBox()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    //stationCallsignComboBox->blockSignals(true);
    Callsign callsign(defaultStationCallsign);
    if (!callsign.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - END-1 (call not valid)";
        //stationCallsignComboBox->blockSignals(false);
        return;
    }
    if (stationCallsignComboBox->findText(defaultStationCallsign, Qt::MatchCaseSensitive) >= 0)
    {
       stationCallsignComboBox->setCurrentIndex(stationCallsignComboBox->findText(defaultStationCallsign, Qt::MatchCaseSensitive));
    }
    //stationCallsignComboBox->blockSignals(false);
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::setDefaultMyGridComboBox()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    Locator locator;
    if (!locator.isValidLocator(defaultMyGrid))
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    if (myGridSquareComboBox->findText(defaultMyGrid, Qt::MatchCaseSensitive) >= 0)
    {
       myGridSquareComboBox->setCurrentIndex(myGridSquareComboBox->findText(defaultMyGrid, Qt::MatchCaseSensitive));
       //qDebug() << Q_FUNC_INFO << ": 1: " << myGridSquareComboBox->currentText();
    }
    else if (myGridSquareComboBox->count()>=3)
    {
       myGridSquareComboBox->findText(myGridSquareComboBox->itemText(2), Qt::MatchCaseSensitive);
       //qDebug() << Q_FUNC_INFO << ": 2:  " << myGridSquareComboBox->currentText();
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": 3";
    }
     //qDebug() << Q_FUNC_INFO << " - END";
}

bool AdifLoTWExportWidget::fillStationCallsignComboBox()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    //stationCallsignComboBox->blockSignals(true);
    stationCallsignComboBox->clear();
    //qDebug() << Q_FUNC_INFO << " -1" ;
    stationCallsignComboBox->addItem(tr("Not defined"));
    //qDebug() << Q_FUNC_INFO << " -2" ;
    if (currentExportMode == ModeADIF)
    {
        //qDebug() << Q_FUNC_INFO << " -3";
        stationCallsignComboBox->addItem(tr("ALL"));
        //qDebug() << Q_FUNC_INFO << " -4";
    }
    QStringList stationCallsigns;
    stationCallsigns.clear();

    if (currentExportMode == ModeLotW)
    {
        stationCallsigns.append(dataProxy->getStationCallSignsFromLogWithLoTWPendingToSend(logNumber));
        //qDebug() << Q_FUNC_INFO << " -3";
        //stationCallsignComboBox->addItems();
        //qDebug() << Q_FUNC_INFO << " -4";
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " -5";
        stationCallsigns.append(dataProxy->getStationCallSignsFromLog(logNumber));
        //qDebug() << Q_FUNC_INFO << " -6";
    }
    if (stationCallsigns.count()<1)
    {
        return false;
    }
    stationCallsignComboBox->addItems(stationCallsigns);
    return true;

     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::fillStationMyGridComboBox()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    // Keep the grid that is shown now
    // clean and fill the combo.
    // If the saved locator is in the list, it is selected.
    //myGridSquareComboBox->blockSignals(true);
    QString tempGrid = myGridSquareComboBox->currentText ();
    myGridSquareComboBox->clear();
    QStringList grids;
    grids.clear ();

    Callsign callsign(stationCallsignComboBox->currentText());
    if (callsign.isValid())
    {
       //qDebug() << Q_FUNC_INFO << " - ValidCall: " << stationCallsignComboBox->currentText();
        grids.append (dataProxy->getGridsToBeSent (stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), currentExportMode, true, logNumber));
    }
    else if (stationCallsignComboBox->currentIndex() == 0)
    { // Not defined call
       //qDebug() << Q_FUNC_INFO << " - Not Defined" ;
        grids.append (dataProxy->getGridsToBeSent ("NOT", startDate->date(), endDate->date(), currentExportMode, true, logNumber));
    }
    else
    { //ALL calls
       //qDebug() << Q_FUNC_INFO << " - ALL Calls" ;
        //myGridSquareComboBox->addItem(tr("ALL"));
        grids.append (dataProxy->getGridsToBeSent ("ALL", startDate->date(), endDate->date(), currentExportMode, false, logNumber));
    }

    myGridSquareComboBox->addItem(tr("Not defined"));
    myGridSquareComboBox->addItem(tr("ALL"));
    myGridSquareComboBox->addItems(grids);

    if (myGridSquareComboBox->findText(tempGrid, Qt::MatchCaseSensitive) >= 0)
    {
       myGridSquareComboBox->setCurrentIndex(myGridSquareComboBox->findText(tempGrid, Qt::MatchCaseSensitive));
    }
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::setTopLabel(const QString &_t)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    topLabel->setText(_t);
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::fillDates()
{
    // Shows the first and last QSO done with one specific callsign.
    //qDebug() << Q_FUNC_INFO << " - Start";
    //startDate->blockSignals(true);
    //endDate->blockSignals(true);

    Callsign callsign(stationCallsignComboBox->currentText());
    if (callsign.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " - Not valid Call";
        startDate->setDate(dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText()));
        endDate->setDate(dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText()));
    }
    else if ((stationCallsignComboBox->currentIndex() == 0) && (stationCallsignComboBox->currentText().length()>3))
    {//Not defined
        //qDebug() << Q_FUNC_INFO << " - Not defined";
        startDate->setDate(dataProxy->getFirstQSODateFromCall("NOT"));
        endDate->setDate(dataProxy->getLastQSODateFromCall("NOT"));
    }
    else
    {//ALL
        //qDebug() << Q_FUNC_INFO << " - ALL";
        startDate->setDate(dataProxy->getFirstQSODateFromCall("ALL"));
        endDate->setDate(dataProxy->getLastQSODateFromCall("ALL"));
    }

    //startDate->blockSignals(false);
    //endDate->blockSignals(false);
    //qDebug() << Q_FUNC_INFO << " - END";
}

QList<int> AdifLoTWExportWidget::fillTable()
{
    //qDebug() << Q_FUNC_INFO << " - Start";

    QList<int> _qsos;
    _qsos.clear();
       //bool justQueued = true;

       QString _myGrid;
       if (myGridSquareComboBox->currentIndex () == 0)
       {
           _myGrid = "NOT";
       }
       else if (myGridSquareComboBox->currentIndex () == 1)
       {
           _myGrid = "ALL";
       }
       else
       {
            _myGrid = myGridSquareComboBox->currentText();
       }
        QString _myCall;
        if (stationCallsignComboBox->currentIndex () == 0)
        {
            //qDebug() << ": NOT";
            _myCall = "NOT";
        }
        else if ((stationCallsignComboBox->currentIndex () == 1) && (currentExportMode != ModeLotW))
        {
            //qDebug() << ": all";
            _myCall = "ALL";
        }
        else
        {
             _myCall = stationCallsignComboBox->currentText();
             //qDebug() << ": Call: " <<_myCall;
        }
        //qsos.clear ();
        switch (currentExportMode)
        {
            case ModeADIF:
            //justQueued = false;
            //_qsos.append(dataProxy->getQSOsListLoTWToSend(_myCall, startDate->date(), endDate->date(), justQueued, logNumber));
             _qsos.append(dataProxy->getQSOsListToBeExported(_myCall, _myGrid, startDate->date(), endDate->date(), logNumber));
            //qsos.append(dataProxy->getQSOsListToBeExported(_myCall, _myGrid, startDate->date(), endDate->date()));
            //qDebug() << Q_FUNC_INFO << " ADIF";
            break;
        case ModeLotW:
            //qDebug() << Q_FUNC_INFO << " LoTW";
            //justQueued = true;
            _qsos.append(dataProxy->getQSOsListLoTWToSend (_myCall, _myGrid, startDate->date(), endDate->date(), true, logNumber));
            //qsos.append(dataProxy->getQSOsListLoTWToSend (_myCall, _myGrid, startDate->date(), endDate->date(), true, logNumber));
            break;
        case ModeClubLog:
             //qDebug() << Q_FUNC_INFO << " ClubLog";
            _qsos.append(dataProxy->getQSOsListClubLogToSent(_myCall, startDate->date(), endDate->date(), true, logNumber));
            //qsos.append(dataProxy->getQSOsListClubLogToSent(_myCall, startDate->date(), endDate->date(), true, logNumber));
            break;
        case ModeEQSL:
            //qDebug() << Q_FUNC_INFO << " EQSL";
            //justQueued = true;
            _qsos.append(dataProxy->getQSOsListEQSLToSent(_myCall, startDate->date(), endDate->date(), true, logNumber));
            //qsos.append(dataProxy->getQSOsListEQSLToSent(_myCall, startDate->date(), endDate->date(), true));
            break;
        case ModeQRZ:
            //qDebug() << Q_FUNC_INFO << " QRZ";
            //justQueued = true;
            _qsos.append(dataProxy->getQSOsListQRZCOMToSent(_myCall, startDate->date(), endDate->date(), true, logNumber));
            //qsos.append(dataProxy->getQSOsListQRZCOMToSent(_myCall, startDate->date(), endDate->date(), true));
            break;
        }

       //qDebug() << Q_FUNC_INFO << " -3" ;
       tableWidget->clearContents();
       tableWidget->setRowCount(0);
       if (tableWidget->columnCount()>0)
       {
            //qDebug() << Q_FUNC_INFO << " pre FOR";
            for (int i=0; i<_qsos.length(); i++)
            {
               //qDebug() << Q_FUNC_INFO << " in FOR " << QString::number(i);
               addQSO(_qsos.at(i));
            }
       }

       //numberLabel->setText(tr("QSOs: ") + QString::number(_qsos.count()) + );
        numberLabel->setText(QString(tr("QSOs: %1/%2")).arg(_qsos.count()).arg(numOfQSOsInThisLog) );
        numberLabel->setToolTip (QString("The current number has %1 QSOs and your selection in this widget shows %2 QSOs").arg(numOfQSOsInThisLog).arg(_qsos.count ()));
       if (_qsos.count()>0)
       {
           //qDebug() << Q_FUNC_INFO << " Enable OKButton";
           okButton->setEnabled(true);
       }
       else
       {
           //qDebug() << Q_FUNC_INFO << " Disable OKButton";
           okButton->setEnabled(false);
       }
       return _qsos;
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::addQSO(const int _qsoID)
{
    //qDebug() << "AdifLoTWExportWidget::addQSO: " << QString::number(_qsoID);

    //qDebug() << Q_FUNC_INFO << " - Start";
    QStringList qsoToAdd;
    qsoToAdd.clear();
    qsoToAdd << dataProxy->getQSODetailsForLoTWDownload(_qsoID);

    //qDebug() << "AdifLoTWExportWidget::addQSO: Columns: " << QString::number(tableWidget->columnCount());
    //qDebug() << "AdifLoTWExportWidget::addQSO: qsoToAdd-length: " << QString::number(qsoToAdd.length());

    if (qsoToAdd.length() == tableWidget->columnCount())
    {
        tableWidget->insertRow(tableWidget->rowCount());

        for (int i = 0; i<qsoToAdd.length(); i++)
        {
            //qDebug() << "AdifLoTWExportWidget::addQSO: qsoToAdd.at(i): " << qsoToAdd.at(i);
            QTableWidgetItem *newItemID = new QTableWidgetItem(qsoToAdd.at(i));
            newItemID->setTextAlignment(Qt::AlignCenter);
            newItemID->setFlags(Qt::NoItemFlags);
            tableWidget->setItem(tableWidget->rowCount()-1, i, newItemID);
        }
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::slotStationCallsignChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    if (starting)
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    blockAllSignals (true);
    QString tmpCall = stationCallsignComboBox->currentText ();
    if (stationCallsignComboBox->count()<1)
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    //qDebug() << Q_FUNC_INFO << " - 01" ;
    fillDates();
    //qDebug() << Q_FUNC_INFO << " - 03" ;
    fillStationMyGridComboBox();
    updateIfNeeded();
    stationCallsignComboBox->setCurrentIndex(stationCallsignComboBox->findText(tmpCall, Qt::MatchCaseSensitive));
    blockAllSignals (false);
     //qDebug() << Q_FUNC_INFO << " - END";
}
void AdifLoTWExportWidget::blockAllSignals(const bool _b)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    stationCallsignComboBox->blockSignals (_b);
    myGridSquareComboBox->blockSignals (_b);
    startDate->blockSignals (_b);
    endDate->blockSignals (_b);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::updateIfNeeded()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    if (currentCall != stationCallsignComboBox->currentText())
    {
        //qDebug() << Q_FUNC_INFO << " - 1";
        currentCall = stationCallsignComboBox->currentText();
        qsos = fillTable();
    }
    else if (currentGrid != myGridSquareComboBox->currentText())
    {
        //qDebug() << Q_FUNC_INFO << " - 2";
        currentGrid = myGridSquareComboBox->currentText();
        qsos = fillTable();
    }
    else if (currentStart != startDate->date())
    {
        //qDebug() << Q_FUNC_INFO << " - 3";
        currentStart = startDate->date();
        qsos = fillTable();
    }
    else if (currentEnd != endDate->date())
    {
        //qDebug() << Q_FUNC_INFO << " - 3";
        currentEnd = endDate->date();
        qsos = fillTable();
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}


void AdifLoTWExportWidget::slotMyGridChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << myGridSquareComboBox->currentText();
    if (starting)
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    blockAllSignals (true);
    QString tmpGrid = myGridSquareComboBox->currentText ();
    //TODO: Add StationCallsigns that has been worked from that grid?
    //TODO: Update the startDate when that grid was activated?
    //TODO: Update the endDate when that grid was activated?

    qsos = fillTable();

    myGridSquareComboBox->setCurrentIndex(myGridSquareComboBox->findText(tmpGrid, Qt::MatchCaseSensitive));
    blockAllSignals (false);
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::slotDateChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    if (starting)
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }
    blockAllSignals (true);
    QDate tmpStartDate = startDate->date();
    QDate tmpEndDate = endDate->date();
    updateIfNeeded();
    startDate->setDate (tmpStartDate);
    endDate->setDate(tmpEndDate);
    blockAllSignals (false);
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::slotOKPushButtonClicked()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    this->hide();

    QString callToSend = getCallOnOK();

    emit qsosToSend (callToSend, qsos, currentExportMode);
    return;
}

QString AdifLoTWExportWidget::getCallOnOK()
{
    QString callToSend;

    int currentIndex = stationCallsignComboBox->currentIndex();
    QString currentText = stationCallsignComboBox->currentText();

    switch (currentIndex)
    {
    case 0:
       //qDebug() << Q_FUNC_INFO << " - StationCallsign index = 0";
        callToSend = "NOT";
        break;

    case 1:
       //qDebug() << Q_FUNC_INFO << " - StationCallsign index = 1";
        if (currentExportMode == ModeLotW ||
            currentExportMode == ModeClubLog ||
            currentExportMode == ModeQRZ ||
            currentExportMode == ModeEQSL)
        {
            callToSend = currentText;
        }
        else
        {
            callToSend = "ALL";
        }
        break;

    default:
        callToSend = currentText;
        break;
    }

    return callToSend;
}

/*
 * QString AdifLoTWExportWidget::getGridOnOK()
{
    QString gridToSend;

    int currentIndex = myGridSquareComboBox->currentIndex();
    QString currentText = myGridSquareComboBox->currentText();

    switch (currentIndex)
    {
    case 0:
       //qDebug() << Q_FUNC_INFO << " - myGridSquareComboBox index = 0";
        gridToSend = "NOT";
        break;

    case 1:
       //qDebug() << Q_FUNC_INFO << " - myGridSquareComboBox index = 1";
        gridToSend = "ALL";
        break;

    default:
        gridToSend = currentText;
        break;
    }

    return gridToSend;
}
*/

void AdifLoTWExportWidget::slotCancelPushButtonClicked()
{
     //qDebug() << Q_FUNC_INFO << " - Start";
     close();
      //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::closeEvent(QCloseEvent *event)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    event->accept();
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::showEvent(QShowEvent *event)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    starting = true;
    event->accept();
    numOfQSOsInThisLog = dataProxy->getHowManyQSOInLog(logNumber);
    if (!fillStationCallsignComboBox())
    {
        // Show the warning message and wait for the user to acknowledge it
        QMessageBox::warning(this, tr("KLog Warning"), tr("There are no queued QSOs."));

        // Close the widget after the user acknowledges the warning
        starting = false;
        QTimer::singleShot(0, this, &AdifLoTWExportWidget::close); // Schedule the widget to close
        return;

        //emit askingToClose();
        //return;          // Exit the function to stop further execution
    }
    //qDebug() << Q_FUNC_INFO << ": 1 - " << stationCallsignComboBox->currentText();
    setDefaultStationComboBox();
    //qDebug() << Q_FUNC_INFO << ": 2 - " << stationCallsignComboBox->currentText();
    fillDates();
    //qDebug() << Q_FUNC_INFO << ": 3 - " << stationCallsignComboBox->currentText();
    fillStationMyGridComboBox();
    setDefaultMyGridComboBox();
    //qDebug() << Q_FUNC_INFO << ": 4 - " << stationCallsignComboBox->currentText();
    starting = false;
    updateIfNeeded();
    //qDebug() << Q_FUNC_INFO << ": 5 - " << stationCallsignComboBox->currentText();
    //event->accept();
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::setExportMode(const ExportMode _EMode)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    currentExportMode = _EMode;
    if (currentExportMode == ModeLotW)
    {
        setWindowTitle(tr("KLog - QSOs to be uploaded to LoTW."));
        topLabel->setText(tr("This table shows the QSOs that will be sent to LoTW."));
    }
    else if (currentExportMode == ModeClubLog)
    {
        setWindowTitle(tr("KLog - QSOs to be uploaded to ClubLog."));
        topLabel->setText(tr("This table shows the QSOs that will be sent to ClubLog."));
    }
    else if (currentExportMode == ModeEQSL)
    {
        setWindowTitle(tr("KLog - QSOs to be uploaded to eQSL.cc."));
        topLabel->setText(tr("This table shows the QSOs that will be sent to eQSL.cc."));
    }
    else if (currentExportMode == ModeQRZ)
    {
        setWindowTitle(tr("KLog - QSOs to be uploaded to QRZ.com."));
        topLabel->setText(tr("This table shows the QSOs that will be sent to QRZ.com."));
    }
    else
    {
        setWindowTitle("KLog - QSOs to be exported to ADIF.");
        topLabel->setText(tr("This table shows the QSOs that will be exported to ADIF."));
    }

    //slotStationCallsignChanged();
     //qDebug() << Q_FUNC_INFO << " - END";
}

void AdifLoTWExportWidget::setLogNumber(const int _logN)
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    if (dataProxy->doesThisLogExist (_logN))
    {
        logNumber = _logN;
    }
    else
    {
        logNumber = -1;
    }
    numOfQSOsInThisLog = dataProxy->getHowManyQSOInLog(logNumber);
     //qDebug() << Q_FUNC_INFO << " - END";
}

