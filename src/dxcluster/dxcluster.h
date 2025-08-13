#ifndef KLOG_CLUSTER_DXCLUSTER_H
#define KLOG_CLUSTER_DXCLUSTER_H
/***************************************************************************
    dxcluster.h  -  description
       -------------------
begin                : oct 2011
copyright            : (C) 2011 by Jaime Robles
email                : jaime@robles.es
***************************************************************************/

/****************************************************************************
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

#include <QSettings>
//#include <QtWidgets>
#include <QtWidgets>
#include <QTcpSocket>
#include <QObject>
#include "../awards.h"
#include "dxspot.h"
#include "../world.h"
#include "../utilities.h"
#include "../dataproxy_sqlite.h"
#include "../frequency.h"
#include "../klogdefinitions.h"

class QWidget;
class QTcpSocket;
class Frequency;
enum TypeOfDXSpot {dxde, shdx, other};

class DXClusterWidget : public QWidget
{
     Q_OBJECT

 public:
    //DXClusterWidget(Awards *awards, QWidget *parent );
     explicit DXClusterWidget(Awards *awards, World *injectedWorld, QWidget *parent = nullptr);


    void init();
    ~DXClusterWidget();

    void setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default);
    void setDXClusterSpotConfig(bool _showhf, bool _showvhf, bool _showwarc, bool _showworked, bool _showconfirmed, bool _showann, bool _showwwv, bool _showwcy );
    //void setDXClusterServer(const QString &clusterToConnect, const int portToConnect);
    void setCurrentLog(const int _log);
    bool isConnected();
    void setMyQRZ(const QString &_qrz);
    void setDXMarathon (const bool _enable);
    void setSaveSpots (const bool _enable);
    void loadSettings();
    void setDXClusterServer(const QString &clusterToConnect, const int portToConnect);

    void rightButtonFromLogMenu(const DXSpot &_spot);

    //void sendSpotToCluster(const QString &_dx, const QString &_freq);

private slots:
    void slotClusterDisplayError(QAbstractSocket::SocketError socketError);
    void slotClusterDataArrived();
    void slotClusterSocketConnected();
    void slotClusterSocketConnectionClosed();
    void slotClusterSendToServer();
    void slotClusterClearLineInput();
    void slotClusterInputTextChanged();

    void slotClusterDXClusterWidgetItemDoubleClicked( QListWidgetItem * item );
    void slotClusterDXClusterWidgetItemEntered( QListWidgetItem * item);
    void slotClusterDXClusterWidgetItemSelected();
    void slotRightButton (const QPoint& pos);
    void slotCheckQRZCom();

signals:
    void dxspotclicked(const DXSpot &_dxSpot); // DXSpotCall, DX-Freq, doubleClicked
    //void dxspotArrived(const QString &_call, double _f);
    void dxspotArrived(const QString &_call, const Frequency &_f);
    void dxspotArrived(const DXSpot &_sp);
    //void dxspot(const QString &_spot); // The text string to be saved

private:
    Awards *awards; // Reference to the Awards instance
    //void TESTADDSPOT(); // Just a test spot
    void addItemToClusterList(const QString &text, const QColor &color); // Adds a message to the list
    void createActions();
    void connectToDXCluster();
    void printSpot(const QString _stringSpot);
    void printSHDX(const QString _stringSpot);
    void printOther(const QString _stringSpot);
    //QStringList readItem(QListWidgetItem * _stringSpot);
    DXSpot readItem(const QString _stringSpot);
    TypeOfDXSpot parseReceivedData(const QString _stringSpot);       // Parses the received data to identify if it is a "DX de" or other line.
    bool checkIfNeedsToBePrinted(EntityStatus _entityStatus);
    void saveSpot (const QString &_spot);
    bool openFile();
    QString cleanSpotter(const QString _call);
    void addData(); //TO BE DELETED, JUST FOR TESTING PURPOSES

    QTcpSocket *tcpSocket = nullptr;
    QListWidget *dxClusterListWidget;
    QLineEdit *inputCommand;
    QPushButton *sendButton;
    QPushButton *clearButton;

    bool dxClusterConnected;
    bool dxClusterAlreadyConnected;
    QString server;
    quint16 port;
    //quint16 blockSize;

    QColor dxSpotColor;

    World *world;
    DataProxy_SQLite *dataProxy;
    Utilities *util;

    bool showhf, showvhf, showwarc, showworked, showconfirmed, showann,  showwwv, showwcy;
    //bool dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY;
    bool showDxMarathon;

    QString myQrz;
    int currentLog;

    QFile *saveSpotsFile;
    bool saveSpots; // write/save the spots to a file

    QAction *checkQRZCOMFromLogAct;
 };

class dxClusterSpotItem : public QListWidgetItem
{
public:
    dxClusterSpotItem( QListWidget* parent, const QString& spot, const QColor& color );
    ~dxClusterSpotItem();

protected:
  QColor spotColor;
};

#endif // DXCLUSTER_H
