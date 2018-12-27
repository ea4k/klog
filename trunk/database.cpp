/***************************************************************************
                          database.cpp  -  description
                             -------------------
    begin                : sept 2011
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include "database.h"
//#include <qDebug>

DataBase::DataBase(const QString _parentClass, const QString _DBName)
{
     //qDebug() << "DataBase::DataBase: PLAIN: " << _parentClass << " / Name = " << _DBName << endl;
    constrid = 1;

    util = new Utilities();
    softVersion = util->getVersion();
    dbName = _DBName;

     //qDebug() << "DataBase::DataBase1: dbName: " << dbName << endl;

    //db = QSqlDatabase::database();

    dbVersion = DBVersionf;
    createConnection();
     //qDebug() << "DataBase::DataBase: PLAIN - connection Name: " << dbConnectionName << endl;
     //qDebug() << "DataBase::DataBase: PLAIN - DB Name: " << db.databaseName() << endl;
    insertPreparedQueries.clear();
    insertQueryFields.clear();
     //qDebug() << "DataBase::DataBase: PLAIN: - END" << endl;
}

DataBase::DataBase(const QString _parentClass, const QString _softVersion, const QString _DBName){
     //qDebug() << "DataBase::DataBase2: " << _parentClass << "/" << _softVersion << " / Name = " << _DBName << endl;
    //TODO: Sometimes the DB is created without the proper calling (without passing softVersion)
    constrid = 2;
    dbVersion = DBVersionf;
    softVersion = _softVersion;
    //inMemoryOnly = inmemoryonly;
    latestReaded = 0.0;
    util = new Utilities();
    util->setVersion(softVersion);

    //dbName = util->getKLogDBFile();
    dbName = _DBName;

     //qDebug() << "DataBase::DataBase2: dbName: " << dbName << endl;
    //dbDir = dbName;
     //qDebug() << "DataBase::DataBase: DB(string): " << dbName << endl;

    //db = QSqlDatabase::database();
    if (util->getVersionDouble()>0)
    {
        createConnection();
    }
     //qDebug() << "DataBase::DataBase: - connection Name: " << dbConnectionName << endl;
     //qDebug() << "DataBase::DataBase: - DB Name: " << db.databaseName() << endl;
    insertPreparedQueries.clear();
    insertQueryFields.clear();

     //qDebug() << "DataBase::DataBase2: END"  << endl;
}



/*
bool DataBase::queryAddField(const QString _field, const QString value)
{
    //QStringList insertPreparedQueries, insertQueryFields;
    insertQueryFields << _field << value;

}


bool DataBase::queryPrepare()
{
    //insertPreparedQueries.clear();
    //insertQueryFields.clear();
    for (int i = 0; i < insertQueryFields.size(); ++i)
    {
        if (insertQueryFields.at(i) != "EOR")
        {
            insertPreparedQueries << insertQueryFields.at(i) << insertQueryFields.at(i+1);
        }
        else
        {
            insertPreparedQueries << "EOR";
            return true;
        }
    }
    return true;

}


bool DataBase::queryExec()
{
     //qDebug()  << "DataBase::queryExec  "  << endl;
    bool sqlOK;
    //insertQueryFields.clear();
    //insertPreparedQueries.clear();
    //bool sqlOK = preparedQuery.exec();
    QSqlQuery preparedQuery;
    //Prepare the Query
    for (int i = 0; i < insertPreparedQueries.size(); ++i)
    {
        preparedQuery.bindValue(insertPreparedQueries.at(i), insertPreparedQueries.at(i+1));
    }



    if (!sqlOK)
    {
        queryErrorManagement("DataBase::queryExec", preparedQuery.lastError().databaseText(), preparedQuery.lastError().number(), preparedQuery.lastQuery());
        //emit queryError(Q_FUNC_INFO, preparedQuery.lastError().databaseText(), preparedQuery.lastError().number(), preparedQuery.lastQuery());
         //qDebug()  << "DataBase::queryExec - FAILED execution: "  << preparedQuery.lastQuery() << endl;
    }
    else
    {
         //qDebug()  << "DataBase::queryExec - executed: "  << preparedQuery.lastQuery() << endl;
    }
    return sqlOK;
}

*/


DataBase::~DataBase()
{
       //qDebug() << "DataBase::~DataBase"  << endl;
}

QString DataBase::getSoftVersion()
{
    QSqlQuery query;

    QString stringQuery ("SELECT MAX (softversion) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);


    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toString();
        }
        else
        {
            query.finish();
            return QString();
        }
    }
    else
    { //ERROR in Query execution
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
    return QString();

}

QString DataBase::getDBVersion()
{
    QSqlQuery query;

    QString stringQuery ("SELECT MAX (dbversion) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);


    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            return QString::number((query.value(0)).toDouble(), 'f', 3);
        }
        else
        {
             query.finish();
            return QString();
        }
    }
    else
    { //ERROR in Query execution
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return QString();
    }
    return QString();
}

/*
 bool DataBase::setDir(const QString _dir)
{
    dbDir = _dir;
    return true;
}
*/

QString DataBase::getDBName()
{
    return db.databaseName();
}

QStringList DataBase::getColumnNamesFromTable(const QString _tableName)
{

    //qDebug() << "DataBase::getColumnNamesFromTable: " << _tableName << endl;
   QSqlQuery query;

   QString queryString = QString("PRAGMA table_info('%1')").arg(_tableName);

   bool sqlOK = query.exec(queryString);
   QStringList list;
   list.clear();
    QString aux;
   if (sqlOK)
   {
        //qDebug() << "DataBase::getColumnNamesFromTable: OK" << endl;
       while(query.next())
       {
           if (query.isValid())
           {
                aux = (query.value(1)).toString();
               if (( aux.toUpper() != "ID" ) && (aux.length()>0))
               {
                   list << aux;
                    //qDebug() << "DataBase::getColumnNamesFromTable: " << (query.value(1)).toString() << endl;
               }
           }
       }
       query.finish();
    }
    else
    {
       queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.finish();
     //qDebug() << "DataBase::getColumnNamesFromTable: " << QString::number(list.size()) << endl;
    return list;

}

void DataBase::compress()
{
       //qDebug() << "DataBase::compress " << endl;

    //QSqlDatabase db = QSqlDatabase::database();
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
    }
   else
    {
        db.exec("VACUUM;");
    }

}

bool DataBase::reConnect(const QString _DBName)
{
     //qDebug() << "DataBase::reConnect:"  << endl;
    db.close();
    dbName = _DBName;
      //qDebug() << "DataBase::reConnect: DB closed"  << endl;
    //dbName = util->getKLogDBFile();
      //qDebug() << "DataBase::reConnect: DB: " << dbDir  << endl;
    return createConnection();
      //qDebug() << "DataBase::reConnect: END"  << endl;

}

bool DataBase::createConnection(bool newDB)
{
     //qDebug() << "DataBase::createConnection: " << QString::number(dbVersion) << "/" << softVersion << endl;
    QString stringQuery;
    QSqlQuery query;


    if (!db.isOpen())
    {
         //qDebug() << "DataBase::createConnection: DB NOT Opened" << endl;
        //db = QSqlDatabase::database();
        qDebug() << "DataBase::createConnection: Start the test"  << endl;
        if (db.isValid())
        {
            qDebug() << "DataBase::createConnection: DB is Valid"  << endl;

        }
        else
        {
            qDebug() << "DataBase::createConnection: DB is not valid, let's call addDataBase"  << endl;
            db = QSqlDatabase::addDatabase("QSQLITE");
            qDebug() << "DataBase::createConnection: Now we call setDatabaseName"  << endl;
            db.setDatabaseName(dbName);
            qDebug() << "DataBase::createConnection: end of not valid"  << endl;
        }
        qDebug() << "DataBase::createConnection: end of valid check, let's try if it is open"  << endl;
        if (!db.open())
        {
            qDebug() << "DataBase::createConnection:Not open "  << endl;
            QMessageBox::warning(0, QObject::tr("Database Error"), db.lastError().text());
             //qDebug() << "DataBase::createConnection: DB creation ERROR"  << endl;
            return false;
        }
        else
        {
             qDebug() << "DataBase::createConnection: created and opened after the creation" << endl;
            if (isTheDBCreated())
            {
                 //qDebug() << "DataBase::createConnection: DB Exists"  << endl;
            }
            else
            {
                 //qDebug() << "DataBase::createConnection: DB does not exist"  << endl;
                createDataBase();

                stringQuery ="PRAGMA main.page_size = 4096;";
                query.exec(stringQuery);
                stringQuery ="PRAGMA main.cache_size=10000;";
                query.exec(stringQuery);
                stringQuery ="PRAGMA main.locking_mode=EXCLUSIVE;";
                query.exec(stringQuery);
                stringQuery ="PRAGMA main.synchronous=NORMAL;";
                query.exec(stringQuery);
                stringQuery ="PRAGMA main.journal_mode=WAL;";
                query.exec(stringQuery);
                stringQuery ="PRAGMA main.cache_size=5000;";
                query.exec(stringQuery);
                stringQuery ="PRAGMA synchronous=OFF;";
                query.exec(stringQuery);
                stringQuery ="PRAGMA main.temp_store = MEMORY;";
                query.exec(stringQuery);
                //stringQuery="PRAGMA auto_vacuum = FULL;";
                //query.exec(stringQuery);
                stringQuery ="PRAGMA case_sensitive_like=OFF;";
                query.exec(stringQuery);
            }
        }
    }
    else
    {
         //qDebug() << "DataBase::createConnection: No Error, DB is open" << endl;
    }


       //qDebug() << "DataBase::createConnection: Going to run - createBandModeMaps " << endl;

    if (createBandModeMaps())
    {
           //qDebug() << "DataBase::createConnection: createBandModeMaps true" << endl;
    }
    else
    {
           //qDebug() << "DataBase::createConnection: createBandModeMaps false SSSSSSSSSSSSSSSTOOOOOOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPPPPPP" << endl;
    }

    //created = true;
    qDebug() << "DataBase::createConnection -------------------------------------------- END" << endl;
    return unMarkAllQSO();
}

bool DataBase::isTheDBCreated()
{
     //qDebug() << "DataBase::isTheDBCreated: Called from: " << QString::number(constrid)  << endl;

    QSqlQuery query;
    int _num = 0;

    QString stringQuery ("SELECT count(id) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);


    if (sqlOK)
    {
         //qDebug() << "DataBase::isTheDBCreated - SQL OK"  << endl;
        query.next();
        if (query.isValid())
        {

              //qDebug() << "DataBase::isTheDBCreated - valid"  << endl;
            _num = (query.value(0)).toInt();
            if (_num > 0)
            {
                  //qDebug() << "DataBase::isTheDBCreated - DB Exists"  << endl;
                  //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END TRUE" << endl;
                 query.finish();
                return true;
            }
            else
            {
                  //qDebug() << "DataBase::isTheDBCreated - DB does not Exist"  << endl;
                  //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END FALSE-1" << endl;
                 query.finish();
                return false;
            }
        }
        else
        {

              //qDebug() << "DataBase::isTheDBCreated - not valid"  << endl;
              //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END FALSE-2" << endl;
             query.finish();
            return false;
        }
    }
    else
    { //ERROR in Query execution
         //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------ ERROR IN QUERY EXECUTION" << endl;
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    query.finish();
     //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END FALSE-X" << endl;
    return false;

}

bool DataBase::recreateTableLog()
{
     //qDebug() << "DataBase::recreateTableLog" << endl;

    if (!createTableLog(false))         // Create modetemp
    {
         //qDebug() << "DataBase::recreateTableLog: CreateTableLog returned false" << endl;
        return false;
    }

    QString queryString;
    queryString.clear();
    QStringList columns;
    columns.clear();
    columns << getColumnNamesFromTable("log");

    queryString =  columns.first();

    for (int i=1;i<columns.size()-1;i++)
    {
        queryString = queryString + ", " + columns.at(i);
    }

    queryString = "INSERT INTO logtemp (" + queryString + ", " + columns.last() + ") SELECT " + queryString + ", " + columns.last() + " FROM log";


    if (execQuery(Q_FUNC_INFO, queryString))
    {
        if (execQuery(Q_FUNC_INFO, "DROP table log"))
        {
            if (execQuery(Q_FUNC_INFO, "ALTER TABLE logtemp RENAME TO log"))
            {
                return true;
            }
            else
            {
                 //qDebug() << "recreateTableLog ERROR - logTemp not renamed" << endl;
                return false;
            }
        }
        else
        {
             //qDebug() << "recreateTableLog ERROR - log table not dropped" << endl;
        }
    }
    else
    {
         //qDebug() << "recreateTableLog ERROR - Data not moved" << endl;
        return false;
    }
     //qDebug() << "recreateTableLog END" << endl;
    return true;
}

bool DataBase::createTableLog(bool temp)
{ //Creates a temporal table or the normal one.

    //QString logToCreate = "log";

    QSqlQuery query;
    QString stringQuery = QString();
    if (temp)
    {
        stringQuery = "CREATE TABLE log" ;
          //qDebug() << "DataBase::createTableLog: log"  << endl;
    }
    else
    {
        stringQuery = "CREATE TABLE logtemp" ;
          //qDebug() << "DataBase::createTableLog: logtemp"  << endl;
    }

             stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "qso_date VARCHAR(10) NOT NULL, "
             "time_on VARCHAR(8) NOT NULL, "
             "call VARCHAR(40) NOT NULL, "
             "rst_sent VARCHAR, "
             "rst_rcvd VARCHAR, "
             "bandid INTEGER NOT NULL, "
             "modeid INTEGER NOT NULL, "
             "srx VARCHAR(10), "
             "stx VARCHAR(10), "
             "points INTEGER,"
             "multiplier INTEGER,"
             "cqz INTEGER, "
             "ituz INTEGER, "
             "dxcc INTEGER, "
             "address VARCHAR, "
             "age INTEGER, "
             "cnty VARCHAR, "
             "comment VARCHAR, "
             "a_index INTEGER, "
             "ant_az INTEGER, "
             "ant_el INTEGER, "
             "ant_path INTEGER, "
             "arrl_sect INTEGER, "
             "award_submitted VARCHAR, "
             "award_granted VARCHAR, "
             "band_rx INTEGER, "
             "checkcontest VARCHAR, "
             "class VARCHAR, "
             "cont VARCHAR(2), "
             "contacted_op VARCHAR(40), "
             "contest_id VARCHAR, "
             "country VARCHAR, "
             "credit_submitted VARCHAR, "
             "credit_granted VARCHAR, "
             "darc_dok VARCHAR,"
             "distance INTEGER, "
             "email VARCHAR, "
             "eq_call VARCHAR, "
             "eqsl_qslrdate VARCHAR(10), "
             "eqsl_qslsdate VARCHAR(10), "
             "eqsl_qsl_rcvd VARCHAR(1), "
             "eqsl_qsl_sent VARCHAR(1), "
             "fists INTEGER, "
             "fists_cc INTEGER, "
             "force_init INTEGER, "
             "freq VARCHAR, "
             "freq_rx VARCHAR, "
             "gridsquare VARCHAR, "
             "guest_op VARCHAR,"
             "hrdlog_qso_upload_date VARCHAR(10), "
             "hrdlog_qso_upload_status  VARCHAR(1), "
             "iota VARCHAR(6), "
             "iota_island_id VARCHAR, "
             "k_index INTEGER, "
             "lat VARCHAR(11), "
             "lon VARCHAR(11), "
             "lotw_qslrdate VARCHAR(10), "
             "lotw_qslsdate VARCHAR(10), "
             "lotw_qsl_rcvd VARCHAR(1), "
             "lotw_qsl_sent VARCHAR(1), "
             "clublog_qso_upload_date VARCHAR(10), "
             "clublog_qso_upload_status VARCHAR(1), "
             "max_bursts INTEGER, "
             "ms_shower VARCHAR, "
             "my_antenna VARCHAR,"
             "my_city VARCHAR, "
             "my_cnty VARCHAR, "
             "my_country INTEGER, "
             "my_cq_zone INTEGER, "
             "my_dxcc INTEGER, "
             "my_fists INTEGER, "
             "my_gridsquare VARCHAR, "
             "my_iota VARCHAR(6), "
             "my_iota_island_id VARCHAR, "
             "my_itu_zone INTEGER ,"
             "my_lat VARCHAR(11), "
             "my_lon VARCHAR(11), "
             "my_name VARCHAR, "
             "my_postal_code VARCHAR ,"
             "my_rig VARCHAR, "
             "my_sig VARCHAR, "
             "my_sig_info VARCHAR, "
             "my_sota_ref VARCHAR, "
             "my_state VARCHAR, "
             "my_street VARCHAR, "
             "my_usaca_counties VARCHAR, "
             "my_vucc_grids VARCHAR, "
             "name VARCHAR, "
             "notes VARCHAR, "
             "nr_bursts INTEGER, "
             "nr_pings INTEGER, "
             "operator VARCHAR, "
             "owner_callsign VARCHAR, "
             "pfx VARCHAR, "
             "precedence VARCHAR, "
             "prop_mode VARCHAR, "
             "public_key VARCHAR, "
             "qrzcom_qso_upload_date VARCHAR(10), "
             "qrzcom_qso_upload_status VARCHAR(1), "
             "qslmsg VARCHAR, "
             "qslrdate VARCHAR(10), "
             "qslsdate VARCHAR(10), "
             "qsl_rcvd VARCHAR(1), "
             "qsl_sent VARCHAR(1), "
             "qsl_rcvd_via VARCHAR(1), "
             "qsl_sent_via VARCHAR(1), "
             "qsl_via VARCHAR, "
             "qso_complete VARCHAR(1), "
             "qso_random INTEGER, "
             "qth VARCHAR, "
             "region VARCHAR, "
             "rig VARCHAR, "
             "rx_pwr REAL, "
             "sat_mode VARCHAR, "
             "sat_name VARCHAR, "
             "sfi INTEGER, "
             "sig VARCHAR, "
             "sig_info VARCHAR, "
             "silent_key VARCHAR(1), "
             "skcc VARCHAR, "
             "sota_ref VARCHAR, "
             "srx_string VARCHAR, "
             "stx_string VARCHAR, "
             "state VARCHAR, "
             "station_callsign VARCHAR, "
             "submode VARCHAR,"
             "swl INTEGER, "
             "uksmg INTEGER, "
             "usaca_counties VARCHAR, "
             "ve_prov VARCHAR, "
             "vucc_grids VARCHAR, "
             "ten_ten INTEGER, "
             "tx_pwr REAL, "
             "web VARCHAR, "
             "qso_date_off VARCHAR(10), "
             "time_off VARCHAR(8), "
             "transmiterid VARCHAR, "
             "marked VARCHAR(1), "
             "lognumber INTEGER NOT NULL, "

             "UNIQUE (call, qso_date, time_on, bandid, modeid, lognumber), "
             "FOREIGN KEY (qso_complete) REFERENCES qso_complete_enumeration, "
             "FOREIGN KEY (qsl_rcvd_via) REFERENCES qsl_via_enumeration, "
             "FOREIGN KEY (qsl_sent_via) REFERENCES qsl_via_enumeration, "
             "FOREIGN KEY (qsl_rcvd) REFERENCES qsl_rec_status, "
             "FOREIGN KEY (qsl_sent) REFERENCES qsl_sent_status, "
             "FOREIGN KEY (prop_mode) REFERENCES prop_mode_enumeration, "
             "FOREIGN KEY (my_country) REFERENCES entity, "
             "FOREIGN KEY (lotw_qsl_rcvd) REFERENCES qsl_rec_status, "
             "FOREIGN KEY (lotw_qsl_sent) REFERENCES qsl_sent_status, "
             "FOREIGN KEY (eqsl_qsl_rcvd) REFERENCES qsl_rec_status, "
             "FOREIGN KEY (eqsl_qsl_sent) REFERENCES qsl_sent_status, "
             "FOREIGN KEY (credit_submitted) REFERENCES award_enumeration, "
             "FOREIGN KEY (credit_granted) REFERENCES award_enumeration, "
             "FOREIGN KEY (country) REFERENCES entity, "
             "FOREIGN KEY (ant_path) REFERENCES ant_path_enumeration, "
             "FOREIGN KEY (arrl_sect) REFERENCES arrl_sect_enumeration, "
             "FOREIGN KEY (band_rx) REFERENCES band, "
             "FOREIGN KEY (modeid) REFERENCES mode, "
             "FOREIGN KEY (submode) REFERENCES mode, "
             "FOREIGN KEY (dxcc) REFERENCES entity, "
             "FOREIGN KEY (bandid) REFERENCES band)");

       //qDebug() << "DataBase::createTableLog: " << stringQuery  << endl;

    if (execQuery(Q_FUNC_INFO, stringQuery))
    {
        return true;
    }
    else
    {
        return false;
    }
/*
    bool sqlOK = query.exec(stringQuery);

    while (query.isActive())
    {query.finish();}
    if (sqlOK)
    {
        return true;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;
    }
*/
}

bool DataBase::createDataBase()
{
       //qDebug() << "DataBase::createDataBase ------------------------------------- START" << QString::number(constrid)  << endl;
    //bool qres;
       //http://www.sqlite.org/
    //http://www.sqlite.org/datatype3.html
       //qDebug() << "DataBase::createData"  << endl;
    //int softDB = dbVersion;
    //QString softV = _softVersion;
    //QString dateString;

    QSqlQuery query;
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists log");
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists band");
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists mode");
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists prefixesofentity");
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists continent");
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists entity");

    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists softwarecontrol");

    QString stringQuery = QString ("CREATE TABLE softwarecontrol ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "dateupgrade VARCHAR(10) NOT NULL, "
                "softversion REAL NOT NULL, "
                "dbversion REAL NOT NULL)");

    execQuery(Q_FUNC_INFO, stringQuery);
    //dateString = (date.currentDateTime()).toString("yyyyMMdd");

    updateDBVersion(softVersion, QString::number(DBVersionf));

    createTableBand(true);
    populateTableBand(true);

    createTableMode(true);
    populateTableMode(true);

    createTableSatellites(true);
    populateTableSatellites(true);

    createTableLog(true);

    createTableEntity(true);


      //DATE YYYY-MM-DD
      //TIME HHmmss
      //http://www.sqlite.org/lang_datefunc.html
      /*
       "confirmed INTEGER NOT NULL, "
      confirmed means:
      confirmed = 0     Set as Worked
      confirmed = 1     Set as Confirmed
      */

      stringQuery = QString("CREATE TABLE continent ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(2) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");
    execQuery(Q_FUNC_INFO, stringQuery);
      stringQuery = QString("CREATE TABLE ant_path_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");
    execQuery(Q_FUNC_INFO, stringQuery);
      stringQuery = QString("CREATE TABLE arrl_sect_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(2) NOT NULL, "
                 "name VARCHAR(30) NOT NULL)");

    execQuery(Q_FUNC_INFO, stringQuery);
      stringQuery = QString("CREATE TABLE qso_complete_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(3) NOT NULL, "
                 "name VARCHAR(10) NOT NULL)");
    execQuery(Q_FUNC_INFO, stringQuery);

      createTableContest();

      stringQuery = QString("CREATE TABLE contestcategory ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(20) NOT NULL, "
                 "name VARCHAR(40) NOT NULL)");
    execQuery(Q_FUNC_INFO, stringQuery);

      stringQuery = QString("CREATE TABLE award_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "name VARCHAR(15) NOT NULL)");

    execQuery(Q_FUNC_INFO, stringQuery);
      stringQuery = QString("CREATE TABLE prefixesofentity ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "prefix VARCHAR(15) NOT NULL,"
                 "dxcc INTEGER NOT NULL,"
                 "cqz INTEGER NOT NULL,"
                 "ituz INTEGER NOT NULL,"
                 "UNIQUE (prefix, dxcc), "
                 "FOREIGN KEY (dxcc) REFERENCES entity)");
    execQuery(Q_FUNC_INFO, stringQuery);

    createTableAwardDXCC();
    createTableAwardWAZ();

      stringQuery = QString("CREATE TABLE qsl_rec_status ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");
      execQuery(Q_FUNC_INFO, stringQuery);

      stringQuery = QString("CREATE TABLE qsl_sent_status ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");
      execQuery(Q_FUNC_INFO, stringQuery);

    createTableQSL_Via_enumeration();
    populateTableQSL_Via_enumeration();


      createTablePropModes();
      createTableLogs(true);
      createTableClubLogStatus();
      populateTableClubLogStatus();

    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('Y', 'Yes')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('N', 'No')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('R', 'Requested')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('Q', 'Queued')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('I', 'Ignore/Invalid')");


    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('Y', 'Yes')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('N', 'No')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('R', 'Requested')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('I', 'Ignore/Invalid')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('V', 'Validated')");


      execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('AF', 'Africa')");
      execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('AS', 'Asia')");
      execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('EU', 'Europe')");
      execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('NA', 'North America')");
      execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('OC', 'Oceania')");
      execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('SA', 'South America')");
      execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('AN', 'Antartica')");

      populateContestData();
      populatePropagationModes();


    execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('G', 'GrayLine')");
    execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('O', 'Other')");
    execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('S', 'ShortPath')");
    execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('L', 'LongPath')");

    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AL', 'Alabama')");
/*
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AK', 'Alaska')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AB', 'Alberta')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AR', 'Arkansas')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AZ', 'Arizona')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('BC', 'British Columbia')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('CO', 'Colorado')");
*/

    //TODO: Awards are deprecated
    execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('AJA')");
    execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('CQDX')");
    execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('CQDXFIELD')");
    execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('DXCC')");

    execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('Y', 'Yes')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('N', 'No')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('NIL', 'Not heard')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('?', 'Uncertain')");

       //qDebug() << "DataBase::createDataBase ------------------------------------- END"  << endl;
    return true;

}

bool DataBase::recreateTableDXCC()
{
    QSqlQuery query;

    if (execQuery(Q_FUNC_INFO, "DROP TABLE awarddxcc"))
    {
        return createTableAwardDXCC();
    }
    return true;
}

bool DataBase::createTableAwardDXCC()
{
      return execQuery(Q_FUNC_INFO, "CREATE TABLE awarddxcc ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "dxcc INTEGER NOT NULL,"
                             "band INTEGER NOT NULL, "
                             "mode INTEGER NOT NULL, "
                             "confirmed INTEGER, "
                             "qsoid INTEGER NOT NULL, "
                             "lognumber INTEGER, "
                             "UNIQUE (dxcc, band, mode, lognumber), "
                             "FOREIGN KEY (dxcc) REFERENCES entity, "
                             "FOREIGN KEY (band) REFERENCES band, "
                             "FOREIGN KEY (mode) REFERENCES mode, "
                             "FOREIGN KEY (qsoid) REFERENCES log)");

    /*
    In awarddxcc confirmed means:
    confirmed = 0     Set as Worked
    confirmed = 1     Set as Confirmed
    */
}

bool DataBase::recreateTableWAZ()
{
    QSqlQuery query;

    if (execQuery(Q_FUNC_INFO, "DROP TABLE awardwaz"))
    {
        return createTableAwardWAZ();
    }
    return true;
}

bool DataBase::createTableAwardWAZ()
{

    return execQuery(Q_FUNC_INFO, "CREATE TABLE awardwaz ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "cqz INTEGER NOT NULL,"
                           "band INTEGER NOT NULL, "
                           "mode INTEGER NOT NULL, "
                           "confirmed INTEGER, "
                           "qsoid INTEGER NOT NULL, "
                           "lognumber INTEGER, "
                           "UNIQUE (cqz, band, mode, lognumber), "
                           "FOREIGN KEY (band) REFERENCES band, "
                           "FOREIGN KEY (mode) REFERENCES mode, "
                           "FOREIGN KEY (qsoid) REFERENCES log)");
    /*
    In awardwaz confirmed means:
    confirmed = 0     Set as Worked
    confirmed = 1     Set as Confirmed
    */
}





int DataBase::getBandIdFromName(const QString b)
{
     //qDebug() << "DataBase::getBandIdFromName: " << b << endl;
    QSqlQuery query;
    if (isValidBand(b))
    {
        QString queryString = QString("SELECT id FROM band WHERE name='%1'").arg(b);

        bool sqlOK = query.exec(queryString);

        if (sqlOK)
        {
            query.next();
            if ( query.isValid() )
            {
                   //qDebug() << "DataBase::getBandIdFromName: OK" << QString::number((query.value(0)).toInt()) << endl;
                int v = (query.value(0)).toInt();
                query.finish();
                return v;

            }
            else
            {
                   //qDebug() << "DataBase::getBandIdFromName: NOK 1" << endl;
                query.finish();
                return -1;
            }
        }
        else
        {
            queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            query.finish();
            return -1;
        }
           //qDebug() << "DataBase::getBandIdFromName: NOK 3" << endl;
    }
    else
    {
         //qDebug() << "DataBase::getBandIdFromName: BAND NOT VALID: " << b << endl;
    }
    query.finish();
    return -1;
}

int DataBase::getModeIdFromName(const QString b)
{
     //qDebug() << "DataBase::getModeIdFromName: " << b << endl;
    QSqlQuery query;
    if (isValidMode(b, false))
    {
        QString queryString = QString("SELECT id FROM mode WHERE name='%1'").arg(b);
          //qDebug() << "DataBase::getModeIdFromName: queryString: " << queryString << endl;
        bool sqlOK = query.exec(queryString);

        if (sqlOK)
        {
            query.next();
            if ( query.isValid() )
            {
                  //qDebug() << "DataBase::getModeIdFromName: OK" << QString::number((query.value(0)).toInt()) << endl;
                return (query.value(0)).toInt();

            }
            else
            {
                  //qDebug() << "DataBase::getModeIdFromName: NOK 1" << endl;
                query.finish();
                return -1;
            }
        }
        else
        {
            queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            query.finish();
        }
    }


      //qDebug() << "DataBase::getModeIdFromName: NOK 3" << endl;
    query.finish();
    return -1;
}


int DataBase::getModeIdFromSubMode(const QString b)
{
      //qDebug() << "DataBase::getModeIdFromSubMode: " << b << endl;
     QSqlQuery query;
     QString queryString = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(b);
/*
     if (_tmp)
     {
         queryString = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(b);
     }
     else
     {
         queryString = QString("SELECT id FROM mode WHERE submode='%1'").arg(b);
     }
*/
     bool sqlOK = query.exec(queryString);


     if (sqlOK)
     {
         query.next();
         if (query.isValid())
         {
                //qDebug() << "DataBase::getModeIdFromName: OK - Mode: " << b << " - " << (query.value(0)).toString() << endl;
             return (query.value(0)).toInt();
         }
         else
         {
                //qDebug() << "DataBase::getModeIdFromName: NOK 1" << "-------- END"<< endl;
             query.finish();
             return -1;
         }
     }
     else
     {
            //qDebug() << "DataBase::getModeIdFromName: NOK 2" << "-------- END"<< endl;
         queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
         return -1;
     }


       //qDebug() << "DataBase::getModeIdFromName: NOK 3" << "-------- END"<< endl;
     query.finish();
     return -1;
}

QString DataBase::getBandNameFromNumber(const int _n)
{
     //qDebug() << "DataBase::getBandNameFromNumber: " << QString::number(_n) << endl;
    QSqlQuery query;
    QString queryString = QString("SELECT name FROM band WHERE id='%1'").arg(_n);

    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if ( query.isValid() )
        {
            if ( isValidBand((query.value(0)).toString())  )
            {
                  //qDebug() << "DataBase::getBandNameFromNumber: " << (query.value(0)).toString() << "-------- END" << endl;
                return (query.value(0)).toString();
            }
            else
            {
                 //qDebug() << "DataBase::getBandNameFromNumber: " << "-------- END-1" << endl;
                query.finish();
                return QString();
            }
        }
        else
        {
              //qDebug() << "DataBase::getBandNameFromNumber: " << "-------- END-2" << endl;
            query.finish();
            return QString();
        }
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    query.finish();
    return QString();

}



QString DataBase::getModeNameFromNumber(const int _n, bool _tmp)
{
    //TODO May fail to identify the sumbode(mode/modetemp... (Review STEP-2 o 3)
      //qDebug() << "DataBase::getModeNameFromNumber: " << QString::number(_n) << endl;
    QSqlQuery query;
    QString queryString;
    if (_tmp)
    {
        queryString = QString("SELECT name FROM modetemp WHERE id='%1'").arg(_n);
    }
    else
    {
        queryString = QString("SELECT name FROM mode WHERE id='%1'").arg(_n);
    }

    bool sqlOK = query.exec(queryString);


    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
    }
    query.next();
       //qDebug() << "DataBase::getModeNameFromNumber: " << QString::number(_n) <<" - " <<  isValidMode((query.value(0)).toString(), _tmp) << endl;
    if ( query.isValid() )
    {
          //qDebug() << "DataBase::getModeNameFromNumber: ------ END-1" << endl;
        return (query.value(0)).toString();
        /* In a version when I change the mode table to include submode, this comparison may need to be checked in both versions
         * at once, failing the query as old version was not having the column submode
         *
        if ( isValidMode((query.value(0)).toString(), _tmp))
        {
               //qDebug() << "DataBase::getModeNameFromNumber - Found: " << (query.value(0)).toString() << endl;
            return (query.value(0)).toString();
        }
        else
        {
               //qDebug() << "DataBase::getModeNameFromNumber - Not Valid Mode: " << (query.value(0)).toString()  << endl;
            return QString();
        }
        */
    }
    else
    {
           //qDebug() << "DataBase::getModeNameFromNumber - Not Valid record"  << endl;
          //qDebug() << "DataBase::getModeNameFromNumber: ------ END-2" << endl;
        query.finish();
        return QString();
    }
}

