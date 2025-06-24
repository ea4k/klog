/***************************************************************************
                          world.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "world.h"
#include "callsign.h"
/*
 QHash<QString, int> worldPrefixes;
To insert a (key, value) pair into the hash, you can use operator[]():
 hash["EA"] = 130;
 hash["EA6"] = 131;
 hash["EA8"] = 132;

*/
World::World(DataProxy_SQLite *dp, const QString &_parentFunction)
{
    Q_UNUSED(_parentFunction);
   //qDebug() << Q_FUNC_INFO << " - Start from: " << _parentFunction;
    worldPrefixes.clear();
    cqz = -1;
    ituz = -1;
    ret = false;
    continentId = -1;
    lat = 0.0;
    lon = 0.0;
    utc = 0.0;
    locator = new Locator();
    created = false;    
    dataProxy = dp;
    read = readWorld();
    util = new Utilities(Q_FUNC_INFO);
   //qDebug() << Q_FUNC_INFO << " - END";
}

World::~World()
{
   //qDebug() << Q_FUNC_INFO;
    delete(locator);
    delete(util);
}

bool World::readEntities()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    entities = dataProxy->getAllEntiNameISOAndPrefix();
    if (entities.count()<1)
        return false;

    //QMapIterator<EntityData, int> i(entities);  // Just test, to see if we are doing ok
    //while (i.hasNext()) {
    //    i.next();
    //  //qDebug() << " - " << i.key().name;
    //}
    //qDebug() << Q_FUNC_INFO << " - END" ;
    return true;
}

// Function to get EntityData based on dxcc ID
EntityData World::getEntityDataFromDXCC(const int _dxcc)
{
   //qDebug() << Q_FUNC_INFO << ": " << QString::number(_dxcc);
    for (auto it = entities.constBegin(); it != entities.constEnd(); ++it) {
        if (it.value() == _dxcc) {
            return it.key();
        }
    }
    // If not found, return a default EntityData object
    return EntityData();
}

QString World::getEntityMainPrefix(int _dxcc)
{
   //qDebug() << Q_FUNC_INFO << ": " << _dxcc;

    EntityData entity = getEntityDataFromDXCC(_dxcc);
   //qDebug() << Q_FUNC_INFO << " - 10";
    Callsign prefix(entity.mainprefix);
   //qDebug() << Q_FUNC_INFO << " - 11";
    QString aux = QString();
   //qDebug() << Q_FUNC_INFO << " - 11";
   //qDebug() << Q_FUNC_INFO << ": prefix: " << prefix.getHostFullPrefix() ;
    if (prefix.isValidPrefix())
        aux = entity.mainprefix;
    return aux;
}

bool World::readWorld()
{ // Used to link a prefix with an Entity quickly, without quering the DB.
   //qDebug() << Q_FUNC_INFO << " - Start";
    specialCalls.clear();
    longPrefixes.clear();
    worldPrefixes.clear();

    //worldPrefixes = dataProxy->getWorldData();
    worldPrefixes = dataProxy->getHashTableData(WorldData);
    specialCalls << dataProxy->getSpecialCallsigns();
    longPrefixes << dataProxy->getLongPrefixes();
   //qDebug() << Q_FUNC_INFO << " - worldPrefixes: " << worldPrefixes.count();
   //qDebug() << Q_FUNC_INFO << " - specialCalls : " << specialCalls.count();
   //qDebug() << Q_FUNC_INFO << " - longPrefixes : " << longPrefixes.count();
    if (!readEntities())
        return false;
    if (worldPrefixes.isEmpty())
        return false;
    if (specialCalls.isEmpty())
        return false;
    return !longPrefixes.isEmpty();
}

bool World::recreate(const QString &_worldFile)
{
   //qDebug() << Q_FUNC_INFO << ": " << _worldFile;
    QSqlQuery query;
      // Delete from entity table
      if (!executeQuery(query, "DELETE FROM entity")) {
        emitQueryError(query);
        return false;
      }

      // Delete from prefixesofentity table
      if (!executeQuery(query, "DELETE FROM prefixesofentity")) {
        emitQueryError(query);
        return false;
      }

    // Create the world file
    //qDebug() << Q_FUNC_INFO << " - END-4";
    return create(_worldFile);
}

