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
//#include <cstdlib>
#include <QTextStream>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "startwizard.h"
#include "mainwindow.h"
#include "utilities.h"


int showNoDB()
{
    QMessageBox msgBox;
    Utilities util(Q_FUNC_INFO);
    QString msg = QString("There is no DB in the KLog folder and could not create it. KLog can't continue like this.\n") +
                  QString("Please remove or rename the %1 file and restart KLog.").arg(util.getCfgFile());

    msgBox.setWindowTitle("KLog");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return -1;
}

int showErrorUpdatingTheDB()
{
    QMessageBox msgBox;
    Utilities util(Q_FUNC_INFO);
    QString msg = QString("KLog could not update the DB and it is a critical step for this release. KLog can't continue like this.\n") +
                  QString("Please use the previews version of KLog to export your log to ADIF. Then do a clean installation of the new KLog version and import the ADIF file. If that doesn't work, please contact the KLog development team.");

    msgBox.setWindowTitle("KLog");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return -1;
}

// Function to handle missing translation files
void loadTranslations(QApplication &app, QTranslator &myappTranslator)
{
    Utilities util(Q_FUNC_INFO);
    QString translationPath, translationPath2;
    QString language = (QLocale::system().name()).left(2);
    bool missingTranslation = true;

// Check possible translation paths
#if defined(Q_OS_WIN)
    translationPath = QCoreApplication::applicationDirPath() + "/translations/klog_" + language + ".qm";
#elif defined(Q_OS_OSX)
    translationPath = QCoreApplication::applicationDirPath() + "/translations/klog_" + language + ".qm";
#else
    translationPath = "/usr/share/klog/translations/klog_" + language + ".qm";
    translationPath2 = util.getHomeDir() + "/translations/klog_" + language + ".qm";
#endif

    if (myappTranslator.load(translationPath) || myappTranslator.load(translationPath2))
    {
        missingTranslation = false;
    }

    if (missingTranslation && language != "en")
    {
        QMessageBox::warning(nullptr, "KLog",
                             QString("No translation files for your language were found. KLog will be shown in English.\n")
                                 + QString("If you have the klog_%1.qm file for your language, copy it to the translations folder and restart KLog.")
                                       .arg(language));
    }

    app.installTranslator(&myappTranslator);
}

