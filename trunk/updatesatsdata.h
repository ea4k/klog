#ifndef UPDATESATSDATA_H
#define UPDATESATSDATA_H
/***************************************************************************
                          updatesatsdata.h  -  description
                             -------------------
    begin                : sep 2018
    copyright            : (C) 2018 by Jaime Robles
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
#include <QObject>
#include <QFile>
#include <QProgressDialog>
#include <QFileDialog>
#include "utilities.h"
#include "dataproxy.h"
#include "dataproxy_sqlite.h"

class UpdateSatsData : public QObject
{
    Q_OBJECT

public:
    explicit UpdateSatsData(DataProxy *dp, QObject *parent = nullptr);
    bool readSatDataFile();

private:

    bool satDataFileRead(const QString& tfileName);
    Utilities *util;
    DataProxy *dataProxy;

signals:
    void satsUpdatedSignal(const bool _p);
//    void clearError();

public slots:
};

#endif // UPDATESATSDATA_H
