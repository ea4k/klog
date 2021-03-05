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
    //qDebug() << "World::World(0)" << _parentFunction << endl;
    constrid = 1;

    worldModel = new QSqlRelationalTableModel(this);
    numberOfEntities = 0;
    progressBarPosition = 0;
    cqz = -1;
    ituz = -1;
    ret = false;
    continentId = -1;
    lat = 0.0;
    lon = 0.0;
    utc = 0.0;
    klogDir = "";
    klogVersion = "";
    locator = new Locator();
    created = false;
    dataProxy = dp;
    //awards = new Awards(Q_FUNC_INFO);
      //qDebug() << "World::World(0) - END" << endl;
}

World::World(DataProxy_SQLite *dp, const QString &_klogDir, const QString &_parentFunction)
{
      //qDebug() << "World::World(1): Dir" << _klogDir << _parentFunction << endl;
    constrid = 2;
    klogDir = _klogDir;
    klogVersion = "";
    worldModel = new QSqlRelationalTableModel(this);
    numberOfEntities = 0;
    progressBarPosition = 0;
    created = false;
    cqz = -1;
    ituz = -1;
    ret = false;
    continentId = -1;
    lat = 0.0;
    lon = 0.0;
    utc = 0.0;
    //flagsDir=":/flags/";
    locator = new Locator();
    dataProxy = dp;
      //qDebug() << "World::World(1): - END"  << endl;

}

World::World(DataProxy_SQLite *dp, const QString &_klogDir, const QString &_klogVer, const QString &_parentFunction)
{
   //qDebug() << "World::World(2): Dir" << _klogDir << _parentFunction << endl;
    constrid = 3;
    klogDir = _klogDir;
    klogVersion = _klogVer;
    //qDebug() << "World::World(2): 1" << endl;
    worldModel = new QSqlRelationalTableModel(this);
    //qDebug() << "World::World(2): 2" << endl;
    numberOfEntities = 0;
    progressBarPosition = 0;
    created = false;
    cqz = -1;
    ituz = -1;
    ret = false;
    continentId = -1;
    lat = 0.0;
    lon = 0.0;
    utc = 0.0;
    //appDir = QString();
    //flagsDir=":/flags/";
      //qDebug() << "World::World(2): 2" << endl;
    locator = new Locator();
      //qDebug() << "World::World(2): 3" << endl;
    dataProxy = dp;
    //qDebug() << "World::World(2): - END" << endl;

}

World::~World()
{
      //qDebug() << "World::~World" << endl;
}


bool World::recreate(const QString &_worldFile)
{
     //qDebug() << "World::recreate: " << _worldFile << endl;

    QSqlQuery query;
    if (query.exec("DELETE FROM entity"))
    {
          //qDebug() << "World::recreate: EMPTY entity"  << endl;
        if (query.exec("DELETE FROM prefixesofentity"))
        {
             //qDebug() << "World::recreate: EMPTY prefixesofentity"  << endl;
             return create(_worldFile);
        }
        else
        {//TODO: Manage the query error
             //qDebug() << "World::recreate: FAILED TO EMPTY prefixesofentity"  << endl;
            emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            return false;
        }
    }
    else
    {//TODO: Manage the query error
         //qDebug() << "World::recreate: FAILED TO EMPTY entity"  << endl;
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
        return false;
    }

    //return false;
}

bool World::create(const QString &_worldFile)
{
       //qDebug() << "World::create: " << _worldFile << endl;
    //klogDir = _worldFile;
       //qDebug() << "World::create: 2 " <<  klogDir << endl;


    if (readCTYCSV(_worldFile))
    {
        created = true;
         //qDebug() << "World::create: TRUE"  << endl;

    }else
    {
        created = false;
         //qDebug() << "World::create: FALSE"  << endl;

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
               //qDebug() << "World::create: updateISONames TRUE"  << endl;
        }
        else
        {
               //qDebug() << "World::create: updateISONames FALSE"  << endl;
        }
    }

       //qDebug() << "World::create: END"  << endl;
    return created;
}

void World::createWorldModel()
{
    worldModel->setTable("entity");

    worldModel->setRelation(Entity_Continent, QSqlRelation("continent", "id", "shortname"));
    worldModel->setSort(Entity_Name, Qt::AscendingOrder);
    worldModel->setHeaderData(Entity_Name, Qt::Horizontal, tr("Entity"));
    worldModel->setHeaderData(Entity_Continent, Qt::Horizontal, tr("Continent"));

    worldModel->select();

}

