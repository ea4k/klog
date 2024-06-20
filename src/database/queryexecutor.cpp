#include "queryexecutor.h"

QueryExecutor::QueryExecutor(const QString &_parentClass)
{
    Q_UNUSED(_parentClass);
    qDebug() << Q_FUNC_INFO << _parentClass ;
}

void QueryExecutor::queryErrorManagement(const QString &_functionFailed, const QString &errorCodeS, const QString &_nativeError, const QString &_failedQuery)
{
    Q_UNUSED(_functionFailed);
    Q_UNUSED(errorCodeS);
    Q_UNUSED(_nativeError);
    Q_UNUSED(_failedQuery);
    //qDebug() << Q_FUNC_INFO << ": constrid    : " << QString::number(constrid) ;
    //qDebug() << Q_FUNC_INFO << ": Function    : " << _functionFailed ;
    //qDebug() << Q_FUNC_INFO << ": Native      : " << _nativeError ;
    //qDebug() << Q_FUNC_INFO << ": Error       : " << _functionFailed << errorCodeS ;
    //qDebug() << Q_FUNC_INFO << ": Query failed: " << _failedQuery ;
}

bool QueryExecutor::execQuery(const QString &function, const QString &stringQuery)
{
    qDebug() << Q_FUNC_INFO << " " << function << " : " << stringQuery ;
    QSqlQuery query;
    query.prepare(stringQuery);
    bool ok = query.exec(stringQuery);
    query.finish();
    if (!ok)
    {
        queryErrorManagement(function, query.lastError().databaseText(), query.lastError().nativeErrorCode(), query.lastQuery());
    }
    return ok;
}
