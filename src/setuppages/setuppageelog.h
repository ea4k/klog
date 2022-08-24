#ifndef SETUPPAGEELOG_H
#define SETUPPAGEELOG_H
/***************************************************************************
                          setuppageelog.h  -  description
                             -------------------
    begin                : oct 2020
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


#include <QtWidgets>
#include "../utilities.h"

class SetupPageELog : public QWidget {
    Q_OBJECT

public:
    SetupPageELog(QWidget *parent=nullptr);
    ~SetupPageELog();

    //Clublog
    void setClubLogEmail(const QString &c);
    void setClubLogPassword(const QString &c);
    void setClubLogActive(const bool &_b);
    void setClubLogRealTime(const bool &_s);

    QString getClubLogEmail();
    QString getClubLogPassword();
    bool getClubLogActive();
    bool getClubLogRealTime();


    // LoTW
    void setTQSLPath(const QString &c);
    QString getTQSLPath();
    void setLoTWActive(const bool &_s);
    bool getLoTWActive();
    void setLoTWUser(const QString &_s);
    QString getLoTWUser();
    void setLoTWPass(const QString &_s);
    QString getLoTWPass();

    // eQSL
    void setEQSLUser(const QString &c);
    void setEQSLPassword(const QString &c);
    void setEQSLActive(const bool _b);
    //void setEQSLRealTime(const QString &_s);

    QString getEQSLUser();
    QString getEQSLPassword();
    bool getEQSLActive();
    //QString getEQSLRealTime();

    //QRZ.COM
    void setQRZCOMUser(const QString &_s);
    void setQRZCOMPassword(const QString &_s);

    void setQRZCOMLogBookKEY(const QString &_s);
    void setQRZCOMActive(const bool _s);
    void setQRZCOMAutoCheck(const bool _s);

    QString getQRZCOMUser();
    QString getQRZCOMPassword();

    QString getQRZCOMLogBookKEY();
    bool getQRZCOMActive();
    bool getQRZCOMAutoCheck();

    bool getQRZCOMSubscriber();
    void setQRZCOMSubscriber(const bool _s);


private slots:

    void slotClubLogActive(const bool _s);
    void slotEQSLActive(const bool _s);
    void slotTQSLActive(const bool _s);
    void slotQRZCOMActive(const bool _s);
    void slotQRZCOMAuto(const bool _s);
    void slotQRZCallTextChanged();

    void slotSelectTQSLClicked();
    void slotPathLineEditChanged(const QString &_q);
    void slotEnterKeyPressed();
    void slotClubLogEmailDefineColor();
    void slotLoTWEmailDefineColor();
    void sloteQSLCallTextChanged();

signals:
    void enterKey();
    void qrzcomAuto(bool _s);

protected:
    //void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
private:
    void setDefaults();
    //QString defaultFileName;
    QGroupBox *clubLogGroup;
    QCheckBox *clubLogSendInRealTimeCheckBox, *clubLogActiveCheckBox;//, *useQSOStationCallCheckBox;
    QLineEdit *clubLogPasswordLineEdit, *clubLogEmailLineEdit;//, *callLineEdit;
    QLabel *clubLogPasswordLabel, *clubLogEmailLabel; //*callLabel,
    QString clubLogEmail, clubLogPass;
    bool clubLogActive, clubLogRealTime;

    QGroupBox *eQSLccGroup;
    QLineEdit *eQSLUserLineEdit, *eQSLPasswordLineEdit;
    QCheckBox *eQSLActiveCheckBox; //*eQSLSendInRealTimeCheckBox,
    QLabel *eQSLpasswordLabel, *eQSLemailLabel;
    bool eqslActive;
    QString eqslUser, eqslPass;

    QGroupBox *lotwGroup, *lotwUpGroup, *lotwDownGroup;
    QLineEdit *lotwUserLineEdit, *lotwPasswordLineEdit;
    QLineEdit *lotwTQSLPathLineEdit;
    QPushButton *lotwSearchTQSLPushButton;
    QCheckBox *lotwUseTQSLCheckBox;
    QLabel *lotwpasswordLabel, *lotwemailLabel;
    bool lotwTQSL;
    QString lotwPath, lotwUser, lotwPass;

    QGroupBox *QRZCOMGroup;
    //QLineEdit *QRZCOMAPILineEdit;
    QLineEdit *QRZCOMUserLineEdit, *QRZCOMPasswordLineEdit, *QRZCOMLogBookKEYLineEdit;
    QLabel *QRZCOMPasswordLabel, *QRZCOMUserLabel, *QRZLogBookKeyLabel;
    QCheckBox *QRZCOMActiveCheckBox, *QRZCOMAutoCheckCheckBox, *QRZCOMSubscriberCheckBox;

    bool qrzcomActive, qrzcomAutoFill;
    QString qrzComUser, qrzcomPass, qrcomLogbookKey;

    Utilities *util;

    QPalette palRed, palBlack;


};

#endif // SETUPPAGEELOG_H
