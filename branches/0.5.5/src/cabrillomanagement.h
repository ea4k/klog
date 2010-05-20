/***************************************************************************
 *   Copyright (C) 2009 by Jaime Robles, EA4TV                             *
 *   http://jaime.robles.es/klog - jaime@robles.es                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CABRILLOMANAGEMENT_H
#define CABRILLOMANAGEMENT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
//This class shall implement the management of cabrillo files.
// We will receive a file and return a log
// Cabrillo V3 specifications are: http://www.kkn.net/~trey/cabrillo/.

//#include <klocale.h>

#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QProgressDialog>
/*
#include <qtextstream.h>
#include <qfiledialog.h>
#include <qstringlist.h>
#include <qdir.h>
*/
#include "qso.h"

typedef QList<Qso> LogBook;

class CabrilloManagement  {
    //friend class World;
public:
    CabrilloManagement();
    ~CabrilloManagement();

    //LogBook cabrilloReadLog(const QString& tfileName);
    void cabrilloReadLog(const QString& tfileName);


private:

    void processLogLine (const QString& tLogLine);

    Qso qso;
    LogBook logbook;

    QString cabrilloVersion;
    QString stationCallsign;
    QString qsoLine;
    QString cabrilloTab;	// The cabrillo tab read from the log
    QStringList fields; // Auxiliary variable to split log/formated strings


};

#endif