QString DataBase::getSubModeNameFromNumber(const int _n, bool _tmp)
{
     //qDebug() << "DataBase::getSubModeNameFromNumber: " << QString::number(_n) << endl;
    QSqlQuery query;
    QString queryString;
    if (_tmp)
    {
        queryString = QString("SELECT submode FROM modetemp WHERE id='%1'").arg(_n);
    }
    else
    {
        queryString = QString("SELECT submode FROM mode WHERE id='%1'").arg(_n);
    }


    bool sqlOk = query.exec(queryString);

       //qDebug() << "DataBase::getSubModeNameFromNumber - query: " << query.lastQuery() << endl;

    if (sqlOk)
    {
        if (query.next())
        {
            if ( query.isValid() )
            {
                if ( isValidMode((query.value(0)).toString(), _tmp)  )
                {
                      //qDebug() << "DataBase::getSubModeNameFromNumber: RETURN: " << (query.value(0)).toString() << endl;
                    return (query.value(0)).toString();
                }
                else
                {
                      //qDebug() << "DataBase::getSubModeNameFromNumber: NO valid mode - END" << endl;
                    query.finish();
                    return QString();
                }
            }
            else
            {
                  //qDebug() << "DataBase::getSubModeNameFromNumber: query not valid - END" << endl;
                query.finish();
                return QString();
            }

        }
        else
        {
              //qDebug() << "DataBase::getSubModeNameFromNumber: query not next - END" << endl;
            query.finish();
            return QString();
        }


    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataBase::getSubModeNameFromNumber: SQL FALSE - END" << endl;
        query.finish();
        return QString();
    }
      //qDebug() << "DataBase::getSubModeNameFromNumber: - END-X" << endl;
    query.finish();
    return QString();
}

bool DataBase::isValidBand (const QString b)
{
      //qDebug() << "DataBase::isValidBand: " << b << endl;
    if (b.length()<1)
    {
        return false;
    }
    QSqlQuery query;
    QString stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(b);
    bool sqlOK = query.exec(stringQuery);
    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            query.finish();
            return true;
        }
        else
        {
            query.finish();
            return false;
        }

    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
    }
    return false;
}

bool DataBase::isValidMode (const QString b, const bool _tmp)
{
      //qDebug() << "DataBase::isValidMode: " << b << endl;
    QString stringQuery;
    if (b.length()<2)
    {
           //qDebug() << "DataBase::isValidMode: (length<2) FALSE"  << endl;
        return false;
    }

    if (_tmp)
    {
        stringQuery = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(b);
    }
    else
    {
        stringQuery = QString("SELECT id FROM mode WHERE submode='%1'").arg(b);
    }

    //stringQuery = QString("SELECT id FROM mode WHERE submode='%1'").arg(b);
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);

    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
    }
    query.next();
    return query.isValid();
}

bool DataBase::isValidBandNumber (const int b)
{
      //qDebug() << "DataBase::isValidBandNumber: " << QString::number(b)<< endl;
    return isValidBand(getBandNameFromNumber(b));

}

bool DataBase::isValidModeNumber (const int b)
{
      //qDebug() << "DataBase::isValidModeNumber: " << QString::number(b)<< endl;
    return isValidMode(getModeNameFromNumber(b, false), false);
}

int DataBase::getBandIdFromFreq(const QString fr)
{
      //qDebug() << "DataBase::getBandIdFromFreq: " << fr << endl;
    //Freq should be in MHz

    QString queryString = QString("SELECT id FROM band WHERE lower <= '%1' and upper >= '%2'").arg(fr).arg(fr);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

      //qDebug() << "DataBase::getBandIdFromFreq: Query: " << query.lastQuery() << endl;
    if (sqlOK)
    {
         //qDebug() << "DataBase::getBandIdFromFreq: Query OK" << endl;
        query.next();


        if (query.isValid())
        {
              //qDebug() << "DataBase::getBandIdFromFreq: Query OK - END" << endl;
            return (query.value(0)).toInt();
        }
        else
        {
              //qDebug() << "DataBase::getBandIdFromFreq: Valid NOK - END" << endl;
            query.finish();
            return -1;
        }
    }
    else
    {
         //qDebug() << "DataBase::getBandIdFromFreq: Query NOK" << endl;
         //qDebug() << "DataBase::getBandIdFromFreq: Query NOK: " << query.lastError().text() << endl;
         //qDebug() << "DataBase::getBandIdFromFreq: Query NOK: " << query.lastError().nativeErrorCode() << endl;
        if (query.lastError().isValid())
        {
             //qDebug() << "DataBase::getBandIdFromFreq: Query NOK - Error VALID" << endl;
        }
        else
        {
             //qDebug() << "DataBase::getBandIdFromFreq: Query NOK - Error NOT-VALID" << endl;
        }


        //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        queryErrorManagement(Q_FUNC_INFO, query.lastError().text(), query.lastError().number(), query.lastQuery());
        query.finish();
       return -2;
    }
      //qDebug() << "DataBase::getBandIdFromFreq: END-X" << endl;
    query.finish();
    return -3;
}


bool DataBase::isThisFreqInBand(const QString b, const QString fr)
{//Freq should be in MHz
     //qDebug() << "DataBase::isThisFreqInBand: " << b << "/" << fr << endl;
    int bandNf = getBandIdFromFreq(fr);
    int bandN = getBandIDFromName2(b);
     //qDebug() << "DataBase::isThisFreqInBand: (b/f)" << QString::number(bandN) << "/" << QString::number(bandNf) << endl;
    if (bandNf == bandN)
    {
         //qDebug() << "DataBase::isThisFreqInBand: OK " << b << "/" << fr << endl;
        return true;
    }
    else
    {
         //qDebug() << "DataBase::isThisFreqInBand: NOK " << b << "/" << fr << endl;
        return false;
    }
      //qDebug() << "DataBase::isThisFreqInBand: END" << endl;
    return false;
}


bool DataBase::unMarkAllQSO()
{
     //qDebug() << "DataBase::unMarkAllQSO" << endl;
    QString stringQuery = QString("UPDATE log SET marked = 'N' WHERE 1");
    return execQuery(Q_FUNC_INFO, stringQuery);
}


bool DataBase::updateIfNeeded()
{
       //qDebug() << "DataBase::updateIfNeeded - Version: " << QString::number(dbVersion) << endl;

    /**************************************************************************************
     * This function should call to bool updateToXXX () being XXX dbVersion and
     *
     */

    //float aux = 0.0;

    //int nameCol = -1;
    //int errorCode = -1;
    //bool toBeUpdated = false;
    //bool sqlOK;

    QSqlQuery query;
    QString stringQuery = QString("SELECT MAX (dbversion) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);

    if (sqlOK)
    {
        query.next();
        latestReaded = (query.value(0)).toFloat();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }

    //QSqlQuery query("SELECT dbversion FROM softwarecontrol");
    //QSqlRecord rec = query.record();
    //query.next();
    //latestReaded = (query.value(0)).toFloat();
    query.finish();
       //qDebug() << "DataBase::updateIfNeeded - LatestReaded: " << QString::number(latestReaded) << endl;



    if (latestReaded >= dbVersion)
    { // DB is updated, no update is needed
          //qDebug() << "DataBase::updateIfNeeded - DB updated (no need to update anything!) " << endl;
        //toBeUpdated = false;
          //qDebug() << "DataBase::updateIfNeeded - TRUE - END "  << endl;
        return true;
    }
    else
    { // DB is outdated. We need to update!!
          //qDebug() << "DataBase::updateIfNeeded - DB outdated... upgrade starts now! " << endl;
        QMessageBox msgBox;
        msgBox.setWindowTitle("KLog");
        msgBox.setText( QObject::tr("KLog DB needs to be upgraded."));
        msgBox.setInformativeText( QObject::tr("Do you want to upgrade it now?") + "\n"+ QObject::tr("If DB is not upgraded KLog may not work properly."));
        msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Apply);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.raise();
        //this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Popup);

        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Apply:
            // Save was clicked
                updateToLatest();
            break;
            case QMessageBox::Discard:
            // Discard was clicked
            break;
            default:
            // should never be reached
             //qDebug() << "DataBase::updateIfNeeded - FALSE - CHECK IF SEEN, shoud not be here! - END "  << endl;
                return false;
            break;
        }
    }

    // If the DB needs to be updated... we update it! :-)

   //qDebug() << "DataBase::updateIfNeeded - END!" << endl;
    return true;
}

bool DataBase::createTheBandQuickReference()
{
/*
          KEY      Value
    QHash<QString, int> bandIDHash;
    QHash<QString, int> modeIDHash;
    QHash<int, QString> IDBandHash;
    QHash<int, QString> IDModeHash
    QHash<int, QString> freqBandIdHash;

*/

      //qDebug() << "DataBase::createTheBandQuickReference: " << endl;

    QString st = "NULL";
    int in = 0;

    QString stringQuery = QString("SELECT id, name, lower FROM band");
    QString fr = QString();
    bandIDHash.clear();
    IDBandHash.clear();
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);

    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }

    while (query.next())
    {

        if (query.isValid())
        {
            st = (query.value(1)).toString();
            in = (query.value(0)).toInt();
            fr = (query.value(2)).toString();
            bandIDHash.insert(st, in );
            IDBandHash.insert(in, st);
            freqBandIdHash.insert(in, fr);

               //qDebug() << "DataBase::createTheBandQuickReference: " << st <<"/" << QString::number(in)<< endl;
        }
        else
        {
            //qDebug() << "DataBase::createTheBandQuickReference: Query not valid -'RETURN FALSE - END" << endl;

          // QMessageBox::warning(0, QObject::tr("Database Error (DataBase::createTheBandQuickReference)"),
          //                      query.lastError().text());
            query.finish();
           return false;
           //TODO: Manage this error, in case the query is NOK.

        }
            //qDebug() << "DataBase::createTheBandQuickReference: Go for the next one!" << endl;
    }
    query.finish();
      //qDebug() << "DataBase::createTheBandQuickReference: END" << endl;
    return true;
}


bool DataBase::createTheModeQuickReference()
{

    /*
              KEY      Value
        QHash<QString, int> modeIDHash;
        QHash<int, QString> IDModeHash

    */
        //qDebug() << "DataBase::createTheModeQuickReference: " << endl;

    if (getDBVersion().toFloat()<0.010)
    {
        // If the version is not updated we don't create the reference
        return true;
    }

        QString st = QString();
        QString sm = QString();
        int in = 0;
        modeIDHash.clear();
        IDModeHash.clear();
        subModeIDHash.clear();
        IDSubModeHash.clear();
        QString stringQuery = QString("SELECT id, name, submode FROM mode");
        QSqlQuery query;

        bool sqlOK = query.exec(stringQuery);

        if (!sqlOK)
        {
            queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            query.finish();
            return false;
        }
        while (query.next())
        {

            if (query.isValid())
            {
                in = (query.value(0)).toInt();
                st = (query.value(1)).toString();
                sm = (query.value(2)).toString();

                modeIDHash.insert(st, in );
                IDModeHash.insert(in, st);
                subModeIDHash.insert(sm, in );
                IDSubModeHash.insert(in, sm);
                   //qDebug() << "DataBase::createTheModeQuickReference: " << st <<"/" << QString::number(in)<< endl;
            }
            else
            {
                 //qDebug() << "DataBase::createTheModeQuickReference: Query not valid - END" << endl;

               //QMessageBox::warning(0, QObject::tr("Database Error (DataBase::createTheModeQuickReference)"),
               //                     query.lastError().text());
                query.finish();
               return false;
               //TODO: Manage this error, in case the query is NOK.

            }
        }
        query.finish();
          //qDebug() << "DataBase::createTheModeQuickReference: END" << endl;
        return true;
}


int DataBase::getBandIDFromName2(const QString b)
{//KEY, value
 //name, id
    /*
              KEY      Value
    QHash<QString, int> bandIDHash;
    QHash<QString, int> modeIDHash;
    QHash<int, QString> IDBandHash;
    QHash<int, QString> IDModeHash
    */

      //qDebug() << "DataBase::getBandIDFromName2: " << b << endl;
      //qDebug() << "DataBase::getBandIDFromName2: This line should be the last one... "  << endl;
    return getBandIdFromName(b);
      //qDebug() << "DataBase::getBandIDFromName2: CHECK IF THIS LINE IS SEEN"  << endl;


    if (b.length()<1)
    {
        return -3;
    }

    if (bandIDHash.contains(b))
    {
           //qDebug() << "DataBase::getBandIDFromName2: " << b << ":" <<  bandIDHash.value(b) << endl;
        return bandIDHash.value(b);
    }
    else
    {
          //qDebug() << "DataBase::getBandIDFromName2: Contains - False" << endl;
        return -1;
    }
       //qDebug() << "DataBase::getBandIDFromName2: Safety exit" << endl;
    return -2;

}

int DataBase::getModeIDFromName2(const QString b)
{
     //qDebug() << "DataBase::getModeIDFromName2: " << b << endl;
    return getModeIdFromName(b);

    if (b.length()<2)
    {
          //qDebug() << "DataBase::getModeIDFromName2: END -3" << endl;
        return -3;
    }

    if (modeIDHash.contains(b))
    {
         //qDebug() << "DataBase::getModeIDFromName2: END - " << b << ":" <<  modeIDHash.value(b) << endl;
        return modeIDHash.value(b);
    }
    else
    {
         //qDebug() << "DataBase::getModeIDFromName2: Contains - False - END" << endl;
        return -1;
    }
      //qDebug() << "DataBase::getModeIDFromName2: Safety exit - END" << endl;
    return -2;

}


