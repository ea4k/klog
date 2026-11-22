#include "profilemanager.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QObject>
#include <QDebug>
#include <QSettings>

bool ProfileManager::exec(QSqlQuery &q, const char *ctx) const
{
    if (q.exec())
        return true;
    qWarning() << "ProfileManager" << ctx << "SQL:" << q.lastError().text();
    return false;
}

bool ProfileManager::ensureSchemaAndMigrate(QString *errorOut)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        if (errorOut) *errorOut = QStringLiteral("Datubaze nav atverta");
        return false;
    }
    QSqlQuery q(db);

    q.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS profiles ("
        " profile_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " callsign VARCHAR(15) NOT NULL UNIQUE,"
        " operator_name VARCHAR, gridsquare VARCHAR(12), qth VARCHAR,"
        " cq_zone INTEGER, itu_zone INTEGER, dxcc INTEGER,"
        " default_rig VARCHAR, default_antenna VARCHAR, default_tx_pwr REAL,"
        " qsl_via VARCHAR, comment VARCHAR,"
        " created_at TEXT DEFAULT (datetime('now')))"));
    if (!exec(q, "createProfiles")) return false;

    q.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS profile_variants ("
        " variant_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " profile_id INTEGER NOT NULL REFERENCES profiles(profile_id),"
        " station_callsign VARCHAR(20) NOT NULL,"
        " kind VARCHAR(8) NOT NULL DEFAULT 'BASE'"
        "   CHECK (kind IN ('BASE','P','M','MM','AM','CONTEST','OTHER')),"
        " gridsquare VARCHAR(12),"
        " dxcc_counts INTEGER NOT NULL DEFAULT 1,"
        " note VARCHAR,"
        " UNIQUE (profile_id, station_callsign))"));
    if (!exec(q, "createVariants")) return false;

    q.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS profile_clubs ("
        " profile_id INTEGER NOT NULL REFERENCES profiles(profile_id),"
        " club VARCHAR(20) NOT NULL,"
        " member_nr VARCHAR(20),"
        " PRIMARY KEY (profile_id, club))"));
    if (!exec(q, "createClubs")) return false;

    // Papildu KLog My Data lauki profila (idempotenti - kluda ja jau ir, to ignorejam)
    const QStringList newCols = {
        "name VARCHAR", "address1 VARCHAR", "address2 VARCHAR", "address3 VARCHAR",
        "address4 VARCHAR", "city VARCHAR", "zip_code VARCHAR", "province VARCHAR",
        "country VARCHAR", "rig1 VARCHAR", "rig2 VARCHAR", "rig3 VARCHAR",
        "antenna1 VARCHAR", "antenna2 VARCHAR", "antenna3 VARCHAR", "power REAL",
        "clublog_email VARCHAR", "clublog_pass VARCHAR", "clublog_app_pass VARCHAR",
        "qrz_user VARCHAR", "qrz_pass VARCHAR", "qrz_logbook_key VARCHAR",
        "eqsl_call VARCHAR", "eqsl_pass VARCHAR",
        "lotw_user VARCHAR", "lotw_pass VARCHAR"
    };
    {
        QStringList have;
        q.prepare(QStringLiteral("PRAGMA table_info(profiles)"));
        if (exec(q, "profilesInfo"))
            while (q.next()) have << q.value(1).toString();
        for (const QString &def : newCols)
        {
            const QString col = def.section(' ', 0, 0);
            if (!have.contains(col))
            {
                q.prepare(QStringLiteral("ALTER TABLE profiles ADD COLUMN ") + def);
                exec(q, "addProfileCol");
            }
        }
    }

    bool hasProfileCol = false;
    q.prepare(QStringLiteral("PRAGMA table_info(log)"));
    if (exec(q, "tableInfo"))
        while (q.next())
            if (q.value(1).toString() == QLatin1String("profile_id"))
                hasProfileCol = true;

    if (!hasProfileCol)
    {
        db.transaction();
        const QStringList steps = {
            QStringLiteral("ALTER TABLE log ADD COLUMN profile_id INTEGER REFERENCES profiles(profile_id)"),
            QStringLiteral("ALTER TABLE log ADD COLUMN variant_id INTEGER REFERENCES profile_variants(variant_id)"),
            QStringLiteral(
              "INSERT OR IGNORE INTO profiles (callsign, operator_name, comment) "
              "SELECT DISTINCT CASE WHEN instr(upper(trim(stationcall)),'/')>0 "
              " THEN substr(upper(trim(stationcall)),1,instr(upper(trim(stationcall)),'/')-1) "
              " ELSE upper(trim(stationcall)) END, operators, comment FROM logs"),
            QStringLiteral(
              "INSERT OR IGNORE INTO profile_variants (profile_id, station_callsign, kind, dxcc_counts) "
              "SELECT DISTINCT p.profile_id, upper(trim(l.stationcall)), "
              " CASE WHEN upper(trim(l.stationcall)) LIKE '%/MM' THEN 'MM' "
              "      WHEN upper(trim(l.stationcall)) LIKE '%/AM' THEN 'AM' "
              "      WHEN upper(trim(l.stationcall)) LIKE '%/P'  THEN 'P' "
              "      WHEN upper(trim(l.stationcall)) LIKE '%/M'  THEN 'M' "
              "      WHEN l.logtype IS NOT NULL AND l.logtype<>'DX' THEN 'CONTEST' "
              "      ELSE 'OTHER' END, "
              " CASE WHEN upper(trim(l.stationcall)) LIKE '%/MM' THEN 0 ELSE 1 END "
              "FROM logs l JOIN profiles p ON p.callsign = "
              " CASE WHEN instr(upper(trim(l.stationcall)),'/')>0 "
              "  THEN substr(upper(trim(l.stationcall)),1,instr(upper(trim(l.stationcall)),'/')-1) "
              "  ELSE upper(trim(l.stationcall)) END "
              "WHERE upper(trim(l.stationcall)) <> p.callsign"),
            QStringLiteral(
              "UPDATE log SET "
              " profile_id = (SELECT p.profile_id FROM logs l JOIN profiles p ON p.callsign = "
              "   CASE WHEN instr(upper(trim(l.stationcall)),'/')>0 "
              "    THEN substr(upper(trim(l.stationcall)),1,instr(upper(trim(l.stationcall)),'/')-1) "
              "    ELSE upper(trim(l.stationcall)) END WHERE l.id = log.lognumber), "
              " variant_id = (SELECT v.variant_id FROM logs l JOIN profile_variants v "
              "   ON v.station_callsign = upper(trim(l.stationcall)) WHERE l.id = log.lognumber)"),
            QStringLiteral("CREATE INDEX IF NOT EXISTS idx_log_profile ON log (profile_id)")
        };
        for (const QString &s : steps) {
            q.prepare(s);
            if (!exec(q, "migrate")) {
                db.rollback();
                if (errorOut) *errorOut = q.lastError().text();
                return false;
            }
        }
        db.commit();
    }

    // Trigeris: katrs jauns QSO automatiski sanem profile_id/variant_id
    // pec lognumber -> logs.stationcall kartejuma. Darbojas visiem ievades
    // celiem (manuala ievade, ADIF imports, UDP) bez izmainam KLog koda.
    q.prepare(QStringLiteral(
        "CREATE TRIGGER IF NOT EXISTS trg_log_profile "
        "AFTER INSERT ON log "
        "WHEN NEW.profile_id IS NULL "
        "BEGIN "
        " UPDATE log SET "
        "  profile_id = (SELECT p.profile_id FROM logs l JOIN profiles p ON p.callsign = "
        "    CASE WHEN instr(upper(trim(l.stationcall)),'/')>0 "
        "     THEN substr(upper(trim(l.stationcall)),1,instr(upper(trim(l.stationcall)),'/')-1) "
        "     ELSE upper(trim(l.stationcall)) END WHERE l.id = NEW.lognumber), "
        "  variant_id = (SELECT v.variant_id FROM logs l JOIN profile_variants v "
        "    ON v.station_callsign = upper(trim(l.stationcall)) WHERE l.id = NEW.lognumber) "
        " WHERE id = NEW.id; "
        "END"));
    if (!exec(q, "createTrigger")) return false;

    return true;
}

