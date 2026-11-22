#include "startprofiledialog.h"
#include "world.h"
#include "setupdialog.h"
#include "dataproxy_sqlite.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QApplication>
#include <QStyleFactory>

static void forceLightPalette(QWidget *w)
{
    QPalette p;
    p.setColor(QPalette::Window,          QColor(0xf0,0xf0,0xf0));
    p.setColor(QPalette::WindowText,      Qt::black);
    p.setColor(QPalette::Base,            Qt::white);
    p.setColor(QPalette::AlternateBase,   QColor(0xe9,0xe9,0xe9));
    p.setColor(QPalette::Text,            Qt::black);
    p.setColor(QPalette::Button,          QColor(0xf0,0xf0,0xf0));
    p.setColor(QPalette::ButtonText,      Qt::black);
    p.setColor(QPalette::Highlight,       QColor(0x30,0x8c,0xc6));
    p.setColor(QPalette::HighlightedText, Qt::white);
    p.setColor(QPalette::ToolTipBase,     Qt::white);
    p.setColor(QPalette::ToolTipText,     Qt::black);
    w->setPalette(p);
}

static const char *SETT_OPEN_LAST   = "profiles/openLastOnStart";
static const char *SETT_LAST_PROFILE= "profiles/lastProfileId";

#include <QDir>
// Tas pats ini fails, ko lieto KLog (Utilities::getCfgFile ekvivalents)
#include <QFile>
#include <QTextStream>

// Ieraksta aktiva profila vardu ~/.klogng/active-profile un nodrosina,
// ka profilam ir savs klogrc (pirmaja reize nokope esoso).
static void writeActiveProfile(const QString &callsign)
{
    const QString home = QDir::homePath() + QStringLiteral("/.klogng");
    QFile ap(home + QStringLiteral("/active-profile"));
    if (ap.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&ap);
        out << callsign;
        ap.close();
    }
    const QString pdir = home + QStringLiteral("/profiles/") + callsign;
    QDir().mkpath(pdir);
    const QString pcfg = pdir + QStringLiteral("/klogrc");
    if (!QFile::exists(pcfg))
        QFile::copy(home + QStringLiteral("/klogrc"), pcfg);
}

static QString klogngCfgFile()
{
    const QString home = QDir::homePath() + QStringLiteral("/.klogng");
    QFile ap(home + QStringLiteral("/active-profile"));
    if (ap.exists() && ap.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        const QString n = QString::fromUtf8(ap.readAll()).trimmed();
        ap.close();
        if (!n.isEmpty())
            return home + QStringLiteral("/profiles/") + n + QStringLiteral("/klogrc");
    }
    return home + QStringLiteral("/klogrc");
}

StartProfileDialog::StartProfileDialog(ProfileManager *pm_, World *world_, DataProxy_SQLite *dp_, QWidget *parent)
    : QDialog(parent), pm(pm_), world(world_), dataProxy(dp_)
{
    setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    forceLightPalette(this);
    setWindowTitle(tr("Select profile"));
    setModal(true);
    resize(420, 380);

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({tr("Callsign"), tr("QSOs"), tr("Note")});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(table, &QTableWidget::cellDoubleClicked,
            this, [this](int, int){ openSelected(); });

    openBtn   = new QPushButton(tr("Open"), this);
    auto *newBtn = new QPushButton(tr("New profile"), this);
    editBtn   = new QPushButton(tr("Edit"), this);
    deleteBtn = new QPushButton(tr("Delete"), this);
    auto *cancelBtn = new QPushButton(tr("Cancel"), this);
    openBtn->setDefault(true);

    connect(openBtn,   &QPushButton::clicked, this, &StartProfileDialog::openSelected);
    connect(newBtn,    &QPushButton::clicked, this, &StartProfileDialog::newProfile);
    connect(editBtn,   &QPushButton::clicked, this, &StartProfileDialog::editSelected);
    connect(deleteBtn, &QPushButton::clicked, this, &StartProfileDialog::deleteSelected);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    openLastCheck = new QCheckBox(tr("Open this profile automatically next time"), this);
    QSettings sett(klogngCfgFile(), QSettings::IniFormat);
    openLastCheck->setChecked(sett.value(QLatin1String(SETT_OPEN_LAST), false).toBool());

    auto *btnCol = new QVBoxLayout;
    btnCol->addWidget(openBtn);
    btnCol->addWidget(cancelBtn);
    btnCol->addSpacing(16);
    btnCol->addWidget(newBtn);
    btnCol->addWidget(editBtn);
    btnCol->addWidget(deleteBtn);
    btnCol->addStretch();

    auto *mid = new QHBoxLayout;
    mid->addWidget(table, 1);
    mid->addLayout(btnCol);

    auto *root = new QVBoxLayout(this);
    root->addLayout(mid, 1);
    root->addWidget(openLastCheck);

    reload();
}

