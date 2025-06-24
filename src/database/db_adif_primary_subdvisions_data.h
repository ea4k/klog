#ifndef KLOG_DATABASE_DB_ADIF_PRIMARY_SUBDVISIONS_DATA_H
#define KLOG_DATABASE_DB_ADIF_PRIMARY_SUBDVISIONS_DATA_H
/***************************************************************************
                          db_adif_primary_subdivisions_data.h  -  description
                             -------------------
    begin                : Apr 2024
    copyright            : (C) 2024 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
/*
    This class contains the data of the Primary Subdivisions of ADIF as listed
    here:
    https://www.adif.org/314/ADIF_314.htm#Primary_Administrative_Subdivision

    This class exists just to have one single place to maintain any ADIF
    modification in the future.

*/
#include <QObject>
#include "queryexecutor.h"

class DB_ADIF_Primary_Subdvisions_data : public QObject
{
    Q_OBJECT
public:
    explicit DB_ADIF_Primary_Subdvisions_data(const QString &_parentClass);
    bool addData();

signals:

private:
    // To add new entity data, a new function should be created.
    // The format should be "add_MAIN-PREFIX-ARRLid
    QueryExecutor *exe;
    bool isDBCreated();     // This function checks if the DB is created.
    bool addValues(const QString &rows, const QStringList &values);
    bool add_Canada_1();    // Adds the data for Canada
    bool add_USA_6();       // Adds the data for Alaska
    bool add_EA6_21();      // Adds the data for Balearic Is
    bool add_EA8_29();      // Adds the data for Canary Is
    bool add_EA9_32();      // Adds the data for Ceuta y Melilla
    bool add_Mexico_50();   // Adds the data for Mexico
    bool add_Argentina_100();// Adds the data for Argentina
    bool add_Brazil_108();  // Adds the data for Brazil
    bool add_I_248();       // Adds the data for Italy
    bool add_EA_281();      // Adds the data for Spain
    bool add_USA_110();     // Adds the data for Hawaii
    bool add_CT_149();      // Adds the data for Azores
    bool add_CT_256();      // Adds the data for Madeira
    bool add_CT_272();      // Adds the data for Portugal
    bool add_USA_291();     // Adds the data for United States of America
    bool add_Japan_339();   // Adds the data for Japan
};

#endif // DB_ADIF_PRIMARY_SUBDVISIONS_DATA_H
