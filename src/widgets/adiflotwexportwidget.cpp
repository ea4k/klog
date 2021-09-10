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
#include "adiflotwexportwidget.h"


AdifLoTWExportWidget::AdifLoTWExportWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent) : QWidget(parent)
{
#ifdef QT_DEBUG
  //qDebug() << ": " << _parentFunction;
#else
#endif

    dataProxy = dp;
    util = new Utilities;
    stationCallsignComboBox = new QComboBox;
    startDate = new QDateEdit;
    endDate = new QDateEdit;
    okButton = new QPushButton;
    cancelButton = new QPushButton;
    tableWidget = new QTableWidget;
    topLabel = new QLabel;
    numberLabel = new QLabel;
    selectedEMode = ModeLotW;   //By default this widget will be used for LoTW Export.
    defaultStationCallsign = QString();

    createUI();
}
void AdifLoTWExportWidget::setDefaultStationCallsign(const QString &_st)
{
    if (util->isValidCall(_st))
    {
        defaultStationCallsign = _st;
    }
}

void AdifLoTWExportWidget::createUI()
{
    //fillStationCallsignComboBox();
    stationCallsignComboBox->setToolTip(tr("Select the Station Callsign that you want to use to upload the log."));

    startDate->clear();
    startDate->setToolTip(tr("Select the start date to export the QSOs. The default date is the date of the first QSO with this station callsign."));

    endDate->clear();

    //endDate->setDate(QDate::currentDate());
    endDate->setToolTip(tr("Select the end date to export the QSOs. The default date is the date of the last QSO with this station callsign."));


    QLabel *stationLabel = new QLabel;
    stationLabel->setText(tr("Station callsign"));

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
    header << tr("DX") << tr("Date/Time") << tr("Band") << tr("Mode");
    tableWidget->setColumnCount(header.length());
    tableWidget->setHorizontalHeaderLabels(header);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(topLabel, 0, 0, 1, -1);
    mainLayout->addWidget(stationLabel, 1, 0);
    mainLayout->addWidget(stationCallsignComboBox, 2, 0);
    mainLayout->addWidget(startLabel, 1, 1);
    mainLayout->addWidget(startDate, 2, 1);
    mainLayout->addWidget(endLabel, 1, 2);
    mainLayout->addWidget(endDate, 2, 2);
    mainLayout->addWidget(tableWidget, 3, 0, 1, -1);
    mainLayout->addWidget(numberLabel, 4, 0);
    mainLayout->addWidget(okButton, 4, 1);
    mainLayout->addWidget(cancelButton, 4, 2);

    setLayout(mainLayout);
    connect(startDate, SIGNAL(dateChanged(QDate)), this, SLOT(slotDateChanged())) ;
    connect(endDate, SIGNAL(dateChanged(QDate)), this, SLOT(slotDateChanged() ));
    connect(stationCallsignComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotStationCallsignChanged() ) ) ;
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOKPushButtonClicked() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelPushButtonClicked() ) );
}

void AdifLoTWExportWidget::setDefaultStationComboBox()
{
    if (!util->isValidCall(defaultStationCallsign))
    {
        return;
    }
    if (stationCallsignComboBox->findText(defaultStationCallsign, Qt::MatchCaseSensitive) >= 0)
    {
       stationCallsignComboBox->setCurrentIndex(stationCallsignComboBox->findText(defaultStationCallsign, Qt::MatchCaseSensitive));
    }
}

void AdifLoTWExportWidget::fillStationCallsignComboBox()
{
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox" << QT_ENDL;
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox: " << QString::number(stationCallsignComboBox->count()) << QT_ENDL;
    stationCallsignComboBox->clear();
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-1" << QT_ENDL;
    stationCallsignComboBox->addItem(tr("Not defined"));
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-2" << QT_ENDL;
    if (currentExportMode == ModeADIF)
    {
         //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-3" << QT_ENDL;
        stationCallsignComboBox->addItem(tr("All"));
         //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-4" << QT_ENDL;
    }
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-99" << QT_ENDL;
    stationCallsignComboBox->addItems(dataProxy->getStationCallSignsFromLog(logNumber));
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-END" << QT_ENDL;
}

void AdifLoTWExportWidget::setTopLabel(const QString &_t)
{
    topLabel->setText(_t);
}

