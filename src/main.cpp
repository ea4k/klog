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
// #include <QtSql>
#include <QTranslator>
// #include <cstdlib>
#include <iostream>
#include <QTextStream>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include <QLoggingCategory>
#include "startwizard.h"
#include "mainwindow.h"
#include "utilities.h"
#include <QElapsedTimer>

// Suppress verbose debug noise from third-party Qt platform plugins (e.g. qt6ct)
// that unconditionally call//qDebug() for every palette/hint query.
static void klogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Filter out the Qt6CTPlatformTheme palette/hint spam produced by qt6ct
    if (msg.contains(QLatin1String("Qt6CTPlatformTheme")))
        return;
    if (msg.contains(QLatin1String("QThreadStorage")))
        return;
    // Forward everything else to Qt's default handler
    qt_message_output(type, context, msg);
}
#ifdef KLOG_USE_VERSION_H
#include "version.h"
#endif


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
#elif defined(Q_OS_MACOS)
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
        QMessageBox::warning(nullptr,
                             "KLog",
                             QString("No translation files for your language were found. KLog will "
                                     "be shown in English.\n")
                                 + QString("If you have the klog_%1.qm file for your language, "
                                           "copy it to the translations folder and restart KLog.")
                                       .arg(language));
    }

    app.installTranslator(&myappTranslator);
}

int main(int argc, char *argv[])
{
    //qDebug() << Q_FUNC_INFO << " -  Start! ";
    //qDebug() << Q_FUNC_INFO << " -  " << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    qInstallMessageHandler(klogMessageHandler);
    QT_REQUIRE_VERSION(argc, argv, "6.0")
    //qDebug() << QT_VERSION_STR;
    QElapsedTimer timer;
    timer.start();

    QDir d1 = QDir();
    //qDebug() << Q_FUNC_INFO << " - STARTED: ";
    Utilities util(Q_FUNC_INFO);
   //qDebug() << Q_FUNC_INFO << " 001: " << timer.elapsed() << "ms"; timer.restart();
    QApplication app(argc, argv);
   //qDebug() << Q_FUNC_INFO << " 002: " << timer.elapsed() << "ms"; timer.restart();
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
   //qDebug() << Q_FUNC_INFO << " 010: " << timer.elapsed() << "ms"; timer.restart();
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
            cout << "Version: KLog-" << app.applicationVersion()
                 << " - pkg: " << pkgVersion << "\n";
        }
        else
        {
            util.printCommandHelp();
        }
        return 0;
    }
   //qDebug() << Q_FUNC_INFO << " 020: " << timer.elapsed() << "ms"; timer.restart();
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

    if (!nix_fix_shared_memory.create(1))
    {
        // create failed - if the segment already exists (orphaned from a previous crash
        // or unclean exit), we need to attach to it and then detach to release it
        if (nix_fix_shared_memory.error() == QSharedMemory::AlreadyExists)
        {
            nix_fix_shared_memory.attach();
            nix_fix_shared_memory.detach();
        }
    }
    else
    {
        // create succeeded (no orphan existed), detach so main singleton logic can work
        nix_fix_shared_memory.detach();
    }
