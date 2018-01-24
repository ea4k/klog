#include "dxccstatuswidget.h"
//#include <QDebug>

/*

TODO: Adjust the header columns
TODO: Add some color to the texts depending on C, W or -
TODO: Call the creation of this depending on the bands that the user is using

*/

DXCCStatusWidget::DXCCStatusWidget(DataProxy *dp, QWidget *parent) : QWidget(parent)
{
   //qDebug() << "DXCCStatusWidget::DXCCStatusWidget" << endl;
    dataProxy = dp;
    awards = new Awards(dataProxy);
    world = new World(dataProxy);

    dxccView = new QTableWidget;
    dxccView->setMouseTracking(true);
    //hv = new QHeaderView(Qt::Vertical, dxccView);
    //hh = new QHeaderView(Qt::Horizontal, this);

    numberOfColumns = 0;
    logNumber = -1; // -1 means that ALL the logs will be used (if showAllLogsButton is not checked)
    tempLog = -1;   // -1 means that ALL the logs will be used

    //searchLineEdit = new QLineEdit;
    refreshButton = new QPushButton;    
    //showAllLogsButton = new QRadioButton;

    bandNames.clear();
    validBands.clear();



    setDefaultBands();
    createUI();
    //qDebug() << "DXCCStatusWidget::DXCCStatusWidget - END" << endl;

}

DXCCStatusWidget::~DXCCStatusWidget(){}

void DXCCStatusWidget::createUI()
{
   //qDebug() << "DXCCStatusWidget::createUI " << endl;

    // We remove the vertical header
    hv = dxccView->verticalHeader();
    hv->hide();
    hv->setStretchLastSection(true);
    hh = dxccView->horizontalHeader();


    refreshButton->setText(tr("Update"));
    //showAllLogsButton->setText("All logs");

    dxccView->setContextMenuPolicy(Qt::CustomContextMenu);
    dxccView->setSortingEnabled(true);
    dxccView->horizontalHeader()->setStretchLastSection(true);
    dxccView->setColumnCount(numberOfColumns);
    dxccView->setRowCount(0);

    QHBoxLayout *bottonLineLayout = new QHBoxLayout;
    bottonLineLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    bottonLineLayout->addWidget(refreshButton);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(dxccView);
    tabLayout->addLayout(bottonLineLayout);
    setLayout(tabLayout);

    dxccView->resizeColumnsToContents();
    dxccView->resizeRowsToContents();

    connect(refreshButton, SIGNAL(clicked()), this, SLOT(slotRefreshButtonClicked() ) );
    connect(dxccView, SIGNAL(itemEntered(QTableWidgetItem *)), this, SLOT(slotItemEntered(QTableWidgetItem *) ) );
    //connect(dxccView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotItemDoubleClicked(QTableWidgetItem *) ) );
    connect(dxccView, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(slotItemDoubleClicked(QTableWidgetItem *) ) );

}

void DXCCStatusWidget::update()
{
   //qDebug() << "DXCCStatusWidget::update " << endl;
    int entities = world->getHowManyEntities();
   //qDebug() << "DXCCStatusWidget::update: " << QString::number(entities) << " entities to update" << endl;
    QStringList list;
    QString aux;
    dxccView->sortByColumn(1, Qt::AscendingOrder);
    dxccView->clearContents();

    tempLog = -1;
    for (int i=1; i<=entities; i++)
    {
        aux = world->getEntityName(i);
        list.clear();
        if (aux.length()>2)
        {
            list << QString::number(i) << aux  << bandNames;
            addEntity(list);
        }
    }
    //qDebug() << "DXCCStatusWidget::update END" << endl;
}

