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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "database.h"
//#include <qDebug>

DataBase::DataBase(const QString &_parentClass, const QString &_DBName)
{
    Q_UNUSED(_parentClass);
    //qDebug() << Q_FUNC_INFO << _parentClass << " / Name = " << _DBName ;
    logLevel = None;
    constrid = 1;
    created = false;
    //qDebug() << Q_FUNC_INFO <<  " 001";
    util = new Utilities(Q_FUNC_INFO);
    //qDebug() << Q_FUNC_INFO <<  " 003";
    softVersion = util->getVersion();
    //qDebug() << Q_FUNC_INFO <<  " 004";
    dbName = _DBName;
    //qDebug() << Q_FUNC_INFO <<  " dbName: " << dbName ;
    dbVersion = DBVersionf;
    //qDebug() << Q_FUNC_INFO <<  " 005";
    exe = new QueryExecutor(Q_FUNC_INFO);
    if (!createConnection(QString(Q_FUNC_INFO)+"1"))
    {
        //qDebug() << Q_FUNC_INFO <<  " 006:Could not create a connection";
        return;
    }
    //qDebug() << Q_FUNC_INFO <<  " 010";
    //qDebug() << Q_FUNC_INFO << " - connection Name: " << dbConnectionName ;
    //qDebug() << Q_FUNC_INFO << " - DB Name: " << db.databaseName() ;
    insertPreparedQueries.clear();
    insertQueryFields.clear();
    //qDebug() << Q_FUNC_INFO << " - END" ;
}

DataBase::DataBase(const QString &_parentClass, const QString &_softVersion, const QString &_DBName)
{
    Q_UNUSED(_parentClass);
    //qDebug() << "DataBase::DataBase2: " << _parentClass << "/" << _softVersion << " / Name = " << _DBName ;
    //TODO: Sometimes the DB is created without the proper calling (without passing softVersion)
    logLevel = None;
    constrid = 2;
    created = false;
    dbVersion = DBVersionf;
    softVersion = _softVersion;
    //inMemoryOnly = inmemoryonly;
    latestReaded = 0.0f;
    util = new Utilities(Q_FUNC_INFO);
    util->setVersion(softVersion);

    dbName = _DBName;

    if (util->getVersionDouble()>0)
    {
        if (!createConnection(QString(Q_FUNC_INFO)+"2"))
            return;
    }
       //qDebug() << "DataBase::DataBase: - connection Name: " << dbConnectionName ;
       //qDebug() << "DataBase::DataBase: - DB Name: " << db.databaseName() ;
    insertPreparedQueries.clear();
    insertQueryFields.clear();
       //qDebug() << "DataBase::DataBase2: END"  ;
}

DataBase::~DataBase()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    delete(util);
    logEvent(Q_FUNC_INFO, "END", Debug);
         //qDebug() << "DataBase::~DataBase"  ;
}


QString DataBase::getSoftVersion()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QSqlQuery query;

    QString stringQuery ("SELECT MAX (softversion) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            logEvent(Q_FUNC_INFO, "END-1", Debug);
            QString aux = (query.value(0)).toString();
            query.finish();
            return aux;
        }
        else
        {
            query.finish();
            logEvent(Q_FUNC_INFO, "END-2", Debug);
            return QString();
        }
    }
    else
    { //ERROR in Query execution
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent(Q_FUNC_INFO, "END-3", Debug);
        return QString();
    }
}

float DataBase::getDBVersion()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QSqlQuery query;

    QString stringQuery ("SELECT MAX (dbversion) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            logEvent(Q_FUNC_INFO, "END-1", Debug);
            float version = (query.value(0)).toFloat(&sqlOK);
            query.finish();
            if (sqlOK)
                return version;
            return -1.3;
        }
        else
        {
            query.finish();
            logEvent(Q_FUNC_INFO, "END-2", Debug);
            return -1.1;
        }
    }
    else
    { //ERROR in Query execution
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent(Q_FUNC_INFO, "END-3", Debug);
        return -1.0;
    }
}

/*
 bool DataBase::setDir(const QString &_dir)
{
    dbDir = _dir;
    return true;
}
*/

QString DataBase::getDBName()
{
    logEvent(Q_FUNC_INFO, "Start-EndEND", Debug);
    return db.databaseName();
}

QStringList DataBase::getColumnNamesFromTable(const QString &_tableName)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - Start: " << _tableName;
    QSqlQuery query;

    QString queryString;
    if (_tableName == "log")
    {
        queryString = "PRAGMA table_info(log)";
    }
    else
    {
        return QStringList();
    }
    //query.prepare(queryString);
    //query.bindValue(":table", _tableName);

    bool sqlOK = query.exec(queryString);
    //qDebug() << Q_FUNC_INFO << " - last query: " << query.lastQuery();
    QStringList list;

    if (sqlOK)
    {
        //qDebug() << Q_FUNC_INFO << " - query: OK";
        while (query.next())
        {
            //qDebug() << Q_FUNC_INFO << " - column: " << query.value(1).toString();
            QString columnName = query.value(1).toString();
            if ((!columnName.isEmpty()) && (columnName.toUpper() != "ID"))
            {
                list << columnName;
            }
        }
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - query: NOK";
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }

    query.finish();
    //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
    return list;
}


void DataBase::compress()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //QSqlDatabase db = QSqlDatabase::database();
    if (!db.open()) { /* Flawfinder: ignore */
        QMessageBox::warning(nullptr, QObject::tr("Database Error"),
                             db.lastError().text());
    }
   else
    {
        QSqlQuery query("VACUUM;");
        query.exec("VACUUM;");
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

bool DataBase::reConnect(const QString &_DBName)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    db.close();
    dbName = _DBName;
        //qDebug() << "DataBase::reConnect: DB closed"  ;
        //qDebug() << "DataBase::reConnect: DB: " << dbDir  ;
    bool sqlOK =  createConnection(Q_FUNC_INFO);
    if (!sqlOK)
    {
   // emit debugLog(Q_FUNC_INFO, "1", 7);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
    return sqlOK;
}

bool DataBase::createConnection(const QString &function, bool newDB)
{
    //qDebug() << Q_FUNC_INFO <<  " - Start";
    //qDebug() << Q_FUNC_INFO <<  " :" << function << "-" << QString::number(dbVersion) << "/" << softVersion ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    Q_UNUSED(function);
    Q_UNUSED(newDB);
    QString stringQuery;
    QSqlQuery query;
    //qDebug() << Q_FUNC_INFO << ": check if open";

    if (!db.isOpen())
    {
       //qDebug() << Q_FUNC_INFO << ": DB NOT Opened" ;
        if (!db.isValid())
        {
            //qDebug() << Q_FUNC_INFO << ": DB is not valid, let's call addDataBase"  ;
            if (!db.isOpen())
            {
                //qDebug() << Q_FUNC_INFO << ": DB is NOT open, let's open: connection name" << db.connectionName();
                QSqlDatabase::removeDatabase("qt_sql_default_connection");
                db = QSqlDatabase::addDatabase("QSQLITE");
            }

            //qDebug() << Q_FUNC_INFO << ": Now we call setDatabaseName"  ;
            db.setDatabaseName(dbName);
            //qDebug() <<Q_FUNC_INFO << ": end of not valid"  ;
        }
        //qDebug() << Q_FUNC_INFO << ": end of valid check, let's try if it is open"  ;
        if (!db.open()) /* Flawfinder: ignore */
        {
            //qDebug() << Q_FUNC_INFO << ": Not open "  ;
            QMessageBox::warning(nullptr, QObject::tr("Database Error"), db.lastError().text());
            //qDebug() << Q_FUNC_INFO << ": DB creation ERROR"  ;
            // emit debugLog(Q_FUNC_INFO, "1", 7);
            logEvent(Q_FUNC_INFO, "END-1", Debug);
            return false;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << ": created and opened after the creation" ;
            if (!isTheDBCreated())
            {
                //qDebug() << Q_FUNC_INFO << ": DB is still not created"  ;
                if (!createDataBase())
                {
                    //qDebug() << Q_FUNC_INFO << ":DB creation failed!!"  ;
                    logEvent(Q_FUNC_INFO, "END-2", Debug);
                    return false;
                }
                //qDebug() << Q_FUNC_INFO << ": After creation"  ;
                setPragma();
            }
        }
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": No Error, DB is open";
    }
    //qDebug() << Q_FUNC_INFO << ": Going to run - createBandModeMaps " ;

    if (!createBandModeMaps())
    {
        //qDebug() << Q_FUNC_INFO << ": createBandModeMaps false Stop";
        logEvent(Q_FUNC_INFO, "END-3", Debug);
        return false;
    }

    logEvent(Q_FUNC_INFO, "END", Debug);
    //qDebug() << Q_FUNC_INFO <<  " - END";
    return unMarkAllQSO();
}

bool DataBase::setPragma()
{
    //qDebug() << Q_FUNC_INFO << " - Start"  ;

    if (!execQuery(Q_FUNC_INFO, "PRAGMA main.page_size = 4096;"))
        return false;

    if (!execQuery(Q_FUNC_INFO, "PRAGMA main.cache_size=10000;"))
        return false;

    if (!execQuery(Q_FUNC_INFO, "PRAGMA main.synchronous=NORMAL;"))
        return false;

    if (!execQuery(Q_FUNC_INFO, "PRAGMA main.journal_mode=WAL;"))
        return false;

    if (!execQuery(Q_FUNC_INFO, "PRAGMA main.cache_size=5000;"))
        return false;

    if (!execQuery(Q_FUNC_INFO, "PRAGMA synchronous=OFF;"))
        return false;

    if (!execQuery(Q_FUNC_INFO, "PRAGMA main.temp_store = MEMORY;"))
        return false;
    //qDebug() << Q_FUNC_INFO << " - END"  ;
    return execQuery(Q_FUNC_INFO, "PRAGMA case_sensitive_like=OFF;");
}

bool DataBase::isTheDBCreated()
{
    //qDebug() << "DataBase::isTheDBCreated: Called from: " << QString::number(constrid)  ;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QSqlQuery query;
    int _num = 0;
    //QString stringQuery ("SELECT name FROM sqlite_sequence WHERE name='softwarecontrol'");
    QString stringQuery ("SELECT count(id) FROM softwarecontrol");
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return false;
    }

    if (!query.next())
    {
        query.finish();
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return false;
    }

    if (!query.isValid())
    {
        query.finish();
        logEvent(Q_FUNC_INFO, "END-3", Debug);
        return false;
    }

    //qDebug() << "DataBase::isTheDBCreated - valid"  ;
    _num = (query.value(0)).toInt();
    query.finish();

    return (_num>0);
}

bool DataBase::recreateTableLog()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    qDebug() << Q_FUNC_INFO << " - Start";
    if (!createTableLog(false))         // Create modetemp
    {
    qDebug() << Q_FUNC_INFO << ": CreateTableLog returned false" ;
       logEvent(Q_FUNC_INFO, "END-1", Debug);
        return false;
    }
    qDebug() << Q_FUNC_INFO << " - 10";
    QString queryString;
    qDebug() << Q_FUNC_INFO << " - 11";
    queryString.clear();
    qDebug() << Q_FUNC_INFO << " - 12";
    QStringList columns;
    qDebug() << Q_FUNC_INFO << " - 13";
    columns.clear();
    qDebug() << Q_FUNC_INFO << " - 14";
    columns << getColumnNamesFromTable("log");
    qDebug() << Q_FUNC_INFO << " - 15";
    queryString =  columns.first();

    qDebug() << Q_FUNC_INFO << " - 20";
    for (int i=1;i<columns.size()-1;i++)
    {
        if ( !(columns.at(i) == "time_on") && !(columns.at(i) == "time_off")  )
        {
            queryString = queryString + ", " + columns.at(i);
        }
    }
    //qDebug() << Q_FUNC_INFO << " - 30";
    queryString = "INSERT INTO logtemp (" + queryString + ", " + columns.last() + ") SELECT " + queryString + ", " + columns.last() + " FROM log";

    if (!execQuery(Q_FUNC_INFO, queryString))
    {
        //qDebug() << Q_FUNC_INFO << ": ERROR - Data not moved" ;
        logEvent(Q_FUNC_INFO, "END-4", Debug);
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - 40";
    if (!execQuery(Q_FUNC_INFO, "DROP table log"))
    {
        //qDebug() << Q_FUNC_INFO << ": ERROR - log table not dropped" ;
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - 50";
    return execQuery(Q_FUNC_INFO, "ALTER TABLE logtemp RENAME TO log");
}

bool DataBase::createTableLog(bool temp)
{ //Creates a temporal table or the normal one.
    logEvent(Q_FUNC_INFO, "Start", Debug);
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    QString stringQuery = QString();
    if (temp)
    {
        stringQuery = "CREATE TABLE log" ;
        //qDebug() << Q_FUNC_INFO << ": log" ;
    }
    else
    {
        stringQuery = "CREATE TABLE logtemp" ;
        //qDebug() << Q_FUNC_INFO << ": logtemp" ;
    }
             stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "qso_date DATETIME NOT NULL, " // 2020-01-01 10:12:01
             "call VARCHAR(40) NOT NULL, "
             "rst_sent VARCHAR, "
             "rst_rcvd VARCHAR, "
             "bandid INTEGER NOT NULL, "
             "modeid INTEGER NOT NULL, "
             "cqz INTEGER, "
             "dxcc INTEGER, "
             "address VARCHAR, "
             "age REAL, "
             "a_index REAL, "
             "altitude REAL, "
             "ant_az REAL, "
             "ant_el REAL, "
             "ant_path INTEGER, "
             "arrl_sect VARCHAR(3), "
             "award_submitted VARCHAR, "
             "award_granted VARCHAR, "
             "band_rx INTEGER, "
             "checkcontest VARCHAR, "
             "class VARCHAR, "
             "clublog_qso_upload_date DATETIME, "
             "clublog_qso_upload_status VARCHAR(1), "
             "cnty VARCHAR, "
             "comment VARCHAR, "
             "cont VARCHAR(2), "
             "contacted_op VARCHAR(40), "
             "contest_id VARCHAR, "
             "country VARCHAR, "
             "credit_submitted VARCHAR, "
             "credit_granted VARCHAR, "
             "darc_dok VARCHAR,"
             "distance REAL, "
             "email VARCHAR, "
             "eq_call VARCHAR, "
             "eqsl_qslrdate DATETIME, "
             "eqsl_qslsdate DATETIME, "
             "eqsl_qsl_rcvd VARCHAR(1), "
             "eqsl_qsl_sent VARCHAR(1), "
             "fists INTEGER, "
             "fists_cc INTEGER, "
             "force_init INTEGER, "
             "freq VARCHAR, "
             "freq_rx VARCHAR, "
             "gridsquare VARCHAR, "
             "gridsquare_ext VARCHAR, "
             "hamlogeu_qso_upload_date DATETIME, "
             "hamlogeu_qso_upload_status VARCHAR(1), "
             "hamqth_qso_upload_date DATETIME, "
             "hamqth_qso_upload_status VARCHAR(1), "
             "hrdlog_qso_upload_date DATETIME, "
             "hrdlog_qso_upload_status VARCHAR(1), "
             "iota VARCHAR(6), "
             "iota_island_id INTEGER, "
             "ituz INTEGER, "
             "k_index INTEGER, "
             "lat VARCHAR(11), "
             "lon VARCHAR(11), "
             "lotw_qslrdate DATETIME, "
             "lotw_qslsdate DATETIME, "
             "lotw_qsl_rcvd VARCHAR(1), "
             "lotw_qsl_sent VARCHAR(1), "
             "max_bursts INTEGER, "
             "ms_shower VARCHAR, "
             "my_altitude REAL, "
             "my_antenna VARCHAR, "
             "my_arrl_sect VARCHAR(3), "
             "my_city VARCHAR, "
             "my_cnty VARCHAR, "
             "my_country INTEGER, "
             "my_cq_zone INTEGER, "
             "my_dxcc INTEGER, "
             "my_fists INTEGER, "
             "my_gridsquare VARCHAR, "
             "my_gridsquare_ext VARCHAR, "
             "my_iota VARCHAR(6), "
             "my_iota_island_id INTEGER, "
             "my_itu_zone INTEGER, "
             "my_lat VARCHAR(11), "
             "my_lon VARCHAR(11), "
             "my_name VARCHAR, "
             "my_pota_ref VARCHAR, "
             "my_postal_code VARCHAR ,"
             "my_rig VARCHAR, "
             "my_sig VARCHAR, "
             "my_sig_info VARCHAR, "
             "my_sota_ref VARCHAR, "
             "my_state VARCHAR, "
             "my_street VARCHAR, "
             "my_usaca_counties VARCHAR, "
             "my_vucc_grids VARCHAR, "
             "my_wwff_ref VARCHAR(11), "
             "name VARCHAR, "
             "notes VARCHAR, "
             "nr_bursts INTEGER, "
             "nr_pings INTEGER, "
             "operator VARCHAR, "
             "owner_callsign VARCHAR, "
             "pfx VARCHAR, "
             "pota_ref VARCHAR, "
             "precedence VARCHAR, "
             "prop_mode VARCHAR, "
             "public_key VARCHAR, "
             "qrzcom_qso_upload_date DATETIME, "
             "qrzcom_qso_upload_status VARCHAR(1), "
             "qslmsg VARCHAR, "
             "qslrdate DATETIME, "
             "qslsdate DATETIME, "
             "qsl_rcvd VARCHAR(1), "
             "qsl_sent VARCHAR(1), "
             "qsl_rcvd_via VARCHAR(1), "
             "qsl_sent_via VARCHAR(1), "
             "qsl_via VARCHAR, "
             "qso_complete INTEGER, "
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
             "srx INTEGER, "
             "stx_string VARCHAR, "
             "stx INTEGER, "
             "state VARCHAR, "
             "station_callsign VARCHAR, "
             "submode VARCHAR, "
             "swl INTEGER, "
             "ten_ten INTEGER, "
             "tx_pwr REAL, "
             "uksmg INTEGER, "
             "usaca_counties VARCHAR, "
             "ve_prov VARCHAR, "
             "vucc_grids VARCHAR, "
             "web VARCHAR, "
             "wwff_ref VARCHAR(11), "
             "qso_date_off DATETIME, " //2020-01-01
             "marked VARCHAR(1), "
             "lognumber INTEGER NOT NULL, "

             "UNIQUE (call, qso_date, bandid, modeid, lognumber), "
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
             "FOREIGN KEY (my_arrl_sect) REFERENCES arrl_sect_enumeration, "
             "FOREIGN KEY (band_rx) REFERENCES band (id), "
             "FOREIGN KEY (modeid) REFERENCES mode (id), "
             "FOREIGN KEY (submode) REFERENCES mode (id), "
             "FOREIGN KEY (dxcc) REFERENCES entity (dxcc), "
             "FOREIGN KEY (bandid) REFERENCES band (id))");

    //qDebug() << Q_FUNC_INFO << ": " << stringQuery;

    if (execQuery(Q_FUNC_INFO, stringQuery))
    {
        //qDebug() << Q_FUNC_INFO << ": Query OK";
        logEvent(Q_FUNC_INFO, "END-1", Debug);
        return true;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": Query NOK";
        logEvent(Q_FUNC_INFO, "END-2", Debug);
        return false;
    }
}

