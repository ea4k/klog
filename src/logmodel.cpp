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
       //qDebug() << "LogModel::LogModel "  << Qt::endl;
    //logModel = new QSqlRelationalTableModel(this);
    dataProxy = dp;
    setTable("log");
    setEditStrategy(QSqlTableModel::OnFieldChange);
       //qDebug() << "LogModel::LogModel - END"  << Qt::endl;
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

      //qDebug() << "LogModel::createlogModel: log: " << QString::number(_i) << Qt::endl;

   //QString contestMode = dataProxy->getLogTypeOfUserLog(_i);


    QString stringQuery = QString("lognumber='%1'").arg(_i);
    QSqlQuery query(stringQuery);
    setFilter(stringQuery);
    setColumnsToDX();

    /*

   //if (contestMode.compare("DX"))
    if (contestMode == "DX")
   {
          //qDebug() << "LogModel::createlogModel: found type DX" << Qt::endl;

   }
   else if (contestMode == "CQ-WW-SSB")
   {
          //qDebug() << "LogModel::createlogModel: found type CQ-WW-SSB" << Qt::endl;
   }
   else
   {
       // THIS POINT SHOULD NOT BE REACHED. It means that there is a kind of contest not supported.
       // Maybe the way should be to move ALL the actions from DX here.
          //qDebug() << "LogModel::createlogModel: log type NOT found" << Qt::endl;
   }
    */

    select();


}

 void LogModel::setColumnsToDX()
 {
        //qDebug() << "LogModel::setColumnsToDX"  << Qt::endl;

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

    //qDebug() << "LogModel::createlogModel - columns: " << QString::number(rec.count()) << Qt::endl;

     nameCol = rec.indexOf("bandid");
     setRelation(nameCol, QSqlRelation("band", "id", "name"));

     nameCol = rec.indexOf("modeid");
     setRelation(nameCol, QSqlRelation("mode", "id", "submode"));

     nameCol = rec.indexOf("id");
     setSort(nameCol, Qt::AscendingOrder);

     nameCol = rec.indexOf("qso_date");
     setHeaderData(nameCol, Qt::Horizontal, tr("Date"));

     nameCol = rec.indexOf("call");
     setHeaderData(nameCol, Qt::Horizontal,tr("Call"));

     nameCol = rec.indexOf("bandid");
     setHeaderData(nameCol, Qt::Horizontal, tr("Band"));

     nameCol = rec.indexOf("modeid");
     setHeaderData(nameCol, Qt::Horizontal, tr("Mode"));

     nameCol = rec.indexOf("rst_sent");
     setHeaderData(nameCol, Qt::Horizontal, tr("RSTtx"));

     nameCol = rec.indexOf("rst_rcvd");
     setHeaderData(nameCol, Qt::Horizontal, tr("RSTrx"));

     nameCol = rec.indexOf("comment");
     setHeaderData(nameCol, Qt::Horizontal, tr("Comment"));
 }


