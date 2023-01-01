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
#include "global.h"
#include "utilities.h"

//bool c;
Utilities::Utilities(const QString &_parentName)
{
    parentName = _parentName;
    //qDebug() << Q_FUNC_INFO << " (" << _parentName << ")";
    init();
}

Utilities::~Utilities()
{
}

void Utilities::init()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    validateCalls = false;
    softwareVersion = "0.0";
    longPrefixes.clear();
    specialCalls.clear();
    darkMode = false;
    logLevel = None;
    setARRLSect();
    setContinent();
    setSponsorsList();
    logLevels.clear();
    logLevels << "None" << "Info" << "Debug" << "Devel";
    setLogColumnNames();

    //qDebug() << Q_FUNC_INFO << " - END";
}

void Utilities::setLogLevel(DebugLogLevel _l)
{
    logLevel = _l;
}

void Utilities::setARRLSect()
{
    ARRL_sects.clear ();
   //QStringList fonts = { "Arial", "Helvetica", "Times" }
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
    //qDebug() << "Utilities::setVersion: " << _v ;
    softwareVersion = _v;
}

QString Utilities::getVersion()
{
    return softwareVersion;
}

double Utilities::getVersionDouble()
{
       //qDebug() << "Utilities::getVersionDouble: " << softwareVersion ;
    if (softwareVersion.count('.')>1)
    {
        QString first = softwareVersion.section('.', 0, 0);
        int pos = softwareVersion.indexOf('.');

        QString decimals = softwareVersion.section('.', pos, -1);
        decimals.remove('.');
        first = first + "." + decimals;
           //qDebug() << "Utilities::getVersionDouble - returning: "  << first ;
        return first.toDouble();
    }
       //qDebug() << "Utilities::getVersionDouble: no points detected" ;
    return softwareVersion.toDouble();
}

int Utilities::getProgresStepForDialog(int totalSteps)
{
       //qDebug() << "Utilities::getProgresStepForDialog";
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
    //qDebug() << "Utilities::trueOrFalse: " << _s ;

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
    {
        return 'Y';
    }
    else
    {
        return 'N';
    }
}

QString Utilities::boolToQString(const bool _b)
{
    if (_b)
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

bool Utilities::QStringToBool(const QString &_s)
{

    if ( (_s.toUpper()) == "Y")
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString Utilities::checkAndFixASCIIinADIF(const QString &_data)
{
    //qDebug() << "SetupDialog::checkAndFixASCIIinADIF " << _data ;
// This function is not really working with ASCII but with Unicode
//TODO: this function is also in the FileManager class. Maybe I should call that one and keep just one copy
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
             //qDebug() << "SetupDialog::checkAndFixunicodeinADIF: " << st.at(i) <<" = " << QString::number(unicodeVal) ;
    }
    // Show into another lineEdit
    return newString;
}

void Utilities::printQString(const QStringList &_qs)
{
        //qDebug() << "Utilities::printQString: COMMENT THIS CALL BEFORE RELEASING" ;
    if (_qs.length()<1)
    {
            //qDebug() << "Utilities::printQString: EMPTY QStringList received!!" ;
        return;
    }
    for (int i=0; i<_qs.length()-1;i++)
    {
            //qDebug() << _qs.at(i) << "/" ;
    }
        //qDebug() << _qs.at(_qs.length()-1) ;
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
         //qDebug() << "WINDOWS DETECTED!: "  << QDir::homePath() + "/klog" ;
    return QDir::homePath()+"/klog";  // We create the \klog for the logs and data

#else
         //qDebug() << "NO WINDOWS DETECTED!"  ;
    return QDir::homePath()+"/.klog";  // We create the ~/.klog for the logs and data
#endif
}

QString Utilities::getDefaultRST(const QString &_m)
{
   //qDebug() << "Utilities::getDefaultRST: " << _m ;

   if ((_m == "SSB") || (_m== "LSB") || (_m=="USB") )
   {
        //qDebug() << "MainWindow::setRSTToMode: Detected SSB/LSB/USB"  ;
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

QString Utilities::getKLogDefaultDatabaseFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
    return getHomeDir() ;
}

QString Utilities::getKLogDBFile()
{
        //qDebug() << "Utilities::getKLogDBFile: start " ;

    dbPath = getKLogDefaultDatabaseFile();
    QFile file(getCfgFile());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  /* Flawfinder: ignore */
    {
        //return dbPath;
        //return getKLogDatabaseFile(dbPath);
    }
    else
    {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            processConfigLine(line);
        }

        if (dbPath.length()<1)
        {
            dbPath = getKLogDefaultDatabaseFile();
        }
    }
       //qDebug() << "Utilities::getKLogDBFile: path to use: " << dbPath ;
    return dbPath + "/logbook.dat";
}

QString Utilities::getKLogDBBackupFile()
{
        //qDebug() << "Utilities::getKLogDBFile: start " ;

    dbPath = getKLogDefaultDatabaseFile();
    QFile file(getCfgFile());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //return dbPath;
        //return getKLogDatabaseFile(dbPath);
    }
    else
    {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            processConfigLine(line);
        }
        if (dbPath.length()<1)
        {
            dbPath = getKLogDefaultDatabaseFile();
        }
    }
       //qDebug() << "Utilities::getKLogDBFile: path to use: " << dbPath ;
    return dbPath + "/" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + "-backup-logbook.dat" ;
}

bool Utilities::processConfigLine(const QString &_line)
{
             //qDebug() << "Utilities::processConfigLine: " << _line ;

        QString line = _line.simplified();
        //line.simplified();
        //QString aux;
        QStringList values = line.split("=", QT_SKIP);

        if (line.startsWith('#')){
                 //qDebug() << "Utilities::processConfigLine: notes Line!" ;
            return true;
        }
        if (!( (line.contains('=')) && (line.contains(';')))){
                 //qDebug() << "Utilities::processConfigLine: Wrong Line!" ;
            return false;
        }
        QString field = (values.at(0)).toUpper();
        QString value = values.at(1);

        int endValue = value.indexOf(';');
        if (endValue>-1){
            value = value.left(value.length() - (value.length() - endValue));
        }

        if (field == "DBPATH")
        {
                  //qDebug() << "Utilities::processConfigLine: dbPATH found: " << value ;
            dbPath = value;
        }
        return true;
}

/*
QString Utilities::getKLogDatabaseFile(const QString &_file)
{
       //qDebug() << "Utilities::getKLogDatabaseFile:" << _file ;
    if ( QFile::exists(_file + "/logbook.dat") )
    {
           //qDebug() << "Utilities::getKLogDatabaseFile:returning: " <<  _file + "/logbook.dat" ;
        return _file + "/logbook.dat";
    }
         //qDebug() << "Utilities::getKLogDatabaseFile: Does not exist so default: " <<  getKLogDefaultDatabaseFile() ;
        return getKLogDefaultDatabaseFile();
}
*/

QString Utilities::getCfgFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
#if defined(Q_OS_WIN)
         //qDebug() << "WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
    return getHomeDir() + "/klogrc.cfg";

