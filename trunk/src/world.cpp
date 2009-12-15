/***************************************************************************
                          world.cpp  -  description
                             -------------------
    begin                : mar dic 17 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@kde.org
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
//#include <klocale.h>
//#include <kdebug.h>

//#include "iostream"
#include "world.h"
// included for debugging
#include <QMessageBox>
#include <QDebug>
//using namespace std;

World::World(){
  prefix = "NULL"; // It is just a fake prefix to start with
  int cqzone = 0;
  int ituzone = 0;
  QString finalPrefix;


  created = false;
  number = 0;

  QStringList lines;
  World::klogDir = QDir::homePath()+"/.klog/";  // We create the ~/.klog for the logs
  QFile file( klogDir+"cty.dat" );
  if (!file.exists()){ // If the cty.dat file is not in the KLOG home, maybe it is in the current directory
      klogDir=QDir::currentPath();   // Maybe it is in the current Path??
      file.setFileName( klogDir+"/cty.dat" );
  /*}else if (!file.exists()){
      klogDir=QDir::homePath();  // Maybe it is in the user's home path???
      file.setName( klogDir+"/cty.dat" );
      //qDebug() << "homePath: " << QDir::homePath() << endl;
      */
  }
// I have no idea where cty.dat is!!!

  if ( file.open( QIODevice::ReadOnly ) ) {
    QTextStream stream( &file );
    QString line;
    while ( !stream.atEnd() ) {
      number++;
      // Read a Line and get all data
      QString data = stream.readLine();
      // We split the line
      QStringList fields = data.split(SEPARATOR1);
      //QStringList fields = QStringList::split(SEPARATOR1, data );
      cqzone = fields[1].toInt();
      ituzone = fields[2].toInt();
      entity.setNumb(number);
      entity.setEntity( fields[0].toLatin1() );
      if (           ((fields[7].trimmed()).count("*"))>0   ) {
        entity.setPfx(((fields[7].trimmed()).remove("*")).toLatin1());
      }else{
        entity.setPfx((fields[7].trimmed()).toLatin1());
      }
      entity.setCqz(cqzone);
      entity.setLon(fields[5].toDouble());
      entity.setLat( fields[4].toDouble());
      entity.setContinent( fields[3].toLatin1());
      entity.setItuz(ituzone);
      entity.setTimeZone( fields[6].toDouble());
      World::map[entity.getPfx()] = entity;
//  qDebug() << QString::number(entity.getNumb()) << " - " << entity.getEntity() << " - Pref: " << entity.getPfx() << endl;
//  qDebug() << "CQz: " << QString::number(entity.getCqz()) << " - ITUz: " << QString::number(entity.getItuz()) << endl;
//  qDebug() << "Continent: " << entity.getContinent() << " - TimeZone: " << QString::number(entity.getTimeZone()) << endl;
//  qDebug() << "Lat: " << QString::number(entity.getLat()) << " - Lon: " << QString::number(entity.getLon()) << endl;
//  qDebug() << "==========================================================================================" << endl << endl;

      World::nmap[number] = entity;
      //I create another data stream to process all prefixes
      // and then split the line
      QString datab = stream.readLine();
      datab.trimmed();
      QStringList prefs = datab.split(SEPARATOR2);
      //QStringList prefs = QStringList::split(SEPARATOR2, datab );

      //While the line does not have a ";"
      while( (datab.count(";"))<1 ) {
        ij = (datab.count(","));
    datab.remove(0,4);
        n = 0;
    QStringList prefs = datab.split(SEPARATOR2);
        //QStringList prefs = QStringList::split(SEPARATOR2, datab );
        while (n < ij){
          cqzone = fields[1].toInt();
          ituzone = fields[2].toInt();

          finalPrefix = prefs[n];

          // Here I have the prefix but also the (zone)[zone] when special zone is added
          // HERE IS NECESARY TO CHECK IF THIS IS A SPECIAL PREFIX IN A DIFFERENT ZONE (CQ OR ITU)
          if (finalPrefix.count("(")>0 ){ // The prefix count special CQzone
           cqzone = (finalPrefix.section("(",1)).section(")",0,0).toInt();
          }

          if (finalPrefix.count("[")>0 ){ // The prefix count special CQzone
           ituzone = (finalPrefix.section("[",1)).section("]",0,0).toInt();
          }
          if (finalPrefix.count("[") >0 ){
           finalPrefix = finalPrefix.left((finalPrefix).indexOf("["));
          }

          if (prefs[n].count("(") >0 ){
           finalPrefix = finalPrefix.left(finalPrefix.indexOf("("));
          }
          if ((finalPrefix.count("=")>0) ){
           finalPrefix = finalPrefix.remove("=");
          }

      //entity.addPrefix(finalPrefix);
          World::cqMap[finalPrefix] = cqzone;
          World::ituMap[finalPrefix] = ituzone;

          World::map[finalPrefix] = entity;
          n++;
        }

        datab = stream.readLine();
        datab.trimmed();  //removing the spaces at the begining of the lines
   //     prefs = QStringList::split(SEPARATOR2, datab );

      }
      // Now we will process when there is ";", the last line of prefixes in an Entity
//qDebug() << "DATAB con ; #" << datab << "#" << endl;
    datab.trimmed();
    datab.remove(0,4);
      prefs = datab.split(SEPARATOR2);
      //prefs = QStringList::split(SEPARATOR2, datab );
      ij = (datab.count(","));
      if (ij==0){
    finalPrefix = datab;
        if ((finalPrefix.count("=")>0) ){
         finalPrefix = finalPrefix.remove("=");
        }
        if ((datab.count(";")>0) ){
         finalPrefix = finalPrefix.remove(";");
        }
        World::map[finalPrefix] = entity;

      }else{
      n = 0;
      while (n <= ij){
        cqzone = fields[1].toInt();
        ituzone = fields[2].toInt();
        finalPrefix = prefs[n];
    if ((finalPrefix.count(";")>0) ){
         finalPrefix = finalPrefix.remove(";");
        }


        // Here I have the prefix but also the (zone)[zone] when special zone is added
        // HERE IS NECESARY TO CHECK IF THIS IS A SPECIAL PREFIX IN A DIFFERENT ZONE (CQ OR ITU)
        if (finalPrefix.count("(")>0 ){ // The prefix contains special CQzone
         cqzone = (finalPrefix.section("(",1)).section(")",0,0).toInt();
        }

        if (finalPrefix.count("[")>0 ){ // The prefix contains special CQzone
         ituzone = (finalPrefix.section("[",1)).section("]",0,0).toInt();
        }
        if (finalPrefix.count("[") >0 ){
         finalPrefix = finalPrefix.left((finalPrefix).indexOf("["));
        }

        if (finalPrefix.count("(") >0 ){
         finalPrefix = finalPrefix.left(finalPrefix.indexOf("("));
        }
        if ((finalPrefix.count("=")>0) ){
         finalPrefix = finalPrefix.remove("=");
        }
        if ((finalPrefix.count(";")>0) ){
         finalPrefix = finalPrefix.remove(";");
        }
    //entity.addPrefix(finalPrefix);
        World::cqMap[finalPrefix] = cqzone;
        World::ituMap[finalPrefix] = ituzone;

        World::map[finalPrefix] = entity;
        n++;
      }
      }


    }
  created = true;
  file.close();


  }else{ //There is no cty.dat
    //ADD A QMESSAGEBOX

  }



