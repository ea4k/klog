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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
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
World::World()
{
    //qDebug() << "World::World(0)" << endl;

    worldModel = new QSqlRelationalTableModel(this);
    numberOfEntities = 0;
    progressBarPosition = 0;
    kontestDir = "";
    kontestVersion = "";
    locator = new Locator();
    created = false;
    //awards = new Awards();

}


World::World(const QString _kontestDir)
{
    //qDebug() << "World::World(1): Dir" << _kontestDir << endl;
    kontestDir = _kontestDir;
    kontestVersion = "";
    worldModel = new QSqlRelationalTableModel(this);
    numberOfEntities = 0;
    progressBarPosition = 0;
    created = false;
    locator = new Locator();
    //awards = new Awards();

}

World::World(const QString _kontestDir, const QString _kontestVer)
{
    //qDebug() << "World::World(2): Dir" << _kontestDir << endl;
    kontestDir = _kontestDir;
    kontestVersion = _kontestVer;
    worldModel = new QSqlRelationalTableModel(this);
    numberOfEntities = 0;
    progressBarPosition = 0;
    created = false;
    locator = new Locator();
    //awards = new Awards();

}

World::~World()
{
    //qDebug() << "World::~World" << endl;
}

bool World::recreate(const QString _kontestDir)
{
    //qDebug() << "World::recreate: " << _kontestDir << endl;

    QSqlQuery query;
    if (query.exec("DELETE FROM entity"))
    {
        if (query.exec("DELETE FROM prefixesofentity"))
        {
             return create(_kontestDir);
        }
        else
        {//TODO: Manage the query error
            return false;
        }
    }
    else
    {//TODO: Manage the query error
        return false;
    }
    return false;
}

bool World::create(const QString _kontestDir)
{
    //qDebug() << "World::create: " << _kontestDir << endl;
    kontestDir = _kontestDir;
    //qDebug() << "World::create: 2 " <<  kontestDir << endl;
    //if (readCTYDAT())
    if (readCTYCSV())
    {
        created = true;
        //qDebug() << "World::create: TRUE"  << endl;

    }else
    {
        created = false;
        //qDebug() << "World::create: FALSE"  << endl;

    }
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

QStringList World::processLine(const QString _line)
{

    //qDebug() << "World::processLine: received: " << _line << endl;
    QString queryString;
    QStringList aa;
    QString line;
    line = (_line).simplified();
    if ((line).count('\'')) // Replaces ' by _
    //The error comes from "Cote d'Ivoire" that breaks the SQL
    {
        line.replace(QChar('\''), QChar('_'));
    }
    //qDebug() << "World::processLine: Received: " << line << endl;
    QSqlQuery query1;

    nullValue=-1;

    //queryString = QString("SELECT id FROM entity WHERE dxcc='%1'").arg(numberOfEntities);
    //ret = query.exec(queryString);
    //query.next();
    //currentEntity = (query.value(0)).toInt();


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
        prefix = list[7];
        currentPrefix = prefix;

        queryString = "SELECT id FROM continent WHERE shortname=='" + continentName + "'";        
        query1.exec(queryString);
        query1.next();
        if (query1.isValid())
        {
            continentId = (query1.value(0)).toInt();
        }
        //qDebug() << "World::processLine Query: " <<  queryString << endl;


        //qDebug() << "World::processLine Query - Read/continenId: " << continentName << "/" << QString::number(continentId) << endl;

        //queryString = QString("INSERT INTO entity (id, name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix) VALUES (NULL,'%1','%2','%3','%4','%5','%6','%7','%8','%9');\n").arg(entityName).arg(cqz).arg(ituz).arg(QString::number(continentId)).arg(lat).arg(lon).arg(utc).arg(numberOfEntities).arg(prefix);
        aa << entityName << QString::number(cqz) << QString::number(ituz) <<QString::number(continentId) << QString::number(lat) << QString::number(lon) << QString::number(utc) << QString::number(numberOfEntities) << prefix;
        //qDebug() << "World::processLine Query: " << queryString << endl;
        return aa;
        //ret = query.exec(queryString);

        progressBarPosition++;
    }
    aa.clear();
    return aa;
}




