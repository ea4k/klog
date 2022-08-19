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

bool g_callsignCheck;
Utilities::Utilities(const QString &_parentName)
{
    parentName = _parentName;
    //qDebug() << "Utilities::Utilities"  ;
    init();

    //palRed.setColor(QPalette::Text, Qt::red);
    //palBlack.setColor(QPalette::Text, Qt::black);
    //qDebug() << "Utilities::Utilities - END"  ;
}

Utilities::~Utilities()
{
}

void Utilities::init()
{
    softwareVersion = "0.0";
    longPrefixes.clear();
    darkMode = false;
    logLevel = None;
    setARRLSect();
    setContinent();
    setSponsorsList();
    logLevels.clear();
    logLevels << "None" << "Info" << "Debug" << "Devel";
    //callValidation = true;
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

bool Utilities::isValidSubCall(const QString &_c)
{
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Start:  %1").arg(_c), Debug);
    // This functions only checks simple calls like EA4K, not composed like EA4K/F of F/EA4K/QRP
    //Rules: http://life.itu.int/radioclub/rr/art19.pdf
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << QString(" - 000 - %1").arg(_c);
    if (_c.contains ('/'))
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Contains / - FALSE"), Debug);
        return false;
    }
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 010";
    if (_c.length()<3)
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Less than 3 chars - FALSE"), Debug);
        return false;
    }
    if (isALongCountryPrefix(_c))
    {
        return false;
    }
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 020";
    if (!(_c.at(_c.length ()-1).isLetter ()))
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Does not end with a char - FALSE"), Debug);
        return false;
    }
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 030";
    for (int i = 0; i<_c.length ();i++)
    {
        if (!_c.at(i).isLetterOrNumber ())
        {
            // Non valid chars
            //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Non valid chars - FALSE"), Debug);
            return false;
        }
    }
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 040";
    int prefixLength = isAPrefix (_c);
    if (prefixLength<1)
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Prefix < 1 - FALSE"), Debug);
        return false;
    }
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 050 - prefixLength: " << QString::number(prefixLength);
    int i = prefixLength;
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("prefixLength: %1").arg(prefixLength), Devel);
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Call: %1").arg(_c), Devel);
    // B1A  1 - Needs a number behind
    // EA4A 2 - Needs a number behind
    // EA6  3 - Does NOT Need a number behind
    // VK9K 4 - Does NOT Need a number behind
    bool hasDigit = false;
    if (prefixLength<3)
    {
        //qDebug() << Q_FUNC_INFO << " - Prefixlength < 3";
        while (i<(_c.length()-1))
        {
            //qDebug() << Q_FUNC_INFO << " - analyzing the call1: " << _c << " - " << _c.at(i) ;
            if ((_c.at(i)).isDigit())
            {
                //qDebug() << Q_FUNC_INFO << " - has digit! i=" + QString::number(i);
                hasDigit = true;
            }
            i++;
        }
        return hasDigit;
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " - Prefixlength >= 3";
        while (!((_c.at(i)).isDigit()) && (i<(_c.length()-1)) )
        { // VP2E
            //qDebug() << Q_FUNC_INFO << " - analyzing the call2: " << _c << " - " << _c.at(i) ;
            i++;
        }
        return !hasDigit;
    }
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - TRUE"), Debug);
}