int DataBase::getSubModeIDFromName2(const QString b)
{
     //qDebug() << "DataBase::getSubModeIDFromName2: " << b << endl;


    return getModeIdFromSubMode(b);

    //return getModeIdFromName(b);

    if (b.length()<2)
    {
          //qDebug() << "DataBase::getSubModeIDFromName2: END -3" << endl;
        return -3;
    }

    if (subModeIDHash.contains(b))
    {
         //qDebug() << "DataBase::getSubModeIDFromName2: END - " << b << ":" <<  modeIDHash.value(b) << endl;
        return subModeIDHash.value(b);
    }
    else
    {
         //qDebug() << "DataBase::getSubModeIDFromName2: Contains - False - END" << endl;
        return -1;
    }
      //qDebug() << "DataBase::getSubModeIDFromName2: Safety exit - END" << endl;
    return -2;

}

QString DataBase::getBandNameFromID2(const int _i)
{
     //qDebug() << "DataBase::getBandNameFromid2: " << QString::number(_i) << endl;
    return getBandNameFromNumber(_i);

    if (IDBandHash.contains(_i))
    {
          //qDebug() << "DataBase::getBandNameFromid2: END OK"  << endl;
        return IDBandHash.value(_i);
    }
    else
    {
          //qDebug() << "DataBase::getBandNameFromid2: END-1"  << endl;
        return "-1";
    }
      //qDebug() << "DataBase::getBandNameFromid2: END-2"  << endl;
    return "-2";

}
QString DataBase::getModeNameFromID2(const int _i)
{
       //qDebug() << "DataBase::getModeNameFromId2: " << QString::number(_i) << endl;
    return getSubModeNameFromNumber(_i);

    if (IDModeHash.contains(_i))
    {
          //qDebug() << "DataBase::getModeNameFromId2: END OK - " << IDModeHash.value(_i) << endl;
        return IDModeHash.value(_i);
    }
    else
    {
           //qDebug() << "DataBase::getModeNameFromId2: END-1" << endl;
        return "-1";
    }
       //qDebug() << "DataBase::getModeNameFromId2: END-2" << endl;
    return "-2";
}

QString DataBase::getSubModeNameFromID2(const int _i)
{
       //qDebug() << "DataBase::getSubModeNameFromId2: " << QString::number(_i) << endl;
    return getSubModeNameFromNumber(_i);

    if (IDSubModeHash.contains(_i))
    {
          //qDebug() << "DataBase::getSubModeNameFromId2: END OK - " << IDModeHash.value(_i) << endl;
        return IDSubModeHash.value(_i);
    }
    else
    {
           //qDebug() << "DataBase::getSubModeNameFromId2: END-1" << endl;
        return "-1";
    }
       //qDebug() << "DataBase::getSubModeNameFromId2: END-2" << endl;
    return "-2";
}

bool DataBase::createBandModeMaps()
{
     //qDebug() << "DataBase::createBandModeMaps" << endl;
    bool b = false;
    bool m = false;


     //return (b && m);
    if (isTheDBCreated())
    {
        b = createTheBandQuickReference();
        m = createTheModeQuickReference();

           //qDebug() << "DataBase::createBandModeMaps - isTheDbCreated TRUE" << endl;
        return (b && m);
    }
    else
    {
           //qDebug() << "DataBase::createBandModeMaps - isTheDbCreated FALSE" << endl;
        return false;
    }
    return false;
      //qDebug() << "DataBase::createBandModeMaps END" << endl;
}

QString DataBase::getFreqFromBandId(const int _i)
{
      //qDebug() << "DataBase::getFreqFromBandId" << endl;

    if (freqBandIdHash.contains(_i))
    {
          //qDebug() << "DataBase::getFreqFromBandId OK END" << endl;
        return freqBandIdHash.value(_i);
    }
    else
    {
          //qDebug() << "DataBase::getFreqFromBandId END-1" << endl;
        return "-1.0";
    }
      //qDebug() << "DataBase::getFreqFromBandId END-2" << endl;
    return "-2.0";
}

int DataBase::getLogTypeNumber(const QString _logType)
{
      //qDebug() << "DataBase::getLogTypeNumber: " << _logType << endl;
     QSqlQuery query;
     QString queryString = QString("SELECT id FROM supportedcontests WHERE name='%1'").arg(_logType);

     bool sqlOK = query.exec(queryString);


     if(!sqlOK)
     {
         queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
     }
     query.next();
     if ( query.isValid() )
     {
        return (query.value(0)).toInt();
     }
     else
     {
         query.finish();
         return -1;
     }
     query.finish();
     return -2;
}

QString DataBase::getLogTypeName(const int _logType)
{
       //qDebug() << "DataBase::getLogTypeName: " << QString::number(_logType) << endl;
     QSqlQuery query;
     QString queryString = QString("SELECT name FROM supportedcontests WHERE id='%1'").arg(_logType);
     bool sqlOK = query.exec(queryString);

     if(!sqlOK)
     {
         queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
         query.finish();
     }
     query.next();
     if ( query.isValid() )
     {
        return (query.value(0)).toString();
     }
     else
     {
         query.finish();
         return QString();
     }
     query.finish();
     return QString();
}

bool DataBase::updateToLatest()
{
/*
 * With the DB updates, the function that is called from here should be also updated.
 * The updateXXX are recursive calls that calls the previous one.
 *
 */
     //qDebug() << "DataBase::updateToLatest " << endl;
    //return updateTo010();
    return updateTo014();

}

bool DataBase::updateTo003()
{// Updates the DB to 0.0.3
 /*
  * This function should be used as a template to create the all the update functions implementing the needed changes
  * in the dB to update from one version to the following one.
  *
  * // dbVersion shows the DB version that is being deployed
  * // latestReaded shows the DB version that is currently deployed.
  *i.e.:
  *  QString stringQuery = QString ("ALTER TABLE award_enumeration ADD COLUMN dxcc INTEGER;");
  *
  */
     //qDebug() << "DataBase::updateTo003: latestRead: " << QString::number(latestReaded) << endl;
    bool IAmIn003 = false;
    bool IAmIn002 = false;
    bool ErrorUpdating = false;

    latestReaded = getDBVersion().toFloat();
    if (latestReaded >= float(0.003))
    {
        //IAmIn003 = true;
        return true;
    }
    else
    {
        IAmIn003 = false;
    }


    while (!IAmIn003 && !ErrorUpdating)
    {
        while (!IAmIn002 && !ErrorUpdating)
        {
            //IAmIn002 = updateTo002();
            IAmIn002 = true;
        }
        if (ErrorUpdating)
        {
            return false;
        }
        //DO ALL THE TASKS TO BE IN 0.003 from 0.002 HERE and set ErrorUpdating if it is not possible.
        IAmIn003 = true;
    }
    return IAmIn003;
}

bool DataBase::updateTo004()
{// Updates the DB to 0.0.4

     //qDebug() << "DataBase::updateTo004: latestRead: " << getDBVersion() << endl;
     //qDebug() << "DataBase::updateTo004: latestRead: " << QString::number(latestReaded) << endl;
    bool IAmIn004 = false;
    bool IAmIn003 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QSqlQuery query;

    bool sqlOk = false;
    latestReaded = getDBVersion().toFloat();
    if (latestReaded >= float(0.004))
    {
          //qDebug() << "DataBase::updateTo004: - I am in 004" << endl;
        return true;
    }
    else
    {
          //qDebug() << "DataBase::updateTo004: - I am not in 004" << endl;
        IAmIn004 = false;
    }

    while (!IAmIn004 && !ErrorUpdating)
    {
         //qDebug() << "DataBase::updateTo004: - And I am not in 004 nor ErrorUpdating" << endl;
        while (!IAmIn003 && !ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo004: - And I am not in 003" << endl;
            //IAmIn002 = updateTo002();
            IAmIn003 = true;
        }
        if (ErrorUpdating)
        {
            return false;
        }
         //qDebug() << "DataBase::updateTo004: - And I am in 003" << endl;
        sqlOk = updateDBVersion(softVersion, "0.004");
         //qDebug() << "DataBase::updateTo004: - Update Version" << endl;
        if (sqlOk)
        { // Version updated
             //qDebug() << "DataBase::updateTo004: - Update OK" << endl;
            sqlOk = execQuery(Q_FUNC_INFO, "DROP TABLE award_enumeration");
        }
        else
        { // Version not updated
             //qDebug() << "DataBase::updateTo004: - Update NOK" << endl;
        }
        //DO ALL THE TASKS TO BE IN 0.004 from 0.003 HERE and set ErrorUpdating if it is not possible.
         //qDebug() << "DataBase::updateTo004: - IAmIn004 = true" << endl;
        IAmIn004 = true;
    }

    if (IAmIn004)
    {
         //qDebug() << "DataBase::updateTo004: - Return... TRUE" << endl;
    }
    else
    {
         //qDebug() << "DataBase::updateTo004: - Return... FALSE" << endl;
    }
     //qDebug() << "DataBase::updateTo004: UPDATED OK!" << endl;
    return IAmIn004;
}


bool DataBase::updateTo005()
{// Updates the DB to 0.0.5

        //qDebug() << "DataBase::updateTo005: latestRead: " << getDBVersion() << endl;
       bool IAmIn005 = false;
       bool IAmIn004 = false;
       bool ErrorUpdating = false;
       QString stringQuery = QString();
       //QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
       QSqlQuery query;
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       //int errorCode;

       bool sqlOk = false;
       latestReaded = getDBVersion().toFloat();

       if (latestReaded >= float(0.005))
       {
              //qDebug() << "DataBase::updateTo005 - Already in 005" << endl;
           return true;
       }
       else
       {
              //qDebug() << "DataBase::updateTo005 - 005 update false" << endl;
           IAmIn005 = false;
       }


       while (!IAmIn005 && !ErrorUpdating)
       {
               //qDebug() << "DataBase::updateTo005 - I am not in 005" << endl;
           while (!IAmIn004 && !ErrorUpdating)
           {
                  //qDebug() << "DataBase::updateTo005 - I am not in 004" << endl;
               IAmIn004 = updateTo004();
           }
            //qDebug() << "DataBase::updateTo005 - I am in 004" << endl;
           if (ErrorUpdating)
           {
                //qDebug() << "DataBase::updateTo005 - 005 update false2" << endl;
               return false;
           }
           sqlOk = updateDBVersion(softVersion, "0.005");

           if (sqlOk)
           { // Version updated
               if (recreateContestData())
               {
                      //qDebug() << "DataBase::updateTo005 - recreateContestData OK" << endl;

                   sqlOk = execQuery(Q_FUNC_INFO, "DROP table logs");

                   sqlOk = createTableLogs(true);
                    if (!sqlOk)
                    {
                        //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                          //qDebug() << "DataBase::updateTo005 - logs table do not created" << endl;
                    }


                   if (howManyQSOsInLog(0)>0)
                   { // If the user has QSOs that were added with previous versions...
                     // We need to create a new log and rename all QSOs to that QSO.
                     //stringQuery = QString("UPDATE log SET lognumber='1' WHERE lognumber='0'");


                       msgBox.setText(QObject::tr("KLog has detected a previous log in the DB. All data will be migrated to a newly created DX type log for you."));
                       msgBox.exec();

                        if (execQuery(Q_FUNC_INFO, "UPDATE log SET lognumber='1' WHERE lognumber='0'"))
                        {}
                        else
                        {
                            //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                            //showError(QObject::tr("QSOs not updated to main log"));
                               //qDebug() << "DataBase::updateTo005 - QSOs not updated to main log" << endl;
                        }

                        QString dateString = (QDate::currentDate()).toString("yyyy/MM/dd");
                        QString callToUse = QString();
                        bool ok;
                        //QString text;

                        //text = QInputDialog::getText(this, QObject::tr("Station Callsign"), QObject::tr("Enter the Station Callsign you want to use in the imported log:"), QLineEdit::Normal, QObject::tr("N0CALL"), &ok);
                        QString text = (QInputDialog::getText(0, QObject::tr("KLog: Enter Station callsign"),
                                                                  QObject::tr("Enter the station callsign used in this log"), QLineEdit::Normal,
                                                              QObject::tr("Station Callsign"), &ok)).toUpper();
                        text = text.toUpper();
                            if (ok && !text.isEmpty())
                            {
                                callToUse = text;
                            }
                            else
                            {
                                callToUse = "N0CALL";
                            }

                        stringQuery = QString("INSERT INTO logs (logdate, stationcall, logtype, logtypen) values('%1','%2','DX', '1')").arg(dateString).arg(callToUse);
                        sqlOk = execQuery(Q_FUNC_INFO, stringQuery);


                        if (sqlOk)
                        {

                        }
                        else
                        {
                            //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                            //showError(QObject::tr("New Log not created"));
                               //qDebug() << "DataBase::updateTo005 - New Log not created" << endl;
                               //qDebug() << "DataBase::clearLog: Log deleted FAILED" << endl;
                        }
                   }
                   else
                   {

                   }

                   IAmIn005 = true;
               }
               else
               {
                      //qDebug() << "DataBase::updateTo005 - recreateContestData FAILED" << endl;
                   ErrorUpdating = true;
               }
           }
           else
           { // Version not updated
                  //qDebug() << "DataBase::updateTo005 - 005 update false6" << endl;
                ErrorUpdating = true;
           }
       }
          //qDebug() << "DataBase::updateTo005 - 005 updated 3" << endl;

       //TODO: Delete the table and recreate it
       if (IAmIn005)
       {
            msgBox.setText(QObject::tr("All the data was migrated correctly. You should now go to Setup->Preferences->Logs to check that everything is OK."));
            msgBox.exec();
       }
           //qDebug() << "DataBase::updateTo005 - I am in 005 already!! " << endl;
        //qDebug() << "DataBase::updateTo005: UPDATED OK!" << endl;
       return IAmIn005;
}

bool DataBase::recreateSatelliteData()
{
     //qDebug() << "DataBase::recreateSatelliteData"  << endl;
    QSqlQuery query;

    if (isTheTableExisting("satellites"))
    {
        if (execQuery(Q_FUNC_INFO, "DROP TABLE satellites"))
        {
            if (createTableSatellites(true))
            {
                  //qDebug() << "DataBase::recreateSatelliteData SAT table created"  << endl;
                return populateTableSatellites(true);
            }
            else
            {
                   //qDebug() << "DataBase::recreateSatelliteData SAT table NOT created"  << endl;
            }
        }
        else
        {
             //qDebug() << "DataBase::recreateSatelliteData execQuery FAILED"  << endl;
        }

    }
    else
    {
        if (createTableSatellites(true))
        {
              //qDebug() << "DataBase::recreateSatelliteData SAT table created"  << endl;
            return populateTableSatellites(true);
        }
        else
        {
               //qDebug() << "DataBase::recreateSatelliteData SAT table NOT created"  << endl;
        }

    }

     //qDebug() << "DataBase::recreateSatelliteData END FALSE"  << endl;
    return false;
}

bool DataBase::recreateContestData()
{
      //qDebug() << "DataBase::recreateContestData"  << endl;
    if (isTheTableExisting("contest"))
    {
        QSqlQuery query;
        bool sqlOk = false;
        sqlOk = execQuery(Q_FUNC_INFO, "DROP TABLE contest");
        if (sqlOk)
        {
            if (createTableContest())
            {
                return populateContestData();
            }
        }
    }
    else
    {
        if (createTableContest())
        {
            return populateContestData();
        }
    }
    return false;
}

bool DataBase::recreateSupportedContest()
{
     //qDebug() << "DataBase::recreateSupportedContest"  << endl;
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists supportedcontests");

    if (isTheTableExisting("supportedcontests"))
    {
        QSqlQuery query;
        bool sqlOk = false;
        sqlOk = execQuery(Q_FUNC_INFO, "DROP TABLE supportedcontests");
        if (sqlOk)
        {
              //qDebug() << "DataBase::recreateSupportedContest SQLOK"  << endl;
            if (createTableSupportedContest())
            {
                  //qDebug() << "DataBase::recreateSupportedContest - createTable OK"  << endl;
                return populateTableSupportedContest();
            }
            else
            {
                  //qDebug() << "DataBase::recreateSupportedContest createTableSupportContest FALSE"  << endl;
                return false;
            }
        }
        else
        {
            //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        }
    }
    else
    {
        if (createTableSupportedContest())
        {
              //qDebug() << "DataBase::recreateSupportedContest - createTable OK"  << endl;
            return populateTableSupportedContest();
        }
        else
        {
              //qDebug() << "DataBase::recreateSupportedContest createTableSupportContest FALSE"  << endl;
            return false;
        }
    }
      //qDebug() << "DataBase::recreateSupportedContest - FALSE end"  << endl;
    return false;
}


bool DataBase::recreatePropModes()
{
     //qDebug() << "DataBase::recreatePropModes"  << endl;
    if (isTheTableExisting("prop_mode_enumeration"))
    {
         //qDebug() << "DataBase::recreatePropModes: Table Exist"  << endl;
        bool sqlOk = false;
        sqlOk = execQuery(Q_FUNC_INFO, "DROP TABLE prop_mode_enumeration");

        if (sqlOk)
        {
             //qDebug() << "DataBase::recreatePropModes SQLOK"  << endl;
            if (createTablePropModes())
            {
                 //qDebug() << "DataBase::recreatePropModes - createTable OK"  << endl;
                if (populatePropagationModes())
                {
                     //qDebug() << "DataBase::recreatePropModes - populatePropModes OK"  << endl;
                    return true;
                }
                else
                {
                     //qDebug() << "DataBase::recreatePropModes - populatePropModes NOK"  << endl;
                    return false;
                }
            }
            else
            {
                   //qDebug() << "DataBase::recreatePropModes createTableSupportContest FALSE-1"  << endl;
                  return false;
            }
        }
        else
        {
            //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
               //qDebug() << "DataBase::recreatePropModes - prop_mode_enumeration table has not been dropped"  << endl;
               //qDebug() << "DataBase::recreatePropModes : Table creation FAILED" << endl;
        }

    }
    else
    {
         //qDebug() << "DataBase::recreatePropModes: Table does NOT Exist"  << endl;
        if (createTablePropModes())
        {
             //qDebug() << "DataBase::recreatePropModes - createTable OK"  << endl;
            if (populatePropagationModes())
            {
                 //qDebug() << "DataBase::recreatePropModes - populatePropModes OK"  << endl;
                return true;
            }
            else
            {
                 //qDebug() << "DataBase::recreatePropModes - populatePropModes NOK"  << endl;
                return false;
            }
        }
        else
        {
               //qDebug() << "DataBase::recreatePropModes createTableSupportContest FALSE-2"  << endl;
        }
    }

      //qDebug() << "DataBase::recreatePropModes - FALSE end"  << endl;
    return false;
}


bool DataBase::createTableLogs(const bool real)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
    //Creating the Sats DB to be able to include satellites to the LOTW

      //qDebug() << "DataBase::createTableLogs" << endl;

    QString stringQuery = QString();
    //QSqlQuery query;
    if (real)
    {
          //qDebug() << "DataBase::createTableLogs - logs" << endl;
        stringQuery = "CREATE TABLE logs" ;
    }
    else
    {
          //qDebug() << "DataBase::createTableLogs - logstemp" << endl;
        stringQuery = "CREATE TABLE logstemp" ;
    }

        stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                            "logdate VARCHAR(10), "
                                            "stationcall VARCHAR(15) NOT NULL, "
                                            "operators VARCHAR, "
                                            "comment VARCHAR, "
                                            "logtype VARCHAR, "
                                            "logtypen INTEGER, "
                                            "FOREIGN KEY (logtypen) REFERENCES supportedcontests(id),"
                                            "FOREIGN KEY (logtype) REFERENCES supportedcontests(name))");



          //qDebug() << "DataBase::createTableLogs - END" << endl;

        return execQuery(Q_FUNC_INFO, stringQuery);


}


bool DataBase::createTablePropModes()
{
     //qDebug() << "DataBase::createTablePropModes" << endl;

    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists prop_mode_enumeration");
    return execQuery(Q_FUNC_INFO, "CREATE TABLE prop_mode_enumeration (id INTEGER PRIMARY KEY AUTOINCREMENT, shortname VARCHAR(8), name VARCHAR(55) )");

}

bool DataBase::createTableSupportedContest()
{
       //qDebug() << "DataBase::createTableSupportedContest" << endl;

    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists supportedcontests");

    QString st = QString("CREATE TABLE supportedcontests ("
                         "id INTEGER PRIMARY KEY, "
                         "longname VARCHAR,"
                         "name VARCHAR)");
    return execQuery(Q_FUNC_INFO, st);
}