QList<Profile> ProfileManager::listProfiles() const
{
    QList<Profile> out;
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "SELECT p.profile_id, p.callsign, p.operator_name, p.gridsquare, p.qth,"
        " p.cq_zone, p.itu_zone, p.dxcc, p.comment,"
        " (SELECT count(*) FROM log WHERE log.profile_id = p.profile_id) "
        "FROM profiles p ORDER BY p.callsign"));
    if (!exec(q, "listProfiles")) return out;
    while (q.next()) {
        Profile p;
        p.id = q.value(0).toInt();
        p.callsign = q.value(1).toString();
        p.operatorName = q.value(2).toString();
        p.gridsquare = q.value(3).toString();
        p.qth = q.value(4).toString();
        p.cqZone = q.value(5).toInt();
        p.ituZone = q.value(6).toInt();
        p.dxcc = q.value(7).toInt();
        p.comment = q.value(8).toString();
        p.qsoCount = q.value(9).toInt();
        out.append(p);
    }
    return out;
}

Profile ProfileManager::getProfile(int id) const
{
    Profile p;
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "SELECT profile_id, callsign, operator_name, gridsquare, qth,"
        " cq_zone, itu_zone, dxcc, comment, name, address1, address2, address3,"
        " address4, city, zip_code, province, country, rig1, rig2, rig3,"
        " antenna1, antenna2, antenna3, power,"
        " clublog_email, clublog_pass, clublog_app_pass,"
        " qrz_user, qrz_pass, qrz_logbook_key,"
        " eqsl_call, eqsl_pass, lotw_user, lotw_pass"
        " FROM profiles WHERE profile_id = :id"));
    q.bindValue(QStringLiteral(":id"), id);
    if (exec(q, "getProfile") && q.next()) {
        p.id = q.value(0).toInt();
        p.callsign = q.value(1).toString();
        p.operatorName = q.value(2).toString();
        p.gridsquare = q.value(3).toString();
        p.qth = q.value(4).toString();
        p.cqZone = q.value(5).toInt();
        p.ituZone = q.value(6).toInt();
        p.dxcc = q.value(7).toInt();
        p.comment = q.value(8).toString();
        p.name = q.value(9).toString();
        p.address1 = q.value(10).toString();
        p.address2 = q.value(11).toString();
        p.address3 = q.value(12).toString();
        p.address4 = q.value(13).toString();
        p.city = q.value(14).toString();
        p.zipCode = q.value(15).toString();
        p.province = q.value(16).toString();
        p.country = q.value(17).toString();
        p.rig1 = q.value(18).toString();
        p.rig2 = q.value(19).toString();
        p.rig3 = q.value(20).toString();
        p.antenna1 = q.value(21).toString();
        p.antenna2 = q.value(22).toString();
        p.antenna3 = q.value(23).toString();
        p.power = q.value(24).toDouble();
        p.clublogEmail   = q.value(25).toString();
        p.clublogPass    = q.value(26).toString();
        p.clublogAppPass = q.value(27).toString();
        p.qrzUser        = q.value(28).toString();
        p.qrzPass        = q.value(29).toString();
        p.qrzLogbookKey  = q.value(30).toString();
        p.eqslCall       = q.value(31).toString();
        p.eqslPass       = q.value(32).toString();
        p.lotwUser       = q.value(33).toString();
        p.lotwPass       = q.value(34).toString();
    }
    return p;
}