bool Utilities::isCountrySuffix (const QString &_c)
{
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << _c;
    QStringList validSuffixes = {"P", "A", "AM", "M", "MM", "LH", "R", "J", "FF", "QRP", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    return !validSuffixes.contains (_c);
}

int Utilities::isAPrefix (const QString &_c)
{
    // Returns -1 if it is not a prefix or valid call.
    // Returns an int with the lenght of the prefix:
    // The length would be including the number, if possible EA4 or;
    // including just the country prefix: EA if the number is not included.
    // Prefixes are at least 1 chars (like in K1K)
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Start: %1").arg(_c), Debug);
    int length = (getPrefixCountryFromCall(_c)).length();
    if (length>0)
    {
        return length;
    }
    else
    {
        return -1;
    }

   /*
    int length = _c.length ();

    if (length < 1)
    {
       //-TEST-qDebug() << "Utilities::isAPrefix: TOO short prefix - false - END \n";
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - Too short = -1"), Debug);
        return -1;
    }

    QString call = _c;


    //This is NOT a long prefix.

   //-TEST-qDebug() << "Utilities::isAPrefix: -10: " << call.at(0) ;
    QChar firstChar = call.at(0);
    bool firstIsValidFirst = isAOneLetterPrefix(firstChar);
    //QList<QChar> validFirstLettersOnly = {'B', 'F', 'G', 'I', 'K', 'M', 'N', 'R', 'W'};
    //bool firstIsValidFirst = validFirstLettersOnly.contains (firstChar);
    if (length == 1)
    {
       //-TEST-qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": Length = 1";
        if (firstIsValidFirst)
        {
           //-TEST-qDebug() << "Utilities::isAPrefix: VALID 1 letter" ;
            //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - Valid 1 letter = 1"), Debug);
            return 1;
        }
        else
        {
           //-TEST-qDebug() << "Utilities::isAPrefix: NOT VALID 1 letter" ;
            //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - Not valid 1 letter = -1"), Debug);
            return -1;
        }
    }
    else if ((  (call.at(1)).isDigit() ) && firstIsValidFirst)
    { // Prefixes for calls like K1
        return 1;
    }

    if (((call.startsWith("KC6")) || (call.startsWith("KG4")) || (call.startsWith("KG6"))) && (call.length() == 4))
    { // KC6xx = Palau, KG4xx = Guantanamo, KG6xx = Guam
        return 3;
    }
    // Looking for long prefixes like EA6, CE0Y, ...
    while (call.length()>=3)
    {
        if (isALongCountryPrefix(call) >0)
        {
            return call.length();
        }
        call.chop(1);
    }
    call = _c;

    // Normal prefixes are now the only option
    for (int i = 1; i<call.length();i++)
    {
        if ((call.at(i)).isDigit())
        {
            return i-1;
        }
    }

   return -1;

    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END"), Debug);
    */
}

void Utilities::setLongPrefixes (const QStringList &_p)
{
    longPrefixes = _p;
    QString aux;
}

bool Utilities::isALongCountryPrefix(const QString &_c)
{// TODO: Complete with https://rsgb.org/main/operating/licensing-novs-visitors/international-prefixes/
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    if (_c.isNull() )
    {
        //qDebug() << Q_FUNC_INFO << ": END - 1";
        return false;
    }
    if (longPrefixes.contains(_c))
    {
        //qDebug() << Q_FUNC_INFO << ": TRUE";
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << ": FALSE";
    }
    return longPrefixes.contains(_c);
    //return validPrefixes.contains (_c);
}

void Utilities::setCallValidation(const bool _b)
{
    g_callsignCheck = _b;
}

QString Utilities::getCheckedComplexCall(const QString &_c)
{ // Normalices calls
    QString call = _c;

    if (call.isNull())
        return QString();
    call.replace('\\', '/');

    if (call=="/")
        return QString();

    QStringList parts;
    parts.clear();

    parts << call.split('/');
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
    return call;
}

QString Utilities::getMainCallFromComplexCall(const QString &_complexCall)
{ // Return the part of the call that provides entity and prefix information:
  // EA4K/F => F
  // VK9/EA4K => VK9

    QString call = _complexCall;

    //qDebug() << Q_FUNC_INFO << ": " << _complexCall;
    if (call.count('/') >1)
    { //Things like F/EA4K/P will become F/EA4K    
        //logEvent (Q_FUNC_INFO, QString("With 2 /"), Debug);  
        call = call.section("/", 0,1);
        //QStringList parts;
        //parts.clear();
        //parts << call.split('/');
        //call = parts.at(0) + "/" + parts.at(1);
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
        bool firstCountry = isCountrySuffix(first);
        bool secondCountry = isCountrySuffix(second);
        //qDebug() << Q_FUNC_INFO << " - 020" ;
        if (firstCountry && secondCountry)
        { // EA4K/VK9M
            //qDebug() << Q_FUNC_INFO << " - 030" ;
            bool firstIsPrefix = isALongCountryPrefix(first);
            bool secondIsPrefix = isALongCountryPrefix(second);
            if (firstIsPrefix && !secondIsPrefix)
            {
                //qDebug() << Q_FUNC_INFO << " - 033" ;
                return first;
            }
            else if (!firstIsPrefix && secondIsPrefix)
            {
                //qDebug() << Q_FUNC_INFO << " - 034" ;
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
        else if (!isCountrySuffix(first))
        {//  1/EA4K
         // TODO: Here the perfect way should be to obtain the country and
         // add the number, if area is requested
            call = second;
        }
        else if (!isCountrySuffix(second))
        {//  EA4K/2
            call = second;
        }
        else
        { // None is a normal country prefix
            return QString();
        }
    }
    return call;
}

bool Utilities::isValidCall(const QString &_c)
{// https://life.itu.int/radioclub/rr/art19.pdf
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("Start = %1").arg(_c), Debug);
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << "Start: " << _c;
    //qDebug() << Q_FUNC_INFO << ": " << _c;
    // Prefixes are at least 2 chars
    if (!g_callsignCheck)
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 001 - true"), Debug);
        return true;
    }
    QString call = _c;
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
    call.replace('\\', '/');

    if (call.count('/')>2)
    {
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 015 - false"), Debug);
        return false;
    }
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
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - 025";

    if (call.count('/') == 1)
    { // Complex calls (like F/EA4K or EA4K/F OR /p OR /qrp
      // We are just checking the call format not if it belongs to a country or whatever.
      // It may return true for wrong calls like "ABC/EA4K"
      // TODO: Add a check just for prefixes to fix the previous
        //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString(" - Call with one /: %1").arg(call), Devel);
        QStringList parts;
        parts.clear();
        parts << call.split ('/');

        if (parts.at(0).length ()<parts.at(1).length ())
        {

            bool result1 = ((isAPrefix (parts.at (0)))>0);
            bool result2 = isValidSubCall (parts.at(1))  ;            
            //return ( ((isAPrefix (parts.at (0)))>0) && (isValidSubCall (parts.at(1)) ) );
            return (result1 && result2);
        }
        else
        { //Both lenght are just the same or the second is showter, we need to check both parts and return true if one is valid
          // It may happen that we have a EA4K/QRP or EA4K/F or EA4K/TTT
            //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - 030 - %1").arg(isValidSubCall(parts.at(0))), Debug);
            return isValidSubCall (parts.at(0));
        }
    }
    //logEvent (QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName), QString("END - %1").arg(isValidSubCall(call)), Debug);
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << " - END";

    return isValidSubCall (call);
}
QString Utilities::getPrefixFromCall2(const QString &_c, bool withAreaNumber)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c << " - WithAreaNumber=" << boolToQString(withAreaNumber);
    if (_c.isNull())
    {
        return QString();
    }
    QString call = _c;
    //qDebug() << Q_FUNC_INFO << " - 010";
    call = getMainCallFromComplexCall(call);
    //qDebug() << Q_FUNC_INFO << " - 011";
    QString call2 = call;
    QString call3 = call;
    bool keepAnalyzing  = true;
    bool found = false;
    //FB1K
    while (keepAnalyzing)
    {
        //qDebug() << Q_FUNC_INFO << ": " << call;
        if (isALongCountryPrefix(call))
        {
            //qDebug() << Q_FUNC_INFO << ": Found!: " << call;
            keepAnalyzing = false;
            found = true;
        }
        else if (call.length()<=2)
        {
           keepAnalyzing = false;
        }
        else
        {
            call3 = call2;
            call2 = call;
            call.chop(1);
        }
    }
    bool firstSpecial = isAOneLetterPrefix(call.at(0));
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

            if (((call2.at(last-1)).isDigit()))
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
                //qDebug() << Q_FUNC_INFO << ": END2.5: " << call;
                return call;
            }
        }
        else
        {
            if (call == call2)
            {
                return call;
            }

            int last = call2.length();
            if (((call2.at(last-1)).isDigit()))
            {
                //qDebug() << Q_FUNC_INFO << ": END3: " << call2;
                return call2;
            }

            //qDebug() << Q_FUNC_INFO << ": END4: " << call;
            return call;
        }
    }
    else
    {
        if (firstSpecial)
        {
            //qDebug() << Q_FUNC_INFO << ": END5: " << call;
          return call;
        }
        int last = call2.length();
        if (!((call2.at(last-1)).isDigit()))
        {
            if (isALongCountryPrefix(call))
            {
                return call;
            }
            //qDebug() << Q_FUNC_INFO << ": END6: " << call2;
            return call2;
        }
        //qDebug() << Q_FUNC_INFO << ": END7: " << call;
        return call;
    }
}