void DXCCStatusWidget::addEntity(QStringList const _ent)
{
   //qDebug() << "DXCCStatusWidget::addEntity: " << _ent.at(1) << " / " << QString::number(_ent.length()) << endl;
    // DXCC id, Entity Name, bandName1, bandName2, ...
    if (_ent.length() != numberOfColumns)
    {
        //qDebug() << "DXCCStatusWidget::addEntity: ERROR: in number of columns" << QString::number(_ent.length()) << "/" << QString::number(numberOfColumns) << endl;
        return;
    }

    int status = -1;

    int ent = (_ent.at(0)).toInt();
    int bandid = 0;

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

    //connect(dxccView, SIGNAL(itemEntered(QTableWidgetItem *)), this, SLOT(slotItemEntered(QTableWidgetItem *) ) );
    //connect(dxccView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotItemDoubleClicked(QTableWidgetItem *) ) );


    //QTableWidgetItem::QTableWidgetItem(const QIcon & icon, const QString & text, int type = Type)
   // QTableWidgetItem *newItemFlag = new QTableWidgetItem(QIcon(flagSt), "T", 0);

    //QFont font;                                 // To show smaller letters "W" and "C" in the table
    //font.setStretch(QFont::UltraCondensed);

    for (int i=2; i < _ent.length(); i++)
    {
        bandid = dataProxy->getIdFromBandName(_ent.at(i));
        QTableWidgetItem *newItem = new QTableWidgetItem(awards->getDXCCStatusBand(ent, bandid, tempLog));
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setFlags(Qt::NoItemFlags);
        //newItem->setFont(font);

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

    QTableWidgetItem *newItemName = new QTableWidgetItem(entName);
    newItemName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    newItemName->setFlags(Qt::ItemIsEnabled);
    //newItemName->setFlags(Qt::ItemIsUserCheckable);
    newItemName->setIcon(flagIcon);

    if (status == 1)
    {
        newItemName->setTextColor(Qt::blue);
        //newItemName->setTextColor(Qt::blue);
        //newItemName->setBackgroundColor(Qt::green);
    }
    else if (status == 0)
    {
        newItemName->setTextColor(Qt::darkRed);
    }
    else
    {
        newItemName->setTextColor(Qt::red);
    }

    dxccView->setItem(dxccView->rowCount()-1, 1, newItemName);
    //qDebug() << "DXCCStatusWidget::addEntity: END" << endl;

}

void DXCCStatusWidget::setBands(QStringList const _ent, const bool _creating)
{// Receives the list of band names

  //qDebug() << "DXCCStatusWidget::setBands: " << QString::number(_ent.length()) << endl;
    if (_creating)
    {
       //qDebug() << "DXCCStatusWidget::setBands (creating true) " << QString::number(_ent.length()) << endl;
    }
    else
    {
       //qDebug() << "DXCCStatusWidget::setBands (creating false) " << QString::number(_ent.length()) << endl;
    }

    QStringList qs;
    qs.clear();
    //qDebug() << "DXCCStatusWidget::setBands - 1 " << endl;
    qs << dataProxy->sortBandNamesBottonUp(_ent);
    //qDebug() << "DXCCStatusWidget::setBands - 2 " << endl;
    if (qs.length()<0)
    {
        //qDebug() << "DXCCStatusWidget::setBands no bands received here " << endl;
        return;
    }

    QString testBand;
    testBand.clear();

    bandNames.clear();
    //bandNames << "Id" << "Entity";
   //qDebug() << "DXCCStatusWidget::setBands - 3 " << endl;
    validBands.clear();
    //validBands << dataProxy->getBands();
    validBands << dataProxy->getBandNames();
    //qDebug() << "DXCCStatusWidget::setBands - 4 " << endl;
    dxccView->clearContents();    
   // for (int x = 0; x < dxccView->columnCount(); x++)
   // {
   //     dxccView->removeColumn(x);
   // }

    //bands.clear();
    for (int i = 0; i<qs.length(); i++)
    {
        //qDebug() << "DXCCStatusWidget::setBands-1: " << qs.at(i) << endl;
        testBand = qs.at(i);

        if (validBands.contains(qs.at(i)))
        {
            bandNames.append(testBand);
           //qDebug() << "DXCCStatusWidget::setBands-2: Added: " << bandNames.last()  << endl;
        }
        else
        {
         //qDebug() << "DXCCStatusWidget::setBands: Not valid band: " << testBand << endl;
        }
    }
   //qDebug() << "DXCCStatusWidget::setBands - 5 " << endl;

    numberOfColumns = 2 + bandNames.length();
    dxccView->setColumnCount(numberOfColumns);  
    dxccView->setRowCount(0);

    QStringList headerqs;
    headerqs.clear();
    headerqs << tr("ID") << tr("Entity") << bandNames;
    dxccView->setHorizontalHeaderLabels(headerqs);

   //qDebug() << "DXCCStatusWidget::setBands: PRE-END" << endl;

    if (!_creating)
    {
        update();
    }

   //qDebug() << "DXCCStatusWidget::setBands: END" << endl;


}

void DXCCStatusWidget::setDefaultBands()
{
   //qDebug() << "DXCCStatusWidget::setDefaultBands" << endl;
    /*
     Default bands:
     160M    80M  40M  30M  20M  17M  15M  12M  10M  6M   4M   2M   70CM
     28     27  25  24  23  22  21  12  19  18  17  16  14
     */

    bandNames.clear();
    bandNames << "160M" << "80M" << "40M" << "30M" << "20M" << "17M" << "15M" << "12M" << "10M" << "6M" << "4M" << "2M" << "70CM";

    setBands(bandNames, true);

}


/*
void DXCCStatusWidget::slotSearchLineEditTextChanged()
{
    //qDebug() << "DXCCStatusWidget::slotSearchLineEditTextChanged: " << searchLineEdit->text() << endl;

}
*/

void DXCCStatusWidget::slotRefreshButtonClicked()
{
  //qDebug() << "DXCCStatusWidget::slotRefreshButtonClicked" << endl;
    //TODO: Define a way to show the status of the selected log or all the logs in the DB
    QStringList _bands = bandNames;
    setBands(_bands);
    //update();
}

void DXCCStatusWidget::refresh()
{
   //qDebug() << "DXCCStatusWidget::refresh" << endl;
    slotRefreshButtonClicked();
}

 void DXCCStatusWidget::setCurrentLog(const int _logN)
 {
    //qDebug() << "DXCCStatusWidget::setCurrentLog: " << QString::number(_logN) << endl;
     if (dataProxy->doesThisLogExist(_logN))
     {
         logNumber = _logN;
     }
     else
     {
         logNumber = -1;
     }
 }

 void DXCCStatusWidget::slotItemEntered(QTableWidgetItem  * item )
 {
    //qDebug() << "DXCCStatusWidget::slotItemEntered: " << ((item->data(0)).toString()).simplified() << endl;
    //qDebug() << "DXCCStatusWidget::slotItemEntered: " << item->text() << endl;
    QString tip;
    tip.clear();
    int row = -1;
    int column = -1;
    int bandi = -1;
    QString band = QString();

    if (item)
    {
        row = dxccView->row(item);
        column = dxccView->column(item);


        //qDebug() << "DXCCStatusWidget::slotItemEntered: row: " << QString::number(row) << endl;
        //qDebug() << "DXCCStatusWidget::slotItemEntered: hrow: " <<  dxccView->verticalHeaderItem(row)->text() << endl;
        row = (dxccView->item(row,0)->text()).toInt();
        //qDebug() << "DXCCStatusWidget::slotItemEntered: column: " << QString::number(column) << endl;
        if (column >= 2)
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
                tip = awards->getQSOofAward(row, bandi);
                //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip << endl;
                if (tip.length()>3)
                {
                    tip = "QSO: " + tip;
                }
                else
                {
                    tip = tr("Text TBD.");
                }

            }

        }
        else
        {
            tip = ""; // TODO define a text to be shown when no band column is selected (maybe Continent & bearing or a link to wikipedia or whatever!)
        }

        item->setToolTip(tip);
        //qDebug() << "DXCCStatusWidget::slotItemEntered: tip: " << tip << endl;


    }
    else
    {}

    //qDebug() << "DXCCStatusWidget::slotItemEntered: END " << endl;

 }

