/***************************************************************************
                          prefix.cpp  -  description
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
#include "prefix.h"


Prefix::Prefix(){
//    special = false; 
}

Prefix::Prefix (const QString& tpfx){
//    special = false;    
    pfx = tpfx;  
}

Prefix::~Prefix(){
}

void Prefix::setPfx(QString &tpfx){
  pfx = tpfx;
}

QString Prefix::getPfx() const{
    return pfx;
  
}

/*
void Prefix::setCqZone (int cqz){ // to try to get the special prefixes with a special zone
    cqZone = cqz;
}

int Prefix::getCqZone() const{ // to try to get the special prefixes with a special zone
    return cqZone;    
}

void Prefix::setItuZone (int ituz){ // to try to get the special prefixes with a special zone
    ituZone = ituz;
}

int Prefix::getItuZone() const{ // to try to get the special prefixes with a special zone
    return ituZone;
}

void Prefix::itIsSpecial(bool spec){ // to try to get the special prefixes with a special zone
 special = spec;   
}

bool Prefix::isItSpecial(){  // to try to get the special prefixes with a special zone
    return special;
}
*/