QString Utilities::getPrefixFromCall(const QString &_c, bool withAreaNumber)
{
    //qDebug() << Q_FUNC_INFO << ": " << _c;

    QString call = _c;
    call = getMainCallFromComplexCall(call);
    /*
    call.replace('\\', '/');

    if (call.count('/') == 2)
    { //Things like F/EA4K/P will become F/EA4K
        //logEvent (Q_FUNC_INFO, QString("With 2 /"), Debug);
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        call = parts.at(0) + "/" + parts.at(1);
    }
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
        bool firstCountry = isCountrySuffix(first);
        bool secondCountry = isCountrySuffix(second);
        //qDebug() << Q_FUNC_INFO << " - 020" ;
        if (firstCountry && secondCountry)
        { // EA4K/VK9M
            //qDebug() << Q_FUNC_INFO << " - 030" ;
            bool firstIsPrefix = isALongCountryPrefix(first);
            bool secondIsPrefix = isALongCountryPrefix(second);
            if (firstIsPrefix && !secondIsPrefix)
            {
                //qDebug() << Q_FUNC_INFO << " - 033" ;
                return first;
            }
            else if (!firstIsPrefix && secondIsPrefix)
            {
                //qDebug() << Q_FUNC_INFO << " - 034" ;
                return second;
            }
            else
            {
                //qDebug() << Q_FUNC_INFO << " - 035" ;
                if (first.length()>second.length())
                {
                    //qDebug() << Q_FUNC_INFO << " - 036" ;
                    call = second;
                }
                else
                {
                    //qDebug() << Q_FUNC_INFO << " - 037" ;
                    call = first;
                }
                //qDebug() << Q_FUNC_INFO << " - 040" ;
            }
            //qDebug() << Q_FUNC_INFO << " - 041" ;
        }
        else if (!isCountrySuffix(first))
        {//  1/E4AK
            //qDebug() << Q_FUNC_INFO << " - 050" ;
            call = second;
        }
        else if (!isCountrySuffix(second))
        {//  E4AK/2
            //qDebug() << Q_FUNC_INFO << " - 060" ;
            call = second;
        }
        else
        { // None is a normal country prefix
            //qDebug() << Q_FUNC_INFO << " - 070" ;
            return QString();
        }
        //qDebug() << Q_FUNC_INFO << " - 090" ;
    }
    //qDebug() << Q_FUNC_INFO << " - END: " << call ;
    */
    if (withAreaNumber)
    {
        return getPrefixFullFromCall(call);
    }
    return getPrefixCountryFromCall(call);
}