void StartProfileDialog::reload()
{
    const QList<Profile> profiles = pm->listProfiles();
    table->setRowCount(profiles.size());
    int row = 0;
    for (const Profile &p : profiles) {
        auto *c0 = new QTableWidgetItem(p.callsign);
        c0->setData(Qt::UserRole, p.id);
        table->setItem(row, 0, c0);
        table->setItem(row, 1, new QTableWidgetItem(QString::number(p.qsoCount)));
        table->setItem(row, 2, new QTableWidgetItem(p.comment));
        ++row;
    }
    const bool any = !profiles.isEmpty();
    openBtn->setEnabled(any);
    editBtn->setEnabled(any);
    deleteBtn->setEnabled(any);
    if (any)
        table->selectRow(0);
}

int StartProfileDialog::currentRowProfileId() const
{
    const int row = table->currentRow();
    if (row < 0) return -1;
    return table->item(row, 0)->data(Qt::UserRole).toInt();
}

void StartProfileDialog::openSelected()
{
    const int id = currentRowProfileId();
    if (id < 0) return;
    selectedId = id;
    QSettings sett(klogngCfgFile(), QSettings::IniFormat);
    sett.setValue(QLatin1String(SETT_OPEN_LAST), openLastCheck->isChecked());
    sett.setValue(QLatin1String(SETT_LAST_PROFILE), id);
    accept();
}

#include <QTabWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>

static bool editProfileDialog(QWidget *parent, Profile &p, const QString &title, World *world, ProfileManager *pm, DataProxy_SQLite *dp)

