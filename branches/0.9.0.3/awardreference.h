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
#ifndef AWARDREFERENCE_H
#define AWARDREFERENCE_H
//This class shall implement the reference of an award.
#include <QString>


class AwardReference{
public:
    AwardReference();
    ~AwardReference();
    void setReference(const QString &tref);
    QString getReference();
    void setNumber(int tnum);
    int getNumber();
    void setLongName(const QString &tref);
    QString getLongName();
    void clear();
    void setWorked(const bool tworked);
    void setConfirmed(const bool tconfirmed);
    bool isWorked();
    bool isConfirmed();

private:
    int number;
    QString reference;
    QString longName;
    bool worked;
    bool confirmed;


};

#endif