/*QString Utilities::getPrefixFromCall(const QString &_c, bool withAreaNumber)
{
    //logEvent (Q_FUNC_INFO, QString("Start: %1").arg(_c), Debug);
    QString call = _c;
    call.replace('\\', '/');

    if (call.count('/') == 2)
    { //Things like F/EA4K/P will become F/EA4K
        //logEvent (Q_FUNC_INFO, QString("With 2 /"), Debug);
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        call = parts.at(0) + "/" + parts.at(1);
    }

    //QString prefix = QString();
    int pref = -1;
    if (call.count('/') == 1)
    { // Complex calls (like F/EA4K or EA4K/F OR /p OR /qrp
        //logEvent (Q_FUNC_INFO, QString("With 1 /"), Debug);
        QStringList parts;
        parts.clear();
        parts << call.split ('/');
        bool secondPartIsCountryPrefix = isCountrySuffix (parts.at(1));
        //bool secondPartIsCountryPrefix = isCountrySuffix (parts.at(1));
        if ((parts.at(0).length ()<parts.at(1).length ()) || (!secondPartIsCountryPrefix))
        { // First one is shorter
            //logEvent (Q_FUNC_INFO, "First one is shorter:", Debug);
            int pref = isAPrefix (parts.at(0));
            if (pref>0)
            {
                //logEvent (Q_FUNC_INFO, QString("R1 = %1").arg(parts.at(0)), Debug);
                return parts.at(0);
            }
            else
            {
                //logEvent (Q_FUNC_INFO, "EMPTY-2", Debug);
                return QString();
            }
        }
        else if(parts.at(0).length ()>parts.at(1).length ())
        { // Second one is shorter
            //logEvent (Q_FUNC_INFO, "Second one is shorter", Debug);
            if (isCountrySuffix (parts.at(1)))
            {
                pref = isAPrefix (parts.at(1));
            }
            else
            {
                pref = isAPrefix (parts.at(0));
            }

            if (pref>0)
            {
                //logEvent (Q_FUNC_INFO, QString("R2 = %1").arg(parts.at(1)), Debug);
                return parts.at(1);
            }
            else
            {
                pref = isAPrefix (parts.at(0));
                if (pref>0)
                {
                    //logEvent (Q_FUNC_INFO, QString("R3 = %1").arg(parts.at(0).left(pref)), Debug);
                    return parts.at(0).left (pref);
                }
                else
                {
                    //logEvent (Q_FUNC_INFO, "EMPTY-3", Debug);
                    return QString();
                }
            }
        }
        else
        { //Both lenght are just the same, we need to check both parts and return true if one is valid
            //qDebug() << Q_FUNC_INFO << ": Same length, we should consider the first one if valid, if not the second one " ;
            if (isALongCountryPrefix(parts.at(0)))
            {
                return parts.at(0);
            }
            else if (isALongCountryPrefix(parts.at(1)))
            {
                return parts.at(1);
            }
        }
    }
    //logEvent (Q_FUNC_INFO, QString("Call before R4= %1").arg(call), Debug);
    pref = isAPrefix (call);
    if (pref>0)
    {
        //logEvent (Q_FUNC_INFO, QString("R4 = pref=%1, call= %2").arg(pref).arg(call.left (pref)), Debug);
        //return call.left(isAPrefix(call.left (pref)));
        return call.left (pref);
    }
    //logEvent (Q_FUNC_INFO, "END", Debug);
    return QString();
}
*/

