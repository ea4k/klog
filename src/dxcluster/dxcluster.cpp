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
#include "../callsign.h"

DXClusterWidget::DXClusterWidget(Awards *awards, QWidget *parent)
          : QWidget(parent),
            awards(awards),
            dataProxy(awards->dataProxy)
{
      //qDebug() << "DXClusterWidget::DXClusterWidget2" << clusterToConnect << QString::number(portToConnect);

    //dataProxy = awards->dataProxy;
    saveSpotsFile = new QFile();

    util = new Utilities(Q_FUNC_INFO);
    //util->setLongPrefixes(dataProxy->getLongPrefixes());
    //util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    world = new World(dataProxy, Q_FUNC_INFO);
    //awards = new Awards(dataProxy, Q_FUNC_INFO);

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
    connect(inputCommand, SIGNAL(returnPressed()), this, SLOT(slotClusterSendToServer()) );

    //connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    connect(dxClusterListWidget, SIGNAL(itemDoubleClicked ( QListWidgetItem *)), this, SLOT(slotClusterDXClusterWidgetItemDoubleClicked( QListWidgetItem * )) );
    connect(dxClusterListWidget, SIGNAL(itemEntered ( QListWidgetItem *)), this, SLOT(slotClusterDXClusterWidgetItemEntered( QListWidgetItem * )) );
    connect(dxClusterListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotClusterDXClusterWidgetItemSelected() ) );
    connect(dxClusterListWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRightButton( const QPoint& ) ) );

    connect(tcpSocket, SIGNAL(connected()), SLOT(slotClusterSocketConnected()) );
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotClusterDataArrived() ));
    connect(tcpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(slotClusterDisplayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), SLOT(slotClusterSocketConnectionClosed()) );


    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClusterClearLineInput()) );
     //TESTADDSPOT();
       //qDebug() << "DXClusterWidget::DXClusterWidget2 - END" ;
}

DXClusterWidget::~DXClusterWidget()
{
   //qDebug() << Q_FUNC_INFO;
    //delete(dataProxy);
    delete(tcpSocket);
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
    clearButton->setToolTip(tr("Clears the DXCluster command line."));
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
  //qDebug() << Q_FUNC_INFO << ": " << ((item->data(0)).toString()).simplified();

    QStringList ql;
    ql.clear();

    if (!item)
        return;

    QString aux = "DOUBLE " + ((item->data(0)).toString()).simplified(); // Mark the string to inform readItem that this
                                                                         // comes from a double click
    DXSpot spot = readItem(aux);
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

    //openFile(); // This functions opens the file to save the DX-Cluster activity. The file will be closed when the DX is disconnected.
    tcpSocket->connectToHost( server, port );
    dxClusterListWidget->setSortingEnabled (false);

    //dxClusterSpotItem * item = new dxClusterSpotItem(dxClusterListWidget, tr("Trying to connect to the server") + "\n", awards->getDefaultColor());
    dxClusterListWidget->addItem (new dxClusterSpotItem(dxClusterListWidget, tr("Trying to connect to the server") + "\n", awards->getDefaultColor()));
}

void DXClusterWidget::slotClusterDisplayError(QAbstractSocket::SocketError socketError)
{
   //qDebug() << Q_FUNC_INFO << ": " << socketError;
    QString errorMessage;

     switch (socketError) {
     case QAbstractSocket::RemoteHostClosedError:
         errorMessage = tr("The DXCluster server desconnected the session.");
         break;
     case QAbstractSocket::HostNotFoundError:
         errorMessage = tr("The host was not found. Please check:") + "\n\n" +
                                     tr ("- your network connection;\n"
                                     "- the host name and port settings.");
         break;
     case QAbstractSocket::ConnectionRefusedError:
         errorMessage = tr("The connection was refused by the peer. "
                                     "Make sure the DXCluster server is running, "
                                     "and check that the host name and port "
                                     "settings are correct.");
         break;
     default:
         errorMessage =  tr("The following error occurred: %1.").arg(socketError);
     }

   //qDebug() << Q_FUNC_INFO << errorMessage;
     QMessageBox::warning(this, tr("KLog DXCluster"), errorMessage);
 }