#else
         //qDebug() << "NO WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
    return getHomeDir() + "/klogrc";

#endif
}

QString Utilities::getDebugLogFile()
{
#if defined(Q_OS_WIN)
         //qDebug() << "WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
    return getHomeDir() + "/klogdebug.log";

#else
         //qDebug() << "NO WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  ;
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
      //qDebug() << "Utilities::getTQSLsFileName: "   ;

#if defined(Q_OS_WIN)
         //qDebug() << "WINDOWS DETECTED!: "   ;
    return "tqsl.exe";
#elif   defined(Q_OS_MACOS)
      //qDebug() << "macOS DETECTED!: "   ;
    return "tqsl.app";
#else
         //qDebug() << "NO WINDOWS/macOS DETECTED!: "   ;
    return "tqsl";
#endif
}

QString Utilities::getTQSLsPath()
{
      //qDebug() << "Utilities::getDefaultProgramsPath " ;

#if defined(Q_OS_WIN64)
         //qDebug() << "WINDOWS DETECTED!: "   ;
    return "C:/Program Files/TrustedQSL/";
#elif defined(Q_OS_WIN32)
    return "C:/Program Files (x86)/TrustedQSL/";
#elif defined(Q_OS_MACOS)
      //qDebug() << "macOS DETECTED!: "   ;
    return "/Applications/TrustedQSL/";
#else
         //qDebug() << "NO WINDOWS/macOS DETECTED!: "   ;
    return "/usr/bin/";
#endif
}

QString Utilities::getCTYFile()
{
    return getHomeDir() + "/cty.csv";
}

