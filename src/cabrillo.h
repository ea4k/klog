#ifndef KLOG_CABRILLO_H
#define KLOG_CABRILLO_H
/***************************************************************************
                          cabrillo.h  -  description
                             -------------------
    begin                : apr 2025
    copyright            : (C) 2025 by Jaime Robles
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
    This class implements general controls on Cabrillo standard
    Refer to https://wwrof.org/cabrillo/

    This class should not need to query the DB neither the DataProxy Class

*/
#include <QObject>
#include <QtDebug>
#include "frequency.h"


class Cabrillo : public QObject
{
    Q_OBJECT
public:
    explicit Cabrillo(QObject *parent = nullptr);
    ~Cabrillo();
    void init();
    QString getFrequency(const Frequency &_fr);

signals:
};

#endif // CABRILLO_H
