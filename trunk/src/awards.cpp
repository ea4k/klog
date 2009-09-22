/***************************************************************************
                          awards.c  -  description
                             -------------------
    begin                : mar ago 27 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/
/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
 *                                                                            *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the            *
* GNU General Public License for more details.                             *
 *                                                                            *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the Free Software              *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA02111-1307USA*
 *****************************************************************************/

//#include <klocale.h>
//#include "iostream"
//using namespace std;

#include "awards.h"

Awards::Awards(){
    listOfAwards.clear();
    awardReference = 0;
    awardDefault.setAwardReference(-1);

}

Awards::~Awards(){}

void Awards::readConfig(){
//cout << "Awards::readConfig() " << endl;
    clearAwards();
    klogDir = QDir::homePath()+"/.klog";  // We create the ~/.klog for
                        // the logs and config file
    if (!QDir::setCurrent ( klogDir )){
        QDir d1(klogDir);
        dirExist = d1.mkdir(klogDir);
    }
    QFile file( "klogrc" );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &file );
        QString data = "";
        data = ""; //
        QString adifTab;
        QString theData;
        while (!stream.atEnd()){
            data = stream.readLine();
            if(!data.isEmpty()){ // If there is another line i read it
                data = data.simplified();
                QStringList fields = data.split("=");
                //QStringList fields = QStringList::split('=', data );
                adifTab = fields[0].toUpper();
                theData = fields[1];
                if (adifTab == "AWARD"){
                    awardAux.clearAward();
                    awardFile = theData;
                    awardAux.readAward(awardFile);
                    awardAux.setAwardReference(awardReference);
                    listOfAwards.append(awardAux);
                    awardReference++;
                }else{
                }
            }//Closes the if != empty
        }// Closes the while
        file.close();
    }else{
    //	//cout << "AWARDS: readConfig -No file opened" << endl;
    }
}


int Awards::checkPrefix(const QString &tprefix){
//cout << "Awards::checkPrefix()  " << endl;
    ListOfAwards::iterator it;
    for ( it = listOfAwards.begin(); it != listOfAwards.end(); ++it ){
        if ( ((*it).getEntityPrefix()).count(tprefix) != 0){
            return (*it).getAwardReference();
        }
    }
    return -1;
}

QString Awards::getAwardName (const QString &tprefix){
//cout << "Awards::getAwardName() " << endl;
    ListOfAwards::iterator it;
    for ( it = listOfAwards.begin(); it != listOfAwards.end(); ++it ){
        if (((*it).getEntityPrefix()).count(tprefix) != 0){
            return (*it).getAwardName();
        }
    }
    return awardDefault.getAwardName();
}

Award Awards::getAwardFor(const QString &tprefix){
//cout << "Awards::getAwardFor() " << tprefix << endl;
    awardAux = awardDefault;
    ListOfAwards::iterator it;
    for ( it = listOfAwards.begin(); it != listOfAwards.end(); ++it ){
        if ((*it).checkPrefix(tprefix)){
//cout << "Awards::getAwardFor() " << "Found award: " << (*it).getAwardName() << endl;
            return (*it);
        }
    }
//cout << "Awards::getAwardFor() " << "No award found for this prefix" << endl;
    return awardDefault; // If we did not found the award for this prefix we simply return

}



Award Awards::getDefaultAward(){
//cout << "Awards::getDefaultAward() " << endl;
    return awardDefault;
};

QStringList Awards::getAwardReferences(Award &taward){
//cout << "Awards::getAwardReferences() " << endl;
    return taward.getReferences();
}


void Awards::clearAwards(){
    listOfAwards.clear();
    awardReference = 0;
    awardDefault.setAwardReference(-1);
}

void Awards::deleteAward(Award &taward){
    ListOfAwards::iterator it;
    for ( it = listOfAwards.begin(); it != listOfAwards.end(); ++it ){
        if (((*it).getAwardName())==(taward.getAwardName())){
            listOfAwards.erase(it);
        }
    }
}
