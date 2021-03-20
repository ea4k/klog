#include "dxccstatuswidget.h"
//#include <QDebug>

/*

TODO: Adjust the header columns
TODO: Add some color to the texts depending on C, W or -
TODO: Call the creation of this depending on the bands that the user is using

*/

DXCCStatusWidget::DXCCStatusWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent) : QWidget(parent)
{
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget from: " << _parentFunction << endl;
    dataProxy = dp;
    locator = new Locator();
    awards = new Awards(dataProxy, Q_FUNC_INFO);
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-1" << endl;
    world = new World(dataProxy,Q_FUNC_INFO);
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-2" << endl;

    dxccView = new QTableWidget;
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-3" << endl;
    dxccView->setMouseTracking(true);
    dxccView->setSortingEnabled(true);
    numberOfColumns = 0;
    logNumber = -1; // -1 means that ALL the logs will be used (if showAllLogsButton is not checked)
    tempLog = -1;   // -1 means that ALL the logs will be used
    loc = QString();
    refreshButton = new QPushButton;

    bandNames.clear();
    validBands.clear();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-4" << endl;
    setDefaultBands();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget-5" << endl;
    createUI();
      //qDebug() << "DXCCStatusWidget::DXCCStatusWidget - END" << endl;
}

DXCCStatusWidget::~DXCCStatusWidget(){}

void DXCCStatusWidget::createUI()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::createUI " << endl;

    // We remove the vertical header
    hv = dxccView->verticalHeader();
    hv->hide();
    hv->setStretchLastSection(true);
    hh = dxccView->horizontalHeader();
      //qDebug() << "DXCCStatusWidget::createUI-10" << endl;
    refreshButton->setText(tr("Update"));
    //showAllLogsButton->setText("All logs");
      //qDebug() << "DXCCStatusWidget::createUI-11" << endl;

    dxccView->setContextMenuPolicy(Qt::CustomContextMenu);
      //qDebug() << "DXCCStatusWidget::createUI-12" << endl;
    dxccView->setColumnCount(numberOfColumns);
      //qDebug() << "DXCCStatusWidget::createUI-13" << endl;
    dxccView->horizontalHeader()->setStretchLastSection(true);
      //qDebug() << "DXCCStatusWidget::createUI-14" << endl;

      //qDebug() << "DXCCStatusWidget::createUI-15" << endl;
    dxccView->setRowCount(0);
      //qDebug() << "DXCCStatusWidget::createUI-20" << endl;
    QHBoxLayout *bottonLineLayout = new QHBoxLayout;
    bottonLineLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    bottonLineLayout->addWidget(refreshButton);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(dxccView);
    tabLayout->addLayout(bottonLineLayout);
    setLayout(tabLayout);
      //qDebug() << "DXCCStatusWidget::createUI-30" << endl;
    dxccView->resizeColumnsToContents();
    dxccView->resizeRowsToContents();
      //qDebug() << "DXCCStatusWidget::createUI-90" << endl;
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(slotRefreshButtonClicked() ) );
      //qDebug() << "DXCCStatusWidget::createUI-91" << endl;
    connect(dxccView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotItemDoubleClicked(QTableWidgetItem *) ) );
      //qDebug() << "DXCCStatusWidget::createUI-92" << endl;
    connect(dxccView, SIGNAL(itemEntered(QTableWidgetItem *)), this, SLOT(slotItemEntered(QTableWidgetItem *) ) );
    //connect(dxccView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRightButton( const QPoint& ) ) );
      //qDebug() << "DXCCStatusWidget::createUI-93" << endl;
    //dxccView->setSortingEnabled(false);
      //qDebug() << "DXCCStatusWidget::createUI-END" << endl;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::update()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::update " << endl;
    //int entities = world->getHowManyEntities();
    int entities = dataProxy->getMaxEntityID(false);

     //qDebug() << "DXCCStatusWidget::update: " << QString::number(entities) << " entities to update" << endl;
    QStringList list;
     //qDebug() << "DXCCStatusWidget::update: -1"  << endl;
    list.clear();
     //qDebug() << "DXCCStatusWidget::update: -2"  << endl;

    QString aux, prefix;
     //qDebug() << "DXCCStatusWidget::update: -3"  << endl;
    if (dxccView->columnCount()>0)
    {
        //dxccView->sortByColumn(1, Qt::AscendingOrder);
        dxccView->clearContents();
        tempLog = -1;
         //qDebug() << "DXCCStatusWidget::update pre FOR" << endl;
        for (int i=1; i<=entities; i++)
        {
             //qDebug() << "DXCCStatusWidget::update in FOR " << QString::number(i) << endl;
            aux = world->getEntityName(i);
            prefix = world->getEntityMainPrefix(i);
            if (prefix.startsWith("*"))
            {
                QString a = prefix.right(prefix.length()-1);
                prefix = a;
            }
             //qDebug() << "DXCCStatusWidget::update in FOR aux: " << aux << endl;
            list.clear();
            if (aux.length()>2)  // Not all integers refers to an entity.
            {
                list << prefix << aux  << bandNames;
                addEntity(list);
            }
        }
    }
    emit debugLog (Q_FUNC_INFO, "END", Debug);
     //qDebug() << "DXCCStatusWidget::update END" << endl;
}

