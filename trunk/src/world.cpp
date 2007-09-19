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
#include <klocale.h>
//#include <kdebug.h>
//#include <ostream.h>
#include "world.h"
 
World::World(){
    prefix = "NULL"; // It is just a fake prefix to start with
     int cqzone = 0; 
     int ituzone = 0;
     QString finalPrefix;


  created = false;
  number = 0;

  QStringList lines;
  World::klogDir = QDir::homeDirPath()+"/.klog/";  // We create the ~/.klog for the logs
  QFile file( klogDir+"cty.dat" );
  if (!file.exists()){ // If the cty.dat file is not in the KLOG home, maybe it is in the current directory
      klogDir=QDir::currentDirPath();   // Maybe it is in the current Path??
      file.setName( klogDir+"/cty.dat" );
  /*}else if (!file.exists()){
      klogDir=QDir::homeDirPath();  // Maybe it is in the user's home path???
      file.setName( klogDir+"/cty.dat" );      
      //cout << "homeDirPath: " << QDir::homeDirPath() << endl;
      */
}
// I have no idea where cty.dat is!!!

  if ( file.open( IO_ReadOnly ) ) {
    QTextStream stream( &file );
    QString line;
    while ( !stream.eof() ) {
      number++;
      // Read a Line and get all data
      QString data = stream.readLine();
      // We split the line
      QStringList fields = QStringList::split(SEPARATOR1, data );
      cqzone = fields[1].toInt();
      ituzone = fields[2].toInt();
    entity.setNumb(number);
    entity.setEntity( fields[0].latin1() );
    entity.setPfx((fields[7].stripWhiteSpace()).latin1());
    entity.setCqz(cqzone);
    entity.setLon(fields[5].toDouble());
    entity.setLat( fields[4].toDouble());
    entity.setContinent( fields[3].latin1());
    entity.setItuz(ituzone);
    entity.setTimeZone( fields[6].toInt());
    World::nmap[number] = entity;
      //I create another data stream to process all prefixes
      // and then split the line
      QString datab = stream.readLine();
      datab.remove(0,4);  //removing the spaces at the begining of the lines
      QStringList prefs = QStringList::split(SEPARATOR2, datab );
      //While the line does not have a ";"
      while( !(datab.contains(';') ) ){
        ij = (datab.contains(','));
        n = 0;
        while (n < ij){
     cqzone = fields[1].toInt();
     ituzone = fields[2].toInt();
     finalPrefix = prefs[n];

     // Here I have the prefix but also the (zone)[zone] when special zone is added 
     // HERE IS NECESARY TO CHECK IF THIS IS A SPECIAL PREFIX IN A DIFFERENT ZONE (CQ OR ITU)
        if (prefs[n].contains('(')>0 ){ // The prefix contains special CQzone
     cqzone = (prefs[n].section('(',1)).section(')',0,0).toInt();
        }

        if (prefs[n].contains('[')>0 ){ // The prefix contains special CQzone
         ituzone = (prefs[n].section('[',1)).section(']',0,0).toInt();
        }
 
 if (prefs[n].contains('[') ){
          finalPrefix = prefs[n].left(prefs[n].find('['));
        }
        if (prefs[n].contains('(') ){
          finalPrefix = prefs[n].left(prefs[n].find('('));      
        }


	World::cqMap[finalPrefix] = cqzone;
	World::ituMap[finalPrefix] = ituzone;
	World::map[finalPrefix] = entity;
//	World::cqMapi[cqzone] = number;
//	World::ituMapi[ituzone] = number;
          //entity.addPrefix(prefs[n].latin1());
          n++;
        }//
        datab = stream.readLine();
        datab.remove(0,4);  //removing the spaces at the begining of the lines
        prefs = QStringList::split(SEPARATOR2, datab );
      }// Now we will process when there is ";"
      ij = (datab.contains(','));
      n = 0;
      while (n <= ij){  // Maybe it is only <, not <=
        if (prefs[n].contains(';')){
          j = prefs[n].length() - 1;
          prefs[n].truncate(j);
//          entity.addPrefix(prefs[n].latin1());
        }else{
        }
 finalPrefix = prefs[n];
 
 // Is a prefix with a Zone?
 
 
 // HERE IS NECESARY TO CHECK IF THIS IS A SPECIAL PREFIX IN A DIFFERENT ZONE (CQ OR ITU)
        if (prefs[n].contains('(')>0 ){ // The prefix contains special CQzone
     cqzone = (prefs[n].section('(',1)).section(')',0,0).toInt();
        }

        if (prefs[n].contains('[')>0 ){ // The prefix contains special CQzone
         ituzone = (prefs[n].section('[',1)).section(']',0,0).toInt();
        }
 if (prefs[n].contains('[') ){
          finalPrefix = prefs[n].left(prefs[n].find('['));
        }
        if (prefs[n].contains('(') ){
          finalPrefix = prefs[n].left(prefs[n].find('('));      
        }

 World::cqMap[finalPrefix] = cqzone;
 World::ituMap[finalPrefix] = ituzone;    
        World::map[finalPrefix] = entity;

        n++;
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


// We create the world reading from a file "cty.dat"
void World::create(){

}

QString World::getPrefix() const{
    return prefix;
}


// We obtain the Entity from a call
// It receives a string, get the prefix and send it to
//   bool Entity::findPrefix(const QString tprx)
// for each prefix till find the country in the list.
// Once we have find the country we return its number or
// "0" if we have not found it.

int World::findEntity(const QString string){
// If no Entity is found, we return "0"

// The first task is to isolate the prefix

	prefixa = string;
	prefixab = string;
	prefix = "NULL";
	qrz = prefixa;      // For last checks
	qrz2 = qrz;
	i = 0;

	if(( (prefixa.contains('/')) || (prefixa.contains('\\')) )){
		i = giveEnt(prefixa); // Checking full "special prefixes" like "4U/OH2BBF" first
		if (i!=0){
			//   prefix = prefixa;
			return i;
		}
		if (prefixa.endsWith("/")||prefixa.endsWith("\\")){ // We look for calls ending in slash "/" or "\"
			
			prefixa.remove(prefixa.length()-1,1);
		}
		if (prefixa.contains('/')){
			ij = prefixa.find('/');
			if ( (abs)(ij) < (abs)((prefixa.length())- ij)  ) { //Like in F/EA4TV
				prefixab = prefixa.left(ij);
				qrz2 = prefixab;
			}else{// Like in EA4TV/F
				if (!((prefixa.upper()).endsWith("/P"))	// Portable
				&& (!(prefixa.upper()).endsWith("/B"))	// Beacon
				&& !((prefixa.upper()).endsWith("/M"))	//Mobile
				&& !((prefixa.upper()).endsWith("/MM"))  // Maritime mobile
				&& (!(prefixa.upper()).endsWith("/QRP"))
				&& (!(prefixa.upper()).endsWith("/0"))
				&& (!(prefixa.upper()).endsWith("/1"))
				&& (!(prefixa.upper()).endsWith("/2"))
				&& (!(prefixa.upper()).endsWith("/3"))
				&& (!(prefixa.upper()).endsWith("/4"))
				&& (!(prefixa.upper()).endsWith("/5"))
				&& (!(prefixa.upper()).endsWith("/6"))
				&& (!(prefixa.upper()).endsWith("/7"))
				&& (!(prefixa.upper()).endsWith("/8"))
				&& (!(prefixa.upper()).endsWith("/9"))){
			// It is necessary to check for special prefixes like /r = Rotuma or /a = Mount Athos...
			// So let's go  
					if ( (prefixa.upper().endsWith("/C")) && (prefixa.upper().startsWith("3D2" )) ){
					// This is Conway Reef
						prefixa = "3D2AM";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/R")) && (prefixa.upper().startsWith("3D2" )) ){
					// This is Rotuma
						prefixa = "3D2AA";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/B")) && (prefixa.upper().startsWith("3Y" )) ){
					// This is Bouvet
						prefixa = "3Y";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/C")) && (prefixa.upper().startsWith("FK" )) ){
					// This is Chesterfield Is.
						prefixa = "TX0AT";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/A")) && (prefixa.upper().startsWith("FO" )) ){
					// This is Austral Is.
						prefixa = "FO/HG9B";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/C")) && (prefixa.upper().startsWith("FO" )) ){
					// This is Clipperton Is.
						prefixa = "FO0AAA";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/G")) && (prefixa.upper().startsWith("FR" )) ){
					// This is Glorioso Is.
						prefixa = "FR/G";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/J")) && (prefixa.upper().startsWith("FR" )) ){
					// This is Juan de Nova & Europa Is.
						prefixa = "FR4FA";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/T")) && (prefixa.upper().startsWith("FR" )) ){
					// This is Tromelin Is.
						prefixa = "FR/T";
						return giveEnt(prefixa);
            				}
					if ( (prefixa.upper().endsWith("/S")) && (prefixa.upper().startsWith("GM" )) ){
					// This is Shetland Is.
						prefixa = "2M1ANT";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/A")) && (prefixa.upper().startsWith("HK0" )) ){
					// This is San Andres/Providencia Is.
						prefixa = "HK0";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/M")) && (prefixa.upper().startsWith("HK0" )) ){
					// This is Malpelo Is.
						prefixa = "5J0M";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/M")) && (prefixa.upper().startsWith("JD" )) ){
					// This is Minami Torishima Is.
						prefixa = "7J1ACH";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/O")) && (prefixa.upper().startsWith("JD" )) ){
					// This is Ogasawara Is.
						prefixa = "7J1ATZ";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/B")) && (prefixa.upper().startsWith("JW" )) ){
					// This is Bear Is.
						prefixa = "JW2FL";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/A")) && (prefixa.upper().startsWith("SV" )) ){
					// This is Mount Athos Is.
						prefixa = "SY2A";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/D")) && (prefixa.upper().startsWith("VP6" )) ){
					// This is Ducie Is.
						prefixa = "VP6DI";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/G")) && (prefixa.upper().startsWith("VP8" )) ){
					// This is South Georgia Is.
						prefixa = "VP8BUB";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/H")) && (prefixa.upper().startsWith("VP8" )) ){
					// This is South Shetland Is.
						prefixa = "4K1ADQ";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/O")) && (prefixa.upper().startsWith("VP8" )) ){
					// This is South Orkney Is.
						prefixa = "AY1ZA";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/O")) && (prefixa.upper().startsWith("VP8" )) ){
					// This is South Sandwich Is.
						prefixa = "4K1ZI";
						return giveEnt(prefixa);
					}
					if ( (prefixa.upper().endsWith("/N")) && (prefixa.upper().startsWith("ZK1" )) ){
					// This is North Cook Is.
						prefixa = "ZK1AAH";
						return giveEnt(prefixa);
					}
            				prefixab = prefixa.right(prefixa.length()-(ij+1));
				}else{  // It is a EA4TV/P or EA4TV/QRP    
					// Peter I, is a special "/P" station... so we need to check it.
					if ( (prefixa.upper().endsWith("/P")) && (prefixa.upper().startsWith("3Y" )) ){
						// This is Peter I
						prefixa = "3Y0PI";
						return giveEnt(prefixa);
					}
					// Marquesas, is a special "/M" station... so we need to check it.     
					if ( (prefixa.upper().endsWith("/M")) && (prefixa.upper().startsWith("FO" )) ){
					// This is Marquesas Is.
						prefixa = "FO/DL1AWI";
						return giveEnt(prefixa);
					}
					prefixab =  prefixa.left(ij);
					if ((prefixa.upper().endsWith("/M"))||(prefixa.upper().endsWith("/MM"))){// /M and /MM does not count as DXCC
						return -1;
					}else{// Normal /p, /m, ... prefix

					}
				}
			qrz2 = prefixab;
      }
    }else{
      ij = prefixa.find("\\");
      if ( (abs)(ij) < (abs)(prefixa.length()-ij ) ) { //Like in F\EA4TV
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
      return (itN.data());
  }else{
      itN = nmap.begin();
  }
  return (itN.data());

/*
 // found = false;
   for ( it = map.begin(); it != map.end(); ++it ) {
    if ( (it.data().getNumb()) == i){
        return (it.data());
    }
   }
    it = map.end();
    return (it.data());
*/
}


int World::giveEnt(const QString tqrz){ 
  qrz = tqrz;
  it = map.begin();
  it = map.find(qrz);
  if (map.find(qrz) != map.end()){
      prefix = qrz;
      return it.data().getNumb();
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
      return it.data().getNumb();
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
      return itITU.data();
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
  itITU = ituMap.find(prefix);
  if (ituMap.find(prefix) != ituMap.end()){
      return itITU.data();
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
		return itCQ.data();
	}
	else
		return 0;
}

int World::getCqzFromCall(const QString  tqrz){
	i = findEntity(tqrz);
	return getCqz();
}

int World::howManyEntities(){
  it = map.end();
  return it.data().getNumb();
}

bool World::isWorldCreated(){
    return created;
}