bool World::create(const QString &_worldFile)
{
    qDebug() << Q_FUNC_INFO << "  " << _worldFile;

    created = readCTYCSV(_worldFile);

    qDebug() << Q_FUNC_INFO << " - 30" ;
    if (created)
    {
        qDebug() << Q_FUNC_INFO << " - 40" ;
        created = insertSpecialEntities();
    }
    if (created)
    {
        qDebug() << Q_FUNC_INFO << " - 50" ;
        if (dataProxy->updateISONames())
        {
            qDebug() << Q_FUNC_INFO << " - 60" ;
            qDebug() << Q_FUNC_INFO << "  updateISONames TRUE" ;
        }
        else
        {
            qDebug() << Q_FUNC_INFO << " - 70" ;
            qDebug() << Q_FUNC_INFO << "  updateISONames FALSE" ;
        }
        qDebug() << Q_FUNC_INFO << " - 80" ;
    }
    if (created)
    { // Let's add the Primary Subdivisions to the DB
        qDebug() << Q_FUNC_INFO << " - 81" ;
        created = dataProxy->addPrimarySubdivisions();
    }
    qDebug() << Q_FUNC_INFO << " - 90" ;
    read = readWorld ();
   //qDebug() << Q_FUNC_INFO << " - END" ;
    return created;
}


QStringList World::readZones (const QString &pref, const int _cq, const int _itu)
{
    //Returns a QStringList: prefix, CQz, ITUz
   //qDebug() << Q_FUNC_INFO << ":  (" << pref << "/" << QString::number(_cq) <<"/" << QString::number(_itu)<< ")";
    QStringList result;
    int cq = _cq;
    int itu = _itu;
    QString azone; // Special zones start with a [
    QString aux = pref;

    if(aux.count('[')==1) // Check if has special CQz
    {
         //qDebug() << Q_FUNC_INFO << ": DETECTED [ !!!!";
        qsizetype pos = aux.indexOf('[');
        azone = aux.sliced(pos, 1);
        //Following line was migrated from qt5
        //azone = (aux.midRef(aux.indexOf('[')+1)).toString();

         //qDebug() << Q_FUNC_INFO << ": (ITU)-1: " << aux << " right of " << QString::number(aux.indexOf('[')) << " = " << azone;
        itu = (azone.left(azone.indexOf(']'))).toInt();

         //qDebug() << Q_FUNC_INFO << ": (ITU)-2: " << azone.left(azone.indexOf(']'));
        aux = aux.left(aux.indexOf('['));
         //qDebug() << Q_FUNC_INFO << ": (ITU): "  << pref << "/" << QString::number(itu) << "/" << aux;
    }

    if(aux.count('(')==1) // Check if has special CQz
    {
         //qDebug() << Q_FUNC_INFO << ": DETECTED ( !!!!";
        qsizetype pos = aux.indexOf('(');
        azone = aux.sliced(pos, 1);
         //Following line was migrated from qt5
        //azone = (aux.midRef(aux.indexOf('(')+1)).toString();
        cq = (azone.left(azone.indexOf(')'))).toInt();
        aux = aux.left(aux.indexOf('('));
         //qDebug() << Q_FUNC_INFO << ": (CQ): "  << pref << "/" << QString::number(cq) << "/" << aux;
    }
     //qDebug() << Q_FUNC_INFO << ": (Pref/CQ/ITU): "  << pref << "= " << aux <<"/" << QString::number(cq) << "/" << QString::number(itu);

    result << aux << QString::number(cq) << QString::number(itu);
     //qDebug() << Q_FUNC_INFO << ": (Pref/CQ/ITU): " << result;
    return result;
}



