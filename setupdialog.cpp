/***************************************************************************
                          setupdialog.cpp  -  description
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

#include "setupdialog.h"
#include <QDebug>

/*
This class calls all the othet "Setup..." to manage the configuration

*/

SetupDialog::SetupDialog(const bool _firstTime)
{
    //qDebug() << "SetupDialog::SetupDialog 1" << endl;
    nolog = true;
    configFileName = "klogrc";
    version = ".";
    pageRequested = 0;
    dataProxy = new DataProxy_SQLite();

    firstTime = _firstTime;
    if (firstTime)
    {
        qDebug() << "SetupDialog::SetupDialog FIRST TIME = TRUE" << endl;
    }
    else
    {
        qDebug() << "SetupDialog::SetupDialog FIRST TIME = FALSE" << endl;
    }

    logsPageTabN=-1;

    locator = new Locator();

    tabWidget = new QTabWidget;


    userDataPage = new SetupPageUserDataPage();
    bandsModesPage = new SetupPageBandsModes(this);
    dxClusterPage = new SetupPageDxCluster(this);
    colorsPage = new SetupPageColors(this);
    miscPage = new SetupPageMisc(this);
    worldEditorPage = new SetupPageWorldEditor (this);

    logsPage = new SetupPageLogs(this);

    tabWidget->addTab(userDataPage, tr("My Data"));
    tabWidget->addTab(bandsModesPage, tr("Bands/Modes"));
    tabWidget->addTab(dxClusterPage, tr("DX-Cluster"));
    tabWidget->addTab(colorsPage, tr("Colors"));
    tabWidget->addTab(miscPage, tr("Misc"));

    tabWidget->addTab(worldEditorPage, tr("World Editor"));
    logsPageTabN = tabWidget->addTab(logsPage, tr("Logs"));

    QPushButton *closeButton = new QPushButton(tr("Cancel"));
    QPushButton *okButton = new QPushButton(tr("OK"));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOkButtonClicked()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(tabWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));

    slotReadConfigData();

    if ((pageRequested==6) && (logsPageTabN>0))// The user is opening a new log
    {
        tabWidget->setCurrentIndex(logsPageTabN);
    }
    nolog = !(haveAtleastOneLog());
}



SetupDialog::SetupDialog(const QString _configFile, const QString _softwareVersion, const int _page, const bool _firstTime)
{
    //qDebug() << "SetupDialog::SetupDialog 2" << endl;
    firstTime = _firstTime;
    dataProxy = new DataProxy_SQLite();
    configFileName = _configFile;
    version = _softwareVersion;
    pageRequested = _page;
    int logsPageTabN=-1;

    locator = new Locator();

    tabWidget = new QTabWidget;

    userDataPage = new SetupPageUserDataPage();
    bandsModesPage = new SetupPageBandsModes(this);
    dxClusterPage = new SetupPageDxCluster(this);
    colorsPage = new SetupPageColors(this);
    miscPage = new SetupPageMisc(this);
    worldEditorPage = new SetupPageWorldEditor (this);

    logsPage = new SetupPageLogs(this);

    tabWidget->addTab(userDataPage, tr("User data"));
    tabWidget->addTab(bandsModesPage, tr("Bands/Modes"));
    tabWidget->addTab(dxClusterPage, tr("DX-Cluster"));
    tabWidget->addTab(colorsPage, tr("Colors"));
    tabWidget->addTab(miscPage, tr("Misc"));

    tabWidget->addTab(worldEditorPage, tr("World Editor"));
    logsPageTabN = tabWidget->addTab(logsPage, tr("Logs"));

    QPushButton *closeButton = new QPushButton(tr("Cancel"));
    QPushButton *okButton = new QPushButton(tr("OK"));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOkButtonClicked()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(tabWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));

    slotReadConfigData();

    if ((pageRequested==6) && (logsPageTabN>0))// The user is opening a new log
    {
        tabWidget->setCurrentIndex(logsPageTabN);
    }

}

SetupDialog::~SetupDialog()
{
    //qDebug() << "SetupDialog::~SetupDialog " << endl;
}

void SetupDialog::setData(const QString _configFile, const QString _softwareVersion, const int _page, const bool _firstTime)
{
    qDebug() << "SetupDialog::setData: " << QString::number(_page) << endl;
    firstTime = _firstTime;
    if (firstTime)
    {
        //qDebug() << "SetupDialog::setData FIRST TIME! " << endl;
    }
    else
    {
        //qDebug() << "SetupDialog::setData NOT FIRST TIME! " << endl;
    }

    setConfigFile(_configFile);
    setSoftVersion(_softwareVersion);
    setPage(_page);

}

