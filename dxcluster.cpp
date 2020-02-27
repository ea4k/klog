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
#include "dxcluster.h"

DXClusterWidget::DXClusterWidget(DataProxy_SQLite *dp, QWidget *parent)
          : QWidget(parent)
{
     //qDebug() << "DXClusterWidget::DXClusterWidget" << endl;
    dataProxy = dp;
    constrid = 1;
    awards = new Awards(dataProxy, Q_FUNC_INFO);
    util = new Utilities;
    saveSpotsFile = new QFile();


    initClass();
     //qDebug() << "DXClusterWidget::DXClusterWidget - END" << endl;
}

DXClusterWidget::DXClusterWidget(DataProxy_SQLite *dp, const QString &clusterToConnect, const int portToConnect, QWidget *parent)
          : QWidget(parent)
{
    //qDebug() << "DXClusterWidget::DXClusterWidget2" << clusterToConnect << QString::number(portToConnect) << endl;
   constrid = 2;
   util = new Utilities;
   saveSpotsFile = new QFile();

    initClass();
    server = clusterToConnect;
    port = quint16(portToConnect);

    dxSpotColor.setNamedColor("slategrey");
    //defaultColor.setNamedColor("slategrey");
    //neededColor.setNamedColor("slategrey");
    //workedColor.setNamedColor("slategrey");
    //confirmedColor.setNamedColor("slategrey");
    //newOneColor.setNamedColor("slategrey");
    dataProxy = dp;
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

    connect(sendButton , SIGNAL(clicked()), this, SLOT(slotClusterSendToServer()) );
    connect(inputCommand, SIGNAL(textChanged(QString)), this, SLOT(slotClusterInputTextChanged()) );

    //connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    connect(dxClusterListWidget, SIGNAL(itemDoubleClicked ( QListWidgetItem *)), this, SLOT(slotClusterDXClusterWidgetItemDoubleClicked( QListWidgetItem * )) );
    connect(dxClusterListWidget, SIGNAL(itemEntered ( QListWidgetItem *)), this, SLOT(slotClusterDXClusterWidgetItemEntered( QListWidgetItem * )) );
    connect(dxClusterListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(slotClusterDXClusterWidgetItemSelected() ) );

//void QListWidget::itemDoubleClicked ( QListWidgetItem * item ) [signal]

    // TO BE DELETED
    addData();

     //TESTADDSPOT();
     //qDebug() << "DXClusterWidget::DXClusterWidget2 - END"  << endl;

}

void DXClusterWidget::initClass()
{
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
    saveSpotsFile->setFileName(util->getSaveSpotsLogFile());
}

void DXClusterWidget::setMyQRZ(const QString &_qrz)
{
    if (_qrz.length()>2)
    {
        myQrz = _qrz;
    }

}

void DXClusterWidget::addData()
{
     //qDebug() << "DXClusterWidget::addData " << endl;

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



}

DXClusterWidget::~DXClusterWidget()
{
     //qDebug() << "DXClusterWidget::~DXClusterWidget" << endl;
}


void DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked( QListWidgetItem * item )
{
    //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: " << item->text() << endl;

    QStringList ql;
    ql.clear();

    if (item)
    {
        ql = readItem(item);
         //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: Length: " << QString::number(ql.length())  << endl;
        if (ql.length()==2)
        {
            ql << "double";
             //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: EMMITED"  << endl;
            emit dxspotclicked(ql);
        }
        else
        {
             //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: NOT EMMITED-1"  << endl;
        }
    }
    else
    {
         //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: NOT EMMITED-2 (no item)"  << endl;
    }

}

void DXClusterWidget::connectToDXCluster()
{
     //qDebug() << "DXClusterWidget::connectToDXCluster" << endl;
    if (dxClusterConnected)
    {
         //qDebug() << "DXClusterWidget::connectToDXCluster: - Already connected!!" << endl;
        return; // If we are connected we don't want to start another connection
    }

    connect(tcpSocket, SIGNAL(connected()), SLOT(slotClusterSocketConnected()) );
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotClusterDataArrived() ));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotClusterDisplayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), SLOT(slotClusterSocketConnectionClosed()) );
    connect(inputCommand, SIGNAL(returnPressed()), this, SLOT(slotClusterSendToServer()) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClusterClearLineInput()) );
    //openFile(); // This functions opens the file to save the DX-Cluster activity. The file will be closed when the DX is disconnected.
    tcpSocket->connectToHost( server, port );
    dxClusterListWidget->setSortingEnabled (false);

    dxClusterSpotItem * item = new dxClusterSpotItem(dxClusterListWidget, tr("Trying to connect to the server") + "\n", awards->getDefaultColor());

}