void DXCCStatusWidget::addEntity(const QStringList &_ent)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::addEntity: " << _ent.at(1) << " / " << QString::number(_ent.length()) << endl;
    // DXCC id, Entity Name, bandName1, bandName2, ...
    if (_ent.length() != numberOfColumns)
    {
          //qDebug() << "DXCCStatusWidget::addEntity: ERROR: in number of columns" << QString::number(_ent.length()) << "/" << QString::number(numberOfColumns) << endl;
        return;
    }

    int status = -1;
    int ent = dataProxy->getEntityIdFromMainPrefix(_ent.at(0));
            //(_ent.at(0)).toInt();
    int bandid = 0;
      //qDebug() << "DXCCStatusWidget::addEntity: ent = " << QString::number(ent) << endl;

    QString entName = _ent.at(1);

    if (entName.length()<2)
    {
          //qDebug() << "DXCCStatusWidget::addEntity: ERROR: entname too short!" << endl;
        return;
    }

    QString flagSt;
    flagSt.clear();
    QString aux;

    aux = dataProxy->getISOName(ent);
    if (aux.length()>1)
    {
        flagSt = ":/" + aux + ".png";
    }
    else
    {
        flagSt.clear();
    }

    flagSt = ":/flags/" + aux + ".png";
    QIcon flagIcon(flagSt);

       //qDebug() << "DXCCStatusWidget::addEntity: Flag: " << flagSt << endl;

    dxccView->insertRow(dxccView->rowCount());
       //qDebug() << "DXCCStatusWidget::addEntity: rowCount:  " << QString::number(dxccView->rowCount()) << endl;

    QTableWidgetItem *newItemID = new QTableWidgetItem(_ent.at(0));

    newItemID->setTextAlignment(Qt::AlignCenter);
    newItemID->setFlags(Qt::NoItemFlags);
    dxccView->setItem(dxccView->rowCount()-1, 0, newItemID);

    for (int i=2; i < _ent.length(); i++)
    {
        bandid = dataProxy->getIdFromBandName(_ent.at(i));
        QTableWidgetItem *newItem = new QTableWidgetItem(awards->getDXCCStatusBand(ent, bandid, tempLog));
        newItem->setTextAlignment(Qt::AlignCenter);        
        newItem->setFlags(Qt::ItemIsEnabled);

        if (newItem->text()=="C")
        {
            newItem->setTextColor(Qt::blue);
            newItem->setBackgroundColor(Qt::green);
            status = 1;
        }
        else if (newItem->text()=="W")
        {
            if (status < 0)
            {
                status = 0;
            }
            newItem->setTextColor(Qt::red);
            newItem->setBackgroundColor(Qt::yellow);
        }
        else
        {
        }

        dxccView->setItem(dxccView->rowCount()-1, i, newItem);
           //qDebug() << "DXCCStatusWidget::addEntity: rowCount-2:  " << QString::number(dxccView->rowCount()) << "/" << QString::number(i) << " / " << newItem->text() << endl;
    }

    QTableWidgetItem *newItemPref = new QTableWidgetItem(_ent.at(0));
    newItemPref->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    newItemPref->setFlags(Qt::ItemIsEnabled);

    QTableWidgetItem *newItemName = new QTableWidgetItem(entName);
    newItemName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    newItemName->setFlags(Qt::ItemIsEnabled);
    //newItemName->setFlags(Qt::ItemIsUserCheckable);
    newItemName->setIcon(flagIcon);

    if (status == 1)
    {
        newItemName->setTextColor(Qt::blue);
        newItemPref->setTextColor(Qt::blue);
        //newItemName->setTextColor(Qt::blue);
        //newItemName->setBackgroundColor(Qt::green);
    }
    else if (status == 0)
    {
        newItemName->setTextColor(Qt::darkRed);
        newItemPref->setTextColor(Qt::darkRed);
    }
    else
    {
        newItemName->setTextColor(Qt::red);
        newItemPref->setTextColor(Qt::red);
    }
    dxccView->setItem(dxccView->rowCount()-1, 0, newItemPref);
    dxccView->setItem(dxccView->rowCount()-1, 1, newItemName);
       //qDebug() << "DXCCStatusWidget::addEntity: END" << endl;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::setBands(QStringList const &_ent, const bool _creating)
{// Receives the list of band names
      //qDebug() << "DXCCStatusWidget::setBands: " << endl;
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
    QStringList qs;
    qs.clear();
      //qDebug() << "DXCCStatusWidget::setBands - 01" << endl;
    qs << dataProxy->sortBandNamesBottonUp(_ent);
      //qDebug() << "DXCCStatusWidget::setBands - 02: Lenght qs: " << QString::number(qs.size()) << endl;
    if (qs.length()<0)
    {
         //qDebug() << "DXCCStatusWidget::setBands no bands received here " << endl;
        return;
    }

    QString testBand;
    testBand.clear();

    bandNames.clear();
    //bandNames << "Id" << "Entity";
      //qDebug() << "DXCCStatusWidget::setBands - 03 " << endl;
    validBands.clear();
    //validBands << dataProxy->getBands();
    validBands = dataProxy->getBandNames();
      //qDebug() << "DXCCStatusWidget::setBands - 04 " << endl;
    //dxccView->clearContents();
    dxccView->clearSelection();
    dxccView->clear();

    for (int i = 0; i<qs.length(); i++)
    {
          //qDebug() << "DXCCStatusWidget::setBands-4.1: " << qs.at(i) << endl;
        testBand = qs.at(i);

        if (validBands.contains(qs.at(i)))
        {
            bandNames.append(testBand);
              //qDebug() << "DXCCStatusWidget::setBands-4.2: Added: " << bandNames.last()  << endl;
        }
        else
        {
            //qDebug() << "DXCCStatusWidget::setBands -4.3 Not valid band: " << testBand << endl;
        }
    }
    dxccView->setColumnCount(0);
    numberOfColumns = dxccView->columnCount();

    while(dxccView->columnCount()>0)
    {
          //qDebug() << "DXCCStatusWidget::setBands: Still pending: " << QString::number(dxccView->columnCount()) << endl;
        dxccView->removeColumn(0);
    }
    numberOfColumns = dxccView->columnCount();
      //qDebug() << "DXCCStatusWidget::setBands: -6 All removed: " << QString::number(numberOfColumns) << endl;


      //qDebug() << "DXCCStatusWidget::setBands - 7 " << endl;

    numberOfColumns = 2 + bandNames.length(); //Prefix, Entity Name
      //qDebug() << "DXCCStatusWidget::setBands - 7.1 - columnCount: " << QString::number(dxccView->columnCount()) << endl;
      //qDebug() << "DXCCStatusWidget::setBands - 7.1 - numberOfColumns: " << QString::number(numberOfColumns) << endl;
    dxccView->setColumnCount(numberOfColumns);  
      //qDebug() << "DXCCStatusWidget::setBands - 7.2 " << endl;
    dxccView->setRowCount(0);
      //qDebug() << "DXCCStatusWidget::setBands - 7.3 " << endl;

    QStringList headerqs;
      //qDebug() << "DXCCStatusWidget::setBands - 7.4 " << endl;
    headerqs.clear();
      //qDebug() << "DXCCStatusWidget::setBands - 7.5 " << endl;
    headerqs << tr("Prefix") << tr("Entity") << bandNames;
      //qDebug() << "DXCCStatusWidget::setBands - 8 " << endl;
    dxccView->setHorizontalHeaderLabels(headerqs);

      //qDebug() << "DXCCStatusWidget::setBands - 9 PRE-END" << endl;

    //bool hasCurrentLoGSatellitesQSOs(const int _logn);

    if (!_creating)
    {
          //qDebug() << "DXCCStatusWidget::setBands: 9.1 !_creating" << endl;
        update();
    }

      //qDebug() << "DXCCStatusWidget::setBands: END" << endl;
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
}