bool DataBase::createTableContest()
{
      //qDebug() << "DataBase::createTableContest" << endl;
    //QSqlQuery query;

    createTableSupportedContest();

    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists contest");

    execQuery(Q_FUNC_INFO, "CREATE TABLE contest ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "contest INTEGER NOT NULL,"
               "catoperator INTEGER NOT NULL,"
               "catassisted INTEGER NOT NULL,"
               "catpower INTEGER NOT NULL,"
               "catband INTEGER NOT NULL,"
               "catoverlay INTEGER NOT NULL,"
               "catmode INTEGER NOT NULL,"
               "FOREIGN KEY (contest) REFERENCES supportedcontests(id), "
               "FOREIGN KEY (catoperator) REFERENCES contestcatoperator(id), "
               "FOREIGN KEY (catassisted) REFERENCES contestcatassisted(id), "
               "FOREIGN KEY (catpower) REFERENCES contestcatpower(id), "
               "FOREIGN KEY (catband) REFERENCES contestcatband(id), "
               "FOREIGN KEY (catoverlay) REFERENCES contestcatoverlay(id), "
               "FOREIGN KEY (catmode) REFERENCES contestcatmode(id))");



    execQuery(Q_FUNC_INFO, "CREATE TABLE contestcatoperator ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    execQuery(Q_FUNC_INFO, "CREATE TABLE contestcatassisted ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    execQuery(Q_FUNC_INFO, "CREATE TABLE contestcatpower ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    execQuery(Q_FUNC_INFO, "CREATE TABLE contestcatband ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    execQuery(Q_FUNC_INFO, "CREATE TABLE contestcatoverlay ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    execQuery(Q_FUNC_INFO, "CREATE TABLE contestcatmode ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");

    populateTableSupportedContest();

    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoperator (id, name) VALUES ('0', 'N/A')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoperator (id, name) VALUES ('1', 'Single-Operator')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoperator (id, name) VALUES ('2', 'Multi-One')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoperator (id, name) VALUES ('3', 'Multi-Two')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoperator (id, name) VALUES ('4', 'Multi-Unlimited')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoperator (id, name) VALUES ('5', 'CheckLog')");

    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatassisted (id, name) VALUES ('0', 'N/A')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatassisted (id, name) VALUES ('1', 'Non-Assisted')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatassisted (id, name) VALUES ('2', 'Assisted')");

    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatpower (id, name) VALUES ('0', 'N/A')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatpower (id, name) VALUES ('1', 'High-Power')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatpower (id, name) VALUES ('2', 'Low-Power')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatpower (id, name) VALUES ('3', 'QRP')");

    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatband (id, name) VALUES ('0', 'N/A')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatband (id, name) VALUES ('1', 'All-Band')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatband (id, name) VALUES ('2', 'Single-Band')");

    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoverlay (id, name) VALUES ('0', 'N/A')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoverlay (id, name) VALUES ('1', 'Classic')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatoverlay (id, name) VALUES ('2', 'Rookie')");

    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatmode (id, name) VALUES ('0', 'N/A')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatmode (id, name) VALUES ('1', 'SSB')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatmode (id, name) VALUES ('2', 'CW')");
    execQuery(Q_FUNC_INFO, "INSERT INTO contestcatmode (id, name) VALUES ('3', 'MIXED')");

       //qDebug() << "DataBase::createTableContest END" << endl;
    return true;

}

bool DataBase::populateTableSupportedContest()
{
      //qDebug() << "DataBase::populateTableSupportedContest" << endl;
    // ADDING ALL THE CATEGORIES OPTIONS
    return execQuery(Q_FUNC_INFO, "INSERT INTO supportedcontests (id, longname, name) VALUES ('0', 'Normal log', 'DX')");

}

bool DataBase::createTableQSL_Via_enumeration()
{
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists qsl_via_enumeration");
    QString st = QString("CREATE TABLE qsl_via_enumeration ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "shortname VARCHAR(1) NOT NULL, "
                         "name VARCHAR(15) NOT NULL)");
     return execQuery(Q_FUNC_INFO, st);
}

bool DataBase::populateTableQSL_Via_enumeration()
{
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_via_enumeration (shortname, name) VALUES ('B', 'Bureau')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_via_enumeration (shortname, name) VALUES ('D', 'Direct')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_via_enumeration (shortname, name) VALUES ('E', 'Electronic')");
    execQuery(Q_FUNC_INFO, "INSERT INTO qsl_via_enumeration (shortname, name) VALUES ('M', 'Manager')");
    return true;
}


bool DataBase::createTableMode(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards

      //qDebug() << "DataBase::createTableMode" << endl;
    QString stringQuery = QString();
    QSqlQuery query;
    if (NoTmp)
    {
        stringQuery = "CREATE TABLE mode" ;
    }
    else
    {
        stringQuery = "CREATE TABLE modetemp" ;
    }

        stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                            "cabrillo VARCHAR(2) NOT NULL, "
                                             "name VARCHAR(40) NOT NULL, "
                                             "submode VARCHAR(40) NOT NULL, "
                                             "deprecated VARCHAR(1) NOT NULL)");

          //qDebug() << "DataBase::createTableMode END" << endl;

        return execQuery(Q_FUNC_INFO, stringQuery);

}

bool DataBase::populateTableMode(const bool NoTmp)
{
       //qDebug() << "DataBase::populateTableMode" << endl;
    //QSqlQuery query;
    QString tableName = QString();
    QString squery = QString();
    if (NoTmp)
    {
        tableName = "mode";
    }
    else
    {
        tableName = "modetemp";
    }


    bool sqlOK = execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('AM', 'AM', 'PH', '0')").arg(tableName));

    //int errorCode = -1;
    if (!sqlOK)
    {
        //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
           //qDebug() << "DataBase::populateTableMode: Mode table population FAILED" << endl;
        //errorCode = query.lastError().number();

    }
    else
    {
           //qDebug() << "DataBase::populateTableMode: Mode table population  OK" << endl;
    }

    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ARDOP', 'ARDOP', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('AMTORFEC', 'TOR', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ASCI', 'RTTY', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ATV', 'ATV', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('C4FM', 'C4FM', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP', 'CHIP', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP64', 'CHIP', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP128', 'CHIP', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CLO', 'CLO', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CONTESTI', 'CONTESTI', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CW', 'CW', 'CW', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DIGITALVOICE', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DSTAR', 'DSTAR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINO', 'DOMINO', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINOEX', 'DOMINO', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINOF', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FAX', 'FAX', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FM', 'FM', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FMHELL', 'HELL', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FT8', 'FT8', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSK31', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSK441', 'FSK441', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSKHELL', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSQCALL', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('GTOR', 'TOR', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELL', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELL80', 'HELL', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HFSK', 'HELL', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT', 'ISCAT', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT-A', 'ISCAT', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT-B', 'ISCAT', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4', 'JT4', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4A', 'JT4', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4B', 'JT4', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4C', 'JT4', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4D', 'JT4', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4E', 'JT4', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4F', 'JT4', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4G', 'JT4', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT6M', 'JT6M', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-1', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-2', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-5', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-10', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-30', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9A', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9B', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9C', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9D', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9E', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9E FAST', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9F', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9F FAST', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9G', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9G FAST', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9H', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9H FAST', 'JT9', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT44', 'JT44', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65', 'JT65', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65A', 'JT65', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65B', 'JT65', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65B2', 'JT65', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65C', 'JT65', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65C2', 'JT65', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK4', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK8', 'MFSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK11', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK16', 'MFSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK22', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK31', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK32', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK64', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK128', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MSK144', 'MSK144', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MT63', 'MT63', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 4/125', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 4/250', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 8/250', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 8/500', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 16/500', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 16/1000', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 32/1000', 'OLIVIA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA', 'OPERA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA-BEACON', 'OPERA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA-QSO', 'OPERA', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC', 'PAC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC2', 'PAC', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC3', 'PAC', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC4', 'PAC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAX', 'PAX', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAX2', 'PAX', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PCW', 'CW', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PKT', 'PKT', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK', 'PSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK10', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK31', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK63', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK63F', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK125', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK250', 'PSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK500', 'PSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK1000', 'PSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKAM10', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKAM31', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKAM50', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKFEC31', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK2K', 'PSK2K', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKHELL', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('Q15', 'Q15', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK31', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK63', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK125', 'PSK', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK250', 'PSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK500', 'PSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64', 'QRA64', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64A', 'QRA64', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64B', 'QRA64', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64C', 'QRA64', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64D', 'QRA64', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64E', 'QRA64', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS', 'ROS', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-EME', 'ROS', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-HF', 'ROS', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-MF', 'ROS', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('RTTY', 'RTTY', 'RY', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('RTTYM', 'RTTYM', 'RY', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SSB', 'SSB', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('LSB', 'SSB', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('USB', 'SSB', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SIM31', 'PSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SSTV', 'SSTV', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('T10', 'T10', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRB', 'THRB', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRBX', 'THRB', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('TOR', 'TOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('V4', 'V4', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('VOI', 'VOI', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('WINMOR', 'WINMOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('WSPR', 'WSPR', 'NO', '0')").arg(tableName));


    createTheModeQuickReference();
       //qDebug() << "DataBase::populateTableMode END" << endl;
    return true;
}


bool DataBase::createTableSatellites(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
    //Creating the Sats DB to be able to include satellites to the LOTW

     //qDebug() << "DataBase::createTableSatellites" << endl;

    // The satmode column has the following format: {Up/down-mode;Up/down-mode}
    // this way we can implement several freqs/modes per sat
    // Initially I will implement Up/Down only but KLog should be prepared to work with "-mode" also
    // being mode "SSB, CW, ... and other ADIF modes

    QString stringQuery = QString();


    QString table = QString();
    if (NoTmp)
    {
        table = "satellites" ;
    }
    else
    {
        table = "satellitestemp" ;
    }
    stringQuery = "CREATE TABLE "+ table;

    stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                            "satarrlid VARCHAR, "
                                            "satname VARCHAR, "
                                            "uplink VARCHAR,"
                                            "downlink VARCHAR,"
                                            "satmode VARCHAR, "
                                            "UNIQUE (satarrlid) )");
/*
 *
 *  uplink/downlink format is the following:
 *      Single frecuency: 145.950
 *      Segment: 145.950-145.975
 *      Several freqs: 145.950,435.950
 *      Several segments: 145.950-145.975,435.950-435.975
 *
 *  satmode format:
 *      Single mode: FM
 *      Modes complex Up/Downlink: USB/LSB
 *      Several modes (one per uplink/downlink pair): FM,SSB
 *      Several complex modes (one per uplink/downlink pair): USB/LSB,LSB/USB,FM
 *
 */
    QString delS = QString();
    delS = "DROP TABLE IF exists " + table;
    execQuery(Q_FUNC_INFO, delS);

    return execQuery(Q_FUNC_INFO, stringQuery);
/*
    bool sqlOK = query.exec(stringQuery);

    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
*/

      //qDebug() << "DataBase::createTableSatellites END" << endl;
    //return sqlOK;

}

bool DataBase::populateTableSatellites(const bool NoTmp)
{
    // Data must come from:
    // https://lotw.arrl.org/lotw-help/frequently-asked-questions/#sats
     //qDebug() << "DataBase::populateTableSatellites" << endl;

    //QSqlQuery query;
    QString tableName = QString();
    QString squery = QString();
    if (NoTmp)
    {
        tableName = "satellites";
    }
    else
    {
        tableName = "satellitestemp";
    }

    // The satmode column has the following format: {Up/down-mode;Up/down-mode}
    // this way we can implement several freqs/modes per sat
    // Initially I will implement Up/Down only but KLog should be prepared to work with "-mode" also
    // being mode "SSB, CW, ... and other ADIF modes
    //To add a band, just create another line:


    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-10', 'AMSAT-OSCAR 10', '435.030,146.180', '145.81', 'SSB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-13', 'AMSAT-OSCAR 13', '435.423-435.573', '145.975-145.825', 'SSB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-16', 'AMSAT-OSCAR 16', '145.92', '437.026', 'FM/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-21', 'OSCAR 21/RS-14', '', '145.8', 'CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-24', 'Arsene-OSCAR 24', '', '145.975', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-27', 'AMRAD-OSCAR 27', '145.85', '436.795', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-3', 'AMSAT-OSCAR 3', '145.975-146.025', '144.325-144.375', 'SSB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-4', 'AMSAT-OSCAR 4', '432.145-432.155', '144.300-144.310', 'SSB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-40', 'AMSAT-OSCAR 40','145.840-145.990,435.790-435.520', '2401.2225-2401.475', 'SSB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-51', 'AMSAT-OSCAR 51', '145.92', '435.3', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-6', 'AMSAT-OSCAR 6', '145.900-146.000', '29.450-29.550', 'SSB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-7', 'AMSAT-OSCAR 7', '145.850-145.950,432.180-432.120', '29.400-29.500,145.920-145.980', 'USB,LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-73', 'AMSAT-OSCAR 73', '435.150-435.130', '145.950-145.970', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-8', 'AMSAT-OSCAR 8', '145.850-145.900,145.900-146.000', '29.400-29.500,435.200-435.100', 'SSB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-85', 'AMSAT-OSCAR 85 (Fox-1A)', '435.170', '145.980', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('ARISS', 'ARISS', '145.200,144.490', '145.800,145.800', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('BIRD-BT', 'BHUTAN-1', '145.825', ',145.825', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('BIRD-MY', 'UiTMSat-1', '145.825', ',145.825', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('BIRD-PH', 'MAYA-1', '145.825', ',145.825', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('BY70-1', 'Bayi Kepu Weixing 1', '145.92', '436.2', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('CAS-3H', 'LilacSat 2', '144.350,144.390', '437.225,144.390', 'FM,PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('CAS-4A', 'CAMSAT 4A', '435.210-435.230', '145.880-145.860', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('CAS-4B', 'CAMSAT 4B', '435.270-435.290', '145.935-145.915', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('DO-64', 'Delfi OSCAR-64', '', '145.870', 'CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('EO-79', 'FUNcube-3', '435.047-435.077', '145.935-145.965', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('EO-88', 'Emirates OSCAR 88 (Nayif-1)', '435.045-435.015', '145.960-145.990', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('FO-12', 'Fuji-OSCAR 12', '145.900-146.000,145.85', '435.900-435.800,435.91', 'SSB,PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('FO-20', 'Fuji-OSCAR 20', '145.900-146.000', '435.900-435.800', 'SSB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('FO-29', 'Fuji-OSCAR 29', '145.900-145.999', '435.900-435.800', 'LSB/USB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('HO-68', 'Hope OSCAR 68', '145.925-145.975,145.825', '435.765-435.715,435.675', 'LSB/USB,FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('IO-86', 'Indonesia OSCAR 86 (LAPAN-ORARI)', '435.880', '145.880', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('LO-19', 'Lusat-OSCAR 19', '145.840-145.900', '437.125-437.150', 'CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('LO-78', 'LituanicaSAT-1', '145.95,145.85', '435.1755,437.543', 'FM,PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('LO-87', 'LUSEX-OSCAR 87', '435.935-435.965', '145.935-145.965' 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('MIREX', 'Mir packet digipeater', '145.985', '145.985', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('NO-44', 'Navy-OSCAR 44', '145.827', '145.827', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('NO-83', 'BRICsat','145.825,28.120', '145.825,435.975' 'PKT,PSK31')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('NO-84', 'PSAT', '145.825,28.120', '435.350', 'PKT,PSK31')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-1',  'Radio Sputnik 1', '145', '29', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-10', 'Radio Sputnik 10', '','29.357,29.403', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-11', 'Radio Sputnik 11', '','29.357,29.403', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-12', 'Radio Sputnik 12', '21.210-21.250', '29.410-29.450', 'SSB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-13', 'Radio Sputnik 13', '21.260-21.300', '145.860-145.900', 'SSB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-15', 'Radio Sputnik 15', '', '29.3525-29.3987', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-2',  'Radio Sputnik 2', '145', '29', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, satmode) VALUES ('RS-5', 'Radio Sputnik 5', 'CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, satmode) VALUES ('RS-6', 'Radio Sputnik 6', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, satmode) VALUES ('RS-7', 'Radio Sputnik 7', 'CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, satmode) VALUES ('RS-8', 'Radio Sputnik 8', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('SAREX', 'Shuttle Amateur Radio Experiment packet digipeater', '144.80,144.49', '144.55', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('SO-35', 'Sunsat-OSCAR 35', '436.291', '145.825', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('SO-41', 'Saudi-OSCAR 41', '145.850', '436.775', 'CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('SO-50', 'Saudi-OSCAR 50', '145.850', '436.795', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('SO-67', 'Sumbandila OSCAR 67', '145.875', '435.345', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('UKUBE1', 'UKube-1 (FUNcube-2)', '435.080-435.060', '145.930-145.950,2401.0', 'LSB/USB,CW')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('UO-14', 'UOSAT-OSCAR 14', '145.975', '435.07', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('VO-52', 'VUsat-OSCAR 52', '435.220-435.280,435.225-435.275', '145.930-145.870,145.925-145.875', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('XW-2A', 'Hope 2A (CAS-3A)', '435.030-435.050', '145.665-145.685', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('XW-2B', 'Hope 2B (CAS-3B)', '435.090-435.110', '145.730-145.750', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('XW-2C', 'Hope 2C (CAS-3C)', '435.150-435.170', '145.795-145.815', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('XW-2D', 'Hope 2D (CAS-3D)', '435.210-435.230', '145.860-145.880', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('XW-2E', 'Hope 2E (CAS-3E)', '435.270-435.290', '145.915-145.935', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('XW-2F', 'Hope 2F (CAS-3F)', '435.330-435.350', '145.980-145.999', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('LO-90', 'LilacSat-OSCAR 90 (LilacSat-1)', '145.985', '436.510', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-91', 'RadFxSat (Fox-1B)', '435.250', '145.960', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('AO-92', 'Fox-1D', '435.350,1267.35', '145.880', 'FM')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('FS-3', 'FalconSat-3', '435.103', '145.840', 'PKT')").arg(tableName));


     //qDebug() << "DataBase::populateTableSatellites - END" << endl;
    return true;
}

bool DataBase::createTableEntity(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
      //qDebug() << "DataBase::createTableEntity" << endl;

    QString stringQuery = QString();
    //QSqlQuery query;
    if (NoTmp)
    {
        stringQuery = "CREATE TABLE entity" ;
    }
    else
    {
        stringQuery = "CREATE TABLE entitytemp" ;
    }

        stringQuery = stringQuery + QString( " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                             "name VARCHAR(40) NOT NULL,"
                                             "cqz INTEGER NOT NULL, "
                                             "ituz INTEGER NOT NULL, "
                                             "continent INTEGER NOT NULL, "
                                             "latitude REAL NOT NULL, "
                                             "longitude REAL NOT NULL, "
                                             "utc INTEGER NOT NULL, "
                                             "dxcc INTEGER NOT NULL, "
                                             "mainprefix VARCHAR(15) NOT NULL, "
                                             "deleted INTEGER, "
                                             "sincedate VARCHAR(10), "
                                             "todate VARCHAR(10), "
                                             "isoname VARCHAR(10), "
                                             "UNIQUE (dxcc, mainprefix), "
                                             "FOREIGN KEY (continent) REFERENCES continent(shortname) )");

       //qDebug() << "DataBase::createTableEntity END" << endl;
    return execQuery(Q_FUNC_INFO, stringQuery);


    //TODO: To add some columns in this the table to mark if worked/confirmed/band/Mode

}


bool DataBase::createTableBand(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards

      //qDebug() << "DataBase::createTableBand" << endl;
    QString stringQuery = QString();
    //QSqlQuery query;
    if (NoTmp)
    {
        stringQuery = "CREATE TABLE band" ;
    }
    else
    {
        stringQuery = "CREATE TABLE bandtemp" ;
    }

        stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                            "lower REAL NOT NULL, "
                                            "upper REAL NOT NULL, "
                                            "cabrillo VARCHAR(6) NOT NULL, "
                                            "name VARCHAR(40) NOT NULL, "
                                            "UNIQUE (lower, upper, cabrillo, name) )");

      //qDebug() << "DataBase::createTableBand END" << endl;
    return execQuery(Q_FUNC_INFO, stringQuery);

}



bool DataBase::populateTableBand(const bool NoTmp)
{
    // Cabrillo definition: http://wwrof.org/cabrillo/cabrillo-specification-v3/

      //qDebug() << "DataBase::populateTableBand" << endl;


    QString tableName = QString();
    QString squery = QString();
    if (NoTmp)
    {
        tableName = "band";
    }
    else
    {
        tableName = "bandtemp";
    }


    //To add a band, just create another line:
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('0', '0', '0', 'Light')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('1mm', '241000', '250000', '241G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2mm', '142000', '149000', '142G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2.5mm', '119980', '120020', '119G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('4mm', '75500', '81000', '75G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('6mm', '47000', '47200', '47G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('1.25CM', '24000', '24250', '24G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('3CM', '10000', '10500', '10G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('6CM', '5650', '5925', '5.7G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('9CM', '3300', '3500', '3.4G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('13CM', '2340', '2450', '2.3G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('23CM', '1240', '1300', '1.2G')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('33CM', '902', '928', '902')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('70CM', '420', '450', '432')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('1.25M', '222', '225', '222')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2M', '144', '148', '144')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('4M', '70', '71', '4M')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('6M', '50', '54', '50')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('10M', '28.0', '29.7', '28000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('12M', '24.89', '24.99', '24000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('15M', '21.0', '21.45', '21000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('17M', '18.068', '18.168', '18100')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('20M', '14.0', '14.35', '14000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('30M', '10.0', '10.15', '10000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('40M', '7.0', '7.3', '7000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('60M', '5.102', '5.404', '5100')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('80M', '3.5', '4.0', '3500')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('160M', '1.8', '2.0', '1800')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('560M', '0.501', '0.504', '560M')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('630M', '0.472', '0.479', '630M')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2190M', '0.1357', '0.1378', '2190M')").arg(tableName));

    createTheBandQuickReference();

      //qDebug() << "DataBase::populateTableBand END" << endl;
    return true;
}


bool DataBase::populatePropagationModes()
{

      //qDebug() << "DataBase::populatePropagationModes" << endl;
    //QSqlQuery query;

    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('AS', 'Aircraft Scatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('AUR', 'Aurora')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('AUE', 'Aurora-E')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('BS', 'Back scatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('ECH', 'EchoLink')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('EME', 'Earth-Moon-Earth')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('ES', 'Sporadic E')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('FAI', 'Field Aligned Irregularities')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('F2', 'F2 Reflection')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('INTERNET', 'Internet-assisted')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('ION', 'Ionoscatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('IRL', 'IRLP')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('MS', 'Meteor scatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('RPT', 'Terrestrial or atmospheric repeater or transponder')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('RS', 'Rain scatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('SAT', 'Satellite')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('TEP', 'Trans-equatorial')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('TR', 'Tropospheric ducting')"));

     //qDebug() << "DataBase::populatePropagationModes END" << endl;
    return true;
}


bool DataBase::populateContestData()
{
      //qDebug() << "DataBase::populateContestData" << endl;


    
    // CONTEST DEFINITIONS START HERE

    // DX
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (0, 0, 0, 0, 0, 0, 0)");

    // DX START
/*
    // CQ WW DX SSB START
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 2, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 2, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 2, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 2, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 2, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 2, 0, 1)");


    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 1, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 2, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 1, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 2, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 1, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 2, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 1, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 2, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 1, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 2, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 1, 1, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 2, 1, 1)");

    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 1, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 2, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 1, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 2, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 1, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 2, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 1, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 2, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 1, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 2, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 1, 2, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 2, 2, 1)");

    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 2, 0, 1, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 2, 0, 2, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 3, 0, 1, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 3, 0, 2, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 4, 0, 1, 1, 0, 1)");
    execQuery(Q_FUNC_INFO, "INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 5, 0, 0, 0, 0, 1)");
    // CQ WW DX SSB END
*/

      //qDebug() << "DataBase::populateContestData END" << endl;
    return true;
}

bool DataBase::howManyQSOsInLog(const int i)
{
      //qDebug() << "DataBase::howManyQSOsInLog" << endl;

    QSqlQuery query;
    QString sqlQueryString = QString("SELECT COUNT(id) from log WHERE lognumber='%1'").arg(i);
    bool sqlOK = query.exec(sqlQueryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
              //qDebug() << "DataBase::howManyQSOsInLog OK END" << endl;
            return (query.value(0)).toInt();
        }
        else
        {
              //qDebug() << "DataBase::howManyQSOsInLog END-1" << endl;
            query.finish();
            return -1;
        }
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
          //qDebug() << "DataBase::howManyQSOsInLog END-2" << endl;
        return -1;
    }
    query.finish();
    return -2;
}

bool DataBase::updateTo006()
{// Updates the DB to 0.0.6

     //qDebug() << "DataBase::updateTo006: latestRead: " << getDBVersion() << endl;
    bool IAmIn006 = false;
    bool IAmIn005 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    //QString dateString = (date.currentDateTime()).toString("yyyyMMdd");

    bool sqlOk = false;
    latestReaded = getDBVersion().toFloat();
    if (latestReaded >= float(0.006))
    {
        return true;
    }
    else
    {
        IAmIn006 = false;
    }

    while (!IAmIn006 && !ErrorUpdating)
    {
           //qDebug() << "DataBase::updateTo006: - Still not in 006" << endl;
        while (!IAmIn005 && !ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo006: - And still not in 005" << endl;
            IAmIn005 = updateTo005();
        }
         //qDebug() << "DataBase::updateTo006: - Already in 005" << endl;
        if (ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo006: UPDATED NOK!(0)" << endl;
            return false;
        }

        sqlOk = updateDBVersion(softVersion, "0.006");

        if(sqlOk)
        {
             //qDebug() << "DataBase::updateTo006: - DB Updated" << endl;
            sqlOk = recreatePropModes();

            if(sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - recreatePropModes OK" << endl;
                sqlOk = updateTableLog(6); // We copy the log into logtemp
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: - recreatePropModes NOK" << endl;
                ErrorUpdating = true;
                IAmIn006 = false;
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(1)" << endl;
                return false;
            }

            sqlOk = createTableBand(false);             // We create the bandTemp
            if (sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - createTableBand OK" << endl;
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(2)" << endl;
                ErrorUpdating = true;
                //return false;
            }

            sqlOk = populateTableBand(false);           // Populate the bandTemp
            if (sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - populateTableBand OK" << endl;
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(3)" << endl;
                ErrorUpdating = true;
            }

            sqlOk = updateBandIdTableLogToNewOnes();
            if (sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - updateBandIdTableLogToNewOnes OK" << endl;
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(4)" << endl;
                ErrorUpdating = true;
            }

            sqlOk = updateBandIdTableAward(1); // DXCC
            if (sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - updateBandIdTableAward 1 OK" << endl;
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(5)" << endl;
                ErrorUpdating = true;
            }

            sqlOk = updateBandIdTableAward(2); // WAZ
            if (sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - updateBandIdTableAward 2 OK" << endl;
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(6)" << endl;
                ErrorUpdating = true;
            }


            if (execQuery(Q_FUNC_INFO, "DROP TABLE band"))
            {
                if (execQuery(Q_FUNC_INFO, "ALTER TABLE bandtemp RENAME TO band"))
                {
                     //qDebug() << "DataBase::updateTo006 - bandtemp renamed" << endl;
                }
                else
                {       
                     //qDebug() << "DataBase::updateTo006 - ERROR - bandtemp not renamed" << endl;
                    ErrorUpdating = true;
                     //qDebug() << "DataBase::updateTo006: UPDATED NOK!(7)" << endl;
                }                
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006 - ERROR - bandtemp not dropped" << endl;
                ErrorUpdating = true;
                IAmIn006 = false;
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(8)" << endl;
                ErrorUpdating = true;
            }

            sqlOk = updateTheModeTableAndSyncLog();
            if (sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - updateTheModeTableAndSyncLog OK" << endl;
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(9)" << endl;
                ErrorUpdating = true;
            }

            sqlOk = createTableClubLogStatus();
            if (sqlOk)
            {
                 //qDebug() << "DataBase::updateTo006: - createTableClubLogStatus OK" << endl;
                sqlOk = populateTableClubLogStatus();
                if (sqlOk)
                {
                     //qDebug() << "DataBase::updateTo006: - populateTableClubLogStatus OK" << endl;
                }
                else
                {
                     //qDebug() << "DataBase::updateTo006: UPDATED NOK!(10)" << endl;
                    ErrorUpdating = true;
                }
            }
            else
            {
                 //qDebug() << "DataBase::updateTo006: UPDATED NOK!(11)" << endl;
                ErrorUpdating = true;
            }

        }
        else
        {// Version not updated
             //qDebug() << "DataBase::updateTo006: - DB NOT Updated" << endl;
            ErrorUpdating = true;
            IAmIn006 = false;
             //qDebug() << "DataBase::updateTo006: UPDATED NOK!(12)" << endl;
            ErrorUpdating = true;
        }
         //qDebug() << "DataBase::updateTo006: - IAmIn006 = TRUE" << endl;
        IAmIn006 = true;

    }
        //DO ALL THE TASKS TO BE IN 0.006 from 0.005 HERE and set ErrorUpdating if it is not possible.
           //qDebug() << "DataBase::updateTo006 - I am in 006 " << endl;
       //qDebug() << "DataBase::updateTo006 - END " << endl;
     //qDebug() << "DataBase::updateTo006: UPDATED OK!" << endl;
    return IAmIn006;
}

bool DataBase::updateTableLog(const int _v)
{
     //qDebug() << "DataBase::updateTableLog " << endl;
    createTableLog(false);
    QString queryString;
    switch (_v)
    {
    case 6:     // If 6, we copy in logtemp the full data coming from the old log. This way, the structure of
                // the log table is updated without any data loss.
        queryString = QString ("INSERT INTO logtemp (qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, srx, stx, points, multiplier, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, band_rx, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_gridsquare, my_iota, my_iota_island_id, my_lat, my_lon, my_name, my_rig, my_sig, my_sig_info, my_state, my_street, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, srx_string, stx_string, state, station_callsign, swl, ten_ten, tx_pwr, web, qso_date_off, time_off, transmiterid, marked, lognumber) SELECT qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, srx, stx, points, multiplier, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, band_rx, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_gridsquare, my_iota, my_iota_island_id, my_lat, my_lon, my_name, my_rig, my_sig, my_sig_info, my_state, my_street, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, srx_string, stx_string, state, station_callsign, swl, ten_ten, tx_pwr, web, qso_date_off, time_off, transmiterid, marked, lognumber FROM log");
    break;
    default:
          //qDebug() << "DataBase::updateTableLog FALSE END" << endl;
        return false;
    break;
    }

    if (execQuery(Q_FUNC_INFO, queryString))
    {
          //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Query executed" << endl;
        queryString = "DROP TABLE log";
        if (execQuery(Q_FUNC_INFO, queryString))
        {
              //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Table log dropped" << endl;

            queryString = "ALTER TABLE logtemp RENAME TO log" ;
            if (execQuery(Q_FUNC_INFO, queryString))
            {
                 //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - tmp renamed - END" << endl;
                return true;
            }
            else
            {                
                  //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Renaming failed" << endl;
            }
        }
        else
        {
              //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Table log Not dropped" << endl;
        }
    }
    else
    {
          //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - query failed" << endl;
    }
      //qDebug() << "DataBase::updateTableLog END" << endl;
    return false;
}


bool DataBase::createTableClubLogStatus()
{
      //qDebug() << "createTableClubLogStatus" << endl;

    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists clublog_status");
    QString queryString = QString("CREATE TABLE clublog_status ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "shortname VARCHAR(1) NOT NULL, "
               "name VARCHAR(15) NOT NULL)");

    return execQuery(Q_FUNC_INFO, queryString);

}

bool DataBase::populateTableClubLogStatus()
{
      //qDebug() << "populateTableClubLogStatus" << endl;

    QString queryString = "INSERT INTO clublog_status (shortname, name) VALUES ('Y', 'Uploaded')";


    if (execQuery(Q_FUNC_INFO, queryString))
    {
        queryString = "INSERT INTO clublog_status (shortname, name) VALUES ('N', 'Do not upload')";
        if (execQuery(Q_FUNC_INFO, queryString))
        {
            queryString = "INSERT INTO clublog_status (shortname, name) VALUES ('M', 'Modified')";
              //qDebug() << "populateTableClubLogStatus END" << endl;
            return execQuery(Q_FUNC_INFO, queryString);
        }

    }
      //qDebug() << "populateTableClubLogStatus FALSE END" << endl;
    return false;
}
/*
bool DataBase::moveFromModeIdToSubmodeId()
{
    return false;
}
*/

bool DataBase::updateTableEntity()
{
      //qDebug() << "DataBase::updateTableEntity" << endl;
    bool result = false;
    QString stringQuery;
    bool sqlOk;

    result = createTableEntity(false);  // Now we have a temp entity table with the correct format
    if (result)
    {
          //qDebug() << "DataBase::updateTableEntity: Table entitytemp created!" << endl;
        // Now we need to move all the data from the old to the temp entity table.

        stringQuery = QString("INSERT INTO entitytemp (name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate) SELECT name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate FROM entity");

        sqlOk = execQuery(Q_FUNC_INFO, stringQuery);

        if (sqlOk)
        {
              //qDebug() << "DataBase::updateTableEntity: Data copied from entity to entitytemp!" << endl;
            stringQuery = "DROP TABLE entity";
            if (execQuery(Q_FUNC_INFO, stringQuery))
            {
                  //qDebug() << "DataBase::updateTableEntity: Table entity DELETED" << endl;
                stringQuery = "ALTER TABLE entitytemp RENAME TO entity";
                return execQuery(Q_FUNC_INFO, stringQuery);

            }
            else
            {
               return false;
            }

        }
        else
        {
              //qDebug() << "DataBase::updateTableEntity: Data NOT copied from entity to entitytemp!" << endl;
            //TODO: If it fails, we should manage errors...
            return false;
        }

    }
    else
    {
          //qDebug() << "DataBase::updateTableEntity: Table entitytemp NOT created!" << endl;
        return false;
    }

    return false;
}

bool DataBase::updateTableLogs()
{
      //qDebug() << "DataBase::updateTableLogs" << endl;
    bool result = false;
    QString stringQuery;
    bool sqlOk;

    result = createTableLogs(false);  // Now we have a temp entity table with the correct format

    if (result)
    {
          //qDebug() << "DataBase::updateTableLogs: Table logstemp created!" << endl;
        // Now we need to move all the data from the old to the temp logs table.

        stringQuery = QString("INSERT INTO logstemp (logdate, stationcall, comment, logtype, logtypen) SELECT logdate, stationcall, comment, logtype, logtypen FROM logs");

        sqlOk = execQuery(Q_FUNC_INFO, stringQuery);

        if (sqlOk)
        {
              //qDebug() << "DataBase::updateTableLogs: data copied" << endl;

            stringQuery = "DROP TABLE logs";
            if (execQuery(Q_FUNC_INFO, stringQuery))
            {
                  //qDebug() << "DataBase::updateTableLogs: Table logs DELETED" << endl;
                stringQuery = "ALTER TABLE logstemp RENAME TO logs";
                return execQuery(Q_FUNC_INFO, stringQuery);
            }
            else
            {                
                 //qDebug() << "DataBase::updateTableLogs: Table logs NOT DELETED" << endl;
               return false;
            }

        }
        else
        {
            //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
              //qDebug() << "DataBase::updateTableLogs: data NOT copied" << endl;
            //TODO: If it fails, we should manage errors...
            return false;
        }

    }
    else
    {
          //qDebug() << "DataBase::updateTableLogs: Table logstemp NOT created!" << endl;
        return false;
    }

      //qDebug() << "DataBase::updateTableLogs: END" << endl;
    return false;
}

bool DataBase::updateModeIdFromSubModeId()
{// Updates the log with the new mode IDs in each QSO:
    // STEP-1: Get the modeid and QSOid from the log
    // STEP-2: uses the modeid to get the name of the mode in the mode table (the old one)
    // STEP-3: uses the name of the mode in the modetemp table (the new one) to get the new ID
    // STEP-4: Updates the new ID in the QSO in the log


       //qDebug() << "DataBase::updateModeIdFromSubModeId: "  << endl;
    bool cancel = false;
    bool alreadyCancelled = false;
    QString modetxt = QString();
    QString sq = QString();
    bool sqlOk2 = false;
    bool sqlOk3 = false;
    int modeFound = -1;
    int id = -1;

    int qsos;
    int i = 0;
    QString aux;
    QSqlQuery query, query2;
    bool sqlOk = query.exec("SELECT COUNT (*) FROM log");

    if (sqlOk)
    {
        //QSqlDatabase::database().commit();
        query.next();
        qsos = (query.value(0)).toInt();
        query.finish();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
           //qDebug() << "DataBase::updateModeIdFromSubModeId: FALSE END"  << endl;
        return false;
    }

    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating mode information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

    sqlOk = query.exec("SELECT modeid, id FROM log ORDER BY modeid");                                                   // STEP-1

    if (sqlOk)
    {

        while (query.next())
        {
            modetxt = QString();
            modeFound = -1;

            if (query.isValid())
            {

                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating mode information...") + "\n" + QObject::tr("QSO: ")  + QString::number(i) + "/" + QString::number(qsos);
                    progress.setLabelText(aux);
                    progress.setValue(i);
                }

                modeFound = (query.value(0)).toInt();
                id = (query.value(1)).toInt();
                   //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-1) modeFound (numb): " << QString::number(modeFound) << endl;

                modetxt = getModeNameFromNumber(modeFound, false);                                                      //STEP-2

                   //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-2) mode found (txt): " << modetxt << endl;

                //TODO The following query can be executed in: getModeIdFromSubMode()

                sq = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(modetxt);                                // STEP-3              
                sqlOk2 = query2.exec(sq);

                if (sqlOk2)
                {

                       //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-3) sqlOK2 TRUE" << endl;
                    if (query2.next())
                    {
                        if (query2.isValid())
                        {
                            modeFound = query2.value(0).toInt();
                            query2.finish();
                            sq = QString ("UPDATE log SET modeid='%1' WHERE id='%2'").arg(modeFound).arg(id);           // STEP-4
                            sqlOk3 = execQuery(Q_FUNC_INFO, sq);

                            if (sqlOk3)
                            {

                                   //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-4) ID: " << QString::number(id) << " updated to: " << QString::number(modeFound) <<"/"<< modetxt << endl;

                            }
                            else
                            {
                                //queryErrorManagement(Q_FUNC_INFO, query3.lastError().databaseText(), query3.lastError().number(), query3.lastQuery());
                                   //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-4) ID: " << QString::number(id) << " NOT updated-2"  << endl;
                            }
                        }
                        else
                        {
                            query2.finish();
                               //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-3) query2 not valid "   << endl;
                        }
                    }
                    else
                    {
                        //qDebug() << "DataBase::updateModeIdFromSubModeId: query2 not next "   << endl;
                    }

                }
                else
                {
                    queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().number(), query2.lastQuery());
                    query2.finish();
                       //qDebug() << "DataBase::updateModeIdFromSubModeId: ID: " << QString::number(id) << " NOT updated-1"  << endl;
                }

            }

            if ( progress.wasCanceled() )
            {
                if (alreadyCancelled)
                {

                }
                else
                {
                    alreadyCancelled = true;

                    QMessageBox msgBox;
                    aux = QObject::tr("Canceling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
                    msgBox.setText(aux);
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Yes:
                          // Yes was clicked
                            cancel = true;
                          break;

                      case QMessageBox::No:
                          // No Save was clicked
                            cancel = false;
                            progress.setCancelButton(0);
                          break;
                      default:
                          // should never be reached
                            cancel = false;
                          break;
                    }
                }
            }


        }
        query.finish();
        if (cancel && (!alreadyCancelled))
        {
              //qDebug() << "DataBase::updateModeIdFromSubModeId: FALSE END 2"  << endl;
            query.finish();
            return false;
        }

          //qDebug() << "DataBase::updateModeIdFromSubModeId: END"  << endl;

        query.finish();
        return true;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataBase::updateModeIdFromSubModeId: FALSE END 3"  << endl;
        query.finish();
        return false;
    }
      //qDebug() << "DataBase::updateModeIdFromSubModeId: CHECK IF this is seen - END"  << endl;
    query.finish();
   return false;
}


bool DataBase::updateBandIdTableLogToNewOnes()
{
     //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: "  << endl;

    QString bandtxt = QString();

    bool cancel = false;
    bool alreadyCancelled = false;
    //int errorCode = -1;

    QString sq = QString();
    bool sqlOk2 = false;
    bool sqlOk3 = false;
    int bandFound = -1;
    int id = -1;
    int qsos;
    int i = 0;
    QString aux;
    QSqlQuery query, query2;
    bool sqlOk = query.exec("SELECT COUNT (*) FROM log");

    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
        query.finish();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
      //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END"  << endl;
        query.finish();
        return false;
    }

    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating bands information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

    sqlOk = query.exec("SELECT bandid, id FROM log ORDER BY bandid DESC");

    if (sqlOk)
    {
        while (query.next() && (!cancel) )
        {
            bandtxt = QString();
            bandFound = -1;

            if (query.isValid())
            {
                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating bands information...") + "\n" + QObject::tr("QSO: ")  + QString::number(i) + "/" + QString::number(qsos);
                    progress.setLabelText(aux);
                    progress.setValue(i);
                }


                bandFound = (query.value(0)).toInt();
                id = (query.value(1)).toInt();
                bandtxt = getBandNameFromNumber(bandFound);

                   //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: band found: " << bandtxt << endl;

                sq = QString("SELECT id FROM bandtemp WHERE name='%1'").arg(bandtxt);
                sqlOk2 = query2.exec(sq);

                if (sqlOk2)
                {
                    if (query2.next())
                    {
                        if (query2.isValid())
                        {
                            bandFound = query2.value(0).toInt();

                            sq = QString ("UPDATE log SET bandid='%1' WHERE id='%2'").arg(bandFound).arg(id);                            
                            query.finish();
                            sqlOk3 = execQuery(Q_FUNC_INFO, sq);
                            if (sqlOk3)
                            {
                                   //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt << endl;
                            }
                            else
                            {
                                //queryErrorManagement(Q_FUNC_INFO, query3.lastError().databaseText(), query3.lastError().number(), query3.lastQuery());
                                   //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-2"  << endl;
                                  //qDebug() << "DataBase::updateBandIdTableLogToNewOnes - QSOs not updated to main log" << endl;
                            }
                        }
                        else
                        {
                               //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: query2 not valid "   << endl;
                        }
                    }
                    else
                    {
                        //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: query2 not next "   << endl;
                    }
                    query2.finish();

                }
                else
                {
                    queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().number(), query2.lastQuery());
                    query2.finish();
                       //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-1"  << endl;
                }

            }

            if ( progress.wasCanceled() )
            {
                if (alreadyCancelled)
                {

                }
                else
                {
                    alreadyCancelled = true;

                    QMessageBox msgBox;
                    aux = QObject::tr("Canceling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
                    msgBox.setText(aux);
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Yes:
                          // Yes was clicked
                            cancel = true;
                          break;

                      case QMessageBox::No:
                          // No Save was clicked
                            cancel = false;
                            progress.setCancelButton(0);
                          break;
                      default:
                          // should never be reached
                            cancel = false;
                          break;
                    }
                }
            }

        }
        query.finish();
        if (cancel && (!alreadyCancelled))
        {
              //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END 2"  << endl;
            return false;
        }
          //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: END OK"  << endl;
        return true;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
          //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END 3"  << endl;
        return false;
    }

}

bool DataBase::updateBandIdTableAward(const int _db)
{
     //qDebug() << "DataBase::updateBandIdTableAward: "  << endl;

    QString table = QString();
    QString field = QString();
    QString awardSelected = QString();

    switch (_db) {
      case 1: //
            table = "awarddxcc";
            field = "band";
            awardSelected = "DXCC";
          break;

      case 2:
            table = "awardwaz";
            field = "band";
            awardSelected = "WAZ";
          break;
      default:
              //qDebug() << "DataBase::updateBandIdTableAward: FALSE END"  << endl;
            return false;
          break;
    }

    QString bandtxt = QString();

    bool cancel = false;
    bool alreadyCancelled = false;
    //int errorCode = -1;


    QString sq = QString();
    bool sqlOk2 = false;
    bool sqlOk3 = false;
    int bandFound = -1;
    int id = -1;
    int qsos;
    int i = 0;
    QString aux;
    QSqlQuery query, query2;


    sq = QString("SELECT COUNT (*) FROM %1").arg(table);

    bool sqlOk = query.exec(sq);

    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
        query.finish();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
          //qDebug() << "DataBase::updateBandIdTableAward: FALSE END-2"  << endl;
        return false;
    }

    int step = util->getProgresStepForDialog(qsos);
    QString progressmsg = QString(QObject::tr("Updating bands information in %1 status...")).arg(awardSelected);

    QProgressDialog progress(progressmsg, QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);


    sq = QString("SELECT %1, id FROM %2 ORDER BY %3 DESC").arg(field).arg(table).arg(field);

    sqlOk = query.exec(sq);

    if (sqlOk)
    {
        while (query.next() && (!cancel) )
        {
            bandtxt = QString();
            bandFound = -1;

            if (query.isValid())
            {
                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating bands information...") + "\n" + QObject::tr("Progress: ")  + QString::number(i) + "/" + QString::number(qsos);
                    progress.setLabelText(aux);
                    progress.setValue(i);
                }


                bandFound = (query.value(0)).toInt();
                id = (query.value(1)).toInt();
                bandtxt = getBandNameFromNumber(bandFound);

                   //qDebug() << "DataBase::updateBandIdTableAward: band found: " << bandtxt << endl;

                sq = QString("SELECT id FROM bandtemp WHERE name='%1'").arg(bandtxt);
                sqlOk2 = query2.exec(sq);

                if (sqlOk2)
                {
                    if (query2.next())
                    {
                        if (query2.isValid())
                        {
                            bandFound = query2.value(0).toInt();
                            query2.finish();
                            sq = QString ("UPDATE %1 SET %2='%3' WHERE id='%4'").arg(table).arg(field).arg(bandFound).arg(id);

                            sqlOk3 = execQuery(Q_FUNC_INFO, sq);
                            if (sqlOk3)
                            {
                                   //qDebug() << "DataBase::updateBandIdTableAward: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt << endl;
                            }
                            else
                            {
                                //queryErrorManagement(Q_FUNC_INFO, query3.lastError().databaseText(), query3.lastError().number(), query3.lastQuery());
                                  //qDebug() << "DataBase::updateBandIdTableAward: ID: " << QString::number(id) << " NOT updated-2"  << endl;

                                  //qDebug() << "DataBase::updateBandIdTableAward - QSOs not updated to main log" << endl;
                            }
                        }
                        else
                        {
                               //qDebug() << "DataBase::updateBandIdTableAward: query2 not valid "   << endl;
                        }
                    }
                    else
                    {
                        //qDebug() << "DataBase::updateBandIdTableAward: query2 not next "   << endl;
                    }
                    query2.finish();

                }
                else
                {
                    queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().number(), query2.lastQuery());
                    query2.finish();
                       //qDebug() << "DataBase::updateBandIdTableAward: ID: " << QString::number(id) << " NOT updated-1"  << endl;
                }

            }

            if ( progress.wasCanceled() )
            {
                if (alreadyCancelled)
                {

                }
                else
                {
                    alreadyCancelled = true;

                    QMessageBox msgBox;
                    aux = QObject::tr("Canceling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
                    msgBox.setText(aux);
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Yes:
                          // Yes was clicked
                            cancel = true;
                          break;

                      case QMessageBox::No:
                          // No Save was clicked
                            cancel = false;
                            progress.setCancelButton(0);
                          break;
                      default:
                          // should never be reached
                            cancel = false;
                          break;
                    }
                }
            }
        }
        if (cancel && (!alreadyCancelled))
        {
              //qDebug() << "DataBase::updateBandIdTableAward: FALSE END-3"  << endl;
            query.finish();
            return false;
        }
         //qDebug() << "DataBase::updateBandIdTableAward: END OK"  << endl;
        query.finish();
        return true;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataBase::updateBandIdTableAward: FALSE END-4"  << endl;
        query.finish();
        return false;
    }
       //qDebug() << "DataBase::updateBandIdTableAward: CHECK IF SEEN END"  << endl;
    query.finish();
      return false;
}

bool DataBase::updateModeIdTableAward(const int _db)
{
     //qDebug() << "DataBase::updateModeIdTableAward: " << QString::number(_db)  << endl;

    QString table = QString();
    QString field = "mode";
    QString awardSelected = QString();

    switch (_db) {
      case 1: //
            table = "awarddxcc";
            awardSelected = "DXCC";
          break;

      case 2:
            table = "awardwaz";
            awardSelected = "WAZ";
          break;
      default:
              //qDebug() << "DataBase::updateModeIdTableAward: FALSE END"   << endl;
            return false;
          break;
    }

    QString bandtxt = QString();

    bool cancel = false;
    bool alreadyCancelled = false;
    //int errorCode = -1;


    QString sq = QString();
    bool sqlOk2 = false;
    bool sqlOk3 = false;
    int bandFound = -1;
    int id = -1;
    int qsos;
    int i = 0;
    QString aux;
    QSqlQuery query, query2;


    sq = QString("SELECT COUNT (*) FROM %1").arg(table);

    bool sqlOk = query.exec(sq);

    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
        query.finish();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataBase::updateModeIdTableAward: FALSE END-2"   << endl;
        query.finish();
        return false;
    }

    int step = util->getProgresStepForDialog(qsos);
    QString progressmsg = QString(QObject::tr("Updating mode information in %1 status...")).arg(awardSelected);

    QProgressDialog progress(progressmsg, QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);


    sq = QString("SELECT %1, id FROM %2 ORDER BY %3 DESC").arg(field).arg(table).arg(field);

    sqlOk = query.exec(sq);

        //qDebug() << "DataBase::updateModeIdTableAward (query): " << query.lastQuery()  << endl;
    if (sqlOk)
    {
        while (query.next() && (!cancel) )
        {
            bandtxt = QString();
            bandFound = -1;

            if (query.isValid())
            {
                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating bands information...") + "\n" + QObject::tr("Progress: ")  + QString::number(i) + "/" + QString::number(qsos);
                    progress.setLabelText(aux);
                    progress.setValue(i);
                }


                bandFound = (query.value(0)).toInt();                              
                id = (query.value(1)).toInt();
                   //qDebug() << "DataBase::updateModeIdTableAward: bandfound: "  << QString::number(bandFound) << endl;
                   //qDebug() << "DataBase::updateModeIdTableAward: id: "  << QString::number(id) << endl;

                bandtxt = getSubModeNameFromNumber(bandFound, true);

                   //qDebug() << "DataBase::updateModeIdTableAward: mode found: " << bandtxt << "/" << QString::number(bandFound) << endl;

                sq = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(bandtxt);                
                sqlOk2 = query2.exec(sq);


                   //qDebug() << "DataBase::updateModeIdTableAward (query2): " << query2.lastQuery()  << endl;
                if (sqlOk2)
                {
                    if (query2.next())
                    {
                        if (query2.isValid())
                        {
                            bandFound = query2.value(0).toInt();
                            query2.finish();

                            sq = QString ("UPDATE %1 SET %2='%3' WHERE id='%4'").arg(table).arg(field).arg(bandFound).arg(id);
                            sqlOk3 = execQuery(Q_FUNC_INFO, sq);

                            if (sqlOk3)
                            {
                                   //qDebug() << "DataBase::updateModeIdTableAward: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt << endl;
                            }
                            else
                            {
                                //queryErrorManagement(Q_FUNC_INFO, query3.lastError().databaseText(), query3.lastError().number(), query3.lastQuery());
                                  //qDebug() << "DataBase::updateModeIdTableAward: ID: " << QString::number(id) << " NOT updated-2"  << endl;

                                  //qDebug() << "DataBase::updateModeIdTableAward - QSOs not updated to main log" << endl;
                            }

                        }
                        else
                        {
                               //qDebug() << "DataBase::updateModeIdTableAward: query2 not valid "   << endl;
                        }
                    }
                    else
                    {
                        //qDebug() << "DataBase::updateModeIdTableAward: query2 not next "   << endl;
                    }

                }
                else
                {
                    queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().number(), query2.lastQuery());
                    query2.finish();
                       //qDebug() << "DataBase::updateModeIdTableAward: ID: " << QString::number(id) << " NOT updated-1"  << endl;
                }

            }

            if ( progress.wasCanceled() )
            {
                if (alreadyCancelled)
                {

                }
                else
                {
                    alreadyCancelled = true;

                    QMessageBox msgBox;
                    aux = QObject::tr("Canceling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
                    msgBox.setText(aux);
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Yes:
                          // Yes was clicked
                            cancel = true;
                          break;

                      case QMessageBox::No:
                          // No Save was clicked
                            cancel = false;
                            progress.setCancelButton(0);
                          break;
                      default:
                          // should never be reached
                            cancel = false;
                          break;
                    }
                }
            }
        }
        if (cancel && (!alreadyCancelled))
        {
              //qDebug() << "DataBase::updateModeIdTableAward: FALSE END-3"   << endl;
            query.finish();
            return false;
        }
         //qDebug() << "DataBase::updateModeIdTableAward: END OK"  << endl;
        query.finish();
        return true;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataBase::updateModeIdTableAward: FALSE END-4"   << endl;
        query.finish();
        return false;
    }
      //qDebug() << "DataBase::updateModeIdTableAward: Checkif seen END"   << endl;
    query.finish();
     return false;
}

/*

bool DataBase::updateModeIdTableLogToNewOnes()
{
      //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: "  << endl;

    QString bandtxt = QString();

    bool cancel = false;
    bool alreadyCancelled = false;
    int errorCode = -1;

    QString sq = QString();
    bool sqlOk2 = false;
    bool sqlOk3 = false;
    int bandFound = -1;
    int id = -1;
    int qsos;
    int i = 0;
    QString aux;
    QSqlQuery query, query2, query3;
    bool sqlOk = query.exec("SELECT COUNT (*) FROM log");
    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
    }
    else
    {
        return false;
    }

    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating mode information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

    sqlOk = query.exec("SELECT modeid, id FROM log ORDER BY bandid DESC");
    if (sqlOk)
    {
        while (query.next() && (!cancel) )
        {
            bandtxt = QString();
            bandFound = -1;

            if (query.isValid())
            {
                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating mode information...\n QSO: ")  + QString::number(i) + "/" + QString::number(qsos);
                    progress.setLabelText(aux);
                    progress.setValue(i);
                }


                bandFound = (query.value(0)).toInt();
                id = (query.value(1)).toInt();
                bandtxt = getModeNameFromNumber(bandFound, false);

                   //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: mode found: " << bandtxt << endl;

                sq = QString("SELECT id FROM modetemp WHERE name='%1'").arg(bandtxt);
                sqlOk2 = query2.exec(sq);
                if (sqlOk2)
                {
                    if (query2.next())
                    {
                        if (query2.isValid())
                        {
                            bandFound = query2.value(0).toInt();

    sq = QString ("UPDATE log SET modeid='%1' WHERE id='%2'").arg(bandFound).arg(id);
                            sqlOk3 = query3.exec(sq);
                            if (sqlOk3)
                            {
                                   //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt << endl;
                            }
                            else
                            {
                                   //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-2"  << endl;
                                  //qDebug() << "DataBase::updateModeIdTableLogToNewOnes - QSOs not updated to main log" << endl;
                                errorCode = query3.lastError().number();
                                  //qDebug() << "DataBase::updateModeIdTableLogToNewOnes - query error: " << QString::number(errorCode) << endl;
                                  //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastQuery: " << query3.lastQuery()  << endl;
                                  //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastError-data: " << query3.lastError().databaseText()  << endl;
                                  //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastError-driver: " << query3.lastError().driverText()  << endl;
                                  //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastError-n: " << QString::number(query3.lastError().number() ) << endl;

                            }

                        }
                        else
                        {
                               //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: query2 not valid "   << endl;
                        }
                    }
                    else
                    {
                        //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: query2 not next "   << endl;
                    }

                }
                else
                {
                       //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-1"  << endl;
                }

            }

            if ( progress.wasCanceled() )
            {
                if (alreadyCancelled)
                {

                }
                else
                {
                    alreadyCancelled = true;

                    QMessageBox msgBox;
                    aux = QObject::tr("Canceling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
                    msgBox.setText(aux);
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    int ret = msgBox.exec();
                    switch (ret) {
                      case QMessageBox::Yes:
                          // Yes was clicked
                            cancel = true;
                          break;

                      case QMessageBox::No:
                          // No Save was clicked
                            cancel = false;
                            progress.setCancelButton(0);
                          break;
                      default:
                          // should never be reached
                            cancel = false;
                          break;
                    }
                }
            }

        }
        if (cancel && (!alreadyCancelled))
        {
            return false;
        }
           //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: FINISHED OK"  << endl;
        return true;
    }
    else
    {
        return false;
    }

}
*/

bool DataBase::updateTo007()
{// Updates the DB to 0.0.7

     //qDebug() << "DataBase::updateTo007: latestRead: " << getDBVersion() << endl;
    bool IAmIn007 = false;
    bool IAmIn006 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    //QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    QSqlQuery query;
    latestReaded = getDBVersion().toFloat();
    bool sqlOk = false;

    if (latestReaded >= float(0.007))
    {
           //qDebug() << "DataBase::updateTo007: - I am in 007" << endl;
        return true;
    }
    else
    {
           //qDebug() << "DataBase::updateTo007: - I am not in 007" << endl;
        IAmIn007 = false;
    }


    while (!IAmIn007 && !ErrorUpdating)
    {
        while (!IAmIn006 && !ErrorUpdating)
        {
              //qDebug() << "DataBase::updateTo007: - And I am not in 006" << endl;
            IAmIn006 = updateTo006();


        }
         //qDebug() << "DataBase::updateTo007: - And I am in 006!!!!" << endl;
        if (ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo007: NOK-1" << endl;
            return false;
        }
        sqlOk = updateDBVersion(softVersion, "0.007");

        if (sqlOk)
        { // Version updated
            IAmIn007 = updateTableLog(6);
        }
        else
        { // Version not updated
             //qDebug() << "DataBase::updateTo007: NOK-2" << endl;
            return false;
        }
        //DO ALL THE TASKS TO BE IN 0.007 from 0.006 HERE and set ErrorUpdating if it is not possible.
        IAmIn007 = true;
    }
     //qDebug() << "DataBase::updateTo007: END" << endl;
    return IAmIn007;
}



bool DataBase::updateTo008()
{// Updates the DB to 0.0.8

     //qDebug() << "DataBase::updateTo008: latestRead: " << getDBVersion() << endl;
    bool IAmIn008 = false;
    bool IAmIn007 = false;
    bool ErrorUpdating = false;
    //QString stringQuery = QString();
    //QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    //QSqlQuery query;

    bool sqlOk = false;
    latestReaded = getDBVersion().toFloat();
    if (latestReaded >= (0.008))
    {
           //qDebug() << "DataBase::updateTo008: - I am in 008" << endl;
        return true;
    }
    else
    {
           //qDebug() << "DataBase::updateTo008: - I am not in 008" << endl;
        IAmIn008 = false;
    }


    while (!IAmIn008 && !ErrorUpdating)
    {
        while (!IAmIn007 && !ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo008: - And I am not in 007" << endl;
            IAmIn007 = updateTo007();

        }
         //qDebug() << "DataBase::updateTo008: - I am in 007" << endl;
        if (ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo008: - NOK-1" << endl;
            return false;
        }

        //DO ALL THE TASKS TO BE IN 0.008 from 0.007 HERE and set ErrorUpdating if it is not possible.

        IAmIn008 = updateTheModeTableAndSyncLog();
        if (IAmIn008)
        {
            sqlOk = updateDBVersion(softVersion, "0.008");
            if (sqlOk)
            { // Version updated
                 //qDebug() << "DataBase::updateTo008: - OK" << endl;
            }
            else
            { // Version not updated
                 //qDebug() << "DataBase::updateTo008: - NOK-3" << endl;
                return false;
            }
        }
        else
        {
             //qDebug() << "DataBase::updateTo008: - NOK-2" << endl;
            return false;
        }

    }
     //qDebug() << "DataBase::updateTo008: - END" << endl;
    return IAmIn008;
}


bool DataBase::updateTo009()
{// Updates the DB to 0.0.9 - We add the Satellite tables

     //qDebug() << "DataBase::updateTo009: latestRead: " << getDBVersion() << endl;
    bool IAmIn009 = false;
    bool IAmIn008 = false;
    bool ErrorUpdating = false;
    //QString stringQuery = QString();
    //QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    //QSqlQuery query;

    bool sqlOk = false;

    //if (latestReaded >= 0.009)
      //qDebug() << "DataBase::updateTo009: Checking:" << QString::number(latestReaded) << ":" << QString::number(0.009)<< endl;
    latestReaded = getDBVersion().toFloat();
   if (latestReaded >= float(0.009))
    //if ((latestReaded = 0.009) || (latestReaded > 0.009))
    {
          //qDebug() << "DataBase::updateTo009: - I am in 009" << endl;
        //IAmIn009 = true;
        return true;
    }
    else
    {
           //qDebug() << "DataBase::updateTo009: - I am not in 009 I am in: " << QString::number(latestReaded)<< endl;
        IAmIn009 = false;
    }
      //qDebug() << "DataBase::updateTo009: compared latestRead: " << QString::number(latestReaded) << endl;
    while (!IAmIn009 && !ErrorUpdating)
    {
        while (!IAmIn008 && !ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo009: - And I am not in 008" << endl;
            IAmIn008 = updateTo008();

        }
         //qDebug() << "DataBase::updateTo009: - And I am already at least in 008" << endl;
        if (ErrorUpdating)
        {
            return false;
        }
        sqlOk = updateDBVersion(softVersion, "0.009");

        if (sqlOk)
        { // Version updated
             //qDebug() << "DataBase::updateTo009: - version updated" << endl;
            //IAmIn009 = updateTableLog(6);
        }
        else
        { // Version not updated
             //qDebug() << "DataBase::updateTo009: - version not updated" << endl;
        }
        //DO ALL THE TASKS TO BE IN 0.009 from 0.008 HERE and set ErrorUpdating if it is not possible.
        if (recreateSatelliteData())
        //if (createTableSatellites(true))
        {
             //qDebug() << "DataBase::updateTo009: - createTableSatellites OK" << endl;
            //if (populateTableSatellites(true))
            if (true)
            {
                 //qDebug() << "DataBase::updateTo009: - populateTableSatellites OK" << endl;
                if (updateTableEntity())
                {
                    //qDebug() << "DataBase::updateTo009: - updateTableEntity OK" << endl;
                   if (updateTheEntityTableISONames())
                   {
                       //qDebug() << "DataBase::updateTo009: - isonames updated" << endl;
                      // Now I need to update the logs table

                      if (updateTableLogs())
                      {
                           //qDebug() << "DataBase::updateTo009: - logs updated and Function finished successfuly!!" << endl;
                          IAmIn009 = true;
                      }
                      else
                      {
                           //qDebug() << "DataBase::updateTo009: - logs NOT updated" << endl;
                          IAmIn009 = false;
                          ErrorUpdating = true;
                      }
                   }
                   else
                   {
                       //qDebug() << "DataBase::updateTo009: - isonames NOT updated" << endl;
                      IAmIn009 = false;
                      ErrorUpdating = true;
                   }
                }
                else
                {
                   ErrorUpdating = true;
                   IAmIn009 = false;
                }
            }
            else
            {
                 //qDebug() << "DataBase::updateTo009: - populateTableSatellites FALSE" << endl;
                ErrorUpdating = true;
                IAmIn009 = false;
            }
        }
        else
        {
             //qDebug() << "DataBase::updateTo009: - createTableSatellites FALSE" << endl;
            ErrorUpdating = true;
            IAmIn009 = false;
        }
    }

       //qDebug() << "DataBase::updateTo009: - END" << endl;
    return IAmIn009;
}


bool DataBase::updateTo010()
{// Updates the DB to 0.010:
    // We add FT8 mode and
    // AS Propagation

     //qDebug() << "DataBase::updateTo010: latestRead: " << getDBVersion() << endl;
    bool IAmIn010 = false;
    bool IAmIn009 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QSqlQuery query;

    bool sqlOk = false;
    latestReaded = getDBVersion().toFloat();
      //qDebug() << "DataBase::updateTo010: Checking (latestRead/dbVersion):" << QString::number(latestReaded) << "/" << QString::number(dbVersion) << endl;
    if (latestReaded >= float(0.01))
    {
         //qDebug() << "DataBase::updateTo010: - I am in 010" << endl;
        IAmIn010 = true;
        return true;
    }
    else
    {
           //qDebug() << "DataBase::updateTo010: - I am not in 010 I am in: " << QString::number(latestReaded)<< endl;

        while (!IAmIn009 && !ErrorUpdating)
        {
               //qDebug() << "DataBase::updateTo010: - Check if I am in 009: !" << endl;
            IAmIn009 = updateTo009();

            if (IAmIn009)
            {
                   //qDebug() << "DataBase::updateTo010: - updateTo009 returned TRUE - I am in 0.009: " << QString::number(latestReaded) << endl;
            }
            else
            {
                   //qDebug() << "DataBase::updateTo010: - updateTo009 returned FALSE - I am NOT in 0.009: " << QString::number(latestReaded) << endl;
                ErrorUpdating = false;
            }
        }

        if (ErrorUpdating)
        {
               //qDebug() << "DataBase::updateTo010: - I Could not update to: " << QString::number(dbVersion) << endl;
            return false;
        }

    }


    sqlOk = execQuery(Q_FUNC_INFO, "UPDATE band SET lower = '0.1357', upper = '0.1378' WHERE name='2190M'");
    if (sqlOk)
    {
           //qDebug() << "DataBase::updateTo010: - Band update OK" << endl;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
           //qDebug() << "DataBase::updateTo010: - Band update NOK" << endl;
    }


    execQuery(Q_FUNC_INFO, QString("INSERT INTO mode (submode, name, cabrillo, deprecated) VALUES ('FT8', 'FT8', 'NO', '0')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('AS', 'Aircraft Scatter')"));


    if (updateDBVersion(softVersion, "0.010"))
    {
           //qDebug() << "DataBase::updateTo010: - We are in 010! " << endl;
        IAmIn010 = true;
    }
    else
    {
           //qDebug() << "DataBase::updateTo010: - Failed to go to 010! " << endl;
        IAmIn010 = false;
    }

       //qDebug() << "DataBase::updateTo010: - END" << endl;
    return IAmIn010;
}

bool DataBase::updateDBVersion(QString _softV, QString _dbV)
{
    QString dateString = (QDate::currentDate()).toString("yyyyMMdd");

     //qDebug() << "DataBase::updateDBVersion: (date/SoftVersion/dbVersion): " << dateString << "/" << _softV << "/" << _dbV << endl;
    QString stringQuery = "INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + _softV + "', '" + _dbV + "')";
    return execQuery(Q_FUNC_INFO, stringQuery);
}


bool DataBase::updateTheModeTableAndSyncLog()
{
       //qDebug() << "DataBase::updateTheModeTableAndSyncLog" << endl;
    QSqlQuery query;

    createTableMode(false);         // Create modetemp
    populateTableMode(false);       // Populate modetemp

    updateModeIdFromSubModeId();    // Updates the log with the new mode IDs in each QSO
    //updateModeIdTableAward(1); //DXCC
    //updateModeIdTableAward(2); // WAZ
    bool sqlOK;
    //QSqlDatabase::database().commit();

    sqlOK = execQuery(Q_FUNC_INFO, "DROP TABLE mode");
    if (sqlOK)
    {
        //QSqlDatabase::database().commit();
           //qDebug() << "DataBase::updateTheModeTableAndSyncLog - OK - mode was dropped" << endl;

        return execQuery(Q_FUNC_INFO, "ALTER TABLE modetemp RENAME TO mode");

    }
    else
    {
        //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataBase::updateTheModeTableAndSyncLog - ERROR - modetemp not dropped" << endl;
        return false;
    }
       //qDebug() << "DataBase::updateTheModeTableAndSyncLog END" << endl;
    return true;
}

bool DataBase::recreateTableBand()
{
      //qDebug() << "DataBase::recreateTableBand" << endl;
    QSqlQuery query;

    createTableBand(false);         // Create modetemp
    populateTableBand(false);       // Populate modetemp

    if (execQuery(Q_FUNC_INFO, "DROP TABLE band"))
    {
        return execQuery(Q_FUNC_INFO, "ALTER TABLE bandtemp RENAME TO mode");
    }
    else
    {
        //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
          //qDebug() << "DataBase::recreateTableBand - ERROR - bandtemp not dropped" << endl;
        return false;
    }
      //qDebug() << "DataBase::recreateTableBand END" << endl;
    return true;
}

bool DataBase::updateTheEntityTableISONames()
{
     //qDebug() << "DataBase::updateTheEntityTableISONames" << endl;
    QSqlQuery query;
    QString sq;
    bool sqlOK;

    //First of all we will check if the entity table does contain data. We can't update something non existent!
    if (!hasTheTableData("entity"))
    {
          //qDebug() << "DataBase::updateTheEntityTableISONames: Entity has NO data" << endl;
        return false;
    }

      //qDebug() << "DataBase::updateTheEntityTableISONames-1" << endl;
    sq = QString ("UPDATE entity SET isoname='mt' WHERE dxcc='246'");  //Sovereign Order of Malta

    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;
    }
      //qDebug() << "DataBase::updateTheEntityTableISONames-2" << endl;

    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='247'");  //Spratly
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;
    }

    sq = QString ("UPDATE entity SET isoname='mc' WHERE dxcc='260'");  //Monaco
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mu' WHERE dxcc='4'");  // Agalega
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mu' WHERE dxcc='165'");  //Mauricio
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mu' WHERE dxcc='207'");  //Rodriguez
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gq' WHERE dxcc='49'");  // Equatorial Guinea
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fj' WHERE dxcc='176'");  //Fidji
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gq' WHERE dxcc='195'");  //Annobon
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fj' WHERE dxcc='489'");  // Conway reef
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fj' WHERE dxcc='460'");  // Rotuma
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sz' WHERE dxcc='468'");  // Swaziland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tn' WHERE dxcc='474'");  // Tunisia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vn' WHERE dxcc='293'");  // Vietnam
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gn' WHERE dxcc='107'");  // Guinea
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bv' WHERE dxcc='24'");  // Bouvet
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='no' WHERE dxcc='199'");  // Peter 1 Is"
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='az' WHERE dxcc='18'");  // Azerbaijan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ge' WHERE dxcc='75'");  // Georgia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='me' WHERE dxcc='514'");  // Montenegro
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lk' WHERE dxcc='315'");  // Sri lanka
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ch' WHERE dxcc='117'");  // ITU HQ
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='289'");  // UN HQ
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tl' WHERE dxcc='511'");  // Timor Leste
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='il' WHERE dxcc='336'");  // Israel
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ly' WHERE dxcc='436'");  // Libya
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cy' WHERE dxcc='215'");  // Cyprus
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tz' WHERE dxcc='470'");  // Tanzania
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ng' WHERE dxcc='450'");  // Nigeria
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mg' WHERE dxcc='438'");  // Madagascar
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mr' WHERE dxcc='444'");  // Mauritania
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ne' WHERE dxcc='187'");  // Niger
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tg' WHERE dxcc='483'");  // Togo
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ws' WHERE dxcc='190'");  // Samoa
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ug' WHERE dxcc='286'");  // Uganda
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ke' WHERE dxcc='430'");  // Kenya
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sn' WHERE dxcc='456'");  // Senegal
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jm' WHERE dxcc='82'");  // Jamaica
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='es' WHERE dxcc='281'");  // Spain
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ls' WHERE dxcc='432'");  //Lesotho
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mw' WHERE dxcc='440'");  // Malawi
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dz' WHERE dxcc='400'");  // Algeria
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ye' WHERE dxcc='492'");  // Yemen
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bb' WHERE dxcc='62'");  // Barbados
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mv' WHERE dxcc='159'");  // Maldives
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gy' WHERE dxcc='129'");  // Guyana
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hr' WHERE dxcc='497'");  // Croatia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gh' WHERE dxcc='424'");  // Ghana
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mt' WHERE dxcc='257'");  // Malta
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='zm' WHERE dxcc='482'");  // Zambia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kw' WHERE dxcc='348'");  // Kuwait
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sl' WHERE dxcc='458'");  // Sierra Leone
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='my' WHERE dxcc='299'");  // West Malaysia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='my' WHERE dxcc='46'");  // East Malaysia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='np' WHERE dxcc='369'");  // Nepal
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cd' WHERE dxcc='414'");  // Dem Rep Congo
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bi' WHERE dxcc='404'");  // Burundi
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sg' WHERE dxcc='381'");  // Singapore
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='rw' WHERE dxcc='454'");  // Rwanda
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tt' WHERE dxcc='90'");  // Trinidad & Tobago
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bw' WHERE dxcc='402'");  // Botswana
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='to' WHERE dxcc='160'");  //  Tonga
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='om' WHERE dxcc='370'");  // Oman
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bt' WHERE dxcc='306'");  // Bhutan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ae' WHERE dxcc='391'");  // Un Arab Emirates
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='qa' WHERE dxcc='376'");  // Qatar
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bh' WHERE dxcc='304'");  // Bahrain
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pk' WHERE dxcc='372'");  // Pakistan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tw' WHERE dxcc='386'");  // Taiwan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tw' WHERE dxcc='505'");  // Pratas Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cn' WHERE dxcc='318'");  // China
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nr' WHERE dxcc='157'");  //  Nauru
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ad' WHERE dxcc='203'");  // Andorra
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gm' WHERE dxcc='422'");  // Gambia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bs' WHERE dxcc='60'");  // Bahamas
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mz' WHERE dxcc='181'");  // Mozambique
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='112'");  // Chile
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='217'");  // San Felix
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='47'");  // Easter Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='125'");  //  Juan Fernandez is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cu' WHERE dxcc='70'");  // Cuba
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ma' WHERE dxcc='446'");  // Morocco
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bo' WHERE dxcc='104'");  // Bolivia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pt' WHERE dxcc='272'");  // Portugal
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pt' WHERE dxcc='256'");  // Madeira
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pt' WHERE dxcc='149'");  // Azores
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='uy' WHERE dxcc='144'");  // Uruguay
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ca' WHERE dxcc='211'");  // Sable Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ca' WHERE dxcc='252'");  // St Paul is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ao' WHERE dxcc='401'");  // Angola
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cv' WHERE dxcc='409'");  // Cape Verde
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='km' WHERE dxcc='411'");  // Comoros
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='de' WHERE dxcc='230'");  // Fed Rep Germany
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ph' WHERE dxcc='375'");  //Philippines
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='er' WHERE dxcc='51'");  // Eritrea
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ps' WHERE dxcc='510'");  // Palestine
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ck' WHERE dxcc='191'");  // North Cook
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ck' WHERE dxcc='234'");  // South Cook
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nu' WHERE dxcc='188'");  // Niue
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ba' WHERE dxcc='501'");  // Bosnia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='balear' WHERE dxcc='21'");  // Balearic is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='canary' WHERE dxcc='29'");  // Canary Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ceuta' WHERE dxcc='32'");  // Ceuta & Melilla //TODO: Fix the flag
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ie' WHERE dxcc='245'");  // Ireland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='am' WHERE dxcc='14'");  // Armenia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lr' WHERE dxcc='434'");  // Liberia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ir' WHERE dxcc='330'");  // Iran
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mv' WHERE dxcc='179'");  // Moldova
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ee' WHERE dxcc='52'");  //  Estonia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='et' WHERE dxcc='53'");  // Ethiopia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='by' WHERE dxcc='27'");  // Belarus
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kg' WHERE dxcc='135'");  // Kyrgyzstan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tm' WHERE dxcc='280'");  // Turkmenistan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tj' WHERE dxcc='262'");  // Turkmenistan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='227'");  // France
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='79'");  // Guadeloupe
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='yt' WHERE dxcc='169'");  // Mayotte
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='516'");  // St Barthelemy
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='36'");  // Clipperton Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nc' WHERE dxcc='162'");  // New Caledonia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nc' WHERE dxcc='512'");  // Chesterfield Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mq' WHERE dxcc='84'");  // Martinique
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pf' WHERE dxcc='175'");  // French Polynesia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pf' WHERE dxcc='508'");  // Austral Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pm' WHERE dxcc='277'");  //  St Pierre & Miquelon
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='re' WHERE dxcc='453'");  //Reunion Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='213'");  // St Marteen
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='99'");  // Glorioso is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='124'");  // Juan de nova, Europa
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='276'");  //  Tromelin - TODO: Add the wikipedia flag
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='41'");  // Crozet
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='131'");  // Kerguelen
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag http://es.wikipedia.org/wiki/Tierras_Australes_y_Ant%C3%A1rticas_Francesas

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='10'");  //Amsterdam & St Paul is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='wf' WHERE dxcc='298'");  // Wallis & Futuna is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gf' WHERE dxcc='63'");  // French Guiana
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='england' WHERE dxcc='223'");  // England
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='114'");  //Isle of Man
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='northernireland' WHERE dxcc='265'");  // Northern Ireland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='122'");  // Jersey
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='scotland' WHERE dxcc='279'");  //  Scotland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='scotland' WHERE dxcc='1279'");  //  Shetland is (Scotland)
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='106'");  // Guernsey
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='wales' WHERE dxcc='294'");  // Wales
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sb' WHERE dxcc='185'");  // Solomon
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sb' WHERE dxcc='507'");  // Temotu Province
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hu' WHERE dxcc='239'");  // Hungary
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ch' WHERE dxcc='287'");  // Switzerland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='li' WHERE dxcc='251'");   // Liechtenstein
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ec' WHERE dxcc='120'");  // Ecuador
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ec' WHERE dxcc='71'");  // Galapagos Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ht' WHERE dxcc='78'");  // Haiti
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='do' WHERE dxcc='72'");  // Dominican Rep
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='co' WHERE dxcc='116'");  // Colombia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='co' WHERE dxcc='216'");  // San Andres & Providencia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='co' WHERE dxcc='161'");  // Malpelo
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kr' WHERE dxcc='137'");  // Rep Korea
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pa' WHERE dxcc='88'");  // Panama
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hn' WHERE dxcc='80'");  // Honduras
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='th' WHERE dxcc='387'");  // Thailand
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='va' WHERE dxcc='295'");  // Vatican City
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sa' WHERE dxcc='378'");  // Saudi Arabia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='it' WHERE dxcc='248'");  // Italy
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='it' WHERE dxcc='1248'");  // African Italy
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='it' WHERE dxcc='2248'");  // Sicily
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='it' WHERE dxcc='225'");  // Sardinia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dj' WHERE dxcc='382'");  // Djibouti
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gd' WHERE dxcc='77'");  // Grenada
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gw' WHERE dxcc='109'");  // Guinea-Bissau
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lc' WHERE dxcc='97'");  // St Lucia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dm' WHERE dxcc='95'");  // Dominica
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vc' WHERE dxcc='98'");  // St Vicent
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jp' WHERE dxcc='339'");  // Japan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jp' WHERE dxcc='177'");  // Minami Torishima
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jp' WHERE dxcc='192'");  // Ogasawara
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mn' WHERE dxcc='363'");  // Mongolia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sj' WHERE dxcc='259'");  // Svalbard
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sj' WHERE dxcc='1259'");  // Svalbard
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sj' WHERE dxcc='118'");  // Jan Mayen
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jo' WHERE dxcc='342'");  // Jordan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='291'");  // United States
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='105'");  // Guantanamo Bay
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mp' WHERE dxcc='166'");  //  Mariana is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='20'");  // Baker & Howland is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gu' WHERE dxcc='103'");  // Guam
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='123'");  // Johnston is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='174'");  // Midway is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='197'");  // Palmyra & Jarbis is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='134'");  // Kingman Reef
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='110'");  // Hawaii
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='138'");  // Kure is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='as' WHERE dxcc='9'");  // American Samoa
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='as' WHERE dxcc='515'");  // Swains is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='297'");  // Wake is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='6'");  // Alaska
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='182'");  // Navassa Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vi' WHERE dxcc='285'");  // Us Virgin is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pr' WHERE dxcc='202'");  // Puerto Rico
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='43'");  // Desecheo Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='no' WHERE dxcc='266'");  // Norway
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ar' WHERE dxcc='100'");  // Argentina
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lu' WHERE dxcc='254'");  // Luxembourg
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lt' WHERE dxcc='146'");  // Lithuania
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bg' WHERE dxcc='212'");  // Bulgaria
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pe' WHERE dxcc='136'");  // Peru
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lb' WHERE dxcc='354'");  // Lebanon
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='at' WHERE dxcc='206'");  // Austria & Viena Intl HQ
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fi' WHERE dxcc='224'");  // Findland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fi' WHERE dxcc='5'");  // Aland is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fi' WHERE dxcc='167'");  // Market Reef
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cz' WHERE dxcc='503'");  // Czech Rep
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sk' WHERE dxcc='504'");  // Slovak Rep
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='be' WHERE dxcc='209'");  // Belgium
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gl' WHERE dxcc='237'");  // Greenland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dk' WHERE dxcc='222'");  // Faroe is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='dk' WHERE dxcc='221'");  // Denmark
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pg' WHERE dxcc='163'");  // Papua New Guinea
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='aw' WHERE dxcc='91'");  // Aruba
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kp' WHERE dxcc='344'");  //Dpr Korea
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nl' WHERE dxcc='263'");  // Netherlands
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cw' WHERE dxcc='517'");  // Curacao
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='bq' WHERE dxcc='520'");  // Bonaire
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='nl' WHERE dxcc='519'");  // Saba & St Eustatius
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='shm' WHERE dxcc='518'");  // Sint Marteen
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='108'");  // Brazil
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='56'");  // Fernando de Noronha
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='253'");  //  St Peter & St Paul
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='273'");  // Trindade & Martim Vaz
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sr' WHERE dxcc='140'");  // Suriname
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='61'");  // Franz Josef Land
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='eh' WHERE dxcc='302'");  // Western Sahara
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='bd' WHERE dxcc='305'");  // Bangladesh
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='si' WHERE dxcc='499'");  // Slovenia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sc' WHERE dxcc='379'");  // Seychelles
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='st' WHERE dxcc='219'");  // Sao Tome & Principe
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='se' WHERE dxcc='284'");  // Sweden
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pl' WHERE dxcc='269'");  // Poland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sd' WHERE dxcc='466'");  // Sudan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='eg' WHERE dxcc='478'");  // Egypt
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='236'");  // Greece
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='180'");  // Mount Athos
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='45'");  // Dodecanese
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='40'");  // Crete
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tv' WHERE dxcc='282'");  // Tuvalu
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='301'");  // Western Kiribati
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='31'");  // Central Kiribati
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='48'");  // Eastern Kiribati
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='490'");  // Banaba is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='so' WHERE dxcc='232'");  // Somalia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sm' WHERE dxcc='278'");  // San Marino
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pw' WHERE dxcc='22'");  // Palau
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tr' WHERE dxcc='390'");  // Turkey
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tr' WHERE dxcc='1390'");  // European Turkey
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='is' WHERE dxcc='242'");  // Iceland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gt' WHERE dxcc='76'");  // Guatemala
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='cr' WHERE dxcc='308'");  // Costa Rica
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cr' WHERE dxcc='37'");  // Coco is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cm' WHERE dxcc='406'");  // Cameroon
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='214'");  // Corsica
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}
    // TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='cf' WHERE dxcc='408'");  // Central African Rep
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cg' WHERE dxcc='412'");  // Rep of Congo
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='ga' WHERE dxcc='420'");  // Gabon
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='td' WHERE dxcc='410'");  // Chad
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ci' WHERE dxcc='428'");  // Cote d'Ivoire
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bj' WHERE dxcc='416'");  // Benin
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ml' WHERE dxcc='442'");  // Mali
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='54'");  // European Russia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='15'");  // Asiatic Russia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='126'");  // Kaliningrad
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='15'");  // Asiatic Rusia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='uz' WHERE dxcc='292'");  // Uzbekistan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kz' WHERE dxcc='130'");  // Kazakhstan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ua' WHERE dxcc='288'");  // Ukraine
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ag' WHERE dxcc='94'");  // Antigua & Barbuda
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bz' WHERE dxcc='66'");  // Belize
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kn' WHERE dxcc='249'");  // St Kitts & Nevis
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='na' WHERE dxcc='464'");  // Namibia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fm' WHERE dxcc='173'");  // Micronesia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fm' WHERE dxcc='168'");  // Marshall Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bn' WHERE dxcc='345'");  // Brunei Darusalam
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ca' WHERE dxcc='1'");  // Canada
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='150'");  // Australia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hm' WHERE dxcc='111'");  // Heard Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='153'");  // Macquarie is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cc' WHERE dxcc='38'");  //  Cocos / Keeling is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='147'");  //  Lord Howe is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='171'");  // Mellish Reed
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nf' WHERE dxcc='189'");  // Norkfolk is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='303'");  // Willis Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cx' WHERE dxcc='35'");  // Christmas is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ai' WHERE dxcc='12'");  //  Anguilla
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ms' WHERE dxcc='96'");  // Montserrat
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vg' WHERE dxcc='65'");  // British is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tc' WHERE dxcc='89'");  // Turks & Caicos is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pn' WHERE dxcc='172'");  // Pitcairn
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='513'");  // Ducie is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fk' WHERE dxcc='141'");  // Falkland is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gs' WHERE dxcc='235'");  // South Georgia is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='southbritish' WHERE dxcc='241'");  // South Shetland is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='238'");  // South Orkney is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gs' WHERE dxcc='240'");  // South Sandwich Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bm' WHERE dxcc='64'");  // Bermuda
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='io' WHERE dxcc='33'");  // Chagos is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hk' WHERE dxcc='321'");  // Hong Kong
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='in' WHERE dxcc='324'");  // India
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='in' WHERE dxcc='11'");  // Andaman & Nicobar
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='in' WHERE dxcc='142'");  // Lakshadweep Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mx' WHERE dxcc='50'");  // Mexico
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mx' WHERE dxcc='204'");  // Revilagigedo
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bf' WHERE dxcc='480'");  // Burkina Faso
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kh' WHERE dxcc='312'");  //  Cambodia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='la' WHERE dxcc='143'");  // Laos
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mo' WHERE dxcc='152'");  // Macao
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mm' WHERE dxcc='309'");  // Myanmar
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='af' WHERE dxcc='3'");  // Afganistan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='id' WHERE dxcc='327'");  // Indonesia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='iq' WHERE dxcc='333'");  // Iraq
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vu' WHERE dxcc='158'");  // Vanuatu
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sy' WHERE dxcc='384'");  // Syria
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lv' WHERE dxcc='145'");  // Latvia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ni' WHERE dxcc='86'");  //  Nicaragua
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ro' WHERE dxcc='275'");  // Romania
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sv' WHERE dxcc='74'");  // El Salvador
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='rs' WHERE dxcc='296'");  // Serbia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ve' WHERE dxcc='148'");  // Venezuela
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ve' WHERE dxcc='17'");  // Aves Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='zw' WHERE dxcc='452'");  // Zimbabwe
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mk' WHERE dxcc='502'");  // Macedonia
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ss' WHERE dxcc='521'");  //Rep South Sudan
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='al' WHERE dxcc='7'");  // Albania
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gi' WHERE dxcc='233'");  // Gibraltar
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='283'");  // UK Base Aereas Cyprus
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sh' WHERE dxcc='250'");  // St Helena
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sh' WHERE dxcc='205'");  // Ascension is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sh' WHERE dxcc='274'");  // Tristan da Cunha & Gough is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ky' WHERE dxcc='69'");  // Cayman Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tk' WHERE dxcc='270'");  // Tokelau Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='marquesas' WHERE dxcc='509'");  // Marquesas Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='170'");  // New Zeland
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='34'");  // Chatham Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='133'");  // Kermadec is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='16'");  // Auckland & Campbell is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='py' WHERE dxcc='132'");  // Paraguay
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='za' WHERE dxcc='462'");  // South Africa
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='za' WHERE dxcc='201'");  // Pr Edward & Marion Is
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='xk' WHERE dxcc='522'");  // Kosovo
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