QStringList World::processLine(const QString &_line)
{

       //qDebug() << "World::processLine: received: " << _line << endl;
    //QString queryString;
    QStringList aa;
    QString line;
    line = (_line).simplified();
    if ((line).count('\'')) // Replaces ' by _
    //The error comes from "Cote d'Ivoire" that breaks the SQL
    {
        line.replace(QChar('\''), QChar('_'));
    }
       //qDebug() << "World::processLine: Received: " << line << endl;
    //QSqlQuery query1;

    nullValue=-1;


    if ( (line.count(':') == 8 ) ) // First line of an Entity
    { //United States:            05:  08:  NA:   43.00:    87.90:     5.0:  K:
           //qDebug() << "World::processLine first: " << line << endl;
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

        //queryString = "SELECT id FROM continent WHERE shortname=='" + continentName + "'";
        //query1.exec(queryString);
        //query1.next();
        //if (query1.isValid())
        //{
        //    continentId = (query1.value(0)).toInt();
        //}


            //qDebug() << "World::processLine Query - Read/continenId: " << continentName << "/" << QString::number(continentId) << endl;

        //queryString = QString("INSERT INTO entity (id, name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix) VALUES (NULL,'%1','%2','%3','%4','%5','%6','%7','%8','%9');\n").arg(entityName).arg(cqz).arg(ituz).arg(QString::number(continentId)).arg(lat).arg(lon).arg(utc).arg(numberOfEntities).arg(prefix);
        aa << entityName << QString::number(cqz) << QString::number(ituz) <<QString::number(continentId) << QString::number(lat) << QString::number(lon) << QString::number(utc) << QString::number(numberOfEntities) << prefix;
        progressBarPosition++;
        return aa;
        //ret = query.exec(queryString);

    }
    aa.clear();
    return aa;
}