void DXCCStatusWidget::setDefaultBands()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::setDefaultBands" << endl;
    /*
     Default bands:
     160M    80M  40M  30M  20M  17M  15M  12M  10M  6M   4M   2M   70CM
     28     27  25  24  23  22  21  12  19  18  17  16  14
     */

    bandNames.clear();
    bandNames << "160M" << "80M" << "40M" << "30M" << "20M" << "17M" << "15M" << "12M" << "10M" << "6M" << "4M" << "2M" << "70CM";

    setBands(bandNames, true);
      //qDebug() << "DXCCStatusWidget::setDefaultBands - END" << endl;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::slotRefreshButtonClicked()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);

      //qDebug() << "DXCCStatusWidget::slotRefreshButtonClicked" << endl;
    //TODO: Define a way to show the status of the selected log or all the logs in the DB
    if (dxccView->rowCount()<1)
    {
          //qDebug() << "DXCCStatusWidget::slotRefreshButtonClicked - rowcount <1" << endl;
        return;
    }
    QStringList _bands = bandNames;
    setBands(_bands);

    //emit updateAwards();
    //update();
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

void DXCCStatusWidget::refresh()
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::refresh" << endl;
    //update();
    slotRefreshButtonClicked();
    emit debugLog (Q_FUNC_INFO, "END", Debug);
}

 void DXCCStatusWidget::setCurrentLog(const int _logN)
 {
     emit debugLog (Q_FUNC_INFO, "Start", Debug);
       //qDebug() << "DXCCStatusWidget::setCurrentLog: " << QString::number(_logN) << endl;
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
       //qDebug() << "DXCCStatusWidget::slotItemEntered: " << ((item->data(0)).toString()).simplified() << endl;
       //qDebug() << "DXCCStatusWidget::slotItemEntered: " << item->text() << endl;
       //qDebug() << "DXCCStatusWidget::slotItemEntered: " << endl;
     emit debugLog (Q_FUNC_INFO, "Start", Debug);
    if (dxccView->rowCount()<1)
    {
        return;
    }

    QString tip;
    tip.clear();
    int row = -1;
    int column = -1;
    int bandi = -1;
    QString band = QString();


    if (item)
    {
      //  row = dxccView->row(item);
        column = dxccView->column(item);

           //qDebug() << "DXCCStatusWidget::slotItemEntered: row: " << QString::number(row) << endl;
           //qDebug() << "DXCCStatusWidget::slotItemEntered: hrow: " <<  dxccView->verticalHeaderItem(row)->text() << endl;
      //  row = (dxccView->item(row,0)->text()).toInt();

        int entiID = dataProxy->getEntityIdFromMainPrefix(dxccView->item(dxccView->row(item),0)->text());
        if (entiID<0)
        { // Some prefix are shown with a * in the begining that is removed to show the real prefix
            entiID = dataProxy->getEntityIdFromMainPrefix("*" + dxccView->item(dxccView->row(item),0)->text());
        }
          //qDebug() << "DXCCStatusWidget::slotItemEntered: entiID: " << QString::number(entiID) << endl;

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
                   //qDebug() << "DXCCStatusWidget::slotItemEntered: band: " << band << endl;
                bandi = dataProxy->getIdFromBandName(band);
                   //qDebug() << "DXCCStatusWidget::slotItemEntered: band: " << QString::number(bandi) << endl;
                tip = awards->getQSOofAward(entiID, bandi);
                   //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip << endl;
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
           //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip << endl;
    }
    else
    {}

      //qDebug() << "DXCCStatusWidget::slotItemEntered: END " << endl;
    emit debugLog (Q_FUNC_INFO, "END", Debug);
 }

