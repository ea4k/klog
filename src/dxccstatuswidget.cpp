/***************************************************************************
                          dxccstatuswidget.cpp  -  description
                             -------------------
    begin                : feb 2016
    copyright            : (C) 2016 by Jaime Robles
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
#include "dxccstatuswidget.h"
#include <type_traits>  // Include this header to use std::as_const
//#include <QDebug>

/*

TODO: Adjust the header columns
TODO: Add some color to the texts depending on C, W or -
TODO: Call the creation of this depending on the bands that the user is using

*/

DXCCStatusWidget::DXCCStatusWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent) : QWidget(parent)
{
    Q_UNUSED(_parentFunction);
#ifdef QT_DEBUG
  //qDebug() << Q_FUNC_INFO << " -  from: " << _parentFunction;
#else
#endif
    qDebug() << Q_FUNC_INFO << " - Start";
    dataProxy = dp;
    locator = new Locator();
    awards = new Awards(dataProxy, Q_FUNC_INFO);
      //qDebug() << Q_FUNC_INFO << " - -1";

    world = new World(dataProxy,Q_FUNC_INFO);
      //qDebug() << Q_FUNC_INFO << " - -2";

    dxccView = new QTableWidget;
      //qDebug() << Q_FUNC_INFO << " - -3";
    dxccView->setMouseTracking(true);
    dxccView->setSortingEnabled(true);
    numberOfColumns = 0;
    logNumber = -1; // -1 means that ALL the logs will be used (if showAllLogsButton is not checked)
    tempLog = -1;   // -1 means that ALL the logs will be used
    loc = QString();
    refreshButton = new QPushButton;

    bandNames.clear();
    //validBands.clear();
      //qDebug() << Q_FUNC_INFO << " - -4";
    //setDefaultBands();
      //qDebug() << Q_FUNC_INFO << " - -5";
    createUI();
      //qDebug() << Q_FUNC_INFO << " -  - END";
}

DXCCStatusWidget::~DXCCStatusWidget()
{
    delete(locator);
    delete(world);
    delete(awards);
}

void DXCCStatusWidget::createUI()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - Start";
    // We remove the vertical header
    hv = dxccView->verticalHeader();
    hv->hide();
    hv->setStretchLastSection(true);
    hh = dxccView->horizontalHeader();
      //qDebug() << Q_FUNC_INFO << " - 10";
    refreshButton->setText(tr("Update"));

    //qDebug() << Q_FUNC_INFO << " - 11";

    dxccView->setContextMenuPolicy(Qt::CustomContextMenu);
    //qDebug << Q_FUNC_INFO << " - 12 - setting column count: " << QString::number(numberOfColumns);
    dxccView->setColumnCount(numberOfColumns);
    //qDebug() << Q_FUNC_INFO << " - 13";
    dxccView->horizontalHeader()->setStretchLastSection(true);
    //qDebug() << Q_FUNC_INFO << " - 14";
    dxccView->setRowCount(0);
      //qDebug() << Q_FUNC_INFO << " - 20";
    QHBoxLayout *bottonLineLayout = new QHBoxLayout;
    bottonLineLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    bottonLineLayout->addWidget(refreshButton);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(dxccView);
    tabLayout->addLayout(bottonLineLayout);
    setLayout(tabLayout);
      //qDebug() << Q_FUNC_INFO << " - 30";
    dxccView->resizeColumnsToContents();
    dxccView->resizeRowsToContents();
      //qDebug() << Q_FUNC_INFO << " - 90";
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(slotRefreshButtonClicked() ) );
      //qDebug() << Q_FUNC_INFO << " - 91";
    connect(dxccView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotItemDoubleClicked(QTableWidgetItem *) ) );
      //qDebug() << Q_FUNC_INFO << " - 92";
    connect(dxccView, SIGNAL(itemEntered(QTableWidgetItem *)), this, SLOT(slotItemEntered(QTableWidgetItem *) ) );
    //connect(dxccView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRightButton( const QPoint& ) ) );
      //qDebug() << Q_FUNC_INFO << " - 93";
    //dxccView->setSortingEnabled(false);
      //qDebug() << Q_FUNC_INFO << " - END";
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::handleDXCCStatusUpdateFailure()
{
    qDebug() << Q_FUNC_INFO << " - Start";
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("It seems that the DXCC status in your database is not updated and KLog can't find any dxcc information. You can try to fix this by updating the log."));
    msgBox.setInformativeText(tr("Do you want to update your DXCC status?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Yes:
          //qDebug() << Q_FUNC_INFO << " - emitting fillInQSOSignal()";
            emit fillInQSOSignal();
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }
}

