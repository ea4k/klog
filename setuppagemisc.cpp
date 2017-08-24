/***************************************************************************
                          setuppagemisc.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
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

//#include <QDebug>

#include "setuppagemisc.h"
//TODO: The defaultFileName QString can be removed and used the content of defaultFileNameLineEdit
//      instead. It is easy to change but the code would not be so clear... Think about this. :-)

SetupPageMisc::SetupPageMisc(QWidget *parent) : QWidget(parent){
//qDebug() << "SetupPageMisc::SetupPageMisc" << endl;

    util = new Utilities;

    imperialCheckBox = new QCheckBox(tr("&Imperial system"), this);
    realTimeCheckbox = new QCheckBox(tr("&Log in real time"), this);
    UTCCheckbox = new QCheckBox(tr("&Time in UTC"), this);
    alwaysADIFCheckBox = new QCheckBox(tr("&Save ADIF on exit"), this);
    useDefaultName = new QCheckBox(tr("Use this &default filename"), this);
    sendQSLWhenRecCheckBox = new QCheckBox(tr("Mark &QSO to send QSL when QSL is received"), this);
    completeWithPreviousCheckBox = new QCheckBox(tr("Complete QSO with previous data"));
    showStationCallWhenSearchCheckBox = new QCheckBox(tr("Show the Station &Callsign used in the search box"), this);  
    keepMyDataCheckBox = new QCheckBox(tr("&Reset to My Data for all QSOs"), this);
    checkNewVersionCheckBox = new QCheckBox(tr("&Check for new versions automatically"), this);
    provideCallCheckBox = new QCheckBox(tr("&Provide Info for statistics"), this);

    defaultFileNameLineEdit = new QLineEdit;
    dbPathLineEdit = new QLineEdit;

    fileNameButton = new QPushButton (tr("Browse"));
    dbPushButton = new QPushButton (tr("Browse"));
    moveDBPushButton = new QPushButton(tr("Move DB"));

    dbPathApplied = true;

    createUI();
    createActions();

    if (areDBPathChangesApplied())
    {
        moveDBPushButton->setEnabled(false);
    }
    else
    {
        moveDBPushButton->setEnabled(true);
    }
}

SetupPageMisc::~SetupPageMisc(){
    //qDebug() << "SetupPageMisc::~SetupPageMisc" << endl;
}

void SetupPageMisc::createUI()
{

    palWrong.setColor(QPalette::Text, Qt::red);
    palRight.setColor(QPalette::Text, Qt::black);

    //TODO: To be removed when the defaultDir is saved in the config file
    #ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!"  << endl;
    klogDir = util->getHomeDir();  // We create the \klog for the logs and data
    defaultFileName = klogDir+"/klog.adi";
    #else
    //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    klogDir = util->getHomeDir();  // We create the ~/.klog for the logs and data
    defaultFileName = klogDir+"/klog.adi";
    #endif
    dbDirNew = klogDir;     // The new path where the DB is to be moved
    dbDirCurrent = dbDirNew;       // The path where the DB is hosted

    defaultFileNameLineEdit->setReadOnly(false);
    defaultFileNameLineEdit->setText(defaultFileName);
    defaultFileNameLineEdit->setEnabled(false);

    dbPathLineEdit->setReadOnly(false);
    dbPathLineEdit->setText(dbDirCurrent);
    dbPathLineEdit->setEnabled(true);

    useDefaultName->setChecked(true);
    alwaysADIFCheckBox->setChecked(true);
    showStationCallWhenSearchCheckBox->setChecked(true);
    keepMyDataCheckBox->setChecked(true);
    completeWithPreviousCheckBox->setChecked(false);

    sendQSLWhenRecCheckBox->setToolTip(tr("QSOs will be marked as pending to send a QSL if you receive the DX QSL and have not sent yours."));
    showStationCallWhenSearchCheckBox->setToolTip(tr("The search box will show also the callsign on the air to do the QSO."));
    keepMyDataCheckBox->setToolTip(tr("All the data from the My Data tab will be used or data from the previous QSO will be maintained."));
    checkNewVersionCheckBox->setToolTip(tr("Check if there is a new release of KLog available every time you start KLog."));
    provideCallCheckBox->setToolTip(tr("If new version checking is selected, KLog will send the developer your callsign, KLog version & Operating system to help in improving KLog."));
    imperialCheckBox ->setToolTip(tr("Check it for Imperial system (Miles instead of Kilometres)."));
    realTimeCheckbox->setToolTip(tr("Select to use real time."));
    UTCCheckbox->setToolTip(tr("Select to use UTC time."));
    alwaysADIFCheckBox->setToolTip(tr("Select if you want to save to ADIF on exit."));    
    useDefaultName->setToolTip(tr("Select to use the following name for the logfile without being asked for it again."));
    completeWithPreviousCheckBox->setToolTip(tr("Complete the current QSO with previous QSO data."));
    defaultFileNameLineEdit->setToolTip(tr("This is the default file where ADIF data will be saved."));
    dbPathLineEdit->setToolTip(tr("This is the directory where the database (logbook.dat) will be saved."));
    fileNameButton->setToolTip(tr("Click to change the default ADIF file."));
    dbPushButton->setToolTip(tr("Click to change the path of the database."));
    moveDBPushButton->setToolTip(tr("Click to move the DB to the new directory."));

    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->addWidget(useDefaultName);
    fileLayout->addWidget(defaultFileNameLineEdit);
    fileLayout->addWidget(fileNameButton);
    defaultFileNameLineEdit->setEnabled(true);
    fileNameButton->setEnabled(true);


    QHBoxLayout *dbLayout = new QHBoxLayout;

    dbLayout->addWidget(dbPathLineEdit);
    dbLayout->addWidget(dbPushButton);
    dbLayout->addWidget(moveDBPushButton);

    UTCCheckbox->setChecked(true);
    realTimeCheckbox->setChecked(true);

    QGridLayout *mainLayou1 = new QGridLayout;
    mainLayou1->addLayout(fileLayout, 0, 0, 1, -1);
    mainLayou1->addLayout(dbLayout, 1, 0, 1, -1);
    mainLayou1->addWidget(alwaysADIFCheckBox, 2, 0, 1, 1);
    mainLayou1->addWidget(UTCCheckbox, 3, 0, 1, 1);
    mainLayou1->addWidget(realTimeCheckbox, 3, 1, 1, 1);
    mainLayou1->addWidget(imperialCheckBox, 4, 0, 1, 1);
    mainLayou1->addWidget(keepMyDataCheckBox, 5, 0, 1, 1);
    mainLayou1->addWidget(completeWithPreviousCheckBox, 5, 1, 1, 1);
    mainLayou1->addWidget(sendQSLWhenRecCheckBox,6, 0, 1, 1);
    mainLayou1->addWidget(showStationCallWhenSearchCheckBox, 6, 1, 1, 1);
    mainLayou1->addWidget(checkNewVersionCheckBox, 7, 0, 1, 1);
    mainLayou1->addWidget(provideCallCheckBox, 7, 1, 1, 1);

    setLayout(mainLayou1);


}

void SetupPageMisc::createActions(){
//void	itemDoubleClicked ( QListWidgetItem * item )

    connect(fileNameButton, SIGNAL(clicked () ), this, SLOT(slotOpenFileButtonClicked() ) );
    connect(useDefaultName, SIGNAL(stateChanged (int) ), this, SLOT(slotUseDefaultButtonStateChanged(int) ) );
    connect(defaultFileNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotDefaultFileNameLineEditChanged() ) );
    connect(checkNewVersionCheckBox, SIGNAL(clicked () ), this, SLOT(slotcheckNewVersionCheckBoxClicked() ) );
    connect(dbPushButton, SIGNAL(clicked () ), this, SLOT(slotDBButtonClicked() ) );
    connect(dbPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotDBLineEditChanged() ) );
    connect(moveDBPushButton, SIGNAL(clicked () ), this, SLOT(slotMoveDBButtonClicked() ) );

}

QString SetupPageMisc::getRealTime(){

    if (realTimeCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageMisc::setRealTime(const QString t){
    //QString st = t;
    if ( (t.toUpper()) == "FALSE")
    {
        realTimeCheckbox->setChecked(false);
    }
    else
    {
        realTimeCheckbox->setChecked(true);
    }

}

QString SetupPageMisc::getUTCTime(){

    if (UTCCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}


void SetupPageMisc::setUTCTime(const QString t){
    if ( (t.toUpper()) == "FALSE")
    {
        UTCCheckbox->setChecked(false);
    }
    else
    {
        UTCCheckbox->setChecked(true);
    }
}


QString SetupPageMisc::getAlwaysADIF()
{
    if (alwaysADIFCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}
void SetupPageMisc::setAlwaysADIF(const QString t)
{ // Defaul value is false
    if ( (t.toUpper()) == "TRUE")
    {
        alwaysADIFCheckBox->setChecked(true);
    }
    else
    {
        alwaysADIFCheckBox->setChecked(false);
    }

}

QString SetupPageMisc::getDefaultFileName()
{
    return defaultFileName;
}
void SetupPageMisc::setDefaultFileName(const QString t)
{
    //qDebug() << "SetupPageMisc::setDefaultFileName: " << t << endl;
    defaultFileName = t;
    defaultFileNameLineEdit->setText(defaultFileName);
}

void SetupPageMisc::slotOpenFileButtonClicked()
{
    defaultFileName = QFileDialog::getOpenFileName(this, tr("Open File"), klogDir, tr("ADIF (*.adi)"));
    defaultFileNameLineEdit->setText(defaultFileName);
}

 void SetupPageMisc::slotDefaultFileNameLineEditChanged()
 {
     //setDefaultFileName();
     defaultFileName = defaultFileNameLineEdit->text();
 }


/*
QString SetupPageMisc::getInMemory()
{
    if (dbInMemory->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}


void SetupPageMisc::setInMemory(const QString t)
{

    if ( (t.toUpper()) == "FALSE")
    {
        dbInMemory->setChecked(false);
    }
    else
    {
        dbInMemory->setChecked(true);
    }
}
*/