QStringList World::processLineP(const QString _line, const int _processingEntity){
    //Returns QStringList: prefix << dxcc << cqz << ituz OR CurrentEntity as a number

    //qDebug() << "World::processLineP: Entity/received: " << QString::number(_processingEntity) << "/" << _line << endl;
    QString queryString;
    QString line;
    int currentEntity = _processingEntity;
    int _cqz, _ituz;
    _cqz = 0;
    _ituz = 0;
    line = (_line).simplified();


    if ((line).count('\'')) // Replaces ' by _
    //The error comes from "Cote d'Ivoire" that breaks the SQL
    {
        line.replace(QChar('\''), QChar('_'));
    }

    //qDebug() << "World::processLineP: Received: " << line << endl;
    QSqlQuery _queryp;
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

        queryString = QString("SELECT cqz FROM entity WHERE dxcc='%1'").arg(currentEntity);
        ret = _queryp.exec(queryString);
        _queryp.next();
        if (_queryp.isValid())
        {
            _cqz = (_queryp.value(0)).toInt();
        }

        queryString = QString("SELECT ituz FROM entity WHERE dxcc='%1'").arg(currentEntity);
        ret = _queryp.exec(queryString);
        _queryp.next();
        if (_queryp.isValid())
        {
            _ituz = (_queryp.value(0)).toInt();
        }

        line = line.remove(';');

        if (line.count(',') == 0) // Only one prefix in the final line
        {
            prefixAndZones = readZones(line, _cqz, _ituz);
            //aa.clear();
            aa << prefixAndZones.at(0) << QString::number(currentEntity) << prefixAndZones.at(1) << prefixAndZones.at(2);

            //qDebug() << "World::processLineP Query (only one final): " << queryString << endl;
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

        queryString = QString("SELECT cqz FROM entity WHERE dxcc='%1'").arg(currentEntity);
        ret = _queryp.exec(queryString);
        _queryp.next();
        if (_queryp.isValid())
        {
            _cqz = (_queryp.value(0)).toInt();
        }
        queryString = QString("SELECT ituz FROM entity WHERE dxcc='%1'").arg(currentEntity);
        ret = _queryp.exec(queryString);
        _queryp.next();
        if (_queryp.isValid())
        {
            _ituz = (_queryp.value(0)).toInt();
        }
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
            //qDebug() << "World::processLine Query (only one final): " << queryString << endl;

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

                //queryString = queryString + QString("INSERT INTO prefixesofentity (prefix, dxcc, cqz, ituz) VALUES ('%1','%2','%3','%4');\n").arg(prefixAndZones.at(0)).arg(currentEntity).arg((prefixAndZones.at(1)).toInt()).arg((prefixAndZones.at(2)).toInt());
                aa << prefixAndZones.at(0) << QString::number(currentEntity) << prefixAndZones.at(1) << prefixAndZones.at(2);

                //ret = query.exec(queryString);

            }

            //qDebug() << "World::processLine Query: " << queryString << endl;

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
        azone = (aux.midRef(aux.indexOf('[')+1)).toString();
        //qDebug() << "World::readZones (ITU)-1: " << aux << " right of " << QString::number(aux.indexOf('[')) << " = " << azone << endl;
        itu = (azone.left(azone.indexOf(']'))).toInt();
        //qDebug() << "World::readZones (ITU)-2: " << azone.left(azone.indexOf(']')) << endl;
        aux = aux.left(aux.indexOf('['));
        //qDebug() << "World::readZones (ITU): "  << pref << "/" << QString::number(itu) << "/" << aux << endl;
    }

    if(aux.count('(')==1) // Check if has special CQz
    {
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

int World::getPrefixId(const QString _qrz)
{
    //qDebug() << "World::getPrefixId: " << _qrz << endl;
    //TODO: Instead of going from long to short, identify prefixes from the begining:
    // character(may be number) + number
    int errorCode;

    if (_qrz.length() < 1)
    {
        return -1;
    }
    int entityID = 0;
    int iaux1, iaux2;

    QString prefixl, prefixr;
    QString queryString;
    QSqlQuery query;

    QString aux = changeSlashAndFindPrefix((_qrz).toUpper());

    query.prepare("SELECT dxcc FROM prefixesofentity WHERE prefix==:prefix");

    QSqlDatabase::database().transaction();

    while ((entityID <= 0) && (aux.length()>=1) )
    {
        //qDebug() << "World::getPrefixId: Looking for the prefix in (aux): " << aux << endl;


        query.bindValue(":prefix", aux);
        if (query.exec())
        {}
        else
        {
             errorCode = query.lastError().number();
             //qDebug() << "World::getPrefixId - query error: " << QString::number(errorCode) << endl;

        }

        query.next();
        if (query.isValid())
        {
            entityID = (query.value(0)).toInt();
            //qDebug() << "World::getPrefixId - Found Entity: " << QString::number(entityID) << endl;
        }
        else
        {

/*

                //qDebug() << "FileManager::adifReadLog: LastQuery: " << preparedQuery.lastQuery()  << endl;
                //qDebug() << "FileManager::adifReadLog: LastError-data: " << preparedQuery.lastError().databaseText()  << endl;
                //qDebug() << "FileManager::adifReadLog: LastError-driver: " << preparedQuery.lastError().driverText()  << endl;
                //qDebug() << "FileManager::adifReadLog: LastError-n: " << QString::number(preparedQuery.lastError().number() ) << endl;

             //qDebug() << "World::getPrefixId - value on query not valid"  << endl;
             */
            entityID = -1;
            aux.chop(1);
        }
        //qDebug() << "World::getPrefixId: in the while" << aux << " = " <<  QString::number(entityID) << endl;
    }

    QSqlDatabase::database().commit();
    //qDebug() << "World::getPrefixId: " <<  _qrz << QString::number(entityID) << endl;
    return entityID;
}

QString World::getQRZEntityName(const QString _qrz)
{
    //qDebug() << "World::getQRZEntityName: " << _qrz << endl;
    if (_qrz.length() < 1 )
    {
        return "";
    }
    QString queryString;
    QSqlQuery query;
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
        return "";
    }

    QString queryString;
    QSqlQuery query;

    queryString = "SELECT name FROM entity WHERE dxcc=='" + QString::number(prefixIDNumber) +"'";
    //qDebug() << "World::getEntityNamee: queryString-2: " << queryString << endl;
    query.exec(queryString);
    query.next();
    //qDebug() << "World::getEntityName end: " << _qrz << " = " << (query.value(0)).toString() << endl;

    if (query.isValid()){
        return (query.value(0)).toString();
    }else{
        return "";
    }

    return "";

}

