/***************************************************************************
                          searchwidget.cpp  -  description
                             -------------------
    begin                : jul 2017
    copyright            : (C) 2017 by Jaime Robles
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

#include "searchwidget.h"

SearchWidget::SearchWidget(DataProxy_SQLite *dp, QWidget *parent) :
    QWidget(parent)
{
    //qDebug() << "SearchWidget::SearchWidget"   << endl;
    searchBoxLineEdit = new QLineEdit;
    dataProxy = dp;
    delayInputTimer = new QTimer;
    searchWindow = new SearchWindow(dataProxy, this);

    awards = new Awards(dataProxy, Q_FUNC_INFO);
    util = new Utilities;
    filemanager = new FileManager(dataProxy);

    world = new World(dataProxy, Q_FUNC_INFO);
    mainStationCallsign = QString();

    currentLog = -1;

      //qDebug() << "SearchWidget::SearchWidget: 00092" << endl;
    stationCallsignComboBox = new QComboBox;

    searchBoxClearButton = new QPushButton(tr("&Clear"), this);
    searchBoxExportButton  = new QPushButton(tr("&Export Highlighted"), this);
    searchBoxSelectAllButton  = new QPushButton(tr("&Select All"), this);
    searchBoxReSearchButton = new QPushButton(tr("&Search"), this);
    searchAllRadioButton = new QRadioButton (tr("All logs"), this);

    stationCallSignShownInSearch = true;

    clear();
    createUI();
    //qDebug() << "SearchWidget::SearchWidget - END"   << endl;
}

SearchWidget::~SearchWidget()
{

}

void SearchWidget::clear()
{
    searchBoxLineEdit->clear();
    searchWindow->clear();
    //qslingNeeded = false;
    setNeedingQSL(false);
    searchSelectAllClicked = false;
    lastSearch = QString();
    callFilter = QString();
    stationCallsignFilter = QString();
    currentLogFilter = QString();
    setModelFilter();
}

void SearchWidget::setCallToSearch (const QString _st)
{
    //qDebug() << Q_FUNC_INFO << " : " << _st;
    searchBoxLineEdit->setText(_st);
}

void SearchWidget::setShowCallInSearch(const bool _sh)
{
    stationCallSignShownInSearch = _sh;
    searchWindow->setStationCallsignInHeader(stationCallSignShownInSearch);
}



void SearchWidget::setVersion (const QString _version)
{
    filemanager->setVersion(_version);
}

void SearchWidget::showQSOs(QList<int> qsoIdList)
{
       //qDebug() << "SearchWidget::showQSOs received QSOs: " << QString::number(qsoIdList.length()) << endl;


    if (qsoIdList.length()<0)
    {
        return;
    }
    //QString _call, _dateTime, _band, _mode, _freq, _qsltx, _qslrx, _id, _stationcallsign, _dxcc;
       //qDebug() << "SearchWidget::showQSOs query: : 01"  << endl;
    int i = 0;

    QString filter;
    filter = QString();

    while ( (qsoIdList.at(i)<= 0) && (i<qsoIdList.length()) )
    {
        i++;
    }

       //qDebug() << "SearchWidget::showQSOs query: : 02 - :" << QString::number(i)  << endl;
    filter = QString("id = '%1'").arg(qsoIdList.at(i));

    i++;
       //qDebug() << "SearchWidget::showQSOs query: : 03 - :" << QString::number(i)  << endl;

    for (int j=i; j<qsoIdList.length(); ++j)
    {
        filter = filter + QString(" OR id = '%1'").arg(qsoIdList.at(j));
    }
       //qDebug() << "SearchWidget::showQSOs query: : " << queryString << endl;

    searchWindow->setFilterString(filter);

}


void SearchWidget::setStationCallsign(const QString &_st)
{
    if (util->isValidCall(_st))
    {
        mainStationCallsign = _st;
        selectStationCallSign();
    }
}

void SearchWidget::selectStationCallSign()
{
    int index = stationCallsignComboBox->findText(mainStationCallsign);
    if (index>=0)
    {
        stationCallsignComboBox->setCurrentIndex(index);
    }
}

void SearchWidget::createUI()
{
    searchBoxClearButton->setToolTip(tr("Clear the searches."));
    searchBoxExportButton->setToolTip(tr("Export the search result to an ADIF file."));
    searchBoxSelectAllButton->setToolTip(tr("Select/Unselect all the QSOs shown."));
    searchBoxReSearchButton->setToolTip(tr("Search in the log."));
    searchAllRadioButton->setToolTip(tr("Search in all logs."));
    searchBoxLineEdit->setToolTip(tr("Enter the callsign to search for. Enter '*' to show all the QSOs... it may be slow in big logs!"));

    stationCallsignComboBox->setToolTip(tr("Select the Station Callsign used to do this QSO."));

    fillStationCallsignComboBox();
    setShowCallInSearch(stationCallSignShownInSearch);

    //searchResultsTreeWidget->setHeaderLabels(labels);
    //(searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);
    //searchResultsTreeWidget->clear();
    //searchResultsTreeWidget->setSortingEnabled(true);


    searchWindow->createlogPanel(currentLog);
    //searchWindow->refresh();

    QHBoxLayout *dxUpRightSearchTopLayout = new QHBoxLayout;
    dxUpRightSearchTopLayout->addWidget(searchBoxLineEdit);
    dxUpRightSearchTopLayout->addWidget(stationCallsignComboBox);
    dxUpRightSearchTopLayout->addWidget(searchAllRadioButton);

    QHBoxLayout *dxUpRightButtonsLayout = new QHBoxLayout;
    dxUpRightButtonsLayout->addWidget(searchBoxReSearchButton);
    dxUpRightButtonsLayout->addWidget(searchBoxClearButton);
    dxUpRightButtonsLayout->addWidget(searchBoxSelectAllButton);
    dxUpRightButtonsLayout->addWidget(searchBoxExportButton);

    QVBoxLayout *dxUpRightSearchTabLayout = new QVBoxLayout;
    dxUpRightSearchTabLayout->addLayout(dxUpRightSearchTopLayout);
    dxUpRightSearchTabLayout->addLayout(dxUpRightButtonsLayout);
    //dxUpRightSearchTabLayout->addWidget(searchResultsTreeWidget);
    dxUpRightSearchTabLayout->addWidget(searchWindow);

    setLayout(dxUpRightSearchTabLayout);

    //connect(dataProxy, SIGNAL(qsoFound(QStringList)), this, SLOT(slotQsoFound(QStringList)) );

    connect(searchBoxLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotStartDelayInputTimer() ) );
    connect(delayInputTimer, SIGNAL(timeout()), this, SLOT(slotDelayInputTimedOut()) );
    //connect(searchBoxLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSearchBoxTextChanged() ) );
    connect(searchBoxExportButton, SIGNAL(clicked()), this, SLOT(slotSearchExportButtonClicked() ) );
    connect(searchBoxClearButton, SIGNAL(clicked()), this, SLOT(slotSearchClearButtonClicked() ) );
    connect(searchBoxReSearchButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxReSearchButtonClicked() ) );
    connect(searchBoxSelectAllButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxSelectAllButtonClicked() ) );
    //connect(searchResultsTreeWidget, SIGNAL(itemSelectionChanged( ) ), this, SLOT(slotSearchBoxSelectionChanged( ) ) );
    //connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    //connect(searchResultsTreeWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRightButtonSearch( const QPoint& ) ) );
    connect(stationCallsignComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(slotStationCallsignChanged() ) ) ;
    connect(searchAllRadioButton, SIGNAL(toggled(bool)), this, SLOT(slotRadioButtonToggled() ) ) ;

    connect(searchWindow, SIGNAL( actionQSODoubleClicked(int)), this, SLOT(slotQSOToEditFromSearch(int)));
    connect(searchWindow, SIGNAL( actionDeleteQSO(int)), this, SLOT( slotQsoDeleteFromSearch(int) ));
    connect(searchWindow, SIGNAL( updateSearchLineEdit()), this, SLOT( slotSearchBoxTextChanged() ));
    connect(searchWindow, SIGNAL( requestFocus()), this, SLOT( slotRequestFocus() ));

}


void SearchWidget::slotRequestFocus()
{
    emit requestBeingShown();
}



/*
void SearchWidget::slotRightButtonSearch(const QPoint& pos)
{
    //qDebug() << "SearchWidget::slotRightButtonSearch"  << endl;

    searchBoxLineEdit->setFocus();
}
*/

