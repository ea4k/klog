/***************************************************************************
    dxcluster.cpp  -  description
       -------------------
begin                : oct 2011
copyright            : (C) 2011 by Jaime Robles
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
*    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
*                                                                           *
*****************************************************************************/
#include <QFont>
#include "dxcluster.h"
/*
DXClusterWidget::DXClusterWidget(DataProxy_SQLite *dp, QWidget *parent)
          : QWidget(parent)
{
       //qDebug() << "DXClusterWidget::DXClusterWidget";
    dataProxy = dp;
    constrid = 1;
    awards = new Awards(dataProxy, Q_FUNC_INFO);
    util = new Utilities(Q_FUNC_INFO);
    saveSpotsFile = new QFile();
    //initClass();
       //qDebug() << "DXClusterWidget::DXClusterWidget - END";
}
*/
DXClusterWidget::DXClusterWidget(DataProxy_SQLite *dp, QWidget *parent)
          : QWidget(parent)
//DXClusterWidget::DXClusterWidget(DataProxy_SQLite *dp, const QString &clusterToConnect, const int portToConnect, QWidget *parent)
//          : QWidget(parent)
{
      //qDebug() << "DXClusterWidget::DXClusterWidget2" << clusterToConnect << QString::number(portToConnect);


    saveSpotsFile = new QFile();

    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    world = new World(dataProxy, Q_FUNC_INFO);
    awards = new Awards(dataProxy, Q_FUNC_INFO);

    tcpSocket = new QTcpSocket(this);

    dxClusterListWidget = new QListWidget();

    inputCommand = new QLineEdit;
    sendButton = new QPushButton;
    clearButton = new QPushButton;

    inputCommand->setDisabled(true);
    inputCommand->setToolTip(tr("Click on Connect to connect to the DX-Cluster server"));

    dxClusterListWidget->setMouseTracking(true);

    sendButton->setText(tr("Connect"));
    clearButton->setText(tr("Clear"));

    QHBoxLayout *bottonLayout = new QHBoxLayout;
    bottonLayout->addWidget(inputCommand);
    bottonLayout->addWidget(sendButton);
    bottonLayout->addWidget(clearButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(dxClusterListWidget);
    layout->addLayout(bottonLayout);

    setLayout(layout);

    dxClusterListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sendButton , SIGNAL(clicked()), this, SLOT(slotClusterSendToServer()) );
    connect(inputCommand, SIGNAL(textChanged(QString)), this, SLOT(slotClusterInputTextChanged()) );

    //connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    connect(dxClusterListWidget, SIGNAL(itemDoubleClicked ( QListWidgetItem *)), this, SLOT(slotClusterDXClusterWidgetItemDoubleClicked( QListWidgetItem * )) );
    connect(dxClusterListWidget, SIGNAL(itemEntered ( QListWidgetItem *)), this, SLOT(slotClusterDXClusterWidgetItemEntered( QListWidgetItem * )) );
    connect(dxClusterListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotClusterDXClusterWidgetItemSelected() ) );
    connect(dxClusterListWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButton( const QPoint& ) ) );
     //TESTADDSPOT();
       //qDebug() << "DXClusterWidget::DXClusterWidget2 - END" ;
}

DXClusterWidget::~DXClusterWidget()
{
   //qDebug() << Q_FUNC_INFO;
    delete(dataProxy);
    delete(awards);
    delete(world);
    delete(util);
    delete(saveSpotsFile);
}

void DXClusterWidget::createActions()
{
   //qDebug() << Q_FUNC_INFO;
    checkQRZCOMFromLogAct = new QAction(tr("Check in QRZ.com"), this);
    //checkQRZCOMFromLogAct->setShortcut(Qt::CTRL | Qt::Key_Q);
    checkQRZCOMFromLogAct->setStatusTip(tr("Check this callsign in QRZ.com"));
    connect(checkQRZCOMFromLogAct, SIGNAL(triggered()), this, SLOT( slotCheckQRZCom() ));
}