QString Utilities::getPrefixFullFromCall(const QString &_c)
{ // Returns the prefix, including the country call area (EA4, KB1, EA6...
    //qDebug() << Q_FUNC_INFO << QString(" - Start: %1").arg(_c);
    if (_c.isNull())
    {
        //qDebug() << Q_FUNC_INFO << QString(" - END-1 NULL");
        return QString();
    }
    bool startsWithOneSpecialetter = isAOneLetterPrefix(_c.at(0));

    QString longPrefix = startsWithLongPrefix(_c);
    if (longPrefix.length()>2)
    {
        return longPrefix;
    }
    if (_c.length()==1)
    {
        if (startsWithOneSpecialetter)
        {
            //qDebug() << Q_FUNC_INFO << QString(" - END-2: %1").arg(_c);
            return _c;
        }
        return QString();
    }
    else if (_c.length()==2)
    {
        //qDebug() << Q_FUNC_INFO << QString(" - END-3: %1").arg(_c);
        return _c;
    }

    int i = 2;
    if (startsWithOneSpecialetter && ((_c.at(1)).isDigit()) )
    {
        i=1;
    }

    QString aux = _c;
    int z = aux.length()-1;

    while (z>0)
    {
        QString pref = startsWithLongPrefix(aux);
        if (pref.length()>2)
        {
            return pref;
        }
        if ((aux.at(z)).isDigit())
        {
            return _c.left(z+1);
        }
        z--;
    }
    return QString();

    // Shorting the call to the prefix
    /*for (int k = i; k<_c.length();k++)
    {
        if (!((_c.at(k)).isDigit()))
        {
            //qDebug() << Q_FUNC_INFO << QString(" - END-4: %1").arg(_c.left(k-1));
            return _c.left(k);
        }
    }
    */
    //qDebug() << Q_FUNC_INFO << QString(" - END-5");

    return _c;
}

QString Utilities::getPrefixCountryFromCall(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << QString(" - Start: %1").arg(_c);
    QString fullPrefix = getPrefixFullFromCall(_c);
    //qDebug() << Q_FUNC_INFO << QString(" - fullPrefix: %1").arg(fullPrefix);
    if (fullPrefix.length()<1)
    {
        return QString();
    }
    else if (fullPrefix.length()==1)
    {
        if (isAOneLetterPrefix(fullPrefix.at(0)))
        {
            return fullPrefix;

        }
        else
        {
            return QString();
        }
    }
    //qDebug() << Q_FUNC_INFO << ": - 010";
    if (isALongCountryPrefix(fullPrefix))
    { // It is a long prefix, we return it as it is
         //qDebug() << Q_FUNC_INFO << QString("END-1 - %1").arg(fullPrefix);
        return fullPrefix;
    }
    else
    { // It is not a long one, we need to remove the number.
      // EA4 => EA, K1 => K, KB1 = KB, E73 => E3
        //qDebug() << Q_FUNC_INFO << ": - 020";
        if (isAOneLetterPrefix(fullPrefix.at(0)) && ((fullPrefix.at(1)).isDigit()))
        { // K1 => K
            //qDebug() << Q_FUNC_INFO << QString("END-2 - %1").arg(QString(fullPrefix.at(0)));
            return QString(fullPrefix.at(0));
        }
        else if (fullPrefix.length()>=2)
        {
            //qDebug() << Q_FUNC_INFO << QString("END-3 - %1").arg(fullPrefix.left(2));
            return fullPrefix.left(2);
        }
    }
    //qDebug() << Q_FUNC_INFO << QString("END");
    return QString();
}


QString Utilities::startsWithLongPrefix(const QString &_c)
{
    //qDebug() << Q_FUNC_INFO << QString(" - Start: %1").arg(_c);
    int i = 0;
    QString aux = QString();

    while (i < _c.length())
    {
        //qDebug() << Q_FUNC_INFO << QString(" Checking (string/i/length) - %1/%2/3").arg(aux).arg(i).arg(_c.length());
        aux.append(_c.at(i));        

        //qDebug() << Q_FUNC_INFO << QString(" - %1: %2").arg(aux).arg(isALongCountryPrefix(aux));
        if (isALongCountryPrefix(aux))
        {
            //qDebug() << Q_FUNC_INFO << QString(" Is Long pref, checking if even longer - %1:").arg(aux);
            if (i+1<_c.length())
            {
                //qDebug() << Q_FUNC_INFO << QString(" Checking... - %1:").arg(aux);
                QString auxLong;
                auxLong = aux;
                auxLong.append(_c.at(i+1));
                if (isALongCountryPrefix(auxLong))
                {
                    return auxLong;
                }
            }
            return aux;
        }
        i++;
    }
    //qDebug() << Q_FUNC_INFO << QString("END");
    return QString();
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
    if (_m>0)
    {
        return true;
    }
    else
    {
        return false;
    }
    //return false;
}