//  wnumber = 0;
}

World::~World(){
}

QString World::getPrefix(const QString tqrz){
    aux = findEntity(tqrz);

return (getEntByNumb(aux)).getPfx();

//	return prefix;
}

// We obtain the Entity from a call
// It receives a string, get the prefix and send it to
//   bool Entity::findPrefix(const QString tprx)
// for each prefix till find the country in the list.
// Once we have find the country we return its number or
// "0" if we have not found it.

int World::findEntity(const QString string){
// If no Entity is found, we return "0"
//qDebug() <<  "World::findEntity: " << string << endl;

// The first task is to isolate the prefix

    prefixa = string;
    prefixab = string;
    prefix = "NULL";
    qrz = prefixa;      // For last checks
    qrz2 = qrz;
    i = 0;
    if (prefixa.count('\\')){
//		qDebug() << "World::findEntity: " << "Contains invert / so changes to /" << endl;
        prefixa.replace(QChar('\\'), QChar('/'));
//		qDebug() << "World::findEntity look: " << prefixa << endl;
    }

    if (prefixa.count('/')) {
//		qDebug() << "World::findEntity count /: " << prefixa << endl;
        i = giveEnt(prefixa); // Checking full "special prefixes" like "4U/OH2BBF" first
        if (i!=0){
            return i;
        }
        if (prefixa.endsWith("/") ){ // We look for calls ending in slash "/" or "\"
//			qDebug() << "World::findEntity ends with /: " << prefixa << endl;
            prefixa.remove(prefixa.length()-1,1);
//			qDebug() << "World::findEntity remove the / so: " << prefixa << endl;
        }
        if (prefixa.count('/')){
                        ij = prefixa.indexOf('/');

            n = (prefixa.length())- ij; // n is the length of the second part
            if (n < 0){
                n = -n;
            }

            if ( ij < n ) { //Like in F/EA4TV
                prefixab = prefixa.left(ij);
                qrz2 = prefixab;
            }else { // Like in EA4TV/F
                if (!((prefixa.toUpper()).endsWith("/P"))	// Portable
                && !((prefixa.toUpper()).endsWith("/MM"))  // Maritime mobile
                && (!(prefixa.toUpper()).endsWith("/B"))	// Beacon
                && !((prefixa.toUpper()).endsWith("/M"))	//Mobile
                && !((prefixa.toUpper()).endsWith("/J"))  // JOTA Scout
                && (!(prefixa.toUpper()).endsWith("/LH")) // Light House
                && (!(prefixa.toUpper()).endsWith("/QRP"))
                && (!(prefixa.toUpper()).endsWith("/0"))
                && (!(prefixa.toUpper()).endsWith("/1"))
                && (!(prefixa.toUpper()).endsWith("/2"))
                && (!(prefixa.toUpper()).endsWith("/3"))
                && (!(prefixa.toUpper()).endsWith("/4"))
                && (!(prefixa.toUpper()).endsWith("/5"))
                && (!(prefixa.toUpper()).endsWith("/6"))
                && (!(prefixa.toUpper()).endsWith("/7"))
                && (!(prefixa.toUpper()).endsWith("/8"))
                && (!(prefixa.toUpper()).endsWith("/9"))){
                // It is necessary to check for special prefixes like /r = Rotuma or /a = Mount Athos...
                    if ( (prefixa.toUpper().endsWith("/C")) && (prefixa.toUpper().startsWith("3D2" )) ){
                                // This is Conway Reef
                        prefixa = "3D2CI";
                        return giveEnt(prefixa);
                    }
                    if ( (prefixa.toUpper().endsWith("/R")) && (prefixa.toUpper().startsWith("3D2" )) ){
                    // This is Rotuma
                        prefixa = "3D2AG/P";
                        return giveEnt(prefixa);
                    }
                    if ( (prefixa.toUpper().endsWith("/B")) && (prefixa.toUpper().startsWith("3Y" )) ){
                    // This is Bouvet
                        prefixa = "3Y";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/C")) && (prefixa.toUpper().startsWith("FK" )) ){
                    // This is Chesterfield Is.
                        prefixa = "TX0AT";
                        return giveEnt(prefixa);
                    }
                    if ( (prefixa.toUpper().endsWith("/A")) && (prefixa.toUpper().startsWith("FO" )) ){
                    // This is Austral Is.
                        prefixa = "FO/HG9B";
                        return giveEnt(prefixa);
                    }
                    if ( (prefixa.toUpper().endsWith("/C")) && (prefixa.toUpper().startsWith("FO" )) ){
                    // This is Clipperton Is.
                        prefixa = "FO0AAA";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/G")) && (prefixa.toUpper().startsWith("FR" )) ){
                            // This is Glorioso Is.
                        prefixa = "FR/G";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/J")) && (prefixa.toUpper().startsWith("FR" )) ){
                    // This is Juan de Nova & Europa Is.
                        prefixa = "FR4FA";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/T")) && (prefixa.toUpper().startsWith("FR" )) ){
                    // This is Tromelin Is.
                        prefixa = "FR/T";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/S")) && (prefixa.toUpper().startsWith("GM" )) ){
                    // This is Shetland Is.
                        prefixa = "2M1ANT";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/A")) && (prefixa.toUpper().startsWith("HK0" )) ){
                    // This is San Andres/Providencia Is.
                        prefixa = "HK0";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/M")) && (prefixa.toUpper().startsWith("HK0" )) ){
                    // This is Malpelo Is.
                        prefixa = "5J0M";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/M")) && (prefixa.toUpper().startsWith("JD" )) ){
                    // This is Minami Torishima Is.
                        prefixa = "7J1ACH";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/O")) && (prefixa.toUpper().startsWith("JD" )) ){
                    // This is Ogasawara Is.
                        prefixa = "7J1ATZ";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/B")) && (prefixa.toUpper().startsWith("JW" )) ){
                    // This is Bear Is.
                        prefixa = "JW2FL";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/A")) && (prefixa.toUpper().startsWith("SV" )) ){
                        // This is Mount Athos Is.
                        prefixa = "SY2A";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/D")) && (prefixa.toUpper().startsWith("VP6" )) ){
                    // This is Ducie Is.
                        prefixa = "VP6DI";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/G")) && (prefixa.toUpper().startsWith("VP8" )) ){
                    // This is South Georgia Is.
                        prefixa = "VP8BUB";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/H")) && (prefixa.toUpper().startsWith("VP8" )) ){
                    // This is South Shetland Is.
                        prefixa = "4K1ADQ";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/O")) && (prefixa.toUpper().startsWith("VP8" )) ){
                    // This is South Orkney Is.
                        prefixa = "AY1ZA";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/O")) && (prefixa.toUpper().startsWith("VP8" )) ){
                    // This is South Sandwich Is.
                        prefixa = "4K1ZI";
                        return giveEnt(prefixa);
                    }

                    if ( (prefixa.toUpper().endsWith("/N")) && (prefixa.toUpper().startsWith("ZK1" )) ){
                    // This is North Cook Is.
                        prefixa = "ZK1AAH";
                        return giveEnt(prefixa);
                    }
                    //TODO Check this prefixab = prefixa;

                    //prefixab = prefixa;
                    prefixa = prefixa.right(prefixa.length()-(ij+1));

//					qDebug() << "Should be the shortest: " << prefixa << endl;

                    prefixab = prefixa;
                    //return i;

                }else{  // It is a EA4TV/P or EA4TV/QRP
                    // Peter I, is a special "/P" station... so we need to check it.
//					qDebug() << "Es un corto especial-1: " << prefixa << endl;
                    if ( (prefixa.toUpper().endsWith("/P")) && (prefixa.toUpper().startsWith("3Y" )) ){
                        // This is Peter I
                        //prefixa = "3Y0PI";
                        prefixa = "3Y0X";
                        return giveEnt(prefixa);
                    }else if ( ( (prefixa.toUpper()).endsWith("/M")) && ((prefixa.toUpper()).startsWith("FO" )) ){
                    // Marquesas, is a special "/M" station... so we need to check it.
//					qDebug() << "Marquesas!: " << prefixa << endl;
                        prefixa = "FO/OH1RX";
                        return giveEnt(prefixa);
                    }else if ( ((prefixa.toUpper()).endsWith("/M"))||((prefixa.toUpper()).endsWith("/MM"))){
// /M and /MM does not count as DXCC
//						qDebug() << "/M Normal; " << prefixa << endl;
                        return 0;
                    }else{// Normal /p, /m, ... prefix
//						qDebug() << "Portable/mobile/maritime NORMAL: " << prefixa << endl;
                        //return 0;
                    }
                }
                qrz2 = prefixab;
            } // end of count a / but it is not a EA4TV/F nor F/EA4TV
            }else{ // It does not have any /
//			qDebug() << "World::findEntity does not have /: " << prefixa << endl;
                        ij = prefixa.indexOf("\\");
            n = (prefixa.length())- ij;
            if (n < 0){
                n = -n;
            }

      if (ij < n ) { //Like in F\EA4TV
        prefixab = prefixa.right(prefixa.length() - ij);
        qrz2 = prefixab;
      }else{// Like in F\EA4TV
        prefixab =  prefixa.left(ij-1);
        qrz2 = prefixab;
      }
    }
  }
  prefixa = prefixab;

  i = giveEnt(prefixa); // Checking full calls like "4U1ITU" first
  if (i!=0){
      return i;
  }
  //////////  Begin the "Normal" check
  if ((prefixa.at(1)).isDigit()){ // The second character is a number as in F5XX
      prefixa.truncate(2);
      i = giveEnt(prefixa);
      if ((i == 0) && ((prefixab.at(2)).isDigit()) ){ // Then is a P40A call...
   prefixa = prefixab;
   prefixa.truncate(1);
   i = giveEnt(prefixa);
      }else if ((i == 0) && (!(prefixab.at(2)).isDigit()) ){ // Then is a F call...
   prefixa = prefixab;
   prefixa.truncate(1);
   i = giveEnt(prefixa);
      }
    }else if ((prefixa.at(2)).isDigit()){ // The third character is a number as in EA4T
        prefixa.truncate(4); // looking for VP2M, VK9X, ...
        i = giveEnt(prefixa);
        if (i == 0){  // Looking for EA6, ZS8, VK9...
            prefixa = prefixab;
            prefixa.truncate(3);
            i = giveEnt(prefixa);
            if (i != 0){
                return i; // We've got it! So no more checkings.
            }
        }else{
            return i;  // We've got it! So no more checkings.
        }
        prefixa = prefixab;   // Looking for EA, VK, ZS, ...
        prefixa.truncate(2);
        i = giveEnt(prefixa);

    }else if ((prefixa.at(3)).isDigit()){ // The forth character is a number as in 3DA0SV
        prefixa.truncate(4); // We will look for 3DA0 or 3XY7C
            i = giveEnt(prefixa);
            if (i == 0){
                prefixa = prefixab;
                prefixa.truncate(3);
                i = giveEnt(prefixa);
            }
        }
        if (i == 0){ // I want to look for calls like RA3AA or NA3A
        // The real prefix is just one letter but the next char is NOT a number
        // so the first condition is not satistied.
        prefixa = prefixab;
        if ((prefixa.at(2)).isDigit()){
            prefixa.truncate(1);
            i = giveEnt(prefixa);
        }
    }else{ // If it is not 0, we have detected the Entity. No reason to keep running this.
        return i;
    }
    if (i == 0){
        i = giveEnt(qrz);
    }else{ // If it is not 0, we have detected the Entity. No reason to keep running this.
        return i;
    }

    if (i == 0){ // We have to test for calls as 5U when reading from a log file
        i = giveEnt(qrz2);
    }else{
        return i;
    }
    if (i == 0){  // We have to test for calls as KL7 when
                  // reading from a log file as we truncate -1 to look for KL
        qrz2.truncate(qrz2.length() -1);
        i = giveEnt(qrz2);
    }else{
        return i;
    }

  // We have to check for calls as 3XY1L when reading from a file
  // but just if we have not find another Entity
    if (i == 0){
        if ( ((prefixa.at(3)).isDigit()) && !(prefixa.at(2).isDigit())){ // The forth character is a number as in 3DA0SV
            prefixa.truncate(2); // We will look for 3X
            i = giveEnt(prefixa);
            if (i == 0){
                prefixa.truncate(1); // Maybe it is a F like station
                i = giveEnt(prefixa);
            }
        }else if((prefixa.at(0)).isDigit()){ // HACK: Looking for 3XDQC stile calls
                prefixa.truncate(2);
                i = giveEnt(prefixa);
        }
    }
    return i; // "i" is the Entity number
}



