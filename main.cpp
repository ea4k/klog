 /* This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include <QtWidgets>
#include <QtSql>
#include <QTranslator>
#include <cstdlib>
#include <QDebug>

#include "startwizard.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QString version = "0.9.2.4" ;
    QDir d1 = QDir();

    QApplication app(argc, argv);

    //QApplication app(argc, argv);
    app.setApplicationName(QString("KLog"));
    app.setApplicationVersion(QString(version));

    // Translations begin
        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(),
                QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&qtTranslator);
        QTranslator myappTranslator;
#if defined(Q_OS_WIN)
        myappTranslator.load(QCoreApplication::applicationDirPath() + "/translations/klog_" + (QLocale::system().name()));
       //qDebug() << "KLog WIN " << endl;
       ////qDebug() << "KLog: " << QLocale::system().name() << endl;
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
        else
        {
            QMessageBox msgBox;
            QString msg = QString();
            msg = QString(QObject::tr("No translation files for your language have been found so KLog will be shown in English.\n\n")) +
                   QString(QObject::tr("If you have the klog_")) + (QLocale::system().name()).left(2) + QString(QObject::tr(".qm file for your language, you can copy it in the "));

         #ifdef Q_OS_WIN
            if (QFile::exists(QDir::homePath()+"/klog/klog_" + (QLocale::system().name()).left(2)+ ".qm") )
            {

               //qDebug() << "KLog OTHER -4: " << QDir::homePath()+"/klog/klog_" + (QLocale::system().name()).left(2) << endl;
                 myappTranslator.load(QDir::homePath()+"/klog/klog_" + (QLocale::system().name()));
            }
            else
            {

                msg = msg + QDir::homePath()+ QObject::tr("/klog/ folder and restart KLog again.\n\n If you want to help to translate KLog into your language, please contact the author.");
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
                msg = msg + QDir::homePath()+"/.klog/ " + QObject::tr("folder and restart KLog again.\n\n If you want to help to translate KLog into your language, please contact the author.");
                msgBox.setText(msg);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
            }

         #endif

        }
#endif






    //myappTranslator.load("klog_" + (QLocale::system().name()).left(2));
    //qDebug() << "KLog locale: " << QLocale::system().name() << endl;
    //qDebug() << "KLog locale2: " << (QLocale().name()).left(2) << endl;
    //qDebug() << "KLog language: " << QLocale::system().language() << endl;
    //qDebug() << "KLog AppPath: " << QCoreApplication::applicationDirPath() << endl;

    app.installTranslator(&myappTranslator);
// Translations end

    QString configFileName, kontestDir;
    int inMemory;
    //bool dbInMemory = true;

#ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!"  << endl;
    kontestDir = QDir::homePath()+"/klog";  // We create the \klog for the logs and data
    configFileName = kontestDir+"/klogrc.cfg";
#else
    //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    kontestDir = QDir::homePath()+"/.klog";  // We create the ~/.kontest for the logs and data
    configFileName = kontestDir+"/klogrc";
#endif

    //if (!QDir::setCurrent (kontestDir) )
    if (!QDir::setCurrent (kontestDir) )
    {
        //qDebug() << "MAIN:  KLogDir does not exist.... creating " << endl;
        //QDir d1(kontestDir);
        //d1.setCurrent()
        if (d1.mkdir(kontestDir))
        {
            if (QDir::setCurrent (kontestDir) )
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

        StartWizard *wizard = new StartWizard(kontestDir, version);        
        wizard->setModal(true);
        inMemory = wizard->exec();

        if (inMemory == 1)
        {
          //qDebug() << "MAIN: Wizard accepted " << QString::number(inMemory) << " ... Will run in Memory " << endl;
            MainWindow mw(kontestDir, version);
            mw.show();
            return app.exec();
        }
        else if (inMemory == 2)
        {
           //qDebug() << "MAIN: Wizard accepted " << QString::number(inMemory) << " ... Will run in file " << endl;
            MainWindow mw(kontestDir, version);
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
                    if (d1.remove(kontestDir))
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
        MainWindow mw(kontestDir, version);
        mw.show();

        return app.exec();
    }

    //return app.exec();
}

