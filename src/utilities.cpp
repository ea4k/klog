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

Utilities::Utilities()
{
       //qDebug() << "Utilities::Utilities"  << endl;

    softwareVersion = "0.0";

    //palRed.setColor(QPalette::Text, Qt::red);
    //palBlack.setColor(QPalette::Text, Qt::black);
       //qDebug() << "Utilities::Utilities - END"  << endl;
}

Utilities::~Utilities()
{

}

void Utilities::setVersion(const QString &_v)
{
       //qDebug() << "Utilities::setVersion: " << _v << endl;
    softwareVersion = _v;
}

QString Utilities::getVersion()
{
    return softwareVersion;
}

double Utilities::getVersionDouble()
{
       //qDebug() << "Utilities::getVersionDouble: " << softwareVersion << endl;

    if (softwareVersion.count('.')>1)
    {
        QString first = softwareVersion.section('.', 0, 0);
        int pos = softwareVersion.indexOf('.');

        QString decimals = softwareVersion.section('.', pos, -1);
        decimals.remove('.');
        first = first + "." + decimals;
           //qDebug() << "Utilities::getVersionDouble - returning: "  << first << endl;
        return first.toDouble();

    }
       //qDebug() << "Utilities::getVersionDouble: no points detected" << endl;
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
         //qDebug() << "Utilities::trueOrFalse: " << _s << endl;

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

QString Utilities::checkAndFixASCIIinADIF(const QString &_data)
{
         //qDebug() << "SetupDialog::checkAndFixASCIIinADIF " << _data << endl;
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
             //qDebug() << "SetupDialog::checkAndFixunicodeinADIF: " << st.at(i) <<" = " << QString::number(unicodeVal) << endl;
    }

    // Show into another lineEdit

    return newString;
}

void Utilities::printQString(const QStringList &_qs)
{
        //qDebug() << "Utilities::printQString: COMMENT THIS CALL BEFORE RELEASING" << endl;
    if (_qs.length()<1)
    {
            //qDebug() << "Utilities::printQString: EMPTY QStringList received!!" << endl;
        return;
    }
    for (int i=0; i<_qs.length()-1;i++)
    {
            //qDebug() << _qs.at(i) << "/" ;
    }
        //qDebug() << _qs.at(_qs.length()-1) << endl;
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
         //qDebug() << "WINDOWS DETECTED!: "  << QDir::homePath() + "/klog" << endl;
    return QDir::homePath()+"/klog";  // We create the \klog for the logs and data

#else
         //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    return QDir::homePath()+"/.klog";  // We create the ~/.klog for the logs and data
#endif
}

QString Utilities::getDefaultRST(const QString &_m)
{
   //qDebug() << "Utilities::getDefaultRST: " << _m << endl;

   if ((_m == "SSB") || (_m== "LSB") || (_m=="USB") )
   {
        //qDebug() << "MainWindow::setRSTToMode: Detected SSB/LSB/USB"  << endl;
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

QString Utilities::getKLogDefaultDatabaseFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
    return getHomeDir() ;
}

QString Utilities::getKLogDBFile()
{
        //qDebug() << "Utilities::getKLogDBFile: start " << endl;

    dbPath = getKLogDefaultDatabaseFile();
    QFile file(getCfgFile());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){

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
       //qDebug() << "Utilities::getKLogDBFile: path to use: " << dbPath << endl;
    return dbPath + "/logbook.dat";
}

QString Utilities::getKLogDBBackupFile()
{
        //qDebug() << "Utilities::getKLogDBFile: start " << endl;

    dbPath = getKLogDefaultDatabaseFile();
    QFile file(getCfgFile());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){

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
       //qDebug() << "Utilities::getKLogDBFile: path to use: " << dbPath << endl;
    return dbPath + "/" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + "-backup-logbook.dat" ;
}

bool Utilities::processConfigLine(const QString &_line)
{
             //qDebug() << "Utilities::processConfigLine: " << _line << endl;

        QString line = _line.simplified();
        //line.simplified();
        //QString aux;

        QStringList values = line.split("=", QString::SkipEmptyParts);


        if (line.startsWith('#')){
                 //qDebug() << "Utilities::processConfigLine: notes Line!" << endl;
            return true;
        }
        if (!( (line.contains('=')) && (line.contains(';')))){
                 //qDebug() << "Utilities::processConfigLine: Wrong Line!" << endl;
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
                  //qDebug() << "Utilities::processConfigLine: dbPATH found: " << value << endl;
            dbPath = value;
        }
        return true;
}

/*
QString Utilities::getKLogDatabaseFile(const QString &_file)
{
       //qDebug() << "Utilities::getKLogDatabaseFile:" << _file << endl;
    if ( QFile::exists(_file + "/logbook.dat") )
    {
           //qDebug() << "Utilities::getKLogDatabaseFile:returning: " <<  _file + "/logbook.dat" << endl;
        return _file + "/logbook.dat";
    }
    else
    {}
         //qDebug() << "Utilities::getKLogDatabaseFile: Does not exist so default: " <<  getKLogDefaultDatabaseFile() << endl;
        return getKLogDefaultDatabaseFile();
}
*/

QString Utilities::getCfgFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
#if defined(Q_OS_WIN)
         //qDebug() << "WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  << endl;
    return getHomeDir() + "/klogrc.cfg";

#else
         //qDebug() << "NO WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  << endl;
    return getHomeDir() + "/klogrc";

#endif

}