Entity World::getEntByNumb(int num){
  i = num;
/***********************************************************
* We receive the number and we try to get the Entity returning *
* a pointer to it                                          *
************************************************************
*/
// Maybe the way to proceed is to check without running all the list!

  itN = nmap.begin();
  itN = nmap.find(i);
  if (itN != nmap.end()){
      return (itN.value());
  }else{
      itN = nmap.begin();
  }
  return (itN.value());
}


int World::giveEnt(const QString tqrz){
  qrz = tqrz;
  it = map.begin();
  it = map.find(qrz);
  if (map.find(qrz) != map.end()){
      prefix = qrz;
//qDebug() << "giveEnt: " << qrz << QString::number(it.value().getNumb()) << endl;
      return it.value().getNumb();
  }
  else
    return 0;
}

bool World::isSpecial(const QString &tpfx){  // This functions returns true if Entity has special zones
                                        // per prefixes i.e. BY, UA, ...
  qrz = tpfx;
  it = map.begin();
  it = map.find(qrz);
  if (map.find(qrz) != map.end()){
      return it.value().getNumb();
  }
  else
    return 0;
}

int World::getItuz(){
    if (prefix=="NULL") // We are calling this function just after start KLog
 return 0;

//  qrz = prefix;
  itITU = ituMap.begin();
  itITU = ituMap.find(prefix);
  if (ituMap.find(prefix) != ituMap.end()){
      return itITU.value();
  }
  else
    return 0;
}


