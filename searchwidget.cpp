#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent)
{
    //qDebug() << "SearchWidget::SearchWidget"   << endl;
    searchBoxLineEdit = new QLineEdit;
    dataProxy = new DataProxy_SQLite;
    awards = new Awards;
    util = new Utilities;
    filemanager = new FileManager;

    world = new World;

    currentLog = -1;

   //qDebug() << "SearchWidget::SearchWidget: 00092" << endl;

    searchResultsTreeWidget = new QTreeWidget;
    searchResultsTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    searchResultsTreeWidget->setSortingEnabled(true);
    //searchResultsTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    searchResultsTreeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    searchResultsTreeWidget->setMouseTracking(true);

    searchBoxClearButton = new QPushButton(tr("&Clear"), this);
    searchBoxExportButton  = new QPushButton(tr("&Export Highlighted"), this);
    searchBoxSelectAllButton  = new QPushButton(tr("&Select All"), this);
    searchBoxReSearchButton = new QPushButton(tr("&Search"), this);
    searchAllRadioButton = new QRadioButton (tr("All"), this);

    stationCallSignShownInSearch = true;

    clear();

    createUI();
//    setDefaultData();
//    clear();
}

void SearchWidget::clear()
{
    searchBoxLineEdit->clear();
    searchResultsTreeWidget->clear();
    qslingNeeded = false;
    searchSelectAllClicked = false;
}

void SearchWidget::setShowCallInSearch(const bool _sh)
{
    stationCallSignShownInSearch = _sh;
}

void SearchWidget::setColors (const QString _newOne, const QString _needed, const QString _worked, const QString _confirmed, const QString _default)
{
    awards->setColors (_newOne, _needed, _worked, _confirmed, _default);
}

void SearchWidget::setVersion (const QString _version)
{
    filemanager->setVersion(_version);
}

void SearchWidget::createUI()
{
    searchBoxClearButton->setToolTip(tr("Clear the searches."));
    searchBoxExportButton->setToolTip(tr("Export the search result to an ADIF file."));
    searchBoxSelectAllButton->setToolTip(tr("Select/Unselect all the QSOs shown."));
    searchBoxReSearchButton->setToolTip(tr("Search in the log."));
    searchAllRadioButton->setToolTip(tr("Search in all logs."));
    searchBoxLineEdit->setToolTip(tr("Enter the QRZ to search for."));
    searchResultsTreeWidget->setToolTip(tr("Search results."));


    QStringList labels;
    if (stationCallSignShownInSearch)
    {
        labels << tr("QRZ") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("Station Callsign") << tr("ID") ;
        searchResultsTreeWidget->setColumnCount(8);
    }
    else
    {
       labels << tr("QRZ") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("ID") ;
       searchResultsTreeWidget->setColumnCount(7);
    }


    searchResultsTreeWidget->setHeaderLabels(labels);
    //QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
   (searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);


    searchResultsTreeWidget->clear();
    //searchResultsTreeWidget->collapseAll();
    searchResultsTreeWidget->setSortingEnabled(true);
    //searchResultsTreeWidget->setItemsExpandable(false);


    QHBoxLayout *dxUpRightSearchTopLayout = new QHBoxLayout;
    dxUpRightSearchTopLayout->addWidget(searchBoxLineEdit);
    dxUpRightSearchTopLayout->addWidget(searchAllRadioButton);

    QHBoxLayout *dxUpRightButtonsLayout = new QHBoxLayout;
    dxUpRightButtonsLayout->addWidget(searchBoxReSearchButton);
    dxUpRightButtonsLayout->addWidget(searchBoxClearButton);
    dxUpRightButtonsLayout->addWidget(searchBoxSelectAllButton);
    dxUpRightButtonsLayout->addWidget(searchBoxExportButton);

    QVBoxLayout *dxUpRightSearchTabLayout = new QVBoxLayout;
    dxUpRightSearchTabLayout->addLayout(dxUpRightSearchTopLayout);
    dxUpRightSearchTabLayout->addLayout(dxUpRightButtonsLayout);
    dxUpRightSearchTabLayout->addWidget(searchResultsTreeWidget);

    setLayout(dxUpRightSearchTabLayout);


    //connect(dataProxy, SIGNAL(qsoFound(QStringList)), this, SLOT(slotQsoFound(QStringList)) );

    connect(searchBoxLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSearchBoxTextChanged() ) );
    connect(searchBoxExportButton, SIGNAL(clicked()), this, SLOT(slotSearchExportButtonClicked() ) );
    connect(searchBoxClearButton, SIGNAL(clicked()), this, SLOT(slotSearchClearButtonClicked() ) );
    connect(searchBoxReSearchButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxReSearchButtonClicked() ) );
    connect(searchBoxSelectAllButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxSelectAllButtonClicked() ) );
    connect(searchResultsTreeWidget, SIGNAL(itemSelectionChanged( ) ), this, SLOT(slotSearchBoxSelectionChanged( ) ) );
    connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    connect(searchResultsTreeWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonSearch( const QPoint& ) ) );

}


void SearchWidget::slotDoubleClickSearch(QTreeWidgetItem * item, int)
{
    //qDebug() << "SearchWidget::slotDoubleClickSearch"  << endl;
    int number = -1;
    if (item){
        if (stationCallSignShownInSearch)
        {
            number = (item->text(7)).toInt();
        }
        else
        {
            number = (item->text(6)).toInt();
        }


        actionQSODoubleClicked(number);
    }
    else
    {}
}

