/***************************************************************************
                          utilities.cpp  -  description
                             -------------------
    begin                : jun 2015
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
#include "utilities.h"
#include "callsign.h"
#include <QRegularExpression>
//bool c;
Utilities::Utilities(const QString &_parentName)
{
    // qDebug() << Q_FUNC_INFO << " - Start";
    parentName = _parentName;
    // qDebug() << Q_FUNC_INFO << " (" << _parentName << ")";
    init();
    // qDebug() << Q_FUNC_INFO << " - END";
}

Utilities::~Utilities(){}

void Utilities::init()
{
    // qDebug() << Q_FUNC_INFO << " - Start";
    validateCalls = false;
    softwareVersion = "0.0";

    //darkMode = false;
    logLevel = None;

    setARRLSect();
    setContinent();
    setSponsorsList();
    logLevels.clear();
    logLevels << "None" << "Info" << "Debug" << "Devel";
    setLogColumnNames();

    // qDebug() << Q_FUNC_INFO << " - END";
}

void Utilities::InitializeHash() {
    ADIFHash = {
        {"ADDRESS", "MultiLineString"},
        {"AGE", "Number"},
        {"ALTITUDE", "Number"},
        {"ANT_AZ", "Number"},
        {"ANT_EL", "Number"},
        {"ANT_PATH", "Enumeration"},
        {"ARRL_SECT", "Enumeration"},
        {"AWARD_SUBMITTED", "SponsoredAwardList"},
        {"AWARD_GRANTED", "SponsoredAwardList"},
        {"A_INDEX", "Number"},
        {"BAND", "Enumeration"},
        {"BAND_RX", "Enumeration"},
        {"CALL", "String"},
        {"CHECK", "String"},
        {"CLASS", "String"},
        {"CLUBLOG_QSO_UPLOAD_DATE", "Date"},
        {"CLUBLOG_QSO_UPLOAD_STATUS", "Enumeration"},
        {"CNTY", "Enumeration"},
        {"COMMENT", "String"},
        {"CONT", "Enumeration"},
        {"CONTACTED_OP", "String"},
        {"CONTEST_ID", "String"},
        {"COUNTRY", "String"},
        {"CQZ", "PositiveInteger"},
        {"CREDIT_SUBMITTED", "CreditList"},
        {"CREDIT_GRANTED", "CreditList"},
        {"DARC_DOK", "Enumeration"},
        {"DISTANCE", "Number"},
        {"DXCC", "Enumeration"},
        {"EMAIL", "String"},
        {"EQ_CALL", "String"},
        {"EQSL_QSLRDATE", "Date"},
        {"EQSL_QSLSDATE", "Date"},
        {"EQSL_QSL_RCVD", "Enumeration"},
        {"EQSL_QSL_SENT", "Enumeration"},
        {"FISTS", "PositiveInteger"},
        {"FISTS_CC", "PositiveInteger"},
        {"FORCE_INIT", "Boolean"},
        {"FREQ", "Number"},
        {"FREQ_RX", "Number"},
        {"GRIDSQUARE", "GridSquare"},
        {"GRIDSQUARE_EXT", "GridSquareExt"},
        {"GUEST_OP", "String"},
        {"HAMLOGEU_QSO_UPLOAD_DATE", "Date"},
        {"HAMLOGEU_UPLOAD_STATUS", "Enumeration"},
        {"HAMQTH_QSO_UPLOAD_DATE", "Date"},
        {"HAMQTH_UPLOAD_STATUS", "Enumeration"},
        {"HRDLOG_QSO_UPLOAD_DATE", "Date"},
        {"HRDLOG_QSO_UPLOAD_STATUS", "Enumeration"},
        {"IOTA", "IOTARefNo"},
        {"IOTA_ISLAND_ID", "PositiveInteger"},
        {"ITUZ", "PositiveInteger"},
        {"K_INDEX", "Integer"},
        {"LAT", "Location"},
        {"LON", "Location"},
        {"LOTW_QSLRDATE", "Date"},
        {"LOTW_QSLSDATE", "Date"},
        {"LOTW_QSL_RCVD", "Enumeration"},
        {"LOTW_QSL_SENT", "Enumeration"},
        {"MAX_BURSTS", "Number"},
        {"MODE", "Enumeration"},
        {"MS_SHOWER", "String"},
        {"MY_ALTITUDE", "Number"},
        {"MY_ANTENNA", "String"},
        {"MY_ARRL_SECT", "Enumeration"},
        {"MY_CITY", "String"},
        {"MY_CNTY", "Enumeration"},
        {"MY_COUNTRY", "String"},
        {"MY_CQ_ZONE", "PositiveInteger"},
        {"MY_DXCC", "Enumeration"},
        {"MY_FISTS", "PositiveInteger"},
        {"MY_GRIDSQUARE", "GridSquare"},
        {"MY_GRIDSQUARE_EXT", "GridSquareExt"},
        {"MY_IOTA", "IOTARefNo"},
        {"MY_IOTA_ISLAND_ID", "PositiveInteger"},
        {"MY_ITU_ZONE", "PositiveInteger"},
        {"MY_LAT", "Location"},
        {"MY_LON", "Location"},
        {"MY_NAME", "String"},
        {"MY_POSTAL_CODE", "String"},
        {"MY_POTA_REF", "POTARef"},
        {"MY_RIG", "String"},
        {"MY_SIG", "String"},
        {"MY_SIG_INFO", "String"},
        {"MY_SOTA_REF", "SOTARef"},
        {"MY_STATE", "Enumeration"},
        {"MY_STREET", "String"},
        {"MY_USACA_COUNTIES", "SecondarySubdivisionList"},
        {"MY_VUCC_GRIDS", "GridSquareList"},
        {"MY_WWFF_REF", "WWFFRef"},
        {"NAME", "String"},
        {"NOTES", "MultilineString"},
        {"NR_BURSTS", "Integer"},
        {"NR_PINGS", "Integer"},
        {"OPERATOR", "String"},
        {"OWNER_CALLSIGN", "String"},
        {"PFX", "String"},
        {"POTA_REF", "POTARef"},
        {"PRECEDENCE", "String"},
        {"PROP_MODE", "Enumeration"},
        {"PUBLIC_KEY", "String"},
        {"QRZCOM_QSO_UPLOAD_DATE", "Date"},
        {"QRZCOM_QSO_UPLOAD_STATUS", "Enumeration"},
        {"QSLMSG", "MultilineString"},
        {"QSLRDATE", "Date"},
        {"QSLSDATE", "Date"},
        {"QSL_RCVD", "Enumeration"},
        {"QSL_RCVD_VIA", "Enumeration"},
        {"QSL_SENT", "Enumeration"},
        {"QSL_SENT_VIA", "Enumeration"},
        {"QSL_VIA", "String"},
        {"QSO_COMPLETE", "Enumeration"},
        {"QSO_DATE", "Date"},
        {"QSO_DATE_OFF", "Date"},
        {"QSO_RANDOM", "Boolean"},
        {"QTH", "String"},
        {"REGION", "Enumeration"},
        {"RIG", "MultilineString"},
        {"RST_RCVD", "String"},
        {"RST_SENT", "String"},
        {"RX_PWR", "Number"},
        {"SAT_MODE", "String"},
        {"SAT_NAME", "String"},
        {"SFI", "Integer"},
        {"SIG", "String"},
        {"SIG_INFO", "String"},
        {"SILENT_KEY", "Boolean"},
        {"SKCC", "String"},
        {"SOTA_REF", "SOTARef"},
        {"SRX", "Integer"},
        {"SRX_STRING", "String"},
        {"STATE", "Enumeration"},
        {"STATION_CALLSIGN", "String"},
        {"STX", "Integer"},
        {"STX_STRING", "String"},
        {"SUBMODE", "String"},
        {"SWL", "Boolean"},
        {"TEN_TEN", "PositiveInteger"},
        {"TIME_OFF", "Time"},
        {"TIME_ON", "Time"},
        {"TX_PWR", "Number"},
        {"UKSMG", "PositiveInteger"},
        {"USACA_COUNTIES", "SecondarySubdivisionList"},
        {"VE_PROV", "String"},
        {"VUCC_GRIDS", "GridSquareList"},
        {"WEB", "String"},
        {"WWFF_REF", "WWFFRef"},
        {"APP_LOTW_RXQSL", "Date"},
        {"APP_LOTW_RXQSO", "String"},
        {"APP_LOTW_QSO_TIMESTAMP", "Date"}
    };
    return;
}


void Utilities::setLogLevel(DebugLogLevel _l)
{
    logLevel = _l;
}

void Utilities::setARRLSect()
{
    ARRL_sects.clear ();
   // qStringList fonts = { "Arial", "Helvetica", "Times" }
   QStringList preARRL_sects = {"AL", "AK", "AB", "AR", "AZ", "BC", "CO", "CT", "DE", "EB", "EMA", "ENY", "EPA", "EWA"
        "GA", "GTA", "ID", "IL", "IN", "IA", "KS", "KY", "LAX", "LA",
        "ME", "MB", "MAR", "MDC", "MI", "MN", "MS", "MO", "MT",
        "NE", "NV", "NH", "NM", "NLI", "NL", "NC", "ND", "NTX",
        "NFL", "NNJ", "NNY", "NT", "NWT", "OH", "OK", "ON",
        "ONE", "ONN", "ONS", "ORG", "OR", "PAC", "PR", "QC",
        "RI", "SV", "SDG", "SF", "SJV", "SB", "SCV", "SK", "SC", "SD",
        "STX", "SFL", "SNJ", "TN", "VI", "UT", "VT", "VA",
        "WCF", "WTX", "WV", "WMA", "WNY", "WPA", "WWA", "WI", "WY"};
   ARRL_sects = preARRL_sects;
}

void Utilities::setContinent()
{
    continent = QStringList();
    QStringList continentList = {"NA", "SA", "EU", "AF", "OC", "AS", "AN"};
    continent = continentList;
}

void Utilities::setSponsorsList()
{
   sponsorsList.clear ();
   QStringList preSponsorsList = {"ADIF_", "ARI_", "ARRL_", "CQ_", "DARC_", "EQSL_", "IARU_", "JARL_", "RSGB_", "TAG_", "WABAG_"};
   sponsorsList = preSponsorsList;
}

void Utilities::setVersion(const QString &_v)
{
    // qDebug() << "Utilities::setVersion: " << _v ;
    softwareVersion = _v;
}

QString Utilities::getVersion()
{
    return softwareVersion;
}

double Utilities::getVersionDouble()
{
       // qDebug() << "Utilities::getVersionDouble: " << softwareVersion ;
    if (softwareVersion.count('.')>1)
    {
        QString first = softwareVersion.section('.', 0, 0);
        int pos = softwareVersion.indexOf('.');

        QString decimals = softwareVersion.section('.', pos, -1);
        decimals.remove('.');
        first = first + "." + decimals;
           // qDebug() << "Utilities::getVersionDouble - returning: "  << first ;
        return first.toDouble();
    }
       // qDebug() << "Utilities::getVersionDouble: no points detected" ;
    return softwareVersion.toDouble();
}

int Utilities::getProgresStepForDialog(int totalSteps)
{
    // qDebug() << Q_FUNC_INFO << ": " << QString::number(totalSteps);
    if (totalSteps <=100)
        return 1;
    else if (totalSteps <=1000)
        return 5;
    else if (totalSteps <=4000)
        return 10;
    else if (totalSteps <=5000)
        return 15;
    else if (totalSteps <=7000)
        return 20;
    else if (totalSteps <=9999)
        return 25;
    else if (totalSteps <=20000)
        return 100;
    else
        return 250;
}

bool Utilities::trueOrFalse(const QString &_s)
{// reads a String and return true if s.upper()== TRUE :-)
    // qDebug() << "Utilities::trueOrFalse: " << _s ;

    if ( (_s.toUpper()) == "TRUE")
    {
        return true;
    }
    else
    {
        return false;
    }
    //return false;
}

QChar Utilities::boolToCharToSQLite(const bool _b)
{
    if (_b)
        return 'Y';
    return 'N';
}


QString Utilities::boolToQString(const bool _b)
{
    if (_b)
        return "True";
    return "False";
}

bool Utilities::QStringToBool(const QString &_s)
{
   // qDebug() << Q_FUNC_INFO << ": " << _s;
    if (_s.toUpper() == "Y")
        return true;
    return false;
    //return ((_s.toUpper()) == "Y");
}

QString Utilities::checkAndFixASCIIinADIF(const QString &_data)
{
    // qDebug() << "SetupDialog::checkAndFixASCIIinADIF " << _data ;
    //TODO This function should be moved to the class ADIF
    // This function is not really working with ASCII but with Unicode
    ushort unicodeVal;
    QString st = _data;
    QString newString;
    newString.clear();
    for(int i=0; i < st.length(); i++)
    {
    // Get unicode VALUE into unicodeVal
        unicodeVal = (st.at(i)).unicode();
        if ((20 <= unicodeVal ) && (unicodeVal <= 126))
        {
            newString.append(st.at(i));
        }
             // qDebug() << "SetupDialog::checkAndFixunicodeinADIF: " << st.at(i) <<" = " << QString::number(unicodeVal) ;
    }
    // Show into another lineEdit
    return newString;
}

void Utilities::printQString(const QStringList &_qs)
{
    // qDebug() << Q_FUNC_INFO << " ---- COMMENT THIS CALL BEFORE RELEASING" ;
    if (_qs.length()<1)
    {
        // qDebug() << Q_FUNC_INFO << " ---- EMPTY QStringList received!!" ;
        return;
    }
    QString aux;
    foreach(aux, _qs)
    {
        // qDebug() << Q_FUNC_INFO << " - " << aux;
    }
    // qDebug() << Q_FUNC_INFO << " - END";
}

QString Utilities::getGlobalAgent(const QString &_klogversion)
{
    return QString("KLog-%1").arg(_klogversion);
}

QString Utilities::getAgent(const QString &_klogversion)
{
    QString version;
    QString ops;
    version = _klogversion;

#if defined(Q_OS_WIN32)
    ops = "KLog-Win32-";
#elif defined(Q_OS_WIN64)
    ops = "KLog-Win64-";
#elif defined(Q_OS_LINUX)
    ops = "KLog-Linux-";
#elif defined(Q_OS_WIN)
    ops = "KLog-Win-"+ version;
#elif defined(Q_OS_MACOS)
    ops = "KLog-macOS-";
#elif defined(Q_OS_OSX)
    ops = "KLog-OSX-";
#elif defined(Q_OS_MAC)
    ops = "KLog-MAC-";
#elif defined(Q_OS_DARWIN)
    ops = "KLog-DARWIN-";
#elif defined(Q_OS_AIX)
    ops = "KLog-aix-";
#elif defined(Q_OS_ANDROID)
    ops = "KLog-android-";
//#elif defined(Q_OS_BSD4)
//    ops = "KLog-bsd4-";
#elif defined(Q_OS_BSDI)
    ops = "KLog-bsdi-";
#elif defined(Q_OS_CYGWIN)
    ops = "KLog-cygwin-";
#elif defined(Q_OS_DARWIN)
    ops = "KLog-darwin-";
#elif defined(Q_OS_DGUX)
    ops = "KLog-dgux-";
#elif defined(Q_OS_DYNIX)
    ops = "KLog-dynix-";
#elif defined(Q_OS_FREEBSD)
    ops = "KLog-freebsd-";
#elif defined(Q_OS_HPUX)
    ops = "KLog-hpux-";
#elif defined(Q_OS_IOS)
    ops = "KLog-ios-";
#elif defined(Q_OS_IRIX)
    ops = "KLog-irix-";
#elif defined(Q_OS_LYNX)
    ops = "KLog-lynx-";
#elif defined(Q_OS_NETBSD)
    ops = "KLog-netbsd-";
#elif defined(Q_OS_OPENBSD)
    ops = "KLog-openbsd-";
#elif defined(Q_OS_OSF)
    ops = "KLog-osf-";
#elif defined(Q_OS_QNX)
    ops = "KLog-qnx-";
#elif defined(Q_OS_SCO)
    ops = "KLog-sco-";
#elif defined(Q_OS_SOLARIS)
    ops = "KLog-solaris-";
#elif defined(Q_OS_TVOS)
    ops = "KLog-tvos-";
#elif defined(Q_OS_UNIX)
    ops = "KLog-unix-";
#elif defined(Q_OS_UNIXWARE)
    ops = "KLog-unixware-";
#elif defined(Q_OS_WHATCHOS)
    ops = "KLog-whatchos-";
#elif defined(Q_OS_WINRT)
    ops = "KLog-winrt-";
#else
    ops = "KLog-Other-";
#endif

    return ops+version;

    //return "KLog-Unknown-" + version;
}

QString Utilities::getHomeDir()
{
//TODO: To be removed when the defaultDir is saved in the config file
#if defined(Q_OS_WIN)
         // qDebug() << "WINDOWS DETECTED!: "  << QDir::homePath() + "/klog" ;
    return QDir::homePath()+"/klog";  // We create the \klog for the logs and data

#else
         // qDebug() << "NO WINDOWS DETECTED!"  ;
    return QDir::homePath()+"/.klog";  // We create the ~/.klog for the logs and data
#endif
}

QString Utilities::getDefaultRST(const QString &_m)
{
   // qDebug() << "Utilities::getDefaultRST: " << _m ;

   if ((_m == "SSB") || (_m== "LSB") || (_m=="USB") )
   {
        // qDebug() << "MainWindow::setRSTToMode: Detected SSB/LSB/USB"  ;
       return "59";
   }
   else if ((_m == "CW") || (_m == "RTTY"))
   {
       return "599";
   }
   else if (_m == "PSK31")
   {
       return "599";
   }
   else if ( (_m.contains("FT", Qt::CaseInsensitive)) || (_m.contains("JT", Qt::CaseInsensitive)) || (_m.contains("QRA64", Qt::CaseInsensitive)) || (_m.contains("JS", Qt::CaseInsensitive)))
   {
        return "0";
   }
   else
   { // By default SSB RST is configured but anything could be added
        return "59";
   }
}

QStringList Utilities::getDefaultLogFields()
{
    QStringList fields;
    fields.clear();
    fields << "qso_date" << "call" << "rst_sent" << "rst_rcvd" << "bandid" << "modeid" << "comment";
    return fields;
}

QString Utilities::getDefaultModeForFreq(const double _f)
{
    // qDebug() << Q_FUNC_INFO << " - Freq: " << QString::number (_f);
    if (_f<10.0)
    {
        if ((_f>=5.0) && (_f<6.0))
                return "USB";
        return "LSB";
    }
    return "USB";
}


QString Utilities::getKLogDefaultDatabaseFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
    return getHomeDir() ;
}

QString Utilities::getDBPath()
{   //Returns the path (folder level) of the DB
    QSettings settings(getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("Misc");
    QString dbPath = settings.value ("DBPath").toString ();

    if (dbPath.length()<1)
    {
        dbPath = getKLogDefaultDatabaseFile();
    }
    return dbPath;
}

QString Utilities::getKLogDBFile()
{   // Returns the full path to the main DB
    // qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(getCfgFile (), QSettings::IniFormat);
    // qDebug() << Q_FUNC_INFO << " - CFG: " << getCfgFile();
    QString dbFile;
    QString filename = "/logbook.dat";

    if (!QFile::exists(getCfgFile ()))
        return getDBPath() + filename;
    // qDebug() << Q_FUNC_INFO << " - Config file DOES NOT Exist";
    //CRITICAL ERROR TO BE MANAGED, KLOG SHOULD STOP

    settings.beginGroup ("Misc");

    if (!settings.contains("DBPath"))
    {
        // qDebug() << Q_FUNC_INFO << " - DBPath DOES NOT Exist, returning default";
        return getDBPath() + filename;
    }

    dbFile = settings.value("DBPath").toString () + filename;
    // qDebug() << Q_FUNC_INFO << "DBPath: " << dbFile;

    if (QFile::exists(dbFile))
        return dbFile;

    // qDebug() << Q_FUNC_INFO << " - DBPath file DOES NOT Exist, returning default";
    return getDBPath() + filename;
}

QString Utilities::getKLogDBBackupFile()
{   // Returns the full path to the backup DB
    return getDBPath() + "/" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + "-backup-logbook.dat" ;
}

QString Utilities::getCfgFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
#if defined(Q_OS_WIN)
         // qDebug() << "WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
    return getHomeDir() + "/klogrc.cfg";

#else
         // qDebug() << "NO WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
    return getHomeDir() + "/klogrc";

#endif
}

QString Utilities::getDebugLogFile()
{
#if defined(Q_OS_WIN)
         // qDebug() << "WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
    return getHomeDir() + "/klogdebug.log";

#else
         // qDebug() << "NO WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
    return getHomeDir() + "/klogdebug.log";

#endif
}

QString Utilities::getSaveSpotsLogFile()
{
    QString filename = "/" + (QDateTime::currentDateTime()).toString("yyyyMMdd") + "-klogdxcluster.txt";
    return getHomeDir() + filename;
}

QString Utilities::getBackupADIFile()
{
   return getHomeDir() + "/" + (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmm") + "-klogbackup.adi";
}

QString Utilities::getClubLogFile()
{
    return getHomeDir() + "/klog-clublog-upload.adi";
}

QString Utilities::getEQSLFile()
{
    return getHomeDir() + "/klog-eqsl-upload.adi";
}

QString Utilities::getLoTWAdifFile()
{
    return getHomeDir() + "/klog-lotw-upload.adi";
}

QString Utilities::getTQSLsFileName()
{
      // qDebug() << "Utilities::getTQSLsFileName: "   ;

#if defined(Q_OS_WIN)
         // qDebug() << "WINDOWS DETECTED!: "   ;
    return "tqsl.exe";
#elif   defined(Q_OS_MACOS)
      // qDebug() << "macOS DETECTED!: "   ;
    return "tqsl.app";
#else
         // qDebug() << "NO WINDOWS/macOS DETECTED!: "   ;
    return "tqsl";
#endif
}

QString Utilities::getTQSLsPath()
{
      // qDebug() << "Utilities::getDefaultProgramsPath " ;
#if defined(Q_OS_WIN64)
         // qDebug() << "WINDOWS DETECTED!: "   ;
    return "C:/Program Files/TrustedQSL/";
#elif defined(Q_OS_WIN32)
    return "C:/Program Files (x86)/TrustedQSL/";
#elif defined(Q_OS_MACOS)
      // qDebug() << "macOS DETECTED!: "   ;
    return "/Applications/TrustedQSL/";
#else
         // qDebug() << "NO WINDOWS/macOS DETECTED!: "   ;
    return "/usr/bin/";
#endif
}

QString Utilities::getCTYFile()
{
    return getHomeDir() + "/cty.csv";
}

int Utilities::getNormalizedDXCCValue(const int _dxcc)
{
    // qDebug() << Q_FUNC_INFO << QString(": %1").arg(_dxcc);
    if (_dxcc >1000)
    {
        // qDebug() << Q_FUNC_INFO << QString(": Special: %1 / Normalized: %2").arg(_dxcc).arg(((QString::number(_dxcc)).last(3)).toInt());

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        return ((QString::number(_dxcc)).last(3)).toInt();
#else
        return ((QString::number(_dxcc)).rightRef(3)).toInt();  //To be replaced by .fromString in Qt6.0
#endif
    }
    else
    {
        return _dxcc;
    }
}

QDate Utilities::getDefaultDate()
{
    //return QDate::fromString("18000101", "yyyyMMdd");
    return QDate::currentDate();
}

bool Utilities::isValidDate(const QDate _d)
{
      // qDebug() << "Utilities::isValidDate: " << _d.toString("yyyyMMdd") ;
    if (_d.isValid())
    {
        if ( _d > QDate::fromString("18000101", "yyyyMMdd") )
        {
              // qDebug() << "Utilities::isValidDate: OK" ;
            return true;
        }
    }
      // qDebug() << "Utilities::isValidDate: Error" ;
    return false;
}

bool Utilities::isValidDateTime(const QString &_d)
{
       // qDebug() << "Utilities::isValidDateTime: " << _d ;
    QDateTime _dateTime = QDateTime::fromString(_d, "yyyyMMddhhmmss");
    if ( _dateTime.isValid()  )
    {
           // qDebug() << "Utilities::isValidDateTime: 1"  ;
        return isValidDate(_dateTime.date());
    }
       // qDebug() << "Utilities::isValidDateTime: Error" ;
    return false;
}

/*

QString Utilities::getCheckedComplexCall(const QString &_c)
{ // Normalices calls
    // qDebug() << Q_FUNC_INFO << " - Start: " << _c;
    QString call = _c;

    if (call.isNull())
        return QString();
    call.replace('\\', '/');

    if (call=="/")
        return QString();

    QStringList parts;
    parts.clear();

    parts << call.split('/');
    if (parts.count()<2)
    {
        return call;
    }
    if ((parts.at(0)).isNull())
    {
        call = parts.at(1);
    }
    else if ((parts.at(1)).isNull())
    {
        call = parts.at(0);
    }
    else
    {
        call = parts.at(0) + "/" + parts.at(1);
    }
    // qDebug() << Q_FUNC_INFO << " - END: " << call;
    return call;
}
*/

