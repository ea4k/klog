/***************************************************************************
                          world.h  -  description
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
#ifndef WORLD_H
#define WORLD_H
const char SEPARATOR1 = ':';
const char SEPARATOR2 = ',';
//#include <qwidget.h>
#include <qstring.h>
#include <qvaluelist.h>
#include <klistview.h>
#include <qstringlist.h>
#include <qfile.h>
//#include <qmessagebox.h>
//#include <qfiledialog.h>
#include <qmap.h>
#include <qdir.h>
#include "entity.h"


typedef QValueList<Entity> Entities;
  static Entities entities;

class World{
public:

	World();

	~World();

	void create();
	bool isWorldCreated();

	int findEntity(const QString  string);	// If no Entity is found, we return "0"

  // HACK: I should fool-proof this function adding a QString as a call to get the zone
	QString getPrefix() const; // Returns the prefix we found in findEntity.

//  int getEnti(const QString ttqrz);
//  int getEntiCompleteQRZ(const QString ttqrz);
//  Entity getEnt(const QString ttqrz);
	Entity getEntByNumb(int num);
//  int getAdifNumber(const QString entity);
	int giveEnt(const QString tqrz);
	int howManyEntities();

	bool isSpecial(const QString &tpfx);  // This functions returns true if Entity has special zones
                                        // per prefixes i.e. BY, UA, ...
	int getItuz(); // HACK: I should fool-proof this function adding a QString as a call to get the zone
	int getCqz();  // HACK: I should fool-proof this function adding a QString as a call to get the zone
	//int getItuz(int tenti);
	//int getCqz(int tenti);
	int getCqzFromCall(const QString  tqrz); // Uses findEntity and getCqz
// isSpecial,  maybe is to be deleted.
  
private:
  int number;
//  bool found;
  bool created;
//  int wnumber;
  int i, ij, n, j, aux;
  QString qrz;  
  QString qrz2;
  QString prefix; // This is the prefix once we finally have found it (written in findEntity)
  Entity entity;
  QString prefixa;
  QString prefixab;
  QString klogDir;  
//  int location[0,0];

	typedef QMap<QString, Entity> EntityMap;  // To map Prefix/Entity
	typedef QMap<int, Entity> EntityNMap;     // To map EntityNumber/Entity
	typedef QMap<QString, int> EntityZMap;    // To map Prefix/Zone (CQ/ITU for all)
//	typedef QMap<int, int> EntityIIMap;	// To map Zone/Entity(number)

	EntityMap map; 				// Prefix/Entity
	EntityMap::Iterator it;
	EntityNMap nmap;			// EntityNumber/Entity
	EntityNMap::Iterator itN;		 
	EntityZMap cqMap, ituMap;
	EntityZMap::Iterator itCQ, itITU;
//	EntityIIMap::cqMapi, ituMapi;			//TODO Map entity/zone to do things faster
//	EntityIIMap::Iteraror itCqMapi, itItuMapi;	//TODO Map entity/zone to do things faster

};
#endif