/*
int World::getItuz(){
    if (tenti<=0) // We are calling this function just after start KLog
 return 0;

//  qrz = prefix;
  itITU = ituMap.begin();
  itITU = itumap.find(prefix);
  if (itumap.find(prefix) != ituMap.end()){
      return itITU.value();
  }
  else
    return 0;
}

*/

  //EntityZMap cqMap, ituMap;
  //EntityNMap::Iterator itCQ, itITU;

int World::getCqz(){
 // qrz = tpfx;
    if (prefix=="NULL") // We are calling this function just after start KLog
        return 0;
    itCQ = cqMap.begin();
        itCQ = cqMap.find(prefix);
        if (cqMap.find(prefix) != cqMap.end()){
                return itCQ.value();
    }
    else
        return 0;
}

int World::getCqzFromCall(const QString  tqrz){
//TODO: Check if the i=... is needed and check if this function is needed or just an unefficiency
// It seems that this way of returning the CQ zone is not very robust
//qDebug() << "World::getCQzFromCall: " << tqrz << endl;
    i = findEntity(tqrz);
    return getCqz();
}

int World::getITUzFromCall(const QString  tqrz){
//TODO: Check if the i=... is needed and check if this function is needed or just an unefficiency
// It seems that this way of returning the ITU zone is not very robust
//qDebug() << "World::getITUzFromCall: " << tqrz << endl;
    i = findEntity(tqrz);
    return getItuz();
    
}

