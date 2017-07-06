 /* This file is part of KLog.                                               *
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QtWidgets>
#include <QtSql>
#include <QTranslator>
#include <cstdlib>
#include <QTextStream>

//#include <QDebug>


#include "startwizard.h"
#include "mainwindow.h"
#include "utilities.h"


int main(int argc, char *argv[])
{
    QString version = "0.9.2.7" ;
    QDir d1 = QDir();
    Utilities util = Utilities();
    QStringList arguments;
    QTextStream cout(stdout);

    QApplication app(argc, argv);

    QString iconSt;
    iconSt = ":/img/klog.ico";
    QIcon KLogIcon(iconSt);
    QApplication::setWindowIcon(KLogIcon);


    //QApplication app(argc, argv);
    app.setApplicationName(QString("KLog"));
    app.setApplicationVersion(QString(version));


    // Now we check if the user is executing from the command line
    arguments.clear();
    arguments << app.arguments();
    if (arguments.length()>1)
    {
        if (arguments.contains("-h"))
        {
            cout << "Usage: klog [OPTION]... [FILE]..." << endl;
            cout << "Options:" << endl;
            cout << "     -?           Display this help" << endl;
            cout << "     -h           Display this help" << endl;
            cout << "     -v           Display program version" << endl;
            //cout << "     -e <file>    Export Adif file <file>" << endl;
        }
        else if (arguments.contains("-?"))
        {
            cout << "Usage: klog [OPTION]... [FILE]..." << endl;
            cout << "Options:" << endl;
            cout << "     -?           Display this help" << endl;
            cout << "     -h           Display this help" << endl;
            cout << "     -v           Display program version" << endl;
        }
        /*
        else if (arguments.contains("-e"))
        {
            //qDebug() << "KLog::main: EXPORT ADIF";
        }
        else if (arguments.contains("-u"))
        {
            //qDebug() << "KLog::main: UPDATE CTY";
        }
        */
        else if (arguments.contains("-v"))
        {
            cout << "Version: KLog-" << app.applicationVersion() << endl;
        }
        else
        {
            cout << "Usage: klog [OPTION]... [FILE]..." << endl;
            cout << "Options:" << endl;
            cout << "     -?           Display this help" << endl;
            cout << "     -h           Display this help" << endl;
            cout << "     -v           Display program version" << endl;
        }

        app.quit();
        return 0;

    }

    //for (int i=0; i<(app.arguments().length()); i++)
    //{
    //  //qDebug() << "KLog::main: Arguments: " << arguments.at(i) << endl;
    //}


    // Translations begin
        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(),
                QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&qtTranslator);
        QTranslator myappTranslator;
#if defined(Q_OS_WIN)
        myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()).left(2) + ".qm");
       //qDebug() << "KLog WIN " << endl;
       //qDebug() << "KLog: " << QLocale::system().name() << endl;
        //qDebug() << "KLog trans: " <<  QLibraryInfo::location(QLibraryInfo::TranslationsPath) << endl;
        //qDebug() << "KLog currentPath: " <<  QDir::currentPath() << endl;
#elif defined(Q_OS_OSX)
        myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()));
        //qDebug() << "KLog OSX " << endl;
        //myappTranslator.load("translations/klog_" + (QLocale::system().name()));

        //qDebug() << "KLog: " << QLocale::system().name() << endl;
        //qDebug() << "KLog trans: " <<  QLibraryInfo::location(QLibraryInfo::TranslationsPath) << endl;
        //qDebug() << "KLog currentPath: " <<  QDir::currentPath() << endl;

