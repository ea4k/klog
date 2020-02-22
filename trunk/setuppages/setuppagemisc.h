#ifndef SETUPPAGEMISC_H
#define SETUPPAGEMISC_H
/***************************************************************************
                          setuppagemisc.h  -  description
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

//#include <QtWidgets>
#include <QtWidgets>
#include "utilities.h"

class SetupPageMisc : public QWidget {
    Q_OBJECT

public:
    SetupPageMisc(QWidget *parent=nullptr);
    ~SetupPageMisc();

    QString getRealTime();
    void setRealTime(const QString &_t);
    QString getUTCTime();
    void setUTCTime(const QString &_t);
    QString getAlwaysADIF();
    void setAlwaysADIF(const QString &_t);
    QString getDefaultFileName();
    void setUseDefaultName(const QString &_t);
    QString getDefaultDBPath();
    void setUseDefaultDBPath(const QString &_t);
    QString getUseDefaultName();
    void setDefaultFileName(const QString &_t);
    //QString getInMemory();
    //void setInMemory(const QString &_t);
    QString getImperial();
    void setImperial(const QString &_t);
    QString getSendQSLWhenRec();
    void setSendQSLWhenRec(const QString &_t);
    QString getShowStationCallSignInSearch();
    void setShowStationCallSignInSearch(const QString &_t);
    QString getKeepMyData();
    void setKeepMyData(const QString &_t);
    QString getCompleteWithPrevious();
    void setCompleteWithPrevious(const QString &_t);
    QString getCheckNewVersions();
    void setCheckNewVersions(const QString &_t);
    QString getReportInfo();
    void setReportInfo(const QString &_t);
    QString getDXMarathon();
    void setDXMarathon(const QString &_t);
    QString getDebugLog();
    void setDebugLog(const QString &_t);
    QString getLogSort();
    void setLogSort(const QString &_t);
    QString getSendEQSLByDefault();
    void setSetEQSLByDefault(const QString &_t);

    bool areDBPathChangesApplied();

private slots:
    void slotOpenFileButtonClicked();
    void slotUseDefaultButtonStateChanged(int state);
    //void slotdefaultFileNameChanged();
    void slotDefaultFileNameLineEditChanged();
    void slotcheckNewVersionCheckBoxClicked();
    void slotDBButtonClicked();
    void slotDBLineEditChanged();
    void slotMoveDBButtonClicked();
    //void slotUDPServerCheckBoxClicked();

private:
    void createActions();
    void createUI();

    Utilities *util;

    QCheckBox *realTimeCheckbox, *UTCCheckbox, *alwaysADIFCheckBox, *useDefaultName, *completeWithPreviousCheckBox;
    QCheckBox *imperialCheckBox, *sendQSLWhenRecCheckBox, *showStationCallWhenSearchCheckBox, *keepMyDataCheckBox;
    QCheckBox *checkNewVersionCheckBox, *provideCallCheckBox, *useDxMarathonCheckBox, *debugLogCheckBox;
    QCheckBox *logSortCheckBox;
    QCheckBox *sendEQSLByDefaultSearchCheckBox;
    QString defaultFileName;
    QLineEdit *defaultFileNameLineEdit, *dbPathLineEdit;
    QPushButton *fileNameButton, *dbPushButton, *moveDBPushButton;


    QString klogDir, dbDirNew, dbDirCurrent; //TODO: To be removed when the defaultDir is saved in the config file
    QPalette palRight, palWrong; // To paint Text in red or black(normal)

    bool dbPathApplied;

    //QWidget *bandsWidget;

};


#endif // SETUPPAGEMISC_H