QString World::getQRZEntityName(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    if (_qrz.length() < 1 )
    {
        return QString();
    }
    int prefixIDNumber = getQRZARRLId(_qrz);
    //qDebug() << Q_FUNC_INFO << " - " << prefixIDNumber;
    return getEntityName(prefixIDNumber);
}

QString World::getEntityName(const int _entityN)
{
   //qDebug() << Q_FUNC_INFO << ": " << _entityN;
    int prefixIDNumber = _entityN;
    if (prefixIDNumber<=0)
    {
        return QString();
    }
    return dataProxy->getEntityNameFromId(prefixIDNumber);
}

int World::getQRZCqz(const QString &_qrz)
{
    //qDebug() << Q_FUNC_INFO << ": " << _qrz;
     if (_qrz.length() < 1 )
     {
         return -1;
     }

    QString aux = util->getPrefixFromCall(_qrz);
    return dataProxy->getCQzFromPrefix(aux);
}

int World::getQRZItuz(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    if (_qrz.length() < 1 )
    {
        return -1;
    }

    QString aux = util->getPrefixFromCall(_qrz);
    return dataProxy->getITUzFromPrefix(aux);
}

int World::getEntityCqz(const int _enti)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _enti;
    if (_enti < 1 )
    {
        return -1;
    }
    return dataProxy->getCQzFromEntity(_enti);
}

int World::getEntityItuz(const int _enti)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _enti;
    if (_enti < 1 )
    {
        return -1;
    }
    return dataProxy->getITUzFromEntity(_enti);
}

int World::getQRZARRLId(const QString &_qrz)
{
  //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    QString  call = _qrz.toUpper();
    Callsign callsign(call);
    if (!callsign.isValidPrefix())
        return -1;

    QString prefix = callsign.getHostFullPrefix();
   //qDebug() << Q_FUNC_INFO << " - prefix: " << prefix;
    int entID = worldPrefixes.value(prefix, -2);
    while ((prefix.length()>1) && (entID<=0))
    {
       //qDebug() << Q_FUNC_INFO << " - " << QString("Pref: %1 / EntID: %2").arg(prefix).arg(entID);
        if (entID<=0)
            prefix.chop(1);
        entID = worldPrefixes.value(prefix, -3);
       //qDebug() << Q_FUNC_INFO << " - " << QString("New Pref: %1 / New EntID: %2").arg(prefix).arg(entID);
    }
   //qDebug() << Q_FUNC_INFO << " - " << QString("Callsign: %1 / Final EntID: %2").arg(_qrz).arg(entID);
    return entID;
}

QString World::getQRZEntityMainPrefix(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    if (_qrz.length() < 1 )
    {
        return "";
    }
    int i = getQRZARRLId(_qrz);

    return getEntityMainPrefix(i);
}

bool World::isNewCQz(const int _cqz)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _cqz;
    return dataProxy->isNewCQz(_cqz);
}

bool World::isNewEntity(const int _entityN)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _entityN;
    if (_entityN <= 0)
    {
        return false;
    }
    return dataProxy->isNewEntity(_entityN);
}

QString World::getQRZContinentShortName(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    return getContinentShortName (getQRZARRLId(_qrz));
}

QString World::getContinentShortName(const int _enti)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _enti;
    if ( _enti < 0 )
    {
        return "--";
    }
    QString a = dataProxy->getContinentShortNameFromEntity(_enti);
    if (a.length()!=2)
    {
        return "--";
    }
    else
    {
        return a;
    }
}

QString World::getQRZContinentNumber(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    int i = getQRZARRLId(_qrz);
    return QString::number(getContinentNumber(i));
}

int World::getContinentNumber(const int _enti)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _enti;
    if (_enti <= 0)
    {
        return -1;
    }
    return dataProxy->getContinentIdFromEntity(_enti);
}

double World::getLongitude(const int _enti)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _enti;
    if (_enti <= 0)
    {
        return 0.0;
    }
    return dataProxy->getLongitudeFromEntity(_enti);
}