bool DXClusterWidget::checkIfNeedsToBePrinted(EntityStatus _entityStatus)
{
    //qDebug() << Q_FUNC_INFO;
    if (!showconfirmed && (_entityStatus.status == confirmed)) {
        return false;
    }

    if (!showhf && dataProxy->isHF(_entityStatus.bandId)) {
        return showwarc && dataProxy->isWARC(_entityStatus.bandId);
    }

    if (!showwarc && dataProxy->isWARC(_entityStatus.bandId)) {
        return false;
    }

    if (!showvhf && dataProxy->isVHF(_entityStatus.bandId)) {
        return false;
    }

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

void DXClusterWidget::addItemToClusterList(const QString &text, const QColor &color)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setForeground(QBrush(color));
    item->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    item->setText(text);
    dxClusterListWidget->insertItem(0, item);
}

void DXClusterWidget::slotClusterDataArrived()
{
   //qDebug() << Q_FUNC_INFO;
    QString dxClusterString = tcpSocket->readAll().trimmed().simplified();
   //qDebug() << Q_FUNC_INFO << " - 010:" << dxClusterString;

    if (dxClusterString.endsWith("\x07\x07\r\n"))
    {
       //qDebug() << Q_FUNC_INFO << " - 011";
        dxClusterString = dxClusterString.remove("\x07\x07\r\n");
    }
    //qDebug() << Q_FUNC_INFO << " Line-2: " << dxClusterString;

    if (dxClusterString.endsWith("\u0007\u0007\r\n"))
    {
       //qDebug() << Q_FUNC_INFO << " - 012";
        dxClusterString = dxClusterString.remove("\u0007\u0007\r\n");
    }

    if (dxClusterString.endsWith("\u0007\u0007"))
    {
       //qDebug() << Q_FUNC_INFO << " - 013";
        dxClusterString = dxClusterString.remove("\u0007\u0007");
    }

   //qDebug() << Q_FUNC_INFO << " - 020:" << dxClusterString;
    saveSpot(dxClusterString);

    TypeOfDXSpot typeOfSpot = parseReceivedData(dxClusterString);

    switch (typeOfSpot)
    {
    case dxde:
        //qDebug() << Q_FUNC_INFO << " - DX de";
        printSpot(dxClusterString);
        break;
    case shdx:
        //qDebug() << Q_FUNC_INFO << " - sh/dx";
        printSHDX(dxClusterString);
        break;
    default:
        //qDebug() << Q_FUNC_INFO << " - Other...";
        printOther(dxClusterString);
    }
}

