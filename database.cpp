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


DataBase::DataBase(const QString _softVersion, bool inmemoryonly){
    //qDebug() << "DataBase::DataBase: " << _softVersion  << endl;
    dbVersion = DBVersionf;
    softVersion = _softVersion;
    inMemoryOnly = inmemoryonly;
    latestReaded = 0.0;
    util = new Utilities();

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
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery query;

    //qDebug() << "DataBase::createConnection: -1" << endl;
    if (inMemoryOnly)
    {
        //qDebug() << "DataBase::createConnection: Memory" << endl;
        db.setDatabaseName(":memory:"); // 2m 07s
    }
    else
    {
        //qDebug() << "DataBase::createConnection: File" << endl;
        db.setDatabaseName("logbook.dat");
    }

    //qDebug() << "DataBase::createConnection: 0" << endl;

    //rc = sqlite3_open(":memory:", &db);
    if (!db.open()) {
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
    createBandModeMaps();
    }
    return unMarkAllQSO();
}

bool DataBase::isTheDBCreated()
{
    //qDebug() << "DataBase::isTheDBCreated"  << endl;
    QSqlQuery query;
    int _num = 0;

    QString stringQuery ("SELECT count(id) FROM softwarecontrol");
    query.exec(stringQuery);
    query.next();
    if (query.isValid())
    {
        //qDebug() << "DataBase::isTheDBCreated - valid"  << endl;
        _num = (query.value(0)).toInt();
        if (_num > 0)
        {
            //qDebug() << "DataBase::isTheDBCreated - DB Exists"  << endl;
            return true;
        }
        else
        {
            //qDebug() << "DataBase::isTheDBCreated - DB does not Exist"  << endl;
            return false;
        }
    }
    else
    {
        //qDebug() << "DataBase::isTheDBCreated - not valid"  << endl;
        return false;
    }
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
        qDebug() << "DataBase::createTableLog: log"  << endl;
    }
    else
    {
        stringQuery = "CREATE TABLE logtemp" ;
        qDebug() << "DataBase::createTableLog: logtemp"  << endl;
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
             "freq INTEGER, "
             "freq_rx INTEGER, "
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
    //qDebug() << "DataBase::createDataBase"  << endl;
    bool qres;
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

    createTableLog(true);


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


      query.exec("CREATE TABLE entity ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
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
                 "UNIQUE (dxcc, mainprefix), "
                 "FOREIGN KEY (continent) REFERENCES continent(shortname))");
//TODO: To add some columns in this the table to mark if worked/confirmed/band/Mode
//query.exec("INSERT INTO entity (name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate) VALUES ('Canada', '0', '0', '0', '0', '0', '0', '0', 'VE', 'q', 'sincedate', 'todate')");



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
      createTableLogs();
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






//To add a mode, just create another line:      



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
    if (isValidBand(b))
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

QString DataBase::getBandNameFromNumber(const int _n)
{
    QSqlQuery query;
    QString queryString = QString("SELECT name FROM band WHERE id='%1'").arg(_n);

    query.exec(queryString);
    query.next();
    if ( query.isValid() )
    {
        if ( isValidBand((query.value(0)).toString())  )
        {
            return (query.value(0)).toString();
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}



QString DataBase::getModeNameFromNumber(const int _n)
{
    QSqlQuery query;
    QString queryString = QString("SELECT name FROM mode WHERE id='%1'").arg(_n);

    query.exec(queryString);
    query.next();
    if ( query.isValid() )
    {
        if ( isValidMode((query.value(0)).toString())  )
        {
            return (query.value(0)).toString();
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}

bool DataBase::isValidBand (const QString b)
{
    if (b.length()<1)
    {
        return false;
    }
    QString stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    return query.isValid();
}

bool DataBase::isValidMode (const QString b)
{
    if (b.length()<2)
    {
        return false;
    }
    QString stringQuery = QString("SELECT id FROM mode WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    return query.isValid();
}

bool DataBase::isValidBandNumber (const int b)
{
    return isValidBand(getBandNameFromNumber(b));

}

bool DataBase::isValidModeNumber (const int b)
{
    return isValidMode(getModeNameFromNumber(b));
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
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        //qDebug() << "DataBase::getBandIdFromFreq: Query NOK" << endl;
       return -1;
    }


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

    return false;
}


bool DataBase::unMarkAllQSO()
{
    QString stringQuery = QString("UPDATE log SET marked = 'N' WHERE 1");
    QSqlQuery query(stringQuery);
    //qDebug() << "MainWindow::slotQSLSentViaBureauFromLog: " << stringQuery << endl;
    //query.exec(stringQuery);
    //TODO: Check if the execution of this query is OK or NOK (should return false)
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
    int errorCode = -1;
    bool toBeUpdated = false;
    bool sqlOK;
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
        return true;
    }
    else
    { // DB is outdated. We need to update!!

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
                return false;
            break;
        }



    }

    // If the DB needs to be updated... we update it! :-)


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
    QHash<double, int> freqBandIdHash;

*/

    //qDebug() << "DataBase::createTheBandQuickReference: " << endl;

    QString st = "NULL";
    int in = 0;
    double fr = 0;
    QSqlQuery query("SELECT id, name, lower FROM band");
    while (query.next())
    {

        if (query.isValid())
        {
            st = (query.value(1)).toString();
            in = (query.value(0)).toInt();
            fr = (query.value(2)).toDouble();
            bandIDHash.insert(st, in );
            IDBandHash.insert(in, st);
            freqBandIdHash.insert(in, fr);

            //qDebug() << "DataBase::createTheBandQuickReference: " << st <<"/" << QString::number(in)<< endl;
        }
        else
        {
           //qDebug() << "DataBase::createTheBandQuickReference: Query not valid" << endl;

          // QMessageBox::warning(0, QObject::tr("Database Error (DataBase::createTheBandQuickReference)"),
          //                      query.lastError().text());
           return false;
           //TODO: Manage this error, in case the query is NOK.

        }
    }
/*
    QHashIterator<QString, int> i(bandIDHash);
    while (i.hasNext()) {
        i.next();
        //qDebug() << i.key() << ": " << QString::number(i.value()) << endl;
    }
*/

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
        QSqlQuery query("SELECT id, name FROM mode");
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
               //qDebug() << "DataBase::createTheModeQuickReference: Query not valid" << endl;

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
    if (b.length()<2)
    {
        return -3;
    }

    if (modeIDHash.contains(b))
    {
        //qDebug() << "DataBase::getModeIDFromName2: " << b << ":" <<  modeIDHash.value(b) << endl;
        return modeIDHash.value(b);
    }
    else
    {
        //qDebug() << "DataBase::getModeIDFromName2: Contains - False" << endl;
        return -1;
    }
     //qDebug() << "DataBase::getModeIDFromName2: Safety exit" << endl;
    return -2;
}

QString DataBase::getBandNameFromID2(const int _i)
{
    //qDebug() << "DataBase::getBandNameFromid2: " << QString::number(_i) << endl;

    if (IDBandHash.contains(_i))
    {
        return IDBandHash.value(_i);
    }
    else
    {
        return "-1";
    }
    return "-2";

}
QString DataBase::getModeNameFromID2(const int _i)
{

   //qDebug() << "DataBase::getModeNameFromId2: " << QString::number(_i) << endl;

    if (IDModeHash.contains(_i))
    {
        return IDModeHash.value(_i);
    }
    else
    {
        return "-1";
    }
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
        return (createTheBandQuickReference() &&  createTheModeQuickReference());
    }
    else
    {
        return false;
    }

}

double DataBase::getFreqFromBandId(const int _i)
{

    if (freqBandIdHash.contains(_i))
    {
        return freqBandIdHash.value(_i);
    }
    else
    {
        return -1.0;
    }
    return -2.0;


}

bool DataBase::updateToLatest()
{
/*
 * With the DB updates, the function that is called from here should be also updated.
 * The updateXXX are recursive calls that calls the previous one.
 *
 */
    //qDebug() << "DataBase::updateToLatest-005 " << endl;
    return updateTo006();
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
        return true;
    }
    else
    {
        IAmIn004 = false;
    }


    while (!IAmIn004 && !ErrorUpdating)
    {
        while (!IAmIn003 && !ErrorUpdating)
        {
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
           while (!IAmIn004 && !ErrorUpdating)
           {               
               IAmIn004 = updateTo004();
           }
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
                   sqlOk = createTableLogs();
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
                        text.toUpper();
                            if (ok && !text.isEmpty())
                            {
                                callToUse = text;
                            }
                            else
                            {
                                callToUse = "N0CALL";
                            }

//QString QInputDialog::getText ( QWidget * parent, const QString & title, const QString & label, QLineEdit::EchoMode mode = QLineEdit::Normal, const QString & text = QString(), bool * ok = 0, Qt::WindowFlags flags = 0, Qt::InputMethodHints inputMethodHints = Qt::ImhNone )


                        stringQuery = QString("INSERT INTO logs (logdate, stationcall, logtype, logtypen) values('%1','%2','DX', '0')").arg(dateString).arg(callToUse);
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
       return IAmIn005;
}

bool DataBase::recreateContestData()
{
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

bool DataBase::updateLog()
{
    //qDebug() << "DataBase::updateLog"  << endl;
    QSqlQuery query;
    bool sqlOk = false;
}

bool DataBase::createTableLogs()
{
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

bool DataBase::createTablePropModes()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE prop_mode_enumeration (id INTEGER PRIMARY KEY AUTOINCREMENT, shortname VARCHAR(8), name VARCHAR(55) )");
}


bool DataBase::createTableContest()
{
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

    return true;

}

bool DataBase::createTableMode(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards

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
    return  query.exec(stringQuery);


}

bool DataBase::populateTableMode(const bool NoTmp)
{
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
    return true;
}


bool DataBase::createTableBand(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards

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

/*
    qres = query.exec("CREATE TABLE band ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "lower REAL NOT NULL, "
                "upper REAL NOT NULL, "
                "cabrillo VARCHAR(6) NOT NULL, "
                "name VARCHAR(40) NOT NULL, "
                "UNIQUE (lower, upper, cabrillo, name) )");
*/

        stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                            "lower REAL NOT NULL, "
                                            "upper REAL NOT NULL, "
                                            "cabrillo VARCHAR(6) NOT NULL, "
                                            "name VARCHAR(40) NOT NULL, "
                                            "UNIQUE (lower, upper, cabrillo, name) )");
    return  query.exec(stringQuery);


}

bool DataBase::populateTableBand(const bool NoTmp)
{
    // Cabrillo definition: http://wwrof.org/cabrillo/cabrillo-specification-v3/

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

    return true;
}


bool DataBase::populatePropagationModes()
{
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

    return true;
}


bool DataBase::populateContestData()
{
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


    return true;
}

bool DataBase::howManyQSOsInLog(const int i)
{

    //qDebug() << "DataProxy_SQLite::haveAtLeastOneLog()" << endl;
    QSqlQuery query;
    QString sqlQueryString = QString("SELECT COUNT(id) from log WHERE lognumber='%1'").arg(i);

    if (query.exec(sqlQueryString))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

bool DataBase::updateTo006()
{// Updates the DB to 0.0.6
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
    qDebug() << "DataBase::updateTo006" << endl;
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
        while (!IAmIn005 && !ErrorUpdating)
        {

            IAmIn005 = true;
        }
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
                qDebug() << "DataBase::updateTo006 - prop_mode_enumeration NOT DROPED" << endl;
            }

            sqlOk = createTablePropModes();
            if (!sqlOk)
            {
                qDebug() << "DataBase::updateTo006 - createTablePropModes FALSE" << endl;
            }
            sqlOk = populatePropagationModes();
            if (!sqlOk)
            {
                qDebug() << "DataBase::updateTo006 - populatePropagationModes FALSE" << endl;
            }
            sqlOk = updateTableLog(6);

            if (!sqlOk)
            {
                qDebug() << "DataBase::updateTo006 - prop_mode table do not created" << endl;
            }

            createTableBand(false);
            populateTableBand(false);
            updateBandIdTableLogToNewOnes();
            if (query.exec("DROP TABLE band"))
            {
                if (query.exec("ALTER TABLE bandtemp RENAME TO band"))
                {

                }
                else
                {
                    qDebug() << "DataBase::updateTo006 - ERROR - bandtemp not renamed" << endl;
                }

            }
            else
            {
                qDebug() << "DataBase::updateTo006 - ERROR - bandtemp not dropped" << endl;
            }

            createTableMode(false);
            populateTableMode(false);

            updateModeIdFromSubModeId();


            if (query.exec("DROP TABLE mode"))
            {
                if (query.exec("ALTER TABLE modetemp RENAME TO mode"))
                {

                }
                else
                {
                    qDebug() << "DataBase::updateTo006 - ERROR - modetemp not renamed" << endl;
                }

            }
            else
            {
                qDebug() << "DataBase::updateTo006 - ERROR - modetemp not dropped" << endl;
            }





            createTableClubLogStatus();
            populateTableClubLogStatus();

        }
        else
        { // Version not updated

        }
        //DO ALL THE TASKS TO BE IN 0.006 from 0.005 HERE and set ErrorUpdating if it is not possible.
        IAmIn006 = true;
    }
    return IAmIn006;
}

bool DataBase::updateTableLog(const int _v)
{
    qDebug() << "DataBase::updateTableLog " << endl;
    createTableLog(false);
    QString queryString;
    switch (_v)
    {
    case 6:
        queryString = QString ("INSERT INTO logtemp (qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, srx, stx, points, multiplier, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, band_rx, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_gridsquare, my_iota, my_iota_island_id, my_lat, my_lon, my_name, my_rig, my_sig, my_sig_info, my_state, my_street, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, srx_string, stx_string, state, station_callsign, swl, ten_ten, tx_pwr, web, qso_date_off, time_off, transmiterid, marked, lognumber) SELECT qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, srx, stx, points, multiplier, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, band_rx, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_gridsquare, my_iota, my_iota_island_id, my_lat, my_lon, my_name, my_rig, my_sig, my_sig_info, my_state, my_street, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, srx_string, stx_string, state, station_callsign, swl, ten_ten, tx_pwr, web, qso_date_off, time_off, transmiterid, marked, lognumber FROM log");
    break;
    default:
        return false;
    break;
    }


    QSqlQuery query;
    if (query.exec(queryString))
    {
        qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Query executed" << endl;
        queryString = "DROP TABLE log";
        if (query.exec(queryString))
        {
            qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Table log dropped" << endl;

            queryString = "ALTER TABLE logtemp RENAME TO log" ;
            if (query.exec(queryString))
            {
                qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - tmp renamed" << endl;
                return true;
            }
            else
            {
                qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Renaming failed" << endl;
            }
        }
        else
        {
            qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - Table log Not dropped" << endl;
        }
    }
    else
    {
        qDebug() << "DataBase::updateTableLog: " << QString::number(_v) << " - query failed" << endl;
    }
    return false;
}


bool DataBase::createTableClubLogStatus()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE clublog_status ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "shortname VARCHAR(1) NOT NULL, "
               "name VARCHAR(15) NOT NULL)");
}

