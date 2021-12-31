/***************************************************************************
                          setuppagesats.cpp  -  description
                             -------------------
    begin                : dec 2018
    copyright            : (C) 2018 by Jaime Robles
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

//#include <QDebug>

#include "setuppages/setuppagesats.h"

SetupPageSats::SetupPageSats(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent){
       //qDebug() << "SetupPageSats::SetupPageSats" << QT_ENDL;
    dataProxy = dp;
    arrild = QString();
    satname = QString();
    uplink = QString();
    downlink = QString();
    modes = QString();
    newSat = new SetupPageSatsNew(dataProxy, this);

    util = new Utilities;

    selectedSat = -1;
    //defaultStationCallSign.clear();

    //setupD = new SetupDialog();

    currentSats = new QComboBox();
    satsAvailable.clear();

    //newSat = new SetupPageSatsNew(dataProxy);
    satsModel = new QSqlRelationalTableModel(this);
    satsView = new QTableView;
    satsView->setContextMenuPolicy(Qt::CustomContextMenu);
    satsView->setSortingEnabled(true);

    createSatsModel();
    createSatsPanel();

    satsView->setCurrentIndex(satsModel->index(0, 0));

    lastSat = 0;

    newSatPushButton = new QPushButton(tr("&New"), this);
    editPushButton = new QPushButton(tr("&Edit"), this);
    removePushButton = new QPushButton(tr("&Remove"), this);
    importPushButton = new QPushButton(tr("&Import"), this);
    exportPushButton = new QPushButton(tr("E&xport"), this);

    newSatPushButton->setToolTip(tr("Add a new satellite."));
    editPushButton->setToolTip(tr("Edit the selected satellite."));
    removePushButton->setToolTip(tr("Remove the selected satellite."));
    importPushButton->setToolTip(tr("Import a satellites file. It will replace the satellites you have in the current list."));
    exportPushButton->setToolTip(tr("Export your current satellites to a file."));

    currentSats->setToolTip(tr("Select the sat you want to open."));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(importPushButton);
    buttonsLayout->addWidget(exportPushButton);
    buttonsLayout->addWidget(newSatPushButton);
    buttonsLayout->addWidget(editPushButton);
    buttonsLayout->addWidget(removePushButton);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(satsView);
    widgetLayout->addWidget(currentSats);
    widgetLayout->addLayout(buttonsLayout);
    //widgetLayout->addLayout(SatDataLayout);


    setLayout(widgetLayout);



    //connect(newSatPushButton, SIGNAL(clicked ( )), this, SLOT(slotNewButtonClicked() ) );
    //QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
    //connect(setupD, SIGNAL(newSatRequested(true)), this, slotNewButtonClicked() )


    createActions();
    updateSelectedSats();


       //qDebug() << "SetupPageSats::SetupPageSats - END" << QT_ENDL;
}

SetupPageSats::~SetupPageSats(){
       //qDebug() << "SetupPageSats::~SetupPageSats" << QT_ENDL;
}


void SetupPageSats::createNewSat()
{
      //qDebug() << "SetupPageSats::createNewSat" << QT_ENDL;
    selectedSat = -1;
    //newSat->clear();
    newSat->setEditing(false);
    newSat->exec();
}

void SetupPageSats::slotNewButtonClicked()
{
       //qDebug() << "SetupPageSats::slotNewButtonClicked" << QT_ENDL;

    createNewSat();
}

void SetupPageSats::slotEditButtonClicked()
{
     //qDebug() << "SetupPageSats::slotEditButtonClicked" << QT_ENDL;
    //QSqlQuery query;
    //int nameCol = -1;

    selectedSat = getSelectedSat();
    QString shortName = dataProxy->getSateliteArrlIdFromId(selectedSat);

    newSat->clear();
    newSat->setEditing(true);
    newSat->setShortName(shortName);
    newSat->setName(dataProxy->getSatelliteName(shortName));
    newSat->setUpLink(dataProxy->getSatelliteFullUplink(shortName));
    newSat->setDownLink(dataProxy->getSatelliteFullDownlink(shortName));
    newSat->setModes(dataProxy->getSatelliteFullMode(shortName));
    newSat->exec();
    //newSat->setStationCallSign(dataProxy->getStationCallSignFromSat(selectedSat));
    //newSat->setOperators(dataProxy->getOperatorsFromSat(selectedSat));
    //newSat->setComment(dataProxy->getCommentsFromSat(selectedSat));
    //newSat->setDateString(dataProxy->getSatDateFromSat(selectedSat));
    //newSat->setTypeN(dataProxy->getSatTypeNFromSat(selectedSat).toInt());

/*
       //qDebug() << "SetupPageSats::slotEditButtonClicked-1 (selectedlog: " << QString::number(selectedLog) << ")" << QT_ENDL;
    QString stringQuery = QString("SELECT * FROM logs WHERE id='%1'").arg(selectedLog);
       //qDebug() << "SetupPageSats::slotEditButtonClicked -2" << QT_ENDL;

    bool sqlOk = query.exec(stringQuery);
    QSqlRecord rec = query.record();
    if (sqlOk)
    {
           //qDebug() << "SetupPageSats::slotEditButtonClicked Query OK" << QT_ENDL;
        QSqlRecord rec = query.record();
        if ( (query.next()) && (query.isValid()) )
        {//id/
/stationcall/comment/logtype/logtypeid
               //qDebug() << "SetupPageSats::slotEditButtonClicked Query Valid" << QT_ENDL;



            //nameCol = rec.indexOf("stationcall");
            //newLog->setStationCallSign((query.value(nameCol)).toString());
            //nameCol = rec.indexOf("operators");
            //newLog->setOperators((query.value(nameCol)).toString());




            //nameCol = rec.indexOf("comment");
            //newLog->setComment((query.value(nameCol)).toString());

            //nameCol = rec.indexOf("logdate");
            //newLog->setDateString((query.value(nameCol)).toString());

            //nameCol = rec.indexOf("logtypen");
               //qDebug() << "SetupPageSats::slotEditButtonClicked -3" << QT_ENDL;
            //newLog->setTypeN((query.value(nameCol)).toInt());

            newLog->exec();
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    */
}