void DXClusterWidget::printSpot(const QString _stringSpot)
{
   //qDebug() << Q_FUNC_INFO;
    QString stringToPrint = _stringSpot;
    DXSpot spot = readItem(stringToPrint);

    EntityStatus _entityStatus;
    _entityStatus.dxcc = -1;
    _entityStatus.logId = currentLog;

    if (spot.isValid()) {
       //qDebug() << Q_FUNC_INFO << " - spot is Valid";
       //qDebug() << Q_FUNC_INFO << " - DX      : " << spot.getDxCall();
       //qDebug() << Q_FUNC_INFO << " - Spotter : " << spot.getSpotter();
       //qDebug() << Q_FUNC_INFO << " - Freq    : " << spot.getFrequency().toQString();
       //qDebug() << Q_FUNC_INFO << " - Comment : " << spot.getComment();

        _entityStatus.dxcc = world->getQRZARRLId(spot.getDxCall());
        _entityStatus.bandId = dataProxy->getBandIdFromFreq(spot.getFrequency().toDouble());

        dxSpotColor = awards->getQRZDXStatusColor(_entityStatus);

        if (showDxMarathon && awards->isDXMarathonNeed(_entityStatus.dxcc, world->getQRZCqz(spot.getDxCall()), QDateTime::currentDateTime().date().year(), currentLog)) {
            stringToPrint += "  ### Needed for DXMarathon - " + QString::number(QDateTime::currentDateTime().date().year()) + " ###";
        }

        emit dxspotArrived(spot);
        _entityStatus.modeId = -1;

        if (!checkIfNeedsToBePrinted(_entityStatus)) {
            return;
        }
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " - spot is NOT Valid";
        dxSpotColor = awards->getDefaultColor();
    }

    int callPad = 15;
    if (spot.getDxCall().length()>11)
        callPad = spot.getDxCall().length() + 3;
    QString paddedDXCall = spot.getDxCall().leftJustified(callPad, ' ');
    QString paddedFreq = ((spot.getFrequency()).toQString(MHz)).rightJustified(10, ' ');
    callPad = 10;
    if (spot.getSpotter().length()>11)
        callPad = spot.getSpotter().length() + 3;
    QString paddedSpotter = QString(spot.getSpotter()+ ":").leftJustified(callPad, ' ');
    QString paddedComments = QString(spot.getComment()).leftJustified(28, ' ');
    QString dateformat;
    if (spot.getSHDX())
        dateformat = (QDateTime::currentDateTimeUtc()).toString("dd-MMM-yyyy hhmm")+"Z";
    else
        dateformat = (QDateTime::currentDateTimeUtc()).time().toString("HHmm")+"Z";
    QString mainaux = "DX de " + paddedSpotter  + paddedFreq + "   " + paddedDXCall + paddedComments + dateformat;
    addItemToClusterList(mainaux, dxSpotColor);
    //addItemToClusterList(stringToPrint, dxSpotColor);
}

void DXClusterWidget::printSHDX(const QString _stringSpot)
{
   //qDebug() << Q_FUNC_INFO << _stringSpot;
    //TODO: Check for spots like:
    //7160.0 GB70RS 19-Feb-2025 1202Z SES <2E0MNG> 50313.0 TZ1CE 19-Feb-2025 1201Z <F2> FT8 -12 dB 779 Hz <GW7SMV>


    QStringList spots = _stringSpot.split(">");

    QString spot;
    QString tmp;        // Stores a string that should be added to the following spot to check
                        // To detect cases like: 50313.0 TZ1CE 19-Feb-2025 1201Z <F2> FT8 -12 dB 779 Hz <GW7SMV>
    foreach (spot, spots)
    {
        tmp = tmp + spot;
        QStringList intraSpot = tmp.split(" ");
        QString lastToken = intraSpot.last();
        if (lastToken.startsWith("<"))
        {
            lastToken.remove(0,1);
            //lastToken.removeFirst();
            Callsign callsign(lastToken);
            if (callsign.isValid())
            {
                printSpot(tmp.simplified());
                tmp.clear();
            }
        }
    }
}
void DXClusterWidget::printOther(const QString _stringSpot)
{
    //qDebug() << Q_FUNC_INFO << _stringSpot;
    dxSpotColor = awards->getDefaultColor();
    addItemToClusterList(_stringSpot, dxSpotColor);
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
    addItemToClusterList(tr("Connected to server"), awards->getDefaultColor());
    dxClusterConnected = true;
    inputCommand->setFocus(Qt::OtherFocusReason);

    if (!dxClusterAlreadyConnected) {
        bool ok;
        QString callsignText = QInputDialog::getText(
            this, tr("KLog message"),
            tr("Enter your callsign to connect to the cluster:"),
            QLineEdit::Normal, myQrz.length() > 2 ? myQrz : "", &ok);

        if (!ok) {
            tcpSocket->disconnectFromHost();
            dxClusterConnected = false;
            return;
        }

        QString passwordText = QInputDialog::getText(
            this, tr("KLog message"),
            tr("Enter your password to connect to the cluster:\n(Just hit enter for no password)"),
            QLineEdit::Normal, "", &ok);

        if (!ok) {
            tcpSocket->abort();
            dxClusterConnected = false;
            return;
        }

        QTextStream os(tcpSocket);
        if (!callsignText.isEmpty()) {
            os << callsignText << "\n";
            sendButton->setText(tr("Disconnect"));
            clearButton->setText(tr("Clear"));
            dxClusterAlreadyConnected = true;
        } else {
            dxClusterAlreadyConnected = false;
        }

        inputCommand->setEnabled(true);
        inputCommand->setToolTip(tr("Enter here the commands to be sent to the DX-Cluster server."));
    }
}

