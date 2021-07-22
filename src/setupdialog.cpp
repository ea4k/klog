/***************************************************************************
                          setupdialog.cpp  -  description
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

#include "setupdialog.h"
#include "setuppages/setuppagemisc.h"
//#include <QDebug>

/*
This class calls all the othet "Setup..." to manage the configuration

*/

SetupDialog::SetupDialog(DataProxy_SQLite *dp, const QString &_configFile, const QString &_softwareVersion, const int _page, const bool _firstTime, QWidget *parent)
{
    //qDebug() << "SetupDialog::SetupDialog 2: " << _configFile << "/" << _softwareVersion << "/" << QString::number(_page) << util->boolToQString(_firstTime) << endl ;

    logSeverity = Info;
    constrid = 2;
    util = new Utilities;
    firstTime = _firstTime;
    latestBackup = QString();
    dataProxy = dp;
    configFileName = _configFile;
    version = _softwareVersion;
    pageRequested = _page;
    int logsPageTabN=-1;
    //qDebug() << "SetupDialog::SetupDialog 01" << Qt::endl;

    locator = new Locator();

    tabWidget = new QTabWidget;
    //qDebug() << "SetupDialog::SetupDialog 01.0" << Qt::endl;
    userDataPage = new SetupPageUserDataPage(dataProxy);
    //qDebug() << "SetupDialog::SetupDialog 01.10" << Qt::endl;
    bandModePage = new SetupPageBandMode(dataProxy, this);
    //qDebug() << "SetupDialog::SetupDialog 01.20" << Qt::endl;
    dxClusterPage = new SetupPageDxCluster(this);
    //qDebug() << "SetupDialog::SetupDialog 01.30" << Qt::endl;
    colorsPage = new SetupPageColors(this);
    //qDebug() << "SetupDialog::SetupDialog 01.40" << Qt::endl;
    miscPage = new SetupPageMisc(this);
    //qDebug() << "SetupDialog::SetupDialog 01.50" << Qt::endl;
    worldEditorPage = new SetupPageWorldEditor (dataProxy, this);
    //qDebug() << "SetupDialog::SetupDialog 01.60" << Qt::endl;
    logsPage = new SetupPageLogs(dataProxy, this);
    //qDebug() << "SetupDialog::SetupDialog 01.70" << Qt::endl;
    eLogPage = new SetupPageELog(this);
    //qDebug() << "SetupDialog::SetupDialog 01.80" << Qt::endl;
    UDPPage = new SetupPageUDP(this);
    //qDebug() << "SetupDialog::SetupDialog 01.90" << Qt::endl;
    satsPage = new SetupPageSats(dataProxy, this);
    //qDebug() << "SetupDialog::SetupDialog 01.100" << Qt::endl;
    hamlibPage = new SetupPageHamLib(dataProxy, this);
    //subdivisionsPage = new SetupPageSubdivisions(dataProxy, this);
    //regionalAwardsPage = new SetupPageRegionalAwards(dataProxy, this);
    //interfacesWindowsPage = new SetupPageInterfacesWindows(this);

    //qDebug() << "SetupDialog::SetupDialog 02" << Qt::endl;
    //tabWidget->addTab(subdivisionsPage, tr("Subdivisions"));
    tabWidget->addTab(userDataPage, tr("User data"));
    tabWidget->addTab(bandModePage, tr("Bands/Modes"));
    tabWidget->addTab(dxClusterPage, tr("D&X-Cluster"));
    tabWidget->addTab(colorsPage, tr("Colors"));
    tabWidget->addTab(miscPage, tr("Misc"));
    tabWidget->addTab(worldEditorPage, tr("World Editor"));
    logsPageTabN = tabWidget->addTab(logsPage, tr("Logs"));
    tabWidget->addTab(eLogPage, tr("eLog"));
    //tabWidget->addTab(clubLogPage, tr("ClubLog"));
    //tabWidget->addTab(lotwPage, tr("LoTW"));

    tabWidget->addTab(UDPPage, tr("WSJT-X"));
    tabWidget->addTab(satsPage , tr("Satellites"));
    tabWidget->addTab(hamlibPage, tr ("HamLib"));
    //tabWidget->addTab(interfacesWindowsPage, tr ("Interfaces"));

    //qDebug() << "SetupDialog::SetupDialog 03" << Qt::endl;

    QPushButton *closeButton = new QPushButton(tr("Cancel"));
    okButton = new QPushButton(tr("OK"));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(tabWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(buttonsLayout);

    //qDebug() << "SetupDialog::SetupDialog 04" << Qt::endl;

    setLayout(mainLayout);
    setWindowTitle(tr("Settings"));

    //qDebug() << "SetupDialog::SetupDialog 05" << Qt::endl;

    slotReadConfigData();
    //qDebug() << "SetupDialog::SetupDialog 05.1" << Qt::endl;

    if ((pageRequested==6) && (logsPageTabN>0))// The user is opening a new log
    {
         //qDebug() << "SetupDialog::SetupDialog 05.2" << Qt::endl;
        tabWidget->setCurrentIndex(logsPageTabN);
    }
    //qDebug() << "SetupDialog::SetupDialog 05.3" << Qt::endl;
    nolog = !(haveAtleastOneLog());

    connect(closeButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOkButtonClicked()));
    connectActions();

    //qDebug() << "SetupDialog::SetupDialog 2  - END" << Qt::endl;
}


SetupDialog::~SetupDialog()
{
       //qDebug() << "SetupDialog::~SetupDialog " << Qt::endl;
}

void SetupDialog::connectActions()
{
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    connect (logsPage, SIGNAL(newLogData(QStringList)), this, SLOT(slotAnalyzeNewLogData(QStringList)));
    connect(logsPage, SIGNAL(focusOK()), this, SLOT(slotFocusOK()) );
    connect (userDataPage, SIGNAL(stationCallSignal(QString)), this, SLOT(slotSetStationCallSign(QString)));
    connect (userDataPage, SIGNAL(operatorsSignal(QString)), this, SLOT(slotSetOperators(QString)));
    connect (userDataPage, SIGNAL(enterKey()), this, SLOT(slotOkButtonClicked()));
    //connect (lotwPage, SIGNAL(enterKey()), this, SLOT(slotOkButtonClicked()));
    connect (eLogPage, SIGNAL(enterKey()), this, SLOT(slotOkButtonClicked()));
    //connect (clubLogPage, SIGNAL(enterKey()), this, SLOT(slotOkButtonClicked()));
    connect (eLogPage, SIGNAL(qrzcomAuto(bool)), this, SLOT(slotQRZCOMAuto(bool)));

    emit debugLog (Q_FUNC_INFO, "END", logSeverity);

}