void DXCCStatusWidget::slotItemDoubleClicked(QTableWidgetItem  * item )
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
       //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << ((item->data(0)).toString()).simplified() << endl;
       //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << item->text() << endl;
      //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked - start" << endl;
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

      //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - 01"  << endl;

    if (item)
    {

        row = dxccView->row(item);

          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column = 1"  << endl;

        _entiNumb= dataProxy->getEntityIdFromMainPrefix(dxccView->item(dxccView->row(item),0)->text());
        if (_entiNumb<0)
        { // Some prefix are shown with a * in the begining that is removed to show the real prefix
            _entiNumb= dataProxy->getEntityIdFromMainPrefix("*" + dxccView->item(dxccView->row(item),0)->text());
        }
       //_entiNumb = ((dxccView->item(row,0))->text()).toInt();
          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - EntityNumber: " << QString::number(_entiNumb) << endl;

       for (int i = 2; i < columns; ++i)
       {
              //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column: "  << QString::number(i) << endl;
              //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column header: " << (dxccView->horizontalHeaderItem(i))->text() << endl;
            //entityName = (dxccView->item(row,i))->text() ;
              //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - item: "  << endl;

          it->setText(dxccView->item(row,i)->text());
            //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - column-txt: "  << it->text() << endl;

          band = dxccView->horizontalHeaderItem(i)->text();
             //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << band << endl;
          bandi = dataProxy->getIdFromBandName(band);
             //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << QString::number(bandi) << endl;
          qsoId = awards->getQSOIdofAward(_entiNumb, bandi);
             //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: QSOid: " << QString::number(qsoId) << endl;
          if (qsoId>0)
          {
              qsos.append(qsoId);
          }

       }
          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - END of for"  << endl;
       if (qsos.length()>0)
       {
              //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - qsos.length = " << QString::number(qsos.length())  << endl;
           emit showQsos(qsos);
       }
          //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: - END of column == 1"  << endl;

    }


       //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: END " << endl;
}