QString Utilities::getDebugLogFile()
{
#if defined(Q_OS_WIN)
         //qDebug() << "WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  << endl;
    return getHomeDir() + "/klogdebug.log";

#else
         //qDebug() << "NO WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  << endl;
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
      //qDebug() << "Utilities::getTQSLsFileName: "   << endl;

#if defined(Q_OS_WIN)
         //qDebug() << "WINDOWS DETECTED!: "   << endl;
    return "tqsl.exe";
#elif   defined(Q_OS_MACOS)
      //qDebug() << "macOS DETECTED!: "   << endl;
    return "tqsl";
#else
         //qDebug() << "NO WINDOWS/macOS DETECTED!: "   << endl;
    return "tqsl";
#endif

}

QString Utilities::getTQSLsPath()
{
      //qDebug() << "Utilities::getDefaultProgramsPath " << endl;

#if defined(Q_OS_WIN64)
         //qDebug() << "WINDOWS DETECTED!: "   << endl;
    return "C:/Program Files/TrustedQSL/";
#elif defined(Q_OS_WIN32)
    return "C:/Program Files (x86)/TrustedQSL/";
#elif defined(Q_OS_MACOS)
      //qDebug() << "macOS DETECTED!: "   << endl;
    return "/Applications/tqsl.app/Contents/MacOS/";
#else
         //qDebug() << "NO WINDOWS/macOS DETECTED!: "   << endl;
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
        return ((QString::number(_dxcc)).right(3)).toInt();
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
      //qDebug() << "Utilities::isValidDate: " << _d.toString("yyyyMMdd") << endl;
    if (_d.isValid())
    {
        if ( _d > QDate::fromString("18000101", "yyyyMMdd") )
        {
              //qDebug() << "Utilities::isValidDate: OK" << endl;
            return true;
        }
    }
      //qDebug() << "Utilities::isValidDate: Error" << endl;
    return false;
}

bool Utilities::isValidDateTime(const QString &_d)
{
       //qDebug() << "Utilities::isValidDateTime: " << _d << endl;
    QDateTime _dateTime = QDateTime::fromString(_d, "yyyyMMddhhmmss");
    if ( _dateTime.isValid()  )
    {
           //qDebug() << "Utilities::isValidDateTime: 1"  << endl;
        return isValidDate(_dateTime.date());
    }
       //qDebug() << "Utilities::isValidDateTime: Error" << endl;
    return false;
}

