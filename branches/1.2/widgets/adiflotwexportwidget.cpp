#include "adiflotwexportwidget.h"


AdifLoTWExportWidget::AdifLoTWExportWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent) : QWidget(parent)
{
    dataProxy = dp;
    stationCallsignComboBox = new QComboBox;
    startDate = new QDateEdit;
    endDate = new QDateEdit;
    okButton = new QPushButton;
    cancelButton = new QPushButton;
    tableWidget = new QTableWidget;
    topLabel = new QLabel;
    numberLabel = new QLabel;
    selectedEMode = ModeLotW;   //By default this widget will be used for LoTW Export.

    createUI();
}

void AdifLoTWExportWidget::createUI()
{
    //fillStationCallsignComboBox();
    stationCallsignComboBox->setToolTip(tr("Select the Station Callsign that you want to use to upload the log."));

    startDate->clear();    
    startDate->setToolTip(tr("Select the start date to export the QSOs. The default date is the date of the first QSO with this station callsign."));

    endDate->clear();

    //endDate->setDate(QDate::currentDate());
    startDate->setToolTip(tr("Select the end date to export the QSOs. The default date is the date of the last QSO with this station callsign."));
    //endDate->setToolTip(tr("Select the last date to export the QSOs. The default date is today."));

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
    connect(stationCallsignComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(slotStationCallsignChanged() ) ) ;
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOKPushButtonClicked() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelPushButtonClicked() ) );
}

void AdifLoTWExportWidget::fillStationCallsignComboBox()
{
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox" << endl;
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox: " << QString::number(stationCallsignComboBox->count()) << endl;
    stationCallsignComboBox->clear();
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-1" << endl;
    stationCallsignComboBox->addItem(tr("Not defined"));
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-2" << endl;
    if (currentExportMode == ModeADIF)
    {
         //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-3" << endl;
        stationCallsignComboBox->addItem(tr("All"));
         //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-4" << endl;
    }
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-99" << endl;
    stationCallsignComboBox->addItems(dataProxy->getStationCallSignsFromLog(-1));
    //qDebug() << "AdifLoTWExportWidget::fillStationCallsignComboBox-END" << endl;
}

void AdifLoTWExportWidget::setTopLabel(const QString &_t)
{    
    topLabel->setText(_t);
}

void AdifLoTWExportWidget::fillTable()
{
    //qDebug() << "AdifLoTWExportWidget::fillTable " << endl;
    QList<int> qsos;
       qsos.clear();
       bool justQueued;
       switch (currentExportMode)
       {
           case ModeADIF:
           justQueued = false;
           break;
       case ModeLotW:
           justQueued = true;

           break;
       }

       if (stationCallsignComboBox->currentIndex() == 0)
       { // Not defined station_callsign (blank)
           //
           qsos.append(dataProxy->getQSOsListLoTWNotSent(QString(), startDate->date(), endDate->date(), justQueued));
       }
       else if(stationCallsignComboBox->currentIndex() == 1)
       { // ALL stations, no matter the station.
           qsos.append(dataProxy->getQSOsListLoTWNotSent("ALL", startDate->date(), endDate->date(), justQueued));
       }
       else
       {
           qsos.append(dataProxy->getQSOsListLoTWNotSent(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), justQueued));
       }
       //qsos.append(dataProxy->getQSOsListLoTWNotSent(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true));
       //qDebug() << "AdifLoTWExportWidget::fillTable QSOS: " << QString::number(qsos.length()) << endl;

       QString aux, prefix;
       //qDebug() << "AdifLoTWExportWidget::fillTable: -3"  << endl;
       tableWidget->clearContents();
       tableWidget->setRowCount(0);
       if (tableWidget->columnCount()>0)
       {
          //qDebug() << "AdifLoTWExportWidget::fillTable pre FOR" << endl;
           for (int i=0; i<qsos.length(); i++)
           {
               //qDebug() << "AdifLoTWExportWidget::fillTable in FOR " << QString::number(i) << endl;
               addQSO(qsos.at(i));
           }

       }

       numberLabel->setText(tr("QSOs: ") + QString::number(qsos.count()));
       if (qsos.count()>0)
       {
           //qDebug() << "AdifLoTWExportWidget::fillTable Enable OKButton" << endl;
           okButton->setEnabled(true);

       }
       else
       {
           //qDebug() << "AdifLoTWExportWidget::fillTable Disable OKButton" << endl;
           okButton->setEnabled(false);
       }
    //qDebug() << "AdifLoTWExportWidget::fillTable END" << endl;
}