int ProfileManager::createProfile(const Profile &p)
{
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "INSERT INTO profiles (callsign, operator_name, gridsquare, qth,"
        " cq_zone, itu_zone, dxcc, comment) "
        "VALUES (:c,:o,:g,:q,:cq,:itu,:dx,:cm)"));
    q.bindValue(QStringLiteral(":c"),  p.callsign.trimmed().toUpper());
    q.bindValue(QStringLiteral(":o"),  p.operatorName);
    q.bindValue(QStringLiteral(":g"),  p.gridsquare);
    q.bindValue(QStringLiteral(":q"),  p.qth);
    q.bindValue(QStringLiteral(":cq"), p.cqZone);
    q.bindValue(QStringLiteral(":itu"),p.ituZone);
    q.bindValue(QStringLiteral(":dx"), p.dxcc);
    q.bindValue(QStringLiteral(":cm"), p.comment);
    if (!exec(q, "createProfile")) return -1;
    return q.lastInsertId().toInt();
}

bool ProfileManager::updateProfile(const Profile &p)
{
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "UPDATE profiles SET callsign=:c, operator_name=:o, gridsquare=:g,"
        " qth=:q, cq_zone=:cq, itu_zone=:itu, dxcc=:dx, comment=:cm,"
        " name=:nm, address1=:a1, address2=:a2, address3=:a3, address4=:a4,"
        " city=:ct, zip_code=:zp, province=:pv, country=:co,"
        " rig1=:r1, rig2=:r2, rig3=:r3,"
        " antenna1=:n1, antenna2=:n2, antenna3=:n3, power=:pw,"
        " clublog_email=:ce, clublog_pass=:cp, clublog_app_pass=:ca,"
        " qrz_user=:qu, qrz_pass=:qp, qrz_logbook_key=:qk,"
        " eqsl_call=:ec, eqsl_pass=:ep,"
        " lotw_user=:lu, lotw_pass=:lp "
        "WHERE profile_id=:id"));
    q.bindValue(QStringLiteral(":c"),  p.callsign.trimmed().toUpper());
    q.bindValue(QStringLiteral(":o"),  p.operatorName);
    q.bindValue(QStringLiteral(":g"),  p.gridsquare);
    q.bindValue(QStringLiteral(":q"),  p.qth);
    q.bindValue(QStringLiteral(":cq"), p.cqZone);
    q.bindValue(QStringLiteral(":itu"),p.ituZone);
    q.bindValue(QStringLiteral(":dx"), p.dxcc);
    q.bindValue(QStringLiteral(":cm"), p.comment);
    q.bindValue(QStringLiteral(":nm"), p.name);
    q.bindValue(QStringLiteral(":a1"), p.address1);
    q.bindValue(QStringLiteral(":a2"), p.address2);
    q.bindValue(QStringLiteral(":a3"), p.address3);
    q.bindValue(QStringLiteral(":a4"), p.address4);
    q.bindValue(QStringLiteral(":ct"), p.city);
    q.bindValue(QStringLiteral(":zp"), p.zipCode);
    q.bindValue(QStringLiteral(":pv"), p.province);
    q.bindValue(QStringLiteral(":co"), p.country);
    q.bindValue(QStringLiteral(":r1"), p.rig1);
    q.bindValue(QStringLiteral(":r2"), p.rig2);
    q.bindValue(QStringLiteral(":r3"), p.rig3);
    q.bindValue(QStringLiteral(":n1"), p.antenna1);
    q.bindValue(QStringLiteral(":n2"), p.antenna2);
    q.bindValue(QStringLiteral(":n3"), p.antenna3);
    q.bindValue(QStringLiteral(":pw"), p.power);
    q.bindValue(QStringLiteral(":ce"), p.clublogEmail);
    q.bindValue(QStringLiteral(":cp"), p.clublogPass);
    q.bindValue(QStringLiteral(":ca"), p.clublogAppPass);
    q.bindValue(QStringLiteral(":qu"), p.qrzUser);
    q.bindValue(QStringLiteral(":qp"), p.qrzPass);
    q.bindValue(QStringLiteral(":qk"), p.qrzLogbookKey);
    q.bindValue(QStringLiteral(":ec"), p.eqslCall);
    q.bindValue(QStringLiteral(":ep"), p.eqslPass);
    q.bindValue(QStringLiteral(":lu"), p.lotwUser);
    q.bindValue(QStringLiteral(":lp"), p.lotwPass);
    q.bindValue(QStringLiteral(":id"), p.id);
    return exec(q, "updateProfile");
}

