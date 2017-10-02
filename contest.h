#ifndef CONTEST_H
#define CONTEST_H
/***************************************************************************
                          contest.h  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QString>
#include <QStringList>
#include <QTextEdit>
//#include <QtWidgets>

class Contest
{
public:


    Contest();
    Contest(const QStringList _qs);
    ~Contest();

    virtual bool isMultiplier(const QStringList _qs);
    virtual int getQSOPoints(const QStringList _qs);
    virtual bool saveFileToSend(const QString& _fileName);
    virtual int getTotalScore();
    virtual int getMultipliers();
    virtual int getPoints();


private:

    int points;
    int multipliers;

};

#endif // CONTEST_H