bool Utilities::isValidSubCall(const QString &_c)
{

    //qDebug() << "Utilities::isValidSubCall: " << _c << endl;
    // This functions only checks simple calls like EA4K, not composed like EA4K/F of F/EA4K/QRP
    //Rules: http://life.itu.int/radioclub/rr/art19.pdf
    if (_c.contains ('/'))
    {
        return false;
    }
    if (_c.length()<3)
    {
        //qDebug() << "Utilities::isValidSubCall: FALSE-1: " << _c << endl;
        return false;
    }
    if (!(_c.at(_c.length ()-1).isLetter ()))
    {
        //qDebug() << "Utilities::isValidSubCall: FALSE-1.1: " << _c << endl;
        return false;
    }
    for (int i = 0; i<_c.length ();i++)
    {
        if (!_c.at(i).isLetterOrNumber ())
        {
            // Non valid chars
            return false;
        }
    }

    int prefixLength = isAPrefix (_c);
    if (prefixLength<1)
    {
        //qDebug() << "Utilities::isValidSubCall: FALSE-1.2: " << _c << endl;
        return false;
    }

    int i = prefixLength;
    //qDebug() << "Utilities::isValidSubCall: prefixLength" << QString::number(prefixLength) << endl;
    //qDebug() << "Utilities::isValidSubCall: call" << _c << endl;
    while(i<_c.length ()-1)
    {
        if (!((_c.at(i).isLetter()) ))
        {
            //qDebug() << "Utilities::isValidSubCall: FALSE-1.3: " << _c << endl;
            return false;
        }
        i++;
    }
    //qDebug() << "Utilities::isValidSubCall: true" << endl;
    return true;
}

int Utilities::isAPrefix (const QString &_c)
{
    // Returns -1 if it is not a prefix or valid call.
    // Returns an int with the lenght of the prefix:
    // The length would be including the number, if possible EA4 or;
    // including just the country prefix: EA if the number is not included.

    //qDebug() << "Utilities::isAPrefix: " << _c << endl;
    // Prefixes are at least 1 chars (like in K1K)
    int length = _c.length ();

    if (length < 1)
    {
        //qDebug() << "Utilities::isAPrefix: TOO short prefix - false - END \n";
        return -1;
    }

    QString call = _c;
    //qDebug() << "Utilities::isAPrefix: -10: " << call.at(0) << endl;
    QChar firstChar = call.at(0);
    QList<QChar> validFirstLettersOnly = {'B', 'F', 'G', 'I', 'K', 'M', 'N', 'R', 'W'};

    if (length == 1)
    {
        if (validFirstLettersOnly.contains (firstChar))
        {
            //qDebug() << "Utilities::isAPrefix: VALID 1 letter" << endl;
            return 1;
        }
        else
        {
            //qDebug() << "Utilities::isAPrefix: NOT VALID 1 letter" << endl;
            return -1;
        }
    }

    QChar secondChar = call.at(1);
    //qDebug() << "Utilities::isAPrefix: SecondChar: " << secondChar << endl;
    int pref = -1;
    if (call.count(QRegularExpression("\\d")) >0) // Does it has any digit?
    {
        //qDebug() << "Utilities::isAPrefix: It has digits: " << call << endl;
        bool done = false;
        int i = -1;
        while ((i < length-1) && !done)
        {
            //qDebug() << "Utilities::isAPrefix: in the while: " << QString::number(i) << endl;
            i++;
            if (call.at(i).isLetter ())
            {
                //qDebug() << "Utilities::isAPrefix: in the while: is a Letter: " << call.at(i) << endl;
                if (pref>0)
                {
                    pref = i;
                    done = true;
                }
            }
            else
            {
                //qDebug() << "Utilities::isAPrefix: in the while: is NOT a Letter: " << call.at(i) << endl;
                if (i > 0)
                {
                    pref = i;
                }
            }
        } // end of while
    }

    //qDebug() << "Utilities::isAPrefix: After the while: " << QString::number(pref) << endl;

    QString prefix;
    if (pref>0)
    {
         //qDebug() << "Utilities::isAPrefix: pref>0 =>: " << call.left (pref) << endl;
        prefix = call.left (pref);
    }
    else
    {
        prefix = call;
        //qDebug() << "Utilities::isAPrefix: pref<=0 =>: " << call << endl;
    }

    length = prefix.length();

    QChar thirdChar = QChar();
    if (length>2)
    {
        thirdChar = prefix.at(2);
    }

    //qDebug() << "Utilities::isAPrefix: -50 "  << endl;
    // The first two characters shall be two letters or a letter followed
    // by a digit or a digit followed by a letter. The first two characters or in certain cases
    // the first character of a call sign constitute the nationality identification

    if (firstChar.isDigit() && secondChar.isDigit())
    {
        //qDebug() << "Utilities::isAPrefix: FALSE-6: " << prefix << endl;
        return -1;
    }

    //qDebug() << "Utilities::isAPrefix: -60 "  << endl;
    if (firstChar.isLetter() && secondChar.isLetter() && thirdChar.isLetter())
    {
        //qDebug() << "Utilities::isAPrefix: FALSE-6: " << prefix << endl;
        return -1;
    }

    //qDebug() << "Utilities::isAPrefix: -70 "  << endl;

    QList<QChar> validFirstLetters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'P', 'R', 'S', 'T' ,'U', 'V', 'W', 'Z'};

    if (secondChar.isDigit ())
    {
        if (!validFirstLetters.contains (firstChar))
        {
            //qDebug() << "Utilities::isAPrefix: NOT VALID 1 letter not valid" << endl;
            return -1;
        }
    }
    /*
     // The following if is to manage not format but specific IARU callsign assignments
     // defined here: http://life.itu.int/radioclub/rr/ap42.pdf
     // For the time being, KLog will not check that
    if ( (firstChar.isLetter ()) && (secondChar.isDigit ()) && (validFirstLetters.contains(firstChar)))
    {
        //qDebug() << "Utilities::isAPrefix validFirstLetters contains the first one!" << endl;
        if (firstChar == 'C')
        {
            if ((secondChar == '1') || (secondChar == '7'))
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.1: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'D')
        {
            //((secondChar == '1') || (secondChar == '8'))
            if (false)
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.2: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'E')
        {
            if (!(secondChar == '2') && !(secondChar == '3') && !(secondChar == '4') && !(secondChar == '7')
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.3: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'H')
        {
            if ((secondChar == '1') )
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.4: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'J')
        {
            if ((secondChar == '1') || (secondChar == '9'))
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.5: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'P')
        {
            if (secondChar == '1')
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.6: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'S')
        {
            if ((secondChar == '1') || (secondChar == '6'))
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.7: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'T')
        {
            if (secondChar == '0')
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.8: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'V')
        {
            if ((secondChar == '1') || (secondChar == '9'))
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.9: " << prefix << endl;
                return false;
            }
        }
        if (firstChar == 'Z')
        {
            if (!((secondChar == '2') || (secondChar == '3')))
            {
                //qDebug() << "Utilities::isAPrefix: FALSE-7.10: " << prefix << endl;
                return false;
            }
        }
        //qDebug() << "Utilities::isAPrefix: 1-Letter + number prefix valid: " << prefix << endl;
    }
    */
    //qDebug() << "Utilities::isAPrefix: After the if's"  << endl;
    // It seems to be a valid prefix, let's count how many chars belong to the prefix, first letter after the digit
    // should be the suffix starting point

    //qDebug() << "Utilities::isAPrefix: Prefix length: " << QString::number(pref) << "/" << prefix << endl;
    if (pref>0)
    {
        return pref;
    }
    else
    {
        return prefix.length ();
    }
}