void DXCCStatusWidget::setMyLocator(const QString &_loc)
{
    emit debugLog (Q_FUNC_INFO, "Start", Debug);
      //qDebug() << "DXCCStatusWidget::setMyLocator: " << _loc << endl;
    QString l = _loc;
    if ( locator->isValidLocator(l.toUpper()) )
    {
        loc = l.toUpper();
    }

}
/*
void DXCCStatusWidget::slotRightButton(const QPoint& pos)
{
       //qDebug() << "DXCCStatusWidget::slotRighButton"  << endl;
    int row = (dxccView->indexAt(pos)).row();

    showDXCCWikipediaAct = new QAction(tr("&Wikipedia"), this);
    showDXCCWikipediaAct->setShortcut(Qt::CTRL + Qt::Key_W);
    showDXCCWikipediaAct->setStatusTip(tr("Show this DXCC in Wikipedia"));
    connect(showDXCCWikipediaAct, SIGNAL(triggered()), this, SLOT(slotWikipedia()));

    righButtonFromLogMenu(row);

}

void DXCCStatusWidget::slotWikipedia()
{
       //qDebug() << "DXCCStatusWidget::slotWikipedia: " << (showDXCCWikipediaAct->data()).toString() << endl;
    //TODO: To be added to the DXCCStatusWidget and create an action that emist the QSO id
    //int QSOid = ((logModel->index((showDXCCWikipediaAct->data()).toInt(), 0)).data(0)).toInt();

      //qDebug() << "DXCCStatusWidget::slotQsoDeleteFromLog (id): " << QString::number(QSOid) << endl;

}

void DXCCStatusWidget::righButtonFromLogMenu(const int trow)
{
        //qDebug() << "DXCCStatusWidget::slotshowRighButtonFromLogMenu:  " << QString::number(trow) << endl;

    //int _qsoID = ((logModel->index(trow, 0)).data(0)).toInt();
       //qDebug() << "DXCCStatusWidget::slotshowRighButtonFromLogMenu:  QSOid: " << QString::number(_qsoID) << endl;

    QMenu menu(this);

    menu.addAction(showDXCCWikipediaAct);
    showDXCCWikipediaAct->setData(trow);


    menu.exec(QCursor::pos());
}
*/