void SearchWidget::slotQsoDeleteFromSearch(const int _qsoId)
{
    //qDebug() << "SearchWidget::slotQsoDeleteFromSearch: " << QString::number(_qsoId) << endl;


    int QSOid = _qsoId;
    //int x = -1;

    QString _qrz = dataProxy->getCallFromId(QSOid);
    if (_qrz.length()>=3)
    {

        QString message = QString(tr("You have requested to delete the QSO with: %1").arg(_qrz));

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
                    searchWindow->slotToolSearchQSL(0);
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

void SearchWidget::slotQSLRecViaBureauFromSearch()
{
       //qDebug() << "SearchWidget::slotQSLRecViaBureauFromLog: " << endl;
    int _qsoId = (qslRecViaBureauFromSearchAct->data()).toInt();
    //logWindow->qslRecViaBureau(_qsoId);
    dataProxy->qslRecViaBureau(_qsoId, QDate::currentDate());

    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}

void SearchWidget::slotQSLRecViaDirectFromSearch()
{
       //qDebug() << "SearchWidget::slotQSLRecViaDirectFromLog: " << endl;
     int _qsoId = (qslRecViaDirectFromSearchAct->data()).toInt();
    //logWindow->qslRecViaDirect(_qsoId);
    dataProxy->qslRecViaDirect(_qsoId, QDate::currentDate());
    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}



void SearchWidget::slotStartDelayInputTimer()
{
    //qDebug() << "SearchWidget::slotStartDelayInputTimer"  << endl;
    int cursorP = searchBoxLineEdit->cursorPosition();
    searchBoxLineEdit->setText((searchBoxLineEdit->text()).toUpper());
    searchBoxLineEdit->setCursorPosition(cursorP);

    delayInputTimer->start(800); //TODO: Define the delay in the Settings
}

void SearchWidget::slotDelayInputTimedOut()
{
    //qDebug() << "SearchWidget::slotDelayInputTimedOut"  << endl;
    delayInputTimer->stop();
    QString _text = (searchBoxLineEdit->text()).toUpper();
    if( _text != lastSearch)
    {
        //DO WHATEVER
        lastSearch = _text;
        slotSearchBoxTextChanged();

    }
}

void SearchWidget::fillStationCallsignComboBox()
{
    stationCallsignComboBox->clear();
    stationCallsignComboBox->addItem(tr("All in log"));
    stationCallsignComboBox->addItem(tr("Not defined"));
    if (searchAllRadioButton->isChecked())
    {
        stationCallsignComboBox->addItems(dataProxy->getStationCallSignsFromLog(-1));
    }
    else
    {
        stationCallsignComboBox->addItems(dataProxy->getStationCallSignsFromLog(currentLog));
    }
}

void SearchWidget::slotRadioButtonToggled()
{
    //qDebug() << "SearchWidget::slotRadioButtonToggled"  << endl;
    fillStationCallsignComboBox();
    slotSearchBoxTextChanged();
}

void SearchWidget::slotStationCallsignChanged()
{
    //qDebug() << "SearchWidget::slotStationCallsignChanged: " << stationCallsignComboBox->currentText() << endl;
    slotSearchBoxTextChanged();
}

/*
void SearchWidget::qslRecViaDirectMarkReq(const int _qsoId)
{
    //qDebug() << "SearchWidget::qslRecViaDirect: " << QString::number(_qsoId) << endl;
    dataProxy->qslRecViaDirect(_qsoId, QDate::currentDate(), true);
}
*/
/*
void SearchWidget::qslRecViaBureauMarkReq(const int _qsoId)
{
    //qDebug() << "SearchWidget::qslRecViaBureau: " << QString::number(_qsoId) << "/" << QDate::currentDate() << endl;
    dataProxy->qslRecViaBureau(_qsoId, QDate::currentDate(), true);
    //qDebug() << "SearchWidget::qslRecViaBureau: END" << endl;
}
*/
void SearchWidget::slotQSOToEditFromSearch(const int _qsoId)
{
    //qDebug() << "slotQSOToEditFromSearch: " << (qsoToEditFromSearchAct->data()).toString() << endl;
    emit actionQSODoubleClicked(_qsoId);
}

/*
void SearchWidget::slotQSLRecViaDirectMarkReqFromSearch()
{
       //qDebug() << "SearchWidget::slotQSLRecViaDirectFromLog: " << endl;
     int _qsoId = (qslRecViaDirectMarkReqFromSearchAct->data()).toInt();
    qslRecViaDirectMarkReq(_qsoId);
    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}
*/
void SearchWidget::slotQSLSentViaDirectMarkDXReqFromSearch()
{
       //qDebug() << "slotQSLSentViaDirectMarkDXReqFromSearch: " << endl;
    int _qsoId = (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toInt();

    dataProxy->qslSentViaDirect(_qsoId, QDate::currentDate());
    dataProxy->qslRecAsRequested(_qsoId, QDate::currentDate());


    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}

/*
 * void SearchWidget::slotQSLSentViaBureauFromSearch()
{
   //    //qDebug() << "SearchWidget::slotQSLSentViaBureauFromSearch: " << (qslSentViaBureauFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslSentViaBureauFromSearchAct->data()).toInt();

    dataProxy->qslSentViaBureau(_qsoId, QDate::currentDate());

    //logWindow->qslSentViaBureau(_qsoId);
    //qslSentViaBureau(_qsoId);
    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}
*/

void SearchWidget::slotQSLSentViaDirectFromSearch()
{
       //qDebug() << "SearchWidget::slotQSLSentViaDirectFromSearch: " << endl;
     int _qsoId = ((qslSentViaDirectFromSearchAct->data()).toInt());
    dataProxy->qslSentViaDirect(_qsoId, QDate::currentDate());
    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }

    //qslSentViaDirect(_qsoId);

}

/*
void SearchWidget::slotQSLSentViaBureuMarkDXReqFromSearch()
{
       //qDebug() << "slotQSLSentViaBureuMarkDXReqFromSearch" << endl;
     int _qsoId = (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt();

    dataProxy->qslSentViaBureau(_qsoId, QDate::currentDate());
    dataProxy->qslRecAsRequested(_qsoId, QDate::currentDate());


    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.

}
*/

/*
void SearchWidget::slotQSLRecViaBureauMarkReqFromSearch()
{
    //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog -  Start" << endl;
    int _qsoId = (qslRecViaBureauMarkReqFromSearchAct->data()).toInt();
       //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 01" << endl;
    qslRecViaBureauMarkReq(_qsoId);
       //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 02: n: " << QString::number(_qsoId) << endl;
    if(qslingNeeded)
    {
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 03" << endl;
        searchWindow->slotToolSearchQSL(0);
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 04" << endl;
    }
    else
    {
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 05" << endl;
        slotSearchBoxTextChanged();
           //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 06" << endl;
    }
       //qDebug() << "SearchWidget::slotQSLRecViaBureauMarkReqFromLog: 07" << endl;
}

*/




/*
void SearchWidget::searchModel()
{
       //qDebug() << "SearchWidget::slotToolSearchQSLToSend - TO PREPARE THE QUERY and optimize the function" << endl;
    slotToolSearchQSL(0);
       //qDebug() << "SearchWidget::slotToolSearchQSLToSend - END" << endl;
}
*/

/*
void SearchWidget::slotQSLSentMarkAsRequested()
{
   // bool qslSentAsRequested(const int _qsoId, const QString _updateDate);

    int _qsoId = (qslSentRequestedAct->data()).toInt();
    dataProxy->qslSentAsRequested(_qsoId, QDate::currentDate());
    if(qslingNeeded)
    {
        searchWindow->slotToolSearchQSL(0);
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}

*/
    /*

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
    */
void SearchWidget::setNeedingQSL(bool const _q)
{
    qslingNeeded = _q;
    searchWindow->setNeedingQSL(qslingNeeded);

}

void SearchWidget::slotSearchBoxTextChanged()
{
    //qDebug() << "SearchWidget::slotSearchBoxTextChanged: "  << searchBoxLineEdit->text() << endl;

    //QString _id, _call, _dateTime, _band, _bandid, _mode, _qsltx, _qslrx, _stationcallsign, _dxcc;
    //QStringList q;
    //_stationcallsign = QString();
    bool searchAll = searchAllRadioButton->isChecked();
    //int i = -1;
    int cursorP = searchBoxLineEdit->cursorPosition();
    /*
    if (searchBoxLineEdit->text() == "*")
    {
        callFilter = QString("call<>'1'");
        setModelFilter();
        searchBoxLineEdit->setCursorPosition(cursorP);
        return;
    }

    */
    searchBoxLineEdit->setText((searchBoxLineEdit->text()).toUpper());

    if (((searchBoxLineEdit->text()).length() < 2) && (searchBoxLineEdit->text() != "*"))
    {
        //qDebug() << "SearchWidget::slotSearchBoxTextChanged: NO FILTER" << endl;
        currentLogFilter = QString("lognumber='-1'");
        setModelFilter();
        return;
    }

    setNeedingQSL(false);
    //qslingNeeded = false; // If I am searching I am not longer looking for QSO to QSL

    QString theCall = searchBoxLineEdit->text();

    //QSqlQuery query;
    QString queryString, aux;
    //QString aux;
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

    if (searchBoxLineEdit->text() == "*")
    {
        callFilter = QString("call<>'1'");
    }
    else
    {
        callFilter = QString("call LIKE '%1'").arg(aux);
    }
    QString searchAllQuery;
    if (searchAll)
    {
        //searchWindow->setCurrentLog("lognumber <> '-1'");
    }
    else
    {
        currentLogFilter = QString("lognumber = '%1'").arg(currentLog);
    }

    //QString stationCallQuery;
    QString stationCallSign;

    if (stationCallsignComboBox->currentIndex() == 0)
    {
        stationCallsignFilter = QString("");
        setModelFilter();
    }
    else if (stationCallsignComboBox->currentIndex() == 1)
    {
        stationCallsignFilter = QString("station_callsign = ''");
        setModelFilter();
    }
    else if (stationCallsignComboBox->currentIndex() > 1)
    {
        stationCallSign = stationCallsignComboBox->currentText();
        stationCallsignFilter= QString("station_callsign = '%1'").arg(stationCallSign);
        setModelFilter();
    }

    else
    { // This should be an error so return
        searchBoxLineEdit->setCursorPosition(cursorP);
        return;
    }

    searchBoxLineEdit->setCursorPosition(cursorP);


}

void SearchWidget::setModelFilter()
{
    //qDebug() << "SearchWidget::setModelFilter - callFilter        = "  << callFilter  << endl;
    //qDebug() << "SearchWidget::setModelFilter - stationCallFilter = " << stationCallsignFilter   << endl;
    //qDebug() << "SearchWidget::setModelFilter - currentLog        = " << currentLogFilter << endl;

    QString filter;
    filter = QString();

    if (callFilter.length()>0)
    {
        filter = callFilter;
    }

    if (stationCallsignFilter.length() >0)
    {
        if (filter.length()>0)
        {
            filter = filter + " AND " + stationCallsignFilter;
        }
        else
        {
            filter = stationCallsignFilter;
        }
    }

    if (currentLogFilter.length() >0)
    {
        if (filter.length()>0)
        {
            filter = filter + " AND " + currentLogFilter;
        }
        else
        {
            filter = currentLogFilter;
        }
    }

    //qDebug() << "SearchWidget::setModelFilter: " << filter  << endl;
    searchWindow->setFilterString(filter);
}

void SearchWidget::setCurrentLog(const int _log)
{
    currentLog = _log;
}

void SearchWidget::slotSearchClearButtonClicked()
{
       //qDebug() << "SearchWidget::slotSearchClearButtonClicked: " << endl;
    searchWindow->clear();
    searchBoxLineEdit->clear();
    searchSelectAllClicked = false;
    setNeedingQSL(false);
    //qslingNeeded = false;
    selectStationCallSign();
}

void SearchWidget::slotSearchBoxSelectAllButtonClicked()
{
       //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: " << endl;
    if (searchSelectAllClicked)
    {
           //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: UN-SELECTING" << endl;
        searchSelectAllClicked = false;
        searchWindow->clearSelection();
        searchBoxSelectAllButton->setText(tr("&Select All"));

    }
    else
    {
           //qDebug() << "SearchWidget::slotSearchBoxSelectAllButtonClicked: SELECTING" << endl;
        searchSelectAllClicked = true;
        searchWindow->selectAll();
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
    /*
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
    */
}

void SearchWidget::slotSearchExportButtonClicked()
{
    //qDebug() << "SearchWidget::slotSearchExportButtonClicked: Selected: " << QString::number(searchWindow->getSelectedQSOs().count())<< endl;
    // MARK QSOs
    // SAVE MARKED QSOs TO FILE
    // UNMARK QSOs
    QList<int> _qsos;
    _qsos.clear();
    _qsos << searchWindow->getSelectedQSOs();
    bool itemsSelected = false;

    for (int i = 0; i < _qsos.size(); ++i)
    {
        QString stringQuery;
        QSqlQuery query;

        stringQuery = QString("UPDATE log SET marked = 'X' WHERE id='%1'").arg(_qsos.at(i));
        bool sqlOK = query.exec(stringQuery);
        if (!sqlOK)
        {
            //qDebug() << "SearchWidget::slotSearchExportButtonClicked: query executed-error: " << query.lastError();
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        }
         //qDebug() << "SearchWidget::slotSearchExportButtonClicked: query executed: " << query.lastQuery();
        itemsSelected = true;
    }

    if (itemsSelected)
    {
           //qDebug() << "SearchWidget::slotSearchExportButtonClicked: to Ask filename" << endl;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                   util->getHomeDir(),
                                   "ADIF (*.adi *.adif)");
        filemanager->adifLogExportMarked(fileName);
           //qDebug() << "SearchWidget::slotSearchExportButtonClicked: to callsign save file" << endl;
        dataProxy->unMarkAllQSO();
    }
    else
    {
        //No items were selected
    }

    //qDebug() << "SearchWidget::slotSearchExportButtonClicked: unmarking..." << endl;
}
void SearchWidget::searchToolNeededQSLToSend()
{
    searchWindow->slotToolSearchQSL(0);
}

void SearchWidget::slotToolSearchRequestedQSLToSend()
{
    searchWindow->slotToolSearchQSL(1);
}

void SearchWidget::slotToolSearchNeededQSLPendingToReceive()
{
    searchWindow->slotToolSearchQSL(2);
}

void SearchWidget::slotToolSearchNeededQSLRequested()
{
    searchWindow->slotToolSearchQSL(3);
}

void SearchWidget::setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default)
{
       //qDebug() << "DXClusterWidget::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default << endl;
    // Just to pass the colors to the awards class
    searchWindow->setColors(_newOne,  _needed, _worked,  _confirmed, _default);
}
