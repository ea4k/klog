#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

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
