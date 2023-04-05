/***************************************************************************
                          showadifimportwidget.cpp  -  description
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

#include "showadifimportwidget.h"


ShowAdifImportWidget::ShowAdifImportWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent) : QWidget(parent)
{
    Q_UNUSED(_parentFunction);
    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
    okButton = new QPushButton;
    //cancelButton = new QPushButton;
    tableWidget = new QTableWidget;
    qsosList.clear();
    setWindowTitle("ShowAdif");
    createUI();
    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());
}

ShowAdifImportWidget::~ShowAdifImportWidget()
{
    delete(util);
}

void ShowAdifImportWidget::createUI()
{
    QLabel *msgLabel = new QLabel;
    msgLabel->setText(tr("The following QSOs are those QSOs that you have received the LoTW confirmation."));
    okButton->setText(tr("Ok"));
    //cancelButton->setText(tr("Cancel"));

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
    mainLayout->addWidget(msgLabel, 0, 0, 1, -1);
    mainLayout->addWidget(tableWidget, 1, 0, 1, -1);

    mainLayout->addWidget(okButton, 2, 1);
   // mainLayout->addWidget(cancelButton, 2, 2);

    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOKPushButtonClicked() ) );
   // connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelPushButtonClicked() ) );
}

void ShowAdifImportWidget::fillTable()
{
    //qDebug() << "ShowAdifImportWidget::fillTable ";
    //header << tr("DX") << tr("Date/Time") << tr("Band") << tr("Mode");

    //QList<int> qsos;
    //qsos.clear();
    //qsos.append(dataProxy->getQSOsListLoTWToSend(stationCallsignComboBox->currentText(), startDate->date(), endDate->date(), true));
    //qDebug() << "ShowAdifImportWidget::fillTable QSOS: " << QString::number(qsosList.length());

    //QString aux, prefix;
    //qDebug() << "ShowAdifImportWidget::fillTable: -3" ;
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    if (tableWidget->columnCount()>0)
    {
       //qDebug() << "ShowAdifImportWidget::fillTable pre FOR";
        for (int i=0; i<qsosList.length(); i++)
        {
            //qDebug() << "ShowAdifImportWidget::fillTable in FOR " << QString::number(i);
            addQSO(qsosList.at(i));
        }
    }
    //qDebug() << "ShowAdifImportWidget::fillTable END";
}

void ShowAdifImportWidget::addQSO(const QStringList &_qso)
{
    //qDebug() << "ShowAdifImportWidget::addQSO: " << QString::number(_qsoID);
    // QRZ-DX, Date-Time, Band, Mode

    QStringList qsoToAdd;
    qsoToAdd.clear();
    qsoToAdd << _qso;

    //qDebug() << "ShowAdifImportWidget::addQSO: Columns: " << QString::number(tableWidget->columnCount());
    //qDebug() << "ShowAdifImportWidget::addQSO: qsoToAdd-length: " << QString::number(qsoToAdd.length());

    if (qsoToAdd.length() == tableWidget->columnCount())
    {
        tableWidget->insertRow(tableWidget->rowCount());

        for (int i = 0; i<qsoToAdd.length(); i++)
        {
            //qDebug() << "ShowAdifImportWidget::addQSO: qsoToAdd.at(i): " << qsoToAdd.at(i);
            QTableWidgetItem *newItemID = new QTableWidgetItem(qsoToAdd.at(i));
            newItemID->setTextAlignment(Qt::AlignCenter);
            newItemID->setFlags(Qt::NoItemFlags);
            tableWidget->setItem(tableWidget->rowCount()-1, i, newItemID);
        }

        //QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(pow(row, column+1)));
        //tableWidget->setItem(row, column, newItem);

    }

    //qDebug() << "ShowAdifImportWidget::addQSO: - END" ;
}


void ShowAdifImportWidget::slotOKPushButtonClicked()
{
    this->hide();
    //emit selection(stationCallsignComboBox->currentText(), startDate->date(), endDate->date());
    close();
}


void ShowAdifImportWidget::closeEvent(QCloseEvent *event)
{
    //qDebug() << "ShowAdifImportWidget::closeEvent";
    event->accept();
}

void ShowAdifImportWidget::showEvent(QShowEvent *event)
{
    //qDebug() << "ShowAdifImportWidget::showEvent";
    fillTable();
    event->accept();
}

void ShowAdifImportWidget::addQSOToTheList(const QStringList _qso)
{
    //qDebug() << "ShowAdifImportWidget::addQSOToTheList - Start";
    // QRZ-DX, Date-Time(yyyyMMdd-hhmmss), Band, Mode
    if (_qso.length()!=4)
    {
        //qDebug() << "ShowAdifImportWidget::addQSOToTheList - NO valid qso list received";
        return;
    }
    if (!util->isValidCall(_qso.at(0)))
    {
        //qDebug() << "ShowAdifImportWidget::addQSOToTheList - NO valid QRZ received";
        return;
    }
    if (dataProxy->getIdFromBandName(_qso.at(2))<0)
    {
        //qDebug() << "ShowAdifImportWidget::addQSOToTheList - NO valid BAND received";
        return;
    }
    if (dataProxy->getIdFromModeName(_qso.at(3))<0)
    {
        //qDebug() << "ShowAdifImportWidget::addQSOToTheList - NO valid Mode received";
        return;
    }

    QDateTime _dateTime = util->getDateTimeFromSQLiteString(_qso.at(1));
    if (!_dateTime.isValid())
    {
        //qDebug() << "ShowAdifImportWidget::addQSOToTheList - NO valid DateTime received";
        return;
    }

    QStringList _newQSO;
    _newQSO.clear();

    _newQSO << _qso.at(0) << util->getDateTimeSQLiteStringFromDateTime(_dateTime) << _qso.at(2) << _qso.at(3);
    qsosList << _newQSO;
    //qDebug() << "ShowAdifImportWidget::addQSOToTheList QSO Added! - "<< _qso.at(0) <<" - END";
}