double World::getLatitude(const int _enti)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _enti;
    if (_enti <= 0)
    {
        return 0.0;
    }
    return dataProxy->getLatitudeFromEntity(_enti);
}

QString World::getQRZLocator(const QString &_qrz)
{
   //qDebug() << Q_FUNC_INFO << " - Start: " << _qrz;
    Callsign callsign(_qrz);
    if (!callsign.isValidPrefix())
        return QString();

    int entity = getQRZARRLId (_qrz);
   //qDebug() << Q_FUNC_INFO << " - Entity: " << entity;
    return locator->getLocator(getLongitude(entity), getLatitude (entity));
}

QString World::getLocator(const int _entityN)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _entityN;
    if (_entityN <= 0)
    {
        return "";
    }
    return locator->getLocator(getLongitude(_entityN), getLatitude(_entityN));
}

bool World::addEntity(const QString &_name, const int _cq, const int _itu, const int _contId, const double _lat, const double _lon, const double _utc, const int _dxcc, const QString &_mainpref)
{
   //qDebug() << Q_FUNC_INFO << " - Start";

    QSqlQuery query;

    query.prepare(QStringLiteral("INSERT INTO entity (name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix) "
                  "VALUES (:name, :cqz, :ituz, :continent, :latitude, :longitude, :utc, :dxcc, :mainprefix)"));

    query.bindValue(QStringLiteral(":name"), _name); // name
    query.bindValue(QStringLiteral(":cqz"), _cq); // CQ
    query.bindValue(QStringLiteral(":ituz"), _itu); // ITU
    query.bindValue(QStringLiteral(":continent"), _contId); // Cont
    query.bindValue(QStringLiteral(":latitude"), _lat); // Lat
    query.bindValue(QStringLiteral(":longitude"), _lon); // Lon
    query.bindValue(QStringLiteral(":utc"), _utc); // UTC
    query.bindValue(QStringLiteral(":dxcc"), _dxcc); // dxcc
    query.bindValue(QStringLiteral(":mainprefix"), _mainpref); // Mainprefix

     //qDebug()  << Q_FUNC_INFO << " Entity name: " << _name;
     //qDebug()  << Q_FUNC_INFO << " Entity cqz:  " << QString::number(_cq);
     //qDebug()  << Q_FUNC_INFO << " Entity ituz: " << QString::number(_itu);
     //qDebug()  << Q_FUNC_INFO << " Entity cont: " << QString::number(_contId);
     //qDebug()  << Q_FUNC_INFO << " Entity lat:  " << QString::number(_lat);
     //qDebug()  << Q_FUNC_INFO << " Entity lon:  " << QString::number(_lon);
     //qDebug()  << Q_FUNC_INFO << " Entity UTC:  " << QString::number(_utc);
     //qDebug()  << Q_FUNC_INFO << " Entity ARRL: " << QString::number(_dxcc);
     //qDebug()  << Q_FUNC_INFO << " Entity Pref: " << _mainpref;

    if (query.exec())
    {
         //qDebug() << Q_FUNC_INFO << "  Entity data added OK: " <<  _name ;
        return true;
    }
    //else if (query.lastError().text() == QString::number(19))
    //{
         //qDebug() << Q_FUNC_INFO << "  Entity data NOT added: error19:  " <<  _name ;
    //}
    //else
    //{
         //qDebug() << Q_FUNC_INFO << "  Entity data NOT added: error else:  " <<  _name ;
        //emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
         //qDebug() << Q_FUNC_INFO << " Entity data NOT added" ;
         //qDebug() << Q_FUNC_INFO << " LastQuery: " << query.lastQuery() ;
         //qDebug() << Q_FUNC_INFO << " LastError-data: " << query.lastError().databaseText() ;
         //qDebug() << Q_FUNC_INFO << " LastError-driver: " << query.lastError().driverText() ;
         //qDebug() << Q_FUNC_INFO << " LastError-n: " << query.lastError().text();
    //}
    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    return false;
}

