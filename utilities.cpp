#include "utilities.h"

Utilities::Utilities()
{
}

int Utilities::getProgresStepForDialog(int totalSteps){
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
    else
        return 50;
}

bool Utilities::trueOrFalse(const QString _s)
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
    return false;
}

QString Utilities::checkAndFixASCIIinADIF(const QString _data)
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

void Utilities::printQString(const QStringList _qs)
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

QString Utilities::getAgent(const QString _klogversion)
{
    QString version;
    version = _klogversion;
    
#if defined(Q_OS_WIN32)
    return "KLog-Win32-" + version;
elif defined(Q_OS_WIN64)
    return "KLog-Win64-" + version;
#elif defined(Q_OS_AIX)
    return "KLog-aix-" + version;    
#elif defined(Q_OS_ANDROID)
    return "KLog-android-" + version;    
#elif defined(Q_OS_BSD4)
    return "KLog-bsd4-" + version;    
#elif defined(Q_OS_BSDI)
    return "KLog-bsdi-" + version;    
#elif defined(Q_OS_CYGWIN)
    return "KLog-cygwin-" + version;    
#elif defined(Q_OS_DARWIN)
    return "KLog-darwin-" + version;    
#elif defined(Q_OS_DGUX)
    return "KLog-dgux-" + version;    
#elif defined(Q_OS_DYNIX)
    return "KLog-dynix-" + version;
#elif defined(Q_OS_FREEBSD)
    return "KLog-freebsd-" + version;    
#elif defined(Q_OS_HPUX)
    return "KLog-hpux-" + version;    
#elif defined(Q_OS_IOS)
    return "KLog-ios-" + version;    
#elif defined(Q_OS_IRIX)
    return "KLog-irix-" + version;    
#elif defined(Q_OS_LYNX)
    return "KLog-lynx-" + version;    
#elif defined(Q_OS_NETBSD)
    return "KLog-netbsd-" + version;    
#elif defined(Q_OS_OPENBSD)
    return "KLog-openbsd-" + version;    
#elif defined(Q_OS_OSF)
    return "KLog-osf-" + version;
#elif defined(Q_OS_QNX)
    return "KLog-qnx-" + version;    
#elif defined(Q_OS_SCO)
    return "KLog-sco-" + version;    
#elif defined(Q_OS_SOLARIS)
    return "KLog-solaris-" + version;    
#elif defined(Q_OS_TVOS)
    return "KLog-tvos-" + version;    
#elif defined(Q_OS_UNIX)
    return "KLog-unix-" + version;    
#elif defined(Q_OS_UNIXWARE)
    return "KLog-unixware-" + version;    
#elif defined(Q_OS_WHATCHOS)
    return "KLog-whatchos-" + version;    
#elif defined(Q_OS_WINRT)
    return "KLog-winrt-" + version;    
#elif defined(Q_OS_LINUX)
    return "KLog-Linux-" + version;
#elif defined(Q_OS_WIN)
    return "KLog-Win-"+ version;
#elif defined(Q_OS_MACOS)
    return "KLog-macOS-" + version;    
#elif defined(Q_OS_OSX)
    return "KLog-OSX-" + version;
#elif defined(Q_OS_MAC)
    return "KLog-MAC-" + version;
    //return "KLog-Linux-" + version;
#else
    return "KLog-Other-" + version;
#endif

    //return "KLog-Unknown-" + version;
}

QString Utilities::getHomeDir()
{
//TODO: To be removed when the defaultDir is saved in the config file
#ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!: "  << QDir::homePath() + "/klog" << endl;
    return QDir::homePath()+"/klog";  // We create the \klog for the logs and data

#else
    //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    return QDir::homePath()+"/.klog";  // We create the ~/.klog for the logs and data

#endif

}

QString Utilities::getKLogDatabaseFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
#ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!: "  << getHomeDir() + "/logbook.dat" << endl;
    return getHomeDir() + "/logbook.dat";

#else
    //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    return getHomeDir() + "/logbook.dat";

#endif

}




QString Utilities::getCfgFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
#ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  << endl;
    return getHomeDir() + "/klogrc.cfg";

#else
    //qDebug() << "NO WINDOWS DETECTED!: " << getHomeDir() + "/klogrc.cfg"  << endl;
    return getHomeDir() + "/klogrc";

#endif

}

QString Utilities::getCTYFile()
{
//TODO: To be removed when the defaultDir is saved in the config file
#ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!"  << endl;
    return getHomeDir() + "/cty.csv";

#else
    //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    return getHomeDir() + "/cty.csv";

#endif

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