QString Utilities::getMainCallFromComplexCall(const QString &_complexCall)
{   //TODO: To be replaced by  QString Callsign::getHostFullPrefix(bool getHomeIfEmpty=true);
  // Return the part of the call that provides entity and prefix information:
  // EA4K/F => F
  // VK9/EA4K => VK9
  // EA4K/MM => EA4K
  // MM/EA4K => MM
    Callsign calls(_complexCall);
    if (calls.isValid())
        return calls.getHostFullPrefix();
    if (calls.isValidPrefix())
        return calls.getHostFullPrefix();
    return QString();

}

int Utilities::getAreaNumberFromCall(const QString &_c)
{
    // qDebug() << Q_FUNC_INFO << ": " << _c;
    QString withAreaNumberPref = getPrefixFromCall(_c, true);
    QString withOutAreaNumberPref = getPrefixFromCall(_c, false);

    // qDebug() << Q_FUNC_INFO << ":    With Area: " << withAreaNumberPref;
    // qDebug() << Q_FUNC_INFO << ": WithOut Area: " << withOutAreaNumberPref;

    QString areaString = withAreaNumberPref.remove(withOutAreaNumberPref);
    // qDebug() << Q_FUNC_INFO << ":  Area Number: " << areaString;
    bool ok = false;
    int areaNumber = areaString.toInt(&ok, 10);
    if (ok)
        return areaNumber;
    return -1;
}

