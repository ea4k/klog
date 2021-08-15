#ifndef LOGMODEL_H
#define LOGMODEL_H
/***************************************************************************
                          logmodel.h  -  description
                             -------------------
    begin                : june 2017
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
#include <QDebug>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "dataproxy_sqlite.h"
#include "utilities.h"


class LogModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    LogModel(DataProxy_SQLite *dp, QObject *parent);
    void createlogModel(const int _i);
    void setColumns(const QStringList &_columns);
private:
    //void showColumn(const QString &_columnName);

    //QSqlRelationalTableModel *logModel;
    DataProxy_SQLite *dataProxy;
    Utilities *util;

    QStringList columns;

signals:
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution

};

#endif // LOGMODEL_H


/*
class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};
*/