int Utilities::getNormalizedDXCCValue(const int _dxcc)
{
    if (_dxcc >1000)
    {
        return ((QString::number(_dxcc)).rightRef(3)).toInt();
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
      //qDebug() << "Utilities::isValidDate: " << _d.toString("yyyyMMdd") ;
    if (_d.isValid())
    {
        if ( _d > QDate::fromString("18000101", "yyyyMMdd") )
        {
              //qDebug() << "Utilities::isValidDate: OK" ;
            return true;
        }
    }
      //qDebug() << "Utilities::isValidDate: Error" ;
    return false;
}

bool Utilities::isValidDateTime(const QString &_d)
{
       //qDebug() << "Utilities::isValidDateTime: " << _d ;
    QDateTime _dateTime = QDateTime::fromString(_d, "yyyyMMddhhmmss");
    if ( _dateTime.isValid()  )
    {
           //qDebug() << "Utilities::isValidDateTime: 1"  ;
        return isValidDate(_dateTime.date());
    }
       //qDebug() << "Utilities::isValidDateTime: Error" ;
    return false;
}

bool Utilities::isValidSimpleCall(const QString &_c)
{
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Start:  %1").arg(_c), Debug);
    // This functions only checks simple calls like EA4K, not composed like EA4K/F of F/EA4K/QRP
    //Rules: http://life.itu.int/radioclub/rr/art19.pdf
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << QString(" - 000 - %1").arg(_c);
    if ((_c.contains('/')) || (_c.contains('\\')))
    {
        //qDebug() << Q_FUNC_INFO << " -001";
        return false;
    }
    int length = _c.length();
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 010";
    if (length<3)
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Less than 3 chars - FALSE"), Debug);
        //qDebug() << Q_FUNC_INFO << " - END2";
        return false;
    }

    //qDebug() << Q_FUNC_INFO << " - 020";
    QString call = _c;
    if (isAKnownCall(call))
    {
        //qDebug() << Q_FUNC_INFO << " - 020.5";
        return true;
    }
    //qDebug() << Q_FUNC_INFO << " - 021";
    // Does it contain any digit?
    bool hasDigit = false;
    for (int i=0;i<=length-1;i++)
    {
        if (!_c.at(i).isLetterOrNumber ())
        {
            // Non valid chars
            //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Non valid chars - FALSE"), Debug);
            //qDebug() << Q_FUNC_INFO << " - END3";
            return false;
        }
        if ((_c.at(i)).isDigit())
            hasDigit = true;
    }
    if (!hasDigit)
    {
        //qDebug() << Q_FUNC_INFO << " - END4";
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - 030";
    if (!(_c.at(length-1).isLetter ()))
    {
        logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Does not end with a char - FALSE"), Debug);
        //qDebug() << Q_FUNC_INFO << " - END6";
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " - 040";
    if (length<=5)
    { //This may fail with JY1 Special call or others special 4 or 5 letter callsigns listed in
        //qDebug() << Q_FUNC_INFO << " - 031 - " << _c;
        if (isAKnownPrefix(_c))
        {
            //qDebug() << Q_FUNC_INFO << " - END5";
            return false;
        }
    }
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 040";
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("prefixLength: %1").arg(prefixLength), Devel);
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Call: %1").arg(_c), Devel);

    bool firstCharNumber = _c.at(0).isDigit();
    bool secondCharNumber = _c.at(1).isDigit();
    bool thirdCharNumber = _c.at(2).isDigit();

    //EA4, 2E3, E33, K1A, 3DA, FB1K  !2EE
    if (firstCharNumber && secondCharNumber)
    { // 11, 22
        //qDebug() << Q_FUNC_INFO << "END - 060 ";
        return false;
    }
    else if(firstCharNumber && !secondCharNumber && !thirdCharNumber)
    { // 2EE
        if (_c.left(3) != "3DA")
        {
            //qDebug() << Q_FUNC_INFO << "END - 070 ";
            return false;
        }
    }
    //qDebug() << Q_FUNC_INFO << "END";
    return true;

    logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - TRUE"), Debug);
}

bool Utilities::isAValidOperatingSuffix (const QString &_c)
{
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << _c;
    QStringList validSuffixes = {"A", "P", "Q", "AM", "M", "MM", "LH", "R", "J", "FF", "QRP", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    return validSuffixes.contains (_c);
}

bool Utilities::isAPrefix (const QString &_c)
{
    // Returns -1 if it is not a prefix or valid call.
    // Returns an int with the lenght of the prefix:
    // The length would be including the number, if possible EA4 or;
    // including just the country prefix: EA if the number is not included.
    // Prefixes are at least 1 chars (like in K), for K1K it should be 2 (K1)
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Start: %1").arg(_c), Debug);
    //qDebug() << Q_FUNC_INFO << "Call/Pref" << _c;
    QString aux = _c;
    if (aux.length()<1)
    {
        return false;
    }
    //qDebug() << Q_FUNC_INFO << "Call/Pref" << _c << "/" << getPrefixFromCall(_c);
    int length = (getPrefixFromCall(aux)).length();
    return (length>0);
}

void Utilities::setLongPrefixes (const QStringList &_p)
{
    //qDebug() << Q_FUNC_INFO << ": Start count: " << QString::number(_p.count());
    longPrefixes.clear();
    longPrefixes.append(_p);
    //qDebug() << Q_FUNC_INFO << ": count: " << QString::number(longPrefixes.count());
}

void Utilities::setSpecialCalls (const QStringList &_p)
{
    //qDebug() << Q_FUNC_INFO << ": Start count: " << QString::number(_p.count());
    specialCalls.clear();
    specialCalls.append(_p);
    //qDebug() << Q_FUNC_INFO << ": count: " << QString::number(specialCalls.count());
}

bool Utilities::isAKnownPrefix(const QString &_c)
{// TODO: Complete with https://rsgb.org/main/operating/licensing-novs-visitors/international-prefixes/
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    QString aux;
    if (_c.isNull() )
    {
        //qDebug() << Q_FUNC_INFO << ": END - 1";
        return false;
    }
    if (longPrefixes.count()<100)
    {
        //qDebug() << Q_FUNC_INFO << ": ********** END - FAIL";
    }
    //qDebug() << Q_FUNC_INFO << QString(": END - 2 - %1 - %2").arg(_c).arg(boolToQString(longPrefixes.contains(_c)));
    return longPrefixes.contains(_c);
}

bool Utilities::isAKnownCall(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    QString aux;
    if (_c.isNull() )
    {
        //qDebug() << Q_FUNC_INFO << ": END - 1";
        return false;
    }
    if (specialCalls.count()<100)
    {
        //qDebug() << Q_FUNC_INFO << ": ********** END - FAIL";
    }
    return specialCalls.contains(_c);
}

void Utilities::setCallValidation(const bool _b)
{
    //g_callsignCheck = _b;
    validateCalls = _b;
}

QString Utilities::getCheckedComplexCall(const QString &_c)
{ // Normalices calls
    //qDebug() << Q_FUNC_INFO << " - Start: " << _c;
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
    //qDebug() << Q_FUNC_INFO << " - END: " << call;
    return call;
}

QString Utilities::getMainCallFromComplexCall(const QString &_complexCall)
{ // Return the part of the call that provides entity and prefix information:
  // EA4K/F => F
  // VK9/EA4K => VK9
  // EA4K/MM => EA4K
  // MM/EA4K => MM

    QString call = _complexCall;
    if (call.length() == 1)
    {
        if ((call.at(0)).isLetterOrNumber())
        {
            return call;
        }
        else
        {
            return QString();
        }
    }

    //qDebug() << Q_FUNC_INFO << ": " << _complexCall;
    if (call.count('/') >1)
    { //Things like F/EA4K/P will become F/EA4K
        //logEvent (Q_FUNC_INFO, QString("With 2 /"), Debug);
        call = call.section("/", 0,1);
    }
    //qDebug() << Q_FUNC_INFO << ": First cut: " << call;
    call = getCheckedComplexCall(call);
    //qDebug() << Q_FUNC_INFO << ": Second cut: " << call;
    //qDebug() << Q_FUNC_INFO << " - 010" ;
    if (call.count('/') == 1)
    {// Complex calls (like F/EA4K - EA4K/F -EA4K/P - EA4K/1 - EA4K/qrp - VK9M/EA4K - EA4K/VK9M
     // K1B/QRP - B1A/VK9M
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        QString first = parts.at(0);
        QString second = parts.at(1);
        // First identify normal suffixes /P, /1, /QRP...
        bool firstCountry = !isAValidOperatingSuffix(first);
        bool secondCountry = !isAValidOperatingSuffix(second);
        //qDebug() << QString("First = %1, Second = %2").arg(boolToQString(firstCountry)).arg(boolToQString(secondCountry));
        if (!firstCountry)
        {
            if ((parts.at(0)) == "MM")
            { // Special case for Scotland EA4K/MM colliding as maritime movil
              // Scotland requires the prefix first like in MM/EA4K
                firstCountry=true;
            }
        }
        //qDebug() << Q_FUNC_INFO << " - 020" ;
        if (firstCountry && secondCountry)
        { // EA4K/VK9M
            //qDebug() << Q_FUNC_INFO << " - 030" ;
            bool firstIsPrefix = isAKnownPrefix(first);
            bool secondIsPrefix = isAKnownPrefix(second);
            if (firstIsPrefix && !secondIsPrefix)
            {
                //qDebug() << Q_FUNC_INFO << " END first - 033" ;
                return first;
            }
            else if (!firstIsPrefix && secondIsPrefix)
            {
                //qDebug() << Q_FUNC_INFO << "END second - 034" ;
                return second;
            }
            else
            {
                if (first.length()>second.length())
                {
                    call = second;
                }
                else
                {
                    call = first;
                }
            }
        }
        else if (isAValidOperatingSuffix(first))
        {//  1/EA4K
         // TODO: Here the perfect way should be to obtain the country and
         // add the number, if area is requested
            call = second;
        }
        else if (isAValidOperatingSuffix(second))
        {//  EA4K/2
            call = first;
        }
        else
        { // None is a normal country prefix
            return QString();
        }
    }
    return call;
}

bool Utilities::isValidCall(const QString &_c, bool _force)
{// https://life.itu.int/radioclub/rr/art19.pdf
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Start = %1").arg(_c), Debug);
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << "Start: " << _c;
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    // Prefixes are at least 2 chars

    if ((!validateCalls) && (!_force))
    {
        //qDebug() << Q_FUNC_INFO << "001 - Not validating calls: " << _c;
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 001 - true"), Debug);
        return true;
    }

    //qDebug() << Q_FUNC_INFO << " - Long prefixes: " << QString::number(longPrefixes.count());
    if (longPrefixes.count()<100)
    {
        //qDebug() << Q_FUNC_INFO << "Long prefixes < 100 " << _c;
        return false;
    }
    QString call = _c;
    //qDebug() << Q_FUNC_INFO << "000 " << _c;
    if (isAKnownCall(call))
    {
        //qDebug() << Q_FUNC_INFO << "001 - Known call: " << _c;
        return true;
    }
    //qDebug() << Q_FUNC_INFO << "- 002 " << call;
    if (call.length()<3)
    {
        //qDebug() << Q_FUNC_INFO << "- 003 " << call;
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 010 - False"), Debug);
        return false;
    }
    //call = getMainCallFromComplexCall(call);
    //qDebug() << Q_FUNC_INFO << "- 004 :" << call;

    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 010"), Devel);
    if (call.count('\\')>0)
    {
        call.replace('\\', '/');
    }
    //qDebug() << Q_FUNC_INFO << " -005";

    if (call.count('/')>2)
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 015 - false"), Debug);
        //qDebug() << Q_FUNC_INFO << " -005.5";
        return false;
    }
    //qDebug() << Q_FUNC_INFO << " -006";
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 020"), Devel);
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 020";
    if (call.count('/') == 2)
    { //Things like F/EA4K/P will become F/EA4K
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Two /; Ignoring the last part: %1").arg(call), Devel);
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        call = parts.at(0) + "/" + parts.at(1);
    }
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString(" - 025: %1").arg(call), Devel);
    //qDebug() << Q_FUNC_INFO << " -025";

    if (call.count('/') == 1)
    { // Complex calls (like F/EA4K or EA4K/F OR /p OR /qrp
      // We are just checking the call format not if it belongs to a country or whatever.
      // It may return true for wrong calls like "ABC/EA4K"
      // TODO: Add a check just for prefixes to fix the previous
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString(" - Call with one /: %1").arg(call), Devel);
        QStringList parts;
        parts.clear();
        parts << call.split ('/');
        //EA4K/P

        bool result1 = ((isAPrefix (parts.at (0))) || (isValidSimpleCall (parts.at(0))));
        //qDebug() << Q_FUNC_INFO << " -027";
        bool result2 = ((isAPrefix (parts.at (1))) || (isValidSimpleCall (parts.at(1))) || isAValidOperatingSuffix(parts.at(1)) );
        //qDebug() << Q_FUNC_INFO << parts.at(0) << "/" << parts.at(1);
        //qDebug() << Q_FUNC_INFO << QString("Result1=%1").arg(boolToQString(result1));
        //qDebug() << Q_FUNC_INFO << QString("Result2=%1").arg(boolToQString(result2));
        //qDebug() << Q_FUNC_INFO << QString("Detailed=%1/%2/%3").arg(boolToQString((isAPrefix (parts.at (1))) )).arg(boolToQString((isValidSimpleCall (parts.at(1))))).arg(boolToQString(isAValidOperatingSuffix(parts.at(1))));
        //qDebug() << Q_FUNC_INFO << "END1";
        return (result1 && result2);
    }
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - %1").arg(isValidSimpleCall(call)), Debug);
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - END";

    return isValidSimpleCall(call);
}

