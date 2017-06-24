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


DataBase::DataBase(const QString _softVersion){
  //qDebug() << "DataBase::DataBase: " << _softVersion  << endl;
    //TODO: Sometimes the DB is created without the proper calling (without passing softVersion)
    dbVersion = DBVersionf;
    softVersion = _softVersion;
    //inMemoryOnly = inmemoryonly;
    latestReaded = 0.0;
    util = new Utilities();
    //db = new QSqlDatabase;
    db = QSqlDatabase::database();
    //created = false;
    if (softVersion>0)
    {
        createConnection();
    }

   //qDebug() << "DataBase::DataBase: END"  << endl;

}



DataBase::~DataBase()
{
    //qDebug() << "DataBase::~DataBase"  << endl;
}

void DataBase::compress()
{
    //qDebug() << "DataBase::compress " << endl;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
    }
   else
    {
        db.exec("VACUUM;");
    }

}

bool DataBase::createConnection()
{
 //qDebug() << "DataBase::createConnection: " << QString::number(dbVersion) << "/" << softVersion << endl;
    QString stringQuery;
   //qDebug() << "DataBase::createConnection: 0" << endl;

    QSqlQuery query;

    //rc = sqlite3_open(":memory:", &db);

    if (!db.isOpen())
    {
       //qDebug() << "DataBase::createConnection: DB NOT Opened" << endl;
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("logbook.dat");

        if (!db.open())
        {
            QMessageBox::warning(0, QObject::tr("Database Error"),
                                 db.lastError().text());
           //qDebug() << "DataBase::createConnection: DB creation ERROR"  << endl;
            return false;
        }
       else
       {
           //qDebug() << "DataBase::createConnection: created?" << endl;

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
       //qDebug() << "DataBase::createConnection: DB already opened"  << endl;
    }
    createBandModeMaps(); //TODO: I have commented out thi line because createBandModeMaps is also called from isThe


    //created = true;
 //qDebug() << "DataBase::createConnection -------------------------------------------- END" << endl;
    return unMarkAllQSO();
}

bool DataBase::isTheDBCreated()
{
  //qDebug() << "DataBase::isTheDBCreated"  << endl;
    //return created;
    //return hasTheTableData("softwarecontrol");

    QSqlQuery query;
    int _num = 0;


    QString stringQuery ("SELECT count(id) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);
    //En esta ejecución da driver not loaded

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
           //qDebug() << "DataBase::isTheDBCreated - valid"  << endl;
            _num = (query.value(0)).toInt();
            if (_num > 0)
            {
               //qDebug() << "DataBase::isTheDBCreated - DB Exists"  << endl;
               //qDebugzw22222() << "DataBase::isTheDBCreated: ------------------------------------------------- END TRUE" << endl;
                return true;
            }
            else
            {
               //qDebug() << "DataBase::isTheDBCreated - DB does not Exist"  << endl;
               //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END FALSE-1" << endl;
                return false;
            }
        }
        else
        {
           //qDebug() << "DataBase::isTheDBCreated - not valid"  << endl;
           //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END FALSE-2" << endl;

            return false;
        }
    }
    else
    {
       //qDebug() << "DataBase::isTheDBCreated: LastQuery: " << query.lastQuery()  << endl;
       //qDebug() << "DataBase::isTheDBCreated: LastError-data: " << query.lastError().databaseText()  << endl;
       //qDebug() << "DataBase::isTheDBCreated: LastError-driver: " << query.lastError().driverText()  << endl;
       //qDebug() << "DataBase::isTheDBCreated: LastError-n: " << QString::number(query.lastError().number() ) << endl;
       //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END FALSE-3" << endl;
        return false;
    }

  //qDebug() << "DataBase::isTheDBCreated: ------------------------------------------------- END FALSE-X" << endl;
    return false;

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
             "cqz INTEGER,"
             "ituz INTEGER,"
             "dxcc INTEGER,"
             "address VARCHAR, "
             "age INTEGER, "
             "cnty VARCHAR, "
             "comment VARCHAR, "
             "a_index INTEGER, "
             "ant_az INTEGER, "
             "ant_el INTEGER, "
             "ant_path INTEGER, "
             "arrl_sect INTEGER, "
             "band_rx INTEGER, "
             "checkcontest VARCHAR, "
             "class VARCHAR, "
             "contacted_op VARCHAR(40), "
             "contest_id VARCHAR, "
             "country VARCHAR, "
             "credit_submitted VARCHAR, "
             "credit_granted VARCHAR, "
             "distance INTEGER, "
             "email VARCHAR, "
             "eq_call VARCHAR, "
             "eqsl_qslrdate VARCHAR(10), "
             "eqsl_qslsdate VARCHAR(10), "
             "eqsl_qsl_rcvd VARCHAR(1), "
             "eqsl_qsl_sent VARCHAR(1), "
             "force_init INTEGER, "
             "freq VARCHAR, "
             "freq_rx VARCHAR, "
             "gridsquare VARCHAR, "
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
             "my_city VARCHAR, "
             "my_cnty VARCHAR, "
             "my_country INTEGER, "
             "my_cq_zone INTEGER, "
             "my_gridsquare VARCHAR, "
             "my_iota VARCHAR(6), "
             "my_iota_island_id VARCHAR, "
             "my_lat VARCHAR(11), "
             "my_lon VARCHAR(11), "
             "my_name VARCHAR, "
             "my_rig VARCHAR, "
             "my_sig VARCHAR, "
             "my_sig_info VARCHAR, "
             "my_state VARCHAR, "
             "my_street VARCHAR, "
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
             "rx_pwr REAL, "
             "sat_mode VARCHAR, "
             "sat_name VARCHAR, "
             "sfi INTEGER, "
             "sig VARCHAR, "
             "sig_info VARCHAR, "
             "srx_string VARCHAR, "
             "stx_string VARCHAR, "
             "state VARCHAR, "
             "station_callsign VARCHAR, "
             "swl INTEGER, "
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
             "FOREIGN KEY (dxcc) REFERENCES entity, "
             "FOREIGN KEY (bandid) REFERENCES band)");

    //qDebug() << "DataBase::createTableLog: " << stringQuery  << endl;
    return query.exec(stringQuery);

}