void SearchWidget::slotSearchBoxTextChanged()
{
    //qDebug() << "SearchWidget::slotSearchBoxTextChanged: "  << searchBoxLineEdit->text() << endl;

    QString _id, _call, _dateTime, _band, _bandid, _mode, _qsltx, _qslrx, _stationcallsign, _dxcc;
    QStringList q;
    bool searchAll = searchAllRadioButton->isChecked();
    int i = -1;
    int cursorP = searchBoxLineEdit->cursorPosition();

    searchBoxLineEdit->setText((searchBoxLineEdit->text()).toUpper());

    if ((searchBoxLineEdit->text()).length() < 2)
    {
        searchResultsTreeWidget->clear();
        return;
    }
    searchResultsTreeWidget->clear();

    qslingNeeded = false; // If I am searching I am not longer looking for QSO to QSL

    QString theCall = searchBoxLineEdit->text();

    QSqlQuery query;
    QString queryString, aux;
    aux.clear();

    if ((theCall.startsWith("1")) || (theCall.startsWith("2")))
    {   // Fix a bug (or my knowledge of SQLite) to search Strings begining with 1 or 2
        // sqlite does not understand statements like SELECT call FROM log WHERE call LIKE '%1A%'
        aux = theCall + "%";

    }
    else
    {
        aux = "%" + theCall + "%";
    }

    if (searchAll)
    {
        queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, dxcc, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call LIKE '%1'").arg(aux);
    }
    else
    {
        queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, dxcc, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call LIKE '%1' AND lognumber='%2'").arg(aux).arg(currentLog);
    }
    aux.clear();


    //qDebug() << "SearchWidget::slotSearchBoxTextChanged: queryString"  << queryString << endl;
    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number());
    }

    QSqlRecord rec = query.record();
    int nameCol = -1;



    //qDebug() << "SearchWidget::slotSearchBoxTextChanged: queryString EXECUTED!"  << endl;
    QColor color;

    //QSqlRecord rec = query.record();
    QFont font;
    font.setBold(true);

    while (query.next())
    {
        if (query.isValid())
        {
            nameCol = rec.indexOf("call");
            _call= (query.value(nameCol)).toString();
            //nameCol = rec.indexOf("call");
            //_call= (query.value(0)).toString();

            nameCol = rec.indexOf("qso_date");
            _dateTime = (query.value(nameCol)).toString();
            //_dateTime = (query.value(1)).toString();

            nameCol = rec.indexOf("time_on");
            _dateTime = _dateTime + "-" +(query.value(nameCol)).toString();
            //_dateTime = _dateTime + "-" +(query.value(2)).toString();

            nameCol = rec.indexOf("bandid");
            _bandid = (query.value(nameCol)).toString();
            //qDebug() << "SearchWidget::slotSearchBoxTextChanged: band: " << QString::number((query.value(3)).toInt()) << endl;
            _band = dataProxy->getNameFromBandId(_bandid.toInt());
            //qDebug() << "SearchWidget::slotSearchBoxTextChanged: _band: " << _band << endl;

            nameCol = rec.indexOf("modeid");
            //qDebug() << "SearchWidget::slotSearchBoxTextChanged: mode: " << QString::number((query.value(nameCol)).toInt()) << endl;
            //_mode = dataProxy->getNameFromModeId((query.value(4)).toInt());
            _mode = dataProxy->getNameFromSubModeId((query.value(nameCol)).toInt());

            nameCol = rec.indexOf("dxcc");
            _dxcc= (query.value(nameCol)).toString();

            //_mode = query.value(4).toString();

            //qDebug() << "SearchWidget::slotSearchBoxTextChanged: mode(b) :  " << _mode << endl;

            //nameCol = rec.indexOf("qsl_sent");
            //_qsltx = (query.value(nameCol)).toString();
            nameCol = rec.indexOf("qsl_rcvd");

            _qsltx = (query.value(nameCol)).toString();
            if (_qsltx.length()<1)
            {
                _qsltx = "N";
            }

            nameCol = rec.indexOf("qsl_sent");
            _qslrx = (query.value(nameCol)).toString();
            if (_qslrx.length()<1)
            {
                _qslrx = "N";
            }
            if (stationCallSignShownInSearch)
            {
                nameCol = rec.indexOf("station_callsign");

                _stationcallsign = (query.value(nameCol)).toString();
                //if (_stationcallsign.length()<3)
                //{
                //    _stationcallsign = stationQRZ;
                //}
                nameCol = rec.indexOf("id");
                _id = (query.value(nameCol)).toString();
            }
            else
            {
                nameCol = rec.indexOf("id");
                _id = (query.value(nameCol)).toString();
            }

                q.clear();
                q << _dxcc << _bandid << _mode << QString::number(currentLog);



                //QColor color = Qt::red;

                //color = Qt::red;
                //TODO: Optimize the awards->getQRZDXStatusColor because is TOO slow
                color =  awards->getQRZDXStatusColor(q);
                //_mode = dataProxy->getSubModeFromId(_mode.toInt());
                //_mode = db->getModeNameFromNumber(_mode.toInt());
/*
    awards.getQRZDXStatusColor(const QStringList _qs);
    // Receives:  QStringList _qs;
    //_qs << QRZ << BandId << lognumber;
*/
                QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
                i = world->getQRZARRLId(_call);
                aux = world->getEntityName(i) + " - CQ: " + QString::number(world->getEntityCqz(i));
                item->setToolTip(0, aux);
                item->setToolTip(1, aux);
                item->setToolTip(2, aux);
                item->setToolTip(3, aux);
                item->setToolTip(4, aux);
                item->setToolTip(5, aux);
                item->setToolTip(6, aux);

                //item->setToolTip(0, world->getQRZEntityName(_call));
                item->setText(0, _call);
                item->setFont(0, font);
                item->setText(1, _dateTime);
                item->setText(2, _band);
               //qDebug() << "SearchWidget::slotSearchBoxTextChanged: mode(c) :  " << _mode << endl;
                item->setText(3, _mode);
                item->setText(4, _qslrx);
                item->setText(5, _qsltx);
                if (stationCallSignShownInSearch)
                {
                    item->setText(6, _stationcallsign);
                    item->setText(7, _id);
                    item->setToolTip(7, aux);

                }
                else
                {
                    item->setText(6, _id);
                }


            item->setForeground(0, QBrush(color));
        } // Closes if next.isValid
    } // Closes While
    (searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);
