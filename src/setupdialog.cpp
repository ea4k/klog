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
#include "klogconfig.h"
//#include <QDebug>

/*
This class calls all the othet "Setup..." to manage the configuration

*/

SetupDialog::SetupDialog(DataProxy_SQLite *dp, const QString &_softwareVersion, const int _page, const bool _firstTime, QWidget *parent)
{
    //qDebug() << "SetupDialog::SetupDialog 2: /" << _softwareVersion << "/" << QString::number(_page) << _firstTime << endl ;

    logSeverity = Info;
    constrid = 2;
    util = new Utilities;
    firstTime = _firstTime;
    latestBackup = QString();
    dataProxy = dp;
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
    logViewPage = new SetupPageLogView(dataProxy, this);
    //subdivisionsPage = new SetupPageSubdivisions(dataProxy, this);
    //regionalAwardsPage = new SetupPageRegionalAwards(dataProxy, this);
    //interfacesWindowsPage = new SetupPageInterfacesWindows(this);

    //qDebug() << "SetupDialog::SetupDialog 02" << Qt::endl;
    //tabWidget->addTab(subdivisionsPage, tr("Subdivisions"));
    tabWidget->addTab(userDataPage, tr("User data"));
    tabWidget->addTab(bandModePage, tr("Bands/Modes"));
    tabWidget->addTab(logViewPage, tr("Log widget"));
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

void SetupDialog::setData(const QString &_softwareVersion, const int _page, const bool _firstTime)
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

    setSoftVersion(_softwareVersion);
    slotReadConfigData ();
    setPage(_page);
    //removeBandModeDuplicates();
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "SetupDialog::setData - END" << Qt::endl;
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
    KlogConfig config;
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

    config.setValue("version", version);
    config.setValue("userdata/callsign", userDataPage->getStationQrz());

    if ((userDataPage->getOperators()).length() >= 3)
    { // There are no valid calls with less than 3 Chars
        config.setValue("userdata/operators",userDataPage->getOperators());
    }
    config.setValue("userdata/cqz", QString::number(userDataPage->getCQz()));
    config.setValue("userdata/ituz", QString::number(userDataPage->getITUz()));
    if ( locator->isValidLocator(userDataPage->getStationLocator()) )
    {
        config.setValue("userdata/stationlocator", userDataPage->getStationLocator());
    }
    else
    {
        config.setValue("userdata/stationlocator", QString());
    }

    config.setValue("userdata/name",userDataPage->getName());
    config.setValue("userdata/address1", userDataPage->getAddress1());
    config.setValue("userdata/address2", userDataPage->getAddress2());
    config.setValue("userdata/address3", userDataPage->getAddress3());
    config.setValue("userdata/address4", userDataPage->getAddress4());
    config.setValue("userdata/city", userDataPage->getCity());
    config.setValue("userdata/zipcode", userDataPage->getZipCode());
    config.setValue("userdata/provincestate", userDataPage->getProvince());
    config.setValue("userdata/country", userDataPage->getCountry());
    config.setValue("userdata/rig1", userDataPage->getRig1());
    config.setValue("userdata/rig2", userDataPage->getRig2());
    config.setValue("userdata/rig3", userDataPage->getRig3());
    config.setValue("userdata/antenna1", userDataPage->getAntenna1());
    config.setValue("userdata/antenna2", userDataPage->getAntenna2());
    config.setValue("userdata/antenna3", userDataPage->getAntenna3());
    config.setValue("userdata/power", userDataPage->getPower());
    config.setValue("userdata/bands", bandModePage->getBands());
    config.setValue("userdata/modes", bandModePage->getModes());
    config.setValue("realtime", miscPage->getRealTime());
    config.setValue("utctime", miscPage->getUTCTime());
    config.setValue("alwaysadif", miscPage->getAlwaysADIF());
    config.setValue("usedefaultname", miscPage->getUseDefaultName());
    config.setValue("defaultadiffile", miscPage->getDefaultFileName());
    config.setValue("dbpath", miscPage->getDefaultDBPath());
    config.setValue("imperialSystem", miscPage->getImperial());
    config.setValue("sendqslwhenrec", miscPage->getSendQSLWhenRec());
    config.setValue("showcallsigninsearch", miscPage->getShowStationCallSignInSearch());
    config.setValue("completewithprevious", miscPage->getCompleteWithPrevious());
    config.setValue("checknewversions", miscPage->getCheckNewVersions());
    config.setValue("managedxmarathon", miscPage->getDXMarathon());
    config.setValue("debuglog", miscPage->getDebugLog());
    config.setValue("sendeqslbydefault", miscPage->getSendEQSLByDefault());
    config.setValue("deletealwaysadifile", miscPage->getDeleteAlwaysAdiFile());
    config.setValue("duplicateqsoslot", QString::number(miscPage->getDupeTime()));
    config.setValue("provideinfo", miscPage->getReportInfo());
    config.setValue("DXC/servertouse", dxClusterPage->getSelectedDxClusterServer());
    config.setValue("DXC/servervariants", dxClusterPage->getDxclusterServersComboBox());
    config.setValue("DXC/showhf", dxClusterPage->getShowHFQCheckbox());
    config.setValue("DXC/showvhf", dxClusterPage->getShowVHFQCheckbox());
    config.setValue("DXC/showwarc", dxClusterPage->getShowWARCQCheckbox());
    config.setValue("DXC/showworked", dxClusterPage->getShowWorkedQCheckbox());
    config.setValue("DXC/showconfirmed", dxClusterPage->getShowConfirmedQCheckbox());
    config.setValue("DXC/showann", dxClusterPage->getShowANNQCheckbox());
    config.setValue("DXC/showwwn", dxClusterPage->getShowWWVQCheckbox());
    config.setValue("DXC/showwcy", dxClusterPage->getShowWCYQCheckbox());
    config.setValue("DXC/save", dxClusterPage->getSaveActivityQCheckbox());
    config.setValue("colors/newone", colorsPage->getNewOneColor());
    config.setValue("colors/needed", colorsPage->getNeededColor());
    config.setValue("colors/worked", colorsPage->getWorkedColor());
    config.setValue("colors/confirmed", colorsPage->getConfirmedColor());
    config.setValue("colors/default", colorsPage->getDefaultColor());
    config.setValue("colors/darkmode", colorsPage->getDarkMode());
    config.setValue("selectedlog", QString::number(logsPage->getSelectedLog()));
    config.setValue("clublog/active", eLogPage->getClubLogActive());
    config.setValue("clublog/realtime", eLogPage->getClubLogRealTime());
    config.setValue("clublog/email", eLogPage->getClubLogEmail());
    config.setValue("clublog/pass", eLogPage->getClubLogPassword());
    config.setValue("eqsl/active", eLogPage->getEQSLActive());
    config.setValue("eqsl/call", eLogPage->getEQSLUser());
    config.setValue("eqsl/pass", eLogPage->getEQSLPassword());
    config.setValue("qrzcom/active", eLogPage->getQRZCOMActive());
    config.setValue("qrzcom/user", eLogPage->getQRZCOMUser());
    config.setValue("qrzcom/pass", eLogPage->getQRZCOMPassword());
    config.setValue("qrzcom/auto", eLogPage->getQRZCOMAutoCheck());
    config.setValue("qrzcom/logbookkey", eLogPage->getQRZCOMLogBookKEY());
    config.setValue("lotw/active", eLogPage->getLoTWActive());
    config.setValue("lotw/path", eLogPage->getTQSLPath());
    config.setValue("lotw/user", eLogPage->getLoTWUser());
    config.setValue("lotw/pass", eLogPage->getLoTWPass());
    config.setValue("udpserver/active", UDPPage->getUDPServer());
    config.setValue("udpserver/interface", UDPPage->getNetworkInterface());
    config.setValue("udpserver/port", UDPPage->getUDPServerPort());
    config.setValue("wsjtx/logfromwsjtx", UDPPage->getLogFromWSJTx());
    config.setValue("wsjtx/logautofromwsjtx", UDPPage->getAutoLogFromWSJTx());
    config.setValue("wsjtx/realtimefromwsjtx", UDPPage->getReaDataFromWSJTx());
    config.setValue("wsjtx/infotimeout", UDPPage->getTimeout());
    config.setValue("hamlib/active", hamlibPage->isActive());
    config.setValue("hamlib/readonly", hamlibPage->isReadOnly());

    hamlibPage->saveConfig();

   //qDebug() << "SetupDialog::slotOkButtonClicked - just before leaving" << endl;

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
        logViewFields.removeDuplicates();
        bandModePage->setActiveModes(modes);
        bandModePage->setActiveBands(bands);
        logViewPage->setActiveFields(logViewFields);
    }

    //qDebug() << "SetupDialog::slotReadConfigData - 1" << Qt::endl;

   if ( !processConfigData())
    {
        emit debugLog (Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }


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
    if (logViewFields.isEmpty())
    {
        logViewFields << "qso_date" << "call" << "rst_sent" << "rst_rcvd" << "bandid" << "modeid" << "comment";
    }
    modes.removeDuplicates();
    bandModePage->setActiveModes(modes);
    bands.removeDuplicates();
    bandModePage->setActiveBands(bands);
    logViewFields.removeDuplicates();
    logViewPage->setActiveFields(logViewFields);
    //qDebug() << "SetupDialog::slotReadConfigData - END" << Qt::endl;
    emit debugLog (Q_FUNC_INFO, "END", logSeverity);

}