void AdifLoTWExportWidget::fillTable()
{
    //qDebug() << "AdifLoTWExportWidget::fillTable " << QT_ENDL;
    QList<int> qsos;
       qsos.clear();
       bool justQueued = true;
       switch (currentExportMode)
       {
           case ModeADIF:
           justQueued = false;
           //qDebug() << "AdifLoTWExportWidget::fillTable ADIF" << QT_ENDL;

           break;
       case ModeLotW:
           //qDebug() << "AdifLoTWExportWidget::fillTable LoTW" << QT_ENDL;
           justQueued = true;

           break;
       case ModeClubLog:
           //qDebug() << "AdifLoTWExportWidget::fillTable ClubLog" << QT_ENDL;
           //justQueued = true;

           break;
       case ModeEQSL:
           //qDebug() << "AdifLoTWExportWidget::fillTable EQSL" << QT_ENDL;
           justQueued = true;
           break;
       case ModeQRZ:
           //qDebug() << "AdifLoTWExportWidget::fillTable QRZ" << QT_ENDL;
           justQueued = true;
           break;
       }

       if (stationCallsignComboBox->currentIndex() == 0)
       { // Not defined station_callsign (blank)
           //qDebug() << "AdifLoTWExportWidget::fillTable blank station callsign " << QT_ENDL;
           qsos.append(dataProxy->getQSOsListLoTWToSend(QString(), startDate->date(), endDate->date(), justQueued, logNumber));
       }
       else if((stationCallsignComboBox->currentIndex() == 1) && (currentExportMode == ModeADIF))
       { // ALL stations, no matter the station.
           //qDebug() << "AdifLoTWExportWidget::fillTable ALL station callsign " << QT_ENDL;
           qsos.append(dataProxy->getQSOsListLoTWToSend("ALL", startDate->date(), endDate->date(), justQueued, logNumber));
       }
       else
       {
           //qDebug() << "AdifLoTWExportWidget::fillTable OTHER station callsign " << QT_ENDL;
           if (currentExportMode == ModeClubLog)
           {
               //qDebug() << "AdifLoTWExportWidget::fillTable Mode ClubLog" << QT_ENDL;
               qsos.append(dataProxy->getQSOsListClubLogToSent(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true, logNumber));
           }
           else if (currentExportMode == ModeEQSL)
           {
               //qDebug() << "AdifLoTWExportWidget::fillTable Mode eQSL" << QT_ENDL;
              qsos.append(dataProxy->getQSOsListEQSLToSent(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true));
           }
           else if (currentExportMode == ModeQRZ)
           {
               //qDebug() << "AdifLoTWExportWidget::fillTable Mode QRZ" << QT_ENDL;
              qsos.append(dataProxy->getQSOsListQRZCOMToSent(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true));
           }
           else if (currentExportMode == ModeLotW)
           {
               //qDebug() << "AdifLoTWExportWidget::fillTable Mode QRZ" << QT_ENDL;
                qsos.append(dataProxy->getQSOsListLoTWToSend (stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true, logNumber));
              //qsos.append(dataProxy->getQSOsListQRZCOMToSent(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true));
           }
           else
           {//(currentExportMode == ModeADIF)
               //qDebug() << "AdifLoTWExportWidget::fillTable Mode LoTW" << QT_ENDL;
               qsos.append(dataProxy->getQSOsListToBeExported(stationCallsignComboBox->currentText(), startDate->date(), endDate->date()));
           }
       }

       //qDebug() << "AdifLoTWExportWidget::fillTable: -3"  << QT_ENDL;
       tableWidget->clearContents();
       tableWidget->setRowCount(0);
       if (tableWidget->columnCount()>0)
       {
          //qDebug() << "AdifLoTWExportWidget::fillTable pre FOR" << QT_ENDL;
           for (int i=0; i<qsos.length(); i++)
           {
               //qDebug() << "AdifLoTWExportWidget::fillTable in FOR " << QString::number(i) << QT_ENDL;
               addQSO(qsos.at(i));
           }
       }

       numberLabel->setText(tr("QSOs: ") + QString::number(qsos.count()));
       if (qsos.count()>0)
       {
           //qDebug() << "AdifLoTWExportWidget::fillTable Enable OKButton" << QT_ENDL;
           okButton->setEnabled(true);
       }
       else
       {
           //qDebug() << "AdifLoTWExportWidget::fillTable Disable OKButton" << QT_ENDL;
           okButton->setEnabled(false);
       }
    //qDebug() << "AdifLoTWExportWidget::fillTable END" << QT_ENDL;
}