#endif

    QSharedMemory sharedMemory("klogshm");  // Create a copy of the shared memory - Unique ID klogshm
    bool is_running;
   //qDebug() << Q_FUNC_INFO << " 030: " << timer.elapsed() << "ms"; timer.restart();
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
   //qDebug() << Q_FUNC_INFO << " 040: " << timer.elapsed() << "ms"; timer.restart();
    // If you already run one instance of the application, then we
    // inform the user about it
    // and complete the current instance of the application
    if (is_running)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::tr("KLog is already running.") + "\n" +
                       QObject::tr("It is allowed to run only one instance."));
        msgBox.exec();
        return 1;
    }

    // END OF Application Singleton

    // Load translations
    //qDebug() << Q_FUNC_INFO << " -  Start of translation activities: "
    //         << (QTime::currentTime()).toString("HH:mm:ss");
    //qDebug() << Q_FUNC_INFO << " -  Detected language: " << (QLocale::system().name()).left(2) << ".qm";
    QTranslator myappTranslator;
    loadTranslations(app, myappTranslator);
   //qDebug() << Q_FUNC_INFO << " 050: " << timer.elapsed() << "ms"; timer.restart();


    QString klogDir = util.getHomeDir();

    //qDebug() << Q_FUNC_INFO << " - 10";
    //qDebug() << Q_FUNC_INFO << " - Setting klog dir: " << (QTime::currentTime()).toString("HH:mm:ss")<< QT_ENDL;;
    // First step when running KLog, if the KLog folder does not exist, KLog creates it
    if (!QDir::setCurrent (klogDir) ){
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
  //qDebug() << Q_FUNC_INFO << " 051: " << timer.elapsed() << "ms"; timer.restart();
    QSplashScreen splash(pixmap);
    splash.show();
    QApplication::processEvents();

    splash.showMessage("Checking database...");
    QApplication::processEvents();
    //int firstTime = true;
    // If the KLog configuration file does not exist, we launch the wizard.
    if (!((QFile::exists(util.getCfgFile ()))))
    {
       //qDebug() << Q_FUNC_INFO << " 052: " << timer.elapsed() << "ms"; timer.restart();
        StartWizard *wizard = new StartWizard(klogDir, version);
        wizard->setModal(true);
        wizard->exec();
        delete wizard;
    }
    else
    {   // KLog configuration file exists, let's look for the DB
      //qDebug() << Q_FUNC_INFO << " 060: " << timer.elapsed() << "ms"; timer.restart();
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
        delete db;
      //qDebug() << Q_FUNC_INFO << " 069: " << timer.elapsed() << "ms"; timer.restart();
    }
   //qDebug() << Q_FUNC_INFO << " 070: " << timer.elapsed() << "ms"; timer.restart();

   //qDebug() << Q_FUNC_INFO << " - 101 " << (QTime::currentTime()).toString("HH:mm:ss");

    splash.showMessage ("Creating the Data Base...");
    QApplication::processEvents();
    DataProxy_SQLite dataProxy (Q_FUNC_INFO, version);
    QApplication::processEvents();

   //qDebug() << Q_FUNC_INFO << " 071: " << timer.elapsed() << "ms"; timer.restart();
    World world(&dataProxy, Q_FUNC_INFO);
   //qDebug() << Q_FUNC_INFO << " 072: " << timer.elapsed() << "ms"; timer.restart();
    dataProxy.setPKGVersion(pkgVersion);

    splash.showMessage("Creating window...");
    QApplication::processEvents();
   //qDebug() << Q_FUNC_INFO << " 080: " << timer.elapsed() << "ms"; timer.restart();

    MainWindow mw(&dataProxy, &world);
    splash.showMessage ("Initializing window...");
    QApplication::processEvents();
   //qDebug() << Q_FUNC_INFO << " 081: " << timer.elapsed() << "ms"; timer.restart();

    //qDebug() << Q_FUNC_INFO << " - 104 " << (QTime::currentTime()).toString("HH:mm:ss");
    mw.init();

   //qDebug() << Q_FUNC_INFO << " 082: " << timer.elapsed() << "ms"; timer.restart();
    //splash.showMessage("Checking for new versions...");
    //mw.checkIfNewVersion();
    //qDebug() << Q_FUNC_INFO << " 083: " << timer.elapsed() << "ms"; timer.restart();
    //splash.showMessage ("Checking if backup is needed...");
    //qDebug() << Q_FUNC_INFO << " 084: " << timer.elapsed() << "ms"; timer.restart();
    //mw.recommendBackupIfNeeded();
   //qDebug() << Q_FUNC_INFO << " 085: " << timer.elapsed() << "ms"; timer.restart();
    splash.showMessage ("Showing window...");
    QApplication::processEvents();
    //qDebug() << Q_FUNC_INFO << " - 110 " << (QTime::currentTime()).toString("HH:mm:ss");
    mw.show();
    //qDebug() << Q_FUNC_INFO << " 086: " << timer.elapsed() << "ms"; timer.restart();
    splash.finish(&mw);

    return app.exec();
}