/*
void SetupPageSats::slotSatsCancelled(const bool _q)
{
    selectedSat = -1;
}
*/

void SetupPageSats::slotRemoveButtonClicked()
{
       //qDebug() << "SetupPageSats::slotRemoveButtonClicked" << QT_ENDL;
    int selectedSat = getSelectedSat();

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("KLog"),
             tr("Do you really want to remove this satellite?") + "\n" +
                tr("This satellite will no be longer available to be selected ..."),
             QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {

        QString stringQuery = QString("DELETE FROM satellites WHERE id='%1'").arg(selectedSat);
        QSqlQuery query(stringQuery);

        bool sqlOk = query.exec();
        if (sqlOk)
        {

            satsModel->select();
            updateSelectedSats();
            stringQuery = QString("DELETE FROM log WHERE lognumber='%1'").arg(selectedSat);
            query.exec(stringQuery);
            sqlOk = query.exec();
               //qDebug() << "SetupPageSats::slotRemoveButtonClicked: LastQuery: " << query.lastQuery()  << QT_ENDL;
            if (sqlOk)
            {

                stringQuery = QString("DELETE FROM awarddxcc WHERE lognumber='%2'").arg(selectedSat);
                query.exec(stringQuery);
                sqlOk = query.exec();
                   //qDebug() << "SetupPageSats::slotRemoveButtonClicked: LastQuery: " << query.lastQuery()  << QT_ENDL;
                if (sqlOk)
                {
                       //qDebug() << "SetupPageSats::slotRemoveButtonClicked (AWARDDXCC REMOVED: " << QString::number(selectedSat) << ")" << QT_ENDL;
                }
                else
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                    showError(tr("Sat has not been removed. (#3)"));
                       //qDebug() << "SetupPageSats::slotRemoveButtonClicked (AWARDDXCC NOT REMOVED: " << QString::number(selectedSat) << ")" << QT_ENDL;
                }

            }
            else
            {
                showError(tr("Sat has not been removed. (#2)"));
                   //qDebug() << "SetupPageSats::slotRemoveButtonClicked (QSOS NOT REMOVED: " << QString::number(selectedSat) << ")" << QT_ENDL;
            }
        }
        else
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            showError(tr("Sat has not been removed. (#1)"));
               //qDebug() << "SetupPageSats::slotRemoveButtonClicked (NOT REMOVED: " << QString::number(selectedSat) << ")" << QT_ENDL;
        }
    }

    //ASK FOR A CONFIRMATION

    //DELETE ALL THE QSO IN THE REMOVED Sat

}