void DXClusterWidget::init()
{
   //qDebug() << Q_FUNC_INFO;
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    dxSpotColor.setNamedColor("slategrey"); //To be replaced by .fromString in Qt6.6
#else
    dxSpotColor.setNamedColor("slategrey"); //To be replaced by .fromString in Qt6.6
#endif

    //dxSpotColor.setNamedColor("slategrey");
    dxClusterConnected = false;
    dxClusterAlreadyConnected = false;
    showDxMarathon = false;

    showhf = true;
    showvhf = true;
    showwarc = true;
    showworked = true;
    showconfirmed = true;
    showann = true;
    showwwv = true;
    showwcy = true;
    saveSpots = false;
    myQrz = QString();
    currentLog = 0;
    server = "dxfun.com";
    port = quint16(8000);
    saveSpotsFile->setFileName(util->getSaveSpotsLogFile());

    dxClusterSpotItem * item = new dxClusterSpotItem(dxClusterListWidget, tr("Click on connect to connect to the DX-Cluster"), awards->getDefaultColor());
    Q_UNUSED(item);
    createActions ();
    //TODO: Check how to add an item in a different way
}

void DXClusterWidget::setMyQRZ(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO;
    if (_qrz.length()>2)
    {
        myQrz = _qrz;
    }
}

void DXClusterWidget::addData()
{
   //qDebug() << Q_FUNC_INFO;
    /*
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
*/

    dxClusterSpotItem * item = new dxClusterSpotItem(dxClusterListWidget, tr("Click on connect to connect to the DX-Cluster"), awards->getDefaultColor());
    Q_UNUSED(item);
}

void DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked( QListWidgetItem * item )
{
   //qDebug() << Q_FUNC_INFO << item->text();

    QStringList ql;
    ql.clear();

    if (item)
    {
        ql = readItem(item);
           //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: Length: " << QString::number(ql.length()) ;
        if (ql.length()==2)
        {
            ql << "double";
               //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: EMMITED" ;
            emit dxspotclicked(ql);
        }
        else
        {
               //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: NOT EMMITED-1" ;
        }
    }
    else
    {
           //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: NOT EMMITED-2 (no item)" ;
    }
}

void DXClusterWidget::connectToDXCluster()
{
   //qDebug() << Q_FUNC_INFO;
    if (dxClusterConnected)
    {
           //qDebug() << "DXClusterWidget::connectToDXCluster: - Already connected!!";
        return; // If we are connected we don't want to start another connection
    }

    connect(tcpSocket, SIGNAL(connected()), SLOT(slotClusterSocketConnected()) );
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotClusterDataArrived() ));
    connect(tcpSocket, SIGNAL(errorOcurred(QAbstractSocket::SocketError)), this, SLOT(slotClusterDisplayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), SLOT(slotClusterSocketConnectionClosed()) );
    connect(inputCommand, SIGNAL(returnPressed()), this, SLOT(slotClusterSendToServer()) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClusterClearLineInput()) );
    //openFile(); // This functions opens the file to save the DX-Cluster activity. The file will be closed when the DX is disconnected.
    tcpSocket->connectToHost( server, port );
    dxClusterListWidget->setSortingEnabled (false);

    //dxClusterSpotItem * item = new dxClusterSpotItem(dxClusterListWidget, tr("Trying to connect to the server") + "\n", awards->getDefaultColor());
    dxClusterListWidget->addItem (new dxClusterSpotItem(dxClusterListWidget, tr("Trying to connect to the server") + "\n", awards->getDefaultColor()));
}


void DXClusterWidget::slotClusterDisplayError(QAbstractSocket::SocketError socketError)
{
   //qDebug() << Q_FUNC_INFO;
     switch (socketError) {
     case QAbstractSocket::RemoteHostClosedError:
         break;
     case QAbstractSocket::HostNotFoundError:
         QMessageBox::warning(this, tr("KLog DXCluster"),
                                  tr("The host was not found. Please check:") + "\n\n" +
                                     tr ("- your network connection;\n"
                                     "- the host name and port settings."));
         break;
     case QAbstractSocket::ConnectionRefusedError:
         QMessageBox::warning(this, tr("KLog DXCluster"),
                                  tr("The connection was refused by the peer. "
                                     "Make sure the DXCluster server is running, "
                                     "and check that the host name and port "
                                     "settings are correct."));
         break;
     default:
         QMessageBox::warning(this, tr("KLog DXCluster"),
                                  tr("The following error occurred: %1.")
                                  .arg(tcpSocket->errorString()));
     }
 }

