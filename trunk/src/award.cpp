/***************************************************************************
                          award  -  description
                             -------------------
    begin                : vie may 20 2005
    copyright            : (C) 2005 by Jaime Robles
    email                : jaime@robles.nu
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the Free Software              *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*
 *****************************************************************************/

//#include <kdebug.h>
#include <klocale.h>
//#include <ostream.h>
#include "award.h"

Award::Award(){
//cout << "Award::Constructor-Default " << endl;
	number = 0;
	awardReference = -1;
	numberOfReferences = -1;
	entityPrefix = "";
	awardName = "DEFAULT";
	sponsor = "KLOG-DEFAULT";
	references.clear();
}

Award::Award(const QString &tfile){
//cout << "Award::Constructor-2 " << endl;
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
//cout << "Award::setAwardReference " << endl;
	awardReference = treference;
}

int Award::getAwardReference() const{
//cout << "Award::getAwardReference " << endl;
	return awardReference;
}

void Award::setNumberOfReferences(int treference){
//cout << "Award::setNumberOfReferences " << endl;
	numberOfReferences = treference;
}

int Award::getNumberOfReferences() const{
//cout << "Award::getNumberOfReferences " << endl;
	return numberOfReferences;
}
	
void Award::setAwardName(const QString &tname){
//cout << "Award::setAwardname " << endl;
	awardName = tname;
}

QString Award::getAwardName() const{
//cout << "Award::getAwardName " << awardName << endl;
	return awardName;
}

void Award::setEntityPrefix(const QString &tprefix){
//cout << "Award::setEntityPrefix " << endl;
	prefixes.append(tprefix);
}

bool Award::checkPrefix(const QString &prefix){
//cout << "Award::checkPrefix " << endl;
	Prefixes::iterator it;
	for ( it = prefixes.begin(); it != prefixes.end(); ++it ){
		if ((getEntityPrefix()).contains(prefix) != 0){
			return true;
		}
	}
	return false;
}
	
QStringList Award::getEntityPrefix() {
//cout << "Award::getEntityPrefix " << endl;
	QStringList listOfPrefixes;
	Prefixes::iterator it;
	listOfPrefixes.clear();
	for ( it = prefixes.begin(); it != prefixes.end(); ++it ){
		listOfPrefixes.append(*it);
	}
	return listOfPrefixes;
}
	
	
void Award::setOrganizer(const QString &tsponsor){
//cout << "Award::setOrganizer " << endl;
	sponsor = tsponsor;
}

QString Award::getOrganizer() const{
//cout << "Award::getOrganizer " << endl;
	return sponsor;
}
	

void Award::setAwardVersion(const QString &tversion){
//cout << "Award::setAwardVersion " << endl;
	version = tversion;
}

QString Award::getAwardVersion() const{
//cout << "Award::getAwardVersion " << endl;
	return version;
};

QString Award::getReference(int treference){
//cout << "Award::getReference " << endl;
	References::iterator it;
	for ( it = references.begin(); it != references.end(); ++it ){
		if (treference == (*it).getNumber() ){
			return (*it).getReference();
		}
	}
	return "NULL-REFERENCE";
}