int World::getQRZCqz(const QString _qrz)
{
    //qDebug() << "World::getQRZCqz: " << _qrz << endl;
    if (_qrz.length() < 1 )
    {
        return -1;
    }

    int prefixIdNumber = getPrefixId(_qrz);
    return getEntityCqz(prefixIdNumber);

}

int World::getQRZItuz(const QString _qrz)
{
    //qDebug() << "World::getQRZItuz: " << _qrz << endl;
    if (_qrz.length() < 1 )
    {
        return -1;
    }

    int prefixIdNumber = getPrefixId(_qrz);
    return getEntityItuz(prefixIdNumber);

}

int World::getEntityCqz(const int _enti)
{
    if (_enti < 1 )
    {
        return -1;
    }

    QSqlQuery query;
    QString queryString;

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

}

int World::getEntityItuz(const int _enti)
{
    if (_enti < 1 )
    {
        return -1;
    }

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
}

int World::getQRZARRLId(const QString _qrz)
{
    //qDebug() << "World::getQRZARRLId" << _qrz << endl;

    if (_qrz.length() < 1 )
    {
        return -1;
    }

    QSqlQuery query;
    QString queryString;

    int prefixIdNumber = getPrefixId(_qrz);
    return prefixIdNumber;
/*
    queryString = "SELECT dxcc FROM prefixesofentity WHERE id='" + QString::number(prefixIdNumber) +"'";
    //qDebug() << "World::getQRZARRLId QUERY: " << queryString << endl;

    if (!query.exec(queryString))
    {
        //qDebug() << "World::getQRZARRLId: 0"  << endl;
        return -1;
    }
    else
    {
        query.next();
        //qDebug() << "World::getQRZARRLId: " <<_qrz << " = " <<  (query.value(0)).toInt() << endl;
        if (query.isValid()){
            //qDebug() << "World::getQRZARRLId: 1 (ARRLid found!)"  << endl;
            return (query.value(0)).toInt();
        }else{
            //qDebug() << "World::getQRZARRLId: 2 (not found!)"  << endl;
            return -1;
        }
    }

    //qDebug() << "World::getQRZARRLId: 3"  << endl;
    return -1;
*/
}