bool Utilities::isValidFreq(const QString &_b)
{
    if (_b.toDouble()>0)
    {
        return true;
    }
    else
    {
        return false;
    }
    //return false;
}

bool Utilities::isValidGrid(const QString &_b)
{
    Locator locator;
    return locator.isValidLocator(_b);
}

bool Utilities::isValidVUCCGrids(const QString &_b)
{
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << _b;
    QStringList tmp;

    //QString a = _b;
    tmp.clear ();
    tmp << _b.split (',', QT_SKIP);

    if ((tmp.length ()!=2) && (tmp.length ()!=4))
    {
        //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": NON VALID LENGTH";
        return false;
    }

    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": tmp: " << tmp;
    QString aux;
    foreach (aux, tmp) {
        aux = aux.trimmed ();

        if ((!isValidGrid (aux)) || (aux.length ()!=4))
        {
            //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": NON VALID";
            return false;
        }
        else
        {
            //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": VALID: " << aux;
        }
    }
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": VALID-END";
    return true;
}

bool Utilities::isValidRST(const QString &_b)
{
    if (_b.length()>0)
    {
        return true;
    }
    return false;
}

bool Utilities::isValidPower(const QString &_b)
{
    if (_b.toDouble()>0)
    {
        return true;
    }
    return false;
}

bool Utilities::isValidComment(const QString &_b)
{
    if (_b.length()>0)
    {
        return true;
    }
    return false;
}

bool Utilities::isValidName(const QString &_b)
{
    if (_b.length()>0)
    {
        return true;
    }
    return false;
}

bool Utilities::isDBFileExisting()
{
    //qDebug() << "Utilities::isDBFileExisting: " << getKLogDBFile() ;
    if (QFile::exists(getKLogDBFile()))
    {
             //qDebug() << "Utilities::isDBFileExisting - true" ;
        return true;
    }
    else
    {
             //qDebug() << "Utilities::isDBFileExisting - false" ;
        return false;
    }
    //return false;
}

bool Utilities::isDBFileExisting(const QString &_file)
{
         //qDebug() << "Utilities::isDBFileExisting2: " << _file ;

    if (QFile::exists(_file))
    {
             //qDebug() << "Utilities::isDBFileExisting2 - true" ;
        return true;
    }
    else
    {
             //qDebug() << "Utilities::isDBFileExisting2 - false" ;
        return false;
    }
    //return false;
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
    if ((c == "Y") || (c == "N") || (c == "R") || (c == "I") || (c == "V"))
    {
        return true;
    }
    else {
        return false;
    }
}

