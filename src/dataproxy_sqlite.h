#ifndef KLOG_DATAPROXY_SQLITE_H
#define KLOG_DATAPROXY_SQLITE_H

/***************************************************************************
                          dataproxy_sqlite.h  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
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
#include <QString>
#include <QStringList>
#include <QObject>
#include <QSqlQuery>
#include "global.h"
#include "database.h"
#include "qso.h"
#include "utilities.h"
#include "klogdefinitions.h"
//#include "regionalaward.h"

class QSO;

enum
{
    CQZones = 40,
    ITUZones = 90,
    DXCCEntities = 521 // http://www.adif.org/adif302.htm#Country%20Codes
};


class DataProxy_SQLite : public QObject
{
    Q_OBJECT

public:
    DataProxy_SQLite(const QString &_parentFunction, const QString &_softVersion="0.0");
    ~DataProxy_SQLite();
    void setLogLevel (const DebugLogLevel _l);
    QString getSoftVersion();
    QString getDBVersion();
    bool reconnectDB();

    void createLogModel();
    void createLogPanel();
    bool haveAtLeastOneLog();

    QStringList getColumnNamesFromTableLog();

    int getIdFromModeName(const QString& _modeName);
    int getIdFromBandName(const QString& _bandName);
    //int getSubModeIdFromSubMode(const QString &_subModeName);

    bool isValidMode(const QString& _modeName);
    bool isValidBand(const QString& _bandName);

    //int getModeIdFromSubModeId(const int _sm);
    bool createHashes();                        // Creates a list of hashes for quick search (band/id & mode/id)

    QStringList getFields();
    //KLOG_DEPRECATED QStringList getBands();
    QStringList getModes(); // Returns the list of submodes
    QStringList sortBandNamesBottonUp(const QStringList _qs);
    QStringList getBandIDs();
    QStringList getModesIDs();
    QStringList getBandsInLog(const int _log);
    QStringList getModesInLog(const int _log);
    int getMostUsedBand(const int _log);
    int getMostUsedMode(const int _log);

    QString getNameFromBandId (const int _id);
    QString getNameFromModeId (const int _id);

    QString getSubModeFromId (const int _id);
    QString getNameFromSubMode (const QString &_sm); // Checks if a submode is deprecated TODO: CHeck if really needed
    //QString getNameFromSubMode (const QString &_sm); // DEPRECATED
    bool isModeDeprecated (const QString &_sm);

    Frequency getFreqFromBandId(const int _id);
    int getBandIdFromFreq(const double _n);
    QString getBandNameFromFreq(const double _n);


    double getLowLimitBandFromBandName(const QString &_sm);
    double getLowLimitBandFromBandId(const int _sm);
    double getUpperLimitBandFromBandName(const QString &_sm);
    bool isThisFreqInBand(const QString &_band, const QString &_fr);

    int getLastQSOid();
    QDate getFirstQSODateFromCall (const QString &_call);  // If the callsign provided is not valid it provides the date of the first QSO
    QDate getLastQSODateFromCall (const QString &_call);   // If the callsign provided is not valid it provides the date of the last QSO

    bool addQSOFromWSJTX (const QString &_dxcall, const QString &_mode, const QString &_band, const double _freq,
                          const QString &_mygrid, const QString &_dxgrid,
                          const QString &_rstTX, const QString &_rstRX, const QString &_sRX, const QString &_sTX,
                          const QString &_comment,
                          const QString &_stationcallsign, const QString &_name, const QString &_operator,
                          const QDateTime &_datetime, const QDateTime &_datetime_off, const double txpower,
                          const int _dxcc, const int _logNumber, bool _sendQSL = true);

    //int addQSO(QSO &_qso);
    //int addQSOQuery(const QSqlQuery &_q);

    bool deleteQSO(const int _qsoId);
    int isWorkedB4(const QString &_qrz, const int _currentLog);
    //QList<int> isThisQSODuplicated(const QString &_callingFunc, const QString &_qrz, const QDateTime &_dateTime, const int _band, const int _mode, const int _secs);
    QList<int> isThisQSODuplicated (const QSO &_qso, const int _secs);
    int getDuplicatedQSOId(const QString &_qrz, const QDateTime &_datetime, const int _band, const int _mode);
    //bool isDXCCConfirmed(const int _dxcc, const int _currentLog);
    bool isQSLReceived(const int _qsoId);
    bool isQSLLoTWReceived(const int _qsoId);
    bool isQSOConfirmed(const int _qsoId, const bool _checkPaper, const bool _checkLoTW);
    bool isQSLSent(const int _qsoId);

    bool qslSentViaDirect(const int _qsoId, const QDate &_updateDate);
    bool qslSentViaBureau(const int _qsoId, const QDate &_updateDate);
    bool qslRecViaBureau(const int _qsoId, const QDate &_updateDate);
    bool qslRecViaBureau(const int _qsoId, const QDate &_updateDate, const bool _queueSentQSL);
    bool qslRecViaDirect(const int _qsoId, const QDate &_updateDate);
    bool qslRecViaDirect(const int _qsoId, const QDate &_updateDate, const bool _queueSentQSL);
    bool qslSentAsRequested(const int _qsoId, const QDate &_updateDate);
    bool qslRecAsRequested(const int _qsoId, const QDate &_updateDate);
    bool setClubLogSent(const int _qsoId, const QString &_st, const QDate &_updateDate);
    bool setLoTWQSLRec (const int _qsoId, const QString &_st, const QDate &_updateDate);

    bool isHF(const int _band);
    bool isWARC(const int _band);
    bool isVHF(const int _band);
    bool isUHF(const int _band);

    int getBandFromId(const int _qsoId);
    int getModeFromId(const int _qsoId);
    int getDXCCFromId(const int _qsoId);
    int getCQZFromId(const int _qsoId);
    QList<int> getBandModeDXCCCQZlogIDFromId(const int _qsoId);

    QString getCallFromId(const int _qsoId);
    QStringList getClubLogRealTimeFromId(const int _qsoId);

    QStringList getFilteredLocators(const QString &_band, const QString &_mode, const QString &_prop, const QString &_sat, bool _confirmed = false);
    //bool updateAwardWAZ();
    // QRZ.com
    bool QRZCOMModifyFullLog(const int _currentLog); // Mark all the log as modified to be sent to QRZ.com
     bool QRZCOMSentQSOs(const QList<int> &_qsos); // Mark as uploaded
    // QRZ.com
    //EQSL
    bool eQSLModifyFullLog(const int _currentLog); // Mark all the log as modified to be sent to eQSL
    bool eQSLSentQSOs(const QList<int> &_qsos); // Mark as uploaded

    //EQSL

    //CLUBLOG
    bool clublogSentQSOs(const QList<int> &_qsos); // Mark as uploaded
    bool clublogModifyFullLog(const int _currentLog); // Mark all the log as modified to be sent to CLubLog
    // CLUBLOG

    //LOTW
    bool lotwSentQueue(const QDate &_updateDate, const int _currentLog);          // Mark LOTW QSL SENT as Q (Queued)
    bool lotwSentYes(const QDate &_updateDate, const int _currentLog, const QString &_station);         // Update LOTW QSL SENT marked as Q as Y (Queued)
    bool lotwSentQSOs(const QList<int> &_qsos);
    int lotwUpdateQSLReception (const QString &_call, const QDateTime &_dateTime, const QString &_band, const QString &_mode, const QDate &_qslrdate);
    QList<int> getQSOsListLoTWToSend(const QString &_stationCallsign, const QString &_myGrid, const QDate &_startDate, const QDate &_endDate, bool _justQueued=true, int _logN = -1);
    //QStringList getQSOsListLoTWNotSent2(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justQueued=true);
    QStringList getQSODetailsForLoTWDownload(const int _id);

    QList<int> getQSOsListeQSLNotSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justQueued=true);

    QList<int> getQSOsListClubLogToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified=true, int _logN = -1);
    QList<int> getQSOsListEQSLToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified=true, int _logN = -1);
    QList<int> getQSOsListQRZCOMToSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, bool _justModified=true, int _logN = -1);
    QList<int> getQSOsListToBeExported(const QString &_stationCallsign, const QString &_grid, const QDate &_startDate, const QDate &_endDate, int _logN = -1);
    QList<int> getQSOsAll();
    QStringList getGridsToBeSent(const QString &_stationCallsign, const QDate &_startDate, const QDate &_endDate, const ExportMode _em, bool _justModified=true, int _logN = -1);


    int getContinentIdFromContinentShortName(const QString &_n);
    QString getContinentShortNameFromEntity(const int _n);
    int getContinentIdFromEntity(const int _n);
    QStringList getContinentShortNames();
    bool isValidContinentShortName(const QString &_n);
    bool isValidDXCC(const int _e);
    QStringList filterValidFields(const QStringList &_fields);

    int getCQzFromPrefix(const QString &_p);
    int getCQzFromEntity(const int _n);
    int getITUzFromEntity(const int _n);
    int getITUzFromPrefix(const QString &_p);
    QString getEntityNameFromId(const int _n);
    int getEntityIdFromName(const QString &_e);
    QString getEntityMainPrefix(const int _entityN);
    QMap<EntityData, int> getAllEntiNameISOAndPrefix();

    int getEntityIdFromMainPrefix(const QString &_e);
    bool isNewCQz(int _c);
    bool isNewEntity(int _e);
    double getLongitudeFromEntity(const int _e);
    double getLatitudeFromEntity(const int _e);
    //KLOG_DEPRECATED int getDXCCFromPrefix(const QString &_p);           // TODO: Replace by int World::getPrefixId(const QString &_prefix)
    QString getEntityPrefixes(const int _enti);
    QStringList getLongPrefixes();
    QStringList getSpecialCallsigns();
    QHash<QString, int> getWorldData();

    QHash<QString, int> getHashTableData(const DataTableHash _data);             //Returns a QHash from a Table (Band, Mode, World)
    QStringList getEntitiesNames(bool _dxccOnly = true);
    QStringList getEntitiesIds();
    int getHowManyEntities();
    int getMaxEntityID(bool limit = true);      // limit = true implies that it will only shouw ARRL valid DXCC (i.e. will not count Sicily)
    QList<int> getListOfDXCCIds();              // Returns the list of valid DXCC id
    QStringList getOperatingYears(const int _currentLog);
    void compressDB();
    bool unMarkAllQSO();            // Unmarks all the marked QSO

    bool clearLog();


    // Stats
    int getQSOonYear(const int _year, const int _logNumber);
    int getDXCConYear(const int _year, const int _logNumber);
    int getCQzonYear(const int _year, const int _logNumber);
    int getQSOsWithDXCC(const int _dxcc, const int _logNumber);
    int getQSOsAtHour(const int _hour, const int _log);
    int getQSOsAtHourOnBand(const int _hour, const int _band ,const int _log);
    int getQSOsOnMonth(const int _month, const int _log);

    int getHowManyQSOInLog(const int _log);
    int getHowManyConfirmedQSLInLog(const int _log);
    int getHowManyQSLSentInLog(const int _log);

    int getQSOsWithContinent(const QString &_cont, const int _logNumber);

    int getQSOsInBand(const QString &_band, const int _log);
    int getQSOsInMode(const QString &_mode, const int _log);
    int getDXCCInBand(const int _bandid, const bool _confirmed, const int _log);

    QList<QList<int>> getTop10QSOPerDXCC(const int _log);

    // Stats - end
    bool updateQSONumberPerLog();

    bool newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber);

    QStringList getContestNames();
    QStringList getContestCat(const int _catn);
    QStringList getContestOverlays();
    //int getContestTypeN(const int _co, const int _catop, const int _catas, const int _catpo, const int _catba, const int _catov, const int _catmo);
    //QStringList getDataFromContestType(const int _n);
    //int getLogTypeNumber(const QString &_logType);
    //QString getLogTypeName(const int _logType);
    //QString getLogTypeOfUserLog(const int _logN);
    int getLogNumberFromQSOId(const int _qsoId);

    QStringList getBandNames();
    QStringList getPropModeList();
    bool isValidPropMode(const QString &_prop);

    bool clearSatList();
    bool addSatellite(const QString &_arrlId, const QString &_name, const QString &_downLink, const QString &_upLink, const QString &_mode, int id = -1);
    int getDBSatId(const QString &_arrlId);
    QStringList getSatellitesList();
    QString getSatelliteUplink(const QString &_sat, int _pair=0);
    QString getSatelliteDownlink(const QString &_sat, int _pair=0);
    QString getSatelliteMode(const QString &_sat);
    QString getSatelliteFullUplink(const QString &_sat);
    QString getSatelliteFullDownlink(const QString &_sat);
    QString getSatelliteFullMode(const QString &_sat);
    QString getSatelliteName(const QString &_sat);
    QString getSateliteArrlIdFromId(const int _id);
    //KLOG_DEPRECATED QStringList getQSLRcvdList();
    //QStringList getQSLSentList();
    //KLOG_DEPRECATED QStringList getClubLogStatusList();
    KLOG_DEPRECATED QStringList getQSLViaList();
    int getHowManyQSOPerPropMode(const QString &_p, const int _logn);


    //QStringList getValidCatOptions(const int _currentCat, const int _lowerCa);

    //bool fillEmptyDXCCInTheLog();

    int getNumberOfManagedLogs();
    QStringList getListOfManagedLogs();
    int getMaxLogNumber();
    QString getStationCallSignFromLog(const int _log);
    QStringList getStationCallSignsFromLog(const int _log);
    QStringList getStationCallSignsFromLogWithLoTWPendingToSend(const int _log);
    QString getOperatorsFromLog(const int _log);
    QString getCommentsFromLog(const int _log);
    QString getLogDateFromLog(const int _log);
    //QString getLogTypeNFromLog(const int _log);
    bool addNewLog (const QStringList _qs);
    bool doesThisLogExist(const int _log);

    bool updateISONames();                          // Update the entities ISO 3166names for the flags
    QString getISOName(const int _n);
    bool addPrimarySubdivisions();                  // Add the primary subdivisions to the BD
    //bool setDXCCAwardStatus(const int _qsoId);
    KLOG_DEPRECATED bool setWAZAwardStatus(const int _qsoId);

    //bool addRegionalAward(RegionalAward _regionalAward);
    bool addDXCCEntitySubdivision(const QString &_name, const QString &_short, const QString &_pref, const QString &_group, const int _regId, const int _dxcc, const int _cq, const int _itu, const QDate &_startDate, const QDate &_endDate, const bool _deleted);
    QList<PrimarySubdivision> getPrimarySubDivisions(const int _entity, const QString &_pref);      // Get the Primary subdivisions for an entity

    //void getFoundInLog(const QString &_txt, const int _log=-1);
    //KLOG_DEPRECATED QString getADIFQSO(const int _qsoId, ExportMode _em = ModeADIF);
    KLOG_DEPRECATED QString getADIFValueFromRec(QSqlRecord _rec, const QString &_fieldName);   // Should be optimized to qso::fromDB / qso::getADIF
    KLOG_DEPRECATED QString getADIFFromQSOQuery(QSqlRecord _rec, ExportMode _em = ModeADIF, bool _justMarked = false, bool _onlyRequested = false, int _logN = -1); // Should be optimized to qso::fromDB / qso::getADIF
    //QString getADIFFromQSOQuery2(QSqlRecord _rec, ExportMode _em = ModeADIF, bool _justMarked = false, bool _onlyRequested = false, int _logN = -1);

    bool showInvalidCallMessage(const QString &_call);

    QList<QSO*> getSatGridStats(int _log=-1);

    QList<QSO*> getGridStats(int _log=-1);
    QList<QSO*> getSatDXCCStats(int _log=-1);

    int getFieldInBand(ValidFieldsForStats _field, const QString &_band, bool confirmedOnly, QString _mode = "ALL", int _log=-1);
    //bool queryPrepare(const QString &_query);
    //bool queryBind(const QString &_field, const QString &value);
    //bool queryExec();

private:
    bool dbCreated;
    DataBase *db;
    QStringList sortBandIdBottonUp(const QStringList _qs);
    double getFreqFromRange(QString _fr, int _pair = 0);            //May even receive: 145.900-146.00 and should return the mid in the range (145.950)
    QStringList getColumnNamesFromTable(const QString &_tableName);
    QString getStringQueryStationCallSign (const QString &_a);      // Creates part of a query regarding the station_call field
    QString getStringQueryMyGrid (const QString &_a);               // Creates part of a query regarding the my_gridsquare field
    QString getStringQueryLogNumber (const int _a);                 // Creates part of a query regarding the lognumber field
    //KLOG_DEPRECATED int getPrefixId(const QString &_qrz);           // TODO: Replace by int World::getPrefixId(const QString &_prefix)
                               // Refactored from fillEmptyDXCCInTheLog
    //bool updateDXCCAndContinent(const int _id, const int _dxcc, const QString &_cont); // Refactored from fillEmptyDXCCInTheLog
    //QString changeSlashAndFindPrefix(const QString &_qrz);
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);
    QSO *qso;
    bool searching;
    int executionN;
    Utilities *util;
    DebugLogLevel logLevel;

    QHash<QString, int> bandIDs;
    QHash<QString, int> modeIDs;
    //QSqlQuery preparedQuery;
    //QSqlRelationalTableModel *logModel;
private slots:
    void slotCaptureDebugLogs(const QString &_func, const QString &_msg, DebugLogLevel _l);
signals:
    void qsoFound(const QStringList _qs); // Each: QString with format: Fieldname:value
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
};

#endif //DATAPROXY_SQLITE_H