QString World::getQRZEntityMainPrefix(const QString _qrz)
{
    if (_qrz.length() < 1 )
    {
        return "";
    }

    QString queryString;
    QSqlQuery query;
    int i = getQRZARRLId(_qrz);
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
}

QString World::getEntityMainPrefix(const int _entityN)
{
    if (_entityN <= 0 )
    {
        return "";
    }

    QString queryString;
    QSqlQuery query;
    queryString = "SELECT mainprefix FROM entity WHERE dxcc=='" + QString::number(_entityN) +"'";

    if (!query.exec(queryString))
    {
        return "";
    }
    else
    {
        query.next();
        //qDebug() << "World::getQRZEntityMainPrefix(int): " <<_qrz << " = " <<  (query.value(0)).toInt() << endl;

        if (query.isValid()){
            return (query.value(0)).toString();
        }else{
            return "";
        }
    }
    return "";
}


bool World::isNewCQz(const int _cqz)
{
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
}

bool World::isNewEntity(const int _entityN)
{
    if (_entityN <= 0)
    {
        return false;
    }

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

}

QString World::getQRZContinentShortName(const QString _qrz)
{
    //qDebug() << "World::getQRZContinentShortName: " << _qrz << endl;
    //QString continentNumber = getQRZContinentNumber (_qrz);
    return getContinentShortName (getQRZARRLId(_qrz));

}


QString World::getContinentShortName(const int _enti)
{
    //qDebug() << "World::getQRZContinentShortName: " << _qrz << endl;

    QString continentNumber = QString::number(_enti);

    if ( _enti < 0 )
    {
        return "--";
    }

    QString queryString;
    QSqlQuery query;
    queryString = "SELECT continent FROM entity WHERE dxcc=='" + continentNumber +"'";

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
}

QString World::getQRZContinentNumber(const QString _qrz)
{
    //qDebug() << "World::getQRZContinentNumber: " << _qrz << endl;

    QString a;
    //if (_qrz.length()<3) // CALLS MAY BE ONLY K
    //{
    //    return "-1";
    //}
    QString queryString;
    QSqlQuery query;
    int i = getQRZARRLId(_qrz);
    queryString = "SELECT continent FROM entity WHERE dxcc=='" + QString::number(i) +"'";


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

}

