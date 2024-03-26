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
  //qDebug() << "DXCCStatusWidget::DXCCStatusWidget from: " << _parentFunction;
#else
#endif


    dataProxy = dp;
    locator = new Locator();
    awards = new Awards(dataProxy, Q_FUNC_INFO);
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-1";
    world = new World(dataProxy,Q_FUNC_INFO);
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-2";

    dxccView = new QTableWidget;
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-3";
    dxccView->setMouseTracking(true);
    dxccView->setSortingEnabled(true);
    numberOfColumns = 0;
    logNumber = -1; // -1 means that ALL the logs will be used (if showAllLogsButton is not checked)
    tempLog = -1;   // -1 means that ALL the logs will be used
    loc = QString();
    refreshButton = new QPushButton;

    bandNames.clear();
    validBands.clear();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-4";
    //setDefaultBands();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-5";
    createUI();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget - END";
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
      //qDebug() << "DXCCStatusWidget::createUI ";

    // We remove the vertical header
    hv = dxccView->verticalHeader();
    hv->hide();
    hv->setStretchLastSection(true);
    hh = dxccView->horizontalHeader();
      //qDebug() << "DXCCStatusWidget::createUI-10";
    refreshButton->setText(tr("Update"));

    //qDebug() << "DXCCStatusWidget::createUI-11";

    dxccView->setContextMenuPolicy(Qt::CustomContextMenu);
    //qDebug << "DXCCStatusWidget::createUI-12 - setting column count: " << QString::number(numberOfColumns);
    dxccView->setColumnCount(numberOfColumns);
    //qDebug() << "DXCCStatusWidget::createUI-13";
    dxccView->horizontalHeader()->setStretchLastSection(true);
    //qDebug() << "DXCCStatusWidget::createUI-14";
    dxccView->setRowCount(0);
      //qDebug() << "DXCCStatusWidget::createUI-20";
    QHBoxLayout *bottonLineLayout = new QHBoxLayout;
    bottonLineLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    bottonLineLayout->addWidget(refreshButton);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(dxccView);
    tabLayout->addLayout(bottonLineLayout);
    setLayout(tabLayout);
      //qDebug() << "DXCCStatusWidget::createUI-30";
    dxccView->resizeColumnsToContents();
    dxccView->resizeRowsToContents();
      //qDebug() << "DXCCStatusWidget::createUI-90";
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(slotRefreshButtonClicked() ) );
      //qDebug() << "DXCCStatusWidget::createUI-91";
    connect(dxccView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotItemDoubleClicked(QTableWidgetItem *) ) );
      //qDebug() << "DXCCStatusWidget::createUI-92";
    connect(dxccView, SIGNAL(itemEntered(QTableWidgetItem *)), this, SLOT(slotItemEntered(QTableWidgetItem *) ) );
    //connect(dxccView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRightButton( const QPoint& ) ) );
      //qDebug() << "DXCCStatusWidget::createUI-93";
    //dxccView->setSortingEnabled(false);
      //qDebug() << "DXCCStatusWidget::createUI-END";
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::update()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    //qDebug()  << Q_FUNC_INFO << ": " << QTime::currentTime().toString("HH:mm:ss");
    //int entities = world->getHowManyEntities();
    int entities = dataProxy->getMaxEntityID(false);
    if (!awards->updateDXCCBandsStatus (-1)) // We update all
    {
        //qDebug() << Q_FUNC_INFO << "awards->updateDXCCBandsStatus returned FALSE";
        // It may be the case that DXCC field in the table is not defined.
        // It should be updated.
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(tr("It seems that the DXCC status in your database is not updated and KLog can't find any dxcc information. You can try to fix this by updating the log."));
        msgBox.setInformativeText(tr("Do you want to update your DXCC status?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Yes:
                //qDebug() << Q_FUNC_INFO << " - emitting fillInQSOSignal()";
                emit fillInQSOSignal();
                return;
            break;
            case QMessageBox::No:
              // No was clicked
                return;
            break;
            default:
              // should never be reached
            break;
        }
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << "awards->updateDXCCBandsStatus returned TRUE";
    }

    //qDebug() << Q_FUNC_INFO << "AFTER awards->updateDXCCBandsStatus returned";
    QList<int> bandIds;
    bandIds.clear();
    QString aux;
    foreach (aux, bandNames)
    {
        bandIds.append (dataProxy->getIdFromBandName (aux));
    }

    //qDebug() << "DXCCStatusWidget::update: " << QString::number(entities) << " entities to update" << QTime::currentTime().toString("HH:mm:ss");
    QList<int> list;
    //qDebug() << "DXCCStatusWidget::update: -1"  << QTime::currentTime().toString("HH:mm:ss");
    list.clear();
    //qDebug() << "DXCCStatusWidget::update: -2"  << QTime::currentTime().toString("HH:mm:ss");
    //QString prefix;
    //qDebug() << "DXCCStatusWidget::update: -3"  << QTime::currentTime().toString("HH:mm:ss");
    if (dxccView->columnCount()>0)
    {
        dxccView->clearContents();

        //qDebug() << "DXCCStatusWidget::update pre FOR" << QTime::currentTime().toString("HH:mm:ss");
        for (int i=1; i<=entities; i++)
        {
            //qDebug() << "DXCCStatusWidget::update in FOR " << QString::number(i) << QTime::currentTime().toString("HH:mm:ss");
            list.clear();
            //if (aux.length()>2)  // Not all integers refers to an entity.
            //{
                list.append (i);
                list.append (bandIds);
                addEntity(list);
            //}
        }
    }
    emit debugLog (Q_FUNC_INFO, "END", Debug);
    //qDebug() << "DXCCStatusWidget::update END" << QTime::currentTime().toString("HH:mm:ss");
}

