#ifndef DATAPROXY_H
#define DATAPROXY_H

#include <QString>
#include <QStringList>

class DataProxy
{
public:
    DataProxy();
   // DataProxy(const QStringList _qs);
    ~DataProxy();
    virtual void createLogModel();
    virtual void createLogPanel();
    virtual bool haveAtLeastOneLog();


    //UI functions
    virtual int getIdFromModeName(const QString& _modeName);
    virtual int getIdFromBandName(const QString& _bandName);
    virtual int getSubModeIdFromSubMode(const QString _subModeName);
    virtual int getModeIdFromSubModeId(const int _sm);

    virtual QString getNameFromBandId (const int _id);
    virtual QString getNameFromModeId (const int _id);
    virtual QString getNameFromSubModeId (const int _id);
    virtual QString getSubModeFromId (const int _id);
    virtual QString getNameFromSubMode (const QString _sm); // Checks if a submode is deprecated //TODO: Check if really needed
    virtual QString getModeFromSubMode (const QString _sm);
    virtual bool isModeDeprecated (const QString _sm);

    virtual double getFreqFromBandId(const int _id);
    virtual int getBandIdFromFreq(const double _n);
    virtual QStringList getBands();
    virtual QStringList getModes();
    virtual QStringList getBandsInLog(const int _log);
    virtual QStringList getModesInLog(const int _log);

    virtual int getLastQSOid();
    virtual bool clearLog();

    virtual bool deleteQSO(const int _qsoId);
    virtual int isWorkedB4(const QString _qrz, const int _currentLog);
    virtual bool isDXCCConfirmed(const int _dxcc, const int _currentLog);
    virtual bool isQSLReceived(const int _qsoId);
    virtual bool isQSLSent(const int _qsoId);


    virtual bool qslSentViaDirect(const int _qsoId, const QString _updateDate);
    virtual bool qslSentViaBureau(const int _qsoId, const QString _updateDate);
    virtual bool qslSentAsRequested(const int _qsoId, const QString _updateDate);
    virtual bool qslRecAsRequested(const int _qsoId, const QString _updateDate);
    virtual bool qslRecViaBureau(const int _qsoId, const QString _updateDate);
    virtual bool qslRecViaBureau(const int _qsoId, const QString _updateDate, const bool _queueSentQSL);
    virtual bool qslRecViaDirect(const int _qsoId, const QString _updateDate);
    virtual bool qslRecViaDirect(const int _qsoId, const QString _updateDate, const bool _queueSentQSL);
    virtual bool setClubLogSent(const int _qsoId, const QString _st, const QString _updateDate);

    virtual bool isHF(const int _band);
    virtual bool isWARC(const int _band);
    virtual bool isVHF(const int _band);

    virtual QString getCallFromId(const int _qsoId);
    virtual QStringList getClubLogRealTimeFromId(const int _qsoId);

    virtual QStringList getOperatingYears(const int _currentLog);
    virtual void compressDB();

    virtual int getDXCConYear(const int _year, const int _logNumber);
    virtual int getCQzonYear(const int _year, const int _logNumber);
    virtual bool newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber);

    virtual QStringList getContestNames();
    virtual QStringList getContestCat(const int _catn);
    virtual QStringList getContestOverlays();
    virtual int getContestTypeN(const int _co, const int _catop, const int _catas, const int _catpo, const int _catba, const int _catov, const int _catmo);
    virtual QStringList getDataFromContestType(const int _n);

    virtual QStringList getBandNames();
    virtual QStringList getPropModeList();

    virtual QStringList getValidCatOptions(const int _currentCat, const int _lowerCa);

    virtual int getHowManyQSOInLog(const int _log);
    virtual int getNumberOfManagedLogs();
    virtual QString getStationCallSignFromLog(const int _log);



/*
    virtual bool isMultiplier(const QStringList _qs);
    virtual int getQSOPoints(const QStringList _qs);
    virtual bool saveFileToSend(const QString& _fileName);
    virtual int getTotalScore();
    virtual int getMultipliers();
    virtual int getPoints();
*/
private:

   // int points;
   // int multipliers;

};
#endif // DATAPROXY_H