// Countries without flag or controversial - Data is added just to keep the DB filled-up
    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='506'");  // Scarboroug Reef
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}    


    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='13'");  //  Antartica
    sqlOK = execQuery(Q_FUNC_INFO, sq);
    if (!sqlOK)
    {return false;}

      //qDebug() << "DataBase::updateTheEntityTableISONames-END" << endl;
    return true;
}

bool DataBase::isTheTableExisting(const QString _tableName)
{
    QSqlQuery query;
    int _n;
    QString sq = QString("SELECT name FROM sqlite_master WHERE name='%1'").arg(_tableName);
    bool sqlOK = query.exec(sq);
    if (sqlOK)
    {
        if (query.next())
        {
            if (query.isValid())
            {
                _n = (query.value(0)).toInt();
                query.finish();
                if (_n > 0)
                {
                      //qDebug() << "DataBase::isTheTableExisting - Table Exists"  << endl;
                    return true;
                }
                else
                {
                      //qDebug() << "DataBase::isTheTableExisting - Table does not Exist"  << endl;
                    return false;
                }
            }
        }
    }
    else
    {
        return false;
    }
    return false;
}



bool DataBase::hasTheTableData(const QString _tableName)
{
     //qDebug() << "DataBase::hasTheTableData" << _tableName << endl;
    QSqlQuery query;
    int _num = 0;

    QString stringQuery = QString("SELECT count(id) FROM %1").arg(_tableName);
    bool sqlOK = query.exec(stringQuery);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
              //qDebug() << "DataBase::hasTheTableData - valid"  << endl;
            _num = (query.value(0)).toInt();
            query.finish();
            if (_num > 0)
            {
                  //qDebug() << "DataBase::hasTheTableData - DB Exists"  << endl;
                return true;
            }
            else
            {
                  //qDebug() << "DataBase::hasTheTableData - DB does not Exist"  << endl;
                return false;
            }
        }
        else
        {
              //qDebug() << "DataBase::hasTheTableData - not valid"  << endl;
            query.finish();
            return false;
        }
    }
    else
    {
          //qDebug() << "DataBase::hasTheTableData: LastQuery: " << query.lastQuery()  << endl;
          //qDebug() << "DataBase::hasTheTableData: LastError-data: " << query.lastError().databaseText()  << endl;
          //qDebug() << "DataBase::hasTheTableData: LastError-driver: " << query.lastError().driverText()  << endl;
          //qDebug() << "DataBase::hasTheTableData LastError-n: " << QString::number(query.lastError().number() ) << endl;
          //qDebug() << "DataBase::updateTheEntityTableISONames" << endl;
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }

      //qDebug() << "DataBase::isTheDBCreated: END FALSE" << endl;
    query.finish();
    return false;
}

