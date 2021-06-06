#ifndef MAINQSOENTRYWIDGET_H
#define MAINQSOENTRYWIDGET_H
/***************************************************************************
                          mainqsoentrywidget.h  -  description
                             -------------------
    begin                : january 2020
    copyright            : (C) 2020 by Jaime Robles
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
#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include "dataproxy_sqlite.h"
#include "utilities.h"

class MainQSOEntryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainQSOEntryWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    void setBands(const QStringList _bands);
    void setModes(const QStringList _modes);

    bool setBand(const QString &_band);
    bool setFreq(const double _f, bool isRX = false);

    bool setMode(const QString &_mode);
    bool setQRZ(const QString &_qrz);
    void setCurrentQRZ(const QString &_qrz);
    bool setDate(const QDateTime _date);
    bool setTime(const QTime _time);
    void setCleaning (const bool _c);
    bool isModeExisting(const QString &_m);
    bool isBandExisting(const QString &_b);

    QString getQrz();
    QString getBand(const int _b=-1);
    QString getMode(const int _m=-1);
    QStringList getModes();
    QDate getDate();
    QTime getTime();
    QDateTime getDateTime();

    void setRealTime(const bool _realTime);
    bool getRealTime();
    void toggleRealTime();
    void setUTC(const bool _utc);
    void setModify(const bool _modify);
    void setUpAndRunning(const bool _u);
    void selectDefaultBand(const bool _init = false);
    void selectDefaultMode(const bool _init = false);

    void setDuplicatedQSOSlot (const int _secs);

    void clear();

protected:
   // void keyPressEvent(QKeyEvent *event);

signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    void showInfoLabel(QString _msg);
    void currentQRZSignal(QString _msg);
    void clearForNextQSOSignal();
    void bandChanged(QString _band);
    void modeChanged(QString _mode);
    void OKClicked();
    void validBands(QStringList _bands);

private slots:
    void slotUpdateTime();
    void slotQRZTextChanged();
    void slotBandComboBoxChanged(const QString _b);
    void slotModeComboBoxChanged(const QString _m);
    void slotOKButtonClicked();
    void slotClearButtonClicked();
    void slotCheckBoxClicked();
    //void slotRealTimeCheckBoxChanged();
    void slotStartDelayInputTimer();
    void slotDelayInputTimedOut();


private:
    void createUI();
    void setInitialData();

    bool validCharactersInCall(const QString &_qrz);
    void clearForNextQSO();
    void checkIfDupe(const QString &_func);
    void setDateAndTimeInternally();
    bool updateBandComboBox(const QString &_band); // If a new band arrives, we add it if possible to KLog
    bool newBandNeededForFreq(const double _f);
    DataProxy_SQLite *dataProxy;
    QGroupBox *qrzgroupBox;//, *searchgroupBox;
    QLineEdit *qrzLineEdit;
    QComboBox *bandComboBox, *modeComboBox;
    QDateEdit *dateEdit;
    QTimeEdit *timeEdit;
    QPushButton *OKButton, *clearButton;
    QCheckBox *realtimeCheckBox;
    //DebugLogLevel logSeverity;
    bool cleaning;
    bool qrzAutoChanging;
    bool InValidCharsInPrevCall;
    bool qrzSmallModDontCalculate;
    bool upAndRunning;

    QString previousQRZ;
    QString currentQrz;


    QStringList bands, modes;

    QTimer *timer;
    bool UTCTime, modify, realTime;
    QPalette palRed, palBlack; // To paint Text in red or black(normal)
    Utilities *util;
    QPalette::ColorRole enabledCR, disabledCR;

    int duplicatedQSOSlotInSecs;
    QTimer *delayInputTimer;
    QString lastQrz;
    double freqTX, freqRX, bottomBandLimit, upperBandLimit;

};

#endif // MAINQSOENTRYWIDGET_H
