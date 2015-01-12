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

#include <QtGui>
#include <QtSql>
#include <QTranslator>
#include <cstdlib>
//#include <QDebug>

#include "startwizard.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QString version = "0.9.1";
    QDir d1 = QDir();

    QApplication app(argc, argv);
// Translations begin
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("klog_" + QLocale::system().name());
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
            msgBox.setText("Install wizard was cancelled before completing...");
            msgBox.setInformativeText("Do you want to remove the KLog dir from your disk?");
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
                        msgBox.setText("Your KLog dir has been removed\n\nThank you for running KLog!.");
                        msgBox.exec();
                    }
                    else
                    {
                        QMessageBox msgBox;
                        msgBox.setText("I could not remove your KLog dir. You should do it manually if you want it removed from your hard disk.\n\nThank you for running KLog!.");
                        msgBox.exec();
                    }
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText("Your KLog dir could not be removed. You should do it manually if you want it removed from your hard disk.\n\nThank you for running KLog!.");
                    msgBox.exec();
                }
            break;
            case QMessageBox::No:
                QMessageBox msgBox;
                msgBox.setText("Remember that your KLog dir is in your system...\n\nThank you for running KLog!.");
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

