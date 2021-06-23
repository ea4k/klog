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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
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

    bool getRealTime();
    void setRealTime(const bool &_t);
    bool getUTCTime();
    void setUTCTime(const bool &_t);
    bool getAlwaysADIF();
    void setAlwaysADIF(const bool &_t);
    QString getDefaultFileName();
    void setUseDefaultName(const bool &_t);
    QString getDefaultDBPath();
    void setUseDefaultDBPath(const QString &_t);
    bool getUseDefaultName();
    void setDefaultFileName(const QString &_t);
    //QString getInMemory();
    //void setInMemory(const QString &_t);
    bool getImperial();
    void setImperial(const bool &_t);
    bool getSendQSLWhenRec();
    void setSendQSLWhenRec(const bool &_t);
    bool getShowStationCallSignInSearch();
    void setShowStationCallSignInSearch(const bool &_t);
    //QString getKeepMyData();
    //void setKeepMyData(const QString &_t);
    bool getCompleteWithPrevious();
    void setCompleteWithPrevious(const bool &_t);
    bool getCheckNewVersions();
    void setCheckNewVersions(const bool &_t);
    bool getReportInfo();
    void setReportInfo(const bool &_t);
    bool getDXMarathon();
    void setDXMarathon(const bool &_t);
    bool getDebugLog();
    void setDebugLog(const bool &_t);
    //QString getLogSort();
    //void setLogSort(const QString &_t);
    bool getSendEQSLByDefault();
    void setSetEQSLByDefault(const bool &_t);
    void setDeleteAlwaysAdiFile(const bool &_t);
    bool getDeleteAlwaysAdiFile();

    void setDupeTime(const int _t);
    int getDupeTime();

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
    QCheckBox *imperialCheckBox, *sendQSLWhenRecCheckBox, *showStationCallWhenSearchCheckBox;
    QCheckBox *checkNewVersionCheckBox, *provideCallCheckBox, *useDxMarathonCheckBox, *debugLogCheckBox;
    //QCheckBox *logSortCheckBox;
    QCheckBox *sendEQSLByDefaultSearchCheckBox, *deleteAlwaysAdiFileCheckBox;
    QString defaultFileName;
    QLineEdit *defaultFileNameLineEdit, *dbPathLineEdit;
    QPushButton *fileNameButton, *dbPushButton, *moveDBPushButton;
    QLineEdit *dupeTimeLineEdit;

    QString klogDir, dbDirNew, dbDirCurrent; //TODO: To be removed when the defaultDir is saved in the config file
    QPalette palRight, palWrong; // To paint Text in red or black(normal)

    bool dbPathApplied;

    //QWidget *bandsWidget;

};


#endif // SETUPPAGEMISC_H