bool DataBase::createDataBase()
{
   //qDebug() << "DataBase::createDataBase ------------------------------------- START"  << endl;
    //bool qres;
       //http://www.sqlite.org/
    //http://www.sqlite.org/datatype3.html
    //qDebug() << "DataBase::createData"  << endl;
    //int softDB = dbVersion;
    //QString softV = _softVersion;
    QString dateString;

    QSqlQuery query;
    query.exec("DROP TABLE log");
    query.exec("DROP TABLE band");
    query.exec("DROP TABLE mode");
    //query.exec("DROP TABLE mode");
    query.exec("DROP TABLE prefixesofentity");
    query.exec("DROP TABLE continent");
    query.exec("DROP TABLE entity");


    query.exec("CREATE TABLE softwarecontrol ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "dateupgrade VARCHAR(10) NOT NULL, "
                "softversion REAL NOT NULL, "
                "dbversion REAL NOT NULL)");

    dateString = (date.currentDateTime()).toString("yyyyMMdd");

    query.exec("INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + softVersion + "', '" + QString::number(dbVersion) + "')");



/*
     if (qres)
    {
        //qDebug() << "DataBase::createDataBase - Band created"  << endl;
    }
    else
    {
        //qDebug() << "DataBase::createDataBase - Band not created"  << endl;
        //qDebug() << "DataBase::createDataBase: " << query.lastError().text() << endl;
    }
*/
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


      query.exec("CREATE TABLE continent ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(2) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");

      query.exec("CREATE TABLE ant_path_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");

      query.exec("CREATE TABLE arrl_sect_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(2) NOT NULL, "
                 "name VARCHAR(30) NOT NULL)");


      query.exec("CREATE TABLE qso_complete_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(3) NOT NULL, "
                 "name VARCHAR(10) NOT NULL)");


      createTableContest();

      query.exec("CREATE TABLE contestcategory ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(20) NOT NULL, "
                 "name VARCHAR(40) NOT NULL");

      query.exec("CREATE TABLE award_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "name VARCHAR(15) NOT NULL)");


      query.exec("CREATE TABLE prefixesofentity ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "prefix VARCHAR(15) NOT NULL,"
                 "dxcc INTEGER NOT NULL,"
                 "cqz INTEGER NOT NULL,"
                 "ituz INTEGER NOT NULL,"
                 "UNIQUE (prefix, dxcc), "
                 "FOREIGN KEY (dxcc) REFERENCES entity)");



      query.exec("CREATE TABLE awarddxcc ("
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




      query.exec("CREATE TABLE awardwaz ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "cqz INTEGER NOT NULL,"
                 "band INTEGER NOT NULL, "
                 "mode INTEGER NOT NULL, "
                 "confirmed INTEGER, "
                 "qsoid INTEGER NOT NULL, "
                 "lognumber INTEGER, "
                 "UNIQUE (cqz, band, mode, confirmed, lognumber), "
                 "FOREIGN KEY (band) REFERENCES band, "
                 "FOREIGN KEY (mode) REFERENCES mode, "
                 "FOREIGN KEY (qsoid) REFERENCES log)");
      /*
      In awardwaz confirmed means:
      confirmed = 0     Set as Worked
      confirmed = 1     Set as Confirmed
      */

      query.exec("CREATE TABLE qsl_rec_status ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");

      query.exec("CREATE TABLE qsl_sent_status ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");


      query.exec("CREATE TABLE qsl_via ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");

      createTablePropModes();
      createTableLogs(true);
      createTableClubLogStatus();
      populateTableClubLogStatus();

      /*
      query.exec("CREATE TABLE sat_modes ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "name VARCHAR(15) NOT NULL,"
                 "uplink INTEGER NOT NULL,"
                 "downlink INTEGER NOT NULL,"
                 "FOREIGN KEY (uplink) REFERENCES band, "
                 "FOREIGN KEY (downlink) REFERENCES band)");
    //http://en.wikipedia.org/wiki/OSCAR#Mode_designators

      //query.exec("INSERT INTO sat_modes (name, uplink, downlink) VALUES ('H', '15M', '148', '144')");


      query.exec("CREATE TABLE sat_modes ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(8) NOT NULL, "
                 "uplink VARCHAR(8) NOT NULL, "
                 "downlink VARCHAR(55) NOT NULL)");

      query.exec("CREATE TABLE sat_modes ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "name VARCHAR(15) NOT NULL,"
                 "uplink INTEGER NOT NULL,"
                 "downlink INTEGER NOT NULL,"
                 "FOREIGN KEY (uplink) REFERENCES band, "
                 "FOREIGN KEY (downlink) REFERENCES band)");
*/
/*
      qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (:id, :firstname, :lastname)" );
       qry.bindValue( ":id", 9 );
       qry.bindValue( ":firstname", "Ralph" );
       qry.bindValue( ":lastname", "Roe" );

       query.prepare("INSERT INTO qsl_sent_status (shortname, name) VALUES (:shortname, :name)");
       query.bindValue(":shortname", "Y");
       query.bindValue(":name", "Yes");

       query.prepare("INSERT INTO qsl_sent_status (shortname, name) VALUES (:shortname, :name)");
       query.bindValue(":shortname", "N");
       query.bindValue(":name", "No");

       query.exec();
*/

    query.exec("INSERT INTO qsl_sent_status (shortname, name) VALUES ('Y', 'Yes')");
    query.exec("INSERT INTO qsl_sent_status (shortname, name) VALUES ('N', 'No')");
    query.exec("INSERT INTO qsl_sent_status (shortname, name) VALUES ('R', 'Requested')");
    query.exec("INSERT INTO qsl_sent_status (shortname, name) VALUES ('Q', 'Queued')");
    query.exec("INSERT INTO qsl_sent_status (shortname, name) VALUES ('I', 'Ignore/Invalid')");


    query.exec("INSERT INTO qsl_rec_status (shortname, name) VALUES ('Y', 'Yes')");
    query.exec("INSERT INTO qsl_rec_status (shortname, name) VALUES ('N', 'No')");
    query.exec("INSERT INTO qsl_rec_status (shortname, name) VALUES ('R', 'Requested')");
    query.exec("INSERT INTO qsl_rec_status (shortname, name) VALUES ('I', 'Ignore/Invalid')");
    query.exec("INSERT INTO qsl_rec_status (shortname, name) VALUES ('V', 'Validated')");


      query.exec("INSERT INTO continent (shortname, name) VALUES ('AF', 'Africa')");
      query.exec("INSERT INTO continent (shortname, name) VALUES ('AS', 'Asia')");
      query.exec("INSERT INTO continent (shortname, name) VALUES ('EU', 'Europe')");
      query.exec("INSERT INTO continent (shortname, name) VALUES ('NA', 'North America')");
      query.exec("INSERT INTO continent (shortname, name) VALUES ('OC', 'Oceania')");
      query.exec("INSERT INTO continent (shortname, name) VALUES ('SA', 'South America')");
      query.exec("INSERT INTO continent (shortname, name) VALUES ('AN', 'Antartica')");

      populateContestData();
      populatePropagationModes();


query.exec("INSERT INTO ant_path_enumeration (shortname, name) VALUES ('G', 'GrayLine')");
query.exec("INSERT INTO ant_path_enumeration (shortname, name) VALUES ('O', 'Other')");
query.exec("INSERT INTO ant_path_enumeration (shortname, name) VALUES ('S', 'ShortPath')");
query.exec("INSERT INTO ant_path_enumeration (shortname, name) VALUES ('L', 'LongPath')");

query.exec("INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AL', 'Alabama')");


query.exec("INSERT INTO award_enumeration (name) VALUES ('AJA')");
query.exec("INSERT INTO award_enumeration (name) VALUES ('CQDX')");
query.exec("INSERT INTO award_enumeration (name) VALUES (CQDXFIELD')");
query.exec("INSERT INTO award_enumeration (name) VALUES (DXCC')");
query.exec("INSERT INTO award_enumeration (name) VALUES (TPEA')");


query.exec("INSERT INTO qsl_via (shortname, name) VALUES ('B', 'Bureau')");
query.exec("INSERT INTO qsl_via (shortname, name) VALUES ('D', 'Direct')");
query.exec("INSERT INTO qsl_via (shortname, name) VALUES ('E', 'Electronic')");
query.exec("INSERT INTO qsl_via (shortname, name) VALUES ('M', 'Manager')");

query.exec("INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('Y', 'Yes')");
query.exec("INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('N', 'No')");
query.exec("INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('NIL', 'Not heard')");
query.exec("INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('?', 'Uncertain')");

    //qDebug() << "DataBase::createDataBase ------------------------------------- END"  << endl;
return true;


}

int DataBase::getBandIdFromName(const QString b)
{
    //qDebug() << "DataBase::getBandIdFromName: " << b << endl;
    QSqlQuery query;
    if (isValidBand(b))
    {
        QString queryString = QString("SELECT id FROM band WHERE name='%1'").arg(b);

        query.exec(queryString);
        query.next();
        if ( query.isValid() )
        {
            //qDebug() << "DataBase::getBandIdFromName: OK" << QString::number((query.value(0)).toInt()) << endl;
            return (query.value(0)).toInt();

        }
        else
        {
            //qDebug() << "DataBase::getBandIdFromName: NOK 1" << endl;
            return -1;
        }
    }
    else
    {
        //qDebug() << "DataBase::getBandIdFromName: NOK 2" << endl;
        return -1;
    }
    //qDebug() << "DataBase::getBandIdFromName: NOK 3" << endl;
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
        query.exec(queryString);
        query.next();
        if ( query.isValid() )
        {
           //qDebug() << "DataBase::getModeIdFromName: OK" << QString::number((query.value(0)).toInt()) << endl;
            return (query.value(0)).toInt();

        }
        else
        {
           //qDebug() << "DataBase::getModeIdFromName: NOK 1" << endl;
            return -1;
        }
    }
    else
    {
       //qDebug() << "DataBase::getModeIdFromName: NOK 2" << endl;
        return -1;
    }
   //qDebug() << "DataBase::getModeIdFromName: NOK 3" << endl;
    return -1;
}


int DataBase::getModeIdFromSubMode(const QString b, const bool _tmp)
{
   //qDebug() << "DataBase::getModeIdFromSubMode: " << b << endl;
     QSqlQuery query;
     QString queryString;
     if (_tmp)
     {
         queryString = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(b);
     }
     else
     {
         queryString = QString("SELECT id FROM mode WHERE submode='%1'").arg(b);
     }

     if (isValidMode(b, _tmp))
     {
         //queryString = QString("SELECT id FROM mode WHERE name='%1'").arg(b);
        //qDebug() << "DataBase::getModeIdFromName: queryString: " << queryString << endl;
         query.exec(queryString);
         query.next();
         if ( query.isValid() )
         {
           //qDebug() << "DataBase::getModeIdFromName: OK" << QString::number((query.value(0)).toInt()) << "-------- END" << endl;
             return (query.value(0)).toInt();

         }
         else
         {
           //qDebug() << "DataBase::getModeIdFromName: NOK 1" << "-------- END"<< endl;
             return -1;
         }
     }
     else
     {
       //qDebug() << "DataBase::getModeIdFromName: NOK 2" << "-------- END"<< endl;
         return -1;
     }
   //qDebug() << "DataBase::getModeIdFromName: NOK 3" << "-------- END"<< endl;
     return -1;
}

QString DataBase::getBandNameFromNumber(const int _n)
{
   //qDebug() << "DataBase::getBandNameFromNumber: " << QString::number(_n) << endl;
    QSqlQuery query;
    QString queryString = QString("SELECT name FROM band WHERE id='%1'").arg(_n);

    query.exec(queryString);
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
            return "";
        }
    }
    else
    {
       //qDebug() << "DataBase::getBandNameFromNumber: " << "-------- END-2" << endl;
        return "";
    }
}



QString DataBase::getModeNameFromNumber(const int _n, const bool _tmp)
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


    query.exec(queryString);
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
            return "";
        }
        */
    }
    else
    {
        //qDebug() << "DataBase::getModeNameFromNumber - Not Valid record"  << endl;
       //qDebug() << "DataBase::getModeNameFromNumber: ------ END-2" << endl;
        return "";
    }
}

QString DataBase::getSubModeNameFromNumber(const int _n, const bool _tmp)
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
                    return "";
                }
            }
            else
            {
               //qDebug() << "DataBase::getSubModeNameFromNumber: query not valid - END" << endl;
                return "";
            }

        }
        else
        {
           //qDebug() << "DataBase::getSubModeNameFromNumber: query not next - END" << endl;
            return "";
        }


    }
    else
    {
       //qDebug() << "DataBase::getSubModeNameFromNumber: SQL FALSE - END" << endl;
        return "";
    }
   //qDebug() << "DataBase::getSubModeNameFromNumber: - END-X" << endl;
    return "";
}

bool DataBase::isValidBand (const QString b)
{
   //qDebug() << "DataBase::isValidBand: " << b << endl;
    if (b.length()<1)
    {
        return false;
    }
    QString stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    return query.isValid();
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

    stringQuery = QString("SELECT id FROM mode WHERE submode='%1'").arg(b);
    QSqlQuery query(stringQuery);
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
     bool sqlOk = false;
    QString queryString = QString("SELECT id FROM band WHERE lower <= '%1' and upper >= '%2'").arg(fr).arg(fr);

    QSqlQuery query(queryString);
    sqlOk = query.exec();
    //qDebug() << "DataBase::getBandIdFromFreq: Query: " << query.lastQuery() << endl;
    if (sqlOk)
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
            return -1;
        }
    }
    else
    {
       //qDebug() << "DataBase::getBandIdFromFreq: Query NOK" << endl;
       return -1;
    }
   //qDebug() << "DataBase::getBandIdFromFreq: END-X" << endl;
    return -1;
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
    QSqlQuery query(stringQuery);
    //qDebug() << "MainWindow::slotQSLSentViaBureauFromLog: " << stringQuery << endl;
    //query.exec(stringQuery);
    //TODO: Check if the execution of this query is OK or NOK (should return false)
   //qDebug() << "DataBase::unMarkAllQSO: END "  << endl;
    return true;
}

