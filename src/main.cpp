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

//#include <QDebug>


#include "startwizard.h"
#include "mainwindow.h"
#include "utilities.h"


int main(int argc, char *argv[])
{
    //qDebug() << "KLog Main: Start! " << endl;
    //qDebug() << "KLog Main: " << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString() << endl;
    QT_REQUIRE_VERSION(argc, argv, "5.11")
    QDir d1 = QDir();
    QString version = "1.5.1";
    //qDebug() << "KLog Main STARTED: " << version << endl;
    Utilities util = Utilities();
    QStringList arguments;
    QTextStream cout(stdout);
    QCoreApplication::setOrganizationName("EA4K");
    QCoreApplication::setOrganizationDomain("klog.xyz");
    QCoreApplication::setApplicationName("KLog");
    QApplication app(argc, argv);

    QString iconSt;
    iconSt = ":/img/klog.ico";
    QIcon KLogIcon(iconSt);
    QApplication::setWindowIcon(KLogIcon);

    //QApplication app(argc, argv);
    app.setApplicationName(QString("KLog"));
    app.setApplicationVersion(QString(version));
    //qDebug() << "KLog Main: -10 " << endl;
    // Now we check if the user is executing from the command line
    arguments.clear();
    arguments << app.arguments();
    if (arguments.length()>1)
    {
        if (arguments.contains("-h"))
        {
            //cout << "Usage: klog [OPTION]... [FILE]..." << endl;
            cout << "Usage: klog [OPTION]..." << endl;
            cout << "Options:" << endl;
            cout << "     -?           Display this help" << endl;
            cout << "     -h           Display this help" << endl;
            cout << "     -v           Display program version" << endl;
            //cout << "     -e <file>    Export Adif file <file>" << endl;
        }
        else if (arguments.contains("-?"))
        {
            cout << "Usage: klog [OPTION]..." << endl;
            cout << "Options:" << endl;
            cout << "     -?           Display this help" << endl;
            cout << "     -h           Display this help" << endl;
            cout << "     -v           Display program version" << endl;
        }
        else if (arguments.contains("-v"))
        {
            cout << "Version: KLog-" << app.applicationVersion() << endl;
        }
        else
        {
            cout << "Usage: klog [OPTION]..." << endl;
            cout << "Options:" << endl;
            cout << "     -?           Display this help" << endl;
            cout << "     -h           Display this help" << endl;
            cout << "     -v           Display program version" << endl;
        }
        app.quit();
        return 0;
    }
    //qDebug() << "KLog Main: Start of translation activities: "<< (QTime::currentTime()).toString("HH:mm:ss") << endl;
    //qDebug() << "KLog Main: Detected language: " << (QLocale::system().name()).left(2) << ".qm" << endl;
    // Translations begin
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    QTranslator myappTranslator;

    bool missingTranslation = false;
    //QString msgOSFilePath = QString();        // The OS depending part of the message to be printed if no translation is found.

    #if defined(Q_OS_WIN)
        //qDebug() << "KLog WIN " << endl;
        //qDebug() << "KLog Main: -20 - WIN" << endl;
        if (QFile::exists(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm") )
        {
            myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm");
        }
        else if (QFile::exists(QDir::homePath()+"/klog/klog_" + (QLocale::system().name()).left(2)+ ".qm") )
        {
            myappTranslator.load(QDir::homePath()+"/klog/klog_" + (QLocale::system().name()));
        }
        else if (((QLocale::system().name()).left(2)) == "en")
        { // If language is English, it will execute without showing message
        }
        else
        {
            missingTranslation = true;
            //msgOSFilePath = QCoreApplication::applicationDirPath() + "/translations/" ;
        }
      //qDebug() << "KLog Main: -20 - end WIN " << endl;
    #elif defined(Q_OS_OSX)
    //qDebug() << "KLog OSX " << endl;

        if (QFile::exists(QCoreApplication::applicationDirPath() + "/translations/klog_" +  (QLocale::system().name()).left(2) + ".qm") )
        {
            myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm");
        }
        else if (((QLocale::system().name()).left(2)) == "en")
        { // If language is English, it will execute without showing message

        }
        else
        {
            missingTranslation = true;
            //msgOSFilePath = QCoreApplication::applicationDirPath() + "/translations/" ;
        }
    #else
           //qDebug() << "KLog OTHER OS: " << (QLocale::system()).name() << endl;
        if (QFile::exists("klog_" + (QLocale::system().name()).left(2) + ".qm") )
        {
            myappTranslator.load("klog_" + (QLocale::system().name()).left(2));
        }
        else if (QFile::exists("/usr/share/klog/translations/klog_" + (QLocale::system().name()).left(2) + ".qm") )
        {
               //qDebug() << "KLog OTHER -2: " << "/usr/share/klog/klog_" + (QLocale::system().name()).left(2) << endl;
            myappTranslator.load("/usr/share/klog/translations/klog_" + (QLocale::system().name()));
        }
        else if (QFile::exists(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm"))
        {
            //qDebug() << "KLog OTHER -3: " << QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) << endl;
            myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()));
        }

        else if (((QLocale::system().name()).left(2)) == "en")
        { // If language is English, it will execute without showing message

        }
        else
        {
            missingTranslation = true;
            //sgOSFilePath = QCoreApplication::applicationDirPath() + "/translations/" ;
        }

    #endif
    //qDebug() << "KLog Main: -40 " << endl;
    if (missingTranslation)
        {
            //qDebug() << "KLog Main: Translation missing! " << endl;
            QMessageBox msgBox;
            QString urlTranslate = QString();
            urlTranslate = "<p><a href=\"https://translate.google.com/?sl=auto&tl=auto#en/auto/No%20translation%20files%20for%20your%20language%20have%20been%20found%20so%20KLog%20will%20be%20shown%20in%20English.%0A%0AIf%20you%20have%20the%20klog_en.qm%20file%20for%20your%20language%2C%20you%20can%20copy%20it%20in%20the%20%2Fhome%2Fdevel%2F.klog%2F%20folder%20and%20restart%20KLog%20again.%0A%0A%20If%20you%20want%20to%20help%20to%20translate%20KLog%20into%20your%20language%2C%20please%20contact%20the%20author.\">TRANSLATE</a></p>";

            QString msg = QString();

            msgBox.setWindowTitle("KLog");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setTextFormat(Qt::RichText);
            QString language = (QLocale::system().name()).left(2);

            msg = QString("No translation files for your language have been found so KLog will be shown in English.") + "<p>" +
                  QString("If you have the klog_%1.qm file for your language, you can copy it into the %2/translations/ folder and restart KLog.</p><p>If you want to help to translate KLog into your language, please contact the author.").arg(language).arg(QCoreApplication::applicationDirPath()) +
                  "</p>" + urlTranslate;

            msgBox.setText(msg);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();

        }

    //qDebug() << "KLog Main-1" << (QTime::currentTime()).toString("HH:mm:ss")  << endl;

    app.installTranslator(&myappTranslator);
    //qDebug() << "KLog Main: End of translation activities: "<< (QTime::currentTime()).toString("HH:mm:ss") << endl;
    // Traslations end

    QString configFileName, klogDir;

    klogDir = util.getHomeDir();
    configFileName = util.getCfgFile();

    //qDebug() << "KLog Main-10" << endl;

    //qDebug() << "KLog Main: Setting klog dir: " << (QTime::currentTime()).toString("HH:mm:ss")<< endl;;
    if (!QDir::setCurrent (klogDir) )
    {
        //qDebug() << "MAIN:  KLogDir does not exist.... creating " << endl;
        if (d1.mkdir(klogDir))
        {
            if (QDir::setCurrent (klogDir) )
            {
                    //qDebug() << "MAIN:  KLogDir has just been created and pointed " << endl;
            }
            else
            {
                    //qDebug() << "MAIN:  KLogDir has just been created and pointed FAILED! " << endl;
            }
        }
        else
        {
                //qDebug() << "MAIN:  KLogDir can not be created?? " << endl;
        }
    }
    else
    {
        //qDebug() << "MAIN:  KLogDir already existed!! " << endl;
    }
    //qDebug() << "KLog Main: Setting klog dir - finished: " << (QTime::currentTime()).toString("HH:mm:ss") << endl;

    //qDebug() << "KLog Main: Setting config file: " << (QTime::currentTime()).toString("HH:mm:ss")  << endl;
    QPixmap pixmap(":img/klog_512x512.png");
    //qDebug() << "KLog Main-51" << (QTime::currentTime()).toString("HH:mm:ss") << endl;
    QSplashScreen splash(pixmap);
    if(!QFile::exists(configFileName))
    {
        //qDebug() << "MAIN:  Starting wizard... " << endl;

        StartWizard *wizard = new StartWizard(klogDir, version);
        wizard->setModal(true);
        int inMemory = wizard->exec();
        //qDebug() << "MAIN: Wizard inMemory: " << QString::number(inMemory) << endl;
        inMemory = 1;

        if (inMemory == 1)
        {
            //qDebug() << "MAIN: Wizard accepted " << QString::number(inMemory) << " ... Will run in Memory " << endl;
            MainWindow mw(klogDir, version);
            mw.init();
            splash.finish(&mw);
            //mw.checkIfNewVersion();
            //mw.recommendBackupIfNeeded();
            mw.show();
            return app.exec();
        }

        else
        {
               //qDebug() << "MAIN: Wizard cancelled " << QString::number(inMemory) << " ... should close " << endl;

            QMessageBox msgBox;
            msgBox.setText(QObject::tr("Install wizard was canceled before completing..."));
            msgBox.setInformativeText(QObject::tr("Do you want to remove the KLog dir from your disk?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();

            switch (ret)
            {
              case QMessageBox::Yes:
                if (QDir::setCurrent (QDir::homePath()) )
                {
                    if (d1.remove(klogDir))
                    {
                        QMessageBox msgBox;
                        msgBox.setText(QObject::tr("Your KLog dir has been removed") +"\n\n" + QObject::tr("Thank you for running KLog!"));
                        msgBox.exec();
                    }
                    else
                    {
                        QMessageBox msgBox;
                        msgBox.setText(QObject::tr("I could not remove your KLog dir. You should do it manually if you want it removed from your hard disk.") +"\n\n" + QObject::tr("Thank you for running KLog!"));
                        msgBox.exec();
                    }
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText(QObject::tr("Your KLog dir could not be removed. You should do it manually if you want it removed from your hard disk.") +"\n\n" + QObject::tr("Thank you for running KLog!"));
                    msgBox.exec();
                }
            break;
            case QMessageBox::No:
                QMessageBox msgBox;
                msgBox.setText(QObject::tr("Remember that your KLog dir is on your system...") + "\n\n" + QObject::tr("Thank you for running KLog!"));
                msgBox.exec();
            break;
            }
            return 0;
        }
    }

    else
    {
            //qDebug() << "Main: Start of DB Activities" << endl;
        DataBase *db = new DataBase(Q_FUNC_INFO, version, util.getKLogDBFile());
           //qDebug() << "Main: After Start of DB Activities" << endl;
        if (!db->createConnection(Q_FUNC_INFO))
        {
                //qDebug() << "Main: Conection not created" << endl;
            return -1; // Exits with an error; no DB has been created
        }
        else
        {
            //qDebug() << "Main: DB to be updated" << endl;
            db->updateIfNeeded(); // Check if we need to update the DB
            //qDebug() << "Main: DB Updated" << endl;
        }
        db->~DataBase();

        splash.show();
          //qDebug() << "KLog Main-100" << (QTime::currentTime()).toString("HH:mm:ss") << endl;
        MainWindow mw(klogDir, version);

        mw.init();
        splash.finish(&mw);
        mw.checkIfNewVersion();
        mw.recommendBackupIfNeeded();

            //qDebug() << "KLog Main-101" << (QTime::currentTime()).toString("HH:mm:ss") << endl;
        mw.show();
            //qDebug() << "KLog Main-101.5" << (QTime::currentTime()).toString("HH:mm:ss") << endl;

           //qDebug() << "KLog Main-102" << (QTime::currentTime()).toString("HH:mm:ss") << endl;

        return app.exec();
           //qDebug() << "KLog Main-103" << (QTime::currentTime()).toString("HH:mm:ss") << endl;
    }
       //qDebug() << "KLog Main-END: " << (QTime::currentTime()).toString("HH:mm:ss")  << endl;

    //return app.exec();
}


