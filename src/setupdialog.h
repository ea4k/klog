#ifndef KLOG_SETUPDIALOG_H
#define KLOG_SETUPDIALOG_H
/***************************************************************************
                          setupdialog.h  -  description
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

#include <QDialog>

#include "setuppages/setuppageuserdata.h"
#include "setuppages/setuppagebandmode.h"
#include "setuppages/setuppagemisc.h"
#include "setuppages/setuppagedxcluster.h"
#include "setuppages/setuppagecolors.h"
#include "setuppages/setuppagelogs.h"
#include "setuppages/setuppageworldeditor.h"
#include "setuppages/setuppageelog.h"
#include "setuppages/setuppageudp.h"
#include "setuppages/setuppagesats.h"
#include "setuppages/setuppagehamlib.h"
#include "setuppages/setuppagesubdivisions.h"
#include "setuppages/setuppagelogview.h"
#include "utilities.h"
#include "locator.h"
#include "klogdefinitions.h"


class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    //SetupDialog(DataProxy_SQLite *dp, const bool _firstTime=true, QWidget *parent = nullptr);
    SetupDialog(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~SetupDialog();
    void init(const QString &_softwareVersion, const int _page=0, const bool _alreadyConfigured = true);

    void setData(const QString &_softwareVersion, const int _page = 0, const bool _alreadyConfigured=true);
    void setClubLogActive(const bool _b);
    //void setQRZCOMAutoCheckActive(const bool _b);
    void checkIfNewBandOrMode();
    void setLogLevel(const DebugLogLevel _sev);
    void loadDarkMode(); // Reads the config to setup the DarkMode

signals:
    void exitSignal(const int status); // 1 = OK, -1 = NOK, 2 = Cancel clicked
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    //void qrzcomAuto(bool);
    //void newLogRequested(const bool _s); // true show new log

private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void slotReadConfigData();
    void slotOkButtonClicked();
    void slotCancelButtonClicked();
    void slotAnalyzeNewLogData(const QStringList _qs);  // We receive the station callsign and operators from the logs tab
    void slotSetStationCallSign(const QString &_p);      // We receive te station callsign from the userData tab to fill the new log
    void slotSetOperators(const QString &_p);            // We receive te station operators from the userData tab to fill the new log
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery);
    void slotFocusOK();
    //void slotQRZCOMAuto(const bool _b);

private:
    void showEvent(QShowEvent *event);
    //void setConfigFile(const QString &_configFile);
    void setSoftVersion(const QString &_softwareVersion);
    void setConfigured(const bool _configured);
    void setPage(const int _page);
    void connectActions();
    void saveSettings();
    bool loadSettings();
    void createIcons();
    //bool processConfigLine(const QString &_line);

    void setDefaults();

    void readActiveBands (const QStringList &actives);
    void readActiveModes (const QStringList &actives);
    bool isValidBand (const QString &b);
    bool isValidMode (const QString &b);

    QString checkAndFixASCIIinADIF(const QString &_data);
    bool haveAtleastOneLog();
    void logEvent(const QString &_func, const QString &_msg, DebugLogLevel _level);

    bool firstTime; // To know if we are calling it from the Start wizard or not
    bool nolog; // If there is no log being managed
    QString dxClusterServerToUse;
    QStringList dxClusterServers;

    QPushButton *okButton, *closeButton;
    QTabWidget *tabWidget;
    int logsPageTabN;

    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;

    SetupPageUserDataPage *userDataPage;
    SetupPageBandMode *bandModePage;
    SetupPageDxCluster *dxClusterPage;
    SetupPageMisc *miscPage;
    SetupPageColors *colorsPage;
    SetupPageLogs *logsPage;
    SetupPageWorldEditor *worldEditorPage;
    SetupPageELog *eLogPage;
    //SetupPageClubLog *clubLogPage;
    //SetupPageEQSL *eQSLPage;
    //SetupPageLoTW *lotwPage;
    SetupPageUDP *UDPPage;
    SetupPageSats *satsPage;
    SetupPageHamLib *hamlibPage;
    //SetupPageInterfacesWindows *interfacesWindowsPage;
    SetupPageSubdivisions *subdivisionsPage;
    SetupPageLogView *logViewPage;
    //SetupPageRegionalAwards *regionalAwardsPage;
    int pageRequested; // The page on the Dialog that is requested to be shown when you call it
    //QString klogDir;
    QString version;

    QStringList bands, modes, logViewFields;
    Locator *locator;
    DataProxy_SQLite *dataProxy;

    Utilities *util;
    //QString windowSize;
    //QString latestBackup;

    int constrid; // Just an id for the constructor to check who is being executed at one specific time
    DebugLogLevel logLevel;    // Manages as syslog, the severity of the application debug log
};

#endif // SETUPDIALOG_H
