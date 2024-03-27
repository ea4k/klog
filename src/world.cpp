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
    //qDebug() << Q_FUNC_INFO << " - Start";
    //worldModel = new QSqlRelationalTableModel(this);
    //qDebug() << Q_FUNC_INFO << " - Start";
    numberOfEntities = 0;
    progressBarPosition = 0;
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
    util = new Utilities(Q_FUNC_INFO);


    if (readWorld())
    {
        //qDebug() << Q_FUNC_INFO << " - World TRUE";
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - World FALSE";
    }

    //qDebug() << Q_FUNC_INFO << " - END";
}

World::~World()
{
      //qDebug() << "World::~World";
    delete(locator);
    delete(util);
}

bool World::readWorld()
{ // Used to link a prefix with an Entity quickly, without quering the DB.
    //qDebug() << Q_FUNC_INFO << " - Start";
    worldPrefixes.clear();
    worldPrefixes = dataProxy->getWorldData();
    if (worldPrefixes.size()>100)
    {
        //qDebug() << Q_FUNC_INFO << " - END true";
        util->setLongPrefixes(dataProxy->getLongPrefixes());
        util->setSpecialCalls(dataProxy->getSpecialCallsigns());
        return true;
    }
    //qDebug() << Q_FUNC_INFO << " - END false";
    return false;
}