bool DataBase::createDataBase()
{
    logEvent(Q_FUNC_INFO, "Start: " +  QString::number(constrid) , Debug);
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

    if  ( (!createTableBand(true))                      ||
          (!populateTableBand(true))                    ||
          (!createTableMode(true))                      ||
          (!populateTableMode(true))                    ||
          (!createTableSatellites(true))                ||
          (!populateTableSatellites(true))              ||
          (!createTableLog(true))                       ||
          (!createTableEntity(true))                    ||
          (!createTablePrimarySubdivisions(true))       ||
          (!createAndPopulateARRLSectEnumeration())     ||
          //(!createAndPopulateQSO_CompleteEnumeration()) ||
          (!createTableQSL_Via_enumeration())           ||
          (!populateTableQSL_Via_enumeration())         ||
          (!createTablePropModes())                     ||
          (!createTableLogs(true))                      ||
          (!createTableClubLogStatus())                 ||
          (!populateTableClubLogStatus())               ||
          (!createAndPopulateQSLSentRecStatus())        ||
          (!createAndPopulateContinents())              ||
          (!createAndPopulateAnt_path_enumeration())    ||
          (!createTableContest())                       ||
          (!populateContestData())                      ||
          (!createAndPopulateAwardEnumeration())        ||
          (!populatePropagationModes())

        )
        return false;



      stringQuery = QString("CREATE TABLE contestcategory ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(20) NOT NULL, "
                 "name VARCHAR(40) NOT NULL)");
    execQuery(Q_FUNC_INFO, stringQuery);


      stringQuery = QString("CREATE TABLE prefixesofentity ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "prefix VARCHAR(15) NOT NULL,"
                 "dxcc INTEGER NOT NULL,"
                 "cqz INTEGER NOT NULL,"
                 "ituz INTEGER NOT NULL,"
                 "UNIQUE (prefix, dxcc), "
                 "FOREIGN KEY (dxcc) REFERENCES entity (dxcc) )");
    execQuery(Q_FUNC_INFO, stringQuery);


    if (updateDBVersion(softVersion, QString::number(DBVersionf)))
    { // It was not possible to save the DB version
        //qDebug() << Q_FUNC_INFO << ": Not possible to update the DB version";
        logEvent(Q_FUNC_INFO, "END-17", Debug);
        return false;
    }

    logEvent(Q_FUNC_INFO, "END", Debug);
    return true;
}


bool DataBase::createTablePrimarySubdivisions(const bool NoTmp)
{
    // NoTmp = false => TMP data table to operate and be deleted afterwards
    //qDebug() << Q_FUNC_INFO ;
/*
* prefnumber (id autoincrement)
* subdivision main entity (arrlid)  : dxcc          : 281
* subdivision main prefix           : prefix        : EA4
* subdivision shortname             : shortname     : M
* subdivision name                  : name          : Madrid
* subdivision group                 : regionalgroup : Comunidad de Madrid
* subdivisional id                  : regionalid    : -1 (oblast for Russia)
* subdivision CQ (if different)     : cqz           : 14
* subdivision ITU (if different)    : ituz          : 37
* subdivision deleted               : deleted       : N
* subdivision start_date            : start_date    : N/A
* subdivision end_date              : end_date      : N/A
*
*/
    QString stringQuery = QString();
    QString table = QString();
    if (NoTmp)
    {
        table = "primary_subdivisions" ;
    }
    else
    {
        table = "primary_subdivisionstemp" ;
    }

    stringQuery = "CREATE TABLE "+ table;

    stringQuery = stringQuery + QString(" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "dxcc INTEGER NOT NULL, "       // arrlId (281)
        "name VARCHAR NOT NULL, "       // Madrid
        "shortname VARCHAR NOT NULL, "  // M
        "prefix VARCHAR, "              // EA4
        "cqz INTEGER NOT NULL, "        // 14
        "ituz INTEGER NOT NULL, "       // 37
        "regionalgroup VARCHAR, "       // Comunidad de Madrid
        "regionalid INTEGER, "          // Oblast in Russia or any other Id
        "start_date DATETIME, "
        "end_date DATETIME, "
        "deleted VARCHAR, "
        "UNIQUE (id, shortname, name), "
        "FOREIGN KEY (cqz) REFERENCES entity (cqz), "
        "FOREIGN KEY (ituz) REFERENCES entity (ituz), "
        "FOREIGN KEY (dxcc) REFERENCES entity (dxcc) )");

    QString delS = QString();
    delS = "DROP TABLE IF exists " + table;
    execQuery(Q_FUNC_INFO, delS);
    //qDebug() << Q_FUNC_INFO << " Dropped table: " << table ;
    return execQuery(Q_FUNC_INFO, stringQuery);

    //qDebug() << Q_FUNC_INFO << " - END" ;
}

bool DataBase::populateTablePrimarySubdivisions()
{
    if (!doesEntityTablehaveData())
        return false;
    DB_ADIF_Primary_Subdvisions_data primarySubDivisions(Q_FUNC_INFO);
    return primarySubDivisions.addData();
}

bool DataBase::recreateTablePrimarySubdivisions()
{
    //qDebug() << Q_FUNC_INFO ;
    QSqlQuery query;

    if (isTheTableExisting("primary_subdivisions"))
    {
        if (execQuery(Q_FUNC_INFO, "DROP TABLE primary_subdivisions"))
        {
            if (createTablePrimarySubdivisions(true))
            {
                    //qDebug() << Q_FUNC_INFO << ": primary_subdivisions table created"  ;
                return populateTablePrimarySubdivisions();
            }
            else
            {
                     //qDebug() << Q_FUNC_INFO << ": primary_subdivisions table NOT created"  ;
            }
        }
        else
        {
               return false;//qDebug() << Q_FUNC_INFO << ": execQuery FAILED"  ;
        }
    }
    else
    {
        if (createTablePrimarySubdivisions(true))
        {
                //qDebug() << Q_FUNC_INFO << ": primary_subdivisions table created"  ;
            return populateTablePrimarySubdivisions();
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << ": primary_subdivisions table NOT created"  ;
        }
    }
       //qDebug() << Q_FUNC_INFO << ": END FALSE"  ;
    return false;
}

int DataBase::getBandIdFromName(const QString &b)
{
    //qDebug() << Q_FUNC_INFO << ": " << b ;
    QString band = b.toUpper();

    QString queryString = QString("SELECT id FROM band WHERE name= :band");
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":band", b);

    bool sqlOK = query.exec();

    if (!sqlOK)
        return -1;
    if (!query.next())
        return -2;
    if (!query.isValid())
        return -3;

    int v = (query.value(0)).toInt();
    query.finish();
    return v;
}

int DataBase::getModeIdFromName(const QString &b)
{
    //qDebug() << Q_FUNC_INFO << ": " << b ;

    //qDebug() << Q_FUNC_INFO << ": " << b ;
    QString band = b.toUpper();

    QString queryString = QString("SELECT id FROM mode WHERE name= :mode");
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":mode", b);

    bool sqlOK = query.exec();

    if (!sqlOK)
        return -1;
    if (!query.next())
        return -2;
    if (!query.isValid())
        return -3;

    int v = (query.value(0)).toInt();
    query.finish();
    return v;
}

int DataBase::getModeIdFromSubMode(const QString &b)
{
    //qDebug() << Q_FUNC_INFO << ": " << b ;
    //qDebug() << Q_FUNC_INFO << ": " << b ;
    QString band = b.toUpper();

    QString queryString = QString("SELECT id FROM mode WHERE submode= :submode");
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":submode", b);

    bool sqlOK = query.exec();

    if (!sqlOK)
        return -1;
    if (!query.next())
        return -2;
    if (!query.isValid())
        return -3;

    int v = (query.value(0)).toInt();
    query.finish();
    return v;
}

QString DataBase::getBandNameFromNumber(const int _n)
{
    //qDebug() << "DataBase::getBandNameFromNumber: " << QString::number(_n) ;
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
                    //qDebug() << "DataBase::getBandNameFromNumber: " << (query.value(0)).toString() << "-------- END" ;
                return (query.value(0)).toString();
            }
            else
            {
                   //qDebug() << "DataBase::getBandNameFromNumber: " << "-------- END-1" ;
                query.finish();
                return QString();
            }
        }
        else
        {
            //qDebug() << "DataBase::getBandNameFromNumber: " << "-------- END-2" ;
            query.finish();
            return QString();
        }
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return QString();
}



QString DataBase::getModeNameFromNumber(const int _n, bool _tmp)
{
    //TODO May fail to identify the sumbode(mode/modetemp... (Review STEP-2 o 3)
        //qDebug() << "DataBase::getModeNameFromNumber: " << QString::number(_n) ;
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
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    query.next();
         //qDebug() << "DataBase::getModeNameFromNumber: " << QString::number(_n) <<" - " <<  isValidMode((query.value(0)).toString(), _tmp) ;
    if ( query.isValid() )
    {
            //qDebug() << "DataBase::getModeNameFromNumber: ------ END-1" ;
        return (query.value(0)).toString();
        /* In a version when I change the mode table to include submode, this comparison may need to be checked in both versions
         * at once, failing the query as old version was not having the column submode
         *
        if ( isValidMode((query.value(0)).toString(), _tmp))
        {
                 //qDebug() << "DataBase::getModeNameFromNumber - Found: " << (query.value(0)).toString() ;
            return (query.value(0)).toString();
        }
        else
        {
                 //qDebug() << "DataBase::getModeNameFromNumber - Not Valid Mode: " << (query.value(0)).toString()  ;
            return QString();
        }
        */
    }
    else
    {
             //qDebug() << "DataBase::getModeNameFromNumber - Not Valid record"  ;
            //qDebug() << "DataBase::getModeNameFromNumber: ------ END-2" ;
        query.finish();
        return QString();
    }
}

QString DataBase::getSubModeNameFromNumber(const int _n, bool _tmp)
{
       //qDebug() << "DataBase::getSubModeNameFromNumber: " << QString::number(_n) ;
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

   //qDebug() << "DataBase::getSubModeNameFromNumber - query: " << query.lastQuery() ;

    if (sqlOk)
    {
        if (query.next())
        {
            if ( query.isValid() )
            {
                if ( isValidMode((query.value(0)).toString(), _tmp)  )
                {
                        //qDebug() << "DataBase::getSubModeNameFromNumber: RETURN: " << (query.value(0)).toString() ;
                    return (query.value(0)).toString();
                }
                else
                {
                        //qDebug() << "DataBase::getSubModeNameFromNumber: NO valid mode - END" ;
                    query.finish();
                    return QString();
                }
            }
            else
            {
                    //qDebug() << "DataBase::getSubModeNameFromNumber: query not valid - END" ;
                query.finish();
                return QString();
            }
        }
        else
        {
                //qDebug() << "DataBase::getSubModeNameFromNumber: query not next - END" ;
            query.finish();
            return QString();
        }
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug() << "DataBase::getSubModeNameFromNumber: SQL FALSE - END" ;
        query.finish();
        return QString();
    }
    //qDebug() << "DataBase::getSubModeNameFromNumber: - END-X" ;
    //query.finish();
    //return QString();
}

bool DataBase::isValidBand (const QString &b)
{
    //qDebug() << Q_FUNC_INFO << ": " << b ;
    //qDebug() << Q_FUNC_INFO << ": bandId: " << b ;
    //qDebug() << "DataBase::isValidBand: " << QString::number(getBandIdFromName(b));

    return (getBandIdFromName(b)>0);
   /*
    QString _band = b.toUpper();

    QSqlQuery query;
    QString stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(_band);
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
           // emit debugLog(Q_FUNC_INFO, "2", 7);
            return false;
        }
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
   //emit debugLog(Q_FUNC_INFO, "3", 7);
    return false;
    */
}

bool DataBase::isValidMode (const QString &b, const bool _tmp)
{
    //qDebug() << Q_FUNC_INFO << ": " << b ;
    if (b.length()<2)
    {
       //qDebug() << Q_FUNC_INFO << ": (length<2) FALSE"  ;
       //emit debugLog(Q_FUNC_INFO, "1", 7);
        return false;
    }

    if (!_tmp)
        return (getModeIdFromSubMode(b)>0);
    QString stringQuery = QString("SELECT id FROM modetemp WHERE submode= :submode");
    QSqlQuery query;
    query.bindValue(":submode", b);
    query.prepare(stringQuery);

    bool sqlOK = query.exec();

    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
    }
    query.next();
    return query.isValid();
}

bool DataBase::isValidBandNumber (const int b)
{
    //qDebug() << Q_FUNC_INFO << ": " << QString::number(b);
    return isValidBand(getBandNameFromNumber(b));
}

bool DataBase::isValidModeNumber (const int b)
{
        //qDebug() << Q_FUNC_INFO << ": "  << QString::number(b);
    return isValidMode(getModeNameFromNumber(b, false), false);
}

int DataBase::getBandIdFromFreq(const QString &fr)
{
        //qDebug() << Q_FUNC_INFO << ": "  << fr ;
    //Freq should be in MHz

    QString queryString = QString("SELECT id FROM band WHERE lower <= '%1' and upper >= '%2'").arg(fr, fr);
    QSqlQuery query;

    bool sqlOK = query.exec(queryString);

        //qDebug() << Q_FUNC_INFO << ": Query: " << query.lastQuery() ;
    if (sqlOK)
    {
           //qDebug() << Q_FUNC_INFO << ": Query OK" ;
        query.next();


        if (query.isValid())
        {
                //qDebug() << Q_FUNC_INFO << ": Query OK - END" ;
            return (query.value(0)).toInt();
        }
        else
        {
                //qDebug() << "DataBase::getBandIdFromFreq: Valid NOK - END" ;
            query.finish();
            return -1;
        }
    }
    else
    {
           //qDebug() << "DataBase::getBandIdFromFreq: Query NOK" ;
           //qDebug() << "DataBase::getBandIdFromFreq: Query NOK: " << query.lastError().text() ;
           //qDebug() << "DataBase::getBandIdFromFreq: Query NOK: " << query.lastError().text() ;
        if (query.lastError().isValid())
        {
               //qDebug() << "DataBase::getBandIdFromFreq: Query NOK - Error VALID" ;
        }
        else
        {
               //qDebug() << "DataBase::getBandIdFromFreq: Query NOK - Error NOT-VALID" ;
        }
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        queryErrorManagement(Q_FUNC_INFO, query.lastError().text(), query.lastError().text(), query.lastQuery());
        query.finish();
       return -2;
    }
    //qDebug() << "DataBase::getBandIdFromFreq: END-X" ;
    //query.finish();
    //return -3;
}


bool DataBase::isThisFreqInBand(const QString &b, const QString &fr)
{//Freq should be in MHz
   //qDebug() << Q_FUNC_INFO << ": " << b << "/" << fr ;
    if (b.length()<2)
    {
       //qDebug() << Q_FUNC_INFO << ": returning false" ;
        return false;
    }
    int bandNf = getBandIdFromFreq(fr);
    int bandN = getBandIdFromName(b);
   //qDebug() << Q_FUNC_INFO << ":  (b/f)" << QString::number(bandN) << "/" << QString::number(bandNf) ;
    if (bandNf == bandN)
    {
       //qDebug() << Q_FUNC_INFO << ": OK " << b << "/" << fr ;
        return true;
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << ": NOK " << b << "/" << fr ;
       //emit debugLog(Q_FUNC_INFO, "1", 7);
        return false;
    }
}


bool DataBase::unMarkAllQSO()
{
    //qDebug() << Q_FUNC_INFO;
    QString stringQuery = QString("UPDATE log SET marked = 'N' WHERE 1");
    return execQuery(Q_FUNC_INFO, stringQuery);
}


bool DataBase::updateIfNeeded()
{
    //qDebug() <<Q_FUNC_INFO << "  - Version: " << QString::number(dbVersion) ;

    /**************************************************************************************
     *
     * This function should call to bool updateToXXX () being XXX dbVersion and
     *
     */


    latestReaded = getDBVersion();
    //qDebug() << Q_FUNC_INFO << QString("latestReaded: %1 / dBversion: %2").arg(latestReaded).arg(dbVersion);

    if (latestReaded<0.0)
        return false;

    //qDebug() << Q_FUNC_INFO << "  - LatestReaded: >= 1.0 we can continue" ;

    if (latestReaded >= dbVersion)
    { // DB is updated, no update is needed
        //qDebug() << Q_FUNC_INFO << "  - DB updated (no need to update anything!) " ;
        return true;
    }
    else
    { // DB is outdated. We need to update!!
        //qDebug() << Q_FUNC_INFO << "  - DB outdated... upgrade starts now! " ;
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
                backupB4Update();
            break;
            case QMessageBox::Discard:
            // Discard was clicked
            break;
            default:
            // should never be reached
               //qDebug() << Q_FUNC_INFO << "  - FALSE - CHECK IF SEEN, shoud not be here! - END "  ;
           //emit debugLog(Q_FUNC_INFO, "2", 7);
                return false;
            //break;
        }
    }
    // If the DB needs to be updated... we update it! :-)
     //qDebug() << Q_FUNC_INFO << " - END!" ;
    return true;
}

void DataBase::backupB4Update()
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;
    QMessageBox msgBox;
    msgBox.setWindowTitle("KLog backup");
    msgBox.setText( QObject::tr("Upgrading software may potentially cause problems. Backing up your DB, before upgrading, is always a good idea."));
    msgBox.setInformativeText( QObject::tr("Do you want to backup your DB now?") );
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.raise();
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Popup);

    int ret = msgBox.exec();

    switch (ret)
    {
        case QMessageBox::Yes:
        // Save was clicked
            logBackup();
        break;
        case QMessageBox::No:
        // NO backup was selected
            updateToLatest();
        break;
    }
    //qDebug() << Q_FUNC_INFO << " - END" ;
}

void DataBase::logBackup()
{
    //qDebug() << Q_FUNC_INFO << " - Start" ;

    QFile DBFile(util->getKLogDBFile());
    QString newFile = util->getKLogDBBackupFile();
    bool copied = DBFile.copy(newFile);
    //qDebug() << Q_FUNC_INFO << " copy: " << newFile ;
    QMessageBox msgBox;
    msgBox.setWindowTitle("KLog DB backup");
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Popup);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if (copied)
    {
        //qDebug() << Q_FUNC_INFO << " - DB backup was OK"  ;
        msgBox.setText( QObject::tr("The backup finished successfully."));
        msgBox.setInformativeText( QObject::tr("You can find the backup in this file: %1").arg(newFile) );
        msgBox.setIcon(QMessageBox::Information);
        msgBox.raise();
        msgBox.exec();
        updateToLatest();
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - DB backup was NOK"  ;
        msgBox.setText( QObject::tr("The backup was not properly done."));
        msgBox.setInformativeText( QObject::tr("You will be sent back to the starting point."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.raise();
        msgBox.exec();
        updateIfNeeded();
    }

    //qDebug() << Q_FUNC_INFO << " - END" ;
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
        //qDebug() << "DataBase::createTheBandQuickReference: " ;
    QString st = "NULL";
    int in = 0;

    QString stringQuery = QString("SELECT id, name, lower FROM band");
    QString fr = QString();
    Frequency ff;
    bandIDHash.clear();
    IDBandHash.clear();
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);

    if (!sqlOK)
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
       // emit debugLog(Q_FUNC_INFO, "1", 7);
        return false;
    }

    while (query.next())
    {
        if (query.isValid())
        {
            st = (query.value(1)).toString();
            in = (query.value(0)).toInt();
            fr = (query.value(2)).toString();
            ff.fromDouble((query.value(2)).toDouble());
            bandIDHash.insert(st, in );
            IDBandHash.insert(in, st);
            freqBandIdHash.insert(in, fr);
            ffreqBandIdHash.insert(in, ff);

                 //qDebug() << Q_FUNC_INFO << ": " << st <<"/" << QString::number(in);
        }
        else
        {
          //qDebug() << Q_FUNC_INFO << " Query not valid -'RETURN FALSE - END" ;
          // QMessageBox::warning(0, QObject::tr("Database Error (DataBase::createTheBandQuickReference)"),
          //                      query.lastError().text());
            query.finish();
           // emit debugLog(Q_FUNC_INFO, "2", 7);
           return false;
           //TODO: Manage this error, in case the query is NOK.
        }
              //qDebug() << Q_FUNC_INFO << " Go for the next one!" ;
    }
    query.finish();
        //qDebug() << Q_FUNC_INFO << " - END" ;
    return true;
}