void Award::readAward(const QString &tfile){
//cout << "Award::readAward " << endl;
	QFile file( tfile );
	if ( file.open( IO_ReadOnly ) ) {
		QTextStream stream( &file );
		QString line;
		QString adifTab;
		QString theData;
		int len; // for the tab length
		QString data = "";
		i = 0;
		while ( !stream.eof()  && (i == 0)) {
			data = stream.readLine();
			if(!data.isEmpty()){
			data = data.simplifyWhiteSpace().upper();
			aux = data.contains('<');
			fields = QStringList::split('<', data );
			for (int a = 0; a < aux; a++){ // It was to aux-
     				if (fields[a].contains(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
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
						awardName = theData.upper();
						//cout << "AWARD leyendo nombre: " << awardName << endl;
					}else if (adifTab == "AWARD_REFERENCES"){
						numberOfReferences = theData.toInt();
					}else if (adifTab == "AWARD_VERSION"){
						version = theData.upper();
					}else if (adifTab == "AWARD_ENTITY"){
						setEntityPrefix(theData.upper());
						//entityPrefix = theData.upper();
					}else if (adifTab == "AWARD_SPONSOR"){
						sponsor = theData.upper();
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
	while (!stream.eof()){
		data = stream.readLine();
		if(!data.isEmpty()){
			data = data.simplifyWhiteSpace().upper();
			aux = data.contains('<');
			fields = QStringList::split('<', data );
			for (int a = 0; a < aux; a++){ // It was to aux-
     				if (fields[a].contains(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
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
					ref.setReference(theData.upper());
				}else if (adifTab == "EOR>"){
					references.append(ref);
					ref.clear();
				}else if (adifTab == "AWARD_REFNAME"){
					ref.setLongName(theData.upper());
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
////cout << "Award::readAwardHeaders " << endl;
QFile file( tfile );
	if ( file.open( IO_ReadOnly ) ) {
		QTextStream stream( &file );
		QString line;
		QString adifTab;
		QString theData;
		int len; // for the tab length
		QString data = "";
		i = 0;
		while ( !stream.eof()  && (i == 0)) {
			data = stream.readLine();
			if(!data.isEmpty()){
			data = data.simplifyWhiteSpace().upper();
			aux = data.contains('<');
			fields = QStringList::split('<', data );
			for (int a = 0; a < aux; a++){ // It was to aux-
     				if (fields[a].contains(':')>1){ //Checks for ADIF fields as <DATE:8:D>20050424
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
						awardName = theData.upper(); 
					}else if (adifTab == "AWARD_REFERENCES"){
						numberOfReferences = theData.toInt();
					}else if (adifTab == "AWARD_VERSION"){
						version = theData.upper();
					}else if (adifTab == "AWARD_ENTITY"){
						setEntityPrefix(theData.upper());
						//entityPrefix = theData.upper();
					}else if (adifTab == "AWARD_SPONSOR"){
						sponsor = theData.upper();
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
//cout << "Award::clearAward " << endl;
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
//cout << "Award::gerReferences " << endl;	
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
//cout << "Award::getReferenceNumber " << endl;
	References::iterator it;
	for ( it = references.begin(); it != references.end(); ++it ){
		 if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
			return (*it).getNumber();
		}
	return -1;

};

void Award::workReference(const QString &refName, const bool tworked){
	//cout << "Award:workReference: " << endl;
	References::iterator it;
	for ( it = references.begin(); it != references.end(); ++it ){
		 if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
			(*it).setWorked(tworked);
		}
}

void Award::confirmReference(const QString &refName, const bool tconfirmed){
	//cout << "Award:confirmReference: " << endl;
	References::iterator it;
	for ( it = references.begin(); it != references.end(); ++it ){
		 if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
			(*it).setConfirmed(tconfirmed);
		}
}

bool Award::isWorked(const QString &refName){
//cout << "Award::isWorked " << endl;
	References::iterator it;
	for ( it = references.begin(); it != references.end(); ++it ){
		 if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
			return (*it).isWorked();
		}
	return false;
}

bool Award::isConfirmed(const QString &refName){
//cout << "Award::isConfirmed " << endl;
	References::iterator it;
	for ( it = references.begin(); it != references.end(); ++it ){
		 if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
			return (*it).isConfirmed();
		}
	return false;
}

bool Award::isValidReference(const QString &refName){
// To know if a reference is a valid one or not.
//cout << "Award::isValidReference:  " << refName << endl;
	References::iterator it;
	for ( it = references.begin(); it != references.end(); ++it ){
		 if (((*it).getReference()+"-"+(*it).getLongName()) == refName)
			return true;
		}
	return false;
}