bool Utilities::isValidIOTA(const QString &_c)
{
    QStringList iota;
    iota.append(_c.split('-'));
    if (!isValidContinent(iota.at(0)))
        return false;

    QString number = iota.at(1);
    bool ok = false;
    int n = number.toInt(&ok);
    if (!ok)
        return false;
    if (n<=0)
        return false;

    return true;
}

int Utilities::getIOTAIdFromIOTA(const QString &_iota)
{// Returns the numbert IOTA id

    if (!isValidIOTA(_iota))
        return -1;

    QStringList iota;
    iota.append(_iota.split('-'));
    QString number = iota.at(1);
    return number.toInt();
}

QString Utilities::getPrefixFromCall(const QString &_c, bool withAreaNumber)
{
  // qDebug() << Q_FUNC_INFO << ": " << _c << " - WithAreaNumber=" << boolToQString(withAreaNumber);

    Callsign _callsign(_c);
    if (!_callsign.isValid() && !_callsign.isValidPrefix())
    {
      // qDebug() << Q_FUNC_INFO << " - 020";
        return QString();
    }

    if (withAreaNumber)
        return _callsign.getHostFullPrefix();
    else
        return _callsign.getHostPrefix();
}


bool Utilities::isSameFreq(const double fr1, const double fr2)
{
    // qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << QString::number(fr1) << "/" << QString::number(fr2) << " = " << QString::number(fabs(fr1 - fr2)) ;

    if (fabs(fr1 - fr2) < 0.00001) // 10 Hz
    {
        // qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - true" ;
        return true;
    }
    else
    {
        // qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - false" ;
        return false;
    }
    //return fabs(fr1 - fr2) < 0.001;
}