void DXClusterWidget::slotClusterDisplayError(QAbstractSocket::SocketError socketError)
{
     //qDebug() << "DXClusterWidget:displayError:" << endl;
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
    //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: " << _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< endl;
    QStringList qs;
    qs.clear();
    qs << _DXEntity << QString::number(_band) << QString::number(_mode)  << QString::number(currentLog);
    //bool isConfirmed = false;
    bool status = awards->isThisSpotConfirmed (qs);

    //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: Status: " << _DXEntity << "/" << QString::number(status);

    if (!showconfirmed)
    {
        //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is confirmed? ("<< QString::number(status)<< ")" << endl;
        if (status)
        {
            //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: It is confirmed: DON'T' print: " << _DXEntity <<"/" << dataProxy->getNameFromBandId(_band) << endl;
            return false;
        }
    }

    if (!showhf)
    {
        //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is HF?" << endl;
        if (dataProxy->isHF(_band))
        {
            //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: Not showing HF but... is it WARC?" << endl;
            if ( (showwarc) && dataProxy->isWARC(_band) )
            {
                //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: Not showing HF but... is WARC, print!" << endl;
                return true;
            }

            //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is HF: DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< endl;
            return false;
        }
        else
        {
            //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is NOT HF" << endl;
        }
    }

    if (!showwarc)
    {
        //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is WARC?" << endl;
        if (dataProxy->isWARC(_band))
        {
            //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is WARC, DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< endl;
            return false;
        }
        else
        {
            //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is NOT WARC" << endl;
        }
    }

    if (!showvhf)
    {
         //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is VHF?" << endl;
        if (dataProxy->isVHF(_band))
        {
             //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is VHF, DON'T print: "<< _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< endl;
            return false;
        }
        else
        {
             //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: is NOT VHF " << endl;
        }
    }
     //qDebug() << "DXClusterWidget::checkIfNeedsToBePrinted: returns TRUE and will be printed: " << _DXEntity << "/" << dataProxy->getNameFromBandId(_band) << QString::number(_mode)<< endl;
    return true;
}