void SetupPageSats::createSatsPanel()
{
       //qDebug() << "SetupPageSats::createSatsPanel" << QT_ENDL;
    satsView->setModel(satsModel);
    QString stringQuery = QString("SELECT * FROM satellites");
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record(); // Number of columns
    int columns = rec.count();

    for (int i = 0; i < columns; i++ ){
        satsView->setColumnHidden(i, true);
    }

    columns = rec.indexOf("id");
    satsView->setColumnHidden(columns, false);
    columns = rec.indexOf("satarrlid");
    satsView->setColumnHidden(columns, false);
    columns = rec.indexOf("satname");
    satsView->setColumnHidden(columns, false);
    columns = rec.indexOf("uplink");
    satsView->setColumnHidden(columns, false);
    columns = rec.indexOf("downlink");
    satsView->setColumnHidden(columns, false);
    columns = rec.indexOf("satmode");
    satsView->setColumnHidden(columns, false);

    satsView->setItemDelegate(new QSqlRelationalDelegate(this));
    satsView->setSelectionMode( QAbstractItemView::SingleSelection);
    satsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    satsView->resizeColumnsToContents();
    satsView->horizontalHeader()->setStretchLastSection(true);

}

void SetupPageSats::createSatsModel()
{
     //qDebug() << "SetupPageSats::createsatsModel" << QT_ENDL;

        QString stringQuery = QString("SELECT * FROM satellites");
        QSqlQuery q(stringQuery);
        QSqlRecord rec = q.record();

        int nameCol;

        //satsModel = new QSqlRelationalTableModel(this);
        satsModel->setTable("satellites");

        nameCol = rec.indexOf("id");
        satsModel->setSort(nameCol, Qt::AscendingOrder);
        satsModel->setHeaderData(nameCol, Qt::Horizontal, tr("ID"));

        nameCol = rec.indexOf("satarrlid");
        satsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Short"));

        nameCol = rec.indexOf("satname");
        satsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Name"));

        nameCol = rec.indexOf("uplink");
        satsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Uplink"));

        nameCol = rec.indexOf("downlink");
        satsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Downlink"));

        nameCol = rec.indexOf("satmode");
        satsModel->setHeaderData(nameCol, Qt::Horizontal, tr("Modes"));

        satsModel->select();
}

void SetupPageSats::slotSatselected(const QModelIndex & index)
{
     //qDebug() << "SetupPageSats::slotSatselected"  << QT_ENDL;
    int row = index.row();
    setSelectedSat((satsModel->index(row, 0)).data(0).toInt());

}

void SetupPageSats::slotSatDoubleClicked(const QModelIndex & index)
{
       //qDebug() << "SetupPageSats::slotSatDoubleClicked"  << QT_ENDL;

    int row = index.row();
    setSelectedSat((satsModel->index(row, 0)).data(0).toInt());
    slotEditButtonClicked();

}
void SetupPageSats::slotAnalyzeNewSatData(const QStringList _qs)
{
    updateSelectedSats();
}

void SetupPageSats::createActions()
{
       //qDebug() << "SetupPageSats::createActions" << QT_ENDL;
    connect(importPushButton, SIGNAL(clicked()), this, SLOT(slotImportButtonClicked() ) );
    connect(exportPushButton, SIGNAL(clicked()), this, SLOT(slotExportButtonClicked() ) );
    connect(newSatPushButton, SIGNAL(clicked()), this, SLOT(slotNewButtonClicked() ) );
    connect(removePushButton, SIGNAL(clicked()), this, SLOT(slotRemoveButtonClicked() ) );
    connect(editPushButton, SIGNAL(clicked()), this, SLOT(slotEditButtonClicked() ) );
    connect(newSat, SIGNAL(newSatData(QStringList)), this, SLOT(slotAnalyzeNewSatData(QStringList) ) );
    connect(satsView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotSatselected(QModelIndex) ) );
    connect(satsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotSatDoubleClicked(QModelIndex) ) );
    //connect(SatView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickSat( const QModelIndex& ) ) );

    //loadAllPushButton->setToolTip(tr("Load all the Sats"));
    //loadSelectedPushButton->setToolTip(tr("Load only the selected Sat"));
    //clearPushButton->setToolTip(tr("Clear selection"));

}

