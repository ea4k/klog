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
    QStringList sortBandNamesBottonUp(const QStringList _qs);
    QStringList getBandIDs();
    QStringList getModesIDs();
    QStringList getBandsInLog(const int _log);
    QStringList getModesInLog(const int _log);

    QString getNameFromBandId (const int _id);
    QString getNameFromModeId (const int _id);
    QString getNameFromSubModeId (const int _id);
    QString getSubModeFromId (const int _id);
    QString getNameFromSubMode (const QString _sm); // Checks if a submode is deprecated TODO: CHeck if really needed
    QString getModeFromSubMode (const QString _sm);
    bool isModeDeprecated (const QString _sm);

    QString getFreqFromBandId(const int _id);
    int getBandIdFromFreq(const double _n);
    double getLowLimitBandFromBandName(const QString _sm);

    int getLastQSOid();

    bool deleteQSO(const int _qsoId);
    int isWorkedB4(const QString _qrz, const int _currentLog);
    bool isThisQSODuplicated(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode);
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
    // Complete with previous
    QString getNameFromQRZ(const QString _call);
    QString getQTHFromQRZ(const QString _call);
    QString getLocatorFromQRZ(const QString _call);
    QString getIOTAFromQRZ(const QString _call);
    QString getQSLViaFromQRZ(const QString _call);
    // /Complete with previous

    int getContinentIdFromContinentShortName(const QString _n);
    QString getContinentShortNameFromEntity(const int _n);
    int getContinentIdFromEntity(const int _n);
    int getCQzFromPrefix(const QString _p);
    int getCQzFromEntity(const int _n);
    int getITUzFromEntity(const int _n);
    int getITUzFromPrefix(const QString _p);
    QString getEntityNameFromId(const int _n);
    QString getEntityMainPrefix(const int _entityN);
    bool isNewCQz(int _c);
    bool isNewEntity(int _e);
    double getLongitudeFromEntity(const int _e);
    double getLatitudeFromEntity(const int _e);
    int getDXCCFromPrefix(const QString _p);
    QString getEntityPrefixes(const int _enti);
    QStringList getEntitiesNames();
    int getHowManyEntities();

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
    QStringList getSatellitesList();

    QStringList getValidCatOptions(const int _currentCat, const int _lowerCa);

    int getHowManyQSOInLog(const int _log);
    int getNumberOfManagedLogs();
    QStringList getListOfManagedLogs();
    int getMaxLogNumber();
    QString getStationCallSignFromLog(const int _log);
    bool addNewLog (const QStringList _qs);
    bool doesThisLogExist(const int _log);

private:
    //bool dbCreated;
    DataBase *db;
    QStringList sortBandIdBottonUp(const QStringList _qs);
    //QSqlRelationalTableModel *logModel;

};

#endif // DATAPROXY_SQLITE_H