bool ProfileManager::deleteProfile(int id, QString *errorOut)
{
    const int n = qsoCount(id);
    if (n > 0) {
        if (errorOut)
            *errorOut = QObject::tr("The profile holds %1 QSOs. Export them to ADIF "
                                    "or move them to another profile first.").arg(n);
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery q(db);
    q.prepare(QStringLiteral("DELETE FROM profile_variants WHERE profile_id=:id"));
    q.bindValue(QStringLiteral(":id"), id);
    if (!exec(q, "deleteVariantsOfProfile")) { db.rollback(); return false; }
    q.prepare(QStringLiteral("DELETE FROM profiles WHERE profile_id=:id"));
    q.bindValue(QStringLiteral(":id"), id);
    if (!exec(q, "deleteProfile")) { db.rollback(); return false; }
    db.commit();
    return true;
}

int ProfileManager::qsoCount(int profileId) const
{
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral("SELECT count(*) FROM log WHERE profile_id=:id"));
    q.bindValue(QStringLiteral(":id"), profileId);
    if (exec(q, "qsoCount") && q.next())
        return q.value(0).toInt();
    return 0;
}

QList<ProfileVariant> ProfileManager::listVariants(int profileId) const
{
    QList<ProfileVariant> out;
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "SELECT variant_id, profile_id, station_callsign, kind, gridsquare,"
        " dxcc_counts FROM profile_variants WHERE profile_id=:id "
        "ORDER BY station_callsign"));
    q.bindValue(QStringLiteral(":id"), profileId);
    if (!exec(q, "listVariants")) return out;
    while (q.next()) {
        ProfileVariant v;
        v.id = q.value(0).toInt();
        v.profileId = q.value(1).toInt();
        v.stationCallsign = q.value(2).toString();
        v.kind = q.value(3).toString();
        v.gridsquare = q.value(4).toString();
        v.dxccCounts = q.value(5).toInt() != 0;
        out.append(v);
    }
    return out;
}