bool World::addPrefix(const QString &_pref, const int _dxcc, const int _cqz, const int _ituz)
{
   //qDebug() << Q_FUNC_INFO << ": _pref: " << _pref;
     //qDebug() << Q_FUNC_INFO << ": _dxcc: " << QString::number(_dxcc);
     //qDebug() << Q_FUNC_INFO << ": _cqz: " << QString::number(_cqz);
     //qDebug() << Q_FUNC_INFO << ": _ituz: " << QString::number(_ituz);
    QSqlQuery query;
    query.prepare(QStringLiteral("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES (:pref, :dxcc, :cqz, :ituz)"));

    query.bindValue(QStringLiteral(":pref"), _pref); // Cont
    query.bindValue(QStringLiteral(":dxcc"), _dxcc); // Cont
    query.bindValue(QStringLiteral(":cqz"), _cqz); // CQ
    query.bindValue(QStringLiteral(":ituz"), _ituz); // ITU
    if (query.exec())
    {
         //qDebug() << Q_FUNC_INFO << "  Prefix data added OK: " <<  _pref ;
         //qDebug() << Q_FUNC_INFO << "  query: " <<  query.executedQuery();
         //qDebug() << Q_FUNC_INFO << "  query: " <<  query.lastQuery();
        return true;
    }
    else if ((query.lastError().nativeErrorCode()).toInt() == 2067)
    //else if (query.lastError().text() == QString::number(19))
    {
        //#pragma "ERROR code 19 should be 2067 to indicate duplicate"
         //qDebug() << Q_FUNC_INFO << "  :Prefix data NOT added: error19:  " <<  _pref ;
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << "  Pref data NOT added: error else:  " <<  _pref ;
         //qDebug() << Q_FUNC_INFO << " LastQuery: " << query.lastQuery() ;
         //qDebug() << Q_FUNC_INFO << " LastError-data: " << query.lastError().databaseText() ;
         //qDebug() << Q_FUNC_INFO << " LastError-driver: " << query.lastError().driverText() ;
         //qDebug() << Q_FUNC_INFO << " LastError-n: " << query.lastError().text();
    }
    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    return false;
}

bool World::readCTYCSV(const QString &_worldFile)
{
   //qDebug() << Q_FUNC_INFO << ": " <<  _worldFile;
    QFile file(_worldFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      //qDebug() << Q_FUNC_INFO << ": File not found: END FALSE" << _worldFile;
        return false;
    }
    QStringList existingPrefixes;
    QList<int> existingDXCCs;

  //qDebug() << Q_FUNC_INFO << "File found:" << _worldFile;

    QTextStream in(&file);
    int numberOfLines = 0;

    // First pass: count lines
    while (!in.atEnd()) {
        in.readLine();
        numberOfLines++;
    }

  //qDebug() << Q_FUNC_INFO << "Number of lines:" << numberOfLines;

    // Reset the file position
    file.seek(0);
    in.seek(0);

// Initialize progress dialog
#ifndef KLOG_TESTING
    QProgressDialog progress(tr("Reading cty.csv..."), tr("Abort reading"), 0, numberOfLines);
    progress.setWindowModality(Qt::ApplicationModal);
#endif

    int progressBarPosition = 0;
    QStringList stringList, stringListPrefixes;

    while (!in.atEnd()) {
#ifndef KLOG_TESTING
        progress.setValue(progressBarPosition);
        if (progress.wasCanceled())
            break;
#endif

        progressBarPosition++;
        stringList.clear();
        stringListPrefixes.clear();

        QString line = in.readLine().simplified().trimmed();
        line.remove(QChar(';'), Qt::CaseInsensitive);
        stringList = line.split(',');

        if (stringList.size() != 10) {
            continue;
        }
        // Si el indicativo ya existe, no se debe nuscar más
        //    Aqui se añaden de forma indefinida... no se valida el indicativo, solo el numero y entonces se va añadiendo varias veces el mismo indicativo
        //    habría que, además del numero, verificar el prefijo primero


        QString mPrefix = stringList.at(0);
        if (existingPrefixes.contains(mPrefix))
        {
            qDebug() << Q_FUNC_INFO << ": Existing prefix: " << mPrefix;
            continue;
        }


        int _entityNumber = stringList.at(2).toInt();

        if (mPrefix.contains(QChar('*'), Qt::CaseInsensitive))
        {
            _entityNumber += 1000;
            // Loop until we find an unused entity number with an empty main prefix
            while (existingDXCCs.contains(_entityNumber))
            {
                _entityNumber += 1000;
            }
            existingDXCCs.append(_entityNumber);
        }

        QString entName = stringList.at(1);
        int contId = dataProxy->getContinentIdFromContinentShortName(stringList.at(3));
        int cqz = stringList.at(4).toInt();
        int ituz = stringList.at(5).toInt();
        double lat = stringList.at(6).toDouble();
        double lon = stringList.at(7).toDouble();
        double utc = stringList.at(8).toDouble();

        qDebug() << Q_FUNC_INFO << ": NEW prefix: " << mPrefix;
        existingPrefixes.append(mPrefix);
        if (addEntity(entName, cqz, ituz, contId, lat, lon, utc, _entityNumber, mPrefix)) {
          //qDebug() << Q_FUNC_INFO << "Entity added:" << entName;
            // stringList.at(9) contains an space separated list of prefixes for that entity
            addPrefixes(stringList.at(9), _entityNumber, cqz, ituz);             //TODO: Handle the error
        } else {
          //qDebug() << Q_FUNC_INFO << "Entity not added:" << entName;
        }

#ifndef KLOG_TESTING
        progress.setLabelText("Reading cty.csv ... \nNow reading " + mPrefix + " data");
#endif
      //qDebug() << Q_FUNC_INFO << "Progress bar position:" << progressBarPosition;
    }

#ifndef KLOG_TESTING
    progress.setValue(numberOfLines);
#endif

    if (created) {
        dataProxy->updateISONames();
    }

    return true;
}