void DXClusterWidget::slotClusterSocketConnectionClosed()
{
    //qDebug() << Q_FUNC_INFO;
    addItemToClusterList(tr("Connection closed by the server"), awards->getDefaultColor());

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
    //qDebug() << Q_FUNC_INFO << " - 000";
    if (!dxClusterConnected) {
        //qDebug() << Q_FUNC_INFO << " - 001";
        //qDebug() << Q_FUNC_INFO << " - Cluster already connected, END";
        connectToDXCluster();
        //qDebug() << Q_FUNC_INFO << " - 002 - END";

        return; // If we try to connect...
    }
    //qDebug() << Q_FUNC_INFO << " - 010";
    QString inputText = inputCommand->text();
    if (inputText.isEmpty()) {
        //qDebug() << Q_FUNC_INFO << " - 020";
        if (sendButton->text() == tr("Disconnect")) {
            // Disconnecting
            {
                //qDebug() << Q_FUNC_INFO << " - 030";
                QTextStream os(tcpSocket);
                os << "bye\n";
                //qDebug() << Q_FUNC_INFO << " - 033 - END";
                return;
            }
        }
        // If input is empty and send button is not "Disconnect", do nothing
        return;
    }
    //qDebug() << Q_FUNC_INFO << " - 050";
    // Write to the server
    QTextStream(tcpSocket) << inputText << "\n";
    inputCommand->clear();
    //qDebug() << Q_FUNC_INFO << " - END";
}

void DXClusterWidget::slotClusterClearLineInput()
{
   //qDebug() << Q_FUNC_INFO;
    inputCommand->clear();
}

void DXClusterWidget::slotClusterInputTextChanged()
{
    //qDebug() << Q_FUNC_INFO;
    QString inputText = inputCommand->text();
    if (inputText.isEmpty()) {
        sendButton->setText(tr("Disconnect"));
    } else if (dxClusterConnected) {
        sendButton->setText(tr("Send"));
    }
    clearButton->setText(tr("Clear"));
}