bool World::recreate(const QString &_worldFile)
{
    //qDebug() << Q_FUNC_INFO << ": " << _worldFile;
    QSqlQuery query;
    if (query.exec("DELETE FROM entity"))
    {
        //qDebug() << Q_FUNC_INFO << ":  EMPTY entity" ;
        if (query.exec("DELETE FROM prefixesofentity"))
        {
            //qDebug() << Q_FUNC_INFO << ": EMPTY prefixesofentity - END-1" ;
            return create(_worldFile);

            //if (create(_worldFile))
            // {
            //     return insertSpecialEntities ();
            // }
        }
        else
        {//TODO: Manage the query error
            //qDebug() << Q_FUNC_INFO << ": FAILED TO EMPTY prefixesofentity - END-2" ;
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            return false;
        }
    }
    else
    {//TODO: Manage the query error
        //qDebug() << Q_FUNC_INFO << ": FAILED TO EMPTY entity - END-3" ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - END-4";
    return false;
}

bool World::create(const QString &_worldFile)
{
    //qDebug() << Q_FUNC_INFO << "  " << _worldFile;

    if (readCTYCSV(_worldFile))
    {
        //qDebug() << Q_FUNC_INFO << " - 10" ;
        util->setLongPrefixes(dataProxy->getLongPrefixes());
        util->setSpecialCalls(dataProxy->getSpecialCallsigns());
        created = true;
        //qDebug() << Q_FUNC_INFO << " - TRUE" ;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - 20" ;
        created = false;
        //qDebug() << Q_FUNC_INFO << " - FALSE" ;
    }
    //qDebug() << Q_FUNC_INFO << " - 30" ;
    if (created)
    {
        //qDebug() << Q_FUNC_INFO << " - 40" ;
        created = insertSpecialEntities();
    }
    if (created)
    {
        //qDebug() << Q_FUNC_INFO << " - 50" ;
        if (dataProxy->updateISONames())
        {
            //qDebug() << Q_FUNC_INFO << " - 60" ;
               //qDebug() << Q_FUNC_INFO << "  updateISONames TRUE" ;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << " - 70" ;
            //qDebug() << Q_FUNC_INFO << "  updateISONames FALSE" ;
        }
        //qDebug() << Q_FUNC_INFO << " - 80" ;
    }
    //qDebug() << Q_FUNC_INFO << " - 90" ;
    readWorld ();
    //qDebug() << Q_FUNC_INFO << " - END" ;
    return created;
}

/*
void World::createWorldModel()
{
    worldModel->setTable("entity");
    worldModel->setRelation(Entity_Continent, QSqlRelation("continent", "id", "shortname"));
    worldModel->setSort(Entity_Name, Qt::AscendingOrder);
    worldModel->setHeaderData(Entity_Name, Qt::Horizontal, tr("Entity"));
    worldModel->setHeaderData(Entity_Continent, Qt::Horizontal, tr("Continent"));
    worldModel->select();
}
*/

QStringList World::processLine(const QString &_line)
{
    //qDebug() << "World::processLine: received: " << _line;
    //QString queryString;
    QStringList aa;
    QString line;
    line = (_line).simplified();
    if ((line).count('\'')) // Replaces ' by _
    //The error comes from "Cote d'Ivoire" that breaks the SQL
    {
        line.replace(QChar('\''), QChar('_'));
    }
       //qDebug() << "World::processLine: Received: " << line;
    //QSqlQuery query1;

    nullValue=-1;

    if ( (line.count(':') == 8 ) ) // First line of an Entity
    { //United States:            05:  08:  NA:   43.00:    87.90:     5.0:  K:
           //qDebug() << "World::processLine first: " << line;
        numberOfEntities++;
        list.clear();
        list << line.split(':');
        entityName = list[0];
        cqz = list[1].toInt();
        ituz = list[2].toInt();
        continentName = (list[3]).simplified();

        continentId=-1;
        lat = list[4].toDouble();
        lon = list[5].toDouble();
        utc  = ((list[6]).simplified()).toDouble();
        //utc  = ((list[6]).simplified()).toInt();
        prefix = list[7];
        currentPrefix = prefix;

        continentId = dataProxy->getContinentIdFromContinentShortName(continentName);

        aa << entityName << QString::number(cqz) << QString::number(ituz) <<QString::number(continentId) << QString::number(lat) << QString::number(lon) << QString::number(utc) << QString::number(numberOfEntities) << prefix;
        progressBarPosition++;
        return aa;
    }
    aa.clear();
    return aa;
}

QStringList World::processLineP(const QString &_line, const int _processingEntity){
    //Returns QStringList: prefix << dxcc << cqz << ituz OR CurrentEntity as a number

       //qDebug() << "World::processLineP: Entity/received: " << QString::number(_processingEntity) << "/" << _line;
    //QString queryString;
    QString line;
    int currentEntity = _processingEntity;
    int _cqz, _ituz;
    //_cqz = 0;
    //_ituz = 0;
    line = (_line).simplified();

    if ((line).count('\'')) // Replaces ' by _
    //The error comes from "Cote d'Ivoire" that breaks the SQL
    {
        line.replace(QChar('\''), QChar('_'));
    }

    //qDebug() << "World::processLineP: Received: " << line;
    //QSqlQuery _queryp;
    QStringList aa, _list;
    aa.clear();

    if ( (line.count(':') == 8 ) ) // First line of an Entity
    { //United States:            05:  08:  NA:   43.00:    87.90:     5.0:  K:
         currentEntity++;
         aa << QString::number(currentEntity);
         return aa;
    }
    else if ( line.endsWith(';') ) // Last line of the Entity
    { //    =WX4TM(4),=WX5S(3)[6],=WY5I(5)[8],=WY7I(4)[7],=WY7LL(4)[7],=WZ4F(4);
           //qDebug() << "World::processLineP last line (; detected): " << line;
        _cqz = dataProxy->getCQzFromEntity(currentEntity);
        _ituz = dataProxy->getITUzFromEntity(currentEntity);

        line = line.remove(';');

        if (line.count(',') == 0) // Only one prefix in the final line
        {
            prefixAndZones = readZones(line, _cqz, _ituz);
            //aa.clear();
            aa << prefixAndZones.at(0) << QString::number(currentEntity) << prefixAndZones.at(1) << prefixAndZones.at(2);

             //readZones returns a QStringList: prefix, CQz, ITUz
             //Returns QStringList: prefix << dxcc << cqz << ituz OR CurrentEntity as a number
            return aa;
        }
        else // More than just one prefix in the final line
        {
               //qDebug() << "World::processLineP Query (MORE one final)(line):" << line;
            _list.clear();
            _list << line.split(',');
            aa.clear();
            //queryString.clear();

            for (int i = 0; i < _list.size(); ++i)
            {
            // PROCESS THE LINE
                   //qDebug() << "World::processLineP LastLine prefixes" << _list.at(i);
                //readZones returns a QStringList: prefix, CQz, ITUz
                prefixAndZones = readZones(_list.at(i), _cqz, _ituz);
                //aa.clear();
                aa << prefixAndZones.at(0) << QString::number(currentEntity) << prefixAndZones.at(1) << prefixAndZones.at(2);
                //return aa;
            }
            //qDebug() << "World::processLineP: END" <<endl;
            return aa;
        }
    }
    else // Lines of the middle...
    {  //    =W4KW(4),=W4LC(4),=W4LSC(3)[6],=W4LWW(4),=W4NBS(4),=W4NI(4),=W4NTI(4),
           //qDebug() << "World::processLine middle (no ; detected): " << line;

        _cqz = dataProxy->getCQzFromEntity(currentEntity);
        _ituz = dataProxy->getITUzFromEntity(currentEntity);

        if (line.endsWith(','))
        {
            line.chop(1);
        }

        if ((line.split(',')).size() == 1) // Only one prefix in the middle line
        { // Not usual, added this check for sanity reasons only
               //qDebug() << "World::processLine Query: (only one middle) ";
            line = line.remove(',');
            prefixAndZones = readZones(line, _cqz, _ituz);
            aa.clear();
            aa << prefixAndZones.at(0) << QString::number(currentEntity) << prefixAndZones.at(1) << prefixAndZones.at(2);

            return aa;
        }
        else
        {
               //qDebug() << "World::processLine Query: (MORE than one middle) ";
            list.clear();
            list << line.split(',');

            //queryString.clear();
            aa.clear();
            for (int i = 0; i < list.size(); ++i)
            {
            // PROCESS THE LINE
                prefixAndZones = readZones(list[i], _cqz, _ituz);

                aa << prefixAndZones.at(0) << QString::number(currentEntity) << prefixAndZones.at(1) << prefixAndZones.at(2);

                //ret = query.exec(queryString);
            }
        }
    }
    //aa.clear();
    return aa;
}

QStringList World::readZones (const QString &pref, const int _cq, const int _itu)
{
    //Returns a QStringList: prefix, CQz, ITUz
    //qDebug() << "World::readZones: (" << pref << "/" << QString::number(_cq) <<"/" << QString::number(_itu)<< ")";
    QStringList result;
    int cq = _cq;
    int itu = _itu;
    QString azone; // Special zones start with a [
    QString aux = pref;

    if(aux.count('[')==1) // Check if has special CQz
    {
        //qDebug() << "World::readZones DETECTED [ !!!!";
        qsizetype pos = aux.indexOf('[');
        azone = aux.sliced(pos, 1);
        //Following line was migrated from qt5
        //azone = (aux.midRef(aux.indexOf('[')+1)).toString();

        //qDebug() << "World::readZones (ITU)-1: " << aux << " right of " << QString::number(aux.indexOf('[')) << " = " << azone;
        itu = (azone.left(azone.indexOf(']'))).toInt();

        //qDebug() << "World::readZones (ITU)-2: " << azone.left(azone.indexOf(']'));
        aux = aux.left(aux.indexOf('['));
        //qDebug() << "World::readZones (ITU): "  << pref << "/" << QString::number(itu) << "/" << aux;
    }

    if(aux.count('(')==1) // Check if has special CQz
    {
        //qDebug() << "World::readZones DETECTED ( !!!!";
        qsizetype pos = aux.indexOf('(');
        azone = aux.sliced(pos, 1);
         //Following line was migrated from qt5
        //azone = (aux.midRef(aux.indexOf('(')+1)).toString();
        cq = (azone.left(azone.indexOf(')'))).toInt();
        aux = aux.left(aux.indexOf('('));
        //qDebug() << "World::readZones (CQ): "  << pref << "/" << QString::number(cq) << "/" << aux;
    }
    //qDebug() << "World::readZones (Pref/CQ/ITU): "  << pref << "= " << aux <<"/" << QString::number(cq) << "/" << QString::number(itu);

    result << aux << QString::number(cq) << QString::number(itu);
    //qDebug() << "World::readZones (Pref/CQ/ITU): " << result;
    return result;
}

int World::getPrefixId(const QString &_prefix)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << _prefix << "/" << QString::number(worldPrefixes.value(_prefix, -2));
    //This function receives the final prefix.

    if (_prefix.length() < 1)
    {
        return -1;
    }
    return worldPrefixes.value(_prefix, -2);
}