bool World::addPrefixes(const QString &prefixes, int entityNumber, int cqz, int ituz)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    QStringList stringListPrefixes = prefixes.split(' ');

    QList<QPair<QString, QPair<int, QPair<int, int>>>> pairPrefixes;
    //pairPrefixes.append(qMakePair(QString("EA"), qMakePair(130, qMakePair(10, 20))));


    for (const QString &prefix : stringListPrefixes) {
        QStringList stringListProcessedPrefix = readZones(prefix, cqz, ituz);
        if (stringListProcessedPrefix.size() == 3)
        {
            //TODO: Add some checks to ensure we are adding a prefix and 3 numbers

            //Returns a QStringList: prefix, CQz, ITUz
            pairPrefixes.append(qMakePair(stringListProcessedPrefix.at(0), qMakePair(entityNumber, qMakePair(stringListProcessedPrefix.at(1).toInt(), stringListProcessedPrefix.at(2).toInt()))));
            //if (addPrefix(stringListProcessedPrefix.at(0), entityNumber, stringListProcessedPrefix.at(1).toInt(), stringListProcessedPrefix.at(2).toInt())) {
            //  //qDebug() << Q_FUNC_INFO << "Prefix added:" << stringListProcessedPrefix.at(0);
            //} else {
            //  //qDebug() << Q_FUNC_INFO << "Prefix not added:" << stringListProcessedPrefix.at(0);
            //}
        }
    }
    return insertPrefixes(pairPrefixes);
}

bool World::insertPrefixes(const QList<QPair<QString, QPair<int, QPair<int, int>>>> &pairPrefixes)
{
   //qDebug() << Q_FUNC_INFO << " - Start ";
    QSqlQuery query;
    query.prepare(
        "INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) "
        "VALUES (:pref, :dxcc, :cqz, :ituz)");

    QSqlDatabase::database().transaction();  // Start the transaction

    foreach (const auto &prefix, pairPrefixes)
    {
        query.bindValue(":pref", prefix.first);
        query.bindValue(":dxcc", prefix.second.first);
        query.bindValue(":cqz", prefix.second.second.first);
        query.bindValue(":ituz", prefix.second.second.second);

        if (!query.exec())
        {
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
            QSqlDatabase::database().rollback();  // Rollback transaction on error
            return false;
        }
    }

    QSqlDatabase::database().commit();  // Commit the transaction
    return true;
}