int World::getContinentNumber(const int _enti)
{
    //qDebug() << "World::getQRZContinentNumber: " << QString::number(_enti) << endl;
    if (_enti <= 0)
    {
        return -1;
    }
    int a = -1;

    QString queryString;
    QSqlQuery query;
    int i = _enti;
    queryString = "SELECT continent FROM entity WHERE dxcc=='" + QString::number(i) +"'";


    if (!query.exec(queryString))
    {
        return -1;
    }
    else
    {
        query.next();
        if ( !(query.isValid()) ) {           
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

}

double World::getQRZLongitude(const QString _qrz)
{
    QString queryString;
    QSqlQuery query;
    int i = getQRZARRLId(_qrz);
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
}

double World::getLongitude(const int _enti)
{
    if (_enti <= 0)
    {
        return 0.0;
    }
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
}

double World::getQRZLatitude(const QString _qrz)
{


    QString queryString;
    QSqlQuery query;
    int i = getQRZARRLId(_qrz);
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

}

double World::getLatitude(const int _enti)
{
    if (_enti <= 0)
    {
        return 0.0;
    }

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

}

QString World::getQRZLocator(const QString _qrz)
{
    if (_qrz.length() < 1)
    {
        return "";
    }
    return locator->getLocator(getQRZLongitude(_qrz), getQRZLatitude(_qrz));
}

QString World::getLocator(const int _enti)
{
    if (_enti <= 0)
    {
        return "";
    }
    return locator->getLocator(getLongitude(_enti), getLatitude(_enti));
}


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



QString World::getQRZEntityPrefixes(const QString _qrz)
{
    QString result;
    result = "";
    QString queryString;
    QSqlQuery query;
    int i = getQRZARRLId(_qrz);
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
}

QString World::getEntityPrefixes(const int _enti)
{
    if (_enti<=0)
    {
        return "";

    }
    QString result;
    result = "";
    QString queryString;
    QSqlQuery query;
    int i = _enti;
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
}


bool World::readCTYDAT()
{
    //qDebug() << "World::readCTYDAT(): " << kontestDir << endl;
    QString tq;
    QString fileName;
    qint64 beginingOfFile;
    int numberOfLines = 0;
    int errorCode = -1;
    //int ii = 0;
    //bool ret;

    #ifdef Q_OS_WIN
        //qDebug() << "WINDOWS DETECTED!"  << endl;
        fileName = kontestDir + "/cty.dat";
    #else
        //qDebug() << "NO WINDOWS"  << endl;
        fileName = kontestDir + "/cty.dat";
    #endif



    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "World::readCTYDAT() File not found: " << fileName << endl;
        return false;
    }
    else
    {
       // //qDebug() << "World::readCTYDAT() File found: " << fileName << endl;
    }
    beginingOfFile = file.pos();

    while (!file.atEnd()) {
        if ( (file.readLine()).contains(':')  )
        {
            numberOfEntities++;
        }
        numberOfLines++;
    }
    //qDebug() << "World::readCTYDAT() - numberOfEntities: " << QString::number(numberOfEntities) << endl;

    // The file is readed twice: 1: Main entity data; 2: prefixes.


    // Starts with main data:
    file.seek(beginingOfFile);
    progressBarPosition = 0;
    QProgressDialog progress(tr("Reading cty.dat..."), tr("Abort reading"), 0, numberOfLines, this);
    progress.setWindowModality(Qt::ApplicationModal);

    numberOfEntities = 0; // Reset this variable to reuse it and assign the "dxcc" to the entities (temp solution)

    QSqlQuery query;
    query.prepare("INSERT INTO entity (id, name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix)"
                "VALUES (?, ?,?, ?, ?, ?, ?, ?, ?, ?)");
    QSqlDatabase::database().transaction();

    QStringList stringList;



    while (!file.atEnd()) {
        progress.setValue(progressBarPosition);
        progressBarPosition++;
        if (progress.wasCanceled())
            break;
        stringList.clear();
        stringList << processLine(file.readLine());

        if (stringList.count()==9 )
        {

            query.addBindValue(QVariant(QVariant::Int));
            query.addBindValue(stringList.at(0));
            query.addBindValue(stringList.at(1));
            query.addBindValue(stringList.at(2));
            query.addBindValue(stringList.at(3));
            query.addBindValue(stringList.at(4));
            query.addBindValue(stringList.at(5));
            query.addBindValue(stringList.at(6));
            query.addBindValue(stringList.at(7));
            query.addBindValue(stringList.at(8));

            if (query.exec())
            {
                 //qDebug()  << "World::readCTYDAT(): Entity data added"  << endl;
            }
            else if (errorCode == 19)
            {

            }
            else
            {
                errorCode = query.lastError().number();

/*
                //qDebug()  << "World::readCTYDAT(): Entity data NOT added"  << endl;
                //qDebug() << "World::readCTYDAT(): LastQuery: " << query.lastQuery()  << endl;
                //qDebug() << "World::readCTYDAT(): LastError-data: " << query.lastError().databaseText()  << endl;
                //qDebug() << "World::readCTYDAT(): LastError-driver: " << query.lastError().driverText()  << endl;
                //qDebug() << "World::readCTYDAT(): LastError-n: " << QString::number(query.lastError().number() ) << endl;
*/
            }
            // To remove all the data before the next entity addition.
            query.bindValue( ":name", "" );
            query.bindValue( ":cqz", "" );
            query.bindValue( ":ituz", "" );
            query.bindValue( ":continent", "" );
            query.bindValue( ":latitude", "" );
            query.bindValue( ":longitude", "" );
            query.bindValue( ":utc", "" );
            query.bindValue( ":dxcc", "-1" );
            query.bindValue( ":mainprefix", "" );

         }


        //qDebug()  << "World::readCTYDAT() tq: " << tq << endl;
        progress.setLabelText("Reading cty.dat ... \nNow reading " + currentPrefix + " data");
        //qDebug() << "World::readCTYDAT() - progressBarPosition: " << QString::number(progressBarPosition) << endl;
    }



    QSqlDatabase::database().commit();

    query.prepare("INSERT INTO prefixesofentity (id, prefix, dxcc, cqz, ituz) VALUES (?, ?, ?, ?, ?)");

    QSqlDatabase::database().transaction();

    // Finish the main data
    // Start with prefixes:

     file.seek(beginingOfFile);
     int k;
     int cEntity=0; // Controls the current entity that is being processed
     progressBarPosition = 0;

     while (!file.atEnd()) {
         progress.setValue(progressBarPosition);
         progressBarPosition++;
         if (progress.wasCanceled())
             break;
         stringList.clear();
         stringList << processLineP(file.readLine(), cEntity);

        if (stringList.size()==1){
            cEntity = (stringList.at(0)).toInt();
        }else{

        }

        k = 0;



        for (int i = 0; i < (stringList.size() / 4); i++){

            query.addBindValue(QVariant(QVariant::Int));

            currentPrefix = stringList.at(k);
            for (int j = 0; j <= 3; j++)
            {
                //qDebug() << "World::readCTYDAT(): FOR-4" << endl;
                query.addBindValue(stringList.at(k));

                k++;
                //qDebug() << "World::readCTYDAT(): stringList.at(" << QString::number(i) << "): " << stringList.at(i) << endl;
            }

            query.exec();

        }

        //currentPrefix = getQRZEntityMainPrefix(cEntity);
        progress.setLabelText("Reading cty.dat ... \nNow reading " + currentPrefix + " data");


    }
    QSqlDatabase::database().commit();

    progress.setValue(numberOfLines);
    return true;
}

bool World::readCTYCSV()
{
    //qDebug() << "World::readCTYCSV(): " << kontestDir << endl;

    QString tq;
    QString fileName;
    qint64 beginingOfFile;
    int numberOfLines = 0;
    int errorCode = -1;
    //int ii = 0;
    //bool ret;

    #ifdef Q_OS_WIN
        //qDebug() << "WINDOWS DETECTED!"  << endl;
        fileName = kontestDir + "/cty.csv";
    #else
        //qDebug() << "NO WINDOWS"  << endl;
        fileName = kontestDir + "/cty.csv";
    #endif



    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "World::readCTYCSV() File not found: " << fileName << endl;
        return false;
    }
    else
    {
       // //qDebug() << "World::readCTYCSV() File found: " << fileName << endl;
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

    QSqlQuery query;
    QSqlQuery queryP;

    query.prepare("INSERT INTO entity (id, name, cqz, ituz, continent, latitude, longitude, utc, dxcc, mainprefix)"
                "VALUES (?, ?,?, ?, ?, ?, ?, ?, ?, ?)");
    queryP.prepare("INSERT INTO prefixesofentity (id, prefix, dxcc, cqz, ituz) VALUES (?, ?, ?, ?, ?)");

    QSqlDatabase::database().transaction();

    QStringList stringList, stringListPrefixes;



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

        tq = file.readLine().simplified();
        tq = tq.trimmed();
        tq.remove(QChar(';'), Qt::CaseInsensitive);

        stringList << tq.split(',');

        if (stringList.count()>=8 )
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
            query.addBindValue(stringList.at(2)); // dxcc
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
                 //qDebug()  << "World::readCTYDAT(): Entity data added"  << endl;
            }
            else if (errorCode == 19)
            {}
            else
            {
                errorCode = query.lastError().number();


                //qDebug()  << "World::readCTYCSV(): Entity data NOT added"  << endl;
                //qDebug() << "World::readCTYCSV(): LastQuery: " << query.lastQuery()  << endl;
                //qDebug() << "World::readCTYCSV(): LastError-data: " << query.lastError().databaseText()  << endl;
                //qDebug() << "World::readCTYCSV(): LastError-driver: " << query.lastError().driverText()  << endl;
                //qDebug() << "World::readCTYCSV(): LastError-n: " << QString::number(query.lastError().number() ) << endl;

            }

            if (stringList.count()>8)
            {
                tq = stringList.at(9);
                stringListPrefixes << tq.split(' ');
                tq = stringListPrefixes.last();
                QString prefAux = QString();

                for (int i = 0; i < stringListPrefixes.size(); ++i)
                {
                    prefAux = stringListPrefixes.at(i);
                 //queryP.prepare("INSERT INTO prefixesofentity (id, prefix, dxcc, cqz, ituz) VALUES (?, ?, ?, ?, ?)");
                    //                                            0    1       2    3    4
                    //  (id, prefix, dxcc, cqz, ituz)
                    //qDebug()  << "World::readCTYCSV(): Prefix: " << stringListPrefixes.at(i) << endl;
                    queryP.addBindValue(QVariant(QVariant::Int));

                    if (prefAux.at(0)=='=')
                    { //TODO: Maybe there is a better way to identify exact calls instead of prefixes , identified with a = before he call.
                        prefAux.remove(0,1);
                    }

                    queryP.addBindValue(prefAux);
                    queryP.addBindValue(stringList.at(2));
                    queryP.addBindValue(stringList.at(4));
                    queryP.addBindValue(stringList.at(5));

                    if (queryP.exec())
                    {
                         //qDebug()  << "World::readCTYCSV(): Prefix added: " << stringListPrefixes.at(i) << endl;
                    }
                    else if (errorCode == 19)
                    {}
                    else
                    {
                        errorCode = query.lastError().number();

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
    return true;

}

QString World::changeSlashAndFindPrefix(const QString _qrz)
{
    int iaux1, iaux2;

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

        if ( iaux1 < iaux2 ) { //Like in F/EA4TV, we can simply take the first part as the prefix
            aux = aux.left(iaux1);
        }
        else
        {
            aux = aux.right(iaux2 -1);
        }
    }
    return aux;
}


bool World::checkQRZValidFormat(const QString _qrz)
{
    //qDebug()  << "World::checkQRZValidFormat: " << _qrz << endl;

    QString aux = changeSlashAndFindPrefix(_qrz.toUpper());

    if (aux.length()<3)
    {
        return false;
    }

    // The format of a Call is defined here:
    // http://en.wikipedia.org/wiki/ITU_prefix_%28amateur_stations%29
    //bool prefixEnded = false;
    //int callLength = _qrz.length();

    int sepPos = 0;


    int barPos = aux.indexOf('/');

    QString prefix = QString();
    QString suffix = QString();
    QString separator = QString();
    // If barPos > 0 we have a complex call like F/EA4TV

    QCharRef c = aux[aux.length()-1];

    if ( c.isDigit() && (barPos<=0)  )
    {
        return false;
    }

    prefix = prefix + aux.at(0);

    if ( ( aux.at(1) ).isDigit() )
    { // W3 A6
        if ((aux.at(2)).isDigit() )
        { // A60
            prefix = prefix + aux.at(1);
            separator = aux.at(2);
            sepPos = 2;
        }
        else
        { // W3A
            separator = aux.at(1);
            sepPos = 1;
        }
    }
    else
    { // EA 3D 2M

        if ((aux.at(2)).isDigit() )
        { // EA0

            prefix = prefix + aux.at(1);
            separator = aux.at(2);
            sepPos = 2;

        }
        else
        { // 3DA
            if (aux.length() < 4)
            {
                prefix = prefix + aux.at(2);
                return false;
            }

            if ((aux.at(3)).isDigit())
            { // 3DA0
                prefix = prefix + aux.at(1);
                prefix = prefix + aux.at(2);
                separator = aux.at(3);
                sepPos = 3;
                if (aux.length() < 5)
                { // 3DA0
                    prefix = prefix + aux.at(3);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }

    for (int i = sepPos+1; i < aux.length(); i++)
    {
        suffix = suffix + aux.at(i);
    }
    //qDebug()  << "World::checkQRZValidFormat: Prefix = " << prefix << endl;
    //qDebug()  << "World::checkQRZValidFormat: Separator = " << separator << endl;
    //qDebug()  << "World::checkQRZValidFormat: Suffix = " << suffix << endl;
    //qDebug()  << "World::checkQRZValidFormat: Call = " << prefix+separator+suffix << endl;
    return true;
}

QStringList World::getEntitiesNames()
{
    //qDebug()  << "World::getEntitiesNames"  << endl;
    QString aux = QString();
    QStringList qs;
    qs.clear();
    QString stringQuery = QString("SELECT mainprefix, name FROM entity");
    QSqlQuery query;

    if (query.exec(stringQuery))
    {
        while ( (query.next())) {
            if (query.isValid())
            {
                aux.clear();
                aux = (query.value(0)).toString() + "-" + (query.value(1)).toString();
                //result = result + ", " + (query.value(0)).toString();
                qs << aux;
            }
            else
            {
            }
        }

    }
    else
    {

    }
    return qs;
}