void SetupDialog::setConfigFile(const QString _configFile)
{
    //qDebug() << "SetupDialog::setConfigFile" << endl;
    configFileName = _configFile;
}

void SetupDialog::setSoftVersion(const QString _softwareVersion)
{
    //qDebug() << "SetupDialog::setSoftVersion" << endl;
    version = _softwareVersion;
}


void SetupDialog::setPage(const int _page)
{
    //qDebug() << "SetupDialog::setPage("<<QString::number(_page) << ")" << endl;
    pageRequested = _page;

    if ((pageRequested==6) && (logsPageTabN>0))// The user is opening a new log
    {
        tabWidget->setCurrentIndex(pageRequested);
    }
}

void SetupDialog::slotCancelButtonClicked()
{

    //qDebug() << "SetupDialog::slotCancelButtonClicked" << endl;
    if (firstTime || nolog)
    {
        if (nolog)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("You need to enter at least one log in the Log's tab."));
            msgBox.exec();
            return;
        }
        emit exitSignal(2);
    }

    //
    close();
}

void SetupDialog::createIcons()
{
    //qDebug() << "SetupDialog::createIcons" << endl;
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
}

void SetupDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    //qDebug() << "SetupDialog::changePage" << endl;
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}



void SetupDialog::slotOkButtonClicked()
{
//qDebug() << "SetupDialog::slotOkButonClicked" << endl;

    if ((userDataPage->getStationQrz()).length() < 3){ // There are no valid calls with less than 3 Chars
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You need to enter at least a valid QRZ."));
        msgBox.exec();
        return;
    }

    if (!haveAtleastOneLog())
    {
        qDebug() << "SetupDialog::slotOkButonClicked - NO LOG!" << endl;

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("You have not selected the kind of log you want.\nYou will be redirected to the Log tab.\nPlease add and select the kind of log you want to use."));
        msgBox.exec();
        tabWidget->setCurrentIndex(logsPageTabN);
        return;
    }


    QFile file (configFileName);
    QString tmp;
    tmp = "true";
    if (file.open (QIODevice::WriteOnly)){
        QTextStream stream (&file);
/*QString stationCall;
    int contest;
    int contestCategory;
    int modes;*/
//QRZ/CQ/ITU/CONTEST
        stream << "version=" << version << ";" << endl;        
        stream << "callsign="  << userDataPage->getStationQrz() << ";" << endl;
        if ((userDataPage->getOperators()).length() >= 3){ // There are no valid calls with less than 3 Chars
            stream << "operators="  << userDataPage->getOperators() << ";" << endl;
        }
        //stream << "contest="  << userDataPage->getContest()  <<   ";" << endl;
        //stream << "contestcategory=" << userDataPage->getContestCategory() <<  ";" <<  endl;
        stream << "cqz=" << QString::number(userDataPage->getCQz()) <<  ";" <<  endl;
        stream << "ituz=" << QString::number(userDataPage->getITUz()) <<  ";" <<  endl;

        if ( locator->isValidLocator(userDataPage->getStationLocator()) )
        {
            stream << "StationLocator=" << userDataPage->getStationLocator() << ";" << endl;
        }


        if ((!(userDataPage->getName()).isNull()) && (  (userDataPage->getName()).length() > 0   ))
        {
            stream << "Name=" << userDataPage->getName() <<";" << endl;
        }
        if ((!(userDataPage->getAddress1()).isNull()) && (  (userDataPage->getAddress1()).length() > 0   ))
        {
            stream << "Address1=" << userDataPage->getAddress1() <<";" << endl;
        }
        if ((!(userDataPage->getAddress2()).isNull())  && (  (userDataPage->getAddress2()).length() > 0   ))
        {
            stream << "Address2=" << userDataPage->getAddress2() <<";" << endl;
        }
        if ((!(userDataPage->getAddress3()).isNull()) && (  (userDataPage->getAddress3()).length() > 0   ))
        {
            stream << "Address3=" << userDataPage->getAddress3() <<";" << endl;
        }
        if ((!(userDataPage->getAddress4()).isNull()) && (  (userDataPage->getAddress4()).length() > 0   ))
        {
            stream << "Address4=" << userDataPage->getAddress4() <<";" << endl;
        }

        if ((!(userDataPage->getCity()).isNull()) && (  (userDataPage->getCity()).length() > 0   ))
        {
            stream << "City=" << userDataPage->getCity() <<";" << endl;
        }
        if ((!(userDataPage->getZipCode()).isNull()) && (  (userDataPage->getZipCode()).length() > 0   ))
        {
            stream << "ZipCode=" << userDataPage->getZipCode() <<";" << endl;
        }
        if ((!(userDataPage->getProvince()).isNull()) && (  (userDataPage->getProvince()).length() > 0   ))
        {
            stream << "ProvinceState=" << userDataPage->getProvince() <<";" << endl;
        }
        if ((!(userDataPage->getCountry()).isNull()) && (  (userDataPage->getCountry()).length() > 0   ))
        {
            stream << "Country=" << userDataPage->getCountry() <<";" << endl;
        }
        if ((!(userDataPage->getRig1()).isNull()) && (  (userDataPage->getRig1()).length() > 0   ))
        {
            stream << "Rig1=" << userDataPage->getRig1() <<";" << endl;
        }
        if ((!(userDataPage->getRig2()).isNull()) && (  (userDataPage->getRig2()).length() > 0   ))
        {
            stream << "Rig2=" << userDataPage->getRig2() <<";" << endl;
        }
        if ((!(userDataPage->getRig3()).isNull()) && (  (userDataPage->getRig3()).length() > 0   ))
        {
            stream << "Rig3=" << userDataPage->getRig3() <<";" << endl;
        }
        if ((!(userDataPage->getAntenna1()).isNull()) && (  (userDataPage->getAntenna1()).length() > 0   ))
        {
            stream << "Antenna1=" << userDataPage->getAntenna1() <<";" << endl;
        }
        if ((!(userDataPage->getAntenna2()).isNull()) && (  (userDataPage->getAntenna2()).length() > 0   ))
        {
            stream << "Antenna2=" << userDataPage->getAntenna2() <<";" << endl;
        }
        if ((!(userDataPage->getAntenna3()).isNull()) && (  (userDataPage->getAntenna2()).length() > 0   ))
        {
            stream << "Antenna3=" << userDataPage->getAntenna3() <<";" << endl;
        }
        if ((userDataPage->getPower()).toFloat()>=0)
        {
            stream << "Power=" << userDataPage->getPower() << ";" << endl;
        }

        //stream << "locator=" << (MyLocatorkLineEdit->text ()).toUpper () <<  ";" << endl;
        //stream << "CallUsed=" << (UserDataPage.qrzLineEdit).text() <<  ";" << endl;
        //stream << "Operators=" <<  ";" << endl;
        stream << "Bands=" << bandsModesPage->getBands() << ";" <<  endl;
        stream << "Modes=" << bandsModesPage->getModes() << ";" <<  endl;

        stream << "InMemory=" << miscPage->getInMemory() << ";" <<  endl;
        stream << "RealTime=" << miscPage->getRealTime() << ";" <<  endl;
        stream << "UTCTime=" << miscPage->getUTCTime() << ";" <<  endl;
        stream << "AlwaysADIF=" << miscPage->getAlwaysADIF() << ";" <<  endl;
        stream << "UseDefaultName=" << miscPage->getUseDefaultName() << ";" <<  endl;
        stream << "DefaultADIFFile=" << miscPage->getDefaultFileName() << ";" <<  endl;
        stream << "ImperialSystem=" << miscPage->getImperial() << ";" <<  endl;
        stream << "SendQSLWhenRec=" << miscPage->getSendQSLWhenRec() << ";" <<  endl;
        stream << "ShowCallsignInSearch=" << miscPage->getShowStationCallSignInSearch() << ";" <<  endl;
        stream << "KeepMyData=" << miscPage->getKeepMyData() << ";" <<  endl;



        if ((!(dxClusterPage->getSelectedDxClusterServer()).isNull()) && (  (dxClusterPage->getSelectedDxClusterServer()).length() > 0   ))
        {
            stream << "DXClusterServerToUse=" << dxClusterPage->getSelectedDxClusterServer() <<";" << endl;
        }

        QStringList stringList;
        stringList.clear();
        stringList << dxClusterPage->getDxclusterServersComboBox();

        if (stringList.count()>0)
        { 
            for (int i = 0; i < stringList.count(); i++)
            {
                 stream << "DXClusterServerPort="<< stringList.at(i) << ";" << endl;
            }
        }

        stream << "DXClusterShowHF=" << dxClusterPage->getShowHFRadiobutton() << ";" <<  endl;
        stream << "DXClusterShowVHF=" << dxClusterPage->getShowVHFRadiobutton() << ";" <<  endl;
        stream << "DXClusterShowWARC=" << dxClusterPage->getShowWARCRadiobutton() << ";" <<  endl;
        stream << "DXClusterShowWorked=" << dxClusterPage->getShowWorkedRadiobutton() << ";" <<  endl;
        stream << "DXClusterShowConfirmed=" << dxClusterPage->getShowConfirmedRadiobutton() << ";" <<  endl;
        stream << "DXClusterShowAnn=" << dxClusterPage->getShowANNRadiobutton() << ";" <<  endl;
        stream << "DXClusterShowWWV=" << dxClusterPage->getShowWWVRadiobutton() << ";" <<  endl;
        stream << "DXClusterShowWCY=" << dxClusterPage->getShowWCYRadiobutton() << ";" <<  endl;

        stream << "NewOneColor=" << colorsPage->getNewOneColor() << ";" <<  endl;
        stream << "NeededColor=" << colorsPage->getNeededColor() << ";" <<  endl;
        stream << "WorkedColor=" << colorsPage->getWorkedColor() << ";" <<  endl;
        stream << "ConfirmedColor=" << colorsPage->getConfirmedColor() << ";" <<  endl;
        stream << "DefaultColor=" << colorsPage->getDefaultColor() << ";" <<  endl;
        stream << "SelectedLog=" << QString::number(logsPage->getSelectedLog()) << ";" <<  endl;


        file.close ();
    }

    QDialog::accept();
    //close();
}