int main(int argc, char *argv[])
{
    //qDebug() << Q_FUNC_INFO << " -  Start! ";
    //qDebug() << Q_FUNC_INFO << " -  " << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QT_REQUIRE_VERSION(argc, argv, "6.0")
    //qDebug() << QT_VERSION_STR;

    QDir d1 = QDir();
    //qDebug() << Q_FUNC_INFO << " - STARTED: ";
    Utilities util(Q_FUNC_INFO);
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

    QString pkgVersion = QString(APP_PKGVERSION);
    //qDebug() << Q_FUNC_INFO << " -  -10 ";
    //qDebug() << Q_FUNC_INFO << " - " << pkgVersion;

    // Now we check if the user is executing from the command line


    // Command-line arguments
    QTextStream cout(stdout);
    QStringList arguments = app.arguments();
    if (arguments.size() > 1)
    {
        Utilities util(Q_FUNC_INFO);
        if (arguments.contains("-v"))
        {
            cout << "Version: KLog-" << app.applicationVersion() << " - pkg: " << pkgVersion << "\n";
        }
        else
        {
            util.printCommandHelp();
        }
        return 0;
    }

    /* Application Singleton
     *
     * We want to run only one instance of KLog application
     *
     */
    QSystemSemaphore semaphore("klogapp", 1);  // create semaphore with unique ID klogapp
    semaphore.acquire();                       // Raise the semaphore, barring other instances to work with shared memory

#ifndef KLOG_Q_OS_WIN
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

    // Load translations
    //qDebug() << Q_FUNC_INFO << " -  Start of translation activities: "<< (QTime::currentTime()).toString("HH:mm:ss");
    //qDebug() << Q_FUNC_INFO << " -  Detected language: " << (QLocale::system().name()).left(2) << ".qm";
    QTranslator myappTranslator;
    loadTranslations(app, myappTranslator);


    QString klogDir = util.getHomeDir();

    //qDebug() << Q_FUNC_INFO << " - 10";
    //qDebug() << Q_FUNC_INFO << " - Setting klog dir: " << (QTime::currentTime()).toString("HH:mm:ss")<< QT_ENDL;;
    // First step when running KLog, if the KLog folder does not exist, KLog creates it
    if (!QDir::setCurrent (klogDir) )
    {
        //qDebug() << Q_FUNC_INFO << " - KLogDir does not exist.... creating ";
        if (d1.mkdir(klogDir))
        {
            if (QDir::setCurrent (klogDir) )
            {
                    //qDebug() << Q_FUNC_INFO << " - KLogDir has just been created and pointed ";
            }
        }
    }
    //qDebug() << Q_FUNC_INFO << " -  Setting klog dir - finished: " << (QTime::currentTime()).toString("HH:mm:ss");

    //qDebug() << Q_FUNC_INFO << " -  Setting config file: " << (QTime::currentTime()).toString("HH:mm:ss") ;
    QPixmap pixmap(":img/klog_512x512.png");
   //qDebug() << Q_FUNC_INFO << " - 51" << (QTime::currentTime()).toString("HH:mm:ss");
    QSplashScreen splash(pixmap);
    //int firstTime = true;
    // If the KLog configuration file does not exist, we launch the wizard.
    if (!((QFile::exists(util.getCfgFile ()))))
    {
        //qDebug() << Q_FUNC_INFO << " -  Starting wizard... ";
        StartWizard *wizard = new StartWizard(klogDir, version);
        wizard->setModal(true);
        wizard->exec();
    }
    else
    {   // KLog configuration file exists, let's look for the DB
       //qDebug() << Q_FUNC_INFO << " -  Start of DB Activities" << (QTime::currentTime()).toString("HH:mm:ss");
        //firstTime = false;
        DataBase *db = new DataBase(Q_FUNC_INFO, version, util.getKLogDBFile());
       //qDebug() << Q_FUNC_INFO << " -  After Start of DB Activities";
        if (!db->createConnection(Q_FUNC_INFO))
        {
           //qDebug() << Q_FUNC_INFO << " - Conection not created";
            return showNoDB();
            //return -1; // Exits with an error; no DB has been created
        }
        else
        {
           //qDebug() << Q_FUNC_INFO << " - DB to be updated";
            if (!db->updateIfNeeded()) // Check if we need to update the DB
            {
               //qDebug() << Q_FUNC_INFO << " - DB NOT Updated";
            }

           //qDebug() << Q_FUNC_INFO << " - DB Updated";
        }
       //qDebug() << Q_FUNC_INFO << " - 98" << (QTime::currentTime()).toString("HH:mm:ss");
        db->~DataBase();
       //qDebug() << Q_FUNC_INFO << " - 99" << (QTime::currentTime()).toString("HH:mm:ss");
    }
  //qDebug() << Q_FUNC_INFO << " - 100 - Show" << (QTime::currentTime()).toString("HH:mm:ss");
    splash.show();
   //qDebug() << Q_FUNC_INFO << " - 101 " << (QTime::currentTime()).toString("HH:mm:ss");

    splash.showMessage ("Creating the Data Base...");
    DataProxy_SQLite dataProxy (Q_FUNC_INFO, version);
    World world(&dataProxy, Q_FUNC_INFO);
    dataProxy.setPKGVersion(pkgVersion);

    splash.showMessage ("Creating window...");
   //qDebug() << Q_FUNC_INFO << " - 102 " << (QTime::currentTime()).toString("HH:mm:ss");

    MainWindow mw(&dataProxy, &world);
    splash.showMessage ("Initializing window...");
    //qDebug() << Q_FUNC_INFO << " - 103 " << (QTime::currentTime()).toString("HH:mm:ss");

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
    //mw.showNotWar();
    //qDebug() << Q_FUNC_INFO << " - 113 " << (QTime::currentTime()).toString("HH:mm:ss");
    //qDebug() << Q_FUNC_INFO << " - END";
    return app.exec();
}