void DXClusterWidget::setCurrentLog(const int _log)
{
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
    qDebug() << "DXClusterWidget::slotClusterDataArrived" << endl;
    QStringList qs;
    QString dxClusterString;
    QString dxCall;
    QString dxFrequency;
    QString spotBand;
    spotBand = "-1";
    //bool isADXSpot = false;
    int dxEntity = -1;

    while ( tcpSocket->canReadLine() )
    {
        dxClusterString =  tcpSocket->readLine();
        dxClusterString = dxClusterString.trimmed();
        saveSpot(dxClusterString);

        QStringList tokens = dxClusterString.split(" ", QString::SkipEmptyParts);
        if (tokens.size()<2){
            return;
        }
        // It is a "DX de SP0TTER FREC DXCALL"
        //0 = DX, 1 = de, 2 = spotter, 3 = Freq, 4 = dxcall, 5 = comment
        //qDebug() << "DXClusterWidget::slotClusterDataArrived: " << "DXCLUSTER->" << dxClusterString << "\nTOKENS: " << tokens << endl;

        if ((tokens[0] == "DX") && (tokens[1] == "de"))
        {
            //qDebug() << "******************** DXClusterWidget::slotClusterDataArrived: DX DE" << endl;
            //isADXSpot = true;
            QString spotter = tokens[2];
            spotter.truncate(spotter.size() - 1);
            dxFrequency = tokens[3];
            // Convert KHz to MHz...
            //dxFrequency = QString::number(abs (dxFrequency.toFloat())/1000);
            dxFrequency = QString::number( (dxFrequency.toDouble())/1000);
            dxCall = tokens[4];
            dxEntity = world->getQRZARRLId(dxCall);
            //
            spotBand = QString::number(dataProxy->getBandIdFromFreq(  dxFrequency.toDouble()  ) );


            qs.clear();
            //spotBand = QString::number(world->getBandIdFromFreq(  dxFrequency  ) );
            qs << QString::number(dxEntity) << spotBand << "-1" << QString::number(currentLog) ;
             //qDebug() << "DXClusterWidget::slotClusterDataArrived: Calling-2: " << QString::number(dxEntity) << endl;
            dxSpotColor = awards->getQRZDXStatusColor(qs);
            if  (showDxMarathon)
            {
                if (awards->isDXMarathonNeed(dxEntity, world->getQRZCqz(dxCall), QDateTime::currentDateTime().date().year(), currentLog))
                {
                    dxClusterString = dxClusterString + "  ### Needed for DXMarathon - " + QString::number(QDateTime::currentDateTime().date().year()) + " ###";
                }
            }


            //qDebug() << "DX de ->" << "Spotter: " << spotter << "Freq: "<< dxFrequency << "DX: " << dxCall << endl;

        }
        else if ((tokens[0] == "To") && (tokens[1] == "ALL"))
        {
            //qDebug() << "DXClusterWidget::slotClusterDataArrived: TO ALL" << endl;
            dxSpotColor = awards->getDefaultColor();
        }
        else if ( (dxClusterString.length()>=5) && (world->checkQRZValidFormat(tokens[1])) && (tokens[0]!="login:"))
        { // Freq / DXCall / Date // time
           //_qs << QRZ << Freq in MHz << lognumber;
           //qDebug() << "DXClusterWidget::slotClusterDataArrived: LENGTH >= 5" << endl;
           //qDebug() << "DXClusterWidget::slotClusterDataArrived: token0=" << tokens[0] << " / token1=" << tokens[1] << endl;

            //isADXSpot = true;
            dxCall = tokens[1];
            dxFrequency = tokens[0];
            dxFrequency = QString::number( (dxFrequency.toDouble())/1000);

            qs.clear();
            spotBand = QString::number(dataProxy->getBandIdFromFreq(  dxFrequency.toDouble()  ) );
            dxEntity = world->getQRZARRLId(dxCall);
             //qDebug() << "DXClusterWidget::slotClusterDataArrived: Calling-1: " << QString::number(dxEntity) << endl;
            qs << QString::number(dxEntity) << spotBand << "-1" << QString::number(currentLog) ;
            dxSpotColor = awards->getQRZDXStatusColor(qs);
            if (showDxMarathon)
            {
                if (awards->isDXMarathonNeed(dxEntity, world->getQRZCqz(dxCall), QDateTime::currentDateTime().date().year(), currentLog))
                {
                    dxClusterString = dxClusterString + "  ### Needed for DXMarathon - " + QString::number(QDateTime::currentDateTime().date().year()) + " ###";
                }
            }


        }
        else
        {
             //qDebug() << "DXClusterWidget::slotClusterDataArrived: DEFAULT" << endl;
            dxSpotColor = awards->getDefaultColor();
        }

      //  qs.clear();
        //spotBand = QString::number(world->getBandIdFromFreq(  dxFrequency  ) );
        //qs << dxCall << spotBand << "0";

        //dxSpotColor = awards->getQRZDXStatusColor(qs);
         //qDebug() << "DXClusterWidget::slotClusterDataArrived: Call/dxSpotColor: " << dxCall <<"/"<< dxSpotColor.name() << endl;
        //dxClusterSpotItem * item = new dxClusterSpotItem(dxClusterListWidget, dxClusterString, dxSpotColor);
        //TODO: Change the "-1" by the mode
        if (!checkIfNeedsToBePrinted(QString::number(dxEntity), spotBand.toInt(), -1))
        {
            //qDebug() << "DXClusterWidget::slotClusterDataArrived - Not to be printed!: " << dxCall << endl;
            return;
        }

//        QString flagSt;
//        QString aux;

        QListWidgetItem *item = new QListWidgetItem();
        item->setForeground(QBrush(dxSpotColor));
        item->setText(dxClusterString);

/*
        if (dxEntity>0)
        {
            flagSt.clear();

            aux = dataProxy->getISOName(dxEntity);
            if (aux.length()>1)
            {
                flagSt = ":/" + aux + ".png";
            }
            else
            {
                flagSt.clear();
            }

            flagSt = ":/flags/" + dataProxy->getISOName(dxEntity) + ".png";
            QIcon flagIcon(flagSt);
            item->setIcon(flagIcon);

        }
*/
        dxClusterListWidget->insertItem(0,item);

        //dxClusterListWidget->insertItem(0,item);
        //QListWidgetItem *item = new QListWidgetItem();
        //item->setForeground(QBrush(awards->getQRZDXStatusColor(qs)));
        //item->setText(dxClusterString);
        //dxClusterListWidget->insertItem(0,item);
    }

     //qDebug() << "--------------------- DXClusterWidget::slotClusterDataArrived: " << dxClusterString << endl;

    //qDebug() << "DXClusterWidget::slotClusterDataArrived: " << dxClusterString << endl;

}

