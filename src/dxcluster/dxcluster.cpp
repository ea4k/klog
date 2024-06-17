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
    connect(dxClusterListWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRightButton( const QPoint& ) ) );
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
   dxSpotColor.fromString(QAnyStringView(("slategrey"))); //To be replaced by .fromString in Qt6.6
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

    if (!item)
        return;

    DXSpot spot = readItem(((item->data(0)).toString()).simplified());
    if (!spot.isValid())
        return;
    spot.setClickStatus(DoubleClick);
    emit dxspotclicked(spot);
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

    connect(tcpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(slotClusterDisplayError(QAbstractSocket::SocketError)));
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

bool DXClusterWidget::checkIfNeedsToBePrinted(EntityStatus _entityStatus)
{
   //qDebug() << Q_FUNC_INFO << _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;

    bool status = awards->isThisSpotConfirmed (_entityStatus);
    //qDebug() << Q_FUNC_INFO << "Status: " << _DXEntity << "/" << QString::number(status);

    if (!showconfirmed)
    {
          //qDebug() << Q_FUNC_INFO << " is confirmed? ("<< QString::number(status)<< ")";
        if (status)
        {
              //qDebug() << Q_FUNC_INFO << " It is confirmed: DON'T' print: " << _DXEntity <<"/" << dataProxy->getNameFromBandId(_band);
            return false;
        }
    }

    if (!showhf)
    {
          //qDebug() << Q_FUNC_INFO << " is HF?";
        if (dataProxy->isHF(_entityStatus.bandId))
        {
              //qDebug() << Q_FUNC_INFO << ": Not showing HF but... is it WARC?";
            if ( (showwarc) && dataProxy->isWARC(_entityStatus.bandId) )
            {
                  //qDebug() << Q_FUNC_INFO << ": Not showing HF but... is WARC, print!";
                return true;
            }
              //qDebug() << Q_FUNC_INFO << ": is HF: DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
            return false;
        }
        else
        {
              //qDebug() << Q_FUNC_INFO << ": is NOT HF";
        }
    }

    if (!showwarc)
    {
        //qDebug() << Q_FUNC_INFO << ": is WARC?";
        if (dataProxy->isWARC(_entityStatus.bandId))
        {
              //qDebug() << Q_FUNC_INFO << ": is WARC, DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
            return false;
        }
        else
        {
              //qDebug() << Q_FUNC_INFO << ": is NOT WARC";
        }
    }

    if (!showvhf)
    {
        //qDebug() << Q_FUNC_INFO << ": is VHF?";
        if (dataProxy->isVHF(_entityStatus.bandId))
        {
               //qDebug() << Q_FUNC_INFO << ": is VHF, DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
            return false;
        }
        else
        {
               //qDebug() << Q_FUNC_INFO << ": is NOT VHF ";
        }
    }
    //qDebug() << Q_FUNC_INFO << ": returns TRUE and will be printed: " << _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< QT_ENDL;
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
  //qDebug() << Q_FUNC_INFO;
    //QStringList qs;
    QString dxClusterString;
    QString spotBand = "-1";

    //QString spotter;
    EntityStatus _entityStatus;
    dxClusterString = tcpSocket->readAll();

  //qDebug() << Q_FUNC_INFO << " Line: " << dxClusterString;
    dxClusterString = dxClusterString.trimmed();
  //qDebug() << Q_FUNC_INFO << " Line-0.5: " << dxClusterString;
    dxClusterString = dxClusterString.simplified();
  //qDebug() << Q_FUNC_INFO << " Line-1: " << dxClusterString;

    if (dxClusterString.endsWith("\x07\x07\r\n"))
        dxClusterString = dxClusterString.remove("\x07\x07\r\n");
  //qDebug() << Q_FUNC_INFO << " Line-2: " << dxClusterString;

    if (dxClusterString.endsWith("\u0007\u0007\r\n"))
        dxClusterString = dxClusterString.remove("\u0007\u0007\r\n");
  //qDebug() << Q_FUNC_INFO << " Line-3: " << dxClusterString;

    saveSpot(dxClusterString);
  //qDebug() << Q_FUNC_INFO << " - While 10";
    DXSpot spot = readItem(dxClusterString);
  //qDebug() << Q_FUNC_INFO << " - While 11";

    _entityStatus.entityId = -1;

    if (spot.isValid())
    {
      //qDebug() << Q_FUNC_INFO << " - Spot is Valid";
        _entityStatus.entityId = world->getQRZARRLId(spot.getDxCall());
        spotBand = QString::number(dataProxy->getBandIdFromFreq(spot.getFrequency().toDouble()) );
        _entityStatus.bandId = spotBand.toInt();

        dxSpotColor = awards->getQRZDXStatusColor(_entityStatus);
        if (showDxMarathon)
        {
            if (awards->isDXMarathonNeed(_entityStatus.entityId, world->getQRZCqz(spot.getDxCall()), QDateTime::currentDateTime().date().year(), currentLog))
            {
                dxClusterString = dxClusterString + "  ### Needed for DXMarathon - " + QString::number(QDateTime::currentDateTime().date().year()) + " ###";
            }
        }

        emit dxspotArrived(spot);
        _entityStatus.modeId = -1;
        if (!checkIfNeedsToBePrinted(_entityStatus))
        {
          //qDebug() << Q_FUNC_INFO << "  - Not to be printed!: " << spot.getDxCall();
            return;
        }
    }
    else
    {
      //qDebug() << Q_FUNC_INFO << " - Spot is NOT Valid";
        dxSpotColor = awards->getDefaultColor();
    }

  //qDebug() << Q_FUNC_INFO << " - While 70";

    QListWidgetItem *item = new QListWidgetItem();
    item->setForeground(QBrush(dxSpotColor));
    item->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    item->setText(dxClusterString);
    dxClusterListWidget->insertItem(0,item);

  //qDebug() << Q_FUNC_INFO << " - END";
}


