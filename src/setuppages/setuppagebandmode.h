#ifndef SETUPPAGEBANDMODE_H
#define SETUPPAGEBANDMODE_H
/***************************************************************************
                          setuppagebandmode.h  -  description
                             -------------------
    begin                : nov 2011
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
#include <QtWidgets>
#include <QSqlQuery>
#include <QStringList>
#include <QListWidget>
#include "../dataproxy_sqlite.h"


class SetupPageBandMode : public QWidget
{
    Q_OBJECT
public:
    SetupPageBandMode(DataProxy_SQLite *dp, QWidget *parent=nullptr);
    ~SetupPageBandMode();


    QString getBands(); // 10m, 12m, 15m
    QString getModes(); //ssb, CW

    void setActiveBands(QStringList q);
    void setActiveModes(QStringList q);

signals:

public slots:

private:
    void addBands(QStringList _b); // read the available bands from the DB
    void addModes(QStringList _b); // read the available modes from the DB

    QListWidget *bandsListWidget, *modesListWidget;

    DataProxy_SQLite *dataProxy;

};

#endif // SETUPPAGEBANDMODE_H
