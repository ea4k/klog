#include "logmodel.h"

LogModel::LogModel(QObject *parent):QSqlRelationalTableModel(parent)
{
    //qDebug() << "LogModel::LogModel "  << endl;
    //logModel = new QSqlRelationalTableModel(this);
    dataProxy = new DataProxy_SQLite();
    setTable("log");

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

   //qDebug() << "LogModel::createlogModel: log: " << QString::number(_i) << endl;

   QString contestMode = dataProxy->getLogTypeOfUserLog(_i);
   //qDebug() << "LogModel::createlogModel - contestMode: " << contestMode << endl;

   QString stringQuery = QString("lognumber='%1'").arg(_i);
   QSqlQuery query(stringQuery);
   setFilter(stringQuery);


   //if (contestMode.compare("DX"))
    if (contestMode == "DX")
   {
       //qDebug() << "LogModel::createlogModel: found type DX" << endl;
       setColumnsToDX();
   }
   else if (contestMode == "CQ-WW-SSB")
   {
       //qDebug() << "LogModel::createlogModel: found type CQ-WW-SSB" << endl;
   }
   else
   {
       // THIS POINT SHOULD NOT BE REACHED. It means that there is a kind of contest not supported.
       // Maybe the way should be to move ALL the actions from DX here.
       //qDebug() << "LogModel::createlogModel: log type NOT found" << endl;
   }


    select();


}

 void LogModel::setColumnsToDX()
 {
     //qDebug() << "LogModel::setColumnsToDX"  << endl;

     QSqlQuery q;
     QString stringQuery = QString("SELECT * from log LIMIT 1");
     QSqlRecord rec; // = q.record();

     int nameCol;

     bool sqlOK = q.exec(stringQuery);
     if (!sqlOK)
     {
         emit queryError(Q_FUNC_INFO, q.lastError().databaseText(), q.lastError().number());
     }
     q.next();
     rec = q.record(); // Number of columns

     //qDebug() << "LogModel::createlogModel - columns: " << QString::number(rec.count()) << endl;



     nameCol = rec.indexOf("bandid");
     setRelation(nameCol, QSqlRelation("band", "id", "name"));

     nameCol = rec.indexOf("modeid");
     //setRelation(nameCol, QSqlRelation("mode", "id", "name"));
     setRelation(nameCol, QSqlRelation("mode", "id", "submode"));

     nameCol = rec.indexOf("id");
     setSort(nameCol, Qt::AscendingOrder);

     nameCol = rec.indexOf("qso_date");
     setHeaderData(nameCol, Qt::Horizontal, tr("Date"));

     nameCol = rec.indexOf("time_on");
     setHeaderData(nameCol, Qt::Horizontal, tr("Time"));

     nameCol = rec.indexOf("call");
     setHeaderData(nameCol, Qt::Horizontal,tr("QRZ"));

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