void SetupDialog::slotQRZCOMAuto(const bool _b)
{
    emit qrzcomAuto(_b);
}

void SetupDialog::setData(const QString &_configFile, const QString &_softwareVersion, const int _page, const bool _firstTime)
{
      //qDebug() << "SetupDialog::setData: " << "/" << _configFile << "/" << _softwareVersion << "/" << QString::number(_page) << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    nolog = !(haveAtleastOneLog());
    firstTime = _firstTime;
    if (firstTime)
    {
          //qDebug() << "SetupDialog::setData FIRST TIME! " << Qt::endl;
    }
    else
    {
          //qDebug() << "SetupDialog::setData NOT FIRST TIME! " << Qt::endl;
        miscPage->setUseDefaultDBPath(miscPage->getDefaultDBPath());
    }

    setConfigFile(_configFile);
    setSoftVersion(_softwareVersion);
    slotReadConfigData ();
    setPage(_page);
    //removeBandModeDuplicates();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "SetupDialog::setData - END" << Qt::endl;
}

void SetupDialog::setConfigFile(const QString &_configFile)
{
       //qDebug() << "SetupDialog::setConfigFile" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    configFileName = _configFile;
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::setSoftVersion(const QString &_softwareVersion)
{
       //qDebug() << "SetupDialog::setSoftVersion" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    version = _softwareVersion;
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}


void SetupDialog::setPage(const int _page)
{
       //qDebug() << "SetupDialog::setPage("<<QString::number(_page) << ")" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    pageRequested = _page;

    if ((pageRequested==6) && (logsPageTabN>0))// The user is opening a new log
    {
        tabWidget->setCurrentIndex(pageRequested);
    }
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::slotCancelButtonClicked()
{
      //qDebug() << "SetupDialog::slotCancelButtonClicked" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    if (firstTime || nolog)
    {
        if (nolog)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            msgBox.setText(tr("You need to enter at least one log in the Logs tab."));
            msgBox.setInformativeText(tr("Do you want to add one log in the Logs tab or exit KLog?\n(Click Yes to add a log or No to exit KLog)"));
            int ret = msgBox.exec();
            if (ret == QMessageBox::No)
            {
                emit debugLog (Q_FUNC_INFO, "END-1", logSeverity);
                emit exitSignal(2);
                return;
            }
            else
            {
                emit debugLog (Q_FUNC_INFO, "END-2", logSeverity);
                return;
            }
        }
    }
    QDialog::reject ();
    close();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::createIcons()
{
       //qDebug() << "SetupDialog::createIcons" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("User data"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *logsButton = new QListWidgetItem(contentsWidget);
    logsButton->setIcon(QIcon(":/images/config.png"));
    logsButton->setText(tr("Logs"));
    logsButton->setTextAlignment(Qt::AlignHCenter);
    logsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *bandsButton = new QListWidgetItem(contentsWidget);
    bandsButton->setIcon(QIcon(":/images/query.png"));
    bandsButton->setText(tr("Bands/Modes"));
    bandsButton->setTextAlignment(Qt::AlignHCenter);
    bandsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *dxClusterButton = new QListWidgetItem(contentsWidget);
    dxClusterButton->setIcon(QIcon(":/images/query.png"));
    dxClusterButton->setText(tr("DX-Cluster"));
    dxClusterButton->setTextAlignment(Qt::AlignHCenter);
    dxClusterButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *colorsButton  = new QListWidgetItem(contentsWidget);
    colorsButton->setIcon(QIcon(":/images/query.png"));
    colorsButton->setText(tr("Colors"));
    colorsButton->setTextAlignment(Qt::AlignHCenter);
    colorsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *miscButton  = new QListWidgetItem(contentsWidget);
    miscButton->setIcon(QIcon(":/images/query.png"));
    miscButton->setText(tr("Misc"));
    miscButton->setTextAlignment(Qt::AlignHCenter);
    miscButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *worldButton  = new QListWidgetItem(contentsWidget);
    worldButton->setIcon(QIcon(":/images/query.png"));
    worldButton->setText(tr("World"));
    worldButton->setTextAlignment(Qt::AlignHCenter);
    worldButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
       //qDebug() << "SetupDialog::changePage" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}



void SetupDialog::slotOkButtonClicked()
{
    //qDebug() << "SetupDialog::slotOkButtonClicked" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);

    if (!miscPage->areDBPathChangesApplied())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("DB has not been moved to new path."));
        msgBox.setInformativeText(tr("Go to the Misc tab and click on Move DB\n or the DB will not be moved to the new location."));
        msgBox.exec();
        emit debugLog (Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }

    if (!util->isValidCall(userDataPage->getStationQrz())){ //
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You need to enter at least a valid callsign."));
        msgBox.setInformativeText(tr("Go to the User tab and enter valid callsign."));
        msgBox.exec();
        emit debugLog (Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

    if (!haveAtleastOneLog())
    {
        //qDebug() << "SetupDialog::slotOkButtonClicked - NO LOG!" << Qt::endl;

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You have not selected the kind of log you want."));
        msgBox.setInformativeText(tr("You will be redirected to the Log tab.\nPlease add and select the kind of log you want to use."));
        msgBox.exec();

        tabWidget->setCurrentIndex(tabWidget->indexOf(logsPage));
        logsPage->createNewLog();
        //emit newLogRequested(true); // Signal to be catched by logsPage true show new log

        emit debugLog (Q_FUNC_INFO, "END-3", logSeverity);
        return;
    }
    //qDebug() << "SetupDialog::slotOkButtonClicked - 10" << Qt::endl;
    QFile file (configFileName);
    QString tmp;
    tmp = "true";
    if (file.open (QIODevice::WriteOnly))   /* Flawfinder: ignore */
    {
        QTextStream stream (&file);
    /*QString stationCall;
    int contest;
    int contestCategory;
    int modes;*/
    //QRZ/CQ/ITU/CONTEST
    stream << "Version=" << version << ";" << Qt::endl;
    stream << "Callsign="  << userDataPage->getStationQrz() << ";" << Qt::endl;
    if ((userDataPage->getOperators()).length() >= 3){ // There are no valid calls with less than 3 Chars
        stream << "Operators="  << userDataPage->getOperators() << ";" << Qt::endl;
    }
    stream << "CQz=" << QString::number(userDataPage->getCQz()) <<  ";" <<  Qt::endl;
    stream << "ITUz=" << QString::number(userDataPage->getITUz()) <<  ";" <<  Qt::endl;

        if ( locator->isValidLocator(userDataPage->getStationLocator()) )
        {
            stream << "StationLocator=" << userDataPage->getStationLocator() << ";" << Qt::endl;
        }


        if ((!(userDataPage->getName()).isNull()) && (  (userDataPage->getName()).length() > 0   ))
        {
            stream << "Name=" << userDataPage->getName() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getAddress1()).isNull()) && (  (userDataPage->getAddress1()).length() > 0   ))
        {
            stream << "Address1=" << userDataPage->getAddress1() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getAddress2()).isNull())  && (  (userDataPage->getAddress2()).length() > 0   ))
        {
            stream << "Address2=" << userDataPage->getAddress2() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getAddress3()).isNull()) && (  (userDataPage->getAddress3()).length() > 0   ))
        {
            stream << "Address3=" << userDataPage->getAddress3() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getAddress4()).isNull()) && (  (userDataPage->getAddress4()).length() > 0   ))
        {
            stream << "Address4=" << userDataPage->getAddress4() <<";" << Qt::endl;
        }

        if ((!(userDataPage->getCity()).isNull()) && (  (userDataPage->getCity()).length() > 0   ))
        {
            stream << "City=" << userDataPage->getCity() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getZipCode()).isNull()) && (  (userDataPage->getZipCode()).length() > 0   ))
        {
            stream << "ZipCode=" << userDataPage->getZipCode() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getProvince()).isNull()) && (  (userDataPage->getProvince()).length() > 0   ))
        {
            stream << "ProvinceState=" << userDataPage->getProvince() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getCountry()).isNull()) && (  (userDataPage->getCountry()).length() > 0   ))
        {
            stream << "Country=" << userDataPage->getCountry() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getRig1()).isNull()) && (  (userDataPage->getRig1()).length() > 0   ))
        {
            stream << "Rig1=" << userDataPage->getRig1() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getRig2()).isNull()) && (  (userDataPage->getRig2()).length() > 0   ))
        {
            stream << "Rig2=" << userDataPage->getRig2() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getRig3()).isNull()) && (  (userDataPage->getRig3()).length() > 0   ))
        {
            stream << "Rig3=" << userDataPage->getRig3() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getAntenna1()).isNull()) && (  (userDataPage->getAntenna1()).length() > 0   ))
        {
            stream << "Antenna1=" << userDataPage->getAntenna1() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getAntenna2()).isNull()) && (  (userDataPage->getAntenna2()).length() > 0   ))
        {
            stream << "Antenna2=" << userDataPage->getAntenna2() <<";" << Qt::endl;
        }
        if ((!(userDataPage->getAntenna3()).isNull()) && (  (userDataPage->getAntenna2()).length() > 0   ))
        {
            stream << "Antenna3=" << userDataPage->getAntenna3() <<";" << Qt::endl;
        }
        if ((userDataPage->getPower()).toFloat()>=0)
        {
            stream << "Power=" << userDataPage->getPower() << ";" << Qt::endl;
        }
        //qDebug() << "SetupDialog::slotOkButtonClicked - 20" << Qt::endl;
        //stream << "locator=" << (MyLocatorkLineEdit->text ()).toUpper () <<  ";" << Qt::endl;
        //stream << "CallUsed=" << (UserDataPage.qrzLineEdit).text() <<  ";" << Qt::endl;
        //stream << "Operators=" <<  ";" << Qt::endl;
        stream << "Bands=" << bandModePage->getBands() << ";" <<  Qt::endl;
        stream << "Modes=" << bandModePage->getModes() << ";" <<  Qt::endl;

        //stream << "InMemory=" << miscPage->getInMemory() << ";" <<  Qt::endl;
        stream << "RealTime=" << miscPage->getRealTime() << ";" <<  Qt::endl;
        stream << "UTCTime=" << miscPage->getUTCTime() << ";" <<  Qt::endl;
        stream << "AlwaysADIF=" << miscPage->getAlwaysADIF() << ";" <<  Qt::endl;
        stream << "UseDefaultName=" << miscPage->getUseDefaultName() << ";" <<  Qt::endl;
        stream << "DefaultADIFFile=" << miscPage->getDefaultFileName() << ";" <<  Qt::endl;
        stream << "DBPath=" << miscPage->getDefaultDBPath() << ";" <<  Qt::endl;
        stream << "ImperialSystem=" << miscPage->getImperial() << ";" <<  Qt::endl;
        stream << "SendQSLWhenRec=" << miscPage->getSendQSLWhenRec() << ";" <<  Qt::endl;
        stream << "ShowCallsignInSearch=" << miscPage->getShowStationCallSignInSearch() << ";" <<  Qt::endl;
        //stream << "KeepMyData=" << miscPage->getKeepMyData() << ";" <<  Qt::endl;
        stream << "CompleteWithPrevious=" << miscPage->getCompleteWithPrevious() << ";" <<  Qt::endl;
        stream << "CheckNewVersions=" << miscPage->getCheckNewVersions() << ";" <<  Qt::endl;
        stream << "ManageDXMarathon=" << miscPage->getDXMarathon() << ";" <<  Qt::endl;
        stream << "DebugLog=" << miscPage->getDebugLog() << ";" << Qt::endl;
        //stream << "LogSort=" << miscPage->getLogSort() << ";" << Qt::endl;
        stream << "SendEQSLByDefault=" << miscPage->getSendEQSLByDefault() << ";" << Qt::endl;
        stream << "DeleteAlwaysAdiFile=" << miscPage->getDeleteAlwaysAdiFile() << ";" << Qt::endl;

        if (miscPage->getDupeTime()>0)
        {
            stream << "DuplicatedQSOSlot=" << QString::number(miscPage->getDupeTime()) << ";" << Qt::endl;
        }

        //stream << "PSTRotatorActive=" << interfacesWindowsPage->getSendToPSTRotator() << ";" << Qt::endl;
        //stream << "PSTRotatorServer=" << interfacesWindowsPage->getPSTRotatorUDPServer() << ";" << Qt::endl;
        //stream << "PSTRotatorPort=" << interfacesWindowsPage->getPSTRotatorUDPServerPort() << ";" << Qt::endl;

        //qDebug() << "SetupDialog::slotOkButtonClicked - 30" << Qt::endl;

        if ( miscPage->getReportInfo())
        {
            stream << "ProvideInfo=True;"  <<  Qt::endl;
        }


        if ((!(dxClusterPage->getSelectedDxClusterServer()).isNull()) && (  (dxClusterPage->getSelectedDxClusterServer()).length() > 0   ))
        {
            stream << "DXClusterServerToUse=" << dxClusterPage->getSelectedDxClusterServer() <<";" << Qt::endl;
        }

        QStringList stringList;
        stringList.clear();
        stringList << dxClusterPage->getDxclusterServersComboBox();

        if (stringList.size()>0)
        {
            for (int i = 0; i < stringList.size(); i++)
            {
                 stream << "DXClusterServerPort="<< stringList.at(i) << ";" << Qt::endl;
            }
        }

        stream << "DXClusterShowHF=" << dxClusterPage->getShowHFQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterShowVHF=" << dxClusterPage->getShowVHFQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterShowWARC=" << dxClusterPage->getShowWARCQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterShowWorked=" << dxClusterPage->getShowWorkedQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterShowConfirmed=" << dxClusterPage->getShowConfirmedQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterShowAnn=" << dxClusterPage->getShowANNQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterShowWWV=" << dxClusterPage->getShowWWVQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterShowWCY=" << dxClusterPage->getShowWCYQCheckbox() << ";" <<  Qt::endl;
        stream << "DXClusterSave=" << dxClusterPage->getSaveActivityQCheckbox() << ";" <<  Qt::endl;

        stream << "NewOneColor=" << colorsPage->getNewOneColor() << ";" <<  Qt::endl;
        stream << "NeededColor=" << colorsPage->getNeededColor() << ";" <<  Qt::endl;
        stream << "WorkedColor=" << colorsPage->getWorkedColor() << ";" <<  Qt::endl;
        stream << "ConfirmedColor=" << colorsPage->getConfirmedColor() << ";" <<  Qt::endl;
        stream << "DefaultColor=" << colorsPage->getDefaultColor() << ";" <<  Qt::endl;
        stream << "DarkMode=" << colorsPage->getDarkMode() << ";" << Qt::endl;
        stream << "SelectedLog=" << QString::number(logsPage->getSelectedLog()) << ";" <<  Qt::endl;
        //qDebug() << "SetupDialog::slotOkButtonClicked SelectedLog: " << logsPage->getSelectedLog() << Qt::endl;
        // CLUBLOG
        //qDebug() << "SetupDialog::slotOkButtonClicked - 40" << Qt::endl;

            if (eLogPage->getClubLogActive())
            {
                stream << "ClubLogActive=True;"<<  Qt::endl;
            }
            else
            {
                stream << "ClubLogActive=False;"<< Qt::endl;
            }
            if (eLogPage->getClubLogRealTime())
            {
                stream << "ClubLogRealTime=True;" <<  Qt::endl;
            }
            else
            {
                stream << "ClubLogRealTime=False;" << Qt::endl;
            }
            tmp = eLogPage->getClubLogEmail() ;
            if (tmp.length()>0)
            {
                 stream << "ClubLogEmail=" << tmp << ";" <<  Qt::endl;
            }

            tmp = eLogPage->getClubLogPassword() ;
            if (tmp.length()>0)
            {
                stream << "ClubLogPass=" << tmp << ";" <<  Qt::endl;
            }

        //qDebug() << "SetupDialog::slotOkButtonClicked - 50" << Qt::endl;
        // eQSL


            if (eLogPage->getEQSLActive())
            {
                stream << "eQSLActive=True;" <<  Qt::endl;
            }
            else
            {
                stream << "eQSLActive=False;" << Qt::endl;
            }
            tmp = eLogPage->getEQSLUser();
            if (tmp.length()>0)
            {
                 stream << "eQSLCall=" << tmp << ";" <<  Qt::endl;
            }

            tmp = eLogPage->getEQSLPassword() ;
            if (tmp.length()>0)
            {
                stream << "eQSLPass=" << tmp << ";" <<  Qt::endl;
            }

        // eQSL - END

        // QRZ.com

            //qDebug() << "SetupDialog::slotOkButtonClicked - Storing QRZ.com data" << Qt::endl;

            if (eLogPage->getQRZCOMActive())
            {
                stream << "QRZcomActive=True;" <<  Qt::endl;
            }
            else
            {
                stream << "QRZcomActive=False;" << Qt::endl;
            }
            tmp = eLogPage->getQRZCOMUser();
            if (tmp.length()>0)
            {
                 stream << "QRZcomUser=" << tmp << ";" <<  Qt::endl;
            }
            tmp = eLogPage->getQRZCOMPassword();
            if (tmp.length()>0)
            {
                 stream << "QRZcomPass=" << tmp << ";" <<  Qt::endl;
            }

            if (eLogPage->getQRZCOMAutoCheck())
            {
                 stream << "QRZcomAuto=True;" << Qt::endl;
            }
            else
            {
                stream << "QRZcomAuto=False;" <<  Qt::endl;
            }
            tmp = eLogPage->getQRZCOMLogBookKEY();
            if (tmp.length()>0)
            {
                 stream << "QRZcomLogBookKey=" << tmp << ";" <<  Qt::endl;
            }

        // QRZ.com - END

        //qDebug() << "SetupDialog::slotOkButtonClicked - 60" << Qt::endl;

        // LOTW
        stream << "LoTWActive=" << eLogPage->getLoTWActive() << ";" <<  Qt::endl;
        tmp = eLogPage->getTQSLPath();
        if (tmp.length()>0)
        {
            stream << "LoTWPath=" << tmp << ";" <<  Qt::endl;
        }
        tmp = eLogPage->getLoTWUser();
        if (tmp.length()>0)
        {
            stream << "LoTWUSer=" << tmp << ";" <<  Qt::endl;
        }
        tmp = eLogPage->getLoTWPass();
        if (tmp.length()>0)
        {
            stream << "LoTWPass=" << tmp << ";" <<  Qt::endl;
        }

        // LOTW
        //qDebug() << "SetupDialog::slotOkButtonClicked - 70" << Qt::endl;
        //WSJTX
        stream << "UDPServer=" << UDPPage->getUDPServer() << ";" <<  Qt::endl;
        stream << "UDPNetworkInterface=" << UDPPage->getNetworkInterface() << ";" <<  Qt::endl;
        stream << "UDPServerPort=" << UDPPage->getUDPServerPort() << ";" <<  Qt::endl;
        stream << "LogFromWSJTX=" << UDPPage->getLogFromWSJTx() << ";" <<  Qt::endl;
        stream << "LogAutoFromWSJTX=" << UDPPage->getAutoLogFromWSJTx() << ";" <<  Qt::endl;
        stream << "RealTimeFromWSJTX=" << UDPPage->getReaDataFromWSJTx() << ";" <<  Qt::endl;
        stream << "InfoTimeOut=" << UDPPage->getTimeout() << ";" <<  Qt::endl;

          //qDebug() << "SetupDialog::slotOkButtonClicked: hamlib" << Qt::endl;
        QString _aa = hamlibPage->getData();
        stream << _aa << Qt::endl;

          //qDebug() << "SetupDialog::slotOkButtonClicked: hamlib-2: " << _aa << Qt::endl;

        //WSJTX

        //Windows Size
        if (windowSize.length()>0)
        {
            stream << "MainWindowSize=" << windowSize << ";" <<  Qt::endl;
        }
        if (latestBackup.length()>0)
        {
            stream << "LatestBackup=" << latestBackup << ";" << Qt::endl;
        }
        file.close ();
    }
    //qDebug() << "SetupDialog::slotOkButtonClicked - just before leaving" << Qt::endl;
    QDialog::accept();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
    //qDebug() << "SetupDialog::slotOkButtonClicked - END" << Qt::endl;
    //close();
}

