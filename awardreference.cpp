/***************************************************************************
                          awardreference  -  description
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
#include "awardreference.h"
//#include <ostream.h>
 
AwardReference::AwardReference(){
	number = -1;
	reference = "X";
        longName = ("Select The Reference");
	worked = false;
	confirmed = false;
}

AwardReference::~AwardReference(){}

void AwardReference::setReference(const QString &tref)
{
	reference = tref;
}

QString AwardReference::getReference(){
	return reference;
}
void AwardReference::setNumber(int tnum){
	number = tnum;
}
int AwardReference::getNumber(){
	return number;
}	

void AwardReference::setLongName(const QString &tref){
	longName = tref;
}
QString AwardReference::getLongName(){
	return longName;
}

void AwardReference::clear(){
	number = -1;
	reference = "X";
        longName = ("Select The Reference");
}

void AwardReference::setWorked(const bool tworked){
	worked = tworked;
	if (tworked == false)
		confirmed = false;
    //cout << "AwardReference worked: " << getLongName() << endl;
}

void AwardReference::setConfirmed(const bool tconfirmed){
	confirmed = tconfirmed;
	if (tconfirmed == true)
		worked = true;
    //cout << "AwardReference confirmed: " << getLongName() << endl;
}

bool AwardReference::isWorked(){
	return worked;
}

bool AwardReference::isConfirmed(){
	return confirmed;
}


