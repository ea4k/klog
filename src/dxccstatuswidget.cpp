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

#ifdef QT_DEBUG
  //qDebug() << "DXCCStatusWidget::DXCCStatusWidget from: " << _parentFunction << QT_ENDL;
#else
#endif


    dataProxy = dp;
    locator = new Locator();
    awards = new Awards(dataProxy, Q_FUNC_INFO);
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-1" << QT_ENDL;
    world = new World(dataProxy,Q_FUNC_INFO);
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-2" << QT_ENDL;

    dxccView = new QTableWidget;
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-3" << QT_ENDL;
    dxccView->setMouseTracking(true);
    dxccView->setSortingEnabled(true);
    numberOfColumns = 0;
    logNumber = -1; // -1 means that ALL the logs will be used (if showAllLogsButton is not checked)
    tempLog = -1;   // -1 means that ALL the logs will be used
    loc = QString();
    refreshButton = new QPushButton;

    bandNames.clear();
    validBands.clear();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-4" << QT_ENDL;
    //setDefaultBands();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-5" << QT_ENDL;
    createUI();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget - END" << QT_ENDL;
}

DXCCStatusWidget::~DXCCStatusWidget(){}

void DXCCStatusWidget::createUI()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::createUI " << QT_ENDL;

    // We remove the vertical header
    hv = dxccView->verticalHeader();
    hv->hide();
    hv->setStretchLastSection(true);
    hh = dxccView->horizontalHeader();
      //qDebug() << "DXCCStatusWidget::createUI-10" << QT_ENDL;
    refreshButton->setText(tr("Update"));

    //qDebug() << "DXCCStatusWidget::createUI-11" << QT_ENDL;

    dxccView->setContextMenuPolicy(Qt::CustomContextMenu);
    //qDebug << "DXCCStatusWidget::createUI-12 - setting column count: " << QString::number(numberOfColumns) << QT_ENDL;
    dxccView->setColumnCount(numberOfColumns);
    //qDebug() << "DXCCStatusWidget::createUI-13" << QT_ENDL;
    dxccView->horizontalHeader()->setStretchLastSection(true);
    //qDebug() << "DXCCStatusWidget::createUI-14" << QT_ENDL;
    dxccView->setRowCount(0);
      //qDebug() << "DXCCStatusWidget::createUI-20" << QT_ENDL;
    QHBoxLayout *bottonLineLayout = new QHBoxLayout;
    bottonLineLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    bottonLineLayout->addWidget(refreshButton);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(dxccView);
    tabLayout->addLayout(bottonLineLayout);
    setLayout(tabLayout);
      //qDebug() << "DXCCStatusWidget::createUI-30" << QT_ENDL;
    dxccView->resizeColumnsToContents();
    dxccView->resizeRowsToContents();
      //qDebug() << "DXCCStatusWidget::createUI-90" << QT_ENDL;
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(slotRefreshButtonClicked() ) );
      //qDebug() << "DXCCStatusWidget::createUI-91" << QT_ENDL;
    connect(dxccView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotItemDoubleClicked(QTableWidgetItem *) ) );
      //qDebug() << "DXCCStatusWidget::createUI-92" << QT_ENDL;
    connect(dxccView, SIGNAL(itemEntered(QTableWidgetItem *)), this, SLOT(slotItemEntered(QTableWidgetItem *) ) );
    //connect(dxccView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRightButton( const QPoint& ) ) );
      //qDebug() << "DXCCStatusWidget::createUI-93" << QT_ENDL;
    //dxccView->setSortingEnabled(false);
      //qDebug() << "DXCCStatusWidget::createUI-END" << QT_ENDL;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::update()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "DXCCStatusWidget::update " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    //int entities = world->getHowManyEntities();
    int entities = dataProxy->getMaxEntityID(false);
    QList<int> bandIds;
    bandIds.clear();
    QString aux;
    foreach (aux, bandNames)
    {
        bandIds.append (dataProxy->getIdFromBandName (aux));
    }

    //qDebug() << "DXCCStatusWidget::update: " << QString::number(entities) << " entities to update" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    QList<int> list;
    //qDebug() << "DXCCStatusWidget::update: -1"  << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    list.clear();
    //qDebug() << "DXCCStatusWidget::update: -2"  << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    //QString prefix;

    //qDebug() << "DXCCStatusWidget::update: -3"  << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    if (dxccView->columnCount()>0)
    {
        dxccView->clearContents();
        tempLog = -1;
        //qDebug() << "DXCCStatusWidget::update pre FOR" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        for (int i=1; i<=entities; i++)
        {
            //qDebug() << "DXCCStatusWidget::update in FOR " << QString::number(i) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
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
    //qDebug() << "DXCCStatusWidget::update END" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
}

void DXCCStatusWidget::addEntity(const QList<int> &_ent)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    // _ent.at(0) = dxcc column of Entity Table (considering big numbers, like 2248 for IT9!)
    // _ent.at(1) until number of Columns are just the bandnames

    //qDebug() << "DXCCStatusWidget::addEntity: " << QString::number(_ent.at(0)) << "/" <<QString::number(_ent.at(1)) << " / " << QString::number(_ent.length()) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    // DXCC id, Entity Name, bandName1, bandName2, ...

    //QString au = "New Line ";
    //foreach (QString a, _ent)
    //{
    //   au = au + " - " + a;
    //}
    //qDebug() << Q_FUNC_INFO << ": " << au << " - " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;;

    if (_ent.length() != numberOfColumns-1)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: in number of columns" << QString::number(_ent.length()) << "/" << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        return;
    }

    int status = -1;

    //qDebug() << "DXCCStatusWidget::addEntity: ent = " << _ent << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    int _dxcc = _ent.at(0);
    QStringList data;
    data.clear();
    data << dataProxy->getEntiNameISOAndPrefixFromId(_dxcc);

    if (data.length ()!=3)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: Wrong Entity Name and Prefix - END" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
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
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: entname too short!" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
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

    //qDebug() << "DXCCStatusWidget::addEntity: Name: " << entName << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    //qDebug() << "DXCCStatusWidget::addEntity: Prefix: " << prefix << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    //qDebug() << "DXCCStatusWidget::addEntity: Flag: " << flagSt << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    dxccView->insertRow(dxccView->rowCount());
    //qDebug() << "DXCCStatusWidget::addEntity: rowCount:  " << QString::number(dxccView->rowCount()) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    QTableWidgetItem *newItemID = new QTableWidgetItem(prefix);
    newItemID->setTextAlignment(Qt::AlignCenter);
    newItemID->setFlags(Qt::NoItemFlags);
    dxccView->setItem(dxccView->rowCount()-1, 0, newItemID);
    awards->updateDXCCBAndsStatus (tempLog);
    for (int i=1; i < _ent.length(); i++)
    {
        //qDebug << Q_FUNC_INFO << ": " << prefix << " - i = " << QString::number(i) << "/" << _ent.at(i);
        int bandid = _ent.at(i);
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
        else
        {
        }

        dxccView->setItem(dxccView->rowCount()-1, i+1, newItem);
        //qDebug() << "DXCCStatusWidget::addEntity: rowCount-2:  " << QString::number(dxccView->rowCount()) << "/" << QString::number(i) << " / " << newItem->text() << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
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
    //qDebug() << "DXCCStatusWidget::addEntity: END" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::addEntity2(const QStringList &_ent)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    // _ent.at(0) = dxcc column of Entity Table (considering big numbers, like 2248 for IT9!)
    // _ent.at(1) until number of Columns are just the bandnames

    //qDebug() << "DXCCStatusWidget::addEntity: " << _ent.at(0) << "/" <<_ent.at(1) << " / " << QString::number(_ent.length()) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    // DXCC id, Entity Name, bandName1, bandName2, ...

    //QString au = "New Line ";
    //foreach (QString a, _ent)
    //{
    //   au = au + " - " + a;
    //}
    //qDebug() << Q_FUNC_INFO << ": " << au << " - " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;;

    if (_ent.length() != numberOfColumns-1)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: in number of columns" << QString::number(_ent.length()) << "/" << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        return;
    }

    int status = -1;

    //qDebug() << "DXCCStatusWidget::addEntity: ent = " << _ent << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    int _dxcc = _ent.at(0).toInt();
    QStringList data;
    data.clear();
    data << dataProxy->getEntiNameISOAndPrefixFromId(_dxcc);

    if (data.length ()!=3)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: Wrong Entity Name and Prefix - END" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
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
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: entname too short!" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
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

    //qDebug() << "DXCCStatusWidget::addEntity: Name: " << entName << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    //qDebug() << "DXCCStatusWidget::addEntity: Prefix: " << prefix << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    //qDebug() << "DXCCStatusWidget::addEntity: Flag: " << flagSt << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    dxccView->insertRow(dxccView->rowCount());
    //qDebug() << "DXCCStatusWidget::addEntity: rowCount:  " << QString::number(dxccView->rowCount()) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    QTableWidgetItem *newItemID = new QTableWidgetItem(prefix);
    newItemID->setTextAlignment(Qt::AlignCenter);
    newItemID->setFlags(Qt::NoItemFlags);
    dxccView->setItem(dxccView->rowCount()-1, 0, newItemID);
    awards->updateDXCCBAndsStatus (tempLog);
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
        else
        {
        }

        dxccView->setItem(dxccView->rowCount()-1, i+1, newItem);
        //qDebug() << "DXCCStatusWidget::addEntity: rowCount-2:  " << QString::number(dxccView->rowCount()) << "/" << QString::number(i) << " / " << newItem->text() << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
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
    //qDebug() << "DXCCStatusWidget::addEntity: END" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::setBands(const QString &_callingFunc, QStringList const &_ent, const bool _creating)
{// Receives the list of band names and defines the columns
    //qDebug() << Q_FUNC_INFO << "(" << _callingFunc << ")" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    //foreach(QString aux, _ent)
    //{
    //    //qDebug << Q_FUNC_INFO << ": " << aux;
    //}
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    QStringList qs;
    qs.clear();
    //qDebug() << "DXCCStatusWidget::setBands - 01" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    qs << dataProxy->sortBandNamesBottonUp(_ent);
    //qDebug() << "DXCCStatusWidget::setBands - 02: Lenght qs: " << QString::number(qs.size()) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    if (qs.length()<0)
    {
        //qDebug() << "DXCCStatusWidget::setBands no bands received here " << QTime::currentTime().toString("HH:mm:ss") << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        return;
    }

    QString testBand;
    testBand.clear();

    bandNames.clear();
    //qDebug() << "DXCCStatusWidget::setBands - 03 " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    validBands.clear();
    validBands = dataProxy->getBandNames();
    //qDebug() << "DXCCStatusWidget::setBands - 04 " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    dxccView->clearSelection();
    dxccView->clear();

    for (int i = 0; i<qs.length(); i++)
    {
        //qDebug() << "DXCCStatusWidget::setBands-4.1: " << qs.at(i) <<" - " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        testBand = qs.at(i);

        if (validBands.contains(qs.at(i)))
        {
            bandNames.append(testBand);
            //qDebug() << "DXCCStatusWidget::setBands-4.2: Added: " << bandNames.last()  << " - " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        }
        else
        {
            //qDebug() << "DXCCStatusWidget::setBands -4.3 Not valid band: " << testBand << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        }
    }
    dxccView->setColumnCount(0);
    numberOfColumns = dxccView->columnCount();

    while(dxccView->columnCount()>0)
    {
        //qDebug() << "DXCCStatusWidget::setBands: Still pending: " << QString::number(dxccView->columnCount()) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        dxccView->removeColumn(0);
    }
    numberOfColumns = dxccView->columnCount();
    //qDebug() << "DXCCStatusWidget::setBands: -6 All removed: " << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    numberOfColumns = 2 + bandNames.length(); //Prefix, Entity Name
    //qDebug() << "DXCCStatusWidget::setBands - 7.1 - columnCount: " << QString::number(dxccView->columnCount()) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    //qDebug() << "DXCCStatusWidget::setBands - 7.1 - numberOfColumns: " << QString::number(numberOfColumns) << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    dxccView->setColumnCount(numberOfColumns);
   //qDebug() << "DXCCStatusWidget::setBands - 7.2 " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    dxccView->setRowCount(0);
    //qDebug() << "DXCCStatusWidget::setBands - 7.3 " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    QStringList headerqs;
      //qDebug() << "DXCCStatusWidget::setBands - 7.4 " << QTime::currentTime()).toString("HH:mm:ss") << QT_ENDL;
    headerqs.clear();
    //qDebug() << "DXCCStatusWidget::setBands - 7.5 " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    headerqs << tr("Prefix") << tr("Entity") << bandNames;
    //qDebug() << "DXCCStatusWidget::setBands - 8 " << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    dxccView->setHorizontalHeaderLabels(headerqs);

    //qDebug() << "DXCCStatusWidget::setBands - 9 PRE-END" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;

    if (_creating)
    {
        //qDebug() << "DXCCStatusWidget::setBands: 9.1 !_creating so updating!" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
        update();
    }

    //qDebug() << "DXCCStatusWidget::setBands: END" << QTime::currentTime().toString("HH:mm:ss") << QT_ENDL;
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
}

void DXCCStatusWidget::setDefaultBands()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::setDefaultBands" << QT_ENDL;
    /*
     Default bands:
     160M    80M  40M  30M  20M  17M  15M  12M  10M  6M   4M   2M   70CM
     28     27  25  24  23  22  21  12  19  18  17  16  14
     */

    bandNames.clear();
    //bandNames << "160M" << "80M" << "40M" << "30M" << "20M" << "17M" << "15M" << "12M" << "10M" << "6M" << "4M" << "2M" << "70CM";
    bandNames << "160M" << "80M" << "40M" << "30M" << "20M" << "17M" << "15M" << "12M" << "10M";

    setBands(Q_FUNC_INFO, bandNames, false);
    //qDebug << "DXCCStatusWidget::setDefaultBands - END" << QT_ENDL;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::slotRefreshButtonClicked()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);

      //qDebug() << "DXCCStatusWidget::slotRefreshButtonClicked" << QT_ENDL;
    //TODO: Define a way to show the status of the selected log or all the logs in the DB
    if (dxccView->rowCount()<1)
    {
          //qDebug() << "DXCCStatusWidget::slotRefreshButtonClicked - rowcount <1" << QT_ENDL;
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
      //qDebug() << "DXCCStatusWidget::refresh" << QT_ENDL;
    //update();
    slotRefreshButtonClicked();
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

 void DXCCStatusWidget::setCurrentLog(const int _logN)
 {
     emit debugLog (Q_FUNC_INFO, "Start", Debug);
       //qDebug() << "DXCCStatusWidget::setCurrentLog: " << QString::number(_logN) << QT_ENDL;
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
    //qDebug << "DXCCStatusWidget::slotItemEntered: " << ((item->data(0)).toString()).simplified() << QT_ENDL;
    //qDebug << "DXCCStatusWidget::slotItemEntered: " << item->text() << QT_ENDL;
    //qDebug << "DXCCStatusWidget::slotItemEntered: " << QT_ENDL;
     emit debugLog (Q_FUNC_INFO, "Start", Debug);
    if (dxccView->rowCount()<1)
    {
        return;
    }

    QString tip;
    tip.clear();
    //int row = -1;
    int column = -1;
    int bandi = -1;
    QString band = QString();


    if (item)
    {
      //  row = dxccView->row(item);
        column = dxccView->column(item);

           //qDebug() << "DXCCStatusWidget::slotItemEntered: row: " << QString::number(row) << QT_ENDL;
           //qDebug() << "DXCCStatusWidget::slotItemEntered: hrow: " <<  dxccView->verticalHeaderItem(row)->text() << QT_ENDL;
      //  row = (dxccView->item(row,0)->text()).toInt();

        int entiID = dataProxy->getEntityIdFromMainPrefix(dxccView->item(dxccView->row(item),0)->text());
        if (entiID<0)
        { // Some prefix are shown with a * in the begining that is removed to show the real prefix
            entiID = dataProxy->getEntityIdFromMainPrefix("*" + dxccView->item(dxccView->row(item),0)->text());
        }
          //qDebug() << "DXCCStatusWidget::slotItemEntered: entiID: " << QString::number(entiID) << QT_ENDL;

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
                band = dxccView->horizontalHeaderItem(column)->text();
                   //qDebug() << "DXCCStatusWidget::slotItemEntered: band: " << band << QT_ENDL;
                bandi = dataProxy->getIdFromBandName(band);
                   //qDebug() << "DXCCStatusWidget::slotItemEntered: band: " << QString::number(bandi) << QT_ENDL;
                if (item->text() == "C")
                {
                    tip = awards->getQSOofAward(entiID, bandi, logNumber, true);
                }
                else
                {
                    tip = awards->getQSOofAward(entiID, bandi, logNumber, false);
                }

                   //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip << QT_ENDL;
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
           //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip << QT_ENDL;
    }
    else
    {}

      //qDebug() << "DXCCStatusWidget::slotItemEntered: END " << QT_ENDL;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
 }

void DXCCStatusWidget::slotItemDoubleClicked(QTableWidgetItem  * item )
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << ((item->data(0)).toString()).simplified() << QT_ENDL;
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << item->text() << QT_ENDL;
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked - start" << QT_ENDL;
    if (dxccView->rowCount()<1)
    {
        return;
    }
    int row = -1;
    //int column = -1;
    int qsoId = -1;
    int bandi = -1;
    int columns = dxccView->columnCount();
    int _entiNumb = -1; //Read the Entity number that the user is selecting.
    QString band = QString();

    QList<int> qsos;
    qsos.clear();

    QTableWidgetItem * it = new QTableWidgetItem(0);

    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - Columns: " << QString::number(columns)  << QT_ENDL;

    if (item)
    {
        row = dxccView->row(item);
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - row: "  << QString::number(row) << QT_ENDL;

        _entiNumb= dataProxy->getEntityIdFromMainPrefix(dxccView->item(dxccView->row(item),0)->text());
        if (_entiNumb<0)
        { // Some prefix are shown with a * in the begining that is removed to show the real prefix
            _entiNumb= dataProxy->getEntityIdFromMainPrefix("*" + dxccView->item(dxccView->row(item),0)->text());
        }
       //_entiNumb = ((dxccView->item(row,0))->text()).toInt();
          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - EntityNumber: " << QString::number(_entiNumb) << QT_ENDL;

       for (int i = 2; i < columns; ++i)
       {
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column: "  << QString::number(i) << QT_ENDL;
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column header: " << (dxccView->horizontalHeaderItem(i))->text() << QT_ENDL;
            //entityName = (dxccView->item(row,i))->text() ;
              //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - item: "  << QT_ENDL;

            it->setText(dxccView->item(row,i)->text());
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column-txt: "  << it->text() << QT_ENDL;

            band = dxccView->horizontalHeaderItem(i)->text();
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << band << QT_ENDL;
            bandi = dataProxy->getIdFromBandName(band);
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << QString::number(bandi) << QT_ENDL;

          if (item->text() == "C")
          {
            qsoId = awards->getQSOIdofAward(_entiNumb, bandi, logNumber, true);
          }
          else
          {
            qsoId = awards->getQSOIdofAward(_entiNumb, bandi, logNumber, false);
          }

          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: QSOid: " << QString::number(qsoId) << QT_ENDL;
          if (qsoId>0)
          {
              qsos.append(qsoId);
          }

        }
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - END of for"  << QT_ENDL;
        if (qsos.length()>0)
        {
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - qsos.length = " << QString::number(qsos.length())  << QT_ENDL;
            emit showQsos(qsos);
        }
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - END of column == 1"  << QT_ENDL;
    }
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: END " << QT_ENDL;
}

void DXCCStatusWidget::setMyLocator(const QString &_loc)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::setMyLocator: " << _loc << QT_ENDL;
    QString l = _loc;
    if ( locator->isValidLocator(l.toUpper()) )
    {
        loc = l.toUpper();
    }

}
/*
void DXCCStatusWidget::slotRightButton(const QPoint& pos)
{
       //qDebug() << "DXCCStatusWidget::slotRighButton"  << QT_ENDL;
    int row = (dxccView->indexAt(pos)).row();

    showDXCCWikipediaAct = new QAction(tr("&Wikipedia"), this);
    showDXCCWikipediaAct->setShortcut(Qt::CTRL + Qt::Key_W);
    showDXCCWikipediaAct->setStatusTip(tr("Show this DXCC in Wikipedia"));
    connect(showDXCCWikipediaAct, SIGNAL(triggered()), this, SLOT(slotWikipedia()));

    righButtonFromLogMenu(row);

}

void DXCCStatusWidget::slotWikipedia()
{
       //qDebug() << "DXCCStatusWidget::slotWikipedia: " << (showDXCCWikipediaAct->data()).toString() << QT_ENDL;
    //TODO: To be added to the DXCCStatusWidget and create an action that emist the QSO id
    //int QSOid = ((logModel->index((showDXCCWikipediaAct->data()).toInt(), 0)).data(0)).toInt();

      //qDebug() << "DXCCStatusWidget::slotQsoDeleteFromLog (id): " << QString::number(QSOid) << QT_ENDL;

}

void DXCCStatusWidget::righButtonFromLogMenu(const int trow)
{
        //qDebug() << "DXCCStatusWidget::slotshowRighButtonFromLogMenu:  " << QString::number(trow) << QT_ENDL;

    //int _qsoID = ((logModel->index(trow, 0)).data(0)).toInt();
       //qDebug() << "DXCCStatusWidget::slotshowRighButtonFromLogMenu:  QSOid: " << QString::number(_qsoID) << QT_ENDL;

    QMenu menu(this);

    menu.addAction(showDXCCWikipediaAct);
    showDXCCWikipediaAct->setData(trow);


    menu.exec(QCursor::pos());
}
*/