void DXClusterWidget::slotClusterSocketConnected()
{
     //qDebug() << "DXClusterWidget::slotClusterSocketConnected" << endl;

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

        //QString callsignText = QInputDialog::getText(this, tr("KLog message"), tr("Enter your callsign to connect to the cluster:"), QLineEdit::Normal, "", &ok);
        QString passwordText = QInputDialog::getText(this, tr("KLog message"), tr("Enter your password to connect to the cluster:\n(Just hit enter for no password)"), QLineEdit::Normal, "", &ok);
        QTextStream os(tcpSocket);
        if ( callsignText.length() > 2 && ok ) {
            os << callsignText << "\n";
        //TODO: Check the DXCluster answer and enter the password if needed.
            sendButton->setText(tr("Disconnect"));
            clearButton->setText(tr("Clear"));
            dxClusterAlreadyConnected = true;
        } else {
            os << tr("Not logged on, you may need to enter your callsign again.") << "\n";
            dxClusterAlreadyConnected = false;
        }
        inputCommand->setEnabled(true);
        inputCommand->setToolTip(tr("Enter here the commands to be sent to the DX-Cluster server."));

    }
}

void DXClusterWidget::slotClusterSocketConnectionClosed()
{
     //qDebug() << "DXClusterWidget::slotClusterSocketConnectionClosed" << endl;
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
     //qDebug() << "DXClusterWidget::slotClusterSendToServer()" << endl;
    if (!dxClusterConnected)
    {

        connectToDXCluster();
        return; // If we try to connect...
    }
    if ( inputCommand ->text().length() < 1 )
    {
         //qDebug() << "DXClusterWidget::slotClusterSendToServer() - Vacio..." << endl;
        QTextStream os(tcpSocket);
        os << "bye\n";
        return;
    }
    //  write to the server
    QTextStream os(tcpSocket);
    os << inputCommand ->text() << "\n";
    inputCommand ->clear();
}

void DXClusterWidget::slotClusterClearLineInput()
{
     //qDebug() << "DXClusterWidget::slotClusterClearLineInput" << endl;

    if ( ((inputCommand->text()).length()) <= 0 )
    {
        if ( dxClusterConnected )
        {
            QTextStream os(tcpSocket);
            os << "bye\n";
        }
        else
        {
        }
    }
    else
    {
        inputCommand->clear();
    }

}

void DXClusterWidget::slotClusterInputTextChanged()
{
     //qDebug() << "DXClusterWidget::slotClusterInputTextChanged" << endl;

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
    else
    {}
}

void DXClusterWidget::setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default)
{
     //qDebug() << "DXClusterWidget::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default << endl;
    // Just to pass the colors to the awards class
    awards->setColors(_newOne,  _needed, _worked,  _confirmed, _default);

}

void DXClusterWidget::setDXClusterSpotConfig(bool _showhf, bool _showvhf, bool _showwarc, bool _showworked, bool _showconfirmed, bool _showann, bool _showwwv, bool _showwcy )
{
     //qDebug() << "DXClusterWidget::setDXClusterSpotConfig " << endl;

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
     //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemSelected " << endl;
    QListWidgetItem * item = dxClusterListWidget->currentItem();

    QStringList ql;
    ql.clear();

    ql = readItem(item);
    if (ql.length()==2)
    {
        ql << "selected";
        emit dxspotclicked(ql);
    }
    else
    {
    }

}