bool DataBase::updateTo011()
{// Updates the DB to 0.011:
    // We add FT8 mode and
    // AS Propagation


     //qDebug() << "DataBase::updateTo011: latestRead: " << getDBVersion() << endl;
    bool IAmIn011 = false;
    bool IAmIn010 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    //QSqlQuery query;

    //bool sqlOk = false;
    latestReaded = getDBVersion().toFloat();
     //qDebug() << "DataBase::updateTo011: Checking (latestRead/dbVersion):" << QString::number(latestReaded) << "/" << QString::number(dbVersion) << endl;
    if (latestReaded >= float(0.011))
    {
         //qDebug() << "DataBase::updateTo011: - I am in 011" << endl;
        IAmIn011 = true;
        return true;
    }
    else
    {
         //qDebug() << "DataBase::updateTo011: - I am not in 0.012 I am in: " << QString::number(latestReaded)<< endl;

        while (!IAmIn010 && !ErrorUpdating)
        {
             //qDebug() << "DataBase::updateTo011: - Check if I am in 010: !" << endl;
            IAmIn010 = updateTo010();

            if (IAmIn010)
            {                
                   //qDebug() << "DataBase::updateTo011: - updateTo010 returned TRUE - I am in 0.010: " << QString::number(latestReaded) << endl;
            }
            else
            {
                 //qDebug() << "DataBase::updateTo011: - updateTo009 returned FALSE - I am NOT in 0.010: " << QString::number(latestReaded) << endl;
                ErrorUpdating = false;
            }
        }

        if (ErrorUpdating)
        {
               //qDebug() << "DataBase::updateTo011: - I Could not update to: " << QString::number(dbVersion) << endl;
            return false;
        }

    }

    if (!recreateSatelliteData())
    {
         //qDebug() << "DataBase::updateTo011: - Sats update NOK " << endl;
        return false;
    }

    if (!recreateTableDXCC())
    {
         //qDebug() << "DataBase::updateTo011: - recreateTableDXCC NOK " << endl;
        return false;
    }

    if (!recreateTableWAZ())
    {
         //qDebug() << "DataBase::updateTo011: - recreateTableWAZ NOK " << endl;
        return false;
    }

    if(!execQuery(Q_FUNC_INFO, "INSERT INTO mode (submode, name, cabrillo, deprecated) VALUES ('MSK144', 'MSK144', 'NO', '0')"))
    {
         //qDebug() << "DataBase::updateTo011: - MSK NOK " << endl;
        return false;
    }


    if (!recreateTableLog())
    {
         //qDebug() << "DataBase::updateTo011: - Failed to recreate Table Log " << endl;
        return false;
    }

    if (updateDBVersion(softVersion, "0.011"))
    {
         //qDebug() << "DataBase::updateTo011: - We are in 011! " << endl;
        IAmIn011 = true;
    }
    else
    {
         //qDebug() << "DataBase::updateTo011: - Failed to go to 011! " << endl;
        IAmIn011 = false;
    }


    if (!updateAwardDXCCTable())
    {
       //qDebug() << "DataBase::updateTo011: - updateAwardDXCCTable NOK " << endl;
      IAmIn011 = false;
      return false;
    }
    if (!updateAwardWAZTable())
    {
         //qDebug() << "DataBase::updateTo011: - updateAwardWAZTable NOK " << endl;
        IAmIn011 = false;
    }

     //qDebug() << "DataBase::updateTo011: - END" << endl;
    return IAmIn011;
}