bool DataBase::createTheModeQuickReference()
{
    /*
              KEY      Value
        QHash<QString, int> modeIDHash;
        QHash<int, QString> IDModeHash

    */
          //qDebug() << Q_FUNC_INFO << ": " ;

    if (getDBVersion()<0.01f)
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
            queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            query.finish();
           // emit debugLog(Q_FUNC_INFO, "1", 7);
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
                     //qDebug() << Q_FUNC_INFO << ": " << st <<"/" << QString::number(in);
            }
            else
            {
                   //qDebug() << Q_FUNC_INFO << ": Query not valid - END" ;

               //QMessageBox::warning(0, QObject::tr("Database Error (DataBase::createTheModeQuickReference)"),
               //                     query.lastError().text());
                query.finish();
               // emit debugLog(Q_FUNC_INFO, "2", 7);
               return false;
               //TODO: Manage this error, in case the query is NOK.
            }
        }
        query.finish();
            //qDebug() << Q_FUNC_INFO << " - END" ;
        return true;
}

bool DataBase::createBandModeMaps()
{
    //qDebug() << Q_FUNC_INFO ;
    bool b = false;
    bool m = false;

     //return (b && m);
    if (isTheDBCreated())
    {
        b = createTheBandQuickReference();
        m = createTheModeQuickReference();

             //qDebug() << Q_FUNC_INFO << " - isTheDbCreated TRUE" ;
        if (!b)
        {
           //emit debugLog(Q_FUNC_INFO, "1", 7);
        }
        if (!m)
        {
           //emit debugLog(Q_FUNC_INFO, "2", 7);
        }
        //qDebug() << Q_FUNC_INFO << " END 1" ;
        return (b && m);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - isTheDbCreated FALSE" ;
       //emit debugLog(Q_FUNC_INFO, "3", 7);
        return false;
    }
    //return false;
    //qDebug() << Q_FUNC_INFO << " - END" ;
}

Frequency DataBase::getFreqFromBandId(const int _i)
{
    //qDebug() << Q_FUNC_INFO  ;

    if (ffreqBandIdHash.contains(_i))
    {
        //qDebug() << Q_FUNC_INFO << " OK END" ;
        return ffreqBandIdHash.value(_i);
    }
    return Frequency(0);
}

/*
int DataBase::getLogTypeNumber(const QString &_logType)
{
        //qDebug() << "DataBase::getLogTypeNumber: " << _logType ;
     QSqlQuery query;
     QString queryString = QString("SELECT id FROM supportedcontests WHERE name='%1'").arg(_logType);

     bool sqlOK = query.exec(queryString);


     if(!sqlOK)
     {
         queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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
     //query.finish();
     //return -2;
}

QString DataBase::getLogTypeName(const int _logType)
{
         //qDebug() << "DataBase::getLogTypeName: " << QString::number(_logType) ;
     QSqlQuery query;
     QString queryString = QString("SELECT name FROM supportedcontests WHERE id='%1'").arg(_logType);
     bool sqlOK = query.exec(queryString);

     if(!sqlOK)
     {
         queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
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
     //query.finish();
     //return QString();
}
*/
bool DataBase::updateToLatest()
{
/*
 * With the DB updates, the function that is called from here should be also updated.
 * The updateXXX are recursive calls that calls the previous one.
 * Update float DBVersionf = 0.027f; in database.h to the latest version!
 */
    qDebug() << Q_FUNC_INFO << " - Start";
    if (requiresManualUpgrade())
    {

        //qDebug() << Q_FUNC_INFO << " requires" ;
        exit(1);
        //return false;
    }
    qDebug() << Q_FUNC_INFO << " - Let's update!";
    return updateTo027();
}


bool DataBase::requiresManualUpgrade()
{
    // If DB version <= 0.006, DB can't be upgraded automatically.
    // Recomendation is to export to ADIF in an olf KLog version (before KLog 1.1);
    // Install new KLog version
    // import ADIF file

    //qDebug() << "DataBase::requiresManualUpgrade - ver: " << getDBVersion() << " / " << DBVersionf ;
    if (getDBVersion() >= 0.007f)
    {
        //qDebug() << "DataBase::requiresManualUpgrade false" ;
        return false;
    }
    else
    {
        //qDebug() << "DataBase::requiresManualUpgrade true" ;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle(QObject::tr("KLog - DB can't be updated automatically"));
        QString aux = QObject::tr("You are upgrading from a too old KLog version and this upgrade can't be upgraded automatically from that version.");
        QString aux2 = QObject::tr("The process to upgrade is:\n- Using an old KLog version export your log to ADIF.\n- Remove your logbook.dat file from your KLog folder.\n- Install the new KLog version.\n- Import your ADIF file.\n\nKLog will finish when you click on OK.");

        msgBox.setText(aux);
        msgBox.setDetailedText(aux2);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    return true;
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
       //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << QString::number(latestReaded) ;
    bool IAmIn003 = false;
    bool IAmIn002 = false;
    bool ErrorUpdating = false;

    latestReaded = getDBVersion();
    if (latestReaded >= 0.003f)
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
           // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
        //DO ALL THE TASKS TO BE IN 0.003 from 0.002 HERE and set ErrorUpdating if it is not possible.
        IAmIn003 = true;
    }
    return IAmIn003;
}

bool DataBase::updateTo004()
{// Updates the DB to 0.0.41
       //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
       //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << QString::number(latestReaded) ;
    bool IAmIn004 = false;
    bool IAmIn003 = false;

    QString stringQuery = QString();
    QSqlQuery query;

    bool sqlOk = false;
    latestReaded = getDBVersion();
    if (latestReaded >= 0.004f)
    {
            //qDebug() <<  Q_FUNC_INFO << " -- I am in 004" ;
        return true;
    }
    else
    {
            //qDebug() <<  Q_FUNC_INFO << " -- I am not in 004" ;
        IAmIn004 = false;
    }

    while (!IAmIn004)
    {
           //qDebug() <<  Q_FUNC_INFO << " -- And I am not in 004 nor ErrorUpdating" ;
        while (!IAmIn003)
        {
               //qDebug() <<  Q_FUNC_INFO << " -- And I am not in 003" ;
            //IAmIn002 = updateTo002();
            IAmIn003 = true;
        }
           //qDebug() <<  Q_FUNC_INFO << " -- And I am in 003" ;
        sqlOk = updateDBVersion(softVersion, "0.004");
           //qDebug() <<  Q_FUNC_INFO << " -- Update Version" ;
        if (sqlOk)
        { // Version updated
               //qDebug() <<  Q_FUNC_INFO << " -- Update OK" ;
            sqlOk = execQuery(Q_FUNC_INFO, "DROP TABLE award_enumeration");
        }
        else
        { // Version not updated
               //qDebug() <<  Q_FUNC_INFO << " -- Update NOK" ;
        }
        //DO ALL THE TASKS TO BE IN 0.004 from 0.003 HERE and set ErrorUpdating if it is not possible.
           //qDebug() <<  Q_FUNC_INFO << " -- IAmIn004 = true" ;
        IAmIn004 = true;
    }

    if (IAmIn004)
    {
           //qDebug() <<  Q_FUNC_INFO << " -- Return... TRUE" ;
    }
    else
    {
           //qDebug() <<  Q_FUNC_INFO << " -- Return... FALSE" ;
    }
       //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
    return IAmIn004;
}


