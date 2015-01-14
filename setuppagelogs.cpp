/***************************************************************************
                          setuppagelogs.cpp  -  description
                             -------------------
    begin                : feb 2012
    copyright            : (C) 2012 by Jaime Robles
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

//#include <QDebug>

#include "setuppagelogs.h"


SetupPageLogs::SetupPageLogs(QWidget *parent) : QWidget(parent){
//qDebug() << "SetupPageLogs::SetupPageLogs" << endl;

    stationCallsignFilled = false;
    operatorsFilled = true;
    logsModel = new QSqlRelationalTableModel(this);
    logsView = new QTableView;
    logsView->setContextMenuPolicy(Qt::CustomContextMenu);
    logsView->setSortingEnabled(true);

    createLogsModel();
    createLogsPanel();

    logsView->setCurrentIndex(logsModel->index(0, 0));

    lastLog = 0;


    newLogPushButton = new QPushButton(tr("New"), this);
    //loadAllPushButton = new QPushButton(tr("Load All"), this);
    //loadSelectedPushButton = new QPushButton(tr("Load"), this);
    //clearPushButton = new QPushButton(tr("Clear"), this);
    editPushButton = new QPushButton(tr("Edit"), this);
    removePushButton = new QPushButton(tr("Remove"), this);


    newLogPushButton->setToolTip(tr("Add a new log"));
    //loadAllPushButton->setToolTip(tr("Load all the logs"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected log"));
    //clearPushButton->setToolTip(tr("Clear selection"));
    editPushButton->setToolTip(tr("Edit the selected log"));
    removePushButton->setToolTip(tr("Remove the selected log"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(newLogPushButton);
    //buttonsLayout->addWidget(loadSelectedPushButton);
    //buttonsLayout->addWidget(loadAllPushButton);
    buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);
    //buttonsLayout->addWidget(clearPushButton);


    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(logsView);
    widgetLayout->addLayout(buttonsLayout);
    //widgetLayout->addLayout(logDataLayout);


    setLayout(widgetLayout);

    //connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );

   // readLogs();
    createActions();

}

SetupPageLogs::~SetupPageLogs(){
    ////qDebug() << "SetupPageLogs::~SetupPageLogs" << endl;
}


QWizardPage *SetupPageLogs::createLogTypePage()
{

    QWizardPage *page = new QWizardPage;
    page->setTitle(tr("New Log"));

    QLabel *stationCallsignLabel = new QLabel(tr("StationCallsign"));
    stationCallsignLabel->setWordWrap(true);
    QLabel *operatorsLabel = new QLabel(tr("Operators"));
    operatorsLabel->setWordWrap(true);

    stationCallsignLineEdit = new QLineEdit;
    operatorsLineEdit = new QLineEdit;
    nameLineEdit = new QLineEdit;
    dateEdit = new QDateEdit;

    stationCallsignLineEdit->setToolTip(tr("Callsign used for this log"));
    operatorsLineEdit->setToolTip(tr("Comma separated list of operators: callsign1, callsign2"));
    nameLineEdit->setToolTip(tr("Your name or the station's responsible"));
    dateEdit->setToolTip(tr("Start date of this log"));


    //callsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));

    QLabel *typeLabel = new QLabel(tr("Type of Operation"));
    typeLabel->setWordWrap(true);
    QLabel *nameLabel = new QLabel(tr("Name"));
    nameLabel->setWordWrap(true);
    QLabel *dateLabel = new QLabel(tr("Date"));
    dateLabel->setWordWrap(true);

    typeComboBox = new QComboBox;
    contestCatModeComboBox = new QComboBox;
    contestCatOperatorsComboBox = new QComboBox;
    contestCatAssistedComboBox = new QComboBox;
    contestCatPowerComboBox = new QComboBox;
    contestCatBandsComboBox = new QComboBox;
    contestBandsComboBox = new QComboBox;

    typeComboBox->setToolTip(tr("Select the kind of operation for this log"));
    QStringList _qs;
    _qs.clear();
    _qs << "DX" << "CQ-WW-SSB"  << "CQ-WW-CW"  << "CQ-WPX-SSB"  << "CQ-WPX-CW" ;
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

    QGridLayout *callsLayout = new QGridLayout; // Widget, row, column
    callsLayout->addWidget(stationCallsignLabel, 0, 0);
    callsLayout->addWidget(stationCallsignLineEdit, 0, 1);
    callsLayout->addWidget(dateLabel, 1, 0);
    callsLayout->addWidget(dateEdit, 1, 1);
    //callsLayout->addWidget(nameLabel, 0, 0);
    //callsLayout->addWidget(nameLineEdit, 0, 1);
    callsLayout->addWidget(operatorsLabel, 2, 0);
    callsLayout->addWidget(operatorsLineEdit, 2, 1);
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


    //page->registerField("stationCallSignLineEdit*", stationCallsignLineEdit);
    //page->registerField("operatorsLineEdit*", operatorsLineEdit);

    page->setLayout(callsLayout);

    return page;
}

void SetupPageLogs::slotNewButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotNewButtonClicked" << endl;
    newLogWizard = new QWizard (this);

     finishButton = newLogWizard->button(QWizard::FinishButton);
   // QAbstractButton* cancelButton = this->button(QWizard::CancelButton);
   // newLogWizard->setButton(finishButton);
    //newLogWizard->setButton(cancelButton);

    //QList<QWizard::WizardButton> wblayout;
    //wblayout << QWizard::Stretch << cancelButton << finishButton;
    //wblayout << *finishButton;
    //newLogWizard->setButtonLayout(wblayout);
    finishButton->setDisabled(true);
    newLogWizard->setButton(QWizard::FinishButton, finishButton);

    //finishButton->setEnabled(false);

    /*

    QAbstractButton* finishButton = this->button(QWizard::FinishButton);
    QAbstractButton* cancelButton = this->button(QWizard::CancelButton);
    connect(finishButton, SIGNAL(clicked()), this, SLOT(slotButtonFinishedClicked()));

    disconnect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect(cancelButton, SIGNAL( clicked() ), this, SLOT( slotCancelWizard() ) );



*  ...
     QList<QWizard::WizardButton> layout;
     layout << QWizard::Stretch << QWizard::BackButton << QWizard::CancelButton
            << QWizard::NextButton << QWizard::FinishButton;
     setButtonLayout(layout);

    wizard()->setButtonText(QWizard::CustomButton1, tr("&Print"));
            wizard()->setOption(QWizard::HaveCustomButton1, true);
            connect(wizard(), SIGNAL(customButtonClicked(int)),
                    this, SLOT(printButtonClicked()));
    */
    newLogWizard->setModal(true);
    newLogWizard->addPage(createLogTypePage());
    newLogWizard->setWindowTitle(tr("New Log"));
    newLogWizard->show();

}

