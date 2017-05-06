#ifndef UTILITIES_H
#define UTILITIES_H
/***************************************************************************
                          utilities.h  -  description
                             -------------------
    begin                : jun 2015
    copyright            : (C) 2015 by Jaime Robles
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
/*
    This class implements general use utilities that can be used from any other class
    Anything that is not directly related with a class itself and could be used from different
    classes should be here

*/
#include <QtGlobal>
#include <QString>
#include <QtWidgets>
#include <QtDebug>

class Utilities
{
public:
    Utilities();
    int getProgresStepForDialog(int totalSteps);
    bool trueOrFalse(const QString _s); // reads a String and return true if s.upper()== TRUE :-)
    QString checkAndFixASCIIinADIF(const QString _data);
    QString getAgent(const QString _klogversion);
    //Devel or debug functions - Not adding any feature to the user
    void printQString(const QStringList _qs);
    QString getKLogDatabaseFile();
    QString getHomeDir();
    QString getCfgFile();
    QString getCTYFile();

    int getNormalizedDXCCValue(const int _dxcc);

};

#endif // UTILITIES_H
