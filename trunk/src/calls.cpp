/***************************************************************************
                          calls  -  description
                             -------------------
    begin                : jue jul 22 2004
    copyright            : (C) 2004 by Jaime Robles
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
//#include <ostream.h>
#include "calls.h"


Calls::Calls(){
}

Calls::Calls(const QString& tqrz, int tnumb){
  qrz = tqrz;
  numb = tnumb;
}



Calls::~Calls(){
}

void Calls::setQrz(const QString &tqrz ){
  qrz = tqrz;
}

QString Calls::getQrz() const {
  return qrz;
}


void Calls::setNumb (int tnumb){
  numb = tnumb  ;
}

int Calls::getNumb() const{
    return numb;}

void Calls::setComment(const QString &tcomment){
    comment = tcomment;
}

QString Calls::getComment()const{
    return comment;}

void Calls::setQslManager(const QString &tqrz){
    manager = tqrz;

}

QString Calls::getQslManager() const {
    return manager;}

void Calls::setQslInfo(const QString &tqslInfo){
  qslInfo = tqslInfo;

}

QString Calls::getQslInfo()const{
    return qslInfo;}


void Calls::setLocator(const QString &tgridSquare ){
  gridSquare = tgridSquare;
}

QString Calls::getLocator() const {
  return gridSquare;
}

void Calls::setIota(const QString &tiota){
  iota = tiota;
}

QString Calls::getIota() const{
    return iota;}

QString Calls::getIotaContinent() const{
  return iota.section('-',0,0);
}

int Calls::getIotaNumber() const {
  return (iota.section('-',1,1)).toInt();
}

void Calls::setName(const QString &tname){
  name = tname;  
}

QString Calls::getName() const{
  return name;  
}

void Calls::setQth(const QString &tqth){
  qth = tqth;
}

QString Calls::getQth() const{
  return qth;
}

