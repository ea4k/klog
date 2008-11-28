/***************************************************************************
                          reference  -  description
                             -------------------
    begin                : sb may 21 2005
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
#include "reference.h"
//#include <ostream.h>
 
Reference::Reference(){
	number = -1;
	reference = "X";
	longName = i18n("Select The Reference");
	worked = false;
	confirmed = false;
}

Reference::~Reference(){}
void Reference::setReference(const QString &tref){
	reference = tref;
}

QString Reference::getReference(){
	return reference;
}
void Reference::setNumber(int tnum){
	number = tnum;
}
int Reference::getNumber(){
	return number;
}	

void Reference::setLongName(const QString &tref){
	longName = tref;
}
QString Reference::getLongName(){
	return longName;
}

void Reference::clear(){
	number = -1;
	reference = "X";
	longName = i18n("Select The Reference");
}

void Reference::setWorked(const bool tworked){
	worked = tworked;
	if (tworked == false)
		confirmed = false;
	//cout << "Reference worked: " << getLongName() << endl;
}

void Reference::setConfirmed(const bool tconfirmed){
	confirmed = tconfirmed;
	if (tconfirmed == true)
		worked = true;
	//cout << "Reference confirmed: " << getLongName() << endl;
}

bool Reference::isWorked(){
	return worked;
}

bool Reference::isConfirmed(){
	return confirmed;
}