QString DXClusterWidget::cleanSpotter(const QString _call)
{
    QString spotter = _call;
    spotter.chop(1);        // Remove the last char '>'
    spotter.remove(0,1);
    return spotter;
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
    qDebug() << Q_FUNC_INFO << " - myQRZ: " << myQrz;
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

    DXSpot spot = readItem(((item->data(0)).toString()).simplified());
    if (spot.isValid())
        emit dxspotclicked(spot);
}

void DXClusterWidget::slotClusterDXClusterWidgetItemEntered( QListWidgetItem * item )
{
   //qDebug() << Q_FUNC_INFO;
/*
    This code comes from slotClusterDXClusterWidgetItemDoubleClicked
*/

    QString tip;
    tip.clear();

    if (item)
    {
        DXSpot spot = readItem(((item->data(0)).toString()).simplified());
        if (spot.isValid())
        {
            tip = world->getQRZEntityName(spot.getDxCall());
            item->setToolTip(tip);
        }
    }
}


bool DXClusterWidget::isConnected()
{
   //qDebug() << Q_FUNC_INFO;
    return dxClusterConnected;
}

DXSpot DXClusterWidget::readItem(const QString _stringSpot)
{
  //qDebug() << Q_FUNC_INFO << _stringSpot;

    DXSpot spot = DXSpot();
    spot.clear();

    if (_stringSpot.length()<5)
       return spot;
  //qDebug() << Q_FUNC_INFO << "05";
    QString dxClusterString = _stringSpot;
    QStringList fields;
    fields.clear();
    fields << dxClusterString.split(" ");
  //qDebug() << Q_FUNC_INFO << "10";
    if ( (fields.at(0) == "DX" ) && (fields.at(1) == "de" ) )
    { // DX de EA0XXX: 21200.1 EA0K The comment 1550
      //qDebug() << Q_FUNC_INFO << ": Identified: DX de";
      //qDebug() << Q_FUNC_INFO << ": 0: " << fields.at(0);
      //qDebug() << Q_FUNC_INFO << ": 1: " << fields.at(1);
      //qDebug() << Q_FUNC_INFO << ": 2: " << fields.at(2);
      //qDebug() << Q_FUNC_INFO << ": 3: " << fields.at(3);
      //qDebug() << Q_FUNC_INFO << ": 4: " << fields.at(4);
      //qDebug() << Q_FUNC_INFO << ": 5: " << fields.at(5);
        spot.setSpotter(fields.at(2));
      //qDebug() << Q_FUNC_INFO << "11";
        Frequency freq;
        freq.fromQString((fields.at(3)), KHz);
        spot.setFrequency(freq);
      //qDebug() << Q_FUNC_INFO << "12";
        spot.setDXCall(fields.at(4));
      //qDebug() << Q_FUNC_INFO << "13";
        spot.setValid(true);
      //qDebug() << Q_FUNC_INFO << "14";
      //qDebug() << Q_FUNC_INFO << ": Identified: Freq1: " << spot.getFrequency().toQString();
    }
    else if (fields.last().endsWith(">"))
    { // 14250.0 EA0XXX      12-Apr-2020 2140Z Comment      <EA0XX>
      //qDebug() << Q_FUNC_INFO << ": Identified: ENDS with >";
      //qDebug() << Q_FUNC_INFO << "20";
      //qDebug() << Q_FUNC_INFO << " - Spotter: " << fields.last().removeLast();
        spot.setSpotter(fields.last());
      //qDebug() << Q_FUNC_INFO << " - Freq: " << fields.at(0);
        Frequency freq;
      //qDebug() << Q_FUNC_INFO << "21";
        if (!freq.fromQString((fields.at(0)), KHz))
            return spot;
      //qDebug() << Q_FUNC_INFO << "22";
      //qDebug() << Q_FUNC_INFO << " - Freq imported ";
        if (!freq.isValid())
            return spot;
      //qDebug() << Q_FUNC_INFO << "23";
      //qDebug() << Q_FUNC_INFO << " - Freq looks valid";
        spot.setFrequency(freq);

      //qDebug() << Q_FUNC_INFO << " - DXCall: " << fields.at(1);
        spot.setDXCall(fields.at(1));
      //qDebug() << Q_FUNC_INFO << "24";
        spot.setValid(true);
      //qDebug() << Q_FUNC_INFO << ": Identified: Freq2: " << spot.getFrequency().toQString();
    }
    else if ((fields.at(0) == "To" ) && (fields.at(1) == "ALL" ))
    { // To ALL Comment
      //qDebug() << Q_FUNC_INFO << ": Identified: To ALL";
      //qDebug() << Q_FUNC_INFO << "30";
        spot.setValid(false);
    }
    else
    {
      //qDebug() << Q_FUNC_INFO << ": Identified: just Text: " << _stringSpot;
      //qDebug() << Q_FUNC_INFO << "40";
        spot.setValid(false);
    }
  //qDebug() << Q_FUNC_INFO << "100";
  //qDebug() << Q_FUNC_INFO << " - END";
    return spot;
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

void DXClusterWidget::slotRightButton(const QPoint& pos)
{
   //qDebug() << Q_FUNC_INFO;
    Q_UNUSED(pos);
    //int row = (dxClusterListWidget->indexAt(pos)).row();
    //qDebug() << Q_FUNC_INFO << " row: " << QString::number(row);
    QListWidgetItem * item = dxClusterListWidget->currentItem();

    DXSpot spot = readItem(((item->data(0)).toString()).simplified());
    if ((spot.isValid()) && (spot.getClickStatus() == RightClick) )
    {
        rightButtonFromLogMenu(spot);
    }
}

void DXClusterWidget::rightButtonFromLogMenu(const DXSpot &_spot)
{
    // This function creates the context menu
   //qDebug() << Q_FUNC_INFO;
    DXSpot spot = _spot;
    checkQRZCOMFromLogAct->setData (spot.getDxCall());
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