void DXClusterWidget::setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default)
{
  //qDebug() << Q_FUNC_INFO << ": " << _newOne.name(QColor::HexRgb) << "/" << _needed.name(QColor::HexRgb) << "/" << _worked.name(QColor::HexRgb) << "/" << _confirmed.name(QColor::HexRgb) << "/" << _default.name(QColor::HexRgb);
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

void DXClusterWidget::slotClusterDXClusterWidgetItemEntered(QListWidgetItem *item)
{
    //qDebug() << Q_FUNC_INFO;
    if (!item) return;

    DXSpot spot = readItem(item->data(0).toString().simplified());
    if (spot.isValid()) {
        item->setToolTip(world->getQRZEntityName(spot.getDxCall()));
    }
}


bool DXClusterWidget::isConnected()
{
   //qDebug() << Q_FUNC_INFO;
    return dxClusterConnected;
}

TypeOfDXSpot DXClusterWidget::parseReceivedData(const QString _stringSpot)
{
    //qDebug() << Q_FUNC_INFO << ": " << _stringSpot;

    QStringList fields = _stringSpot.split(" ");
    if (fields.length() < 2)
        return other;
    QString first = fields.at(0);
    QString second = fields.at(1);
    QString last = fields.last();
    Frequency  firstF;
    firstF.fromQString(first, KHz);

    if (first == "DX" && second == "de")
    {
        //qDebug() << Q_FUNC_INFO << ": Identified DX de";
        return dxde;
    }
    else if (firstF.isValid() && last.endsWith(">"))
    {
        //qDebug() << Q_FUNC_INFO << ": Identified sh/dx";
        //qDebug() << firstF.toQString();
        return shdx;
    }
    //qDebug() << Q_FUNC_INFO << ": Identified other";
    return other;
}

DXSpot DXClusterWidget::readItem(const QString _stringSpot)
{
   //qDebug() << Q_FUNC_INFO << ": " << _stringSpot;
    DXSpot spot;
    spot.clear();
    spot.setDateTime(QDateTime::currentDateTimeUtc());

    QStringList fields = _stringSpot.split(" ");

    if (fields.size() < 5)
        return spot;

    // Check for double click
    bool doubleClick = (fields.at(0) == "DOUBLE");
    if (doubleClick) {
        fields.removeFirst();
    }

    Frequency freq;
    if (fields.at(0) == "DX" && fields.at(1) == "de")
    {   //"DX de IC8CUQ: 14250.0 IZ3WUW tnx QSO 73! 1429Z"
       //qDebug() << Q_FUNC_INFO << ": Identified DX de";
        spot.setSHDX(false);
        QString spotter = fields.at(2);
        spotter.remove(':');
        spot.setSpotter(spotter.trimmed());
        if (doubleClick)
            freq.fromQString(fields.at(3), MHz);
        else
            freq.fromQString(fields.at(3), KHz);

        if (freq.isValid())
        {
           //qDebug() << Q_FUNC_INFO << ": Freq is Valid";
            spot.setFrequency(freq);
            spot.setDXCall(fields.at(4));

            QString aux = fields.last();
            aux.chop(1);            
            QTime time = QTime::fromString(aux,"HHmm");
            QDateTime datetime = QDateTime::currentDateTime();
            datetime.setTime(time);
            spot.setDateTime(datetime);

            aux.clear();
            int i;
            for (i = 5; i <= fields.count()-2; i++)
            {
                aux = aux + fields.at(i) + " ";
            }
            spot.setComment(aux.trimmed());
        }
        else
            return spot;
    }
    // Check if the QSO is comming from sh/dx
    freq.fromQString(fields.at(0), KHz);
    if (freq.isValid())
    {   // Coming from SH/DX
       //qDebug() << Q_FUNC_INFO << ": - Valid Freq";
        spot.setSHDX(true);
        spot.setFrequency(freq);
        spot.setDXCall(fields.at(1));
        QString spotter = fields.last();
        spotter.remove("<");
        spot.setSpotter(spotter);

        QString aux = (fields.last());
        aux.chop(1);
        QTime time;
        time.fromString(aux,"HHmm");
        QDateTime datetime;
        datetime.setTime(time);
        QDate date;
        aux = fields.at(fields.count()-2);
        date.fromString(aux, "DD-MMM-yyyy");

        datetime.setDate(date);
        spot.setDateTime(datetime);


        aux.clear();
        int i;
        for (i = 4; i <= fields.count()-2; i++)
        {
            aux = aux + fields.at(i) + " ";
        }
        spot.setComment(aux.trimmed());
        return spot;
    }

    // Check if the QSO is comming from a double click

    return spot;
}

void DXClusterWidget::setDXClusterServer(const QString &clusterToConnect, const int portToConnect)
{
    //qDebug() << Q_FUNC_INFO << clusterToConnect << ":" << portToConnect;
    server = clusterToConnect;
    port = quint16(portToConnect);
    if ((server.length()< 3) || (port <= 0))
    {
        server = "dxfun.com";
        port = 8000;
    }
    //qDebug() << Q_FUNC_INFO << server << ":" << port;
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
    return true;
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

    if (!openFile())
        return;

    //qDebug() << "DXClusterWidget::saveSpot: File Open";
    QTextStream out(saveSpotsFile);
    out << util->getDateTimeSQLiteStringFromDateTime(QDateTime::currentDateTime()) << " - " << _spot.simplified().toUtf8();
    out << Qt::endl;
    saveSpotsFile->close();
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