/*
bool SetupPageSats::addNewSat(const QStringList _qs)
{
       //qDebug() << "SetupPageSats::addNewSat: " << _qs.at(2) << QT_ENDL;
    QString aux = QString();
    int nameCol = -1;

    QString _dateString = _qs.at(0);
    QString _stationCallsign = _qs.at(1);
    QString _typeContest = _qs.at(2);
    QString _comment = _qs.at(3);
    QString _typeContestN = _qs.at(4);


    QString queryString = QString("SELECT * FROM logs WHERE logdate='%1' AND stationcall='%2' AND logtype='%3' AND logtypen='%4'").arg(_dateString).arg(_stationCallsign).arg(_typeContest).arg(_typeContestN);
    //"logs"
    //"id, logdate, stationcall, comment, logtype"
       //qDebug() << "SetupPageSats::addNewLog query1: " << queryString << QT_ENDL;
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);
    QSqlRecord rec = query.record(); // Number of columns

    while ( (query.next()) && (query.isValid()) )
    {
        nameCol = rec.indexOf("id");
        aux = (query.value(nameCol)).toString();
           //qDebug() << "SetupPageSats::addNewLog: id = " << aux << QT_ENDL;
        return false;
    }
    queryString = QString("INSERT INTO logs (logdate, stationcall, comment, logtype, logtypen) values('%1','%2','%3','%4', '%5')").arg(_dateString).arg(_stationCallsign).arg(_comment).arg(_typeContest).arg(_typeContestN);

       //qDebug() << "SetupPageSats::addNewSat query1: " << queryString << QT_ENDL;
    sqlOK = query.exec(queryString);
    if (sqlOK)
    {
           //qDebug() << "SetupPageSats::addNewSat ADDED! id = "  << QT_ENDL;
        satsModel->select();
        updateSelectedSats();
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
*/

void SetupPageSats::updateSelectedSats()
{
   //qDebug() << "SetupPageSats::updateSelectedSats" << QT_ENDL;
    satsAvailable = readSats();
    satsModel->select();

    if (satsAvailable.length()>0)
    {
        currentSats->clear();
        currentSats->addItems(satsAvailable);
    }
    else
    {
          //qDebug() << "SetupPageSats::updateSelectedSats Not selected (less than 1)" << QT_ENDL;
        currentSats->clear();
    }
}

QStringList SetupPageSats::readSats()
{
       //qDebug() << "SetupPageSats::readSats" << QT_ENDL;

    QString aux, aux2;
    QStringList _sats;
    QSqlQuery query;
    int nameCol = -1;
    bool sqlOk = false;
    //QDate date = QDate::currentDate();
    aux2.clear();
    aux.clear();
    _sats.clear();


    aux = "SELECT id, satarrlid, satname, uplink, downlink, satmode FROM satellites";

    sqlOk = query.exec(aux);
    if (sqlOk)
    {
        QSqlRecord rec = query.record();

        while ( (query.next()) && (query.isValid()) )
        {
            aux2.clear();

            nameCol = rec.indexOf("id");
            aux2 = (query.value(nameCol)).toString();

            nameCol = rec.indexOf("satarrlid");
            aux2 = aux2.append("-");
            aux2.append((query.value(nameCol)).toString());

            nameCol = rec.indexOf("satname");
             aux2 = aux2.append("-");
             aux2.append((query.value(nameCol)).toString());

             nameCol = rec.indexOf("uplink");
             aux2 = aux2.append("-");
             aux2.append((query.value(nameCol)).toString());

             nameCol = rec.indexOf("downlink");
             aux2 = aux2.append("-");
             aux2.append((query.value(nameCol)).toString());

             nameCol = rec.indexOf("satmode");
             aux2 = aux2.append("-");
             aux2.append((query.value(nameCol)).toString());

             _sats.append(aux2);

         }
         return _sats;
     }
     else
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
         return _sats;
     }


     //_sats.clear();
        //qDebug() << "SetupPageSats::readSats: " << QString::number(_sats.size())<< QT_ENDL;

     //return _sats;
}

int SetupPageSats::getSelectedSat()
{
     //qDebug() << "SetupPageSats::getSelectedSat: " << currentSats->currentText() << QT_ENDL;
    QString selectedSat = currentSats->currentText();
    int i = 0;
    QStringList qs;
    qs.clear();
    qs << selectedSat.split("-");
    i = (qs.at(0)).toInt();
    if (i>=1)
    {
        return i;
    }
    else
    {
        return 0;
    }
    //return 0;
}

void SetupPageSats::setSelectedSat(const int _i)
{
     //qDebug() << "SetupPageSats::SetupPageSats::setSelectedSat: " << QString::number(_i) << QT_ENDL;

    QString n = QString::number(_i) + "-";
    int selected = currentSats->findText(n, Qt::MatchStartsWith);
    if (selected >= 0)
    {
         //qDebug() << "SetupPageSats::SetupPageSats::setSelectedSat selected>0: " << QString::number(selected) << QT_ENDL;
        currentSats->setCurrentIndex(selected);
    }
    else
    {
          //qDebug() << "SetupPageSats::SetupPageSats::setSelectedSat not selected" << QT_ENDL;
        return;
    }
}