void SetupPageLogs::slotEditButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotEditButtonClicked" << endl;
}

void SetupPageLogs::slotRemoveButtonClicked()
{
    //qDebug() << "SetupPageLogs::slotRemoveButtonClicked" << endl;
}

void SetupPageLogs::createLogsPanel()
{
    logsView->setModel(logsModel);
    QString stringQuery = QString("SELECT * FROM logs");
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record(); // Number of columns
    int columns = rec.count();

    for (int i = 0; i < columns; i++ ){
        logsView->setColumnHidden(i, true);
    }

    columns = rec.indexOf("id");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("logdate");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("stationcall");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("comment");
    logsView->setColumnHidden(columns, false);
    columns = rec.indexOf("logtype");
    logsView->setColumnHidden(columns, false);

    logsView->setItemDelegate(new QSqlRelationalDelegate(this));
    logsView->setSelectionMode( QAbstractItemView::SingleSelection);
    logsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    logsView->resizeColumnsToContents();
    logsView->horizontalHeader()->setStretchLastSection(true);

}

void SetupPageLogs::createLogsModel()
{
        QString stringQuery = QString("SELECT * FROM logs");
        QSqlQuery q(stringQuery);
        QSqlRecord rec = q.record();
/*
      query.exec("CREATE TABLE logs ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "logdate VARCHAR(10), "
                 "stationcall VARCHAR(15) NOT NULL, "
                 "comment VARCHAR, "
                 "logtype VARCHAR )");

*/
        int nameCol;

        //logsModel = new QSqlRelationalTableModel(this);
        logsModel->setTable("logs");

        nameCol = rec.indexOf("id");
        logsModel->setSort(nameCol, Qt::AscendingOrder);
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("ID"));

        nameCol = rec.indexOf("logdate");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Date"));

        nameCol = rec.indexOf("stationcall");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Station Callsign"));

        nameCol = rec.indexOf("comment");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Comments"));

        nameCol = rec.indexOf("logtype");
        logsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Type"));


        logsModel->select();
}