bool Utilities::isValidFreq(const QString &_b)
{
    return  (_b.toDouble()>0);
}

bool Utilities::isValidGrid(const QString &_b)
{
    Locator locator;
    return locator.isValidLocator(_b);
}



bool Utilities::isValidGrid_ext(const QString &_b)
{// TODO: Implement the checks
 // https://www.adif.org/314/ADIF_314.htm#QSO_Field_GRIDSQUARE_EXT
    QString testLocator = _b.toUpper();
    QRegularExpression rx;
    rx.setPattern("^([A-X][A-X])([0-9][0-9])?$");
    if (rx.match(testLocator).hasMatch())
    {
        // qDebug() << Q_FUNC_INFO << ": " << testLocator;
        return true;
    }
    return false;
}

bool Utilities::isValidVUCCGrids(const QString &_b)
{
    // qDebug() << Q_FUNC_INFO << ": " << _b;
    if(_b.endsWith (','))
        return false;
    // qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << _b;
    QStringList tmp;

    // qString a = _b;
    tmp.clear ();
    tmp << _b.split (',', QT_SKIP);
   // qDebug() << Q_FUNC_INFO << ": 10";
    if ((tmp.length () !=2) && (tmp.length () !=4) )
    {
        // qDebug() << Q_FUNC_INFO << ": 11 - not valid lenght";
        return false;
    }
    // qDebug() << Q_FUNC_INFO << ": 20";

    QString aux;
    foreach (aux, tmp) {
        aux = aux.trimmed ();

        //if ((!isValidGrid (aux)) || (aux.length ()!=4))
        if (!isValidGrid (aux))
        {
           // qDebug() << Q_FUNC_INFO << ": 30 - non valid: " << aux;
            return false;
        }
        else
        {
           // qDebug() << Q_FUNC_INFO << ": 31 - valid";
        }
    }
   // qDebug() << Q_FUNC_INFO << ": 100 - VALID END";
    return true;
}