QString World::getQRZEntityName(const QString &_qrz)
{
    //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    if (_qrz.length() < 1 )
    {
        return QString();
    }
    int prefixIDNumber = getQRZARRLId(_qrz);
    return getEntityName(prefixIDNumber);
}

QString World::getEntityName(const int _entityN)
{
    int prefixIDNumber = _entityN;
    if (prefixIDNumber<=0)
    {
        return QString();
    }
    return dataProxy->getEntityNameFromId(prefixIDNumber);
}

int World::getQRZCqz(const QString &_qrz)
{
    //qDebug() << "World::getQRZCqz: " << _qrz;
     if (_qrz.length() < 1 )
     {
         return -1;
     }

    QString aux = util->getPrefixFromCall(_qrz);
    return dataProxy->getCQzFromPrefix(aux);
}

int World::getQRZItuz(const QString &_qrz)
{
       //qDebug() << "World::getQRZItuz: " << _qrz;
    if (_qrz.length() < 1 )
    {
        return -1;
    }

    QString aux = util->getPrefixFromCall(_qrz);
    return dataProxy->getITUzFromPrefix(aux);
}

int World::getEntityCqz(const int _enti)
{
    if (_enti < 1 )
    {
        return -1;
    }
    return dataProxy->getCQzFromEntity(_enti);
}

