/***************************************************************************
                          entity.h  -  description
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

#ifndef ENTITY_H
#define ENTITY_H
#include "prefix.h"
#include <QList>
// Commented out the below line as ENTITIES is defined as a const int in awarddxcc.h. This causes an
// obscure compiler error.
//#define ENTITIES 340

 class Entity{

public:

  Entity();
  Entity (const int tnumb, const QString& tentity, const QString& tpfx, const int tcqz, const long tlon, const long tlat, const QString& tcontinent, const int tituz, const int ttimeZone);
//  Entity (const int tnumb, const QString& tentity, const QString& tpfx, const int tcqz, const long tlon, const long tlat, const QString& tcontinent, const int tituz, const int ttimeZone);


  ~Entity();

  typedef QList<Prefix> Prefixes;
 Prefixes prefixes;


  void setEntity(const QString &tentity);
  QString getEntity() const;

  void setPfx(const QString &tpfx);
  QString getPfx() const;

  void setNumb(int tnumb);
  int getNumb() const;

  void setCqz (int tcqz);
  int getCqz() const;

  void setLon(double tlon);
  double  getLon() const;

  void setLat(double tlat);
  double getLat() const;

  void setItuz(int tituz);
  int getItuz() const;

  void setAdifCode(int tAdif);
  int getAdifCode() const;

  void setTimeZone(int ttimeZone);
  int getTimeZone() const;

  void setContinent(const QString &tcontinent);
  QString getContinent()const;

  void addPrefix(const QString tprx);
//  int findPrefix(const QString tprx);

// We obtain the Entity from a call


private:
  int numb;
  QString entity;
  QString pfx;         // Main Prefix
  QString continent;
  int cqz;
  double lon;
  double lat;
 // int tzone;
  int ituz;
  int timeZone;
    int adifCode;


};
#endif
