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
   // bandsWidget = new QWidget;

//TODO: To be removed when the defaultDir is saved in the config file
#ifdef Q_OS_WIN
    //qDebug() << "WINDOWS DETECTED!"  << endl;
    kontestDir = QDir::homePath()+"/klog";  // We create the \klog for the logs and data
    defaultFileName = kontestDir+"/klog.adi";
#else
    //qDebug() << "NO WINDOWS DETECTED!"  << endl;
    kontestDir = QDir::homePath()+"/.klog";  // We create the ~/.klog for the logs and data
    defaultFileName = kontestDir+"/klog.adi";
#endif


    imperialCheckBox = new QCheckBox(tr("&Imperial system"), this);
    dbInMemory = new QCheckBox(tr("DB in &Memory"), this);
    realTimeCheckbox = new QCheckBox(tr("&Log in real time"), this);
    UTCCheckbox = new QCheckBox(tr("&Time in UTC"), this);
    alwaysADIFCheckBox = new QCheckBox(tr("&Save ADIF on exit"), this);
    useDefaultName = new QCheckBox(tr("Use this &default filename"), this);
    sendQSLWhenRecCheckBox = new QCheckBox(tr("Mark &QSO to send QSL when QSL is received"), this);
    sendQSLWhenRecCheckBox->setToolTip(tr("QSOs will be marked to be pending to send the QSL if the DX QSL is received and you have not sent your."));

    showStationCallWhenSearchCheckBox = new QCheckBox(tr("Show the Station &Callsign used in the search box"), this);
    showStationCallWhenSearchCheckBox->setToolTip(tr("The search box will show also the callsign on the air to do the QSO."));


    keepMyDataCheckBox = new QCheckBox(tr("&Reset to My Data for all QSO"), this);
    keepMyDataCheckBox->setToolTip(tr("All the data from the My Data tab will be used or data from the previous QSO will be maintained."));


    imperialCheckBox ->setToolTip(tr("Check it for Imperial system (Miles instead of Kilometres)."));
    dbInMemory->setToolTip(tr("Working in memory (vs in file) is much quicker but you will need to save the ADIF file each time you exit KLog."));
    realTimeCheckbox->setToolTip(tr("Select to use real time."));
    UTCCheckbox->setToolTip(tr("Select to use UTC time."));
    alwaysADIFCheckBox->setToolTip(tr("Select if you want to save to ADIF on exit."));
    useDefaultName->setToolTip(tr("Select to use the following name for the logfile and not being asked for it anymore."));

    defaultFileNameLineEdit = new QLineEdit;
    defaultFileNameLineEdit->setToolTip(tr("This is the default file where ADIF will be saved."));
    defaultFileNameLineEdit->setReadOnly(true);
    defaultFileNameLineEdit->setText(defaultFileName);
    defaultFileNameLineEdit->setEnabled(false);

    useDefaultName->setChecked(true);
    alwaysADIFCheckBox->setChecked(true);
    showStationCallWhenSearchCheckBox->setChecked(true);
    keepMyDataCheckBox->setChecked(true);

    fileNameButton = new QPushButton;
    fileNameButton->setToolTip(tr("Click to change the default ADIF file"));
    //TODO: Add an icon "open" to this pushbutton

    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->addWidget(useDefaultName);
    fileLayout->addWidget(defaultFileNameLineEdit);
    fileLayout->addWidget(fileNameButton);
    defaultFileNameLineEdit->setEnabled(true);
    fileNameButton->setEnabled(true);

    UTCCheckbox->setChecked(true);
    realTimeCheckbox->setChecked(true);
    //showStationCallWhenSearchCheckBox->setChecked(true);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(UTCCheckbox);
    timeLayout->addWidget(realTimeCheckbox);

    //QHBoxLayout *fileOptLayout = new QHBoxLayout;
    //fileOptLayout->addWidget(useDefaultName);
    //fileOptLayout->addWidget(alwaysADIFCheckBox);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dbInMemory);
    mainLayout->addLayout(fileLayout);
    mainLayout->addWidget(alwaysADIFCheckBox);
    //mainLayout->addWidget(useDefaultName);
    //mainLayout->addLayout(fileOptLayout);
    //mainLayout->addWidget(realTimeCheckbox);
    //mainLayout->addWidget(UTCCheckbox);
    mainLayout->addLayout(timeLayout);
    mainLayout->addWidget(imperialCheckBox);
    mainLayout->addWidget(keepMyDataCheckBox);
    mainLayout->addWidget(sendQSLWhenRecCheckBox);
    mainLayout->addWidget(showStationCallWhenSearchCheckBox);


    setLayout(mainLayout);

    connect(fileNameButton, SIGNAL(clicked () ), this, SLOT(slotOpenFileButtonClicked() ) );
    connect(useDefaultName, SIGNAL(stateChanged (int) ), this, SLOT(slotUseDefaultButtonStateChanged(int) ) );
}

SetupPageMisc::~SetupPageMisc(){
    //qDebug() << "SetupPageMisc::~SetupPageMisc" << endl;
}

void SetupPageMisc::createActions(){
//void	itemDoubleClicked ( QListWidgetItem * item )


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
    defaultFileName = t;
    defaultFileNameLineEdit->setText(defaultFileName);
}

void SetupPageMisc::slotOpenFileButtonClicked()
{
    defaultFileName = QFileDialog::getOpenFileName(this, tr("Open File"), kontestDir, tr("ADIF (*.adi)"));
    defaultFileNameLineEdit->setText(defaultFileName);

}

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
        fileNameButton->setEnabled(true);

    }
    else
    {
        defaultFileNameLineEdit->setEnabled(false);
        fileNameButton->setEnabled(false);
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
