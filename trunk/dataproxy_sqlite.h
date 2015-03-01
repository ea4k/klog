#ifndef DATAPROXY_SQLITE_H
#define DATAPROXY_SQLITE_H

#include <QString>
#include <QStringList>

#include "dataproxy.h"
#include "database.h"

class DataProxy_SQLite : public DataProxy {

public:
    DataProxy_SQLite();
    ~DataProxy_SQLite();

    void createLogModel();
    void createLogPanel();
    bool haveAtLeastOneLog();

    int getIdFromModeName(const QString& _modeName);
    int getIdFromBandName(const QString& _bandName);

    QString getNameFromBandId (const int _id);
    QString getNameFromModeId (const int _id);

    double getFreqFromBandId(const int _id);

    int getLastQSOid();

    bool deleteQSO(const int _qsoId);
    int isWorkedB4(const QString _qrz, const int _currentLog);
    bool isDXCCConfirmed(const int _dxcc, const int _currentLog);
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

    bool isHF(const int _band);
    bool isWARC(const int _band);
    bool isVHF(const int _band);

    QString getCallFromId(const int _qsoId);
    QStringList getOperatingYears(const int _currentLog);
    void compressDB();

    bool clearLog();

    int getDXCConYear(const int _year, const int _logNumber);
    int getCQzonYear(const int _year, const int _logNumber);

    QStringList getContestNames();
    QStringList getContestCat(const int _catn);

    QStringList getBandNames();
    QStringList getValidCatOptions(const int _currentCat, const int _lowerCa);

    int getNumberOfManagedLogs();

private:
    bool dbCreated;
    DataBase *db;
    //QSqlRelationalTableModel *logModel;

};

#endif // DATAPROXY_SQLITE_H