/*
    if (((theCall.at(cursorP-1)).isSpace()) && (cursorP>1))
    {
        ->setText(theCall.remove(cursorP-1, 1));
        cursorP--;
    }
    */

    searchBoxLineEdit->setCursorPosition(cursorP);





/*  // The following is a new implementation. It is still not used because it is toooo slow.

    int cursorP = searchBoxLineEdit->cursorPosition();
    bool searchAll = searchAllRadioButton->isChecked();

    int logToSearch = -1;
    if (searchAllRadioButton->isChecked())
    {

    }
    else
    {
        logToSearch = currentLog;
    }

    searchBoxLineEdit->setText((searchBoxLineEdit->text()).toUpper());

    if ((searchBoxLineEdit->text()).length() < 2)
    {
        searchResultsTreeWidget->clear();
        return;
    }
    searchResultsTreeWidget->clear();

    dataProxy->getFoundInLog(searchBoxLineEdit->text(), logToSearch );

    searchBoxLineEdit->setCursorPosition(cursorP);
*/


}

/*
//void SearchWidget::slotQsoFound(const QStringList _qso)
{
    // Each time a QSO is found in DataProxy emits a QStringList with the following format and this
    // function is just reading the data and adding that QSO to the Search treewidget
    // Format: QString: "field:value" |"field:value" |"field:value" |"field:value" |
    // Data arriving in _qso with the format:
    // call << qso_date << time_on << bandid << modeid << qsl_rcvd << qsl_sent << station_calsign << id

    //qDebug() << "SearchWidget::slotQsoFound: "  <<  _qso.at(0) << endl;


    QString _id, _call, _dateTime, _bandid, _modeid, _qsltx, _qslrx, _stationcallsign;
    QFont font;
    QStringList q;
    font.setBold(true);
    QString aux;
    //TODO: Check if the fields that are coming are correct (call, date, ...)

    aux = _qso.at(8);
    _id = aux.section(':', 1, 1);
    aux = _qso.at(0);
    _call = aux.section(':', 1, 1);;
    aux = _qso.at(1);
    _dateTime = aux.section(':', 1, 1);
    aux = _qso.at(2);
    _dateTime = _dateTime + "-" + aux.section(':', 1, 3); // the time has a ':' included so we nee dto take three sections
    aux = _qso.at(3);
    _bandid = aux.section(':', 1, 1);
    aux = _qso.at(4);
    _modeid = aux.section(':', 1, 1);
    aux = _qso.at(5);
    _qslrx = aux.section(':', 1, 1);
    aux = _qso.at(5);
    _qsltx = aux.section(':', 1, 1);
    aux = _qso.at(7);
    _stationcallsign = aux.section(':', 1, 1);


    q.clear();
    q << _call << _bandid << _modeid << QString::number(dataProxy->getLogNumberFromQSOId(_id.toInt()));
    //qDebug() << "SearchWidget::slotQsoFound: Calculated log"  <<  QString::number(dataProxy->getLogNumberFromQSOId(_id.toInt())) << endl;
    //qDebug() << "SearchWidget::slotQsoFound: Fixed log"  <<  currentLog << endl;
    QColor color =  awards->getQRZDXStatusColor(q);
    //qDebug() << "SearchWidget::slotQsoFound: Color1: "  <<  color.name() << endl;


    QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
    int i = world->getQRZARRLId(_call);
    aux = world->getEntityName(i) + " - CQ: " + QString::number(world->getEntityCqz(i));
    item->setToolTip(0, aux);
    item->setToolTip(1, aux);
    item->setToolTip(2, aux);
    item->setToolTip(3, aux);
    item->setToolTip(4, aux);
    item->setToolTip(5, aux);
    item->setToolTip(6, aux);

    item->setText(0, _call);
    item->setFont(0, font);
    item->setText(1, _dateTime);
    item->setText(2, dataProxy->getNameFromBandId(_bandid.toInt()));
   //qDebug() << "SearchWidget::slotSearchBoxTextChanged: mode(c) :  " << _mode << endl;
    item->setText(3, dataProxy->getNameFromModeId(_modeid.toInt()));
    item->setText(4, _qslrx);
    item->setText(5, _qsltx);
    if (stationCallSignShownInSearch)
    {
        item->setText(6, _stationcallsign);
        item->setText(7, _id);
        item->setToolTip(7, aux);

    }
    else
    {
        item->setText(6, _id);
    }

    (searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);
    item->setForeground(0, QBrush(color));
    //qDebug() << "SearchWidget::slotQsoFound: Color2: "  <<  color.name() << endl;
}
*/
void SearchWidget::setCurrentLog(const int _log)
{
    currentLog = _log;
}