bool DXClusterWidget::checkIfNeedsToBePrinted(const QString &_DXEntity, int const _band, const int _mode)
{
   //qDebug() << Q_FUNC_INFO << _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
    QStringList qs;
    qs.clear();
    qs << _DXEntity << QString::number(_band) << QString::number(_mode)  << QString::number(currentLog);
    //bool isConfirmed = false;
    bool status = awards->isThisSpotConfirmed (qs);

      //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: Status: " << _DXEntity << "/" << QString::number(status);

    if (!showconfirmed)
    {
          //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is confirmed? ("<< QString::number(status)<< ")";
        if (status)
        {
              //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: It is confirmed: DON'T' print: " << _DXEntity <<"/" << dataProxy->getNameFromBandId(_band);
            return false;
        }
    }

    if (!showhf)
    {
          //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is HF?";
        if (dataProxy->isHF(_band))
        {
              //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: Not showing HF but... is it WARC?";
            if ( (showwarc) && dataProxy->isWARC(_band) )
            {
                  //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: Not showing HF but... is WARC, print!";
                return true;
            }

              //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is HF: DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
            return false;
        }
        else
        {
              //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is NOT HF";
        }
    }

    if (!showwarc)
    {
          //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is WARC?";
        if (dataProxy->isWARC(_band))
        {
              //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is WARC, DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
            return false;
        }
        else
        {
              //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is NOT WARC";
        }
    }

    if (!showvhf)
    {
           //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is VHF?";
        if (dataProxy->isVHF(_band))
        {
               //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is VHF, DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
            return false;
        }
        else
        {
               //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is NOT VHF ";
        }
    }
       //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: returns TRUE and will be printed: " << _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
    return true;
}

void DXClusterWidget::setCurrentLog(const int _log)
{
   //qDebug() << Q_FUNC_INFO;
    if (dataProxy->doesThisLogExist(_log))
    {
        currentLog = _log;
    }
    else
    {
        currentLog = -1;
    }
}