void DXCCStatusWidget::processEntities(int entities)
{       // entities just has the number of entities (aprox 350)
    qDebug() << Q_FUNC_INFO << " - Start";
    QList<int> bandIds = getBandIds();

    if (dxccView->columnCount() > 0) //{
        dxccView->clearContents();
        //qDebug() << Q_FUNC_INFO << "pre FOR" << QTime::currentTime().toString("HH:mm:ss");
        // Now we create a list where the first number is the entity and the rest are bands.
    for (int i = 1; i <= entities; i++) {
            QList<int> list = { i };
            list.append(bandIds);
            addEntity(list);
    }
   // }
}

QList<int> DXCCStatusWidget::getBandIds()
{
    qDebug() << Q_FUNC_INFO << " - Start";
    QList<int> bandIds;
    //for (const QString &bandName : qAsConst(bandNames)) {
    for (const QString &bandName : std::as_const(bandNames)) {
        bandIds.append(dataProxy->getIdFromBandName(bandName));
    }
    return bandIds;
}

void DXCCStatusWidget::update()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - Start";
    int entities = dataProxy->getMaxEntityID(false);
    if (!awards->updateDXCCBandsStatus (-1)) // We update all
    {
        //qDebug() << Q_FUNC_INFO << "awards->updateDXCCBandsStatus returned FALSE";
        // It may be the case that DXCC field in the table is not defined.
        // It should be updated.
        handleDXCCStatusUpdateFailure();
        return;
    }
  //qDebug() << Q_FUNC_INFO << "awards->updateDXCCBandsStatus returned TRUE";

    processEntities(entities);

  //qDebug() << Q_FUNC_INFO << "AFTER awards->updateDXCCBandsStatus returned";

    emit debugLog (Q_FUNC_INFO, "END", Debug);
  //qDebug() << "DXCCStatusWidget::update END" << QTime::currentTime().toString("HH:mm:ss");
}


