#ifndef KLOG_EDI_H
#define KLOG_EDI_H
/***************************************************************************
                          edi.h  -  description
                             -------------------
    begin                : ap 2025
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
    This class implements general controls on EDI standard
    Refer to https://www.darc.de/fileadmin/_migrated/content_uploads/EDI_REG1TEST.pdf

    This class should not need to query the DB neither the DataProxy Class

*/
#include <QObject>
//#include <QtGlobal>

//#include "klogdefinitions.h"


class EDI : public QObject {
    Q_OBJECT
    //friend class tst_Adif;

public:
    EDI(const QString &_parentName);
    ~EDI();
    void init();
    bool isValidCall(const QString &_b);            //>0


    void setLogLevel(DebugLogLevel _l);
    QString getEDIFField(const QString &_fieldName, const QString &_data);


signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);

private:
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    void InitializeBandHash();
    void InitializeModeHash();


    bool isValidCall(const QString &_c);

    QString parentName;
    DebugLogLevel logLevel;
    QHash<QString, QString> bandHash;   // ADIF, EDI
    QHash<int, QString> modeTXHash;       // code, TX mode
    QHash<int, QString> modeRXHash;       // code, RX mode

};

#endif // EDI_H