void DXClusterWidget::slotClusterDataArrived()
{
    qDebug() << Q_FUNC_INFO;
    QStringList qs;
    QString dxClusterString;
    QString dxCall;
    QString dxFrequency;
    QString spotBand;
    bool dxSpot = false;
    spotBand = "-1";
    //bool isADXSpot = false;
    int dxEntity = -1;
    QString spotter;

    while ( tcpSocket->canReadLine() )
    {
        spotter.clear();
        dxFrequency.clear();
        dxCall.clear();
        dxEntity = -1;

        dxClusterString =  tcpSocket->readLine();
        dxClusterString = dxClusterString.trimmed();
        // Remove BELL-string if exists
        dxClusterString = dxClusterString.remove("\a");
        saveSpot(dxClusterString);

        QStringList tokens = dxClusterString.split(" ", QT_SKIP);
        if (tokens.size()<2){
            qDebug() << Q_FUNC_INFO << " - Tokens < 2";
            return;
        }
        // It is a "DX de SP0TTER FREC DXCALL"
        //0 = DX, 1 = de, 2 = spotter, 3 = Freq, 4 = dxcall, 5 = comment
        qDebug() << Q_FUNC_INFO << " - DXCLUSTER->" << dxClusterString << "\nTOKENS: " << tokens;
        bool freqOK = false;
        double freq = (tokens[0]).toDouble(&freqOK) / 1000; // This freq Check is just for the "else if"


        if ((tokens[0] == "DX") && (tokens[1] == "de"))
        { // Dx de EA0AA EA0XX
            qDebug() << Q_FUNC_INFO << "  - Identified: DX de";
            dxSpot = true;
            spotter = tokens[2];
            spotter.truncate(spotter.size() - 1);

            dxFrequency = QString::number( ((tokens[3]).toDouble(&freqOK))/1000); // Convert KHz to MHz...
            if (!freqOK)
            {
                qDebug() << Q_FUNC_INFO << " - freqOK false";
                return;
            }
            dxCall = tokens[4];
            dxEntity = world->getQRZARRLId(dxCall);
            spotBand = QString::number(dataProxy->getBandIdFromFreq(  dxFrequency.toDouble()  ) );
        }
        else if ((freqOK) && (util->isValidCall(tokens[1])) )
        {// 18.000 EA0XX
            qDebug() << Q_FUNC_INFO << "  - Identified: Freq Call";
            dxSpot = true;
            dxCall = tokens[1];
            dxFrequency = tokens[0];
            dxFrequency = QString::number(freq); // Freq was calculated before entering the if block
            spotter = tokens.last();
            spotter.chop(1);        // Remove the last char '>'
            spotter.remove(0,1);    // Remove the first char '<'
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "  - Identified: Not a DXSpot";
            dxSpot = false;
            dxSpotColor = awards->getDefaultColor();
        }

        if (dxSpot)
        {
            qs.clear();
            qs << QString::number(dxEntity) << spotBand << "-1" << QString::number(currentLog) ;
            qDebug() << Q_FUNC_INFO << " - DX_Entity:  " << QString::number(dxEntity);
            qDebug() << Q_FUNC_INFO << " - DX Spotter: " << spotter ;
            qDebug() << Q_FUNC_INFO << " - Freq:       " << dxFrequency;
            qDebug() << Q_FUNC_INFO << " - DX:         " << dxCall;

            dxSpotColor = awards->getQRZDXStatusColor(qs);
            if (showDxMarathon)
            {
                if (awards->isDXMarathonNeed(dxEntity, world->getQRZCqz(dxCall), QDateTime::currentDateTime().date().year(), currentLog))
                {
                    dxClusterString = dxClusterString + "  ### Needed for DXMarathon - " + QString::number(QDateTime::currentDateTime().date().year()) + " ###";
                }
            }
        }

         //TODO: Change the "-1" by the mode
        if (!checkIfNeedsToBePrinted(QString::number(dxEntity), spotBand.toInt(), -1))
        {
            qDebug() << Q_FUNC_INFO << "  - Not to be printed!: " << dxCall;
            return;
        }

        QListWidgetItem *item = new QListWidgetItem();
        item->setForeground(QBrush(dxSpotColor));
        item->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        item->setText(dxClusterString);
        dxClusterListWidget->insertItem(0,item);

        if (!dxSpot)
            return;

        qDebug() << Q_FUNC_INFO << " - Everything OK emitting...";
        emit dxspotArrived(dxCall, freq);
    }

    qDebug() << Q_FUNC_INFO << " - END";
}

void DXClusterWidget::slotClusterSocketConnected()
{
   //qDebug() << Q_FUNC_INFO;

    QListWidgetItem *item = new QListWidgetItem();
    item->setForeground(QBrush(awards->getDefaultColor()));
    item->setText(tr("Connected to server"));
    dxClusterListWidget->insertItem(0,item);

//    dxClusterSpotItem * item = new dxClusterSpotItem(dxclusterListWidget, i18n("Connected to server"), awards->getDefaultColor());
    dxClusterConnected = true;
    inputCommand->setFocus(Qt::OtherFocusReason);

    if (( dxClusterConnected ) && (!dxClusterAlreadyConnected) ){
        bool ok;
        QString callsignText;
        if (myQrz.length()>2)
        {
            callsignText = QInputDialog::getText(this, tr("KLog message"), tr("Enter your callsign to connect to the cluster:"), QLineEdit::Normal, myQrz, &ok);
        }
        else
        {
            callsignText = QInputDialog::getText(this, tr("KLog message"), tr("Enter your callsign to connect to the cluster:"), QLineEdit::Normal, "", &ok);
        }
        if(!ok)
        {
            tcpSocket->disconnectFromHost();
            return;
        }
        //QString callsignText = QInputDialog::getText(this, tr("KLog message"), tr("Enter your callsign to connect to the cluster:"), QLineEdit::Normal, "", &ok);
        QString passwordText = QInputDialog::getText(this, tr("KLog message"), tr("Enter your password to connect to the cluster:\n(Just hit enter for no password)"), QLineEdit::Normal, "", &ok);
        if(!ok)
        {
            tcpSocket->abort();
            return;
        }
        QTextStream os(tcpSocket);
        if ( callsignText.length() > 2 && ok ) {
            os << callsignText << "\n";
            //TODO: Check the DXCluster answer and enter the password if needed.
            sendButton->setText(tr("Disconnect"));
            clearButton->setText(tr("Clear"));
            dxClusterAlreadyConnected = true;
        } else {
            //os << tr("Not logged on, you may need to enter your callsign again.") << "\n";
            dxClusterAlreadyConnected = false;
        }
        inputCommand->setEnabled(true);
        inputCommand->setToolTip(tr("Enter here the commands to be sent to the DX-Cluster server."));
    }
}

