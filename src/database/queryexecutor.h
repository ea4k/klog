#ifndef KLOG_DATABASE_QUERYEXECUTOR_H
#define KLOG_DATABASE_QUERYEXECUTOR_H
/***************************************************************************
                          queryexecutor.h  -  description
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
    This class defines a test to try to control DB executions from one
    single place.
    This class may be removed without any notice :)
*/

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>

class QueryExecutor : public QObject
{
    Q_OBJECT
public:
    explicit QueryExecutor(const QString &_parentClass);
    bool execQuery(const QString &function, const QString &stringQuery);
    void queryErrorManagement(const QString &_functionFailed, const QString &errorCodeS, const QString &_nativeError, const QString &_failedQuery);

private:


};

#endif // QUERYEXECUTOR_H
