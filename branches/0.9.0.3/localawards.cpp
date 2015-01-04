/***************************************************************************
                          localawards.c  -  description
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
//#include <QDebug>
//using namespace std;

#include "localawards.h"

LocalAwards::LocalAwards(){
    listOfLocalAwards.clear();
    awardReference = 0;
    awardDefault.setAwardReference(-1);
    awardAux.setAwardReference(-1);

}

LocalAwards::~LocalAwards(){}

void LocalAwards::readConfig(){
//cout << "LocalAwards::readConfig() " << endl;
    clearLocalAwards();
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
                    listOfLocalAwards.append(awardAux);
                    awardReference++;
                }else{
                }
            }//Closes the if != empty
        }// Closes the while
        file.close();
    }else{
    //	//cout << "LOCALAWARDS: readConfig -No file opened" << endl;
    }
}


int LocalAwards::checkPrefix(const QString &tprefix){
//qDebug()   << "LocalAwards::checkPrefix()  " << tprefix << endl;
    ListOfLocalAwards::iterator it;
    for ( it = listOfLocalAwards.begin(); it != listOfLocalAwards.end(); ++it ){
        if ( ((*it).getEntityPrefix()).count(tprefix) != 0){
            return (*it).getAwardReference();
        }
    }
    return -1;
}

QString LocalAwards::getAwardName (const QString &tprefix){
//cout << "LocalAwards::getAwardName() " << endl;
    ListOfLocalAwards::iterator it;
    for ( it = listOfLocalAwards.begin(); it != listOfLocalAwards.end(); ++it ){
        if (((*it).getEntityPrefix()).count(tprefix) != 0){
            return (*it).getAwardName();
        }
    }
    return awardDefault.getAwardName();
}

Award LocalAwards::getAwardFor(const QString &_tprefix)
{
  QString tprefix;
  tprefix = _tprefix; 
  
//qDebug()  << "LocalAwards::getAwardFor() " << tprefix << endl;
    //awardAux = awardDefault;
  ListOfLocalAwards::iterator it;
  for ( it = listOfLocalAwards.begin(); it != listOfLocalAwards.end(); ++it ){
    if ((*it).checkPrefix(tprefix)){
//qDebug()  << "LocalAwards::getAwardFor() " << "Found award: " << (*it).getAwardName() << endl;
            return (*it);
        }
    }
//qDebug()  << "LocalAwards::getAwardFor() " << "No award found for this prefix" << endl;
    return awardDefault; // If we did not found the award for this prefix we simply return

}



Award LocalAwards::getDefaultAward(){
//cout << "LocalAwards::getDefaultAward() " << endl;
    return awardDefault;
};

QStringList LocalAwards::getAwardReferences(Award &taward){
//cout << "LocalAwards::getAwardReferences() " << endl;
    return taward.getReferences();
}


void LocalAwards::clearLocalAwards(){
    listOfLocalAwards.clear();
    awardReference = 0;
    awardDefault.setAwardReference(-1);
}

void LocalAwards::deleteAward(Award &taward){
    ListOfLocalAwards::iterator it;
    for ( it = listOfLocalAwards.begin(); it != listOfLocalAwards.end(); ++it ){
        if (((*it).getAwardName())==(taward.getAwardName())){
            listOfLocalAwards.erase(it);
        }
    }
}
