#ifndef DXCLUSTER_H
#define DXCLUSTER_H
/***************************************************************************
    dxcluster.h  -  description
       -------------------
begin                : oct 2011
copyright            : (C) 2011 by Jaime Robles
email                : jaime@robles.es
***************************************************************************/

/*****************************************************************************
* This file is part of KLog.                                             *
*                                                                           *
*    KLog is free software: you can redistribute it and/or modify        *
*    it under the terms of the GNU General Public License as published by   *
*    the Free Software Foundation, either version 3 of the License, or      *
*    (at your option) any later version.                                    *
*                                                                           *
*    KLog is distributed in the hope that it will be useful,             *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*    GNU General Public License for more details.                           *
*                                                                           *
*    You should have received a copy of the GNU General Public License      *
*    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
*                                                                           *
*****************************************************************************/

#include <QWidget>
//#include <QtWidgets>
#include <QtGui>
#include <QTcpSocket>
#include <QObject>
#include "awards.h"
#include "world.h"

class QWidget;
class QTcpSocket;

class DXClusterWidget : public QWidget
{
     Q_OBJECT

 public:
    DXClusterWidget(QWidget *parent );

    //DXClusterWidget(QWidget *parent);
    DXClusterWidget(const QString &clusterToConnect, const int portToConnect, QWidget *parent );

    void setColors (const QString _newOne, const QString _needed, const QString _worked, const QString _confirmed, const QString _default);
    void setDXClusterSpotConfig(bool _showhf, bool _showvhf, bool _showwarc, bool _showworked, bool _showconfirmed, bool _showann, bool _showwwv, bool _showwcy );
    void setDXClusterServer(const QString &clusterToConnect, const int portToConnect);


    ~DXClusterWidget();


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

signals:
    void dxspotclicked(const QStringList _qs); // DXSpotCall, DX-Freq, doubleClicked

private:

    void connectToDXCluster();
    QStringList readItem(QListWidgetItem * item);

    void addData(); //TO BE DELETED, JUST FOR TESTING PURPOSES

    QTcpSocket *tcpSocket;
    QListWidget *dxClusterListWidget;
    QLineEdit *inputCommand;
    QPushButton *sendButton;
    QPushButton *clearButton;

    bool dxClusterConnected;
    bool dxClusterAlreadyConnected;
    QString server;
    quint16 port;
    quint16 blockSize;

    QColor dxSpotColor;

    World *world;
    Awards *awards;

    bool showhf, showvhf, showwarc, showworked, showconfirmed, showann,  showwwv, showwcy;
    bool dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY;


 };

class dxClusterSpotItem : public QListWidgetItem {

public:

    dxClusterSpotItem( QListWidget* parent, const QString& spot, const QColor& color );
    ~dxClusterSpotItem();

protected:

  QColor spotColor;

};

#endif // DXCLUSTER_H