bool Utilities::isValidQSL_Sent(const QString &c)
{
    if ((c == "Y") || (c == "N") || (c == "R") || (c == "Q") || (c == "I"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Utilities::isValidUpload_Status(const QString &c)
{
    if ((c == "Y") || (c == "N") || (c == "M"))
    {
        return true;
    }
    else {
        return false;
    }
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
    if (((_d > 0) && (_d < 523))  || (_d == 1206) || (_d == 1279) || (_d == 1248) || (_d == 2248) || (_d == 1259) || (_d == 1390))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Utilities::isValidAntPath(const QString &_s)
{
   if ((_s == "G") || (_s == "O") || (_s == "S") || (_s == "L"))
   {
       return true;
   }
   else
   {
        return false;
   }
}

bool Utilities::isValidARRLSect(const QString &_s)
{

    if (ARRL_sects.contains (_s.toUpper ()))
        return true;
    return false;
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

/*
QPalette Utilities::getPalete(bool _ok)
{
   //qDebug() << "Utilities::getPalete"  ;
    if (_ok)
    {
       //qDebug() << "Utilities::getPalete - true"  ;
        return palRed;
        //return QPalette::setColor(QPalette::Text, Qt::red);
    }
    else
    {
       //qDebug() << "Utilities::getPalete - false"  ;
        return palBlack;
    }
}
*/

void Utilities::setDarkMode(const QString &_dm)
{
    darkMode = trueOrFalse(_dm);
}

bool Utilities::isDarkMode()
{
    if (darkMode)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QHash<QString, QString> Utilities::LogColumnNames;
QString Utilities::getLogColumnName(const QString &_column)
{
    //QObject::tr
    //qDebug() << QString("%1-%2").arg(Q_FUNC_INFO).arg(parentName) << ": " << _column;
    if (LogColumnNames.empty()) {
        initializeLogColumnNames();
    }
    return LogColumnNames.contains(_column) ? LogColumnNames[_column] : _column;
}

void Utilities::initializeLogColumnNames()
{
    LogColumnNames = {
        {("qso_date"),QObject::tr("Date")},
        {("call"),QObject::tr("Call")},
        {("rst_sent"),QObject::tr("RSTtx")},
        {("rst_rcvd"),QObject::tr("RSTrx")},
        {("bandid"),QObject::tr("Band")},
        {("comment"),QObject::tr("Comment")},
        {("modeid"),QObject::tr("Mode")},
        {("cqz"),QObject::tr("CQz")},
        {("ituz"),QObject::tr("ITUz")},
        {("dxcc"),QObject::tr("DXCC")},
        {("address"),QObject::tr("Address")},
        {("age"),QObject::tr("Age")},
        {("cnty"),QObject::tr("County")},
        {("a_index"),QObject::tr("A_Index")},
        {("ant_az"),QObject::tr("Ant_Az")},
        {("ant_el"),QObject::tr("Ant_El")},
        {("ant_path"),QObject::tr("Ant_Path")},
        {("arrl_sect"),QObject::tr("ARRL_SECT")},
        {("award_submitted"),QObject::tr("Award_Submitted")},
        {("award_granted"),QObject::tr("Award_granted")},
        {("band_rx"),QObject::tr("Band_RX")},
        {("checkcontest"),QObject::tr("CheckContest")},
        {("class"),QObject::tr("Class")},
        {("clublog_qso_upload_date"),QObject::tr("ClubLog SDate")},
        {("clublog_qso_upload_staus"),QObject::tr("ClubLog status")},
        {("cont"),QObject::tr("Continent")},
        {("contacted_op"),QObject::tr("Contacted Op")},
        {("contest_id"),QObject::tr("Contest Id")},
        {("country"),QObject::tr("Country")},
        {("credit_submitted"),QObject::tr("Credit Submitted")},
        {("credit_granted"),QObject::tr("Credit granted")},
        {("dark_dok"),QObject::tr("Dark Dok", "Do not translate unsure, common hamradio term.")},
        {("distance"),QObject::tr("Distance")},
        {("email"),QObject::tr("Email")},
        {("eq_call"),QObject::tr("EQ_Call")},
        {("eqsl_qslrdate"),QObject::tr("eQSL RDate")},
        {("eqsl_qslsdate"),QObject::tr("eQSL SDate")},
        {("eqsl_qsl_rcvd"),QObject::tr("eQSL Rcvd")},
        {("eqsl_qsl_sent"),QObject::tr("eQSL Sent")},
        {("fists"),QObject::tr("Fists", "Do not translate unsure, common hamradio term.")},
        {("fists_cc"),QObject::tr("Fists CC", "Do not translate unsure, common hamradio term.")},
        {("force_init"),QObject::tr("Force Init")},
        {("freq"),QObject::tr("Freq")},
        {("freq_rx"),QObject::tr("Freq RX")},
        {("gridsquare"),QObject::tr("Gridsquare")},
        {("guest_op"),QObject::tr("Guest OP")},
        {("hrdlog_qso_upload_date"),QObject::tr("HRDLog SDate")},
        {("hrdlog_qso_upload_status"),QObject::tr("HRDLog status")},
        {("iota"),QObject::tr("IOTA")},
        {("iota_island_id"),QObject::tr("IOTA Island id")},
        {("k_index"),QObject::tr("K Index")},
        {("lat"),QObject::tr("Lat")},
        {("lon"),QObject::tr("Lon")},
        {("lotw_qslrdate"),QObject::tr("LoTW RDate")},
        {("lotw_qslsdate"),QObject::tr("LoTW SDate")},
        {("lotw_qsl_rcvd"),QObject::tr("LoTW Rcvd")},
        {("lotw_qsl_sent"),QObject::tr("LoTW Sent")},
        {("max_bursts"),QObject::tr("Max Bursts")},
        {("multiplier"),QObject::tr("Multiplier")},
        {("ms_shower"),QObject::tr("MS Shower")},
        {("my_antenna"),QObject::tr("My Antenna")},
        {("my_city"),QObject::tr("My City")},
        {("my_cnty"),QObject::tr("My Cnty")},
        {("my_country"),QObject::tr("My Country")},
        {("my_cq_zone"),QObject::tr("My CQz")},
        {("my_dxcc"),QObject::tr("My DXCC")},
        {("my_fists"),QObject::tr("My Fists", "Do not translate unsure, common hamradio term.")},
        {("my_gridsquare"),QObject::tr("My Gridsquare")},
        {("my_iota"),QObject::tr("My IOTA")},
        {("my_iota_island_id"),QObject::tr("My IOTA island id")},
        {("my_itu_zone"),QObject::tr("My ITUz")},
        {("my_lat"),QObject::tr("My Lat")},
        {("my_lon"),QObject::tr("My Lon")},
        {("my_name"),QObject::tr("My Name")},
        {("my_postal_code"),QObject::tr("My Postal code")},
        {("my_rig"),QObject::tr("My Rig")},
        {("my_sig"),QObject::tr("My Sig")},
        {("my_sig_info"),QObject::tr("My Sig Info")},
        {("my_sota_ref"),QObject::tr("My SOTA ref")},
        {("my_state"),QObject::tr("My State")},
        {("my_street"),QObject::tr("My Street")},
        {("my_usaca_counties"),QObject::tr("My USACA counties")},
        {("my_vucc_grids"),QObject::tr("My VUCC grids")},
        {("name"),QObject::tr("Name")},
        {("notes"),QObject::tr("Notes")},
        {("nr_bursts"),QObject::tr("Nr bursts", "Do not translate unsure, common hamradio term.")},
        {("nr_pings"),QObject::tr("Nr pings", "Do not translate unsure, common hamradio term.")},
        {("operator"),QObject::tr("Operator")},
        {("owner_callsign"),QObject::tr("Owner Callsign")},
        {("pfx"),QObject::tr("Pfx")},
        {("points"),QObject::tr("Points")},
        {("precedence"),QObject::tr("Precedence")},
        {("prop_mode"),QObject::tr("Prop Mode")},
        {("public_key"),QObject::tr("Public Key")},
        {("qrzcom_qso_upload_date"),QObject::tr("QRZcom SDate")},
        {("qrzcom_qso_upload_status"),QObject::tr("QRZcom status")},
        {("qslmsg"),QObject::tr("QSL msg")},
        {("qslrdate"),QObject::tr("QSL RDate")},
        {("qslsdate"),QObject::tr("QSL SDate")},
        {("qsl_rcvd"),QObject::tr("QSL Rcvd")},
        {("qsl_sent"),QObject::tr("QSL Sent")},
        {("qsl_rcvd_via"),QObject::tr("QSL rcvd via")},
        {("qsl_sent_via"),QObject::tr("QSL sent via")},
        {("qsl_via"),QObject::tr("QSL via")},
        {("qso_complete"),QObject::tr("QSO complete")},
        {("qso_random"),QObject::tr("QSO random")},
        {("qth"),QObject::tr("QTH")},
        {("region"),QObject::tr("Region")},
        {("rig"),QObject::tr("Rig")},
        {("rx_pwr"),QObject::tr("RX Pwr")},
        {("sat_mode"),QObject::tr("Sat mode")},
        {("sat_name"),QObject::tr("Sat name")},
        {("sfi"),QObject::tr("SFI")},
        {("sig"),QObject::tr("Sig")},
        {("sig_info"),QObject::tr("Sig Info")},
        {("silent_key"),QObject::tr("Silent key", "Do not translate unsure, common hamradio term.")},
        {("skcc"),QObject::tr("SKCC")},
        {("sota_ref"),QObject::tr("SOTA Ref")},
        {("srx_string"),QObject::tr("SRX String")},
        {("srx"),QObject::tr("SRX")},
        {("stx_string"),QObject::tr("STX String")},
        {("state"),QObject::tr("State")},
        {("station_callsign"),QObject::tr("Station Callsign")},
        {("submode"),QObject::tr("Submode")},
        {("swl"),QObject::tr("SWL", "Do not translate unsure, common hamradio term.")},
        {("uksmg"),QObject::tr("UKSMG")},
        {("usaca_counties"),QObject::tr("USACA counties")},
        {("ve_prov"),QObject::tr("VE prov")},
        {("vucc_grids"),QObject::tr("VUCC grids")},
        {("ten_ten"),QObject::tr("Ten-Ten", "Do not translate, it is a hamradio group name.")},
        {("tx_pwr"),QObject::tr("TX Pwr")},
        {("web"),QObject::tr("Web")},
        {("qso_date_off"),QObject::tr("QSO Date off")},
        {("transmiterid"),QObject::tr("Transmitter id")},
        {("lognumber"),QObject::tr("Log number")}
    };


}

bool Utilities::isValidContinent(const QString &_s)
{
    if (continent.contains (_s.toUpper ()))
            return true;
    else
            return false;
}

bool Utilities::isValidDistance(const double _d)
{
    return (_d>=0);
}

bool Utilities::isValidSponsor(const QString &_s)
{
    if (sponsorsList.contains (_s.toUpper ()))
            return true;
    else
            return false;
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