bool Utilities::isValidRST(const QString &_b)
{
    return (!_b.isEmpty());
}

bool Utilities::isValidPower(const QString &_b)
{
    return (_b.toDouble()>0);
}

bool Utilities::isValidComment(const QString &_b)
{
    return (!_b.isEmpty());
}

bool Utilities::isValidName(const QString &_b)
{
    return (!_b.isEmpty());
}

bool Utilities::fileExists(const QString &_fileName)
{
    // qDebug() << Q_FUNC_INFO << _file ;
    return (QFile::exists(_fileName));
}

bool Utilities::isValidADIFField(const QString &_b)
{
       // qDebug() << "Utilities::isValidADIFField: " << _b ;
    /*
        This functions checks if the ADIF field has the proper format.
        <Field:length:Data type>Data
    */

    if (!((_b.startsWith('<')) &&  (_b.count('>')) == 1 ))
    {
           // qDebug() << "Utilities::isValidADIFField: BAD FORMAT: No < or > delimiters: " << _b ;
        return false;
    }
    if (_b.simplified() == "<EOR>")
    {
        return true;
    }

    QStringList validDataTypes = {"B", "N", "D", "T", "S", "I", "M", "G", "E", "L"};
    QStringList qs;
    qs.clear();
    qs.append(_b.split('>'));

    if (qs.size()!= 2)
    {
           // qDebug() << "Utilities::isValidADIFField-0 (not two): " << QString::number(qs.size()) ;
        return false;
    }

    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

      // qDebug() << "Utilities::isValidADIFField-Field: " << field ;
      // qDebug() << "Utilities::isValidADIFField_Data: " << data ;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
        if (!validDataTypes.contains(dataType.toUpper()))
        {
               // qDebug() << "Utilities::isValidADIFField - FORMAT ERROR: Wrong data type: " << dataType ;
            return false;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
           // qDebug() << "Utilities::isValidADIFField - FORMAT ERROR, more than 2 \":\" - " << field ;
        return false;
    }

    if ( length != separatorPosition)
    {
           // qDebug() << "Utilities::isValidADIFField: Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) ;
        return false;
    }

    if (separatorPosition <= 0)
    {
        // qDebug() << "Utilities::isValidADIFField: Length problem <= 0" ;
        return false;
    }
       // qDebug() << "FileManager::checkADIFValidFormat: Return true" ;
    return true;
}

bool Utilities::isValidQSL_Rcvd(const QString &c)
{
    QString v = c.toUpper();
    return ((v == "Y") || (v == "N") || (v == "R") || (v == "I") || (v == "V"));
}

bool Utilities::isValidQSL_Sent(const QString &c)
{
    QString v = c.toUpper();
    return ((v == "Y") || (v == "N") || (v == "R") || (v == "Q") || (v == "I"));
}

bool Utilities::isValidUpload_Status(const QString &c)
{
   // qDebug() << Q_FUNC_INFO << " - " << c;
    QString v = c.toUpper();
    return ((v == "Y") || (v == "N") || (v == "M"));
}

QStringList Utilities::getValidADIFFieldAndData(const QString &_b)
{
   // qDebug() << Q_FUNC_INFO << _b ;
    /*
        This functions checks if the ADIF field has the proper format.
        <Field:length:Data type>Data
    */
    QStringList result;
    result.clear();

    if (!(_b.startsWith('<')))
    {
        // qDebug() << Q_FUNC_INFO << ": BAD FORMAT: No < or > delimiters: " << _b ;
        return QStringList();
    }
    if (_b.simplified() == "<EOR>")
    {
        // qDebug() << : EOR" ;
        result << "EOR" << "EOR";
        return result;
    }
    QString aux = _b;
    QStringList qs;
    qs.clear();

    if ((aux.contains("APP_LOTW_")) && aux.contains("//"))
    { // Trying to fix a LoTW ADIF bug
        qs.append(aux.split("//"));
        aux = qs.at(0);
    }

    // qDebug() << : -20" ;
    QStringList validDataTypes = {"B", "N", "D", "T", "S", "I", "M", "G", "E", "L"};

    qs.clear();
    qs.append(aux.split('>'));

    if (qs.size()!= 2)
    {
        // qDebug() << -0 (not two): " << QString::number(qs.size()) ;
        return result;
    }
    // qDebug() << : -30" ;
    // qString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

    // qDebug() << -Field: " << field ;
    // qDebug() << _Data: " << data ;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
          // qDebug() <<  - DataType: -" << dataType << "-" ;
        if (!validDataTypes.contains(dataType.toUpper()))
        {
              // qDebug() <<  - FORMAT ERROR: Wrong data type: " << dataType ;
            return result;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
        // qDebug() <<  - FORMAT ERROR, more than 2 \":\" - " << field ;
        return result;
    }
    // qDebug() << : -60" ;
    if ( length != separatorPosition)
    {
        // qDebug() << : Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) ;
        return result;
    }

    if (separatorPosition <= 0)
    {
        // qDebug() << : Length problem <= 0" ;
        return result;
    }
     // qDebug() << : -90: f: " << field ;
      // qDebug() << : -90: d: " << data;ield = field.section(':', 0, 0);
    result.clear();
    result << field.section(':', 0, 0) << data;
     // qDebug() << "Utilities::checkADIFValidFormat: Return true: " << result.at(0) << "/" << result.at(1) ;
    return result;
}

