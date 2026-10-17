#ifndef KLOGNG_PROFILEMANAGER_H
#define KLOGNG_PROFILEMANAGER_H

#include <QString>
#include <QList>
#include <QSqlQuery>

struct Profile {
    int     id = -1;
    QString callsign;
    QString operatorName;
    QString gridsquare;
    QString qth;
    int     cqZone  = 0;
    int     ituZone = 0;
    int     dxcc    = 0;
    QString comment;
    // KLog My Data lauki
    QString name;
    QString address1, address2, address3, address4;
    QString city, zipCode, province, country;
    QString rig1, rig2, rig3;
    QString antenna1, antenna2, antenna3;
    double  power = 0.0;
    // eLog servisi
    QString clublogEmail, clublogPass, clublogAppPass;
    QString qrzUser, qrzPass, qrzLogbookKey;
    QString eqslCall, eqslPass;
    QString lotwUser, lotwPass;
    int     qsoCount = 0;
};

struct ProfileVariant {
    int     id = -1;
    int     profileId = -1;
    QString stationCallsign;
    QString kind;
    QString gridsquare;
    bool    dxccCounts = true;
};

struct ProfileClub {
    int     profileId = -1;
    QString club;        // isais nosaukums (SKCC, FISTS, AGB...)
    QString memberNr;    // numurs var but ar burtiem (SKCC C/T/S limeni)
};

class ProfileManager
{
public:
    ProfileManager() = default;

    bool ensureSchemaAndMigrate(QString *errorOut = nullptr);

    QList<Profile> listProfiles() const;
    Profile getProfile(int id) const;
    int  createProfile(const Profile &p);
    bool updateProfile(const Profile &p);
    bool deleteProfile(int id, QString *errorOut);

    QList<ProfileVariant> listVariants(int profileId) const;
    int  createVariant(const ProfileVariant &v);
    bool deleteVariant(int variantId);

    int  qsoCount(int profileId) const;

    // Nolasa KLog [UserData] iestatijumus no klogrc un saglaba tos profila
    bool saveSettingsToProfile(int profileId, const QString &cfgFile);

    QList<ProfileClub> listClubs(int profileId) const;
    bool setClubs(int profileId, const QList<ProfileClub> &clubs);

private:
    bool exec(QSqlQuery &q, const char *ctx) const;
};

#endif