void DXCCStatusWidget::addEntity(const QList<int> &_ent)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << " - Start";
    // _ent.at(0) = dxcc column of Entity Table (considering big numbers, like 2248 for IT9!)
    // _ent.at(1) until number of Columns are just the bandnames
    //qDebug() << Q_FUNC_INFO << QString::number(_ent.at(0)) << "/" <<QString::number(_ent.at(1)) << " / " << QString::number(_ent.length()) << QTime::currentTime().toString("HH:mm:ss");
    // DXCC id, Entity Name, bandName1, bandName2, ...

    if (_ent.length() != numberOfColumns-1)
    {
        //qDebug() << Q_FUNC_INFO << "  ERROR: in number of columns" << QString::number(_ent.length()) << "/" << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss");
        return;
    }

    int status = -1;


    int _dxcc = _ent.at(0);
    //qDebug() << Q_FUNC_INFO << " Processing Entity: " << QString::number(_dxcc);

    EntityData entity = world->getEntityDataFromDXCC(_dxcc);
    if (entity.name.length()<3)
        return;

    QString flagSt;
    flagSt.clear();
    if (entity.isoname.length()>1)
    {
        //flagSt = ":/" + isoName + ".png";
        flagSt = ":/flags/" + entity.isoname + ".png";
    }
    else
    {
        flagSt.clear();
    }

    QIcon flagIcon(flagSt);

    dxccView->insertRow(dxccView->rowCount());

    QTableWidgetItem *newItemID = new QTableWidgetItem(entity.mainprefix);
    newItemID->setTextAlignment(Qt::AlignCenter);
    newItemID->setFlags(Qt::NoItemFlags);
    dxccView->setItem(dxccView->rowCount()-1, 0, newItemID);

    for (int i=1; i < _ent.length(); i++)
    {
        //qDebug() << Q_FUNC_INFO << ": " << prefix << " - i = " << QString::number(i) << "/" << _ent.at(i);
        int bandid = _ent.at(i);
        QTableWidgetItem *newItem = new QTableWidgetItem(awards->getDXCCStatusBand(_dxcc, bandid));
        //QTableWidgetItem *newItem = new QTableWidgetItem(awards->getDXCCStatusBand2(_dxcc, bandid, tempLog));
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setFlags(Qt::ItemIsEnabled);
        //qDebug() << Q_FUNC_INFO << ": C/W/- : " << newItem->text();
        if (newItem->text()=="C")
        {
            newItem->setForeground (QBrush(Qt::blue));
                    //newItem->setTextColor(Qt::blue);
            newItem->setBackground(QBrush(Qt::green));
            status = 1;
        }
        else if (newItem->text()=="W")
        {
            if (status < 0)
            {
                status = 0;
            }
            newItem->setForeground (QBrush(Qt::red));
            newItem->setBackground(QBrush(Qt::yellow));
        }

        dxccView->setItem(dxccView->rowCount()-1, i+1, newItem);
        //qDebug() << Q_FUNC_INFO << "  rowCount-2:  " << QString::number(dxccView->rowCount()) << "/" << QString::number(i) << " / " << newItem->text() << QTime::currentTime().toString("HH:mm:ss");
    }

    QTableWidgetItem *newItemPref = new QTableWidgetItem(entity.mainprefix);
    newItemPref->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    newItemPref->setFlags(Qt::ItemIsEnabled);

    QTableWidgetItem *newItemName = new QTableWidgetItem(entity.name);
    newItemName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    newItemName->setFlags(Qt::ItemIsEnabled);
    //newItemName->setFlags(Qt::ItemIsUserCheckable);
    newItemName->setIcon(flagIcon);
    if (status == 1)
    {
        newItemName->setForeground (QBrush(Qt::blue));
        newItemName->setBackground(QBrush(Qt::green));
    }
    else if (status == 0)
    {
        newItemName->setForeground(QBrush(Qt::darkRed));
        newItemPref->setForeground(QBrush(Qt::darkRed));
    }
    else
    {
        newItemName->setForeground(QBrush(Qt::red));
        newItemPref->setForeground(QBrush(Qt::red));
    }
    dxccView->setItem(dxccView->rowCount()-1, 0, newItemPref);
    dxccView->setItem(dxccView->rowCount()-1, 1, newItemName);
    //qDebug() << Q_FUNC_INFO << "  END" << QTime::currentTime().toString("HH:mm:ss");
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::setBands(const QString &_callingFunc, const QStringList &_listOfNewBands, bool _creating)
{
    qDebug() << Q_FUNC_INFO << " - Start";
    Q_UNUSED(_callingFunc);
    emit debugLog(Q_FUNC_INFO, "Start", Debug);
Posiblemente haya que repensar esta funcion que debería tocar solo las bandas... pero borra y pone bandas

    if (_listOfNewBands.isEmpty())
    {
        return;
    }

    QStringList sortedBands = dataProxy->sortBandNamesBottonUp(_listOfNewBands);

    if (sortedBands.isEmpty())
    {
        emit debugLog(Q_FUNC_INFO, "No bands received", Debug);
        return;
    }

    Utilities util(Q_FUNC_INFO);
    if (util.areThoseListsTheSame(bandNames, _listOfNewBands))
    {
        emit debugLog(Q_FUNC_INFO, "Bands are the same", Debug);
        return;
    }

    bandNames.clear();
    QStringList allValidBands = dataProxy->getBandNames();

    foreach (const QString &band, sortedBands)
    {
        if (allValidBands.contains(band))
        {
            bandNames.append(band);
        }
    }

    resetDXCCView();

    numberOfColumns = 2 + bandNames.length();
    dxccView->setColumnCount(numberOfColumns);
    dxccView->setRowCount(0);

    QStringList headers;
    headers << tr("Prefix") << tr("Entity") << bandNames;
    dxccView->setHorizontalHeaderLabels(headers);

    if (_creating)
    {
        update();
    }

    emit debugLog(Q_FUNC_INFO, "End", Debug);
}

