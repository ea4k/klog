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
    selectedEMode = ModeLotW;   //By default this widget will vbe used for LoTW Export.

    createUI();
}

void AdifLoTWExportWidget::createUI()
{
    stationCallsignComboBox->clear();
    stationCallsignComboBox->addItems(dataProxy->getStationCallSignsFromLog(-1));
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

    mainLayout->addWidget(okButton, 4, 1);
    mainLayout->addWidget(cancelButton, 4, 2);

    setLayout(mainLayout);
    connect(startDate, SIGNAL(dateChanged(QDate)), this, SLOT(slotDateChanged()) );
    connect(endDate, SIGNAL(dateChanged(QDate)), this, SLOT(slotDateChanged()) );
    connect(stationCallsignComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(slotStationCallsignChanged() ) ) ;
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOKPushButtonClicked() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelPushButtonClicked() ) );
}

void AdifLoTWExportWidget::setTopLabel()
{

}

void AdifLoTWExportWidget::fillTable()
{
    //qDebug() << "AdifLoTWExportWidget::fillTable " << endl;
    //header << tr("DX") << tr("Date/Time") << tr("Band") << tr("Mode");
    /*
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row+1)*(column+1)));
    tableWidget->setItem(row, column, newItem);
    */
    //QList<int> DataProxy_SQLite::getQSOsListLoTWNotSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool justQueued)
    QList<int> qsos;
    qsos.clear();
    qsos.append(dataProxy->getQSOsListLoTWNotSent(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true));
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

        //QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(pow(row, column+1)));
        //tableWidget->setItem(row, column, newItem);

    }

    //qDebug() << "AdifLoTWExportWidget::addQSO: - END"  << endl;
}

void AdifLoTWExportWidget::slotStationCallsignChanged()
{
    startDate->setDate(QDate::fromString((dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText())), "yyyy/MM/dd"));
    endDate->setDate(QDate::fromString((dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText())), "yyyy/MM/dd"));
    fillTable();
}

void AdifLoTWExportWidget::slotDateChanged()
{
    fillTable();
}

void AdifLoTWExportWidget::slotOKPushButtonClicked()
{
    this->hide();
    emit selection(stationCallsignComboBox->currentText(), startDate->date(), endDate->date());
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
    startDate->setDate(QDate::fromString((dataProxy->getFirstQSODateFromCall(stationCallsignComboBox->currentText())), "yyyy/MM/dd"));
    endDate->setDate(QDate::fromString((dataProxy->getLastQSODateFromCall(stationCallsignComboBox->currentText())), "yyyy/MM/dd"));
    event->accept();
}

void AdifLoTWExportWidget::setExportMode(const ExportMode _EMode)
{
    if (_EMode == ModeLotW)
    {
        setWindowTitle("KLog - QSOs to be uploaded to LoTW.");
        topLabel->setText(tr("This table shows the QSOs that will be sent to LoTW."));
    }
    else
    {
        setWindowTitle("KLog - QSOs to be exported to ADIF.");
        topLabel->setText(tr("This table shows the QSOs that will be exported to ADIF."));
    }
}