void DXClusterWidget::slotClusterSocketConnectionClosed()
{
   //qDebug() << Q_FUNC_INFO;
    QListWidgetItem *item = new QListWidgetItem();
    item->setForeground(QBrush(awards->getDefaultColor()));
    item->setText(tr("Connection closed by the server"));
    dxClusterListWidget->insertItem(0,item);
    dxClusterConnected = false;
    dxClusterAlreadyConnected = false;
    sendButton->setText(tr("Connect"));
    inputCommand->setDisabled(true);
    inputCommand->setToolTip(tr("Click on Connect to connect to the DX-Cluster server."));
    //connect(inputCommand, SIGNAL(returnPressed()), this, SLOT(slotClusterSendToServer()) );
    disconnect (inputCommand, SIGNAL(returnPressed()), this, SLOT(slotClusterSendToServer()) );
}

void DXClusterWidget::slotClusterSendToServer()
{
   //qDebug() << Q_FUNC_INFO;
    if (!dxClusterConnected)
    {
        connectToDXCluster();
        return; // If we try to connect...
    }
    if (( inputCommand ->text().length() < 1 ) && ( sendButton->text() == tr("Disconnect") ) )
    {
        //qDebug() << Q_FUNC_INFO << " - Disconnecting";
        QTextStream os(tcpSocket);
        os << "bye\n";
        return;
    }
    else if ( inputCommand ->text().length() < 1 )
    {
        //qDebug() << Q_FUNC_INFO << " - Empty";
        return;
    }

    //  write to the server
    QTextStream os(tcpSocket);
    os << inputCommand ->text() << "\n";
    inputCommand ->clear();
}

void DXClusterWidget::slotClusterClearLineInput()
{
   //qDebug() << Q_FUNC_INFO;
    inputCommand->clear();
}

void DXClusterWidget::slotClusterInputTextChanged()
{
   //qDebug() << Q_FUNC_INFO;
    if ( ((inputCommand->text()).length()) <= 0 )
    {
        sendButton->setText(tr("Disconnect"));
        clearButton->setText(tr("Clear"));
    }
    else if (dxClusterConnected)
    {
        sendButton->setText(tr("Send"));
        clearButton->setText(tr("Clear"));
    }
}

void DXClusterWidget::setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default)
{
   //qDebug() << Q_FUNC_INFO << ": " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default;
    // Just to pass the colors to the awards class
    awards->setColors(_newOne,  _needed, _worked,  _confirmed, _default);
}

void DXClusterWidget::setDXClusterSpotConfig(bool _showhf, bool _showvhf, bool _showwarc, bool _showworked, bool _showconfirmed, bool _showann, bool _showwwv, bool _showwcy )
{
   //qDebug() << Q_FUNC_INFO;
    showhf = _showhf;
    showvhf = _showvhf;
    showwarc = _showwarc;
    showworked = _showworked;
    showconfirmed = _showconfirmed;
    showann = _showann;
    showwwv = _showwwv;
    showwcy = _showwcy;
}