void SetupDialog::slotReadConfigData()
{
    //qDebug() << "SetupDialog::slotReadConfigData" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    if (firstTime)
    {
        //qDebug() << "SetupDialog::slotReadConfigData - First time" << Qt::endl;
        setDefaults();
        bands.removeDuplicates();
        modes.removeDuplicates();
        bandModePage->setActiveModes(modes);
        bandModePage->setActiveBands(bands);
    }

    //qDebug() << "SetupDialog::slotReadConfigData - 1" << Qt::endl;

    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  /* Flawfinder: ignore */
    {
        //qDebug() << "SetupDialog::slotReadConfigData() File not found" << configFileName << Qt::endl;
        //firstTime = true;
        emit debugLog (Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    //qDebug() << "SetupDialog::slotReadConfigData - 2" << Qt::endl;
    //dxClusterServers.clear();

    while (!file.atEnd()){
        QByteArray line = file.readLine();
        processConfigLine(line);
        //qDebug() << "SetupDialog::slotReadConfigData - in the while" << Qt::endl;
    }
    //qDebug() << "SetupDialog::slotReadConfigData - 3" << Qt::endl;

    dxClusterPage->setDxclusterServersComboBox(dxClusterServers);
    dxClusterPage->setSelectedDxClusterServer(dxClusterServerToUse);

    if (modes.isEmpty())
    {
        modes << "SSB" << "CW" << "RTTY";

    }
    if (bands.isEmpty())
    {
        bands << "10M" << "12M" << "15M" << "17M" << "20M" << "40M" << "80M" << "160M";

    }
    modes.removeDuplicates();
    bandModePage->setActiveModes(modes);
    bands.removeDuplicates();
    bandModePage->setActiveBands(bands);
    //qDebug() << "SetupDialog::slotReadConfigData - END" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);

}

bool SetupDialog::processConfigLine(const QString &_line)
{
    //qDebug() << "SetupDialog::processConfigLine: " << _line << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);

    QString line = _line.simplified();
    //line.simplified();
    int i = 0; //aux variable
    QStringList values = line.split("=", Qt::SkipEmptyParts);
    QString tab = QString();


    if (line.startsWith('#')){
           //qDebug() << "SetupDialog::processConfigLine: Comment Line!" << Qt::endl;
        emit debugLog (Q_FUNC_INFO, "END-1", logSeverity);
        return true;
    }
    if (!( (line.contains('=')) && (line.contains(';')))){
           //qDebug() << "SetupDialog::processConfigLine: Wrong Line!" << Qt::endl;
        emit debugLog (Q_FUNC_INFO, "END-2", logSeverity);
        return false;
    }
    QString value = values.at(1);
    tab = (values.at(0)).toUpper();

    int endValue = value.indexOf(';');
    if (endValue>-1){

        value = value.left(value.length() - (value.length() - endValue));
    }
    value = checkAndFixASCIIinADIF(value); // Check whether the value is valid.
      //qDebug() << "SetupDialog::processConfigLine: TAB: " << tab << Qt::endl;
      //qDebug() << "SetupDialog::processConfigLine: VALUE: " << value << Qt::endl;
    if (tab == "CALLSIGN"){
           //qDebug() << "SetupDialog::processConfigLine: CALLSIGN: " << value << Qt::endl;
        userDataPage->setStationQrz(value);
    }else if (tab == "OPERATORS"){
        userDataPage->setOperators(value);
    }else if (tab=="CQZ"){
        userDataPage->setCQz((value).toInt());

    }else if (tab=="ITUZ"){
        userDataPage->setITUz((value).toInt());
    }else if (tab=="CONTEST"){
        //userDataPage->setContest(value);
    }else if (tab=="MODES"){
        readActiveModes(value);
        modes.removeDuplicates();
        bandModePage->setActiveModes(modes);
    }else if (tab=="BANDS"){
        readActiveBands(value);
        bands.removeDuplicates();
        bandModePage->setActiveBands(bands);
    //}else if (tab=="INMEMORY"){
    //    miscPage->setInMemory(value);
    }else if (tab=="REALTIME"){
        miscPage->setRealTime(value);
    }else if (tab=="UTCTIME"){
        miscPage->setUTCTime(value);
    }else if (tab=="ALWAYSADIF"){
        miscPage->setAlwaysADIF(value);
    }else if (tab=="USEDEFAULTNAME"){
        miscPage->setDefaultFileName(value);
    }else if (tab=="DBPATH"){
        miscPage->setUseDefaultDBPath(value);
    }else if (tab=="DEFAULTADIFFILE"){
        miscPage->setDefaultFileName(value);
           //qDebug() << "SetupDialog::processConfigLine: FILE: " << value << Qt::endl;
    }else if (tab=="IMPERIALSYSTEM"){
        miscPage->setImperial(value.toUpper());
    }else if (tab=="COMPLETEWITHPREVIOUS"){
        miscPage->setCompleteWithPrevious(value.toUpper());
    }else if (tab=="SENDQSLWHENREC"){
        miscPage->setSendQSLWhenRec(value.toUpper());
    }else if (tab=="MANAGEDXMARATHON"){
        miscPage->setDXMarathon(value.toUpper());
    }else if (tab=="DEBUGLOG"){
        miscPage->setDebugLog(value);
    }
    else if (tab=="SHOWCALLSIGNINSEARCH"){
        miscPage->setShowStationCallSignInSearch(value.toUpper());
    }
    else if (tab=="CHECKNEWVERSIONS"){
        miscPage->setCheckNewVersions(value);
    }
    else if (tab=="PROVIDEINFO"){
        miscPage->setReportInfo(value);
    }
    /*
    else if (tab=="LOGSORT"){
        miscPage->setLogSort(value);
    }
    */
    else if (tab=="SENDEQSLBYDEFAULT"){
        miscPage->setSetEQSLByDefault(value);
    }
    else if (tab=="DUPLICATEDQSOSLOT"){
        if (value.toInt()>=0)
        {
            miscPage->setDupeTime(value.toInt());
        }
    }

    /*
    else if (tab=="PSTROTATORACTIVE"){
        interfacesWindowsPage->setSendToPSTRotator(value);
    }
    else if (tab=="PSTROTATORPORT"){
        interfacesWindowsPage->setPSTRotatorUDPServerPort(value);
    }
    else if (tab=="PSTROTATORSERVER")
    {
        interfacesWindowsPage->setPSTRotatorUDPServer(value);
    }
    */
    else if (tab=="UDPSERVER"){
        UDPPage->setUDPServer(value);
    }
    else if (tab=="UDPNETWORKINTERFACE"){
        UDPPage->setNetworkInterface(value);
    }
    else if (tab=="UDPSERVERPORT"){
        UDPPage->setUDPServerPort(value);
    }
    else if (tab=="LOGFROMWSJTX")
    {
        UDPPage->setLogFromWSJTx(value);
    }
    else if (tab=="LOGAUTOFROMWSJTX")
    {
        UDPPage->setAutoLogFromWSJTx(value);
    }
    else if (tab=="REALTIMEFROMWSJTX")
    {
        UDPPage->setReaDataFromWSJTx(value);
    }
    else if (tab=="INFOTIMEOUT")
    {
        UDPPage->setTimeout(value);
    }
    else if (tab =="NAME")
    {
        userDataPage->setName(value);
    }
    else if (tab =="ADDRESS1")
    {
        userDataPage->setAddress1(value);
    }
    else if (tab =="ADDRESS2")
    {
        userDataPage->setAddress2(value);
    }
    else if (tab =="ADDRESS3")
    {
        userDataPage->setAddress3(value);
    }
    else if (tab =="ADDRESS4")
    {
        userDataPage->setAddress4(value);
    }
    else if (tab =="CITY")
    {
        userDataPage->setCity(value);
    }
    else if (tab =="ZIPCODE")
    {
        userDataPage->setZipCode(value);
    }
    else if (tab =="PROVINCESTATE")
    {
        userDataPage->setProvince(value);
    }
    else if (tab =="COUNTRY")
    {
        userDataPage->setCountry(value);
    }
    else if (tab =="POWER")
    {
        userDataPage->setPower(value);
    }
    else if (tab =="RIG1")
    {
        userDataPage->setRig1(value);
    }
    else if (tab =="RIG2")
    {
        userDataPage->setRig2(value);
    }
    else if (tab =="RIG3")
    {
        userDataPage->setRig3(value);
    }

    else if (tab =="ANTENNA1")
    {
        userDataPage->setAntenna1(value);
    }
    else if (tab =="ANTENNA2")
    {
        userDataPage->setAntenna2(value);
    }
    else if (tab =="ANTENNA3")
    {
        userDataPage->setAntenna3(value);
    }
    else if (tab =="STATIONLOCATOR"){

        if ( locator->isValidLocator(value) )
        {
            userDataPage->setStationLocator(value);
        }
    }else if (tab =="DXCLUSTERSHOWHF"){
        dxClusterPage->setShowHFQCheckbox(value);
    }else if (tab =="DXCLUSTERSHOWVHF"){
        dxClusterPage->setShowVHFQCheckbox(value);
    }else if (tab =="DXCLUSTERSHOWWARC"){
        dxClusterPage->setShowWARCQCheckbox(value);
    }else if (tab =="DXCLUSTERSHOWWORKED"){
        dxClusterPage->setShowWorkedQCheckbox(value);
    }else if (tab =="DXCLUSTERSHOWCONFIRMED"){
        dxClusterPage->setShowConfirmedQCheckbox(value);
    }else if (tab =="DXCLUSTERSHOWANN"){
        dxClusterPage->setShowANNQCheckbox(value);
    }else if (tab =="DXCLUSTERSHOWWWV"){
        dxClusterPage->setShowWWVQCheckbox(value);
    }else if (tab =="DXCLUSTERSHOWWCY"){
        dxClusterPage->setShowWCYQCheckbox(value);
    }else if(tab =="DXCLUSTERSERVERPORT"){
        dxClusterServers << value;
           //qDebug() << "SetupDialog::processConfigLine: dxClusterServers: " << dxClusterServers.last() << Qt::endl;
    }else if (tab  =="DXCLUSTERSERVERTOUSE"){
        dxClusterServerToUse=value;
    }
    else if (tab  =="DXCLUSTERSAVE"){
        dxClusterPage->setSaveActivityQCheckbox(value);
    }
    else if(tab =="NEWONECOLOR"){
        colorsPage->setNewOneColor(value);
    }else if(tab =="NEEDEDCOLOR"){
        colorsPage->setNeededColor(value);
    }else if(tab =="WORKEDCOLOR"){
        colorsPage->setWorkedColor(value);
    }else if(tab =="CONFIRMEDCOLOR"){
        colorsPage->setConfirmedColor(value);
    }else if(tab =="DEFAULTCOLOR"){
        colorsPage->setDefaultColor(value);
          //qDebug() << "SetupDialog::processConfigLine: DEFAULTCOLOR: " << value << Qt::endl;
    }else if(tab =="DARKMODE"){
        colorsPage->setDarkMode(value);
    }else if(tab =="HAMLIBRIGTYPE"){
          //qDebug() << "SetupDialog::processConfigLine: Before HAMLIBRIGTYPE: " << value << Qt::endl;
        hamlibPage->setRigType(value);
          //qDebug() << "SetupDialog::processConfigLine: After HAMLIBRIGTYPE: " << value << Qt::endl;
    }else if(tab =="HAMLIBSERIALPORT"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALPORT: " << value << Qt::endl;
        hamlibPage->setSerialPort(value);
    }else if(tab =="HAMLIBSERIALBAUDS"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALBAUDS: " << value << Qt::endl;
        hamlibPage->setSerialSpeed(value);
    }else if(tab =="HAMLIB"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIB: " << value << Qt::endl;
        hamlibPage->setActive(value);
    }else if(tab=="HAMLIBREADONLY"){
        hamlibPage->setReadOnly(value);
    }else if(tab =="HAMLIBSERIALDATABITS"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDATABITS: " << value << Qt::endl;
        hamlibPage->setDataBits(value);
    }else if(tab =="HAMLIBSERIALSTOPBITS"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALSTOPBITS: " << value << Qt::endl;
        hamlibPage->setStopBits(value);
    }else if(tab =="HAMLIBSERIALFLOWCONTROL"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALFLOWCONTROL: " << value << Qt::endl;
        hamlibPage->setFlowControl(value);
    }else if(tab =="HAMLIBSERIALPARITY"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALPARITY: " << value << Qt::endl;
        hamlibPage->setParity(value);
    }else if(tab =="HAMLIBSERIALRTS"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALRTS: " << value << Qt::endl;
        //hamlibPage->setRTS(value);
    }else if(tab =="HAMLIBSERIALDTR"){
          //qDebug() << "SetupDialog::processConfigLine: HAMLIBSERIALDTR: " << value << Qt::endl;
        //hamlibPage->setDTR(value);
    }else if (tab == "HAMLIBRIGPOLLRATE"){
        hamlibPage->setPollingInterval(value.toInt());
    }else if (tab == "HAMLIBNETADDRESS"){
        hamlibPage->setRadioNetworkAddress (value);
    }else if (tab == "HAMLIBNETPORT"){
        hamlibPage->setRadioNetworkPort (value.toInt ());
    }else if(tab =="SELECTEDLOG"){
           //qDebug() << "SetupDialog::processConfigLine: SELECTEDLOG: " << value << Qt::endl;
        i = value.toInt();

        if (dataProxy->doesThisLogExist(i))
        {
               //qDebug() << "SetupDialog::processConfigLine: dataProxy->doesThisLogExist TRUE" << Qt::endl;
        }
        else
        {
               //qDebug() << "SetupDialog::processConfigLine: dataProxy->doesThisLogExist FALSE" << Qt::endl;
            i = 0;
            while(!dataProxy->doesThisLogExist(i))
            {
                i++;
            }
        }
        logsPage->setSelectedLog(i);
           //qDebug() << "SetupDialog::processConfigLine: dataProxy->doesThisLogExist END" << Qt::endl;

    }else if(tab =="CLUBLOGACTIVE"){
        eLogPage->setClubLogActive(util->trueOrFalse(value));

    }
    else if(tab =="CLUBLOGREALTIME"){
        //clubLogPage->setClubLogRealTime(value);
        eLogPage->setClubLogRealTime(util->trueOrFalse(value));
    }
    else if(tab =="CLUBLOGPASS"){
        //clubLogPage->setPassword(value);
        eLogPage->setClubLogPassword(value);
    }
    else if(tab =="CLUBLOGEMAIL"){
        //clubLogPage->setEmail(value);
        eLogPage->setClubLogEmail(value);
    }
    else if(tab =="EQSLACTIVE"){
        //eQSLPage->setActive(value);
        eLogPage->setEQSLActive(util->trueOrFalse(value));
    }
    else if(tab =="EQSLCALL"){
        //eQSLPage->setCallsign(value);
        eLogPage->setEQSLUser(value);
    }
    else if(tab =="EQSLPASS"){
        //eQSLPage->setPassword(value);
        eLogPage->setEQSLPassword(value);
    }
    else if(tab =="QRZCOMACTIVE"){
        //eQSLPage->setActive(value);
        eLogPage->setQRZCOMActive(value);
    }
    else if(tab =="QRZCOMUSER"){
        eLogPage->setQRZCOMUser(value);
    }
    else if(tab =="QRZCOMAUTO"){
        eLogPage->setQRZCOMAutoCheck(value);
    }
    else if(tab =="QRZCOMPASS"){
        eLogPage->setQRZCOMPassword(value);
    }
    else if(tab =="QRZCOMLOGBOOKKEY"){
        eLogPage->setQRZCOMLogBookKEY(value);
    }
    else if(tab =="LOTWACTIVE"){
        eLogPage->setLoTWActive(util->trueOrFalse(value));
    }
    else if(tab =="LOTWPATH"){
        eLogPage->setTQSLPath(value);
    }
    else if(tab =="LOTWUSER"){
        eLogPage->setLoTWUser(value);
    }
    else if(tab =="LOTWPASS"){
        eLogPage->setLoTWPass(value);
    }
    else if(tab =="MAINWINDOWSIZE"){
        QStringList values;
        values.clear();
        values << value.split("x");
        if ((values.at(0).toInt()>0) && (values.at(1).toInt()>0))
        {
            windowSize = value;
        }
    }
    else if(tab =="DELETEALWAYSADIFILE"){
            miscPage->setDeleteAlwaysAdiFile(util->trueOrFalse(value));
        }
    else if (tab == "LATESTBACKUP"){
        if (value.length()>0)
        {
            latestBackup = value;
        }
        else
        {
            latestBackup = QString();
        }
    //s.append("LatestBackup=" + (QDateTime::currentDateTime()).toString("yyyyMMdd-hhmmss") + ";\n" );
    }
    else
    {
           //qDebug() << "SetupDialog::processConfigLine: NONE: " << Qt::endl;
    }

    // Lines are: Option = value;

       //qDebug() << "SetupDialog::processConfigLine: END "  << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
    return true;


}

void SetupDialog::readActiveBands (const QString &actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
    // bands used in the program
      //qDebug() << "SetupDialog::readActiveBands: " << actives << Qt::endl;

    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    bool atLeastOne = false;

    QStringList values = actives.split(", ", Qt::SkipEmptyParts);
    QStringList _abands;

    for (int i = 0; i < values.size() ; i++)
    {
        if (isValidBand(values.at(i)))
        {
            if (!atLeastOne)
            {
                   //qDebug() << "SetupDialog::readActiveBands (at least One!): " << values.at(i) << Qt::endl;
                atLeastOne = true;
                _abands.clear();
            }

            _abands << values.at(i);
               //qDebug() << "SetupDialog::readActiveBands: " << values.at(i) << Qt::endl;
        }

    }

    bands.clear();
    //_abands.removeDuplicates();

    bands << dataProxy->getBandsInLog(-1);
    bands << _abands;
    bands.removeDuplicates();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::readActiveModes (const QString &actives)
{
       //qDebug() << "SetupDialog::readActiveModes: " << actives << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);

    bool atLeastOne = false;
    QStringList _amodes;//, _backModes;
    // _backModes.clear();
    // _backModes << modes;
    QStringList values = actives.split(", ", Qt::SkipEmptyParts);
    values.removeDuplicates();

    for (int i = 0; i < values.size() ; i++)
    {
        if (isValidMode(values.at(i)))
        {
            if (!atLeastOne)
            {
                atLeastOne = true;
                _amodes.clear();
            }
            _amodes << values.at(i);
        }
    }

    modes.clear();
    modes = dataProxy->getModesInLog(-1);
    modes << _amodes;
    modes.removeDuplicates();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
       //qDebug() << "SetupDialog::readActiveModes: " << modes.join(" / ") << Qt::endl;
}

bool SetupDialog::isValidBand (const QString &b)
{
       //qDebug() << "SetupDialog::isValidBand: "<< b << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    QString stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
    return query.isValid();

}
bool SetupDialog::isValidMode (const QString &b)
{
       //qDebug() << "SetupDialog::isValidMode: " << b << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    QString stringQuery = QString("SELECT id FROM mode WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
    return query.isValid();

}

void SetupDialog::setDefaults()
{
       //qDebug() << "SetupDialog::setDefaults" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    miscPage->setRealTime("TRUE");
    miscPage->setUTCTime("TRUE");
    miscPage->setImperial("FALSE"); //Metric system is the default
    miscPage->setAlwaysADIF("FALSE");
    miscPage->setSendQSLWhenRec("TRUE");
    miscPage->setShowStationCallSignInSearch("TRUE");
    miscPage->setCheckNewVersions("TRUE");
    miscPage->setReportInfo("FALSE");
    miscPage->setDXMarathon("FALSE");
    miscPage->setDebugLog("FALSE");
    //miscPage->setLogSort("FALSE");
    miscPage->setSetEQSLByDefault("TRUE");

    UDPPage->setUDPServer("FALSE");
    UDPPage->setUDPServerPort("2237");
    UDPPage->setTimeout("2000");
    UDPPage->setLogFromWSJTx("FALSE");
    UDPPage->setReaDataFromWSJTx("FALSE");
    UDPPage->setAutoLogFromWSJTx("FALSE");
    //interfacesWindowsPage->setSendToPSTRotator("FALSE");
    //interfacesWindowsPage->setPSTRotatorUDPServer("locahost");
    //interfacesWindowsPage->setPSTRotatorUDPServerPort("12040");

    dxClusterPage->setShowHFQCheckbox("TRUE");
    dxClusterPage->setShowVHFQCheckbox("TRUE");
    dxClusterPage->setShowWARCQCheckbox("TRUE");
    dxClusterPage->setShowWorkedQCheckbox("TRUE");
    dxClusterPage->setShowConfirmedQCheckbox("TRUE");
    dxClusterPage->setShowANNQCheckbox("TRUE");
    dxClusterPage->setShowWWVQCheckbox("TRUE");
    dxClusterPage->setShowWCYQCheckbox("TRUE");
    dxClusterServers.clear();
    dxClusterServers.append("dxfun.com:8000");
    dxClusterServerToUse = "dxfun.com:8000";

    if (modes.isEmpty())
    {
        modes << "SSB" << "CW" << "RTTY";
        modes.removeDuplicates();
    }

    if (bands.isEmpty())
    {
        bands << "10M" << "12M" << "15M" << "17M" << "20M" << "40M" << "80M" << "160M";
        bands.removeDuplicates();
    }
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

QString SetupDialog::checkAndFixASCIIinADIF(const QString &_data)
{
       //qDebug() << "SetupDialog::checkAndFixASCIIinADIF " << _data << Qt::endl;
//TODO: this function is also in the FileManager class. Maybe I should call that one and keep just one copy
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
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
           //qDebug() << "SetupDialog::checkAndFixunicodeinADIF: " << st.at(i) <<" = " << QString::number(unicodeVal) << Qt::endl;
    }

    // Show into another lineEdit

    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
    return newString;
}

bool SetupDialog::haveAtleastOneLog()
{
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    emit debugLog (Q_FUNC_INFO, "END-1", logSeverity);
    return dataProxy->haveAtLeastOneLog();
    //emit debugLog (Q_FUNC_INFO, "END", logSeverity);

}

void SetupDialog::setClubLogActive(const bool _b)
{
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    eLogPage->setClubLogActive(_b);
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::setQRZCOMAutoCheckActive(const bool _b)
{
     eLogPage->setQRZCOMAutoCheck(util->boolToQString(_b));
}

void SetupDialog::setEQSLActive(const bool _b)
{
    eLogPage->setEQSLActive(_b);
}

void SetupDialog::checkIfNewBandOrMode()
{
      //qDebug() << "SetupDialog::checkIfNewBandOrMode: logSeverity: " << QString::number(logSeverity) << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    QStringList _items;

    _items.clear();
       //qDebug() << "SetupDialog::checkIfNewBandOrMode -1" << Qt::endl;
    _items << dataProxy->getBandsInLog(-1);
       //qDebug() << "SetupDialog::checkIfNewBandOrMode -2" << Qt::endl;
    _items << (bandModePage->getBands()).split(", ", Qt::SkipEmptyParts);
       //qDebug() << "SetupDialog::checkIfNewBandOrMode -3" << Qt::endl;
    _items.removeDuplicates();
       //qDebug() << "SetupDialog::checkIfNewBandOrMode -4" << Qt::endl;
    bandModePage->setActiveBands(_items);
       //qDebug() << "SetupDialog::checkIfNewBandOrMode -5" << Qt::endl;

    _items.clear();
    _items << dataProxy->getModesInLog(-1);
    _items << (bandModePage->getModes()).split(", ", Qt::SkipEmptyParts);
    _items.removeDuplicates();
    bandModePage->setActiveModes(_items);
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
       //qDebug() << "SetupDialog::checkIfNewBandOrMode END" << Qt::endl;
}


void SetupDialog::slotAnalyzeNewLogData(const QStringList _qs)
{
      //qDebug() << "SetupDialog::slotAnalyzeNewLogData (length=" << QString::number(_qs.length()) << ")" << Qt::endl;
       //qDebug() << "SetupDialog::slotAnalyzeNewLogData" << Qt::endl;
 // We receive the station callsign and operators from the logs tab
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    if (_qs.length()!=2)
    {
        emit debugLog (Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    userDataPage->setStationQrz(_qs.at(0));
    userDataPage->setOperators(_qs.at(1));
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::slotSetStationCallSign(const QString &_p)
{
       //qDebug() << "SetupDialog::slotSetStationCallSign: " << _p << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    logsPage->setDefaultStationCallsign(_p);
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::slotSetOperators(const QString &_p)
{
       //qDebug() << "SetupDialog::slotSetOperators: " << _p << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    logsPage->setDefaultOperators(_p);
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::setSeverity(const DebugLogLevel _sev)
{
    logSeverity = _sev;
}

void SetupDialog::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery)
{   emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    emit queryError(functionFailed, errorCodeS, nativeError, failedQuery);
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
}

void SetupDialog::slotFocusOK()
{
      //qDebug() << "SetupDialog::slotFocusOK" << Qt::endl;
    okButton->setFocus(Qt::OtherFocusReason);
}

void SetupDialog::showEvent(QShowEvent *event)
{
    //qDebug() << Q_FUNC_INFO << Qt::endl;
    //qDebug() << Q_FUNC_INFO << " - selectedLog: " << QString::number(logsPage->getSelectedLog()) << Qt::endl;
    QWidget::showEvent(event);

    userDataPage->setStationFocus();
}
