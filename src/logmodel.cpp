/***************************************************************************
                          logmodel.cpp  -  description
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

#include "logmodel.h"

LogModel::LogModel(DataProxy_SQLite *dp, QObject *parent):QSqlRelationalTableModel(parent)
{
    //qDebug() << Q_FUNC_INFO ;
    //logModel = new QSqlRelationalTableModel(this);
    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO << "llamando a filterValidFields";
    columns = dataProxy->filterValidFields(util->getDefaultLogFields());
    setTable("log");
    setEditStrategy(QSqlTableModel::OnFieldChange);
    //qDebug() << Q_FUNC_INFO << " - END";
}


void LogModel::createlogModel(const int _i)
{
/*
    Log_Id = 0,
    Log_Name = 1,
    Log_BandId = 2,
    Log_ModeId = 3,
    Log_DateId = 4,
    Log_TimeId = 5

setRelation ( int column, const QSqlRelation & relation )

    model->setTable("employee");
    model->setRelation(2, QSqlRelation("city", "id", "name"));

The setRelation() call specifies that column 2 in table employee
is a foreign key that maps with field id of table city, and that
the view should present the city's name field to the user.

*/

/*
This should be coherent with the logview
*/

    //qDebug() << Q_FUNC_INFO ;


    QString stringQuery = QString("lognumber='%1'").arg(_i);
    //QSqlQuery query(stringQuery);
    setFilter(stringQuery);
    setColumns(columns);

    select();
    //qDebug() << Q_FUNC_INFO << " - END";
}

 void LogModel::setColumns(const QStringList &_columns)
 {
     //qDebug() << Q_FUNC_INFO ;
     //QString auxt;
     //foreach(auxt, _columns)
     //{
     //    //qDebug() << Q_FUNC_INFO << ": " << auxt;
     //}
    columns.clear();
    //qDebug() << Q_FUNC_INFO << "llamando a filterValidFields";
    columns << dataProxy->filterValidFields(_columns);

     QSqlQuery q;
     QString stringQuery = QString("SELECT * from log LIMIT 1");
     QSqlRecord rec; // = q.record();

     int nameCol;

     bool sqlOK = q.exec(stringQuery);
     if (!sqlOK)
     {
         emit queryError(Q_FUNC_INFO, q.lastError().databaseText(), q.lastError().nativeErrorCode(), q.lastQuery());

     }
     q.next();
     rec = q.record(); // Number of columns

    //qDebug() << "LogModel::createlogModel - columns: " << QString::number(rec.count()) << QT_ENDL;

     if (_columns.contains("bandid"))
     {
         nameCol = rec.indexOf("bandid");
         setRelation(nameCol, QSqlRelation("band", "id", "name"));
     }

    if (_columns.contains("band_rx"))
    {
        nameCol = rec.indexOf("band_rx");
        setRelation(nameCol, QSqlRelation("band", "id", "name"));
    }

    if (_columns.contains("modeid"))
    {
        nameCol = rec.indexOf("modeid");
        setRelation(nameCol, QSqlRelation("mode", "id", "submode"));
    }

     if (_columns.contains("dxcc"))
     {
         nameCol = rec.indexOf("dxcc");
         setRelation(nameCol, QSqlRelation("entity", "dxcc", "name"));
     }


     nameCol = rec.indexOf("id");
     setSort(nameCol, Qt::AscendingOrder);
     QString aux;

     foreach(aux, columns)
     {
         nameCol = rec.indexOf(aux);
         setHeaderData(nameCol, Qt::Horizontal, util->getLogColumnName(aux));
         //qDebug() << Q_FUNC_INFO << ": - " << aux;
     }
     //qDebug() << Q_FUNC_INFO << " - END";
 }
/*
 void LogModel::showColumn(const QString &_columnName)
 {
     QString stringQuery;
     stringQuery = QString("SELECT * FROM log LIMIT 1");
     QSqlQuery query;
     bool sqlOK = query.exec(stringQuery);
     if (!sqlOK)
     {
         emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
     }
     QSqlRecord rec;
     rec = query.record(); // Number of columns

     int nameCol = rec.indexOf(_columnName);
     setHeaderData(nameCol, Qt::Horizontal, _columnName);

 }
*/