bool DataBase::updateTo005()
{// Updates the DB to 0.0.5
          //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
       bool IAmIn005 = false;
       bool IAmIn004 = false;
       bool ErrorUpdating = false;
       QString stringQuery = QString();
       QSqlQuery query;
       QMessageBox msgBox;
       msgBox.setWindowTitle(QObject::tr("KLog - DB update"));
       msgBox.setIcon(QMessageBox::Information);
       //int errorCode;

       bool sqlOk = false;
       latestReaded = getDBVersion();

       if (latestReaded >= 0.005f)
       {
                //qDebug() << "DataBase::updateTo005 - Already in 005" ;
           return true;
       }
       else
       {
                //qDebug() << "DataBase::updateTo005 - 005 update false" ;
           IAmIn005 = false;
       }


       while (!IAmIn005 && !ErrorUpdating)
       {
                 //qDebug() << "DataBase::updateTo005 - I am not in 005" ;
           while (!IAmIn004 && !ErrorUpdating)
           {
                    //qDebug() << "DataBase::updateTo005 - I am not in 004" ;
               IAmIn004 = updateTo004();
           }
              //qDebug() << "DataBase::updateTo005 - I am in 004" ;
           if (ErrorUpdating)
           {
                  //qDebug() << "DataBase::updateTo005 - 005 update false2" ;
              // emit debugLog(Q_FUNC_INFO, "1", 7);
               return false;
           }
           sqlOk = updateDBVersion(softVersion, "0.005");

           if (sqlOk)
           { // Version updated
               if (recreateContestData())
               {
                        //qDebug() << "DataBase::updateTo005 - recreateContestData OK" ;

                   sqlOk = execQuery(Q_FUNC_INFO, "DROP table logs");

                   sqlOk = createTableLogs(true);
                    if (!sqlOk)
                    {
                        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                            //qDebug() << "DataBase::updateTo005 - logs table do not created" ;
                    }


                   if (howManyQSOsInLog(0)>0)
                   { // If the user has QSOs that were added with previous versions...
                     // We need to create a new log and rename all QSOs to that QSO.
                     //stringQuery = QString("UPDATE log SET lognumber='1' WHERE lognumber='0'");


                       msgBox.setText(QObject::tr("KLog has detected a previous log in the DB. All data will be migrated to a newly created DX type log for you."));
                       msgBox.exec();

                        if (!execQuery(Q_FUNC_INFO, "UPDATE log SET lognumber='1' WHERE lognumber='0'"))
                        {
                          queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                            //showError(QObject::tr("QSOs not updated to main log"));
                                 //qDebug() << "DataBase::updateTo005 - QSOs not updated to main log" ;
                        }

                        QString dateString = (QDate::currentDate()).toString("yyyy-MM-dd");
                        QString callToUse = QString();
                        bool ok;
                        //QString text;

                        //text = QInputDialog::getText(this, QObject::tr("Station Callsign"), QObject::tr("Enter the Station Callsign you want to use in the imported log:"), QLineEdit::Normal, QObject::tr("N0CALL"), &ok);
                        QString text = (QInputDialog::getText(nullptr, QObject::tr("KLog: Enter Station callsign"),
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


                        if (!sqlOk)
                        {
                            queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                            //showError(QObject::tr("New Log not created"));
                                 //qDebug() << "DataBase::updateTo005 - New Log not created" ;
                                 //qDebug() << "DataBase::clearLog: Log deleted FAILED" ;
                        }
                   }
                   IAmIn005 = true;
               }
               else
               {
                        //qDebug() << "DataBase::updateTo005 - recreateContestData FAILED" ;
                   ErrorUpdating = true;
               }
           }
           else
           { // Version not updated
                    //qDebug() << "DataBase::updateTo005 - 005 update false6" ;
                ErrorUpdating = true;
           }
       }
            //qDebug() << "DataBase::updateTo005 - 005 updated 3" ;

       //TODO: Delete the table and recreate it
       if (IAmIn005)
       {
            msgBox.setText(QObject::tr("All the data was migrated correctly. You should now go to Setup->Preferences->Logs to check that everything is okay."));
            msgBox.exec();
       }
       else
        {
          // emit debugLog(Q_FUNC_INFO, "2", 7);
        }
             //qDebug() << "DataBase::updateTo005 - I am in 005 already!! " ;
          //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
       return IAmIn005;
}

bool DataBase::recreateSatelliteData()
{
       //qDebug() << "DataBase::recreateSatelliteData"  ;
    QSqlQuery query;

    if (isTheTableExisting("satellites"))
    {
        if (execQuery(Q_FUNC_INFO, "DROP TABLE satellites"))
        {
            if (createTableSatellites(true))
            {
                    //qDebug() << Q_FUNC_INFO << ": SAT table created"  ;
                return populateTableSatellites(true);
            }
            else
            {
                     //qDebug() << Q_FUNC_INFO << ": SAT table NOT created"  ;
            }
        }
        else
        {
               //qDebug() << Q_FUNC_INFO << ": execQuery FAILED"  ;
        }
    }
    else
    {
        if (createTableSatellites(true))
        {
                //qDebug() << Q_FUNC_INFO << ": SAT table created"  ;
            return populateTableSatellites(true);
        }
        else
        {
                 //qDebug() << Q_FUNC_INFO << ": SAT table NOT created"  ;
        }
    }
       //qDebug() << Q_FUNC_INFO << ": END FALSE"  ;
    return false;
}

bool DataBase::recreateContestData()
{
        //qDebug() << "DataBase::recreateContestData"  ;
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
       //qDebug() << "DataBase::recreateSupportedContest"  ;
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists supportedcontests");

    if (isTheTableExisting("supportedcontests"))
    {
        QSqlQuery query;
        bool sqlOk = false;
        sqlOk = execQuery(Q_FUNC_INFO, "DROP TABLE supportedcontests");
        if (sqlOk)
        {
                //qDebug() << "DataBase::recreateSupportedContest SQLOK"  ;
            if (createTableSupportedContest())
            {
                    //qDebug() << "DataBase::recreateSupportedContest - createTable OK"  ;
                return populateTableSupportedContest();
            }
            else
            {
                    //qDebug() << "DataBase::recreateSupportedContest createTableSupportContest FALSE"  ;
                return false;
            }
        }
        else
        {
            queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        }
    }
    else
    {
        if (createTableSupportedContest())
        {
                //qDebug() << "DataBase::recreateSupportedContest - createTable OK"  ;
            return populateTableSupportedContest();
        }
        else
        {
                //qDebug() << "DataBase::recreateSupportedContest createTableSupportContest FALSE"  ;
            return false;
        }
    }
        //qDebug() << "DataBase::recreateSupportedContest - FALSE end"  ;
    return false;
}


bool DataBase::recreatePropModes()
{
       //qDebug() << "DataBase::recreatePropModes"  ;
    if (isTheTableExisting("prop_mode_enumeration"))
    {
           //qDebug() << "DataBase::recreatePropModes: Table Exist"  ;
        bool sqlOk = false;
        sqlOk = execQuery(Q_FUNC_INFO, "DROP TABLE prop_mode_enumeration");

        if (sqlOk)
        {
               //qDebug() << "DataBase::recreatePropModes SQLOK"  ;
            if (createTablePropModes())
            {
                   //qDebug() << "DataBase::recreatePropModes - createTable OK"  ;
                if (populatePropagationModes())
                {
                       //qDebug() << "DataBase::recreatePropModes - populatePropModes OK"  ;
                    return true;
                }
                else
                {
                       //qDebug() << "DataBase::recreatePropModes - populatePropModes NOK"  ;
                    return false;
                }
            }
            else


            {
                     //qDebug() << "DataBase::recreatePropModes createTableSupportContest FALSE-1"  ;
                  return false;
            }
        }
        else
        {
            //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
                 //qDebug() << "DataBase::recreatePropModes - prop_mode_enumeration table has not been dropped"  ;
                 //qDebug() << "DataBase::recreatePropModes : Table creation FAILED" ;
        }
    }
    else
    {
           //qDebug() << "DataBase::recreatePropModes: Table does NOT Exist"  ;
        if (createTablePropModes())
        {
               //qDebug() << "DataBase::recreatePropModes - createTable OK"  ;
            if (populatePropagationModes())
            {
                   //qDebug() << "DataBase::recreatePropModes - populatePropModes OK"  ;
                return true;
            }
            else
            {
                   //qDebug() << "DataBase::recreatePropModes - populatePropModes NOK"  ;
                return false;
            }
        }
        else
        {
                 //qDebug() << "DataBase::recreatePropModes createTableSupportContest FALSE-2"  ;
        }
    }

        //qDebug() << "DataBase::recreatePropModes - FALSE end"  ;
    return false;
}


bool DataBase::createTableLogs(const bool real)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
    //Creating the Sats DB to be able to include satellites to the LOTW

    //qDebug() << Q_FUNC_INFO << " - Start: " << QString::number(real);

    QString stringQuery = QString();
    //QSqlQuery query;
    if (real)
    {
        //qDebug() << Q_FUNC_INFO << " - logs" ;
        stringQuery = "CREATE TABLE logs" ;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << "  - logstemp" ;
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



        //qDebug() << Q_FUNC_INFO << "  - END" ;

        return execQuery(Q_FUNC_INFO, stringQuery);
}


bool DataBase::createTablePropModes()
{
       //qDebug() << "DataBase::createTablePropModes" ;

    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists prop_mode_enumeration");
    return execQuery(Q_FUNC_INFO, "CREATE TABLE prop_mode_enumeration (id INTEGER PRIMARY KEY AUTOINCREMENT, shortname VARCHAR(8), name VARCHAR(55) )");
}

bool DataBase::createTableSupportedContest()
{
         //qDebug() << "DataBase::createTableSupportedContest" ;
    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists supportedcontests");

    QString st = QString("CREATE TABLE supportedcontests ("
                         "id INTEGER PRIMARY KEY, "
                         "longname VARCHAR,"
                         "name VARCHAR)");
    return execQuery(Q_FUNC_INFO, st);
}

bool DataBase::createTableContest()
{
        //qDebug() << "DataBase::createTableContest" ;
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

         //qDebug() << "DataBase::createTableContest END" ;
    return true;
}

bool DataBase::populateTableSupportedContest()
{
        //qDebug() << "DataBase::populateTableSupportedContest" ;
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
        //qDebug() << "DataBase::createTableMode" ;
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

            //qDebug() << "DataBase::createTableMode END" ;

        return execQuery(Q_FUNC_INFO, stringQuery);
}

bool DataBase::populateTableModePSK(bool NoTmp)
{
    QStringList modes = {"8PSK125", "8PSK125F", "8PSK125FL", "8PSK250", "8PSK250F", "8PSK250FL",
                         "8PSK500", "8PSK500F", "8PSK1000", "8PSK1000F", "8PSK1200F", "FSK31",
                         "PSK10", "PSK31", "PSK63", "PSK63F", "PSK63RC4", "PSK63RC5", "PSK63RC10",
                         "PSK63RC20", "PSK63RC32", "PSK125", "PSK125C12", "PSK125R", "PSK125RC10",
                         "PSK125RC12", "PSK125RC16", "PSK125RC4", "PSK125RC5", "PSK250", "PSK250C6",
                         "PSK250R", "PSK250RC2", "PSK250RC3", "PSK250RC5", "PSK250RC6", "PSK250RC7", "PSK500",
                         "PSK500C2", "PSK500C4", "PSK500R", "PSK500RC2", "PSK500RC3", "PSK500RC4", "PSK800C2",
                         "PSK800RC2", "PSK1000", "PSK1000C2", "PSK1000R", "PSK1000RC2", "PSKAM10", "PSKAM31",
                         "PSKAM50", "PSKFEC31", "QPSK31", "QPSK63", "QPSK125", "QPSK250", "QPSK500", "SIM31" };

    QString table;
    if (NoTmp)
    {
        table = "mode";
    }
    else
    {
        table = "modetemp";
    }
    QString aux;
    foreach (aux, modes)
    {
        if (!execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('%2', 'PSK', 'DG', '1')").arg(table).arg(aux)))
            return false;
    }
    return true;
}

bool DataBase::populateTableMode(const bool NoTmp)
{
    //qDebug() << "DataBase::populateTableMode" ;
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
        //queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() << "DataBase::populateTableMode: Mode table population FAILED" ;
        //errorCode = query.lastError().text();
    }
    else
    {
             //qDebug() << "DataBase::populateTableMode: Mode table population  OK" ;
    }
    if (!populateTableModePSK(NoTmp))
        return false;
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ARDOP', 'ARDOP', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('AMTORFEC', 'TOR', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ASCI', 'RTTY', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ATV', 'ATV', 'NO', '0')").arg(tableName));

    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP', 'CHIP', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP64', 'CHIP', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CHIP128', 'CHIP', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CLO', 'CLO', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CONTESTI', 'CONTESTI', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('CW', 'CW', 'CW', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DIGITALVOICE', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('C4FM', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DMR', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DSTAR', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FREEDV', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('M17', 'DIGITALVOICE', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('VARA HF', 'DYNAMIC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('VARA SATELLITE', 'DYNAMIC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('VARA FM 1200', 'DYNAMIC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('VARA FM 9600', 'DYNAMIC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINO', 'DOMINO', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINOEX', 'DOMINO', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOMINOF', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM-M', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM4', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM5', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM8', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM11', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM16', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM22', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM44', 'DOMINO', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('DOM88', 'DOMINO', 'NO', '1')").arg(tableName));

    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FAX', 'FAX', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FM', 'FM', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FMHELL', 'HELL', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FT4', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FST4', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FST4W', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FT8', 'FT8', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSK441', 'FSK441', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSKHELL', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('FSQCALL', 'MFSK', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('GTOR', 'TOR', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELL', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELLX5', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELLX9', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HELL80', 'HELL', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('HFSK', 'HELL', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT', 'ISCAT', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT-A', 'ISCAT', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ISCAT-B', 'ISCAT', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JTMS', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JS8', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4', 'JT4', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4A', 'JT4', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4B', 'JT4', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4C', 'JT4', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4D', 'JT4', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4E', 'JT4', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4F', 'JT4', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT4G', 'JT4', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT6M', 'JT6M', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-1', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-2', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-5', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-10', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9-30', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9A', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9B', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9C', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9D', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9E', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9E FAST', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9F', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9F FAST', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9G', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9G FAST', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9H', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT9H FAST', 'JT9', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT44', 'JT44', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65', 'JT65', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65A', 'JT65', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65B', 'JT65', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65B2', 'JT65', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65C', 'JT65', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('JT65C2', 'JT65', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK4', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK8', 'MFSK', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK11', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK16', 'MFSK', 'DG', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK22', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK31', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK32', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK64', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK64L', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK128', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MFSK128L', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MSK144', 'MSK144', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('MT63', 'MT63', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 4/125', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 4/250', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 8/250', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 8/500', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 16/500', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 16/1000', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OLIVIA 32/1000', 'OLIVIA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA', 'OPERA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA-BEACON', 'OPERA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('OPERA-QSO', 'OPERA', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC', 'PAC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC2', 'PAC', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC3', 'PAC', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAC4', 'PAC', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAX', 'PAX', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PAX2', 'PAX', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PCW', 'CW', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PKT', 'PKT', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSK2K', 'PSK2K', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('PSKHELL', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SLOWHELL', 'HELL', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('Q15', 'Q15', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('Q65', 'MFSK', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64', 'QRA64', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64A', 'QRA64', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64B', 'QRA64', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64C', 'QRA64', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64D', 'QRA64', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('QRA64E', 'QRA64', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS', 'ROS', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-EME', 'ROS', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-HF', 'ROS', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('ROS-MF', 'ROS', 'DG', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('RTTY', 'RTTY', 'RY', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('RTTYM', 'RTTYM', 'RY', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SSB', 'SSB', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('LSB', 'SSB', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('USB', 'SSB', 'PH', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SSTV', 'SSTV', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('T10', 'T10', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRB', 'THRB', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRBX', 'THRB', 'NO', '1')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRBX1', 'THRB', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRBX2', 'THRB', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THRBX4', 'THRB', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THROB1', 'THRB', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THROB2', 'THRB', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THROB4', 'THRB', 'NO', '0')").arg(tableName));

    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR-M', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR4', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR5', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR8', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR11', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR16', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR22', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR25X4', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR50X1', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR50X2', 'THOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('THOR100', 'THOR', 'NO', '0')").arg(tableName));


    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('TOR', 'TOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('NAVTEX', 'TOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('SITORB', 'TOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('V4', 'V4', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('VOI', 'VOI', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('WINMOR', 'WINMOR', 'NO', '0')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (submode, name, cabrillo, deprecated) VALUES ('WSPR', 'WSPR', 'NO', '0')").arg(tableName));


    createTheModeQuickReference();
         //qDebug() << "DataBase::populateTableMode END" ;
    return true;
}


bool DataBase::createTableSatellites(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
    //Creating the Sats DB to be able to include satellites to the LOTW

       //qDebug() << "DataBase::createTableSatellites" ;

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
    //qDebug() << Q_FUNC_INFO << " - Dropped table: " << table;
    execQuery(Q_FUNC_INFO, delS);

    return execQuery(Q_FUNC_INFO, stringQuery);
}

bool DataBase::populateTableSatellites(const bool NoTmp)
{
    // Data must come from:
    // https://lotw.arrl.org/lotw-help/frequently-asked-questions/#sats
       //qDebug() << "DataBase::populateTableSatellites" ;

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
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('LO-87', 'LUSEX-OSCAR 87', '435.935-435.965', '145.935-145.965', 'LSB/USB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('MIREX', 'Mir packet digipeater', '145.985', '145.985', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('NO-44', 'Navy-OSCAR 44', '145.827', '145.827', 'PKT')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('NO-83', 'BRICsat','145.825,28.120', '145.825,435.975','PKT,PSK31')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('NO-84', 'PSAT', '145.825,28.120', '435.350', 'PKT,PSK31')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-1',  'Radio Sputnik 1', '145', '29', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-10', 'Radio Sputnik 10', '','29.357,29.403', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-11', 'Radio Sputnik 11', '','29.357,29.403', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-12', 'Radio Sputnik 12', '21.210-21.250', '29.410-29.450', 'SSB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-13', 'Radio Sputnik 13', '21.260-21.300', '145.860-145.900', 'SSB')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-15', 'Radio Sputnik 15', '', '29.3525-29.3987', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-2',  'Radio Sputnik 2', '145', '29', '')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('RS-44', 'DOSAAF-85', '145.935-145.995', '435.610-435.670', 'LSB/USB')").arg(tableName));
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
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (satarrlid, satname, uplink, downlink, satmode) VALUES ('QO-100', 'Es''hail-2', '2400.050-2409.500', '10489.550-10499.000', 'SSB,CW')").arg(tableName));

       //qDebug() << "DataBase::populateTableSatellites - END" ;
    return true;
}

bool DataBase::createTableEntity(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
    //qDebug() << Q_FUNC_INFO << " - Start";

    QString table = "entity";
    if (!NoTmp)
        table = "entitytemp";
     //TODO: To add some columns in this the table to mark if worked/confirmed/band/Mode
    QString stringQuery = QStringLiteral(
        "CREATE TABLE %1 ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name VARCHAR(40) NOT NULL,"
        "cqz INTEGER NOT NULL, "
        "ituz INTEGER NOT NULL, "
        "continent INTEGER NOT NULL, "
        "latitude REAL NOT NULL, "
        "longitude REAL NOT NULL, "
        "utc REAL NOT NULL, "
        "dxcc INTEGER NOT NULL, "
        "mainprefix VARCHAR(15) NOT NULL, "
        "deleted INTEGER, "
        "sincedate VARCHAR(10), "
        "todate VARCHAR(10), "
        "isoname VARCHAR(10), "
        "UNIQUE (dxcc, mainprefix), "
        "FOREIGN KEY (continent) REFERENCES continent(id) )").arg(table);
    if (!execQuery(Q_FUNC_INFO, stringQuery))
    {
        return false;
    }
    QStringList indexQueries = {
            QStringLiteral("CREATE INDEX idx_%1_mainprefix ON %1 (mainprefix)").arg(table),
            QStringLiteral("CREATE INDEX idx_%1_cqz ON %1 (cqz)").arg(table),
            QStringLiteral("CREATE INDEX idx_%1_ituz ON %1 (ituz)").arg(table),
            QStringLiteral("CREATE INDEX idx_%1_continent ON %1 (continent)").arg(table)
    };

    for (const QString &indexQuery : indexQueries)
    {
        if (!execQuery(Q_FUNC_INFO, indexQuery))
        {
            return false;
        }
    }
    return true;
}


bool DataBase::createTableBand(const bool NoTmp)
{ // NoTmp = false => TMP data table to operate and be deleted afterwards
        //qDebug() << Q_FUNC_INFO << " - Start" ;
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

        //qDebug() << Q_FUNC_INFO << " - END" ;
    return execQuery(Q_FUNC_INFO, stringQuery);
}

bool DataBase::syncLogQSOsOnBandTableChange()
{
    //qDebug() << Q_FUNC_INFO ;
    QString stringQuery;
    QSqlQuery query;

    stringQuery = "SELECT DISTINCT log.bandid FROM log ORDER BY bandid DESC";

    if (!query.exec(stringQuery))
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }

    QList<int> bandIDs;
    bandIDs.clear();
    int bandid = -1;
    while(query.next())
    {
        if (!query.isValid())
        {
            query.finish();
            return false;
        }
        bandid = (query.value(0)).toInt();
        if (bandid>0)
        {
            bandIDs.append (bandid);
            bandid = -1;
        }
    }

    if (bandIDs.length ()>0)
    {
        foreach(int i, bandIDs)
        {
            stringQuery = QString("UPDATE log SET bandid = (SELECT DISTINCT bandtemp.id FROM bandtemp INNER JOIN band ON band.name = bandtemp.name WHERE band.id='%1') WHERE log.bandid='%1'").arg(i);
            //qDebug() << Q_FUNC_INFO << ": " << stringQuery ;
            if (!query.exec(stringQuery))
            {
                query.finish();
                return false;
            }
        }
    }
    query.finish ();

    //qDebug() << Q_FUNC_INFO << " - END" ;
    return true;
}

bool DataBase::populateTableBand(const bool NoTmp)
{
    // Cabrillo definition: http://wwrof.org/cabrillo/cabrillo-specification-v3/

        //qDebug() << "DataBase::populateTableBand" ;


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
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('SUBMM', '300000', '7500000', 'SUBMM')").arg(tableName));
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
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('4M', '70', '71', '70')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('5M', '54.000001', '69.9', '5M')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('6M', '50', '54', '50')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('8M', '40', '45', '40')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('10M', '28.0', '29.7', '28000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('12M', '24.89', '24.99', '24900')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('15M', '21.0', '21.45', '21000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('17M', '18.068', '18.168', '18100')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('20M', '14.0', '14.35', '14000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('30M', '10.0', '10.15', '10000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('40M', '7.0', '7.3', '7000')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('60M', '5.102', '5.404', '60M')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('80M', '3.5', '4.0', '3500')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('160M', '1.8', '2.0', '1800')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('560M', '0.501', '0.504', '560M')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('630M', '0.472', '0.479', '630M')").arg(tableName));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO %1 (name, lower, upper, cabrillo) VALUES ('2190M', '0.1357', '0.1378', '2190M')").arg(tableName));

    createTheBandQuickReference();

        //qDebug() << "DataBase::populateTableBand END" ;
    return true;
}


bool DataBase::populatePropagationModes()
{
        //qDebug() << "DataBase::populatePropagationModes" ;
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
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('GWAVE', 'Ground Wave')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('INTERNET', 'Internet-assisted')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('ION', 'Ionoscatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('IRL', 'IRLP')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('LOS', 'Line of Sight')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('MS', 'Meteor scatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('RPT', 'Terrestrial or atmospheric repeater or transponder')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('RS', 'Rain scatter')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('SAT', 'Satellite')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('TEP', 'Trans-equatorial')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('TR', 'Tropospheric ducting')"));

       //qDebug() << "DataBase::populatePropagationModes END" ;
    return true;
}


bool DataBase::populateContestData()
{
        //qDebug() << "DataBase::populateContestData" ;



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

        //qDebug() << "DataBase::populateContestData END" ;
    return true;
}

bool DataBase::howManyQSOsInLog(const int i)
{
        //qDebug() << "DataBase::howManyQSOsInLog" ;

    QSqlQuery query;
    QString sqlQueryString = QString("SELECT COUNT(id) from log WHERE lognumber='%1'").arg(i);
    bool sqlOK = query.exec(sqlQueryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
                //qDebug() << "DataBase::howManyQSOsInLog OK END" ;
            return (query.value(0)).toInt();
        }
        else
        {
                //qDebug() << "DataBase::howManyQSOsInLog END-1" ;
            query.finish();
            return false;
        }
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
            //qDebug() << "DataBase::howManyQSOsInLog END-2" ;
        return false;
    }
    //query.finish();
    //return -2;
}

bool DataBase::updateTo006()
{// Updates the DB to 0.0.6
    //qDebug() << Q_FUNC_INFO << ": latestRead: " << getDBVersion() ;

    QString stringQuery = QString();

    double latestReaded = getDBVersion();
    if (latestReaded >= 0.006f)
        return true;

    if (!updateTo005())
        return false;

    if (!recreatePropModes())
        return false;
    if (!updateTableLog(6))
        return false;

    if (!createTableBand(false))
        return false;

    if (!populateTableBand(false))
        return false;

    if (!updateBandIdTableLogToNewOnes())
        return false;

    if (!execQuery(Q_FUNC_INFO, "DROP TABLE band"))
        return false;

    if (!execQuery(Q_FUNC_INFO, "ALTER TABLE bandtemp RENAME TO band"))
        return false;

    if (!updateTheModeTableAndSyncLog())
        return false;

    if (!createTableClubLogStatus())
        return false;

    if (!populateTableClubLogStatus())
        return false;

    return updateDBVersion(softVersion, "0.006");
}

bool DataBase::updateTableLog(const int _version)
{
    //qDebug() << Q_FUNC_INFO << QString::number(_version);
    // _version helps to identify which update we need to run
    if ((_version!=6) && (_version!=7))
        return false;

    QString queryString;
    QString oldFields;  // This variable holds the list of fields BEFORE the update.
                        // Updates are usually done to add ADIF fields to the DB.
    // We need to move all the table except the new values that are included in this release:
    switch (_version)
    {
        case 6:
            oldFields = "qso_date, call, rst_sent, rst_rcvd, bandid, modeid, srx, stx, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, band_rx, checkcontest, class, contacted_op, contest_id, country, credit_submitted, credit_granted, distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd, eqsl_qsl_sent, force_init, freq, freq_rx, gridsquare, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_city, my_cnty, my_country, my_cq_zone, my_gridsquare, my_iota, my_iota_island_id, my_lat, my_lon, my_name, my_rig, my_sig, my_sig_info, my_state, my_street, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qslmsg, qslrdate, qslsdate, qsl_rcvd, qsl_sent, qsl_rcvd_via, qsl_sent_via, qsl_via, qso_complete, qso_random, qth, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, srx_string, stx_string, state, station_callsign, swl, ten_ten, tx_pwr, web, qso_date_off, marked, lognumber";
        break;
        case 7:
            oldFields = "qso_date, call, rst_sent, rst_rcvd, bandid, modeid, cqz, ituz, dxcc, address, age, cnty, comment, a_index, ant_az, ant_el, ant_path, arrl_sect, award_submitted, award_granted, band_rx, checkcontest, class, clublog_qso_upload_date, clublog_qso_upload_status, cont, contacted_op, contest_id, country, credit_submitted, credit_granted, darc_dok,distance, email, eq_call, eqsl_qslrdate, eqsl_qslsdate, eqsl_qsl_rcvd,eqsl_qsl_sent,fists, fists_cc, force_init, freq, freq_rx, gridsquare, hrdlog_qso_upload_date, hrdlog_qso_upload_status, iota, iota_island_id, k_index, lat, lon, lotw_qslrdate, lotw_qslsdate, lotw_qsl_rcvd, lotw_qsl_sent, max_bursts, ms_shower, my_antenna, my_city, my_cnty, my_country, my_cq_zone, my_dxcc, my_fists, my_gridsquare, my_iota, my_iota_island_id, my_itu_zone, my_lat, my_lon, my_name, my_postal_code, my_rig, my_sig, my_sig_info, my_sota_ref, my_state, my_street, my_usaca_counties, my_vucc_grids, name, notes, nr_bursts, nr_pings, operator, owner_callsign, pfx, precedence, prop_mode, public_key, qrzcom_qso_upload_date, qrzcom_qso_upload_status, qslmsg, qslrdate, qslsdate, qsl_rcvd,qsl_sent, qsl_rcvd_via, qsl_sent_via,qsl_via, qso_complete, qso_random, qth, region, rig, rx_pwr, sat_mode, sat_name, sfi, sig, sig_info, silent_key,skcc, sota_ref, srx_string, srx, stx_string, stx, state, station_callsign, submode, swl, uksmg, usaca_counties, ve_prov, vucc_grids, ten_ten, tx_pwr, web, qso_date_off, marked, lognumber";
        break;
        default:
        // We should never reach this value but if we reach it, weexit the function
            return false;
        break;
    }
    queryString = QString ("INSERT INTO logtemp (%1) SELECT %2 FROM log").arg(oldFields).arg(oldFields);
    // Everything is ready, we can:
    //  - create the temp table for log,
    //  - move the data from the old table to the new one, taking into account the version
    //  - rename drop the old table
    //  - rename the new table temp as the regular log table

    //qDebug() << Q_FUNC_INFO << " - Creating the table logtemp";
    if (!createTableLog(false))
        return false;

    //qDebug() << Q_FUNC_INFO << " - Creation went OK, lets move the data to the logtemp table";
    if (!execQuery(Q_FUNC_INFO, queryString))
        return false;

    //qDebug() << Q_FUNC_INFO << " - Data movement went OK, lets drop old log table";
    if (!execQuery(Q_FUNC_INFO, "DROP TABLE log"))
        return false;

    //qDebug() << Q_FUNC_INFO << " - Deletion of the table went OK, lets move the logtemp to log";
    queryString = "ALTER TABLE logtemp RENAME TO log" ;
    return execQuery(Q_FUNC_INFO, queryString);
}


bool DataBase::createTableClubLogStatus()
{
        //qDebug() << "createTableClubLogStatus" ;

    execQuery(Q_FUNC_INFO, "DROP TABLE IF exists clublog_status");
    QString queryString = QString("CREATE TABLE clublog_status ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "shortname VARCHAR(1) NOT NULL, "
               "name VARCHAR(15) NOT NULL)");

    return execQuery(Q_FUNC_INFO, queryString);
}

bool DataBase::populateTableClubLogStatus()
{
    //qDebug() << Q_FUNC_INFO;

    QString queryString = "INSERT INTO clublog_status (shortname, name) VALUES ('Y', 'Uploaded')";
    if (!execQuery(Q_FUNC_INFO, queryString))
        return false;

    queryString = "INSERT INTO clublog_status (shortname, name) VALUES ('N', 'Do not upload')";
    if (!execQuery(Q_FUNC_INFO, queryString))
        return false;

    queryString = "INSERT INTO clublog_status (shortname, name) VALUES ('M', 'Modified')";
    return execQuery(Q_FUNC_INFO, queryString);
}


//TODO: Awards are deprecated
bool DataBase::createAndPopulateAwardEnumeration()
{
    QString stringQuery = QString("CREATE TABLE award_enumeration ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "name VARCHAR(15) NOT NULL)");

    if (!execQuery(Q_FUNC_INFO, stringQuery))
    {
            return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('AJA')"))
    {
            return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('CQDX')"))
    {
            return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('CQDXFIELD')"))
    {
            return false;
    }
    return execQuery(Q_FUNC_INFO, "INSERT INTO award_enumeration (name) VALUES ('DXCC')");
}

bool DataBase::createAndPopulateARRLSectEnumeration()
{
    QString stringQuery = QString("CREATE TABLE arrl_sect_enumeration ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "shortname VARCHAR(2) NOT NULL, "
               "name VARCHAR(30) NOT NULL)");
    if (!execQuery(Q_FUNC_INFO, stringQuery))
        return false;
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AL', 'Alabama')"))
        return false;

    /*
    execQuery(");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AB', 'Alberta')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AR', 'Arkansas')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AZ', 'Arizona')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('BC', 'British Columbia')");
    execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('CO', 'Colorado')");
    */

    //The following must be the last one
    return execQuery(Q_FUNC_INFO, "INSERT INTO arrl_sect_enumeration (shortname, name) VALUES ('AK', 'Alaska')");
        return false;
}



//bool DataBase::createAndPopulateQSO_CompleteEnumeration()
//{
//    QString stringQuery = QString("CREATE TABLE qso_complete_enumeration ("
//             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//             "shortname VARCHAR(3) NOT NULL, "
//             "name VARCHAR(10) NOT NULL)");
//    if (!execQuery(Q_FUNC_INFO, stringQuery))
//    {
//        return false;
//    }
//    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('Y', 'Yes')"))
//    {
//        return false;
//    }
//    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('N', 'No')"))
//    {
//        return false;
//    }
//    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('H', 'Not heard')"))
//    {
//        return false;
//    }
//    return execQuery(Q_FUNC_INFO, "INSERT INTO qso_complete_enumeration (shortname, name) VALUES ('?', 'Uncertain')");
//}


bool DataBase::createAndPopulateAnt_path_enumeration()
{
    QString stringQuery = QString("CREATE TABLE ant_path_enumeration ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(1) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");
    if (!execQuery(Q_FUNC_INFO, stringQuery))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('G', 'GrayLine')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('O', 'Other')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('S', 'ShortPath')"))
    {
        return false;
    }
    return execQuery(Q_FUNC_INFO, "INSERT INTO ant_path_enumeration (shortname, name) VALUES ('L', 'LongPath')");
}

bool DataBase::createAndPopulateContinents()
{
    //qDebug() << Q_FUNC_INFO ;
    QString stringQuery = QString("CREATE TABLE continent ("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "shortname VARCHAR(2) NOT NULL, "
                 "name VARCHAR(15) NOT NULL)");
    if (!execQuery(Q_FUNC_INFO, stringQuery))
    {
        return false;
    }

    if (!execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('AF', 'Africa')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('AS', 'Asia')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('EU', 'Europe')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('NA', 'North America')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('OC', 'Oceania')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('SA', 'South America')"))
    {
        return false;
    }
    return execQuery(Q_FUNC_INFO, "INSERT INTO continent (shortname, name) VALUES ('AN', 'Antartica')");
}

bool DataBase::createAndPopulateQSLSentRecStatus()
{
    //qDebug() << Q_FUNC_INFO << " - Start";


    QString stringQuery = QString("CREATE TABLE qsl_rec_status ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "shortname VARCHAR(1) NOT NULL, "
               "name VARCHAR(15) NOT NULL)");
    if (!execQuery(Q_FUNC_INFO, stringQuery))
    {
        return false;
    }

    stringQuery = QString("CREATE TABLE qsl_sent_status ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "shortname VARCHAR(1) NOT NULL, "
               "name VARCHAR(15) NOT NULL)");
    if (!execQuery(Q_FUNC_INFO, stringQuery))
    {
        return false;
    }

    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('Y', 'Yes')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('N', 'No')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('R', 'Requested')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('Q', 'Queued')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_sent_status (shortname, name) VALUES ('I', 'Ignore/Invalid')"))
    {
        return false;
    }


    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('Y', 'Yes')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('N', 'No')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('R', 'Requested')"))
    {
        return false;
    }
    if (!execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('I', 'Ignore/Invalid')"))
    {
        return false;
    }
    return execQuery(Q_FUNC_INFO, "INSERT INTO qsl_rec_status (shortname, name) VALUES ('V', 'Validated')");
}

bool DataBase::recreateTableEntity()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QString stringQuery;

    if (!createTableEntity(false))
        return false;

    //qDebug() << Q_FUNC_INFO << " - 10";
    stringQuery = QString("INSERT INTO entitytemp (name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate) SELECT name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix, deleted, sincedate, todate FROM entity");
    if (!execQuery(Q_FUNC_INFO, stringQuery))
        return false;

    //qDebug() << Q_FUNC_INFO << " - 30";
    stringQuery = "DROP TABLE entity";
    if (!execQuery(Q_FUNC_INFO, stringQuery))
        return false;

    //qDebug() << Q_FUNC_INFO << " - 30";
    stringQuery = "ALTER TABLE entitytemp RENAME TO entity";
    return execQuery(Q_FUNC_INFO, stringQuery);
}

bool DataBase::updateTableLogs()
{
    //qDebug() << Q_FUNC_INFO ;
    QString stringQuery;

    if (!createTableLogs(false))  // Now we have a temp entity table with the correct format
        return false;

    //qDebug() << Q_FUNC_INFO << ": Table logstemp created!" ;
    // Now we need to move all the data from the old to the temp logs table.

    stringQuery = QString("INSERT INTO logstemp (logdate, stationcall, comment, logtype, logtypen) SELECT logdate, stationcall, comment, logtype, logtypen FROM logs");
    if (!execQuery(Q_FUNC_INFO, stringQuery))
        return false;

    stringQuery = "DROP TABLE logs";
    if (!execQuery(Q_FUNC_INFO, stringQuery))
        return false;

    stringQuery = "ALTER TABLE logstemp RENAME TO logs";
    return execQuery(Q_FUNC_INFO, stringQuery);
}

bool DataBase::updateModeIdFromSubModeId()
{// Updates the log with the new mode IDs in each QSO:
    // STEP-1: Get the modeid and QSOid from the log
    // STEP-2: uses the modeid to get the name of the mode in the mode table (the old one)
    // STEP-3: uses the name of the mode in the modetemp table (the new one) to get the new ID
    // STEP-4: Updates the new ID in the QSO in the log
    //TODO: Optimize this function

    //qDebug() << Q_FUNC_INFO ;
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
    QSqlQuery query;
    bool sqlOk = query.exec("SELECT COUNT (*) FROM log");

    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
        query.finish();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
             //qDebug() << Q_FUNC_INFO << ": FALSE END"  ;
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
                     //qDebug() << Q_FUNC_INFO << ": (STEP-1) modeFound (numb): " << QString::number(modeFound) ;

                modetxt = getModeNameFromNumber(modeFound, false);                                                      //STEP-2

                     //qDebug() << Q_FUNC_INFO << ": (STEP-2) mode found (txt): " << modetxt ;

                //TODO The following query can be executed in: getModeIdFromSubMode()

                sq = QString("SELECT id FROM modetemp WHERE submode='%1'").arg(modetxt);                                // STEP-3
                QSqlQuery query2;
                sqlOk2 = query2.exec(sq);

                if (sqlOk2)
                {
                         //qDebug() << Q_FUNC_INFO << ": (STEP-3) sqlOK2 TRUE" ;
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
                                //qDebug() << Q_FUNC_INFO << ": (STEP-4) ID: " << QString::number(id) << " updated to: " << QString::number(modeFound) <<"/"<< modetxt ;
                            }
                            else
                            {
                                //queryErrorManagement(Q_FUNC_INFO, query3.lastError().databaseText(), query3.lastError().nativeErrorCode(), query3.lastQuery());
                                //qDebug() << Q_FUNC_INFO << ": (STEP-4) ID: " << QString::number(id) << " NOT updated-2"  ;
                            }
                        }
                        else
                        {
                            query2.finish();
                                 //qDebug() << Q_FUNC_INFO << ": (STEP-3) query2 not valid "   ;
                        }
                    }
                    else
                    {
                          //qDebug() << Q_FUNC_INFO << ": query2 not next "   ;
                    }
                }
                else
                {
                    queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().nativeErrorCode(), query2.lastQuery());
                    query2.finish();
                         //qDebug() << Q_FUNC_INFO << ": ID: " << QString::number(id) << " NOT updated-1"  ;
                }
            }

            if ( progress.wasCanceled() )
            {
                if (!alreadyCancelled)
                {
                    alreadyCancelled = true;

                    QMessageBox msgBox;
                    msgBox.setWindowTitle(QObject::tr("KLog - DB update"));
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
                            progress.setCancelButton(nullptr);
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
                //qDebug() << Q_FUNC_INFO << ": FALSE END 2"  ;
            query.finish();
            return false;
        }
            //qDebug() << Q_FUNC_INFO << ": END"  ;
        query.finish();
        return true;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug() << Q_FUNC_INFO << ": FALSE END 3"  ;
        query.finish();
        return false;
    }
    //qDebug() << Q_FUNC_INFO << ": CHECK IF this is seen - END"  ;
}


bool DataBase::updateBandIdTableLogToNewOnes()
{
       //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: "  ;

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
    QSqlQuery query;
    bool sqlOk = query.exec("SELECT COUNT (*) FROM log");

    if (sqlOk)
    {
        query.next();
        qsos = (query.value(0)).toInt();
        query.finish();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END"  ;
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

                     //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: band found: " << bandtxt ;
                QSqlQuery query2;
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
                                     //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt ;
                            }
                            else
                            {
                                    //queryErrorManagement(Q_FUNC_INFO, query3.lastError().databaseText(), query3.lastError().nativeErrorCode(), query3.lastQuery());
                                    //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-2"  ;
                                    //qDebug() << "DataBase::updateBandIdTableLogToNewOnes - QSOs not updated to main log" ;
                            }
                        }
                        else
                        {
                                 //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: query2 not valid "   ;
                        }
                    }
                    else
                    {
                          //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: query2 not next "   ;
                    }
                    query2.finish();
                }
                else
                {
                    queryErrorManagement(Q_FUNC_INFO, query2.lastError().databaseText(), query2.lastError().nativeErrorCode(), query2.lastQuery());
                    query2.finish();
                         //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-1"  ;
                }
            }

            if ( progress.wasCanceled() )
            {
                if (!alreadyCancelled)
                {
                    alreadyCancelled = true;

                    QMessageBox msgBox;
                    msgBox.setWindowTitle(QObject::tr("KLog - DB update"));
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
                            progress.setCancelButton(nullptr);
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
                //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END 2"  ;
            return false;
        }
            //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: END OK"  ;
        return true;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
            //qDebug() << "DataBase::updateBandIdTableLogToNewOnes: FALSE END 3"  ;
        return false;
    }
}


/*

bool DataBase::updateModeIdTableLogToNewOnes()
{
        //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: "  ;

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

                     //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: mode found: " << bandtxt ;

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
                                     //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: ID: " << QString::number(id) << " updated to: " << QString::number(bandFound) <<"/"<< bandtxt ;
                            }
                            else
                            {
                                     //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-2"  ;
                                    //qDebug() << "DataBase::updateModeIdTableLogToNewOnes - QSOs not updated to main log" ;
                                errorCode = query3.lastError().nativeErrorCode();
                                    //qDebug() << "DataBase::updateModeIdTableLogToNewOnes - query error: " << QString::number(errorCode) ;
                                    //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastQuery: " << query3.lastQuery()  ;
                                    //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastError-data: " << query3.lastError().databaseText()  ;
                                    //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastError-driver: " << query3.lastError().driverText()  ;
                                    //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: LastError-n: " << QString::number(query3.lastError().nativeErrorCode() ) ;

                            }

                        }
                        else
                        {
                                 //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: query2 not valid "   ;
                        }
                    }
                    else
                    {
                          //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: query2 not next "   ;
                    }

                }
                else
                {
                         //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: ID: " << QString::number(id) << " NOT updated-1"  ;
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
             //qDebug() << "DataBase::updateModeIdTableLogToNewOnes: FINISHED OK"  ;
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
       //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn007 = false;
    bool IAmIn006 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();

    QSqlQuery query;
    latestReaded = getDBVersion();
    bool sqlOk = false;

    if (latestReaded >= 0.007f)
    {
             //qDebug() <<  Q_FUNC_INFO << " -- I am in 007" ;
        return true;
    }
    else
    {
             //qDebug() <<  Q_FUNC_INFO << " -- I am not in 007" ;
        IAmIn007 = false;
    }


    while (!IAmIn007 && !ErrorUpdating)
    {
        while (!IAmIn006 && !ErrorUpdating)
        {
                //qDebug() <<  Q_FUNC_INFO << " -- And I am not in 006" ;
            IAmIn006 = updateTo006();
        }
           //qDebug() <<  Q_FUNC_INFO << " -- And I am in 006!!!!" ;
        if (ErrorUpdating)
        {
               //qDebug() <<  Q_FUNC_INFO << " -NOK-1" ;
             // emit debugLog(Q_FUNC_INFO, "1", 7);

            return false;
        }
        sqlOk = updateDBVersion(softVersion, "0.007");

        if (sqlOk)
        { // Version updated
            IAmIn007 = updateTableLog(6);
        }
        else
        { // Version not updated
               //qDebug() <<  Q_FUNC_INFO << " -NOK-2" ;
              // emit debugLog(Q_FUNC_INFO, "2", 7);

            return false;
        }
        //DO ALL THE TASKS TO BE IN 0.007 from 0.006 HERE and set ErrorUpdating if it is not possible.
        IAmIn007 = true;
    }
       //qDebug() <<  Q_FUNC_INFO << " -END" ;
    if (!IAmIn007)
    {
       // emit debugLog(Q_FUNC_INFO, "3", 7);
    }
    return IAmIn007;
}


bool DataBase::updateTo008()
{// Updates the DB to 0.0.8
    //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn008 = false;
    bool IAmIn007 = false;

    latestReaded = getDBVersion();
    if (latestReaded >= 0.008f)
    {
          //qDebug() <<  Q_FUNC_INFO << " -- I am in 008" ;
        return true;
    }
    else
    {
          //qDebug() <<  Q_FUNC_INFO << " -- I am not in 008" ;
        IAmIn008 = false;
    }


    while (!IAmIn008)
    {
        while (!IAmIn007 )
        {
               //qDebug() <<  Q_FUNC_INFO << " -- And I am not in 007" ;
            IAmIn007 = updateTo007();
        }
           //qDebug() <<  Q_FUNC_INFO << " -- I am in 007" ;


        //DO ALL THE TASKS TO BE IN 0.008 from 0.007 HERE and set ErrorUpdating if it is not possible.

        IAmIn008 = updateTheModeTableAndSyncLog();
        if (IAmIn008)
        {
            bool sqlOk = updateDBVersion(softVersion, "0.008");
            if (sqlOk)
            { // Version updated
                   //qDebug() <<  Q_FUNC_INFO << " -- OK" ;
            }
            else
            { // Version not updated
                   //qDebug() <<  Q_FUNC_INFO << " -- NOK-3" ;
               // emit debugLog(Q_FUNC_INFO, "2", 7);
                return false;
            }
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- NOK-2" ;
           // emit debugLog(Q_FUNC_INFO, "3", 7);
            return false;
        }
    }
       //qDebug() <<  Q_FUNC_INFO << " -- END" ;
    if (!IAmIn008)
    {/*emit debugLog(Q_FUNC_INFO, "4", 7);*/
    }
    return IAmIn008;
}


bool DataBase::updateTo009()
{// Updates the DB to 0.0.9 - We add the Satellite tables
       //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn009 = false;
    bool IAmIn008 = false;
    bool ErrorUpdating = false;

    //qDebug() <<  Q_FUNC_INFO << " -Checking:" << QString::number(latestReaded) << ":" << QString::number(0.009);
    latestReaded = getDBVersion();
   if (latestReaded >= 0.009f)
    //if ((latestReaded = 0.009) || (latestReaded > 0.009))
    {
            //qDebug() <<  Q_FUNC_INFO << " -- I am in 009" ;
        //IAmIn009 = true;
        return true;
    }
    else
    {
             //qDebug() <<  Q_FUNC_INFO << " -- I am not in 009 I am in: " << QString::number(latestReaded);
        IAmIn009 = false;
    }
        //qDebug() <<  Q_FUNC_INFO << " -compared latestRead: " << QString::number(latestReaded) ;
    while (!IAmIn009 && !ErrorUpdating)
    {
        while (!IAmIn008 && !ErrorUpdating)
        {
               //qDebug() <<  Q_FUNC_INFO << " -- And I am not in 008" ;
            IAmIn008 = updateTo008();
        }
           //qDebug() <<  Q_FUNC_INFO << " -- And I am already at least in 008" ;
        if (ErrorUpdating)
        {
           // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
        bool sqlOk = updateDBVersion(softVersion, "0.009");

        if (sqlOk)
        { // Version updated
               //qDebug() <<  Q_FUNC_INFO << " -- version updated" ;
            //IAmIn009 = updateTableLog(6);
        }
        else
        { // Version not updated
               //qDebug() <<  Q_FUNC_INFO << " -- version not updated" ;
        }
        //DO ALL THE TASKS TO BE IN 0.009 from 0.008 HERE and set ErrorUpdating if it is not possible.
        if (recreateSatelliteData())
        //if (createTableSatellites(true))
        {
               //qDebug() <<  Q_FUNC_INFO << " -- createTableSatellites OK" ;
            //if (populateTableSatellites(true))

            if (recreateTableEntity())
            {
                  //qDebug() <<  Q_FUNC_INFO << " -- recreateTableEntity OK" ;
               if (updateTheEntityTableISONames())
               {
                     //qDebug() <<  Q_FUNC_INFO << " -- isonames updated" ;
                  // Now I need to update the logs table

                  if (updateTableLogs())
                  {
                         //qDebug() <<  Q_FUNC_INFO << " -- logs updated and Function finished successfuly!!" ;
                      IAmIn009 = true;
                  }
                  else
                  {
                         //qDebug() <<  Q_FUNC_INFO << " -- logs NOT updated" ;
                      IAmIn009 = false;
                      ErrorUpdating = true;
                  }
               }
               else
               {
                     //qDebug() <<  Q_FUNC_INFO << " -- isonames NOT updated" ;
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
               //qDebug() <<  Q_FUNC_INFO << " -- createTableSatellites FALSE" ;
            ErrorUpdating = true;
            IAmIn009 = false;
        }
    }

         //qDebug() <<  Q_FUNC_INFO << " -- END" ;
    if (!IAmIn009)
    {
       // emit debugLog(Q_FUNC_INFO, "1", 7);
    }
    return IAmIn009;
}


bool DataBase::updateTo010()
{// Updates the DB to 0.010:
    // We add FT8 mode and
    // AS Propagation

       //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn010 = false;
    bool IAmIn009 = false;
    bool ErrorUpdating = false;
    QString stringQuery = QString();
    QSqlQuery query;

    latestReaded = getDBVersion();
        //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << QString::number(latestReaded) << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.010f)
    {
           //qDebug() <<  Q_FUNC_INFO << " -- I am in 010" ;
        IAmIn010 = true;
        return true;
    }
    else
    {
             //qDebug() <<  Q_FUNC_INFO << " -- I am not in 010 I am in: " << QString::number(latestReaded);

        while (!IAmIn009 && !ErrorUpdating)
        {
                 //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 009: !" ;
            IAmIn009 = updateTo009();

            if (IAmIn009)
            {
                     //qDebug() <<  Q_FUNC_INFO << " -- updateTo009 returned TRUE - I am in 0.009: " << QString::number(latestReaded) ;
            }
            else
            {
                     //qDebug() <<  Q_FUNC_INFO << " -- updateTo009 returned FALSE - I am NOT in 0.009: " << QString::number(latestReaded) ;
                ErrorUpdating = false;
            }
        }

        if (ErrorUpdating)
        {
                 //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
           // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
    }


    bool sqlOk = execQuery(Q_FUNC_INFO, "UPDATE band SET lower = '0.1357', upper = '0.1378' WHERE name='2190M'");
    if (sqlOk)
    {
             //qDebug() <<  Q_FUNC_INFO << " -- Band update OK" ;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
             //qDebug() <<  Q_FUNC_INFO << " -- Band update NOK" ;
    }


    execQuery(Q_FUNC_INFO, QString("INSERT INTO mode (submode, name, cabrillo, deprecated) VALUES ('FT8', 'FT8', 'NO', '0')"));
    execQuery(Q_FUNC_INFO, QString("INSERT INTO prop_mode_enumeration (shortname, name) VALUES ('AS', 'Aircraft Scatter')"));


    if (updateDBVersion(softVersion, "0.010"))
    {
             //qDebug() <<  Q_FUNC_INFO << " -- We are in 010! " ;
        IAmIn010 = true;
    }
    else
    {
             //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 010! " ;
        IAmIn010 = false;
       // emit debugLog(Q_FUNC_INFO, "1", 7);
    }

         //qDebug() <<  Q_FUNC_INFO << " -- END" ;
    return IAmIn010;
}

bool DataBase::updateDBVersion(QString _softV, QString _dbV)
{
    QString dateString = util->getDateSQLiteStringFromDate(QDate::currentDate());
    //qDebug() << "DataBase::updateDBVersion: (date/SoftVersion/dbVersion): " << dateString << "/" << _softV << "/" << _dbV ;
    QString stringQuery = "INSERT INTO softwarecontrol (dateupgrade, softversion, dbversion) VALUES ('" + dateString + "', '" + _softV + "', '" + _dbV + "')";
    return execQuery(Q_FUNC_INFO, stringQuery);
}


bool DataBase::updateTheModeTableAndSyncLog()
{
         //qDebug() << "DataBase::updateTheModeTableAndSyncLog" ;
    QSqlQuery query;

    createTableMode(false);         // Create modetemp
    populateTableMode(false);       // Populate modetemp

    updateModeIdFromSubModeId();    // Updates the log with the new mode IDs in each QSO
    //updateModeIdTableAward(1); //DXCC
    //updateModeIdTableAward(2); // WAZ


    bool sqlOK = execQuery(Q_FUNC_INFO, "DROP TABLE mode");
    if (sqlOK)
    {
             //qDebug() << "DataBase::updateTheModeTableAndSyncLog - OK - mode was dropped" ;
        sqlOK = execQuery(Q_FUNC_INFO, "ALTER TABLE modetemp RENAME TO mode");
        if (!sqlOK)
        {
           // emit debugLog(Q_FUNC_INFO, "1", 7);
        }
        return sqlOK;
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug() << "DataBase::updateTheModeTableAndSyncLog - ERROR - modetemp not dropped" ;
       // emit debugLog(Q_FUNC_INFO, "2", 7);
        return false;
    }
         //qDebug() << "DataBase::updateTheModeTableAndSyncLog END" ;
    //return true;
}

bool DataBase::recreateTableBand()
{
    //qDebug() << "DataBase::recreateTableBand" ;
    QSqlQuery query;

    createTableBand(false);         // Create modetemp
    populateTableBand(false);       // Populate modetemp
    syncLogQSOsOnBandTableChange();

    if (execQuery(Q_FUNC_INFO, "DROP TABLE band"))
    {
       // emit debugLog(Q_FUNC_INFO, "1", 7);
        return execQuery(Q_FUNC_INFO, "ALTER TABLE bandtemp RENAME TO band");
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            //qDebug() << "DataBase::recreateTableBand - ERROR - bandtemp not dropped" ;
       // emit debugLog(Q_FUNC_INFO, "2", 7);
        return false;
    }
        //qDebug() << "DataBase::recreateTableBand END" ;
}

QMultiMap<QString, int> DataBase::fillCountryCodes()
{
    //qDebug() << Q_FUNC_INFO;
    QMultiMap<QString, int> countryCodes;
    countryCodes.clear ();
    countryCodes.insert ("mt", 246);
    countryCodes.insert ("un", 247);
    countryCodes.insert ("mc", 260);
    countryCodes.insert ("mu", 4);
    countryCodes.insert ("mu", 165);
    countryCodes.insert ("mu", 207);
    countryCodes.insert ("gq", 49);
    countryCodes.insert ("gq", 195);
    countryCodes.insert ("fj", 176);
    countryCodes.insert ("fj", 489);
    countryCodes.insert ("fj", 460);
    countryCodes.insert ("sz", 468);
    countryCodes.insert ("tn", 474);
    countryCodes.insert ("vn", 293);
    countryCodes.insert ("gn", 107);
    countryCodes.insert ("bv", 24);
    countryCodes.insert ("no", 199);
    countryCodes.insert ("az", 18);
    countryCodes.insert ("ge", 75);
    countryCodes.insert ("me", 514);
    countryCodes.insert ("lk", 315);
    countryCodes.insert ("ch", 177);  // ITU HQ
    countryCodes.insert ("us", 289);  // UN HQ
    countryCodes.insert ("tl", 511);  // Timor Leste
    countryCodes.insert ("il", 336);  // Israel
    countryCodes.insert ("ly", 436);  // Libya
    countryCodes.insert ("cy", 215);  // Cyprus
    countryCodes.insert ("tz", 470);  // Tanzania
    countryCodes.insert ("ng", 450);  // Nigeria
    countryCodes.insert ("mg", 438);  // Madagascar
    countryCodes.insert ("mr", 444);  // Mauritania
    countryCodes.insert ("ne", 187);  // Niger
    countryCodes.insert ("tg", 483);  // Togo
    countryCodes.insert ("ws", 190);  // Samoa
    countryCodes.insert ("ug", 286);  // Uganda
    countryCodes.insert ("ke", 430);  // Kenya
    countryCodes.insert ("sn", 456);  // Senegal
    countryCodes.insert ("jm", 82);   // Jamaica
    countryCodes.insert ("es", 281);  // Spain
    countryCodes.insert ("ls", 432);  // Lesotho
    countryCodes.insert ("mw", 440);  // Malawi
    countryCodes.insert ("dz", 400);  // Algeria
    countryCodes.insert ("ye", 492);  // Yemen
    countryCodes.insert ("bb", 62);   // Barbados
    countryCodes.insert ("mv", 159);  // Maldives
    countryCodes.insert ("gy", 129);  // Guyana
    countryCodes.insert ("hr", 497);  // Croatia
    countryCodes.insert ("gh", 424);  // Ghana
    countryCodes.insert ("mt", 257);  // Malta
    countryCodes.insert ("zm", 482);  // Zambia
    countryCodes.insert ("kw", 348);  // Kuwait
    countryCodes.insert ("sl", 458);  //Sierra Leone
    countryCodes.insert ("my", 299);  // West Malaysia
    countryCodes.insert ("my", 46);   // East Malaysia
    countryCodes.insert ("np", 369);  // Nepal
    countryCodes.insert ("cd", 414);  // Dem Rep Congo
    countryCodes.insert ("bi", 404);  // Burundi
    countryCodes.insert ("sg", 381);  // Singapore
    countryCodes.insert ("rw", 454);  // Rwanda
    countryCodes.insert ("tt", 90);   // Trinidad & Tobago
    countryCodes.insert ("bw", 402);
    countryCodes.insert ("to", 160);
    countryCodes.insert ("om", 370);
    countryCodes.insert ("bt", 306);  // Bhutan
    countryCodes.insert ("ae", 391);  // Un Arab Emirates
    countryCodes.insert ("qa", 376);  // Qatar
    countryCodes.insert ("bh", 304);  // Bahrain
    countryCodes.insert ("pk", 372);  // Pakistan
    countryCodes.insert ("tw", 386);  // Taiwan
    countryCodes.insert ("tw", 505);  // Pratas Is
    countryCodes.insert ("cn", 318);  // China
    countryCodes.insert ("nr", 157);  //  Nauru
    countryCodes.insert ("ad", 203);  // Andorra
    countryCodes.insert ("gm", 422);  // Gambia
    countryCodes.insert ("bs", 60);
    countryCodes.insert ("mz", 181);  // Mozambique
    countryCodes.insert ("cl", 112);  // Chile
    countryCodes.insert ("cl", 217);  // San Felix
    countryCodes.insert ("cl", 47);   // Easter Is
    countryCodes.insert ("cl", 125);  //  Juan Fernandez is
    countryCodes.insert ("cu", 70);   // Cuba
    countryCodes.insert ("ma", 446);  // Morocco
    countryCodes.insert ("bo", 104);  // Bolivia
    countryCodes.insert ("pt", 272);  // Portugal
    countryCodes.insert ("pt", 256);  // Madeira
    countryCodes.insert ("pt", 149);  // Azores
    countryCodes.insert ("uy", 144);  // Uruguay
    countryCodes.insert ("ca", 211);  // Sable Is
    countryCodes.insert ("ca", 252);  // St Paul is
    countryCodes.insert ("ao", 401);  // Angola
    countryCodes.insert ("cv", 409);  // Cape Verde
    countryCodes.insert ("km", 411);  // Comoros
    countryCodes.insert ("de", 230);  // Fed Rep Germany
    countryCodes.insert ("ph", 375);  //Philippines
    countryCodes.insert ("er", 51);   // Eritrea
    countryCodes.insert ("ps", 510);  // Palestine
    countryCodes.insert ("ck", 191);  // North Cook
    countryCodes.insert ("ck", 234);  // South Cook
    countryCodes.insert ("nu", 188);  // Niue
    countryCodes.insert ("ba", 501);  // Bosnia
    countryCodes.insert ("balear", 21); // Balearic is
    countryCodes.insert ("canary", 29); // Canary Is
    countryCodes.insert ("ceuta", 32);  // Ceuta & Melilla //TODO: Fix the flag
    countryCodes.insert ("ie", 245); // Ireland
    countryCodes.insert ("am", 14);
    countryCodes.insert ("lr", 434);
    countryCodes.insert ("ir", 330);  // Iran
    countryCodes.insert ("mv", 179); // Moldova
    countryCodes.insert ("ee", 52);  //  Estonia
    countryCodes.insert ("et", 53);  // Ethiopia
    countryCodes.insert ("by", 27);  // Belarus
    countryCodes.insert ("kg", 135); // Kyrgyzstan
    countryCodes.insert ("tm", 280); // Turkmenistan
    countryCodes.insert ("tj", 262); // Tajikistan
    countryCodes.insert ("fr", 227); // France
    countryCodes.insert ("fr", 79);  // Guadeloupe
    countryCodes.insert ("yt", 169); // Mayotte
    countryCodes.insert ("fr", 516); // St Barthelemy
    countryCodes.insert ("fr", 36);  // Clipperton Is
    countryCodes.insert ("nc", 162);  // New Caledonia
    countryCodes.insert ("nc", 512);  // Chesterfield Is
    countryCodes.insert ("mq", 84);   // Martinique
    countryCodes.insert ("pf", 175);  // French Polynesia
    countryCodes.insert ("pf", 508);  // Austral Is
    countryCodes.insert ("pm", 277);  //  St Pierre & Miquelon
    countryCodes.insert ("re", 453);  //Reunion Is
    countryCodes.insert ("fr", 213); // St Marteen
    countryCodes.insert ("fr", 99);  // Glorioso is
    countryCodes.insert ("fr", 124); // Juan de nova, Europa
    countryCodes.insert ("fr", 276);  //  Tromelin - TODO: Add the wikipedia flag
    countryCodes.insert ("fr", 41);  // Crozet
    countryCodes.insert ("fr", 131);  // Kerguelen
    //TODO: Add the wikipedia flag http://es.wikipedia.org/wiki/Tierras_Australes_y_Ant%C3%A1rticas_Francesas
    countryCodes.insert ("fr", 10);  //Amsterdam & St Paul is
    //TODO: Add the wikipedia flag
    countryCodes.insert ("wf", 298);  // Wallis & Futuna is
    countryCodes.insert ("gf", 63);  // French Guiana
    //TODO: Add the wikipedia flag
    countryCodes.insert ("england", 223);  // England
    countryCodes.insert ("gb", 114); //Isle of Man
    //TODO: Add the wikipedia flag
    countryCodes.insert ("northernireland", 265); // Northern Ireland
    countryCodes.insert ("gb", 122);  // Jersey
    countryCodes.insert ("scotland", 279);  //  Scotland
    countryCodes.insert ("scotland", 1279);  //  Shetland is (Scotland)
    countryCodes.insert ("gb", 106);  // Guernsey
    //TODO: Add the wikipedia flag
    countryCodes.insert ("wales", 294);  // Wales
    countryCodes.insert ("sb", 185); // Solomon
    countryCodes.insert ("sb", 507);  // Temotu Province
    countryCodes.insert ("hu", 239);  // Hungary
    countryCodes.insert ("ch", 287);  // Switzerland
    countryCodes.insert ("li", 251); // Liechtenstein
    countryCodes.insert ("ec", 120);  // Ecuador
    countryCodes.insert ("ec", 71);  // Galapagos Is
    countryCodes.insert ("ht", 78);  // Haiti
    countryCodes.insert ("do", 72);  // Dominican Rep
    countryCodes.insert ("co", 116);  // Colombia
    countryCodes.insert ("co", 216);  // San Andres & Providencia
    countryCodes.insert ("co", 161);  // Malpelo
    countryCodes.insert ("kr", 137);  // Rep Korea
    countryCodes.insert ("pa", 88);  // Panama
    countryCodes.insert ("hn", 80); // Honduras
    countryCodes.insert ("th", 387);  // Thailand
    countryCodes.insert ("va", 295);
    countryCodes.insert ("sa", 378); // Saudi Arabia
    countryCodes.insert ("it", 248);  // Italy
    countryCodes.insert ("it", 1248);  // African Italy
    countryCodes.insert ("it", 2248); // Sicily
    countryCodes.insert ("it", 225);  // Sardinia
    countryCodes.insert ("dj", 382);  // Djibouti
    countryCodes.insert ("gd", 77);  // Grenada
    countryCodes.insert ("gw", 109);  // Guinea-Bissau
    countryCodes.insert ("lc", 97);  // St Lucia
    countryCodes.insert ("dm", 95);  // Dominica
    countryCodes.insert ("vc", 98);  // St Vicent
    countryCodes.insert ("jp", 339);  // Japan
    countryCodes.insert ("jp", 177);  // Minami Torishima
    countryCodes.insert ("jp", 192);  // Ogasawara
    countryCodes.insert ("mn", 363);  // Mongolia
    countryCodes.insert ("sj", 259);  // Svalbard
    countryCodes.insert ("sj", 1259);  // Svalbard
    countryCodes.insert ("sj", 118);  // Jan Mayen
    countryCodes.insert ("jo", 342);
    countryCodes.insert ("us", 291);
    countryCodes.insert ("us", 105);
    countryCodes.insert ("mp", 166);
    countryCodes.insert ("us", 20);
    countryCodes.insert ("gu", 103);
    countryCodes.insert ("us", 123);
    countryCodes.insert ("us", 174);
    countryCodes.insert ("us", 197);
    countryCodes.insert ("us", 134);
    countryCodes.insert ("us", 110);  // Hawaii
    //TODO: Add the wikipedia flag
    countryCodes.insert ("us", 138);  // Kure is
    countryCodes.insert ("as", 9);  // American Samoa
    countryCodes.insert ("as", 515);  // Swains is
    countryCodes.insert ("us", 297);  // Wake is
    countryCodes.insert ("us", 6); // Alaska
    countryCodes.insert ("us", 182);  // Navassa Is
    countryCodes.insert ("vi", 285);  // Us Virgin is
    countryCodes.insert ("pr", 202);  // Puerto Rico
    countryCodes.insert ("us", 43);  // Desecheo Is
    countryCodes.insert ("no", 266);  // Norway
    countryCodes.insert ("ar", 100);  // Argentina
    countryCodes.insert ("lu", 254);  // Luxembourg
    countryCodes.insert ("lt", 146);  // Lithuania
    countryCodes.insert ("bg", 212);  // Bulgaria
    countryCodes.insert ("pe", 136);  // Peru
    countryCodes.insert ("lb", 354);  // Lebanon
    countryCodes.insert ("at", 206);  // Austria & Viena Intl HQ
    countryCodes.insert ("fi", 224);  // Findland
    countryCodes.insert ("fi", 5); // Aland is
    countryCodes.insert ("fi", 167);  // Market Reef
    countryCodes.insert ("cz", 503);  // Czech Rep
    countryCodes.insert ("sk", 504);  // Slovak Rep
    countryCodes.insert ("be", 209);  // Belgium
    countryCodes.insert ("gl", 237);  // Greenland
    countryCodes.insert ("dk", 222); // Faroe is
    //TODO: Add the wikipedia flag
    countryCodes.insert ("dk", 221);  // Denmark
    countryCodes.insert ("pg", 163);  // Papua New Guinea
    countryCodes.insert ("aw", 91);  // Aruba
    countryCodes.insert ("kp", 344);  //Dpr Korea
    countryCodes.insert ("nl", 263); // Netherlands
    countryCodes.insert ("cw", 517);  // Curacao
    //TODO: Add the wikipedia flag
    countryCodes.insert ("bq", 520);  // Bonaire
    //TODO: Add the wikipedia flag
    countryCodes.insert ("nl", 519);  // Saba & St Eustatius
    //TODO: Add the wikipedia flag
    countryCodes.insert ("shm", 518);  // Sint Marteen
    //TODO: Add the wikipedia flag
    countryCodes.insert ("br", 108);  // Brazil
    countryCodes.insert ("br", 56);  // Fernando de Noronha
    countryCodes.insert ("br", 253);  //  St Peter & St Paul
    countryCodes.insert ("br", 273);  // Trindade & Martim Vaz
    countryCodes.insert ("sr", 140);  // Suriname
    countryCodes.insert ("ru", 61);  // Franz Josef Land
    countryCodes.insert ("eh", 302); // Western Sahara
    //TODO: Add the wikipedia flag
    countryCodes.insert ("bd", 305);  // Bangladesh
    countryCodes.insert ("si", 499);  // Slovenia
    countryCodes.insert ("sc", 379);  // Seychelles
    countryCodes.insert ("st", 219);  // Sao Tome & Principe
    countryCodes.insert ("se", 284);  // Sweden
    countryCodes.insert ("pl", 269);  // Poland
    countryCodes.insert ("sd", 466);  // Sudan
    countryCodes.insert ("eg", 478);  // Egypt
    countryCodes.insert ("gr", 236);  // Greece
    countryCodes.insert ("gr", 180);  // Mount Athos
    countryCodes.insert ("gr", 45);  // Dodecanese
    countryCodes.insert ("gr", 40);  // Crete
    countryCodes.insert ("tv", 282); // Tuvalu
    countryCodes.insert ("ki", 301); // Western Kiribati
    countryCodes.insert ("ki", 31);  // Central Kiribati
    countryCodes.insert ("ki", 48); // Eastern Kiribati
    countryCodes.insert ("ki", 490); // Banaba is
    countryCodes.insert ("so", 232);  // Somalia
    countryCodes.insert ("sm", 278); // San Marino
    countryCodes.insert ("pw", 22);  // Palau
    countryCodes.insert ("tr", 390);  // Turkey
    countryCodes.insert ("tr", 1390);  // European Turkey
    countryCodes.insert ("is", 242); // Iceland
    countryCodes.insert ("gt", 76); // Guatemala
    //TODO: Add the wikipedia flag
    countryCodes.insert ("cr", 308);  // Costa Rica
    countryCodes.insert ("cr", 37); // Coco is
    countryCodes.insert ("cm", 406);  // Cameroon
    countryCodes.insert ("fr", 214);  // Corsica
    // TODO: Add the wikipedia flag
    countryCodes.insert ("cf", 408);  // Central African Rep
    countryCodes.insert ("cg", 412);  // Rep of Congo
    countryCodes.insert ("ga", 420);  // Gabon
    countryCodes.insert ("td", 410);  // Chad
    countryCodes.insert ("ci", 428);  // Cote d'Ivoire
    countryCodes.insert ("bj", 416); // Benin
    countryCodes.insert ("ml", 442);  // Mali
    countryCodes.insert ("ru", 54); // European Russia
    countryCodes.insert ("ru", 15);  // Asiatic Russia
    countryCodes.insert ("ru", 126);  // Kaliningrad
    countryCodes.insert ("uz", 292);  // Uzbekistan
    countryCodes.insert ("kz", 130);  // Kazakhstan
    countryCodes.insert ("ua", 288);  // Ukraine
    countryCodes.insert ("ag", 94);  // Antigua & Barbuda
    countryCodes.insert ("bz", 66);  // Belize
    countryCodes.insert ("kn", 249);  // St Kitts & Nevis
    countryCodes.insert ("na", 464);  // Namibia
    countryCodes.insert ("fm", 173);  // Micronesia
    countryCodes.insert ("fm", 168);  // Marshall Is
    countryCodes.insert ("bn", 345); // Brunei Darusalam
    countryCodes.insert ("ca", 1);  // Canada
    countryCodes.insert ("au", 150);  // Australia
    countryCodes.insert ("hm", 111);  // Heard Is
    countryCodes.insert ("au", 153);  // Macquarie is
    countryCodes.insert ("cc", 38);  //  Cocos / Keeling is
    countryCodes.insert ("au", 147); //  Lord Howe is
    countryCodes.insert ("au", 171); // Mellish Reed
    countryCodes.insert ("nf", 189); // Norkfolk is
    countryCodes.insert ("au", 303);  // Willis Is
    countryCodes.insert ("cx", 35);  // Christmas is
    countryCodes.insert ("ai", 12); //  Anguilla
    countryCodes.insert ("ms", 96);  // Montserrat
    countryCodes.insert ("vg", 65);  // British is
    countryCodes.insert ("tc", 89);  // Turks & Caicos is
    countryCodes.insert ("pn", 172);  // Pitcairn
    countryCodes.insert ("gb", 513);  // Ducie is
    countryCodes.insert ("fk", 141);  // Falkland is
    countryCodes.insert ("gs", 235);  // South Georgia is
    countryCodes.insert ("southbritish", 241);  // South Shetland is
    countryCodes.insert ("un", 238);  // South Orkney is
    countryCodes.insert ("gs", 240);  // South Sandwich Is
    countryCodes.insert ("bm", 64);  // Bermuda
    countryCodes.insert ("io", 33);  // Chagos is
    countryCodes.insert ("hk", 321);  // Hong Kong
    countryCodes.insert ("in", 324);  // India
    countryCodes.insert ("in", 11);  // Andaman & Nicobar
    countryCodes.insert ("in", 142);  // Lakshadweep Is
    countryCodes.insert ("mx", 50);  // Mexico
    countryCodes.insert ("mx", 204);  // Revilagigedo
    countryCodes.insert ("bf", 480);  // Burkina Faso
    countryCodes.insert ("kh", 312); //  Cambodia
    countryCodes.insert ("la", 143);  // Laos
    countryCodes.insert ("mo", 152); // Macao
    countryCodes.insert ("mm", 309);  // Myanmar
    countryCodes.insert ("af", 3);  // Afganistan
    countryCodes.insert ("id", 327);  // Indonesia
    countryCodes.insert ("iq", 333);  // Iraq
    countryCodes.insert ("vu", 158);  // Vanuatu
    countryCodes.insert ("sy", 384); // Syria
    countryCodes.insert ("lv", 145);  // Latvia
    countryCodes.insert ("ni", 86);  //  Nicaragua
    countryCodes.insert ("ro", 275); // Romania
    countryCodes.insert ("sv", 74);  // El Salvador
    countryCodes.insert ("rs", 296); // Serbia
    countryCodes.insert ("ve", 148);  // Venezuela
    countryCodes.insert ("ve", 17); // Aves Is
    countryCodes.insert ("zw", 452);  // Zimbabwe
    countryCodes.insert ("mk", 502);  // Macedonia
    countryCodes.insert ("ss", 521);  //Rep South Sudan
    countryCodes.insert ("al", 7);  // Albania
    countryCodes.insert ("gi", 233);  // Gibraltar
    countryCodes.insert ("gb", 283);  // UK Base Aereas Cyprus
    countryCodes.insert ("sh", 250); // St Helena
    countryCodes.insert ("sh", 205);  // Ascension is
    countryCodes.insert ("sh", 274);  // Tristan da Cunha & Gough is
    countryCodes.insert ("ky", 69); // Cayman Is
    countryCodes.insert ("tk", 270);  // Tokelau Is
    countryCodes.insert ("marquesas", 509);  // Marquesas Is
    countryCodes.insert ("nz", 170);  // New Zeland
    countryCodes.insert ("nz", 34);  // Chatham Is
    countryCodes.insert ("nz", 133);  // Kermadec is
    countryCodes.insert ("nz", 16);  // Auckland & Campbell is
    countryCodes.insert ("py", 132);  // Paraguay
    countryCodes.insert ("za", 462);  // South Africa
    countryCodes.insert ("za", 201);  // Pr Edward & Marion Is
    countryCodes.insert ("xk", 522);  // Kosovo
    // Countries without flag or controversial - Data is added just to keep the DB filled-up
    countryCodes.insert ("un", 506);  // Scarboroug Reef
    countryCodes.insert ("un", 13); //  Antartica

    return countryCodes;
}

bool DataBase::updateTheEntityTableISONames()
{
    //qDebug() << "DataBase::updateTheEntityTableISONames" ;
    QSqlQuery query;
    QString sq;

    //First of all we will check if the entity table does contain data. We can't update something non existent!
    if (!hasTheTableData("entity"))
    {
       //qDebug() << Q_FUNC_INFO << " : Entity has NO data";
       // emit debugLog(Q_FUNC_INFO, "1", 7);
        return false;
    }
    QMultiMap<QString, int> countryCodes;
    countryCodes.clear ();

    countryCodes = fillCountryCodes ();

    foreach (const QString &str, countryCodes.uniqueKeys()) {
        foreach (int i, countryCodes.values(str))
        {
            if (!updateEntity (str, i))
            {
                //qDebug() << Q_FUNC_INFO << " : Update entity failed: " << str;
                return false;
            }
            //qDebug() << str << ':' << i;
        }
    }
    //qDebug() << "DataBase::updateTheEntityTableISONames-END" ;
    return true;
}

bool DataBase::updateEntity (const QString &_codeString, const int _code)
{
    //qDebug() << Q_FUNC_INFO << "ISOCode: " << _codeString << " - int: " << QString::number(_code);
    QString sq = QString ("UPDATE entity SET isoname='%1' WHERE dxcc='%2'").arg(_codeString).arg(_code);
    //TODO Capture and manage the error
    return execQuery(Q_FUNC_INFO, sq);
}

bool DataBase::doesEntityTablehaveData()
{
    QString sq = QString("SELECT COUNT(id) FROM entity");
    QSqlQuery query;
    if (!query.exec(sq))
        return false;
    else if(!query.next())
        return false;
    else if (!query.isValid())
        return false;

    return ((query.value(0)).toInt() > 0);
}

bool DataBase::isTheTableExisting(const QString &_tableName)
{
    QSqlQuery query;

    QString sq = QString("SELECT COUNT(name) FROM sqlite_master WHERE name='%1'").arg(_tableName);
    if (query.exec(sq))
        return false;

    if (!query.next())
        return false;

    if (!query.isValid())
        return false;

    int _n = (query.value(0)).toInt();
    query.finish();
    return (_n>0);
}



bool DataBase::hasTheTableData(const QString &_tableName)
{
    //qDebug() << Q_FUNC_INFO << ": " << _tableName ;
    QSqlQuery query;

    QString stringQuery = QString("SELECT count(id) FROM %1").arg(_tableName);
    if (!query.exec(stringQuery))
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return false;
    }

    if (!query.next())
        return false;

    if (!query.isValid())
        return false;

    int _num = (query.value(0)).toInt();
    query.finish();
    return (_num > 0);
}

bool DataBase::updateTo011()
{// Updates the DB to 0.011:
    // We add FT8 mode and
    // AS Propagation


       //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn011 = false;
    bool IAmIn010 = false;

    QString stringQuery = QString();
    //QSqlQuery query;

    //bool sqlOk = false;
    latestReaded = getDBVersion();
       //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << QString::number(latestReaded) << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.011f)
    {
           //qDebug() <<  Q_FUNC_INFO << " -- I am in 011" ;
        IAmIn011 = true;
        return true;
    }
    else
    {
           //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.012 I am in: " << QString::number(latestReaded);
        bool ErrorUpdating = false;
        while (!IAmIn010 && !ErrorUpdating)
        {
               //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 010: !" ;
            IAmIn010 = updateTo010();

            if (IAmIn010)
            {
                     //qDebug() <<  Q_FUNC_INFO << " -- updateTo010 returned TRUE - I am in 0.010: " << QString::number(latestReaded) ;
            }
            else
            {
                   //qDebug() <<  Q_FUNC_INFO << " -- updateTo009 returned FALSE - I am NOT in 0.010: " << QString::number(latestReaded) ;
                ErrorUpdating = false;
            }
        }

        if (ErrorUpdating)
        {
                 //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
           // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
    }

    if (!recreateSatelliteData())
    {
           //qDebug() <<  Q_FUNC_INFO << " -- Sats update NOK " ;
       // emit debugLog(Q_FUNC_INFO, "2", 7);
        return false;
    }

    //if (!recreateTableDXCC())
    //{
        //qDebug() <<  Q_FUNC_INFO << " -- recreateTable DXCC NOK " ;
       // emit debugLog(Q_FUNC_INFO, "3", 7);
    //    return false;
    //}


    if(!execQuery(Q_FUNC_INFO, "INSERT INTO mode (submode, name, cabrillo, deprecated) VALUES ('MSK144', 'MSK144', 'NO', '0')"))
    {
           //qDebug() <<  Q_FUNC_INFO << " -- MSK NOK " ;
       // emit debugLog(Q_FUNC_INFO, "5", 7);
        return false;
    }


    if (!recreateTableLog())
    {
           //qDebug() <<  Q_FUNC_INFO << " -- Failed to recreate Table Log " ;
       // emit debugLog(Q_FUNC_INFO, "6", 7);
        return false;
    }

    if (updateDBVersion(softVersion, "0.011"))
    {
           //qDebug() <<  Q_FUNC_INFO << " -- We are in 011! " ;
        IAmIn011 = true;
    }
    else
    {
           //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 011! " ;
        IAmIn011 = false;
    }

       //qDebug() <<  Q_FUNC_INFO << " -- END" ;
    if (!IAmIn011)
    {// emit debugLog(Q_FUNC_INFO, "8", 7);
    }
    return IAmIn011;
}


bool DataBase::updateTo012()
{
    // Updates the DB to 0.012:
    // Change the table qsl_via to qsl_via_enumeration
    // Solve the supportedContest table missing bug in some deployments


           //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
        bool IAmIn011 = false;
        bool ErrorUpdating = false;
        latestReaded = getDBVersion();
           //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
        if (latestReaded >= 0.012f)
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am in 012" ;
            return true;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.012 I am in: " << getDBVersion() ;
            while (!IAmIn011 && !ErrorUpdating)
            {
                   //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 011: !" ;
                IAmIn011 = updateTo011();

                if (IAmIn011)
                {
                         //qDebug() <<  Q_FUNC_INFO << " -- updateTo011 returned TRUE - I am in 0.011: " << QString::number(latestReaded) ;
                }
                else
                {
                        //qDebug() <<  Q_FUNC_INFO << " -- updateTo011 returned FALSE - I am NOT in 0.011: " << QString::number(latestReaded) ;
                    ErrorUpdating = false;
                }
            }
            if (ErrorUpdating)
            {
                    //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
                // emit debugLog(Q_FUNC_INFO, "1", 7);
                return false;
            }
        }

        // Now I am in the previous version and I can update the DB.
        //bool sqlOK = false;

        if (isTheTableExisting("supportedcontest"))
        {
            bool sqlOK = execQuery(Q_FUNC_INFO, "DROP TABLE supportedcontest");
            if (sqlOK)
            {
                if (createTableSupportedContest())
                {
                    sqlOK = recreateSupportedContest();
                }
                else
                {
                       //qDebug() <<  Q_FUNC_INFO << " -UPDATED NOK-1!" ;
                   // emit debugLog(Q_FUNC_INFO, "2", 7);
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
                      //qDebug() <<  Q_FUNC_INFO << " -UPDATED NOK-2!" ;
                  // emit debugLog(Q_FUNC_INFO, "3", 7);
                   return false;
               }
            }
        }

        if (updateDBVersion(softVersion, "0.012"))
        {
               //qDebug() <<  Q_FUNC_INFO << " -- We are in 012! " ;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 012! " ;
           // emit debugLog(Q_FUNC_INFO, "4", 7);
            return false;
        }

           //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
        return true;
}

bool DataBase::updateTo013()
{
    // Updates the DB to 0.013:
    // Change the table qsl_via to qsl_via_enumeration
    // Solve the supportedContest table missing bug in some deployments


           //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
        bool IAmIn012 = false;
        bool ErrorUpdating = false;
        latestReaded = getDBVersion();
           //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
        if (latestReaded >= 0.013f)
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am in 013" ;
            return true;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.013 I am in: " << getDBVersion() ;
            while (!IAmIn012 && !ErrorUpdating)
            {
                   //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 012: !" ;
                IAmIn012 = updateTo012();

                if (IAmIn012)
                {
                         //qDebug() <<  Q_FUNC_INFO << " -- updateTo012 returned TRUE - I am in 0.012: " << QString::number(latestReaded) ;
                }
                else
                {
                        //qDebug() <<  Q_FUNC_INFO << " -- updateTo011 returned FALSE - I am NOT in 0.012: " << QString::number(latestReaded) ;
                    ErrorUpdating = false;
                }
            }
            if (ErrorUpdating)
            {
                    //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
               // emit debugLog(Q_FUNC_INFO, "1", 7);
                return false;
            }
        }

        // Now I am in the previous version and I can update the DB.

        if (!recreateSupportedContest())
        {
           // emit debugLog(Q_FUNC_INFO, "2", 7);
            /*emit debugLog(Q_FUNC_INFO, "0", 7);*/
            return false;
        }

         execQuery(Q_FUNC_INFO, "DROP TABLE IF exists qsl_via");
         if (createTableQSL_Via_enumeration())
         {
             if (!populateTableQSL_Via_enumeration())
             {
                // emit debugLog(Q_FUNC_INFO, "3", 7);
                 return false;
             }
         }
         else
         {
            // emit debugLog(Q_FUNC_INFO, "4", 7);
             return false;
         }
        if (updateDBVersion(softVersion, "0.013"))
        {
               //qDebug() <<  Q_FUNC_INFO << " -- We are in 013! " ;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 013! " ;
           // emit debugLog(Q_FUNC_INFO, "5", 7);
            return false;
        }

           //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
        return true;
}


bool DataBase::updateTo014()
{
    // Updates the DB to 0.014:
    // Updates the Satellite DB


           //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
        bool IAmIn013 = false;
        bool ErrorUpdating = false;
        latestReaded = getDBVersion();
           //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
        if (latestReaded >= 0.014f)
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am in 013" ;
            return true;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.013 I am in: " << getDBVersion() ;
            while (!IAmIn013 && !ErrorUpdating)
            {
                   //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 013: !" ;
                IAmIn013 = updateTo013();

                if (IAmIn013)
                {
                         //qDebug() <<  Q_FUNC_INFO << " -- updateTo012 returned TRUE - I am in 0.013: " << QString::number(latestReaded) ;
                }
                else
                {
                        //qDebug() <<  Q_FUNC_INFO << " -- updateTo011 returned FALSE - I am NOT in 0.013: " << QString::number(latestReaded) ;
                    ErrorUpdating = false;
                }
            }
            if (ErrorUpdating)
            {
                    //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
               // emit debugLog(Q_FUNC_INFO, "1", 7);
                return false;
            }
        }

        // Now I am in the previous version and I can update the DB.


        if (!recreateSatelliteData())
        {
               //qDebug() <<  Q_FUNC_INFO << " -- Sats update NOK " ;
           // emit debugLog(Q_FUNC_INFO, "2", 7);
            return false;
        }


        if (updateDBVersion(softVersion, "0.014"))
        {
               //qDebug() <<  Q_FUNC_INFO << " -- We are in 013! " ;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 013! " ;
           // emit debugLog(Q_FUNC_INFO, "3", 7);
            return false;
        }

           //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
        return true;
}


bool DataBase::updateTo015()
{
    // Updates the DB to 0.015:
    // Adds the FT4 mode

    //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn014 = false;
    bool ErrorUpdating = false;
    latestReaded = getDBVersion();
      //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.015f)
    {
          //qDebug() <<  Q_FUNC_INFO << " -- I am in 013" ;
        return true;
    }
    else
    {
          //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.014 I am in: " << getDBVersion() ;
        while (!IAmIn014 && !ErrorUpdating)
        {
              //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 014: !" ;
            IAmIn014 = updateTo014();
            if (IAmIn014)
            {
                  //qDebug() <<  Q_FUNC_INFO << " -- updateTo013 returned TRUE - I am in 0.014: " << QString::number(latestReaded) ;
            }
            else
            {
                  //qDebug() <<  Q_FUNC_INFO << " -- updateTo011 returned FALSE - I am NOT in 0.014: " << QString::number(latestReaded) ;
                ErrorUpdating = false;
            }
        }
        if (ErrorUpdating)
        {
              //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
           // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
    }

    // Now I am in the previous version and I can update the DB.


    if (updateTheModeTableAndSyncLog())
    {
          //qDebug() <<  Q_FUNC_INFO << " -- updateTheModeTableAndSyncLog OK" ;
    }
    else
    {
          //qDebug() <<  Q_FUNC_INFO << " -UPDATED NOK!(9)" ;
        //ErrorUpdating = true;
    }


    if (updateDBVersion(softVersion, "0.015"))
    {
          //qDebug() <<  Q_FUNC_INFO << " -- We are in 015! " ;
    }
    else
    {
          //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 014! " ;
       // emit debugLog(Q_FUNC_INFO, "2", 7);
        return false;
    }
      //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
    return true;
}

bool DataBase::updateTo016()
{
    // Updates the DB to 0.016:
    // Updates the Satellite DB


           //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
        bool IAmIn015 = false;
        bool ErrorUpdating = false;
        latestReaded = getDBVersion();
           //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
        if (latestReaded >= 0.016f)
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am in 015" ;
            return true;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.015 I am in: " << getDBVersion() ;
            while (!IAmIn015 && !ErrorUpdating)
            {
                   //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 015: !" ;
                IAmIn015 = updateTo015();

                if (IAmIn015)
                {
                         //qDebug() <<  Q_FUNC_INFO << " -- updateTo015 returned TRUE - I am in 0.015: " << QString::number(latestReaded) ;
                }
                else
                {
                        //qDebug() <<  Q_FUNC_INFO << " -- updateTo015 returned FALSE - I am NOT in 0.015: " << QString::number(latestReaded) ;
                    ErrorUpdating = false;
                }
            }
            if (ErrorUpdating)
            {
                    //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
               // emit debugLog(Q_FUNC_INFO, "1", 7);
                return false;
            }
        }

        // Now I am in the previous version and I can update the DB.

        if (!recreateSatelliteData())
        {
               //qDebug() <<  Q_FUNC_INFO << " -- Sats update NOK " ;
           // emit debugLog(Q_FUNC_INFO, "2", 7);
            return false;
        }
        if (updateDBVersion(softVersion, "0.016"))
        {
               //qDebug() <<  Q_FUNC_INFO << " -- We are in 016! " ;
        }
        else
        {
               //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 016! " ;
           // emit debugLog(Q_FUNC_INFO, "3", 7);
            return false;
        }
           //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
        return true;
}

bool DataBase::updateTo017()
{
    // Updates the DB to 0.017:
    // Updates the Satellite DB

    //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn016 = false;
    bool ErrorUpdating = false;
    latestReaded = getDBVersion();
    //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.017f)
    {
        //qDebug() <<  Q_FUNC_INFO << " -- I am in 017" ;
        return true;
    }
    else
    {
        //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.017 I am in: " << getDBVersion() ;
        while (!IAmIn016 && !ErrorUpdating)
        {
            //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 016: !" ;
            IAmIn016 = updateTo016();
            if (IAmIn016)
            {
                //qDebug() <<  Q_FUNC_INFO << " -- updateTo016 returned TRUE - I am in 0.016: " << QString::number(latestReaded) ;
            }
            else
            {
                //qDebug() <<  Q_FUNC_INFO << " -- updateTo016 returned FALSE - I am NOT in 0.016: " << QString::number(latestReaded) ;
                ErrorUpdating = false;
            }
        }
        if (ErrorUpdating)
        {
            //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
           // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
    }

    // Now I am in the previous version and I can update the DB.

        // Query to remove the "/" from a Date: update log set qso_date = replace(qso_date, '/', '')
        // Query to remove the ":" from the Time: update log set time_on = replace(time_on, ':', '')
        // Query to join columns:
        //  update log set qso_date = replace((SELECT qso_date from log)||'-'||time_on, '', '')
    /*
    PROCEDURE: Move to SQLITE - datetime type: // 2020-01-01 10:12:01
    // Query to remove the "/" from a Date: update log set qso_date = replace(qso_date, '/', '-')
    // Join: SELECT qso_date || ' ' || time_on from log
    UPDATE logtemp SET qso_date = (SELECT qso_date || ' ' || time_on FROM log)
    */

    /*
    QString stringQuery;
    stringQuery = "update log set qso_date = replace((SELECT qso_date from log)||'-'||time_on, '', '')";
    //UPDATE log set qso_date =  qso_date ||'-'||time_on
    bool sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {return false;}
    */

    // Modify the qso_date with the time_on data with the right format
    QString stringQuery;

    stringQuery = "UPDATE log set qso_date = replace(qso_date ||' '||time_on, '/', '-')";
    bool sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "2", 7);*/return false;}

    stringQuery = "UPDATE log set qso_date_off = replace(qso_date_off, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "3", 7);*/return false;}

    stringQuery = "UPDATE log set qslsdate = replace(qslsdate, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "4", 7);*/return false;}

    stringQuery = "UPDATE log set qslrdate = replace(qslrdate, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "5", 7);*/return false;}

    stringQuery = "UPDATE log set lotw_qslsdate = replace(lotw_qslsdate, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "6", 7);*/return false;}

    stringQuery = "UPDATE log set lotw_qslrdate = replace(lotw_qslrdate, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "7", 7);*/return false;}

    stringQuery = "UPDATE log set eqsl_qslrdate = replace(eqsl_qslrdate, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "8", 7);*/return false;}

    stringQuery = "UPDATE log set eqsl_qslsdate = replace(eqsl_qslsdate, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "9", 7);*/return false;}

    stringQuery = "UPDATE log set hrdlog_qso_upload_date = replace(hrdlog_qso_upload_date, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "10", 7);*/return false;}

    stringQuery = "UPDATE log set hrdlog_qso_upload_date = replace(hrdlog_qso_upload_date, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "11", 7);*/return false;}

    stringQuery = "UPDATE log set clublog_qso_upload_date = replace(clublog_qso_upload_date, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "12", 7);*/return false;}

    stringQuery = "UPDATE log set qrzcom_qso_upload_date = replace(qrzcom_qso_upload_date, '/', '-')";
    sqlOK = execQuery(Q_FUNC_INFO, stringQuery);
    if (!sqlOK)
    {/*emit debugLog(Q_FUNC_INFO, "13", 7);*/return false;}
    if (!recreateTableLog())
    {
        //qDebug() <<  Q_FUNC_INFO << " -- Failed to recreate Table Log " ;
       // emit debugLog(Q_FUNC_INFO, "14", 7);
        return false;
    }
    //qDebug() <<  Q_FUNC_INFO << " -- Recreated Table Log " ;

    /*

    else
    {
        //qDebug() <<  Q_FUNC_INFO << " -- Table log recreated OK" ;
    }
    */

    // REMOVE THE FOLLOWING LINE ONCE THIS FUNCTION HAS BEEN UPDATED
    //return false;

    // If everything went OK, we update the DB number.
    if (updateDBVersion(softVersion, "0.017"))
    {
        //qDebug() <<  Q_FUNC_INFO << " -- We are in 017! " ;
    }
    else
    {
        //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 017! " ;
       // emit debugLog(Q_FUNC_INFO, "15", 7);
        return false;
    }
    //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
    return true;
}

bool DataBase::updateTo018()
{
    // Updates the DB to 0.018:
    // Adds the Subdivisions

    //qDebug() <<  Q_FUNC_INFO << " -latestRead: " << getDBVersion() ;
    bool IAmIn017 = false;
    bool ErrorUpdating = false;
    latestReaded = getDBVersion();
    //qDebug() <<  Q_FUNC_INFO << " -Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.018f)
    {
        //qDebug() <<  Q_FUNC_INFO << " -- I am in 018" ;
        return true;
    }
    else
    {
        //qDebug() <<  Q_FUNC_INFO << " -- I am not in 0.018 I am in: " << getDBVersion() ;
        while (!IAmIn017 && !ErrorUpdating)
        {
            //qDebug() <<  Q_FUNC_INFO << " -- Check if I am in 017: !" ;
            IAmIn017 = updateTo017();
            if (IAmIn017)
            {
                //qDebug() <<  Q_FUNC_INFO << " -- updateTo017 returned TRUE - I am in 0.017: " << QString::number(latestReaded) ;
            }
            else
            {
                //qDebug() <<  Q_FUNC_INFO << " -- updateTo017 returned FALSE - I am NOT in 0.017: " << QString::number(latestReaded) ;
                ErrorUpdating = false;
            }
        }
        if (ErrorUpdating)
        {
            //qDebug() <<  Q_FUNC_INFO << " -- I Could not update to: " << QString::number(dbVersion) ;
           // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
    }

    // Now I am in the previous version and I can update the DB.

    //if (recreateTablePrimarySubdivisions())
    //    return false;
    if (!createTablePrimarySubdivisions(true))
        return false;
    DB_ADIF_Primary_Subdvisions_data primarySubDivisions(Q_FUNC_INFO);
    if (!primarySubDivisions.addData())
        return false;
    // REMOVE THE FOLLOWING LINE ONCE THIS FUNCTION HAS BEEN UPDATED
    //return false;

    // If everything went OK, we update the DB number.
    if (updateDBVersion(softVersion, "0.018"))
    {
        //qDebug() <<  Q_FUNC_INFO << " -- We are in 018! " ;
    }
    else
    {
        //qDebug() <<  Q_FUNC_INFO << " -- Failed to go to 018! " ;
       // emit debugLog(Q_FUNC_INFO, "15", 7);
        return false;
    }
    //qDebug() <<  Q_FUNC_INFO << " -UPDATED OK!" ;
    return true;
}

bool DataBase::updateTo019()
{// Adds FTS4 and FST4W modes
 // Adds RS-44 sat
    //qDebug() << Q_FUNC_INFO << " "  << getDBVersion() ;
    bool IAmIn018 = false;
    bool ErrorUpdating = false;
    latestReaded = getDBVersion();
   //qDebug() << Q_FUNC_INFO << " : Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.019f)
    {
       //qDebug() << Q_FUNC_INFO << " : - I am in 019" ;
        return true;
    }
    else
    {
        while (!IAmIn018 && !ErrorUpdating)
        {
            IAmIn018 = updateTo018();
            if (!IAmIn018)
            {
                ErrorUpdating = false;
            }
        }
        if (ErrorUpdating)
        {
             // emit debugLog(Q_FUNC_INFO, "1", 7);
            return false;
        }
    }

    // Now I am in the previous version and I can update the DB.


    if (updateTheModeTableAndSyncLog() && recreateSatelliteData())
    {
          //qDebug() << Q_FUNC_INFO << " : - updateTheModeTableAndSyncLog OK" ;
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << " : UPDATED NOK!(9)" ;
        return false;
    }


    if (updateDBVersion(softVersion, "0.019"))
    {
          //qDebug() << Q_FUNC_INFO << " : - We are in 019! " ;
    }
    else
    {
       //qDebug() << Q_FUNC_INFO << " : - Failed to go to 018! " ;
       // emit debugLog(Q_FUNC_INFO, "2", 7);
        return false;
    }

   //qDebug() << Q_FUNC_INFO << " : UPDATED OK!" ;
    return true;
}

bool DataBase::updateTo022()
{// Adds Q65 mode
    //qDebug() << Q_FUNC_INFO << " "  << getDBVersion() ;
    bool IamInPreviousVersion = false;
    bool ErrorUpdating = false;
    latestReaded = getDBVersion();
   //qDebug() << Q_FUNC_INFO << " : Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.022f)
    {
       //qDebug() << Q_FUNC_INFO << " : - I am in 022" ;
        return true;
    }
    while (!IamInPreviousVersion && !ErrorUpdating)
    {
        IamInPreviousVersion = updateTo021();
        if (!IamInPreviousVersion)
        {
            return false;
        }
    }

    // Now I am in the previous version and I can update the DB.

    if (!updateTheModeTableAndSyncLog() )
    {
        //qDebug() << Q_FUNC_INFO << " : - updateTheModeTableAndSyncLog OK" ;
        return false;
    }

    if (!updateDBVersion(softVersion, QString::number(0.022)))
    {
        //qDebug() << Q_FUNC_INFO << " : - Failed to go to the previous version! " ;
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " : - We are in the updated version! " ;
    //qDebug() << Q_FUNC_INFO << " : UPDATED OK!" ;
    return true;
}

bool DataBase::updateTo021()
{// Adds 5M & 8M bands
     //qDebug() << Q_FUNC_INFO << " "  << getDBVersion() ;
     bool IamInPreviousVersion = false;
     bool ErrorUpdating = false;
     latestReaded = getDBVersion();
    //qDebug() << Q_FUNC_INFO << " : Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
     if (latestReaded >= 0.021f)
     {
        //qDebug() << Q_FUNC_INFO << " : - I am in 019" ;
         return true;
     }
     while (!IamInPreviousVersion && !ErrorUpdating)
     {
         IamInPreviousVersion = updateTo019();
         if (!IamInPreviousVersion)
         {
             return false;
         }
     }

     // Now I am in the previous version and I can update the DB.

     if (!recreateTableBand ())
     {
         //qDebug() << Q_FUNC_INFO << " : - updateTheModeTableAndSyncLog OK" ;
         return false;
     }

     if (!updateDBVersion(softVersion, QString::number(0.021)))
     {
         //qDebug() << Q_FUNC_INFO << " : - Failed to go to the previous version! " ;
         return false;
     }
     //qDebug() << Q_FUNC_INFO << " : - We are in the updated version! " ;
     //qDebug() << Q_FUNC_INFO << " : UPDATED OK!" ;
     return true;
}

bool DataBase::updateTo023()
{// Recreates the table band
    //qDebug() << Q_FUNC_INFO << " "  << getDBVersion() ;
    bool IamInPreviousVersion = false;
    bool ErrorUpdating = false;
    latestReaded = getDBVersion();
   //qDebug() << Q_FUNC_INFO << " : Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.023f)
    {
        //qDebug() << Q_FUNC_INFO << " : - I am in 023" ;
        return true;
    }
    while (!IamInPreviousVersion && !ErrorUpdating)
    {
        IamInPreviousVersion = updateTo022();
        if (!IamInPreviousVersion)
        {
            return false;
        }
    }

    // Now I am in the previous version and I can update the DB.

    if (!recreateTableBand())
    {
        //qDebug() << Q_FUNC_INFO << " : - updateTheModeTableAndSyncLog OK" ;
        return false;
    }

    if (!updateDBVersion(softVersion, QString::number(0.023)))
    {
        //qDebug() << Q_FUNC_INFO << " : - Failed to go to the previous version! " ;
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " : - We are in the updated version! " ;
    //qDebug() << Q_FUNC_INFO << " : UPDATED OK!" ;
    return true;
}

bool DataBase::updateTo024()
{// Recreates the table band
   //qDebug() << Q_FUNC_INFO << " " << getDBVersion() ;
    bool IamInPreviousVersion = false;
    bool ErrorUpdating = false;
    latestReaded = getDBVersion();
    //qDebug() << Q_FUNC_INFO << " : Checking (latestRead/dbVersion):" << getDBVersion() << "/" << QString::number(dbVersion) ;
    if (latestReaded >= 0.024f)
    {
        //qDebug() << Q_FUNC_INFO << " : - I am in 024" ;
        return true;
    }
    while (!IamInPreviousVersion && !ErrorUpdating)
    {
        IamInPreviousVersion = updateTo023();
        if (!IamInPreviousVersion)
        {
            //qDebug() << Q_FUNC_INFO << " : Update to previous version failed";
            return false;
        }
    }

    // Now I am in the previous version and I can update the DB.

    if (!updateTheEntityTableISONames())
    {
        //qDebug() << Q_FUNC_INFO << " : Update of entityTableIsonames failed";
        return false;
    }


    if (updateDBVersion(softVersion, QString::number(dbVersion)))
    {
        //qDebug() << Q_FUNC_INFO << " : - Failed to go to the previous version! " ;
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " : - We are in the updated version! " ;
    //qDebug() << Q_FUNC_INFO << " : UPDATED OK!" ;
    return true;
}

bool DataBase::updateTo025()
{
    // Updates the DB to 0.025:
    // Adds the mods on ADIF 3.1.3

    //qDebug() << Q_FUNC_INFO << " latestRead: " << getDBVersion() ;

    latestReaded = getDBVersion();
    if (latestReaded >= 0.025f)
    {
          //qDebug() << Q_FUNC_INFO << " - I am in 023" ;
        return true;
    }

    if (!updateTo014())
        return false;


    // Now I am in the previous version and I can update the DB.

    if (!updateTheModeTableAndSyncLog())
        return false;

    if (!recreatePropModes ())
        return false;

    return updateDBVersion(softVersion, "0.025");
}

bool DataBase::updateTo026()
{
    // Updates the DB to 0.026:
    // Recreates entity table to make UTC a real value
    // Adds many Modes & Submodes from ADIF 3.1.4
    //qDebug() << Q_FUNC_INFO << " latestRead: " << getDBVersion() ;
    latestReaded = getDBVersion();
    if (latestReaded >= 0.026f)
    {
        //qDebug() << Q_FUNC_INFO << " - I am in 026" ;
        return true;
    }
    //qDebug() << Q_FUNC_INFO << " - 10" ;
    if (!updateTo025())
        return false;

    //qDebug() << Q_FUNC_INFO << " - 20" ;
    if (!recreateTableEntity())
        return false;

    //qDebug() << Q_FUNC_INFO << " - 30" ;
    if (!updateTheModeTableAndSyncLog())
        return false;
    //qDebug() << Q_FUNC_INFO << " - 40" ;
    if (!updateTableLog(7))
        return false;
    if (!recreateTablePrimarySubdivisions())
        return false;
    //qDebug() << Q_FUNC_INFO << " - 50" ;
    return updateDBVersion(softVersion, "0.026");
}

bool DataBase::updateTo027()
{
    // Updates the DB to 0.027:
    // Recreates entity log to adjust the type for several numeral
    qDebug() << Q_FUNC_INFO << " latestRead: " << getDBVersion() ;
    latestReaded = getDBVersion();
    if (latestReaded >= 0.027f)
    {
        qDebug() << Q_FUNC_INFO << " - I am in 027" ;
        return true;
    }
    qDebug() << Q_FUNC_INFO << " - 10" ;
    if (!updateTo026())
        return false;
    qDebug() << Q_FUNC_INFO << " - 20" ;
    // Start executing the code to update to this version
    if (!recreateTableLog())
    {
        qDebug() << Q_FUNC_INFO << " - 21" ;
        return false;
    }
    qDebug() << Q_FUNC_INFO << " - 30" ;
    if (isTheTableExisting("qso_complete_enumeration"))
    {
        qDebug() << Q_FUNC_INFO << " - 31" ;
        if (!execQuery(Q_FUNC_INFO, "DROP TABLE qso_complete_enumeration"))
            return false;
    }
    // Modify the DB version
    qDebug() << Q_FUNC_INFO << " - 50" ;
    return updateDBVersion(softVersion, "0.027");
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
        query.next();
        qsos = (query.value(0)).toInt();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return qsos;
}

int DataBase::getLastInsertedQSO()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    QString stringQuery = QString("SELECT last_insert_rowid()");

    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    int id = -1;

    if (sqlOK)
    {
        query.next();
        id = (query.value(0)).toInt();
    }
    else
    {
        queryErrorManagement(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return id;
    //qDebug() << Q_FUNC_INFO << " - END";
}

void DataBase::queryErrorManagement(const QString &_functionFailed, const QString &errorCodeS, const QString &_nativeError, const QString &_failedQuery)
{
    Q_UNUSED(_functionFailed);
    Q_UNUSED(errorCodeS);
    Q_UNUSED(_nativeError);
    Q_UNUSED(_failedQuery);
    //qDebug() << Q_FUNC_INFO << ": constrid    : " << QString::number(constrid) ;
    //qDebug() << Q_FUNC_INFO << ": Function    : " << _functionFailed ;
    //qDebug() << Q_FUNC_INFO << ": Native      : " << _nativeError ;
    //qDebug() << Q_FUNC_INFO << ": Error       : " << _functionFailed << errorCodeS ;
    //qDebug() << Q_FUNC_INFO << ": Query failed: " << _failedQuery ;
}

 bool DataBase::execQuery(const QString &function, const QString &stringQuery)
 {
    //qDebug() << Q_FUNC_INFO << ": " << function << " : " << stringQuery ;
    QSqlQuery query;
    query.prepare(stringQuery);
    bool ok = query.exec(stringQuery);
    query.finish();
    if (!ok)
    {
        //qDebug() << Q_FUNC_INFO << ": NOK";
        queryErrorManagement(function, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    //qDebug() << Q_FUNC_INFO << ": OK";
    return ok;
 }

 void DataBase::logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level)
 {
     //qDebug() << _func << ": log: " << _msg;
    if (logLevel<=_level)
        emit debugLog (_func, _msg, _level);
 }

 void DataBase::setLogLevel(const DebugLogLevel _b)
 {
     logEvent (Q_FUNC_INFO, "Start", Debug);
     logLevel = _b;
     logEvent (Q_FUNC_INFO, "END", Debug);
 }
