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
    return "C:/Program Files (x86)/TrustedQSL/";
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

bool Utilities::isValidCall(const QString &_c)
{
    //qDebug() << "Utilities::isValidCall: " << _c << endl;
    //Rules: http://life.itu.int/radioclub/rr/art19.pdf
    if (_c.length()<3)
    {
        //qDebug() << "Utilities::isValidCall: FALSE-1: " << _c << endl;
        return false;
    }

    if ( !(_c.at(0).isLetterOrNumber()) )
    {
        //qDebug() << "Utilities::isValidCall: FALSE-2: " << _c << endl;
        return false;
    }


    QString call = _c;
    //qDebug() << "Utilities::isValidCall: -10 "  << endl;
    if ((call.contains('/')) || (call.contains('\\')))
    {
        call.replace('\\', '/');
        if (call.count('/')>2)
        {
            //qDebug() << "Utilities::isValidCall: FALSE-3: " << call << endl;
            return false;
        }

        QStringList parts;
        parts.clear();
        parts << call.split('/');

        if ((parts.at(0)).length()==(parts.at(1)).length())
        {
            if ((((parts.at(0)).back()).isLetter()) && !(((parts.at(1)).back()).isLetter()))
            {
                call = parts.at(0);
            }
            else
            {
                call = parts.at(1);
            }
        }
        else if ((parts.at(0)).length()>(parts.at(1)).length())
        {
            call = parts.at(0);
            if (parts.length()>2)
            {
                if( (parts.at(2)).length() > call.length() )
                {
                    call = parts.at(2);
                }
            }
        }
        else
        {
            call = parts.at(1);
            if (parts.length()>2)
            {
                if( (parts.at(2)).length() > call.length() )
                {
                    call = parts.at(2);
                }
            }
        }
    }
    //qDebug() << "Utilities::isValidCall: -20 "  << endl;
    for (int i=0; i<call.length(); i++)
    {
        //qDebug() << "Utilities::isValidCall: FOR: " << call << "/" << call.at(i)  << endl;
        if (( !(call.at(i).isLetterOrNumber()) ) && !(call.at(i) != "/"))
        {
            //qDebug() << "Utilities::isValidCall: FALSE-4 " << call << endl;
            return false;
        }
    }

   /*
    if ((call.back()).isDigit())
    {
        //qDebug() << "Utilities::isValidCall: FALSE-3 " << call << endl;
        return false;
    }
 */
    //qDebug() << "Utilities::isValidCall: -30 "  << endl;
    QChar firstChar = call.at(0);
    QChar secondChar = call.at(1);
    QChar thirdChar = call.at(2);

    if ((call.length() == 3) && ( thirdChar.isDigit()))
    {
        //qDebug() << "Utilities::isValidCall: FALSE-5: " << call << endl;
        return false;
    }

    // The first two characters shall be two letters or a letter followed
    // by a digit or a digit followed by a letter. The first two characters or in certain cases
    // the first character of a call sign constitute the nationality identification4

    if (firstChar.isDigit() && secondChar.isDigit())
    {
        //qDebug() << "Utilities::isValidCall: FALSE-6: " << call << endl;
        return false;
    }

    if (firstChar.isLetter() && secondChar.isLetter() && thirdChar.isLetter())
    {
          //qDebug() << "Utilities::isValidCall: FALSE-6: " << call << endl;
        return false;
    }

    QList<QChar> validFirstLetters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'I', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'R', 'S', 'T' ,'U', 'V', 'W', 'Z'};

    if ((firstChar.isLetter()) && (secondChar.isDigit()) && (!validFirstLetters.contains(firstChar)) )
    {
        if (validFirstLetters.contains(firstChar))
        {
            if (firstChar == 'C')
            {
                if ((secondChar == '1') || (secondChar == '7'))
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.1: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'D')
            {
                if ((secondChar == '5') )
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.2: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'E')
            {
                if (!(secondChar == '2') && !(secondChar == '3') && !(secondChar == '4'))
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.3: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'H')
            {
                if ((secondChar == '1') )
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.4: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'J')
            {
                if ((secondChar == '1') || (secondChar == '9'))
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.5: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'P')
            {
                if (secondChar == '1')
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.6: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'S')
            {
                if ((secondChar == '1') || (secondChar == '6'))
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.7: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'T')
            {
                if (secondChar == '0')
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.8: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'V')
            {
                if ((secondChar == '1') || (secondChar == '9'))
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.9: " << call << endl;
                    return false;
                }
            }
            if (firstChar == 'Z')
            {
                if (!(secondChar == '2') && !(secondChar == '3'))
                {
                      //qDebug() << "Utilities::isValidCall: FALSE-7.10: " << call << endl;
                    return false;
                }
            }

        }
        else
        {
              //qDebug() << "Utilities::isValidCall: FALSE-8: " << call << endl;
            return false;
        }

          //qDebug() << "Utilities::isValidCall: FALSE-9: " << call << endl;
        return false;
    }






    //qDebug() << "Utilities::isValidCall: TRUE: " << call << endl;
    return true;
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