bool Utilities::areThoseListsTheSame(const QStringList &_list1, const QStringList &_list2)
{
    // qDebug() << Q_FUNC_INFO << " - Start";

    // If the lists don't have the same number of items are not the same.
    if (_list1.size() != _list2.size())
        return false;

    // Let's order the lists and compare their elements
    QStringList _first, _second;
    _first.clear();
    _first << _list1;
    _first.sort(Qt::CaseInsensitive);

    _second.clear();
    _second << _list2;
    _second.sort(Qt::CaseInsensitive);

    for (int i=0;i<_first.count();i++)
    {
        // qDebug() << Q_FUNC_INFO << ": " << _first.at(i) << "/" << _second.at(i);
        if (_first.at(i) != _second.at(i))
            return false;
    }
    // qDebug() << Q_FUNC_INFO << " - END true";
    return true;
}

QString Utilities::getAValidCall (const QString &_wrongCall)
{
    // qDebug() << "Utilities::getAValidCall: " << _wrongCall ;
    QString _confirmedCall;

    // Check if the provided call is empty or invalid
    if (_wrongCall.isEmpty()) {
        _confirmedCall = QObject::tr("An empty callsign has been detected. If it is possible, please enter the right call.");
    } else {
        _confirmedCall = QObject::tr("A wrong callsign has been found: %1. Please enter a new callsign or confirm that the current one is a good callsign.").arg(_wrongCall);
    }
    // Prompt the user to enter a valid callsign
    bool ok;
    QString text = QInputDialog::getText(nullptr, QObject::tr("KLog - Not valid callsign found"), _confirmedCall, QLineEdit::Normal, _wrongCall, &ok);

    // Validate the entered callsign
    Callsign callsign(text);
    if (ok && callsign.isValid()) {
        _confirmedCall = text;
    } else {
        _confirmedCall.clear();
    }

    return _confirmedCall;

}

QString Utilities::getDateTimeSQLiteStringFromDateTime(const QDateTime &_d)
{
    return QString(_d.toString("yyyy-MM-dd hh:mm:ss"));
}

QString Utilities::getDateSQLiteStringFromDate(const QDate &_d)
{
    return QString(_d.toString("yyyy-MM-dd"));
}

bool Utilities::isValidDateFromString(const QString &_s)
{
    return getDateFromSQliteString(_s).isValid();
}

bool Utilities::isValidTimeFromString(const QString &_s)
{
    return getDateTimeFromSQLiteString(_s).time().isValid();
}

bool Utilities::isValidDateTimeFromString(const QString &_s)
{
    return getDateTimeFromSQLiteString(_s).isValid();
}

bool Utilities::isValidDXCC(const int _d)
{//TODO: Look for a better way to check, taking into account how KLog is identifiying the DXCC
    return (((_d > 0) && (_d < 523))  || (_d == 1206) || (_d == 1279) || (_d == 1248) || (_d == 2248) || (_d == 1259) || (_d == 1390));
}

bool Utilities::isValidAntPath(const QString &_s)
{ // MOVE IT TO ADIF
   return ((_s == "G") || (_s == "O") || (_s == "S") || (_s == "L"));
}

bool Utilities::isValidARRLSect(const QString &_s)
{
    return (ARRL_sects.contains (_s.toUpper ()));
}

QDateTime Utilities::getDateTimeFromSQLiteString(const QString &_s)
{
    return QDateTime::fromString(_s, "yyyy-MM-dd hh:mm:ss");
}

QTime Utilities::getTimeFromSQLiteString(const QString &_s)
{
    if (_s.length()==5)
    {
        return QTime::fromString(_s, "hh:mm");
    }
    else
    {
        return QTime::fromString(_s, "hh:mm:ss");
    }
}

QDate Utilities::getDateFromSQliteString(const QString &_s)
{
    //It may receive "just" a date or a "date time".

    if (getDateTimeFromSQLiteString(_s).isValid()) // if we have received a full date time
    {
        return (getDateTimeFromSQLiteString(_s)).date();
    }
    else // If we have received "just a date" or an error
    {
        return  QDate::fromString(_s, "yyyy-MM-dd");
    }
}

QDate Utilities::getDateFromADIFDateString(const QString &_s)
{// Expects an ADIF DATE format string: "YYYYMMDD"
    // qDebug() << "Utilities::getDateFromADIFDateString: " << _s ;
    return QDate::fromString(_s, "yyyyMMdd");
}

QTime Utilities::getTimeFromADIFTimeString(const QString &_s)
{// Expects and ADIF TIME format String "HHMMSS" or "HHMM"
    // qDebug() << "Utilities::getTimeFromADIFTimeString: " << _s ;
    if (_s.length()==4)
    {
        return QTime::fromString(_s, "hhmm");
    }
    else
    {
        return QTime::fromString(_s, "hhmmss");
    }
}

QDate Utilities::getDateFromLoTWQSLDateString(const QString &_s)
{
    qDebug() << "Utilities::getDateFromLoTWQSLDateString: " << _s ;
    QStringList datet;
    datet.clear();
    if (_s.endsWith ('Z'))
    {
        datet << _s.split("T");
    }
    else
    {
        datet << _s.split(" ");
    }
    // qDebug() << "Utilities::getDateFromLoTWQSLDateString: " << datet.at(0) ;
    return QDate::fromString(datet.at(0), "yyyy-MM-dd");
}

QString Utilities::getADIFDateFromQDateTime(const QDateTime &_d)
{
    return getADIFDateFromQDate(_d.date());
}

QString Utilities::getADIFDateFromQDate(const QDate &_d)
{
    if (!_d.isValid())
    {
        return QString();
    }
    else
    {
        return _d.toString("yyyyMMdd");
    }
}

QString Utilities::getADIFTimeFromQDateTime(const QDateTime &_d)
{
    return getADIFTimeFromQTime(_d.time());
}

QString Utilities::getADIFTimeFromQTime(const QTime &_d)
{
    if (!_d.isValid())
    {
        return QString();
    }
    else
    {
        if (_d.second() == 0)
            return _d.toString("hhmm");
        return _d.toString("hhmmss");
    }
}


QString Utilities::getCabrilloDateFromQDate(const QDate &_d)
{// Will produce the Cabrillo DATE format: "YYYY-MM-DD"
    if (!_d.isValid())
    {
        return QString("0000-00-00");
    }
    else
    {
        return _d.toString("yyyy-MM-dd");
    }
}

QString Utilities::getCabrilloTimeFromQDateTime(const QDateTime &_d)
{// Will produce the Cabrillo TIME format: "HHMM"
    if (!_d.isValid())
    {
        return QString("0000");
    }
    else
    {
        return _d.time().toString("hhmm");
    }
}



QString Utilities::getOnlineServiceName(OnLineProvider _service)
{//enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ}; //, HamQTH, HRDLog
    switch (_service)
    {
        case LoTW:
        {
            return "LoTW";
        }
        case ClubLog:
        {
            return "ClubLog";
        }
        case QRZ:
        {
            return "QRZ.com";
        }
        case eQSL:
        {
            return "eQSL.cc";
        }
    }
    return QString();
}

QString Utilities::getClearSQLi(QString _s)
{
    return _s.remove ('\'');
}

