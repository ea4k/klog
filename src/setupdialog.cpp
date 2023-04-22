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
//#include <QDebug>

/*
This class calls all the othet "Setup..." to manage the configuration

*/

SetupDialog::SetupDialog(DataProxy_SQLite *dp, QWidget *parent)
{
     //qDebug() << Q_FUNC_INFO ;

    Q_UNUSED(parent);
    //contentsWidget->update();
    //pagesWidget->update();
    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);

     //qDebug() << Q_FUNC_INFO << ": 01";

    locator = new Locator();
    tabWidget = new QTabWidget;
     //qDebug() << Q_FUNC_INFO << ": 01.0";
    userDataPage = new SetupPageUserDataPage(dataProxy);
     //qDebug() << Q_FUNC_INFO << ": 01.10";
    bandModePage = new SetupPageBandMode(dataProxy, this);
     //qDebug() << Q_FUNC_INFO << ": 01.20";
    dxClusterPage = new SetupPageDxCluster(this);
     //qDebug() << Q_FUNC_INFO << ": 01.30";
    colorsPage = new SetupPageColors(this);
     //qDebug() << Q_FUNC_INFO << ": 01.40";
    miscPage = new SetupPageMisc(this);
     //qDebug() << Q_FUNC_INFO << ": 01.50";
    worldEditorPage = new SetupPageWorldEditor (dataProxy, this);
     //qDebug() << Q_FUNC_INFO << ": 01.60";
    logsPage = new SetupPageLogs(dataProxy, this);
     //qDebug() << Q_FUNC_INFO << ": 01.70";
    eLogPage = new SetupPageELog(this);
     //qDebug() << Q_FUNC_INFO << ": 01.80";
    UDPPage = new SetupPageUDP(this);
     //qDebug() << Q_FUNC_INFO << ": 01.90";
    satsPage = new SetupPageSats(dataProxy, this);
     //qDebug() << Q_FUNC_INFO << ": 01.100";
    hamlibPage = new SetupPageHamLib(dataProxy, this);
     //qDebug() << Q_FUNC_INFO << ": 01.101";
    logViewPage = new SetupPageLogView(dataProxy, this);
     //qDebug() << Q_FUNC_INFO << ": 02";

    tabWidget->addTab(userDataPage, tr("User data"));
    tabWidget->addTab(bandModePage, tr("Bands/Modes"));
    tabWidget->addTab(logViewPage, tr("Log widget"));
    tabWidget->addTab(dxClusterPage, tr("D&X-Cluster"));
    tabWidget->addTab(colorsPage, tr("Colors"));
    tabWidget->addTab(miscPage, tr("Misc"));
    tabWidget->addTab(worldEditorPage, tr("World Editor"));
    logsPageTabN = tabWidget->addTab(logsPage, tr("Logs"));
    tabWidget->addTab(eLogPage, tr("eLog"));
    tabWidget->addTab(UDPPage, tr("WSJT-X"));
    tabWidget->addTab(satsPage , tr("Satellites"));
     //qDebug() << Q_FUNC_INFO << ": 02.100";
    tabWidget->addTab(hamlibPage, tr ("HamLib"));
     //qDebug() << "SetupDialog::SetupDialog 03";

    closeButton = new QPushButton(tr("Cancel"));
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

     //qDebug() << Q_FUNC_INFO << ": 04";

    setLayout(mainLayout);
    setWindowTitle(tr("Settings"));
    //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupDialog::init(const QString &_softwareVersion, const int _page, const bool _alreadyConfigured)
{
    //qDebug() << Q_FUNC_INFO;
    logLevel = None;
    constrid = 2;
    nolog = true;
    firstTime = true;

    version = QString();
    pageRequested = 0;
    //qDebug() << Q_FUNC_INFO << "00";
    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    userDataPage->setPrefixes ();
    //qDebug() << Q_FUNC_INFO << "000";
    firstTime = !_alreadyConfigured;
    if (firstTime)
    {
         //qDebug() << Q_FUNC_INFO << " - Running for the FIRST TIME";
    }
    version = _softwareVersion;
    pageRequested = _page;

    slotReadConfigData();

    if ((pageRequested==6) && (logsPageTabN>0))// The user is opening a new log
    {
         //qDebug() << Q_FUNC_INFO << ": 5.2";
        tabWidget->setCurrentIndex(logsPageTabN);
    }
     //qDebug() << Q_FUNC_INFO << ": 5.3";
    nolog = !(haveAtleastOneLog());

    connectActions();
     //qDebug() << Q_FUNC_INFO << " - END";
}