void SearchWidget::slotSearchClearButtonClicked()
{
    //qDebug() << "SearchWidget::slotSearchClearButtonClicked: " << endl;
    searchResultsTreeWidget->clear();
    searchBoxLineEdit->clear();
    searchSelectAllClicked = false;
    qslingNeeded = false;
}

void SearchWidget::slotSearchBoxSelectAllButtonClicked()
{
    //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: " << endl;
    if (searchSelectAllClicked)
    {
        //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: UN-SELECTING" << endl;
        searchSelectAllClicked = false;
        searchResultsTreeWidget->clearSelection();
        searchBoxSelectAllButton->setText(tr("&Select All"));


    }
    else
    {
        //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: SELECTING" << endl;
        searchSelectAllClicked = true;
        searchResultsTreeWidget->selectAll();
        searchBoxSelectAllButton->setText(tr("&Clear selection"));

    }

}

void SearchWidget::slotSearchBoxReSearchButtonClicked()
{
    //qDebug() << "SearchWidget::slotSearchBoxReSearchButtonClicked: "  << endl;
    slotSearchBoxTextChanged();
}

void SearchWidget::slotSearchBoxSelectionChanged()
{// Detects when a selection has been done in the search box and changes
 // The button to clear selection
    //qDebug() << "SearchWidget::slotSearchBoxSelectionChanged: "  << endl;

    if ((searchResultsTreeWidget->selectedItems()).size() > 0 )
    {
        searchBoxSelectAllButton->setText(tr("&Clear selection"));
        searchSelectAllClicked = true;
    }
    else
    {
        searchBoxSelectAllButton->setText(tr("&Select All"));
        searchSelectAllClicked = false;
    }

/*

    if (searchSelectAllClicked)
    {
        //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: UN-SELECTING" << endl;
        searchSelectAllClicked = false;
        searchResultsTreeWidget->clearSelection();
        searchBoxSelectAllButton->setText(tr("&Select All"));


    }
    else
    {
        //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: SELECTING" << endl;
        searchSelectAllClicked = true;
        searchResultsTreeWidget->selectAll();
        searchBoxSelectAllButton->setText(tr("&Clear selection"));

    }

*/

}

void SearchWidget::slotSearchExportButtonClicked()
{
    //qDebug() << "SearchWidget::slotSearchExportButtonClicked: " << endl;
    // MARK QSOs
    // SAVE MARKED QSOs TO FILE
    // UNMARK QSOs
    bool itemsSelected = false;
    int _qsoId = 0;
    int i = 0;
    QString stringQuery;
    QSqlQuery query;
    QTreeWidgetItem *item = searchResultsTreeWidget->topLevelItem(i);
    bool sqlOK;

    while (i <= searchResultsTreeWidget->topLevelItemCount() )
    {
        item = searchResultsTreeWidget->topLevelItem(i);
        if (item == 0)
        {
            i = searchResultsTreeWidget->topLevelItemCount() + 1;
            //qDebug() << "SearchWidget::slotSearchExportButtonClicked: ITEM = 0" << endl;
        }
        else
        {
            if (stationCallSignShownInSearch)
            {
                _qsoId = ((item)->text(7)).toInt();
            }
            else
            {
                _qsoId = ((item)->text(6)).toInt();
            }

            if ((item)->isSelected())
            {
                stringQuery = QString("UPDATE log SET marked = 'X' WHERE id='%1'").arg(_qsoId);
                sqlOK = query.exec(stringQuery);
                if (!sqlOK)
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number());
                }
                itemsSelected = true;
                //TODO: Prepare this while/query execution
                i++;
                //qDebug() << "SearchWidget::slotSearchExportButtonClicked: ITEM MARKED: " << QString::number(_qsoId) << endl;
            }
            else
            {
                stringQuery = QString("UPDATE log SET marked = 'N' WHERE id='%1'").arg(_qsoId);
                sqlOK = query.exec(stringQuery);
                if (!sqlOK)
                {
                    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number());
                }
                //TODO: Prepare this while/query execution
                i++;
                //qDebug() << "SearchWidget::slotSearchExportButtonClicked: ITEM NOT MARKED: " << QString::number(_qsoId) << endl;
            }
        }
    }

    if (itemsSelected)
    {
        //qDebug() << "SearchWidget::slotSearchExportButtonClicked: to Ask filename" << endl;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                   util->getHomeDir(),
                                   "ADIF (*.adi *.adif)");
        filemanager->adifLogExportMarked(fileName);
        //qDebug() << "SearchWidget::slotSearchExportButtonClicked: to call save file" << endl;
        dataProxy->unMarkAllQSO();
    }
    else
    {
        //No items were selected
    }

    //qDebug() << "SearchWidget::slotSearchExportButtonClicked: unmarking..." << endl;

}















