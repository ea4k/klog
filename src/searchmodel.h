#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H
/***************************************************************************
                          searchmodel.h  -  description
                             -------------------
    begin                : sep 2020
    copyright            : (C) 2020 by Jaime Robles
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
#include "awards.h"
#include "dataproxy_sqlite.h"


class SearchModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    SearchModel(DataProxy_SQLite *dp, QObject *parent);
    void createSearchModel(const int _i);
    void setFilterString(const QString &_st);
    void setStationCallsignInHeader(const bool _s);
    void update();
    void setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default);
    void setDXCCColumn(const int _i);
    void setBandIdColumn(const int _i);
    void setModeIdColumn(const int _i);
    void setLogNColumn(const int _i);

private:
    //void setColumnsToDX();
    //QSqlRelationalTableModel *SearchModel;
    QVariant data(const QModelIndex &index, int role) const;
    Awards *award;

    DataProxy_SQLite *dataProxy;
    bool stationCallsignInHeader;
    int dxcc, bandid, modeid, logn;
signals:
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution

};

#endif // SEARCHMODEL_H