bool DataBase::updateIfNeeded()
{
   //qDebug() << "DataBase::updateIfNeeded - Version: " << QString::number(dbVersion) << endl;

    /**************************************************************************************
     * This function should call to bool updateToXXX () being XXX dbVersion and
     *
     */

    float aux = 0.0;

    int nameCol;
    //int errorCode = -1;
    bool toBeUpdated = false;
    //bool sqlOK;
    QString dateString;
    QString SQLString, auxs;
    QStringList SQLStrings;
    SQLStrings.clear();

    QSqlQuery query("SELECT dbversion FROM softwarecontrol");
    QSqlRecord rec = query.record();

    while ( (query.next())) { // We run the DB to find what is the latest DB version updated.
        if (query.isValid())
        {
            nameCol = rec.indexOf("dbversion");
            aux = (query.value(nameCol)).toFloat();

            //qDebug() << "DataBase::updateIfNeeded - Version found: " << QString::number(aux) << endl;
            if (aux > latestReaded)
            {
                latestReaded = aux;
            }
            else
            {
            }
        }
        else
        {
        }
    }

    if (latestReaded >= dbVersion)
    { // DB is updated, no update is needed
        //qDebug() << "DataBase::updateIfNeeded - DB updated (no need to update anything!) " << endl;
        toBeUpdated = false;
       //qDebug() << "DataBase::updateIfNeeded - TRUE - END "  << endl;
        return true;
    }
    else
    { // DB is outdated. We need to update!!
        //qDebug() << "DataBase::updateIfNeeded - DB outdated... upgrade starts now! " << endl;
        QMessageBox msgBox;
        msgBox.setText( QObject::tr("KLog DB needs to be upgraded."));
        msgBox.setInformativeText( QObject::tr("Do you want to upgrade it now?\nIf DB is not upgraded KLog may not work properly."));
        msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Apply);

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
    QString fr = 0;
    bandIDHash.clear();
    IDBandHash.clear();
    QSqlQuery query("SELECT id, name, lower FROM band");
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
           return false;
           //TODO: Manage this error, in case the query is NOK.

        }
         //qDebug() << "DataBase::createTheBandQuickReference: Go for the next one!" << endl;
    }
/*
    QHashIterator<QString, int> i(bandIDHash);
    while (i.hasNext()) {
        i.next();
        //qDebug() << i.key() << ": " << QString::number(i.value()) << endl;
    }
*/
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
        QString st = "NULL";
        int in = 0;
        modeIDHash.clear();
        IDModeHash.clear();
        QSqlQuery query("SELECT id, submode FROM mode");
        while (query.next())
        {

            if (query.isValid())
            {
                st = (query.value(1)).toString();
                in = (query.value(0)).toInt();
                modeIDHash.insert(st, in );
                IDModeHash.insert(in, st);
                //qDebug() << "DataBase::createTheModeQuickReference: " << st <<"/" << QString::number(in)<< endl;
            }
            else
            {
              //qDebug() << "DataBase::createTheModeQuickReference: Query not valid - END" << endl;

               //QMessageBox::warning(0, QObject::tr("Database Error (DataBase::createTheModeQuickReference)"),
               //                     query.lastError().text());
               return false;
               //TODO: Manage this error, in case the query is NOK.

            }
        }
/*
        QHashIterator<QString, int> i(modeIDHash);
        while (i.hasNext()) {
            i.next();
            //qDebug() << i.key() << ": " << QString::number(i.value()) << endl;
        }

*/
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
    //return getModeIdFromName(b);

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