SetupDialog::~SetupDialog()
{
     //qDebug() << Q_FUNC_INFO ;
    delete(locator);
    delete(userDataPage);
    delete(bandModePage);
    delete(dxClusterPage);
    delete(miscPage);
    delete(worldEditorPage);
    delete(logsPage);
    delete(eLogPage);
    delete(colorsPage);
    delete(UDPPage);
    delete(satsPage);
    delete(hamlibPage);
    delete(logViewPage);
    delete(util);
}

void SetupDialog::connectActions()
{
     //qDebug() << Q_FUNC_INFO;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOkButtonClicked()));
    connect (logsPage, SIGNAL(newLogData(QStringList)), this, SLOT(slotAnalyzeNewLogData(QStringList)));
    connect(logsPage, SIGNAL(focusOK()), this, SLOT(slotFocusOK()) );
    connect (userDataPage, SIGNAL(mainCallsignSignal(QString)), this, SLOT(slotSetStationCallSign(QString)));
    connect (userDataPage, SIGNAL(operatorsSignal(QString)), this, SLOT(slotSetOperators(QString)));
    connect (userDataPage, SIGNAL(enterKey()), this, SLOT(slotOkButtonClicked()));
    connect (eLogPage, SIGNAL(enterKey()), this, SLOT(slotOkButtonClicked()));

    logEvent(Q_FUNC_INFO, "END", Debug);
}

//void SetupDialog::slotQRZCOMAuto(const bool _b)
//{
    //emit qrzcomAuto(_b);
//}

void SetupDialog::setData(const QString &_softwareVersion, const int _page, const bool _alreadyConfigured)
{
       //qDebug() << "SetupDialog::setData: " << _softwareVersion << "/" << QString::number(_page);
    logEvent(Q_FUNC_INFO, "Start", Debug);
    nolog = !(haveAtleastOneLog());
    firstTime = !_alreadyConfigured;
    if (firstTime)
    {
         //qDebug() << Q_FUNC_INFO << " - Running for the FIRST time";
    }
    else
    {
         //qDebug() << Q_FUNC_INFO << " - Running NOT for the FIRST time";
        miscPage->setUseDefaultDBPath(miscPage->getDefaultDBPath());
    }

    setSoftVersion(_softwareVersion);
    slotReadConfigData ();
    setPage(_page);
    //removeBandModeDuplicates();
    logEvent(Q_FUNC_INFO, "END", Debug);
       //qDebug() << "SetupDialog::setData - END";
}

void SetupDialog::setSoftVersion(const QString &_softwareVersion)
{
     //qDebug() << "SetupDialog::setSoftVersion";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    version = _softwareVersion;
    logEvent(Q_FUNC_INFO, "END", Debug);
}


void SetupDialog::setPage(const int _page)
{
        //qDebug() << "SetupDialog::setPage("<<QString::number(_page) << ")";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    pageRequested = _page;

    if ((pageRequested==6) && (logsPageTabN>0))// The user is opening a new log
    {
        tabWidget->setCurrentIndex(pageRequested);
    }
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::slotCancelButtonClicked()
{
       //qDebug() << "SetupDialog::slotCancelButtonClicked";
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
                emit debugLog (Q_FUNC_INFO, "END-1", logLevel);
                emit exitSignal(2);
                return;
            }
            else
            {
                emit debugLog (Q_FUNC_INFO, "END-2", logLevel);
                return;
            }
        }
    }
    hamlibPage->stopHamlib();
    QDialog::reject ();
    close();
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::createIcons()
{
        //qDebug() << "SetupDialog::createIcons";
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
        //qDebug() << "SetupDialog::changePage";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
    logEvent(Q_FUNC_INFO, "END", Debug);
}
void SetupDialog::loadDarkMode()
{// Reads the config to setup the DarkMode
    //qDebug() << Q_FUNC_INFO;
    colorsPage->loadDarkMode ();
}

