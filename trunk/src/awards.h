/***************************************************************************
                          awards.h  -  description
                             -------------------
    begin                : mar ago 27 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@robles.nu
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
#ifndef AWARDS_H
#define AWARDS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
// This class shall read the awardsrc file to know which awards we want to
// check and then will read and create the awards.
#include <qvaluelist.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qstring.h>
#include <klocale.h>
#include <qstringlist.h>
#include "world.h"
#include "reference.h"
#include "award.h"

class Awards  {
// This class read the award config file and creates the awards.
// The user can create awards' files and KLog will read it and manage
// the award for the user's log
public:
	Awards();
	~Awards();
	
	void readConfig();
	int checkPrefix(const QString &tprefix);
		// Returns the Award for a Prefix or -1 if none
	QString getAwardName (const QString &tprefix);
	Award getAwardFor(const QString &tprefix);
	//Award getIntAwardFor(int tEntity);
	Award getDefaultAward();
	QStringList getAwardReferences(Award &taward);
	void clearAwards();
	void deleteAward(Award &taward);
		//Returns a QStringlist with all the references of an award
	
private:
	typedef QValueList<Award> ListOfAwards; // The list of references of the award.
	ListOfAwards listOfAwards;		// The references of the current award.
	QString awardFile;
	QString klogDir;
	bool dirExist;
	Award awardAux;	
	Award awardDefault;		// The defaultAward;
	QStringList listOfReferences;
	int awardReference;
	
};

#endif