{
    QDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.resize(520, 460);

    auto *callP = new QLineEdit(p.callsign, &dlg);
    auto *operP = new QLineEdit(p.operatorName, &dlg);
    auto *gridP = new QLineEdit(p.gridsquare, &dlg);
    auto *qthP = new QLineEdit(p.qth, &dlg);
    auto *commentP = new QLineEdit(p.comment, &dlg);
    auto *nameP = new QLineEdit(p.name, &dlg);
    QLineEdit &call = *callP, &oper = *operP, &grid = *gridP,
              &qth = *qthP, &comment = *commentP, &name = *nameP;
    auto *cqzP = new QSpinBox(&dlg); auto *ituzP = new QSpinBox(&dlg);
    QSpinBox &cqz = *cqzP, &ituz = *ituzP;
    cqz.setRange(0, 40); cqz.setValue(p.cqZone);
    ituz.setRange(0, 90); ituz.setValue(p.ituZone);

    auto *a1P=new QLineEdit(p.address1,&dlg); auto *a2P=new QLineEdit(p.address2,&dlg);
    auto *a3P=new QLineEdit(p.address3,&dlg); auto *a4P=new QLineEdit(p.address4,&dlg);
    auto *cityP=new QLineEdit(p.city,&dlg); auto *zipP=new QLineEdit(p.zipCode,&dlg);
    auto *provP=new QLineEdit(p.province,&dlg); auto *countryP=new QLineEdit(p.country,&dlg);
    QLineEdit &a1=*a1P,&a2=*a2P,&a3=*a3P,&a4=*a4P,&city=*cityP,&zip=*zipP,&prov=*provP,&country=*countryP;

    auto *r1P=new QLineEdit(p.rig1,&dlg); auto *r2P=new QLineEdit(p.rig2,&dlg);
    auto *r3P=new QLineEdit(p.rig3,&dlg); auto *n1P=new QLineEdit(p.antenna1,&dlg);
    auto *n2P=new QLineEdit(p.antenna2,&dlg); auto *n3P=new QLineEdit(p.antenna3,&dlg);
    QLineEdit &r1=*r1P,&r2=*r2P,&r3=*r3P,&n1=*n1P,&n2=*n2P,&n3=*n3P;
    auto *pwrP = new QDoubleSpinBox(&dlg); QDoubleSpinBox &pwr = *pwrP;
    pwr.setRange(0, 10000); pwr.setDecimals(1); pwr.setSuffix(" W"); pwr.setValue(p.power);

    // Automatiska zonu/DXCC aizpilde pec zimes.
    // /MM un /AM: nav DXCC, neko neaizpildam (kustigi objekti).
    auto fillFromCallsign = [&call, &cqz, &ituz, &country, world]() {
            if (!world) return;
            const QString c = call.text().trimmed().toUpper();
            if (c.isEmpty()) return;
            if (c.endsWith(QStringLiteral("/MM")) || c.endsWith(QStringLiteral("/AM")))
                return;                       // juras/gaisa stacija - bez DXCC
            const int cq  = world->getQRZCqz(c);
            const int itu = world->getQRZItuz(c);
            const int dx  = world->getQRZARRLId(c);
            if (dx < 0) return;               // prefikss nav atpazits
            if (cqz.value() == 0 && cq > 0)   cqz.setValue(cq);
            if (ituz.value() == 0 && itu > 0) ituz.setValue(itu);
            if (country.text().trimmed().isEmpty())
                country.setText(world->getQRZEntityName(c));
    };
    QObject::connect(&call, &QLineEdit::editingFinished, fillFromCallsign);
    fillFromCallsign();   // uzreiz ari atverot dialogu (tuksiem laukiem)

    // Automatiska zonu/DXCC aizpilde pec zimes.
    // /MM un /AM: nav DXCC, neko neaizpildam (kustigi objekti).


    auto *tabsP = new QTabWidget(&dlg); QTabWidget &tabs = *tabsP;

    auto *wStationP = new QWidget(&dlg); QWidget &wStation = *wStationP; QFormLayout fs(&wStation);
    fs.addRow(QObject::tr("Callsign:"), &call);
    fs.addRow(QObject::tr("Operator name:"), &name);
    fs.addRow(QObject::tr("Operator callsigns:"), &oper);
    fs.addRow(QObject::tr("Locator:"), &grid);
    fs.addRow(QObject::tr("QTH:"), &qth);
    fs.addRow(QObject::tr("CQ zone:"), &cqz);
    fs.addRow(QObject::tr("ITU zone:"), &ituz);
    fs.addRow(QObject::tr("Note:"), &comment);
    tabs.addTab(&wStation, QObject::tr("Station"));

    auto *wAddrP = new QWidget(&dlg); QWidget &wAddr = *wAddrP; QFormLayout fa(&wAddr);
    fa.addRow(QObject::tr("Address 1:"), &a1);
    fa.addRow(QObject::tr("Address 2:"), &a2);
    fa.addRow(QObject::tr("Address 3:"), &a3);
    fa.addRow(QObject::tr("Address 4:"), &a4);
    fa.addRow(QObject::tr("City:"), &city);
    fa.addRow(QObject::tr("Postal code:"), &zip);
    fa.addRow(QObject::tr("Province/State:"), &prov);
    fa.addRow(QObject::tr("Country:"), &country);
    tabs.addTab(&wAddr, QObject::tr("Address"));

    auto *wRigP = new QWidget(&dlg); QWidget &wRig = *wRigP; QFormLayout fr(&wRig);
    fr.addRow(QObject::tr("Rig 1:"), &r1);
    fr.addRow(QObject::tr("Rig 2:"), &r2);
    fr.addRow(QObject::tr("Rig 3:"), &r3);
    fr.addRow(QObject::tr("Antenna 1:"), &n1);
    fr.addRow(QObject::tr("Antenna 2:"), &n2);
    fr.addRow(QObject::tr("Antenna 3:"), &n3);
    fr.addRow(QObject::tr("Power:"), &pwr);
    tabs.addTab(&wRig, QObject::tr("Equipment"));

    // --- Klubi ---
    auto *wClub = new QWidget(&dlg);
    auto *clubTable = new QTableWidget(wClub);
    clubTable->setColumnCount(2);
    clubTable->setHorizontalHeaderLabels({QObject::tr("Club"), QObject::tr("Member number")});
    clubTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    clubTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    auto *addClub = new QPushButton(QObject::tr("Add"), wClub);
    auto *delClub = new QPushButton(QObject::tr("Remove"), wClub);
    QObject::connect(addClub, &QPushButton::clicked, [clubTable]() {
        const int r = clubTable->rowCount();
        clubTable->insertRow(r);
        clubTable->setItem(r, 0, new QTableWidgetItem());
        clubTable->setItem(r, 1, new QTableWidgetItem());
        clubTable->editItem(clubTable->item(r, 0));
    });
    QObject::connect(delClub, &QPushButton::clicked, [clubTable]() {
        const int r = clubTable->currentRow();
        if (r >= 0) clubTable->removeRow(r);
    });
    auto *clubBtns = new QHBoxLayout;
    clubBtns->addWidget(addClub);
    clubBtns->addWidget(delClub);
    clubBtns->addStretch();
    auto *clubLay = new QVBoxLayout(wClub);
    clubLay->addWidget(clubTable, 1);
    clubLay->addLayout(clubBtns);
    if (pm && p.id > 0) {
        const QList<ProfileClub> cl = pm->listClubs(p.id);
        for (const ProfileClub &c : cl) {
            const int r = clubTable->rowCount();
            clubTable->insertRow(r);
            clubTable->setItem(r, 0, new QTableWidgetItem(c.club));
            clubTable->setItem(r, 1, new QTableWidgetItem(c.memberNr));
        }
    }
    tabs.addTab(wClub, QObject::tr("Clubs"));

    // Poga uz pilno KLog konfiguraciju
    auto *fullCfgBtn = new QPushButton(QObject::tr("Full settings..."), &dlg);
    fullCfgBtn->setEnabled(dp != nullptr);
    QObject::connect(fullCfgBtn, &QPushButton::clicked, [&dlg, dp, world, pm, &p]() {
        if (!dp) return;
        SetupDialog sd(dp, world, &dlg);
        sd.init(QString(), 0, true);
        sd.exec();
        // pec aizversanas nolasam UserData atpakal profila
        if (pm && p.id > 0)
            pm->saveSettingsToProfile(p.id, QDir::homePath() + QStringLiteral("/.klogng/klogrc"));
    });

    auto *boxP = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    QDialogButtonBox &box = *boxP;
    QObject::connect(&box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(&box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    auto *bottom = new QHBoxLayout;
    bottom->addWidget(fullCfgBtn);
    bottom->addStretch();
    bottom->addWidget(&box);

    QVBoxLayout root(&dlg);
    root.addWidget(&tabs);
    root.addLayout(bottom);

    if (dlg.exec() != QDialog::Accepted) return false;
    if (call.text().trimmed().isEmpty()) return false;

    p.callsign = call.text().trimmed().toUpper();
    p.operatorName = oper.text();
    p.name = name.text();
    p.gridsquare = grid.text().trimmed().toUpper();
    p.qth = qth.text();
    p.cqZone = cqz.value();
    p.ituZone = ituz.value();
    p.comment = comment.text();
    p.address1 = a1.text();  p.address2 = a2.text();
    p.address3 = a3.text();  p.address4 = a4.text();
    p.city = city.text();    p.zipCode = zip.text();
    p.province = prov.text(); p.country = country.text();
    p.rig1 = r1.text();  p.rig2 = r2.text();  p.rig3 = r3.text();
    p.antenna1 = n1.text(); p.antenna2 = n2.text(); p.antenna3 = n3.text();
    p.power = pwr.value();

    if (pm && p.id > 0) {
        QList<ProfileClub> cl;
        for (int r = 0; r < clubTable->rowCount(); ++r) {
            ProfileClub c;
            c.profileId = p.id;
            c.club     = clubTable->item(r,0) ? clubTable->item(r,0)->text() : QString();
            c.memberNr = clubTable->item(r,1) ? clubTable->item(r,1)->text() : QString();
            if (!c.club.trimmed().isEmpty()) cl.append(c);
        }
        pm->setClubs(p.id, cl);
    }
    return true;
}

void StartProfileDialog::newProfile()
{
    Profile p;
    if (!editProfileDialog(this, p, tr("New profile"), world, pm, dataProxy))
        return;
    if (pm->createProfile(p) < 0)
        QMessageBox::warning(this, tr("Error"),
            tr("Could not create the profile. Does callsign %1 already exist?").arg(p.callsign));
    reload();
}

void StartProfileDialog::editSelected()
{
    const int id = currentRowProfileId();
    if (id < 0) return;
    Profile p = pm->getProfile(id);
    if (!editProfileDialog(this, p, tr("Edit profile %1").arg(p.callsign), world, pm, dataProxy))
        return;
    pm->updateProfile(p);
    reload();
}

void StartProfileDialog::deleteSelected()
{
    const int id = currentRowProfileId();
    if (id < 0) return;
    const Profile p = pm->getProfile(id);
    const int n = pm->qsoCount(id);
    if (QMessageBox::question(this, tr("Delete profile?"),
            tr("Delete profile %1 (%2 QSOs)?").arg(p.callsign).arg(n))
        != QMessageBox::Yes)
        return;
    QString err;
    if (!pm->deleteProfile(id, &err))
        QMessageBox::warning(this, tr("Cannot delete"), err);
    reload();
}

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariant>
#include <QDate>

// Uzstada visu profila datu komplektu KLog konfigura (UserData grupa)
static void applyProfileToSettings(QSettings &st, const Profile &ap)
{
    st.beginGroup(QStringLiteral("UserData"));
    if (!ap.callsign.isEmpty())     st.setValue(QStringLiteral("Callsign"), ap.callsign);
    if (!ap.gridsquare.isEmpty())   st.setValue(QStringLiteral("StationLocator"), ap.gridsquare);
    if (!ap.operatorName.isEmpty()) st.setValue(QStringLiteral("Operators"), ap.operatorName);
    if (!ap.name.isEmpty())         st.setValue(QStringLiteral("Name"), ap.name);
    if (ap.cqZone > 0)              st.setValue(QStringLiteral("CQz"), ap.cqZone);
    if (ap.ituZone > 0)             st.setValue(QStringLiteral("ITUz"), ap.ituZone);
    if (!ap.address1.isEmpty())     st.setValue(QStringLiteral("Address1"), ap.address1);
    if (!ap.address2.isEmpty())     st.setValue(QStringLiteral("Address2"), ap.address2);
    if (!ap.address3.isEmpty())     st.setValue(QStringLiteral("Address3"), ap.address3);
    if (!ap.address4.isEmpty())     st.setValue(QStringLiteral("Address4"), ap.address4);
    if (!ap.city.isEmpty())         st.setValue(QStringLiteral("City"), ap.city);
    if (!ap.zipCode.isEmpty())      st.setValue(QStringLiteral("ZipCode"), ap.zipCode);
    if (!ap.province.isEmpty())     st.setValue(QStringLiteral("ProvinceState"), ap.province);
    if (!ap.country.isEmpty())      st.setValue(QStringLiteral("Country"), ap.country);
    if (!ap.rig1.isEmpty())         st.setValue(QStringLiteral("Rig1"), ap.rig1);
    if (!ap.rig2.isEmpty())         st.setValue(QStringLiteral("Rig2"), ap.rig2);
    if (!ap.rig3.isEmpty())         st.setValue(QStringLiteral("Rig3"), ap.rig3);
    if (!ap.antenna1.isEmpty())     st.setValue(QStringLiteral("Antenna1"), ap.antenna1);
    if (!ap.antenna2.isEmpty())     st.setValue(QStringLiteral("Antenna2"), ap.antenna2);
    if (!ap.antenna3.isEmpty())     st.setValue(QStringLiteral("Antenna3"), ap.antenna3);
    if (ap.power > 0)               st.setValue(QStringLiteral("Power"), ap.power);
    st.endGroup();

    st.beginGroup(QStringLiteral("ClubLog"));
    if (!ap.clublogEmail.isEmpty())   st.setValue(QStringLiteral("ClubLogEmail"), ap.clublogEmail);
    if (!ap.clublogPass.isEmpty())    st.setValue(QStringLiteral("ClubLogPass"), ap.clublogPass);
    if (!ap.clublogAppPass.isEmpty()) st.setValue(QStringLiteral("ClubLogAppPass"), ap.clublogAppPass);
    st.endGroup();

    st.beginGroup(QStringLiteral("QRZcom"));
    if (!ap.qrzUser.isEmpty())       st.setValue(QStringLiteral("QRZcomUser"), ap.qrzUser);
    if (!ap.qrzPass.isEmpty())       st.setValue(QStringLiteral("QRZcomPass"), ap.qrzPass);
    if (!ap.qrzLogbookKey.isEmpty()) st.setValue(QStringLiteral("QRZcomLogBookKey"), ap.qrzLogbookKey);
    st.endGroup();

    st.beginGroup(QStringLiteral("eQSL"));
    if (!ap.eqslCall.isEmpty()) st.setValue(QStringLiteral("eQSLCall"), ap.eqslCall);
    if (!ap.eqslPass.isEmpty()) st.setValue(QStringLiteral("eQSLPass"), ap.eqslPass);
    st.endGroup();

    st.beginGroup(QStringLiteral("LoTW"));
    if (!ap.lotwUser.isEmpty()) st.setValue(QStringLiteral("LoTWUser"), ap.lotwUser);
    if (!ap.lotwPass.isEmpty()) st.setValue(QStringLiteral("LoTWPass"), ap.lotwPass);
    st.endGroup();
}

static int lognumberForProfile(ProfileManager *pm, int profileId)
{
    const Profile p = pm->getProfile(profileId);
    if (p.id < 0) return -1;
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(QStringLiteral(
        "SELECT id FROM logs WHERE upper(trim(stationcall)) = :c "
        "ORDER BY id LIMIT 1"));
    q.bindValue(QStringLiteral(":c"), p.callsign);
    if (q.exec() && q.next())
        return q.value(0).toInt();
    // logs ieraksta nav (jauns profils) - izveidojam
    q.prepare(QStringLiteral(
        "INSERT INTO logs (logdate, stationcall, logtype, logtypen) "
        "VALUES (:d, :c, 'DX', 1)"));
    q.bindValue(QStringLiteral(":d"), QDate::currentDate().toString(QStringLiteral("yyyy-MM-dd")));
    q.bindValue(QStringLiteral(":c"), p.callsign);
    if (q.exec())
        return q.lastInsertId().toInt();
    return -1;
}

int StartProfileDialog::chooseProfileOnStartup(ProfileManager *pm, World *world, DataProxy_SQLite *dp, QWidget *parent)
{
    qApp->setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    QPalette lp;
    lp.setColor(QPalette::Window,          QColor(0xf0,0xf0,0xf0));
    lp.setColor(QPalette::WindowText,      Qt::black);
    lp.setColor(QPalette::Base,            Qt::white);
    lp.setColor(QPalette::AlternateBase,   QColor(0xe9,0xe9,0xe9));
    lp.setColor(QPalette::Text,            Qt::black);
    lp.setColor(QPalette::Button,          QColor(0xf0,0xf0,0xf0));
    lp.setColor(QPalette::ButtonText,      Qt::black);
    lp.setColor(QPalette::Highlight,       QColor(0x30,0x8c,0xc6));
    lp.setColor(QPalette::HighlightedText, Qt::white);
    lp.setColor(QPalette::ToolTipBase,     Qt::white);
    lp.setColor(QPalette::ToolTipText,     Qt::black);
    lp.setColor(QPalette::PlaceholderText, QColor(0x80,0x80,0x80));
    qApp->setPalette(lp);

    QSettings sett(klogngCfgFile(), QSettings::IniFormat);
    if (sett.value(QLatin1String(SETT_OPEN_LAST), false).toBool()) {
        const int last = sett.value(QLatin1String(SETT_LAST_PROFILE), -1).toInt();
        if (last > 0 && pm->getProfile(last).id == last)
        {
            const int ln = lognumberForProfile(pm, last);
            if (ln > 0)
            {
                const Profile ap = pm->getProfile(last);
                writeActiveProfile(ap.callsign);
                QSettings s2(klogngCfgFile(), QSettings::IniFormat);
                s2.setValue(QStringLiteral("SelectedLog"), ln);
                applyProfileToSettings(s2, ap);
                s2.sync();
            }
            return last;
        }
    }
    StartProfileDialog dlg(pm, world, dp, parent);
    if (dlg.exec() == QDialog::Accepted)
    {
        const int ln = lognumberForProfile(pm, dlg.selectedProfileId());
        if (ln > 0)
        {
            const Profile ap = pm->getProfile(dlg.selectedProfileId());
            writeActiveProfile(ap.callsign);
            QSettings s2(klogngCfgFile(), QSettings::IniFormat);
            s2.setValue(QStringLiteral("SelectedLog"), ln);
            applyProfileToSettings(s2, ap);
            s2.sync();
        }
        return dlg.selectedProfileId();
    }
    return -1;
}
