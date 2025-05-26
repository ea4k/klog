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

const QMap<QString, LogModel::ValidationFunc> LogModel::s_validationRules = {
    { "my_dxcc", [](const QVariant &v) { bool ok; int dxcc = v.toInt(&ok); return (ok && (dxcc >= 0) && (dxcc <= 530)); } },
    { "age", [](const QVariant &v) { bool ok; int age = v.toInt(&ok); return ok && age > 0.0 && age < 120.0; } },
    { "ant_az", [](const QVariant &v) { bool ok; double az = v.toDouble(&ok); return ok && az >= 0.0 && az <= 360.0; } },
    // ... add more column validators here ...
};

LogModel::LogModel(DataProxy_SQLite *dp, QObject *parent):QSqlRelationalTableModel(parent)
{
     //qDebug() << Q_FUNC_INFO ;
    //logModel = new QSqlRelationalTableModel(this);
    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
     //qDebug() << Q_FUNC_INFO << "llamando a filterValidFields";
    columns.clear();
    columns.append(dataProxy->filterValidFields(util->getDefaultLogFields()));
    setTable("log");

    setEditStrategy(QSqlTableModel::OnFieldChange);
    //qDebug() << Q_FUNC_INFO << " - END";
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{ // Used to check if the data to be shown in the logview table must or not be shown
  // Depending on the data validation. Check: ValidationFunc above
    //qDebug() << Q_FUNC_INFO;
    if (role != Qt::DisplayRole)
        return QSqlRelationalTableModel::data(index, role);

    QString columnName = this->record().fieldName(index.column());

    auto it = s_validationRules.find(columnName);
    if (it != s_validationRules.end()) {
        QVariant value = QSqlRelationalTableModel::data(index, role);
        if (!it.value()(value)) {
            return QVariant(); // Hide invalid data by returning an empty QVariant
        }
    }

    return QSqlRelationalTableModel::data(index, role);
}

bool LogModel::createlogModel(const int _i)
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
    if (!setColumns(columns))
    {
        //qDebug() << Q_FUNC_INFO << " - ERROR on setColumns";
        return false;
    }

    if (!select())
    {
        //qDebug() << Q_FUNC_INFO << " - ERROR on select()";
        return false;
    }

  //qDebug() << Q_FUNC_INFO << " - END";
    return true;
}

bool LogModel::setColumns(const QStringList &_columns)
{
    //qDebug() << Q_FUNC_INFO ;
    QString auxt;
    foreach(auxt, _columns)
    {
        //qDebug() << Q_FUNC_INFO << ": " << auxt;
    }
    columns.clear();
     //qDebug() << Q_FUNC_INFO << " - calling filterValidFields";
    columns << dataProxy->filterValidFields(_columns);

     QSqlQuery q;
     QString stringQuery = QString("SELECT * from log LIMIT 1");
     QSqlRecord rec; // = q.record();

     int nameCol;

     if (!q.exec(stringQuery))
     {
        emit queryError(Q_FUNC_INFO, q.lastError().databaseText(), q.lastError().nativeErrorCode(), q.lastQuery());
        //qDebug() << Q_FUNC_INFO << " - END - 1";
        return false;
     }

     //if (!q.next())
     //{
     //    //qDebug() << Q_FUNC_INFO << " - END - 2";
     //    return false;
     //}
     rec = q.record(); // Number of columns

     //qDebug() <<Q_FUNC_INFO << ": - columns: " << QString::number(rec.count());

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

     //if (_columns.contains("qso_complete"))
     //{
     //    nameCol = rec.indexOf("qso_complete");
     //    setRelation(nameCol, QSqlRelation("qso_complete_enumeration", "id", "shortname"));
     //}

     nameCol = rec.indexOf("id");
     setSort(nameCol, Qt::AscendingOrder);
     QString aux;

     foreach(aux, columns)
     {
         nameCol = rec.indexOf(aux);
         if (!setHeaderData(nameCol, Qt::Horizontal, util->getLogColumnName(aux)))
         {
             //qDebug() << Q_FUNC_INFO << ": - ERROR when adding the following column to the log view model: " << aux;
             return false;
         }
         //qDebug() << Q_FUNC_INFO << ": - " << aux;
    }
    //qDebug() << Q_FUNC_INFO << " - END";
    return true;
 }

