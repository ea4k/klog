/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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
#include <QtWidgets>
//#include <QtSql>
#include <QTranslator>
#include <cstdlib>
#include <QTextStream>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "klogdefinitions.h"
#include "startwizard.h"
#include "mainwindow.h"
#include "utilities.h"



int main(int argc, char *argv[])
{
    //qDebug() << "KLog Main: Start! ";
    //qDebug() << "KLog Main: " << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QT_REQUIRE_VERSION(argc, argv, "5.9")
    //qDebug() << QT_VERSION_STR;

    QDir d1 = QDir();
    //QCoreApplication::setApplicationVersion(QString(APP_VERSION));
    //qDebug() << "KLog Main STARTED: " << version;
    Utilities util(Q_FUNC_INFO);
    QStringList arguments;
    QTextStream cout(stdout);
    //QCoreApplication::setOrganizationName("EA4K");
    //QCoreApplication::setOrganizationDomain("klog.xyz");
    //QCoreApplication::setApplicationName("KLog");
    QApplication app(argc, argv);

    QString iconSt;
    iconSt = ":/img/klog.ico";
    QIcon KLogIcon(iconSt);
    QApplication::setWindowIcon(KLogIcon);
    app.setApplicationName(QString("KLog"));
    app.setOrganizationName("EA4K");
    app.setOrganizationDomain("klog.xyz");
    app.setApplicationVersion(QString(APP_VERSION));
    QString version = QCoreApplication::applicationVersion();
    //qDebug() << "KLog Main: -10 ";
    // Now we check if the user is executing from the command line
    arguments.clear();
    arguments << app.arguments();
    if (arguments.length()>1)
    {
        if (arguments.contains("-h"))
        {
            //cout << "Usage: klog [OPTION]... [FILE]...";

            cout << "Usage: klog [OPTION]...";
            cout << "Options:";
            cout << "     -?           Display this help";
            cout << "     -h           Display this help";
            cout << "     -v           Display program version";

            //cout << "     -e <file>    Export Adif file <file>";
        }
        else if (arguments.contains("-?"))
        {
            cout << "Usage: klog [OPTION]...";
            cout << "Options:";
            cout << "     -?           Display this help";
            cout << "     -h           Display this help";
            cout << "     -v           Display program version";
        }
        else if (arguments.contains("-v"))
        {
            cout << "Version: KLog-" << app.applicationVersion();

        }
        else
        {
            cout << "Usage: klog [OPTION]...";
            cout << "Options:";
            cout << "     -?           Display this help";
            cout << "     -h           Display this help";
            cout << "     -v           Display program version";
        }
        app.quit();
        return 0;
    }
    //qDebug() << "KLog Main: Start of translation activities: "<< (QTime::currentTime()).toString("HH:mm:ss");
    //qDebug() << "KLog Main: Detected language: " << (QLocale::system().name()).left(2) << ".qm";
    // Translations begin
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)); /* Flawfinder: ignore */
    app.installTranslator(&qtTranslator);
    QTranslator myappTranslator;

    bool missingTranslation = false;
    //QString msgOSFilePath = QString();        // The OS depending part of the message to be printed if no translation is found.

    #if defined(Q_OS_WIN)
        //qDebug() << "KLog WIN ";
        //qDebug() << "KLog Main: -20 - WIN";
        if (QFile::exists(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm") ) /* Flawfinder: ignore */
        {
            myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm"); /* Flawfinder: ignore */
        }
        else if (QFile::exists(QDir::homePath()+"/klog/klog_" + (QLocale::system().name()).left(2)+ ".qm") ) /* Flawfinder: ignore */
        {
            myappTranslator.load(QDir::homePath()+"/klog/klog_" + (QLocale::system().name())); /* Flawfinder: ignore */
        }
        else if (((QLocale::system().name()).left(2)) == "en") /* Flawfinder: ignore */
        { // If language is English, it will execute without showing message
        }
        else
        {
            missingTranslation = true;
            //msgOSFilePath = QCoreApplication::applicationDirPath() + "/translations/" ;
        }
      //qDebug() << "KLog Main: -20 - end WIN ";
    #elif defined(Q_OS_OSX)
    //qDebug() << "KLog OSX ";

        if (QFile::exists(QCoreApplication::applicationDirPath() + "/translations/klog_" +  (QLocale::system().name()).left(2) + ".qm") ) /* Flawfinder: ignore */
        {
            myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm"); /* Flawfinder: ignore */
        }
        else if (((QLocale::system().name()).left(2)) == "en") /* Flawfinder: ignore */
        { // If language is English, it will execute without showing message

        }
        else
        {
            missingTranslation = true;
        }
    #else
           //qDebug() << "KLog OTHER OS: " << (QLocale::system()).name();
        if (QFile::exists("klog_" + (QLocale::system().name()).left(2) + ".qm") ) /* Flawfinder: ignore */
        {
            myappTranslator.load("klog_" + (QLocale::system().name()).left(2)); /* Flawfinder: ignore */
        }
        else if (QFile::exists("/usr/share/klog/translations/klog_" + (QLocale::system().name()).left(2) + ".qm") ) /* Flawfinder: ignore */
        {
               //qDebug() << "KLog OTHER -2: " << "/usr/share/klog/klog_" + (QLocale::system().name()).left(2); /* Flawfinder: ignore */
            myappTranslator.load("/usr/share/klog/translations/klog_" + (QLocale::system().name()));  /* Flawfinder: ignore */
        }
        else if (QFile::exists(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm")) /* Flawfinder: ignore */
        {
            //qDebug() << "KLog OTHER -3: " << QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2);
            myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name())); /* Flawfinder: ignore */
        }

        else if (((QLocale::system().name()).left(2)) == "en") /* Flawfinder: ignore */
        { // If language is English, it will execute without showing message

        }
        else
        {
            missingTranslation = true;
            //sgOSFilePath = QCoreApplication::applicationDirPath() + "/translations/" ;
        }

    #endif
    //qDebug() << "KLog Main: -40 ";
    if (missingTranslation)
        {
            //qDebug() << "KLog Main: Translation missing! ";
            QMessageBox msgBox;
            QString urlTranslate = QString();
            urlTranslate = "<p><a href=\"https://translate.google.com/?sl=auto&tl=auto#en/auto/No%20translation%20files%20for%20your%20language%20have%20been%20found%20so%20KLog%20will%20be%20shown%20in%20English.%0A%0AIf%20you%20have%20the%20klog_en.qm%20file%20for%20your%20language%2C%20you%20can%20copy%20it%20in%20the%20%2Fhome%2Fdevel%2F.klog%2F%20folder%20and%20restart%20KLog%20again.%0A%0A%20If%20you%20want%20to%20help%20to%20translate%20KLog%20into%20your%20language%2C%20please%20contact%20the%20author.\">TRANSLATE</a></p>";

            QString msg = QString();

            msgBox.setWindowTitle("KLog");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setTextFormat(Qt::RichText);
            QString language = (QLocale::system().name()).left(2); /* Flawfinder: ignore */

            msg = QString("No translation files for your language have been found so KLog will be shown in English.") + "<p>" +
                  QString("If you have the klog_%1.qm file for your language, you can copy it into the %2/translations/ folder and restart KLog.</p><p>If you want to help to translate KLog into your language, please contact the author.").arg(language).arg(QCoreApplication::applicationDirPath()) +
                  "</p>" + urlTranslate;

            msgBox.setText(msg);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();

        }

    //qDebug() << "KLog Main-1" << (QTime::currentTime()).toString("HH:mm:ss") ;

    app.installTranslator(&myappTranslator);
    //qDebug() << "KLog Main: End of translation activities: "<< (QTime::currentTime()).toString("HH:mm:ss");
    // Traslations end

    /* Application Singleton
     *
     * We want to run only one instance of KLog application
     */
    QSystemSemaphore semaphore("klogapp", 1);  // create semaphore with unique ID klogapp
    semaphore.acquire();                       // Raise the semaphore, barring other instances to work with shared memory

#ifndef Q_OS_WIN
    // in linux / unix shared memory is not freed when the application terminates abnormally,
    // so you need to get rid of the garbage
    QSharedMemory nix_fix_shared_memory("klogshm");

    if (nix_fix_shared_memory.attach())
    {
        nix_fix_shared_memory.detach(); // if there is no running instance then it remove the orphaned shared memory
    }
#endif

    QSharedMemory sharedMemory("klogshm");  // Create a copy of the shared memory - Unique ID klogshm
    bool is_running;

    /*
     * trying to attach a copy of the shared memory to an existing segment
     *
     * if successful, it determines that there is already a running instance
     * otherwise allocate 1 byte of memory and no instance is running
     */
    if (sharedMemory.attach())
    {
        is_running = true;
    }
    else
    {
        sharedMemory.create(1);
        is_running = false;
    }

    semaphore.release();

    // If you already run one instance of the application, then we inform the user about it
    // and complete the current instance of the application
    if (is_running)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::tr("KLog is already running.") + "\n" + QObject::tr("It is allowed to run only one instance."));
        msgBox.exec();
        return 1;
    }

    // END OF Application Singleton

    QString klogDir = util.getHomeDir();
    //configFileName = util.getCfgFile();

    //qDebug() << "KLog Main-10";

    //qDebug() << "KLog Main: Setting klog dir: " << (QTime::currentTime()).toString("HH:mm:ss")<< QT_ENDL;;
    if (!QDir::setCurrent (klogDir) )
    {
        //qDebug() << "MAIN:  KLogDir does not exist.... creating ";
        if (d1.mkdir(klogDir))
        {
            if (QDir::setCurrent (klogDir) )
            {
                    //qDebug() << "MAIN:  KLogDir has just been created and pointed ";
            }
        }
    }
    //qDebug() << "KLog Main: Setting klog dir - finished: " << (QTime::currentTime()).toString("HH:mm:ss");

    //qDebug() << "KLog Main: Setting config file: " << (QTime::currentTime()).toString("HH:mm:ss") ;
    QPixmap pixmap(":img/klog_512x512.png");
    //qDebug() << "KLog Main-51" << (QTime::currentTime()).toString("HH:mm:ss");
    QSplashScreen splash(pixmap);


    if (!((QFile::exists(util.getCfgFile ())) || (QFile::exists(util.getSetFile ()))))
    {
        //qDebug() << "MAIN:  Starting wizard... ";
        StartWizard *wizard = new StartWizard(klogDir, version);
        wizard->setModal(true);
        //int inMemory = wizard->exec();
        wizard->exec();
        //MainWindow mw(version);
        //mw.init();
        //splash.finish(&mw);
        //mw.showNotWar();
        //mw.show();
        //return app.exec();
    }
    else
    {
        //qDebug() << "Main: Start of DB Activities" << (QTime::currentTime()).toString("HH:mm:ss");
        DataBase *db = new DataBase(Q_FUNC_INFO, version, util.getKLogDBFile());
        //qDebug() << "Main: After Start of DB Activities";
        if (!db->createConnection(Q_FUNC_INFO))
        {
            //qDebug() << "Main: Conection not created";
            return -1; // Exits with an error; no DB has been created
        }
        else
        {
            //qDebug() << "Main: DB to be updated";
            db->updateIfNeeded(); // Check if we need to update the DB
            //qDebug() << "Main: DB Updated";
        }
        //qDebug() << "Main: DB Updated";
        //qDebug() << "KLog Main-98" << (QTime::currentTime()).toString("HH:mm:ss");
        db->~DataBase();
        //qDebug() << "KLog Main-99" << (QTime::currentTime()).toString("HH:mm:ss");
    }
    //qDebug() << Q_FUNC_INFO << " - 100 " << (QTime::currentTime()).toString("HH:mm:ss");
    splash.show();
    //qDebug() << Q_FUNC_INFO << " - 101 " << (QTime::currentTime()).toString("HH:mm:ss");
    splash.showMessage ("Creating window...");
    //qDebug() << Q_FUNC_INFO << " - 102 " << (QTime::currentTime()).toString("HH:mm:ss");
    MainWindow mw(version);
    //qDebug() << Q_FUNC_INFO << " - 103 " << (QTime::currentTime()).toString("HH:mm:ss");
    splash.showMessage ("Initializing window...");
    //qDebug() << Q_FUNC_INFO << " - 104 " << (QTime::currentTime()).toString("HH:mm:ss");
    mw.init();
    //qDebug() << Q_FUNC_INFO << " - 105 " << (QTime::currentTime()).toString("HH:mm:ss");
    splash.showMessage ("Checking for new versions...");
    //qDebug() << Q_FUNC_INFO << " - 106 " << (QTime::currentTime()).toString("HH:mm:ss");
    mw.checkIfNewVersion();
    //qDebug() << Q_FUNC_INFO << " - 107 " << (QTime::currentTime()).toString("HH:mm:ss");
    splash.showMessage ("Checking if backup is needed...");
    //qDebug() << Q_FUNC_INFO << " - 108 " << (QTime::currentTime()).toString("HH:mm:ss");
    mw.recommendBackupIfNeeded();
    //qDebug() << Q_FUNC_INFO << " - 109 " << (QTime::currentTime()).toString("HH:mm:ss");
    splash.showMessage ("Showing window...");
    //qDebug() << Q_FUNC_INFO << " - 110 " << (QTime::currentTime()).toString("HH:mm:ss");
    mw.show();
    //qDebug() << Q_FUNC_INFO << " - 111 " << (QTime::currentTime()).toString("HH:mm:ss");
    splash.finish(&mw);
    //qDebug() << Q_FUNC_INFO << " - 112 " << (QTime::currentTime()).toString("HH:mm:ss");
    mw.showNotWar();
    //qDebug() << Q_FUNC_INFO << " - 113 " << (QTime::currentTime()).toString("HH:mm:ss");
    //qDebug() << Q_FUNC_INFO << " - END";
    return app.exec();
}