#else
       //qDebug() << "KLog OTHER " << endl;
        if (QFile::exists("klog_" + (QLocale::system().name()).left(2) + ".qm") )
        {
           //qDebug() << "KLog OTHER -1: " << "klog_" + (QLocale::system().name()).left(2) << endl;
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
            //qDebug() << "KLog Long Language: " <<  (QLocale::system().name()) << endl;
            //qDebug() << "KLog Language: " <<  (QLocale::system().name()).left(2) << endl;
        }
        else
        {
            QMessageBox msgBox;
            QString urlTranslate = QString();
            urlTranslate = "<p><a href=\"https://translate.google.com/?sl=auto&tl=auto#en/auto/No%20translation%20files%20for%20your%20language%20have%20been%20found%20so%20KLog%20will%20be%20shown%20in%20English.%0A%0AIf%20you%20have%20the%20klog_en.qm%20file%20for%20your%20language%2C%20you%20can%20copy%20it%20in%20the%20%2Fhome%2Fdevel%2F.klog%2F%20folder%20and%20restart%20KLog%20again.%0A%0A%20If%20you%20want%20to%20help%20to%20translate%20KLog%20into%20your%20language%2C%20please%20contact%20the%20author.\">TRANSLATE</a></p>";
            QString msg = QString();
            msg = QString(QObject::tr("No translation files for your language have been found so KLog will be shown in English.\n\n")) + "<p>" +
                   QString(QObject::tr("If you have the klog_")) + (QLocale::system().name()).left(2) + QString(QObject::tr(".qm file for your language, you can copy it in the "));
            msgBox.setWindowTitle("KLog");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setTextFormat(Qt::RichText);
// The following URL links to google translator
//URL: https://translate.google.com/?sl=auto&tl=auto#en/auto/No%20translation%20files%20for%20your%20language%20have%20been%20found%20so%20KLog%20will%20be%20shown%20in%20English.%0A%0AIf%20you%20have%20the%20klog_en.qm%20file%20for%20your%20language%2C%20you%20can%20copy%20it%20in%20the%20%2Fhome%2Fdevel%2F.klog%2F%20folder%20and%20restart%20KLog%20again.%0A%0A%20If%20you%20want%20to%20help%20to%20translate%20KLog%20into%20your%20language%2C%20please%20contact%20the%20author.

            QString msg2;
            msg2 = QString();
            msg2 = QObject::tr("folder and restart KLog again.") + "</p><p>" + QObject::tr("If you want to help to translate KLog into your language, please contact the author.</p>") + urlTranslate;

            #ifdef Q_OS_WIN

            if (QFile::exists(QDir::homePath()+"/klog/klog_" + (QLocale::system().name()).left(2)+ ".qm") )
            {

               //qDebug() << "KLog OTHER -4: " << QDir::homePath()+"/klog/klog_" + (QLocale::system().name()).left(2) << endl;
                 myappTranslator.load(QDir::homePath()+"/klog/klog_" + (QLocale::system().name()));
            }
            else
            {

                msg = msg + QDir::homePath()+ "/klog/" + msg2;
                msgBox.setText(msg);                
                msgBox.setInformativeText("Do you want to remove the KLog dir from your disk?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
                msgBox.setDefaultButton(QMessageBox::Yes);
                msgBox.exec();
            }


         #else

           //qDebug() << "KLog OTHER -5: " << QDir::homePath()+"/.klog/klog_" + (QLocale::system().name()).left(2)<< endl;

            if (QFile::exists(QDir::homePath()+"/.klog/klog_" + (QLocale::system().name()).left(2)+ ".qm") )
            {
               //qDebug() << "KLog OTHER -4: " << QDir::homePath()+"/.klog/klog_" + (QLocale::system().name()).left(2) << endl;
                 myappTranslator.load(QDir::homePath()+"/.klog/klog_" + (QLocale::system().name()));
            }
            else
            {
                //msg = msg + QDir::homePath()+"/.klog/ " + QObject::tr("folder and restart KLog again.\n\n If you want to help to translate KLog into your language, please contact the author.")  + urlTranslate;
                msg = msg + QDir::homePath()+"/.klog/ " + msg2;
                msgBox.setText(msg);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
            }

         #endif

        }
#endif

    //qDebug() << "KLog Main-1" << endl;

    //myappTranslator.load("klog_" + (QLocale::system().name()).left(2));
    //qDebug() << "KLog locale: " << QLocale::system().name() << endl;
    //qDebug() << "KLog locale2: " << (QLocale().name()).left(2) << endl;
    //qDebug() << "KLog language: " << QLocale::system().language() << endl;
    //qDebug() << "KLog AppPath: " << QCoreApplication::applicationDirPath() << endl;

    app.installTranslator(&myappTranslator);
// Translations end

    QString configFileName, klogDir;
    int inMemory;
    //bool dbInMemory = true;

    klogDir = util.getHomeDir();
    configFileName = util.getCfgFile();

//#ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!"  << endl;
    //klogDir = QDir::homePath()+"/klog";  // We create the \klog for the logs and data
//    configFileName = klogDir+"/klogrc.cfg";
//#else
    //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    //klogDir = QDir::homePath()+"/.klog";  // We create the ~/.kontest for the logs and data
//    configFileName = klogDir+"/klogrc";
//#endif

   //qDebug() << "KLog Main-10" << endl;

    //if (!QDir::setCurrent (klogDir) )
    if (!QDir::setCurrent (klogDir) )
    {
        //qDebug() << "MAIN:  KLogDir does not exist.... creating " << endl;
        //QDir d1(klogDir);
        //d1.setCurrent()
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

    if(!QFile::exists(configFileName))
    {
        //qDebug() << "MAIN:  Starting wizard... " << endl;

        StartWizard *wizard = new StartWizard(klogDir, version);
        wizard->setModal(true);
        inMemory = wizard->exec();

        if (inMemory == 1)
        {
          //qDebug() << "MAIN: Wizard accepted " << QString::number(inMemory) << " ... Will run in Memory " << endl;
            MainWindow mw(klogDir, version);
            mw.show();
            return app.exec();
        }
        else if (inMemory == 2)
        {
           //qDebug() << "MAIN: Wizard accepted " << QString::number(inMemory) << " ... Will run in file " << endl;
            MainWindow mw(klogDir, version);
            mw.show();
            return app.exec();
        }
        else
        {
           //qDebug() << "MAIN: Wizard cancelled " << QString::number(inMemory) << " ... should close " << endl;

            QMessageBox msgBox;
            msgBox.setText(QObject::tr("Install wizard was cancelled before completing..."));
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
                        msgBox.setText(QObject::tr("Your KLog dir has been removed\n\nThank you for running KLog!"));
                        msgBox.exec();
                    }
                    else
                    {
                        QMessageBox msgBox;
                        msgBox.setText(QObject::tr("I could not remove your KLog dir. You should do it manually if you want it removed from your hard disk.\n\nThank you for running KLog!"));
                        msgBox.exec();
                    }
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText(QObject::tr("Your KLog dir could not be removed. You should do it manually if you want it removed from your hard disk.\n\nThank you for running KLog!"));
                    msgBox.exec();
                }
            break;
            case QMessageBox::No:
                QMessageBox msgBox;
                msgBox.setText(QObject::tr("Remember that your KLog dir is in your system...\n\nThank you for running KLog!"));
                msgBox.exec();
            break;
            }
            return 0;
        }
    }
    else
    {
       //qDebug() << "KLog Main-100" << endl;
        MainWindow mw(klogDir, version);
       //qDebug() << "KLog Main-101" << endl;
        mw.show();
       //qDebug() << "KLog Main-102" << endl;

        return app.exec();
       //qDebug() << "KLog Main-103" << endl;
    }
    //qDebug() << "KLog Main-END" << endl;
    //return app.exec();
}