bool Utilities::isValidCall(const QString &_c)
{
    //qDebug() << "Utilities::isValidCall: " << _c << endl;
    // Prefixes are at least 2 chars
    QString call = _c;
    if (_c.length()<3)
    {
        //qDebug() << "Utilities::isValidCall: FALSE-1: " << _c << endl;
        return false;
    }

    call.replace('\\', '/');

    if (call.count('/')>2)
    {
        //qDebug() << "Utilities::isValidCall: FALSE-3: " << call << endl;
        return false;
    }
    if (call.count('/') == 2)
    { //Things like F/EA4K/P will become F/EA4K
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        call = parts.at(0)+parts.at(1);
    }
    //QString prefix = QString();

    if (call.count('/') == 1)
    { // Complex calls (like F/EA4K or EA4K/F OR /p OR /qrp
      // We are just checking the call format not if it belongs to a country or whatever.
      // It may return true for wrong calls like "ABC/EA4K"
      // TODO: Add a check just for prefixes to fix the previous

        QStringList parts;
        parts.clear();
        parts << call.split ('/');

        //return ( ((isAPrefix (parts.at (0)))>0) || ((isAPrefix (parts.at (1)))>0) || (isValidSubCall (parts.at(0)) ) || (isValidSubCall (parts.at(1)) ));


        if (parts.at(0).length ()<parts.at(1).length ())
        {
            return ( ((isAPrefix (parts.at (0)))>0) && (isValidSubCall (parts.at(1)) ) );
        }
        else
        { //Both lenght are just the same or the second is showter, we need to check both parts and return true if one is valid
          // It may happen that we have a EA4K/QRP or EA4K/F or EA4K/TTT
            return isValidSubCall (parts.at(0));
        }
    }
    return isValidSubCall (call);
}