void AdifLoTWExportWidget::addQSO(const int _qsoID)
{
    //qDebug() << "AdifLoTWExportWidget::addQSO: " << QString::number(_qsoID) << endl;


    QStringList qsoToAdd;
    qsoToAdd.clear();
    qsoToAdd << dataProxy->getQSODetailsForLoTWDownload(_qsoID);

    //qDebug() << "AdifLoTWExportWidget::addQSO: Columns: " << QString::number(tableWidget->columnCount()) << endl;
    //qDebug() << "AdifLoTWExportWidget::addQSO: qsoToAdd-length: " << QString::number(qsoToAdd.length()) << endl;

    if (qsoToAdd.length() == tableWidget->columnCount())
    {
        tableWidget->insertRow(tableWidget->rowCount());

        for (int i = 0; i<qsoToAdd.length(); i++)
        {
            //qDebug() << "AdifLoTWExportWidget::addQSO: qsoToAdd.at(i): " << qsoToAdd.at(i) << endl;
            QTableWidgetItem *newItemID = new QTableWidgetItem(qsoToAdd.at(i));
            newItemID->setTextAlignment(Qt::AlignCenter);
            newItemID->setFlags(Qt::NoItemFlags);
            tableWidget->setItem(tableWidget->rowCount()-1, i, newItemID);
        }

    }

    //qDebug() << "AdifLoTWExportWidget::addQSO: - END"  << endl;
}

void AdifLoTWExportWidget::slotStationCallsignChanged()
{
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged"  << endl;
    if (stationCallsignComboBox->count()<1)
    {
        //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged count <1 "  << endl;
        return;
    }
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged-01"  << endl;
    startDate->setDate(dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText()));
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged-02"  << endl;
    endDate->setDate(dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText()));    
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged-03"  << endl;
    fillTable();
    //qDebug() << "AdifLoTWExportWidget::slotStationCallsignChanged - END" << endl;
}

void AdifLoTWExportWidget::slotDateChanged()
{
    //slotStationCallsignChanged();
    fillTable();
}

void AdifLoTWExportWidget::slotOKPushButtonClicked()
{
    //qDebug() << "AdifLoTWExportWidget::slotOKPushButtonClicked" << endl;
    this->hide();
    if (stationCallsignComboBox->currentIndex() == 0)
    {
        emit selection(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), currentExportMode);
    }
    else if (stationCallsignComboBox->currentIndex() == 1)
    {
        if (currentExportMode == ModeLotW)
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
    //qDebug() << "AdifLoTWExportWidget::slotOKPushButtonClicked - END" << endl;
    close();
}

void AdifLoTWExportWidget::slotCancelPushButtonClicked()
{
     //qDebug() << "AdifLoTWExportWidget::slotCancelPushButtonClicked" << endl;
     close();
}

void AdifLoTWExportWidget::closeEvent(QCloseEvent *event)
{
    //qDebug() << "AdifLoTWExportWidget::closeEvent" << endl;
    event->accept();
}

void AdifLoTWExportWidget::showEvent(QShowEvent *event)
{
    //qDebug() << "AdifLoTWExportWidget::showEvent" << endl;
    //startDate->setDate(QDate::fromString((dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText())), "yyyy-MM-dd"));
    //endDate->setDate(QDate::fromString((dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText())), "yyyy-MM-dd"));
    startDate->setDate(dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText()));
    endDate->setDate(dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText()));


    event->accept();
    //qDebug() << "AdifLoTWExportWidget::showEvent - END" << endl;
}

void AdifLoTWExportWidget::setExportMode(const ExportMode _EMode)
{
    //qDebug() << "AdifLoTWExportWidget::setExportMode" << endl;
    currentExportMode = _EMode;
    if (currentExportMode == ModeLotW)
    {
        setWindowTitle("KLog - QSOs to be uploaded to LoTW.");
        topLabel->setText(tr("This table shows the QSOs that will be sent to LoTW."));
    }
    else
    {
        setWindowTitle("KLog - QSOs to be exported to ADIF.");
        topLabel->setText(tr("This table shows the QSOs that will be exported to ADIF."));
    }
    fillStationCallsignComboBox();
    //qDebug() << "AdifLoTWExportWidget::setExportMode-END" << endl;
}
