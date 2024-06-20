#ifndef DB_ADIF_PRIMARY_SUBDVISIONS_DATA_H
#define DB_ADIF_PRIMARY_SUBDVISIONS_DATA_H

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
    bool add_EA6_21();      // Adds the data for Balearic Is
    bool add_EA_281();      // Adds the data for Spain


};

#endif // DB_ADIF_PRIMARY_SUBDVISIONS_DATA_H
