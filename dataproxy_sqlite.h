#ifndef DATAPROXY_SQLITE_H
#define DATAPROXY_SQLITE_H

#include <QString>

#include "dataproxy.h"
#include "database.h"

class DataProxy_SQLite : public DataProxy {

public:
    DataProxy_SQLite();
    ~DataProxy_SQLite();

    void createLogModel();
    void createLogPanel();
    int getIdFromModeName(const QString& _modeName);
    int getIdFromBandName(const QString& _bandName);

    QString getNameFromBandId (const int _id);
    QString getNameFromModeId (const int _id);

    int getLastQSOid();

    bool deleteQSO(const int _qsoId);
    int isWorkedB4(const QString _qrz, const int _currentLog);
    bool isQSLReceived(const int _qsoId);
    bool isQSLSent(const int _qsoId);

    bool qslSentViaDirect(const int _qsoId, const QString _updateDate);
    bool qslSentViaBureau(const int _qsoId, const QString _updateDate);
    bool qslRecViaBureau(const int _qsoId, const QString _updateDate);
    bool qslRecViaBureau(const int _qsoId, const QString _updateDate, const bool _queueSentQSL);
    bool qslRecViaDirect(const int _qsoId, const QString _updateDate);
    bool qslRecViaDirect(const int _qsoId, const QString _updateDate, const bool _queueSentQSL);
    bool qslSentAsRequested(const int _qsoId, const QString _updateDate);
    bool qslRecAsRequested(const int _qsoId, const QString _updateDate);


    QString getCallFromId(const int _qsoId);

    bool clearLog();

private:
    bool dbCreated;
    DataBase *db;
    QSqlRelationalTableModel *logModel;

};

#endif // DATAPROXY_SQLITE_H