void DXCCStatusWidget::addEntity(const QList<int> &_ent)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    // _ent.at(0) = dxcc column of Entity Table (considering big numbers, like 2248 for IT9!)
    // _ent.at(1) until number of Columns are just the bandnames

    //qDebug() << "DXCCStatusWidget::addEntity: " << QString::number(_ent.at(0)) << "/" <<QString::number(_ent.at(1)) << " / " << QString::number(_ent.length()) << QTime::currentTime().toString("HH:mm:ss");
    // DXCC id, Entity Name, bandName1, bandName2, ...

    if (_ent.length() != numberOfColumns-1)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: in number of columns" << QString::number(_ent.length()) << "/" << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss");
        return;
    }

    int status = -1;

    //qDebug() << "DXCCStatusWidget::addEntity: ent = " << _ent << QTime::currentTime().toString("HH:mm:ss");
    int _dxcc = _ent.at(0);
    QStringList data;
    data.clear();
    data << dataProxy->getEntiNameISOAndPrefixFromId(_dxcc);

    if (data.length ()!=3)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: Wrong Entity Name and Prefix - END" << QTime::currentTime().toString("HH:mm:ss");
        return;
    }

    QString prefix = data.at(0);
    QString entName = data.at(1);
    QString isoName = data.at(2);

    if ((entName.length()<2) || (prefix.length ()<1) || (isoName.length ()<2))
    {
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: entname too short!" << QTime::currentTime().toString("HH:mm:ss");
        return;
    }

    QString flagSt;
    flagSt.clear();

    if (isoName.length()>1)
    {
        //flagSt = ":/" + isoName + ".png";
        flagSt = ":/flags/" + isoName + ".png";
    }
    else
    {
        flagSt.clear();
    }

    //flagSt = ":/flags/" + isoName + ".png";
    QIcon flagIcon(flagSt);

    //qDebug() << "DXCCStatusWidget::addEntity: Name: " << entName << QTime::currentTime().toString("HH:mm:ss");
    //qDebug() << "DXCCStatusWidget::addEntity: Prefix: " << prefix << QTime::currentTime().toString("HH:mm:ss");
    //qDebug() << "DXCCStatusWidget::addEntity: Flag: " << flagSt << QTime::currentTime().toString("HH:mm:ss");

    dxccView->insertRow(dxccView->rowCount());
    //qDebug() << "DXCCStatusWidget::addEntity: rowCount:  " << QString::number(dxccView->rowCount()) << QTime::currentTime().toString("HH:mm:ss");

    QTableWidgetItem *newItemID = new QTableWidgetItem(prefix);
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
        //qDebug() << "DXCCStatusWidget::addEntity: rowCount-2:  " << QString::number(dxccView->rowCount()) << "/" << QString::number(i) << " / " << newItem->text() << QTime::currentTime().toString("HH:mm:ss");
    }

    QTableWidgetItem *newItemPref = new QTableWidgetItem(prefix);
    newItemPref->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    newItemPref->setFlags(Qt::ItemIsEnabled);

    QTableWidgetItem *newItemName = new QTableWidgetItem(entName);
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
    //qDebug() << "DXCCStatusWidget::addEntity: END" << QTime::currentTime().toString("HH:mm:ss");
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::addEntity2(const QStringList &_ent)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    // _ent.at(0) = dxcc column of Entity Table (considering big numbers, like 2248 for IT9!)
    // _ent.at(1) until number of Columns are just the bandnames

    //qDebug() << Q_FUNC_INFO;
    // DXCC id, Entity Name, bandName1, bandName2, ...

    //QString au = "New Line ";
    //foreach (QString a, _ent)
    //{
    //   au = au + " - " + a;
    //}
    //qDebug() << Q_FUNC_INFO << ": " << au << " - " << QTime::currentTime().toString("HH:mm:ss");;

    if (_ent.length() != numberOfColumns-1)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: in number of columns" << QString::number(_ent.length()) << "/" << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss");
        return;
    }

    int status = -1;

    //qDebug() << "DXCCStatusWidget::addEntity: ent = " << _ent << QTime::currentTime().toString("HH:mm:ss");
    int _dxcc = _ent.at(0).toInt();
    QStringList data;
    data.clear();
    data << dataProxy->getEntiNameISOAndPrefixFromId(_dxcc);

    if (data.length ()!=3)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: Wrong Entity Name and Prefix - END" << QTime::currentTime().toString("HH:mm:ss");
        return;
    }
    //au = "data: ";
    //foreach (QString a, data)
    //{
    //    au = au + " - " + a;
    //}
    //qDebug() << Q_FUNC_INFO << ": " << au;
    QString prefix = data.at(0);
    QString entName = data.at(1);
    QString isoName = data.at(2);

    if ((entName.length()<2) || (prefix.length ()<1) || (isoName.length ()<2))
    {
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: entname too short!" << QTime::currentTime().toString("HH:mm:ss");
        return;
    }

    QString flagSt;
    flagSt.clear();

    if (isoName.length()>1)
    {
        flagSt = ":/" + isoName + ".png";
    }
    else
    {
        flagSt.clear();
    }

    flagSt = ":/flags/" + isoName + ".png";
    QIcon flagIcon(flagSt);

    //qDebug() << "DXCCStatusWidget::addEntity: Name: " << entName << QTime::currentTime().toString("HH:mm:ss");
    //qDebug() << "DXCCStatusWidget::addEntity: Prefix: " << prefix << QTime::currentTime().toString("HH:mm:ss");
    //qDebug() << "DXCCStatusWidget::addEntity: Flag: " << flagSt << QTime::currentTime().toString("HH:mm:ss");

    dxccView->insertRow(dxccView->rowCount());
    //qDebug() << "DXCCStatusWidget::addEntity: rowCount:  " << QString::number(dxccView->rowCount()) << QTime::currentTime().toString("HH:mm:ss");

    QTableWidgetItem *newItemID = new QTableWidgetItem(prefix);
    newItemID->setTextAlignment(Qt::AlignCenter);
    newItemID->setFlags(Qt::NoItemFlags);
    dxccView->setItem(dxccView->rowCount()-1, 0, newItemID);
    awards->updateDXCCBandsStatus (tempLog);
    for (int i=1; i < _ent.length(); i++)
    {
        //qDebug << Q_FUNC_INFO << ": " << prefix << " - i = " << QString::number(i) << "/" << _ent.at(i);
        int bandid = dataProxy->getIdFromBandName(_ent.at(i));
        QTableWidgetItem *newItem = new QTableWidgetItem(awards->getDXCCStatusBand(_dxcc, bandid));
        //QTableWidgetItem *newItem = new QTableWidgetItem(awards->getDXCCStatusBand2(_dxcc, bandid, tempLog));
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setFlags(Qt::ItemIsEnabled);

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
        //qDebug() << "DXCCStatusWidget::addEntity: rowCount-2:  " << QString::number(dxccView->rowCount()) << "/" << QString::number(i) << " / " << newItem->text() << QTime::currentTime().toString("HH:mm:ss");
    }

    QTableWidgetItem *newItemPref = new QTableWidgetItem(prefix);
    newItemPref->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    newItemPref->setFlags(Qt::ItemIsEnabled);

    QTableWidgetItem *newItemName = new QTableWidgetItem(entName);
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
    //qDebug() << "DXCCStatusWidget::addEntity: END" << QTime::currentTime().toString("HH:mm:ss");
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::setBands(const QString &_callingFunc, QStringList const &_ent, const bool _creating)
{// Receives the list of band names and defines the columns
    //qDebug() << Q_FUNC_INFO << "(" << _callingFunc << ")" << QTime::currentTime().toString("HH:mm:ss");
    Q_UNUSED(_callingFunc);
    //foreach(QString aux, _ent)
    //{
    //    //qDebug << Q_FUNC_INFO << ": " << aux;
    //}
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    QStringList qs;
    qs.clear();
    //qDebug() << "DXCCStatusWidget::setBands - 01" << QTime::currentTime().toString("HH:mm:ss");
    qs << dataProxy->sortBandNamesBottonUp(_ent);
    //qDebug() << "DXCCStatusWidget::setBands - 02: Lenght qs: " << QString::number(qs.size()) << QTime::currentTime().toString("HH:mm:ss");
    if (qs.length()<0)
    {
        //qDebug() << "DXCCStatusWidget::setBands no bands received here " << QTime::currentTime().toString("HH:mm:ss") << QTime::currentTime().toString("HH:mm:ss");
        return;
    }

    QString testBand;
    testBand.clear();

    bandNames.clear();
    //qDebug() << "DXCCStatusWidget::setBands - 03 " << QTime::currentTime().toString("HH:mm:ss");
    validBands.clear();
    validBands = dataProxy->getBandNames();
    //qDebug() << "DXCCStatusWidget::setBands - 04 " << QTime::currentTime().toString("HH:mm:ss");
    dxccView->clearSelection();
    dxccView->clear();

    for (int i = 0; i<qs.length(); i++)
    {
        //qDebug() << "DXCCStatusWidget::setBands-4.1: " << qs.at(i) <<" - " << QTime::currentTime().toString("HH:mm:ss");
        testBand = qs.at(i);

        if (validBands.contains(qs.at(i)))
        {
            bandNames.append(testBand);
            //qDebug() << "DXCCStatusWidget::setBands-4.2: Added: " << bandNames.last()  << " - " << QTime::currentTime().toString("HH:mm:ss");
        }
        //else
        //{
            //qDebug() << "DXCCStatusWidget::setBands -4.3 Not valid band: " << testBand << QTime::currentTime().toString("HH:mm:ss");
        //}
    }
    dxccView->setColumnCount(0);
    numberOfColumns = dxccView->columnCount();

    while(dxccView->columnCount()>0)
    {
        //qDebug() << "DXCCStatusWidget::setBands: Still pending: " << QString::number(dxccView->columnCount()) << QTime::currentTime().toString("HH:mm:ss");
        dxccView->removeColumn(0);
    }
    numberOfColumns = dxccView->columnCount();
    //qDebug() << "DXCCStatusWidget::setBands: -6 All removed: " << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss");
    numberOfColumns = 2 + bandNames.length(); //Prefix, Entity Name
    //qDebug() << "DXCCStatusWidget::setBands - 7.1 - columnCount: " << QString::number(dxccView->columnCount()) << QTime::currentTime().toString("HH:mm:ss");
    //qDebug() << "DXCCStatusWidget::setBands - 7.1 - numberOfColumns: " << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss");
    dxccView->setColumnCount(numberOfColumns);
   //qDebug() << "DXCCStatusWidget::setBands - 7.2 " << QTime::currentTime().toString("HH:mm:ss");
    dxccView->setRowCount(0);
    //qDebug() << "DXCCStatusWidget::setBands - 7.3 " << QTime::currentTime().toString("HH:mm:ss");

    QStringList headerqs;
      //qDebug() << "DXCCStatusWidget::setBands - 7.4 " << QTime::currentTime()).toString("HH:mm:ss");
    headerqs.clear();
    //qDebug() << "DXCCStatusWidget::setBands - 7.5 " << QTime::currentTime().toString("HH:mm:ss");
    headerqs << tr("Prefix") << tr("Entity") << bandNames;
    //qDebug() << "DXCCStatusWidget::setBands - 8 " << QTime::currentTime().toString("HH:mm:ss");
    dxccView->setHorizontalHeaderLabels(headerqs);

    //qDebug() << "DXCCStatusWidget::setBands - 9 PRE-END" << QTime::currentTime().toString("HH:mm:ss");

    if (_creating)
    {
        //qDebug() << "DXCCStatusWidget::setBands: 9.1 !_creating so updating!" << QTime::currentTime().toString("HH:mm:ss");
        update();
    }
    //qDebug() << "DXCCStatusWidget::setBands: END" << QTime::currentTime().toString("HH:mm:ss");
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
}