QString Utilities::getPrefixFromCall(const QString &_c, bool withAreaNumber)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c << " - WithAreaNumber=" << boolToQString(withAreaNumber);
    if (_c.isNull())
    {
        return QString();
    }

    QString call = _c;
    int length = call.length();

    bool firstSpecial = isAOneLetterPrefix(call.at(0));
    if (length==1)
    {
        if (firstSpecial)
        {
            return _c;
        }
        else
        {
            return QString();
        }
    }
    //qDebug() << Q_FUNC_INFO << " - Going to calculate - length = " << QString::number(length);
    if (length>2)
    {
        //qDebug() << Q_FUNC_INFO << " - Going to Look for a known call" ;
        if (isAKnownCall(_c))
        {
            //qDebug() << Q_FUNC_INFO << " - Known CALL found!";
            return _c;
        }
         //qDebug() << Q_FUNC_INFO << " - Call not found!" ;
    }

    //qDebug() << Q_FUNC_INFO << " - 010";
    call = getMainCallFromComplexCall(call);
    //qDebug() << Q_FUNC_INFO << " - 011: " << call;

    QString call2 = call;
    QString call3 = call;
    int i = length;
    while (i>0)
    {
        //qDebug() << Q_FUNC_INFO << QString(" While (i=%1) = ").arg(i) << call;
        if (isAKnownPrefix(call))
        {// EA, EA6, VK9N, VP2E, K, K1, KN1, 4U1I
            //qDebug() << Q_FUNC_INFO << QString("- Known prefix found: %1").arg(call);
            if (withAreaNumber)
            {
                //qDebug() << Q_FUNC_INFO << QString("- With Area number");
                if ( (call2.back()).isDigit() )
                {
                    //qDebug() << Q_FUNC_INFO << QString("- With Area number -call2- & last is a digit");
                    return call2;
                }
                else if((call3.back()).isDigit())
                {
                    //qDebug() << Q_FUNC_INFO << QString("- With Area number -call3- & last is a digit");
                    return call3;
                }
            }
            //qDebug() << Q_FUNC_INFO << QString("- With NO Area number");
            return call;
        }
        //qDebug() << Q_FUNC_INFO << QString("- Known prefix NOT found: %1").arg(call);
        call3 = call2;
        call2 = call;
        call.chop(1);
        i--;
    }
    return QString();
    /*
    QString call2 = call;
    QString call3 = call;
    bool keepAnalyzing  = true;
    bool found = false;
    bool secondCharIsNumber = false;

    if ((call.at(1)).isDigit())
    {
        secondCharIsNumber = true;
    }

    //GW1A
    while (keepAnalyzing)
    {
        //qDebug() << Q_FUNC_INFO << " - call: " << call;
        //qDebug() << Q_FUNC_INFO << " - call2: " << call2;
        //qDebug() << Q_FUNC_INFO << " - call3: " << call3;
        if (isAKnownPrefix(call))
        {
            //qDebug() << Q_FUNC_INFO << ": Found!: " << call;
            keepAnalyzing = false;
            found = true;
        }
        else if (call.length()<=2)
        {
           //qDebug() << Q_FUNC_INFO << ": Not found & call<=2: " << call;
           keepAnalyzing = false;
        }
        else
        {
            //qDebug() << Q_FUNC_INFO << ": else, chop: " << call;
            call3 = call2;
            call2 = call;
            call.chop(1);
        }
    }

    //qDebug() << Q_FUNC_INFO << ": **** Out of while: " << call << "/" << call2 << "/" << call3;
    if (found)
    {
        //qDebug() << Q_FUNC_INFO << ": END-0 " << call;
        return call;
    }
    else
    {
        if (firstSpecial)
        {
            call3 = call2;
            call2 = call;
            call.chop(1);
        }
        //qDebug() << Q_FUNC_INFO << ": **** Out of while Special 1-Letter: " << call << "/" << call2 << "/" << call3;
    }

    //qDebug() << Q_FUNC_INFO << ": **** To process: " << call << "/" << call2 << "/" << call3;
    if (withAreaNumber)
    {
        //qDebug() << Q_FUNC_INFO << ": WithAreaNumber";
        if (firstSpecial)
        {
            //qDebug() << Q_FUNC_INFO << ": FirstSpecial";
            int last = call2.length();
            //qDebug() << Q_FUNC_INFO << ": last="<< QString::number(last);
            if (call.length()==1)
            {
                //qDebug() << Q_FUNC_INFO << ": END0: " << call;
                return call;
            }
            else if (((call2.at(last-1)).isDigit()))
            {//K1
                //qDebug() << Q_FUNC_INFO << ": END1: " << call2;
                return call2;
            }
            else if ((call3.at(call3.length()-1)).isDigit())
            {//KB1
                //qDebug() << Q_FUNC_INFO << ": END2: " << call3;
                return call3;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << ": END3: " << call;
                return call;
            }
        }
        else
        {
            if (call == call2)
            {
                //qDebug() << Q_FUNC_INFO << ": END4: " << call;
                return call;
            }

            int last = call2.length();
            if (((call2.at(last-1)).isDigit()))
            {
                //qDebug() << Q_FUNC_INFO << ": END5: " << call2;
                return call2;
            }
            //qDebug() << Q_FUNC_INFO << ": END6: " << call;
            //return call;
        }
    }
//Si se pide el numero pero el prefijo no trae numero se devuelve sin numero.
    if (firstSpecial)
    {
        //qDebug() << Q_FUNC_INFO << ": call2: " << call2;
        if (isAKnownPrefix(call2))
        {
            //qDebug() << Q_FUNC_INFO << ": END7.1: " << call;
            return call2;
        }
        else if (!secondCharIsNumber && (call.at(3)).isDigit())
        {
            //qDebug() << Q_FUNC_INFO << ": END7.2: " << call;
            return call;
        }
        //qDebug() << Q_FUNC_INFO << ": END7.3: " << call;
        //return call;
    }
    int last = call2.length();
    if (!((call2.at(last-1)).isDigit()))
    {
        if (isAKnownPrefix(call))
        {
            //qDebug() << Q_FUNC_INFO << ": END7.5: " << call2;
            return call;
        }
        //qDebug() << Q_FUNC_INFO << ": END8: " << call2;
        return call2;
    }
    //qDebug() << Q_FUNC_INFO << ": END9: " << call;
    return call;
    */
}

