#ifndef KLOG_SETUPPAGES_SETUPPAGELOGVIEW_H
#define KLOG_SETUPPAGES_SETUPPAGELOGVIEW_H
/***************************************************************************
                          setuppagelogview.h  -  description
                             -------------------
    begin                : ago 2021
    copyright            : (C) 2021 by Jaime Robles
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
#include "../utilities.h"


class SetupPageLogView : public QWidget
{
    Q_OBJECT

public:
    SetupPageLogView(DataProxy_SQLite *dp, QWidget *parent=nullptr);
    ~SetupPageLogView();
    void init();
    //QString getFields();
    void saveSettings();
    void loadSettings();

signals:

public slots:

private:
    void addFields (QStringList _b); // read the available fields from the DB
    QStringList getActiveFields();
    void setActiveFields(QStringList _q);

    QListWidget *fieldsListWidget;

    DataProxy_SQLite *dataProxy;
};

#endif // SETUPPAGELOGVIEW_H