void DXClusterWidget::slotClusterDXClusterWidgetItemSelected()
{
   //qDebug() << Q_FUNC_INFO;
    QListWidgetItem * item = dxClusterListWidget->currentItem();

    QStringList ql;
    ql.clear();

    ql = readItem(item);
    if (ql.length()==2)
    {
        ql << "selected";
        emit dxspotclicked(ql);
    }
}

void DXClusterWidget::slotClusterDXClusterWidgetItemEntered( QListWidgetItem * item )
{
   //qDebug() << Q_FUNC_INFO;
/*
    This code comes from slotClusterDXClusterWidgetItemDoubleClicked
*/
    QString tip;
    tip.clear();
    QStringList ql;
    ql.clear();

    if (item)
    {
        ql = readItem(item);
        if (ql.length()==2)
        {
            tip = world->getQRZEntityName(ql.at(0));
            item->setToolTip(tip);
        }
    }
}


bool DXClusterWidget::isConnected()
{
   //qDebug() << Q_FUNC_INFO;
    return dxClusterConnected;
}

QStringList DXClusterWidget::readItem(QListWidgetItem * item)
{
   //qDebug() << Q_FUNC_INFO;
    // Returns: fields << dxCallsign << dxFreq;

    QStringList fields;
    QString dxClusterString;
    //int currentEntity;
    QString dxCallsign, dxFreq;
    bool FirstFrecOK;

    if (item)
    {
        fields.clear();
        dxClusterString = ((item->data(0)).toString()).simplified();
        fields << dxClusterString.split(" ");

        (fields.at(0)).toFloat(&FirstFrecOK); // Just to see if the first string is a frecuency

        if ( (fields.at(0) == "DX" ) && (fields.at(1) == "de" ) )
        { // DX de EA0XXX: 21200.1 EA0K The comment 1550
            if ( world->getQRZARRLId(fields.at(4))> 0 )
            {
                   //qDebug() << Q_FUNC_INFO << "-  ARRLid: " << QString::number(world->getQRZARRLId(fields.at(4))) ;
                dxCallsign = (fields.at(4)).toUpper();
                (fields.at(3)).toFloat(&FirstFrecOK);
                if (FirstFrecOK)
                { // The frecuency is valid
                    dxFreq = fields.at(3);
                    fields.clear();
                    //TODO: Change FREQ into bandId
                    fields << dxCallsign << dxFreq;
                    return fields;
                }
                else
                { // the frecuency is not a number!
                    dxCallsign.clear();
                    dxFreq.clear();
                    fields.clear();
                }
            }
            else
            { // The callsign is not from a valid country
              // TODO: if it is from a new entity/prefix it would not work.
                   //qDebug() << Q_FUNC_INFO << " Entity not valid" ;
                dxCallsign.clear();
                dxFreq.clear();
                fields.clear();
            }
        }
        //else if (( isAFrecuency(fields.at(0) ) ) && ( isACall(fields.at(1)) ) )
        else if ( (((fields.at(0)).toDouble()) > 0.0 )&& ( world->getQRZARRLId(fields.at(1))> 0 ) )
        { // 14205.0 EA0K   5-Mar-2012 1500Z    <EA0XXX>
            dxCallsign = (fields.at(1)).toUpper();
            dxFreq = fields.at(0);
            fields.clear();
            fields << dxCallsign << dxFreq;
            return fields;
        }
        else
        {
            dxCallsign.clear();
            dxFreq.clear();
            fields.clear();
        }
    }
    else
    {  // No Item
    }

    return fields;
}

void DXClusterWidget::setDXClusterServer(const QString &clusterToConnect, const int portToConnect)
{
   //qDebug() << Q_FUNC_INFO;
    server = clusterToConnect;
    port = quint16(portToConnect);
    if ((server.length()< 3) || (port <= 0))
    {
        server = "dxfun.com";
        port = 8000;
    }
}

void DXClusterWidget::setDXMarathon (const bool _enable)
{
   //qDebug() << Q_FUNC_INFO;
    showDxMarathon = _enable;
}

void  DXClusterWidget::setSaveSpots (const bool _enable)
{
   //qDebug() << Q_FUNC_INFO;
    saveSpots = _enable;
}

