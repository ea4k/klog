#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H
/***************************************************************************
                          setupdialog.h  -  description
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

#include <QDialog>

#include "setuppageuserdata.h"
#include "setuppagebandmode.h"
#include "setuppagemisc.h"
#include "setuppagedxcluster.h"
#include "setuppagecolors.h"
#include "setuppagelogs.h"
#include "setuppageworldeditor.h"
#include "setuppageclublog.h"
#include "setuppageudp.h"
#include "setuppagesats.h"
#include "setuppagehamlib.h"
#include "utilities.h"


#include "locator.h"


class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    SetupDialog(DataProxy *dp, const bool _firstTime=true);
    SetupDialog(DataProxy *dp, const QString _configFile, const QString _softwareVersion, const int _page=0, const bool _firstTime = true);
    ~SetupDialog();

    void setData(const QString _configFile, const QString _softwareVersion, const int _page, const bool _firstTime=true);
    void setClubLogActive(const bool _b);
    void checkIfNewBandOrMode();
    void setSeverity(const int _sev);

public slots:

signals:
    void exitSignal(const int status); // 1 = OK, -1 = NOK, 2 = Cancel clicked
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution
    void debugLog (QString _func, QString _msg, int _level);
    //void newLogRequested(const bool _s); // true show new log

private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void slotReadConfigData();
    void slotOkButtonClicked();
    void slotCancelButtonClicked();
    void slotAnalyzeNewLogData(const QStringList _qs);  // We receive the station callsign and operators from the logs tab
    void slotSetStationCallSign(const QString _p);      // We receive te station callsign from the userData tab to fill the new log
    void slotSetOperators(const QString _p);            // We receive te station operators from the userData tab to fill the new log
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery);

private:

    void setConfigFile(const QString _configFile);
    void setSoftVersion(const QString _softwareVersion);
    void setConfigured(const bool _configured);
    void setPage(const int _page);
    void connectActions();

    void createIcons();
    bool processConfigLine(const QString _line);

    void setDefaults();

    void readActiveBands (const QString actives);
    void readActiveModes (const QString actives);
    bool isValidBand (const QString b);
    bool isValidMode (const QString b);

    QString checkAndFixASCIIinADIF(const QString _data);
    bool haveAtleastOneLog();

    bool firstTime; // To know if we are calling it from the Start wizard or not
    bool nolog; // If there is no log being managed
    QString dxClusterServerToUse;
    QStringList dxClusterServers;

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
    SetupPageClubLog *clubLogPage;
    SetupPageUDP *UDPPage;
    SetupPageSats *satsPage;
    SetupPageHamLib *hamlibPage;

    int pageRequested; // The page on the Dialog that is requested to be shown when you call it
    //QString klogDir;
    QString configFileName, version;

    QStringList bands, modes;
    Locator *locator;
    DataProxy *dataProxy;

    Utilities *util;

    int constrid; // Just an id for the constructor to check who is being executed at one specific time
    int logSeverity;    // Manages as syslog, the severity of the application debug log
};


#endif // SETUPDIALOG_H