bool SetupDialog::loadSettings()
{
    if (!QFile::exists(util->getSetFile ()))
    {
         //qDebug() << Q_FUNC_INFO << " - Nothing to load";
        return false;
    }
    QSettings settings(util->getSetFile (), QSettings::IniFormat);

     //qDebug() << Q_FUNC_INFO << " - 10 - General";
    version = settings.value ("Version").toString();
    //latestBackup = settings.value ("LatestBackup").toString ();

     //qDebug() << Q_FUNC_INFO << " - 20 - user";
    userDataPage->loadSettings();

     //qDebug() << Q_FUNC_INFO << " - 30 - bands";
    settings.beginGroup ("BandMode");

    QStringList listAux;
    listAux.clear();
    listAux << "SSB" << "CW" << "RTTY";
    readActiveModes (settings.value("Modes", listAux).toStringList ());
    listAux.clear();
    listAux << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    readActiveBands (settings.value("Bands", listAux).toStringList ());


    modes.removeDuplicates();
    bandModePage->setActiveModes(modes);

    bands.removeDuplicates();
    bandModePage->setActiveBands (bands);
    settings.endGroup ();

     //qDebug() << Q_FUNC_INFO << " - 40 - logview";
    logViewPage->loadSettings ();
    logViewFields.clear();
    logViewFields << logViewPage->getActiveFields ();
     //qDebug() << Q_FUNC_INFO << " - 50 - dxcluster";
    dxClusterPage->loadSettings ();
     //qDebug() << Q_FUNC_INFO << " - 60 - colors";
    colorsPage->loadSettings ();
     //qDebug() << Q_FUNC_INFO << " - 70 - misc";
    miscPage->loadSettings ();
     //qDebug() << Q_FUNC_INFO << " - 80 - logs";
    logsPage->loadSettings();
     //qDebug() << Q_FUNC_INFO << " - 90 - elog";
    eLogPage->loadSettings ();
     //qDebug() << Q_FUNC_INFO << " - 100 - UDP";
    UDPPage->loadSettings ();
     //qDebug() << Q_FUNC_INFO << " - 110 - Sats";
    hamlibPage->loadSettings ();
     //qDebug() << Q_FUNC_INFO << " - 120 - HamLib";
    return true;
}

void SetupDialog::saveSettings()
{
     //qDebug() << Q_FUNC_INFO << " - Start";
    //QSettings settings(util->getSetFile (), QSettings::IniFormat);

    userDataPage->saveSettings();       // Groups done
    bandModePage->saveSettings ();      // Groups done
    logViewPage->saveSettings ();
    dxClusterPage->saveSettings ();
    miscPage->saveSettings ();
    colorsPage->saveSettings ();
    logsPage->saveSettings();
    eLogPage->saveSettings ();
    UDPPage->saveSettings ();
    hamlibPage->saveSettings ();
     //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupDialog::slotOkButtonClicked()
{
     //qDebug() << Q_FUNC_INFO ;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    if (!miscPage->areDBPathChangesApplied())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("DB has not been moved to new path."));
        msgBox.setInformativeText(tr("Go to the Misc tab and click on Move DB\n or the DB will not be moved to the new location."));
        msgBox.exec();
        emit debugLog (Q_FUNC_INFO, "END-1", logLevel);
        return;
    }

    if (!util->isValidCall(userDataPage->getMainCallsign())){ //
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You need to enter at least a valid callsign."));
        msgBox.setInformativeText(tr("Go to the User tab and enter valid callsign."));
        msgBox.exec();
        emit debugLog (Q_FUNC_INFO, "END-2", logLevel);
        return;
    }

    if (!haveAtleastOneLog())
    {
         //qDebug() << "SetupDialog::slotOkButtonClicked - NO LOG!";
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You have not selected the kind of log you want."));
        msgBox.setInformativeText(tr("You will be redirected to the Log tab.\nPlease add and select the kind of log you want to use."));
        msgBox.exec();

        tabWidget->setCurrentIndex(tabWidget->indexOf(logsPage));
        logsPage->createNewLog();
        //emit newLogRequested(true); // Signal to be catched by logsPage true show new log
         //qDebug() << Q_FUNC_INFO << "END-3" ;
        emit debugLog (Q_FUNC_INFO, "END-3", logLevel);
        return;
    }
     //qDebug() << "SetupDialog::slotOkButtonClicked - 10";
    saveSettings();

    hamlibPage->stopHamlib();
     //qDebug() << "SetupDialog::slotOkButtonClicked - just before leaving";
    QDialog::accept();
    logEvent(Q_FUNC_INFO, "END", Debug);
     //qDebug() << "SetupDialog::slotOkButtonClicked - END";
    close();
}