//void Utilities::setDarkMode(const QString &_dm)
//{
//    darkMode = trueOrFalse(_dm);
//}

//bool Utilities::isDarkMode()
//{
//    return darkMode;
//}

void Utilities::setLogColumnNames()
{
    // QMap<QString, QString> columnNames;
    //columnNames.insert("", QObject::tr(""));
    columnNames.insert("altitude", QObject::tr("Altitude"));
    columnNames.insert("qso_date", QObject::tr("Date"));
    columnNames.insert("call", QObject::tr("Call"));
    columnNames.insert("rst_sent", QObject::tr("RSTtx"));
    columnNames.insert("rst_rcvd", QObject::tr("RSTrx"));
    columnNames.insert("bandid", QObject::tr("Band"));
    columnNames.insert("comment", QObject::tr("Comment"));
    columnNames.insert("modeid", QObject::tr("Mode"));
    columnNames.insert("cqz", QObject::tr("CQz"));
    columnNames.insert("ituz", QObject::tr("ITUz"));
    columnNames.insert("dxcc", QObject::tr("DXCC"));
    columnNames.insert("address", QObject::tr("Address"));
    columnNames.insert("age", QObject::tr("Age"));
    columnNames.insert("cnty", QObject::tr("County"));
    columnNames.insert("a_index", QObject::tr("A_Index"));
    columnNames.insert("ant_az", QObject::tr("Ant_Az"));
    columnNames.insert("ant_el", QObject::tr("Ant_El"));
    columnNames.insert("ant_path", QObject::tr("Ant_Path"));
    columnNames.insert("arrl_sect", QObject::tr("ARRL_SECT"));
    columnNames.insert("award_submitted", QObject::tr("Award_Submitted"));
    columnNames.insert("award_granted", QObject::tr("Award_granted"));
    columnNames.insert("band_rx", QObject::tr("Band_RX"));
    columnNames.insert("checkcontest", QObject::tr("CheckContest"));
    columnNames.insert("class", QObject::tr("Class"));
    columnNames.insert("clublog_qso_upload_date", QObject::tr("ClubLog SDate", "Date when it was sent"));
    columnNames.insert("clublog_qso_upload_status", QObject::tr("ClubLog status"));
    columnNames.insert("cont", QObject::tr("Continent"));
    columnNames.insert("contacted_op", QObject::tr("Contacted_Op"));
    columnNames.insert("contest_id", QObject::tr("Contest Id"));
    columnNames.insert("country", QObject::tr("Country"));
    columnNames.insert("credit_submitted", QObject::tr("Credit Submitted"));
    columnNames.insert("credit_granted", QObject::tr("Credit granted"));
    columnNames.insert("darc_dok", QObject::tr("Darc Dok", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("distance", QObject::tr("Distance"));
    columnNames.insert("email", QObject::tr("Email"));
    columnNames.insert("eq_call", QObject::tr("EQ_Call"));
    columnNames.insert("eqsl_qslrdate", QObject::tr("eQSL RDate"));
    columnNames.insert("eqsl_qslsdate", QObject::tr("eQSL SDate", "Date when it was sent"));
    columnNames.insert("eqsl_qsl_rcvd", QObject::tr("eQSL Rcvd"));
    columnNames.insert("eqsl_qsl_sent", QObject::tr("eQSL Sent"));
    columnNames.insert("fists", QObject::tr("Fists", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("fists_cc", QObject::tr("Fists CC", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("force_init", QObject::tr("Force Init"));
    columnNames.insert("freq", QObject::tr("Freq"));
    columnNames.insert("freq_rx", QObject::tr("Freq RX"));
    columnNames.insert("gridsquare", QObject::tr("Gridsquare"));
    columnNames.insert("gridsquare_ext", QObject::tr("Gridsquare ext", "Extended gridsquare"));
    columnNames.insert("hamlogeu_qso_upload_status", QObject::tr("HamLog status"));
    columnNames.insert("hamlogeu_qso_upload_date", QObject::tr("HamLog SDate", "Date when it was sent"));
    columnNames.insert("hrdlog_qso_upload_date", QObject::tr("HRDLog SDate", "Date when it was sent"));
    columnNames.insert("hrdlog_qso_upload_status", QObject::tr("HRDLog status"));
    columnNames.insert("hamqth_qso_upload_status", QObject::tr("HamQTH status"));
    columnNames.insert("hamqth_qso_upload_date", QObject::tr("HamQTH SDate", "Date when it was sent"));
    columnNames.insert("iota", QObject::tr("IOTA"));
    columnNames.insert("iota_island_id", QObject::tr("IOTA Island id"));
    columnNames.insert("k_index", QObject::tr("K Index"));
    columnNames.insert("lat", QObject::tr("Lat"));
    columnNames.insert("lon", QObject::tr("Lon"));
    columnNames.insert("lotw_qslrdate", QObject::tr("LoTW RDate"));
    columnNames.insert("lotw_qslsdate", QObject::tr("LoTW SDate", "Date when it was sent"));
    columnNames.insert("lotw_qsl_rcvd", QObject::tr("LoTW Rcvd"));
    columnNames.insert("lotw_qsl_sent", QObject::tr("LoTW Sent"));
    columnNames.insert("marked", QObject::tr("Marcado"));
    columnNames.insert("max_bursts", QObject::tr("Max Bursts"));
    columnNames.insert("ms_shower", QObject::tr("MS Shower"));
    columnNames.insert("my_altitude", QObject::tr("My Altitude"));
    columnNames.insert("my_antenna", QObject::tr("My Antenna"));
    columnNames.insert("my_arrl_sect", QObject::tr("My ARRL Sect"));
    columnNames.insert("my_city", QObject::tr("My City"));
    columnNames.insert("my_cnty", QObject::tr("My Cnty"));
    columnNames.insert("my_country", QObject::tr("My Country"));
    columnNames.insert("my_cq_zone", QObject::tr("My CQz"));
    columnNames.insert("my_dxcc", QObject::tr("My DXCC"));
    columnNames.insert("my_fists", QObject::tr("My Fists", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("my_gridsquare", QObject::tr("My Gridsquare"));
    columnNames.insert("my_gridsquare_ext", QObject::tr("My Gridsquare Ext"));
    columnNames.insert("my_iota", QObject::tr("My IOTA"));
    columnNames.insert("my_iota_island_id", QObject::tr("My IOTA island id"));
    columnNames.insert("my_itu_zone", QObject::tr("My ITUz"));
    columnNames.insert("my_lat", QObject::tr("My Lat"));
    columnNames.insert("my_lon", QObject::tr("My Lon"));
    columnNames.insert("my_name", QObject::tr("My Name"));
    columnNames.insert("my_postal_code", QObject::tr("My Postal code"));
    columnNames.insert("my_pota_ref", QObject::tr("My POTA"));
    columnNames.insert("my_rig", QObject::tr("My Rig"));
    columnNames.insert("my_sig", QObject::tr("My Sig"));
    columnNames.insert("my_sig_info", QObject::tr("My Sig Info"));
    columnNames.insert("my_sota_ref", QObject::tr("My SOTA"));
    columnNames.insert("my_state", QObject::tr("My State"));
    columnNames.insert("my_street", QObject::tr("My Street"));
    columnNames.insert("my_usaca_counties", QObject::tr("My USACA counties"));
    columnNames.insert("my_vucc_grids", QObject::tr("My VUCC grids"));
    columnNames.insert("my_wwff_ref", QObject::tr("My WWFF"));
    columnNames.insert("name", QObject::tr("Name"));
    columnNames.insert("notes", QObject::tr("Notes"));
    columnNames.insert("nr_bursts", QObject::tr("Nr bursts", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("nr_pings", QObject::tr("Nr pings", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("operator", QObject::tr("Operator"));
    columnNames.insert("owner_callsign", QObject::tr("Owner Callsign"));
    columnNames.insert("pota_ref", QObject::tr("POTA"));
    columnNames.insert("pfx", QObject::tr("Pfx"));
    columnNames.insert("precedence", QObject::tr("Precedence"));
    columnNames.insert("prop_mode", QObject::tr("Prop Mode"));
    columnNames.insert("public_key", QObject::tr("Public Key"));
    columnNames.insert("qrzcom_qso_upload_date", QObject::tr("QRZcom SDate"));
    columnNames.insert("qrzcom_qso_upload_status", QObject::tr("QRZcom status"));
    columnNames.insert("qslmsg", QObject::tr("QSL msg"));
    columnNames.insert("qslrdate", QObject::tr("QSL RDate"));
    columnNames.insert("qslsdate", QObject::tr("QSL SDate"));
    columnNames.insert("qsl_rcvd", QObject::tr("QSL Rcvd"));
    columnNames.insert("qsl_sent", QObject::tr("QSL Sent"));
    columnNames.insert("qsl_rcvd_via", QObject::tr("QSL rcvd via"));
    columnNames.insert("qsl_sent_via", QObject::tr("QSL sent via"));
    columnNames.insert("qsl_via", QObject::tr("QSL via"));
    columnNames.insert("qso_complete", QObject::tr("QSO complete"));
    columnNames.insert("qso_random", QObject::tr("QSO random"));
    columnNames.insert("qth", QObject::tr("QTH"));
    columnNames.insert("region", QObject::tr("Region"));
    columnNames.insert("rig", QObject::tr("Rig"));
    columnNames.insert("rx_pwr", QObject::tr("RX Pwr"));
    columnNames.insert("sat_mode", QObject::tr("Sat mode"));
    columnNames.insert("sat_name", QObject::tr("Sat name"));
    columnNames.insert("sfi", QObject::tr("SFI"));
    columnNames.insert("sig", QObject::tr("Sig"));
    columnNames.insert("sig_info", QObject::tr("Sig Info"));
    columnNames.insert("silent_key", QObject::tr("Silent key", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("skcc", QObject::tr("SKCC"));
    columnNames.insert("sota_ref", QObject::tr("SOTA Ref"));
    columnNames.insert("srx_string", QObject::tr("SRX String"));
    columnNames.insert("srx", QObject::tr("SRX"));
    columnNames.insert("stx_string", QObject::tr("STX String"));
    columnNames.insert("state", QObject::tr("State"));
    columnNames.insert("station_callsign", QObject::tr("Station Callsign"));
    columnNames.insert("submode", QObject::tr("Submode"));
    columnNames.insert("swl", QObject::tr("SWL", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("uksmg", QObject::tr("UKSMG"));
    columnNames.insert("usaca_counties", QObject::tr("USACA counties"));
    columnNames.insert("ve_prov", QObject::tr("VE prov"));
    columnNames.insert("vucc_grids", QObject::tr("VUCC grids"));
    columnNames.insert("ten_ten", QObject::tr("Ten-Ten", "Do not translate, it is a hamradio group name."));
    columnNames.insert("tx_pwr", QObject::tr("TX Pwr"));
    columnNames.insert("web", QObject::tr("Web"));
    columnNames.insert("wwff_ref", QObject::tr("WWFF"));
    columnNames.insert("qso_date_off", QObject::tr("QSO Date off"));
    columnNames.insert("lognumber", QObject::tr("Log number"));
}

QString Utilities::getLogColumnName(const QString &_column)
{
    // qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << _column;
    QString aux = columnNames.value(_column);
    if (!aux.isEmpty())
    {
        return aux;
    }
    return _column;
}

QString Utilities::getLogColumnDBName(const QString &_column)
{
    // qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << _column;
    QString aux = columnNames.key(_column);
    if (!aux.isEmpty())
    {
        return aux;
    }
    return _column;
}

bool Utilities::isValidContinent(const QString &_s)
{
    return (continent.contains (_s.toUpper ()));
}

bool Utilities::isValidPropMode(const QString &_s)
{ //TODO MOVE IT TO ADIF
    QStringList propModes;
    propModes.clear ();
    propModes << "AS" << "AUE" << "AUR" << "BS" << "ECH" << "EME" << "ES"
                << "F2" << "FAI" << "GWAVE" << "INTERNET" << "ION" << "IRL"
                << "LOS" << "MS" << "RPT" << "RS" << "SAT" << "TEP" << "TR";
    return propModes.contains (_s.toUpper ());
}

bool Utilities::isValidEmail(const QString &_s)
{
    // qRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    QRegularExpression mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

    //mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    //mailREX.setPatternSyntax(QRegExp::RegExp)

    // match two digits followed by a space and a word

    QRegularExpressionMatch match = mailREX.match(_s);
    return match.hasMatch(); // true
    //return mailREX.exactMatch(_s);
}


//bool Utilities::isValidDistance(const double _d)
//{
//    return (_d>0);
//}

bool Utilities::isValidSponsor(const QString &_s)
{
    return (sponsorsList.contains (_s.toUpper ()));
}

void Utilities::openQrzcom(const QString _call)
 {
     QString url = "https://www.qrz.com/lookup/" + _call;
     QDesktopServices::openUrl(QUrl(url));
}

void Utilities::printCommandHelp(){
    QTextStream cout(stdout);
    cout << "Usage: klog [OPTION]...\n";
    cout << "Options:\n";
    cout << "     -?           Display this help\n";
    cout << "     -h           Display this help\n";
    cout << "     -v           Display program version\n";
}

QString Utilities::debugLevelToString(DebugLogLevel _l)
{
    switch (_l) {
        case None:
            return "None";
        break;
        case Info:
            return "Info";
        break;
        case Debug:
            return "Debug";
        break;
        case Devel:
            return "Devel";
        break;
        default:
            return "Undefined";
        break;
    }
}

DebugLogLevel Utilities::stringToDebugLevel(const QString &_s)
{
    QString aux;
    aux = _s.toUpper();
    if (aux == "INFO")
        return Info;
    else if (aux == "DEBUG")
        return Debug;
    else if (aux == "DEVEL")
        return Devel;
    else
        return None;
}

bool Utilities::isValidLogLevel(const QString &_s)
{
    return logLevels.contains (_s);
}

QStringList Utilities::getDebugLevels()
{
    return logLevels;
}

void Utilities::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    emit debugLog (_func, _msg, _level);
    /*
    // qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << _func << ": " << _level << ": " << _msg;
    if (logLevel<=_level)
    {
    // qDebug() << _func << ": " << _level << ": " << _msg;
        emit debugLog (_func, _msg, _level);
    }
    */
}
