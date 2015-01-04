/***************************************************************************
                          award  -  description
                             -------------------
    begin                : vie may 20 2005
    copyright            : (C) 2005 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
 *                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/

#include <QDebug>
//#include <klocale.h>

#include "award.h"

Award::Award(){
//qDebug()   << "Award::Constructor-Default " << endl;
    number = 0;
    awardReference = -1;
    numberOfReferences = -1;
    entityPrefix = "";
    awardName = "DEFAULT";
    sponsor = "KLOG-DEFAULT";
    references.clear();
}

Award::Award(const QString &tfile){
//qDebug()    << "Award::Constructor-2 " << endl;
    number = 0;
    awardReference = -1;
    numberOfReferences = -1;
    entityPrefix = "";
    awardName = "DEFAULT";
    sponsor = "KLOG-DEFAULT";
    references.clear();
    readAward(tfile);
}

Award::~Award(){}

void Award::setAwardReference(int treference){
//qDebug() << "Award::setAwardReference " << endl;
    awardReference = treference;
}

int Award::getAwardReference() const{
//qDebug() << "Award::getAwardReference " << endl;
    return awardReference;
}

void Award::setNumberOfReferences(int treference){
//qDebug() << "Award::setNumberOfReferences " << endl;
    numberOfReferences = treference;
}

int Award::getNumberOfReferences() const{
//qDebug() << "Award::getNumberOfReferences " << endl;
    return numberOfReferences;
}

void Award::setAwardName(const QString &tname){
//qDebug() << "Award::setAwardname " << endl;
    awardName = tname;
}

QString Award::getAwardName() const{
//qDebug() << "Award::getAwardName " << awardName << endl;
    return awardName;
}

void Award::setEntityPrefix(const QString &tprefix){
//qDebug() << "Award::setEntityPrefix " << endl;
    prefixes.append(tprefix);
}

bool Award::checkPrefix(const QString &prefix){
//qDebug() << "Award::checkPrefix " << endl;
    Prefixes::iterator it;
    for ( it = prefixes.begin(); it != prefixes.end(); ++it ){
//		if ((getEntityPrefix()).count(prefix) != 0){
        if ((getEntityPrefix()).count(prefix) == true){
            return true;
        }
    }
    return false;
}

QStringList Award::getEntityPrefix() {
//qDebug() << "Award::getEntityPrefix " << endl;
    QStringList listOfPrefixes;
    Prefixes::iterator it;
    listOfPrefixes.clear();
    for ( it = prefixes.begin(); it != prefixes.end(); ++it ){
        listOfPrefixes.append(*it);
    }
    return listOfPrefixes;
}


void Award::setOrganizer(const QString &tsponsor){
//qDebug() << "Award::setOrganizer " << endl;
    sponsor = tsponsor;
}

QString Award::getOrganizer() const{
//qDebug() << "Award::getOrganizer " << endl;
    return sponsor;
}


void Award::setAwardVersion(const QString &tversion){
//qDebug() << "Award::setAwardVersion " << endl;
    version = tversion;
}

QString Award::getAwardVersion() const{
//qDebug() << "Award::getAwardVersion " << endl;
    return version;
};

QString Award::getReference(int treference){
//qDebug() << "Award::getReference " << endl;
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
        if (treference == (*it).getNumber() ){
            return (*it).getReference();
        }
    }
    return "NULL-REFERENCE";
}

void Award::readAward(const QString &tfile){
//qDebug() << "Award::readAward " << endl;
    QFile file( tfile );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString line;
        QString adifTab;
        QString theData;
        int len; // for the tab length
        QString data = "";
        i = 0;
        while ( !stream.atEnd()  && (i == 0)) {
            data = stream.readLine();
            if(!data.isEmpty()){
            data = data.simplified().toUpper();
            aux = data.count('<');
            //aux = data.count('<');
            fields = data.split("<", QString::SkipEmptyParts);
            //fields = QStringList::split("<", data );
            for (int a = 0; a < aux; a++){ // It was to aux-
                    //if (fields[a].count(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
                    if (fields[a].count(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
                    adifTab = fields[a].section(':',0,0);
                    if (adifTab == "EOH>"){
                        i = 1;
                    }else{
                    len = (fields[a].section(':',1,1)).toInt();
                    theData = ((fields[a]).section('>',1));
                    theData.truncate(len);
                    }

                }else{
                    adifTab = fields[a].section(':',0,0);
                    if (adifTab == "EOH>"){
                        i = 1;
                    }else{
                    len = (fields[a].section(':',1)).section('>',0,0).toInt();
                    theData = ((fields[a]).section('>',1));
                    //theData = ((fields[a].section(':',1)).section('>',1));
                    theData.truncate(len);
                    }
                }

                    if (adifTab == "AWARD_NAME"){
                        awardName = theData.toUpper();
                //qDebug() << "AWARD reading the name: " << awardName << endl;
                    }else if (adifTab == "AWARD_REFERENCES"){
                        numberOfReferences = theData.toInt();
                    }else if (adifTab == "AWARD_VERSION"){
                        version = theData.toUpper();
                    }else if (adifTab == "AWARD_ENTITY"){
                        setEntityPrefix(theData.toUpper());
                        //entityPrefix = theData.toUpper();
                    }else if (adifTab == "AWARD_SPONSOR"){
                        sponsor = theData.toUpper();
                    }else if (adifTab == "EOH>"){
                        i = 1;
                    }
            }
            }
        }
    // The Header reading ends here

    data = ""; //
    ref.clear();
    references.append(ref);
    while (!stream.atEnd()){
        data = stream.readLine();
        if(!data.isEmpty()){
            data = data.simplified().toUpper();
            //aux = data.count('<');
            aux = data.count('<');
            fields = data.split("<", QString::SkipEmptyParts);
            //fields = QStringList::split('<', data );
            for (int a = 0; a < aux; a++){ // It was to aux-
                    //if (fields[a].count(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
                    if (fields[a].count(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
                    adifTab = fields[a].section(':',0,0);
                    len = (fields[a].section(':',1,1)).toInt();
                    theData = ((fields[a]).section('>',1));
                    theData.truncate(len);
                }else{
                    adifTab = fields[a].section(':',0,0);
                    len = (fields[a].section(':',1)).section('>',0,0).toInt();
                    theData = ((fields[a]).section('>',1));
                    //theData = ((fields[a].section(':',1)).section('>',1));
                    theData.truncate(len);
                }
                if (adifTab == "AWARD_ABBREV"){
                    ref.setReference(theData.toUpper());
                }else if (adifTab == "EOR>"){
                    references.append(ref);
                    ref.clear();
                }else if (adifTab == "AWARD_REFNAME"){
                    ref.setLongName(theData.toUpper());
                }else if (adifTab == "AWARD_REFNUMBER"){
                    ref.setNumber(theData.toInt());
                }
        }
    }
    }
    file.close();
    }

}

void Award::readAwardHeaders(const QString &tfile){
//qDebug() << "Award::readAwardHeaders " << endl;
QFile file( tfile );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString line;
        QString adifTab;
        QString theData;
        int len; // for the tab length
        QString data = "";
        i = 0;
        while ( !stream.atEnd()  && (i == 0)) {
            data = stream.readLine();
            if(!data.isEmpty()){
            data = data.simplified().toUpper();
//			aux = data.count('<');
            aux = data.count('<');
            fields = data.split("<", QString::SkipEmptyParts);
            //fields = QStringList::split('<', data );
            for (int a = 0; a < aux; a++){ // It was to aux-
//     				if (fields[a].count(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
                    if (fields[a].count(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
                    adifTab = fields[a].section(':',0,0);
                    if (adifTab == "EOH>"){
                        i = 1;
                    }else{
                    len = (fields[a].section(':',1,1)).toInt();
                    theData = ((fields[a]).section('>',1));
                    theData.truncate(len);
                    }

                }else{
                    adifTab = fields[a].section(':',0,0);
                    if (adifTab == "EOH>"){
                        i = 1;
                    }else{
                    len = (fields[a].section(':',1)).section('>',0,0).toInt();
                    theData = ((fields[a]).section('>',1));
                    //theData = ((fields[a].section(':',1)).section('>',1));
                    theData.truncate(len);
                    }
                }

                    if (adifTab == "AWARD_NAME"){
                        awardName = theData.toUpper();
                    }else if (adifTab == "AWARD_REFERENCES"){
                        numberOfReferences = theData.toInt();
                    }else if (adifTab == "AWARD_VERSION"){
                        version = theData.toUpper();
                    }else if (adifTab == "AWARD_ENTITY"){
                        setEntityPrefix(theData.toUpper());
                        //entityPrefix = theData.toUpper();
                    }else if (adifTab == "AWARD_SPONSOR"){
                        sponsor = theData.toUpper();
                    }else if (adifTab == "EOH>"){
                        i = 1;
                    }
            }
            }
        }
    file.close();
    }


}
void Award::clearAward(){
//qDebug() << "Award::clearAward " << endl;
    number = -1;
    awardReference = -1;
    numberOfReferences = -1;
    entityPrefix = "";
    awardName = "DEFAULT";
    sponsor = "KLOG-DEFAULT";
    references.clear();
    prefixes.clear();
}

QStringList Award::getReferences(){
//Returns the list of references of the Award
//qDebug() << "Award::gerReferences " << endl;
    listOfReferences.clear();
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
        listOfReferences.append((*it).getReference() + "-" + (*it).getLongName());
        }
    return listOfReferences;
}
/*
References Award::getAllReferences(){
    return references;
}
*/

int Award::getReferenceNumber(const QString &refName){
//qDebug() << "Award::getReferenceNumber " << endl;
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
         if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
            return (*it).getNumber();
        }
    return -1;

};

void Award::workReference(const QString &refName, const bool tworked){
    //qDebug() << "Award:workReference: " << endl;
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
         if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
            (*it).setWorked(tworked);
        }
}

void Award::confirmReference(const QString &refName, const bool tconfirmed){
    //qDebug() << "Award:confirmReference: " << endl;
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
         if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
            (*it).setConfirmed(tconfirmed);
        }
}

bool Award::isWorked(const QString &refName){
//qDebug() << "Award::isWorked " << endl;
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
         if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
            return (*it).isWorked();
        }
    return false;
}

bool Award::isConfirmed(const QString &refName){
//qDebug() << "Award::isConfirmed " << endl;
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
         if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
            return (*it).isConfirmed();
        }
    return false;
}

bool Award::isValidReference(const QString &refName){
// To know if a reference is a valid one or not.
//qDebug() << "Award::isValidReference:  " << refName << endl;
    References::iterator it;
    for ( it = references.begin(); it != references.end(); ++it ){
         if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
            return true;
        }
    return false;
}