int ProfileManager::createVariant(const ProfileVariant &v)
{
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "INSERT INTO profile_variants (profile_id, station_callsign, kind,"
        " gridsquare, dxcc_counts) VALUES (:p,:s,:k,:g,:d)"));
    q.bindValue(QStringLiteral(":p"), v.profileId);
    q.bindValue(QStringLiteral(":s"), v.stationCallsign.trimmed().toUpper());
    q.bindValue(QStringLiteral(":k"), v.kind);
    q.bindValue(QStringLiteral(":g"), v.gridsquare);
    q.bindValue(QStringLiteral(":d"), v.dxccCounts ? 1 : 0);
    if (!exec(q, "createVariant")) return -1;
    return q.lastInsertId().toInt();
}

bool ProfileManager::deleteVariant(int variantId)
{
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral("DELETE FROM profile_variants WHERE variant_id=:id"));
    q.bindValue(QStringLiteral(":id"), variantId);
    return exec(q, "deleteVariant");
}


bool ProfileManager::saveSettingsToProfile(int profileId, const QString &cfgFile)
{
    if (profileId < 0) return false;
    Profile p = getProfile(profileId);
    if (p.id < 0) return false;

    QSettings st(cfgFile, QSettings::IniFormat);
    st.beginGroup(QStringLiteral("UserData"));
    const QString call = st.value(QStringLiteral("Callsign")).toString().trimmed().toUpper();
    // zimi nemainam - tas ir profila identitate
    if (!call.isEmpty() && call != p.callsign)
    {
        st.endGroup();
        return false;   // Setup pusē nomainita zime - neparrakstam profilu
    }
    p.operatorName = st.value(QStringLiteral("Operators")).toString();
    p.name         = st.value(QStringLiteral("Name")).toString();
    p.gridsquare   = st.value(QStringLiteral("StationLocator")).toString();
    p.cqZone       = st.value(QStringLiteral("CQz")).toInt();
    p.ituZone      = st.value(QStringLiteral("ITUz")).toInt();
    p.address1     = st.value(QStringLiteral("Address1")).toString();
    p.address2     = st.value(QStringLiteral("Address2")).toString();
    p.address3     = st.value(QStringLiteral("Address3")).toString();
    p.address4     = st.value(QStringLiteral("Address4")).toString();
    p.city         = st.value(QStringLiteral("City")).toString();
    p.zipCode      = st.value(QStringLiteral("ZipCode")).toString();
    p.province     = st.value(QStringLiteral("ProvinceState")).toString();
    p.country      = st.value(QStringLiteral("Country")).toString();
    p.rig1         = st.value(QStringLiteral("Rig1")).toString();
    p.rig2         = st.value(QStringLiteral("Rig2")).toString();
    p.rig3         = st.value(QStringLiteral("Rig3")).toString();
    p.antenna1     = st.value(QStringLiteral("Antenna1")).toString();
    p.antenna2     = st.value(QStringLiteral("Antenna2")).toString();
    p.antenna3     = st.value(QStringLiteral("Antenna3")).toString();
    p.power        = st.value(QStringLiteral("Power")).toDouble();
    st.endGroup();

    st.beginGroup(QStringLiteral("ClubLog"));
    p.clublogEmail   = st.value(QStringLiteral("ClubLogEmail")).toString();
    p.clublogPass    = st.value(QStringLiteral("ClubLogPass")).toString();
    p.clublogAppPass = st.value(QStringLiteral("ClubLogAppPass")).toString();
    st.endGroup();

    st.beginGroup(QStringLiteral("QRZcom"));
    p.qrzUser       = st.value(QStringLiteral("QRZcomUser")).toString();
    p.qrzPass       = st.value(QStringLiteral("QRZcomPass")).toString();
    p.qrzLogbookKey = st.value(QStringLiteral("QRZcomLogBookKey")).toString();
    st.endGroup();

    st.beginGroup(QStringLiteral("eQSL"));
    p.eqslCall = st.value(QStringLiteral("eQSLCall")).toString();
    p.eqslPass = st.value(QStringLiteral("eQSLPass")).toString();
    st.endGroup();

    st.beginGroup(QStringLiteral("LoTW"));
    p.lotwUser = st.value(QStringLiteral("LoTWUser")).toString();
    p.lotwPass = st.value(QStringLiteral("LoTWPass")).toString();
    st.endGroup();

    return updateProfile(p);
}