QString Utilities::getPrefixFromCall(const QString &_c)
{
    //qDebug() << "Utilities::getPrefixFromCall: " << _c << endl;

    QString call = _c;
    call.replace('\\', '/');

    if (call.count('/') == 2)
    { //Things like F/EA4K/P will become F/EA4K
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        call = parts.at(0)+parts.at(1);
    }

    QString prefix = QString();
    int pref = -1;
    if (call.count('/') == 1)
    { // Complex calls (like F/EA4K or EA4K/F OR /p OR /qrp

        QStringList parts;
        parts.clear();
        parts << call.split ('/');
        if (parts.at(0).length ()<parts.at(1).length ())
        { // First one is shorter
            //qDebug() << "Utilities::getPrefixFromCall: First one is shorter: " << endl;
            int pref = isAPrefix (parts.at(0));
            if (pref>0)
            {
                //qDebug() << "Utilities::getPrefixFromCall: R1=" << call.left (pref) << endl;
                return parts.at(0);
            }
            else
            {
                //qDebug() << "Utilities::getPrefixFromCall: EMPTY-2"  << endl;
                return QString();
            }
        }
        else if(parts.at(0).length ()>parts.at(1).length ())
        { // Second one is shorter
            //qDebug() << "Utilities::getPrefixFromCall: Second one is shorter: " << endl;
            pref = isAPrefix (parts.at(1));
            if (pref>0)
            {
                //qDebug() << "Utilities::getPrefixFromCall: R2=" << parts.at(1) << endl;

                return parts.at(1);
            }
            else
            {
                pref = isAPrefix (parts.at(0));
                if (pref>0)
                {
                    //qDebug() << "Utilities::getPrefixFromCall: R3=" << parts.at(0).left (pref) << endl;
                    return parts.at(0).left (pref);
                }
                else
                {
                    //qDebug() << "Utilities::getPrefixFromCall: EMPTY-3"  << endl;
                    return QString();
                }
            }

        }
        else
        { //Both lenght are just the same, we need to check both parts and return true if one is valid
            //qDebug() << "Utilities::getPrefixFromCall: Same length, we shoudl consider the first one if valid, if not the second one " << endl;
        }
    }

    pref = isAPrefix (call);
    if (pref>0)
    {
        //qDebug() << "Utilities::getPrefixFromCall: R4=" << call.left (pref) << endl;
        return call.left (pref);
    }
    //qDebug() << "Utilities::getPrefixFromCall: EMPTY-4"  << endl;
    return QString();
}