void DXClusterWidget::slotClusterDXClusterWidgetItemEntered( QListWidgetItem * item )
{
     //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemEntered" << endl;
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
        else
        {
        }
    }
    else
    {
    }

}


bool DXClusterWidget::isConnected()
{
    return dxClusterConnected;
}

QStringList DXClusterWidget::readItem(QListWidgetItem * item)
{
    qDebug() << "DXClusterWidget::readItem" << endl;

    QStringList fields;
    QString dxClusterString;
    //int currentEntity;
    QString dxCallsign, dxFreq;
    bool FirstFrecOK;

    if (item) {

        fields.clear();
        dxClusterString = ((item->data(0)).toString()).simplified();        
        fields << dxClusterString.split(" ");

        (fields.at(0)).toFloat(&FirstFrecOK); // Just to see if the first string is a frecuency

        if ( (fields.at(0) == "DX" ) && (fields.at(1) == "de" ) )
        { // DX de EA4K: 21200.1 EA0JC The comment 1550

            if ( world->getQRZARRLId(fields.at(4))> 0 )
            {
                 //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: ARRLid: " << QString::number(world->getQRZARRLId(fields.at(4)))  << endl;
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
            { // The call is not from a valid country
              // TODO: if it is from a new entity/prefix it would not work.
                 //qDebug() << "DXClusterWidget::slotClusterDXClusterWidgetItemDoubleClicked: Entity not valid"  << endl;
                dxCallsign.clear();
                dxFreq.clear();
                fields.clear();
            }
        }
        //else if (( isAFrecuency(fields.at(0) ) ) && ( isACall(fields.at(1)) ) )
        else if ( (((fields.at(0)).toDouble()) > 0.0 )&& ( world->getQRZARRLId(fields.at(1))> 0 ) )
        { // 14205.0 EA0JC    5-Mar-2012 1500Z    <EA4K>

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
    server = clusterToConnect;
    port = quint16(portToConnect);
     //qDebug() << "DXClusterWidget::setDXClusterServer: " << server << ":"<< QString::number(port)  << endl;
}

void DXClusterWidget::setDXMarathon (const bool _enable)
{
    showDxMarathon = _enable;
}

void  DXClusterWidget::setSaveSpots (const bool _enable)
{
    saveSpots = _enable;
}

bool DXClusterWidget::openFile()
{
    if (saveSpotsFile->isOpen())
    {
        return true;
    }
    if (!saveSpotsFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        QString aux = tr("It was not possible to open the a file to save the DX-Spots for writting. DX-Cluster activity will be saved!");
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

void DXClusterWidget::saveSpot(const QString &_spot)
{
    qDebug() << "DXClusterWidget::saveSpot: " << _spot  << endl;
    if (!saveSpots)
    {
        qDebug() << "DXClusterWidget::saveSpot: Not saving" << endl;
        return;
    }
    else
    {
        if (openFile())
        {
            qDebug() << "DXClusterWidget::saveSpot: File Open" << endl;
            QTextStream out(saveSpotsFile);
            //out << _spot << endl;
            out << (QDateTime::currentDateTime()).toString("yyyy/MM/dd-hh:mm:ss") << " - " << _spot  << endl;
            saveSpotsFile->close();
        }
        else {
            {
                qDebug() << "DXClusterWidget::saveSpot: File NOT Open" << endl;
            }
        }
    }
}

/*
void DXClusterWidget::TESTADDSPOT()
{
     //qDebug() << "DXClusterWidget::TESTADDSPOT "   << endl;
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
     //qDebug() << "dxClusterSpotItem::dxClusterSpotItem - Constructor" << endl;
    spotColor = color;
    setText(spot);
    // Experimenting with fonts for the cluster
    QFont f("Helvetica");
    f.setFixedPitch(true);
    setFont(f);
}

dxClusterSpotItem::~dxClusterSpotItem()
{
     //qDebug() << "dxClusterSpotItem::dxClusterSpotItem - Destructor" << endl;
}