void DXCCStatusWidget::resetDXCCView()
{
    qDebug() << Q_FUNC_INFO << " - Start";
    dxccView->clearSelection();
    dxccView->clear();

    while (dxccView->columnCount() > 0) {
        dxccView->removeColumn(0);
    }
}

void DXCCStatusWidget::setDefaultBands()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - Start";
      //qDebug() << "DXCCStatusWidget::setDefaultBands";
    /*
     Default bands:
     160M    80M  40M  30M  20M  17M  15M  12M  10M  6M   4M   2M   70CM
     28     27  25  24  23  22  21  12  19  18  17  16  14
     */

    bandNames.clear();
    //bandNames << "160M" << "80M" << "40M" << "30M" << "20M" << "17M" << "15M" << "12M" << "10M" << "6M" << "4M" << "2M" << "70CM";
    bandNames << "160M" << "80M" << "40M" << "30M" << "20M" << "17M" << "15M" << "12M" << "10M";

    setBands(Q_FUNC_INFO, bandNames, false);
    //qDebug << "DXCCStatusWidget::setDefaultBands - END";
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::slotRefreshButtonClicked()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - Start";

  //qDebug() << Q_FUNC_INFO;
    //TODO: Define a way to show the status of the selected log or all the logs in the DB
    if (dxccView->rowCount()<1)
    {
      //qDebug() << Q_FUNC_INFO << " - rowcount <1";
        return;
    }
    QStringList _bands = bandNames;
    setBands(Q_FUNC_INFO, _bands, true);

    //emit updateAwards();
    //update();
  //qDebug() << Q_FUNC_INFO << " - END";
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::refresh()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - Start";
    //update();
    slotRefreshButtonClicked();
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

 void DXCCStatusWidget::setCurrentLog(const int _logN)
 {
     emit debugLog (Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - " << QString::number(_logN);
     if (dataProxy->doesThisLogExist(_logN))
     {
         logNumber = _logN;
     }
     else
     {
         logNumber = -1;
     }
     emit debugLog (Q_FUNC_INFO, "END", Debug);
 }

 void DXCCStatusWidget::slotItemEntered(QTableWidgetItem  * item )
 {
    //qDebug << Q_FUNC_INFO << " - " << ((item->data(0)).toString()).simplified();
    //qDebug << Q_FUNC_INFO << " - " << item->text();
    qDebug() << Q_FUNC_INFO << " - Start";
     emit debugLog (Q_FUNC_INFO, "Start", Debug);
    if (dxccView->rowCount()<1)
    {
        return;
    }

    QString tip;
    tip.clear();
    //int row = -1;

    //int bandi = -1;
    //QString band = QString();


    if (item)
    {
      //  row = dxccView->row(item);
        int column = dxccView->column(item);

           //qDebug() << Q_FUNC_INFO << " - row: " << QString::number(row);
           //qDebug() << Q_FUNC_INFO << " - hrow: " <<  dxccView->verticalHeaderItem(row)->text();
      //  row = (dxccView->item(row,0)->text()).toInt();

        int entiID = dataProxy->getEntityIdFromMainPrefix(dxccView->item(dxccView->row(item),0)->text());
        if (entiID<0)
        { // Some prefix are shown with a * in the begining that is removed to show the real prefix
            entiID = dataProxy->getEntityIdFromMainPrefix("*" + dxccView->item(dxccView->row(item),0)->text());
        }
          //qDebug() << Q_FUNC_INFO << " - entiID: " << QString::number(entiID);

        if ((column == 0) || (column == 1))
        {
            //tip = tr("Pref: ") + (dxccView->item(row, 1))->text() + " - ";
            tip = tr("Pref: ") + world->getEntityMainPrefix(entiID) + " - ";
            tip = tip + tr("CQ: ") + QString::number(dataProxy->getCQzFromEntity(entiID)) + " - ";
            tip = tip + tr("ITU: ") + QString::number(dataProxy->getITUzFromEntity(entiID)) + " - ";
            QString dxLoc = locator->getLocator(dataProxy->getLongitudeFromEntity(entiID), dataProxy->getLatitudeFromEntity(entiID)) ;
            tip = tip + tr("Beam: ") + QString::number(locator->getBeamBetweenLocators(loc, dxLoc));
        }

        else if (column >= 2)
        {
            if (item->text() == "-")
            {
                tip = tr("Entity not worked in this band.");
            }
            else
            {
                QString band = dxccView->horizontalHeaderItem(column)->text();
                   //qDebug() << Q_FUNC_INFO << " - band: " << band;
                int bandi = dataProxy->getIdFromBandName(band);
                   //qDebug() << Q_FUNC_INFO << " - band: " << QString::number(bandi);
                if (item->text() == "C")
                {
                    tip = awards->getQSOofAward(entiID, bandi, logNumber, true);
                }
                else
                {
                    tip = awards->getQSOofAward(entiID, bandi, logNumber, false);
                }

                   //qDebug() << Q_FUNC_INFO << " - tip: " << tip;
                if (tip.length()>3)
                {
                    tip = "QSO: " + tip;
                }
                else
                {
                    //tip = tr("Text TBD.");
                    tip = tr("");
                }
            }
        }
        else
        {
             //tip = tr("ADD: Pref: Bearing, CQ & ITU.");
            tip = ""; // TODO define a text to be shown when no band column is selected (maybe Continent and bearing or a link to wikipedia or whatever!)
        }

        item->setToolTip(tip);
           //qDebug() << Q_FUNC_INFO << " - tip: " << tip;
    }
      //qDebug() << Q_FUNC_INFO << " - END ";
    emit debugLog (Q_FUNC_INFO, "END", Debug);
 }

void DXCCStatusWidget::slotItemDoubleClicked(QTableWidgetItem  * item )
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << ((item->data(0)).toString()).simplified();
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << item->text();
    qDebug() << Q_FUNC_INFO << " - Start";
    if (dxccView->rowCount()<1)
    {
        return;
    }
    int row = -1;
    //int column = -1;

    //int bandi = -1;
    int columns = dxccView->columnCount();
    int _entiNumb = -1; //Read the Entity number that the user is selecting.
    //QString band = QString();

    QList<int> qsos;
    qsos.clear();

    QTableWidgetItem * it = new QTableWidgetItem(0);

    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - Columns: " << QString::number(columns) ;

    if (item)
    {
        row = dxccView->row(item);
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - row: "  << QString::number(row);

        _entiNumb= dataProxy->getEntityIdFromMainPrefix(dxccView->item(dxccView->row(item),0)->text());
        if (_entiNumb<0)
        { // Some prefix are shown with a * in the begining that is removed to show the real prefix
            _entiNumb= dataProxy->getEntityIdFromMainPrefix("*" + dxccView->item(dxccView->row(item),0)->text());
        }
       //_entiNumb = ((dxccView->item(row,0))->text()).toInt();
          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - EntityNumber: " << QString::number(_entiNumb);

       for (int i = 2; i < columns; ++i)
       {
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column: "  << QString::number(i);
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column header: " << (dxccView->horizontalHeaderItem(i))->text();
            //entityName = (dxccView->item(row,i))->text() ;
              //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - item: " ;

            it->setText(dxccView->item(row,i)->text());
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column-txt: "  << it->text();

            QString band = dxccView->horizontalHeaderItem(i)->text();
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << band;
            int bandi = dataProxy->getIdFromBandName(band);
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << QString::number(bandi);
            int qsoId = -1;
          if (item->text() == "C")
          {
            qsoId = awards->getQSOIdofAward(_entiNumb, bandi, logNumber, true);
          }
          else
          {
            qsoId = awards->getQSOIdofAward(_entiNumb, bandi, logNumber, false);
          }

          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: QSOid: " << QString::number(qsoId);
          if (qsoId>0)
          {
              qsos.append(qsoId);
          }
        }
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - END of for" ;
        if (qsos.length()>0)
        {
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - qsos.length = " << QString::number(qsos.length()) ;
            emit showQsos(qsos);
        }
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - END of column == 1" ;
    }
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: END ";
}

