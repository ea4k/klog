/***************************************************************************
                          award  -  description
                             -------------------
    begin                : vie may 20 2005
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

#ifndef AWARD_H
#define AWARD_H

//This class shall implement ONE award.
// It will receive a QString with the name of a file that has the information for the
// award.
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QDir>
//#include <klocale.h>
//#include "world.h"
#include "reference.h"

class Award  {
    //friend class World;
public:
    Award();
    Award(const QString &tfile);
    ~Award();

    void setAwardReference(int treference);
    int getAwardReference() const;

    void setNumberOfReferences(int treference);
    int getNumberOfReferences() const;

    QStringList getReferences();

    void setAwardName(const QString &tname);
    QString getAwardName() const;

    void setAwardVersion(const QString &tversion);
    QString getAwardVersion() const;

    void setEntityPrefix(const QString &prefix);
    QStringList getEntityPrefix();

    bool checkPrefix(const QString &prefix);

    void setOrganizer(const QString &torganizer);
    QString getOrganizer() const;

    QString getReference(int treference);
    int getReferenceNumber(const QString &refName);

    void workReference(const QString &refName, const bool tworked);
    void confirmReference(const QString &refName, const bool tconfirmed);
    bool isWorked(const QString &refName);
    bool isConfirmed(const QString &refName);

    bool isValidReference(const QString &refName);

    void clearAward();
    void readAward(const QString &tfile);
    void readAwardHeaders(const QString &tfile);
//	References getAllReferences();


private:
    int awardReference;	// Each award shall have an internal reference
    int numberOfReferences;	// To know how many references are in this award
    QString awardName;	// Self explained, isn't it ;-)
    QString entityPrefix;	// Where is this award from. To activate this award if the worked call is from the appropriate entity.
    QString sponsor;	// Information about the organizer (URL, ...) (not mandatory field)
    QString version;	// the version of the award file (just header)
    QString file;		// Definition file;
    QStringList listOfReferences; // To be returned to create the GUI


    typedef QList<Reference> References; // The list of references of the award.
    References references;
    //typedef QMap<QString, Entity> PrefixMap;  // Some awards are for several prefix like						// TPEA (EA, EA6, EA8 & EA9)
    typedef QList<QString> Prefixes;
    Prefixes prefixes;	// Here we will save the list of prefixes for that entity.

    int entity;
    QStringList fields;	// Auxiliary variable to split log/formated strings
    int i;			// Auxiliary int for counters
    int aux;
    int len;		//
    Reference ref;
    int number; // To count the number of created references
    QString klogDir;
    bool dirExist;

};

#endif