bool Utilities::isAOneLetterPrefix(const QChar &_c)
{
    QList<QChar> validFirstLettersOnly = {'B', 'F', 'G', 'I', 'K', 'M', 'N', 'R', 'W'};
    return validFirstLettersOnly.contains (_c);
}

bool Utilities::isSameFreq(const double fr1, const double fr2)
{
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << QString::number(fr1) << "/" << QString::number(fr2) << " = " << QString::number(fabs(fr1 - fr2)) ;

    if (fabs(fr1 - fr2) < 0.001)
    {
        //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - true" ;
        return true;
    }
    else
    {
        //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - false" ;
        return false;
    }
    //return fabs(fr1 - fr2) < 0.001;
}

bool Utilities::isValidBandId(const int _b)
{
    if (_b>0)
    {
        return true;
    }
    else
    {
        return false;
    }
    //return false;
}

bool Utilities::isValidModeId(const int _m)
{
    return (_m>0);
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

bool Utilities::isValidVUCCGrids(const QString &_b)
{
    //qDebug() << Q_FUNC_INFO << ": " << _b;
    if(_b.endsWith (','))
        return false;
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << _b;
    QStringList tmp;

    //QString a = _b;
    tmp.clear ();
    tmp << _b.split (',', QT_SKIP);
   //qDebug() << Q_FUNC_INFO << ": 10";
    if ((tmp.length () !=2) && (tmp.length () !=4) )
    {
        //qDebug() << Q_FUNC_INFO << ": 11 - not valid lenght";
        return false;
    }
    //qDebug() << Q_FUNC_INFO << ": 20";

    QString aux;
    foreach (aux, tmp) {
        aux = aux.trimmed ();

        //if ((!isValidGrid (aux)) || (aux.length ()!=4))
        if (!isValidGrid (aux))
        {
           //qDebug() << Q_FUNC_INFO << ": 30 - non valid: " << aux;
            return false;
        }
        else
        {
           //qDebug() << Q_FUNC_INFO << ": 31 - valid";
        }
    }
   //qDebug() << Q_FUNC_INFO << ": 100 - VALID END";
    return true;
}

bool Utilities::isValidRST(const QString &_b)
{
    return (_b.length()>0);
}

bool Utilities::isValidPower(const QString &_b)
{
    return (_b.toDouble()>0);
}

bool Utilities::isValidComment(const QString &_b)
{
    return (_b.length()>0);
}

bool Utilities::isValidName(const QString &_b)
{
    return (_b.length()>0);
}

bool Utilities::isDBFileExisting()
{
    //qDebug() << "Utilities::isDBFileExisting: " << getKLogDBFile() ;
    return (QFile::exists(getKLogDBFile()));
}

bool Utilities::isDBFileExisting(const QString &_file)
{
         //qDebug() << "Utilities::isDBFileExisting2: " << _file ;

    return (QFile::exists(_file));
}

bool Utilities::isValidADIFField(const QString &_b)
{
       //qDebug() << "Utilities::isValidADIFField: " << _b ;
    /*
        This functions checks if the ADIF field has the proper format.
        <Field:length:Data type>Data
    */

    if (!((_b.startsWith('<')) &&  (_b.count('>')) == 1 ))
    {
           //qDebug() << "Utilities::isValidADIFField: BAD FORMAT: No < or > delimiters: " << _b ;
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
           //qDebug() << "Utilities::isValidADIFField-0 (not two): " << QString::number(qs.size()) ;
        return false;
    }

    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

      //qDebug() << "Utilities::isValidADIFField-Field: " << field ;
      //qDebug() << "Utilities::isValidADIFField_Data: " << data ;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
        if (!validDataTypes.contains(dataType.toUpper()))
        {
               //qDebug() << "Utilities::isValidADIFField - FORMAT ERROR: Wrong data type: " << dataType ;
            return false;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
           //qDebug() << "Utilities::isValidADIFField - FORMAT ERROR, more than 2 \":\" - " << field ;
        return false;
    }

    if ( length != separatorPosition)
    {
           //qDebug() << "Utilities::isValidADIFField: Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) ;
        return false;
    }

    if (separatorPosition <= 0)
    {
        //qDebug() << "Utilities::isValidADIFField: Length problem <= 0" ;
        return false;
    }
       //qDebug() << "FileManager::checkADIFValidFormat: Return true" ;
    return true;
}

bool Utilities::isValidQSL_Rcvd(const QString &c)
{
    return ((c == "Y") || (c == "N") || (c == "R") || (c == "I") || (c == "V"));
}

bool Utilities::isValidQSL_Sent(const QString &c)
{
    return ((c == "Y") || (c == "N") || (c == "R") || (c == "Q") || (c == "I"));
}

bool Utilities::isValidUpload_Status(const QString &c)
{
    return ((c == "Y") || (c == "N") || (c == "M"));
}

QStringList Utilities::getValidADIFFieldAndData(const QString &_b)
{
   //qDebug() << "Utilities::getValidADIFFieldAndData: " << _b ;
    /*
        This functions checks if the ADIF field has the proper format.
        <Field:length:Data type>Data
    */
    QStringList result;
    result.clear();

    if (!(_b.startsWith('<')))
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: BAD FORMAT: No < or > delimiters: " << _b ;
        return QStringList();
    }
    if (_b.simplified() == "<EOR>")
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: EOR" ;
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

    //qDebug() << "Utilities::getValidADIFFieldAndData: -20" ;
    QStringList validDataTypes = {"B", "N", "D", "T", "S", "I", "M", "G", "E", "L"};

    qs.clear();
    qs.append(aux.split('>'));

    if (qs.size()!= 2)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData-0 (not two): " << QString::number(qs.size()) ;
        return result;
    }
    //qDebug() << "Utilities::getValidADIFFieldAndData: -30" ;
    //QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

    //qDebug() << "Utilities::getValidADIFFieldAndData-Field: " << field ;
    //qDebug() << "Utilities::getValidADIFFieldAndData_Data: " << data ;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
          //qDebug() << "Utilities::getValidADIFFieldAndData - DataType: -" << dataType << "-" ;
        if (!validDataTypes.contains(dataType.toUpper()))
        {
              //qDebug() << "Utilities::getValidADIFFieldAndData - FORMAT ERROR: Wrong data type: " << dataType ;
            return result;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData - FORMAT ERROR, more than 2 \":\" - " << field ;
        return result;
    }
    //qDebug() << "Utilities::getValidADIFFieldAndData: -60" ;
    if ( length != separatorPosition)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) ;
        return result;
    }

    if (separatorPosition <= 0)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: Length problem <= 0" ;
        return result;
    }
     //qDebug() << "Utilities::getValidADIFFieldAndData: -90: f: " << field ;
      //qDebug() << "Utilities::getValidADIFFieldAndData: -90: d: " << data<< endl;
    //field = field.section(':', 0, 0);
    result.clear();
    result << field.section(':', 0, 0) << data;
     //qDebug() << "Utilities::checkADIFValidFormat: Return true: " << result.at(0) << "/" << result.at(1) ;
    return result;
}