int World::getEntityItuz(const int _enti)
{
    if (_enti < 1 )
    {
        return -1;
    }
    return dataProxy->getITUzFromEntity(_enti);
}

int World::getQRZARRLId(const QString &_qrz)
{
    //qDebug() << Q_FUNC_INFO << ": " << _qrz;
    if (_qrz.length() < 1 )
    {
        return -1;
    }
    QString pref = util->getPrefixFromCall(_qrz);
    //qDebug() << Q_FUNC_INFO << ": prefix: " << pref;
    return getPrefixId(pref);
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

QString World::getEntityMainPrefix(const int _entityN)
{
    if (_entityN <= 0 )
    {
        return QString();
        //return tr("NONE");
    }
    return dataProxy->getEntityMainPrefix(_entityN);
}

bool World::isNewCQz(const int _cqz)
{
    return dataProxy->isNewCQz(_cqz);
}

bool World::isNewEntity(const int _entityN)
{
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
       //qDebug() << "World::getQRZContinentShortName: " << QString::number(_enti);
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
       //qDebug() << "World::getQRZContinentNumber: " << QString::number(_enti);
    if (_enti <= 0)
    {
        return -1;
    }
    return dataProxy->getContinentIdFromEntity(_enti);
}

double World::getLongitude(const int _enti)
{
    if (_enti <= 0)
    {
        return 0.0;
    }
    return dataProxy->getLongitudeFromEntity(_enti);
}

double World::getLatitude(const int _enti)
{
    if (_enti <= 0)
    {
        return 0.0;
    }
    return dataProxy->getLatitudeFromEntity(_enti);
}

QString World::getQRZLocator(const QString &_qrz)
{
    //qDebug() << Q_FUNC_INFO << " - Start: " << _qrz;
    if (_qrz.length() < 1)
    {
        return "";
    }
    int i = getQRZARRLId (_qrz);
    //qDebug() << Q_FUNC_INFO << " - 2";
    return locator->getLocator(getLongitude(i), getLatitude (i));
}

QString World::getLocator(const int _entityN)
{
    if (_entityN <= 0)
    {
        return "";
    }
    return locator->getLocator(getLongitude(_entityN), getLatitude(_entityN));
}

bool World::addEntity(const QString &_name, const int _cq, const int _itu, const int _contId, const double _lat, const double _lon, const double _utc, const int _dxcc, const QString &_mainpref)
{
    QSqlQuery query;

    query.prepare("INSERT INTO entity (name, cqz, ituz, cont, latitude, longitude, utc, dxcc, mainprefix) "
                  "VALUES (:name, :cqz, :ituz, :cont, :lat, :lon, :utc, :dxcc, :mainpref)");

    query.bindValue(":name", _name); // name
    query.bindValue(":cqz", _cq); // CQ
    query.bindValue(":ituz", _itu); // ITU
    query.bindValue(":cont", _contId); // Cont
    query.bindValue(":lat", _lat); // Lat
    query.bindValue(":lon", _lon); // Lon
    query.bindValue(":utc", _utc); // UTC
    query.bindValue(":dxcc", _dxcc); // dxcc
    query.bindValue(":mainpref", _mainpref); // Mainprefix

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
    //else if (query.lastError().nativeErrorCode() == QString::number(19))
    //{
        //qDebug() << Q_FUNC_INFO << "  Entity data NOT added: error19:  " <<  _name ;
    //}
    //else
    //{
        //qDebug() << Q_FUNC_INFO << "  Entity data NOT added: error else:  " <<  _name ;
        //emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        //qDebug() << Q_FUNC_INFO << " Entity data NOT added" ;
        //qDebug() << Q_FUNC_INFO << " LastQuery: " << query.lastQuery() ;
        //qDebug() << Q_FUNC_INFO << " LastError-data: " << query.lastError().databaseText() ;
        //qDebug() << Q_FUNC_INFO << " LastError-driver: " << query.lastError().driverText() ;
        //qDebug() << Q_FUNC_INFO << " LastError-n: " << query.lastError().nativeErrorCode();
    //}
    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    return false;
}

bool World::addPrefix(const QString &_pref, const int _dxcc, const int _cqz, const int _ituz)
{
    //qDebug() << Q_FUNC_INFO << ": _pref: " << _pref;
    //qDebug() << Q_FUNC_INFO << ": _dxcc: " << QString::number(_dxcc);
    //qDebug() << Q_FUNC_INFO << ": _cqz: " << QString::number(_cqz);
    //qDebug() << Q_FUNC_INFO << ": _ituz: " << QString::number(_ituz);
    QSqlQuery query;
    query.prepare("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES (:pref, :dxcc, :cqz, :ituz)");

    query.bindValue(":pref", _pref); // Cont
    query.bindValue(":dxcc", _dxcc); // Cont
    query.bindValue(":cqz", _cqz); // CQ
    query.bindValue(":ituz", _ituz); // ITU
    if (query.exec())
    {
        //qDebug() << Q_FUNC_INFO << "  Prefix data added OK: " <<  _pref ;
        //qDebug() << Q_FUNC_INFO << "  query: " <<  query.executedQuery();
        //qDebug() << Q_FUNC_INFO << "  query: " <<  query.lastQuery();
        return true;
    }
    else if (query.lastError().nativeErrorCode() == QString::number(19))
    {
        //qDebug() << Q_FUNC_INFO << "  :Prefix data NOT added: error19:  " <<  _pref ;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << "  Pref data NOT added: error else:  " <<  _pref ;
        //qDebug() << Q_FUNC_INFO << " LastQuery: " << query.lastQuery() ;
        //qDebug() << Q_FUNC_INFO << " LastError-data: " << query.lastError().databaseText() ;
        //qDebug() << Q_FUNC_INFO << " LastError-driver: " << query.lastError().driverText() ;
        //qDebug() << Q_FUNC_INFO << " LastError-n: " << query.lastError().nativeErrorCode();
    }
    emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    return false;
}

bool World::readCTYCSV(const QString &_worldFile)
{
#ifdef KLOG_TESTING
    qDebug() << Q_FUNC_INFO << " - We are testing";
#endif
#ifndef KLOG_TESTING
    qDebug() << Q_FUNC_INFO << " - We are NOT testing";
#endif
    qDebug() << Q_FUNC_INFO << _worldFile;
    QString tq;
    tq.clear();

    qint64 beginingOfFile;
    int numberOfLines = 0;

    //qDebug() << Q_FUNC_INFO << " - 10";
    QFile file( _worldFile );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  /* Flawfinder: ignore */
    {
        //qDebug() << Q_FUNC_INFO << ":  File not found: END FALSE" << _worldFile;
        return false;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << "  File found: " << _worldFile;
    }
    //qDebug() << Q_FUNC_INFO << " - 20";
    beginingOfFile = file.pos();

    while (!file.atEnd()) {
        if ( (file.readLine()).contains(';')  )
        {
            numberOfEntities++;
        }
        numberOfLines++;
    }
    //qDebug() << Q_FUNC_INFO << " - 30";
    //qDebug() << Q_FUNC_INFO << "  - numberOfEntities: " << QString::number(numberOfEntities);
    // The file is readed twice: 1: Main entity data; 2: prefixes.

    // Starts with main data:
    file.seek(beginingOfFile);
    progressBarPosition = 0;
#ifndef KLOG_TESTING
    QProgressDialog progress(tr("Reading cty.csv..."), tr("Abort reading"), 0, numberOfLines);
    progress.setWindowModality(Qt::ApplicationModal);
#endif
    numberOfEntities = 0; // Reset this variable to reuse it and assign the "dxcc" to the entities (temp solution)
    //qDebug() << Q_FUNC_INFO << " - 40";

      // Prefixes information

    //qDebug() << Q_FUNC_INFO << " - 50";

    QStringList stringList, stringListPrefixes, stringListProcessedPrefix;

    int entN;
    //qDebug() << Q_FUNC_INFO << " - 60";
    while (!file.atEnd()) {
#ifndef KLOG_TESTING
        progress.setValue(progressBarPosition);

        if (progress.wasCanceled())
            break;
#endif
        progressBarPosition++;
        stringList.clear();
        stringListPrefixes.clear();
    //
    //     9Y,Trinidad & Tobago,90,SA,9,11,10.38,61.28,4.0,9Y 9Z;
    //     5A,Libya,436,AF,34,38,27.20,-16.60,-2.0,5A;
    // MainPref / Name / dxcc / Cont / CQ / ITU / LAT / LON / -UTC / prefixes
    //    0        1      2      3      4    5     6     7      8       9

        tq = file.readLine();
        //qDebug() << Q_FUNC_INFO << "  Line: " << tq;
        tq = tq.simplified();
        //qDebug() << Q_FUNC_INFO << "  Line simplified: " << tq;
        tq = tq.trimmed();
        //qDebug() << Q_FUNC_INFO << "  Line trimmed: " << tq;
        tq.remove(QChar(';'), Qt::CaseInsensitive);
        //qDebug() << Q_FUNC_INFO << "  Line without ;: " << tq;

        stringList << tq.split(',');

        //qDebug() << Q_FUNC_INFO << "  Line stringList-0: " << stringList.at(0);
        //qDebug() << Q_FUNC_INFO << "  Line stringList Length: " << QString::number(stringList.length());

        // stringList.at(9) contains an space separated list of prefixes for that entity
        QString mPrefix = QString();
        if (stringList.size()==10 )
        {
            int entityNumber;
            if (( stringList.at(0)).contains(QChar('*'), Qt::CaseInsensitive))
            { // This is a special Entity. Not really an ARRL Entity but interesting for the DXer.
                // From http://www.country-files.com/cty-dat-format
                // (A “*” preceding this prefix indicates that the country is on the DARC WAEDC list, and counts in CQ-sponsored contests, but not ARRL-sponsored contests).
                entN = (stringList.at(2)).toInt() + 1000;

                while ( (dataProxy->getEntityMainPrefix(entN)).size()>0  )
                {
                    //qDebug() << Q_FUNC_INFO << "  entN: " << QString::number(entN);
                    //qDebug() << Q_FUNC_INFO << "  dataProxy->getEntityMainPrefix: " << QString::number(entN);
                    entN = entN + 1000;
                }
                entityNumber = entN;
            }
            else
            {
                entityNumber = stringList.at(2).toInt();
            }

            mPrefix = stringList.at(0);
            QString entName = stringList.at(1);
            //QString continent = stringList.at(3);
            int contId = dataProxy->getContinentIdFromContinentShortName(stringList.at(3));
            int cqz = stringList.at(4).toInt();
            int ituz = stringList.at(5).toInt();
            double lat = stringList.at(6).toDouble();
            double lon = stringList.at(7).toDouble();
            double utc = stringList.at(8).toDouble();

            bool entityAdded = addEntity(entName, cqz, ituz, contId, lat, lon, utc, entityNumber, mPrefix);
            if (entityAdded)
            {
                //qDebug() << Q_FUNC_INFO << "  Entity added: " << entName;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << "  Entity Not added: " << entName;
            }
            if (entityAdded)
            {
                //qDebug() << Q_FUNC_INFO << "  Let's going for the prefixes: ... if entity is not added, we should not go to the prefixes!" ;
                QString listOfPrefixesString = stringList.at(9);
                stringListPrefixes << listOfPrefixesString.split(' ');
                QString prefAux = QString();
                for (int i = 0; i < stringListPrefixes.size(); ++i)
                {
                    //qDebug() << Q_FUNC_INFO << " - 100 - " << stringListPrefixes.at(i);
                    prefAux = stringListPrefixes.at(i);
                    //qDebug() << Q_FUNC_INFO << " - 101 - " << prefAux;
                    QStringList stringListProcessedPrefix;
                    stringListProcessedPrefix.clear();
                    //qDebug() << Q_FUNC_INFO << " - 102 - " ;
                    stringListProcessedPrefix << readZones (prefAux, cqz, ituz);
                    //qDebug() << Q_FUNC_INFO << " - 103 - " ;
                    //Returns a QStringList: prefix, CQz, ITUz

                    bool prefixAdded = addPrefix(stringListProcessedPrefix.at(0), entityNumber, stringListProcessedPrefix.at(1).toInt(), stringListProcessedPrefix.at(2).toInt());
                    if (prefixAdded)
                    {
                       //qDebug() << Q_FUNC_INFO << ": Prefix added: " << stringListProcessedPrefix.at(0);
                    }
                    else
                    {
                       //qDebug() << Q_FUNC_INFO << ": Prefix NOT added: " << stringListProcessedPrefix.at(0);
                    }
                }
            }
        }
#ifndef KLOG_TESTING
        progress.setLabelText("Reading cty.csv ... \nNow reading " + mPrefix + " data");
#endif
#ifdef KLOG_TESTING
        qDebug() << Q_FUNC_INFO << QString("Reading cty.csv... Now reading %1").arg(mPrefix);
#endif
        //qDebug() << Q_FUNC_INFO << " - progressBarPosition: " << QString::number(progressBarPosition);
    }
    //qDebug() << Q_FUNC_INFO << " - 100";
#ifndef KLOG_TESTING
    progress.setValue(numberOfLines);
#endif
    //qDebug() << Q_FUNC_INFO << " - 102";
    if (created)
    {
        //qDebug() << Q_FUNC_INFO << " - 110";
        dataProxy->updateISONames();
    }
    //qDebug() << Q_FUNC_INFO << " END TRUE " ;
    return true;
}

/*QStringList World::getEntitiesNames()
{
       //qDebug()  << "World::getEntitiesNames" ;
     return dataProxy->getEntitiesNames();
}
*/

int World::getHowManyEntities()
{
    return dataProxy->getHowManyEntities();
}

bool World::insertSpecialEntities()
{ //https://en.wikipedia.org/wiki/Non-ITU_prefix
    //qDebug() << Q_FUNC_INFO;
    // T9 to E7
    // 4N & YZ to 4O
    int entityID = dataProxy->getEntityIdFromMainPrefix("E7");
    int cqz = dataProxy->getCQZFromId(entityID);
    int ituz = dataProxy->getITUzFromEntity(entityID);
    QString queryString = QString("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES ('%1', '%2', '%3', '%4') ").arg("T9").arg(entityID).arg(cqz).arg(ituz);
    QSqlQuery query;
    bool sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        query.finish();
        //qDebug() << Q_FUNC_INFO << " : T9 not added ";
        return false;
    }
    entityID = dataProxy->getEntityIdFromMainPrefix("4O");
    cqz = dataProxy->getCQzFromEntity(entityID);
    ituz = dataProxy->getITUzFromEntity(entityID);
    queryString = QString("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES ('%1', '%2', '%3', '%4') ").arg("4N").arg(entityID).arg(cqz).arg(ituz);

    sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        query.finish();
        //qDebug() << Q_FUNC_INFO << " : 4N not added ";
        return false;
    }
    queryString = QString("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES ('%1', '%2', '%3', '%4') ").arg("YZ").arg(entityID).arg(cqz).arg(ituz);

    sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        query.finish();
        //qDebug() << Q_FUNC_INFO << " : YZ not added ";
        return false;
    }
    return true;
    //qDebug() << Q_FUNC_INFO << " - END";
}

bool World::hasSpecialEntities()
{ // Checks if T9 is added to the list of prefixes to validate if special prefixes have been added.
    //qDebug() << Q_FUNC_INFO;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    query.finish();
    return false;
}