void SearchWidget::slotRighButtonSearch(const QPoint& pos)
{
    //qDebug() << "SearchWidget::slotRighButtonSearch"  << endl;

    QTreeWidgetItem *item = searchResultsTreeWidget->itemAt(pos);
    int _qsoID = 0;

    if (item)
    {
        //qDebug() << "SearchWidget::slotRighButtonSearch ITEM=true"  << endl;
        // 6 is the column in the searchResultsTreeWidget where the id is saved
        if (stationCallSignShownInSearch)
        {
            //qDebug() << "SearchWidget::slotRighButtonSearch stationCallSignShownInSearch = true"  << endl;
            _qsoID = ((item)->text(7)).toInt();
            //qDebug() << "SearchWidget::slotRighButtonSearch QSO1: " << QString::number(_qsoID)  << endl;
        }
        else
        {
            //qDebug() << "SearchWidget::slotRighButtonSearch stationCallSignShownInSearch = false"  << endl;
            _qsoID = ((item)->text(6)).toInt();
            //qDebug() << "SearchWidget::slotRighButtonSearch QSO2: " << QString::number(_qsoID)  << endl;
        }
        //qDebug() << "SearchWidget::slotRighButtonSearch QSO: " << QString::number(_qsoID)  << endl;
        showMenuRightButtonSearchCreateActions();
        //qDebug() << "SearchWidget::slotRighButtonSearch -05"   << endl;
        righButtonSearchMenu(_qsoID);
        //qDebug() << "SearchWidget::slotRighButtonSearch -06"   << endl;
    }else
    {
        //qDebug() << "SearchWidget::slotRighButtonSearch ITEM=false"  << endl;
        return;
    }

    //qDebug() << "SearchWidget::slotRighButtonSearch: "  << QString::number(_qsoID) << endl;

}

void SearchWidget::righButtonSearchMenu(const int trow)
{
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu:  " << QString::number(trow) << endl;


    bool qslReceived = dataProxy->isQSLReceived(trow);
    bool qslSent = dataProxy->isQSLSent(trow);

    QMenu menu(this);

    menu.addAction(delQSOFromSearchAct);
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -03"  << endl;
    delQSOFromSearchAct->setData(trow);
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -04"  << endl;

    menu.addAction(qsoToEditFromSearchAct);
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -05"  << endl;
    qsoToEditFromSearchAct->setData(trow);
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -06"  << endl;

    menu.addSeparator();
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -07"  << endl;
    if (qslSent)
    {
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSLSent"  << endl;
    }
    else
    {
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Sent"  << endl;
        QMenu *menuSentQsl = menu.addMenu(tr("QSL Send"));
        menuSentQsl->addAction(qslSentViaBureauFromSearchAct);
        menuSentQsl->addAction(qslSentViaDirectFromSearchAct);
        menuSentQsl->addAction(qslSentRequestedAct);
        if (!qslReceived)
        {
            menuSentQsl->addAction(qslSentViaBureauMarkRcvReqFromSearchAct);
            menuSentQsl->addAction(qslSentViaDirectMarkRcvReqFromSearchAct);
            qslSentViaBureauMarkRcvReqFromSearchAct->setData(trow);
            qslSentViaDirectMarkRcvReqFromSearchAct->setData(trow);
        }
        qslSentViaBureauFromSearchAct->setData(trow);
        qslSentViaDirectFromSearchAct->setData(trow);
        qslSentRequestedAct->setData(trow);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -end qsl not sent"  << endl;

    }

    if (qslReceived)
    {
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSLRec"  << endl;
    }
    else
    {
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec"  << endl;
        QMenu *menuRecQsl = menu.addMenu(tr("QSL Rcvd"));
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 01"  << endl;
        menuRecQsl->addAction(qslRecViaBureauFromSearchAct);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 02"  << endl;
        menuRecQsl->addAction(qslRecViaBureauMarkReqFromSearchAct);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 03"  << endl;
        menuRecQsl->addAction(qslRecViaDirectFromSearchAct);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 04"  << endl;
        menuRecQsl->addAction(qslRecViaDirectMarkReqFromSearchAct);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 05"  << endl;
        menuRecQsl->addAction(qslRecRequestedAct);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 06"  << endl;

        qslRecViaBureauFromSearchAct->setData(trow);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 07"  << endl;
        qslRecViaBureauMarkReqFromSearchAct->setData(trow);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 08"  << endl;
        qslRecViaDirectFromSearchAct->setData(trow);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 09"  << endl;
        qslRecViaDirectMarkReqFromSearchAct->setData(trow);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -QSL Not Rec - 10"  << endl;
        qslRecRequestedAct->setData(trow);
        //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -end qsl not rec"  << endl;
    }
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -end qsl"  << endl;
    menu.exec(QCursor::pos());
    //qDebug() << "SearchWidget::slotshowRighButtonSearchMenu: -END"  << endl;
}


