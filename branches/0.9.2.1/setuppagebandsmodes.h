#ifndef SETUPPAGEBANDSMODES_H
#define SETUPPAGEBANDSMODES_H
/***************************************************************************
                          setuppagebandsmodes.h  -  description
                             -------------------
    begin                : sept 2011
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

//#include <QtWidgets>
#include <QtGui>
#include <QSqlQuery>
#include <QStringList>
#include <QListWidget>
#include "dataproxy.h"
#include "dataproxy_sqlite.h"


class SetupPageBandsModes : public QWidget {
    Q_OBJECT

public:
    SetupPageBandsModes(QWidget *parent=0);
    ~SetupPageBandsModes();

    QString getBands(); // 10m, 12m, 15m
    QString getModes(); //ssb, CW

    void setActiveBands(QStringList q);
    void setActiveModes(QStringList q);

private slots:
    void slotBandActiveItemDoubleClicked ( QListWidgetItem * item );
    void slotBandNotActiveItemDoubleClicked ( QListWidgetItem * item );
    void slotBandSelButtonClicked( );
    void slotBandUnSelButtonClicked( );
    void slotModeSelButtonClicked( );
    void slotModeUnSelButtonClicked( );

private:
    void createActions();

    //QWidget *bandsWidget;
    QListWidget *bandsNotActiveListWidget, *bandsActiveListWidget;
    QListWidget *modesNotActiveListWidget, *modesActiveListWidget;
    QStringList bands, modes;
    QPushButton *bandSelPushButton, *bandUnSelPushButton, *modeSelPushButton, *modeUnSelPushButton;

    DataProxy *dataProxy;

    //QCheckBox *10m;
};


#endif // SETUPPAGEBANDSMODES_H