void DXCCStatusWidget::setDefaultBands()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
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
    //qDebug << Q_FUNC_INFO << " - END";
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::refresh()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::refresh";
    //update();
    slotRefreshButtonClicked();
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

 void DXCCStatusWidget::setCurrentLog(const int _logN)
 {
     emit debugLog (Q_FUNC_INFO, "Start", Debug);
       //qDebug() << "DXCCStatusWidget::setCurrentLog: " << QString::number(_logN);
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
    //qDebug << "DXCCStatusWidget::slotItemEntered: " << ((item->data(0)).toString()).simplified();
    //qDebug << "DXCCStatusWidget::slotItemEntered: " << item->text();
    //qDebug << "DXCCStatusWidget::slotItemEntered: ";
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

           //qDebug() << "DXCCStatusWidget::slotItemEntered: row: " << QString::number(row);
           //qDebug() << "DXCCStatusWidget::slotItemEntered: hrow: " <<  dxccView->verticalHeaderItem(row)->text();
      //  row = (dxccView->item(row,0)->text()).toInt();

        int entiID = dataProxy->getEntityIdFromMainPrefix(dxccView->item(dxccView->row(item),0)->text());
        if (entiID<0)
        { // Some prefix are shown with a * in the begining that is removed to show the real prefix
            entiID = dataProxy->getEntityIdFromMainPrefix("*" + dxccView->item(dxccView->row(item),0)->text());
        }
          //qDebug() << "DXCCStatusWidget::slotItemEntered: entiID: " << QString::number(entiID);

        if ((column == 0) || (column == 1))
        {
            //tip = tr("Pref: ") + (dxccView->item(row, 1))->text() + " - ";
            tip = tr("Pref: ") + dataProxy->getEntityMainPrefix(entiID) + " - ";
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
                   //qDebug() << "DXCCStatusWidget::slotItemEntered: band: " << band;
                int bandi = dataProxy->getIdFromBandName(band);
                   //qDebug() << "DXCCStatusWidget::slotItemEntered: band: " << QString::number(bandi);
                if (item->text() == "C")
                {
                    tip = awards->getQSOofAward(entiID, bandi, logNumber, true);
                }
                else
                {
                    tip = awards->getQSOofAward(entiID, bandi, logNumber, false);
                }

                   //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip;
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
           //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip;
    }
      //qDebug() << "DXCCStatusWidget::slotItemEntered: END ";
    emit debugLog (Q_FUNC_INFO, "END", Debug);
 }

void DXCCStatusWidget::slotItemDoubleClicked(QTableWidgetItem  * item )
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << ((item->data(0)).toString()).simplified();
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << item->text();
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked - start";
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
      //qDebug() << "DXCCStatusWidget::setMyLocator: " << _loc;
    QString l = _loc;
    if ( locator->isValidLocator(l.toUpper()) )
    {
        loc = l.toUpper();
    }
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