int World::getCQzFromEntity(const int tint){ // Uses getCqz
 qDebug() << "World::getCQzFromEntity: " << QString::number(tint) << endl;
 
 return (getEntByNumb(tint)).getCqz();
//     i = tint;
//     return getCqz();

}

int World::getITUzFromEntity(const int tint){ // Uses getCqz
qDebug() << "World::getITUzFromEntity: " << QString::number(tint) << endl;
//     i = tint;
//     return getItuz();
  return (getEntByNumb(tint)).getItuz();

}

int World::howManyEntities(){
    return number;
  //it = map.end();
  //return it.value().getNumb();
}

bool World::isWorldCreated(){
    return created;
}

QString World::getEntityName(int num){
qDebug() << "World::getEntityName: " << (getEntByNumb(num)).getEntity() << endl;  
 // i = num; //TODO: Just to avoid a compilation warning... to be deleted when this function is implemented
 return (getEntByNumb(num)).getEntity();
    
}

QString World::getMainPrefix (int num){
qDebug() << "World::getMainPrefix: " << (getEntByNumb(num)).getPfx() << endl;  
//  i = num; //TODO: Just to avoid a compilation warning... to be deleted when this function is implemented
//    return "NULL";
  return (getEntByNumb(num)).getPfx();
    
}

