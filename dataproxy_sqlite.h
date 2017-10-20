#ifndef DATAPROXY_SQLITE_H
#define DATAPROXY_SQLITE_H
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
#include <QString>
#include <QStringList>
#include <QObject>
//#include <QtGlobal>

#include "dataproxy.h"
#include "database.h"




class DataProxy_SQLite : public DataProxy {
    Q_OBJECT

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
    int getMostUsedBand(const int _log);
    int getMostUsedMode(const int _log);

    QString getNameFromBandId (const int _id);
    QString getNameFromModeId (const int _id);
    QString getNameFromSubModeId (const int _id);

    QString getSubModeFromId (const int _id);
    QString getNameFromSubMode (const QString _sm); // Checks if a submode is deprecated TODO: CHeck if really needed
    //QString getNameFromSubMode (const QString _sm); // DEPRECATED
    bool isModeDeprecated (const QString _sm);

    QString getFreqFromBandId(const int _id);
    int getBandIdFromFreq(const double _n);
    QString getBandNameFromFreq(const double _n);
    double getLowLimitBandFromBandName(const QString _sm);
    double getLowLimitBandFromBandId(const QString _sm);

    int getLastQSOid();

    bool deleteQSO(const int _qsoId);
    int isWorkedB4(const QString _qrz, const int _currentLog);
    bool isThisQSODuplicated(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode);
    int getDuplicatedQSOId(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode);
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
    bool isUHF(const int _band);

    QString getCallFromId(const int _qsoId);
    QStringList getClubLogRealTimeFromId(const int _qsoId);
    // Complete with previous
    QString getNameFromQRZ(const QString _call);
    QString getQTHFromQRZ(const QString _call);
    QString getLocatorFromQRZ(const QString _call);
    QString getIOTAFromQRZ(const QString _call);
    QString getQSLViaFromQRZ(const QString _call);
    // /Complete with previous

    //LOTW


    //LOTW

    int getContinentIdFromContinentShortName(const QString _n);
    QString getContinentShortNameFromEntity(const int _n);
    int getContinentIdFromEntity(const int _n);
    QStringList getContinentShortNames();
    bool isValidContinentShortName(const QString _n);

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
    bool unMarkAllQSO();            // Unmarks all the marked QSO

    bool clearLog();

    int getQSOonYear(const int _year, const int _logNumber);
    int getDXCConYear(const int _year, const int _logNumber);
    int getCQzonYear(const int _year, const int _logNumber);
    bool newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber);

    QStringList getContestNames();
    QStringList getContestCat(const int _catn);
    QStringList getContestOverlays();
    int getContestTypeN(const int _co, const int _catop, const int _catas, const int _catpo, const int _catba, const int _catov, const int _catmo);
    QStringList getDataFromContestType(const int _n);
    int getLogTypeNumber(const QString _logType);
    QString getLogTypeName(const int _logType);
    QString getLogTypeOfUserLog(const int _logN);
    int getLogNumberFromQSOId(const int _qsoId);

    QStringList getBandNames();
    QStringList getPropModeList();
    QStringList getSatellitesList();   
    QString getSatelliteUplink(const QString _sat);
    QString getSatelliteDownlink(const QString _sat);
    QStringList getQSLRcvdList();
    QStringList getQSLSentList();
    QStringList getClubLogStatusList();
    QStringList getQSLViaList();

    QStringList getValidCatOptions(const int _currentCat, const int _lowerCa);

    int getHowManyQSOInLog(const int _log);
    int getHowManyConfirmedQSLInLog(const int _log);
    int getNumberOfManagedLogs();
    QStringList getListOfManagedLogs();
    int getMaxLogNumber();
    QString getStationCallSignFromLog(const int _log);
    bool addNewLog (const QStringList _qs);
    bool doesThisLogExist(const int _log);

    bool updateISONames(); // Update the entities ISO names for the flags
    QString getISOName(const int _n);

    void getFoundInLog(const QString _txt, const int _log=-1);


private:
    bool dbCreated;
    DataBase *db;
    QStringList sortBandIdBottonUp(const QStringList _qs);
    double getFreqFromRange(QString _fr); //May even receive: 145.900-146.00 and should return the mid in the range (145.950)
    bool searching;
    int executionN;
    //QSqlRelationalTableModel *logModel;

signals:
    void qsoFound(const QStringList _qs); // Each: QString with format: Fieldname:value
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN); // To alert about any failed query execution

};

#endif // DATAPROXY_SQLITE_H