void SetupDialog::slotReadConfigData()
{
    //qDebug() << "SetupDialog::slotReadConfigData" << endl;

    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //qDebug() << "SetupDialog::slotReadConfigData() File not found" << configFileName << endl;
        //firstTime = true;
        return;
    }

    //dxClusterServers.clear();

    setDefaults();

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        processConfigLine(line);
    }

    dxClusterPage->setDxclusterServersComboBox(dxClusterServers);
    dxClusterPage->setSelectedDxClusterServer(dxClusterServerToUse);

}

bool SetupDialog::processConfigLine(const QString _line)
{

    //qDebug() << "SetupDialog::processConfigLine: " << _line << endl;

    QString line = (_line.toUpper()).simplified();
    //line.simplified();

    QStringList values = line.split("=", QString::SkipEmptyParts);



    if (line.startsWith('#')){
        //qDebug() << "SetupDialog::processConfigLine: Comment Line!" << endl;
        return true;
    }
    if (!( (line.contains('=')) && (line.contains(';')))){
        //qDebug() << "SetupDialog::processConfigLine: Wrong Line!" << endl;
        return false;
    }
    QString value = values.at(1);
    int endValue = value.indexOf(';');
    if (endValue>-1){

        value = value.left(value.length() - (value.length() - endValue));
    }
    value = checkAndFixASCIIinADIF(value); // Check whether the value is valid.

    if (values.at(0) == "CALLSIGN"){
        //qDebug() << "SetupDialog::processConfigLine: CALLSIGN: " << value << endl;
        userDataPage->setStationQrz(value);
    }else if (values.at(0) == "OPERATORS"){
        userDataPage->setOperators(value);
    }else if (values.at(0)=="CQZ"){
        userDataPage->setCQz((value).toInt());

    }else if (values.at(0)=="ITUZ"){
        userDataPage->setITUz((value).toInt());
    }else if (values.at(0)=="CONTEST"){
        //userDataPage->setContest(value);
    }else if (values.at(0)=="MODES"){
        readActiveModes(value);
        bandsModesPage->setActiveModes(modes);
    }else if (values.at(0)=="BANDS"){
        readActiveBands(value);
        bandsModesPage->setActiveBands(bands);
    }else if (values.at(0)=="INMEMORY"){
        miscPage->setInMemory(value);
    }else if (values.at(0)=="REALTIME"){
        miscPage->setRealTime(value);
    }else if (values.at(0)=="UTCTIME"){
        miscPage->setUTCTime(value);
    }else if (values.at(0)=="ALWAYSADIF"){
        miscPage->setAlwaysADIF(value);
    }else if (values.at(0)=="USEDEFAULTNAME"){
        miscPage->setDefaultFileName(value);
    }else if (values.at(0)=="DEFAULTADIFFILE"){
        miscPage->setDefaultFileName(value.toLower());
    }else if (values.at(0)=="IMPERIALSYSTEM"){
        miscPage->setImperial(value.toLower());
    }else if (values.at(0)=="KEEPMYDATA"){
        miscPage->setKeepMyData(value.toLower());
    }else if (values.at(0)=="SENDQSLWHENREC"){
        miscPage->setSendQSLWhenRec(value.toLower());
    }
    else if (values.at(0)=="SHOWCALLSIGNINSEARCH"){
        miscPage->setShowStationCallSignInSearch(value.toLower());
    }
    else if (values.at(0)=="NAME")
    {
        userDataPage->setName(value);
    }
    else if (values.at(0)=="ADDRESS1")
    {
        userDataPage->setAddress1(value);
    }
    else if (values.at(0)=="ADDRESS2")
    {
        userDataPage->setAddress2(value);
    }
    else if (values.at(0)=="ADDRESS3")
    {
        userDataPage->setAddress3(value);
    }
    else if (values.at(0)=="ADDRESS4")
    {
        userDataPage->setAddress4(value);
    }
    else if (values.at(0)=="CITY")
    {
        userDataPage->setCity(value);
    }
    else if (values.at(0)=="ZIPCODE")
    {
        userDataPage->setZipCode(value);
    }
    else if (values.at(0)=="PROVINCESTATE")
    {
        userDataPage->setProvince(value);
    }
    else if (values.at(0)=="COUNTRY")
    {
        userDataPage->setCountry(value);
    }
    else if (values.at(0)=="POWER")
    {
        userDataPage->setPower(value);
    }
    else if (values.at(0)=="RIG1")
    {
        userDataPage->setRig1(value);
    }
    else if (values.at(0)=="RIG2")
    {
        userDataPage->setRig2(value);
    }
    else if (values.at(0)=="RIG3")
    {
        userDataPage->setRig3(value);
    }

    else if (values.at(0)=="ANTENNA1")
    {
        userDataPage->setAntenna1(value);
    }
    else if (values.at(0)=="ANTENNA2")
    {
        userDataPage->setAntenna2(value);
    }
    else if (values.at(0)=="ANTENNA3")
    {
        userDataPage->setAntenna3(value);
    }
    else if (values.at(0)=="STATIONLOCATOR"){

        if ( locator->isValidLocator(value) )
        {
            userDataPage->setStationLocator(value);
        }

    }else if (values.at(0)=="DXCLUSTERSHOWHF"){
        dxClusterPage->setShowHFRadiobutton(value);
    }else if (values.at(0)=="DXCLUSTERSHOWVHF"){
        dxClusterPage->setShowVHFRadiobutton(value);
    }else if (values.at(0)=="DXCLUSTERSHOWWARC"){
        dxClusterPage->setShowWARCRadiobutton(value);
    }else if (values.at(0)=="DXCLUSTERSHOWWORKED"){
        dxClusterPage->setShowWorkedRadiobutton(value);
    }else if (values.at(0)=="DXCLUSTERSHOWCONFIRMED"){
        dxClusterPage->setShowConfirmedRadiobutton(value);
    }else if (values.at(0)=="DXCLUSTERSHOWANN"){
        dxClusterPage->setShowANNRadiobutton(value);
    }else if (values.at(0)=="DXCLUSTERSHOWWWV"){
        dxClusterPage->setShowWWVRadiobutton(value);
    }else if (values.at(0)=="DXCLUSTERSHOWWCY"){
        dxClusterPage->setShowWCYRadiobutton(value);
    }else if(values.at(0)=="DXCLUSTERSERVERPORT"){
        dxClusterServers << value;
        //qDebug() << "SetupDialog::processConfigLine: dxClusterServers: " << dxClusterServers.last() << endl;

    }else if (values.at(0) =="DXCLUSTERSERVERTOUSE"){
        dxClusterServerToUse=value;
    }
    else if(values.at(0)=="NEWONECOLOR"){
        colorsPage->setNewOneColor(value);
    }else if(values.at(0)=="NEEDEDCOLOR"){
        colorsPage->setNeededColor(value);
    }else if(values.at(0)=="WORKEDCOLOR"){
        colorsPage->setWorkedColor(value);
    }else if(values.at(0)=="CONFIRMEDCOLOR"){
        colorsPage->setConfirmedColor(value);
    }else if(values.at(0)=="DEFAULTCOLOR"){
        colorsPage->setDefaultColor(value);
    }else if(values.at(0)=="SELECTEDLOG"){
        logsPage->setSelectedLog(value.toInt());

    }else{
        //qDebug() << "SetupDialog::processConfigLine: NONE: " << endl;
    }



    // Lines are: Option = value;


    return true;


}