void SetupPageLogs::createActions()
{
    connect(newLogPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );
    connect(removePushButton, SIGNAL(clicked ( )), this, SLOT(slotRemoveButtonClicked() ) );
    connect(editPushButton, SIGNAL(clicked ( )), this, SLOT(slotEditButtonClicked() ) );


    //loadAllPushButton->setToolTip(tr("Load all the logs"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected log"));
    //clearPushButton->setToolTip(tr("Clear selection"));

}

void SetupPageLogs::readLogs()
{
    //qDebug() << "SetupPageLogs::readLogs" << endl;

    QString aux, callUsed;
    QStringList _logs;
    QSqlQuery query;
    QDate date = QDate::currentDate();

    _logs.clear();
    aux = "SELECT lognumber FROM log";

    query.exec(aux);
    QSqlRecord rec = query.record();

    while ( (query.next()) && (query.isValid()) ) {
        aux = (query.value(0)).toString();
        //qDebug() << "SetupPageLogs::readLogs: " << aux << endl;
        if (!(_logs.contains(aux)))
        {
            _logs += aux;
        }
    }
    int i = 0;
    int nameCol = 0;
    while (i <= _logs.size())
    {
        aux = QString("SELECT qso_date, station_callsign, operator FROM log WHERE lognumber = '%1'").arg(_logs.at(i));
        while ( (query.next()) && (query.isValid()) )
        {
            nameCol = rec.indexOf("qso_date");
            aux = (query.value(nameCol)).toString();
            if (date < QDate::fromString(aux, "yyyy/MM/dd"))
            {
                date = QDate::fromString(aux, "yyyy/MM/dd");
            }

                nameCol = rec.indexOf("station_callsign");
                aux = (query.value(nameCol)).toString();
                nameCol = rec.indexOf("operator");
                //revisar como va
                aux = (query.value(nameCol)).toString();

        }


    }

        //qDebug() << "SetupPageLogs::readLogs: " << QString::number(_logs.size())<< endl;
}

void SetupPageLogs::slotStationCallSignTextChanged()
{
    //qDebug() << "SetupPageLogs::slotStationCallSignTextChanged" << endl;
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

void SetupPageLogs::slotTypeComboBoxChanged(const int _ind)
{
    //qDebug() << "SetupPageLogs::slotTypeComboBoxChanged" << endl;
//    connect(typeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotTypeComboBoxChanged() ) ) ;

    int i = _ind;
    //int i = typeComboBox->currentIndex();
    //qDebug() << "SetupPageLogs::slotTypeComboBoxChanged: " << QString::number(i) << endl;


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



void SetupPageLogs::slotCatAssistedComboBoxChanged()
{
   //qDebug() << "SetupPageLogs:slotCatAssistedComboBoxChanged: " << QString::number(contestCatAssistedComboBox->currentIndex())  << endl;
//connect(contestCatAssistedComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatAssistedComboBoxChanged() ) ) ;
}
void SetupPageLogs::slotCatOperatorsComboBoxChanged()
{
   //qDebug() << "SetupPageLogs::slotCatOperatorsComboBoxChanged(): " << QString::number(contestCatOperatorsComboBox->currentIndex())  << endl;
//connect(contestCatOperatorsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatOperatorsComboBoxChanged() ) ) ;
}
void SetupPageLogs::slotCatPowerComboBoxChanged()
{
    //qDebug() << "SetupPageLogs::slotCatPowerComboBoxChanged(): " << QString::number(contestCatPowerComboBox->currentIndex())  << endl;
//connect(contestCatPowerComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatPowerComboBoxChanged() ) ) ;
}

void SetupPageLogs::slotCatBandsComboBoxChanged()
{
    //qDebug() << "SetupPageLogs::slotCatBandsComboBoxChanged(): " << QString::number(contestCatBandsComboBox->currentIndex())  << endl;
//connect(contestCatBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotCatBandsComboBoxChanged() ) ) ;
}
void SetupPageLogs::slotBandsComboBoxChanged()
{
    //qDebug() << "SetupPageLogs::slotBandsComboBoxChanged(): " << QString::number(contestBandsComboBox->currentIndex()) << endl;
            //connect(contestBandsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandsComboBoxChanged() ) ) ;
}

void SetupPageLogs::slotCatModeComboBoxChanged()
{
    //qDebug() << "SetupPageLogs::slotModeComboBoxChanged(): " << QString::number(contestCatModeComboBox->currentIndex())  << endl;

}