QString Utilities::getAValidCall (const QString &_wrongCall)
{
    //qDebug() << "Utilities::getAValidCall: " << _wrongCall ;
    QString _confirmedCall;
    _confirmedCall.clear();

    bool ok;
    if (_wrongCall.length() > 0)
    {
        //qDebug() << "Utilities::getAValidCall (Don't have VALID CALL): " << _wrongCall ;
        _confirmedCall = QString(QObject::tr("A wrong callsign has been found: %1. Please enter a new callsign or confirm that the current one is a good callsign.")).arg(_wrongCall);
    }
    else
    {
        //qDebug() << "Utilities::getAValidCall (Don't have ANY CALL): " << _wrongCall ;
        _confirmedCall = QString(QObject::tr("An empty callsign has been detected. If it is possible, please enter the right call."));
    }

    QString text = QInputDialog::getText(nullptr, QObject::tr("KLog - Not valid callsign found"),
                                               _confirmedCall, QLineEdit::Normal, _wrongCall, &ok);
    if (!(ok && isValidCall(text)))
    {
        _confirmedCall = text;
    }
    else
    {
        _confirmedCall = QString();
    }
    //qDebug() << "Utilities::getAValidCall: " << _confirmedCall ;
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
{
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
   //qDebug() << "Utilities::getDateFromADIFDateString: " << _s ;
    return QDate::fromString(_s, "yyyyMMdd");
}

QTime Utilities::getTimeFromADIFTimeString(const QString &_s)
{// Expects and ADIF TIME format String "HHMMSS" or "HHMM"
    //qDebug() << "Utilities::getTimeFromADIFTimeString: " << _s ;
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
    //qDebug() << "Utilities::getDateFromLoTWQSLDateString: " << _s ;
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
    //qDebug() << "Utilities::getDateFromLoTWQSLDateString: " << datet.at(0) ;
    return QDate::fromString(datet.at(0), "yyyy-MM-dd");
}


QString Utilities::getADIFDateFromQDateTime(const QDateTime &_d)
{
    if (!_d.isValid())
    {
        return QString();
    }
    else
    {
        return _d.date().toString("yyyyMMdd");
    }
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
    if (!_d.isValid())
    {
        return QString();
    }
    else
    {
        return _d.time().toString("hhmmss");
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

void Utilities::setDarkMode(const QString &_dm)
{
    darkMode = trueOrFalse(_dm);
}

bool Utilities::isDarkMode()
{
    return darkMode;
}

void Utilities::setLogColumnNames()
{
    // QMap<QString, QString> columnNames;
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
    columnNames.insert("clublog_qso_upload_date", QObject::tr("ClubLog SDate"));
    columnNames.insert("clublog_qso_upload_status", QObject::tr("ClubLog status"));
    columnNames.insert("cont", QObject::tr("Continent"));
    columnNames.insert("contacted_op", QObject::tr("Contacted_Op"));
    columnNames.insert("contest_id", QObject::tr("Contest Id"));
    columnNames.insert("country", QObject::tr("Country"));
    columnNames.insert("credit_submitted", QObject::tr("Credit Submitted"));
    columnNames.insert("credit_granted", QObject::tr("Credit granted"));
    columnNames.insert("dark_dok", QObject::tr("Dark Dok", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("distance", QObject::tr("Distance"));
    columnNames.insert("email", QObject::tr("Email"));
    columnNames.insert("eq_call", QObject::tr("EQ_Call"));
    columnNames.insert("eqsl_qslrdate", QObject::tr("eQSL RDate"));
    columnNames.insert("eqsl_qslsdate", QObject::tr("eQSL SDate"));
    columnNames.insert("eqsl_qsl_rcvd", QObject::tr("eQSL Rcvd"));
    columnNames.insert("eqsl_qsl_sent", QObject::tr("eQSL Sent"));
    columnNames.insert("fists", QObject::tr("Fists", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("fists_cc", QObject::tr("Fists CC", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("force_init", QObject::tr("Force Init"));
    columnNames.insert("freq", QObject::tr("Freq"));
    columnNames.insert("freq_rx", QObject::tr("Freq RX"));
    columnNames.insert("gridsquare", QObject::tr("Gridsquare"));
    columnNames.insert("guest_op", QObject::tr("Guest OP"));
    columnNames.insert("hrdlog_qso_upload_date", QObject::tr("HRDLog SDate"));
    columnNames.insert("hrdlog_qso_upload_status", QObject::tr("HRDLog status"));
    columnNames.insert("iota", QObject::tr("IOTA"));
    columnNames.insert("iota_island_id", QObject::tr("IOTA Island id"));
    columnNames.insert("k_index", QObject::tr("K Index"));
    columnNames.insert("lat", QObject::tr("Lat"));
    columnNames.insert("lon", QObject::tr("Lon"));
    columnNames.insert("lotw_qslrdate", QObject::tr("LoTW RDate"));
    columnNames.insert("lotw_qslsdate", QObject::tr("LoTW SDate"));
    columnNames.insert("lotw_qsl_rcvd", QObject::tr("LoTW Rcvd"));
    columnNames.insert("lotw_qsl_sent", QObject::tr("LoTW Sent"));
    columnNames.insert("max_bursts", QObject::tr("Max Bursts"));
    columnNames.insert("multiplier", QObject::tr("Multiplier"));
    columnNames.insert("ms_shower", QObject::tr("MS Shower"));
    columnNames.insert("my_antenna", QObject::tr("My Antenna"));
    columnNames.insert("my_city", QObject::tr("My City"));
    columnNames.insert("my_cnty", QObject::tr("My Cnty"));
    columnNames.insert("my_country", QObject::tr("My Country"));
    columnNames.insert("my_cq_zone", QObject::tr("My CQz"));
    columnNames.insert("my_dxcc", QObject::tr("My DXCC"));
    columnNames.insert("my_fists", QObject::tr("My Fists", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("my_gridsquare", QObject::tr("My Gridsquare"));
    columnNames.insert("my_iota", QObject::tr("My IOTA"));
    columnNames.insert("my_iota_island_id", QObject::tr("My IOTA island id"));
    columnNames.insert("my_itu_zone", QObject::tr("My ITUz"));
    columnNames.insert("my_lat", QObject::tr("My Lat"));
    columnNames.insert("my_lon", QObject::tr("My Lon"));
    columnNames.insert("my_name", QObject::tr("My Name"));
    columnNames.insert("my_postal_code", QObject::tr("My Postal code"));
    columnNames.insert("my_rig", QObject::tr("My Rig"));
    columnNames.insert("my_sig", QObject::tr("My Sig"));
    columnNames.insert("my_sig_info", QObject::tr("My Sig Info"));
    columnNames.insert("my_sota_ref", QObject::tr("My SOTA ref"));
    columnNames.insert("my_state", QObject::tr("My State"));
    columnNames.insert("my_street", QObject::tr("My Street"));
    columnNames.insert("my_usaca_counties", QObject::tr("My USACA counties"));
    columnNames.insert("my_vucc_grids", QObject::tr("My VUCC grids"));
    columnNames.insert("name", QObject::tr("Name"));
    columnNames.insert("notes", QObject::tr("Notes"));
    columnNames.insert("nr_bursts", QObject::tr("Nr bursts", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("nr_pings", QObject::tr("Nr pings", "Do not translate if unsure, common hamradio term."));
    columnNames.insert("operator", QObject::tr("Operator"));
    columnNames.insert("owner_callsign", QObject::tr("Owner Callsign"));
    columnNames.insert("pfx", QObject::tr("Pfx"));
    columnNames.insert("points", QObject::tr("Points"));
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
    columnNames.insert("qso_date_off", QObject::tr("QSO Date off"));
    columnNames.insert("transmiterid", QObject::tr("Transmitter id"));
    columnNames.insert("lognumber", QObject::tr("Log number"));
}
QString Utilities::getLogColumnName(const QString &_column)
{
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << _column;


    QString aux;
    aux = columnNames.value(_column);
    if (aux.length()>0)
    {
        return aux;
    }
    return _column;
/*
    if (_column == "qso_date")
        return QObject::tr("Date");
    else if (_column == "call")
        return QObject::tr("Call");
    else if (_column == "rst_sent")
        return QObject::tr("RSTtx");
    else if (_column == "rst_rcvd")
        return QObject::tr("RSTrx");
    else if (_column == "bandid")
        return QObject::tr("Band");
    else if (_column == "comment")
        return QObject::tr("Comment");
    else if (_column == "modeid")
        return QObject::tr("Mode");
    else if (_column == "cqz")
        return QObject::tr("CQz");
    else if (_column == "ituz")
        return QObject::tr("ITUz");
    else if (_column == "dxcc")
        return QObject::tr("DXCC");
    else if (_column == "address")
        return QObject::tr("Address");
    else if (_column == "age")
        return QObject::tr("Age");
    else if (_column == "cnty")
        return QObject::tr("County");
    else if (_column == "a_index")
        return QObject::tr("A_Index");
    else if (_column == "ant_az")
        return QObject::tr("Ant_Az");
    else if (_column == "ant_el")
        return QObject::tr("Ant_El");
    else if (_column == "ant_path")
        return QObject::tr("Ant_Path");
    else if (_column == "arrl_sect")
        return QObject::tr("ARRL_SECT");
    else if (_column == "award_submitted")
        return QObject::tr("Award_Submitted");
    else if (_column == "award_granted")
        return QObject::tr("Award_granted");
    else if (_column == "band_rx")
        return QObject::tr("Band_RX");
    else if (_column == "checkcontest")
        return QObject::tr("CheckContest");
    else if (_column == "class")
        return QObject::tr("Class");
    else if (_column == "clublog_qso_upload_date")
        return QObject::tr("ClubLog SDate");
    else if (_column == "clublog_qso_upload_staus")
        return QObject::tr("ClubLog status");
    else if (_column == "cont")
        return QObject::tr("Continent");
    else if (_column == "contacted_op")
        return QObject::tr("Contacted Op");
    else if (_column == "contest_id")
        return QObject::tr("Contest Id");
    else if (_column == "country")
        return QObject::tr("Country");
    else if (_column == "credit_submitted")
        return QObject::tr("Credit Submitted");
    else if (_column == "credit_granted")
        return QObject::tr("Credit granted");
    else if (_column == "dark_dok")
        return QObject::tr("Dark Dok", "Do not translate if unsure, common hamradio term.");
    else if (_column == "distance")
        return QObject::tr("Distance");
    else if (_column == "email")
        return QObject::tr("Email");
    else if (_column == "eq_call")
        return QObject::tr("EQ_Call");
    else if (_column == "eqsl_qslrdate")
        return QObject::tr("eQSL RDate");
    else if (_column == "eqsl_qslsdate")
        return QObject::tr("eQSL SDate");
    else if (_column == "eqsl_qsl_rcvd")
        return QObject::tr("eQSL Rcvd");
    else if (_column == "eqsl_qsl_sent")
        return QObject::tr("eQSL Sent");
    else if (_column == "fists")
        return QObject::tr("Fists", "Do not translate if unsure, common hamradio term.");
    else if (_column == "fists_cc")
        return QObject::tr("Fists CC", "Do not translate if unsure, common hamradio term.");
    else if (_column == "force_init")
        return QObject::tr("Force Init");
    else if (_column == "freq")
        return QObject::tr("Freq");
    else if (_column == "freq_rx")
        return QObject::tr("Freq RX");
    else if (_column == "gridsquare")
        return QObject::tr("Gridsquare");
    else if (_column == "guest_op")
        return QObject::tr("Guest OP");
    else if (_column == "hrdlog_qso_upload_date")
        return QObject::tr("HRDLog SDate");
    else if (_column == "hrdlog_qso_upload_status")
        return QObject::tr("HRDLog status");
    else if (_column == "iota")
        return QObject::tr("IOTA");
    else if (_column == "iota_island_id")
        return QObject::tr("IOTA Island id");
    else if (_column == "k_index")
        return QObject::tr("K Index");
    else if (_column == "lat")
        return QObject::tr("Lat");
    else if (_column == "lon")
        return QObject::tr("Lon");
    else if (_column == "lotw_qslrdate")
        return QObject::tr("LoTW RDate");
    else if (_column == "lotw_qslsdate")
        return QObject::tr("LoTW SDate");
    else if (_column == "lotw_qsl_rcvd")
        return QObject::tr("LoTW Rcvd");
    else if (_column == "lotw_qsl_sent")
        return QObject::tr("LoTW Sent");
    else if (_column == "max_bursts")
        return QObject::tr("Max Bursts");
    else if (_column == "multiplier")
        return QObject::tr("Multiplier");
    else if (_column == "ms_shower")
        return QObject::tr("MS Shower");
    else if (_column == "my_antenna")
        return QObject::tr("My Antenna");
    else if (_column == "my_city")
        return QObject::tr("My City");
    else if (_column == "my_cnty")
        return QObject::tr("My Cnty");
    else if (_column == "my_country")
        return QObject::tr("My Country");
    else if (_column == "my_cq_zone")
        return QObject::tr("My CQz");
    else if (_column == "my_dxcc")
        return QObject::tr("My DXCC");
    else if (_column == "my_fists")
        return QObject::tr("My Fists", "Do not translate if unsure, common hamradio term.");
    else if (_column == "my_gridsquare")
        return QObject::tr("My Gridsquare");
    else if (_column == "my_iota")
        return QObject::tr("My IOTA");
    else if (_column == "my_iota_island_id")
        return QObject::tr("My IOTA island id");
    else if (_column == "my_itu_zone")
        return QObject::tr("My ITUz");
    else if (_column == "my_lat")
        return QObject::tr("My Lat");
    else if (_column == "my_lon")
        return QObject::tr("My Lon");
    else if (_column == "my_name")
        return QObject::tr("My Name");
    else if (_column == "my_postal_code")
        return QObject::tr("My Postal code");
    else if (_column == "my_rig")
        return QObject::tr("My Rig");
    else if (_column == "my_sig")
        return QObject::tr("My Sig");
    else if (_column == "my_sig_info")
        return QObject::tr("My Sig Info");
    else if (_column == "my_sota_ref")
        return QObject::tr("My SOTA ref");
    else if (_column == "my_state")
        return QObject::tr("My State");
    else if (_column == "my_street")
        return QObject::tr("My Street");
    else if (_column == "my_usaca_counties")
        return QObject::tr("My USACA counties");
    else if (_column == "my_vucc_grids")
        return QObject::tr("My VUCC grids");
    else if (_column == "name")
        return QObject::tr("Name");
    else if (_column == "notes")
        return QObject::tr("Notes");
    else if (_column == "nr_bursts")
        return QObject::tr("Nr bursts", "Do not translate if unsure, common hamradio term.");
    else if (_column == "nr_pings")
        return QObject::tr("Nr pings", "Do not translate if unsure, common hamradio term.");
    else if (_column == "operator")
        return QObject::tr("Operator");
    else if (_column == "owner_callsign")
        return QObject::tr("Owner Callsign");
    else if (_column == "pfx")
        return QObject::tr("Pfx");
    else if (_column == "points")
        return QObject::tr("Points");
    else if (_column == "precedence")
        return QObject::tr("Precedence");
    else if (_column == "prop_mode")
        return QObject::tr("Prop Mode");
    else if (_column == "public_key")
        return QObject::tr("Public Key");
    else if (_column == "qrzcom_qso_upload_date")
        return QObject::tr("QRZcom SDate");
    else if (_column == "qrzcom_qso_upload_status")
        return QObject::tr("QRZcom status");
    else if (_column == "qslmsg")
        return QObject::tr("QSL msg");
    else if (_column == "qslrdate")
        return QObject::tr("QSL RDate");
    else if (_column == "qslsdate")
        return QObject::tr("QSL SDate");
    else if (_column == "qsl_rcvd")
        return QObject::tr("QSL Rcvd");
    else if (_column == "qsl_sent")
        return QObject::tr("QSL Sent");
    else if (_column == "qsl_rcvd_via")
        return QObject::tr("QSL rcvd via");
    else if (_column == "qsl_sent_via")
        return QObject::tr("QSL sent via");
    else if (_column == "qsl_via")
        return QObject::tr("QSL via");
    else if (_column == "qso_complete")
        return QObject::tr("QSO complete");
    else if (_column == "qso_random")
        return QObject::tr("QSO random");
    else if (_column == "qth")
        return QObject::tr("QTH");
    else if (_column == "region")
        return QObject::tr("Region");
    else if (_column == "rig")
        return QObject::tr("Rig");
    else if (_column == "rx_pwr")
        return QObject::tr("RX Pwr");
    else if (_column == "sat_mode")
        return QObject::tr("Sat mode");
    else if (_column == "sat_name")
        return QObject::tr("Sat name");
    else if (_column == "sfi")
        return QObject::tr("SFI");
    else if (_column == "sig")
        return QObject::tr("Sig");
    else if (_column == "sig_info")
        return QObject::tr("Sig Info");
    else if (_column == "silent_key")
        return QObject::tr("Silent key", "Do not translate if unsure, common hamradio term.");
    else if (_column == "skcc")
        return QObject::tr("SKCC");
    else if (_column == "sota_ref")
        return QObject::tr("SOTA Ref");
    else if (_column == "srx_string")
        return QObject::tr("SRX String");
    else if (_column == "srx")
        return QObject::tr("SRX");
    else if (_column == "stx_string")
        return QObject::tr("STX String");
    else if (_column == "state")
        return QObject::tr("State");
    else if (_column == "station_callsign")
        return QObject::tr("Station Callsign");
    else if (_column == "submode")
        return QObject::tr("Submode");
    else if (_column == "swl")
        return QObject::tr("SWL", "Do not translate if unsure, common hamradio term.");
    else if (_column == "uksmg")
        return QObject::tr("UKSMG");
    else if (_column == "usaca_counties")
        return QObject::tr("USACA counties");
    else if (_column == "ve_prov")
        return QObject::tr("VE prov");
    else if (_column == "vucc_grids")
        return QObject::tr("VUCC grids");
    else if (_column == "ten_ten")
        return QObject::tr("Ten-Ten", "Do not translate, it is a hamradio group name.");
    else if (_column == "tx_pwr")
        return QObject::tr("TX Pwr");
    else if (_column == "web")
        return QObject::tr("Web");
    else if (_column == "qso_date_off")
        return QObject::tr("QSO Date off");
    else if (_column == "transmiterid")
        return QObject::tr("Transmitter id");
    else if (_column == "lognumber")
        return QObject::tr("Log number");
    else
    {
        return _column;
    }
    */
}

bool Utilities::isValidContinent(const QString &_s)
{
    return (continent.contains (_s.toUpper ()));
}

bool Utilities::isValidPropMode(const QString &_s)
{
    QStringList propModes;
    propModes.clear ();
    propModes << "AS" << "AUE" << "AUR" << "BS" << "ECH" << "EME" << "ES"
                << "F2" << "FAI" << "GWAVE" << "INTERNET" << "ION" << "IRL"
                << "LOS" << "MS" << "RPT" << "RS" << "SAT" << "TEP" << "TR";
    return propModes.contains (_s.toUpper ());
}

bool Utilities::isValidDistance(const double _d)
{
    return (_d>=0);
}

bool Utilities::isValidSponsor(const QString &_s)
{
    return (sponsorsList.contains (_s.toUpper ()));
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
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << _func << ": " << _level << ": " << _msg;
    if (logLevel<=_level)
    {
    //qDebug() << _func << ": " << _level << ": " << _msg;
        emit debugLog (_func, _msg, _level);
    }
    */
}