bool DXClusterWidget::openFile()
{
   //qDebug() << Q_FUNC_INFO;
    if (saveSpotsFile->isOpen())
    {
        return true;
    }
    if (!saveSpotsFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) /* Flawfinder: ignore */
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        QString aux = tr("It was not possible to open the file to save the DX-Spots for writing. DX-Cluster activity will not be saved!");
        msgBox.setText(aux);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }
    else
    {
        return true;
    }
}

void DXClusterWidget::slotRighButton(const QPoint& pos)
{
   //qDebug() << Q_FUNC_INFO;
    Q_UNUSED(pos);
    //int row = (dxClusterListWidget->indexAt(pos)).row();
    //qDebug() << Q_FUNC_INFO << " row: " << QString::number(row);
    QListWidgetItem * item = dxClusterListWidget->currentItem();

    QStringList ql;
    ql.clear();

    ql = readItem(item); //qstringlist << dxCallsign << dxFreq;
    if (ql.length()==2)
    {
        rightButtonFromLogMenu(ql);
    }
}

void DXClusterWidget::rightButtonFromLogMenu(const QStringList _ql)
{
    // This function creates the context menu
   //qDebug() << Q_FUNC_INFO;
    checkQRZCOMFromLogAct->setData (_ql.at(0));
    QMenu menu(this);
    menu.addAction(checkQRZCOMFromLogAct);
    menu.exec(QCursor::pos());
}



void DXClusterWidget::slotCheckQRZCom()
{
   //qDebug() << Q_FUNC_INFO;
    QString _c = checkQRZCOMFromLogAct->data ().toString ();
    util->openQrzcom (_c);
}

/*
void DXClusterWidget::rightButtonFromLogMenu(const int trow)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(trow);
    int _qsoID = ((logModel->index(trow, 0)).data(0)).toInt();
}
*/
void DXClusterWidget::saveSpot(const QString &_spot)
{
   //qDebug() << Q_FUNC_INFO << ": "  << _spot ;
    if (!saveSpots)
    {
          //qDebug() << "DXClusterWidget::saveSpot: Not saving";
        return;
    }
    else
    {
        if (openFile())
        {
              //qDebug() << "DXClusterWidget::saveSpot: File Open";
            QTextStream out(saveSpotsFile);

            out << util->getDateTimeSQLiteStringFromDateTime(QDateTime::currentDateTime()) << " - " << _spot.simplified().toUtf8();
            out << Qt::endl;
            saveSpotsFile->close();
        }
    }
}

void DXClusterWidget::loadSettings()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    QString aux = settings.value("DXClusterServerToUse").toString ();

    if (aux.contains(':'))
    {
        setDXClusterServer((aux.split(':', QT_SKIP)).at(0), ((aux.split(':', QT_SKIP)).at(1)).toInt());
    }

    //qDebug() << Q_FUNC_INFO << " - END";
}

/*
void DXClusterWidget::TESTADDSPOT()
{
       //qDebug() << "DXClusterWidget::TESTADDSPOT "  ;
    ; // Just a test spot
    QListWidgetItem *item = new QListWidgetItem();
    item->setForeground(QBrush(dxSpotColor));
    item->setText("DX de SP0TTER 14.000 DX1CALL");
    dxClusterListWidget->insertItem(0,item);
}
*/




/***************************************************************************
** This is an auxiliary class intended to provide color to the DX-Cluster **
** spots.                                                                 **
** It may be moved to a self .h & .cpp archives                           **
****************************************************************************/
dxClusterSpotItem::dxClusterSpotItem( QListWidget *parent, const QString& spot, const QColor& color ) : QListWidgetItem( parent ){
   //qDebug() << Q_FUNC_INFO;
    spotColor = color;
    setText(spot);
    // Experimenting with fonts for the cluster
    QFont f("Helvetica");
    f.setFixedPitch(true);
    setFont(f);
}

dxClusterSpotItem::~dxClusterSpotItem()
{
   //qDebug() << Q_FUNC_INFO;
}