bool DataBase::updateTo012()
{
    // Updates the DB to 0.012:
    // Change the table qsl_via to qsl_via_enumeration
    // Solve the supportedContest table missing bug in some deployments


         //qDebug() << "DataBase::updateTo012: latestRead: " << getDBVersion() << endl;
        bool IAmIn012 = false;
        bool IAmIn011 = false;
        bool ErrorUpdating = false;
        latestReaded = getDBVersion().toFloat();
         //qDebug() << "DataBase::updateTo012: Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) << endl;
        if (latestReaded >= float(0.012))
        {
             //qDebug() << "DataBase::updateTo012: - I am in 012" << endl;
            IAmIn012 = true;
            return true;
        }
        else
        {
             //qDebug() << "DataBase::updateTo012: - I am not in 0.012 I am in: " << getDBVersion() << endl;
            while (!IAmIn011 && !ErrorUpdating)
            {
                 //qDebug() << "DataBase::updateTo012: - Check if I am in 011: !" << endl;
                IAmIn011 = updateTo011();

                if (IAmIn011)
                {
                       //qDebug() << "DataBase::updateTo012: - updateTo011 returned TRUE - I am in 0.011: " << QString::number(latestReaded) << endl;
                }
                else
                {
                      //qDebug() << "DataBase::updateTo012: - updateTo011 returned FALSE - I am NOT in 0.011: " << QString::number(latestReaded) << endl;
                    ErrorUpdating = false;
                }
            }
            if (ErrorUpdating)
            {
                  //qDebug() << "DataBase::updateTo012: - I Could not update to: " << QString::number(dbVersion) << endl;
                return false;
            }
        }

        // Now I am in the previous version and I can update the DB.
        bool sqlOK = false;

        if (isTheTableExisting("supportedcontest"))
        {
            sqlOK = execQuery(Q_FUNC_INFO, "DROP TABLE supportedcontest");
            if (sqlOK)
            {
                if (createTableSupportedContest())
                {
                    sqlOK = recreateSupportedContest();
                }
                else
                {
                     //qDebug() << "DataBase::updateTo012: UPDATED NOK-1!" << endl;
                   return false;
                }
            }
        }
        else
        {
            if (createTableSupportedContest())
            {
               if (!recreateSupportedContest())
               {
                    //qDebug() << "DataBase::updateTo012: UPDATED NOK-2!" << endl;
                   return false;
               }
            }
        }

        if (updateDBVersion(softVersion, "0.012"))
        {
             //qDebug() << "DataBase::updateTo012: - We are in 012! " << endl;
        }
        else
        {
             //qDebug() << "DataBase::updateTo011: - Failed to go to 012! " << endl;
            return false;
        }

         //qDebug() << "DataBase::updateTo012: UPDATED OK!" << endl;
        return true;

}

