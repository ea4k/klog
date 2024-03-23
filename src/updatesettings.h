#ifndef KLOG_UPDATESETTINGS_H
#define KLOG_UPDATESETTINGS_H
/***************************************************************************
                          updatesettings.h  -  description
                             -------------------
    begin                : mar 2023
    copyright            : (C) 2023 by Jaime Robles
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

#include <QObject>
#include <QtGlobal>

//#include <QSettings>
//#include <QFile>
//#include <QString>
#include "utilities.h"
#include "locator.h"


class UpdateSettings : public QObject
{
    Q_OBJECT
public:
    UpdateSettings();
    ~UpdateSettings();
    bool updateFile();

private:
    bool processConfigLine(const QString &_line);
    bool findInFile();
    bool renameFile(const QString &_oldName, const QString &_newName);
};

#endif // UPDATESETTINGS_H