void DXCCStatusWidget::slotItemDoubleClicked(QTableWidgetItem  * item )
{
    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << ((item->data(0)).toString()).simplified() << endl;
    qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: " << item->text() << endl;

    int row = -1;
    int column = -1;
    int qsoId = -1;
    int bandi = -1;
    QString band = QString();

    if (item)
    {
        row = dxccView->row(item);
        column = dxccView->column(item);


        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: row: " << QString::number(row) << endl;
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: hrow: " <<  dxccView->verticalHeaderItem(row)->text() << endl;
        row = (dxccView->item(row,0)->text()).toInt();
        //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: column: " << QString::number(column) << endl;
        if (column >= 2)
        {
            if (item->text() == "-")
            {
                //tip = tr("Entity not worked in this band.");
            }
            else
            {
                band = dxccView->horizontalHeaderItem(column)->text();
                //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << band << endl;
                bandi = dataProxy->getIdFromBandName(band);
                //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: band: " << QString::number(bandi) << endl;
                qsoId = awards->getQSOIdofAward(row, bandi);
                //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: tip: " << tip << endl;
                if (qsoId>0)
                {
                    emit showQso(qsoId);
                }
            }
        }
        else
        {
            //tip = ""; // TODO define a text to be shown when no band column is selected (maybe Continent & bearing or a link to wikipedia or whatever!)
        }
    }
    else
    {}

    //qDebug() << "DXCCStatusWidget::slotItemDoubleClicked: END " << endl;
}