void DXCCStatusWidget::setMyLocator(const QString &_loc)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - " << _loc;
    QString l = _loc;
    if ( locator->isValidLocator(l.toUpper()) )
    {
        loc = l.toUpper();
    }
}


void DXCCStatusWidget::setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default)
{
  //qDebug() << Q_FUNC_INFO << ": " << _newOne.name(QColor::HexRgb) << "/" << _needed.name(QColor::HexRgb) << "/" << _worked.name(QColor::HexRgb) << "/" << _confirmed.name(QColor::HexRgb) << "/" << _default.name(QColor::HexRgb);
    // Just to pass the colors to the awards class
    qDebug() << Q_FUNC_INFO << " - Start";
    awards->setColors(_newOne,  _needed, _worked,  _confirmed, _default);
}

/*
void DXCCStatusWidget::slotRightButton(const QPoint& pos)
{
       //qDebug() << "DXCCStatusWidget::slotRighButton" ;
    int row = (dxccView->indexAt(pos)).row();

    showDXCCWikipediaAct = new QAction(tr("&Wikipedia"), this);
    showDXCCWikipediaAct->setShortcut(Qt::CTRL | Qt::Key_W);
    showDXCCWikipediaAct->setStatusTip(tr("Show this DXCC in Wikipedia"));
    connect(showDXCCWikipediaAct, SIGNAL(triggered()), this, SLOT(slotWikipedia()));

    righButtonFromLogMenu(row);

}

void DXCCStatusWidget::slotWikipedia()
{
       //qDebug() << "DXCCStatusWidget::slotWikipedia: " << (showDXCCWikipediaAct->data()).toString();
    //TODO: To be added to the DXCCStatusWidget and create an action that emist the QSO id
    //int QSOid = ((logModel->index((showDXCCWikipediaAct->data()).toInt(), 0)).data(0)).toInt();

      //qDebug() << "DXCCStatusWidget::slotQsoDeleteFromLog (id): " << QString::number(QSOid);

}

void DXCCStatusWidget::righButtonFromLogMenu(const int trow)
{
        //qDebug() << "DXCCStatusWidget::slotshowRighButtonFromLogMenu:  " << QString::number(trow);

    //int _qsoID = ((logModel->index(trow, 0)).data(0)).toInt();
       //qDebug() << "DXCCStatusWidget::slotshowRighButtonFromLogMenu:  QSOid: " << QString::number(_qsoID);

    QMenu menu(this);

    menu.addAction(showDXCCWikipediaAct);
    showDXCCWikipediaAct->setData(trow);


    menu.exec(QCursor::pos());
}
*/