bool SetupDialog::processConfigData()
{
    //qDebug() << "SetupDialog::processConfigLine: " << endl;
    emit debugLog (Q_FUNC_INFO, "Start", logSeverity);
    KlogConfig config;
    if ( !config.contains("version") )
    {
        return false;
    }
    //LFF
    userDataPage->setStationQrz(config.value("userdata/callsign").toString());
    userDataPage->setOperators(config.value("userdata/operators").toString());
    userDataPage->setCQz(config.value("userdata/cqz").toInt());
    userDataPage->setITUz(config.value("userdata/ituz").toInt());
    readActiveModes(config.value("userdata/modes").toString());
    modes.removeDuplicates();
    bandModePage->setActiveModes(modes);
    miscPage->setRealTime(config.value("realtime").toBool());
    miscPage->setUTCTime(config.value("utctime").toBool());
    miscPage->setAlwaysADIF(config.value("alwaysadif").toBool());
    miscPage->setDefaultFileName(config.value("usedefaultname").toString());
    miscPage->setUseDefaultDBPath(config.value("dbpath").toString());
    miscPage->setDefaultFileName(config.value("defaultadiffile").toString().toLower());
    miscPage->setImperial(config.value("imperialsystem").toBool());
    miscPage->setCompleteWithPrevious(config.value("completewithprevious").toBool());
    miscPage->setSendQSLWhenRec(config.value("sendqslwhenrec").toBool());
    miscPage->setDXMarathon(config.value("managedxmarathon").toBool());
    miscPage->setDebugLog(config.value("debuglog").toBool());
    miscPage->setShowStationCallSignInSearch(config.value("showcallsigninsearch").toBool());
    miscPage->setCheckNewVersions(config.value("checknewversions").toBool());
    miscPage->setReportInfo(config.value("provideinfo").toBool());
    miscPage->setSetEQSLByDefault(config.value("sendeqslbydefault").toBool());
    miscPage->setDupeTime(config.value("duplicatedqsoslot").toInt());

    UDPPage->setUDPServer(config.value("udpserver/active").toBool());
    UDPPage->setNetworkInterface(config.value("udpserver/interface").toString());
    UDPPage->setUDPServerPort(config.value("udpserver/port").toString());
    UDPPage->setLogFromWSJTx(config.value("wsjtx/logfromwsjtx").toBool());
    UDPPage->setAutoLogFromWSJTx(config.value("wsjtx/logautofromwsjtx").toBool());
    UDPPage->setReaDataFromWSJTx(config.value("wsjtx/realtimefromwsjtx").toBool());
    UDPPage->setTimeout(config.value("wsjtx/infotimeout").toString());

    userDataPage->setName(config.value("userdata/name").toString());
    userDataPage->setAddress1(config.value("userdata/address1").toString());
    userDataPage->setAddress2(config.value("userdata/address2").toString());
    userDataPage->setAddress3(config.value("userdata/address3").toString());
    userDataPage->setAddress4(config.value("userdata/address4").toString());
    userDataPage->setCity(config.value("userdata/city").toString());
    userDataPage->setZipCode(config.value("userdata/zipcode").toString());
    userDataPage->setProvince(config.value("userdata/provincestate").toString());
    userDataPage->setCountry(config.value("userdata/country").toString());
    userDataPage->setPower(config.value("userdata/power").toString());
    userDataPage->setRig1(config.value("userdata/rig1").toString());
    userDataPage->setRig2(config.value("userdata/rig2").toString());
    userDataPage->setRig3(config.value("userdata/rig3").toString());
    userDataPage->setAntenna1(config.value("userdata/antenna1").toString());
    userDataPage->setAntenna2(config.value("userdata/antenna2").toString());
    userDataPage->setAntenna3(config.value("userdata/antenna3").toString());
    if ( locator->isValidLocator(config.value("userdata/stationlocator").toString() ))
    {
        userDataPage->setStationLocator(config.value("userdata/stationlocator").toString());
    }

    dxClusterPage->setShowHFQCheckbox(config.value("DXC/showhf").toBool());
    dxClusterPage->setShowVHFQCheckbox(config.value("DXC/showvhf").toBool());
    dxClusterPage->setShowWARCQCheckbox(config.value("DXC/showwarc").toBool());
    dxClusterPage->setShowWorkedQCheckbox(config.value("DXC/showworked").toBool());
    dxClusterPage->setShowConfirmedQCheckbox(config.value("DXC/showconfirmed").toBool());
    dxClusterPage->setShowANNQCheckbox(config.value("DXC/showann").toBool());
    dxClusterPage->setShowWWVQCheckbox(config.value("DXC/showwwn").toBool());
    dxClusterPage->setShowWCYQCheckbox(config.value("DXC/showwcy").toBool());
    dxClusterServers = config.value("DXC/servervariants").toStringList();
    dxClusterServerToUse = config.value("DXC/servertouse").toString();
    dxClusterPage->setSaveActivityQCheckbox(config.value("DXC/save").toBool());

    colorsPage->setNewOneColor(config.value("colors/newone").toString());
    colorsPage->setNeededColor(config.value("colors/needed").toString());
    colorsPage->setWorkedColor(config.value("colors/worked").toString());
    colorsPage->setConfirmedColor(config.value("colors/confirmed").toString());
    colorsPage->setDefaultColor(config.value("colors/default").toString());
    colorsPage->setDarkMode(config.value("colors/darkmode").toBool());

    hamlibPage->setRigType(config.value("hamlib/rigtype").toString());
    hamlibPage->setSerialPort(config.value("hamlib/serialport").toString());
    hamlibPage->setSerialSpeed(config.value("hamlib/serialbauds").toString());
    hamlibPage->setActive(config.value("hamlib/active").toBool());
    hamlibPage->setReadOnly(config.value("hamlib/readonly").toBool());
    hamlibPage->setDataBits(config.value("hamlib/serialdatabits").toString());
    hamlibPage->setStopBits(config.value("hamlib/serialstopbits").toString());
    hamlibPage->setFlowControl(config.value("hamlib/serialflowcontrol").toString());
    hamlibPage->setParity(config.value("hamlib/serialparity").toString());
    hamlibPage->setPollingInterval(config.value("hamlib/pollrate").toInt());
    hamlibPage->setRadioNetworkAddress(config.value("hamlib/netaddress").toString());
    hamlibPage->setRadioNetworkPort(config.value("hamlib/netport").toInt());

    int i = config.value("selectedlog").toInt();

    if (dataProxy->doesThisLogExist(i))
    {
        //qDebug() << "SetupDialog::processConfigLine: dataProxy->doesThisLogExist TRUE" << endl;
    }
    else
    {
        //qDebug() << "SetupDialog::processConfigLine: dataProxy->doesThisLogExist FALSE" << endl;
        i = 0;
        while(!dataProxy->doesThisLogExist(i))
        {
            i++;
        }
    }
    logsPage->setSelectedLog(i);

    eLogPage->setClubLogActive(config.value("clublog/active").toBool());
    eLogPage->setClubLogRealTime(config.value("clublog/realtime").toBool());
    eLogPage->setClubLogPassword(config.value("clublog/pass").toString());
    eLogPage->setClubLogEmail(config.value("clublog/email").toString());

    eLogPage->setEQSLActive(config.value("eqsl/active").toBool());
    eLogPage->setEQSLUser(config.value("eqsl/call").toString());
    eLogPage->setEQSLPassword(config.value("eqsl/pass").toString());

    eLogPage->setQRZCOMActive(config.value("qrzcom/active").toBool());
    eLogPage->setQRZCOMUser(config.value("qrzcom/user").toString());
    eLogPage->setQRZCOMAutoCheck(config.value("qrzcom/auto").toBool());
    eLogPage->setQRZCOMPassword(config.value("qrzcom/pass").toString());
    eLogPage->setQRZCOMLogBookKEY(config.value("qrzcom/logbookkey").toString());

    eLogPage->setLoTWActive(config.value("lotw/active").toBool());
    eLogPage->setTQSLPath(config.value("lotw/path").toString());
    eLogPage->setLoTWUser(config.value("lotw/user").toString());
    eLogPage->setLoTWPass(config.value("lotw/pass").toString());

    miscPage->setDeleteAlwaysAdiFile(config.value("deletealwaysadifile").toBool());
    latestBackup = config.value("latestbackup", QString()).toString();

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
     eLogPage->setQRZCOMAutoCheck(_b);
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