void SearchWidget::showMenuRightButtonSearchCreateActions()
{
 //qDebug() << "SearchWidget::showMenuRightButtonSearchCreateActions" << endl;

    delQSOFromSearchAct = new QAction(tr("&Delete"), this);
    delQSOFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_D);
    delQSOFromSearchAct->setStatusTip(tr("Delete a QSO"));
    connect(delQSOFromSearchAct, SIGNAL(triggered()), this, SLOT(slotQsoDeleteFromSearch()));

    qsoToEditFromSearchAct = new QAction(tr("&Edit QSO"), this);
    qsoToEditFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_E);
    qsoToEditFromSearchAct->setStatusTip(tr("Edit this QSO"));
    connect(qsoToEditFromSearchAct, SIGNAL(triggered()), this, SLOT(slotQSOToEditFromSearch()));

    qslSentViaBureauFromSearchAct = new QAction(tr("Via &bureau"), this);
    qslSentViaBureauFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_B);
    qslSentViaBureauFromSearchAct->setStatusTip(tr("Send this QSL via bureau"));
    connect(qslSentViaBureauFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauFromSearch() ));

    qslSentViaDirectFromSearchAct = new QAction(tr("D&irect"), this);
    qslSentViaDirectFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_I);
    qslSentViaDirectFromSearchAct->setStatusTip(tr("Send this QSL via direct"));
    connect(qslSentViaDirectFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectFromSearch()   ));

    qslSentRequestedAct = new QAction(tr("&Request my QSL"), this);
    qslSentRequestedAct->setShortcut(Qt::CTRL + Qt::Key_R);
    qslSentRequestedAct->setStatusTip(tr("Mark my QSL as requested"));
    connect(qslSentRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLSentMarkAsRequested()   ));

    qslSentViaDirectMarkRcvReqFromSearchAct = new QAction(tr("Via Direct && mark DX QSL as requested"), this);
    qslSentViaDirectMarkRcvReqFromSearchAct->setStatusTip(tr("Send this QSL via direct & mark DX QSL as requested"));
    connect(qslSentViaDirectMarkRcvReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectMarkDXReqFromSearch() ));

    qslSentViaBureauMarkRcvReqFromSearchAct = new QAction(tr("Via Bureau && mark DX QSL as requested"), this);
    qslSentViaBureauMarkRcvReqFromSearchAct->setStatusTip(tr("Send this QSL via bureau & mark DX QSL as requested"));
    connect(qslSentViaBureauMarkRcvReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureuMarkDXReqFromSearch() ));


    qslRecRequestedAct = new QAction(tr("&Request the QSL"), this);
    //qslSentRequestedAct->setShortcut(Qt::CTRL + Qt::Key_R);
    qslRecRequestedAct->setStatusTip(tr("Mark the QSL as requested"));
    connect(qslRecRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLRecMarkAsRequested()   ));


    qslRecViaBureauMarkReqFromSearchAct = new QAction(tr("Via bureau && mark my QSL as requested"), this);
    qslRecViaBureauMarkReqFromSearchAct->setStatusTip(tr("QSL received via bureau & mark my QSL as requested"));
    connect(qslRecViaBureauMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauMarkReqFromSearch() ));

    qslRecViaBureauFromSearchAct = new QAction(tr("Via bureau"), this);
    qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via bureau"));
    //qslRecViaBureauFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(qslRecViaBureauFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauFromSearch() ));

    qslRecViaDirectMarkReqFromSearchAct = new QAction(tr("Direc&t && mark as my QSL requested"), this);
    qslRecViaDirectMarkReqFromSearchAct->setStatusTip(tr("QSL received via direct & mark my QSL as requested"));
    connect(qslRecViaDirectMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectMarkReqFromSearch() ));

    qslRecViaDirectFromSearchAct = new QAction(tr("Direc&t"), this);
    qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via direct"));
    //qslRecViaDirectFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_T);
    connect(qslRecViaDirectFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectFromSearch() ));
}


void SearchWidget::slotQSLSentViaBureuMarkDXReqFromSearch()
{
    //qDebug() << "slotQSLSentViaBureuMarkDXReqFromSearch: " << (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt();

    dataProxy->qslSentViaBureau(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));
    dataProxy->qslRecAsRequested(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));


    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.

}

void SearchWidget::slotQSLSentViaDirectMarkDXReqFromSearch()
{
    //qDebug() << "slotQSLSentViaDirectMarkDXReqFromSearch: " << (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toInt();

    dataProxy->qslSentViaDirect(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));
    dataProxy->qslRecAsRequested(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));


    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}

void SearchWidget::slotQSLSentViaBureauFromSearch()
{
   // //qDebug() << "SearchWidget::slotQSLSentViaBureauFromSearch: " << (qslSentViaBureauFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslSentViaBureauFromSearchAct->data()).toInt();

    dataProxy->qslSentViaBureau(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));

    //logWindow->qslSentViaBureau(_qsoId);
    //qslSentViaBureau(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}

void SearchWidget::slotQSLSentViaDirectFromSearch()
{
    //qDebug() << "SearchWidget::slotQSLSentViaDirectFromSearch: " << (qslSentViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((qslSentViaDirectFromSearchAct->data()).toInt());
    dataProxy->qslSentViaDirect(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }

    //qslSentViaDirect(_qsoId);

}