/*
QPair<QString, QString> Utilities::getCallParts(const QString &_c)
{ // DXCC prefix, if different from original, full call

    QString call = _c;
    QPair<QString, QString> pair;
    pair.first = QString();
    pair.second = QString();

    if (_c.length()<3)
    {
        //qDebug() << "Utilities::isValidCall: FALSE-1: " << _c << endl;
        return pair;
    }

    call.replace('\\', '/');

    if (call.count('/')>2)
    {
        //qDebug() << "Utilities::isValidCall: FALSE-3: " << call << endl;
        return pair;
    }
    if (call.count('/') == 2)
    { //Things like F/EA4K/P will become F/EA4K
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        call = parts.at(0)+parts.at(1);
    }
    QString prefix = QString();

    if (call.count('/') == 1)
    { // Complex calls (like F/EA4K or EA4K/F OR /p OR /qrp
        QStringList parts;
        parts.clear();
        parts << call.split('/');
        //qDebug() << "Utilities::isValidCall: first: " << parts.at(0) << endl;
        //qDebug() << "Utilities::isValidCall: second: " << parts.at(1) << endl;

        QStringList validSuffixes = {"P", "M", "MM", "QRP", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
        if (validSuffixes.contains (parts.at(1)))
        {
            //qDebug() << "Utilities::isValidCall: returning a result just with: " << parts.at(1) << endl;
            pair.second = parts.at(1);
            return pair;
        }

        if (parts.at(0).length ()<=parts.at(1).length ())
        {
            if (isAPrefix (parts.at(0))>0)
            //if (isAPrefix (parts.at(0)))
            {
                //qDebug() << "Utilities::isValidCall: first is shorter " << endl;
                pair.first = parts.at(0);
                pair.second = parts.at(1);
            }
            else
            {
                //qDebug() << "Utilities::isValidCall: first is shorter but not a prefix" << endl;
                return pair;
            }
        }
        else
        {
             //qDebug() << "Utilities::isValidCall: second is shorter " << endl;
             if (isAPrefix (parts.at(1))>0)
             {

                 pair.first = parts.at(1);
                 pair.second = parts.at(0);
             }
             else
             {
                 //qDebug() << "Utilities::isValidCall: second is shorter but not a prefix" << endl;
                 return pair;
             }
        }
    }
    return pair;
}
*/

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
         //qDebug() << "Utilities::isDBFileExisting: " << getKLogDBFile() << endl;

    if (QFile::exists(getKLogDBFile()))
    {
             //qDebug() << "Utilities::isDBFileExisting - true" << endl;
        return true;
    }
    else
    {
             //qDebug() << "Utilities::isDBFileExisting - false" << endl;
        return false;
    }
    //return false;
}

bool Utilities::isDBFileExisting(const QString &_file)
{
         //qDebug() << "Utilities::isDBFileExisting2: " << _file << endl;

    if (QFile::exists(_file))
    {
             //qDebug() << "Utilities::isDBFileExisting2 - true" << endl;
        return true;
    }
    else
    {
             //qDebug() << "Utilities::isDBFileExisting2 - false" << endl;
        return false;
    }
    //return false;
}

bool Utilities::isValidADIFField(const QString &_b)
{
       //qDebug() << "Utilities::isValidADIFField: " << _b << endl;
    /*
        This functions checks if the ADIF field has the proper format.
        <Field:length:Data type>Data
    */

    if (!((_b.startsWith('<')) &&  (_b.count('>')) == 1 ))
    {
           //qDebug() << "Utilities::isValidADIFField: BAD FORMAT: No < or > delimiters: " << _b << endl;
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
           //qDebug() << "Utilities::isValidADIFField-0 (not two): " << QString::number(qs.size()) << endl;
        return false;
    }

    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

      //qDebug() << "Utilities::isValidADIFField-Field: " << field << endl;
      //qDebug() << "Utilities::isValidADIFField_Data: " << data << endl;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
        if (!validDataTypes.contains(dataType.toUpper()))
        {
               //qDebug() << "Utilities::isValidADIFField - FORMAT ERROR: Wrong data type: " << dataType << endl;
            return false;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
           //qDebug() << "Utilities::isValidADIFField - FORMAT ERROR, more than 2 \":\" - " << field << endl;
        return false;
    }

    if ( length != separatorPosition)
    {
           //qDebug() << "Utilities::isValidADIFField: Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) << endl;
        return false;
    }

    if (separatorPosition <= 0)
    {
        //qDebug() << "Utilities::isValidADIFField: Length problem <= 0" << endl;
        return false;
    }

       //qDebug() << "FileManager::checkADIFValidFormat: Return true" << endl;

    return true;
}

bool Utilities::isValidQSL_Rcvd(const QString &c)
{
    if ((c == "Y") | (c == "N") | (c == "R") | (c == "I") | (c == "V"))
    {
        return true;
    }
    else {
        return false;
    }
}

bool Utilities::isValidQSL_Sent(const QString &c)
{
    if ((c == "Y") | (c == "N") | (c == "R") | (c == "Q") | (c == "I"))
    {
        return true;
    }
    else {
        return false;
    }
}

