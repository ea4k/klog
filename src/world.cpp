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

    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());
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
{
    worldPrefixes.clear();
    worldPrefixes = dataProxy->getWorldData();
    if (worldPrefixes.size()>100)
    {
        return true;
    }
    return false;
}

bool World::recreate(const QString &_worldFile)
{
     //qDebug() << "World::recreate: " << _worldFile;
    QSqlQuery query;
    if (query.exec("DELETE FROM entity"))
    {
          //qDebug() << "World::recreate: EMPTY entity" ;
        if (query.exec("DELETE FROM prefixesofentity"))
        {
             //qDebug() << "World::recreate: EMPTY prefixesofentity" ;
             if (create(_worldFile))
             {
                 return insertSpecialEntities ();
             }
        }
        else
        {//TODO: Manage the query error
             //qDebug() << "World::recreate: FAILED TO EMPTY prefixesofentity" ;
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            return false;
        }
    }
    else
    {//TODO: Manage the query error
         //qDebug() << "World::recreate: FAILED TO EMPTY entity" ;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
        return false;
    }
    return false;
}

bool World::create(const QString &_worldFile)
{
       //qDebug() << "World::create: " << _worldFile;
    //klogDir = _worldFile;
       //qDebug() << "World::create: 2 " <<  klogDir;

    if (readCTYCSV(_worldFile))
    {
        created = true;
         //qDebug() << "World::create: TRUE" ;

    }else
    {
        created = false;
         //qDebug() << "World::create: FALSE" ;
    }
    if (created)
    {
        created = insertSpecialEntities();
    }
    if (created)
    {
        //dataProxy->updateISONames();
        if (dataProxy->updateISONames())
        {
               //qDebug() << "World::create: updateISONames TRUE" ;
        }
        else
        {
               //qDebug() << "World::create: updateISONames FALSE" ;
        }
    }
       //qDebug() << "World::create: END" ;
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
    QString azone;
    QString aux = pref;

    if(aux.count('[')==1) // Check if has special CQz
    {
           //qDebug() << "World::readZones DETECTED [ !!!!";
        azone = (aux.midRef(aux.indexOf('[')+1)).toString();
           //qDebug() << "World::readZones (ITU)-1: " << aux << " right of " << QString::number(aux.indexOf('[')) << " = " << azone;
        itu = (azone.left(azone.indexOf(']'))).toInt();

           //qDebug() << "World::readZones (ITU)-2: " << azone.left(azone.indexOf(']'));
        aux = aux.left(aux.indexOf('['));
           //qDebug() << "World::readZones (ITU): "  << pref << "/" << QString::number(itu) << "/" << aux;
    }

    if(aux.count('(')==1) // Check if has special CQz
    {
           //qDebug() << "World::readZones DETECTED ( !!!!";
        azone = (aux.midRef(aux.indexOf('(')+1)).toString();
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
    //qDebug() << "World::getQRZEntityName: " << _qrz;
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
    //qDebug() << Q_FUNC_INFO << ": " << pref;
    return getPrefixId(pref);
}

QString World::getQRZEntityMainPrefix(const QString &_qrz)
{
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
        //qDebug() << "World::getQRZContinentShortName: " << _qrz;
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
       //qDebug() << "World::getQRZContinentNumber: " << _qrz;
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

double World::getQRZLongitude(const QString &_qrz)
{
    int i = getQRZARRLId(_qrz);
    return dataProxy->getLongitudeFromEntity(i);
}

double World::getLongitude(const int _enti)
{
    if (_enti <= 0)
    {
        return 0.0;
    }
    return dataProxy->getLongitudeFromEntity(_enti);
}

double World::getQRZLatitude(const QString &_qrz)
{
    int i = getQRZARRLId(_qrz);
    return dataProxy->getLatitudeFromEntity(i);
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
    if (_qrz.length() < 1)
    {
        return "";
    }
    return locator->getLocator(getQRZLongitude(_qrz), getQRZLatitude(_qrz));
}

QString World::getLocator(const int _entityN)
{
    if (_entityN <= 0)
    {
        return "";
    }
    return locator->getLocator(getLongitude(_entityN), getLatitude(_entityN));
}

bool World::readCTYCSV(const QString &_worldFile)
{
     //qDebug() << "World::readCTYCSV(): " << _worldFile;
    QString tq;
    tq.clear();
    QString entityNumber;
    //entityNumber = "-1";
    //QString fileName;
    qint64 beginingOfFile;
    int numberOfLines = 0;
    //int errorCode = -1;

    QFile file( _worldFile );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  /* Flawfinder: ignore */
    {
          //qDebug() << "World::readCTYCSV() File not found: END FALSE" << fileName;
        return false;
    }
    else
    {
         //qDebug() << "World::readCTYCSV() File found: " << fileName;
    }
    beginingOfFile = file.pos();

    while (!file.atEnd()) {
        if ( (file.readLine()).contains(';')  )
        {
            numberOfEntities++;
        }
        numberOfLines++;
    }
      //qDebug() << "World::readCTYCSV() - numberOfEntities: " << QString::number(numberOfEntities);
    // The file is readed twice: 1: Main entity data; 2: prefixes.

    // Starts with main data:
    file.seek(beginingOfFile);
    progressBarPosition = 0;
    //QProgressDialog progress(tr("Reading cty.csv..."), tr("Abort reading"), 0, numberOfLines, this);
    QProgressDialog progress(tr("Reading cty.csv..."), tr("Abort reading"), 0, numberOfLines);
    progress.setWindowModality(Qt::ApplicationModal);

    numberOfEntities = 0; // Reset this variable to reuse it and assign the "dxcc" to the entities (temp solution)

    QSqlQuery query;    // Entity information
    QSqlQuery queryP;   // Prefixes information

    query.prepare("INSERT INTO entity (id, name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    queryP.prepare("INSERT INTO prefixesofentity (id, prefix, dxcc, cqz, ituz) VALUES (?, ?, ?, ?, ?)");

    QSqlDatabase::database().transaction();
    QStringList T9StringList;
    T9StringList.clear();

    QStringList stringList, stringListPrefixes, stringListProcessedPrefix;

    int entN;
    //entN = -1;

    while (!file.atEnd()) {
        progress.setValue(progressBarPosition);
        progressBarPosition++;
        if (progress.wasCanceled())
            break;
        stringList.clear();
        stringListPrefixes.clear();
//
//     9Y,Trinidad & Tobago,90,SA,9,11,10.38,61.28,4.0,9Y 9Z;
//     5A,Libya,436,AF,34,38,27.20,-16.60,-2.0,5A;
// MainPref / Name / dxcc / Cont / CQ / ITU / LAT / LON / -UTC / prefixes
//    0        1        2      3      4    5     6     7      8       9

        tq = file.readLine();
           //qDebug()  << "World::readCTYCSV(): Line: " << tq;
        tq = tq.simplified();
           //qDebug()  << "World::readCTYCSV(): Line simplified: " << tq;
        tq = tq.trimmed();
          //qDebug()  << "World::readCTYCSV(): Line trimmed: " << tq;
        tq.remove(QChar(';'), Qt::CaseInsensitive);
          //qDebug()  << "World::readCTYCSV(): Line without ;: " << tq;

        stringList << tq.split(',');

           //qDebug()  << "World::readCTYCSV(): Line stringList-0: " << stringList.at(0);

        if (( stringList.at(0)).contains(QChar('*'), Qt::CaseInsensitive))
        { // This is a special Entity. Not really an ARRL Entity but interesting for the DXer.
          // From http://www.country-files.com/cty-dat-format
          // (A “*” preceding this prefix indicates that the country is on the DARC WAEDC list, and counts in CQ-sponsored contests, but not ARRL-sponsored contests).
            //entN = -1;
            entN = (stringList.at(2)).toInt() + 1000;

            while ( (dataProxy->getEntityMainPrefix(entN)).size()>0  )
            {
                  //qDebug()  << "World::readCTYCSV() entN: " << QString::number(entN);
                   //qDebug()  << "World::readCTYCSV() dataProxy->getEntityMainPrefix: " << QString::number(entN);
                entN = entN + 1000;
            }
            entityNumber = QString::number(entN);
        }
        else
        {
            entityNumber = stringList.at(2);
        }
           //qDebug()  << "World::readCTYCSV(): Line stringList Length: " << QString::number(stringList.length());

        if (stringList.size()>=8 )
        {
        //(id, name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix)
            query.addBindValue(QVariant(QVariant::Int));
            query.addBindValue(stringList.at(1)); // name
            query.addBindValue(stringList.at(4)); // CQ
            query.addBindValue(stringList.at(5)); // ITU
            query.addBindValue(stringList.at(3)); // Cont
            query.addBindValue(stringList.at(6)); // Lat
            query.addBindValue(stringList.at(7)); // Lon
            query.addBindValue(stringList.at(8)); // UTC
            //query.addBindValue(stringList.at(2)); // dxcc
            query.addBindValue(entityNumber); // dxcc
            query.addBindValue(stringList.at(0)); // Mainprefix
               //qDebug()  << "World::readCTYCSV(): Entity name: " << stringList.at(1);
               //qDebug()  << "World::readCTYCSV(): Entity cqz:  " << stringList.at(4);
               //qDebug()  << "World::readCTYCSV(): Entity ituz: " << stringList.at(5);
               //qDebug()  << "World::readCTYCSV(): Entity cont: " << stringList.at(3);
               //qDebug()  << "World::readCTYCSV(): Entity lat:  " << stringList.at(6);
               //qDebug()  << "World::readCTYCSV(): Entity lon:  " << stringList.at(7);
               //qDebug()  << "World::readCTYCSV(): Entity UTC:  " << stringList.at(8);
               //qDebug()  << "World::readCTYCSV(): Entity ARRL: " << stringList.at(2);
               //qDebug()  << "World::readCTYCSV(): Entity Pref: " << stringList.at(0);

            if (query.exec())
            {
                   //qDebug()  << "World::readCTYDAT(): Entity data added: " <<  stringList.at(1) ;
            }
            else if (query.lastError().nativeErrorCode() == QString::number(19))
            {
               //qDebug()  << "World::readCTYDAT(): Entity data added: error19:  " <<  stringList.at(1) ;
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
            }
            else
            {
                //qDebug()  << "World::readCTYDAT(): Entity data added: error else:  " <<  stringList.at(1) ;
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
                //errorCode = query.lastError().nativeErrorCode();
                  //qDebug() << "World::readCTYCSV(): Entity data NOT added" ;
                  //qDebug() << "World::readCTYCSV(): LastQuery: " << query.lastQuery() ;
                  //qDebug() << "World::readCTYCSV(): LastError-data: " << query.lastError().databaseText() ;
                  //qDebug() << "World::readCTYCSV(): LastError-driver: " << query.lastError().driverText() ;
                  //qDebug() << "World::readCTYCSV(): LastError-n: " << QString::number(query.lastError().nativeErrorCode() );
            }
                //qDebug()  << "World::readCTYCSV(): Entity ADDED or NOT" ;
            if (stringList.size()>8)
            {
                tq = stringList.at(9);
                stringListPrefixes << tq.split(' ');
                tq = stringListPrefixes.last();
                QString prefAux = QString();

                for (int i = 0; i < stringListPrefixes.size(); ++i)
                {
                    prefAux = stringListPrefixes.at(i);
                    //queryP.prepare("INSERT INTO prefixesofentity (id, prefix, dxcc, cqz, ituz) VALUES (?, ?, ?, ?, ?)");
                    //                                               0    1       2    3    4
                    //  (id, prefix, dxcc, cqz, ituz)
                      //qDebug()  << "World::readCTYCSV(): Prefix: " << stringListPrefixes.at(i);
                    queryP.addBindValue(QVariant(QVariant::Int));


                    //readZones (const QString &pref, const int _cq, const int _itu)
                    //Returns a QStringList: prefix, CQz, ITUz
                    stringListProcessedPrefix.clear();
                    stringListProcessedPrefix << readZones(prefAux, (stringList.at(4)).toInt(), (stringList.at(5)).toInt());

                    queryP.addBindValue(stringListProcessedPrefix.at(0));
                    queryP.addBindValue(entityNumber);
                    queryP.addBindValue(stringListProcessedPrefix.at(1));
                    queryP.addBindValue(stringListProcessedPrefix.at(2));

                    if (queryP.exec())
                    {// T9 is the former prefix of Bosnia and Herzegovina; it moved to E7 on 7 August 2007.
                     // YZ and 4N belonged to Yugoslavia, along with 4O (which went to successor state Montenegro)
                     // YT and YU (which went to successor state Serbia)
                           //qDebug()  << "World::readCTYCSV(): Prefix added: " << stringListPrefixes.at(i);
                    }
                    else if (queryP.lastError().nativeErrorCode() != QString::number(19))
                    {
                        //errorCode = queryP.lastError().nativeErrorCode();
                        emit queryError(Q_FUNC_INFO, queryP.lastError().databaseText(), queryP.lastError().nativeErrorCode(), queryP.lastQuery());
                          //qDebug() << "World::readCTYCSV(): Prefix data NOT added" ;
                          //qDebug() << "World::readCTYCSV(): Prefix LastQuery: " << query.lastQuery() ;
                          //qDebug() << "World::readCTYCSV(): Prefix LastError-data: " << query.lastError().databaseText() ;
                          //qDebug() << "World::readCTYCSV(): Prefix LastError-driver: " << query.lastError().driverText() ;
                          //qDebug() << "World::readCTYCSV(): Prefix LastError-n: " << QString::number(query.lastError().nativeErrorCode() );
                    }
                }
            }
         }
          //qDebug()  << "World::readCTYCSV() tq: " << tq;
        progress.setLabelText("Reading cty.csv ... \nNow reading " + currentPrefix + " data");
          //qDebug() << "World::readCTYCSV() - progressBarPosition: " << QString::number(progressBarPosition);
    }

    QSqlDatabase::database().commit();

    progress.setValue(numberOfLines);
    if (created)
    {
        dataProxy->updateISONames();
    }
      //qDebug()  << "World::readCTYCSV() END TRUE " ;
    return true;
}

QStringList World::getEntitiesNames()
{
       //qDebug()  << "World::getEntitiesNames" ;
     return dataProxy->getEntitiesNames();
}

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