int World::extractEntityNumber(const QStringList &stringList)
{
    if (stringList.size() < 3) {
        qWarning() << Q_FUNC_INFO << "Insufficient elements in stringList!";
        return -1; // or another error code
    }

    int _entityNumber = stringList.at(2).toInt();
    QString _dirtyPrefix = stringList.at(0);
    QString _prefix;
    if (_prefix.contains(QChar('*'), Qt::CaseInsensitive))
    {

    }

    if (_prefix.contains(QChar('*'), Qt::CaseInsensitive)) {
        _entityNumber += 1000;
        // Loop until we find an unused entity number with an empty main prefix
        while ((!getEntityMainPrefix(_entityNumber).isEmpty()) || existingDXCCId(_entityNumber)) {
            _entityNumber += 1000;
        }
    }

   //qDebug() << Q_FUNC_INFO << "Final extracted entity number:" << _entityNumber;
    return _entityNumber;
}

bool World::existingDXCCId(const int _id) const
{
   //qDebug() << Q_FUNC_INFO << " - Start ";
    QSqlQuery query;
    query.prepare("SELECT 1 FROM entity WHERE dxcc = :id LIMIT 1");
    query.bindValue(":id", _id);
    if (!query.exec())
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        return false;
    }
    return query.next(); // Returns true if a row exists, false otherwise
}

int World::getHowManyEntities()
{
   //qDebug() << Q_FUNC_INFO << " - Start ";
    return dataProxy->getHowManyEntities();
}

bool World::insertSpecialEntities()
{ //https://en.wikipedia.org/wiki/Non-ITU_prefix
   //qDebug() << Q_FUNC_INFO << " - Start ";
    // T9 to E7
    // 4N & YZ to 4O
    int entityID = dataProxy->getEntityIdFromMainPrefix("E7");
    int cqz = dataProxy->getCQZFromId(entityID);
    int ituz = dataProxy->getITUzFromEntity(entityID);

    QSqlQuery query;
    query.prepare("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES (?, ?, ?, ?)");

    // Insert "T9"
    query.addBindValue("T9");
    query.addBindValue(entityID);
    query.addBindValue(cqz);
    query.addBindValue(ituz);
    if (!query.exec())
    {
        query.finish();
        return false;
    }

    entityID = dataProxy->getEntityIdFromMainPrefix("4O");
    cqz = dataProxy->getCQzFromEntity(entityID);
    ituz = dataProxy->getITUzFromEntity(entityID);

    // Insert "4N"
    query.addBindValue("4N");
    query.addBindValue(entityID);
    query.addBindValue(cqz);
    query.addBindValue(ituz);
    if (!query.exec())
    {
        query.finish();
        return false;
    }

    // Insert "YZ"
    query.addBindValue("YZ");
    query.addBindValue(entityID);
    query.addBindValue(cqz);
    query.addBindValue(ituz);
    if (!query.exec())
    {
        query.finish();
        return false;
    }

    return true;
     //qDebug() << Q_FUNC_INFO << " - END";
}

bool World::hasSpecialEntities()
{ // Checks if T9 is added to the list of prefixes to validate if special prefixes have been added.
   //qDebug() << Q_FUNC_INFO << " - Start ";
    QString queryString = QString("SELECT dxcc from prefixesofentity WHERE prefix='T9'");
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        query.next();
        if (query.isValid())
        {
            if ((query.value(0)).toInt()>1)
            {
                query.finish();
                return true;
            }
        }
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return false;
}

bool World::isAKnownCall(const QString &_callsign)
{
   //qDebug() << Q_FUNC_INFO << ": " << _callsign;
    return specialCalls.contains(_callsign);
}

