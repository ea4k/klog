#ifndef ADIF_H
#define ADIF_H
/***************************************************************************
                          adif.h  -  description
                             -------------------
    begin                : ago 2023
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
/*
    This class implements general controls on ADIF standard
    Refer to https://www.adif.org/

    This classs hould not need to query the DB neither the DataProxy Class

*/
#include <QObject>
#include <QtGlobal>
#include <QString>
#include <QtDebug>
#include <QStringList>
//#include "locator.h"
#include "klogdefinitions.h"


class Adif : public QObject {
    Q_OBJECT
    //friend class tst_Adif;

public:
    Adif(const QString &_parentName);
    ~Adif();
    void init();
    bool isValidFreq(const QString &_b);
    bool isValidPower(const QString &_b);
    void setLogLevel(DebugLogLevel _l);
    QString getADIFField(const QString &_fieldName, const QString &_data);

signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);

private:
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    void InitializeHash();
    void setARRLSect();
    void setContinents();
    void setSponsorsList();

    bool isValidCall(const QString &_c);

    QString parentName;
    DebugLogLevel logLevel;
    QHash<QString, QString> ADIFHash; // Name, type

    QStringList ARRL_sects, continents, sponsorsList;

};

#endif // ADIF_H