QStringList Utilities::getValidADIFFieldAndData(const QString &_b)
{
   //qDebug() << "Utilities::getValidADIFFieldAndData: " << _b << endl;
    /*
        This functions checks if the ADIF field has the proper format.
        <Field:length:Data type>Data
    */
    QStringList result;
    result.clear();

    if (!(_b.startsWith('<')))
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: BAD FORMAT: No < or > delimiters: " << _b << endl;
        return QStringList();
    }
    if (_b.simplified() == "<EOR>")
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: EOR" << endl;
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

    //qDebug() << "Utilities::getValidADIFFieldAndData: -20" << endl;
    QStringList validDataTypes = {"B", "N", "D", "T", "S", "I", "M", "G", "E", "L"};

    qs.clear();
    qs.append(aux.split('>'));

    if (qs.size()!= 2)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData-0 (not two): " << QString::number(qs.size()) << endl;
        return result;
    }
    //qDebug() << "Utilities::getValidADIFFieldAndData: -30" << endl;
    //QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

    //qDebug() << "Utilities::getValidADIFFieldAndData-Field: " << field << endl;
    //qDebug() << "Utilities::getValidADIFFieldAndData_Data: " << data << endl;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
          //qDebug() << "Utilities::getValidADIFFieldAndData - DataType: -" << dataType << "-" << endl;
        if (!validDataTypes.contains(dataType.toUpper()))
        {

              //qDebug() << "Utilities::getValidADIFFieldAndData - FORMAT ERROR: Wrong data type: " << dataType << endl;
            return result;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData - FORMAT ERROR, more than 2 \":\" - " << field << endl;
        return result;
    }
    //qDebug() << "Utilities::getValidADIFFieldAndData: -60" << endl;
    if ( length != separatorPosition)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) << endl;
        return result;
    }

    if (separatorPosition <= 0)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: Length problem <= 0" << endl;
        return result;
    }
     //qDebug() << "Utilities::getValidADIFFieldAndData: -90: f: " << field << endl;
      //qDebug() << "Utilities::getValidADIFFieldAndData: -90: d: " << data<< endl;
    //field = field.section(':', 0, 0);
    result.clear();
    result << field.section(':', 0, 0) << data;
     //qDebug() << "Utilities::checkADIFValidFormat: Return true: " << result.at(0) << "/" << result.at(1) << endl;
    return result;
}

QString Utilities::getAValidCall (const QString &_wrongCall)
{
    //qDebug() << "Utilities::getAValidCall: " << _wrongCall << endl;
    QString _confirmedCall;
    _confirmedCall.clear();

    bool ok;
    if (_wrongCall.length() > 0)
    {

        //qDebug() << "Utilities::getAValidCall (Don't have VALID CALL): " << _wrongCall << endl;
        _confirmedCall = QString(QObject::tr("A wrong callsign has been found: %1. Please enter a new callsign or confirm that the current one is a good callsign.")).arg(_wrongCall);
    }
    else
    {
        //qDebug() << "Utilities::getAValidCall (Don't have ANY CALL): " << _wrongCall << endl;
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



    //qDebug() << "Utilities::getAValidCall: " << _confirmedCall << endl;
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
   //qDebug() << "Utilities::getDateFromADIFDateString: " << _s << endl;
    return QDate::fromString(_s, "yyyyMMdd");

}

QTime Utilities::getTimeFromADIFTimeString(const QString &_s)
{// Expects and ADIF TIME format String "HHMMSS" or "HHMM"
    //qDebug() << "Utilities::getTimeFromADIFTimeString: " << _s << endl;
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
    //qDebug() << "Utilities::getDateFromLoTWQSLDateString: " << _s << endl;
    QStringList datet;
    datet.clear();
    datet << _s.split(" ");
    //qDebug() << "Utilities::getDateFromLoTWQSLDateString: date:" << datet.at(0) << endl;
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
/*
QPalette Utilities::getPalete(bool _ok)
{
   //qDebug() << "Utilities::getPalete"  << endl;
    if (_ok)
    {
       //qDebug() << "Utilities::getPalete - true"  << endl;
        return palRed;
        //return QPalette::setColor(QPalette::Text, Qt::red);
    }
    else
    {
       //qDebug() << "Utilities::getPalete - false"  << endl;
        return palBlack;
    }
}
*/