//void SetupPageSats::readSelectedSat(const int _i)
//{

//}

void SetupPageSats::showError(const QString &_errorC)
{
    QString text = QString(tr("An error has occurred showing the following error code:") + "\n'%1'").arg(_errorC);


    QMessageBox::warning(this, tr("KLog - SetupPageSats"),
                                   text,
                                   QMessageBox::Ok);

}

void SetupPageSats::slotImportButtonClicked()
{
     //qDebug() << "SetupPageSats::slotImportButtonClicked" << QT_ENDL;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Satellites File"),
                                                     util->getHomeDir(),
                                                     "SATS (*.dat)");
    if (fileName.isNull())
    {
        return;
    }

    UpdateSatsData *updateSat = new UpdateSatsData(dataProxy, this);
    if (updateSat->satDataFileRead(fileName))
    {

         //qDebug() << "SetupPageSats::slotImportButtonClicked IMPORTED OK" << QT_ENDL;
    }
    else
    {
         //qDebug() << "SetupPageSats::slotImportButtonClicked IMPORTED NOK" << QT_ENDL;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("KLog warning"));
        QString aux = QString();
        aux = tr("An unexpected error ocurred while importing the satellite data.\n\nIt may be caused because the file you are trying to import does not have the right format.");
        //msgBox.setText(aux + "MW-1#" + QString::number(errorCode));
        msgBox.setText(aux);
        msgBox.setDetailedText(tr("Please check the format or contact the developer for analysis with the error code: ") + "SetupPageSats error #1");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Ok:
            break;
            default:
            // should never be reached
            break;
        }

    }
    satsModel->select();
    updateSelectedSats();

}

void SetupPageSats::slotExportButtonClicked()
{
     //qDebug() << "SetupPageSats::slotExportButtonClicked" << QT_ENDL;

    int nameCol = -1;
    QString queryString, aux;
    QSqlQuery query;

    int satsN = -1;

    bool sqlOk = false;
    aux.clear();
    queryString = "SELECT COUNT (*) from satellites";
    sqlOk = query.exec(queryString);
    if (sqlOk)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                satsN = (query.value(0)).toInt();
                 //qDebug() << "SetupPageSats::slotExportButtonClicked: Sats: " << QString::number(satsN) << QT_ENDL;
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());

    }
    QDateTime *dateTime = new QDateTime();
    //dateTime->currentDateTime();

    queryString = "SELECT id, satarrlid, satname, uplink, downlink, satmode FROM satellites";
    sqlOk = query.exec(queryString);

    if (sqlOk)
    {

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Satellites File"),
                                   util->getHomeDir(),
                                   "SATS (*.dat)");

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
            return ;
        QTextStream out(&file);

        out << "<app_klog_data:4>SATS" << QT_ENDL;
        out << "<app_klog_sats_references:" << QString::number((QString::number(satsN)).length()) << ">" << QString::number(satsN) << QT_ENDL;
        out << "<app_klog_sats_version:8>" << (dateTime->currentDateTime().date()).toString("yyyyMMdd");
        out << "<EOH>" << QT_ENDL;
        QSqlRecord rec = query.record();

        while ( (query.next()) && (query.isValid()) )
        {
            nameCol = rec.indexOf("satarrlid");
            aux = (query.value(nameCol)).toString();
            QString satText = "<app_klog_sats_arrlid:" + QString::number(aux.length()) + ">" + aux + " ";

            nameCol = rec.indexOf("satname");
            aux = (query.value(nameCol)).toString();
            satText = satText + "<app_klog_sats_name:" + QString::number(aux.length()) + ">" + aux + " ";

            nameCol = rec.indexOf("uplink");
            aux = (query.value(nameCol)).toString();
            satText = satText + "<app_klog_sats_uplink:" + QString::number(aux.length()) + ">" + aux + " ";

            nameCol = rec.indexOf("downlink");
            aux = (query.value(nameCol)).toString();
            satText = satText + "<app_klog_sats_downlink:" + QString::number(aux.length()) + ">" + aux + " ";

            nameCol = rec.indexOf("satmode");
            aux = (query.value(nameCol)).toString();
            satText = satText + "<app_klog_sats_mode:" + QString::number(aux.length()) + ">" + aux + " " + "<eor>";

            out << satText << QT_ENDL;
            //satText.clear();
        }

    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }

     //qDebug() << "SetupPageSats::slotExportButtonClicked END" << QT_ENDL;
}