QString DataBase::getBandNameFromID2(const int _i)
{
   //qDebug() << "DataBase::getBandNameFromid2: " << QString::number(_i) << endl;
    //return getBandNameFromNumber(_i);

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
    //return getSubModeNameFromNumber(_i);

    if (IDModeHash.contains(_i))
    {
       //qDebug() << "DataBase::getModeNameFromId2: END OK" << endl;
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

bool DataBase::createBandModeMaps()
{
   //qDebug() << "DataBase::createBandModeMaps" << endl;
     //bool b = createTheBandQuickReference();
     //bool m = createTheModeQuickReference();

     //return (b && m);
    if (isTheDBCreated())
    {
      //qDebug() << "DataBase::createBandModeMaps - isDbCreated TRUE" << endl;
        return (createTheBandQuickReference() &&  createTheModeQuickReference());
    }
    else
    {
      //qDebug() << "DataBase::createBandModeMaps - isDbCreated FALSE" << endl;
        return false;
    }

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

bool DataBase::updateToLatest()
{
/*
 * With the DB updates, the function that is called from here should be also updated.
 * The updateXXX are recursive calls that calls the previous one.
 *
 */
   //qDebug() << "DataBase::updateToLatest " << endl;
    return updateTo009();
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
   //qDebug() << "DataBase::updateTo003" << endl;
    bool IAmIn003 = false;
    bool IAmIn002 = false;
    bool ErrorUpdating = false;

    if (latestReaded >= 0.003)
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

   //qDebug() << "DataBase::updateTo004" << endl;
    bool IAmIn004 = false;
    bool IAmIn003 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    QSqlQuery query;

    bool sqlOk = false;

    if (latestReaded >= 0.004)
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
        sqlOk = query.exec("INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + softVersion + "', '" + QString::number(dbVersion) + "')");
        if (sqlOk)
        { // Version updated
            sqlOk = query.exec("DROP TABLE award_enumeration");
        }
        else
        { // Version not updated

        }
        //DO ALL THE TASKS TO BE IN 0.004 from 0.003 HERE and set ErrorUpdating if it is not possible.
        IAmIn004 = true;
    }
    return IAmIn004;
}


bool DataBase::updateTo005()
{// Updates the DB to 0.0.5

      //qDebug() << "DataBase::updateTo005" << endl;
       bool IAmIn005 = false;
       bool IAmIn004 = false;
       bool ErrorUpdating = false;
       QString stringQuery = QString();
       QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
       QSqlQuery query;
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       int errorCode;

       bool sqlOk = false;

       if (latestReaded >= 0.005)
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
           sqlOk = query.exec("INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + softVersion + "', '" + QString::number(dbVersion) + "')");
           if (sqlOk)
           { // Version updated
               if (recreateContestData())
               {
                   //qDebug() << "DataBase::updateTo005 - recreateContestData OK" << endl;
                   sqlOk = query.exec ("DROP table logs");
                   sqlOk = createTableLogs(true);
               if (!sqlOk)
                   {
                       //qDebug() << "DataBase::updateTo005 - logs table do not created" << endl;
                   }


                   if (howManyQSOsInLog(0)>0)
                   { // If the user has QSOs that were added with previous versions...
                     // We need to create a new log and rename all QSOs to that QSO.
                     //stringQuery = QString("UPDATE log SET lognumber='1' WHERE lognumber='0'");


                       msgBox.setText(QObject::tr("KLog has detected a previous log in the DB. All data will be migrated to a newly created DX type log for you."));
                       msgBox.exec();

                        if (query.exec("UPDATE log SET lognumber='1' WHERE lognumber='0'"))
                        {}
                        else
                        {
                            //showError(QObject::tr("QSOs not updated to main log"));
                            //qDebug() << "DataBase::updateTo005 - QSOs not updated to main log" << endl;
                            errorCode = query.lastError().number();
                            //qDebug() << "DataBase::updateTo005 - query error: " << QString::number(errorCode) << endl;
                            //qDebug() << "DataBase::updateTo005: LastQuery: " << query.lastQuery()  << endl;
                            //qDebug() << "DataBase::updateTo005: LastError-data: " << query.lastError().databaseText()  << endl;
                            //qDebug() << "DataBase::updateTo005: LastError-driver: " << query.lastError().driverText()  << endl;
                            //qDebug() << "DataBase::updateTo005: LastError-n: " << QString::number(query.lastError().number() ) << endl;
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
                        if (query.exec(stringQuery))
                        {

                        }
                        else
                        {
                            //showError(QObject::tr("New Log not created"));
                            //qDebug() << "DataBase::updateTo005 - New Log not created" << endl;
                            //qDebug() << "DataProxy_SQLite::clearLog: Log deleted FAILED" << endl;
                            errorCode = query.lastError().number();
                            //qDebug() << "DataBase::updateTo005a - query error: " << QString::number(errorCode) << endl;
                            //qDebug() << "DataBase::updateTo005a: LastQuery: " << query.lastQuery()  << endl;
                            //qDebug() << "DataBase::updateTo005a: LastError-data: " << query.lastError().databaseText()  << endl;
                            //qDebug() << "DataBase::updateTo005a: LastError-driver: " << query.lastError().driverText()  << endl;
                            //qDebug() << "DataBase::updateTo005a: LastError-n: " << QString::number(query.lastError().number() ) << endl;

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
            msgBox.setText(QObject::tr("All the data was migrated correctly. You should now to go to Setup->Preferences->Logs to check that everything is OK."));
            msgBox.exec();
       }
        //qDebug() << "DataBase::updateTo005 - I am in 005 already!! " << endl;
       return IAmIn005;
}

bool DataBase::recreateContestData()
{
   //qDebug() << "DataBase::recreateContestData"  << endl;
    QSqlQuery query;
    bool sqlOk = false;
    sqlOk = query.exec("DROP TABLE contest");
    if (sqlOk)
    {
        if (createTableContest())
        {
            return populateContestData();
        }
    }
    return false;

}
/*
bool DataBase::updateLog()
{
    //qDebug() << "DataBase::updateLog"  << endl;
    QSqlQuery query;
    bool sqlOk = false;
    return sqlOk;
}


bool DataBase::createTableLogs()
{
    createTable

    QSqlQuery query;
    return query.exec("CREATE TABLE logs ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "logdate VARCHAR(10), "
               "stationcall VARCHAR(15) NOT NULL, "
               "comment VARCHAR, "
               "logtype VARCHAR, "
               "logtypen INTEGER, "
               "FOREIGN KEY (logtypen) REFERENCES supportedcontests(id),"

                      "FOREIGN KEY (logtype) REFERENCES supportedcontests(name))");

}
*/

bool DataBase::createTableLogs(const bool real)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
    //Creating the Sats DB to be able to include satellites to the LOTW

   //qDebug() << "DataBase::createTableLogs" << endl;

    QString stringQuery = QString();
    QSqlQuery query;
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
        return  query.exec(stringQuery);
}


bool DataBase::createTablePropModes()
{
   //qDebug() << "DataBase::createTablePropModes" << endl;
    QSqlQuery query;
    return query.exec("CREATE TABLE prop_mode_enumeration (id INTEGER PRIMARY KEY AUTOINCREMENT, shortname VARCHAR(8), name VARCHAR(55) )");
}


bool DataBase::createTableContest()
{
   //qDebug() << "DataBase::createTableContest" << endl;
    QSqlQuery query;

    query.exec("CREATE TABLE contest ("
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

    query.exec("CREATE TABLE supportedcontests ("
                   "id INTEGER PRIMARY KEY, "
                   "longname VARCHAR,"
                   "name VARCHAR)");

    query.exec("CREATE TABLE contestcatoperator ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    query.exec("CREATE TABLE contestcatassisted ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    query.exec("CREATE TABLE contestcatpower ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    query.exec("CREATE TABLE contestcatband ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    query.exec("CREATE TABLE contestcatoverlay ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");
    
    query.exec("CREATE TABLE contestcatmode ("
                   "id INTEGER PRIMARY KEY, "
                   "name VARCHAR)");

    // ADDING ALL THE CATEGORIES OPTIONS

    query.exec("INSERT INTO supportedcontests (id, longname, name) VALUES ('0', 'Normal log', 'DX')");

   // query.exec("INSERT INTO supportedcontests (id, longname, name) VALUES ('1', 'CQ WW DX Contest(SSB)', 'CQ-WW-SSB')");

    query.exec("INSERT INTO contestcatoperator (id, name) VALUES ('0', 'N/A')");
    query.exec("INSERT INTO contestcatoperator (id, name) VALUES ('1', 'Single-Operator')");
    query.exec("INSERT INTO contestcatoperator (id, name) VALUES ('2', 'Multi-One')");
    query.exec("INSERT INTO contestcatoperator (id, name) VALUES ('3', 'Multi-Two')");
    query.exec("INSERT INTO contestcatoperator (id, name) VALUES ('4', 'Multi-Unlimited')");
    query.exec("INSERT INTO contestcatoperator (id, name) VALUES ('5', 'CheckLog')");

    query.exec("INSERT INTO contestcatassisted (id, name) VALUES ('0', 'N/A')");
    query.exec("INSERT INTO contestcatassisted (id, name) VALUES ('1', 'Non-Assisted')");
    query.exec("INSERT INTO contestcatassisted (id, name) VALUES ('2', 'Assisted')");

    query.exec("INSERT INTO contestcatpower (id, name) VALUES ('0', 'N/A')");
    query.exec("INSERT INTO contestcatpower (id, name) VALUES ('1', 'High-Power')");
    query.exec("INSERT INTO contestcatpower (id, name) VALUES ('2', 'Low-Power')");
    query.exec("INSERT INTO contestcatpower (id, name) VALUES ('3', 'QRP')");

    query.exec("INSERT INTO contestcatband (id, name) VALUES ('0', 'N/A')");
    query.exec("INSERT INTO contestcatband (id, name) VALUES ('1', 'All-Band')");
    query.exec("INSERT INTO contestcatband (id, name) VALUES ('2', 'Single-Band')");

    query.exec("INSERT INTO contestcatoverlay (id, name) VALUES ('0', 'N/A')");
    query.exec("INSERT INTO contestcatoverlay (id, name) VALUES ('1', 'Classic')");
    query.exec("INSERT INTO contestcatoverlay (id, name) VALUES ('2', 'Rookie')");

    query.exec("INSERT INTO contestcatmode (id, name) VALUES ('0', 'N/A')");
    query.exec("INSERT INTO contestcatmode (id, name) VALUES ('1', 'SSB')");
    query.exec("INSERT INTO contestcatmode (id, name) VALUES ('2', 'CW')");
    query.exec("INSERT INTO contestcatmode (id, name) VALUES ('3', 'MIXED')");

    //qDebug() << "DataBase::createTableContest END" << endl;
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
        return  query.exec(stringQuery);

}

bool DataBase::populateTableMode(const bool NoTmp)
{
   //qDebug() << "DataBase::populateTableMode" << endl;
    QSqlQuery query;
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


    //query.exec("INSERT INTO mode (submode, name, cabrillo, deprecated) VALUES ('AM', 'AM', 'PH', '0')");
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('AM', 'AM', 'PH', '0')").arg(tableName));

    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('AMTORFEC', 'TOR', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ASCI', 'RTTY', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ATV', 'ATV', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP', 'CHIP', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP64', 'CHIP', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP128', 'CHIP', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CLO', 'CLO', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CONTESTI', 'CONTESTI', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CW', 'CW', 'CW', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DIGITALVOICE', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DSTAR', 'DSTAR', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINO', 'DOMINO', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINOEX', 'DOMINO', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINOF', 'DOMINO', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FAX', 'FAX', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FM', 'FM', 'PH', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FMHELL', 'HELL', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSK31', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSK441', 'FSK441', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSKHELL', 'HELL', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('GTOR', 'TOR', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELL', 'HELL', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELL80', 'HELL', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HFSK', 'HELL', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT', 'ISCAT', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT-A', 'ISCAT', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT-B', 'ISCAT', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4', 'JT4', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4A', 'JT4', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4B', 'JT4', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4C', 'JT4', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4D', 'JT4', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4E', 'JT4', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4F', 'JT4', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4G', 'JT4', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT6M', 'JT6M', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9', 'JT9', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-1', 'JT9', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-2', 'JT9', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-5', 'JT9', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-10', 'JT9', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-30', 'JT9', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT44', 'JT44', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65', 'JT65', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65A', 'JT65', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65B', 'JT65', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65B2', 'JT65', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65C', 'JT65', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65C2', 'JT65', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK4', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK8', 'MFSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK11', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK16', 'MFSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK22', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK31', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK32', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK64', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK128', 'MFSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MSK144', 'MSK144', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MT63', 'MT63', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 4/125', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 4/250', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 8/250', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 8/500', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 16/500', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 16/1000', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 32/1000', 'OLIVIA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA', 'OPERA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA-BEACON', 'OPERA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA-QSO', 'OPERA', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC', 'PAC', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC2', 'PAC', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC3', 'PAC', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC4', 'PAC', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAX', 'PAX', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAX2', 'PAX', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PCW', 'CW', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PKT', 'PKT', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK', 'PSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK10', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK31', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK63', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK63F', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK125', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK250', 'PSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK500', 'PSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK1000', 'PSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKAM10', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKAM31', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKAM50', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKFEC31', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK2K', 'PSK2K', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKHELL', 'HELL', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('Q15', 'Q15', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK31', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK63', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK125', 'PSK', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK250', 'PSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QPSK500', 'PSK', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS', 'ROS', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-EME', 'ROS', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-HF', 'ROS', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-MF', 'ROS', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('RTTY', 'RTTY', 'RY', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('RTTYM', 'RTTYM', 'RY', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SSB', 'SSB', 'PH', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('LSB', 'SSB', 'PH', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('USB', 'SSB', 'PH', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SSTV', 'SSTV', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRB', 'THRB', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRBX', 'THRB', 'NO', '1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR', 'THOR', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('TOR', 'TOR', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('V4', 'V4', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('VOI', 'VOI', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('WINMOR', 'WINMOR', 'NO', '0')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('WSPR', 'WSPR', 'NO', '0')").arg(tableName));
    createTheModeQuickReference();
   //qDebug() << "DataBase::populateTableMode END" << endl;
    return true;
}


bool DataBase::createTableSatellites(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
    //Creating the Sats DB to be able to include satellites to the LOTW

   //qDebug() << "DataBase::createTableSatellites" << endl;

    QString stringQuery = QString();
    QSqlQuery query;
    if (NoTmp)
    {
        stringQuery = "CREATE TABLE satellites" ;
    }
    else
    {
        stringQuery = "CREATE TABLE satellitestemp" ;
    }

        stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                            "satarrlid VARCHAR, "
                                            "satname VARCHAR, "
                                            "satmode VARCHAR, "
                                            "UNIQUE (satarrlid, satmode) )");



    //qDebug() << "DataBase::createTableSatellites END" << endl;
        return  query.exec(stringQuery);
}

bool DataBase::populateTableSatellites(const bool NoTmp)
{
    // Data must come from:
    // https://lotw.arrl.org/lotw-help/frequently-asked-questions/#sats
   //qDebug() << "DataBase::populateTableSatellites" << endl;

    QSqlQuery query;
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


    //To add a band, just create another line:
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-10', 'AMSAT-OSCAR 10')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-13', 'AMSAT-OSCAR 13')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-16', 'AMSAT-OSCAR 16')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-21', 'OSCAR 21/RS-14')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-24', 'Arsene-OSCAR 24')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-27', 'AMRAD-OSCAR 27')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-3', 'AMSAT-OSCAR 3')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-4', 'AMSAT-OSCAR 4')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-40', 'AMSAT-OSCAR 40')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-51', 'AMSAT-OSCAR 51')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-6', 'AMSAT-OSCAR 6')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-7', 'AMSAT-OSCAR 7')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-73', 'AMSAT-OSCAR 73')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-8', 'AMSAT-OSCAR 8')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('AO-85', 'AMSAT-OSCAR 85 (Fox-1A)')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('ARISS', 'ARISS')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('CAS-3H', 'LilacSat 2')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('DO-64', 'Delfi OSCAR-64')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('FO-12', 'Fuji-OSCAR 12')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('FO-20', 'Fuji-OSCAR 20')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('FO-29', 'Fuji-OSCAR 29')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('HO-68', 'Hope OSCAR 68')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('LO-19', 'Lusat-OSCAR 19')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('LO-78', 'LituanicaSAT-1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('NO-44', 'Navy-OSCAR 44')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-1', 'Radio Sputnik 1')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-10', 'Radio Sputnik 10')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-11', 'Radio Sputnik 11')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-12', 'Radio Sputnik 12')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-13', 'Radio Sputnik 13')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-15', 'Radio Sputnik 15')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-2', 'Radio Sputnik 2')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-5', 'Radio Sputnik 5')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-6', 'Radio Sputnik 6')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-7', 'Radio Sputnik 7')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('RS-8', 'Radio Sputnik 8')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('SO-35', 'Sunsat-OSCAR 35')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('SO-41', 'Saudi-OSCAR 41')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('SO-50', 'Saudi-OSCAR 50')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('SO-67', 'Sumbandila OSCAR 67')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('UO-14', 'UOSAT-OSCAR 14')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('VO-52', 'VUsat-OSCAR 52')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('XW-2A', 'Hope 2A')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('XW-2B', 'Hope 2B')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('XW-2C', 'Hope 2C')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('XW-2D', 'Hope 2D')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('XW-2E', 'Hope 2E')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (satarrlid, satname) VALUES ('XW-2F', 'Hope 2F')").arg(tableName));
   //qDebug() << "DataBase::populateTableSatellites - END" << endl;
    return true;
}

bool DataBase::createTableEntity(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
   //qDebug() << "DataBase::createTableEntity" << endl;

    QString stringQuery = QString();
    QSqlQuery query;
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
    return  query.exec(stringQuery);

    //TODO: To add some columns in this the table to mark if worked/confirmed/band/Mode
    //query.exec("INSERT INTO entity (name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate) VALUES ('Canada', '0', '0', '0', '0', '0', '0', '0', 'VE', 'q', 'sincedate', 'todate')");
}


bool DataBase::createTableBand(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards

   //qDebug() << "DataBase::createTableBand" << endl;
    QString stringQuery = QString();
    QSqlQuery query;
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
    return  query.exec(stringQuery);
}



bool DataBase::populateTableBand(const bool NoTmp)
{
    // Cabrillo definition: http://wwrof.org/cabrillo/cabrillo-specification-v3/

   //qDebug() << "DataBase::populateTableBand" << endl;

    QSqlQuery query;
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
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('0', '0', '0', 'Light')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('1mm', '241000', '250000', '241G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2mm', '142000', '149000', '142G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2.5mm', '119980', '120020', '119G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('4mm', '75500', '81000', '75G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('6mm', '47000', '47200', '47G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('1.25CM', '24000', '24250', '24G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('3CM', '10000', '10500', '10G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('6CM', '5650', '5925', '5.7G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('9CM', '3300', '3500', '3.4G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('13CM', '2340', '2450', '2.3G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('23CM', '1240', '1300', '1.2G')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('33CM', '902', '928', '902')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('70CM', '420', '450', '432')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('1.25M', '222', '225', '222')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2M', '144', '148', '144')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('4M', '70', '71', '4M')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('6M', '50', '54', '50')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('10M', '28.0', '29.7', '28000')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('12M', '24.89', '24.99', '24000')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('15M', '21.0', '21.45', '21000')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('17M', '18.068', '18.168', '18100')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('20M', '14.0', '14.35', '14000')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('30M', '10.0', '10.15', '10000')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('40M', '7.0', '7.3', '7000')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('60M', '5.102', '5.404', '5100')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('80M', '3.5', '4.0', '3500')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('160M', '1.8', '2.0', '1800')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('560M', '0.501', '0.504', '560M')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('630M', '0.472', '0.479', '630M')").arg(tableName));
    query.exec(QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2190M', '0.136', '0.137', '2190M')").arg(tableName));
    createTheBandQuickReference();

   //qDebug() << "DataBase::populateTableBand END" << endl;
    return true;
}


bool DataBase::populatePropagationModes()
{
   //qDebug() << "DataBase::populatePropagationModes" << endl;
    QSqlQuery query;

    //query.exec("INSERT INTO prop_mode_enumeration (id, shortname, name) VALUES ('0', Not', 'Not Identified')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('AUR', 'Aurora')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('AUE', 'Aurora-E')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('BS', 'Back scatter')");

    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('ECH', 'EchoLink')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('EME', 'Earth-Moon-Earth')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('ES', 'Sporadic E')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('FAI', 'Field Aligned Irregularities')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('F2', 'F2 Reflection')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('INTERNET', 'Internet-assisted')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('ION', 'Ionoscatter')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('IRL', 'IRLP')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('MS', 'Meteor scatter')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('RPT', 'Terrestrial or atmospheric repeater or transponder')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('RS', 'Rain scatter')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('SAT', 'Satellite')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('TEP', 'Trans-equatorial')");
    query.exec("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('TR', 'Tropospheric ducting')");

   //qDebug() << "DataBase::populatePropagationModes END" << endl;
    return true;
}


bool DataBase::populateContestData()
{
   //qDebug() << "DataBase::populateContestData" << endl;
    QSqlQuery query;

    
    // CONTEST DEFINITIONS START HERE

    // DX
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (0, 0, 0, 0, 0, 0, 0)");
    // DX START
/*
    // CQ WW DX SSB START
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 2, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 2, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 2, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 2, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 2, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 2, 0, 1)");


    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 1, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 2, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 1, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 2, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 1, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 2, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 1, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 2, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 1, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 2, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 1, 1, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 2, 1, 1)");

    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 1, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 1, 2, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 1, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 2, 2, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 1, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 1, 3, 2, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 1, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 1, 2, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 1, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 2, 2, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 1, 2, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 1, 2, 3, 2, 2, 1)");

    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 2, 0, 1, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 2, 0, 2, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 3, 0, 1, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 3, 0, 2, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 4, 0, 1, 1, 0, 1)");
    query.exec("INSERT INTO contest (contest, catoperator, catassisted, catpower, catband, catoverlay, catmode) VALUES (1, 5, 0, 0, 0, 0, 1)");
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

    if (query.exec(sqlQueryString))
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
            return -1;
        }
    }
    else
    {
       //qDebug() << "DataBase::howManyQSOsInLog END-2" << endl;
        return -1;
    }
}

bool DataBase::updateTo006()
{// Updates the DB to 0.0.6

    //qDebug() << "DataBase::updateTo006" << endl;
    bool IAmIn006 = false;
    bool IAmIn005 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    QSqlQuery query;

    bool sqlOk = false;

    if (latestReaded >= 0.006)
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
            return false;
        }
        sqlOk = query.exec("INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + softVersion + "', '" + QString::number(dbVersion) + "')");
        if (sqlOk)
        { // Version updated
            sqlOk = query.exec("DROP TABLE prop_mode_enumeration");
        if (!sqlOk)
            {
               //qDebug() << "DataBase::updateTo006 - prop_mode_enumeration NOT DROPED" << endl;
            }

            sqlOk = createTablePropModes();
        if (!sqlOk)
            {
               //qDebug() << "DataBase::updateTo006 - createTablePropModes FALSE" << endl;
            }
            sqlOk = populatePropagationModes();
        if (!sqlOk)
            {
               //qDebug() << "DataBase::updateTo006 - populatePropagationModes FALSE" << endl;
            }
            sqlOk = updateTableLog(6); // We copy the log into logtemp

        if (!sqlOk)
            {
               //qDebug() << "DataBase::updateTo006 - prop_mode table do not created" << endl;
            }

            createTableBand(false);             // We create the bandTemp
            populateTableBand(false);           // Populate the bandTemp
            updateBandIdTableLogToNewOnes();
            updateBandIdTableAward(1); // DXCC
            updateBandIdTableAward(2); // WAZ

            if (query.exec("DROP TABLE band"))
            {
                if (query.exec("ALTER TABLE bandtemp RENAME TO band"))
                {

                }
                else
                {
                   //qDebug() << "DataBase::updateTo006 - ERROR - bandtemp not renamed" << endl;
                }

            }
            else
            {
               //qDebug() << "DataBase::updateTo006 - ERROR - bandtemp not dropped" << endl;
            }


            createTableMode(false);         // Create modetemp
            populateTableMode(false);       // Populate modetemp

            updateModeIdFromSubModeId();    // Updates the log with the new mode IDs in each QSO
            //updateModeIdTableAward(1); //DXCC
            //updateModeIdTableAward(2); // WAZ

            if (query.exec("DROP TABLE mode"))
            {
                if (query.exec("ALTER TABLE modetemp RENAME TO mode"))
                {

                }
                else
                {
                   //qDebug() << "DataBase::updateTo006 - ERROR - modetemp not renamed" << endl;
                }

            }
            else
            {
               //qDebug() << "DataBase::updateTo006 - ERROR - modetemp not dropped" << endl;
            }

            createTableClubLogStatus();
            populateTableClubLogStatus();

        }
        else
        { // Version not updated

        }
        //DO ALL THE TASKS TO BE IN 0.006 from 0.005 HERE and set ErrorUpdating if it is not possible.
        //qDebug() << "DataBase::updateTo006 - I am in 006 " << endl;
        IAmIn006 = true;
    }
    //qDebug() << "DataBase::updateTo006 - END " << endl;
    return IAmIn006;
}

bool DataBase::updateTableLog(const int _v)
{
  //qDebug() << "DataBase::updateTableLog " << endl;
    createTableLog(false);
    QString queryString;
    switch (_v)
    {
    case 6: // If 6, we copy in logtemp the full data coming from the old log. This way, the structure of the log table is updated without any data loss.
        queryString = QString ("INSERT INTO logtemp (qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, srx, stx, points, multiplier, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, band_rx, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_gridsquare, my_iota, my_iota_island_id, my_lat, my_lon, my_name, my_rig, my_sig, my_sig_info, my_state, my_street, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, srx_string, stx_string, state, station_callsign, swl, ten_ten, tx_pwr, web, qso_date_off, time_off, transmiterid, marked, lognumber) SELECT qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, srx, stx, points, multiplier, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, band_rx, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_gridsquare, my_iota, my_iota_island_id, my_lat, my_lon, my_name, my_rig, my_sig, my_sig_info, my_state, my_street, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, srx_string, stx_string, state, station_callsign, swl, ten_ten, tx_pwr, web, qso_date_off, time_off, transmiterid, marked, lognumber FROM log");
    break;
    default:
       //qDebug() << "DataBase::updateTableLog FALSE END" << endl;
        return false;
    break;
    }


    QSqlQuery query;
    if (query.exec(queryString))
    {
       //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Query executed" << endl;
        queryString = "DROP TABLE log";
        if (query.exec(queryString))
        {
           //qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Table log dropped" << endl;

            queryString = "ALTER TABLE logtemp RENAME TO log" ;
            if (query.exec(queryString))
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
    QSqlQuery query;
    return query.exec("CREATE TABLE clublog_status ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "shortname VARCHAR(1) NOT NULL, "
               "name VARCHAR(15) NOT NULL)");
}

bool DataBase::populateTableClubLogStatus()
{
   //qDebug() << "populateTableClubLogStatus" << endl;
    QSqlQuery query;

    if (query.exec("INSERT INTO clublog_status (shortname, name) VALUES ('Y', 'Uploaded')"))
    {
        if (query.exec("INSERT INTO clublog_status (shortname, name) VALUES ('N', 'Do not upload')"))
        {
           //qDebug() << "populateTableClubLogStatus END" << endl;
            return query.exec("INSERT INTO clublog_status (shortname, name) VALUES ('M', 'Modified')");
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
    QSqlQuery query;
    bool sqlOk;

    result = createTableEntity(false);  // Now we have a temp entity table with the correct format
    if (result)
    {
       //qDebug() << "DataBase::updateTableEntity: Table entitytemp created!" << endl;
        // Now we need to move all the data from the old to the temp entity table.

        stringQuery = QString("INSERT INTO entitytemp (name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate) SELECT name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate FROM entity");

        sqlOk = query.exec(stringQuery);

        if (sqlOk)
        {
           //qDebug() << "DataBase::updateTableEntity: Data copied from entity to entitytemp!" << endl;

            if (query.exec("DROP TABLE entity"))
            {
               //qDebug() << "DataBase::updateTableEntity: Table entity DELETED" << endl;

                if (query.exec("ALTER TABLE entitytemp RENAME TO entity"))
                {
                   //qDebug() << "DataBase::updateTableEntity: Table entitytemp renamed" << endl;
                    return true;
                }
                else
                {
                  //qDebug() << "DataBase::updateTableEntity: Table entitytemp NOT renamed" << endl;
                   return false;
                }

            }
            else
            {
              //qDebug() << "DataBase::updateTableEntity: Table entity NOT DELETED" << endl;
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
    QSqlQuery query;
    bool sqlOk;

    result = createTableLogs(false);  // Now we have a temp entity table with the correct format

    if (result)
    {
       //qDebug() << "DataBase::updateTableLogs: Table logstemp created!" << endl;
        // Now we need to move all the data from the old to the temp logs table.

        stringQuery = QString("INSERT INTO logstemp (logdate, stationcall, comment, logtype, logtypen) SELECT logdate, stationcall, comment, logtype, logtypen FROM logs");

        sqlOk = query.exec(stringQuery);

        if (sqlOk)
        {
           //qDebug() << "DataBase::updateTableLogs: data copied" << endl;

            if (query.exec("DROP TABLE logs"))
            {
               //qDebug() << "DataBase::updateTableLogs: Table logs DELETED" << endl;

                if (query.exec("ALTER TABLE logstemp RENAME TO logs"))
                {
                   //qDebug() << "DataBase::updateTableLogs: Table logstemp renamed" << endl;
                    return true;
                }
                else
                {
                  //qDebug() << "DataBase::updateTableLogs: Table logstemp NOT renamed" << endl;
                   return false;
                }

            }
            else
            {
              //qDebug() << "DataBase::updateTableLogs: Table logs NOT DELETED" << endl;
               return false;
            }

        }
        else
        {
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
    QSqlQuery query, query2, query3;
    bool sqlOk = query.exec("SELECT COUNT (*) FROM log");
    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
    }
    else
    {
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
            modetxt = "";
            modeFound = -1;

            if (query.isValid())
            {

                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating mode information...\n QSO: ")  + QString::number(i) + "/" + QString::number(qsos);
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

                            sq = QString ("UPDATE log SET modeid='%1' WHERE id='%2'").arg(modeFound).arg(id);           // STEP-4
                            sqlOk3 = query3.exec(sq);
                            if (sqlOk3)
                            {
                                //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-4) ID: " << QString::number(id) << " updated to: " << QString::number(modeFound) <<"/"<< modetxt << endl;
                            }
                            else
                            {
                                //qDebug() << "DataBase::updateModeIdFromSubModeId: (STEP-4) ID: " << QString::number(id) << " NOT updated-2"  << endl;
                            }


                        }
                        else
                        {
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
                    aux = QObject::tr("Cancelling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
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
           //qDebug() << "DataBase::updateModeIdFromSubModeId: FALSE END 2"  << endl;
            return false;
        }
       //qDebug() << "DataBase::updateModeIdFromSubModeId: END"  << endl;
        return true;
    }
    else
    {
       //qDebug() << "DataBase::updateModeIdFromSubModeId: FALSE END 3"  << endl;
        return false;
    }
   //qDebug() << "DataBase::updateModeIdFromSubModeId: CHECK IF this is seen - END"  << endl;

}


bool DataBase::updateBandIdTableLogToNewOnes()
{
  //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: "  << endl;

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
   //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END"  << endl;
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
            bandtxt = "";
            bandFound = -1;

            if (query.isValid())
            {
                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating bands information...\n QSO: ")  + QString::number(i) + "/" + QString::number(qsos);
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
                            sqlOk3 = query3.exec(sq);
                            if (sqlOk3)
                            {
                                //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt << endl;
                            }
                            else
                            {
                                //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-2"  << endl;
                               //qDebug() << "DataBase::updateBandIdTableLogToNewOnes - QSOs not updated to main log" << endl;
                                errorCode = query3.lastError().number();
                               //qDebug() << "DataBase::updateBandIdTableLogToNewOnes - query error: " << QString::number(errorCode) << endl;
                               //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: LastQuery: " << query3.lastQuery()  << endl;
                               //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: LastError-data: " << query3.lastError().databaseText()  << endl;
                               //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: LastError-driver: " << query3.lastError().driverText()  << endl;
                               //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: LastError-n: " << QString::number(query3.lastError().number() ) << endl;

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

                }
                else
                {
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
                    aux = QObject::tr("Cancelling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
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
           //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END 2"  << endl;
            return false;
        }
       //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: END OK"  << endl;
        return true;
    }
    else
    {
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


    sq = QString("SELECT COUNT (*) FROM %1").arg(table);

    bool sqlOk = query.exec(sq);
    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
    }
    else
    {
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
            bandtxt = "";
            bandFound = -1;

            if (query.isValid())
            {
                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating bands information...\n Progress: ")  + QString::number(i) + "/" + QString::number(qsos);
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

    sq = QString ("UPDATE %1 SET %2='%3' WHERE id='%4'").arg(table).arg(field).arg(bandFound).arg(id);
                            sqlOk3 = query3.exec(sq);
                            if (sqlOk3)
                            {
                                //qDebug() << "DataBase::updateBandIdTableAward: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt << endl;
                            }
                            else
                            {
                               //qDebug() << "DataBase::updateBandIdTableAward: ID: " << QString::number(id) << " NOT updated-2"  << endl;

                               //qDebug() << "DataBase::updateBandIdTableAward - QSOs not updated to main log" << endl;
                                errorCode = query3.lastError().number();
                               //qDebug() << "DataBase::updateBandIdTableAward - query error: " << QString::number(errorCode) << endl;
                               //qDebug() << "DataBase::updateBandIdTableAward: LastQuery: " << query3.lastQuery()  << endl;
                               //qDebug() << "DataBase::updateBandIdTableAward: LastError-data: " << query3.lastError().databaseText()  << endl;
                               //qDebug() << "DataBase::updateBandIdTableAward: LastError-driver: " << query3.lastError().driverText()  << endl;
                               //qDebug() << "DataBase::updateBandIdTableAward: LastError-n: " << QString::number(query3.lastError().number() ) << endl;
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

                }
                else
                {
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
                    aux = QObject::tr("Cancelling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
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
            return false;
        }
      //qDebug() << "DataBase::updateBandIdTableAward: END OK"  << endl;
        return true;
    }
    else
    {
       //qDebug() << "DataBase::updateBandIdTableAward: FALSE END-4"  << endl;
        return false;
    }
    //qDebug() << "DataBase::updateBandIdTableAward: CHECK IF SEEN END"  << endl;
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


    sq = QString("SELECT COUNT (*) FROM %1").arg(table);

    bool sqlOk = query.exec(sq);
    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
    }
    else
    {
       //qDebug() << "DataBase::updateModeIdTableAward: FALSE END-2"   << endl;
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
            bandtxt = "";
            bandFound = -1;

            if (query.isValid())
            {
                i++;

                if (( (i % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    aux = QObject::tr("Updating bands information...\n Progress: ")  + QString::number(i) + "/" + QString::number(qsos);
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

    sq = QString ("UPDATE %1 SET %2='%3' WHERE id='%4'").arg(table).arg(field).arg(bandFound).arg(id);
                            sqlOk3 = query3.exec(sq);
                            //qDebug() << "DataBase::updateModeIdTableAward (query3 update): " << query3.lastQuery()  << endl;
                            if (sqlOk3)
                            {
                                //qDebug() << "DataBase::updateModeIdTableAward: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt << endl;
                            }
                            else
                            {
                               //qDebug() << "DataBase::updateModeIdTableAward: ID: " << QString::number(id) << " NOT updated-2"  << endl;

                               //qDebug() << "DataBase::updateModeIdTableAward - QSOs not updated to main log" << endl;
                                errorCode = query3.lastError().number();
                               //qDebug() << "DataBase::updateModeIdTableAward - query error: " << QString::number(errorCode) << endl;
                               //qDebug() << "DataBase::updateModeIdTableAward: LastQuery: " << query3.lastQuery()  << endl;
                               //qDebug() << "DataBase::updateModeIdTableAward: LastError-data: " << query3.lastError().databaseText()  << endl;
                               //qDebug() << "DataBase::updateModeIdTableAward: LastError-driver: " << query3.lastError().driverText()  << endl;
                               //qDebug() << "DataBase::updateModeIdTableAward: LastError-n: " << QString::number(query3.lastError().number() ) << endl;

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
                    aux = QObject::tr("Cancelling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
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
            return false;
        }
      //qDebug() << "DataBase::updateModeIdTableAward: END OK"  << endl;
        return true;
    }
    else
    {
       //qDebug() << "DataBase::updateModeIdTableAward: FALSE END-4"   << endl;
        return false;
    }
   //qDebug() << "DataBase::updateModeIdTableAward: Checkif seen END"   << endl;
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
            bandtxt = "";
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
                    aux = QObject::tr("Cancelling this update will cause data inconsistencies and possibly data loss. Do you still want to cancel?");
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

   //qDebug() << "DataBase::updateTo007" << endl;
    bool IAmIn007 = false;
    bool IAmIn006 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    QSqlQuery query;

    bool sqlOk = false;

    if (latestReaded >= 0.007)
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
        if (ErrorUpdating)
        {
            return false;
        }
        sqlOk = query.exec("INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + softVersion + "', '" + QString::number(dbVersion) + "')");
        if (sqlOk)
        { // Version updated
            IAmIn007 = updateTableLog(6);
        }
        else
        { // Version not updated

        }
        //DO ALL THE TASKS TO BE IN 0.004 from 0.003 HERE and set ErrorUpdating if it is not possible.
        IAmIn007 = true;
    }
    return IAmIn007;
}



bool DataBase::updateTo008()
{// Updates the DB to 0.0.8

   //qDebug() << "DataBase::updateTo008" << endl;
    bool IAmIn008 = false;
    bool IAmIn007 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    QSqlQuery query;

    bool sqlOk = false;

    if (latestReaded >= 0.008)
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
        if (ErrorUpdating)
        {
            return false;
        }
        sqlOk = query.exec("INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + softVersion + "', '" + QString::number(dbVersion) + "')");
        if (sqlOk)
        { // Version updated
            //IAmIn008 = updateTableLog(6);
        }
        else
        { // Version not updated

        }
        //DO ALL THE TASKS TO BE IN 0.008 from 0.007 HERE and set ErrorUpdating if it is not possible.

        IAmIn008 = updateTheModeTableAndSyncLog();

        //IAmIn008 = true;
    }
    return IAmIn008;
}


bool DataBase::updateTo009()
{// Updates the DB to 0.0.9 - We add the Satellite tables

   //qDebug() << "DataBase::updateTo009" << endl;
    bool IAmIn009 = false;
    bool IAmIn008 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QString dateString = (date.currentDateTime()).toString("yyyyMMdd");
    QSqlQuery query;

    bool sqlOk = false;

    if (latestReaded >= 0.009)
    {
       //qDebug() << "DataBase::updateTo009: - I am in 009" << endl;
        return true;
    }
    else
    {
       //qDebug() << "DataBase::updateTo009: - I am not in 009" << endl;
        IAmIn009 = false;
    }


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
        sqlOk = query.exec("INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + softVersion + "', '" + QString::number(dbVersion) + "')");
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




        if (createTableSatellites(true))
        {
           //qDebug() << "DataBase::updateTo009: - createTableSatellites OK" << endl;
            if (populateTableSatellites(true))
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

bool DataBase::updateTheModeTableAndSyncLog()
{
   //qDebug() << "DataBase::updateTheModeTableAndSyncLog" << endl;
    QSqlQuery query;

    createTableMode(false);         // Create modetemp
    populateTableMode(false);       // Populate modetemp

    updateModeIdFromSubModeId();    // Updates the log with the new mode IDs in each QSO
    //updateModeIdTableAward(1); //DXCC
    //updateModeIdTableAward(2); // WAZ

    if (query.exec("DROP TABLE mode"))
    {
        if (query.exec("ALTER TABLE modetemp RENAME TO mode"))
        {
            return true;
        }
        else
        {
           //qDebug() << "DataBase::updateTheModeTableAndSyncLog - ERROR - modetemp not renamed" << endl;
            return false;
        }
    }
    else
    {
       //qDebug() << "DataBase::updateTheModeTableAndSyncLog - ERROR - modetemp not dropped" << endl;
        return false;
    }
   //qDebug() << "DataBase::updateTheModeTableAndSyncLog END" << endl;
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
    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='246'");  //Sovereign Order of Malta
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
   //qDebug() << "DataBase::updateTheEntityTableISONames-2" << endl;

    sq = QString ("UPDATE entity SET isoname='247' WHERE dxcc='un'");  //Spratly
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='260' WHERE dxcc='mc'");  //Monaco
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mu' WHERE dxcc=''");  // Agalega
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mu' WHERE dxcc='165'");  //Mauricio
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mu' WHERE dxcc='207'");  //Rodriguez
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gq' WHERE dxcc='49'");  // Equatorial Guinea
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fj' WHERE dxcc='176'");  //Fidji
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gq' WHERE dxcc='195'");  //Annobon
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fj' WHERE dxcc='489'");  // Conway reef
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fj' WHERE dxcc='460'");  // Rotuma
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sz' WHERE dxcc='468'");  // Swaziland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tn' WHERE dxcc='474'");  // Tunisia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vn' WHERE dxcc='293'");  // Vietnam
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gn' WHERE dxcc='107'");  // Guinea
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bv' WHERE dxcc='24'");  // Bouvet
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='no' WHERE dxcc='199'");  // Peter 1 Is"
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='az' WHERE dxcc='18'");  // Azerbaijan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ge' WHERE dxcc='75'");  // Georgia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='me' WHERE dxcc='514'");  // Montenegro
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lk' WHERE dxcc='315'");  // Sri lanka
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ch' WHERE dxcc='117'");  // ITU HQ
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='289'");  // UN HQ
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tl' WHERE dxcc='511'");  // Timor Leste
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='il' WHERE dxcc='336'");  // Israel
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ly' WHERE dxcc='436'");  // Libya
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cy' WHERE dxcc='215'");  // Cyprus
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tz' WHERE dxcc='470'");  // Tanzania
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ng' WHERE dxcc='450'");  // Nigeria
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='mg' WHERE dxcc='438'");  // Madagascar
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mr' WHERE dxcc='444'");  // Mauritania
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='ne' WHERE dxcc='187'");  // Niger
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tg' WHERE dxcc='483'");  // Togo
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ws' WHERE dxcc='190'");  // Samoa
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ug' WHERE dxcc='286'");  // Uganda
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ke' WHERE dxcc='430'");  // Kenya
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sn' WHERE dxcc='456'");  // Senegal
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jm' WHERE dxcc='82'");  // Jamaica
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='es' WHERE dxcc='281'");  // Spain
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ls' WHERE dxcc='432'");  //Lesotho
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mw' WHERE dxcc='440'");  // Malawi
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dz' WHERE dxcc='400'");  // Algeria
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ye' WHERE dxcc='492'");  // Yemen
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bb' WHERE dxcc='62'");  // Barbados
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mv' WHERE dxcc='159'");  // Maldives
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gy' WHERE dxcc='129'");  // Guyana
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hr' WHERE dxcc='497'");  // Croatia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gh' WHERE dxcc='424'");  // Ghana
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mt' WHERE dxcc='257'");  // Malta
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='zm' WHERE dxcc='482'");  // Zambia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kw' WHERE dxcc='348'");  // Kuwait
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sl' WHERE dxcc='458'");  // Sierra Leone
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='my' WHERE dxcc='299'");  // West Malaysia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='my' WHERE dxcc='46'");  // East Malaysia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='np' WHERE dxcc='369'");  // Nepal
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cd' WHERE dxcc='414'");  // Dem Rep Congo
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bi' WHERE dxcc='404'");  // Burundi
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sg' WHERE dxcc='381'");  // Singapore
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='rw' WHERE dxcc='454'");  // Rwanda
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tt' WHERE dxcc='90'");  // Trinidad & Tobago
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bw' WHERE dxcc='402'");  // Botswana
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='to' WHERE dxcc='160'");  //  Tonga
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='om' WHERE dxcc='370'");  // Oman
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bt' WHERE dxcc='306'");  // Bhutan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ae' WHERE dxcc='391'");  // Un Arab Emirates
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='qa' WHERE dxcc='376'");  // Qatar
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bh' WHERE dxcc='304'");  // Bahrain
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pk' WHERE dxcc='372'");  // Pakistan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tw' WHERE dxcc='386'");  // Taiwan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tw' WHERE dxcc='505'");  // Pratas Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cn' WHERE dxcc='318'");  // China
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nr' WHERE dxcc='157'");  //  Nauru
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ad' WHERE dxcc='203'");  // Andorra
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gm' WHERE dxcc='422'");  // Gambia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bs' WHERE dxcc='60'");  // Bahamas
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mz' WHERE dxcc='181'");  // Mozambique
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='112'");  // Chile
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='217'");  // San Felix
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='47'");  // Easter Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cl' WHERE dxcc='125'");  //  Juan Fernandez is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cu' WHERE dxcc='70'");  // Cuba
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ma' WHERE dxcc='446'");  // Morocco
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bo' WHERE dxcc='104'");  // Bolivia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pt' WHERE dxcc='272'");  // Portugal
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pt' WHERE dxcc='256'");  // Madeira
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pt' WHERE dxcc='149'");  // Azores
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='uy' WHERE dxcc='144'");  // Uruguay
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ca' WHERE dxcc='211'");  // Sable Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ca' WHERE dxcc='252'");  // St Paul is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ao' WHERE dxcc='401'");  // Angola
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cv' WHERE dxcc='409'");  // Cape Verde
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='km' WHERE dxcc='411'");  // Comoros
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='de' WHERE dxcc='230'");  // Fed Rep Germany
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ph' WHERE dxcc='375'");  //Philippines
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='er' WHERE dxcc='51'");  // Eritrea
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ps' WHERE dxcc='510'");  // Palestine
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ck' WHERE dxcc='191'");  // North Cook
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ck' WHERE dxcc='234'");  // South Cook
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nu' WHERE dxcc='188'");  // Niue
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ba' WHERE dxcc='501'");  // Bosnia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='es' WHERE dxcc='21'");  // Balearic is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='canary' WHERE dxcc='29'");  // Canary Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='es' WHERE dxcc='32'");  // Ceuta & Melilla
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ie' WHERE dxcc='245'");  // Ireland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='am' WHERE dxcc='14'");  // Armenia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lr' WHERE dxcc='434'");  // Liberia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ir' WHERE dxcc='330'");  // Iran
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mv' WHERE dxcc='179'");  // Moldova
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ee' WHERE dxcc='52'");  //  Estonia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='et' WHERE dxcc='53'");  // Ethiopia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='by' WHERE dxcc='27'");  // Belarus
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kg' WHERE dxcc='135'");  // Kyrgyzstan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tm' WHERE dxcc='262'");  // Turkmenistan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='227'");  // France
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='79'");  // Guadeloupe
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='yt' WHERE dxcc='169'");  // Mayotte
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='516'");  // St Barthelemy
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nc' WHERE dxcc='162'");  // New Caledonia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nc' WHERE dxcc='512'");  // Chesterfield Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mq' WHERE dxcc='84'");  // Martinique
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pf' WHERE dxcc='175'");  // French Polynesia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pf' WHERE dxcc='508'");  // Austral Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pm' WHERE dxcc='277'");  //  St Pierre & Miquelon
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='re' WHERE dxcc='453'");  //Reunion Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='213'");  // St Marteen
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='99'");  // Glorioso is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='124'");  // Juan de nova, Europa
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='276'");  //  Tromelin - TODO: Add the wikipedia flag
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='41'");  // Crozet
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='131'");  // Kerguelen
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag http://es.wikipedia.org/wiki/Tierras_Australes_y_Ant%C3%A1rticas_Francesas

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='10'");  //Amsterdam & St Paul is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='wf' WHERE dxcc='298'");  // Wallis & Futuna is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gf' WHERE dxcc='63'");  // French Guiana
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='england' WHERE dxcc='223'");  // England
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='114'");  //Isle of Man
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='northernireland' WHERE dxcc='265'");  // Northern Ireland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='122'");  // Jersey
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='scotland' WHERE dxcc='279'");  //  Scotland & Shetland is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='106'");  // Guernsey
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='wales' WHERE dxcc='294'");  // Wales
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sb' WHERE dxcc='185'");  // Solomon
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sb' WHERE dxcc='507'");  // Temotu Province
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hu' WHERE dxcc='239'");  // Hungary
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ch' WHERE dxcc='287'");  // Switzerland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='li' WHERE dxcc='251'");   // Liechtenstein
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ec' WHERE dxcc='120'");  // Ecuador
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ec' WHERE dxcc='71'");  // Galapagos Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ht' WHERE dxcc='78'");  // Haiti
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='do' WHERE dxcc='72'");  // Dominican Rep
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='co' WHERE dxcc='116'");  // Colombia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='co' WHERE dxcc='216'");  // San Andres & Providencia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='co' WHERE dxcc='161'");  // Malpelo
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kr' WHERE dxcc='137'");  // Rep Korea
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pa' WHERE dxcc='88'");  // Panama
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hn' WHERE dxcc='80'");  // Honduras
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='th' WHERE dxcc='387'");  // Thailand
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='va' WHERE dxcc='295'");  // Vatican City
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sa' WHERE dxcc='378'");  // Saudi Arabia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='it' WHERE dxcc='248'");  // Italy
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dj' WHERE dxcc='382'");  // Djibouti
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gd' WHERE dxcc='77'");  // Grenada
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gw' WHERE dxcc='109'");  // Guinea-Bissau
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lc' WHERE dxcc='97'");  // St Lucia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dm' WHERE dxcc='95'");  // Dominica
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vc' WHERE dxcc='98'");  // St Vicent
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jp' WHERE dxcc='339'");  // Japan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jp' WHERE dxcc='177'");  // Minami Torishima
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jp' WHERE dxcc='192'");  // Ogasawara
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mn' WHERE dxcc='363'");  // Mongolia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sj' WHERE dxcc='259'");  // Svalbard
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sj' WHERE dxcc='118'");  // Jan Mayen
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='jo' WHERE dxcc='342'");  // Jordan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='291'");  // United States
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='105'");  // Guantanamo Bay
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mp' WHERE dxcc='166'");  //  Mariana is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='20'");  // Baker & Howland is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gu' WHERE dxcc='103'");  // Guam
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='123'");  // Johnston is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='174'");  // Midway is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='197'");  // Palmyra & Jarbis is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='134'");  // Kingman Reef
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='110'");  // Hawaii
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='138'");  // Kure is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='as' WHERE dxcc='9'");  // American Samoa
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='as' WHERE dxcc='515'");  // Swains is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='297'");  // Wake is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='6'");  // Alaska
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='182'");  // Navassa Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vi' WHERE dxcc='285'");  // Us Virgin is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pr' WHERE dxcc='202'");  // Puerto Rico
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='us' WHERE dxcc='43'");  // Desecheo Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='no' WHERE dxcc='266'");  // Norway
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ar' WHERE dxcc='100'");  // Argentina
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lu' WHERE dxcc='254'");  // Luxembourg
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lt' WHERE dxcc='146'");  // Lithuania
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bg' WHERE dxcc='212'");  // Bulgaria
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pe' WHERE dxcc='136'");  // Peru
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lb' WHERE dxcc='354'");  // Lebanon
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='at' WHERE dxcc='206'");  // Austria & Viena Intl HQ
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fi' WHERE dxcc='224'");  // Findland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fi' WHERE dxcc='5'");  // Aland is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fi' WHERE dxcc='167'");  // Market Reef
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cz' WHERE dxcc='503'");  // Czech Rep
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sk' WHERE dxcc='504'");  // Slovak Rep
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='be' WHERE dxcc='209'");  // Belgium
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gl' WHERE dxcc='237'");  // Greenland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='dk' WHERE dxcc='222'");  // Faroe is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='dk' WHERE dxcc='221'");  // Denmark
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pg' WHERE dxcc='163'");  // Papua New Guinea
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='aw' WHERE dxcc='91'");  // Aruba
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kp' WHERE dxcc='344'");  //Dpr Korea
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nl' WHERE dxcc='263'");  // Netherlands
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cw' WHERE dxcc='517'");  // Curacao
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='bq' WHERE dxcc='520'");  // Bonaire
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='nl' WHERE dxcc='519'");  // Saba & St Eustatius
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='shm' WHERE dxcc='518'");  // Sint Marteen
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='108'");  // Brazil
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='56'");  // Fernando de Noronha
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='253'");  //  St Peter & St Paul
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='br' WHERE dxcc='273'");  // Trindade & Martim Vaz
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sr' WHERE dxcc='140'");  // Suriname
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='61'");  // Franz Josef Land
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='eh' WHERE dxcc='302'");  // Western Sahara
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='bd' WHERE dxcc='305'");  // Bangladesh
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='si' WHERE dxcc='499'");  // Slovenia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sc' WHERE dxcc='379'");  // Seychelles
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='st' WHERE dxcc='219'");  // Sao Tome & Principe
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='se' WHERE dxcc='284'");  // Sweden
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pl' WHERE dxcc='269'");  // Poland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sd' WHERE dxcc='466'");  // Sudan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='eg' WHERE dxcc='479'");  // Egypt
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='236'");  // Greece
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='180'");  // Mount Athos
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='45'");  // Dodecanese
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gr' WHERE dxcc='40'");  // Crete
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tv' WHERE dxcc='282'");  // Tuvalu
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='301'");  // Western Kiribati
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='31'");  // Central Kiribati
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='48'");  // Eastern Kiribati
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ki' WHERE dxcc='490'");  // Banaba is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='so' WHERE dxcc='232'");  // Somalia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sm' WHERE dxcc='278'");  // San Marino
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pw' WHERE dxcc='22'");  // Palau
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='tr' WHERE dxcc='390'");  // Turkey
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='is' WHERE dxcc='242'");  // Iceland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gt' WHERE dxcc='76'");  // Guatemala
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    //TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='cr' WHERE dxcc='308'");  // Costa Rica
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cr' WHERE dxcc='37'");  // Coco is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cm' WHERE dxcc='406'");  // Cameroon
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fr' WHERE dxcc='214'");  // Corsica
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}
    // TODO: Add the wikipedia flag

    sq = QString ("UPDATE entity SET isoname='cf' WHERE dxcc='408'");  // Central African Rep
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cg' WHERE dxcc='412'");  // Rep of Congo
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}


    sq = QString ("UPDATE entity SET isoname='ga' WHERE dxcc='420'");  // Gabon
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='td' WHERE dxcc='410'");  // Chad
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ci' WHERE dxcc='428'");  // Cote d'Ivoire
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bj' WHERE dxcc='416'");  // Benin
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ml' WHERE dxcc='442'");  // Mali
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='54'");  // European Rusia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='126'");  // Kaliningrad
    sqlOK = query.exec(sq);
    if (!sqlOK)

    sq = QString ("UPDATE entity SET isoname='ru' WHERE dxcc='15'");  // Asiatic Rusia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='uz' WHERE dxcc='292'");  // Uzbekistan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kz' WHERE dxcc='130'");  // Kazakhstan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ua' WHERE dxcc='288'");  // Ukraine
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ag' WHERE dxcc='94'");  // Antigua & Barbuda
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bz' WHERE dxcc='66'");  // Belize
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kn' WHERE dxcc='249'");  // St Kitts & Nevis
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='na' WHERE dxcc='464'");  // Namibia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fm' WHERE dxcc='173'");  // Micronesia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fm' WHERE dxcc='168'");  // Marshall Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bn' WHERE dxcc='345'");  // Brunei Darusalam
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ca' WHERE dxcc='1'");  // Canada
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='150'");  // Australia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hm' WHERE dxcc='111'");  // Heard Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='153'");  // Macquarie is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cc' WHERE dxcc='38'");  //  Cocos / Keeling is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='147'");  //  Lord Howe is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='171'");  // Mellish Reed
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nf' WHERE dxcc='189'");  // Norkfolk is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='au' WHERE dxcc='303'");  // Willis Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='cx' WHERE dxcc='35'");  // Christmas is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ai' WHERE dxcc='12'");  //  Anguilla
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ms' WHERE dxcc='96'");  // Montserrat
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vg' WHERE dxcc='65'");  // British is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tc' WHERE dxcc='89'");  // Turks & Caicos is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='pn' WHERE dxcc='172'");  // Pitcairn
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='513'");  // Ducie is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='fk' WHERE dxcc='141'");  // Falkland is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gs' WHERE dxcc='235'");  // South Georgia is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='241'");  // South Shetland is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='238'");  // South Orkney is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gs' WHERE dxcc='240'");  // South Sandwich Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bm' WHERE dxcc='64'");  // Bermuda
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='io' WHERE dxcc='33'");  // Chagos is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='hk' WHERE dxcc='321'");  // Hong Kong
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='in' WHERE dxcc='324'");  // India
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='in' WHERE dxcc='11'");  // Andaman & Nicobar
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='in' WHERE dxcc='142'");  // Lakshadweep Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mx' WHERE dxcc='50'");  // Mexico
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mx' WHERE dxcc='204'");  // Revilagigedo
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='bf' WHERE dxcc='480'");  // Burkina Faso
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='kh' WHERE dxcc='312'");  //  Cambodia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='la' WHERE dxcc='143'");  // Laos
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mo' WHERE dxcc='152'");  // Macao
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mm' WHERE dxcc='309'");  // Myanmar
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='af' WHERE dxcc='3'");  // Afganistan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='id' WHERE dxcc='327'");  // Indonesia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='iq' WHERE dxcc='333'");  // Iraq
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='vu' WHERE dxcc='158'");  // Vanuatu
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sy' WHERE dxcc='384'");  // Syria
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='lv' WHERE dxcc='145'");  // Latvia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ni' WHERE dxcc='86'");  //  Nicaragua
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ro' WHERE dxcc='275'");  // Romania
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sv' WHERE dxcc='74'");  // El Salvador
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='rs' WHERE dxcc='296'");  // Serbia && Kosovo
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ve' WHERE dxcc='148'");  // Venezuela
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ve' WHERE dxcc='17'");  // Aves Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='zw' WHERE dxcc='452'");  // Zimbabwe
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='mk' WHERE dxcc='502'");  // Macedonia
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ss' WHERE dxcc='521'");  //Rep South Sudan
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='al' WHERE dxcc='7'");  // Albania
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gi' WHERE dxcc='233'");  // Gibraltar
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='gb' WHERE dxcc='283'");  // UK Base Aereas Cyprus
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sh' WHERE dxcc='250'");  // St Helena
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sh' WHERE dxcc='205'");  // Ascension is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='sh' WHERE dxcc='274'");  // Tristan da Cunha & Gough is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='ky' WHERE dxcc='69'");  // Cayman Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='tk' WHERE dxcc='270'");  // Tokelau Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='170'");  // New Zeland
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='34'");  // Chatham Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='133'");  // Kermadec is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='nz' WHERE dxcc='16'");  // Auckland & Campbell is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='py' WHERE dxcc='132'");  // Paraguay
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='za' WHERE dxcc='462'");  // South Africa
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='za' WHERE dxcc='201'");  // Pr Edward & Marion Is
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

// Countries without flag or controversial - Data is added just to keep the DB filled-up
    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='506'");  // Scarboroug Reef
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

    sq = QString ("UPDATE entity SET isoname='un' WHERE dxcc='13'");  //  Antartica
    sqlOK = query.exec(sq);
    if (!sqlOK)
    {return false;}

   //qDebug() << "DataBase::updateTheEntityTableISONames-END" << endl;
    return true;
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
        return false;
    }

   //qDebug() << "DataBase::isTheDBCreated: END FALSE" << endl;
    return false;
}