QString SetupPageMisc::getUseDefaultName()
{
    if (useDefaultName->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

void SetupPageMisc::setUseDefaultName(const QString t)
{

    if ( (t.toUpper()) == "FALSE")
    {
        useDefaultName->setChecked(false);
    }
    else
    {
        useDefaultName->setChecked(true);
    }
}

QString SetupPageMisc::getImperial()
{
    if (imperialCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

void SetupPageMisc::setImperial(const QString t)
{

    if ( (t.toUpper()) == "FALSE")
    {
        imperialCheckBox->setChecked(false);
    }
    else
    {
        imperialCheckBox->setChecked(true);
    }
}


void SetupPageMisc::slotUseDefaultButtonStateChanged(int state)
{
    //qDebug() << "SetupPageMisc::slotUseDefaultButtonStateChanged" << endl;

    if (state)
    {
        defaultFileNameLineEdit->setEnabled(true);
        moveDBPushButton->setEnabled(true);

    }
    else
    {
        defaultFileNameLineEdit->setEnabled(false);
        moveDBPushButton->setEnabled(false);
    }
}

QString SetupPageMisc::getSendQSLWhenRec(){

    if (sendQSLWhenRecCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}


void SetupPageMisc::setSendQSLWhenRec(const QString t){
    if ( (t.toUpper()) == "FALSE")
    {
        sendQSLWhenRecCheckBox->setChecked(false);
    }
    else
    {
        sendQSLWhenRecCheckBox->setChecked(true);
    }
}

QString SetupPageMisc::getShowStationCallSignInSearch()
{

    if (showStationCallWhenSearchCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

void SetupPageMisc::setShowStationCallSignInSearch(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showStationCallWhenSearchCheckBox->setChecked(false);
    }
    else
    {
        showStationCallWhenSearchCheckBox->setChecked(true);
    }

}


QString SetupPageMisc::getKeepMyData()
{

    if (keepMyDataCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

void SetupPageMisc::setKeepMyData(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        keepMyDataCheckBox->setChecked(false);
    }
    else
    {
        keepMyDataCheckBox->setChecked(true);
    }

}

QString SetupPageMisc::getCompleteWithPrevious()
{
    if (completeWithPreviousCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

void SetupPageMisc::setCompleteWithPrevious(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        completeWithPreviousCheckBox->setChecked(false);
    }
    else
    {
        completeWithPreviousCheckBox->setChecked(true);
    }
}

void SetupPageMisc::slotcheckNewVersionCheckBoxClicked()
{
    if (checkNewVersionCheckBox->isChecked())
    {
        provideCallCheckBox->setEnabled(true);
    }
    else
    {
        provideCallCheckBox->setEnabled(false);
        provideCallCheckBox->setChecked(false);
    }
}

QString SetupPageMisc::getCheckNewVersions()
{
    if (checkNewVersionCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageMisc::setCheckNewVersions(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        checkNewVersionCheckBox->setChecked(false);
    }
    else
    {
        checkNewVersionCheckBox->setChecked(true);
    }
}

QString SetupPageMisc::getReportInfo()
{
    if (checkNewVersionCheckBox->isChecked())
    {
        if (provideCallCheckBox->isChecked())
        {
            return "True";
        }
        else
        {
            return "False";
        }
    }
    else
    {
        return "False";
    }
}

void SetupPageMisc::setReportInfo(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        provideCallCheckBox->setChecked(false);
    }
    else
    {
        provideCallCheckBox->setChecked(true);
    }
}

QString SetupPageMisc::getDefaultDBPath()
{    
    return dbDirCurrent;
}


void SetupPageMisc::setUseDefaultDBPath(const QString t)
{
    dbDirCurrent = t;
    dbPathLineEdit->setText(dbDirCurrent);

}

void SetupPageMisc::slotDBButtonClicked()
{
//    QString dbDirBack = dbDir;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    dbDirCurrent,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir.length()>=1)
    {
        dbDirNew = dir;
    }

    dbPathLineEdit->setText(dbDirNew);
    if (dbDirCurrent == dbDirNew)
    {
        dbPathApplied = true;
    }
    else
    {
        dbPathApplied = false;
        moveDBPushButton->setEnabled(true);
    }
    //setUseDefaultDBPath(dbDirNew);

}

void SetupPageMisc::slotDBLineEditChanged()
{
    //qDebug() << "SetupPageMisc::slotDBLineEditChanged: " << dbPathLineEdit->text() << endl;
    QString aux;
    aux = dbPathLineEdit->text();

   if ( QFile::exists(aux) )
   {
       dbPathLineEdit->setToolTip(tr("This is the directory where DB (logbook.dat) will be saved."));
       dbPathLineEdit->setPalette(palRight);
       dbDirNew = dbPathLineEdit->text();
   }
   else
   {
        dbPathLineEdit->setToolTip(tr("Please specify an existing directory where the database (logbook.dat) will be saved."));
        dbPathLineEdit->setPalette(palWrong);
   }

   if (dbDirCurrent == dbDirNew)
   {
       dbPathApplied = true;
       moveDBPushButton->setEnabled(false);
   }
   else
   {
       dbPathApplied = false;
       moveDBPushButton->setEnabled(true);
   }
}

void SetupPageMisc::slotMoveDBButtonClicked()
{
    QString source = dbDirCurrent + "/logbook.dat";
    QString target = dbDirNew + "/logbook.dat";
    QMessageBox msgBox;

    //qDebug() << "SetupPageMisc::slotMoveDBButtonClicked (source): " << source << endl;
    //qDebug() << "SetupPageMisc::slotMoveDBButtonClicked (target): " << target << endl;
    if ( QFile::exists(dbDirNew) )
    {
        //dbDirCurrent
        //dbDir
        if (QFile::copy(source, target))
        {

            dbDirCurrent = dbDirNew;
            if (QFile::remove(source))
            {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("File moved"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                dbPathApplied = true;

            }
            else
            {
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("File copied"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                dbPathApplied = true;
            }
        }
        else
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("File NOT copied"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            dbPathApplied = false;
            moveDBPushButton->setEnabled(true);
        }


    }
    else
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("The target directory does not exist. Please select an existing directory."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        dbPathApplied = false;
        moveDBPushButton->setEnabled(true);
    }
}

bool SetupPageMisc::areDBPathChangesApplied()
{
    return dbPathApplied;
}
