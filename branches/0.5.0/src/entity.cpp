/***************************************************************************
                          entity.cpp  -  description
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

#include "entity.h"

Entity::Entity(){
}

Entity::Entity (const int tnumb, const QString& tentity, const QString& tpfx, const int tcqz, const long tlon, const long tlat, const QString& tcontinent, const int tituz,  const int ttimeZone){

  numb = tnumb;
  entity = tentity;
  pfx = tpfx;
  cqz = tcqz;
  lon = tlon;
  lat = tlat;
  timeZone = ttimeZone;
  continent = tcontinent;
  ituz = tituz;
	adifCode = 0;
}
/*
Entity::Entity (const int tnumb, const QString& tentity, const QString& tpfx, const int tcqz, const long tlon, const long tlat, const QString& tcontinent, const int tituz,  const int ttimeZone){

  numb = tnumb;
  entity = tentity;
  pfx = tpfx;
  cqz = tcqz;
  lon = tlon;
  lat = tlat;
  timeZone = ttimeZone;
  continent = tcontinent;
  ituz = tituz;
}
*/
Entity::~Entity(){
}

void Entity::setEntity(const QString &tentity){
  entity = tentity;
}

QString Entity::getEntity() const{
    return entity;  // We can have the Entity name translated!
}

void Entity::setPfx(const QString &tpfx){
    pfx = tpfx;
}

QString Entity::getPfx() const{
    return pfx;
}

void Entity::setNumb(int tnumb){
    numb = tnumb;
}

int Entity::getNumb() const{
    return numb;
}

void Entity::setCqz (int tcqz){
    cqz = tcqz;
}

int Entity::getCqz() const{
    return cqz;
}

void Entity::setLon(double tlon){
    lon = tlon;
}

double  Entity::getLon() const{
//qDebug() << "Entity::getLon(): " << QString::number(lon) << endl;
    return lon;
}

void Entity::setLat(double tlat){
    lat = tlat;
}

double Entity::getLat() const{
//qDebug() << "Entity::getLat(): " << QString::number(lat) << endl;
    return  lat;
}

void Entity::setItuz(int tituz){
    ituz = tituz;
}

int Entity::getItuz() const{
    return ituz;
}

void Entity::setContinent(const QString &tcontinent){
    continent = tcontinent;
}

QString Entity::getContinent() const{
    return continent;
}

void Entity::addPrefix(const QString tprx){
  prefixes.append(tprx);
}

void Entity::setTimeZone(int ttimeZone){
    timeZone = ttimeZone;
}

int Entity::getTimeZone() const{
    return timeZone;
}



void Entity::setAdifCode(int tAdif){
	adifCode = tAdif;
}


int Entity::getAdifCode() const{
	return adifCode;
}