bool DataBase::populateTableClubLogStatus()
{
    QSqlQuery query;

    if (query.exec("INSERT INTO clublog_status (shortname, name) VALUES ('Y', 'Uploaded')"))
    {
        if (query.exec("INSERT INTO clublog_status (shortname, name) VALUES ('N', 'Do not upload')"))
        {
            return query.exec("INSERT INTO clublog_status (shortname, name) VALUES ('M', 'Modified')");
        }
    }
    return false;

}

bool DataBase::moveFromModeIdToSubmodeId()
{

    return false;
}

bool DataBase::updateModeIdFromSubModeId()
{
    qDebug() << "DataBase::updateModeIdFromSubModeId: "  << endl;
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
        return false;
    }

    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating mode information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

    sqlOk = query.exec("SELECT modeid, id FROM log");
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

                modetxt = getModeNameFromID2(modeFound);
                //qDebug() << "DataBase::updateModeIdFromSubModeId: mode found: " << modetxt << endl;

                sq = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(modetxt);
                sqlOk2 = query2.exec(sq);
                if (sqlOk2)
                {
                    if (query2.next())
                    {
                        if (query2.isValid())
                        {
                            modeFound = query2.value(0).toInt();
                            sq = QString ("UPDATE log SET modeid='%1' WHERE id='%2'").arg(modeFound).arg(id);
                            sqlOk3 = query3.exec(sq);
                            if (sqlOk3)
                            {
                                //qDebug() << "DataBase::updateModeIdFromSubModeId: ID: " << QString::number(id) << " updated to: " << QString::number(modeFound) <<"/"<< modetxt << endl;
                            }
                            else
                            {
                                //qDebug() << "DataBase::updateModeIdFromSubModeId: ID: " << QString::number(id) << " NOT updated-2"  << endl;
                            }


                        }
                        else
                        {
                            //qDebug() << "DataBase::updateModeIdFromSubModeId: query2 not valid "   << endl;
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
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }

}


bool DataBase::updateBandIdTableLogToNewOnes()
{
    qDebug() << "DataBase::updateBandIdTableLogToNewOnes: "  << endl;

    QString bandtxt = QString();

    bool cancel = false;
    bool alreadyCancelled = false;


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

    QProgressDialog progress(QObject::tr("Updating bands information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);



    sqlOk = query.exec("SELECT bandid, id FROM log");
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
            return false;
        }
        //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FINISHED OK"  << endl;
        return true;
    }
    else
    {
        return false;
    }

}
