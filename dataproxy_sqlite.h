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
    int getSubModeIdFromSubMode(const QString _subModeName);
    int getModeIdFromSubModeId(const int _sm);
    QStringList getBands();
    QStringList getModes();
    QStringList getBandsInLog(const int _log);
    QStringList getModesInLog(const int _log);

    QString getNameFromBandId (const int _id);
    QString getNameFromModeId (const int _id);
    QString getNameFromSubModeId (const int _id);
    QString getSubModeFromId (const int _id);
    QString getNameFromSubMode (const QString _sm); // Checks if a submode is deprecated TODO: CHeck if really needed
    QString getModeFromSubMode (const QString _sm);
    bool isModeDeprecated (const QString _sm);

    double getFreqFromBandId(const int _id);
    int getBandIdFromFreq(const double _n);

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
    bool setClubLogSent(const int _qsoId, const QString _st, const QString _updateDate);

    bool isHF(const int _band);
    bool isWARC(const int _band);
    bool isVHF(const int _band);

    QString getCallFromId(const int _qsoId);
    QStringList getClubLogRealTimeFromId(const int _qsoId);

    QStringList getOperatingYears(const int _currentLog);
    void compressDB();

    bool clearLog();

    int getDXCConYear(const int _year, const int _logNumber);
    int getCQzonYear(const int _year, const int _logNumber);
    bool newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber);

    QStringList getContestNames();
    QStringList getContestCat(const int _catn);
    QStringList getContestOverlays();
    int getContestTypeN(const int _co, const int _catop, const int _catas, const int _catpo, const int _catba, const int _catov, const int _catmo);
    QStringList getDataFromContestType(const int _n);

    QStringList getBandNames();
    QStringList getPropModeList();

    QStringList getValidCatOptions(const int _currentCat, const int _lowerCa);

    int getHowManyQSOInLog(const int _log);
    int getNumberOfManagedLogs();
    QString getStationCallSignFromLog(const int _log);

private:
    bool dbCreated;
    DataBase *db;
    //QSqlRelationalTableModel *logModel;

};

#endif // DATAPROXY_SQLITE_H