QList<ProfileClub> ProfileManager::listClubs(int profileId) const
{
    QList<ProfileClub> out;
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "SELECT profile_id, club, member_nr FROM profile_clubs "
        "WHERE profile_id=:id ORDER BY club"));
    q.bindValue(QStringLiteral(":id"), profileId);
    if (!exec(q, "listClubs")) return out;
    while (q.next()) {
        ProfileClub c;
        c.profileId = q.value(0).toInt();
        c.club      = q.value(1).toString();
        c.memberNr  = q.value(2).toString();
        out.append(c);
    }
    return out;
}

bool ProfileManager::setClubs(int profileId, const QList<ProfileClub> &clubs)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery q(db);
    q.prepare(QStringLiteral("DELETE FROM profile_clubs WHERE profile_id=:id"));
    q.bindValue(QStringLiteral(":id"), profileId);
    if (!exec(q, "clearClubs")) { db.rollback(); return false; }
    for (const ProfileClub &c : clubs) {
        if (c.club.trimmed().isEmpty()) continue;
        q.prepare(QStringLiteral(
            "INSERT INTO profile_clubs (profile_id, club, member_nr) "
            "VALUES (:p, :c, :n)"));
        q.bindValue(QStringLiteral(":p"), profileId);
        q.bindValue(QStringLiteral(":c"), c.club.trimmed().toUpper());
        q.bindValue(QStringLiteral(":n"), c.memberNr.trimmed());
        if (!exec(q, "insertClub")) { db.rollback(); return false; }
    }
    db.commit();
    return true;
}