QStringList World::processLineP(const QString &_line, const int _processingEntity){
    //Returns QStringList: prefix << dxcc << cqz << ituz OR CurrentEntity as a number

       //qDebug() << "World::processLineP: Entity/received: " << QString::number(_processingEntity) << "/" << _line << endl;
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

       //qDebug() << "World::processLineP: Received: " << line << endl;
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
           //qDebug() << "World::processLineP last line (; detected): " << line << endl;
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
               //qDebug() << "World::processLineP Query (MORE one final)(line):" << line << endl;
            _list.clear();
            _list << line.split(',');
            aa.clear();
            //queryString.clear();

            for (int i = 0; i < _list.size(); ++i)
            {
            // PROCESS THE LINE
                   //qDebug() << "World::processLineP LastLine prefixes" << _list.at(i) << endl;
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
           //qDebug() << "World::processLine middle (no ; detected): " << line << endl;

        _cqz = dataProxy->getCQzFromEntity(currentEntity);
        _ituz = dataProxy->getITUzFromEntity(currentEntity);

        if (line.endsWith(','))
        {
            line.chop(1);
        }

        if ((line.split(',')).size() == 1) // Only one prefix in the middle line
        { // Not usual, added this check for sanity reasons only
               //qDebug() << "World::processLine Query: (only one middle) " << endl;
            line = line.remove(',');
            prefixAndZones = readZones(line, _cqz, _ituz);
            aa.clear();
            aa << prefixAndZones.at(0) << QString::number(currentEntity) << prefixAndZones.at(1) << prefixAndZones.at(2);

            return aa;
        }
        else
        {
               //qDebug() << "World::processLine Query: (MORE than one middle) " << endl;
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
        //qDebug() << "World::readZones: (" << pref << "/" << QString::number(_cq) <<"/" << QString::number(_itu)<< ")" << endl;

    QStringList result;
    int cq = _cq;
    int itu = _itu;
    QString azone;
    QString aux = pref;

    if (aux.startsWith("=")){
        aux = aux.remove(0,1);

    }

    if(aux.count('[')==1) // Check if has special CQz
    {
           //qDebug() << "World::readZones DETECTED [ !!!!" << endl;
        azone = (aux.midRef(aux.indexOf('[')+1)).toString();
           //qDebug() << "World::readZones (ITU)-1: " << aux << " right of " << QString::number(aux.indexOf('[')) << " = " << azone << endl;
        itu = (azone.left(azone.indexOf(']'))).toInt();
           //qDebug() << "World::readZones (ITU)-2: " << azone.left(azone.indexOf(']')) << endl;
        aux = aux.left(aux.indexOf('['));
           //qDebug() << "World::readZones (ITU): "  << pref << "/" << QString::number(itu) << "/" << aux << endl;
    }

    if(aux.count('(')==1) // Check if has special CQz
    {
           //qDebug() << "World::readZones DETECTED ( !!!!" << endl;
        azone = (aux.midRef(aux.indexOf('(')+1)).toString();
        cq = (azone.left(azone.indexOf(')'))).toInt();
        aux = aux.left(aux.indexOf('('));
           //qDebug() << "World::readZones (CQ): "  << pref << "/" << QString::number(cq) << "/" << aux << endl;
    }
       //qDebug() << "World::readZones (Pref/CQ/ITU): "  << pref << "= " << aux <<"/" << QString::number(cq) << "/" << QString::number(itu) << endl;

    result << aux << QString::number(cq) << QString::number(itu);
       //qDebug() << "World::readZones (Pref/CQ/ITU): " << result << endl;
    return result;


}

int World::getPrefixId(const QString &_qrz)
{
      //qDebug() << "World::getPrefixId: -" << _qrz <<"-" << endl;
    //TODO: Instead of going from long to short, identify prefixes from the begining:
    // character(may be number) + number

    if (_qrz.length() < 1)
    {
        return -1;
    }
    int entityID = 0;

    QString aux = changeSlashAndFindPrefix((_qrz).toUpper());

    while ((entityID <= 0) && (aux.length()>=1) )
    {
          //qDebug() << "World::getPrefixId: in the while before calling dataProxy: " << aux << endl;
        if (aux == "KG4")
        {
            if (_qrz.length()!=5)
            {
                aux = "K";
            }
        }
        entityID = dataProxy->getDXCCFromPrefix(aux);
          //qDebug() << "World::getPrefixId: in the while After calling dataProxy" << endl;

          //qDebug() << "World::getPrefixId: in the while" << aux << " = " <<  QString::number(entityID) << endl;
        if (entityID<=0)
        {
            aux.chop(1);
        }

    }
      //qDebug() << "World::getPrefixId: END: " <<  _qrz << QString::number(entityID) << endl;
    return entityID;
}

QString World::getQRZEntityName(const QString &_qrz)
{
        //qDebug() << "World::getQRZEntityName: " << _qrz << endl;
    if (_qrz.length() < 1 )
    {
        return QString();
    }
    //QString queryString;
    //QSqlQuery query;
    int prefixIDNumber = getPrefixId(_qrz);
    return getEntityName(prefixIDNumber);
/*
    queryString = "SELECT dxcc FROM prefixesofentity WHERE id=='" + QString::number(prefixIDNumber) +"'";
       //qDebug() << "World::getQRZEntityName: queryString-1: " << queryString << endl;
    query.exec(queryString);
    query.next();
    if (query.isValid())
    {
        prefixIDNumber = (query.value(0)).toInt();
    }
    else
    {
        prefixIDNumber = -1;
    }

       //qDebug() << "World::getQRZEntityName: " <<_qrz << " = " <<  (query.value(0)).toString() << endl;

    queryString = "SELECT name FROM entity WHERE dxcc=='" + QString::number(prefixIDNumber) +"'";
       //qDebug() << "World::getQRZEntityName: queryString-2: " << queryString << endl;
    query.exec(queryString);
    query.next();
       //qDebug() << "World::getQRZEntityName end: " << _qrz << " = " << (query.value(0)).toString() << endl;

    if (query.isValid()){
        return (query.value(0)).toString();
    }else{
        return "";
    }

    return "";
*/
}

QString World::getEntityName(const int _entityN)
{
    int prefixIDNumber = _entityN;
    if (prefixIDNumber<=0)
    {
        return QString();
    }
    return dataProxy->getEntityNameFromId(prefixIDNumber);
/*
    QString queryString;
    QSqlQuery query;

    queryString = QString("SELECT name FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(prefixIDNumber);

    //queryString = "SELECT name FROM entity WHERE dxcc=='" + QString::number(prefixIDNumber) +"'";
       //qDebug() << "World::getEntityName: queryString-2: " << queryString << endl;
    query.exec(queryString);
    query.next();


    if (query.isValid()){
        return (query.value(0)).toString();
    }else{
        return "";
    }

    return "";
*/
}

int World::getQRZCqz(const QString &_qrz)
{
        //qDebug() << "World::getQRZCqz: " << _qrz << endl;

     if (_qrz.length() < 1 )
     {
         return -1;
     }

     int i = -1;
     QString aux = _qrz;

     while ((i <= 0) && (aux.length()>=1) )
     {
         i = dataProxy->getCQzFromPrefix(aux);
         if (i<=0)
         {
             aux.chop(1);
         }
     }
     return i;



    //int prefixIdNumber = getPrefixId(_qrz);
    //return getEntityCqz(prefixIdNumber);

}

int World::getQRZItuz(const QString &_qrz)
{
       //qDebug() << "World::getQRZItuz: " << _qrz << endl;
    if (_qrz.length() < 1 )
    {
        return -1;
    }

    int i = -1;
    QString aux = _qrz;

    while ((i <= 0) && (aux.length()>=1) )
    {
        i = dataProxy->getITUzFromPrefix(aux);
        if (i<=0)
        {
            aux.chop(1);
        }
    }
    return i;

}


int World::getEntityCqz(const int _enti)
{
    if (_enti < 1 )
    {
        return -1;
    }

    return dataProxy->getCQzFromEntity(_enti);
/*
    QSqlQuery query;
    QString queryString;

    //queryString = QString("SELECT cqz FROM prefixesofentity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(prefixIDNumber);
    queryString = "SELECT cqz FROM prefixesofentity WHERE dxcc=='" + QString::number(_enti) +"'";
    query.exec(queryString);
    query.next();
       //qDebug() << "World::getEntityCqz: " << QString::number(_enti) << " = " <<  (query.value(0)).toInt() << endl;

    if (query.isValid()){
        return (query.value(0)).toInt();
    }else{
        return -1;
    }

    return -1;
*/
}


int World::getEntityItuz(const int _enti)
{
    if (_enti < 1 )
    {
        return -1;
    }
    return dataProxy->getITUzFromEntity(_enti);
/*
    QSqlQuery query;
    QString queryString;

    queryString = "SELECT ituz FROM prefixesofentity WHERE dxcc=='" + QString::number(_enti) +"'";
    query.exec(queryString);
    query.next();
       //qDebug() << "World::getEntityItuz: " << QString::number(_enti) << " = " <<  (query.value(0)).toInt() << endl;

    if (query.isValid()){
        return (query.value(0)).toInt();
    }else{
        return -1;
    }

    return -1;
*/
}

int World::getQRZARRLId(const QString &_qrz)
{
      //qDebug() << "World::getQRZARRLId" << _qrz << endl;

    if (_qrz.length() < 1 )
    {
        return -1;
    }

    int prefixIdNumber = getPrefixId(_qrz);
    return prefixIdNumber;

}

QString World::getQRZEntityMainPrefix(const QString &_qrz)
{
    if (_qrz.length() < 1 )
    {
        return "";
    }
    int i = getQRZARRLId(_qrz);
    return getEntityMainPrefix(i);

    //QString queryString;
    //QSqlQuery query;


/*
    queryString = "SELECT mainprefix FROM entity WHERE dxcc=='" + QString::number(i) +"'";

    if (!query.exec(queryString))
    {
        return "";
    }
    else
    {
        query.next();

           //qDebug() << "World::getQRZEntityMainPrefix(id/qrz): " << QString::number(i) << "/" <<_qrz << " = " <<  (query.value(0)).toString() << endl;
        if (query.isValid()){

            return (query.value(0)).toString();
        }
        else
        {

            return "";
        }
    }




    return "";
*/
}


QString World::getEntityMainPrefix(const int _entityN)
{
    if (_entityN <= 0 )
    {
        return QString();
        //return tr("NONE");
    }
    return dataProxy->getEntityMainPrefix(_entityN);
 /*
    QString queryString;
    QSqlQuery query;

    queryString = QString("SELECT mainprefix FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_entityN);

    //queryString = "SELECT mainprefix FROM entity WHERE dxcc=='" + QString::number(_entityN) +"'";

    if (!query.exec(queryString))
    {
        return "";
    }
    else
    {
        query.next();

        if (query.isValid()){
            return (query.value(0)).toString();
        }else{
            return "";
        }
    }
    return "";
*/
}


bool World::isNewCQz(const int _cqz)
{
    return dataProxy->isNewCQz(_cqz);
/*
    QString queryString;
    QSqlQuery query;
    queryString = "SELECT id FROM log WHERE cqz=='" + QString::number(_cqz) +"'";

    if (!query.exec(queryString))
    {
        return false;
    }
    else
    {
        query.next();

        if (query.isValid()){
            return true;
        }else{
            return false;
        }
    }
    return false;
*/
}

bool World::isNewEntity(const int _entityN)
{
    if (_entityN <= 0)
    {
        return false;
    }
    return dataProxy->isNewEntity(_entityN);
/*
    QString queryString;
    QSqlQuery query;
    queryString = "SELECT id FROM log WHERE dxcc=='" + QString::number(_entityN) +"'";

    if (!query.exec(queryString))
    {
        return false;
    }
    else
    {
        query.next();

        if (query.isValid()){
            return true;
        }else{
            return false;
        }
    }
    return false;
*/
}

QString World::getQRZContinentShortName(const QString &_qrz)
{
        //qDebug() << "World::getQRZContinentShortName: " << _qrz << endl;
    //QString continentNumber = getQRZContinentNumber (_qrz);
    return getContinentShortName (getQRZARRLId(_qrz));

}


QString World::getContinentShortName(const int _enti)
{
       //qDebug() << "World::getQRZContinentShortName: " << QString::number(_enti) << endl;

    //QString continentNumber = QString::number(_enti);

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

/*
    QString queryString;
    QSqlQuery query;
    queryString = QString("SELECT continent FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(_enti);
    //queryString = "SELECT continent FROM entity WHERE dxcc=='" + continentNumber +"'";

    if (!query.exec(queryString))
    {
        return "--";
    }
    else
    {
        query.next();

        if ( !(query.isValid()) ) {
               //qDebug() << "World::getQRZContinentShortName: NO VALID"  << endl;
            return "--";
        }else{
               //qDebug() << "World::getQRZContinentShortName: VALID"  << endl;
            return (query.value(0)).toString();
        }
    }

    return "--";
*/
}

QString World::getQRZContinentNumber(const QString &_qrz)
{
       //qDebug() << "World::getQRZContinentNumber: " << _qrz << endl;

    int i = getQRZARRLId(_qrz);
    return QString::number(getContinentNumber(i));
  /*
    QString a;
    QString queryString;
    QSqlQuery query;

    queryString = QString("SELECT continent FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(i);

    //queryString = "SELECT continent FROM entity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return "-1";
    }
    else
    {
        query.next();
        if ( !(query.isValid()) ) {
              //qDebug() << "World::getQRZContinentNumber(qrz/i/Cont): NO VALID"  << endl;
            return "-1";
        }else{
               //qDebug() << "World::getQRZContinentNumber(qrz/i/Cont): VALID"  << endl;
            a = (query.value(0)).toString();
        }


           //qDebug() << "World::getQRZContinentNumber(qrz/i/Cont): " <<_qrz << "/" << QString::number(i) << "/" <<  a << endl;
        return a;
    }
    return "-1";
*/
}

int World::getContinentNumber(const int _enti)
{
       //qDebug() << "World::getQRZContinentNumber: " << QString::number(_enti) << endl;
    if (_enti <= 0)
    {
        return -1;
    }
    return dataProxy->getContinentIdFromEntity(_enti);
/*
    int a = -1;

    QString queryString;
    QSqlQuery query;
    int i = _enti;
    queryString = QString("SELECT continent FROM entity WHERE (mainprefix NOT LIKE '*%') AND dxcc='%1'").arg(i);
    //queryString = "SELECT continent FROM entity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return -1;
    }
    else
    {
        query.next();
        if ( !(query.isValid()) )
        {
            return -1;
        }else{
            a = (query.value(0)).toInt();
            queryString = "SELECT id FROM continent where shortname=='" + query.value(0).toString() + "'";
            if (query.exec(queryString))
            {
                query.next();
                if(query.isValid())
                {
                    return query.value(0).toInt();
                }
                else
                { // Value not valid
                    return -1;
                }
            }
            else
            { // Error in the query
                return -1;
            }

        }
        return a;
    }
    return -1;
*/
}

double World::getQRZLongitude(const QString &_qrz)
{

    int i = getQRZARRLId(_qrz);
    return dataProxy->getLongitudeFromEntity(i);
/*
    QString queryString;
    QSqlQuery query;
    queryString = "SELECT longitude FROM entity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) ) {
               //qDebug() << "World::getQRZLongitude: NO VALID"  << endl;
            return 0.0;
        }else{
               //qDebug() << "World::getQRZLongitude: VALID"  << endl;
            return (query.value(0)).toDouble();
        }

    }
    return 0.0;
*/
}

double World::getLongitude(const int _enti)
{
    if (_enti <= 0)
    {
        return 0.0;
    }

    return dataProxy->getLongitudeFromEntity(_enti);


/*

    QString queryString;
    QSqlQuery query;

    int i = _enti;
    queryString = "SELECT longitude FROM entity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) ) {
               //qDebug() << "World::getQRZLongitude: NO VALID"  << endl;
            return 0.0;
        }else{
               //qDebug() << "World::getQRZLongitude: VALID"  << endl;
            return (query.value(0)).toDouble();
        }

    }
    return 0.0;
*/
}

double World::getQRZLatitude(const QString &_qrz)
{

    int i = getQRZARRLId(_qrz);
    return dataProxy->getLatitudeFromEntity(i);
 /*
    QString queryString;
    QSqlQuery query;
    queryString = "SELECT latitude FROM entity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) ) {
               //qDebug() << "World::getQRZLatitud: NO VALID"  << endl;
            return 0.0;
        }else{
               //qDebug() << "World::getQRZLatitud: VALID"  << endl;
            return (query.value(0)).toDouble();
        }
    }

    return 0.0;
*/
}

double World::getLatitude(const int _enti)
{
    if (_enti <= 0)
    {
        return 0.0;
    }
    return dataProxy->getLatitudeFromEntity(_enti);

/*
    QString queryString;
    QSqlQuery query;
    int i = _enti;
    queryString = "SELECT latitude FROM entity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return 0.0;
    }
    else
    {
        query.next();

        if ( !(query.isValid()) ) {
               //qDebug() << "World::getQRZLatitud: NO VALID"  << endl;
            return 0.0;
        }else{
               //qDebug() << "World::getQRZLatitud: VALID"  << endl;
            return (query.value(0)).toDouble();
        }
    }

    return 0.0;
*/
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

/*
int World::getBandIdFromFreq(const QString fr)
{
       //qDebug() << "World::getBandIdFromFreq: " << fr  << endl;
    //Freq should be in MHz
    QSqlQuery query;
    QString queryString = QString("SELECT id FROM band WHERE lower <= '%1' and upper >= '%2'").arg(fr).arg(fr);
       //qDebug() << "World::getBandIdFromFreq query: " << queryString  << endl;


    if (!query.exec(queryString))
    {
        return -1;
    }
    else
    {
        query.next();

        int b=-1;
        if (query.isValid())
        {
            b = (query.value(0)).toInt();
               //qDebug() << "World::getBandIdFromFreq value: " << QString::number(b)  << endl;
            //return (query.value(0)).toInt();
            return b;
        }
        else
        {
               //qDebug() << "World::getBandIdFromFreq value not valid!!" << endl;
            return -1;
        }
    }

    return -1;
}
*/



QString World::getQRZEntityPrefixes(const QString &_qrz)
{

    int i = getQRZARRLId(_qrz);
    return dataProxy->getEntityMainPrefix(i);
/*
    QString result;
    result = "";
    QString queryString;
    QSqlQuery query;
    queryString = "SELECT prefix FROM prefixesofentity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return "";
    }
    else
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                result = result + ", " + (query.value(0)).toString();
            }
            else
            {
            }
        }

        if (result.length() < 1)
        {
            return result;
        }
        else
        {
            result = result.remove(0,2);
            return result;
        }
    }

    return "";
*/
}

bool World::readCTYCSV(const QString &_worldFile)
{
     //qDebug() << "World::readCTYCSV(): " << _worldFile << endl;

    QString tq;
    tq.clear();
    QString entityNumber;
    //entityNumber = "-1";
    QString fileName;
    qint64 beginingOfFile;
    int numberOfLines = 0;
    //int errorCode = -1;

    QFile file( _worldFile );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          //qDebug() << "World::readCTYCSV() File not found: END FALSE" << fileName << endl;
        return false;
    }
    else
    {
         //qDebug() << "World::readCTYCSV() File found: " << fileName << endl;
    }
    beginingOfFile = file.pos();



    while (!file.atEnd()) {
        if ( (file.readLine()).contains(';')  )
        {
            numberOfEntities++;
        }
        numberOfLines++;
    }
      //qDebug() << "World::readCTYCSV() - numberOfEntities: " << QString::number(numberOfEntities) << endl;

    // The file is readed twice: 1: Main entity data; 2: prefixes.


    // Starts with main data:
    file.seek(beginingOfFile);
    progressBarPosition = 0;
    QProgressDialog progress(tr("Reading cty.csv..."), tr("Abort reading"), 0, numberOfLines, this);
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
           //qDebug()  << "World::readCTYCSV(): Line: " << tq << endl;
        tq = tq.simplified();
           //qDebug()  << "World::readCTYCSV(): Line simplified: " << tq << endl;
        tq = tq.trimmed();
          //qDebug()  << "World::readCTYCSV(): Line trimmed: " << tq << endl;
        tq.remove(QChar(';'), Qt::CaseInsensitive);
          //qDebug()  << "World::readCTYCSV(): Line without ;: " << tq << endl;

        stringList << tq.split(',');

           //qDebug()  << "World::readCTYCSV(): Line stringList-0: " << stringList.at(0) << endl;

        if (( stringList.at(0)).contains(QChar('*'), Qt::CaseInsensitive))
        { // This is a special Entity. Not really an ARRL Entity but interesting for the DXer.
          // From http://www.country-files.com/cty-dat-format
          // (A “*” preceding this prefix indicates that the country is on the DARC WAEDC list, and counts in CQ-sponsored contests, but not ARRL-sponsored contests).
            //entN = -1;
            entN = (stringList.at(2)).toInt() + 1000;

            while ( (dataProxy->getEntityMainPrefix(entN)).size()>0  )
            {
                  //qDebug()  << "World::readCTYCSV() entN: " << QString::number(entN) << endl;
                   //qDebug()  << "World::readCTYCSV() dataProxy->getEntityMainPrefix: " << QString::number(entN) << endl;
                entN = entN + 1000;
            }


            entityNumber = QString::number(entN);
        }
        else
        {
            entityNumber = stringList.at(2);
        }

           //qDebug()  << "World::readCTYCSV(): Line stringList Length: " << QString::number(stringList.length()) << endl;

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


               //qDebug()  << "World::readCTYCSV(): Entity name: " << stringList.at(1) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity cqz:  " << stringList.at(4) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity ituz: " << stringList.at(5) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity cont: " << stringList.at(3) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity lat:  " << stringList.at(6) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity lon:  " << stringList.at(7) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity UTC:  " << stringList.at(8) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity ARRL: " << stringList.at(2) << endl;
               //qDebug()  << "World::readCTYCSV(): Entity Pref: " << stringList.at(0) << endl;


            if (query.exec())
            {
                   //qDebug()  << "World::readCTYDAT(): Entity data added: " <<  stringList.at(1)  << endl;
            }
            else if (query.lastError().number() == 19)
            {
               //qDebug()  << "World::readCTYDAT(): Entity data added: error19:  " <<  stringList.at(1)  << endl;
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
            }
            else
            {
                //qDebug()  << "World::readCTYDAT(): Entity data added: error else:  " <<  stringList.at(1)  << endl;
                emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
                //errorCode = query.lastError().number();


                  //qDebug() << "World::readCTYCSV(): Entity data NOT added"  << endl;
                  //qDebug() << "World::readCTYCSV(): LastQuery: " << query.lastQuery()  << endl;
                  //qDebug() << "World::readCTYCSV(): LastError-data: " << query.lastError().databaseText()  << endl;
                  //qDebug() << "World::readCTYCSV(): LastError-driver: " << query.lastError().driverText()  << endl;
                  //qDebug() << "World::readCTYCSV(): LastError-n: " << QString::number(query.lastError().number() ) << endl;

            }

                //qDebug()  << "World::readCTYCSV(): Entity ADDED or NOT"  << endl;
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
                      //qDebug()  << "World::readCTYCSV(): Prefix: " << stringListPrefixes.at(i) << endl;
                    queryP.addBindValue(QVariant(QVariant::Int));


                    //readZones (const QString &pref, const int _cq, const int _itu)
                    //Returns a QStringList: prefix, CQz, ITUz
                    stringListProcessedPrefix.clear();
                    stringListProcessedPrefix << readZones(prefAux, (stringList.at(4)).toInt(), (stringList.at(5)).toInt());

                    if (prefAux.at(0)=='=')
                    { //TODO: Maybe there is a better way to identify exact calls instead of prefixes , identified with a = before he call.
                        prefAux.remove(0,1);
                    }

                    queryP.addBindValue(stringListProcessedPrefix.at(0));
                    queryP.addBindValue(entityNumber);
                    queryP.addBindValue(stringListProcessedPrefix.at(1));
                    queryP.addBindValue(stringListProcessedPrefix.at(2));

                    if (queryP.exec())
                    {// T9 is the former prefix of Bosnia and Herzegovina; it moved to E7 on 7 August 2007.
                     // YZ and 4N belonged to Yugoslavia, along with 4O (which went to successor state Montenegro)
                     // YT and YU (which went to successor state Serbia)
                           //qDebug()  << "World::readCTYCSV(): Prefix added: " << stringListPrefixes.at(i) << endl;


                    }
                    else if (queryP.lastError().number() == 19)
                    {}
                    else
                    {
                        //errorCode = queryP.lastError().number();
                        emit queryError(Q_FUNC_INFO, queryP.lastError().databaseText(), queryP.lastError().number(), queryP.lastQuery());


                          //qDebug() << "World::readCTYCSV(): Prefix data NOT added"  << endl;
                          //qDebug() << "World::readCTYCSV(): Prefix LastQuery: " << query.lastQuery()  << endl;
                          //qDebug() << "World::readCTYCSV(): Prefix LastError-data: " << query.lastError().databaseText()  << endl;
                          //qDebug() << "World::readCTYCSV(): Prefix LastError-driver: " << query.lastError().driverText()  << endl;
                          //qDebug() << "World::readCTYCSV(): Prefix LastError-n: " << QString::number(query.lastError().number() ) << endl;
                    }

                }
            }

         }

          //qDebug()  << "World::readCTYCSV() tq: " << tq << endl;
        progress.setLabelText("Reading cty.csv ... \nNow reading " + currentPrefix + " data");
          //qDebug() << "World::readCTYCSV() - progressBarPosition: " << QString::number(progressBarPosition) << endl;
    }


    QSqlDatabase::database().commit();

    progress.setValue(numberOfLines);
    if (created)
    {
        dataProxy->updateISONames();
    }

      //qDebug()  << "World::readCTYCSV() END TRUE "  << endl;
    return true;

}