bool DataBase::updateTo013()
{
    // Updates the DB to 0.013:
    // Change the table qsl_via to qsl_via_enumeration
    // Solve the supportedContest table missing bug in some deployments


         //qDebug() << "DataBase::updateTo013: latestRead: " << getDBVersion() << endl;
        bool IAmIn013 = false;
        bool IAmIn012 = false;
        bool ErrorUpdating = false;
        latestReaded = getDBVersion().toFloat();
         //qDebug() << "DataBase::updateTo013: Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) << endl;
        if (latestReaded >= float(0.013))
        {
             //qDebug() << "DataBase::updateTo013: - I am in 013" << endl;
            IAmIn013 = true;
            return true;
        }
        else
        {
             //qDebug() << "DataBase::updateTo013: - I am not in 0.013 I am in: " << getDBVersion() << endl;
            while (!IAmIn012 && !ErrorUpdating)
            {
                 //qDebug() << "DataBase::updateTo013: - Check if I am in 012: !" << endl;
                IAmIn012 = updateTo012();

                if (IAmIn012)
                {
                       //qDebug() << "DataBase::updateTo013: - updateTo012 returned TRUE - I am in 0.012: " << QString::number(latestReaded) << endl;
                }
                else
                {
                      //qDebug() << "DataBase::updateTo013: - updateTo011 returned FALSE - I am NOT in 0.012: " << QString::number(latestReaded) << endl;
                    ErrorUpdating = false;
                }
            }
            if (ErrorUpdating)
            {
                  //qDebug() << "DataBase::updateTo013: - I Could not update to: " << QString::number(dbVersion) << endl;
                return false;
            }
        }

        // Now I am in the previous version and I can update the DB.

        if (!recreateSupportedContest())
        {
            return false;
        }

         execQuery(Q_FUNC_INFO, "DROP TABLE IF exists qsl_via");
         if (createTableQSL_Via_enumeration())
         {
             if (populateTableQSL_Via_enumeration())
             {

             }
             else
             {
                 return false;
             }
         }
         else
         {
             return false;
         }


        if (updateDBVersion(softVersion, "0.013"))
        {
             //qDebug() << "DataBase::updateTo013: - We are in 013! " << endl;
        }
        else
        {
             //qDebug() << "DataBase::updateTo013: - Failed to go to 013! " << endl;
            return false;
        }

         //qDebug() << "DataBase::updateTo013: UPDATED OK!" << endl;
        return true;

}


bool DataBase::updateTo014()
{
    // Updates the DB to 0.014:
    // Updates the Satellite DB



         //qDebug() << "DataBase::updateto014: latestRead: " << getDBVersion() << endl;
        bool IAmIn014 = false;
        bool IAmIn013 = false;
        bool ErrorUpdating = false;
        latestReaded = getDBVersion().toFloat();
         //qDebug() << "DataBase::updateto014: Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) << endl;
        if (latestReaded >= float(0.04))
        {
             //qDebug() << "DataBase::updateto014: - I am in 013" << endl;
            IAmIn014 = true;
            return true;
        }
        else
        {
             //qDebug() << "DataBase::updateto014: - I am not in 0.013 I am in: " << getDBVersion() << endl;
            while (!IAmIn013 && !ErrorUpdating)
            {
                 //qDebug() << "DataBase::updateto014: - Check if I am in 013: !" << endl;
                IAmIn013 = updateTo013();

                if (IAmIn013)
                {
                       //qDebug() << "DataBase::updateto014: - updateTo012 returned TRUE - I am in 0.013: " << QString::number(latestReaded) << endl;
                }
                else
                {
                      //qDebug() << "DataBase::updateto014: - updateTo011 returned FALSE - I am NOT in 0.013: " << QString::number(latestReaded) << endl;
                    ErrorUpdating = false;
                }
            }
            if (ErrorUpdating)
            {
                  //qDebug() << "DataBase::updateto014: - I Could not update to: " << QString::number(dbVersion) << endl;
                return false;
            }
        }

        // Now I am in the previous version and I can update the DB.


        if (!recreateSatelliteData())
        {
             //qDebug() << "DataBase::updateTo014: - Sats update NOK " << endl;
            return false;
        }


        if (updateDBVersion(softVersion, "0.014"))
        {
             //qDebug() << "DataBase::updateto014: - We are in 013! " << endl;
        }
        else
        {
             //qDebug() << "DataBase::updateto014: - Failed to go to 013! " << endl;
            return false;
        }

         //qDebug() << "DataBase::updateTo014: UPDATED OK!" << endl;
        return true;

}


bool DataBase::updateAwardDXCCTable()
{
     //qDebug() << "DataBase::updateAwardDXCCTable" << endl;

    QList<AwarddxccEntry> dxccStatusList;
    //QList<AwarddxccEntryCheck> dxccStatusListCheck;
    dxccStatusList.clear();
    //dxccStatusListCheck.clear();

    AwarddxccEntry awardEntry;
    awardEntry.dxcc = QString();
    awardEntry.band = QString();
    awardEntry.status = QString();
    awardEntry.logNumber = QString();
    awardEntry.qsoID = QString();

    //AwarddxccEntryCheck awardEntryCheck;
    //awardEntryCheck.dxcc = QString();
    //awardEntryCheck.band = QString();
    //awardEntryCheck.status = QString();


    QString stringQuery = QString("SELECT id, bandid, modeid, dxcc, qsl_rcvd, lognumber FROM log ORDER BY dxcc");
    QSqlQuery query;//, query2;

    bool sqlOK = query.exec(stringQuery);
    QSqlRecord rec = query.record();
    if (!sqlOK)
    {

        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    else
    {
         //qDebug() << "DataBase::updateAwardDXCCTable SELECT when OK" << endl;
    }
    QStringList dxccStatus = QStringList(); //dxcc, band, mode, confirmed, lognumber, qsoid (per award set)
    QStringList dxccStatusCheck = QStringList(); //dxcc, band, mode, confirmed, lognumber (per award set) just to check
    int nameCol = -1;

    QString _aux = QString();

     //qDebug() << "DataBase::updateAwardDXCCTable before the while" << endl;
    while (query.next())
    {
         //qDebug() << "DataBase::updateAwardDXCCTable IN the while" << endl;
        if (query.isValid())
        {
             //qDebug() << "DataBase::updateAwardDXCCTable VALID" << endl;
            awardEntry.dxcc.clear();
            awardEntry.band.clear();
            awardEntry.status.clear();
            awardEntry.logNumber.clear();
            awardEntry.qsoID.clear();

             //qDebug() << "DataBase::updateAwardDXCCTable in the while" << endl;
            nameCol = rec.indexOf("qsl_rcvd");

            if ((query.value(nameCol)).toString() == "Y")
            {
              awardEntry.status = "1";
            }
            else
            {
                awardEntry.status = "0";
            }
             //qDebug() << "DataBase::updateAwardDXCCTable - status" << awardEntry.status << endl;
            if ((awardEntry.status == "1") || (awardEntry.status == "0") )
            {

                nameCol = rec.indexOf("dxcc");
                awardEntry.dxcc = (query.value(nameCol)).toString();

                if ((awardEntry.dxcc).toInt()>0)
                {
                    nameCol = rec.indexOf("bandid");
                    awardEntry.band = (query.value(nameCol)).toString();

                    nameCol = rec.indexOf("modeid");
                    awardEntry.mode = (query.value(nameCol)).toString();

                    nameCol = rec.indexOf("id");
                    awardEntry.qsoID = (query.value(nameCol)).toString();

                    nameCol = rec.indexOf("lognumber");
                    awardEntry.logNumber = (query.value(nameCol)).toString();

                     //qDebug() << "DataBase::updateAwardDXCCTable: Adding: " << awardEntry.dxcc <<"/" << awardEntry.band <<"/" << awardEntry.mode <<"/" << awardEntry.status <<"/"  << awardEntry.logNumber <<"/" << awardEntry.qsoID << endl;
                    dxccStatusList.append(awardEntry);

                }
            } // END OF IF VALID
        }
    } // END OF  WHILE

     //qDebug() << "DataBase::updateAwardDXCCTable - END OF WHILE" << endl;

    query.finish();


     //qDebug() << "DataBase::updateAwardDXCCTable: Log analized... let's clean the table!" << endl;

    stringQuery = QString("DELETE FROM awarddxcc");

    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {return false;}
    else
    {
         //qDebug() << "DataBase::updateAwardDXCCTable: awarddxcc table DELETED" << endl;
    }


     //qDebug() << "DataBase::updateAwardDXCCTable: Now we start writing the table!!" << endl;

    //int i = 0;
    _aux.clear();

    int qsos = dxccStatusList.length();
    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating DXCC award information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

     //qDebug() << "DataBase::updateAwardDXCCTable: INSERTING: " << QString::number(qsos) << " QSOS..." << endl;

    for (int j=0;j<dxccStatusList.length();j++)
    {

        stringQuery = QString("INSERT INTO awarddxcc (dxcc, band, mode, confirmed, lognumber, qsoid) VALUES ('%1', '%2', '%3', '%4', '%5', '%6') ").arg(dxccStatusList.at(j).dxcc).arg(dxccStatusList.at(j).band).arg(dxccStatusList.at(j).mode).arg(dxccStatusList.at(j).status).arg(dxccStatusList.at(j).logNumber).arg(dxccStatusList.at(j).qsoID);
        //sqlOK = query.exec(Q_FUNC_INFO, stringQuery);
        sqlOK = query.exec(stringQuery);
        if (!sqlOK)
        {

             //qDebug() << "DataBase::updateAwardDXCCTable: Error: " << QString::number(query.lastError().number()) << endl;
            if (query.lastError().number() == 19)
            { // DUPLICATED RECORD: Means that there is already a record in the award... so this set is worked. QSL can be Y or N in the award but inthe log may be other options
              // We should only take into account if N or Y
                if (dxccStatusList.at(j).status!="1")
                { // If tne new status is not confirmed, no change. DO NOTHING
                     //qDebug() << "DataBase::updateAwardDXCCTable: Duplicated but DO NOTHING as new status is not Confirmed!!!" << endl;
                }
                else
                {
                     //qDebug() << "DataBase::updateAwardDXCCTable: Duplicated but NOW is confirmed!!!" << endl;
                    stringQuery = QString("SELECT confirmed, lognumber, qsoid FROM awarddxcc WHERE dxcc='%1' AND band='%2' AND mode='%3'").arg(dxccStatusList.at(j).dxcc).arg(dxccStatusList.at(j).band).arg(dxccStatusList.at(j).mode);
                    QSqlQuery query2;//, query2;

                    sqlOK = query2.exec(stringQuery);
                    QSqlRecord rec = query2.record();
                    if (!sqlOK)
                    {
                        queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().number(), query2.lastQuery());
                        query2.finish();
                        return false;
                    }
                    else
                    {
                        query2.next();
                        if (query2.isValid())
                        {
                            nameCol = rec.indexOf("confirmed");
                            stringQuery = (query2.value(nameCol)).toString();
                            QString _qsoid = QString();
                            nameCol = rec.indexOf("qsoid");
                            _qsoid = (query2.value(nameCol)).toString();

                            if ((stringQuery == "0") && (dxccStatusList.at(j).status == "1"))
                            {
                                query2.finish();
                                stringQuery = QString ("UPDATE awarddxcc SET confirmed = '1', qsoid = '%1' WHERE qsoid='%2'").arg(dxccStatusList.at(j).qsoID).arg(_qsoid);
                                if (execQuery(Q_FUNC_INFO, stringQuery))
                                {

                                }
                                else
                                {
                                     //qDebug() << "DataBase::updateAwardDXCCTable: Duplicated but UPDATE IS NOT DONE" << endl;
                                }

                            }
                            else
                            {
                                 //qDebug() << "DataBase::updateAwardDXCCTable: Duplicated but UPDATE NOT NEEDED" << endl;
                            }
                        }
                        else
                        {
                             //qDebug() << "DataBase::updateAwardDXCCTable: Duplicated SELECT query is not Valid" << endl;
                        }
                    }
                }

                 //qDebug() << "DataBase::updateAwardDXCCTable: Duplicated!" << endl;
            }
            else
            {
                queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                query.finish();
                return false;
            }
        }
        else
        {
             //qDebug() << "DataBase::updateAwardDXCCTable: INSERT OK: " << endl;
        }

        query.finish();
         //qDebug() << "DataBase::updateAwardDXCCTable: Checking steps " << endl;
        if (( (j % step )== 0) )
        { // To update the speed I will only show the progress once each X QSOs
            _aux = QObject::tr("Updating DXCC Award information...") + "\n" + QObject::tr("QSO: ")  + QString::number(j) + "/" + QString::number(qsos);
            progress.setLabelText(_aux);
            progress.setValue(j);
        }
        if ( progress.wasCanceled() )
        {
                //qDebug() << "DataBase::updateAwardDXCCTable: progress canceled" << endl;
            return true;
        }
    }

    progress.setValue(qsos);
     //qDebug() << "DataBase::updateAwardDXCCTable: LAST END OK " << endl;
    return true;
}

bool DataBase::updateAwardWAZTable()
{    
     //qDebug() << "DataBase::updateAwardWAZTable" << endl;
    QList<AwarddxccEntry> dxccStatusList;
    //QList<AwarddxccEntryCheck> dxccStatusListCheck;
    dxccStatusList.clear();
    //dxccStatusListCheck.clear();

    AwarddxccEntry awardEntry;
    awardEntry.dxcc = QString();
    awardEntry.band = QString();
    awardEntry.status = QString();
    awardEntry.logNumber = QString();
    awardEntry.qsoID = QString();

    QString stringQuery = QString("SELECT id, bandid, modeid, cqz, qsl_rcvd, lognumber FROM log ORDER BY cqz");
    QSqlQuery query;//, query2;

    bool sqlOK = query.exec(stringQuery);
    QSqlRecord rec = query.record();
    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
    else
    {
         //qDebug() << "DataBase::updateAwardWAZTable SELECT when OK" << endl;
    }
    QStringList dxccStatus = QStringList(); //cqz, band, mode, confirmed, lognumber, qsoid (per award set)
    QStringList dxccStatusCheck = QStringList(); //cqz, band, mode, confirmed, lognumber (per award set) just to check
    int nameCol = -1;

    QString _aux = QString();

     //qDebug() << "DataBase::updateAwardWAZTable before the while" << endl;
    while (query.next())
    {
         //qDebug() << "DataBase::updateAwardWAZTable IN the while" << endl;
        if (query.isValid())
        {
             //qDebug() << "DataBase::updateAwardWAZTable VALID" << endl;
            awardEntry.dxcc.clear();
            awardEntry.band.clear();
            awardEntry.status.clear();
            awardEntry.logNumber.clear();
            awardEntry.qsoID.clear();

             //qDebug() << "DataBase::updateAwardWAZTable in the while" << endl;
            nameCol = rec.indexOf("qsl_rcvd");

            if ((query.value(nameCol)).toString() == "Y")
            {
              awardEntry.status = "1";
            }
            else
            {
                awardEntry.status = "0";
            }
             //qDebug() << "DataBase::updateAwardWAZTable - status" << awardEntry.status << endl;
            if ((awardEntry.status == "1") || (awardEntry.status == "0") )
            {

                nameCol = rec.indexOf("cqz");
                awardEntry.dxcc = (query.value(nameCol)).toString();

                if ((awardEntry.dxcc).toInt()>0)
                {
                    nameCol = rec.indexOf("bandid");
                    awardEntry.band = (query.value(nameCol)).toString();

                    nameCol = rec.indexOf("modeid");
                    awardEntry.mode = (query.value(nameCol)).toString();

                    nameCol = rec.indexOf("id");
                    awardEntry.qsoID = (query.value(nameCol)).toString();

                    nameCol = rec.indexOf("lognumber");
                    awardEntry.logNumber = (query.value(nameCol)).toString();

                     //qDebug() << "DataBase::updateAwardWAZTable: Adding: " << awardEntry.dxcc <<"/" << awardEntry.band <<"/" << awardEntry.mode <<"/" << awardEntry.status <<"/"  << awardEntry.logNumber <<"/" << awardEntry.qsoID << endl;
                    dxccStatusList.append(awardEntry);

                }
            } // END OF IF VALID
        }
    } // END OF  WHILE

     //qDebug() << "DataBase::updateAwardWAZTable - END OF WHILE" << endl;

    query.finish();


     //qDebug() << "DataBase::updateAwardWAZTable: Log analized... let's clean the table!" << endl;

    stringQuery = QString("DELETE FROM awardwaz");

    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {return false;}
    else
    {
         //qDebug() << "DataBase::updateAwardWAZTable: awardwaz table DELETED" << endl;
    }


     //qDebug() << "DataBase::updateAwardWAZTable: Now we start writing the table!!" << endl;

    //int i = 0;
    _aux.clear();

    int qsos = dxccStatusList.length();
    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating WAZ award information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

     //qDebug() << "DataBase::updateAwardWAZTable: INSERTING: " << QString::number(qsos) << " QSOS..." << endl;

    for (int j=0;j<dxccStatusList.length();j++)
    {

        stringQuery = QString("INSERT INTO awardwaz (cqz, band, mode, confirmed, lognumber, qsoid) VALUES ('%1', '%2', '%3', '%4', '%5', '%6') ").arg(dxccStatusList.at(j).dxcc).arg(dxccStatusList.at(j).band).arg(dxccStatusList.at(j).mode).arg(dxccStatusList.at(j).status).arg(dxccStatusList.at(j).logNumber).arg(dxccStatusList.at(j).qsoID);
        //sqlOK = query.exec(Q_FUNC_INFO, stringQuery);
        sqlOK = query.exec(stringQuery);
        if (!sqlOK)
        {

             //qDebug() << "DataBase::updateAwardWAZTable: Error: " << QString::number(query.lastError().number()) << endl;
            if (query.lastError().number() == 19)
            { // DUPLICATED RECORD: Means that there is already a record in the award... so this set is worked. QSL can be Y or N in the award but inthe log may be other options
              // We should only take into account if N or Y
                if (dxccStatusList.at(j).status!="1")
                { // If tne new status is not confirmed, no change. DO NOTHING
                     //qDebug() << "DataBase::updateAwardWAZTable: Duplicated but DO NOTHING as new status is not Confirmed!!!" << endl;
                }
                else
                {
                     //qDebug() << "DataBase::updateAwardWAZTable: Duplicated but NOW is confirmed!!!" << endl;
                    stringQuery = QString("SELECT confirmed, lognumber, qsoid FROM awardwaz WHERE cqz='%1' AND band='%2' AND mode='%3'").arg(dxccStatusList.at(j).dxcc).arg(dxccStatusList.at(j).band).arg(dxccStatusList.at(j).mode);
                    QSqlQuery query2;//, query2;

                    sqlOK = query2.exec(stringQuery);
                    QSqlRecord rec = query2.record();
                    if (!sqlOK)
                    {
                        queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().number(), query2.lastQuery());
                        query2.finish();
                        return false;
                    }
                    else
                    {
                        query2.next();
                        if (query2.isValid())
                        {
                            nameCol = rec.indexOf("confirmed");
                            stringQuery = (query2.value(nameCol)).toString();
                            QString _qsoid = QString();
                            nameCol = rec.indexOf("qsoid");
                            _qsoid = (query2.value(nameCol)).toString();

                            if ((stringQuery == "0") && (dxccStatusList.at(j).status == "1"))
                            {
                                query2.finish();
                                stringQuery = QString ("UPDATE awardwaz SET confirmed = '1', qsoid = '%1' WHERE qsoid='%2'").arg(dxccStatusList.at(j).qsoID).arg(_qsoid);
                                if (execQuery(Q_FUNC_INFO, stringQuery))
                                {

                                }
                                else
                                {
                                     //qDebug() << "DataBase::updateAwardWAZTable: Duplicated but UPDATE IS NOT DONE" << endl;
                                }

                            }
                            else
                            {
                                 //qDebug() << "DataBase::updateAwardWAZTable: Duplicated but UPDATE NOT NEEDED" << endl;
                            }
                        }
                        else
                        {
                             //qDebug() << "DataBase::updateAwardWAZTable: Duplicated SELECT query is not Valid" << endl;
                        }
                    }
                }

                 //qDebug() << "DataBase::updateAwardWAZTable: Duplicated!" << endl;
            }
            else
            {
                queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                query.finish();
                return false;
            }
        }
        else
        {
             //qDebug() << "DataBase::updateAwardWAZTable: INSERT OK: " << endl;
        }

        query.finish();
         //qDebug() << "DataBase::updateAwardWAZTable: Checking steps " << endl;
        if (( (j % step )== 0) )
        { // To update the speed I will only show the progress once each X QSOs
            _aux = QObject::tr("Updating WAZ Award information...") + "\n" + QObject::tr("QSO: ")  + QString::number(j) + "/" + QString::number(qsos);
            progress.setLabelText(_aux);
            progress.setValue(j);
        }
        if ( progress.wasCanceled() )
        {
                //qDebug() << "DataBase::updateAwardWAZTable: progress canceled" << endl;
            return true;
        }
    }

    progress.setValue(qsos);
     //qDebug() << "DataBase::updateAwardWAZTable: LAST END OK " << endl;
    return true;



}


int DataBase::getNumberOfQsos(const int _logNumber)
{
    QString stringQuery = QString();
    if (_logNumber<1)
    {
        stringQuery = QString("SELECT COUNT (*) FROM log");
    }
    else
    {
        stringQuery = QString("SELECT COUNT (*) FROM log WHERE lognumber='%1'").arg(_logNumber);
    }
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    int qsos = 0;

    if (sqlOK)
    {
        //QSqlDatabase::database().commit();
        query.next();
        qsos = (query.value(0)).toInt();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());

    }
    query.finish();
    return qsos;
}

void DataBase::queryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery)
{
     //qDebug() << "DataBase::queryErrorManagement: constrid - " << QString::number(constrid) << endl;
     //qDebug() << "DataBase::queryErrorManagement: Function: " << functionFailed << endl;
     //qDebug() << "DataBase::queryErrorManagement: Error N#: " << QString::number(errorCodeN) << endl;
     //qDebug() << "DataBase::queryErrorManagement: Error: " << functionFailed << errorCodeS << endl;
     //qDebug() << "DataBase::queryErrorManagement: Query failed: " << failedQuery << endl;
}

 bool DataBase::beginTransaction()
 {
      //qDebug() << "DataBase::beginTransaction: " << endl;
     QSqlDatabase db = QSqlDatabase::database();
     db.setDatabaseName(dbName);
     return execQuery(Q_FUNC_INFO, "BEGIN IMMEDIATE TRANSACTION");
 }


 bool DataBase::commitTransaction()
 {
      //qDebug() << "DataBase::commitTransaction: " << endl;
     return db.commit();
 }


 bool DataBase::execQuery(const QString function, const QString stringQuery)
 {
     //qDebug() << "DataBase::execQuery: " << function << " : " << stringQuery << endl;
    QSqlQuery query;



    if (query.exec(stringQuery))
    {
        while (query.isActive())
        {
             //qDebug() << "DataBase::execQuery: Still active... " << endl;
            query.finish();
        }
         //qDebug() << "DataBase::execQuery: No longer active... " << endl;
        return true;
    }
    else
    {        

        queryErrorManagement(function, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        query.finish();
        return false;
    }
 }