void SetupDialog::slotReadConfigData()
{
     //qDebug() << Q_FUNC_INFO << " - Start";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (firstTime)
    //if (!QFile::exists (util->getSetFile ()))
    {
         //qDebug() << Q_FUNC_INFO << " - FirstTime";
        setDefaults();
        bands.removeDuplicates();
        modes.removeDuplicates();
        logViewFields.removeDuplicates();
        bandModePage->setActiveModes(modes);
        bandModePage->setActiveBands(bands);
        logViewPage->setActiveFields(logViewFields);
    }
    loadSettings();

    //dxClusterPage->setDxclusterServersComboBox(dxClusterServers);
    //dxClusterPage->setSelectedDxClusterServer(dxClusterServerToUse);

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
     //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::readActiveBands (const QStringList &actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
    // bands used in the program
       //qDebug() << "SetupDialog::readActiveBands: " << actives << QT_ENDL;

    logEvent(Q_FUNC_INFO, "Start", Debug);
    bool atLeastOne = false;

    QStringList values = actives;
    QStringList _abands;

    for (int i = 0; i < values.size() ; i++)
    {
        if (isValidBand(values.at(i)))
        {
            if (!atLeastOne)
            {
                atLeastOne = true;
                _abands.clear();
            }
            _abands << values.at(i);
        }
    }

    bands.clear();

    bands << dataProxy->getBandsInLog(-1);
    bands << _abands;
    bands.removeDuplicates();
     //qDebug() << Q_FUNC_INFO << " - END";
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::readActiveModes (const QStringList &actives)
{
     //qDebug() << "SetupDialog::readActiveModes: " << actives;
    logEvent(Q_FUNC_INFO, "Start", Debug);

    bool atLeastOne = false;
    QStringList _amodes;

    QStringList values = actives;
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
    logEvent(Q_FUNC_INFO, "END", Debug);
        //qDebug() << "SetupDialog::readActiveModes: " << modes.join(" / ");
}

bool SetupDialog::isValidBand (const QString &b)
{
        //qDebug() << "SetupDialog::isValidBand: "<< b;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    logEvent(Q_FUNC_INFO, "END", Debug);
    return query.isValid();
}
bool SetupDialog::isValidMode (const QString &b)
{
        //qDebug() << "SetupDialog::isValidMode: " << b;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QString stringQuery = QString("SELECT id FROM mode WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    logEvent(Q_FUNC_INFO, "END", Debug);
    return query.isValid();
}

void SetupDialog::setDefaults()
{
        //qDebug() << "SetupDialog::setDefaults";
    logEvent(Q_FUNC_INFO, "Start", Debug);
    miscPage->setRealTime("TRUE");
    miscPage->setUTCTime("TRUE");
    miscPage->setImperial("FALSE"); //Metric system is the default
    miscPage->setAlwaysADIF("FALSE");
    miscPage->setSendQSLWhenRec("TRUE");
    miscPage->setShowStationCallSignInSearch("TRUE");
    miscPage->setCheckNewVersions("TRUE");
    //miscPage->setReportInfo("FALSE");
    miscPage->setDXMarathon("FALSE");
    miscPage->setDebugLogLevel(util->getDebugLevels().at(0));
    //miscPage->setLogSort("FALSE");
    miscPage->setSetEQSLByDefault("TRUE");
    miscPage->setCheckCalls (true);

    UDPPage->setUDPServer(false);
    UDPPage->setUDPServerPort(2237);
    UDPPage->setTimeout(2000);
    UDPPage->setLogFromWSJTx(false);
    UDPPage->setReaDataFromWSJTx(false);
    UDPPage->setAutoLogFromWSJTx(false);
    //interfacesWindowsPage->setSendToPSTRotator("FALSE");
    //interfacesWindowsPage->setPSTRotatorUDPServer("locahost");
    //interfacesWindowsPage->setPSTRotatorUDPServerPort("12040");
    dxClusterPage->init ();

    //dxClusterServers.clear();
    //dxClusterServers.append("dxfun.com:8000");
    //dxClusterServerToUse = "dxfun.com:8000";

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
    logEvent(Q_FUNC_INFO, "END", Debug);
}

QString SetupDialog::checkAndFixASCIIinADIF(const QString &_data)
{
        //qDebug() << "SetupDialog::checkAndFixASCIIinADIF " << _data;
//TODO: this function is also in the FileManager class. Maybe I should call that one and keep just one copy
    logEvent(Q_FUNC_INFO, "Start", Debug);
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
            //qDebug() << "SetupDialog::checkAndFixunicodeinADIF: " << st.at(i) <<" = " << QString::number(unicodeVal);
    }

    // Show into another lineEdit

    logEvent(Q_FUNC_INFO, "END", Debug);
    return newString;
}