QString World::changeSlashAndFindPrefix(const QString &_qrz)
{
      //qDebug() << "World::changeSlashAndFindPrefix: -"  << _qrz <<"-" << endl;


    QString aux = _qrz.toUpper();

    if ((aux).count('\\')) // Replaces \ by / to ease operation.
    {
        aux.replace(QChar('\\'), QChar('/'));
    }
    else
    {
        return aux;
    }

    if (aux.count('/')) // / found! Checking different options
    {
        int iaux1, iaux2;

        if (aux.endsWith("/") )
        { // We look for calls ending in slash "/" or "\"
            aux.remove(aux.length()-1,1);
        }
        iaux1 = aux.indexOf('/');
            //qDebug() << "World::changeSlashAndFindPrefix: Slash found at: "  << QString::number(iaux1) << endl;

        iaux2 = (aux.length())- iaux1; // iaux2 is the length of the second part
        if (iaux2 < 0){
            iaux2 = -iaux2;
        }

        if ( iaux1 < iaux2 ) { //Like in F/EA0XXX, we can simply take the first part as the prefix
            aux = aux.left(iaux1);
        }
        else
        {
            aux = aux.right(iaux2 -1);
        }
    }
    return aux;
}


bool World::checkQRZValidFormat(const QString &_qrz)
{
        //qDebug()  << "World::checkQRZValidFormat: -" << _qrz <<"-" << endl;

    QString aux = changeSlashAndFindPrefix(_qrz.toUpper());

    if (aux.length()<3)
    {
        return false;
    }

    // The format of a Call is defined here:
    // http://en.wikipedia.org/wiki/ITU_prefix_%28amateur_stations%29
    //bool prefixEnded = false;
    //int callLength = _qrz.length();

    //int sepPos = 0;


    int barPos = aux.indexOf('/');

    //QString prefix = QString();
    //QString suffix = QString();
    //QString separator = QString();
    // If barPos > 0 we have a complex call like F/EA0K

    QCharRef c = aux[aux.length()-1];

    if ( c.isDigit() && (barPos<=0)  )
    {
        return false;
    }

    //prefix = prefix + aux.at(0);

    if ( ( aux.at(1) ).isDigit() )
    { // W3 A6
        if ((aux.at(2)).isDigit() )
        { // A60
            //prefix = prefix + aux.at(1);
            //separator = aux.at(2);
            //sepPos = 2;
        }
        else
        { // W3A
            //separator = aux.at(1);
            //sepPos = 1;
        }
    }
    else
    { // EA 3D 2M

        if ((aux.at(2)).isDigit() )
        { // EA0

            //prefix = prefix + aux.at(1);
            //separator = aux.at(2);
            //sepPos = 2;

        }
        else
        { // 3DA
            if (aux.length() < 4)
            {
                //prefix = prefix + aux.at(2);
                return false;
            }

            if ((aux.at(3)).isDigit())
            { // 3DA0
                //prefix = prefix + aux.at(1);
                //prefix = prefix + aux.at(2);
                //separator = aux.at(3);
                //sepPos = 3;
                if (aux.length() < 5)
                { // 3DA0
                    //prefix = prefix + aux.at(3);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }

    /*
    for (int i = sepPos+1; i < aux.length(); i++)
    {
        suffix = suffix + aux.at(i);
    }
    */

    return true;
}

QStringList World::getEntitiesNames()
{
       //qDebug()  << "World::getEntitiesNames"  << endl;
     return dataProxy->getEntitiesNames();
}

int World::getHowManyEntities()
{
    return dataProxy->getHowManyEntities();
}

bool World::insertSpecialEntities()
{ //https://en.wikipedia.org/wiki/Non-ITU_prefix
    //qDebug() << Q_FUNC_INFO << endl;
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
        //qDebug() << Q_FUNC_INFO << " : T9 not added " << endl;
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
        //qDebug() << Q_FUNC_INFO << " : 4N not added " << endl;
        return false;
    }
    queryString = QString("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES ('%1', '%2', '%3', '%4') ").arg("YZ").arg(entityID).arg(cqz).arg(ituz);

    sqlOK = query.exec(queryString);

    if (!sqlOK)
    {
        query.finish();
        //qDebug() << Q_FUNC_INFO << " : YZ not added " << endl;
        return false;
    }



    //qDebug() << Q_FUNC_INFO << " - END" << endl;

}

bool World::hasSpecialEntities()
{ // Checks if T9 is added to the list of prefixes to validate if special prefixes have been added.
    //qDebug() << Q_FUNC_INFO << endl;
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
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());

    }
    query.finish();
    return false;

}