bool World::isAKnownPrefix(const QString &_prefix)
{
   //qDebug() << Q_FUNC_INFO << ": " << _prefix;
    return longPrefixes.contains(_prefix);
}

bool World::executeQuery(QSqlQuery &query, const QString &queryString) const
{
    if (!query.exec(queryString)) {
        return false;
    }
    return true;
}

void World::emitQueryError(const QSqlQuery &query) const
{
    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
}


int World::getHowManyEmptyDXCCorCont()
{   // Refactored from DataProxy_SQLite::fillEmptyDXCCInTheLog()
    QSqlQuery query;
    QString queryString = QString("SELECT COUNT (id) FROM log WHERE dxcc IS NULL OR dxcc<'1' OR cont IS NULL");
    bool sqlOK = query.exec(queryString);
    int qsos = -1;
    if (sqlOK)
    {
        query.next();
        qsos = (query.value(0)).toInt();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
    }
    query.finish();
    return qsos;
}

bool World::fillEmptyDXCCInTheLog()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    int qsos = getHowManyEmptyDXCCorCont();
    if (qsos < 1)
    {
        return true;
    }
    int step = util->getProgresStepForDialog(qsos);

    QProgressDialog progress(QObject::tr("Updating information..."), QObject::tr("Abort updating"), 0, qsos);
    progress.setMaximum(qsos);
    progress.setWindowModality(Qt::WindowModal);

    QString queryString = QString("SELECT id, call FROM log WHERE dxcc IS NULL OR dxcc<1 OR cont IS NULL");
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (sqlOK)
    {
        int nameCol = -1;

        QSqlRecord rec = query.record();
        QString _call = QString();
        int _id = -1;
        int _dxcc = -1;
        QString _aux = QString();
        QString _continent = QString();
        int j = 0;

        while (query.next())
        {
            if (query.isValid())
            {
                nameCol = rec.indexOf("id");
                _id = (query.value(nameCol)).toInt();
                nameCol = rec.indexOf("call");
                _call = (query.value(nameCol)).toString();
                _dxcc = getQRZARRLId(_call);
                //_dxcc = QString::number(getPrefixId(_call));
                _continent = dataProxy->getContinentShortNameFromEntity(_dxcc);
                    //qDebug() << Q_FUNC_INFO << ":   DXCC: " << _dxcc;
                    //qDebug() << Q_FUNC_INFO << ":   Cont: " << _continent;
                // UPDATE THE ID WITH THE DXCC
                sqlOK = updateDXCCAndContinent(_id, _dxcc, _continent);
                if (!sqlOK)
                {
                    query.finish();
                    return false;
                }
                if (( (j % step )== 0) )
                { // To update the speed I will only show the progress once each X QSOs
                    _aux = QObject::tr("Updating DXCC and Continent information...") + "\n" + QObject::tr("QSO: ")  + QString::number(j) + "/" + QString::number(qsos);
                    //_aux = "Updating ...";
                    progress.setLabelText(_aux);
                    progress.setValue(j);
                }
                if ( progress.wasCanceled() )
                {
                    //qDebug() << Q_FUNC_INFO << ":   progress canceled";
                    query.finish();
                    return true;
                }
                j++;
            }
        }
        query.finish();
        progress.setValue(qsos);
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("KLog DXCC"));
        msgBox.setText(tr("All QSOs have been updated with a DXCC and the Continent.") );
        msgBox.exec();
    }
    else
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    query.finish();
    return true;
}

bool World::updateDXCCAndContinent(const int _id, const int _dxcc, const QString &_cont)
{   // Refactored from DataProxy_SQLite::fillEmptyDXCCInTheLog()
    QString queryString = QString("UPDATE log SET dxcc = :dxcc', cont = :cont WHERE id = :id");
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":dxcc", _dxcc);
    query.bindValue(":cont", _cont);
    query.bindValue(":id", _id);

    if (query.exec(queryString))
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().text(), query.lastQuery());
        query.finish();
        return false;
    }
    query.finish();
    return true;
}