void SearchWidget::slotQSLSentMarkAsRequested()
{
   // bool qslSentAsRequested(const int _qsoId, const QString _updateDate);

    int _qsoId = (qslSentRequestedAct->data()).toInt();
    dataProxy->qslSentAsRequested(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}

void SearchWidget::slotQSLRecMarkAsRequested()
{
    int _qsoId = (qslRecRequestedAct->data()).toInt();
    dataProxy->qslRecAsRequested(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}


void SearchWidget::slotQSLRecViaBureauFromSearch()
{
    //qDebug() << "SearchWidget::slotQSLRecViaBureauFromLog: " << "- Id = " << QString::number( ((logModel->index( ( (qslRecViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslRecViaBureauFromSearchAct->data()).toInt();
    //logWindow->qslRecViaBureau(_qsoId);
    dataProxy->qslRecViaBureau(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));

    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}

void SearchWidget::slotQSLRecViaBureauMarkReqFromSearch()
{
    //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: " << "- Id = " << QString::number( ((logModel->index( ( (qslRecViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslRecViaBureauMarkReqFromSearchAct->data()).toInt();
    qslRecViaBureauMarkReq(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }

}


void SearchWidget::slotQSLRecViaDirectFromSearch()
{
    //qDebug() << "SearchWidget::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslRecViaDirectFromSearchAct->data()).toInt();
    //logWindow->qslRecViaDirect(_qsoId);
    dataProxy->qslRecViaDirect(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}

void SearchWidget::slotQSLRecViaDirectMarkReqFromSearch()
{
    //qDebug() << "SearchWidget::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslRecViaDirectMarkReqFromSearchAct->data()).toInt();
    qslRecViaDirectMarkReq(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}






void SearchWidget::qslRecViaBureauMarkReq(const int _qsoId)
{
   // //qDebug() << "SearchWidget::qslRecViaBureau: " << QString::number(_qsoId) << "/" << (QDate::currentDate()).toString("yyyy/MM/dd") << endl;
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);
    dataProxy->qslRecViaBureau(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"), true);
    awards->setAwards(_qsoId);   //Update the DXCC award status
    emit logRefresh();
    emit updateAwards();

}


void SearchWidget::qslRecViaDirectMarkReq(const int _qsoId)
{
    //qDebug() << "SearchWidget::qslRecViaDirect: " << QString::number(_qsoId) << endl;

    dataProxy->qslRecViaDirect(_qsoId, (QDate::currentDate()).toString("yyyy/MM/dd"), true);
    awards->setAwards(_qsoId);
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);

    emit updateAwards();
}



void SearchWidget::slotQSOToEditFromSearch()
{
    //qDebug() << "slotQSOToEditFromSearch: " << (qsoToEditFromSearchAct->data()).toString() << endl;

    actionQSODoubleClicked((qsoToEditFromSearchAct->data()).toInt());
}

void SearchWidget::slotQsoDeleteFromSearch()
{
    //qDebug() << "SearchWidget::slotQsoDeleteFromSearch: " << (delQSOFromSearchAct->data()).toString() << endl;


    int QSOid = (delQSOFromSearchAct->data()).toInt();
    //int x = -1;

    QString _qrz = dataProxy->getCallFromId(QSOid);
    if (_qrz.length()>=3)
    {

        QString message = QString(tr("You have requested to delete the QSO with:") + (" %1")).arg(_qrz);

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(message);
        msgBox.setInformativeText(tr("Are you sure?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Yes:

            if(dataProxy->deleteQSO(QSOid))
            {
                emit actionQSODelete(QSOid);

                emit logRefresh();
                if(qslingNeeded)
                {
                    searchToolNeededQSLToSend();
                }
                else
                {
                    slotSearchBoxTextChanged();
                }
                //dxccStatusWidget->refresh();
                //awards->recalculateAwards();
                emit updateAwards();

            }
            else
            {
                //TODO: The QSO could not be removed...
            }

            break;
            case QMessageBox::No:
              // No was clicked
            break;
            default:
              // should never be reached
            break;
        }

    }
    else
    {
         // TODO: The QSO to be removed was not found in the log
    }
}

void SearchWidget::slotToolSearchQSL(const int actionQSL)
{
    //qDebug() << "SearchWidget::slotToolSearchQSL: " << QString::number(actionQSL) << " - LogNumber: " << QString::number(currentLog) << endl;
    // 2 means QSL_RCVD = 'R'
    QString stringQuery = QString();
    QString message = QString();
    QString aux = QString();
    int i = -1;


    switch (actionQSL)
    {
        case 0://void searchToolNeededQSLToSend();
         //aux = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(currentLog);
         //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 0" << endl;
         stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, log.id FROM log JOIN awarddxcc ON awarddxcc.qsoid=log.id WHERE awarddxcc.confirmed='0' AND log.qsl_sent!='Y' AND log.qsl_sent!='Q' AND log.qsl_sent!='R' AND log.lognumber='%1'").arg(currentLog);
            message = tr("Needed QSO to send the QSL");
            qslingNeeded = true;
            requestBeingShown();
            //dxUpRightTab->setCurrentIndex(2);
        break;
        case 1:
        //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 1" << endl;
            stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, dxcc, station_callsign, id FROM log  WHERE qsl_sent=='R' AND lognumber='%1'").arg(currentLog);
            message = tr("My QSL requested to be sent");
        break;
        case 2://void slotToolSearchNeededQSLPendingToReceive();
        //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 2" << endl;
        stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, dxcc, station_callsign, log.id FROM log WHERE lognumber='%1' AND ( (qsl_sent='Y' AND qsl_rcvd!='Y' AND qsl_rcvd!='I') OR qsl_rcvd='R')").arg(currentLog);
            message = tr("DX QSL pending to be received");
        break;
    case 3://void slotToolSearchNeededQSLRequested()
        //qDebug() << "SearchWidget::slotToolSearchQSL: CASE 3" << endl;
        stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, dxcc, station_callsign, log.id FROM log WHERE lognumber='%1' AND  qsl_rcvd='R'").arg(currentLog);
        message = tr("DX QSL pending to be received");
    break;
        default:
        //qDebug() << "SearchWidget::slotToolSearchQSL: CASE DEFAULT" << endl;
        // should never be reached
            return;
        break;
    }

    int nameCol = -1;
    QString _call, _dateTime, _band, _mode, _freq, _qsltx, _qslrx, _id, _stationcallsign, _dxcc;
    QFont font;
    font.setBold(true);
    QColor color;
    QStringList q;

    searchResultsTreeWidget->clear();
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record();

    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number());
        //qDebug() << "SearchWidget::slotToolSearchQSL: Query ERROR" << endl;
     //TODO: Control the error!!
    }

    else
    {
        emit toStatusBar(message); // updating the status bar
        while(query.next())
        {
        if (query.isValid())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
            //aux = world->getQRZEntityName(_call);
            i = world->getQRZARRLId(_call);
            aux = world->getEntityName(i) + " - CQ: " + QString::number(world->getEntityCqz(i));
            item->setToolTip(0, aux);
            item->setToolTip(1, aux);
            item->setToolTip(2, aux);
            item->setToolTip(3, aux);
            item->setToolTip(4, aux);
            item->setToolTip(5, aux);
            item->setToolTip(6, aux);

            nameCol = rec.indexOf("call");
            _call= (query.value(nameCol)).toString();
            nameCol = rec.indexOf("qso_date");
            _dateTime = (query.value(nameCol)).toString();
            nameCol = rec.indexOf("time_on");
            _dateTime = _dateTime + "-" +(query.value(nameCol)).toString();

            nameCol = rec.indexOf("bandid");
            _freq = (query.value(nameCol)).toString();
            _band = dataProxy->getNameFromBandId(_freq.toInt());
            //_band = db->getBandNameFromNumber( _freq.toInt() );

            nameCol = rec.indexOf("modeid");
            _mode = dataProxy->getSubModeFromId((query.value(nameCol)).toInt());

            nameCol = rec.indexOf("dxcc");
            _dxcc= (query.value(nameCol)).toString();

            //qDebug() << "SearchWidget::slotToolSearchQSL: Mode: " << _mode << endl;
            //qDebug() << "SearchWidget::slotToolSearchQSL: mode " << QString::number((query.value(nameCol)).toInt()) << endl;

            nameCol = rec.indexOf("qsl_sent");
            _qsltx = (query.value(nameCol)).toString();
            if (_qsltx.length()<1)
            {
                _qsltx = "N";
            }

            nameCol = rec.indexOf("qsl_rcvd");
            _qslrx = (query.value(nameCol)).toString();
            if (_qslrx.length()<1)
            {
                _qslrx = "N";
            }

            if (stationCallSignShownInSearch)
            {
                //qDebug() << "SearchWidget::slotToolSearchQSL: stationCallSign "<< endl;

                nameCol = rec.indexOf("station_callsign");
                if (((query.value(nameCol)).toString()).length()>=3)
                {
                    _stationcallsign = (query.value(nameCol)).toString();
                }
                else
                {
                    _stationcallsign.clear();
                }

            }

            nameCol = rec.indexOf("id");
            _id= (query.value(nameCol)).toString();

            q.clear();
            q << _dxcc << _freq << _mode << QString::number(currentLog);

            color = awards->getQRZDXStatusColor(q);

            item->setText(0, _call);
            item->setText(1, _dateTime);
            item->setText(2, _band);
            item->setText(3, _mode);
            item->setText(4, _qsltx);
            item->setText(5, _qslrx);
            if (stationCallSignShownInSearch)
            {
                item->setText(6, _stationcallsign);
                item->setText(7, _id);
                item->setToolTip(7, aux);
            }
            else
            {
                item->setText(6, _id);
            }

            item->setForeground(0, QBrush(color));
        }
        else
        {
        //TODO: Check what is happening here!
        }
    }
        //qslingNeeded = true;
        requestBeingShown();
        //dxUpRightTab->setCurrentIndex(2);
    }
}

void SearchWidget::searchToolNeededQSLToSend()
{
   //qDebug() << "SearchWidget::slotToolSearchQSLToSend - TO PREPARE THE QUERY and optimize the function" << endl;
    slotToolSearchQSL(0);
}

void SearchWidget::slotToolSearchRequestedQSLToSend()
{
   //qDebug() << "SearchWidget::slotToolSearchRequestedQSLToSend" << endl;
    slotToolSearchQSL(1);
}

void SearchWidget::slotToolSearchNeededQSLPendingToReceive()
{
    //qDebug() << "SearchWidget::slotToolSearchNeededQSLPendingToReceive "  << endl;
    // QSL RCVD requested or
    // QSL Sent Y and qsl_rcvd!=Y AND qsl_rcvd!=I
    //select call from log where (qsl_sent='Y' and qsl_rcvd!='Y' and qsl_rcvd!='I') OR
    //QString stringQuery = QString("SELECT call FROM log where (qsl_sent='Y' AND qsl_rcvd!='Y' AND qsl_rcvd!='I') OR qsl_rcvd='R'");
    slotToolSearchQSL(2);
}

void SearchWidget::slotToolSearchNeededQSLRequested()
{ // Requested DX QSL
    slotToolSearchQSL(3);
}