void SetupDialog::readActiveBands (const QString actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
    // bands used in the program
    //qDebug() << "SetupDialog::readActiveBands: " << actives << endl;

    bool atLeastOne = false;

    QStringList values = actives.split(", ", QString::SkipEmptyParts);

    for (int i = 0; i < values.count() ; i++)
    {
        if (isValidBand(values.at(i)))
        {
            if (!atLeastOne)
            {
                //qDebug() << "SetupDialog::readActiveBands (at least One!): " << values.at(i) << endl;
                atLeastOne = true;
                bands.clear();
            }

            bands << values.at(i);
            //qDebug() << "SetupDialog::readActiveBands: " << values.at(i) << endl;
        }

    }
}

void SetupDialog::readActiveModes (const QString actives)
{
    //qDebug() << "SetupDialog::readActiveModes: " << actives << endl;

    bool atLeastOne = false;

    QStringList values = actives.split(", ", QString::SkipEmptyParts);

    for (int i = 0; i < values.count() ; i++)
    {
        if (isValidMode(values.at(i)))
        {
            if (!atLeastOne)
            {
                atLeastOne = true;
                modes.clear();
            }

            modes << values.at(i);
        }

    }

}

bool SetupDialog::isValidBand (const QString b)
{
    //qDebug() << "SetupDialog::isValidBand: "<< b << endl;
    QString stringQuery = QString("SELECT id FROM band WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    return query.isValid();

}
bool SetupDialog::isValidMode (const QString b)
{
    //qDebug() << "SetupDialog::isValidMode: " << b << endl;

    QString stringQuery = QString("SELECT id FROM mode WHERE name='%1'").arg(b);
    QSqlQuery query(stringQuery);
    query.next();
    return query.isValid();

}

void SetupDialog::setDefaults()
{
    //qDebug() << "SetupDialog::setDefaults" << endl;
    miscPage->setInMemory("TRUE");
    miscPage->setRealTime("TRUE");
    miscPage->setUTCTime("TRUE");
    miscPage->setImperial("FALSE"); //Metric system is the default
    miscPage->setAlwaysADIF("FALSE");
    miscPage->setSendQSLWhenRec("TRUE");
    miscPage->setShowStationCallSignInSearch("TRUE");
    miscPage->setKeepMyData("TRUE");

    dxClusterPage->setShowHFRadiobutton("TRUE");
    dxClusterPage->setShowVHFRadiobutton("TRUE");
    dxClusterPage->setShowWARCRadiobutton("TRUE");
    dxClusterPage->setShowWorkedRadiobutton("TRUE");
    dxClusterPage->setShowConfirmedRadiobutton("TRUE");
    dxClusterPage->setShowANNRadiobutton("TRUE");
    dxClusterPage->setShowWWVRadiobutton("TRUE");
    dxClusterPage->setShowWCYRadiobutton("TRUE");
    dxClusterServers.clear();
    dxClusterServers.append("dxfun.com:8000");
    dxClusterServerToUse = "dxfun.com:8000";

}

QString SetupDialog::checkAndFixASCIIinADIF(const QString _data)
{
    //qDebug() << "SetupDialog::checkAndFixASCIIinADIF " << _data << endl;
//TODO: this function is also in the FileManager class. Maybe I should call that one and keep just one copy
    int asciiVal;
    QString st = _data;
    QString newString;
    newString.clear();
    for(int i=0; i < st.length(); i++)
    {
    // Get ASCII VALUE into asciiVal
        asciiVal = st.at(i).toAscii();
        if ((32 <= asciiVal ) && (asciiVal <= 126))
        {
            newString.append(st.at(i));
        }
        //qDebug() << "SetupDialog::checkAndFixASCIIinADIF: " << st.at(i) <<" = " << QString::number(asciiVal) << endl;
    }

    // Show into another lineEdit


    return newString;
}

bool SetupDialog::haveAtleastOneLog()
{
    return dataProxy->haveAtLeastOneLog();
}