bool SetupDialog::haveAtleastOneLog()
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    emit debugLog (Q_FUNC_INFO, "END-1", logLevel);
    return dataProxy->haveAtLeastOneLog();
    //logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::setClubLogActive(const bool _b)
{
    logEvent(Q_FUNC_INFO, "Start", Debug);
    eLogPage->setClubLogActive(_b);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::checkIfNewBandOrMode()
{
       //qDebug() << "SetupDialog::checkIfNewBandOrMode: logLevel: " << QString::number(logLevel);
    logEvent(Q_FUNC_INFO, "Start", Debug);
    QStringList _items;

    _items.clear();
        //qDebug() << "SetupDialog::checkIfNewBandOrMode -1";
    _items << dataProxy->getBandsInLog(-1);
        //qDebug() << "SetupDialog::checkIfNewBandOrMode -2";
    _items << bandModePage->getBands();
        //qDebug() << "SetupDialog::checkIfNewBandOrMode -3";
    _items.removeDuplicates();
        //qDebug() << "SetupDialog::checkIfNewBandOrMode -4";
    bandModePage->setActiveBands(_items);
        //qDebug() << "SetupDialog::checkIfNewBandOrMode -5";

    _items.clear();
    _items << dataProxy->getModesInLog(-1);
    _items << bandModePage->getModes();
    _items.removeDuplicates();
    bandModePage->setActiveModes(_items);
    logEvent(Q_FUNC_INFO, "END", Debug);
        //qDebug() << "SetupDialog::checkIfNewBandOrMode END";
}


void SetupDialog::slotAnalyzeNewLogData(const QStringList _qs)
{
       //qDebug() << "SetupDialog::slotAnalyzeNewLogData (length=" << QString::number(_qs.length()) << ")";
        //qDebug() << "SetupDialog::slotAnalyzeNewLogData";
 // We receive the station callsign and operators from the logs tab
    logEvent(Q_FUNC_INFO, "Start", Debug);
    if (_qs.length()!=2)
    {
        emit debugLog (Q_FUNC_INFO, "END-1", logLevel);
        return;
    }
    userDataPage->setMainCallsign(_qs.at(0));
    userDataPage->setOperators(_qs.at(1));
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::slotSetStationCallSign(const QString &_p)
{
        //qDebug() << "SetupDialog::slotSetStationCallSign: " << _p;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    logsPage->setDefaultStationCallsign(_p);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::slotSetOperators(const QString &_p)
{
        //qDebug() << "SetupDialog::slotSetOperators: " << _p;
    logEvent(Q_FUNC_INFO, "Start", Debug);
    logsPage->setDefaultOperators(_p);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::setLogLevel(const DebugLogLevel _sev)
{
    logLevel = _sev;
    miscPage->setDebugLogLevel(util->debugLevelToString(_sev));
}

void SetupDialog::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery)
{   logEvent(Q_FUNC_INFO, "Start", Debug);
    emit queryError(functionFailed, errorCodeS, nativeError, failedQuery);
    logEvent(Q_FUNC_INFO, "END", Debug);
}

void SetupDialog::slotFocusOK()
{
    okButton->setFocus(Qt::OtherFocusReason);
}

void SetupDialog::showEvent(QShowEvent *event)
{
     //qDebug() << Q_FUNC_INFO;
     //qDebug() << Q_FUNC_INFO << " - selectedLog: " << QString::number(logsPage->getSelectedLog());
    QWidget::showEvent(event);
    eLogPage->loadSettings ();
    miscPage->loadSettings ();
    userDataPage->setStationFocus();
}

void SetupDialog::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    if (logLevel<=_level)
        emit debugLog (_func, _msg, _level);
}