void AdifLoTWExportWidget::addQSO(const int _qsoID)
{
    //qDebug() << "AdifLoTWExportWidget::addQSO: " << QString::number(_qsoID) << QT_ENDL;


    QStringList qsoToAdd;
    qsoToAdd.clear();
    qsoToAdd << dataProxy->getQSODetailsForLoTWDownload(_qsoID);

    //qDebug() << "AdifLoTWExportWidget::addQSO: Columns: " << QString::number(tableWidget->columnCount()) << QT_ENDL;
    //qDebug() << "AdifLoTWExportWidget::addQSO: qsoToAdd-length: " << QString::number(qsoToAdd.length()) << QT_ENDL;

    if (qsoToAdd.length() == tableWidget->columnCount())
    {
        tableWidget->insertRow(tableWidget->rowCount());

        for (int i = 0; i<qsoToAdd.length(); i++)
        {
            //qDebug() << "AdifLoTWExportWidget::addQSO: qsoToAdd.at(i): " << qsoToAdd.at(i) << QT_ENDL;
            QTableWidgetItem *newItemID = new QTableWidgetItem(qsoToAdd.at(i));
            newItemID->setTextAlignment(Qt::AlignCenter);
            newItemID->setFlags(Qt::NoItemFlags);
            tableWidget->setItem(tableWidget->rowCount()-1, i, newItemID);
        }

    }

    //qDebug() << "AdifLoTWExportWidget::addQSO: - END"  << QT_ENDL;
}

void AdifLoTWExportWidget::slotStationCallsignChanged()
{
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged"  << QT_ENDL;
    if (stationCallsignComboBox->count()<1)
    {
        //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged count <1 "  << QT_ENDL;
        return;
    }
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged-01"  << QT_ENDL;
    startDate->setDate(dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText()));
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged-02"  << QT_ENDL;
    endDate->setDate(dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText()));
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged-03"  << QT_ENDL;
    fillTable();
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged - END" << QT_ENDL;
}

void AdifLoTWExportWidget::slotDateChanged()
{
    //slotStationCallsignChanged();
    fillTable();
}

void AdifLoTWExportWidget::slotOKPushButtonClicked()
{
    //qDebug() << "AdifLoTWExportWidget::slotOKPushButtonClicked" << QT_ENDL;
    this->hide();
    if (stationCallsignComboBox->currentIndex() == 0)
    {
        emit selection("NOT", startDate->date(), endDate->date(), currentExportMode);
    }
    else if (stationCallsignComboBox->currentIndex() == 1)
    {
        if ((currentExportMode == ModeLotW) || (currentExportMode == ModeClubLog) || (currentExportMode == ModeQRZ)|| (currentExportMode == ModeEQSL))
        {
            emit selection(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), currentExportMode);
        }
        else
        {
            emit selection("ALL", startDate->date(), endDate->date(), currentExportMode);
        }
    }
    else
    {
        emit selection(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), currentExportMode);
    }
    //qDebug() << "AdifLoTWExportWidget::slotOKPushButtonClicked - END" << QT_ENDL;
    close();
}

void AdifLoTWExportWidget::slotCancelPushButtonClicked()
{
     //qDebug() << "AdifLoTWExportWidget::slotCancelPushButtonClicked" << QT_ENDL;
     close();
}

void AdifLoTWExportWidget::closeEvent(QCloseEvent *event)
{
    //qDebug() << "AdifLoTWExportWidget::closeEvent" << QT_ENDL;
    event->accept();
}

void AdifLoTWExportWidget::showEvent(QShowEvent *event)
{
    //qDebug() << "AdifLoTWExportWidget::showEvent" << QT_ENDL;

    startDate->setDate(dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText()));
    endDate->setDate(dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText()));
    setDefaultStationComboBox();

    event->accept();
    //qDebug() << "AdifLoTWExportWidget::showEvent - END" << QT_ENDL;
}

void AdifLoTWExportWidget::setExportMode(const ExportMode _EMode)
{
    //qDebug() << "AdifLoTWExportWidget::setExportMode" << QT_ENDL;
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
    fillStationCallsignComboBox();
    //qDebug() << "AdifLoTWExportWidget::setExportMode-END" << QT_ENDL;
}

void AdifLoTWExportWidget::setLogNumber(const int _logN)
{
    if (dataProxy->doesThisLogExist (_logN))
    {
        logNumber = _logN;
    }
    else
    {
        logNumber = -1;
    }
}
