#ifndef FILEAWARDMANAGER_H
#define FILEAWARDMANAGER_H
/***************************************************************************
                          fileawardmanager.h  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
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
#include <QFileDialog>
#include <QFile>
#include "utilities.h"
#include "dataproxy_sqlite.h"
#include "world.h"

class FileAwardManager : public QWidget
{
    Q_OBJECT
public:
    explicit FileAwardManager(DataProxy_SQLite *dp, const QString &_parentFunction);
    ~FileAwardManager();
    bool importNewAwardFile();

signals:

public slots:

private:
    void showError(const QString &_header, const QString &_msg, const QString &_detailedMsg);
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    World *world;

};

#endif // FILEAWARDMANAGER_H
